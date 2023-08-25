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
 *                              SipMsgBuilder.h
 *
 *  This file provides headers for the message builder object of the sip stack.
 *
 *
 *    Author                         Date
 *    ------                        ------
 *  Oren Libis                    20-Nov-2000
 *********************************************************************************/

#ifndef TRANSPORT_MSG_BUILDER_H
#define TRANSPORT_MSG_BUILDER_H


#ifdef __cplusplus
extern "C" {
#endif



/*-----------------------------------------------------------------------*/
/*                        INCLUDE HEADER FILES                           */
/*-----------------------------------------------------------------------*/

#include "_SipCommonUtils.h"
#include "AdsRpool.h"
#include "TransportConnection.h"

/*-----------------------------------------------------------------------*/
/*                           TYPE DEFINITIONS                            */
/*-----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/*                          FUNCTIONS HEADERS                            */
/*-----------------------------------------------------------------------*/

/************************************************************************************
 * TransportMsgBuilderReceivedMsg
 * ----------------------------------------------------------------------------------
 * General: implementation for the transport call back.
 *
 * Return Value: none
 * ----------------------------------------------------------------------------------
 * Arguments:
 * Input:   pTransportMgr - pointer to the transport
 *          bytesRecv      - the number of bytes that were received from the network.
 *          pBuffer        - the buffer that was received from the network.
 *          pLocalAddr     - The local address to which the message was received.
 *          pRecvFromAddr  - The address from which the message was received.
 *          hInjectedMsg   - If there is an 'injected' msg, we should not try to
 *                           parse the udp message, but take this message instead.
 * Output:  none.
 ***********************************************************************************/
RvStatus RVCALLCONV TransportMsgBuilderReceivedMsg(
                                IN  TransportMgr                   *pTransportMgr,
                                IN  RvInt32                         bytesRecv,
                                IN  RvChar                         *pBuffer,
                                IN  RvSipTransportConnectionHandle  hConn,
                                IN  RvSipTransportLocalAddrHandle   hLocalAddr,
                                IN  SipTransportAddress*            pRecvFromAddr,
                                IN  RvSipMsgHandle                  hInjectedMsg);

/************************************************************************************
 * TransportMsgBuilderTcpProcessMsg
 * ----------------------------------------------------------------------------------
 * General: The TransportMsgBuilderTcpProcessMsg uses to process multi-threaded
 *          message received events for TCP connections. Each time message received
 *          event encountered for TCP connection, the events processing function
 *            copies event information, frees event and applies the
 *            TransportMsgBuilderTcpProcessMsg function with the event info.
 * Return Value: RvStatus - RV_OK or returned by subsequent calls error value
 * ----------------------------------------------------------------------------------
 * Arguments:
 * Input:   pBuffer          - the SIP message buffer (before parsing).
 *            totalMsgSize     - number of bytes in entire SIP message.
 *            pConn             - pointer to the connection.
 *          sipMsgSize         - SIP part lenght of the message (without SDP).
 *          sdpLength         - SDP part lenght of the message.
 *          localAddr        - local address where the message was sent.
 *            recvFrom         - address from where the message was sent.
 *          hInjectedMsg     - If there is an 'injected' msg, we should not try to
 *                             parse the udp message, but take this message instead.
 *          pSigCompInfo     - Transport SigComp information related to the message.
 * Output:  none
 ***********************************************************************************/
RvStatus RVCALLCONV TransportMsgBuilderTcpProcessMsg(
                            IN RvChar                      *pBuffer,
                            IN RvInt32                       totalMsgSize,
                            IN TransportConnection          *pConn,
                            IN RvInt32                       sipMsgSize,
                            IN RvUint32                   sdpLength,
                            IN RvSipTransportLocalAddrHandle hLocalAddr,
                            IN SipTransportAddress        *recvFrom,
                            IN RvSipMsgHandle              hInjectedMsg,
                            IN SipTransportSigCompMsgInfo *pSigCompInfo);

/************************************************************************************
* TransportMsgBuilderTcpAccumulateMsg
* ----------------------------------------------------------------------------------
* General:    the TransportMsgBuilderTcpAccumulateMsg used in multi threaded SIP
*            stack configuration (CMT thread). It goes through the buffer and
*            read all the sip messages that in it (if more then 1).
*            this function calls the prepare function to remove crlfs,and the Set
*            function which set the body.
*            the prepare function returns the number of bytes actually was read from
*            the buffer. and if any bytes left unprocessed, then move on (with the while)
*            and process them as new sip msg. When single SIP message boundaries are
*            discovered, MESSAGE_RECEIVED event is created and sent to the processing queue.
* Return Value: RvStatus - RV_OK, RV_ERROR_UNKNOWN
* ----------------------------------------------------------------------------------
* Arguments:
* Input:   pBuf        - the buffer that was received from the network
*          bufSize     - the buffer size.
*          conn           - the connection
*          pLocalAddr  - The local address to which the message was received
* Output:  none
***********************************************************************************/
RvStatus RVCALLCONV TransportMsgBuilderTcpAccumulateMsg(
                            IN RvChar                       *pBuf,
                            IN RvUint32                      bufSize,
                            IN TransportConnection          *conn,
                            IN RvSipTransportLocalAddrHandle hLocalAddr);

#if (RV_LOGMASK != RV_LOGLEVEL_NONE)
/************************************************************************************
 * TransportMsgBuilderPrintMsg
 * ----------------------------------------------------------------------------------
 * General: log print function. used only for printing the buffer to the log.
 *
 * Return Value: none
 * ----------------------------------------------------------------------------------
 * Arguments:
 * Input:   pTransportMgr    - pointer to the transport instance
 *          pBuf             - the buffer that was received from the network.
 *          msgDirection     - The direction of the sent message.
 *          bAdditional      - Indication if the request to print a message is in
 *                             addition to the general flow (for example before
 *                             compression of after decompression)
 * Output:  none
 ***********************************************************************************/
void RVCALLCONV TransportMsgBuilderPrintMsg(
                     IN TransportMgr                     *pTransportMgr,
                     IN RvChar                           *pBuf,
                     IN SipTransportMgrBuilderPrintMsgDir msgDirection,
                     IN RvBool                            bAdditional);
#else
#define TransportMsgBuilderPrintMsg(_t,_p,_s,_e)
#endif /*#if (RV_LOGMASK != RV_LOGLEVEL_NONE)*/




#ifdef __cplusplus
}
#endif

#endif  /*END OF: define _SIP_MSG_BUILDER_H*/
