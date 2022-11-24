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
 * FILE PURPOSE:    PSBL optional module support for 7-ZIP
 *******************************************************************************
 * FILE NAME:       omod_7zip.h
 *
 * DESCRIPTION:     PSBL optional module support for 7-ZIP
 *
 * (C) Copyright 2003, Texas Instruments, Inc
 ******************************************************************************/

#ifndef OMOD_7ZIP_H
#define OMOD_7ZIP_H

#include <psbl/errno.h>
#include <psbl/system.h>

typedef struct 
{
	Status (*load_ram_7zip)(int, char **, u4 *);

} sbl_ext_7zip;

#define m_7zip ( ( sbl_ext_7zip *)sbl_mods[ ZIP7_ID ].m_p )

#define ZIP7_FLAG      0x37

#endif /* OMOD_LZMA_H */
