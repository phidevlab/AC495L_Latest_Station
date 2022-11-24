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
 * FILE PURPOSE:    PSPboot configuration parameters.
 *******************************************************************************
 * FILE NAME:       sysconf.h
 *
 * DESCRIPTION:     PSPboot configuration parameters.
 *
 ******************************************************************************/

#ifndef _SYSCONF_H_
#define _SYSCONF_H_

/* Macros for unit conversions */
#define MEG(x)      ((x) << 20)
#define MHZ(x)      ((x) * 1000 * 1000)
#define KILOBYTE(x) ((x) << 10)
#define KHZ(x)      ((x) * 1000)

/* Definitions for cache types */
#define CACHE_WT_NWA  0     /* write through, no write allocate*/
#define CACHE_WT     1          /* 1050 only, write through, write allocate */
#define CACHE_NONE   2        /* cache disabled */
#define CACHE_WB     3          /* write back, write allocate */

/* Pre-processor macro to get the board name string into the code
 * This depends of BOARD=XXX and XXX=XXX being defined externally 
 * (probably through a makefile). The BOARDNAME macro will return the 
 * string "XXX"
 */
#define BRDSTR(E)   #E
#define MKSTR(X)    BRDSTR(X)
#ifdef HW_TYPE
#define BOARDNAME   HW_TYPE
#else
#define BOARDNAME   MKSTR(BOARD)
#endif

#define CONF_HEAP_SZ    KILOBYTE(1024)

/* Cable Modem 401B board: Avalanche I SoC */
#if defined (TNETC401B) 

#define CONF_REFXTAL_FREQ   MHZ(25)
#define CONF_CPU_FREQ       MHZ(125)
#define CONF_SYS_FREQ       MHZ(125)
#define CONF_SDRAM_SZ       MEG(8)
#define CONF_FLASH_SZ       MEG(4)
#define CONF_NUM_MAC_PORT   1
#define CONF_CACHE          CACHE_WT

/* Cable Modem 520 board: Puma S SoC */
#elif defined (TNETC520)

#define CONF_REFXTAL_FREQ   MHZ(25)
#define CONF_CPU_FREQ       MHZ(125)
#define CONF_SYS_FREQ       MHZ(125)
#define CONF_SDRAM_SZ       MEG(16)
#define CONF_FLASH_SZ       MEG(4)
#define CONF_NUM_MAC_PORT   1
#define CONF_CACHE          CACHE_WT

/* Cable Modem 620 board: Puma S SoC */
#elif defined (TNETC620)

#define CONF_REFXTAL_FREQ   MHZ(25)
#define CONF_CPU_FREQ       MHZ(125)
#define CONF_SYS_FREQ       MHZ(125)
#define CONF_SDRAM_SZ       MEG(16)
#define CONF_FLASH_SZ       MEG(8)
#define CONF_NUM_MAC_PORT   1
#define CONF_CACHE          CACHE_WT

/* Cable Modem 420 board: Puma S SoC */
#elif defined (TNETC420)

#define CONF_REFXTAL_FREQ   MHZ(25)
#define CONF_CPU_FREQ       MHZ(125)
#define CONF_SYS_FREQ       MHZ(125)
#define CONF_SDRAM_SZ       MEG(8)
#define CONF_FLASH_SZ       MEG(2)
#define CONF_NUM_MAC_PORT   1
#define CONF_CACHE          CACHE_WT

/* Cable Modem 405T board: Avalanche D SoC */
#elif defined (TNETC405T)

#define CONF_REFXTAL_FREQ   MHZ(25)
#define CONF_CPU_FREQ       MHZ(125)
#define CONF_SYS_FREQ       MHZ(125)
#define CONF_SDRAM_SZ       MEG(8)
#define CONF_FLASH_SZ       MEG(2)
#define CONF_NUM_MAC_PORT   1
#define CONF_CACHE          CACHE_WT

/* AR7 Verification and Debug Board. */
#elif defined(AR7VDB)   
#define TNETD73XX_BOARD
#define CONF_REFXTAL_FREQ   MHZ(25)
#define CONF_AFEXTAL_FREQ   MHZ(25)
#define CONF_XTAL3IN_FREQ   MHZ(25)
#define CONF_CPU_FREQ       MHZ(150)
#define CONF_SYS_FREQ       MHZ(125)
#define CONF_SDRAM_SZ       MEG(32)
#define CONF_FLASH_SZ       MEG(16)
#define CONF_NUM_MAC_PORT   2
#define CONF_CACHE          CACHE_WB

/* AR7 development boards */
#elif defined(AR7DB) 
#define TNETD73XX_BOARD
#define CONF_REFXTAL_FREQ   MHZ(25)
#define CONF_AFEXTAL_FREQ   MHZ(25)
#define CONF_XTAL3IN_FREQ   MHZ(25)
#define CONF_CPU_FREQ       MHZ(150)
#define CONF_SYS_FREQ       MHZ(125)
#define CONF_SDRAM_SZ       MEG(16)
#define CONF_FLASH_SZ       MEG(4)
#define CONF_NUM_MAC_PORT   2
#define CONF_CACHE          CACHE_WB

#elif defined(AR7RD) 
#define TNETD73XX_BOARD
#define CONF_REFXTAL_FREQ   MHZ(25)
#define CONF_AFEXTAL_FREQ   MHZ(25)
#define CONF_XTAL3IN_FREQ   MHZ(25)
#define CONF_CPU_FREQ       MHZ(150)
#define CONF_SYS_FREQ       MHZ(125)
#define CONF_SDRAM_SZ       MEG(16)
#define CONF_FLASH_SZ       MEG(4)
#define CONF_NUM_MAC_PORT   1
#define CONF_CACHE          CACHE_WB

#elif defined(AR7WRD) 
#define TNETD73XX_BOARD
#define CONF_REFXTAL_FREQ   MHZ(25)
#define CONF_AFEXTAL_FREQ   MHZ(25)
#define CONF_XTAL3IN_FREQ   MHZ(25)
#define CONF_CPU_FREQ       MHZ(150)
#define CONF_SYS_FREQ       MHZ(125)
#define CONF_SDRAM_SZ       MEG(16)
#define CONF_FLASH_SZ       MEG(4)
#define CONF_NUM_MAC_PORT   1
#define CONF_CACHE          CACHE_WB

#elif defined(AR7Wi) 
#define TNETD73XX_BOARD
#define CONF_REFXTAL_FREQ   MHZ(25)
#define CONF_AFEXTAL_FREQ   MHZ(25)
#define CONF_XTAL3IN_FREQ   MHZ(25)
#define CONF_CPU_FREQ       MHZ(150)
#define CONF_SYS_FREQ       MHZ(125)
#define CONF_SDRAM_SZ       MEG(16)
#define CONF_FLASH_SZ       MEG(4)
#define CONF_NUM_MAC_PORT   1
#define CONF_CACHE          CACHE_WB

#elif defined (AR7VWi) 
#define TNETD73XX_BOARD
#define CONF_REFXTAL_FREQ   MHZ(25)
#define CONF_AFEXTAL_FREQ   MHZ(25)
#define CONF_XTAL3IN_FREQ   MHZ(25)
#define CONF_CPU_FREQ       MHZ(150)
#define CONF_SYS_FREQ       MHZ(125)
#define CONF_SDRAM_SZ       MEG(16)
#define CONF_FLASH_SZ       MEG(4)
#define CONF_NUM_MAC_PORT   1
#define CONF_CACHE          CACHE_WB

#elif defined (AR7L0)
#define TNETD73XX_BOARD
#define CONF_REFXTAL_FREQ   MHZ(25)
#define CONF_AFEXTAL_FREQ   MHZ(25)
#define CONF_XTAL3IN_FREQ   MHZ(25)
#define CONF_CPU_FREQ       MHZ(150)
#define CONF_SYS_FREQ       MHZ(125)
#define CONF_SDRAM_SZ       MEG(8)
#define CONF_FLASH_SZ       MEG(2)
#define CONF_NUM_MAC_PORT   1
#define CONF_CACHE          CACHE_WB        

/* TNETV1050 Verification and Debug Board */
#elif defined(TNETV1050VDB)   
#define TNETV1050_BOARD
#define CONF_REFXTAL_FREQ   MHZ(25)
#define CONF_AUDIO_FREQ     KHZ(8192)
#define CONF_ALT_FREQ       MHZ(12)
#define CONF_CPU_FREQ       KHZ(162500)
#define CONF_SYS_FREQ       MHZ(125)
#define CONF_SDRAM_SZ       MEG(64)
#define CONF_FLASH_SZ       MEG(16)
#define CONF_NUM_MAC_PORT   2
#define CONF_CACHE          CACHE_WT

/* TNETV1050 Software Development Board */
#elif defined(TNETV1050SDB)   
#define TNETV1050_BOARD
#define CONF_REFXTAL_FREQ   MHZ(25)
#define CONF_AUDIO_FREQ     KHZ(8192)
#define CONF_ALT_FREQ       MHZ(12)
#define CONF_CPU_FREQ       KHZ(162500)
#define CONF_SYS_FREQ       MHZ(125)
#define DSP_MEM_SIZE 0x80000

#define CONF_SDRAM_SZ       0x00F80000/*By Steve Chang. MEG(16) Reserve the last 0.5 MB for the use of DSP firmware*/

#ifdef CONFIG_FLASH_SIZE
#define CONF_FLASH_SZ       MEG(CONFIG_FLASH_SIZE)
#else
#define CONF_FLASH_SZ       MEG(8)
#endif

#if defined (CONFIG_420) || \
    defined (CONFIG_430) || defined (CONFIG_440) || \
    defined (CONFIG_450) || defined(CONFIG_450_EXP) ||\
    defined (CONFIG_405)
#define CONF_NUM_MAC_PORT   2
#define EXTERNAL_PHY_PORT0      0   
#define MAC0_EXT_PHY_MASK       0x2   
#define MAC0_INT_PHY_MASK       0x40000000   
#define EXTERNAL_PHY_PORT1      0   
#define MAC1_EXT_PHY_MASK       0x8   
#define MAC1_INT_PHY_MASK       0x80000000
#endif

#define CONF_CACHE          CACHE_WB

/* Wireless LAN Software Development AP */
#elif defined(WLAN)   
#define CONF_REFXTAL_FREQ   MHZ(25)
#define CONF_CPU_FREQ       MHZ(125)
#define CONF_SYS_FREQ       MHZ(125)
#define CONF_SDRAM_SZ       MEG(8)
#define CONF_FLASH_SZ       FWBGet_flash_type()
#define CONF_NUM_MAC_PORT   1
#define CONF_CACHE          CACHE_WT

/* Wireless LAN Gateway Cable Modem */
#elif defined(TNETC621)   
#define CONF_REFXTAL_FREQ   MHZ(25)
#define CONF_CPU_FREQ       MHZ(125)
#define CONF_SYS_FREQ       MHZ(125)
#define CONF_SDRAM_SZ       MEG(16)
#define CONF_FLASH_SZ       FWBGet_flash_type()
#define CONF_NUM_MAC_PORT   1
#define CONF_CACHE          CACHE_WT

/* MIPS SEAD2 emulation platform */
#elif defined(SEAD2)   
#define CONF_REFXTAL_FREQ   MHZ(32)
#define CONF_CPU_FREQ       MHZ(32)
#define CONF_SYS_FREQ       MHZ(32)
#define CONF_SDRAM_SZ       MEG(32)
#define CONF_FLASH_SZ       MEG(32)
#define CONF_NUM_MAC_PORT   2
#define CONF_CACHE          CACHE_WT

/* Apex Verification and debug board */
#elif defined(TNETV1020VDB)   
#define TNETV1020_BOARD
#define CONF_XTAL1IN_FREQ   MHZ(25)
#define CONF_XTAL2IN_FREQ   MHZ(25)
#define CONF_OSCIN_FREQ     MHZ(25)
#define CONF_CPU_FREQ       MHZ(275)
#define CONF_SYS_FREQ       KHZ(137500)
#define CONF_SDRAM_SZ       MEG(32)
#define CONF_FLASH_SZ       MEG(16)
#define CONF_NUM_MAC_PORT   2
#define CONF_CACHE          CACHE_WB

/* WLAN Apex board */
#elif defined(TNETWA115VAG)   
#define TNETV1020_BOARD
#define CONF_XTAL1IN_FREQ   MHZ(25)
#define CONF_XTAL2IN_FREQ   MHZ(25)
#define CONF_OSCIN_FREQ     MHZ(25)
#define CONF_CPU_FREQ       MHZ(250)
#define CONF_SYS_FREQ       MHZ(125)
#define CONF_SDRAM_SZ       MEG(8)
#define CONF_FLASH_SZ       MEG(4)
#define CONF_NUM_MAC_PORT   1
#define CONF_CACHE          CACHE_WB

/* TNETV1051_BOARD */
#elif defined(TNETV1051_BOARD)   
/*#define TNETV1051_BOARD*/
#define CONF_REFXTAL_FREQ     MHZ(25)
#define CONF_AFEXTAL_FREQ     KHZ(16384)
#define CONF_MIPS_FREQ        MHZ(300)
#define CONF_CPU_FREQ         MHZ(300)
#define CONF_SYS_FREQ         MHZ(150)
#define CONF_PBUS_FREQ        MHZ(75)

#if !(defined(CONFIG_450) || defined(CONFIG_450_EXP))
/*Reserve the last 0.5 MB for the use of DSP firmware*/
#define DSP_MEM_SIZE 0x80000
#define CONF_SDRAM_SZ       (MEG(64) - DSP_MEM_SIZE)
#else
/*Reserve the last 1 MB for the use of DSP firmware/bootloader display buffer*/
#define DSP_MEM_SIZE 0x100000
#define CONF_SDRAM_SZ       (MEG(128) - DSP_MEM_SIZE)
#endif

#ifdef CONFIG_FLASH_SIZE
#define CONF_FLASH_SZ       MEG(CONFIG_FLASH_SIZE)
#else
#define CONF_FLASH_SZ       MEG(16)
#endif

#define CONF_NUM_MAC_PORT       2
#define EXTERNAL_PHY_PORT0      1
#define MAC0_EXT_PHY_MASK       0x00080000
#define MAC0_INT_PHY_MASK       0x00008000
#define EXTERNAL_PHY_PORT1      1
#define MAC1_EXT_PHY_MASK       0x00040000
#define MAC1_INT_PHY_MASK       0x80000000
#define EXTERNAL_PHY_RESET_GPIO 6
#define CONF_CACHE      CACHE_WT

#else
#error "Fatal Error: No board type defined"
#endif /* ifdef AVALANCHE_EVM3 */


/* Phy mask definitions */

#if defined(TNETD73XX_BOARD)
#define MAC0_PHY_MASK       0x80000000
#endif

#if defined (AR7DB) || defined (AR7VDB)
#define MAC1_PHY_MASK       0x55555555
#endif

#if defined (AR7RD) || defined (AR7Wi) || defined (AR7L0)
#define MAC1_PHY_MASK       0x00000002
#endif

#if defined (AR7WRD) || defined (AR7VWi)
#define MAC1_PHY_MASK       0x001E0000
#endif

#ifdef TNETV1050_BOARD
#define MAC0_PHY_MASK       0x40000000
#define MAC1_PHY_MASK       0x80000000
#endif

#if defined(SEAD2)
#define MAC0_PHY_MASK       0x55555555
#define MAC1_PHY_MASK       0xAAAAAAAA
#endif

#if defined(TNETV1020VDB)
#define MAC0_PHY_MASK       0x00000002
#define MAC1_PHY_MASK       0x001E0000
#endif

#if defined(TNETWA115VAG)
#define MAC0_PHY_MASK       0x00000000
#define MAC1_PHY_MASK       0x0000003F
#endif

#endif /* _SYSCONF_H_ */
