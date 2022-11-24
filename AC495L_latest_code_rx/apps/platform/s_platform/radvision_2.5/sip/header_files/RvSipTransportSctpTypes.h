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
 *                              <RvSipTransportSctpTypes.h>
 *
 * This file defines the API types, used by the SCTP module of the Transport
 * Layer. The module allows you to control sending and receiving of messages
 * over the SCTP network protocol.
 *
 *    Author                         Date
 *    ------                        ------
 *    Igor                          March 2005
 *****************************************************************************/


#ifndef RVSIP_TRANSPORTSCTPTYPES_H
#define RVSIP_TRANSPORTSCTPTYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------*/
/*                        INCLUDE HEADER FILES                           */
/*-----------------------------------------------------------------------*/

#include "RV_SIP_DEF.h"

#if (RV_NET_TYPE & RV_NET_SCTP)

/*-----------------------------------------------------------------------*/
/*                          TYPE DEFINITIONS                             */
/*-----------------------------------------------------------------------*/
#define RVSIP_TRANSPORT_SCTP_MULTIHOMED_ADDR_NUM_AVG_PER_CONN (6)

#define RVSIP_TRANSPORT_SCTP_SENDMSG_FLAG_ORDERED (0x1)

/*  RvSipTransportSctpMsgSendingParams
 * --------------------------------
 * The structure unifies a number of parameters, used while sending
 * the message over SCTP. It should be used with
 * RvSipXXXSetSctpMsgSendingParams and RvSipXXXGetSctpMsgSendingParams
 * ("XXX" stands for CallLeg, RegClient, Transaction, Transmitter and Conn).
 *
 * sctpStream
 * ---------------------
 * ID of the stream, on which the message should be sent.
 * Default value: 0
 *
 * sctpMsgSendingFlags
 * ----------------------
 * Flags, joined with "&" that should be set into the outgong message.
 * For example, RVSIP_TRANSPORT_SCTP_SENDMSG_FLAG_ORDERED flag.
 * Default Value: 0
 */
typedef struct
{
    RvInt32 sctpStream;
    RvInt32 sctpMsgSendingFlags;
} RvSipTransportSctpMsgSendingParams;

#endif /* #if (RV_NET_TYPE & RV_NET_SCTP) */

#ifdef __cplusplus
}
#endif


#endif /* END OF: #ifndef RVSIP_TRANSPORTSCTPTYPES_H */



