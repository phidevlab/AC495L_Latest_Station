/******************************************************************************
Filename:    sipStun.h
Description:
*******************************************************************************
                Copyright (c) 2005 RADVISION Inc.
*******************************************************************************
NOTICE:
This document contains information that is proprietary to RADVISION LTD.
No part of this publication may be reproduced in any form whatsoever
without written prior approval by RADVISION LTD.

RADVISION LTD. reserves the right to revise this publication and make
changes without obligation to notify any person of such revisions or
changes.
******************************************************************************/
#ifndef SIP_STUN_H
#define SIP_STUN_H

#include "rvSipStunApi.h"
#include "rvccapi.h"

#define IPP_STUN_THREAD_PRIORITY    RV_THREAD_PRIORITY_DEFAULT
#define IPP_STUN_THREAD_STACK_SIZE  0x4000 /*16384*/
#define IPP_STUN_THREAD_QUEUE_SIZE  10
#define IPP_STUN_REQ_TIME_TO_LIVE_ms    3000
#define IPP_STUN_MAX_STUN_MSG_SIZE  100

rvDeclareList(RvIppStunAddrData)
typedef RvList(RvIppStunAddrData) RvStunAddrList;


/* MtfStunFrameType
 * ------------------------------------------------------------------------
 * The type of transaction the STUN frame holds.
 */
typedef enum
{
    MtfStunFrameCallLeg,
    MtfStunFrameRegClient,
    MtfStunFrameSubs
} MtfStunFrameType;


/* RvStun
 * ------------------------------------------------------------------------
 * This object is built while proceeding CallLegFinalDestResolvedEvIO or
 * RegClientFinalDestResolvedEv().
 * It contains full context of message to be sent.
 * It persists in stunMgr->database until addr mapping is complete, or error
 * happens, or transaction is closed as result of user action or timeout.
 */
typedef struct
{
    RvSipBodyHandle             hBody;
    RvSdpMsg                    sdpMsg;
    RvStunAddrList              stunAddrList; /* List of IP addresses found in the message */
    RvBool                      isViaOnly;

    RvSipMsgHandle              hMsgToSend; /* Handle to the SIP message we're dealing with */
    RvSipTranscHandle           hTransc; /* Handle of the SIP transaction we're dealing with */

    MtfStunFrameType            type; /* Type of transaction we're dealing with */

    void*                       sipHandle; /* SIP handle we're dealing with. This can be
                                              a call leg, a reg client or a subs handle */
    void*                       sipAppHandle; /* The application's handle of the relevant
                                                 SIP handle */

    IppTimer                    timeToLiveTimer;
} RvStun;


void    RvStunDestruct(IN RvStun* frame);
/*-----------------------------------------------------------------------*/
void RvStunNotifyError(RvStun  * frame);
/*-----------------------------------------------------------------------*/
typedef void (*RvStunResolvedHandlerCB)(IN RvStun* data);
typedef void (*RvStunErrorHandlerCB)(IN RvStun* data);
/*-----------------------------------------------------------------------*/


/*-----------------------------------------------------------------------*/
/*                        RvStunMgrDb                                    */
/*-----------------------------------------------------------------------*
* General:  database contained in stunMgr.
*           comprise of 2 table:
*           list of RvStun objects waiting for resolution of their addresses
*           list of RvIppStunAdrData objects -addresses waiting for resolution
*-----------------------------------------------------------------------*/
typedef struct _RvStunMgrDb
{
    RvPtrList           stunList; /*objects waiting for stun client response */
    RvStunAddrList      stunAddrDataList; /*addresses waiting for stun client response */
    RvMutex             mutex; /*synch object guarding access to the whole database*/

    RvStunResolvedHandlerCB resolvedHandler;
    RvStunErrorHandlerCB    errorHandler;
}RvStunMgrDb;
/*-----------------------------------------------------------------------*/
void    RvStunMgrDbDestruct( RvStunMgrDb*);
/*-----------------------------------------------------------------------*/
/*
 *  some of found addresses maybe already requested due to previous Stun requests
 * extract those addresses which are not requested yet
 */
RvStatus    RvStunMgrDbMergeRequest( INOUT RvStunMgrDb* db,
                                     IN RvStun*          frame);
/*-----------------------------------------------------------------------*/
RvStatus    RvStunMgrDbUpdateByResponse( INOUT RvStunMgrDb* db, IN RvIppStunAddrData* stunAddr);
/*-----------------------------------------------------------------------*/

/*
 *  hObject - key to search in DB. Maybe frame->hTransc, frame->hTransc, frame->hRegClient, frame->hTransc, &frame->timeToLiveTimer
 */
RvStatus    RvStunMgrDbDeleteFrameByKey( INOUT RvStunMgrDb* db, void* hObject);
/*-----------------------------------------------------------------------*/
RvStatus    RvStunMgrDbRemoveFrame( INOUT RvStunMgrDb* db, RvStun* frame);
/*-----------------------------------------------------------------------*/



/*-----------------------------------------------------------------------*/
/*                        RvStunMsgData                                  */
/*-----------------------------------------------------------------------*
* General:  keep stun msg while sending it over SIP stack socket.        *
*                                                                        *
*------------------------------------------------------------------------*/
typedef struct
{
    RvSipTransmitterHandle  hTrx;
    RvUint8                 buf[IPP_STUN_MAX_STUN_MSG_SIZE];
    RvSize_t                size;
}RvStunMsgData;
/*-----------------------------------------------------------------------*/
void    RvStunMsgDataConstruct( RvStunMsgData*,
                                   RvSipStackHandle hStack,
                                   IN RvUint8* buf,
                                   IN RvSize_t size);
/*-----------------------------------------------------------------------*/
void    RvStunMsgDataDestruct( RvStunMsgData*);
/*-----------------------------------------------------------------------*/
RvStatus    RvStunMsgDataSend(  RvStunMsgData* data, RvSipMsgHandle hMsgToSend);
/*-----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/*                        RvStunMsgData*List                             */
/*-----------------------------------------------------------------------*
* General:  pointer container to stun msg                                *
*                                                                        *
*------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
RvStunMsgData*  RvStunMsgDataListFind( RvPtrList* l, RvSipTransmitterHandle hTrx);
/*-----------------------------------------------------------------------*/


/*-----------------------------------------------------------------------*/
/*                        RvStunMgr                                      */
/*-----------------------------------------------------------------------*
* General:  stunMgr singleton.
*
*-----------------------------------------------------------------------*/
typedef struct
{
    /* Application related callbacks for STUN */
    RvIppStunIsAddressResolveNeededCB   isAddressResolveNeededCB;
    RvIppStunAddressResolveStartCB      addressResolveStartCB;
    RvIppAddressResolveReplyCB          addressResolveReplyCB;

    /* Old stack event callbacks which STUN needs to override */
    RvSipCallLegStateChangedEv          old_pfnCallLegStateChangedEvHandler;
    RvSipSubsFinalDestResolvedEv        old_pfnSubsFinalDestResolvedEvHandler;
    RvSipCallLegFinalDestResolvedEv     old_pfnFinalDestResolvedEvHandler;
    RvSipRegClientFinalDestResolvedEv   old_pfnRegClientFinalDestResolvedEvHandler;
    RvSipTransactionStateChangedEv      old_pfnTransEvStateChanged;
    RvSipTransportBufferReceivedEv      old_pfnTtransportEvBufferReceived;

    /*database*/
    RvStunMgrDb                         database;

    /*objects to send msg*/
    RvPtrList                           stunMsgDataList;
    RvMutex                             stunMsgDataMutex;

    SendMethod                          sendMethod;
        /* Contains bound address and method allowing
           to send raw buffer from bound address. This is left here for
           backward compatibility a lot more than due to any real need... */
} RvStunMgr;


#endif /* SIP_STUN_H*/
