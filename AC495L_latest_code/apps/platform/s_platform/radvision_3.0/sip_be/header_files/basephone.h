/******************************************************************************
Filename:    basephone.h
Description: Base Phone Header
*******************************************************************************
                Copyright (c) 2005 RADVISION
*******************************************************************************
NOTICE:
This document contains information that is proprietary to RADVISION.
No part of this publication may be reproduced in any form whatsoever 
without written prior approval by RADVISION.

RADVISION reserves the right to revise this publication and make changes
without obligation to notify any person of such revisions or changes.
******************************************************************************/
#ifndef BASEPHONE_H
#define BASEPHONE_H

#include "rvcccall.h"
#include "rvccapi.h"


typedef struct
{
	RvCCCallMgr		callMgr;
	RvCCProvider*	pmdmProvider;	/*initialized by Sip/H323 phone constructor*/
	RvCCProvider*	pnetProvider;	/*initialized by Sip/H323 phone constructor*/
}RvCCBasePhone;

void rvCCBasePhoneConstruct(RvCCBasePhone* x);

void rvCCBasePhoneDestruct(RvCCBasePhone* x);

/* 
 *	access to global object
 */
RvCCBasePhone*	rvCCBasePhoneGetBasePhone(void);

RvCCCallMgr*	rvCCBasePhoneGetCallMgr(void);
RvCCProvider*	rvCCBasePhoneGetMdmProvider(void);
RvCCProvider*	rvCCBasePhoneGetNetProvider(void);

void	rvCCBasePhoneSetMdmProvider(RvCCProvider* p);
void	rvCCBasePhoneSetNetProvider(RvCCProvider* p);

#endif /*BASEPHONE_H*/
