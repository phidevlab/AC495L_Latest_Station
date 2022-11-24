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
 * FILE PURPOSE:    Standard (Variable) arguments
 *******************************************************************************
 * FILE NAME:       stdarg.h
 *
 * DESCRIPTION:     Standard (Variable) arguments
 *
 * (C) Copyright 2003, Texas Instruments, Inc
 ******************************************************************************/

#ifndef __STDARG_H__
#define __STDARG_H__

#ifdef	_STANDALONE	/* For stand-alone (no OS) */
typedef char *va_list;
 /********************************************************************/
/* WARNING - va_arg will not work for "float" type, must use double */
/* ALSO NOTE THAT DOUBLES MUST BE DOUBLE WORD ALIGNED               */
/********************************************************************/
#define va_end(_ap)
  
#define va_start(_ap, _parmN) \
         (_ap = ((char *)&(_parmN)) + (sizeof(_parmN) < 4 ? 4 : sizeof(_parmN)))

#define va_arg(_ap, _type)                                       \
         ((sizeof(_type) == sizeof(double)                       \
             ? ((_ap = (void *)(((int)_ap + 7) & ~7)),           \
	        (_ap += 8), (*(_type *)(_ap - 8)))               \
	     : ((_ap += 4), (*(_type *)(_ap - 4)))))

#else
#include <stdarg.h>
#endif

#endif
