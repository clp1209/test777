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
	{ 0x81882db0, "hwmon_device_unregister" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x92997ed8, "_printk" },
	{ 0x3be18b76, "i2c_transfer" },
	{ 0xa19b956, "__stack_chk_fail" },
	{ 0xc3055d20, "usleep_range_state" },
	{ 0x4dfa8d4b, "mutex_lock" },
	{ 0x3213f038, "mutex_unlock" },
	{ 0x656e4a6e, "snprintf" },
	{ 0xd3ff68a1, "devm_kmalloc" },
	{ 0xcefb0c9f, "__mutex_init" },
	{ 0x2dc59b6b, "hwmon_device_register_with_groups" },
	{ 0x947a19c4, "_dev_err" },
	{ 0xe092a2f2, "_dev_info" },
	{ 0xbe239ab1, "i2c_del_driver" },
	{ 0x63b61982, "param_ops_int" },
	{ 0xc9d7e676, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("i2c:wb_mac_bsc_td3");
MODULE_ALIAS("i2c:wb_mac_bsc_td3_x2");
MODULE_ALIAS("i2c:wb_mac_bsc_td4");
MODULE_ALIAS("i2c:wb_mac_bsc_th3");
MODULE_ALIAS("i2c:wb_mac_bsc_th4");
