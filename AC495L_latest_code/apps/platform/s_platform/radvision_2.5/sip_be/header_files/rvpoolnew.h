/*****************************************************************************
Filename: rvpoolnew.h
Description: Replacement candidate for rvPool.
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
#ifndef RV_RVPOOLNEW_H
#define RV_RVPOOLNEW_H

#include "rvtypes.h"
#include "rvalloc.h"
#include "rvlock.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define RV_POOLNEW_BIN_COUNT    (4)

struct RvPoolNewBlock_;
struct RvPoolNewPage_;

typedef struct RvPoolNewBlock_ {
    struct RvPoolNewBlock_* next; 
    struct RvPoolNewPage_ * page;
} RvPoolNewBlock;

typedef struct RvPoolNewPage_ {
    RvPoolNewBlock *        blockList;
    RvSize_t                availableBlocks;
    struct RvPoolNewPage_ * prev;
    struct RvPoolNewPage_ * next;
} RvPoolNewPage;

typedef void (*RvPoolNewCtorCB)(void * ptr, void * data);
typedef void (*RvPoolNewDtorCB)(void * ptr, void * data);


typedef struct RvPoolNew_ {
    RvSize_t        blockSize;
    RvSize_t        blocksPerPage;
    RvSize_t        realPageSize;
    RvPoolNewCtorCB ctor;
    RvPoolNewDtorCB dtor;
    void *          data;
    RvBool          freeUnusedPages;
    RvAlloc *       alloc;
    RvLock          lock;
    RvLogMgr *      logMgr;
    RvPoolNewPage * bins[RV_POOLNEW_BIN_COUNT+1];    
} RvPoolNew;

RvPoolNew* rvPoolNewConstruct(
    OUT     RvPoolNew *                     pool,
    IN      RvSize_t                        blockSize,
    IN      RvSize_t                        blocksPerPage,
    IN      RvPoolNewCtorCB                 ctor,
    IN      RvPoolNewDtorCB                 dtor,
    IN      void *                          data,
    IN      RvBool                          freeUnusedPages,
    IN      RvLogMgr *                      logMgr,
    IN      RvAlloc *                       alloc);

void rvPoolNewDestruct(
    IN      RvPoolNew *                     pool);

void * rvPoolNewAllocBlock(
    IN OUT  RvPoolNew *                     pool);

void rvPoolNewFreeBlock(
    IN OUT  RvPoolNew *                     pool,
    IN      void *                          ptr);


	
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RV_RVPOOLNEW_H */
