/*****************************************************************************
Filename: rvalloccache.h
Description: Cache-driven allocator adaptor
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
$Revision:$
$Date:$ 
$Author: Amir Shavit$
*****************************************************************************/
#ifndef RV_ALLOCCACHE_H
#define RV_ALLOCCACHE_H

#include "rvtypes.h"
#include "rvalloc.h"
#include "rvpoolnew.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* rvCacheAllocTlsInit() must be called before using this allocator,
   rvCacheAllocTlsFinish() has to be called afterwards. */
/* This allocator can only be used the threads created by the stack. */
extern RvAlloc rvAllocCacheTlsAlloc;


/********************************************************************************************
 * rvAllocCacheGlobalAcquire - Returns a reference to a the global instance of the thread-
 * safe cache-allocator. The rvAllocCache modules has a reference counter to the global 
 * instance of the allocator. The first call to 'rvAllocCacheGlobalAcquire' constructs the 
 * object.
 *
 * RETURN  : Pointer to thread-safe rvAllocCache instance, or NULL upon failure.
 */
RvAlloc * rvAllocCacheGlobalAcquire(void);

/********************************************************************************************
 * rvAllocCacheGlobalAcquire - Decreases the reference counter to the global instance of the
 * thread-safe cache-allocator. 'rvAllocCacheGlobalRelease' has to be called as many times as
 * 'rvAllocCacheGlobalAcquire' was called in order to destroy the global instance.
 */
void rvAllocCacheGlobalRelease(void);

/********************************************************************************************
 * rvCacheAllocTlsInit - Initialize the module's TLS support. Must be called prior to any use
 * of the global 'rvAllocCacheTlsSub' allocator.
 */
RvBool rvCacheAllocTlsInit(void);

/********************************************************************************************
 * rvCacheAllocTlsFinish - Finalizes the module's TLS support. After calling this function,
 * 'rvAllocCacheTlsSub' cannot be used.
 */
void rvCacheAllocTlsFinish(void);

/********************************************************************************************
 * rvAllocCacheEmitStats - Emits the cache-allocate memory usage statistics to the log.
 *
 * INPUT   : allocCache - Pointer to the cache-allocator whom stats are to be emited.
 *                        If this parameter is set to NULL, the global cache-allocator
 *                        will be used.
 */
void rvAllocCacheEmitStats(
	IN	RvAlloc *					allocCache);

	
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RV_ALLOCCACHE_H */
