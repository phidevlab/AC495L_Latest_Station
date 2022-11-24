/********************************************************************
 * AudioCodes improved application 	                 				*
 *																	*
 * File Name:	acl_dsp_telephony_commands.h						*
 *																	*
 * Copyright (C) 2008 AudioCodes Ltd.								*
 *																	*
 * Modifications :													*
 *																	*
 * 31/03/08 - Coding started.										*
 *																	*
 ********************************************************************/

#ifndef _NEW_PHONE_API_H_
#define _NEW_PHONE_API_H_

#include "acl_voip_conf.h"

/******************************************************************************
*	Function:	 dsp_init
*
*	Description: Initialize the DSP .
*
*	Parameters:	 [I] regional_settings_params_t* p_regional_settings_params
*				 [I] voip_dsp_advanced_channel_configuration_t* p_voip_dsp_advanced_channel_configuration
*
*	Returns: 	case of error -1 else 0
*
*******************************************************************************/
void dsp_init(regional_settings_params_t* p_regional_settings_params, 
			voip_dsp_advanced_channel_configuration_t* p_voip_dsp_advanced_channel_configuration);
 
/******************************************************************************
*	Function:	 slic_init
*
*	Description: Initialize the SLIC .
*
*	Parameters:	 [I] 
*
*	Returns: 	case of error -1 else 0
*
*******************************************************************************/
void slic_init();

/******************************************************************************
*	Function:	 slac_init
*
*	Description: Initialize the SLAC .
*
*	Parameters:	 [I] 
*
*	Returns: 	case of error -1 else 0
*
*******************************************************************************/

void slac_init();

/******************************************************************************
*	Function:	 dsp_uninit
*
*	Description: Un Initialize the DSP .
*
*	Parameters:	 
*
*	Returns: 	case of error -1 else 0
*
*******************************************************************************/
void dsp_uninit();

/******************************************************************************
*	Function:	 slic_uninit
*
*	Description: Un Initialize the SLIC .
*
*	Parameters:	 
*
*	Returns: 	case of error -1 else 0
*
*******************************************************************************/
void slic_uninit();


/******************************************************************************
*	Function:	 slac_uninit
*
*	Description: Un Initialize the SLAC .
*
*	Parameters:	 
*
*	Returns: 	case of error -1 else 0
*
*******************************************************************************/
void slac_uninit();

/******************************************************************************
*	Function:	 dsp_voice_start
*
*	Description: 
*
*	Parameters:	 
*
*	Returns: 	case of error -1 else 0
*
*******************************************************************************/
int dsp_voice_start(int n, int ch, int payload, int ptime, int dtmf_inband, 
					unsigned int rtp_context, int faxMethod);


/******************************************************************************
*	Function:	 dsp_voice_update
*
*	Description: 
*
*	Parameters:	 
*
*	Returns: 	case of error -1 else 0
*
*******************************************************************************/
int dsp_voice_update(int n, int ch, int payload, int ptime, int dtmf_inband,
					u32 rtp_context, int faxMethod);



/******************************************************************************
*	Function:	 dsp_voice_stop
*
*	Description: 
*
*	Parameters:	 
*
*	Returns: 	case of error -1 else 0
*
*******************************************************************************/
void dsp_voice_stop(int n, int ch);

/******************************************************************************
*	Function:	 dsp_3wc
*
*	Description: 	setting 3 way conference parameter
*
*	Parameters:	 
*
*	Returns: 	case of error -1 else 0
*
*******************************************************************************/
int dsp_3wc(int n, int confChannel, int mateChannel, int confMode);

/******************************************************************************
*	Function:	 dsp_fax_start
*
*	Description: 	starting fax T.38
*
*	Parameters:	 
*
*	Returns: 	
*
*******************************************************************************/
void dsp_fax_start(int n, int ch,  fax_transport_mode_t fax_transport_mode, int fax_maxrate);

/******************************************************************************
*	Function:	 dsp_fax_stop
*
*	Description: 	stoping fax
*
*	Parameters:	 
*
*	Returns: 	
*
*******************************************************************************/
void dsp_fax_stop(int n, int ch, fax_transport_mode_t fax_transport_mode);

/******************************************************************************
*	Function:	 dsp_ibs_stop
*
*	Description: 	stop dtmf generation
*
*	Parameters:	 
*
*	Returns: 	
*
*******************************************************************************/
void dsp_ibs_stop(int n, int ch);

/******************************************************************************
*	Function:	 dsp_vmwi
*
*	Description: 	Play MWI
*
*	Parameters:	 
*
*	Returns: 	
*
*******************************************************************************/
void dsp_vmwi(int n, int activate);

/******************************************************************************
*	Function:	 dsp_play
*
*	Description: 	Play a tone towards the local TDM
*
*	Parameters:	 
*
*	Returns: 	
*
*******************************************************************************/
void dsp_play(int n, voip_signal_t s, int start);

/******************************************************************************
*	Function:	 dsp_ring
*
*	Description: 	Play a tone towards the local TDM and set caller id timer
*
*	Parameters:	 
*
*	Returns: 	
*
*******************************************************************************/
void dsp_ring(int n, int start);

/******************************************************************************
*	Function:	 dsp_call_waiting_alert
*
*	Description: 	Play caller id type 2 towards local TDM
*
*	Parameters:	 
*
*	Returns: 	
*
*******************************************************************************/
void dsp_call_waiting_alert(int n);

/******************************************************************************
*	Function:	 dsp_dtmf
*
*	Description: 	Play DTMF towards the local TDM
*
*	Parameters:	 
*
*	Returns: 	
*
*******************************************************************************/
void dsp_dtmf(int n, int key, int pressed, int dur) ;



int acgPlayTone_phi(int ch, int tone_index,int flag,int duration);

/************************************/
/***********  Stub Functions  ***********/
/************************************/

int dsp_send_daa_commands(void* data);

int dsp_adaptation_layer_send_general_command(void *data);

int dsp_msg_log(sip_log_message_t *logmsg);

int keypad_open(int n);

int keypad_close(int n);

void dsp_get_info(int line, voip_state_t *state);

 
#endif /* _NEW_PHONE_API_H_ */

