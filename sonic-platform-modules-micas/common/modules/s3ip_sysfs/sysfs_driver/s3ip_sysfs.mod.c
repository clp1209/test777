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

SYMBOL_CRC(s3ip_sysfs_syseeprom_drivers_register, 0x36b6858f, "");
SYMBOL_CRC(s3ip_sysfs_syseeprom_drivers_unregister, 0xd0d3a757, "");
SYMBOL_CRC(s3ip_sysfs_cpld_drivers_register, 0x7a34a09e, "");
SYMBOL_CRC(s3ip_sysfs_cpld_drivers_unregister, 0xa3c7862a, "");
SYMBOL_CRC(s3ip_sysfs_curr_sensor_drivers_register, 0xdd390fa7, "");
SYMBOL_CRC(s3ip_sysfs_curr_sensor_drivers_unregister, 0x36440847, "");
SYMBOL_CRC(s3ip_sysfs_fan_drivers_register, 0xccb07025, "");
SYMBOL_CRC(s3ip_sysfs_fan_drivers_unregister, 0xf6a6c728, "");
SYMBOL_CRC(s3ip_sysfs_fpga_drivers_register, 0xe01c46eb, "");
SYMBOL_CRC(s3ip_sysfs_fpga_drivers_unregister, 0xe70b45c6, "");
SYMBOL_CRC(s3ip_sysfs_psu_drivers_register, 0x491ab68f, "");
SYMBOL_CRC(s3ip_sysfs_psu_drivers_unregister, 0x5d3ae557, "");
SYMBOL_CRC(s3ip_sysfs_slot_drivers_register, 0x356cdb47, "");
SYMBOL_CRC(s3ip_sysfs_slot_drivers_unregister, 0x960fc07e, "");
SYMBOL_CRC(s3ip_sysfs_sysled_drivers_register, 0x50d875eb, "");
SYMBOL_CRC(s3ip_sysfs_sysled_drivers_unregister, 0x4f69147d, "");
SYMBOL_CRC(s3ip_sysfs_temp_sensor_drivers_register, 0x3fb5ef22, "");
SYMBOL_CRC(s3ip_sysfs_temp_sensor_drivers_unregister, 0xc5328b50, "");
SYMBOL_CRC(s3ip_sysfs_sff_drivers_register, 0xb0196964, "");
SYMBOL_CRC(s3ip_sysfs_sff_drivers_unregister, 0x7b6e3277, "");
SYMBOL_CRC(s3ip_sysfs_vol_sensor_drivers_register, 0x3edb549c, "");
SYMBOL_CRC(s3ip_sysfs_vol_sensor_drivers_unregister, 0x18e914ff, "");
SYMBOL_CRC(s3ip_sysfs_watchdog_drivers_register, 0x81c3333a, "");
SYMBOL_CRC(s3ip_sysfs_watchdog_drivers_unregister, 0xb5f52f93, "");
SYMBOL_CRC(s3ip_sysfs_system_drivers_register, 0x81ca80e1, "");
SYMBOL_CRC(s3ip_sysfs_system_drivers_unregister, 0xb532b3c9, "");
SYMBOL_CRC(s3ip_sysfs_eeprom_drivers_register, 0x0e32c33b, "");
SYMBOL_CRC(s3ip_sysfs_eeprom_drivers_unregister, 0x066b6e7b, "");

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x53b0078, "filp_open" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x1c31cb1a, "sysfs_create_bin_file" },
	{ 0x37a0cba, "kfree" },
	{ 0x85cb606c, "kset_unregister" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x65487097, "__x86_indirect_thunk_rax" },
	{ 0x92997ed8, "_printk" },
	{ 0xa19b956, "__stack_chk_fail" },
	{ 0xa916b694, "strnlen" },
	{ 0x8c8569cb, "kstrtoint" },
	{ 0x5a921311, "strncmp" },
	{ 0xfcaa842, "kobject_init_and_add" },
	{ 0xedb869e1, "sysfs_create_group" },
	{ 0xbcab6ee6, "sscanf" },
	{ 0x6a6e05bf, "kstrtou8" },
	{ 0xfb578fc5, "memset" },
	{ 0x96a9e84f, "kernel_read" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x44d597d7, "sysfs_remove_group" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0x696a81bc, "param_ops_bool" },
	{ 0x4cda5dcb, "filp_close" },
	{ 0x5c02b152, "kmalloc_trace" },
	{ 0x63b61982, "param_ops_int" },
	{ 0x349cba85, "strchr" },
	{ 0x601c4e91, "sysfs_remove_bin_file" },
	{ 0xf90a1e85, "__x86_indirect_thunk_r8" },
	{ 0xeb233a45, "__kmalloc" },
	{ 0x81676dd4, "kmalloc_caches" },
	{ 0x240feb40, "kset_create_and_add" },
	{ 0x7c4bb40b, "kobject_put" },
	{ 0xc9d7e676, "module_layout" },
};

MODULE_INFO(depends, "");

