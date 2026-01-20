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
	{ 0x9ad306d6, "__spi_register_driver" },
	{ 0x3c2406df, "gpiod_set_value_cansleep" },
	{ 0xedf90ac, "of_match_device" },
	{ 0xd3ff68a1, "devm_kmalloc" },
	{ 0x8a856723, "of_property_read_variable_u32_array" },
	{ 0x3b9189d7, "of_find_property" },
	{ 0x3577ab87, "devm_gpiod_get_optional" },
	{ 0xa98ca3fd, "gpiod_direction_output" },
	{ 0xcefb0c9f, "__mutex_init" },
	{ 0xf75a06ce, "devm_nvmem_register" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x9ff55ce3, "device_create_file" },
	{ 0xa19b956, "__stack_chk_fail" },
	{ 0x947a19c4, "_dev_err" },
	{ 0xe092a2f2, "_dev_info" },
	{ 0x7670a2f1, "driver_unregister" },
	{ 0xacdaa001, "device_remove_file" },
	{ 0x68f31cbd, "__list_add_valid" },
	{ 0x4dfa8d4b, "mutex_lock" },
	{ 0x65487097, "__x86_indirect_thunk_rax" },
	{ 0x2bd9e5a8, "spi_sync" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0x3213f038, "mutex_unlock" },
	{ 0x4166c32e, "__dynamic_dev_dbg" },
	{ 0xbcab6ee6, "sscanf" },
	{ 0x6383b27c, "__x86_indirect_thunk_rdx" },
	{ 0x63b61982, "param_ops_int" },
	{ 0xc9d7e676, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("spi:wb_93xx46");
MODULE_ALIAS("spi:eeprom-93xx46");
MODULE_ALIAS("spi:at93c46");
MODULE_ALIAS("spi:at93c46d");
MODULE_ALIAS("spi:at93c56");
MODULE_ALIAS("spi:at93c66");
MODULE_ALIAS("spi:93lc46b");
MODULE_ALIAS("of:N*T*Ceeprom-93xx46");
MODULE_ALIAS("of:N*T*Ceeprom-93xx46C*");
MODULE_ALIAS("of:N*T*Catmel,at93c46");
MODULE_ALIAS("of:N*T*Catmel,at93c46C*");
MODULE_ALIAS("of:N*T*Catmel,at93c46d");
MODULE_ALIAS("of:N*T*Catmel,at93c46dC*");
MODULE_ALIAS("of:N*T*Catmel,at93c56");
MODULE_ALIAS("of:N*T*Catmel,at93c56C*");
MODULE_ALIAS("of:N*T*Catmel,at93c66");
MODULE_ALIAS("of:N*T*Catmel,at93c66C*");
MODULE_ALIAS("of:N*T*Cmicrochip,93lc46b");
MODULE_ALIAS("of:N*T*Cmicrochip,93lc46bC*");
