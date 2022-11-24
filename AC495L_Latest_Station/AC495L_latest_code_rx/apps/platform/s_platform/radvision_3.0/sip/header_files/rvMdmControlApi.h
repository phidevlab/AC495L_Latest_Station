/******************************************************************************
Filename:    rvMdmControlApi.h
Description: This file includes MDM extension APIs and type definitions.
			 Note:
			 ----
			 Since version 3.0 and up the APIs and type definitions in this file
			 are deprecated and located in file rvMtfInternals.h.
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
#ifndef RV_MDMCONTROL_API_H
#define RV_MDMCONTROL_API_H

/* This is done for backward compatibility */
#include "rvMtfInternals.h"

/*@*****************************************************************************
*  rvMtfGetLogMgr (RvMtfBasePkg)
* -----------------------------------------------------------------------------
*  General :   Gets the MTF Log Manager as void*. This function can be used  
*			   to pass the Log Manager to another RADVISION toolkit.
*
*  Arguments:
*  Input:      hConn - Handle to the connection.
*
*  Return Value:  Pointer to the MTF Log Manager.
****************************************************************************@*/
RVAPI void* RVCALLCONV rvMtfGetLogMgr(void);

/*@*****************************************************************************
*  rvMtfTerminalMakeCall (RvMtfTerminationsPkg)
* -----------------------------------------------------------------------------
*  General :    Initiates an outgoing call to a destination address on the  
*				specified line and termination. Before calling this function, 
*				the application should send an off-hook event. To disconnect 
*				the call, the application should send an on-hook event. This  
*				function skips digit collection and translation into an 
*               address.
*
*  Arguments:
*  Input:       terminalHndl	- Terminal handle on which to dial.
*				address			- Destination address to which to dial. 
*								  This address will be included in the To header 
*                                 of the outgoing Invite message.
*
*  Return Value:    RV_OK if successful, other if not.
****************************************************************************@*/
RVAPI RvStatus RVCALLCONV rvMtfTerminalMakeCall(
    IN RvIppTerminalHandle  terminalHndl,
    IN const RvChar*        address);

#endif /*RV_MDMCONTROL_API_H*/
