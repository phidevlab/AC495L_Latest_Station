/*
 * zlib.h
 *
 * This file declares data structures used by the compression library which
 * need to be visible at the API level, so that the size of VpScratchMemType
 * can be determined.  A VpScratchMemType buffer is passed by the top-level
 * application to VpBootLoad().  This way, the application can manage its
 * own memory allocation.
 *
 * This code is based on zlib, a general purpose compression library,
 * Copyright (C) 1995-2003 Jean-loup Gailly and Mark Adler.
 * http://www.zlib.org
 */

/* zlib.h -- interface of the 'zlib' general purpose compression library
  version 1.2.1, November 17th, 2003

  Copyright (C) 1995-2003 Jean-loup Gailly and Mark Adler

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Jean-loup Gailly        Mark Adler
  jloup@gzip.org          madler@alumni.caltech.edu


  The data format used by the zlib library is described by RFCs (Request for
  Comments) 1950 to 1952 in the files http://www.ietf.org/rfc/rfc1950.txt
  (zlib format), rfc1951.txt (deflate format) and rfc1952.txt (gzip format).
*/

#ifndef ZLIB_H
#define ZLIB_H

#include "inflate.h"
#include "zconf.h"

#define ZLIB_VERSION "1.2.1.f-Legerity-VpAPI"
#define ZLIB_VERNUM 0x121f

typedef z_stream FAR *z_streamp;

#define Z_NO_FLUSH      0
#define Z_PARTIAL_FLUSH 1 /* will be removed, use Z_SYNC_FLUSH instead */
#define Z_SYNC_FLUSH    2
#define Z_FULL_FLUSH    3
#define Z_FINISH        4
#define Z_BLOCK         5

#define Z_OK            0
#define Z_STREAM_END    1
#define Z_NEED_DICT     2
#define Z_ERRNO        (-1)
#define Z_STREAM_ERROR (-2)
#define Z_DATA_ERROR   (-3)
#define Z_MEM_ERROR    (-4)
#define Z_BUF_ERROR    (-5)
#define Z_VERSION_ERROR (-6)

#define Z_NULL  0  /* for initializing zalloc, zfree, opaque */

extern int16 VpZlibInflate(z_streamp strm, int16 flush);
extern uint32 adler32(uLong adler, const Bytef *buf, uInt len);
extern int16 VpZlibInflateInit2(z_streamp strm, int16  windowBits,
					voidpf state, Bytef *win);

#endif /* ZLIB_H */
