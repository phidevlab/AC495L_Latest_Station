/******************************************************************************
Filename:    rvccapi.h
Description: Generic call control API
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

#ifndef RV_CCAPI_H
#define RV_CCAPI_H

#include "rvsdp.h"
#include "rvmutex.h"
#include "rvmdm.h"
#include "rvCallControlApi.h"
#include "rvmdmmediastream.h"
#include "rvIppCfwApi.h"


#define RV_MTF_VERSION   "Multimedia Terminal Framework Version 3.0.0.45 Copyright (c) RADVISION"


#define RV_IPP_ADDRESS_SIZE                 64

struct RvCCCallMgr_;


typedef enum
{
    RvDistinctiveRingNone = 0,
    RvDistinctiveRinging,
    RvDistinctiveRingback
} RvDistinctiveRingType;


typedef struct
{
    RvChar          digit;
    RvInt32         duration;
} RvDtmfParameters;

#define rvDtmfParametersInit(_dtmfParam) memset(_dtmfParam, 0, sizeof(RvDtmfParameters))

/*===============================*/
/*  RvCCTerminal                 */
/*===============================*/
/*NirS*/
//NBE: hanselhe, use unified MACRO
#ifdef CONFIG_320
#define RV_CCTERMINAL_MAXCONNS      4 /* NirM - Currently we support only 2 lines */ 
#elif CONFIG_310
#define RV_CCTERMINAL_MAXCONNS      3 
#elif CONFIG_350
#define RV_CCTERMINAL_MAXCONNS      7
#elif CONFIG_ININ
#define RV_CCTERMINAL_MAXCONNS      2
#else
#define RV_CCTERMINAL_MAXCONNS      3
#endif


struct RvCCConnection_;

typedef struct
{
    void*                       terminal;
    RvCCTerminalState           state;
    RvInt32                     activeConnection;
    RvCCAudioTermState          audioTermState;
    struct RvCCConnection_*     connections[RV_CCTERMINAL_MAXCONNS];
    RvMutex                     activeConnMutex;
    RvInt32                     displayColumn;
    RvInt32                     displayRow;
    struct RvCCTerminalClbks_*  clbks;
} RvCCTerminal;


/*-----------------------------------------------------------------------
                    T E R M I N A L     F U N C T I O N S
 ------------------------------------------------------------------------*/
const RvChar* rvCCTerminalGetId(RvCCTerminal* x);




/*-----------------------------------------------------------------------
                T E R M I N A L     C A L L B A C K S
 ------------------------------------------------------------------------*/

/*Return "false" if the terminal is not accepted by the MGC, i.e., is not in configuration.*/
typedef RvBool (*RvCCTerminalActiveCB)(RvCCTerminal* x);
typedef void (*RvCCTerminalInactiveCB)(RvCCTerminal* x);

typedef struct RvCCTerminalClbks_
{
    RvCCTerminalActiveCB    terminalActiveCB;
    RvCCTerminalInactiveCB  terminalInactiveCB;
} RvCCTerminalClbks;


/*-----------------------------------------------------------------------
                T E R M I N A L      A P I
 ------------------------------------------------------------------------*/

void rvCCTerminalInit(
    IN RvCCTerminal*       x,
    IN void*               term,
    IN RvCCTerminalClbks*  clbks);

void rvCCTerminalEnd(RvCCTerminal* x);

RvCCTerminalEvent rvCCTerminalMapEvent(RvCCTerminal* x, const char* pkg, const char* id,
                                    void * args, char* key);

int rvCCTerminalGetActiveConnectionId(RvCCTerminal* x) ;
void rvCCTerminalSetActiveConnectionId(
                RvCCTerminal*   x,
                int             id);

struct RvCCConnection_ * rvCCTerminalGetActiveConnection(RvCCTerminal* x);
struct RvCCConnection_ * rvCCTerminalFindFreeConnection(RvCCTerminal* x);

int rvCCTerminalFindNextActiveConnection(
                RvCCTerminal*               x,
                struct RvCCConnection_*     cur);

int rvCCTerminalGetNumActiveConnections(RvCCTerminal* x);
int rvCCTerminalIsIdleConnectionsExist(RvCCTerminal* x);

struct RvCCConnection_* rvCCTerminalGetConnectionByIndex(
                RvCCTerminal*   x,
                int             i);

void rvCCTerminalProcessEventId(
                RvCCTerminal*           t,
                RvCCTerminalEvent       eventId,
                char*                   keyId,
                struct RvCCCallMgr_*    callMgr,
                RvMdmMediaStreamInfo*   media);

/* Call when the active connection goes back to idle
   Then either look for a connection with a state different than idle,
   (some connection in hold, etc) or set the first connection as
   the active one */
void rvCCTerminalResetActiveConnection(RvCCTerminal* x);
int rvCCTerminalParseConnectionId(const char* line) ;

/* Use to set connection in ephemeral termination */
void rvCCTerminalSetEphemeralActiveConnection(
                RvCCTerminal*       x,
                struct RvCCConnection_ * conn);

#define rvCCTerminalGetCurDisplayRow(x)     ((x)->displayRow)
#define rvCCTerminalGetCurDisplayColumn(x)  ((x)->displayColumn)

void rvCCTerminalSetState(
                RvCCTerminal*       x,
                RvCCTerminalState   state);

RvCCTerminalState rvCCTerminalGetState(RvCCTerminal *x);


/*===============================*/
/*  RvCCConnection               */
/*===============================*/
typedef struct RvCCConnection_
{
    void*                           connection;

    RvCCConnType                    type;
    RvCCConnState                   state;
    RvCCTermConnState               termState;
    RvCCConnTransferState           transferState;
    RvInt32                         lineId;

    RvCCMediaState                  mediaState;
    RvCCTerminal*                   term;
    struct RvCCCall_*               call;
    struct RvCCConnection_*         curParty;
    RvMdmMediaStreamInfo            newMediaStream;

    struct RvCCConnectionFuncs_*    funcs;
    struct RvCCConnectionClbks_*    clbks;

    RvMutex                         mutex;
    /* Connections of the same termination that
       point to the other transfer line or conference line */
    struct RvCCConnection_*         transferLine;
    RvChar                          thirdPartyDestAddress[RV_IPP_ADDRESS_SIZE];
	RvBool                          blindTransferToDestination;
    /* keep digit from incoming userInputSignal in  case UserInputSignalUpdate arrives */

    /* For extension */
    void*                           userData;

    /*Presentation id and permissions of remote party (retrieved from incoming messages*/
    RvMdmTermPresentationInfo       remotePresentationInfo;
    /* 070729 - EA - merge to rv 2.5 */ 
    RvBool send_source_user_id; /* RG - when cleared, don't send my user id */ /* by itzik - jungo/radvision */
    RvString                        distinctiveRinging;
    RvString                        distinctiveRingback;
} RvCCConnection;

/*-----------------------------------------------------------------------
                C O N N E C T I O N     F U N C T I O N S
 ------------------------------------------------------------------------*/
/* To be called by subclass constructors */
void rvCCConnectionConstruct(
                RvCCConnection*                 x,
                RvCCTerminal*                   term,
                void*                           conn,
                RvCCConnType                    type,
                struct RvCCConnectionFuncs_*    funcs,
                struct RvCCConnectionClbks_*    clbks);

void rvCCConnectionDestruct(RvCCConnection* x);
void rvCCConnectionResetState(RvCCConnection* x);

/* Process the termination connection events */
RvCCTerminalEvent rvCCTermConnProcessEvents(
                RvCCConnection*     conn,
                RvCCTerminalEvent   eventId,
                RvCCEventCause*     cause);

/* generic connection state machine */
RvCCTerminalEvent rvCCConnectionStateMachine(
                RvCCConnection*     conn,
                RvCCTerminalEvent   event,
                RvCCEventCause*     cause,
                RvBool*             stillAlive);

void rvCCConnectionGetThirdPartyDestAddress(
                IN RvCCConnection*      x,
                OUT char*               address);

void rvCCConnectionSetThirdPartyDestAddress(
                IN RvCCConnection*     x,
                IN char*               address);

void rvCCConnectionSetLineId(RvCCConnection* x,int lineId);
int  rvCCConnectionGetLineId(RvCCConnection* x);

void rvCCConnectionSetState(
                RvCCConnection*     x,
                RvCCConnState       state);

void rvCCConnectionSetTermState(
                RvCCConnection*     x,
                RvCCTermConnState   state);

void rvCCConnectionSetBlindTransferToDestination(
				RvCCConnection*     x,
				RvBool              val);

RvBool rvCCConnectionGetBlindTransferToDestination(
				RvCCConnection*     x);

#define rvCCConnectionGetState(x) ((x)->state)
#define rvCCConnectionGetTermState(x) ((x)->termState)
#define rvCCConnectionGetMediaState(x) ((x)->mediaState)
#define rvCCConnectionGetCall(x)  ((x)->call)
#define rvCCConnectionSetCall(x,c)  ((x)->call=(c))
#define rvCCConnectionGetTerminal(x) ((x)->term)
#define rvCCConnectionSetTerminal(x,t) ((x)->term=(t))
#define rvCCConnectionGetType(x) ((x)->type)
#define rvCCConnectionSetType(x,t) ((x)->type=(t))

/* Transfer feature */
#define rvCCConnectionSetTransferType(x,c)  (((x)->call)->transferType = (c))

RvMdmMediaStreamInfo* rvCCConnectionGetNewMedia(RvCCConnection* x);

void rvCCConnectionSetMediaState(
				RvCCConnection* x,
				RvCCMediaState  state);
void rvCCConnectionSetNewMedia(
                RvCCConnection*         x,
                RvMdmMediaStreamInfo*   media);

void rvCCConnectionAddNewMediaLocalDescr(
                RvCCConnection*     x,
                RvSdpMsg*           sdpMsg);

void rvCCConnectionAddNewMediaRemoteDescr(
                RvCCConnection*     x,
                RvSdpMsg*           sdpMsg);

void rvCCConnectionClearNewMedia(RvCCConnection* x);

void rvCCConnectionSetConnectParty(
                RvCCConnection*     x,
                RvCCConnection*     party);

RvCCConnection *rvCCConnectionGetConnectParty(RvCCConnection *x);

RvSdpConnectionMode rvCCConnectionGetConnModeByHoldState (RvCCConnection * x);/*EM 03/19/2012 VI 91772: Adding RV code */

void rvCCConnectionSetTransferLine(
                                   RvCCConnection*     x,
                                   RvCCConnection*     transferLine);

RvCCConnection* rvCCConnectionGetTransferLine(RvCCConnection*     x);

void rvCCConnectionSetTransferState(
                                    RvCCConnection*          x,
                                    RvCCConnTransferState    transferState);

RvCCConnTransferState rvCCConnectionGetTransferState(RvCCConnection*     x);

RvBool rvCCConnectionSetRemotePresentationInfo(
                RvCCConnection*				    x,
				RvMdmTermPresentationInfo*      presentationInfo);

RvMdmTermPresentationInfo* rvCCConnectionGetRemotePresentationInfo(RvCCConnection* x);

#define rvCCConnectionSetUserData(x,d) ((x)->userData=d)
#define rvCCConnectionGetUserData(x) ((x)->userData)

RvBool rvCCConnectionIsCallOnHold(RvCCConnection* x);

void rvCCConnectionSetThirdPartyAddress(
                RvCCConnection*     c,
                char*               foundDestAddress);

const char* rvCCConnectionGetDistinctiveRinging( RvCCConnection* c);
const char* rvCCConnectionGetDistinctiveRingback(RvCCConnection* c);
void rvCCConnectionSetDistinctiveRinging(RvCCConnection* c, char* ringStr);
void rvCCConnectionSetDistinctiveRingback(RvCCConnection* c, char* ringStr);
void rvCCConnectionResetDistinctiveRinging(RvCCConnection* c);
void rvCCConnectionResetDistinctiveRingback(RvCCConnection* c);

void rvCCConnectionAccept(
                          RvCCConnection*     x,
                          RvSdpMsg*           inMedia);
                          ;
typedef void (*RvCCConnectionAcceptF)(RvCCConnection* x,RvSdpMsg* inMedia);

void rvCCConnectionReject(
                          RvCCConnection*     x,
                          RvCCEventCause      reason);

typedef void (*RvCCConnectionRejectF)(
                          RvCCConnection*     x,
                          RvCCEventCause      reason);

void rvCCConnectionMakeCall(
                          RvCCConnection*     x,
                          RvSdpMsg*           inMedia);

typedef void (*RvCCConnectionMakeCallF)(
                          RvCCConnection*     x,
                          RvSdpMsg*           inMedia);

/*Disconnect a connection and associated media.
Connection drops from the call.*/
void rvCCConnectionDisconnect(
                          RvCCConnection*     x,
                          RvCCEventCause      cause);

typedef void (*RvCCConnectionDisconnectF)(
                          RvCCConnection*     x,
                          RvCCEventCause      cause);

/*Start a ringback in the connection
Connection must be in ESTABLISHED state
Ringback will stop in modify media or moving out of ESTABLISHED ?*/
void rvCCConnectionRingback(
                          RvCCConnection*     x,
                          RvCCEventCause      cause);

typedef void (*RvCCConnectionRingbackF)(
                          RvCCConnection*     x,
                          RvCCEventCause      cause);


/***************************************************************************
* rvCCConnectionForwardCall
* ------------------------------------------------------------------------
* General: This function forward an incoming call to the sendToAddress..
* Return Value:
*           RvStatus
* ------------------------------------------------------------------------
*  Arguments:
*  Input:   mdmConn: mdm connection
*           sendToAddress - Address to forward the call to.
*
*  Output:  none.
***************************************************************************/
RvStatus rvCCConnectionForwardCall(
                RvCCConnection*     x,
                RvChar*             sendToAddress);

typedef RvStatus (*RvCCConnectionForwardCallF)(
                RvCCConnection*     x,
                RvChar*             sendToAddress);

RvCCMediaState rvCCConnectionCreateMedia(
                RvCCConnection*     x,
                RvSdpMsg*           inMedia);

typedef RvCCMediaState (*RvCCConnectionCreateMediaF)(
                RvCCConnection*     x,
                RvSdpMsg*           inMedia);

RvSdpMsg* rvCCConnectionGetMedia(RvCCConnection* x);
typedef RvSdpMsg* (*RvCCConnectionGetMediaF)(RvCCConnection* x);

RvSdpMsg* rvCCConnectionGetMediaCaps(RvCCConnection* x);
typedef RvSdpMsg* (*RvCCConnectionGetMediaCapsF)(RvCCConnection* x);

RvBool rvCCConnectionSetRemoteMedia(
                RvCCConnection*     x,
                RvSdpMsg*           inMedia);

typedef RvBool (*RvCCConnectionSetRemoteMediaF)(
                RvCCConnection*     x,
                RvSdpMsg*           inMedia);

RvSdpMsg* rvCCConnectionSetLocalMedia(
                RvCCConnection*     x,
                RvSdpMsg*           capsSubset);

typedef RvSdpMsg* (*RvCCConnectionSetLocalMediaF)(
                RvCCConnection*     x,
                RvSdpMsg*           capsSubset);

RvBool rvCCConnectionRestartMedia(RvCCConnection* x);
typedef RvBool (*RvCCConnectionRestartMediaF)(RvCCConnection* x);

RvBool rvCCConnectionModifyMedia(
                RvCCConnection*         x,
                RvMdmMediaStreamInfo*   streamDescr);

typedef RvBool (*RvCCConnectionModifyMediaF)(
                RvCCConnection*         x,
                RvMdmMediaStreamInfo*   streamDescr);

void rvCCConnectionModifyMediaDone(
                RvCCConnection*             x,
                RvBool                      status,
                RvMdmMediaStreamInfo*       mediaStream,
                RvMdmTermReasonModifyMedia  reason);

typedef void (*RvCCConnectionModifyMediaDoneF)(
                RvCCConnection*             x,
                RvBool                      status,
                RvMdmMediaStreamInfo*       mediaStream,
                RvMdmTermReasonModifyMedia  reason);

void rvCCConnectionConnectMedia(RvCCConnection* x);
typedef void (*RvCCConnectionConnectMediaF)(RvCCConnection* x);

void rvCCConnectionDisconnectMedia(RvCCConnection* x);
typedef void (*RvCCConnectionDisconnectMediaF)(RvCCConnection* x);

void rvCCConnectionCallAnswered(
                RvCCConnection*     x,
                RvSdpMsg*           inMedia);

typedef void (*RvCCConnectionCallAnsweredF)(
                RvCCConnection*     x,
                RvSdpMsg*           inMedia);

/* Digit collection has been completed */
RvCCConnection* rvCCConnectionAddressAnalyze(
                IN RvCCConnection*      x,
                OUT RvCCEventCause*     reason);

typedef RvCCConnection* (*RvCCConnectionAddressAnalyzeF)(
                IN RvCCConnection*      x,
                OUT RvCCEventCause*     reason);

/* Connection can be released */
void rvCCConnectionRelease(RvCCConnection* x);
typedef void (*RvCCConnectionReleaseF)(RvCCConnection* x);

/* Drop the connection, this connection is usually in hold */
/* It goes away silently, another connection exist to go on hook */
void rvCCConnectionDrop(RvCCConnection* x);
typedef void (*RvCCConnectionDropF)(RvCCConnection* x);

/* Transfer - close existing media and open new one */
void rvCCConnectionTransferInit(RvCCConnection* x);
typedef void (*RvCCConnectionTransferInitF)(RvCCConnection* x);

/* The other side of transfer */
void rvCCConnectionTransferOffered(
                RvCCConnection*     x,
                RvSdpMsg*           origMedia);

typedef void (*RvCCConnectionTransferOfferedF)(
                RvCCConnection*     x,
                RvSdpMsg*           origMedia);

RvCCConnection* rvCCConnectionTransferConnected(
                RvCCConnection*     x,
                RvSdpMsg*           origMedia);

typedef RvCCConnection* (*RvCCConnectionTransferConnectedF)(
                RvCCConnection*     x,
                RvSdpMsg*           origMedia);

/* Should be called after rvCCConnectionTransferInit, not needed by all protocols */
void rvCCConnectionTransferInProgress(RvCCConnection* x);
typedef void (*RvCCConnectionTransferInProgressF)(RvCCConnection* x);

/*Check if this is the matching MDM connection of a Transfered call to
  move from the old call to the new one */
RvBool rvCCConnectionIsTransferedConn(
                RvCCConnection*     x,
                char*               data);

typedef RvBool (*RvCCConnectionIsTransferedConnF)(
                RvCCConnection*     x,
                char*               data);

/* Get the caller identifier - leave here for backward compatibility */
typedef const char* (*RvCCConnectionGetCallerIdF)(RvCCConnection* x);
const char* rvCCConnectionGetCallerId(RvCCConnection* x);

/* Get the caller identifiers */
typedef const char* (*RvCCConnectionGetCallerNameF)(RvCCConnection* x);
const char* rvCCConnectionGetCallerName(RvCCConnection* x);

typedef const char* (*RvCCConnectionGetCallerAddressF)(RvCCConnection* x);
const char* rvCCConnectionGetCallerAddress(RvCCConnection* x);

typedef const char* (*RvCCConnectionGetCallerNumberF)(
                RvCCConnection*     x,
                int                 index);

const char* rvCCConnectionGetCallerNumber(
                RvCCConnection*     x,
                int                 index);

/* Activate local Conference */
typedef RvBool (*RvCCConnectionJoinConferenceF)(
                RvCCConnection*     x,
                RvCCConnection*     confControl);

RvBool rvCCConnectionJoinConference(
                RvCCConnection*     x,
                RvCCConnection*     confControl);

/* Leave the local conference */
typedef RvBool (*RvCCConnectionLeaveConferenceF)(
                RvCCConnection*     x,
                RvCCConnection*     confControl);

RvBool rvCCConnectionLeaveConference(
                RvCCConnection*     x,
                RvCCConnection*     confControl);

typedef void (*RvCCConnectionProcessTermEventF)(
                RvCCConnection*     x,
                RvCCTerminalEvent   eventId,
                RV_BOOL*            callStillAlive,
                RvCCEventCause      reason);

void rvCCConnectionProcessTermEvent(
                RvCCConnection*     x,
                RvCCTerminalEvent   eventId,
                RV_BOOL*            callStillAlive,
                RvCCEventCause      reason);

typedef void (*RvCCConnectionTerminateF)(RvCCConnection* x);
void rvCCConnectionTerminate(RvCCConnection* x);

/* Terminal connection functions */

/* Remote Party set us on hold */
/* Stop sending media to party and ignore incoming media from party */
typedef RvBool (*RvCCConnectionTermRemoteHoldF)(RvCCConnection* x);
RvBool rvCCConnectionTermRemoteHold(RvCCConnection* x);

/* resume sending/receiving  media to party */
typedef RvBool (*RvCCConnectionTermRemoteUnholdF)(RvCCConnection* x);
RvBool rvCCConnectionTermRemoteUnhold(RvCCConnection* x);

/* Stop sending media to all parties, ignore all incoming media  */
typedef RvBool (*RvCCConnectionTermHoldF)(RvCCConnection* x);
RvBool rvCCConnectionTermHold(RvCCConnection* x);

/* Resume sending and receiving media to all parties */
typedef RvBool (*RvCCConnectionTermUnholdF)(RvCCConnection* x);
RvBool rvCCConnectionTermUnhold(RvCCConnection* x);

/* Control hold indicator */
typedef void (*RvCCConnectionTermSetHoldIndicatorF)(
                RvCCConnection*     x,
                RV_BOOL             on);

void rvCCConnectionTermSetHoldIndicator(
                RvCCConnection*     x,
                RV_BOOL             on);

/* Control nute indicator */
typedef void (*RvCCConnectionTermSetMuteIndicatorF)(RvCCConnection* x,RvBool on);
void rvCCConnectionTermSetMuteIndicator(RvCCConnection* x,RvBool on);

/* Disconnect the audio */
typedef void (*RvCCConnectionTermDisconnectMediaF)(RvCCConnection* x);
void rvCCConnectionTermDisconnectMedia(RvCCConnection* x);

/* Connect the audio (to current active audio termination) */
typedef void (*RvCCConnectionTermConnectMediaF)(RvCCConnection* x);
void rvCCConnectionTermConnectMedia(RvCCConnection* x);

typedef void (*RvCCConnectionTermPlayDtmfF)(IN RvCCConnection* x, IN RvDtmfParameters* dtmfParam);
void rvCCConnectionTermPlayDtmf(IN RvCCConnection* x, IN RvDtmfParameters* dtmfParam);


/* mute */
typedef void (*RvCCConnectionTermMuteF)(RvCCConnection* x);
void rvCCConnectionTermMute(RvCCConnection* x);
/* Unmute */
typedef void (*RvCCConnectionTermUnmuteF)(RvCCConnection* x);
void rvCCConnectionTermUnmute(RvCCConnection* x);

#ifdef RV_MTF_VIDEO
/* video Fast Update Picture */
typedef RvStatus (*RvCCConnectionVideoFastUpdatePictureFreezeF)(RvCCConnection* x);
RvStatus rvCCConnectionSendFastUpdatePictureFreeze(RvCCConnection* x);

typedef RvStatus (*RvCCConnectionVideoFastUpdatePictureF)(RvCCConnection* x);
RvStatus rvCCConnectionSendFastUpdatePicture(RvCCConnection* x);

typedef RvStatus (*RvCCConnectionVideoFastUpdateGOBF)(RvCCConnection* x, RvUint firstGob, RvUint numGobs);
RvStatus rvCCConnectionSendFastUpdateGOB(RvCCConnection* x, RvUint firstGob, RvUint numGobs);

typedef RvStatus (*RvCCConnectionVideoFastUpdateMBF)(RvCCConnection* x, RvUint firstGob, RvUint firstMb, RvUint numMbs);
RvStatus rvCCConnectionSendFastUpdateMB(RvCCConnection* x, RvUint firstGob, RvUint firstMb, RvUint numMbs);
#endif /*RV_MTF_VIDEO */


/*-----------------------------------------------------------------------
                C O N N E C T I O N     C A L L B A C K S
 ------------------------------------------------------------------------*/

typedef void (*RvCCConnectionInitiatedCB)(RvCCConnection* x);
void rvCCConnectionInitiatedCB(RvCCConnection* x);

typedef void (*RvCCConnectionNewDigitCB)(RvCCConnection* x,RvCCEventCause reason);
void rvCCConnectionNewDigitCB(RvCCConnection* x,RvCCEventCause reason);

/* Returns TRUE if the call can be initiated, if returns False, outReason will specify the
   reason for failure.  */
typedef RvCCConnection* (*RvCCConnectionAddressAnalyzeCB)(RvCCConnection* x, const char* address, RvCCEventCause inReason, RvCCEventCause* outReason);
RvCCConnection*  rvCCConnectionAddressAnalyzeCB(RvCCConnection* x,const char* address, RvCCEventCause inReason, RvCCEventCause* outReason);

typedef void (*RvCCConnectionInProcessCB)(RvCCConnection* x,RvCCEventCause reason);
void rvCCConnectionInProcessCB(RvCCConnection* x,RvCCEventCause reason);

typedef void (*RvCCConnectionTransferInProcessCB)(RvCCConnection* x);
void rvCCConnectionTransferInProcessCB(RvCCConnection* x);

typedef void (*RvCCConnectionCallDeliveredCB)(RvCCConnection* x,RvCCEventCause reason);
void rvCCConnectionCallDeliveredCB(RvCCConnection* x,RvCCEventCause reason);

typedef RvCCMediaState (*RvCCConnectionOfferedCB)(RvCCConnection* x,RvCCEventCause reason);
RvCCMediaState rvCCConnectionOfferedCB(RvCCConnection* x,RvCCEventCause reason);

typedef RvBool (*RvCCConnectionCallAnsweredCB)(RvCCConnection* x);
RvBool rvCCConnectionCallAnsweredCB(RvCCConnection* x);

typedef void (*RvCCConnectionMediaCreatedCB)(RvCCConnection* x);
void rvCCConnectionMediaCreatedCB(RvCCConnection* x);

typedef RvCCMediaState (*RvCCConnectionTransferOfferedCB)(RvCCConnection* x);
RvCCMediaState rvCCConnectionTransferOfferedCB(RvCCConnection* x);

typedef RvCCMediaState (*RvCCConnectionTransferInitCB)(RvCCConnection* x);
RvCCMediaState rvCCConnectionTransferInitCB(RvCCConnection* x);

typedef RvBool (*RvCCConnectionConferenceJoinedCB)(RvCCConnection* x);
RvBool rvCCConnectionConferenceJoinedCB(RvCCConnection* x);

typedef RvBool (*RvCCConnectionMediaUpdatedCB)(RvCCConnection* x, RvSdpMsg* inMedia);
RvBool rvCCConnectionMediaUpdatedCB(RvCCConnection* x, RvSdpMsg* inMedia);

typedef RvBool (*RvCCConnectionMediaModifiedCB)(RvCCConnection* x, RvMdmMediaStreamInfo* streamDescr);
RvBool rvCCConnectionMediaModifiedCB(RvCCConnection* x, RvMdmMediaStreamInfo* streamDescr);

typedef void (*RvCCConnectionMediaModifiedDoneCB)(RvCCConnection* x,
                                                   RvBool status,
                                                   RvMdmMediaStreamInfo* mediaStream,
                                                   RvMdmTermReasonModifyMedia reason);
void rvCCConnectionMediaModifiedDoneCB(RvCCConnection* x,
                                                   RvBool status,
                                                   RvMdmMediaStreamInfo* mediaStream,
                                                   RvMdmTermReasonModifyMedia reason);

typedef RvSdpMsg* (*RvCCConnectionCapsUpdatedCB)(RvCCConnection* x, RvSdpMsg* capsSubSet);
RvSdpMsg* rvCCConnectionCapsUpdatedCB(RvCCConnection* x, RvSdpMsg* capsSubSet);



/*  ====Terminal Callbacks==== */
/***************************************************************************
* rvCCTerminalCfwActivateCompletedCB
* ------------------------------------------------------------------------
* General: User call back.
*           This CB is implemented by the user application.
*           It is called to indicate the result of the user's request
*           to perform call forward activate.
* Return Value:
*           None
* ------------------------------------------------------------------------
*  Arguments:
*  Input:   t - pointer to terminal
*           cfwType - cfw type
*           cfwDestination - phone number of where to forward the call
*  Output:  returnCode - result.
***************************************************************************/
void rvCCTerminalCfwActivateCompletedCB(
                RvCCTerminal*           t,
                RvIppCfwType		    cfwType,
                char*				    cfwDestination,
                RvIppCfwReturnReasons   returnCode);

/***************************************************************************
* rvCCTerminalCfwDeactivateCompletedCB
* ------------------------------------------------------------------------
* General: User call back.
*           This CB is implemented by the user application.
*           It is called to indicate the result of the user's request
*           to perform call forward deactivate.
* Return Value:
*           None
* ------------------------------------------------------------------------
*  Arguments:
*  Input:   t - pointer to terminal
*           cfwType - cfw type
*
*  Output:  returnCode - result.
***************************************************************************/
void rvCCTerminalCfwDeactivateCompletedCB(RvCCTerminal *t,
                                RvIppCfwType        cfwType,
                                RvIppCfwReturnReasons returnCode);

/* Extension APIs */
/*================*/
typedef void (*RvCCConnectionSendUserEventCB)(RvCCConnection* x,
                                              RvCCTerminalEvent event, RvCCEventCause* reason);
void rvCCConnectionSendUserEventCB(RvCCConnection* x,
                                   RvCCTerminalEvent event, RvCCEventCause* reason);

typedef void (*RvCCConnectionDisplayCB)(RvCCConnection* conn, RvCCTerminal* term,
                                        RvCCTerminalEvent event, RvCCEventCause cause,
                                        void* displayData);

void rvCCConnectionDisplay(
                RvCCConnection*     x,
                RvCCTerminal*       term,
                RvCCTerminalEvent   event,
                RvCCEventCause      cause,
                void*               displayData);


typedef struct RvCCConnectionFuncs_
{
    RvCCConnectionAcceptF                       acceptF;
    RvCCConnectionRejectF                       rejectF;
    RvCCConnectionMakeCallF                     makeCallF;
    RvCCConnectionDisconnectF                   disconnectF;
    RvCCConnectionRingbackF                     ringbackF;
    RvCCConnectionForwardCallF                  forwardCallF;
    RvCCConnectionCreateMediaF                  createMediaF;
    RvCCConnectionGetMediaF                     getMediaF;
    RvCCConnectionGetMediaCapsF                 getMediaCapsF;
    RvCCConnectionSetRemoteMediaF               setRemoteMediaF;
    RvCCConnectionSetLocalMediaF                setLocalMediaF;
    RvCCConnectionRestartMediaF                 restartMediaF;
    RvCCConnectionModifyMediaF                  modifyMediaF;
    RvCCConnectionModifyMediaDoneF              modifyMediaDoneF;
    RvCCConnectionCallAnsweredF                 callAnsweredF;
    RvCCConnectionAddressAnalyzeF               addressAnalyzeF;
    RvCCConnectionReleaseF                      releaseF;
    RvCCConnectionDropF                         dropF;
    RvCCConnectionTransferInitF                 transferInitF;
    RvCCConnectionTransferOfferedF              transferOfferedF;
    RvCCConnectionTransferConnectedF            transferConnectedF;
    RvCCConnectionTransferInProgressF           transferInProgressF;
    RvCCConnectionIsTransferedConnF             isTransferedConnF;
    RvCCConnectionGetCallerNameF                getCallerNameF;
    RvCCConnectionGetCallerAddressF             getCallerAddressF;
    RvCCConnectionGetCallerNumberF              getCallerNumberF;
    RvCCConnectionGetCallerIdF                  getCallerIdF;
    RvCCConnectionJoinConferenceF               joinConferenceF;
    RvCCConnectionLeaveConferenceF              leaveConferenceF;
    RvCCConnectionProcessTermEventF             processTermEventF;
    RvCCConnectionTerminateF                    terminateF;

    /* Termination connection */
    RvCCConnectionTermSetHoldIndicatorF  setHoldIndicatorF;
    RvCCConnectionTermRemoteHoldF        remoteHoldF;
    RvCCConnectionTermRemoteUnholdF      remoteUnholdF;
    RvCCConnectionTermHoldF              holdF;
    RvCCConnectionTermUnholdF            unholdF;
    RvCCConnectionTermDisconnectMediaF   disconnectMediaF;
    RvCCConnectionTermConnectMediaF      connectMediaF;
    RvCCConnectionTermPlayDtmfF          playDtmfF;
    RvCCConnectionTermSetMuteIndicatorF  setMuteIndicatorF;
    RvCCConnectionTermMuteF              muteF;
    RvCCConnectionTermUnmuteF            UnmuteF;

    /* Display */
    RvCCConnectionDisplayCB              displayF;
#ifdef RV_MTF_VIDEO
    /* Video Fast Update Commands */
    RvCCConnectionVideoFastUpdatePictureF   videoFastUpdatePictureF;
    RvCCConnectionVideoFastUpdateGOBF       videoFastUpdateGOBF;
    RvCCConnectionVideoFastUpdateMBF        videoFastUpdateMBF;
    RvCCConnectionVideoFastUpdatePictureFreezeF  videoFastUpdatePictureFreezeF;
#endif
} RvCCConnectionFuncs;

typedef struct RvCCConnectionClbks_
{
    RvCCConnectionInitiatedCB       initiatedCB;
    RvCCConnectionNewDigitCB        newDigitCB;
    RvCCConnectionCallAnsweredCB    callAnsweredCB;
    RvCCConnectionAddressAnalyzeCB  addressAnalyzeCB;
    RvCCConnectionInProcessCB       inProcessCB;
    RvCCConnectionTransferInProcessCB   transferInProcessCB;
    RvCCConnectionCallDeliveredCB   callDeliveredCB;
    RvCCConnectionOfferedCB         offeredCB;

    RvCCConnectionTransferOfferedCB transferOfferedCB;
    RvCCConnectionTransferInitCB    transferInitCB;

    RvCCConnectionConferenceJoinedCB  conferenceJoinedCB;

    /* media */
    RvCCConnectionMediaCreatedCB    mediaCreatedCB;
    RvCCConnectionMediaUpdatedCB    mediaUpdatedCB;
    RvCCConnectionCapsUpdatedCB     capsUpdatedCB;
    RvCCConnectionMediaModifiedCB       mediaModifiedCB;
    RvCCConnectionMediaModifiedDoneCB   mediaModifiedDoneCB;

    RvCCConnectionSendUserEventCB       sendUserEventCB;

} RvCCConnectionClbks;


/*===============================*/
/*  RvCCTermConnection           */
/*===============================*/
typedef struct RvCCTermConnection_
{
    void*               termConnection;

    RvCCTermConnState   state;
    RvCCConnection*     connection;
    RvCCTerminal*       term;
} RvCCTermConnection;



/*===============================*/
/*  RvCCProvider                 */
/*===============================*/
typedef struct RvCCProvider_
{
    void*                       provider;
    struct RvCCProviderFuncs_*  funcs;
} RvCCProvider;

/*-----------------------------------------------------------------------
                P R O V I D E R     F U N C T I O N S
 ------------------------------------------------------------------------*/

/* Initialize a connection to a terminal (address) in an idle state */
/* To route a call to this connection, call Accept */
RvCCConnection* rvCCProviderCreateConnection(
                RvCCProvider*   x,
                RvCCTerminal*   t);

typedef RvCCConnection* (*RvCCProviderCreateConnectionF)(RvCCProvider* x,RvCCTerminal* t);

/* called by the application at start up time */
RvCCTerminal* rvCCProviderCreateTerminal(RvCCProvider* x, char* termId);
typedef RvCCTerminal* (*RvCCProviderCreateTerminalF)(RvCCProvider* x, char* termId);

typedef struct RvCCProviderFuncs_
{
    RvCCProviderCreateConnectionF   createConnectionF;
    RvCCProviderCreateTerminalF     createTerminalF;
} RvCCProviderFuncs;

/*-----------------------------------------------------------------------
                    P R O V I D E R     C A L L B A C K S
 ------------------------------------------------------------------------*/

typedef RvCCTerminal* RvCCTerminalPtr;
#define RvCCTerminalPtrDestruct       rvDefaultDestruct
#define RvCCTerminalPtrConstructCopy  rvDefaultConstructCopy

rvDeclareMap(RvCharPtr,RvCCTerminalPtr)

/******************************************************************************
*  rvCCTextPrintEvent
*  ----------------------------
*  General :       Prints the connection Terminal Events.
*
*  Return Value:   None.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          conn                 pointer to Call Control Connection
*       .          origEventId          pointer to original Event
*                  newEventId           new event
*                  reason               Event Cause
*  Output          None.
*
*
*
******************************************************************************/
#if (RV_LOGMASK & RV_LOGLEVEL_INFO)
void rvCCTextPrintEvent(
                IN      RvCCConnection*     conn,
                INOUT   RvCCTerminalEvent*  origEventId,
                IN      RvCCTerminalEvent   newEventId,
                INOUT   RvCCEventCause*     origReason,
                IN      RvCCEventCause      newReason);

/******************************************************************************
*  rvCCTextPrintMedia
*  ----------------------------
*  General :       Returns a string containing media capabilities
*  Return Value:   constant string
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           x                     Connection for which media are printed
*                   media                 Object containing Media Capabilities
*                   headline              String to be printed as headline
*                   logLevelDebug         Indicates in which log level fucntion will print
*                                         the media parameters
*
*  Output           None.
*
******************************************************************************/
void rvCCTextPrintMedia(
                RvCCConnection*                 x,
                const RvMdmMediaStreamInfo*     media,
                char*                           headline,
                RvLogMessageType                logLevel);

/******************************************************************************
*  rvCCTextPrintSdpMsg
*  ----------------------------
*  General :       Returns a string containing media capabilities
*  Return Value:   constant string
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           sdp                   SDP message to be printed
*                   headline              String to be printed as headline
*                   logLevelDebug         Indicates in which log level fucntion will print
*                                         the media parameters
*
*  Output           None.
*
******************************************************************************/
void rvCCTextPrintSdpMsg(
                        RvSdpMsg*           sdp,
                        char*               headline,
                        RvLogMessageType    logLevel);

#else
#define rvCCTextPrintEvent(_conn, _origEventId, _newEventId, _reason, _newReason)
#define rvCCTextPrintMedia(_x, _media, _headline, _logLevel)
#define rvCCTextPrintSdpMsg(_sdp, _headline, _logLevel)
#endif

#endif /* RV_CCAPI_H */


