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
**|                                                                          |**
**+--------------------------------------------------------------------------+**
*******************************************************************************/

/******************************************************************************
 * FILE PURPOSE:    Multi Channel Direct Memory Access (MC-DMA) Header
 ********************************************************************************
 * FILE NAME:       mcdma.h
 *
 * DESCRIPTION:     Platform and OS independent API for MCDMA Controller
 *
 * REVISION HISTORY:
 * 27 Nov 02 - PSP TII
 *
 *******************************************************************************/

#ifndef __MCDMA_H__
#define __MCDMA_H__

#define DMA_TIMEOUT 0xFFFF0000
#define ERR_DMA_ACTIVE      -1
#define ERR_INVALID_LENGTH  -2
#define ERR_INVALID_INPUT   -3

static UINT32 mcdma_base_address;

#define MCDMA_REG32_DATA(addr)              (*(volatile UINT32 *)(addr))
#define MCDMA_REG32_WRITE(addr, data)       MCDMA_REG32_DATA(addr) = data;
#define MCDMA_REG32_READ(addr, data)        data = (UINT32) MCDMA_REG32_DATA(addr);

/* MCDMA module definations */

#define D_OK 				  0
#define MCDMA_NUM_CHANNELS    4
#define MCDMA_CH_OFFSET       0x40
#define MCDMA0                (mcdma_base_address + 0x0)
#define MCDMA1                (mcdma_base_address + 0x40)
#define MCDMA2                (mcdma_base_address + 0x80)
#define MCDMA3                (mcdma_base_address + 0xC0)

#define MCDMA_SRC_ADDR        0x0
#define MCDMA_DST_ADDR        0x4
#define MCDMA_CONTROL         0x8

#define MCDMA_START_BIT       0x80000000 /* bit 31 */
#define MCDMA_STOP_MASK       0x7FFFFFFF /* bit 31 */
#define MCDMA_ACTIVE_BIT      0x40000000 /* bit 30 */

#define MCDMA_BURST_MODE_SHIFT	20
#define MCDMA_SRC_MODE_SHIFT	22
#define MCDMA_DST_MODE_SHIFT	24



typedef enum MCDMA_CHANNEL_tag
{
    MCDMA_CHANNEL_0 = 0,
    MCDMA_CHANNEL_1,
    MCDMA_CHANNEL_2,
    MCDMA_CHANNEL_3,

} MCDMA_CHANNEL_T;

typedef enum MCDMA_CTRL_tag
{
    MCDMA_STOP = 0,
    MCDMA_START

} MCDMA_CTRL_T;

typedef enum MCDMA_ADDR_MODE_tag
{
    MCDMA_INCREMENTING = 0,
    MCDMA_FIXED = 2

} MCDMA_ADDR_MODE_T;

typedef enum MCDMA_BURST_MODE_tag
{
    MCDMA_1_WORD_BURST = 0,
    MCDMA_2_WORD_BURST = 1,
    MCDMA_4_WORD_BURST = 2,

} MCDMA_BURST_MODE_T;


void mcdma_init(UINT32 base_addr);
UINT32 mcdma_control(MCDMA_CHANNEL_T mcdma_ch, MCDMA_CTRL_T mcdma_ctrl);
INT32 mcdma_setdmaparams (MCDMA_CHANNEL_T mcdma_ch,
                          UINT32 src_addr,
                          UINT32 dst_addr,
                          UINT32 length,
                          MCDMA_BURST_MODE_T burst_mode,
                          MCDMA_ADDR_MODE_T src_addr_mode,
                          MCDMA_ADDR_MODE_T dst_addr_mode);

#endif /* __MCDMA_H__ */
