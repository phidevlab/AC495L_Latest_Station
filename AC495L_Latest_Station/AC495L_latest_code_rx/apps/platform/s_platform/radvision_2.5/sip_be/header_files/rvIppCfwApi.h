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

#include "rvtypes.h"
#include "rvMdmControlApi.h"

#define RV_IPP_CFW_MAX_NUMBER_OF_TYPES	3


/* RvIppCfwReturnReasons
 * ------------------------------------------------------------------------
 * Return reasons for the activation and deactivation callbacks of call
 * forwarding.
 */
typedef enum
{
	RV_IPP_CFW_SUCCESS,
        /* Activation or deactivation procedure has successfully finished */

	RV_IPP_CFW_INVALID_DEACTIVATION,
        /* Forwarding type was not activated */

	RV_IPP_CFW_INVALID_PARAM,
        /* One of parameters is not valid */

	RV_IPP_CFW_ADDRESS_NOT_FOUND,
        /* Local mapping of phone number failed */

	RV_IPP_CFW_NOT_ALLOWED,
        /* Not allowed in the "blocked" cases */

	RV_IPP_CFW_CANCELLED_BY_USER 
        /* Forwarding request for activation or deactivation was canceled by the user */
} RvIppCfwReturnReasons;


/* RvIppCfwType
 * ------------------------------------------------------------------------
 * The type of call forwarding being invoked.
 */
typedef enum
{
	RV_IPP_CFW_TYPE_UNCONDITIONAL,
        /* Call forwarding is unconditional. The call should be forwarded
           no matter what the terminal's status is. */

	RV_IPP_CFW_TYPE_BUSY,
        /* Call forwarding should be invoked if the terminal is busy. */

	RV_IPP_CFW_TYPE_NO_REPLY,
        /* Call forwarding should be invoked when the terminal does not
           reply on incoming calls */

	RV_IPP_CFW_TYPE_NONE
        /* No call forwarding should be used.
           Must be last, for max number of cfw types */
} RvIppCfwType;


typedef void (*rvIppCfwActivateCompletedCB)(
                IN RvIppTerminalHandle	    term,
                IN RvIppCfwType             cfwType,
                IN RvChar*                  cfwDestination,
                IN RvIppCfwReturnReasons    returnCode);

typedef void (*rvIppCfwDeactivateCompletedCB)(
                RvIppTerminalHandle	term,
                RvIppCfwType 	 	cfwType,
                RvIppCfwReturnReasons returnCode);

typedef struct
{
	rvIppCfwActivateCompletedCB		activateCompleted;
	rvIppCfwDeactivateCompletedCB	deactivateCompleted;
} RvIppCfwCBs;


typedef struct
{
	RvIppCfwCBs	cfwCallBacks;
} RvIppCfwCfg;

/***************************************************************************
* rvCCCfwGetTypeNumber
* ------------------------------------------------------------------------
* General: It converts from cfw type string to cfw type number.
* Return Value:
*			Returns the valid cfw type Number, if the string is a valid one. Otherwise,
*			Returns RV_IPP_CFW_TYPE_NUM if the string is unknown.
* ------------------------------------------------------------------------
*  Arguments:
*  Input:	typeValue - Pointer to cfw type string
*
*  Output:	None
***************************************************************************/
RVAPI RvIppCfwType RVCALLCONV rvCCCfwGetTypeNumber(IN const RvChar *typeValue);

#endif /*RV_RVIPPCFWAPI_H*/





















