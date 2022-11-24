/******************************************************************************
Filename:    rvmap.h
Description: wrapper to generate the functions for rvtree
*******************************************************************************
                Copyright (c) 2000 RADVision Inc.
*******************************************************************************
NOTICE:
This document contains information that is proprietary to RADVision LTD.
No part of this publication may be reproduced in any form whatsoever
without written prior approval by RADVision LTD..

RADVision LTD. reserves the right to revise this publication and make
changes without obligation to notify any person of such revisions or
changes.
******************************************************************************/

#ifndef RV_COMMON_MAP_H
#define RV_MAP_H

#include "rvtree.h"

#define rvDeclareMap(K, V) \
                                                                                \
typedef struct                                                                  \
 {                                                                              \
    RvTree tree;                                                                \
 } _RvMap##K##V;                                                                \
                                                                                \
typedef RvTreeIter _RvMapIter##K##V; \
typedef RvTreeRevIter _RvMapRevIter##K##V; \
                                                                                \
_RvMap##K##V *_rvMapConstruct##K##V(_RvMap##K##V *map, RvAlloc *a); \
void _rvMapSetValue##K##V(_RvMap##K##V *map, const K *key, const V *value); \
const V *_rvMapGetValue##K##V(const _RvMap##K##V *map, const K *key); \
V *_rvMapFindValue##K##V(_RvMap##K##V *map, const K *key); \
void _rvMapRemove##K##V(_RvMap##K##V *map, const K *key); \
_RvMapIter##K##V _rvMapBegin##K##V(const _RvMap##K##V *map); \
_RvMapIter##K##V _rvMapEnd##K##V(const _RvMap##K##V *map); \
_RvMapRevIter##K##V _rvMapRevBegin##K##V(const _RvMap##K##V *map); \
_RvMapRevIter##K##V _rvMapRevEnd##K##V(const _RvMap##K##V *map); \
const K *_rvMapIterGetKey##K##V(_RvMapIter##K##V iter); \
V *_rvMapIterGetValue##K##V(_RvMapIter##K##V iter); \
const K *_rvMapRevIterGetKey##K##V(_RvMapRevIter##K##V iter); \
V *_rvMapRevIterGetValue##K##V(_RvMapRevIter##K##V iter);


#define rvDefineMap(K, V) \
                                                                                \
void rvKeyConstructCopy##K##V(void *d, const void *s, RvAlloc *a)               \
{                                                                               \
    K##ConstructCopy((K *)d, (const K *)s, a);                                  \
}                                                                               \
                                                                                \
void rvValueConstructCopy##K##V(void *d, const void *s, RvAlloc *a)             \
{                                                                               \
    V##ConstructCopy((V *)d, (const V *)s, a);                                  \
}                                                                               \
                                                                                \
void rvKeyDestruct##K##V(void *x)                                               \
{                                                                               \
    K##Destruct((K *)x);                                                        \
}                                                                               \
                                                                                \
void rvValueDestruct##K##V(void *x)                                             \
{                                                                               \
    V##Destruct((V *)x);                                                        \
}                                                                               \
                                                                                \
RvBool rvKeyLess##K##V(const void *a, const void *b)                            \
{                                                                               \
    return K##Less((const K *)a, (const K *)b);                                 \
}                                                                               \
                                                                                \
RvBool rvKeyEqual##K##V(const void *a, const void *b)                           \
{                                                                               \
    return K##Equal((const K *)a, (const K *)b);                                \
}                                                                               \
                                                                                \
RvTreeInfo rvTreeInfo##K##V =                                                   \
{                                                                               \
    sizeof(K),                                                                  \
    sizeof(V),                                                                  \
    rvKeyConstructCopy##K##V,                                                   \
    rvValueConstructCopy##K##V,                                                 \
	rvKeyDestruct##K##V,                                                        \
    rvValueDestruct##K##V,                                                      \
    rvKeyLess##K##V,                                                            \
    rvKeyEqual##K##V                                                            \
};                                                                              \
                                                                                \
_RvMap##K##V *_rvMapConstruct##K##V(_RvMap##K##V *map, RvAlloc *a)              \
{                                                                               \
    rvTreeConstruct(&map->tree, a, &rvTreeInfo##K##V);                          \
    return map;                                                                 \
}                                                                               \
                                                                                \
void _rvMapSetValue##K##V(_RvMap##K##V *map, const K *key, const V *value)      \
{                                                                               \
     rvTreeSetValue(&map->tree, key, value);                                    \
}                                                                               \
                                                                                \
const V *_rvMapGetValue##K##V(const _RvMap##K##V *map, const K *key)            \
{                                                                               \
     return (const V *)rvTreeGetValue(&map->tree, key);                         \
}                                                                               \
                                                                                \
V *_rvMapFindValue##K##V(_RvMap##K##V *map, const K *key)                       \
{                                                                               \
     return (V *)rvTreeFindValue(&map->tree, key);                              \
}                                                                               \
                                                                                \
void _rvMapRemove##K##V(_RvMap##K##V *map, const K *key)                        \
{                                                                               \
     rvTreeRemove(&map->tree, key);                                             \
}                                                                               \
                                                                                \
_RvMapIter##K##V _rvMapBegin##K##V(const _RvMap##K##V *map)                     \
{                                                                               \
    return rvTreeBegin(&map->tree);                                             \
}                                                                               \
                                                                                \
_RvMapIter##K##V _rvMapEnd##K##V(const _RvMap##K##V *map)                       \
{                                                                               \
    return rvTreeEnd(&map->tree);                                               \
}                                                                               \
                                                                                \
_RvMapRevIter##K##V _rvMapRevBegin##K##V(const _RvMap##K##V *map)               \
{                                                                               \
    return rvTreeRevBegin(&map->tree);                                          \
}                                                                               \
                                                                                \
_RvMapRevIter##K##V _rvMapRevEnd##K##V(const _RvMap##K##V *map)                 \
{                                                                               \
    return rvTreeRevEnd(&map->tree);                                            \
}                                                                               \
                                                                                \
const K *_rvMapIterGetKey##K##V(_RvMapIter##K##V iter)                          \
{                                                                               \
    return (const K *)rvTreeIterGetKey(iter);                                   \
}                                                                               \
                                                                                \
V *_rvMapIterGetValue##K##V(_RvMapIter##K##V iter)                              \
{                                                                               \
    return (V *)rvTreeIterGetValue(iter, &rvTreeInfo##K##V);                    \
}                                                                               \
                                                                                \
const K *_rvMapRevIterGetKey##K##V(_RvMapRevIter##K##V iter)                    \
{                                                                               \
    return (const K *)rvTreeRevIterGetKey(iter);                                \
}                                                                               \
                                                                                \
V *_rvMapRevIterGetValue##K##V(_RvMapRevIter##K##V iter)                        \
{                                                                               \
    return (V *)rvTreeRevIterGetValue(iter, &rvTreeInfo##K##V);                 \
}



#define rvMapConstruct(K, V) _rvMapConstruct##K##V
#define rvMapDestruct(map) rvTreeDestruct(&(map)->tree)
#define rvMapConstructCopy(d, s, a) rvTreeConstructCopy(&(d)->tree, &(s)->tree, (a))
#define rvMapCopy(d, s) rvTreeCopy(&(d)->tree, &(s)->tree)
#define rvMapSetValue(K, V) _rvMapSetValue##K##V
#define rvMapGetValue(K, V) _rvMapGetValue##K##V
#define rvMapFindValue(K, V) _rvMapFindValue##K##V
#define rvMapRemove(K, V) _rvMapRemove##K##V
#define rvMapSize(map) rvTreeSize(&(map)->tree)
#define rvMapEmpty(map) (rvMapSize(map) == 0)
#define rvMapClear(map) rvTreeClear(&(map)->tree)
#define rvMapGetAllocator(map) rvTreeGetAllocator(&(map)->tree)
#define rvMapBegin(K, V) _rvMapBegin##K##V
#define rvMapEnd(K, V) _rvMapEnd##K##V
#define rvMapRevBegin(K, V) _rvMapRevBegin##K##V
#define rvMapRevEnd(K, V) _rvMapRevEnd##K##V

/* Forward iterator */
#define rvMapIterNext rvTreeIterNext
#define rvMapIterPrev rvTreeIterPrev
#define rvMapIterEqual(a,b) rvTreeIterEqual((a),(b))
#define rvMapIterCopy(a,b) rvTreeIterCopy((a),(b))
#define rvMapIterGetKey(K, V) _rvMapIterGetKey##K##V
#define rvMapIterGetValue(K, V) _rvMapIterGetValue##K##V

/* Reverse iterator */
#define rvMapRevIterNext rvTreeRevIterNext
#define rvMapRevIterPrev rvTreeRevIterPrev
#define rvMapRevIterEqual(a,b) rvTreeRevIterEqual((a),(b))
#define rvMapRevIterCopy(a,b) rvTreeRevIterCopy((a),(b))
#define rvMapRevIterGetKey(K, V) _rvMapRevIterGetKey##K##V
#define rvMapRevIterGetValue(K, V) _rvMapRevIterGetValue##K##V

#define RvMap(K, V) _RvMap##K##V
#define RvMapIter(K, V) _RvMapIter##K##V
#define RvMapRevIter(K, V) _RvMapRevIter##K##V


#endif
