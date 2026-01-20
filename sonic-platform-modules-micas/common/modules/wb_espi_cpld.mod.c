#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
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
	{ 0x2c635209, "module_layout" },
	{ 0x7a08b1ac, "param_ops_uint" },
	{ 0xae2ae519, "param_ops_int" },
	{ 0x9d6d692d, "platform_driver_unregister" },
	{ 0xf78aa7ac, "__platform_driver_register" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0xd0da656b, "__stack_chk_fail" },
	{ 0x3213f038, "mutex_unlock" },
	{ 0x4dfa8d4b, "mutex_lock" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x92997ed8, "_printk" },
	{ 0xec2dcbc3, "_dev_info" },
	{ 0xc2aad509, "misc_register" },
	{ 0xaf88e69b, "kmem_cache_alloc_trace" },
	{ 0x30a93ed, "kmalloc_caches" },
	{ 0x85bd1608, "__request_region" },
	{ 0x9a08203d, "gpiod_direction_output_raw" },
	{ 0xd7be8952, "gpio_to_desc" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xcefb0c9f, "__mutex_init" },
	{ 0xc3fcadfd, "devm_kmalloc" },
	{ 0xfe990052, "gpio_free" },
	{ 0x1035c7c2, "__release_region" },
	{ 0xdbdf6c92, "ioport_resource" },
	{ 0x37a0cba, "kfree" },
	{ 0x4bc81f50, "misc_deregister" },
	{ 0x9e7d6bd0, "__udelay" },
	{ 0x25fb07f8, "gpiod_set_value_cansleep" },
	{ 0x87a21cb3, "__ubsan_handle_out_of_bounds" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("acpi*:WBCPLD:*");

MODULE_INFO(srcversion, "5FF5E3DCAC1C30A7455B22B");
