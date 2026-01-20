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

SYMBOL_CRC(serial_buffer_if_allocced, 0x62cfed7b, "");
SYMBOL_CRC(serial_buffer_alloc, 0x156314a1, "");
SYMBOL_CRC(serial_buffer_release, 0xb1f0bc60, "");
SYMBOL_CRC(serial_buffer_realloc, 0x86bb52ed, "");
SYMBOL_CRC(serial_buffer_write, 0x98c88763, "");
SYMBOL_CRC(serial_buffer_read, 0x230247ef, "");
SYMBOL_CRC(serial_get_buffer_status, 0x52b7afdc, "");
SYMBOL_CRC(serial_get_buffer_size, 0x5bc6af78, "");

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x37a0cba, "kfree" },
	{ 0x4dfa8d4b, "mutex_lock" },
	{ 0x3213f038, "mutex_unlock" },
	{ 0x75ce3177, "debugfs_create_dir" },
	{ 0xcefb0c9f, "__mutex_init" },
	{ 0x656e4a6e, "snprintf" },
	{ 0xa66a08cf, "debugfs_create_file" },
	{ 0xa19b956, "__stack_chk_fail" },
	{ 0x908e8794, "single_open" },
	{ 0xf41f91f0, "debugfs_remove" },
	{ 0xeb233a45, "__kmalloc" },
	{ 0x69acdf38, "memcpy" },
	{ 0x66f10083, "seq_printf" },
	{ 0xda06b58b, "seq_lseek" },
	{ 0x10f28306, "seq_read" },
	{ 0x7f64050, "single_release" },
	{ 0xc9d7e676, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "BA570510092821E27433B45");
