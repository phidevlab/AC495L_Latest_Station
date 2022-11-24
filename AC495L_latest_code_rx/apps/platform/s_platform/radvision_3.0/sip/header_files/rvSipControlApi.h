/******************************************************************************
Filename:    rvSipControlApi.h
Description: This file includes SIP extension APIs and type definitions.
Note: In version 3.0 and higher, some of the APIs and type definitions in
			 this file are deprecated and located in file rvMtfInternals.h.
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
#include "rvsdp.h"

#ifdef RV_CFLAG_TLS
#include "sipTls.h"
#endif

#define SIP_NUM_OF_MODULES 41 /* Indicates the number of SIP Stack log modules.
								 The value should be same as the size of RvSipStackModule,
								 or STACK_NUM_OF_LOG_SOURCE_MODULES+2 (StackInternal.h). */

#ifdef RV_SIP_IMS_ON
#define AKA_OP_ARRAY_SIZE 16
#endif

/*@*****************************************************************************
 * Type: RvIppSipModuleFilter (RvMtfBasePkg)
 * -----------------------------------------------------------------------------
 * Description: This structure is used for setting the SIP Stack's logging
 * options. It contains a SIP module and a log filter mask.
 ****************************************************************************@*/
typedef struct {
    RvSipStackModule    moduleId;
		/* SIP Stack module */
    RvUint8             filter;
		/* Mask for the module's log filter */
} RvIppSipModuleFilter;

/*@*****************************************************************************
 * Type: RvIppSipLogOptions (RvMtfBasePkg)
 * -----------------------------------------------------------------------------
 * Description: This structure is used for setting the SIP Stack's logging
 * options. It contains a list of SIP Stack modules and their log filters.
 ****************************************************************************@*/
typedef struct {
    int                     num;
		/* Number of modules. */
    RvIppSipModuleFilter    filters[SIP_NUM_OF_MODULES];
		/* List of SIP Stack modules and their log filters. */
} RvIppSipLogOptions;

/*@*****************************************************************************
 * Enum: RvCallWaitingReply (RvMtfBasePkg)
 * -----------------------------------------------------------------------------
 * Description: The value of this enumerator indicates which SIP reply will be
 * sent for an incoming call in case of Call Waiting.
 ****************************************************************************@*/
typedef enum{
    RV_REPLY_RINGING = 180,
		/* Indicates that "180 Ringing" will be sent for incoming Call
	Waiting. */
    RV_REPLY_QUEUED  = 182
		/* Indicates that "182 Queued" will be sent for incoming Call
	Waiting. */
} RvCallWaitingReply;


/*@*****************************************************************************
 * Type: RvIppSipPhoneCfg (RvMtfBasePkg)
 * -----------------------------------------------------------------------------
 * Description: This structure contains the MTF configuration parameters.
 ****************************************************************************@*/
typedef struct
{
    RvUint16            stackTcpPort;
        /* The TCP port on which the Stack listens.
           Default: 5060. */

    RvUint16            stackUdpPort;
        /* The UDP port on which the Stack listens.
           Default: 5060. */

    RvChar*             userDomain;
        /* This domain name will be sent in the From header of outgoing INVITE
	       messages if the Registrar address parameter is configured as an IP
		   address, or if the Registrar address is not configured. */

    RvChar*             localAddress;
        /* Local IP address. This parameter is mandatory. */

    RvChar*             registrarAddress;
        /* The Registrar IP address or domain name. If this parameter is not set,
           Registration messages will not be sent.
           Default: NULL. */

    RvUint16            registrarPort;
        /* The number of the Port on which the Registrar listens.
           Default: 5060. */

    RvChar*             outboundProxyAddress;
        /* The outbound Proxy IP address. If this parameter is set, all
	       outgoing messages (including Registration messages) will be sent to
	       this proxy according to the Stack behavior.
           Default: NULL.
           Note: To configure a host name instead of an IP address, set this
	       parameter to NULL and configure the outboundProxyHostName parameter in
	       the SIP Stack. */

    RvUint16            outboundProxyPort;
        /* The number of the Port on which the outbound Proxy listens.
           Default: 5060. */

    RvSipTransport      transportType;
        /* The Transport type of the outgoing messages. Valid values are:
           RVSIP_TRANSPORT_UDP, RVSIP_TRANSPORT_TCP, and RVSIP_TRANSPORT_UNDEFINED.
 	       This parameter determines the transport type for all terminations,
	       unless it is configured in the termination configuration (transportType
	   parameter in RvMtfTerminationConfig).
           Default: RVSIP_TRANSPORT_UDP. */

    int                 maxCallLegs;
        /* The maximum number of call-legs the Stack can handle simultaneously.
           Default: 10. */

    int                 maxRegClients;
        /* The maximum number of RegClient objects the Stack can handle
           simultaneously.
           Default: 2. */

    RvBool              tcpEnabled;
        /* Indicates support for TCP. When set to RV_TRUE, the MTF
	   supports a TCP connection. When set to RV_FALSE, the MTF
	   does not support a TCP connection (incoming TCP messages are ignored).
           Default: RV_TRUE. */

    int                 priority;
        /* The priority of the MTF task. It is recommended to use
	       one of the following values, which define the appropriate value
	       according to the operating system: RV_THREAD_PRIORITY_MAX,
           RV_THREAD_PRIORITY_DEFAULT, or RV_THREAD_PRIORITY_MIN.
           These values are defined in the file rvthread.h.
           Default: RV_THREAD_PRIORITY_DEFAULT. */

    RvChar*             username;
        /* Used for Authentication. This parameter can also be configured for each
           termination separately. When configured, it is used for all terminations
	       whose usernames have not been configured. When left blank and not
	       configured in a termination, the Authentication header will not be sent
	       with the Registration request.
           Default: NULL. */

    RvChar*             password;
        /* Used for Authentication. This parameter can also be configured for each
           termination separately. When configured, it is used for all terminations
	   whose usernames have not been configured. When left blank and not
	   configured in a termination, the Authentication header will not be sent with
	   the Registration request.
           Default: NULL. */

    RvBool              autoRegister;
        /* If set to RV_TRUE, the MTF sends the initial registration
	   request to the Registrar for every termination that registers to the
	   MTF. If set to RV_FALSE, the initial registration request is not sent.
	   It can be sent manually at any time by calling
	   rvMdmTermMgrRegisterAllTermsToNetwork_() or
	   rvMdmTermMgrRegisterTermToNetwork_(). The MTF will send re-Registration
	   requests regardless of the value of this parameter.
           Default: RV_FALSE. */

    RvInt32             registrationExpire;
        /* The timeout (in seconds) for sending Re-registration requests to the
	       Registrar.
           Default: 60,000 seconds. */

    RvInt32             unregistrationExpire;
        /* Defines the timeout (in seconds) to wait for a reply from the Registrar
	   after an Unregistration request is sent. This occurs when the user
	   application unregisters a termination from the MTF by calling
	   rvMdmTermMgrUnregisterTermination(), which starts
	   the un-registration process. This process is asynchronous.
       If the termination is registered with a Registrar, the MTF sends an
	   Unregistration request to the Registrar and waits for a reply.
	   If the timeout period passes, the process will continue only if the timeout
	   expires or a reply is received from the Registrar.
       When either of these scenarios occurs, the MTF calls the user callback
       RvMdmTermUnregisterTermCompletedCB() to notify the user application that
	   the unregistration process has ended.
           Default: 20 seconds. */

    RvInt32            maxAuthenticateRetries;
	/* Indicates how many times to send an authenticated register message per
	   registration cycle. A registration cycle starts when a register message
	   is sent and ends in one of the following ways:
		1. A 200 OK reply is received, or
		2. The number of 401\407 reject replies exceeds this parameter
		   value.
	   The minimum value of maxAuthenticateRetries is 4, even if the user has
	   set it to a lower value or not at all. If the value is lower than 4,
	   authentication has no chance to succeed.
	   If this parameter value is very large, transaction resources may be
	   exhausted before the limit is reached.
	   Default: 4.*/
	RvBool            removeOldAuthHeaders;
	/*  Multiple requests may be sent in the course of the authorization process. Some requests 
	    may be rejected with a 401/407 reply. Each request adds an authorization header
		to a callLeg/regClient object. The authorization headers may relate to different
		proxies along the path to the final destination. Usually, proxies will examine all 
		authorization headers that comply with a specific realm until a header with the correct
		credentials is found. If no such header is found, a 401/407 is sent to the MTF,
		which will add a new authorization header with new credentials.
		This process continues until all proxies approve the requests.
		Some proxies are not capable of processing multiple authorization headers. These
		proxies process only one header and reject the request if this header does not include
		the correct credentials.
		This configuration parameter causes all authorization headers to be deleted, except 
		for the last one.
		Values: 0: Old headers are not removed. 1: Old headers are removed.
		Default: 0. */


#ifdef RV_SIP_IMS_ON
	RvBool              disableAkaAuthentication;
	/* This parameter disables AKA authentication even if the user application is compiled with IMS support.
	   When this parameter is set to False, both AKA and MD5 are supported. This means that if a request for authentication
	   is received, the reply will be sent either with AKA or with MD5, depending on the request. In addition, an empty authorization
	   header is added to every outgoing requests to enabling AKA authentication. When set to True,
	   only MD5 is supported. In this case, when a request for AKA authentication is received, the reply
	   will be sent with MD5. */

	RvUint8*            akaAuth_op;
	/* Pointer to a set of 16 values that is used for AKA RES calculation.
	   Different IMS servers may use different sets of values. If a mismatch
	   occurs between the server and client values, the authentication fails. */
#endif

    RvInt32             referTimeout;
        /* The interval (in milliseconds) required to wait for a NOTIFY message
	       after sending a REFER command, before disconnecting the call-leg.
           Default: 2000 milliseconds. */

    int                 debugLevel;
        /* This parameter is not used. */

    RvIppSipLogOptions* logOptions;
        /* Defines log options for the SIP Stack. See the Logging chapter in the
	       Programmer Guide.
           Default: NULL. */

    unsigned int        dialToneDuration;
        /* Duration of Dial Tone signal (in milliseconds) when the user is off-hook.
	       When this interval expires, the Dial Tone stops and the connection
	       disconnects. A value of 0 indicates an infinite Dial Tone.
           Default: 30,000 milliseconds. */

    RvUint32            watchdogTimeout;
        /* The time interval (in seconds) used for periodically printing the SIP
	       Stack resources. If set to zero, the timer is disabled.
           Watchdog printouts are added to the log only if the IPP_SIPCTRL module
	       was added to the log configuration. See the Logging chapter in the
           Programmer Guide.
           Default: 0. */

    RvCallWaitingReply  callWaitingReply;
        /* The SIP message to be sent as a reply to an invite during an incoming
	       Call Waiting call. Possible values are: RV_REPLY_RINGING=180, or
           RV_REPLY_QUEUED=182.
           Default: RV_REPLY_QUEUED */

    RvBool              outOfBandDtmf;
        /* When this parameter is set to RV_TRUE, out-of-band DTMF is enabled.
		   Default: False. */

    RvIppCfwCfg         cfwCallCfg;
	/* Displays the Call Forwarding-related configuration. */

    RvSdpStackCfg       sdpStackCfg;
        /* Defines the SDP Stack configuration. See SDP documentation for
	       more information. */

    RvBool              connectMediaOn180;
        /* When True, media connects when both "180 Ringing" and "183 In
	       Progress" messages are received. When False, media connects when a
	       "183 In Progress" message is received. Regardless of this setting,
	       media will connect when a "200 OK" message is received.
	       Default: False.*/
	RvBool              addUpdateSupport;
	    /* When True, UPDATE requests will be supported, i.e., the terminal
	       will be able to send and receive Update messages. Default: False.  */

	RvUint8             updateRetryAfterTimeout;
	    /* The MTF uses this value when it rejects an UPDATE message because
		   a previous offer-answer process has not yet been completed.
		   In such cases the MTF will send a 500 (Server Internal Error) response
		   with a Retry-After header with this value.
		   Value range: 0 .. 10 seconds. Default: 10 seconds. */

	RvUint16            callerUpdateResendTimeout;
		/*  When the MTF is the initiator of the call and an UPDATE the MTF sent
	        was rejected with 491 (Request Pending), the MTF should retry the
	        UPDATE after the time set in this parameter.
	        Value range: 2100 - 4000 milliseconds in 10ms units. Default: 4000. */

	RvUint16            calleeUpdateResendTimeout;
	    /*  When the MTF is not the initiator of the call and an UPDATE the
		    MTF sent was rejected with 491 (Request Pending), the MTF should
	        retry the UPDATE after the time set in this parameter.
	        Value range: 0 - 2000 seconds in 10ms units. Default:2000. */

    RvBool		autoAnswer;
	/* When True, incoming calls are automatically answered. When False, a
	   local user should answer incoming calls manually.
	   Default: False. */

    RvBool 		autoDisconnect;
	/* When True, the line is automatically disconnected when the remote
	   party disconnects, thereby freeing the line for another call.
	   When False, the local user should disconnect the line manually when
	   the remote party disconnects. Following call disconnection or
	   cancellation by the remote party, the local user hears a warning tone
	   until he disconnects also. Until the call is disconnected, the local
	   user cannot use the line for another call.
	   It is recommended  to set this parameter to True for softphone
	   applications and IP Phone applications.
	   Default: False.*/
//ACL NB 100429 VI83018 UnattendedTransfer issue- Received from RV at Apr 29th 2010
	RvBool 		semiTransferWithNoCancel;
		/* When set to true, Semi-Transfer will be performed without sending Cancel from A to C, similar to 
		   Attended-Transfer (figure 11 in Transfer draft-ietf-sipping-cc-transfer , section 6.6 ). 
		   In Semi-Transfer scenario: A and B are talking, A presses Transfer and calling C, before C 
		   answers A presses Transfer second time. When this flag is set to False, A will send Cancel
		   to C before sending REFER to B. When this flag is set to True, A will not cancel the call 
		   with C. The flow will be similar to Attended-Transfer, only that user A is no longer
		   part of the call phone-wise, but is part of the call signaling-wise.
		   Default: False */

	RvBool		blindTransferByeAfterRefer;
		/* When set to true, in blind transfer BYE will be sent from A to B right after B acks a REFER message
		   with 202 Accepted. This behavior is defined in rfc 5589 section 6 - Basic Transfer:
		   "If the Transferor's agent does not wish to participate in the remainder of the REFER
		   process and has no intention of assisting with recovery from transfer failure, it
		   could emit a BYE to the Transferee as soon as the REFER transaction completes. This flow is
		   sometimes known as "unattended transfer" or "blind transfer"."
		   When set to false A will send BYE only after NOTIFY request from B is received. 
		   Default: False */

#ifdef RV_CFLAG_TLS
    RvIppTransportTlsCfg transportTlsCfg;
        /* TLS Transport-related configuration. */
#endif
} RvIppSipPhoneCfg;

/*@*****************************************************************************
 * Enum: RvMtfMsgProcessType (RvMtfSipExtPkg)
 * -----------------------------------------------------------------------------
 * Description: This enumerator includes values that indicate how the MTF
 * should process an incoming message: ignore it or not.
 ****************************************************************************@*/
typedef enum
{
    RV_MTF_IGNORE_BY_STACK,
        /* Commands both the SIP Stack and the MTF to ignore the message. */
    RV_MTF_IGNORE_BY_MTF,
        /* Commands the MTF to ignore the message, while the SIP Stack should
           process the message. */
    RV_MTF_DONT_IGNORE
        /* Commands both the SIP Stack and the MTF to process the message. */
} RvMtfMsgProcessType;


/*@*****************************************************************************
 * Type: RvIppSipStackCallbacks (RvMtfSipExtPkg)
 * -----------------------------------------------------------------------------
 * Description: This structure contains pointers to SIP extension callbacks.
 * The user application can use these callbacks to control SIP Stack behavior.
 ****************************************************************************@*/
typedef struct
{
    RvSipSubsEvHandlers             sipSubsEvHandlers;
	/* SIP structure containing MTF implementations of Subscription
	   callbacks. */

    RvSipCallLegEvHandlers          sipCallLegEvHandlers;
	/* SIP structure containing MTF implementations of CallLeg callbacks. */

    RvSipTransactionEvHandlers      sipTransEvHandlers;
	/* SIP structure containing MTF implementations of Transaction callbacks. */

    RvSipRegClientEvHandlers        sipRegClientEvHandlers;
	/* SIP structure containing MTF implementations of RegClient callbacks. */

    RvSipAuthenticatorEvHandlers    sipAuthEvHandlers;
	/* SIP structure containing MTF implementations of Authentication
	   callbacks. */

    RvSipTransportMgrEvHandlers     sipTransportEvHandlers;
	/* SIP structure containing MTF implementations of Transport callbacks. */

    RvSipTransmitterMgrEvHandlers   sipTransmitterEvHandlers;
	/* SIP structure containing MTF implementations of Transmitter callbacks. */

} RvIppSipStackCallbacks;

/* Done for backward compatibility. */
#include "rvMtfInternals.h"

#ifdef __cplusplus
}
#endif


#endif /*SIPCONTROL_API_H*/
