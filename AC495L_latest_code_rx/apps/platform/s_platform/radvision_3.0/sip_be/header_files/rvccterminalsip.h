/******************************************************************************
Filename:    rvccterminalsip.h
Description: SIP Terminal Header
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
#include "rvstring.h"
#include "rvtimer.h"
#include "rvvector.h"
#include "rvansi.h"
#include "rvccapi.h"
#include "RvSipRegClient.h"

#ifndef RV_CCTERMINAL_SIP_H
#define RV_CCTERMINAL_SIP_H


typedef enum {
	RV_TERM_NETWORK_REG_TYPE_REGISTERING,
	RV_TERM_NETWORK_REG_TYPE_UNREGISTERING
} RvNetworkRegisterType;


typedef enum {
	RV_TERM_NETWORK_REG_STATE_UNREGISTERED,
	RV_TERM_NETWORK_REG_STATE_REGISTERED
} RvNetworkRegisterState;


typedef enum {
	RV_TERM_TOOLKIT_REG_STATE_NONE,	 	/* No request to unregister, or the originator is the network  */
	RV_TERM_TOOLKIT_REG_STATE_UNREGISTERING,	/* Request from IPPTK to unregister */
} RvToolkitRegisterState;

#define MAX_NUM_OF_ROUTE_HEADER_LIST 10
/*defines a struct that contains all the data that is related to the routeList*/
typedef struct
{
/*The page on which we save the service-route headers,in order to
	create later the Route header list from*/
    HPAGE                     serviceRouteListPage;
	
	/*routeHeader list (built from the serviceRoute list)*/
    RvSipRouteHopHeaderHandle routeHopHeaderList[MAX_NUM_OF_ROUTE_HEADER_LIST];
	
	/*counts how many service-route headers are in the list*/
    RvInt                     numOfRouteHopHeadersInList;
	
} RouteListDataType;

typedef struct RvCCTerminalSip_ {
	RvCCProvider*			provider;
	RvMdmXTerm*				mdmXTerm;
	char                    terminalId[RV_NAME_STR_SZ];
	RvCCTerminalType		termType;
	RvSipTransport          transportType;
	RvAlloc*				alloc;
	RvSipRegClientHandle	regClientObject;
	IppTimer				registerTimer; /*When it expires, register termination with proxy*/
	/*Configuration Parameters*/
	char					username[RV_NAME_STR_SZ];
	char					password[RV_NAME_STR_SZ];
	char					registrarAddress[RV_IPP_ADDRESS_SIZE];
	RvUint16				registrarPort;
	char					outboundProxyAddress[RV_IPP_ADDRESS_SIZE]; 
	RvUint16				outboundProxyPort;
	char					presentationName[RV_NAME_STR_SZ];
	unsigned int			registerExpires; /*Validation time of client registration to proxy (in sec).*/
	unsigned int			clientRegisterExpires ;/* to send proxy our default expires even if they change the expires*/
	unsigned int			unregisterExpires; /*Validation time of client unregistration to proxy (in sec). (set timer)*/
	RvNetworkRegisterType	networkRegisterType;       /* Registering OR Unregistering */
	/* The following 2 fields indicates the state of unregistration:
	networkRegisterState: indicates the final state of the terminal in front of the network: registered or unregistered.
	toolkitRegisterState: indicates who is the originator of the unregister request.
	If the originator is the toolkit, the state will be RV_TERM_TOOLKIT_REG_STATE_UNREGISTERING.
	Otherwise, it the network is the originator, or there is No request to perform unregistration,
	the state will be RV_TERM_TOOLKIT_REG_STATE_NONE. */
	RvNetworkRegisterState	networkRegisterState; /* Registered OR Unregistered */
	RvToolkitRegisterState	toolkitRegisterState;
	char					nonce[RV_NAME_STR_SZ];
#if 0 /*removed temporary until we get reply from RV if it was fixed in MTF3.0*/
	/*ACL AL 080910 SIP Registration issue: not answering to 401 message*/
	RvUint32				regClientUnauthCount;    /* Counts the number of 401/407 replies of the reg client. This count is set to 0 when the reg client is recreated. */ 
#endif
	RvUint32                authRetriesCount;   /* how many times we have responded to consequent 401/407 msgs */
	RouteListDataType       routeListData;
} RvCCTerminalSip;


RvCCTerminalSip* rvCCTerminalSipConstruct(RvCCTerminal*					x,
										  RvCCProvider*					p,
										  const char*					id,
										  RvMdmTermDefaultProperties*	termProperties,
										  RvAlloc*						a);

void rvCCTerminalSipDestruct(RvCCTerminal * x);



#define RV_SIPCTRL_PROXY_REG_DELTA					1000
#define rvCCTerminalSipGetImpl(x)					((RvCCTerminalSip*)x->terminal)
#define rvCCTerminalSipGetProvider(x)				((x)->provider)
#define rvCCTerminalSipGetTermId(x)					((x)->terminalId)
#define rvCCTerminalSipGetRegisterTimer(x)			(&(x)->registerTimer)
#define rvCCTerminalSipGetUsername(x)				((x)->username)
#define rvCCTerminalSipGetPassword(x)				((x)->password)
#define rvCCTerminalSipGetRegClientObject(x)		((x)->regClientObject)
#define rvCCTerminalSipGetAuthRetriesCount(x)       ((x)->authRetriesCount)
#define rvCCTerminalSipGetRegisterExpires( x)		((x)->registerExpires)
#define rvCCTerminalSipGetClientRegisterExpires		(x) ((x)->clientRegisterExpires)
/*Compute timeout that intialize register timer*/
//#define rvCCTerminalSipGetRegisterTimeout( x)		((x)->registerExpires *1000 - RV_SIPCTRL_PROXY_REG_DELTA) 
/* ACL NB 070508                                                  */
/* The timeout for next registration should be registerExpires/2  */
/* registerExpires *1000 convert to msec units                    */
/* registerExpires *1000/2 =  registerExpires *500                */
/*#define rvCCTerminalSipGetRegisterTimeout( x)		((x)->registerExpires * 500 ) */
#define rvCCTerminalSipGetRegisterTimeout( x)		((x)->registerExpires * 660) 
#define rvCCTerminalSipGetUnregisterTimeout(x)		((x)->unregisterExpires *1000)
#define rvCCTerminalSipSetAuthRetriesCount(x,a)     ((x)->authRetriesCount=a)
#define rvCCTerminalSipSetRegisterExpires( x, a)	((x)->registerExpires=a)
#define rvCCTerminalSipSetUnregisterExpires( x, a)  ((x)->unregisterExpires=a)
#define rvCCTerminalSipSetRegType(x,a)				((x)->networkRegisterType=a)
#define rvCCTerminalSipSetRegState(x,a)				((x)->networkRegisterState=a)
#define rvCCTerminalSipGetToolkitRegisterState(x)	((x)->toolkitRegisterState)
#define rvCCTerminalSipSetToolkitRegisterState(x,a)	((x)->toolkitRegisterState=a)
#define rvCCTerminalSipSetClientRegisterExpires(x,a) ((x)->clientRegisterExpires=a)
#define rvCCTerminalSipGetMdmTerminal(x)			((RvCCTerminal*)((x)->mdmXTerm))
#define rvCCTerminalSipGetRouteListData(x)           (&(x)->routeListData)
#if 0 /*removed temporary until we get reply from RV if it was fixed in MTF3.0*/
/*ACL AL 080910 SIP Registration issue: not answering to 401 message*/
#define rvCCTerminalSipGetRegClientUnauthCount(x)      ((x)->regClientUnauthCount)
#define rvCCTerminalSipSetRegClientUnauthCount(x,a)    ((x)->regClientUnauthCount = a)
#endif

/******************************************************************************
*  rvCCTerminalSipSetNonce
*  ----------------------------
*  General :        copy nonce into RvCCTerminalSip object.
*
*  Return Value:   pointer to CC Terminal object
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          x				pointer to CC Terminal Sip object
*				   nonce			nonce string
*  Output:         none.
******************************************************************************/
void rvCCTerminalSipSetNonce(IN RvCCTerminalSip* x,OUT char* nonce);

/******************************************************************************
*  rvCCTerminalSipGetNonce
*  ----------------------------
*  General :        get nonce string from RvCCTerminalSip object.
*
*  Return Value:   nonce string
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          x				pointer to CC Terminal Sip object
*
*  Output:         none.
******************************************************************************/
const char* rvCCTerminalSipGetNonce(IN RvCCTerminalSip* x);

/******************************************************************************
*  rvCCTerminalSipGetRegistrarAddress
*  ----------------------------------
*  General :        get registrarAddress string from RvCCTerminalSip object.
*
*  Return Value:   registrarAddress string
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          x				pointer to CC Terminal Sip object
*
*  Output:         none.
******************************************************************************/
const char* rvCCTerminalSipGetRegistrarAddress(IN RvCCTerminalSip* x);

/******************************************************************************
*  rvCCTerminalSipGetRegistrarPort
*  ----------------------------------
*  General :        get registrarPort number from RvCCTerminalSip object.
*
*  Return Value:   port number
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          x				pointer to CC Terminal Sip object
*
*  Output:         none.
******************************************************************************/
RvUint16 rvCCTerminalSipGetRegistrarPort(IN RvCCTerminalSip* x);

/******************************************************************************
*  rvCCTerminalSipGetOutboundProxyAddress
*  ---------------------------------------
*  General :        get OutboundProxyAddress string from RvCCTerminalSip object.
*
*  Return Value:   OutboundProxy address string
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          x				pointer to CC Terminal Sip object
*
*  Output:         none.
******************************************************************************/
const char* rvCCTerminalSipGetOutboundProxyAddress(IN RvCCTerminalSip* x);

/******************************************************************************
*  rvCCTerminalSipGetOutboundProxyPort
*  ------------------------------------
*  General :        get OutboundProxyPort number from RvCCTerminalSip object.
*
*  Return Value:   port number
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          x				pointer to CC Terminal Sip object
*
*  Output:         none.
******************************************************************************/
RvUint16 rvCCTerminalSipGetOutboundProxyPort(IN RvCCTerminalSip* x);

/******************************************************************************
*  rvCCTerminalSipResetRouteHopHeaderList
*  --------------------------------------
*  General :        resets the routeListData of the RvCCTerminalSip object.
*
*  Return Value:   none
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          term				pointer to sip terminal
*				   
*  Output:         none.
******************************************************************************/
void rvCCTerminalSipResetRouteHopHeaderList(INOUT RvCCTerminalSip* term);

/******************************************************************************
*  rvCCTerminalSipGetTransportType
*  ----------------------------------------
*  General :        returns the transport type of the rvCCTerminalSip object
*                   
*  Return Value:   transport type
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          x				pointer to sip terminal	
*				   
*  Output:        none
******************************************************************************/
RvSipTransport rvCCTerminalSipGetTransportType(IN RvCCTerminalSip* x);

/******************************************************************************
*  rvCCTerminalSipGetTransportByConnection
*  ----------------------------------------
*  General :        retrieves the transport type of the rvCCTerminalSip object
*                   that is attached to a sip connection
*
*  Return Value:   transport type
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          x 		rvCCConnection		
*				   
*  Output:        none
******************************************************************************/
RvSipTransport rvCCTerminalSipGetTransportByConnection(IN RvCCConnection* c);

#endif /*RV_CCTERMINAL_SIP_H*/
