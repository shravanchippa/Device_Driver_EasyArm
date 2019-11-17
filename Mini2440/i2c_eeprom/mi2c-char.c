/*
 * Char device wrapper for the mi2c-i2c kernel module example.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/slab.h>

#include "mi2c.h"
#include "mi2c-i2c.h"

#define USER_BUFF_SIZE 256


struct mi2c_dev {
	dev_t devt;
	struct cdev cdev;
	struct semaphore sem;
	struct class *class;
	char *user_buff;
};

static struct mi2c_dev mi2c_dev;

/* 
 * An example just to demo how you would use the write/read functions.
 * I have some BlinkM leds this driver is handling. This is how you read
 * their current color value. No error handling on the val pointer.
 */
static ssize_t mi2c_write(struct file *filp, char __user *buff, 
				size_t count, loff_t *offp)
{
	
	size_t len;
	unsigned int result,i;
	unsigned char buffer[2];
	/* 
	Generic user progs like cat will continue calling until we 
	return zero. So if *offp != 0, we know this is at least the
	second call.
	*/
	
	if (down_interruptible(&mi2c_dev.sem)) 
		return -ERESTARTSYS;

	memset(mi2c_dev.user_buff, 0, USER_BUFF_SIZE);
        copy_from_user(mi2c_dev.user_buff,buff,count);
	
	buffer[0] =mi2c_dev.user_buff[0];
	i=buffer[0];
	printk("\nStart Addres =%d\n",i);
	len=count-1;
	if((len > 0 && len <= 128) && (i + len <= 128) && (i >= 0 && i < 128))
	{

	/* We know the Arduino is device_id 2. It's just a demo... */
	result = mi2c_i2c_write(0, mi2c_dev.user_buff, count);

	if (result != count)
		return result;
        for(i=1;i<=len;i++)
	printk("%c",mi2c_dev.user_buff[i]);
	}
	else
	{	
	printk("Either length or address are out of boundries\n");
	return -1;		
	}	
	up(&mi2c_dev.sem);
	
	return result;	
}

static ssize_t mi2c_read(struct file *filp, char __user *buff, 
				size_t count, loff_t *offp)
{
	//ssize_t status;
	size_t len;
	unsigned int result,i;
	unsigned char buffer[2];
	/* 
	Generic user progs like cat will continue calling until we 
	return zero. So if *offp != 0, we know this is at least the
	second call.
	*/
	
	if (down_interruptible(&mi2c_dev.sem)) 
		return -ERESTARTSYS;

	memset(mi2c_dev.user_buff, 0, USER_BUFF_SIZE);
        copy_from_user(mi2c_dev.user_buff,buff,1);
	len = count;
	buffer[0] =mi2c_dev.user_buff[0];
	i=buffer[0];
	printk("\nStart Addres =%d\n",i);

	if((len > 0 && len <= 128) && (i + len <= 128) && (i >= 0 && i < 128))
	{

	/* We know the Arduino is device_id 2. It's just a demo... */
	result = mi2c_i2c_write(0, buffer, 1);

	if (result != 1)
		return result;
	memset(mi2c_dev.user_buff, 0, USER_BUFF_SIZE);
	result = mi2c_i2c_read(0, mi2c_dev.user_buff, len);

	if (result != len)
		return result;
        for(i=0;i<len;i++)
	printk("%c",mi2c_dev.user_buff[i]);
        copy_to_user(buff,mi2c_dev.user_buff,len);
	}
	else
	{	
	printk("Either length or address are out of boundries\n");
	return -1;		
	}	
	up(&mi2c_dev.sem);
	
	return result;	
}

static int mi2c_open(struct inode *inode, struct file *filp)
{	
	int status = 0;

	if (down_interruptible(&mi2c_dev.sem)) 
		return -ERESTARTSYS;
	
	if (!mi2c_dev.user_buff) {
		mi2c_dev.user_buff = kmalloc(USER_BUFF_SIZE, GFP_KERNEL);

		if (!mi2c_dev.user_buff) {
			printk(KERN_ALERT 
				"mi2c_open: user_buff alloc failed\n");

			status = -ENOMEM;
		}
	}

	up(&mi2c_dev.sem);

	return status;
}

static const struct file_operations mi2c_fops = {
	.owner = THIS_MODULE,
	.open =	mi2c_open,	
	.read =	mi2c_read,
	.write = mi2c_write,
};

static int __init mi2c_init_cdev(void)
{
	int error;

	mi2c_dev.devt = MKDEV(0, 0);

	error = alloc_chrdev_region(&mi2c_dev.devt, 0, 1, DRIVER_NAME);
	if (error < 0) {
		printk(KERN_ALERT 
			"alloc_chrdev_region() failed: error = %d \n", 
			error);
		
		return -1;
	}

	cdev_init(&mi2c_dev.cdev, &mi2c_fops);
	mi2c_dev.cdev.owner = THIS_MODULE;

	error = cdev_add(&mi2c_dev.cdev, mi2c_dev.devt, 1);
	if (error) {
		printk(KERN_ALERT "cdev_add() failed: error = %d\n", error);
		unregister_chrdev_region(mi2c_dev.devt, 1);
		return -1;
	}	

	return 0;
}

static int __init mi2c_init_class(void)
{
	mi2c_dev.class = class_create(THIS_MODULE, DRIVER_NAME);

	if (!mi2c_dev.class) {
		printk(KERN_ALERT "class_create(mi2c) failed\n");
		return -1;
	}

	if (!device_create(mi2c_dev.class, NULL, mi2c_dev.devt, 
				NULL, DRIVER_NAME)) {
		class_destroy(mi2c_dev.class);
		return -1;
	}

	return 0;
}

static int __init mi2c_init(void)
{
	printk(KERN_INFO "mi2c_init()\n");

	memset(&mi2c_dev, 0, sizeof(struct mi2c_dev));

	sema_init(&mi2c_dev.sem, 1);

	if (mi2c_init_cdev() < 0)
		goto init_fail_1;

	if (mi2c_init_class() < 0)
		goto init_fail_2;

	if (mi2c_init_i2c() < 0)
		goto init_fail_3;


	return 0;

init_fail_3:
	device_destroy(mi2c_dev.class, mi2c_dev.devt);
	class_destroy(mi2c_dev.class);

init_fail_2:
	cdev_del(&mi2c_dev.cdev);
	unregister_chrdev_region(mi2c_dev.devt, 1);

init_fail_1:

	return -1;
}
module_init(mi2c_init);

static void __exit mi2c_exit(void)
{
	printk(KERN_INFO "mi2c_exit()\n");

	mi2c_cleanup_i2c();

	device_destroy(mi2c_dev.class, mi2c_dev.devt);
  	class_destroy(mi2c_dev.class);

	cdev_del(&mi2c_dev.cdev);
	unregister_chrdev_region(mi2c_dev.devt, 1);

	if (mi2c_dev.user_buff)
		kfree(mi2c_dev.user_buff);
}
module_exit(mi2c_exit);


MODULE_AUTHOR("Scott Ellis");
MODULE_DESCRIPTION("mi2c driver");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_VERSION("0.1");

