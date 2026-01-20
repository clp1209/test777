#!/usr/bin/python
# -*- coding: utf-8 -*-

import base64
import binascii
import imp
import json
import os
import re
import ssl
import subprocess
import sys
import syslog
import time
import urllib.request, urllib.parse, urllib.error

import subprocess
import http.cookiejar
import requests
import urllib.request, urllib.error, urllib.parse
from requests.packages.urllib3.exceptions import InsecureRequestWarning
from rest_api import bmc_restful_logger

requests.packages.urllib3.disable_warnings(InsecureRequestWarning)

headers = {'Content-type': 'application/json'}
context = ssl._create_unverified_context()
__DEBUG__ = "N"
LOGIN_TRY_TIME = 3
RESTRETURNKEY = "status"
RESTRETURNSUCCESSKEY = "OK"


def d_print(debug_info):
    if(__DEBUG__ == "Y"):
        print(debug_info)


SYSLOG_IDENTIFIER = "DeviceMessage"
def log_debug(msg, also_print_to_console=False):
    try:
        # msg = msg.decode('utf-8').encode('gb2312')
        syslog.openlog(SYSLOG_IDENTIFIER)
        syslog.syslog(syslog.LOG_DEBUG, msg)
        syslog.closelog()

        if also_print_to_console:
            click.echo(msg)
    except Exception as e:
        pass

class BMCMessage():
    _session = None
    _diagToBmcCmdPrefix = "/usr/local/bin/rj_rawcmd_cli diag cmd "
    _sessionFile = "bmcsession"
    BMC_LOGIN_PATH = "/login"
    BMC_URLPRE_PATH = "http://240.1.1.1:8080"
    FANS_PRE_STRING = "/xyz/openbmc_project/inventory/system/chassis/motherboard/fan"
    FANS_STATE_STRING = "/xyz/openbmc_project/state/odm_fan0"
    FANS_PATH = "/xyz/openbmc_project/sensors/fan_tach/enumerate"
    FAN_FRU_PATH = "/xyz/openbmc_project/inventory/system/chassis/motherboard/enumerate"
    SENSORS_PATH = "/xyz/openbmc_project/sensors/enumerate"

    def getBmcUrl(self, name):
        return self.BMC_URLPRE_PATH + name

    def getBmcValue(self, path):
        return self.bmcget(self.getBmcUrl(path));

    def __init__(self):
        d_print("bmcmessage init...")
        self.rest_session = requests.session()
        self.rest_session.keep_alive = False
        pass

    def __del__(self):
        pass

    def bmcget(self, url):
        d_print(url)
        for i in range(3):
            try:
                response = self.rest_session.get(url, verify=False, timeout=60)
                if self.isReturnSuccess(response.text):
                    return json.loads(response.text)['data']
                bmc_restful_logger("bmcget:%s,status:%s,text:%s" %(url,response.status_code,response.text))
            except Exception as e:
                bmc_restful_logger("bmcget:%s,Exception:%s" %(url,str(e)))
        return None

    def bmcput(self, url, data):
        d_print(url)
        d_print(data)
        request = {}
        request["data"] = data
        data_json = json.dumps(request)
        d_print(data_json)
        try:
            response = self.rest_session.put(
                url, data=data_json, headers=headers, verify=False, timeout=60)
            if self.isReturnSuccess(response.text):
                return 1
            else:
                bmc_restful_logger("data:%s,bmcput:%s,status:%s,text:%s" %(data,url,response.status_code,response.text))
                return 0
        except Exception as e:
            bmc_restful_logger("data:%s,bmcput:%s,Exception:%s" %(data,url,str(e)))
            return 0

    def bmcpost(self, url, data):
        d_print(url)
        d_print(data)
        request = {}
        request = data
        data_json = json.dumps(request)
        d_print(data_json)
        for i in range(3):
            try:
                response = self.rest_session.post(
                    url, data=data_json, headers=headers, verify=False, timeout=120)
                if self.isReturnSuccess(response.text):
                    return json.loads(response.text)['data']['Outputs']
                bmc_restful_logger("data:%s,bmcpost:%s,status:%s,text:%s" %(data,url,response.status_code,response.text))
            except Exception as e:
                bmc_restful_logger("bmcpost:%s,Exception:%s" %(url,str(e)))
        return None

    def postSffTemp(self, data):
        url = "/xyz/openbmc_project/sensors/temperature/ODM_SSF_Temp/attr/Scale"
        return self.bmcput(self.getBmcUrl(url), data)

    def isNeedLogin(self, val):
        if RESTRETURNKEY in json.loads(val) and json.loads(val)[RESTRETURNKEY] == "error" and json.loads(val)['data']['description'] == 'Login required':
            return True
        else:
            return False

    def isReturnSuccess(self, val):
        if RESTRETURNKEY in json.loads(val) and json.loads(val)[RESTRETURNKEY] == RESTRETURNSUCCESSKEY:
            return True
        else:
            return False

    def getfansList(self):
        fan_ret = self.bmcget(self.getBmcUrl(self.FANS_PATH))
        return fan_ret

    def getfanFruList(self):
        fanfru_ret = self.bmcget(self.getBmcUrl(self.FAN_FRU_PATH))
        return fanfru_ret

    def getFansMsg(self):
        curFanList = self.getfansList()
        fanfru_ret = self.getfanFruList()
        if fanfru_ret == None:
            return None
        curfanFruLists = [it for it in fanfru_ret  if "fan" in it]
        fan_state = self.bmcget(self.getBmcUrl(self.FANS_STATE_STRING))
        if fan_state == None:
            return None
        if curFanList == None:
            return None
        curFanLists = [elem for elem in curFanList if "ODM_Fan" in elem]
        curFanLists = sorted(curFanLists)
        result = dict()
        result["Number"] = len(curFanLists)
        for item in curFanLists:
            rest1 = item.split("/")
            ret = rest1[-1].replace("ODM_", "").upper()
            fansreadNamevalue = self.FANS_PRE_STRING + str(int(ret[3:4]) - 1)
            dict1 = curFanList.get(item, None)
            dict2 = fanfru_ret.get(fansreadNamevalue, None)
            if dict1 == None:
                dict_t = dict2
            elif dict2 == None:
                dict_t = dict1
            else:
                dict_t = dict(dict1, **dict2)

            fanPresentKey = "Fan%dPresent" % (int(ret[3:4]))
            fanStateKey = "Fan%dState" % (int(ret[3:4]))
            if fan_state != None  and fan_state.get(fanPresentKey, None) == "xyz.openbmc_project.State.ODM_Fan.CurrentState.Ready":
                dict_t["Present"] = True
            else:
                dict_t["Present"] = False

            if fan_state != None and fan_state.get(fanStateKey, None) == "xyz.openbmc_project.State.ODM_Fan.CurrentState.Ready":
                dict_t["Running"] = True
            else:
                dict_t["Running"] = False
            dict_t["PN"] = dict_t.get("Model", None)
            dict_t["SN"] = dict_t.get("SerialNumber", None)
            dict_t["Speed"] = dict_t.get("Value", None)
            dict_t["LowThd"] = dict_t.get("CriticalLow", None)
            dict_t["HighThd"] = dict_t.get("CriticalHigh", None)
            result[ret] = dict_t
        return result

    def login_times(self):
        n = 0
        while True:
            if n > LOGIN_TRY_TIME:
                return False
            if self.login() == True:
                return True
            n += 1
        return False

    def login(self):
        try:
            url = self.getBmcUrl(self.BMC_LOGIN_PATH)
            self.rest_session = requests.session()
            data_json = json.dumps(LOGIN_REQUEST_DATA)
            response = self.rest_session.post(
                url, data=data_json, headers=headers, verify=False, timeout=60)
            if self.isReturnSuccess(response.text):
                self.saveSessionToFile()
                return True
            else:
                return False
        except Exception:
            return False

    def getPsuNum(self):
        return len(self.getPsuValues())

    def getPsuMsg(self):
        PSU_FAN_PATH = "/xyz/openbmc_project/sensors/fan_tach/enumerate"
        PSU_FRU_PATH = "/xyz/openbmc_project/inventory/system/chassis/motherboard/enumerate"
        PSU_PRESENCE_STR = "/xyz/openbmc_project/state/odm_psu0"
        STATE_STR = "xyz.openbmc_project.State.ODM_PSU.CurrentState.Ready"
        PSU_PRE_STR ="/xyz/openbmc_project/inventory/system/chassis/motherboard/powersupply"

        psuNums = self.bmcget(self.getBmcUrl(PSU_FAN_PATH))
        if psuNums == None:
            return None
        psuNumStrs = [psu_item for psu_item in psuNums if "Psu" in psu_item]
        ret = self.bmcget(self.getBmcUrl(PSU_FRU_PATH))
        value = self.bmcget(self.getBmcUrl(PSU_PRESENCE_STR))
        if ret == None:
            return None
        psuRestult = dict()
        psuRestult["Number"] = len(psuNumStrs)
        PSUNAME = "PSU"
        for psu_child in range(len(psuNumStrs)):
            index = psu_child + 1
            psuNames = PSU_PRE_STR + str(index - 1)
            psuDict = dict()
            psuPresentKey = "PSU%dPresent" % (index )
            psuStateKey = "PSU%dState" % (index )
            psuName = PSUNAME + str(index )
            itemdict = ret.get(psuNames, {})
            psuDict = itemdict
            if value.get(psuPresentKey, None) == STATE_STR:
                psuDict["Present"] = True
            else:
                psuDict["Present"] = False

            if value.get(psuStateKey, None) == STATE_STR:
                psuDict["PowerStatus"] = True
            else:
                psuDict["PowerStatus"] = False
            psuDict["PN"] = itemdict.get("Model", None)
            psuDict["SN"] = itemdict.get("SerialNumber", None)
            psuRestult[psuName] = psuDict
        return psuRestult

    def getSensorsNum(self):
        return len(self.getSensorsNames())

    def getSensorsName(self, index):
        result = self.getSensorsNames()[index]
        rest1 = result.split(":")
        return "".join(rest1[-1:])

    def getSensorsFullName(self, index):
        return self.getSensorsNames()[index]

    def getSensorsInputNumsByIndex(self, index):
        name = self.getSensorsFullName(index)
        return self.getSensorsinputNumByName(name)

    def getsensorLists(self):
        ret_t = self.bmcget(self.getBmcUrl(self.SENSORS_PATH))
        if ret_t == None:
            return None
        result = dict()
        for item in ret_t:
            childitem = dict()
            rest1 = item.split("/")
            ret = rest1[-1]
            psus = ""
            value1 = ""
            if "ODM_Fan" in ret:
                psus = ret[0:8].replace("ODM_", "").upper()
                value1 = ret.replace("ODM_", "").upper()
            elif "ODM_Psu" in ret:
                psus = ret[0:8].replace("ODM_", "").upper()
                value1 = ret.replace("ODM_", "").upper()
            elif "_Temp" in ret:
                psus = "temperature".upper()
                value1 = ret.upper()
            else:
                continue
            ret_tmp = ret_t.get(item, None)
            if ret_tmp == None:
                continue
            typename = self.getInputTypes(value1)
            ret_tmp["Type"] = typename
            ret_tmp["CriticalLow"] = self.getFinalValueByType(typename, ret_tmp.get("CriticalLow", 0))
            ret_tmp["LowThd"] = ret_tmp.get("CriticalLow", 0)
            ret_tmp["CriticalHigh"] = self.getFinalValueByType(typename, ret_tmp.get("CriticalHigh", 0))
            ret_tmp["HighThd"] = ret_tmp.get("CriticalHigh", 0)
            ret_tmp["Value"] = self.getFinalValueByType(typename, ret_tmp.get("Value", 0))
            if psus in result:
                result.get(psus)[value1] = ret_tmp
            else:
                childitem[value1] = ret_tmp
                result[psus] = childitem
        return result

    def getInputTypes(self, name):
        if "TEMP" in name:
            return "temperature"
        elif "SYS_" in name:
            return "voltage"
        elif "FAN" in name:
            return "RPM"
        elif "_CURR" in name:
            return "amp"
        elif "_VOL" in name:
            return "voltage"
        elif "POWER" in name:
            return "power"
        else:
            return "voltage"
        return None

    def getSensorInputType(self, name):
        if "Temp" in name:
            return "temperature"
        elif "SYS_" in name:
            return "voltage"
        elif "Fan" in name:
            return "RPM"
        elif "_Curr" in name:
            return "amp"
        elif "_Vol" in name:
            return "voltage"
        elif "Power" in name:
            return "power"
        else:
            return "voltage"
        return None

    def getFinalValueByType(self, inputtype, value):
        if inputtype == "voltage" or inputtype == "temperature" or inputtype == "power" or inputtype == "wattage" or inputtype == "amp":
            return float(value) / 1000
        else:
            return value

    def getDealValueByType(self, inputtype, value):
        inputtype = self.getSensorInputType(inputtype)
        if inputtype == "voltage" or inputtype == "temperature" or inputtype == "power" or inputtype == "wattage" or inputtype == "amp":
            return float(value) / 1000
        else:
            return value

    @property
    def session(self):
        return self._session

    def send_diag_cmd_t_bmc(self, str_cmd):
        fun_name_full = self._diagToBmcCmdPrefix + str_cmd
        data_post = {"Command": fun_name_full}
        PSU_PRESENCE_STR = "/api/hw/rawcmd"
        realurl = self.getBmcUrl(PSU_PRESENCE_STR)
        return self.bmcpost(realurl, data_post)


class MonitorMessage():

    def __init__(self):
        print("test")

    def test(self):
        print("Monitor")

#import config

class x86Message(MonitorMessage):
    MAILBOX_DIR       = "/sys/bus/i2c/devices/"
    module_product    = None
    _configfile_pre   = "config"

    def __init__(self):
        config_file = self._configfile_pre + ".py"
        dir = os.path.dirname(os.path.realpath(__file__))
        if config_file in os.listdir(dir):
            dir = os.path.dirname(os.path.realpath(__file__))
            realpath = os.path.join(dir, config_file)
            self.module_product = imp.load_source(self._configfile_pre, realpath)
            #print self.module_product.factest_module
        else:
            raise Exception("device config file load error")
        d_print('x86 init...')

    def test(self):
        print("x86Message")

    def getFanNum(self):
        return len(self.getFanNames())

    def getFanName(self, index):
        x = self.getFanNames()
        name = x[index]
        return name

    def get_pmc_register(self, reg_name):
        retval = 'ERR'
        mb_reg_file = self.MAILBOX_DIR + reg_name

        if (not os.path.isfile(mb_reg_file)):
            return None
        try:
            fd = os.open("/sys/bus/i2c/devices/0-0056/eeprom", os.O_RDWR|os.O_CREAT)
            retval=os.read(fd, 256)
            tmp = ""
            for i in retval:
                tmp = tmp + chr(i)
        except Exception as error:
            pass
        finally:
            os.close(fd)
        retval = tmp.rstrip('\r\n').lstrip(" ")
        return retval

#     def getValue(self, location, bit , type):
#         value_t = self.get_pmc_register(location)
#         if value_t.startswith("ERR"):
#             return value_t
#         if (type == 1):
#             return float(value_t) / 1000
#         elif (type == 2):
#             return float(value_t) / 100
#         elif (type == 3):
#             psu_status = int(value_t, 16)
#             return (psu_status & (1 << bit)) >> bit
#         else:
#             return value_t;

    def getFanFru(self, name):
        return self.getFanStatus(name)

    def getonieversion(self):
        if not os.path.isfile('/host/machine.conf'):
            return ""
        machine_vars = {}
        with open('/host/machine.conf') as machine_file:
            for line in machine_file:
                tokens = line.split('=')
                if len(tokens) < 2:
                    continue
                machine_vars[tokens[0]] = tokens[1].strip()
        return machine_vars.get("onie_version")

    def tlve2set(self,bin_data):
        RUIJIE_CARDID = self.module_product.RUIJIE_CARDID
        _value = {}
        _value[boardTLV._TLV_CODE_VENDOR_EXT]     = self.getTLVcardidBin()
        _value[boardTLV._TLV_CODE_PRODUCT_NAME]   = self.module_product.RUIJIE_PRODUCTNAME
        _value[boardTLV._TLV_CODE_PART_NUMBER]    = self.module_product.RUIJIE_PART_NUMBER
        _value[boardTLV._TLV_CODE_LABEL_REVISION] = self.module_product.RUIJIE_LABEL_REVISION
        _value[boardTLV._TLV_CODE_PLATFORM_NAME]  = DeviceMessage().getplatform()
        _value[boardTLV._TLV_CODE_ONIE_VERSION]   = self.getonieversion()
        _value[boardTLV._TLV_CODE_MAC_SIZE]       = self.module_product.RUIJIE_MAC_SIZE
        _value[boardTLV._TLV_CODE_MANUF_NAME]     = self.module_product.RUIJIE_MANUF_NAME
        _value[boardTLV._TLV_CODE_MANUF_COUNTRY]  = self.module_product.RUIJIE_MANUF_COUNTRY
        _value[boardTLV._TLV_CODE_VENDOR_NAME]    = self.module_product.RUIJIE_VENDOR_NAME
        _value[boardTLV._TLV_CODE_DIAG_VERSION]   = self.module_product.RUIJIE_DIAG_VERSION
        _value[boardTLV._TLV_CODE_SERVICE_TAG]    = self.module_product.RUIJIE_SERVICE_TAG
        _value[boardTLV._TLV_CODE_MANUF_DATE] =time.strftime('%m/%d/%Y %H:%M:%S', time.localtime())  # 自动添加setmac时
        _value[boardTLV._TLV_CODE_PRODUCT_NAME]   = bin_data.get(boardTLV._TLV_CODE_PRODUCT_NAME)
        _value[boardTLV._TLV_CODE_SERIAL_NUMBER]  = bin_data.get(boardTLV._TLV_CODE_SERIAL_NUMBER)
        _value[boardTLV._TLV_CODE_DEVICE_VERSION]= bin_data.get(boardTLV._TLV_CODE_DEVICE_VERSION)
        _value[boardTLV._TLV_CODE_MAC_BASE] = bin_data.get(boardTLV._TLV_CODE_MAC_BASE)
        rst, ret = boardTLV().generate_value(_value)
        boardTLV().writeToEEprom(rst , self.module_product.E2_LOC)

    #productConf = {'tlveeprom': '0-0056/eeprom' }

    def getCPUeeprom(self):
        e2loc = self.module_product.CPUEEPROMS
        name = self.dumpValueByI2c(e2loc.get('bus'),e2loc.get('loc'))
        return name

    def setCPUeeprom(self,bindata):
        e2loc = self.module_product.CPUEEPROMS
        Osutil.writeToE2(e2loc.get('bus'),e2loc.get('loc'), bindata)

    def dumpValueByI2c(self, bus, loc):
        tmp = ""
        for i in range(256):
            ret,val = Osutil.rji2cget(bus, loc, i)
            tmp += chr(Osutil.strtoint(val))
        return tmp

    def geteepromLocation(self,bus, devno):
        location = "%d-00%02x/eeprom" % (bus,devno)
        return location

    def getTlvLoc(self):
        e2loc = self.module_product.E2_LOC
        location = "%d-00%02x/eeprom" % (e2loc.get('bus'),e2loc.get('devno') )
        return location

    def getTLVe2(self):
        name = self.get_pmc_register(self.getTlvLoc())
        if name == None:
            return name
        return boardTLV().decode_eeprom(name)

    def getcardid(self):
        return self.module_product.RUIJIE_CARDID

    def getproductname(self):
        return self.module_product.RUIJIE_PRODUCTNAME

    def getTLVproductname(self):
        name = self.get_pmc_register(self.getTlvLoc())
        if name == None:
            return name
        return boardTLV().getcardName(name)

    def getTLVcardidBin(self):
        return boardTLV().generate_e2_cardid(self.module_product.RUIJIE_CARDID)

    def resetTLVe2(self):
        cardidbin = self.getTLVcardidBin()
        _value = {}
        _value[boardTLV._TLV_CODE_VENDOR_EXT]  = cardidbin
        rst, ret = boardTLV().generate_value(_value)
        print(boardTLV().writeToEEprom(rst, self.module_product.E2_LOC))

class boardTLV():
    _TLV_INFO_ID_STRING = "TlvInfo\x00"
    _TLV_INFO_VERSION = 0x01
    _TLV_INFO_LENGTH = 0x00
    _TLV_INFO_LENGTH_VALUE = 0xba
    _TLV_CODE_PRODUCT_NAME = 0x21
    _TLV_CODE_PART_NUMBER = 0x22
    _TLV_CODE_SERIAL_NUMBER = 0x23
    _TLV_CODE_MAC_BASE = 0x24
    _TLV_CODE_MANUF_DATE = 0x25
    _TLV_CODE_DEVICE_VERSION = 0x26
    _TLV_CODE_LABEL_REVISION = 0x27
    _TLV_CODE_PLATFORM_NAME = 0x28
    _TLV_CODE_ONIE_VERSION = 0x29
    _TLV_CODE_MAC_SIZE = 0x2A
    _TLV_CODE_MANUF_NAME = 0x2B
    _TLV_CODE_MANUF_COUNTRY = 0x2C
    _TLV_CODE_VENDOR_NAME = 0x2D
    _TLV_CODE_DIAG_VERSION = 0x2E
    _TLV_CODE_SERVICE_TAG = 0x2F
    _TLV_CODE_VENDOR_EXT = 0xFD
    _TLV_CODE_CRC_32 = 0xFE
    _TLV_DISPLAY_VENDOR_EXT = 1
    _TLV_CODE_RJ_CARID = 0x01
    _TLV_INFO_HDR_LEN = 11


    def _crc32(self,v):
        return '0x%08x' % (binascii.crc32(v) & 0xffffffff)

    def getTLV_BODY(self, key, productname):
        x = []
        tmp = []
        temp_len = 0
        if key == self._TLV_CODE_MAC_BASE:
            arr = productname.split(':')
            for tt in arr:
                tmp.append(int(tt, 16).to_bytes(1, "big"))
        elif key == self._TLV_CODE_DEVICE_VERSION:
            tmp.append(productname.to_bytes(1, "big"))
        elif key == self._TLV_CODE_MAC_SIZE:
            tmp.append((productname >> 8).to_bytes(1, "big"))
            tmp.append((productname & 0x00ff).to_bytes(1, "big"))
        else:
            for data in productname:
                tmp.append(ord(data).to_bytes(1, "big"))
        temp_len = len(tmp)
        x.append((key).to_bytes(1, "big"))
        x.append((temp_len).to_bytes(1, "big"))
        for i in tmp:
            x.append(i)
        return x

    def generate_value(self,_t):
        ret = []
        for i in self._TLV_INFO_ID_STRING:
            ret.append(ord(i).to_bytes(1, "big"))
        ret.append((self._TLV_INFO_VERSION).to_bytes(1, "big"))
        ret.append((self._TLV_INFO_LENGTH).to_bytes(1, "big"))
        ret.append((self._TLV_INFO_LENGTH_VALUE).to_bytes(1, "big"))

        total_len = 0
        for key in _t:
            x = self.getTLV_BODY(key, _t[key])
            ret += x
            total_len += len(x)
        ret[10] = (total_len + 6).to_bytes(1, "big")
        ret.append((0xFE).to_bytes(1, "big"))
        ret.append((0x04).to_bytes(1, "big"))
        s = self._crc32(bytearray(b''.join(ret)))
        for t in range(0, 4):
            ret.append(chr(int(s[2 * t + 2:2 * t + 4], 16)))
        totallen = len(ret)
        if (totallen < 256):
            for left_t in range(0, 256 - totallen):
                ret.append(chr(0x00))
        return (ret, True)

    def generate_e2_cardid(self, cardid):
        s = "%08x" % cardid
        ret = ""
        for t in range(0, 4):
            ret += chr(int(s[2 * t:2 * t + 2], 16))
        ret = chr(0x01) + chr(len(ret)) + ret
        return ret

    def decode_eeprom(self, e):
        total_len = (ord(e[9]) << 8) | ord(e[10])
        tlv_index = self._TLV_INFO_HDR_LEN
        tlv_end = self._TLV_INFO_HDR_LEN + total_len
        ret = []
        while (tlv_index + 2) < len(e) and tlv_index < tlv_end:
            rt = self.decoder(None, e[tlv_index:tlv_index + 2 + ord(e[tlv_index + 1])])
            ret.append(rt)
            if ord(e[tlv_index]) == self._TLV_CODE_CRC_32:
                break
            tlv_index += ord(e[tlv_index + 1]) + 2
        for item in ret:
            if item['code'] == self._TLV_CODE_VENDOR_EXT:
                rt = self.decoder(None, item["value"]
                             [0: 0 + 2 + ord(item["value"][0 + 1])])
                ret.append(rt)
        return ret

    def getcardid(self, eeprom):
        ret = self.decode_eeprom(eeprom)
        for item in ret:
            if item['code'] == self._TLV_CODE_RJ_CARID:
                return item.get('value',None)
        return None

    def getcardName(self,eeprom):
        ret = self.decode_eeprom(eeprom)
        for item in ret:
            if item['code'] == self._TLV_CODE_PRODUCT_NAME:
                return item.get('value',None)
        return None

    def decoder(self,s, t):
        if ord(t[0]) == self._TLV_CODE_PRODUCT_NAME:
            name = "Product Name"
            value = str(t[2:2 + ord(t[1])])
        elif ord(t[0]) == self._TLV_CODE_PART_NUMBER:
            name = "Part Number"
            value = t[2:2 + ord(t[1])]
        elif ord(t[0]) == self._TLV_CODE_SERIAL_NUMBER:
            name = "Serial Number"
            value = t[2:2 + ord(t[1])]
        elif ord(t[0]) == self._TLV_CODE_MAC_BASE:
            name = "Base MAC Address"
            value = ":".join([bytes.decode(binascii.b2a_hex(ord(T).to_bytes(1, "big"))) for T in t[2:8]]).upper()
        elif ord(t[0]) == self._TLV_CODE_MANUF_DATE:
            name = "Manufacture Date"
            value = t[2:2 + ord(t[1])]
        elif ord(t[0]) == self._TLV_CODE_DEVICE_VERSION:
            name = "Device Version"
            value = str(ord(t[2]))
        elif ord(t[0]) == self._TLV_CODE_LABEL_REVISION:
            name = "Label Revision"
            value = t[2:2 + ord(t[1])]
        elif ord(t[0]) == self._TLV_CODE_PLATFORM_NAME:
            name = "Platform Name"
            value = t[2:2 + ord(t[1])]
        elif ord(t[0]) == self._TLV_CODE_ONIE_VERSION:
            name = "ONIE Version"
            value = t[2:2 + ord(t[1])]
        elif ord(t[0]) == self._TLV_CODE_MAC_SIZE:
            name = "MAC Addresses"
            value = str((ord(t[2]) << 8) | ord(t[3]))
        elif ord(t[0]) == self._TLV_CODE_MANUF_NAME:
            name = "Manufacturer"
            value = t[2:2 + ord(t[1])]
        elif ord(t[0]) == self._TLV_CODE_MANUF_COUNTRY:
            name = "Manufacture Country"
            value = t[2:2 + ord(t[1])]
        elif ord(t[0]) == self._TLV_CODE_VENDOR_NAME:
            name = "Vendor Name"
            value = t[2:2 + ord(t[1])]
        elif ord(t[0]) == self._TLV_CODE_DIAG_VERSION:
            name = "Diag Version"
            value = t[2:2 + ord(t[1])]
        elif ord(t[0]) == self._TLV_CODE_SERVICE_TAG:
            name = "Service Tag"
            value = t[2:2 + ord(t[1])]
        elif ord(t[0]) == self._TLV_CODE_VENDOR_EXT:
            name = "Vendor Extension"
            value = ""
            if self._TLV_DISPLAY_VENDOR_EXT:
                value = t[2:2 + ord(t[1])]
        elif ord(t[0]) == self._TLV_CODE_CRC_32 and len(t) == 6:
            name = "CRC-32"
            value = "0x%08X" % (((ord(t[2]) << 24) | (
                ord(t[3]) << 16) | (ord(t[4]) << 8) | ord(t[5])),)
        elif ord(t[0]) == self._TLV_CODE_RJ_CARID:
            name = "rj_cardid"
            value = ""
            for c in t[2:2 + ord(t[1])]:
                value += "%02X" % (ord(c),)
        else:
            name = "Unknown"
            value = ""
            for c in t[2:2 + ord(t[1])]:
                value += "0x%02X " % (ord(c),)
        return {"name": name, "code": ord(t[0]), "value": value}

    def writeToEEprom(self,rst_arr , E2_LOC):
        index = 0
        for item in rst_arr:
            Osutil.rji2cset(E2_LOC["bus"], E2_LOC["devno"], index, ord(item))
            index += 1
        os.system("rmmod at24 ")
        os.system("modprobe at24 ")
        os.system("rm -f /var/cache/sonic/decode-syseeprom/syseeprom_cache")


class Osutil():
    SONIC_CFGGEN_PATH = '/usr/local/bin/sonic-cfggen'
    HWSKU_KEY = "DEVICE_METADATA['localhost']['hwsku']"
    PLATFORM_KEY = "DEVICE_METADATA['localhost']['platform']"
    u_disk_mount_partition = []
    r8xxx_device_status = {}

    @staticmethod
    def writeToE2(bus, loc, rst_arr):
        index = 0
        for item in rst_arr:
            Osutil.rji2cset(bus, loc, index, item)
            index += 1
    @staticmethod
    def strtoint(str_tmp):  # Hexadecimal string transfer int ,such as "4040"/"0x4040"/"0X4040" = 16448
        value = 0
        rest_v = str_tmp.replace("0X", "").replace("0x", "")
        for index in range(len(rest_v)):
            value |= int(rest_v[index], 16) << ((len(rest_v) - index - 1) * 4)
        return value
    @staticmethod
    def rj_os_system(cmd):
        status, output = subprocess.getstatusoutput(cmd)
        return status, output

    @staticmethod
    def rji2cset(bus, devno, address, byte):
        command_line = "i2cset -f -y %d 0x%02x 0x%02x 0x%02x" % (
            bus, devno, address, byte)
        retrytime = 6
        ret_t = ""
        for i in range(retrytime):
            ret, ret_t = Osutil.rj_os_system(command_line)
            if ret == 0:
                return True, ret_t
        return False, ret_t

    @staticmethod
    def rji2cget(bus, devno, address):
        command_line = "i2cget -f -y %d 0x%02x 0x%02x " % (bus, devno, address)
        retrytime = 6
        ret_t = ""
        for i in range(retrytime):
            ret, ret_t = Osutil.rj_os_system(command_line)
            if ret == 0:
                return True, ret_t
            time.sleep(0.1)
        return False, ret_t
    @staticmethod
    def isValidMac(mac):
        if re.match(r"^\s*([0-9a-fA-F]{2,2}:){5,5}[0-9a-fA-F]{2,2}\s*$", mac):
            return True
        return False

    @staticmethod
    def rj_os_system_regex_raise(cmd,regexp="([\s\S]*)",info="",no_raise = False):
        output_ret = ""
        status, output = subprocess.getstatusoutput(cmd)
        re_search_result = re.search(regexp, output)
        #print "cmd:{}\nin:{}\nregexp: {}\n out:{}".format(cmd,output,regexp,re_search_result)
        if re_search_result:
            output_ret = re_search_result.group(1)
        if status or not re_search_result:
            if no_raise:
                return None
            raise Exception(output + "\n" + info)
        return output_ret

    @staticmethod
    def setethmac_igb(eth, mac):
        rulefile = "/etc/udev/rules.d/70-persistent-net.rules"
        if Osutil.isValidMac(mac) == False:
            return False, "MAC非法"
        cmd = "ethtool -e %s | grep 0x0010 | awk '{print \"0x\"$13$12$15$14}'" % eth
        ret, log = Osutil.rj_os_system(cmd)
        log_debug(log)
        magic = ""
        if ret == 0 and len(log):
            magic = log
        macs = mac.upper().split(":")

        # Temporarily change the local ETH0 to the value after setMAC
        ifconfigcmd = "ifconfig eth0 hw ether %s" % mac
        log_debug(ifconfigcmd)
        ret, status = Osutil.rj_os_system(ifconfigcmd)
        if ret:
            return False, "软件设置网卡MAC出错"
        index = 0
        for item in macs:
            cmd = "ethtool -E %s magic %s offset %d value 0x%s length 1" % (
                eth, magic, index, item)
            log_debug(cmd)
            index += 1
            ret, log = Osutil.rj_os_system(cmd)
            if ret != 0:
                return False, "设置硬件网卡MAC出错"
        # Gets the return value of the setting
        cmd_t = "ethtool -e eth0 offset 0 length 6"
        ret, log = Osutil.rj_os_system(cmd_t)
        m = re.split(':', log)[-1].strip().upper()
        mac_result = m.upper().split(" ")

        for ind, s in enumerate(macs):
            if s != mac_result[ind]:
                return False, "MAC比较出错"
        if os.path.exists(rulefile):
            os.remove(rulefile)
        print("MGMT MAC【%s】" % mac)
        return True, "success"

    @staticmethod
    def get_udisk_location(tool):

        pre_dir = "/home/admin/upgrade/"
        shell_ret, log = Osutil.rj_os_system("ls {} |grep  {}".format(pre_dir,tool))
        if shell_ret == 0:
            return pre_dir
        #note：only the serial server is available
        usb_block = "/sys/devices/pci0000\:00/0000\:00\:14.0/*/*/*/host*/tar*/*/block"
        sdx = Osutil.rj_os_system_regex_raise("ls {}".format(usb_block),info="获取U盘设备失败")
        partitions = Osutil.rj_os_system_regex_raise("ls {}/{} |grep {}".format(usb_block,sdx,sdx),info="获取U盘分区失败").split("\n")
        mountdir = None
        for partition in partitions:
            try:
                _, mountdir = Osutil.rj_os_system("mount |grep  -Po '(?<=/dev/{} on )\S+'".format(partition))
                if not mountdir:
                    temp_dir = Osutil.rj_os_system_regex_raise("mktemp -d")
                    Osutil.rj_os_system_regex_raise("mount /dev/{} {}".format(partition,temp_dir))
                    Osutil.u_disk_mount_partition.append(temp_dir)
                    Osutil.rj_os_system_regex_raise("ls {}/{}".format(temp_dir,tool),regexp = "({})".format(tool))
                    return  temp_dir
                else:
                    return mountdir
            except Exception as e:
                continue
        else:
            raise Exception("无法找到{}".format(tool))

    @staticmethod
    def r8xxx_devices_status_get():
        #ip route save 1> dump
        #ip route restore < dump
        if "default_gw" not in Osutil.r8xxx_device_status:
            default_gw = Osutil.rj_os_system_regex_raise("ip route  |grep default",regexp="(\d{1,3}(?:\.\d{1,3}){3})",no_raise=True)
            eth_default_gw = Osutil.rj_os_system_regex_raise("ip route  |grep default",regexp="(?<=dev)( \S+)",no_raise=True)
            if default_gw:
                Osutil.r8xxx_device_status["default_gw"] = {"dev":eth_default_gw,"ip":default_gw}

        ip_log = Osutil.rj_os_system_regex_raise("ip addr show ",no_raise=True)
        #print ip_log
        #get eth and ip
        ip_list = re.findall("[^@](eth\d+):(?:(?!\n\d+:)[\s\S])*?(?<=inet )(\d{1,3}(?:\.\d{1,3}){3}/\d+)",ip_log)
        #print ip_list
        Osutil.r8xxx_device_status.update(dict(ip_list))

    @staticmethod
    def get_mac_index_of_r8xxx(eth):
        mac_list = Osutil.rj_os_system_regex_raise("ifconfig -a |grep -oP '^eth\d(?=:)'",info = "获取网卡列表失败").split("\n")
        index = 1
        for mac in mac_list:
            driver = Osutil.rj_os_system_regex_raise("ethtool -i " + mac, "(?<=driver: )(\S+)",info = "获取网卡驱动信息失败！")
            if not re.match("r8\d+",driver):
                continue
            if mac == eth:
                return index
            else:
                index += 1
        else:
            return None

    @staticmethod
    def get_kernel_version():
        try:
            kernel_v_str = "uname -v"
            ret, version = Osutil.rj_os_system(kernel_v_str)
            if ret !=0:
                return None
            version = version.split(" ")
            cur_version = version[3].split("-")
            return cur_version[0]
        except Exception as e:
            return None

    @staticmethod
    def r8xxx_devices_status_restore(eth_list):
        for eth in eth_list:
            if eth == "default_gw":
                continue
            ip = Osutil.r8xxx_device_status.get(eth,None)
            if ip:
                Osutil.rj_os_system_regex_raise("ifconfig {} {}".format(eth,ip),no_raise=True)

        default_gw = Osutil.r8xxx_device_status.get("default_gw",None)
        if default_gw and default_gw["dev"] not in Osutil.r8xxx_device_status:# The ETH of default is restored
            Osutil.rj_os_system_regex_raise("route add default gw {}".format(default_gw["ip"]),no_raise=True)
            Osutil.r8xxx_device_status.pop("default_gw")

    @staticmethod
    def recover_r8xxx_net():
        mac_list = Osutil.rj_os_system_regex_raise("ifconfig -a |grep -oP '^eth\d(?=:)'",info = "获取网卡列表失败").split("\n")
        # Storage Devices that use r8XXX drivers are used to recover IP addresses
        r8xxx_dev_list = []
        for eth in mac_list:
            driver = Osutil.rj_os_system_regex_raise("ethtool -i " + eth, "(?<=driver: )(\S+)",info = "获取网卡驱动信息失败！")
            if not re.match("r8\d+",driver):
                continue
            r8xxx_dev_list.append(eth)
            Osutil.rj_os_system("ifconfig {} up".format(eth))
            if "eth0" == eth:
                Osutil.rj_os_system(" systemctl restart privatenetwork")
                Osutil.rj_os_system("ifconfig eth0.4088 up")

        Osutil.r8xxx_devices_status_restore(r8xxx_dev_list)

    @staticmethod
    def setethmac_r8xxx(eth, mac):
        #tool_dir = "/home/admin/aa"

        mac_tmp = re.sub(':',"",mac)
        mac_r8xxx_format = re.sub('(\w\w)(?=\w)',"\g<1> ",mac_tmp)
        try:
            tool_dir = Osutil.get_udisk_location("r8xxx_setmac.sh")
            index = Osutil.get_mac_index_of_r8xxx(eth)
            if not index:
                return False,"获取网卡索引失败"
            Osutil.r8xxx_devices_status_get()
            driver = Osutil.rj_os_system_regex_raise("ethtool -i " + eth, "(?<=driver: )(\S+)",info = "获取网卡驱动信息失败！")
            cur_version = Osutil.get_kernel_version()
            if not cur_version:
                return False,"获取内核版本失败"
            Osutil.rj_os_system_regex_raise("rmmod {} ; lsmod |grep pgdrv || insmod {}/pgdrv-{}.ko".format(driver,tool_dir,cur_version),info = "Cannot found pgdrv.ko")
            Osutil.rj_os_system_regex_raise("bash {}/r8xxx_setmac.sh {} \"{}\"".format(tool_dir,index,mac_r8xxx_format),regexp="(PG EFuse is Successful)")
        finally:
            while Osutil.u_disk_mount_partition:
                dir_tmp = Osutil.u_disk_mount_partition.pop()
                Osutil.rj_os_system("umount {}".format(dir_tmp))

            Osutil.rj_os_system("rmmod pgdrv ;  modprobe {} ".format(driver))
            Osutil.recover_r8xxx_net()
        return True,""


    @staticmethod
    def get_platform_and_hwsku():
        try:
            proc = subprocess.Popen([Osutil.SONIC_CFGGEN_PATH, '-H', '-v', Osutil.PLATFORM_KEY],
                                    stdout=subprocess.PIPE,
                                    shell=False,
                                    stderr=subprocess.STDOUT)
            stdout = proc.communicate()[0]
            proc.wait()
            platform = bytes.decode(stdout).rstrip('\n')

            proc = subprocess.Popen([Osutil.SONIC_CFGGEN_PATH, '-d', '-v', Osutil.HWSKU_KEY],
                                    stdout=subprocess.PIPE,
                                    shell=False,
                                    stderr=subprocess.STDOUT)
            stdout = proc.communicate()[0]
            proc.wait()
            hwsku = bytes.decode(stdout).rstrip('\n')
        except OSError as e:
            raise OSError("Cannot detect platform")

        return (str(platform), str(hwsku))

class DeviceMessage():
    _message = None

    def __init__(self, GLObalSelectBmc=True):
        #if GLObalSelectBmc:
        #    self._message = BMCMessage()
        #else:
        #    self._message = x86Message()
        pass

    @property
    def message(self):
        return self._message

    def getFRUList(self):
        cmd = "bmc_get_e2_show_list"
        ret_info = BMCMessage().send_diag_cmd_t_bmc(cmd)
        return ret_info[0]

    def getSETFRUList(self):
        str1 = "bmc_get_e2_set_list"
        ret_info = BMCMessage().send_diag_cmd_t_bmc(str1)
        return ret_info[0]

    def getFRUeeprom(self, e2type, index):
        strtmp = "bmc_get_e2_bin -json \"%s\""
        param = {}
        param["e2type"] = e2type
        param["index"] = index
        paramtmp = strtmp % str(param)
        ret_info = BMCMessage().send_diag_cmd_t_bmc(paramtmp)
        ret_dict = eval(ret_info[0])
        if ret_dict.get("status") == "ok":
            return ret_dict.get('ret_info')
        else:
            return None

    def setFRUeeprom(self, e2type, index, bindata):
        strtmp  = "bmc_write_e2_bin -json \"%s\""
        param1 = []
        param = {}
        param["e2type"] = e2type
        param["index"] = index
        ret_t = base64.b64encode(bindata)
        param["bin"] = ret_t
        param1.append(param)
        paramtmp = strtmp % str(param1)
        ret_info = BMCMessage().send_diag_cmd_t_bmc(paramtmp)
        ret_dict = eval(ret_info[0])
        if ret_dict.get("status") == "ok":
            return True
        else:
            print(ret_dict.get("ret_info"))
            return False

    def getplatform(self):
        '''get sonic platform information'''
        (platform, hwsku) = Osutil.get_platform_and_hwsku()
        return platform


    def settlve2(self, bin_data, cm_flag):
        '''set TLV E2'''
        return x86Message().tlve2set(bin_data)

    def getTLVe2(self):
        '''get TLV E2'''
        return x86Message().getTLVe2();

    def getTLVe2Dict(self):
        '''get TLV E2 dict'''
        ret = self.getTLVe2()
        ret_t = {}
        for item in ret:
            ret_t[item['code']] = item['value']
        return ret_t

    def resetTLVE2(self):
        return x86Message().resetTLVe2();

    def getcardid(self):
        '''set TLV Board ID'''
        return x86Message().getcardid();

    def getproductname(self, readtype):
        '''Read product name  0 is read from config files,1 is read from tlv'''
        if readtype == 0:
            return x86Message().getproductname()
        elif readtype == 1:
            return x86Message().getTLVproductname()
        else:
            return None
    def getCPUeeprom(self):
        return x86Message().getCPUeeprom();

    def setCPUEEPROM(self,bindata):
        return x86Message().setCPUeeprom(bindata)

    def getFanAll(self):
        return self.message.getFansMsg()

    def getPsuAll(self):
        return self.message.getPsuMsg();

    def getsensorAll(self):
        return self.message.getsensorLists()

    def util_setmac(self,eth, mac):
        try:
            eth_driver = Osutil.rj_os_system_regex_raise("ethtool -i " + eth, "(?<=driver: )(\S+)",info = "获取网卡驱动信息失败！")
            eth_driver = re.sub("r8\d+","r8xxx",eth_driver)
            return eval("Osutil.setethmac_"+eth_driver)(eth, mac);
        except Exception as e:
            print(traceback.format_exc())
            return False,str(e)

    def getDmiSysByType(self, type_t):
        ret, log = Osutil.rj_os_system("which dmidecode ")
        if ret != 0 or len(log) <= 0:
            error = "cmd find dmidecode"
            return False, error
        cmd = log + " -t %s" % type_t
        ret1, log1 = Osutil.rj_os_system(cmd)
        if ret != 0 or len(log1) <= 0:
            return False, "命令执行出错[%s]" % cmd
        its = log1.replace("\t", "").strip().split("\n")
        ret = {}
        for it in its:
            if ":" in it:
                key = it.split(":")[0].lstrip()
                value = it.split(":")[1].lstrip()
                ret[key] = value
        return True, ret
    pass
