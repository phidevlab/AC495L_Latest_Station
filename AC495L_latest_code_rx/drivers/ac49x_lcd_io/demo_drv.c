#include <linux/config.h>
#include <linux/init.h>
#include <linux/module.h>

#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>

#include <asm/uaccess.h>

#include <linux/fcntl.h>
#include <linux/delay.h>
#include "char_lcd.h"


extern SSP_HAL_I2C_INFO_T* ssp_i2c_open( void );
static SSP_HAL_I2C_INFO_T * g_fd_i2c = NULL;

extern int ssp_i2c_write( SSP_HAL_I2C_INFO_T  *info, UINT8    *buffer, UINT32   len);
extern int ssp_dev_init(void);





static int lcd_open(struct inode *p_inode, struct file *p_file )
{
	printk(KERN_INFO "klcd Driver: open()\n");

	if(NULL == g_fd_i2c)
	{	g_fd_i2c = ssp_i2c_open();
		if(NULL == g_fd_i2c)
		{
			printk("open ssp spi fail.\n");
			return -1;
		}
		printk("i2c device is opened.\n");
	}
	return 0;
}

static int lcd_close(struct inode *p_inode, struct file *p_file )
{
	printk(KERN_INFO "klcd Driver: close()\n\n");
	return 0;
}
/* file operation structure */
static struct file_operations klcd_fops =
{
	.owner          = THIS_MODULE,
	.open           = lcd_open,
	.release        = lcd_close,
	//.read           = lcd_read,
	//.write          = lcd_write,
	//.unlocked_ioctl	= lcd_ioctl,
};





static int hello_init(void)
{
    printk(KERN_INFO "[init] Can you feel me?\n");

	struct device *dev_ret;

	// dynamically allocate device major number
	if(alloc_chrdev_region(&dev_number, MINOR_NUM_START , MINOR_NUM_COUNT , DEVICE_NAME ) < 0)
	{
		printk( KERN_DEBUG "ERR: Failed to allocate major number \n" );
		return -1;
	}

	// create a class structure
	klcd_class = class_create(THIS_MODULE, CLASS_NAME);

	if(IS_ERR(klcd_class))
	{
		unregister_chrdev_region( dev_number, MINOR_NUM_COUNT );
		printk(KERN_DEBUG "ERR: Failed to create class structure \n");

		return PTR_ERR(klcd_class) ;
	}

	// create a device and registers it with sysfs
	dev_ret = device_create(klcd_class, NULL, dev_number, NULL, DEVICE_NAME);

	if(IS_ERR(dev_ret))
	{
		class_destroy(klcd_class);
		unregister_chrdev_region(dev_number, MINOR_NUM_COUNT);
		printk( KERN_DEBUG "ERR: Failed to create device structure \n" );

		return PTR_ERR(dev_ret);
	}

	// initialize a cdev structure
	cdev_init(&klcd_cdev, &klcd_fops);

	// add a character device to the system
	if(cdev_add( &klcd_cdev, dev_number, MINOR_NUM_COUNT) < 0)
	{
		device_destroy(klcd_class, dev_number);
		class_destroy(klcd_class);
		unregister_chrdev_region(dev_number, MINOR_NUM_COUNT);
		printk(KERN_DEBUG "ERR: Failed to add cdev \n");

		return -1;
	}




    return 0;
}

static void hello_exit(void)
{
    printk(KERN_INFO "[exit] Yes.\n");
	// remove a cdev from the system
	cdev_del(&klcd_cdev);

	// remove device
	device_destroy(klcd_class, dev_number);

	// destroy class
	class_destroy(klcd_class);

	// deallocate device major number
	unregister_chrdev_region(MAJOR(dev_number), MINOR_NUM_COUNT);	

	printk(KERN_INFO "klcd Driver Exited. \n");

}


module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("Alan Wang <alan@wrcode.com>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple Hello World Module");
MODULE_ALIAS("A simple module");

 
