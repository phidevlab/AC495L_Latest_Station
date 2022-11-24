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
#ifndef RV_EPP_H
#define RV_EPP_H

#include "rvtypes.h"
#include "rvsocket.h"
#include "ippmisc.h"
/*#include "rvstring.h"*/
/*#include "rvdefalloc.h"*/

#ifdef __cplusplus
extern "C" {
#endif

typedef int RvEppConnId;

#define RV_EPPCONNPARAMETERS_MODE			0x01
#define RV_EPPCONNPARAMETERS_CODEC			0x02
#define RV_EPPCONNPARAMETERS_RATE			0x04
#define RV_EPPCONNPARAMETERS_IP				0x08
#define RV_EPPCONNPARAMETERS_PORT			0x10

typedef unsigned int RvEppConnMode;
#define RV_EPPCONNMODE_INACTIVE				0x00
#define RV_EPPCONNMODE_SEND					0x01
#define RV_EPPCONNMODE_RECV					0x02
#define RV_EPPCONNMODE_SENDRECV				0x03
#define RV_EPPCONNMODE_CONFERENCE			0x04
#define RV_EPPCONNMODE_REPLICATE			0x05
#define RV_EPPCONNMODE_LOOPBACK				0x06
#define RV_EPPCONNMODE_CONTINUITY			0x07

typedef struct {
	unsigned int mask;
	unsigned int mode;
	unsigned int codec;
	unsigned int rate;
	char ip[RV_IPP_LEN_STRING_IP];
	RvUint16 port;
} RvEppConnParameters;

RvEppConnParameters* rvEppConnParametersConstruct(RvEppConnParameters* cp);
RvEppConnParameters* rvEppConnParametersConstructDecode(RvEppConnParameters* cp, char* connParams);
char* rvEppConnParametersEncode(const RvEppConnParameters* cp, char* connParamStr);
#define rvEppConnParametersDestruct(cp)
#define rvEppConnParametersIsModeSet(cp)		((cp)->mask & RV_EPPCONNPARAMETERS_MODE)
#define rvEppConnParametersIsCodecSet(cp)		((cp)->mask & RV_EPPCONNPARAMETERS_CODEC)
#define rvEppConnParametersIsRateSet(cp)		((cp)->mask & RV_EPPCONNPARAMETERS_RATE)
#define rvEppConnParametersIsIpSet(cp)			((cp)->mask & RV_EPPCONNPARAMETERS_IP)
#define rvEppConnParametersIsPortSet(cp)		((cp)->mask & RV_EPPCONNPARAMETERS_PORT)

/* Get functions return undefined results if the attribute has not yet been set */
#define rvEppConnParametersGetMode(cp)			((cp)->mode)
#define rvEppConnParametersGetCodec(cp)			((cp)->codec)
#define rvEppConnParametersGetRate(cp)			((cp)->rate)
#define rvEppConnParametersGetIp(cp)			((cp)->ip)
#define rvEppConnParametersGetPort(cp)			((cp)->port)

void rvEppConnParametersSetMode(RvEppConnParameters* cp, RvEppConnMode mode);
void rvEppConnParametersSetCodec(RvEppConnParameters* cp, unsigned int codec);
void rvEppConnParametersSetRate(RvEppConnParameters* cp, unsigned int rate);
void rvEppConnParametersSetIp(RvEppConnParameters* cp, const char* ip);
void rvEppConnParametersSetPort(RvEppConnParameters* cp, RvUint16 port);

#ifdef __cplusplus
}
#endif

#endif
