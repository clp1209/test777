// SPDX-License-Identifier: GPL-2.0
/*
 * v3000_cpld_espi.c - eSPI backend adapter for WB CPLD driver
 *
 * This file encapsulates the AMD eSPI SDK API calls to isolate vendor
 * dependencies. If the SDK API changes, only this file needs updating.
 *
 * Copyright (C) 2025
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/delay.h>
#include "wb_cpld.h"

/*
 * eSPI SDK API usage notes:
 * Based on espi-amd.c analysis, the driver provides Peripheral Channel
 * IO access via espi_periph_io_read() and espi_periph_io_write().
 *
 * These functions use standard inb()/outb() after validating the IO
 * decode range configuration.
 *
 * For WB CPLD access:
 * - IO base (default 0x400) must be configured in eSPI IO decode range
 * - CS_CTL switching is handled by CPLD bus layer via GPIO
 * - Each 8-bit IO access goes through eSPI Peripheral Channel
 */

/**
 * wb_espi_init - Initialize eSPI backend
 * @backend: eSPI backend structure
 * @io_base: IO port base address (e.g., 0x700)
 * @io_size: IO port window size (e.g., 0x100)
 *
 * Initialize the eSPI backend. For AMD platforms, the eSPI controller
 * should already be initialized by espi-amd.ko, and IO decode ranges
 * should be configured in BIOS or by platform init code.
 *
 * Return: 0 on success, negative error code on failure
 */
int wb_espi_init(struct wb_espi_backend *backend, u16 io_base, u16 io_size)
{
	if (!backend)
		return -EINVAL;

	backend->io_base = io_base;
	backend->io_size = io_size;
	backend->priv = NULL;

	/*
	 * TODO: Verify IO decode range is configured in eSPI controller
	 * This may require calling into espi-amd driver or checking ACPI
	 * For now, assume BIOS/platform code has configured it
	 */

	pr_debug("wb_cpld: eSPI backend initialized (io_base=0x%x, size=0x%x)\n",
		 io_base, io_size);

	return 0;
}
EXPORT_SYMBOL_GPL(wb_espi_init);

/**
 * wb_espi_read8 - Read 8-bit value from eSPI IO port
 * @backend: eSPI backend structure
 * @offset: Register offset within IO window
 * @val: Pointer to store read value
 *
 * Performs 8-bit IO read through eSPI Peripheral Channel.
 * The actual CS_CTL switching must be done by caller (bus layer).
 *
 * Implementation: Direct inb() call
 * Rationale: espi-amd driver validates decode range in its ioctl handlers,
 * but for kernel driver access, we use direct IO port access. The eSPI
 * controller hardware automatically routes IO accesses to Peripheral Channel.
 *
 * Return: 0 on success, negative error code on failure
 */
int wb_espi_read8(struct wb_espi_backend *backend, u16 offset, u8 *val)
{
	u16 port;

	if (!backend || !val)
		return -EINVAL;

	if (offset >= backend->io_size) {
		pr_err("wb_cpld: offset 0x%x exceeds io_size 0x%x\n",
		       offset, backend->io_size);
		return -EINVAL;
	}

	port = backend->io_base + offset;

	/*
	 * Direct IO port read
	 * eSPI controller hardware routes this to Peripheral Channel
	 * based on configured IO decode ranges
	 */
	*val = inb(port);

	return 0;
}
EXPORT_SYMBOL_GPL(wb_espi_read8);

/**
 * wb_espi_write8 - Write 8-bit value to eSPI IO port
 * @backend: eSPI backend structure
 * @offset: Register offset within IO window
 * @val: Value to write
 *
 * Performs 8-bit IO write through eSPI Peripheral Channel.
 * The actual CS_CTL switching must be done by caller (bus layer).
 *
 * Return: 0 on success, negative error code on failure
 */
int wb_espi_write8(struct wb_espi_backend *backend, u16 offset, u8 val)
{
	u16 port;

	if (!backend)
		return -EINVAL;

	if (offset >= backend->io_size) {
		pr_err("wb_cpld: offset 0x%x exceeds io_size 0x%x\n",
		       offset, backend->io_size);
		return -EINVAL;
	}

	port = backend->io_base + offset;

	/*
	 * Direct IO port write
	 * eSPI controller hardware routes this to Peripheral Channel
	 */
	outb(val, port);

	return 0;
}
EXPORT_SYMBOL_GPL(wb_espi_write8);

/**
 * wb_espi_cleanup - Cleanup eSPI backend resources
 * @backend: eSPI backend structure
 *
 * Cleanup any resources allocated during init.
 * For current implementation (direct IO), nothing to do.
 */
void wb_espi_cleanup(struct wb_espi_backend *backend)
{
	if (!backend)
		return;

	pr_debug("wb_cpld: eSPI backend cleanup\n");
	/* Nothing to cleanup for direct IO implementation */
}
EXPORT_SYMBOL_GPL(wb_espi_cleanup);

MODULE_DESCRIPTION("WB CPLD eSPI backend adapter");
MODULE_LICENSE("GPL");
