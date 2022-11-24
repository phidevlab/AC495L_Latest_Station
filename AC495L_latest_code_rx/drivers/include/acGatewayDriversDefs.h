/******************************************************************** 
 * acGatewayDriversDefs - AudioCodes Analog VoIP Gateway DSP and SLIC API          * 
 *                                                                        *
 * Structure, Enum and Constant Type Definitions                          * 	
 *                                                                        *
 * File Name:	acGatewayDriversDefs.h                                    *
 *                                                                        *
 * Copyright (C) 2007 AudioCodes Ltd.                                     *
 *                                                                        *
 * Modifications :                                                        *
 *                                                                        *
 * 30/12/07 - Coding started.                                             *
 *                                                                        *
 ********************************************************************/

#ifndef	_ACGATEWAYDRIVERSDEFS_H
#define	_ACGATEWAYDRIVERSDEFS_H

#ifndef __KERNEL__
#include "dsp_drv.h"
#endif

/********************************************************************
 * Constant Values Definitions												*
 ********************************************************************/

/*****   acGateway General parameters  *****/

#define ACG_NUMBER_OF_DSP_CHANNEL		NUM_OF_DSP_CHANNELS

#define DSP_NAME_STR			"AclDsp"
#define FXS_DRV_STR				"FxsDrv"
#define DAA_DRV_STR				"DaaDrv"

#define ACL_DSP_DRV_FD_NAME_STR                                 "/dev/"DSP_NAME_STR
#define ACL_FXS_DRV_FD_NAME_STR                                 "/dev/"FXS_DRV_STR
#define ACL_DAA_DRV_FD_NAME_STR                                 "/dev/"DAA_DRV_STR


/******************************************************************** 
 * Enumerated Type Definitions											*
 ********************************************************************/

/* this is the list of the commands to the dsp and slic drivers */
typedef enum {

	ACG_DOWNLOAD_CMD_E,	/*used*/
	ACG_REINITIALIZE_DSP_CMD_E,
	ACG_RESET_BUF_DES_E,
	ACG_DECODE_MEDIA_CMD_E,	/*used*/
	ACG_GET_MEDIA_CMD_E,	/*used*/
	ACG_DSP_GET_EVENT_CMD,	/*used*/
	ACG_OPEN_CHANNEL_CMD_E,	/*used*/
	ACG_UPDATE_CHANNEL_CMD_E,	/*used*/
	ACG_PLAY_TONE_CMD_E,	/*used*/
	ACG_PLAY_TONE_SEQUENCE_CMD_E,	/*used*/
	ACG_PLAY_TONE_EXTENDED_CMD_E,
	ACG_STOP_TONE_CMD_E,	/*used*/
	ACG_PLAY_MWI_CMD_E,	/*used*/
	ACG_STOP_MWI_CMD_E,	/*used*/
	ACG_SET_3WAYCONF_CMD_E,	/*used */
	ACG_STOP_3WAYCONF_CMD_E,	/*used */
	ACG_CLOSE_CHANNEL_CMD_E,	/*used*/
	ACG_REINITIALIZE_ECHO_CANCELER_CMD_E,	/*used*/
	ACG_DEBUG_INFORMATION_ACTIVATION,	/*used*/
	ACG_START_FAX_RELAY_CMD_E,	/*used*/
	ACG_START_FAX_VBD_CMD_E,	/*used*/
	ACG_STOP_FAX_RELAY_CMD_E,	/*used*/
	ACG_STOP_FAX_VBD_CMD_E,	/*used*/
	ACG_ACTIVATE_RTP_CMD_E,	/*used*/
	ACG_DEACTIVATE_RTP_CMD_E,	/*used*/
    	ACG_MANIPULATE_TIMESLOTS_CMD_E,
   	ACG_SEND_CALLER_ID_CMD,	/*used*/
	ACG_STOP_CALLER_ID_CMD,	/*used*/
	ACG_RTP_UNBIND_CMD,	/*used*/
	ACG_SET_REGULAR_RTCP_PARAM_CMD,
	ACG_GET_REGULAR_RTCP_PARAM_CMD,	
	ACG_GET_VERSION_CMD,
	
	/*slic write cmd*/
	ACG_SLIC_INIT_CMD,	/*used*/
	ACG_SLIC_SET_LINE_STATE_CMD,	/*used*/
	ACG_SLIC_SET_TIME_SLOT_CMD,	/*used*/
	ACG_SLIC_START_RING_CMD,	/*used*/
	ACG_SLIC_STOP_RING_CMD,	/*used*/
	
	/*slic read cmd	*/
	ACG_SLIC_GET_TELEPHONY_EVENT_CMD,	/*used*/
    
  	/*daa write cmd*/
	ACG_DAA_INIT_CMD,	
	ACG_DAA_SEIZE_LINE_CMD,
	ACG_DAA_RELEASE_LINE_CMD,
        ACG_DAA_GENERATE_FLASH_CMD,
	
	/*daa read cmd	*/
	ACG_DAA_GET_TELEPHONY_EVENT_CMD,
	/* Ntp Timestamp*/
	ACG_SET_NTP_TIMESTAMP_CMD_E,

	/* AlexChen SDK upgrade */
	ACG_UPDATE_FXS_CADENCE_PROFILE,

	/*TDM playback cmd	*/
	ACG_PLAYBACK_TDM_START_CMD,  /*used*/
	ACG_PLAYBACK_NETWORK_START_CMD,  /*used*/
	ACG_PLAYBACK_PLAY_SILENCE_CMD,  /*used*/
	ACG_PLAYBACK_PLAY_VOICE_CMD,  /*used*/
	ACG_PLAYBACK_END_CMD,  /*used*/

    /*TimeSlot Manipulation cmd  */
	ACG_TIMESLOT_MANIPULATION_CMD,

}acgEDriversCommands;

/* This enumerator should not be modified!!!! */
typedef enum {

	ACG_VALIDATION_PLATFORM_GET_CMD,
	ACG_VALIDATION_CHIP_GET_CMD
    
}acgEValidationCommands;

/********************************************************************* 
 * Structure Definitions     												  *
 *********************************************************************/

typedef struct {

	unsigned int 					Command;
	int 							Channel;
	void				 			*data;
} acgTCommand;

#endif /*_ACGATEWAYDRIVERSDEFS_H */


