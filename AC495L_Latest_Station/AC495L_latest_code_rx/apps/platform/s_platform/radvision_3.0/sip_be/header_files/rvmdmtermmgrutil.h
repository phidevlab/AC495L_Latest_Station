/******************************************************************************
Filename:    rvmdmtermmgrutil.h
Description:
*******************************************************************************
                Copyright (c) 2001 RADVISION
*******************************************************************************
NOTICE:
This document contains information that is proprietary to RADVISION.
No part of this publication may be reproduced in any form whatsoever
without written prior approval by RADVISION.

RADVISION reserves the right to revise this publication and make changes
without obligation to notify any person of such revisions or changes.
******************************************************************************/
#ifndef RV_MDMTERMMGRUTIL_H
#define RV_MDMTERMMGRUTIL_H

#include "rvtimer.h"
#include "rvmutex.h"
#include "rvmdmobjects.h"

/* TODO: This kind of timers should be ditched. They are not used enough 
   as an excuse to their existence */

/* Note: timers are not destructed, just invalidated.
   The callback function will be responsible of destructing and
   releasing the timer */
typedef void (*rvMdmTermTimerCB)(IN void*);
typedef struct
{
	IppTimer				  timer;
	RvBool				  valid;
	void*				  data;
	rvMdmTermTimerCB   func;
	unsigned int          loopCnt;
	RvMutex*              mutex;
	RvAlloc*			  alloc;
} RvMdmTermTimer;

RvMdmTermTimer* rvMdmTermTimerCreate(RvAlloc* alloc,RvMilliseconds ms,
										  rvMdmTermTimerCB func, void* data,
										  RvMutex* mutex);
/* Call AFTER acquiring callback */
void rvMdmTermTimerCancel(RvMdmTermTimer* t);

#endif /* RV_MDMTERMMGRUTIL_H */

