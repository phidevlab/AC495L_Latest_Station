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
 * FILE PURPOSE:    String library header
 *******************************************************************************
 * FILE NAME:       string.h
 *
 * DESCRIPTION:     String library header.
 *
 * (C) Copyright 2003, Texas Instruments, Inc
 ******************************************************************************/

#ifndef _STRING_H
#define _STRING_H

#ifdef _STANDALONE
#include <psbl/stddef.h>

extern char *strcat(char *s1, const char *s2);
extern char *strcpy(char *s1, const char *s2);
extern char *strncpy(char *s1, const char *s2, int len);
extern char *strchr(const char *s, int c);
extern int strcmp(const char *cs, const char *ct);
extern int strncmp(const char * cs,const char * ct,size_t count);
extern size_t strlen(const char *s);
extern char * strsep(char **s, const char * ct);
extern void memcpy(void *tgt, const void *src, size_t sz);
extern int memcmp(const void *cs, const void *ct, size_t n);
extern void memset(void *tgt, char val, size_t len);
extern char *strtok(char *s, int c);
#else
#include <string.h>
#endif
#endif /* _STRING_H */
