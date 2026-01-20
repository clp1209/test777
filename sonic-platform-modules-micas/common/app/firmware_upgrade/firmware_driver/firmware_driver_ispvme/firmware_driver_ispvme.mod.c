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
	{ 0x53b0078, "filp_open" },
	{ 0xec516f29, "misc_deregister" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0xd3ff68a1, "devm_kmalloc" },
	{ 0x8a856723, "of_property_read_variable_u32_array" },
	{ 0x44c7d5dc, "platform_driver_unregister" },
	{ 0x1b183398, "gpiod_get_raw_value" },
	{ 0x656e4a6e, "snprintf" },
	{ 0xa79fc55b, "gpiod_set_raw_value" },
	{ 0xfe990052, "gpio_free" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x92997ed8, "_printk" },
	{ 0x1af5384e, "vfs_iter_read" },
	{ 0x1000e51, "schedule" },
	{ 0xa19b956, "__stack_chk_fail" },
	{ 0xa916b694, "strnlen" },
	{ 0x68f31cbd, "__list_add_valid" },
	{ 0x154b3ad7, "gpiod_direction_output_raw" },
	{ 0x8b063b96, "gpiod_direction_input" },
	{ 0x9166fada, "strncpy" },
	{ 0x92a45316, "iov_iter_kvec" },
	{ 0xe1dd9dcf, "of_property_read_string" },
	{ 0xe1537255, "__list_del_entry_valid" },
	{ 0x70e15cb4, "misc_register" },
	{ 0xf2c40a08, "vfs_iter_write" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0xe2d5255a, "strcmp" },
	{ 0xe6ba4a90, "__platform_driver_register" },
	{ 0x260a70f8, "vfs_fsync" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0x4cda5dcb, "filp_close" },
	{ 0xae72fb94, "gpio_to_desc" },
	{ 0x47229b5c, "gpio_request" },
	{ 0x63b61982, "param_ops_int" },
	{ 0xc9d7e676, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "B4D975D2C4A35F5A304A735");
