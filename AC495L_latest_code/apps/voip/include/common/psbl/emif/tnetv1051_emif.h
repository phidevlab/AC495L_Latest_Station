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
**|         Copyright (c) 1998-2003 Texas Instruments Incorporated           |**
**|                        ALL RIGHTS RESERVED                               |**
**|                                                                          |**
**| Permission is hereby granted to licensees of Texas Instruments           |**
**| Incorporated (TI) products to use this computer program for the sole     |**
**| purpose of implementing a licensee product based on TI products.         |**
**| No other rights to reproduce, use, or disseminate this computer          |**
**| program, whether in part or in whole, are granted.                       |**
**|                                                                          |**
**| TI makes no representation or warranties with respect to the             |**
**| performance of this computer program, and specifically disclaims         |**
**| any responsibility for any damages, special or consequential,            |**
**| connected with the use of this program.                                  |**
**|                                                                          |**
**+--------------------------------------------------------------------------+**
*******************************************************************************/

/*******************************************************************************
 * FILE PURPOSE:    EMIF configuration for TNETV1051
 *******************************************************************************
 * FILE NAME:       tnetv1051_emif.h.h
 *
 * DESCRIPTION:     EMIF configuration for TNETV1051
 *
 * (C) Copyright 2003, Texas Instruments, Inc
 ******************************************************************************/

#define EMIF_ASYNC_WAITCYCLE_MAX_EXT_WAIT_3     0x01   
#define EMIF_ASYNC_WAITCYCLE_MAX_EXT_WAIT_2     0x01   
#define EMIF_ASYNC_WAITCYCLE_MAX_EXT_WAIT_1     0x01   
#define EMIF_ASYNC_WAITCYCLE_MAX_EXT_WAIT_0     0x01   
#define EMIF_ASYNC_CS5_WAIT     0x00   
#define EMIF_ASYNC_CS4_WAIT     0x00   
#define EMIF_ASYNC_CS3_WAIT     0x00   
#define EMIF_ASYNC_CS2_WAIT     0x00   
#define EMIF_ASYNC_MAX_EXT_WAIT 0x80   
#define EMIF_SDRAMCFG_SR        0x0   
#define EMIF_SDRAMCFG_PD        0x0   
#define EMIF_SDRAMCFG_PDWR      0x0   
#define EMIF_SDRAMCFG_DDR_DR_ST 0x0   
#define EMIF_SDRAMCFG_BIT_17_LOCK       0x0   
#define EMIF_SDRAMCFG_NM        0x1   
#define EMIF_SDRAMCFG_DISABLE_DDR_DLL   0x0   
#define EMIF_SDRAMCFG_BIT_13_LOCK       0x0   
#define EMIF_SDRAMCFG_CL        0x6   
#define EMIF_SDRAMCFG_BIT_11_9_LOCK     0x0   
#define EMIF_SDRAMCFG_IBANK     0x2   
#define EMIF_SDRAMCFG_EBANK     0x0   
#define EMIF_SDRAMCFG_PAGESIZE  0x2   
#define EMIF_SDRAMRCF_DDR_REFRESH_THRESH        0x00  
#define EMIF_SDRAM_REFRESH_INTERVAL     1040        /* Suraj changed 7800 to 1040 Value in Nanosecs */   
#define EMIF_ASYNC_BANK_1_SS    0   
#define EMIF_ASYNC_BANK_1_EW    0   
#define EMIF_ASYNC_BANK_1_W_SETUP       1   
#define EMIF_ASYNC_BANK_1_W_STROBE      26   
#define EMIF_ASYNC_BANK_1_W_HOLD        3   
#define EMIF_ASYNC_BANK_1_R_SETUP       1   
#define EMIF_ASYNC_BANK_1_R_STROBE      26   
#define EMIF_ASYNC_BANK_1_R_HOLD        3   
#define EMIF_ASYNC_BANK_1_TA    1   
#define EMIF_ASYNC_BANK_1_ASIZE 0   
#define EMIF_ASYNC_BANK_2_SS    0   
#define EMIF_ASYNC_BANK_2_EW    0   
#define EMIF_ASYNC_BANK_2_W_SETUP       1   
#define EMIF_ASYNC_BANK_2_W_STROBE      26   
#define EMIF_ASYNC_BANK_2_W_HOLD        3   
#define EMIF_ASYNC_BANK_2_R_SETUP       1   
#define EMIF_ASYNC_BANK_2_R_STROBE      26   
#define EMIF_ASYNC_BANK_2_R_HOLD        3   
#define EMIF_ASYNC_BANK_2_TA    1   
#define EMIF_ASYNC_BANK_2_ASIZE 2   
#define EMIF_ASYNC_BANK_3_SS    0   
#define EMIF_ASYNC_BANK_3_EW    0   
#define EMIF_ASYNC_BANK_3_W_SETUP       1   
#define EMIF_ASYNC_BANK_3_W_STROBE      26   
#define EMIF_ASYNC_BANK_3_W_HOLD        3   
#define EMIF_ASYNC_BANK_3_R_SETUP       1   
#define EMIF_ASYNC_BANK_3_R_STROBE      26   
#define EMIF_ASYNC_BANK_3_R_HOLD        3   
#define EMIF_ASYNC_BANK_3_TA    1   
#define EMIF_ASYNC_BANK_3_ASIZE 0   
#define EMIF_ASYNC_BANK_4_SS    0   
#define EMIF_ASYNC_BANK_4_EW    0   
#define EMIF_ASYNC_BANK_4_W_SETUP       1   
#define EMIF_ASYNC_BANK_4_W_STROBE      26   
#define EMIF_ASYNC_BANK_4_W_HOLD        3   
#define EMIF_ASYNC_BANK_4_R_SETUP       1   
#define EMIF_ASYNC_BANK_4_R_STROBE      26   
#define EMIF_ASYNC_BANK_4_R_HOLD        3   
#define EMIF_ASYNC_BANK_4_TA    1   
#define EMIF_ASYNC_BANK_4_ASIZE 2   
#define EMIF_SDRAMTR_T_RFC      0xa   
#define EMIF_SDRAMTR_T_RP       0x2   
#define EMIF_SDRAMTR_T_RCD      0x2   
#define EMIF_SDRAMTR_T_WR       0x2   
#define EMIF_SDRAMTR_T_RAS      0x6   
#define EMIF_SDRAMTR_T_RC       0x9   
#define EMIF_SDRAMTR_T_RRD      0x1   
#define EMIF_DDRSR_PHY_DLL_READY        0x1   
#define EMIF_DDRSR_DDR  0x1   
#define EMIF_DDRPCR_DDR_PHY_CTR 0x294A8A06    
#define EMIF_SDRTSAR_TOTAL_ACCESSES     0x00   
#define EMIF_SDRAMTSAR_TOTAL_ACTIVATE   0x00   
#define EMIF_DDRTPRR_DDR_PHY_ID_REV     0x00   
#define EMIF_SDRAMARETR_T_XS    0x19   
#define EMIF_IRR_WR     0x00   
#define EMIF_IRR_LT     0x00   
#define EMIF_IRR_AT     0x00   
#define EMIF_IMR_WR_MASKED      0x00   
#define EMIF_IMR_LT_MASKED      0x00   
#define EMIF_IMR_AT_MASKED      0x00   
#define EMIF_IMSR_WR_MASK_SET   0x00   
#define EMIF_IMSR_LT_MASK_SET   0x00   
#define EMIF_IMSR_AT_MASK_SET   0x00   
#define EMIF_IMCR_WR_CLR        0x00   
#define EMIF_IMCR_LT_CLR        0x00   
#define EMIF_IMCR_AT_CLR        0x00   
#define EMIF_IOCR_IO_CTRL       0x00   
#define EMIF_IOSR_IO_STAT       0x00    


