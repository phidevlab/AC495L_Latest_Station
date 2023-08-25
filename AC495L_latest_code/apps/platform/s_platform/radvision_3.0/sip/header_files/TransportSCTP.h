/******************************************************************************
NOTICE:
This document contains information that is proprietary to RADVISION LTD..
No part of this publication may be reproduced in any form whatsoever without
written prior approval by RADVision LTD..

RADVISION LTD. reserves the right to revise this publication and make changes
without obligation to notify any person of such revisions or changes.

******************************************************************************/



/******************************************************************************
 *                              TransportSCTP.h
 *
 *  This file provides interface for the Transport Layer above SCTP Stack.
 *  It contains:
 *      1. API for managing SCTP module.
 *      2. API for managing SCTP connections.
 *      3. Socket style API for operations on SCTP connections.
 *
 *    Author                         Date
 *    ------                        ------
 *    Igor                          01/04/2005
 *****************************************************************************/


#ifndef TRANSPORT_SCTP_H
#define TRANSPORT_SCTP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "AdsHash.h"
#include "AdsRpool.h"

#include "TransportProcessingQueue.h"



#if (RV_NET_TYPE & RV_NET_SCTP)

/*-----------------------------------------------------------------------*/
/*                          TYPE DEFINITIONS                             */
/*-----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/*                          FUNCTIONS HEADERS                            */
/*-----------------------------------------------------------------------*/

/******************************************************************************
* TransportSctpMgrSetConfiguration
* -----------------------------------------------------------------------------
* General:      Set configuration of the SCTP module.
* Return Value: RV_OK on success,
*               error code, defined in rverror.h or RV_SIP_DEF.h otherwise.
* -----------------------------------------------------------------------------
* Arguments:
* Input:  pTransportMgr - pointer to the Transport Manager object.
*         pTransportCfg - configuration of the SCTP module.
* Output: none.
******************************************************************************/
void RVCALLCONV TransportSctpMgrSetConfiguration(
                                    IN TransportMgr    *pTransportMgr,
                                    IN SipTransportCfg *pTransportCfg);

/******************************************************************************
* TransportSctpMgrConstruct
* -----------------------------------------------------------------------------
* General:      Constructs data structures, serving SCTP module. Such as a list
*               of local addresses, a pool of multihoming addresses and other.
* Return Value: RV_OK on success,
*               error code, defined in rverror.h or RV_SIP_DEF.h otherwise.
* -----------------------------------------------------------------------------
* Arguments:
* Input:  pTransportMgr - pointer to the Transport Manager object.
* Output: none.
******************************************************************************/
RvStatus RVCALLCONV TransportSctpMgrConstruct(
                                    IN    TransportMgr    *pTransportMgr);

/******************************************************************************
* TransportSctpMgrDestruct
* -----------------------------------------------------------------------------
* General:      Destructs data structures, serving SCTP module.
* Return Value: none
* -----------------------------------------------------------------------------
* Arguments:
* Input:  pTransportMgr - pointer to the Transport Manager object.
* Output: none.
******************************************************************************/
void RVCALLCONV TransportSctpMgrDestruct(
                                    IN    TransportMgr    *pTransportMgr);

/******************************************************************************
* TransportSctpMgrInitializeLocalAddressList
* -----------------------------------------------------------------------------
* General:      prepares the list of SCTP Local Addresses.
* Return Value: RV_OK on success,
*               error code, defined in rverror.h or RV_SIP_DEF.h otherwise.
* -----------------------------------------------------------------------------
* Arguments:
* Input:  pTransportMgr - pointer to the Transport Manager object.
*         pTransportCfg - configuration of the SCTP module.
* Output: none.
******************************************************************************/
RvStatus RVCALLCONV TransportSctpMgrInitializeLocalAddressList(
                                    IN    TransportMgr    *pTransportMgr,
                                    INOUT SipTransportCfg *pTransportCfg);

/******************************************************************************
* TransportSctpMgrOpenLocalAddresses
* -----------------------------------------------------------------------------
* General:      open addresses for listening. The addresse are taken
*               from the manager list of SCTP Local Addresses.
* Return Value: RV_OK on success,
*               error code, defined in rverror.h or RV_SIP_DEF.h otherwise.
* -----------------------------------------------------------------------------
* Arguments:
* Input:  pTransportMgr - pointer to the Transport Manager object.
* Output: none.
******************************************************************************/
RvStatus RVCALLCONV TransportSctpMgrOpenLocalAddresses(
                                        IN TransportMgr    *pTransportMgr,
                                        IN SipTransportCfg *pTransportCfg);

/******************************************************************************
* TransportSctpMgrFindLocalAddressHandle
* -----------------------------------------------------------------------------
* General:      Finds the List of Local Addresses for the object, matching
*               the details of the transport address, supplied by the user.
* Return Value: RV_OK on success,
*               error code, defined in rverror.h or RV_SIP_DEF.h otherwise.
* -----------------------------------------------------------------------------
* Arguments:
* Input:  pTransportMgr  - pointer to the Transport Manager object.
*         pLocalAddr     - details of the transport address.
* Output: phLocalAddress - handle of the found Local Address object.
*                          NULL, if no address was found, or some error occured
******************************************************************************/
void RVCALLCONV TransportSctpMgrFindLocalAddressHandle(
                            IN  TransportMgr                  *pTransportMgr,
                            IN  TransportMgrLocalAddr         *pLocalAddr,
                            OUT RvSipTransportLocalAddrHandle *phLocalAddress);

/******************************************************************************
* TransportSctpEventCallback
* -----------------------------------------------------------------------------
* General:      handles events, raised by the Select Engine.
* Return Value: none.
* -----------------------------------------------------------------------------
* Arguments:
* Input:  selectEngine - the Select Engine.
*         fd           - file descriptor coresponding the registered socket.
*         event        - the type of the select event.
*         error        - indicates whether an error occured on the socket.
* Output: none.
******************************************************************************/
void TransportSctpEventCallback(
        IN RvSelectEngine*  selectEngine,
        IN RvSelectFd*      fd,
        IN RvSelectEvents   selectEvent,
        IN RvBool           error);

/******************************************************************************
* TransportSctpOpen
* -----------------------------------------------------------------------------
* General:      Creates listening or client socket and start to listen on it or
*               connect it to the remote socket.
* Return Value: RV_OK on success,
*               error code, defined in rverror.h or RV_SIP_DEF.h otherwise.
* -----------------------------------------------------------------------------
* Arguments:
* Input:  pTransportMgr  - pointer to the Transport Manager object.
*         pConn          - pointer to the Connection object, socket for which
*                          should be opened.
* Output: none.
******************************************************************************/
RvStatus RVCALLCONV TransportSctpOpen (IN TransportMgr         *pTransportMgr,
                                       IN TransportConnection  *pConn);

/******************************************************************************
* TransportSctpClose
* -----------------------------------------------------------------------------
* General:      Close the connection.
*               If the Connection object is in closing state, the socket will
*               be closed immediately. Otherwise, the gracefull shutdown will
*               be performed and the socket will be freed on CLOSE event.
*               The Connection object is freed.
* Return Value: RV_OK on success,
*               error code, defined in rverror.h or RV_SIP_DEF.h otherwise.
* -----------------------------------------------------------------------------
* Arguments:
* Input:   pConn - Pointer to the Connection object, connection of which
*                  should be closed.
******************************************************************************/
RvStatus RVCALLCONV TransportSctpClose (IN TransportConnection *pConn);

/******************************************************************************
* TransportSctpSend
* -----------------------------------------------------------------------------
* General:      sends the SIP message over the connection.
*               The message is kept inside list. Each Connection object has
*               its own list of outgoing messages.
*               Upon the successful call to the system send function,
*               the Connection object notifies its owners about this.
* Return Value: RV_OK on success,
*               error code, defined in rverror.h or RV_SIP_DEF.h otherwise.
* -----------------------------------------------------------------------------
* Arguments:
* Input:    pConn - Pointer to the Connection object.
* Output:   none.
******************************************************************************/
RvStatus RVCALLCONV TransportSctpSend (TransportConnection  *pConn);

/******************************************************************************
* TransportSctpWriteQueueEvent
* -----------------------------------------------------------------------------
* General:      Treats the WRITE event, raised by Select Engine for the
*               connection. If there are elements in the Connection list of
*               outgoing messages, the messages, kept in the elements will be
*               sent.
* Return Value: RV_OK on success,
*               error code, defined in rverror.h or RV_SIP_DEF.h otherwise.
* -----------------------------------------------------------------------------
* Arguments:
* Input:   pConn - the Connection object.
* Output:  none.
******************************************************************************/
RvStatus RVCALLCONV TransportSctpWriteQueueEvent (
                                        IN TransportConnection* pConn);

/******************************************************************************
* TransportSctpConnectedQueueEvent
* -----------------------------------------------------------------------------
* General:      Treats the CONNECTED event, raised by Select Engine: moves
*               the Connection object to the CONNECTED state.
* Return Value: RV_OK on success,
*               error code, defined in rverror.h or RV_SIP_DEF.h otherwise.
* -----------------------------------------------------------------------------
* Arguments:
* Input:    pConn - the connection.
*           error - RV_TRUE, if Select Engine handled error on connection.
* Output:  none.
******************************************************************************/
RvStatus RVCALLCONV TransportSctpConnectedQueueEvent(
                                         IN TransportConnection* pConn,
                                         IN RvBool               error);

/******************************************************************************
* TransportSctpDisconnectQueueEvent
* -----------------------------------------------------------------------------
* General:      Treats the DISCONNECT event, picked up from the Transport
*               Processing Queue (TPQ):initiates the Connection object freeing.
* Return Value: RV_OK on success,
*               error code, defined in rverror.h or RV_SIP_DEF.h otherwise.
* -----------------------------------------------------------------------------
* Arguments:
* Input:   pConn - the connection object.
* Output:  none.
******************************************************************************/
RvStatus RVCALLCONV TransportSctpDisconnectQueueEvent(
                                IN TransportConnection* pConn);

/******************************************************************************
* TransportSctpConnectionConstruct
* -----------------------------------------------------------------------------
* General:      Constructs data structures, used for support in SCTP,
*               such as lists of the multihoming addresses.
* Return Value: RV_OK on success,
*               error code, defined in rverror.h or RV_SIP_DEF.h otherwise.
* -----------------------------------------------------------------------------
* Arguments:
* Input:  pConn - pointer to the Connection object.
* Output: none.
******************************************************************************/
RvStatus RVCALLCONV TransportSctpConnectionConstruct(
                                    IN  TransportConnection* pConn);

/******************************************************************************
* TransportSctpConnectionDestruct
* -----------------------------------------------------------------------------
* General:      Destructs data structures, used by for SCTP support.
* Return Value: RV_OK on success,
*               error code, defined in rverror.h or RV_SIP_DEF.h otherwise.
* -----------------------------------------------------------------------------
* Arguments:
* Input:  pConn - pointer to the Connection object, socket for which
*                 should be opened.
* Output: none.
******************************************************************************/
void RVCALLCONV TransportSctpConnectionDestruct(
                                    IN  TransportConnection* pConn);

/******************************************************************************
* TransportSctpConnectionLocalAddressAdd
* -----------------------------------------------------------------------------
* General:      Binds the addresses to the connection socket.
*               On success, adds them to the list of the connection local
*               multihoming addresses. 
* Return Value: RV_OK on success,
*               error code, defined in rverror.h or RV_SIP_DEF.h otherwise.
* -----------------------------------------------------------------------------
* Arguments:
* Input:  pConn           - pointer to the Connection object.
*         pAddressDetails - array of the addresses to be added.
*         numOfAddrs      - number of addresses in the array.
* Output: none.
******************************************************************************/
RvStatus RVCALLCONV TransportSctpConnectionLocalAddressAdd(
                                    IN  TransportConnection *pConn,
                                    IN  RvSipTransportAddr  *pAddressDetails,
                                    IN  RvInt32             numOfAddrs);

/******************************************************************************
* TransportSctpConnectionLocalAddressRemove
* -----------------------------------------------------------------------------
* General:      Unbinds the addresses from the connection socket.
*               On success, removes them from the list of the connection local
*               multihoming addresses. 
* Return Value: RV_OK on success,
*               error code, defined in rverror.h or RV_SIP_DEF.h otherwise.
* -----------------------------------------------------------------------------
* Arguments:
* Input:  pConn           - pointer to the Connection object.
*         pAddressDetails - array of the addresses to be removed.
*         numOfAddrs      - number of addresses in the array.
* Output: none.
******************************************************************************/
RvStatus RVCALLCONV TransportSctpConnectionLocalAddressRemove(
                                    IN  TransportConnection *pConn,
                                    IN  RvSipTransportAddr  *pAddressDetails,
                                    IN  RvInt32             numOfAddrs);

/******************************************************************************
* TransportSctpConnectionLocalAddressFindByStrIp
* -----------------------------------------------------------------------------
* General:      Search the Connection list of the local multihoming addresses
*               for the asked IP. 
* Return Value: RV_OK on success,
*               error code, defined in rverror.h or RV_SIP_DEF.h otherwise.
* -----------------------------------------------------------------------------
* Arguments:
* Input:  pConn       - pointer to the Connection object.
*         strIp       - requested IP.
* Output: ppRvAddress - element from the list of multihoming addresses.
******************************************************************************/
RvStatus RVCALLCONV TransportSctpConnectionLocalAddressFindByStrIp(
                                    IN  TransportConnection *pConn,
                                    IN  RvChar*             strIp,
                                    OUT RvAddress           **ppRvAddress);

/******************************************************************************
* TransportSctpConnectionInsertIntoHash
* -----------------------------------------------------------------------------
* General:      Goes through the Connection list of the remote addresses and
*               inserts the Connection handle into the Connection Hash at
*               the {hLocalAddr:RemoteAddress} place.
*               Or inserts the connection into the hash by alias only.
*               Stores the pointer to the inserted hash element into
*               the Connection list of hash element pointers.
* Return Value: RV_OK on success,
*               error code, defined in rverror.h or RV_SIP_DEF.h otherwise.
* -----------------------------------------------------------------------------
* Arguments:
* Input:  pConn    - pointer to the Connection object.
*         bByAlias - if RV_TRUE - the Connection will be inserted into hash
*                    by alias, not by IPs.
* Output: none.
******************************************************************************/
RvStatus RVCALLCONV TransportSctpConnectionInsertIntoHash(
                                    IN TransportConnection *pConn,
                                    IN RvBool              bByAlias);

/******************************************************************************
* TransportSctpConnectionRemoveFromHash
* -----------------------------------------------------------------------------
* General:      Goes through the Connection list of the hash element pointers
*               and removes them fromthe hash. Empties the list of pointers.
*               As a result, the Connection is removed from the hash.
* Return Value: RV_OK on success,
*               error code, defined in rverror.h or RV_SIP_DEF.h otherwise.
* -----------------------------------------------------------------------------
* Arguments:
* Input:  pConn       - pointer to the Connection object.
* Output: none.
******************************************************************************/
void RVCALLCONV TransportSctpConnectionRemoveFromHash(
                                    IN  TransportConnection *pConn);

/******************************************************************************
* TransportSctpConnectionSetPrimaryAddr
* -----------------------------------------------------------------------------
* General:      Set the provided address as a SCTP session primary address.
*               Outgoing messages will be sent to this address.
*               Uses Socket Layer API for this.
*               The caller responsobility to ensure, that the provided address
*               is bound to the Connection socket.
* Return Value: RV_OK on success,
*               error code, defined in rverror.h or RV_SIP_DEF.h otherwise.
* -----------------------------------------------------------------------------
* Arguments:
* Input:  pConn           - pointer to the Connection object.
*         pAddressDetails - address to be set as a primary.
* Output: none.
******************************************************************************/
RvStatus RVCALLCONV TransportSctpConnectionSetPrimaryAddr(
                                    IN  TransportConnection *pConn,
                                    IN  RvSipTransportAddr  *pAddressDetails);

/******************************************************************************
* TransportSctpConnectionGetPrimaryAddr
* -----------------------------------------------------------------------------
* General:      Get the details of the address used currently as a SCTP session
*               primary address. Uses Socket Layer API for this.
* Return Value: RV_OK on success,
*               error code, defined in rverror.h or RV_SIP_DEF.h otherwise.
* -----------------------------------------------------------------------------
* Arguments:
* Input:  pConn           - pointer to the Connection object.
* Output: pAddressDetails - address to be set as a primary.
******************************************************************************/
RvStatus RVCALLCONV TransportSctpConnectionGetPrimaryAddr(
                                    IN  TransportConnection *pConn,
                                    OUT RvSipTransportAddr  *pAddressDetails);

/******************************************************************************
* TransportSctpConnectionInit
* -----------------------------------------------------------------------------
* General:      Initilizes the Connection object data structures related SCTP.
*               For example, copies list of the multihoming local addresses
*               from the hLocalAddr object into the Connection object to be 
*               initialized.
* Return Value: RV_OK on success,
*               error code, defined in rverror.h or RV_SIP_DEF.h otherwise.
* -----------------------------------------------------------------------------
* Arguments:
* Input:  pConn           - pointer to the Connection object.
* Output: none.
******************************************************************************/
RvStatus RVCALLCONV TransportSctpConnectionInit(
                                    IN  TransportConnection *pConn);

/******************************************************************************
* TransportSctpLocalAddressOpen
* -----------------------------------------------------------------------------
* General:      Create the Connection object, initilaizes it, opens the socket
*               and starts to listen on it.
*               Binds it to the Local Address object.
* Return Value: RV_OK on success,
*               error code, defined in rverror.h or RV_SIP_DEF.h otherwise.
* -----------------------------------------------------------------------------
* Arguments:
* Input:  pTransportMgr - Transport Manager object.
*         strSetOfIPs   - list of the local multihoming addresses.
*         pLocalAddr    - Local Address object, to be bound to the connection.
* Output: none.
******************************************************************************/
RvStatus RVCALLCONV TransportSctpLocalAddressOpen(
                                    IN  TransportMgr*          pTransportMgr,
                                    IN  RvChar*                strSetOfIPs,
                                    IN  TransportMgrLocalAddr* pLocalAddr);

/******************************************************************************
* TransportSctpLocalAddressGetNumIpv4Ipv6Ips
* -----------------------------------------------------------------------------
* General: Get number of IPv4 and IPv6 IPs, to which socket of SCTP Local
*          Address is bound.
* Return Value: RV_OK on success,
*               error code, defined in rverror.h or RV_SIP_DEF.h otherwise.
* -----------------------------------------------------------------------------
* Arguments:
* Input:   pLocalAddr    - Pointer to Local Address object
* Output:  pNumOfIpv4    - Number of IPv4 IPs, to which the socket is bound
*          pNumOfIpv6    - Number of IPv6 IPs, to which the socket is bound
******************************************************************************/
RvStatus RVCALLCONV TransportSctpLocalAddressGetNumIpv4Ipv6Ips(
                                    IN  TransportMgrLocalAddr*  pLocalAddr,
                                    OUT RvUint32*               pNumOfIpv4,
                                    OUT RvUint32*               pNumOfIpv6);

#endif /*#if (RV_NET_TYPE & RV_NET_SCTP)*/

#ifdef __cplusplus
}
#endif

#endif /*TRANSPORT_SCTP_H*/
