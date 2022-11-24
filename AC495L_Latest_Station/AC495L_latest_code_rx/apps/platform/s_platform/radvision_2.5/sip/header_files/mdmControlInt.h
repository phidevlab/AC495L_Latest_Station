/******************************************************************************
Filename:    mdmControlInt.h
Description: MDM Control Internal APIs
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
#ifndef MDMCONTROL_INT_H
#define MDMCONTROL_INT_H

#include "rvmdm.h"
#include "rvMdmControlApi.h"
#include "rvccapi.h"



/*-----------------------------------------------------------------------
					S T A C K		E X T E N S I O N S		
 ------------------------------------------------------------------------*/
RvCCTerminalEvent  rvIppMdmExtPreProcessEventCB(
    IN RvCCConnection*         c,
    IN RvCCTerminalEvent       eventId,
    INOUT RvCCEventCause*       reason);

void rvIppMdmExtPostProcessEventCB(
    IN RvCCConnection*          c,
    IN RvCCTerminalEvent        eventId,
    IN RvCCEventCause           reason);

void rvIppMdmExtConnectionCreatedCB(
    IN RvCCConnection*			c); 

void rvIppMdmExtConnectionDestructedCB(
    IN RvCCConnection*			c);

RvCCTerminalEvent rvIppMdmExtMapUserEventCB(
    IN const RvChar*        pkg,
    IN const RvChar*        id,
    IN RvMdmParameterList*  args,
    IN RvChar*              key);

void rvIppMdmExtDisplayCB(RvCCConnection*		c, 
						RvCCTerminal*		term,
						RvCCTerminalEvent	event, 
						RvCCEventCause		cause,
						void*				displayData);


#endif /*MDMCONTROL_INT_H*/
