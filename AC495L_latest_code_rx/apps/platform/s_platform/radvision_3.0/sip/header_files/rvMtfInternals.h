/******************************************************************************
Filename:    rvMtfInternals.h
Description: This file includes the internal APIs and type definitions.
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

#ifndef RV_DEPR_TYPES_H
#define RV_DEPR_TYPES_H

#include "rvmdm.h"
#include "rvCallControlApi.h"
#include "rvSipControlApi.h"
#include "rvmdmmediastream.h"



/*==================================================================================
					 T Y P E S		F R O M		rvCallControlApi.h
===================================================================================*/

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

/*===========================*/
/* AudioTerm States - Bitmap */
/*===========================*/
#define RV_CCAUDIOTERM_PASSIVE    0   /*00000000*/
#define RV_CCAUDIOTERM_HS_ACTIVE  1   /*00000001*/
#define RV_CCAUDIOTERM_HT_ACTIVE  2   /*00000010*/
#define RV_CCAUDIOTERM_HF_ACTIVE  4   /*00000100*/

typedef int RvCCAudioTermState;


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
	RV_CCMEDIASTATE_MODIFYING,			/*	6 */ 
	RV_CCMEDIASTATE_MODIFYING_BEFORE_CONNECTED, /* 7 */
	RV_CCMEDIASTATE_FAILED				/*  8 */
} RvCCMediaState;

typedef enum {
	RV_CCTERMAUDIO_NONE,
	RV_CCTERMAUDIO_HANDSET,
	RV_CCTERMAUDIO_HANDSFREE,
	RV_CCTERMAUDIO_HEADSET
} RvCCTerminalAudioType;


/*==================================================================================
					 T Y P E S		F R O M		rvSipControlApi.h
===================================================================================*/

/* RvIppSipControlHandle
 * ----------------------------------------------------------------------------
 * This type is DEPRECATED!!! SIP Control object handle. 
 * SIP Control handle enables the user application with better control of the
 * SIP Stack in changing or adding to the default functionality of the
 * Multimedia Terminal Framework.
 */
RV_DECLARE_HANDLE(RvIppSipControlHandle);

/* RvIppSipTerminalHandle
 * ----------------------------------------------------------------------------
 * This type is DEPRECATED!*/
RV_DECLARE_HANDLE(RvIppSipTerminalHandle);

/*****************************************************************************
 * RvIppSipStackConfigCB 
 * -----------------------------------------------------------------------------
 * General:
 *			This function is DEPRECATED!!! use RvMtfChangeSipStackConfigEv instead.
 *			This is a prototype for Callback to configure the SIP stack.
 *			When this function is called, pStackCfg is filled with default values, 
 *			which the user can change.
 *			After this function returns, validation checking is done on the values, 
 *			by both IPP TK and SIP stack.
 *			Invalid values will be changed to default values, and unsupported parameters 
 *			will be ignored.
 *
 * Arguments:
 *
 * Input:  pStackCfg    -     pointer to stack configuration object.
 *
 * Output: None.
 *
 * Return Value: None.
 ****************************************************************************/
typedef void (*RvIppSipStackConfigCB)(IN RvSipStackCfg* pStackCfg);

/*****************************************************************************
 * RvIppSipRegisterStackEventsCB 
 * -----------------------------------------------------------------------------
 * General:
 *		   This function is DEPRECATED!!! use RvMtfRegisterSipStackEventsEv instead.
 *         This is a prototype for registering callbacks to the SIP stack.
 *         This callback enables the user to listen for stack events that IPP TK doesn't.
 *
 * Arguments:
 *
 * Input:  sipControl   -       pointer to SipControl structure
 *         stackHandle  -       pointer to sip stack handle.
 *
 * Output: None.
 *
 * Return Value: None.
 ****************************************************************************/
typedef void (*RvIppSipRegisterStackEventsCB)(IN RvIppSipControlHandle  sipMgrHndl);

/*****************************************************************************
 * RvIppSipPreCallLegCreatedIncomingCB 
 * -----------------------------------------------------------------------------
 * General:
 *		   This function is DEPRECATED!!! use RvMtfPreCallCreatedIncomingEv instead.
 *         This is a prototype for processing SIP incoming new calls.
 *         This callback is called before MTF processes a new incoming call.
 *
 * Arguments:
 *
 * Input:  sipMgrHndl    -     pointer to sipMgr handle.
 *       . hCallLeg      -     Call-Leg handle
 *         phAppCallLeg  -     Application call-leg handle
 *         userData      -     user application data.
 *
 * Output: None.
 *
 * Return Value: rvFalse - if you don't want the IPP to perform its default processing.
 *               rvTrue  - continue as usual
 ****************************************************************************/
typedef RvBool (*RvIppSipPreCallLegCreatedIncomingCB)(
    IN  RvIppSipControlHandle   sipMgrHndl,
    IN  RvSipCallLegHandle      hCallLeg,
    OUT RvSipAppCallLegHandle*  phAppCallLeg,
    IN  void*                   userData);

/*****************************************************************************
 * RvIppSipPostCallLegCreatedIncomingCB 
 * -----------------------------------------------------------------------------
 * General:
 *		   This function is DEPRECATED!!! use RvMtfPostCallCreatedIncomingEv instead.
 *         This is a prototype for processing SIP incoming new calls.
 *         This callback is called just after an incoming call in the SIP Stack
 *          begins to be handled by the Multimedia Terminal Framework. 
 *
 * Arguments:
 *
 * Input:  sipMgrHndl  - Pointer to sipMgr handle. 
 *       . hCallLeg    - Call-Leg handle.
 *         hLine -		Connection object associated with this call leg.
 *         userData    - The user-specified data associated with the call.
 *
 * Output: None.
 *
 * Return Value: None.
 ****************************************************************************/
typedef void (*RvIppSipPostCallLegCreatedIncomingCB)(
    IN RvIppSipControlHandle    sipMgrHndl,
    IN RvSipCallLegHandle       hCallLeg,
    IN RvIppConnectionHandle    hLine,
    IN void*                    userData);

/*****************************************************************************
 * RvIppSipPreCallLegCreatedOutgoingCB 
 * -----------------------------------------------------------------------------
 * General:
 *		   This function is DEPRECATED!!! use RvMtfPreCallCreatedOutgoingEv instead.
 *         This is a prototype for processing SIP outgoing new calls.
 *         This callback is called before IPP TK build headers to call-leg object,
 *         and will use "to" and "from" values returned from user to build TO and 
 *		   FROM headers.
 *
 * Arguments:
 *
 * Input:  sipMgrHndl   -      pointer to sipMgr handle.
 *       . hCallLeg     -      Call-Leg handle
 *         to           -      destination address
 *         from         -      source address
 *         userData     -      user application data.
 *
 * Output: None.
 *
 * Return Value: None.
 ****************************************************************************/
typedef void (*RvIppSipPreCallLegCreatedOutgoingCB)(
    IN RvIppSipControlHandle    sipMgrHndl,
    IN RvSipCallLegHandle       hCallLeg,
    IN RvChar*                  to,
    IN RvChar*                  from,
    IN void*                    userData);


/*****************************************************************************
 * RvIppSipPostCallLegCreatedOutgoingCB 
 * -----------------------------------------------------------------------------
 * General:
 *		   This function is DEPRECATED!!! use RvMtfPostCallCreatedOutgoingEv instead.
 *         Prototype for SipPostCallLegCreatedOutgoingCB.
 *                   This callback is called after IPP TK built headers
 *                   in call-leg object, and before callMake() was called.
 *                   User can change any of the headers in call-leg object.
 *
 * Arguments:
 *
 * Input:  sipMgrHndl  - Pointer to sipMgr handle. 
 *       . hCallLeg    - Call-Leg handle.
 *         hLine -		Connection object associated with this call leg.
 *
 * Output: None.
 *
 * Return Value: None.
 ****************************************************************************/
typedef void (*RvIppSipPostCallLegCreatedOutgoingCB)(
    IN RvIppSipControlHandle    sipMgrHndl,
    IN RvSipCallLegHandle       hCallLeg,
    IN void*                    userData);

/*****************************************************************************
 * RvIppSipPreStateChangedCB 
 * -----------------------------------------------------------------------------
 * General:
 *		   This function is DEPRECATED!!! use RvMtfPreCallLegStateChangedEv instead.
 *         This callback is called when the SIP Stack event
 *         RvSipCallLegStateChangedEv() is invoked, before the Multimedia
 *         Terminal Framework handles a SipCallLeg state machine change.
 *
 * Arguments:
 *
 * Input:  sipMgrHndl  - Pointer to sipMgr handle.
 *       .  hCallLeg    - Call-Leg handle.
 *          hLine -		Connection object associated with this call leg.
 *          eState      - CallLeg state.
 *          eReason     - State change reason.
 *          userData    - The user-specified data associated with the call.
 *
 * Output: None.
 *
 * Return Value: RV_FALSE if the application does not want the Multimedia
 *                Terminal Framework to perform its default processing.
 *                RV_TRUE to continue as usual.
 ****************************************************************************/
typedef RvBool (*RvIppSipPreStateChangedCB)(
    IN RvIppSipControlHandle            sipMgrHndl,
    IN RvSipCallLegHandle               hCallLeg,
    IN RvIppConnectionHandle            hLine,
    IN RvSipCallLegState                eState,
    IN RvSipCallLegStateChangeReason    eReason,
    IN void*                            userData);

/*****************************************************************************
 * RvIppSipPostCallLegStateChangedCB 
 * -----------------------------------------------------------------------------
 * General:
 *		   This function is DEPRECATED!!! use RvMtfPostCallLegStateChangedEv() instead.
 *         Prototype for SipPostStateChangedCB.
 *         This callback is called after a SipCallLeg state machine change
 *
 * Arguments:
 *
 * Input:  sipMgrHndl     -    pointer to sipMgr handle.
 *       .  hCallLeg      -     Call-Leg handle
 *          phAppCallLeg  -     Application call-leg handle
 *          eState        -     CallLeg state
 *          eReason       -     state change treason
 *          userData      -     user application data.
 *
 * Output: None.
 *
 * Return Value: None.
 ****************************************************************************/
typedef void (*RvIppSipPostStateChangedCB)(
    IN  RvIppSipControlHandle               sipMgrHndl,
    IN  RvSipCallLegHandle				    hCallLeg,
    IN RvIppConnectionHandle				hLine,
    IN  RvSipCallLegState			        eState,
    IN  RvSipCallLegStateChangeReason       eReason,
    IN  void*                               userData);

/*****************************************************************************
 * RvIppSipPreMsgToSendCB 
 * -----------------------------------------------------------------------------
 * General:
 *		   This function is DEPRECATED!!! 
 *         This callback is invoked before the Multimedia Terminal Framework
 *            modifies the message that is about to be sent. Enables the
 *            application to change the outgoing SIP message. 
 *
 * Arguments:
 *
 * Input:  sipMgrHndl   - Pointer to the sipMgr handle. 
 *       .  hCallLeg     - Call-Leg handle.
 *          hLine  - Connection object associated with this call leg.
 *          hMsg         - SIP message handle.
 *          userData     - The user-specified data associated with the call.
 *
 * Output: None.
 *
 * Return Value: RV_FALSE if the application does not want the Multimedia
 *                Terminal Framework to perform its default processing.
 *                RV_TRUE to continue as usual. 
 ****************************************************************************/
typedef RvBool (*RvIppSipPreMsgToSendCB)(
    IN RvIppSipControlHandle    sipMgrHndl,
    IN RvSipCallLegHandle       hCallLeg,
    IN RvIppConnectionHandle    hLine,
    IN RvSipMsgHandle           hMsg,
    IN void*                    userData);

/*****************************************************************************
 * RvIppSipPostMsgToSendCB 
 * -----------------------------------------------------------------------------
 * General:
 *		   This function is DEPRECATED!!! use RvMtfMsgToSendEv instead.
 *         This callback is invoked before the Multimedia Terminal Framework
 *            modifies the message that is about to be sent. Enables the application
 *            to change the outgoing SIP message. 
 *
 * Arguments:
 *
 * Input:  sipMgrHndl   - Pointer to the sipMgr handle. 
 *       .  hCallLeg     - Call-Leg handle.
 *          hLine  - Connection object associated with this call leg.
 *          hMsg         - SIP message handle.
 *          userData     - The user-specified data associated with the call.
 *
 * Output: None.
 *
 * Return Value: RV_FALSE if the application does not want the Multimedia
 *                Terminal Framework to perform its default processing.
 *                RV_TRUE to continue as usual. 
 ****************************************************************************/
typedef void (*RvIppSipPostMsgToSendCB)(
    IN RvIppSipControlHandle    sipMgrHndl,
    IN RvSipCallLegHandle       hCallLeg,
    IN RvIppConnectionHandle    hLine,
    IN RvSipMsgHandle           hMsg,
    IN void*                    userData);

/*****************************************************************************
 * RvIppSipPreMsgReceivedCB 
 * -----------------------------------------------------------------------------
 * General:
 *		   This function is DEPRECATED!!! use RvMtfPreMsgReceivedEv instead.
 *         This callback is invoked before the Multimedia Terminal Framework modifies
 *			the message received. It enables the application to change the incoming SIP
 *			message. It enables the application to change incoming SIP message and to
 *			decide whether messages should be ignored or processed by the SIP Stack or
 *			the Multimedia Terminal Framework. 
 *
 * Arguments:
 *
 * Input:  sipMgrHndl   - Pointer to the sipMgr handle. 
 *       .  hCallLeg     - Call-Leg handle.
 *          hLine  - Connection object associated with this call leg.
 *          hMsg         - SIP message handle.
 *          userData     - The user-specified data associated with the call.
 *
 * Output: None.
 *
 * Return Value:    - RvMtfMsgProcessType can be set the one of the following values: 
 *					- RV_MTF_IGNORE_BY_STACK—This value indicates to both SIP Stack and
 *					  Multimedia Terminal Framework to ignore the message. When this value is
 *					  returned, callback RvIppSipPreCallLegCreatedIncomingCB() should return
 *					  False as well, otherwise a memory leak will occur (when
 *					  RvIppSipPreCallLegCreatedIncomingCB() is called resources (of SIP
 *					  connection) will be allocated but not be released). 
 *					- RV_MTF_IGNORE_BY_MTF—This value indicates to the Multimedia Terminal
 *					  Framework to ignore the message, but message will still be processed by
 *					  the SIP Stack. 
 *					- RV_MTF_DONT_IGNORE—This value indicates both SIP Stack and Multimedia
 *					  Terminal Framework to process the message. 
 ****************************************************************************/
typedef RvMtfMsgProcessType (*RvIppSipPreMsgReceivedCB)(
    IN RvIppSipControlHandle    sipMgrHndl,
    IN RvSipCallLegHandle       hCallLeg,
    IN RvIppConnectionHandle    hLine,
    IN RvSipMsgHandle           hMsg,
    IN void*                    userData);

/*****************************************************************************
 * RvIppSipPostMsgReceivedCB 
 * -----------------------------------------------------------------------------
 * General:
 *		   This function is DEPRECATED!!! use RvMtfPostMsgReceivedEv instead.
 *         This callback is invoked before the Multimedia Terminal Framework modifies
 *  the received message. It enables the application to change the incoming SIP message. 
 *
 * Arguments:
 *
 * Input:  sipMgrHndl   - Pointer to the sipMgr handle. 
 *       .  hCallLeg     - Call-Leg handle.
 *          hLine  - Connection object associated with this call leg.
 *          hMsg         - SIP message handle.
 *          userData     - The user-specified data associated with the call.
 *
 * Output: None.
 *
 * Return Value:  None.
 ****************************************************************************/
typedef void (*RvIppSipPostMsgReceivedCB)(
    IN RvIppSipControlHandle    sipMgrHndl,
    IN RvSipCallLegHandle       hCallLeg,
    IN RvIppConnectionHandle    hLine,
    IN RvSipMsgHandle           hMsg,
    IN void*                    userData);

/*****************************************************************************
 * RvIppSipPreRegClientStateChangedCB 
 * -----------------------------------------------------------------------------
 * General:
 *		   This function is DEPRECATED!!! use RvMtfRegClientStateChangedEv instead.
 *        This callback is invoked before the MTF handles registration state changes
 *			of the SIP User Agent.
 *
 * Arguments:
 *
 * Input:  sipMgrHndl       - Pointer to sipMgr handle.
 *         hRegClient       - SIP registration client instance.
 *         mdmTerminalHndl  - The terminal handle used.
 *         eState           - SIP registration client state
 *         eReason          - Reason for the state changed that occurred.
 *         userData         - The userData provided by the application in its
 *                            RvIppSipExtClbks struct.
 *
 * Output: None.
 *
 * Return Value:  RV_FALSE if the application doesn't want the MTF to perform any
 *               default processing due to the state change.
 *               RV_TRUE to continue as usual.
 ****************************************************************************/
typedef RvBool (*RvIppSipPreRegClientStateChangedCB)(
    IN RvIppSipControlHandle            sipMgrHndl,
    IN RvSipRegClientHandle             hRegClient,
    IN RvIppTerminalHandle              mdmTerminalHndl,
    IN RvSipRegClientState              eState,
    IN RvSipRegClientStateChangeReason  eReason,
    IN void*                            userData);


/*****************************************************************************
 * RvIppSipRegExpResolutionNeededCB 
 * -----------------------------------------------------------------------------
 * General:
 *		   This function is DEPRECATED!!! use RvMtfRegExpResolutionNeededEv instead.
 *        This callback is invoked before the MTF receives RvSipTransmitterMgrRegExpResolutionNeededEv
 *		  event from SIP stack. It will be called when Tel-URI scheme ("tel:") is returned in
 *		  callback RvMdmTermMapDialStringToAddressCB(), and Outbound proxy is not configured. 
 *
 * Arguments:
 *
 * Input:  sipMgrHndl     - Pointer to sipMgr handle.
 *        hTrxMgr        - Transmitter manager
 *        pAppTrxMgr     - A handle to an application's context
 *        hTrx           - handle to transmitter (NULL if the 
 *                         transmitter is not an application transmitter)
 *        hLine    - Handle to MTF call associated with this callback
 *        userData       - user application data associated with MTF extention callbacks
 *
 * Output: pRegExpParams  - a struct that holds the information for the regular expression application
 *                         The pMatchArray should be filled in by 
 *                         RvSipTransmitterRegExpResolutionNeededEv with substring 
 *                         match addresses.  Any unused structure elements 
 *                         should contain the value -1.
 *                         Each startOffSet element that is not -1 indicates 
 *                         the start offset of the next largest substring match 
 *                         within the string. The relative endOffSet element 
 *                         indicates the end offset of the match. 
 *
 * Return Value:  None.
 *****************************************************************************/
typedef RvStatus     (*RvIppSipRegExpResolutionNeededCB)(
    IN RvIppSipControlHandle                        sipMgrHndl,
    IN  RvSipTransmitterMgrHandle                   hTrxMgr,
    IN  void*                                       pAppTrxMgr,
    IN  RvSipTransmitterHandle                      hTrx,
    IN  RvIppConnectionHandle                       hLine,
    INOUT RvSipTransmitterRegExpResolutionParams*   pRegExpParams,
    IN void*                                        userData);

/*****************************************************************************
 * Type: RvIppSipExtClbks 
 * -----------------------------------------------------------------------------
 * Description: This structure contains SIP stack callbacks which user application 
 *              can implement to gain control over SIP stack.
 *****************************************************************************/
typedef struct
{
    RvIppSipStackConfigCB                   stackConfigF;
    RvIppSipRegisterStackEventsCB           registerStackEventsF;
    RvIppSipPreCallLegCreatedIncomingCB     preCallLegCreatedIncomingF;
    RvIppSipPostCallLegCreatedIncomingCB    postCallLegCreatedIncomingF;
    RvIppSipPreCallLegCreatedOutgoingCB     preCallLegCreatedOutgoingF;
    RvIppSipPostCallLegCreatedOutgoingCB    postCallLegCreatedOutgoingF;
    RvIppSipPreStateChangedCB               preStateChangedF;
    RvIppSipPostStateChangedCB              postStateChangedF;
    RvIppSipPreMsgToSendCB                  preMsgToSendF;
    RvIppSipPostMsgToSendCB                 postMsgToSendF;
    RvIppSipPreMsgReceivedCB                preMsgReceivedF;
    RvIppSipPostMsgReceivedCB               postMsgReceivedF;
    RvIppSipPreRegClientStateChangedCB      PreRegClientStateChangedF;
    RvIppSipRegExpResolutionNeededCB		transmitterRegExpResolutionNeededF;
    void*                                   userData;

} RvIppSipExtClbks;



/******************************************************************************
*  rvIppSipSystemInit
*  ------------------
*  General :        This function is DEPRECATED!!! use rvMtfSystemInit() instead.
*					Initializes IPP elements which needs to be ready before
*                   constructing IPP TK -
*                   1. Initialize core
*                   2. Reset extension APIs structure.
*                   This function will be used to initialize IPP TK, and should
*                   be the first function to be called.
*
*  Return Value:   None
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          None
*
*  Output:         None
******************************************************************************/
RVAPI void RVCALLCONV rvIppSipSystemInit(void);

/******************************************************************************
 * rvIppSipSystemEnd 
 * -----------------------------------------------------------------------------
 * General:
 *				This function is DEPRECATED!!! use rvMtfSystemEnd() instead.
 *				Shutdown MTF System - end core.
 *				This function will be used to shutdown MTF, and should be
 *				the last function to be called.
 *
 * Arguments:
 *
 * Input:  None.
 *
 * Output: None.
 *
 * Return Value:  None.
 *****************************************************************************/
RVAPI void RVCALLCONV rvIppSipSystemEnd(void);

/******************************************************************************
 * rvIppSipInitConfig 
 * -----------------------------------------------------------------------------
 * General:
 *			This function is DEPRECATED!!! use rvMtfInitConfig() instead.
 *			Initializes configuration structure.
 *          Currently it only fills the structure with zeros, and later
 *          it will be filled with default values by IPP and by the stack.
 *          Should be called before rvIppSipStackInitialize().
 *
 * Arguments:
 *
 * Input:  cfg   -  Configuration structure
 *
 * Output: None.
 *
 * Return Value:  True if successful, False if failed
 *****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppSipInitConfig(
				OUT RvIppSipPhoneCfg*    cfg);


/******************************************************************************
 * rvIppSipStackInitialize 
 * -----------------------------------------------------------------------------
 * General:
 *			This function is DEPRECATED!!! use rvMtfInitConfig() instead.
 *			Initializes configuration structure.
 *          Currently it only fills the structure with zeros, and later
 *          it will be filled with default values by IPP and by the stack.
 *          Should be called before rvIppSipStackInitialize().
 *
 * Arguments:
 *
 * Input:  cfg   -  Configuration structure
 *
 * Output: None.
 *
 * Return Value:  True if successful, False if failed
 *****************************************************************************/
RVAPI RvBool RVCALLCONV rvIppSipStackInitialize(
    OUT RvSipStackHandle* stackHandle,
    IN  RvIppSipPhoneCfg* cfg);


/******************************************************************************
*  rvIppSipRegisterExtClbks
*  --------------------------------
*  General :        This function is DEPRECATED!!! use rvMtfRegisterSipExtCallbacks() instead.
*					This function registers SIP extensibility callbacks.
*                   This function should be called before rvIppSipStackInitialize().
*
*  Return Value:   None
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          clbks              Structure includes pointers to user implementations
*
*  Output:         none.
******************************************************************************/
RVAPI void RVCALLCONV rvIppSipRegisterExtClbks(IN RvIppSipExtClbks* clbks);



/******************************************************************************
*  rvIppSipControlMsgSetSdpBody
*  --------------------------------
*  General :         This function is DEPRECATED!!! 
*					 This function adds SDP body to a SIP message.
*
*  Return Value:    True, if successful.
*                           False if failed.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:               hMsg            Handle to SIP message
*                           sdpMsg      pointer to SDP structure
*
*  Output:          None.
******************************************************************************/
RVAPI RvBool RVCALLCONV rvIppSipControlMsgSetSdpBody(
    IN RvSipMsgHandle   hMsg,
    IN const RvSdpMsg*  sdpMsg);

/******************************************************************************
*  rvIppSipControlGetCallLeg
*  --------------------------------
*  General :          This function is DEPRECATED!!! use rvMtfGetSipCallLeg() instead.
*					  Returns a handle to SIP stack Call leg associated with
*                     the given connection.
*  Return Value:      a handle to SIP stack Call leg
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:               connHndl         Handle to SIP connection
*
*  Output:          None
******************************************************************************/
RVAPI RvSipCallLegHandle RVCALLCONV rvIppSipControlGetCallLeg(
    IN RvIppConnectionHandle   connHndl);

/******************************************************************************
 * rvIppSipPhoneConstruct 
 * -----------------------------------------------------------------------------
 * General:
 *        THIS FUNCTION IS DEPRECATED!!!
 *        Use rvMtfSipConstruct() instead.
 *
 * Arguments:
 *
 * Input:  mgr       -         pointer to MdmTermination Manager
 *         cfg       -         pointer to configuration data
 *         sipStack  -         sip stack handle
 *
 * Output: None.
 *
 * Return Value:  None.
 *****************************************************************************/
RVAPI void RVCALLCONV rvIppSipPhoneConstruct(
    OUT RvMdmTermMgr*       mgr,
    IN  RvIppSipPhoneCfg*   cfg,
    IN  RvSipStackHandle    sipStack);

/******************************************************************************
*  rvIppSipControlGetStackCallbacks
*  --------------------------------
*  General :          This function is DEPRECATED!!! 
*					  This function returns a list of stack callbacks used by MTF.
*
*  Return Value:  transport type.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:               sipMgrHndl    -      Handle to SipControl object
*
*  Output:          RvIppSipStackCallbacks*.
******************************************************************************/
RVAPI RvIppSipStackCallbacks* RVCALLCONV rvIppSipControlGetStackCallbacks(
    IN RvIppSipControlHandle sipMgrHndl);

/******************************************************************************
*  rvIppSipControlGetRegistrarAddress
*  --------------------------------
*  General :       This function is DEPRECATED!!! 
*				   The function returns ip address of Registrar.
*
*  Return Value:   None
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:                sipMgrHndl           Handle to SipControl object
*
*  Output:             registrarAddress       address of Registrar
*                            registrarAddressLen    length of registrarAddress string allocated
*                                       by application
******************************************************************************/
RVAPI void RVCALLCONV rvIppSipControlGetRegistrarAddress(
    IN  RvIppSipControlHandle   sipMgrHndl,
    OUT RvChar*                 registrarAddress,
    IN  RvSize_t                registrarAddressLen);

/******************************************************************************
*  rvIppSipControlGetUserDomain
*  ----------------------------
*  General :        This function is DEPRECATED!!! 
*				    The function returns domain of Registrar
*
*  Return Value:    None
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:               sipMgrHndl          Handle to SipControl object
*
*  Output:              userDomain          domain of the User
*                       userDomainLen       length of userDomain string
******************************************************************************/
RVAPI void RVCALLCONV rvIppSipControlGetUserDomain(
    IN  RvIppSipControlHandle   sipMgrHndl,
    OUT RvChar*                 userDomain,
    IN  RvSize_t                userDomainLen);

/******************************************************************************
*  rvIppSipControlGetOutboundProxyAddress
*  --------------------------------
*  General :        This function is DEPRECATED!!! 
*				    The function returns ip address of Outbound Proxy
*
*  Return Value:    None
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           sipMgrHndl              Handle to SipControl object
*
*  Output:          outboundProxyAddress    address of Outbound Proxy
*                   outboundProxyAddressLen length of outboundProxyAddress string allocated
*                                            by application
******************************************************************************/
RVAPI void RVCALLCONV rvIppSipControlGetOutboundProxyAddress(
    IN  RvIppSipControlHandle   sipMgrHndl,
    OUT RvChar*                 outboundProxyAddress,
    IN  RvSize_t                outboundProxyAddressLen);

/******************************************************************************
*  rvIppSipControlGetExtUserData
*  --------------------------------
*  General :        This function is DEPRECATED!!! 
*				    The function returns user data that was set by user when extension
*                   callbacks were registered.
*
*  Return Value:     User data.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:                  sipMgrHndl                   Handle to SipControl object
*
*  Output:             None.
******************************************************************************/
RVAPI void* RVCALLCONV rvIppSipControlGetExtUserData(IN RvIppSipControlHandle sipMgrHndl);


/******************************************************************************
*  rvIppSipControlGetTransportType
*  --------------------------------
*  General :        This function is DEPRECATED!!! 
*				    The function returns transport type.
*
*  Return Value:  transport type.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:               sipMgrHndl          Handle to SipControl object
*
*  Output:          None.
******************************************************************************/
RVAPI RvSipTransport RVCALLCONV rvIppSipControlGetTransportType(
    IN RvIppSipControlHandle    sipMgrHndl);

/******************************************************************************
*  rvIppSipControlGetStackHandle
*  --------------------------------
*  General :      This function is DEPRECATED!!! use rvMtfGetSipStackHandle() instead.
*				  This function returns stack handle stored inside SipControl object.
*
*  Return Value:  transport type.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:               sipMgrHndl          Handle to SipControl object
*
*  Output:          stack handle
******************************************************************************/
RVAPI RvSipStackHandle RVCALLCONV rvIppSipControlGetStackHandle(
    IN RvIppSipControlHandle sipMgrHndl);



/******************************************************************************
*  rvIppSipControlGetTerminal
*  --------------------------------
*  General :        This function is DEPRECATED!!! 
*					Returns a handle to Mdm terminal
*
*  Return Value:  a handle to Mdm terminal
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:               hAppCallLeg         Handle to application call-leg handle
*
*  Output:          None
******************************************************************************/
RVAPI RvIppTerminalHandle RVCALLCONV rvIppSipControlGetMdmTerminal(
    IN RvSipAppCallLegHandle   hAppCallLeg);

/******************************************************************************
*  rvIppSipControlGetMdmTerminal
*  --------------------------------
*  General :        This function is DEPRECATED!!! 
*					Returns a handle to MDM terminal
*
*  Return Value:    a handle to MDM terminal
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:               connHndl         Handle to SIP connection
*
*  Output:          None
******************************************************************************/
RVAPI RvIppTerminalHandle RVCALLCONV rvIppSipConnGetMdmTerminal(
                      IN RvIppConnectionHandle          connHndl);


/*==================================================================================
					 T Y P E S		F R O M		rvMdmControlApi.h
===================================================================================*/

/* RvIppProviderHandle
 * ------------------------------------------------------------------------
 * This type is DEPRECATED!!! 
 * The provider handle in the MTF. This handle associates a group of
 * terminations (RvIppTerminalHandle) into a given MTF instance.
 */
RV_DECLARE_HANDLE(RvIppProviderHandle);



/**********************************************************************************
                E X T E N S I O N       A P I       D E F I N I T I O N S
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
*  General :        This callback is DEPRECATED!!! use RvMtfPreProcessEventEv() instead.
*					Prototype for PreProcessEvent CallBack.
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
                                        IN      RvIppConnectionHandle   connHndl,
                                        IN      RvCCTerminalEvent       eventId,
                                        INOUT   RvCCEventCause*			reason);

/******************************************************************************
*  RvIppMdmPostProcessEventCB
*  ----------------------------
*  General :        This callback is DEPRECATED!!! use RvMtfPostProcessEventEv() instead.
*					Prototype for PostProcessEvent CallBack.
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
                                        IN  RvIppConnectionHandle   connHndl,
                                        IN  RvCCTerminalEvent       eventId,
                                        IN  RvCCEventCause			reason);


/******************************************************************************
*  RvIppMdmConnectionCreatedCB
*  ----------------------------
*  General :        This callback is DEPRECATED!!! 
*					Prototype for Mdm Connection Created CallBack.
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
*  General :        This callback is DEPRECATED!!! 
*					Prototype for Mdm Connection Destructed CallBack.
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
*  General :        This callback is DEPRECATED!!!
*					Prototype for MapUserEvent CallBack.
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
*  General :        This callback is DEPRECATED!!! use RvMtfUpdateTextDisplayEv() instead.
*					Prototype for Display CallBack.
*                   Enables user application to control the display.
*
*  Return Value:    None.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          connHndl         connection Handle
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
    IN RvCCEventCause		    cause,
    IN void*                    displayData);

/**********************************************************************************
                E X T E N S I O N       A P I       S T R U C T U R E
**********************************************************************************/

/* RvIppMdmExtClbks
 * ------------------------------------------------------------------------
 * This type is DEPRECATED!!! use RvMtfCallControlClbks instead.
 * MDM extension callbacks struct.
 * These callbacks can be used by the application to support proprietary
 * behavior or to add additional functionality.
 */
typedef struct
{
    RvIppMdmDisplayCB                   display;
        /* Enables the application to control the display */

    void*                               displayData;
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
*			This function is DEPRECATED!!! use rvMtfRegisterMdmExtCallbacks() instead.
*			It is used to register the MDM Control's extension callbacks.
*			Most of the callbacks have an additional userData parameter, which will get
*			the value indicated in the clbks struct.
*  Arguments:
* Input:  clbks    - The callbacks and userData to register.
* Output: None.
*
* Return Value: None.
 *****************************************************************************/
RVAPI void RVCALLCONV rvIppMdmRegisterExtClbks(IN RvIppMdmExtClbks* clbks);



/**********************************************************************************
                    P R O V I D E R         A P I s
**********************************************************************************/

/******************************************************************************
*  rvIppMdmProviderGetDialToneDuration
*  ----------------------------
*  General :        This function is DEPRECATED!!! 
*					Retrieves Dial-Tone Duration configured value.
*
*
*  Return Value:    number of seconds to play Dial-Tone.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          providerHndl     provider Handle
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
*  General :        This function is DEPRECATED!!! 
*					Retrieves the display data.
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
*  General :        This function is DEPRECATED!!! use rvMtfTerminationGetByTermId() instead.
*					Retrieves the terminal handle based on terminal-id
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
*  General :        This function is DEPRECATED!!! 
*					Retrieves the terminal handle based on IP address
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
*  General :        This function is DEPRECATED!!! 
*					Retrieves the first terminal handle which exists under this provider.
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
*  General :        This function is DEPRECATED!!! 
*					Retrieves the first terminal handle based on E.164 number
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
                    T E R M I N A L     A P I s
**********************************************************************************/

/******************************************************************************
*  rvIppMdmTerminalGetProvider
*  ----------------------------
*  General :        This function is DEPRECATED!!! 
*					Retrieves the provider which this terminal belongs to.
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
*  General :        This function is DEPRECATED!!! use rvMtfTerminationGetId() instead.
*					Retrieves the terminal ID.
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
*  General :        This function is DEPRECATED!!! 
*					Retrieves the terminal type based on terminalHandle..
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
*  General :        This function is DEPRECATED!!! 
*					Retrieves the last MDM event received for this terminal.
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
*  General :        This function is DEPRECATED!!! 
*					Retrieves the the terminal's media Capabilities.
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
*  General :        This function is DEPRECATED!!! 
*					Retrieves a specific media Stream.
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
*  General :        This function is DEPRECATED!!! 
*					Finds if there is 1 digit in the current dial string
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
*  General :        This function is DEPRECATED!!! 
*					Retrieves last digit of current dial string
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
*  General :        This function is DEPRECATED!!! 
*					Retrieves the terminal's phone number
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
*  General :        This function is DEPRECATED!!! 
*					Retrieves the type of the active Audio Termination
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
*  General :        This function is DEPRECATED!!! 
*					Retrieves the handle of the active Audio Termination
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
*  General :        This function is DEPRECATED!!! use rvMtfTerminationStopAllActiveSignals() instead.
*					Stops playing signals on this terminal
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
*  General :        This function is DEPRECATED!!! 
*					Start playing user signal on UI terminal.
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
*  General :        This function is DEPRECATED!!! 
*					Start playing user signal on Audio terminal.
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
*  General :        This function is DEPRECATED!!! 
*					start playing DialTone on this terminal.
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
*  General :        This function is DEPRECATED!!! 
*					Start ringing on this terminal.
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
*  General :        This function is DEPRECATED!!! 
*					Stop ringing on this terminal.
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
*  General :        This function is DEPRECATED!!! 
*					Start  RingBack Tone on this terminal.
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
*  General :        This function is DEPRECATED!!! 
*					Start  CallWaiting Tone  on this terminal.
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
*  General :        This function is DEPRECATED!!! 
*					Start  Caller CallWaiting Tone  on this terminal.
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
*  General :        This function is DEPRECATED!!! 
*					Start  Busy Tone  on this terminal.
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
*  General :        This function is DEPRECATED!!! 
*					Start  Warning Tone  on this terminal.
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
*  General :        This function is DEPRECATED!!! 
*					Start  DTMF Tone  on this terminal.
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
*  General :        This function is DEPRECATED!!! 
*					Stop  DTMF Tone  on this terminal.
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
*  General :        This function is DEPRECATED!!! 
*					Set or Clear the Hold Indicator
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
*  General :        This function is DEPRECATED!!! 
*					Set or Clear the Line Indicator
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
    IN RvCCTerminalIndState     state);

/******************************************************************************
*  rvIppMdmTerminalSendLineActive
*  ----------------------------
*  General :        This function is DEPRECATED!!! 
*					Set or Clears the Line Indicator
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
*  General :        This function is DEPRECATED!!! 
*					Places text  to the terminal's display
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
RVAPI RvBool RVCALLCONV rvIppMdmTerminalSetDisplay(
    IN RvIppTerminalHandle  terminalHndl,
    IN const RvChar*        text,
    IN RvInt32              row,
    IN RvInt32              column);

/******************************************************************************
*  rvIppMdmTerminalClearDisplay
*  ----------------------------
*  General :        This function is DEPRECATED!!! 
*					Clears the terminal display
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
*  General :        This function is DEPRECATED!!! 
*					updates the caller ID
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
*  General :        This function is DEPRECATED!!! 
*					Instructs Terminal to start collecting digits
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
*  General :        This function is DEPRECATED!!! use rvMtfGetLastDialString() instead.
*					Retrieves current dial string
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
*  General :        This function is DEPRECATED!!! 
*					clears the  current dial string
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
*  General :        This function is DEPRECATED!!! 
*					Finds whether OOB DTMF is enabled
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
*  General :        This function is DEPRECATED!!! 
*					Finds whether we support DTMF play on this terminal.
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
*  General :        This function is DEPRECATED!!! 
*					Get the maximum number of connections on the terminal
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
*  General :        This function is DEPRECATED!!! 
*					Get the  number of active connections on the terminal
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
*  General :        This function is DEPRECATED!!! use rvMtfTerminationGetActiveConnection() instead. 
*					Get the  active connection
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
*  General :        This function is DEPRECATED!!! 
*					Get the  current location of cursor on the display
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
*  General :        This function is DEPRECATED!!! 
*					Get the  current location of cursor on the display
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
*  General :        This function is DEPRECATED!!! use rvMtfTerminalGetOtherConnectionOnHold() instead.
*					Finds whether there is another held connection on this terminal.
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
*  General :        This function is DEPRECATED!!! use rvMtfTerminalGetOtherConnectionOnHold() instead.
*					Get the held connection of this terminal.
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
*  General :        This function is DEPRECATED!!! 
*					Set terminal state.
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
*  General :        This function is DEPRECATED!!! use rvMtfTerminationGetState() instead.
*					Get terminal state.
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
*  General :        This function is DEPRECATED!!! 
*					Get Mdm term object.
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
 *			This function is DEPRECATED!!! 
 *			Get the terminal's handle from the MDM termination.
 * Arguments:
 * Input:  mdmTerm  - MDM termination handle.
 * Output: None.
 *
 * Return Value: Returns a pointer to the terminal's handle, or NULL if it fails.
 *****************************************************************************/
RVAPI RvIppTerminalHandle RVCALLCONV rvIppMdmTerminalGetHandle(
    IN RvMdmTerm* mdmTerm);




/**********************************************************************************
                    C O N N E C T I O N     A P I s
**********************************************************************************/

/******************************************************************************
*  rvIppMdmConnGetTerminal
*  ----------------------------
*  General :   This function is DEPRECATED!!! use rvMtfConnectionGetTerminal() instead.
*				Gets a handle to the Terminal object associated with the Connection.
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
*  General :   This function is DEPRECATED!!! use rvMtfConnectionGetLineId() instead.
*				Gets the ID of the line on which the call was created..
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
*  General :   This function is DEPRECATED!!! use rvMtfConnectionGetState() instead.
*				Returns the Connection state.
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
*  General :   This function is DEPRECATED!!! use rvMtfConnectionGetTermConnState() instead.
*				Gets the Termination state of the Connection.
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
*  General :   This function is DEPRECATED!!! 
*				Gets the media state of the Connection.
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
*  General :   This function is DEPRECATED!!! 
*				Gets the Connection type
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
*  General :   This function is DEPRECATED!!! 
*				Gets the other party connected to this Connection.
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
*  General :   This function is DEPRECATED!!! 
*				Gets the media descriptor, which includes the parameters of the
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
*  General :   This function is DEPRECATED!!! 
*				Gets the capabilities loaded by the user application during
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
*  General :   This function is DEPRECATED!!! use rvMtfConnectionSetUserData() instead.
*				Sets the user data of the call from the connection object.
*
*  Return Value:   RV_OK on success, or failure if the connection handle has
*                  no call object associated with it yet.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:        connHndl       -   A handle to the Connection
*                userData       -   a pointer to the user data to be stored.
*
*  Output:       none.
*
****************************************************************************/
RVAPI RvStatus RVCALLCONV rvIppMdmConnSetUserData(
    IN RvIppConnectionHandle    connHndl,
    IN void*                    userData);

/******************************************************************************
*  rvIppMdmConnGetUserData
*  ----------------------------
*  General :   This function is DEPRECATED!!! use rvMtfConnectionGetUserData() instead.
*				Gets the user data of the call from the connection object.
*
*  Return Value:   Returns a pointer to the user data in the call object.
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
*  General :   This function is DEPRECATED!!! use rvMtfConnectionGetCallerName() instead.
*				Gets the caller name as received from the remote party
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
*  General :   This function is DEPRECATED!!! 
*				Gets the caller number as received from the remote party
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
*  General :   This function is DEPRECATED!!! 
*				Gets the caller number as received from the remote party
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
*  General :   This function is DEPRECATED!!! use rvMtfConnectionGetCallerAddress() instead.
*				Gets the caller address as received from the remote party
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
 *			This function is DEPRECATED!!! 
 *			Gets the caller ID as received from the remote party in an incoming call.
 *			This will be empty for outgoing calls and will return RV_FALSE.
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
*  General :        This function is DEPRECATED!!! 
 *					Get Presentation information (name and permission) of
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
 *			This function is DEPRECATED!!! use rvMtfConnectionGetCallType() instead.
 *			Get the call state of this connection.
 * Arguments:
 * Input:  connHndl     - A handle to the connection.
 * Output: None.
 *
 * Return Value: The call's state.
 *****************************************************************************/
RVAPI RvCCCallState RVCALLCONV rvIppMdmConnGetCallState(
    IN RvIppConnectionHandle    connHndl);

/******************************************************************************
 * rvIppMdmTerminalGetUserData
 * ----------------------------------------------------------------------------
 * General:
 *			This function is DEPRECATED!!! use rvMtfTerminalGetAppHandle() instead.
 *			Get the terminal user data.
 * Arguments:
 * Input:  terminalHndl     - A handle to the terminal.
 * Output: None.
 *
 * Return Value: user data.
 *****************************************************************************/
RVAPI void*  RVCALLCONV rvIppMdmTerminalGetUserData( IN RvIppTerminalHandle terminalHndl);

/******************************************************************************
 * rvIppMdmTerminalSetUserData
 * ----------------------------------------------------------------------------
 * General:
 *			This function is DEPRECATED!!! use rvMtfTerminalSetAppHandle() instead.
 *			Get the terminal user data.
 * Arguments:
 * Input:	terminalHndl     - A handle to the terminal.
 *			userData		 - user data
 * Output: None.
 *
 * Return Value: None.
 *****************************************************************************/
RVAPI void RVCALLCONV rvIppMdmTerminalSetUserData ( 
                                                   IN RvIppTerminalHandle   terminalHndl,
                                                   IN void*                 userData);

#endif /*RV_DEPR_TYPES_H*/
