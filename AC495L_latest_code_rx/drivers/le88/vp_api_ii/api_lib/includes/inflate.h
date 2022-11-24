/*
 * inflate.h
 *
 * This file renames some types declared in zstructs.h.  The names used in
 * zstructs.h are chosen to avoid namespace collisions in the top-level
 * application.  The names chosen here are the names used in the official
 * zlib distribution.
 *
 * This code is based on zlib, a general purpose compression library,
 * Copyright (C) 1995-2003 Jean-loup Gailly and Mark Adler.
 * http://www.zlib.org
 *
 * This software is the property of Legerity , Inc. Please refer to the
 * Non Disclosure Agreement (NDA) that you have signed for more information
 * on legal obligations in using, modifying or distributing this file.
 */

#ifndef _INFLATE_H
#define _INFLATE_H

#include "zstructs.h"

typedef VpZlibStreamType z_stream;
typedef VpZlibCodeType zcode;
typedef VpZlibInflateModeType inflate_mode;
#define inflate_state VpZlibInflateStateType

#endif /* _INFLATE_H */



