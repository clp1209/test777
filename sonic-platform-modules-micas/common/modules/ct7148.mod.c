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
	{ 0xd3ff68a1, "devm_kmalloc" },
	{ 0xcefb0c9f, "__mutex_init" },
	{ 0xb1439093, "i2c_match_id" },
	{ 0x5e239d38, "devm_hwmon_device_register_with_info" },
	{ 0xbe239ab1, "i2c_del_driver" },
	{ 0x4dfa8d4b, "mutex_lock" },
	{ 0x15ba50a6, "jiffies" },
	{ 0x3213f038, "mutex_unlock" },
	{ 0xcc0f059, "i2c_smbus_read_byte_data" },
	{ 0x92997ed8, "_printk" },
	{ 0x63b61982, "param_ops_int" },
	{ 0xc9d7e676, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Csensylink,ct7318");
MODULE_ALIAS("of:N*T*Csensylink,ct7318C*");
MODULE_ALIAS("i2c:ct7318");
