/******************************************************************************
Filename:    rvIppCfwApi.h
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

#ifndef RV_RVIPPCFWAPI_H
#define RV_RVIPPCFWAPI_H

#include "rvMtfHandles.h"
#include "rvtypes.h"

/*@*****************************************************************************
 * Package: RvMtfCallForwardPkg (root)
 * -----------------------------------------------------------------------------
 * Title: Call Forward
 *
 * General: This section contains functions and type definitions  
 *  		for using the Call Forward feature in the MTF.
 ****************************************************************************@*/

#define RV_IPP_CFW_MAX_NUMBER_OF_TYPES	3

/*@*****************************************************************************
 * Enum: RvIppCfwReturnReasons (RvMtfCallForwardPkg)
 * -----------------------------------------------------------------------------
 * Description: This enumerator indicates the reasons for success or failure
 * of activation or deactivation of call forwarding.
 ****************************************************************************@*/
typedef enum
{
	RV_IPP_CFW_SUCCESS,
        /* The activation or deactivation procedure was completed successfully. */

	RV_IPP_CFW_INVALID_DEACTIVATION,
        /* The forwarding type was not activated. */

	RV_IPP_CFW_INVALID_PARAM,
        /* One of the parameters is not valid. */

	RV_IPP_CFW_ADDRESS_NOT_FOUND,
        /* Local mapping of the phone number failed. */

	RV_IPP_CFW_NOT_ALLOWED,
        /* Not allowed in blocked cases. */

	RV_IPP_CFW_CANCELLED_BY_USER
        /* Forwarding request for activation or deactivation was cancelled by
		the user. */
} RvIppCfwReturnReasons;


/*@*****************************************************************************
 * Enum: RvIppCfwType (RvMtfCallForwardPkg)
 * -----------------------------------------------------------------------------
 * Description: This enumerator indicates the type of call forwarding
 * being invoked.
 ****************************************************************************@*/
typedef enum
{
	RV_IPP_CFW_TYPE_UNCONDITIONAL,
        /* Call forwarding is unconditional. The call should be forwarded
           no matter what the status of the terminal. */

	RV_IPP_CFW_TYPE_BUSY,
        /* Call forwarding should be invoked if the terminal is busy. */

	RV_IPP_CFW_TYPE_NO_REPLY,
        /* Call forwarding should be invoked when the terminal does not
           reply to incoming calls. */

	RV_IPP_CFW_TYPE_NONE
        /* Call forwarding should not be used. Must be last for the
		maximum number of CFW types. */
} RvIppCfwType;

/*@*****************************************************************************
 * rvIppCfwActivateCompletedCB (RvMtfCallForwardPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         The MTF calls this callback to notify the application that
 *         activation of Call Forward was completed. The callback is called
 *         after a CFW event was sent to activate Call Forward in the terminal.
 *
 * Arguments:
 *
 * Input:	hTerm			- Handle to the terminal.
 *			cfwType			- Indicates the type of Call Forward that was
 *                            activated.
 *			cfwDestination	- Destination address to which calls are forwarded.
 *			returnCode		- Indicates the status of the activation.
 *
 * Return Value: None.
 ****************************************************************************@*/
typedef void (*rvIppCfwActivateCompletedCB)(
                IN RvIppTerminalHandle	    hTerm,
                IN RvIppCfwType             cfwType,
                IN RvChar*                  cfwDestination,
                IN RvIppCfwReturnReasons    returnCode);

/*@*****************************************************************************
 * rvIppCfwDeactivateCompletedCB (RvMtfCallForwardPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         The MTF calls this callback to notify the application that
 *         deactivation of Call Forward was completed. The callback is called
 *         after a CFW event was sent to deactivate Call Forward in the terminal.
 *
 * Arguments:
 *
 * Input:	hTerm			- Handle to the terminal.
 *			cfwType			- Indicates the type of Call Forward that was
 *                            deactivated.
 *			returnCode		- Indicates the status of the deactivation.
 *
 * Return Value: None.
 ****************************************************************************@*/
typedef void (*rvIppCfwDeactivateCompletedCB)(
                RvIppTerminalHandle		hTerm,
                RvIppCfwType 	 		cfwType,
                RvIppCfwReturnReasons	returnCode);

/*@*****************************************************************************
 * Type: RvIppCfwCBs (RvMtfCallForwardPkg)
 * -----------------------------------------------------------------------------
 * Description: This structure contains Call Forward callbacks. These callbacks
 * are optional.
 ****************************************************************************@*/
typedef struct
{
	rvIppCfwActivateCompletedCB		activateCompleted;
	rvIppCfwDeactivateCompletedCB	deactivateCompleted;
} RvIppCfwCBs;

/*@*****************************************************************************
 * Type: RvIppCfwCfg (RvMtfCallForwardPkg)
 * -----------------------------------------------------------------------------
 * Description: This structure contains the Call Forward configuration.
 ****************************************************************************@*/
typedef struct
{
	RvIppCfwCBs	cfwCallBacks;
} RvIppCfwCfg;

/***************************************************************************
* rvCCCfwGetTypeNumber
* ------------------------------------------------------------------------
* General:	This function is deprecated but is kept for backward compatibility.
*           It converts the CFW type string to the CFW type number.
*
* Return Value:
*			The valid CFW type number if the string is valid.
*			RV_IPP_CFW_TYPE_NUM if the string is unknown.
* ------------------------------------------------------------------------
*  Arguments:
*  Input:	typeValue - Pointer to the CFW type string.
*
***************************************************************************/
RVAPI RvIppCfwType RVCALLCONV rvCCCfwGetTypeNumber(IN const RvChar *typeValue);

#endif /*RV_RVIPPCFWAPI_H*/





















