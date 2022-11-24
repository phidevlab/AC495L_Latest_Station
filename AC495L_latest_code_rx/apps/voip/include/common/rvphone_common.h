/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	rvphone_common.h                                    *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 26/02/08 - Coding started.                                       *
 *																	*
 ********************************************************************/

#ifndef _RVPHONE_COMMON_H_
#define _RVPHONE_COMMON_H_

#include "rvalloc.h"
#include "rvmap.h"
#include "rvstring.h"

#ifdef CONFIG_RG_VOIP_RV_SIP
#include "rvccprovidersip.h"
#include "rvcccall.h"
#include "rvccapi.h"
#endif
#ifdef CONFIG_RG_VOIP_RV_H323
#include "rvccproviderH323.h"
#endif

/* This file incorporates definitions from Radvision include files that would
 * otherwise cause multiple definitions, as they appear twice in two separate
 * header files that would otherwise need to be included when working with both
 * H323 and SIP: sipphone.h and H323phone.h */

#ifdef CONFIG_RG_VOIP_RV_SIP
void rvCCSipPhoneDestruct(RvCCSipPhone *x);

void rvCCSIPPhoneSystemInit(void);

RvCCProvider *rvCCSipPhoneGetSipProvider(void);
RvCCProvider *rvCCSipPhoneGetMdmProvider(void);
void rvCCSipPhoneSetMdmTermMgr(RvMdmTermMgr *mgr);
RvSipControl *rvCCSipPhoneGetSipMgr(void);

void rvIppSipPhoneConstruct(RvMdmTermMgr* mgr, RvIppSipPhoneCfg* cfg,
    RvSipStackHandle sipStack);
#endif

#ifdef CONFIG_RG_VOIP_RV_H323

#ifndef CONFIG_RG_VOIP_RV_SIP
rvDeclareMap(RvString, RvString)
#endif

typedef struct RvCCH323Phone_{
    RvAlloc *a;
    RvCCProvider mdmProvider;
    RvCCProvider h323Provider;
    RvCCCallMgr	callMgr;
    RvMap(RvString, RvString) addressList;
} RvCCH323Phone;

void rvCCH323PhoneSystemInit(void);
void rvCCH323PhoneConstruct(RvCCH323Phone *x, RvMdmTermMgr *termMgr,
    RvIppH323PhoneCfg *cfg, HAPP hApp,HSSEAPP hSSEApp);
void rvCCH323PhoneDestruct(RvCCH323Phone *x);
RvCCProvider *rvCCH323PhoneGetH323Provider(void);
RvCCProvider *rvCCH323PhoneGetMdmProvider(void);
void rvCCH323PhoneSetMdmTermMgr(RvMdmTermMgr *mgr);
void rvCCH323PhoneSetFromAddress(RvCCH323Phone *x, char *address);
RvH323Control *rvCCH323PhoneGetH323Mgr(void);

void rvIppH323PhoneConstruct(RvMdmTermMgr *mgr,
    RvIppH323PhoneCfg *cfg, HAPP hApp,HSSEAPP hSSEApp);
#endif

#endif
