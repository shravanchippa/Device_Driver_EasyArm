#include<linux/fs.h>
#include<linux/module.h>
#include<asm/uaccess.h>
#include <linux/gpio.h>

int dev_no,ret;
char buff_so[4];
char buff_ds[4];


int myled_open (struct inode *, struct file *);
int myled_close (struct inode *, struct file *);
ssize_t myled_read (struct file *, char __user *, size_t, loff_t *);
ssize_t myled_write (struct file *, const char __user *, size_t, loff_t *);


const struct file_operations myled_fops=
			{ .owner=THIS_MODULE,
			  .open=myled_open,
			  .release=myled_close,
			  .read=myled_read,
			  .write=myled_write
			};		

int myled_init(void)
{
gpio_set_value(62,0);
gpio_set_value(63,0);	
printk("led_module inserted\n");
dev_no=register_chrdev(0,"MYLED",&myled_fops);	//to get the major number from kernel by cat /proc/devices
return 0;				  //after geting the major number (250 MYLED)
}					        //mknod /dev/MYLED c 250 0
						//ls -l /dev (it will show all drivers)
						//crw-r--r--  1 root root    250,   0 Oct  7 02:50 MYLED
						//compile the user_app (gcc -o user_app user_app.c)
						//./user_app 

void myled_exit(void)
{
unregister_chrdev(dev_no,"MYLED");
printk("led_module removed\n");
}

//...........................................................................

int myled_open (struct inode *inode, struct file *file)
{
printk("led_module opened\n");
gpio_direction_output(62,1);		//to configure the pin as output
gpio_direction_output(63,1);		//to configure the pin as output
return 0;
}
//..........................................................................

ssize_t myled_read (struct file *file, char __user *usr, size_t ret, loff_t *off)
{
printk("led_module read\n");
ret=copy_to_user(usr, buff_so, ret);

return ret;
}


ssize_t myled_write (struct file *file, const char __user *usr, size_t ret, loff_t *off)
{
printk("led_module write\n");
ret=copy_from_user(buff_ds, usr, ret);
printk("Written in Kernel : %s\n",buff_ds);
if(buff_ds[1]=='N')
{
gpio_set_value(62,1);
gpio_set_value(63,1);
}
else
{
gpio_set_value(62,0);
gpio_set_value(63,0);
}
return ret;
}


int myled_close (struct inode *inode, struct file *file)
{
printk("led_module closed\n");
return 0;
} 

module_init(myled_init);
module_exit(myled_exit);



MODULE_AUTHOR("ASHUTOSH SINGH)");
MODULE_DESCRIPTION("this is test modules used to test led");
MODULE_LICENSE("GPL"); 



