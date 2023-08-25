
/*
 * FILE PURPOSE:    EMIF configuration for TNETV1061SDB
 *
 * FILE NAME:       emif_petra.h
 *
 * DESCRIPTION:     EMIF configuration for TNETV1061SDB
 *
 */

#ifndef _EMIF_PETRA_H_
#define _EMIF_PETRA_H_

#ifdef _STANDALONE
#include <psbl/hw.h>
#else
#include "hw.h"
#endif

/*
 * EMIF ChipSelect bit in Nand Flash Ctrl Register.
 */
#ifndef CS_START_NUM
#define CS_START_NUM    0x2
#endif
#define CS_MAX_NUM      0x5
#define CS_MIN_NUM      0x0

#define CS_ECC_START_BIT 0x8

#ifndef ERROR
#define ERROR   -1
#endif

#ifndef OK
#define OK      0x0
#endif

/*
 * Values to be programmed depending on the CS on Petra board.
 */
#if defined(VOLCANO_ARIES_BOARD)
#define ASYNC_CONFIG_SEL_REG EMIF_ASYNC_CS4
#elif defined(TNETV1051_BOARD) && defined(VDB_VARIANT)
#define ASYNC_CONFIG_SEL_REG EMIF_ASYNC_CS0
#elif defined(TNETV1051_BOARD)
#ifdef NAND_SBL
#define ASYNC_CONFIG_SEL_REG EMIF_ASYNC_CS0
#else
#define ASYNC_CONFIG_SEL_REG EMIF_ASYNC_CS3
#endif
#else
#define ASYNC_CONFIG_SEL_REG EMIF_ASYNC_CS0
#endif

/*
 * Clk Freq in MHz
 */

#define EMIF_FULL_RATE  150
/*
 * Async Config Registers timing bit positions.
 */
#define ASYNC_w_setup_bit   26
#define ASYNC_w_strobe_bit  20
#define ASYNC_w_hold_bit    17
#define ASYNC_r_setup_bit   13
#define ASYNC_r_strobe_bit  7
#define ASYNC_r_hold_bit    4
#define ASYNC_asize_bit     0
#define ASYNC_asize_VAL     0

/*
 * Async Config Registers Mask Values.
 */
#define ASYNC_w_setup_MASK  (~(0x3C000000))
#define ASYNC_w_strobe_MASK (~(0x03F00000))
#define ASYNC_w_hold_MASK   (~(0x000E0000))
#define ASYNC_r_setup_MASK  (~(0x0001E000))
#define ASYNC_r_strobe_MASK (~(0x00001F80))
#define ASYNC_r_hold_MASK   (~(0x00000070))
#define ASYNC_asize_MASK    (~(0x00000003))

/*
 * Nand Flash Timing cycles
 */
typedef struct nand_timing_val
{
    unsigned int tCLS_VAL,
    tALS_VAL,
    tCS_VAL,
    tCLH_VAL,
    tALH_VAL,
    tCH_VAL,
    tWP_VAL;
}NAND_TIMING_CYCLES;



/*
 * Extern Declarations
 */
 extern void async_emif_init (NAND_TIMING_CYCLES *nand_time_cyles);
#endif /* _EMIF_PETRA_H_ */
