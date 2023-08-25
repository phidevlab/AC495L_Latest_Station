/*******************************************************************************
**+--------------------------------------------------------------------------+**
**|                            ****                                          |**
**|                            ****                                          |**
**|                            ******o***                                    |**
**|                      ********_///_****                                   |**
**|                      ***** /_//_/ ****                                   |**
**|                       ** ** (__/ ****                                    |**
**|                           *********                                      |**
**|                            ****                                          |**
**|                            ***                                           |**
**|                                                                          |**
**|         Copyright (c) 1998-2005 Texas Instruments Incorporated           |**
**|                        ALL RIGHTS RESERVED                               |**
**|                                                                          |**
**+--------------------------------------------------------------------------+**
*******************************************************************************/

/*******************************************************************************
 * FILE PURPOSE:    SoC related definitions
 *******************************************************************************
 * FILE NAME:       hw.h
 *
 * DESCRIPTION:     SoC related definitions
 *
 ******************************************************************************/

#ifndef _HW_H_
#define _HW_H_

#ifdef _STANDALONE
#include <psbl/sysconf.h>
#endif
#include <acl/config.h>

#define EMIF_SDRAM_BASE           0x94000000
#define EMIF_SDRAM_MAX_SIZE       0x08000000 /*0x08000000 or 0x01000000*/

#ifdef BE
#define ASCII_DISP_BASE           0xbc4001c3
#else
#define ASCII_DISP_BASE           0xbc4001c0
#endif

#define ASCII_DISP_OFFSET         8


#if defined(SEAD2)  /* The board being used is SEAD2 */

#define SIO1__BASE     0xbc700000
#define SIO1__OFFSET   8
#define SIO1__EN       1
#define SIO1__FDEN     1
#define SIO1__RSTMASK  0x00
#define SIO0__BASE     0xa8610e00
#define SIO0__OFFSET   4
#define SIO0__EN       1
#define SIO0__FDEN     1
#define SIO0__RSTMASK  0x01

#else

#define SIO_OFFSET     4
#if defined(VOLCANO_ARIES_BOARD)
#define SIO1_BASE     0xa8610f00
#define SIO1_RSTMASK  0x02
#define _SIO0_BASE     0xa8610e00
#define _SIO0_RSTMASK  0x01
#elif defined(TNETV1051_BOARD)
#if defined(CONFIG_450) || defined(CONFIG_450_EXP)
#define SIO2_BASE     0xa8613f00
#define SIO2_RSTMASK  0x80
#define _SIO0_BASE     0xa8610e00
#define _SIO0_RSTMASK  0x01
#else
#define _SIO0_BASE     0xa8613f00
#define _SIO0_RSTMASK  0x80
#define SIO2_BASE     0xa8610e00
#define SIO2_RSTMASK  0x01
#endif /*CONFIG_450 || CONFIG_450_EXP*/
#define SIO1_BASE     0xa8610f00
#define SIO1_RSTMASK  0x02
#elif defined(TNETV1050_BOARD) || defined(TNETV1061_BOARD)
#define SIO1_BASE     0xa8610f00
#define SIO1_RSTMASK  0x42
#define _SIO0_BASE     0xa8610e00
#define _SIO0_RSTMASK  0x01
#endif/*#if define (VOLCANO_ARIES_BOARD)*/

/*Add by Kenny. Auto select serial port to init*/
#ifdef CONFIG_TTYS0
#define SIO0_BASE       _SIO0_BASE
#define SIO0_RSTMASK    _SIO0_RSTMASK
#elif CONFIG_TTYS1
#define SIO0_BASE       SIO1_BASE
#define SIO0_RSTMASK    SIO1_RSTMASK
#elif CONFIG_TTYS2
#define SIO0_BASE       SIO2_BASE
#define SIO0_RSTMASK    SIO2_RSTMASK
#elif defined(CONFIG_450) || defined(CONFIG_450_EXP)
#define SIO0_BASE       _SIO0_BASE
#define SIO0_RSTMASK    _SIO0_RSTMASK
#else
#error does not define TTY
#endif


#endif /* ifdef SEAD2 */

#define CS0_BASE 0xb0000000

#define _CS0_SIZE CONF_FLASH_SZ

#ifndef _ASSEMBLER_
extern unsigned int CS0_SIZE;
#endif


#define CS1_BASE 0xb4000000
#define CS1_SIZE CONF_SDRAM_SZ

#define CS3_BASE 0xbc000000
#ifdef NAND_SBL
#define _CS3_SIZE (16*1024*1024)
#else
#define _CS3_SIZE (128*1024*1024)
#endif

#define CS4_BASE 0xbd000000

#ifdef DUAL_FLASH
#define CS4_SIZE _CS0_SIZE
#else
#define CS4_SIZE (16*1024*1024)
#endif

#define CS5_BASE 0xbe000000
#define CS5_SIZE (16*1024*1024)

#if defined(TNETV1051_BOARD)
#ifndef NAND_SBL
#ifndef EMIF_BASE
#define EMIF_BASE        0xA8610800
#endif
#define EMIF_REV        (*(volatile unsigned int*)(EMIF_BASE+0x00))
#define EMIF_GASYNC     (*(volatile unsigned int*)(EMIF_BASE+0x04))
#define EMIF_DRAMCTL    (*(volatile unsigned int*)(EMIF_BASE+0x08))
#define EMIF_REFRESH    (*(volatile unsigned int*)(EMIF_BASE+0x0c))
#define EMIF_ASYNC_CS0  (*(volatile unsigned int*)(EMIF_BASE+0x10))
#define EMIF_ASYNC_CS3  (*(volatile unsigned int*)(EMIF_BASE+0x14))
#define EMIF_ASYNC_CS4  (*(volatile unsigned int*)(EMIF_BASE+0x18))
#define EMIF_ASYNC_CS5  (*(volatile unsigned int*)(EMIF_BASE+0x1c))

#define ASYNC_BASE       0xA8610800
#define NAND_FLASH_CTRL_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x60))
#define NAND_FLASH_STAT_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x64))
#define NAND_FLASH_ECC1_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x70))
#define NAND_FLASH_ECC2_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x74))
#define NAND_FLASH_ECC3_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x78))
#define NAND_FLASH_ECC4_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x7C))

#else
/* ARIES SDB */
#define BOOTCR_PLL_BYPASS_BITNUM  5
#define BOOTCR_PLL_MODE_BITNUM    25
#define ASYNC_BASE       0xA8610800
#define DDR_BASE         0xA8613000
#define EMIF_REV        (*(volatile unsigned int*)(DDR_BASE+0x00))
#define EMIF_GASYNC     (*(volatile unsigned int*)(DDR_BASE+0x04))
#define EMIF_DRAMCTL    (*(volatile unsigned int*)(DDR_BASE+0x08))
#define EMIF_REFRESH    (*(volatile unsigned int*)(DDR_BASE+0x0c))
#define EMIF_ASYNC_CS0  (*(volatile unsigned int*)(ASYNC_BASE+0x10))
#define EMIF_ASYNC_CS3  (*(volatile unsigned int*)(ASYNC_BASE+0x14))
#define EMIF_ASYNC_CS4  (*(volatile unsigned int*)(ASYNC_BASE+0x18))
#define EMIF_ASYNC_CS5  (*(volatile unsigned int*)(ASYNC_BASE+0x1c))
#define NAND_FLASH_CTRL_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x60))
#define NAND_FLASH_STAT_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x64))
#define NAND_FLASH_ECC1_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x70))
#define NAND_FLASH_ECC2_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x74))
#define NAND_FLASH_ECC3_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x78))
#define NAND_FLASH_ECC4_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x7C))
#endif /* NAND_SBL */

#elif defined(VOLCANO_ARIES_BOARD)
/* VOLCANO_ARIES_BOARD */
#define BOOTCR_PLL_BYPASS_BITNUM  5
#define BOOTCR_PLL_MODE_BITNUM    25
#define ASYNC_BASE       0xA8610800
#define DDR_BASE         0xA8613000
#define EMIF_REV        (*(volatile unsigned int*)(DDR_BASE+0x00))
#define EMIF_GASYNC     (*(volatile unsigned int*)(DDR_BASE+0x04))
#define EMIF_DRAMCTL    (*(volatile unsigned int*)(DDR_BASE+0x08))
#define EMIF_REFRESH    (*(volatile unsigned int*)(DDR_BASE+0x0c))
#define EMIF_ASYNC_CS0  (*(volatile unsigned int*)(ASYNC_BASE+0x10))
#define EMIF_ASYNC_CS3  (*(volatile unsigned int*)(ASYNC_BASE+0x14))
#define EMIF_ASYNC_CS4  (*(volatile unsigned int*)(ASYNC_BASE+0x18))
#define EMIF_ASYNC_CS5  (*(volatile unsigned int*)(ASYNC_BASE+0x1c))
#define NAND_FLASH_CTRL_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x60))
#define NAND_FLASH_STAT_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x64))
#define NAND_FLASH_ECC1_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x70))
#define NAND_FLASH_ECC2_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x74))
#define NAND_FLASH_ECC3_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x78))
#define NAND_FLASH_ECC4_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x7C))

#elif defined(TNETV1061_BOARD)
/* PETRA TNETV1061_BOARD */
#define BOOTCR_PLL_BYPASS_BITNUM  5
#define BOOTCR_PLL_MODE_BITNUM    25
#define BOOTCR_SYSPLSEL_BITNUM	  15	
#define BOOTCR_ASYNCPLSEL_BITNUM  17
#define ASYNC_BASE       0xA8610800
#define DDR_BASE         0xA8613000
#define EMIF_REV        (*(volatile unsigned int*)(ASYNC_BASE+0x00))
#define EMIF_GASYNC     (*(volatile unsigned int*)(ASYNC_BASE+0x04))
#define EMIF_ASYNC_CS0  (*(volatile unsigned int*)(ASYNC_BASE+0x10))
#define EMIF_ASYNC_CS3  (*(volatile unsigned int*)(ASYNC_BASE+0x14))
#define EMIF_ASYNC_CS4  (*(volatile unsigned int*)(ASYNC_BASE+0x18))
#define EMIF_ASYNC_CS5  (*(volatile unsigned int*)(ASYNC_BASE+0x1c))
#define NAND_FLASH_CTRL_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x60))
#define NAND_FLASH_STAT_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x64))
#define NAND_FLASH_ECC1_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x70))
#define NAND_FLASH_ECC2_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x74))
#define NAND_FLASH_ECC3_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x78))
#define NAND_FLASH_ECC4_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x7C))
#define DDR_DRAMCTL     (*(volatile unsigned int*)(DDR_BASE+0x08))
#define DDR_REFRESH     (*(volatile unsigned int*)(DDR_BASE+0x0c))

#else

#ifndef EMIF_BASE
#define EMIF_BASE        0xA8610800
#endif
#define ASYNC_BASE       0xA8610800
#define EMIF_REV        (*(volatile unsigned int*)(EMIF_BASE+0x00))
#define EMIF_GASYNC     (*(volatile unsigned int*)(EMIF_BASE+0x04))
#define EMIF_DRAMCTL    (*(volatile unsigned int*)(EMIF_BASE+0x08))
#define EMIF_REFRESH    (*(volatile unsigned int*)(EMIF_BASE+0x0c))
#define EMIF_ASYNC_CS0  (*(volatile unsigned int*)(EMIF_BASE+0x10))
#define EMIF_ASYNC_CS3  (*(volatile unsigned int*)(EMIF_BASE+0x14))
#define EMIF_ASYNC_CS4  (*(volatile unsigned int*)(EMIF_BASE+0x18))
#define EMIF_ASYNC_CS5  (*(volatile unsigned int*)(EMIF_BASE+0x1c))
#define NAND_FLASH_CTRL_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x60))
#define NAND_FLASH_STAT_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x64))
#define NAND_FLASH_ECC1_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x70))
#define NAND_FLASH_ECC2_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x74))
#define NAND_FLASH_ECC3_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x78))
#define NAND_FLASH_ECC4_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x7C))

#endif /* TNETV1051_BOARD */

#define GPIO_BASE     0xa8610900
#if defined(TNETV1050_BOARD) || defined(TNETV1061_BOARD)
/* These defines are for GPIO EN,OUT and DAT 0 */
#define GPIO_EN         (*(volatile unsigned int *)(GPIO_BASE+0x18))
#define GPIO_OUT        (*(volatile unsigned int *)(GPIO_BASE+0x8))
#define GPIO_DIR        (*(volatile unsigned int *)(GPIO_BASE+0x10))
#else
#define GPIO_EN         (*(volatile unsigned int *)(GPIO_BASE+0xc))
#define GPIO_OUT        (*(volatile unsigned int *)(GPIO_BASE+0x4))
#define GPIO_DIR        (*(volatile unsigned int *)(GPIO_BASE+0x8))
#ifndef __ASSEMBLER__
#define APP_DIDR_HI     (*(volatile unsigned int *)(GPIO_BASE+0x3c))
#else
#define APP_DIDR_HI     0xa861093c
#define PLL_300         0xb7fe
#define PLL_250         0x97fe
#endif
#define ARIES_1051     0
#define ARIES_1052     1
#define ARIES_1053     2 
#endif

#define AVALANCHE_RESET_CONTROL_BASE    (0xa8611600) /* AVALANCHE Reset Control */

#define TITAN_IOMUX_PIN_SEL_1	   (AVALANCHE_RESET_CONTROL_BASE + 0x0C)
#define TITAN_IOMUX_PIN_SEL_2	   (AVALANCHE_RESET_CONTROL_BASE + 0x10)
#define TITAN_IOMUX_PIN_SEL_3	   (AVALANCHE_RESET_CONTROL_BASE + 0x14)
#define TITAN_IOMUX_PIN_SEL_4	   (AVALANCHE_RESET_CONTROL_BASE + 0x18)
#define TITAN_IOMUX_PIN_SEL_5	   (AVALANCHE_RESET_CONTROL_BASE + 0x1C)
#define TITAN_IOMUX_PIN_SEL_6	   (AVALANCHE_RESET_CONTROL_BASE + 0x20)
#define TITAN_IOMUX_PIN_SEL_7	   (AVALANCHE_RESET_CONTROL_BASE + 0x24)
#define TITAN_IOMUX_PIN_SEL_8	   (AVALANCHE_RESET_CONTROL_BASE + 0x28)
#define TITAN_IOMUX_PIN_SEL_9	   (AVALANCHE_RESET_CONTROL_BASE + 0x2C)
#define TITAN_IOMUX_PIN_SEL_10	   (AVALANCHE_RESET_CONTROL_BASE + 0x30)
#define TITAN_IOMUX_PIN_SEL_11	   (AVALANCHE_RESET_CONTROL_BASE + 0x34)
#define TITAN_IOMUX_PIN_SEL_12	   (AVALANCHE_RESET_CONTROL_BASE + 0x38)
#define TITAN_IOMUX_PIN_SEL_13	   (AVALANCHE_RESET_CONTROL_BASE + 0x3C)
#define TITAN_IOMUX_PIN_SEL_14	   (AVALANCHE_RESET_CONTROL_BASE + 0x40)
#define TITAN_IOMUX_PIN_SEL_15	   (AVALANCHE_RESET_CONTROL_BASE + 0x44)
#define TITAN_IOMUX_PIN_SEL_16	   (AVALANCHE_RESET_CONTROL_BASE + 0x48)
#define TITAN_IOMUX_PIN_SEL_17	   (AVALANCHE_RESET_CONTROL_BASE + 0x4C)
#define TITAN_IOMUX_PIN_SEL_18	   (AVALANCHE_RESET_CONTROL_BASE + 0x50)
#define TITAN_IOMUX_PIN_SEL_19	   (AVALANCHE_RESET_CONTROL_BASE + 0x54)
#define TITAN_IOMUX_PIN_SEL_20	   (AVALANCHE_RESET_CONTROL_BASE + 0x58)
#define TITAN_IOMUX_PIN_SEL_21	   (AVALANCHE_RESET_CONTROL_BASE + 0x5C)


#define RESET_BASE  0xa8611600
#define RESET_REG       (*(volatile unsigned *)(RESET_BASE+0x00))
#define RESET_STATUS    (*(volatile unsigned *)(RESET_BASE+0x08))

#if defined(TNETV1051_BOARD)
#define ARIES_PIN_MUX_BASE           (RESET_BASE+0x0C)
#define ARIES_PIN_MUX_REG_ADDR(n)    ((ARIES_PIN_MUX_BASE+((n)*4)))
#define ARIES_PIN_MUX_REG(n)         (*(volatile unsigned *)(ARIES_PIN_MUX_BASE+((n)*4)))
#define IOMUX_SSP1                  ARIES_PIN_MUX_REG_ADDR(10-1)
#define IOMUX_SSP2                  ARIES_PIN_MUX_REG_ADDR(14-1)
#define IOMUX_SSP1_VAL              0x01540000
#define IOMUX_SSP1_CLR              0xFC03FFFF       /* SSP0_0 - SSP0_3 */
#define IOMUX_SSP2_VAL              0x0F700000       /* SSP1_0,SSP1_2,SSP1_3, EXTINT1 */
#define IOMUX_SSP2_CLR              0xF00FFFFF
#else /* TNETV1051_BOARD */
#define IOMUX_SSP1                  TITAN_IOMUX_PIN_SEL_1
#define IOMUX_SSP2                  TITAN_IOMUX_PIN_SEL_5
#define IOMUX_SSP1_VAL              0x00055000
#define IOMUX_SSP1_CLR              0xFFF00FFF
#define IOMUX_SSP2_VAL              0x00000300
#define IOMUX_SSP2_CLR              0xFFFFFCFF
#endif /* TNETV1051_BOARD */

#define BOOTCR      0xa8611a00

#ifdef DUAL_FLASH
#define SEC_FLASH_BASE          CS4_BASE
#define SEC_FLASH_SIZE          CS4_SIZE
#define _SEC_FLASH_SIZE         CONF_FLASH_SZ
#endif

#if defined (TNETD73XX_BOARD)
#define IS_OHIO_CHIP() (( (REG32_R( 0xA8610914,15,0 ) == 0x2b) || (REG32_R( 0xA8610914,15,0 ) == 0x18)) ? 1:0)
#define AVALANCHE_MII_SEL_REG         (0xa8611A08)
#endif
#endif /* _HW_H_ */


#if defined(CONFIG_405)

#define HW_REVISION_GPIO_BIT0 1
#define HW_REVISION_GPIO_BIT1 2
#define HW_REVISION_GPIO_BIT2 3
#define SOC_TYPE_GPIO 5
#define HOOK_SWITCH_GPIO 0

#define LCD_BACKLIGHT_EN_GPIO	7
#define RING_LED_GPIO			47

#define SW_RST_GPIO 33
#define SMI_MCLK 50
#define SMI_MDIO 49

#define EN_USB_PWR_GPIO 35
#define USB_OVR_CURRN_GPIO 4


#elif CONFIG_420

#define HOOK_SWITCH_GPIO 0
#define HW_REVISION_GPIO_BIT0 1
#define HW_REVISION_GPIO_BIT1 2
#define HW_REVISION_GPIO_BIT2 3
#define SOC_TYPE_GPIO 4

#define FE_RING_LED_GPIO 40
#define FE_SPEAKER_LED_GPIO 41
#define FE_HEADSET_LED_GPIO 42
#define FE_MUTE_LED_GPIO 43

#define GBE_RING_LED_GPIO 36
#define GBE_SPEAKER_LED_GPIO 34
#define GBE_HEADSET_LED_GPIO 35
#define GBE_MUTE_LED_GPIO 33
#define GBE_HW_WD_GPIO 37

#define FE_LCD_BACKLIGHT_EN_GPIO 46
#define GBE_LCD_BACKLIGHT_EN_GPIO 5
#define LCD_RST_GPIO 47

#define SW_RST_GPIO 6
#define SMI_MCLK 39
#define SMI_MDIO 38

#elif defined (CONFIG_430) || defined (CONFIG_440)

#define HW_REVISION_GPIO_BIT0 1
#define HW_REVISION_GPIO_BIT1 2
#define HW_REVISION_GPIO_BIT2 3
#define SOC_TYPE_GPIO 4

#define BLF_LCD_CS 5    /*blf lcd cs0*/

#define BLF_LCD_BACKLIGHT_EN_GPIO 5

#define SPK_SHDN 23

#define RING_LED_GPIO 36

#define HOOK_SWITCH_GPIO 0

#define LCD_BACKLIGHT_EN_GPIO 7
#define LCD_RST_GPIO 37

#define SW_RST_GPIO 6

#define CHIP74LVC595_OE_GPIO 35
#define CHIP74LVC595_DS_GPIO 34
#define CHIP74LVC595_STCP_GPIO 32
#define CHIP74LVC595_SHCP_GPIO 33

#define SMI_MCLK 39
#define SMI_MDIO 38

#elif defined (CONFIG_450)|| defined(CONFIG_450_EXP)

#define HW_REVISION_GPIO_BIT0 0
#define HW_REVISION_GPIO_BIT1 1
#define HW_REVISION_GPIO_BIT2 2
#define HW_REVISION_GPIO_BIT3 3
#define SOC_TYPE_GPIO 4

#define RING_LED_GPIO 37
#define VOICEMAIL_LED_GPIO 38
#define SPEAKER_LED_GPIO 39
#define HEADSET_LED_GPIO 40
//#define MUTE_LED_GPIO 39

#define LCD_BACKLIGHT_EN_GPIO 47

#define CHIP74LVC595_OE_GPIO 45
#define CHIP74LVC595_DS_GPIO 46
#define CHIP74LVC595_STCP_GPIO 40
#define CHIP74LVC595_SHCP_GPIO 42

#define HOOK_SWITCH_GPIO 0

#define LCD_SPI_CLK 25
#define LCD_SPI_CS 26
#define LCD_SPI_DOUT 28

#define SW_RST_GPIO 30
#define SMI_MCLK 44
#define SMI_MDIO 43

#define CPGMAC_I "9999:0:0:1:1:0:0:0:1:1:100:15:4:100:15:4:0:0"

#endif

