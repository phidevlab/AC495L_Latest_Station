/******************************************************************************
Filename:    rvCallControlApi.h
Description: Generic call control types
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

#ifndef RV_RVCCAPI_H
#define RV_RVCCAPI_H

#include "rvtypes.h"

/* Note: #include "rvMtfInternals.h" at the end of file. */

/*@*****************************************************************************
 * Package: RvMtfConnectionsPkg (root)
 * -----------------------------------------------------------------------------
 * Title: Connections
 *
 * General: This section contains functions and type definitions for connections
 *  		and calls.
 ****************************************************************************@*/


/*@*****************************************************************************
 * Enum: RvMtfCallType (RvMtfConnectionsPkg)
 * -----------------------------------------------------------------------------
 * Description: This enumerator indicates the type or state of a call: Basic,
 * Conference or Transfer.
 ****************************************************************************@*/
typedef enum
{
	RV_CCCALLSTATE_NORMAL,
		/* This state indicates a basic call with two parties. The Call remains in this state until one of the following occurs:
 			- The local user presses the Conference key (CONFERENCE event) to join an additional participant in the call.
 			  In this case, the state changes to RV_CCCALLSTATE_CONFERENCE_INIT.
 			- The local user presses the Transfer key (TRANSFER or BLIND_TRANFER events) to transfer the remote party
 			  to a different destination. In this case, the state changes to RV_CCCALLSTATE_TRANSFER_INIT. */
	RV_CCCALLSTATE_CONFERENCE_INIT,
		/* This state indicates the start of a Conference process. The call moves to this state when the user first
		   presses the Conference key. In this state, the remote party is put on Hold and the local user establishes
		   a call with a new participant. The call remains in this state until one of the following occurs:
 		 	- The local user presses the Conference key again. In this case, the state changes to RV_CCCALLSTATE_CONFERENCE_COMPLETED
 			- The local user cancels the conference and the state returns to RV_CCCALLSTATE_NORMAL.*/
	RV_CCCALLSTATE_CONFERENCE_COMPLETED,
		/* This state indicates the completion of the conference process, meaning that all three parties have
		   been connected. The call remains in this state until one of the following occurs:
 			- One of the participants leaves the call. In this case, the state changes to RV_CCCALLSTATE_NORMAL
 			- The local user disconnects the call (CONFERENCE event). In this case, the call is destructed.  */
	RV_CCCALLSTATE_TRANSFER_INIT
		/* This state indicates the start of a Transfer process. The call moves to this state after the user first
		   presses Transfer. The Connection remains in this state until the local user presses the Transfer key again.
		   Then a REFER message is sent to the remote party, both calls are disconnected, and the call is destructed. */
} RvMtfCallType;

/* For backward compatibility */
#define RvCCCallState		RvMtfCallType


/*@*****************************************************************************
 * Enum: RvMtfTerminationState (RvMtfTerminationsPkg)
 * -----------------------------------------------------------------------------
 * Description: This enumerator indicates the state of the terminal. It is
 * mainly used to check whether the terminal is in the process of Call Forward
 * activation.
 ****************************************************************************@*/
typedef enum
{
	RV_CCTERMINAL_IDLE_STATE,
        /* This state indicates an idle state. Meaning, terminal is not in process of Call Forward activation
           or deactivation. Once activation or deactivation process is done, terminal is in IDLE state, */

	RV_CCTERMINAL_CFW_ACTIVATING_STATE,
        /* This state indicates that a Call Forward activation process has started and has not yet been completed. This
           process starts when the user presses the Call Forward button, and is complete when one of the following occurs:
 			- The user presses the Call Forward button again, to cancel the activation
 			- The user completes dialing the destination number
 			- The user answers an incoming call
		  When the process ends, the callback RvIppCfwActivateCompletedCB() is called. When the terminal is in this state,
		  all other keys ar blocked except for the Line and Hook keys to enable the user to answer incoming calls. */

	RV_CCTERMINAL_ERROR_STATE
        /* This state is not in use. */
} RvMtfTerminationState;

/* For backward compatibility */
#define RvCCTerminalState		RvMtfTerminationState


/*@*****************************************************************************
 * Enum: RvMtfEvent (RvMtfSignalsEventsPkg)
 * -----------------------------------------------------------------------------
 * Description: This enumerator represents a user event that is sent from
 * the application to the MTF to indicate that the user pressed a key.
 * The user events are indicated through RvMtfPreProcessEventEv() and
 * RvMtfPostProcessEventEv(), enabling the application to follow the exact
 * events occurring on a given termination.
 * Internal events cannot be sent from the application to the MTF, but instead
 * are sent in the MTF internal state machine.
 * The events can affect the connections, the terminations or the media
 * itself.
 ****************************************************************************@*/
typedef enum
{
    RV_CCTERMEVENT_NONE,
        /* 0. No action will be taken. */

    RV_CCTERMEVENT_UNKNOWN,
        /* 1. Unknown event, will be ignored. */

    RV_CCTERMEVENT_GW_ACTIVE,
        /* 2. This event indicates that the gateway is active. It is used to reset the display after terminal starts up.  */

    RV_CCTERMEVENT_OFFHOOK,
        /* 3. The user has gone off-hook (has lifted the phone receiver) or has
              pressed a Line key. These events are logically equivalent, since a
              call can be answered or initiated by pressing the Line key. Caused
              by a kf/kd event with keyid of kh for a UI termination, or an
              al/of event for an Analog termination. */

    RV_CCTERMEVENT_DIALTONE,
        /* 4. Internal event, should be sent by the user application. Applied by the Call Control to move from an INITIATED to a DIALING
		      event. */

    RV_CCTERMEVENT_DIGITS,
        /* 5. The user pressed a DTMF key. */

    RV_CCTERMEVENT_DIGIT_END,
        /* 6. The user released a DTMF key. */

    RV_CCTERMEVENT_DIALCOMPLETED,
        /* 7. Dialing is complete. This event may be internal (if the user
              returns from the callback RvMtfMapDialStringToAddressEv() with a
              value of RV_MDMDIGITMAP_UNAMBIGUOUSMATCH or
			  RV_MDMDIGITMAP_NOMATCH), or it may be sent directly by the user
			  (if the matching is done asynchronously). */

    RV_CCTERMEVENT_MAKECALL,
        /* 8. Internal event, should be sent by the user application. Initiates an incoming call in the connection. */

    RV_CCTERMEVENT_RINGBACK,
        /* 9. Internal event, should be sent by the user application. The outgoing call has reached the other end. */

    RV_CCTERMEVENT_RINGING,
        /* 10. Internal event, should be sent by the user application. An incoming call is transitioning to the ALERTING state. */

    RV_CCTERMEVENT_CALLANSWERED,
        /* 11. Internal event, should be sent by the user application. An outgoing call was answered
               at the destination end and is moving to a CONNECTED state. */

    RV_CCTERMEVENT_ONHOOK,
        /* 12. The user went on-hook. */

    RV_CCTERMEVENT_HOLD,
        /* 13. Internal event, should be sent by the user application. This can originate, for example, from sending the RV_CCTERMEVENT_HOLDKEY event. */

    RV_CCTERMEVENT_MUTE,
        /* 14. The user pressed the Mute key. */

    RV_CCTERMEVENT_HOLDKEY,
        /* 15. The user pressed the Hold key. */

    RV_CCTERMEVENT_UNHOLD,
        /* 16. Internal event, should be sent by the user application. This can originate, for example, from a user pressing the Line key.
			   on a line in a HELD state. */

    RV_CCTERMEVENT_CONFERENCE,
        /* 17. The user pressed the Conference key. The first pressing will activate an additional line
               to call the added party. The second pressing will connect all parties in the conference. */

    RV_CCTERMEVENT_TRANSFER,
        /* 18. The user pressed the Transfer key. The first pressing will activate an additional line
               to call the transfer destination. The second pressing will complete the transfer
               and drop the user from the transfer. */

    RV_CCTERMEVENT_LINE,
        /* 19. The user pressed the Line key. */

    RV_CCTERMEVENT_LINEOTHER,
        /* 20. Internal event, should be sent by the user application. The user pressed the Line key, and
		       a connected call (on Hold or not) exists on a different line. */

    RV_CCTERMEVENT_HEADSET,
        /* 21. The user pressed the Headset key. */

    RV_CCTERMEVENT_HANDSFREE,
        /* 22. The user pressed the Handsfree key. */

    RV_CCTERMEVENT_AUDIOHANDSET,
        /* 23. Internal event, should be sent by the user application. The event indicates that the Handset should be made the active audio termination,
               the media from the current active termination to the new one, etc. */

    RV_CCTERMEVENT_AUDIOHANDSFREE,
        /* 24. Internal event, should be sent by the user application. The event indicates that the Speaker should be made the active audio termination, 
               i.e., the media should be moved from the current active termination to the new
			   one, etc. */

    RV_CCTERMEVENT_FAILGENERAL,
        /* 25. Internal event, should be sent by the user application. The event indicates a general failure,
		       while the reason code specifies the reason for the failure. */

    RV_CCTERMEVENT_MEDIAOK,
        /* 26. Internal event, should be sent by the user application. Creation or modification of a media stream on a termination has succeeded. */

    RV_CCTERMEVENT_MEDIAFAIL,
        /* 27. Internal event, should be sent by the user application. Creation or modification of a media stream on a termination has failed. */

    RV_CCTERMEVENT_DISCONNECTING,
        /* 28. Internal event, should be sent by the user application. Causes the connection to disconnect from the call. */

    RV_CCTERMEVENT_DISCONNECTED,
        /* 29. Internal event. The connection was disconnected and the resources were released. */

    RV_CCTERMEVENT_INCOMINGCALL,
        /* 30. Internal event, should be sent by the user application. The event is usually caused by an incoming signaling message
        	   indicating the establishment of a new call. */

    RV_CCTERMEVENT_REJECTCALL,
        /* 31. Internal event, should be sent by the user application. Rejects an incoming call before the OFFERED state. For example,
        	   because there are no available lines. */

    RV_CCTERMEVENT_TRANSFER_INIT,
        /* 32. This event indicates to the transferring endpoint (A) to start
		       the Transfer process, i.e., to send the transferree endpoint (B)
               a signaling message indicating that it should establish a call
               with the Transfer destination endpoint.*/

    RV_CCTERMEVENT_TRANSFER_OFFERED,
        /* 33. Internal event, should be sent by the user application. The connection is offered an
		       incoming transferred call. This connection represents the
			   transfer destination. The call will replace the existing call
               with the transferring party. */

    RV_CCTERMEVENT_REMOTE_DISCONNECTED,
        /* 34. Internal event, should be sent by the user application. The other party disconnected the call. */

    RV_CCTERMEVENT_ONHOOK_OTHER,
        /* 35. Internal event, should be sent by the user application. */

    RV_CCTERMEVENT_REJECT_KEY,
        /* 36. The user rejected an incoming call on a given line. */

    RV_CCTERMEVENT_MEDIANOTACCEPTED,
        /* 37. Internal event, should be sent by the user application. Currently not supported. The event indicates that the media
               offered by the remote party is not supported by the local party.
			   Processing the event will result in a local warning tone and an
               outgoing signaling message, indicating that the call is rejected
               with reason 415—Media. */

    RV_CCTERMEVENT_MODIFYMEDIA,
        /* 38. Internal event, should be sent by the user application. The event indicates the beginning of a dynamic media change process and is caused by
               the user application calling rvMtfMediaStartModify(). Processing
               the event will result in a Re-Invite message being sent that
			   includes the new media. */

	RV_CCTERMEVENT_MODIFYMEDIA_BY_UPDATE,
		/* 39.  Internal event, should be sent by the user application. Indicates the beginning of a dynamic media change process
				and is caused by the user application calling rvMtfMediaStartModify().
				Processing the event will result in an UPDATE message being
				sent that includes the new media. */

    RV_CCTERMEVENT_MODIFYMEDIA_DONE,
        /* 40. Internal event, should be sent by the user application. The event is processed when the state
               machine has finished processing a dynamic media change and will
			   result in notifying the user application about whether or not
               the process was completed successfully. */

    RV_CCTERMEVENT_BLIND_TRANSFER,
        /* 41. The user pressed the Blind Transfer key. */

    RV_CCTERMEVENT_REDIAL,
        /* 42. This event is sent to activate Redial functionality. */

    RV_CCTERMEVENT_CFW,
        /* 43. This event is sent to activate Call Forward functionality.
        	   Subsequent sending of this event will deactivate Call Forwarding. */

    RV_CCTERMEVENT_USER
        /* 44. All values higher than this value can be used by the user and
		       will be ignored by the MTF state machine and passed to the user application for processing. */

} RvMtfEvent;

#define RvCCTerminalEvent		RvMtfEvent

/*@*****************************************************************************
 * Enum: RvMtfConnectionState (RvMtfConnectionsPkg)
 * -----------------------------------------------------------------------------
 * Description: This enumerator indicates the state of the connection.
 * The connection state represents the state of the call from the perspective
 * of the user, not the signaling.
 ****************************************************************************@*/
typedef enum
{
	RV_CCCONNSTATE_IDLE,
        /* 0
            This state is the initial and final state for all new Connections. Connections in
			the IDLE state are not an active part of a call, and therefore are available for
			new calls. The Connection remains in this state until one of the following occurs:
 			- A new call arrives (an Invite message is received from a remote party) and the state
 			  changes to RV_CCCONNSTATE_INITIATED
 			- The user goes off hook (an OFF_HOOK event is received) and the state changes to
 			  RV_CCCONNSTATE_INITIATED. Connections typically do not stay in the IDLE state long. Instead:
 			- The user application calls rvMtfferminalMakeCall() and the state changes to
			  RV_CCCONNSTATE_ADDRESS_ANALYZE. 	*/

	RV_CCCONNSTATE_INITIATED,
        /* 1
           This state indicates that the local user has begun the process of placing a call,
           but has not yet begun dialing the destination address. After changing to this state,
           the MTF notifies the user application to play a dial tone by calling the callback RvMtfStartSignalEv(),
           and changes the state to RV_CCCONNSTATE_DIALING. This transition of states is done automatically
           by the MTF and cannot be triggered by the local user or the remote party. */

	RV_CCCONNSTATE_DIALING,
	    /* 2
           This state indicates that the local user has begun dialing a destination telephone address
           but has not yet completed dialing. At this stage, the user starts dialing the phone number
           (DIGITS events are received). If the user application uses its own digitmap mechanism,
           the callback RvMtfMatchDialStringfoPatternEv() is called for every digit received.
           The Connection remains in this state while the user dials the phone number (DIGITS event is received).
           When the user completes dialing (i.e., the dialed string matches a legal digitmap pattern), the
		   MTF notifies the user application to open a media stream by calling the callback RvMtfMediaCreateStreamEv()
		   and the state changes to RV_CCCONNSTATE_ADDRESS_ANALYZE. If the user goes on hook (an ON_HOOK event is received),
		   the state changes to RV_CCCONNSTATE_IDLE. */

	RV_CCCONNSTATE_ADDRESS_ANALYZE,
        /* 3
           This state is entered when the user completes dialing the phone number (i.e., the phone number matches
           a legal digitmap pattern). At this stage the callback RvMtfMapDialStringfoAddressEv() is called.
           If the user application returns the destination address (the callback returns RV_OK), an Invite message is sent
		   to the remote party and the state changes to RV_CCCONNSTATE_INPROCESS. If the user application does not
		   return the destination address (the callback returns a value other than RV_OK), the MTF notifies the
		   user application to play a warning tone (by calling the callback RvMtfStartSignalEv()), and changes the
		   state to RV_CCCONNSTATE_FAILED. */

	RV_CCCONNSTATE_INPROCESS,
        /* 4
           	This state indicates that the local user is contacting the destination party,
			meaning that a signaling message (Invite) was sent out. The Connection remains
			in this state until one of the following occurs:
 			- The remote party answers the call (200 OK is received). In this case, the MTF connects the call
 			  by sending an ACK message and changes the state to RV_CCCONNSTATE_CONNECTED.
 			- The remote party indicates that the call is in progress (1XX is received). In this case, the state
 			  is changed to RV_CCCONNSTATE_CALL_DELIVERED.
 			- The local user went on hook (ON_HOOK event). In this case, the state changes to RV_CCCONNSTATE_DISCONNECTED. */

	RV_CCCONNSTATE_CALL_DELIVERED,
        /* 5
           This state indicates that an outgoing call is being offered to the destination party and the
           remote party is notified that the call is in progress.
           For incoming calls, the Connection transitions to this state after the call is answered by the
           local party (OFF_HOOK event) but before a signaling message is sent to remote party (200 OK)
           to connect the call. After sending 200 OK, the state changes to RV_CCCONNSTATE_CONNECTED.
		   For outgoing calls, the connection transitions to this state after the call is answered by
		   remote party (200 OK is received). At this stage, MTF sends ACK message to the remote party and
		   changes the state to RV_CCCONNSTATE_CONNECTED. */

	RV_CCCONNSTATE_OFFERED,
	    /* 6
           This state indicates that an incoming call is being offered to the local user, meaning that
           an Invite message was received. When in this state, the MTF checks for an available line
           (Connection in Idle state). If all lines are busy the MTF sends out an 4XX message.
           If an available line is found, the MTF notifies the user application to open a media stream
           by calling RvMtfMediaCreateStreamEv(). If the user application fails to open the media (the callback
           returns a value other than RV_OK), the MTF notifies the user application to play a warning tone by
           calling RvMtfStartSignalEv() and the state changes to RV_CCCONNSTATE_FAILED. If the media is opened
           successfully, the following occurs:
 			- The MTF indicates to the remote party that the call is in progress by sending out a 180 Ringing message
 			- The MTF notifies the user application to play a ringing tone by calling RvMtfStartSignalEv()
 			- The Connection transitions to the state RV_CCCONNSTATE_ALERTING. */

	RV_CCCONNSTATE_ALERTING,
        /* 7
           	This state indicates that the local user is being notified of an incoming call. The
			Connection remains in this state until one of the following happens:
 			- Local user answers the call (OFF_HOOK event). In this case the state changes to RV_CCCONNSTATE_CALL_DELIVERED.
 			- Remote party cancelled the call (a Cancel message is received). In this case state changes to
			  RV_CCCONNSTATE_DISCONNECTED.
 			- Local user rejects the call (REJECT_KEY event). In this case state changes to RV_CCCONNSTATE_ALERTING_REJECTED.*/

	RV_CCCONNSTATE_DISCONNECTED,
        /* 8
           This state indicates that the connection is no longer part of the call. A Connection in this state is
           interpreted as one that previously belonged to a call. It enters this state when the local user goes
           on hook (ON_HOOK event) or the remote party disconnects the call (BYE received), or when the call is
           rejected (either by the local user or by the MTF due to a shortage of available lines).
           In this state, the Connection changes to the RV_CCCONNSTATE_IDLE state. */

	RV_CCCONNSTATE_CONNECTED,
        /* 9
           This state indicates that a connection is actively part of a call. A Connection remains in this state
           until one of the parties disconnects the call:
 			- The local user goes on hook (ON_HOOK event). In this case, the state changes to RV_CCCONNSTATE_DISCONNECTING.
 			- The remote party disconnects the call (BYE received). In this case, the state changes to RV_CCCONNSTATE_DISCONNECTED.
 			- The local user presses the Transfer key (TRANSFER event) during a connected call. In this case, the MTF
 			  notifies the user application to open a new media stream and the state changes to RV_CCCONNSTATE_TRANSFER_INIT. */

	RV_CCCONNSTATE_FAILED,
	    /* 10
           This state indicates that call establishment failed, for example when media failed to open
           (outgoing and incoming calls) or the phone number failed to match a destination address (outgoing call).
           The MTF automatically changes this state to RV_CCCONNSTATE_DISCONNECTED.*/

	RV_CCCONNSTATE_TRANSFER_INIT,
        /* 11
           This state is relevant for the transferring endpoint (A) when it already has one call established with
           the transferee (B). The state indicates that the Transfer process has been started. The Connection reaches
           this state from the RV_CCCONNSTATE_CONNECTED state. The Connection enters this state
		   when the user presses the Transfer key (TRANSFER event) during a connected call. When the Connection enters
		   this state, the MTF sends an INVITE message to the Transfer destination (C) and changes the state to
		   RV_CCCONNSTATE_TRANSFER_INPROCESS.*/

	RV_CCCONNSTATE_TRANSFER_INPROCESS,
        /* 12
           This state is relevant for the transferring endpoint (A) when it already has one call established with
           the transferee (B). The state is parallel to RV_CCCONNSTATE_INPROCESS during a basic call, and indicates
           that the Transfer destination has been contacted. The Connection remains in this state until an XX message
           arrives from the Transfer destination (C), after which the state will change to RV_CCCONNSTATE_TRANSFER_DELIVERED.*/

	RV_CCCONNSTATE_TRANSFER_DELIVERED,
        /* 13
           This state is relevant for transferring endpoint (A) and for the Transfer destination (C). It is relevant
           for the transferring endpoint (A) when one call has already been established with the transferee (B) and
           a call with the Transfer destination (C) is being established. The Connection reaches this state from
           RV_CCCONNSTATE_TRANSFER_INPROCESS. The Connection remains in this state until the Transfer destination (C)
           answers the call (200 OK is received), after which the state changes to RV_CCCONNSTATE_CONNECTED.
		   It is relevant for the Transfer destination (C) when it received a call from the transferee (B) (Invite was received)
		   that is automatically answered and replaces an existing call with the transferring endpoint (A).
		   The Connection reaches this state from RV_CCCONNSTATE_TRANSFER_ALERTING. When in this state, the MTF
		   answers the call (sends 200 OK) and changes the state to RV_CCCONNSTATE_CONNECTED.*/

	RV_CCCONNSTATE_TRANSFER_OFFERED,
        /* 14
           This state is relevant for the Transfer destination (C). This state indicates that an incoming
           transferred call is being offered to the connection by the transferee (B). When in this state,
           the MTF sends 180 Ringing to the transferee (B) and the state changes to RV_CCCONNSTATE_TRANSFER_ALERTING.*/

	RV_CCCONNSTATE_TRANSFER_ALERTING,
        /* 15
           This state is relevant for the Transfer destination (C). This state indicates that an incoming
           transferred call is alerting in the Connection. This Connection represents the Transfer destination,
           and the call replaces the existing call with the transferring party (A). The call is automatically
           accepted and the connection state will move to the RV_CCCONNSTATE_TRANSFER_DELIVERED state.*/

	RV_CCCONNSTATE_REJECTED,
        /* 16
            This state indicates that an incoming call has been rejected before transitioning
			to the RV_CCCONNSTATE_ALERTING state. This may have happened because no lines were available.
			The Connection will remain in this state until the local user goes on hook (ON_HOOK or LINE events),
			after which the state will change to RV_CCCONNSTATE_DISCONNECTED. */

	RV_CCCONNSTATE_ALERTING_REJECTED,
        /* 17
           This state indicates that an incoming call has been rejected by the user while it was already in the
           ALERTING state. This can happen when the local user rejects the call (REJECT_KEY event).
           The Connection will remain in this state until the local user goes on hook (ON_HOOK or LINE events),
           after which the state will change to RV_CCCONNSTATE_DISCONNECTED.*/

	RV_CCCONNSTATE_UNKNOWN,
	    /* 18
           The state is unknown. */

	RV_CCCONNSTATE_USER
        /* This  must always be the last one */
} RvMtfConnectionState;

/* For backward compatibility */
#define RvCCConnState		RvMtfConnectionState


/*@*****************************************************************************
 * Enum: RvMtfConnectionTransferState (RvMtfConnectionsPkg)
 * -----------------------------------------------------------------------------
 * Description: This enumerator indicates the connection Transfer state. This
 * state is relevant during a Transfer process only.
 ****************************************************************************@*/
typedef enum {
    RV_CCCONNSTATE_TRANSFER_IDLE,        /* 0, Not engaged in a Transfer process. */
    RV_CCCONNSTATE_TRANSFERROR_1ST_CALL, /* 1, The transferring party, first call. */
    RV_CCCONNSTATE_TRANSFERROR_2ND_CALL, /* 2, The transferring party, second call. */
    RV_CCCONNSTATE_TRANSFEREE,			 /* 3, The transferree. Currently not in use. */
    RV_CCCONNSTATE_TRANSFERER			 /* 4, The party that replaces the call with
										    the transferror with a call with the
                                            transferree. Currently not in use. */
} RvMtfConnectionTransferState;

/* For backward compatibility */
#define RvCCConnTransferState		RvMtfConnectionTransferState


/*@*****************************************************************************
 * Enum: RvMtfTermConnState (RvMtfConnectionsPkg)
 * -----------------------------------------------------------------------------
 * Description: This enumerator indicates the Connection Terminal state. The
 * Connection Terminal state represents the state of the call from the
 * perspective of the terminal.
 ****************************************************************************@*/
typedef enum
{
	RV_CCTERMCONSTATE_IDLE,
        /* 0
           There are no calls on the terminal; all lines are idle. */

	RV_CCTERMCONSTATE_RINGING,
        /* 1
           The active line is ringing with the incoming call. */

	RV_CCTERMCONSTATE_TALKING,
        /* 2
           The active line is connected to the remote party. */

	RV_CCTERMCONSTATE_HELD,
        /* 3
           The active line has put the call on Hold. */

	RV_CCTERMCONSTATE_REMOTE_HELD,
        /* 4
           The active line was put on Hold by the remote party (Re-Invite with SDP body with attribute of "a=sendonly" is received). */

	RV_CCTERMCONSTATE_BRIDGED,
        /* 5
           At least one call is connected, in addition to the active line. */

	RV_CCTERMCONSTATE_DROPPED,
        /* 6
           Either the local party or the remote party is disconnected. */

	RV_CCTERMCONSTATE_MUTE,
        /* 7
           The active line is Mute. */

	RV_CCTERMCONSTATE_REMOTE_HELD_LOCAL_HELD
        /* 8
           Both parties have put the call on Hold. */
} RvMtfTermConnState;

/* For backward compatibility */
#define RvCCTermConnState		RvMtfTermConnState


/*@*****************************************************************************
 * Enum: RvMtfReason (RvMtfSignalsEventsPkg)
 * -----------------------------------------------------------------------------
 * Description: This enumerator indicates a reason for events. It provides
 * additional information for certain events that are reported to the application.
 ****************************************************************************@*/
typedef enum
 {
    RV_CCCAUSE_INCOMING_CALL,
        /* 0. The call is an incoming call. */

    RV_CCCAUSE_OUTGOING_CALL,
        /* 1. The call is an outgoing call. */

    RV_CCCAUSE_CALL_WAITING,
        /* 2. The incoming call is a Call Waiting (there is an active call in
              the terminal already). */

    RV_CCCAUSE_BUSY,
        /* 3. The call is not completed because the party is busy (no lines
		      available). */

    RV_CCCAUSE_NOT_FOUND,
        /* 4. Reason for rejecting an incoming call: Destination was not found. */

    RV_CCCAUSE_REORDER_TONE,
        /* 5. Indicates a general failure. Will cause a warning tone to be
		      heard. */

    RV_CCCAUSE_TRANSFER,
        /* 6. The reason for the call is Transfer. */

    RV_CCCAUSE_UNHOLD,
        /* 7. The call was released from Hold. */

    RV_CCCAUSE_CALL_CANCELLED,
        /* 8. The call was canceled by the local party. */

    RV_CCCAUSE_LOCAL_HOLD,
        /* 9. The call was put on Hold by the local party. */

    RV_CCCAUSE_REMOTE_HOLD,
        /* 10. The call was put on Hold by the remote party. The event is caused
               by an incoming signaling message indicating that the call should
			   be put on Hold. */

    RV_CCCAUSE_NEW_CALL,
        /* 11. The call is a new call. */

    RV_CCCAUSE_NORMAL,
        /* 12. This reason is used when no specific reason is required. */

    RV_CCCAUSE_RESOURCES_NOT_AVAILABLE,
        /* 13. The operation failed due to lack of resources. */

    RV_CCCAUSE_MEDIA_NOT_SUPPORTED,
        /* 14. The phone does not support the required media parameters. */

    RV_CCCAUSE_EVENT_BEGIN,
        /* 15. This reason indicates that Key Down was pressed for a digit. */

    RV_CCCAUSE_EVENT_END,
        /* 16. This reason indicates that Key Up was pressed for a digit. */

    RV_CCCAUSE_OPERATION_SUCCEEDED,
        /* 17. The process ended successfully. */

    RV_CCCAUSE_OPERATION_FAILED,
        /* 18. The process ended in failure. */

    RV_CCCAUSE_AUTH_FAIL,
        /* 19. (SIP only) Authentication failed. */

	RV_CCCAUSE_BAD_REQUEST,
        /* 20. Message received with malformed syntax. This is equivalent to
		       the status code 400 "Bad Request". */
    RV_CCCAUSE_UNKNOWN
        /* 21. Reason is unknown. */
} RvMtfReason;

/* For backward compatibility */
#define RvCCEventCause		RvMtfReason


/*@*****************************************************************************
 * Enum: RvCCTerminalType (RvMtfTerminationsPkg)
 * -----------------------------------------------------------------------------
 * Description: This enumerator indicates the type of a terminal. It is mainly
 * used for media callbacks.
 ****************************************************************************@*/
typedef enum
{
	RV_CCTERMINALTYPE_UNKNOWN,
        /* The terminal type is unknown. */

	RV_CCTERMINALTYPE_EPHEMERAL,
        /* This is an ephemeral terminal, usually associated with an RTP
		   session. */

	RV_CCTERMINALTYPE_ANALOG,
        /* This terminal is a physical terminal, associated with an analog
		   line. */

	RV_CCTERMINALTYPE_UI,
        /* A user interface termination, associated with keys, LCD displays,
		   lights, etc. */

	RV_CCTERMINALTYPE_AT,
        /* Audio transducer termination, representing an audio device in the
		   phone, such as Handset, Speaker etc.*/

#ifdef RV_MTF_VIDEO
		RV_CCTERMINALTYPE_VT,
        /* Video transducer termination, representing a video device in the
		   phone, such as Camera or Screen. This type is relevant when
		   compiling the MTF with RV_MTF_VIDEO.*/
#endif

} RvMtfTerminalType;

/* For backward compatibility */
#define RvCCTerminalType		RvMtfTerminalType


/*@*****************************************************************************
* Enum: RvMtfUpdateState (RvMtfBasePkg)
* -----------------------------------------------------------------------------
* Description: This enumerator reflects the UPDATE method process state.
 ****************************************************************************@*/
typedef enum
{
	RV_CCUPDATESTATE_NONE = -1,
		/* Indicates that there is no process going on in which an Update
		   message is being sent or received. */

	RV_CCUPDATESTATE_SENT = RV_TRUE,
		/*  Indicates that an Update message was sent to which was not replied. */

	RV_CCUPDATESTATE_RECEIVED,
		/*  Indicates that an Update message was received to which was not
		    replied. */

} RvMtfUpdateState;


/*@*****************************************************************************
* Enum: RvMtfOfferAnswerState (RvMtfBasePkg)
* -----------------------------------------------------------------------------
* Description: This enumerator reflects the offer-answer state during the call
* setup. Once the call is connected (initial INVITE is ACKed), it remains in
* the RV_MTF_OFFERANSWER_OFFER_ANSWERED state.
 ****************************************************************************@*/
typedef enum
{
	RV_MTF_OFFERANSWER_OFFER_NONE ,
		/* Indicates an idle state. The call is not yet involved in an
		   offer-answer process. */

	RV_MTF_OFFERANSWER_OFFER_SENT ,
		/* Indicates that an offer was sent to the remote party. An answer
		   has not yet been accepted. */

	RV_MTF_OFFERANSWER_OFFER_RECEIVED,
		/* Indicates that an offer was received from the remote party. It has
		   not yet been answered. */

	RV_MTF_OFFERANSWER_OFFER_ANSWERED,
		/* Indicates that an offer was answered. An offer-answer process was
		   completed. */

	RV_MTF_OFFERANSWER_NUM_OF_STATES
		/* This is internal value, indicating the number of states. It should
		   always be the last member of this enumeration. */
} RvMtfOfferAnswerState;

/* This is done for backward compatibility */
#include "rvMtfInternals.h"


#endif /*RV_RVCCAPI_H*/
