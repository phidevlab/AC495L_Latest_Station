/******************************************************************************
Filename:    rvMtfExtControlTypes.h
Description: This file includes types definitions for extensibility of MTF.

*******************************************************************************
                Copyright (c) 2008 RADVISION
*******************************************************************************
NOTICE:
This document contains information that is proprietary to RADVISION.
No part of this publication may be reproduced in any form whatsoever
without written prior approval by RADVISION.

RADVISION reserves the right to revise this publication and make changes
without obligation to notify any person of such revisions or changes.
******************************************************************************/
#ifndef MTFCONTROL_TYPES_H
#define MTFCONTROL_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "rvMtfHandles.h"
#include "rvMtfBaseTypes.h"
#include "rvMtfEventsApi.h"
#include "rvSipControlApi.h"

#ifdef RV_CFLAG_TLS
#include "sipTls.h"
#endif

/*@*****************************************************************************
 * Package: RvMtfSipExtPkg (root)
 * -----------------------------------------------------------------------------
 * Title: SIP Control Extensibility
 *
 * General: This section contains functions and type definitions that allow
 *  		limited control of the SIP Stack.
 ****************************************************************************@*/

 /*@*****************************************************************************
  * Package: RvMtfCallControlExtPkg (root)
  * -----------------------------------------------------------------------------
  * Title: Call Control Extensibility
  *
  * General: This section contains functions and type definitions that allow
  *  		 limited control of Call Control features.
 ****************************************************************************@*/

/*@*****************************************************************************
 * RvMtfChangeSipStackConfigEv (RvMtfSipExtPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         This is a prototype for a callback that enables the application to
 *		   change the default settings of SIP Stack configuration parameters.
 *         When this function is called, pStackCfg is filled with default values
 *		   set by the MTF and SIP. The application can change the values of
 *		   allowed parameters only (for a complete list of these, see the
 *         Programmer Guide). After this callback returns, both the MTF and the
 *         SIP Stack perform validation checks on the returned values. If allowed
 *         parameters were set to invalid values, the values are changed to
 *		   default ones. If unallowed parameters were changed, they are ignored.
 *		   In both cases a warning is printed to the log output.
 *
 * Arguments:
 *
 * Input:  pStackCfg - Pointer to the SIP Stack configuration filled with
 *                     default values.
 *
 * Output: pStackCfg - Pointer to the SIP Stack configuration filled with
 *                     application values.
 *
 * Return Value: None.
 ****************************************************************************@*/
typedef void (RVCALLCONV *RvMtfChangeSipStackConfigEv)(
							INOUT RvSipStackCfg* pStackCfg);

/*@*****************************************************************************
 * RvMtfRegisterSipStackEventsEv (RvMtfSipExtPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         This is a prototype for a callback that enables the application to
 *		   register its own implementations to SIP Stack events.
 *         This callback enables the user to listen for SIP Stack events for
 *         which the MTF does not listen.
 *
 *		   Note: The application must not override a callback that was already
 *         registered by the MTF. This can be avoided by checking whether the
 *         callback pointer is NULL or not (for an example, see the MTF Sample
 *         Application).
 *
 * Arguments:
 *
 * Input:  hSipStack		-  Handle to the SIP Stack.
 *		   sipStackClbks	-  Structure with pointers to implementations of
 *							   MTF to SIP Stack callbacks.
 *
 * Return Value: None.
 ****************************************************************************@*/
typedef void (RVCALLCONV *RvMtfRegisterSipStackEventsEv)(
					IN RvSipStackHandle				hSipStack,
					IN RvIppSipStackCallbacks*		sipStackClbks);

/*@*****************************************************************************
 * RvMtfPreCallCreatedIncomingEv (RvMtfSipExtPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         This callback is called after a new incoming SIP call-leg is created,
 *		   and before the MTF processes the new call. At this point, no MTF
 *		   objects associated with the call-leg have been created yet, so the
 *         application cannot associate its data with the call. The data can be
 *		   associated either by calling rvMtfConnectionSetAppHandle()
 *		   after this callback was called, or when the callback
 *		   RvMtfPostCallCreatedIncomingEv() is called. The main use of this
 *		   callback is for indicating to the MTF that this call should be ignored.
 *
 * Arguments:
 *
 * Input:  hCallLeg - Handle to SIP Stack call-leg.
 *         hAppMtf  - Handle to application data associated with MTF instance
 *					  (no MTF objects related to the call were created yet).
 *
 * Return Value: One of the following:
 *					- RV_MTF_IGNORE_BY_STACK —
 *					  This value is not relevant for this callback.
 *					- RV_MTF_IGNORE_BY_MTF —
 *					  This value indicates to the MTF to ignore the message,
 *					  but the SIP Stack will still process it.
 *					- RV_MTF_DONT_IGNORE —
 *					  This value indicates to both the SIP Stack and the MTF
 *					  to process the message.
 ****************************************************************************@*/
typedef RvMtfMsgProcessType (RVCALLCONV *RvMtfPreCallCreatedIncomingEv)(
    IN  RvSipCallLegHandle      hCallLeg,
    IN  RvMtfAppHandle			hAppMtf);

/*@*****************************************************************************
 * RvMtfPostCallCreatedIncomingEv (RvMtfSipExtPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         This callback is called after a new incoming SIP call-leg is created,
 *		   and before the MTF processes the new call. At this point the MTF has
 *		   already created the objects that are related to the call-leg (hConn).
 *		   The application can set its data associated with the call.
 *
 * Arguments:
 *
 * Input:  hCallLeg - Handle to the SIP Stack call-leg.
 *
 * Output: hConnApp - Handle to the application data associated with the call.
 *
 * Return Value: None.
 ****************************************************************************@*/
typedef void (RVCALLCONV *RvMtfPostCallCreatedIncomingEv)(
			IN RvSipCallLegHandle			hCallLeg,
			OUT RvMtfConnAppHandle*			hConnApp);

/*@*****************************************************************************
 * RvMtfPreCallCreatedOutgoingEv (RvMtfSipExtPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         This callback is called after the MTF creates a new SIP call-leg
 *		   and before the MTF builds SIP headers in the call-leg object.
 *         The "to" and "from" values will be used for building TO and FROM
 *		   headers in the outgoing Invite message. The application can change
 *		   these values to different ones, and those values will be used in the
 *		   TO and FROM headers. The application can also set its data associated
 *         with the call.
 *
 * Arguments:
 *
 * Input:  hCallLeg     - Handle to the SIP Stack call-leg.
 *         hConn		- Handle to the connection object (associated with this
 *                        call-leg).
 *         to           - The destination address used for the TO header.
 *         from         - The source address used for the FROM header.
 *
 * Output: to           - The destination address to be used for the TO header.
 *         from         - The source address to be used for the FROM header.
 *         hConnApp     - The handle to the application data associated with the
 *                        connection object (the call).
 *
 * Return Value: None.
 ****************************************************************************@*/
typedef void (RVCALLCONV *RvMtfPreCallCreatedOutgoingEv)(
    IN RvSipCallLegHandle			hCallLeg,
	IN RvIppConnectionHandle		hConn,
    INOUT RvChar*					to,
    INOUT RvChar*					from,
    OUT RvMtfConnAppHandle*			hConnApp);


/*@*****************************************************************************
 * RvMtfPostCallCreatedOutgoingEv (RvMtfSipExtPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         This callback is called after the MTF creates a new SIP CallLeg,
 *		   i.e., after the MTF has built SIP headers in the call-leg object and
 *         before it sends out the Invite message. The application can set
 *         additional SIP headers to the outgoing Invite message,
 *		   or modify/remove the SIP headers set by the MTF (not recommended).
 *
 * Arguments:
 *
 * Input:  hCallLeg    - Handle to the SIP Stack call-leg.
 *         hConn       - Handle to the connection object (associated with this
 *                       call-leg).
 *         hConnApp    - Handle to the application data associated with the
 *                       connection object (the call).
 *
 * Return Value: None.
 ****************************************************************************@*/
typedef void (RVCALLCONV *RvMtfPostCallCreatedOutgoingEv)(
    IN RvSipCallLegHandle       hCallLeg,
    IN RvIppConnectionHandle    hConn,
    IN RvMtfConnAppHandle       hConnApp);

/*@*****************************************************************************
 * RvMtfPreCallLegStateChangedEv (RvMtfSipExtPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         This callback is called when the SIP Stack event
 *         RvSipCallLegStateChangedEv() is invoked, and before the MTF processes
 *		   this event. The application may retrieve information from the
 *         call-leg or the message, or may choose to indicate to the MTF to
 *		   ignore the event. If the application indicates to the MTF to ignore
 *		   the event, then the application is responsible for "wrapping out" the
 *		   scenario (releasing objects, etc.).
 *
 * Arguments:
 *
 * Input:   hCallLeg    - Handle to the SIP Stack call-leg.
 *          hConn   	- Handle to the connection object (associated with this
 *                        call-leg).
 *          hConnApp    - Handle to the application data associated with the
 *                        connection object (the call).
 *          eState      - SIP call-leg state.
 *          eReason     - SIP reason for the change of state.
 *
 * Return Value: One of the following:
 *					- RV_MTF_IGNORE_BY_STACK —
 *					  This value is not relevant for this callback.
 *					- RV_MTF_IGNORE_BY_MTF —
 *					  This value indicates to the MTF to ignore
 *					  the message, but the SIP Stack will still process it.
 *					- RV_MTF_DONT_IGNORE —
 *					  This value indicates to both the SIP Stack and the MTF
 *					  to process the message.
 ****************************************************************************@*/
typedef RvMtfMsgProcessType (RVCALLCONV *RvMtfPreCallLegStateChangedEv)(
    IN RvSipCallLegHandle               hCallLeg,
    IN RvIppConnectionHandle            hConn,
    IN RvMtfConnAppHandle				hConnApp,
    IN RvSipCallLegState                eState,
    IN RvSipCallLegStateChangeReason    eReason);

/*@*****************************************************************************
 * RvMtfPostCallLegStateChangedEv (RvMtfSipExtPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         This callback is called when the SIP Stack event
 *         RvSipCallLegStateChangedEv() is invoked, and after the MTF processes
 *		   this event. The application may retrieve information from the
 *         call-leg or the message.
 *
 * Arguments:
 *
 * Input:   hCallLeg    - Handle to the SIP Stack call-leg.
 *          hConn   	- Handle to the connection object (associated with this
 *                        call-leg).
 *          hConnApp    - Handle to the application data associated with the
 *                        connection object (the call).
 *          eState      - SIP call-leg state.
 *          eReason     - SIP reason for the change of state.
 *
 * Return Value: None.
 ****************************************************************************@*/
typedef void (RVCALLCONV *RvMtfPostCallLegStateChangedEv)(
    IN RvSipCallLegHandle               hCallLeg,
    IN RvIppConnectionHandle            hConn,
    IN RvMtfConnAppHandle				hConnApp,
    IN RvSipCallLegState                eState,
    IN RvSipCallLegStateChangeReason    eReason);

/*@*****************************************************************************
 * RvMtfMsgToSendEv (RvMtfSipExtPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         This callback is invoked before the SIP message is sent and after the
 *         MTF adds its data to the message (SDP body, etc.). The application
 *		   can retrieve information from the outgoing message or add its own
 *		   data to the message (for example, it can add SIP headers, modify the
 *         SDP body set by the MTF, etc.).
 *
 * Arguments:
 *
 * Input:   hCallLeg  - Handle to the SIP Stack call-leg.
 *          hConn     - Handle to the connection object (associated with this
 *                      call-leg).
 *          hConnApp  - Handle to the application data associated with the
 *                      connection object (the call).
 *          hMsg	  - Handle to the SIP message.
 *
 * Return Value: None.
 ****************************************************************************@*/
typedef void (RVCALLCONV *RvMtfMsgToSendEv)(
    IN RvSipCallLegHandle       hCallLeg,
    IN RvIppConnectionHandle    hConn,
	IN RvMtfConnAppHandle		hConnApp,
    IN RvSipMsgHandle           hMsg);

/*@*****************************************************************************
 * RvMtfPreMsgReceivedEv (RvMtfSipExtPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         This callback is invoked when a new SIP message is received and
 *		   before the MTF and the SIP Stack process this message. The application
 *         may change the SIP message before it is processed by the MTF.
 *		   Also, the application may decide that a message will be processed
 *		   by the SIP Stack but ignored by the MTF, or ignored by both, or
 *         processed by both.
 *
 * Arguments:
 *
 * Input:   hCallLeg    - Handle to the SIP Stack call-leg.
 *          hConn   	- Handle to the connection object (associated with this
 *                        call-leg).
 *          hConnApp    - Handle to the application data associated with the
 *                        connection object (the call).
 *          hMsg		- Handle to the SIP message.
 *
 * Return Value: One of the following:
 *				 - RV_MTF_IGNORE_BY_STACK —
 *				   This value indicates to both the SIP Stack and the MTF
 *				   to ignore the message. When this value is returned, the
 *				   callback RvIppSipPreCallLegCreatedIncomingCB() should also
 *                 return RV_MTF_IGNORE_BY_STACK, otherwise a memory leak will
 *                 occur (when RvIppSipPreCallLegCreatedIncomingCB()
 *				   is called, MTF resources are allocated but not released).
 *				 - RV_MTF_IGNORE_BY_MTF —
 *				   This value indicates to the MTF to ignore the message,
 *				   but the SIP Stack will still process it.
 *				 - RV_MTF_DONT_IGNORE —
 *				   This value indicates to both the SIP Stack and the MTF
 *				   to process the message.
 ****************************************************************************@*/
typedef RvMtfMsgProcessType (RVCALLCONV *RvMtfPreMsgReceivedEv)(
    IN RvSipCallLegHandle       hCallLeg,
    IN RvIppConnectionHandle    hConn,
    IN RvMtfConnAppHandle		hConnApp,
    IN RvSipMsgHandle           hMsg);

/*@*****************************************************************************
 * RvMtfPostMsgReceivedEv (RvMtfSipExtPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         This callback is invoked when a new SIP message is received. This
 *		   should be done after the MTF processes the message, but before the SIP
 *		   Stack processes it. The application may change the SIP message after
 *         it is processed by the MTF.
 *
 * Arguments:
 *
 * Input:   hCallLeg    - Handle to the SIP Stack call-leg.
 *          hConn   	- Handle to the connection object (associated with this
 *                        call-leg).
 *          hConnApp    - Handle to the application data associated with the
 *                        connection object (the call).
 *          hMsg		- Handle to the SIP message.
 *
 * Return Value:  None.
 ****************************************************************************@*/
typedef void (RVCALLCONV *RvMtfPostMsgReceivedEv)(
    IN RvSipCallLegHandle       hCallLeg,
    IN RvIppConnectionHandle    hConn,
    IN RvMtfConnAppHandle		hConnApp,
    IN RvSipMsgHandle           hMsg);

/*@*****************************************************************************
 * RvMtfRegClientStateChangedEv (RvMtfSipExtPkg)
 * -----------------------------------------------------------------------------
 * General:
 *			This callback is invoked before the MTF handles registration state
 *			changes of the SIP RegClient. The RegClient state indicates the
 *          status of the registration with the SIP server.
 *
 * Arguments:
 *
 * Input:  hRegClient       - Handle to the SIP reg-client object.
 *         hTerminal		- Handle to the terminal object associated with
 *                            this event.
 *         hAppTerminal     - Handle to the application data associated with
 *                            the terminal object.
 *         eState           - SIP reg-client state.
 *         eReason          - Reason for the reg-client change of state.
 *
 * Return Value:  One of the following:
 *					- RV_MTF_IGNORE_BY_STACK —
 *					  This value is not relevant for this callback.
 *					- RV_MTF_IGNORE_BY_MTF —
 *					  This value indicates to the MTF to ignore the message,
 *					  but the SIP Stack will still process it.
 *					- RV_MTF_DONT_IGNORE —
 *					  This value indicates to both the SIP Stack and the MTF
 *					  to process the message.
 ****************************************************************************@*/
typedef RvMtfMsgProcessType (RVCALLCONV *RvMtfRegClientStateChangedEv)(
			IN RvSipRegClientHandle             hRegClient,
			IN RvIppTerminalHandle              hTerminal,
			IN RvMtfTerminalAppHandle           hAppTerminal,
			IN RvSipRegClientState              eState,
			IN RvSipRegClientStateChangeReason  eReason);
/*@*****************************************************************************
 * RvMtfRegClientCreatedEv (RvMtfSipExtPkg)
 * -----------------------------------------------------------------------------
 * General:
 *			This callback is invoked After the MTF has created a Sip RegClient
 *          object.
 *			The user application may now add any desireable data to this object.
 *
 * Arguments:
 *
 * Input:  hRegClient       - Handle to the SIP reg-client object.
 *
 * Return Value:     RV_OK on success, error code otherwise
 ****************************************************************************@*/
typedef RvStatus (RVCALLCONV *RvMtfRegClientCreatedEv)(
			IN RvSipRegClientHandle             hRegClient);


/*@*****************************************************************************
 * RvMtfRegExpResolutionNeededEv (RvMtfSipExtPkg)
 * -----------------------------------------------------------------------------
 * General:
 *        This callback is invoked before the MTF receives the
 *        RvSipTransmitterMgrRegExpResolutionNeededEv event from the SIP Stack.
 *		  It is called when the Tel-URI scheme ("tel:") is returned in the
 *		  callback RvMdmTermMapDialStringToAddressCB() and the outbound proxy
 *        was not configured.
 *
 * Arguments:
 *
 * Input: hTrxMgr        - Transmitter manager.*
 *        hTrx           - Handle to the transmitter (NULL if the transmitter
 *                         is not an application transmitter).
 *
 * Output: pRegExpParams - Structure that holds the information for the regular
 *                         expression application.
 *                         The pMatchArray should be filled in by
 *                         RvSipTransmitterRegExpResolutionNeededEv with
 *                         substring match addresses. Any unused structure
 *                         elements should contain the value -1.
 *                         Each startOffSet element that is not -1, indicates
 *                         the start offset of the next-largest substring match
 *                         in the string. The relative endOffSet element
 *                         indicates the end offset of the match.
 *
 * Return Value:  None.
 ****************************************************************************@*/
typedef RvStatus     (RVCALLCONV *RvMtfRegExpResolutionNeededEv)(
    IN  RvSipTransmitterMgrHandle                   hTrxMgr,
    IN  RvSipTransmitterHandle                      hTrx,
    INOUT RvSipTransmitterRegExpResolutionParams*   pRegExpParams);

/*@*****************************************************************************
*  RvMtfPreProcessEventEv (RvMtfCallControlExtPkg)
* -----------------------------------------------------------------------------
*  General :    This callback is invoked before an MDM event is processed in the
*               MTF state machine. The application can indicate to the MTF to
*               ignore this event by setting newEventId to RV_MTF_EVENT_NONE.
*               The application can also replace the event with a different one
*			    by setting newEventId to another event. In this case, the MTF
*				processes the event returned by the application instead of
*				the event passed in the callback.
*
*  Arguments:
*  Input:       hConn		- Handle to the connection object.
*               hConnApp	- Handle to the application data associated with
*                             the connection (the call).
*				connState	- Connection state.
*				eventId		- Event to be processed by the MTF.
*				reason		- Reason for the event.
*
*  Output:      newEventId	- Event to be processed instead of the original
*                             event. If it is not set, origEventId will be
*				              processed.
*				reason		- Reason for the event to be processed instead of
*                             the original event. If it is not set, origEventId
*                             will be processed.
*
*  Return Value:    None.
****************************************************************************@*/
typedef void  (RVCALLCONV *RvMtfPreProcessEventEv)(
                                        IN      RvIppConnectionHandle   hConn,
										IN		RvMtfConnAppHandle		hConnApp,
										IN		RvMtfConnectionState	connState,
                                        IN		RvMtfEvent				eventId,
										OUT		RvMtfEvent*				newEventId,
                                        INOUT   RvMtfReason*			reason);

/*@*****************************************************************************
*  RvMtfPostProcessEventEv (RvMtfCallControlExtPkg)
* -----------------------------------------------------------------------------
*  General :    This callback is invoked after an MDM event was processed by the
*               MTF state machine.
*
*  Arguments:
*  Input:       hConn		- Handle to the connection object.
*               hConnApp	- Handle to the application data associated with the
*                             connection (the call).
*				connState	- Connection state.
*				eventId		- Event that was processed by the MTF.
*				reason		- Reason for the event.
*
*  Return Value:    None.
****************************************************************************@*/
typedef void (RVCALLCONV *RvMtfPostProcessEventEv)(
                                        IN  RvIppConnectionHandle   hConn,
										IN	RvMtfConnAppHandle		hConnApp,
										IN	RvMtfConnectionState	connState,
                                        IN  RvMtfEvent				eventId,
                                        IN  RvMtfReason				reason);


/*@*****************************************************************************
*  RvMtfUpdateTextDisplayEv (RvMtfCallControlExtPkg)
* -----------------------------------------------------------------------------
*  General :    This callback is invoked whenever the MTF indicates to the
*               application that the text display in the terminal should be
*				updated. The application should decide on the specific text
*               to be used by checking the states of the connection and the
*               terminal.
*
*  Arguments:
*  Input:       hConn		- Handle to the connection object.
*               hConnApp	- Handle to the application data associated with
*                             the connection (the call).
*				connState	- Connection state.
*				hTerminal	- Handle to the terminal object.
*				eventId		- Event that was processed by the MTF.
*				reason		- Reason for the event.
*
*  Return Value:    None.
****************************************************************************@*/
typedef void (RVCALLCONV *RvMtfUpdateTextDisplayEv)(
    IN RvIppConnectionHandle    hConn,
	IN RvMtfConnAppHandle		hConnApp,
	IN RvMtfConnectionState		connState,
    IN RvIppTerminalHandle      hTerminal,
    IN RvMtfEvent			    eventId,
    IN RvMtfReason				reason);

/*@*****************************************************************************
 * Type: RvMtfCallControlClbks (RvMtfCallControlExtPkg)
 * -----------------------------------------------------------------------------
 * Description: This structure includes Call Control Extensibility callbacks.
 * The application can use these callbacks to support proprietary behavior
 * of the terminal or to add new functionality from the user perspective (not
 * from the signaling perspective).
 ****************************************************************************@*/
typedef struct
{
    RvMtfPreProcessEventEv				preProcessEventCB;
        /* Enables the application to process an event before the MTF processes
	    it, or to handle its own events. */

    RvMtfPostProcessEventEv				postProcessEventCB;
        /* Enables the application to process an event after the MTF processes
	    it. */

    RvMtfUpdateTextDisplayEv            updateTextDisplayCB;
        /* Enables the application to control the text display. */

} RvMtfCallControlClbks;


/*@*****************************************************************************
 * Type: RvMtfSipControlClbks (RvMtfSipExtPkg)
 * -----------------------------------------------------------------------------
 * Description: This structure contains SIP Stack callbacks that the user
 *              application can implement to gain control over the SIP Stack.
 ****************************************************************************@*/
typedef struct
{
    RvMtfChangeSipStackConfigEv             stackConfigCB;
		/* This callback enables modifying the SIP Stack configuration. */
    RvMtfRegisterSipStackEventsEv           registerStackEventsCB;
		/* This callback enables registering the application implementation
	       for SIP callbacks to the SIP Stack. */
    RvMtfPreCallCreatedIncomingEv		    preCallLegCreatedIncomingCB;
    RvMtfPostCallCreatedIncomingEv		    postCallLegCreatedIncomingCB;
    RvMtfPreCallCreatedOutgoingEv		    preCallLegCreatedOutgoingCB;
    RvMtfPostCallCreatedOutgoingEv		    postCallLegCreatedOutgoingCB;
    RvMtfPreCallLegStateChangedEv	        preStateChangedCB;
    RvMtfPostCallLegStateChangedEv          postStateChangedCB;
 //   RvMtfPreMsgToSendEv                   preMsgToSendCB;
    RvMtfMsgToSendEv		                postMsgToSendCB;
    RvMtfPreMsgReceivedEv	                preMsgReceivedCB;
    RvMtfPostMsgReceivedEv                  postMsgReceivedCB;
    RvMtfRegClientStateChangedEv			regClientStateChangedCB;
	RvMtfRegClientCreatedEv					regClientCreatedCB;
    RvMtfRegExpResolutionNeededEv		    regExpResolutionNeededCB;

} RvMtfSipControlClbks;




#ifdef __cplusplus
}
#endif


#endif /*MTFCONTROL_TYPES_H*/


