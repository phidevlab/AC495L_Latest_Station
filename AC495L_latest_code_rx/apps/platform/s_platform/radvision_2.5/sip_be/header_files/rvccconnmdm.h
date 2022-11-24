/******************************************************************************
Filename:    rvccconnmdm.h
Description: Mdm Connection Header
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
#ifndef RV_CCCONN_MDM_H
#define RV_CCCONN_MDM_H

#include "rvalloc.h"
#include "rvstring.h"

#include "rvccapi.h"
#include "rvccprovidermdm.h"
#include "rvccterminalmdm.h"
#include "rvmdmmediastream.h"


typedef struct RvCCConnMdm_ {
	RvCCTerminal*			terminal;
	RvMdmTerm*				ephTerm;
	RvString				dialString;
	unsigned int			streamId;
	RvAlloc*				alloc;
	IppTimer				cfnrTimer;	/* call forward no reply Timer object */
} RvCCConnMdm;


/* APIs */
/*======*/

void rvCCConnMdmConstruct(RvCCConnection* x, RvCCTerminal* term, RvCCProviderMdm* p, RvAlloc* a);

void rvCCConnMdmDestruct(RvCCConnection* x);

#define rvCCConnMdmGetIns(x)	((RvCCConnMdm*)(x)->connection)

#define rvCCConnMdmGetTerminal(x)	((rvCCConnMdmGetIns(x))->terminal)

#define rvCCConnMdmSetEphTerm(x, a)	((rvCCConnMdmGetIns(x))->ephTerm=a)

#define rvCCConnMdmGetEphTerm(x)	((rvCCConnMdmGetIns(x))->ephTerm)

RvCCTerminal* rvCCConnMdmGetEphXTerm(RvCCConnection* x);

struct RvCCTerminalMdm_* rvCCConnMdmGetTerminalImpl(RvCCConnection* c);


void rvCCConnMdmProcessEvent(RvCCConnection* conn, RvCCTerminalEvent event, 
							 RvBool* callStillAlive, RvCCEventCause reason);

void rvCCConnMdmReject(RvCCConnection* x, RvCCEventCause reason);

void rvCCConnMdmMakeCall(RvCCConnection* x,RvSdpMsg* inMedia);

/*Disconnect a connection and associated media.
Connection drops from the call.*/
void rvCCConnMdmDisconnecting(RvCCConnection* x,RvCCEventCause reason);

/*Start a ringback in the connection
Connection must be in ESTABLISHED state
*/
void rvCCConnMdmRingback(RvCCConnection* x, /*h.e RvSdpMsg* inMedia,*/RvCCEventCause reason);


RvSdpMsg* rvCCConnMdmGetMedia(RvCCConnection* x);

RvSdpMsg* rvCCConnMdmGetMediaCaps(RvCCConnection* x);

/*Note: should this function receive parameters from the application
or should they be set by the connection/provider ?*/
RvCCMediaState rvCCConnMdmCreateMedia(RvCCConnection* x, RvSdpMsg* inMedia);

RvBool rvCCConnMdmSetRemoteMedia(RvCCConnection* x,RvSdpMsg* inMedia);

RvSdpMsg* rvCCConnMdmSetLocalMedia(RvCCConnection* x,RvSdpMsg* caps);

RvBool rvCCConnMdmModifyMedia(RvCCConnection* x, RvMdmMediaStreamInfo* mediaStream );

void rvCCConnMdmModifyMediaDone(RvCCConnection* x, 
								RvBool status, 
								RvMdmMediaStreamInfo* newMedia,
								RvMdmTermReasonModifyMedia reason);

RvBool  rvCCConnMdmConnectMedia(RvCCConnection* x);

void rvCCConnMdmDisconnectMedia(RvCCConnection* x);

RvBool rvCCConnMdmRestartMedia(RvCCConnection* x);

void rvCCConnMdmCallAnswered(RvCCConnection* x, RvSdpMsg* inMedia);


void rvCCConnMdmDisconnect(RvCCConnection* x);

/* Connection can be released */
void rvCCConnMdmRelease(RvCCConnection* x);

/* returns NULL if dial string was not matched or not found.*/
RvCCConnection* rvCCConnMdmAddressAnalyze(RvCCConnection* x, RvCCEventCause* reason);

RvBool rvCCConnMdmJoinConference(RvCCConnection* x,RvCCConnection* confControl);
RvBool rvCCConnMdmLeaveConference(RvCCConnection* x,RvCCConnection* confControl);

void rvCCConnMdmTransferOffered(RvCCConnection* x,RvSdpMsg* inMedia);
RvCCConnection* rvCCConnMdmTransferConnected(RvCCConnection* x, RvSdpMsg* inMedia);
RvBool rvCCConnMdmIsTransferedConn(RvCCConnection* x, char* data);

void rvCCConnMdmInitiatedCB(RvCCConnection* x);
void rvCCConnMdmDialingCB(RvCCConnection* x);
void rvCCConnMdmNewDigitCB(RvCCConnection* x, RvCCEventCause reason);
void rvCCConnMdmCallDeliveredCB(RvCCConnection* x, RvCCEventCause reason);
RvCCMediaState rvCCConnMdmOfferedCB(RvCCConnection* x,RvCCEventCause reason);
RvBool rvCCConnMdmCallAnsweredCB(RvCCConnection* x);
RvCCMediaState rvCCConnMdmTransferOfferedCB(RvCCConnection* x);
const char* rvCCConnMdmGetCallerNumber(RvCCConnection* x,int index);

RvBool rvCCConnMdmConnectLineAndRtpTerms(RvCCConnection*		x, 
										 RvCCTerminal*			at,
										 struct RvCCTerminalMdm_ *ephTerm,
										 RvMdmTerm*				rtpTerm,
										 RvMdmMediaStreamInfo*	rtpMedia);

RvBool rvCCConnMdmDisconnectLineAndRtpTerms(RvCCConnection*			x, 
											 RvCCTerminal*			at,
											 struct RvCCTerminalMdm_ *ephTerm,
											 RvMdmTerm*				rtpTerm,
											 RvMdmMediaStreamInfo*	rtpMedia);

/* Default display function */
void rvCCConnMdmDisplay(RvCCConnection* conn, RvCCTerminal* term,
						RvCCTerminalEvent event, RvCCEventCause cause,
						void* displayData);

void rvCCConnMdmTerminate(RvCCConnection* x);

#ifdef RV_MTF_VIDEO
/************************************************************************************
 * rvCCConnMdmVideoFastUpdatePicture 
 *     The function uses extention API to call the callback function
 *     supplied by the user to inform the application that
 *     Fast Update Picture command has arrived to encoder (camera)
 ************************************************************************************/
void rvCCConnMdmVideoFastUpdatePicture(RvCCConnection *);

/************************************************************************************
 * rvCCConnMdmVideoFastUpdateGOB 
 *     The function uses extention API to call the callback function
 *     supplied by the user to inform the application that
 *     Fast Update Group Of Blocks command has arrived to encoder (camera)
 ************************************************************************************/
void rvCCConnMdmVideoFastUpdateGOB(RvCCConnection *, RvUint, RvUint);

/************************************************************************************
 * rvCCConnMdmVideoFastUpdateMB 
 *     The function uses extention API to call the callback function
 *     supplied by the user to inform the application that
 *     Fast Update Multi Blocks command has arrived to encoder (camera)
 ************************************************************************************/
void rvCCConnMdmVideoFastUpdateMB(RvCCConnection *, RvUint, RvUint, RvUint);
							 

/************************************************************************************
 * rvCCConnMdmVideoFastUpdatePictureFreeze 
 *     The function uses extention API to call the callback function
 *     supplied by the user to inform the application that
 *     Fast Update Picture Freeze command has arrived to decoder (screen)
 ************************************************************************************/
void rvCCConnMdmVideoFastUpdatePictureFreeze(RvCCConnection *);

/* Video Fast Update Command */
RvStatus rvCCConnMdmSendFastUpdatePictureFreeze(RvCCConnection *conn);
RvStatus rvCCConnMdmSendFastUpdatePicture(RvCCConnection *conn);
RvStatus rvCCConnMdmSendFastUpdateGOB(RvCCConnection *conn,
									RvUint firstGob,
									RvUint numGobs);
RvStatus rvCCConnMdmSendFastUpdateMB(RvCCConnection *conn,
									RvUint firstGob,
									RvUint firstMb,
									RvUint numMbs);
#endif /* RV_MTF_VIDEO */

/*==================================================================================
============= T E R M I N A L	C O N N E C T I O N		A P I	====================
===================================================================================*/
RvBool rvCCConnTermMdmHold(RvCCConnection* x);
RvBool rvCCConnTermMdmUnhold(RvCCConnection* x);
RvBool rvCCConnTermMdmRemoteHold(RvCCConnection* x);
RvBool rvCCConnTermMdmRemoteUnhold(RvCCConnection* x);
void rvCCConnTermMdmSetHoldIndicator(RvCCConnection* x, RvBool on);
void rvCCConnTermMdmSetMuteIndicator(RvCCConnection* x, RvBool on);
void rvCCConnTermMdmPlayDtmf(IN RvCCConnection* x, IN RvDtmfParameters* dtmfParam);
void rvCCConnTermMdmMute(RvCCConnection* x);
void rvCCConnTermMdmUnmute(RvCCConnection* x);


#endif /* RV_CCCONN_MDM_H*/
