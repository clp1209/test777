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
	{ 0x46a7119b, "pmbus_check_byte_register" },
	{ 0xa71109b1, "pmbus_check_word_register" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x436e19f8, "pmbus_read_byte_data" },
	{ 0x99acf981, "pmbus_clear_faults" },
	{ 0x47b58a77, "pmbus_set_page" },
	{ 0xfb578fc5, "memset" },
	{ 0xd3ff68a1, "devm_kmalloc" },
	{ 0xb1439093, "i2c_match_id" },
	{ 0xaa27cbb8, "pmbus_do_probe" },
	{ 0xbe239ab1, "i2c_del_driver" },
	{ 0xc9d7e676, "module_layout" },
};

MODULE_INFO(depends, "pmbus_core");

MODULE_ALIAS("i2c:wb_csu550");
MODULE_ALIAS("i2c:wb_csu800");
MODULE_ALIAS("i2c:wb_fsp1200");
MODULE_ALIAS("i2c:wb_dps550");
