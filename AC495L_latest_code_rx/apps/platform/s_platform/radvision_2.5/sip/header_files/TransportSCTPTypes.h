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
 *                              <TransportSctpTypes.h>
 *
 * This file defines the internal API types, used by the SCTP module
 * of the Transport Layer. The module allows you to control sending and
 * receiving of messages over the SCTP network protocol.
 *
 *    Author                         Date
 *    ------                        ------
 *    Igor                          March 2005
 *****************************************************************************/


#ifndef TRANSPORT_SCTP_TYPES_H
#define TRANSPORT_SCTP_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "AdsHash.h"
#include "AdsRlist.h"

#include "RvSipTransportSctpTypes.h"
    

#if (RV_NET_TYPE & RV_NET_SCTP)

/*-----------------------------------------------------------------------*/
/*                          TYPE DEFINITIONS                             */
/*-----------------------------------------------------------------------*/

/*
 * TransportCfgSctp
 * ----------------
 * localSctpAddresses - array of local SCTP addresses, each of which is
 *                      representing by string, containing comma separated IPs.
 *                      On these addresses SIP TK will listen for incoming SCTP
 *                      connections.
 * localSctpPorts     - ports of the SCTP addresses.
 *                      On these ports SIP TK will listen for incoming SCTP
 *                      connections.
 * numOfSctpAddresses - number of local SCTP addresses in the array.
 * numOfOutStreams    - number of outbound streams,enabled for any SCTP socket.
 *                      Is set using SetSockOpt API on socket construct.
 * numOfInStreams     - number of inbound streams, enabled for any SCTP socket.
 *                      Is set using SetSockOpt API on socket construct.
 * avgNumOfMultihomingAddresses - average number of multihoming addresses
 *                      (both local and remote) kept by the Connection object.
 */
typedef struct {
    RvChar**                localSctpAddresses;
    RvUint16*               localSctpPorts;
    RvInt32                 numOfSctpAddresses;
    RvInt32                 numOfOutStreams;
    RvInt32                 numOfInStreams;
    RvInt32                 avgNumOfMultihomingAddresses;
}TransportCfgSctp;

/*
 * TransportMgrSctpData
 * --------------------
 * hSctpMultihomingAddressPool - pool of the RvAddress objects, from which
 *                       the objects are taken for the list of local and remote
 *                       multihoming addresses, kept in the Connection objects.
 * hSctpConnHashElemPointerPool - pool of the pointers to the elements of
 *                       the connection hash. The pool objects are used by
 *                       the Connection objetcs.
 * sctpAvgNumOfMultihomingAddresses - average number of multihoming addresses
 *                       (both local and remote) kept by the Connection object.
 * sctpNumOfOutStreams - number of outbound streams,enabled for any SCTP socket.
 *                       Is set using SetSockOpt API on socket construct.
 * sctpNumOfInStreams  - number of inbound streams, enabled for any SCTP socket.
 *                       Is set using SetSockOpt API on socket construct.
 * hSctpLocalAddrList  - list of Local Address objects, implementing addresses,
 *                       on which SIP TK listens for incoming SCTP connections.
 * sctpNumOfSctpAddresses - number of the opened local SCTP addresses.
*/
typedef struct  {
    RLIST_POOL_HANDLE   hSctpMultihomingAddressPool;
    RLIST_POOL_HANDLE   hSctpConnHashElemPointerPool;
    RvInt32             sctpAvgNumOfMultihomingAddresses;
    RvInt32             sctpNumOfOutStreams;
    RvInt32             sctpNumOfInStreams;
    RLIST_HANDLE        hSctpLocalAddrList;
    RvInt32             sctpNumOfSctpAddresses;
} TransportMgrSctpData;

/*
 * TransportConnectionSctpData
 * ---------------------------
 * hSctpLocalAddrsList - list of RvAddress objects, representing local
 *                       multihoming addresses.
 * hSctpRemoteAddrsList - list of RvAddress objects, representing remote
 *                       multihoming addresses.
 * hSctpConnHashElemPtrList - list o fpointers to the Connection hash elements.
 * sctpParams   - parameters,used on sending message over the Connection
 * numIPv4addrs - number of local multihoming IPs of IPv4 type 
 * numIPv6addrs - number of local multihoming IPs of IPv6 type
*/
typedef struct  {
    RLIST_HANDLE                        hSctpLocalAddrsList;
    RLIST_HANDLE                        hSctpRemoteAddrsList;
    RLIST_HANDLE                        hSctpConnHashElemPtrList;
    RvSipTransportSctpMsgSendingParams  sctpParams;
    RvUint32                            numIPv4addrs;
    RvUint32                            numIPv6addrs;
} TransportConnectionSctpData;

#endif /*#if (RV_NET_TYPE & RV_NET_SCTP)*/

#endif /*TRANSPORT_SCTP_TYPES_H*/

#ifdef __cplusplus
}
#endif

