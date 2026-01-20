// SPDX-License-Identifier: GPL-2.0
/*
 * wb_espi_cpld_dev.c - WB eSPI CPLD platform_device creator
 *
 * This module follows the common/modules pattern:
 *   - one small "*_device" module creates a platform_device instance
 *   - the real driver module (wb_espi_cpld.ko) binds via .name match
 *
 * Configuration is provided via module parameters and passed to the driver
 * using platform_data.
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/stat.h>

#include "wb_espi_cpld.h"

static u16 io_base = WB_CPLD_IO_BASE_DEFAULT;
static u16 io_size = WB_CPLD_IO_SIZE_DEFAULT;
static int cs_ctl0_gpio = -1;
static int cs_ctl1_gpio = -1;

static int g_wb_espi_cpld_dev_error = 1;
module_param(g_wb_espi_cpld_dev_error, int, S_IRUGO | S_IWUSR);

module_param(io_base, ushort, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(io_base, "IO base address (default: 0x700, must match BIOS config)");
module_param(io_size, ushort, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(io_size, "IO window size (default: 0x100, must match BIOS config)");
module_param(cs_ctl0_gpio, int, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(cs_ctl0_gpio, "Linux GPIO number for CS_CTL0 (required)");
module_param(cs_ctl1_gpio, int, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(cs_ctl1_gpio, "Linux GPIO number for CS_CTL1 (required)");

#define WB_ESPI_CPLD_DEV_ERROR(fmt, args...) do {					\
	if (g_wb_espi_cpld_dev_error)						\
		printk(KERN_ERR "[WB_ESPI_CPLD_DEV][ERR][func:%s line:%d] " fmt,	\
		       __func__, __LINE__, ##args);				\
} while (0)

static struct wb_espi_cpld_pdata wb_espi_cpld_pdata;

static void wb_espi_cpld_dev_release(struct device *dev)
{
	/* platform_device is static; nothing to free */
}

static struct platform_device wb_espi_cpld_pdev = {
	.name = "wb-espi-cpld",
	.id = -1,
	.dev = {
		.platform_data = &wb_espi_cpld_pdata,
		.release = wb_espi_cpld_dev_release,
	},
};

static int __init wb_espi_cpld_device_init(void)
{
	/*
	 * We require the GPIO numbers here because this module exists exactly
	 * to provide board-specific wiring to the driver in a consistent way.
	 */
	if (cs_ctl0_gpio < 0 || cs_ctl1_gpio < 0) {
		WB_ESPI_CPLD_DEV_ERROR("invalid GPIO numbers: cs_ctl0_gpio=%d cs_ctl1_gpio=%d\n",
				       cs_ctl0_gpio, cs_ctl1_gpio);
		return -EINVAL;
	}

	wb_espi_cpld_pdata.io_base = io_base;
	wb_espi_cpld_pdata.io_size = io_size;
	wb_espi_cpld_pdata.cs_ctl0_gpio = cs_ctl0_gpio;
	wb_espi_cpld_pdata.cs_ctl1_gpio = cs_ctl1_gpio;

	return platform_device_register(&wb_espi_cpld_pdev);
}

static void __exit wb_espi_cpld_device_exit(void)
{
	platform_device_unregister(&wb_espi_cpld_pdev);
}

module_init(wb_espi_cpld_device_init);
module_exit(wb_espi_cpld_device_exit);

MODULE_AUTHOR("Platform Driver Team");
MODULE_DESCRIPTION("WB eSPI CPLD platform device");
MODULE_LICENSE("GPL");
