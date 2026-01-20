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

SYMBOL_CRC(pca9641_setmuxflag, 0x9590fa6e, "");

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xa534e67, "i2c_register_driver" },
	{ 0x4cfef8c, "i2c_mux_del_adapters" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x53b0078, "filp_open" },
	{ 0xae3ca9da, "i2c_smbus_write_byte_data" },
	{ 0xf9a482f9, "msleep" },
	{ 0x4cda5dcb, "filp_close" },
	{ 0xa19b956, "__stack_chk_fail" },
	{ 0x92997ed8, "_printk" },
	{ 0xbe239ab1, "i2c_del_driver" },
	{ 0xcc0f059, "i2c_smbus_read_byte_data" },
	{ 0x92a45316, "iov_iter_kvec" },
	{ 0xf2c40a08, "vfs_iter_write" },
	{ 0x260a70f8, "vfs_fsync" },
	{ 0x1af5384e, "vfs_iter_read" },
	{ 0x12191062, "__i2c_transfer" },
	{ 0x65487097, "__x86_indirect_thunk_rax" },
	{ 0xd73637bb, "i2c_mux_alloc" },
	{ 0x8a856723, "of_property_read_variable_u32_array" },
	{ 0xe1dd9dcf, "of_property_read_string" },
	{ 0xfd5dfbcb, "i2c_mux_add_adapter" },
	{ 0x947a19c4, "_dev_err" },
	{ 0xe092a2f2, "_dev_info" },
	{ 0x15ba50a6, "jiffies" },
	{ 0x890d25b7, "_dev_warn" },
	{ 0xc3055d20, "usleep_range_state" },
	{ 0x1000e51, "schedule" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xae72fb94, "gpio_to_desc" },
	{ 0x154b3ad7, "gpiod_direction_output_raw" },
	{ 0xa79fc55b, "gpiod_set_raw_value" },
	{ 0x1b183398, "gpiod_get_raw_value" },
	{ 0xfe990052, "gpio_free" },
	{ 0x63b61982, "param_ops_int" },
	{ 0xc9d7e676, "module_layout" },
};

MODULE_INFO(depends, "i2c-mux");

MODULE_ALIAS("of:N*T*Cnxp,wb_pca9541");
MODULE_ALIAS("of:N*T*Cnxp,wb_pca9541C*");
MODULE_ALIAS("of:N*T*Cnxp,wb_pca9641");
MODULE_ALIAS("of:N*T*Cnxp,wb_pca9641C*");
MODULE_ALIAS("i2c:wb_pca9541");
MODULE_ALIAS("i2c:wb_pca9641");
