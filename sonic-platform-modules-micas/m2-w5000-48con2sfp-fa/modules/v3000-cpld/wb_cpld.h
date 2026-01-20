/* SPDX-License-Identifier: GPL-2.0 */
/*
 * v3000_cpld.h - WB CPLD driver via eSPI with GPIO chip select
 *
 * Copyright (C) 2025
 */

#ifndef __WB_CPLD_H__
#define __WB_CPLD_H__

#include <linux/types.h>
#include <linux/mutex.h>
#include <linux/device.h>
#include <linux/gpio/consumer.h>
#include <linux/miscdevice.h>

/* Default IO base and size (可通过module param配置) */
#define WB_CPLD_IO_BASE_DEFAULT	0x700
#define WB_CPLD_IO_SIZE_DEFAULT	0x100
#define WB_CPLD_MAX_REG_OFFSET	(WB_CPLD_IO_SIZE_DEFAULT - 1)

/* Maximum number of CPLD devices */
#define WB_CPLD_MAX_DEVS		2

/* CS_CTL target selection (hardware multiplexer control) */
enum wb_cpld_target {
	WB_CPLD_NONE  = 0,	/* CS_CTL=00, both CPLDs disconnected */
	WB_CPLD_CTRL  = 1,	/* CS_CTL=01, CTRL CPLD selected */
	WB_CPLD_CPU   = 2,	/* CS_CTL=10, CPU CPLD selected */
	/* 11 reserved */
};

/* eSPI backend abstraction */
struct wb_espi_backend {
	void *priv;		/* 厂商SDK handle if needed */
	u16 io_base;
	u16 io_size;
};

/* Bus层：管理共享资源（eSPI、GPIO、锁） */
struct wb_cpld_bus {
	struct device *dev;
	struct gpio_desc *cs_ctl0;	/* AGPIO9, CS_CTL[0] */
	struct gpio_desc *cs_ctl1;	/* AGPIO10, CS_CTL[1] */
	struct mutex lock;		/* Serialize transactions */
	struct wb_espi_backend espi;
	u16 io_base;
	u16 io_size;
};

/* Device层：代表一个CPLD逻辑设备 (使用miscdevice) */
struct wb_cpld_dev {
	struct wb_cpld_bus *bus;
	enum wb_cpld_target target;
	const char *name;		/* "cpld_cpu" or "cpld_ctrl" */
	struct miscdevice miscdev;	/* miscdevice结构 */
};

/* Global device array */
extern struct wb_cpld_dev *g_wb_cpld_devs[WB_CPLD_MAX_DEVS];

/* eSPI backend functions (implemented in v3000_cpld_espi.c) */
int wb_espi_init(struct wb_espi_backend *backend, u16 io_base, u16 io_size);
int wb_espi_read8(struct wb_espi_backend *backend, u16 offset, u8 *val);
int wb_espi_write8(struct wb_espi_backend *backend, u16 offset, u8 val);
void wb_espi_cleanup(struct wb_espi_backend *backend);

/* Bus layer functions (implemented in v3000_cpld_bus.c) */
void wb_cpld_set_target(struct wb_cpld_bus *bus, enum wb_cpld_target target);
int wb_cpld_read_reg(struct wb_cpld_bus *bus, enum wb_cpld_target target,
		     u16 offset, u8 *val);
int wb_cpld_write_reg(struct wb_cpld_bus *bus, enum wb_cpld_target target,
		      u16 offset, u8 val);

/* Device layer functions (implemented in v3000_cpld_dev.c) */
int wb_cpld_dev_register(struct wb_cpld_bus *bus, enum wb_cpld_target target,
			 const char *name);
void wb_cpld_dev_unregister(struct wb_cpld_bus *bus);

#endif /* __WB_CPLD_H__ */
