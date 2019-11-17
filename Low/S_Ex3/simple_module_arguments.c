
/***************************************************************************
 ***************************************************************************/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/stat.h>

static char *whom = "world";
static int howmany = 1;
static bool status=1;
module_param(howmany, int, S_IRUGO);
module_param(whom, charp, S_IRUGO);
module_param(status, bool, S_IRUGO);

int init_module(void)
{
	printk("simple module with agrments is loaded\n");
	printk("char string name =%s int value =%d status = %d\n",whom,howmany,status);
	return 0;
}

void cleanup_module(void)
{
	printk("simple module with arguments is unloaded\n");
}

MODULE_LICENSE("GPL");

