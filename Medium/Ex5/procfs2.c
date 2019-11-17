#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include<linux/stat.h>
#include <linux/string.h>
/* Specifically, a module */
/* We're doing kernel work */
/* Necessary because we use the proc fs */

#define PROCFS_MAX_SIZE 1024

#define procfs_name "buffer1k"
/**
* This structure hold information about the /proc file
*
*/
static struct proc_dir_entry *Our_Proc_File=0;

static char procfs_buffer[PROCFS_MAX_SIZE];
/**
* The size of the buffer
*
*/
static unsigned long procfs_buffer_size = 0;

/* Put data into the proc fs file.
*
* Arguments
* =========
* 1. The buffer where the data is to be inserted, if
*
you decide to use it.
* 2. A pointer to a pointer to characters. This is
*
useful if you don't want to use the buffer
*
allocated by the kernel.
* 3. The current position in the file
* 4. The size of the buffer in the first argument.
* 5. Write a "1" here to indicate EOF.
* 6. A pointer to data (useful in case one common
*
read for multiple /proc/... entries)
*
* Usage and Return Value
* ======================
* A return value of zero means you have no further
* information at this time (end of file). A negative
* return value is an error condition.
*
* For More Information
* ====================
* The way I discovered what to do with this function
* wasn't by reading documentation, but by reading the
* code which used it. I just looked to see what uses
* the get_info field of proc_dir_entry struct (I used a
* combination of find and grep, if you're interested),
* and I saw that it is used in <kernel source
* directory>/fs/proc/array.c.
*
* If something is unknown about the kernel, this is
* usually the way to go. In Linux we have the great
* advantage of having the kernel source code for
* free − use it.
*/

static int procfile_read(char *buffer,char **buffer_location,off_t offset, int buffer_length, int *eof, void *data)
{
int ret;
printk(KERN_INFO "procfile_read (/proc/%s) called\n", procfs_name);
/*
* We give all of our information in one go, so if the
* user asks us if we have more information the
* answer should always be no.
*
* This is important because the standard read
* function from the library would continue to issue
* the read system call until the kernel replies
* that it has no more information, or until its
* buffer is filled.
*/
if (offset > 0) {
/* we have finished to read, return 0 */
ret = 0;
} else {
/* fill the buffer, return the buffer size */
ret = sprintf(buffer, "welcome to HelloWorld!\n");
}
return ret;
}


static int procfile_write(struct file *file, const char *buffer, unsigned long count,void *data)
{
/* get buffer size */
procfs_buffer_size = count;
printk(KERN_INFO "procfile_write (/proc/%s) called\n", procfs_name);
if (procfs_buffer_size > PROCFS_MAX_SIZE ) {
procfs_buffer_size = PROCFS_MAX_SIZE;
}
/* write data to the buffer */
if (copy_from_user(procfs_buffer, buffer, procfs_buffer_size) ) {
return -EFAULT;
}
printk(KERN_INFO "%s\n", procfs_buffer);
return procfs_buffer_size;
}

int init_module()
{
Our_Proc_File = create_proc_entry(procfs_name, 0644, NULL);
if (Our_Proc_File == NULL) {
remove_proc_entry(procfs_name, NULL);
printk(KERN_ALERT "Error: Could not initialize /proc/%s\n",procfs_name);
return -EINVAL;
}
Our_Proc_File->read_proc=procfile_read;
Our_Proc_File->write_proc= procfile_write;

//Our_Proc_File->owner=THIS_MODULE;
Our_Proc_File->mode=S_IFREG | S_IRUGO | S_IWUGO;
Our_Proc_File->uid=0;
Our_Proc_File->gid=0;
Our_Proc_File->size=37;
printk(KERN_INFO "/proc/%s created\n", procfs_name);
return 0;
/* everything is ok */
}
void cleanup_module()
{
remove_proc_entry(procfs_name, NULL);
printk(KERN_INFO "/proc/%s removed\n", procfs_name);
}


