/******************************************************************************
Filename    :IppStdInc.h
Description :Header for users of SDP stack

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
$Revision:1.0$
$Date:7/18/99$
$Author:Dan Elbert$
******************************************************************************/

#ifndef IPP_STD_INC_H
#define IPP_STD_INC_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#include "rvmemory.h"
#include "rvconfig.h"
#include "rvalloc.h"
#include "rvmtfalloc.h"
#include "rvdefalloc.h"
#include "rvtypes.h"
#include "ex_rvtypes.h"
#include "rverror.h"
#include "rvtm.h"
#include "rvlog.h"
#include "rvmutex.h"

#include "rvmap.h"
#include "rvalgo.h"
/*SDP support*/
#include "rvsdpmsg.h"
#include "rvsdpprsaux.h"
#include "rvsdpsymb.h"

#define APP_NAME_STR_SZ			256
#define APP_SHORT_STR_SZ		256
#define APP_MEDIUM_STR_SZ		512

#define RV_SIPCTRL_CALLER_ID_IPADDRESS		1
#define RVSIPCTRL_DEFAULT_PORT				5060
#define RVSIPCTRL_NO_PORT					0

#ifdef __cplusplus
extern "C" {
#endif

extern RvAlloc*	userDefaultAlloc;



/*send message over socket of another library*/
/*get bound address and methods allowing 
to send raw buffer from this address*/
/*---------------------------------------------------*/
typedef RvStatus (*SendMsgCB)( IN void* usrData, const char* dsrIpString, RvUint16 dstPort, IN RvUint8* buf, IN RvSize_t size);
typedef struct 
{
	char		srcAddr [64];
	RvUint16	srcPort;
	void*		usrData;
	SendMsgCB	sendMsgCB;
} SendMethod;



#ifdef __cplusplus
}
#endif


#endif /* IPP_STD_INC_H */

