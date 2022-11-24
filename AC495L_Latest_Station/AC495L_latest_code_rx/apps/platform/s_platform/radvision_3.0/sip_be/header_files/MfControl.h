/******************************************************************************
Filename:    MfControl.h
Description: Common module of Sample Gateway
*******************************************************************************
                Copyright (c) 2005 RADVISION Inc.
*******************************************************************************
NOTICE:
This document contains information that is proprietary to RADVISION LTD.
No part of this publication may be reproduced in any form whatsoever
without written prior approval by RADVISION LTD.

RADVISION LTD. reserves the right to revise this publication and make
changes without obligation to notify any person of such revisions or
changes.
******************************************************************************/
#ifndef RV_MF_CONTROL_H
#define RV_MF_CONTROL_H
/***************************************************************************************
* This module:
*	Defines MfControl C-interface that provides upper abstraction layer to access Media Framework:
 *
 **************************************************************************************/
/*=====================================================*/
#if defined(__cplusplus)
extern "C" {
#endif

#define RV_MFCTRL_VER         	1

#include "MFVideoDefs.h"
#include "ippcodec.h"

/******************************************************************************
* MfControl - abstract interface to create and to maintain data path layout.
*         Data path of any complexity is comprised of elements of 2 kinds: links and operators.
*         Links are linear part of path where audio/video/data buffers are propagated and processed
*         without splitting or mixing.
*         Operators are points of buffer mixing and/or splitting. An operator mixes incoming data
*         flowing into the operator via one or more links.
*         After data mixing or splitting an operator inserts mixed or split data into one or more outgoing links.
*         Any Link may start in one operator and finish in one operator.
*         The first operator this link is outgoing from. The last operator this link is incoming to.
*
*         What kind of buffer processing a link object proceeds depends on its type.
*         To create link object user calls RvMfCLinkNew() passing object type and additional parameters.
*         Possible link object types enumerated in RvMfCLinkType struct.
*         Link type and additional parameters for each type are gathered in appropriate RvMfCXXXParam structures.
*         To modify link object parameter user calls RvMfCLinkModify().
*
*         What kind of mixing/splitting activity an operator object proceeds depends on its type.
*         To create operator object user calls RvMfCOperatorNew() passing object type and additional parameters.
*         Possible link object types enumerated in RvMfCOperType struct.
*         Operator type and additional parameters for each type are gathered in RvMfCOperatorNewParam struct.
*
*         To build and maintain a complex layout composed of a few of link and operator objects user calls RvMfCOperatorCmd()
*         Possible command types are enumerated in RvMfCOpCmd struct. Appropriate command parameter are summarized in RvMfCOpCmdParam struct.
*
* Notes:  1. MfControl interface put no restriction on thread model that MfControl implementation operates in.
*         Link object may process and propagate data buffers synchronously or in context of 2 or more threads.
*         A few different Link/Operator objects may the same thread context etc.
*         2. MfControl interface may be extended to include more Link/Operator/Command types.
*
******************************************************************************/

/*==================================*/
/*====== Database interface  =======*/
/*==================================*/
/******************************************************************************
* MfCDb - is prefix of API that provides an access to internal database
*         that any MfControl implementation must maintain.
*         This database contains 2 types of records: for Link objects and for Operator objects.
*         Record adding/removing/modifying is accomplished by MfControl and is transparent for user.
*         Link object record is inserted to DB during RvMfCLinkNew() and is removed during RvMfCLinkdelete().
*         Each Link object record has unique key - char* linkName. MfCDbLinkFindByName().
*         MfCDbLinkFindByName()     - find Link object in DB
*         MfCDbGetOperElmIn()       - find operator object that passed link finishes in.
*         MfCDbGetOperElmOut()      - find operator object that passed link starts from.
*
*         Operator object record is inserted during RvMfCOperatorNew() and is removed during RvMfCOperatorDelete().
*         Each Operator object record has unique key - char* opName.
*         MfCDbOperFindByName()     - find operator object in DB
*
******************************************************************************/
RV_DECLARE_HANDLE(HLinkDB);
RV_DECLARE_HANDLE(HOperDB);

typedef enum
{
    _containerIn  =1,
    _containerOut
}ContainerDir;


/******************************************************************************
*  MfCDbLinkFindHndl
*  ----------------------------
*  General :       search Link DB and returns Link handle (if any)
*  Return Value:   RvMfCLinkHandle on success or NULL
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          char*               linkName
*
*  Output          None.
******************************************************************************/
RVAPI HLinkDB MfCDbLinkFindByName( IN const char* linkName);

/******************************************************************************
*  MfCDbOperFindByName
*  ----------------------------
*  General :       search operator DB and returns operator handle (if any)
*  Return Value:   HOperDB on success or NULL
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          const char*       opName
*
*  Output          None.
******************************************************************************/
RVAPI HOperDB MfCDbOperFindByName( IN const char* opName);

/******************************************************************************
*  MfCDbGetOperElmIn
*  ----------------------------
*  General :       Read from a link record. Return a handle of operator this link finish in.
*  Return Value:   HOperDB on success or NULL
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          HLinkDB       hLinkElm
*
*  Output          None.
******************************************************************************/
RVAPI HOperDB    MfCDbGetOperElmIn(  IN HLinkDB hLinkElm);

/******************************************************************************
*  MfCDbGetOperElmOut
*  ----------------------------
*  General :       Read from a link record. Return a handle of operator this link start from.
*  Return Value:   HOperDB on success or NULL
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          HLinkDB       hLinkElm
*
*  Output          None.
******************************************************************************/
RVAPI HOperDB    MfCDbGetOperElmOut( IN HLinkDB hLinkElm);

	typedef enum {
	tlinkNoCommand = 0,
	tlinkForceIntra = 1
} tLinkCommand;
/*=====================================================*/
/*=====================================================*/
typedef enum
{
    FRAME_SIZE_SQCIF    = MF_RESOLUTION_SQCIF, //sub-QCIF
    FRAME_SIZE_QCIF     = MF_RESOLUTION_QCIF ,
    FRAME_SIZE_CIF      = MF_RESOLUTION_CIF  ,
    FRAME_SIZE_4CIF     = MF_RESOLUTION_4CIF ,
    FRAME_SIZE_16CIF    = MF_RESOLUTION_16CIF,
    FRAME_SIZE_SPFMT    = MF_RESOLUTION_SPFMT,
    FRAME_SIZE_QVGA     = MF_RESOLUTION_QVGA ,
    FRAME_SIZE_MAX
}VideoFrameSize;

typedef enum
{
    FRAME_RATE_10   =10, //capture number per sec
    FRAME_RATE_15   =15, //capture number per sec
    FRAME_RATE_20   =20, //capture number per sec
    FRAME_RATE_30   =30, //capture number per sec
    FRAME_RATE_MAX
}VidFrameRate;

/*=====================================================*/
#define MFC_MAX_SDP_MSG     1024
typedef enum
{
	RvMfCAudioIoDeviceNone = 0,
	RvMfCAudioIoDeviceMicSpeak = 1,
	RvMfCAudioIoDeviceHeadSet,
	RvMfCAudioIoDeviceHandSet,
	RvMfCAudioIoDeviceMax
}RvMfCAudioIoDeviceType;
struct _RvMfControlConstructParam;

/* create and return handle of Internal implementation*/
typedef RvStatus    (*RvMfCCreateInternalCB)( IN struct _RvMfControlConstructParam* param);


/******************************************************************************
*  RvMfControlConstructParam
*  ----------------------------
*  General :       Parameter struct passed to RvMfControlInit()
*  ----------------------------------------------------------------------------
*  Note:           Many of the parameters have sense for concrete MfControl implementation only
*
******************************************************************************/
typedef	struct _RvMfControlConstructParam
{
    //log
        void*           logMgr;
        RvLogMessageType    mfcLogMask;
    //limits
        RvUint32    max_num_in_chains;// max number of incoming chains (including mic)
        RvUint32    max_num_out_chains;// max number of outgoing chains (including mic)

        RvUint32   max_num_nodes_of_chain;
        RvUint32   max_num_groups       ;//max number of groups in system. obsolete.
        RvUint32   max_num_messages     ;//max number of persisting Control Messages in system
        RvUint32   amax_num_messageSize ;//max Control Message size
        RvUint32   max_num_rtpSessions  ;//max number of CMFRtpSocket objects (open sockets are *2 more) in system
        RvChar     RTPAddress[64]       ;//IP address where RTP socket would be open on. if "0.0.0.0" Host address will be used
        RvUint16   portLocalBase;        //base port for CMFPortPool - mechanism of port pool maintenance
        RvUint16   max_num_portLocal    ;//port range for CMFPortPool  - mechanism of port pool maintenance

    //contexts
        RvUint32    recv_aud_cntx_poll_interval_msec;//
        RvUint32    recv_aud_cntx_priority;//
        RvUint32    recv_vid_cntx_poll_interval_msec;//
        RvUint32    recv_vid_cntx_priority;//
        RvUint32    recv_cntx_stack_size;//

        RvInt32    play_aud_cntx_poll_interval_msec;//
        RvInt32    play_vid_cntx_poll_interval_msec;//
        RvInt32    play_serv_priority;//
        RvInt32    play_cntx_stack_size;//

    //callback that must build MfControlInternal - derived object
    // if NULL builds MfControlInternal object itself
        RvMfCCreateInternalCB    createInternalCB;

    // sample frequency frame size
        RvUint32    nSamplesPerSecAudio;
        RvUint32    nSamplesPerFrameAudio;
        RvUint32    nSamplesPerSecVideo;
        RvUint32    nFrameRatePerSecVideo;// capture frame rate
    // video resolution on start
        VideoFrameSize    nFrameSize; // camera capturing resolution
        VideoFrameSize    nFrameSizeLocal; //LOCAL display resolution

    RvMfCAudioIoDeviceType    defDevice; // default device to be initialized on the start-up of the system: headset, handset ot mic&speak

}RvMfControlConstructParam;


/*
* predefined media configuration
*/
#define RvMfControlSetDefaultConfig(  /* RvMfControlConstructParam* */ mfcParam, /* RvLogMgr* */ logMgr) \
{\
    mfcParam->defDevice = RvMfCAudioIoDeviceMicSpeak;\
    mfcParam->logMgr = logMgr;\
    mfcParam->mfcLogMask          =  RV_LOGLEVEL_EXCEP   | \
                                     RV_LOGLEVEL_ERROR   | \
                                     RV_LOGLEVEL_WARNING;  \
    mfcParam->max_num_in_chains             = 10;\
    mfcParam->max_num_out_chains            = 10;\
    mfcParam->recv_aud_cntx_poll_interval_msec  = 20;\
    mfcParam->recv_aud_cntx_priority            = 0/*THREAD_PRIORITY_NORMAL*/;\
    mfcParam->recv_vid_cntx_poll_interval_msec  = 20;\
    mfcParam->recv_vid_cntx_priority            = 0;/*THREAD_PRIORITY_NORMAL, THREAD_PRIORITY_ABOVE_NORMAL*/\
    mfcParam->recv_cntx_stack_size          = 20000;\
    mfcParam->play_aud_cntx_poll_interval_msec  = 20;\
    mfcParam->play_vid_cntx_poll_interval_msec  = 30;\
    mfcParam->play_serv_priority            = 0/*THREAD_PRIORITY_NORMAL*/;\
    mfcParam->play_cntx_stack_size          = 20000;\
    mfcParam->max_num_nodes_of_chain        = mfcParam->max_num_in_chains + \
                                              mfcParam->max_num_out_chains +4; \
    mfcParam->max_num_groups                = 10;\
    mfcParam->max_num_messages              = 10;\
    mfcParam->amax_num_messageSize          = 200;\
    mfcParam->max_num_rtpSessions           = 20;\
    strcpy( mfcParam->RTPAddress, "0.0.0.0");\
    mfcParam->portLocalBase                 = 7000;\
    mfcParam->max_num_portLocal             = 8;\
    mfcParam->createInternalCB              = NULL;/*RvMfCDefaultCreateInternalCB;*/\
    mfcParam->nSamplesPerFrameAudio         = 160;\
    mfcParam->nSamplesPerSecAudio           = 8000;\
    mfcParam->nSamplesPerSecVideo           = 90000;\
    mfcParam->nFrameRatePerSecVideo         = 30;\
    mfcParam->nFrameSize                    = 2;/* QCIF =1; CIF=2*/\
    mfcParam->nFrameSizeLocal               = 1;/* QCIF =1; CIF=2*/\
}


/***************************************************************************************
 * RvMfControlConstruct
 * -------------------------------------------------------------------------------------
 * General:   initialize internal RvMfControl object
 *
 * Return Value: none
 * -------------------------------------------------------------------------------------
 * Arguments:
 * Input: 	 RvMfControlConstructParam*		- params of initialization
 * Output:   None
 **************************************************************************************/
RVAPI
RvStatus RvMfControlInit( IN RvMfControlConstructParam* param);

/***************************************************************************************
 * RvMfControlDestruct
 * -------------------------------------------------------------------------------------
 * General:   destruct internal RvMfControl object
 *
 * Return Value: none
 * -------------------------------------------------------------------------------------
 * Arguments:
 * Input: 	 RvMfControlHandle      handle of internal object
 * Output:   None
 **************************************************************************************/
RVAPI
void RvMfControlEnd();

/******************************************************************************
*  RvMfCLinkType
*  ----------------------------
*  General :    Enumerate possible types of Link object.
*
******************************************************************************/
typedef enum
{
    RVMFC_LINK_TYPE_MIN,
        /*audio physical terminations*/
    RVMFC_LINK_TYPE_MIC,    /*in channel*/
    RVMFC_LINK_TYPE_SPEAK,/*out channel*/
        /*video physical terminations*/
    RVMFC_LINK_TYPE_CAMERA, /*in channel*/
    RVMFC_LINK_TYPE_WINDOW,    /*out channel*/
        /*RTP terminations*/
    RVMFC_LINK_TYPE_RTP_IN, /*in channel*/
    RVMFC_LINK_TYPE_RTP_OUT /*out channel*/,

    RVMFC_LINK_TYPE_MAX
}RvMfCLinkType;

typedef struct  _RvMfCLinkParamBase
{
  RvMfCLinkType    type;
  const char*      szLinkName;
}RvMfCLinkParamBase;


/*
* although RvMfCLinkRtpInParam and RvMfCLinkRtpOutParam have the same fields they MUST be different types
* in order C++ compiler correctly links code
*/


/******************************************************************************
*  RvMfCLinkRtpInParam
*  ----------------------------
*  General :    Structure passed to RvMfCLinkNew() in order to build Link object of RVMFC_LINK_TYPE_RTP_IN type.
*               1.  This Link object doesn't start in any operator. It must initialize receiving socket
*                   and extract buffers on its own. It parses these buffers, decodes them, maintains jitter buffer,
*                   and propagates decoded audio buffers until finishing operator.
*               2.  This Link object finish in OPR_TYPE_MIX_AUDIO/OPR_TYPE_MIX_VIDEO operator. Buffers are
*                   extracted by the operator or are inserted to the operator by Link.
**
******************************************************************************/
typedef struct
{
    /*
    * the 1st fields of struct must be the same as of RvMfCLinkParamBase
    */
    RvMfCLinkType     type;
    const char*       szLinkName;
    RvUint32          sessId;
    /*net address attributes*/
    RvBool            bIsIPV6;
    RvUint16          portLocal;        /*may be 0 */
    char*             szIpLocal;
    /*media definition attributes*/
    char*             szDescr;/*descr - is local dsp wrapped by SDP msg. address should be updated. INOUT param*/
    /**/
    RvBool            bIsVideo;
    /*both audio & video attrbutes*/
    RvUint32          nSamplesPerSec;
    /*just audio attrbutes*/
    RvUint32          nSamplesPerFrame;
    /*just video attrbutes*/
    VideoFrameSize    frameSize;
    RvUint32          nFrameRate;       /* capture rate */
    /**/
}RvMfCLinkRtpInParam;

/******************************************************************************
*  RvMfCLinkRtpOutParam
*  ----------------------------
*  General :    Structure passed to RvMfCLinkNew() in order to build Link object of RVMFC_LINK_TYPE_RTP_OUT type.
*               1.  This Link object doesn't finish in any operator. It should initialize sending socket
*                   and propagate buffers from starting operator until this socket.
*               2.  This Link object starts in OPR_TYPE_MIX_AUDIO/OPR_TYPE_MIX_VIDEO operator. Buffers are
*                   inserted by the operator or are extracted from the operator by Link.
*                   Usually operator doesn't encode outgoing buffers. To encode these buffers before
*                   sending them via socket is responsibility of Link object.
*
******************************************************************************/
typedef struct
{
    /*
    * the 1st fields of struct must be the same as of RvMfCLinkParamBase
    */
    RvMfCLinkType     type;
    const char*       szLinkName;
    RvUint32          sessId;
    /*net address attributes*/
    RvBool            bIsIPV6;
    RvUint16          portLocal;        /*may be 0 */
    char*             szIpLocal;
    /*media definition attributes*/
    char*             szDescr;/*descr - is remote dsp wrapped by SDP msg.  IN param*/
    /**/
    RvBool            bIsVideo;
    /*both audio & video attrbutes*/
    RvUint32          nSamplesPerSec;
    /*just audio attrbutes*/
    RvUint32          nSamplesPerFrame;
    /*just video attrbutes*/
    VideoFrameSize    frameSize;
    RvUint32          nFrameRate;       /* capture rate */
    /**/
		tLinkCommand Command;
}RvMfCLinkRtpOutParam;


/******************************************************************************
*  RvMfCLinkMicParam
*  ----------------------------
*  General :    Structure passed to RvMfCLinkNew() in order to build Link object of RVMFC_LINK_TYPE_MIC type.
*               1.  This Link object doesn't start in any operator. It must initialize microphone facility
*                   and extract audio buffers on its own. It propagates raw (not-encoded) buffers until finishing operator.
*               2.  This Link object finish in OPR_TYPE_MIX_AUDIO operator. Not-encoded buffers buffers
*                   are extracted by the operator or are inserted to the operator by Link.
*  ----------------------------------------------------------------------------
*  Note:
*
******************************************************************************/
typedef struct
{
    /*
    * the 1st fields of struct must be the same as of RvMfCLinkParamBase
    */
    RvMfCLinkType   type;
    const char*       szLinkName;
    RvUint32       nSamplesPerSec;
    RvUint32       nSamplesPerFrame;
}RvMfCLinkMicParam;


/******************************************************************************
*  RvMfCLinkSpeakParam
*  ----------------------------
*  General :    Structure passed to RvMfCLinkNew() in order to build Link object of RVMFC_LINK_TYPE_SPEAKER type.
*               1.  This Link object doesn't finish in any operator. It must initialize window
*                   and insert audio buffers in this window.
*               2.  This Link object starts in OPR_TYPE_MIX_AUDIO operator. It propagates raw (not-encoded)
*                   buffers. These buffers are inserted by the operator or are extracted from the operator by Link.
*
******************************************************************************/
typedef struct
{
    /*
    * the 1st fields of struct must be the same as of RvMfCLinkParamBase
    */
    RvMfCLinkType  type;
    const char*       szLinkName;
    RvUint32       nSamplesPerSec;
    RvUint32       samplesPerFrame;
}RvMfCLinkSpeakParam;


/******************************************************************************
*  RvMfCLinkWindowParam
*  ----------------------------
*  General :    Structure passed to RvMfCLinkNew() in order to build Link object of RVMFC_LINK_TYPE_WINDOW type.
*               1.  This Link object doesn't finish in any operator. It must initialize window
*                   and insert video buffers in this window.
*               2.  This Link object starts in OPR_TYPE_MIX_VIDEO operator. Not-encoded buffers
*                   are inserted by the operator or are extracted from the operator by Link.
*
******************************************************************************/
typedef struct
{
    /*
    * the 1st fields of struct must be the same as of RvMfCLinkParamBase
    */
    RvMfCLinkType   type;
    const char*       szLinkName;
    VideoFrameSize  frameSize;
    VidFrameRate    frameRate;//capture rate
#ifdef WIN32
    HVIDEOOBJ            hVideoObj;
#endif
}RvMfCLinkWindowParam;


/******************************************************************************
*  RvMfCLinkCameraParam
*  ----------------------------
*  General :    Structure passed to RvMfCLinkNew() in order to build Link object of RVMFC_LINK_TYPE_CAMERA type.
*               1.  This Link object doesn't start in any operator. It must initialize camera facility
*                   and extract video buffers on its own.
*               2.  This Link object finish in OPR_TYPE_MIX_VIDEO operator. It propagates raw (not-encoded)
*                   buffers. These buffers are extracted by the operator or are inserted to the operator by Link.
*               3.  This Link object is responsible for local window, if the last is required.
*                   It should initialize such object and facilitate it.
*
******************************************************************************/
typedef struct
{
    /*
    * the 1st fields of struct must be the same as of RvMfCLinkParamBase
    */
    RvMfCLinkType   type;
    const char*       szLinkName;
    /// capture resolution
    VideoFrameSize  frameSize;
    /// local display resolution
    VideoFrameSize  frameSizeLocal;
    VidFrameRate  frameRate;//capture rate
#ifdef WIN32
    HVIDEOOBJ            hVideoObj;
#endif
    RvBool          bDisplayLocal;
    RvUint32        local_display_poll_interval_msec;
}RvMfCLinkCameraParam;

typedef union
{
    /*
     the 1st fields of all below structures must be the same as of RvMfCLinkParamBase */

    /* devices*/
    RvMfCLinkMicParam       mic;
    RvMfCLinkSpeakParam     speaker;
    RvMfCLinkWindowParam    wnd;
    RvMfCLinkCameraParam    camera;
    /* ephemeral*/
    RvMfCLinkRtpInParam     in;
    RvMfCLinkRtpOutParam    out;
}RvMfCLinkNewParam;

typedef union
{
    /*
    * the 1st fields of all below struct must be the same as of RvMfCLinkParamBase
    */

    /* devices*/
    RvMfCLinkMicParam       mic;
    RvMfCLinkSpeakParam     speaker;
    RvMfCLinkWindowParam    wnd;
    RvMfCLinkCameraParam    camera;
    /* ephemeral*/
    RvMfCLinkRtpInParam     in;
    RvMfCLinkRtpOutParam    out;
}RvMfCLinkModifyParam;

/***************************************************************************************
* RvMfCLinkNew
* -------------------------------------------------------------------------------------
* General:  Create Link object.
*           Possible link object types enumerated in RvMfCLinkType struct.
*           Link type and additional parameters for each type are defined in appropriate RvMfCXXXParam structures.
*           Role and tasks of each Link type are detailed in comments for these structures.
* Return Value: RV_OK if success
* -------------------------------------------------------------------------------------
* Arguments:
* Input:
*			RvMfCLinkParamBase*	- param: in reality one of RvMfCXXXParam is passed.
*
* Output:
*			HLinkDB	        - hLink: handle of created Link object
*
**************************************************************************************/
RVAPI RvStatus RvMfCLinkNew( OUT HLinkDB *hLink,INOUT RvMfCLinkParamBase* param);


/***************************************************************************************
* RvMfCLinkDelete
* -------------------------------------------------------------------------------------
* General: Delete Link object.
*
* Return Value: RV_OK if success
* -------------------------------------------------------------------------------------
* Arguments:
* Input:
*			HLinkDB	        - hLink: handle of Link object
* Output:
*			None
*
**************************************************************************************/
RVAPI RvStatus RvMfCLinkDelete( IN HLinkDB hLink);


/***************************************************************************************
* RvMfCLinkStart
* -------------------------------------------------------------------------------------
* General: Start buffer handling and propagating activity.
*
* Return Value: RV_OK if success
* -------------------------------------------------------------------------------------
* Arguments:
* Input:
*			HLinkDB	        - hLink: handle of Link object
* Output:
*			None
*
**************************************************************************************/
RVAPI RvStatus RvMfCLinkStart  ( IN HLinkDB hLink);


/***************************************************************************************
* RvMfCLinkStop
* -------------------------------------------------------------------------------------
* General:  Stop buffer handling and propagating activity.
*           Release all buffers hung inside Link object (say, in asynchronous nodes)
*
* Return Value: RV_OK if success
* -------------------------------------------------------------------------------------
* Arguments:
* Input:
*			HLinkDB	        - hLink: handle of Link object
* Output:
*			None
*
**************************************************************************************/
RVAPI RvStatus RvMfCLinkStop ( IN HLinkDB hLink);

/***************************************************************************************
* RvMfCLinkModify
* -------------------------------------------------------------------------------------
* General: Modify media link object.
*          Compare current object parameters with requested ones.
*			If parameters may be changed on-fly accomplishes this job.
*			Else return error.
*			Example of possible change is request to silence support/unsupport for some audio codecs
*			Example of impossible change is request to change IP address:
*			                             RtpReceiver node (built inside LinkRtpIn object) cannot do it.
*
* Return Value: RV_OK if success
* -------------------------------------------------------------------------------------
* Arguments:
* Input:
*			HLinkDB	        - hLink: handle of Link object
*			RvMfCLinkModifyParam*	- param: parameters of media
* Output:
*			None
*
**************************************************************************************/
RVAPI RvStatus RvMfCLinkModify ( IN HLinkDB hLink, IN RvMfCLinkModifyParam* param);



/***************************************************************************************
* RvMfCLinkIsStarted
* -------------------------------------------------------------------------------------
* General:  Check if Link object in Start or Stop mode.
*
* Return Value: RV_OK if success
* -------------------------------------------------------------------------------------
* Arguments:
* Input:
*			HLinkDB	        - hLink: handle of Link object
* Output:
*			None
*
**************************************************************************************/
RVAPI RvBool RvMfCLinkIsStarted ( IN HLinkDB hLink);

/***************************************************************************************
 *
 * Operator object and operations on it
 * Operator types
 * Operations on Operator object
 *
 **************************************************************************************/

/******************************************************************************
*  RvMfCLinkType
*  ----------------------------
*  General :    Enumerate possible types of Operator object.
*
******************************************************************************/
typedef enum
{
    OPR_TYPE_MIN,
    OPR_TYPE_MIX_AUDIO, // extract and mix all incoming link objects.
                        // in case 1 incoming and 1 outgoing links connect them peer-to-peer
    OPR_TYPE_MIX_VIDEO, // extract all incoming link objects.
                        // in case 1 incoming and 1 outgoing links connect them peer-to-peer
    OPR_TYPE_SPLITTER,  // not yet in use by MTF
    OPR_TYPE_MAX
}RvMfCOprType;


typedef struct
{
  RvMfCOprType    type;
}RvMfCOprBaseParam;

/******************************************************************************
*  RvMfCOprMixAudioParam
*  ----------------------------
*  General :    Structure passed to RvMfCOperatorNew() in order to build an operator object of OPR_TYPE_MIX_AUDIO type.
*               1.  This operator mix (raw) audio buffers of all incoming Link objects.
*               2.  in case there are only 1 incoming and 1 outgoing Link objects this operator must
*                   connect them peer-to-peer. Means to propagate buffers extracted from incoming Link
*                   to outgoing Link.
*
******************************************************************************/
typedef struct
{
  RvMfCOprType    type;
  /*....................*/
}RvMfCOprMixAudioParam;


/******************************************************************************
*  RvMfCOprMixVideoParam
*  ----------------------------
*  General :    Structure passed to RvMfCOperatorNew() in order to build an operator object of OPR_TYPE_MIX_VIDEO type.
*               1.  This operator make composite picture(s) from video buffers of incoming Link objects.
*                   Such composite picture(s) is inserted into outgoing Link objects. There are 2 possibility:
*                   a. The operator builds different picture for each outgoing Link.
*                      The Link (of RVMFC_LINK_TYPE_RTP_OUT type) encodes the picture and sends via socket.
*                   b. The operator builds the same picture for all outgoing Links.
*                      Then it would be waste task to encode the picture in each Link. The operator should
*                      encode the picture once and insert the same encoded buffers into all outgoing Links.
*               2.  in case there are only 1 incoming and 1 outgoing Link objects this operator must
*                   connect them peer-to-peer. Means to propagate buffers extracted from incoming Link
*                   to outgoing Link.
*
******************************************************************************/
typedef struct
{
  RvMfCOprType    type;
  VideoFrameSize  frameSize;
  /*....................*/
}RvMfCOprMixVideoParam;


/******************************************************************************
*  RvMfCOprSplitterParam
*  ----------------------------
*  General :    Not used by MTF
******************************************************************************/
typedef struct
{
  RvMfCOprType    type;
  /*....................*/
}RvMfCOprSplitterParam;


typedef union
{
    /*
    * the 1st fields of all below struct must be the same as of RvMfCLinkParamBase
    */

    /* devices*/
    RvMfCOprMixAudioParam     mixAudio;
    RvMfCOprMixVideoParam     mixVideo;
    RvMfCOprSplitterParam     splitter;
}RvMfCOperatorNewParam;

/***************************************************************************************
* RvMfCOperatorNew
* -------------------------------------------------------------------------------------
* General:  Create Operator object.
*           Possible link object types enumerated in RvMfCOprType.
*           Operator type and additional parameters for each type are defined in appropriate RvMfCOprXXXParam structures.
*           Role and tasks of each Operator type are detailed in comments for these structures.
* Return Value: RV_OK if success
* -------------------------------------------------------------------------------------
* Arguments:
* Input:
*			RvMfCLinkParamBase*	- param: in reality one of RvMfCXXXParam is passed.
*
* Output:
*			HLinkDB	        - hLink: handle of created Link object
*
**************************************************************************************/
RVAPI RvStatus RvMfCOperatorNew(  OUT HOperDB *hOperator, IN const char* opName, IN RvMfCOprBaseParam* param);


/***************************************************************************************
* RvMfCOperatorDelete
* -------------------------------------------------------------------------------------
* General: Delete Operator object.
*
* Return Value: RV_OK if success
* -------------------------------------------------------------------------------------
* Arguments:
* Input:
*			HLinkDB	        - hLink: handle of Link object
* Output:
*			None
*
**************************************************************************************/
RVAPI void RvMfCOperatorDelete( IN HOperDB hOperator);

/**************************************************************************************
 * Commands defined over Operator object
**************************************************************************************/
///
typedef enum
{
    OPCMD_MIX_AUDIO_ADD_IN,         // add incoming link
    OPCMD_MIX_AUDIO_ADD_OUT,        // add outgoing link to mix processing
    OPCMD_MIX_VIDEO_ADD_IN,         // add incoming link
    OPCMD_MIX_VIDEO_ADD_OUT,        // add outgoing link to mix processing
    OPCMD_SPLIT_ADD_IN,             // add incoming link to splitter operator; may be called only once
    OPCMD_SPLIT_ADD_OUT,            // add outgoing link to splitter operator
    OPCMD_REMOVE_IN,                // remove incoming link from any operator
    OPCMD_REMOVE_OUT,               // remove outgoing link from any operator
    MOVE_IN,                        // move incoming link from operator to operator
    MOVE_OUT,                       // move outgoing link from operator to operator
    OPCMD_MAX
}RvMfCOpCmd;



/******************************************************************************
*  RvMfCOpCmd_mix_audio_add_in
*  ----------------------------
*  General :    Structure passed to RvMfCOperatorCmd() during request to add Link object finishing in this operator.
******************************************************************************/
typedef	struct
{
    HOperDB  hOperator;
	HLinkDB hLinkIn;
}RvMfCOpCmd_mix_audio_add_in;

/******************************************************************************
*  RvMfCOpCmd_mix_audio_add_out
*  ----------------------------
*  General :    Structure passed to RvMfCOperatorCmd() during request to add Link object starting in this operator.
******************************************************************************/
typedef	struct
{
    HOperDB  hOperator;
	HLinkDB hLinkOut;
}RvMfCOpCmd_mix_audio_add_out;

/******************************************************************************
*  RvMfCOpCmd_mix_video_add_in
*  ----------------------------
*  General :    Structure passed to RvMfCOperatorCmd() during request to add Link object finishing in this operator.
******************************************************************************/
typedef	struct
{
    HOperDB  hOperator;
	HLinkDB hLinkIn;
}RvMfCOpCmd_mix_video_add_in;

/******************************************************************************
*  RvMfCOpCmd_mix_video_add_out
*  ----------------------------
*  General :    Structure passed to RvMfCOperatorCmd() during request to add Link object starting in this operator.
******************************************************************************/
typedef	struct
{
    HOperDB  hOperator;
	HLinkDB hLinkOut;
}RvMfCOpCmd_mix_video_add_out;


/******************************************************************************
*  RvMfCOpCmd_remove_in
*  ----------------------------
*  General :    Structure passed to RvMfCOperatorCmd() during request to remove Link object finishing in this operator.
******************************************************************************/
typedef	struct
{
    HOperDB  hOperator;
	HLinkDB hLinkIn;
}RvMfCOpCmd_remove_in;

/******************************************************************************
*  RvMfCOpCmd_remove_out
*  ----------------------------
*  General :    Structure passed to RvMfCOperatorCmd() during request to remove Link object starting in this operator.
******************************************************************************/
typedef	struct
{
    HOperDB  hOperator;
	HLinkDB hLinkOut;
}RvMfCOpCmd_remove_out;

/* not used by MTF
typedef	struct
{
    HOperDB  hOperator;
	HLinkDB hLinkIn;
}RvMfCOpCmd_split_add_in;

typedef	struct
{
    HOperDB  hOperator;
	HLinkDB hLinkOut;
}RvMfCOpCmd_split_add_out;
*/

typedef union
{
	RvMfCOpCmd_mix_audio_add_in mix_audio_add_in ;
	RvMfCOpCmd_mix_audio_add_out mix_audio_add_out ;

	RvMfCOpCmd_mix_video_add_in mix_video_add_in ;
	RvMfCOpCmd_mix_video_add_out mix_video_add_out ;

    RvMfCOpCmd_remove_in            remove_in      ;
	RvMfCOpCmd_remove_out           remove_out     ;


/* not used by MTF
	RvMfCOpCmd_split_add_in         split_add_in         ;
	RvMfCOpCmd_split_add_out        split_add_out        ;
    RvMfCOpCmd_move_in              move_in              ;
	RvMfCOpCmd_move_out             move_out             ;
*/
}RvMfCOpCmdParam;


/***************************************************************************************
* RvMfCOperatorCmd
* -------------------------------------------------------------------------------------
* General:  Send command to Operator object.
*           Possible command types enumerated in RvMfCOpCmd.
*           Command type and additional parameters for each type are defined in  RvMfCOpCmdParam structure.
*           Role and tasks of each command type are detailed in comments for these structures.
* Return Value: RV_OK if success
* -------------------------------------------------------------------------------------
* Arguments:
* Input:
*			RvMfCLinkParamBase*	- param: in reality one of RvMfCXXXParam is passed.
*
* Output:
*			HLinkDB	        - hLink: handle of created Link object
*
**************************************************************************************/
RVAPI RvStatus RvMfCOperatorCmd( IN RvMfCOpCmd cmd, IN RvMfCOpCmdParam* param);
/*====================================================================*/


/*
* MfC Log support
*/
RvLogMgr*        MfCLogMgr();
RvLogSource*     MfCLogSrc();
#undef MF_SOURCE
#define MF_SOURCE       MfCLogSrc()

/***************************************************************
*				M A C R O
***************************************************************/
#define	CHECK_PTR( p, return_code) if(p==NULL){rc = return_code; goto err_exit;}
#define	CHECK_FUNC( f) rc = f; if(rc != 0) goto err_exit;

/*=====================================================*/
#if defined(__cplusplus)
}
#endif
/*=====================================================*/

#endif /*RV_MF_CONTROL_H*/





















