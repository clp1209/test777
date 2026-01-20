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
	{ 0xe6ba4a90, "__platform_driver_register" },
	{ 0x44c7d5dc, "platform_driver_unregister" },
	{ 0x4950bb37, "pcie_device_func_write" },
	{ 0x53b0078, "filp_open" },
	{ 0x92a45316, "iov_iter_kvec" },
	{ 0xf2c40a08, "vfs_iter_write" },
	{ 0x260a70f8, "vfs_fsync" },
	{ 0x4cda5dcb, "filp_close" },
	{ 0x2a9ad4c3, "i2c_device_func_write" },
	{ 0x618de573, "io_device_func_write" },
	{ 0x92997ed8, "_printk" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xa19b956, "__stack_chk_fail" },
	{ 0xbb526d92, "pcie_device_func_read" },
	{ 0x1af5384e, "vfs_iter_read" },
	{ 0x7751408f, "i2c_device_func_read" },
	{ 0x64b4b7aa, "io_device_func_read" },
	{ 0x4dfa8d4b, "mutex_lock" },
	{ 0x3213f038, "mutex_unlock" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0x947a19c4, "_dev_err" },
	{ 0xbcab6ee6, "sscanf" },
	{ 0xd3ff68a1, "devm_kmalloc" },
	{ 0xe1dd9dcf, "of_property_read_string" },
	{ 0x95e087c, "of_property_read_variable_u8_array" },
	{ 0x8a856723, "of_property_read_variable_u32_array" },
	{ 0xe2d5255a, "strcmp" },
	{ 0xcd49819e, "kthread_create_on_node" },
	{ 0x2270d46a, "wake_up_process" },
	{ 0x48879f20, "hrtimer_init" },
	{ 0xd955afa6, "hrtimer_start_range_ns" },
	{ 0x13204864, "of_get_named_gpio_flags" },
	{ 0x57e55184, "devm_gpio_request_one" },
	{ 0xae72fb94, "gpio_to_desc" },
	{ 0x154b3ad7, "gpiod_direction_output_raw" },
	{ 0x4166c32e, "__dynamic_dev_dbg" },
	{ 0xe092a2f2, "_dev_info" },
	{ 0x7d853e5c, "devm_watchdog_register_device" },
	{ 0xedb869e1, "sysfs_create_group" },
	{ 0xcefb0c9f, "__mutex_init" },
	{ 0x44d597d7, "sysfs_remove_group" },
	{ 0x83596d3a, "gpiod_set_raw_value_cansleep" },
	{ 0x8b063b96, "gpiod_direction_input" },
	{ 0x4a666d56, "hrtimer_cancel" },
	{ 0xe6872c99, "kthread_stop" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0x65487097, "__x86_indirect_thunk_rax" },
	{ 0xf07464e4, "hrtimer_forward" },
	{ 0x7f02188f, "__msecs_to_jiffies" },
	{ 0x151f4898, "schedule_timeout_uninterruptible" },
	{ 0xb3f7646e, "kthread_should_stop" },
	{ 0x63b61982, "param_ops_int" },
	{ 0xc9d7e676, "module_layout" },
};

MODULE_INFO(depends, "wb_pcie_dev,wb_i2c_dev,wb_io_dev,watchdog");

MODULE_ALIAS("of:N*T*Cwb_wdt");
MODULE_ALIAS("of:N*T*Cwb_wdtC*");
