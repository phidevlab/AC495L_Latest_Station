/********************************************************************
 * acIPphoneAPI - AudioCodes IP phone API                 	*
 *               															*
 * API Function Implementation									    		*
 *                                                                      *
 * File Name:	acIPPhoneAPI.c                                          *
 *                                                                      *
 * Copyright (C) 2008 AudioCodes Ltd.                                   *
 *                                                                      *
 * Modifications :                                                      *
 *                                                                      *
 * 12/11/08 - Coding started.                                           *
 *																	    *
 ********************************************************************/


 #include "acIPPhoneAPI.h"
#include "acGatewayDriversDefs.h"
#include "acl_LCD_adaptation_layer.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

/*#define DBG_PRINT*/
#ifdef DBG_PRINT
#define ACIPP_Log_Print(a1,a2...)		printf("[%s:%d] " a1, __FUNCTION__, __LINE__, ##a2);
#else
#define ACIPP_Log_Print(a1,a2...)
#endif

#define ACIPP_Error_Print(a1,a2...)		{	printf("[%s:%d] " a1, __FUNCTION__, __LINE__, ##a2); \
								 	perror("Error message "); \
								}

extern int 	dspFd;	/* DSP file descriptor */


int acIPPStartRing()
{
	ACIPP_Log_Print(">\n");
	acIPPCommand RingStartCmd;
	int retValue=ACG_SUCCESS;
	RingStartCmd.Channel=0;
	RingStartCmd.Command=ACIPP_RING_START_CMD_E;

	acIPPSetAudioDevice(AC494_CODEC_SPEAKER);
	//in the StopRing function there is no need to change back because the off hook/speaker keys set the audio device

	if((retValue = (write(dspFd,(char *)&RingStartCmd,sizeof(acIPPCommand)))) != ACG_SUCCESS) {
		ACIPP_Error_Print("error at write ACG_RING_START_CMD\r\n");
		retValue = ACG_FAILURE;
	}

	ACIPP_Log_Print("<\n");
	return (retValue);


}

int acIPPStopRing()
{
	ACIPP_Log_Print(">\n");

	acIPPCommand StopRingCmd;
	int retValue=ACG_SUCCESS;
	StopRingCmd.Channel=0;
	StopRingCmd.Command=ACIPP_RING_STOP_CMD_E;

	if((retValue = (write(dspFd,(char *)&StopRingCmd,sizeof(acIPPCommand)))) != ACG_SUCCESS) {
		ACIPP_Error_Print("error at write ACG_RING_STOP_CMD\r\n");
		retValue = ACG_FAILURE;
	}
	ACIPP_Log_Print("<\n");
	return (retValue);
}

 int acIPPSetAudioDevice(acIPPCodecType IPPCodecType)
 {
 	ACIPP_Log_Print(">\n");

	acIPPCommand	setSpeakerCmd;
	int 				retValue = ACG_SUCCESS;

	memset(&setSpeakerCmd, 0, sizeof(acIPPCommand));

	setSpeakerCmd.Command = ACIPP_SET_SPK_STATUS_CMD_E;
	setSpeakerCmd.Channel = 0; //IPPhone

	setSpeakerCmd.data = (void *)&IPPCodecType;

	if((retValue = (write(dspFd,(char *)&setSpeakerCmd,sizeof(acIPPCommand)))) != ACG_SUCCESS) {
		ACIPP_Error_Print("error at write ACG_SET_SPEAKER_CMD\r\n");
		retValue = ACG_FAILURE;
	}
	ACIPP_Log_Print("<\n");
	return (retValue);

 }


 int acIPPSetAudioDevice_gw(int ch,acIPPCodecType IPPCodecType)
 {
 	ACIPP_Log_Print(">\n");

	acIPPCommand	setSpeakerCmd;
	int 				retValue = ACG_SUCCESS;

	memset(&setSpeakerCmd, 0, sizeof(acIPPCommand));

	setSpeakerCmd.Command = ACIPP_SET_SPK_STATUS_CMD_E;
	setSpeakerCmd.Channel = ch; //IPPhone

	setSpeakerCmd.data = (void *)&IPPCodecType;


	if((retValue = (write(dspFd,(char *)&setSpeakerCmd,sizeof(acIPPCommand)))) != ACG_SUCCESS) {
		ACIPP_Error_Print("error at write ACG_SET_SPEAKER_CMD gw\r\n");
		printf("ret value is : %d",retValue);
		retValue = ACG_FAILURE;
	}
	ACIPP_Log_Print("<\n");

	printf("---------------setSpeakerCmd.Command:%d,  setSpeakerCmd.Channel :%d,  setSpeakerCmd.data:%d,  retValue:%d\n"
	,setSpeakerCmd.Command,setSpeakerCmd.Channel,setSpeakerCmd.data,retValue);

	return (retValue);

 }


 int acIPPSetOutputGain(int ch,acgEVoiceGain volume)
 {
 	ACIPP_Log_Print(">\n");

 	acIPPCommand setOutputGainCmd;
	int retValue=ACG_SUCCESS;

	memset(&setOutputGainCmd,0,sizeof(acIPPCommand));
	setOutputGainCmd.Command=ACIPP_SET_OUTPUT_GAIN_CMD_E;
	setOutputGainCmd.Channel=ch;
	setOutputGainCmd.data =(void*)&volume;

	if((retValue = (write(dspFd,(char *)&setOutputGainCmd,sizeof(acIPPCommand)))) != ACG_SUCCESS) {
		ACIPP_Error_Print("error at write ACG_SET_OUTPUT_GAIN_CMD\r\n");
		retValue = ACG_FAILURE;
	}
	ACIPP_Log_Print("<\n");
	return retValue;


 }


  int acIPPSetInputGain(acgEVoiceGain volume)
 {
 	ACIPP_Log_Print(">\n");

 	acIPPCommand setInputGainCmd;
	int retValue=ACG_SUCCESS;

	memset(&setInputGainCmd,0,sizeof(acIPPCommand));
	setInputGainCmd.Command=ACIPP_SET_INPUT_GAIN_CMD_E;
	setInputGainCmd.Channel=0;
	setInputGainCmd.data =(void*)&volume;

	if((retValue = (write(dspFd,(char *)&setInputGainCmd,sizeof(acIPPCommand)))) != ACG_SUCCESS) {
		ACIPP_Error_Print("error at write ACG_SET_INPUT_GAIN_CMD\r\n");
		retValue = ACG_FAILURE;
	}
	ACIPP_Log_Print("<\n");
	return retValue;
 }
#ifndef GW_APPL_SDP
  int acIPPShowVolume(int volume,int max_volume)
  {
#ifndef CONFIG_GTEK
  	return LCDDisplayVolume(volume,max_volume);
#endif

	return 0;
  }
#endif



