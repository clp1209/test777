// SPDX-License-Identifier: GPL-2.0
/*
 * wb_espi_cpld.c - WB eSPI CPLD driver (miscdevice interface)
 *
 * Exposes two nodes:
 *   - /dev/cpld_cpu
 *   - /dev/cpld_ctrl
 *
 * Access model:
 *   - IO decode window: io_base..io_base+io_size-1
 *   - CS selection: CS_CTL[1:0] via two GPIOs (AGPIO9/AGPIO10 on V3000 board)
 *     Hardware defines:
 *       CS_CTL[1:0] = {gpio10, gpio9}
 *       01 = CS->CS1 (CTRL CPLD)
 *       10 = CS->CS2 (CPU CPLD)
 *       11 = Reserved
 *       00 = Undefined (default low state)
 *
 * This module intentionally does NOT configure IO decode registers.
 * It relies on BIOS/firmware to configure the IO decode window. We only
 * query and validate the current decode configuration and log it for debug.
 *
 * TODO: If the final platform requires "direct" Peripheral Channel
 *       transactions without CPU IO decode, switch the backend to that
 *       controller API instead of decode + IO cycles.
 *
 */

#include <linux/acpi.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/stat.h>
#include <linux/mutex.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#include "wb_espi_cpld.h"

/*
 * Vendor eSPI controller kernel API (exported by espi-amd.c).
 *
 * We resolve them dynamically so wb_espi_cpld can load even if the controller
 * driver is built out-of-tree / loaded later.
 */
extern int amd_espi_enable_io_decode(u16 base, u16 size);
extern int amd_espi_get_io_decode_range(unsigned int idx, u16 *base, u8 *size_field, bool *enabled);
extern int amd_espi_io_decode_covers(u16 base, u16 size);
extern int amd_espi_io_read8(u16 port, u8 *val);
extern int amd_espi_io_write8(u16 port, u8 val);

static unsigned int cs_delay_us = 10;
module_param(cs_delay_us, uint, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(cs_delay_us, "Delay after CS_CTL switch in microseconds (default: 10)");

static int g_wb_espi_cpld_debug;
static int g_wb_espi_cpld_error = 1;
module_param(g_wb_espi_cpld_debug, int, S_IRUGO | S_IWUSR);
module_param(g_wb_espi_cpld_error, int, S_IRUGO | S_IWUSR);

#define WB_ESPI_CPLD_DEBUG_VERBOSE(fmt, args...) do {				\
	if (g_wb_espi_cpld_debug)						\
		printk(KERN_INFO "[WB_ESPI_CPLD][VER][func:%s line:%d] " fmt,	\
		       __func__, __LINE__, ##args);				\
} while (0)

#define WB_ESPI_CPLD_DEBUG_ERROR(fmt, args...) do {				\
	if (g_wb_espi_cpld_error)						\
		printk(KERN_ERR "[WB_ESPI_CPLD][ERR][func:%s line:%d] " fmt,	\
		       __func__, __LINE__, ##args);				\
} while (0)

/* Global device array (minor -> device) */
static struct wb_cpld_dev *g_wb_cpld_devs[WB_CPLD_MAX_DEVS];

static void wb_espi_cpld_set_target(struct wb_cpld_bus *bus, enum wb_cpld_target target)
{
	/* GPIO polarity may be active-low on some boards. */
	gpiod_set_value_cansleep(bus->cs_ctl0, target & 0x1);
	gpiod_set_value_cansleep(bus->cs_ctl1, (target >> 1) & 0x1);

	/* TODO: confirm whether delay is needed and what value is correct. */
	if (cs_delay_us)
		udelay(cs_delay_us);
}

static int wb_espi_bind_controller(struct wb_cpld_bus *bus)
{
	int ret, i;
	int covers;
	u16 rb;
	u8 rs;
	bool en;

	bus->ops.enable_io_decode = symbol_get(amd_espi_enable_io_decode);
	bus->ops.get_io_decode_range = symbol_get(amd_espi_get_io_decode_range);
	bus->ops.io_decode_covers = symbol_get(amd_espi_io_decode_covers);
	bus->ops.io_read8 = symbol_get(amd_espi_io_read8);
	bus->ops.io_write8 = symbol_get(amd_espi_io_write8);

	if (!bus->ops.get_io_decode_range || !bus->ops.io_decode_covers ||
	    !bus->ops.io_read8 || !bus->ops.io_write8) {
		WB_ESPI_CPLD_DEBUG_ERROR("espi-amd API not ready (symbols missing), defer probe\n");
		ret = -EPROBE_DEFER;
		goto err_put;
	}

	for (i = 0; i < 4; i++) {
		ret = bus->ops.get_io_decode_range(i, &rb, &rs, &en);
		if (ret) {
			WB_ESPI_CPLD_DEBUG_ERROR("get io decode range%d failed: %d\n", i, ret);
			goto err_put;
		}

		/*
		 * rs is the raw HW size field ("last offset" semantics).
		 * For humans we also print a computed size = rs + 1.
		 */
		dev_info(bus->dev, "espi io_decode range%d: %s base=0x%04x size_field=0x%02x size=0x%03x\n",
			 i, en ? "EN" : "DIS", rb, rs, (unsigned int)rs + 1);
	}

	covers = bus->ops.io_decode_covers(bus->io_base, bus->io_size);
	if (covers <= 0) {
		WB_ESPI_CPLD_DEBUG_ERROR("BIOS IO decode does not cover requested window: base=0x%x size=0x%x covers=%d\n",
					 bus->io_base, bus->io_size, covers);
		ret = -ENODEV;
		goto err_put;
	}

	return 0;

err_put:
	if (bus->ops.io_write8) {
		symbol_put(amd_espi_io_write8);
		bus->ops.io_write8 = NULL;
	}
	if (bus->ops.io_read8) {
		symbol_put(amd_espi_io_read8);
		bus->ops.io_read8 = NULL;
	}
	if (bus->ops.io_decode_covers) {
		symbol_put(amd_espi_io_decode_covers);
		bus->ops.io_decode_covers = NULL;
	}
	if (bus->ops.get_io_decode_range) {
		symbol_put(amd_espi_get_io_decode_range);
		bus->ops.get_io_decode_range = NULL;
	}
	return ret;
}

static void wb_espi_unbind_controller(struct wb_cpld_bus *bus)
{
	if (bus->ops.io_write8) {
		symbol_put(amd_espi_io_write8);
		bus->ops.io_write8 = NULL;
	}
	if (bus->ops.io_read8) {
		symbol_put(amd_espi_io_read8);
		bus->ops.io_read8 = NULL;
	}
	if (bus->ops.io_decode_covers) {
		symbol_put(amd_espi_io_decode_covers);
		bus->ops.io_decode_covers = NULL;
	}
	if (bus->ops.get_io_decode_range) {
		symbol_put(amd_espi_get_io_decode_range);
		bus->ops.get_io_decode_range = NULL;
	}
}

static int wb_espi_read8(struct wb_cpld_bus *bus, u16 offset, u8 *val)
{
	u16 port;
	int ret;

	if (offset >= bus->io_size)
		return -EINVAL;

	port = bus->io_base + offset;
	if (!bus->ops.io_read8)
		return -ENODEV;

	ret = bus->ops.io_read8(port, val);
	return ret;
}

static int wb_espi_write8(struct wb_cpld_bus *bus, u16 offset, u8 val)
{
	u16 port;
	int ret;

	if (offset >= bus->io_size)
		return -EINVAL;

	port = bus->io_base + offset;
	if (!bus->ops.io_write8)
		return -ENODEV;

	ret = bus->ops.io_write8(port, val);
	return ret;
}

static int wb_cpld_read_reg(struct wb_cpld_bus *bus, enum wb_cpld_target target,
			    u16 offset, u8 *val)
{
	int ret;

	if (!bus || !val)
		return -EINVAL;

	if (target != WB_CPLD_CPU && target != WB_CPLD_CTRL)
		return -EINVAL;

	mutex_lock(&bus->lock);
	wb_espi_cpld_set_target(bus, target);
	ret = wb_espi_read8(bus, offset, val);
	wb_espi_cpld_set_target(bus, WB_CPLD_NONE);
	mutex_unlock(&bus->lock);

	return ret;
}

static int wb_cpld_write_reg(struct wb_cpld_bus *bus, enum wb_cpld_target target,
			     u16 offset, u8 val)
{
	int ret;

	if (!bus)
		return -EINVAL;

	if (target != WB_CPLD_CPU && target != WB_CPLD_CTRL)
		return -EINVAL;

	mutex_lock(&bus->lock);
	wb_espi_cpld_set_target(bus, target);
	ret = wb_espi_write8(bus, offset, val);
	wb_espi_cpld_set_target(bus, WB_CPLD_NONE);
	mutex_unlock(&bus->lock);

	return ret;
}

#define CPLD_TMP_BUF_SIZE 256

static int wb_cpld_open(struct inode *inode, struct file *file)
{
	unsigned int minor = iminor(inode);
	struct wb_cpld_dev *cpld;

	if (minor >= WB_CPLD_MAX_DEVS)
		return -ENODEV;

	cpld = g_wb_cpld_devs[minor];
	if (!cpld)
		return -ENODEV;

	file->private_data = cpld;
	return 0;
}

static int wb_cpld_release(struct inode *inode, struct file *file)
{
	file->private_data = NULL;
	return 0;
}

static ssize_t wb_cpld_read(struct file *file, char __user *buf,
			    size_t count, loff_t *ppos)
{
	struct wb_cpld_dev *cpld = file->private_data;
	u8 tmp[CPLD_TMP_BUF_SIZE];
	size_t i, n;
	int ret;

	if (!cpld || !cpld->bus)
		return -ENODEV;

	if (*ppos >= cpld->bus->io_size)
		return 0;

	n = min_t(size_t, count, cpld->bus->io_size - *ppos);
	n = min_t(size_t, n, sizeof(tmp));

	for (i = 0; i < n; i++) {
		ret = wb_cpld_read_reg(cpld->bus, cpld->target, *ppos + i, &tmp[i]);
		if (ret) {
			WB_ESPI_CPLD_DEBUG_ERROR("read failed at off=0x%llx ret=%d\n",
						 *ppos + i, ret);
			return ret;
		}
	}

	if (copy_to_user(buf, tmp, n))
		return -EFAULT;

	*ppos += n;
	return n;
}

static ssize_t wb_cpld_write(struct file *file, const char __user *buf,
			     size_t count, loff_t *ppos)
{
	struct wb_cpld_dev *cpld = file->private_data;
	u8 tmp[CPLD_TMP_BUF_SIZE];
	size_t i, n;
	int ret;

	if (!cpld || !cpld->bus)
		return -ENODEV;

	if (*ppos >= cpld->bus->io_size)
		return -EINVAL;

	n = min_t(size_t, count, cpld->bus->io_size - *ppos);
	n = min_t(size_t, n, sizeof(tmp));

	if (copy_from_user(tmp, buf, n))
		return -EFAULT;

	for (i = 0; i < n; i++) {
		ret = wb_cpld_write_reg(cpld->bus, cpld->target, *ppos + i, tmp[i]);
		if (ret) {
			WB_ESPI_CPLD_DEBUG_ERROR("write failed at off=0x%llx ret=%d\n",
						 *ppos + i, ret);
			return ret;
		}
	}

	*ppos += n;
	return n;
}

static loff_t wb_cpld_llseek(struct file *file, loff_t off, int whence)
{
	struct wb_cpld_dev *cpld = file->private_data;
	loff_t new_pos;

	if (!cpld || !cpld->bus)
		return -ENODEV;

	switch (whence) {
	case SEEK_SET:
		new_pos = off;
		break;
	case SEEK_CUR:
		new_pos = file->f_pos + off;
		break;
	case SEEK_END:
		new_pos = cpld->bus->io_size + off;
		break;
	default:
		return -EINVAL;
	}

	if (new_pos < 0 || new_pos > cpld->bus->io_size)
		return -EINVAL;

	file->f_pos = new_pos;
	return new_pos;
}

static const struct file_operations wb_cpld_fops = {
	.owner = THIS_MODULE,
	.open = wb_cpld_open,
	.release = wb_cpld_release,
	.read = wb_cpld_read,
	.write = wb_cpld_write,
	.llseek = wb_cpld_llseek,
};

static int wb_cpld_register_devs(struct wb_cpld_bus *bus)
{
	static const struct {
		enum wb_cpld_target target;
		const char *name;
		int idx;
	} devs[WB_CPLD_MAX_DEVS] = {
		{ WB_CPLD_CPU,  "cpld_cpu",  0 },
		{ WB_CPLD_CTRL, "cpld_ctrl", 1 },
	};
	int i, ret;

	for (i = 0; i < WB_CPLD_MAX_DEVS; i++) {
		struct wb_cpld_dev *cpld;

		cpld = kzalloc(sizeof(*cpld), GFP_KERNEL);
		if (!cpld)
			return -ENOMEM;

		cpld->bus = bus;
		cpld->target = devs[i].target;
		cpld->name = devs[i].name;
		cpld->miscdev.minor = MISC_DYNAMIC_MINOR;
		cpld->miscdev.name = cpld->name;
		cpld->miscdev.fops = &wb_cpld_fops;
		cpld->miscdev.mode = 0666;

		ret = misc_register(&cpld->miscdev);
		if (ret) {
			WB_ESPI_CPLD_DEBUG_ERROR("misc_register(%s) failed: %d\n",
						 cpld->name, ret);
			kfree(cpld);
			return ret;
		}

		g_wb_cpld_devs[devs[i].idx] = cpld;
		dev_info(bus->dev, "registered /dev/%s\n", cpld->name);
	}

	return 0;
}

static void wb_cpld_unregister_devs(void)
{
	int i;

	for (i = 0; i < WB_CPLD_MAX_DEVS; i++) {
		if (!g_wb_cpld_devs[i])
			continue;
		misc_deregister(&g_wb_cpld_devs[i]->miscdev);
		kfree(g_wb_cpld_devs[i]);
		g_wb_cpld_devs[i] = NULL;
	}
}

static int wb_espi_cpld_setup_gpios(struct wb_cpld_bus *bus)
{
	if (bus->cs_ctl0_gpio < 0 || bus->cs_ctl1_gpio < 0)
		return -EINVAL;

	if (gpio_request(bus->cs_ctl0_gpio, "wb_espi_cpld_cs_ctl0"))
		return -EBUSY;
	if (gpio_request(bus->cs_ctl1_gpio, "wb_espi_cpld_cs_ctl1")) {
		gpio_free(bus->cs_ctl0_gpio);
		return -EBUSY;
	}

	gpio_direction_output(bus->cs_ctl0_gpio, 0);
	gpio_direction_output(bus->cs_ctl1_gpio, 0);

	bus->cs_ctl0 = gpio_to_desc(bus->cs_ctl0_gpio);
	bus->cs_ctl1 = gpio_to_desc(bus->cs_ctl1_gpio);
	if (!bus->cs_ctl0 || !bus->cs_ctl1) {
		gpio_free(bus->cs_ctl1_gpio);
		gpio_free(bus->cs_ctl0_gpio);
		return -ENODEV;
	}

	return 0;
}

static void wb_espi_cpld_cleanup_gpios(struct wb_cpld_bus *bus)
{
	gpio_free(bus->cs_ctl1_gpio);
	gpio_free(bus->cs_ctl0_gpio);
}

static int wb_espi_cpld_probe(struct platform_device *pdev)
{
	struct wb_cpld_bus *bus;
	struct wb_espi_cpld_pdata *pdata = dev_get_platdata(&pdev->dev);
	int ret;

	if (!pdata)
		return -EINVAL;

	bus = devm_kzalloc(&pdev->dev, sizeof(*bus), GFP_KERNEL);
	if (!bus)
		return -ENOMEM;

	bus->dev = &pdev->dev;
	bus->io_base = pdata->io_base;
	bus->io_size = pdata->io_size;
	bus->cs_ctl0_gpio = pdata->cs_ctl0_gpio;
	bus->cs_ctl1_gpio = pdata->cs_ctl1_gpio;
	mutex_init(&bus->lock);

	ret = wb_espi_cpld_setup_gpios(bus);
	if (ret)
		return ret;

	ret = wb_espi_bind_controller(bus);
	if (ret)
		goto err_gpio;

	/*
	 * Request IO region to avoid accidental conflicts.
	 * Decode routing is configured by the eSPI controller driver.
	 */
	if (!request_region(bus->io_base, bus->io_size, "wb_espi_cpld")) {
		WB_ESPI_CPLD_DEBUG_ERROR("request_region [0x%x..0x%x] failed\n",
					 bus->io_base,
					 bus->io_base + bus->io_size - 1);
		ret = -EBUSY;
		goto err_gpio;
	}

	/* Safe default: disconnect both CPLDs. */
	wb_espi_cpld_set_target(bus, WB_CPLD_NONE);

	ret = wb_cpld_register_devs(bus);
	if (ret)
		goto err_region;

	platform_set_drvdata(pdev, bus);

	dev_info(&pdev->dev, "WB CPLD ready (io_base=0x%x io_size=0x%x)\n",
		 bus->io_base, bus->io_size);
	return 0;

err_region:
	release_region(bus->io_base, bus->io_size);
err_gpio:
	wb_espi_unbind_controller(bus);
	wb_espi_cpld_cleanup_gpios(bus);
	return ret;
}

static int wb_espi_cpld_remove(struct platform_device *pdev)
{
	struct wb_cpld_bus *bus = platform_get_drvdata(pdev);

	wb_cpld_unregister_devs();
	wb_espi_cpld_set_target(bus, WB_CPLD_NONE);
	release_region(bus->io_base, bus->io_size);
	wb_espi_unbind_controller(bus);
	wb_espi_cpld_cleanup_gpios(bus);
	return 0;
}

#ifdef CONFIG_ACPI
static const struct acpi_device_id wb_cpld_acpi_match[] = {
	{ "WBCPLD", 0 }, /* TODO: replace with real HID */
	{ }
};
MODULE_DEVICE_TABLE(acpi, wb_cpld_acpi_match);
#endif

static struct platform_driver wb_cpld_driver = {
	.probe = wb_espi_cpld_probe,
	.remove = wb_espi_cpld_remove,
	.driver = {
		.name = "wb-espi-cpld",
		.acpi_match_table = ACPI_PTR(wb_cpld_acpi_match),
	},
};

module_platform_driver(wb_cpld_driver);

MODULE_AUTHOR("Platform Driver Team");
MODULE_DESCRIPTION("WB eSPI CPLD driver (IO decode + GPIO CS)");
MODULE_LICENSE("GPL");
