
/* stuff in common between md4, md5, and sha1 */

#ifndef __hash_h__
#define __hash_h__

#include <stdlib.h>
#include <string.h>

#ifndef min
#define min(a,b) (((a)>(b))?(b):(a))
#endif

/* Vector Crays doesn't have a good 32-bit type, or more precisely,
   int32_t as defined by <bind/bitypes.h> isn't 32 bits, and we don't
   want to depend in being able to redefine this type.  To cope with
   this we have to clamp the result in some places to [0,2^32); no
   need to do this on other machines.  Did I say this was a mess?
   */

#ifdef _CRAY
#define CRAYFIX(X) ((X) & 0xffffffff)
#else
#define CRAYFIX(X) (X)
#endif

//static inline unsigned int cshift(int x, unsigned int n)
static unsigned int cshift(unsigned int x, unsigned int n)
{
    x = CRAYFIX(x);
    return CRAYFIX((x << n) | (x >> (32 - n)));
}

#endif /* __hash_h__ */
