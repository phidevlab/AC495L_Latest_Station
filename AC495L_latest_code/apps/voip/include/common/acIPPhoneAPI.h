/******************************************************************** 
 * acGatewayAPI - AudioCodes Analog VoIP Gateway DSP and SLIC API                  	* 
 *                                                                      *
 * API Function Prototypes                                              * 	
 *                                                                      *
 * File Name:	acIPPhoneAPI.h                                          *
 *                                                                      *
 * Copyright (C) 2008 AudioCodes Ltd.                                   *
 *                                                                      *
 * Modifications :                                                      *
 *                                                                      *
 * 12/11/08 - Coding started.                                           *
 *                                                                      *
 ********************************************************************/

#ifndef	ACIPPHONEAPI_H
#define	ACIPPHONEAPI_H

#include "dsp_drv.h"
#include "acIPPDriversDefs.h"
#ifndef GW_APPL_SDP
#include "acl_keypad_adaptation_layer.h"
#include "acl_dsp_telephony_events.h"
#endif
 int acIPPSetAudioDevice(acIPPCodecType IPPCodecType);

int acIPPStartRing();

int acIPPStopRing();

 int acIPPSetOutputGain(int ch,acgEVoiceGain volume);

 int acIPPSetInputGain(acgEVoiceGain volume);
#ifndef GW_APPL_SDP
  int acIPPShowVolume(int volume,int max_volume);
#endif

#endif

