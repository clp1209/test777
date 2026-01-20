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
	{ 0xa534e67, "i2c_register_driver" },
	{ 0x2da36aa9, "i2c_smbus_write_byte" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x53b0078, "filp_open" },
	{ 0xae3ca9da, "i2c_smbus_write_byte_data" },
	{ 0xf9a482f9, "msleep" },
	{ 0x4cda5dcb, "filp_close" },
	{ 0xa19b956, "__stack_chk_fail" },
	{ 0x92997ed8, "_printk" },
	{ 0x92a45316, "iov_iter_kvec" },
	{ 0xf2c40a08, "vfs_iter_write" },
	{ 0x260a70f8, "vfs_fsync" },
	{ 0x1af5384e, "vfs_iter_read" },
	{ 0x34db050b, "_raw_spin_lock_irqsave" },
	{ 0xd35cce70, "_raw_spin_unlock_irqrestore" },
	{ 0x3ce4ca6f, "disable_irq" },
	{ 0xfcec0987, "enable_irq" },
	{ 0xbe239ab1, "i2c_del_driver" },
	{ 0x12191062, "__i2c_transfer" },
	{ 0x65487097, "__x86_indirect_thunk_rax" },
	{ 0xcc0f059, "i2c_smbus_read_byte_data" },
	{ 0xd73637bb, "i2c_mux_alloc" },
	{ 0x3577ab87, "devm_gpiod_get_optional" },
	{ 0x3b9189d7, "of_find_property" },
	{ 0xedf90ac, "of_match_device" },
	{ 0x1ce7cddf, "of_device_get_match_data" },
	{ 0x8a856723, "of_property_read_variable_u32_array" },
	{ 0xe1dd9dcf, "of_property_read_string" },
	{ 0xb2d48f15, "irq_domain_simple_ops" },
	{ 0xa827f7df, "__irq_domain_add" },
	{ 0xf9176399, "irq_create_mapping_affinity" },
	{ 0x20a789ac, "irq_set_chip_data" },
	{ 0x6297f5e7, "handle_simple_irq" },
	{ 0x9150d474, "irq_set_chip_and_handler_name" },
	{ 0x38d37ea8, "devm_request_threaded_irq" },
	{ 0x25d336a5, "__irq_resolve_mapping" },
	{ 0x2c7db649, "irq_dispose_mapping" },
	{ 0x4ce82c80, "irq_domain_remove" },
	{ 0x4cfef8c, "i2c_mux_del_adapters" },
	{ 0xfd5dfbcb, "i2c_mux_add_adapter" },
	{ 0x947a19c4, "_dev_err" },
	{ 0xe092a2f2, "_dev_info" },
	{ 0x890d25b7, "_dev_warn" },
	{ 0xc3055d20, "usleep_range_state" },
	{ 0x1000e51, "schedule" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xae72fb94, "gpio_to_desc" },
	{ 0x154b3ad7, "gpiod_direction_output_raw" },
	{ 0xa79fc55b, "gpiod_set_raw_value" },
	{ 0x1b183398, "gpiod_get_raw_value" },
	{ 0xfe990052, "gpio_free" },
	{ 0x9590fa6e, "pca9641_setmuxflag" },
	{ 0xde6963ed, "i2c_smbus_read_byte" },
	{ 0x549525ef, "handle_nested_irq" },
	{ 0x63b61982, "param_ops_int" },
	{ 0xc9d7e676, "module_layout" },
};

MODULE_INFO(depends, "i2c-mux,wb_i2c_mux_pca9641");

MODULE_ALIAS("of:N*T*Cnxp,wb_pca9540");
MODULE_ALIAS("of:N*T*Cnxp,wb_pca9540C*");
MODULE_ALIAS("of:N*T*Cnxp,wb_pca9542");
MODULE_ALIAS("of:N*T*Cnxp,wb_pca9542C*");
MODULE_ALIAS("of:N*T*Cnxp,wb_pca9543");
MODULE_ALIAS("of:N*T*Cnxp,wb_pca9543C*");
MODULE_ALIAS("of:N*T*Cnxp,wb_pca9544");
MODULE_ALIAS("of:N*T*Cnxp,wb_pca9544C*");
MODULE_ALIAS("of:N*T*Cnxp,wb_pca9545");
MODULE_ALIAS("of:N*T*Cnxp,wb_pca9545C*");
MODULE_ALIAS("of:N*T*Cnxp,wb_pca9546");
MODULE_ALIAS("of:N*T*Cnxp,wb_pca9546C*");
MODULE_ALIAS("of:N*T*Cnxp,wb_pca9547");
MODULE_ALIAS("of:N*T*Cnxp,wb_pca9547C*");
MODULE_ALIAS("of:N*T*Cnxp,wb_pca9548");
MODULE_ALIAS("of:N*T*Cnxp,wb_pca9548C*");
MODULE_ALIAS("i2c:wb_pca9540");
MODULE_ALIAS("i2c:wb_pca9542");
MODULE_ALIAS("i2c:wb_pca9543");
MODULE_ALIAS("i2c:wb_pca9544");
MODULE_ALIAS("i2c:wb_pca9545");
MODULE_ALIAS("i2c:wb_pca9546");
MODULE_ALIAS("i2c:wb_pca9547");
MODULE_ALIAS("i2c:wb_pca9548");
