#!/usr/bin/python
# -*- coding: UTF-8 -*-
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

CPUEEPROMS = {"name":"cpueeprom","bus":4,"loc":0x57,"E2PRODUCT":'2', "E2TYPE":'4'}

#rg_eeprom  = "0-0056/eeprom"
E2_LOC = {"bus":0, "devno":0x56}
E2_PROTECT = {"bus":1, "devno":0x0d, "addr":0x19, "open":0, "close":0xff}
FAN_PROTECT = {"bus":1, "devno":0x0d, "addr":0x19, "open":0x00, "close":0xff}


