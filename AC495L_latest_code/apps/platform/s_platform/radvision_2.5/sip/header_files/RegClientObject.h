/*
*********************************************************************************
*                                                                               *
* NOTICE:                                                                       *
* This document contains information that is confidential and proprietary to    *
* RADVision LTD.. No part of this publication may be reproduced in any form     *
* whatsoever without written prior approval by RADVision LTD..                  *
*                                                                               *
* RADVision LTD. reserves the right to revise this publication and make changes *
* without obligation to notify any person of such revisions or changes.         *
*********************************************************************************
*/


/*********************************************************************************
 *                              <RegClientObject.c>
 *
 * This file defines the RegClient object, attributes and interface functions.
 *
 *    Author                         Date
 *    ------                        ------
 *    Tamar Barzuza                 Jan 2001
 *********************************************************************************/

#ifndef REG_CLIENT_OBJECT_H
#define REG_CLIENT_OBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------*/
/*                        INCLUDE HEADER FILES                           */
/*-----------------------------------------------------------------------*/
#include "RV_SIP_DEF.h"
#ifndef RV_SIP_PRIMITIVES
#include "RvSipRegClientTypes.h"
#include "_SipTransaction.h"
#include "RegClientMgrObject.h"
#include "RvSipExpiresHeader.h"
#include "RvSipPartyHeader.h"
#include "RvSipAuthenticator.h"
#include "_SipCommonTypes.h"
#include "RvSipCompartmentTypes.h"

#ifdef RV_SIP_AUTH_ON
#include "_SipAuthenticator.h"
#endif /*#ifdef RV_SIP_AUTH_ON*/
#ifdef RV_SIP_IMS_ON
#include "RvSipSecAgreeTypes.h"
#endif /* #ifdef RV_SIP_IMS_ON */


/* RegClient
 * --------------------------------------------------------------------------
 * The RegClient is a REGISTER client object. It is used to request registration
 * from a chosen server according to the rules defined in the RFC 2543.
 * It supportes redirect and un-authenticated responses.
 *
 * pAuthListInfo -- Information regarding to the hListAuthObj.
 * hListAuthObj  -- List of Authentication Data structures,
 *                  each of which contains challenge, received with 401/407 response.
 * hInitialAuthorization     -- Handle to the empty authorization header, to be set in
 *                            the outgoing initial request.
 * -----------------------
 * version 4.5 parameters
 * -----------------------
 * hSecObj       -- Handle to the Security Object
 * hSecAgree         -- A security-agreement associated with this reg-client. The reg-client will 
 *                      obtain a security-agreement with the server and maintain it via 
 *                      this security-agreement object.
 */
typedef struct
{
    RvSipTransportObjEventInfo        terminationInfo; /* Information required for the object termination
                                                        in case first try to send object termination
                                                        event failed due to no enough resources */
    RvInt32                        strCallId;
    RvBool                         bIsDetach;        /* true if the register client is independent in managing its call-id and cseq step */
    HPAGE                           hPage;             /* an inner memory page */
    RvSipAppRegClientHandle         hAppRegClient;   /* The application handle to the register-client*/
    RvSipRegClientState             eState;          /* The inner state of the register-client */
    RvBool                         bWasRegistered;  /* Indicates if the regClient was once in state registered*/
    RvSipAddressHandle              hRequestUri;     /* The Request-Uri of the registrar */
    RvSipPartyHeaderHandle          hFromHeader;     /* From header assosiated with the register-client */
    RvSipPartyHeaderHandle          hToHeader;       /* To header assosiated with the register-client */
    RLIST_HANDLE                    hContactList;    /* A list of all Contact headers assosiated with this
                                                        register-client */
    RvSipExpiresHeaderHandle        hExpireHeader;   /* Expires header assosiated with this register-client */
    RvSipTranscHandle               hActiveTransc;   /* A transaction to be used to comunicate with the server */
    RegClientMgr                   *pMgr;            /* the register-clients manager */
    SipCSeq                         cseq;            /* The CSeq-Step assosiated with this register-client */
    SipTripleLock                    *tripleLock;     /* Processing/API & object locks */
    SipTripleLock                    regClientTripleLock; /* RegClient object only Processing/API & object locks */
    RvInt32                        regClientUniqueIdentifier; /*A unique identifier for this
                                                                reg client (randomly generated
                                                                when the reg client is created.*/
    RvSipMsgHandle                  hReceivedMsg;    /* The last message to be received by this register-client
                                                      (when relevant) */
    RvSipMsgHandle                  hOutboundMsg;    /* The next message to be sent by this register client
                                                      (when relevant */
    RvSipRegClientEvHandlers        *regClientEvHandlers;
#ifdef RV_SIP_AUTH_ON
    AuthObjListInfo                 *pAuthListInfo;
    RLIST_HANDLE                    hListAuthObj;
#endif /* #ifdef RV_SIP_AUTH_ON */
    SipTransportObjLocalAddresses   localAddr;       /* The local IP port and UDP socket to send messages from */
    SipTransportOutboundAddress     outboundAddr;    /* */
    RvSipTransportConnectionHandle  hTcpConn;
    RvBool                          bIsPersistent;
#ifdef SIP_DEBUG
    RvChar                         *pCallId;
#endif
#ifdef RV_SIGCOMP_ON
    RvBool                            bSigCompEnabled;     /* Indication if the application chose to stop  */
                                                         /* the SigComp processing in outgoing messages  */
    RvSipCompartmentHandle          hSigCompCompartment; /* Handle to the compartment that is related to */
                                                         /* the RegClient.                               */
#endif
    /* Pointer to the transaction's timers timeouts values.
       points to the structure allocated on it's page */
    RvSipTimers*          pTimers;
    RvBool                bUseFirstRouteRegisterRequest; /*The register should use the first route*/

#if (RV_NET_TYPE & RV_NET_SCTP)
    RvSipTransportSctpMsgSendingParams sctpParams;
#endif
    
#ifdef RV_SIP_IMS_ON
    RvSipAuthorizationHeaderHandle hInitialAuthorization;
#endif /*#ifdef RV_SIP_IMS_ON*/
    
	/* new parameters - version 4.5 */
#ifdef RV_SIP_IMS_ON
    RvSipSecAgreeHandle            hSecAgree;
    RvSipSecObjHandle   hSecObj;    /* Handle to the Security Object */
#endif /* #ifdef RV_SIP_IMS_ON */

} RegClient;


/***************************************************************************
 * RegClientInitiate
 * ------------------------------------------------------------------------
 * General: Initiate a new register-client object.
 * Return Value: RV_ERROR_OUTOFRESOURCES - Not enough resources to initiate a new
 *                                   RegClient object.
 *               RV_OK -        Success.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:     pRegClientMgr - pointer to the register-clients manager.
 *          pRegClient - pointer to the new register-client object.
 *          hAppRegClient - The API's handle to the register-client.
 ***************************************************************************/
RvStatus RVCALLCONV RegClientInitiate(
                                  IN RegClientMgr           *pRegClientMgr,
                                  IN RegClient              *pRegClient,
                                  IN RvSipAppRegClientHandle hAppRegClient);


/***************************************************************************
 * RegClientDestruct
 * ------------------------------------------------------------------------
 * General: destruct a register-client object. Recycle all allocated memory.
 * Return Value: RV_OK -        Success.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:     pRegClientMgr - pointer to the register-clients manager.
 *          pRegClient - pointer to the register-client object.
 ***************************************************************************/
RvStatus RVCALLCONV RegClientDestruct(IN RegClient *pRegClient);

/***************************************************************************
 * RegClientChangeState
 * ------------------------------------------------------------------------
 * General: Change the register-client state to the state given. Call the
 *          EvStateChanged callback with the new state.
 * Return Value: -
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: eNewState - The new state of the register-client.
 *        eReason - The reason for the state change.
 *        pRegClient - The register-client in which the state has
 *                       changed.
 * Output:(-)
 ***************************************************************************/
void RVCALLCONV RegClientChangeState(
                            IN  RvSipRegClientState             eNewState,
                            IN  RvSipRegClientStateChangeReason eReason,
                            IN  RegClient                       *pRegClient);

/***************************************************************************
 * RegClientGenerateCallId
 * ------------------------------------------------------------------------
 * General: Generate a unique CallId for this re-boot cycle.
 * Return Value: RV_ERROR_OUTOFRESOURCES - The register-client manager page was full
 *               RV_OK - Call-Id was generated successfully.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:     pRegClientMgr - Pointer to the register-clients manager.
 *          hPage - The memory page to use.
 * Output:  pstrCallId - Pointer to the call-id offset
 *          ppCallId - Pointer to the call-id pointer offset (NULL when sip-debug)
 ***************************************************************************/
RvStatus RVCALLCONV RegClientGenerateCallId(
                                         IN  RegClientMgr            *pRegClientMgr,
                                         IN  HPAGE                    hPage,
                                         OUT RvInt32                *pstrCallId,
                                         OUT RvChar                **ppCallId);


/***************************************************************************
 * RegClientGetCallId
 * ------------------------------------------------------------------------
 * General:Returns the Call-Id of the reg-client object.
 * Return Value: RV_ERROR_INSUFFICIENT_BUFFER - The buffer given by the application
 *                                       was isdufficient.
 *               RV_ERROR_NOT_FOUND           - The reg-client does not contain a call-id
 *                                       yet.
 *               RV_OK            - Call-id was copied into the
 *                                       application buffer. The size is
 *                                       returned in the actualSize param.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:     hRegClient   - The Sip Stack handle to the reg-client.
 *          bufSize    - The size of the application buffer for the call-id.
 * Output:     strCallId  - An application allocated buffer.
 *          actualSize - The call-id actual size.
 ***************************************************************************/
RvStatus RVCALLCONV RegClientGetCallId (
                            IN  RegClient              *pRegClient,
                            IN  RvInt32             bufSize,
                            OUT RvChar              *pstrCallId,
                            OUT RvInt32             *actualSize);

/***************************************************************************
 * RegClientAddContactsToMsg
 * ------------------------------------------------------------------------
 * General: Inserts to the message the list of Contact headers assosiated
 *          with this register-client.
 * Return Value: RV_ERROR_NULLPTR - pTransaction or hMsg are NULL pointers.
 *                 RV_OutOfResource - Couldn't allocate memory for the new
 *                                    message, and the assosiated headers.
 *               RV_OK - Success.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: pRegClient - The handle to the register-client.
 *          hMsg - The message to insert the Contact headers to.
 ***************************************************************************/
RvStatus RVCALLCONV RegClientAddContactsToMsg(
                                            IN RegClient     *pRegClient,
                                            IN RvSipMsgHandle hMsg);

/***************************************************************************
 * RegClientSendRequest
 * ------------------------------------------------------------------------
 * General: Request with REGISTER method on the active transaction of the
 *          register-client object.
 * Return Value: RV_OK - success.
 *               RV_ERROR_OUTOFRESOURCES - Not enough resources request.
 *               RV_ERROR_UNKNOWN - failure sending the request.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:     pRegClient - The pointer to the register-client object.
 *          hRequestUri - The Request-Uri to send the request to.
 ***************************************************************************/
RvStatus RVCALLCONV RegClientSendRequest(
                                       IN  RegClient         *pRegClient,
                                       IN RvSipAddressHandle  hRequestUri);

#if (RV_THREADNESS_TYPE != RV_THREADNESS_SINGLE)

/************************************************************************************
 * RegClientLockAPI
 * ----------------------------------------------------------------------------------
 * General: Locks regclient according to API schema
 * Return Value: RvStatus
 * ----------------------------------------------------------------------------------
 * Arguments:
 * Input:     pRegClient - pointer to the regclient.
***********************************************************************************/
RvStatus RVCALLCONV RegClientLockAPI(IN  RegClient *pRegClient);


/************************************************************************************
 * RegClientUnLockAPI
 * ----------------------------------------------------------------------------------
 * General: Locks regclient according to API schema
 * Return Value: RvStatus
 * ----------------------------------------------------------------------------------
 * Arguments:
 * Input:     pRegClient - pointer to the regclient.
***********************************************************************************/
void RVCALLCONV RegClientUnLockAPI(IN  RegClient *pRegClient);

/************************************************************************************
 * RegClientLockMsg
 * ----------------------------------------------------------------------------------
 * General: Locks regclient according to MSG processing schema
 * Return Value: RvStatus
 * ----------------------------------------------------------------------------------
 * Arguments:
 * Input:     pRegClient - pointer to the regclient.
***********************************************************************************/
RvStatus RVCALLCONV RegClientLockMsg(IN  RegClient *pRegClient);


/************************************************************************************
 * RegClientUnLockMsg
 * ----------------------------------------------------------------------------------
 * General: UnLocks regclient according to MSG processing schema
 * Return Value: RvStatus
 * ----------------------------------------------------------------------------------
 * Arguments:
 * Input:     pRegClient - pointer to the regclient.
***********************************************************************************/
void RVCALLCONV RegClientUnLockMsg(IN  RegClient *pRegClient);

#else
#define RegClientLockAPI(a) (RV_OK)
#define RegClientUnLockAPI(a)
#define RegClientLockMsg(a) (RV_OK)
#define RegClientUnLockMsg(a)
#endif
/***************************************************************************
 * RegClientMgrLock
 * ------------------------------------------------------------------------
 * General: Lock the register-client manager.
 * Return Value: -
 * ------------------------------------------------------------------------
 * Arguments:
 *    Input: hLock - The lock to un-lock.
 ***************************************************************************/
void RVCALLCONV RegClientMgrLock(IN RegClientMgr *pRegClientMgr);


/***************************************************************************
 * RegClientMgrUnLock
 * ------------------------------------------------------------------------
 * General: Lock the register-client manager.
 * Return Value: -
 * ------------------------------------------------------------------------
 * Arguments:
 *    Input: hLock - The lock to un-lock.
 ***************************************************************************/
void RVCALLCONV RegClientMgrUnLock(IN RegClientMgr *pRegClientMgr);

/***************************************************************************
 * RegClientTerminate
 * ------------------------------------------------------------------------
 * General: Terminates a CallLeg object, Free the resources taken by it and
 *          remove it from the manager call-leg list.
 * Return Value: (-)
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:     pCallLeg - Pointer to the call-leg to terminate.
 *          eReason - termination reason
 ***************************************************************************/
void RVCALLCONV RegClientTerminate(IN RegClient *pRegClient,
                                   RvSipRegClientStateChangeReason eReason);

/***************************************************************************
 * RegClientAttachOnConnection
 * ------------------------------------------------------------------------
 * General: Attach the RegClient as the connection owner
 * Return Value: RvStatus
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:     pRegClient    - pointer to RegClient
 *            hConn         - The connection handle
 ***************************************************************************/
RvStatus RegClientAttachOnConnection(IN RegClient                        *pRegClient,
                                 IN RvSipTransportConnectionHandle  hConn);


/***************************************************************************
 * RegClientDetachFromConnection
 * ------------------------------------------------------------------------
 * General: detach the RegClient from the connection owner
 * Return Value: RvStatus
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:     pRegClient    - pointer to RegClient
 ***************************************************************************/
void RegClientDetachFromConnection(IN RegClient        *pRegClient);


/***************************************************************************
 * RegClientConnStateChangedEv
 * ------------------------------------------------------------------------
 * General: This is a callback function implementation called by the transport
 *          layer when ever a connection state was changed.
 * Return Value:RvStatus
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:     hConn   -   The connection handle
 *          hOwner  -   Handle to the connection owner.
 *          eStatus -   The connection status
 *          eReason -   A reason for the new state or undefined if there is
 *                      no special reason for
 ***************************************************************************/
RvStatus RVCALLCONV RegClientConnStateChangedEv(
            IN  RvSipTransportConnectionHandle             hConn,
            IN  RvSipTransportConnectionOwnerHandle        hRegClient,
            IN  RvSipTransportConnectionState              eState,
            IN  RvSipTransportConnectionStateChangedReason eReason);


/***************************************************************************
 * RegClientStateIntoString
 * ------------------------------------------------------------------------
 * General: returns the string for the state received as enumeration.
 * Return Value: The state string.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: eNewState - The state enumeration.
 ***************************************************************************/
RvChar *RVCALLCONV RegClientStateIntoString(
                                            IN RvSipRegClientState eNewState);

#ifdef RV_SIGCOMP_ON
/***************************************************************************
 * RegClientSetCompartment
 * ------------------------------------------------------------------------
 * General: Associates the RegClient to a SigComp Compartment.
 * Return Value: RvStatus
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:     pRegClient   - The regClient handle.
 *             hCompartment - Handle to the SigComp Compartment.
 ***************************************************************************/
RvStatus RVCALLCONV RegClientSetCompartment(
                            IN RegClient             *pRegClient,
                            IN RvSipCompartmentHandle hCompartment);

/***************************************************************************
 * RegClientHandleSigCompTransc
 * ------------------------------------------------------------------------
 * General: The function should be called when a SigComp-SIP message is
 *          about to be send. According to the SigComp model each SigComp-SIP
 *          message MUST be related to any SigComp compartment. Thus, the
 *          function verifies that the RegClient is currently associated
 *          with a SigComp Compartment and that its' SigComp transaction
 *          is part of this comparment. In case that the RegClient is not
 *          part of any compartment, then new one is created for the
 *          RegClient (and its transactions) use, and the transaction
 * Return Value: RvStatus
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:     pRegClient   - The regClient handle.
 ***************************************************************************/
RVAPI RvStatus RVCALLCONV RegClientHandleSigCompTransc(
                            IN RvSipTranscHandle  hTransc,
                            IN RegClient         *pRegClient);

/***************************************************************************
 * RegClientResetSigCompDataForDestChange
 * ------------------------------------------------------------------------
 * General: The function reset the SigComp data that was stored until now in
 *          order to avoid misunderstanding of the next sent message by the
 *          new remote destination, that doesn't have this data.
 * Return Value: RvStatus
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:     pRegClient     - A pointer to the RegClient.
 ***************************************************************************/
RvStatus RVCALLCONV RegClientResetSigCompDataForDestChange(IN RegClient *pRegClient);

#endif /* RV_SIGCOMP_ON */

/***************************************************************************
 * RegClientSetTimers
 * ------------------------------------------------------------------------
 * General: Sets timeout values for the reg-client's transactions timers.
 * Return Value: RvStatus.
 * ------------------------------------------------------------------------
 * Arguments:
 *    Input: pRegClient - Handle to the reg-client object.
 *           pTimers - Pointer to the struct contains all the timeout values.
 ***************************************************************************/
RvStatus RegClientSetTimers(IN  RegClient*      pRegClient,
                            IN  RvSipTimers*  pNewTimers);

#endif /* RV_SIP_PRIMITIVES */

#ifdef RV_SIP_IMS_ON
/******************************************************************************
 * RegClientSetSecObj
 * ----------------------------------------------------------------------------
 * General: Sets Security Object into the Reg-Client.
 *          As a result of this operation, all messages, sent by this
 *          Reg-Client, will be protected with the security mechanism,
 *          provided by the Security Object.
 * Return Value: RV_OK on error,
 *          error code, defined in RV_SIP_DEF.h or rverror.h, otherwise.
 * ----------------------------------------------------------------------------
 * Arguments:
 *  Input:  pRegClient - Pointer to the Reg-Client object.
 *          hSecObj    - Handle to the Security Object. Can be NULL.
 *****************************************************************************/
RvStatus RegClientSetSecObj(IN  RegClient*        pRegClient,
                            IN  RvSipSecObjHandle hSecObj);
#endif /*#ifdef RV_SIP_IMS_ON*/

#ifdef __cplusplus
}
#endif


#endif /* END OF: #ifndef REG_CLIENT_OBJECT_H */
