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
 * FILE PURPOSE:    Flash Memory Manager Header
 *******************************************************************************
 * FILE NAME:       fmm.h
 *
 * DESCRIPTION:     Flash Memory Manager Header
 *
 * (C) Copyright 2003, Texas Instruments, Inc
 ******************************************************************************/

#ifndef _FMM_H_
#define _FMM_H_


#ifdef _STANDALONE
#include <psbl/stddef.h>
#else
#include "stddef.h"
#endif

int FMMInit();
int FMMSetSectionInfo(const char *section_tag, unsigned int base_address, \
                                                            unsigned int size);
int FMMAllocateSection(const char *section_tag, unsigned int request_size, unsigned int dualFlag);
int FMMGetSectionInfo(const char *section_tag, unsigned int *base, unsigned int *size);
int FMMDumpAllSections();
int FMMConfigSections();

#define FMM_PSPBOOT_TAG "PSBL"
#define FMM_ENV_TAG     "ENV"
#define FMM_FFS_TAG     "FFS"
#define FMM_DUAL_FFS_TAG     "DUALFFS"
#define FMM_SECONDARY_FFS_TAG "FFS2"

#endif /* _FMM_H_ */

