/*****************************************************************************
Filename: rvAkaAuth.h
Description: Utility functions for AKA Authentication
******************************************************************************
                      Copyright (c) 2006 RADVision Inc.
******************************************************************************
NOTICE:
This document contains information that is proprietary to RADVision Inc.
No part of this publication may be reproduced in any form whatsoever without
written prior approval by RADVision Inc.

RADVision Inc. reserves the right to revise this publication and make changes
without obligation to notify any person of such revisions or changes.
******************************************************************************
$Revision:$
$Date: September 2007$
$Author: Tamar Aharony$
*****************************************************************************/
#ifndef RV_AKAAUTH_H
#define RV_AKAAUTH_H

#ifdef RV_SIP_IMS_ON
#include "RvSipMsgTypes.h"
#include "RvSipAuthenticator.h"
#include "RvSipCallLegTypes.h"
#include "RvSipRegClientTypes.h"
#include "rvccterminalsip.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define USER_KEY_LEN   16
#define AKA_AUTN_LEN   16
#define AKA_RES_LEN    16
#define AKA_RAND_LEN   16
#define AKA_AUTS_LEN   14
#define AKA_AUTS_B64_ENCODED_LEN 20
#define AKA_SQN_LEN     6


/***************************************************************************************
* AppSetSharedSecretInAuthObj
* -------------------------------------------------------------------------------------
* General:  This function sets the shared secret in the authentication object:
*           1. Generates the Aka RES value.
*           2. Sets the user-name and the RES value in the auth-obj.
* Return Value: RvStatus.
* -------------------------------------------------------------------------------------
* Arguments:
* Input:    hAuthObj - Handle to the authentication object.
* Output:   none
**************************************************************************************/
RvStatus AppSetSharedSecretInAuthObj( IN RvSipAuthObjHandle hAuthObj, RvCCTerminalSip* sipTerm);

/***************************************************************************
 * rvAkaAuthHandleRegClientUnauthState
 * ------------------------------------------------------------------------
 * General: Handles the UNAUTHENTICATED state.
 *          1) Before resending the request, we remove the old authorization header,
 *          if exists. (Otherwise the request will have 2 Authorization headers,
 *          one with AUTS parameter, and one without).
 *          2) Resend the INVITE request with RvSipRegClientAuthenticate().
 ***************************************************************************/
RvStatus rvAkaAuthHandleRegClientUnauthState(RvSipRegClientHandle hRegClient, RvCCTerminalSip* sipTerm);

/***************************************************************************
 * rvAkaAuthHandleCallLegUnauthState
 * ------------------------------------------------------------------------
 * General: Handles the UNAUTHENTICATED state.
 *          1) Before resending the request, we remove the old authorization header,
 *          if exists. (Otherwise the request will have 2 Authorization headers,
 *          one with AUTS parameter, and one without).
 *          2) Resend the INVITE request with RvSipCallLegAuthenticate().
 ***************************************************************************/
RvStatus rvAkaAuthHandleCallLegUnauthState(RvSipCallLegHandle hCallLeg, RvCCTerminalSip* sipTerm);

/***************************************************************************************
* rvAkaAuthCallLegBuildInitialId
* -------------------------------------------------------------------------------------
* General:  Build a CallLeg authorization header with initial identification to start the
*           process of AKA authorization.
* Return Value: -
* -------------------------------------------------------------------------------------
* Arguments:
* Input:    hCallLeg - handle to the SIP callLeg object
* Output:   none
**************************************************************************************/
RvStatus rvAkaAuthCallLegBuildInitialId(RvSipCallLegHandle    hCallLeg, RvSipMsgHandle hMsg, RvCCTerminalSip*  sipTerm);

/***************************************************************************************
* rvAkaAuthRegClientBuildInitialId
* -------------------------------------------------------------------------------------
* General:  Build a RegClient authorization header with initial identification to start the
*           process of AKA authorization.
* Return Value: -
* -------------------------------------------------------------------------------------
* Arguments:
* Input:    hRegClient - handle to the SIP Reg Client object
* Output:   none
**************************************************************************************/
RvStatus rvAkaAuthRegClientBuildInitialId(RvSipRegClientHandle*  hRegClient, RvCCTerminalSip*  sipTerm);

/***************************************************************************************
* rvAkaAuthIsAkaAlgorithmInAuthHeader
* -------------------------------------------------------------------------------------
* General:  Checks if the  algorithm in the authentication header is AKA
*
* Return Value: -
* -------------------------------------------------------------------------------------
* Arguments:
* Input:    authHeader - handle to the SIP Authorization header
* Output:   none
**************************************************************************************/
RvBool rvAkaAuthIsAkaAlgorithmInAuthHeader(RvSipAuthenticationHeaderHandle hAuthHeader);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RV_AKAAUTH_H */

#endif /* RV_SIP_IMS_ON */
