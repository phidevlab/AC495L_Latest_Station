/******************************************************************************
Filename:    rvmdmtermevent.h
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
#ifndef RV_MDMTERMEVENT_H
#define RV_MDMTERMEVENT_H

#include "rvtimer.h"
#include "rvthread.h"

#include "rvccterminalmdm.h"
#include "rvmdmobjects.h"


RvBool rvMdmTermProcessObsEvent(RvMdmTerm* mdmTerm,const char* pkg,const char* id,
						  RvMdmMediaStream* media,RvMdmParameterList * args);

void  rvMdmDigitMapBuildDDEvComplete(RvMdmParameterList *parameters,
										const char * digitString,
										RvMdmDigitMapMatchType matchType,
										void* userData);
unsigned char rvMdmDigitMapTranslateDDEvent(const char * eventName, const RvMdmParameterList* args);

void rvMdmTermDigitMapTimerProcess(RvCCTerminalMdm * x);

void rvMdmTermProcessEvents(RvCCTerminalMdm * x,const RvMdmEventsDescriptor * eventsDescr);

RvBool rvMdmTermQueueObsEvent(RvMdmTerm* mdmTerm,const char* pkg,const char* id,
						  RvMdmMediaStream* media,RvMdmParameterList * args);
RvBool rvMdmTermQueueRegisterTermEvent(RvMdmTerm* mdmTerm);
RvBool rvMdmTermQueueUnregTermEvent(RvMdmTerm* mdmTerm);

void rvMdmTermProcessBlockedEvent(RvCCTerminal* x);


#endif /* RV_MEGTERMEVENT_H */

