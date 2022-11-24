/******************************************************************************
Filename:    rvccCfwMgr.h
Description: Generic call forward types and definitions.
*******************************************************************************
                Copyright (c) 2005 RADVISION
*******************************************************************************
NOTICE:
This document contains information that is proprietary to RADVISION.
No part of this publication may be reproduced in any form whatsoever
without written prior approval by RADVISION.

RADVISION reserves the right to revise this publication and make changes
without obligation to notify any person of such revisions or changes.
******************************************************************************/

#ifndef RV_RVCCCFWMGR_H
#define RV_RVCCCFWMGR_H

#include "rvtypes.h"
#include "rvIppCfwApi.h"

typedef enum
{
	RV_IPP_CFW_STATE_DEACTIVATED,	/* When cfw deactivation event is received */
	RV_IPP_CFW_STATE_ACTIVATE_START,	/* When cfw activation event is received, the activation process is started. i.e, waiting for digit events of the diverted-to number */
	RV_IPP_CFW_STATE_ACTIVATE_COMPLETED,	/* When activation is done. After mapping successfully the diverted-to number to address*/
	RV_IPP_CFW_STATE_NUM			/* Must be last, for max number of cfw state */
} RvCCCfwState;

typedef struct
{
	RvCCCfwState	cfwState;	/* cfw state */
	RvIppCfwType	cfwType;	/* Unconditional, Busy, or NoReply */
	RvChar			sendToAddress[RV_IPP_ADDRESS_SIZE]; /* Address of the diverted to destination */
	RvUint			cfnrTimeout;	/* relevant for CallForwardNoReply only, indicates how
								   long a new call will ring before call will be diverted. */	
} RvIppCfwTypeInfo;



typedef struct
{
	RvIppCfwTypeInfo 	cfwTypes[RV_IPP_CFW_MAX_NUMBER_OF_TYPES];
} rvCCCfwData;


typedef enum
{
	RV_IPP_CFW_EVENT_REGULAR_HANDLE,/* no cfw activation started. handle event as usual */
	RV_IPP_CFW_EVENT_CFW_HANDLE,	/* cfw activation started and a related cfw event received. Handle the event as relate to cfw */
	RV_IPP_CFW_EVENT_DO_NOT_HANDLE	/* cfw activation started and a NON related cfw received, ignore the event */
} RvCCCfwEventHandling;


/***************************************************************************
* rvCCCfwHandleCallForwardTerminalEvent
* ------------------------------------------------------------------------
* General:	This function handles event of call forward. 
*			It validates the event parameters, and calls to function to 
*			handle the event
* Return Value:
*			none.
* ------------------------------------------------------------------------
*  Arguments:
*  Input:	t			- terminal.
*  Output:	None
***************************************************************************/
RvCCTerminalEvent rvCCCfwHandleCallForwardTerminalEvent(RvCCTerminal *t);




/***************************************************************************
* rvCCCfwHandleActivationCompleted
* ------------------------------------------------------------------------
* General:	This function handles the case when the activation completed.
*			In case of failure, a warning tone is started
*			NOTE: This warning tone will be stopped when the user will press 
*			on any button of cfw.
*			In any case the states are updated and a user call back is called 
*			to indicate the result of the activation process.
* Return Value:
*			void
* ------------------------------------------------------------------------
*  Arguments:
*  Input:	t			- terminal.
*			IsCompletedSuccessfully - true: success, false: otherwise
*			returnReason: reason of failure in case of activation completed failure.
*  Output:	None
***************************************************************************/
RvCCTerminalEvent rvCCCfwHandleActivationCompleted(
                RvCCTerminal*           t,
                RvBool			        IsCompletedSuccessfully,
                RvIppCfwReturnReasons	returnReason);

/***************************************************************************
* rvCCCfwUpdateCfwAddress
* ------------------------------------------------------------------------
* General:	This function updates the cfw type which is in activate start state
*			with the found destination address.
*			In addition, The terminal state is changed to be idle and a 
*			user callback is called to indicate the user on a successful cfw activation.
* Return Value:
*			void
* ------------------------------------------------------------------------
*  Arguments:
*  Input:	t			- terminal.
*			destAddress - Address of the diverted-to number.
*			destAddressLen - length of the diverted-to address.
*  Output:	None
***************************************************************************/
void rvCCCfwUpdateCfwAddress(
                RvCCTerminal*   t, 
                char*           destAddress, 
                RvUint          destAddressLen);                             


/***************************************************************************
 * rvCCCfwSendCallForward
 * ------------------------------------------------------------------------
 * General: This function processes the call to be forwarded.
 *
 * Return Value: rvTrue in case of regular call, 
 *				rvFalse if call was forwarded successfully and there is 
 *				nothing else to process.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: 	data			- user data which is the cfw data.
  ***************************************************************************/
RvBool rvCCCfwSendCallForward(
                RvCCTerminal*   t,
                RvCCConnection* c,
                RvIppCfwType    cfwType);                              

/***************************************************************************
 * rvCCfwNoReplyTimerExpires
 * ------------------------------------------------------------------------
 * General: This function is called when the cfnr timer is expired.
 *
 * Return Value: RvBool (rvTrue in case of success, rvFalse in case of failure
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: 	data			- user data which is the cfw data.
  ***************************************************************************/
RvBool rvCCfwNoReplyTimerExpires(void* data);



/***************************************************************************
* rvCCCfwProcessIncomingCall
* ------------------------------------------------------------------------
* General: Process the incoming call. If there is an activated cfw type, 
*			get the one with the highest priority, and process the incoming 
*			call accordingly.
*			NOTE: All the input parameters should be valid when this function is called.
* Return Value:
*			rvTrue: the incoming call should be process as a normal call, 
*			rvFalse: the incoming call was handled as a forward call.
* ------------------------------------------------------------------------
*  Arguments:
*  Input:	x - pointer to connection
*
*  Output:	None
***************************************************************************/
RvBool rvCCCfwProcessIncomingCall(RvCCConnection* x);



/***************************************************************************
* rvCCCfwGetDivertedToAddress
* ------------------------------------------------------------------------
* General: Get address of the diverted to user within specific cfw type.
*			NOTE: All the input parameters should be valid when this function is called.
* Return Value:
*			RvStatus
* ------------------------------------------------------------------------
*  Arguments:
*  Input:	t - pointer to terminal
*			typeNumber - cfw type
*			addressLen - length of the diverted to address
*
*  Output:	divertedToAddress - Address of the diverted-to user.
*			The user that the call will be forwarded to.
***************************************************************************/
RvStatus rvCCCfwGetDivertedToAddress(
    IN  RvCCTerminal*   t,
    IN  RvIppCfwType    typeNumber,
    IN  RvUint          addressLen,
    OUT RvChar*         divertedToAddress);

/***************************************************************************
* rvCCCfwGetCFNRTimeout
* ------------------------------------------------------------------------
* General: Get timeout value of the call forward no reply..
*			NOTE: All the input parameters should be valid when this function is called.
* Return Value:
*			RvStatus
* ------------------------------------------------------------------------
*  Arguments:
*  Input:	t - pointer to terminal
*			typeNumber - cfw type
*
*  Output:	cfnrTimeout - timeoutvalue of cfnr timeout
***************************************************************************/
RvStatus rvCCCfwGetCfnrTimeout(
                RvCCTerminal*   t, 
                RvUint *        cfnrTimeout);

/***************************************************************************
* rvCfwDataInitToDefaultValues
* ------------------------------------------------------------------------
* General: Initialize cfwData structure to default values
* Return Value:
*			None
* ------------------------------------------------------------------------
*  Arguments:
*  Input:	cfwData - Pointer to cfw data
*
*  Output:	None
***************************************************************************/
void rvCCCfwDataInitToDefaultValues(rvCCCfwData *cfwData);

#endif /*RV_RVCCCFWMGR_H*/





















