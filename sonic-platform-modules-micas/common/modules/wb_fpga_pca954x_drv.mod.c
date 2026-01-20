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
	{ 0xa534e67, "i2c_register_driver" },
	{ 0x4cfef8c, "i2c_mux_del_adapters" },
	{ 0x65487097, "__x86_indirect_thunk_rax" },
	{ 0xd73637bb, "i2c_mux_alloc" },
	{ 0x8a856723, "of_property_read_variable_u32_array" },
	{ 0x947a19c4, "_dev_err" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x92997ed8, "_printk" },
	{ 0xe092a2f2, "_dev_info" },
	{ 0xfd5dfbcb, "i2c_mux_add_adapter" },
	{ 0xbe239ab1, "i2c_del_driver" },
	{ 0x4950bb37, "pcie_device_func_write" },
	{ 0x2a9ad4c3, "i2c_device_func_write" },
	{ 0x4206d81c, "indirect_device_func_write" },
	{ 0x618de573, "io_device_func_write" },
	{ 0x53b0078, "filp_open" },
	{ 0x92a45316, "iov_iter_kvec" },
	{ 0xf2c40a08, "vfs_iter_write" },
	{ 0x260a70f8, "vfs_fsync" },
	{ 0x4cda5dcb, "filp_close" },
	{ 0x73ef02bc, "spi_device_func_write" },
	{ 0xa19b956, "__stack_chk_fail" },
	{ 0x5d729fdd, "i2c_adapter_type" },
	{ 0x63b61982, "param_ops_int" },
	{ 0xc9d7e676, "module_layout" },
};

MODULE_INFO(depends, "i2c-mux,wb_pcie_dev,wb_i2c_dev,wb_indirect_dev,wb_io_dev,wb_spi_dev");

MODULE_ALIAS("i2c:wb_fpga_pca9540");
MODULE_ALIAS("i2c:wb_fpga_pca9541");
MODULE_ALIAS("i2c:wb_fpga_pca9542");
MODULE_ALIAS("i2c:wb_fpga_pca9543");
MODULE_ALIAS("i2c:wb_fpga_pca9544");
MODULE_ALIAS("i2c:wb_fpga_pca9545");
MODULE_ALIAS("i2c:wb_fpga_pca9546");
MODULE_ALIAS("i2c:wb_fpga_pca9547");
MODULE_ALIAS("i2c:wb_fpga_pca9548");
