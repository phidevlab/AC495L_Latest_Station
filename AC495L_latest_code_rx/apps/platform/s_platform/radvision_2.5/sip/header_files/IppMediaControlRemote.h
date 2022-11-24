/******************************************************************************
Filename:    IppMediaControlremote.h
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
#ifndef RV_IPP_I_MEDIA_CONTROL_REMOTE_H
#define RV_IPP_I_MEDIA_CONTROL_REMOTE_H

/*=====================================================*/
#if defined(__cplusplus)
extern "C" {
#endif
/*=====================================================*/
#include    "IppMediaControl.h"
/***************************************************************************************
 *	Implements IppMediaControl interface as stub <---> proxy.
 *	----------------------------------------------------------
 *	This library may be compiled with IPP_MEDIA_CONTROL_REMOTE flag w/o it.
 *	proxy side should use IPP_MEDIA_CONTROL_REMOTE -compiled lib
 *	stub side should use lib compiled w/o IPP_MEDIA_CONTROL_REMOTE flag
 *	
 **************************************************************************************/

#ifdef IPP_MEDIA_CONTROL_REMOTE

extern RvIppMediaControlInterface	test_mediaControlCB;
	
/***************************************************************************************
 * RvIppMediaControlRemoteCreate
 * -------------------------------------------------------------------------------------
 * General:   allocates and constructs RvIppMediaControlRemote object
 *				
 * Return Value: object handle or NULL if failed
 * -------------------------------------------------------------------------------------
 * Arguments:
 * Input: 	 RvIppRtpControl*		- application CBs
 *			 RvIppDevControl*		- application CBs
 * Output:   None
 **************************************************************************************/
RvIppMediaControlHandle RvIppMediaControlCreate( IN RvIppMediaControlInterface* callbacks,
												IN const char* localIp);

void RvIppMediaControlDelete( RvIppMediaControlHandle hnd);


#else /*IPP_MEDIA_CONTROL_REMOTE*/
/***************************************************************************************/
/***********         MEDIA CONTROL       ***********************************************/
/***************************************************************************************/

/***************************************************************************************
 * RvIppMediaControlCreate
 * -------------------------------------------------------------------------------------
 * General:   allocates and constructs RvIppMediaControl object
 *				
 * Return Value: object handle or NULL if failed
 * -------------------------------------------------------------------------------------
 * Arguments:
 * Input: 	 RvIppRtpControl*		- application CBs
 *			 RvIppDevControl*		- application CBs
 * Output:   None
 **************************************************************************************/
RvIppMediaControlHandle RvIppMediaControlCreate( IN RvIppMediaControlCB* callbacks,
												IN RvEppClient*	ec);
												
/***************************************************************************************
 * RvIppMediaControlDelete
 * -------------------------------------------------------------------------------------
 * General:   destructs and deallocates RvIppMediaControl object
 *				
 * Return Value: None
 * -------------------------------------------------------------------------------------
 * Arguments:
 * Input: 	 RvIppMediaControlHandle	- object handle
 *			 
 * Output:   None
 **************************************************************************************/
void RvIppMediaControlDelete( RvIppMediaControlHandle hnd);


/***************************************************************************************
 * RvIppMediaControlGetInterface
 * -------------------------------------------------------------------------------------
 * General:   return pointer to table of interface functions
 *				
 * Return Value: None
 * -------------------------------------------------------------------------------------
 * Arguments:
 * Input: 	 RvIppMediaControlHandle	- object handle
 *			 
 * Output:   None
 **************************************************************************************/
RvIppMediaControlInterface* RvIppMediaControlGetInterface( RvIppMediaControlHandle hnd);
#endif /*IPP_MEDIA_CONTROL_REMOTE*/


/***************************************************************************************
 * RvIppMediaDispatchCommand
 * -------------------------------------------------------------------------------------
 * General: This is an additional function aimed to workaround the current design restriction.
 *			
 *			We need to support an external mechanism of communication via GUI-incorporated listening socket.
 *			Listening socket calls this function upon each receiving buffer. If returned parameter - bHandled - is true
 *			it means that passed buffer was utilized by current library. 
 *			If bHandled =false than listen socket should handle this message itself.
 * -------------------------------------------------------------------------------------
 * Arguments:
 * Input: 	RvIppMediaControlHandle	- object handle
 *			RvAddress*				- sockFromAddr /sock address that sends this buf/
 * 			RvUint8*				- buf, 
 *			 
 * Output:   None
 *			RvBool*					- bHandled  /true if the buffer is handled by this module/
 * 
 **************************************************************************************/
RvStatus RvIppMediaDispatchCommand( IN RvIppMediaControlHandle hnd,
								   IN RvAddress*	sockFromAddr,
								   IN RvUint8*		buf, 
								   OUT RvBool*		bHandled);
								   



	
/*=====================================================*/
#if defined(__cplusplus)
}
#endif
/*=====================================================*/

#endif /*RV_IPP_I_MEDIA_CONTROL_REMOTE_H*/
