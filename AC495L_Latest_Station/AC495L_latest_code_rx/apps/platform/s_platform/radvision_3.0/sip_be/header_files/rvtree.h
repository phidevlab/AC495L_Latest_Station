/******************************************************************************
Filename:    rvtree.h
Description: balanced binary tree class
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

#ifndef RV_COMMON_TREE_H
#define RV_COMMON_TREE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include "rvtypes.h"
#include "rvalloc.h"

struct RvTreeNode;

typedef void (*RvCopyConstructFunc)(void *, const void *, RvAlloc *);
typedef void (*RvDeleteFunc)(void *);
typedef RvBool (*RvCompFunc)(const void *, const void *);

typedef struct
{
	size_t keySize, valueSize;
	RvCopyConstructFunc keyConstructCopy, valueConstructCopy;
	RvDeleteFunc keyDestruct, valueDestruct;
	RvCompFunc keyLess, keyEqual;
} RvTreeInfo;

typedef struct
{
	struct RvTreeNode *tree;
	RvAlloc *allocator;
	const RvTreeInfo *info;
	size_t count;
} RvTree;

RVCOREAPI
RvTree *rvTreeConstruct(RvTree *tree, RvAlloc *allocator, const RvTreeInfo *info);
RVCOREAPI
void rvTreeDestruct(RvTree *tree);
RVCOREAPI
RvTree *rvTreeConstructCopy(RvTree *d, const RvTree *s, RvAlloc *a);
RVCOREAPI
RvTree *rvTreeCopy(RvTree *d, const RvTree *s);
RVCOREAPI
void rvTreeSetValue(RvTree *tree, const void *key, const void *value);
RVCOREAPI
const void *rvTreeGetValue(const RvTree *tree, const void *key);
RVCOREAPI
void *rvTreeFindValue(const RvTree *tree, const void *key);
RVCOREAPI
void rvTreeRemove(RvTree *tree, const void *key);
RVCOREAPI
size_t rvTreeSize(const RvTree *tree);
RVCOREAPI
void rvTreeClear(RvTree *tree);
RVCOREAPI
RvAlloc* rvTreeGetAllocator(const RvTree *tree);

typedef struct RvTreeNode *RvTreeIter;
typedef struct RvTreeNode *RvTreeRevIter;

RVCOREAPI
RvTreeIter rvTreeBegin(const RvTree *tree);
RVCOREAPI
RvTreeIter rvTreeEnd(const RvTree *tree);
RVCOREAPI
RvTreeRevIter rvTreeRevBegin(const RvTree *tree);
RVCOREAPI
RvTreeRevIter rvTreeRevEnd(const RvTree *tree);

/* Forward iterator */
RVCOREAPI
RvTreeIter rvTreeIterNext(RvTreeIter iter);
RVCOREAPI
RvTreeIter rvTreeIterPrev(RvTreeIter iter);
#define rvTreeIterEqual(a,b)     ((a) == (b))
#define rvTreeIterCopy(a,b)      ((a) = (b))
RVCOREAPI
const void *rvTreeIterGetKey(RvTreeIter iter);
RVCOREAPI
void *rvTreeIterGetValue(RvTreeIter iter, const RvTreeInfo *info);

/* Reverse iterator */
RVCOREAPI
RvTreeRevIter rvTreeRevIterNext(RvTreeRevIter iter);
RVCOREAPI
RvTreeRevIter rvTreeRevIterPrev(RvTreeRevIter);
#define rvTreeRevIterEqual(a,b)  ((a) == (b))
#define rvTreeRevIterCopy(a,b)   ((a) = (b))
RVCOREAPI
const void *rvTreeRevIterGetKey(RvTreeRevIter iter);
RVCOREAPI
void *rvTreeRevIterGetValue(RvTreeRevIter iter, const RvTreeInfo *info);



#ifdef __cplusplus
}
#endif

#endif
