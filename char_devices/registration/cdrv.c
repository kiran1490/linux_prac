/*file name: cdrv.c
 * Description: This is the skeleton module of the chardriver with static major and minor numbers -- Char driver registration.
 * Data structures : 
 * 1. dev_t -- 32bit uint which forms from the majorno-12bits and minorno-20 bits -- use MKDEV macro to genetare the dev_t
 * 2. struct cdev -- character device structure which is used in the cdev_init,cdev_add and cdev_release apis
 * 		     A wrapper around the struct file_operations structure.
 * 3. struct file_operations -- The structure is used in implementing the file operations of the character file.
 * 				These file operations is used when the device is being accessed from the vfs.
 *
 * API's Used:
 * 1. register_chrdev_region(dev_t mydev,COUNT,DRV_NAME)-- claim the owenrship of the file. Make me the owner of the mydev file
 * 2. my_char_cdev = cdev_alloc(); -- It is similar to malloc to get the cdev object
 * 3. cdev_init(my_char_cdev,&fops); -- copy the fops to the cdev object
 * 4. cdev_add(my_char_cdev,mydev,COUNT); -- Bind the file which is created by mknod with the my_char_cdev -- assign file operations to the mydev device file
 * 5. cdev_del(my_char_cdev); -- free the my_char_cdev object
 * 6. unregister_chrdev_region(mydev,COUNT); -- free major number minor number used
 *
 * Procedure:
 * 1. code the file operations(open,release,read and write)
 * 2. Enclose the file operations in the file_operations struct
 * 3. Code the module_init which has the all the char region registartion and cdev apis upto init
 * 4. Code the module_exit with the free and unregistering the major numbers
 * 	
 *
 * */



#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/printk.h>
#include<linux/fs.h>
#include<linux/cdev.h>

#define MAJORNO 190
#define MINORNO 0
#define COUNT 1
#define SUCCESS 0
#define FAIL -1
#define DRV_NAME "cdrv"


static struct cdev *my_char_dev;
static dev_t mydev;


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
void cdrv_exit(void)
{
	cdev_del(my_char_dev);
	unregister_chrdev_region(mydev,COUNT);
	printk("rmmod successed\n");
}
static struct file_operations cdrv_fops = {
	.owner = THIS_MODULE,
	.open = cdrv_open,
	.read = cdrv_read,
	.write = cdrv_write,
	.release = cdrv_release
};
int cdrv_init(void)
{
	int ret = 0;
	mydev = MKDEV(MAJORNO,MINORNO);
	ret = register_chrdev_region(mydev,COUNT,DRV_NAME);
	my_char_dev = cdev_alloc();
	cdev_init(my_char_dev,&cdrv_fops);
	ret = cdev_add(my_char_dev,mydev,COUNT);
	if(ret < 0)
		printk("error in registering the device driver\n");
	printk("Driver loaded successfully\n");
	return 0;
}
module_init(cdrv_init);
module_exit(cdrv_exit);

MODULE_AUTHOR("Kiran Allada kiran.allada.180@gmail.com");
MODULE_DESCRIPTION("Char driver skeleton");
MODULE_LICENSE("Dual MIT/DUAL");
