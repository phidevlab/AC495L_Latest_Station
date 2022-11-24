/********************************************************************
 * AudioCodes improved application general definitions	            *
 *																	*
 * File Name:	acl_voip_conf.h 									*
 *																	*
 * Copyright (C) 2008 AudioCodes Ltd.								*
 *																	*
 * Modifications :													*
 *																	*
 * 26/02/08 - Coding started.										*
 *																	*
 ********************************************************************/
#ifndef _VOIP_CONF_H_
#define _VOIP_CONF_H_

#include "acl_call_mngr.h"
#include "voip_main.h"
#ifdef CONFIG_GTEK
#include "version.h"
#endif

/********************************************/
/************	General Definitions    ************/
/********************************************/

#define VOIP_LOCAL_PORT_POOL        7090
#define VOIP_LOCAL_PORT_POOL_RANGE (MAX_NUM_OF_LINES * 4 + 4)

#define SYSLOG_APP_LOG_PORT         514 /*same as SYSLOG_UDP_PORT, but SYSLOG_UDP_PORT isn't "public"*/
#define SYSLOG_APP_DEF_ADDR         "0.0.0.0" 
#define SYSLOG_PRINT_TO_UDP         0x1
#define SYSLOG_PRINT_TO_TERMINAL    0x2
#define SYSLOG_PRINT_TO_TERMINAL_AND_UDP    0x3

#define PACKET_RECORDING_DEF_REMOTE_ADDR	"0.0.0.0"
#define PACKET_RECORDING_DEF_REMOTE_PORT	6666

/***************************************************************************/
/* Regional Settings Parameters Data Structures and Definitions */
#define REGIONAL_SETTINGS_MAX_COUNTRY_NAME_LEN			32
#define REGIONAL_SETTINGS_FXS_COEFFICIENTS_PARAMS_LEN	80
#define REGIONAL_SETTINGS_FXS_CADENCE_PARAMS_LEN		18

#define MAX_PTIME_VALS              6
#define CONFIGURATION_FILE_NAME_SIZE	128
#define CONFIGURATION_FILE_NAME         "/audc/voip/VoIPCfgFile_Proxy_improved_appl.cfg"


#if defined(S_PLATFORM)

#ifdef CONFIG_GTEK
#define USER_AGENT_STRING 	"3xxHD_"IP_Phone_3xxHD_VERSION"_ACL/"ACL_VOIP_TOOLKIT_VERSION
#else
#define USER_AGENT_STRING 	"AC494_Linux_SDK/"ACL_VOIP_TOOLKIT_VERSION
#endif

#elif defined(B_CEVA_PLATFORM)
#define USER_AGENT_STRING 	"VoIPerfect_CPE_VoIP_Toolkit/"ACL_VOIP_TOOLKIT_VERSION

#elif defined(AC48_CPE_VOIP_TOOLKIT)

#ifdef AC488_CPE_VOIP_TOOLKIT
#define USER_AGENT_STRING 	"AC488_CPE_VoIP_Toolkit/"ACL_VOIP_TOOLKIT_VERSION
#else
#define USER_AGENT_STRING 	"AC483_CPE_VoIP_Toolkit/"ACL_VOIP_TOOLKIT_VERSION
#endif /*AC488_CPE_VOIP_TOOLKIT*/

#endif


/***************************************************************************/


/************************************************/
/************	Enumerators Definitions    ************/
/************************************************/

/***************************************************************************/
/* Regional Settings enumerators */
typedef enum {
	DSP_REGIONAL_SETTINGS_CALL_PROGRESS_DIAL_TONE,
	DSP_REGIONAL_SETTINGS_CALL_PROGRESS_RINGBACK_TONE,
	DSP_REGIONAL_SETTINGS_CALL_PROGRESS_BUSY_TONE,
	DSP_REGIONAL_SETTINGS_CALL_PROGRESS_REORDER_TONE,
	DSP_REGIONAL_SETTINGS_CALL_PROGRESS_OFF_HOOK_WARNING_TONE,
	DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_TONE,
	DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_RINGBACK_TONE,
	DSP_REGIONAL_SETTINGS_CALL_PROGRESS_STUTTER_TONE,
	DSP_REGIONAL_SETTINGS_CALL_PROGRESS_HOWLER_TONE,
	DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_TONE_2,
	DSP_REGIONAL_SETTINGS_CALL_PROGRESS_TONE_LAST,
} enum_dsp_regional_settings_params_call_progress_t;
/***************************************************************************/
#if 0
//NIRP 070624 AC52917 Distinctive ringing - FXS level support
typedef enum {
	SLIC_RING_CADENCE_PROFILE_NA = -1, /* don't care, don't change */
	SLIC_RING_CADENCE_PROFILE_1,
	SLIC_RING_CADENCE_PROFILE_2,
	SLIC_RING_CADENCE_PROFILE_3,
	SLIC_RING_CADENCE_PROFILE_LAST,
} enum_ring_cadence_profile_t;
#endif



/************************************************/
/************	Structures Definitions    **************/
/************************************************/

/***************************************************************************/
/* Regional Settings structures */

typedef struct {

	enum_dsp_regional_settings_params_call_progress_t enum_dsp_regional_settings_params_call_progress;
	unsigned int	EnableField;// If EnableField =0 the the tone info will not be configured in the DSP
	unsigned int	ToneCadenceType;// Tac49xCallProgressSignalType: Call Progress Signal Type(Continues, Cadence....) 
	unsigned int	FreqA;
	unsigned int	FreqB;
	unsigned int	FreqLevelA;
	unsigned int	FreqLevelB;
	unsigned int	ToneOn0;
	unsigned int	ToneOff0;
	unsigned int	ToneOn1;
	unsigned int	ToneOff1;
	unsigned int	ToneOn2;
	unsigned int	ToneOff2;
	unsigned int	ToneOn3;
	unsigned int	ToneOff3;

} dsp_regional_settings_params_call_progress_t;


typedef struct {

	dsp_regional_settings_params_call_progress_t	dsp_regional_settings_params_call_progressArray[DSP_REGIONAL_SETTINGS_CALL_PROGRESS_TONE_LAST];
	unsigned int									CallerIDStandard;//According to Tac49xCallerIdStandard

} dsp_regional_settings_params_t;


typedef struct {

	unsigned char	FXS_COEFFICIENTS[REGIONAL_SETTINGS_FXS_COEFFICIENTS_PARAMS_LEN ];
	unsigned char	FXS_RING_CAD_PROFILE[REGIONAL_SETTINGS_FXS_CADENCE_PARAMS_LEN];

} slic_regional_settings_params_t;

typedef struct {    
	
	int             	iPerformFXSCalibration;    
	unsigned char	Reg98; 
	unsigned char	Reg99;
	unsigned char	Reg100; 
	unsigned char	Reg101;  
	unsigned char	Reg102;  
	unsigned char	Reg103; 
	unsigned char	Reg105;  
	unsigned char	Reg107;
} slic_calibration_results_t;

typedef struct {

	unsigned char					CountryName[REGIONAL_SETTINGS_MAX_COUNTRY_NAME_LEN];
	dsp_regional_settings_params_t	dsp_regional_settings_params;
	slic_regional_settings_params_t 	slic_regional_settings_params;
   	slic_calibration_results_t 		slic_calibration_resultsArray[VOIP_ABSOLUTE_MAX_NUMBER_OF_LINES];
	int                             			iNumOfFXSLines;  
	int                             			iNumOfFXOLines;

} regional_settings_params_t;


typedef struct {

	int IsOpenChannelDBInitialized;
	int IsActivateRTTPDBInitialized;
	int ChannelID;							                /*!< DSP channel ID . */
	//Open / Update channel                         
	int             VoiceOutputGainChannelArr[CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES];			    /*!< valuse are between -31 to +31, Voice decoder output gain. Default = 0.	*/
	int             AutomaticAgcEnable;					    /*!< Enables Automatic Gain Control. Default = 0 (disabled) */
	acgEAGCLocation    AgcLocation;                            /*!< @li ENCODER_INPUT = AGC is placed at encoder input (default). @li DECODER_OUTPUT = AGC is placed at decoder output.*/
	int             AgcTargetEnergy;					    /*!< The desired output energy in -dBm. @n Default = -19 dBm.                               */
	int             VoiceJitterBufferMinDelay;              /*!< Minimum delay of Dynamic Jitter Buffer, or fixed delay in Static Jitter Buffer, in msec resolution. @n Range: 0 - 160 msec. @n Default = 35 msec. */
	int             DynamicJitterBufferOptimizationFactor;  /*!< Dynamic Jitter Buffer Optimization Factor. Default = 7. @li 0 = Static Jitter Buffer. @li 1-12 = This an empirical, user-defined factor, where: 1 is optimal for delay and 12 is optimal for voice quality. @li 13 = Optimal for fax and data transfer. @li 14-15 = Reserved.*/
	int             EchoCancelerMode;                       /*!< Echo canceler operation mode. Default = 1 (enabled).*/
	int             CngRelayEnable;                         /*!< CNG Relay Enable on the detection side. On the generation side, CNG relay is always enabled. default = 0 (disabled) - CNG is transferred through voice (default). Enable - CNG signal is transferred in T.38 packet.*/
	acgEFaxMaxRate     FaxMaxRate;                             /*!< Fax Relay Max Rate. Default = ACG_FAX_MAX_RATE__14_4_KBPS.*/
	int             FaxErrorCorrectionMode;                 /*!< Enable the T.30 fax relay error correction mode Default = 1 (enabled) .*/
	int		 RtcpXrEnable[CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES];	/* Enable the RTCP-XR (Voice Quality Monitor - RFC3611) */
	//Activate RTP
	int SilenceCompressionMode;                         /*!< Encoder Silence Compression Mode. @n Default = ACG_SILENCE_COMPRESSION_MODE__DISABLE.*/
	int PcmAdPcmComfortNoiseRtpPayloadTypeEnable;       /*!< PCM/ADPCM Comfort Noise Payload Type Enable. (If enabled, then the decoder can support both modes) @li CONTROL__DISABLE = Payload Type of SID is similar to Voice payload type (default) @li CONTROL__ENABLE = PCM/ADPCM SID has a different RTP payload Type*/
	int g726_payload_number;                            /* Nirp 060709 add support for configuring payload type via WBM */
	int dtmf_payload_number;                            /* Nirp 060709 add support for configuring payload type via WBM */
	int bypassCoder;                                    /*!< Bypass Coder Type. Default = ACG_CODER__G711ALAW.*/ 
	int faxBypassPayloadType;                           /*!< Fax Bypass Payload Type. Default = 100. Tulip default 102 */ 
	int dataBypassPayloadType;                          /*!< Data Bypass Payload Type. Default= 101. Tulip default 103 */

	/*EA - 080521 - AC64079 - New feature request for Starhub - T38FaxMaxBuffer should be ... configurable...*/
	int FaxMaxBuffer;
	int FaxMaxDatagram;
	int ImageDataRedundancy;
	int T30ControlDataRedundancy;
	int FaxModemJit;
	// Patch for Alarm message detection - automaticlly switch to inband ACL NB 080416 
	int autoSwitchToDtmfInband;

} voip_dsp_advanced_channel_configuration_t;


typedef struct codec_info_t {
    rtp_payload_type_t payload_type;
    int ptime_vals[MAX_PTIME_VALS+1];
    int brate_hi; /* normally -1, for G723 - 0/1 */
} codec_info_t;


/************************************************/
/***************	Prototypes    ******************/
/************************************************/

void voip_conf_init(void);
void voip_conf_uninit(void);
#ifdef CONFIG_RG_VOIP_TUTORIAL
void voip_conf_tx_poll_load_new(void);
#endif

//nirp dsp cli
voip_dial_match_t voip_conf_match_number(char *dial_string, long *timer_duration, int is_registered_with_proxy);
void voip_conf_map_number(char *dial_string, char **user_id, char **user_addr);
int voip_conf_local_address_get(char *ip_address);
void voip_conf_localtime_get(struct tm *tm_time);//NIRP 070111
void voip_conf_line_conf_get(int line_number, line_conf_t *conf);
void voip_conf_signalling_conf_get(acl_call_mngr_params_t *params);
void voip_conf_codec_info_get(int codec_number, codec_t *codec);
void voip_conf_dynamic_payloads_info_get(void);//Nirp 060709 configurable payload type - update DTMF payload type parameters
void voip_conf_dialing_param_get(acl_call_mngr_params_t *params);/* ACL NB 070528 get all dialing parameters*/

//nirp regional
void voip_conf_regional_settings_info_sync(void);
int voip_conf_regional_settings_find_country_in_DB(int* pIndex, char* CountryName);

void voip_conf_g723_high_bitrate_get(int *pG723_bitrate);/* ACL NIRP 061224 */
void voip_conf_faxDataMethod_get(int *pFaxDataMethod); /* ACL NB 06*/
void voip_conf_bypassCoder_get(int *pBypassCoder);     /* ACL NB 06*/
int voip_conf_dialtone_timeout_get(void);/* Nirp 060720 */
int voip_conf_tone_timeout_get(int tone);/* ACL NB 070606*/
int voip_conf_unansweredCall_timeout_get(void);/* ACL NB 070718 */
/* AlexChen SDK upgrade 20090414 */
void  voip_conf_regret_call_enable_get(char *pIsEnable);
int voip_conf_regret_call_timeout_get(void);
int expandDigitMap( void );	/* AlexChen 20090331 SDK upgrade */
int digitMapMatch( char * dialStr );
int digit_map_style_set( void );

void voip_conf_dsp_params_get(regional_settings_params_t                **regionalSettings, 
                              voip_dsp_advanced_channel_configuration_t *advancedChannelConfiguration);

//Nirp advanced channel configuration
void voip_conf_get_advanced_channel_configuration(voip_dsp_advanced_channel_configuration_t* p_voip_dsp_advanced_channel_configuration);
/* ACL NB 061205 */
void voip_conf_mwi_get(voip_mwi_params_t *params);
/* AlexChen 20090408 SDK upgrade */
void  voip_conf_call_id_type2_enable_get(char *pIsEnable);
/* ACL NB 061210 */
void voip_conf_cfw_type_get(voip_cfw_params_t *params);
void voip_conf_stutter_tone_dur_get(int *duration);
void voip_conf_cfw_keySequence_get(char *keySequence);
//NIRP 070104
void voip_conf_out_of_service_tone_get(int *out_of_service_tone);

/* ACL NB 061225 */
void voip_conf_ua_get(char *buff);
 
/* 070111 Yair: write voip external configuration to acl_conf */
void voip_conf_write_ext(int fd);

/* ACL NB 070118 */
void voip_conf_voip_log_lvl_get(int *pLogLevel);

/* ACL NB 070208 */
int voip_conf_ext_cfw_dest_set(int idx, char *dest);
int voip_conf_ext_cfw_dest_get(int idx, char **dest);

/* AlexChen 20090407 SDK upgrade */
void voip_conf_dnd_keySequence_get(char *keySequence);
int  voip_conf_dnd_enable_get(char *pIsEnable);
void voip_conf_ext_is_redundant_proxy_set(int IsRedundatProxy);
int voip_conf_ext_is_redundant_proxy_get(void);
int voip_conf_dnd_line_set(int idx, int isEnable);
int voip_conf_dnd_line_get(int idx,char *pIsEnable );
/* AlexChen 20090408 SDK upgrade */
int voip_conf_remote_fax_side_reinvite_get();

/* ACL NB 070220 */
int voip_conf_ext_sync_check_set(int sync_check);
/* ACL NB 070614 get prack enable parameter */
void  voip_conf_prack_enable_get(char *pIsEnable);
void  voip_conf_fxs_polarity_revasal_enable_get(char *pIsEnable);
/* ACL NB 070718 get FAxMaxRate */
void  voip_conf_fax_max_rate_get(int *pMaxRate);
/* AlexChen 20090409 SDK upgrade for mgcp fax */
void  voip_conf_fax_end_report_enable_get(char *pIsEnable);
void  voip_conf_fax_audio_startup_enable_get(char *pIsEnable);
void  voip_conf_fax_audio_startup_payload_get(int *payload);
void  voip_conf_fax_max_buffer_get(int *pMaxBuffer);
void  voip_conf_fax_max_datagram_get(int *pMaxDatagram);

void voip_conf_get_sdp_include_ptime(char *pInclude_ptime);
/* ACL NB 070729 get rport enable parameter */
void  voip_conf_rport_enable_get(char *pIsEnable);
/* ACL NB 071014 get KA (using options) enable parameter */
void  voip_conf_ka_options_enabled_get(char *pIsEnable);
void  voip_conf_ka_options_timeout_get(int *pTimeout);
/*EA - 071121 RFC 4240 conference*/
void  voip_conf_remote_conference_get(int *remoteConference);
void  voip_conf_remote_conference_conf_addr_get(char **confAddr);
char *voip_conf_remote_conference_mac_addr_get(void);
/* ACL NB 071118 get blind transfer enabled parameter */
void  voip_conf_blind_transfer_enable_get(char *pIsEnable);
/*EA - 071225 Enabling Radvision's logs through acl_conf */
int voip_conf_rv_log_filter_get(void);

int voip_conf_get_number_of_lines(analog_port_type_t AnalogPortType);

#ifdef RV_CFLAG_TLS
/*EA - 071231 - VI#59630 support SIP over TLS */
int voip_conf_is_transport_protocol_tls(void);
#endif /*RV_CFLAG_TLS*/
/* ACL NB 071031 added Stun Client  support*/
void  voip_conf_stun_enable_get(char *pIsEnable);
void  voip_conf_stun_get(voip_stun_params_t *pStunParms);
void voip_conf_key_Sequence_sip_info_strings_get(char **header, char **body);
/*EA 080122 - VI#60671 - for Setting Syslog IP address for application logs through GUI*/
void voip_conf_syslog_server_info_get(int *serverPortForAppSyslog, char **serverAddrForAppSyslog, int *udpTerminalFlag);

int voip_conf_util_load_cfg_file(char* fileName);
void voip_conf_test(void);

int  voip_conf_packet_recording_enable_get(void);
int  voip_conf_network_recording_enable_get(void);
int  voip_conf_tdm_recording_enable_get(void);
int  voip_conf_tdm_network_recording_channel_get(void);
/* AlexChen SDK upgrade 20090415 */
/*ACL XY 090106 - Fax Fallback. Configurable option for updating fax mode to transparent upon "audio" Re-invite from remote */
void  voip_conf_update_fax_to_transparent_enable_get(char *pIsEnable);
/*EA - 090107 - AC68794 - Make the 'Mute RTP' option (upon hold) configurable*/
void  voip_conf_rtp_mute_on_hold_get(char *pMuteRtp);

void voip_conf_debug_info_get(int *packetRecordingPort, char *packetRecordingAddr);

int handle_debug_cmd(void *arg, int DebugType);
int voip_conf_get_number_of_lines(analog_port_type_t AnalogPortType);

#endif
