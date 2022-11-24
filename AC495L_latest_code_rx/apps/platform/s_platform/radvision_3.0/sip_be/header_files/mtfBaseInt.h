/******************************************************************************
Filename:    mtfBaseInt.h
Description: This file includes internal functions for initialization and termination
			 of MTF.
*******************************************************************************
                Copyright (c) 2008 RADVISION
*******************************************************************************
NOTICE:
This document contains information that is proprietary to RADVISION.
No part of this publication may be reproduced in any form whatsoever
without written prior approval by RADVISION.

RADVISION reserves the right to revise this publication and make changes
without obligation to notify any person of such revisions or changes.
******************************************************************************/
#ifndef RV_RVMTFMGR_INT_H
#define RV_RVMTFMGR_INT_H

#include "rvMtfMediaApi.h"
#include "rvMtfExtControlTypes.h"
#include "rvMdmControlApi.h"

#ifdef RV_MTF_VIDEO
#include "rvMdmControlVideoApi.h"
#endif

typedef struct RvMtfBaseMgr_
{
	RvMdmTermMgr		    termMgr;
	RvSipStackHandle		sipStackHandle;
	RvMtfAppHandle		    userData;
	RvMdmTermClass*		    uiClass;
	RvMdmTermClass*		    phoneClass;
	RvMdmTermClass*		    atClass;
	RvMdmTermClass*		    rtpClass;
	RvMdmTerm*				handsetTermination;
	RvMdmTerm*				handsfreeTermination;
	RvMdmTerm*				headsetTermination;
    RvMtfMediaClbks         mediaClbks;
    RvMtfMdmClbks		    mdmClbks;

#ifdef RV_MTF_VIDEO
	RvMdmTerm*				cameraTermination;
	RvMdmTerm*				screenTermination;
    RvIppMdmVideoExtClbks   videoClbks;
#endif

} RvMtfBaseMgr;

RvMtfBaseMgr* rvGetMtfMgrByMdmTerm(RvMdmTerm* term);

RvMtfBaseMgr* rvGetMtfMgrByTermMgr(RvMdmTermMgr* termMgr);

RvBool rvStartSignalIntCB(
						RvMdmTerm*		term,
						RvMdmSignal *	s,
						RvMdmError*     mdmError);

RvBool rvPlaySignalIntCB(
						 IN  RvMdmTerm*          term,
						 IN  RvMdmSignal*        s,
						 IN  RvBool              reportCompletion,
						 OUT RvMdmError*         mdmError);

RvBool rvStopSignalIntCB(
						 RvMdmTerm*      term,
						 RvMdmSignal*    s,
						 RvMdmError*     mdmError);

void rvRegisterTermCompletedCB(
						IN struct RvMdmTerm_*   term,
						IN RvMdmError*          mdmError);

void rvUnregisterTermCompletedCB(
						IN struct RvMdmTerm_*   term,
						IN RvMdmError*          mdmError);

void rvUnregisterTermFromNetworkCompletedCB(
						IN struct RvMdmTerm_*   term,
						IN RvMdmError*          mdmError);

RvBool rvMapDialStringToAddressCB(
						RvMdmTerm*		term,
						const char*		dialString,
						char*			address);

RvChar* rvMapAddressToTerminationCB(
						RvMdmTermMgr*		termMgr,
						const RvChar*		address);

RvBool rvCreateMediaCB(
					   RvMdmTerm				*term,
					   RvMdmMediaStream			*media,
					   RvMdmMediaStreamDescr*	streamDescr);

RvBool rvModifyMediaCB(
						RvMdmTerm				*term,
						RvMdmMediaStream		*stream,
						RvMdmMediaStreamDescr	*streamDescr);

RvBool rvDestroyMediaCB(
						RvMdmTerm			*term,
						RvMdmMediaStream	*media);


RvBool rvConnectMediaTerminationsCB(
						RvMdmTermMgr *mgr,
						RvMdmTerm *source,
						RvMdmMediaStream *m1,
						RvMdmTerm *target,
						RvMdmMediaStream *m2,
						RvMdmStreamDirection direction);

RvBool rvDisconnectMediaTerminationsCB(
						RvMdmTermMgr *mgr,
						RvMdmTerm *source,
						RvMdmMediaStream *m1,
						RvMdmTerm *target,
						RvMdmMediaStream *m2);

void rvModifyMediaCompletedCB(
						struct RvMdmTerm_*		term,
						RvBool					status,
						RvMdmMediaDescriptor*	media,
						RvMdmMediaStreamDescr*	streamDescr,
						RvMdmTermReasonModifyMedia	reason);




#endif /*RV_RVMTFMGR_INT_H*/

