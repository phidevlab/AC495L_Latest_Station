/******************************************************************************
Filename:    rvmdmtermsignal.h
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
#ifndef RV_MDMTERMSIGNAL_H
#define RV_MDMTERMSIGNAL_H

#include "rvtimer.h"
#include "rvansi.h"

#include "rvmdmobjects.h"
#include "rvmdmtermmgrutil.h"
#include "rvmdm.h"

struct RvMdmTerm_ ;
struct RvCCTerminalMdm_;

RvBool rvMdmTermProcessNewSignals(struct RvCCTerminalMdm_ * x,const RvMdmSignalsDescriptor * signalsDescr);

void rvMdmTermEventStopSignals(struct RvCCTerminalMdm_  * x);

typedef struct
{
	struct RvCCTerminalMdm_ *	term;
	RvMdmSignalEx			signal;
	RvMdmTermTimer*				timer;
} RvMdmCurSignal;

RvMdmCurSignal* RvMdmCurSignalConstructCopy(RvMdmCurSignal* d,const RvMdmCurSignal* s,RvAlloc* a);
RvBool RvMdmCurSignalEqual(const RvMdmCurSignal* x,const RvMdmCurSignal* y);
#define RvMdmCurSignalDestruct       rvMdmCurSignalDestruct

rvDeclareList(RvMdmCurSignal)

typedef struct
{
	struct RvCCTerminalMdm_ *	term;
	RvMdmSignalExList		list;
	RvMdmTermTimer*				timer;
	unsigned int				curElem;
} RvMdmCurSignalList;

RvMdmCurSignalList* RvMdmCurSignalListConstructCopy(RvMdmCurSignalList* d,const RvMdmCurSignalList* s,RvAlloc* a);
RvBool rvMdmCurSignalListEqual(const RvMdmCurSignalList* x,const RvMdmCurSignalList* y);
void rvMdmCurSignalListDestruct(RvMdmCurSignalList* x);
#define RvMdmCurSignalListDestruct    rvMdmCurSignalListDestruct
#define RvMdmCurSignalListEqual       rvMdmCurSignalListEqual

void   rvMdmTermEventMoveSignals(struct RvCCTerminalMdm_* src,struct RvCCTerminalMdm_* dst);

rvDeclareList(RvMdmCurSignalList)

#endif /* RV_MDMTERMSIGNAL_H */
