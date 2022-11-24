/******************************************************************************
Filename:    rvMtfBaseApi.h
Description: This file includes public functions for initialization and termination
			 of MTF, as well as registration and unregistration of terminations.
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
#ifndef RV_RVMTFMGR_H
#define RV_RVMTFMGR_H

#include "rvMtfExtControlTypes.h"
#include "rvMtfMediaApi.h"
#include "rvMtfEventsApi.h"
#include "RvSipMsgTypes.h"

#ifdef RV_MTF_VIDEO
#include "rvMdmControlVideoApi.h"
#endif

/*@*****************************************************************************
 * Package: RvMtfBasePkg (root)
 * -----------------------------------------------------------------------------
 * Title: Base
 *
 * General: This section contains functions and type definitions that are used
 *          for working with the MTF, for example for initializing, configuring
 *          and terminating the MTF.
 ****************************************************************************@*/

/*@*****************************************************************************
 * Package: RvMtfTerminationsPkg (root)
 * -----------------------------------------------------------------------------
 * Title: Terminations
 *
 * General: This section contains functions and type definitions used for
 *          working with terminations, for example for registering,
 *          configuring and unregistering terminations.
 ****************************************************************************@*/

/*@*****************************************************************************
 * rvMtfTerminationInitConfig (RvMtfTerminationsPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         Initializes the RvMtfTerminationConfig structure with default values.
 *         This function should be called before passing RvMtfTerminationConfig to
 *         other functions.
 *
 * Arguments:
 *
 * Input:  termConfig    - Pointer to the uninitialized structure.
 *
 * Return Value: None.
 ****************************************************************************@*/
RVAPI void RVCALLCONV rvMtfTerminationInitConfig(RvMtfTerminationConfig* termConfig);

/*@*****************************************************************************
 * rvMtfSystemInit (RvMtfBasePkg)
 * -----------------------------------------------------------------------------
 * General:
 *        Initializes MTF elements that must be ready before the MTF can be
 *        constructed. This function initializes the core and the allocator
 *        and resets global structures. This is the first function to be
 *        called in the initialization process.
 *
 * Arguments:
 *
 * Input:  None.
 *
 * Output: None.
 *
 * Return Value:  None.
 ****************************************************************************@*/
RVAPI void RVCALLCONV rvMtfSystemInit(void);

/*@*****************************************************************************
 * rvMtfSystemEnd (RvMtfBasePkg)
 * -----------------------------------------------------------------------------
 * General:
 *        Shuts down the MTF system (Common Core). This is the last function
 *        to be called in the shutdown process.
 *
 * Arguments:
 *
 * Input:  None.
 *
 * Output: None.
 *
 * Return Value:  None.
 ****************************************************************************@*/
RVAPI void RVCALLCONV rvMtfSystemEnd(void);

/*@*****************************************************************************
 * rvMtfInitConfig (RvMtfBasePkg)
 * -----------------------------------------------------------------------------
 * General:
 *         Initializes the MTF configuration structure with default values.
 *         This function should be called before rvMtfSipConstruct().
 *
 * Arguments:
 *
 * Input:  cfg  -   Pointer to the MTF configuration structure.
 *
 * Return Value: RV_OK if successful, other on failure.
 ****************************************************************************@*/
RVAPI RV_Status RVCALLCONV rvMtfInitConfig(
		OUT RvIppSipPhoneCfg*    cfg);

/*@*****************************************************************************
 * rvMtfSipConstruct (RvMtfBasePkg)
 * -----------------------------------------------------------------------------
 * General:
 *			Constructs the MTF. After this function is called, the MTF will
 *			start processing user events and SIP messages.
 *
 * Arguments:
 *
 * Input:  config		-   Pointer to the configuration structure.
 *
 * Output: mtfHandle    -   Handle to the MTF.
 *
 * Return Value:  RV_OK if successful, other on failure.
 ****************************************************************************@*/
RVAPI RvStatus RVCALLCONV rvMtfSipConstruct(
                                 OUT  RvMtfHandle*				hTerm,
                                 IN   RvIppSipPhoneCfg*			config);

/*@*****************************************************************************
 * rvMtfSipDestruct (RvMtfBasePkg)
 * -----------------------------------------------------------------------------
 * General:
 *        Destructs the MTF. After this function is called, the MTF will stop
 *		  processing user events and SIP messages.
 *
 * Arguments:
 *
 * Input:  hMtf       -   Pointer to the configuration structure.
 *
 * Return Value:  RV_OK if successful, other on failure.
 ****************************************************************************@*/
RVAPI RvStatus RVCALLCONV rvMtfSipDestruct(RvMtfHandle     hTerm);

/*@*****************************************************************************
 * rvMtfLoadMediaCapabilities (RvMtfBasePkg)
 * -----------------------------------------------------------------------------
 * General:
 *         Loads media capabilities to the MTF. The media capabilities will be
 *         passed to the application when the callback createMedia is called.
 *         This function should be called after rvMtfSipConstruct() is called.
 *
 * Arguments:
 *
 * Input:  mtfHandle - Handle to the MTF.
 *         sdp       - Pointer to an SDP structure that contains media
 *                     capabilities.
 *
 * Return Value: None.
 ****************************************************************************@*/
RVAPI RvStatus RVCALLCONV rvMtfLoadMediaCapabilities(
                      IN RvMtfHandle        hMtf,
                      IN RvSdpMsg*          mediaCaps);

/*@*****************************************************************************
 * rvMtfRegisterMediaCallbacks (RvMtfBasePkg)
 * -----------------------------------------------------------------------------
 * General:
 *         Registers media callbacks to the MTF. This function should be called
 *         after rvMtfSipConstruct() is called.
 *
 * Arguments:
 *
 * Input:  mtfHandle  - Handle to the MTF.
 *         mediaClbks - Pointer to the structure that contains media callbacks.
 *
 * Return Value: None.
 ****************************************************************************@*/
RVAPI void RVCALLCONV rvMtfRegisterMediaCallbacks(
                IN RvMtfHandle          hMtf,
                IN RvMtfMediaClbks*     mediaClbks);

/*@*****************************************************************************
 * rvMtfRegisterMdmCallbacks (RvMtfBasePkg)
 * -----------------------------------------------------------------------------
 * General:
 *         Registers MDM callbacks to the MTF. This function should be called
 *         after rvMtfSipConstruct() is called.
 *
 * Arguments:
 *
 * Input:  mtfHandle  - Handle to the MTF.
 *         mdmClbks   - Pointer to the structure that contains MDM callbacks.
 *
 * Return Value: None.
 ****************************************************************************@*/
RVAPI void RVCALLCONV rvMtfRegisterMdmCallbacks(
                IN RvMtfHandle          hMtf,
                IN RvMtfMdmClbks*       mdmClbks);

/*@*****************************************************************************
 * rvMtfSipRegisterExtClbks (RvMtfBasePkg)
 * -----------------------------------------------------------------------------
 * General:
 *         Registers SIP Extensibility callbacks to the MTF. This function
 *         should be called before rvMtfSipConstruct() is called.
 *
 * Arguments:
 *
 * Input:  clbks - Pointer to the structure that contains SIP Extensibility
 *                 callbacks.
 *
 * Return Value: None.
 ****************************************************************************@*/
RVAPI void RVCALLCONV rvMtfSipRegisterExtClbks(IN RvMtfSipControlClbks* clbks);


/*@*****************************************************************************
 * rvMtfStart (RvMtfBasePkg)
 * -----------------------------------------------------------------------------
 * General:
 *         Starts the MTF. After this function is called, the MTF will start
 *         processing events. This should be the last function to be called
 *         in the MTF initialization process.
 *
 * Arguments:
 *
 * Input:  mtfHandle    - Handle to the MTF.
 *
 * Return Value: RV_OK on success, a negative value on failure.
 ****************************************************************************@*/
RVAPI RvStatus RVCALLCONV rvMtfStart(IN RvMtfHandle        hMtf);

/*@*****************************************************************************
 * rvMtfStop (RvMtfBasePkg)
 * -----------------------------------------------------------------------------
 * General:
 *         Stops the MTF. After this function is called, the MTF will stop
 *         processing events. This should be the first function to be called
 *         in the MTF shutdown process.
 *
 * Arguments:
 *
 * Input:  mtfHandle - Handle to the MTF.
 *
 * Return Value: RV_OK on success, a negative value on failure.
 ****************************************************************************@*/
RVAPI RvStatus RVCALLCONV rvMtfStop(IN RvMtfHandle     hMtf);


/*@*****************************************************************************
 * rvMtfSetApplicationHandle (RvMtfBasePkg)
 * -----------------------------------------------------------------------------
 * General:
 *         Sets the application handle (user data) associated with the MTF
 *         instance. This data can be retrieved by calling
 *         rvMtfGetApplicationHandle().
 *
 * Arguments:
 *
 * Input:  mtfHandle    - Handle to the MTF instance.
 *         hAppMtf		- Handle to the MTF application data associated with
 *                        the MTF instance.
 *
 * Return Value: None.
 ****************************************************************************@*/
RVAPI RvStatus RVCALLCONV rvMtfSetApplicationHandle(
                IN RvMtfHandle          hMtf,
                IN RvMtfAppHandle       hAppMtf);

/*@*****************************************************************************
 * rvMtfGetApplicationHandle (RvMtfBasePkg)
 * -----------------------------------------------------------------------------
 * General:
 *         Gets the application handle (user data) associated with the MTF
 *         instance, as was set by the application when
 *         rvMtfSetApplicationHandle() was called.
 *
 * Arguments:
 *
 * Input:  mtfHandle    - Handle to the MTF instance.
 *
 * Output: hAppMtf		- Handle to the MTF application data associated with
 *                        the MTF instance.
 *
 * Return Value: None.
 ****************************************************************************@*/
RVAPI RvStatus RVCALLCONV rvMtfGetApplicationHandle(
                IN  RvMtfHandle          hMtf,
                OUT RvMtfAppHandle*      hAppMtf);

/*@*****************************************************************************
 * rvMtfRegisterIPPhoneTerminations (RvMtfBasePkg)
 * -----------------------------------------------------------------------------
 * General:
 *			This function is relevant for IP Phone/Videophone applications
 *			only. It registers IP Phone terminations to the MTF.
 *			The registration process is asynchronous; the application will
 *          be notified that the process is complete when the callback
 *			RvMtfRegisterIPPhoneTermsCompletedEv() is called. When
 *			registration is complete, the terminal will be able to send and
 *			receive audio calls. To send and receive video calls, the
 *          application should call rvMtfRegisterVideoTerminations() after
 *          calling this function.
 *
 * Arguments:
 *
 * Input:  hMtf	     - Handle to the MTF instance.
 *         termId    - String containing the termination ID. The termination ID
 *                     should be unique. This parameter is used to register
 *                     with the Registrar (this value is set in the From and To
 *					   headers of the Register message) and to match incoming
 *                     calls to the terminal. Incoming calls should be received
 *                     with: <termid>@example.com.
 *         hAppTerm  - Handle to the application data associated with this
 *                     terminal.
 *
 * Output: hTerm	 - Handle to the termination. This handle can be used to
 *                     unregister, or to retrieve information from the terminal.
 *
 *
 * Return Value: RV_OK if successful, other if not.
 ****************************************************************************@*/
RVAPI RvStatus	RVCALLCONV rvMtfRegisterIPPhoneTerminations (
				 IN RvMtfHandle			        hMtf,
				 IN RvChar*				        termId,
				 IN RvMtfTerminalAppHandle		hAppTerm,
                 OUT RvIppTerminalHandle*	    hTerm);

/*@*****************************************************************************
 * rvMtfUnregisterIPPhoneTerminations (RvMtfBasePkg)
 * -----------------------------------------------------------------------------
 * General:
 *			This function is relevant for IP Phone/Videophone applications only.
 *			It unregisters IP Phone terminations from the MTF.
 *          The unregistration process is asynchronous; the application will
 *          be notified that the process is complete when the callback
 *          RvMtfUnregisterIPPhoneTermsCompletedEv() is called. When the
 *          unregistration is complete, the terminal will not be able to send
 *			and receive calls.
 *
 * Arguments:
 *
 * Input:  hMtf		     - Handle to the MTF instance.
 *         hTerm		 - Handle to the termination.
 *
 * Return Value: RV_OK if successful, other if not.
 ****************************************************************************@*/
RVAPI RvStatus	RVCALLCONV rvMtfUnregisterIPPhoneTerminations (
				IN RvMtfHandle			    hMtf,
                IN RvIppTerminalHandle		hTerm);

/*@*****************************************************************************
 * rvMtfRegisterAnalogTermination (RvMtfBasePkg)
 * -----------------------------------------------------------------------------
 * General:
 *			This function is relevant for IAD/Residential Gateway applications
 *			only. It registers one analog termination to the MTF.
 *			The application should call this function to register each analog
 *          terminal/line separately. Each termination can be configured
 *			to different values for Registrar, transport type, etc.
 *			The registration process is asynchronous; the application will be
 *			notified that the process is complete when the callback
 *          RvMtfRegisterAnalogTermCompletedEv() is called. When the registration
 *			is complete, the terminal will be able to send and receive calls.
 *
 * Arguments:
 *
 * Input:  hMtf	      - Handle to the MTF instance.
 *		   termConfig - Structure containing the configuration parameters.
 *         termId     - String containing the termination ID. The
 *                      termination ID should be unique. This parameter is used
 *                      to register with the Registrar (this value will be set
 *					    in the From and To headers of Register message) and to
 *                      match incoming calls to the terminal. Incoming calls
 *                      should be received with: <termid>@example.com.
 *         hAppTerm   - Handle to the application data associated with this
 *                      terminal.
 *
 * Output: hTerm	  - Handle to the termination. This handle can be used
 *                      to unregister, or to retrieve information from the
 *                      terminal.
 *
 * Return Value: RV_OK if successful, other if not.
 ****************************************************************************@*/
RVAPI RvStatus	RVCALLCONV rvMtfRegisterAnalogTermination (
				IN RvMtfHandle			        hMtf,
				IN RvChar*				        termId,
				IN RvMtfTerminationConfig*	    termConfig,
				IN RvMtfTerminalAppHandle		hAppTerm,
                OUT RvIppTerminalHandle*	    hTerm);

/*@*****************************************************************************
 * rvMtfUnregisterAnalogTermination (RvMtfBasePkg)
 * -----------------------------------------------------------------------------
 * General:
 *			This function is relevant for IAD/Residential Gateway applications
 *			only. It unregisters one analog termination from the MTF.
 *          The application should call this function to unregister each analog
 *			terminal/line separately. The unregistration process is asynchronous;
 *			the application will be notified that the process is complete when
 *			the callback RvMtfUnregisterIPPhoneTermsCompletedEv() is called.
 *			When the unregistration is complete, the terminal will not be able
 *			to send and receive calls.
 *
 * Arguments:
 *
 * Input:  hMtf		     - Handle to MTF instance.
 *         hTerm		 - Handle to Termination.
 *
 * Return Value: RV_OK if successful, other if not.
 ****************************************************************************@*/
RVAPI RvStatus	RVCALLCONV rvMtfUnregisterAnalogTermination (
				IN RvMtfHandle					hMtf,
				RvIppTerminalHandle				hTerm);

#ifdef RV_MTF_VIDEO
/*@*****************************************************************************
 * rvMtfRegisterVideoTerminations (RvMtfBasePkg)
 * -----------------------------------------------------------------------------
 * General:
 *			This function is relevant for Videophone applications only.
 *			It registers video terminations to the MTF. The
 *			registration process is asynchronous; the application will be
 *          notified that the process is complete when the callback
 *			RvMtfRegisterVideoTermsCompletedEv() is called. When the
 *			registration is complete, the terminal will be able to send and
 *          receive video calls.
 *
 * Arguments:
 *
 * Input:  hMtf	    - Handle to the MTF instance.
 *         hAppTerm - Handle to the application data associated with the
 *                    video terminal.
 *
 * Output: hTerm	- Handle to the video termination. This handle can be
 *                    used to unregister, or to retrieve information from the
 *                    terminal.
 *
 * Return Value: RV_OK if successful, other if not.
 ****************************************************************************@*/
RVAPI RvStatus	RVCALLCONV rvMtfRegisterVideoTerminations (
				IN RvMtfHandle			        hMtf,
                IN RvMtfTerminalAppHandle		hAppTerm);

/*@*****************************************************************************
 * rvMtfUnregisterVideoTerminations (RvMtfBasePkg)
 * -----------------------------------------------------------------------------
 * General:
 *			This function is relevant for Videophone applications only.
 *			It unregisters video terminations from the MTF.
 *          The unregistration process is asynchronous; the application will be
 *          notified that the process is complete when the callback
 *			RvMtfUnregisterVideoTermsCompletedEv() is called.
 *			When the unregistration is complete, the terminal will not be able
 *			to send and receive calls.
 *
 * Arguments:
 *
 * Input:  hMtf		     - Handle to the MTF instance.
 *         hTerm		 - Handle to the termination.
 *
 * Return Value: RV_OK if successful, other if not.
 ****************************************************************************@*/
RVAPI RvStatus	RVCALLCONV rvMtfUnregisterVideoTerminations (
                IN RvMtfHandle			    hMtf);

#endif /* RV_MTF_VIDEO */



/*@*****************************************************************************
 * rvMtfRegisterTerminationToSipServer (RvMtfTerminationsPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         Sends a Register message to the SIP Registrar for this
 *		   termination (analog or IP Phone). The SIP Registrar address is taken
 *         from the configuration.
 *
 * Arguments:
 *
 * Input:  mtfHandle  - Pointer to the MTF.
 *         hTerm	  - Handle to the terminal to be registered to the
 *                      Registrar.
 *
 * Return Value: RV_OK if successful.
 ****************************************************************************@*/
RVAPI RvStatus  RVCALLCONV rvMtfRegisterTerminationToSipServer (
                IN RvMtfHandle              hMtf,
                IN RvIppTerminalHandle      hTerm);

/*@*****************************************************************************
 * rvMtfRegisterAllTerminationsToSipServer (RvMtfBasePkg)
 * -----------------------------------------------------------------------------
 * General:
 *         Sends a Register message to the SIP Registrar for every
 *         termination that is registered to the MTF (both analog and IP Phone).
 *		   The SIP Registrar address is taken from the configuration.
 *
 * Arguments:
 *
 * Input:  mtfHandle        - Pointer to the MTF.
 *
 * Return Value: RV_OK if successful.
 ****************************************************************************@*/
RVAPI RvStatus  RVCALLCONV rvMtfRegisterAllTerminationsToSipServer (
				IN RvMtfHandle    hMtf);


/*@*****************************************************************************
 * rvMtfUnregisterTerminationFromSipServer (RvMtfTerminationsPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         Unregisters the termination from the SIP Registrar
 *		   by sending a Register message with Expires 0.
 *         The SIP Registrar address is taken from the configuration.
 *
 * Arguments:
 *
 * Input:  mtfHandle        - Pointer to the MTF.
 *         hTerm		    - Handle to the terminal to be unregistered.
 *
 * Return Value: RV_OK if successful.
 ****************************************************************************@*/
RVAPI RvStatus RVCALLCONV rvMtfUnregisterTerminationFromSipServer(
                IN RvMtfHandle				hMtf,
                IN RvIppTerminalHandle      hTerm);



#endif /*RV_RVMTFMGR_H*/

