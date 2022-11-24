/******************************************************************************
Filename   : rvmdmtermdigitmap.h
Description: Structures and functions to save digitmap information
******************************************************************************
                Copyright (c) 2001 RADVision Inc.
************************************************************************
NOTICE:
This document contains information that is proprietary to RADVision LTD.
No part of this publication may be reproduced in any form whatsoever 
without written prior approval by RADVision LTD..

RADVision LTD. reserves the right to revise this publication and make 
changes without obligation to notify any person of such revisions or 
changes.
******************************************************************************/

#ifndef RV_MDMTERMDIGITMAP_H
#define RV_MDMTERMDIGITMAP_H

#include "rvalloc.h"
#include "rvstring.h"
/*#include "rvwildcard.h"*/
#include "rvmap.h"
#include "rvmutex.h"

#include "rvmdmobjects.h"
#include "rvmdm.h"
#include "rvccterminalmdm.h"

rvDeclareMap(RvIString,RvMdmDigitMap)

typedef struct RvMdmDigitMapDB_ {
	RvMap(RvIString,RvMdmDigitMap)	maps;
	RvMutex								mutex;	
} RvMdmDigitMapDB;

void rvMdmDigitMapDBConstruct(RvMdmDigitMapDB* db,RvAlloc* alloc);
void rvMdmDigitMapDBDestruct(RvMdmDigitMapDB* db);
void rvMdmDigitMapDBCopy(RvMdmDigitMapDB* dest,const RvMdmDigitMapDB* src);

/* Process the digitmap descriptor */
void rvMdmTermProcessDigitMap(struct RvCCTerminalMdm_* x,
							  const RvMdmDigitMapDescriptor * digitMapDescr);

/* Process a digitMap parameter in a requested event */
RvBool rvMdmTermProcessDigitMapParameter(struct RvCCTerminalMdm_ * x,
										 const RvMdmRequestedEvent* reqEvent,
										 RvBool activateDigitMap);

void rvMdmTermDeactivateDigitMap(struct RvCCTerminalMdm_ * x);
void rvMdmTermActivateDigitMap(struct RvCCTerminalMdm_ * x);
void rvMdmTermDigitMapTimerRestart(struct RvCCTerminalMdm_* x,RvMilliseconds time);
void rvMdmTermDigitMapTimerConstruct(struct RvCCTerminalMdm_* x);
void rvMdmTermDigitMapTimerDestruct(struct RvCCTerminalMdm_* x);

const RvMdmDigitMap* rvMdmTermGetDigitMap(struct RvCCTerminalMdm_ * x, const RvString* id);
void rvMdmTermAddDigitMap(struct RvCCTerminalMdm_ * x ,const RvMdmDigitMap* newMap, const RvString* id);


#endif /*RV_MEGTERMDIGITMAP_H*/
