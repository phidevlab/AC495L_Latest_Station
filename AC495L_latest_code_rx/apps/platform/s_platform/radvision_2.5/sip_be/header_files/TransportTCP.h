/************************************************************************************************************************
NOTICE:
This document contains information that is proprietary to RADVision LTD..
No part of this publication may be reproduced in any form whatsoever without
written prior approval by RADVision LTD..

RADVision LTD. reserves the right to revise this publication and make changes
without obligation to notify any person of such revisions or changes.

************************************************************************************************************************/



/*********************************************************************************
 *                              TransportTCP.h
 *
 *  This file provides headers for the transport layer of the sip stack.
 *
 *
 *    Author                         Date
 *    ------                        ------
 *
 *********************************************************************************/


#ifndef TRANSPORT_TCP_H
#define TRANSPORT_TCP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "AdsRpool.h"
#include "TransportProcessingQueue.h"



/*-----------------------------------------------------------------------*/
/*                          TYPE DEFINITIONS                             */
/*-----------------------------------------------------------------------*/



/*-----------------------------------------------------------------------*/
/*                          FUNCTIONS HEADERS                            */
/*-----------------------------------------------------------------------*/

/************************************************************************************
 * TransportTcpEventCallback
 * ----------------------------------------------------------------------------------
 * General: Treats the events that may occur on a connection (Accept, Connect, Write, Read,
 *          close).
 * Return Value: RvStatus -
 * ----------------------------------------------------------------------------------
 * Arguments:
 * Input:   selectEngine - select engine which we registered on
 *          fd      - file descriptor coresponding with registered socket
 *          event   - The type of the event
 *          error   - indicates whether an error occured in the li level
 * Output:  (-)
 ***********************************************************************************/
void TransportTcpEventCallback(
        IN RvSelectEngine*  selectEngine,
        IN RvSelectFd*      fd,
        IN RvSelectEvents   selectEvent,
        IN RvBool           error);

/******************************************************************************
 * TransportTCPOpen
 * ----------------------------------------------------------------------------
 * General: opens connection to destination address
 * Return Value: RvStatus.
 * ----------------------------------------------------------------------------
 * Arguments:
 * Input:  pTransportMgr   - the pointer to the transport.
 *         pConn           - The pointer to the connection.
 *         bConnect        - If RV_FALSE, the socket will not be connected.
 *                           In this case TransportTCPConnect should be called.
 *         clientLocalPort - Port, to which the socket will be bound.
 *                           This parameter is actual for client connections.
 *                           Listening connections use port of their Local
 *                           Address objects.
*****************************************************************************/
RvStatus RVCALLCONV TransportTCPOpen (IN TransportMgr*        pTransportMgr,
                                      IN TransportConnection* pConn,
                                      IN RvBool               bConnect,
                                      IN RvUint16             clientLocalPort);

/******************************************************************************
 * TransportTCPConnect
 * ----------------------------------------------------------------------------
 * General: connects socket of client connection to destination address
 * Return Value: RvStatus.
 * ----------------------------------------------------------------------------
 * Arguments:
 * Input:  pConn - The pointer to the connection.
 *****************************************************************************/
RvStatus RVCALLCONV TransportTCPConnect (IN TransportConnection* pConn);

/************************************************************************************
 * TransportTCPClose
 * ----------------------------------------------------------------------------------
 * General: Close the connection.
 *          if the status of the connection is closing then close immediatelly
 *           the connection.
 *           otherwise, call shutdown to tell the other side to close also, and wait for
 *           closed event to occur on the socket and just then, close the connection
 *           and remove from the list.
 * Return Value: RvStatus.
 * ----------------------------------------------------------------------------------
 * Arguments:
 * Input:   pConn - Pointer to the relevant connection.
 ***********************************************************************************/
RvStatus RVCALLCONV TransportTCPClose (IN TransportConnection *pConn);

/************************************************************************************
 * TransportTCPSend
 * ----------------------------------------------------------------------------------
 * General: send the data over the connection. The data is kept inside list.
 *           each connection has its own list of outgoing messages and when the connection
 *           sends the message successfully it notifies its owner so that the owner
 *           can decide wheather to free the page of the message or not.
 * Return Value: RvStatus
 * ----------------------------------------------------------------------------------
 * Arguments:
 * Input:   pTcpConnInfo - Pointer to the relevante connection.
 ***********************************************************************************/
RvStatus RVCALLCONV TransportTCPSend (TransportConnection  *pConn);

 /************************************************************************************
 * TransportTcpWriteQueueEvent
 * ----------------------------------------------------------------------------------
 * General: Treats the write event from the li package.
 *          This events indicates that we are ready to write, so if we have something - we
 *          should send it.
 *
 * Return Value: RvStatus -
 * ----------------------------------------------------------------------------------
 * Arguments:
 * Input:   pTransportMgr - Transport manager object.
 *          ev - pointer to the PQUEUE event.
 * Output:  RV_Satus
 ***********************************************************************************/
RvStatus RVCALLCONV TransportTcpWriteQueueEvent (
                                IN TransportMgr* pTransportMgr,
                                IN TransportProcessingQueueEvent    *ev);

/************************************************************************************
 * TransportTcpConnectedQueueEvent
 * ----------------------------------------------------------------------------------
 * General: treats the CONNECTED_EVENT multi-threaded processing queue event.
 * Return Value: RvStatus
 * ----------------------------------------------------------------------------------
 * Arguments:
 * Input:   ev            - Pointer to the event.
 * Output:  none
 ***********************************************************************************/
RvStatus RVCALLCONV TransportTcpConnectedQueueEvent(IN TransportProcessingQueueEvent    *ev);

/************************************************************************************
 * TransportTcpDisconnectQueueEvent
 * ----------------------------------------------------------------------------------
 * General: Treats the Disconnect event from the processing queue package.
 *
 * Return Value: RvStatus -
 * ----------------------------------------------------------------------------------
 * Arguments:
 * Input:   pTcpConnInfo - Pointer to the relevante connection.
 *          socket     - Pointer to the relevant connection info structure.
 * Output:
 ***********************************************************************************/
RvStatus RVCALLCONV TransportTcpDisconnectQueueEvent (IN TransportProcessingQueueEvent    *ev);

/************************************************************************************
 * TransportTcpHandleReadEvent
 * ----------------------------------------------------------------------------------
 * General: Treats the read event from the li package.
 *
 * Return Value: RvStatus
 * ----------------------------------------------------------------------------------
 * Arguments:
 * Input:   pConn - Pointer to the relevant connection.
 *          socket     - Pointer to the relevant connection info structure.
 * Output:
 ***********************************************************************************/
RvStatus RVCALLCONV TransportTcpHandleReadEvent (IN TransportConnection   *pConn);

/************************************************************************************
 * TransportTcpHandleWriteEvent
 * ----------------------------------------------------------------------------------
 * General: Handles write event for TCP
 *
 * Return Value: (-)
 * ----------------------------------------------------------------------------------
 * Arguments:
 * Input:   pTcpConn - Pointer to the relevant connection.
 *          event    - The event to be treated
 * Output:
 ***********************************************************************************/
RvStatus RVCALLCONV TransportTcpHandleWriteEvent(IN TransportConnection   *pConn);

#endif /*TRANSPORT_TCP_H*/

#ifdef __cplusplus
}
#endif

