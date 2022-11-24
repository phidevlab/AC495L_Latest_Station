 /*******************************************************************************   
  * FILE PURPOSE:    AudioCodes IPP Init LCD module
  *******************************************************************************   
  * FILE NAME:       blf_lcd.c
  *                                                                                 
  * DESCRIPTION:  Init BLF LCD
  *                                                                                 
  * REVISION HISTORY:  
  *   
  * Date           Description                               Author
  *-----------------------------------------------------------------------------
  * 26 Oct 2012    Initial Creation                          Kenny Xuan
  * 
  *                                                                                 
  * (C) Copyright 2012, AudioCodes, Inc
  ******************************************************************************/  
#include <linux/config.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/tty.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/fb.h>
#include <linux/init.h>

#include <asm/io.h>
#include <asm/irq.h>
#include <asm/uaccess.h>
#include <asm/page.h>
#include <linux/proc_fs.h>

#include <linux/fb.h>
#include <linux/vmalloc.h>
#include <linux/version.h>   
#include <linux/kthread.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
#include <linux/moduleparam.h>                
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#endif

//#include "board_param.h"
#include <pal.h>
#include "st7567_lcd.h"
#include "lcd_param.h"
#include "lcd_regs.h"

extern SSP_HAL_SPI_INFO_T* ssp_spi_open(void);
extern int ssp_spi_write_read(SSP_HAL_SPI_INFO_T  *info, u8 *write_buf,
                       u32 write_len, u8 *read_buf, u32 read_len);
extern int ssp_spi_close(SSP_HAL_SPI_INFO_T  *info);

extern SSP_HAL_I2C_INFO_T* ssp_i2c_open( void );
static SSP_HAL_I2C_INFO_T * g_fd_i2c = NULL;

extern int ssp_i2c_write( SSP_HAL_I2C_INFO_T  *info, UINT8    *buffer, UINT32   len);
extern int ssp_dev_init(void);

struct lcd_par
{
/*
*  The hardware specific data in this structure uniquely defines a video
*  mode.
*
*  If your hardware supports only one video mode, you can leave it empty.
*/
    char *screen_base;
    unsigned long screen_size;  /*memory size*/
};

#define BPP   1
#define XRES 132
#define YRES 64
//#define SCREEN_SIZE (( XRES * (YRES + (YRES % 8)) * BPP ) / 8)
#define LINE_SIZE  ((XRES+7)/8)
#define SCREEN_SIZE  (YRES*LINE_SIZE)


static SSP_HAL_SPI_INFO_T * fd_spi = NULL;
static DEFINE_SEMAPHORE(lcd_sysc);

int lcd_init(void);
int lcd_setup(void);

static int lcd_blank(int blank_mode, struct fb_info *info);

static int
lcd_ioctl(struct fb_info *info,
		       unsigned int cmd, 
			   unsigned long arg);

static int
lcd_mmap(struct fb_info *info, 
       struct vm_area_struct *vma);

static int writeFB = 0;
static volatile int write_task_finish = 0;

//static DEFINE_SEMAPHORE(thread_sem);
static DECLARE_COMPLETION(thread_sem);
static DECLARE_WAIT_QUEUE_HEAD(thr_wq);

static struct fb_ops lcd_ops = {
    .owner       =THIS_MODULE,
    //.fb_open     =lcd_open,    /* only if you need it to do something */
    //.fb_release  =lcd_release, /* only if you need it to do something */
    //  fb_get_fix: lcd_get_fix,
    //  fb_get_var: lcd_get_var,
    //  fb_set_var: lcd_set_var,
    //  fb_get_cmap:  fbgen_get_cmap,
    //  fb_set_cmap:  fbgen_set_cmap,
    //  fb_pan_display: fbgen_pan_display,
    .fb_blank    =lcd_blank,
    .fb_ioctl    =lcd_ioctl,   /* optional */
    .fb_mmap     =lcd_mmap,
};

/* This macro is for 405 LCD Contrast test only. When the test is no needed,
 * this macro should be commented out */
//#define DEBUG_LCD_CONTRAST_IN_PROC 

#ifdef DEBUG_LCD_CONTRAST_IN_PROC
#define MAX_SETTING_RECORDS       128 //PAGE_SIZE
static struct proc_dir_entry *proc_entry;
static unsigned char setting_records[MAX_SETTING_RECORDS+4] = {0};  // make Space for lcdcontrast records
static int next_lcdcontrast = 0;;  
static int overlap_times = 0;  

int lcdcontrast_read( char *page, char **start, off_t off,
                             int count, int *eof, void *data )
{
	int len=0;
	int i;

	len = sprintf(page, "print num:%d,  overlap_times:%d\n",next_lcdcontrast, overlap_times);
	for (i=0; i<next_lcdcontrast; i++)
		len += sprintf(page+len, "%02x,", setting_records[i]);
	len += sprintf(page+len, " print end\n");
	return len;
}

ssize_t lcdcontrast_write( struct file *filp, const char __user *buff,
                                  unsigned long len, void *data )
{
	printk("clear the lcdcontrast Setting-Records \n");
	memset( setting_records, 0, MAX_SETTING_RECORDS );
	next_lcdcontrast = 0;
	return len;
}

#endif
/* ------------------- chipset specific functions -------------------------- */
/** 
 * set gpio to input pin or output pin; 
 */
int gpio_set_direction(unsigned char pin, BOOL dir)
{
	PAL_sysGpioCtrl(pin, GPIO_PIN, dir);

	return 0;
}

/** 
 * set ouput pin to a state; 
 */
int gpio_set_output(unsigned char pin, BOOL status)
{
	PAL_sysGpioOutBit(pin, status);

	return 0;
}

/** 
 * get iuput pin state; 
 */
BOOL gpio_get_input(unsigned char pin)
{
	return (BOOL)PAL_sysGpioInBit(pin);
}

void lcd_spi_a0_cmd(void)
{
	/*a0 = L ,CMD*/
	gpio_set_output(LCD_A0,LOW);
}
void lcd_spi_a0_data(void)
{
	/*A0 = H,DATA*/
	gpio_set_output(LCD_A0,HIGH);
}
void lcd_spi_rstb_activate(void)
{
	gpio_set_output(LCD_RST,LOW);
}
void lcd_spi_rstb_deactivate(void)
{
	gpio_set_output(LCD_RST,HIGH);
}


static int lcd_blank(int blank_mode, struct fb_info *info)
{
/*
*  Blank the screen if blank_mode != 0, else unblank. If blank == NULL
*  then the caller blanks by setting the CLUT (Color Look Up Table) to all
*  black. Return 0 if blanking succeeded, != 0 if un-/blanking failed due
*  to e.g. a video mode which doesn't support it. Implements VESA suspend
*  and powerdown modes on hardware that supports disabling hsync/vsync:
*    blank_mode == 2: suspend vsync
*    blank_mode == 3: suspend hsync
*    blank_mode == 4: powerdown
*/
    if(info)
    {
        struct lcd_par *par = info->par;
        memset(par->screen_base, 0, par->screen_size);
    }

    return 0;
}

/*For ESD test*/
void mon_lcd(void)
{
#if 0
    unsigned char status[2];

    st7567_status(fd_i2c,status,2);

    if(status[0] != 0x10)
    {
        printk("top_status: 0x%x 0x%x\n",status[0],status[1]);
        printk("LCD doesn't work, reinitialize it.\n");

        /*reset un1604c*/
        //st7567_contral(fd_i2c,0xe2);
        lcd_setup();
    }

#endif
}

int inline get_bit(unsigned char *data, int offset)
{
	int i,j;
	unsigned char tmp, bit, mask;

	i = offset/8;
	j = offset%8;
	
	tmp = data[i];
	mask = (1<<(7-j));
	bit = tmp&mask;		
	return bit?1:0;
}

#if 1
int st7567_do_frame_buffer(unsigned char *ptr)
{
#define PAGE_SIZE (LINE_SIZE*8)
	int page;
	static unsigned char ss[SCREEN_SIZE]={0}; /*saved screen*/
	static unsigned char init_flag = 1;
	unsigned char *p_new = ptr;
	unsigned char *p_old = &ss[0];
	int j;
	unsigned char tmp[XRES];
	
	/*Do frame buffer*/
	for(page=0xb0;page<0xb8;page++)
	{
		if(memcmp(p_new, p_old, PAGE_SIZE)||init_flag)
		{	/*set page address*/
			for(j=0; j<XRES; j++)
			{				
				tmp[j] = (get_bit(p_new+0*LINE_SIZE, j))
					    |(get_bit(p_new+1*LINE_SIZE, j)<<1)
					    |(get_bit(p_new+2*LINE_SIZE, j)<<2)
					    |(get_bit(p_new+3*LINE_SIZE, j)<<3)
					    |(get_bit(p_new+4*LINE_SIZE, j)<<4)
					    |(get_bit(p_new+5*LINE_SIZE, j)<<5)
					    |(get_bit(p_new+6*LINE_SIZE, j)<<6)
					    |(get_bit(p_new+7*LINE_SIZE, j)<<7);				
			}			
		    st7567_contral(fd_spi,page);
			/*set column address*/
		    st7567_contral(fd_spi,0x10);
		    st7567_contral(fd_spi,0x00);
			st7567_write_data(fd_spi, tmp, XRES);

			memcpy(p_old, p_new, PAGE_SIZE);
		}		
	    p_new += PAGE_SIZE;
		p_old += PAGE_SIZE;
	}
	init_flag = 0;
}
#else
int st7567_do_frame_buffer(unsigned char *ptr)
{
#define PAGE_SIZE (LINE_SIZE*8)
	int page;
	static unsigned char ss[SCREEN_SIZE]={0}; /*saved screen*/
	static unsigned char init_flag = 1;
	unsigned char *p_new = ptr;
	unsigned char *p_old = &ss[0];
	int j;
	unsigned char tmp[XRES];
	
	/*Do frame buffer*/
	for(page=0xb7;page>=0xb0;page--)
	{
		if(memcmp(p_new, p_old, PAGE_SIZE)||init_flag)
		{	/*set page address*/
			for(j=0; j<XRES; j++)
			{				
				tmp[j] = (get_bit(p_new+0*LINE_SIZE, j)<<7)
					    |(get_bit(p_new+1*LINE_SIZE, j)<<6)
					    |(get_bit(p_new+2*LINE_SIZE, j)<<5)
					    |(get_bit(p_new+3*LINE_SIZE, j)<<4)
					    |(get_bit(p_new+4*LINE_SIZE, j)<<3)
					    |(get_bit(p_new+5*LINE_SIZE, j)<<2)
					    |(get_bit(p_new+6*LINE_SIZE, j)<<1)
					    |(get_bit(p_new+7*LINE_SIZE, j));				
			}			
		    st7567_contral(fd_spi,page);
			/*set column address*/
		    st7567_contral(fd_spi,0x10);
		    st7567_contral(fd_spi,0x00);
			st7567_write_data(fd_spi, tmp, XRES);

			memcpy(p_old, p_new, PAGE_SIZE);
		}		
	    p_new += PAGE_SIZE;
		p_old += PAGE_SIZE;
	}
	init_flag = 0;
}

#endif
static int write_queue_task(void *data)
{
    struct lcd_par *par = (struct lcd_par *)data;
    int timeout = 20;	/*20 original*/
    struct task_struct *tsk = current;
    static int mon_lcd_times = 0;
    unsigned char* ptr = par->screen_base;

    DECLARE_WAITQUEUE(wait, tsk);
//    daemonize("lcd");

    add_wait_queue(&thr_wq, &wait);
    while(1)
    {
        down_interruptible(&lcd_sysc);

        /*Mon lcd every 1 second*/
        if(mon_lcd_times < 5)
        {
            mon_lcd_times++;			
        }
        else
        {
            mon_lcd();
            mon_lcd_times = 0;
			
        }

		st7567_do_frame_buffer(ptr);

        up(&lcd_sysc);
        interruptible_sleep_on_timeout(&thr_wq, timeout);
        mb();
    }
    remove_wait_queue(&thr_wq, &wait);

    /* Tell people we have exitd */
    //up(&thread_sem);
	complete(&thread_sem);
	
    return 0;
}


/*
 * Here we define the default structs fb_fix_screeninfo and fb_var_screeninfo
 * if we don't use modedb. If we do use modedb see tifb_init how to use it
 * to get a fb_var_screeninfo. Otherwise define a default var as well. 
 */
/* Variable Screen Information */
static struct fb_var_screeninfo lcd_var =
{
    .xoffset = 0,
    .yoffset = 0,
    .transp = {0, 0, 0},
    .nonstd = 0,
    .activate = 0,
    .height = -1,
    .width = -1,
    .accel_flags = 0,
    /*
    left_margin:  LEFT_MARGIN,
    right_margin: RIGHT_MARGIN,
    upper_margin: UPPER_MARGIN,
    lower_margin: LOWER_MARGIN,
    */
    .sync = 0,
    .vmode = FB_VMODE_NONINTERLACED
};

static struct fb_fix_screeninfo lcd_fix  =
{
    .id =           "LCD FB Driver",
    .type =         FB_TYPE_PACKED_PIXELS,
    .type_aux =     0,
    .visual =       FB_VISUAL_MONO01,
    .xpanstep =     1,
    .ypanstep =     1,
    .ywrapstep =    1, 
    .accel =        FB_ACCEL_NONE,
};

int __init lcd_probe(struct platform_device *op)
{
    struct platform_device *dev = op;
    struct fb_info *lcd_info;
    struct lcd_par *default_par;

    lcd_info = framebuffer_alloc(sizeof(struct lcd_par), &dev->dev);
    if(!lcd_info)
    {
        printk("LCD: Memory allocation failed for fb_info\n");
        return -ENOMEM;
    }

    default_par = lcd_info->par;
    default_par->screen_size = SCREEN_SIZE;
    default_par->screen_base = (char *)__get_free_page(GFP_KERNEL);
    if(default_par->screen_base == NULL)
    {
        printk(KERN_ALERT "LCD: Unable to allocate Memory for Frame Buffer");
        framebuffer_release(lcd_info);
        return -EINVAL;
    }

    printk("LCD: Screen base 0x%p\n", default_par->screen_base);

    memset((void *)default_par->screen_base, 0, PAGE_SIZE);

    SetPageReserved(virt_to_page(default_par->screen_base));


    lcd_fix.smem_start  = (unsigned long)virt_to_phys(default_par->screen_base); /*Physical address*/
    lcd_fix.smem_len    = default_par->screen_size;
	lcd_fix.line_length   = LINE_SIZE;

    lcd_var.xres = XRES;
    lcd_var.xres_virtual = XRES;

    lcd_var.yres = YRES;
    lcd_var.yres_virtual = YRES;

    lcd_var.grayscale = 0;
    lcd_var.bits_per_pixel = BPP;

    /* Initialize fbinfo */
    lcd_info->screen_base        = (char *)virt_to_phys(default_par->screen_base); /*Physical address*/
    lcd_info->flags              = FBINFO_FLAG_DEFAULT;
    lcd_info->fix                = lcd_fix;
    lcd_info->fbops              = &lcd_ops;
    lcd_info->var                = lcd_var;

    /* initialize var_screeninfo */
    lcd_var.activate = FB_ACTIVATE_FORCE;
    fb_set_var(lcd_info, &lcd_var);

    /* Register the Frame Buffer  */
    if(register_framebuffer(lcd_info) < 0)
    {
        printk(KERN_ALERT "LCD: Frame Buffer Registration Failed!\n");
        framebuffer_release(lcd_info);
        ClearPageReserved(virt_to_page(default_par->screen_base));
        free_page((int)default_par->screen_base);
        return -EINVAL;
    }

    printk("LCD: Registered the Frame Buffer successfully\n");

    dev_set_drvdata(&dev->dev, lcd_info);

    if(lcd_setup() < 0)
    {
        return -ENOMEM;
    }

	lcd_blank(0, lcd_info);
    init_waitqueue_head(&thr_wq);
    //kernel_thread(write_queue_task, default_par, CLONE_FS | CLONE_FILES | CLONE_SIGHAND);
    kthread_run(write_queue_task, default_par, "klcdqueue");

    return 0;
}

static int lcd_remove(struct platform_device *op)
{
    struct fb_info *info = dev_get_drvdata(&op->dev);

    if(info)
    {
        struct lcd_par *par = info->par;
    /*
        *  If your driver supports multiple boards, you should unregister and
        *  clean up all instances.
       */
        write_task_finish = 1;
        wake_up_interruptible(&thr_wq);
        //down(&thread_sem);
		wait_for_completion(&thread_sem);
		
        unregister_framebuffer(info);
        /* ... */

        framebuffer_release(info);
        //kfree(par->screen_base);
        ClearPageReserved(virt_to_page(par->screen_base));
        free_page((int)par->screen_base);
    }

    if(NULL != fd_spi)
    {
        ssp_spi_close(fd_spi);
        fd_spi = NULL;
    }

    return 0;
}

static void lcd_platform_release(struct device *device)
{
}

static struct platform_driver lcd_driver = {
	.driver = {
		.name = "st7567_lcd",
		.owner = THIS_MODULE,
	},
	.probe		= lcd_probe,
	.remove		= lcd_remove,
};

static struct platform_device lcd_device = {
	.name	= "st7567_lcd",
    .id = 0,
    .dev  =
    {
        .release = lcd_platform_release,
    }
};

int __init lcd_init(void)
{
	int ret = 0;
#ifdef DEBUG_LCD_CONTRAST_IN_PROC
	memset( setting_records, 0, MAX_SETTING_RECORDS );
	proc_entry = create_proc_entry( "lcdcontrast", 0644, NULL);
	if (proc_entry == NULL) 
	{
		ret = -ENOMEM;
		printk(KERN_INFO "lcdcontrast: Couldn't create proc entry\n");
	} 
	else 
	{
		overlap_times = 0;
		next_lcdcontrast = 0;
		proc_entry->read_proc = lcdcontrast_read;
		proc_entry->write_proc = lcdcontrast_write;
		proc_entry->owner = THIS_MODULE;
		printk(KERN_INFO "lcdcontrast: Debug Feature Added, max=%d.\n",MAX_SETTING_RECORDS);
	}
#endif

	printk("LCD: Init\n");
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


void lcd_cleanup(void)
{
#ifdef DEBUG_LCD_CONTRAST_IN_PROC
      remove_proc_entry("lcdcontrast", NULL);
      printk(KERN_INFO "lcdcontrast: Debug Feature Deleted.\n");
#endif
	platform_device_unregister(&lcd_device);
	platform_driver_unregister(&lcd_driver);
}

int st7567_status(SSP_HAL_SPI_INFO_T * fd_spi, unsigned char *status, unsigned int len)
{
    return 0;
}


int st7567_contral(SSP_HAL_SPI_INFO_T * fd_spi, unsigned char cmd)
{
	int length = 0;

	lcd_spi_a0_cmd();

	//length = ssp_spi_write_read(fd_spi,(unsigned char*)&cmd, 1, NULL, 0);

    return length;
}

int st7567_write_data(SSP_HAL_SPI_INFO_T * fd_spi, unsigned char *data, int len)
{
	int length = 0;
	char out[2]={0};
	
	lcd_spi_a0_data();

	//length = ssp_spi_write_read(fd_spi,(unsigned char*)data, len, out, 1);
    return 0;
}

int st7567_spi_write_read(SSP_HAL_SPI_INFO_T  *info, u8 *write_buf,
                       u32 write_len, u8 *read_buf, u32 read_len)
{
	lcd_spi_a0_data();

	return ssp_spi_write_read(info, write_buf, write_len, read_buf, read_len);
}

int lcd_setup(void)
{
	unsigned char read_buf[4];
	unsigned char lcd_parameter[] =
	{
		/*reset*/
		0xe2,
		/*set ev = 9.0v, lcd contrast*/
		0x81,
		0x1c,/*Please keep this element in this place[2] and DON'T adjust it. It will be replaced by "GetMainLcdContrast()" as below.*/
		/*seg direction*/
		0xa0,
		/*com direction*/
		0xc8,
		/*inverse display*/
		0xa6,
		/*bias select = 1/9*/
		0xa2,
		/*power control*/
		0x2f,
		/*set booster = 5x*/
		0xf8,
		0x01,
		/*regulation ratio*/
		0x25,
		/*set start line*/
		0x40,
		/*display on*/
		0xaf,
		/*end of char array*/
		0x0
	};
      
	

	/*if(NULL == fd_spi)
	{
		fd_spi = ssp_spi_open();
		if(NULL == fd_spi)
		{
			printk("open ssp spi fail.\n");
			return -1;
		}
		printk("ssp dev opened\n");
	}*/

	/*if(NULL == g_fd_i2c)
	{	g_fd_i2c = ssp_i2c_open();
		if(NULL == g_fd_i2c)
		{
			printk("open ssp spi fail.\n");
			return -1;
		}
		printk("i2c device is opened.\n");
	}
	/*UINT8 data_write_cmd[] = {0x00, 0x01};

	
	
	g_fd_i2c->addr = 0x27;
	ssp_i2c_write(&g_fd_i2c,&data_write_cmd[0],sizeof(data_write_cmd[0]));
	mdelay(1000);
	ssp_i2c_write(&g_fd_i2c,&data_write_cmd[1],sizeof(data_write_cmd[1]));
	mdelay(1000);
	ssp_i2c_write(&g_fd_i2c,&data_write_cmd[0],sizeof(data_write_cmd[0]));*/
	



	/*SPI LCD  register INIT*/
	/*GPIO28 GPIO31*/
	/*SYS_RESET_PIN_SEL_REG(9) &= 0xf3f3ffff;
	SYS_RESET_PIN_SEL_REG(9) |= ~(0xf3f3ffff);
	gpio_set_direction(LCD_A0,OUTPUT);
	gpio_set_direction(LCD_RST,OUTPUT);
	gpio_set_output(LCD_A0,HIGH);
	gpio_set_output(LCD_RST,HIGH);


	lcd_spi_rstb_activate();
	udelay(200000);
	lcd_spi_rstb_deactivate();
	lcd_spi_a0_cmd();

	lcd_parameter[2] = (unsigned char)GetMainLcdContrast();
#ifdef DEBUG_LCD_CONTRAST_IN_PROC
	printk("F %s lcd_contrast=%02x \n",__func__, lcd_parameter[2]);
	setting_records[next_lcdcontrast++] = lcd_parameter[2];
	if (next_lcdcontrast >= MAX_SETTING_RECORDS)
	{
		next_lcdcontrast = 0;
		++overlap_times;
	}
#endif
	//ssp_spi_write_read(fd_spi,lcd_parameter,strlen(lcd_parameter),read_buf,0);

	printk("Main Lcd Contrast : %d\n", GetMainLcdContrast());

	printk("lcd_spi_init\n");*/
	return 0;
}


static int
lcd_ioctl(struct fb_info *info,
		       unsigned int cmd, 
			   unsigned long arg)
{
    down_interruptible(&lcd_sysc);

    if(cmd == LCDFB_LCD_CONTRAST)
    {
        if(arg >= 0 && arg <= 0x3f)
		{
			st7567_contral(fd_spi,0x81);
#ifdef DEBUG_LCD_CONTRAST_IN_PROC
			setting_records[next_lcdcontrast++] = (unsigned char)arg;
			if (next_lcdcontrast >= MAX_SETTING_RECORDS)
			{
				next_lcdcontrast = 0;
				++overlap_times;
			}
#endif
			st7567_contral(fd_spi,arg);
		}
    }
    else if(cmd == LCDFB_LCD_ENABLE)
    {
        writeFB = 1;
    }
    else if(cmd == LCDFB_LCD_DISABLE)
    {
        writeFB = 0;
    }
    up(&lcd_sysc);
    return 0;
}


static int
lcd_mmap(struct fb_info *info, 
       struct vm_area_struct *vma)
{
    struct lcd_par *par = info->par;

    vma->vm_flags |= (VM_SHARED|VM_DONTEXPAND | VM_DONTDUMP);

    printk("lcd_mmap, MMAP ==>> start : 0x%08lx\n", __pa((int)par->screen_base));

    if(remap_pfn_range(vma, vma->vm_start, __pa(par->screen_base) >> PAGE_SHIFT, PAGE_SIZE, PAGE_SHARED))
    {
        printk("LCD: mmap fail...\n");
        ClearPageReserved(virt_to_page(par->screen_base));
        return -EAGAIN;
    }
    return 0;
}


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Driver for ST7567 LCD");
MODULE_AUTHOR("Maintainer: Kenny Xuan <xuan.guanglei@audiocodes.com>");

module_init(lcd_init);

#ifdef MODULE

static void __exit  lcd_exit(void)
{
    lcd_cleanup();
}

module_exit(lcd_exit);

#endif /* MODULE */
