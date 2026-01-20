# serialutil.py
#
# Platform-specific Serial interface for SONiC
#

try:
    import os
    import subprocess
except ImportError as e:
    raise ImportError("%s - required module not found" % str(e))

def subprocess_os_cmd(cmd):
    if len(cmd) <= 0:
        return -1
    p = subprocess.Popen(cmd,shell=True)
    p.wait()
    returncode = p.returncode
    if returncode !=0:
        return -1
    else:
        return 0

class SerialUtil(object):
    MIN_CONSOLE = 1
    MAX_CONSOLE = 48
    serial_pci_path = "/sys/bus/pci/devices/0000:01:00.0/"
    serial_platform_path = "/sys/bus/platform/devices/rg-uart.%d/tty/ttyRG%d/"
    console_stty_cmd = "sudo stty -F /dev/ttyRG%d "
    CONSOLE_TO_DEVICE_MAP = {
        "1":"/dev/ttyRG0",
        "2":"/dev/ttyRG1",
        "3":"/dev/ttyRG2",
        "4":"/dev/ttyRG3",
        "5":"/dev/ttyRG4",
        "6":"/dev/ttyRG5",
        "7":"/dev/ttyRG6",
        "8":"/dev/ttyRG7",
        "9":"/dev/ttyRG8",
        "10":"/dev/ttyRG9",
        "11":"/dev/ttyRG10",
        "12":"/dev/ttyRG11",
        "13":"/dev/ttyRG12",
        "14":"/dev/ttyRG13",
        "15":"/dev/ttyRG14",
        "16":"/dev/ttyRG15",
        "17":"/dev/ttyRG16",
        "18":"/dev/ttyRG17",
        "19":"/dev/ttyRG18",
        "20":"/dev/ttyRG19",
        "21":"/dev/ttyRG20",
        "22":"/dev/ttyRG21",
        "23":"/dev/ttyRG22",
        "24":"/dev/ttyRG23",
        "25":"/dev/ttyRG24",
        "26":"/dev/ttyRG25",
        "27":"/dev/ttyRG26",
        "28":"/dev/ttyRG27",
        "29":"/dev/ttyRG28",
        "30":"/dev/ttyRG29",
        "31":"/dev/ttyRG30",
        "32":"/dev/ttyRG31",
        "33":"/dev/ttyRG32",
        "34":"/dev/ttyRG33",
        "35":"/dev/ttyRG34",
        "36":"/dev/ttyRG35",
        "37":"/dev/ttyRG36",
        "38":"/dev/ttyRG37",
        "39":"/dev/ttyRG38",
        "40":"/dev/ttyRG39",
        "41":"/dev/ttyRG40",
        "42":"/dev/ttyRG41",
        "43":"/dev/ttyRG42",
        "44":"/dev/ttyRG43",
        "45":"/dev/ttyRG44",
        "46":"/dev/ttyRG45",
        "47":"/dev/ttyRG46",
        "48":"/dev/ttyRG47",
    }

    def __init__(self):
        self.baudrate_sysfs_path = self.serial_pci_path + "rg_uart_baudrate%d"
        self.present_sysfs_path = self.serial_pci_path + "rg_uart_presence%d"

    def console_check(self, console):
        if console < self.MIN_CONSOLE or console > self.MAX_CONSOLE:
            return -1
        else:
            return 0

    def serial_read_sysfs_file(self, sysfspath):
        if not os.path.exists(sysfspath):
            return False, ""
        else:
            try:
                with open(sysfspath, "rb", buffering=0) as sysfsfile:
                    buff = sysfsfile.read()
            except Exception as err:
                return False, ""
            return True, buff
    
    def set_console_baudrate(self, console, new_baudrate):
        """
        Set console baudrate
        @param console: console ID, start with 1
        @param new_baudrate: new console baudrate
        @return: 0 for sucess, return -1 if error
        """
        if self.console_check(console) != 0:
            return -1
        ret = subprocess_os_cmd((self.console_stty_cmd + "%d") % (console - 1, new_baudrate))
        return ret

    def get_console_baudrate(self, console):
        """ 
        Get console baudrate.
        @param console: console ID, start with 1
        @return: the console baudrate, return -1 if error
        """ 
        baudrate_sysfs_path = self.baudrate_sysfs_path % (console - 1)
        ret, baudrate = self.serial_read_sysfs_file(baudrate_sysfs_path)
        baudrate = baudrate.strip()
        if ret and baudrate.isdigit():
            return baudrate
        return None

    def get_console_link_status(self, console):
        """ 
        Get the console link status.
        @param console: console ID, start with 1
        @return: the console link status: Present or Not present
        """ 
        index = console - 1
        present_sysfs_path = self.present_sysfs_path % (int(index / 8))
        # Present need to check console
        if self.console_check(console) != 0:
            return "Not present"
        ret, present = self.serial_read_sysfs_file(present_sysfs_path)
        present = present.strip()
        if ret and present.isdigit():
            present = int(present, 10) & (1 << (index % 8))
            if present:
                return "Present"
        return "Not present"

    def get_console_nums(self):
        return self.MAX_CONSOLE

    def get_console_device_map(self):
        return self.CONSOLE_TO_DEVICE_MAP