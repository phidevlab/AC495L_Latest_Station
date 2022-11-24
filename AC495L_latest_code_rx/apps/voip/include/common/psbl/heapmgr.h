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
 * FILE PURPOSE:    SBL Heap Manager file 
 *******************************************************************************
 * FILE NAME:       heapmgr.h
 *
 * DESCRIPTION:     PSBL Heap Manager file, supporting malloc/free replicas.
 *
 * (C) Copyright 2003, Texas Instruments, Inc
 ******************************************************************************/

#ifndef _HEAPMGR_H_
#define _HEAPMGR_H_

#include <psbl/system.h>
#include <psbl/stddef.h>

#define TI_HMEM_ID   0x54494d
#define ALIGN_8(x)   (((x) + 7) & -8)
#define FREE         FALSE
#define USED         TRUE

typedef struct HMEM_DESC {
	u4                 state     :8;
	u4                 identifier:24;
	u4                 size;
	struct HMEM_DESC  *prev;	
	struct HMEM_DESC  *next;	
} HMEM_DESC;

typedef struct HMEM_CB {
	u1         *end;
	u1		   *base;
	HMEM_DESC  *desc_start;
	bool		initialized;
	u1			align_desc;
} HMEM_CB;

int _malloc_init(u1 *, u4); 
void *_malloc(u4);
void _free(void *);
void hmem_dbg(HMEM_DESC *);

#endif /* _HEAPMGR_H_ */
