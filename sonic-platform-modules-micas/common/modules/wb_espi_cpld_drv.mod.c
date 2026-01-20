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
	{ 0x3c2406df, "gpiod_set_value_cansleep" },
	{ 0x9e7d6bd0, "__udelay" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0x4dfa8d4b, "mutex_lock" },
	{ 0xf90a1e85, "__x86_indirect_thunk_r8" },
	{ 0x3213f038, "mutex_unlock" },
	{ 0xa19b956, "__stack_chk_fail" },
	{ 0x92997ed8, "_printk" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0x44c7d5dc, "platform_driver_unregister" },
	{ 0x6e9dd606, "__symbol_put" },
	{ 0xd3ff68a1, "devm_kmalloc" },
	{ 0xcefb0c9f, "__mutex_init" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xae72fb94, "gpio_to_desc" },
	{ 0x154b3ad7, "gpiod_direction_output_raw" },
	{ 0x868784cb, "__symbol_get" },
	{ 0xfe990052, "gpio_free" },
	{ 0xe092a2f2, "_dev_info" },
	{ 0x65487097, "__x86_indirect_thunk_rax" },
	{ 0xdbdf6c92, "ioport_resource" },
	{ 0x85bd1608, "__request_region" },
	{ 0x81676dd4, "kmalloc_caches" },
	{ 0x5c02b152, "kmalloc_trace" },
	{ 0x70e15cb4, "misc_register" },
	{ 0x37a0cba, "kfree" },
	{ 0x1035c7c2, "__release_region" },
	{ 0xec516f29, "misc_deregister" },
	{ 0x63b61982, "param_ops_int" },
	{ 0x1d18679c, "param_ops_uint" },
	{ 0xc9d7e676, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("acpi*:WBCPLD:*");
