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

/* RvCCCallState
 * ------------------------------------------------------------------------
 * The call's state.
 */
typedef enum
{
	RV_CCCALLSTATE_NORMAL,					/* 0 */
	RV_CCCALLSTATE_CONFERENCE_INIT,			/* 1 */
	RV_CCCALLSTATE_CONFERENCE_COMPLETED,	/* 2 */
	RV_CCCALLSTATE_TRANSFER_INIT			/* 3 */
} RvCCCallState;

/* RvCCTerminalState
 * ------------------------------------------------------------------------
 * The terminal's state.
 */
typedef enum
{
	RV_CCTERMINAL_IDLE_STATE,
        /* IDLE STATE, all events are accepted in any time */

	RV_CCTERMINAL_CFW_ACTIVATING_STATE,
        /* CFW activating process started. This is used to block non relevant
           events to be handled. For example, ONHOOK, TRANSFER events etc.. */

	RV_CCTERMINAL_ERROR_STATE
        /* Error has occurred on this terminal. */
} RvCCTerminalState;


typedef enum
{
	RV_INDSTATE_ON,
		RV_INDSTATE_OFF,
		RV_INDSTATE_BLINK,
		RV_INDSTATE_FAST_BLINK,
		RV_INDSTATE_SLOW_BLINK
} RvCCTerminalIndState;

/* RvCCConnType
 * ------------------------------------------------------------------------
 * The type of connections in the MTF.
 */
typedef enum
 {
	RV_CCCONNTYPE_MDM,
        /* The connection type is an MDM one, associated with a local termination */

	RV_CCCONNTYPE_NETWORK
        /* The connection is a network connection, associated with a remote client or
           server */
} RvCCConnType;

/* RvCCConnState
 * ------------------------------------------------------------------------
 * The connection's state
 */
typedef enum
{
	RV_CCCONNSTATE_IDLE,
        /* 0
           This state is the initial state for all new connections. Connections in the IDLE
           state are not actively part of a call. Connections typically do not stay in the
           IDLE state for long, but instead transition to other states. */

	RV_CCCONNSTATE_INITIATED,
        /* 1
           This state indicates that the originating end of a call has begun the process of
           placing a call, but has not yet begun dialing the destination address. Typically, a
           terminal (phone) has gone off-hook. */

	RV_CCCONNSTATE_DIALING,
	    /* 2
           This state indicates that the originating end of a call has begun dialing a
           destination telephone address, but has not yet completed dialing. At this stage
           the user callback RvMdmTermMatchDialStringCB() is called for every digit
           received. */

	RV_CCCONNSTATE_ADDRESS_ANALYZE,
        /* 3
           This state is entered when the complete initial information package or dialing
           string from the originating party are available. At this stage the user callback
           RvMdmTermMapDialStringToAddressCB() is called. This state is exited when
           the routing address becomes available. */

	RV_CCCONNSTATE_INPROCESS,
        /* 4
           This state implies that the connection object is contacting the destination side.
           The contact is established as a result of the underlying protocol messages. */

	RV_CCCONNSTATE_CALL_DELIVERED,
        /* 5
           This state indicates that an outgoing call is being offered to the destination side
           (which is in an ALERTING state). For incoming calls, the connection transitions
           to this state after the call is answered but before transitioning to the
           CONNECTED state. */

	RV_CCCONNSTATE_OFFERED,
	    /* 6
           This state indicates that an incoming call is being offered to the connection. */

	RV_CCCONNSTATE_ALERTING,
        /* 7
           This state implies that the Terminal is being notified of an incoming call. */

	RV_CCCONNSTATE_DISCONNECTED,
        /* 8
           This state implies that the connection is no longer part of the call. A connection
           in this state is interpreted as one that previously belonged to a call. */

	RV_CCCONNSTATE_CONNECTED,
        /* 9
           This state implies that a connection is actively part of a call. In common terms,
           two people talking to one another are represented by two connections in the
           CONNECTED state. */

	RV_CCCONNSTATE_FAILED,
	    /* 10
           This state indicates that a connection to this end of the call has failed. */

	RV_CCCONNSTATE_TRANSFER_INIT,
        /* 11
           This state is relevant for transferring endpoint (A). The state indicates that the
           Transfer process has been started. */

	RV_CCCONNSTATE_TRANSFER_INPROCESS,
        /* 12
           This state is relevant for transferring endpoint (A). The state is relevant to
           RV_CCCONNSTATE_INPROCESS, and indicates that the Transfer destination
           has been contacted. */

	RV_CCCONNSTATE_TRANSFER_DELIVERED,
        /* 13
           This state is relevant for transferree endpoint (B). For incoming transfer calls
           (User C in the example above), the connection transitions to this state after the
           call is answered and before transitioning to the CONNECTED state. */

	RV_CCCONNSTATE_TRANSFER_OFFERED,
        /* 14
           This state is relevant for transfer destination endpoint (C). This state indicates
           that an incoming transferred call is being offered to the connection. This
           connection represents the transfer destination (User C in the example above) and
           the call replaces the existing call with the transferring party. The call moves to
           the TRANSFER_ALERTING state. */

	RV_CCCONNSTATE_TRANSFER_ALERTING,
        /* 15
           This state is relevant for transfer destination endpoint (C). This state indicates
           that an incoming transferred call is alerting in the connection. This connection
           represents the transfer destination, and the call replaces the existing call with the
           transferring party. The call is automatically accepted and will move to the
           TRANSFER_DELIVERED state. */

	RV_CCCONNSTATE_REJECTED,
        /* 16
           This state indicates that an incoming call has been rejected before transitioning
           to the OFFERED state. For example, this may have happened because no lines
           were available. */

	RV_CCCONNSTATE_ALERTING_REJECTED,
        /* 17
           This state indicates that an incoming call has been rejected by the user while it
           was already in the ALERTING state. For example, because the user pressed a
           REJECT key. */

	RV_CCCONNSTATE_UNKNOWN,
	    /* 18
           The state is unknown. */

	RV_CCCONNSTATE_USER
        /* This  must always be the last one */
} RvCCConnState;


/*===========================*/
/* Connection transfer States*/
/*===========================*/
typedef enum {
    RV_CCCONNSTATE_TRANSFER_IDLE,           /* 0, Not in a transfer process */
    RV_CCCONNSTATE_TRANSFERROR_1ST_CALL,	/* 1, the one who transfers, 1st call */
    RV_CCCONNSTATE_TRANSFERROR_2ND_CALL,	/* 2, the one who transfers, 2nd call */
    RV_CCCONNSTATE_TRANSFEREE,			    /* 3, the one who is moved. MEANWHILE, NOT IN USED */
    RV_CCCONNSTATE_TRANSFERER			    /* 4, the one who replaces the call with the transferror
                                                to a call with the transferree. MEANWHILE, NOT IN USED */
} RvCCConnTransferState;



/*===========================*/
/* AudioTerm States - Bitmap */
/*===========================*/
#define RV_CCAUDIOTERM_PASSIVE    0   /*00000000*/
#define RV_CCAUDIOTERM_HS_ACTIVE  1   /*00000001*/
#define RV_CCAUDIOTERM_HT_ACTIVE  2   /*00000010*/
#define RV_CCAUDIOTERM_HF_ACTIVE  4   /*00000100*/

typedef int RvCCAudioTermState;


/* RvCCTermConnState
 * ------------------------------------------------------------------------
 * The termination connection's state
 */
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
           The active line was put on Hold by the remote party. */

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
} RvCCTermConnState;


/* RvCCMediaState
 * ------------------------------------------------------------------------
 * The media state, as associated to a given connection.
 */
typedef enum
{
	RV_CCMEDIASTATE_NONE = -1,			/* -1 */
	RV_CCMEDIASTATE_CREATING = RV_TRUE,	/*  1 */
	RV_CCMEDIASTATE_CREATED,			/*  2 */
	RV_CCMEDIASTATE_CONNECTED,			/*	3 */
	RV_CCMEDIASTATE_DISCONNECTED,		/*  4 */
    RV_CCMEDIASTATE_NOTSUPPORTED,       /*  5 */
	RV_CCMEDIASTATE_MODIFYING,			/*	6 */ /* modify media */
	RV_CCMEDIASTATE_FAILED				/*  7 */
} RvCCMediaState;


/* RvCCTerminalEvent
 * ------------------------------------------------------------------------
 * Termination events. These events are indicated through
 * RvIppMdmPrePrecessEventCB() and RvIppMdmPostProcessEventCB(), enabling
 * the application to follow on the exact events occurring on a given
 * termination.
 * These events can affect the connections, the terminations or the media
 * itself.
 */
typedef enum
{
    RV_CCTERMEVENT_NONE,
        /* 0. No action will be taken. */

    RV_CCTERMEVENT_UNKNOWN,
        /* 1. Unknown event, will be ignored. */

    RV_CCTERMEVENT_GW_ACTIVE,
        /* 2. This event indicates that the gateway is active. It is used by the
              Call Control to initialize the display. Caused by an rvcc/ga event. */

    RV_CCTERMEVENT_OFFHOOK,
        /* 3. The user has gone off-hook (has lifted the phone receiver) or has
              pressed a Line key. These events are logically equivalent, as a call
              can be answered or initiated by pressing the Line key. Caused by a kf/kd
              event with keyid of kh for a UI termination, or an al/of event for an
              Analog termination. */

    RV_CCTERMEVENT_DIALTONE,
        /* 4. Applied by the Call Control to move from INITIATED to DIALING event. */

    RV_CCTERMEVENT_DIGITS,
        /* 5. The user has pressed a DTMF key. Caused by a kp/kd event for UI
              termination, or a dd/d(n) event for Analog termination. */

    RV_CCTERMEVENT_DIGIT_END,
        /* 6. The user has released a DTMF key. Caused by a kp/ku event for UI
              termination. Not used in Analog terminations. */

    RV_CCTERMEVENT_DIALCOMPLETED,
        /* 7. Dialing is complete. This can either be an internal event (if the
              user returns from the RvMdmTermMatchDialStringCB() callback with a value
              RV_MDMDIGITMAP_UNAMBIGUOUSMATCH or RV_MDMDIGITMAP_NOMATCH) or sent directly
              by the user (if the user does the match asynchronously). For the UI
              termination, it is caused by the kp/ce event. For the Analog termination,
              it is caused by the dd/ce event. */

    RV_CCTERMEVENT_MAKECALL,
        /* 8. Internal Call Control event. It initiates an incoming call in the
              connection. */

    RV_CCTERMEVENT_RINGBACK,
        /* 9. Internal Call Control event. The outgoing call has reached the other end. */

    RV_CCTERMEVENT_RINGING,
        /* 10. Internal Call Control event. An incoming call is transitioning to the
               ALERTING state. */

    RV_CCTERMEVENT_CALLANSWERED,
        /* 11. Internal Call Control event. An outgoing call has been answered in the
               destination end and is moving to a CONNECTED state. */

    RV_CCTERMEVENT_ONHOOK,
        /* 12. The user went on-hook, thereby disconnecting the call. For the UI
               termination this can be caused by a kf/ku event with keyid of kh,
               a al/on event, or a line event (kf/ku with keyid l00n) if the line
               was already in a call. For the Analog termination it is caused by
               the al/on event. */

    RV_CCTERMEVENT_HOLD,
        /* 13. Internal hold event propagated to the Connection State Machine.
               This can originate, for example, from a user pressing the Hold key. */

    RV_CCTERMEVENT_MUTE,
        /* 14. The user pressed the Mute key. Currently not implemented. */

    RV_CCTERMEVENT_HOLDKEY,
        /* 15. The user pressed the Hold key. Caused by a kf/ku event with keyid of kl. */

    RV_CCTERMEVENT_UNHOLD,
        /* 16. Internal Unhold event propagated to the Connection State Machine. This
               can originate, for example, from a user pressing the Line key on a line
               in a HELD state. */

    RV_CCTERMEVENT_CONFERENCE,
        /* 17. The user pressed the Conference key. Caused by a kf/ku event with
               keyid of kc. The first time will activate an additional line to call
               the added party. The second time will connect all parties in the
               conference. */

    RV_CCTERMEVENT_TRANSFER,
        /* 18. The user pressed the Transfer key. Caused by a kf/ku event with keyid
               of kt. The first time will activate an additional line to call the
               transfer destination. The second time will complete the transfer and
               drop the user from the transfer. */

    RV_CCTERMEVENT_LINE,
        /* 19. The user pressed the Line key. Caused by a kf/ku event with keyid of l00n. */

    RV_CCTERMEVENT_LINEOTHER,
        /* 20. This is an internal event. The user pressed the Line key, and there is a
               connected call (either on hold or not) in a different line. */

    RV_CCTERMEVENT_HEADSET,
        /* 21. The user pressed the Headset key. Caused by a kf/ku event with keyid of ht. */

    RV_CCTERMEVENT_HANDSFREE,
        /* 22. The user pressed the Handsfree key. Caused by a kf/ku event with keyid of hf. */

    RV_CCTERMEVENT_AUDIOHANDSET,
        /* 23. This is an internal event. The event indicates the Call Control to make the
               Handset the active audio termination, meaning to move the media from the
               current active termination to the new one, etc. */

    RV_CCTERMEVENT_AUDIOHANDSFREE,
        /* 24. This is an internal event. The event indicates to the state machine to
               make the Speaker the active audio termination, i.e., to move the media
               from the current active termination to the new one, etc. */

    RV_CCTERMEVENT_FAILGENERAL,
        /* 25. This is an internal event. The event indicates a general failure, while
               the reason code specifies the reason for the failure. */

    RV_CCTERMEVENT_MEDIAOK,
        /* 26. Internal Call Control event. Creating or modifying a media stream on a
               termination has succeeded. */

    RV_CCTERMEVENT_MEDIAFAIL,
        /* 27. Internal Call Control event. Creating or modifying a media stream on a
               termination has failed. */

    RV_CCTERMEVENT_DISCONNECTING,
        /* 28. Internal Call Control event. Causes the connection to disconnect
               from the call. */

    RV_CCTERMEVENT_DISCONNECTED,
        /* 29. Internal Call Control event. The connection has been disconnected.
               Release resources. */

    RV_CCTERMEVENT_INCOMINGCALL,
        /* 30. This is an internal event. The event is usually caused by an incoming
               signaling message indicating the establishment of a new call. */

    RV_CCTERMEVENT_REJECTCALL,
        /* 31. Internal Call Control event. Rejects an incoming call before the OFFERED
               state. For example, because there are no available lines. */

    RV_CCTERMEVENT_TRANSFER_INIT,
        /* 32. This event indicates to transferring endpoint (A) to start the Transfer
               process, i.e., to send the transferree endpoint (B) a signaling message
               indicating that it should establish a call with the Transfer destination
               endpoint.*/

    RV_CCTERMEVENT_TRANSFER_OFFERED,
        /* 33. Internal Call Control event. An incoming transferred call is being offered
               to the connection. This connection represents the transfer destination.
               The call will replace the existing call with the transferring party. */

    RV_CCTERMEVENT_REMOTE_DISCONNECTED,
        /* 34. Internal Call Control event. The other party has disconnected the call. */

    RV_CCTERMEVENT_ONHOOK_OTHER,
        /* 35. Internal Call Control event. */

    RV_CCTERMEVENT_REJECT_KEY,
        /* 36. The user has rejected an incoming call on a given line. Caused by
               rvcc/reject event. */

    RV_CCTERMEVENT_MEDIANOTACCEPTED,
        /* 37. This is an internal event. The event indicates that the media offered
               by the remote party is not supported by the local party. Processing
               the event will result in a local warning tone and an outgoing signaling
               message, indicating that the call is rejected with reason 415—Media
               Not Supported for SIP. */

    RV_CCTERMEVENT_MODIFYMEDIA,
        /* 38. This event is relevant for SIP Phone only. The event indicates the
               beginning of a dynamic media change process and is caused by the user
               application calling rvMdmTermModifyMedia(). Processing the event will
               result in a Re-Invite message being sent that includes the new media. */

    RV_CCTERMEVENT_MODIFYMEDIA_DONE,
        /* 39. This is an internal event. The event is processed when the state machine
               has finished processing a dynamic media change and will result in notifying
               the user application about whether or not the process has been completed
               successfully. */

    RV_CCTERMEVENT_BLIND_TRANSFER,
        /* 40. The user pressed the Blind Transfer key. Caused by a kf/ku event with
               keyid of kbt. */

    RV_CCTERMEVENT_REDIAL,
        /* 41. This event is sent to activate Redial functionality. */

    RV_CCTERMEVENT_CFW,
        /* 42. This event is sent to activate Call Forward functionality. */

    RV_CCTERMEVENT_USER
        /* All values higher than this value can be used by the user and will be ignored by
           the Call Control. */
} RvCCTerminalEvent;


/* RvCCEventCause
 * ------------------------------------------------------------------------
 * The event reason field provides additional information to the Call Control
 * about the events.
 * These reasons are indicated by RvIppMdmPreProcessEventCB(),
 * RvIppMdmPostProcessEventCB() and RvIppMdmDisplayCB().
 */
typedef enum
 {
    RV_CCCAUSE_INCOMING_CALL,
        /* 0. The call is an incoming call. */

    RV_CCCAUSE_OUTGOING_CALL,
        /* 1. The call is an outgoing call. */

    RV_CCCAUSE_CALL_WAITING,
        /* 2. The incoming call is a call waiting (there is an active call in
              the terminal already). */

    RV_CCCAUSE_BUSY,
        /* 3. The call is not completed because the party is busy (no lines available). */

    RV_CCCAUSE_NOT_FOUND,
        /* 4. Reason for rejecting an incoming call - destination was not found. */

    RV_CCCAUSE_REORDER_TONE,
        /* 5. Indicates a general failure, will cause a warning tone to be heard. */

    RV_CCCAUSE_TRANSFER,
        /* 6. The reason for the call is transfer. */

    RV_CCCAUSE_UNHOLD,
        /* 7. Call was released from Hold. */

    RV_CCCAUSE_CALL_CANCELLED,
        /* 8. Call was canceled by the local party. */

    RV_CCCAUSE_LOCAL_HOLD,
        /* 9. The call was put on hold by the local user. */

    RV_CCCAUSE_REMOTE_HOLD,
        /* 10. The call was put on Hold by the remote party. The event is caused by an
               incoming signaling message indicating that the call should be put on Hold. */

    RV_CCCAUSE_NEW_CALL,
        /* 11. The call is a new call. */

    RV_CCCAUSE_NORMAL,
        /* 12. This reason is used when no specific reason is required. */

    RV_CCCAUSE_RESOURCES_NOT_AVAILABLE,
        /* 13. Operation failed due to lack of resources. */

    RV_CCCAUSE_MEDIA_NOT_SUPPORTED,
        /* 14. The phone does not support the required media parameters. */

    RV_CCCAUSE_EVENT_BEGIN,
        /* 15. This reason indicates that Key Down was pressed for a digit. */

    RV_CCCAUSE_EVENT_END,
        /* 16. This reason indicates that Key Up was pressed for a digit. */

    RV_CCCAUSE_OPERATION_SUCCEEDED,
        /* 17. The process ended successfully. */

    RV_CCCAUSE_OPERATION_FAILED,
        /* 18. The process ended with failure. */

    RV_CCCAUSE_AUTH_FAIL,
        /* 19. (SIP only) Authentication failed. */

    RV_CCCAUSE_UNKNOWN
        /* 20. Reason is unknown. */
} RvCCEventCause;


/* RvCCTerminalType
 * ------------------------------------------------------------------------
 * The type associated with a given terminal.
 */
typedef enum
{
	RV_CCTERMINALTYPE_UNKNOWN,
        /* The terminal type is unknown */

	RV_CCTERMINALTYPE_EPHEMERAL,
        /* This is an ephemeral terminal, usually associated with an RTP session */

	RV_CCTERMINALTYPE_ANALOG,
        /* This terminal is physical terminal, associated with an analog line */

	RV_CCTERMINALTYPE_UI,
        /* A user interface termination, associated with keys, LCD displays, lights,
           etc */

#ifdef RV_MTF_VIDEO
	RV_CCTERMINALTYPE_VT,
        /* Video transducer termination, representing a video device in the phone */

#endif
	RV_CCTERMINALTYPE_AT
        /* Audio transducer termination, representing an audio device in the phone */
} RvCCTerminalType;


typedef enum {
	RV_CCTERMAUDIO_NONE,
	RV_CCTERMAUDIO_HANDSET,
	RV_CCTERMAUDIO_HANDSFREE,
	RV_CCTERMAUDIO_HEADSET
} RvCCTerminalAudioType;








#endif /*RV_RVCCAPI_H*/
