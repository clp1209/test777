#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif


static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xe6ba4a90, "__platform_driver_register" },
	{ 0xdd7a298d, "i2c_del_adapter" },
	{ 0x44c7d5dc, "platform_driver_unregister" },
	{ 0xbb526d92, "pcie_device_func_read" },
	{ 0x36ee675d, "indirect_device_func_read" },
	{ 0x7751408f, "i2c_device_func_read" },
	{ 0x64b4b7aa, "io_device_func_read" },
	{ 0x53b0078, "filp_open" },
	{ 0x92a45316, "iov_iter_kvec" },
	{ 0x1af5384e, "vfs_iter_read" },
	{ 0x4cda5dcb, "filp_close" },
	{ 0xb1ba28f3, "spi_device_func_read" },
	{ 0xa19b956, "__stack_chk_fail" },
	{ 0x92997ed8, "_printk" },
	{ 0x4950bb37, "pcie_device_func_write" },
	{ 0x4206d81c, "indirect_device_func_write" },
	{ 0x2a9ad4c3, "i2c_device_func_write" },
	{ 0x618de573, "io_device_func_write" },
	{ 0xf2c40a08, "vfs_iter_write" },
	{ 0x260a70f8, "vfs_fsync" },
	{ 0x73ef02bc, "spi_device_func_write" },
	{ 0xd3ff68a1, "devm_kmalloc" },
	{ 0x8a856723, "of_property_read_variable_u32_array" },
	{ 0xe1dd9dcf, "of_property_read_string" },
	{ 0x3b9189d7, "of_find_property" },
	{ 0xd9a5ea54, "__init_waitqueue_head" },
	{ 0x7f02188f, "__msecs_to_jiffies" },
	{ 0x34a86ef4, "i2c_add_adapter" },
	{ 0xc643eace, "i2c_add_numbered_adapter" },
	{ 0x947a19c4, "_dev_err" },
	{ 0xe092a2f2, "_dev_info" },
	{ 0xc3055d20, "usleep_range_state" },
	{ 0x63b61982, "param_ops_int" },
	{ 0xc9d7e676, "module_layout" },
};

MODULE_INFO(depends, "wb_pcie_dev,wb_indirect_dev,wb_i2c_dev,wb_io_dev,wb_spi_dev");

MODULE_ALIAS("of:N*T*Cwb-fpga-i2c");
MODULE_ALIAS("of:N*T*Cwb-fpga-i2cC*");
