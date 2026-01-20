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

SYMBOL_CRC(pcie_device_func_read, 0xbb526d92, "");
SYMBOL_CRC(pcie_device_func_write, 0x4950bb37, "");

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x65487097, "__x86_indirect_thunk_rax" },
	{ 0x92997ed8, "_printk" },
	{ 0x656e4a6e, "snprintf" },
	{ 0xe2d5255a, "strcmp" },
	{ 0xa19b956, "__stack_chk_fail" },
	{ 0xe6ba4a90, "__platform_driver_register" },
	{ 0xedc03953, "iounmap" },
	{ 0xec516f29, "misc_deregister" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0x44c7d5dc, "platform_driver_unregister" },
	{ 0xd3ff68a1, "devm_kmalloc" },
	{ 0xe1dd9dcf, "of_property_read_string" },
	{ 0x8a856723, "of_property_read_variable_u32_array" },
	{ 0xd158619a, "pci_get_domain_bus_and_slot" },
	{ 0x70e15cb4, "misc_register" },
	{ 0x8395b40b, "pci_read_config_byte" },
	{ 0xde80cd09, "ioremap" },
	{ 0x947a19c4, "_dev_err" },
	{ 0xe092a2f2, "_dev_info" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0x69acdf38, "memcpy" },
	{ 0x7682ba4e, "__copy_overflow" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0x63b61982, "param_ops_int" },
	{ 0xc9d7e676, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Cwb-pci-dev");
MODULE_ALIAS("of:N*T*Cwb-pci-devC*");
