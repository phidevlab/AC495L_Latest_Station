#if (0)
******************************************************************************
Filename   :
Description:
******************************************************************************
                Copyright (c) 1999 RADVision Inc.
************************************************************************
NOTICE:
This document contains information that is proprietary to RADVision LTD.
No part of this publication may be reproduced in any form whatsoever
without written prior approval by RADVision LTD..

RADVision LTD. reserves the right to revise this publication and make
changes without obligation to notify any person of such revisions or
changes.
******************************************************************************
$Revision:$
$Date:$
$Author: S. Cipolli$
******************************************************************************
#endif

#ifndef RV_EPPCLIENT_H
#define RV_EPPCLIENT_H

#include "rvtypes.h"
#include "rvptrlist.h"
#include "rvthread.h"
#include "rvsocket.h"
#include "rvepp.h"
#include "rvlist.h"

#ifdef __cplusplus
extern "C" {
#endif

#define EP_PROFILE_ANALOG	0
#define EP_PROFILE_IPPHONE	1
/* Client */
struct RvEppClient_;
		
typedef struct {
	struct RvEppClient_*	ec;
	RvSocket		s;
	char			name[32];
	char			packages[32];
	RvAddress		address;
	void*			data;
	int				profile;
} RvEppClientEndpoint;

RvEppClientEndpoint* rvEppClientEndpointConstruct( RvEppClientEndpoint* ece, 
												  struct RvEppClient_* ec,
												  const char* name, 
												  const char* packages,
												  const char* ip, 
												  RvUint16 port, 
												  int profile);

void rvEppClientEndpointDestruct(RvEppClientEndpoint* ece);
#define rvEppClientEndpointGetName(ece)			((ece->name))
#define rvEppClientEndpointGetPackages(ece)		((ece)->packages)
#define rvEppClientEndpointGetProfile(ece)		((ece)->profile)
#define rvEppClientEndpointGetUserData(ece)		((ece)->data)
#define rvEppClientEndpointSetUserData(ece, d)	((ece)->data = (d))

typedef RvBool (*RvEppClientRegisterAllowed)( int profile, const char* name);/*return rvTrue if registration of ece is allowed */

typedef RvBool (*RvEppClientRegister)( RvEppClientEndpoint* ece, void *data);/*return rvTrue if register procedure succeeded*/

typedef void (*RvEppClientEvent)(struct RvEppClient_* ec, RvEppClientEndpoint* ece, const char* eventName, char* eventParams, void *data);

typedef void (*RvEppClientUnregister)(struct RvEppClient_* ec, RvEppClientEndpoint* ece, void *data);

typedef struct RvEppClient_ 
{
	RvSocket				s;
	RvThread				thread;
	RvBool					done;
	RvPtrList				endpoints;
	RvAddress				localAddr;
	RvEppClientRegisterAllowed	eRegisterAllowed;
	RvEppClientRegister		eRegister;
	RvEppClientEvent		eEvent;
	RvEppClientUnregister	eUnregister;
	void *data;
} RvEppClient;


RvEppClient* rvEppClientConstruct( 
									RvEppClient* ec, 
									const char*			szEppClientIp,
									RvUint16 port,
									RvEppClientRegisterAllowed	eRegisterAllowed,
									RvEppClientRegister eRegister, 
									RvEppClientEvent eEvent,
									RvEppClientUnregister eUnregister, 
									void *data);
void rvEppClientDestruct(RvEppClient* ec);
RvEppConnId rvEppClientOpen( RvEppClientEndpoint* ece, RvEppConnParameters* connParams);
RvBool rvEppClientModify( RvEppClientEndpoint* ece, RvEppConnId connId, RvEppConnParameters* connParams);
char* rvEppClientQuery( RvEppClientEndpoint* ece, char* termConnId, const char* propertyName, OUT char* propertyValue);
RvBool rvEppClientSet( RvEppClientEndpoint* ece, char* termConnId,
					  const char* propertyName, const char* propertyValue);
RvUint16 rvEppClientGetPort(RvEppClient* ec, RvEppClientEndpoint* ece,
  RvEppConnId connId);
RvBool rvEppClientClose( RvEppClientEndpoint* ece, RvEppConnId connId);
RvBool rvEppClientStart( RvEppClientEndpoint* ece, const char* signal);
RvBool rvEppClientStop( RvEppClientEndpoint* ece, const char* signal);

void rvEppClientEndpointSend(RvEppClientEndpoint* ece,
							 IN char* msgSend,
							 IN RvSize_t sizeSend,
							 IN RvSize_t msgRcvdMaxSize,
							 OUT char* msgRcvd);
							 
#ifdef __cplusplus
}
#endif

#endif
