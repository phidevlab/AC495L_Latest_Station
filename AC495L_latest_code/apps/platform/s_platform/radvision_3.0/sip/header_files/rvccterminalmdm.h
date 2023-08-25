/******************************************************************************
Filename:    rvccterminalmdm.h
Description: Mdm Terminal Header
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
#ifndef RV_CCTERMINAL_MDM_H
#define RV_CCTERMINAL_MDM_H

#include "rvstring.h"
#include "rvtimer.h"
#include "rvvector.h"
#include "rvccapi.h"
#include "rvccconnmdm.h"
#include "rvmdmobjects.h"
#include "rvmdm.h"
#include "rvmdmmediastream.h"
#include "rvmdmtermdigitmap.h"
#include "rvmdmtermsignal.h"
#include "rvccCfwMgr.h"

/* Maximal number of simultaneous connections
   (lines) for regular MDM termination (IP Phone)
   and analog MDM termination (Analog line).
   Has to be less than RV_CCTERMINAL_MAXCONNS
*/
/* NirS */ 
//NBE: hanselhe, use unified MACRO
#ifdef CONFIG_320
#define RV_CCTERMINALMDM_MAXCONNS      3   /* NirM - Currently we support only 2 lines */ 
#elif CONFIG_310
#define RV_CCTERMINALMDM_MAXCONNS      2 
#elif CONFIG_350
#define RV_CCTERMINALMDM_MAXCONNS      6
#elif CONFIG_ININ
#define RV_CCTERMINALMDM_MAXCONNS      1
#else
#define RV_CCTERMINALMDM_MAXCONNS      2
#endif

#define RV_CCTERMINALMDM_ANALOG_MAXCONNS	RV_CCTERMINALMDM_MAXCONNS

#define  PHYS_TERM_STREAM_ID    1 /* A physical Terminal (Audio/Video) has ONLY 1 stream (streamId=1)*/

/* Timeout for event timer */
#define RV_MDMTERM_EVENTTIME	20

#define RV_BLOCKEDEVENT_NOTIFY		0
#define RV_BLOCKEDEVENT_REGISTER	0x01
#define RV_BLOCKEDEVENT_UNREGISTER	0x02

rvDeclareList(RvMdmEvent)

typedef struct RvCCTerminalMdm_
{
    RvCCProvider*                       provider;
    RvMdmTerm                           mdmTerm;
	char							    terminalId[RV_NAME_STR_SZ]; 
    RvCCTerminalType                    termType;
    RvMdmTermType                       type;
    RvBool                              inactive;
    RvAlloc*                            alloc;
    RvMutex                             mutex;

	/* Audio Terminations*/
	RvCCTerminal*   activeAudioTerm;
	RvCCTerminal*   handsfreeAudioTerm;
	RvCCTerminal*   handsetAudioTerm;
	RvCCTerminal*	headsetAudioTerm;

	/* Video Terminations*/
	RvCCTerminal*   activeCameraTerm;
	RvCCTerminal*   activeScreenTerm;
	RvCCTerminal*   cameraVideoTerm;
	RvCCTerminal*	screenVideoTerm;

	RvMdmEvent							lastEvent;

	RvMdmMediaDescriptor				mediaDescr;/*List of all media streams */
	RvMdmMediaDescriptor*				mediaCaps; /*Media capabilities */

	/* Streams */
	RvVector(RvMdmMediaStreamInfo)      streamList;
	RvMdmMediaStreamInfo				mediaEventData; /*data for media event (for example: modify media)*/

	/*----------------------------------*/
	/* Event processing state variables */
	/*----------------------------------*/
	RvMdmEventsDescriptor               eventsDescr;
    const RvChar                        *compEventPkg; /*Completion event package, different for analog and ip phone*/
	RvBool								digitMapActive;
	RvMdmDigitMap						digitMap;
	RvString							dialString;
	RvString							redialString;
	RvMdmDigitMapData *					digitMapEC;
	/* Current status of the digitmap matching */
	RvMdmDigitMapMatchType				digitMapStat;
	IppTimer							digitMapTimer[2];
	IppTimer*							digitMapCurTimer;
	RvMdmMediaStream*					digitMapMedia;
	/* Requested event for digitmap event */
	const RvMdmRequestedEvent *			digitMapReqEvent;

	/* Store previously defined digitmaps */
	RvMdmDigitMapDB						storedDigitMaps;

	/*----------------------------------*/
	/* Signals variables                */
	/*----------------------------------*/
	RvList(RvMdmCurSignal)				curSignals;
	RvList(RvMdmCurSignalList)			curSignalLists;

	/*-----------------------------------------------*/
	/* Used to queue events when processing commands */
	/*-----------------------------------------------*/
	RvMutex								blockedEventMutex;
    RvList(RvMdmEvent)                  blockedEventBuffer;
	unsigned int						blockedEventTypeMask;

    IppTimer                            dialToneTimer;

    /* Termination default state */
	RvMdmTermDefaultProperties          defaultState;

	/*-----------------------------------------------*/
	/* DTMF related                                  */
	/*-----------------------------------------------*/
	RvBool								outOfBandDtmfRelayEnabled;
	RvBool								playDtmfEnabled;

	RvCCConnection*						holdingConnection; /*the connection that is on Hold*/

	RvMdmTermPhoneNumbers				phoneNumbers;
	RvMdmTermPresentationInfo			presentationInfo;
	rvCCCfwData							cfwData;
} RvCCTerminalMdm;

void rvCCTerminalMdmConstruct(
    IN RvCCTerminal*                x,
    IN RvCCProvider*                p,
    IN const RvChar*                id,
    IN RvCCTerminalType             type,
    IN RvCCTerminalClbks*           clbks,
    IN RvMdmTermDefaultProperties*  defaultProperties,
    IN RvAlloc*                     a);
void rvCCTerminalMdmDestruct(RvCCTerminal * x);

const RvChar* rvCCTerminalMdmGetTermId(IN RvCCTerminal* term);

void rvCCTerminalMdmTerminateAllCalls(RvCCTerminal* t);

#define rvCCTerminalMdmGetImpl(x)					((RvCCTerminalMdm*)x->terminal)

#define rvCCTerminalMdmGetLastEvent(x)				(&(x)->lastEvent)


/*API for Handling Blocked events*/
#define rvCCTerminalMdmBlockedEventIsObsOn(x)	((x)->blockedEventTypeMask==RV_BLOCKEDEVENT_NOTIFY)
#define rvCCTerminalMdmBlockedEventIsRegOn(x)	((x)->blockedEventTypeMask&RV_BLOCKEDEVENT_REGISTER)
#define rvCCTerminalMdmBlockedEventIsUnregOn(x)	((x)->blockedEventTypeMask&RV_BLOCKEDEVENT_UNREGISTER)
#define rvCCTerminalMdmBlockedEventSetReg(x, b)	((x)->blockedEventTypeMask=((b==rvTrue)?((x)->blockedEventTypeMask|RV_BLOCKEDEVENT_REGISTER):((x)->blockedEventTypeMask&~RV_BLOCKEDEVENT_REGISTER)))
#define rvCCTerminalMdmBlockedEventSetUnreg(x, b)	((x)->blockedEventTypeMask=((b==rvTrue)?((x)->blockedEventTypeMask|RV_BLOCKEDEVENT_UNREGISTER):((x)->blockedEventTypeMask&~RV_BLOCKEDEVENT_UNREGISTER)))

void rvCCTerminalMdmProcessEvent(RvCCTerminal* x, const char* pkg, const char* id,
										 RvMdmMediaStream* media, RvMdmParameterList * args);
/* Setters & Getters */
#define rvCCTerminalMdmIsPhysical(x)		((x)->type==RV_MDMTERMTYPE_PHYSICAL)
#define rvCCTerminalMdmIsEphemeral(x)		((x)->type==RV_MDMTERMTYPE_EPHEMERAL)
#define rvCCTerminalMdmIsUnknownType(x)		((x)->type==RV_MDMTERMTYPE_UNKNOWN)
#define rvCCTerminalMdmIsInactive(x)		((x)->inactive)

#define rvCCTerminalMdmGetProvider(x)		((x)->provider)
#define rvCCTerminalMdmGetMdmTerm(x)		(&((x)->mdmTerm))
#define rvCCTerminalMdmGetId(x)				((x)->terminalId)
#define rvCCTerminalMdmGetRtpTerm(x)		((x)->rtpTerm)
#define rvCCTerminalMdmGetType(x)			((x)->termType)
#define rvCCTerminalMdmGetCompEventPkg(x)	((x)->compEventPkg)
#define rvCCTerminalMdmGetHoldingConn(x)	((rvCCTerminalMdmGetImpl(x))->holdingConnection)
#define rvCCTerminalMdmGetDialToneTimer(x)	(&(rvCCTerminalMdmGetImpl(x)->dialToneTimer))

#define rvCCTerminalMdmSetRtpTerm(x, t)		((x)->rtpTerm=t)
#define rvCCTerminalMdmSetType(x, t)		((x)->type=t)
#define rvCCTerminalMdmSetInactive(x, b)		((x)->inactive=b)
#define rvCCTerminalMdmSetHandsetTerm(x, a)	((rvCCTerminalMdmGetImpl(x))->handsetAudioTerm=a)
#define rvCCTerminalMdmSetHeadsetTerm(x, a) ((rvCCTerminalMdmGetImpl(x))->headsetAudioTerm=a)
#define rvCCTerminalMdmSetHandsfreeTerm(x, a) ((rvCCTerminalMdmGetImpl(x))->handsfreeAudioTerm=a)
/* Set this now to be used later in case user wants to Transfer or Conference a Held call with an active one*/
#define rvCCTerminalMdmSetHoldingConn(x, a)	((rvCCTerminalMdmGetImpl(x))->holdingConnection=a)

#define rvCCTerminalMdmSetCameraTerm(x, a)	((rvCCTerminalMdmGetImpl(x))->cameraVideoTerm=a)
#define rvCCTerminalMdmSetScreenTerm(x, a) ((rvCCTerminalMdmGetImpl(x))->screenVideoTerm=a)

#define rvCCTerminalMdmGetHandsetTerm(x)	((rvCCTerminalMdmGetImpl(x))->handsetAudioTerm)
#define rvCCTerminalMdmGetHeadsetTerm(x) ((rvCCTerminalMdmGetImpl(x))->headsetAudioTerm)
#define rvCCTerminalMdmGetHandsfreeTerm(x) ((rvCCTerminalMdmGetImpl(x))->handsfreeAudioTerm)

#define rvCCTerminalMdmGetCameraTerm(x)	((rvCCTerminalMdmGetImpl(x))->cameraVideoTerm)
#define rvCCTerminalMdmGetScreenTerm(x) ((rvCCTerminalMdmGetImpl(x))->screenVideoTerm)

#define rvCCTerminalMdmIncColumnDisplay(x, a)	((x)->displayColumn+=a)
#define rvCCTerminalMdmGetColumnDisplay(x)		((x)->displayColumn)
#define rvCCTerminalMdmResetColumnDisplay(x)	((x)->displayColumn=0)

RvCCTerminal* rvCCTerminalMdmGetActiveAudioTerm(RvCCTerminal* t);
RvCCTerminal* rvCCTerminalMdmGetActiveVideoTerm(RvCCTerminal* t);
void rvCCTerminalMdmSetActiveVideoTerm(RvCCTerminal* t,RvCCTerminal* at);

struct RvMdmTermMgr_* rvCCTerminalMdmGetTermMgr(RvCCTerminalMdm * x);

RvBool rvCCTerminalMdmMoveSignals(RvCCTerminal* src,RvCCTerminal* dst);
const RvSdpMsg* rvCCTerminalMdmGetMediaCaps(RvCCTerminalMdm * x);
RvMdmMediaStreamInfo * rvCCTerminalMdmGetMediaStream(RvCCTerminalMdm * x,unsigned int id);
RvMdmStreamId rvCCTerminalMdmAddEmptyMediaStream(RvCCTerminalMdm * x);
void rvCCTerminalMdmClearAllMediaStreams(RvCCTerminalMdm * x);
void rvCCTerminalMdmClearMediaStream(RvCCTerminalMdm * x, RvUint32 streamId);
RvBool rvCCTerminalMdmCreateMedia(RvCCTerminalMdm* term, RvMdmMediaStreamInfo* media, RvMdmMediaDescriptor* rspMedia);
RvBool rvCCTerminalMdmUpdateRtpMedia(RvCCTerminalMdm* term, RvMdmMediaStreamInfo* media, RvMdmMediaDescriptor* rspMedia, RvSdpMsg* sdpMediaCaps);

#define rvCCTerminalMdmGetNumOfStreams(x)		(rvVectorSize(&((x))->streamList))
#define rvCCTerminalMdmGetMediaStreamElem(x,i)	rvVectorAt(&((x))->streamList,i)

RvCCTerminal* rvCCTerminalMdmSelectRtpTerm(RvCCTerminalMdm* term, RvCCConnection* c);
RvBool rvCCTerminalMdmIsFirstDigit(RvCCTerminal* x);
RvChar rvCCTerminalMdmGetLastDigit(RvCCTerminal* x);
RvCCConnection* rvCCTerminalMdmFindFreeConnection(RvCCTerminal* t);
char* rvCCTerminalMdmGetDialString(RvCCTerminal* t);
void rvCCTerminalMdmResetDialString(RvCCTerminal* t);
/* Transfer feature */
void rvCCTerminalMdmSetWaitForDigits(RvCCTerminal* x);


/******************************************************************************
*  rvCCTerminalMdmIsOutOfBandDtmfEnabled
*  --------------------------------------
*  General :        get from the configuration the state of OOD
*
*  Return Value:    rvTrue(support OOD),rvFalse(doesn't support OOD)
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          x            terminal object.
*       .
*
*
*
*
*  Output          None.
*
******************************************************************************/
RvBool rvCCTerminalMdmIsOutOfBandDtmfEnabled(RvCCTerminal* t);


RvBool rvCCTerminalMdmIsDtmfPlayEnabled(RvCCTerminal* t);
RvBool rvCCTerminalMdmSendCallerId(RvCCTerminal* x, const char* callerName,const char* callerNumber,
								  const char* address,const char* callerId);
RvBool rvCCTerminalMdmStartCallWaitingSignal(RvCCTerminal* x);
RvBool rvCCTerminalMdmStartCallWaitingCallerSignal(RvCCTerminal* x);
RvBool rvCCTerminalMdmSetHoldInd(RvCCTerminal* x, RvBool on);
RvBool rvCCTerminalMdmSetLineInd(RvCCTerminal* x, int lineId, RvCCTerminalIndState state);
RvBool rvCCTerminalMdmSendLineActive(RvCCTerminal* x, int lineId, RvBool active);
RvBool rvCCTerminalMdmSetMuteInd(RvCCTerminal* x, RvBool on);
RvBool rvCCTerminalMdmSetAudioInd(
    RvCCTerminal*				x,
								  char*						id,
	RvCCTerminalIndState        state);

const char* rvCCTerminalMdmGetPhoneNumber(RvCCTerminal* x);

RvBool rvCCTerminalMdmOtherHeldConnExist	(RvCCTerminal* t, RvCCConnection* currConn, RvCCConnection** otherHeldConn);
RvBool rvCCTerminalMdmOtherAlertingConnExist(RvCCTerminal* t, RvCCConnection* currConn);

RvMdmMediaStreamInfo* rvCCTerminalMdmGetMediaEventData(RvCCTerminalMdm* term);

void rvCCTerminalMdmSetMediaEventData(RvCCTerminalMdm* term, RvMdmMediaStreamInfo* media);
RvBool rvCCTerminalMdmStartSignalUI(
    IN RvCCTerminal*        x,
    IN const RvChar*        pkg,
    IN const RvChar*        id,
    IN RvMdmParameterList*  args);

RvBool rvCCTerminalMdmStartSignalAudio(
    IN RvCCTerminal*        x,
    IN const RvChar*        pkg,
    IN const RvChar*        id,
    IN RvMdmParameterList*  params);

RvCCTerminalEvent rvCCTerminalMdmMapEvent(RvCCTerminal*     x,
                                          const char*       pkg,
                                          const char*       id,
                                          RvMdmParameterList* args,
										  char*				key);

RvMdmDigitMapMatchType terminalMdmMatchDigitMap(
    IN RvMdmXTerm*      xTerm,
    IN const RvChar*    dialString,
    IN unsigned int*    timeDuration);

RvMdmTermType terminalMdmGetTermType(IN RvMdmXTerm* xTerm);

RvBool terminalMdmModifyMedia(
    IN      RvMdmTerm*  x,
    INOUT   RvSdpMsg*   sdpMsg,
	IN      RvChar*     id);

/*Phone Numbers*/
RvStatus terminalMdmSetPhoneNumber(
    IN RvMdmTerm*       x,
    IN const RvChar*    number);

RvMdmTermPhoneNumbers* rvCCTerminalMdmGetPhoneNumbers(RvCCTerminalMdm *term);


RvBool rvCCTerminalMdmLoadPresentationInfo(RvCCTerminalMdm *			term, 
											RvMdmTermDefaultProperties* termProperties);
RvMdmTermPresentationInfo* rvCCTerminalMdmGetPresentationInfo(RvCCTerminal *t);

RvMdmMediaStreamInfo* rvCCTerminalMdmCreateLineMedia(RvCCTerminal* t, 
													 RvCCTerminal* activeTerm);

RvMdmMediaStreamInfo* rvCCTerminalMdmSetDefaultMedia(
    IN RvCCTerminal*    t,
    IN int              streamId,
    IN const RvChar*    username);

void rvCCTerminalMdmFillSdpDefaultInfo(
    IN RvCCTerminal*    t,
    IN const RvSdpMsg*  sdp,
    IN const RvChar*    username);

const char* rvCCTerminalMdmGetLocalAddress(RvCCTerminal* t);

RvBool rvCCTerminalMdmIsAudioTerminationExist(
    IN RvCCTerminal*            t,
    IN RvCCTerminalAudioType    audio);
void rvCCTerminalMdmSetActiveAudio(
    IN RvCCTerminal*            t,
    IN RvCCTerminalAudioType    audio);
RvCCTerminalAudioType rvCCTerminalMdmGetActiveAudio(IN RvCCTerminal* t);
void rvCCTerminalMdmSetActiveAudioTerm(
    IN RvCCTerminal*    t,
    IN RvCCTerminal*    at);
RvCCTerminal* rvCCTerminalMdmGetActiveVideoCamTerm(IN RvCCTerminal* t);
RvCCTerminal* rvCCTerminalMdmGetActiveVideoScrTerm(IN RvCCTerminal* t);
void rvCCTerminalMdmSetActiveVideoCamTerm(
    IN RvCCTerminal*    t,
    IN RvCCTerminal*    at);
void rvCCTerminalMdmSetActiveVideoScrTerm(
    IN RvCCTerminal*    t,
    IN RvCCTerminal*    at);
void rvCCTerminalMdmStopSignals(IN RvCCTerminal* x);
RvBool rvCCTerminalMdmStartDialToneSignal(IN RvCCTerminal* x);
RvBool rvCCTerminalMdmStartRingingSignal(IN RvCCTerminal* x);
RvBool rvCCTerminalMdmStopRingingSignal(IN RvCCTerminal* x);
RvBool rvCCTerminalMdmStartRingbackSignal(IN RvCCTerminal* x);
RvBool rvCCTerminalMdmStartCallWaitingSignal(IN RvCCTerminal* x);
RvBool rvCCTerminalMdmStartCallWaitingCallerSignal(IN RvCCTerminal* x);
RvBool rvCCTerminalMdmStartBusySignal(IN RvCCTerminal* x);
RvBool rvCCTerminalMdmStartWarningSignal(IN RvCCTerminal* x);
RvBool rvCCTerminalMdmStartDTMFTone(
    IN RvCCTerminal*        x,
    IN RvDtmfParameters*    dtmfParam);
RvBool rvCCTerminalMdmStopDTMFTone(IN RvCCTerminal* x);
RvBool rvCCTerminalMdmSetDisplay(
    IN RvCCTerminal*    x,
    IN const RvChar*    text,
    IN RvInt32          row,
    IN RvInt32          column);
RvBool rvCCTerminalMdmClearDisplay(IN RvCCTerminal* x);
RvBool rvCCTerminalMdmSendCallerId(
    IN RvCCTerminal*    x,
    IN const RvChar*    callerName,
    IN const RvChar*    callerNumber,
    IN const RvChar*    callerAddress,
    IN const RvChar*    callerId);
void rvCCTerminalMdmInstallDigitMap(
    IN RvCCTerminal*        x,
    IN RvBool               activateDigitMap);
int rvCCTerminalMdmGetMaxConnections(IN RvCCTerminal* t);



#endif /* RV_CCTERMINAL_MDM_H*/
