#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <asm/current.h>
#include <linux/device.h>
#include "ioctl_const.h"

#define CHAR_DEV_NAME "sample_ioctl_cdrv"
#define MAX_LENGTH length
#define SUCCESS 0

static char *char_device_buf;
struct cdev *sample_ioctl_cdev;
static unsigned int length = 4000;
dev_t mydev;
int count=1;
static struct class *sample_ioctl_class;

static int char_dev_open(struct inode *inode,  struct file  *file)
{
	static int counter = 0;
	counter++;
	printk(KERN_INFO "Number of times open() was called: %d\n", counter);
	printk (KERN_INFO "MAJOR number = %d, MINOR number = %d\n",imajor (inode), iminor (inode));
	printk(KERN_INFO "Process id of the current process: %d\n",current->pid );
	printk (KERN_INFO "ref=%d\n", module_refcount(THIS_MODULE));
	return SUCCESS;
}

static int char_dev_release(struct inode *inode,struct file *file)
{
	return SUCCESS;
}

static ssize_t char_dev_read(struct file *file,  char *buf,  size_t lbuf, loff_t *ppos)
{
	//int maxbytes; /* number of bytes from ppos to MAX_LENGTH */
	int bytes_to_do; /* number of bytes to read */
	int nbytes; /* number of bytes actually read */

	nbytes =  copy_to_user( buf, /* to */       char_device_buf , /* from */     bytes_to_do ); /* how many bytes */
	
	return nbytes;	
}

static ssize_t char_dev_write(struct file *file,    const char *buf,  size_t lbuf,   loff_t *ppos)
{
	int nbytes; /* Number of bytes written */
	int bytes_to_do; /* Number of bytes to write */
	//int maxbytes; /* Maximum number of bytes that can be written */

	nbytes =    copy_from_user( char_device_buf , /* to */	 buf, /* from */ bytes_to_do ); /* how many bytes */
	return nbytes;
}


/*
				     big_kernel_lock()	
ioctl----> sys_ioctl---->do_ioctl ------------------->fops--->ioctl==> char_dev_ioctl 
                                       unlock()


*/
static int char_dev_ioctl(struct inode *inode,    struct file *filp,    unsigned int cmd,   unsigned long arg)
{
 /*	step 1: verify args */

        unsigned int i, size;
        char *new_buf;
        char c;
        int retbytes;

        if( _IOC_TYPE(cmd) != sample_ioctl_MAGIC ) return -ENOTTY;
        if( _IOC_NR(cmd) > sample_ioctl_MAX_CMDS ) return -ENOTTY;

        if( _IOC_DIR(cmd) & _IOC_READ )
                if( !access_ok(VERIFY_WRITE, (void *)arg, _IOC_SIZE(cmd) ) )
                        return -EFAULT;
        if( _IOC_DIR(cmd) & _IOC_WRITE )
                if( !access_ok(VERIFY_READ, (void *)arg, _IOC_SIZE(cmd) ) )
                        return -EFAULT;

	/* implement support of commands using switch/case */

        /* sounds ok lets continue */
        switch(cmd) {

        case sample_ioctl_FILL_ZERO:
                for(i=0; i<MAX_LENGTH; i++) char_device_buf[i] = 0;
		printk(KERN_DEBUG " FILL_ZERO config executed\n ");
                break;

        case sample_ioctl_FILL_CHAR:
                retbytes = copy_from_user( &c, (char *)arg, sizeof(char) );
                for(i=0; i<MAX_LENGTH; i++) char_device_buf[i] = c;
		printk(KERN_DEBUG "FILL_CHAR config executed \n");
                break;

        case sample_ioctl_SET_SIZE:
		 if (!capable(CAP_SYS_ADMIN))
                	return -EPERM;

                retbytes=copy_from_user( &size, (unsigned int*)arg,
                                sizeof(unsigned int) );
                new_buf = (char *)kmalloc( size*sizeof(char),
                                           GFP_KERNEL );
                if( !new_buf ) return -ENOSPC;
                kfree( char_device_buf );
                char_device_buf = (char *)new_buf;
                MAX_LENGTH = size;
                for(i=0; i<MAX_LENGTH; i++) char_device_buf[i] = 0;
                filp->f_pos = 0;
		printk(KERN_DEBUG "SET_SIZE config executed\n");
                break;
	
	case sample_ioctl_GET_SIZE:
		if (!capable(CAP_SYS_ADMIN))
                        return -EPERM;

		size = MAX_LENGTH;
                retbytes=copy_to_user( (unsigned int*)arg, &size , sizeof(unsigned int) );
		printk(KERN_DEBUG "GET_SIZE config executed \n");	
                break;

        }

        return SUCCESS;
}	


static struct file_operations char_dev_fops = {
	.owner = THIS_MODULE,
	.ioctl = char_dev_ioctl,
	.read = char_dev_read,
	.write = char_dev_write,
	.open = char_dev_open,
	.release = char_dev_release,
};

static __init int char_dev_init(void)
{
	int ret;

	if (alloc_chrdev_region (&mydev, 0, count, CHAR_DEV_NAME) < 0) {
            printk (KERN_ERR "failed to reserve major/minor range\n");
            return -1;
    }

        if (!(sample_ioctl_cdev = cdev_alloc ())) {
            printk (KERN_ERR "cdev_alloc() failed\n");
            unregister_chrdev_region (mydev, count);
            return -1;
 	}
	cdev_init(sample_ioctl_cdev,&char_dev_fops);

	ret=cdev_add(sample_ioctl_cdev,mydev,count);
	if( ret < 0 ) {
		printk(KERN_INFO "Error registering device driver\n");
	        cdev_del (sample_ioctl_cdev);
                unregister_chrdev_region (mydev, count); 	
		return -1;
	}

	sample_ioctl_class = class_create (THIS_MODULE, "VIRTUAL");
        device_create (sample_ioctl_class, NULL, mydev, NULL, "%s", "sample_ioctl_cdrv");

	printk(KERN_INFO"\nDevice Registered: %s\n",CHAR_DEV_NAME);
	printk (KERN_INFO "Major number = %d, Minor number = %d\n", MAJOR (mydev),MINOR (mydev));

	char_device_buf =(char *)kmalloc(MAX_LENGTH,GFP_KERNEL);
	return 0;
}

static __exit void  char_dev_exit(void)
{
	 device_destroy (sample_ioctl_class, mydev);
         class_destroy (sample_ioctl_class);
	 cdev_del(sample_ioctl_cdev);
	 unregister_chrdev_region(mydev,1);
	 kfree(char_device_buf);
	 printk(KERN_INFO "\n Driver unregistered \n");
}
module_init(char_dev_init);
module_exit(char_dev_exit);

MODULE_AUTHOR("Team");
MODULE_DESCRIPTION("Character Device Driver - Test");
MODULE_LICENSE("GPL");
