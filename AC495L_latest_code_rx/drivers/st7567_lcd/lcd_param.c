#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/tty.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <asm/mach-audc/prom.h>

#include <asm/io.h>
#include <asm/irq.h>
#include <asm/uaccess.h>
#include <asm/page.h>

#include <linux/fb.h>
#include <linux/vmalloc.h>
#include <linux/device.h>
//#include "ipp_lcd_env.h"
#include "lcd_param.h"

#define LCD_PARAM "LCD_PARAM"

#ifdef CONFIG_MIPS_TNETV1050SDB_AC3xxHD

#define MAIN_LCD_CONTRAST 0x8
#define BLF_LCD_CONTRAST 0x0

#elif defined (CONFIG_MIPS_TNETV1050SDB_AC405HD)

#define MAIN_LCD_CONTRAST 0x1b
#define BLF_LCD_CONTRAST 0x0

#elif defined (CONFIG_MIPS_TNETV1050SDB_AC420HD)

#define MAIN_LCD_CONTRAST 0x12
#define BLF_LCD_CONTRAST 0x0

#elif defined (CONFIG_MIPS_TNETV1050SDB_AC430HD)  \
      || defined (CONFIG_MIPS_TNETV1050SDB_AC440HD)

#define MAIN_LCD_CONTRAST 0x45
#define BLF_LCD_CONTRAST 0x8c

#elif defined (CONFIG_MIPS_TNETV1051SDB_AC450HD)

#define MAIN_LCD_CONTRAST 0x8
#define BLF_LCD_CONTRAST 0x0

#else

//#define MAIN_LCD_CONTRAST 0x0
//#define BLF_LCD_CONTRAST 0x0

#define MAIN_LCD_CONTRAST 0x1b
#define BLF_LCD_CONTRAST 0x0

#endif

int GetMainLcdContrast(void)
{
    char *LcdParam = NULL;

    LcdParam = prom_getenv(LCD_PARAM);
    if(NULL == LcdParam)
    {
        return MAIN_LCD_CONTRAST;
    }

    return simple_strtoul(LcdParam,NULL,0);
}

int GetBlfLcdContrast2(void)
{
    const char *LcdParam = NULL;

    LcdParam = prom_getenv(LCD_PARAM);
    if(NULL == LcdParam)
    {
        return BLF_LCD_CONTRAST;
    }

    LcdParam = strstr(LcdParam,";");
    if(NULL == LcdParam)
    {
        return BLF_LCD_CONTRAST;
    }

    LcdParam++;
    return simple_strtoul(LcdParam,NULL,0);
}

int GetHwRevision(void)
{
	char *HwRevision = NULL;

	HwRevision = prom_getenv("HW_REVISION");
	if(NULL == HwRevision)
	{
		return 0;
	}

	return simple_strtoul(HwRevision,NULL,0);
}

//EXPORT_SYMBOL(GetMainLcdContrast);
//EXPORT_SYMBOL(GetBlfLcdContrast2);
//EXPORT_SYMBOL(GetHwRevision);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Lcd parameter APIs");
MODULE_AUTHOR("Maintainer: Kenny Xuan <xuan.guanglei@audiocodes.com>");

