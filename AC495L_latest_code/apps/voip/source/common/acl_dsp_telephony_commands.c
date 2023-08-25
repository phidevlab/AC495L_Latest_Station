/********************************************************************
 * AudioCodes improved application phone adaptation layer           *
 *                                                                  *
 * File Name:	acl_dsp_telephony_commands.c                        *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 28/01/08 - Coding started.                                       *
 *																	*
 ********************************************************************/



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

#if defined(ACL_SIP_APPLICATION)
#include <ipplog.h>
#endif
#include "voip_lcd.h"
#include "voip_utils.h"
#include "acl_line_connection.h"
#include "acl_voip_conf.h"
#include "iphone_localNetTerm.h"
#include "iphone_fxs.h"
#include "iphone_fxo.h"
#include "voip_rtp_utils.h"
#include "voip_media_handler.h"
#include "acl_dsp_telephony_commands.h"
#include "acl_dsp_telephony_events.h"

#include "acl_common.h"
#include "acGatewayAPI.h"
#ifdef CONFIG_RG_IPPHONE
#include "acIPPhoneAPI.h"
#endif


#define  AC_DSP_TELEPHONY_COMMAND_DEBUG 1


#ifdef AC_DSP_TELEPHONY_COMMAND_DEBUG
//#define DBG_PRINT(a1,a2...)	IppLogMessage(RV_FALSE, "[%s:%4d] " a1,__FUNCTION__, __LINE__, ##a2)
#define DBG_PRINT(a1,a2...)		printf("[%s:%4d] " a1,__FUNCTION__, __LINE__, ##a2)
#else
#define DBG_PRINT(a1,a2...)
#endif

#define DBG_PRINT_ERR(a1,a2...) IppLogMessage(RV_TRUE, "[%s:%4d] " a1,__FUNCTION__, __LINE__, ##a2)


/*** Definitions and Macros ***/


/***  Structure Definitions   ***/


/*** Global Parameters  ***/

static acgTChannelParameters	ChannelParameters[CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES][NUMBER_OF_CHANNELS_PER_LINE];

static lines_t					lines[CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES];

extern lcd_caller_id_data_t 		*caller_id;

int setchannelparamters_Phi(int ch,int payload, int ptime, int dtmf_inband,int dtmfplayload);
extern pthread_t  	startFxsTid;
extern pthread_t    startFxoTid;

appl_code2code_t payload2coder[] =
{
	{0, 		ACG_CODER__G711MULAW},
	{2, 		ACG_CODER__G726_32},
	{4, 		ACG_CODER__G723HIGH},
	{8, 		ACG_CODER__G711ALAW},
	{9,		ACG_CODER__G722_48K},
	{13, 		-1},
	{15, 		-1},
	{18, 		ACG_CODER__G729},
/* add before this record*/
	{-1, -1},
};


extern pthread_t receiverTid;
extern pthread_t receiverEventTid;
extern pthread_t  acl_UDPrxJobTid;
extern pthread_t  acl_RTCPrxJobTid;




extern int 	dspFd;	/* DSP file descriptor */

/*** prototypes declaration ***/
#if defined(S_PLATFORM)
static int ReadFirmware(char *pfirmwareName, char **ppfirmwareBuff, int *pfirmwareSize);
#endif

static int SetChannelParams(int n, int ch, int payload, int ptime,
						int dtmf_method, int rtp_context, int faxDataMethod);

static int SetAdvancedChannelParams(voip_dsp_advanced_channel_configuration_t* p_voip_dsp_advanced_channel_configuration,
								regional_settings_params_t* p_regional_settings_params);
static void FillRegionalSettingsParams(acgTInitializationParameters 	*pInitializationParams, regional_settings_params_t* p_regional_settings_params);

#ifdef CONFIG_RG_ATA
static int IsCallerIDType2Supported(acgECallerIdStandard CallerIdStandard);
static int IsDTMF_CallerID(acgECallerIdStandard CallerIdStandard);
static int Is_MWI_Supported(acgECallerIdStandard CallerIdStandard);
#endif

void acl_rtp_unbind(int dspChannel);

void callerIdCB(void *data);

#if defined(B_CEVA_PLATFORM)
void initDsp(acgTInitializationParameters 	*pInitializationParams);
#endif /* B_CEVA_PLATFORM */


/*** General Functions ***/

int getLineByDspChannel(int DspChannel)
{
	DBG_PRINT("DspChannel = %d\r\n", DspChannel);

	if ((0 > DspChannel) || (NUMBER_OF_DSP_CHANNELS <= DspChannel))
		return -1;

	return(LineAndVoiceChannel2DspChannelConversionTable[DspChannel].line);
}


/************************************/
/**********  Internal Functions **********/
/************************************/
#if defined(S_PLATFORM)
static int ReadFirmware(char *pfirmwareName, char **ppfirmwareBuff, int *pfirmwareSize)
{
       FILE	                        	*firmwareFd;
	struct stat	                 fileStat;
    if((firmwareFd = fopen(pfirmwareName,"rb")) == NULL)
	{
		perror("can't open firmware file\r\n");
		return -1;
	}

	if(stat(pfirmwareName, &fileStat) == -1)
	{
	      fclose(firmwareFd);
		perror("can't get file status\r\n");
		return -1;
	}

	if((*ppfirmwareBuff = (char *)malloc(fileStat.st_size)) == NULL)
	{
	       fclose(firmwareFd);
		perror("can't allocate file size \r\n");
		return -1;
	}

	if(fread(*ppfirmwareBuff ,1, fileStat.st_size, firmwareFd) != (unsigned int)fileStat.st_size)
	{
	        fclose(firmwareFd);
               free(*ppfirmwareBuff);
		perror("error while read file\r\n");
		return -1;
	}

        *pfirmwareSize =  fileStat.st_size;
	fclose(firmwareFd);
       return 0;
}
#endif /*S_PLATFORM */

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
void dsp_init(regional_settings_params_t* p_regional_settings_params, voip_dsp_advanced_channel_configuration_t* p_voip_dsp_advanced_channel_configuration)
{
	int								    LineIndex, VoiceChannelIndex;

	acgTInitializationParameters 	    InitializationParams;

	int 								dspChannelIndex;
	int								    iNumOfLines;
	acgTDebugInformation 				DebugInformation;

   	DBG_PRINT(">>\r\n");

#if defined(S_PLATFORM)
	char	                        			*pFirmwareBuff = NULL;
       int                                              FileSize;
#if defined(AC49_SMB_VOIP_TOOLKIT) || defined(AC494_TULIP2_40) || defined(AC49_ORCHID_ATA_VOIP_TOOLKIT)
       char	                        			*pSecondFirmwareBuff = NULL;
       int                                              SecondFileSize;
#endif
#endif

	memset(&InitializationParams, 0, sizeof(InitializationParams));
	memset(&DebugInformation, 0, sizeof(DebugInformation));

    	iNumOfLines=voip_conf_get_number_of_lines(ANALOG_PORT_TYPE_FXO_FXS);

	/* Gets the defaults values to the DSP initialization structure */
	if(acgGetDefaultInitDeviceParameters(&InitializationParams)!= ACG_SUCCESS )
	{
		printf("error - acgGetDefaultInitDeviceParameters()\r\n");
		return;
	}

	voip_conf_debug_info_get(&InitializationParams.RemoteRecordingPort,
                                        			InitializationParams.RemoteRecordingIpAddress);

	DebugInformation.PacketRecording = voip_conf_packet_recording_enable_get();

	FillRegionalSettingsParams(&InitializationParams, p_regional_settings_params);

printf("DSP_FIRMWARE_NAME = %s\r\n", DSP_FIRMWARE_NAME);
#if defined(S_PLATFORM)
	if(ReadFirmware(DSP_FIRMWARE_NAME, &pFirmwareBuff, &FileSize)==-1)
	{
            perror("error while read firmware\r\n");
		return;
	}
	InitializationParams.PrimaryDSPDeviceInformation.pProgram = pFirmwareBuff;
	InitializationParams.PrimaryDSPDeviceInformation.ProgramSize = FileSize;
	InitializationParams.PrimaryDSPDeviceInformation.NumberOfDevices = 1;
	printf("DSP_FIRMWARE_NAME = %s\r\n", DSP_FIRMWARE_NAME);
#if defined(AC49_SMB_VOIP_TOOLKIT) || defined(AC49_ORCHID_ATA_VOIP_TOOLKIT)
	if(ReadFirmware(SECOND_DSP_FIRMWARE_NAME, &pSecondFirmwareBuff, &SecondFileSize)==-1)
	{
		perror("error while read second firmware\r\n");
		return;
	}
	printf("SECOND_DSP_FIRMWARE_NAME = %s\r\n", SECOND_DSP_FIRMWARE_NAME);

	InitializationParams.SecondaryDSPDeviceInformation.pProgram = pSecondFirmwareBuff;
	InitializationParams.SecondaryDSPDeviceInformation.ProgramSize = SecondFileSize;
	InitializationParams.SecondaryDSPDeviceInformation.NumberOfDevices = 3;
#endif
#if defined(AC494_TULIP2_40)
       if(ReadFirmware(SECOND_DSP_FIRMWARE_NAME, &pSecondFirmwareBuff, &SecondFileSize)==-1)
        {
            perror("error while read second firmware\r\n");
                return;
        }

        InitializationParams.SecondaryDSPDeviceInformation.pProgram = pSecondFirmwareBuff;
        InitializationParams.SecondaryDSPDeviceInformation.ProgramSize = SecondFileSize;
        InitializationParams.SecondaryDSPDeviceInformation.NumberOfDevices = 1;
#endif

#else

	/* AC48x DSP Template */
	InitializationParams.DspVersionTemplateNumber = 0;

#endif /* defined(S_PLATFORM) */


	/* Only packet recording can be enabled before initializing the DSP and opening the channels */
	if (DebugInformation.PacketRecording)
	{
		/* Debug information activation */
		if(acgDebugInformationActivation(&DebugInformation) != ACG_SUCCESS )
		{
			printf("error - acgDebugInformationActivation()\r\n");
		}
	}

#if defined(B_CEVA_PLATFORM)

	initDsp(&InitializationParams);

#else

	/* Initialize the DSP */
	if(acgInitializeDSP(&InitializationParams) != ACG_SUCCESS )
	{
		printf("error - acgInitializeDSP()\r\n");
		return;
	}

#endif /* B_CEVA_PLATFORM */

#if defined(S_PLATFORM)
	free(InitializationParams.PrimaryDSPDeviceInformation.pProgram);
#if defined(AC49_SMB_VOIP_TOOLKIT) || defined(AC494_TULIP2_40) || defined(AC49_ORCHID_ATA_VOIP_TOOLKIT)
	free(InitializationParams.SecondaryDSPDeviceInformation.pProgram);
#endif

#endif
	memset(&lines, 0, sizeof(lines));

	for(LineIndex=0; LineIndex < iNumOfLines ; LineIndex++)
	{
		for(VoiceChannelIndex=0; VoiceChannelIndex < NUMBER_OF_CHANNELS_PER_LINE; VoiceChannelIndex++)
		{
			lines[LineIndex].ch[VoiceChannelIndex].id = getDspChannelByLineAndVoiceChannel(LineIndex, VoiceChannelIndex);

			lines[LineIndex].ch[VoiceChannelIndex].conferenceId = getDspChannelByLineAndVoiceChannel(LineIndex, VoiceChannelIndex);

#if defined(AC488_CPE_VOIP_TOOLKIT)
			/* Needed for AC48802 Template */
			if(lines[LineIndex].ch[VoiceChannelIndex].conferenceId == -1)
				lines[LineIndex].ch[VoiceChannelIndex].conferenceId = 1 - LineIndex;
#endif

			/* Setting default parameters to all channels */
			if(acgGetDefaultChannelParameters(&(ChannelParameters[LineIndex][VoiceChannelIndex])) != ACG_SUCCESS )
			{
				printf("error - acgGetDefaultChannelParameters()\r\n");
				return;
			}
		}
	}

#if defined(AC494_TULIP2_21)
	lines[2].ch[0].id = 2;
	lines[2].ch[1].id = 2;
#endif

    /* Set advance parameter for all channels */
	if(SetAdvancedChannelParams(p_voip_dsp_advanced_channel_configuration, p_regional_settings_params) != 0 )
	{
		printf("error - SetAdvancedChannelParams()\r\n");
		return;
	}

    /* Open channels accroding the config file */
	for(LineIndex=0; LineIndex < iNumOfLines ; LineIndex++)
	{
		for(VoiceChannelIndex=0; VoiceChannelIndex < NUMBER_OF_CHANNELS_PER_LINE; VoiceChannelIndex++)
		{
			if( (VoiceChannelIndex == 0) && (lines[LineIndex].ch[VoiceChannelIndex].id != -1) )
			{
				/* Open only primary channels */
				dspChannelIndex = lines[LineIndex].ch[VoiceChannelIndex].id;

				/* Open a DSP channel */
				if(acgOpenChannel(dspChannelIndex, &(ChannelParameters[LineIndex][VoiceChannelIndex])) != ACG_SUCCESS )
				{
					printf("error - acgOpenChannel()\r\n");
					return;
				}

				lines[LineIndex].ch[VoiceChannelIndex].is_channel_open = 1;
			}
		}
	}

    /* Activate the debug recording */
	DebugInformation.SpecificRecordingInformation.Channel = voip_conf_tdm_network_recording_channel_get();
	DebugInformation.SpecificRecordingInformation.NetworkRecording = voip_conf_network_recording_enable_get();
	DebugInformation.SpecificRecordingInformation.TDMRecording = voip_conf_tdm_recording_enable_get();

	/* Check if network or tdm recording are enabled */
	if (DebugInformation.SpecificRecordingInformation.NetworkRecording ||
			DebugInformation.SpecificRecordingInformation.TDMRecording)
	{
		/* Debug information activation only for primary channels */
		if(acgDebugInformationActivation(&DebugInformation) != ACG_SUCCESS )
		{
			printf("error - acgDebugInformationActivation(): ch=%d\r\n", DebugInformation.SpecificRecordingInformation.Channel);
		}
	}

	/* launch the DSP related threads */
	aclVoipRtpUtils_resetNetworkingDBs();
	create_media_threads();
    printf("in dsp_init\n ");
    if (aclVoipRtpUtils_createSockets() == -1)
    {
		printf("error - aclVoipRtpUtils_createSockets()\r\n");
      		/*problem - print some error and probably return*/
    }

	/* create the rtp/rtcp rx thread */
	iphone_createRtpRxThread();
	iphone_createRtcpRxThread();

	DBG_PRINT("<<\r\n");

}

#ifdef CONFIG_RG_ATA
/******************************************************************************
*	Function:	 slic_init
*
*	Description: Initialize the SLIC .
*
*	Parameters:	 [I] void *arg
*
*	Returns: 	case of error -1 else 0
*
*******************************************************************************/
void slic_init(void *arg)
{
	DBG_PRINT(">>\r\n");

	if(startFxs(arg) == -1)
		printf("\nerror at startFxs");

	DBG_PRINT("<<\r\n");

}

#ifdef FXO_SILABS

/******************************************************************************
*	Function:	 slac_init
*
*	Description: Initialize the SLAC .
*
*	Parameters:	 [I] void *arg
*
*	Returns: 	case of error -1 else 0
*
*******************************************************************************/
void slac_init(void *arg)
{
    DBG_PRINT(">>\r\n");

	if(startFxo(arg) == -1)
		printf("\nerror at startFxo");

	DBG_PRINT("<<\r\n");
}
#endif /* FXO_SILABS */

#endif /* CONFIG_RG_ATA */

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
void dsp_uninit()
{
	printf(">>\r\n");

	pthread_kill(receiverTid, SIGUSR1);
	pthread_kill(receiverEventTid, SIGUSR1);
	pthread_kill(acl_UDPrxJobTid, SIGUSR1);
	pthread_kill(acl_RTCPrxJobTid, SIGUSR1);

	/* Close the DSP */
	if(acgCloseDSP() != ACG_SUCCESS )
	{
 		printf("error - acgCloseDSP()\r\n");
		return;
	}
    aclVoipRtpUtils_freeNetworkingDbs();
    aclVoipRtpUtils_resetNetworkingDBs();
	printf("<<DSP uninit\r\n");

}

#ifdef CONFIG_RG_ATA

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
void slic_uninit()
{
	DBG_PRINT(">>\r\n");


	pthread_kill(startFxsTid, SIGUSR1);
	/* Close the SLIC device */
	if(acgCloseFXS() != ACG_SUCCESS )
	{
		printf("error - acgCloseFXS()\r\n");
		return;
	}

	DBG_PRINT("<<\r\n");
}

#ifdef FXO_SILABS

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
void slac_uninit()
{
	DBG_PRINT(">>\r\n");

        pthread_kill(startFxoTid, SIGUSR1);
	/* Close the SLAC device */
	if(acgCloseDAA() != ACG_SUCCESS )
	{
		printf("error - acgCloseDaa()\r\n");
		return;
	}

	DBG_PRINT("<<\r\n");
}
#endif /* FXO_SILABS */

#endif /* CONFIG_RG_ATA */

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
					unsigned int rtp_context, int faxMethod)
{
	int dspChannel;

   	DBG_PRINT(">>\r\n");

    	if(n < 0 || n > (CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES -1))
	{
		printf("line %d is not defined\r\n", n);
		return ACG_FAILURE;
	}

	if (ch < 0 || ch > 1)
	{
		printf("ch %d is not defined\r\n", ch);
		return ACG_FAILURE;
	}

	dspChannel = lines[n].ch[ch].id;
	/*  In order to configure a channel with the desired ptime and codecs, it
     		needs to be closed and re-opened with the desired arguments  */

   	DBG_PRINT("n = %d  ch = %d  dspChannel = %d\r\n", n, ch, dspChannel);

	SetChannelParams(n, ch, payload, ptime, dtmf_inband, rtp_context, faxMethod);

	if(dspChannel != -1)
	{
		if(lines[n].ch[ch].is_channel_open)
		{
   			DBG_PRINT("\r\n");

			/* Close a DSP channel */
			if(acgCloseChannel(dspChannel) != ACG_SUCCESS )
			{
				printf("error - acgCloseChannel()\r\n");
				return ACG_FAILURE;
			}

			lines[n].ch[ch].is_channel_open = 0;

		}

   		DBG_PRINT("\r\n");

		/* Open a DSP channel */
		if(acgOpenChannel(dspChannel, &(ChannelParameters[n][ch])) != ACG_SUCCESS )
		{
			printf("error - acgOpenChannel()\r\n");
			return ACG_FAILURE;
		}

		lines[n].ch[ch].is_channel_open = 1;

		/* Activate the DSP channel RTP stream */

		if(acgActivateRTP(dspChannel) != ACG_SUCCESS )
		{
			printf("error - acgActivateRTP()\r\n");
			return ACG_FAILURE;
		}
	}

	DBG_PRINT("<<\r\n");

	return ACG_SUCCESS;

}

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
					u32 rtp_context,int faxMethod)
{
	int dspChannel;

   	DBG_PRINT(">>\r\n");

    	if(n < 0 || n > (CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES -1))
	{
		printf("line %d is not defined\r\n", n);
		return ACG_FAILURE;
	}

	if (ch < 0 || ch > 1)
	{
		printf("ch %d is not defined\r\n", ch);
		return ACG_FAILURE;
	}

	dspChannel = lines[n].ch[ch].id;
	/*  In order to configure a channel with the desired ptime and codecs, it
     		needs to be closed and re-opened with the desired arguments  */

   	DBG_PRINT("n = %d  ch = %d  dspChannel = %d\r\n", n, ch, dspChannel);

	SetChannelParams(n, ch, payload, ptime, dtmf_inband, rtp_context, faxMethod);

	if(dspChannel != -1)
	{
		if(lines[n].ch[ch].is_channel_open)
		{
   			DBG_PRINT("\r\n");

			/* Update a DSP channel */
			if(acgUpdateChannel(dspChannel, &(ChannelParameters[n][ch])) != ACG_SUCCESS )
			{
				printf("error - acgUpdateChannel()\r\n");
				return ACG_FAILURE;
			}
		}
		else
		{
			DBG_PRINT("line %d ch %d is close - Can't update a close channel\r\n", n, ch);
		}

   		DBG_PRINT("\r\n");
	}

	DBG_PRINT("<<\r\n");

	return ACG_SUCCESS;

}



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
void dsp_voice_stop(int n, int ch)
{
	int dspChannel;

   	DBG_PRINT(">>\r\n");

	if(n < 0 || n > (CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES -1))
	{
		printf("line %d is not defined\r\n", n);
		return;
	}

	if (ch < 0 || ch > 1)
	{
		printf("ch %d is not defined\r\n", ch);
		return;
	}

	dspChannel = lines[n].ch[ch].id;

   	printf ("closing channel RADV n = %d  ch = %d  dspChannel = %d\r\n", n, ch, dspChannel);

	if(dspChannel != -1)
	{
		DBG_PRINT("\r\n");

    		/*EA - break the linkage between the dspCahnnel
    		(absCh in the rtp module and the socket and port that were assigned to it.*/
		acl_rtp_unbind(dspChannel);

		if(lines[n].ch[ch].is_channel_open)
		{
		   	DBG_PRINT("\r\n");

			if(ch == 0)
			{
   				DBG_PRINT("\r\n");

				/* Deactivate (close) the DSP channel RTP stream */
				if(acgDeactivateRTP(dspChannel) != ACG_SUCCESS )
				{
					printf("error - acgDeactivateRTP()\r\n");
					return;
				}

			}
			else	/* ch == 1 */
			{
			   	DBG_PRINT("\r\n");

				/* Close a DSP channel */
				if(acgCloseChannel(dspChannel) != ACG_SUCCESS )
				{
					printf("error - acgCloseChannel()\r\n");
					return;
				}

				lines[n].ch[ch].is_channel_open = 0;
			}

		}
	}

	DBG_PRINT("<<\r\n");

}


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
int dsp_3wc(int n, int confChannel, int mateChannel, int confMode)
{
	int dspConfChannel;
	int dspMateChannel;

   	DBG_PRINT(">>\r\n");


	if(n < 0 || n > (CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES -1))
	{
		printf("line %d is not defined\r\n", n);
		return ACG_FAILURE;
	}

	dspConfChannel = lines[n].ch[mateChannel].conferenceId;
	dspMateChannel = lines[n].ch[confChannel].conferenceId;


   	DBG_PRINT("dspConfChannel = %d  dspMateChannel = %d\r\n", dspConfChannel, dspMateChannel);

	if ((dspConfChannel < 0 || dspConfChannel > (NUMBER_OF_DSP_CHANNELS - 1)) || (dspMateChannel < 0 || dspMateChannel > (NUMBER_OF_DSP_CHANNELS - 1)))
	{
		printf("channel is not defined\r\n");
		return ACG_FAILURE;
	}

	if(confMode == ACG__3_WAY_CONFERENCE_MODE__1)
	{
   		DBG_PRINT("\r\n");

#if defined(AC488_CPE_VOIP_TOOLKIT)

		acgTChannelParameters	tempChannelParameters;

		acl_line_t		*pCurrentLine=NULL;

		/* When system is after reboot, DSP channel is actually a primary channel which is
		 	set to default parameters, the channel must be close and reopen */

		/* Close a DSP channel */
		if(acgCloseChannel(dspMateChannel) != ACG_SUCCESS )
		{
			printf("error - acgCloseChannel()\r\n");
			return ACG_FAILURE;
		}

		lines[n].ch[confChannel].is_channel_open = 0;

		memcpy(&tempChannelParameters, &ChannelParameters[n][confChannel], sizeof(acgTChannelParameters));
		/* The mate channel is configured to DTMF transparent to ignore dtmf coming from TDM to be send as RFC2833 if configured */
		tempChannelParameters.SignalingParameters.DTMFTransportMethod = ACG_SIG_TRANSPORT__TRANSPARENT;

		/* Open a DSP channel */
		if(acgOpenChannel(dspMateChannel, &tempChannelParameters) != ACG_SUCCESS )
		{
			printf("error - acgOpenChannel()\r\n");
			return ACG_FAILURE;
		}

		lines[n].ch[confChannel].is_channel_open = 1;

		/* Activate the DSP channel RTP stream */
		if(acgActivateRTP(dspMateChannel) != ACG_SUCCESS )
		{
			printf("error - acgActivateRTP()\r\n");
			return ACG_FAILURE;
		}

		pCurrentLine = (acl_line_t *)get_line(n);
		if(!pCurrentLine)
			return ACG_FAILURE;

		rtp_add_conf_mate(pCurrentLine->active_call1, dspMateChannel);

#endif

		/* Start 3 way conference */
		if(acgStart3WayConference(dspConfChannel, dspMateChannel) != ACG_SUCCESS )
		{
			printf("error - acgStart3WayConference()\r\n");
			return ACG_FAILURE;
		}

	}
	else	/* confMode == ACG__3_WAY_CONFERENCE_MODE__DISABLE */
	{
	   	DBG_PRINT("\r\n");

#if defined(AC488_CPE_VOIP_TOOLKIT)

		rtp_remove_conf_mate(dspMateChannel);

		/* The mate channel should be reconfigured to the original channel configuration */

		/* Update a DSP channel */
		if(acgUpdateChannel(dspMateChannel, &(ChannelParameters[n][confChannel])) != ACG_SUCCESS )
		{
			printf("error - acgUpdateChannel()\r\n");
			return ACG_FAILURE;
		}


#endif
		/* Stop 3 way conference */
		if(acgStop3WayConference(dspConfChannel, dspMateChannel) != ACG_SUCCESS )
		{
			printf("error - acgStop3WayConference()\r\n");
			return ACG_FAILURE;
		}
	}

	DBG_PRINT("<<\r\n");

	return ACG_SUCCESS;

}


#ifdef CONFIG_RG_ATA

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
void dsp_fax_start(int n, int ch, fax_transport_mode_t fax_transport_mode, int fax_maxrate)
{
	int dspChannel;

	acgEFaxModemTransportMethod 	FaxModemTransportMethod = -1;

	DBG_PRINT(">>\r\n");

	if(n < 0 || n > (CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES -1))
	{
		printf("line %d is not defined\r\n", n);
		return;
	}

	if (ch < 0 || ch > 1)
	{
		printf("ch %d is not defined\r\n", ch);
		return;
	}

	dspChannel = lines[n].ch[ch].id;

	if(dspChannel != -1)
	{
		switch(fax_transport_mode)
		{
			case FAX_TRANSPORT_RELAY:

				if(fax_maxrate != -1)
				{
					if((int)ChannelParameters[n][ch].FaxModemParameters.FaxMaxRate != fax_maxrate)
					{
						DBG_PRINT("Updating FaxMaxRate\r\n");

						ChannelParameters[n][ch].FaxModemParameters.FaxMaxRate = (acgEFaxMaxRate)fax_maxrate;

						/* Update a DSP channel */
						if(acgUpdateChannel(dspChannel, &(ChannelParameters[n][ch])) != ACG_SUCCESS )
						{
							printf("error - acgUpdateChannel()\r\n");
							return;
						}
					}
				}

				FaxModemTransportMethod = ACG_FAX_MODEM_TRANSPORT__RELAY;

				break;

			case FAX_VOICE_BAND_DATA:

				FaxModemTransportMethod = ACG_FAX_MODEM_TRANSPORT__VBD;

				break;

			default:
				printf("Unexpected fax_transport_mode %d\r\n", fax_transport_mode);
				return;

				break;

		}

		DBG_PRINT("acgStartFax %d\r\n", FaxModemTransportMethod);

		/* Start fax */
		if(acgStartFax(dspChannel, FaxModemTransportMethod) != ACG_SUCCESS )
		{
			printf("error - acgStartFax()\r\n");
			return;
		}
	}

	DBG_PRINT("<<\r\n");

}

/******************************************************************************
*	Function:	 dsp_fax_stop
*
*	Description: 	stoping fax (Both for T38 and VBD)
*
*	Parameters:
*
*	Returns:
*
*******************************************************************************/
void dsp_fax_stop(int n, int ch, fax_transport_mode_t fax_transport_mode)
{
	int dspChannel;

	acgEFaxModemTransportMethod FaxModemTransportMethod;

	DBG_PRINT(">>\r\n");

	if(n < 0 || n > (CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES -1))
	{
		printf("line %d is not defined\r\n", n);
		return;
	}

	if (ch < 0 || ch > 1)
	{
		printf("ch %d is not defined\r\n", ch);
		return;
	}

	dspChannel = lines[n].ch[ch].id;

	if(dspChannel != -1)
	{
		switch(fax_transport_mode)
		{
			case FAX_TRANSPORT_RELAY:
				FaxModemTransportMethod = ACG_FAX_MODEM_TRANSPORT__RELAY;
				break;

			case FAX_VOICE_BAND_DATA:
				FaxModemTransportMethod = ACG_FAX_MODEM_TRANSPORT__VBD;
				break;

			default:
				printf("unexpected fax_transport_mode %d\r\n", fax_transport_mode);
				return;

				break;
		}

		DBG_PRINT("acgStopFax::FaxModemTransportMethod %d\r\n", FaxModemTransportMethod);

		/* Stop fax */
		if(acgStopFax(dspChannel, FaxModemTransportMethod) != ACG_SUCCESS )
		{
			printf("error - acgStopFax()\r\n");
			return;
		}
	}

	DBG_PRINT("<<\r\n");

}

#endif
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
/* ACL NB 060626 PATCH  - Work around */
/* Must stop dtmf after closing rtp( because of a DSP bug ) */
void dsp_ibs_stop(int n, int ch)
{
	int dspChannel;

	DBG_PRINT(">>\r\n");

	if(n < 0 || n > (CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES -1))
	{
		printf("line %d is not defined\r\n", n);
		return;
	}

	if (ch < 0 || ch > 1)
	{
		printf("ch %d is not defined\r\n", ch);
		return;
	}

	dspChannel = lines[n].ch[ch].id;

	if(dspChannel != -1)
	{
		/* Stop a tone if already been generated */
		if(acgStopTone(dspChannel) != ACG_SUCCESS )
		{
			printf("error - acgStopTone()\r\n");
			return;
		}
	}
	DBG_PRINT("<<\r\n");

}



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
void dsp_vmwi(int n, int activate)
{
#ifdef CONFIG_RG_ATA
	int dspChannel;
	acgECallerIdServiceType CallerIdServiceType;

	DBG_PRINT(">>\r\n");

	if(n < 0 || n > (CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES -1))
	{
		printf("line %d is not defined\r\n", n);
		return;
	}

	dspChannel = lines[n].ch[0].id;

	if(dspChannel != -1)
	{
		if (Is_MWI_Supported(ChannelParameters[n][0].CallerIDParameters.CallerIdStandard) == 0)
		{
			printf("(Caller Id Standard=%d) MWI is not supported. \n", ChannelParameters[n][0].CallerIDParameters.CallerIdStandard);
			return;
		}

		/* VMWI can't be called during voice session */
		CallerIdServiceType = get_hook_state(n) ? AGC_CALLER_ID_SERVICE_TYPE__2_OFF_HOOK : AGC_CALLER_ID_SERVICE_TYPE__1_ON_HOOK;

		if (activate)
		{
			/* Play MWI */
			if(acgPlayMWI(dspChannel, CallerIdServiceType) != ACG_SUCCESS )
			{
				printf("error - acgPlayMWI()\r\n");
				return;
			}
		}
		else
		{
			/* Stop MWI */
			if(acgStopMWI(dspChannel, CallerIdServiceType) != ACG_SUCCESS )
			{
				printf("error - acgStopMWI()\r\n");
				return;
			}
		}
	}

	DBG_PRINT("<<\r\n");
#endif

}



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
void dsp_play(int n, voip_signal_t s, int start)
{
	int 			dspChannel;
	acgTTone 	Tone;

	static AC_CODE2CODE_MAP tones[] = {
		{ VOIP_SIGNAL_DIAL_TONE, 			DSP_REGIONAL_SETTINGS_CALL_PROGRESS_DIAL_TONE },
		{ VOIP_SIGNAL_BUSY_TONE, 			DSP_REGIONAL_SETTINGS_CALL_PROGRESS_BUSY_TONE },
		{ VOIP_SIGNAL_RING_TONE, 			DSP_REGIONAL_SETTINGS_CALL_PROGRESS_RINGBACK_TONE },
		{ VOIP_SIGNAL_WARNING_TONE, 		DSP_REGIONAL_SETTINGS_CALL_PROGRESS_REORDER_TONE },
		{ VOIP_SIGNAL_HOOK_OFF_TONE, 		DSP_REGIONAL_SETTINGS_CALL_PROGRESS_HOWLER_TONE },
		{ VOIP_SIGNAL_REORDER_TONE, 		DSP_REGIONAL_SETTINGS_CALL_PROGRESS_REORDER_TONE },
		{ VOIP_SIGNAL_CALLER_WAITING_TONE, 	DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_TONE},                    /* ACL NB 0600607*/
		{ VOIP_SIGNAL_CALLER_WAITING_RINGBACK_TONE, DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_RINGBACK_TONE }, /* ACL NB 0600607*/
		{ VOIP_SIGNAL_STUTTER_TONE, DSP_REGIONAL_SETTINGS_CALL_PROGRESS_STUTTER_TONE }, /* ACL NB 070102*/
		{ VOIP_SIGNAL_CALLER_WAITING_TONE_2, DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_TONE_2},                /* ACL NB 070909*/
		{ -1, -1 }
	};

	DBG_PRINT(">> n=%d,signal=%d,start=%d\r\n",n,s,start);

	if(n < 0 || n > (CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES -1))
	{
		printf("line %d is not defined\r\n", n);
		return;
	}

	memset(&Tone, 0, sizeof(Tone));

	dspChannel = lines[n].ch[0].id;

	if(dspChannel != -1)
	{

		if (s == -1)
		{
			printf("Unknown signal %d\n", s);
			return;
		}

		Tone.ToneElement.Type = ACG_TONE_TYPE__CALL_PROGRESS_TONE;
		Tone.ToneElement.ToneIndex = AC_CODE_TO_VALUE(tones, s);

		Tone.Direction = ACG_DIRECTION__TO_LOCAL_TDM;
		Tone.OnDuration = -1;

		if (start)
		{
			/* Play a call progerss tone */
			if(acgPlayTone(dspChannel, &Tone) != ACG_SUCCESS )
			{
				printf("error - acgPlayTone()\r\n");
				return;
			}
		}
		else
		{
			/* Stop a tone if already been generated */
			if(acgStopTone(dspChannel) != ACG_SUCCESS )
			{
				printf("error - acgStopTone()\r\n");
				return;
			}
		}

	}

	DBG_PRINT("<<\r\n");

}

int acgPlayTone_phi(int ch, int tone_index,int flag,int duration)
{
	acgTTone 	Tone;

	int 			dspChannel = -1;

	memset(&Tone, 0, sizeof(Tone));
	int onduration;
	if(duration>0)
	{
        onduration=-1;
	}
	else
        onduration=300;

	dspChannel = ch;//atoi(argv[1]);
//acIPPSetAudioDevice_gw(1,AC494_CODEC_SPEAKER);

	if(isValidDSPChannel(dspChannel))
	{

        printf("Dsp channel is valid %d \n",dspChannel);
		Tone.Direction = (acgEDirection) 0;//atoi(argv[2]);
		Tone.OnDuration = onduration;//atoi(argv[3]);
		Tone.OffDuration = 0;//atoi(argv[4]);
		Tone.ToneElement.Type = 1;//(acgEToneType)atoi(argv[5]);
		Tone.ToneElement.ToneIndex = tone_index;//atoi(argv[6]);

		/* Play a tone towards the local TDM side or network side */

    if(flag){
        printf("flag is set \n");
		if(acgPlayTone(dspChannel, &Tone) != ACG_SUCCESS )
		{
			printf("error - acgDeactivateRTP()\r\n");
			return (-1);
		}
    }else{

        if(acgStopTone(dspChannel) != ACG_SUCCESS )
		{
			printf("error - acgDeactivateRTP()\r\n");
			return (-1);
		}

    }



	}
	else
	{
		printf("Invalid DSP channel %d\r\n", dspChannel);
		return (-1);
	}

	return 0;

}






#ifdef CONFIG_RG_ATA
/******************************************************************************
*	Function:	 dsp_ring_ATA
*
*	Description: 	Play a tone towards the local TDM and set caller id timer
*
*	Parameters:
*
*	Returns:
*
*******************************************************************************/
void dsp_ring_ATA(int n, int start)
{
	int 				dspChannel;

	DBG_PRINT(">>\r\n");

	if(n < 0 || n > (CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES -1))
	{
		printf("line %d is not defined\r\n", n);
		return;
	}

	dspChannel = lines[n].ch[0].id;

	DBG_PRINT("dspChannel = %d  n = %d\r\n", dspChannel, n);

	if(dspChannel != -1)
	{
		if(start)
		{
			/* AlexChen update cadence */
			acgUpdateCadence(caller_id[n].distRingType, n);
			//acgUpdateCadence(2, n);
			/* Start ring on a FXS port */
#if defined (AC49_SMB_VOIP_TOOLKIT) || defined(AC494_TULIP2_40) || defined(AC49_ORCHID_ATA_VOIP_TOOLKIT)
			if(acgStartRing(n) != ACG_SUCCESS)
#else
			if(acgStartRing(dspChannel) != ACG_SUCCESS )
#endif
			{
				printf("error - acgStartRing()\r\n");
				return;
			}

			AC_TIMER_SET(CALLER_ID_DELAY, callerIdCB, (void *)&(lines[n].ch[0].id));

			lines[n].is_caller_id_timer_active = 1;
		}
		else
		{
			if (lines[n].is_caller_id_timer_active)
			{
				lines[n].is_caller_id_timer_active = 0;

				if (AC_TIMER_EXISTS(callerIdCB, (void *)&(lines[n].ch[0].id)))
				{
					AC_TIMER_DEL(callerIdCB, (void *)&(lines[n].ch[0].id));
				}

				/* Stop caller Id generation if already started */
				if(acgStopCallerID(dspChannel) != ACG_SUCCESS )
				{
					printf("error - acgStopCallerID()\r\n");
				}

			}

			/* Stop ring on a FXS port */
#if defined (AC49_SMB_VOIP_TOOLKIT) || defined(AC494_TULIP2_40) || defined(AC49_ORCHID_ATA_VOIP_TOOLKIT)
			if(acgStopRing(n) != ACG_SUCCESS)
#else
			if(acgStopRing(dspChannel) != ACG_SUCCESS )
#endif
			{
				printf("error - acgStopRing()\r\n");
				return;
			}
		}

	}

	DBG_PRINT("<<\r\n");

}
#endif /*CONFIG_RG_ATA*/



#ifdef CONFIG_RG_IPPHONE
/******************************************************************************
*	Function:	 dsp_ring_IPP
*
*	Description: 	Ringing on IPP
*
*	Parameters:
*
*	Returns:
*
*******************************************************************************/
 void dsp_Ring_IPP(int n , int start)
 {
 	if (start)
		acIPPStartRing();
	else
		acIPPStopRing();
 }
#endif /*CONFIG_RG_IPPHONE*/

/******************************************************************************
*	Function:	 dsp_ring
*
*	Description: 	Adaptation layer for ringing of ATA/IPP
*
*	Parameters:
*
*	Returns:
*
*******************************************************************************/

void dsp_ring(int n, int start)
{
	DBG_PRINT(">> n=%d, start=%d\r\n",n,start);

#ifdef CONFIG_RG_ATA
	dsp_ring_ATA(n,start);
#else
//	dsp_Ring_IPP(n,start);
#endif
}


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
void dsp_call_waiting_alert(int n)
{
#ifdef CONFIG_RG_ATA
	int 				dspChannel;
	acgTCallerId 		CallerId;

	DBG_PRINT(">>\r\n");

	if(n < 0 || n > (CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES -1))
	{
		printf("line %d is not defined\r\n", n);
		return;
	}

	dspChannel = lines[n].ch[0].id;

	if(dspChannel != -1)
	{

		memset(&CallerId, 0, sizeof(CallerId));

		if (!lines[n].is_caller_id_timer_active)
		{
			CallerId.CallerIdServiceType = AGC_CALLER_ID_SERVICE_TYPE__2_OFF_HOOK;

			//NIRP 070218 in case of caller id type 2 - make sure that configured standard supports CID type 2
			if ( !IsCallerIDType2Supported(ChannelParameters[n][0].CallerIDParameters.CallerIdStandard))
			{
				printf("generate_caller_id: (standard=%d) caller id type 2 is not supported. \n", ChannelParameters[n][0].CallerIDParameters.CallerIdStandard);
				return;
			}

		//   	snprintf(CallerId.time, TIME_STRING_LEN, "%.2d%.2d%.2d%.2d", localtime_validate.tm_mon + 1,
		//	         localtime_validate.tm_mday, localtime_validate.tm_hour, localtime_validate.tm_min);

			AC_STRNCPY(CallerId.Number, caller_id[n].number, MAX_DIAL_STRING_LEN - 1);
			AC_STRNCPY(CallerId.Name, caller_id[n].name, MAX_CALLER_ID_LEN - 1);
			AC_STRNCPY(CallerId.Addr, caller_id[n].addr, AC_DOTTED_IP_LEN - 1);

			/* TODO: When caller is anonymous, indicate it in number_status and
			 * name_status */
			CallerId.NumberStatus = ACG_PHONE_CALLER_NAME_PRESENT;
			CallerId.NameStatus = ACG_PHONE_CALLER_NAME_PRESENT;

			if (!IsDTMF_CallerID(ChannelParameters[n][0].CallerIDParameters.CallerIdStandard))
			{
				if(acgPlayCallerID(dspChannel, &CallerId) != ACG_SUCCESS )
				{
					printf("error - acgPlayCallerID()\r\n");
					return;
				}
			}
			else
			{
				printf("CallerIdStandard %d is not supporting DTMF caller Id\r\n", ChannelParameters[n][0].CallerIDParameters.CallerIdStandard);
			}

		}
		else
		{
			printf("CallerId timer is already active\r\n");
		}

	}

	DBG_PRINT("<<\r\n");

#endif

}



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
void dsp_dtmf(int n, int key, int pressed, int dur) /* ACL NB 060720 - added duration */
{
#ifdef CONFIG_RG_ATA

	int 			dspChannel;
	acgTTone 	Tone;

	DBG_PRINT(">>\r\n");

	if(n < 0 || n > (CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES -1))
	{
		printf("line %d is not defined\r\n", n);
		return;
	}

	memset(&Tone, 0, sizeof(Tone));

	dspChannel = lines[n].ch[0].id;

	if(dspChannel != -1)
	{

		if (pressed)
		{
			Tone.ToneElement.Type = ACG_TONE_TYPE__DTMF;
			Tone.ToneElement.ToneIndex = key;

			Tone.Direction = ACG_DIRECTION__TO_LOCAL_TDM;

			if (dur != 0 )
				Tone.OnDuration = dur;
			else
				Tone.OnDuration = -1;	/* Play tone forever */

			/* Play DTMF */
			if(acgPlayTone(dspChannel, &Tone) != ACG_SUCCESS )
			{
				printf("error - acgPlayTone()\r\n");
				return;
			}

		}
		else
		{
			/* Stop a tone if already been generated */
			if(acgStopTone(dspChannel) != ACG_SUCCESS )
			{
				printf("error - acgStopTone()\r\n");
				return;
			}
		}
	}

	DBG_PRINT("<<\r\n");
#endif
}



/************************************/
/***********  Stub Functions  ***********/
/************************************/

int dsp_adaptation_layer_send_general_command(void *data)
{
	return 0;
}

int dsp_send_daa_commands(void *data)
{
    dsp_send_daa_commands_args_t * p_dsp_send_daa_commands_args = data;
    int daa_command = p_dsp_send_daa_commands_args->command_enum;
//    int daa_channel = p_dsp_send_daa_commands_args->channel;
    int daa_line = p_dsp_send_daa_commands_args->line;

    switch(daa_command)
    {
    case  DSP_DAA_COMMANDS_CHANGE_HOOK_STATE_TO_OFF_HOOK:
        acgSeizeLine(daa_line);
        break;
    case  DSP_DAA_COMMANDS_CHANGE_HOOK_STATE_TO_ON_HOOK:
        acgReleaseLine(daa_line);
        break;
    case DSP_DAA_COMMANDS_FLASH_HOOK:
        acgGenerateFlash(daa_line);
        break;
    default:
        break;
    }
    return 0;
}

int dsp_msg_log(sip_log_message_t *logmsg)
{
	logmsg->message[logmsg->size] = (char)0;

	printf("\n%s\r\n", logmsg->message);
	return 0;
}

int keypad_open(int n)
{
    return 0;
}

int keypad_close(int n)
{
    return 0;
}

void dsp_get_info(int line, voip_state_t *state)
{

}

int handle_debug_cmd(void *arg, int DebugType)
{
	return 0;
}

/************************************/
/**********  Internal Functions **********/
/************************************/
#if defined(B_CEVA_PLATFORM)

#define CHUNK_SIZE 256

#define CONDITIONALLY_WRITE_CHUNK() 																\
	{                                                                                           \
		if (data_chunk_cnt == CHUNK_SIZE) 														\
		{                                    													\
			pInitializationParams->PrimaryDSPDeviceInformation.NumberOfDevices = 1;				\
			pInitializationParams->PrimaryDSPDeviceInformation.pProgram = data_chunk;			\
			pInitializationParams->PrimaryDSPDeviceInformation.ProgramSize = CHUNK_SIZE;		\
			if(acgInitializeDSP(pInitializationParams) != ACG_SUCCESS )							\
			{                                                                                   \
				printf("error - acgInitializeDSP()\r\n");										\
				return;                                   										\
			}                                              										\
			data_chunk_cnt = 0;                             									\
			memset(data_chunk, 0x0, CHUNK_SIZE);               									\
		}																						\
	}


void initDsp(acgTInitializationParameters  *pInitializationParams)
{
	FILE *pBinaryFd;
	char file_name[100] = {0};
	char data_chunk[CHUNK_SIZE] = {0};
	int data_chunk_cnt = 0;
        int curr_char, prev_char;
        unsigned char count;

	strcpy(file_name,"/audiocodes/binary/ceva.fwb");

	if(!(pBinaryFd= fopen(file_name,"r")))
	{
		printf("error open file %s", file_name);
		exit(-1);
	}

	prev_char = EOF;
    	while ((curr_char = fgetc(pBinaryFd)) != EOF)
	{
		data_chunk[data_chunk_cnt++] = curr_char;
		CONDITIONALLY_WRITE_CHUNK();
        if (curr_char == prev_char)
	{
		count = fgetc(pBinaryFd);
		while (count > 0)
		{
			data_chunk[data_chunk_cnt++] = curr_char;
			count--;
			CONDITIONALLY_WRITE_CHUNK();
		}
		prev_char = EOF;
	}
        else
            	prev_char = curr_char;

		CONDITIONALLY_WRITE_CHUNK();
  	} //end while


    	if(fclose(pBinaryFd) == -1)
	{
		printf("error closing file %s", file_name);
		return;
	}

	/*end of firmware downloading*/
	pInitializationParams->PrimaryDSPDeviceInformation.NumberOfDevices = 1;
	pInitializationParams->PrimaryDSPDeviceInformation.pProgram = NULL;
	pInitializationParams->PrimaryDSPDeviceInformation.ProgramSize = 0;

	if(acgInitializeDSP(pInitializationParams) != ACG_SUCCESS )
	{
		printf("error - acgInitializeDSP()\r\n");
		return;
	}

}

#endif /* B_CEVA_PLATFORM */

static int SetChannelParams(int n, int ch, int payload, int ptime,
						int dtmf_method, int rtp_context, int faxDataMethod)
{

	int 	deafult_payload_value;
	int 	rtp_fd;
    //	int 	g723_high_bitrate;

	DBG_PRINT(">>\r\n");

	rtp_fd = ((rtp_session_t *)rtp_context)->fd;

    //	g723_high_bitrate = (payload&ACL_723_LOW_BITRATE_BIT) ? 0:1;

//	payload&=~ACL_723_BITRATE_MASK;	/* need to re-mask payload */
    //payload=9;
	DBG_PRINT("\npayload = %d\n", payload);

	if(payload < FIRST_DYNAMIC_PAYLOAD)
	{

		if(RTP_PAYLOAD_G723 == payload)
		{
		//	ChannelParameters[n][ch].RTPStreamParameters.Coder = g723_high_bitrate ? ACG_CODER__G723HIGH : ACG_CODER__G723LOW;
		}
		else if (RTP_PAYLOAD_G722 == payload)
		{
			if (codec_get_by_payload(payload))
			{
				switch (64){//codec_get_by_payload(payload)->g722_bit_rate){

					case 48:
						ChannelParameters[n][ch].RTPStreamParameters.Coder = ACG_CODER__G722_48K;
						break;
					case 56:
						ChannelParameters[n][ch].RTPStreamParameters.Coder = ACG_CODER__G722_56K;
						break;
					case 64:
						ChannelParameters[n][ch].RTPStreamParameters.Coder = ACG_CODER__G722_64K;
						break;
					default:
						printf("\nG.722 bitrate % is not supported", codec_get_by_payload(payload)->g722_bit_rate);
						return ACG_FAILURE;
				}
			}
			else
			{
				return ACG_FAILURE;
			}
		}
		else
		{
			ChannelParameters[n][ch].RTPStreamParameters.Coder = appl_code2code(payload2coder, payload);
		}

		ChannelParameters[n][ch].RTPStreamParameters.TxVoicePayloadType = payload;
		ChannelParameters[n][ch].RTPStreamParameters.RxVoicePayloadType = payload;

	}
	else	/* Dynamic payload type */
	{
		#warning "dynamic payload type should be handled - the below implementation is not finished"

		deafult_payload_value = payload_type_id_to_enum(payload);
		if(deafult_payload_value == -1)
			return ACG_FAILURE;

		switch(deafult_payload_value)
		{
			case RTP_PAYLOAD_G726_16:
				ChannelParameters[n][ch].RTPStreamParameters.Coder = ACG_CODER__G726_16;
				break;

			default:
				printf("\n%s line %d unsupported dynamic payload number %d\r\n", __FUNCTION__, __LINE__, deafult_payload_value);
				break;
		}

		get_remote_current_codec_payload_number(rtp_fd, &(ChannelParameters[n][ch].RTPStreamParameters.TxVoicePayloadType));
		ChannelParameters[n][ch].RTPStreamParameters.RxVoicePayloadType = payload;
	}


	DBG_PRINT("\nCoder = %d on line %d  ch = %d\n", ChannelParameters[n][ch].RTPStreamParameters.Coder, n, ch);

	ChannelParameters[n][ch].RTPStreamParameters.PTime = ptime;

	switch (dtmf_method)
	{
		case -1:		/* N/A */
			/***  Do Nothing ***/
		  	printf("\n%s line %d dtmf_method=N/A\r\n", __FUNCTION__, __LINE__);
		  	break;

		case DTMF_INBAND:
			ChannelParameters[n][ch].SignalingParameters.DTMFTransportMethod = ACG_SIG_TRANSPORT__TRANSPARENT;
			break;

		case DTMF_RFC2833:
			ChannelParameters[n][ch].SignalingParameters.DTMFTransportMethod = ACG_SIG_TRANSPORT__RELAY;
			get_remote_dtmf_payload_number(rtp_fd, &(ChannelParameters[n][ch].RTPStreamParameters.TxDTMFRelayPayloadType));
			break;
		case DTMF_VIA_SIP:
			ChannelParameters[n][ch].SignalingParameters.DTMFTransportMethod = ACG_SIG_TRANSPORT__VOICE_MUTE;
			break;

		default:
			DBG_PRINT("\ndtmf method unknown %d\r\n", dtmf_method);
			break;
	}


	switch(faxDataMethod & 0x0F )	/* Fax Configuration */
	{
		case -1:		/* N/A */
			/***  Do Nothing ***/
		  	printf("\n%s line %d faxMethod=N/A\r\n", __FUNCTION__, __LINE__);
		  	break;

		case FAX_TRANSPORT_DISABLE:		/* Fax transparent */
			ChannelParameters[n][ch].FaxModemParameters.FaxTransferMethod = ACG_FAX_MODEM_TRANSPORT__DISABLE;
		  	break;

		case FAX_TRANSPORT_RELAY: 		/* T.38 */
			ChannelParameters[n][ch].FaxModemParameters.FaxTransferMethod = ACG_FAX_MODEM_TRANSPORT__RELAY;
		  	break;

		case FAX_VOICE_BAND_DATA: 		/* voice band data : bypass with fax payload type g711 and SIP support ( re-invite )*/
			ChannelParameters[n][ch].FaxModemParameters.FaxTransferMethod = ACG_FAX_MODEM_TRANSPORT__VBD;
		  	break;

		case FAX_TRANSPORT_BYPASS:		/* Fax bypass */
			ChannelParameters[n][ch].FaxModemParameters.FaxTransferMethod = ACG_FAX_MODEM_TRANSPORT__BYPASS;
		  	break;

		default:
		  	DBG_PRINT("\nfax Method unknown %d\r\n", faxDataMethod & 0x0F);
			break;
	}


	switch(faxDataMethod & 0xF0 )	/* Modem Configuration */
   	{
		case DATA_TRANSPORT_DISABLE:	/* Modem transparent */
			ChannelParameters[n][ch].FaxModemParameters.ModemTransferMethod = ACG_FAX_MODEM_TRANSPORT__DISABLE;
		 	break;

		case DATA_VOICE_BAND_DATA :	 /* voice band data : bypass with fax payload type g711 and SIP support ( re-invite )*/
			ChannelParameters[n][ch].FaxModemParameters.ModemTransferMethod = ACG_FAX_MODEM_TRANSPORT__VBD;
		  	break;

		case DATA_TRANSPORT_BYPASS:	/* Modem bypass */
			ChannelParameters[n][ch].FaxModemParameters.ModemTransferMethod = ACG_FAX_MODEM_TRANSPORT__BYPASS;
		  	break;

		default:/* default - can be -1 this means N/A - do nothing */
		  	DBG_PRINT("\n%s line %d modem Method unknown %d\r\n", __FUNCTION__, __LINE__, faxDataMethod & 0xF0);
		  	break;
   	}

	/* If FaxTransferMethod is configures as T.38, ModemTransferMethod can be configured as
		any method. If FaxTransferMethod is not configured as T.38, FaxTransferMethod and
		ModemTransferMethod must be the same */

	switch(ChannelParameters[n][ch].FaxModemParameters.FaxTransferMethod)
	{
		case ACG_FAX_MODEM_TRANSPORT__VBD:

			if(ChannelParameters[n][ch].FaxModemParameters.ModemTransferMethod == ACG_FAX_MODEM_TRANSPORT__VBD)
			{
				switch(ChannelParameters[n][ch].FaxModemParameters.BypassCoder)
				{
					case ACG_CODER__G711ALAW:
						ChannelParameters[n][ch].RTPStreamParameters.FaxBypassPayloadType = 8;
						ChannelParameters[n][ch].RTPStreamParameters.ModemByassPayloadType = 8;
						break;
					case ACG_CODER__G711MULAW:
						ChannelParameters[n][ch].RTPStreamParameters.FaxBypassPayloadType = 0;
						ChannelParameters[n][ch].RTPStreamParameters.ModemByassPayloadType = 0;
						break;
					default:
					  	printf("\n%s line %d illegal  BypassCoder %d\r\n", __FUNCTION__, __LINE__, ChannelParameters[n][ch].FaxModemParameters.BypassCoder);
						return ACG_FAILURE;
						break;
				}
			}
			else
			{
		  		printf("\n%s line %d ModemTransferMethod %d must be ACG_FAX_MODEM_TRANSPORT__VBD\r\n", __FUNCTION__, __LINE__, ChannelParameters[n][ch].FaxModemParameters.ModemTransferMethod);
				return ACG_FAILURE;
			}

			break;

		case ACG_FAX_MODEM_TRANSPORT__BYPASS:

			if(ChannelParameters[n][ch].FaxModemParameters.ModemTransferMethod == ACG_FAX_MODEM_TRANSPORT__BYPASS)
			{
				if((ChannelParameters[n][ch].RTPStreamParameters.FaxBypassPayloadType >= FIRST_DYNAMIC_PAYLOAD) && (ChannelParameters[n][ch].RTPStreamParameters.FaxBypassPayloadType <= LAST_DYNAMIC_PAYLOAD))
				{
					if((ChannelParameters[n][ch].RTPStreamParameters.ModemByassPayloadType >= FIRST_DYNAMIC_PAYLOAD) && (ChannelParameters[n][ch].RTPStreamParameters.ModemByassPayloadType <= LAST_DYNAMIC_PAYLOAD))
					{
						switch(ChannelParameters[n][ch].FaxModemParameters.BypassCoder)
						{
							case ACG_CODER__G711ALAW:
								break;
							case ACG_CODER__G711MULAW:
								break;
							default:
							  	printf("\n%s line %d illegal  BypassCoder %d\r\n", __FUNCTION__, __LINE__, ChannelParameters[n][ch].FaxModemParameters.BypassCoder);
								return ACG_FAILURE;
								break;
						}
					}
					else
					{
						printf("\n%s line %d ModemByassPayloadType %d - must be netween %d to %d\r\n", __FUNCTION__, __LINE__,
								ChannelParameters[n][ch].RTPStreamParameters.ModemByassPayloadType,
								FIRST_DYNAMIC_PAYLOAD, LAST_DYNAMIC_PAYLOAD);

						return ACG_FAILURE;
					}
				}
				else
				{
					printf("\n%s line %d FaxBypassPayloadType %d - must be netween %d to %d\r\n", __FUNCTION__, __LINE__,
							ChannelParameters[n][ch].RTPStreamParameters.FaxBypassPayloadType,
							FIRST_DYNAMIC_PAYLOAD, LAST_DYNAMIC_PAYLOAD);

					return ACG_FAILURE;
				}
			}
			else
			{
		  		printf("\n%s line %d ModemTransferMethod %d must be ACG_FAX_MODEM_TRANSPORT__BYPASS\r\n", __FUNCTION__, __LINE__, ChannelParameters[n][ch].FaxModemParameters.ModemTransferMethod);
				return ACG_FAILURE;
			}

			break;

		case ACG_FAX_MODEM_TRANSPORT__DISABLE:

			if(ChannelParameters[n][ch].FaxModemParameters.ModemTransferMethod != ACG_FAX_MODEM_TRANSPORT__DISABLE)
		  		printf("\n%s line %d ModemTransferMethod %d must be ACG_FAX_MODEM_TRANSPORT__DISABLE\r\n", __FUNCTION__, __LINE__, ChannelParameters[n][ch].FaxModemParameters.ModemTransferMethod);
				return ACG_FAILURE;

			break;

		default:
			break;
	}

	DBG_PRINT("<<\r\n");

	return ACG_SUCCESS;

}

static int SetChannelParamsNoCall(int n, int ch, int payload, int ptime, int dtmf_method,int payloadDTMF)
{

	int 	deafult_payload_value;
	int 	rtp_fd;
    int 	g723_high_bitrate;

	printf(">>\r\n");

    g723_high_bitrate = (payload&ACL_723_LOW_BITRATE_BIT) ? 0:1;

	payload&=~ACL_723_BITRATE_MASK;	/* need to re-mask payload */

	printf("\n payload = %d\n", payload);

	if(payload < FIRST_DYNAMIC_PAYLOAD)
	{

		if(RTP_PAYLOAD_G723 == payload)
		{
			ChannelParameters[n][ch].RTPStreamParameters.Coder = g723_high_bitrate ? ACG_CODER__G723HIGH : ACG_CODER__G723LOW;
		}
		else if (RTP_PAYLOAD_G722 == payload)
		{
			if (codec_get_by_payload(payload))
			{
				switch(64){

					case 48:
						ChannelParameters[n][ch].RTPStreamParameters.Coder = ACG_CODER__G722_48K;
						break;
					case 56:
						ChannelParameters[n][ch].RTPStreamParameters.Coder = ACG_CODER__G722_56K;
						break;
					case 64:
						ChannelParameters[n][ch].RTPStreamParameters.Coder = ACG_CODER__G722_64K;
						break;
					default:
                        printf("\nG.722 bitrate % is not supported", codec_get_by_payload(payload)->g722_bit_rate);
						return ACG_FAILURE;
				}
			}
			else
			{
				return ACG_FAILURE;
			}
		}
		else
		{
			ChannelParameters[n][ch].RTPStreamParameters.Coder = appl_code2code(payload2coder, payload);
		}

		ChannelParameters[n][ch].RTPStreamParameters.TxVoicePayloadType = payload;
		ChannelParameters[n][ch].RTPStreamParameters.RxVoicePayloadType = payload;

	}
	else	/* Dynamic payload type */
	{
		#warning "dynamic payload type should be handled - the below implementation is not finished"

		deafult_payload_value = payload_type_id_to_enum(payload);
		if(deafult_payload_value == -1)
			return ACG_FAILURE;

		switch(deafult_payload_value)
		{
			case RTP_PAYLOAD_G726_16:
				ChannelParameters[n][ch].RTPStreamParameters.Coder = ACG_CODER__G726_16;
				break;

			default:
				printf("\n%s line %d unsupported dynamic payload number %d\r\n", __FUNCTION__, __LINE__, deafult_payload_value);
				break;
		}
		ChannelParameters[n][ch].RTPStreamParameters.RxVoicePayloadType = payload;
	}


	printf("\nCoder = %d on line %d  ch = %d\n", ChannelParameters[n][ch].RTPStreamParameters.Coder, n, ch);

	ChannelParameters[n][ch].RTPStreamParameters.PTime = ptime;

	switch (dtmf_method)
	{
		case -1:		/* N/A */
			/***  Do Nothing ***/
		  	printf("\n%s line %d dtmf_method=N/A\r\n", __FUNCTION__, __LINE__);
		  	break;

		case DTMF_INBAND:
			ChannelParameters[n][ch].SignalingParameters.DTMFTransportMethod = ACG_SIG_TRANSPORT__TRANSPARENT;
			break;

		case DTMF_RFC2833:
			ChannelParameters[n][ch].SignalingParameters.DTMFTransportMethod = ACG_SIG_TRANSPORT__RELAY;
			ChannelParameters[n][ch].RTPStreamParameters.TxDTMFRelayPayloadType = payloadDTMF;
			break;
		case DTMF_VIA_SIP:
			ChannelParameters[n][ch].SignalingParameters.DTMFTransportMethod = ACG_SIG_TRANSPORT__VOICE_MUTE;
			break;

		default:
			DBG_PRINT("\ndtmf method unknown %d\r\n", dtmf_method);
			break;
	}

	DBG_PRINT("<<\r\n");

	return ACG_SUCCESS;

}
int setchannelparamters_Phi(int ch,int payload, int ptime, int dtmf_inband,int dtmfplayload)
{
	int dspChannel;
    int n = 0;

   	DBG_PRINT(">>\r\n");

	dspChannel = ch;
	/*  In order to configure a channel with the desired ptime and codecs, it
     		needs to be closed and re-opened with the desired arguments  */

   	printf("n = %d  ch = %d  dspChannel = %d\r\n", n, ch, dspChannel);

	SetChannelParamsNoCall(n, ch, payload, ptime, dtmf_inband,dtmfplayload);

	if(dspChannel != -1)
	{
		if(lines[n].ch[ch].is_channel_open)
		{
            printf("----------inside is_channel_open\n");
   			DBG_PRINT("\r\n");

			/* Close a DSP channel */
			if(acgCloseChannel(dspChannel) != ACG_SUCCESS )
			{
				printf("error - acgCloseChannel()\r\n");
				return ACG_FAILURE;
			}

			lines[n].ch[ch].is_channel_open = 0;

		}

   		DBG_PRINT("\r\n");

		/* Open a DSP channel */
		printf("Channel paameter[][] %d \n",ChannelParameters[n][ch]);
		if(acgOpenChannel(dspChannel, &(ChannelParameters[n][ch])) != ACG_SUCCESS )
		{
			printf("error - acgOpenChannel()\r\n");
			return ACG_FAILURE;
		}

		lines[n].ch[ch].is_channel_open = 1;

		/* Activate the DSP channel RTP stream */

		if(acgActivateRTP(dspChannel) != ACG_SUCCESS )
		{
			printf("error - acgActivateRTP()\r\n");
			return ACG_FAILURE;
		}
	}

	DBG_PRINT("<<\r\n");

	return ACG_SUCCESS;

}






static int SetAdvancedChannelParams(voip_dsp_advanced_channel_configuration_t* p_voip_dsp_advanced_channel_configuration,
								regional_settings_params_t* p_regional_settings_params)
{
	int	LineIndex, VoiceChannelIndex;

	DBG_PRINT(">>\r\n");

	for(LineIndex=0; LineIndex < CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES ; LineIndex++)
	{
		for(VoiceChannelIndex=0; VoiceChannelIndex < NUMBER_OF_CHANNELS_PER_LINE; VoiceChannelIndex++)
		{
			ChannelParameters[LineIndex][VoiceChannelIndex].CallerIDParameters.CallerIdStandard = p_regional_settings_params->dsp_regional_settings_params.CallerIDStandard;

			ChannelParameters[LineIndex][VoiceChannelIndex].GainParameters.AgcEnable = p_voip_dsp_advanced_channel_configuration->AutomaticAgcEnable;
			ChannelParameters[LineIndex][VoiceChannelIndex].GainParameters.AgcLocation = p_voip_dsp_advanced_channel_configuration->AgcLocation;
			ChannelParameters[LineIndex][VoiceChannelIndex].GainParameters.AgcTargetEnergy = p_voip_dsp_advanced_channel_configuration->AgcTargetEnergy;
			ChannelParameters[LineIndex][VoiceChannelIndex].GainParameters.VoiceOutputGain = p_voip_dsp_advanced_channel_configuration->VoiceOutputGainChannelArr[LineIndex]+ ACG_VOICE_GAIN__0_DB;	/* need to match acgEVoiceGain */

			ChannelParameters[LineIndex][VoiceChannelIndex].JitterBufferParameters.VoiceJitterBufferMinDelay = p_voip_dsp_advanced_channel_configuration->VoiceJitterBufferMinDelay;
			ChannelParameters[LineIndex][VoiceChannelIndex].JitterBufferParameters.DynamicJitterBufferOptimizationFactor = p_voip_dsp_advanced_channel_configuration->DynamicJitterBufferOptimizationFactor;

			ChannelParameters[LineIndex][VoiceChannelIndex].EchoCancellerParameters.EchoCanceler = p_voip_dsp_advanced_channel_configuration->EchoCancelerMode;

			ChannelParameters[LineIndex][VoiceChannelIndex].VoiceQualityMonitoringParameters.RtcpXrEnable = p_voip_dsp_advanced_channel_configuration->RtcpXrEnable[LineIndex];

			ChannelParameters[LineIndex][VoiceChannelIndex].FaxModemParameters.CngRelay = p_voip_dsp_advanced_channel_configuration->CngRelayEnable;
			ChannelParameters[LineIndex][VoiceChannelIndex].FaxModemParameters.FaxMaxRate = p_voip_dsp_advanced_channel_configuration->FaxMaxRate;
			ChannelParameters[LineIndex][VoiceChannelIndex].FaxModemParameters.FaxErrorCorrection = p_voip_dsp_advanced_channel_configuration->FaxErrorCorrectionMode;
			ChannelParameters[LineIndex][VoiceChannelIndex].FaxModemParameters.BypassCoder = appl_code2code(payload2coder, p_voip_dsp_advanced_channel_configuration->bypassCoder);

			ChannelParameters[LineIndex][VoiceChannelIndex].RTPStreamParameters.FaxBypassPayloadType = p_voip_dsp_advanced_channel_configuration->faxBypassPayloadType;
			ChannelParameters[LineIndex][VoiceChannelIndex].RTPStreamParameters.ModemByassPayloadType = p_voip_dsp_advanced_channel_configuration->dataBypassPayloadType;
			ChannelParameters[LineIndex][VoiceChannelIndex].RTPStreamParameters.SilenceCompressionMode = p_voip_dsp_advanced_channel_configuration->SilenceCompressionMode;

			ChannelParameters[LineIndex][VoiceChannelIndex].RTPStreamParameters.TxDTMFRelayPayloadType = p_voip_dsp_advanced_channel_configuration->dtmf_payload_number;
			ChannelParameters[LineIndex][VoiceChannelIndex].RTPStreamParameters.RxDTMFRelayPayloadType = p_voip_dsp_advanced_channel_configuration->dtmf_payload_number;

			/**** PcmAdPcmComfortNoiseRtpPayloadTypeEnable is not supported in acGatewayAPI layer****/
			/**** g726_payload_number is not supported in acGatewayAPI layer****/
		}
	}

	DBG_PRINT("<<\r\n");

	return 0;
}


#ifdef CONFIG_RG_ATA
static int IsCallerIDType2Supported(acgECallerIdStandard CallerIdStandard)
{
	if ( (ACG_CALLER_ID_STANDARD__ETSI == CallerIdStandard)  ||
	    (ACG_CALLER_ID_STANDARD__DTMF_CLIP_DANISH == CallerIdStandard) ||
	    (ACG_CALLER_ID_STANDARD__DTMF_CLIP_INDIAN == CallerIdStandard) ||
	    (ACG_CALLER_ID_STANDARD__DTMF_CLIP_BRAZILIAN == CallerIdStandard) )
	   	return 0;
	else
	    	return 1;
}

static int IsDTMF_CallerID(acgECallerIdStandard CallerIdStandard)
{
	//if ( (ACG_CALLER_ID_STANDARD__ETSI == CallerIdStandard)   || AlexChen 20090226
	if ( (ACG_CALLER_ID_STANDARD__DTMF_CLIP_ETSI == CallerIdStandard)   ||
	     (ACG_CALLER_ID_STANDARD__DTMF_CLIP_DANISH == CallerIdStandard) ||
	     (ACG_CALLER_ID_STANDARD__DTMF_CLIP_INDIAN == CallerIdStandard) ||
	     (ACG_CALLER_ID_STANDARD__DTMF_CLIP_BRAZILIAN == CallerIdStandard) )
	    	return 1;
	else
	    	return 0;
}

static int Is_MWI_Supported(acgECallerIdStandard CallerIdStandard)
{
	if (!IsDTMF_CallerID(CallerIdStandard))
		return 1;

	if (ACG_CALLER_ID_STANDARD__DTMF_CLIP_DANISH == CallerIdStandard)
		return 1;
	else
		return 0;
}
#endif

static void FillRegionalSettingsParams(acgTInitializationParameters 	*pInitializationParams, regional_settings_params_t* p_regional_settings_params)
{
	int 				CallProgressToneIndex;

	dsp_regional_settings_params_call_progress_t	*pDsp_regional_settings_params_call_progress;

	DBG_PRINT(">>\r\n");

	pDsp_regional_settings_params_call_progress = &(p_regional_settings_params->dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[0]);

	pInitializationParams->NumberOfCallProgressTones = DSP_REGIONAL_SETTINGS_CALL_PROGRESS_TONE_LAST;

	for(CallProgressToneIndex=0; CallProgressToneIndex < DSP_REGIONAL_SETTINGS_CALL_PROGRESS_TONE_LAST; CallProgressToneIndex++)
	{
		if (!pDsp_regional_settings_params_call_progress[CallProgressToneIndex].EnableField)
			continue;

		pInitializationParams->CallProgressTones[CallProgressToneIndex].Type = pDsp_regional_settings_params_call_progress[CallProgressToneIndex].ToneCadenceType;
		pInitializationParams->CallProgressTones[CallProgressToneIndex].ToneAFrequency = pDsp_regional_settings_params_call_progress[CallProgressToneIndex].FreqA;
		pInitializationParams->CallProgressTones[CallProgressToneIndex].ToneB_OrAmpModulationFrequency = pDsp_regional_settings_params_call_progress[CallProgressToneIndex].FreqB;
		pInitializationParams->CallProgressTones[CallProgressToneIndex].ToneALevel = pDsp_regional_settings_params_call_progress[CallProgressToneIndex].FreqLevelA;
		pInitializationParams->CallProgressTones[CallProgressToneIndex].ToneBLevel = pDsp_regional_settings_params_call_progress[CallProgressToneIndex].FreqLevelB;
		pInitializationParams->CallProgressTones[CallProgressToneIndex].DetectionTimeOrCadenceFirstOnOrBurstDuration = pDsp_regional_settings_params_call_progress[CallProgressToneIndex].ToneOn0;
		pInitializationParams->CallProgressTones[CallProgressToneIndex].CadenceFirstOffDuration = pDsp_regional_settings_params_call_progress[CallProgressToneIndex].ToneOff0;
		pInitializationParams->CallProgressTones[CallProgressToneIndex].CadenceSecondOnDuration = pDsp_regional_settings_params_call_progress[CallProgressToneIndex].ToneOn1;
		pInitializationParams->CallProgressTones[CallProgressToneIndex].CadenceSecondOffDuration = pDsp_regional_settings_params_call_progress[CallProgressToneIndex].ToneOff1;
		pInitializationParams->CallProgressTones[CallProgressToneIndex].CadenceThirdOnDuration = pDsp_regional_settings_params_call_progress[CallProgressToneIndex].ToneOn2;
		pInitializationParams->CallProgressTones[CallProgressToneIndex].CadenceThirdOffDuration = pDsp_regional_settings_params_call_progress[CallProgressToneIndex].ToneOff2;
		pInitializationParams->CallProgressTones[CallProgressToneIndex].CadenceFourthOnDuration = pDsp_regional_settings_params_call_progress[CallProgressToneIndex].ToneOn3;
		pInitializationParams->CallProgressTones[CallProgressToneIndex].CadenceFourthOffDuration = pDsp_regional_settings_params_call_progress[CallProgressToneIndex].ToneOff3;

		pInitializationParams->CallProgressTones[CallProgressToneIndex].CadenceVoiceAddedWhileFirstOff = ACG_CONTROL__ENABLE;
		pInitializationParams->CallProgressTones[CallProgressToneIndex].CadenceVoiceAddedWhileSecondOff = ACG_CONTROL__ENABLE;
		pInitializationParams->CallProgressTones[CallProgressToneIndex].CadenceVoiceAddedWhileThirdOff = ACG_CONTROL__ENABLE;
		pInitializationParams->CallProgressTones[CallProgressToneIndex].CadenceVoiceAddedWhileFourthOff = ACG_CONTROL__ENABLE;

	}

	DBG_PRINT("<<\r\n");

}

void acl_rtp_unbind(int dspChannel)
{

#ifdef AC48_CPE_VOIP_TOOLKIT

	acgTCommand		rtpUnbindCmd;
	int 				retValue = ACG_FAILURE;

	if(dspFd == -1)
	{
		printf("DSP file descriptor was not initialized\r\n");
		return ;
	}

	memset(&rtpUnbindCmd, 0, sizeof(acgTCommand));

	rtpUnbindCmd.Command = ACG_RTP_UNBIND_CMD;
	rtpUnbindCmd.Channel = dspChannel;

	/* For AC48x Drivers - Don't unbind RTP if a DTMF Activity or IBS Event are in progress */
	while(retValue == ACG_FAILURE)
	{
		retValue = write(dspFd,(char *)&rtpUnbindCmd,sizeof(acgTCommand));
	}

#endif /*AC48_CPE_VOIP_TOOLKIT*/

	rtp_unbind(dspChannel);

}



void callerIdCB(void *data)
{
#ifdef CONFIG_RG_ATA

	acgTCallerId 		CallerId;
	int 				dspChannel;
	int 				line;

	DBG_PRINT(">>\r\n");

	dspChannel = *((int *)data);

	line = getLineByDspChannel(dspChannel);


	if(dspChannel == -1)
	{
		printf("dspChannel %d is invalid\r\n", dspChannel);
		return;
	}


	if(line < 0 || line > (CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES -1))
	{
		printf("line %d is not defined\r\n", line);
		return;
	}

	CallerId.CallerIdServiceType = AGC_CALLER_ID_SERVICE_TYPE__1_ON_HOOK;

//   	snprintf(CallerId.time, TIME_STRING_LEN, "%.2d%.2d%.2d%.2d", localtime_validate.tm_mon + 1,
//	         localtime_validate.tm_mday, localtime_validate.tm_hour, localtime_validate.tm_min);

	AC_STRNCPY(CallerId.Number, caller_id[line].number, MAX_DIAL_STRING_LEN - 1);
	AC_STRNCPY(CallerId.Name, caller_id[line].name, MAX_CALLER_ID_LEN - 1);
	AC_STRNCPY(CallerId.Addr, caller_id[line].addr, AC_DOTTED_IP_LEN - 1);

	/* TODO: When caller is anonymous, indicate it in number_status and
	 * name_status */
	CallerId.NumberStatus = ACG_PHONE_CALLER_NAME_PRESENT;
	CallerId.NameStatus = ACG_PHONE_CALLER_NAME_PRESENT;

	if (!IsDTMF_CallerID(ChannelParameters[line][0].CallerIDParameters.CallerIdStandard))
	{
		if(acgPlayCallerID(dspChannel, &CallerId) != ACG_SUCCESS )
		{
			printf("error - acgPlayCallerID()\r\n");
			return;
		}
	}
	else
	{
		printf("CallerIdStandard %d is not supporting DTMF caller Id\r\n", ChannelParameters[line][0].CallerIDParameters.CallerIdStandard);
	}

	lines[line].is_caller_id_timer_active = 0;

	DBG_PRINT("<<\r\n");
#endif
}

