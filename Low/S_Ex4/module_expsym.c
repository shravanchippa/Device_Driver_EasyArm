/***********************************************************************
 ************************** ********** ********************************/

#include <linux/module.h>
#include <linux/init.h>


char MyString[16];
char MyInteger;

EXPORT_SYMBOL(MyString);
EXPORT_SYMBOL(MyInteger);

int expsym_init(void)                  						/*------ module entry point -------*/ 
{
	  printk("Export Symbol Module Initialized\n");       			/*------ kernel print function ----*/
	  memcpy(MyString, "Hello World\n", 13);
	  return 0;  
}

void expsym_exit(void)              						/*------ cleanup module -----------*/
{
	  printk("Export Symbol Module is removed\n");
}

module_init(expsym_init);
module_exit(expsym_exit);

MODULE_LICENSE("GPL");

