/******************************************************************************
Filename:    rvccprovidermdm.h
Description: Mdm Provider Header
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
#ifndef RV_CCPROVIDER_MDM_H
#define RV_CCPROVIDER_MDM_H

#include "rvmap.h"
#include "rvqueue.h"
#include "rvthread.h"
#include "rvmutex.h"

#include "rvmdm.h"
#include "rvccapi.h"
#include "rvcccall.h"
#include "rvmdmmediastream.h"

typedef void (*RvCCProviderMdmNotifyEventCB)(void* data);



/* MDM PROVIDER*/
/*-------------*/
typedef struct RvCCProviderMdm_
{
	RvMap(RvCharPtr,RvCCTerminalPtr)	terminals;
	RvCCCallMgr*		                callMgr;
	RvChar				                localAddress[64];
	RvMdmTermMgr*		                mdmTermMgr;
	RvMdmTermClass		                mdmTermClass;
	RvBool				                active;
	RvCCConnectionFuncs                 connFuncs;
	RvCCConnectionClbks                 connClbks;
	RvMutex				                lock; /* Mutex (Semaphore) for this Term Mgr */
	RvMutex				                mutex;
	void*                               displayData;
	RvCCProviderMdmNotifyEventCB        notifyEventCB;
	void*                               notifyEventData;
	RvUint32                            dialToneDuration;
	RvBool				                outOfBandDtmf;
	RvAlloc*			                a;
	RvIppCfwCfg			                cfwCallCfg;
	
} RvCCProviderMdm;

void rvCCProviderMdmConstruct(RvCCProvider* p, RvCCCallMgr* callMgr,
							  RvMdmTermMgr* termMgr,
							  void			*callCfg, /*Used in case of cfw, otherwise, it's dummy */
							  const char* localAddress,
							  unsigned int dialToneDuration,
							  RvBool outOfBandDtmf, RvAlloc* a);

void rvCCProviderMdmDestruct(RvCCProvider* p);
void rvMdmProviderStart(IN RvMdmXTermMgr* x);
void rvMdmProviderStop(IN RvMdmXTermMgr * x);
RvCCConnection* rvCCProviderMdmCreateConnection(RvCCProvider* x, RvCCTerminal* t);
RvCCTerminal* rvCCProviderMdmCreateTerminal(RvCCProvider* x, RvCCTerminalType type,
											const char *id,
											RvCCTerminalClbks* clbks,
											RvMdmTermDefaultProperties* termProperties);
void rvCCProviderMdmRemoveTerminal(RvCCProvider* x, RvCCTerminal* t);
RvCCTerminal* rvCCProviderFindTerminalByTermId(RvCCProvider* x, const char* termId);
RvCCTerminal* rvCCProviderFindTerminalByAddress(RvCCProvider* x, const char* address);
RvCCTerminalType rvCCProviderMdmFindTerminalType(RvMdmTermClass * c, const char* id);
RVAPI RvCCTerminal* rvCCProviderFindTermByType(IN RvCCProvider*		x,
										 IN RvCCTerminalType	termType);
/* Find out if specific terminal exists in the mdm provider terminal list */
RvBool rvCCProviderMdmFindIfTerminalExist(RvCCProvider* x, RvMdmTerm *term);

RvCCTerminal* rvCCProviderMdmFindAnyTerminal(RvCCProvider* x);
RvCCTerminal* rvCCProviderMdmFindTerminalByNumber(RvCCProvider* x,const char* phoneNumber);

/*Those callbacks will be registered by the Application, the rest will be implemented by RvCCCall*/
void rvCCProviderMdmRegisterAddressAnalyzeCB(RvCCProvider* x, RvCCConnectionAddressAnalyzeCB addressAnalyzeCB);
void rvCCProviderMdmRegisterInProcessCB(RvCCProvider* x, RvCCConnectionInProcessCB inProcessCB);
void rvCCProviderMdmRegisterNewDigitCB(RvCCProvider* x, RvCCConnectionNewDigitCB newDigitCB);

#define rvCCProviderMdmGetImpl(p)			((RvCCProviderMdm*)p->provider)
#define rvCCProviderMdmGetTermMgr(p)		(rvCCProviderMdmGetImpl(p)->mdmTermMgr)
#define rvCCProviderMdmSetTermMgr(p, t)		(rvCCProviderMdmGetImpl(p)->mdmTermMgr=t)
#define rvCCProviderMdmGetCallMgr(p)		(rvCCProviderMdmGetImpl(p)->callMgr)
#define rvCCProviderMdmGetDialToneDuration(p)	(rvCCProviderMdmGetImpl(p)->dialToneDuration)
#define rvCCProviderMdmGetoutOfBandDtmf(p)	(rvCCProviderMdmGetImpl(p)->outOfBandDtmf)


void rvCCProviderMdmSetDisplayData(RvCCProvider* x,void* data);
void* rvCCProviderMdmGetDisplayData(RvCCProvider* x);

void rvCCProviderMdmProcessTerminationEvent(IN void* data);
/* Notify about a user event
   (used so user events can be processed in stack thread */
void  rvCCProviderMdmNotifyUserEvent(RvCCProvider* p);


RvMdmTerm* rvMdmProviderRegisterPhysTerm(
    IN RvMdmXTermMgr*               x,
    IN RvMdmTermClass*              c,
    IN const RvChar*                id,
    IN RvMdmTermDefaultProperties*  termProperties);
RvMdmTerm* rvMdmProviderRegisterPhysTermAsync(
    IN RvMdmXTermMgr*               x,
    IN RvMdmTermClass*              c,
    IN const RvChar*                id,
    IN RvMdmTermDefaultProperties*  termProperties,
    IN void*                        userData);

RvBool rvMdmProviderUnregisterTerm(IN RvMdmXTermMgr* x, IN RvMdmXTerm* xTerm);

void rvCCProviderMdmPrintMedia(const RvMdmStreamDescriptor* descr);
char* mapAddressToTermination(RvMdmTermMgr* mgr, const char* address);


#endif /* RV_CCPROVIDER_MDM_H*/
