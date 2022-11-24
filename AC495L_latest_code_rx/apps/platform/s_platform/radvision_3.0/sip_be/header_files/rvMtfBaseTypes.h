/******************************************************************************
Filename:    rvMtfBaseTypes.h
Description: This files include basic type definitions for MTF.
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

#ifndef RV_MTF_BASE_TYPES_H
#define RV_MTF_BASE_TYPES_H

#include "rvMtfSignalsTypes.h"
#include "rvMtfHandles.h"
#include "rvIppCfwApi.h"
#include "rvmdmobjects.h"
#include "RvSipStackTypes.h"
#include "RvSipCallLegTypes.h"
#include "RvSipRegClientTypes.h"
#include "RvSipSubscriptionTypes.h"
#include "RvSipAuthenticator.h"
#include "RvSipMsgTypes.h"
#include "rvtypes.h"


/*@*****************************************************************************
 * Type: RvMtfTerminationConfig (RvMtfTerminationsPkg)
 * -----------------------------------------------------------------------------
 * Description: This structure includes the configuration parameters for a
 * termination that are set when the termination is registered to the MTF.
 *
 ****************************************************************************@*/
typedef struct  {
    RvChar						username[RV_NAME_STR_SZ];
		/* This parameter is used for Authentication. */

    RvChar						password[RV_NAME_STR_SZ];
		/* This parameter is used for Authentication. */

    RvChar						displayName[RV_NAME_STR_SZ];
		/* This parameter is used as the sendind username in the
	       outgoing Invite. */

	RvChar						registrarAddress[RV_NAME_STR_SZ];
		/* IP address or domain name of the Registrar. All Register messages
	       will be sent to this address. */

	RvUint16					registrarPort;
		/* Port number of the Registrar. */

	RvChar						outboundProxyAddress[RV_NAME_STR_SZ];
		/* IP address or domain name of the outbound Proxy. All outgoing
	       messages will be sent to this address. */

	RvUint16					outboundProxyPort;
		/* Port number of the outbound Proxy. */

	RvUint16					registerExpires;
		/* Register refresh timeout. Indicates the time period between
		   Register messages, in seconds. When this parameter is not set,
	       the value will be taken from the registrationExpire parameter
	       in RvIppSipPhoneCfg. */

	RvSipTransport				transportType;
		/* The Transport type of the outgoing messages. Valid values are
		   RVSIP_TRANSPORT_UDP, RVSIP_TRANSPORT_TCP and
		   RVSIP_TRANSPORT_UNDEFINED. If this parameter is set to
		   RVSIP_TRANSPORT_UNDEFINED, the value will be taken from the
		   transportType parameter in RvIppSipPhoneCfg.
           Default: RVSIP_TRANSPORT_UDP. */

	RvMdmDigitMap*				digitMap;
		/* Defines digitmap pattern. If this parameter is not set, MTF
		   will use its default one. */

} RvMtfTerminationConfig;


/*@*****************************************************************************
*  RvMtfRegisterAnalogTermCompletedEv (RvMtfBasePkg)
* -----------------------------------------------------------------------------
*  General :    This callback is called after the application registers an
*               analog termination to the MTF by calling
                rvMtfRegisterAnalogTermination(), to indicate that the
*				registration is complete.
*
*  Arguments:
*  Input:          hTerm	- Handle to the terminal.
*				   hAppTerm	- Handle to the application data associated with
*                  the terminal.
*
*  Return Value:   None.
****************************************************************************@*/
typedef void (RVCALLCONV *RvMtfRegisterAnalogTermCompletedEv)(
									IN RvIppTerminalHandle      hTerm,
									IN RvMtfTerminalAppHandle	hAppTerm);

/*@*****************************************************************************
*  RvMtfRegisterIPPhoneTermsCompletedEv (RvMtfBasePkg)
* -----------------------------------------------------------------------------
*  General :    This callback is called after the application registers
*               IP Phone terminations to the MTF by calling
*               rvMtfRegisterIPPhoneTerminations(), to indicate that the
*               registration is complete.
*
*  Arguments:
*  Input:          hTerm	- Handle to the terminal.
*				   hAppTerm	- Handle to the application data associated with
*                  the terminal.
*
*  Return Value:    None.
****************************************************************************@*/
typedef void (RVCALLCONV *RvMtfRegisterIPPhoneTermsCompletedEv)(
									IN RvIppTerminalHandle      hTerm,
									IN RvMtfTerminalAppHandle	hAppTerm);

/*@*****************************************************************************
*  RvMtfRegisterVideoTermsCompletedEv (RvMtfBasePkg)
* -----------------------------------------------------------------------------
*  General :  This callback is called after the application registers video
*			  terminations to the MTF by calling
*             rvMtfRegisterVideoTerminations(), to indicate that the
*             registration is complete.
*
*  Arguments:
*  Input:          hTerm	- Handle to the terminal.
*				   hAppTerm	- Handle to the application data associated with
*                             the terminal.
*
*  Return Value:    None.
****************************************************************************@*/
typedef void (RVCALLCONV *RvMtfRegisterVideoTermsCompletedEv)(
									IN RvIppTerminalHandle      hTerm,
									IN RvMtfTerminalAppHandle	hAppTerm);

/*@*****************************************************************************
*  RvMtfUnregisterAnalogTermCompletedEv (RvMtfBasePkg)
* -----------------------------------------------------------------------------
*  General :    This callback is called after the application unregisters an
*               analog termination to the MTF by calling
*				rvMtfUnregisterAnalogTermination(), to indicate that the
*				unregistration is complete.
*
*  Arguments:
*  Input:          hTerm	- Handle to the terminal.
*				   hAppTerm	- Handle to the application data associated with
*                             the terminal.
*
*  Return Value:    None.
****************************************************************************@*/
typedef void (RVCALLCONV *RvMtfUnregisterAnalogTermCompletedEv)(
									IN RvIppTerminalHandle      hTerm,
									IN RvMtfTerminalAppHandle	hAppTerm);

/*@*****************************************************************************
*  RvMtfUnregisterIPPhoneTermsCompletedEv (RvMtfBasePkg)
* -----------------------------------------------------------------------------
*  General :    This callback is called after the application unregisters
*               IP Phone terminations from the MTF by calling
*				rvMtfUnregisterIPPhoneTerminations(), to indicate that the
*				unregistration is complete.
*
*  Arguments:
*  Input:          hMtf		- Handle to the MTF instance.
*				   hAppTerm	- Handle to the application data associated with
*                             the terminal.
*
*  Return Value:    None.
****************************************************************************@*/
typedef void (RVCALLCONV *RvMtfUnregisterIPPhoneTermsCompletedEv)(
									IN RvIppTerminalHandle      hTerm,
									IN RvMtfTerminalAppHandle	hAppTerm);

/*@*****************************************************************************
*  RvMtfUnregisterVideoTermsCompletedEv (RvMtfBasePkg)
* -----------------------------------------------------------------------------
*  General :    This callback is called after the application unregisters video
*				terminations from the MTF by calling
*               rvMtfUnregisterVideoTerminations(), to indicate that the
*				unregistration is complete.
*
*  Arguments:
*  Input:           hTerm		- Handle to the terminal.
*				    hAppTerm	- Handle to the application data associated
*                                 with the terminal.
*
*  Return Value:    None.
****************************************************************************@*/
typedef void (RVCALLCONV *RvMtfUnregisterVideoTermsCompletedEv)(
									IN RvIppTerminalHandle      hTerm,
									IN RvMtfTerminalAppHandle	hAppTerm);

/*@*****************************************************************************
*  RvMtfUnregisterTermFromServerCompletedEv (RvMtfBasePkg)
* -----------------------------------------------------------------------------
*  General :    This callback is called after the application registers a
*				termination (analog or IP Phone) to the SIP server, to indicate
*               that the registration is complete (either a reply was received
*               from the server, or a timeout occurred). The application registers
*				a termination to the SIP Registrar by calling
*				rvMtfRegisterTerminationToSipServer().
*
*  Arguments:
*  Input:          hTerm	- Handle to the terminal.
*				   hAppTerm	- Handle to the application data associated with
*                  the terminal.
*
*  Return Value:    None.
****************************************************************************@*/
typedef void (RVCALLCONV *RvMtfUnregisterTermFromServerCompletedEv)(
									IN RvIppTerminalHandle      hTerm,
									IN RvMtfTerminalAppHandle	hAppTerm);

/*@*****************************************************************************
*  RvMtfMapDialStringToAddressEv (RvMtfBasePkg)
* -----------------------------------------------------------------------------
*  General :    This callback is called when the user presses a complete
*				phone number (dial string). It requires the application to
*               return a SIP address that will be used to send out an Invite
*				message. This callback is relevant when the user establishes
*				a call by pressing digits. It is not relevant when the user
*               provides the destination SIP address by calling
*				rvMtfTerminalMakeCall().
*
*  Arguments:
*  Input:			hTerm		- Handle to the terminal.
*				    hAppTerm	- Handle to the application data associated
*                                 with the terminal.
*					dialString	- The phone number dialed by the user.
*
*  Output:          address		- The string containing the SIP address to be
*                                 used to send out the Invite message.
*
*  Return Value:    RV_OK if successful, other if not.
****************************************************************************@*/
typedef RvStatus (RVCALLCONV *RvMtfMapDialStringToAddressEv)(
									IN  RvIppTerminalHandle		hTerm,
									IN  RvMtfTerminalAppHandle	hAppTerm,
									IN  RvChar*					dialString,
									OUT RvChar*					address);

/*@*****************************************************************************
*  RvMtfMapAddressToTerminationEv (RvMtfBasePkg)
* -----------------------------------------------------------------------------
*  General :    This callback is optional. It is used for mapping incoming
*				calls to a termination. If this callback is not registered by
*				the application, the MTF will use its default implementation
*				and will match the username in the To header of the incoming
*				Invite to the termination ID. If the application registers
*			    this callback, it can decide dynamically which termination
*               will receive the incoming call. The returned termination
*				must be registered with the MTF before the callback is called.
*
*  Arguments:
*  Input:			hMtf		- Handle to the MTF instance.
*				    hAppMtf		- Handle to the application data associated
*                                 with the MTF instance.
*					address		- The SIP address taken from the To header
*                                 of the incoming Invite message.
*
*  Output:          termId		- The termination ID to receive the call.
*                                 This termination must be registered with
*								  the MTF before the callback is called.
*
*  Return Value:    RV_OK if successful, other if not.
****************************************************************************@*/
typedef RvStatus (RVCALLCONV *RvMtfMapAddressToTerminationEv)(
									IN  RvMtfHandle			hMtf,
									IN  RvMtfAppHandle		hAppMtf,
									IN  RvChar*				address,
									OUT RvChar*				termId);

/*@*****************************************************************************
*  RvMtfMatchDialStringToPatternEv (RvMtfBasePkg)
* -----------------------------------------------------------------------------
*  General :    This callback is optional. It is used for matching a phone number
*				dialed by the user to a digitmap pattern. This callback is relevant for  
*				user applications that registered their own digitmap pattern. User applications
*				that do not register their own digitmap can use the MTF's default digitmap 
*				(see the MTF Programmer Guide for more information 
*			    about the default digitmap). 
*               This callback is called every time the user presses a digit.
*				The user application should check whether the dial string dialed by the user
*				matches one of its patterns and return the matching type. If the matching
*				type is PARTIALMATCH or FULLMATCH, the user application should also set the 
*				timeout that the MTF needs to wait for the next digit. 
*
*  Arguments:
*  Input:			hTerm			- Handle to the terminal.
*					hAppTerm		- Handle to the user application data associated with
*									  the terminal.
*					dialString		- The phone number that was dialed by the user so far.
*
*  Output:          timerDuration	- The time duration to wait for the next digit, 
*									  or a timeout.
*
*  Return Value:    The digitmap matching type of the phone number. Possible values are:

*		            NOMATCH--The dial string does not match any legal pattern. 
*			        The MTF stops collecting digits and starts playing a warning tone.

*		            PARTIALMATCH--The dial string may match a legal pattern. 
*				    The MTF continues collecting digits.

*		            FULLMATCH--The dial string matches a legal pattern, but might be		
*				    ambiguous (i.e., more digits can be collected).
*					The MTF continues collecting digits.

*		            UNAMBIGUOUSMATCH--The dial string matches a legal pattern.
*				    The MTF stops collecting digits and tries to map this dial string
*				    to a destination address, by invoking the callback RvMtfMapDialStringToAddressEv().
*														   
****************************************************************************@*/
typedef RvMdmDigitMapMatchType (RVCALLCONV *RvMtfMatchDialStringToPatternEv)(
									IN  RvIppTerminalHandle		hTerm,
									IN  RvMtfTerminalAppHandle	hAppTerm,
									IN  RvChar*					dialString,
									OUT RvUint*					timerDuration);


/*@*****************************************************************************
 * Type: RvMtfMdmClbks (RvMtfBasePkg)
 * -----------------------------------------------------------------------------
 * Description: This structure includes MDM callbacks.
 ****************************************************************************@*/
typedef struct
{
    RvMtfStartSignalEv							startSignalCB;
    RvMtfStopSignalEv							stopSignalCB;
    RvMtfRegisterIPPhoneTermsCompletedEv		registerIPPhoneTermsCompletedCB;
    RvMtfUnregisterIPPhoneTermsCompletedEv		unregisterIPPhoneTermsCompletedCB;
	RvMtfRegisterVideoTermsCompletedEv			registerVideoTermsCompletedCB;
    RvMtfUnregisterVideoTermsCompletedEv		unregisterVideoTermsCompletedCB;
	RvMtfRegisterAnalogTermCompletedEv			registerAnalogTermCompletedCB;
    RvMtfUnregisterAnalogTermCompletedEv		unregisterAnalogTermCompletedCB;
    RvMtfUnregisterTermFromServerCompletedEv	unregisterTermFromServerCompletedCB;
    RvMtfMapDialStringToAddressEv				mapDialStringToAddressCB;
	RvMtfMapAddressToTerminationEv				mapAddressToTerminationCB;
	RvMtfMatchDialStringToPatternEv				matchDialStringToPatternCB;
}RvMtfMdmClbks;



#endif /*RV_MTF_BASE_TYPES_H*/
