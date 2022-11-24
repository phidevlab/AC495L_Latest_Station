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
 * FILE PURPOSE:    Supporting Library to standard Library
 *******************************************************************************
 * FILE NAME:       support.h
 *
 * DESCRIPTION:     Supporting Library to standard Library
 *
 * (C) Copyright 2003, Texas Instruments, Inc
 ******************************************************************************/

                     
/* support.c calls */
#ifndef	___SUPPORT_H___
#define	___SUPPORT_H___

#include <psbl/stddef.h>
#include <psbl/stdio.h>
       
#ifdef _STANDALONE
void GetInputLine(char *str,int len,char *leagals);
int isalnum(int c);
int isspace(int c);
int islower(int c);
int isdigit(int c);
int isxdigit(int c);
int toupper(int c);
#endif /* _STANDALONE */
#endif
