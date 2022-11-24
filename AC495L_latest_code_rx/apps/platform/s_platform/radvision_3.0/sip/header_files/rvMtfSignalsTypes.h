/******************************************************************************
Filename   : rvMtfSignalsTypes.h
Description: This file includes type definitions and APIs for processing MTF
			 signals callbacks.
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
#ifndef RV_RVMGRMGRSIGNALS_H
#define RV_RVMGRMGRSIGNALS_H

#include "ipp_inc_std.h"
#include "rvMtfHandles.h"

/*@*****************************************************************************
 * Package: RvMtfSignalsEventsPkg (root)
 * -----------------------------------------------------------------------------
 * Title: Signals and events
 *
 * General: This section contains functions and type definitions that are used 
 *  		for handling signals and for sending events to the MTF.
 ****************************************************************************@*/

/*@*****************************************************************************
* Enum: RvMtfDigit (RvMtfSignalsEventsPkg)
* -----------------------------------------------------------------------------
* Description: This enumerator indicates a specific digit.
*****************************************************************************@*/
typedef enum
{
    RV_MTF_DIGIT_0,
	RV_MTF_DIGIT_1,
	RV_MTF_DIGIT_2,
	RV_MTF_DIGIT_3,
	RV_MTF_DIGIT_4,
	RV_MTF_DIGIT_5,
	RV_MTF_DIGIT_6,
	RV_MTF_DIGIT_7,
	RV_MTF_DIGIT_8,
	RV_MTF_DIGIT_9,
	RV_MTF_DIGIT_STAR,
	RV_MTF_DIGIT_POUND,
	RV_MTF_DIGIT_NONE
} RvMtfDigit;

/*@*****************************************************************************
* Type: RvMtfSignalParams (RvMtfSignalsEventsPkg)
* -----------------------------------------------------------------------------
* Description: This structure includes signal parameters.
*****************************************************************************@*/
typedef struct{
    RvChar                distictiveTone[RV_MEDIUM_STR_SZ];
        /* A string that includes a distinctive tone received from the remote 
	   party. This parameter is relevant for the signals RV_MTF_SIGNAL_RINGBACK 
	   and RV_MTF_SIGNAL_RINGING only. */

    RvMtfDigit          digit;
        /* Indicates a specific digit. This parameter is relevant for the signal
	   type RV_MTF_SIGNAL_DIGIT only. */

    RvUint32            lineId;
        /* Indicates the line number. This parameter is relevant for the signal
	   type RV_MTF_SIGNAL_IND_LINE only */
} RvMtfSignalParams;

/*@*****************************************************************************
* Enum: RvMtfSignalParams (RvMtfSignalsEventsPkg)
* -----------------------------------------------------------------------------
* Description: This enumerator indicates signal states such as turned on,
* turned off, blinking, etc.
*****************************************************************************@*/
typedef enum {
    RV_MTF_SIGNAL_STATE_ON,
		/* Indicates that the signal should be turned on. */

    RV_MTF_SIGNAL_STATE_OFF,
		/* Indicates that the signal should be turned off. */

    RV_MTF_SIGNAL_STATE_BLINK,
        /* Indicates that the signal should blink. Relevant for
	   RV_MTF_SIGNAL_IND_XXX only. */

    RV_MTF_SIGNAL_STATE_FAST_BLINK
        /* Indicates that the signal should blink rapidly. Relevant for
	   RV_MTF_SIGNAL_IND_XXX only. */

} RvMtfSignalState;

/*@*****************************************************************************
* Enum: RvMtfSignalParams (RvMtfSignalsEventsPkg)
* -----------------------------------------------------------------------------
* Description: This enumerator indicates the signal type.
*****************************************************************************@*/
typedef enum {
    RV_MTF_SIGNAL_DIALTONE,
		/* Indicates a dial tone. */
    RV_MTF_SIGNAL_RINGBACK,
		/* Indicates a ringback tone. */
    RV_MTF_SIGNAL_RINGING,
		/* Indicates a ringing tone. */
    RV_MTF_SIGNAL_CALLWAITING_CALLER,
		/* Indicates a call waiting tone (at the caller). */
    RV_MTF_SIGNAL_CALLWAITING_CALLEE,
		/* Indicates a call waiting tone (at the callee). */
    RV_MTF_SIGNAL_BUSY,
		/* Indicates a busy tone. */
    RV_MTF_SIGNAL_WARNING,
		/* Indicates a warning tone. */
    RV_MTF_SIGNAL_DIGIT,
		/* Indicates a digit tone. */
    RV_MTF_SIGNAL_IND_LINE,
		/* Indicates a line indicator. */
    RV_MTF_SIGNAL_IND_HOLD,
		/* Indicates a Hold indicator. */
    RV_MTF_SIGNAL_IND_MUTE,
		/* Indicates a Mute indicator. */
    RV_MTF_SIGNAL_IND_HANDSFREE,
		/* Indicates a Speaker indicator. */
    RV_MTF_SIGNAL_IND_HEADSET
		/* Indicates a Headset indicator. */
} RvMtfSignalType;

/*@*****************************************************************************
*  RvMtfStartSignalEv (RvMtfSignalsEventsPkg)
* -----------------------------------------------------------------------------
* General:  The MTF calls this callback when the application applies a tone or
*           an indicator to update the terminal interface. For example: Start
*           playing a dial tone, turn on/off a Hold indicator, etc.
*
*			Note: Some signals are stopped by calling this callback and not by
*			calling RvMtfStopSignalEv (see signal state).
*
*  Arguments:
*  Input:          hTerm	    - Handle to the terminal.
*                  signalType   - Signal type.
*				   signalState	- Signal state.
*				   signalParams	- Signal parameters.
*
*  Return Value:    None.

****************************************************************@*/
typedef void (RVCALLCONV *RvMtfStartSignalEv)(
			   IN RvIppTerminalHandle     hTerm,
			   IN RvMtfTerminalAppHandle  hAppTerm,
			   IN RvMtfSignalType         signalType,
			   IN RvMtfSignalState        signalState,
			   IN RvMtfSignalParams*      signalParams);


/*@*****************************************************************************
*  RvMtfStopSignalEv (RvMtfSignalsEventsPkg)
* -----------------------------------------------------------------------------
*  General : The MTF calls this callback when the application should stop
*            playing a tone or indicator to update the terminal interface.
*		     For example: Stop playing a dial tone, etc.
*
*  Arguments:
*  Input:          hTerm	    - Handle to the terminal.
*                  signalType   - Signal type.
*				   signalState	- Signal state.
*				   signalParams	- Signal parameters.
*
*  Return Value:    None.
****************************************************************************@*/
typedef void (RVCALLCONV *RvMtfStopSignalEv)(
				IN RvIppTerminalHandle     hTerm,
			    IN RvMtfTerminalAppHandle  hAppTerm,
				IN RvMtfSignalType         signalType);

#endif /*RV_RVMGRMGRSIGNALS_H*/







