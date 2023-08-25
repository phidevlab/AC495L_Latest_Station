/******************************************************************************
Filename:    rvccconnsip.h
Description: Sip Connection Header
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
#ifndef RV_CCCONN_SIP_H
#define RV_CCCONN_SIP_H

#include "rvalloc.h"
#include "rvstring.h"

#include "rvccapi.h"
#include "rvccprovidersip.h"
#include "rvmdmmediastream.h"
#include "sipMgr.h"

typedef struct
{
	RvCCProvider*			provider;
	RvCCTerminal*			terminal;
	RvSipCallLegHandle		callLegHndl;
	char					remoteAddress[RV_SIPCTRL_ADDRESS_SIZE];
	char					localAddress[RV_SIPCTRL_ADDRESS_SIZE];
    char                    fromAddress[RV_SIPCTRL_ADDRESS_SIZE]; /*address of EP from where the INVITE was sent*/
	char					callerId[RV_SIPCTRL_ADDRESS_SIZE];
    char					callerName[RV_SIPCTRL_ADDRESS_SIZE];
	char					callerNumber[RV_SIPCTRL_ADDRESS_SIZE];
	char					callerAddress[RV_SIPCTRL_ADDRESS_SIZE];
	RvMdmStreamId         	streamId;
	RvAlloc*				alloc;
	char					sdpAddress[RV_SIPCTRL_ADDRESS_SIZE]; /*Store ip address from SDP message for Unhold*/
	RvBool					rejectCall; /*In case message body is not valid*/
	IppTimer				referTimer;	/* After sending Refer, if no Notify message was received, this timer will disconnect the call-leg when it expires*/
	IppTimer				ringingTimer;	/* Ringing must be resent after 1 min. see RFC 3261: 13.3.1.1 Progress*/
    char                    nonce[RV_NAME_STR_SZ ];
} RvCCConnSip;


/* APIs */
/*======*/

void rvCCConnSipConstruct(RvCCConnection* x, RvCCProvider* p, RvCCTerminal* t, RvAlloc* a);
void rvCCConnSipDestruct(RvCCConnection* x);

#define rvCCConnSipGetIns(x)				((RvCCConnSip*)(x)->connection)
#define rvCCConnSipGetProvider(x)			((rvCCConnSipGetIns(x))->provider)
#define rvCCConnSipGetTerminal(x)			((rvCCConnSipGetIns(x))->terminal)
#define rvCCConnSipSetTerminal(x, a)		((rvCCConnSipGetIns(x))->terminal=a)

#define rvCCConnSipIsRejectCall(x)			((rvCCConnSipGetIns(x))->rejectCall)
#define rvCCConnSipSetRejectCall(x, a)		((rvCCConnSipGetIns(x))->rejectCall=a)
void rvCCConnSipProcessEvent(
                RvCCConnection*     conn, 
                RvCCTerminalEvent   event,
                RvBool*             callStillAlive, 
                RvCCEventCause      reason);
                             
void rvCCConnSipAccept(RvCCConnection* x,RvSdpMsg* inMedia);

void rvCCConnSipReject(RvCCConnection* x, RvCCEventCause reason);

void rvCCConnSipMakeCall(RvCCConnection* x,RvSdpMsg* inMedia);

/*Disconnect a connection and associated media.
Connection drops from the call.*/
void rvCCConnSipDisconnecting(RvCCConnection* x,RvCCEventCause reason);

/*Start a ringback in the connection
Connection must be in ESTABLISHED state
Ringback will stop in modify media or moving out of ESTABLISHED ?*/
void rvCCConnSipRingback(RvCCConnection* x, /*h.e RvSdpMsg* inMedia, */ RvCCEventCause reason);

RvStatus rvCCConnSipForwardCall(RvCCConnection* x, RvChar *sendToAddress);

/*Usually called by rvCCCallTransfer*/
void rvCCConnSipPark(RvCCConnection* oldConn, RvCCConnection* newConn);

void rvCCConnSipNotifyError(RvCCConnection *connection);

RvSdpMsg* rvCCConnSipGetMedia(RvCCConnection* x);
RvSdpMsg* rvCCConnSipGetMediaRemote(RvCCConnection* x);
RvSdpMsg* rvCCConnSipGetAllMediaCaps(RvCCConnection* x);

/*Note: should this function receive parameters from the application
or should they be set by the connection/provider ?*/
RvCCMediaState rvCCConnSipCreateMedia(RvCCConnection* x, RvSdpMsg* inMedia);

RvBool rvCCConnSipSetRemoteMedia(RvCCConnection* x,RvSdpMsg* inMedia);

void rvCCConnSipConnectMedia(RvCCConnection* x);

RvBool rvCCConnSipModifyMedia(RvCCConnection* x, RvMdmMediaStreamInfo* streamDescr);
void rvCCConnSipModifyMediaDone(RvCCConnection* x,
								RvBool status,
								RvMdmMediaStreamInfo* newMedia,
								RvMdmTermReasonModifyMedia reason);

RvBool rvCCConnSipCheckMedia(RvCCConnection* x);


void rvCCConnSipDisconnectMedia(RvCCConnection* x);

void rvCCConnSipCallAnswered(RvCCConnection* x, RvSdpMsg* inMedia);

void rvCCConnSipDisconnect(RvCCConnection* x);

RvBool rvCCConnSipHold(RvCCConnection* x);

RvBool rvCCConnSipUnhold(RvCCConnection* x);

RvBool rvCCConnSipRemoteHold(RvCCConnection* x);

RvBool rvCCConnSipRemoteUnhold(RvCCConnection* x);

const char* rvCCConnSipGetCallerId(RvCCConnection* x);
const char* rvCCConnSipGetCallerAddress(RvCCConnection* x);
const char* rvCCConnSipGetCallerName(RvCCConnection* x);
const char* rvCCConnSipGetCallerNumber(RvCCConnection* x,int index);
void rvCCConnSipSetCallerName(RvCCConnection* x, char* name);
void rvCCConnSipSetCallerNumber(RvCCConnection* x, char* name);

void rvCCConnSipSetCallerId(RvCCConnection* x, char* callerId);
void rvCCConnSipSetCallerAddress(RvCCConnection* x, char* address);

void rvCCConnSipTransferInit(RvCCConnection* x);
void rvCCConnSipTransferInProgress(RvCCConnection* x);

void rvCCConnSipPlayDtmf(IN RvCCConnection* x, IN RvDtmfParameters* dtmfParam);

void rvCCConnSipTerminate(RvCCConnection* x);

void rvCCConnSipInitiatedCB(RvCCConnection* x);
void rvCCConnSipDialingCB(RvCCConnection* x);
void rvCCConnSipCallDeliveredCB(RvCCConnection* x, RvCCEventCause reason);
RvCCMediaState rvCCConnSipOfferedCB(RvCCConnection* x,RvCCEventCause reason);
RvCCMediaState rvCCConnSipTransferInitCB(RvCCConnection* x);
RvCCMediaState rvCCConnSipTransferOfferedCB(RvCCConnection* x);
void rvCCConnSipMediaCreatedCB(RvCCConnection* x);

const RvSdpMsg* rvCCConnSipGetLocalMedia(RvCCConnection* x);
RvSdpMsg* rvCCConnSipGetMediaCaps(RvCCConnection* x);

void rvCCConnSipSetRemoteAddress(RvCCConnection* x, char* address);
void rvCCConnSipGetRemoteAddress(RvCCConnection* x);
void rvCCConnSipSetLocalAddress(RvCCConnection* x, char* address);

RvBool rvCCConnSipIsDisconnected(RvCCConnection* x);
void rvCCConnSipSetCallLegHandle(RvCCConnection* x, RvSipCallLegHandle hCallLeg);
RvSipCallLegHandle rvCCConnSipGetCallLegHandle(RvCCConnection* x);
void rvCCConnSipSetNonce(IN RvCCConnection* x,IN char* nonce);
const char* rvCCConnSipGetNonce(RvCCConnection* x);

/************************************************************************/
/* ACL NB -080219 Patch received from RV Incident #140823: Crash when   */
/* we toggle between 2 calls.(Tue 19-Feb-08)                            */
/************************************************************************/
void switchLocalAndRemote(RvMdmMediaStreamInfo* media);


#endif /* RV_CCCONN_SIP_H*/
