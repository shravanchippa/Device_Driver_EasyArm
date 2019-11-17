/***************************************************************************
 ***************************************************************************/

#include <linux/kernel.h>
#include <linux/module.h>
#include "add.h"

int add(int a, int b)
{
	return a + b;
}
EXPORT_SYMBOL_GPL(add);

int init_module(void)
{
	printk("simple module is loaded\n");
	return 0;
}

void cleanup_module(void)
{
	printk("simple module is unloaded\n");
}

MODULE_LICENSE("GPL");

