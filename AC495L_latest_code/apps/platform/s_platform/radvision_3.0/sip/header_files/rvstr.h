#ifndef RV_STR_H
#define RV_STR_H

#include "rvtypes.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct {
    char* delimiters;
    char* p;
} RvStrTok;

/* Internal data structure used to store a pointer to a string and the length */
typedef struct {
    char * buf;
    int len;
} RvpStrPtrN;

RVAPI RvStrTok* rvStrTokConstruct(RvStrTok* t, char* delimiters, char* str);
#define rvStrTokDestruct(t)		
RVAPI char* rvStrTokGetToken(RvStrTok* t);
RVAPI void* rvStrTokGetCurPosition(RvStrTok* t);
RVAPI char* rvStrCopy(char* d, const char* s);
char* rvStrRevN(char* s, int n);
char *rvStrNcpyz(char *dst, const char *src, RvSize_t size);
int rvStrNicmp(const char *str1, const char *str2, RvSize_t n);
RVAPI int rvStrIcmp(const char *str1, const char *str2);
void rvStrToLower(char *str);
RVAPI char* rvStrFindFirstNotOf(char* str, char* delimiters);
RVAPI char* rvStrFindFirstOf(char* str, char* delimiters);

#if defined(__cplusplus)
}
#endif

#endif
