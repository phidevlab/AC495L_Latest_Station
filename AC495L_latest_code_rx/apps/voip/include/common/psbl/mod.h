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
**|         Copyright (c) 1998-2008 Texas Instruments Incorporated           |**
**|                        ALL RIGHTS RESERVED                               |**
**|                                                                          |**
**+--------------------------------------------------------------------------+**
*******************************************************************************/

/*******************************************************************************
 * FILE PURPOSE:    PSBL optional module support.
 *******************************************************************************
 * FILE NAME:       mod.h
 *
 * DESCRIPTION:     PSBL optional module support.
 *
 * (C) Copyright 2003, Texas Instruments, Inc
 ******************************************************************************/

#ifndef _MOD_H_
#define _MOD_H_

#ifdef _STANDALONE
#include <psbl/ffs.h>
#include <psbl/stdio.h>
#endif

typedef struct _sbl_ext_fs {
    FFS_FILE* (*open)(const char *filename, const char *type);
    size_t    (*write)(const void *ptr, size_t size, size_t nitems, FFS_FILE *stream);
    int       (*close)(FFS_FILE *stream);
    size_t    (*read)(void *ptr, size_t size, size_t nitems, FFS_FILE *stream);
} sbl_ext_fs;

extern sbl_ext_fs* sbl_omod_fs;

#endif /* _MOD_H_ */
