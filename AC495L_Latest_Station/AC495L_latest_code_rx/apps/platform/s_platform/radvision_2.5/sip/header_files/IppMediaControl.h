/******************************************************************************
Filename:    IppMediaControl.h
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
#ifndef RV_IPP_I_MEDIA_CONTROL_H
#define RV_IPP_I_MEDIA_CONTROL_H
/***************************************************************************************
* This module:
*	Defines interface between 2 levels of application code: 
 *	the 1st receives device and rtp stream -related commands from TK,
 *	the 2nd controls device and rtp sessions.
 * 
 **************************************************************************************/

/*=====================================================*/
#if defined(__cplusplus)
extern "C" {
#endif
/*=====================================================*/

/***************************************************************************************/
/**************     RTP SESSION CONTROL       ******************************************/
/***************************************************************************************/
	
/***************************************************************************************
 * RvIppMediaControlSetRtpIncoming
 * -------------------------------------------------------------------------------------
 * General:   placeholder for application CB
 *				
 * Return Value: RV_OK if success
 * -------------------------------------------------------------------------------------
 * Arguments:
 * Input: 	 const char* rtpId  /usually "audio" or "video"/
 *			 RvSdpMediaDescr*		- Local media parameters with not-initialized RTP/RTCP addresses.  
 *										Contains exactly 1 payload. Should be analyzed by sdp and codec related API. 
 * Output:   
 *			 application should initialize RTP/RTCP addresses of descrLocal if they are not exist 
 *			 i.e. a creation of new incoming RTP session is requested. 
 *														
**************************************************************************************/
typedef RvStatus (*RvIppMediaControlSetRtpIncoming)( IN const char* rtpId, INOUT RvSdpMediaDescr* descrLocal);


/***************************************************************************************
 * RvIppMediaControlSetRtpOutgoing
 * -------------------------------------------------------------------------------------
 * General:   placeholder for application CB
 *				
 * Return Value: RV_OK if success
 * -------------------------------------------------------------------------------------
 * Arguments:
 * Input: 	 const char* rtpId of rtp session that should be modified
 *			 RvSdpMediaDescr*		- Remote media parameters. 
 *										Contains exactly 1 payload. Should be analyzed by sdp and codec related API. 
 * Output:   None
 *														
 * Note:	descrRemote = NULL  means to stop sending data from given rtp session 
 *			
 *														
**************************************************************************************/
typedef RvStatus (*RvIppMediaControlSetRtpOutgoing)(	IN const char* rtpId, IN RvSdpMediaDescr* descrRemote);

/***************************************************************************************
 * RvIppMediaControlDestroyRtp
 * -------------------------------------------------------------------------------------
 * General:   placeholder for application CB
 *				
 * Return Value: RV_OK if success
 * -------------------------------------------------------------------------------------
 * Arguments:
 * Input: 	 const char* rtpId of rtp session should be destroyed
 *			 RvSdpMediaDescr*		- media parameters. Contains exactly 1 payload. 
 *														Should be analyzed by sdp and codec related API 
 * Output:   None
 **************************************************************************************/
typedef RvStatus (*RvIppMediaControlDestroyRtp)( IN const char* rtpId); /*usually "audio" or "video"*/


/***************************************************************************************
 * RvIppMediaControlDynamicModifyRtp
 * -------------------------------------------------------------------------------------
 * General:   This function is called directly by application RTP level to inform low level
 *				about media changes (re-INVITE)  	
 *				
 * Return Value: RV_OK if success
 * -------------------------------------------------------------------------------------
 * Arguments: /this is temporary interface for QA 9.2005-11.2005. TBD along with GUI development  
 * Input: 	 const char*	- rtpId of any of rtp sessions that should be modified.
 *			 RvSdpMsg*		- sdpMsg containing all local media with initialized RTP/RTCP addresses:  
 * Output:   None
 *														
 * Note:	if application need to change more than 1 rtp session it calls this function several times														
 *														
**************************************************************************************/
typedef RvStatus (*RvIppMediaControlDynamicModifyRtp)( IN const char* rtpId, IN RvSdpMsg* sdpMsg);


/***************************************************************************************/
/***********         DEVICE CONTROL       **********************************************/
/***************************************************************************************/

/***************************************************************************************
 * RvIppMediaControlOpenDev
 * -------------------------------------------------------------------------------------
 * General:   placeholder for application CB
 *				
 * Return Value: RV_OK if success
 * -------------------------------------------------------------------------------------
 * Arguments:
 * Input: 	 const char*			- devId		/ "at/hs" etc./
 *													
 * Output:   None
**************************************************************************************/
typedef RvStatus (*RvIppMediaControlOpenDev)(	IN const char*	devId);


/***************************************************************************************
 * RvIppMediaControlCloseDev
 * -------------------------------------------------------------------------------------
 * General:   placeholder for application CB
 *				
 * Return Value: RV_OK if success
 * -------------------------------------------------------------------------------------
 * Arguments:
 * Input: 	 const char*			- devId		/ "at/hs" etc./
 *													
 * Output:   None
**************************************************************************************/
typedef RvStatus (*RvIppMediaControlCloseDev)(	IN const char*	devId);


/***************************************************************************************/
/***********         CONNECT STREAMS CONTROL       *************************************/
/***************************************************************************************/
typedef enum{
	IPPMEDIACONTROL_DIR_NULL = 0x0,
	IPPMEDIACONTROL_DIR_TARGET2SOURCE	 = 0x1,
	IPPMEDIACONTROL_DIR_SOURCE2TARGET	 = 0x2,
	IPPMEDIACONTROL_DIR_BOTHWAYS	 = IPPMEDIACONTROL_DIR_TARGET2SOURCE | IPPMEDIACONTROL_DIR_SOURCE2TARGET,
	IPPMEDIACONTROL_DIR_ISOLATE = 0x4
}RvIppMediaControlRtpDirection;


/***************************************************************************************
 * RvIppMediaControlConnectRtpToRtp
 * -------------------------------------------------------------------------------------
 * General:   placeholder for application CB
 *				
 * Return Value: RV_OK if success
 * -------------------------------------------------------------------------------------
 * Arguments:
 * Input: 	 const char*	- devId		/ id that was used during registration in MDM as termination. Like "at/hs" etc./
 *			 const char*	- rtpId of rtp session should be connected to given device. 
 *														
 * Output:   
 *			 RvIppMediaControlRtpDirection	- direction	of rtp stream. 
 *			 
 *	Note:	no need in the current version		 
 *			 
**************************************************************************************/
typedef RvStatus (*RvIppMediaControlConnectRtpToRtp)(	IN const char*		rtpId_src,
														IN const char*		rtpId_target,
														IN RvIppMediaControlRtpDirection	direction);


/***************************************************************************************
 * RvIppMediaControlDisconnectRtpFromRtp
 * -------------------------------------------------------------------------------------
 * General:   placeholder for application CB
 *				
 * Return Value: RV_OK if success
 * -------------------------------------------------------------------------------------
 * Arguments:
 * Input: 	 const char*	- devId		/ id that was used during registration in MDM as termination. Like "at/hs" etc./
 *			 const char*	- rtpId of rtp session should be connected to given device. 
 *														
 * Output:   
 *			 RvIppMediaControlRtpDirection	- direction	of rtp stream. 
 *			 
 *	Note:	no need in the current version		 
 *			 
**************************************************************************************/
typedef RvStatus (*RvIppMediaControlDisconnectRtpFromRtp)(	IN const char*		rtpId_src,
															IN const char*		rtpId_target);


/***************************************************************************************
 * RvIppMediaControlConnectRtpToDev
 * -------------------------------------------------------------------------------------
 * General:   placeholder for application CB
 *				
 * Return Value: RV_OK if success
 * -------------------------------------------------------------------------------------
 * Arguments:
 * Input: 	 const char*	- devId		/ id that was used during registration in MDM as termination. Like "at/hs" etc./
 *			 const char*	- rtpId of rtp session should be connected to given device. 
 *														
 * Output:   
 *			 RvIppMediaControlRtpDirection	- direction	of rtp stream. 
 *			 
**************************************************************************************/
typedef RvStatus (*RvIppMediaControlConnectRtpToDev)(	IN const char*		devId_src,
														IN const char*		rtpId_target,
														IN RvIppMediaControlRtpDirection direction);


/***************************************************************************************
 * RvIppMediaControlDisconnectRtpFromDev
 * -------------------------------------------------------------------------------------
 * General:   placeholder for application CB
 *				
 * Return Value: RV_OK if success
 * -------------------------------------------------------------------------------------
 * Arguments:
 * Input: 	 const char*	- devId		/ id that was used during registration in MDM as termination. Like "at/hs" etc./
 *			 const char*	- rtpId of rtp session should be connected to given device. 
 *														
 * Output:   
 *			 RvIppMediaControlRtpDirection	- direction	of rtp stream. 
 *			 
**************************************************************************************/
typedef RvStatus (*RvIppMediaControlDisconnectRtpFromDev)(	IN const char*		devId_src,
															IN const char*		rtpId_target,
															IN RvIppMediaControlRtpDirection direction);



/***************************************************************************************/
/***********         MEDIA CONTROL INTERFACE       *************************************/
/***************************************************************************************/
/***************************************************************************************
 * RvIppMediaControlHandle 
 * -------------------------------------------------------------------------------------
 * General: Manifests an object that implements RvIppMediaControl interface. 
 *			Application allocates and constructs this object by  RvIppMediaControlCreate() 
 *			passing own (application) callbacks. 
 *			Application destructs and deallocates this object by  RvIppMediaControlDelete() 
 *
 **************************************************************************************/
typedef void*	RvIppMediaControlHandle;

/***************************************************************************************
 * RvIppMediaControlInterface and RvIppMediaControlCB
 * -------------------------------------------------------------------------------------
 * General: Defines interface between 2 levels of application code: 
 *			the 1st receives rtp and device -related commands from TK,
 *			the 2nd controls real rtp sessions and devices.
 *
 * -------------------------------------------------------------------------------------
 * This interface allows to decouple 2 modules,   
 *	and to implement this interface as stub <--> proxy pair
 * For proxy
 * 
 **************************************************************************************/

typedef	struct 
{
	/*
	 * Rtp session control
	 */
	RvIppMediaControlSetRtpIncoming		setRtpIncoming;		
	RvIppMediaControlSetRtpOutgoing		setRtpOutgoing;		
	RvIppMediaControlDestroyRtp			destroyRtp;		
	
	/*
	 * Device control
	 */
	RvIppMediaControlOpenDev				openDev;
	RvIppMediaControlCloseDev				closeDev;

	/*
	 * connect streams
	 */
	RvIppMediaControlConnectRtpToRtp		connectRtpToRtp;		/*when we call it?*/
	RvIppMediaControlDisconnectRtpFromRtp	disconnectRtpFromRtp;	/*when we call it?*/
	RvIppMediaControlConnectRtpToDev		connectRtpToDev;
	RvIppMediaControlDisconnectRtpFromDev	disconnectRtpFromDev;

}RvIppMediaControlInterface;

typedef	struct 
{
	RvIppMediaControlDynamicModifyRtp	dynamic_modifyRtp;

}RvIppMediaControlCB;



/*=====================================================*/
#if defined(__cplusplus)
}
#endif
/*=====================================================*/

#endif /*RV_IPP_I_MEDIA_CONTROL_H*/
