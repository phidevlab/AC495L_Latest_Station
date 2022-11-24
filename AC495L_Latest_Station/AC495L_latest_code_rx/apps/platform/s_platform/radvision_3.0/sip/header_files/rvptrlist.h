/******************************************************************************
Filename    :
Description :
******************************************************************************
                Copyright (c) 1999 RADVision Inc.
************************************************************************
NOTICE:
This document contains information that is proprietary to RADVision LTD.
No part of this publication may be reproduced in any form whatsoever
without written prior approval by RADVision LTD..

RADVision LTD. reserves the right to revise this publication and make
changes without obligation to notify any person of such revisions or
changes.
******************************************************************************
$Revision:$
$Date:$
$Author: S. Cipolli$
******************************************************************************/

#ifndef RV_COMMON_PTRLIST_H
#define RV_COMMON_PTRLIST_H

#include "rvtypes.h"
#include "rvlist.h"

#if defined(__cplusplus)
extern "C" {
#endif

rvDeclareList(RvVoidPtr)
		
/* PtrList iterators */
#define RvPtrListIter				RvListIter(RvVoidPtr)
#define rvPtrListIterData(i)		((i)->data)
#define rvPtrListIterPrev(i)		((i)->prev)
#define rvPtrListIterNext(i)		((i)->next)

#define RvPtrListIterData(i)		((i)->data)
#define RvPtrListIterPrev(i)		((i)->prev)
#define RvPtrListIterNext(i)		((i)->next)

#define RvPtrListRevIter			RvListRevIter(RvVoidPtr)
#define rvPtrListRevIterData(i)		((i)->data)
#define rvPtrListRevIterPrev(i)		((i)->next)
#define rvPtrListRevIterNext(i)		((i)->prev)

#define RvPtrListRevIterData(i)		((i)->data)
#define RvPtrListRevIterPrev(i)		((i)->next)
#define RvPtrListRevIterNext(i)		((i)->prev)

/* Public PtrList interface */
typedef  RvList(RvVoidPtr) RvPtrList;


#define rvPtrListConstruct			rvListConstruct(RvVoidPtr)
#define rvPtrListConstructCopy		rvListConstructCopy(RvVoidPtr)
#define rvPtrListCopy				rvListCopy(RvVoidPtr)
#define rvPtrListDestruct			rvListDestruct(RvVoidPtr)
#define rvPtrListGetAllocator		rvListGetAllocator
#define rvPtrListSize				rvListSize
#define rvPtrListBegin				rvListBegin
#define rvPtrListEnd				rvListEnd
#define rvPtrListRevBegin			rvListRevBegin
#define rvPtrListRevEnd				rvListRevEnd
#define rvPtrListPopFront			rvListPopFront(RvVoidPtr)
#define rvPtrListPopBack			rvListPopBack(RvVoidPtr)
#define rvPtrListSplice				rvListSplice(RvVoidPtr)
#define rvPtrListErase				rvListErase(RvVoidPtr)
#define rvPtrListClear				rvListClear(RvVoidPtr)
#define rvPtrListSwap				rvListSwap(RvVoidPtr)
#define rvPtrListEqual				rvListEqual(RvVoidPtr)
#define rvPtrListRemoveIf			rvListRemoveIf(RvVoidPtr)
#define rvPtrListEmpty				rvListEmpty

/*RVCOREAPI RvVoidPtr rvPtrListFront(RvPtrList* l);
RVCOREAPI RvVoidPtr rvPtrListBack(RvPtrList* l);
RVCOREAPI void rvPtrListPushFront(RvPtrList* l, RvVoidPtr x);
*/
RVCOREAPI void rvPtrListPushBack(RvPtrList* l, RvVoidPtr x);
/*RVCOREAPI RvPtrListIter rvPtrListInsert(RvPtrList* l, RvPtrListIter i, RvVoidPtr x);
*/
RVCOREAPI RvPtrListIter rvPtrListRemove(RvPtrList* l, RvVoidPtr x);



#if defined(__cplusplus)
}
#endif

#endif
