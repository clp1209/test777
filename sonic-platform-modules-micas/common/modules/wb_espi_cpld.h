/* SPDX-License-Identifier: GPL-2.0 */
/*
 * wb_espi_cpld.h - WB eSPI CPLD access via eSPI Peripheral Channel
 *
 * Current implementation programs the eSPI controller's IO decode window and
 * performs 8-bit accesses through the controller driver (espi-amd).
 *
 * This header is local to wb_espi_cpld*.ko modules.
 */

#ifndef __WB_ESPI_CPLD_H__
#define __WB_ESPI_CPLD_H__

#include <linux/types.h>
#include <linux/mutex.h>
#include <linux/device.h>
#include <linux/gpio/consumer.h>
#include <linux/miscdevice.h>

#define WB_CPLD_IO_BASE_DEFAULT	0x700
#define WB_CPLD_IO_SIZE_DEFAULT	0x100
#define WB_CPLD_MAX_REG_OFFSET	(WB_CPLD_IO_SIZE_DEFAULT - 1)

#define WB_CPLD_MAX_DEVS	2

enum wb_cpld_target {
	WB_CPLD_NONE = 0,	/* CS_CTL=00, undefined (default low) */
	WB_CPLD_CTRL = 1,	/* CS_CTL=01, CS->CS1 */
	WB_CPLD_CPU  = 2,	/* CS_CTL=10, CS->CS2 */
	WB_CPLD_RESV = 3,	/* CS_CTL=11, reserved */
};

struct wb_espi_cpld_pdata {
	u16 io_base;
	u16 io_size;
	int cs_ctl0_gpio;
	int cs_ctl1_gpio;
};

struct wb_espi_ops {
	int (*enable_io_decode)(u16 base, u16 size);
	int (*get_io_decode_range)(unsigned int idx, u16 *base, u8 *size_field, bool *enabled);
	int (*io_decode_covers)(u16 base, u16 size);
	int (*io_read8)(u16 port, u8 *val);
	int (*io_write8)(u16 port, u8 val);
};

struct wb_cpld_bus {
	struct device *dev;
	struct gpio_desc *cs_ctl0;
	struct gpio_desc *cs_ctl1;
	int cs_ctl0_gpio;
	int cs_ctl1_gpio;
	struct mutex lock;
	u16 io_base;
	u16 io_size;
	struct wb_espi_ops ops;
};

struct wb_cpld_dev {
	struct wb_cpld_bus *bus;
	enum wb_cpld_target target;
	const char *name;		/* "cpld_cpu" / "cpld_ctrl" */
	struct miscdevice miscdev;
};

#endif /* __WB_ESPI_CPLD_H__ */
