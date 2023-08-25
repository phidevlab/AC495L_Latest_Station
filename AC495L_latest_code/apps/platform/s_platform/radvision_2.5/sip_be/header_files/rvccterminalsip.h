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

typedef struct RvCCTerminalSip_ {
	RvCCProvider*			provider;
	RvMdmXTerm*				mdmXTerm;
	char                    terminalId[RV_NAME_STR_SZ];
	RvCCTerminalType		termType;
	RvAlloc*				alloc;
	RvSipRegClientHandle	regClientObject;
	IppTimer				registerTimer; /*When it expires, register termination with proxy*/
	RvBool					isAuthenticated; /*To make sure we don't send another authentication more than once if authentication failed*/
	/*Configuration Parameters*/
	char					username[RV_NAME_STR_SZ];
	char					password[RV_NAME_STR_SZ];
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
	/*ACL JL 59875 limits number of registrations to proxy to  a configurable number RV patch*/
	RvUint32                authRetriesCount;   /* how many times we have responded to consequent 401/407 msgs */

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
#define rvCCTerminalSipGetIsAuthenticated(x)		((x)->isAuthenticated)
/*ACL JL 59875 limits number of registrations to proxy to  a configurable number RV patch*/
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
#define rvCCTerminalSipGetRegisterTimeout( x)		((x)->registerExpires * 660 ) 
#define rvCCTerminalSipGetUnregisterTimeout(x)		((x)->unregisterExpires *1000)
#define rvCCTerminalSipSetIsAuthenticated(x,a)		((x)->isAuthenticated=a)
/*ACL JL 59875 limits number of registrations to proxy to  a configurable number RV patch*/
#define rvCCTerminalSipSetAuthRetriesCount(x,a)     ((x)->authRetriesCount=a)
#define rvCCTerminalSipSetRegisterExpires( x, a)	((x)->registerExpires=a)
#define rvCCTerminalSipSetUnregisterExpires( x, a)  ((x)->unregisterExpires=a)
#define rvCCTerminalSipSetRegType(x,a)				((x)->networkRegisterType=a)
#define rvCCTerminalSipSetRegState(x,a)				((x)->networkRegisterState=a)
#define rvCCTerminalSipGetToolkitRegisterState(x)	((x)->toolkitRegisterState)
#define rvCCTerminalSipSetToolkitRegisterState(x,a)	((x)->toolkitRegisterState=a)
#define rvCCTerminalSipSetClientRegisterExpires(x,a) ((x)->clientRegisterExpires=a)
#define rvCCTerminalSipGetMdmTerminal(x)			((RvCCTerminal*)((x)->mdmXTerm))

/******************************************************************************
*  rvCCTerminalSipSetNonce
*  ----------------------------
*  General :        copy nonce into RvCCTerminalSip object.
*
*  Return Value:   pointer to CC Terminal object
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          x				pointer to CC Terminal object
*				   nonce			nonce string
*  Output:         none.
******************************************************************************/
void rvCCTerminalSipSetNonce(IN RvCCTerminal* x,OUT char* nonce);

/******************************************************************************
*  rvCCTerminalSipGetNonce
*  ----------------------------
*  General :        get nonce string from RvCCTerminalSip object.
*
*  Return Value:   nonce string
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          x				pointer to CC Terminal object
*
*  Output:         none.
******************************************************************************/
const char* rvCCTerminalSipGetNonce(IN RvCCTerminal* x);

#endif /*RV_CCTERMINAL_SIP_H*/
