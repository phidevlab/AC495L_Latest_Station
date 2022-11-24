/*
 * zstructs.h
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
 *
 * This software is the property of Legerity , Inc. Please refer to the
 * Non Disclosure Agreement (NDA) that you have signed for more information
 * on legal obligations in using, modifying or distributing this file.
 */

#ifndef VP_ZLIB_STRUCTS_H
#define VP_ZLIB_STRUCTS_H

#include "vp_hal.h"     /* for definitions of basic types */

/* In the official zlib distribution, the following struct is called 'z_stream'
   and is defined in zlib.h. */
typedef struct VpZlibStreamStruct {
    VpImagePtrType next_in; /* next input byte */
    uint32 avail_in;        /* number of bytes available at next_in */
    uint32 total_in;        /* total nb of input bytes read so far */
    uint8p next_out;        /* next output byte should be put there */
    uint16 avail_out;       /* remaining free space at next_out */
    uint32 total_out;       /* total nb of bytes output so far */
    struct VpZlibInflateStateType *state; /* not visible by applications */
    uint16   data_type;     /* best guess about the data type: ascii or binary */
#ifdef VP_COMPRESS_TEST
    uint32   adler;         /* adler32 value of the uncompressed data */
#endif /* VP_COMPRESS_TEST */
} VpZlibStreamType;

/* In the official zlib distribution, the following struct is called 'code'
   and is defined in inftrees.h. */
typedef struct {
    uint8 op;               /* operation, extra bits, table bits */
    uint8 bits;             /* bits in this part of the code */
    uint16 val;             /* offset in table or code value */
} VpZlibCodeType;

/* In the official zlib distribution, the following enum is called
   'inflate_mode' and is defined in inflate.h. */
typedef enum {
    HEAD,       /* i: waiting for magic header */
    DICTID,     /* i: waiting for dictionary check value */
    DICT,       /* waiting for inflateSetDictionary() call */
        TYPE,       /* i: waiting for type bits, including last-flag bit */
        TYPEDO,     /* i: same, but skip check to exit inflate on new block */
        STORED,     /* i: waiting for stored size (length and complement) */
        COPY,       /* i/o: waiting for input or output to copy stored block */
        TABLE,      /* i: waiting for dynamic block table lengths */
        LENLENS,    /* i: waiting for code length code lengths */
        CODELENS,   /* i: waiting for length/lit and distance code lengths */
            LEN,        /* i: waiting for length/lit code */
            LENEXT,     /* i: waiting for length extra bits */
            DIST,       /* i: waiting for distance code */
            DISTEXT,    /* i: waiting for distance extra bits */
            MATCH,      /* o: waiting for output space to copy string */
            LIT,        /* o: waiting for output space to write literal */
    CHECK,      /* i: waiting for 32-bit check value */
    DONE,       /* finished check, done -- remain here until reset */
    BAD,        /* got a data error -- remain here until reset */
    MEM,        /* got an inflate() memory error -- remain here until reset */
    SYNC,       /* looking for synchronization bytes to restart inflate() */
    VP_ZLIB_INFLATEM_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpZlibInflateModeType;

/* Maximum size of dynamic tree.  The maximum found in a long but non-
   exhaustive search was 1004 code structures (850 for length/literals
   and 154 for distances, the latter actually the result of an
   exhaustive search).  The true maximum is not known, but the value
   below is more than safe. */
#define ENOUGH	1440

/* In the official zlib distribution, the following struct is called
   "inflate_state" and is defined in inflate.h. */
struct VpZlibInflateStateType {
    VpZlibInflateModeType mode; /* current inflate mode */
    int16 last;                 /* true if processing last block */
    int16 wrap;                 /* bit 0 true for zlib, bit 1 true for gzip */
    int16 havedict;             /* true if dictionary provided */
    int16 flags;                /* gzip header method and flags (0 if zlib) */
    uint32 check;               /* protected copy of check value */
    uint32 total;               /* protected copy of output count */
        /* sliding window */
    uint16 wbits;               /* log base 2 of requested window size */
    uint16 wsize;               /* window size or zero if not using window */
    uint16 whave;               /* valid bytes in the window */
    uint16 write;               /* window write index */
    uint8p window;              /* allocated sliding window, if needed */
        /* bit accumulator */
    uint32 hold;                /* input bit accumulator */
    uint16 bits;                /* number of bits in "in" */
        /* for string and stored block copying */
    uint16 length;              /* literal or length of data to copy */
    uint16 offset;              /* distance back to copy string from */
        /* for table and code decoding */
    uint16 extra;               /* extra bits needed */
        /* fixed and dynamic code tables */
    VpZlibCodeType const *lencode;  /* starting table for length/literal codes */
    VpZlibCodeType const *distcode; /* starting table for distance codes */
    uint16 lenbits;             /* index bits for lencode */
    uint16 distbits;            /* index bits for distcode */
        /* dynamic table building */
    uint16 ncode;               /* number of code length code lengths */
    uint16 nlen;                /* number of length code lengths */
    uint16 ndist;               /* number of distance code lengths */
    uint16 have;                /* number of code lengths in lens[] */
    VpZlibCodeType *next;       /* next available space in codes[] */
    uint16 lens[320];           /* temporary storage for code lengths */
    uint16 work[288];           /* work area for code table building */
    VpZlibCodeType codes[ENOUGH];   /* space for code tables */
    uint16 virgin;
    VpZlibCodeType fixed[544];
};

#endif /* !VP_ZLIB_STRUCTS_H */



