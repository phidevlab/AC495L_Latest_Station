/******************************************************************************
Filename:    sipphone.h
Description: Sip Phone Application Header
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
#ifndef SIPPHONE_H
#define SIPPHONE_H

#include "rvalloc.h"
#include "rvmap.h"
#include "rvstring.h"

#include "rvcccall.h"
#include "rvccapi.h"
#include "rvccprovidersip.h"
#include "rvccconnsip.h"
#include "rvSipControlApi.h"
#include "basephone.h"

#define RV_DEFAULT_REFER_TIMEOUT    2000 /*Default timeout to wait for Notify*/

typedef struct
{
	RvAlloc* a;
	RvCCProvider		mdmProvider;
	RvCCProvider		sipProvider;
	RvBool              started;
	RvBool				autoRegister; /*When True, IPP TK will send Registration in startup, 
										     and Re-Registration every time the timer expires.
											 When False, the user will use IPP API to send Registration, 
											 IPP TK won't send Registration at all.*/
}RvCCSipPhone;

void rvCCSipPhoneDestruct(RvCCSipPhone* x);

RvCCProvider* rvCCSipPhoneGetSipProvider(void);
RvCCProvider* rvCCSipPhoneGetMdmProvider(void);
void rvCCSipPhoneSetMdmTermMgr(RvMdmTermMgr* mgr);
RvSipControl* rvCCSipPhoneGetSipMgr(void);

RvBool sipPhoneRegisterAllTermsToNetwork(RvCCSipPhone* sipPhone);
void rvCCSipPhoneSetFromAddress(RvSipAppCallLegHandle legHandle, char *longFrom);
void rvCCSipPhoneFinishTermUnregistrationProcess(RvCCTerminal*  t);

/***************************************************************************
 * rvCCSipPhoneIsCallingMyself
 * ------------------------------------------------------------------------
 * General: This function checks whether user called himself.
 *          User called himself if username equals termination id, and if
 *          address either equals local address or registrar address.
 *
 * ------------------------------------------------------------------------
 * Arguments:
 * input:   c           - connection
 *          address     - address
 *          destAddress - destination address to fill.
 * output:  reason      - event cause
 * Return Value: Returns True if this is a self call.
 ***************************************************************************/
RvBool rvCCSipPhoneIsCallingMyself(RvCCConnection* c, char* destAddress);

#endif /*SIPPHONE_H*/
