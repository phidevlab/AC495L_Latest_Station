/*****************************************************************************
Filename: rvalloccanaries.h
Description: Allocator adaptor for detection of memory corruptions
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
$Author: Amir Shavit
*****************************************************************************/
#ifndef RV_ALLOCCANARIES_H
#define RV_ALLOCCANARIES_H

#include "rvtypes.h"
#include "rvalloc.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

RvAlloc * rvAllocCanariesConstruct(RvAlloc * alloc, RvAlloc * baseAlloc);
RvAlloc * rvAllocCanariesGetAllocator(RvAlloc * alloc);
void rvAllocCanariesDestruct(RvAlloc * alloc);
	
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RV_ALLOCCANARIES_H */
