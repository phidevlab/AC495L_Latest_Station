
#include "iphone.h"
#include "acl_log.h"

/* dummy definition */
int	waitUntilVoipTaskReadyForReconfig;

static IPHONE_PARAMETERS 	iphoneParameters;

CODEC_INFORMATION defaultInfoPerCodec[NUMBER_OF_CONFIGURABLE_CODECS] = {
	{ACL__G711ALAW, 	G711_BASIC_FRAME_DURATION*G711_BASIC_MULTIPLIER, G711_BASIC_FRAME_DURATION, 	G711_BASIC_MULTIPLIER},
	{ACL__G711MULAW, 	G711_BASIC_FRAME_DURATION*G711_BASIC_MULTIPLIER, G711_BASIC_FRAME_DURATION, 	G711_BASIC_MULTIPLIER},
	{ACL__G726_16, 		G726_BASIC_FRAME_DURATION*G726_BASIC_MULTIPLIER, G726_BASIC_FRAME_DURATION, 	G726_BASIC_MULTIPLIER},
	{ACL__G726_32,	 	G726_BASIC_FRAME_DURATION*G726_BASIC_MULTIPLIER, G726_BASIC_FRAME_DURATION, 	G726_BASIC_MULTIPLIER},
	{ACL__G723LOW, 		G723_BASIC_FRAME_DURATION*G723_BASIC_MULTIPLIER, G723_BASIC_FRAME_DURATION, 	G723_BASIC_MULTIPLIER},
	{ACL__G723HIGH, 	G723_BASIC_FRAME_DURATION*G723_BASIC_MULTIPLIER, G723_BASIC_FRAME_DURATION, 	G723_BASIC_MULTIPLIER},
	{ACL__G729, 		G729_BASIC_FRAME_DURATION*G729_BASIC_MULTIPLIER, G729_BASIC_FRAME_DURATION, 	G729_BASIC_MULTIPLIER},
#if !defined(AC488_CPE_VOIP_TOOLKIT) &&!defined(AC483_CPE_VOIP_TOOLKIT)
	{ACL__G722_48K,		G722_BASIC_FRAME_DURATION*G722_BASIC_MULTIPLIER, G722_BASIC_FRAME_DURATION,     G722_BASIC_MULTIPLIER},
#endif
};




int init_acMsgQDB(void);
void init_iphone_structure(IPHONE_PARAMETERS *);

int main(int argc, char *argv[])
{
	init_acMsgQDB();

	init_iphone_structure(&iphoneParameters);
	
	if(setIphoneParameters_api(&iphoneParameters) == -1)
	{
		acl_log(ACL_LOG_ERROR,"error setting iphone parameters \r\n");
		exit(1);
	}
	
	return startTerminalCommands(argc, argv);
    
} /* end of main() */


void init_iphone_structure(IPHONE_PARAMETERS *iphoneParams)
{
	int 	i;

	memset(iphoneParams, 0, sizeof(IPHONE_PARAMETERS));

	iphoneParams->DebugLevel = ENABLE_PRINTS;
	iphoneParams->EnableCallerIdType2 = ENABLE_CALLER_ID_TYPE_2; 
	iphoneParams->FaxByPassPayloadType = FAX_BYPASS_PAYLOAD_TYPE;
	iphoneParams->ModemByPassPayloadType = MODEM_BYPASS_PAYLOAD_TYPE;

	memcpy(iphoneParams->codecInfo, defaultInfoPerCodec, sizeof(defaultInfoPerCodec));

	for(i = 0; i < NUMBER_OF_ABS_CHANNLES; i++)
	{
		iphoneParams->DtmfTransportType[i] 				= IPHONE_DTMF_TRANSPORT_TYPE;
		iphoneParams->dtmfRelayRFC2833PayloadType[i] 	= IPHONE_DTMF_RELAY_RFC2833_PAYLOAD_TYPE;
		iphoneParams->VoiceVolume[i] 					= IPHONE_VOICE_VOLUME;
		iphoneParams->AutomaticGainControl[i] 			= IPHONE_AUTOMATIC_GAIN_CONTROL;
		iphoneParams->JitterBufferMinDelay[i] 			= IPHONE_JITTER_BUFFER_MIN_DELAY;
		iphoneParams->OptimizationFactor[i] 			= IPHONE_OPTIMIZATION_FACTOR;
		iphoneParams->SilenceCompression[i] 			= IPHONE_SILENCE_COMPRESSION;
		iphoneParams->EchoCancellation[i] 				= IPHONE_ECHO_CANCELLATION;
		iphoneParams->FaxTransportType[i] 				= IPHONE_FAX_TRANSPORT_TYPE;
		iphoneParams->FaxBypassCodecType[i] 			= FAX_BYPASS_CODEC_TYPE;
		iphoneParams->ErrorCorrectionMode[i] 			= IPHONE_ERROR_CORRECTION_MODE;
		iphoneParams->CallerIdStandard[i] 				= IPHONE_CALLER_ID_STANDARD;
	}

#ifdef AC488_CPE_VOIP_TOOLKIT
	iphoneParams->AC488ConferenceSupportEnable = AC488_CONFERENCE_DISABLE;
#endif

#if defined(AC488_CPE_VOIP_TOOLKIT)||defined(AC483_CPE_VOIP_TOOLKIT)
	/*Dial tone
	[CALL PROGRESS TONE #0]*/
	iphoneParams->CPTValues[0].ToneType 	 					= 1;
#ifdef EXTENDED_IBS_SUPPORTED
	iphoneParams->CPTValues[0].u.Components[0].Frequency 			= 350;
	iphoneParams->CPTValues[0].u.Components[1].Frequency 	 		= 440;
	iphoneParams->CPTValues[0].u.Components[0].ComponentLevel	 	= 19;
	iphoneParams->CPTValues[0].u.Components[1].ComponentLevel 	= 19;
	iphoneParams->CPTValues[0].ToneCadences[0].TOnDuration		= 500;
	iphoneParams->CPTValues[0].ToneCadences[0].TOffDuration		= 0;
	iphoneParams->CPTValues[0].ToneCadences[1].TOnDuration 		= 0;
	iphoneParams->CPTValues[0].ToneCadences[1].TOffDuration		= 0;
#else
	iphoneParams->CPTValues[0].LowFreq 		 = 350;
	iphoneParams->CPTValues[0].HighFreq 	 = 440;
	iphoneParams->CPTValues[0].LowFreqLevel	 = 19;
	iphoneParams->CPTValues[0].HighFreqLevel = 19;
	iphoneParams->CPTValues[0].TOn1		     = 500;
	iphoneParams->CPTValues[0].TOff1		 = 0;
	iphoneParams->CPTValues[0].TOn2 		 = 0;
	iphoneParams->CPTValues[0].TOff2		 = 0;
#endif
	/*Dial tone
	[CALL PROGRESS TONE #1]*/
	iphoneParams->CPTValues[1].ToneType 	 					= 1;
#ifdef EXTENDED_IBS_SUPPORTED
	iphoneParams->CPTValues[1].u.Components[0].Frequency 			= 425;
	iphoneParams->CPTValues[1].u.Components[1].Frequency 	 		= 0;
	iphoneParams->CPTValues[1].u.Components[0].ComponentLevel	 	= 22;
	iphoneParams->CPTValues[1].u.Components[1].ComponentLevel 	= 0;
	iphoneParams->CPTValues[1].ToneCadences[0].TOnDuration		= 500;
	iphoneParams->CPTValues[1].ToneCadences[0].TOffDuration		= 0;
	iphoneParams->CPTValues[1].ToneCadences[1].TOnDuration 		= 0;
	iphoneParams->CPTValues[1].ToneCadences[1].TOffDuration		= 0;
#else
	iphoneParams->CPTValues[1].LowFreq 		 = 425;
	iphoneParams->CPTValues[1].HighFreq 	 = 0;
	iphoneParams->CPTValues[1].LowFreqLevel  = 22;
	iphoneParams->CPTValues[1].HighFreqLevel = 0;
	iphoneParams->CPTValues[1].TOn1		     = 500;
	iphoneParams->CPTValues[1].TOff1 		 = 0;
	iphoneParams->CPTValues[1].TOn2 		 = 0;
	iphoneParams->CPTValues[1].TOff2 		 = 0;
#endif

	/*Ringback
	[CALL PROGRESS TONE #2]*/
	iphoneParams->CPTValues[2].ToneType 	 					= 2;
#ifdef EXTENDED_IBS_SUPPORTED
	iphoneParams->CPTValues[2].u.Components[0].Frequency 			= 440;
	iphoneParams->CPTValues[2].u.Components[1].Frequency 	 		= 480;
	iphoneParams->CPTValues[2].u.Components[0].ComponentLevel	 	= 13;
	iphoneParams->CPTValues[2].u.Components[1].ComponentLevel 	= 13;
	iphoneParams->CPTValues[2].ToneCadences[0].TOnDuration		= 200;
	iphoneParams->CPTValues[2].ToneCadences[0].TOffDuration		= 400;
	iphoneParams->CPTValues[2].ToneCadences[1].TOnDuration 		= 0;
	iphoneParams->CPTValues[2].ToneCadences[1].TOffDuration		= 0;
#else
	iphoneParams->CPTValues[2].LowFreq 		 = 440;
	iphoneParams->CPTValues[2].HighFreq 	 = 480;
	iphoneParams->CPTValues[2].LowFreqLevel  = 13;
	iphoneParams->CPTValues[2].HighFreqLevel = 13;
	iphoneParams->CPTValues[2].TOn1		     = 200;
	iphoneParams->CPTValues[2].TOff1 		 = 400;
	iphoneParams->CPTValues[2].TOn2 		 = 0;
	iphoneParams->CPTValues[2].TOff2 		 = 0;
#endif
	
	/*Busy
	[CALL PROGRESS TONE #3]*/
	iphoneParams->CPTValues[3].ToneType 	 					= 3;
#ifdef EXTENDED_IBS_SUPPORTED
	iphoneParams->CPTValues[3].u.Components[0].Frequency 			= 480;
	iphoneParams->CPTValues[3].u.Components[1].Frequency 	 		= 620;
	iphoneParams->CPTValues[3].u.Components[0].ComponentLevel	 	= 8;
	iphoneParams->CPTValues[3].u.Components[1].ComponentLevel 	= 8;
	iphoneParams->CPTValues[3].ToneCadences[0].TOnDuration		= 50;
	iphoneParams->CPTValues[3].ToneCadences[0].TOffDuration		= 50;
	iphoneParams->CPTValues[3].ToneCadences[1].TOnDuration 		= 0;
	iphoneParams->CPTValues[3].ToneCadences[1].TOffDuration		= 0;
#else
	iphoneParams->CPTValues[3].LowFreq 		 = 480;
	iphoneParams->CPTValues[3].HighFreq 	 = 620;
	iphoneParams->CPTValues[3].LowFreqLevel  = 8;
	iphoneParams->CPTValues[3].HighFreqLevel = 8;
	iphoneParams->CPTValues[3].TOn1		     = 50;
	iphoneParams->CPTValues[3].TOff1 		 = 50;
	iphoneParams->CPTValues[3].TOn2 		 = 0;
	iphoneParams->CPTValues[3].TOff2 		 = 0;
#endif

	/*Congestion
	[CALL PROGRESS TONE #4]*/
	iphoneParams->CPTValues[4].ToneType 	 					= 4;
#ifdef EXTENDED_IBS_SUPPORTED
	iphoneParams->CPTValues[4].u.Components[0].Frequency 			= 480;
	iphoneParams->CPTValues[4].u.Components[1].Frequency 	 		= 620;
	iphoneParams->CPTValues[4].u.Components[0].ComponentLevel	 	= 8;
	iphoneParams->CPTValues[4].u.Components[1].ComponentLevel 	= 8;
	iphoneParams->CPTValues[4].ToneCadences[0].TOnDuration		= 25;
	iphoneParams->CPTValues[4].ToneCadences[0].TOffDuration		= 25;
	iphoneParams->CPTValues[4].ToneCadences[1].TOnDuration 		= 0;
	iphoneParams->CPTValues[4].ToneCadences[1].TOffDuration		= 0;
#else
	iphoneParams->CPTValues[4].LowFreq 		 = 480;
	iphoneParams->CPTValues[4].HighFreq 	 = 620;
	iphoneParams->CPTValues[4].LowFreqLevel  = 8;
	iphoneParams->CPTValues[4].HighFreqLevel = 8;
	iphoneParams->CPTValues[4].TOn1		     = 25;
	iphoneParams->CPTValues[4].TOff1 		 = 25;
	iphoneParams->CPTValues[4].TOn2 		 = 0;
	iphoneParams->CPTValues[4].TOff2 		 = 0;
#endif

	/*Ringback
	[CALL PROGRESS TONE #5]*/
	iphoneParams->CPTValues[5].ToneType 	 					= 2;
#ifdef EXTENDED_IBS_SUPPORTED
	iphoneParams->CPTValues[5].u.Components[0].Frequency 			= 400;
	iphoneParams->CPTValues[5].u.Components[1].Frequency 	 		= 0;
	iphoneParams->CPTValues[5].u.Components[0].ComponentLevel	 	= 16;
	iphoneParams->CPTValues[5].u.Components[1].ComponentLevel 	= 0;
	iphoneParams->CPTValues[5].ToneCadences[0].TOnDuration		= 100;
	iphoneParams->CPTValues[5].ToneCadences[0].TOffDuration		= 300;
	iphoneParams->CPTValues[5].ToneCadences[1].TOnDuration 		= 0;
	iphoneParams->CPTValues[5].ToneCadences[1].TOffDuration		= 0;
#else
	iphoneParams->CPTValues[5].LowFreq 		 = 400;
	iphoneParams->CPTValues[5].HighFreq 	 = 0;
	iphoneParams->CPTValues[5].LowFreqLevel  = 16;
	iphoneParams->CPTValues[5].HighFreqLevel = 0;
	iphoneParams->CPTValues[5].TOn1		     = 100;
	iphoneParams->CPTValues[5].TOff1 		 = 300;
	iphoneParams->CPTValues[5].TOn2 		 = 0;
	iphoneParams->CPTValues[5].TOff2 		 = 0;
#endif
	
	/*Dial tone
	[CALL PROGRESS TONE #6]*/
	iphoneParams->CPTValues[6].ToneType 	 					= 1;
#ifdef EXTENDED_IBS_SUPPORTED
	iphoneParams->CPTValues[6].u.Components[0].Frequency 			= 440;
	iphoneParams->CPTValues[6].u.Components[1].Frequency 	 		= 0;
	iphoneParams->CPTValues[6].u.Components[0].ComponentLevel	 	= 22;
	iphoneParams->CPTValues[6].u.Components[1].ComponentLevel 	= 0;
	iphoneParams->CPTValues[6].ToneCadences[0].TOnDuration		= 500;
	iphoneParams->CPTValues[6].ToneCadences[0].TOffDuration		= 0;
	iphoneParams->CPTValues[6].ToneCadences[1].TOnDuration 		= 0;
	iphoneParams->CPTValues[6].ToneCadences[1].TOffDuration		= 0;
#else
	iphoneParams->CPTValues[6].LowFreq 		 = 440;
	iphoneParams->CPTValues[6].HighFreq 	 = 0;
	iphoneParams->CPTValues[6].LowFreqLevel  = 22;
	iphoneParams->CPTValues[6].HighFreqLevel = 0;
	iphoneParams->CPTValues[6].TOn1		     = 500;
	iphoneParams->CPTValues[6].TOff1 		 = 0;
	iphoneParams->CPTValues[6].TOn2 		 = 0;
	iphoneParams->CPTValues[6].TOff2 		 = 0;
#endif
	
	/*Congestion
	[CALL PROGRESS TONE #7]*/
	iphoneParams->CPTValues[7].ToneType 	 					= 4;
#ifdef EXTENDED_IBS_SUPPORTED
	iphoneParams->CPTValues[7].u.Components[0].Frequency 			= 440;
	iphoneParams->CPTValues[7].u.Components[1].Frequency 	 		= 0;
	iphoneParams->CPTValues[7].u.Components[0].ComponentLevel	 	= 8;
	iphoneParams->CPTValues[7].u.Components[1].ComponentLevel 	= 0;
	iphoneParams->CPTValues[7].ToneCadences[0].TOnDuration		= 24;
	iphoneParams->CPTValues[7].ToneCadences[0].TOffDuration		= 24;
	iphoneParams->CPTValues[7].ToneCadences[1].TOnDuration 		= 0;
	iphoneParams->CPTValues[7].ToneCadences[1].TOffDuration		= 0;
#else
	iphoneParams->CPTValues[7].LowFreq 		 = 440;
	iphoneParams->CPTValues[7].HighFreq 	 = 0;
	iphoneParams->CPTValues[7].LowFreqLevel  = 8;
	iphoneParams->CPTValues[7].HighFreqLevel = 0;
	iphoneParams->CPTValues[7].TOn1		     = 24;
	iphoneParams->CPTValues[7].TOff1 		 = 24;
	iphoneParams->CPTValues[7].TOn2 		 = 0;
	iphoneParams->CPTValues[7].TOff2 		 = 0;
#endif
	
	/*Busy
	[CALL PROGRESS TONE #8]*/
	iphoneParams->CPTValues[8].ToneType 	 					= 3;
#ifdef EXTENDED_IBS_SUPPORTED
	iphoneParams->CPTValues[8].u.Components[0].Frequency 			= 440;
	iphoneParams->CPTValues[8].u.Components[1].Frequency 	 		= 0;
	iphoneParams->CPTValues[8].u.Components[0].ComponentLevel	 	= 8;
	iphoneParams->CPTValues[8].u.Components[1].ComponentLevel 	= 0;
	iphoneParams->CPTValues[8].ToneCadences[0].TOnDuration		= 50;
	iphoneParams->CPTValues[8].ToneCadences[0].TOffDuration		= 50;
	iphoneParams->CPTValues[8].ToneCadences[1].TOnDuration 		= 0;
	iphoneParams->CPTValues[8].ToneCadences[1].TOffDuration		= 0;
#else
	iphoneParams->CPTValues[8].LowFreq 		 = 440;
	iphoneParams->CPTValues[8].HighFreq 	 = 0;
	iphoneParams->CPTValues[8].LowFreqLevel  = 8;
	iphoneParams->CPTValues[8].HighFreqLevel = 0;
	iphoneParams->CPTValues[8].TOn1		     = 50;
	iphoneParams->CPTValues[8].TOff1 		 = 50;
	iphoneParams->CPTValues[8].TOn2 		 = 0;
	iphoneParams->CPTValues[8].TOff2 		 = 0;
#endif
	
	/*Call Waiting
	[CALL PROGRESS TONE #9]*/
	iphoneParams->CPTValues[9].ToneType 	 					= 9;
#ifdef EXTENDED_IBS_SUPPORTED
	iphoneParams->CPTValues[9].u.Components[0].Frequency 			= 690;
	iphoneParams->CPTValues[9].u.Components[1].Frequency 	 		= 2070;
	iphoneParams->CPTValues[9].u.Components[0].ComponentLevel	 	= 15;
	iphoneParams->CPTValues[9].u.Components[1].ComponentLevel 	= 15;
	iphoneParams->CPTValues[9].ToneCadences[0].TOnDuration		= 4;
	iphoneParams->CPTValues[9].ToneCadences[0].TOffDuration		= 7;
	iphoneParams->CPTValues[9].ToneCadences[1].TOnDuration 		= 0;
	iphoneParams->CPTValues[9].ToneCadences[1].TOffDuration		= 0;
#else
	iphoneParams->CPTValues[9].LowFreq 		 = 690;
	iphoneParams->CPTValues[9].HighFreq 	 = 2070;
	iphoneParams->CPTValues[9].LowFreqLevel  = 15;
	iphoneParams->CPTValues[9].HighFreqLevel = 15;
	iphoneParams->CPTValues[9].TOn1		     = 4;
	iphoneParams->CPTValues[9].TOff1 		 = 7;
	iphoneParams->CPTValues[9].TOn2 		 = 0;
	iphoneParams->CPTValues[9].TOff2 		 = 0;
#endif

	iphoneParams->NumberOfCallProgressTones  = 10;
#endif /*defined(AC488_CPE_VOIP_TOOLKIT)||defined(AC483_CPE_VOIP_TOOLKIT)*/
}
/******************************************************************************/
