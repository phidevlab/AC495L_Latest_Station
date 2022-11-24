/******************************************************************************/
/*            TEXAS INSTRUMENTS INCORPORATED PROPRIETARY INFORMATION          */
/*   Created 2005, (C) Copyright 2005 Texas Instruments.  All rights reserved.*/
/*                                                                            */
/*  FILE    : nand.h                                                          */
/*  PURPOSE : Definitions for NAND                                            */
/*  PROJECT : DaVinci ROM boot-loader                                         */
/*  AUTHOR  : Vignesh LA                                                      */
/*  DATE    : April-25-2005                                                   */
/******************************************************************************/

#ifndef _NAND_H_
#define _NAND_H_

#ifdef _STANDALONE
#include <psbl/sysconf.h>
#else
/*#include "platform.h" */
#endif


/* NAND FLASH ADDRESS */
#ifdef _STANDALONE
#if defined(TNETV1051_BOARD)
#ifdef NAND_SBL
#define NAND_DATA_ADDR  ((volatile unsigned char*)0xb0000000)
#define NAND_ALE_ADDR   ((volatile unsigned char*)0xb0002000)
#define NAND_CMD_ADDR   ((volatile unsigned char*)0xb0004000)
#else
#define NAND_DATA_ADDR  ((volatile unsigned char*)0xbc000000)
#define NAND_ALE_ADDR   ((volatile unsigned char*)0xbc002000)
#define NAND_CMD_ADDR   ((volatile unsigned char*)0xbc004000)
#endif
#elif defined(VOLCANO_ARIES_BOARD)
#define NAND_DATA_ADDR  ((volatile unsigned char*)0xbd000000)
#define NAND_ALE_ADDR   ((volatile unsigned char*)0xbd002000)
#define NAND_CMD_ADDR   ((volatile unsigned char*)0xbd004000)
#else
#define NAND_DATA_ADDR  ((volatile unsigned char*)0xb0000000)
#define NAND_ALE_ADDR   ((volatile unsigned char*)0xb0002000)
#define NAND_CMD_ADDR   ((volatile unsigned char*)0xb0004000)
#endif

#else
extern volatile unsigned char * NAND_DATA_ADDR;
extern volatile unsigned char * NAND_ALE_ADDR;
extern volatile unsigned char * NAND_CMD_ADDR;
#endif


#define NAND_TYPE_MASK_0X00000020 (0x00000020)

/* NAND FLASH COMMANDS */
#define NAND_LO_PAGE        0
#define NAND_HI_PAGE        1
#define NAND_READ_30H       0x30
#define NAND_EXTRA_PAGE     0x50
#define NAND_RDID           0x90
#define NAND_RDIDADD        0x00
#define NAND_RST            0xFF
#define NAND_PGRM_START     0x80
#define NAND_PGRM_END       0x10
#define NAND_RDY            0x40
#define NAND_PGM_FAIL       0x01
#define NAND_BERASEC1       0x60
#define NAND_BERASEC2       0xD0
#define NAND_STATUS         0x70

/*STATUS OUTPUT*/
#define NAND_STATUS_SUCCESS 0xC0
#define NAND_TIMEOUT (5000*12)

#define MAX_NAND_SUPPORTED  (33)            /* Change in accordance with the device info */
#define UNKNOWN_NAND        (0xFF)          /* Unknown device id */
#define NAND_NOT_FOUND      (0)
#define NAND_FOUND          (1)

#define START_BLOCK_ALIGN  0
#define END_BLOCK_ALIGN 1
#define START_PAGE_ALIGN 2
#define END_PAGE_ALIGN 3
#define PAGE_SIZE_MASK      0x03
#define BLOCK_SIZE_MASK     0x30
#define SPARE_AREA_MASK     0x04


#define MAX_OOB_SIZE 512
#define MAX_PAGE_SIZE (16*1024)

#define NAND_START_OFFSET 0x0000000
#define NAND_END_OFFSET 0x8000000

#ifdef _STANDALONE
#if defined(NAND_SBL)
#define NAND_CHIP_SEL       (2)
#else
#define NAND_CHIP_SEL       (3)
#endif
#else
extern unsigned int NAND_CHIP_SEL;
#endif
#define NAND_CHIP_MASK      (0)

#define WR_CMD      0
#define RD_CMD      1
#define ER_CMD      2
#define EMIF_XFER_SIZE 512
#define ECC_START_OFFSET 40

typedef struct  {
    unsigned char dev_id;         /* DeviceID */
    unsigned char  p_cnt;         /* page count per block */
    unsigned short p_byte1;               /* byte count per page(include redundant parts)*/
    unsigned char blk_shift;      /* Number of bits by which block address is to be shifted */
    unsigned char addr_cycles;            /* Number of address cycles */
} NAND_DEVICE_INFO;

/***    NAND_info structure */
typedef struct  {
    unsigned char nand_width;      /* NAND width 1->16 bits 0->8 bits */
    unsigned char dev_code;        /* NAND_DevTable index */
    unsigned char page_cnt;        /* page count per block */
    unsigned char addr_cycles;     /* Number of address cycles */
    unsigned char big_block;       /* 1 - Big block device, 0 - small block device */
    unsigned short page_size;              /* Number of bytes in a page including extra data 264/528 */
    unsigned char blk_shift;       /* Number of bits by which block address is to be shifted */
    unsigned char page_shift;      /* Number of bits by which   page address is to be shifted */
    unsigned int blk_cnt;
    unsigned int next_page;
    unsigned int next_block;
} NAND_INFO;

typedef struct
{
 unsigned char maker_code;
 unsigned char device_code;
 unsigned short page_size;
 unsigned int block_size;
 unsigned char spare_area;
}FLASHDEF;

unsigned int get_nand_info ( FLASHDEF *nand_info );
int nand_read_with_ecc(unsigned int addr, unsigned int size, char* buf, char* oob_buf);
int nand_block_isbad ( unsigned int addr );
int nand_read_oob(unsigned int addr, unsigned int size, char *oob_buf);
int nand_write_oob(unsigned int addr, unsigned int size, char *oob_buf);
int nand_block_markbad(unsigned int addr);
int nand_write_with_ecc(unsigned int addr, unsigned int size, char *buf, char *oob_buf);
int nand_erase_block(unsigned int start_addr,unsigned int end_addr, bool erase_oob);
int nand_erase_all(unsigned int start_addr,unsigned int end_addr, bool erase_oob);

#define SAMSUNG_K9F1G08X0A

#define SAMSUNG_K9F1G08X0A_PAGE_SIZE 2048
#define SAMSUNG_K9F1G08X0A_NUM_PAGES_PER_BLOCK 64
#define SAMSUNG_K9F1G08X0A_BLOCK_SIZE (SAMSUNG_K9F1G08X0A_PAGE_SIZE * SAMSUNG_K9F1G08X0A_NUM_PAGES_PER_BLOCK)
#define SAMSUNG_K9F1G08X0A_NUM_OF_BLOCKS 1024
#define SAMSUNG_K9F1G08X0A_FLASH_SIZE (SAMSUNG_K9F1G08X0A_BLOCK_SIZE * SAMSUNG_K9F1G08X0A_NUM_OF_BLOCKS)
#define SAMSUNG_K9F1G08X0A_OOB_AREA 64

#ifdef SAMSUNG_K9F1G08X0A
#define NAND_PAGE_SIZE SAMSUNG_K9F1G08X0A_PAGE_SIZE
#define NAND_BLOCK_SIZE SAMSUNG_K9F1G08X0A_BLOCK_SIZE
#define OOB_SIZE SAMSUNG_K9F1G08X0A_OOB_AREA
#define FLASH_SIZE SAMSUNG_K9F1G08X0A_FLASH_SIZE
#define PAGES_PER_BLOCK SAMSUNG_K9F1G08X0A_NUM_PAGES_PER_BLOCK
#define NUM_OF_BLOCKS SAMSUNG_K9F1G08X0A_NUM_OF_BLOCKS
#define TOTAL_PAGE_SIZE (SAMSUNG_K9F1G08X0A_PAGE_SIZE + SAMSUNG_K9F1G08X0A_OOB_AREA)
#endif

#endif /*_NAND_H_ */
