/*****************************************************************************
Filename: mtfalloc.h
Description: 
******************************************************************************
                      Copyright (c) 2006 RADVision Inc.
******************************************************************************
NOTICE:
This document contains information that is proprietary to RADVision Inc.
No part of this publication may be reproduced in any form whatsoever without
written prior approval by RADVision Inc.

RADVision Inc. reserves the right to revise this publication and make changes
without obligation to notify any person of such revisions or changes.
******************************************************************************
$Revision: $
$Date: $ 
$Author: Amir Shavit$
*****************************************************************************/

#ifndef __MTFALLOC_H__
#define __MTFALLOC_H__

#include "ipplog.h"

#ifdef RV_MTF_USE_CACHE_ALLOCATOR

#include "rvalloccache.h"

/* Initializes the cache allocator module  */
RVCOREAPI RvBool rvMtfAllocatorInit(void);


/* Returns a handle to the a TLS allocator */
RVCOREAPI RvAlloc* rvMtfAllocatorGet(void);

/* Finalizes the cache allocator module */
RVCOREAPI void rvMtfAllocatorFinish(void);

#endif /* RV_MTF_USE_CACHE_ALLOCATOR*/

#ifndef RV_MTF_USE_CACHE_ALLOCATOR

/* RvMemoryAlloc calls direct OS function, and will enable printing of unreleased memory in shutdown */
#define rvMtfAllocatorAlloc(size, p) \
(RvMemoryAlloc(NULL, size, IppLogMgr(), p));

#elif defined (RV_DEBUG)

/* rvAllocAllocate uses our Cache Allocator instead of direct OS calls */
#define rvMtfAllocatorAlloc(size, p) \
    {rvMtfIncreaseTotalAllocs(size); ((*p) = rvAllocAllocate(rvMtfAllocatorGet(), size));}

#else
#define rvMtfAllocatorAlloc(size, p) \
    ((*p) = rvAllocAllocate(rvMtfAllocatorGet(), size))   

#endif /*RV_MTF_USE_CACHE_ALLOCATOR */


/* Deallocates memory*/
RVCOREAPI RvBool rvMtfAllocatorDealloc(IN void *ptr, RvSize_t size);

#if defined (RV_DEBUG)
RVCOREAPI RvInt rvMtfGetTotalAllocsFromCacheAllocator();
RVCOREAPI void rvMtfIncreaseTotalAllocs(RvInt size);
#endif

#endif /* __MTFALLOC_H__ */
