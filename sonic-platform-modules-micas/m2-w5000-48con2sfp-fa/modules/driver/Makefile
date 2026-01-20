KVERSION   ?= $(shell uname -r)

obj-m :=  rg_cpld.o
ifneq ($(KVERSION),3.16.0-5-amd64)
obj-m += fpga_pcie_uart.o
endif
