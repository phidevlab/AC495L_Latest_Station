/**
 * @file   gpiodrv.c
 * @author Derek Molloy
 * @date   7 April 2015
 * @version 0.1
 * @brief   An introductory character driver to support the second article of my series on
 * Linux loadable kernel module (LKM) development. This module maps to /dev/ebbchar and
 * comes with a helper C program that can be run in Linux user space to communicate with
 * this the LKM.
 * @see http://www.derekmolloy.ie/ for a full description and follow-up descriptions.
 */

#include <linux/init.h>           // Macros used to mark up functions e.g. __init __exit
#include <linux/module.h>         // Core header for loading LKMs into the kernel
#include <linux/device.h>         // Header to support the kernel Driver Model
#include <linux/kernel.h>         // Contains types, macros, functions for the kernel
#include <linux/fs.h>             // Header for the Linux file system support
#include <asm/uaccess.h>          // Required for the copy to user function

#include <pal.h> //include file for audiocodes GPIO APIs
#include <pal_vlynq.h>
#include <pal_vlynqIoctl.h>



#define  DEVICE_NAME "phiGPIO"    ///< The device will appear at /dev/ebbchar using this value
#define  CLASS_NAME  "phiAudiocom"        ///< The device class -- this is a character device driver

MODULE_LICENSE("GPL");            ///< The license type -- this affects available functionality
MODULE_AUTHOR("AJ");              ///< The author -- visible when you use modinfo
MODULE_DESCRIPTION("A simple Linux char driver for the Audiocodes SoC");  ///< The description -- see modinfo
MODULE_VERSION("0.1");            ///< A version number to inform users

static int    majorNumber;                  ///< Stores the device number -- determined automatically
static char   message[256] = {0};           ///< Memory for the string that is passed from userspace
static short  size_of_message;              ///< Used to remember the size of the string stored
static int    numberOpens = 0;              ///< Counts the number of times the device is opened
static struct class*  charClass  = NULL; ///< The device-driver class struct pointer
static struct device* charDevice = NULL; ///< The device-driver device struct pointer

// The prototype functions for the character driver -- must come before the struct definition
static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

// phi functions to manipulate GPIOs
static void PHI_gpioInit();
static void PHI_setGPIO(unsigned int gpio_pin, int value);
static int  PHI_getGPIO(unsigned int gpio_pin);
static void PHI_sysGpioFuncSet(int pinSelReg,   int shift,     int func);


/** @brief Devices are represented as file structure in the kernel. The file_operations structure from
 *  /linux/fs.h lists the callback functions that you wish to associated with your file operations
 *  using a C99 syntax structure. char devices usually implement open, read, write and release calls
 */
static struct file_operations fops =
{
   .open = dev_open,
   .read = dev_read,
   .write = dev_write,
   .release = dev_release,
};

//changes for GPIO
static struct gpio_read_operations{
   unsigned int gpio_no;        // GPIO No.
   unsigned int gpio_command;   // GPIO Command - 0- get Data and 1- set Data.
   unsigned int gpio_value;	// GPIO value.
}gpio_read_ops;

static struct gpio_write_operations{
   unsigned int gpio_no;        // GPIO No.
   unsigned int gpio_command;   // GPIO Command - 0- get Data and 1- set Data.
   unsigned int gpio_value;	// GPIO value.
}gpio_write_ops;

/** @brief The LKM initialization function
 *  The static keyword restricts the visibility of the function to within this C file. The __init
 *  macro means that for a built-in driver (not a LKM) the function is only used at initialization
 *  time and that it can be discarded and its memory freed up after that point.
 *  @return returns 0 if successful
 */
static int __init gpiochar_init(void){
   printk(KERN_INFO "GPIOChar: Initializing the GPIOChar LKM\n");

   // Try to dynamically allocate a major number for the device -- more difficult but worth it
   majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
   if (majorNumber<0){
      printk(KERN_ALERT "GPIOChar failed to register a major number\n");
      return majorNumber;
   }
   printk(KERN_INFO "GPIOChar: registered correctly with major number %d\n", majorNumber);

   // Register the device class
   charClass = class_create(THIS_MODULE, CLASS_NAME);
   if (IS_ERR(charClass)){                // Check for error and clean up if there is
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to register device class\n");
      return PTR_ERR(charClass);          // Correct way to return an error on a pointer
   }
   printk(KERN_INFO "GPIOChar: device class registered correctly\n");

   // Register the device driver
   charDevice = device_create(charClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
   if (IS_ERR(charDevice)){               // Clean up if there is an error
      class_destroy(charClass);           // Repeated code but the alternative is goto statements
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to create the device\n");
      return PTR_ERR(charDevice);
   }
   printk(KERN_INFO "GPIOChar: device class created correctly\n"); // Made it! device was initialized
   memset(&gpio_read_ops,0,sizeof(gpio_read_ops));
   memset(&gpio_write_ops,0,sizeof(gpio_write_ops));
   PHI_gpioInit();
   printk(KERN_INFO "GPIOChar: GPIOs are initialized....\n"); // Made it! GPIO was initialized
   return 0;
}

/** @brief The LKM cleanup function
 *  Similar to the initialization function, it is static. The __exit macro notifies that if this
 *  code is used for a built-in driver (not a LKM) that this function is not required.
 */
static void __exit gpiochar_exit(void){
   PHI_gpioInit();
   device_destroy(charClass, MKDEV(majorNumber, 0));     // remove the device
   class_unregister(charClass);                          // unregister the device class
   class_destroy(charClass);                             // remove the device class
   unregister_chrdev(majorNumber, DEVICE_NAME);             // unregister the major number
   printk(KERN_INFO "GPIOChar: Goodbye from the LKM!\n");
}

/** @brief The device open function that is called each time the device is opened
 *  This will only increment the numberOpens counter in this case.
 *  @param inodep A pointer to an inode object (defined in linux/fs.h)
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 */
static int dev_open(struct inode *inodep, struct file *filep){
   numberOpens++;
   printk(KERN_INFO "GPIOChar: Device has been opened %d time(s)\n", numberOpens);
   return 0;
}

/** @brief This function is called whenever device is being read from user space i.e. data is
 *  being sent from the device to the user. In this case is uses the copy_to_user() function to
 *  send the buffer string to the user and captures any errors.
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 *  @param buffer The pointer to the buffer to which this function writes the data
 *  @param len The length of the b
 *  @param offset The offset if required
 */
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
   int error_count = 0;
   // copy_to_user has the format ( * to, *from, size) and returns 0 on success
  // error_count = copy_to_user(buffer, message, size_of_message);

    memset(&gpio_read_ops,0,sizeof(gpio_read_ops));
    memcpy(&gpio_read_ops, buffer, sizeof(gpio_read_ops));

  // printk(KERN_INFO "GPIO no. is: %d\n",      gpio_ops.gpio_no);
  // printk(KERN_INFO "GPIO command. is: %d\n", gpio_ops.gpio_command);
   //printk(KERN_INFO "GPIO value is: %d\n",    gpio_ops.gpio_value);

   switch(gpio_read_ops.gpio_command){
   	case 0: // get the gpio value
	        gpio_read_ops.gpio_value  = PHI_getGPIO(gpio_read_ops.gpio_no);
	        printk("gpio_read_ops.gpio_value=%d",gpio_read_ops.gpio_value);
	       // printk("read gpio no=%d  value =%d\n",gpio_read_ops.gpio_no, gpio_read_ops.gpio_value);
            memcpy(buffer, &gpio_read_ops, sizeof(gpio_read_ops));
                /*error_count = copy_to_user(buffer, message, size_of_message);
                if (error_count==0){            // if true then have success
      			//printk(KERN_INFO "EBBChar: Sent %d characters to the user\n", size_of_message);
      			return (size_of_message=0);  // clear the position to the start and return 0*
   		}
   		else {
      			printk(KERN_INFO "EBBChar: Failed to send %d characters to the user\n", error_count);
      			return -EFAULT;              // Failed -- return a bad address message (i.e. -14)
   		}*/

		break;

	case 1: // set the gpio value
		//PHI_setGPIO(gpio_ops.gpio_no, gpio_ops.gpio_value); // value 0- low and 1- high
		break;
   }
   return sizeof(gpio_read_ops);
}

/** @brief This function is called whenever the device is being written to from user space i.e.
 *  data is sent to the device from the user. The data is copied to the message[] array in this
 *  LKM using the sprintf() function along with the length of the string.
 *  @param filep A pointer to a file object
 *  @param buffer The buffer to that contains the string to write to the device
 *  @param len The length of the array of data that is being passed in the const char buffer
 *  @param offset The offset if required
 */
static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){


   memset(&gpio_write_ops,0,sizeof(gpio_write_ops));
   memcpy(&gpio_write_ops, buffer, sizeof(gpio_write_ops));

   //printk(KERN_INFO "GPIO no. is: %d\n",      gpio_ops.gpio_no);
   //printk(KERN_INFO "GPIO command. is: %d\n", gpio_ops.gpio_command);
   //printk(KERN_INFO "GPIO value is: %d\n",    gpio_ops.gpio_value);

   switch(gpio_write_ops.gpio_command){
   	case 0: // get the gpio value
	       /* gpio_ops.gpio_value  = PHI_getGPIO(gpio_ops.gpio_no);
		memcpy(buffer, &gpio_ops, sizeof(gpio_ops));
                error_count = copy_to_user(buffer, message, size_of_message);*/

		break;

	case 1: // set the gpio value
		PHI_setGPIO(gpio_write_ops.gpio_no, gpio_write_ops.gpio_value); // value 0- low and 1- high
		break;
   }
   return len;
}

/** @brief The device release function that is called whenever the device is closed/released by
 *  the userspace program
 *  @param inodep A pointer to an inode object (defined in linux/fs.h)
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 */
static int dev_release(struct inode *inodep, struct file *filep){
   printk(KERN_INFO "EBBChar: Device successfully closed\n");
   return 0;
}

/** @brief A module must use the module_init() module_exit() macros from linux/init.h, which
 *  identify the initialization function at insertion time and the cleanup function (as
 *  listed above)
 */


static void PHI_sysGpioFuncSet(int pinSelReg,   int shift,     int func)
{
	unsigned int temp;
	unsigned int addr;
#if defined(CONFIG_ORCHID)||defined(CONFIG_ORCHID_ATA)
	addr = ARIES_IOMUX_PIN_SEL_1 + (4 * (pinSelReg-1));
#else
	addr = TITAN_IOMUX_PIN_SEL_1 + (4 * (pinSelReg-1));
#endif
	temp = *(volatile unsigned int *)addr;
	temp &= ~(0x3 << shift);
	temp |= (func << shift);
	*(volatile unsigned int *)addr = temp;
}


static void PHI_gpioInit(){


   /*PHI_sysGpioFuncSet(8, 30, 2);  PAL_sysGpioCtrl(0, GPIO_PIN, GPIO_INPUT_PIN);
   PHI_sysGpioFuncSet(9, 0, 2);   PAL_sysGpioCtrl(1, GPIO_PIN, GPIO_INPUT_PIN);
   PHI_sysGpioFuncSet(9, 2, 2);   PAL_sysGpioCtrl(2, GPIO_PIN, GPIO_OUTPUT_PIN);
   PHI_sysGpioFuncSet(9, 4, 2);   PAL_sysGpioCtrl(3, GPIO_PIN, GPIO_OUTPUT_PIN);
   PHI_sysGpioFuncSet(9, 6, 2);   PAL_sysGpioCtrl(4, GPIO_PIN, GPIO_OUTPUT_PIN);
   PHI_sysGpioFuncSet(9, 14, 2);  PAL_sysGpioCtrl(5, GPIO_PIN, GPIO_OUTPUT_PIN);
   PHI_sysGpioFuncSet(9, 12, 2);  PAL_sysGpioCtrl(6, GPIO_PIN, GPIO_OUTPUT_PIN);
   PHI_sysGpioFuncSet(9, 10, 2);  PAL_sysGpioCtrl(7, GPIO_PIN, GPIO_OUTPUT_PIN);
   PHI_sysGpioFuncSet(9, 8, 2);   PAL_sysGpioCtrl(8, GPIO_PIN, GPIO_OUTPUT_PIN);
   PHI_sysGpioFuncSet(10, 22, 2); PAL_sysGpioCtrl(27, GPIO_PIN, GPIO_OUTPUT_PIN);
   PHI_sysGpioFuncSet(8, 22, 2);  PAL_sysGpioCtrl(29, GPIO_PIN, GPIO_INPUT_PIN);
   PHI_sysGpioFuncSet(14, 20, 2); PAL_sysGpioCtrl(31, GPIO_PIN, GPIO_INPUT_PIN);
   PHI_sysGpioFuncSet(14, 22, 2); PAL_sysGpioCtrl(32, GPIO_PIN, GPIO_INPUT_PIN);
   PHI_sysGpioFuncSet(14, 24, 2); PAL_sysGpioCtrl(33, GPIO_PIN, GPIO_OUTPUT_PIN);*/
  // PHI_sysGpioFuncSet(9, 10, 2); PAL_sysGpioCtrl(7, GPIO_PIN, GPIO_OUTPUT_PIN);
   //PHI_sysGpioFuncSet(14, 20, 2); PAL_sysGpioCtrl(31, GPIO_PIN, GPIO_OUTPUT_PIN);
 // PHI_sysGpioFuncSet(15, 21, 2);  PAL_sysGpioCtrl(47, GPIO_PIN, GPIO_OUTPUT_PIN);
   // PHI_sysGpioFuncSet(10, 24, 2);  PAL_sysGpioCtrl(28, GPIO_PIN, GPIO_OUTPUT_PIN);

   //PHI_sysGpioFuncSet(9, 26, 2);  PAL_sysGpioCtrl(14, GPIO_PIN, GPIO_OUTPUT_PIN);

   /*PAL_sysGpioOutBit(2, 0);
   PAL_sysGpioOutBit(3, 0);
   PAL_sysGpioOutBit(4, 1);
   PAL_sysGpioOutBit(5, 0);
   PAL_sysGpioOutBit(6, 1);
   PAL_sysGpioOutBit(7, 0);
   PAL_sysGpioOutBit(8, 0);
   PAL_sysGpioOutBit(27, 0);*/
   // PAL_sysGpioOutBit(7, 0);
    //PAL_sysGpioOutBit(31, 0);
    //PAL_sysGpioOutBit(47, 0);

//PAL_sysGpioOutBit(28, 0);
PHI_sysGpioFuncSet(10, 24, 2);  PAL_sysGpioCtrl(28, GPIO_PIN, GPIO_OUTPUT_PIN);
PHI_sysGpioFuncSet(8, 30, 2);  PAL_sysGpioCtrl(0, GPIO_PIN, GPIO_INPUT_PIN);
PAL_sysGpioOutBit(28, 0);
}

static void PHI_setGPIO(unsigned int gpio_pin, int value){
   PAL_sysGpioOutBit(gpio_pin, value);
}

static int PHI_getGPIO(unsigned int gpio_pin){
   return PAL_sysGpioInBit(gpio_pin);
}


module_init(gpiochar_init);
module_exit(gpiochar_exit);
