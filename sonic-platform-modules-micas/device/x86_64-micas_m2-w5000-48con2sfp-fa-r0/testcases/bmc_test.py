from connectbmc import ConnectBmc
import unittest
import re
import subprocess

class BmcTest(unittest.TestCase):
    is_x86 = False

    @classmethod
    def setUpClass(cls):
        ret, log = subprocess.getstatusoutput("arch")
        cls.is_x86 = True if "x86_64" in log else False

    def test_connect_to_bmc(self):
        """Test whether BMC can be accessed normally"""
        ret, log = ConnectBmc.wait_bmc_ready()
        self.assertTrue(ret, "Abnormal: %s" % log)
        print("BMC is accessible")

    def test_get_bmc_version(self):
        """Get the currently running BMC version"""
        ret, version = ConnectBmc.exec_bmc_cmd("cat /etc/os-release")
        self.assertTrue(ret, "Failed to get BMC os-release: %s" % version)
        print(version)

    def test_get_dev_baudrate(self):
        """Obtain BMC baudrate"""
        ret, env_baudrate_log = ConnectBmc.exec_bmc_cmd("fw_printenv baudrate")
        self.assertTrue(ret, "Failed to get env baudrate: %s" % env_baudrate_log)
        env_baudrate = env_baudrate_log.split("=")[1] if "=" in env_baudrate_log else "NA"
        print("BMC env baudrate: %s" % env_baudrate)
        self.assertIn(env_baudrate, ["9600", "115200"], "Invalid baudrate: %s" % env_baudrate_log)
        for tty_idx in range(0, 5):
            tty_dev = "/dev/ttyS%d" % tty_idx
            ret, tty_baudrate = ConnectBmc.exec_bmc_cmd("stty -F %s speed" % tty_dev)
            self.assertTrue(ret, "Failed to get %s baudrate: %s" % (tty_dev, tty_baudrate))
            self.assertEqual(tty_baudrate, env_baudrate, \
                "Abnormal %s baudrate %s(env baudrate %s)" % (tty_dev, tty_baudrate, env_baudrate))
            print("BMC %s baudrate: %s" % (tty_dev, tty_baudrate))

    def test_get_console_monitor_config(self):
        """Get BMC serial port configuration"""
        reg_addr = "0x1E78909C"
        expect_val = "0x02000003"
        ret, reg_val = ConnectBmc.exec_bmc_cmd("devmem %s 32" % reg_addr)
        self.assertTrue(ret, "Failed to get %s: %s" % (reg_addr, reg_val))
        self.assertEqual(reg_val.lower(), expect_val.lower(), "%s != %s" % (reg_val, expect_val))
        print("Console configure(%s): %s" % (reg_addr, reg_val))

    def test_get_postcode_monitor_mode(self):
        """Get BIOS Postcode Configuration"""
        if not self.is_x86:
            print("CPU PLATFROM is not X86, unsupport")
            return
        service = "bios_post_code_monitor.service"
        mode_type = "/sys/devices/platform/ast-snoop-dma.0/mode"
        mode_enable = "/sys/devices/platform/ast-snoop-dma.0/enable"
        mode_type_set = "/usr/local/bin/ast-functions ast_product_func_call get_post_data_mode; echo $?"
        ret, status = ConnectBmc.exec_bmc_cmd("systemctl status %s |grep Active" % service)
        self.assertTrue(ret, "Failed to get %s status: %s" % (service, status))
        self.assertIn("active (running)", status, "Abnormal %s status: %s" % (service, status))
        print("%s status: %s" % (service, status))
        ret, type_log = ConnectBmc.exec_bmc_cmd("cat %s" % mode_type)
        self.assertTrue(ret, "Failed to get mode type: %s" % type_log)
        ret, type_set_log = ConnectBmc.exec_bmc_cmd(mode_type_set)
        self.assertIn("%s [" % type_set_log, type_log, \
            "Abnormal postcode mode: %s, excepted: %s" % (type_log, type_set_log))
        print("Postcode mode: %s" % type_log)
        ret, enable_log = ConnectBmc.exec_bmc_cmd("cat %s" % mode_enable)
        self.assertTrue(ret, "Failed to get mode enable: %s" % enable_log)
        self.assertIn("Enable", enable_log, "Abnormal postcode enable: %s" % enable_log)
        print("Postcode enable: %s" % enable_log)

    def test_get_mac_addr(self):
        """Obtain BMC MAC address"""
        plus_num_cmd = "/usr/local/bin/ast-functions ast_product_func_call plus_mac_num; echo $?"
        bmc_mac_cmd = "ifconfig eth0 |grep HWaddr"
        cpu_mac_cmd = "show platform syseeprom |grep \"Base MAC Address\""
        ret, plus_num = ConnectBmc.exec_bmc_cmd(plus_num_cmd)
        self.assertTrue(ret, "Failed to get plus mac num: %s" % plus_num)
        self.assertTrue(plus_num.isdigit() and 0 < int(plus_num) <= 2, \
            "Invalid plus mac num: %s" % plus_num)
        ret, bmc_mac_log = ConnectBmc.exec_bmc_cmd(bmc_mac_cmd)
        self.assertTrue(ret, "Failed to get BMC mac: %s" % bmc_mac_log)
        pattern= "([a-fA-F0-9]{2}(:[a-fA-F0-9]{2}){5})"
        bmc_mac_match = re.search(pattern, bmc_mac_log)
        self.assertTrue(bmc_mac_match is not None, "BMC mac address not found: %s" % bmc_mac_log)
        bmc_mac = bmc_mac_match.group(1)
        print("BMC MAC: %s" % bmc_mac)
        ret, cpu_mac_log = subprocess.getstatusoutput(cpu_mac_cmd)
        self.assertEqual(ret, 0, "Failed to get CPU mac: %s" % cpu_mac_log)
        cpu_mac_match = re.search(pattern, cpu_mac_log)
        self.assertTrue(cpu_mac_match is not None, "CPU mac address not found: %s" % cpu_mac_log)
        cpu_mac = cpu_mac_match.group(1)
        bmc_mac_right = re.sub('(\w\w)(?=\w)',"\g<1>:", \
            '{:012x}'.format(int((int("0x"+cpu_mac.replace(":",""), 16) + int(plus_num)) & 0xffffffffffff)))
        self.assertTrue(bmc_mac_right.lower() == bmc_mac.lower(), \
            "BMC mac %s, expected: %s" % (bmc_mac, bmc_mac_right))

    def test_get_phy_port_status(self):
        """Obtain the status of all links in phy"""
        phy_status_cmd = "phy.sh show_port"
        ret, phy_status_log = ConnectBmc.exec_bmc_cmd(phy_status_cmd)
        self.assertTrue(ret, "Failed to phy status: %s" % phy_status_log)
        if "unsupport" in phy_status_log:
            print(phy_status_log)
            return
        phy_status = phy_status_log.split("\n")
        for port_status in phy_status:
            status = port_status.split(":")[1].strip() if ":" in port_status else "NA"
            self.assertIn(status, ["up", "down"], "Unknow: %s" % port_status)
            print(port_status)

    def test_get_productname(self):
        """Obtain device related names"""
        productname = "productname"
        conffitname = "conffitname"
        fullproductname = "full%s" % productname
        productname_path = "/etc/device/{}"
        env_cmd = "fw_printenv"
        ret, env_log = ConnectBmc.exec_bmc_cmd(env_cmd)
        self.assertTrue(ret, "Failed to env variable: %s" % env_log)
        product_info = dict(re.findall("(%s|%s)=(\S+)$" % (productname, conffitname), env_log, re.M))
        env_fullproductname = product_info.get(productname)
        env_conffitname = product_info.get(conffitname)
        self.assertTrue(env_fullproductname and env_conffitname, "Abnormal env variable: %s" % env_log)
        env_productname = "-".join(env_fullproductname.split("-")[:2])
        name_list = [fullproductname, productname, conffitname]
        for name in name_list: 
            ret2, file_name = ConnectBmc.exec_bmc_cmd("cat %s" % productname_path.format(name))
            self.assertEqual(locals().get("env_%s" % name), file_name, \
                "\"%s\" != \"%s\"" % (locals().get("env_%s" % name), file_name))
            print("%s: %s" % (name, file_name))

    def test_get_mac_rov_status(self):
        """Check the current Mac rov status"""
        cfg_cmd = "rj_rawcmd_cli get device_cfg"
        ret, cfg = ConnectBmc.exec_bmc_cmd(cfg_cmd)
        self.assertIsInstance(cfg, dict, "Failed to get device cfg: %s" % cfg)
        bmc_cfg_list = cfg.get("bmcs", [])
        self.assertIsInstance(bmc_cfg_list[0], dict, "Failed to get bmc cfg: %s" % bmc_cfg_list)
        reg_dict = bmc_cfg_list[0].get("VidRovReg")
        if reg_dict is None:
            print("unsupport Mac rov")
            return
        self.assertTrue(isinstance(reg_dict, dict) and isinstance(reg_dict.get("value"), dict), \
            "Abnormal mac reg cfg: %s" % reg_dict)
        vout_dict = bmc_cfg_list[0].get("AvsVoutCommand")
        self.assertTrue(isinstance(vout_dict, dict) and isinstance(vout_dict.get("value"), dict) and \
            vout_dict.get("value").get("loc"), "Abnormal mac vout cfg: %s" % vout_dict)
        vid_map_dict = bmc_cfg_list[0].get("VidTransform")
        self.assertTrue(isinstance(vid_map_dict, dict), "Abnormal mac vid map cfg: %s" % vid_map_dict)
        reg = reg_dict.get("value")
        get_reg_val = "i2cget -f -y %d 0x%02x 0x%02x" % (reg.get("bus"), reg.get("addr"), reg.get("offset"))
        ret, log = ConnectBmc.exec_bmc_cmd(get_reg_val)
        self.assertTrue(ret and "0x" in log, "Failed to get reg val(%s): %s" % (get_reg_val, log))
        print("Mac register value: %s" % log)
        reg_val = int(log, 16)
        vid_val = vid_map_dict.get("%s" % reg_val)
        self.assertIsInstance(vid_val, int, "Failed to get vid map value, reg_value: %s, map: %s" \
            % (reg_val, vid_map_dict))
        print("Mac vid value: 0x%x" % vid_val)
        get_vout_val = "cat %s" % vout_dict.get("value").get("loc")
        ret, log = ConnectBmc.exec_bmc_cmd(get_vout_val)
        self.assertTrue(ret and "0x" in log, "Failed to get vout val(%s): %s" % (get_vout_val, log))
        print("Mac vout value: %s" % log)
        vout_val = int(log, 16)
        self.assertEqual(vid_val, vout_val, "Abnormal Mac rov: vid value != vout value")
        print("Mac rov has been done")

    def test_get_ready_status(self):
        """Get BMC ready register status"""
        ready_cmd = "i2cget -f -y 0 0x0d 0x1a"
        ready_mask = 0x08
        ret, ready_status = ConnectBmc.exec_bmc_cmd(ready_cmd)
        self.assertTrue(ret and "0x" in ready_status, "Failed to BMC ready status: %s" % ready_status)
        self.assertNotEqual(int(ready_status, 16) & ready_mask, 0, "BMC ready: abnormal, %s" % ready_status)
        print("BMC ready ok: %s" % ready_status)

    def test_get_cpu_error_gpio(self):
        """Get MCE GPIO configuration"""
        if not self.is_x86:
            print("CPU PLATFROM is not X86, unsupport")
            return
        service = "cpu-error-gpio.service"
        gpio_path = "/sys/class/gpio/"
        expect_gpio_list = ["gpio72", "gpio73", "gpio74", "gpio75"]
        ret, service_status = ConnectBmc.exec_bmc_cmd("systemctl status %s |grep Active" % service)
        self.assertTrue(ret, "Failed to get %s status: %s" % (service, service_status))
        print("%s status: %s" % (service, service_status))
        self.assertIn("active (running)", service_status, \
            "Abnormal %s status: %s" % (service, service_status))
        ret, gpio_status = ConnectBmc.exec_bmc_cmd("ls %s" % gpio_path)
        self.assertTrue(ret, "Failed to get gpio status: %s" % gpio_status)
        real_gpio_list = [str(gpio) for gpio in re.findall("^(gpio\d+)$", gpio_status, re.M)]
        self.assertEqual(sorted(expect_gpio_list), sorted(real_gpio_list), \
            "real gpios: %s, expected: %s" % (real_gpio_list, expect_gpio_list))
        print("Gpios: %s" % real_gpio_list)
