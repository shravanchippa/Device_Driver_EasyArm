#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <mach/regs-gpio.h>
#include <mach/hardware.h>


#define MAJORNO 10
#define MINORNO  0
#define DEVICE_NAME "misc_char_cdrv"
#define MAX_LENGTH 4000
#define SUCCESS 0

static char char_device_buf[MAX_LENGTH];
int count=1,inuse=0,int_data=0;

static unsigned long led_table [] = {	S3C2410_GPB5,	S3C2410_GPB6,	S3C2410_GPB7,	S3C2410_GPB8 };

static unsigned int led_cfg_table [] = {S3C2410_GPB5_OUTP,S3C2410_GPB6_OUTP,S3C2410_GPB7_OUTP,	S3C2410_GPB8_OUTP};


static int char_dev_open(struct inode *inode,
			    struct file  *file)
{
	if(inuse)
	{
		printk(KERN_INFO "\nDevice busy %s\n",DEVICE_NAME);
		return -EBUSY;
	}	
	inuse=1;
	printk(KERN_INFO "Open operation invoked \n");
	return SUCCESS;
		
}

static int char_dev_release(struct inode *inode, struct file *file)
{
	inuse=0;
	return SUCCESS;
}

static ssize_t char_dev_write(struct file *file, const char *buf, size_t lbuf, loff_t *ppos)
{
int j;   
   	int nbytes =copy_from_user (char_device_buf, buf, lbuf);
       	printk (KERN_INFO "\n Rec'vd data from app %s , nbytes=%d\n",char_device_buf,nbytes);
	strict_strtoul(char_device_buf, 0, (unsigned long *)&int_data);
	printk (KERN_INFO "\n Rec'vd data from app %d\n",int_data);
	
	for (j = 0; j < 4; j++){
	if(int_data & (1 << j)) 
	s3c2410_gpio_setpin(led_table[j], 0);
	else
	s3c2410_gpio_setpin(led_table[j], 1);
	}
       return nbytes;

}

static struct file_operations char_dev_fops = {
	.owner = THIS_MODULE,
	.write = char_dev_write,
	.open = char_dev_open,
	.release = char_dev_release
};

static struct miscdevice misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &char_dev_fops,
};
static int __init char_dev_init(void)
{
	int ret;
	int i;
	ret = misc_register(&misc);

	if( ret < 0 ) {
		printk("Error registering device driver\n");
		return ret;
	}

	
	for (i = 0; i < 4; i++) {
		s3c2410_gpio_cfgpin(led_table[i], led_cfg_table[i]);
		s3c2410_gpio_setpin(led_table[i], 0);
	}
	printk (DEVICE_NAME"\tinitialized\n");
	
	memset(char_device_buf,'\0',MAX_LENGTH);
	return ret;
}

static void __exit char_dev_exit(void)
{
	
	 misc_deregister(&misc);
	 printk(KERN_INFO "\n Driver unregistered \n");
}

module_init(char_dev_init);
module_exit(char_dev_exit);

MODULE_AUTHOR("Team");
MODULE_DESCRIPTION("Character Device Driver - Test");
MODULE_LICENSE("GPL");
/* End of code */
