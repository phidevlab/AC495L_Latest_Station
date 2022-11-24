
/*
*********************************************************************************
*                                                                               *
* NOTICE:                                                                       *
* This document contains information that is confidential and proprietary to    *
* RADVision LTD.. No part of this publication may be reproduced in any form     *
* whatsoever without written prior approval by RADVision LTD..                  *
*                                                                               *
* RADVision LTD. reserves the right to revise this publication and make changes *
* without obligation to notify any person of such revisions or changes.         *
*********************************************************************************
*/


/*********************************************************************************
 *                              <RegClientSecAgree.h>
 *
 * In this file the reg-client attaches to a security agreement-object and uses it 
 * to obtain and maintain a security-agreement according to RFC3329.
 *
 *    Author                         Date
 *    ------                        ------
 *    Tamar Barzuza                 Apr 2001
 *********************************************************************************/

#ifndef REG_CLIENT_SEC_AGREE_H
#define REG_CLIENT_SEC_AGREE_H

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------*/
/*                        INCLUDE HEADER FILES                           */
/*-----------------------------------------------------------------------*/
#include "RV_SIP_DEF.h"
#ifdef RV_SIP_IMS_ON
#include "RvSipRegClientTypes.h"
#include "RegClientObject.h"
#include "RvSipSecAgreeTypes.h"


/*-----------------------------------------------------------------------*/
/*                          TYPE DEFINITIONS                             */
/*-----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/*                          FUNCTIONS HEADERS                            */
/*-----------------------------------------------------------------------*/

/***************************************************************************
 * RegClientSecAgreeAttachSecAgree
 * ------------------------------------------------------------------------
 * General: Attaches a security-agreement object to this reg-client. 
 *          If this security-agreement object represents an agreement that
 *          was obtained with the remote party, the reg-client will exploit this 
 *          agreement and the data it brings. If not, the reg-client will use this 
 *          security-agreement to negotiate with the remote party.
 *          
 * Return Value: RvStatus.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:   pRegClient   - Pointer to the reg-client.
 *          hSecAgree  - Handle to the security-agreement.
 ***************************************************************************/
RvStatus RVCALLCONV RegClientSecAgreeAttachSecAgree(
								IN  RegClient                     *pRegClient,
								IN  RvSipSecAgreeHandle          hSecAgree);

/***************************************************************************
 * RegClientSecAgreeDetachSecAgree
 * ------------------------------------------------------------------------
 * General: Detaches the security-agreement object of this reg-client. 
 *          This security-agreement is no longer used.
 *          
 * Return Value: RvStatus.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:   pRegClient         - Pointer to the reg-client.
 ***************************************************************************/
RvStatus RVCALLCONV RegClientSecAgreeDetachSecAgree(
										IN  RegClient            *pRegClient);

/***************************************************************************
 * RegClientSecAgreeMsgToSend
 * ------------------------------------------------------------------------
 * General: Processes a message that is about to be sent by the reg-client:
 *          adds the required security information.
 *          
 * Return Value: RvStatus.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:   pRegClient         - Pointer to the reg-client.
 *          hMsg             - Handle to the message about to be sent.
 ***************************************************************************/
RvStatus RVCALLCONV RegClientSecAgreeMsgToSend(
											IN  RegClient                     *pRegClient,
											IN  RvSipMsgHandle               hMsg);

/***************************************************************************
 * RegClientSecAgreeDestResolved
 * ------------------------------------------------------------------------
 * General: Notified the security-agreement object that destination was 
 *          resolved. The security-agreement will modify the message and 
 *          its inner state accordingly.
 *          
 * Return Value: (-)
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:   pRegClient      - Pointer to the reg-client.
 *          hMsg            - Handle to the message
 *          hTransc         - The transaction that notified destination resolution
 *                            Notice: only one of hTrx and hTransc is valid.
 ***************************************************************************/
void RVCALLCONV RegClientSecAgreeDestResolved(
								IN  RegClient                      *pRegClient,
						        IN  RvSipMsgHandle                hMsg,
							    IN  RvSipTranscHandle             hTransc);

/***************************************************************************
 * RegClientSecAgreeMsgSendFailure
 * ------------------------------------------------------------------------
 * General: Processes a message send failure in the register-client:
 *          invalidates security-agreement.
 *          
 * Return Value: RvStatus.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:   pRegClient       - Pointer to the reg-client.
 ***************************************************************************/
RvStatus RVCALLCONV RegClientSecAgreeMsgSendFailure(
											IN  RegClient        *pRegClient);

/***************************************************************************
 * RegClientSecAgreeMsgRcvd
 * ------------------------------------------------------------------------
 * General: Processes a message being received by the reg-client:
 *          loads the required security information and verifies the security
 *          agreement validity.
 *          
 * Return Value: RvStatus.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:   pRegClient    - Pointer to the reg-client.
 *          hMsg          - Handle to the message received.
 ***************************************************************************/
RvStatus RVCALLCONV RegClientSecAgreeMsgRcvd(
										IN  RegClient                   *pRegClient,
										IN  RvSipMsgHandle               hMsg);


/*-----------------------------------------------------------------------*/
/*                          CALLBACKS                                    */
/*-----------------------------------------------------------------------*/

/***************************************************************************
 * RegClientSecAgreeAttachSecObjEv
 * ------------------------------------------------------------------------
 * General: Attaches a security object to this reg-client. 
 *          
 * Return Value: RvStatus.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:  hOwner          - Handle to the reg-client.
 *         hSecAgree       - Handle to the security-agreement object.
 *         hSecObj         - Handle to the security object
 ***************************************************************************/
RvStatus RVCALLCONV RegClientSecAgreeAttachSecObjEv(
										IN  void*                      hOwner,
										IN  RvSipSecAgreeHandle        hSecAgree,
										IN  RvSipSecObjHandle          hSecObj);

/***************************************************************************
 * RegClientSecAgreeDetachSecAgreeEv
 * ------------------------------------------------------------------------
 * General: Detaches a security-agreement object from this reg-client. 
 *          
 * Return Value: RvStatus.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:  hOwner          - Handle to the reg-client.
 *         hSecAgree       - Handle to the security-agreement object.
 ***************************************************************************/
RvStatus RVCALLCONV RegClientSecAgreeDetachSecAgreeEv(
										IN  void*                      hOwner,
										IN  RvSipSecAgreeHandle        hSecAgree);

#endif /* #ifdef RV_SIP_IMS_ON */

#ifdef __cplusplus
}
#endif


#endif /* END OF: ifdef REG_CLIENT_SEC_AGREE_H */


