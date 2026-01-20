#!/usr/bin/python
# -*- coding: UTF-8 -*-
from platform_common import *

PCA9548START  = -1
PCA9548BUSEND = -2

RUIJIE_CARDID      = 0x00004075
RUIJIE_PRODUCTNAME = "AC51-48C2G"
RUIJIE_PART_NUMBER    = "RJ000001"
RUIJIE_LABEL_REVISION = "R01"
RUIJIE_ONIE_VERSION   = "2018.02"
RUIJIE_MAC_SIZE       = 3
RUIJIE_MANUF_NAME     = "Ruijie"
RUIJIE_MANUF_COUNTRY  = "CHN"
RUIJIE_VENDOR_NAME    = "Alibaba"
RUIJIE_DIAG_VERSION   = "0.1.0.15"
RUIJIE_SERVICE_TAG    = "www.ruijie.com.cn"


LOCAL_LED_CONTROL = {

}



MACLED_PARAMS = [{
             "cmdwrformat": "bsp_init.py pci wr  8 0 0  0 64  %02x",
             "cmdrdformat": "bsp_init.py pci rd  8 0 0  0 64",
             "reset":0x98,
             "yellowflash":0x85,
             "greenflash":0x83,
             "yellow":0x86,
             "green":0x84
}]

# start system modules
STARTMODULE  = {
                "fancontrol":0,
                "avscontrol":0,
                "avscontrol_restful":0,
                "sfptempmodule":0,
                "sfptempmodule_interval":3,
                "macledreset": 0,
                "macledreset_interval": 5,
                "macledset_param":MACLED_PARAMS,
                "irq_affinity_set": 1,
                }

FRULISTS = [
            {"name":"bmceeprom","bus":4,"loc":0x53,"E2PRODUCT":'6', "E2TYPE":'3', "CANRESET":'1' },
            {"name":"cpueeprom","bus":4,"loc":0x57,"E2PRODUCT":'2', "E2TYPE":'4', "CANRESET":'1' },
            {"name":"psu1","bus":24,"loc":0x50, "E2TYPE":'c',"CANRESET":'0' },
            {"name":"psu2","bus":25,"loc":0x50, "E2TYPE":'c',"CANRESET":'0' },
        ]

# rg_eeprom  = "0-0056/eeprom"
E2_LOC = {"bus":0, "devno":0x56}
E2_PROTECT = {"bus":1, "devno":0x0d, "addr":0x19, "open":0, "close":0xff}
FAN_PROTECT = {"bus":1, "devno":0x0d, "addr":0x19, "open":0x00, "close":0xff}


CPLDVERSIONS = [
        {"name":"CPU_MODULE_CPLD", "gettype":"lpc"},
        {"name":"CPU_BOARD_CPLD", "gettype":"restful", "url":"/xyz/openbmc_project/version/attr/CpldBoardVersion"},
        #{"bus":1, "devno":0x0d, "name":"CPU底板CPLD-FAN"},
        #{"bus":5, "devno":0x0d, "name":"CPU底板CPLD"},
]

FIRMWARE_TOOLS = {"cpld":  [{"channel":"0","cmd":"firmware_upgrade %s cpld %s cpld", "successtips":"CPLD Upgrade succeeded!"}
                           ],
                "fpga": [{"channel":"1","cmd":"firmware_upgrade %s fpga %s fpga", "successtips":"FPGA Upgrade succeeded!"}],
                "bmc": "",
                "bios": "",
                "cpldinterface": {
                    "all":             {"name":"as13-32h_cpld_all_cpu_pwr", "needrefresh":1},
                    "CPU_BOARD_CPLD":  {"name":"as13-32h_cpld_1_cpu",       "needrefresh":1},
                    "CPU_MODULE_CPLD": {"name":"as13-32h_cpld_2_cpu",       "needrefresh":1},
                    "MAC_BOARD_CPLD_1":{"name":"as13-32h_cpld_3_cpu"},
                    "MAC_BOARD_CPLD_2":{"name":"as13-32h_cpld_4_cpu"},
                    "refresh":         {"name":"as13-32h_cpld_transfr_bmc"},
                },
                "cpldprogram": {
                    "CPU_BOARD_CPLD":  {"name":"as13-32h_cpld_1_cpu_pwr",   "needrefresh":1},
                    "CPU_MODULE_CPLD": {"name":"as13-32h_cpld_2_cpu_pwr",   "needrefresh":1},
                    "MAC_BOARD_CPLD_1":{"name":"as13-32h_cpld_3_cpu_pwr",   "needrefresh":1},
                    "MAC_BOARD_CPLD_2":{"name":"as13-32h_cpld_4_cpu_pwr",   "needrefresh":1},
                },
                "refresh": {
                    "CPU_BOARD_CPLD":  {"name":"as13-32h_cpld_1_transfr_bmc"},
                    "CPU_MODULE_CPLD": {"name":"as13-32h_cpld_2_transfr_bmc"},
                    "MAC_BOARD_CPLD_1":{"name":"as13-32h_cpld_3_transfr_bmc"},
                    "MAC_BOARD_CPLD_2":{"name":"as13-32h_cpld_4_transfr_bmc"},
                    "FPGA":            {"name":"fpga_transfr_bmc"},
                    "BIOS":            {"name":"bios_transfr_bmc"},
                },
              }

# drivers list
DRIVERLISTS = [
        {"name":"wb_i2c_i801", "delay":0, "reload": 0},
        {"name":"r8168", "delay":0, "reload": 0},
        {"name":"igb", "delay":0, "reload": 0},
        {"name":"i2c_dev", "delay":0},
        {"name":"i2c_algo_bit","delay":0},
        {"name":"i2c_mux", "delay":0},
        {"name":"i2c_mux_pca954x force_create_bus=1", "delay":0}, # force_deselect_on_exit=1
        {"name":"ruijie_common dfd_my_type=0x4075", "delay":0},
        {"name":"fpga_i2c_ocores", "delay":0},
        {"name":"fpga_pcie_uart", "delay":0},
        {"name":"fpga_uart_ocores", "delay":0},
        {"name":"i2c_gpio", "delay":0},
        {"name":"rg_gpio_xeon", "delay":1},
        {"name":"at24", "delay":0},
        {"name":"rg_cpld", "delay":0},
        {"name":"optoe", "delay":0},
        {"name":"lpc_dbg", "delay":0},
        {"name":"firmware_gpio_driver", "delay":0},
        {"name":"irq_interface", "delay":0},
]

DEVICE = [
        {"name":"24c02","bus":0,"loc":0x56 },
        {"name":"rg_cpld","bus":5,"loc":0x30 },
]

INIT_PARAM = [
    {"loc": "5-0030/sfp_protect0", "value": "0x59"},
    {"loc": "5-0030/sfp_txdis0", "value": "0x00"},
    {"loc": "5-0030/sfp_protect0", "value": "0x4e"},
]

INIT_COMMAND = [
    "hw_test.bin lpc_cpld_wr8 0x00010092 0x01",
    "hw_test.bin lpc_cpld_wr8 0x00010094 0x01",
    "balance_affinity.sh"
]

IRQ_AFFINITY_CONFIG = [
    {"irqs":"/sys/bus/pci/devices/0000:08:00.0/msi_irqs/", "cpu_affinity":["020"], "way":"pcie"},
    {"irqs":"/sys/bus/pci/devices/0000:09:00.0/msi_irqs/", "cpu_affinity":["010"], "way":"pcie"},
    {"irqs":"/sys/bus/pci/devices/0000:0a:00.0/msi_irqs/", "cpu_affinity":["008", "008","008","008","008"], "way":"pcie"},
    {"irqs":"/sys/bus/pci/devices/0000:0b:00.0/msi_irqs/", "cpu_affinity":["004", "004","004","004","004"], "way":"pcie"}
    #{"irqs":"xxxx", "cpu_affinity":["xxx"], "way":"pcie"}
]

INIT_COMMAND_STAGE2 = [
    "ethtool -G eth3 tx 4096 rx 4096",
    "ethtool -G eth4 tx 4096 rx 4096"
]
