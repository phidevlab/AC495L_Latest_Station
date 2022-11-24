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
 * FILE PURPOSE:    Executable load related definitions
 *******************************************************************************
 * FILE NAME:       loadfile.h
 *
 * DESCRIPTION:     Executable load related definitions
 *
 * (C) Copyright 2003, Texas Instruments, Inc
 ******************************************************************************/

#ifndef _LOADFILE_H_
#define _LOADFILE_H_
#include <psbl/system.h>

typedef enum {
	SBL_STR_MODNOTSUPPORTED	= 0,
	SBL_STR_CKSUMERR		= 1,
} sbl_str;

typedef struct sbl_ext_fileformat {
	int  (*load_ram)(char *, u4, int, u4 *);
} sbl_ext_fileformat;

#define m_tibinary  ((sbl_ext_fileformat *)sbl_mods[TIBINARY_ID].m_p)
#define m_elf       ((sbl_ext_fileformat *)sbl_mods[ELF_ID].m_p)
#define TIZIP_FLAG      0x54

#endif /* _LOADFILE_H_ */
