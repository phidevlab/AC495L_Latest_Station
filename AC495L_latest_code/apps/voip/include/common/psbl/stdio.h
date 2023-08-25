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
 * FILE PURPOSE:    Standard Input/Output
 *******************************************************************************
 * FILE NAME:       stdio.h
 *
 * DESCRIPTION:     Standard Input/Output
 *
 * (C) Copyright 2003, Texas Instruments, Inc
 ******************************************************************************/

#ifndef _STDIO_H_
#define _STDIO_H_

#ifdef	_STANDALONE
#include <psbl/ffs.h>
#include <psbl/stddef.h>
#include <psbl/stdarg.h>

extern void* sys_malloc(size_t);
extern void* sys_realloc(void*,size_t);
extern void sys_free(void*);

#ifdef _UTILS_
#include "_ctype.h"
#define sys_printf printf
#define sys_sprintf sprintf
#define sys_malloc malloc
#define sys_realloc realloc
#define sys_free free
#define FILE FFS_FILE
#define SEEK_SET FFS_SEEK_SET
#define SEEK_CVR FFS_SEEK_CVR
#define SEEK_END FFS_SEEK_END
#endif /* _UTILS_ */

#define	NULL	0

#ifndef min
#define min(a,b) ((a>b)?b:a)
#endif

#else	/* NON-stand alone (i.e. there is an operating system underneath) */
#include <ctype.h>

#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#define sys_printf  printf
#define sys_sprintf sprintf
#define sys_malloc  malloc
#define sys_realloc realloc
#define sys_free    free

#endif /* _STANDALONE */

#define FFS_SEEK_SET  (0x0000)
#define FFS_SEEK_CUR  (0x0001)
#define FFS_SEEK_END  (0x0002)

#ifndef EOF
#define EOF    (-1)
#endif

#ifdef	_STANDALONE
FFS_FILE *fopen(const char *filename, const char *type);
size_t    fread(void *ptr, size_t size, size_t nitems, FFS_FILE *stream);
size_t    fwrite(const void *ptr, size_t size, size_t nitems, FFS_FILE *stream);
int       fseek(FFS_FILE *stream, int offset, int ptrname);
void      rewind(FFS_FILE *stream);
int       remove(const char *);
int       ftell(FFS_FILE *stream);
int       fflush(FFS_FILE *stream);
int       feof(FFS_FILE *stream);
int       fclose(FFS_FILE *stream);
void      PrintfRedirect(void (*afunction)(char *));
void      PrintfRestore();
char     *fgets(char *buffer, size_t size, FFS_FILE *stream);

/* Sio Library entries */
extern int SioVersion(void);
extern void SioInit(void);
extern void SioOutChar(char);
extern void SioOutStr(char *);
int SioTxEmpty();
extern void SioFlush(void);
extern int SioInChar(void);
extern int SioInCharCheck(char *);
extern void SioDump(void);

/* Printf related routines. */
int sys_printf(const char *format, ...); 
int sys_sprintf(char *s, const char *format, ...);
int sys_vprintf(const char *format, va_list arg);
int sys_vsprintf(char *s, const char *format, va_list arg); 
int       sys_fprintf(FFS_FILE *fp, const char *format, ...);

#define getch 	SioInChar
#define getchar SioInChar
#define putchar SioOutChar
#endif /* _STANDALONE */

#define ARG_LEN 150

#endif /* _STDIO_H_ */
