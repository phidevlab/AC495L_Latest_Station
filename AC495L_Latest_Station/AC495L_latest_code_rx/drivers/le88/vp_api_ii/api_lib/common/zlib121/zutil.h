/*
 * zutil.h
 *
 * This code is based on zlib, a general purpose compression library,
 * Copyright (C) 1995-2003 Jean-loup Gailly and Mark Adler.
 * http://www.zlib.org
 *
 * This software is the property of Legerity , Inc. Please refer to the
 * Non Disclosure Agreement (NDA) that you have signed for more information
 * on legal obligations in using, modifying or distributing this file.
 */

#ifndef ZUTIL_H
#define ZUTIL_H

#define ZLIB_INTERNAL
#include "zlib.h"

#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#ifndef local
#  define local static
#endif
/* compile with -Dlocal if your debugger can't find static symbols */


/* Diagnostic functions */
#define Assert(cond,msg)
#define Trace(x)
#define Tracev(x)
#define Tracevv(x)
#define Tracec(c,x)
#define Tracecv(c,x)

#endif /* ZUTIL_H */
