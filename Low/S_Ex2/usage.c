/***************************************************************************
this module is going to use add function which is exported by other module
 ***************************************************************************/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include "add.h"

int usage_init(void)
{
	printk("usage module loaded\n");
	printk("sum=%d\n",add(2,3));
	return 0;
}

void usage_exit(void)
{
	printk("usage module unloaded\n");
}

module_init(usage_init);
module_exit(usage_exit);

MODULE_LICENSE("GPL");

