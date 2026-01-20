import requests
import time
import subprocess

BMC_RAW_CMD = "http://240.1.1.1:8080/api/hw/rawcmd"
BMC_INFO_URL = "http://240.1.1.1:8080/api/bmc/info"
ERROR_RETURN = False
SUCCESS_RETURN = True
PING_CMD = "ping 240.1.1.1 -c 3 -i 0.3"
PING_OK = ", 0% packet loss"
ETH_STS_CMD = "ifconfig eth0; ifconfig eth0.4088"
INTERVAL = 10

class ConnectBmc(object):
    @staticmethod
    def get_from_bmc(uri, maxtimeout=30):
        resp = None
        try:
            resp = requests.get(uri, timeout=maxtimeout)
        except Exception as e:
            return ERROR_RETURN, str(e)
        if not resp:
            return ERROR_RETURN, "resp is None"
        data = resp.json()
        if not data or "data" not in data or "status" not in data:
            return ERROR_RETURN, "resp data is abnormal"
        if data["status"] != "OK":
            return ERROR_RETURN, "status is not OK"
        return SUCCESS_RETURN, data["data"]

    @staticmethod
    def post_to_bmc(uri, data, maxtimeout=120):
        resp = None
        try:
            resp = requests.post(uri, json=data, timeout=maxtimeout)
        except Exception as e:
            return ERROR_RETURN, str(e)
        if not resp:
            return ERROR_RETURN, "resp is None"
        data = resp.json()
        if "status" not in data and "data" not in data:
            return ERROR_RETURN, "resp data is abnormal"
        if data["status"] != "OK":
            return ERROR_RETURN, "status is not OK"
        return SUCCESS_RETURN, data["data"]

    @staticmethod
    def exec_bmc_cmd(cmd):
        data_list = {}
        data_list["Command"] = cmd
        ret, output = ConnectBmc.post_to_bmc(BMC_RAW_CMD, data_list)
        if not ret or len(output) == 0:
            return ret, output
        return ret, output["Outputs"]

    @staticmethod
    def wait_bmc_ready(timeout=360):
        ret = False
        log = "Not waiting for BMC to be ready"
        while timeout > 0:
            start_time = time.time()
            ret, log = subprocess.getstatusoutput(PING_CMD)
            if ret or PING_OK not in log:
                time.sleep(INTERVAL)
                end_time = time.time()
                timeout -= round(end_time - start_time)
                if timeout < 0:
                    ret, eth_log = subprocess.getstatusoutput(ETH_STS_CMD)
                    log += "\n" + eth_log
                ret = False
                continue
            ret, log = ConnectBmc.get_from_bmc(BMC_INFO_URL)
            if ret:
                break
            time.sleep(INTERVAL)
            end_time = time.time()
            timeout -= round(end_time - start_time)
        return ret, log
