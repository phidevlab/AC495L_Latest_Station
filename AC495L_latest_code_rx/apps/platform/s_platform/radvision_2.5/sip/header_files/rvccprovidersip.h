/******************************************************************************
Filename:    rvccprovidersip.h
Description: SIP Provider Header
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
#ifndef RV_CCPROVIDER_SIP_H
#define RV_CCPROVIDER_SIP_H

#include "rvmap.h"
#include "rvmutex.h"
#include "rvvector.h"

#include "rvmdm.h"
#include "rvccapi.h"
#include "rvcccall.h"
#include "inc.h"
#include "sipphone.h"
#include "sipMgr.h"

#include "rvSipControlApi.h"
#include "rvmdmmediastream.h"

typedef struct RvCCProviderSip_ {
	RvSipControl			sipMgr;
	RvCCCallMgr*			callMgr;
	RvMap(RvCharPtr,RvCCTerminalPtr)		terminals;
	char					localAddress[RV_SIPCTRL_ADDRESS_SIZE];	
	RvBool					active;
	RvCCConnectionFuncs		connFuncs;
	RvCCConnectionClbks		connClbks;
	RvMutex					lock;		  
	RvAlloc*				alloc;
	RvMdmStreamDescriptor*	mediaCaps;
	RvVector(RvMdmMediaStreamInfo)       streamList;
	RvMutex					mutex;
    RvInt32                 referTimeout;
	RvCallWaitingReply      callWaitingReply;
	RvUint32				watchdogTimeout;
	IppTimer				watchdogTimer;
} RvCCProviderSip;

void rvCCProviderSipConstruct(RvCCProvider* p, RvCCCallMgr* callMgr, 
							  RvSipStackHandle stackHandle,
							  RvIppSipPhoneCfg* cfg, RvAlloc* a);
void rvCCProviderSipDestruct(RvCCProvider* p);
void rvCCProviderSipStart(RvCCProvider* x);
void rvCCProviderSipStop(RvCCProvider* x);
RvCCTerminal* rvSipProviderRegisterTerm(RvCCProvider*						x,
									 const char*					id,
									 RvCCTerminalType				type,
									 RvMdmTermDefaultProperties*	termProperties);

void rvSipProviderUnregisterTerm(RvCCProvider*		x,
								 RvCCTerminal*		t,
								 RvCCTerminalType	type);

RvCCTerminal* rvCCProviderSipCreateTerminal(RvCCProvider*					x, 
									 RvCCTerminalType				type, 
									 const char *					id,
									 RvMdmTermDefaultProperties*	termProperties);
RvCCConnection* rvCCProviderSipCreateConnection(RvCCProvider* x, RvCCTerminal* t);
RvCCTerminal* rvCCProviderSipFindTerminalById(RvCCProvider* x, const char* termId);
RvCCTerminal* rvCCProviderSipFindTerminalByRegClient(RvCCProvider*			x, 
												 RvSipRegClientHandle	regClientObject);

void rvCCProviderSipSetMediaCaps(RvCCProvider* x, const RvMdmStreamDescriptor* media);


/*Application will register only those 2 clbks, the rest will be implemented by Call*/
void rvCCProviderSipRegisterAddressAnalyzeCB(RvCCProvider* x, RvCCConnectionAddressAnalyzeCB addressAnalyzeCB);
void rvCCProviderSipRegisterInProcessCB(RvCCProvider* x, RvCCConnectionInProcessCB inProcessCB);

#define rvCCProviderSipGetImpl(p)			((RvCCProviderSip*)p->provider)
#define rvCCProviderSipGetMediaCaps(p)		(p->mediaCaps)
#define rvCCProviderSipGetLocalAddress(p)	((p)->localAddress)

RvMdmMediaStreamInfo * rvCCProviderSipGetMediaStream(RvCCProviderSip * x, RvUint32 streamId);
RvMdmStreamId rvCCProviderSipAddMediaStream(RvCCProviderSip * x, RvMdmMediaStreamInfo* media);
RvMdmStreamId rvCCProviderSipAddEmptyMediaStream(RvCCProviderSip * x);
void rvCCProviderSipClearMediaStream(RvCCProviderSip * x, RvUint32 streamId);

#define rvCCProviderSipGetNumOfStreams(x)		(rvVectorSize(&(x)->streamList))
#define rvCCProviderSipGetMediaStreamElem(x,i)	rvVectorAt(&(x)->streamList,i)
#define rvCCProviderSipGetCallMgr(x)			((rvCCProviderSipGetImpl(x))->callMgr)
#define rvCCProviderSipGetSipMgr(x)				(&(rvCCProviderSipGetImpl(x))->sipMgr)

RvBool rvCCProviderSipHandleReinviteResponse(RvCCConnection*				c,
											 RvSipMsgHandle					hMsg,
										     RvSipCallLegStateChangeReason	eReason);


void rvCCProviderSipProcessEvent(
    RvSipAppCallLegHandle           hAppCallLeg,
    RvSipCallLegHandle              hCallLeg,
    RvSipCallLegState               state,
    RvSipCallLegStateChangeReason   reason);

void rvCCProviderSipProcessEventModify(
    RvSipAppCallLegHandle           hAppCallLeg,
    RvSipCallLegHandle              hCallLeg,
    RvSipCallLegModifyState         eModifyState,
    RvSipCallLegStateChangeReason   reason);

RvSipAppCallLegHandle rvCCProviderSipNewCallLeg(void);

void rvCCProviderSipSetCallParams(
    RvSipAppCallLegHandle   hAppCallLeg,
    RvSipCallLegHandle      hCallLeg,
    char*                   to,
    char*                   from,
    char*                   contact,
    char*                   callerName,
    char*                   callerDisplayName);

void rvCCProviderSipSetMedia(
    RvSipMsgHandle          hMsg,
    RvSipAppCallLegHandle   hAppCallLeg);

RvBool rvCCProviderSipGetMedia(
    RvSipMsgHandle          hMsg,
    RvSipAppCallLegHandle   hAppCallLeg);

void rvCCProviderSipMsgReceived(
    RvSipMsgHandle          hMsg,
    RvSipAppCallLegHandle   hAppCallLeg);

void rvCCProviderSipReferConnected(
    RvSipAppCallLegHandle   hAppCallLeg,
    RvSipAppCallLegHandle   hNewAppCallLeg);

void rvCCProviderSipTransferNotified(
    RvSipAppCallLegHandle   hAppCallLeg,
    RvBool                  isNotifyReceived);


#endif /* RV_CCPROVIDER_SIP_H*/
