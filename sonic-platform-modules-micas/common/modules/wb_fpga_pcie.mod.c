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
	{ 0x92997ed8, "_printk" },
	{ 0x14425bba, "__pci_register_driver" },
	{ 0x408f7367, "pci_disable_msi" },
	{ 0xf7c65084, "pci_disable_device" },
	{ 0xcda74bbb, "pci_unregister_driver" },
	{ 0xd3ff68a1, "devm_kmalloc" },
	{ 0xc4c525e3, "pci_read_config_dword" },
	{ 0x121e5023, "pci_enable_device" },
	{ 0xd9d82b25, "pci_set_master" },
	{ 0x72037300, "pci_alloc_irq_vectors_affinity" },
	{ 0xfcc60d21, "pci_write_config_dword" },
	{ 0x947a19c4, "_dev_err" },
	{ 0xe092a2f2, "_dev_info" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xa19b956, "__stack_chk_fail" },
	{ 0x63b61982, "param_ops_int" },
	{ 0xc9d7e676, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("pci:v000010EEd00007022sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00007011sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001DEDd00007022sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001DEDd00007021sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001DEDd00005220sv*sd*bc*sc*i*");
