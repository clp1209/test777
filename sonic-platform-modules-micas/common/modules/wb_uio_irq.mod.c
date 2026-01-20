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
	{ 0x42336ad0, "uio_unregister_device" },
	{ 0x37a0cba, "kfree" },
	{ 0x44d597d7, "sysfs_remove_group" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x34db050b, "_raw_spin_lock_irqsave" },
	{ 0xbcab6ee6, "sscanf" },
	{ 0x3ce4ca6f, "disable_irq" },
	{ 0xd35cce70, "_raw_spin_unlock_irqrestore" },
	{ 0x2cf56265, "__dynamic_pr_debug" },
	{ 0xfcec0987, "enable_irq" },
	{ 0xa19b956, "__stack_chk_fail" },
	{ 0x92997ed8, "_printk" },
	{ 0x1cdb6f80, "irq_get_irq_data" },
	{ 0x44c7d5dc, "platform_driver_unregister" },
	{ 0x27bbf221, "disable_irq_nosync" },
	{ 0x81676dd4, "kmalloc_caches" },
	{ 0x5c02b152, "kmalloc_trace" },
	{ 0x8a856723, "of_property_read_variable_u32_array" },
	{ 0xdf69cce1, "__uio_register_device" },
	{ 0xedb869e1, "sysfs_create_group" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xae72fb94, "gpio_to_desc" },
	{ 0x5f12eb28, "gpiod_to_irq" },
	{ 0xbc477a2, "irq_set_irq_type" },
	{ 0x947a19c4, "_dev_err" },
	{ 0x63b61982, "param_ops_int" },
	{ 0xc9d7e676, "module_layout" },
};

MODULE_INFO(depends, "uio");

MODULE_ALIAS("of:N*T*Cuio-irq");
MODULE_ALIAS("of:N*T*Cuio-irqC*");
