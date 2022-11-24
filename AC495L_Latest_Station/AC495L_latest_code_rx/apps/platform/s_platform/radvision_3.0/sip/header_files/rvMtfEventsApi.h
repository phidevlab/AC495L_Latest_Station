/******************************************************************************
Filename   : rvMtfEventsApi.h
Description: This file includes type definitions and APIs for sending events
			 to MTF.
******************************************************************************
                Copyright (c) 2008 RADVISION
************************************************************************
NOTICE:
This document contains information that is proprietary to RADVISION.
No part of this publication may be reproduced in any form whatsoever
without written prior approval by RADVISION.

RADVISION reserves the right to revise this publication and make
changes without obligation to notify any person of such revisions or
changes.
******************************************************************************/
#ifndef RV_RVMGRMGREVENTS_H
#define RV_RVMGRMGREVENTS_H

#include "rvIppCfwApi.h"
#include "rvMtfSignalsTypes.h"
#include "rvCallControlApi.h"


/* See RvMtfEvent defined in rvCallControlApi.c */

/*@*****************************************************************************
 * Type: RvMtfEventInfo (RvMtfSignalsEventsPkg)
 * -----------------------------------------------------------------------------
 * Description: This structure includes additional information about a user
 * event.
 ****************************************************************************@*/
typedef struct
{
    RvMtfDigit		digit;
        /* This parameter is relevant for the events RV_MTF_EVENT_DIGIT and
	       RV_MTF_EVENT_DIGIT_END only. */
    RvUint32		lineId;
        /* This parameter is relevant for the event RV_MTF_EVENT_LINE only. */
    RvInt			cfwNoReplyTimeout;
        /* This parameter is relevant for the event RV_MTF_EVENT_CFW with
	       cfwType = NO_REPLY only. */
	RvIppCfwType    cfwType;
        /* This parameter is relevant for the event RV_MTF_EVENT_CFW only. */
	RvBool			cfwActivate;
        /* This parameter is relevant for the event RV_MTF_EVENT_CFW only. */
} RvMtfEventInfo;

/*@*****************************************************************************
 * rvMtfSendKeyEvent (RvMtfSignalsEventsPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         Sends a key event to the MTF. The key event should be sent whenever
 *         the user presses a key in the terminal.
 *
 * Arguments:
 *
 * Input:  term    - Handle to the terminal on which the key was pressed.
 *         event   - Event to be sent to the MTF.
 *         info    - Parameters related to the event. Note that not all events
 *                   have parameters.
 *
 * Return Value: RV_OK upon success.
 ****************************************************************************@*/
RVAPI RvStatus RVCALLCONV rvMtfSendKeyEvent(
                                 IN RvIppTerminalHandle		term,
                                 IN RvMtfEvent			    event,
                                 IN RvMtfEventInfo*			info);

/*@*****************************************************************************
 * rvMtfSendUserEvent (RvMtfSignalsEventsPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         Sends a user event to be processed by the user application
 *		   when the callbacks preProcess and postProcess are called.
 *
 * Arguments:
 *
 * Input:  term         - Handle to the terminal of the call.
 *         userEvent    - User event. Should be bigger than RV_CCTERMEVENT_USER.
 *
 * Return Value: RV_OK upon success.
 ****************************************************************************@*/
RVAPI RvStatus RVCALLCONV rvMtfSendUserEvent(
                                  IN RvIppTerminalHandle   term,
                                  IN RvUint                userEvent);

/*@*****************************************************************************
 * rvMtfSendDialCompletedEvent (RvMtfSignalsEventsPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         Sends a dial completed event to the MTF. This event is relevant for 
 *		   user applications that use their own digitmap pattern for dialing 
 *		   phone numbers. The user application should send this event to notify the MTF 
 *		   that the user has completed dialing the phone number. After calling this 
 *		   function, the MTF will call the RvMtfMapDialStringToAddressEv() callback to
 *		   map the phone number to the destination SIP address.
 *
 * Arguments:
 *
 * Input:	term         - Handle to the terminal of the call.
 *			dialString   - The phone number dialed by the user.
 *			matchType	 - Matching type of the digitmap pattern. Possible values are:
 *							RV_MDMDIGITMAP_NOMATCH, 
 *							RV_MDMDIGITMAP_PARTIALMATCH, 
 *							RV_MDMDIGITMAP_FULLMATCH, 
 *							RV_MDMDIGITMAP_UNAMBIGUOUSMATCH.
 *
 * Return Value: RV_OK upon success.
 ****************************************************************************@*/
RVAPI RvStatus RVCALLCONV rvMtfSendDialCompletedEvent(
                                  IN RvIppTerminalHandle		term,
								  IN RvChar*					dialString,
                                  IN RvMdmDigitMapMatchType		matchType);


#endif /*RV_RVMGRMGREVENTS_H*/









