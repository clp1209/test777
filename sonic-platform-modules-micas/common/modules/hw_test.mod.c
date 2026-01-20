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
	{ 0xedc03953, "iounmap" },
	{ 0xde80cd09, "ioremap" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0xa19b956, "__stack_chk_fail" },
	{ 0x92997ed8, "_printk" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0xe1537255, "__list_del_entry_valid" },
	{ 0x37a0cba, "kfree" },
	{ 0x8a2117cf, "mdio_bus_type" },
	{ 0x93c3e5f9, "bus_find_device" },
	{ 0x81676dd4, "kmalloc_caches" },
	{ 0x5c02b152, "kmalloc_trace" },
	{ 0x68f31cbd, "__list_add_valid" },
	{ 0xb8ff5c2c, "mdiobus_alloc_size" },
	{ 0xe914e41e, "strcpy" },
	{ 0x17edbbd5, "__mdiobus_register" },
	{ 0x692e4634, "mdiobus_unregister" },
	{ 0x63b74f59, "class_find_device" },
	{ 0x70e15cb4, "misc_register" },
	{ 0xec516f29, "misc_deregister" },
	{ 0x656e4a6e, "snprintf" },
	{ 0xc70cfdc1, "mdiobus_read" },
	{ 0xf8a7fa1b, "mdiobus_write" },
	{ 0xc9d7e676, "module_layout" },
};

MODULE_INFO(depends, "libphy");

