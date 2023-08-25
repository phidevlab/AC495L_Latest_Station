/********************************************************************
 * acGatewayAPI - AudioCodes Analog VoIP Gateway DSP and SLIC API                  	*
 *               															*
 * API Function Implementation									    		*
 *                                                                      *
 * File Name:	acGatewayAPI.c                                          *
 *                                                                      *
 * Copyright (C) 2007 AudioCodes Ltd.                                   *
 *                                                                      *
 * Modifications :                                                      *
 *                                                                      *
 * 30/12/07 - Coding started.                                           *
 *																	    *
 ********************************************************************/



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <fcntl.h>


#include "acGatewayAPI.h"


//#define ACG_Log_Print(a1,a2...)		printf("[%s:%d] " a1, __FUNCTION__, __LINE__, ##a2);
#define ACG_Log_Print(a1,a2...)

#define ACG_Error_Print(a1,a2...)		{	printf("[%s:%d] " a1, __FUNCTION__, __LINE__, ##a2); \
								 	perror("Error message "); \
								}

/*** Global structure for holding acGateway default parameters ***/
static acgTChannelParameters 	gChannelParameters;

int 							dspFd = -1;	        /* DSP file descriptor */
int 							slicFd = -1;	    /* SLIC file descriptor */
int                        		daaFd = -1;         /* DAA file descriptor */

/*ACL AllenZ 090531 added for wave tone playback*/
static int 	ringingToneFd[ACG_NUMBER_OF_DSP_CHANNEL];   /*fd for tdm playback file*/
static int	msec2bytes = 10;
static int 	g_ring_request_count[ACG_NUMBER_OF_DSP_CHANNEL] = {0};
static int  voice_data_size[ACG_NUMBER_OF_DSP_CHANNEL] = {0};
/*** acGatewayAPI prototypes declaration ***/
static void acgFillDefaultParameters(acgTChannelParameters *pParams);


/****************************************************************
********												  ********
********	DSP Channel Configuration and Control API Functions	  ********
********												  ********
*****************************************************************/

/******************************************************************************
*	Function:	 acgGetDefaultInitDeviceParameters
*
*	Description: Gets the default values to the DSP initialization structure
*
*	Parameters:
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgGetDefaultInitDeviceParameters(acgTInitializationParameters *pInitializationParams)
{

	int 				retValue = ACG_SUCCESS;

	ACG_Log_Print(">>\r\n");

	pInitializationParams->NumberOfCallProgressTones = 9;

                /* Dial Tone (ANSI TI.401-1993) */
	pInitializationParams->CallProgressTones[0].ToneAFrequency                 = 350;
	pInitializationParams->CallProgressTones[0].ToneB_OrAmpModulationFrequency = 440;
	pInitializationParams->CallProgressTones[0].Type							= ACG_CALL_PROGRESS_SIGNAL_TYPE__CONTINUOUS;
	pInitializationParams->CallProgressTones[0].ToneALevel                     = 13;
	pInitializationParams->CallProgressTones[0].ToneBLevel                     = 13;
	pInitializationParams->CallProgressTones[0].DetectionTimeOrCadenceFirstOnOrBurstDuration = 300; /* 3 sec */
                /* Dial Tone (ITU-T Q.35) */
	pInitializationParams->CallProgressTones[1].ToneAFrequency                 = 425;
	pInitializationParams->CallProgressTones[1].ToneB_OrAmpModulationFrequency = 0;
	pInitializationParams->CallProgressTones[1].Type							= ACG_CALL_PROGRESS_SIGNAL_TYPE__CONTINUOUS;
	pInitializationParams->CallProgressTones[1].ToneALevel                     = 10;
	pInitializationParams->CallProgressTones[1].ToneBLevel                     = 0;
	pInitializationParams->CallProgressTones[1].DetectionTimeOrCadenceFirstOnOrBurstDuration = 300; /* 3 sec */
	            /* Audible Ringing Tone (ANSI TI.401-1993) */
	pInitializationParams->CallProgressTones[2].ToneAFrequency                 = 440;
	pInitializationParams->CallProgressTones[2].ToneB_OrAmpModulationFrequency = 480;
	pInitializationParams->CallProgressTones[2].Type							= ACG_CALL_PROGRESS_SIGNAL_TYPE__CADENCE;
	pInitializationParams->CallProgressTones[2].ToneALevel                     = 19;
	pInitializationParams->CallProgressTones[2].ToneBLevel                     = 19;
	pInitializationParams->CallProgressTones[2].DetectionTimeOrCadenceFirstOnOrBurstDuration  = 200;
	pInitializationParams->CallProgressTones[2].CadenceFirstOffDuration        = 400;
 	            /* Busy Tone (ANSI TI.401-1993) */
	pInitializationParams->CallProgressTones[3].ToneAFrequency                 = 480;
	pInitializationParams->CallProgressTones[3].ToneB_OrAmpModulationFrequency = 620;
	pInitializationParams->CallProgressTones[3].Type							= ACG_CALL_PROGRESS_SIGNAL_TYPE__CADENCE;
	pInitializationParams->CallProgressTones[3].ToneALevel                     = 24;
	pInitializationParams->CallProgressTones[3].ToneBLevel                     = 24;
	pInitializationParams->CallProgressTones[3].DetectionTimeOrCadenceFirstOnOrBurstDuration  = 50;
	pInitializationParams->CallProgressTones[3].CadenceFirstOffDuration        = 50;
	            /* Reorder (ANSI TI.401-1993) */
	pInitializationParams->CallProgressTones[4].ToneAFrequency                 = 480;
	pInitializationParams->CallProgressTones[4].ToneB_OrAmpModulationFrequency = 620;
	pInitializationParams->CallProgressTones[4].Type							= ACG_CALL_PROGRESS_SIGNAL_TYPE__CADENCE;
	pInitializationParams->CallProgressTones[4].ToneALevel                     = 24;
	pInitializationParams->CallProgressTones[4].ToneBLevel                     = 24;
	pInitializationParams->CallProgressTones[4].DetectionTimeOrCadenceFirstOnOrBurstDuration  = 25;
	pInitializationParams->CallProgressTones[4].CadenceFirstOffDuration        = 25;
	            /* Audible Ringer Tone */
	pInitializationParams->CallProgressTones[5].ToneAFrequency                 = 400;
	pInitializationParams->CallProgressTones[5].ToneB_OrAmpModulationFrequency = 0;
	pInitializationParams->CallProgressTones[5].Type							= ACG_CALL_PROGRESS_SIGNAL_TYPE__CADENCE;
	pInitializationParams->CallProgressTones[5].ToneALevel                     = 16;
	pInitializationParams->CallProgressTones[5].ToneBLevel                     = 0;
	pInitializationParams->CallProgressTones[5].DetectionTimeOrCadenceFirstOnOrBurstDuration  = 100;
	pInitializationParams->CallProgressTones[5].CadenceFirstOffDuration        = 300;
 	            /* Dial Tone */
	pInitializationParams->CallProgressTones[6].ToneAFrequency                 = 440;
	pInitializationParams->CallProgressTones[6].ToneB_OrAmpModulationFrequency = 0;
	pInitializationParams->CallProgressTones[6].Type							= ACG_CALL_PROGRESS_SIGNAL_TYPE__CONTINUOUS;
	pInitializationParams->CallProgressTones[6].ToneALevel                     = 10;
	pInitializationParams->CallProgressTones[6].ToneBLevel                     = 0;
	pInitializationParams->CallProgressTones[6].DetectionTimeOrCadenceFirstOnOrBurstDuration = 300; /* 3 sec */
	            /* Fast Busy Tone(T1.401-1993) */
	pInitializationParams->CallProgressTones[7].ToneAFrequency                 = 440;
	pInitializationParams->CallProgressTones[7].ToneB_OrAmpModulationFrequency = 0;
	pInitializationParams->CallProgressTones[7].Type							= ACG_CALL_PROGRESS_SIGNAL_TYPE__CADENCE;
	pInitializationParams->CallProgressTones[7].ToneALevel                     = 24;
	pInitializationParams->CallProgressTones[7].ToneBLevel                     = 0;
	pInitializationParams->CallProgressTones[7].DetectionTimeOrCadenceFirstOnOrBurstDuration  = 24;
	pInitializationParams->CallProgressTones[7].CadenceFirstOffDuration        = 24;
	            /* Busy Tone */
	pInitializationParams->CallProgressTones[8].ToneAFrequency                 = 440;
	pInitializationParams->CallProgressTones[8].ToneB_OrAmpModulationFrequency = 0;
	pInitializationParams->CallProgressTones[8].Type							= ACG_CALL_PROGRESS_SIGNAL_TYPE__CADENCE;
	pInitializationParams->CallProgressTones[8].ToneALevel                     = 24;
	pInitializationParams->CallProgressTones[8].ToneBLevel                     = 0;
	pInitializationParams->CallProgressTones[8].DetectionTimeOrCadenceFirstOnOrBurstDuration  = 50;
	pInitializationParams->CallProgressTones[8].CadenceFirstOffDuration        = 50;

	strcpy(pInitializationParams->RemoteRecordingIpAddress,"1.1.1.1");
	pInitializationParams->RemoteRecordingPort = 8888;

	pInitializationParams->PrimaryDSPDeviceInformation.pProgram = 0;
	pInitializationParams->PrimaryDSPDeviceInformation.ProgramSize = 0;
	pInitializationParams->PrimaryDSPDeviceInformation.NumberOfDevices = 0;

	pInitializationParams->SecondaryDSPDeviceInformation.pProgram = 0;
	pInitializationParams->SecondaryDSPDeviceInformation.ProgramSize = 0;
	pInitializationParams->SecondaryDSPDeviceInformation.NumberOfDevices = 0;

	/*Initial advanced channel parameters*/
	pInitializationParams->AdvancedChannelParameters.FaxModemJitter = 0;
	pInitializationParams->AdvancedChannelParameters.BypassJitter = 150; /*ACL A.L - modify the default value from 35;*/
	pInitializationParams->AdvancedChannelParameters.DataBypassOutputGain = ACG_FAX_AND_DATA_GAIN__DEFAULT_0_DB;
	pInitializationParams->AdvancedChannelParameters.FaxBypassOutputGain = ACG_FAX_AND_DATA_GAIN__DEFAULT_0_DB;

	pInitializationParams->DspVersionTemplateNumber = 0;

	ACG_Log_Print("<<\r\n");

	return (retValue);

} /* end of acgGetDefaultInitDeviceParameters() */
/******************************************************************************/



/******************************************************************************
*	Function:	 acgInitializeDSP
*
*	Description: Initialize the DSP.
*
*	Parameters:	 [I] acgTInitializationParameters *pInitializationParams
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgInitializeDSP(acgTInitializationParameters *pInitializationParams)
{
	acgTCommand 	     initDspCmd;
	int 				retValue = ACG_SUCCESS;

	ACG_Log_Print(">>\r\n");

	if(dspFd == -1)
	{
		printf("acGatewayAPI version is: %s\n", ACG_VERSION);

		if((dspFd = open(ACL_DSP_DRV_FD_NAME_STR, O_RDWR)) == ACG_FAILURE)
		{
			ACG_Error_Print("can't open dsp device\r\n");
			return (ACG_FAILURE);
		}
	}
printf("dspFd =%d\n",dspFd);

	memset(&initDspCmd, 0, sizeof(acgTCommand));

	initDspCmd.Command = ACG_DOWNLOAD_CMD_E;

	initDspCmd.data = (void *)pInitializationParams;

	if((retValue = (write(dspFd,(char *)&initDspCmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		ACG_Error_Print("error at write ACG_DOWNLOAD_CMD_E\r\n");
		retValue = ACG_FAILURE;
	}

	/* Initialize global channel parameters DB */
	acgFillDefaultParameters(&gChannelParameters);

	ACG_Log_Print("<<\r\n");

	return (retValue);
}

/******************************************************************************
*	Function:	 acgReInitializeDSP
*
*	Description: ReInitialize the DSP.
*
*	Parameters:	 [I] acgTInitializationParameters *pInitializationParams
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgReInitializeDSP(acgTInitializationParameters *pInitializationParams)
{
	acgTCommand 	initDspCmd;
	int 				retValue = ACG_SUCCESS;

	ACG_Log_Print(">>\r\n");

	printf("acGatewayAPI version is: %s\n", ACG_VERSION);

	if((dspFd = open(ACL_DSP_DRV_FD_NAME_STR, O_RDWR)) == ACG_FAILURE)
	{
		ACG_Error_Print("can't open dsp device\r\n");
		return (ACG_FAILURE);
	}


	memset(&initDspCmd, 0, sizeof(acgTCommand));

	initDspCmd.Command = ACG_REINITIALIZE_DSP_CMD_E;

	initDspCmd.data = (void *)pInitializationParams;

    initDspCmd.Channel = 0;
	if((retValue = (write(dspFd,(char *)&initDspCmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		ACG_Error_Print("error at write ACG_REINITIALIZE_DSP_CMD_E\r\n");
		retValue = ACG_FAILURE;
	}

	ACG_Log_Print("<<\r\n");

	return (retValue);
}

/******************************************************************************
*	Function:	 acgCloseDSP
*
*	Description: Closing the DSP.
*
*	Parameters:
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgCloseDSP()
{
	int 	retValue = ACG_SUCCESS;

	ACG_Log_Print(">>\r\n");

	retValue = close(dspFd);

	dspFd = -1;

	ACG_Log_Print("<<\r\n");

	return (retValue);
}

/******************************************************************************
*	Function:	 acgGetChannelParameters
*
*	Description: Get the current channel configuration parameters.
*
*	Parameters:	 [O] acgTChannelParameters *pParams
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgGetDefaultChannelParameters(acgTChannelParameters *pParams)
{
	int retValue = ACG_SUCCESS;

	ACG_Log_Print(">>\r\n");

	memcpy(pParams, &gChannelParameters, sizeof(acgTChannelParameters));

	ACG_Log_Print("<<\r\n");

	return (retValue);
}

/******************************************************************************
*	Function:	 acgOpenChannel
*
*	Description: Open a DSP channel.
*
*	Parameters:	 [I] int Channel
*				 [I] acgTChannelParameters *pParams
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgOpenChannel(int Channel, acgTChannelParameters *pParams)
{
	acgTCommand		openChannelCmd;
	int 				retValue = ACG_SUCCESS;

	ACG_Log_Print(">>\r\n");

	memset(&openChannelCmd, 0, sizeof(acgTCommand));

	openChannelCmd.Command = ACG_OPEN_CHANNEL_CMD_E;
	openChannelCmd.Channel = Channel;

	openChannelCmd.data = (void *)pParams;

	if((retValue = (write(dspFd,(char *)&openChannelCmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		ACG_Error_Print("error at write ACG_OPEN_CHANNEL_CMD_E\r\n");
		retValue = ACG_FAILURE;
	}

    printf("channel opened %d \n",Channel);
	/*if(Channel==0)
	{
        acIPPSetAudioDevice_gw(0,AC494_CODEC_SPEAKER);
        printf("channel 1 is opend\n");

	}
	else{
        acIPPSetAudioDevice_gw(0,AC494_CODEC_HANDSET);
         printf("channel 0 or 2 is opend\n");
	}*/


	ACG_Log_Print("<<\r\n");

	return (retValue);
}

/******************************************************************************
*	Function:	 acgUpdateChannel
*
*	Description: Update a DSP channel.
*
*	Parameters:	 [I] int Channel
*				 [I] acgTChannelParameters *pParams
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgUpdateChannel(int Channel, acgTChannelParameters *pParams)
{
	acgTCommand		updateChannelCmd;
	int 				retValue = ACG_SUCCESS;

	ACG_Log_Print(">>\r\n");

	memset(&updateChannelCmd, 0, sizeof(acgTCommand));

	updateChannelCmd.Command = ACG_UPDATE_CHANNEL_CMD_E;
	updateChannelCmd.Channel = Channel;

	updateChannelCmd.data = (void *)pParams;

	if((retValue = (write(dspFd,(char *)&updateChannelCmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		ACG_Error_Print("error at write ACG_UPDATE_CHANNEL_CMD_E\r\n");
		retValue = ACG_FAILURE;
	}

	ACG_Log_Print("<<\r\n");

	return (retValue);
}



int Initialize_ssrc(int stn_no)
{

    printf("Writing SSRC \n");
    acgTChannelParameters *pParams;
    acgTCommand		updateChannelCmd;
    int retValue = ACG_SUCCESS;
    memset(&updateChannelCmd, 0, sizeof(acgTCommand));


    updateChannelCmd.Command = ACG_START_FAX_VBD_CMD_E;

    updateChannelCmd.Channel = stn_no;

    updateChannelCmd.data = (void *)pParams;
    printf("<<\r\n");
    if((retValue = (write(dspFd,(char *)&updateChannelCmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		perror("error at write ssrc \r\n");
		retValue = ACG_FAILURE;
	}

//	printf("<<\r\n");

	return (retValue);


}

/******************************************************************************
*	Function:	 acgCloseChannel
*
*	Description: Close a DSP channel.
*
*	Parameters:	 [I] int Channel
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgCloseChannel(int Channel)
{
	acgTCommand		closeChannelCmd;
	int 				retValue = ACG_FAILURE;

	ACG_Log_Print(">>\r\n");

	memset(&closeChannelCmd, 0, sizeof(acgTCommand));

	closeChannelCmd.Command = ACG_CLOSE_CHANNEL_CMD_E;
	closeChannelCmd.Channel = Channel;

	/* For AC48x Drivers - Don't close channel if a DTMF Activity or IBS Event are in progress */
	while(retValue == ACG_FAILURE)
	{
		retValue = write(dspFd,(char *)&closeChannelCmd,sizeof(acgTCommand));
	}

	retValue = ACG_SUCCESS;

	ACG_Log_Print("<<\r\n");

	return (retValue);
}

/******************************************************************************
*	Function:	 acgActivateRTP
*
*	Description: Activate the DSP channel RTP stream.
*
*	Parameters:	 [I] int Channel
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgActivateRTP(int Channel)
{
	acgTCommand		activateRTPCmd;
	int 				retValue = ACG_SUCCESS;

	ACG_Log_Print(">>\r\n");

	memset(&activateRTPCmd, 0, sizeof(acgTCommand));

	activateRTPCmd.Command = ACG_ACTIVATE_RTP_CMD_E;
	activateRTPCmd.Channel = Channel;
	printf("activateRTPCmd.Command:%d, activateRTPCmd.Channel:%d\n ",activateRTPCmd.Command,activateRTPCmd.Channel);

	if((retValue = (write(dspFd,(char *)&activateRTPCmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		ACG_Error_Print("error at write ACG_ACTIVATE_RTP_CMD_E\r\n");
		retValue = ACG_FAILURE;
	}

	ACG_Log_Print("<<\r\n");

	return (retValue);
}

/******************************************************************************
*	Function:	 acgDeactivateRTP
*
*	Description: Deactivates (closes) the DSP channel RTP stream.
*
*	Parameters:	 [I] int Channel
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgDeactivateRTP(int Channel)
{
	acgTCommand		deactivateRTPCmd;
	int 				retValue = ACG_FAILURE;

	ACG_Log_Print(">>\r\n");

	memset(&deactivateRTPCmd, 0, sizeof(acgTCommand));

	deactivateRTPCmd.Command = ACG_DEACTIVATE_RTP_CMD_E;
	deactivateRTPCmd.Channel = Channel;

	/* For AC48x Drivers - Don't deactivate RTP  if a DTMF Activity or IBS Event are in progress */
	while(retValue == ACG_FAILURE)
	{
		retValue = write(dspFd,(char *)&deactivateRTPCmd,sizeof(acgTCommand));
	}
	printf("--------------retValue:%d\n",retValue);

	retValue = ACG_SUCCESS;
	printf("++++++++++retValue:%d\n",retValue);

	ACG_Log_Print("<<\r\n");

	return (retValue);
}


/****************************************************************
********												  ********
********		DSP Signal Generation API Functions			  ********
********												  ********
*****************************************************************/



/******************************************************************************
*	Function:	 acgPlayTone
*
*	Description: Play a tone towards the local TDM side or network side.
*
*	Parameters:	 [I] int Channel
*				 [I] acgTTone *pTone
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgPlayTone(int Channel, acgTTone *pTone)
{
	acgTCommand		playToneCmd;
	int 				retValue = ACG_SUCCESS;

	ACG_Log_Print(">>\r\n");

	memset(&playToneCmd, 0, sizeof(acgTCommand));

	playToneCmd.Command = ACG_PLAY_TONE_CMD_E;
	playToneCmd.Channel = Channel;

	playToneCmd.data = (void *)pTone;

	if((retValue = (write(dspFd,(char *)&playToneCmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		ACG_Error_Print("error at write ACG_PLAY_TONE_CMD_E\r\n");
		retValue = ACG_FAILURE;
	}

	ACG_Log_Print("<<\r\n");

	return (retValue);
}

/******************************************************************************
*	Function:	 acgPlayToneSequence
*
*	Description: Play a sequence of tones towards the local TDM side or network side.
*
*	Parameters:	 [I] int Channel
*				 [I] acgTToneSequence *pTones
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgPlayToneSequence(int Channel, acgTToneSequence *pTones)
{
	acgTCommand		playToneSequenceCmd;
	int 				retValue = ACG_SUCCESS;

	ACG_Log_Print(">>\r\n");

	memset(&playToneSequenceCmd, 0, sizeof(acgTCommand));

	playToneSequenceCmd.Command = ACG_PLAY_TONE_SEQUENCE_CMD_E;
	playToneSequenceCmd.Channel = Channel;

	playToneSequenceCmd.data = (void *)pTones;

	if((retValue = (write(dspFd,(char *)&playToneSequenceCmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		ACG_Error_Print("error at write ACG_PLAY_TONE_SEQUENCE_CMD_E\r\n");
		retValue = ACG_FAILURE;
	}

	ACG_Log_Print("<<\r\n");

	return (retValue);
}

/******************************************************************************
*	Function:	 acgPlayToneExtended
*
*	Description: Play a user defined combined tones towards the local TDM side or network side.
*
*	Parameters:	 [I] int Channel
*				 [I] acgTToneExtended *pTones
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/

int acgPlayToneExtended(int Channel, acgTToneExtended *pTones)
{
	acgTCommand		playToneExtendedCmd;
	int 				retValue = ACG_SUCCESS;

	ACG_Log_Print(">>\r\n");

	memset(&playToneExtendedCmd, 0, sizeof(acgTCommand));

	playToneExtendedCmd.Command = ACG_PLAY_TONE_EXTENDED_CMD_E;
	playToneExtendedCmd.Channel = Channel;

	playToneExtendedCmd.data = (void *)pTones;

	if((retValue = (write(dspFd,(char *)&playToneExtendedCmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		ACG_Error_Print("error at write ACG_PLAY_TONE_SEQUENCE_CMD_E\r\n");
		retValue = ACG_FAILURE;
	}

	ACG_Log_Print("<<\r\n");

	return (retValue);
}

/******************************************************************************
*	Function:	 acgStopTone
*
*	Description: Stop a tone if already been generated towards the local TDM side or network side.
*
*	Parameters:	 [I] int Channel
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgStopTone(int Channel)
{
	acgTCommand		stopToneCmd;
	int 				retValue = ACG_SUCCESS;

	ACG_Log_Print(">>\r\n");

	memset(&stopToneCmd, 0, sizeof(acgTCommand));

	stopToneCmd.Command = ACG_STOP_TONE_CMD_E;
	stopToneCmd.Channel = Channel;

	if((retValue = (write(dspFd,(char *)&stopToneCmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		ACG_Error_Print("error at write ACG_STOP_TONE_CMD_E\r\n");
		retValue = ACG_FAILURE;
	}

	ACG_Log_Print("<<\r\n");

	return (retValue);
}

/******************************************************************************
*	Function:	 acgPlayMWI
*
*	Description: Play MWI.
*
*	Parameters:	 [I] int Channel
*				 [I] acgECallerIdServiceType	CallerIdServiceType
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgPlayMWI(int Channel, acgECallerIdServiceType CallerIdServiceType)
{
	acgTCommand		playMWICmd;
	acgTMWI 			MWI;
	int 				retValue = ACG_SUCCESS;

	ACG_Log_Print(">>\r\n");

	memset(&playMWICmd, 0, sizeof(acgTCommand));
	memset(&MWI, 0, sizeof(acgTMWI));


	playMWICmd.Command = ACG_PLAY_MWI_CMD_E;
	playMWICmd.Channel = Channel;

	MWI.Activate = ACG_CONTROL__ENABLE;
	MWI.CallerIdServiceType = CallerIdServiceType;

	playMWICmd.data = (void *)&MWI;

	if((retValue = (write(dspFd,(char *)&playMWICmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		ACG_Error_Print("error at write ACG_PLAY_MWI_CMD_E\r\n");
		retValue = ACG_FAILURE;
	}

	ACG_Log_Print("<<\r\n");

	return (retValue);
}

/******************************************************************************
*	Function:	 acgStopMWI
*
*	Description: Stop MWI
*
*	Parameters:	 [I] int Channel
*				 [I] acgECallerIdServiceType	CallerIdServiceType
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgStopMWI(int Channel, acgECallerIdServiceType CallerIdServiceType)
{
	acgTCommand		stopMWICmd;
	acgTMWI 			MWI;
	int 				retValue = ACG_SUCCESS;

	ACG_Log_Print(">>\r\n");

	memset(&stopMWICmd, 0, sizeof(acgTCommand));
	memset(&MWI, 0, sizeof(acgTMWI));

	stopMWICmd.Command = ACG_STOP_MWI_CMD_E;
	stopMWICmd.Channel = Channel;

	MWI.Activate = ACG_CONTROL__DISABLE;
	MWI.CallerIdServiceType = CallerIdServiceType;

	stopMWICmd.data = (void *)&MWI;

	if((retValue = (write(dspFd,(char *)&stopMWICmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		ACG_Error_Print("error at write ACG_STOP_MWI_CMD_E\r\n");
		retValue = ACG_FAILURE;
	}

	ACG_Log_Print("<<\r\n");

	return (retValue);
}


/******************************************************************************
*	Function:	 acgPlayCallerID
*
*	Description: Generate caller Id towards the local TDM.
*
*	Parameters:	 [I] int Channel
*				 [I] acgTCallerId *pCallerId
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgPlayCallerID(int Channel, acgTCallerId *pCallerId)
{
	acgTCommand		playCallerIdCmd;
	int 				retValue = ACG_SUCCESS;

	ACG_Log_Print(">>\r\n");

	memset(&playCallerIdCmd, 0, sizeof(acgTCommand));

	playCallerIdCmd.Command = ACG_SEND_CALLER_ID_CMD;
	playCallerIdCmd.Channel = Channel;

	playCallerIdCmd.data = (void *)pCallerId;

	if((retValue = (write(dspFd,(char *)&playCallerIdCmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		ACG_Error_Print("error at write ACG_SEND_CALLER_ID_CMD\r\n");
		retValue = ACG_FAILURE;
	}

	ACG_Log_Print("<<\r\n");

	return (retValue);
}

/******************************************************************************
*	Function:	 acgStopCallerID
*
*	Description: Stop caller Id generation.
*
*	Parameters:	 [I] int Channel
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgStopCallerID(int Channel)
{
	acgTCommand		stopCallerIdCmd;
	int 				retValue = ACG_SUCCESS;

	ACG_Log_Print(">>\r\n");

	memset(&stopCallerIdCmd, 0, sizeof(acgTCommand));

	stopCallerIdCmd.Command = ACG_STOP_CALLER_ID_CMD;
	stopCallerIdCmd.Channel = Channel;

	if((retValue = (write(dspFd,(char *)&stopCallerIdCmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		ACG_Error_Print("error at write ACG_STOP_CALLER_ID_CMD\r\n");
		retValue = ACG_FAILURE;
	}

	ACG_Log_Print("<<\r\n");

	return (retValue);
}

/******************************************************************************
*	Function:	 acgStart3WayConference
*
*	Description: Start 3 way conference.
*
*	Parameters:	 [I] int Channel
*				 [I] int ConferenceChannelMate
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgStart3WayConference(int Channel, int ConferenceChannelMate)
{
	acgTCommand			start3WayConferenceCmd;
	acgT3WayConference	_3WayConference;

	int 				retValue = ACG_SUCCESS;

	ACG_Log_Print(">>\r\n");

	memset(&start3WayConferenceCmd, 0, sizeof(acgTCommand));
	memset(&_3WayConference, 0, sizeof(acgT3WayConference));

	start3WayConferenceCmd.Command = ACG_SET_3WAYCONF_CMD_E;
	start3WayConferenceCmd.Channel = Channel;

	_3WayConference.ConferenceChannelMate = ConferenceChannelMate;
	_3WayConference.Mode = ACG__3_WAY_CONFERENCE_MODE__1;

	start3WayConferenceCmd.data = (void *)&_3WayConference;

	if((retValue = (write(dspFd,(char *)&start3WayConferenceCmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		printf("error at write ACG_SET_3WAYCONF_CMD_E\r\n");
		retValue = ACG_FAILURE;
	}



	return (retValue);
}

/******************************************************************************
*	Function:	 acgStop3WayConference
*
*	Description: Stop 3 way conference.
*
*	Parameters:	 [I] int Channel
*				 [I] int ConferenceChannelMate
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgStop3WayConference(int Channel, int ConferenceChannelMate)
{
	acgTCommand			stop3WayConferenceCmd;
	acgT3WayConference	_3WayConference;

	int 				retValue = ACG_SUCCESS;

	printf(">>\r\n");

	memset(&stop3WayConferenceCmd, 0, sizeof(acgTCommand));
	memset(&_3WayConference, 0, sizeof(acgT3WayConference));

	stop3WayConferenceCmd.Command = ACG_SET_3WAYCONF_CMD_E;
	stop3WayConferenceCmd.Channel = Channel;

	_3WayConference.ConferenceChannelMate = ConferenceChannelMate;
	_3WayConference.Mode = ACG__3_WAY_CONFERENCE_MODE__DISABLE;

	stop3WayConferenceCmd.data = (void *)&_3WayConference;

	if((retValue = (write(dspFd,(char *)&stop3WayConferenceCmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		printf("error at write ACG_SET_3WAYCONF_CMD_E\r\n");
		retValue = ACG_FAILURE;
	}

	printf("<<\r\n");

	return (retValue);
}

/******************************************************************************
*	Function:	 acgDecodeMedia
*
*	Description: Send media packet toward DSP.
*
*	Parameters:	 [I] int Channel
*				 [I] acgTDecodeMedia decodeMedia
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgDecodeMedia(int Channel, acgTDecodeMedia *pDecodeMedia)
{
	acgTCommand		decodeMediaCmd;
	int 				retValue = ACG_SUCCESS;

	decodeMediaCmd.Command = ACG_DECODE_MEDIA_CMD_E;
	decodeMediaCmd.Channel = Channel;

	decodeMediaCmd.data = (void *)pDecodeMedia;

	if((retValue = (write(dspFd,(char *)&decodeMediaCmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		ACG_Error_Print("error at write ACG_DECODE_MEDIA_CMD_E\r\n");
		retValue = ACG_FAILURE;
	}

	return (retValue);
}

/******************************************************************************
*	Function:	 acgGetMedia
*
*	Description: Get media packet from DSP.
*
*	Parameters:	 [I/O] acgTGetMedia *pGetMedia
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1) or ACG_NO_PACKET (1)
*
*******************************************************************************/
int acgGetMedia(acgTGetMedia *pGetMedia)
{
	acgTCommand		getMediaCmd;
	int 				retValue = ACG_SUCCESS;

	memset(&getMediaCmd, 0, sizeof(acgTCommand));

	getMediaCmd.Command = ACG_GET_MEDIA_CMD_E;

	getMediaCmd.data = (void *)pGetMedia;

	if((retValue = read(dspFd, &getMediaCmd, sizeof(acgTCommand))) == ACG_FAILURE) {
		ACG_Error_Print("error at read ACG_GET_MEDIA_CMD_E\r\n");
	}

	return (retValue);
}

/******************************************************************************
*	Function:	 acgReinitializeEchoCanceller
*
*	Description: Reinitialize echo canceller.
*
*	Parameters:	 [I] int Channel
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgReinitializeEchoCanceller(int Channel)
{
	acgTCommand		reinitializeEchoCancellerCmd;
	int 				retValue = ACG_SUCCESS;
    printf("-------------------inside acgReinitializeEchoCanceller channel is :%d\n",Channel);
	ACG_Log_Print(">>\r\n");

	memset(&reinitializeEchoCancellerCmd, 0, sizeof(acgTCommand));

	reinitializeEchoCancellerCmd.Command = ACG_REINITIALIZE_ECHO_CANCELER_CMD_E;
	reinitializeEchoCancellerCmd.Channel = Channel;

	if((retValue = (write(dspFd,(char *)&reinitializeEchoCancellerCmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		ACG_Error_Print("error at write ACG_REINITIALIZE_ECHO_CANCELER_CMD_E\r\n");
		retValue = ACG_FAILURE;
	}

	ACG_Log_Print("<<\r\n");

	return (retValue);
}

/******************************************************************************
*	Function:	 acgDebugInformationActivation
*
*	Description: Debug information activation.
*
*	Parameters:	 [I] acgTDebugInformation *pDebugInformation
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgDebugInformationActivation(acgTDebugInformation *pDebugInformation)
{
	acgTCommand		debugInformationCmd;
	int 				retValue = ACG_SUCCESS;

	ACG_Log_Print(">>\r\n");

	if(dspFd == -1)
	{
		printf("acGatewayAPI version is: %s\n", ACG_VERSION);

		if((dspFd = open(ACL_DSP_DRV_FD_NAME_STR, O_RDWR)) == ACG_FAILURE)
		{
			ACG_Error_Print("can't open dsp device\r\n");
			return (ACG_FAILURE);
		}
	}

	memset(&debugInformationCmd, 0, sizeof(acgTCommand));

	debugInformationCmd.Command = ACG_DEBUG_INFORMATION_ACTIVATION;

	debugInformationCmd.data = (void *)pDebugInformation;

	if((retValue = (write(dspFd,(char *)&debugInformationCmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		ACG_Error_Print("error at write ACG_DEBUG_INFORMATION_ACTIVATION\r\n");
		retValue = ACG_FAILURE;
	}

	ACG_Log_Print("<<\r\n");

	return (retValue);
}


/******************************************************************************
*	Function:	 acgStartFax
*
*	Description: Start fax.
*
*	Parameters:	 [I] int Channel
*				 [I] acgEFaxModemTransportMethod FaxModemTransportMethod
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgStartFax(int Channel, acgEFaxModemTransportMethod FaxModemTransportMethod)
{
	acgTCommand		startFaxCmd;
	int 				retValue = ACG_SUCCESS;

	ACG_Log_Print(">>\r\n");

	memset(&startFaxCmd, 0, sizeof(acgTCommand));

	if(FaxModemTransportMethod == ACG_FAX_MODEM_TRANSPORT__RELAY)
	{
		startFaxCmd.Command = ACG_START_FAX_RELAY_CMD_E;
	}
	else if(FaxModemTransportMethod == ACG_FAX_MODEM_TRANSPORT__VBD)
	{
		startFaxCmd.Command = ACG_START_FAX_VBD_CMD_E;
	}
	else
	{
		ACG_Error_Print("Can't start fax with FaxModemTransportMethod %d\r\n", FaxModemTransportMethod);
	}

	startFaxCmd.Channel = Channel;

	if((retValue = (write(dspFd,(char *)&startFaxCmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		ACG_Error_Print("error at write acgStartFax\r\n");
		retValue = ACG_FAILURE;
	}

	ACG_Log_Print("<<\r\n");

	return (retValue);
}

/******************************************************************************
*	Function:	 acgStopFax
*
*	Description: Stop fax.
*
*	Parameters:	 [I] int Channel
*				 [I] acgEFaxModemTransportMethod FaxModemTransportMethod
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgStopFax(int Channel, acgEFaxModemTransportMethod FaxModemTransportMethod)
{
	acgTCommand		stopFaxCmd;
	int 				retValue = ACG_SUCCESS;

	ACG_Log_Print(">>\r\n");

	memset(&stopFaxCmd, 0, sizeof(acgTCommand));

	if(FaxModemTransportMethod == ACG_FAX_MODEM_TRANSPORT__RELAY)
	{
		stopFaxCmd.Command = ACG_STOP_FAX_RELAY_CMD_E;
	}
	else if(FaxModemTransportMethod == ACG_FAX_MODEM_TRANSPORT__VBD)
	{
		stopFaxCmd.Command = ACG_STOP_FAX_VBD_CMD_E;
	}
	else
	{
		ACG_Error_Print("Can't stop fax with FaxModemTransportMethod %d\r\n", FaxModemTransportMethod);
	}

	stopFaxCmd.Channel = Channel;

	if((retValue = (write(dspFd,(char *)&stopFaxCmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		ACG_Error_Print("error at write acgStopFax\r\n");
		retValue = ACG_FAILURE;
	}

	ACG_Log_Print("<<\r\n");

	return (retValue);
}

/******************************************************************************
*	Function:	 acgSetRegularRTCPParameters
*
*	Description: set regular rtcp parameters.
*
*	Parameters:	 [I] int Channel
*				 [I] acgTRegularRTCPParamters *pRegularRTCPParameters
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgSetRegularRTCPParameters(int Channel, acgTRegularRTCPParamters *pRegularRTCPParameters)
{
    acgTCommand     SetRegularRTCPCmd;
    int                 retValue = ACG_SUCCESS;

    ACG_Log_Print(">>\r\n");

    memset(&SetRegularRTCPCmd, 0, sizeof(acgTCommand));
    SetRegularRTCPCmd.Channel = Channel;
    SetRegularRTCPCmd.Command = ACG_SET_REGULAR_RTCP_PARAM_CMD;
    SetRegularRTCPCmd.data = (void *)pRegularRTCPParameters;

	if((retValue = (write(dspFd,(char *)&SetRegularRTCPCmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		ACG_Error_Print("error at write acgSetRegularRTCPParameters\r\n");
        retValue = ACG_FAILURE;
    }

    ACG_Log_Print("<<\r\n");

    return (retValue);
}

/******************************************************************************
*	Function:	 acgGetRegularRTCPParameters
*
*	Description: get regular rtcp parameters.
*
*	Parameters:	 [I] int Channel
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgGetRegularRTCPParameters(int Channel)
{
    acgTCommand     GetRegularRTCPCmd;
    int                 retValue = ACG_SUCCESS;

    ACG_Log_Print(">>\r\n");

    memset(&GetRegularRTCPCmd, 0, sizeof(acgTCommand));
    GetRegularRTCPCmd.Channel = Channel;
    GetRegularRTCPCmd.Command = ACG_GET_REGULAR_RTCP_PARAM_CMD;

	if((retValue = (write(dspFd,(char *)&GetRegularRTCPCmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		ACG_Error_Print("error at write acgGetRegularRTCPParameters\r\n");
        retValue = ACG_FAILURE;
    }

    ACG_Log_Print("<<\r\n");

    return (retValue);
}

/******************************************************************************
*	Function:	 acgSetNtpTimeStamp
*
*	Description: set ntp time
*
*	Parameters:	 [I] int Channel
*				 [I] acgT49xNtpTimeStamp *pNtpTimeStamp
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgSetNtpTimeStamp(int Channel, acgTNtpTimeStamp *pNtpTimeStamp)
{
    acgTCommand     NtpTimeStampCmd;
    int                 retValue = ACG_SUCCESS;

    ACG_Log_Print(">>\r\n");

    memset(&NtpTimeStampCmd, 0, sizeof(acgTCommand));
    NtpTimeStampCmd.Channel = Channel;
    NtpTimeStampCmd.Command = ACG_SET_NTP_TIMESTAMP_CMD_E;
    NtpTimeStampCmd.data = (void *)pNtpTimeStamp;

	if((retValue = (write(dspFd,(char *)&NtpTimeStampCmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		ACG_Error_Print("error at write acgSetNtpTimeStamp\r\n");
        retValue = ACG_FAILURE;
    }

    ACG_Log_Print("<<\r\n");

    return (retValue);
}

/****************************************************************
********												  ********
********			Event Handling API Functions				  ********
********												  ********
*****************************************************************/


/******************************************************************************
*	Function:	 acgGetEvent
*
*	Description: Get a DSP event.
*
*	Parameters:	 [I] acgTEventInfo *pEvent
*				 [I] acgEPollingMethod PollingMethod
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1) or ACG_NO_PACKET (1)
*
*******************************************************************************/
int acgGetEvent (acgTEventInfo *pEvent, acgEPollingMethod PollingMethod)
{
	acgTCommand		getEventCmd;

	acgTGetEventInfo		GetEventInfo;

	int 				retValue = ACG_SUCCESS;

	ACG_Log_Print(">>\r\n");

	memset(&getEventCmd, 0, sizeof(acgTCommand));

	getEventCmd.Command = ACG_DSP_GET_EVENT_CMD;

	GetEventInfo.PollingMethod = PollingMethod;
	GetEventInfo.Buff = (void *)pEvent;

	getEventCmd.data = (void *)&GetEventInfo;

	if((retValue = (read(dspFd,(char *)&getEventCmd,sizeof(acgTCommand)))) == ACG_FAILURE) {
		ACG_Error_Print("error at write ACG_DSP_GET_EVENT_CMD\r\n");
		retValue = ACG_FAILURE;
	}

	ACG_Log_Print("<<\r\n");

	return (retValue);

}

/******************************************************************************
*	Function:	 acgGetTelephonyEvent
*
*	Description: Get a telephony (FXS) event.
*
*	Parameters:	 [I] acgTEventInfo *pEvent
*				 [I] acgEPollingMethod PollingMethod
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int  acgGetTelephonyEvent (acgTEventInfo *pEvent, acgEPollingMethod PollingMethod)
{
	acgTCommand		getTelephonyEventCmd;

	acgTGetEventInfo		GetEventInfo;

	int 				retValue = ACG_SUCCESS;

	ACG_Log_Print(">>\r\n");

	memset(&getTelephonyEventCmd, 0, sizeof(acgTCommand));

	getTelephonyEventCmd.Command = ACG_SLIC_GET_TELEPHONY_EVENT_CMD;

	GetEventInfo.PollingMethod = PollingMethod;
	GetEventInfo.Buff = (void *)pEvent;

	getTelephonyEventCmd.data = (void *)&GetEventInfo;

	if((retValue = (read(slicFd,(char *)&getTelephonyEventCmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		ACG_Error_Print("error at write ACG_SLIC_GET_TELEPHONY_EVENT_CMD\r\n");
		retValue = ACG_FAILURE;
	}

	ACG_Log_Print("<<\r\n");

	return (retValue);
}


/****************************************************************
********												  ********
********				SLIC API Functions					  ********
********												  ********
*****************************************************************/


/******************************************************************************
*	Function:	 acgInitializeFXS
*
*	Description: Initialize the SLIC device.
*
*	Parameters:	[I] acgTFXSInitializationParameters	*pFXSInitializationParameters
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgInitializeFXS(acgTFXSInitializationParameters	*pFXSInitializationParameters)
{
	acgTCommand		initializeFXSCmd;
	int 				retValue = ACG_SUCCESS;

	ACG_Log_Print(">>\r\n");

	if((slicFd = open(ACL_FXS_DRV_FD_NAME_STR, O_RDWR)) == -1)
	{
		ACG_Error_Print("can't open slic device\r\n");
		return ACG_FAILURE;
	}

	memset(&initializeFXSCmd, 0, sizeof(acgTCommand));

	initializeFXSCmd.Command = ACG_SLIC_INIT_CMD;
	initializeFXSCmd.data =  (void *)pFXSInitializationParameters;

	if((retValue = (write(slicFd,(char *)&initializeFXSCmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		ACG_Error_Print("error at write ACG_SLIC_INIT_CMD\r\n");
		retValue = ACG_FAILURE;
	}

	ACG_Log_Print("<<\r\n");

	return (retValue);
}


/******************************************************************************
*	Function:	 acgCloseFXS
*
*	Description: Close the SLIC device.
*
*	Parameters:
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgCloseFXS()
{
	int 	retValue = ACG_SUCCESS;

	ACG_Log_Print(">>\r\n");

	retValue = close(slicFd);

	slicFd = -1;

	ACG_Log_Print("<<\r\n");

	return (retValue);
}


/******************************************************************************
*	Function:	 acgStartRing
*
*	Description: Start ring on a FXS port.
*
*	Parameters:	 [I] int Channel
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgStartRing(int channel)
{
	acgTCommand		startRingCmd;
	int 				retValue = ACG_SUCCESS;

	ACG_Log_Print(">>\r\n");

	memset(&startRingCmd, 0, sizeof(acgTCommand));

	startRingCmd.Command = ACG_SLIC_START_RING_CMD;
	startRingCmd.Channel = channel;

	if((retValue = (write(slicFd,(char *)&startRingCmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		ACG_Error_Print("error at write ACG_SLIC_START_RING_CMD\r\n");
		retValue = ACG_FAILURE;
	}

	ACG_Log_Print("<<\r\n");

	return (retValue);
}

/******************************************************************************
*	Function:	 acgStopRing
*
*	Description: Stop ring on a FXS port.
*
*	Parameters:	 [I] int Channel
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgStopRing(int channel)
{
	acgTCommand		stopRingCmd;
	int 				retValue = ACG_SUCCESS;

	ACG_Log_Print(">>\r\n");

	memset(&stopRingCmd, 0, sizeof(acgTCommand));

	stopRingCmd.Command = ACG_SLIC_STOP_RING_CMD;
	stopRingCmd.Channel = channel;

	if((retValue = (write(slicFd,(char *)&stopRingCmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		ACG_Error_Print("error at write ACG_SLIC_STOP_RING_CMD\r\n");
		retValue = ACG_FAILURE;
	}

	ACG_Log_Print("<<\r\n");

	return (retValue);
}

/******************************************************************************
*	Function:	 acgGetVersionInfo
*
*	Description: Get firmware version info .
*
*	Parameters:	 [I] acgTGetVersionInfo *pGetVersionInfo
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgGetVersionInfo(acgTGetVersionInfo *pGetVersionInfo)
{
	acgTCommand		getVersionCmd;
	int 				retValue = ACG_SUCCESS;

	memset(&getVersionCmd, 0, sizeof(acgTCommand));

	getVersionCmd.Command = ACG_GET_VERSION_CMD;
	getVersionCmd.data = (void *)pGetVersionInfo;

	if((retValue = (write(dspFd,(char *)&getVersionCmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		ACG_Error_Print("error at read ACG_GET_VERSION_CMD\r\n");
	}

	return (retValue);
}

/***************************/
/*     acGateway local functions     */
/***************************/

static void acgFillDefaultParameters(acgTChannelParameters *pParams)
{
    int     Character;
	pParams->RTPStreamParameters.Coder = DEFAULT_CODER;
	pParams->RTPStreamParameters.PTime = DEFAULT_PTIME;
	pParams->RTPStreamParameters.TxVoicePayloadType = DEFAULT_TX_VOICE_PAYLOAD_TYPE;
	pParams->RTPStreamParameters.RxVoicePayloadType = DEFAULT_RX_VOICE_PAYLOAD_TYPE;
	pParams->RTPStreamParameters.TxDTMFRelayPayloadType = DEFAULT_TX_DTMF_RELAY_PAYLOAD_TYPE;
	pParams->RTPStreamParameters.RxDTMFRelayPayloadType = DEFAULT_RX_DTMF_RELAY_PAYLOAD_TYPE;
	pParams->RTPStreamParameters.FaxBypassPayloadType = DEFAULT_FAX_BYPASS_PAYLOAD_TYPE;
	pParams->RTPStreamParameters.ModemByassPayloadType = DEFAULT_MODEM_BYPASS_PAYLOAD_TYPE;
	pParams->RTPStreamParameters.SilenceCompressionMode = DEFAULT_SILENCE_COMPRESSION_MODE;
	pParams->RTPStreamParameters.IbsRedundancyLevel = DEFAULT_IBS_REDUNDANCY_LEVEL;
	pParams->RTPStreamParameters.RtcpMeanTxInterval = DEFAULT_RTCP_MEAN_TX_INTERVAL;
	pParams->RTPStreamParameters.TxSignalingRfc2833PayloadType = DEFAULT_RFC2833TxPT;
	pParams->RTPStreamParameters.RxSignalingRfc2833PayloadType = DEFAULT_RFC2833RxPT;
	pParams->RTPStreamParameters.Rfc2198PayloadType = DEFAULT_RFC2198PT;

	pParams->RTPStreamParameters.CNameLength = DEFAULT_CANONICAL_NAME_LENGTH;
	pParams->RTPStreamParameters.TxRtcpFilter = DEFAULT_TX_FILTER_RTCP;
	pParams->RTPStreamParameters.RxRtcpFilter = DEFAULT_RX_FILTER_RTCP;
	for(Character=0; Character<ACG_MAX_RTP_CANONICAL_NAME_STRING_LENGTH; Character++)  /*AC49X_MAX_RTP_CANONICAL_NAME_STRING_LENGTH*/
        pParams->RTPStreamParameters.CName[Character] = 0;
	pParams->SignalingParameters.DTMFTransportMethod = DEFAULT_DTMF_TRANSPORT_METHOD;
	pParams->SignalingParameters.CPTTransportMethod = DEFAULT_CPT_TRANSPORT_METHOD;
	pParams->SignalingParameters.DetectionRedirection = DEFAULT_DETECTION_REDIRECTION;
	pParams->SignalingParameters.DetectCallProgress = DEFAULT_DETECT_CALLPROGRESS;
	pParams->SignalingParameters.DetectDTMFRegister = DEFAULT_DETECT_DTMF_REGISTER;
	pParams->SignalingParameters.IbsLevel_minus_dbm = DEFAULT_IBS_LEVEL;
	pParams->SignalingParameters.LongDtmfDetectionEnable = DEFAULT_LONG_DTMF_DETECT;

	pParams->VoiceQualityMonitoringParameters.RtcpXrEnable= DEFAULT_RTCP_XR;

	pParams->FaxModemParameters.FaxTransferMethod = DEFAULT_FAX_TRANSFER_METHOD;
	pParams->FaxModemParameters.ModemTransferMethod = DEFAULT_MODEM_TRANSFER_METHOD;
	pParams->FaxModemParameters.FaxMaxRate = DEFAULT_FAX_MAX_RATE;
	pParams->FaxModemParameters.FaxErrorCorrection = DEFAULT_FAX_ERROR_CORRECTION;
	pParams->FaxModemParameters.CngRelay = DEFAULT_CNG_RELAY;
	pParams->FaxModemParameters.BypassCoder = DEFAULT_BYPASS_CODER;
	pParams->FaxModemParameters.ImageDataRedundancyLevel = DEFAULT_IMAGE_DATA_REDUNDANCY_LEVEL__NONE;
	pParams->FaxModemParameters.T30ControlDataRedundancyLevel = DEFAULT_T30_CONTROL_DATA_REDUNDANCY_LEVEL__NONE;
	pParams->FaxModemParameters.CEDTransferMode = DEFAULT_CED_TRANSFER_MODE;
	pParams->FaxModemParameters.V34TransferMethod = DEFAULT_V34_TRANSFER_METHOD;
	pParams->FaxModemParameters.V32TransferMethod = DEFAULT_V32_TRANSFER_METHOD;
	pParams->FaxModemParameters.V23TransferMethod = DEFAULT_V23_TRANSFER_METHOD;
	pParams->FaxModemParameters.V22TransferMethod = DEFAULT_V22_TRANSFER_METHOD;
	pParams->FaxModemParameters.V21TransferMethod = DEFAULT_V21_TRANSFER_METHOD;
	pParams->FaxModemParameters.Bell103TransferMethod = DEFAULT_BELL103_TRANSFER_METHOD;
	pParams->FaxModemParameters.V34FaxTransferMethod = DEFAULT_V34_FAX_TRANSFER_METHOD;

	pParams->CallerIDParameters.CallerIdStandard = DEFAULT_CALLER_ID_STANDARD;
	pParams->CallerIDParameters.CallerIdOnHookDetection = DEFAULT_CALLER_ID_ON_HOOK_DETECTION;
	pParams->CallerIDParameters.CallerIdOffHookDetection = DEFAULT_CALLER_ID_OFF_HOOK_DETECTION;

	pParams->GainParameters.VoiceOutputGain = DEFAULT_VOICE_OUTPUT_GAIN;
	pParams->GainParameters.InputGain = DEFAULT_INPUT_GAIN;
	pParams->GainParameters.AgcEnable = DEFAULT_AGC_MODE;
	pParams->GainParameters.AgcLocation = DEFAULT_AGC_LOCATION;
	pParams->GainParameters.AgcTargetEnergy = DEFAULT_AGC_TARGET_ENERGY;

	pParams->JitterBufferParameters.DynamicJitterBufferOptimizationFactor = DEFAULT_DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR;
	pParams->JitterBufferParameters.VoiceJitterBufferMinDelay = DEFAULT_VOICE_JITTER_BUFFER_MIN_DELAY;
	pParams->JitterBufferParameters.VoiceJitterBufferMaxDelay = DEFAULT_VOICE_JITTER_BUFFER_MAX_DELAY;

	pParams->EchoCancellerParameters.EchoCanceler = DEFAULT_ECHO_CANCELER;
	pParams->EchoCancellerParameters.EchoCancelerNonLinearProcessor = DEFAULT_ECHO_CANCELER_NON_LINEAR_PROCESSOR;
	pParams->EchoCancellerParameters.EchoCancelerLength = DEFAULT_ECHO_CANCELER_LENGTH;

	pParams->MiscellaneousParameters.TestMode = DEFAULT_TEST_MODE;

}



/******************************************************************************
*	Function:	 acgInitializeDAA
*
*	Description: Initialize the DAA device.
*
*	Parameters:
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgInitializeDAA(void )
{
	acgTCommand		initializeDaaCmd;
	int 			retValue = ACG_SUCCESS;

	//printf("acgInitializeDAA - daaFd = %d\r\n",daaFd);
	ACG_Log_Print(">>\r\n");

	if((daaFd = open(ACL_DAA_DRV_FD_NAME_STR, O_RDWR)) == -1)
	{
		perror("can't open Daa driver\n");
		return -1;
	}


	memset(&initializeDaaCmd, 0, sizeof(acgTCommand));

	initializeDaaCmd.Command = ACG_DAA_INIT_CMD;

	if((retValue = (write(daaFd,(char *)&initializeDaaCmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		ACG_Error_Print("error at write ACG_DAA_INIT_CMD\r\n");
		retValue = ACG_FAILURE;
	}

	ACG_Log_Print("<<\r\n");

	return (retValue);
}

/******************************************************************************
*	Function:	 acgGetDAAEvent
*
*	Description: Get a telephony (FXO) event.
*
*	Parameters:	 [I] acgTEventInfo *pEvent
*				 [I] acgEPollingMethod PollingMethod
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int  acgGetDAAEvent (acgTEventInfo *pEvent, acgEPollingMethod PollingMethod)
{
	acgTCommand		getDaaEventCmd;

	acgTGetEventInfo		GetEventInfo;

	int 				retValue = ACG_SUCCESS;

	ACG_Log_Print(">>\r\n");
	memset(&getDaaEventCmd, 0, sizeof(acgTCommand));

	getDaaEventCmd.Command = ACG_DAA_GET_TELEPHONY_EVENT_CMD;

	GetEventInfo.PollingMethod = PollingMethod;
	GetEventInfo.Buff = (void *)pEvent;

	getDaaEventCmd.data = (void *)&GetEventInfo;

	if((retValue = (read(daaFd,(char *)&getDaaEventCmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		ACG_Error_Print("error at write ACG_DAA_GET_TELEPHONY_EVENT_CMD\r\n");
		retValue = ACG_FAILURE;
	}

	ACG_Log_Print("<<\r\n");

	return (retValue);

}



/******************************************************************************
*	Function:	 acgSeizeLine
*
*	Description: Seizes a DAA port.
*
*	Parameters:	 [I] int Channel
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgSeizeLine(int channel)
{
    acgTCommand		seizeLineCmd;
	int 				retValue = ACG_SUCCESS;

	memset(&seizeLineCmd, 0, sizeof(acgTCommand));

	seizeLineCmd.Command = ACG_DAA_SEIZE_LINE_CMD;
	seizeLineCmd.Channel = channel;

	if((retValue = (write(daaFd,(char *)&seizeLineCmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		ACG_Error_Print("error at write ACG_DAA_SEIZE_LINE_CMD[%d]\r\n", daaFd);
		retValue = ACG_FAILURE;
	}




	return (retValue);
}


/******************************************************************************
*	Function:	 acgReleaseLine
*
*	Description: Releases a DAA port.
*
*	Parameters:	 [I] int Channel
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgReleaseLine(int channel)
{
       acgTCommand		releaseLineCmd;
	int 				retValue = ACG_SUCCESS;

	memset(&releaseLineCmd, 0, sizeof(acgTCommand));

	releaseLineCmd.Command = ACG_DAA_RELEASE_LINE_CMD;
	releaseLineCmd.Channel = channel;

	if((retValue = (write(daaFd,(char *)&releaseLineCmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		ACG_Error_Print("error at write ACG_DAA_RELEASE_LINE_CMD[%d]\r\n", daaFd);
		retValue = ACG_FAILURE;
	}


	return (retValue);
}

/******************************************************************************
*	Function:	 acgGenerateFlash
*
*	Description: Generates flash hook towards a DAA port.
*
*	Parameters:	 [I] int Channel
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgGenerateFlash(int channel)
{
     acgTCommand		generateFlash;
	int 				retValue = ACG_SUCCESS;


	memset(&generateFlash, 0, sizeof(acgTCommand));

	generateFlash.Command = ACG_DAA_GENERATE_FLASH_CMD;
	generateFlash.Channel = channel;

	if((retValue = (write(daaFd,(char *)&generateFlash,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		ACG_Error_Print("error at write ACG_DAA_GENERATE_FLASH_CMD[%d]\r\n", daaFd);
		retValue = ACG_FAILURE;
	}

	return (retValue);
}

/******************************************************************************
*	Function:	 acgCloseDaa
*
*	Description: Close the Daa device.
*
*	Parameters:
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgCloseDAA()
{
	int 	retValue = ACG_SUCCESS;

	ACG_Log_Print(">>\r\n");

	retValue = close(daaFd);

	daaFd = -1;

	ACG_Log_Print("<<\r\n");

	return (retValue);
}

static int acgMs2Bytes(acgECoder coder)
{
    int iFactor = 10;
    switch(coder)
    {
	    case ACG_CODER__G711ALAW:
			iFactor = 80; break;
	    case ACG_CODER__G711MULAW:
	        iFactor = 80; break;
	    case ACG_CODER__G726_16:
	        iFactor = 20; break;
	    case ACG_CODER__G726_24:
	        iFactor = 30; break;
	    case ACG_CODER__G726_32:
	        iFactor = 40; break;
	    case ACG_CODER__G726_40:
	        iFactor = 50; break;
	    case ACG_CODER__G723LOW:
	        iFactor = 20; break;
	    case ACG_CODER__G723HIGH:
	        iFactor = 24; break;
	    case ACG_CODER__G729:
	        iFactor = 10; break;
		case ACG_CODER__LINEAR_16_BIT_PCM_SAMPLE_RATE_16KHZ:
			iFactor = 320; break;
	    default:
	        break;
	}
    return iFactor;
}
/******************************************************************************
*	Function:	 acgUpdateCadence
*
*	Description: Update SLIC Cadence
*
*	Parameters:
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgUpdateCadence(int Cadence, int Channel)
{
	acgTCommand		updateCadence;
	int 				retValue = ACG_SUCCESS;

	memset(&updateCadence, 0, sizeof(acgTCommand));
	ACG_Log_Print(">>\r\n");

	if (Cadence == SLIC_RING_CADENCE_PROFILE_NA)
		return retValue;

	updateCadence.Command = ACG_UPDATE_FXS_CADENCE_PROFILE;
	updateCadence.Channel = Channel;
	updateCadence.data = (void *)Cadence;

	if ((retValue = write(slicFd, (char *)&updateCadence, sizeof(acgTCommand))) != ACG_SUCCESS){
		retValue = ACG_FAILURE;
	}

	return (retValue);
}

/******************************************************************************
*	Function:	 acgPlaybackStart
*
*	Description: Activate playback once.
*
*	Parameters:
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgPlaybackStart(int Channel, int pbType, acgECoder	coder, char *pstrFileName)
{
	int     retValue = ACG_SUCCESS;
	acgTCommand RingStartCmd;
	acgTPlaybackData Playbackvoicedata;

	ACG_Log_Print(">>\r\n");

	printf("acgPlayback_%s channel = %d pWaveFile = %s Coder = %d\n",(0==pbType)?"TDM":"Network", Channel, pstrFileName, coder);
    /* The START command must end by END command, if the END command does not execute, */
    /* the second START command is not permitted to execute*/
	if(g_ring_request_count[Channel])
	{
	    printf("WARNING: There is a playback command is executing on channel %d, please wait for END or set acgPlaybackEnd to stop it manually\r\n", Channel);
	    return retValue;
    }
	g_ring_request_count[Channel]++;
	printf("acgplaybackstart PHI %d \n",g_ring_request_count[Channel]);
	voice_data_size[Channel] = 0x7fffffff;
	msec2bytes = acgMs2Bytes(coder);

	if(pstrFileName == NULL)
	{
		printf("NO recoding file ,parameter error! \r\n");
		goto playback_fail ;
	}

	ringingToneFd[Channel] = open(pstrFileName, O_RDONLY);

	if(ringingToneFd[Channel]<0)
	{
		printf("Open tone file fail \r\n");
		goto playback_fail;
 	}

 	/* Get the size of voice file */
 	voice_data_size[Channel] = lseek(ringingToneFd[Channel], 0, SEEK_END);
 	printf("\r\nvoice_data_size[%d]; channel[%d]\r\n", voice_data_size[Channel], Channel);
 	/* Restore to the begin of voice file*/
    lseek(ringingToneFd[Channel], 0, SEEK_SET);

    memset(&Playbackvoicedata, 0, sizeof(acgTPlaybackData));
	Playbackvoicedata.Coder = coder;
	Playbackvoicedata.size = voice_data_size[Channel];

    memset(&RingStartCmd, 0, sizeof(acgTCommand));
	RingStartCmd.Channel = Channel;
	if(0==pbType)
	    RingStartCmd.Command = ACG_PLAYBACK_TDM_START_CMD;
    else
	    RingStartCmd.Command = ACG_PLAYBACK_NETWORK_START_CMD;
	RingStartCmd.data = (void *)&Playbackvoicedata;

	if((retValue = (write(dspFd,(char *)&RingStartCmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		printf("error at write acgPlaybackStart\r\n");
        retValue = ACG_FAILURE;
	}

	ACG_Log_Print("<\n");
	return (retValue);

playback_fail:
	printf("Wave_ring_fail\r\n");
	close(ringingToneFd[Channel]);
	ringingToneFd[Channel] =0;

	return ACG_FAILURE;
}

int acgPlaybackEnd(int Channel)
{
	acgTCommand RingStopCmd;
 	int retValue=0;
	RingStopCmd.Channel = Channel;
	RingStopCmd.Command = ACG_PLAYBACK_END_CMD;

	if (g_ring_request_count[Channel] == 0)
	{
        printf("1 if g_ring_request_count[Channel] %d \n ",g_ring_request_count[Channel]);
		return -3;
		}
	else
	{

		g_ring_request_count[Channel] --;
		printf("1 else g_ring_request_count[Channel] %d \n ",g_ring_request_count[Channel]);
    }
	if(g_ring_request_count[Channel] > 0)
	{
        	return -2;
	} /*requestion > 0 do not stop*/


	if((retValue = (write(dspFd,(char *)&RingStopCmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		ACG_Error_Print("error at write ACG_PLAYBACK_END_CMD\r\n");
	}

	if(ringingToneFd[Channel])
	{
        printf("ringingToneFd \n");
        close(ringingToneFd[Channel]);
	   ringingToneFd[Channel] =0;
    }
	return retValue;
}

void acgPlaybackDataGet(int getsize, int Channel, unsigned char *pDataRead)
{
	static acgTPlaybackData playbackvoicedata[ACG_NUMBER_OF_DSP_CHANNEL];
	acgTCommand PlaybackCmd;
	int retValue;
	int readsize;
	int offset;

	PlaybackCmd.Channel = Channel;
	PlaybackCmd.Command = ACG_PLAYBACK_PLAY_VOICE_CMD;

    if(pDataRead)
    {
        getsize = DEFAULT_PLAYBACK_BUF;
        if(pDataRead)
	        memcpy(playbackvoicedata[Channel].DataRead, pDataRead, sizeof(playbackvoicedata[Channel].DataRead));
    }
    else
    {
    	getsize = (getsize/10) * msec2bytes;

    	if(getsize > 1600)
    		getsize = 1600;
	}

	if(ringingToneFd[Channel])
	{

		offset = lseek(ringingToneFd[Channel], 0, SEEK_CUR);
		//printf("%d offset \n",offset);
		readsize = read(ringingToneFd[Channel], (char *)playbackvoicedata[Channel].buf, getsize) ;
		//printf("%d offset readsize %d \n",readsize,getsize);
		if((readsize+offset) >= voice_data_size[Channel])
		{
			playbackvoicedata[Channel].size = voice_data_size[Channel] - offset;
			readsize = playbackvoicedata[Channel].size;
			// printf("if of offset size %d \n",readsize);
		}
		else
		{
            // printf("else of offset size %d \n",readsize);
			playbackvoicedata[Channel].size = readsize;
		}


		if(readsize < 0)
		{

			return;
		}
    }
    else
    {
        /*ACG_Error_Print("no wavefile\n ");*/
        return;
    }

    PlaybackCmd.data = &playbackvoicedata[Channel];

	if((retValue = (write(dspFd,(char *)&PlaybackCmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		ACG_Error_Print("error at write ACG_PLAYBACK_PLAY_VOICE_CMD\r\n");
    }

    if(readsize < getsize)/*file end , set END command to DSP*/
    {
        printf("read is less thant get\n");
        offset = lseek(ringingToneFd[Channel], 0, SEEK_SET);
	  //  acgPlaybackEnd(Channel);
    }
}

/******************************************************************************
*	Function:	 acgTimeSlotManipulation
*
*	Description: Manipulate TimeSlot.
*
*	Parameters:	 [I] acgTTimeSlotManipulationAttr *pAttr
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acgTimeSlotManipulation (/*int Channel,*/ acgTTimeSlotManipulationAttr *pAttr)

{
	acgTCommand		TSManipulateCmd;
	int 			retValue = ACG_SUCCESS;

	ACG_Log_Print(">>\r\n");

	memset(&TSManipulateCmd, 0, sizeof(acgTCommand));

	TSManipulateCmd.Command = ACG_TIMESLOT_MANIPULATION_CMD;
	TSManipulateCmd.Channel = 0;
	/*TSManipulateCmd.Channel = Channel;*/

    TSManipulateCmd.data =(void *) pAttr;

	if((retValue = (write(dspFd,(char *)&TSManipulateCmd,sizeof(acgTCommand)))) != ACG_SUCCESS) {
		ACG_Error_Print("error at write ACG_TIMESLOT_MANIPULATION_CMD\r\n");
		retValue = ACG_FAILURE;
	}

	ACG_Log_Print("<<\r\n");

	return (retValue);
}
