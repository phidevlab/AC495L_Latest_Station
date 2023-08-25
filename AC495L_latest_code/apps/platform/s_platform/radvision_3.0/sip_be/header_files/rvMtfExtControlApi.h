/******************************************************************************
Filename:    rvMtfExtControlApi.h
Description: This file includes APIs for extensibility of MTF:
			 1. Processing application proprietary events (special keys on the phone etc.)
			 2. Enabling a manipulation of SIP message
			 3. Gaining a direct access of SIP stack
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
#ifndef MTF_CONTROL_API_H
#define MTF_CONTROL_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include "rvMtfExtControlTypes.h"



/*@*****************************************************************************
 * rvMtfRegisterSipExtCallbacks (RvMtfBasePkg)
 * -----------------------------------------------------------------------------
 * General:
 *        Registers MTF SIP Extensibility callbacks. The application can
 *		  use these callbacks to gain direct access to the SIP Stack.
 *
 * Arguments:
 *
 * Input:  clbks - Pointer to user implementations of SIP Extensibility
 *                 callbacks.
 *
 * Return Value:  None.
 ****************************************************************************@*/
RVAPI void RVCALLCONV rvMtfRegisterSipExtCallbacks(
					IN RvMtfSipControlClbks* clbks);

/*@*****************************************************************************
 * rvMtfRegisterMdmExtCallbacks (RvMtfBasePkg)
 * -----------------------------------------------------------------------------
 * General:
 *         Registers MDM Extensibility callbacks to the MTF. This function
 *         should be called after rvMtfSipConstruct() is called.
 *
 * Arguments:
 *
 * Input:  clbks - Pointer to the structure that contains MDM Extensibility
 *                 callbacks.
 *
 * Return Value: None.
 ****************************************************************************@*/
RVAPI void RVCALLCONV rvMtfRegisterMdmExtCallbacks(
                IN RvMtfCallControlClbks*			clbks);

/*@*****************************************************************************
 * rvMtfConnGetSipCallLeg (RvMtfSipExtPkg)
 * -----------------------------------------------------------------------------
 * General:
 *        Returns a handle to the SIP Stack call-leg associated with the given
 *        connection. The call-leg can be used to manipulate SIP messages.
 *
 * Arguments:
 *
 * Input:  hConn	  - Handle to the connection object (represents the call).
 *
 * Output:  hCallLeg  -	Handle to the SIP call-leg associated with the given
 *                      connection.
 *
 * Return Value:  RV_OK if successful, other if not.
 ****************************************************************************@*/
RVAPI RvStatus RVCALLCONV rvMtfConnGetSipCallLeg(
    IN RvIppConnectionHandle	hConn,
	OUT RvSipCallLegHandle*		hCallLeg);


/*@*****************************************************************************
 * rvMtfGetSipStackHandle (RvMtfSipExtPkg)
 * -----------------------------------------------------------------------------
 * General:
 *        Returns a handle to the SIP Stack.
 *
 * Arguments:
 *
 * Input:	hMtf		-   Handle to the MTF instance.
 *
 * Output:  hSipStack	-	Handle to the SIP Stack.
 *
 * Return Value:  RV_OK if successful, other if not.
 ****************************************************************************@*/
RVAPI RvStatus RVCALLCONV rvMtfGetSipStackHandle(
				IN RvMtfHandle				hMtf,
				OUT RvSipStackHandle*		hSipStack);

/*@*****************************************************************************
*  rvMtfTerminationGetByTermId (RvMtfTerminationsPkg)
* -----------------------------------------------------------------------------
*  General :    Retrieves a terminal from the list of registered terminals
*				by terminal ID.
*
*  Arguments:
*  Input:       hMtf		- Handle to the MTF instance.
*				termId		- The terminal ID.
*
*  Output:      hTerm  - Handle of the terminal with the given terminal ID.
*
*  Return Value: RV_OK if the terminal was found, other if not.
****************************************************************************@*/
RVAPI RV_Status RVCALLCONV rvMtfTerminationGetByTermId(
    IN RvMtfHandle				hMtf,
    IN RvChar*					termId,
	OUT RvIppTerminalHandle*	hTerm);


/*@*****************************************************************************
*  rvMtfTerminationGetId (RvMtfTerminationsPkg)
* -----------------------------------------------------------------------------
*  General :    Retrieves a terminal ID.
*
*  Arguments:
*  Input:       hTerm		- Handle to the terminal.
*				termIdLen   - Length of termId.
*
*  Output:      termId		- The terminal ID.
*
*  Return Value:    RV_OK if successful, other if not.
****************************************************************************@*/
RVAPI RV_Status RVCALLCONV rvMtfTerminationGetId(
		IN    RvIppTerminalHandle   hTerm,
		INOUT RvChar*               termId,
		IN    RvSize_t              termIdLen);


/*@*****************************************************************************
*  rvMtfTerminationStopAllActiveSignals (RvMtfTerminationsPkg)
* -----------------------------------------------------------------------------
*  General :    Indicates the application to stop all active signals.
*				After calling this function, the RvMtfStopSignalCB() callback
*				will be called for each active signal.
*
*  Arguments:
*  Input:       hTerm			- Handle to the terminal.
*
*  Return Value:    None.
****************************************************************************@*/
RVAPI void RVCALLCONV rvMtfTerminationStopAllActiveSignals(
					IN RvIppTerminalHandle		hTerm);



/*@*****************************************************************************
*  rvMtfTerminationGetLastDialString (RvMtfTerminationsPkg)
* -----------------------------------------------------------------------------
*  General :    Returns the last digit string that the user dialed on a
*				specific terminal.
*
*  Arguments:
*  Input:       hTerm			- Handle to the terminal.
*				hConn			- Handle to the connection.
*				dialStringLen	- Length of dialString.
*
*  Output:      dialString		- String that contains the last digit string
*								  dialed by the user.
*
*  Return Value:    RV_OK if successful, other if not.
****************************************************************************@*/
RVAPI RV_Status RVCALLCONV rvMtfTerminationGetLastDialString(
    IN		RvIppTerminalHandle		hTerm,
	IN		RvIppConnectionHandle   hConn,
    OUT		RvChar*					dialString,
    IN		RvSize_t				dialStringLen);


/*@*****************************************************************************
*  rvMtfTerminationGetActiveConnection (RvMtfTerminationsPkg)
* -----------------------------------------------------------------------------
*  General :    Returns the active connection on this terminal. The active
*				connection belongs to the connected call (i.e., the call
*				that is not on Hold or disconnected).
*
*  Arguments:
*  Input:       hTerm			- Handle to the terminal.
*
*  Output:      hConn			- Handle to the active connection.
*
*  Return Value:    RV_OK if successful, other if not.
****************************************************************************@*/
RVAPI RV_Status RVCALLCONV rvMtfTerminationGetActiveConnection(
				IN RvIppTerminalHandle		hTerm,
				OUT RvIppConnectionHandle*  hConn);

/*@*****************************************************************************
*  rvMtfTerminationGetOtherConnectionOnHold (RvMtfTerminationsPkg)
* -----------------------------------------------------------------------------
*  General :    Gets a connection (call) existing on this terminal
*               that was put on Hold and differs from the active connection.
*				If such a connection does not exist, the output parameter will be set to NULL.
*
*  Arguments:
*  Input:       hTerm			- Handle to the terminal.
*				hConn			- Handle to the active connection.
*
*  Output:      hHeldConn		- Handle to the connection on Hold. If none
*								  exists, this parameter will be set to NULL.
*
*  Return Value:    RV_OK if successful, other if not.
****************************************************************************@*/
RVAPI RvStatus RVCALLCONV rvMtfTerminationGetOtherConnectionOnHold(
				IN RvIppTerminalHandle		hTerm,
				IN RvIppConnectionHandle	hConn,
				OUT RvIppConnectionHandle*	hHeldConn);



/*@*****************************************************************************
*  rvMtfTerminationGetState (RvMtfTerminationsPkg)
* -----------------------------------------------------------------------------
*  General :    Returns the terminal state.
*
*  Arguments:
*  Input:       hTerm			- Handle to the terminal.
*
*  Output:      terminalState	- The terminal state.
*
*  Return Value:    RV_OK if successful, other if not.
****************************************************************************@*/
RVAPI RV_Status RVCALLCONV rvMtfTerminationGetState(
				IN RvIppTerminalHandle			hTerm,
				OUT RvMtfTerminationState*		terminalState);


/*================================================================================
                    C O N N E C T I O N     A P I s
================================================================================*/



/*@*****************************************************************************
*  rvMtfConnectionGetTerminal (RvMtfConnectionsPkg)
* -----------------------------------------------------------------------------
*  General :    Returns the terminal to which the connection belongs.
*
*  Arguments:
*  Input:       hConn - Handle to the connection.
*
*  Output:      hTerm - Handle to the terminal to which the connection belongs.
*
*  Return Value:    RV_OK if successful, other if not.
****************************************************************************@*/
RVAPI RvStatus RVCALLCONV rvMtfConnectionGetTerminal(
    IN RvIppConnectionHandle		hConn,
	OUT RvIppTerminalHandle*		hTerm);

/*@*****************************************************************************
*  rvMtfConnectionGetLineId (RvMtfConnectionsPkg)
* -----------------------------------------------------------------------------
*  General :    Gets the line ID of the line on which the call was created
*				(Line 1 or Line 2).
*
*  Arguments:
*  Input:       hConn			- Handle to the connection.
*
*  Output:      lineId			- Line ID of the connection.
*
*  Return Value:    RV_OK if successful, other if not.
****************************************************************************@*/
RVAPI RvStatus RVCALLCONV rvMtfConnectionGetLineId(
    IN RvIppConnectionHandle			hConn,
	OUT RvInt32*						lineId);

/*@*****************************************************************************
*  rvMtfConnectionGetState (RvMtfConnectionsPkg)
* -----------------------------------------------------------------------------
*  General :    Returns the connection state.
*
*  Arguments:
*  Input:       hConn			- Handle to the connection.
*
*  Output:      connState		- Connection state.
*
*  Return Value:    RV_OK if successful, other if not.
****************************************************************************@*/
RVAPI RvStatus RVCALLCONV rvMtfConnectionGetState(
    IN RvIppConnectionHandle		hConn,
	OUT	RvMtfConnectionState*		connState);

/*@*****************************************************************************
*  rvMtfConnectionGetTermConnState (RvMtfConnectionsPkg)
* -----------------------------------------------------------------------------
*  General :    Returns the Term-Connection state.
*
*  Arguments:
*  Input:       hConn			- Handle to the connection.
*
*  Output:      state			- Term-Connection state.
*
*  Return Value:    RV_OK if successful, other if not.
****************************************************************************@*/
RVAPI RvStatus RVCALLCONV rvMtfConnectionGetTermConnState(
		IN RvIppConnectionHandle	hConn,
		OUT RvMtfTermConnState*		state);


/*@*****************************************************************************
*  rvMtfConnectionSetAppHandle (RvMtfConnectionsPkg)
* -----------------------------------------------------------------------------
*  General :    Sets the application data to be associated with the call.
*
*  Arguments:
*  Input:       hConn			- Handle to the connection.
*				hConnApp		- Handle to the application data to be
*								  associated with the connection (the call).
*
*  Return Value:    RV_OK if successful, other if not.
****************************************************************************@*/
RVAPI RvStatus RVCALLCONV rvMtfConnectionSetAppHandle(
    IN RvIppConnectionHandle    hConn,
    IN RvMtfConnAppHandle		hConnApp);

/*@*****************************************************************************
*  rvMtfConnectionGetAppHandle (RvMtfConnectionsPkg)
* -----------------------------------------------------------------------------
*  General :    Gets the application data that is associated with the call.
*
*  Arguments:
*  Input:       hConn			- Handle to the connection.
*
*  Output:      hConnApp		- Handle to the application data that is
*								  associated with the connection (the call).
*
*  Return Value:    RV_OK if successful, other if not.
****************************************************************************@*/
RVAPI RvStatus RVCALLCONV rvMtfConnectionGetAppHandle(
    IN RvIppConnectionHandle	hConn,
	IN RvMtfConnAppHandle*		hConnApp);

/*@*****************************************************************************
*  rvMtfConnectionGetCallerName (RvMtfConnectionsPkg)
* -----------------------------------------------------------------------------
*  General :    Gets the caller name as received from the remote party
*				in the incoming Invite message (in the From header).
*				This function is relevant for incoming calls only (the caller
*				name will be empty in case of an outgoing call).
*
*  Arguments:
*  Input:       hConn			- Handle to the connection.
*				callerNameLen	- Length of callerName.
*
*  Output:      callerName		- String containing the caller name.
*
*  Return Value:    RV_OK if callerName is not empty, other if empty.
****************************************************************************@*/
RVAPI RvStatus RVCALLCONV rvMtfConnectionGetCallerName(
    IN    RvIppConnectionHandle     hConn,
    OUT	  RvChar*                   callerName,
    IN    RvSize_t                  callerNameLen);


/*@*****************************************************************************
*  rvMtfConnectionGetCallerAddress (RvMtfConnectionsPkg)
* -----------------------------------------------------------------------------
*  General :   Gets the caller address as received from the remote party
*              in the incoming Invite message. This function is relevant for
*			   incoming calls only (the caller address will be empty in case of
*			   an outgoing call).
*
*  Arguments:
*  Input:       hConn				- Handle to the connection.
*				callerAddressLen	- Length of callerAddress.
*
*  Output:      callerAddress		- String containing the caller address.
*
*  Return Value:    RV_OK if callerAddress is not empty, other if empty.
****************************************************************************@*/
RVAPI RvStatus RVCALLCONV rvMtfConnectionGetCallerAddress(
    IN    RvIppConnectionHandle		hConn,
    INOUT RvChar*					callerAddress,
    IN    RvSize_t					callerAddressLen);


/*@*****************************************************************************
*  rvMtfConnectionGetCallType (RvMtfConnectionsPkg)
* -----------------------------------------------------------------------------
*  General :   Gets the call's type: Basic, Conference or Transfer.
*
*  Arguments:
*  Input:       hConn			- Handle to the connection.
*
*  Output:      callType		- Type of call.
*
*  Return Value:    RV_OK if successful, other if not.
****************************************************************************@*/
RVAPI RvStatus RVCALLCONV rvMtfConnectionGetCallType(
    IN RvIppConnectionHandle    hConn,
	OUT RvMtfCallType*			callType);


/*@*****************************************************************************
*  rvMtfConnectionBlindTransferToDestination (RvMtfConnectionsPkg)
* -----------------------------------------------------------------------------
*  General :   Transfers the call of the given connection to the given 
*              destination blindly (blind transfer).
*
*  Arguments:
*  Input:       hConn			- Handle to the connection of the transferred 
*								  call (party B in transfer).
*	            destination		- URI of the target destination for the blind 
*								  transfer (party C in transfer). 
*
*  Return Value:    RV_OK if successful, other if not.
****************************************************************************@*/
RVAPI RvStatus RVCALLCONV rvMtfConnectionBlindTransferToDestination(
	IN RvIppConnectionHandle    hConn,
	IN RvChar*					destination);

/*@*****************************************************************************
*  rvMtfTerminationGetAppHandle (RvMtfTerminationsPkg)
* -----------------------------------------------------------------------------
*  General :   Gets the application data associated with the terminal.
*
*  Arguments:
*  Input:       hTerm			- Handle to the terminal.
*
*  Output:      hAppTerm		- Handle to the application data associated
*                                 with the terminal.
*
*  Return Value:  RV_OK if successful, other if not.
****************************************************************************@*/
RVAPI  RvStatus RVCALLCONV rvMtfTerminationGetAppHandle(
					IN RvIppTerminalHandle			hTerm,
					OUT RvMtfTerminalAppHandle*		hAppTerm);

/*@*****************************************************************************
*  rvMtfTerminationSetAppHandle (RvMtfTerminationsPkg)
* -----------------------------------------------------------------------------
*  General :   Sets the application data associated with the terminal.
*
*  Arguments:
*  Input:       hTerm			- Handle to the terminal.
*				hAppTerm		- Handle to the application data associated with
*                                 the terminal.
*
*  Return Value:  RV_OK if successful, other if not.
****************************************************************************@*/
RVAPI RvStatus RVCALLCONV rvMtfTerminationSetAppHandle (
                               IN RvIppTerminalHandle		hTerm,
                               IN RvMtfTerminalAppHandle    hAppTerm);


/*@*****************************************************************************
* rvMtfTerminationIsUpdateAllowedByRemoteParty (RvMtfTerminationsPkg)
* -----------------------------------------------------------------------------
* General:
*         Queries whether UPDATE is allowed by the remote party of the call.
*		  This function can be used before sending an UPDATE.
*
*         Note: The use of this API is optional. The MTF checks all 
*         preconditions before sending an UPDATE request.
*
* Arguments:
*
* Input:	hTerm			 - Handle to the terminal.
*			hConn			 - Handle to the connection. If set to NULL,
*                              the active call will be considered.
*			bIsUpdateAllowed - Indicates whether UPDATE is supported by 
*                              the remote party.
*
* Return Value: RV_OK if successful, other if not.
****************************************************************************@*/
RVAPI RvStatus rvMtfTerminationIsUpdateAllowedByRemoteParty(
						   IN RvIppTerminalHandle		hTerm,
						   IN RvIppConnectionHandle		hConn,
						   IN RvBool*					bIsUpdateAllowed);

/*@*****************************************************************************
* rvMtfTerminationIsUpdateInProcess (RvMtfTerminationsPkg)
* -----------------------------------------------------------------------------
* General:
*         Queries whether an UPDATE is currently in process, i.e., whether  
*         an UPDATE message was sent or received and was not yet replied to.
*		  A new UPDATE cannot be sent until the previous UPDATE is replied 
*		  to. This function can be used before sending an UPDATE.
*
*         Note: The use of this API is optional. The MTF checks all 
*         preconditions before sending an UPDATE.
*
* Arguments:
*
* Input:	hTerm			 - Handle to the terminal.
*			hConn			 - Handle to the connection. If set to NULL, 
*                              the active call will be considered.
*			bIsUpdateAllowed - Indicates whether an UPDATE is in process.
*
* Return Value: RV_OK if successful, other if not.
****************************************************************************@*/
RVAPI RvStatus rvMtfTerminationIsUpdateInProcess(
						IN RvIppTerminalHandle			hTerm,
						IN RvIppConnectionHandle		hConn,
						IN RvBool*						bIsUpdateInProcess);

/*@*****************************************************************************
* rvMtfTerminationGetOfferAnswerState (RvMtfTerminationsPkg)
* -----------------------------------------------------------------------------
* General:
*         Returns the offer-answer state of the active call.
*         The offer-answer state reflects the offer-answer exchange until the 
*         call is established. It does not reflect the re-invite offer-answer 
*		  exchange during a connected call.
*		  An UPDATE message cannot be sent if the offer-answer state is not 
*         Answered.
*
*         Note: The use of this API is optional. The MTF checks all 
*         preconditions before sending an UPDATE.
*
* Arguments:
*
* Input:	hTerm			 - Handle to the terminal.
*			hConn			 - Handle to the connection. If set to NULL,
*                              the active call will be considered.
*			stateOfferAnswer - The state of offer-answer.
*
* Return Value: RV_OK if successful, other if not.
****************************************************************************@*/
RVAPI RvStatus rvMtfTerminationGetOfferAnswerState(
						IN RvIppTerminalHandle			hTerm,
						IN RvIppConnectionHandle		hConn,
						IN RvMtfOfferAnswerState*		stateOfferAnswer);

/*@*****************************************************************************
* rvMtfTerminationIsUpdateFeasible (RvMtfTerminationsPkg)
* -----------------------------------------------------------------------------
* General:
*         Queries whether an UPDATE may be sent at this time by verifying that
*         all preconditions for sending an UPDATE request have been fulfilled.
*
*         Note: The use of this API is optional. The MTF checks all 
*         preconditions before sending an UPDATE.
*
* Arguments:
*
* Input:	hTerm			  - Handle to the terminal.
*			hConn			  - Handle to the connection. If set to NULL,
*                               the active call will be considered.
*			bIsUpdateFeasible - Indicates whether an UPDATE can be sent.
*
* Return Value: RV_OK if successful, other if not.
****************************************************************************@*/
RVAPI RvStatus rvMtfTerminationIsUpdateFeasible(
					   IN RvIppTerminalHandle		hTerm,
					   IN RvIppConnectionHandle		hConn,
					   IN RvBool*					bIsUpdateFeasible);

#ifdef __cplusplus
}
#endif


#endif /*MTF_CONTROL_API_H*/
