
#ifndef _MD5_H_
#define _MD5_H_


#include <stdlib.h>
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_BITYPES_H
#include <sys/bitypes.h>
#endif
#ifdef KRB5
#include <krb5-types.h>
#elif defined(KRB4)
#include <ktypes.h>
#endif


struct md5 {
  unsigned int sz[2];
  unsigned int counter[4];
  unsigned char save[64];
};

typedef struct md5 MD5_CTX;

#define MD5_DIGEST_LENGTH 16

void MD5_Init (struct md5 *m);
void MD5_Update (struct md5 *m, const void *p, size_t len);
void MD5_Final (void *res, struct md5 *m); /* unsigned int res[4] */

#endif

