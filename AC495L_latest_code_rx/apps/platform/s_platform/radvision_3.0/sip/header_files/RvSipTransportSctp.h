/*
*******************************************************************************                                                                               *
* NOTICE:                                                                     *
* This document contains information that is confidential and proprietary to  *
* RADVision LTD.. No part of this publication may be reproduced in any form   *
* whatsoever without written prior approval by RADVision LTD..                *
*                                                                             *
* RADVision LTD. reserves the right to revise this publication and make       *
* changes without obligation to notify any person of such revisions or changes*
*******************************************************************************
*/


/******************************************************************************
 *                              <RvSipTransportSctp.h>
 *
 * This file defines the API for the SCTP module of the Transport Layer.
 * The module allows you to control sending and receiving of messages over
 * the SCTP network protocol.
 *
 *    Author                         Date
 *    ------                        ------
 *    Igor                          March 2005
 *****************************************************************************/


#ifndef RVSIP_TRANSPORTSCTP_H
#define RVSIP_TRANSPORTSCTP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "RV_SIP_DEF.h"

#if (RV_NET_TYPE & RV_NET_SCTP)
    
/******************************************************************************
 * RvSipTransportSctpInitializeMsgSendingParams
 * ----------------------------------------------------------------------------
 * General: initializes RvSipTransportSctpMsgSendingParams structure.
 * Return Value:
 * ----------------------------------------------------------------------------
 * Arguments:
 * Input:
 * Output: pSctpParams - pointer to structure to be initialized.
 *****************************************************************************/
RVAPI void RVCALLCONV RvSipTransportSctpInitializeMsgSendingParams(
                          OUT RvSipTransportSctpMsgSendingParams *pSctpParams);

/******************************************************************************
 * RvSipTransportConnectionSctpLocalAddressAdd
 * ----------------------------------------------------------------------------
 * General: binds a new multihoming addresses to the connection socket
 * Return Value: RV_OK on success,
 *               error code, defined in RV_SIP_DEF.h or rverror.h otherwise
 * ----------------------------------------------------------------------------
 * Arguments:
 * Input:  hConn           - Handle to the connection.
 *         pAddressDetails - Array of the addresses to be added.
 *         addrStructSize  - Size of the array element.
 *         numOfAddrs      - Number of elements in the array.
 * Output:
 *****************************************************************************/
RVAPI RvStatus RVCALLCONV RvSipTransportConnectionSctpLocalAddressAdd(
                           IN  RvSipTransportConnectionHandle hConn,
                           IN  RvSipTransportAddr             *pAddressDetails,
                           IN  RvUint32                       addrStructSize,
                           IN  RvInt32                        numOfAddrs);

/******************************************************************************
 * RvSipTransportConnectionSctpLocalAddressRemove
 * ----------------------------------------------------------------------------
 * General: unbinds the multihoming addresses from the connection socket
 * Return Value: RV_OK on success,
 *               error code, defined in RV_SIP_DEF.h or rverror.h otherwise
 * ----------------------------------------------------------------------------
 * Arguments:
 * Input:  hConn           - Handle to the connection.
 *         pAddressDetails - Array of the addresses to be removed.
 *         addrStructSize  - Size of the array element.
 *         numOfAddrs      - Number of elements in the array.
 * Output:
 *****************************************************************************/
RVAPI RvStatus RVCALLCONV RvSipTransportConnectionSctpLocalAddressRemove(
                           IN  RvSipTransportConnectionHandle hConn,
                           IN  RvSipTransportAddr             *pAddressDetails,
                           IN  RvUint32                       addrStructSize,
                           IN  RvInt32                        numOfAddrs);

/******************************************************************************
 * RvSipTransportConnectionSctpGetNumOfLocalAddr
 * ----------------------------------------------------------------------------
 * General: extracts number of the local multihoming addresses
 * Return Value: RV_OK on success,
 *               error code, defined in RV_SIP_DEF.h or rverror.h otherwise
 * ----------------------------------------------------------------------------
 * Arguments:
 * Input:     hConn           - Handle to the connection.
 * Output:    pNumOfLocalAddr - Number of local addresses.
 *****************************************************************************/
RVAPI RvStatus RVCALLCONV RvSipTransportConnectionSctpGetNumOfLocalAddr(
                           IN  RvSipTransportConnectionHandle hConn,
                           OUT RvUint32                       *pNumOfLocalAddr);

/******************************************************************************
 * RvSipTransportConnectionSctpGetLocalAddresses
 * ----------------------------------------------------------------------------
 * General: extracts a set of local mutlihoming addresses
 * Return Value: RV_OK on success,
 *               error code, defined in RV_SIP_DEF.h or rverror.h otherwise
 * ----------------------------------------------------------------------------
 * Arguments:
 * Input:  hConn             - Handle to the connection.
 *         addrStructSize    - Size of the element of the array pLocalAddrDetails.
 *         pNumOfLocalAddr   - Maximal possible number of elements in the array.
 * Output: pNumOfLocalAddr   - Number of elements in the array.
 *         pLocalAddrDetails - Array of the SCTP Local Addresses.
 *****************************************************************************/
RVAPI RvStatus RVCALLCONV RvSipTransportConnectionSctpGetLocalAddresses(
                      IN    RvSipTransportConnectionHandle hConn,
                      IN    RvUint32                       addrStructSize,
                      INOUT RvUint32                       *pNumOfLocalAddr,
                      OUT   RvSipTransportAddr             *pLocalAddrDetails);

/******************************************************************************
 * RvSipTransportConnectionSctpGetNumOfRemoteAddr
 * ----------------------------------------------------------------------------
 * General: extracts the number of the remote multihoming addresses
 * Return Value: RV_OK on success,
 *               error code, defined in RV_SIP_DEF.h or rverror.h otherwise
 * ----------------------------------------------------------------------------
 * Arguments:
 * Input:     hConn            - Handle to the connection.
 * Output:    pNumOfRemoteAddr - Number of remote addresses.
 *****************************************************************************/
RVAPI RvStatus RVCALLCONV RvSipTransportConnectionSctpGetNumOfRemoteAddr(
                           IN  RvSipTransportConnectionHandle hConn,
                           OUT RvUint32                       *pNumOfRemoteAddr);

/******************************************************************************
 * RvSipTransportConnectionSctpGetRemoteAddresses
 * ----------------------------------------------------------------------------
 * General: extartcs a set of the remote multihoming addresses
 * Return Value: RV_OK on success,
 *               error code, defined in RV_SIP_DEF.h or rverror.h otherwise
 * ----------------------------------------------------------------------------
 * Arguments:
 * Input:  hConn              - Handle to the connection.
 *         addrStructSize     - Size of the element of the array pRemoteAddrDetails.
 *         pNumOfRemoteAddr   - Maximal possible number of elements in the array.
 * Output: pNumOfRemoteAddr   - Number of elements in the array.
 *         pRemoteAddrDetails - Array of the SCTP Remote Addresses.
 *****************************************************************************/
RVAPI RvStatus RVCALLCONV RvSipTransportConnectionSctpGetRemoteAddresses(
                      IN    RvSipTransportConnectionHandle hConn,
                      IN    RvUint32                       addrStructSize,
                      INOUT RvUint32                       *pNumOfRemoteAddr,
                      OUT   RvSipTransportAddr             *pRemoteAddrDetails);

/******************************************************************************
* RvSipTransportConnectionSctpSetPrimaryAddress
* ----------------------------------------------------------------------------
* General: sets the provided address as a primary address.
*          Outgoing messages on the Connection will be sent to this address.
* Return Value: RV_OK on success,
 *               error code, defined in RV_SIP_DEF.h or rverror.h otherwise
 * ----------------------------------------------------------------------------
 * Arguments:
 * Input:  hConn          - Handle to the connection.
 *         addrStructSize - Size of the structure, holding address.
 *         pAddr          - address.
 * Output:
 *****************************************************************************/
RVAPI RvStatus RVCALLCONV RvSipTransportConnectionSctpSetPrimaryAddress(
                           IN  RvSipTransportConnectionHandle hConn,
                           IN  RvUint32                       addrStructSize,
                           IN  RvSipTransportAddr             *pAddr);

/******************************************************************************
 * RvSipTransportConnectionSctpGetPrimaryAddress
 * ----------------------------------------------------------------------------
 * General: extracts the details of the local primary address
 *          Outgoing messages on the Connection are sent to this address.
 * Return Value: RV_OK on success,
 *               error code, defined in RV_SIP_DEF.h or rverror.h otherwise
 * ----------------------------------------------------------------------------
 * Arguments:
 * Input:  hConn          - Handle to the connection.
 *         addrStructSize - Size of the structure, which will hold the address.
 * Output: pAddr          - address.
 *****************************************************************************/
RVAPI RvStatus RVCALLCONV RvSipTransportConnectionSctpGetPrimaryAddress(
                           IN  RvSipTransportConnectionHandle hConn,
                           IN  RvUint32                       addrStructSize,
                           OUT RvSipTransportAddr             *pAddr);

#endif /* END OF: #ifndef RVSIP_TRANSPORTSCTP_H */

#ifdef __cplusplus
}
#endif


#endif /* END OF: #ifndef RVSIP_TRANSPORTSCTP_H*/
