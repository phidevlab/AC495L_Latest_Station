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
#include "char_lcd_io.h"

#include <linux/errno.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/tty.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/fb.h>

#include <asm/io.h>
#include <asm/irq.h>
#include <asm/uaccess.h>
#include <asm/page.h>
#include <linux/proc_fs.h>

#include <linux/fb.h>
#include <linux/vmalloc.h>

#include <linux/kthread.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
#include <linux/moduleparam.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#endif

extern SSP_HAL_I2C_INFO_T *ssp_i2c_open(void);
static SSP_HAL_I2C_INFO_T *g_fd_i2c = NULL;

extern int ssp_i2c_write(SSP_HAL_I2C_INFO_T *info, UINT8 *buffer, UINT32 len);
extern int ssp_dev_init(void);

/*
 * description:	This function opens i2c device and written i2c device fd.
 */
static int i2c_interface_open(struct inode *p_inode, struct file *p_file)
{
	int i = 0;
	char temp_buf[1];
	char temp_buf_1[1];
	////printk(KERN_INFO "klcd Driver: open()\n");
	if (NULL == g_fd_i2c)
	{
		g_fd_i2c = ssp_i2c_open();
		if (NULL == g_fd_i2c)
		{
			////printk("open ssp spi fail.\n");
			return -1;
		}
		////printk("i2c device is opened.\n");
	}

	/*for (i = 0; i < 300; i++)
	{

		g_fd_i2c->addr = 0x38;
		char values_ff[1] = {0xAA};
        //strcpy(io_msg.kbuf, values_ff);
		ssp_i2c_write(g_fd_i2c, values_ff, 1);

		
		ssp_i2c_read(g_fd_i2c, temp_buf, sizeof(temp_buf));




		char values_55[1] = {0x55};
        //strcpy(io_msg.kbuf, values_ff);
		ssp_i2c_write(g_fd_i2c, values_55, 1);

		
		ssp_i2c_read(g_fd_i2c, temp_buf_1, sizeof(temp_buf_1));
	}*/

	return 0;
}

static int i2c_interface_close(struct inode *p_inode, struct file *p_file)
{
	////printk(KERN_INFO "klcd Driver: close()\n");
	return 0;
}

static void lcd_setup()
{
}

/*
 * description:	This function separates out upper and lower nibble of lcd data.
 */
static void lcd_send_char(unsigned char data, unsigned char addr)
{
	char data_u, data_l;
	uint8_t data_t[4];
	////printk(KERN_INFO "data is : = %x\n",data);
	g_fd_i2c->addr = addr;
	////printk(KERN_INFO "lcd_send_char->g_fd_i2c->addr=%x\n",g_fd_i2c->addr);
	data_u = (data & MASK_HIGHER_BIT);
	data_l = ((data << 4) & MASK_HIGHER_BIT);
	data_t[0] = data_u | DATA_EN_ACTIVATE;	 // P0,RS=1 P1,R/W=0 (W=0,R=1) P2,EN=1 P3,BACKLIGHT=1
	data_t[1] = data_u | DATA_EN_DEACTIVATE; // P0,RS=1 P1,R/W=0 (W=0,R=1) P2,EN=0 P3,BACKLIGHT=1
	data_t[2] = data_l | DATA_EN_ACTIVATE;	 // P0,RS=1 P1,R/W=0 (W=0,R=1) P2,EN=1 P3,BACKLIGHT=1
	data_t[3] = data_l | DATA_EN_DEACTIVATE; // P0,RS=1 P1,R/W=0 (W=0,R=1) P2,EN=0 P3,BACKLIGHT=1

	ssp_i2c_write(g_fd_i2c, (unsigned char *)&data_t, sizeof(data_t));
}
/*
 * description:	This function separates out upper and lower nibble of lcd command.
 */

void lcd_send_cmd(unsigned char cmd, unsigned char addr)
{
	char cmd_u, cmd_l;
	uint8_t cmd_t[4];
	// printk(KERN_INFO "cmd is : = %x\n",cmd);
	g_fd_i2c->addr = addr;
	cmd_u = (cmd & MASK_HIGHER_BIT);
	cmd_l = ((cmd << 4) & MASK_HIGHER_BIT);
	cmd_t[0] = cmd_u | CMD_EN_ACTIVATE;	  // P0,RS=0 P1,R/W=0 (W=0,R=1) P2,EN=1 P3,BACKLIGHT=1 (ON)
	cmd_t[1] = cmd_u | CMD_EN_DEACTIVATE; // P0,RS=0 P1,R/W=0 (W=0,R=1) P2,EN=0 P3,BACKLIGHT=1 (ON)
	cmd_t[2] = cmd_l | CMD_EN_ACTIVATE;	  // P0,RS=0 P1,R/W=0 (W=0,R=1) P2,EN=1 P3,BACKLIGHT=1 (ON)
	cmd_t[3] = cmd_l | CMD_EN_DEACTIVATE; // P0,RS=0 P1,R/W=0 (W=0,R=1) P2,EN=0 P3,BACKLIGHT=1 (ON)
	ssp_i2c_write(g_fd_i2c, (unsigned char *)&cmd_t, sizeof(cmd_t));
}

static void lcd_platform_release(struct device *device)
{
}

/*
 * description:	clear the display on the LCD
 */
static void lcd_clear_display(unsigned char addr)
{

	lcd_send_cmd(0x01, addr); // clear display
}

/*
 * description:	Initialize the lcd 2 line 16 character in 4 bit mode
 * @param cmd, addr-i2c device address
 */
static void lcd_initialize(unsigned char *cmd, unsigned char addr)
{
	if (cmd == NULL)
	{
		// printk( KERN_DEBUG "ERR: Empty data for lcd_print \n");
		return;
	}

	// printk( KERN_INFO "msg=%x \n",addr);
	while (*(cmd) != '\0')
	{
		lcd_send_cmd(*cmd, addr);
		cmd++;
	}
}
/*static void send_data_to_ioexpander(unsigned char *cmd , unsigned char addr)
{
	//printk( KERN_INFO "Inside send_data_to_ioexpander\n");
	char cmd_t;
	g_fd_i2c->addr = addr;
	cmd_t= (*cmd^res[0]);
	//printk( KERN_INFO "res[0]=%x \n",res[0]);
	//printk( KERN_INFO "cmd_t=%x \n",cmd_t);
	res[0]=cmd_t;
	ssp_i2c_write(g_fd_i2c,(unsigned char*)&cmd_t,sizeof(cmd_t));

}*/
static void set(unsigned char data, unsigned char addr)
{
	// printk(KERN_INFO "In set_bit\n");
	unsigned char cmd_t;
	g_fd_i2c->addr = addr;
	// printk( KERN_INFO "data=%x \n",data);
	// cmd_t=  (data|res[0]);
	// printk( KERN_INFO "res[0]=%x \n",res[0]);

	// printk( KERN_INFO "cmd_t=%x \n",cmd_t);
	// res[0]=cmd_t;
	// ssp_i2c_write(g_fd_i2c,(unsigned char*)&cmd_t,sizeof(cmd_t));
	ssp_i2c_write(g_fd_i2c, (unsigned char *)&data, sizeof(data));
}
static void clear(unsigned char data, unsigned char addr)
{
	// printk(KERN_INFO "In clear_bit\n");
	char cmd_t;
	g_fd_i2c->addr = addr;
	// printk( KERN_INFO "data=%x \n",data);
	cmd_t = (data & res[0]);
	// printk( KERN_INFO "res[0]=%x \n",res[0]);
	// printk( KERN_INFO "data=%x \n",data);
	// printk( KERN_INFO "cmd_t=%x \n",cmd_t);
	res[0] = cmd_t;
	ssp_i2c_write(g_fd_i2c, (unsigned char *)&cmd_t, sizeof(cmd_t));
}
static void send_data_to_ioexpander(unsigned char *cmd, unsigned int pin, unsigned int pin_status, unsigned char addr)
{
	////printk( KERN_INFO "Inside send_data_to_ioexpander\n");
	char cmd_t;
	g_fd_i2c->addr = addr;
	////printk(KERN_INFO "addr=0x%x\n",addr);
	////printk(KERN_INFO "pin=%u\n",pin);
	////printk(KERN_INFO "pin_status=%u\n",pin_status);
	switch (pin)
	{
	case ZERO:
		switch (pin_status)
		{
		case SET:
			set(*cmd, addr);
			break;

		case CLEAR:
			clear(*cmd, addr);
			break;
		}

		break;
	case ONE:
		switch (pin_status)
		{
		case SET:
			set(*cmd, addr);
			break;

		case CLEAR:
			clear(*cmd, addr);
			break;
		}

		break;
	case TWO:
		switch (pin_status)
		{
		case SET:
			set(*cmd, addr);
			break;

		case CLEAR:
			clear(*cmd, addr);
			break;
		}

		break;
	case THREE:
		switch (pin_status)
		{
		case SET:
			set(*cmd, addr);
			break;

		case CLEAR:
			clear(*cmd, addr);
			break;
		}

		break;
	case FOUR:
		switch (pin_status)
		{
		case SET:
			set(*cmd, addr);
			break;

		case CLEAR:
			clear(*cmd, addr);
			break;
		}

		break;
	case FIVE:
		switch (pin_status)
		{
		case SET:
			set(*cmd, addr);
			break;

		case CLEAR:
			clear(*cmd, addr);
			break;
		}

		break;
	case SIX:
		switch (pin_status)
		{
		case SET:
			set(*cmd, addr);
			break;

		case CLEAR:
			clear(*cmd, addr);
			break;
		}

		break;
	case SEVEN:
		switch (pin_status)
		{
		case SET:
			set(*cmd, addr);
			break;

		case CLEAR:
			clear(*cmd, addr);
			break;
		}

		break;
	}

	// ssp_i2c_write(g_fd_i2c,(unsigned char*)&cmd_t,sizeof(cmd_t));
}

static void ioexpander_initialization(unsigned char *cmd, unsigned char addr)
{
	////printk( KERN_INFO "Inside ioexpander_initialization\n");
	char data[MIN_BUF_LENGTH];
	g_fd_i2c->addr = addr;
	res[0] = *cmd;
	data[0] = *cmd;
	////printk( KERN_INFO "data[0]=%x \n",data[0]);
	ssp_i2c_write(g_fd_i2c, data, sizeof(data));
}
/*
 * description: 	print a string data on the specified position of the LCD
 * 			(If the line number is 1 and the string is too long to be fit in the first line, the LCD
 * 			 will continue to print the string on the second line)
 *
 * @param lineNumber 	the line number of the LCD where the string is printed. It should be either 1 or 2.
 * 			Otherwise, it is readjusted to 1.
 *
 * @param nthCharacter  the nth character of the line where the string is printed.
 * 			It starts from 0, which indicates the beginning of the line specified.
 */

static void lcd_print(char *msg, unsigned int lineNumber, unsigned int start_position, unsigned char addr)
{
	unsigned int counter = start_position;
	unsigned int lineNum = lineNumber;
	unsigned int startChar = start_position;
	g_fd_i2c->addr = addr;
	if (msg == NULL)
	{
		// printk( KERN_DEBUG "ERR: Empty data for lcd_print_WithPosition \n");
		return;
	}

	if ((lineNum != LCD_FIRST_LINE) && (lineNum != LCD_SECOND_LINE))
	{
		// printk( KERN_DEBUG "ERR: Invalid line number input readjusted to 1 \n");
		lineNum = LCD_FIRST_LINE;
	}

	if (lineNum == LCD_FIRST_LINE)
	{ // printk(KERN_INFO "lineNum == 1");
		lcd_setPosition(LCD_FIRST_LINE, startChar, addr);

		while (*(msg) != '\0')
		{
			if (counter >= NUM_CHARS_PER_LINE)
			{
				lineNum = LCD_SECOND_LINE; // continue writing on the next line if the string is too long
				counter = 0;
				startChar = 0;
				break;
			}
			lcd_send_char(*msg, addr);
			msg++;
			counter++;
		}
	}

	if (lineNum == LCD_SECOND_LINE)
	{ // printk(KERN_INFO "lineNum == 2");
		lcd_setPosition(LCD_SECOND_LINE, startChar, addr);

		while (*(msg) != '\0')
		{
			if (counter >= NUM_CHARS_PER_LINE)
			{
				break;
			}
			lcd_send_char(*msg, addr);
			msg++;
			counter++;
		}
	}
}

/*
 * description:  	 set the cursor to the nth character of the line specified.
 * @param line 		 the line number should be either 1 or 2.
 * @param nthCharacter	 n'th character where the cursor should start on the line specified.
 * 			 It starts from 0, which indicates the beginning of the line.
 */
void lcd_setPosition(unsigned int line, unsigned int start_position, unsigned char addr)
{
	char command;
	////printk( KERN_INFO "*****  startCharacter  %d  *****\n", start_position);
	if (line == LCD_FIRST_LINE)
	{
		command = LCD_FIRST_LINE_ADDRESS + (char)start_position;
		lcd_send_cmd(command, addr);
	}
	else if (line == LCD_SECOND_LINE)
	{
		command = LCD_SECOND_LINE_ADDRESS + (char)start_position;
		lcd_send_cmd(command, addr);
	}
	else
	{
		////printk("ERR: Invalid line number. Select either 1 or 2 \n");
	}
}
static ssize_t i2c_read(struct file *p_file, char __user *buf, size_t len, loff_t *off)
{
	char temp_buf[1];
	int ret = -1;
	g_fd_i2c->addr = 0x24;
	ret = ssp_i2c_read(g_fd_i2c, temp_buf, sizeof(temp_buf));

	////printk(KERN_INFO "ret value is : %d \n", ret);
	////printk(KERN_INFO "temp_buf=%x\n",temp_buf[0]);
	if (ret > 0)
	{
		if (copy_to_user(buf, temp_buf, 1))
			ret = -EFAULT;
	}
	////printk(KERN_INFO " buf=%x\n",*buf);

	////printk(KERN_INFO "klcd Driver: read()\n");
	temp_buf[0] = 0x55;
	////printk(KERN_INFO "temp_buf=0x%x\n",temp_buf[0]);
	copy_to_user(buf, temp_buf, 1);
	////printk(KERN_INFO "buf=0x%x\n",*buf);
	return len;
}

static long i2c_ioctl(struct file *p_file, unsigned int device_type, unsigned long arg)
{
	struct ioctl_lcd_data ioctl_lcd_arguments;
	struct ioctl_io_data ioctl_io_arguments;
	// char  temp_buf[MIN_BUF_LENGTH];
	char temp_buf[1];
	int command;

	if (((const void *)arg) == NULL)
	{
		//	//printk( KERN_DEBUG "ERR: Invalid argument for klcd IOCTL \n");
		return -EINVAL;
	}

	memset(ioctl_lcd_arguments.kbuf_lcd, '\0', sizeof(char) * MAX_BUF_LENGTH);
	memset(ioctl_io_arguments.kbuf_io, '\0', sizeof(char) * MAX_BUF_LENGTH);
	////printk(KERN_INFO "device_type=%u ",device_type);
	switch (device_type)
	{

	case IO_WRITE:
		if (copy_from_user(&ioctl_io_arguments, (const void *)arg, sizeof(ioctl_io_arguments)))
		{
			////printk( KERN_DEBUG "ERR: Failed to copy from user space buffer \n" );
			return -EFAULT;
		}

		// copy_from_user( temp_buf,(const void *)arg,1);

		// printk(KERN_INFO "Wtemp_buf: 0x%x\n",ioctl_io_arguments.kbuf_io);
		//  g_fd_i2c->addr = ioctl_io_arguments.i2c_device_address;

		g_fd_i2c->addr = ioctl_io_arguments.i2c_device_address;
		// printk(KERN_INFO "i2c address before writing %x\n",ioctl_io_arguments.i2c_device_address);
		// ssp_i2c_write(g_fd_i2c,temp_buf,1);
		ssp_i2c_write(g_fd_i2c, ioctl_io_arguments.kbuf_io, 1);
		//printk(KERN_INFO "Wtemp_buf: 0x%x\n",ioctl_io_arguments.kbuf_io[0]);
		break;

	case IO_READ:

		// g_fd_i2c->addr = ioctl_io_arguments.i2c_device_address;
		g_fd_i2c->addr = 0x20;
		//g_fd_i2c->addr = 0x38;
		// printk(KERN_INFO "I2C Address : 0x%x\n",g_fd_i2c->addr);
		ssp_i2c_read(g_fd_i2c, temp_buf, sizeof(temp_buf));
		
		 copy_to_user( (const void *)arg,temp_buf,1);
		 //printk(KERN_INFO "Wtemp_buf: 0x%x\n",ioctl_io_arguments.kbuf_io[0]);
		// copy_to_user((char *)arg,temp_buf,sizeof(temp_buf));

		break;

	case LCD:
		if (copy_from_user(&ioctl_lcd_arguments, (const void *)arg, sizeof(ioctl_lcd_arguments)))
		{
			////printk( KERN_DEBUG "ERR: Failed to copy from user space buffer \n" );
			return -EFAULT;
		}
		command = ioctl_lcd_arguments.command;

		break;

	default:
		////printk(KERN_DEBUG "klcd Driver (ioctl): No such command \n");
		return -ENOTTY;
	}
	////printk(KERN_INFO "command: %d\n",command);

	switch ((char)command)
	{
	case IOCTL_CLEAR_DISPLAY:
		lcd_clear_display(ioctl_lcd_arguments.i2c_device_address);
		break;

	case IOCTL_PRINT:
		lcd_print(ioctl_lcd_arguments.kbuf_lcd, ioctl_lcd_arguments.lineNumber, ioctl_lcd_arguments.start_position, ioctl_lcd_arguments.i2c_device_address);
		break;

	case IOCTL_LCD_INITIALIZATION:
		lcd_initialize(ioctl_lcd_arguments.kbuf_lcd, ioctl_lcd_arguments.i2c_device_address);
		break;

	case IOCTL_IO_INITIALIZATION:
		ioexpander_initialization(ioctl_io_arguments.kbuf_io, ioctl_io_arguments.i2c_device_address);
		break;

	case IOCTL_SET_BIT_IO_EXPANDER:
		////printk("******Inside IOCTL_SET_BIT_IO_EXPANDER*******\n");
		send_data_to_ioexpander(ioctl_io_arguments.kbuf_io, ioctl_io_arguments.pin_no, ioctl_io_arguments.pin_status, ioctl_io_arguments.i2c_device_address);
		break;

	case IOCTL_GET_BIT_IO_EXPANDER:
		////printk("******Inside IOCTL_GET_BIT_IO_EXPANDER*******\n");
		g_fd_i2c->addr = 0x24;
		ssp_i2c_read(g_fd_i2c, temp_buf, sizeof(temp_buf));
		copy_to_user((char *)arg, temp_buf, sizeof(temp_buf));
		////printk(KERN_INFO "temp_buf: %x\n",temp_buf[0]);
		////printk(KERN_INFO "arg: %c\n",(char *)arg);
		break;

	default:
		////printk(KERN_DEBUG "klcd Driver (ioctl): No such command \n");
		return -ENOTTY;
	}

	return 0;
}

static ssize_t i2c_write(struct file *p_file, const char __user *buf, size_t len, loff_t *off)
{
	char kbuf[1] = {0x48};
	unsigned long copyLength;

	if (buf == NULL)
	{
		////printk( KERN_DEBUG "ERR: Empty user space buffer \n" );
		return -ENOMEM;
	}

	// Copy user space buffer to kernel space buffer
	if (copy_from_user(kbuf, buf, len))
	{
		// printk( KERN_DEBUG "ERR: Failed to copy from user space buffer \n" );
		return -EFAULT;
	}

	// printk( KERN_INFO "***** value copied from user space:  0x%x  *****\n", kbuf[0]);

	// printk( KERN_INFO "***** copyLength:  %lu *****\n", copyLength );

	g_fd_i2c->addr = 0x24;

	ssp_i2c_write(g_fd_i2c, kbuf, sizeof(kbuf));
	// printk(KERN_INFO "klcd lcd_setup 2\n");
	// printk(KERN_INFO "klcd Driver: write()\n");

	return len;
}

static struct proc_dir_entry *proc_entry;

int __init lcd_probe(struct platform_device *op)
{
	// printk(KERN_INFO "klcd lcd_probe\n");

	return 0;
}

static int lcd_remove(struct platform_device *op)
{
	// printk(KERN_INFO "klcd lcd_remove\n");
	if (NULL != g_fd_i2c)
	{
		ssp_i2c_close(g_fd_i2c);
		g_fd_i2c = NULL;
	}
	return 0;
}

/* file operation structure */
static struct file_operations klcd_fops =
	{
		.owner = THIS_MODULE,
		.open = i2c_interface_open,
		.release = i2c_interface_close,
		.read = i2c_read,
		.write = i2c_write,
		.unlocked_ioctl = i2c_ioctl};

static struct platform_driver lcd_driver = {
	.driver = {
		.name = "char_lcd_io",
		.owner = THIS_MODULE,
	},
	.probe = lcd_probe,
	.remove = lcd_remove,
};

static struct platform_device lcd_device = {
	.name = "char_lcd_io",
	.id = 0,
	.dev =
		{
			.release = lcd_platform_release,
		}};

static int lcd_io_init(void)
{
	// printk(KERN_INFO "Inside lcd_init\n");

	struct device *dev_ret;
	int ret = 0;

	// dynamically allocate device major number
	if (alloc_chrdev_region(&dev_number, MINOR_NUM_START, MINOR_NUM_COUNT, DEVICE_NAME) < 0)
	{
		// printk( KERN_DEBUG "ERR: Failed to allocate major number \n" );
		return -1;
	}

	// create a class structure
	klcd_io_class = class_create(THIS_MODULE, CLASS_NAME);

	if (IS_ERR(klcd_io_class))
	{
		unregister_chrdev_region(dev_number, MINOR_NUM_COUNT);
		// printk(KERN_DEBUG "ERR: Failed to create class structure \n");

		return PTR_ERR(klcd_io_class);
	}

	// create a device and registers it with sysfs
	dev_ret = device_create(klcd_io_class, NULL, dev_number, NULL, DEVICE_NAME);

	if (IS_ERR(dev_ret))
	{
		class_destroy(klcd_io_class);
		unregister_chrdev_region(dev_number, MINOR_NUM_COUNT);
		// printk( KERN_DEBUG "ERR: Failed to create device structure \n" );

		return PTR_ERR(dev_ret);
	}

	// initialize a cdev structure
	cdev_init(&klcd_io_cdev, &klcd_fops);

	// add a character device to the system
	if (cdev_add(&klcd_io_cdev, dev_number, MINOR_NUM_COUNT) < 0)
	{
		device_destroy(klcd_io_class, dev_number);
		class_destroy(klcd_io_class);
		unregister_chrdev_region(dev_number, MINOR_NUM_COUNT);
		// printk(KERN_DEBUG "ERR: Failed to add cdev \n");

		return -1;
	}

	// printk("LCD: Init\n");

	lcd_driver.driver.bus = platform_bus_type_ptr;
	ret = platform_driver_register(&lcd_driver);

	if (!ret)
	{
		ret = platform_device_register(&lcd_device);
		if (ret)
			platform_driver_unregister(&lcd_driver);
	}

	return ret;
}

static void lcd_io_exit(void)
{
	// turn off LCD display
	lcd_clear_display(g_fd_i2c->addr);

	// remove a cdev from the system
	cdev_del(&klcd_io_cdev);

	// remove device
	device_destroy(klcd_io_class, dev_number);

	// destroy class
	class_destroy(klcd_io_class);

	// deallocate device major number
	unregister_chrdev_region(MAJOR(dev_number), MINOR_NUM_COUNT);
	platform_device_unregister(&lcd_device);
	platform_driver_unregister(&lcd_driver);

	// printk(KERN_INFO "klcd Driver Exited. \n");
}

module_init(lcd_io_init);

module_exit(lcd_io_exit);

MODULE_AUTHOR("Alan Wang <alan@wrcode.com>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple Lcd Module");
MODULE_ALIAS("A simple module");
