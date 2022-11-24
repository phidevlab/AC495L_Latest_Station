/******************************************************************************
Filename:    rvcccall.h
Description: Generic call API
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

#ifndef RV_CCCALL_H
#define RV_CCCALL_H

#include "rvptrvector.h"
#include "rvptrlist.h"
#include "rvccapi.h"


/*===================*/
/*	RvCCCall		 */
/*===================*/

typedef enum
{
	RV_CCCALL_TRANSFER_NONE,	/* 0 */
	RV_CCCALL_TRANSFER_BLIND,	/* 1 */
	RV_CCCALL_TRANSFER_SEMI,	/* 2 */
	RV_CCCALL_TRANSFER_FULL		/* 3 */
} RvCCCallTransferType;

typedef struct RvCCCall_
{
	void*					callMgr;
	RvPtrList				connections;	/*Connection is removed from this list after it gets "disconnect" event*/
	unsigned int			connNum;		/*Number of "live" connections (can be disconnected, but not deallocated)*/
	RvMutex					lock;
	RvCCCallState			state;
	RvCCCallTransferType	transferType; /* Transfer feature: Blind transfer, semi or full consultation transfer */
	RvCCTerminal*			conferenceControl;
	RvChar					originAddress[64];	/*For network calls only*/
	unsigned int			lockCnt;               /* counts how many times mutex were locked*/
    RvBool                  destructionCandidate;  /* indicates the call object should be destroyed when mutex is unlocked by all holders*/
} RvCCCall;

/*-----------------------------------------------------------------------
						C A L L		F U N C T I O N S
 ------------------------------------------------------------------------*/
#define rvCCCallGetConnNum(x) ((x)->connNum)
#define rvCCCallGetState(x) ((x)->state)
/* Transfer feature */
#define rvCCCallGetTransferType(x) ((x)->transferType)
#define rvCCCallSetTransferType(x,t) ((x)->transferType=(t))

void rvCCCallSetState(RvCCCall* call, RvCCCallState state);
#define rvCCCallGetCallMgr(x)	((x)->callMgr)

void rvCCCallSetOriginAddress(RvCCCall* call, RvChar* address);

/*Initiates a call from origConn to destConn. origConn must be in ESTABLISHED state
and destConn in IDLE state.*/
void rvCCCallConnect(
                RvCCConnection*     origConn, 
                RvCCConnection*     destConn);                

/*Gets all connections associated with this call */
/*RvPtrVectorIter rvCCCallGetConnections(RvCCCall* call);*/

/*Add an additional party to the call (one-step conferencing)*/
void rvCCCallAddParty(
                RvCCCall*           call,
                RvCCConnection*     party);

void rvCCCallSetConference(
                RvCCCall*           call, 
                RvCCConnection*     party);                           

/*Add an additional party to the call, go into conferencing state */
void rvCCCallAddConferenceParty(
                RvCCCall*           call, 
                RvCCConnection*     party);
                                
/***************************************************************************
 * rvCCCallClearTransferParties
 * ------------------------------------------------------------------------
 * General: Clear Transfer lines mutually, 
 *			and goes into Normal state in the MDM connections				*

 * Return Value: (-)
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: 	mdmConnLine1 - mdm connection of line 1.
 *			mdmConnLine2 - mdm connection of line 2.
 ***************************************************************************/
void rvCCCallClearTransferParties(
                RvCCConnection*     mdmConnLine1, 
                RvCCConnection*     mdmConnLine2);                                  

/*Set Transfer lines mutually, and goes into Transfer state*/
void rvCCCallSetTransferParties(
                RvCCConnection*     x, 
                RvCCConnection*     party);                                

/*This function removes all connections of the call, except for the connection's party.
  It is used when the call is switched from Conference state to Normal state.
  parties removed are probably not successful calls*/
void rvCCCallRemoveConferenceParties(RvCCConnection* x);

/*Removes a party from the call */
RvBool rvCCCallRemoveParty(
                RvCCCall*           call,
                RvCCConnection*     party);                           

/*Drop all parties from call*/
void rvCCCallDrop(
                RvCCCall*       call,
                RvCCEventCause  reason);                  

/* Lock the call */
void rvCCCallLock(RvCCCall* call);

/* Unlock the call */
void rvCCCallUnlock(RvCCCall* call);

/*-----------------------------------------------------------------------
						C A L L		C A L L B A C K S
 ------------------------------------------------------------------------*/

void rvCCCallTransferInProcessCB(
                RvCCConnection* conn);

void rvCCCallTransferConnectedCB(
                RvCCConnection*     conn, 
                RvCCConnection*     newConn);

/***************************************************************************
 * rvCCCallTransferCompletedCB
 * ------------------------------------------------------------------------
 * General: This function is called when the transfer process is completed.
 *			If the transfer process completed successfully, it is needed to 
 *			finish the transfer process and to disconnect the connections. 
 *			If the transfer process is completed with a failure, it is needed 
 *			to go back to the lines status BEFORE the transfer process. 
 *
 * Return Value: (-)
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: 	conn			- protocol connection
 *			isSuccessfully	- Is the transfer process completed successfully or not.
  ***************************************************************************/
void rvCCCallTransferCompletedCB(
                RvCCConnection*     conn, 
                RvBool              isSuccessfully);                                 

void rvCCCallAlerting(
                IN RvCCConnection*  x, 
                IN RvCCEventCause   reason);

void rvCCCallConnected(
                IN RvCCConnection*  x, 
                IN RvCCEventCause   reason);

void rvCCCallDisconnectedRemote(
                IN RvCCConnection*  x, 
                IN RvCCEventCause   reason);


/***************************************************************************
 * rvCCCallProcessIncomingCB
 * ------------------------------------------------------------------------
 * General: This function is called when an incoming call should be processed.
 *			In case of CFW feature used, it is needed to check activation of 
 *			cfw type. If a cfw type is activated, perform the following:
 *			If the type is unconditional-forward the call.
 *			If the type is busy, check if all the connections are used, 
 *			if yes, forward the call
 *			If the type is no reply, start timer, but continue process the 
 *			call normally (return rvTrue).
 *
 * Return Value: return rvTrue if the call should be processed as normal call,
 *				 rvFalse, otherwise. (for example, if the call has been forwarded 
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: 	conn	- protocol connection
 *			reason	- event cause of this event.
  ***************************************************************************/
RvBool rvCCCallProcessIncomingCB(
                RvCCConnection*     x,
                RvCCEventCause      reason);                                 

void rvCCCallRejected(
                IN RvCCConnection*  x, 
                IN RvCCEventCause   reason);
                      
RvBool rvCCCallPartyReleased(IN RvCCConnection* x);

RvBool rvCCCallHold(
                RvCCConnection*     x,
                RvCCEventCause      reason);
                
void rvCCCallTalking(
                RvCCConnection*     x,
                RvCCEventCause      reason);
                     
void rvCCCallDtmfPressed(
                IN RvCCConnection*      x, 
                IN RvDtmfParameters*    dtmfParam);                         

RvBool rvCCCallConferenceJoinedCB(RvCCConnection* x);
RvBool rvCCCallConferenceLeftCB(RvCCConnection* x);

RvBool rvCCCallMediaUpdatedCB(
                RvCCConnection*     conn, 
                RvSdpMsg*           inMedia);

RvBool rvCCCallModifyMediaCB(
                RvCCConnection*         x, 
                RvMdmMediaStreamInfo*   streamDescr);                             

void    rvCCCallModifyMediaDoneCB(
                RvCCConnection* x,
                RvBool status,
                RvMdmMediaStreamInfo* mediaStream,
                RvMdmTermReasonModifyMedia reason);

void rvCCCallTerminatedCB(
                RvCCConnection*     conn, 
                RvCCCall*           call);

void rvCCCallSendUserEventCB(
                RvCCConnection*     x, 
                RvCCTerminalEvent   event,
				RvCCEventCause*     reason);

/*===================*/
/*	RvCCCallMgr		 */
/*===================*/

typedef struct RvCCCallMgr_
{
	RvPtrList		calls;
	RvCCCall*		activeCall;
	RvMutex			mutex;
} RvCCCallMgr;

/*-----------------------------------------------------------------------
					C A L L	- M A N A G E R	 	F U N C T I O N S
 ------------------------------------------------------------------------*/
void rvCCCallMgrConstructA(
                RvCCCallMgr*    x, 
                RvAlloc*        a);
                           
void		rvCCCallMgrDestruct(RvCCCallMgr* x);

RvCCCall*	rvCCCallMgrCreateCall(RvCCCallMgr* x);
void rvCCCallMgrReleaseCall(
                RvCCCallMgr*    x, 
                RvCCCall*       call);

void rvCCCallMgrMoveConnection(
                RvCCCallMgr*    x,
                RvCCCall*       callSource, 
                RvCCCall*       callDest,
                RvCCConnection* conn);

/* Transfer-Replaces feature */
RvBool rvCCCallMgrReplaceConnByLocal(
                IN RvCCCallMgr* x,
                IN RvCCConnection* conn,
                IN char* address,
                IN char* data,
                OUT RvCCConnection** oldParty);

int rvCCCallMgrReplaceConn(
                RvCCCallMgr*    x, 
                RvCCConnection* call1Conn, 
                RvCCConnection* call2Conn);

/******************************************************************************
*  RvCCCallConferenceSendDtmfByConn
*  ----------------------------
*  General :        send dtmf to all parties in conference
*
*  Return Value:    None.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          conn            current connection.
*       .          digit           digit from user
*
*
*
*
*  Output          None.
*
******************************************************************************/
void RvCCCallConferenceSendDtmfByConn(
                IN RvCCConnection*  conn, 
                IN RvChar           digit);
                                      
/******************************************************************************
*  rvCCCallHandleOutOfBandDTMF
*  ----------------------------
*  General :        choose if to send to one party or more(conf)
*
*  Return Value:    None.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          conn            current connection.
*       .          digit           digit from user
*
*
*
*
*  Output          None.
*
******************************************************************************/
void rvCCCallHandleOutOfBandDTMF(
                IN RvCCConnection*  conn,
                IN RvChar           digit, 
                int                 duration);

#endif /*RV_CCCALL_H*/





















