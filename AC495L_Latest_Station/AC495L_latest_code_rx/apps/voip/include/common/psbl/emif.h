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
 * FILE PURPOSE:    EMIF configuration header file.
 *******************************************************************************
 * FILE NAME:       emif.h
 *
 * DESCRIPTION:     EMIF configuration header file.
 *
 * (C) Copyright 2003, Texas Instruments, Inc
 ******************************************************************************/

/* some internal definitions - need not be modified */

#if defined(TNETV1061_BOARD) || defined(TNETC4700_BOARD) || defined(VOLCANO_ARIES_BOARD)
/* EMIF 2.1/2.2 Register Definitions */
#define SDR_SDRAM_REFRESH_ACCESS_MUL	11
#define DDR_SDRAM_REFRESH_ACCESS_MUL	8
/* some internal definitions - need not be modified */

#define EMIF_ASYNC_WAITCYCLE_MAX_EXT_WAIT_3_POS   31
#define EMIF_ASYNC_WAITCYCLE_MAX_EXT_WAIT_2_POS   30
#define EMIF_ASYNC_WAITCYCLE_MAX_EXT_WAIT_1_POS   29
#define EMIF_ASYNC_WAITCYCLE_MAX_EXT_WAIT_0_POS   28
#define EMIF_ASYNC_CS5_WAIT_POS					  22
#define EMIF_ASYNC_CS4_WAIT_POS					  20
#define EMIF_ASYNC_CS3_WAIT_POS					  18
#define EMIF_ASYNC_CS2_WAIT_POS					  16
#define EMIF_ASYNC_MAX_EXT_WAIT_POS			      0


#define EMIF_SDRAMCFG_SR_POS                31
#define EMIF_SDRAMCFG_PD_POS                30
#define EMIF_SDRAMCFG_PDWR_POS              29
#define EMIF_SDRAMCFG_DDR_DR_ST_POS         17
#define EMIF_SDRAMCFG_BIT_17_LOCK_POS       16
#define EMIF_SDRAMCFG_NM_POS                14
#define EMIF_SDRAMCFG_DISABLE_DDR_DLL_POS   13
#define EMIF_SDRAMCFG_BIT_13_LOCK_POS       12
#define EMIF_SDRAMCFG_CL_POS                9
#define EMIF_SDRAMCFG_BIT_11_9_LOCK_POS     8
#define EMIF_SDRAMCFG_IBANK_POS             4
#define EMIF_SDRAMCFG_EBANK_POS             3
#define EMIF_SDRAMCFG_PAGESIZE_POS          0


#define EMIF_SDRAMRCF_DDR_REFRESH_THRESH_POS   16
#define EMIF_SDRAMRFC_REFRESH_INTERVAL_POS     0

#define EMIF_ASYNC_BANK_SS_POS         31
#define EMIF_ASYNC_BANK_EW_POS         30
#define EMIF_ASYNC_BANK_W_SETUP_POS    26
#define EMIF_ASYNC_BANK_W_STROBE_POS   20
#define EMIF_ASYNC_BANK_W_HOLD_POS     17
#define EMIF_ASYNC_BANK_R_SETUP_POS    13
#define EMIF_ASYNC_BANK_R_STROBE_POS    7
#define EMIF_ASYNC_BANK_R_HOLD_POS      4
#define EMIF_ASYNC_BANK_TA_POS          2
#define EMIF_ASYNC_BANK_ASIZE_POS       0


#define EMIF_SDRAMTR_T_RFC_POS		        27
#define EMIF_SDRAMTR_T_RP_POS		        24
#define EMIF_SDRAMTR_T_RCD_POS		        20
#define EMIF_SDRAMTR_T_WR_POS		        16
#define EMIF_SDRAMTR_T_RAS_POS		        12
#define EMIF_SDRAMTR_T_RC_POS		        8
#define EMIF_SDRAMTR_T_RRD_POS		        4


#define EMIF_DDRSR_PHY_DLL_READY_POS        3
#define EMIF_DDRSR_DDR_POS		        	0

#define EMIF_DDRPCR_DDR_PHY_CTR_POS	        0

#define EMIF_DDRPSR_DDR_PHY_CTR_POS	        0

#define EMIF_SDRTSAR_TOTAL_ACCESSES_POS     0
#define EMIF_SDRAMTSAR_TOTAL_ACTIVATE_POS   0

#define EMIF_DDRTPRR_DDR_PHY_ID_REV_POS     0

#define EMIF_SDRAMARETR_T_XS_POS     0

#define EMIF_IRR_WR_POS     2
#define EMIF_IRR_LT_POS     1
#define EMIF_IRR_AT_POS     0


#define EMIF_IMR_WR_MASKED_POS     2
#define EMIF_IMR_LT_MASKED_POS     1
#define EMIF_IMR_AT_MASKED_POS     0


#define EMIF_IMSR_WR_MASK_SET_POS     2
#define EMIF_IMSR_LT_MASK_SET_POS     1
#define EMIF_IMSR_AT_MASK_SET_POS     0


#define EMIF_IMCR_WR_CLR_POS     2
#define EMIF_IMCR_LT_CLR_POS     1
#define EMIF_IMCR_AT_CLR_POS     0


#define EMIF_IOCR_IO_CTRL_POS     0

#define EMIF_IOSR_IO_STAT_POS     0

/* End of bit position definitions */
#else
/* EMIF 1.X Register Definitions */

#define EMIF_ASYNC_WAITCYCLE_MAX_EXT_WAIT_POS    0
#define EMIF_ASYNC_WAITCYCLE_WP_POS             30

#define EMIF_SDRAMCFG_NM_POS           14
#define EMIF_SDRAMCFG_PA_POS            7
#define EMIF_SDRAMCFG_IBANK_POS         4
#define EMIF_SDRAMCFG_EBANK_POS         3
#define EMIF_SDRAMCFG_PAGESIZE_POS      0

#define EMIF_ASYNC_BANK_SS_POS         31
#define EMIF_ASYNC_BANK_EW_POS         30
#define EMIF_ASYNC_BANK_W_SETUP_POS    26
#define EMIF_ASYNC_BANK_W_STROBE_POS   20
#define EMIF_ASYNC_BANK_W_HOLD_POS     17
#define EMIF_ASYNC_BANK_R_SETUP_POS    13
#define EMIF_ASYNC_BANK_R_STROBE_POS    7
#define EMIF_ASYNC_BANK_R_HOLD_POS      4
#define EMIF_ASYNC_BANK_TA_POS          2
#define EMIF_ASYNC_BANK_ASIZE_POS       0

#endif

/* EMIF configuration starts here. */

#if defined     (TNETV1050SDB)
#include <psbl/emif/tnetv1050sdb_emif.h>

#elif defined   (TNETV1050VDB)
#include <psbl/emif/tnetv1050vdb_emif.h>

#elif defined   (AR7DB)
#include <psbl/emif/ar7db_emif.h>

#elif defined   (AR7RD)
#include <psbl/emif/ar7rd_emif.h>

#elif defined   (AR7WRD)
#include <psbl/emif/ar7wrd_emif.h>

#elif defined   (AR7VWi)
#include <psbl/emif/ar7vwi_emif.h>

#elif defined   (AR7Wi)
#include <psbl/emif/ar7wi_emif.h>

#elif defined   (AR7VDB)
#include <psbl/emif/ar7vdb_emif.h>

#elif defined   (AR7L0)
#include <psbl/emif/ar7l0_emif.h>

#elif defined   (SEAD2)
#include <psbl/emif/sead2_emif.h>

#elif defined   (TNETV1020VDB)
#include <psbl/emif/tnetv1020vdb_emif.h>

#elif defined   (TNETWA115VAG)
#include <psbl/emif/tnetwa115vag_emif.h>

#elif defined   (TNETV1051_BOARD)
#include <psbl/emif/tnetv1051_emif.h>

#endif

