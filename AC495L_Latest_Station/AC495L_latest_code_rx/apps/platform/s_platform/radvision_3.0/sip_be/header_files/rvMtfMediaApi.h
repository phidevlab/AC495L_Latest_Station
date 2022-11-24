/******************************************************************************
Filename:    rvMtfMediaApi.h
Description: This file includes type definitions and APIs for processing media
			 callbacks and manipulating media in application.
*******************************************************************************
                Copyright (c) 2008 RADVISION
*******************************************************************************
NOTICE:
This document contains information that is proprietary to RADVISION.
No part of this publication may be reproduced in any form whatsoever
without written prior approval by RADVISION.

RADVISION reserves the right to revise this publication and make changes
without obligation to notify any person of such revisions or changes.
******************************************************************************/
#ifndef RV_RVMGRMGRMEDIA_H
#define RV_RVMGRMGRMEDIA_H

#include "rvMtfHandles.h"
#include "rvCallControlApi.h"

/*@*****************************************************************************
 * Package: RvMtfMediaPkg (root)
 * -----------------------------------------------------------------------------
 * Title: Media integration
 *
 * General: This section contains functions and type definitions that are used
 *  		for integrating the media engine of the application with the MTF.
 ****************************************************************************@*/

/*@*****************************************************************************
* Enum: RvMtfMediaAction (RvMtfMediaPkg)
* -----------------------------------------------------------------------------
* Description: This enumerator indicates what type of action should be
* performed on a media stream that was created or modified by the application.
*****************************************************************************@*/
typedef enum
{
    RVMTF_MEDIA_NEW,
		/* Indicates a newly-created media stream. Relevant for the callback
		   RvMtfMediaCreateStreamEv() only. */
    RVMTF_MEDIA_MODIFY_SESSION,
		/* Indicates that the parameters of an existing media stream should be
		   changed (codec, codec attribute, media type. etc.). Relevant for the
		   callback RvMtfMediaModifyStreamEv() only. */
    RVMTF_MEDIA_MUTE,
        /* Indicates that the media stream should be muted. Relevant for the
		   callback RvMtfMediaModifyStreamEv() only. */
    RVMTF_MEDIA_UNMUTE,
		/* Indicates that the media stream should be unmuted. Relevant for the
		   callback RvMtfMediaModifyStreamEv() only. */
    RVMTF_MEDIA_HOLD_LOCAL,
        /* Indicates that the media stream should be put on Hold by the local
		   party. Relevant for the callback RvMtfMediaModifyStreamEv() only. */
    RVMTF_MEDIA_HOLD_REMOTE,
        /* Indicates that the media stream should be put on Hold by the remote
		   party. Relevant for the callback RvMtfMediaModifyStreamEv() only. */
	RVMTF_MEDIA_HOLD_LOCAL_AND_REMOTE,
		/* Indicates that the media stream should be put on Hold by both the
		   local and the remote party. Relevant for the callback
		   RvMtfMediaModifyStreamEv() only.*/
    RVMTF_MEDIA_UNHOLD_LOCAL,
		/* Indicates that the media stream should be unheld by the local party.
		   Relevant for the callback RvMtfMediaModifyStreamEv() only.*/
    RVMTF_MEDIA_UNHOLD_REMOTE,
		/* Indicates that the media stream should be unheld by the remote party.
		   Relevant for the callback RvMtfMediaModifyStreamEv() only.*/
    RVMTF_MEDIA_UNKNOWN
		/* Indicates that the action is unknown. */
}RvMtfMediaAction;

/*@*****************************************************************************
* Type: RvMtfMediaParams (RvMtfMediaPkg)
* -----------------------------------------------------------------------------
* Description: This structure contains information about the media stream on
* which the application is required to perform an action. This structure is
* used when the media callbacks RvMtfMediaCreateStreamEv() and
* RvMtfMediaModifyStreamEv() are called.
*****************************************************************************@*/
typedef struct
{
    RvMtfMediaAction		action;
		/* Indicates the action that the application should perform on the media
	       stream. When the callback RvMtfMediaCreateStreamEv() is called, this
	       parameter will always be set to RVMTF_MEDIA_NEW. */
    RvSdpMsg*				localSdp;
		/* Pointer to the SDP message that contains the media parameters of the
	       local party. */
    RvSdpMsg*				remoteSdp;
		/* Pointer to the SDP message that contains the media parameters of the
		   remote party, as received in the incoming SIP message. */
	RvSdpMsg*				localCapsSdp;
		/* Pointer to the SDP message that contains the media capabilities of
		   the local party, which were loaded to the MTF during initialization
	       by calling rvMtfLoadMediaCapabilities(). */

} RvMtfMediaParams;

/*@*****************************************************************************
* Enum: RvMtfDynamicModifyMediaStatus (RvMtfMediaPkg)
* -----------------------------------------------------------------------------
* Description: This enumerator indicates the result of a modifying media process
* during a call.
*****************************************************************************@*/
typedef enum
{
    RV_MTF_MODIFYMEDIA_STATUS_UNKNOWN,
		/* Modify media failed, reason unknown. */
	RV_MTF_MODIFYMEDIA_STATUS_SUCCESS,
		/* Modify media succeeded. */
	RV_MTF_MODIFYMEDIA_STATUS_IN_PROCESS,
		/* Modify media failed because a previous modify media process was not
		   yet completed. */
	RV_MTF_MODIFYMEDIA_STATUS_REMOTE_REJECTED,
		/* Modify media failed because the remote party rejected the request
		   (in case of an outgoing request). */
	RV_MTF_MODIFYMEDIA_STATUS_LOCAL_FAILED
		/* Modify media failed due to a local failure. */
} RvMtfDynamicModifyMediaStatus;


/*@*****************************************************************************
 * RvMtfMediaStartPhysicalDeviceEv (RvMtfMediaPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         This callback is called when the application is required to
 *         initialize an audio device (handset, speaker or headset). This callback
 *         is called once, on the first call only.
 *
 * Arguments:
 *
 * Input:	hTerm		- Handle to the terminal.
 *			hAppTerm	- Handle to the application data associated with the
 *                        terminal.
 *			termType	- Type of the terminal, indicating the type of
 *						  application to be initialized: audio or video.
 *
 * Return Value: RV_OK if successful, other if not.
 ****************************************************************************@*/
typedef RvStatus (RVCALLCONV *RvMtfMediaStartPhysicalDeviceEv)(
                    IN RvIppTerminalHandle				hTerm,
					IN RvMtfTerminalAppHandle			hAppTerm,
					IN RvMtfTerminalType				termType);

/*@*****************************************************************************
 * RvMtfMediaStopPhysicalDeviceEv (RvMtfMediaPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         This callback is called when the application is required to stop an
 *         audio device (handset, speaker or headset). Currently, this callback
 *         is not called at all. The application should stop all audio
 *         devices after the MTF is shut down.
 *
 * Arguments:
 *
 * Input:	hTerm		- Handle to the terminal.
 *			hAppTerm	- Handle to the application data associated with the
 *                        terminal.
 *			termType	- Type of the terminal, indicating the type of
 *                        application to be stopped: audio or video.
 *
 * Return Value: RV_OK if successful, other if not.
 ****************************************************************************@*/
typedef RvStatus (RVCALLCONV *RvMtfMediaStopPhysicalDeviceEv)(
                    IN RvIppTerminalHandle				hTerm,
					IN RvMtfTerminalAppHandle			hAppTerm,
					IN RvMtfTerminalType				termType);

/*@*****************************************************************************
 * RvMtfMediaCreateStreamEv (RvMtfMediaPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         This callback is called when the application is required to create a
 *         new media stream (RTP session) with the specific parameters provided
 *         in this callback (codec, etc.). The application should return the IP
 *         address and port number of the actual new media stream (set in
 *         params->localSdp). This callback is usually called when a new call
 *         is established.
 *
 * Arguments:
 *
 * Input:	hTerm		- Handle to the terminal.
 *			hAppTerm	- Handle to the application data associated with the
 *                        terminal.
 *			params		- Media parameters with which the new media stream
 *                        should be created.
 *
 * Output: params		- Media parameters with which the new media stream was
 *                        created. The localSdp should include the IP address
 *                        and port number of the new media stream. All other
 *						  fields should remain the same.
 *
 * Return Value: RV_OK if successful, other if not.
 ****************************************************************************@*/
typedef RvStatus (RVCALLCONV *RvMtfMediaCreateStreamEv)(
                    IN		RvIppTerminalHandle				hTerm,
					IN		RvMtfTerminalAppHandle			hAppTerm,
                    INOUT	RvMtfMediaParams*				params);

/*@*****************************************************************************
 * RvMtfMediaModifyStreamEv (RvMtfMediaPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         This callback is called when the application is required to modify
 *         an existing media stream (RTP session) with the specific parameters
 *         provided in this callback (codec, etc.).The application should return
 *         the IP address and port number of the actual media stream (set in
 *         params->localSdp). This callback is usually called during a
 *         connected call.
 *
 * Arguments:
 *
 * Input:	hTerm		- Handle to the terminal.
 *			hAppTerm	- Handle to the application data associated with the
 *                        terminal.
 *			params		- Media parameters with which the media stream should
 *                        be modified.
 *
 * Output: params		- Media parameters with which the media stream was
 *                        modified. The localSdp should include the IP address
 *                        and port number of the media stream. All other fields
 *                        should remain the same.
 *
 * Return Value: RV_OK if successful, other if not.
 ****************************************************************************@*/
typedef RvStatus (RVCALLCONV *RvMtfMediaModifyStreamEv)(
                    IN		RvIppTerminalHandle				hTerm,
					IN		RvMtfTerminalAppHandle			hAppTerm,
                    INOUT	RvMtfMediaParams*				params);

/*@*****************************************************************************
 * RvMtfMediaDestroyStreamEv (RvMtfMediaPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         This callback is called when the application is required to
 *         terminate an existing media stream (RTP session). This callback is
 *         usually called when the call disconnects.
 *
 * Arguments:
 *
 * Input:	hTerm		- Handle to the terminal.
 *			hAppTerm	- Handle to the application data associated with the
 *                        terminal.
 *
 * Return Value: RV_OK if successful, other if not.
 ****************************************************************************@*/
typedef RvStatus (RVCALLCONV *RvMtfMediaDestroyStreamEv)(
                    IN RvIppTerminalHandle				hTerm,
					IN RvMtfTerminalAppHandle			hAppTerm);

/*@*****************************************************************************
 * RvMtfMediaConnectStreamsEv (RvMtfMediaPkg)
 * -----------------------------------------------------------------------------
 * General:
 *			This callback is called when the application is required to connect
 *			one of the following:
 *				1.	An existing media stream (RTP session) with an audio/video
 *					device, to enable media to flow from both parties of the
 *                  call, or
 *				2.	An existing media stream with another existing media stream,
 *					to create mixing of media and enable the establishment of a
 *                  Conference call.
 *			This callback is usually called when a call is being connected
 *			(a remote party or a local user answered the call), or during a
 *          Conference call.
 *
 * Arguments:
 *
 * Input:	hTermSource		- Handle to the source terminal. This may be an RTP
 *                            terminal or an audio/video device.
 *			hAppTermSource	- Handle to the application data associated with
 *                            the source terminal.
 *			hTermTarget		- Handle to the target terminal. This is always an
 *                            RTP terminal.
 *			hAppTermTarget	- Handle to the application data associated with
 *                            the target terminal.
 *			termSourceType	- Type of the source terminal, indicating whether
 *                            the application should connect the RTP session to
 *							  an audio/video device or to another RTP session.
 *
 * Return Value: RV_OK if successful, other if not.
 ****************************************************************************@*/
typedef RvStatus (RVCALLCONV *RvMtfMediaConnectStreamsEv)(
                    IN RvIppTerminalHandle				hTermSource,
					IN RvMtfTerminalAppHandle			hAppTermSource,
                    IN RvIppTerminalHandle				hTermTarget,
					IN RvMtfTerminalAppHandle			hAppTermTarget,
					IN RvMtfTerminalType				termSourceType);

/*@*****************************************************************************
 * RvMtfMediaDisconnectEv (RvMtfMediaPkg)
 * -----------------------------------------------------------------------------
 * General:
 *			This callback is called when the application is required to
 *			disconnect one of the following:
 *				1.	An existing media stream (RTP session) from an audio/video
 *					device, to stop media flowing from both parties of the call,
 *                  or
 *				2.	An existing media stream from another existing media stream,
 *					to stop the mixing of media during a Conference call.
 *			This callback is usually called when a basic call is disconnected
 *			(the remote party or the local user answered the call), or when one
 *          of the participants left the Conference call.
 *
 * Arguments:
 *
 * Input:	hTermSource		- Handle to the source terminal. This may be an RTP
 *                            terminal or an audio/video device.
 *			hAppTermSource	- Handle to the application data associated with
 *                            the source terminal.
 *			hTermTarget		- Handle to the target terminal. This is always an
 *                            RTP terminal.
 *			hAppTermTarget	- Handle to the application data associated with
 *                            the target terminal.
 *			termSourceType	- Type of the source terminal, indicating whether
 *                            the application should disconnect the RTP session
 *							  from the audio/video device or from another RTP
 *                            session.
 *
 * Return Value: RV_OK if successful, other if not.
 ****************************************************************************@*/
typedef RvStatus (RVCALLCONV *RvMtfMediaDisconnectEv)(
                    IN RvIppTerminalHandle				hTermSource,
					IN RvMtfTerminalAppHandle			hAppTermSource,
                    IN RvIppTerminalHandle				hTermTarget,
					IN RvMtfTerminalAppHandle			hAppTermTarget,
					IN RvMtfTerminalType				termSourceType);

/*@*****************************************************************************
 * RvMtfMediaModifyCompletedEv (RvMtfMediaPkg)
 * -----------------------------------------------------------------------------
 * General:
 *			This callback is called when a process of modifying media during a
 *			connected call is completed (dynamic media change). The process may
 *          be initiated by the local party or the remote party.
 *			The callback indicates the result of the process, whether it
 *			succeeded or failed, and the reason for the failure.
 *			During the process of dynamic media change, the application should
 *			keep both the old media stream and the new media stream open (i.e.,
 *          listen on both sockets).
 *			If the process was completed successfully, the application should
 *          close the old media stream and leave the new media stream open.
 *			If the process failed, the application should close the new media
 *			stream and leave the old media stream open.
 *
 * Arguments:
 *
 * Input:	hTerm			- Handle to the terminal on which the media was
 *                            modified.
 *			hAppTermSource	- Handle to the application data associated with
 *                            the terminal.
 *			status			- Status of the process.
 *			sdpMsg			- SDP message containing the new media parameters.
 *
 * Return Value: RV_OK if successful, other if not.
 ****************************************************************************@*/
typedef void (RVCALLCONV *RvMtfMediaModifyCompletedEv)(
                    IN RvIppTerminalHandle				hTerm,
					IN RvMtfTerminalAppHandle			hAppTerm,
                    IN RvMtfDynamicModifyMediaStatus	status,
                    IN RvSdpMsg*						sdpMsg);

/*@*****************************************************************************
* Type: RvMtfMediaClbks (RvMtfMediaPkg)
* -----------------------------------------------------------------------------
* Description: This structure includes pointers to media callbacks that should
* be implemented by the application for implementing media (audio/video)
* during calls.
*****************************************************************************@*/
typedef struct
{
    RvMtfMediaStartPhysicalDeviceEv					startPhysicalDeviceCB;
    RvMtfMediaCreateStreamEv						createMediaStreamCB;
    RvMtfMediaModifyStreamEv						modifyMediaStreamCB;
    RvMtfMediaStopPhysicalDeviceEv					stopPhysicalDeviceCB;
    RvMtfMediaDestroyStreamEv						destroyMediaStreamCB;
    RvMtfMediaConnectStreamsEv						connectMediaCB;
    RvMtfMediaDisconnectEv							disconnectMediaCB;
    RvMtfMediaModifyCompletedEv						modifyMediaCompletedCB;

} RvMtfMediaClbks;

/*@*****************************************************************************
* Enum: RvMtfModifyMessage (RvMtfMediaPkg)
* -----------------------------------------------------------------------------
* Description: This enumerator indicates which signaling message should be sent
* to notify the remote party about a change of media during a connected call.
*****************************************************************************@*/
typedef enum
{
    RV_MTF_MODIFYMEDIA_BY_REINVITE,
		/* Indicates that media will be changed by sending a Re-Invite message
		   to the remote party. */
	RV_MTF_MODIFYMEDIA_BY_UPDATE
		/* Indicates that media will be changed by sending an Update message to
		   the remote party. */
} RvMtfModifyMessage;


/*@*****************************************************************************
 * rvMtfMediaStartModify (RvMtfMediaPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         Sends a Re-Invite with SDP to the remote party. This function should
 *         be called during a connected call only. After this function is called,
 *         the callback RvMtfModifyMediaCompletedCB() will be called to indicate
 *         the result of the process (for example: a reply from the remote
 *         party, local failure, etc.).
 *
 * Arguments:
 *
 * Input:	hTerm			- Handle to the terminal.
 *			sdpMsg			- Pointer to the SDP message to be sent with
 *                            the Re-Invite.
 *							  The MTF sets the session level parameters:
 *                            "o=", "v=", "t=", etc.
 *							  The application should set the media level
 *                            parameters: "m=", "a=", etc.
 *			modifyMessage	- Indicates which message will be sent out (Update
 *                            or Re-Invite).
 *
 * Return Value: RV_OK upon success.
 ****************************************************************************@*/
RVAPI RvStatus RVCALLCONV rvMtfMediaStartModify(
				IN RvIppTerminalHandle			hTerm,
				IN RvSdpMsg*					sdpMsg,
				IN RvMtfModifyMessage			modifyMessage);



#endif /*RV_RVMGRMGRMEDIA_H*/

