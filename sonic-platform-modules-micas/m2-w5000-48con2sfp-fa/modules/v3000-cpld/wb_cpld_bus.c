// SPDX-License-Identifier: GPL-2.0
/*
 * v3000_cpld_bus.c - WB CPLD bus layer
 *
 * This layer manages shared resources (eSPI backend, GPIO CS control, lock)
 * and provides atomic transaction-level access to CPLD registers.
 *
 * Copyright (C) 2025
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/acpi.h>
#include <linux/gpio/consumer.h>
#include <linux/delay.h>
#include "wb_cpld.h"

/* Module parameters for IO base/size (可在加载时调整) */
static u16 io_base = WB_CPLD_IO_BASE_DEFAULT;
static u16 io_size = WB_CPLD_IO_SIZE_DEFAULT;
module_param(io_base, ushort, 0444);
MODULE_PARM_DESC(io_base, "eSPI IO base address (default: 0x700)");
module_param(io_size, ushort, 0444);
MODULE_PARM_DESC(io_size, "eSPI IO window size (default: 0x100)");

/* CS_CTL switching delay in microseconds (可调整) */
static unsigned int cs_delay_us = 10;
module_param(cs_delay_us, uint, 0644);
MODULE_PARM_DESC(cs_delay_us, "Delay after CS_CTL switch in microseconds (default: 10)");

static struct wb_cpld_bus *g_cpld_bus;

/**
 * wb_cpld_set_target - Switch CS_CTL to select target CPLD
 * @bus: CPLD bus structure
 * @target: Target CPLD (NONE/CTRL/CPU)
 *
 * Controls AGPIO9/AGPIO10 to set CS_CTL[1:0]:
 *   CS_CTL=00 (NONE):  AGPIO10=0, AGPIO9=0 (both disconnected)
 *   CS_CTL=01 (CTRL):  AGPIO10=0, AGPIO9=1 (CTRL CPLD selected)
 *   CS_CTL=10 (CPU):   AGPIO10=1, AGPIO9=0 (CPU CPLD selected)
 *
 * Note: GPIOs are configured as ACTIVE_HIGH, but hardware may be active-low.
 * TODO: Verify GPIO polarity with hardware team and adjust if needed.
 */
void wb_cpld_set_target(struct wb_cpld_bus *bus, enum wb_cpld_target target)
{
	/*
	 * CS_CTL bit mapping:
	 * cs_ctl0 = AGPIO9  = CS_CTL[0]
	 * cs_ctl1 = AGPIO10 = CS_CTL[1]
	 */
	gpiod_set_value_cansleep(bus->cs_ctl0, target & 0x1);
	gpiod_set_value_cansleep(bus->cs_ctl1, (target >> 1) & 0x1);

	/*
	 * Add delay for CS signal stabilization
	 * TODO: Verify if delay is needed based on hardware testing
	 */
	if (cs_delay_us > 0)
		udelay(cs_delay_us);
}
EXPORT_SYMBOL_GPL(wb_cpld_set_target);

/**
 * wb_cpld_read_reg - Read CPLD register with atomic CS switching
 * @bus: CPLD bus structure
 * @target: Target CPLD (CTRL or CPU)
 * @offset: Register offset (0x00 ~ 0xFF)
 * @val: Pointer to store read value
 *
 * Atomic transaction:
 *   1. Lock mutex
 *   2. Set CS_CTL to target
 *   3. Read via eSPI
 *   4. Restore CS_CTL to 00
 *   5. Unlock mutex
 *
 * Return: 0 on success, negative error code on failure
 */
int wb_cpld_read_reg(struct wb_cpld_bus *bus, enum wb_cpld_target target,
		     u16 offset, u8 *val)
{
	int ret;

	if (!bus || !val)
		return -EINVAL;

	if (target != WB_CPLD_CTRL && target != WB_CPLD_CPU)
		return -EINVAL;

	mutex_lock(&bus->lock);

	/* Set CS_CTL to target CPLD */
	wb_cpld_set_target(bus, target);

	/* Read via eSPI backend */
	ret = wb_espi_read8(&bus->espi, offset, val);

	/* Restore CS_CTL to default (both disconnected) */
	wb_cpld_set_target(bus, WB_CPLD_NONE);

	mutex_unlock(&bus->lock);

	return ret;
}
EXPORT_SYMBOL_GPL(wb_cpld_read_reg);

/**
 * wb_cpld_write_reg - Write CPLD register with atomic CS switching
 * @bus: CPLD bus structure
 * @target: Target CPLD (CTRL or CPU)
 * @offset: Register offset (0x00 ~ 0xFF)
 * @val: Value to write
 *
 * Return: 0 on success, negative error code on failure
 */
int wb_cpld_write_reg(struct wb_cpld_bus *bus, enum wb_cpld_target target,
		      u16 offset, u8 val)
{
	int ret;

	if (!bus)
		return -EINVAL;

	if (target != WB_CPLD_CTRL && target != WB_CPLD_CPU)
		return -EINVAL;

	mutex_lock(&bus->lock);

	/* Set CS_CTL to target CPLD */
	wb_cpld_set_target(bus, target);

	/* Write via eSPI backend */
	ret = wb_espi_write8(&bus->espi, offset, val);

	/* Restore CS_CTL to default */
	wb_cpld_set_target(bus, WB_CPLD_NONE);

	mutex_unlock(&bus->lock);

	return ret;
}
EXPORT_SYMBOL_GPL(wb_cpld_write_reg);

/*
 * Platform device probe/remove
 */

static int wb_cpld_bus_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct wb_cpld_bus *bus;
	int ret;

	dev_info(dev, "WB CPLD bus driver probing (io_base=0x%x, io_size=0x%x)\n",
		 io_base, io_size);

	bus = devm_kzalloc(dev, sizeof(*bus), GFP_KERNEL);
	if (!bus)
		return -ENOMEM;

	bus->dev = dev;
	bus->io_base = io_base;
	bus->io_size = io_size;
	mutex_init(&bus->lock);

	/*
	 * Get GPIO descriptors for CS_CTL control
	 * TODO: Update resource names when ACPI table is finalized
	 * Expected ACPI resources:
	 *   GpioIo(...) { AGPIO9 }   // cs-ctl0
	 *   GpioIo(...) { AGPIO10 }  // cs-ctl1
	 */
	bus->cs_ctl0 = devm_gpiod_get(dev, "cs-ctl0", GPIOD_OUT_LOW);
	if (IS_ERR(bus->cs_ctl0))
		return dev_err_probe(dev, PTR_ERR(bus->cs_ctl0),
			"Failed to get cs-ctl0 GPIO (AGPIO9)\n"
			"TODO: Check ACPI CRS resource mapping\n");

	bus->cs_ctl1 = devm_gpiod_get(dev, "cs-ctl1", GPIOD_OUT_LOW);
	if (IS_ERR(bus->cs_ctl1))
		return dev_err_probe(dev, PTR_ERR(bus->cs_ctl1),
			"Failed to get cs-ctl1 GPIO (AGPIO10)\n"
			"TODO: Check ACPI CRS resource mapping\n");

	/* Initialize eSPI backend */
	ret = wb_espi_init(&bus->espi, bus->io_base, bus->io_size);
	if (ret)
		return dev_err_probe(dev, ret,
			"Failed to initialize eSPI backend\n"
			"TODO: Verify eSPI IO decode range is configured\n");

	/* Ensure CS_CTL starts at 00 (both CPLDs disconnected) */
	wb_cpld_set_target(bus, WB_CPLD_NONE);

	platform_set_drvdata(pdev, bus);
	g_cpld_bus = bus;

	/* Register child devices for cpld_cpu and cpld_ctrl */
	ret = wb_cpld_dev_register(bus, WB_CPLD_CPU, "cpld_cpu");
	if (ret) {
		dev_err(dev, "Failed to register cpld_cpu device: %d\n", ret);
		goto err_cleanup_espi;
	}

	ret = wb_cpld_dev_register(bus, WB_CPLD_CTRL, "cpld_ctrl");
	if (ret) {
		dev_err(dev, "Failed to register cpld_ctrl device: %d\n", ret);
		wb_cpld_dev_unregister(bus);
		goto err_cleanup_espi;
	}

	dev_info(dev, "WB CPLD bus initialized successfully\n");
	return 0;

err_cleanup_espi:
	wb_espi_cleanup(&bus->espi);
	g_cpld_bus = NULL;
	return ret;
}

static int wb_cpld_bus_remove(struct platform_device *pdev)
{
	struct wb_cpld_bus *bus = platform_get_drvdata(pdev);

	dev_info(bus->dev, "WB CPLD bus driver removing\n");

	wb_cpld_dev_unregister(bus);
	wb_espi_cleanup(&bus->espi);

	/* Restore CS_CTL to safe state */
	wb_cpld_set_target(bus, WB_CPLD_NONE);

	g_cpld_bus = NULL;

	return 0;
}

/*
 * ACPI device matching
 * TODO: Replace placeholder HID with actual value from vendor ACPI table
 */
#ifdef CONFIG_ACPI
static const struct acpi_device_id wb_cpld_acpi_match[] = {
	{ "WBCPLD  ", 0 },  /* TODO: Update with real HID from ACPI */
	{ }
};
MODULE_DEVICE_TABLE(acpi, wb_cpld_acpi_match);
#endif

static struct platform_driver wb_cpld_bus_driver = {
	.probe = wb_cpld_bus_probe,
	.remove = wb_cpld_bus_remove,
	.driver = {
		.name = "wb-cpld-bus",
		.acpi_match_table = ACPI_PTR(wb_cpld_acpi_match),
	},
};

module_platform_driver(wb_cpld_bus_driver);

MODULE_AUTHOR("Platform Driver Team");
MODULE_DESCRIPTION("WB CPLD bus driver");
MODULE_LICENSE("GPL");
