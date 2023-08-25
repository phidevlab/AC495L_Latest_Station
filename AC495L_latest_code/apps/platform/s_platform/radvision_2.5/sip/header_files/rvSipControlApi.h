/******************************************************************************
Filename:    rvSipControlApi.h
Description: Sip Phone Application - protocol specific APIs
*******************************************************************************
                Copyright (c) 2004 RADVISION
*******************************************************************************
NOTICE:
This document contains information that is proprietary to RADVISION.
No part of this publication may be reproduced in any form whatsoever
without written prior approval by RADVISION.

RADVISION reserves the right to revise this publication and make changes
without obligation to notify any person of such revisions or changes.
******************************************************************************/
#ifndef SIPCONTROL_API_H
#define SIPCONTROL_API_H
/*********************************************************************************
 *                              <rvSipControlApi.h>
 *
 * This file contains the SIP CONTROL API which may be used by the User of the
 * IP Phone Toolkit, in order to tailor the behaviour to its needs,
 *
 *
 * The SIP Cpntrol  API functions are grouped as follows:
 *
 * The Extension API
 * ------------------------
 *  TO DO:
 *
 * The Adaptor  API
 * -----------------
 * TO DO:
 *
 *    Author                         Date
 *    ------                        ------
 *
 *********************************************************************************/


#ifdef __cplusplus
extern "C" {
#endif


#include "rvmdm.h"
#include "RvSipStackTypes.h"
#include "RvSipCallLegTypes.h"
#include "RvSipRegClientTypes.h"
#include "RvSipSubscriptionTypes.h"
#include "RvSipAuthenticator.h"
#include "rvIppCfwApi.h"

#ifdef RV_CFLAG_TLS
#include "sipTls.h"
#endif


/* RvIppSipControlHandle
 * ----------------------------------------------------------------------------
 * SIP Control object handle.
 * SIP Control handle enables the user application with better control of the
 * SIP Stack in changing or adding to the default functionality of the
 * Multimedia Terminal Framework.
 */
RV_DECLARE_HANDLE(RvIppSipControlHandle);

RV_DECLARE_HANDLE(RvIppSipTerminalHandle);

/* RvMtfMsgProcessType
* ----------------------------------------------------------------------------
* This enumeration includes values for indicating MTF how to process
* incoming message: whether to ignore it or not.
*/
typedef enum 
{
    RV_MTF_IGNORE_BY_STACK,
        /* This value indicates both SIP stack and MTF to ignore the message */
    RV_MTF_IGNORE_BY_MTF,
        /* This value indicates MTF to ignore the message, but message will
            still be processed by SIP stack*/
    RV_MTF_DONT_IGNORE
        /* This value indicates both SIP stack and MTF to process the message */
} RvMtfMsgProcessType;

/*-----------------------------------------------------------------------
                    S T A C K       E X T E N S I O N S
 ------------------------------------------------------------------------*/

/******************************************************************************
*  RvIppSipStackConfigCB
*  ----------------------------
*  General :        Prototype for Callback to configure the SIP stack.
*                   When this function is called, pStackCfg is filled
*                   with default values, which the user can change.
*                   After this function returns, validation checking is
*                   done on the values, by both IPP TK and SIP stack.
*                   Invalid values will be changed to default values, and
*                   unsupported parameters will be ignored.
*
*  Return Value:    None.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          pStackCfg         pointer to stack configuration object.
*       .
*
*  Output          None.
*
******************************************************************************/
typedef void (*RvIppSipStackConfigCB)(IN RvSipStackCfg* pStackCfg);


/******************************************************************************
*  RvIppSipRegisterStackEventsCB
*  ----------------------------
*  General :       Prototype for registering callbacks to the SIP stack.
*                  This function enables the user to listen for stack
*                  events that IPP TK doesn't.
*
*  Return Value:   None.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          sipControl          pointer to SipControl structure
*                  stackHandle         pointer to sip stack handle.
*       .
*
*  Output          None.
*
******************************************************************************/
typedef void (*RvIppSipRegisterStackEventsCB)(IN RvIppSipControlHandle  sipMgrHndl);

/******************************************************************************
*  RvIppSipPreCallLegCreatedIncomingCB
*  ----------------------------
*  General :       Prototype for PreCallLegCreatedIncomingCB.
*                  This callback is called before IPP processes an incoming call.
*
*
*  Return Value:    rvFalse - if you don't want the IPP to perform its default processing.
*                   rvTrue  - continue as usual
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          sipMgrHndl         pointer to sipMgr handle.
*       .          hCallLeg           Call-Leg handle
*                  phAppCallLeg       Application call-leg handle
*                  userData           user application data.
*  Output          None.
*
******************************************************************************/
typedef RvBool (*RvIppSipPreCallLegCreatedIncomingCB)(
    IN  RvIppSipControlHandle   sipMgrHndl,
    IN  RvSipCallLegHandle      hCallLeg,
    OUT RvSipAppCallLegHandle*  phAppCallLeg,
    IN  void*                   userData);

/******************************************************************************
*  RvIppSipPostCallLegCreatedIncomingCB
*  ----------------------------
*  General :        Prototype for PreCallLegCreatedIncomingCB
*
*  Return Value:    None.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          sipMgrHndl         pointer to sipMgr handle.
*       .          hCallLeg           Call-Leg handle
*                  phAppCallLeg       Application call-leg handle
*                  userData           user application data.
*
*  Output          phAppCallLeg       Application call-leg handle
*
******************************************************************************/
typedef void (*RvIppSipPostCallLegCreatedIncomingCB)(
    IN		RvIppSipControlHandle    sipMgrHndl,
    IN		RvSipCallLegHandle       hCallLeg,
    INOUT	RvSipAppCallLegHandle*	 phAppCallLeg,
    IN		void*                    userData);

/******************************************************************************
*  RvIppSipPreCallLegCreatedOutgoingCB
*  ----------------------------
*  General :        Prototype for RvIppSipPreCallLegCreatedOutgoingCB.
*                   This callback is called before IPP TK build headers
*                   to call-leg object, and will use "to" and "from" values
*                   returned from user to build TO and FROM headers.
*
*  Return Value:    None.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          sipMgrHndl         pointer to sipMgr handle.
*       .          hCallLeg           Call-Leg handle
*                  to                 destination address
*                  from               source address
*                  userData           user application data.
*
*  Output          None.
*
******************************************************************************/
typedef void (*RvIppSipPreCallLegCreatedOutgoingCB)(
    IN RvIppSipControlHandle    sipMgrHndl,
    IN RvSipCallLegHandle       hCallLeg,
    IN RvChar*                  to,
    IN RvChar*                  from,
    IN void*                    userData);



/******************************************************************************
*  RvIppSipPostCallLegCreatedOutgoingCB
*  ----------------------------
*  General :        Prototype for SipPostCallLegCreatedOutgoingCB.
*                   This callback is called after IPP TK built headers
*                   in call-leg object, and before callMake() was called.
*                   User can change any of the headers in call-leg object.
*
*  Return Value:    None.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          sipMgrHndl         pointer to sipMgr handle.
*       .          hCallLeg           Call-Leg handle
*                  userData           user application data.
*
*  Output          None.
*
******************************************************************************/
typedef void (*RvIppSipPostCallLegCreatedOutgoingCB)(
    IN RvIppSipControlHandle    sipMgrHndl,
    IN RvSipCallLegHandle       hCallLeg,
    IN void*                    userData);

/******************************************************************************
*  RvIppSipPreStateChangedCB
*  ----------------------------
*  General :        Prototype for SipPreStateChangedCB.
*                   This callback is called when stack event RvSipCallLegStateChangedEv
*                   is invoked, before a IPP handles a SipCallLeg state machine change.
*
*
*  Return Value:    rvFalse - if you don't want the IPP to performs its default processing.
*                   rvTrue  - continue as usual
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          sipMgrHndl         pointer to sipMgr handle.
*       .          hCallLeg           Call-Leg handle
*                  phAppCallLeg       Application call-leg handle
*                  eState             CallLeg state
*                  eReason            state change reason
*                  userData           user application data.
*  Output          None.
*
******************************************************************************/
typedef RvBool (*RvIppSipPreStateChangedCB)(
    IN RvIppSipControlHandle            sipMgrHndl,
    IN RvSipCallLegHandle               hCallLeg,
    IN RvSipAppCallLegHandle            hAppCallLeg,
    IN RvSipCallLegState                eState,
    IN RvSipCallLegStateChangeReason    eReason,
    IN void*                            userData);


/******************************************************************************
*  RvIppSipPostStateChangedCB
*  ----------------------------
*  General :        Prototype for SipPostStateChangedCB.
*                   This callback is called after a SipCallLeg state machine change
*
*  Return Value:    None.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          sipMgrHndl         pointer to sipMgr handle.
*       .          hCallLeg           Call-Leg handle
*                  phAppCallLeg       Application call-leg handle
*                  eState             CallLeg state
*                  eReason            state change treason
*                  userData           user application data.

*  Output          None.
*
******************************************************************************/
typedef void (*RvIppSipPostStateChangedCB)(
    IN  RvIppSipControlHandle               sipMgrHndl,
    IN  RvSipCallLegHandle				    hCallLeg,
    IN  RvSipAppCallLegHandle               hAppCallLeg,
    IN  RvSipCallLegState			        eState,
    IN  RvSipCallLegStateChangeReason       eReason,
    IN  void*                               userData);

/******************************************************************************
*  RvIppSipPreMsgToSendCB
*  ----------------------------
*  General :       Invoked before the IPP modifies the message about to be sent. 
*                  Enables the application to change the outgoing SIP message.                 
*
*  Return Value:    rvFalse - if you don't want the IPP to performs its default processing.
*                   rvTrue  - continue as usual
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          sipMgrHndl         pointer to sipMgr handle.
*       .          hCallLeg           Call-Leg handle
*                  phAppCallLeg       Application call-leg handle
*                  hMsg               sip message handle
*                  userData           user application data.
*  Output          None.
*
******************************************************************************/
typedef RvBool (*RvIppSipPreMsgToSendCB)(
    IN RvIppSipControlHandle    sipMgrHndl,
    IN RvSipCallLegHandle       hCallLeg,
    IN RvSipAppCallLegHandle    hAppCallLeg,
    IN RvSipMsgHandle           hMsg,
    IN void*                    userData);

/******************************************************************************
*  RvIppSipPostMsgToSendCB
*  ----------------------------
*  General :        Invoked before the IPP modifies the message about to be sent.
*                   Enables the application to change the outgoing SIP message.                  
*
*  Return Value:    None.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          sipMgrHndl         pointer to sipMgr handle.
*       .          hCallLeg           Call-Leg handle
*                  phAppCallLeg       Application call-leg handle
*                  hMsg               sip message handle
*                  userData           user application data.
*  Output          None.
*
******************************************************************************/
typedef void (*RvIppSipPostMsgToSendCB)(
    IN RvIppSipControlHandle    sipMgrHndl,
    IN RvSipCallLegHandle       hCallLeg,
    IN RvSipAppCallLegHandle    hAppCallLeg,
    IN RvSipMsgHandle           hMsg,
    IN void*                    userData);

/******************************************************************************
 * RvIppSipPreMsgReceivedCB
 * ----------------------------------------------------------------------------
 * General:
 *  Prototype for SipPreMsgReceivedCB. Invoked before the IPP modifies the 
 *  message received. Enables the application to change the incoming SIP message.
 *
 * Arguments:
 *  Input:          sipMgrHndl         pointer to sipMgr handle.
 *                  hCallLeg           Call-Leg handle
 *                  phAppCallLeg       Application call-leg handle
 *                  hMsg               sip message handle
 *                  userData           user application data.
 * Output: None
 *
 * Return Value: RV_MTF_IGNORE_BY_STACK - indicates both SIP stack and MTF to ignore 
 *               the message.
 *               RV_MTF_IGNORE_BY_MTF - indicates MTF to ignore the message, but 
 *               message will still be processed by SIP stack.
 *               RV_MTF_DONT_IGNORE - indicates both SIP stack and MTF to process 
*                the message.
 *****************************************************************************/
typedef RvMtfMsgProcessType (*RvIppSipPreMsgReceivedCB)(
    IN RvIppSipControlHandle    sipMgrHndl,
    IN RvSipCallLegHandle       hCallLeg,
    IN RvSipAppCallLegHandle    hAppCallLeg,
    IN RvSipMsgHandle           hMsg,
    IN void*                    userData);

/******************************************************************************
*  RvIppSipPostMsgReceivedCB
*  ----------------------------
*  General :        Prototype for SipPostMsgReceivedCB. Invoked before the IPP
*                  modifies the message received. Enables the application
*                  to change the incoming SIP message
*
*  Return Value:    None.
*
*  ----------------------------------------------------------------------------
*  Input:          sipMgrHndl         pointer to sipMgr handle.
*       .          hCallLeg           Call-Leg handle
*                  phAppCallLeg       Application call-leg handle
*                  hMsg               sip message handle
*                  userData           user application data.
*  Output          None.
*
******************************************************************************/
typedef void (*RvIppSipPostMsgReceivedCB)(
    IN RvIppSipControlHandle    sipMgrHndl,
    IN RvSipCallLegHandle       hCallLeg,
    IN RvSipAppCallLegHandle    hAppCallLeg,
    IN RvSipMsgHandle           hMsg,
    IN void*                    userData);

/******************************************************************************
 * RvIppSipPreRegClientStateChangedCB
 * ----------------------------------------------------------------------------
 * General:
 *  This callback is invoked before the MTF handles registration state changes
 *  of the SIP User Agent.
 *
 * Arguments:
 * Input:  sipMgrHndl       - Pointer to sipMgr handle.
 *         hRegClient       - SIP registration client instance.
 *         mdmTerminalHndl  - The terminal handle used.
 *         eState           - SIP registration client state
 *         eReason          - Reason for the state changed that occurred.
 *         userData         - The userData provided by the application in its
 *                            RvIppSipExtClbks struct.
 * Output: None.
 *
 * Return Value: RV_FALSE if the application doesn't want the MTF to perform any
 *               default processing due to the state change.
 *               RV_TRUE to continue as usual.
 *****************************************************************************/
typedef RvBool (*RvIppSipPreRegClientStateChangedCB)(
    IN RvIppSipControlHandle            sipMgrHndl,
    IN RvSipRegClientHandle             hRegClient,
    IN RvIppTerminalHandle              mdmTerminalHndl,
    IN RvSipRegClientState              eState,
    IN RvSipRegClientStateChangeReason  eReason,
    IN void*                            userData);


/******************************************************************************
*  struct RvIppSipExtClbks
*  ----------------------------
*
*  General :  Structure containing SIP extension API which User application can
*             implement.
*
*******************************************************************************/

typedef struct
{
    RvIppSipStackConfigCB                   stackConfigF;
    RvIppSipRegisterStackEventsCB           registerStackEventsF;
    RvIppSipPreCallLegCreatedIncomingCB     preCallLegCreatedIncomingF;
    RvIppSipPostCallLegCreatedIncomingCB    postCallLegCreatedIncomingF;
    RvIppSipPreCallLegCreatedOutgoingCB     preCallLegCreatedOutgoingF;
    RvIppSipPostCallLegCreatedOutgoingCB    postCallLegCreatedOutgoingF;
    RvIppSipPreStateChangedCB               preStateChangedF;
    RvIppSipPostStateChangedCB              postStateChangedF;
    RvIppSipPreMsgToSendCB                  preMsgToSendF;
    RvIppSipPostMsgToSendCB                 postMsgToSendF;
    RvIppSipPreMsgReceivedCB                preMsgReceivedF;
    RvIppSipPostMsgReceivedCB               postMsgReceivedF;
    RvIppSipPreRegClientStateChangedCB      PreRegClientStateChangedF;
    void*                                   userData;

} RvIppSipExtClbks;

/*-----------------------------------------------------------------------
                    S I P   A D A P T O R   A P I
 ------------------------------------------------------------------------*/
#define SIP_NUM_OF_MODULES 39 /* Should be same as size of RvSipStackModule, or STACK_NUM_OF_LOG_SOURCE_MODULES+2 (StackInternal.h)*/

typedef struct {
    RvSipStackModule    moduleId;
    RvUint8             filter;
} RvIppSipModuleFilter;

typedef struct {
    int                     num;
    RvIppSipModuleFilter    filters[SIP_NUM_OF_MODULES];
} RvIppSipLogOptions;


typedef enum{
    RV_REPLY_RINGING = 180,
    RV_REPLY_QUEUED  = 182
} RvCallWaitingReply;


/* RvIppSipPhoneCfg
 * ------------------------------------------------------------------------
 * This structure contains the parameters for the configuration of the
 * SIP Stack before calling rvIppSipStackInitialize().
 */
typedef struct
{
    RvUint16            stackTcpPort;
        /* The TCP port on which the Stack listens.
           Default: 5060. */

    RvUint16            stackUdpPort;
        /* The UDP port on which the Stack listens.
           Default: 5060. */

    RvChar*             userDomain;
        /* This domain name will be sent in the From header of outgoing INVITE messages
           in either of the following cases: If the Registrar address parameter is configured
           as an IP address, or if the Registrar address is not configured at all. */

    RvChar*             localAddress;
        /* Local IP address. This parameter is mandatory and may not be left empty. */

    RvChar*             registrarAddress;
        /* The Registrar IP address or domain name. If this parameter is not set,
           Registration messages will not be sent.
           Default: NULL. */

    RvUint16            registrarPort;
        /* The number of the Port on which the Registrar listens.
           Default: 5060. */

    RvChar*             outboundProxyAddress;
        /* The IP address of the outbound Proxy. If this parameter is set, all outgoing
           messages (including Registration messages) will be sent to this Proxy according
           to the Stack behavior.
           Default: NULL.
           Note: To configure a host name instead of an IP address, set this parameter to
           NULL and configure the outboundProxyHostName parameter in the SIP Stack. */

    RvUint16            outboundProxyPort;
        /* The number of the Port on which the outbound Proxy listens.
           Default: 5060. */

    RvSipTransport      transportType;
        /* The Transport type of the outgoing messages. Valid values are:
           - RVSIP_TRANSPORT_UDP
           - RVSIP_TRANSPORT_TCP
		   - RVSIP_TRANSPORT_UNDEFINED
           Default: RVSIP_TRANSPORT_UDP. */

    int                 maxCallLegs;
        /* The maximum number of call-legs the Stack can handle simultaneously.
           Default: 10. */

    int                 maxRegClients;
        /* The maximum number of RegClient objects the Stack can handle
           simultaneously.
           Default: 2. */

    RvBool              tcpEnabled;
        /* This parameter indicates support for TCP. When set to RV_TRUE, the Terminal
           Framework will support a TCP connection. When set to RV_FALSE, the Terminal
           Framework will not support a TCP connection (incoming TCP messages will be
           ignored).
           Default: RV_TRUE. */

    int                 priority;
        /* The priority of the Terminal Framework task. It is recommended to use one of
           the following values, which define the appropriate value according to the
           operating system:
           - RV_THREAD_PRIORITY_MAX
           - RV_THREAD_PRIORITY_DEFAULT
           - RV_THREAD_PRIORITY_MIN
           These values are defined in the file rvthread.h.
           Default: RV_THREAD_PRIORITY_DEFAULT. */

    RvChar*             username;
        /* Used for Authentication. This parameter can also be configured for each
           termination separately. If this parameter is configured, it will be used
           for all terminations whose username has not been configured. If this
           parameter is left empty and is not configured in a termination, the
           Authentication header will not be sent with the Registration request.
           Default: NULL. */

    RvChar*             password;
        /* Used for Authentication. This parameter can also be configured for each
           termination separately. If this parameter is configured, it will be used
           for all terminations whose username has not been configured. If this
           parameter is left empty and is not configured in a termination, the
           Authentication header will not be sent with the Registration request.
           Default: NULL. */

    RvBool              autoRegister;
        /* If set to RV_TRUE, the Terminal Framework will send the initial registration request
           to the Registrar for every termination that registers to the Multimedia Terminal
           Framework. If set to RV_FALSE, the initial registration request will not be sent. It can
           be sent manually at any time by calling rvMdmTermMgrRegisterAllTermsToNetwork_() or
           rvMdmTermMgrRegisterTermToNetwork_(). Regardless of the value of this parameter, the
           Terminal Framework will send re-Registration requests.
           Default: RV_FALSE. */

    RvInt32             registrationExpire;
        /* The timeout (in seconds) for sending Re-registration requests to the Registrar.
           Default: 60,000 seconds. */

    RvInt32             unregistrationExpire;
        /* This parameter defines the timeout (in seconds) to wait for a reply from the
           Registrar, after an Unregistration request was sent. The Unregistration request is
           sent when the user application unregisters a termination from the Multimedia
           Terminal Framework by calling rvMdmTermMgrUnregisterTermination(). Once
           this function is called, an un-registration process begins. This process is
           asynchronous.
           If the termination is registered with a Registrar, the Multimedia Terminal
           Framework will send an Unregistration request to the Registrar and wait for a
           reply. The UnregistrationExpire parameter defines the timeout to wait for a
           reply. The process will continue only when the timeout expires or when a reply
           is received from the Registrar.
           When either of these scenarios occurs, the Multimedia Terminal Framework will
           call the user callback RvMdmTermUnregisterTermCompletedCB() to notify the
           user application that the unregistration process has ended.
           Default: 20 seconds. */

    RvInt32             referTimeout;
        /* The timeout (in milliseconds) for waiting for NOTIFY after sending REFER, before
           disconnecting the call-leg.
           Default: 2000 milliseconds. */

    int                 debugLevel;
        /* This parameter is not used. */

    RvIppSipLogOptions* logOptions;
        /* This parameter defines log options for the Sip Stack. See the Logging chapter
           in the Programmer's Guide.
           Default: NULL. */

    unsigned int        dialToneDuration;
        /* Duration of Dial Tone signal (in milliseconds) when going off-hook. When the
           user goes off-hook and timeout expires, Dial Tone will be stopped and the
           connection will disconnect. A value of 0 indicates an infinite Dial Tone.
           Default: 30,000 milliseconds. */

    RvUint32            watchdogTimeout;
        /* This parameter indicates the time interval (in seconds) used for periodically
           printing the resources of the SIP Stack. If set to zero, the timer is disabled.
           Watchdog printouts will be added to the log only if the module IPP_SIPCTRL
           was added to the log configuration. See the Logging chapter in the Programmer's
           Guide.
           Default: 0. */

    RvCallWaitingReply  callWaitingReply;
        /* When the incoming call is a Call Waiting call, this parameter indicates which
           SIP message will be sent as a reply to the Invite. Possible values are:
           - RV_REPLY_RINGING=180
           - RV_REPLY_QUEUED=182
           Default: RV_REPLY_QUEUED */

    RvBool              outOfBandDtmf;
        /* When this parameter is set to RV_TRUE, out-of-band DTMF is enabled. */

	RvIppCfwCfg         cfwCallCfg;
	/* Call Forwarding related configuration. */	

    RvSdpStackCfg       sdpStackCfg;
        /* This parameter defines SDP stack configuration. See SDP documentation for more details.*/

    RvBool              connectMediaOn180;   
        /* When True, media will be connected both in 180, 183 and 200, when False, media will be connect in 183 
           and 200 only*/

#ifdef RV_CFLAG_TLS
    RvIppTransportTlsCfg transportTlsCfg;
        /* TLS Transport related configuration. */
#endif
	/*ACL JL 59875 limits number of registrations to proxy to  a configurable number RV patch*/
	RvInt32             maxAuthenticateRetries;/* how many times we should respond to consequent 401/407 msgs */

} RvIppSipPhoneCfg;


/******************************************************************************
*  rvIppSipSystemInit
*  ------------------
*  General :        Initializes IPP elements which needs to be ready before
*                   constructing IPP TK -
*                   1. Initialize core
*                   2. Reset extension APIs structure.
*                   This function will be used to initialize IPP TK, and should
*                   be the first function to be called.
*
*  Return Value:   None
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          None
*
*  Output:         None
******************************************************************************/
RVAPI void RVCALLCONV rvIppSipSystemInit(void);

/******************************************************************************
*  rvIppSipSystemEnd
*  ------------------
*  General :        Shutdown IPP System - end core.
*                   This function will be used to shutdown IPP TK, and should be
*                   the last function to be called.
*
*  Return Value:   None
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          None
*
*  Output:         None
******************************************************************************/
RVAPI void RVCALLCONV rvIppSipSystemEnd(void);

/******************************************************************************
*  rvIppSipInitConfig
*  ------------------
*  General :        Initializes configuration structure.
*                   Currently it only fills the structure with zeros, and later
*                   it will be filled with default values by IPP and by the stack.
*                   Should be called before rvIppSipStackInitialize().
*
*  Return Value:   True if successful, False if failed
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          RvIppSipPhoneCfg     Configuration structure
*
*  Output:         none.
******************************************************************************/
RVAPI RvBool RVCALLCONV rvIppSipInitConfig(
    OUT RvIppSipPhoneCfg*    cfg);

RVAPI RvBool RVCALLCONV rvIppSipStackInitialize(
    OUT RvSipStackHandle* stackHandle,
    IN  RvIppSipPhoneCfg* cfg);

/******************************************************************************
*  rvIppSipPhoneConstruct
*  ----------------------------

*  General :
*
*  Return Value:    None.
*
*  ----------------------------------------------------------------------------
*  Input:          mgr                pointer to MdmTermination Manager
*       .          cfg                pointer to configuration data
*                  sipStack           sip stack handle
*
*  Output          None.
*
******************************************************************************/
RVAPI void RVCALLCONV rvIppSipPhoneConstruct(
    OUT RvMdmTermMgr*       mgr,
    IN  RvIppSipPhoneCfg*   cfg,
    IN  RvSipStackHandle    sipStack);

/******************************************************************************
*  rvIppSipRegisterExtClbks
*  --------------------------------
*  General :       Registers SIP user callbacks
*                   Should be called before rvIppSipStackInitialize()
*
*  Return Value:   None
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          clbks              Structure includes pointers to user implementations
*
*  Output:         none.
******************************************************************************/
RVAPI void RVCALLCONV rvIppSipRegisterExtClbks(IN RvIppSipExtClbks* clbks);

/******************************************************************************
*  rvIppSipControlGetRegistrarAddress
*  --------------------------------
*  General :       Returns ip address of Registrar
*
*  Return Value:   None
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:                sipMgrHndl           Handle to SipControl object
*
*  Output:             registrarAddress       address of Registrar
*                            registrarAddressLen    length of registrarAddress string allocated
*                                       by application
******************************************************************************/
RVAPI void RVCALLCONV rvIppSipControlGetRegistrarAddress(
    IN  RvIppSipControlHandle   sipMgrHndl,
    OUT RvChar*                 registrarAddress,
    IN  RvSize_t                registrarAddressLen);

/******************************************************************************
*  rvIppSipControlGetUserDomain
*  ----------------------------
*  General :        Returns domain of Registrar
*
*  Return Value:    None
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:               sipMgrHndl          Handle to SipControl object
*
*  Output:              userDomain          domain of the User
*                       userDomainLen       length of userDomain string
******************************************************************************/
RVAPI void RVCALLCONV rvIppSipControlGetUserDomain(
    IN  RvIppSipControlHandle   sipMgrHndl,
    OUT RvChar*                 userDomain,
    IN  RvSize_t                userDomainLen);

/******************************************************************************
*  rvIppSipControlGetOutboundProxyAddress
*  --------------------------------
*  General :        Returns ip address of Outbound Proxy
*
*  Return Value:    None
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           sipMgrHndl              Handle to SipControl object
*
*  Output:          outboundProxyAddress    address of Outbound Proxy
*                   outboundProxyAddressLen length of outboundProxyAddress string allocated
*                                            by application
******************************************************************************/
RVAPI void RVCALLCONV rvIppSipControlGetOutboundProxyAddress(
    IN  RvIppSipControlHandle   sipMgrHndl,
    OUT RvChar*                 outboundProxyAddress,
    IN  RvSize_t                outboundProxyAddressLen);

/******************************************************************************
*  rvIppSipControlGetExtUserData
*  --------------------------------
*  General :             This function returns user data that was set by user when extension
*                              callbacks were registered.
*
*  Return Value:     User data.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:                  sipMgrHndl                   Handle to SipControl object
*
*  Output:             None.
******************************************************************************/
RVAPI void* RVCALLCONV rvIppSipControlGetExtUserData(IN RvIppSipControlHandle sipMgrHndl);


/******************************************************************************
*  rvIppSipControlGetTransportType
*  --------------------------------
*  General :          This function checks transport type.
*
*  Return Value:  transport type.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:               sipMgrHndl          Handle to SipControl object
*
*  Output:          None.
******************************************************************************/
RVAPI RvSipTransport RVCALLCONV rvIppSipControlGetTransportType(
    IN RvIppSipControlHandle    sipMgrHndl);

typedef struct
{
    RvSipSubsEvHandlers             sipSubsEvHandlers;  /*call leg callbacks */
    RvSipCallLegEvHandlers          sipCallLegEvHandlers;   /*call leg callbacks */
    RvSipTransactionEvHandlers      sipTransEvHandlers;  /*Transaction callbacks*/
    RvSipRegClientEvHandlers        sipRegClientEvHandlers; /*Client Registration callbacks*/
    RvSipAuthenticatorEvHandlers    sipAuthEvHandlers;  /*Authentication callbacks*/
    RvSipTransportMgrEvHandlers     sipTransportEvHandlers;  /*Transport callbacks*/
} RvIppSipStackCallbacks;

/******************************************************************************
*  rvIppSipControlGetStackHandle
*  --------------------------------
*  General :          return stack handle stored inside SipControl object.
*
*  Return Value:  transport type.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:               sipMgrHndl          Handle to SipControl object
*
*  Output:          stack handle
******************************************************************************/
RVAPI RvSipStackHandle RVCALLCONV rvIppSipControlGetStackHandle(
    IN RvIppSipControlHandle sipMgrHndl);

/******************************************************************************
*  rvIppSipControlGetStackCallbacks
*  --------------------------------
*  General :          get list of stack callbacks used by toolkit.
*
*  Return Value:  transport type.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:               sipMgrHndl          Handle to SipControl object
*
*  Output:          RvIppSipStackCallbacks*.
******************************************************************************/
RVAPI RvIppSipStackCallbacks* RVCALLCONV rvIppSipControlGetStackCallbacks(
    IN RvIppSipControlHandle sipMgrHndl);

/******************************************************************************
*  rvIppSipControlMsgSetSdpBody
*  --------------------------------
*  General :         This function adds SDP body to a SIP message.
*
*  Return Value:    True, if successful.
*                           False if failed.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:               hMsg            Handle to SIP message
*                           sdpMsg      pointer to SDP structure
*
*  Output:          None.
******************************************************************************/
RVAPI RvBool RVCALLCONV rvIppSipControlMsgSetSdpBody(
    IN RvSipMsgHandle   hMsg,
    IN const RvSdpMsg*  sdpMsg);

/******************************************************************************
*  rvIppSipControlGetTerminal
*  --------------------------------
*  General :          Returns a handle to Mdm terminal
*
*  Return Value:  a handle to Mdm terminal
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:               hAppCallLeg         Handle to application call-leg handle
*
*  Output:          None
******************************************************************************/
RVAPI RvIppTerminalHandle RVCALLCONV rvIppSipControlGetMdmTerminal(
    IN RvSipAppCallLegHandle   hAppCallLeg);

/******************************************************************************
*  rvIppSipControlGetCallLeg
*  --------------------------------
*  General :          Returns a handle to SIP stack Call leg associated with 
*                     the given connection.
*  Return Value:      a handle to SIP stack Call leg
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:               connHndl         Handle to SIP connection
*
*  Output:          None
******************************************************************************/
RVAPI RvSipCallLegHandle RVCALLCONV rvIppSipControlGetCallLeg(
    IN RvIppConnectionHandle   connHndl);


#ifdef __cplusplus
}
#endif


#endif /*SIPCONTROL_API_H*/
