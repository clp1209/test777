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
	{ 0x3c2406df, "gpiod_set_value_cansleep" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x8b063b96, "gpiod_direction_input" },
	{ 0xa98ca3fd, "gpiod_direction_output" },
	{ 0xbfa8ae13, "spi_bitbang_cleanup" },
	{ 0x5d675381, "spi_bitbang_setup" },
	{ 0xee50c2a9, "__devm_spi_alloc_controller" },
	{ 0x3577ab87, "devm_gpiod_get_optional" },
	{ 0xcafc59d0, "devm_gpiod_get" },
	{ 0x57f1c3fb, "spi_bitbang_setup_transfer" },
	{ 0xb12bd430, "spi_bitbang_init" },
	{ 0x17339ed, "devm_spi_register_controller" },
	{ 0xd3ff68a1, "devm_kmalloc" },
	{ 0x35883128, "devm_gpiod_get_index" },
	{ 0x44c7d5dc, "platform_driver_unregister" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0xa0b69f35, "gpiod_get_value_cansleep" },
	{ 0xc9d7e676, "module_layout" },
};

MODULE_INFO(depends, "spi-bitbang");

MODULE_ALIAS("of:N*T*Cwb-spi-gpio");
MODULE_ALIAS("of:N*T*Cwb-spi-gpioC*");
