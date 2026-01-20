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

SYMBOL_CRC(s3ip_switch_driver_get, 0x74efcde4, "");

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x53b0078, "filp_open" },
	{ 0x3eed7c94, "i2c_get_adapter" },
	{ 0x656e4a6e, "snprintf" },
	{ 0xb742fd7, "simple_strtol" },
	{ 0xd7a99fc1, "iterate_dir" },
	{ 0x69acdf38, "memcpy" },
	{ 0x37a0cba, "kfree" },
	{ 0xc3055d20, "usleep_range_state" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x65487097, "__x86_indirect_thunk_rax" },
	{ 0x92997ed8, "_printk" },
	{ 0x1af5384e, "vfs_iter_read" },
	{ 0xa19b956, "__stack_chk_fail" },
	{ 0xa916b694, "strnlen" },
	{ 0xa4ebb6a9, "i2c_smbus_xfer" },
	{ 0x68f31cbd, "__list_add_valid" },
	{ 0x8c8569cb, "kstrtoint" },
	{ 0x55385e2e, "__x86_indirect_thunk_r14" },
	{ 0x1e6d26a8, "strstr" },
	{ 0x5a921311, "strncmp" },
	{ 0x1a79c8e9, "__x86_indirect_thunk_r13" },
	{ 0x92a45316, "iov_iter_kvec" },
	{ 0x449ad0a7, "memcmp" },
	{ 0xe1537255, "__list_del_entry_valid" },
	{ 0xfb578fc5, "memset" },
	{ 0xf2c40a08, "vfs_iter_write" },
	{ 0x96a9e84f, "kernel_read" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xe2d5255a, "strcmp" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0xe4a7709c, "i2c_put_adapter" },
	{ 0x260a70f8, "vfs_fsync" },
	{ 0x4cda5dcb, "filp_close" },
	{ 0x20000329, "simple_strtoul" },
	{ 0x5c02b152, "kmalloc_trace" },
	{ 0x632e6618, "i2c_smbus_read_i2c_block_data" },
	{ 0x754d539c, "strlen" },
	{ 0x63b61982, "param_ops_int" },
	{ 0x349cba85, "strchr" },
	{ 0xf9a482f9, "msleep" },
	{ 0xeb233a45, "__kmalloc" },
	{ 0x81676dd4, "kmalloc_caches" },
	{ 0xc9d7e676, "module_layout" },
};

MODULE_INFO(depends, "");

