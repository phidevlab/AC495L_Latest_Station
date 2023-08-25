/***********************************************************************
        Copyright (c) 2002 RADVISION Ltd.
************************************************************************
NOTICE:
This document contains information that is confidential and proprietary
to RADVISION Ltd.. No part of this document may be reproduced in any
form whatsoever without written prior approval by RADVISION Ltd..

RADVISION Ltd. reserve the right to revise this publication and make
changes without obligation to notify any person of such revisions or
changes.
***********************************************************************/

#ifndef __RTPUTIL_H
#define __RTPUTIL_H


#include "rvselect.h"
#include "rvnet2host.h"
#include "rvtimer.h"
#include "rvlog.h"
#include "rvloglistener.h"
#include "rvrtplogger.h"
#include "rtp.h"
#include "rvrtpencryptionplugin.h"
#include "rvrtpencryptionkeyplugin.h"
#include "rvrtpaddresslist.h"
#include "rvrandomgenerator.h"
#include "rtpProfilePlugin.h"
#include "rvrtpnatfw.h"

#ifdef __cplusplus
extern "C" {
#endif

#define RTP_DESTINATION_UNREACHABLE  (RvSocketErrorCode(RV_ERROR_UNKNOWN))
/* This struct is here because some applications like to include it in their own code and access
   it directly (bad, but what can we do?) */
typedef struct
{
    RvBool                   isAllocated;            /* RV_TRUE if the RTP package allocated this struct internally */
    RvSocket                 socket;                 /* UDP socket used for this session */
    RvSelectEngine           *selectEngine;          /* Select engine used by this fd */
    RvSelectFd               selectFd;               /* fd object to use for non-blocking mode */
    RvUint32                 sSrc;                   /* Synchronization source (SSRC) */
    RvUint32                 sSrcMask;
    RvUint32                 sSrcPattern;
    RvRtpEventHandler_CB     eventHandler;           /* RTP received message indication event handler */
    void *                   context;                /* RTP received message indication event handler context */
    RvUint16                 sequenceNumber;         /* RTP session's sequence number */
	RvRtpAddressList         addressList;            /* address list to store the remote addresses for unicast/multicast */
  /*  RvAddress              remoteAddress;              Remote address of this session */
    RvBool                   remoteAddressSet;       /* RV_TRUE if we already set the remote address */
    RvBool                   useSequenceNumber;      /* TRUE, if it is used users sequence numbers, FALSE - automatic */
    RvRtcpSession            hRTCP;                  /* Associate RTCP session if available */
/* Security hooks*/
	RvRtpEncryptionPlugIn    *encryptionPlugInPtr;   /* Registered encryption plug in */
	RvRtpEncryptionKeyPlugIn *encryptionKeyPlugInPtr; /* Registered encryption key plug in */
	RvRtpEncryptionMode      encryptionMode;          /* RFC1889, H235_PADDING, H235_CIPHERTEXTSTEALING */

    RtpProfilePlugin*        profilePlugin;           /* profile plugin for RTP/SRTP */
/* SRTP plugin information */
    RvUint32                 roc;                     /* srtp rollover counter   */
#ifdef __H323_NAT_FW__
    RvRtpDemux               demux;                   /* handle of demultiplexing obj */
    RvUint32                 demuxTblEntry;           /* index of entry in demux table */
#endif /* __H323_NAT_FW__ */

    RvLock                   lock;     
    
} RvRtpSessionInfo;                                   /* RvRtpSession */

 
typedef struct _rtpLogManager
{
	RvBool                bExternal;        /* TRUE if External Log Manager is executed */
	RvLogMgr              logMngr;          /* used when internal logmanager executed */

    RvLogSource           logSource[RVRTP_MODULES_NUMBER]; /* all log sources for RTP lib */
    RvBool                bLogSourceInited[RVRTP_MODULES_NUMBER]; /* TRUE, if logsource for specific module is initialized */
    
	RvLogListener         listener;         /* log listener */
    RvRtpPrintLogEntry_CB printCB;          /* logger entry print external function */
	void*                 logContext;       /* context for printCB */

} RvRtpLogManager;


#define RV_RTP_MAXIPS RvUint32Const(20)
/* Internal RTP instance used. There a single such object */
typedef struct
{
    RvSelectEngine*       selectEngine;           /* Select engine used */
    
    RvAddress             rvLocalAddress;         /* local address */
    RvAddress             hostIPs[RV_RTP_MAXIPS]; /* Local host IPs */
    RvUint32              addressesNum;           /* maximal number of addresses stored */
    RvRandomGenerator     randomGenerator;        /* Random numbers generator to use */
    RvRtpLogManager       logManager;             /*  Logger mannager */
    
} RvRtpInstance;

/* Private functions, which are not accessible to user */ 
RVAPI
void   RVCALLCONV ConvertToNetwork(void *data, RvInt32 pos, RvInt32 n);
RVAPI
void   RVCALLCONV ConvertFromNetwork(void *data, RvInt32 pos, RvInt32 n);

RVAPI
void   RVCALLCONV ConvertToNetwork2(void *data, RvInt32 pos, RvInt32 n);
RVAPI
void   RVCALLCONV ConvertFromNetwork2(void *data, RvInt32 pos, RvInt32 n);

RVAPI
RvBool   RVCALLCONV  isMyIP(RvAddress* address);


RvStatus      rtpGetLogManager(OUT RvRtpLogManager** lmngrPtr);
/* Private function only to allow access to the SRTP module */
RVAPI
RvLogSource* RVCALLCONV   rtpGetSource(RvRtpModule module);

RvBool   rtcpSessionIsSessionInShutdown(
	 IN RvRtcpSession hRTCP);
/* Private function only to allow access to the SRTP module */
RVAPI
RvUint32  RVCALLCONV rtcpSessionGetIndex(
     IN RvRtcpSession hRTCP);

RvStatus rtcpSetRtpSession(
     IN RvRtcpSession hRTCP, 
     IN RvRtpSession hRTP);

/* Private function only to allow access to the SRTP module */
RVAPI
RvStatus  RVCALLCONV RvRtcpSetProfilePlugin(
    IN RvRtcpSession hRTCP, 
    IN RtpProfilePlugin* profilePlugin);


/*******************************************************************************
 * RtpSendPacket
 * scope: Private
 * description sends regular or multiplexed packet to the remote peer
 *
 * input:
 *    socketPtr      - pointer to socket
 *    buffer         - filled buffer
 *    p              - pointer to filled RvRtpParam structure
 *    natAddressPtr  - pointer to RtpNatAddress
 *    paddingSize    - size of padding
 * output: none
 * return value: Non-negative value on success
 *               Negative value on failure
 *******************************************************************************/
RVAPI
RvStatus  RVCALLCONV RtpSendPacket(
    IN RvSocket* socketPtr, 
    IN RvUint8* buffer,
    IN RvUint32 paddingSize,
    IN RvRtpParam* p,
    IN RtpNatAddress* natAddressPtr);


/************************************************************************************
 * RtpOpenFrom
 * description: Opens an RTP session in the memory that the application allocated.
 * scope:       private
 * input: demux       - demux obkect handle, if demultiplexing is supported, othrwise NULL
 *        pRtpAddress - contains The UDP port number to be used for the RTP session.
 *        ssrcPattern - Synchronization source Pattern value for the RTP session.
 *        ssrcMask    - Synchronization source Mask value for the RTP session.
 *        buffer      - Application allocated buffer with a value no less than the
 *                      value returned by the function RvRtpGetAllocationSize().
 *        bufferSize  - size of the buffer.
 * output: none.
 * return value: If no error occurs, the function returns the handle for the opened RTP
 *               session. Otherwise, it returns NULL.
 *Note:
 *	1) RtpOpenFrom opens an RTP session in the memory that the application!!! allocated.
 *     therefore RvRtpSessionShutdown should not be used.
 *  2) RtpOpenFrom opens one socket with the same port for receiving and for 
 *     sending (no demultiplexing).
 *  3) if demultiplexing is supported first session will be opened with demux=NULL
 *     for other demultiplexed sessions association of RTP session with shared socket
 *     will be in function RvRtpDemuxOpenSession
 ***********************************************************************************/
RVAPI
RvRtpSession RVCALLCONV RtpOpenFrom(
    IN  RvRtpDemux    demux,
    IN  RvNetAddress* pRtpAddress,
    IN  RvUint32      ssrcPattern,
    IN  RvUint32      ssrcMask,
    IN  void*         buffer,
    IN  RvInt32       bufferSize);

/************************************************************************************
 * RtpOpenEx
 * description: Opens an RTP session and an associated RTCP session.
 * input: demux       - demux handle, if demultiplexing is supported, NULL - otherwise
 *        pRtpAddress - contains  the UDP port number to be used for the RTP session.
 *        ssrcPattern - Synchronization source Pattern value for the RTP session.
 *        ssrcMask    - Synchronization source Mask value for the RTP session.
 *        cname       - The unique name representing the source of the RTP data.
 * output: none.
 * return value: If no error occurs, the function returns the handle for the open
 *               RTP session. Otherwise, the function returns NULL.
 * Note:
 * 1. RtpOpenEx opens one socket for RTP session with the same port for receiving
 * and for sending, and one for RTCP session with the next port for receiving
 * and for sending (if cname is not NULL).
 * 2. If demultiplexing is supported first session will be opened with demux=NULL
 * for other demultiplexed sessions association of RTP session with shared socket
 * will be in function RvRtpDemuxOpenSession
 ***********************************************************************************/
RVAPI
RvRtpSession RVCALLCONV RtpOpenEx(
    IN  RvRtpDemux    demux,
    IN  RvNetAddress* pRtpAddress,
    IN  RvUint32      ssrcPattern,
    IN  RvUint32      ssrcMask,
    IN  char *        cname);

/************************************************************************************
 * RtpSetRemoveEventHandler
 * description: sets or removes RTP event handler.
 * input: sock         - pointer to socket
 *        selectEngine - pointer to pointer on selectEngine.
 *        selectFd     - pointer to file descriptor.
 *        hasFd        - true, if event handler already has been set.
 *        addFd        - true, if event handler have to be set .
 *        selectCB     - callback function for Read events
 * output: none.
 ***********************************************************************************/
void RtpSetRemoveEventHandler(
        IN RvSocket*              sock,
        IN RvSelectEngine**       selectEngine,
        IN RvSelectFd*            selectFd,
        IN RvBool                 hasFd,
        IN RvBool                 addFd,
        IN RvSelectCb             selectCB);

/************************************************************************************
 * RtpAddRemoteAddress
 * description: Adds the new RTP address of the remote peer or the address of a multicast
 *              group or of multiunicast address list to which the RTP stream will be sent.
 *              Adds multiplexID, if specified
 * input: hRTP  - Handle of the RTP session.
 *        pRtpAddress contains
 *            ip    - IP address to which RTP packets should be sent.
 *            port  - UDP port to which RTP packets should be sent.
 * output: none.
 * return value: none.
 ***********************************************************************************/
RVAPI
void RVCALLCONV RtpAddRemoteAddress(
	IN RvRtpSession  hRTP,   /* RTP Session Opaque Handle */
	IN RvNetAddress* pRtpAddress,
    IN RvUint32*     pMultiplexID);

/************************************************************************************
 * RtpRemoveRemoteAddress
 * description: removes the specified RTP address of the remote peer or the address 
 *              of a multicast group or of multiunicast address list to which the 
 *              RTP stream was sent.
 *              Removes RTP remote address with multiplexID, if pMultiplexID is specified
 * input: hRTP  - Handle of the RTP session.
 *        pRtpAddress contains
 *            ip    - IP address to which RTP packets should be sent.
 *            port  - UDP port to which RTP packets should be sent.
 *        pMultiplexID - pointer to multiplexID of the remote RTP
 * output: none.
 * return value:If an error occurs, the function returns a negative value.
 *              If no error occurs, the function returns a non-negative value.
 ***********************************************************************************/
RVAPI
RvStatus RVCALLCONV RtpRemoveRemoteAddress(
	IN RvRtpSession  hRTP,
	IN RvNetAddress* pRtpAddress,
    IN RvUint32*     pMultiplexID);


#ifdef __cplusplus
}
#endif

#endif  /* __RTPUTIL_H */
