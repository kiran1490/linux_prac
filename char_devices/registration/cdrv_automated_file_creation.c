/*file name: cdrv_automated_file_creation.c
 * Description: This is the skeleton module of the chardriver with dynamic allocation of  major and minor numbers and creating the driver file automatically ny triggering uevent to udevd instaed of using the mknod command -- Char driver registration.
 * Data structures : 
 * 1. dev_t -- 32bit uint which forms from the majorno-12bits and minorno-20 bits -- use MKDEV macro to genetare the dev_t
 * 2. struct cdev -- character device structure which is used in the cdev_init,cdev_add and cdev_release apis
 * 		     A wrapper around the struct file_operations structure.
 * 3. struct file_operations -- The structure is used in implementing the file operations of the character file.
 * 				These file operations is used when the device is being accessed from the vfs.
 * 4. struct class *virt_class -- the class struct to crateing a new class of devices which can be seen on /sys/class/ folder.
 * 5. struct device *virt_device -- the device struct to creating a new device under/sys/class/virt_class/ folder
 *
 * API's Used:
 * 1. alloc_chrdev_region(dev_t *mydev,baseminor,COUNT,DRV_NAME)-- allocate a major number which is free and assign the count number of devices from basminor
 *
 *
 * Note:the following apis and data structures are same as the static allocation of the major and minor number char driver file.
 *
 *
 * 2. my_char_cdev = cdev_alloc(); -- It is similar to malloc to get the cdev object
 * 3. cdev_init(my_char_cdev,&fops); -- copy the fops to the cdev object
 * 4. cdev_add(my_char_cdev,mydev,COUNT); -- Bind the file which is created by mknod with the my_char_cdev -- assign file operations to the mydev device file
 * 5. cdev_del(my_char_cdev); -- free the my_char_cdev object
 * 6. unregister_chrdev_region(mydev,COUNT); -- free major number minor number used
 * ***** device creation and deletion apis******
 * 7. virt_class_member = create_class(THIS_MODULE,"virt_class") -- to create a new class of devices with name virt_class
 * 8. virt_device_member = create_device(virt_class_member,NULL,mydev,NULL,"virt_device") -- to create a new device with name virt_device
 * 9. destroy_device(virt_class_member,mydev) -- to destroy the created device
 * 10. destroy_class(virt_class_member) -- to destroy the class
 * Procedure:
 * 1. code the file operations(open,release,read and write)
 * 2. Enclose the file operations in the file_operations struct
 * 3. Code the module_init which has the all the char region registartion, cdev apis upto init, class and device creation apis
 * 4. Code the module_exit with the free and unregistering the major numbers and destroying the device and class deletion apis
 * 	
 *
 * */



#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/printk.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>

#define COUNT 1
#define SUCCESS 0
#define FAIL -1
#define DRV_NAME "cdrv_virt_device"


static struct cdev *my_char_dev;
static dev_t mydev;

static struct class *virt_class;
static struct device *virt_device;

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
	device_destroy(virt_class,mydev);
	class_destroy(virt_class);
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
	int baseminor = 0;
	/*
	 * change from the static example
	 */
	ret = alloc_chrdev_region(&mydev,baseminor,COUNT,DRV_NAME);
	my_char_dev = cdev_alloc();
	cdev_init(my_char_dev,&cdrv_fops);
	ret = cdev_add(my_char_dev,mydev,COUNT);
	if(ret < 0)
		printk("error in registering the device driver\n");
	virt_class = class_create(THIS_MODULE,"virtClass");
	virt_device = device_create(virt_class,NULL,mydev,NULL,"virtDev");
	printk("Driver loaded successfully\n");
	return 0;
}
module_init(cdrv_init);
module_exit(cdrv_exit);

MODULE_AUTHOR("Kiran Allada kiran.allada.180@gmail.com");
MODULE_DESCRIPTION("Char driver skeleton");
MODULE_LICENSE("GPL");
