/***********************************************************************
***********************************************************************/

#include <linux/module.h>
#include <linux/init.h>

/*--------------------------------------------------------------------*/
extern char MyString[];

int impsym_init(void)                  			/*------ module entry point -------*/ 
{
  printk("Import Symbol Module Initialized\n");  	/*- kernel print function -*/
  printk("%s", MyString);
  return 0;  
}

void impsym_exit(void)              			/*------ cleanup module -----------*/
{
  printk("Import Symbol Module is removed\n");
}

module_init(impsym_init);
module_exit(impsym_exit);

MODULE_LICENSE("GPL");

