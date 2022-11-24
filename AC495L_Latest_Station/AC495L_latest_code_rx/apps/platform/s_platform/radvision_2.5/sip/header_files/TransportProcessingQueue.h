
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
 *                              TransportEventQueue.h
 *
 *  This file provides internal definitions for the transport layer event
 *  queue
 *
 *
 *
 *    Author                         Date
 *    ------                        ------
 *********************************************************************************/


#ifndef TRANSPORT_PROCESSINF_QUEUE_H
#define TRANSPORT_PROCESSINF_QUEUE_H


#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------*/
/*                        INCLUDE HEADER FILES                           */
/*-----------------------------------------------------------------------*/
#include "RvSipTransport.h"
#include "_SipTransportTypes.h"
#include "TransportConnection.h"

/*-----------------------------------------------------------------------*/
/*                                  MACROS                               */
/*-----------------------------------------------------------------------*/
#define TRANSPORT_READ_BUFFRES_READY   0
#define TRANSPORT_EVENT_LOG_LEN        (65)

/*-----------------------------------------------------------------------*/
/*                          TYPE DEFINITIONS                             */
/*-----------------------------------------------------------------------*/
typedef RvUint8 TransportNotificationMsg;



/*
 * transportQueueEventType
 * ----------------------
 * This enum defines types of queue events
 */
typedef enum {
    TIMER_EXPIRED_EVENT = 1,
    MESSAGE_RCVD_EVENT,
    WRITE_EVENT,
    CONNECTED_EVENT,
    DISCONNECTED_EVENT,
    INTERNAL_OBJECT_EVENT,
    EXTERNAL_OBJECT_EVENT
} transportQueueEventType;




/* TransportMsgReceivedEvent
 * --------------------------------------------------------------------------
 * TransportMsgReceivedEvent structure represents event specific info of the
 * MESSAGE_RCVD_EVENT event of the processing queue.
 *
 * bytesRecv        - number of received by 'read' bytes.
 * receivedBuffer    - pointer to receivied SIP message
 * localAddr        - local address where SIP message was received
 * rcvFromAddr        - address from where SIP message was received
 * sipMsgSize        - used only for SIP messages, received from TCP socket.
 *                    Size of SIP headers of the received message
 * sdpLength        - used only for SIP messages, received from TCP socket.
 *                    Size of SDP part of the received message
 * pConnInfo        - used only for SIP messages, received from TCP socket.
 *                    Pointer to TCP connection where the mesage was received
 * pTransportMgr    - used only for SIP messages, received from UDP socket.
 *                    Pointer to transport info structure
 * hInjectedMsg     - Handle to an already parsed message, that was injected, and
 *                    was not received from network. when handling this event,
 *                    we will first check this parameter, and if not exist,
 *                    we will parse the message in receivedBuffer.
 * sigCompMsgInfo   - Contains event SigComp specific info of the received message
 */

typedef struct {
    RvInt32                          bytesRecv;
    RvUint8                         *receivedBuffer;
    RvSipTransportLocalAddrHandle    hLocalAddr;
    SipTransportAddress              rcvFromAddr;
    RvInt32                          sipMsgSize;
    RvInt32                          sdpLength;
    RvSipTransportConnectionHandle   hConn;
    TransportMgr                    *pTransportMgr;
    RvSipMsgHandle                   hInjectedMsg;
#ifdef RV_SIGCOMP_ON
    SipTransportSigCompMsgInfo       sigCompMsgInfo;
#endif /* RV_SIGCOMP_ON */
} TransportMsgReceivedEvent;

/* TransportTcpConnectedEvent
 * --------------------------------------------------------------------------
 * TransportTcpConnectedEvent structure represents event specific info of the
 * RVSIP_TRANSPORT_CONNECTION_STATE_TCP_CONNECTED event of the processing queue. This event
 * is relevant only for TCP connections.
 *
 * pConnInfo        - Pointer to TCP connection where the mesage was received
 * error            - specifies if errors occured during CMT processing
 */
typedef struct {
    TransportConnection* pConnInfo;
    RvBool                 error;
} TransportTcpConnectedEvent;


/* TransportTcpDisconnectedEvent
 * --------------------------------------------------------------------------
 * TransportTcpDisconnectedEvent structure represents event specific info of the
 * DISCONNECTED_EVENT event of the processing queue. This event is relevant only
 * for TCP connections.
 *
 * pConnInfo        - Pointer to TCP connection where the mesage was received
 */
typedef struct {
    TransportConnection *pConnInfo;
} TransportTcpDisconnectedEvent;


/* TransportTcpWriteEvent
 * --------------------------------------------------------------------------
 * TransportTcpWriteEvent structure represents event specific info of the
 * WRITE_EVENT event of the processing queue. This event is relevant only
 * for TCP connections.
 *
 * pConnInfo        - Pointer to TCP connection where the mesage was received
 */
typedef struct {
    TransportConnection *pConnInfo;
} TransportTcpWriteEvent;

/* TransportObjectEvent
 * --------------------------------------------------------------------------
 * TransportObjectEvent structure represents event specific info
 * of the INTERNAL_OBJECT_EVENT event of the processing queue.
 *
 * obj         - Pointer to object to be deleted (e.g transaction or calleg)
 * eventFunc   - Function to be applied when the event is poped from the queue.
 *               Can be NULL.
 *               This function receives only 1 parameter.
 *               Can't be extended, because it uses API types.
 *               Should be avoided to use. Use eventFunc1.
 *               The func was left for backward comaptibility.
 * eventFuncEx - Function to be applied when the event is poped from the queue.
 *               The function is extension of eventFunc.
 *               Number of parameters the function receives, can be changed.
 *               Can be NULL.
 * param1      - parameter to be passed to eventFuncEx (e.g. state enumeration)
 * param2      - parameter to be passed to eventFuncEx (e.g. reason enumeration)
 */
typedef struct {
    void*                            obj;
    RvSipTransportObjectEventHandler eventFunc;
    SipTransportTpqEventHandler      eventFuncEx;
    RvInt32                          param1;
    RvInt32                          param2;
#if (RV_LOGMASK & (RV_LOGLEVEL_DEBUG|RV_LOGLEVEL_ERROR))
    RvChar                           strLoginfo[TRANSPORT_EVENT_LOG_LEN];
#endif
} TransportObjectEvent;

/* TransportProcessingQueueEvent
 * ---------------------------------------------------------------------------
 * the TransportProcessingQueueEvent defines processing queue event object.
 *
 * memPage          -- A memory page where event was allocated.
 * type                -- event type.
 * evData            -- type specific event data
 */
typedef struct {
    transportQueueEventType  type;
    union
    {
        TransportMsgReceivedEvent        msgReceived;
        TransportTcpConnectedEvent        connected;
        TransportTcpDisconnectedEvent    disconnected;
        TransportTcpWriteEvent            write;
        TransportObjectEvent            objEvent;
    } typeSpecific;
} TransportProcessingQueueEvent;  /*PQUEUE_Event*/

/************************************************************************************
 * TransportProcessingQueueAllocateEvent
 * ----------------------------------------------------------------------------------
 * General:  Allocates and initiates event, according to the event type.
 * Return Value: RvStatus
 * ----------------------------------------------------------------------------------
 * Arguments:
 * Input:   hTtransport - handle to transport manager structure
 *            hConn          - optional pointer to TCP connection info structure
 *            type          - event type
 *          bAllocateRcvdBuff - allocate rcvdBuffer or not. (for injected message
 *                          there is no need of this buffer)
 * Output:  ev              - pointer to allocated processing event
 ***********************************************************************************/
RvStatus RVCALLCONV TransportProcessingQueueAllocateEvent(
                               IN  RvSipTransportMgrHandle            hTtransport,
                               IN  RvSipTransportConnectionHandle   hConn,
                               IN  transportQueueEventType            type,
                               IN  RvBool                          bAllocateRcvdBuff,
                               OUT TransportProcessingQueueEvent    **ev);


/************************************************************************************
 * TransportProcessingQueueTailEvent
 * ----------------------------------------------------------------------------------
 * General: Send Event through the processing queue
 *
 * Return Value: RvStatus - RV_OK
 *                           RV_ERROR_UNKNOWN
 *                           RV_ERROR_BADPARAM
 * ----------------------------------------------------------------------------------
 * Arguments:
 * Input:   hTtransport    - Handle to transport instance
 *          ev             - event to be sent
 * Output:  none
 ***********************************************************************************/
RvStatus RVCALLCONV TransportProcessingQueueTailEvent(
                        IN  RvSipTransportMgrHandle            hTtransport,
                        IN  TransportProcessingQueueEvent    *ev);


/************************************************************************************
 * TransportProcessingQueueFreeEvent
 * ----------------------------------------------------------------------------------
 * General:  Allocates and initiates message received event.
 * Return Value: RvStatus returned when thread is terminated
 * ----------------------------------------------------------------------------------
 * Arguments:
 * Input:   hTransport     - pointer to transport info structure
 *            ev             - pointer to allocated processing event
 ***********************************************************************************/
RvStatus RVCALLCONV TransportProcessingQueueFreeEvent(IN RvSipTransportMgrHandle        hTransport,
                                           IN  TransportProcessingQueueEvent    *ev);


/************************************************************************************
 * TransportProcessingQueueThreadDispatchEvents
 * ----------------------------------------------------------------------------------
 * General:  Main function of processing thread. Waits for events to arrive
 *           in the processing queue. Deletes the event from the queue and process
 *           specified by the event request.
 * Return Value: RvStatus returned when thread is terminated
 * ----------------------------------------------------------------------------------
 * Arguments:
 * Input:   pContext - pointer to transport mgr info structure
 * Output:  none
 ***********************************************************************************/
void TransportProcessingQueueThreadDispatchEvents(
                                          IN RvThread *pProcThread,
                                          IN void     *pContext);

/************************************************************************************
 * TransportProcessingQueueDispatchEvents
 * ----------------------------------------------------------------------------------
 * General:  Main function of processing thread/preemption cb. Waits for events to arrive
 *           in the processing queue. Deletes the event from the queue and process
 *           specified by the event request.
 * Return Value: RvStatus returned when thread is terminated
 * ----------------------------------------------------------------------------------
 * Arguments:
 * Input:   pTransportMgr - pointer to transport info structure
 * Output:  none
 ***********************************************************************************/
RvStatus RVCALLCONV TransportProcessingQueueDispatchEvents(IN TransportMgr *pTransportMgr);

/************************************************************************************
 * TransportProcessingQueueResourceAvailable
 * ----------------------------------------------------------------------------------
 * General: Resource available callback:
 *          First try and resend events.
 *          Than try and re-notify connections on events that were OOR-ed before
 *
 * Return Value: RvStatus
 * ----------------------------------------------------------------------------------
 * Arguments:
 * Input:     pMgr        - Transport manager handler
 *          msg         - This message indicates the type of resource that was freed: PQ cell or read buffer
 * Output:  none
 ***********************************************************************************/
RvStatus TransportProcessingQueueResourceAvailable(IN TransportMgr* pMgr,
                                                  IN TransportNotificationMsg msg);

#ifdef __cplusplus
}
#endif

#endif  /*END OF: define TRANSPORT_PROCESSINF_QUEUE_H*/
