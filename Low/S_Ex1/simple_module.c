/***************************************************************************
simple module contain init_module & cleanup_module
 ***************************************************************************/    

#include <linux/kernel.h>
#include <linux/module.h>

int init_module(void)
{
	printk("Hello World - simple module is loaded\n");
	return 0;
}

void cleanup_module(void)
{
	printk("simple module is unloaded\n");
}

MODULE_LICENSE("GPL");

