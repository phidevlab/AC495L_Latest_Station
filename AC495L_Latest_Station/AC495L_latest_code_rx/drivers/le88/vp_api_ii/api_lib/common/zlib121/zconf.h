/*
 * zconf.h
 *
 * This code is based on zlib, a general purpose compression library,
 * Copyright (C) 1995-2003 Jean-loup Gailly and Mark Adler.
 * http://www.zlib.org
 *
 * This software is the property of Legerity , Inc. Please refer to the
 * Non Disclosure Agreement (NDA) that you have signed for more information
 * on legal obligations in using, modifying or distributing this file.
 */

#ifndef ZCONF_H
#define ZCONF_H

#include "vp_hal.h"

#define OF(args)  args
#define FAR
#define ZEXTERN extern
#define ZEXPORT
#define ZEXPORTVA

typedef uint8   Byte;  /* 8 bits */
typedef uint16  uInt;  /* 16 bits or more */
typedef uint32  uLong; /* 32 bits or more */

typedef Byte  FAR Bytef;
typedef uint8 FAR charf;
typedef int16 FAR intf;
typedef uInt  FAR uIntf;
typedef uLong FAR uLongf;

typedef void const *voidpc;
typedef void FAR   *voidpf;
typedef void       *voidp;

#endif /* ZCONF_H */
