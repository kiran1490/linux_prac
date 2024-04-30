/*file name: cdrv_misc_driver.c
 * Description: register the char driver as misc driver i.e populate the miscdevice structure -- Char driver registration.
 * Data structures : 
 * 1. miscdevice -- The struct used ofr registering the char device with major number(10) default
 * 2. struct file_operations -- The structure is used in implementing the file operations of the character file.
 * 				These file operations is used when the device is being accessed from the vfs.
 *
 * API's Used:
 * 1. misc_register(struct miscdevice*)-- api to register the char device with the misc device system
 * 2. misc_deregister_chrdev(struct miscdevice*); -- api to deregister the char driver with misc device system
 * Procedure:
 * 1. code the file operations(open,release,read and write)
 * 2. Enclose the file operations in the file_operations struct
 * 3. Code the module_init with the misc_register() api
 * 4. Code the module_exit with the free and misc_deregister() apis
 * 	
 *
 * */



#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/printk.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/miscdevice.h>

#define COUNT 1
#define SUCCESS 0
#define FAIL -1
#define DRV_NAME "misc_virt_driver"



static int cdrv_open(struct inode * cdrv_inode, struct file * cdrv_file)
{
	dump_stack();
	printk("Successfully opened the driver file\n");
	return SUCCESS;
}

ssize_t cdrv_read(struct file *cdrv_file, char __user *buf, size_t count, loff_t *offset)
{
	dump_stack();
	printk("Successfully reached the read function\n");
	return count;
}

ssize_t cdrv_write(struct file *cdrv_file, const char __user *ibuf, size_t count, loff_t *offset)
{
	dump_stack();
	printk("successfully reached the write function\n");
	return count;
}

static int cdrv_release(struct inode *inode, struct file *file)
{
	dump_stack();
	printk("Successfully closed the driver file\n");
	return SUCCESS;
}
static struct file_operations cdrv_fops = {
	.owner = THIS_MODULE,
	.open = cdrv_open,
	.read = cdrv_read,
	.write = cdrv_write,
	.release = cdrv_release
};
static struct miscdevice misc_drv = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DRV_NAME,
	.fops = &cdrv_fops
};
int cdrv_init(void)
{
	int ret = 0;
	/*
	 * wrapper function around cdev calls
	 */
	ret = misc_register(&misc_drv);
	printk("Driver loaded successfully\n");
	return 0;
}
void cdrv_exit(void)
{
	misc_deregister(&misc_drv);
	printk("rmmod successed\n");
}
module_init(cdrv_init);
module_exit(cdrv_exit);

MODULE_AUTHOR("Kiran Allada kiran.allada.180@gmail.com");
MODULE_DESCRIPTION("Char driver skeleton");
MODULE_LICENSE("GPL");
