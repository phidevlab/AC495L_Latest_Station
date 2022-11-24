/******************************************************************************
Filename:    rvMdmControlApi.h
Description: MDM Control Internal Extension APIs
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

#include "rvCallControlApi.h"
#include "rvmdmmediastream.h"

/* RvIppProviderHandle
 * ------------------------------------------------------------------------
 * The provider handle in the MTF. This handle associates a group of
 * terminations (RvIppTerminalHandle) into a given MTF instance.
 */
RV_DECLARE_HANDLE(RvIppProviderHandle);

/* RvIppTerminalHandle
 * ------------------------------------------------------------------------
 * A termination handle in the MTF. This handle indicates a specific
 * terminal within the MTF.
 * When the MTF is used as an IAD, there may be several terminations/
 * terminals, each able to hold several different connections.
 * When the MTF is used as a client/terminal, there may be a single
 * terminal/termination, that may hold several different connections.
 *
 * Connections on such a termination are associated to
 * RvIppConnectionHandle.
 */
RV_DECLARE_HANDLE(RvIppTerminalHandle);

/* RvIppConnectionHandle
 * ------------------------------------------------------------------------
 * A connection handle in the MTF. This handle indicates a specific,
 * temporary connection/call within the MTF on a specific termination.
 *
 * Connections belong to specific terminations through
 * RvIppTerminalHandle.
 */
RV_DECLARE_HANDLE(RvIppConnectionHandle);



/**********************************************************************************
				E X T E N S I O N		A P I		D E F I N I T I O N S
**********************************************************************************/

/********************************
*
*  Mdm Pre/Post Process Events
*  ---------------------------
*
*  Code logic is as follows:
*
*
*      1. rvCCTermConnProcessEvents
*      2. rvIppMdmExtPreProcessEventCB
*      3. rvCCConnectionStateMachine
*      4. rvCCConnectionDisplay
*      5. rvIppMdmExtPostProcessEventCB
*
**********************************************/


/******************************************************************************
*  RvIppMdmPreProcessEventCB
*  ----------------------------
*  General :        Prototype for PreProcessEvent CallBack.
*
*  Return Value:    None.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          connHndl           connection handle.
*       .          eventId            Terminal event ID
*                  reason             pointer to event reason
*
*
*  Output          reason
*
******************************************************************************/
typedef RvCCTerminalEvent  (*RvIppMdmPreProcessEventCB)(
										IN		RvIppConnectionHandle	connHndl,
										IN		RvCCTerminalEvent 		eventId,
										INOUT	RvCCEventCause*			reason);

/******************************************************************************
*  RvIppMdmPostProcessEventCB
*  ----------------------------
*  General :        Prototype for PostProcessEvent CallBack.
*
*  Return Value:    None.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          connHndl           connection handle.
*                  eventId            Terminal event ID
*                  reason             event reason
*
*
*  Output          None.
*
******************************************************************************/
typedef void (*RvIppMdmPostProcessEventCB)(
										IN	RvIppConnectionHandle	connHndl,
										IN	RvCCTerminalEvent		eventId,
										IN	RvCCEventCause			reason);


/******************************************************************************
*  RvIppMdmConnectionCreatedCB
*  ----------------------------
*  General :        Prototype for Mdm Connection Created CallBack.
*
*  Return Value:    None.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          connHndl            handle of new connection.
*
*  Output          None.
*
******************************************************************************/
typedef void (*RvIppMdmConnectionCreatedCB)(
    IN RvIppConnectionHandle    connHndl);

/******************************************************************************
*  RvIppMdmConnectionDestructedCB
*  ----------------------------
*  General :        Prototype for Mdm Connection Destructed CallBack.
*
*  Return Value:    None.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          connHndl            handle of new connection.
*
*  Output          None.
*
******************************************************************************/
typedef void (*RvIppMdmConnectionDestructedCB)(
    IN RvIppConnectionHandle    connHndl);

/******************************************************************************
*  RvIppMdmMapUserEventCB
*  ----------------------------
*  General :        Prototype for MapUserEvent CallBack.
*                   Enables user application to use its own events.
*
*  Return Value:    RvCCTerminalEvent - event mapped by user application.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          pkg            package of the event
*                   id            id of the event
*                  args           arguments
*                  key            line id - relevant for Line and Reject events only
*  Output          None.
*
******************************************************************************/
typedef RvCCTerminalEvent (*RvIppMdmMapUserEventCB)(
    IN const RvChar*        pkg,
    IN const RvChar*        id,
    IN RvMdmParameterList*  args,
    IN RvChar*              key);

/******************************************************************************
*  RvIppMdmDisplayCB
*  ----------------------------
*  General :        Prototype for Display CallBack.
*                   Enables user application to control the display.
*
*  Return Value:    None.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          connHndl			connection Handle
*                  terminalHndl     terminal   Handle
*                  event            terminal event
*                  cause            event cause
*
*  Output          displayData      pointer to user defined structure used in order to display.
*
******************************************************************************/
typedef void (*RvIppMdmDisplayCB)(
    IN RvIppConnectionHandle    connHndl,
    IN RvIppTerminalHandle      terminalHndl,
    IN RvCCTerminalEvent        event,
    IN RvCCEventCause           cause,
    IN void*                    displayData);

/**********************************************************************************
				E X T E N S I O N		A P I		S T R U C T U R E
**********************************************************************************/

/* RvIppMdmExtClbks
 * ------------------------------------------------------------------------
 * MDM extension callbacks struct.
 * These callbacks can be used by the application to support proprietary
 * behavior or to add additional functionality.
 */
typedef struct
{
    RvIppMdmDisplayCB                   display;
        /* Enables the application to control the display */

	void*						    	displayData;
	    /* Application data */

    RvIppMdmMapUserEventCB              mapUserEvent;
        /* Enables the application to handle its own events */

    RvIppMdmPreProcessEventCB           preProcessEvent;
        /* Enables the application to intervene prior to the processing
           of events by the MTF */

    RvIppMdmPostProcessEventCB          postProcessEvent;
        /* Enables the application to act after an event has been
           processed by the MTF */

    RvIppMdmConnectionCreatedCB         connectionCreated;
        /* Notifies the application on newly created MDM connections */

    RvIppMdmConnectionDestructedCB      connectionDestructed;
        /* Notifies the application on destructed MDM connections */
} RvIppMdmExtClbks;


/******************************************************************************
*  rvIppMdmRegisterExtClbks
*  ----------------------------------------------------------------------------
 * General:
 *  This function is used to register the MDM Control's extension callbacks.
 *  Most of the callbacks have an additional userData parameter, which will get
 *  the value indicated in the clbks struct.
*  Arguments:
 * Input:  clbks    - The callbacks and userData to register.
 * Output: None.
*
 * Return Value: None.
 *****************************************************************************/
RVAPI void RVCALLCONV rvIppMdmRegisterExtClbks(IN RvIppMdmExtClbks* clbks);



/**********************************************************************************
					P R O V I D E R			A P I s
**********************************************************************************/

/******************************************************************************
*  rvIppMdmProviderGetDialToneDuration
*  ----------------------------
*  General :        Retrieves Dial-Tone Duration configured value.
*
*
*  Return Value:    number of seconds to play Dial-Tone.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          providerHndl		provider Handle
*
*
*  Output          none.
*
******************************************************************************/
RVAPI RvUint32 RVCALLCONV rvIppMdmProviderGetDialToneDuration(
    IN RvIppProviderHandle providerHndl);


/******************************************************************************
*  rvIppMdmProviderGetDisplayData
*  ----------------------------
*  General :        Retrieves the display data.
*
*
*  Return Value:    pointer to display Data data structure.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          providerHndl     provider Handle
*
*
*  Output          none.
*
******************************************************************************/
RVAPI void* RVCALLCONV rvIppMdmProviderGetDisplayData(
    IN RvIppProviderHandle providerHndl);

/******************************************************************************
*  rvIppMdmProviderFindTerminalByTermId
*  ----------------------------
*  General :        Retrieves the terminal handle based on terminal-id
*
*
*  Return Value:    terminal handle
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           providerHndl        provider Handle
*                   termId              terminal ID
*
*  Output          none.
*
******************************************************************************/
RVAPI RvIppTerminalHandle RVCALLCONV rvIppMdmProviderFindTerminalByTermId(
    IN RvIppProviderHandle  providerHndl,
    IN const RvChar*        termId);

/******************************************************************************
*  rvIppMdmProviderFindTerminalByAddress
*  ----------------------------
*  General :        Retrieves the terminal handle based on IP address
*
*
*  Return Value:    terminal handle
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           providerHndl        provider Handle
*                   address             IP address
*
*  Output          none.
*
******************************************************************************/
RVAPI RvIppTerminalHandle RVCALLCONV rvIppMdmProviderFindTerminalByAddress(
    IN RvIppProviderHandle  providerHndl,
    IN const RvChar*        address);


/******************************************************************************
*  rvIppMdmProviderFindAnyTerminal
*  ----------------------------
*  General :        Retrieves the first terminal handle which exists under this provider.
*
*
*  Return Value:    terminal handle
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           providerHndl        provider Handle
*
*
*  Output          none.
*
******************************************************************************/
RVAPI RvIppTerminalHandle RVCALLCONV rvIppMdmProviderFindAnyTerminal(
    IN RvIppProviderHandle providerHndl);

/******************************************************************************
*  rvIppMdmProviderFindTerminalByNumber
*  ----------------------------
*  General :        Retrieves the first terminal handle based on E.164 number
*
*
*  Return Value:    terminal handle
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           providerHndl        provider Handle
*
*
*  Output          none.
*
******************************************************************************/
RVAPI RvIppTerminalHandle RVCALLCONV rvIppMdmProviderFindTerminalByNumber(
    IN RvIppProviderHandle  providerHndl,
    IN const RvChar*        phoneNumber);


/**********************************************************************************
					T E R M I N A L		A P I s
**********************************************************************************/

/******************************************************************************
*  rvIppMdmTerminalGetProvider
*  ----------------------------
*  General :        Retrieves the provider which this terminal belongs to.
*
*
*  Return Value:    provider handle
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*
*
*  Output          none.
*
****************************************************************************/
RVAPI RvIppProviderHandle RVCALLCONV rvIppMdmTerminalGetProvider(
    IN RvIppTerminalHandle terminalHndl);

/******************************************************************************
*  rvIppMdmTerminalGetId
*  ----------------------------
*  General :        Retrieves the terminal ID.
*
*
*  Return Value:    RvTrue if managed to retrieve the value, rvFalse otherwise.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*                   termId              place holder for the terminalID
*                   termIdLen           length of placeholder.
*  Output          none.
*
****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmTerminalGetId(
    IN    RvIppTerminalHandle   terminalHndl,
    INOUT RvChar*               termId,
    IN    RvSize_t              termIdLen);


/******************************************************************************
*  rvIppMdmTerminalGetType
*  ----------------------------
*  General :        Retrieves the terminal type based on terminalHandle..
*
*
*  Return Value:    Terminal Type.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*
*  Output          none.
*
****************************************************************************/
RVAPI RvCCTerminalType RVCALLCONV rvIppMdmTerminalGetType(
    IN RvIppTerminalHandle terminalHndl);

/******************************************************************************
*  rvIppMdmTerminalGetLastEvent
*  ----------------------------
*  General :        Retrieves the last MDM event received for this terminal.
*
*
*  Return Value:    Terminal Type.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*                   event               pointer to Mdm Event structure where
*                                       last MDM event received will be stored
*
*  Output:
*
****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmTerminalGetLastEvent(
    IN    RvIppTerminalHandle   terminalHndl,
    INOUT RvMdmEvent*           event);

/******************************************************************************
*  rvIppMdmTerminalGetMediaCaps
*  ----------------------------
*  General :        Retrieves the the terminal's media Capabilities.
*
*
*  Return Value:    rvTrue if succeeded in retrieval, rvFalse otherwise.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*                   mediaCaps           pointer to mediaCapability structure where
*                                       media capabilities will be stored.
*
*  Output:
*
****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmTerminalGetMediaCaps(
    IN    RvIppTerminalHandle   terminalHndl,
    INOUT RvSdpMsg*             mediaCaps);

/******************************************************************************
*  rvIppMdmTerminalGetMediaStream
*  ----------------------------
*  General :        Retrieves a specific media Stream.
*
*
*  Return Value:    rvTrue if succeeded in retrieval, rvFalse otherwise.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*                   mediaStreamId       id of media stream
*                   mediaStream         pointer to RvMdmMediaStreamInfo structure where
*                                       media stream will be stored.
*
*  Output:
*
****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmTerminalGetMediaStream(
    IN    RvIppTerminalHandle       terminalHndl,
    IN    RvUint32                  mediaStreamId,
    INOUT RvMdmMediaStreamInfo*     mediaStream);

/******************************************************************************
*  rvIppMdmTerminalIsFirstDigit
*  ----------------------------
*  General :        Finds if there is 1 digit in the current dial string
*
*
*  Return Value:    rvTrue if there is one digit. rvFalse otherwise
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*
*
*  Output:          None.
*
****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmTerminalIsFirstDigit(IN RvIppTerminalHandle terminalHndl);

/******************************************************************************
*  rvIppMdmTerminalGetLastDigit
*  ----------------------------
*  General :        Retrieves last digit of current dial string
*
*
*  Return Value:    rvTrue if successful. rvFalse otherwise
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*
*
*  Output:          None.
*
****************************************************************************/
RVAPI RvChar RVCALLCONV rvIppMdmTerminalGetLastDigit(RvIppTerminalHandle terminalHndl);

/******************************************************************************
*  rvIppMdmTerminalGetPhoneNumber
*  ----------------------------
*  General :        Retrieves the terminal's phone number
*
*
*  Return Value:    rvTrue if successful. rvFalse otherwise
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*                   phoneNumber         place holder for the phone number
*                   phoneNumberLen      length of placeholder.
*
*  Output:          None.
*
****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmTerminalGetPhoneNumber(
    IN    RvIppTerminalHandle   terminalHndl,
    INOUT RvChar*               phoneNumber,
    IN    RvSize_t              phoneNumberLen);

/******************************************************************************
*  rvIppMdmTerminalGetActiveAudioType
*  ----------------------------
*  General :        Retrieves the type of the active Audio Termination
*
*
*  Return Value:    Audio Type -
*                           RV_CCTERMAUDIO_NONE,
*                           RV_CCTERMAUDIO_HANDSET,
*                           RV_CCTERMAUDIO_HANDSFREE,
*                           RV_CCTERMAUDIO_HEADSET
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*
*
*  Output:          None.
*
****************************************************************************/
RVAPI RvCCTerminalAudioType RVCALLCONV rvIppMdmTerminalGetActiveAudioType(
    IN RvIppTerminalHandle terminalHndl);

/******************************************************************************
*  rvIppMdmTerminalGetActiveAudioTerm
*  ----------------------------
*  General :        Retrieves the handle of the active Audio Termination
*
*
*  Return Value:    active termination Handle
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        current terminal Handle
*
*
*  Output:          None.
*
****************************************************************************/
RVAPI RvIppTerminalHandle RVCALLCONV rvIppMdmTerminalGetActiveAudioTerm(
    IN RvIppTerminalHandle terminalHndl);

/******************************************************************************
*  rvIppMdmTerminalStopSignals
*  ----------------------------
*  General :        Stops playing signals on this terminal
*
*
*  Return Value:    None.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*
*
*  Output:          None.
*
****************************************************************************/
RVAPI void RVCALLCONV rvIppMdmTerminalStopSignals(IN RvIppTerminalHandle terminalHndl);

/******************************************************************************
*  rvIppMdmTerminalStartUserSignalUI
*  ----------------------------
*  General :        start playing user signal on UI terminal.
*
*
*  Return Value:    rvTrue if successfully. rvFalse otherwise
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*                   pkg - package name
*                   id - event id
*                   params - parameters list
*  Output:          None.
*
****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmTerminalStartUserSignalUI(
    IN RvIppTerminalHandle  terminalHndl,
    IN const RvChar*        pkg,
    IN const RvChar*        id,
    IN RvMdmParameterList*  params);

/******************************************************************************
*  rvIppMdmTerminalStartUserSignalAT
*  ----------------------------
*  General :        start playing user signal on Audio terminal.
*
*
*  Return Value:    rvTrue if successful. rvFalse otherwise
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*                   pkg - package name
*                   id - event id
*                   params - parameters list
*
*  Output:          None.
*
****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmTerminalStartUserSignalAT(
    IN RvIppTerminalHandle  terminalHndl,
    IN const RvChar*        pkg,
    IN const RvChar*        id,
    IN RvMdmParameterList*  params);



/******************************************************************************
*  rvIppMdmTerminalStartDialToneSignal
*  ----------------------------
*  General :        start playing DialTone on this terminal.
*                   Note that signal will be stopped according to dialTone timer value
*
*  Return Value:    rvTrue if successful. rvFalse otherwise
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*
*
*  Output:          None.
*
****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmTerminalStartDialToneSignal(
    IN RvIppTerminalHandle terminalHndl);

/******************************************************************************
*  rvIppMdmTerminalStartRingingSignal
*  ----------------------------
*  General :        Start ringing on this terminal.
*
*
*  Return Value:    rvTrue if successful. rvFalse otherwise
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*
*
*  Output:          None.
*
****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmTerminalStartRingingSignal(
    IN RvIppTerminalHandle terminalHndl);

/******************************************************************************
*  rvIppMdmTerminalStopRingingSignal
*  ----------------------------
*  General :        Stop ringing on this terminal.
*
*
* *  Return Value:    rvTrue if successful. rvFalse otherwise
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*
*
*  Output:          None.
*
****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmTerminalStopRingingSignal(
    IN RvIppTerminalHandle terminalHndl);

/******************************************************************************
*  rvIppMdmTerminalStartRingbackSignal
*  ----------------------------
*  General :        Start  RingBack Tone on this terminal.
*
*
*  Return Value:    rvTrue if successful. rvFalse otherwise
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*
*
*  Output:          None.
*
****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmTerminalStartRingbackSignal(
    IN RvIppTerminalHandle terminalHndl);

/******************************************************************************
*  rvIppMdmTerminalStartCallWaitingSignal
*  ----------------------------
*  General :        Start  CallWaiting Tone  on this terminal.
*
*
*  Return Value:    rvTrue if successful. rvFalse otherwise
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*
*
*  Output:          None.
*
****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmTerminalStartCallWaitingSignal(
    IN RvIppTerminalHandle terminalHndl);

/******************************************************************************
*  rvIppMdmTerminalStartCallWaitingCallerSignal
*  ----------------------------
*  General :        Start  Caller CallWaiting Tone  on this terminal.
*
*  Return Value:    rvTrue if successful. rvFalse otherwise
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*
*
*  Output:          None.
*
****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmTerminalStartCallWaitingCallerSignal(
    IN RvIppTerminalHandle terminalHndl);

/******************************************************************************
*  rvIppMdmTerminalStartBusySignal
*  ----------------------------
*  General :        Start  Busy Tone  on this terminal.
*
*
*  Return Value:    rvTrue if successful. rvFalse otherwise
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*
*
*  Output:          None.
*
****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmTerminalStartBusySignal(
    IN RvIppTerminalHandle terminalHndl);

/******************************************************************************
*  rvIppMdmTerminalStartWarningSignal
*  ----------------------------
*  General :        Start  Warning Tone  on this terminal.
*
*
*  Return Value:    rvTrue if successful. rvFalse otherwise
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*
*
*  Output:          None.
*
****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmTerminalStartWarningSignal(
    IN RvIppTerminalHandle terminalHndl);

/******************************************************************************
*  rvIppMdmTerminalStartDTMFTone
*  ----------------------------
*  General :        Start  DTMF Tone  on this terminal.
*
*
*  Return Value:    rvTrue if successful. rvFalse otherwise
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*                   digit               digit to send
*
*  Output:          None.
*
****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmTerminalStartDTMFTone(
    IN RvIppTerminalHandle  terminalHndl,
    IN RvChar               digit);

/******************************************************************************
*  rvIppMdmTerminalStopDTMFTone
*  ----------------------------
*  General :        Stop  DTMF Tone  on this terminal.
*
*
*  Return Value:    rvTrue if successful. rvFalse otherwise
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*                   digit               digit to send
*
*  Output:          None.
*
****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmTerminalStopDTMFTone(
    IN RvIppTerminalHandle  terminalHndl,
    IN RvChar               digit);

/******************************************************************************
*  rvIppMdmTerminalSetHoldInd
*  ----------------------------
*  General :        Set or Clear the Hold Indicator
*
*
*  Return Value:    rvTrue if successful. rvFalse otherwise
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*                   on                  if on the indicator is lir/
*
*  Output:          None.
*
****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmTerminalSetHoldInd(
    IN RvIppTerminalHandle  terminalHndl,
    IN RvBool               on);

/******************************************************************************
*  rvIppMdmTerminalSetLineInd
*  ----------------------------
*  General :        Set or Clear the Line Indicator
*
*
*  Return Value:    rvTrue if successful. rvFalse otherwise
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*                   lineId              ID of the line
*                   state               indicator state:
*                                       RV_INDSTATE_ON,   RV_INDSTATE_OFF,
*                                       RV_INDSTATE_BLINK,RV_INDSTATE_FAST_BLINK,
*                                       RV_INDSTATE_SLOW_BLINK
*
*  Output:          None.
*
****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmTerminalSetLineInd(
    IN RvIppTerminalHandle      terminalHndl,
    IN RvInt32                  lineId,
	IN RvCCTerminalIndState		state);

/******************************************************************************
*  rvIppMdmTerminalSendLineActive
*  ----------------------------
*  General :        Set or Clears the Line Indicator
*
*
*  Return Value:    rvTrue if successful. rvFalse otherwise
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*                   lineId              ID of the line
*                   active              if True line becomes active
*
*  Output:          None.
*
****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmTerminalSendLineActive(
    IN RvIppTerminalHandle  terminalHndl,
    IN RvInt32              lineId,
    IN RvBool               active);

/******************************************************************************
*  rvIppMdmTerminalSetDisplay
*  ----------------------------
*  General :        Places text  to the terminal's display
*
*
*  Return Value:    rvTrue if successful. rvFalse otherwise
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          	terminalHndl		terminal Handle
*                   text				text to display
*					row					location
*                   column
*
*  Output:          None.
*
****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmTerminalSetDisplay(
    IN RvIppTerminalHandle  terminalHndl,
    IN const RvChar*        text,
    IN RvInt32              row,
    IN RvInt32              column);

/******************************************************************************
*  rvIppMdmTerminalClearDisplay
*  ----------------------------
*  General :        Clears the terminal display
*
*
*  Return Value:    rvTrue if successful. rvFalse otherwise
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*                   text                text to display
*                   row                 location
*                   column
*
*  Output:          None.
*
****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmTerminalClearDisplay(
    IN RvIppTerminalHandle terminalHndl);


/******************************************************************************
*  rvIppMdmTerminalSendCallerId
*  ----------------------------
*  General :        updates the caller ID
*
*
*  Return Value:    rvTrue if successful. rvFalse otherwise
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*                   callerName          string representation of caller name
*                   callerNumber        E164 number
*                   address             ip address
*                   callerId            display name
*  Output:          None.
*
****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmTerminalSendCallerId(
    IN RvIppTerminalHandle  terminalHndl,
    IN const RvChar*        callerName,
    IN const RvChar*        callerNumber,
    IN const RvChar*        address,
    IN const RvChar*        callerId);

/******************************************************************************
*  rvIppMdmTerminalSetWaitForDigits
*  ----------------------------
*  General :        Instructs Terminal to start collecting digits
*
*
*  Return Value:    rvTrue if successful. rvFalse otherwise
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*
*
*  Output:          None.
*
****************************************************************************/
RVAPI void RVCALLCONV rvIppMdmTerminalSetWaitForDigits(
    IN RvIppTerminalHandle terminalHndl);

/******************************************************************************
*  rvIppMdmTerminalGetDialString
*  ----------------------------
*  General :        Retrieves current dial string
*
*
*  Return Value:    rvTrue if successful. rvFalse otherwise
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*                   dialString          place holder for the dial string
*                   dialStringLen       length of dialString
*
*  Output:          None.
*
****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmTerminalGetDialString(
    IN    RvIppTerminalHandle   terminalHndl,
    INOUT RvChar*               dialString,
    IN    RvSize_t              dialStringLen);

/******************************************************************************
*  rvIppMdmTerminalResetDialString
*  ----------------------------
*  General :        clears the  current dial string
*
*
*  Return Value:    None.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*
*  Output:          None.
*
****************************************************************************/
RVAPI void RVCALLCONV rvIppMdmTerminalResetDialString(IN RvIppTerminalHandle terminalHndl);

/******************************************************************************
*  rvIppMdmTerminalIsOutOfBandDtmfEnabled
*  ----------------------------
*  General :        Finds whether OOB DTMF is enabled
*
*
*  Return Value:    rvTrue  - OOB DTMF enabled, rvFalse - Disabled
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*
*  Output:          None.
*
****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmTerminalIsOutOfBandDtmfEnabled(
    IN RvIppTerminalHandle terminalHndl);

/******************************************************************************
*  rvIppMdmTerminalIsDtmfPlayEnabled
*  ----------------------------
*  General :        Finds whether we support DTMF play on this terminal.
*
*
*  Return Value:    rvTrue  - OOB DTMF enabled, rvFalse - Disabled
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*
*  Output:          None.
*
****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmTerminalIsDtmfPlayEnabled(
    IN RvIppTerminalHandle terminalHndl);

/******************************************************************************
*  rvIppMdmTerminalGetMaxConnections
*  ----------------------------
*  General :        Get the maximum number of connections on the terminal
*
*
*  Return Value:    number of connections.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*
*  Output:          None.
*
****************************************************************************/
RVAPI RvInt32 RVCALLCONV rvIppMdmTerminalGetMaxConnections(
    IN RvIppTerminalHandle terminalHndl);

/******************************************************************************
*  rvIppMdmTerminalGetNumActiveConnections
*  ----------------------------
*  General :        Get the  number of active connections on the terminal
*
*
*  Return Value:    number of connections.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*
*  Output:          None.
*
****************************************************************************/
RVAPI RvInt32 RVCALLCONV rvIppMdmTerminalGetNumActiveConnections(
    IN RvIppTerminalHandle terminalHndl);

/******************************************************************************
*  rvIppMdmTerminalGetActiveConnection
*  ----------------------------
*  General :        Get the  active connection
*
*
*  Return Value:    connection handle.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*
*  Output:          None.
*
****************************************************************************/
RVAPI RvIppConnectionHandle RVCALLCONV rvIppMdmTerminalGetActiveConnection(
    IN RvIppTerminalHandle terminalHndl);

/******************************************************************************
*  rvIppMdmTerminalGetCurDisplayRow
*  ----------------------------
*  General :        Get the  current location of cursor on the display
*
*
*  Return Value:    location in rows.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*
*  Output:          None.
*
****************************************************************************/
RVAPI RvInt32 RVCALLCONV rvIppMdmTerminalGetCurDisplayRow(
    IN RvIppTerminalHandle terminalHndl);

/******************************************************************************
*  rvIppMdmTerminalGetCurDisplayColumn
*  ----------------------------
*  General :        Get the  current location of cursor on the display
*
*
*  Return Value:    location in columns.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*
*  Output:          None.
*
****************************************************************************/
RVAPI RvInt32 RVCALLCONV rvIppMdmTerminalGetCurDisplayColumn(
    IN RvIppTerminalHandle terminalHndl);

/******************************************************************************
*  rvIppMdmTerminalOtherHeldConnExist
*  ----------------------------
*  General :        Finds whether there is another held connection on this terminal.
*
*
*  Return Value:    rvTrue if successful. rvFalse otherwise
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl        terminal Handle
*                   currConnHndl        current Connection handle
*  Output:          None.
*
****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmTerminalOtherHeldConnExist(
    IN RvIppTerminalHandle     terminalHndl,
    IN RvIppConnectionHandle   currConnHndl);


/******************************************************************************
*  rvIppMdmTerminalGetHeldConn
*  ----------------------------
*  General :        Get the held connection of this terminal.
*
*  Return Value:    held connection
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl -  terminal Handle
*  Output:          LineId       -  line Id of held connection.
****************************************************************************/
RVAPI RvIppConnectionHandle rvIppMdmTerminalGetHeldConn(
    IN  RvIppTerminalHandle terminalHndl,
    OUT RvInt32*            lineId);




/******************************************************************************
*  rvIppMdmTerminalSetState
*  ----------------------------
*  General :        Set terminal state.
*
*  Return Value:    None
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl -  terminal Handle
*                   State       -  terminal state.
*  Output:          None
****************************************************************************/
RVAPI void RVCALLCONV rvIppMdmTerminalSetState(
    IN RvIppTerminalHandle  terminalHndl,
    IN RvCCTerminalState    state);




/******************************************************************************
*  rvIppMdmTerminalGetState
*  ----------------------------
*  General :        Get held connection on this terminal.
*
*  Return Value:    terminal state
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl -  terminal Handle
*  Output:          None
****************************************************************************/
RVAPI RvCCTerminalState RVCALLCONV rvIppMdmTerminalGetState(
    IN RvIppTerminalHandle terminalHndl);

/******************************************************************************
*  rvIppMdmTerminalGetMdmTerm
*  ----------------------------
*  General :        Get Mdm term object.
*
*  Return Value:    Mdm term object
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl -  terminal Handle
*  Output:          None
****************************************************************************/
RVAPI RvMdmTerm* RVCALLCONV rvIppMdmTerminalGetMdmTerm(
    IN RvIppTerminalHandle terminalHndl);

/******************************************************************************
 * rvIppMdmTerminalGetHandle
 * ----------------------------------------------------------------------------
 * General:
 *  Get the terminal's handle from the MDM termination.
 * Arguments:
 * Input:  mdmTerm  - MDM termination handle.
 * Output: None.
 *
 * Return Value: Returns a pointer to the terminal's handle, or NULL if it fails.
 *****************************************************************************/
RVAPI RvIppTerminalHandle RVCALLCONV rvIppMdmTerminalGetHandle(
    IN RvMdmTerm* mdmTerm);

/******************************************************************************
 * rvMtfTerminalMakeCall
 * ----------------------------------------------------------------------------
 * General:
 *  Initiate an outgoing call to a destination address on the specified line and
 *  termination. This function skips the part of collecting digits and then
 *  translating them into an address.
 * Arguments:
 * Input:  terminalHndl - Terminal handle to dial on.
 *         address      - Destination address to dial to.
 * Output: None.
 *
 * Return Value: RV_OK on success, other on failure.
 *****************************************************************************/
RVAPI RvStatus RVCALLCONV rvMtfTerminalMakeCall(
    IN RvIppTerminalHandle  terminalHndl,
    IN const RvChar*        address);


/**********************************************************************************
                    C O N N E C T I O N     A P I s
**********************************************************************************/

/******************************************************************************
*  rvIppMdmConnGetTerminal
*  ----------------------------
*  General :   Gets a handle to the Terminal object associated with the Connection.
*
*  Return Value:  Returns a handle to the Terminal object associated with the Connection.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*
*  Input:        connHndl       -   A handle to the Connection
*
*  Output:       none.
*
****************************************************************************/
RVAPI RvIppTerminalHandle RVCALLCONV rvIppMdmConnGetTerminal(
    IN RvIppConnectionHandle connHndl);

/******************************************************************************
*  rvIppMdmConnGetLineId
*  ----------------------------
*  General :   Gets the ID of the line on which the call was created..
*
*  Return Value:  Returns the ID of the line on which the call was created.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*
*  Input:        connHndl       -   A handle to the Connection
*
*  Output:       none.
*
****************************************************************************/
RVAPI RvInt32 RVCALLCONV rvIppMdmConnGetLineId(
    IN RvIppConnectionHandle connHndl);

/******************************************************************************
*  rvIppMdmConnGetState
*  ----------------------------
*  General :   Returns the Connection state.
*
*  Return Value:  Returns the State of the Connection.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*
*  Input:        connHndl       -   A handle to the Connection
*
*  Output:       none.
*
****************************************************************************/
RVAPI RvCCConnState RVCALLCONV rvIppMdmConnGetState(
    IN RvIppConnectionHandle connHndl);

/******************************************************************************
*  rvIppMdmConnGetTermState
*  ----------------------------
*  General :   Gets the Termination state of the Connection.
*
*  Return Value:  Returns the Termination state of the Connection.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*
*  Input:        connHndl       -   A handle to the Connection
*
*  Output:       none.
*
****************************************************************************/
RVAPI RvCCTermConnState RVCALLCONV rvIppMdmConnGetTermState(
    IN RvIppConnectionHandle connHndl);

/******************************************************************************
*  rvIppMdmConnGetMediaState
*  ----------------------------
*  General :   Gets the media state of the Connection.
*
*  Return Value:  Returns the media state of the Connection.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*
*  Input:        connHndl       -   A handle to the Connection
*
*  Output:       none.
*
****************************************************************************/
RVAPI RvCCMediaState RVCALLCONV rvIppMdmConnGetMediaState(
    IN RvIppConnectionHandle connHndl);

/******************************************************************************
*  rvIppMdmConnGetType
*  ----------------------------
*  General :   Gets the Connection type
*
*  Return Value:  Returns the Connection type (MDM or network).
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:        connHndl       -   A handle to the Connection
*
*
*  Output:       none.
*
****************************************************************************/
RVAPI RvCCConnType RVCALLCONV rvIppMdmConnGetType(
    IN RvIppConnectionHandle connHndl);

/******************************************************************************
*  rvIppMdmConnGetConnectParty
*  ----------------------------
*  General :   Gets the other party connected to this Connection.
*
*  Return Value:   Returns the Connection handle connected to this Connection..
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:        connHndl       -   A handle to the Connection
*
*
*  Output:       none.
*
****************************************************************************/
RVAPI RvIppConnectionHandle RVCALLCONV rvIppMdmConnGetConnectParty(
    IN RvIppConnectionHandle connHndl);

/******************************************************************************
*  rvIppMdmConnGetLocalMedia
*  ----------------------------
*  General :   Gets the media descriptor, which includes the parameters of the
*             local media.
*
*  Return Value:   True if local media is not empty. False if it is empty.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:        connHndl       -   A handle to the Connection
*                localMedia     -   A pointer to the media descriptor filled
*                                   with the parameters of the local media.
*
*  Output:       none.
*
****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmConnGetLocalMedia(
    IN    RvIppConnectionHandle     connHndl,
    INOUT RvSdpMsg*                 localMedia);

/******************************************************************************
*  rvIppMdmConnGetMediaCaps
*  ----------------------------
*  General :   Gets the capabilities loaded by the user application during
*              initialization..
*
*  Return Value:   True if media capabilities is not empty. False if it is empty.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:        connHndl       -   A handle to the Connection
*                mediaCaps      -   A pointer to the media descriptor that includes
*                                   all media capabilities..
*
*  Output:       none.
*
****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmConnGetMediaCaps(
    IN    RvIppConnectionHandle connHndl,
    INOUT RvSdpMsg*             mediaCaps);

/******************************************************************************
*  rvIppMdmConnSetUserData
*  ----------------------------
*  General :   Sets the user data in the Connection object.
*
*  Return Value:   Returns a pointer to the user data in the Connection object.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:        connHndl       -   A handle to the Connection
*                userData       -   a pointer to the user data to be stored.
*
*  Output:       none.
*
****************************************************************************/
RVAPI void RVCALLCONV rvIppMdmConnSetUserData(
    IN RvIppConnectionHandle    connHndl,
    IN void*                    userData);

/******************************************************************************
*  rvIppMdmConnGetUserData
*  ----------------------------
*  General :   Gets the user data in the Connection object.
*
*  Return Value:   Returns a pointer to the user data in the Connection object.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:        connHndl         - A handle to the Connection
*
*  Output:       none.
*
****************************************************************************/
RVAPI void* RVCALLCONV rvIppMdmConnGetUserData(
    IN RvIppConnectionHandle connHndl);

/******************************************************************************
*  rvIppMdmConnGetCallerName
*  ----------------------------
*  General :   Gets the caller name as received from the remote party
*              in an incoming call. Will be empty in an outgoing call.
*
*  Return Value:    True if callerName is not empty. False if it is empty.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           connHndl         -  A handle to the Connection
*                   callerNameLen    -  Length of the caller number.
*
*  Output:          callerName     -  A pointer to a string containing the caller name..
*
****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmConnGetCallerName(
    IN    RvIppConnectionHandle     connHndl,
    INOUT RvChar*                   callerName,
    IN    RvSize_t                  callerNameLen);

/******************************************************************************
*  rvIppMdmConnGetCallerNumber
*  ----------------------------
*  General :   Gets the caller number as received from the remote party
*           in an incoming call. Will be empty in outgoing calls.
*
*  Return Value:    True if callerNumber is not empty. False if it is empty.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           connHndl         -  A handle to the Connection
*                   callerNumberLen -  Length of the caller number.
*
*  Output:          callerNumber,   -  caller address.
*
*  Note: If more than 1 number exists, the function retrieves the first
*  E.164 number
****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmConnGetCallerNumber(
    IN    RvIppConnectionHandle connHndl,
    INOUT RvChar*               callerNumber,
    IN    RvSize_t              callerNumberLen);

/******************************************************************************
*  rvIppMdmConnGetCallerNumberByIndex
*  ----------------------------
*  General :   Gets the caller number as received from the remote party
*           in an incoming call. Will be empty in outgoing calls.
*
*  Return Value:    True if callerNumber is not empty. False if it is empty.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           connHndl        -   A handle to the Connection
*                   callerNumberLen -  Length of the caller number.
*                   index           -  index of number to retrieve
*  Output:          callerNumber,   -  caller address.
*
*  Note: If more than 1 number exists, the function retrieves the first
*  E.164 number
****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmConnGetCallerNumberByIndex(
    IN    RvIppConnectionHandle connHndl,
    INOUT RvChar*               callerNumber,
    IN    RvSize_t              callerNumberLen,
    IN    RvSize_t              index);


/******************************************************************************
*  rvIppMdmConnGetCallerAddress
*  ----------------------------
*  General :   Gets the caller address as received from the remote party
*           in an incoming call. Will be empty in outgoing calls.
*
*  Return Value:    True if callerAddress is not empty. False if it is empty.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           connHndl         -  A handle to the Connection
*                   callerAddressLen -  Length of the caller address.
*
*  Output:          callerAddress,   -  caller address.
*
****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmConnGetCallerAddress(
    IN    RvIppConnectionHandle connHndl,
    INOUT RvChar*               callerAddress,
    IN    RvSize_t              callerAddressLen);

/******************************************************************************
 * rvIppMdmConnGetCallerId
 * ----------------------------------------------------------------------------
 * General:
 *  Gets the caller ID as received from the remote party in an incoming call.
 *  This will be empty for outgoing calls and will return RV_FALSE.
 * Arguments:
 * Input:  connHndl     - A handle to the connection.
 *         callerId     - A pointer to the string that will hold the caller's ID.
 *         callerIdLen  - Length of the callerId parameter in bytes.
 * Output: callerId     - The caller's ID will be copied to this buffer if this
 *                        function returns RV_TRUE.
 *
 * Return Value: RV_TRUE if callerId is not empty, RV_FALSE if it is empty.
 *****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmConnGetCallerId(
    IN    RvIppConnectionHandle     connHndl,
    INOUT RvChar*                   callerId,
    IN    RvSize_t                  callerIdLen);

/******************************************************************************
*  rvIppMdmConnGetRemotePresentationInfo
*  -------------------------------------
*  General :        Get Presentation information (name and permission) of
*                   remote party. This information was retrieved from incoming messages.
*
*                   Note: implemented in H323 only!
*  Return Value:    True if Presentation information was returned successfully,
*                   False, if not.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           connHndl -          connection Handle
*  Output:          presentationInfo -  a pointer to Presentation information object.
****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppMdmConnGetRemotePresentationInfo(
    IN  RvIppConnectionHandle       connHndl,
    OUT RvMdmTermPresentationInfo*  presentationInfo);

/******************************************************************************
 * rvIppMdmConnGetCallState
 * ----------------------------------------------------------------------------
 * General:
 *  Get the call state of this connection.
 * Arguments:
 * Input:  connHndl     - A handle to the connection.
 * Output: None.
 *
 * Return Value: The call's state.
 *****************************************************************************/
RVAPI RvCCCallState RVCALLCONV rvIppMdmConnGetCallState(
    IN RvIppConnectionHandle    connHndl);


/******************************************************************************
 * rvMtfGetLogMgr
 * ----------------------------------------------------------------------------
 * General:
 *  Get the toolkit LogMgr as void*. Used to pass it to another toolkit
 * Arguments:
 * Input:
 * Output: None.
 *
 * Return Value: point to toolkit LogMgr.
 *****************************************************************************/
RVAPI void* RVCALLCONV rvMtfGetLogMgr(void);


#endif /*RV_MDMCONTROL_API_H*/
