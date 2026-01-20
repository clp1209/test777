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
	{ 0x65487097, "__x86_indirect_thunk_rax" },
	{ 0x92997ed8, "_printk" },
	{ 0x6383b27c, "__x86_indirect_thunk_rdx" },
	{ 0xe6ba4a90, "__platform_driver_register" },
	{ 0xcefb0c9f, "__mutex_init" },
	{ 0x44d597d7, "sysfs_remove_group" },
	{ 0xdd7a298d, "i2c_del_adapter" },
	{ 0x8c8569cb, "kstrtoint" },
	{ 0x4dfa8d4b, "mutex_lock" },
	{ 0x3213f038, "mutex_unlock" },
	{ 0xa19b956, "__stack_chk_fail" },
	{ 0xa78af5f3, "ioread32" },
	{ 0x4a453f53, "iowrite32" },
	{ 0x842c8e9d, "ioread16" },
	{ 0x6a86bc1, "iowrite16" },
	{ 0xb19b445, "ioread8" },
	{ 0x848d372e, "iowrite8" },
	{ 0x9bb4e317, "ioread32be" },
	{ 0xe419bc99, "iowrite32be" },
	{ 0x44c7d5dc, "platform_driver_unregister" },
	{ 0x34db050b, "_raw_spin_lock_irqsave" },
	{ 0xe2964344, "__wake_up" },
	{ 0xd35cce70, "_raw_spin_unlock_irqrestore" },
	{ 0x66cca4f9, "__x86_indirect_thunk_rcx" },
	{ 0xd48b5e99, "platform_get_irq" },
	{ 0xd3ff68a1, "devm_kmalloc" },
	{ 0xe26fc3c5, "platform_get_resource" },
	{ 0x99dda064, "devm_ioremap_resource" },
	{ 0xd9a5ea54, "__init_waitqueue_head" },
	{ 0x38d37ea8, "devm_request_threaded_irq" },
	{ 0xc643eace, "i2c_add_numbered_adapter" },
	{ 0x90cea828, "i2c_new_client_device" },
	{ 0xedb869e1, "sysfs_create_group" },
	{ 0x8a856723, "of_property_read_variable_u32_array" },
	{ 0x8ee70e4a, "of_match_node" },
	{ 0x4166c32e, "__dynamic_dev_dbg" },
	{ 0x947a19c4, "_dev_err" },
	{ 0xe092a2f2, "_dev_info" },
	{ 0x890d25b7, "_dev_warn" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x15ba50a6, "jiffies" },
	{ 0xc3055d20, "usleep_range_state" },
	{ 0xe2c17b5d, "__SCT__might_resched" },
	{ 0xfe487975, "init_wait_entry" },
	{ 0x8c26d495, "prepare_to_wait_event" },
	{ 0x92540fbf, "finish_wait" },
	{ 0x8ddd8aad, "schedule_timeout" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0xe4a7709c, "i2c_put_adapter" },
	{ 0x3eed7c94, "i2c_get_adapter" },
	{ 0x63b61982, "param_ops_int" },
	{ 0xc9d7e676, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Copencores,rg-i2c-ocores");
MODULE_ALIAS("of:N*T*Copencores,rg-i2c-ocoresC*");
MODULE_ALIAS("of:N*T*Caeroflexgaisler,i2cmst");
MODULE_ALIAS("of:N*T*Caeroflexgaisler,i2cmstC*");
