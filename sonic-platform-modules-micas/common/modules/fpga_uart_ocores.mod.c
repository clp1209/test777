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
	{ 0xa95f9d6f, "uart_register_driver" },
	{ 0xe6ba4a90, "__platform_driver_register" },
	{ 0x8c303e36, "uart_unregister_driver" },
	{ 0x6450712, "uart_remove_one_port" },
	{ 0x65487097, "__x86_indirect_thunk_rax" },
	{ 0x92997ed8, "_printk" },
	{ 0x20000329, "simple_strtoul" },
	{ 0x34db050b, "_raw_spin_lock_irqsave" },
	{ 0xd35cce70, "_raw_spin_unlock_irqrestore" },
	{ 0x86bb52ed, "serial_buffer_realloc" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x4dfa8d4b, "mutex_lock" },
	{ 0x3213f038, "mutex_unlock" },
	{ 0xd3ff68a1, "devm_kmalloc" },
	{ 0x62cfed7b, "serial_buffer_if_allocced" },
	{ 0x5bc6af78, "serial_get_buffer_size" },
	{ 0xe26fc3c5, "platform_get_resource" },
	{ 0xde80cd09, "ioremap" },
	{ 0xd48b5e99, "platform_get_irq" },
	{ 0x38d37ea8, "devm_request_threaded_irq" },
	{ 0xbb4947a9, "uart_add_one_port" },
	{ 0x156314a1, "serial_buffer_alloc" },
	{ 0xedc03953, "iounmap" },
	{ 0x44c7d5dc, "platform_driver_unregister" },
	{ 0x8427cc7b, "_raw_spin_lock_irq" },
	{ 0x4b750f53, "_raw_spin_unlock_irq" },
	{ 0x725da1c0, "uart_get_baud_rate" },
	{ 0x4551c02e, "__pm_runtime_resume" },
	{ 0x19e336b8, "uart_update_timeout" },
	{ 0x66cca4f9, "__x86_indirect_thunk_rcx" },
	{ 0x6ebe366f, "ktime_get_mono_fast_ns" },
	{ 0xdbf60a92, "__pm_runtime_suspend" },
	{ 0xbd394d8, "tty_termios_baud_rate" },
	{ 0x5641485b, "tty_termios_encode_baud_rate" },
	{ 0x20e0a5c5, "uart_write_wakeup" },
	{ 0x52b7afdc, "serial_get_buffer_status" },
	{ 0x230247ef, "serial_buffer_read" },
	{ 0x906c6132, "__tty_insert_flip_char" },
	{ 0x3e83712b, "tty_flip_buffer_push" },
	{ 0x4166c32e, "__dynamic_dev_dbg" },
	{ 0x1d24c881, "___ratelimit" },
	{ 0xa19b956, "__stack_chk_fail" },
	{ 0xe523ad75, "synchronize_irq" },
	{ 0xe2964344, "__wake_up" },
	{ 0x4f94bbc0, "uart_handle_dcd_change" },
	{ 0xad6581a5, "uart_handle_cts_change" },
	{ 0xf53d1c85, "uart_insert_char" },
	{ 0x15ba50a6, "jiffies" },
	{ 0x98c88763, "serial_buffer_write" },
	{ 0x4a17ed66, "sysrq_mask" },
	{ 0xfe6ab90c, "uart_try_toggle_sysrq" },
	{ 0x6383b27c, "__x86_indirect_thunk_rdx" },
	{ 0x7647726c, "handle_sysrq" },
	{ 0x2cf56265, "__dynamic_pr_debug" },
	{ 0xa421bf9f, "do_SAK" },
	{ 0x63b61982, "param_ops_int" },
	{ 0xc9d7e676, "module_layout" },
};

MODULE_INFO(depends, "serial_buffer");

MODULE_ALIAS("platform:rg-uart");
MODULE_ALIAS("platform:fpga-uart");
