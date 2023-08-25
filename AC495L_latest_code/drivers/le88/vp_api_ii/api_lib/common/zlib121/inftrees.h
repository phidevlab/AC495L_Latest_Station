/*
 * inftrees.h
 *
 * This code is based on zlib, a general purpose compression library,
 * Copyright (C) 1995-2003 Jean-loup Gailly and Mark Adler.
 * http://www.zlib.org
 *
 * This software is the property of Legerity , Inc. Please refer to the
 * Non Disclosure Agreement (NDA) that you have signed for more information
 * on legal obligations in using, modifying or distributing this file.
 */


#ifndef INFTREES_H
#define INFTREES_H

#define MAXD 154

/* Type of code to build for inftable() */
typedef enum {
    CODES,
    LENS,
    DISTS,
    CODE_TYPE_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} codetype;

extern int16 VpZlibInflateTable OF((codetype type, uint16 FAR *lens,
                             uint16 codes, zcode FAR * FAR *table,
                             uint16 FAR *bits, uint16 FAR *work));

#endif /* INFTREES_H */