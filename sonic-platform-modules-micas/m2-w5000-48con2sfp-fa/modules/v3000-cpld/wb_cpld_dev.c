// SPDX-License-Identifier: GPL-2.0
/*
 * wb_cpld_dev.c - WB CPLD device layer (miscdevice interface)
 *
 * Provides /dev/cpld_cpu and /dev/cpld_ctrl character devices for
 * generic register access via read/write/lseek operations.
 *
 * Usage from userspace:
 *   # Read register 0x10
 *   dd if=/dev/cpld_cpu bs=1 skip=16 count=1 | hexdump -C
 *
 *   # Write 0x42 to register 0x20
 *   printf '\x42' | dd of=/dev/cpld_cpu bs=1 seek=32 conv=notrunc
 *
 *   # Python example
 *   with open("/dev/cpld_cpu", "rb") as f:
 *       f.seek(0)
 *       version = f.read(6)
 *
 * Copyright (C) 2025
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/miscdevice.h>
#include "wb_cpld.h"

/* Global device array */
struct wb_cpld_dev *g_wb_cpld_devs[WB_CPLD_MAX_DEVS];

/* Temporary buffer size for read/write operations */
#define CPLD_TMP_BUF_SIZE  256

/*
 * file_operations implementations
 */

static int cpld_dev_open(struct inode *inode, struct file *file)
{
	unsigned int minor = iminor(inode);
	struct wb_cpld_dev *cpld;

	if (minor >= WB_CPLD_MAX_DEVS) {
		pr_err("wb_cpld: minor %u out of range\n", minor);
		return -ENODEV;
	}

	cpld = g_wb_cpld_devs[minor];
	if (!cpld) {
		pr_err("wb_cpld: device for minor %u not found\n", minor);
		return -ENODEV;
	}

	file->private_data = cpld;
	return 0;
}

static int cpld_dev_release(struct inode *inode, struct file *file)
{
	file->private_data = NULL;
	return 0;
}

static ssize_t cpld_dev_read(struct file *file, char __user *buf,
			     size_t count, loff_t *offset)
{
	struct wb_cpld_dev *cpld = file->private_data;
	u8 tmp_buf[CPLD_TMP_BUF_SIZE];
	size_t i, read_count;
	int ret;

	if (!cpld || !cpld->bus) {
		pr_err("wb_cpld: invalid device or bus\n");
		return -ENODEV;
	}

	/* Check offset range */
	if (*offset >= WB_CPLD_MAX_REG_OFFSET + 1)
		return 0;  /* EOF */

	/* Limit read count */
	read_count = min_t(size_t, count, WB_CPLD_MAX_REG_OFFSET + 1 - *offset);
	read_count = min_t(size_t, read_count, sizeof(tmp_buf));

	/* Read registers sequentially */
	for (i = 0; i < read_count; i++) {
		ret = wb_cpld_read_reg(cpld->bus, cpld->target,
				       *offset + i, &tmp_buf[i]);
		if (ret) {
			pr_err("wb_cpld: read failed at offset 0x%llx: %d\n",
			       *offset + i, ret);
			return ret;
		}
	}

	/* Copy to user space */
	if (copy_to_user(buf, tmp_buf, read_count)) {
		pr_err("wb_cpld: copy_to_user failed\n");
		return -EFAULT;
	}

	*offset += read_count;
	return read_count;
}

static ssize_t cpld_dev_write(struct file *file, const char __user *buf,
			      size_t count, loff_t *offset)
{
	struct wb_cpld_dev *cpld = file->private_data;
	u8 tmp_buf[CPLD_TMP_BUF_SIZE];
	size_t i, write_count;
	int ret;

	if (!cpld || !cpld->bus) {
		pr_err("wb_cpld: invalid device or bus\n");
		return -ENODEV;
	}

	/* Check offset range */
	if (*offset >= WB_CPLD_MAX_REG_OFFSET + 1)
		return -EINVAL;

	/* Limit write count */
	write_count = min_t(size_t, count, WB_CPLD_MAX_REG_OFFSET + 1 - *offset);
	write_count = min_t(size_t, write_count, sizeof(tmp_buf));

	/* Copy from user space */
	if (copy_from_user(tmp_buf, buf, write_count)) {
		pr_err("wb_cpld: copy_from_user failed\n");
		return -EFAULT;
	}

	/* Write registers sequentially */
	for (i = 0; i < write_count; i++) {
		ret = wb_cpld_write_reg(cpld->bus, cpld->target,
					*offset + i, tmp_buf[i]);
		if (ret) {
			pr_err("wb_cpld: write failed at offset 0x%llx: %d\n",
			       *offset + i, ret);
			return ret;
		}
	}

	*offset += write_count;
	return write_count;
}

static loff_t cpld_dev_llseek(struct file *file, loff_t offset, int whence)
{
	loff_t new_pos;

	switch (whence) {
	case SEEK_SET:
		new_pos = offset;
		break;
	case SEEK_CUR:
		new_pos = file->f_pos + offset;
		break;
	case SEEK_END:
		new_pos = WB_CPLD_MAX_REG_OFFSET + 1 + offset;
		break;
	default:
		return -EINVAL;
	}

	if (new_pos < 0 || new_pos > WB_CPLD_MAX_REG_OFFSET + 1)
		return -EINVAL;

	file->f_pos = new_pos;
	return new_pos;
}

static long cpld_dev_ioctl(struct file *file, unsigned int cmd,
			   unsigned long arg)
{
	/* Reserved for future extensions:
	 * - Bulk read/write
	 * - Get version info
	 * - Device reset
	 */
	return -ENOTTY;
}

static const struct file_operations cpld_dev_fops = {
	.owner		= THIS_MODULE,
	.open		= cpld_dev_open,
	.release	= cpld_dev_release,
	.read		= cpld_dev_read,
	.write		= cpld_dev_write,
	.llseek		= cpld_dev_llseek,
	.unlocked_ioctl	= cpld_dev_ioctl,
};

/*
 * Device registration functions
 */

/**
 * wb_cpld_dev_register - Register a CPLD device
 * @bus: CPLD bus structure
 * @target: CPLD target (CPU or CTRL)
 * @name: Device name ("cpld_cpu" or "cpld_ctrl")
 *
 * Creates a character device /dev/cpld_cpu or /dev/cpld_ctrl
 * for register access via standard read/write/lseek operations.
 *
 * Return: 0 on success, negative error code on failure
 */
int wb_cpld_dev_register(struct wb_cpld_bus *bus,
			     enum wb_cpld_target target,
			     const char *name)
{
	struct wb_cpld_dev *cpld;
	int minor, ret;

	if (!bus || !name)
		return -EINVAL;

	if (target != WB_CPLD_CPU && target != WB_CPLD_CTRL)
		return -EINVAL;

	minor = (target == WB_CPLD_CPU) ? 0 : 1;

	cpld = kzalloc(sizeof(*cpld), GFP_KERNEL);
	if (!cpld)
		return -ENOMEM;

	cpld->bus = bus;
	cpld->target = target;
	cpld->name = name;

	/* Configure miscdevice */
	cpld->miscdev.minor = MISC_DYNAMIC_MINOR;
	cpld->miscdev.name = name;
	cpld->miscdev.fops = &cpld_dev_fops;
	cpld->miscdev.mode = 0666;  /* rw-rw-rw- */

	/* Register miscdevice, creates /dev/cpld_cpu or /dev/cpld_ctrl */
	ret = misc_register(&cpld->miscdev);
	if (ret) {
		dev_err(bus->dev, "Failed to register misc device %s: %d\n",
			name, ret);
		kfree(cpld);
		return ret;
	}

	g_wb_cpld_devs[minor] = cpld;

	dev_info(bus->dev, "Registered /dev/%s (target=%d)\n", name, target);
	return 0;
}
EXPORT_SYMBOL_GPL(wb_cpld_dev_register);

/**
 * wb_cpld_dev_unregister - Unregister all CPLD devices
 * @bus: CPLD bus structure
 */
void wb_cpld_dev_unregister(struct wb_cpld_bus *bus)
{
	int i;

	for (i = 0; i < WB_CPLD_MAX_DEVS; i++) {
		if (g_wb_cpld_devs[i]) {
			misc_deregister(&g_wb_cpld_devs[i]->miscdev);
			kfree(g_wb_cpld_devs[i]);
			g_wb_cpld_devs[i] = NULL;
		}
	}
}
EXPORT_SYMBOL_GPL(wb_cpld_dev_unregister);

MODULE_DESCRIPTION("WB CPLD device layer");
MODULE_LICENSE("GPL");
