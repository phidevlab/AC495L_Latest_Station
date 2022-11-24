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
 * FILE PURPOSE:    EMIF configuration for TNETV1050VDB
 *******************************************************************************
 * FILE NAME:       tnetv1050vdb_emif.h
 *
 * DESCRIPTION:     EMIF configuration for TNETV1050VDB
 *
 * (C) Copyright 2003, Texas Instruments, Inc
 ******************************************************************************/

#define EMIF_ASYNC_WAITCYCLE_MAX_EXT_WAIT   0x80
#define EMIF_ASYNC_WAITCYCLE_WAIT_POL        0x1

#define EMIF_SDRAMCFG_NM                0x0
#define EMIF_SDRAMCFG_PA                0x0
#define EMIF_SDRAMCFG_IBANK             0x2
#define EMIF_SDRAMCFG_EBANK             0x0
#define EMIF_SDRAMCFG_PAGESIZE          0x1
#define EMIF_SDRAM_REFRESH_INTERVAL    7800        /* Value in Nanosecs */


#define EMIF_ASYNC_BANK_1_SS            0x0
#define EMIF_ASYNC_BANK_1_EW            0x0    
#define EMIF_ASYNC_BANK_1_W_SETUP       0x1 
#define EMIF_ASYNC_BANK_1_W_STROBE     0x16 
#define EMIF_ASYNC_BANK_1_W_HOLD        0x7 
#define EMIF_ASYNC_BANK_1_R_SETUP       0x1
#define EMIF_ASYNC_BANK_1_R_STROBE     0x16
#define EMIF_ASYNC_BANK_1_R_HOLD        0x2
#define EMIF_ASYNC_BANK_1_TA            0x1
#define EMIF_ASYNC_BANK_1_ASIZE         0x2   

#define EMIF_ASYNC_BANK_2_SS            0x0
#define EMIF_ASYNC_BANK_2_EW            0x0    
#define EMIF_ASYNC_BANK_2_W_SETUP       0x1 
#define EMIF_ASYNC_BANK_2_W_STROBE     0x16 
#define EMIF_ASYNC_BANK_2_W_HOLD        0x7 
#define EMIF_ASYNC_BANK_2_R_SETUP       0x1
#define EMIF_ASYNC_BANK_2_R_STROBE     0x16
#define EMIF_ASYNC_BANK_2_R_HOLD        0x2
#define EMIF_ASYNC_BANK_2_TA            0x1
#define EMIF_ASYNC_BANK_2_ASIZE         0x2

#define EMIF_ASYNC_BANK_3_SS            0x0
#define EMIF_ASYNC_BANK_3_EW            0x0    
#define EMIF_ASYNC_BANK_3_W_SETUP       0x1 
#define EMIF_ASYNC_BANK_3_W_STROBE     0x16 
#define EMIF_ASYNC_BANK_3_W_HOLD        0x7 
#define EMIF_ASYNC_BANK_3_R_SETUP       0x1
#define EMIF_ASYNC_BANK_3_R_STROBE     0x16
#define EMIF_ASYNC_BANK_3_R_HOLD        0x2
#define EMIF_ASYNC_BANK_3_TA            0x1
#define EMIF_ASYNC_BANK_3_ASIZE         0x0

#define EMIF_ASYNC_BANK_4_SS            0x0
#define EMIF_ASYNC_BANK_4_EW            0x0    
#define EMIF_ASYNC_BANK_4_W_SETUP       0x1 
#define EMIF_ASYNC_BANK_4_W_STROBE     0x16 
#define EMIF_ASYNC_BANK_4_W_HOLD        0x7 
#define EMIF_ASYNC_BANK_4_R_SETUP       0x1
#define EMIF_ASYNC_BANK_4_R_STROBE     0x16
#define EMIF_ASYNC_BANK_4_R_HOLD        0x2
#define EMIF_ASYNC_BANK_4_TA            0x1
#define EMIF_ASYNC_BANK_4_ASIZE         0x2

