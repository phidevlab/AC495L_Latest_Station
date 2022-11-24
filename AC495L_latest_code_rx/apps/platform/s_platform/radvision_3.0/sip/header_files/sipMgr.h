#ifndef SIPCONTROL_H
#define SIPCONTROL_H

#define RV_SIPCTRL_ADDRESS_SIZE			64

#include "inc.h"
#include "rvthread.h"
#include "rvtimer.h"
#include "rvvector.h"
#include "rvsdpenc.h"
#include "sipRegClient.h"
#include "sipAuthClient.h"
#include "rvsemaphore.h"
#include "rvSipControlApi.h"
#if (RV_OS_TYPE == RV_OS_TYPE_VXWORKS)
#include <vxWorks.h>
#include <taskLib.h>
#include <time.h>
#endif

#ifdef RV_SIP_IMS_ON
#include "AKA_Auc.h"
#endif


/* RESPONSE STATUS CODES */
/*=======================*/
#define RV_SIPCTRL_STATUS_TRYING		100	/*An action was taken, but the user has not been located yet*/
#define RV_SIPCTRL_STATUS_RINGING		180	/*User was located, the system is trying to alert him*/
#define RV_SIPCTRL_STATUS_FORWARDED		181 /*Call is being forward to a different destination/s*/
#define RV_SIPCTRL_STATUS_QUEUED		182 /*Called party is temporarily unavailable */
#define RV_SIPCTRL_STATUS_SESSIONPROGRESS	183 /*convey information about the progress of the call that is not otherwise classified */
#define RV_SIPCTRL_STATUS_OK			200 /*Request succeeded */
#define RV_SIPCTRL_STATUS_MULTICHOISES	300 /*The address resolves to several different locations*/
#define RV_SIPCTRL_STATUS_MOVEDPERM		301 /*The user is no longer available in this address */
#define RV_SIPCTRL_STATUS_MOVEDTEMP		302 /*The user is temporarily available in a new address*/
#define RV_SIPCTRL_STATUS_USEPROXY		303 /*The request must be sent via proxy*/
#define RV_SIPCTRL_STATUS_ALTSERVICE	380 /*Call failed, but alternative choices are possible*/
#define RV_SIPCTRL_STATUS_BADREQUEST	400 /*Malformed syntax */
#define RV_SIPCTRL_STATUS_UNAUTHORIZED	401 /*Request requires user authentication */
#define RV_SIPCTRL_STATUS_PAYMENTREQ	402 /*For future use*/
#define RV_SIPCTRL_STATUS_FORBIDDEN		403 /*Server refuses to fulfill the request*/
#define RV_SIPCTRL_STATUS_NOTFOUND		404 /*User Does not exist at the domain specified in URI*/
#define RV_SIPCTRL_STATUS_NOTALLOWED	405 /*The method specified in the request for this address*/
#define RV_SIPCTRL_STATUS_ENTNOTACCEPT  406 /*Response entities have content characteristics not acceptable according to the accpet headers*/
#define RV_SIPCTRL_STATUS_PROXYAUTHREQ	407 /*Client must first authenticate itself with the proxy*/
#define RV_SIPCTRL_STATUS_REQTIMEOUT	408 /*Server cannot response within the expiration time of the request*/
#define RV_SIPCTRL_STATUS_CONFLICT		409 /*Request can't be completed due to conflict with current state of the resource*/
#define RV_SIPCTRL_STATUS_GONE			410 /*Requested resource not available permanently */
#define RV_SIPCTRL_STATUS_LENGTHREQ		411 /*Content-length header field is missing */
#define RV_SIPCTRL_STATUS_ENTTOOLARGE	413 /*Request is larger than the server is able to process*/
#define RV_SIPCTRL_STATUS_URITOOLONG	414 /*Request-URI is too long */
#define RV_SIPCTRL_STATUS_MEDIANOTSUPP	415 /*UNSUPPORTED_MEDIA_TYPE:*/
#define RV_SIPCTRL_STATUS_BADEXTENSION	420 /*protocol extension was not understood by server*/
#define RV_SIPCTRL_STATUS_TEMPUNAVAIL	480 /*Callee is currently unavailable*/
#define RV_SIPCTRL_STATUS_CALLNOTEXIST	481 /*Call does not exist*/
#define RV_SIPCTRL_STATUS_LOOPDETECTED	482 /*The server received request with a Via path containing itself*/
#define RV_SIPCTRL_STATUS_TOOMANYHOPS	483 /*Request contains too many Via entries than allowed */
#define RV_SIPCTRL_STATUS_PARTIALADRS	484 /*URI or To address is incomplete */
#define RV_SIPCTRL_STATUS_AMBIGOUS		485 /*Callee address is ambigous */
#define RV_SIPCTRL_STATUS_BUSYHERE		486 /*Callee is not able or not willing to take additional calls*/
#define RV_SIPCTRL_STATUS_REQ_TERMINATED 487 /*Call was cancelled*/
#define RV_SIPCTRL_STATUS_NOTACCEPT_HERE 488 /*Not acceptable here*/
#define RV_SIPCTRL_STATUS_REQ_PENDING   491 /*Request Pending*/
#define RV_SIPCTRL_STATUS_SERVERERR		500 /*Internal error in the server*/
#define RV_SIPCTRL_STATUS_NOTIMPLEMENT	501 /*Server doesn't support the functionality required to fulfill the request*/
#define RV_SIPCTRL_STATUS_BADGATEWAY	502 /*Server received invalid response from a server it accessed*/
#define RV_SIPCTRL_STATUS_SERVICEUNAVAIL 503/*Server is temporarily unavailable due to overloading or maintenace*/
#define RV_SIPCTRL_STATUS_GATEWAYTIMEOUT 504/*Server didn't receive a timely response from a server it accessed*/
#define RV_SIPCTRL_STATUS_VERSIONNOTSUPP 505/*Server doesn't support the SIP protocol version that was used in the request*/
#define RV_SIPCTRL_STATUS_BUSYEVERYWHERE 600/*Callee is busy and no other endpoint will answer*/
#define RV_SIPCTRL_STATUS_DECLINE		603 /*Callee does not wish or cannot participate*/
#define RV_SIPCTRL_STATUS_NOTEXISTANYWHERE 604/*User does not exist anywhere*/
#define RV_SIPCTRL_STATUS_NOTACCEPTABLE	606 /*User was contacted successfuly but parts of session description are not acceptable*/



/* OK RESPONSE REASONS */
/*=====================*/
#define RV_SIPCTRL_REASON_BYE		1
#define RV_SIPCTRL_REASON_CANCEL	2
#define RV_SIPCTRL_REASON_INVITE	3
#define RV_SIPCTRL_REASON_OPTIONS	4
#define RV_SIPCTRL_REASON_REGISTER	5

/*-----------------------------------------------------------------------
           C A L L - L E G   C A L L - B A C K  F U N C T I O N S
 ------------------------------------------------------------------------*/

/***************************************************************************
 * RvSipControlCallLegCreatedEv
 * ------------------------------------------------------------------------
 * General:  Notifies that a new call-leg was created, and exchanges handles
 *           with the application.
 * Return Value: (-)
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: 	hCallLeg - The new sip stack call-leg handle
 * Output: 	phAppCallLeg - The application handle for this call-leg.
 ***************************************************************************/
typedef void
		(RVCALLCONV * RvSipControlCallLegCreatedEv)(
                                   IN  RvSipCallLegHandle            hCallLeg,
                                   OUT RvSipAppCallLegHandle         *phAppCallLeg);




/***************************************************************************
 * RvSipControlCallLegStateChangedEv
 * ------------------------------------------------------------------------
 * General: Notifies the application of a call-leg state change.
 *          For each state change the new state is supplied and the
 *      	reason for the state change is also given.
 * Return Value: (-)
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: 	hCallLeg -    The sip stack call-leg handle
 *			hAppCallLeg - The application handle for this call-leg.
 *			eState -      The new call-leg state
 *			eReason -     The reason for the state change.
 ***************************************************************************/
typedef void
		(RVCALLCONV * RvSipControlCallLegStateChangedEv)(
                                   IN  RvSipCallLegHandle            hCallLeg,
                                   IN  RvSipAppCallLegHandle         hAppCallLeg,
                                   IN  RvSipCallLegState             eState,
                                   IN  RvSipCallLegStateChangeReason eReason);

#ifndef EXPRESS_EXTRA_LEAN
/***************************************************************************
 * RvSipControlCallLegReferStateChangedEv
 * ------------------------------------------------------------------------
 * General: Notifies the application of a call-leg refer state change.
 *          For each state change the new state is supplied and the
 *      	reason for the state change is also given.
 * Return Value: (-)
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: 	hCallLeg -    The sip stack call-leg handle
 *			hAppCallLeg - The application handle for this call-leg.
 *			eState -      The new call-leg refer state
 *			eReason -     The reason for the state change.
 ***************************************************************************/
typedef void
		(RVCALLCONV * RvSipControlCallLegReferStateChangedEv)(
                            IN  RvSipCallLegHandle                    hCallLeg,
                            IN  RvSipAppCallLegHandle                 hAppCallLeg,
                            IN  RvSipCallLegReferState                eState,
                            IN  RvSipCallLegStateChangeReason         eReason);


/***************************************************************************
 * RvSipControlCallLegReferNotifyEv
 * ------------------------------------------------------------------------
 * General: notify the application on a refer-notify related event. A
 *          refer-notify is a NOTIFY request that is a part of a refer process.
 *          The callback supplies a reason for the event occurrence as well.
 * Return Value: (-)
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: 	hCallLeg -    The sip stack call-leg handle
 *			hAppCallLeg - The application handle for this call-leg.
 *			eNotification - The new call-leg notify state
 *			eReason -     The reason for the state change.
 *          responseCode - In states: RVSIP_CALL_LEG_REFER_NOTIFY_READY,
 *                         RVSIP_CALL_LEG_REFER_NOTIFY_RECVD,
 *                         RVSIP_CALL_LEG_REFER_NOTIFY_SENT, the response code
 *                         will relate to the status code to be found in the
 *                         NOTIFY request message body. In all other states
 *                         the response code will relate to the status code
 *                         of the response to the NOTIFY request.
 ***************************************************************************/
typedef void
		(RVCALLCONV * RvSipControlCallLegReferNotifyEv)(
                            IN  RvSipCallLegHandle              hCallLeg,
                            IN  RvSipAppCallLegHandle           hAppCallLeg,
                            IN  RvSipCallLegReferNotifyEvents   eNotification,
                            IN  RvSipCallLegStateChangeReason   eReason,
                            IN  RV_INT16                        responseCode);
#endif /* #ifndef EXPRESS_EXTRA_LEAN */


/***************************************************************************
 * RvSipControlCallLegMsgToSendEv
 * ------------------------------------------------------------------------
 * General: An event indicating that a call-leg related outgoing
 *          message is about to be sent.
 * Return Value: RV_OK (the returned status is ignored in the current
 *                           stack version)
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: 	hCallLeg -    The sip stack call-leg handle
 *			hAppCallLeg - The application handle for this call-leg.
 *			hMsg -        Handle to the outgoing message.
 ***************************************************************************/
typedef RvStatus
		(RVCALLCONV * RvSipControlCallLegMsgToSendEv)(
		                           IN  RvSipCallLegHandle            hCallLeg,
                                   IN  RvSipAppCallLegHandle         hAppCallLeg,
                                   IN  RvSipMsgHandle                hMsg);


/***************************************************************************
 * RvSipControlCallLegMsgReceivedEv
 * ------------------------------------------------------------------------
 * General: An event indicating that a call-leg related incoming
 *          message has been received.
 * Return Value: RV_OK (the returned status is ignored in the current
 *                           stack version)
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: 	hCallLeg -    The sip stack call-leg handle
 *			hAppCallLeg - The application handle for this call-leg.
 *			hMsg -        Handle to the outgoing message.
 ***************************************************************************/
typedef RvStatus
		(RVCALLCONV * RvSipControlCallLegMsgReceivedEv)(
	                                IN  RvSipCallLegHandle            hCallLeg,
                                    IN  RvSipAppCallLegHandle         hAppCallLeg,
                                    IN  RvSipMsgHandle                hMsg);



/***************************************************************************
 * RvSipControlCallLegModifyRequestRcvdEv
 * ------------------------------------------------------------------------
 * General: Notifies the application that a request to modify the session
 *          has been received. (A re-INVITE request)
 * Return Value: (-)
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: 	hCallLeg -    The sip stack call-leg handle
 *			hAppCallLeg - The application handle for this call-leg.
 ***************************************************************************/
typedef void
		(RVCALLCONV * RvSipControlCallLegModifyRequestRcvdEv)(
		                           IN  RvSipCallLegHandle            hCallLeg,
                                   IN  RvSipAppCallLegHandle         hAppCallLeg);


/***************************************************************************
 * RvSipControlCallLegModifyResultRcvdEv
 * ------------------------------------------------------------------------
 * General: Notifies the application that a final response to the modify request
 *          the user sent was received.
 * Return Value: (-)
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: 	hCallLeg -    The sip stack call-leg handle
 *			hAppCallLeg - The application handle for this call-leg.
 *			status -      The final response code.
 ***************************************************************************/
typedef void
		(RVCALLCONV * RvSipControlCallLegModifyResultRcvdEv)(
		                            IN  RvSipCallLegHandle            hCallLeg,
                                    IN  RvSipAppCallLegHandle         hAppCallLeg,
                                    IN  RV_UINT16                     status);


typedef void
		(RVCALLCONV * RvSipControlCallLegTrancRequestRcvdEv)(
		                           IN  RvSipCallLegHandle            hCallLeg,
                                   IN  RvSipAppCallLegHandle         hAppCallLeg,
								   IN  RvSipTranscHandle             hTransc);

typedef void
		(RVCALLCONV * RvSipControlCallLegTranscResponseRcvdEv)(
		                            IN  RvSipCallLegHandle            hCallLeg,
                                    IN  RvSipAppCallLegHandle         hAppCallLeg,
									IN  RvSipTranscHandle             hTransc,
                                    IN  RV_UINT16                     status);

typedef struct
{
	RvSipControlCallLegCreatedEv                pfnCallLegCreatedEvHandler;
    RvSipControlCallLegStateChangedEv           pfnStateChangedEvHandler;
    RvSipControlCallLegMsgToSendEv              pfnMsgToSendEvHandler;
    RvSipControlCallLegMsgReceivedEv            pfnMsgReceivedEvHandler;
    RvSipControlCallLegModifyRequestRcvdEv      pfnModifyRequestRcvdEvHandler;
    RvSipControlCallLegModifyResultRcvdEv       pfnModifyResultRcvdEvHandler;
	RvSipControlCallLegTrancRequestRcvdEv       pfnTrancRequestRcvdEvHandler;
    RvSipControlCallLegTranscResponseRcvdEv     pfnTranscResponseRcvdEv;
#ifndef EXPRESS_EXTRA_LEAN
    RvSipControlCallLegReferStateChangedEv      pfnReferStateChangedEvHandler;
    RvSipControlCallLegReferNotifyEv            pfnReferNotifyEvHandler;
#endif

} RvSipControlCallLegEvHandlers;



typedef struct
{
	RvSipStackHandle				stackHndl;		/*Handle to stack manager*/

	/* stack callback handlers for all main stack object
	 * they are stored here because SIP stack has no Get() access to this data
	 */
	RvIppSipStackCallbacks			stackCallbacks;
	RvSipTransport					transportType;
	RvUint16						stackPort;
	char							registrarAddress[RV_SIPCTRL_ADDRESS_SIZE];
	RvUint16						registrarPort;
	char							userDomain[RV_SIPCTRL_ADDRESS_SIZE];/*To use in Registration request as the host in Contact header*/
	char							outboundProxyAddress[RV_SIPCTRL_ADDRESS_SIZE]; /* outbound proxy address */
	RvUint16						outboundProxyPort;
	unsigned int					registerExpires; /*Validation time of client registration to proxy (in sec).*/
	unsigned int					unregisterExpires; /*Validation time of client unregistration to proxy (in sec).*/
	unsigned int					clientRegisterExpires;
	RvInt32                         maxAuthenticateRetries; /* how many times we should respond to consequent 401/407 msgs */
	RvBool                          removeOldAuthHeaders;   /* Will all authentication headers be included in a request */
	char							username[RV_NAME_STR_SZ];
	char							password[RV_NAME_STR_SZ];
	char							contact[RV_NAME_STR_SZ];
    RvBool                          connectMediaOn180;   /* When True, media will be connected both in 180, 183 and 200, 
                                                       when False, media will be connect in 183 and 200 only*/
	RvBool                          addUpdateSupport;
	RvUint8                         updateRetryAfterTimeout;
	RvUint16                        callerUpdateResendTimeout;
	RvUint16                        calleeUpdateResendTimeout;
#ifdef RV_SIP_IMS_ON
	RvBool                          disableAkaAuthentication; /*  When true and the flag RV_SIP_IMS_ON is set,
	                                                              no Aka authentication will be done, only MD5 */
#endif
#ifdef RV_CFLAG_TLS
	RvBool							isTlsEnabled;
#endif
}RvSipControl;


/*
RvSipControl* g_sipControl;
*/

typedef enum
{
	CallHandle,
	RegHandle
}HandleType;


typedef struct
{
	RvSipResource					sipResource;
#ifndef RV_SIP_PRIMITIVES
    RvSipCallLegResources           CallLegResources;
    RvSipRegClientResources         RegClientResources;
	RvSipSubsResources              SubsResources;
#endif /* RV_SIP_PRIMITIVES */
    RvSipStackResources             SipStackResources;
    RvSipTranscResources            TransactionResources;
    RvSipTransmitterResources       TransmitterResources;
    RvSipTransportResources         TransportResources;
#ifdef RV_SIGCOMP_ON
    RvSipCompartmentResources       CompartmentResources;
#endif /* RV_SIGCOMP_ON */
} StackResources;

/*-----------------------------------------------------------------------
                  S I P   C O N T R O L    A P I
 ------------------------------------------------------------------------*/

void rvSipControlConstruct(RvSipControl* x, 
                           RvSipStackHandle stackHandle,
                           RvIppSipPhoneCfg* cfg);
void rvSipControlDestruct(RvSipControl* x);
RvStatus rvSipControlWatchDogTimerConstruct(RvCCProvider* provider);
void rvSipControlStart(RvSipControl* x);
void rvSipControlStop(RvSipControl* x);

#define rvSipControlGetStackHandle(x)			((x)->stackHndl)
#define rvSipControlGetStackPort(x)				((x)->stackPort)
#define rvSipControlGetRegistrarAddress(x)		((x)->registrarAddress)
#define rvSipControlGetRegistrarPort(x)         (((x)->registrarPort==0) ? 5060 : (x)->registrarPort)
#define rvSipControlGetOutboundProxyAddress(x)	((x)->outboundProxyAddress)
#define rvSipControlGetOutboundProxyPort(x)     (((x)->outboundProxyPort==0) ? 5060 : (x)->outboundProxyPort)
#define rvSipControlGetUsername(x)				((x)->username)
#define rvSipControlGetPassword(x)				((x)->password)
#define rvSipControlGetRegisterExpires(x)		((x)->registerExpires)
#define rvSipControlGetTransportType(x)			((x)->transportType)
#define rvSipControlIsTlsEnabled(x)            ((x)->isTlsEnabled)
#define rvSipControlGetMaxAuthenticateRetries(x) ((x)->maxAuthenticateRetries)
void* rvSipControlGetExtUserData(RvSipControl* x);

void rvSipControlSetRegistrarAddress(RvSipControl* x, char* registrarAddress);
#define rvSipControlSetRegistrarPort(x, a)		((x)->registrarPort = a)


/*************************************************************************************
 * rvSipControlForwardCall - Forward  a call
 * ------------------------------------------------------------------------
 * parameters:
 *  IN   hCallLeg    - sip call leg handle 
 *  IN   sendToAddress - the address of the diverted-to user
 * return:
 *       RV_OK if succeeded
 *************************************************************************************/
RvStatus rvSipControlForwardCall(RvSipCallLegHandle	hCallLeg, RvCCConnection* x, RvChar *sendToAddress);

RV_Status rvSipControlCallAccept(RvSipCallLegHandle hCall, RvSdpMsg* sdpMsg);
RV_Status rvSipControlUpdateAccept(RvSipCallLegHandle hCall, RvSdpMsg* sdpMsg);
void rvSipControlReject(RvSipCallLegHandle call, int reason);
void rvSipControlRejectUpdate(RvSipCallLegHandle call, RvSipTranscHandle   hTransc, int reason);
RvStatus rvSipControlCallMake(RvSipControl* x, RvSipAppCallLegHandle call,
							   char* to, char* from);
void rvSipControlSendRinging(RvSipCallLegHandle call);
void rvSipControlSendQueued(RvSipCallLegHandle call);
RvStatus rvSipControlModifyOrRefresh(RvSipCallLegHandle hCall);
RvBool rvSipControlSendModify(RvSipCallLegHandle hCall, RvCCConnection* c, RvSdpMsg* sdpMsg);

/***************************************************************************
 * rvIppGetUserScheme
 * ------------------------------------------------------------------------
 * General: get user scheme: sips or sip or none....
 * ------------------------------------------------------------------------
 * Arguments:
 * input:	str			- pointer to url
 * output:	scheme		- the scheme string
 ***************************************************************************/
void rvIppGetUserScheme(char* str, char* scheme);


/***************************************************************************
 * rvSipMgrGetUserNameFromUrl
 * ------------------------------------------------------------------------
 * General: get user name (ui-alias)
 * ------------------------------------------------------------------------
 * Arguments:
 * input:	str			- pointer to url
 * output:	scheme		- the user name string
 * Return:  None
 ***************************************************************************/
void rvSipMgrGetUserNameFromUrl(char* str, char* username);

/***************************************************************************
 * rvSipMgrGetHostFromUrl
 * ------------------------------------------------------------------------
 * General: get user name (ui-alias)
 * ------------------------------------------------------------------------
 * Arguments:
 * input:	str			- pointer to url
 *			addressLen  - size of address
 * output:	address		- the address string
 * Return:  None
 ***************************************************************************/
void rvSipMgrGetHostFromUrl(char* str, char* address, int addressLen);

/***************************************************************************
 * rvSipMgrGetPortFromUrl
 * ------------------------------------------------------------------------
 * General: Get port number from URL address
 * ------------------------------------------------------------------------
 * Arguments:
 * input:	str			- pointer to url
 *			
 * output:	None
 * Return:  port number
 ***************************************************************************/
RvUint16 rvSipMgrGetPortFromUrl(char* str);

/***************************************************************************
 * rvSipControlRefer
 * ------------------------------------------------------------------------
 * General: Handle the refer message.
 *			This function sends a refer message.
 * Return Value: Returns RvBool: true in case of success, false otherwise.
 * ------------------------------------------------------------------------
 * Arguments:
 * input:	hCall			- handle of the Sip call leg
 *			hMatchedCall	- handle of the matched call.
 *			to				- string 'to'
 *			from			- string 'from'
 * output:	None
 ***************************************************************************/
RvBool rvSipControlRefer(RvSipCallLegHandle hCall, RvSipCallLegHandle hMatchedCall, char* to, char* from);
void rvSipControlTerminate(RvSipCallLegHandle hCall);
RvBool rvSipControlRegisterClient(RvCCTerminalSip *		sipTerm);

RvBool rvSipControlUnregisterClient( RvCCTerminalSip*		sipTerm);

RvStatus rvSipControlMsgSetBody(RvSipMsgHandle hMsg, const RvSdpMsg* sdpMsg);
RvStatus rvSipControlGetToAddressByMessage(RvSipMsgHandle hSipMsg, char *address);
RvBool rvSipControlMsgGetReferredAddress(RvSipMsgHandle hMsg, char* address);

const RV_CHAR*  rvSipControlGetCallLegStateName(RvSipCallLegState  eState);
const RV_CHAR*  rvSipControlGetErrorCodeName(RvStatus errorC);

const RV_CHAR*  rvSipControlGetCallLegStateChangeName (IN  RvSipCallLegStateChangeReason  eState);
const RV_CHAR*  rvSipControlGetCallLegModifyStateName (IN RvSipCallLegModifyState modifyState);
void rvSipControlNewEvent(void* data);

RvBool rvSipControlIsStaleTrue(IN void* handle,IN HandleType type);

RvBool rvSipControlIsNameEmpty(IN const RvChar* name);

RvBool rvSipControlIsAddressEmpty(IN RvChar* address);

/***************************************************************************
 * rvSipControlSetContact
 * ------------------------------------------------------------------------
 * General: copy contact string to RvSipControl object.
 *
 * Return Value: none
 *
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: 	x          -  pointer to Sip control object
 *          contact	   -  contact string
 *
 ***************************************************************************/
void rvSipControlSetContact(RvSipControl* x, char* contact);

RvStatus rvSipControlGetNonceFromAuthHeader(IN RvSipAuthenticationHeaderHandle authHeader,OUT char* nonce);
RvStatus rvSipControlSendDTMF(IN RvSipCallLegHandle hCallLeg, IN RvDtmfParameters* dtmfParam);
 
RvStatus rvSipControlSendFastUpdateGOB(RvSipCallLegHandle hCall,
									   RvUint firstGob,
									   RvUint numGobs);
RvStatus rvSipControlSendFastUpdateMB(RvSipCallLegHandle hCall,
									   RvUint firstGob,
									   RvUint firstMb,
									   RvUint numMbs);

RvStatus rvSipControlSetTransportType(RvSipAddressHandle		hAddress,
										RvSipTransport			transportType);

RvBool rvSipControlIsMediaBody(IN RvSipMsgHandle hMsg);

RvStatus rvSipControlSendUpdate(RvCCConnection* c, RvSdpMsg* sdpMsg);

RvBool rvSipControlRemoveOldAuthorizationHeaders(IN RvSipControl* x);

#ifdef RV_SIP_IMS_ON
RvBool rvSipControlIsAkaAuthenticationDisabled(IN RvSipControl* x);	
#endif

#endif





















