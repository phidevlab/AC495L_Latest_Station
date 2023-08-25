/******************************************************************************
Filename    :ipp_inc.std.h
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

#ifndef IPP_CallControl_STD_H
#define IPP_CallControl_STD_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "rvconfig.h"
#include "rvalloc.h"
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



#define RV_NAME_STR_SZ			256
#define RV_SHORT_STR_SZ			256
#define RV_MEDIUM_STR_SZ		512
#define RV_LONG_STR_SZ			1024




/*
 *	from rvutil.h
 */

#ifdef __cplusplus
extern "C" {
#endif

extern RvAlloc*	prvDefaultAlloc;

#ifdef __cplusplus
}
#endif


#include "ipplog.h"
#include "ipptimer.h"

#include "IppStdInc.h"



#endif /* IPP_CallControl_STD_H */

