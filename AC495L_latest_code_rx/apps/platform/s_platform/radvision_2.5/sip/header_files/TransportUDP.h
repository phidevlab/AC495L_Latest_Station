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


#ifndef TRANSPORT_UDP_H
#define TRANSPORT_UDP_H

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------*/
/*                        INCLUDE HEADER FILES                           */
/*-----------------------------------------------------------------------*/
#include "RV_SIP_DEF.h"
#include "TransportMgrObject.h"

/*-----------------------------------------------------------------------*/
/*                          TYPE DEFINITIONS                             */
/*-----------------------------------------------------------------------*/



/*-----------------------------------------------------------------------*/
/*                          FUNCTIONS HEADERS                            */
/*-----------------------------------------------------------------------*/
/************************************************************************************
 * TransportUDPOpen
 * ----------------------------------------------------------------------------------
 * General: open new udp socket
 *
 * Return Value: The new socket
 * ----------------------------------------------------------------------------------
 * Arguments:
 * Input:   pTransportMgr      - the transport info structure
 *          pAddress            - the ip address for openning the socket.
 *                                it is the local address that the socket is binded to.
 *                                the port would be taken from the hTransportHandle.
 *          pSocket             - pointer to a non initialize socket.
 * Output:  none
 ***********************************************************************************/
RvStatus RVCALLCONV TransportUDPOpen(
                                        IN  TransportMgr*    pTransportMgr,
                                        IN  RvAddress*       pAddress,
                                        OUT RvSocket*        pSocket);

/******************************************************************************
 * TransportUDPClose
 * ----------------------------------------------------------------------------
 * General: Closes UDP address - closes network socket, unregisters it from
 *          Select Engine, destructs FileDescriptor
 *
 * Return Value: RvStatus
 * ----------------------------------------------------------------------------
 * Arguments:
 * Input:   pTransportMgr   - pointer to the Transport manager
 *          pLocalAddr      - pointer to the address to be closed
 *****************************************************************************/
RvStatus RVCALLCONV TransportUDPClose(
                                    IN TransportMgr             *pTransportMgr,
                                    IN TransportMgrLocalAddr    *pLocalAddr);

/******************************************************************************
 * TransportUdpEventCallback
 * ----------------------------------------------------------------------------------
 * General:  Callback function that is called from the LI package whenever
 *           we are getting event from there.
 * Return Value: none
 * ----------------------------------------------------------------------------
 * Arguments:
 * input   : selectEngine   - Events engine of this fd
 *           fd             - File descriptor that this event occured on
 *           selectEvent    - Event that happened
 *           error          - RV_TRUE if an error occured
 * Output:  RvStatus
 ***********************************************************************************/
void TransportUdpEventCallback(
        IN RvSelectEngine*  selectEngine,
        IN RvSelectFd*      fd,
        IN RvSelectEvents   selectEvent,
        IN RvBool           error);

#ifdef __cplusplus
}
#endif

#endif /*TRANSPORT_UDP_H*/
