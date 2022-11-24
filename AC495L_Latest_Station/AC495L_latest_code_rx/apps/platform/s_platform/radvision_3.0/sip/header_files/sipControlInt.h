/******************************************************************************
Filename:    sipControlInt.h
Description: SIP Control Internal Extension APIs
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
#ifndef SIPCONTROL_INT_H
#define SIPCONTROL_INT_H

#include "rvmdm.h"
#include "RvSipStackTypes.h"
#include "RvSipCallLegTypes.h"
#include "RvSipRegClientTypes.h"
#include "sipMgr.h"

/*-----------------------------------------------------------------------
					S T A C K		E X T E N S I O N S		
 ------------------------------------------------------------------------*/
/******************************************************************************
*  rvIppSipExtStackConfigCB
*  -------------------------
*  General :        Calls user callback for changing stack configuration, and
*                   validating the changed parameters.
*                   Parameters with illegal or unsupported values will be ignored
*                   or changed back to the original value.
*
*  Return Value:   None
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          pStackCfg              pointer of stack configuration structure
*
*  Output:         none.
******************************************************************************/
void rvIppSipExtStackConfigCB(RvSipStackCfg* stackCfg);

/******************************************************************************
*  rvIppSipExtRegisterStackEventsCB
*  ---------------------------------
*  General :        Calls user callback for registering stack events that IPP TK
*                   doesn't listen on.
*
*  Return Value:   None
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          stackHandle              stack handle
*
*  Output:         none.
******************************************************************************/
void rvIppSipExtRegisterStackEventsCB(RvSipControl*  sipMgr, RvSipStackHandle stackHandle);

/******************************************************************************
*  rvIppSipExtPreCallLegCreatedIncomingCB
*  ---------------------------------
*  General :        Calls user callback to be called when RvSipCallLegCreatedEvHandler event
*                   is received from the stack, before it is processed by IPP TK.
*
*  Return Value:   True - if event should be processed by IPP TK
*                  False - if event should be ignored by IPP TK
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          sipMgrHndl				handle of SipControl object
*                  hCallLeg					handle of call leg
*                  phAppCallLeg             handle of application
*
*  Output:         none.
******************************************************************************/
RvBool rvIppSipExtPreCallLegCreatedIncomingCB(
												RvIppSipControlHandle    sipMgrHndl,
												RvSipCallLegHandle       hCallLeg,
												RvSipAppCallLegHandle*   phAppCallLeg);

/******************************************************************************
*  rvIppSipExtPostCallLegCreatedIncomingCB
*  ---------------------------------
*  General :        Calls user callback to be called when RvSipCallLegCreatedEvHandler event
*                   is received from the stack, after it is processed by IPP TK.
*
*  Return Value:   None
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          sipMgrHndl   - Handle of SipControl object
*                  hCallLeg     - Handle of call leg
*                  hLine  - Handle to the MTF connection associated with
*                                 the call leg.
*
*  Output:         none.
******************************************************************************/
void rvIppSipExtPostCallLegCreatedIncomingCB(
    IN RvIppSipControlHandle    sipMgrHndl,
	IN RvSipCallLegHandle       hCallLeg,
	IN RvIppConnectionHandle    hLine);

/******************************************************************************
*  rvIppSipExtPreCallLegCreatedOutgoingCB
*  ---------------------------------
*  General :        Calls user callback to be called before sending out Invite
*                   message - after call-leg was created, and before setting
*                   parameters to the Invite message.
*
*  Return Value:   None
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:  sipMgrHndl   - handle of SipControl object
*          hLine  - handle of the MTF connection object
*          hCallLeg     - handle of call leg
*          to, from     - address strings
*  Output: none.
******************************************************************************/
void rvIppSipExtPreCallLegCreatedOutgoingCB(
    IN RvIppSipControlHandle    sipMgrHndl,
    IN RvIppConnectionHandle    hLine,
    IN RvSipCallLegHandle       hCallLeg,
    IN RvChar*                  to,
    IN RvChar*                  from);

/******************************************************************************
*  rvIppSipExtPostCallLegCreatedOutgoingCB
*  ---------------------------------
*  General :        Calls user callback to be called before sending out Invite
*                   message - after setting parameters to the Invite message,
*                   and before calling RvSipCallLegMake() to send out the message.
*
*  Return Value:   None
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          sipMgrHndl   - Handle of SipControl object
*                  hLine  - Connection object associated with this call
*                  hCallLeg     - The call leg created
*  Output:         none.
******************************************************************************/
void rvIppSipExtPostCallLegCreatedOutgoingCB(
    IN RvIppSipControlHandle    sipMgrHndl, 
    IN RvIppConnectionHandle    hLine,
    IN RvSipCallLegHandle       hCallLeg);


/******************************************************************************
*  rvIppSipExtPreStateChangedCB
*  ---------------------------------
*  General :        Calls user callback after processing Sip CallLeg changed event
*                   .
*
*  Return Value:   none
*                  
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          sipMgrHndl				handle of SipControl object
*                  hCallLeg					handle of call leg
*                  phAppCallLeg             handle of application
*                  RvSipCallLegState		eState
*                  RvSipCallLegStateChangeReason		eReason, 
*                  
*  Output:         rvTrue, rvFalse.
*  Note:		   eState == RVSIP_CALL_LEG_STATE_DISCONNECTED is the last occasion to use an application handle - phAppCallLeg.
******************************************************************************/
RvBool rvIppSipExtPreStateChangedCB(RvIppSipControlHandle sipMgrHndl,
										   RvSipCallLegHandle     hCallLeg,
										   IN  RvSipAppCallLegHandle         hAppCallLeg,
										   IN  RvSipCallLegState			 eState,
										   IN  RvSipCallLegStateChangeReason eReason);


void rvIppSipExtPostStateChangedCB(RvIppSipControlHandle sipMgrHndl,
                                   RvSipCallLegHandle     hCallLeg,
                                   IN  RvSipAppCallLegHandle         hAppCallLeg,
                                   IN  RvSipCallLegState			 eState,
                                   IN  RvSipCallLegStateChangeReason eReason);

RvBool rvIppSipExtPreMsgToSendCB(RvIppSipControlHandle sipMgrHndl,
										RvSipCallLegHandle	hCallLeg,
										IN  RvSipAppCallLegHandle   hAppCallLeg,
										IN  RvSipMsgHandle          hMsg);

void rvIppSipExtPostMsgToSendCB(RvIppSipControlHandle sipMgrHndl,
										RvSipCallLegHandle	hCallLeg,
										IN  RvSipAppCallLegHandle   hAppCallLeg,
										IN  RvSipMsgHandle          hMsg);

RvMtfMsgProcessType rvIppSipExtPreMsgReceivedCB(RvIppSipControlHandle sipMgrHndl,
										  RvSipCallLegHandle	hCallLeg,
										  IN  RvSipAppCallLegHandle   hAppCallLeg,
										  IN  RvSipMsgHandle          hMsg);

void rvIppSipExtPostMsgReceivedCB(RvIppSipControlHandle sipMgrHndl,
										  RvSipCallLegHandle	hCallLeg,
										  IN  RvSipAppCallLegHandle   hAppCallLeg,
										  IN  RvSipMsgHandle          hMsg);
RvBool RvIppSipExtPreRegClientStateChangedCB(	RvIppSipControlHandle			sipMgrHndl,
												RvSipRegClientHandle            hRegClient,
												RvIppTerminalHandle				mdmTerminalHndl,
												RvSipRegClientState             eState,
												RvSipRegClientStateChangeReason eReason);

RvBool RvIppSipExtRegClientCreatedCB(
				RvSipRegClientHandle            hRegClient);

RvStatus RvIppSipExtRegExpResolutionNeededCB(
                                IN RvIppSipControlHandle                        sipMgrHndl,
                                IN  RvSipTransmitterMgrHandle                   hTrxMgr,
                                IN  void*                                       pAppTrxMgr,
                                IN  RvSipTransmitterHandle                      hTrx,
                                IN  RvSipAppTransmitterHandle                   hAppTrx,
                                INOUT RvSipTransmitterRegExpResolutionParams*   pRegExpParams);


#endif /*SIPCONTROL_INT_H*/
