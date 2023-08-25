/** \file vp_debug.h
 * vp_debug.h
 *
 * This file contains the configuration and compile time settings for
 * the VoicePath API debug statements.
 *
 * Copyright (c) 2006, Legerity Inc.
 * All rights reserved
 *
 * This software is the property of Legerity , Inc. Please refer to the
 * Non Disclosure Agreement (NDA) that you have signed for more information
 * on legal obligations in using, modifying or distributing this file.
 */

#ifndef VP_DEBUG_H
#define VP_DEBUG_H

#include "vp_api_cfg.h"

#ifdef VP_DEBUG

/* VP-API Debug flags: */
#define DBG_ERROR       0x00000001 /* Any error condition */
#define DBG_WARNING     0x00000002 /* Any warning condition */
#define DBG_INFO        0x00000004 /* Un-categorized information */
#define DBG_API_FUNC    0x00000008 /* API I/F function enter and exit (except tick) */
#define DBG_HAL_FUNC    0x00000010 /* HAL I/F function enter and exit */
#define DBG_SYS_FUNC    0x00000020 /* SysService I/F function enter and exit */
#define DBG_TEST_FUNC   0x00000040 /* Test I/F function enter and exit */
#define DBG_INT_FUNC    0x00000080 /* Internal I/F function enter and exit */
#define DBG_TICK_FUNC   0x00000100 /* API Tick function */
#define DBG_MPI_TX      0x00000200 /* MPI transmit trace */
#define DBG_MPI_RX      0x00000400 /* MPI receive trace */
#define DBG_HBI_TX      0x00000800 /* HBI transmit trace */
#define DBG_HBI_RX      0x00001000 /* HBI receive trace */
#define DBG_EVENT       0x00002000 /* API event trace */
#define DBG_OPTION      0x00004000 /* Verbose option debug */
#define DBG_TEST        0x00008000 /* Verbose test debug */
#define DBG_API_INIT    0x00010000 /* API Initialization Code*/
#define DBG_FXO_FUNC    0x00020000 /* FXO Detection Code */

#define DBGSC_DEFAULT   DBG_ERROR | DBG_WARNING | DBG_TEST | DBG_TEST_FUNC | DBG_INFO

  #define VP_DOUT(flag, strings)                \
    if ((flag) & (dbgFlag))                     \
    {                                           \
        printf(" [%s]", #flag);                \
        printf##strings;                        \
        printf("\n");                           \
    }
#else
 #define VP_DOUT(flag, strings)
#endif

EXTERN uint32 dbgFlag; /* Decleration for the default flag variable */

#endif



