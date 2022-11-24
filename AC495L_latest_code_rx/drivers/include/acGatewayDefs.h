/******************************************************************** 
 * acGatewayDefs - AudioCodes Analog VoIP Gateway DSP and SLIC API                  	* 
 *                                                                      *
 * Structure, Enum and Constant Type Definitions                        *
 *                                                                      *
 * File Name:	acGatewayDefs.h                                         *
 *                                                                      *
 * Copyright (C) 2007 AudioCodes Ltd.                                   *
 *                                                                      *
 * Modifications :                                                      *
 *                                                                      *
 * 30/12/07 - Coding started.											* 	
 *                                                                      *
 ********************************************************************/

#ifndef	_ACGATEWAYDEFS_H
#define	_ACGATEWAYDEFS_H


#include "acGatewayDriversDefs.h"   	/* AudioCodes Gateway Definitions */

/********************************************************************
 * Constant Values Definitions												*
 ********************************************************************/
#define ACG_VERSION 					"0.1.4"


/*****   acGateway General definitions  *****/

#define MAX_TONES_TO_PLAY			32
#define MAX_TONE_SEQUENCE_TO_PLAY 	6
#define MAX_TONE_EXTENDED_TO_PLAY	6

#define ACG_SUCCESS					0
#define ACG_FAILURE					-1
#define ACG_NO_PACKET				1

#define TIME_STRING_LEN 				9
#define MAX_CALLER_ID_LEN		 		16
#define MAX_DIAL_STRING_LEN 			16

#define ACG_STR_MAX_LEN				128

#define ACG_MAC_ADDR_MAX_ENTRIES 4
#define ACG_SUCCESS					0
#define ACG_FAILURE					-1

#define ACG_MAX_RTP_CANONICAL_NAME_STRING_LENGTH 64  /*255*/

/*****   acGateway Default parameters  *****/

#define DEFAULT_CODER								ACG_CODER__G711ALAW			
#define DEFAULT_PTIME								20
#define DEFAULT_TX_VOICE_PAYLOAD_TYPE 				8
#define DEFAULT_RX_VOICE_PAYLOAD_TYPE 				8
#define DEFAULT_TX_DTMF_RELAY_PAYLOAD_TYPE		96
#define DEFAULT_RX_DTMF_RELAY_PAYLOAD_TYPE		96
#define DEFAULT_FAX_BYPASS_PAYLOAD_TYPE			102
#define DEFAULT_MODEM_BYPASS_PAYLOAD_TYPE		103
#define DEFAULT_SILENCE_COMPRESSION_MODE			ACG_SILENCE_COMPRESSION_MODE__DISABLE
#define DEFAULT_IBS_REDUNDANCY_LEVEL			    ACG_IBS_REDUNDANCY_LEVEL__REPETITIONS_NONE
#define DEFAULT_RTP_REDUNDANCY_MODE                 ACG_CONTROL__DISABLE
#define DEFAULT_RTCP_MEAN_TX_INTERVAL           5000
#define DEFAULT_RFC2833TxPT                     96
#define DEFAULT_RFC2833RxPT                     96
#define DEFAULT_RFC2198PT                       104
#define DEFAULT_CANONICAL_NAME_LENGTH           0
#define DEFAULT_TX_FILTER_RTCP                  ACG_CONTROL__DISABLE
#define DEFAULT_RX_FILTER_RTCP                  ACG_CONTROL__DISABLE

#define DEFAULT_DTMF_TRANSPORT_METHOD			ACG_SIG_TRANSPORT__RELAY_ENABLE_VOICE_MUTE
#define DEFAULT_CPT_TRANSPORT_METHOD			ACG_SIG_TRANSPORT__TRANSPARENT
#define DEFAULT_DETECTION_REDIRECTION			ACG_IBS_DETECTION_DIRECTION____TO_LOCAL_TDM
#define DEFAULT_DETECT_CALLPROGRESS			    ACG_CONTROL__ENABLE
#define DEFAULT_DETECT_DTMF_REGISTER			ACG_CONTROL__ENABLE
#define DEFAULT_IBS_LEVEL           			9
#define DEFAULT_LONG_DTMF_DETECT			    ACG_CONTROL__ENABLE

#define DEFAULT_RTCP_XR							ACG_CONTROL__DISABLE
	
#define DEFAULT_FAX_TRANSFER_METHOD			ACG_FAX_MODEM_TRANSPORT__RELAY
#define DEFAULT_MODEM_TRANSFER_METHOD		ACG_SIG_TRANSPORT__RELAY_ENABLE_VOICE_MUTE
#define DEFAULT_FAX_MAX_RATE					ACG_FAX_MAX_RATE__14_4_KBPS
#define DEFAULT_FAX_ERROR_CORRECTION			ACG_CONTROL__ENABLE
#define DEFAULT_CNG_RELAY						ACG_CONTROL__DISABLE
#define DEFAULT_BYPASS_CODER					ACG_CODER__G711ALAW
#define DEFAULT_IMAGE_DATA_REDUNDANCY_LEVEL__NONE ACG_IMAGE_DATA_REDUNDANCY_LEVEL__NONE
#define DEFAULT_T30_CONTROL_DATA_REDUNDANCY_LEVEL__NONE ACG_T30_CONTROL_DATA_REDUNDANCY_LEVEL__NONE
#define DEFAULT_CED_TRANSFER_MODE ACG_CED_TRANSFER_MODE__BY_FAX_RELAY
#define DEFAULT_V34_TRANSFER_METHOD ACG_FAX_MODEM_TRANSPORT__BYPASS
#define DEFAULT_V32_TRANSFER_METHOD ACG_FAX_MODEM_TRANSPORT__BYPASS
#define DEFAULT_V23_TRANSFER_METHOD ACG_FAX_MODEM_TRANSPORT__BYPASS
#define DEFAULT_V22_TRANSFER_METHOD ACG_FAX_MODEM_TRANSPORT__BYPASS
#define DEFAULT_V21_TRANSFER_METHOD ACG_FAX_MODEM_TRANSPORT__BYPASS
#define DEFAULT_BELL103_TRANSFER_METHOD ACG_FAX_MODEM_TRANSPORT__DISABLE
#define DEFAULT_V34_FAX_TRANSFER_METHOD ACG_FAX_MODEM_TRANSPORT__DISABLE

#define DEFAULT_CALLER_ID_STANDARD				ACG_CALLER_ID_STANDARD__TELCORDIA_BELLCORE
#define DEFAULT_CALLER_ID_ON_HOOK_DETECTION	ACG_CONTROL__ENABLE
#define DEFAULT_CALLER_ID_OFF_HOOK_DETECTION	ACG_CONTROL__ENABLE

#define DEFAULT_VOICE_OUTPUT_GAIN				ACG_VOICE_GAIN__0_DB
#define DEFAULT_INPUT_GAIN						ACG_VOICE_GAIN__0_DB
#define DEFAULT_AGC_MODE						ACG_CONTROL__DISABLE
#define DEFAULT_AGC_LOCATION					ACC_AGC_LOCATION__AT_ENCODER_INPUT
#define DEFAULT_AGC_TARGET_ENERGY				-19


#define DEFAULT_DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR	 ACG_DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR__7
#define DEFAULT_VOICE_JITTER_BUFFER_MIN_DELAY				35
#define DEFAULT_VOICE_JITTER_BUFFER_MAX_DELAY				150

#define DEFAULT_ECHO_CANCELER								ACG_CONTROL__ENABLE
#define DEFAULT_ECHO_CANCELER_NON_LINEAR_PROCESSOR		ACG_ECHO_CANCELER_NON_LINEAR_PROCESSOR_MODE__ENABLE
#define DEFAULT_ECHO_CANCELER_LENGTH		ACG_ECHO_CANCELER_LENGTH__32_MSEC/*ACL BW 091222. item #4-Echo canceller tail length.*/


#define DEFAULT_TEST_MODE		ACG_TEST_MODE__NONE

#ifndef _BASE_TYPES_DEFS
#define _BASE_TYPES_DEFS
typedef unsigned char   U8;
typedef unsigned short  U16;
typedef unsigned long   U32;
#endif /* #ifndef _BASE_TYPES_DEFS */

/******************************************************************** 
 * Enumerated Type Definitions											*
 ********************************************************************/

//NIRP 070624 AC52917 Distinctive ringing - FXS level support
typedef enum {
	SLIC_RING_CADENCE_PROFILE_NA = -1, /* don't care, don't change */
	SLIC_RING_CADENCE_PROFILE_1,
	SLIC_RING_CADENCE_PROFILE_2,
	SLIC_RING_CADENCE_PROFILE_3,
	SLIC_RING_CADENCE_PROFILE_LAST,
} enum_ring_cadence_profile_t;


/* Generic enable/disable enum definition */
typedef enum 
{
	ACG_CONTROL__DISABLE,
	ACG_CONTROL__ENABLE
} acgEControl;


/* Channel coder type enum definition */
typedef enum 
{
	ACG_CODER__CHANNEL_DISABLED		= 0, 
	ACG_CODER__TRANSPARENT			= 1, 
	ACG_CODER__G711ALAW  				= 3, 
    	ACG_CODER__G711MULAW				= 4, 
    	ACG_CODER__G726_16				= 5, 
    	ACG_CODER__G726_24				= 6, 
    	ACG_CODER__G726_32				= 7, 
    	ACG_CODER__G726_40				= 8, 
    	ACG_CODER__G727_16				= 9, 
    	ACG_CODER__G727_24_16				= 10,
    	ACG_CODER__G727_24				= 11, 
    	ACG_CODER__G727_32_16				= 12, 
    	ACG_CODER__G727_32_24				= 13, 
    	ACG_CODER__G727_32				= 14, 
    	ACG_CODER__G727_40_16				= 15, 
    	ACG_CODER__G727_40_24				= 16, 
    	ACG_CODER__G727_40_32				= 17, 
	ACG_CODER__G728_16	        		= 18,
	ACG_CODER__LINEAR_16_BIT_PCM		= 19,
    	ACG_CODER__G723LOW				= 25, 
    	ACG_CODER__G723HIGH				= 26, 
    	ACG_CODER__G729			    		= 27, 
    	ACG_CODER__GSM_FULL_RATE_13_2	= 28, 
    	ACG_CODER__MICROSOFT_GSM_13_2	= 29,
    	ACG_CODER__EVRC_8_8K_0_8_K   		= 30, 
    	ACG_CODER__QCELP_8           			= 31,
    	ACG_CODER__QCELP_13          			= 32,
	ACG_CODER__LINEAR_8_BIT_PCM  		= 47,  
	ACG_CODER__G729E_11_8K      			= 48,
    	ACG_CODER__GSM_EFR          			= 49,
	ACG_CODER__ILBC_15_2K				= 51,
	ACG_CODER__ILBC_13_33K			= 52,
	ACG_CODER__LINEAR_16_BIT_PCM_SAMPLE_RATE_16KHZ = 53,
	ACG_CODER__G722_48K				= 54,
	ACG_CODER__G722_56K				= 55,
	ACG_CODER__G722_64K				= 56,
	ACG_CODER__EVRC_B					= 66,
    	ACG_CODER__G729_EV				=67
} acgECoder; /* Compatible with Tac49xCoder */

/* In-band signaling transport method enum definition */
typedef enum
{
	ACG_SIG_TRANSPORT__VOICE_MUTE,
	ACG_SIG_TRANSPORT__RELAY,			/* RFC 2833 */
	ACG_SIG_TRANSPORT__TRANSPARENT,
	ACG_SIG_TRANSPORT__RELAY_ENABLE_VOICE_MUTE,
	ACG_SIG_TRANSPORT__MIXED
} acgESignalingTransportMethod; /* Compatible with Tac49xIbsTransferMode */

typedef enum
{
    ACG_IBS_DETECTION_DIRECTION____TO_LOCAL_TDM,
    ACG_IBS_DETECTION_DIRECTION___TO_NETWORK
}acgEIbsDetectionDirection;

typedef enum 
{
    ACG_TEST_MODE__NONE,
    ACG_TEST_MODE__1KHZ_TO_ENCODER,
    ACG_TEST_MODE__ALL_CHANNELS_LOOP_BACK  
} acgETestMode;

/* Fax and Modem transport enum definition */
typedef enum
{
	ACG_FAX_MODEM_TRANSPORT__DISABLE,
	ACG_FAX_MODEM_TRANSPORT__RELAY,		/* T.38 */
	ACG_FAX_MODEM_TRANSPORT__BYPASS,
	ACG_FAX_MODEM_TRANSPORT__VBD,	/* Not compatible with DSP drivers */
	
} acgEFaxModemTransportMethod; /* Compatible with Tac49xFaxModemTransferMethod */

/* Fax relay max rate enum definition */
typedef enum 
{
	ACG_FAX_MAX_RATE__2_4_KBPS,
	ACG_FAX_MAX_RATE__4_8_KBPS,
	ACG_FAX_MAX_RATE__7_2_KBPS,
	ACG_FAX_MAX_RATE__9_6_KBPS,
	ACG_FAX_MAX_RATE__12_KBPS,
	ACG_FAX_MAX_RATE__14_4_KBPS
} acgEFaxMaxRate; /* Compatible with Tac49xMaxRate */

/*Image data redundancy enum definition*/
typedef enum 
{
    ACG_IMAGE_DATA_REDUNDANCY_LEVEL__NONE,
    ACG_IMAGE_DATA_REDUNDANCY_LEVEL__1,
    ACG_IMAGE_DATA_REDUNDANCY_LEVEL__2,                         
    ACG_IMAGE_DATA_REDUNDANCY_LEVEL__3     
} acgEImageDataRedundancyLevel; /* Compatible with Tac49xImageDataRedundancyLevel */

/*T30 control data redundancy level enum definition*/
typedef enum 
{
    ACG_T30_CONTROL_DATA_REDUNDANCY_LEVEL__NONE,
    ACG_T30_CONTROL_DATA_REDUNDANCY_LEVEL__1,
    ACG_T30_CONTROL_DATA_REDUNDANCY_LEVEL__2,                         
    ACG_T30_CONTROL_DATA_REDUNDANCY_LEVEL__3,                         
    ACG_T30_CONTROL_DATA_REDUNDANCY_LEVEL__4,                         
    ACG_T30_CONTROL_DATA_REDUNDANCY_LEVEL__5,                         
    ACG_T30_CONTROL_DATA_REDUNDANCY_LEVEL__6,                         
    ACG_T30_CONTROL_DATA_REDUNDANCY_LEVEL__7
} acgET30ControlDataRedundancyLevel; /* Compatible with Tac49xT30ControlDataRedundancyLevel */

/*CED transfer mode enum definition*/
typedef enum
{
    ACG_CED_TRANSFER_MODE__BY_FAX_RELAY,
    ACG_CED_TRANSFER_MODE__IN_VOICE_OR_PCM_BYPASS_MODE_TRIGGER_FAX_BY_V21_PREAMBLE
} acgECedTransferMode;

/* Caller ID standard enum definition */
typedef enum 
{
    	ACG_CALLER_ID_STANDARD__TELCORDIA_BELLCORE,
	ACG_CALLER_ID_STANDARD__ETSI,
	ACG_CALLER_ID_STANDARD__NTT,
	ACG_CALLER_ID_STANDARD__DTMF_CLIP_ETSI=8,
	ACG_CALLER_ID_STANDARD__DTMF_CLIP_DANISH,
	ACG_CALLER_ID_STANDARD__DTMF_CLIP_INDIAN,
	ACG_CALLER_ID_STANDARD__DTMF_CLIP_BRAZILIAN,
	ACG_CALLER_ID_STANDARD__DTMF_CLIP_NTT_DID 
} acgECallerIdStandard; /* Compatible with Tac49xCallerIdStandard */

/* Voice input and output gain value enum definition */
typedef enum 
{
    	ACG_VOICE_GAIN__MUTE,
	ACG_VOICE_GAIN__minus31_DB,
	ACG_VOICE_GAIN__minus30_DB,
	ACG_VOICE_GAIN__minus29_DB,
	ACG_VOICE_GAIN__minus28_DB,
	ACG_VOICE_GAIN__minus27_DB,
	ACG_VOICE_GAIN__minus26_DB,
	ACG_VOICE_GAIN__minus25_DB,
	ACG_VOICE_GAIN__minus24_DB,
	ACG_VOICE_GAIN__minus23_DB,
	ACG_VOICE_GAIN__minus22_DB,
	ACG_VOICE_GAIN__minus21_DB,
	ACG_VOICE_GAIN__minus20_DB,
	ACG_VOICE_GAIN__minus19_DB,
	ACG_VOICE_GAIN__minus18_DB,
	ACG_VOICE_GAIN__minus17_DB,
	ACG_VOICE_GAIN__minus16_DB,
	ACG_VOICE_GAIN__minus15_DB,
	ACG_VOICE_GAIN__minus14_DB,
	ACG_VOICE_GAIN__minus13_DB,
	ACG_VOICE_GAIN__minus12_DB,
	ACG_VOICE_GAIN__minus11_DB,
	ACG_VOICE_GAIN__minus10_DB,
	ACG_VOICE_GAIN__minus9_DB,
	ACG_VOICE_GAIN__minus8_DB,
	ACG_VOICE_GAIN__minus7_DB,
	ACG_VOICE_GAIN__minus6_DB,
	ACG_VOICE_GAIN__minus5_DB,
	ACG_VOICE_GAIN__minus4_DB,
	ACG_VOICE_GAIN__minus3_DB,
	ACG_VOICE_GAIN__minus2_DB,
	ACG_VOICE_GAIN__minus1_DB,
	ACG_VOICE_GAIN__0_DB,
	ACG_VOICE_GAIN__plus1_DB,
	ACG_VOICE_GAIN__plus2_DB,
	ACG_VOICE_GAIN__plus3_DB,
	ACG_VOICE_GAIN__plus4_DB,
	ACG_VOICE_GAIN__plus5_DB,
	ACG_VOICE_GAIN__plus6_DB,
	ACG_VOICE_GAIN__plus7_DB,
	ACG_VOICE_GAIN__plus8_DB,
	ACG_VOICE_GAIN__plus9_DB,
	ACG_VOICE_GAIN__plus10_DB,
	ACG_VOICE_GAIN__plus11_DB,
	ACG_VOICE_GAIN__plus12_DB,
	ACG_VOICE_GAIN__plus13_DB,
	ACG_VOICE_GAIN__plus14_DB,
	ACG_VOICE_GAIN__plus15_DB,
	ACG_VOICE_GAIN__plus16_DB,
	ACG_VOICE_GAIN__plus17_DB,
	ACG_VOICE_GAIN__plus18_DB,
	ACG_VOICE_GAIN__plus19_DB,
	ACG_VOICE_GAIN__plus20_DB,
	ACG_VOICE_GAIN__plus21_DB,
	ACG_VOICE_GAIN__plus22_DB,
	ACG_VOICE_GAIN__plus23_DB,
	ACG_VOICE_GAIN__plus24_DB,
	ACG_VOICE_GAIN__plus25_DB,
	ACG_VOICE_GAIN__plus26_DB,
	ACG_VOICE_GAIN__plus27_DB,
	ACG_VOICE_GAIN__plus28_DB,
	ACG_VOICE_GAIN__plus29_DB,
	ACG_VOICE_GAIN__plus30_DB,
	ACG_VOICE_GAIN__plus31_DB
} acgEVoiceGain; /* Compatible with Tac49xVoiceGain */

typedef enum
{
	ACC_AGC_LOCATION__AT_ENCODER_INPUT,
	ACC_AGC_LOCATION__AT_DECODER_OUTPUT
} acgEAGCLocation;


/* Echo canceler non-linear processor (NLP) mode enum definition */
typedef enum 
{
	ACG_ECHO_CANCELER_NON_LINEAR_PROCESSOR_MODE__DISABLE,
	ACG_ECHO_CANCELER_NON_LINEAR_PROCESSOR_MODE__ENABLE,
} acgEEchoCancelerNonLinearProcessor; 

/* Echo canceller length */
typedef enum 
{
    ACG_ECHO_CANCELER_LENGTH__8_MSEC,
    ACG_ECHO_CANCELER_LENGTH__16_MSEC,
    ACG_ECHO_CANCELER_LENGTH__24_MSEC,
    ACG_ECHO_CANCELER_LENGTH__32_MSEC,
} acgEEchoCancelerLength;


typedef enum
{
	ACG_IBS_SYSTEM_CODE__NONE,
	ACG_IBS_SYSTEM_CODE__DTMF,
	ACG_IBS_SYSTEM_CODE__SILENCE,
	ACG_IBS_SYSTEM_CODE__MF_R1_OR_SS5,
	ACG_IBS_SYSTEM_CODE__MF_R2_FORWARD,
	ACG_IBS_SYSTEM_CODE__MF_R2_BACKWARD,
	ACG_IBS_SYSTEM_CODE__CALL_PROGRESS_0_TO_15,
	ACG_IBS_SYSTEM_CODE__CALL_PROGRESS_16_TO_31,
	ACG_IBS_SYSTEM_CODE__USER_DEFINED_TONES_0_TO_15,
	ACG_IBS_SYSTEM_CODE__USER_DEFINED_TONES_16_TO_31,
	ACG_IBS_SYSTEM_CODE__ON_THE_FLY_TONES,  /*!< Generates a signal combined of User-Defined Frequency 1, 2 & 3. @n Refer to <A HREF="IBS_SYSTEM_CODE__ON_THE_FLY_TONES - On-the-Fly Digits.html">on-the-fly digits</A>.*/
	ACG_IBS_SYSTEM_CODE__FD_TEST_MODE_A,
	ACG_IBS_SYSTEM_CODE__FD_TEST_MODE_B,
	ACG_IBS_SYSTEM_CODE__ADVANCED_SIGNALS
}acgEIbsSystemCode; /* Compatible with Tac49xIbsSystemCode */

typedef enum 
{
	ACG_DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR__0_NONE,
	ACG_DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR__1,
	ACG_DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR__2,
	ACG_DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR__3,
	ACG_DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR__4,
	ACG_DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR__5,
	ACG_DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR__6,
	ACG_DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR__7,
	ACG_DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR__8,
	ACG_DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR__9,
	ACG_DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR__10,
	ACG_DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR__11,
	ACG_DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR__12_MAX,
	ACG_DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR__STATIC_DELAY_FOR_BYPASS_MODE
} acgEDynamicJitterBufferOptimizationFactor; /* Compatible with Tac49xDynamicJitterBufferOptimizationFactor */

typedef enum 
{
	ACG_SILENCE_COMPRESSION_MODE__DISABLE,
	ACG_SILENCE_COMPRESSION_MODE__ENABLE_NOISE_ADAPTATION_ENABLE
} acgESilenceCompressionMode;  /* Compatible with Tac49xSilenceCompressionMode */

typedef enum
{
    ACG_IBS_REDUNDANCY_LEVEL__REPETITIONS_NONE,
    ACG_IBS_REDUNDANCY_LEVEL__REPETITIONS_1,
    ACG_IBS_REDUNDANCY_LEVEL__REPETITIONS_2,
    ACG_IBS_REDUNDANCY_LEVEL__REPETITIONS_3
} acgEIbsRedundancyLevel;


/* Signal direction enum definition */
typedef enum 
{
	ACG_DIRECTION__TO_LOCAL_TDM = 0,
	ACG_DIRECTION__TO_NETWORK = 1,                      /* Network means DTMF via RFC4733 */
	ACG_DIRECTION__TO_ENCODER_INPUT =2,                 /* EncoderInput means IBS */
	ACG_DIRECTION__TO_LOCAL_TDM_AND_NETWORK = 3,	    /* Network means DTMF via RFC4733 *//* Not support on AC48x Drivers */
	ACG_DIRECTION__TO_LOCAL_TDM_AND_ENCODER_INPUT = 4,  /* EncoderInput means IBS */
	ACG_DIRECTION__MAXIMUM
} acgEDirection; /* Compatible with Tac49xIbsStringGeneratorRedirection; */

/* Tone generation type enum definition */
typedef enum
{
	ACG_TONE_TYPE__DTMF,
	ACG_TONE_TYPE__CALL_PROGRESS_TONE
} acgEToneType;	

typedef enum
{
	AGC_CALLER_ID_SERVICE_TYPE__1_ON_HOOK,
	AGC_CALLER_ID_SERVICE_TYPE__2_OFF_HOOK
} acgECallerIdServiceType; /* Compatible with Tac49xCallerIdServiceType */

typedef enum 
{
	ACG_PHONE_CALLER_NUMBER_PRESENT,
	ACG_PHONE_CALLER_NUMBER_PRIVATE,
	ACG_PHONE_CALLER_NUMBER_UNAVAILABLE,
} acgEPhoneCallerNumberStatus;


typedef enum 
{
	ACG_PHONE_CALLER_NAME_PRESENT,
	ACG_PHONE_CALLER_NAME_PRIVATE,
	ACG_PHONE_CALLER_NAME_UNAVAILABLE,
} acgEPhoneCallerNameStatus;

typedef enum
{
	ACG__3_WAY_CONFERENCE_MODE__DISABLE,
	ACG__3_WAY_CONFERENCE_MODE__1,
} acgE3WayConferenceMode; /* Compatible with Tac49x3WayConferenceMode; */

typedef enum
{
	ACG_PROTOCOL__PROPRIETARY,
	ACG_PROTOCOL__RTP,
	ACG_PROTOCOL__RTCP,
	ACG_PROTOCOL__FAX,
	ACG_PROTOCOL__ATM_AAL2,
       ACG_PROTOCOL__ATM_AAL1,
	ACG_PROTOCOL__MEDIATED_CHANNEL_RTP,
	ACG_PROTOCOL__MEDIATED_CHANNEL_RTCP,
	ACG_PROTOCOL__CONFERENCE_1,
	ACG_PROTOCOL__CONFERENCE_2,
	ACG_PROTOCOL__DEBUG,
	ACG_PROTOCOL__PLAYBACK_COMMAND,
	ACG_PROTOCOL__PLAYBACK_VOICE,
	ACG_PROTOCOL__IP_SEC,
	ACG_PROTOCOL__CONFERENCE,
	ACG_PROTOCOL__MEDIATED_CHANNEL_ATM_AAL1,
	ACG_PROTOCOL__MEDIATED_CHANNEL_ATM_AAL2,
	ACG_PROTOCOL__RECORD_COMMAND,
	ACG_PROTOCOL__RECORD_TDM_PACKET,
	ACG_PROTOCOL__RECORD_NETWORK_PACKET,
       ACG_PROTOCOL__3GPP_IUB,
	ACG_PROTOCOL__ERROR
} acgEProtocol; /* Compatible with Tac49xProtocol */

typedef enum
{
    ACG_FAX_AND_DATA_GAIN__DEFAULT_0_DB,
    ACG_FAX_AND_DATA_GAIN__minus31_DB,
    ACG_FAX_AND_DATA_GAIN__minus30_DB,
    ACG_FAX_AND_DATA_GAIN__minus29_DB,
    ACG_FAX_AND_DATA_GAIN__minus28_DB,
    ACG_FAX_AND_DATA_GAIN__minus27_DB,
    ACG_FAX_AND_DATA_GAIN__minus26_DB,
    ACG_FAX_AND_DATA_GAIN__minus25_DB,
    ACG_FAX_AND_DATA_GAIN__minus24_DB,
    ACG_FAX_AND_DATA_GAIN__minus23_DB,
    ACG_FAX_AND_DATA_GAIN__minus22_DB,
    ACG_FAX_AND_DATA_GAIN__minus21_DB,
    ACG_FAX_AND_DATA_GAIN__minus20_DB,
    ACG_FAX_AND_DATA_GAIN__minus19_DB,
    ACG_FAX_AND_DATA_GAIN__minus18_DB,
    ACG_FAX_AND_DATA_GAIN__minus17_DB,
    ACG_FAX_AND_DATA_GAIN__minus16_DB,
    ACG_FAX_AND_DATA_GAIN__minus15_DB,
    ACG_FAX_AND_DATA_GAIN__minus14_DB,
    ACG_FAX_AND_DATA_GAIN__minus13_DB,
    ACG_FAX_AND_DATA_GAIN__minus12_DB,
    ACG_FAX_AND_DATA_GAIN__minus11_DB,
    ACG_FAX_AND_DATA_GAIN__minus10_DB,
    ACG_FAX_AND_DATA_GAIN__minus9_DB,
    ACG_FAX_AND_DATA_GAIN__minus8_DB,
    ACG_FAX_AND_DATA_GAIN__minus7_DB,
    ACG_FAX_AND_DATA_GAIN__minus6_DB,
    ACG_FAX_AND_DATA_GAIN__minus5_DB,
    ACG_FAX_AND_DATA_GAIN__minus4_DB,
    ACG_FAX_AND_DATA_GAIN__minus3_DB,
    ACG_FAX_AND_DATA_GAIN__minus2_DB,
    ACG_FAX_AND_DATA_GAIN__minus1_DB,
    ACG_FAX_AND_DATA_GAIN__0_DB,
    ACG_FAX_AND_DATA_GAIN__plus1_DB,
    ACG_FAX_AND_DATA_GAIN__plus2_DB,
    ACG_FAX_AND_DATA_GAIN__plus3_DB,
    ACG_FAX_AND_DATA_GAIN__plus4_DB,
    ACG_FAX_AND_DATA_GAIN__plus5_DB,
    ACG_FAX_AND_DATA_GAIN__plus6_DB,
    ACG_FAX_AND_DATA_GAIN__plus7_DB,
    ACG_FAX_AND_DATA_GAIN__plus8_DB,
    ACG_FAX_AND_DATA_GAIN__plus9_DB,
    ACG_FAX_AND_DATA_GAIN__plus10_DB,
    ACG_FAX_AND_DATA_GAIN__plus11_DB,
    ACG_FAX_AND_DATA_GAIN__plus12_DB,
    ACG_FAX_AND_DATA_GAIN__plus13_DB,
    ACG_FAX_AND_DATA_GAIN__plus14_DB,
    ACG_FAX_AND_DATA_GAIN__plus15_DB,
    ACG_FAX_AND_DATA_GAIN__plus16_DB,
    ACG_FAX_AND_DATA_GAIN__plus17_DB,
    ACG_FAX_AND_DATA_GAIN__plus18_DB,
    ACG_FAX_AND_DATA_GAIN__plus19_DB,
    ACG_FAX_AND_DATA_GAIN__plus20_DB,
    ACG_FAX_AND_DATA_GAIN__plus21_DB,
    ACG_FAX_AND_DATA_GAIN__plus22_DB,
    ACG_FAX_AND_DATA_GAIN__plus23_DB,
    ACG_FAX_AND_DATA_GAIN__plus24_DB,
    ACG_FAX_AND_DATA_GAIN__plus25_DB,
    ACG_FAX_AND_DATA_GAIN__plus26_DB,
    ACG_FAX_AND_DATA_GAIN__plus27_DB,
    ACG_FAX_AND_DATA_GAIN__plus28_DB,
    ACG_FAX_AND_DATA_GAIN__plus29_DB,
    ACG_FAX_AND_DATA_GAIN__plus30_DB,
    ACG_FAX_AND_DATA_GAIN__plus31_DB
} acgEFaxAndDataGain;

typedef enum
{
	ACG_CALL_PROGRESS_SIGNAL_TYPE__CONTINUOUS,
	ACG_CALL_PROGRESS_SIGNAL_TYPE__CADENCE,
	ACG_CALL_PROGRESS_SIGNAL_TYPE__BURST,
	ACG_CALL_PROGRESS_SIGNAL_TYPE__SPECIAL_INFORMATION_TONE
} acgECallProgressSignalType;  /* Compatible with Tac49xCallProgressSignalType */

typedef enum 
{
	ACG_RTCP_RR_Rx,	/* Rx Receiver Report */
	ACG_RTCP_SR_Rx,	/* Rx Sender Report */
	ACG_RTCP_RR_Tx,	/* Tx Receiver Report */
	ACG_RTCP_SR_Tx		/* Tx Sender Report */
}acgERTCPEventType;

typedef enum 
{
	ACG_RTCP_XR_INFO,	/* RTCP XR Local Information */
	ACG_RTCP_XR_REMOTE_INFO,	/* RTCP XR Remote Information */
}acgERTCPXREventType;

typedef enum 
{
    ACG_RTCP_PARAMETER_INDEX__TRANSMIT_PACKET_COUNT,
    ACG_RTCP_PARAMETER_INDEX__TRANSMIT_OCTETS_COUNT,
    ACG_RTCP_PARAMETER_INDEX__RECEIVE_PACKET_COUNT,
    ACG_RTCP_PARAMETER_INDEX__RECEIVE_OCTETS_COUNT,
    ACG_RTCP_PARAMETER_INDEX__JITTER,
    ACG_RTCP_PARAMETER_INDEX__PACKET_LOSS,
    ACG_RTCP_PARAMETER_INDEX__ROUND_TRIP_DELAY
}acgERTCPRegularParameter;  /* Compatible with Tac49xSetRtcpParametersAttr */

/* DSP and FXS/FXO event types enum definiton */
typedef enum
{
	/* DSP events : */
	ACG_EVENT_TYPE__NO_EVENT=0,				/* Indicates that no event is currently avilable */
	ACG_EVENT_TYPE__DTMF_START,				/* Beginning of DTMF digit (0-9,*,#,A-D). More info in Tone struct. */
	ACG_EVENT_TYPE__DTMF_END,					/* End of DTMF digit (0-9,*,#,A-D). More info in Tone struc.  */
	ACG_EVENT_TYPE__CALL_PROGRESS_TONE_START,/* Beginning of call progress tone (dial tone, ...). More info in Tone struct. */
	ACG_EVENT_TYPE__CALL_PROGRESS_TONE_END,	/* End of call progress tone (dial tone, ...). More info in Tone struct. */
	ACG_EVENT_TYPE__CALLER_ID_END,			/* Caller ID end message . More info in CallerID struct. */
	ACG_EVENT_TYPE__FAX_START,				/* Fax signal detected. More info in FaxModem struct. */
	ACG_EVENT_TYPE__FAX_SIGNAL,				/* Fax signal detected. More info in FaxModem struct. */
	ACG_EVENT_TYPE__MODEM_START,				/* Modem signal detected. More info in FaxModem struct. */
	ACG_EVENT_TYPE__FAX_OR_MODEM_START,		/* Ambigous fax or modem signal detected (or switch to bypass). More info in FaxModem struct. */
	ACG_EVENT_TYPE__FAX_OR_MODEM_END,		/* End of fax or modem - back to voice */
	ACG_EVENT_TYPE__RTCP_INFO,				/* RTCP information */
	ACG_EVENT_TYPE__RTCP_XR_INFO,				/* RTCP-XR information */
	/* FXS/FXO events : */
	ACG_EVENT_TYPE__ON_HOOK=100,			/* Receiver On-hook */
	ACG_EVENT_TYPE__OFF_HOOK,				/* Receiver Off-hook */
	ACG_EVENT_TYPE__FLASH_HOOK,			/* Flash hook signal */
	ACG_EVENT_TYPE__RING_START,			/* Start of ring signal */
	ACG_EVENT_TYPE__RING_END,				/* Start of ring signal */
	ACG_EVENT_TYPE__POLARITY_REVERSAL,	/* Tip/Ring polarity reversal */
} acgEEventType;

typedef enum             
{
	ACG_INITIATING_FLAG__MEDIA_GATEWAY_STATE_IS_INITIATED_BY_REMOTE_MEDIA_GATEWAY,
	ACG_INITIATING_FLAG__MEDIA_GATEWAY_STATE_IS_INITIATED_BY_DSP
} acgEInitiatingFlag;


typedef enum
{
	ACG_REASON_IDENTIFIER_CODE_Reason_NULL = 0,
	ACG_REASON_IDENTIFIER_CODE_V8CM,
	ACG_REASON_IDENTIFIER_CODE_V8JM,
	ACG_REASON_IDENTIFIER_CODE_V32AA_1800HZ,
	ACG_REASON_IDENTIFIER_CODE_V32AC,
	ACG_REASON_IDENTIFIER_CODE_V22USB1_2250HZ,
	ACG_REASON_IDENTIFIER_CODE_V22SB1,
	ACG_REASON_IDENTIFIER_CODE_V22bisS1,
	ACG_REASON_IDENTIFIER_CODE_V21Channel2,
	ACG_REASON_IDENTIFIER_CODE_V21Channel1,
       ACG_REASON_IDENTIFIER_CODE_V21Preamp,
	ACG_REASON_IDENTIFIER_CODE_V23ForwardChannel_1300Hz,
	ACG_REASON_IDENTIFIER_CODE_V23LowChannel,
	ACG_REASON_IDENTIFIER_CODE_BELL103ANS_2225Hz,
	ACG_REASON_IDENTIFIER_CODE_V21Channel2HDLCFlag,
	ACG_REASON_IDENTIFIER_CODE_Indeterminatesignal,
	ACG_REASON_IDENTIFIER_CODE_Silence,
	ACG_REASON_IDENTIFIER_CODE_CNG,
	ACG_REASON_IDENTIFIER_CODE_Voice,
	ACG_REASON_IDENTIFIER_CODE_TimeOut,
	ACG_REASON_IDENTIFIER_CODE_PStateTransition,
	ACG_REASON_IDENTIFIER_CODE_Cleardown,
	ACG_REASON_IDENTIFIER_CODE_CED_ANS,
	ACG_REASON_IDENTIFIER_CODE_ANSam,
	ACG_REASON_IDENTIFIER_CODE_ANS_Phasereversal,
	ACG_REASON_IDENTIFIER_CODE_ANSam_Phasereversal,
	ACG_REASON_IDENTIFIER_CODE_Reserve1,
	ACG_REASON_IDENTIFIER_CODE_Reserve2,
	ACG_REASON_IDENTIFIER_CODE_Reserve3,
	ACG_REASON_IDENTIFIER_CODE_Reserve4,
	ACG_REASON_IDENTIFIER_CODE_V8bisCRe,
	ACG_REASON_IDENTIFIER_CODE_V8bisCRd,
	ACG_REASON_IDENTIFIER_CODE_TIA825A45bits,
	ACG_REASON_IDENTIFIER_CODE_TIA825A50bits,
	ACG_REASON_IDENTIFIER_CODE_EDT,
	ACG_REASON_IDENTIFIER_CODE_Bell103modem,
	ACG_REASON_IDENTIFIER_CODE_V21TextTelephone,
	ACG_REASON_IDENTIFIER_CODE_V23Text,
	ACG_REASON_IDENTIFIER_CODE_V18TextTelephone,
}acgEReasonIdentifierCode;

/* Event polling method enum definition */
typedef enum
{
	ACG_POLLING_METHOD__BLOCKING,		/* Blocking mode - function will not return until an event is received */
	ACG_POLLING_METHOD__NON_BLOCKING	/* Non-blocking mode - funciton will return immediately with event type or
									   EVENT_TYPE__NO_EVENT */
} acgEPollingMethod;
/* Compatible with Tac49xTimeSlotManipulationCommandType */
typedef enum
{
    ACG_TIME_SLOT_MANIPULATION_COMMAND_TYPE__TDM_SIDE_LOOP,
    ACG_TIME_SLOT_MANIPULATION_COMMAND_TYPE__NETWORK_SIDE_LOOP,
    ACG_TIME_SLOT_MANIPULATION_COMMAND_TYPE__SWAP_CHANNELS,
    ACG_TIME_SLOT_MANIPULATION_COMMAND_TYPE__RESTORE_CONNECTION,
    ACG_TIME_SLOT_MANIPULATION_COMMAND_TYPE__SWAP_CHANNELS_SPECIFIC_PORT_TYPE
} acgETimeSlotManipulationCommandType; 

/* Compatible with Tac49xTdmPortType */
typedef enum 
{
	ACG_SLOT_TYPE__VOICE,
	ACG_SLOT_TYPE__CAS,
	ACG_SLOT_TYPE__VOICE_CONNECTED_SLOT
} acgESlotType; 

/********************************************************************* 
 * Structure Definitions     												  *
 *********************************************************************/

typedef struct
{
	acgECallProgressSignalType    	Type;  /*Call Progress Signal Type.*/
	int							ToneAFrequency;  /* Frequency, expressed in Hz, in range 300-1980 Hz, in steps of 1 Hz. Unused frequencies must be set to zero.*/
	int							ToneB_OrAmpModulationFrequency;  /* Frequency, expressed in Hz, within the range of 300-1980 Hz, in steps of 1 Hz. Unused frequencies must be set to zero. If AM Factor <> 0, then this field represents the AM frequency in the range of 1-128 Hz.*/
	int							ToneALevel;  /* Output level of the low frequency tone, in call progress generation. Output levels, ranging from -63dBm to 0 dBm. Units are in (-dBm)*/
	int							ToneBLevel;  /* Output level of the high frequency tone, in Call Progress generation. Output levels, ranging from -63 dBm to 0 dBm. Units are in (-dBm)*/
	int							DetectionTimeOrCadenceFirstOnOrBurstDuration;  /*!< The content of this field is according to the signal type: @li Continuous (1) = detection time. @li Cadence (2) = Ton state. Burst @li (3) = duration.*/
	int							CadenceFirstOffDuration;  /* When the signal is cadence, this value represents the off duration. Units are in 10 msec. When it is not used, set it to zero.*/
	int							CadenceSecondOnDuration;  /* When the signal is cadence, this value represents the on duration. Units are in 10 msec. When it is not used, set it to zero.*/
	int							CadenceSecondOffDuration;  /* When the signal is cadence, this value represents the off duration. Units are in 10 msec. When it is not used, set it to zero.*/
	int							CadenceThirdOnDuration;  /* When the signal is cadence, this value represents the on duration. Units are in 10 msec. When it is not used, set it to zero.*/
	int							CadenceThirdOffDuration;  /* When the signal is cadence, this value represents the off duration. Units are in 10 msec. When it is not used, set it to zero.*/
	int							CadenceFourthOnDuration;  /* When the signal is cadence, this value represents the on duration. Units are in 10 msec. When it is not used, set it to zero.*/
	int							CadenceFourthOffDuration;  /* When the signal is cadence, this value represents the off duration. Units are in 10 msec. When it is not used, set it to zero.*/
	acgEControl					CadenceVoiceAddedWhileFirstOff;  /* If the signal is Cadence, this value configures whether voice is added to the signal during the silence time. It is used in the call waiting tone. @li 0 = During Off time, mute is generated. @li 1 = During Off time, voice is generated.*/
	acgEControl					CadenceVoiceAddedWhileSecondOff;  /* If the signal is Cadence, this value configures whether voice is added to the signal during the silence time. It is used in the call waiting tone. @li 0 = During Off time, mute is generated. @li 1 = During Off time, voice is generated.*/
	acgEControl					CadenceVoiceAddedWhileThirdOff;  /* If the signal is Cadence, this value configures whether voice is added to the signal during the silence time. It is used in the call waiting tone. @li 0 = During Off time, mute is generated. @li 1 = During Off time, voice is generated.*/
	acgEControl					CadenceVoiceAddedWhileFourthOff;  /* If the signal is Cadence, this value configures whether voice is added to the signal during the silence time. It is used in the call waiting tone. @li 0 = During Off time, mute is generated. @li 1 = During Off time, voice is generated.*/

} acgTCallProgressTone;

typedef struct
{
	char     *pProgram;
	int        ProgramSize;
	int        NumberOfDevices;
	int        iEnableDownload;
	
} acgTDSPDeviceInformation;


typedef struct 
{
	int                     FaxModemJitter;
	int                     BypassJitter;
    acgEFaxAndDataGain    DataBypassOutputGain;
    acgEFaxAndDataGain    FaxBypassOutputGain; 
}acgTAdvancedChannelParameters;

typedef struct 
{
	int 			NumberOfCallProgressTones;	/* Number of defined call progress tones */
	acgTCallProgressTone	CallProgressTones[MAX_TONES_TO_PLAY];

	char			RemoteRecordingIpAddress[ACG_STR_MAX_LEN]; /* IP Address to which the recording packet will be send to */
	int			RemoteRecordingPort;						/* Port to which the recording packet will be send to */
	/**** AC49x Gen 3 related parameters ****/
	acgTDSPDeviceInformation 	PrimaryDSPDeviceInformation;
	acgTDSPDeviceInformation 	SecondaryDSPDeviceInformation;
	acgTAdvancedChannelParameters AdvancedChannelParameters;
	/**** AC48x Gen 2 related parameters ****/
	int 			DspVersionTemplateNumber;

} acgTInitializationParameters;

typedef struct
{
	void     *coefficients;

} acgTFXSInitializationParameters;

typedef struct 
{
	acgECoder	Coder;	/* The channel encoder and decoder codec type */
	int			PTime;	/* The tx packetization time in ms */
	int		TxVoicePayloadType;				/* RTP payload type for transmitted voice packets */
	int		RxVoicePayloadType;				/* RTP payload type for received voice packets */
	int		TxDTMFRelayPayloadType;			/* RTP payload type for transmitted RFC2833 packets */
	int		RxDTMFRelayPayloadType;			/* RTP payload type for received RFC2833 packets */
	int		FaxBypassPayloadType;			/* RTP payload type for fax bypass packets */
	int		ModemByassPayloadType;			/* RTP payload type for modem bypass packets */
	acgESilenceCompressionMode		SilenceCompressionMode;	    /* Encoder Silence Compression Mode. @n Default = SILENCE_COMPRESSION_MODE__DISABLE.*/
	acgEIbsRedundancyLevel        IbsRedundancyLevel;           /* IBS Redundancy Level */
    acgEControl                   RtpRedundancy;                /* RTP Redundancy */
    U8                            TxSignalingRfc2833PayloadType;/* Tx Signaling RFC 2833 Payload Type */
    U8                            RxSignalingRfc2833PayloadType;/* Rx Signaling RFC 2833 Payload Type */
    U8                            Rfc2198PayloadType;           /* RFC 2198 Payload Type */
    U16                           RtcpMeanTxInterval;           /* RTCP Mean Tx Interval */
    int		                      CNameLength;   /*!< RTP canonical name string length. Must be equal to or less than 64 bytes.*/
    U8                            CName[ACG_MAX_RTP_CANONICAL_NAME_STRING_LENGTH];  /* RTP canonical name */
    acgEControl                   TxRtcpFilter;                                     /* RTCP packets arenot transmitted to the network. */ 
    acgEControl                   RxRtcpFilter;                                     /* Received RTCP packets are ignored. */ 
} acgTRTPStreamParameters;

typedef struct
{
	acgESignalingTransportMethod	DTMFTransportMethod;	/* The mothod for DTMF signals transport */
	acgESignalingTransportMethod	CPTTransportMethod;	/* The mothod for Call Progress transfer mode */
    acgEIbsDetectionDirection DetectionRedirection;             /* Configures the IBS detection direction */
    acgEControl DetectCallProgress;                                    /*Enables Call Progress Tones detection*/
    acgEControl DetectDTMFRegister;                                  /*Enables DTMF detection*/    
    acgEControl LongDtmfDetectionEnable;                    /*Long DTMF Detection Enable*/
    int         IbsLevel_minus_dbm;                         /*IBS Level-1 (dBm)*/
} acgTSignalingParameters;

typedef struct
{
	acgEFaxModemTransportMethod	FaxTransferMethod;		/*!< Fax transport method. Default = FAX_MODEM_TRANSPORT__DISABLE.*/
	acgEFaxModemTransportMethod	ModemTransferMethod;	/*!< Modem transport method. Default = FAX_MODEM_TRANSPORT__DISABLE.*/
	acgEFaxMaxRate				FaxMaxRate;				/*!< Fax Relay Max Rate. Default = FAX_MAX_RATE__14_4_KBPS.*/
	acgEControl					FaxErrorCorrection;		/*!< Enable the T.30 fax relay error correction mode Default = CONTROL__ENABLE.*/
	acgEControl					CngRelay;				/*!< CNG Relay on the detection side.  ACG_CONTROL__ENABLE = CNG signal is relayed using T.38 protocol */
	acgECoder					BypassCoder;			/*!< The codec used for fax or modem bypass */
	acgEImageDataRedundancyLevel ImageDataRedundancyLevel; /*< Redundancy for output Image Data (2400?4400 bps). @li 0 = No Redundancy (Default). @li 1-3 = Redundancy level 1-3.*/
	acgET30ControlDataRedundancyLevel T30ControlDataRedundancyLevel;/*!< Redundancy for output T.30 Control Data (300 bps). @n (Default = 0). @li 0 = No Redundancy. @li 1-7 = Redundancy levels 1-7.*/
	
    acgEFaxModemTransportMethod   V21TransferMethod; /*Only ACG_FAX_MODEM_TRANSPORT__DISABLE and     
                                                          ACG_FAX_MODEM_TRANSPORT__BYPASS are supported */    
    acgEFaxModemTransportMethod   V22TransferMethod; /*Only ACG_FAX_MODEM_TRANSPORT__DISABLE and     
                                                          ACG_FAX_MODEM_TRANSPORT__BYPASS are supported */    
    acgEFaxModemTransportMethod   V23TransferMethod; /*Only ACG_FAX_MODEM_TRANSPORT__DISABLE and     
                                                          ACG_FAX_MODEM_TRANSPORT__BYPASS are supported */    
    acgEFaxModemTransportMethod   V32TransferMethod; /*Only ACG_FAX_MODEM_TRANSPORT__DISABLE and     
                                                          ACG_FAX_MODEM_TRANSPORT__BYPASS are supported */    
    acgEFaxModemTransportMethod   V34TransferMethod; /*Only ACG_FAX_MODEM_TRANSPORT__DISABLE and     
                                                          ACG_FAX_MODEM_TRANSPORT__BYPASS are supported */    
    acgEFaxModemTransportMethod   Bell103TransferMethod; /*Only ACG_FAX_MODEM_TRANSPORT__DISABLE and     
                                                          ACG_FAX_MODEM_TRANSPORT__BYPASS are supported */    
    acgEFaxModemTransportMethod   V34FaxTransferMethod;    
    acgECedTransferMode           CEDTransferMode;    
} acgTFaxModemParameters;

typedef struct
{
	acgECallerIdStandard	CallerIdStandard;			/*!< Caller ID standard. Default = CALLER_ID_STANDARD__TELCORDIA_BELLCORE.*/
	/* Currently not supported */
	acgEControl			CallerIdOnHookDetection;	/*!< Enables the Call ID "on hook" service (Type 1) detection. Default = CONTROL__DISABLE.*/
	acgEControl			CallerIdOffHookDetection;	/*!< Enables the Caller ID "off hook" service (Type 2, CID on Call Waiting) detection. Default = CONTROL__DISABLE.*/
} acgTCallerIDParameters;


typedef struct
{
	acgEVoiceGain		VoiceOutputGain;		/*!< Voice decoder output gain. Default = VOICE_GAIN__0_DB.*/
	acgEVoiceGain		InputGain;			/*!< Input gain. Default = VOICE_GAIN__0_DB.*/
	acgEControl		AgcEnable;			/*!< Enables Automatic Gain Control. Default = CONTROL__DISABLE.*/
	acgEAGCLocation   	AgcLocation;              	/*!< @li ENCODER_INPUT = AGC is placed at encoder input (default). @li DECODER_OUTPUT = AGC is placed at decoder output.*/
	int             		AgcTargetEnergy;	    	/*!< The desired output energy in -dBm. @n Default = -19 dBm.                               */
} acgTGainParameters;

typedef struct
{
	acgEDynamicJitterBufferOptimizationFactor	DynamicJitterBufferOptimizationFactor;  /*!< Dynamic Jitter Buffer Optimization Factor. Default = 7. @li 0 = Static Jitter Buffer. @li 1-12 = This an empirical, user-defined factor, where: 1 is optimal for delay and 12 is optimal for voice quality. @li 13 = Optimal for fax and data transfer. @li 14-15 = Reserved.*/
	int		VoiceJitterBufferMinDelay;				/*!< Minimum delay of Dynamic Jitter Buffer, or fixed delay in Static Jitter Buffer, in msec resolution. @n Range: 0 - 280 msec. @n Default = 35 msec. */
	int		VoiceJitterBufferMaxDelay;				/*!< Maximum delay of Dynamic Jitter Buffer in msec resolution. @n Range: 0 - 300 msec. @n Default = 150 msec. @n @b Note: The actual maximum size limit varies from product to product.*/
} acgTJitterBufferParameters;

typedef struct
{
	acgEControl EchoCanceler;         /*!< Echo canceler operation mode. Default = CONTROL__ENABLE.*/
	acgEEchoCancelerNonLinearProcessor EchoCancelerNonLinearProcessor;  /*!< Echo canceler non-linear processor mode. @n Default = ECHO_CANCELER_NON_LINEAR_PROCESSOR_MODE__MEDIUM_SENSITIVITY. @n Refer to ::Tac49xEchoCancelerNonLinearProcessor.*/
	acgEEchoCancelerLength EchoCancelerLength;/*!< Echo canceler length. Default = ACG_ECHO_CANCELER_LENGTH__32_MSEC.*/
} acgTEchoCancellerParameters;

typedef struct
{
	acgEControl RtcpXrEnable;         /*Enables RTCP-XR (Voice Quality Monitor - RFC3611) */
} acgTVoiceQualityMonitoringParameters;

typedef struct 
{
    acgETestMode TestMode;    
}acgTMiscParameters;

typedef struct 
{
	acgTRTPStreamParameters		            RTPStreamParameters;	/* The RTP media stream parameters */
	acgTSignalingParameters			        SignalingParameters;	/* In-band Signaling parameetrs */
	acgTFaxModemParameters		            FaxModemParameters;		/* Fax and Modem configuration parameters */
	acgTCallerIDParameters			        CallerIDParameters;		/* Caller ID Parameters */
	acgTGainParameters			            GainParameters;			/* Caller ID Parameters */
	acgTJitterBufferParameters		        JitterBufferParameters;	/* Jitter Buffer Parameters */
	acgTEchoCancellerParameters		        EchoCancellerParameters;/* Echo Canceller Parameters */
	acgTVoiceQualityMonitoringParameters    VoiceQualityMonitoringParameters;/* RTCP-XR Parameters */
    acgTMiscParameters                      MiscellaneousParameters;   
} acgTChannelParameters;

typedef struct
{
    int gpio_no;
    int cnt;
}gpioinfo;

typedef struct
{
	acgEToneType	Type;			/* The type of tone (DTMF, call progress tone, ...) */
	int			ToneIndex;		/* The tone to play - 0-15 for DTMF or Call Progress Tone Index */
} acgTToneElement;


typedef struct
{
	acgTToneElement	ToneElement;
	acgEDirection		Direction;		/* The generation direction - towards local TDM or remote network */
	int				OnDuration;		/*On duration in msec.*/
	int				OffDuration;		/* Off duration in msec.*/

} acgTTone;


typedef struct
{
	acgEDirection	Direction;		/* The generation direction - towards local TDM or remote network */
    	int		        OnDuration;            /* On duration (identical to all digits) in msec.*/                                                            
    	int			OffDuration;           /* Off duration (identical to all digits) in msec.*/                                                           

	int			NumOfTones;		/* Number of tones to play */
	acgTToneElement	Tones[MAX_TONES_TO_PLAY];	/* Array of indices to play (0-15 for DTMF or Call Progress Tone Index) */
} acgTToneSequence;


typedef struct
{
    int					SignalLevel_minus_dbm;  /*!< Signal Level, in -dBm resolution. For User-Defined Tones signals or Call Progress Tones signals, if the value 0 is defined in this field (SignalLevel) the levels defined in the ::Ac49xUserDefinedTonesDeviceConfiguration or ::Ac49xCallProgressDeviceConfiguration functions will be generated.*/
    int					Digit;                  /*!< Digit to be played. Refer to <A HREF="DIGIT Field According to Signaling Type.html">Digit Field According to Signaling Type table</A> and to <A HREF="Advanced Signals.html"> Advanced Signals Digits</A>.*/
    unsigned int			SignalDuration;         /*!< The signal duration, in 1 msec resolution.*/
   acgEIbsSystemCode          SystemCode;             /*!< The IBS system. @n ON_THE_FLY_TONES = Generates a signal combined of UserDefinedToneA, UserDefinedToneB and UserDefinedToneC. @n Refer to <A HREF="IBS_SYSTEM_CODE__ON_THE_FLY_TONES - On-the-Fly Digits.html">on-the-fly digits</A>.*/
}acgTToneExtendedElement;

typedef struct                 
    {                                   
    acgEDirection  GeneratorRedirection;                     /*!< Tone Generation Redirection.*/
    acgEControl    Repeat;                                   /*!< Repeat the string. @li CONTROL__DISABLE = String is generated only once. @li CONTROL__ENABLE = String is generated repeatedly.*/
    int		     NumberOfDigits;                           /*!< Number of digits in the string. Range: 1-6.*/
    unsigned int		     TotalDuration;                            /*!< If the field Repeat is enabled, this field defines the total duration of the repeated string. If Repeat is disabled, the field is ignored. Resolution is 1 msec.*/
    unsigned short		     UserDefinedToneA;                         /*!< This field enables users to generate any signal combined of three frequencies (UserDefinedToneA, UserDefinedToneB and UserDefinedToneC) on the fly. The field is valid only if users set parameter SystemCode to option ON_THE_FLY_TONES. Range is 0 - 3990 Hz, in units of Hertz.*/
    unsigned short		     UserDefinedToneB;                         /*!< This field enables users to generate any signal combined of three frequencies (UserDefinedToneA, UserDefinedToneB and UserDefinedToneC) on the fly. The field is valid only if users set the parameter SystemCode to ON_THE_FLY_TONES. Range is 0 - 3990 Hz, in units of Hertz. When generating a single frequency signal, set this field to 0.*/
    unsigned short		     UserDefinedToneC;                         /*!< This field enables users to generate any signal combined of three frequencies (UserDefinedToneA, UserDefinedToneB and UserDefinedToneC) on the fly. The field is valid only if users set the parameter SystemCode to ON_THE_FLY_TONES. Range is 0 - 3990 Hz, in units of Hertz. When generating a single frequency signal, set this field to 0.*/
    unsigned short		     UserDefinedToneD;                         /*!< This field enables users to generate any signal combined of three frequencies (UserDefinedToneA, UserDefinedToneB and UserDefinedToneC) on the fly. The field is valid only if users set the parameter SystemCode to ON_THE_FLY_TONES. Range is 0 - 3990 Hz, in units of Hertz. When generating a single frequency signal, set this field to 0.*/
    unsigned char		     AmFactor;                                 /*!< Amplitude Modulation Factor k of the on-the-fly tones.*/
    acgTToneExtendedElement	Tones[MAX_TONE_EXTENDED_TO_PLAY];  /*!< Signal to be played.*/
    } acgTToneExtended;          

	
typedef struct
{
	acgECallerIdServiceType			CallerIdServiceType;	/* The caller Id service type (type 1 or type 2) */
	char 							Number[MAX_DIAL_STRING_LEN]; 			/* Caller's number */
	char 							Name[MAX_CALLER_ID_LEN]; 				/* Caller's user-id */

	/*** Currently Time and Addr are not supported ***/
	char 							Time[TIME_STRING_LEN];					 /* System time, the format is MMDDhhmm */
	char 							Addr[ACG_STR_MAX_LEN]; 					/* Caller's IP address */

	U8				CallType;

	/*** Currently NumberStatus and NameStatus are not supported ***/
	acgEPhoneCallerNumberStatus 	NumberStatus; /* is 'number' valid? */
	acgEPhoneCallerNameStatus		NameStatus; /* is 'name' valid? */
} acgTCallerId;

typedef struct
{
	acgECallerIdServiceType		CallerIdServiceType;	/* The caller Id service type (type 1 or type 2) */
	acgEControl				Activate;				/* Activation of the message waiting indicator*/
} acgTMWI;

typedef struct
{
	int						ConferenceChannelMate;       /* If two of the conference channels are on the same core, this parameter describes the channel ID of the channel with which the conference is performed.*/
	acgE3WayConferenceMode	Mode;                          		/* 3-Way Conference Mode */
} acgT3WayConference;

typedef struct
{
	acgERTCPRegularParameter	RTCPParamIndex;       /* Index of the modified parameter */
	int						RTCPParamValue;       /* The value of the parameter*/    
} acgTRegularRTCPParamters;

typedef volatile struct
{
    int TimeX1Sec;                     /*!<  This field represents the NTP time, in seconds. */
    int TimeX1SecFraction;             /*!<  This field represents the NTP time, in fractions of seconds. This field's resolution is 1 / (2^32) seconds. */
} acgTNtpTimeStamp;

typedef struct
{
	int 						Channel;				/* The channel to set debug information */
	acgEControl				NetworkRecording;	/* Recording of the network packets (TDM Out) */
	acgEControl				TDMRecording;		/* Recording of the TDM input (TDM In) */
} acgTSpecificRecordingInformation;

typedef struct
{
	acgEControl					PacketRecording;				/* DSP packet recording (Device, Channel, RTP information) */
	acgTSpecificRecordingInformation	SpecificRecordingInformation;	/* Network and TDM Recording information */
} acgTDebugInformation;

typedef struct
{
	char* 		Buff;			/* Pointer to the transmitted packet */
	int 			Len;				/* Size of the transmitted packet in bytes */
	acgEProtocol	MediaProtocol;	/* Media type to be decoded */
} acgTDecodeMedia;


typedef struct
{
	int 			Channel;				/* [Out] - The channel from which the media packet is recevied */
	char* 		Buff;				/* [Out] - Pointer to the received packet */
	int 			Len;					/* [Out] - Size of the received packet in bytes */
	acgEProtocol	MediaProtocol;		/* [Out] - Media type of the received packet */
	acgEPollingMethod	PollingMethod;	/* [In] - Method of acgGetMedia() - Blocking / Non Blocking */
} acgTGetMedia;

typedef struct
{
	acgEPollingMethod	PollingMethod;	/* [In] - Method of acgGetEvent() - Blocking / Non Blocking */
	char* 			Buff;				/* [Out] - Pointer to the received event */
} acgTGetEventInfo;

typedef struct 
{
	char *FirmwareVersion;	
}acgTGetVersionInfo;

typedef struct 
{
	int 				Channel;		/* The channel from which the event is recevied */
	acgEEventType	EventType; 	/* The event type code */
	union
	{	
		struct
		{
			acgEToneType	Type;		/* The type of tone (DTMF, call progress tone, ...) */
			int			ToneIndex;	/* The tone to play - 0-15 for DTMF or Call Progress Tone Index */
			acgEDirection	Direction;	/* The generation direction - towards local TDM or remote network */
			int			Duration;		/* Tone duration in [ms] - relevant for Call Progress Tone */
		} Tone;	/* DTMF or Call Progress Tone start or end event information */

		/*** Currently Caller ID detection is not supported ***/
		struct
		{
			int TBD;	/* Need to be define */
		} CallerID;	/* Caller ID event information */

		struct
		{
			acgEInitiatingFlag	InitiatingFlag;		/* Indicating whether the local or remote initiated the signal */
			acgEReasonIdentifierCode ReasonIdentifierCode;
		} FaxModem;	/* Fax or Modem detection event information */

		union
		{
			struct
			{
				acgERTCPEventType  ReportType;    	/*According to enum acgERTCPEventType*/ 
				unsigned int  PacketCount;   			/*Number of packets transmitted*/
				unsigned int  OctetCount;   			/*Number of Octets transmitted*/
				unsigned int  Jitter;        				/*Delay Jitter [msec]*/
				unsigned int  RoundTrip;     			/*Local<->Remote round trip delay [msec]*/
				unsigned int  FractionLost;  			/*Fraction of packets lost */ 
				unsigned int  CumLost;       			/*Number of packets lost */
				unsigned int  ExtHighSeq;    			/*Extended highest sequence number recieved*/
				unsigned int  SsrcSender;    			/*Synchronization Source of sender*/
				unsigned int  RtpTimeStamp;  		/*RTP TimeStamp of the RTCP report*/
				unsigned int  PacketsReceived; 		/*Number of received packets*/
				unsigned int  OctetsReceived; 		/*Number of transmitted octets */
			}RTCP;

			struct
			{
				acgERTCPXREventType ReportType;	/*According to enum acgERTCPXREventType*/ 
				unsigned char MOS_LQ;				/*The estimated mean opinion score for listening quality */
				unsigned char MOS_CQ;				/*The estimated mean opinion score for conversational quality*/
				unsigned char RFactor; 		  		/*A voice quality metric describing the segment of the call that is carried over this RTP session*/
			}RTCP_XR;
		
		} RTCPInfo;	/* Caller ID event information */

	} EventInfo;
} acgTEventInfo;

#define MAX_PLAYBACK_BUF 				1600
#define DEFAULT_PLAYBACK_BUF 			1460
#define MIN_WAV_HEADER_SIZE				44

typedef struct
{
    acgECoder Coder;
	int size;
	unsigned char buf[MAX_PLAYBACK_BUF];
	unsigned char DataRead[8];
} acgTPlaybackData;

typedef struct
{
    acgETimeSlotManipulationCommandType	    CommandType;
	acgESlotType                            RxSlotType;  
	U8                                      RxChannelId;
	acgESlotType				            TxSlotType;
	U8						                TxChannelId;
} acgTTimeSlotManipulationAttr;

#endif /*_ACGATEWAYDEFS_H */

