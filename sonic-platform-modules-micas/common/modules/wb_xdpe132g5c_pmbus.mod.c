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
	{ 0x44d597d7, "sysfs_remove_group" },
	{ 0x5c3c7387, "kstrtoull" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xdad7b47, "pmbus_write_word_data" },
	{ 0x99acf981, "pmbus_clear_faults" },
	{ 0xa19b956, "__stack_chk_fail" },
	{ 0x92997ed8, "_printk" },
	{ 0x41e2c71f, "pmbus_read_word_data" },
	{ 0x656e4a6e, "snprintf" },
	{ 0xe128cfc9, "devm_kmemdup" },
	{ 0xaa27cbb8, "pmbus_do_probe" },
	{ 0xedb869e1, "sysfs_create_group" },
	{ 0x436e19f8, "pmbus_read_byte_data" },
	{ 0x3854774b, "kstrtoll" },
	{ 0xf9a482f9, "msleep" },
	{ 0xbe239ab1, "i2c_del_driver" },
	{ 0x63b61982, "param_ops_int" },
	{ 0xc9d7e676, "module_layout" },
};

MODULE_INFO(depends, "pmbus_core");

MODULE_ALIAS("of:N*T*Cinfineon,wb_xdpe132g5c_pmbus");
MODULE_ALIAS("of:N*T*Cinfineon,wb_xdpe132g5c_pmbusC*");
MODULE_ALIAS("of:N*T*Cinfineon,wb_xdpe1a2g5b_pmbus");
MODULE_ALIAS("of:N*T*Cinfineon,wb_xdpe1a2g5b_pmbusC*");
MODULE_ALIAS("i2c:wb_xdpe132g5c_pmbus");
MODULE_ALIAS("i2c:wb_xdpe1a2g5b_pmbus");
