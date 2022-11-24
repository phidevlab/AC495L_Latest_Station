/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	acl_voip_conf.c                                     *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 26/02/08 - Coding started.                                       *
 *																	*
 ********************************************************************/

#include <sys/time.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "acl_voip_conf.h"
#include "voip_main.h"
#include "acl_common.h"
#include "acl_line_connection.h"
#include "voip_utils.h"
#include "voip_rtp_utils.h"
#include "rvstr.h"

#include "cip_def.h"

#ifdef CONFIG_RG_AUDIO_MGT
#include <vendor/hw_config.h>
#endif

#define DEBUGGING_PARSER 0
 // #define  VOIP_CONF_DEBUG//Nirp - add debug information to the module
#ifdef CONFIG_RG_VOIP_RV_SIP
#include <ipplog.h>
/* ER08 - Added macro for error messages (not regular debug messages) that shouls go into the RV logs */
#define DPRINTK_ERR(a1,a2...)	IppLogMessage(RV_TRUE, "[%s:%4d] " a1,__FUNCTION__, __LINE__, ##a2)
#else /* CONFIG_RG_VOIP_RV_SIP */
#define DPRINTK_ERR(a1,a2...)	printf("[%s:%4d] ERROR: " a1,__FUNCTION__, __LINE__, ##a2)
#endif /* CONFIG_RG_VOIP_RV_SIP */

#ifdef VOIP_CONF_DEBUG
#ifdef CONFIG_RG_VOIP_RV_SIP
/*071016 - EA did this change during work on VI#57767 in order to syncronize our prntings with RV*/
#define DPRINTK(a1,a2...)	IppLogMessage(RV_FALSE, "[%s:%4d] " a1,__FUNCTION__, __LINE__, ##a2)
#else /* CONFIG_RG_VOIP_RV_SIP */
#define DPRINTK(a1,a2...)	printf("[%s:%4d] " a1,__FUNCTION__, __LINE__, ##a2)
#endif /* CONFIG_RG_VOIP_RV_SIP */
#else /* VOIP_CONF_DEBUG */
#define DPRINTK(a1,a2...)
#endif /* VOIP_CONF_DEBUG */

// by itzik - 19072006 - digitmap
int compareDialStr2digitMap(char *digitMap, char *dialStr);

int hex2int(char *str);

/* AlexChen 20090402 SDK upgrade . is_redundant_proxy */
extern char g_cAcl_cfgfilename[];
//nirp regional
static int iCurrentRGSIndex=0;

// by itzik - for adding the port number at the format addr:port
static char user_addr_temp[AC_MAX_HOST_NAME_LEN+1];

/* AlexChen 20090331 for SDK upgrade */
static digit_map_cb_t  stDigitMap;
static digit_map_type_t digitmap_type = DIGIT_MAP_UNKNOW_STYLE;

#define MAX_USER_AGENT_LEN          128
#define MAX_CODEC_NAME_LEN          16
#define MAX_SIP_INFO_INFORMATION_LEN          256
#define MAX_DIGIT_MAP_LEN           152
#define MAX_PHONEBOOK_DIAL_STR_LEN  16
#define MAX_PHONEBOOK_NAME_LEN      32
#define MAX_PHONEBOOK_RECORDS       10

typedef struct {
    int     rvLogFilter;
    int     SIPLogLevel;
    int     udpOrTerminalFlag; /*this flag indicates whether we send our logs to terminal or udp server*/
    int     serverPortForAppSyslog;
    char    serverAddrForAppSyslog[AC_DOTTED_IP_LEN];
} system_log_params_t;




typedef struct {
    conf_mode_t confMode;
    char        confMsAddr[AC_DOTTED_IP_LEN];
} voip_conference_params_t;

typedef struct {
    int unansweredCallTimeout;
    int warningToneTimeout;
    int offhookToneTimeout;
    int dialToneTimeout;
    int stutterToneDuration; /* ACL NB */
} voip_timeout_params_t;

typedef struct {
    int     bitRateHi;
    char    name[MAX_CODEC_NAME_LEN];
    codec_t codec;
} voip_conf_codec_ex_t;

typedef struct voip_phonebook_record_t{
    char                            dialString[MAX_PHONEBOOK_DIAL_STR_LEN];
    voip_destination_type_t         destinationType;
    int                             index;
    char                            userId[MAX_PHONEBOOK_DIAL_STR_LEN];
    char                            userAddress[AC_DOTTED_IP_LEN];
    int                             userPort;
    int                             localLine;
    char                            name[MAX_PHONEBOOK_NAME_LEN]; /*ACL NB - TBD*/
    struct voip_phonebook_record_t  *next;
} voip_phonebook_record_t;


typedef struct {
    int     dialTimeout;
    int     dialStrMaxSize;
    int     dialCompleteKeyEnabled;
    char    dialCompleteKey;
    char    digitMap[MAX_DIGIT_MAP_LEN];
} voip_dial_params_t;

typedef struct {
    int                         useAppConfConfiguration;
    regional_settings_params_t  regionalSettingsParams;
	unsigned char				selectedCountry[REGIONAL_SETTINGS_MAX_COUNTRY_NAME_LEN];
} voip_regional_settings_params_ex_t;

typedef struct {
    int     kaEnabled;
    int     kaTimeout;
    char    rportEnabled;
    char    prackEnabled;
    char    includePTime;
    char    blindTransferEnabled;
    char    mssClampingEnabled;
} voip_advanced_signaling_params_t;

typedef struct {
    char    sip_info_key_seq_header[MAX_SIP_INFO_INFORMATION_LEN];
    char    sip_info_key_seq_body[MAX_SIP_INFO_INFORMATION_LEN];
} voip_sip_info_key_seq_t;

typedef struct {
    char                stunEnabled;
    voip_stun_params_t  voip_stun_info;
} voip_stun_params_ex_t;

typedef struct {
    int     enable_packet_recording;
    int     channel;
    int     enable_network_recording;
    int     enable_tdm_recording;
    int     remotePort;
    char    remoteAddr[AC_DOTTED_IP_LEN];
} voip_debug_information_params_t;


typedef struct {
    acl_call_mngr_params_t                      voip_call_mngr_info;
    voip_stun_params_ex_t                       voip_stun_ex_info;
    voip_conference_params_t                    voip_conference_info;
    voip_timeout_params_t                       voip_timeouts_info;
    voip_cfw_params_t                           voip_cfw_info;
    voip_mwi_params_t                           voip_mwi_info;
	voip_dnd_params_t							 voip_dnd_info;
    voip_conf_codec_ex_t                        codecs_info[NUMBER_OF_CODECS];
    voip_phonebook_record_t                     *phonebook;
    voip_dial_params_t                          dialingInfo;
    voip_regional_settings_params_ex_t          regionalSettingsInfo;
    voip_advanced_signaling_params_t            voip_advanced_signaling_info;
    voip_sip_info_key_seq_t				voip_sip_info_key_seq_info;
    /*DSP related parameters*/
    voip_dsp_advanced_channel_configuration_t   voip_dsp_advanced_channel_configuration;
    voip_debug_information_params_t              voip_debug_information_info;
    t38_transport_mode_t                        faxTransportMode;
    data_transport_mode_t                       dataTransportMode;
    out_of_service_tone_t                       outOfServiceTone;
    /*telephone interface*/
    char                                        polarityRevasalEnabled;
    char                                        userAgentStr[MAX_USER_AGENT_LEN+1];
    int					   numberOfFxsLines;
    int					   numberOfFxoLines;
    voip_paging_params_t         paging;
} voip_config_t;

/* ACL NB build a new struct system_config_t that will be saperated from the voip */
typedef struct {
    system_log_params_t                         system_log_info;
    int                                         timeZoneOffset;
    int                                         dbgEnabled;
    char                                        wan_mac_addr[20];
    char                                        local_ip[AC_DOTTED_IP_LEN];
} system_config_t;

static voip_config_t    voip_config_params;
static system_config_t  system_config_params;

static regional_settings_params_t slic_dsp_general_and_regional_settings_params_array[]=
{
	{
	    "USA",
	    {
            {
                {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_DIAL_TONE,					 1/*EnableField*/,0/*continous */,	350/*FreqA*/,	440/*FreqB*/,	19/*FreqLevelA*/,	19/*FreqLevelB*/,	500/*ToneOn0*/, 0/*ToneOff0*/,	    0/*ToneOn1*/,	0/*ToneOff1*/,		0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_RINGBACK_TONE,				 1/*EnableField*/,1/*cadence*/,		440/*FreqA*/,	480/*FreqB*/,	19/*FreqLevelA*/,	19/*FreqLevelB*/,	200/*ToneOn0*/,400/*ToneOff0*/,     0/*ToneOn1*/,	0/*ToneOff1*/,		0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_BUSY_TONE,					 1/*EnableField*/,1/*cadence */,	480/*FreqA*/,	620/*FreqB*/,	24/*FreqLevelA*/,	24/*FreqLevelB*/,	50/*ToneOn0*/,	50/*ToneOff0*/,     0/*ToneOn1*/,	0/*ToneOff1*/,		0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_REORDER_TONE,				 1/*EnableField*/,1/*cadence */,	480/*FreqA*/,	620/*FreqB*/,	24/*FreqLevelA*/,	24/*FreqLevelB*/,	25/*ToneOn0*/,	25/*ToneOff0*/,	    0/*ToneOn1*/,	0/*ToneOff1*/,		0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_OFF_HOOK_WARNING_TONE,		 1/*EnableField*/,1/*cadence */,	480/*FreqA*/,	620/*FreqB*/,	24/*FreqLevelA*/,	24/*FreqLevelB*/,	25/*ToneOn0*/,	25/*ToneOff0*/,	    0/*ToneOn1*/,	0/*ToneOff1*/,		0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        //{DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_TONE,			 1/*EnableField*/,1/*cadence */,	350/*FreqA*/,	440/*FreqB*/,	13/*FreqLevelA*/,	13/*FreqLevelB*/,	30/*ToneOn0*/,	1000/*ToneOff0*/,	30/*ToneOn1*/,	1000/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
					/*ACL AL 090114 VI69851 - change the CALL_WAITING_TONE parameters due to customer complain (we took the updated params from the MP11x)*/
        		/*{DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_TONE,		     1/ *EnableField* /,1/ *cadence * /,	350/ *FreqA* /,	440/ *FreqB* /,	13/ *FreqLevelA* /,	13/ *FreqLevelB* /,	30/ *ToneOn0* /,	1000/ *ToneOff0* /,	30/ *ToneOn1* /,	1000/ *ToneOff1* /,	0/ *ToneOn2* /, 0/ *ToneOff2* /, 0/ *ToneOn3* /, 0/ *ToneOff3* /},*/
				{DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_TONE,			 1/*EnableField*/,1/*cadence */,	440/*FreqA*/,	0/*FreqB*/,		20/*FreqLevelA*/,	32/*FreqLevelB*/,	30/*ToneOn0*/,	500/*ToneOff0*/,	30/*ToneOn1*/,	500/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_RINGBACK_TONE, 1/*EnableField*/,1/*cadence */,	440/*FreqA*/,	480/*FreqB*/,	19/*FreqLevelA*/,	19/*FreqLevelB*/,	200/*ToneOn0*/,400/*ToneOff0*/,	    0/*ToneOn1*/,	0/*ToneOff1*/,		0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
                {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_STUTTER_TONE,               1/*EnableField*/,1/*cadence */,	350/*FreqA*/,	440/*FreqB*/,	19/*FreqLevelA*/,	19/*FreqLevelB*/,	25/*ToneOn0*/,	15/*ToneOff0*/,	    0/*ToneOn1*/,	0/*ToneOff1*/,		0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
                {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_HOWLER_TONE,				 1/*EnableField*/,1/*continous */,1400/*FreqA*/,	2600/*FreqB*/,	0/*FreqLevelA*/,	 0/*FreqLevelB*/,	10/*ToneOn0*/, 10/*ToneOff0*/,	    0/*ToneOn1*/,	0/*ToneOff1*/,		0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_TONE_2,	     1/*EnableField*/,1/*cadence */,		440/*FreqA*/,	440/*FreqB*/,	13/*FreqLevelA*/,	0/*FreqLevelB*/, 100/*ToneOn0*/,1000/*ToneOff0*/, 100/*ToneOn1*/,	1000/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/}
            },
            0/*CID - Tac49xCallerIdStandard*/
        },
	    {
            {/*coeff*/
	            0xD2, 0x00, 0xD2, 0x4C, 0x01, 0x49, 0xCA, 0x0C, 0x98, 0xB3, 0xDA, 0x2B,
	            0x4D, 0x4A, 0xB3, 0xAA, 0xA3, 0x65, 0x2C, 0x9A, 0xAF, 0xB2, 0x25, 0x2D,
	            0x01, 0x8A, 0xC3, 0xC0, 0xBC, 0xC0, 0x24, 0x24, 0xBB, 0x8F, 0xAA, 0x2D,
	            0xA2, 0x26, 0xCA, 0x8F, 0x88, 0xB8, 0x70, 0xB3, 0x36, 0x34, 0xAD, 0x2B,
	            0x2D, 0x2A, 0xA4, 0x22, 0x4D, 0x82, 0xA3, 0xA1, 0x80, 0xFA, 0xE0, 0x50,
	            0x00, 0x86, 0x23, 0x53, 0xA1, 0x22, 0x1A, 0x42, 0xAA, 0x4A, 0x25, 0xC2,
	            0x72, 0xA6, 0x5A, 0x60, 0x96, 0xBC, 0xC0, 0x00
            },
	        {/*cad*/
	            0x00, 0x08, 0x01, 0x0E, 0x00, 0x00, 0x00, 0x0A, 0x01, 0x07, 0x20, 0xC8,
	            0x01, 0x05, 0x23, 0x20, 0x40, 0x00
            }
	    },
    {
      {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
     },/*slic_calibration_resultsArray*/
		0, 0,
	},
	{
	    "Israel",
	    {
            {
                {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_DIAL_TONE,					1/*EnableField*/,0/*continous */,	400/*FreqA*/,	0/*FreqB*/,		13/*FreqLevelA*/,	0/*FreqLevelB*/,	500/*ToneOn0*/,0/*ToneOff0*/,	0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_RINGBACK_TONE,			    1/*EnableField*/,1/*cadence*/,		400/*FreqA*/,	0/*FreqB*/,		13/*FreqLevelA*/,	0/*FreqLevelB*/,	200/*ToneOn0*/,400/*ToneOff0*/,0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_BUSY_TONE,					1/*EnableField*/,1/*cadence */,		400/*FreqA*/,	0/*FreqB*/,		13/*FreqLevelA*/,	0/*FreqLevelB*/,	50/*ToneOn0*/,	50/*ToneOff0*/,0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_REORDER_TONE,				1/*EnableField*/,1/*cadence */,		400/*FreqA*/,	0/*FreqB*/,		13/*FreqLevelA*/,	0/*FreqLevelB*/,	25/*ToneOn0*/,	25/*ToneOff0*/,0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_OFF_HOOK_WARNING_TONE,		1/*EnableField*/,1/*cadence */,		400/*FreqA*/,	0/*FreqB*/,		13/*FreqLevelA*/,	0/*FreqLevelB*/,	25/*ToneOn0*/,	25/*ToneOff0*/,0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_TONE,			1/*EnableField*/,1/*cadence */,		400/*FreqA*/,	0/*FreqB*/,		13/*FreqLevelA*/,	0/*FreqLevelB*/,	30/*ToneOn0*/,	1000/*ToneOff0*/,	50/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_RINGBACK_TONE,1/*EnableField*/,1/*cadence */,		400/*FreqA*/,	0/*FreqB*/,		13/*FreqLevelA*/,	0/*FreqLevelB*/,	10/*ToneOn0*/,	10/*ToneOff0*/,	10/*ToneOn1*/,	10/*ToneOff1*/,	60/*ToneOn2*/, 300/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_STUTTER_TONE,              1/*EnableField*/,1/*cadence */,		400/*FreqA*/,	0/*FreqB*/,		13/*FreqLevelA*/,	0/*FreqLevelB*/,	25/*ToneOn0*/,	15/*ToneOff0*/,	0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
                {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_HOWLER_TONE,				1/*EnableField*/,1/*continous */,1400/*FreqA*/,	2600/*FreqB*/,	0/*FreqLevelA*/,	 0/*FreqLevelB*/,	10/*ToneOn0*/, 10/*ToneOff0*/,	    0/*ToneOn1*/,	0/*ToneOff1*/,		0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_TONE_2,		1/*EnableField*/,1/*cadence */,		440/*FreqA*/,	440/*FreqB*/,	13/*FreqLevelA*/,	0/*FreqLevelB*/, 100/*ToneOn0*/,1000/*ToneOff0*/, 100/*ToneOn1*/,	1000/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/}
            },
		    0/*CID - Tac49xCallerIdStandard*/
        },
	    {
	        {/*coeff - AC_600*/
	            0x00, 0x00, 0x3D, 0x4C, 0x01, 0x49, 0xCA, 0xEA, 0x98, 0x22, 0xEB, 0x4A,
	            0x2C, 0x34, 0x25, 0x4B, 0x24, 0xB3, 0x3D, 0x9A, 0xA2, 0xDA, 0x27, 0x9F,
	            0x01, 0x8A, 0x1D, 0x01, 0x01, 0x11, 0x01, 0x90, 0x01, 0x90, 0x01, 0x90,
	            0x01, 0x90, 0x01, 0x90, 0x88, 0xAB, 0x60, 0xA2, 0xBD, 0xAE, 0x97, 0x2A,
	            0x26, 0x43, 0xAE, 0xBA, 0x8F, 0x82, 0xFA, 0xE1, 0x80, 0x2A, 0x00, 0x50,
	            0x00, 0x86, 0x22, 0x52, 0xA1, 0x2B, 0x1B, 0xA3, 0x29, 0xFE, 0x87, 0xAB,
	            0xFA, 0x9F, 0xB9, 0xF0, 0x96, 0x2E, 0x01, 0x00
            },
	        {/*cad*/
	            0x00, 0x08, 0x01, 0x0E, 0x00, 0x00, 0x00, 0x0A, 0x01, 0x07, 0x20, 0xC8,
	            0x01, 0x05, 0x23, 0x20, 0x40, 0x00
            }
	    },
    {
      {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
     },/*slic_calibration_resultsArray*/		0, 0,
	},
	{
	    "China",
	    {
            {
                {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_DIAL_TONE,					1/*EnableField*/,0/*continous */,	450/*FreqA*/,	0/*FreqB*/,		10/*FreqLevelA*/,	0/*FreqLevelB*/,	500/*ToneOn0*/,	0/*ToneOff0*/,	0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_RINGBACK_TONE,				1/*EnableField*/,1/*cadence*/,		450/*FreqA*/,	0/*FreqB*/,		10/*FreqLevelA*/,	0/*FreqLevelB*/,	100/*ToneOn0*/,	400/*ToneOff0*/,0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_BUSY_TONE,					1/*EnableField*/,1/*cadence */,		450/*FreqA*/,	0/*FreqB*/,		10/*FreqLevelA*/,	0/*FreqLevelB*/,	35/*ToneOn0*/,	35/*ToneOff0*/, 0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_REORDER_TONE,				1/*EnableField*/,1/*cadence */,		450/*FreqA*/,	0/*FreqB*/,		10/*FreqLevelA*/,	0/*FreqLevelB*/,	70/*ToneOn0*/,	70/*ToneOff0*/,	0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_OFF_HOOK_WARNING_TONE,		1/*EnableField*/,1/*cadence */,		450/*FreqA*/,	0/*FreqB*/,		10/*FreqLevelA*/,	0/*FreqLevelB*/,	70/*ToneOn0*/,	70/*ToneOff0*/,	0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_TONE,			1/*EnableField*/,1/*cadence */,		450/*FreqA*/,	0/*FreqB*/,		20/*FreqLevelA*/,	0/*FreqLevelB*/,	40/*ToneOn0*/,	400/*ToneOff0*/,	0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_RINGBACK_TONE,1/*EnableField*/,1/*cadence */,		450/*FreqA*/,	0/*FreqB*/,		10/*FreqLevelA*/,	0/*FreqLevelB*/,	100/*ToneOn0*/,	400/*ToneOff0*/,0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_STUTTER_TONE,              1/*EnableField*/,1/*cadence */,		450/*FreqA*/,	0/*FreqB*/,		10/*FreqLevelA*/,	0/*FreqLevelB*/,	25/*ToneOn0*/,	15/*ToneOff0*/,0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
                {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_HOWLER_TONE,				1/*EnableField*/,0/*continous */,	950/*FreqA*/,	  0/*FreqB*/,	0/*FreqLevelA*/,	 0/*FreqLevelB*/,	25/*ToneOn0*/, 0/*ToneOff0*/,	    0/*ToneOn1*/,	0/*ToneOff1*/,		0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_TONE_2,		1/*EnableField*/,1/*cadence */,		450/*FreqA*/,	0/*FreqB*/,		20/*FreqLevelA*/,	0/*FreqLevelB*/,	40/*ToneOn0*/,	400/*ToneOff0*/,	0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/}
            },
   	        0/*CID - Tac49xCallerIdStandard*/
        },
	    {
	        {/*coeff*/
	            0x14, 0x00, 0x8F, 0x4C, 0x01, 0x49, 0xCA, 0xE2, 0x98, 0x4A, 0xB1, 0xCA,
	            0x22, 0xA2, 0xDB, 0x32, 0x3A, 0xA3, 0x33, 0x9A, 0xAB, 0xC3, 0x97, 0x9F,
	            0x01, 0x8A, 0x2E, 0x01, 0xBA, 0x00, 0x4A, 0xB8, 0x32, 0xBA, 0x2D, 0xB1,
	            0xAA, 0xCA, 0x42, 0x35, 0x88, 0x42, 0x50, 0x5C, 0x9F, 0x32, 0xCC, 0x2B,
	            0xBE, 0x22, 0xA4, 0xDA, 0xAD, 0x82, 0xAC, 0xA1, 0x80, 0xBA, 0x50, 0x50,
	            0x00, 0x86, 0xA2, 0x52, 0x32, 0xC5, 0x13, 0xA1, 0x22, 0x3B, 0xB4, 0xD5,
	            0x6A, 0x87, 0xA8, 0xF0, 0x96, 0x2E, 0x01, 0x00
            },
	        {/*cad*/
	            0x00, 0x08, 0x01, 0x0E, 0x00, 0x00, 0x00, 0x0A, 0x01, 0x07, 0x20, 0xC8,
	            0x01, 0x05, 0x23, 0x20, 0x40, 0x00
            }
	    },
    {
      {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
     },/*slic_calibration_resultsArray*/		0, 0,
	},
	{
        "France",
	    {
            {
                {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_DIAL_TONE,					1/*EnableField*/,0/*continous */,	440/*FreqA*/,	0/*FreqB*/,		11/*FreqLevelA*/,	0/*FreqLevelB*/,	500/*ToneOn0*/,	0/*ToneOff0*/,	0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_RINGBACK_TONE,				1/*EnableField*/,1/*cadence*/,		440/*FreqA*/,	0/*FreqB*/,		11/*FreqLevelA*/,	0/*FreqLevelB*/,	150/*ToneOn0*/,	350/*ToneOff0*/,0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_BUSY_TONE,					1/*EnableField*/,1/*cadence */,		440/*FreqA*/,	0/*FreqB*/,		11/*FreqLevelA*/,	0/*FreqLevelB*/,	50/*ToneOn0*/,	50/*ToneOff0*/, 0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_REORDER_TONE,				1/*EnableField*/,1/*cadence */,		440/*FreqA*/,	0/*FreqB*/,		11/*FreqLevelA*/,	0/*FreqLevelB*/,	50/*ToneOn0*/,	50/*ToneOff0*/,	0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_OFF_HOOK_WARNING_TONE,		1/*EnableField*/,1/*cadence */,		440/*FreqA*/,	0/*FreqB*/,		11/*FreqLevelA*/,	0/*FreqLevelB*/,	50/*ToneOn0*/,	50/*ToneOff0*/,	0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_TONE,			1/*EnableField*/,1/*cadence */,		440/*FreqA*/,	0/*FreqB*/,		11/*FreqLevelA*/,	0/*FreqLevelB*/,	30/*ToneOn0*/,	1000/*ToneOff0*/,	0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_RINGBACK_TONE,1/*EnableField*/,1/*cadence */,		440/*FreqA*/,	0/*FreqB*/,		11/*FreqLevelA*/,	0/*FreqLevelB*/,	150/*ToneOn0*/, 350/*ToneOff0*/,0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_STUTTER_TONE,              1/*EnableField*/,1/*cadence */,		440/*FreqA*/,	0/*FreqB*/,		11/*FreqLevelA*/,	0/*FreqLevelB*/,	25/*ToneOn0*/, 15/*ToneOff0*/,  0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
                {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_HOWLER_TONE,				1/*EnableField*/,0/*continous */,	950/*FreqA*/,	  0/*FreqB*/,	0/*FreqLevelA*/,	 0/*FreqLevelB*/,	25/*ToneOn0*/, 0/*ToneOff0*/,	    0/*ToneOn1*/,	0/*ToneOff1*/,		0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_TONE_2,		1/*EnableField*/,1/*cadence */,		440/*FreqA*/,	0/*FreqB*/,		11/*FreqLevelA*/,	0/*FreqLevelB*/,	30/*ToneOn0*/,	1000/*ToneOff0*/,	0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/}
            },
		    1/*CID - Tac49xCallerIdStandard*/
        },
	    {
	        {/*coeff*/
	            0x6D, 0x00, 0x36, 0x4C, 0x01, 0x49, 0xCA, 0xDE, 0x98, 0x23, 0xA0, 0xB2,
	 	        0xA1, 0x22, 0xAA, 0x46, 0x2A, 0x3B, 0x23, 0x9A, 0x4A, 0xA2, 0x37, 0x1F,
	            0x01, 0x8A, 0x2E, 0x01, 0x32, 0x00, 0xE4, 0x18, 0x22, 0x2A, 0x7B, 0x21,
	            0xA2, 0x2A, 0x33, 0xA4, 0x88, 0xB2, 0x40, 0x42, 0x3D, 0x2B, 0x3C, 0x4B,
	            0xAF, 0xC4, 0xA4, 0xA3, 0x2E, 0x82, 0x32, 0xA1, 0x80, 0x2A, 0xA0, 0x50,
	            0x00, 0x86, 0x2A, 0x43, 0x22, 0x65, 0x12, 0xA1, 0x73, 0x32, 0xA4, 0xA2,
	            0x7A, 0x87, 0xA8, 0xF0, 0x96, 0x2E, 0x01, 0x00
            },
	        {/*cad*/
	            0x00, 0x08, 0x01, 0x0E, 0x00, 0x00, 0x00, 0x0A, 0x01, 0x07, 0x20, 0xC8,
	            0x01, 0x05, 0x23, 0x20, 0x40, 0x00
            }
	    },
    {
      {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
     },/*slic_calibration_resultsArray*/		0, 0,
	},
	{
	    "Germany",
	    {
            {
                {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_DIAL_TONE,					1/*EnableField*/,0/*continous */,	425/*FreqA*/,	0/*FreqB*/,		3/*FreqLevelA*/,	0/*FreqLevelB*/,	500/*ToneOn0*/,	0/*ToneOff0*/,	0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_RINGBACK_TONE,				1/*EnableField*/,1/*cadence*/,		425/*FreqA*/,	0/*FreqB*/,		3/*FreqLevelA*/,	0/*FreqLevelB*/,	100/*ToneOn0*/,	400/*ToneOff0*/,0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_BUSY_TONE,					1/*EnableField*/,1/*cadence */,		425/*FreqA*/,	0/*FreqB*/,		3/*FreqLevelA*/,	0/*FreqLevelB*/,	48/*ToneOn0*/,	48/*ToneOff0*/, 0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_REORDER_TONE,				1/*EnableField*/,1/*cadence */,		425/*FreqA*/,	0/*FreqB*/,		3/*FreqLevelA*/,	0/*FreqLevelB*/,	24/*ToneOn0*/,	24/*ToneOff0*/,	0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_OFF_HOOK_WARNING_TONE,		1/*EnableField*/,1/*cadence */,		425/*FreqA*/,	0/*FreqB*/,		3/*FreqLevelA*/,	0/*FreqLevelB*/,	24/*ToneOn0*/,	24/*ToneOff0*/,	0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_TONE,			1/*EnableField*/,1/*cadence */,		425/*FreqA*/,	0/*FreqB*/,		3/*FreqLevelA*/,	0/*FreqLevelB*/,	20/*ToneOn0*/,	20/*ToneOff0*/,	20/*ToneOn1*/,	500/*ToneOff1*/,0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_RINGBACK_TONE,1/*EnableField*/,1/*cadence */,		425/*FreqA*/,	0/*FreqB*/,		3/*FreqLevelA*/,	0/*FreqLevelB*/,	100/*ToneOn0*/,	400/*ToneOff0*/,0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_STUTTER_TONE,              1/*EnableField*/,1/*cadence */,		425/*FreqA*/,	0/*FreqB*/,		3/*FreqLevelA*/,	0/*FreqLevelB*/,	25/*ToneOn0*/,	15/*ToneOff0*/, 0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
                {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_HOWLER_TONE,				1/*EnableField*/,0/*continous */,	950/*FreqA*/,	  0/*FreqB*/,	0/*FreqLevelA*/,	 0/*FreqLevelB*/,	25/*ToneOn0*/, 0/*ToneOff0*/,	    0/*ToneOn1*/,	0/*ToneOff1*/,		0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_TONE_2,		1/*EnableField*/,1/*cadence */,		425/*FreqA*/,	0/*FreqB*/,		3/*FreqLevelA*/,	0/*FreqLevelB*/,	20/*ToneOn0*/,	20/*ToneOff0*/,	20/*ToneOn1*/,	500/*ToneOff1*/,0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/}
            },
		    1/*CID - Tac49xCallerIdStandard*/
        },
	    {
	        {/*coeff*/
	            0x14, 0x00, 0x95, 0x4C, 0x01, 0x49, 0xCA, 0xE3, 0x98, 0x2A, 0x31, 0x33,
	            0x22, 0xAC, 0x3B, 0xCC, 0x2A, 0x3C, 0x23, 0x9A, 0xA3, 0x62, 0x97, 0x9F,
	            0x01, 0x8A, 0x2E, 0x01, 0x27, 0x00, 0x3A, 0x28, 0x9F, 0x4A, 0xAA, 0x41,
	            0x24, 0x3A, 0x22, 0xA4, 0x88, 0x2B, 0x40, 0xB2, 0xAD, 0x2B, 0xBC, 0xFC,
	            0xBF, 0xBB, 0x25, 0x6E, 0xAD, 0x82, 0xA2, 0xA1, 0x80, 0xAA, 0xB0, 0x50,
	            0x00, 0x86, 0x33, 0x5A, 0x22, 0x24, 0x1A, 0xA1, 0xAB, 0x22, 0xB3, 0xAA,
	            0x4D, 0xA5, 0x2A, 0x60, 0x96, 0x33, 0xA1, 0x00
            },
	        {/*cad*/
	            0x00, 0x08, 0x01, 0x0E, 0x00, 0x00, 0x00, 0x0A, 0x01, 0x07, 0x20, 0xC8,
	            0x01, 0x05, 0x23, 0x20, 0x40, 0x00
            }
	    },
    {
      {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
     },/*slic_calibration_resultsArray*/		0, 0,
    },
	{
	    "Netherlands",
	    {
            {
                {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_DIAL_TONE,					1/*EnableField*/,0/*continous */,	425/*FreqA*/,	0/*FreqB*/,		5/*FreqLevelA*/,	0/*FreqLevelB*/,	500/*ToneOn0*/,	0/*ToneOff0*/,	0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_RINGBACK_TONE,				1/*EnableField*/,1/*cadence*/,		425/*FreqA*/,	0/*FreqB*/,		5/*FreqLevelA*/,	0/*FreqLevelB*/,	100/*ToneOn0*/,	400/*ToneOff0*/,0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_BUSY_TONE,					1/*EnableField*/,1/*cadence */,		425/*FreqA*/,	0/*FreqB*/,		5/*FreqLevelA*/,	0/*FreqLevelB*/,	50/*ToneOn0*/,	50/*ToneOff0*/, 0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_REORDER_TONE,				1/*EnableField*/,1/*cadence */,		425/*FreqA*/,	0/*FreqB*/,		5/*FreqLevelA*/,	0/*FreqLevelB*/,	25/*ToneOn0*/,	25/*ToneOff0*/,	0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_OFF_HOOK_WARNING_TONE,		1/*EnableField*/,1/*cadence */,		425/*FreqA*/,	0/*FreqB*/,		5/*FreqLevelA*/,	0/*FreqLevelB*/,	25/*ToneOn0*/,	25/*ToneOff0*/,	0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_TONE,			1/*EnableField*/,1/*cadence */,		425/*FreqA*/,	0/*FreqB*/,		9/*FreqLevelA*/,	0/*FreqLevelB*/,	20/*ToneOn0*/,	20/*ToneOff0*/,	20/*ToneOn1*/,	500/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_RINGBACK_TONE,1/*EnableField*/,1/*cadence */,		425/*FreqA*/,	0/*FreqB*/,		5/*FreqLevelA*/,	0/*FreqLevelB*/,	50/*ToneOn0*/,	950/*ToneOff0*/,0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_STUTTER_TONE,              1/*EnableField*/,1/*cadence */,		425/*FreqA*/,	0/*FreqB*/,		5/*FreqLevelA*/,	0/*FreqLevelB*/,	25/*ToneOn0*/,	15/*ToneOff0*/,0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
                {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_HOWLER_TONE,				1/*EnableField*/,0/*continous */,	950/*FreqA*/,	  0/*FreqB*/,	0/*FreqLevelA*/,	 0/*FreqLevelB*/,	25/*ToneOn0*/, 0/*ToneOff0*/,	    0/*ToneOn1*/,	0/*ToneOff1*/,		0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_TONE_2,		1/*EnableField*/,1/*cadence */,		425/*FreqA*/,	0/*FreqB*/,		9/*FreqLevelA*/,	0/*FreqLevelB*/,	20/*ToneOn0*/,	20/*ToneOff0*/,	20/*ToneOn1*/,	500/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/}
            },
		    1/*CID - Tac49xCallerIdStandard*/
        },
	    {
	        {/*coeff*/
	            0x00, 0x00, 0x3D, 0x4C, 0x01, 0x49, 0xCA, 0xEA, 0x98, 0x22, 0xEB, 0x4A,
	            0x2C, 0x34, 0x25, 0x4B, 0x24, 0xB3, 0x3D, 0x9A, 0xA2, 0xDA, 0x27, 0x9F,
	            0x01, 0x8A, 0x1D, 0x01, 0x01, 0x11, 0x01, 0x90, 0x01, 0x90, 0x01, 0x90,
	            0x01, 0x90, 0x01, 0x90, 0x88, 0xAB, 0x60, 0xA2, 0xBD, 0xAE, 0x97, 0x2A,
	            0x26, 0x43, 0xAE, 0xBA, 0x8F, 0x82, 0xFA, 0xE1, 0x80, 0x2A, 0x00, 0x50,
	            0x00, 0x86, 0x72, 0x53, 0x22, 0x2C, 0x12, 0x33, 0x2E, 0x7A, 0x87, 0x3A,
	            0xF2, 0xAF, 0xA9, 0xF0, 0x96, 0x2E, 0x01, 0x00
            },
	        {/*cad*/
	            0x00, 0x08, 0x01, 0x0E, 0x00, 0x00, 0x00, 0x0A, 0x01, 0x07, 0x20, 0xC8,
	            0x01, 0x05, 0x23, 0x20, 0x40, 0x00
            }
	    },
    {
      {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
     },/*slic_calibration_resultsArray*/		0, 0,
	},
	{
	    "UK",
	    {
            {
                {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_DIAL_TONE,					1/*EnableField*/,0/*continous */,	350/*FreqA*/,	440/*FreqB*/,	10/*FreqLevelA*/,	10/*FreqLevelB*/,	500/*ToneOn0*/,	0/*ToneOff0*/,	0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_RINGBACK_TONE,				1/*EnableField*/,1/*cadence*/,		400/*FreqA*/,	450/*FreqB*/,	16/*FreqLevelA*/,	16/*FreqLevelB*/,	40/*ToneOn0*/,	20/*ToneOff0*/, 40/*ToneOn1*/,	200/*ToneOff1*/,0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_BUSY_TONE,					1/*EnableField*/,1/*cadence */,		400/*FreqA*/,	0/*FreqB*/,		13/*FreqLevelA*/,	0/*FreqLevelB*/,	37/*ToneOn0*/,	37/*ToneOff0*/, 0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_REORDER_TONE,				1/*EnableField*/,1/*cadence */,		400/*FreqA*/,	0/*FreqB*/,		19/*FreqLevelA*/,	0/*FreqLevelB*/,	40/*ToneOn0*/,	35/*ToneOff0*/,	22/*ToneOn1*/,	52/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_OFF_HOOK_WARNING_TONE,		1/*EnableField*/,1/*cadence */,		400/*FreqA*/,	0/*FreqB*/,		19/*FreqLevelA*/,	0/*FreqLevelB*/,	40/*ToneOn0*/,	35/*ToneOff0*/,	22/*ToneOn1*/,	52/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_TONE,			1/*EnableField*/,1/*cadence */,		400/*FreqA*/,	0/*FreqB*/,		13/*FreqLevelA*/,	0/*FreqLevelB*/,	40/*ToneOn0*/,1000/*ToneOff0*/,	0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_RINGBACK_TONE,1/*EnableField*/,1/*cadence */,		400/*FreqA*/,	450/*FreqB*/,	16/*FreqLevelA*/,	16/*FreqLevelB*/,	40/*ToneOn0*/,	20/*ToneOff0*/,40/*ToneOn1*/,	200/*ToneOff1*/,0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_STUTTER_TONE,              1/*EnableField*/,1/*cadence */,		350/*FreqA*/,	440/*FreqB*/,	10/*FreqLevelA*/,	10/*FreqLevelB*/,	25/*ToneOn0*/,	15/*ToneOff0*/,0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
                {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_HOWLER_TONE,				1/*EnableField*/,0/*continous */,	950/*FreqA*/,	  0/*FreqB*/,	0/*FreqLevelA*/,	 0/*FreqLevelB*/,	25/*ToneOn0*/, 0/*ToneOff0*/,	    0/*ToneOn1*/,	0/*ToneOff1*/,		0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_TONE_2,		1/*EnableField*/,1/*cadence */,		400/*FreqA*/,	0/*FreqB*/,		13/*FreqLevelA*/,	0/*FreqLevelB*/,	40/*ToneOn0*/,1000/*ToneOff0*/,	0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/}
            },
		    1/*CID - Tac49xCallerIdStandard*/
        },
	    {
	        {/*coeff*/
	            0x4F, 0x00, 0xC3, 0x4C, 0x01, 0x49, 0xCA, 0xD6, 0x98, 0x64, 0x41, 0x5A,
	            0xA1, 0x32, 0x35, 0x23, 0xAA, 0x2B, 0xA4, 0x9A, 0x3A, 0xA2, 0xC6, 0x2E,
	            0x01, 0x8A, 0x2D, 0x01, 0xED, 0x00, 0xAC, 0xC8, 0xE4, 0x29, 0xCE, 0x32,
	            0x34, 0x4C, 0xDA, 0xAD, 0x88, 0x4C, 0x30, 0xBC, 0x2C, 0x43, 0x3C, 0x4A,
	            0x9F, 0xA2, 0xA5, 0xB3, 0xAE, 0x82, 0xAD, 0xA1, 0x80, 0xA2, 0x50, 0x50,
	            0x00, 0x86, 0xB3, 0x5A, 0x32, 0xAB, 0x12, 0xA1, 0xEB, 0x2A, 0x23, 0xAA,
	            0x3A, 0x24, 0x4B, 0x40, 0x96, 0x54, 0xA0, 0x00
            },
	        {/*cad*/
	            0x00, 0x08, 0x01, 0x0E, 0x00, 0x00, 0x00, 0x0A, 0x01, 0x07, 0x20, 0xC8,
	            0x01, 0x05, 0x23, 0x20, 0x40, 0x00
            }
	    },
    {
      {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
     },/*slic_calibration_resultsArray*/		0, 0,
	},
	{
	    "Brazil",
	    {
            {
                {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_DIAL_TONE,					1/*EnableField*/,0/*continous */,	425/*FreqA*/,	0/*FreqB*/,	    15/*FreqLevelA*/,	0/*FreqLevelB*/,	500/*ToneOn0*/,	0/*ToneOff0*/,	0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_RINGBACK_TONE,				1/*EnableField*/,1/*cadence*/,		425/*FreqA*/,	0/*FreqB*/,	    15/*FreqLevelA*/,	0/*FreqLevelB*/,	100/*ToneOn0*/,	400/*ToneOff0*/, 0/*ToneOn1*/,	0/*ToneOff1*/,  0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_BUSY_TONE,					1/*EnableField*/,1/*cadence */,		425/*FreqA*/,	0/*FreqB*/,		10/*FreqLevelA*/,	0/*FreqLevelB*/,	25/*ToneOn0*/,	25/*ToneOff0*/, 0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_REORDER_TONE,				1/*EnableField*/,1/*cadence */,		425/*FreqA*/,	0/*FreqB*/,		10/*FreqLevelA*/,	0/*FreqLevelB*/,	25/*ToneOn0*/,	25/*ToneOff0*/,	0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_OFF_HOOK_WARNING_TONE,		1/*EnableField*/,1/*cadence */,		425/*FreqA*/,	0/*FreqB*/,		14/*FreqLevelA*/,	0/*FreqLevelB*/,	25/*ToneOn0*/,	25/*ToneOff0*/,	0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_TONE,			1/*EnableField*/,2/*burst */,		425/*FreqA*/,	0/*FreqB*/,		10/*FreqLevelA*/,	0/*FreqLevelB*/,	100/*ToneOn0*/,  1000/*ToneOff0*/,0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_RINGBACK_TONE,1/*EnableField*/,1/*cadence*/,		425/*FreqA*/,	0/*FreqB*/,	    15/*FreqLevelA*/,	0/*FreqLevelB*/,	100/*ToneOn0*/,	400/*ToneOff0*/, 0/*ToneOn1*/,	0/*ToneOff1*/,  0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_STUTTER_TONE,              1/*EnableField*/,1/*cadence */,		425/*FreqA*/,	0/*FreqB*/,	    15/*FreqLevelA*/,	0/*FreqLevelB*/,	25/*ToneOn0*/,	15/*ToneOff0*/,0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
                {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_HOWLER_TONE,				1/*EnableField*/,0/*continous */,	950/*FreqA*/,	  0/*FreqB*/,	0/*FreqLevelA*/,	 0/*FreqLevelB*/,	25/*ToneOn0*/, 0/*ToneOff0*/,	    0/*ToneOn1*/,	0/*ToneOff1*/,		0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_TONE_2,		1/*EnableField*/,2/*burst */,		425/*FreqA*/,	0/*FreqB*/,		10/*FreqLevelA*/,	0/*FreqLevelB*/,	100/*ToneOn0*/,  1000/*ToneOff0*/,0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/}
            },
		    11/*CID - Tac49xCallerIdStandard - CALLER_ID_STANDARD__DTMF_CLIP_BRAZILIAN*/
        },
	    {
	        {/*coeff*/
                0x1D, 0x00, 0xCA, 0x4C, 0x01, 0x49, 0xCA, 0x07, 0x98, 0xA3, 0x2B, 0x2B,
                0x3C, 0xB2, 0xB4, 0x22, 0x24, 0xA2, 0xAC, 0x9A, 0xA4, 0xBA, 0xE7, 0x9F,
                0x01, 0x8A, 0xDC, 0x01, 0x3A, 0xB0, 0xAB, 0xB3, 0xA2, 0xAD, 0x23, 0x26,
                0x2A, 0xAE, 0xAB, 0xA6, 0x88, 0xB3, 0x60, 0x32, 0x2D, 0x32, 0xA5, 0xAB,
                0xA5, 0xB2, 0xED, 0x42, 0x37, 0x82, 0x2D, 0xA1, 0x80, 0xBC, 0x40, 0x50,
                0x00, 0x86, 0xA2, 0x52, 0xA1, 0x32, 0x1B, 0x42, 0xDA, 0x6D, 0xA7, 0x49,
                0xFA, 0x2F, 0xAA, 0xF0, 0x96, 0x2E, 0x01, 0x00
            },
	        {/*cad*/
	            0x00, 0x08, 0x01, 0x0E, 0x00, 0x00, 0x00, 0x0A, 0x01, 0x07, 0x20, 0xC8,
	            0x01, 0x05, 0x23, 0x20, 0x40, 0x00
            }
	    },
    {
      {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
     },/*slic_calibration_resultsArray*/		0, 0,
	},
	{
	    "Italy",
	    {
            {
                {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_DIAL_TONE,					1/*EnableField*/,1/*cadence*/,	    425/*FreqA*/,	0/*FreqB*/,		14/*FreqLevelA*/,	0/*FreqLevelB*/,	60/*ToneOn0*/,	100/*ToneOff0*/,	20/*ToneOn1*/,	20/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_RINGBACK_TONE,				1/*EnableField*/,1/*cadence*/,		425/*FreqA*/,	0/*FreqB*/,		25/*FreqLevelA*/,	0/*FreqLevelB*/,	100/*ToneOn0*/,	400/*ToneOff0*/,    0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_BUSY_TONE,					1/*EnableField*/,1/*cadence */,		425/*FreqA*/,	0/*FreqB*/,		25/*FreqLevelA*/,	0/*FreqLevelB*/,	20/*ToneOn0*/,	20/*ToneOff0*/,     0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_REORDER_TONE,				1/*EnableField*/,1/*cadence */,		425/*FreqA*/,	0/*FreqB*/,		20/*FreqLevelA*/,	0/*FreqLevelB*/,	20/*ToneOn0*/,	20/*ToneOff0*/,	0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_OFF_HOOK_WARNING_TONE,		1/*EnableField*/,1/*cadence */,		425/*FreqA*/,	0/*FreqB*/,		20/*FreqLevelA*/,	0/*FreqLevelB*/,	20/*ToneOn0*/,	20/*ToneOff0*/,	0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_TONE,			1/*EnableField*/,1/*cadence */,		425/*FreqA*/,	0/*FreqB*/,		13/*FreqLevelA*/,	0/*FreqLevelB*/,	40/*ToneOn0*/,  10/*ToneOff0*/, 25/*ToneOn1*/,	10/*ToneOff1*/,	15/*ToneOn2*/,1000/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_RINGBACK_TONE,1/*EnableField*/,1/*cadence */,		425/*FreqA*/,	0/*FreqB*/,		20/*FreqLevelA*/,	0/*FreqLevelB*/,	100/*ToneOn0*/,	400/*ToneOff0*/,    0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_STUTTER_TONE,              1/*EnableField*/,1/*cadence */,		425/*FreqA*/,	0/*FreqB*/,		20/*FreqLevelA*/,	0/*FreqLevelB*/,	25/*ToneOn0*/,	15/*ToneOff0*/,     0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
                {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_HOWLER_TONE,				1/*EnableField*/,0/*continous */,	950/*FreqA*/,	  0/*FreqB*/,	0/*FreqLevelA*/,	 0/*FreqLevelB*/,	25/*ToneOn0*/, 0/*ToneOff0*/,	    0/*ToneOn1*/,	0/*ToneOff1*/,		0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_TONE_2,		1/*EnableField*/,1/*cadence */,		425/*FreqA*/,	0/*FreqB*/,		13/*FreqLevelA*/,	0/*FreqLevelB*/,	40/*ToneOn0*/,  10/*ToneOff0*/, 25/*ToneOn1*/,	10/*ToneOff1*/,	15/*ToneOn2*/,1000/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/}
            },
		    1/*CID - Tac49xCallerIdStandard*/
        },
	    {
	        {/* coeff NIRP 070917 using ETSI coeffs following a recomendatiom we got from Legerity */

                0x60, 0x00, 0x01, 0x4C, 0x01, 0x49, 0xCA, 0xDD, 0x98, 0x3E, 0x41, 0xB2,
                0x22, 0x32, 0x2C, 0x4A, 0x7A, 0x32, 0xB3, 0x9A, 0xAA, 0xB2, 0xA7, 0x9F,
                0x01, 0x8A, 0x2E, 0x01, 0x2C, 0x00, 0x2B, 0xC8, 0xBB, 0xA9, 0xAD, 0xA1,
                0xAB, 0x2B, 0x8F, 0x26, 0x88, 0xA2, 0x40, 0xA6, 0xAC, 0x2C, 0xBC, 0xBD,
                0x8F, 0xBB, 0xB5, 0x27, 0x3E, 0x82, 0x35, 0xA1, 0x80, 0x3A, 0xF0, 0x50,
                0x00, 0x86, 0x23, 0x53, 0x32, 0x97, 0x14, 0xA1, 0x2B, 0x2A, 0xB3, 0xFE,
                0x43, 0x45, 0x23, 0x60, 0x96, 0xD9, 0x71, 0x00
            },
	        {/*cad*/
	            0x00, 0x08, 0x01, 0x0E, 0x00, 0x00, 0x00, 0x0A, 0x01, 0x07, 0x20, 0xC8,
	            0x01, 0x05, 0x23, 0x20, 0x40, 0x00
            }
	    },
    {
      {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
     },/*slic_calibration_resultsArray*/		0, 0,
	},
///
	{
	    "Argentina",
	    {
            {
                {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_DIAL_TONE,					1/*EnableField*/,0/*cadence*/,	   425/*FreqA*/,	0/*FreqB*/,		14/*FreqLevelA*/,	0/*FreqLevelB*/,	50/*ToneOn0*/,	  0/*ToneOff0*/,	0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/,   0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_RINGBACK_TONE,			    1/*EnableField*/,1/*cadence*/,		425/*FreqA*/,	0/*FreqB*/,		25/*FreqLevelA*/,	0/*FreqLevelB*/,	100/*ToneOn0*/,400/*ToneOff0*/,  0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/,   0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_BUSY_TONE,					1/*EnableField*/,1/*cadence */,		450/*FreqA*/,	0/*FreqB*/,		25/*FreqLevelA*/, 0/*FreqLevelB*/,	30/*ToneOn0*/,	 20/*ToneOff0*/,  0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/,   0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_REORDER_TONE,				1/*EnableField*/,1/*cadence */,		425/*FreqA*/,	0/*FreqB*/,		20/*FreqLevelA*/,	0/*FreqLevelB*/,	30/*ToneOn0*/,	 40/*ToneOff0*/,	0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/,   0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_OFF_HOOK_WARNING_TONE,		1/*EnableField*/,1/*cadence */,		425/*FreqA*/,	0/*FreqB*/,		20/*FreqLevelA*/,	0/*FreqLevelB*/,	30/*ToneOn0*/,	 40/*ToneOff0*/,	0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/,   0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_TONE,			1/*EnableField*/,1/*cadence */,		425/*FreqA*/,	0/*FreqB*/,		20/*FreqLevelA*/,	0/*FreqLevelB*/,	30/*ToneOn0*/,3000/*ToneOff0*/,  0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/,   0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_RINGBACK_TONE,1/*EnableField*/,1/*cadence */,		425/*FreqA*/,	0/*FreqB*/,		13/*FreqLevelA*/,	0/*FreqLevelB*/,	40/*ToneOn0*/,	 20/*ToneOff0*/, 40/*ToneOn1*/,400/*ToneOff1*/,	0/*ToneOn2*/,   0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_STUTTER_TONE,              1/*EnableField*/,1/*cadence */,		425/*FreqA*/,	0/*FreqB*/,		20/*FreqLevelA*/,	0/*FreqLevelB*/,	25/*ToneOn0*/,	 15/*ToneOff0*/,  0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/,   0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
                {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_HOWLER_TONE,				1/*EnableField*/,0/*continous */,	950/*FreqA*/,	0/*FreqB*/,	    0/*FreqLevelA*/,	0/*FreqLevelB*/,	25/*ToneOn0*/,   0/*ToneOff0*/,	0/*ToneOn1*/,	0/*ToneOff1*/,	0/*ToneOn2*/,   0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_TONE_2,		 1/*EnableField*/,1/*cadence */,		425/*FreqA*/,	0/*FreqB*/,		13/*FreqLevelA*/,	0/*FreqLevelB*/,	40/*ToneOn0*/,  10/*ToneOff0*/, 25/*ToneOn1*/, 10/*ToneOff1*/,15/*ToneOn2*/,1000/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/}
            },
		    1/*CID - Tac49xCallerIdStandard*/
        },
	    {
	        {/* coeff  */

                0x00, 0x00, 0x3D, 0x4C, 0x01, 0x49, 0xCA, 0xEA, 0x98, 0x22, 0xEB, 0x4A,
                0x2C, 0x34, 0x25, 0x4B, 0x24, 0xB3, 0x3D, 0x9A, 0xA2, 0xDA, 0x27, 0x9F,
                0x01, 0x8A, 0x1D, 0x01, 0x01, 0x11, 0x01, 0x90, 0x01, 0x90, 0x01, 0x90,
                0x01, 0x90, 0x01, 0x90, 0x88, 0xAB, 0x60, 0xA2, 0xBD, 0xAE, 0x97, 0x2A,
                0x26, 0x43, 0xAE, 0xBA, 0x8F, 0x82, 0xFA, 0xE1, 0x80, 0x2A, 0x00, 0x50,
                0x00, 0x86, 0x22, 0x52, 0xA1, 0x2B, 0x1B, 0xA3, 0x29, 0xFE, 0x87, 0xAB,
                0xFA, 0x9F, 0xB9, 0xF0, 0x96, 0x2E, 0x01, 0x00
            },
	        {/*cad*/
	            0x00, 0x08, 0x01, 0x0E, 0x00, 0x00, 0x00, 0x0A, 0x01, 0x07, 0x20, 0xC8,
	            0x01, 0x05, 0x23, 0x20, 0x40, 0x00
            }
	    },
    {
      {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
     },/*slic_calibration_resultsArray*/		0, 0,
	},

////
    {
	    "USER_DEFINED",
	    {
            {
                {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_DIAL_TONE,					1/*EnableField*/,0/*continous */,	350/*FreqA*/,	440/*FreqB*/,	19/*FreqLevelA*/,	19/*FreqLevelB*/,	500/*ToneOn0*/,	0/*ToneOff0*/, 0/*ToneOn1*/,	0/*ToneOff1*/,		0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_RINGBACK_TONE,				1/*EnableField*/,1/*cadence*/,		440/*FreqA*/,	480/*FreqB*/,	19/*FreqLevelA*/,	19/*FreqLevelB*/,	0/*ToneOn0*/,	0/*ToneOff0*/, 200/*ToneOn1*/,	400/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_BUSY_TONE,					1/*EnableField*/,1/*cadence */,		480/*FreqA*/,	620/*FreqB*/,	26/*FreqLevelA*/,	26/*FreqLevelB*/,	0/*ToneOn0*/,	0/*ToneOff0*/, 50/*ToneOn1*/,	50/*ToneOff1*/,		0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_REORDER_TONE,				0/*EnableField*/,1/*cadence */,		0/*FreqA*/,		0/*FreqB*/,		0/*FreqLevelA*/,	0/*FreqLevelB*/,	0/*ToneOn0*/,	0/*ToneOff0*/, 0/*ToneOn1*/,	0/*ToneOff1*/,		0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_OFF_HOOK_WARNING_TONE,		0/*EnableField*/,1/*cadence */,		0/*FreqA*/,		0/*FreqB*/,		0/*FreqLevelA*/,	0/*FreqLevelB*/,	0/*ToneOn0*/,	0/*ToneOff0*/, 0/*ToneOn1*/,	0/*ToneOff1*/,		0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_TONE,			0/*EnableField*/,1/*cadence */,		0/*FreqA*/,		0/*FreqB*/,		0/*FreqLevelA*/,	0/*FreqLevelB*/,	0/*ToneOn0*/,	0/*ToneOff0*/, 0/*ToneOn1*/,	0/*ToneOff1*/,		0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_RINGBACK_TONE,0/*EnableField*/,1/*cadence */,		0/*FreqA*/,		0/*FreqB*/,		0/*FreqLevelA*/,	0/*FreqLevelB*/,	0/*ToneOn0*/,	0/*ToneOff0*/, 0/*ToneOn1*/,	0/*ToneOff1*/,		0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_STUTTER_TONE,              1/*EnableField*/,1/*cadence */,		350/*FreqA*/,	440/*FreqB*/,	19/*FreqLevelA*/,	19/*FreqLevelB*/,	25/*ToneOn0*/,	15/*ToneOff0*/, 0/*ToneOn1*/,	0/*ToneOff1*/,		0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
                {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_HOWLER_TONE,				1/*EnableField*/,0/*continous */,	950/*FreqA*/,	  0/*FreqB*/,	0/*FreqLevelA*/,	 0/*FreqLevelB*/,	25/*ToneOn0*/, 0/*ToneOff0*/,	    0/*ToneOn1*/,	0/*ToneOff1*/,		0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
		        {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_TONE_2,		0/*EnableField*/,1/*cadence */,		0/*FreqA*/,		0/*FreqB*/,		0/*FreqLevelA*/,	0/*FreqLevelB*/,	0/*ToneOn0*/,	0/*ToneOff0*/, 0/*ToneOn1*/,	0/*ToneOff1*/,		0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/}
            },
		    0/*CID - Tac49xCallerIdStandard*/
        },
	    {
	        {/*coeff*/
	            0x00, 0x00, 0x00, 0x4C, 0x00, 0x49, 0xCA, 0xEA, 0x98, 0x36, 0x4B, 0xBB,
	            0x4C, 0x4B, 0xA4, 0xB3, 0x24, 0xB2, 0xAC, 0x9A, 0x23, 0xCA, 0x27, 0x9F,
	            0x01, 0x8A, 0x1D, 0x01, 0xCA, 0xB0, 0xB2, 0x33, 0x2B, 0x2C, 0xA9, 0x74,
	            0xD3, 0xAC, 0x33, 0xA5, 0x88, 0xAC, 0x60, 0xB2, 0xBD, 0xA2, 0x97, 0x45,
	            0x36, 0x22, 0xAE, 0xAA, 0x8F, 0x82, 0x42, 0x21, 0x80, 0x22, 0x10, 0x50,
	            0x00, 0x86, 0x33, 0x5F, 0x22, 0xBB, 0x1D, 0xB3, 0xA8, 0xFC, 0x87, 0xDC,
	            0xF2, 0xAF, 0x2A, 0xF0, 0x96, 0x2E, 0x01, 0x00
            },
	        {/*cad*/
	            0x00, 0x08, 0x01, 0x0E, 0x00, 0x00, 0x00, 0x0A, 0x01, 0x07, 0x20, 0xC8,
	            0x01, 0x05, 0x23, 0x20, 0x40, 0x00
            }
	    },
    {
      {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
     },/*slic_calibration_resultsArray*/		0, 0,
	},


    {"END_OF_LIST",
	    { { {-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
               	  {-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
               	  {-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
               	  {-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
               	  {-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
               	  {-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
               	  {-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
               	  {-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
               	  {-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                  {-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, }, 0 }, {{0}, {0}},        {
      {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
     },/*slic_calibration_resultsArray*/
		0, 0,
}
};

static regional_settings_params_t defaultRegionalSettingsParams =
{
    "USA",
    {
        {
            {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_DIAL_TONE,					 1/*EnableField*/,0/*continous */,	350/*FreqA*/,	440/*FreqB*/,	19/*FreqLevelA*/,	19/*FreqLevelB*/,	500/*ToneOn0*/, 0/*ToneOff0*/,	    0/*ToneOn1*/,	0/*ToneOff1*/,		0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
            {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_RINGBACK_TONE,				 1/*EnableField*/,1/*cadence*/,		440/*FreqA*/,	480/*FreqB*/,	19/*FreqLevelA*/,	19/*FreqLevelB*/,	200/*ToneOn0*/,400/*ToneOff0*/,     0/*ToneOn1*/,	0/*ToneOff1*/,		0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
            {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_BUSY_TONE,					 1/*EnableField*/,1/*cadence */,	480/*FreqA*/,	620/*FreqB*/,	24/*FreqLevelA*/,	24/*FreqLevelB*/,	50/*ToneOn0*/,	50/*ToneOff0*/,     0/*ToneOn1*/,	0/*ToneOff1*/,		0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
            {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_REORDER_TONE,				 1/*EnableField*/,1/*cadence */,	480/*FreqA*/,	620/*FreqB*/,	24/*FreqLevelA*/,	24/*FreqLevelB*/,	25/*ToneOn0*/,	25/*ToneOff0*/,	    0/*ToneOn1*/,	0/*ToneOff1*/,		0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
            {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_OFF_HOOK_WARNING_TONE,		 1/*EnableField*/,1/*cadence */,	480/*FreqA*/,	620/*FreqB*/,	24/*FreqLevelA*/,	24/*FreqLevelB*/,	25/*ToneOn0*/,	25/*ToneOff0*/,	    0/*ToneOn1*/,	0/*ToneOff1*/,		0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
            {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_TONE,			 1/*EnableField*/,1/*cadence */,	350/*FreqA*/,	440/*FreqB*/,	13/*FreqLevelA*/,	13/*FreqLevelB*/,	30/*ToneOn0*/,	1000/*ToneOff0*/,	30/*ToneOn1*/,	1000/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
            {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_RINGBACK_TONE, 1/*EnableField*/,1/*cadence */,	440/*FreqA*/,	480/*FreqB*/,	19/*FreqLevelA*/,	19/*FreqLevelB*/,	200/*ToneOn0*/,400/*ToneOff0*/,	    0/*ToneOn1*/,	0/*ToneOff1*/,		0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
            {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_STUTTER_TONE,               1/*EnableField*/,1/*cadence */,	350/*FreqA*/,	440/*FreqB*/,	19/*FreqLevelA*/,	19/*FreqLevelB*/,	25/*ToneOn0*/,	15/*ToneOff0*/,	    0/*ToneOn1*/,	0/*ToneOff1*/,		0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
            {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_HOWLER_TONE,				 1/*EnableField*/,1/*continous */,1400/*FreqA*/,	2600/*FreqB*/,	0/*FreqLevelA*/,	 0/*FreqLevelB*/,	10/*ToneOn0*/, 10/*ToneOff0*/,	    0/*ToneOn1*/,	0/*ToneOff1*/,		0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/},
            {DSP_REGIONAL_SETTINGS_CALL_PROGRESS_CALL_WAITING_TONE_2,		 1/*EnableField*/,1/*cadence */,	350/*FreqA*/,	440/*FreqB*/,	13/*FreqLevelA*/,	13/*FreqLevelB*/,	30/*ToneOn0*/,	1000/*ToneOff0*/,	30/*ToneOn1*/,	1000/*ToneOff1*/,	0/*ToneOn2*/, 0/*ToneOff2*/, 0/*ToneOn3*/, 0/*ToneOff3*/}
        },
        0/*CID - Tac49xCallerIdStandard*/
    },
    {
        {/*coeff*/
            0xD2, 0x00, 0xD2, 0x4C, 0x01, 0x49, 0xCA, 0x0C, 0x98, 0xB3, 0xDA, 0x2B,
            0x4D, 0x4A, 0xB3, 0xAA, 0xA3, 0x65, 0x2C, 0x9A, 0xAF, 0xB2, 0x25, 0x2D,
            0x01, 0x8A, 0xC3, 0xC0, 0xBC, 0xC0, 0x24, 0x24, 0xBB, 0x8F, 0xAA, 0x2D,
            0xA2, 0x26, 0xCA, 0x8F, 0x88, 0xB8, 0x70, 0xB3, 0x36, 0x34, 0xAD, 0x2B,
            0x2D, 0x2A, 0xA4, 0x22, 0x4D, 0x82, 0xA3, 0xA1, 0x80, 0xFA, 0xE0, 0x50,
            0x00, 0x86, 0x23, 0x53, 0xA1, 0x22, 0x1A, 0x42, 0xAA, 0x4A, 0x25, 0xC2,
            0x72, 0xA6, 0x5A, 0x60, 0x96, 0xBC, 0xC0, 0x00
        },
        {/*cad*/
            0x00, 0x08, 0x01, 0x0E, 0x00, 0x00, 0x00, 0x0A, 0x01, 0x07, 0x20, 0xC8,
            0x01, 0x05, 0x23, 0x20, 0x40, 0x00
        }
    },
    {
      {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
     },/*slic_calibration_resultsArray*/		0, 0,
};

static codec_info_t defaultCodecs[] =
{
    {RTP_PAYLOAD_PCMU,      {30, 20, 10, 0}, -1},
    {RTP_PAYLOAD_PCMA,      {30, 20, 10, 0}, -1},
    {RTP_PAYLOAD_G729,      {30, 20, 10, 0}, -1},
    {RTP_PAYLOAD_G723,      {60, 30, 0}, 1},
	{RTP_PAYLOAD_G726_16,   {30, 20, 10, 0}, -1},//Nirp Vocoders
    {RTP_PAYLOAD_G726_32,   {30, 20, 10, 0}, -1},//Nirp Vocoders
    {RTP_PAYLOAD_G722,      {30, 20, 10, 0}, -1},
    {ILLEGAL_RTP_PAYLOAD, {0, 0, 0, 0}, -1}
};

static char* voip_conf_parse_line(char* line);
void voip_conf_print_voip_config_params();
void voip_conf_print_system_config_params();
void voip_conf_print_regional_settings(voip_regional_settings_params_ex_t *reg_set_params);
void voip_conf_print_phonebook_params(voip_phonebook_record_t *curPhoneRec);

struct timeval *sys_get_time(struct timeval *tv)
{
#ifdef __OS_VXWORKS__
    struct timespec tp;

    clock_gettime(CLOCK_REALTIME, &tp);
    tv->tv_sec = tp.tv_sec;
    tv->tv_usec = tp.tv_nsec / 1000;
#else
    gettimeofday(tv, NULL);
#endif
    return tv;
}

struct timeval *utc_time_get(struct timeval *tv)
{
    sys_get_time(tv);

#ifdef CONFIG_TI_404_COMMON
    if (is_clock_set())
	tv->tv_sec -= time_offset * 60;
#endif

    return tv;
}

time_t utc_time(time_t *t)
{
    struct timeval tv;

    utc_time_get(&tv);

    if (t)
	*t = tv.tv_sec;

    return tv.tv_sec;
}

void voip_conf_convert_strbuf_2_charbuf(char *strbuf, unsigned char *charbuf, int bufsize)
{
	char doubleChar[4];
	int cnt = 0;
	int strbufLen = strlen(strbuf);

	if(strbufLen/2 > bufsize)
	{
		printf("\nInput string size is more then twice bigger then output buffer size!!! Input length = %d  Output Length = %d\n", strbufLen, bufsize);
		return;
	}

	memset(doubleChar, 0, 4);

	while (cnt < strbufLen/2)
	{
		sprintf(doubleChar, "0x%c%c", *strbuf, *(strbuf+1));
		strbuf += 2;
		*charbuf = hex2int(doubleChar);

		charbuf++;
		cnt += 1;
	}

}


/***************************************************************************************/
/*   voip_conf_set_voip_config_defaults                                                */
/***************************************************************************************/
void voip_conf_set_voip_config_defaults()
{
    acl_call_mngr_params_t                      *voip_call_mngr_info = &(voip_config_params.voip_call_mngr_info);
    voip_stun_params_ex_t                       *voip_stun_ex_info = &(voip_config_params.voip_stun_ex_info);
    voip_conference_params_t                    *voip_conference_info = &(voip_config_params.voip_conference_info);
    voip_timeout_params_t                       *voip_timeouts_info = &(voip_config_params.voip_timeouts_info);
    voip_cfw_params_t                           *voip_cfw_info = &(voip_config_params.voip_cfw_info);
	voip_dnd_params_t							*voip_dnd_info = &(voip_config_params.voip_dnd_info);
    voip_mwi_params_t                           *voip_mwi_info = &(voip_config_params.voip_mwi_info);
    voip_conf_codec_ex_t                        *codecs_info = voip_config_params.codecs_info;
    voip_dial_params_t                          *dialingInfo = &(voip_config_params.dialingInfo);
    voip_regional_settings_params_ex_t          *regionalSettingsInfo = &(voip_config_params.regionalSettingsInfo);
    voip_advanced_signaling_params_t            *voip_advanced_signaling_info = &(voip_config_params.voip_advanced_signaling_info);
    voip_sip_info_key_seq_t            *voip_sip_info_key_seq_info = &(voip_config_params.voip_sip_info_key_seq_info);
    /*DSP related parameters*/
    voip_dsp_advanced_channel_configuration_t   *voip_dsp_advanced_channel_configuration = &(voip_config_params.voip_dsp_advanced_channel_configuration);
    int  i;
	char id[11];
    char description[65];

    memset(&voip_config_params, 0, sizeof(voip_config_t));
    voip_call_mngr_info->protocol = VOIP_PROT_SIP;
    voip_call_mngr_info->lines = AC_ZALLOC_E(sizeof(line_conf_t) * MAX_NUM_OF_LINES);
	for (i = 0; i < MAX_NUM_OF_LINES; i++)
	{
	    sprintf(id, "%010d", i+1);
	    sprintf(description, "Line %d", i+1);

        voip_call_mngr_info->lines[i].enabled = 1;
        strcpy(voip_call_mngr_info->lines[i].id, id);
        strcpy(voip_call_mngr_info->lines[i].display_name, description);
        voip_call_mngr_info->lines[i].send_caller_id = 1;
    }

    voip_call_mngr_info->ep_num = MAX_NUM_OF_LINES;
    voip_call_mngr_info->dtmf_transport_method = DTMF_RFC2833;
    voip_call_mngr_info->keys_sequence = KEY_SEQ_FLASH_ONLY;
    voip_call_mngr_info->media_port = 4000;
    voip_call_mngr_info->media_tos = 0xb8;
    voip_call_mngr_info->mss_clamping_limit = 540;
    voip_call_mngr_info->local_host_port = VOIP_LOCAL_PORT_POOL;
    voip_call_mngr_info->local_host_port_range = VOIP_LOCAL_PORT_POOL_RANGE;
    /*voip_call_mngr_info->is_auto_dial = 0;*/
    /*voip_call_mngr_info->auto_dial_timeout = 0;*/
    /*voip_call_mngr_info->auto_dial_dest = "/0";*/
    /*voip_call_mngr_info->use_secondary_dial_tone = 0;*/
    voip_call_mngr_info->secondary_dial_tone_key = 9;

    /*H.323 params ????*/
    if (VOIP_PROT_H323 == voip_call_mngr_info->protocol)
    {
        /*voip_call_mngr_info->sp.h323.e164_alias_list;*/
        /*voip_call_mngr_info->sp.h323.fast_start;*/
        /*voip_call_mngr_info->sp.h323.gk_address = "";*/
        voip_call_mngr_info->sp.h323.gk_port = 1719;
        /*voip_call_mngr_info->sp.h323.gk_registry = 0;*/
        voip_call_mngr_info->sp.h323.signal_port = 1720;
    }

    /*SIP params */
    if (VOIP_PROT_SIP == voip_call_mngr_info->protocol)
    {
        voip_call_mngr_info->sp.sip.sip_transport_protocol = VOIP_SIP_TRANSPORT_UDP;
        /*voip_call_mngr_info->sp.sip.use_proxy = 0;*/
        voip_call_mngr_info->sp.sip.proxy_port = 5060;
        /*voip_call_mngr_info->sp.sip.proxy_address = "/0";*/
        /*voip_call_mngr_info->sp.sip.use_outbound_proxy = 0;*/
        voip_call_mngr_info->sp.sip.outbound_proxy_port = 5060;
        /*voip_call_mngr_info->sp.sip.outbound_proxy_address = "";*/
        /*voip_call_mngr_info->sp.sip.use_registrar = 0;*/
        voip_call_mngr_info->sp.sip.registrar_port = 5060;
        /*voip_call_mngr_info->sp.sip.registrar_address = "";*/
        voip_call_mngr_info->sp.sip.sip_port = 5060;
        /*voip_call_mngr_info->sp.sip.proxy_user_name = "";*/
        /*voip_call_mngr_info->sp.sip.proxy_password = "";*/
        voip_call_mngr_info->sp.sip.proxy_registration_timeout = 6;
        /*voip_call_mngr_info->sp.sip.user_doamin = "";*/
        voip_call_mngr_info->sp.sip.uregistration_timeout = 20;
        /*voip_call_mngr_info->sp.sip.watchdog_timeout = 0;*/
        voip_call_mngr_info->sp.sip.callWaitingEnabled = 1;
        voip_call_mngr_info->sp.sip.callWaitingReply = CW_SIP_REPLY_QUEUED;
        voip_call_mngr_info->sp.sip.retransmissionT1 = 500;
        voip_call_mngr_info->sp.sip.retransmissionT2 = 4000;
        voip_call_mngr_info->sp.sip.retransmissionT4 = 5000;
        voip_call_mngr_info->sp.sip.inviteLingerTimer = 32000;
        voip_call_mngr_info->sp.sip.use_proxy_port_ip_for_registrar = 1;
        voip_call_mngr_info->sp.sip.auth_retries = 4;
        /*voip_call_mngr_info->sp.sip.connectMediaOn180 = 0;*/
#ifdef RV_CFLAG_TLS
        voip_call_mngr_info->sp.sip.transportTlsCfg.stackTlsPort = 5061;
        /*voip_call_mngr_info->sp.sip.transportTlsCfg.stackNumOfTlsAddresses = 0;*/
        voip_call_mngr_info->sp.sip.transportTlsCfg.tlsPostConnectAssertFlag = 1;
        voip_call_mngr_info->sp.sip.transportTlsCfg.tlsMethod = RVSIP_TRANSPORT_TLS_METHOD_TLS_V1;
        voip_call_mngr_info->sp.sip.transportTlsCfg.privateKeyType = RVSIP_TRANSPORT_PRIVATE_KEY_TYPE_RSA_KEY;
        voip_call_mngr_info->sp.sip.transportTlsCfg.certDepth = 5;
#endif /* RV_CFLAG_TLS */
    }

    /*MGCP params ????*/
    if (VOIP_PROT_MGCP == voip_call_mngr_info->protocol)
    {
        voip_call_mngr_info->sp.mgcp.mg_port = 2427;
        /*voip_call_mngr_info->sp.mgcp.mgc_address = "";*/
        voip_call_mngr_info->sp.mgcp.mgc_port = 2727;
    }

    /*STUN*/
    /*voip_stun_ex_info->stunEnabled = 0;*/
    /*voip_stun_ex_info->voip_stun_info.stunClientResponsePort = 0;*/
    /*voip_stun_ex_info->voip_stun_info.stunNeedMask = 0;*/
    /*voip_stun_ex_info->voip_stun_info.stunServerAddress = 0;*/
    voip_stun_ex_info->voip_stun_info.stunServerPort = 3478;


    /*Conference*/
    voip_conference_info->confMode = CONF_LOCAL;
    /*voip_conference_info->confMsAddr = "";*/

    /*Timeouts*/
    voip_timeouts_info->dialToneTimeout = 30;
    voip_timeouts_info->offhookToneTimeout = 120;
    voip_timeouts_info->stutterToneDuration = 2500;
    voip_timeouts_info->unansweredCallTimeout = 60;
    voip_timeouts_info->warningToneTimeout = 40;

    /*Call forward*/
    /*voip_cfw_info->isEnabled = 0;*/
    voip_cfw_info->cfwType = CFW_CFU;
    strcpy(voip_cfw_info->keySequence, "72");
    voip_cfw_info->cfnrTimeout = 1;

	/* AlexChen SDK upgrade dnd features  04072009 */
	voip_dnd_info->isEnabled = 0;
	strcpy(voip_dnd_info->keySequence, "68");
	for(i=0; i<MAX_NUM_OF_LINES; i++)
		voip_dnd_info->line_actived[i] = 0;

    /*Message waiting indication*/
    /*voip_mwi_info->isEnabled = 0;*/
    /*voip_mwi_info->subscribe2MWI = 0;*/
    voip_mwi_info->subsServerPort = 5060;
    /*voip_mwi_info->expiraitionTimeout;*/
    /*voip_mwi_info->subsServerName = "";*/

    /*Codecs info*/
    for (i = 0; defaultCodecs[i].payload_type != -1; i++)
    {
	if(strlen(code2str_ex(rtp_payload_type_t_str, defaultCodecs[i].payload_type)) < sizeof(codecs_info[i].name))
	{
        	strcpy(codecs_info[i].name, code2str_ex(rtp_payload_type_t_str, defaultCodecs[i].payload_type));
	}
	else
	{
   	 	printf("[%s:%d] String length of payload type string %d is equal or bigger then sizeof name %d\r\n",
					__FUNCTION__, __LINE__,
					strlen(code2str_ex(rtp_payload_type_t_str, defaultCodecs[i].payload_type)),
					sizeof(codecs_info[i].name));
		return;
	}

        codecs_info[i].bitRateHi = defaultCodecs[i].brate_hi;
        codecs_info[i].codec.enabled = 1;
        codecs_info[i].codec.payload = defaultCodecs[i].payload_type;
        codecs_info[i].codec.rate = AUDIO_CODEC_RATE;
        codecs_info[i].codec.ptime = defaultCodecs[i].ptime_vals[1];
    }

    /*dialing info*/
    dialingInfo->dialCompleteKeyEnabled = 1;
    dialingInfo->dialCompleteKey = '#';
    dialingInfo->dialStrMaxSize = 15;
    dialingInfo->dialTimeout = 5;
    /*dialingInfo->digitMap = "";*/

    /*Regional settings*/
    strcpy((char *)regionalSettingsInfo->selectedCountry, "USA");
    /*regionalSettingsInfo->useAppConfConfiguration = 0;*/
    memcpy(&(regionalSettingsInfo->regionalSettingsParams), &defaultRegionalSettingsParams, sizeof(regional_settings_params_t));

    /*Advanced signaling info*/
    /*voip_advanced_signaling_info->blindTransferEnabled = 0;*/
    voip_advanced_signaling_info->includePTime = 1;
    /*voip_advanced_signaling_info->kaEnabled = 0;*/
    voip_advanced_signaling_info->kaTimeout = 300;
    voip_advanced_signaling_info->prackEnabled = 1;
    voip_advanced_signaling_info->rportEnabled = 1;
    /*voip_advanced_signaling_info->mssClampingEnabled = 0;*/

    strcpy((char *)voip_sip_info_key_seq_info->sip_info_key_seq_header, "text/plain");
    strcpy((char *)voip_sip_info_key_seq_info->sip_info_key_seq_body, "FLASH");



    /*Dsp advanced channel configuration*/
    /*voip_dsp_advanced_channel_configuration->IsOpenChannelDBInitialized;*/
    /*voip_dsp_advanced_channel_configuration->IsOpenChannelDBInitialized;*/
    /*voip_dsp_advanced_channel_configuration->IsActivateRTTPDBInitialized;*/
    /*voip_dsp_advanced_channel_configuration->ChannelID;*/
    /*voip_dsp_advanced_channel_configuration->AutomaticAgcEnable = 0;*/
    voip_dsp_advanced_channel_configuration->AgcLocation = AUTO_GAIN_CTL_REMOTE;
    voip_dsp_advanced_channel_configuration->AgcTargetEnergy = -19;
    voip_dsp_advanced_channel_configuration->VoiceJitterBufferMinDelay = 35;
    voip_dsp_advanced_channel_configuration->DynamicJitterBufferOptimizationFactor = 7;
    voip_dsp_advanced_channel_configuration->EchoCancelerMode = 0;//1;
	printf("##########EchoCancelerMode=0#########\n");
    /*voip_dsp_advanced_channel_configuration->CngRelayEnable = 0;*/
    voip_dsp_advanced_channel_configuration->FaxMaxRate = FAX_MAX_RATE_14_4_KBPS;
    voip_dsp_advanced_channel_configuration->FaxMaxBuffer = 1024;
    voip_dsp_advanced_channel_configuration->FaxMaxDatagram = 122;
    voip_dsp_advanced_channel_configuration->ImageDataRedundancy = 0;
    voip_dsp_advanced_channel_configuration->T30ControlDataRedundancy = 0;
    voip_dsp_advanced_channel_configuration->FaxModemJit = 0;

    voip_dsp_advanced_channel_configuration->FaxErrorCorrectionMode = 1;
    /*voip_dsp_advanced_channel_configuration->SilenceCompressionMode = 0;*/
    voip_dsp_advanced_channel_configuration->PcmAdPcmComfortNoiseRtpPayloadTypeEnable = 1;
    voip_dsp_advanced_channel_configuration->g726_payload_number = RTP_PAYLOAD_G726_16;
    voip_dsp_advanced_channel_configuration->dtmf_payload_number = RTP_PAYLOAD_DTMF;
    voip_dsp_advanced_channel_configuration->bypassCoder = RTP_PAYLOAD_PCMA;
    voip_dsp_advanced_channel_configuration->faxBypassPayloadType = 102;
    voip_dsp_advanced_channel_configuration->dataBypassPayloadType = 103;

    /*  EA - 080319 - phonebook should be initialized with NULL. we had some very wierd behaviour when allocating
        memory on runtime, I had to allocate all the phonebook here.*/
    voip_config_params.phonebook = AC_ZALLOC_E(sizeof(voip_phonebook_record_t) * MAX_PHONEBOOK_RECORDS);
    voip_config_params.faxTransportMode = T38_TRANSPORT_RELAY;
    voip_config_params.dataTransportMode = DATA_VOICE_BAND_DATA;
    voip_config_params.outOfServiceTone = OUT_OF_SERVICE_REORDER_TONE;
    /*telephone interface*/
    /*voip_config_params.polarityRevasalEnabled = 0;*/

    strcpy(voip_config_params.userAgentStr, USER_AGENT_STRING);

    voip_config_params.numberOfFxsLines = 2;
    voip_config_params.numberOfFxoLines = 0;

    strcpy(voip_config_params.voip_debug_information_info.remoteAddr, PACKET_RECORDING_DEF_REMOTE_ADDR);
    voip_config_params.voip_debug_information_info.remotePort = PACKET_RECORDING_DEF_REMOTE_PORT;
    voip_config_params.voip_debug_information_info.enable_packet_recording = 0;
    voip_config_params.voip_debug_information_info.channel = -1;
    voip_config_params.voip_debug_information_info.enable_network_recording = 0;
    voip_config_params.voip_debug_information_info.enable_tdm_recording = 0;
}
/***************************************************************************************/
/*   voip_conf_set_system_config_defaults                                              */
/***************************************************************************************/
void voip_conf_set_system_config_defaults()
{
    memset(&system_config_params, 0, sizeof(system_config_t));

    /*system_config_params.dbgEnabled = 0;*/
    /*system_config_params.local_ip;*/
    /*system_config_params.timeZoneOffset;*/
    /*system_config_params.wan_mac_addr;*/

    /*system_config_params.system_log_info.rvLogFilter = 0;*/
    /*system_config_params.system_log_info.SIPLogLevel = 0;*/
    strcpy(system_config_params.system_log_info.serverAddrForAppSyslog, SYSLOG_APP_DEF_ADDR);
    system_config_params.system_log_info.serverPortForAppSyslog = SYSLOG_APP_LOG_PORT;
    system_config_params.system_log_info.udpOrTerminalFlag = SYSLOG_PRINT_TO_TERMINAL;
}
/***************************************************************************************/
/*                                                                                     */
/***************************************************************************************/
/***************************************************************************************
 * voip_conf_util_get_next_line
 * -------------------------------------------------------------------------------------
 * General:   Goes through the received buffer, and returns the beginning of the next line,
			  while skipping empty lines.
 * Return Value: Returns a pointer to the beggining of the next line
 * -------------------------------------------------------------------------------------
 * Arguments:
 * Input: 	   string 			- the buffer to be parsed
 * Output:   string pointer 	- pointer to the beggining of the next line
 **************************************************************************************/
static char* voip_conf_util_get_next_line(char** p)
{
	char *result = NULL;

	result = strtok( *p, "\n" );

	if( result == NULL )
	{
		return NULL;
	}
	else
	{
		*p +=(strlen(result) + strlen("\n"));
	}
	return result;

}

int voip_conf_util_load_cfg_file(char* configBuf)
{
    char   	*ptr = NULL;
    char   	*retString = NULL;

    char	*pline;

    if(configBuf !=NULL)
    {
        ptr = configBuf;
    }
    else
    {
        printf("illegal configuration file - NULL\r\n");
        return (-1);
    }

    pline = AC_ZALLOC_L(MAX_LEN);
    if(NULL==pline)
    {
	   printf("Error allocating memory\r\n");
	   return (-1);
    }

    while((retString = voip_conf_util_get_next_line(&ptr))!=NULL)
    {
        strcpy(pline, retString);

        /* Skip lines starting with ';' and '#' */
        if(pline[0]==';' || pline[0]=='#')
            continue;

       // printf("voip_conf_parse_line:: %s \n", pline);

        /*parse the given line*/
        if(!voip_conf_parse_line(pline))
        {
		if(pline)
			nfree(pline);

		printf("\n** Illegal value in the configuration file - Exit **\r\n");
		return (-1);
        }
#if DEBUGGING_PARSER
        printf("\n");
#endif /* DEBUGGING_PARSER */
    }

    if(pline)
	    nfree(pline);

#if DEBUGGING_PARSER
    printf("***************after parsing********************\n");
    voip_conf_print_phonebook_params(voip_config_params.phonebook);
    voip_conf_print_voip_config_params();
    voip_conf_print_system_config_params();
#endif /* DEBUGGING_PARSER */

    return 0;
}

static char* trailing_spaces_clear(char* inputStr)
{
    int len;

    if (NULL == inputStr)
        return inputStr;

    len = strlen(inputStr) - 1;

    while ((inputStr[len] == ' ') || (inputStr[len] == '\t'))/*go over the string as long as you encounter spaces*/
    {
        printf("[%s:%d] ******** ERROR: encountered space or tab ********\n", __FUNCTION__, __LINE__);
        return NULL;

/*      foundation - if we wish to fix.
        inputStr[len] = '\0';
        len--;
*/
    }
    return inputStr;
}

static char* leading_spaces_clear(char* inputStr)
{
    int len = 0;

    if (NULL == inputStr)
        return inputStr;

    while ((inputStr[len] == ' ') || (inputStr[len] == '\t'))/*go over the string as long as you encounter spaces*/
    {
        printf("[%s:%d] ******** ERROR: encountered space or tab ********\n", __FUNCTION__, __LINE__);
        return NULL;
/*      foundation - if we wish to fix.
        len++; /*I increment the len
*/
    }
    return &inputStr[len]; /*the returned string*/
}

static char* spaces_clear(char* inputStr)
{
    char *outputStr = NULL;
    outputStr = trailing_spaces_clear(inputStr);
    outputStr = leading_spaces_clear(outputStr);
    return outputStr;
}

static char* acl_strtok_get_n_clear_token(RvStrTok* t)
{
    char *token = NULL;
    char *end = NULL;
    token = rvStrTokGetToken(t);
    /*clear the \r\n from the value
      find some better way to deal with blanks and nasty characters*/
    end = (char*)rvStrFindFirstOf(token, "\t\r\n");
    if (*end != 0)
        *end = 0;
    token = spaces_clear(token);

#if DEBUGGING_PARSER
    if (token) /* for debugging parser */
    {
        printf("%s/", token);
    }
#endif

   if(NULL == token)
   	printf("[%s:%d] print_rvStrTokGetToken return NULL\n", __FUNCTION__, __LINE__);

    return token;
}

static char* defensive_strcpy(char *dstVal, char *srcVal, int dstSize)
{
    if ((NULL == srcVal) || ((NULL == dstVal)))
        return NULL;
    if ((int)strlen(srcVal) >= dstSize)
    {
        printf("[%s:%d] ******** ERROR: \"%s\" is to long. destination size is only %d ********\n", __FUNCTION__, __LINE__, srcVal, dstSize);
        return NULL;
    }
    /*emtpy strings in the configuration file are set with the word "NULL"*/
    if (0 == strcmp("NULL", srcVal))
        memset(dstVal, 0, dstSize);
    else
        strcpy(dstVal, srcVal);
    return dstVal;
}

static char* enum_validity_check(int inputEnum, char *retval)
{
    if (-1 == inputEnum)
    {
        printf("[%s:%d] ******** ERROR: \"%s\" is an illegal value ********\n", __FUNCTION__, __LINE__, retval);
        return NULL;
    }
    return retval;
}

static char* unallowed_char_replace(char* inputString, char *replaced, char replacing)
{
    char *place = NULL;

    place = (char*)rvStrFindFirstOf(inputString, replaced);
    if (*place != 0)
        *place = replacing;
    return inputString;
}

static char* user_agent_handler(RvStrTok* t)
{
    char *retval = NULL;

    unallowed_char_replace(t->p, "/", '$');
    retval = acl_strtok_get_n_clear_token(t);
    unallowed_char_replace(retval, "$", '/');

    return retval;
}

static char* voip_conf_parse_line(char* line)
{
    char *val = NULL;
    RvStrTok t;
    char *node = NULL;
    char *retval = "Success";
    int index = -1;

    voip_phonebook_record_t *curPhoneRec = NULL;
    voip_phonebook_record_t *lastPhoneRec = NULL;
    static int              firstPhoneBookLine = 0;
    static int              firstCodecParams = 0;

    rvStrTokConstruct(&t, "/=", line); /*do not insert comment with '#'*/

    node = acl_strtok_get_n_clear_token(&t);
   	if(NULL == node)
		return NULL;

    if(!strcmp(node,"acl_conf") )
    {
        /*ignore - this feild is optional*/
        node = acl_strtok_get_n_clear_token(&t);
	if(NULL == node)
		return NULL;
    }
    if(!strcmp(node,"local_ip_address"))
    {
            val = acl_strtok_get_n_clear_token(&t);
            retval = defensive_strcpy(system_config_params.local_ip, val, AC_DOTTED_IP_LEN);
    }
    else if(!strcmp(node,"admin"))
    {
        node = acl_strtok_get_n_clear_token(&t);
	if(NULL == node)
		return NULL;
        if(!strcmp(node,"tz_offset"))
        {
            retval = acl_strtok_get_n_clear_token(&t);
            system_config_params.timeZoneOffset = atoi(retval);
        } /*tz_offset*/
    } /*admin*/
    else if(!strcmp(node,"system"))
    {
        node = acl_strtok_get_n_clear_token(&t);
	if(NULL == node)
		return NULL;

        if(!strcmp(node,"dbg_enabled"))
        {
            retval = acl_strtok_get_n_clear_token(&t);
            system_config_params.dbgEnabled = atoi(retval);
        } /*dbg_enabled*/
        else if(!strcmp(node,"rv_log_filter"))
        {
            retval = acl_strtok_get_n_clear_token(&t);
            system_config_params.system_log_info.rvLogFilter = atoi(retval);
        } /*rv_log_filter*/
    }
    else if(!strcmp(node,"voip"))
    {
        node = acl_strtok_get_n_clear_token(&t);
	if(NULL == node)
		return NULL;
        if(!strcmp(node,"codec"))
        {
            /* If the user configured codec list - clear the default parameters */
            if (0 == firstCodecParams) /*first record*/
            {
		memset(voip_config_params.codecs_info, 0, sizeof(voip_config_params.codecs_info));
        	firstCodecParams = 1;
            }

            index = atoi(acl_strtok_get_n_clear_token(&t)); /*get index*/
            node = acl_strtok_get_n_clear_token(&t);
	    if(NULL == node)
		   return NULL;
            if(!strcmp(node,"enabled"))
            {
                retval = acl_strtok_get_n_clear_token(&t);
                voip_config_params.codecs_info[index].codec.enabled = atoi(retval);
            } /*enabled*/
            else if(!strcmp(node,"name"))
            {
                val = acl_strtok_get_n_clear_token(&t);
                retval = defensive_strcpy(voip_config_params.codecs_info[index].name, val, MAX_CODEC_NAME_LEN);
                voip_config_params.codecs_info[index].codec.payload = str2code_ex(rtp_payload_type_t_str, val);
                retval = enum_validity_check(voip_config_params.codecs_info[index].codec.payload, retval);
            } /*name*/
            else if(!strcmp(node,"ptime"))
            {
                retval = acl_strtok_get_n_clear_token(&t);
                voip_config_params.codecs_info[index].codec.ptime = atoi(retval);
            } /*ptime*/
            else if(!strcmp(node,"bit_rate_hi"))
            {
                retval = acl_strtok_get_n_clear_token(&t);
                voip_config_params.codecs_info[index].bitRateHi = atoi(retval);
            } /*bit_rate_hi*/
	    else if(!strcmp(node, "g722_bit_rate"))
	    {
		retval = acl_strtok_get_n_clear_token(&t);
		voip_config_params.codecs_info[index].codec.g722_bit_rate= atoi(retval);
	    }/* frq of g722 */

	    /*Set codec rate to each codec */
	    voip_config_params.codecs_info[index].codec.rate = AUDIO_CODEC_RATE;
        } /*codec*/
        else if(!strcmp(node,"dial_timeout"))
        {
            retval = acl_strtok_get_n_clear_token(&t);
            voip_config_params.dialingInfo.dialTimeout = atoi(retval);
        } /*dial_timeout*/
        else if(!strcmp(node,"phone_number_max_size"))
        {
            retval = acl_strtok_get_n_clear_token(&t);
            voip_config_params.dialingInfo.dialStrMaxSize = atoi(retval);
        } /*phone_number_max_size*/
        else if(!strcmp(node,"warning_tone_timeout"))
        {
            retval = acl_strtok_get_n_clear_token(&t);
            voip_config_params.voip_timeouts_info.warningToneTimeout = atoi(retval);
        } /*warning_tone_timeout*/
        else if(!strcmp(node,"offhook_tone_timeout"))
        {
            retval = acl_strtok_get_n_clear_token(&t);
            voip_config_params.voip_timeouts_info.offhookToneTimeout = atoi(retval);
        } /*offhook_tone_timeout*/
        else if(!strcmp(node,"fxs"))
        {
            node = acl_strtok_get_n_clear_token(&t);
	    if(NULL == node)
		    return NULL;
            if(!strcmp(node,"polarity_reversal"))
            {
                node = acl_strtok_get_n_clear_token(&t);
		if(NULL == node)
			return NULL;
                if(!strcmp(node,"enabled"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.polarityRevasalEnabled = atoi(retval);
                } /*enabled*/
            } /*polarity_reversal*/
        } /*fxs*/
        else if(!strcmp(node,"unanswered_call_timeout"))
        {
            retval = acl_strtok_get_n_clear_token(&t);
            voip_config_params.voip_timeouts_info.unansweredCallTimeout = atoi(retval);
        } /*unanswered_call_timeout*/
        else if (!strcmp(node,"paging"))
        {
            node = acl_strtok_get_n_clear_token(&t);
            if (!strcmp(node,"enable"))
            {
                retval = acl_strtok_get_n_clear_token(&t);
                voip_config_params.paging.isEnabled = atoi(retval);
            }
            else if (!strcmp(node,"group"))
            {
                node = acl_strtok_get_n_clear_token(&t);
                index = atoi(node); /*get index*/

                if (index >=0 && index < NUM_OF_PAGING)
                {
                    node = acl_strtok_get_n_clear_token(&t);
                    if (!strcmp(node,"address"))
                    {
                        val = acl_strtok_get_n_clear_token(&t);
                        retval = defensive_strcpy(voip_config_params.paging.address[index] , val, PAGING_IP_LENGTH);
                    }
                    else if (!strcmp(node,"port"))
                    {
                        retval = acl_strtok_get_n_clear_token(&t);
                        voip_config_params.paging.port[index] = atoi(retval);
                    }
                }
            }
        }
        else if(!strcmp(node,"signalling"))
        {
            node = acl_strtok_get_n_clear_token(&t);
	    if(NULL == node)
		   return NULL;
            if(!strcmp(node,"protocol"))
            {
                retval = acl_strtok_get_n_clear_token(&t);
                voip_config_params.voip_call_mngr_info.protocol = str2code_ex(voip_protocols_t_str, retval);

                retval = enum_validity_check(voip_config_params.voip_call_mngr_info.protocol, retval);
            } /*protocol*/
            /*I parse SIP params only if SIP is set otherwise it will run over other protocol params since we have a union*/
            else if ((!strcmp(node,"sip")) && (VOIP_PROT_SIP == voip_config_params.voip_call_mngr_info.protocol))
            {
               node = acl_strtok_get_n_clear_token(&t);
		if(NULL == node)
			return NULL;
                if(!strcmp(node,"sdp_include_ptime"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_advanced_signaling_info.includePTime = atoi(retval);
                } /*sdp_include_ptime*/
                else if(!strcmp(node,"transport_protocol"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_call_mngr_info.sp.sip.sip_transport_protocol = str2code_ex(voip_sip_transport_prot_t_str, retval);
                    retval = enum_validity_check(voip_config_params.voip_call_mngr_info.sp.sip.sip_transport_protocol, retval);
                } /*transport_protocol*/
                else if(!strcmp(node,"port"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_call_mngr_info.sp.sip.sip_port = atoi(retval);
                } /*port*/
#ifdef RV_CFLAG_TLS
                else if(!strcmp(node,"tls_port"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_call_mngr_info.sp.sip.transportTlsCfg.stackTlsPort = atoi(retval);
                } /*tls_port*/
#endif /*RV_CFLAG_TLS*/
                else if(!strcmp(node,"proxy_address"))
                {
                    val = acl_strtok_get_n_clear_token(&t);
                    retval = defensive_strcpy(voip_config_params.voip_call_mngr_info.sp.sip.proxy_address, val, (AC_MAX_HOST_NAME_LEN + 1));
                } /*proxy_address*/
                else if(!strcmp(node,"proxy_port"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_call_mngr_info.sp.sip.proxy_port = atoi(retval);
                } /*proxy_port*/
                else if(!strcmp(node,"auth_retries"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_call_mngr_info.sp.sip.auth_retries = atoi(retval);
                } /*auth_retries*/
                else if(!strcmp(node,"proxy_timeout"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_call_mngr_info.sp.sip.proxy_registration_timeout = atoi(retval);
                } /*proxy_timeout*/
                else if(!strcmp(node,"sip_registrar"))
                {
                    node = acl_strtok_get_n_clear_token(&t);
		    if(NULL == node)
			    return NULL;
                    if(!strcmp(node,"enabled"))
                    {
                        retval = acl_strtok_get_n_clear_token(&t);
                        voip_config_params.voip_call_mngr_info.sp.sip.use_registrar = atoi(retval);
                    } /*enabled*/
                    if(!strcmp(node,"port"))
                    {
                        retval = acl_strtok_get_n_clear_token(&t);
                        voip_config_params.voip_call_mngr_info.sp.sip.registrar_port = atoi(retval);
                    } /*port*/
                    if(!strcmp(node,"addr"))
                    {
                        val = acl_strtok_get_n_clear_token(&t);
                        retval = defensive_strcpy(voip_config_params.voip_call_mngr_info.sp.sip.registrar_address, val, (AC_MAX_HOST_NAME_LEN + 1));
                    } /*addr*/
                } /*sip_registrar*/
                else if(!strcmp(node,"sip_outbound_proxy"))
                {
                    node = acl_strtok_get_n_clear_token(&t);
		    if(NULL == node)
			    return NULL;
                    if(!strcmp(node,"enabled"))
                    {
                        retval = acl_strtok_get_n_clear_token(&t);
                        voip_config_params.voip_call_mngr_info.sp.sip.use_outbound_proxy = atoi(retval);
                    } /*enabled*/
                    if(!strcmp(node,"port"))
                    {
                        retval = acl_strtok_get_n_clear_token(&t);
                        voip_config_params.voip_call_mngr_info.sp.sip.outbound_proxy_port = atoi(retval);
                    } /*port*/
                    if(!strcmp(node,"addr"))
                    {
                        val = acl_strtok_get_n_clear_token(&t);
                        retval = defensive_strcpy(voip_config_params.voip_call_mngr_info.sp.sip.outbound_proxy_address , val, (AC_MAX_HOST_NAME_LEN + 1));
                    } /*addr*/
                } /*sip_outbound_proxy*/
                else if(!strcmp(node,"sip_t1"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_call_mngr_info.sp.sip.retransmissionT1 = atoi(retval);
                } /*sip_t1*/
                else if(!strcmp(node,"sip_t2"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_call_mngr_info.sp.sip.retransmissionT2 = atoi(retval);
                } /*sip_t2*/
                else if(!strcmp(node,"sip_t4"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_call_mngr_info.sp.sip.retransmissionT4 = atoi(retval);
                } /*sip_t4*/
                else if(!strcmp(node,"sip_invite_timer"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_call_mngr_info.sp.sip.inviteLingerTimer = atoi(retval);
                } /*sip_invite_timer*/
                else if(!strcmp(node,"proxy_gateway"))
                {
                    val = acl_strtok_get_n_clear_token(&t);
                    retval = defensive_strcpy(voip_config_params.voip_call_mngr_info.sp.sip.user_doamin, val, (AC_MAX_USERNAME_LEN + 1));
                } /*proxy_gateway*/
                else if(!strcmp(node,"digit_map"))
                {
                    val = acl_strtok_get_n_clear_token(&t);
                    retval = defensive_strcpy(voip_config_params.dialingInfo.digitMap, val, MAX_DIGIT_MAP_LEN);
                } /*digit_map*/
                else if(!strcmp(node,"number_rules"))
                {
			/* NirM, because in number_rules node '=' is a valid character we don't want it to be tokenized */
                    retval = defensive_strcpy(voip_config_params.voip_call_mngr_info.sp.sip.numberRules, t.p, MAX_DIGIT_MAP_LEN);
               } /*number_rules*/
                else if(!strcmp(node,"use_proxy_ip_port_for_registrar"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_call_mngr_info.sp.sip.use_proxy_port_ip_for_registrar = atoi(retval);
                } /*use_proxy_ip_port_for_registrar*/
                else if(!strcmp(node,"prack"))
                {
                    node = acl_strtok_get_n_clear_token(&t);
		    if(NULL == node)
			    return NULL;
                    if(!strcmp(node,"enabled"))
                    {
                        retval = acl_strtok_get_n_clear_token(&t);
                        voip_config_params.voip_advanced_signaling_info.prackEnabled = atoi(retval);
                    } /*enabled*/
                } /*prack*/
                else if(!strcmp(node,"rport"))
                {
                    node = acl_strtok_get_n_clear_token(&t);
    		    if(NULL == node)
    			    return NULL;
                    if(!strcmp(node,"enabled"))
                    {
                        retval = acl_strtok_get_n_clear_token(&t);
                        voip_config_params.voip_advanced_signaling_info.rportEnabled = atoi(retval);
                    } /*enabled*/
                } /*rport*/
                else if(!strcmp(node,"connectMediaOn180"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_call_mngr_info.sp.sip.connectMediaOn180 = atoi(retval);
                } /*connectMediaOn180*/
                else if(!strcmp(node,"ka_options"))
                {
                    node = acl_strtok_get_n_clear_token(&t);
		    if(NULL == node)
			    return NULL;
                    if(!strcmp(node,"enabled"))
                    {
                        /*if false override whatever value that there is in ka timeout*/
                        retval = acl_strtok_get_n_clear_token(&t);
                        voip_config_params.voip_advanced_signaling_info.kaEnabled = atoi(retval);
                    } /*enabled*/
                    else if(!strcmp(node,"timeout"))
                    {
                        retval = acl_strtok_get_n_clear_token(&t);
                        voip_config_params.voip_advanced_signaling_info.kaTimeout = atoi(retval);
                    } /*timeout*/
                } /*ka_options*/
                else if(!strcmp(node,"stun"))
                {
                    node = acl_strtok_get_n_clear_token(&t);
		    		if(NULL == node)
			    		return NULL;
                    if(!strcmp(node,"enabled"))
                    {
                        /*if false override whatever disable stun*/
                        retval = acl_strtok_get_n_clear_token(&t);
                        voip_config_params.voip_stun_ex_info.stunEnabled = atoi(retval);
                    } /*enabled*/
                    if(!strcmp(node,"stun_server_ip"))
                    {
                        val = acl_strtok_get_n_clear_token(&t);
                        retval = defensive_strcpy(voip_config_params.voip_stun_ex_info.voip_stun_info.stunServerAddress, val, 64);
                    } /*stun_server_ip*/
                    else if(!strcmp(node,"stun_need_mask"))
                    {
                        val = acl_strtok_get_n_clear_token(&t);
                        retval = defensive_strcpy(voip_config_params.voip_stun_ex_info.voip_stun_info.stunNeedMask, val, 64);
                    } /*stun_need_mask*/
                    else if(!strcmp(node,"stun_server_port"))
                    {
                        retval = acl_strtok_get_n_clear_token(&t);
                        voip_config_params.voip_stun_ex_info.voip_stun_info.stunServerPort = atoi(retval);
                    } /*stun_server_port*/
                    else if(!strcmp(node,"stun_client_port"))
                    {
                        retval = acl_strtok_get_n_clear_token(&t);
                        voip_config_params.voip_stun_ex_info.voip_stun_info.stunClientResponsePort = atoi(retval);
                    } /*stun_client_port*/
                } /*stun*/
				else if(!strcmp(node,"redundant"))
                {
                    node = acl_strtok_get_n_clear_token(&t);
		    		if(NULL == node)
			    		return NULL;
                    if(!strcmp(node,"use_redundant_proxy"))
                    {
                        retval = acl_strtok_get_n_clear_token(&t);
                        voip_config_params.voip_call_mngr_info.sp.sip.use_redundant_proxy= atoi(retval);
                    } /*use_redundant_proxy*/
                    if(!strcmp(node,"redundant_proxy_port"))
                    {
                        retval = acl_strtok_get_n_clear_token(&t);
                        voip_config_params.voip_call_mngr_info.sp.sip.redundant_proxy_port = atoi(retval);
                    } /*redundant_proxy_port*/
                    else if(!strcmp(node,"redundant_proxy_address"))
                    {
                        val = acl_strtok_get_n_clear_token(&t);
                        retval = defensive_strcpy(voip_config_params.voip_call_mngr_info.sp.sip.redundant_proxy_address, val, 64);
                    } /*redundant_proxy_address*/
                    else if(!strcmp(node,"redundant_proxy_ka_period"))
                    {
                        retval = acl_strtok_get_n_clear_token(&t);
                        voip_config_params.voip_call_mngr_info.sp.sip.redundant_proxy_ka_period = atoi(retval);
                    } /*redundant_proxy_ka_period*/
                    else if(!strcmp(node,"redundant_proxy_is_symmetric"))
                    {
                        retval = acl_strtok_get_n_clear_token(&t);
                        voip_config_params.voip_call_mngr_info.sp.sip.redundant_proxy_is_symmetric = atoi(retval);
                    } /*redundant_proxy_is_symmetric*/
					else if(!strcmp(node,"is_redundant_proxy"))
                    {
                        retval = acl_strtok_get_n_clear_token(&t);
                        voip_config_params.voip_call_mngr_info.sp.sip.is_redundant_proxy = atoi(retval);
                    } /*is_redundant_proxy*/
                } /*redundant_proxy*/
                else if(!strcmp(node,"use_proxy"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_call_mngr_info.sp.sip.use_proxy = atoi(retval);
                } /*use_proxy*/
#if 0
                else if(!strcmp(node,"nat_ip"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_call_mngr_info.sp.sip.use_proxy = atoi(retval);
                } /*nat_ip*/
#endif
            } /*sip*/
            /*I parse MGCP params only if MGCP is set otherwise it will run over other protocol paramssince we have a union*/
            else if((!strcmp(node,"mgcp")) && (VOIP_PROT_MGCP == voip_config_params.voip_call_mngr_info.protocol))
            {
                node = acl_strtok_get_n_clear_token(&t);
				if(NULL == node)
					return NULL;
				if(!strcmp(node,"gateway_name"))
                {
                    val = acl_strtok_get_n_clear_token(&t);
                    retval = defensive_strcpy(voip_config_params.voip_call_mngr_info.sp.mgcp.user_domain, val, (AC_MAX_DOMAIN_NAME_LEN + 1));
                } /*gateway_name*/
                if(!strcmp(node,"mgc_address"))
                {
                    val = acl_strtok_get_n_clear_token(&t);
                    retval = defensive_strcpy(voip_config_params.voip_call_mngr_info.sp.mgcp.mgc_address, val, (AC_MAX_HOST_NAME_LEN + 1));
                } /*mgc_address*/
                else if(!strcmp(node,"mgc_port"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_call_mngr_info.sp.mgcp.mgc_port = atoi(retval);
                } /*mgc_port*/
                else if(!strcmp(node,"mg_port"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_call_mngr_info.sp.mgcp.mg_port = atoi(retval);
                } /*mg_port*/
            } /*mgcp*/
        } /*signalling*/
        else if(!strcmp(node,"dial_complete_key"))
        {
            node = acl_strtok_get_n_clear_token(&t);
	    if(NULL == node)
		    return NULL;
            if(!strcmp(node,"enabled"))
            {
                retval = acl_strtok_get_n_clear_token(&t);
                voip_config_params.dialingInfo.dialCompleteKeyEnabled = atoi(retval);
            }
            else if(!strcmp(node,"key"))
            {
                val = acl_strtok_get_n_clear_token(&t);

		if(strlen(val) != 1)
		{
		        printf("[%s:%d] ******** ERROR: \"%s\" is an illegal value ********\n", __FUNCTION__, __LINE__, val);
		        return NULL;
		}
		voip_config_params.dialingInfo.dialCompleteKey = (char)val[0];
            }
        } /*dial_complete_key*/
        else if(!strcmp(node,"out_of_band_dtmf"))
        {
            retval = acl_strtok_get_n_clear_token(&t);
            voip_config_params.voip_call_mngr_info.dtmf_transport_method = str2code_ex(dtmf_transport_mode_t_str, retval);
            retval = enum_validity_check(voip_config_params.voip_call_mngr_info.dtmf_transport_method, retval);
        } /*out_of_band_dtmf*/
        else if(!strcmp(node,"key_sequence_style"))
        {
            retval = acl_strtok_get_n_clear_token(&t);
            voip_config_params.voip_call_mngr_info.keys_sequence = str2code_ex(key_sequence_style_t_str, retval);
            retval = enum_validity_check(voip_config_params.voip_call_mngr_info.keys_sequence, retval);
        } /*key_sequence_style*/
        else if(!strcmp(node,"sip_info_key_seq"))
        {
            node = acl_strtok_get_n_clear_token(&t);
	    if(NULL == node)
		    return NULL;
            if(!strcmp(node,"sip_info_key_seq_header"))
            {
                val = acl_strtok_get_n_clear_token(&t);
                retval = defensive_strcpy(voip_config_params.voip_sip_info_key_seq_info.sip_info_key_seq_header, val, 255);

            	if(strlen(t.p) > 0)
		{
	                node = acl_strtok_get_n_clear_token(&t);

			if(NULL == node)
				return NULL;

			strcat(voip_config_params.voip_sip_info_key_seq_info.sip_info_key_seq_header, "/");
			strcat(voip_config_params.voip_sip_info_key_seq_info.sip_info_key_seq_header, node);
		}
            }
            else if(!strcmp(node,"sip_info_key_seq_body"))
            {
                val = acl_strtok_get_n_clear_token(&t);
                retval = defensive_strcpy(voip_config_params.voip_sip_info_key_seq_info.sip_info_key_seq_body, val, 255);
            }
        } /*sip_info_key_seq*/
        else if(!strcmp(node,"num_of_fxs_lines"))
        {
		retval = acl_strtok_get_n_clear_token(&t);
		voip_config_params.numberOfFxsLines = atoi(retval);
        } /*num_of_fxs_lines*/
        else if(!strcmp(node,"num_of_fxo_lines"))
        {
		retval = acl_strtok_get_n_clear_token(&t);
		voip_config_params.numberOfFxoLines = atoi(retval);
        } /*num_of_fxs_lines*/
        else if(!strcmp(node,"dtmf_payload"))
        {
            retval = acl_strtok_get_n_clear_token(&t);
            voip_config_params.voip_dsp_advanced_channel_configuration.dtmf_payload_number = atoi(retval);
        } /*dtmf_payload*/
        else if(!strcmp(node,"g726_payload"))
        {
            retval = acl_strtok_get_n_clear_token(&t);
            voip_config_params.voip_dsp_advanced_channel_configuration.g726_payload_number = atoi(retval);
        } /*g726_payload*/
#if 0
        else if(!strcmp(node,"comfort_noise_payload"))
        {
        } /*comfort_noise_payload*/
#endif
        else if(!strcmp(node,"services"))
        {
            node = acl_strtok_get_n_clear_token(&t);
	    if(NULL == node)
		    return NULL;
            if(!strcmp(node,"call_waiting"))
            {
                node = acl_strtok_get_n_clear_token(&t);
		if(NULL == node)
			return NULL;
                if(!strcmp(node,"enabled"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_call_mngr_info.sp.sip.callWaitingEnabled = atoi(retval);
                } /*enabled*/
                else if(!strcmp(node,"sip_reply"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_call_mngr_info.sp.sip.callWaitingReply = str2code_ex(cw_sip_reply_t_str, retval);
                    retval = enum_validity_check(voip_config_params.voip_call_mngr_info.sp.sip.callWaitingReply, retval);
                } /*sip_reply*/
				if(!strcmp(node,"type2_enabled"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_call_mngr_info.sp.sip.callWaitingType2Enabled = atoi(retval);
                } /*type2_enabled*/
            } /*call_waiting*/
            else if(!strcmp(node,"conference"))
            {
                node = acl_strtok_get_n_clear_token(&t);
		if(NULL == node)
			return NULL;
                if(!strcmp(node,"conf_mode"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_conference_info.confMode = str2code_ex(conf_mode_t_str, retval);
                    retval = enum_validity_check(voip_config_params.voip_conference_info.confMode, retval);
                } /*conf_mode*/
                else if(!strcmp(node,"conf_ms_addr"))
                {
                    val = acl_strtok_get_n_clear_token(&t);
                    retval = defensive_strcpy(voip_config_params.voip_conference_info.confMsAddr, val, AC_DOTTED_IP_LEN);
                } /*conf_ms_addr*/
            } /*conference*/
            else if(!strcmp(node,"call_forward"))
            {
                node = acl_strtok_get_n_clear_token(&t);
		if(NULL == node)
			return NULL;
                if(!strcmp(node,"enabled"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_cfw_info.isEnabled = atoi(retval);
                } /*enabled*/
                else if(!strcmp(node,"cfnr_timeout"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_cfw_info.cfnrTimeout = atoi(retval);
                } /*cfnr_timeout*/
                else if(!strcmp(node,"cfw_type"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_cfw_info.cfwType = str2code_ex(cfw_type_t_str, retval);
                    retval = enum_validity_check(voip_config_params.voip_cfw_info.cfwType, retval);
                } /*cfw_type*/
                else if(!strcmp(node,"sequence"))
                {
                    val = acl_strtok_get_n_clear_token(&t);
                    retval = defensive_strcpy(voip_config_params.voip_cfw_info.keySequence, val, (MAX_KEY_SEQ_STRING_LEN + 1));
                } /*sequence*/
				else if(!strcmp(node,"line"))
				{
					index = atoi(acl_strtok_get_n_clear_token(&t)); /*get index*/
            		node = acl_strtok_get_n_clear_token(&t);
					if(NULL == node)
		    			return NULL;
            		if(!strcmp(node,"cfw_dest"))
            		{
                		val = acl_strtok_get_n_clear_token(&t);
                    	retval = defensive_strcpy(voip_config_params.voip_cfw_info.line[index].cfw_dest, val, (AC_MAX_DOMAIN_NAME_LEN + 1));
            		} /*cfw_dest*/
				}
            } /*call_forward*/
            else if(!strcmp(node,"msg_waiting_ind"))
            {
                node = acl_strtok_get_n_clear_token(&t);
		if(NULL == node)
			return NULL;
                if(!strcmp(node,"enabled"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_mwi_info.isEnabled = atoi(retval);
                } /*enabled*/
                else if(!strcmp(node,"subscribe"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_mwi_info.subscribe2MWI = atoi(retval);
                } /*subscribe*/
                else if(!strcmp(node,"subscribe_port"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_mwi_info.subsServerPort = atoi(retval);
                } /*subscribe_port*/
                else if(!strcmp(node,"subscribe_ip"))
                {
                    val = acl_strtok_get_n_clear_token(&t);
                    retval = defensive_strcpy(voip_config_params.voip_mwi_info.subsServerName, val, (AC_MAX_HOST_NAME_LEN + 1));
                } /*subscribe_ip*/
                else if(!strcmp(node,"expiration_timeout"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_mwi_info.expiraitionTimeout = atoi(retval);
                } /*subscribe_port*/
            } /*msg_waiting_ind*/
            else if(!strcmp(node,"stutter_tone_dur"))
            {
                retval = acl_strtok_get_n_clear_token(&t);
                voip_config_params.voip_timeouts_info.stutterToneDuration = atoi(retval);
            } /*stutter_tone_dur*/
			else if(!strcmp(node,"blind_transfer"))
            {
                retval = acl_strtok_get_n_clear_token(&t);
                voip_config_params.voip_advanced_signaling_info.blindTransferEnabled = atoi(retval);
            } /*blind_transfer*/
            else if(!strcmp(node,"out_of_service_bahavior"))
            {
                retval = acl_strtok_get_n_clear_token(&t);
                voip_config_params.outOfServiceTone = str2code_ex(out_of_service_tone_t_str, retval);
                retval = enum_validity_check(voip_config_params.outOfServiceTone, retval);
            } /*out_of_service_bahavior*/
			else if(!strcmp(node,"do_not_disturb"))
            {
                node = acl_strtok_get_n_clear_token(&t);
				if(NULL == node)
					return NULL;
                if(!strcmp(node,"enabled"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_dnd_info.isEnabled = atoi(retval);
                } /*enabled*/
                else if(!strcmp(node,"sequence"))
                {
                    val = acl_strtok_get_n_clear_token(&t);
                    retval = defensive_strcpy(voip_config_params.voip_dnd_info.keySequence, val, (MAX_KEY_SEQ_STRING_LEN + 1));
                } /*sequence*/
				else if(!strcmp(node,"line"))
				{
					index = atoi(acl_strtok_get_n_clear_token(&t)); /*get index*/
            		node = acl_strtok_get_n_clear_token(&t);
					if(NULL == node)
		    			return NULL;
            		if(!strcmp(node,"actived"))
            		{
                		retval = acl_strtok_get_n_clear_token(&t);
                		voip_config_params.voip_dnd_info.line_actived[index] = atoi(retval);
            		} /*actived*/
				}
            } /*do_not_disturb*/
        } /*services*/
        else if(!strcmp(node,"media_port"))
        {
            retval = acl_strtok_get_n_clear_token(&t);
            voip_config_params.voip_call_mngr_info.media_port = atoi(retval);
        } /*media_port*/
        else if(!strcmp(node,"media_tos"))
        {
            retval = acl_strtok_get_n_clear_token(&t);
            voip_config_params.voip_call_mngr_info.media_tos = hex2int(retval);
        } /*media_tos*/
        else if(!strcmp(node,"mss_clamping"))
        {
            node = acl_strtok_get_n_clear_token(&t);
	    if(NULL == node)
		    return NULL;
            if(!strcmp(node,"enabled"))
            {
                retval = acl_strtok_get_n_clear_token(&t);
                voip_config_params.voip_advanced_signaling_info.mssClampingEnabled = atoi(retval);
            } /*enabled*/
            else if(!strcmp(node,"value"))
            {
                retval = acl_strtok_get_n_clear_token(&t);
                voip_config_params.voip_call_mngr_info.mss_clamping_limit = atoi(retval);
            } /*value*/
        } /*mss_clamping*/
        else if(!strcmp(node,"dialtone_timeout"))
        {
            retval = acl_strtok_get_n_clear_token(&t);
            voip_config_params.voip_timeouts_info.dialToneTimeout = atoi(retval);
        } /*dialtone_timeout*/
#if 0
        else if(!strcmp(node,"flash_min"))
        {
        } /*flash_min*/
        else if(!strcmp(node,"flash_max"))
        {
        } /*flash_max*/
#endif
        else if(!strcmp(node,"auto_dialing"))
        {
            node = acl_strtok_get_n_clear_token(&t);
	    if(NULL == node)
		    return NULL;
            if(!strcmp(node,"enabled"))
            {
                retval = acl_strtok_get_n_clear_token(&t);
                voip_config_params.voip_call_mngr_info.is_auto_dial = atoi(retval);
            } /*enabled*/
            else if(!strcmp(node,"timeout"))
            {
                retval = acl_strtok_get_n_clear_token(&t);
                voip_config_params.voip_call_mngr_info.auto_dial_timeout = atoi(retval);
            } /*timeout*/
            else if(!strcmp(node,"destination"))
            {
                val = acl_strtok_get_n_clear_token(&t);
                retval = defensive_strcpy(voip_config_params.voip_call_mngr_info.auto_dial_dest, val, (AC_MAX_DOMAIN_NAME_LEN + 1));
            } /*destination*/
        } /*auto_dialing*/
        else if(!strcmp(node,"secondary_dial_tone"))
        {
            node = acl_strtok_get_n_clear_token(&t);
	    if(NULL == node)
		    return NULL;
            if(!strcmp(node,"enabled"))
            {
                retval = acl_strtok_get_n_clear_token(&t);
                voip_config_params.voip_call_mngr_info.use_secondary_dial_tone = atoi(retval);
            } /*enabled*/
            else if(!strcmp(node,"key_sequence"))
            {
                retval = acl_strtok_get_n_clear_token(&t);
                voip_config_params.voip_call_mngr_info.secondary_dial_tone_key = atoi(retval);
            } /*key_sequence*/
        } /*secondary_dial_tone*/
        else if(!strcmp(node,"line"))
        {
            index = atoi(acl_strtok_get_n_clear_token(&t)); /*get index*/
            node = acl_strtok_get_n_clear_token(&t);
	    if(NULL == node)
		    return NULL;
            if(!strcmp(node,"enabled"))
            {
                retval = acl_strtok_get_n_clear_token(&t);
                voip_config_params.voip_call_mngr_info.lines[index].enabled = atoi(retval);
            } /*enabled*/
            else if(!strcmp(node,"id"))
            {
                val = acl_strtok_get_n_clear_token(&t);
                retval = defensive_strcpy(voip_config_params.voip_call_mngr_info.lines[index].id, val, MAX_CALLER_ID_SIZE);
            } /*id*/
            else if(!strcmp(node,"snd_callerid"))
            {
                retval = acl_strtok_get_n_clear_token(&t);
                voip_config_params.voip_call_mngr_info.lines[index].send_caller_id = atoi(retval);
            } /*snd_callerid*/
            else if(!strcmp(node,"description"))
            {
                val = acl_strtok_get_n_clear_token(&t);
                retval = defensive_strcpy(voip_config_params.voip_call_mngr_info.lines[index].display_name, val, 255);
            } /*description*/
            else if(!strcmp(node,"auth_name"))
            {
                val = acl_strtok_get_n_clear_token(&t);
                retval = defensive_strcpy(voip_config_params.voip_call_mngr_info.lines[index].usernameServerAuth, val, MAX_USENAME_STR_SIZE);
            } /*auth_name*/
            else if(!strcmp(node,"auth_password"))
            {
                val = acl_strtok_get_n_clear_token(&t);
                retval = defensive_strcpy(voip_config_params.voip_call_mngr_info.lines[index].pswdServerAuth, val, MAX_PSWD_STR_SIZE);
            } /*auth_password*/
	 	else if(!strcmp(node,"line_voice_volume"))
	 	{
	                retval = acl_strtok_get_n_clear_token(&t);
		 	voip_config_params.voip_dsp_advanced_channel_configuration.VoiceOutputGainChannelArr[index] = atoi(retval);
	 	} /*line_voice_volume*/
        } /*line*/
#if 0
        else if(!strcmp(node,"ipphone"))
        {
            node = acl_strtok_get_n_clear_token(&t);
	    if(NULL == node)
		    return NULL;
            if(!strcmp(node,"ringing"))
            {
                node = acl_strtok_get_n_clear_token(&t);
		if(NULL == node)
			return NULL;
                if(!strcmp(node,"mute"))
                {
                } /*mute*/
                else if(!strcmp(node,"volume"))
                {
                } /*volume*/
            } /*ringing*/
            else if(!strcmp(node,"handset"))
            {
                node = acl_strtok_get_n_clear_token(&t);
		if(NULL == node)
			return NULL;
                if(!strcmp(node,"volume"))
                {
                } /*volume*/
            } /*handset*/
            else if(!strcmp(node,"loudspeaker"))
            {
                node = acl_strtok_get_n_clear_token(&t);
		if(NULL == node)
			return NULL;
                if(!strcmp(node,"volume"))
                {
                } /*volume*/
            } /*loudspeaker*/
        } /*ipphone*/
#endif
        else if(!strcmp(node,"audio"))
        {
            node = acl_strtok_get_n_clear_token(&t);
	    if(NULL == node)
		    return NULL;
            if(!strcmp(node,"jitter_buffer"))
            {
                node = acl_strtok_get_n_clear_token(&t);
		if(NULL == node)
			return NULL;
                if(!strcmp(node,"min_delay"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_dsp_advanced_channel_configuration.VoiceJitterBufferMinDelay = atoi(retval);
                } /*min_delay*/
                else if(!strcmp(node,"optimization_factor"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_dsp_advanced_channel_configuration.DynamicJitterBufferOptimizationFactor = atoi(retval);
                } /*optimization_factor*/
            } /*jitter_buffer*/
            else if(!strcmp(node,"echo_cancellation"))
            {
                node = acl_strtok_get_n_clear_token(&t);
		if(NULL == node)
			return NULL;
                if(!strcmp(node,"enabled"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_dsp_advanced_channel_configuration.EchoCancelerMode = atoi(retval);
                } /*enabled*/
            } /*echo_cancellation*/
        else if(!strcmp(node,"rtpc_xr"))
        {
            index = atoi(acl_strtok_get_n_clear_token(&t)); /*get index*/
            node = acl_strtok_get_n_clear_token(&t);
	    if(NULL == node)
		   return NULL;
            if(!strcmp(node,"enabled"))
            {
                retval = acl_strtok_get_n_clear_token(&t);
                voip_config_params.voip_dsp_advanced_channel_configuration.RtcpXrEnable[index] = atoi(retval);
            } /*enabled*/
        } /*rtpc_xr*/
            else if(!strcmp(node,"automatic_gain_control"))
            {
                node = acl_strtok_get_n_clear_token(&t);
		if(NULL == node)
			return NULL;
                if(!strcmp(node,"auto_gain_enabled"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_dsp_advanced_channel_configuration.AutomaticAgcEnable = atoi(retval);
                } /*auto_gain_enabled*/
                else if(!strcmp(node,"auto_gain_location"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_dsp_advanced_channel_configuration.AgcLocation = str2code_ex(auto_gain_ctl_mode_location_t_str, retval);
                    retval = enum_validity_check(voip_config_params.voip_dsp_advanced_channel_configuration.AgcLocation, retval);
                } /*auto_gain_location*/
                else if(!strcmp(node,"auto_gain_target_energy"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_dsp_advanced_channel_configuration.AgcTargetEnergy = atoi(retval);
                } /*auto_gain_target_energy*/
            } /*automatic_gain_control*/
            else if(!strcmp(node,"silence_compression_enable"))
            {
                retval = acl_strtok_get_n_clear_token(&t);
                voip_config_params.voip_dsp_advanced_channel_configuration.SilenceCompressionMode = atoi(retval);
	    } /*silence_compression_enable*/
            else if(!strcmp(node,"g711_g726_comfort_noise_enable"))
            {
                retval = acl_strtok_get_n_clear_token(&t);
                voip_config_params.voip_dsp_advanced_channel_configuration.PcmAdPcmComfortNoiseRtpPayloadTypeEnable = atoi(retval);
            } /*g711_g726_comfort_noise_enable*/
            else if(!strcmp(node,"fax"))
            {
                node = acl_strtok_get_n_clear_token(&t);
		if(NULL == node)
			return NULL;
                if(!strcmp(node,"bypass_coder"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_dsp_advanced_channel_configuration.bypassCoder = str2code_ex(rtp_payload_type_t_str, retval);
                    retval = enum_validity_check(voip_config_params.voip_dsp_advanced_channel_configuration.bypassCoder, retval);
                } /*bypass_coder*/
                else if(!strcmp(node,"cng_detection_enabled"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_dsp_advanced_channel_configuration.CngRelayEnable = atoi(retval);
                } /*cng_detection_enabled*/
				else if(!strcmp(node,"remote_fax_side_reinvite"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_call_mngr_info.sp.sip.remote_fax_side_reinvite = atoi(retval);
                } /*remote_fax_side_reinvite*/
				else if(!strcmp(node,"update_fax_to_transparent_enable"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_call_mngr_info.update_fax_to_transparent_enable = atoi(retval);
                } /*update_fax_to_transparent_enable*/
				else if(!strcmp(node,"fax_end_report"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_call_mngr_info.sp.mgcp.fax_end_report = atoi(retval);
                } /*fax_end_report*/
                else if(!strcmp(node,"max_rate"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_dsp_advanced_channel_configuration.FaxMaxRate = str2code_ex(fax_max_rate_t_str, retval);
                    retval = enum_validity_check(voip_config_params.voip_dsp_advanced_channel_configuration.FaxMaxRate, retval);
                } /*max_rate*/
                else if(!strcmp(node,"max_buffer"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_dsp_advanced_channel_configuration.FaxMaxBuffer = atoi(retval);
                } /*max_buffer*/
                else if(!strcmp(node,"max_datagram"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_dsp_advanced_channel_configuration.FaxMaxDatagram = atoi(retval);
                } /*max_buffer*/

		/* Support the below two configuration formats due to typo in the official release */
                else if(!strcmp(node,"error_coerrection_enable") || !strcmp(node,"error_correction_enable"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_dsp_advanced_channel_configuration.FaxErrorCorrectionMode = atoi(retval);
                } /*error_coerrection_enable||error_correction_enable*/
                else if(!strcmp(node,"fax_bypass_payload"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_dsp_advanced_channel_configuration.faxBypassPayloadType = atoi(retval);
                } /*fax_bypass_payload*/
                else if(!strcmp(node,"data_bypass_payload"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_dsp_advanced_channel_configuration.dataBypassPayloadType = atoi(retval);
                } /*data_bypass_payload*/
                else if(!strcmp(node,"fax_transport_mode"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.faxTransportMode = str2code_ex(t38_transport_mode_t_str, retval);
                    retval = enum_validity_check(voip_config_params.faxTransportMode, retval);
                } /*fax_transport_mode*/
                else if(!strcmp(node,"data_transport_mode"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.dataTransportMode = str2code_ex(data_transport_mode_t_str, retval);
                    retval = enum_validity_check(voip_config_params.dataTransportMode, retval);
                } /*data_transport_mode*/
				else if(!strcmp(node,"audio_startup_enabled"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_call_mngr_info.fax_audio_startup_enabled = atoi(retval);
                } /*audio_startup_enabled*/
				else if(!strcmp(node,"audio_startup_payload"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.voip_call_mngr_info.fax_audio_startup_payload = atoi(retval);
                } /*audio_startup_payload*/
            } /*fax*/
        } /*audio*/
        else if(!strcmp(node,"regional_settings"))
        {
            node = acl_strtok_get_n_clear_token(&t);
	    if(NULL == node)
		    return NULL;
            if(!strcmp(node,"selected_country"))
            {
                val = acl_strtok_get_n_clear_token(&t);
                retval = defensive_strcpy((char *)voip_config_params.regionalSettingsInfo.selectedCountry, val, REGIONAL_SETTINGS_MAX_COUNTRY_NAME_LEN);
            } /*selected_country*/
            else if(!strcmp(node,"acl_conf_country"))
            {
                val = acl_strtok_get_n_clear_token(&t);
                retval = defensive_strcpy((char *)voip_config_params.regionalSettingsInfo.regionalSettingsParams.CountryName, val, REGIONAL_SETTINGS_MAX_COUNTRY_NAME_LEN);
            } /*acl_conf_country*/
            else if(!strcmp(node,"use_acl_conf_configuration"))
            {
                retval = acl_strtok_get_n_clear_token(&t);
                voip_config_params.regionalSettingsInfo.useAppConfConfiguration = atoi(retval);

		/* If user wants to set call progress tones values from configuration file - remove the default values */
		if(voip_config_params.regionalSettingsInfo.useAppConfConfiguration)
		{
			memset(&voip_config_params.regionalSettingsInfo.regionalSettingsParams.dsp_regional_settings_params,
						0, sizeof( voip_config_params.regionalSettingsInfo.regionalSettingsParams.dsp_regional_settings_params));
		}
            } /*use_acl_conf_configuration*/
            else if(!strcmp(node,"caller_id_standard"))
            {
                retval = acl_strtok_get_n_clear_token(&t);
                voip_config_params.regionalSettingsInfo.regionalSettingsParams.dsp_regional_settings_params.CallerIDStandard = atoi(retval);
            } /*caller_id_standard*/
            else if(!strcmp(node,"call_progress_tones"))
            {
                index = atoi(acl_strtok_get_n_clear_token(&t)); /*get index*/
                node = acl_strtok_get_n_clear_token(&t);
		if(NULL == node)
			return NULL;
                if(!strcmp(node,"tone_enabled"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.regionalSettingsInfo.regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[index].EnableField = atoi(retval);
                } /*tone_enabled*/
                else if(!strcmp(node,"tone_name"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                } /*tone_name*/
                else if(!strcmp(node,"tone_cadence_type"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.regionalSettingsInfo.regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[index].ToneCadenceType = atoi(retval);
                } /*tone_cadence_type*/
                else if(!strcmp(node,"frequency_a"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.regionalSettingsInfo.regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[index].FreqA = atoi(retval);
                } /*frequency_a*/
                else if(!strcmp(node,"frequency_b"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.regionalSettingsInfo.regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[index].FreqB = atoi(retval);
                } /*frequency_b*/
                else if(!strcmp(node,"frequency_a_level"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.regionalSettingsInfo.regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[index].FreqLevelA = atoi(retval);
                } /*frequency_a_level*/
                else if(!strcmp(node,"frequency_b_level"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.regionalSettingsInfo.regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[index].FreqLevelB = atoi(retval);
                } /*frequency_b_level*/
                else if(!strcmp(node,"tone_on_0"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.regionalSettingsInfo.regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[index].ToneOn0 = atoi(retval);
                } /*tone_on_0*/
                else if(!strcmp(node,"tone_off_0"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.regionalSettingsInfo.regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[index].ToneOff0 = atoi(retval);
                } /*tone_off_0*/
                else if(!strcmp(node,"tone_on_1"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.regionalSettingsInfo.regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[index].ToneOn1 = atoi(retval);
                } /*tone_on_1*/
                else if(!strcmp(node,"tone_off_1"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.regionalSettingsInfo.regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[index].ToneOff1 = atoi(retval);
                } /*tone_off_1*/
                else if(!strcmp(node,"tone_on_2"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.regionalSettingsInfo.regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[index].ToneOn2 = atoi(retval);
                } /*tone_on_2*/
                else if(!strcmp(node,"tone_off_2"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.regionalSettingsInfo.regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[index].ToneOff2 = atoi(retval);
                } /*tone_off_2*/
                else if(!strcmp(node,"tone_on_3"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.regionalSettingsInfo.regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[index].ToneOn3= atoi(retval);
                } /*tone_on_3*/
                else if(!strcmp(node,"tone_off_3"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_config_params.regionalSettingsInfo.regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[index].ToneOff3 = atoi(retval);
                } /*tone_off_3*/
            } /*call_progress_tones*/
            else if(!strcmp(node,"fxs_configuration"))
            {
                node = acl_strtok_get_n_clear_token(&t);
		if(NULL == node)
			return NULL;
                if(!strcmp(node,"fxs_coefficients"))
                {
	                retval = acl_strtok_get_n_clear_token(&t);
			voip_conf_convert_strbuf_2_charbuf(retval,
                                                       voip_config_params.regionalSettingsInfo.regionalSettingsParams.slic_regional_settings_params.FXS_COEFFICIENTS,
                                                       REGIONAL_SETTINGS_FXS_COEFFICIENTS_PARAMS_LEN);

                    /*strcpy(voip_config_params.regionalSettingsInfo.regionalSettingsParams.slic_regional_settings_params.FXS_COEFFICIENTS, val);*/
                } /*fxs_coefficients*/
                else if(!strcmp(node,"fxs_ring_cadence_profile"))
                {
                    retval = acl_strtok_get_n_clear_token(&t);
                    voip_conf_convert_strbuf_2_charbuf(retval,
                                                       voip_config_params.regionalSettingsInfo.regionalSettingsParams.slic_regional_settings_params.FXS_RING_CAD_PROFILE,
                                                       REGIONAL_SETTINGS_FXS_CADENCE_PARAMS_LEN);
                    /*strcpy(voip_config_params.regionalSettingsInfo.regionalSettingsParams.slic_regional_settings_params.FXS_RING_CAD_PROFILE, val);*/
                } /*fxs_ring_cadence_profile*/
            } /*fxs_configuration*/
        } /*regional_settings*/
        else if(!strcmp(node,"log_level"))
        {
            retval = acl_strtok_get_n_clear_token(&t);
            system_config_params.system_log_info.SIPLogLevel = atoi(retval);
        } /*log_level*/
		else if(!strcmp(node,"rtp_mute_on_hold"))
        {
            retval = acl_strtok_get_n_clear_token(&t);
            voip_config_params.voip_call_mngr_info.rtp_mute_on_hold = atoi(retval);
        } /*rtp_mute_on_hold*/
		else if(!strcmp(node,"regret_call_enabled"))
        {
            retval = acl_strtok_get_n_clear_token(&t);
            voip_config_params.voip_call_mngr_info.regret_call_enabled = atoi(retval);
        } /*regret_call_enabled*/
		else if(!strcmp(node,"regret_call_timeout"))
        {
            retval = acl_strtok_get_n_clear_token(&t);
            voip_config_params.voip_call_mngr_info.regret_call_timeout = atoi(retval);
        } /*regret_call_timeout*/
        else if(!strcmp(node,"syslog"))
        {
            node = acl_strtok_get_n_clear_token(&t);
	    if(NULL == node)
		   return NULL;
            if(!strcmp(node,"syslog_server_addr"))
            {
                val = acl_strtok_get_n_clear_token(&t);
                retval = defensive_strcpy(system_config_params.system_log_info.serverAddrForAppSyslog, val, AC_DOTTED_IP_LEN);
            } /*syslog_server_addr*/
            else if(!strcmp(node,"syslog_server_port"))
            {
                retval = acl_strtok_get_n_clear_token(&t);
                system_config_params.system_log_info.serverPortForAppSyslog = atoi(retval);
            } /*syslog_server_port*/
            else if(!strcmp(node,"syslog_udp_terminal_flag"))
            {
                retval = acl_strtok_get_n_clear_token(&t);
                system_config_params.system_log_info.udpOrTerminalFlag = atoi(retval);
            } /*syslog_udp_terminal_flag*/
        } /*syslog*/
        else if(!strcmp(node,"user_agent"))
        {
            val = user_agent_handler(&t);
            retval = defensive_strcpy(voip_config_params.userAgentStr, val, (MAX_USER_AGENT_LEN+1));
        } /*user_agent*/
        else if(!strcmp(node,"phonebook"))
        {
            curPhoneRec = voip_config_params.phonebook;
            lastPhoneRec = NULL;

            index = atoi(acl_strtok_get_n_clear_token(&t)); /*get index*/

            /*find if there is already a record for the given index*/
            if (0 == firstPhoneBookLine) /*first record*/
            {
                curPhoneRec->next = NULL;
                curPhoneRec->index = index;
                firstPhoneBookLine = 1;
            }
            while (curPhoneRec != NULL)
            {
                lastPhoneRec = curPhoneRec;
                if (curPhoneRec->index != index)
                    curPhoneRec = curPhoneRec->next;
                else /*found it*/
                    break;
            }
            if (curPhoneRec == NULL) /*did not find it - allocate*/
            {
                /*  EA - 080319 - I commented the allocation - we cannot allocate during runtime due to wierd behaviour
                    the phonebook is allocated upon init of the acl_voip_conf module. just make the appropriate links
                curPhoneRec = AC_ZALLOC_E(sizeof(voip_phonebook_record_t));
                if (NULL == voip_config_params.phonebook)
                    voip_config_params.phonebook = curPhoneRec;
                */
                /*  curPhoneRec was incremented using the linked list mechanism - now this will return a valid record only if it
                    was "initialized". if you are inside this if, then it wasnt. now I reverse curPhoneRec to point to the last valid
                    record and now I increment to the next record on the phonebook array*/
                curPhoneRec = lastPhoneRec;
                curPhoneRec++;
                if (lastPhoneRec)
                    lastPhoneRec->next = curPhoneRec; /*keep track*/
                curPhoneRec->next = NULL;
                curPhoneRec->index = index;
            }
            /*if you reached here then you surely have an allocated record for the given index*/
            node = acl_strtok_get_n_clear_token(&t);
	    if(NULL == node)
		    return NULL;
            if(!strcmp(node,"number"))
            {
                val = acl_strtok_get_n_clear_token(&t);
                retval = defensive_strcpy(curPhoneRec->dialString, val, MAX_PHONEBOOK_DIAL_STR_LEN);
            } /*number*/
            else if(!strcmp(node,"destination_type"))
            {
                retval = acl_strtok_get_n_clear_token(&t);
                curPhoneRec->destinationType = str2code_ex(voip_destination_type_t_str, retval);
                retval = enum_validity_check(curPhoneRec->destinationType, retval);
            } /*destination_type*/
            else if(!strcmp(node,"local_line"))
            {
	            retval = acl_strtok_get_n_clear_token(&t);
                    curPhoneRec->localLine = atoi(retval);
            } /*destination_type*/
            else if(!strcmp(node,"user_id"))
            {
                val = acl_strtok_get_n_clear_token(&t);
                retval = defensive_strcpy(curPhoneRec->userId, val, MAX_PHONEBOOK_DIAL_STR_LEN);
            } /*user_id*/
            else if(!strcmp(node,"user_address"))
            {
                val = acl_strtok_get_n_clear_token(&t);
                retval = defensive_strcpy(curPhoneRec->userAddress, val, AC_DOTTED_IP_LEN);
            } /*user_address*/
            else if(!strcmp(node,"user_port"))
            {
                retval = acl_strtok_get_n_clear_token(&t);
                curPhoneRec->userPort = atoi(retval);
            } /*user_port*/
        } /*phonebook*/
        else if(!strcmp(node,"packet_recording"))
        {
            node = acl_strtok_get_n_clear_token(&t);
            if(NULL == node)
                return NULL;
            if(!strcmp(node,"enabled"))
            {
                retval = acl_strtok_get_n_clear_token(&t);
                voip_config_params.voip_debug_information_info.enable_packet_recording= atoi(retval);
            } /*enabled*/
            else if(!strcmp(node,"remote_ip"))
            {
                val = acl_strtok_get_n_clear_token(&t);
                retval = defensive_strcpy(voip_config_params.voip_debug_information_info.remoteAddr, val, AC_DOTTED_IP_LEN);
            } /*remote_ip*/
            else if(!strcmp(node,"remote_port"))
            {
                retval = acl_strtok_get_n_clear_token(&t);
                voip_config_params.voip_debug_information_info.remotePort = atoi(retval);
            } /*remote_port*/
        } /*packet_recording*/
	else if(!strcmp(node,"network_recording"))
        {
            node = acl_strtok_get_n_clear_token(&t);
            if(NULL == node)
                return NULL;
            if(!strcmp(node,"enabled"))
            {
                retval = acl_strtok_get_n_clear_token(&t);
                voip_config_params.voip_debug_information_info.enable_network_recording= atoi(retval);
            } /*enabled*/
        } /*packet_recording*/
	else if(!strcmp(node,"tdm_recording"))
        {
            node = acl_strtok_get_n_clear_token(&t);
            if(NULL == node)
                return NULL;
            if(!strcmp(node,"enabled"))
            {
                retval = acl_strtok_get_n_clear_token(&t);
                voip_config_params.voip_debug_information_info.enable_tdm_recording= atoi(retval);
            } /*enabled*/
        } /*packet_recording*/
	else if(!strcmp(node,"debug_information_channel"))
        {
            node = acl_strtok_get_n_clear_token(&t);
            if(NULL == node)
                return NULL;
            if(!strcmp(node,"channel"))
            {
                retval = acl_strtok_get_n_clear_token(&t);
                voip_config_params.voip_debug_information_info.channel= atoi(retval);
            } /*enabled*/
        } /*packet_recording*/

    }
    /*  retval is initialized with a non null string. if a node is legal but we decided
        not to deal with it then it wont be parsed as a non null node and retval will have a value
        if a node was illegal the parser will return null and therefore retval should be also null*/
    if (NULL == node)   /*one of the nodes was illegal == spaces / tabs*/
        retval = NULL;
    return retval;
}
void* paging_get_config()
{
    return &voip_config_params.paging;
}
/***************************************************************************************/
/*                                                                                     */
/***************************************************************************************/
void voip_conf_uninit(void)
{
    acl_call_mngr_params_t       *voip_call_mngr_info = &(voip_config_params.voip_call_mngr_info);

    /*release allocated memory*/
    voip_phonebook_record_t *curPhoneRecord = voip_config_params.phonebook;
    voip_phonebook_record_t *nextPhoneRecord;

    /*free from the first to the last*/
    while (curPhoneRecord)
    {
        nextPhoneRecord = curPhoneRecord->next;
        free(curPhoneRecord);
        curPhoneRecord = nextPhoneRecord;
    }

    if(voip_call_mngr_info->lines)
	free(voip_call_mngr_info->lines);
}

void voip_conf_init(void)
{
    voip_conf_set_voip_config_defaults();
    voip_conf_set_system_config_defaults();

/*
    voip_conf_print_voip_config_params();
    voip_conf_print_system_config_params();
*/
}

/*******************************************************************************
 * Function:	voip_conf_regional_settings_find_country_in_DB
 * Description:	This function searches acl_conf selected country name in the DB.
 * Argument:	pIndex (OUT) index of selected country in the DB. if country is not found, index of dummy entry
 *				is returned so that a new entry can be added to the DB.
 *				CountryName (IN).
 * Return:		returns TRUE(1) if acl_conf country name is found in the DB, FALSE(0) otherwise.
 * Remarks:
 *
 ******************************************************************************/
//nirp regional
int voip_conf_regional_settings_find_country_in_DB(int* pIndex, char* CountryName)
{
    int i;

    DPRINTK("Enter... looking for %s in the DB.\n", CountryName);

    //go over the DB.....
    for(i=0; strcmp((char *)slic_dsp_general_and_regional_settings_params_array[i].CountryName, "END_OF_LIST"); i++)
    {
        if (!strcmp((char *)slic_dsp_general_and_regional_settings_params_array[i].CountryName, CountryName))
        {//found the country name

            DPRINTK("Found country %s index location %d \n", CountryName, i);
            *pIndex=i;
            iCurrentRGSIndex=*pIndex;
            return TRUE;
        }
    }
    DPRINTK("country %s was not found, returning index location %d \n", CountryName, i-1);
    *pIndex=i-1;
    iCurrentRGSIndex=*pIndex;
    return FALSE;
}

//Nirp advanced channel configuration - read advanced channel configuration from acl_conf
void voip_conf_get_advanced_channel_configuration(voip_dsp_advanced_channel_configuration_t* p_voip_dsp_advanced_channel_configuration)
{
        int index;
	int number_of_lines;

	voip_dsp_advanced_channel_configuration_t* adv_channel_conf = &(voip_config_params.voip_dsp_advanced_channel_configuration);

   	number_of_lines=CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES;

   	//rtcp-xr
	for(index=0; index<number_of_lines; index++)
	{
   		p_voip_dsp_advanced_channel_configuration->RtcpXrEnable[index] = adv_channel_conf->RtcpXrEnable[index];
    		p_voip_dsp_advanced_channel_configuration->VoiceOutputGainChannelArr[index] = adv_channel_conf->VoiceOutputGainChannelArr[index];
	}

	//Gain
	p_voip_dsp_advanced_channel_configuration->AutomaticAgcEnable                       = adv_channel_conf->AutomaticAgcEnable;
    p_voip_dsp_advanced_channel_configuration->AgcLocation                              = adv_channel_conf->AgcLocation;
    p_voip_dsp_advanced_channel_configuration->AgcTargetEnergy                          = adv_channel_conf->AgcTargetEnergy;
	//Jitter
    p_voip_dsp_advanced_channel_configuration->VoiceJitterBufferMinDelay                = adv_channel_conf->VoiceJitterBufferMinDelay;
    p_voip_dsp_advanced_channel_configuration->DynamicJitterBufferOptimizationFactor    = adv_channel_conf->DynamicJitterBufferOptimizationFactor;
    //echo canceller
	p_voip_dsp_advanced_channel_configuration->EchoCancelerMode                         = adv_channel_conf->EchoCancelerMode;
    //fax
    p_voip_dsp_advanced_channel_configuration->bypassCoder                              = adv_channel_conf->bypassCoder;     /* ACL NB 061031 */
    p_voip_dsp_advanced_channel_configuration->faxBypassPayloadType                     = adv_channel_conf->faxBypassPayloadType;     /* ACL NB 061031 */
    p_voip_dsp_advanced_channel_configuration->dataBypassPayloadType                    = adv_channel_conf->dataBypassPayloadType;     /* ACL NB 061031 */
    p_voip_dsp_advanced_channel_configuration->CngRelayEnable                           = adv_channel_conf->CngRelayEnable;
    p_voip_dsp_advanced_channel_configuration->FaxMaxRate                               = adv_channel_conf->FaxMaxRate;
    p_voip_dsp_advanced_channel_configuration->FaxMaxBuffer                               = adv_channel_conf->FaxMaxBuffer;
    p_voip_dsp_advanced_channel_configuration->FaxMaxDatagram                               = adv_channel_conf->FaxMaxDatagram;
    p_voip_dsp_advanced_channel_configuration->ImageDataRedundancy                               = adv_channel_conf->ImageDataRedundancy;
    p_voip_dsp_advanced_channel_configuration->T30ControlDataRedundancy                               = adv_channel_conf->T30ControlDataRedundancy;
    p_voip_dsp_advanced_channel_configuration->FaxModemJit                               = adv_channel_conf->FaxModemJit;

    p_voip_dsp_advanced_channel_configuration->FaxErrorCorrectionMode                   = adv_channel_conf->FaxErrorCorrectionMode;
    //silence compression
    p_voip_dsp_advanced_channel_configuration->SilenceCompressionMode                   = adv_channel_conf->SilenceCompressionMode;
    p_voip_dsp_advanced_channel_configuration->PcmAdPcmComfortNoiseRtpPayloadTypeEnable = adv_channel_conf->PcmAdPcmComfortNoiseRtpPayloadTypeEnable;

    //dynamic payload type
    p_voip_dsp_advanced_channel_configuration->g726_payload_number                      = adv_channel_conf->g726_payload_number;
    p_voip_dsp_advanced_channel_configuration->dtmf_payload_number                      = adv_channel_conf->dtmf_payload_number;

    p_voip_dsp_advanced_channel_configuration->autoSwitchToDtmfInband                      = adv_channel_conf->autoSwitchToDtmfInband;

}


/*******************************************************************************
 * Function:	voip_conf_regional_settings_info_sync
 * Description:	Syncronise the acl_conf data with the DB (if needed).
 * Argument:	void
 * Return:		void
 * Remarks:
 ******************************************************************************/
//nirp regional
void voip_conf_regional_settings_info_sync(void)
{
	regional_settings_params_t *reg_set_params = &(voip_config_params.regionalSettingsInfo.regionalSettingsParams);
	int indexInDB, iteration;

	if(voip_config_params.regionalSettingsInfo.useAppConfConfiguration)
	{
		voip_conf_regional_settings_find_country_in_DB(&indexInDB, (char *)reg_set_params->CountryName);
		DPRINTK("voip_conf_regional_settings_info_sync: using acl_conf configuration (acl_conf_country=%s, index=%d)\n", reg_set_params->CountryName, indexInDB);
		//Now, fill the parameters
		//country name

		strcpy((char *)slic_dsp_general_and_regional_settings_params_array[indexInDB].CountryName, (char *)reg_set_params->CountryName);
		//dsp parameters
		slic_dsp_general_and_regional_settings_params_array[indexInDB].dsp_regional_settings_params.CallerIDStandard =
            reg_set_params->dsp_regional_settings_params.CallerIDStandard;
		for (iteration = 0; iteration < DSP_REGIONAL_SETTINGS_CALL_PROGRESS_TONE_LAST; iteration++)
		{
			DPRINTK("iteration =%d\n", iteration);
			slic_dsp_general_and_regional_settings_params_array[indexInDB].dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[iteration].enum_dsp_regional_settings_params_call_progress=iteration;
			slic_dsp_general_and_regional_settings_params_array[indexInDB].dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[iteration].EnableField =
                reg_set_params->dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[iteration].EnableField;
			slic_dsp_general_and_regional_settings_params_array[indexInDB].dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[iteration].ToneCadenceType =
                reg_set_params->dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[iteration].ToneCadenceType;
			slic_dsp_general_and_regional_settings_params_array[indexInDB].dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[iteration].FreqA =
                reg_set_params->dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[iteration].FreqA;
			slic_dsp_general_and_regional_settings_params_array[indexInDB].dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[iteration].FreqB =
                reg_set_params->dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[iteration].FreqB;
			slic_dsp_general_and_regional_settings_params_array[indexInDB].dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[iteration].FreqLevelA =
                reg_set_params->dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[iteration].FreqLevelA;
			slic_dsp_general_and_regional_settings_params_array[indexInDB].dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[iteration].FreqLevelB =
                reg_set_params->dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[iteration].FreqLevelB;
			slic_dsp_general_and_regional_settings_params_array[indexInDB].dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[iteration].ToneOn0 =
                reg_set_params->dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[iteration].ToneOn0;
			slic_dsp_general_and_regional_settings_params_array[indexInDB].dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[iteration].ToneOff0 =
                reg_set_params->dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[iteration].ToneOff0;
			slic_dsp_general_and_regional_settings_params_array[indexInDB].dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[iteration].ToneOn1 =
                reg_set_params->dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[iteration].ToneOn1;
			slic_dsp_general_and_regional_settings_params_array[indexInDB].dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[iteration].ToneOff1 =
                reg_set_params->dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[iteration].ToneOff1;
			slic_dsp_general_and_regional_settings_params_array[indexInDB].dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[iteration].ToneOn2 =
                reg_set_params->dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[iteration].ToneOn2;
			slic_dsp_general_and_regional_settings_params_array[indexInDB].dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[iteration].ToneOff2 =
                reg_set_params->dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[iteration].ToneOff2;
			slic_dsp_general_and_regional_settings_params_array[indexInDB].dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[iteration].ToneOn3 =
                reg_set_params->dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[iteration].ToneOn3;
			slic_dsp_general_and_regional_settings_params_array[indexInDB].dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[iteration].ToneOff3 =
                reg_set_params->dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[iteration].ToneOff3;
		}

		//FXS parameters
		memcpy( (char *)slic_dsp_general_and_regional_settings_params_array[indexInDB].slic_regional_settings_params.FXS_COEFFICIENTS,
                (char *)reg_set_params->slic_regional_settings_params.FXS_COEFFICIENTS, REGIONAL_SETTINGS_FXS_COEFFICIENTS_PARAMS_LEN);
		memcpy( (char *)slic_dsp_general_and_regional_settings_params_array[indexInDB].slic_regional_settings_params.FXS_RING_CAD_PROFILE,
                (char *)reg_set_params->slic_regional_settings_params.FXS_RING_CAD_PROFILE, REGIONAL_SETTINGS_FXS_CADENCE_PARAMS_LEN);
/*
		strcpy( (char *)slic_dsp_general_and_regional_settings_params_array[indexInDB].slic_regional_settings_params.FXS_COEFFICIENTS,
                (char *)reg_set_params->slic_regional_settings_params.FXS_COEFFICIENTS);
		strcpy( (char *)slic_dsp_general_and_regional_settings_params_array[indexInDB].slic_regional_settings_params.FXS_RING_CAD_PROFILE,
                (char *)reg_set_params->slic_regional_settings_params.FXS_RING_CAD_PROFILE);
*/

	}
	//now, we can check which coutry is selected by the user and send this information to the DSP
	voip_conf_regional_settings_find_country_in_DB(&indexInDB, (char *)voip_config_params.regionalSettingsInfo.selectedCountry);

//	slic_dsp_general_and_regional_settings_params_array[indexInDB].slic_calibration_resultsArray[0].iPerformFXSCalibration=set_get_path_flag(rgs_set, Scalibration_enable);
	DPRINTK("voip_conf_regional_settings_info_sync: (selected country=%s, index=%d)\n", voip_config_params.regionalSettingsInfo.selectedCountry, indexInDB);
}

#if 0 /*NirM, need to implement for FXO support */
/* NIRP 080304 PSTN line access code */
/* This function returns the PSTN line ID */
static char* voip_conf_get_pstn_line_id(void)
{
	set_t **entry;
	int index=0;

	entry=set_get_son(set_get(rg_conf, Svoip "/" Sline), 0);
	if (entry)
	{
		do {
			if (NULL == entry)//reached end of list....
				break;

			if (!strcmp(PSTN_LINE_DESCRIPTION, set_get_path_strz(entry, Sdescription)))
			{
				DPRINTK("found a match, line %d .\n", index);
				return set_get_path_strz(entry, Sid);
			}

			index++;
			entry=set_get_next(entry);
		} while(entry);

	}

	console_printf("[%s:%d] No Match .\n", __FUNCTION__, __LINE__);

	return NULL;
}

/* NIRP 080304 PSTN line access code */
/* This function find a match between the pstn access code and a dial string */
static voip_dial_match_t is_dial_string_match_pstn_access_code(char *dial_string)
{
	char* pstn_access_code = set_get_path_strz(rg_conf, Svoip "/" Spstn_line_access_code);
	int dial_string_len=strlen(dial_string);
	int pstn_access_code_len=strlen(pstn_access_code);

	/* If we don't have any FXO ACL NB 080806 */
	if (voip_conf_get_number_of_lines(ANALOG_PORT_TYPE_FXO) == 0)
		return MATCH_DIAL_NOMATCH;
	if (dial_string_len < pstn_access_code_len)
	//look for a partial match
	{
		if (strncmp(dial_string, pstn_access_code, dial_string_len))
			return MATCH_DIAL_PARTIALMATCH;
		else
			return MATCH_DIAL_NOMATCH;
	}
	else if (dial_string_len == pstn_access_code_len)
	{
		//try to find a ful match
		if (strcmp(dial_string, pstn_access_code))
			return MATCH_DIAL_NOMATCH;
		else
			return MATCH_DIAL_UNAMBIGUOUSMATCH;
	}
	else
		return MATCH_DIAL_NOMATCH;
}
#endif

/* Returns 1 if the speed-dial entry is to be used according to the dial
 * string.
 */
/* This is called inside of protected code */
static int is_dial_string_match_entry(voip_phonebook_record_t *record, char *dial_string, int prefix)
{
    int                 use_proxy;
    voip_protocols_t    signalling_type;
    char                *entry_number = record->dialString;

    signalling_type = voip_config_params.voip_call_mngr_info.protocol;
    use_proxy = (signalling_type == VOIP_PROT_SIP && voip_config_params.voip_call_mngr_info.sp.sip.use_proxy) ||
                (signalling_type == VOIP_PROT_H323 && voip_config_params.voip_call_mngr_info.sp.h323.gk_registry);

    /* If dial string doesn't match the speed-dial number, return no match */
    if (strcmp(entry_number, dial_string))
    {
        if (!prefix || memcmp(entry_number, dial_string, strlen(dial_string)))
            return 0;
    }

    /* If speed dial entry has a proxy destination and we aren't using proxy,
    * return no match */
    if (record->destinationType == VOIP_DESTINATION_TYPE_PROXY && !use_proxy)
    {
        return 0;
    }

    return 1;
}

/* This is called inside of protected code */
static voip_phonebook_record_t *phonebook_entry_get(char *dial_string)
{
    voip_phonebook_record_t *record = voip_config_params.phonebook;

    for (; record; record = record->next)
    {
        if (is_dial_string_match_entry(record, dial_string, 0))
            break;
    }

    return record;
}

static voip_phonebook_record_t *phonebook_entry_prefix(char *dial_string, voip_phonebook_record_t *skip)
{
    voip_phonebook_record_t *record = voip_config_params.phonebook;

    for (; record; record = record->next)
    {
        if (skip && memcmp(record, skip, sizeof(voip_phonebook_record_t)))
            continue;
        if (is_dial_string_match_entry(record, dial_string, 1))
            break;
    }

    return record;
}


/* translate dial string accepted, as the '*' and '#' keys are translated to 'e'
 * and 'f', respectively, by the MDM - we need to reverse this translation */
static void convert_dtmf(char *dialed)
{
    /* find the next appearance of E or F (we may reach the string's end) */
    while (*(dialed += strcspn(dialed, "efEF")))
    {
// ACL NB 070604 	*dialed = strcspn(dialed, "eE") ? 0 : '*';
 	*dialed = strcspn(dialed, "eE") ? '#' : '*';
	dialed++; /* continue search after the current match */
    }
}

/* match dialed number
 * return: matched, dial timeout
 */
voip_dial_match_t voip_conf_match_number(char *dialstr,
    long *timer_duration, int is_registered_with_proxy)
{
    voip_phonebook_record_t *record;
    voip_dial_match_t match = MATCH_DIAL_PARTIALMATCH;
    int force_match = 0;
    char digitMap[MAX_DIGIT_MAP_LEN];
    int len; // ACL NB 070604
    char dial_force_key[1];

#if 0 /*NirM, need to implement for FXO support */
	/* NIRP 080304 PSTN line access code */
	ret = is_dial_string_match_pstn_access_code(dialstr);
	if (MATCH_DIAL_UNAMBIGUOUSMATCH == ret)
	{
		voip_conf_unlock();
		return ret;
	}
#endif

    if ( voip_config_params.dialingInfo.dialCompleteKeyEnabled == 1)
    {
        len = strlen(dialstr); // ACL NB 070604 - VI53849
        dial_force_key[0] = voip_config_params.dialingInfo.dialCompleteKey;
        if (dial_force_key[0] == '*')
            dial_force_key[0] = 'e';
        if (dial_force_key[0] == '#')
            dial_force_key[0] = 'f';

        if ((len > 1)  && ( dialstr[len - 1] == dial_force_key[0]) ) /* default = pound */
        {
            dialstr[len - 1] = 0;
            force_match = 1;
        }
    }

    if (strlen(dialstr) >= (size_t)voip_config_params.dialingInfo.dialStrMaxSize)
    {
        force_match = 1;
    }

    /* by itzik - 19072006 - compare the dial string with the digit map */
    if(!force_match)
    {
	AC_STRNCPY(digitMap, voip_config_params.dialingInfo.digitMap, sizeof(digitMap) - 1);
		if (digitmap_type == DIGIT_MAP_OLD_STYLE)
	        force_match=compareDialStr2digitMap(digitMap,dialstr);
		else if (digitmap_type == DIGIT_MAP_MGCP_LIKE_STYLE)
			force_match=digitMapMatch(dialstr);
    }

    record = phonebook_entry_get(dialstr);

    if (record)
    {
        match = MATCH_DIAL_UNAMBIGUOUSMATCH;
        if (!force_match && (is_registered_with_proxy ||
            phonebook_entry_prefix(dialstr, record)))
        {
            match = MATCH_DIAL_MATCH;
        }
    }
    else if (force_match)
        match = MATCH_DIAL_NOMATCH;

    *timer_duration = voip_config_params.dialingInfo.dialTimeout;

    DPRINTK_ERR("dialstring %s - match: %d\n", dialstr, match);

    return match;
}

/* map dialed number
 * return two strings
 * first - user ID
 * second - user address
 * if dialed number matches no entry - empty strings are returned
 */
void voip_conf_map_number(char *dial_string, char **user_id, char **user_addr)
{
    voip_phonebook_record_t *record;
    voip_protocols_t        signalling_type;
    char                    *addr = NULL;
    int                     sip_port;
    if (!dial_string)
	    return;

#if 0 /*NirM, need to implement for FXO support */
   /* NIRP 080304 PSTN line access code */
   if (MATCH_DIAL_UNAMBIGUOUSMATCH == is_dial_string_match_pstn_access_code(dial_string))
   {
   	signalling_type = set_get_path_enum(rg_conf, Svoip "/" Ssignalling "/" Sprotocol, voip_protocols_t_str);
	if (VOIP_PROT_SIP == signalling_type)
	{
		sip_port = set_get_path_int(rg_conf, Svoip "/" Ssignalling "/" Ssip "/" Sport);
	}
	else
	{
		sip_port = 0;
	}

	*user_id = strdup_e(voip_conf_get_pstn_line_id());
	if (sip_port)
	{
		int user_addr_size = snprintf(NULL, 0, "%s:%d", local_ip, sip_port) + 1;
		*user_addr = zalloc_e(user_addr_size);
		snprintf(*user_addr, user_addr_size, "%s:%d", local_ip, sip_port);
	}
	else
		*user_addr = strdup_e(local_ip);

	goto Exit;
}
#endif

    if (!(record = phonebook_entry_get(dial_string)))
    {
        /* No phonebook entry found, return "" as user id and addr */
        *user_id = strdup_e("");
        *user_addr = strdup_e("");
        return;
    }

    switch (record->destinationType)
    {
    case VOIP_DESTINATION_TYPE_LOCAL:
        signalling_type = voip_config_params.voip_call_mngr_info.protocol;
        if (VOIP_PROT_SIP == signalling_type)
            sip_port = voip_config_params.voip_call_mngr_info.sp.sip.sip_port;
        else
            sip_port = 0;
        *user_id = strdup_e(voip_config_params.voip_call_mngr_info.lines[record->localLine].id);
        if (sip_port)
        {
            int user_addr_size = snprintf(NULL, 0, "%s:%d", system_config_params.local_ip, sip_port) + 1;

            *user_addr = AC_ZALLOC_E(user_addr_size);
            snprintf(*user_addr, user_addr_size, "%s:%d", system_config_params.local_ip, sip_port);
        }
        else
            *user_addr = strdup_e(system_config_params.local_ip);
        break;
    case VOIP_DESTINATION_TYPE_DIRECT:

        *user_id = strdup_e(record->userId);
        //*user_addr = strdup_e(set_get_path_strz(record, Suser_addr));
        // by itzik - 20 June 2006
        AC_STRNCPY(user_addr_temp, record->userAddress, sizeof(user_addr_temp) - 1);

        sip_port = record->userPort;

        int user_addr_size = snprintf(NULL, 0, "%s:%d", user_addr_temp, sip_port) + 1;

        *user_addr = AC_ZALLOC_E(user_addr_size);
        snprintf(*user_addr, user_addr_size, "%s:%d", user_addr_temp, sip_port);
        break;
    case VOIP_DESTINATION_TYPE_PROXY:
        signalling_type = voip_config_params.voip_call_mngr_info.protocol;
        if (VOIP_PROT_SIP == signalling_type)
        {
			int is_proxy_redundant = voip_conf_ext_is_redundant_proxy_get();
			if (is_proxy_redundant)
				addr = strdup_e(voip_config_params.voip_call_mngr_info.sp.sip.redundant_proxy_address);
			else
            	addr = strdup_e(voip_config_params.voip_call_mngr_info.sp.sip.proxy_address);
            // by itzik
            AC_STRNCPY(user_addr_temp, addr ,sizeof(user_addr_temp) - 1);

 	    free(addr);

            sip_port = voip_config_params.voip_call_mngr_info.sp.sip.proxy_port;

            int user_addr_size = snprintf(NULL, 0, "%s:%d", user_addr_temp, sip_port) + 1;

            *user_addr = AC_ZALLOC_E(user_addr_size);
            snprintf(*user_addr, user_addr_size, "%s:%d", user_addr_temp, sip_port);
        }
        else if (VOIP_PROT_H323 == signalling_type)
        {
            addr = voip_config_params.voip_call_mngr_info.sp.h323.gk_address;
        }
        else /* If we got here, acl_conf is corrupt. This sould never happen. */
        {
            DPRINTK_ERR("Bad value for signaling protocol");
        }

        *user_id = strdup_e(record->userId);
        // by itzik
        // *user_addr = strdup_e(addr);

        break;
    default:
        DPRINTK_ERR("Corrupted entry in phone-book: dial string \"%s\" is of unknown type", dial_string);
    }
}

int voip_conf_local_address_get(char *ip_address)
{
    if (NULL == ip_address)
    {
        printf("[%s:%d] ip_address is NULL\n", __FUNCTION__, __LINE__);
        return (-1);
    }
    strcpy(ip_address, system_config_params.local_ip);

    return 0;
}

void voip_conf_localtime_get(struct tm *tm_time)//NIRP 070111
{
    time_t tm = utc_time(NULL); /* UTC time */

    tm += system_config_params.timeZoneOffset * 60;

    memcpy(tm_time, gmtime(&tm), sizeof(*tm_time));
}

/* return configuration parameters for requested line
 * return:
 * enabled
 * id
 * send_caller_id
 */
void voip_conf_line_conf_get(int line_number, line_conf_t *conf)
{
    if (NULL == conf)
    {
        printf("[%s:%d] conf is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
    conf->enabled = voip_config_params.voip_call_mngr_info.lines[line_number].enabled;
    AC_STRNCPY(conf->id, voip_config_params.voip_call_mngr_info.lines[line_number].id,
               sizeof(conf->id) - 1);
    conf->send_caller_id = voip_config_params.voip_call_mngr_info.lines[line_number].send_caller_id;
    AC_STRNCPY(conf->display_name, voip_config_params.voip_call_mngr_info.lines[line_number].display_name,
	           sizeof(conf->display_name) - 1);
}

/* return parameters for SIP
 * used UDP port
 * flag 'use proxy'
 * proxy address
 */
/* This is called inside of protected code */
static void voip_conf_sip_get(acl_call_mngr_params_t *params)
{
    int cnt,num_of_fxs_lines;
    if (NULL == params)
    {
        printf("[%s:%d] params is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
    params->sp.sip.sip_port = voip_config_params.voip_call_mngr_info.sp.sip.sip_port;
    AC_STRNCPY(params->sp.sip.user_doamin, voip_config_params.voip_call_mngr_info.sp.sip.user_doamin,
               sizeof(params->sp.sip.user_doamin) - 1);

	// by itzik - 19072006 - digitmap
    AC_STRNCPY(params->sp.sip.numberRules, voip_config_params.voip_call_mngr_info.sp.sip.numberRules,
               sizeof(params->sp.sip.numberRules) - 1);

    params->sp.sip.use_proxy_port_ip_for_registrar = voip_config_params.voip_call_mngr_info.sp.sip.use_proxy_port_ip_for_registrar;

    params->sp.sip.use_outbound_proxy = voip_config_params.voip_call_mngr_info.sp.sip.use_outbound_proxy;
    params->sp.sip.outbound_proxy_port = voip_config_params.voip_call_mngr_info.sp.sip.outbound_proxy_port;
    AC_STRNCPY(params->sp.sip.outbound_proxy_address, voip_config_params.voip_call_mngr_info.sp.sip.outbound_proxy_address,
    sizeof(params->sp.sip.outbound_proxy_address) - 1);

    params->sp.sip.use_registrar = voip_config_params.voip_call_mngr_info.sp.sip.use_registrar;
    params->sp.sip.registrar_port = voip_config_params.voip_call_mngr_info.sp.sip.registrar_port;
    AC_STRNCPY(params->sp.sip.registrar_address, voip_config_params.voip_call_mngr_info.sp.sip.registrar_address,
               sizeof(params->sp.sip.registrar_address) - 1);

	/* AlexChen 20090402 SDK upgrade */
	params->sp.sip.use_redundant_proxy = voip_config_params.voip_call_mngr_info.sp.sip.use_redundant_proxy;
    params->sp.sip.redundant_proxy_port = voip_config_params.voip_call_mngr_info.sp.sip.redundant_proxy_port;
    AC_STRNCPY(params->sp.sip.redundant_proxy_address, voip_config_params.voip_call_mngr_info.sp.sip.redundant_proxy_address,
		sizeof(params->sp.sip.redundant_proxy_address) - 1);
    params->sp.sip.redundant_proxy_ka_period = voip_config_params.voip_call_mngr_info.sp.sip.redundant_proxy_ka_period;
    params->sp.sip.redundant_proxy_is_symmetric = voip_config_params.voip_call_mngr_info.sp.sip.redundant_proxy_is_symmetric;
	params->sp.sip.is_redundant_proxy = voip_config_params.voip_call_mngr_info.sp.sip.is_redundant_proxy;


	params->sp.sip.retransmissionT1 = voip_config_params.voip_call_mngr_info.sp.sip.retransmissionT1;
	params->sp.sip.retransmissionT2 = voip_config_params.voip_call_mngr_info.sp.sip.retransmissionT2;
	params->sp.sip.retransmissionT4 = voip_config_params.voip_call_mngr_info.sp.sip.retransmissionT4;
	params->sp.sip.inviteLingerTimer = voip_config_params.voip_call_mngr_info.sp.sip.inviteLingerTimer;

    params->sp.sip.use_proxy = voip_config_params.voip_call_mngr_info.sp.sip.use_proxy;
    params->sp.sip.proxy_registration_timeout = voip_config_params.voip_call_mngr_info.sp.sip.proxy_registration_timeout;
	/*ACL JL 59875 limits number of registrations to proxy to  a configurable number RV patch*/
	params->sp.sip.auth_retries = voip_config_params.voip_call_mngr_info.sp.sip.auth_retries;


    AC_STRNCPY(params->sp.sip.proxy_address, voip_config_params.voip_call_mngr_info.sp.sip.proxy_address,
               sizeof(params->sp.sip.proxy_address) - 1);
    AC_STRNCPY(params->sp.sip.proxy_user_name, voip_config_params.voip_call_mngr_info.sp.sip.proxy_user_name,
               sizeof(params->sp.sip.proxy_user_name) - 1);
    AC_STRNCPY(params->sp.sip.proxy_password, voip_config_params.voip_call_mngr_info.sp.sip.proxy_password,
               sizeof(params->sp.sip.proxy_password) - 1);
    /*unobscure_str(params->sp.sip.proxy_password);*/
    params->sp.sip.proxy_port = voip_config_params.voip_call_mngr_info.sp.sip.proxy_port; /* ACL NB 060604 proxy_port*/

    num_of_fxs_lines=voip_conf_get_number_of_lines(ANALOG_PORT_TYPE_FXS);
    for (cnt = 0; cnt < num_of_fxs_lines;cnt++)
    {
        AC_STRNCPY(params->lines[cnt].usernameServerAuth, voip_config_params.voip_call_mngr_info.lines[cnt].usernameServerAuth,
            sizeof(params->lines[cnt].usernameServerAuth) - 1);
        AC_STRNCPY(params->lines[cnt].pswdServerAuth, voip_config_params.voip_call_mngr_info.lines[cnt].pswdServerAuth,
            sizeof(params->lines[cnt].pswdServerAuth) - 1);
        /*unobscure_str(params->lines[cnt].pswdServerAuth);*/
    }

    params->sp.sip.uregistration_timeout = voip_config_params.voip_call_mngr_info.sp.sip.uregistration_timeout;
    params->sp.sip.watchdog_timeout      = voip_config_params.voip_call_mngr_info.sp.sip.watchdog_timeout;
    /* ACL NB 070404 */
    params->sp.sip.callWaitingEnabled = voip_config_params.voip_call_mngr_info.sp.sip.callWaitingEnabled;
    /* ACL NB 061023 */
    params->sp.sip.callWaitingReply = voip_config_params.voip_call_mngr_info.sp.sip.callWaitingReply;
	/* AlexChen 20090408 SDK upgrade caller id type2 */
	params->sp.sip.callWaitingType2Enabled = voip_config_params.voip_call_mngr_info.sp.sip.callWaitingType2Enabled;

    params->sp.sip.sip_transport_protocol = voip_config_params.voip_call_mngr_info.sp.sip.sip_transport_protocol;
    /*EA - 071115 - connectMediaOn180*/
    params->sp.sip.connectMediaOn180 = voip_config_params.voip_call_mngr_info.sp.sip.connectMediaOn180;

/*EA - 071218 - VI#59630 support SIP over TLS */
#ifdef RV_CFLAG_TLS
    params->sp.sip.transportTlsCfg.stackTlsPort = voip_config_params.voip_call_mngr_info.sp.sip.transportTlsCfg.stackTlsPort;
#else /* RV_CFLAG_TLS */
    DPRINTK("TLS is not supported\r\n");
#endif /* RV_CFLAG_TLS */

	/*ACL NB 080625 */
    /* Note it is combination of 2 parameters */
    if ( !voip_config_params.voip_dsp_advanced_channel_configuration.CngRelayEnable)
      params->sp.sip.remote_fax_side_reinvite = voip_config_params.voip_call_mngr_info.sp.sip.remote_fax_side_reinvite;
    else
      params->sp.sip.remote_fax_side_reinvite = 0;
}

/* return parameters for H323
 * flag 'gatekeeper_registry'
 * signalling port
 * gatekeeper port
 * gatekeeper address
 * flag fast_start_enable
 */
/* This is called inside of protected code */
static void voip_conf_h323_get(acl_call_mngr_params_t *params)
{
    if (NULL == params)
    {
        printf("[%s:%d] params is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
    AC_STRNCPY(params->sp.h323.gk_address,
	           voip_config_params.voip_call_mngr_info.sp.h323.gk_address,
	           sizeof(params->sp.h323.gk_address) - 1);

    params->sp.h323.gk_registry = voip_config_params.voip_call_mngr_info.sp.h323.gk_registry;
    params->sp.h323.signal_port = voip_config_params.voip_call_mngr_info.sp.h323.signal_port;
    params->sp.h323.gk_port = voip_config_params.voip_call_mngr_info.sp.h323.gk_port;
    params->sp.h323.fast_start = voip_config_params.voip_call_mngr_info.sp.h323.fast_start;
    params->sp.h323.e164_alias_list = voip_config_params.voip_call_mngr_info.sp.h323.e164_alias_list;
}

/* return parameters for MGCP
 * mgc address
 * mgc port
 * mg port
 */
/* This is called inside of protected code */
static void voip_conf_mgcp_get(acl_call_mngr_params_t *params)
{
    if (NULL == params)
    {
        printf("[%s:%d] params is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
	/* AlexChen SDK upgrade 20090409 */
	AC_STRNCPY(params->sp.mgcp.user_domain,
               voip_config_params.voip_call_mngr_info.sp.mgcp.user_domain,
	           sizeof(params->sp.mgcp.user_domain) - 1);

	AC_STRNCPY(params->sp.mgcp.mgc_address,
               voip_config_params.voip_call_mngr_info.sp.mgcp.mgc_address,
	           sizeof(params->sp.mgcp.mgc_address) - 1);

    params->sp.mgcp.mgc_port = voip_config_params.voip_call_mngr_info.sp.mgcp.mgc_port;
    params->sp.mgcp.mg_port = voip_config_params.voip_call_mngr_info.sp.mgcp.mg_port;
}

/* return common parameters for every signalling protocols
 * media_port
 * protocol
 * local host port start
 * local host ports range
 * out of band dtmf flag
 */
void voip_conf_signalling_conf_get(acl_call_mngr_params_t *params)
{
    if (NULL == params)
    {
        printf("[%s:%d] params is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
    params->media_port = voip_config_params.voip_call_mngr_info.media_port;

	params->media_tos = voip_config_params.voip_call_mngr_info.media_tos;

    params->protocol = voip_config_params.voip_call_mngr_info.protocol;
    params->local_host_port = voip_config_params.voip_call_mngr_info.local_host_port;
    params->local_host_port_range = voip_config_params.voip_call_mngr_info.local_host_port_range;
    params->dtmf_transport_method = voip_config_params.voip_call_mngr_info.dtmf_transport_method; /* ACL NB 060723 */
    params->keys_sequence = voip_config_params.voip_call_mngr_info.keys_sequence;        /* ACL NB 060913 */

    /* Fill specific for signalling protocol parameters */
    switch (params->protocol)
    {
    case VOIP_PROT_SIP:
	voip_conf_sip_get(params);
	break;

    case VOIP_PROT_H323:
	voip_conf_h323_get(params);
	break;

    case VOIP_PROT_MGCP:
	voip_conf_mgcp_get(params);
	break;
    }
}

/* return configuration parameters for requested codec
 * return:
 * payload type
 * enabled
 * ptime
 * */
void voip_conf_codec_info_get(int codec_number, codec_t *codec)
{
    if (NULL == codec)
    {
        printf("[%s:%d] codec is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
    if (strcmp(voip_config_params.codecs_info[codec_number].name, "") != 0)
    {
        codec->payload = voip_config_params.codecs_info[codec_number].codec.payload;
	if (codec->payload >= FIRST_DYNAMIC_PAYLOAD)
		codec->payload = voip_config_params.voip_dsp_advanced_channel_configuration.g726_payload_number;
        codec->enabled = voip_config_params.codecs_info[codec_number].codec.enabled;
        codec->ptime = voip_config_params.codecs_info[codec_number].codec.ptime;
        codec->rate = voip_config_params.codecs_info[codec_number].codec.rate;
	codec->g722_bit_rate = voip_config_params.codecs_info[codec_number].codec.g722_bit_rate;
    }
}

//NirM,  configure payload type - update payload type parameters
void voip_conf_dynamic_payloads_info_get(void)
{
//    acl_update_code2str_code(rtp_payload_type_t_str, "telephone-event", set_get_path_int(voip_set, Srtp_dtmf_payload));
    payload_type_enum_to_id_update(RTP_PAYLOAD_DTMF, voip_config_params.voip_dsp_advanced_channel_configuration.dtmf_payload_number);
    payload_type_enum_to_id_update(RTP_PAYLOAD_G726_16, voip_config_params.voip_dsp_advanced_channel_configuration.g726_payload_number);
}

void voip_conf_dsp_params_get(regional_settings_params_t                **regionalSettings,
                              voip_dsp_advanced_channel_configuration_t *advancedChannelConfiguration)
{
    if (NULL == regionalSettings)
    {
        printf("[%s:%d] regionalSettings is NULL\n", __FUNCTION__, __LINE__);
        return;
    }

	voip_conf_regional_settings_info_sync();

	*regionalSettings = (void*)&slic_dsp_general_and_regional_settings_params_array[iCurrentRGSIndex];//nirp regional

	voip_conf_get_advanced_channel_configuration(advancedChannelConfiguration);


}

// ACL NB 060424
/*******************************************************************************
 * Function:	   voip_conf_faxMethod_get
 * Description:	This function get the faxMethod
 * Argument:	   pFaxMethod (OUT) - Disable, FaxRelay or Bypass
 * Return:
 * Remarks:
 *
 ******************************************************************************/
void voip_conf_faxDataMethod_get(int *pFaxDataMethod)
{
    if (NULL == pFaxDataMethod)
    {
        printf("[%s:%d] pFaxDataMethod is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
    *pFaxDataMethod =  voip_config_params.faxTransportMode;
    *pFaxDataMethod |= voip_config_params.dataTransportMode;
}

/* ACL NIRP 061224 */
/*******************************************************************************
 * Function:	   voip_conf_g723_high_bitrate_get
 * Description:	    This function gets the 723 bitrate (low - 5.3k or high 6.3k)
 * Argument:	   pG723_bitrate (OUT) - low (0) or high (1)
 * Return:
 * Remarks:
 *
 ******************************************************************************/
void voip_conf_g723_high_bitrate_get(int *pG723_bitrate)
{
    int i;
    if (NULL == pG723_bitrate)
    {
        printf("[%s:%d] pG723_bitrate is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
    for (i = 0; i < NUMBER_OF_CODECS; i++)
    {
        if (strstr(voip_config_params.codecs_info[i].name, "723"))
        {
            if (voip_config_params.codecs_info[i].codec.enabled)
            {
                *pG723_bitrate = voip_config_params.codecs_info[i].bitRateHi;
                break;
            }
        }
    }
}


// Nirp 060719
/*******************************************************************************
 * Function:	    voip_conf_bypassCoder_get
 * Description:	    This function gets the configured bypass mode Coder (PCMU/A)
 * Argument:	    pBypassCoder (OUT) - RTP_PAYLOAD_PCMU or RTP_PAYLOAD_PCMA
 * Return:
 * Remarks:
 *
 ******************************************************************************/
void voip_conf_bypassCoder_get(int *pBypassCoder)
{
    if (NULL == pBypassCoder)
    {
        printf("[%s:%d] pBypassCoder is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
    *pBypassCoder = voip_config_params.voip_dsp_advanced_channel_configuration.bypassCoder;
}

// ACL NIRP 070401
/*******************************************************************************
 * Function:	   voip_conf_out_of_service_tone_get
 * Description:	This function returns the tone to be played during out of service
 * Arguments:
 * Return:
 * Remarks:
 *
 ******************************************************************************/
void voip_conf_out_of_service_tone_get(int *out_of_service_tone)
{
    if (NULL == out_of_service_tone)
    {
        printf("[%s:%d] out_of_service_tone is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
    *out_of_service_tone = voip_config_params.outOfServiceTone;
}

// ACL NB 061205
/*******************************************************************************
 * Function:	   voip_conf_mwi_get
 * Description:	This function gets the configuration of thr MWI feature
 * Arguments:
 * Return:
 * Remarks:
 *
 ******************************************************************************/
void voip_conf_mwi_get(voip_mwi_params_t *params)
{
    if (NULL == params)
    {
        printf("[%s:%d] params is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
    params->isEnabled      = voip_config_params.voip_mwi_info.isEnabled;
    params->subsServerPort = voip_config_params.voip_mwi_info.subsServerPort;
    params->subscribe2MWI  = voip_config_params.voip_mwi_info.subscribe2MWI;
    params->expiraitionTimeout  = voip_config_params.voip_mwi_info.expiraitionTimeout;
    strcpy(params->subsServerName, voip_config_params.voip_mwi_info.subsServerName);
}

// ACL NB 061205
/*******************************************************************************
 * Function:	   voip_conf_stutter_tone_dur_get
 * Description:	This function gets the stutter tone duration
 * Arguments:
 * Return:
 * Remarks:
 *
 ******************************************************************************/
void voip_conf_stutter_tone_dur_get(int *duration)
{
    if (NULL == duration)
    {
        printf("[%s:%d] duration is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
    *duration = voip_config_params.voip_timeouts_info.stutterToneDuration;
}

/*******************************************************************************
 *AlexChen 20090408 SDK upgrade add the option disable Caller ID type II*
 * Function:	   voip_conf_call_id_type2_enable_get
******************************************************************************/
void  voip_conf_call_id_type2_enable_get(char *pIsEnable)
{
   *pIsEnable = voip_config_params.voip_call_mngr_info.sp.sip.callWaitingType2Enabled;
}

// ACL NB 061210
/*******************************************************************************
 * Function:	   voip_conf_cfw_type_get
 * Description:	This function gets the configuration of thr CFW feature
 * Arguments:
 * Return:
 * Remarks:
 *
 ******************************************************************************/
void voip_conf_cfw_type_get(voip_cfw_params_t *params)
{
    if (NULL == params)
    {
        printf("[%s:%d] params is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
    params->isEnabled   = voip_config_params.voip_cfw_info.isEnabled;
    params->cfnrTimeout = voip_config_params.voip_cfw_info.cfnrTimeout;
    params->cfwType     = voip_config_params.voip_cfw_info.cfwType;
    strcpy(params->keySequence, voip_config_params.voip_cfw_info.keySequence);
}
// ACL NB 070103
/*******************************************************************************
 * Function:	   voip_conf_cfw_type_get
 * Description:	This function gets the user defined key Sequence
 * Arguments:
 * Return:
 * Remarks:
 *
 ******************************************************************************/
void voip_conf_cfw_keySequence_get(char *keySequence)
{
    if (NULL == keySequence)
    {
        printf("[%s:%d] keySequence is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
    if (voip_config_params.voip_cfw_info.isEnabled != 0)
        strcpy(keySequence, voip_config_params.voip_cfw_info.keySequence);
    else
        keySequence[0]=0;
}

/* AlexChen SDK upgrade 04072009 DND features */
/*******************************************************************************
 * Function:	   voip_conf_dnd_keySequence_get
 * Description:	This function gets the user defined key Sequence
 * Arguments:
 * Return:
 * Remarks:
 *
 ******************************************************************************/
void voip_conf_dnd_keySequence_get(char *keySequence)
{
	if (NULL == keySequence)
	{
        printf("[%s:%d] keySequence is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
	if (voip_config_params.voip_dnd_info.isEnabled)
        strcpy(keySequence, voip_config_params.voip_dnd_info.keySequence);
    else
        keySequence[0]=0;
}

/*******************************************************************************
 * Function:	   voip_conf_dnd_enable_get
 ******************************************************************************/
int  voip_conf_dnd_enable_get(char *pIsEnable)
{
	return voip_config_params.voip_dnd_info.isEnabled;
}

// Nirp 060720
/*******************************************************************************
 * Function:	    voip_conf_dialtone_timeout_get
 * Description:	    This function gets the configured dial tone timeout
 * Argument:
 * Return:		    dial tone timeout which is configured in acl_conf (in MS)
 * Remarks:
 *
 ******************************************************************************/
int voip_conf_dialtone_timeout_get(void)
{
    int DialToneTimeout;

    DialToneTimeout = voip_config_params.voip_timeouts_info.dialToneTimeout;
    return DialToneTimeout*1000;
}


/*******************************************************************************
 * Function:	    voip_conf_tone_timeout_get
 * Description:	    This function gets the configured tone timeout
 * Argument:
 * Return:		    tone timeout which is configured in acl_conf (in MS)
 * Remarks:
 *
 ******************************************************************************/
int voip_conf_tone_timeout_get(int tone)
{
    int toneTimeout = 0;

    if (tone == VOIP_SIGNAL_REORDER_TONE )
        toneTimeout = voip_config_params.voip_timeouts_info.warningToneTimeout;
    else if (tone == VOIP_SIGNAL_HOOK_OFF_TONE )
        toneTimeout = voip_config_params.voip_timeouts_info.offhookToneTimeout;
    else if (tone == VOIP_SIGNAL_DIAL_TONE )
        toneTimeout = voip_config_params.voip_timeouts_info.dialToneTimeout;

    return toneTimeout*1000;
}

/*******************************************************************************
 * Function:	    voip_conf_unansweredCall_timeout_get
 * Description:	    This function gets the configured unanswered call timeout before cancel
 * Argument:
 * Return:		    unanswered call timeout which is configured in acl_conf (in MS)
 * Remarks:
 *
 ******************************************************************************/
int voip_conf_unansweredCall_timeout_get(void)
{
    int timeout;

    timeout = voip_config_params.voip_timeouts_info.unansweredCallTimeout;
    return timeout*1000;
}

/*******************************************************************************
 * Function:	   voip_conf_regret_call_enable_get
 ******************************************************************************/
void  voip_conf_regret_call_enable_get(char *pIsEnable)
{
   *pIsEnable = voip_config_params.voip_call_mngr_info.regret_call_enabled;

   return;
}

/*******************************************************************************
 * Function:	    voip_conf_regret_call_timeout_get
 * Description:	    This function gets the configured regret call timeout
 * Argument:
 * Return:
 * Remarks:
 *
 ******************************************************************************/
int voip_conf_regret_call_timeout_get(void)
{
    int regretCallTimeout;

	regretCallTimeout = voip_config_params.voip_call_mngr_info.regret_call_timeout;
    return regretCallTimeout*1000;
}

// by itzik - 19072006 - digitmap
/****************************************************************************
*
*	Name:		compareDialStr2digitMap
*----------------------------------------------------------------------------
*	Author:		Itzik F
*----------------------------------------------------------------------------
*	Abstract:	compare the dial string to the digit map
*----------------------------------------------------------------------------
*	Input:		digitMap -
*				dialStr -
*----------------------------------------------------------------------------
*   Output:		none
*----------------------------------------------------------------------------
*	Returns: 	if there is a match = 1, else 0
******************************************************************************/
int compareDialStr2digitMap(char *digitMap, char *dialStr)
{
    int match=0;
    int i=0;
    int j;
    int len;
    int dialStrLen;
    char dial_str[MAX_CALLER_ID_SIZE];

    if((digitMap == NULL) || (dialStr == NULL))
        return 0;

    len = strlen(digitMap);
    dialStrLen=strlen(dialStr);

    AC_STRNCPY(dial_str, (char *)dialStr, MAX_CALLER_ID_SIZE - 1);
    /* convert # * from digit map to e and f*/
    dtmf_convert(dial_str);

    while(i<len && !match)
    { 	/* scan the dial string until the end or until there is a mismatch */

        for(j=0,match=1; i<len && match && digitMap[i] != ';'; i++ , j++)
        {
            if((digitMap[i] != 'x' && digitMap[i] != dial_str[j]) || j>=dialStrLen)
                match=0;
        }
        if(j != dialStrLen)
            match=0;
        for(;i<len && digitMap[i] != ';';i++)
            ;
        if(digitMap[i] == ';')
            i++;

    } // end while

    return match;

} /* end of compareDialStr2digitMap */
/******************************************************************************/
/****************************************************************************
*
*   Name:       digitGetUnitRange
*----------------------------------------------------------------------------
*   Abstract:   analyze range that enclosed between square brackets("[" and "]").
*----------------------------------------------------------------------------
*   Input:      mapRange -len
*----------------------------------------------------------------------------
*   Returns:    on success = 0, else -1
******************************************************************************/
int digitGetUnitRange(char ** mapRange, int len, digit_sub_scheme_t * subScheme)
{
    int i, j;
    int start, end;

    if (NULL == mapRange || NULL == subScheme )
        return -1;

    i = 0;
    while ( i < len )
    {
        switch ((*mapRange)[i++])
        {
        case '[':
            break;
        case ']':
            *mapRange += i;
            if ( subScheme->rangeMap == 0)
                return -1;
            return 0;
        case '-':
            if ( i <= 1 || i >= (int)len -1 )
                return -1;
            start = (*mapRange)[i-2] - '0';
            end = (*mapRange)[i] - '0';

            if ( start > 9 || end > 9 )
                return -1;
            for ( j = start; j <= end; j ++)
                subScheme->rangeMap |= (1 << j);

            i ++;
            break;
        case '0':
            subScheme->rangeMap |= DIGIT_0_MASK;
            break;
        case '1':
            subScheme->rangeMap |= DIGIT_1_MASK;
            break;
        case '2':
            subScheme->rangeMap |= DIGIT_2_MASK;
            break;
        case '3':
            subScheme->rangeMap |= DIGIT_3_MASK;
            break;
        case '4':
            subScheme->rangeMap |= DIGIT_4_MASK;
            break;
        case '5':
            subScheme->rangeMap |= DIGIT_5_MASK;
            break;
        case '6':
            subScheme->rangeMap |= DIGIT_6_MASK;
            break;
        case '7':
            subScheme->rangeMap |= DIGIT_7_MASK;
            break;
        case '8':
            subScheme->rangeMap |= DIGIT_8_MASK;
            break;
        case '9':
            subScheme->rangeMap |= DIGIT_9_MASK;
            break;
        case '*':
            subScheme->rangeMap |= DIGIT_S_MASK;
            break;
        case '#':
            subScheme->rangeMap |= DIGIT_O_MASK;
            break;
        case 'a':
        case 'A':
            subScheme->rangeMap |= DIGIT_A_MASK;
            break;
        case 'b':
        case 'B':
            subScheme->rangeMap |= DIGIT_B_MASK;
            break;
        case 'c':
        case 'C':
            subScheme->rangeMap |= DIGIT_C_MASK;
            break;
        case 'd':
        case 'D':
            subScheme->rangeMap |= DIGIT_D_MASK;
            break;
        case ' ':
            break;
        default:
            return -1;
        }
    }

    return -1;
}
/****************************************************************************
*
*	Name:		expandDigitMap
*----------------------------------------------------------------------------
*	Abstract:	expand digit map
*----------------------------------------------------------------------------
*	Input:
*----------------------------------------------------------------------------
*	Returns: 	 on success = 0, else -1
******************************************************************************/
int expandDigitMap( void )
{
    int i, j;
    char * pucStart, * pucEnd;
    int len;
    char * digitMap;

    digitMap = voip_config_params.dialingInfo.digitMap;

    digit_map_style_set();
    if( digitmap_type != DIGIT_MAP_MGCP_LIKE_STYLE )
        return -1;

    len = strlen(digitMap);

    if ( len > MAX_DIGITMAP_STRLEN )
        len = MAX_DIGITMAP_STRLEN;


    memset(&stDigitMap, 0, sizeof(digit_map_cb_t));

    i = j = 0;
    pucStart = digitMap;
    pucEnd = pucStart + len;

    while ( pucStart < pucEnd )
    {
        switch (*pucStart)
        {
        case '|':
            if (i == j )
                break;
            if (i >= 1)
            {
                stDigitMap.digitSubScheme[i-1].isLastUnit = 1;
            }
            else
            {
                printf("function: expandDigitMap error!");
            }
            for ( ; j<i; j ++ )
            {
                stDigitMap.digitSubScheme[j].nextSubSchemeIndex = (int)i;
            }
            break;
        case '[':
            if (digitGetUnitRange(&pucStart, (int)(pucEnd - pucStart), &stDigitMap.digitSubScheme[i++]))
                goto error;
            continue;
        case '.':
	     /*continous emergence of posotion(".") in the digit map,in this case,we should only keep the first one,
		 for example assume we have digit map "12.3.4.5",  "3.4."will be ingored*/
            if ( i > j + 1 && stDigitMap.digitSubScheme[i-2].isVarLen == 1)
            {
                stDigitMap.digitSubScheme[i-2].rangeMap |= stDigitMap.digitSubScheme[i-1].rangeMap;
                stDigitMap.digitSubScheme[i-1].rangeMap = 0;
                i --;
            }
            else if ( i > j )
            {
                stDigitMap.digitSubScheme[i-1].isVarLen = 1;
            }
            break;
        case '0':
            stDigitMap.digitSubScheme[i++].rangeMap |= DIGIT_0_MASK;
            break;
        case '1':
            stDigitMap.digitSubScheme[i++].rangeMap |= DIGIT_1_MASK;
            break;
        case '2':
            stDigitMap.digitSubScheme[i++].rangeMap |= DIGIT_2_MASK;
            break;
        case '3':
            stDigitMap.digitSubScheme[i++].rangeMap |= DIGIT_3_MASK;
            break;
        case '4':
            stDigitMap.digitSubScheme[i++].rangeMap |= DIGIT_4_MASK;
            break;
        case '5':
            stDigitMap.digitSubScheme[i++].rangeMap |= DIGIT_5_MASK;
            break;
        case '6':
            stDigitMap.digitSubScheme[i++].rangeMap |= DIGIT_6_MASK;
            break;
        case '7':
            stDigitMap.digitSubScheme[i++].rangeMap |= DIGIT_7_MASK;
            break;
        case '8':
            stDigitMap.digitSubScheme[i++].rangeMap |= DIGIT_8_MASK;
            break;
        case '9':
            stDigitMap.digitSubScheme[i++].rangeMap |= DIGIT_9_MASK;
            break;
        case '*':
            stDigitMap.digitSubScheme[i++].rangeMap |= DIGIT_S_MASK;
            break;
        case '#':
            stDigitMap.digitSubScheme[i++].rangeMap |= DIGIT_O_MASK;
            break;
        case 'a':
        case 'A':
            stDigitMap.digitSubScheme[i++].rangeMap |= DIGIT_A_MASK;
            break;
        case 'b':
        case 'B':
            stDigitMap.digitSubScheme[i++].rangeMap |= DIGIT_B_MASK;
            break;
        case 'c':
        case 'C':
            stDigitMap.digitSubScheme[i++].rangeMap |= DIGIT_C_MASK;
            break;
        case 'd':
        case 'D':
            stDigitMap.digitSubScheme[i++].rangeMap |= DIGIT_D_MASK;
            break;
        case 'x':
        case 'X':
            stDigitMap.digitSubScheme[i++].rangeMap |=
                ( DIGIT_0_MASK | DIGIT_1_MASK | DIGIT_2_MASK | DIGIT_3_MASK
                 |DIGIT_4_MASK | DIGIT_5_MASK | DIGIT_6_MASK | DIGIT_7_MASK
                 |DIGIT_8_MASK | DIGIT_9_MASK );
            break;
        case ' ':
            break;
        default:
            goto error;
        }

        pucStart ++;
    }

    if ( i == 0)
        goto error;

    stDigitMap.validUnitNum = (int)i;
    stDigitMap.digitSubScheme[i-1].isLastUnit = 1;
    for ( ; j<i; j ++ )
    {
        stDigitMap.digitSubScheme[j].nextSubSchemeIndex = (int)i;
    }

    return 0;
error:
    printf("\r\nexpandDigitMap: Invalid digit-map.");
    return -1;
}

/****************************************************************************
*
*	Name:		digitMapMatch
*----------------------------------------------------------------------------
*	Abstract:	compare the dial string to the digit map according to RFC3435 digit map syntax
*----------------------------------------------------------------------------
*	Input:		dialStr
*----------------------------------------------------------------------------
*	Returns: 	if there is a match = 1, else 0
******************************************************************************/
int digitMapMatch( char * dialStr )
{
    int i, j, index, mask, flag;
    digit_sub_scheme_t * subScheme;
   int len;
   /*EA - 080911 - VI66018 - Improved digit map 'MGCP Like' - inserted Xin's changes*/
   char dial_str[MAX_CALLER_ID_SIZE];

    if ( NULL == dialStr )
        return 0;

    len = strlen(dialStr);
   /*EA - 080911 - VI66018 - Improved digit map 'MGCP Like' - inserted Xin's changes*/
    AC_STRNCPY(dial_str, (char *)dialStr, MAX_CALLER_ID_SIZE - 1);
    convert_dtmf(dial_str);

    index = 0;
    flag = 0;
    subScheme = NULL;

    while ( index < stDigitMap.validUnitNum )
    {
        i = j = 0;
        for ( i = 0; i< len; i++ )
        {
            /*EA - 080911 - VI66018 - Improved digit map 'MGCP Like' - inserted Xin's changes*/
            switch ( dial_str[i] )
            {
            case '0':
                mask = DIGIT_0_MASK;
                break;
            case '1':
                mask = DIGIT_1_MASK;
                break;
            case '2':
                mask = DIGIT_2_MASK;
                break;
            case '3':
                mask = DIGIT_3_MASK;
                break;
            case '4':
                mask = DIGIT_4_MASK;
                break;
            case '5':
                mask = DIGIT_5_MASK;
                break;
            case '6':
                mask = DIGIT_6_MASK;
                break;
            case '7':
                mask = DIGIT_7_MASK;
                break;
            case '8':
                mask = DIGIT_8_MASK;
                break;
            case '9':
                mask = DIGIT_9_MASK;
                break;
            case '*':
                mask = DIGIT_S_MASK;
                break;
            case '#':
                mask = DIGIT_O_MASK;
                break;
            case 'a':
            case 'A':
                mask = DIGIT_A_MASK;
                break;
            case 'b':
            case 'B':
                mask = DIGIT_B_MASK;
                break;
            case 'c':
            case 'C':
                mask = DIGIT_C_MASK;
                break;
            case 'd':
            case 'D':
                mask = DIGIT_D_MASK;
                break;
            default:
                return 0;
            }

            subScheme = stDigitMap.digitSubScheme + (index + j);
            if ( subScheme->isVarLen )
            {
                /*Position(".") is the last unit of one sub-scheme*/
                if ( subScheme->isLastUnit )
                {
                    return 1;
		  }
                j ++;
                subScheme = stDigitMap.digitSubScheme + (index + j);
            }

            if ( subScheme->rangeMap & mask )
            {
                /*full match */
                if ( subScheme->isLastUnit )
                {
                    return 1;
                }
                j ++;
                continue;
            }
            /*handling Position(".")  */
            else if ( j > 0 && subScheme[-1].isVarLen && (subScheme[-1].rangeMap & mask ))
            {
                continue;
            }

            /* no match */
            break;
        }

        /* partial match */
        if ( i == len )
        {
            /* position  (".") allows for zero occurrences of the preceding construct,
                a match will be occured in the following case(the last unit is position("."))*/
            subScheme = stDigitMap.digitSubScheme + (index + j);
            if (subScheme->isVarLen && subScheme->isLastUnit)
            {
                return 1;
            }
            flag = 1;
        }
        else
            flag = 1;

	 /*if there is a mismatch in one sub-scheme,we should check others*/
        if (subScheme)
        {
            if ( subScheme->nextSubSchemeIndex <= index )
            {
                return 0;
            }

            index = subScheme->nextSubSchemeIndex;
        }
    }

    if ( flag )
    {
        return 0;
    }

    return 1;
}

/****************************************************************************
*
*   Name:       digit_map_style_get
*----------------------------------------------------------------------------
*   Abstract:   we do`t need to check the digt map syntax here,we check it in s_sec_dial function
*----------------------------------------------------------------------------
*   Input:
*----------------------------------------------------------------------------
*   Returns:    on success = 0, else -1
******************************************************************************/
int digit_map_style_set( void )
{
    char * start, * end;
    char isMgcpLikeStyle=0;
    char isOldStyle=0;
    int len;
    char * digitMap;

    digitMap = voip_config_params.dialingInfo.digitMap;

    len = strlen(digitMap);

    start = digitMap;
    end = start + len;

    while ( start < end )
    {
        switch (*start)
        {
            case '|':
            case '[':
            case '.':
                isMgcpLikeStyle = 1;
                break;
            case ';':
                isOldStyle = 1;
                break;
            default:
                break;
        }

        start ++;
    }

    digitmap_type = isMgcpLikeStyle ? DIGIT_MAP_MGCP_LIKE_STYLE : DIGIT_MAP_OLD_STYLE;
    return 0;
}

/* AlexChen 20090402 for is_redundant_proxy update */
void voip_conf_ext_is_redundant_proxy_set(int IsRedundatProxy)
{
	char cfgbuf[CONFIGURATION_BUFFER_MAX_LEN] = {0};
	int fd;
	int len;
	char *p = NULL;

	fd = open(g_cAcl_cfgfilename, O_RDWR);
	len = read(fd, cfgbuf, CONFIGURATION_BUFFER_MAX_LEN);

	p = strstr(cfgbuf, "is_redundant_proxy");

	if (p){
		*(p+19) = 48 + IsRedundatProxy;

		lseek(fd, 0, SEEK_SET);
		write(fd, cfgbuf, len);
	}
	close(fd);
	DPRINTK("->\r\n");

	DPRINTK("set registrar to %s \r\n", IsRedundatProxy?"redundant":"primary");
	DPRINTK("<-\r\n");
}
/*******************************************************************************
 * Function:	voip_conf_ext_is_redundant_proxy_get
 * Description:	This function reads the registrar proxy type from rg_conf
 * Arguments:	none
 * Return:		current sip registrar: 0 - primary 1 - redundant
 *
 ******************************************************************************/
/*JL VI # 62393 redundant proxy feature */
int voip_conf_ext_is_redundant_proxy_get(void)
{

	DPRINTK("->\r\n");

	int IsRedundatProxy = voip_config_params.voip_call_mngr_info.sp.sip.is_redundant_proxy;

	DPRINTK("registrar is %s \r\n", IsRedundatProxy?"redundant":"primary");
	DPRINTK("<-\r\n");
	return IsRedundatProxy;
}

/* AlexChen SDK upgrade 20090407 DND feature */
/*******************************************************************************
 * Function:	   voip_conf_dnd_line_set
 * Description:
 * Arguments:
 * Return:
 *
 ******************************************************************************/
int voip_conf_dnd_line_set(int idx, int isEnable)
{
	voip_config_params.voip_dnd_info.line_actived[idx] = isEnable;
	/* need to be done */
	return 0;
}

/*******************************************************************************
 * Function:	   voip_conf_dnd_line_get
 * Description:
 * Arguments:
 * Return:
 *
 ******************************************************************************/
int voip_conf_dnd_line_get(int idx,char *pIsEnable )
{
	*pIsEnable = voip_config_params.voip_dnd_info.line_actived[idx];
	return *pIsEnable;
}

/*******************************************************************************
 * Function:	   voip_conf_remote_fax_side_reinvite_get()
 * Description:
 * Arguments:
 * Return:
 *
 ******************************************************************************/
int voip_conf_remote_fax_side_reinvite_get()
{
	return voip_config_params.voip_call_mngr_info.sp.sip.remote_fax_side_reinvite;
}
/* ACL NB 061225 */
/*******************************************************************************
 * Function:	   voip_conf_ua_get
 * Description:	This function gets the usragent string
 * Arguments:
 * Return:
 * Remarks:
 *
 ******************************************************************************/
void voip_conf_ua_get(char *buff)
{
    if (NULL == buff)
    {
        printf("[%s:%d] buff is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
    strcpy(buff, voip_config_params.userAgentStr);
}

/* 070111 Yair: write voip external configuration to acl_conf */
void voip_conf_write_ext(int fd)
{
}



/* ACL NB 070508*/
/* NOTR:Set cmd can be only on the ext_conf sub-tree*/
/*******************************************************************************
 * Function:	   voip_conf_ext_cfw_dest_set
 * Description:	This function sets the CFW dest url
 * Arguments:
 * Return:
 *
 ******************************************************************************/
int voip_conf_ext_cfw_dest_set(int idx, char *dest)
{
   return(0);
}

/*******************************************************************************
 * Function:	   voip_conf_ext_cfw_dest_get
 * Description:	This function sets the CFW dest url
 * Arguments:
 * Return:
 *
 ******************************************************************************/
int voip_conf_ext_cfw_dest_get(int idx, char **dest)
{
    if (NULL == dest || idx > MAX_NUM_OF_LINES)
    {
        printf("[%s:%d] dest is NULL\n", __FUNCTION__, __LINE__);
        return (-1);
    }
    *dest = voip_config_params.voip_cfw_info.line[idx].cfw_dest;

	 return(0);
}


/* ACL NB 070118 */
/*******************************************************************************
 * Function:	   voip_conf_voip_log_lvl_get
 * Description:	This function gets the voip log level
 * Arguments:
 * Return:
 *
 ******************************************************************************/
void voip_conf_voip_log_lvl_get(int *pLogLevel)
{
    if (NULL == pLogLevel)
    {
        printf("[%s:%d] pLogLevel is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
    *pLogLevel= system_config_params.system_log_info.SIPLogLevel;
}

/* ACL */
/* NOTE:Set cmd can be only on the ext_conf sub-tree */
/*******************************************************************************
 * Function:	   voip_conf_ext_sync_check_set
 * Description:	This function sets sync-check flag
 * Arguments:
 * Return:
 *
 ******************************************************************************/
int voip_conf_ext_sync_check_set(int sync_check)
{
   return(0);
}

/* ACL NB 070528 get dialing parameters*/
/*******************************************************************************
 * Function:	   voip_conf_dialing_param_get
 * Description:	retuens all dialing parameters
 * Arguments:
 * Return:
 *
 ******************************************************************************/
void voip_conf_dialing_param_get(acl_call_mngr_params_t *params)
{
    if (NULL == params)
    {
        printf("[%s:%d] params is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
    params->is_auto_dial             = voip_config_params.voip_call_mngr_info.is_auto_dial;
    params->auto_dial_timeout        = voip_config_params.voip_call_mngr_info.auto_dial_timeout * 1000; //convert to msec
    strcpy(params->auto_dial_dest, voip_config_params.voip_call_mngr_info.auto_dial_dest);
    params->use_secondary_dial_tone  = voip_config_params.voip_call_mngr_info.use_secondary_dial_tone;
    params->secondary_dial_tone_key  = voip_config_params.voip_call_mngr_info.secondary_dial_tone_key;
}

/* ACL NB 070614 get prack enable parameter */
/*******************************************************************************
 * Function:	   voip_conf_prack_enable_get
 ******************************************************************************/
void  voip_conf_prack_enable_get(char *pIsEnable)
{
    if (NULL == pIsEnable)
    {
        printf("[%s:%d] pIsEnable is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
    *pIsEnable = voip_config_params.voip_advanced_signaling_info.prackEnabled;
}

/* ACL NB 070614 get fxs_polarity_revasal */
/*******************************************************************************
 * Function:	   voip_conf_fxs_polarity_revasal_enable_get
 * Description:	Gets fxs_polarity_revasal  flag
 *
 ******************************************************************************/
void  voip_conf_fxs_polarity_revasal_enable_get(char *pIsEnable)
{
    if (NULL == pIsEnable)
    {
        printf("[%s:%d] pIsEnable is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
    *pIsEnable = voip_config_params.polarityRevasalEnabled;
}

/* ACL NB 070718 get FAxMaxRate */
/*******************************************************************************
 * Function:	   voip_conf_fax_max_rate_get
 * Description:	Gets fax max rate
 *
 ******************************************************************************/
void  voip_conf_fax_max_rate_get(int *pMaxRate)
{
    if (NULL == pMaxRate)
    {
        printf("[%s:%d] pMaxRate is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
    *pMaxRate = voip_config_params.voip_dsp_advanced_channel_configuration.FaxMaxRate;
}

/* AlexChen SDK upgrade 20090409 for mgcp */
/*******************************************************************************
 * Function:	   voip_conf_fax_end_report_enable_get
 ******************************************************************************/
void  voip_conf_fax_end_report_enable_get(char *pIsEnable)
{
	*pIsEnable = voip_config_params.voip_call_mngr_info.sp.mgcp.fax_end_report;
   return;
}

/*******************************************************************************
 * Function:	   voip_conf_fax_audio_startup_get
 ******************************************************************************/
void  voip_conf_fax_audio_startup_enable_get(char *pIsEnable)
{
	*pIsEnable = voip_config_params.voip_call_mngr_info.fax_audio_startup_enabled;
	return;
}

/*******************************************************************************
 * Function:	   voip_conf_fax_audio_startup_payload_get
 ******************************************************************************/
void  voip_conf_fax_audio_startup_payload_get(int *payload)
{
	*payload = voip_config_params.voip_call_mngr_info.fax_audio_startup_payload;
	return;
}
/*******************************************************************************
 * Function:	   voip_conf_fax_max_buffer_get
 * Description:	Gets fax max buffer
 *
 ******************************************************************************/
void  voip_conf_fax_max_buffer_get(int *pMaxBuffer)
{
    if (NULL == pMaxBuffer)
    {
        printf("[%s:%d] pMaxBuffer is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
    *pMaxBuffer = voip_config_params.voip_dsp_advanced_channel_configuration.FaxMaxBuffer;
}

/*******************************************************************************
 * Function:	   voip_conf_fax_max_buffer_get
 * Description:	Gets fax max buffer
 *
 ******************************************************************************/
void  voip_conf_fax_max_datagram_get(int *pMaxDatagram)
{
    if (NULL == pMaxDatagram)
    {
        printf("[%s:%d] pMaxDatagram is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
    *pMaxDatagram = voip_config_params.voip_dsp_advanced_channel_configuration.FaxMaxDatagram;
}

/*******************************************************************************
 * Function:	   voip_conf_get_number_of_lines
 * Description:	Gets number of lines
 *
 ******************************************************************************/
int voip_conf_get_number_of_lines(analog_port_type_t AnalogPortType)
{
	static int iNumOfFXSLines;
	static int iNumOfFXOLines;

	iNumOfFXSLines = voip_config_params.numberOfFxsLines;
	iNumOfFXOLines = voip_config_params.numberOfFxoLines;

	/* NirM, Need to be modified to support FXO */
	switch(AnalogPortType)
	{
		case ANALOG_PORT_TYPE_FXS:
			return iNumOfFXSLines;
			break;
		case ANALOG_PORT_TYPE_FXO:
			return iNumOfFXOLines;
			break;
		case ANALOG_PORT_TYPE_FXO_FXS:
			return iNumOfFXSLines+iNumOfFXOLines;
			break;
		default:
			printf("[%s:%d] invalid AnalogPortType(%d) \n", __FUNCTION__, __LINE__, AnalogPortType);
			return 0;
	}
}


/*******************************************************************************
 * Function:	   voip_conf_get_sdp_include_ptime
 * Description:
 *
 ******************************************************************************/
void  voip_conf_get_sdp_include_ptime(char *pInclude_ptime)
{
    if (NULL == pInclude_ptime)
    {
        printf("[%s:%d] pInclude_ptime is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
    *pInclude_ptime=voip_config_params.voip_advanced_signaling_info.includePTime;
}

/* ACL NB 070729 get rport enable parameter */
/*******************************************************************************
 * Function:	   voip_conf_rport_enable_get
 ******************************************************************************/
void  voip_conf_rport_enable_get(char *pIsEnable)
{
    if (NULL == pIsEnable)
    {
        printf("[%s:%d] pIsEnable is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
    *pIsEnable = voip_config_params.voip_advanced_signaling_info.rportEnabled;
}

/* ACL NB 071014 get Ka (using optins msg) enable parameter */
/*******************************************************************************
 * Function:	   voip_conf_ka_options_enabled_get
 ******************************************************************************/
void  voip_conf_ka_options_enabled_get(char *pIsEnable)
{
    if (NULL == pIsEnable)
    {
        printf("[%s:%d] pIsEnable is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
    *pIsEnable = voip_config_params.voip_advanced_signaling_info.kaEnabled;
}

/* ACL NB 071014 get Ka (using optins msg) timeout parameter */
/*******************************************************************************
 * Function:	   voip_conf_ka_options_timeout_get
 ******************************************************************************/
void  voip_conf_ka_options_timeout_get(int *pTimeout)
{
    if (NULL == pTimeout)
    {
        printf("[%s:%d] pTimeout is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
   //convert from seconds to msec
   *pTimeout = voip_config_params.voip_advanced_signaling_info.kaTimeout * 1000;
}

/*EA - 071121 RFC 4240 conference*/
/*******************************************************************************
 * Function:	   voip_conf_remote_conference_get
 ******************************************************************************/
void  voip_conf_remote_conference_get(int *remoteConference)
{
    if (NULL == remoteConference)
    {
        printf("[%s:%d] remoteConference is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
    *remoteConference = voip_config_params.voip_conference_info.confMode;
}

/*******************************************************************************
 * Function:	   voip_conf_remote_conference_conf_addr_get
 ******************************************************************************/
void  voip_conf_remote_conference_conf_addr_get(char **confAddr)
{
    if (NULL == confAddr)
    {
        printf("[%s:%d] confAddr is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
    *confAddr = voip_config_params.voip_conference_info.confMsAddr;
}

/*******************************************************************************
 * Function:	   voip_conf_remote_conference_mac_addr_get
 ******************************************************************************/
char *voip_conf_remote_conference_mac_addr_get(void)
{
    return system_config_params.wan_mac_addr;
}

/*******************************************************************************
 * Function:	   voip_conf_blind_transfer_enabled_get
 ******************************************************************************/
void  voip_conf_blind_transfer_enable_get(char *pIsEnable)
{
    if (NULL == pIsEnable)
    {
        printf("[%s:%d] pIsEnable is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
    *pIsEnable = voip_config_params.voip_advanced_signaling_info.blindTransferEnabled;
}

/*******************************************************************************
 * Function:	   voip_conf_rv_log_filter_get
 ******************************************************************************/
int voip_conf_rv_log_filter_get(void)
{
   return system_config_params.system_log_info.rvLogFilter;
}

/*EA - 071231 - VI#59630 support SIP over TLS */
#ifdef RV_CFLAG_TLS
/*******************************************************************************
 * Function:	   voip_conf_is_transport_protocol_tls
 ******************************************************************************/
int voip_conf_is_transport_protocol_tls(void)
{
    if ((VOIP_SIP_TRANSPORT_TCP == voip_config_params.voip_call_mngr_info.sp.sip.sip_transport_protocol) &&
        (0 < voip_config_params.voip_call_mngr_info.sp.sip.transportTlsCfg.stackNumOfTlsAddresses))
        return 1;
    return 0;
}
#endif /*RV_CFLAG_TLS*/
/* ACL NB 071031 get Stun Client enable parameter */
/*******************************************************************************
 * Function:	   voip_conf_stun_enable_get
 ******************************************************************************/
void  voip_conf_stun_enable_get(char *pIsEnable)
{
    if (NULL == pIsEnable)
    {
        printf("[%s:%d] pIsEnable is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
    *pIsEnable = voip_config_params.voip_stun_ex_info.stunEnabled;
}

/* ACL NB 071031 get Stun Client parameters (all configuration ) */
/*******************************************************************************
 * Function:	   voip_conf_stun_get
 * Description:	This function gets the configuration of the Stun feature
 * Arguments:
 * Return:
 * Remarks:
 *
 ******************************************************************************/
void  voip_conf_stun_get(voip_stun_params_t *pStunParms)
{
    if (NULL == pStunParms)
    {
        printf("[%s:%d] pStunParms is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
    memcpy(pStunParms, &(voip_config_params.voip_stun_ex_info.voip_stun_info), sizeof(voip_stun_params_t));
}


/*EA - 080114 - VI#60461 - Bezeq Interop*/
/*******************************************************************************
 * Function:	    voip_conf_key_Sequence_sip_info_strings_get
 * Description:	    This function gets the SIP INFO header and body in order to
 *                  send flash via SIP INFO
 * Arguments:
 * Return:
 * Remarks:
 *
 ******************************************************************************/
void voip_conf_key_Sequence_sip_info_strings_get(char **header, char **body)
{
    if (NULL == header)
    {
        printf("[%s:%d] header is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
    if (NULL == body)
    {
        printf("[%s:%d] body is NULL\n", __FUNCTION__, __LINE__);
        return;
    }

    *header = voip_config_params.voip_sip_info_key_seq_info.sip_info_key_seq_header;
    *body = voip_config_params.voip_sip_info_key_seq_info.sip_info_key_seq_body;

    return;
}


/*EA 080122 - VI#60671 - for Setting Syslog IP address for application logs through GUI*/
/*******************************************************************************
 * Function:	   voip_conf_syslog_server_info_get
 ******************************************************************************/
void voip_conf_syslog_server_info_get(int *serverPortForAppSyslog, char **serverAddrForAppSyslog, int *udpTerminalFlag)
{
    if (NULL == serverAddrForAppSyslog)
    {
        printf("[%s:%d] serverAddrForAppSyslog is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
    if (NULL == serverPortForAppSyslog)
    {
        printf("[%s:%d] serverPortForAppSyslog is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
    if (NULL == udpTerminalFlag)
    {
        printf("[%s:%d] udpTerminalFlag is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
    *serverAddrForAppSyslog = system_config_params.system_log_info.serverAddrForAppSyslog;
    *serverPortForAppSyslog = system_config_params.system_log_info.serverPortForAppSyslog;
    *udpTerminalFlag = system_config_params.system_log_info.udpOrTerminalFlag;
}

/*******************************************************************************
 * Function:	   voip_conf_packet_recording_enable_get
 ******************************************************************************/
int  voip_conf_packet_recording_enable_get(void)
{
    return(voip_config_params.voip_debug_information_info.enable_packet_recording);
}

/*******************************************************************************
 * Function:	   voip_conf_network_recording_enable_get
 ******************************************************************************/
int  voip_conf_network_recording_enable_get(void)
{
    return(voip_config_params.voip_debug_information_info.enable_network_recording);
}

/*******************************************************************************
 * Function:	   voip_conf_packet_recording_enable_get
 ******************************************************************************/
int  voip_conf_tdm_recording_enable_get(void)
{
    return(voip_config_params.voip_debug_information_info.enable_tdm_recording);
}

/*******************************************************************************
 * Function:	   voip_conf_tdm_network_recording_channel_get
 ******************************************************************************/
int  voip_conf_tdm_network_recording_channel_get(void)
{
    return(voip_config_params.voip_debug_information_info.channel);
}

/*ACL XY 090106 - Fax Fallback. Configurable option for updating fax mode to transparent upon "audio" Re-invite from remote */
/*******************************************************************************
 * Function:	   voip_conf_update_fax_to_transparent_enable_get
 ******************************************************************************/
void  voip_conf_update_fax_to_transparent_enable_get(char *pIsEnable)
{
	*pIsEnable = voip_config_params.voip_call_mngr_info.update_fax_to_transparent_enable;
   	return;
}

/*EA - 090107 - AC68794 - Make the 'Mute RTP' option (upon hold) configurable*/
/*******************************************************************************
 * Function:	   voip_conf_rtp_mute_on_hold_get
 ******************************************************************************/
void  voip_conf_rtp_mute_on_hold_get(char *pMuteRtp)
{
    *pMuteRtp = voip_config_params.voip_call_mngr_info.rtp_mute_on_hold;
    return;
}



/*******************************************************************************
 * Function:	   voip_conf_debug_info_get
 ******************************************************************************/
void voip_conf_debug_info_get(int *packetRecordingPort, char *packetRecordingAddr)
{
    if (NULL == packetRecordingAddr)
    {
        printf("[%s:%d] packetRecordingAddr is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
    if (NULL == packetRecordingPort)
    {
        printf("[%s:%d] packetRecordingPort is NULL\n", __FUNCTION__, __LINE__);
        return;
    }
    strcpy(packetRecordingAddr, voip_config_params.voip_debug_information_info.remoteAddr);
    *packetRecordingPort = voip_config_params.voip_debug_information_info.remotePort;
}

/***************************************************************************************/
/*   voip_conf_print_call_mgt_params                                                */
/***************************************************************************************/
void voip_conf_print_call_mgt_params(acl_call_mngr_params_t *voip_call_mngr_info)
{
    int i;

    printf("voip_call_mngr_info->protocol = %s\n", code2str_ex(voip_protocols_t_str, voip_call_mngr_info->protocol));

    for (i = 0; i < MAX_NUM_OF_LINES; i++)
	{
        printf("voip_call_mngr_info->lines[i].enabled = %d\n", voip_call_mngr_info->lines[i].enabled);
        printf("voip_call_mngr_info->lines[i].id = %s\n", voip_call_mngr_info->lines[i].id);
        printf("voip_call_mngr_info->lines[i].display_name = %s\n", voip_call_mngr_info->lines[i].display_name);
        printf("voip_call_mngr_info->lines[i].usernameServerAuth = %s\n", voip_call_mngr_info->lines[i].usernameServerAuth);
        printf("voip_call_mngr_info->lines[i].pswdServerAuth = %s\n", voip_call_mngr_info->lines[i].pswdServerAuth);
        printf("voip_call_mngr_info->lines[i].send_caller_id = %d\n", voip_call_mngr_info->lines[i].send_caller_id);
    }

    printf("voip_call_mngr_info->ep_num = %d\n", voip_call_mngr_info->ep_num);
    printf("voip_call_mngr_info->dtmf_transport_method = %s\n", code2str_ex(dtmf_transport_mode_t_str, voip_call_mngr_info->dtmf_transport_method));
    printf("voip_call_mngr_info->protocol = %s\n", code2str_ex(voip_protocols_t_str, VOIP_PROT_SIP));
    printf("voip_call_mngr_info->media_port = %d\n", voip_call_mngr_info->media_port);
    printf("voip_call_mngr_info->media_tos = 0x%x\n", voip_call_mngr_info->media_tos);
    printf("voip_call_mngr_info->mss_clamping_limit = %d\n", voip_call_mngr_info->mss_clamping_limit);
    printf("voip_call_mngr_info->local_host_port = %d\n", voip_call_mngr_info->local_host_port);
    printf("voip_call_mngr_info->local_host_port_range = %d\n", voip_call_mngr_info->local_host_port_range);
    printf("voip_call_mngr_info->is_auto_dial = %d\n", voip_call_mngr_info->is_auto_dial);
    printf("voip_call_mngr_info->auto_dial_timeout = %d\n", voip_call_mngr_info->auto_dial_timeout);
    printf("voip_call_mngr_info->auto_dial_dest = %s\n", voip_call_mngr_info->auto_dial_dest);
    printf("voip_call_mngr_info->use_secondary_dial_tone = %d\n", voip_call_mngr_info->use_secondary_dial_tone);
    printf("voip_call_mngr_info->secondary_dial_tone_key = %d\n", voip_call_mngr_info->secondary_dial_tone_key);

#if 0
    /*H.323 params ????*/
    voip_call_mngr_info->sp.h323.e164_alias_list;
    /*voip_call_mngr_info->sp.h323.fast_start;*/
    /*voip_call_mngr_info->sp.h323.gk_address = "";*/
    voip_call_mngr_info->sp.h323.gk_port = 1719;
    /*voip_call_mngr_info->sp.h323.gk_registry = 0;*/
    voip_call_mngr_info->sp.h323.signal_port = 1720;
#endif

    /*SIP params */
    if (VOIP_PROT_SIP == voip_call_mngr_info->protocol)
    {
        printf("voip_call_mngr_info->sp.sip.sip_transport_protocol = %s\n",
            code2str_ex(voip_sip_transport_prot_t_str, voip_call_mngr_info->sp.sip.sip_transport_protocol));
        printf("voip_call_mngr_info->sp.sip.use_proxy = %d\n", voip_call_mngr_info->sp.sip.use_proxy);
        printf("voip_call_mngr_info->sp.sip.proxy_port = %d\n", voip_call_mngr_info->sp.sip.proxy_port);
        printf("voip_call_mngr_info->sp.sip.proxy_address = %s\n", voip_call_mngr_info->sp.sip.proxy_address);
        printf("voip_call_mngr_info->sp.sip.use_outbound_proxy = %d\n", voip_call_mngr_info->sp.sip.use_outbound_proxy);
        printf("voip_call_mngr_info->sp.sip.outbound_proxy_port = %d\n", voip_call_mngr_info->sp.sip.outbound_proxy_port);
        printf("voip_call_mngr_info->sp.sip.outbound_proxy_address = %s\n", voip_call_mngr_info->sp.sip.outbound_proxy_address);
        printf("voip_call_mngr_info->sp.sip.use_registrar = %d\n", voip_call_mngr_info->sp.sip.use_registrar);
        printf("voip_call_mngr_info->sp.sip.registrar_port = %d\n", voip_call_mngr_info->sp.sip.registrar_port);
        printf("voip_call_mngr_info->sp.sip.registrar_address = %s\n", voip_call_mngr_info->sp.sip.registrar_address);
        printf("voip_call_mngr_info->sp.sip.sip_port = %d\n", voip_call_mngr_info->sp.sip.sip_port);
        printf("voip_call_mngr_info->sp.sip.proxy_user_name = %s\n", voip_call_mngr_info->sp.sip.proxy_user_name);
        printf("voip_call_mngr_info->sp.sip.proxy_password = %s\n", voip_call_mngr_info->sp.sip.proxy_password);
        printf("voip_call_mngr_info->sp.sip.proxy_registration_timeout = %d\n", voip_call_mngr_info->sp.sip.proxy_registration_timeout);
        printf("voip_call_mngr_info->sp.sip.user_doamin = %s\n", voip_call_mngr_info->sp.sip.user_doamin);
        printf("voip_call_mngr_info->sp.sip.uregistration_timeout = %d\n", voip_call_mngr_info->sp.sip.uregistration_timeout);
        printf("voip_call_mngr_info->sp.sip.watchdog_timeout = %d\n", voip_call_mngr_info->sp.sip.watchdog_timeout);
        printf("voip_call_mngr_info->sp.sip.callWaitingEnabled = %d\n", voip_call_mngr_info->sp.sip.callWaitingEnabled);
        printf("voip_call_mngr_info->sp.sip.callWaitingEnabled = %s\n",
            code2str_ex(cw_sip_reply_t_str, voip_call_mngr_info->sp.sip.callWaitingEnabled));
        printf("voip_call_mngr_info->sp.sip.retransmissionT1 = %d\n", voip_call_mngr_info->sp.sip.retransmissionT1);
        printf("voip_call_mngr_info->sp.sip.retransmissionT2 = %d\n", voip_call_mngr_info->sp.sip.retransmissionT2);
        printf("voip_call_mngr_info->sp.sip.retransmissionT4 = %d\n", voip_call_mngr_info->sp.sip.retransmissionT4);
        printf("voip_call_mngr_info->sp.sip.inviteLingerTimer = %d\n", voip_call_mngr_info->sp.sip.inviteLingerTimer);
        printf("voip_call_mngr_info->sp.sip.use_proxy_port_ip_for_registrar = %d\n", voip_call_mngr_info->sp.sip.use_proxy_port_ip_for_registrar);
        printf("voip_call_mngr_info->sp.sip.auth_retries = %d\n", voip_call_mngr_info->sp.sip.auth_retries);
        printf("voip_call_mngr_info->sp.sip.connectMediaOn180 = %d\n", voip_call_mngr_info->sp.sip.connectMediaOn180);
#ifdef RV_CFLAG_TLS
        printf("voip_call_mngr_info->sp.sip.transportTlsCfg.stackTlsPort = %d\n", voip_call_mngr_info->sp.sip.transportTlsCfg.stackTlsPort);
        printf("voip_call_mngr_info->sp.sip.transportTlsCfg.stackNumOfTlsAddresses = %d\n", voip_call_mngr_info->sp.sip.transportTlsCfg.stackNumOfTlsAddresses);
        printf("voip_call_mngr_info->sp.sip.transportTlsCfg.tlsPostConnectAssertFlag = %d\n", voip_call_mngr_info->sp.sip.transportTlsCfg.tlsPostConnectAssertFlag);
        printf("voip_call_mngr_info->sp.sip.transportTlsCfg.tlsMethod = %d\n", voip_call_mngr_info->sp.sip.transportTlsCfg.tlsMethod);
        printf("voip_call_mngr_info->sp.sip.transportTlsCfg.privateKeyType = %d\n", voip_call_mngr_info->sp.sip.transportTlsCfg.privateKeyType);
        printf("voip_call_mngr_info->sp.sip.transportTlsCfg.certDepth = %d\n", voip_call_mngr_info->sp.sip.transportTlsCfg.certDepth);
#endif /* RV_CFLAG_TLS */
    }

    if (VOIP_PROT_MGCP == voip_call_mngr_info->protocol)
    {
        /*MGCP params */
        printf("voip_call_mngr_info->sp.mgcp.mgc_address = %s\n", voip_call_mngr_info->sp.mgcp.mgc_address);
        printf("voip_call_mngr_info->sp.mgcp.mgc_port = %d\n", voip_call_mngr_info->sp.mgcp.mgc_port);
        printf("voip_call_mngr_info->sp.sip.proxy_port = %d\n", voip_call_mngr_info->sp.mgcp.mg_port);
    }
}

void voip_conf_print_stun_params(voip_stun_params_ex_t *voip_stun_ex_info)
{
    printf("voip_stun_ex_info->stunEnabled = %d\n", voip_stun_ex_info->stunEnabled);
    printf("voip_stun_ex_info->voip_stun_info.stunClientResponsePort = %d\n", voip_stun_ex_info->voip_stun_info.stunClientResponsePort);
    printf("voip_stun_ex_info->voip_stun_info.stunNeedMask = %s\n", voip_stun_ex_info->voip_stun_info.stunNeedMask);
    printf("voip_stun_ex_info->voip_stun_info.stunServerAddress = %s\n", voip_stun_ex_info->voip_stun_info.stunServerAddress);
    printf("voip_stun_ex_info->voip_stun_info.stunServerPort = %d\n", voip_stun_ex_info->voip_stun_info.stunServerPort);
}

void voip_conf_print_conference_params(voip_conference_params_t *voip_conference_info)
{
    printf("voip_conference_info->confMode = %s\n", code2str_ex(conf_mode_t_str, voip_conference_info->confMode));
    printf("voip_conference_info->confMsAddr = %s\n", voip_conference_info->confMsAddr);
}

void voip_conf_print_cfw_params(voip_cfw_params_t *voip_cfw_info)
{
    printf("voip_cfw_info->isEnabled = %d\n", voip_cfw_info->isEnabled);
    printf("voip_cfw_info->cfwType = %s\n", code2str_ex(cfw_type_t_str, voip_cfw_info->cfwType));
    printf("voip_cfw_info->keySequence = %s\n", voip_cfw_info->keySequence);
    printf("voip_cfw_info->cfnrTimeout = %d\n", voip_cfw_info->cfnrTimeout);
}

void voip_conf_print_mwi_params(voip_mwi_params_t *voip_mwi_info)
{
    printf("voip_mwi_info->isEnabled = %d\n", voip_mwi_info->isEnabled);
    printf("voip_mwi_info->subscribe2MWI = %d\n", voip_mwi_info->subscribe2MWI);
    printf("voip_mwi_info->subsServerPort = %d\n", voip_mwi_info->subsServerPort);
    printf("voip_mwi_info->expiraitionTimeout = %d\n", voip_mwi_info->expiraitionTimeout);
    printf("voip_mwi_info->subsServerName = %s\n", voip_mwi_info->subsServerName);
}

void voip_conf_print_sip_info_key_seq_params(voip_sip_info_key_seq_t *voip_sip_info_key_seq)
{
    printf("voip_config_params.voip_sip_info_key_seq_info.sip_info_key_seq_header = %s\n", voip_sip_info_key_seq->sip_info_key_seq_header);
    printf("voip_config_params.voip_sip_info_key_seq_info.sip_info_key_seq_body = %s\n", voip_sip_info_key_seq->sip_info_key_seq_body);
}

void voip_conf_print_codec_params(voip_conf_codec_ex_t *codecs_info)
{
    int i;

    for (i = 0; i < NUMBER_OF_CODECS; i++)
    {
        if (strcmp(voip_config_params.codecs_info[i].name, "") != 0)
        {
            printf("codecs_info[%d].name = %s\n", i, codecs_info[i].name);
            printf("codecs_info[%d].bitRateHi = %d\n", i, codecs_info[i].bitRateHi);
            printf("codecs_info[%d].codec.enabled = %d\n", i, codecs_info[i].codec.enabled);
            printf("codecs_info[%d].codec.payload = %s\n", i, code2str_ex(rtp_payload_type_t_str, codecs_info[i].codec.payload));
            printf("codecs_info[%d].codec.rate = %d\n", i, codecs_info[i].codec.rate);
            printf("codecs_info[%d].codec.ptime = %d\n", i, codecs_info[i].codec.ptime);
        }
    }
}

void voip_conf_print_dsp_advanced_channel_params(voip_dsp_advanced_channel_configuration_t *voip_dsp_advanced_channel_configuration)
{
	int index;

    printf("voip_dsp_advanced_channel_configuration->IsOpenChannelDBInitialized = %d\n",
        voip_dsp_advanced_channel_configuration->IsOpenChannelDBInitialized);
    printf("voip_dsp_advanced_channel_configuration->IsActivateRTTPDBInitialized = %d\n",
        voip_dsp_advanced_channel_configuration->IsActivateRTTPDBInitialized);
    printf("voip_dsp_advanced_channel_configuration->ChannelID = %d\n",
        voip_dsp_advanced_channel_configuration->ChannelID);

	for(index=0; index<CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES; index++)
	{
   	    	printf("voip_dsp_advanced_channel_configuration->RtcpXrEnable of line %d = %d\n",
				index, voip_dsp_advanced_channel_configuration->RtcpXrEnable[index]);
		printf("voip_dsp_advanced_channel_configuration->VoiceOutputGainChannelArr of line %d = %d\n",
        			index, voip_dsp_advanced_channel_configuration->VoiceOutputGainChannelArr[index]);
	}
    printf("voip_dsp_advanced_channel_configuration->AutomaticAgcEnable = %d\n",
        voip_dsp_advanced_channel_configuration->AutomaticAgcEnable);
    printf("voip_dsp_advanced_channel_configuration->AgcLocation = %d\n",
        voip_dsp_advanced_channel_configuration->AgcLocation);
    printf("voip_dsp_advanced_channel_configuration->AgcTargetEnergy = %d\n",
        voip_dsp_advanced_channel_configuration->AgcTargetEnergy);
    printf("voip_dsp_advanced_channel_configuration->VoiceJitterBufferMinDelay = %d\n",
        voip_dsp_advanced_channel_configuration->VoiceJitterBufferMinDelay);
    printf("voip_dsp_advanced_channel_configuration->DynamicJitterBufferOptimizationFactor = %d\n",
        voip_dsp_advanced_channel_configuration->DynamicJitterBufferOptimizationFactor);
    printf("voip_dsp_advanced_channel_configuration->EchoCancelerMode = %d\n",
        voip_dsp_advanced_channel_configuration->EchoCancelerMode);
    printf("voip_dsp_advanced_channel_configuration->CngRelayEnable = %d\n",
        voip_dsp_advanced_channel_configuration->CngRelayEnable);
    printf("voip_dsp_advanced_channel_configuration->FaxMaxRate = %s\n",
        code2str_ex(fax_max_rate_t_str, voip_dsp_advanced_channel_configuration->FaxMaxRate));
    printf("voip_dsp_advanced_channel_configuration->FaxMaxBuffer = %d\n",
        voip_dsp_advanced_channel_configuration->FaxMaxBuffer);
    printf("voip_dsp_advanced_channel_configuration->FaxMaxDatagram = %d\n",
        voip_dsp_advanced_channel_configuration->FaxMaxDatagram);
    printf("voip_dsp_advanced_channel_configuration->FaxErrorCorrectionMode = %d\n",
        voip_dsp_advanced_channel_configuration->FaxErrorCorrectionMode);
    printf("voip_dsp_advanced_channel_configuration->SilenceCompressionMode = %d\n",
        voip_dsp_advanced_channel_configuration->SilenceCompressionMode);
    printf("voip_dsp_advanced_channel_configuration->PcmAdPcmComfortNoiseRtpPayloadTypeEnable = %d\n",
        voip_dsp_advanced_channel_configuration->PcmAdPcmComfortNoiseRtpPayloadTypeEnable);
    printf("voip_dsp_advanced_channel_configuration->g726_payload_number = %s\n",
        code2str_ex(rtp_payload_type_t_str, voip_dsp_advanced_channel_configuration->g726_payload_number));
    printf("voip_dsp_advanced_channel_configuration->dtmf_payload_number = %s\n",
        code2str_ex(rtp_payload_type_t_str, voip_dsp_advanced_channel_configuration->dtmf_payload_number));
    printf("voip_dsp_advanced_channel_configuration->bypassCoder = %s\n",
        code2str_ex(rtp_payload_type_t_str, voip_dsp_advanced_channel_configuration->bypassCoder));
    printf("voip_dsp_advanced_channel_configuration->faxBypassPayloadType = %d\n",
        voip_dsp_advanced_channel_configuration->faxBypassPayloadType);
    printf("voip_dsp_advanced_channel_configuration->dataBypassPayloadType = %d\n",
        voip_dsp_advanced_channel_configuration->dataBypassPayloadType);
}

void voip_conf_print_phonebook_params(voip_phonebook_record_t *curPhoneRec)
{
    while (curPhoneRec != NULL)
    {
        printf("curPhoneRec = %p\n", curPhoneRec);
        printf("&curPhoneRec->index = %p curPhoneRec->index = %d\n", &curPhoneRec->index, curPhoneRec->index);
        printf("curPhoneRec->name = %p curPhoneRec->name = %s\n", curPhoneRec->name, curPhoneRec->name);
        printf("curPhoneRec->dialString = %p curPhoneRec->dialString = %s\n", curPhoneRec->dialString, curPhoneRec->dialString);
        printf("curPhoneRec->destinationType = %p curPhoneRec->destinationType = %s\n", &curPhoneRec->destinationType, code2str_ex(voip_destination_type_t_str, curPhoneRec->destinationType));
        printf("&curPhoneRec->localLine = %p curPhoneRec->localLine = %d\n", &curPhoneRec->localLine, curPhoneRec->localLine);
        printf("curPhoneRec->userId = %p curPhoneRec->userId = %s\n", curPhoneRec->userId, curPhoneRec->userId);
        printf("curPhoneRec->userAddress = %p curPhoneRec->userAddress = %s\n", curPhoneRec->userAddress, curPhoneRec->userAddress);
        printf("&curPhoneRec->userPort = %p curPhoneRec->userPort = %d\n", &curPhoneRec->userPort, curPhoneRec->userPort);
        printf("&curPhoneRec->next = %p curPhoneRec->next = %p\n", &curPhoneRec->next, curPhoneRec->next);

        curPhoneRec = curPhoneRec->next;
    }
}

void voip_conf_print_regional_settings(voip_regional_settings_params_ex_t *reg_set_params)
{
    int i;

    printf("reg_set_params->useAppConfConfiguration = %d\n", reg_set_params->useAppConfConfiguration);
    printf("reg_set_params->selectedCountry = %s\n", reg_set_params->selectedCountry);
    printf("reg_set_params->regionalSettingsParams.CountryName = %s\n", reg_set_params->regionalSettingsParams.CountryName);
    printf("reg_set_params->regionalSettingsParams.dsp_regional_settings_params.CallerIDStandard = %d\n",
        reg_set_params->regionalSettingsParams.dsp_regional_settings_params.CallerIDStandard);

    for (i = 0; i < DSP_REGIONAL_SETTINGS_CALL_PROGRESS_TONE_LAST; i++)
    {
        printf("reg_set_params->regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[%d].enum_dsp_regional_settings_params_call_progress = %d\n", i,
            reg_set_params->regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[i].enum_dsp_regional_settings_params_call_progress);
        printf("reg_set_params->regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[%d].EnableField = %d\n", i,
            reg_set_params->regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[i].EnableField);
        printf("reg_set_params->regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[%d].ToneCadenceType = %d\n", i,
            reg_set_params->regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[i].ToneCadenceType);
        printf("reg_set_params->regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[%d].FreqA = %d\n", i,
            reg_set_params->regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[i].FreqA);
        printf("reg_set_params->regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[%d].FreqB = %d\n", i,
            reg_set_params->regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[i].FreqB);
        printf("reg_set_params->regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[%d].FreqLevelA = %d\n", i,
            reg_set_params->regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[i].FreqLevelA);
        printf("reg_set_params->regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[%d].FreqLevelB = %d\n", i,
            reg_set_params->regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[i].FreqLevelB);
        printf("reg_set_params->regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[%d].ToneOn0 = %d\n", i,
            reg_set_params->regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[i].ToneOn0);
        printf("reg_set_params->regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[%d].ToneOff0 = %d\n", i,
            reg_set_params->regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[i].ToneOff0);
        printf("reg_set_params->regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[%d].ToneOn1 = %d\n", i,
            reg_set_params->regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[i].ToneOn1);
        printf("reg_set_params->regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[%d].ToneOff1 = %d\n", i,
            reg_set_params->regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[i].ToneOff1);
        printf("reg_set_params->regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[%d].ToneOn2 = %d\n", i,
            reg_set_params->regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[i].ToneOn2);
        printf("reg_set_params->regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[%d].ToneOff2 = %d\n", i,
            reg_set_params->regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[i].ToneOff2);
        printf("reg_set_params->regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[%d].ToneOn3 = %d\n", i,
            reg_set_params->regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[i].ToneOn3);
        printf("reg_set_params->regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[%d].ToneOff3 = %d\n", i,
            reg_set_params->regionalSettingsParams.dsp_regional_settings_params.dsp_regional_settings_params_call_progressArray[i].ToneOff3);
    }
    printf("reg_set_params->regionalSettingsParams.slic_regional_settings_params.FXS_COEFFICIENTS = ");
    for (i = 0; i < REGIONAL_SETTINGS_FXS_COEFFICIENTS_PARAMS_LEN; i++)
    {
        if ((i%16) == 0)
            printf("\n");
        printf("0x%02x ", reg_set_params->regionalSettingsParams.slic_regional_settings_params.FXS_COEFFICIENTS[i]);
    }
    printf("\n");
    printf("reg_set_params->regionalSettingsParams.slic_regional_settings_params.FXS_RING_CAD_PROFILE = ");
    for (i = 0; i < REGIONAL_SETTINGS_FXS_CADENCE_PARAMS_LEN; i++)
    {
        if ((i%16) == 0)
            printf("\n");
        printf("0x%02x ", reg_set_params->regionalSettingsParams.slic_regional_settings_params.FXS_RING_CAD_PROFILE[i]);
    }
    printf("\n");
}

/***************************************************************************************/
/*   voip_conf_print_voip_config_params                                              */
/***************************************************************************************/
void voip_conf_print_voip_config_params()
{
    voip_dial_params_t                  *dialingInfo = &voip_config_params.dialingInfo;
    voip_advanced_signaling_params_t    *voip_advanced_signaling_info = &voip_config_params.voip_advanced_signaling_info;
    voip_timeout_params_t               *voip_timeouts_info = &(voip_config_params.voip_timeouts_info);

    voip_conf_print_call_mgt_params(&voip_config_params.voip_call_mngr_info);
    voip_conf_print_stun_params(&voip_config_params.voip_stun_ex_info);
    voip_conf_print_conference_params(&voip_config_params.voip_conference_info);

    /*Timeouts*/
    printf("voip_timeouts_info->dialToneTimeout = %d\n", voip_timeouts_info->dialToneTimeout);
    printf("voip_timeouts_info->offhookToneTimeout = %d\n", voip_timeouts_info->offhookToneTimeout);
    printf("voip_timeouts_info->stutterToneDuration = %d\n", voip_timeouts_info->stutterToneDuration);
    printf("voip_timeouts_info->unansweredCallTimeout = %d\n", voip_timeouts_info->unansweredCallTimeout);
    printf("voip_timeouts_info->warningToneTimeout = %d\n", voip_timeouts_info->warningToneTimeout);

    voip_conf_print_cfw_params(&voip_config_params.voip_cfw_info);
    voip_conf_print_mwi_params(&voip_config_params.voip_mwi_info);
    voip_conf_print_sip_info_key_seq_params(&voip_config_params.voip_sip_info_key_seq_info);
    voip_conf_print_codec_params(voip_config_params.codecs_info);

    printf("dialingInfo->dialCompleteKeyEnabled = %d\n", dialingInfo->dialCompleteKeyEnabled);
    printf("dialingInfo->dialCompleteKey = %c\n", dialingInfo->dialCompleteKey);
    printf("dialingInfo->dialStrMaxSize = %d\n", dialingInfo->dialStrMaxSize);
    printf("dialingInfo->dialTimeout = %d\n", dialingInfo->dialTimeout);
    printf("dialingInfo->digitMap = %s\n", dialingInfo->digitMap);

    /*Regional settings*/
    voip_conf_print_regional_settings(&voip_config_params.regionalSettingsInfo);

    /*Advanced signaling info*/
    printf("voip_advanced_signaling_info->blindTransferEnabled = %d\n", voip_advanced_signaling_info->blindTransferEnabled);
    printf("voip_advanced_signaling_info->includePTime = %d\n", voip_advanced_signaling_info->includePTime);
    printf("voip_advanced_signaling_info->kaEnabled = %d\n", voip_advanced_signaling_info->kaEnabled);
    printf("voip_advanced_signaling_info->kaTimeout = %d\n", voip_advanced_signaling_info->kaTimeout);
    printf("voip_advanced_signaling_info->prackEnabled = %d\n", voip_advanced_signaling_info->prackEnabled);
    printf("voip_advanced_signaling_info->rportEnabled = %d\n", voip_advanced_signaling_info->rportEnabled);
    printf("voip_advanced_signaling_info->mssClampingEnabled = %d\n", voip_advanced_signaling_info->mssClampingEnabled);

    voip_conf_print_dsp_advanced_channel_params(&voip_config_params.voip_dsp_advanced_channel_configuration);

    voip_conf_print_phonebook_params(voip_config_params.phonebook);

    printf("voip_config_params.faxTransportMode = %s\n", code2str_ex(t38_transport_mode_t_str, voip_config_params.faxTransportMode));
    printf("voip_config_params.dataTransportMode = %s\n", code2str_ex(data_transport_mode_t_str, voip_config_params.dataTransportMode));
    printf("voip_config_params.outOfServiceTone = %s\n", code2str_ex(out_of_service_tone_t_str, voip_config_params.outOfServiceTone));
    printf("voip_config_params.polarityRevasalEnabled = %d\n", voip_config_params.polarityRevasalEnabled);
    printf("voip_config_params.numberOfFxsLines = %d\n", voip_config_params.numberOfFxsLines);
    printf("voip_config_params.numberOfFxoLines = %d\n", voip_config_params.numberOfFxoLines);

}

/***************************************************************************************/
/*   voip_conf_print_debug_information_params                                           */
/***************************************************************************************/
void voip_conf_print_debug_information_params()
{
    printf("voip_config_params.voip_debug_information_info.enable_packet_recording = %d\n", voip_config_params.voip_debug_information_info.enable_packet_recording);
    printf("voip_config_params.voip_debug_information_info.enable_network_recording = %d\n", voip_config_params.voip_debug_information_info.enable_network_recording);
    printf("voip_config_params.voip_debug_information_info.enable_tdm_recording = %d\n", voip_config_params.voip_debug_information_info.enable_tdm_recording);
    printf("voip_config_params.voip_debug_information_info.channel = %d\n", voip_config_params.voip_debug_information_info.channel);
    printf("voip_config_params.voip_debug_information_info.remoteAddr = %s\n", voip_config_params.voip_debug_information_info.remoteAddr);
    printf("voip_config_params.voip_debug_information_info.remotePort = %d\n", voip_config_params.voip_debug_information_info.remotePort);
}

/***************************************************************************************/
/*   voip_conf_print_system_config_params                                              */
/***************************************************************************************/
void voip_conf_print_system_config_params()
{
    printf("system_config_params.dbgEnabled = %d\n", system_config_params.dbgEnabled);
    printf("system_config_params.local_ip = %s\n", system_config_params.local_ip);
    printf("system_config_params.timeZoneOffset = %d\n", system_config_params.timeZoneOffset);
    printf("system_config_params.wan_mac_addr = %s\n", system_config_params.wan_mac_addr);
    printf("system_config_params.system_log_info.rvLogFilter = %d\n", system_config_params.system_log_info.rvLogFilter);
    printf("system_config_params.system_log_info.SIPLogLevel = %d\n", system_config_params.system_log_info.SIPLogLevel);
    printf("system_config_params.system_log_info.serverAddrForAppSyslog = %s\n", system_config_params.system_log_info.serverAddrForAppSyslog);
    printf("system_config_params.system_log_info.serverPortForAppSyslog = %d\n", system_config_params.system_log_info.serverPortForAppSyslog);
    printf("system_config_params.system_log_info.udpOrTerminalFlag = %d\n", system_config_params.system_log_info.udpOrTerminalFlag);
}

void voip_conf_test()
{
    acl_call_mngr_params_t                      callMngrParams;
    line_conf_t                                 lineConf[2];
    voip_regional_settings_params_ex_t          voipRegionalSettings;
    voip_dsp_advanced_channel_configuration_t   advancedChannelConfiguration;
    voip_mwi_params_t                           mwiParams;
    voip_cfw_params_t                           cfwParams;
    voip_conf_codec_ex_t                        codecinfo[NUMBER_OF_CODECS];
    voip_stun_params_ex_t                       stunExParms;

    regional_settings_params_t *regionalSettings = NULL;
    char ipAddress[AC_DOTTED_IP_LEN];
    char testKeySequence [MAX_KEY_SEQ_STRING_LEN + 1];
    char userAgent[MAX_USER_AGENT_LEN+1];

    char *cfwDest = NULL;
    char *conferenceAddr = NULL;
    char *conferenceMacAddr = NULL;
    char *serverAddrForAppSyslog = NULL;
    char *userAddr = NULL;
    char *userId = NULL;

    voip_dial_match_t dialMtach;

    int pG723Bitrate;
    int faxDataMethod;
    int bypassCoder;
    int toneTimeout;
    int unansweredCallTimeout;
    int stutterToneDur;
    int outOfServiceTone;
    int logLevel;
    char prackIsEnable;
    char polarityRevasalIsEnable;
    int faxMaxRate;
    int faxMaxBuffer;
    int faxMaxDatagram;
    char IncludePtime;
    char rportIsEnable;
    char kaIsEnable;
    int kaTimeout;
    int remoteConference;
    char btIsEnable;
    int rvLogFilter;
#ifdef RV_CFLAG_TLS
    int isTransportProtocolTls;
#endif /*RV_CFLAG_TLS*/
    char stunIsEnable;
    int serverPortForAppSyslog;
    int udpTerminalFlag;
    long timerDuration;
    int i;


	memset(&callMngrParams, 0, sizeof(callMngrParams));
	memset(&advancedChannelConfiguration, 0, sizeof(advancedChannelConfiguration));
	memset(&stunExParms, 0, sizeof(stunExParms));
	memset(&mwiParams, 0, sizeof(mwiParams));
	memset(&voipRegionalSettings, 0, sizeof(voipRegionalSettings));

    callMngrParams.lines = lineConf;
    /*
    voip_dial_match_t voip_conf_match_number(char *dial_string, long *timer_duration, int is_registered_with_proxy);
    void voip_conf_map_number(char *dial_string, char **user_id, char **user_addr);
    */
    voip_conf_local_address_get(ipAddress);
    printf("ipAddress = %s\n", ipAddress);

    /*void voip_conf_localtime_get(&tm_time);*///NIRP 070111

    for (i = 0; i < CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES; i++)
    {
        voip_conf_line_conf_get(i, &lineConf[i]);

        printf("lineConf.enabled = %d\n", lineConf[i].enabled);
        printf("lineConf.display_name = %s\n", lineConf[i].display_name);
        printf("lineConf.id = %s\n", lineConf[i].id);
        printf("lineConf.send_caller_id = %d\n", lineConf[i].send_caller_id);
        printf("lineConf.pswdServerAuth = %s\n", lineConf[i].pswdServerAuth);
        printf("lineConf.usernameServerAuth = %s\n", lineConf[i].usernameServerAuth);
    }
    voip_conf_signalling_conf_get(&callMngrParams);
    voip_conf_print_call_mgt_params(&callMngrParams);

    for (i = 0; i < NUMBER_OF_CODECS; i++)
    {
        voip_conf_codec_info_get(i, &codecinfo[i].codec);
    }
    voip_conf_print_codec_params(codecinfo);

    /*void voip_conf_dynamic_payloads_info_get(void);*///Nirp 060709 configurable payload type - update DTMF payload type parameters
    voip_conf_dialing_param_get(&callMngrParams);/* ACL NB 070528 get all dialing parameters*/

    printf("is_auto_dial = %d\n", callMngrParams.is_auto_dial);
    printf("auto_dial_timeout = %d\n", callMngrParams.auto_dial_timeout);
    printf("auto_dial_dest = %s\n", callMngrParams.auto_dial_dest);
    printf("use_secondary_dial_tone = %d\n", callMngrParams.use_secondary_dial_tone);
    printf("secondary_dial_tone_key = %d\n", callMngrParams.secondary_dial_tone_key);

    //nirp regional
    /*voip_conf_regional_settings_find_country_in_DB(int* pIndex, char* CountryName);*/

    voip_conf_g723_high_bitrate_get(&pG723Bitrate);/* ACL NIRP 061224 */
    printf("pG723Bitrate = %d\n", pG723Bitrate);

    voip_conf_faxDataMethod_get(&faxDataMethod); /* ACL NB 06*/
    printf("faxDataMethod = %d\n", faxDataMethod);

    voip_conf_bypassCoder_get(&bypassCoder);     /* ACL NB 06*/
    printf("bypassCoder = %s\n", code2str_ex(rtp_payload_type_t_str, bypassCoder));

    toneTimeout = voip_conf_dialtone_timeout_get();/* Nirp 060720 */
    printf("dialToneCallTimeout = %d\n", toneTimeout);

    toneTimeout = voip_conf_tone_timeout_get(VOIP_SIGNAL_REORDER_TONE);
    printf("reordertoneTimeout = %d\n", toneTimeout);
    toneTimeout = voip_conf_tone_timeout_get(VOIP_SIGNAL_HOOK_OFF_TONE);
    printf("offhookCallTimeout = %d\n", toneTimeout);
    toneTimeout = voip_conf_tone_timeout_get(VOIP_SIGNAL_DIAL_TONE);
    printf("dialToneCallTimeout = %d\n", toneTimeout);

    unansweredCallTimeout = voip_conf_unansweredCall_timeout_get();/* ACL NB 070718 */
    printf("unansweredCallTimeout = %d\n", unansweredCallTimeout);

    voip_conf_dsp_params_get(&regionalSettings, &advancedChannelConfiguration);
    memcpy(&voipRegionalSettings.regionalSettingsParams, regionalSettings, sizeof(regional_settings_params_t));
    voip_conf_print_regional_settings(&voipRegionalSettings);
    voip_conf_print_dsp_advanced_channel_params(&advancedChannelConfiguration);

    voip_conf_get_advanced_channel_configuration(&advancedChannelConfiguration);
    voip_conf_print_dsp_advanced_channel_params(&advancedChannelConfiguration);

    voip_conf_mwi_get(&mwiParams);
    voip_conf_print_mwi_params(&mwiParams);
        /* ACL NB 061210 */
    voip_conf_cfw_type_get(&cfwParams);
    voip_conf_print_cfw_params(&cfwParams);

    voip_conf_stutter_tone_dur_get(&stutterToneDur);
    printf("stutterToneDur = %d\n", stutterToneDur);

    voip_conf_cfw_keySequence_get(testKeySequence);
    printf("keySequence = %s\n", testKeySequence);

    voip_conf_out_of_service_tone_get(&outOfServiceTone);
    printf("outOfServiceTone = %d\n", outOfServiceTone);

    voip_conf_ua_get(userAgent);
    printf("userAgent = %s\n", userAgent);

    voip_conf_write_ext(82);

    voip_conf_voip_log_lvl_get(&logLevel);
    printf("logLevel = %d\n", logLevel);

    voip_conf_ext_cfw_dest_set(5, "10.16.2.56");
    voip_conf_ext_cfw_dest_get(5, &cfwDest);
    printf("cfwDest = %s\n", cfwDest);

    voip_conf_ext_sync_check_set(0);
    voip_conf_ext_sync_check_set(1);

    voip_conf_prack_enable_get(&prackIsEnable);
    printf("prackIsEnable = %d\n", prackIsEnable);

    voip_conf_fxs_polarity_revasal_enable_get(&polarityRevasalIsEnable);
    printf("polarityRevasalIsEnable = %d\n", polarityRevasalIsEnable);

    voip_conf_fax_max_rate_get(&faxMaxRate);
    printf("faxMaxRate = %s\n", code2str_ex(fax_max_rate_t_str, faxMaxRate));

    voip_conf_fax_max_buffer_get(&faxMaxBuffer);
    printf("faxMaxBuffer = %d\n", faxMaxBuffer);

    voip_conf_fax_max_buffer_get(&faxMaxDatagram);
    printf("faxMaxDatagram = %d\n", faxMaxDatagram);

    voip_conf_get_sdp_include_ptime(&IncludePtime);
    printf("IncludePtime = %d\n", IncludePtime);

    voip_conf_rport_enable_get(&rportIsEnable);
    printf("rportIsEnable = %d\n", rportIsEnable);

    voip_conf_ka_options_enabled_get(&kaIsEnable);
    printf("kaIsEnable = %d\n", kaIsEnable);
    voip_conf_ka_options_timeout_get(&kaTimeout);
    printf("kaTimeout = %d\n", kaTimeout);

    voip_conf_remote_conference_get(&remoteConference);
    printf("remoteConference = %d\n", remoteConference);

    voip_conf_remote_conference_conf_addr_get(&conferenceAddr);
    printf("conferenceAddr = %s\n", conferenceAddr);

    conferenceMacAddr = voip_conf_remote_conference_mac_addr_get();
    printf("conferenceMacAddr = %s\n", conferenceMacAddr);

    voip_conf_blind_transfer_enable_get(&btIsEnable);
    printf("btIsEnable = %d\n", btIsEnable);

    rvLogFilter = voip_conf_rv_log_filter_get();
    printf("rvLogFilter = %d\n", rvLogFilter);

#ifdef RV_CFLAG_TLS
    isTransportProtocolTls = voip_conf_is_transport_protocol_tls();
    printf("isTransportProtocolTls = %d\n", isTransportProtocolTls);
#endif /*RV_CFLAG_TLS*/

    voip_conf_stun_enable_get(&stunIsEnable);
    printf("stunIsEnable = %d\n", stunIsEnable);

    voip_conf_stun_get(&(stunExParms.voip_stun_info));
    voip_conf_print_stun_params(&stunExParms);
    voip_conf_syslog_server_info_get(&serverPortForAppSyslog, &serverAddrForAppSyslog, &udpTerminalFlag);
    printf("serverPortForAppSyslog = %d\n", serverPortForAppSyslog);
    printf("serverAddrForAppSyslog = %s\n", serverAddrForAppSyslog);
    printf("udpTerminalFlag = %d\n", udpTerminalFlag);



    dialMtach = voip_conf_match_number("9", &timerDuration, 0);
    printf("dialString = %s dialMtach = %d timerDuration = %ld\n", "9", dialMtach, timerDuration);
    dialMtach = voip_conf_match_number("94", &timerDuration, 0);
    printf("dialString = %s dialMtach = %d timerDuration = %ld\n", "94", dialMtach, timerDuration);
    dialMtach = voip_conf_match_number("944", &timerDuration, 0);
    printf("dialString = %s dialMtach = %d timerDuration = %ld\n", "944", dialMtach, timerDuration);
    dialMtach = voip_conf_match_number("9445", &timerDuration, 0);
    printf("dialString = %s dialMtach = %d timerDuration = %ld\n", "9445", dialMtach, timerDuration);
    dialMtach = voip_conf_match_number("8523423423423423423423423423424234", &timerDuration, 0);
    printf("dialString = %s dialMtach = %d timerDuration = %ld\n", "8523423423423423423423423423424234", dialMtach, timerDuration);
    dialMtach = voip_conf_match_number("9", &timerDuration, 1);
    printf("dialString = %s dialMtach = %d timerDuration = %ld\n", "9", dialMtach, timerDuration);
    dialMtach = voip_conf_match_number("94", &timerDuration, 1);
    printf("dialString = %s dialMtach = %d timerDuration = %ld\n", "4", dialMtach, timerDuration);
    dialMtach = voip_conf_match_number("944", &timerDuration, 1);
    printf("dialString = %s dialMtach = %d timerDuration = %ld\n", "44", dialMtach, timerDuration);
    dialMtach = voip_conf_match_number("9445", &timerDuration, 1);
    printf("dialString = %s dialMtach = %d timerDuration = %ld\n", "445", dialMtach, timerDuration);
    dialMtach = voip_conf_match_number("8523423423423423423423423423424234", &timerDuration, 1);
    printf("dialString = %s dialMtach = %d timerDuration = %ld\n", "8523423423423423423423423423424234", dialMtach, timerDuration);

    voip_conf_map_number("9", &userId, &userAddr);
    printf("dialString = %s userId = %s userAddr = %s\n", "9", userId, userAddr);

    free(userId);
    free(userAddr);
    voip_conf_map_number("94", &userId, &userAddr);
    printf("dialString = %s userId = %s userAddr = %s\n", "94", userId, userAddr);

    free(userId);
    free(userAddr);
    voip_conf_map_number("944", &userId, &userAddr);
    printf("dialString = %s userId = %s userAddr = %s\n", "944", userId, userAddr);

    free(userId);
    free(userAddr);
    voip_conf_map_number("9445", &userId, &userAddr);
    printf("dialString = %s userId = %s userAddr = %s\n", "9445", userId, userAddr);

    free(userId);
    free(userAddr);
    voip_conf_map_number("93", &userId, &userAddr);
    printf("dialString = %s userId = %s userAddr = %s\n", "93", userId, userAddr);

    free(userId);
    free(userAddr);
    voip_conf_map_number("943", &userId, &userAddr);
    printf("dialString = %s userId = %s userAddr = %s\n", "943", userId, userAddr);

    free(userId);
    free(userAddr);
    voip_conf_map_number("976", &userId, &userAddr);
    printf("dialString = %s userId = %s userAddr = %s\n", "976", userId, userAddr);

    free(userId);
    free(userAddr);
    voip_conf_map_number("2", &userId, &userAddr);
    printf("dialString = %s userId = %s userAddr = %s\n", "2", userId, userAddr);

    free(userId);
    free(userAddr);
    voip_conf_map_number("85", &userId, &userAddr);
    printf("dialString = %s userId = %s userAddr = %s\n", "8523423423423423423423423423424234", userId, userAddr);

    free(userId);
    free(userAddr);

}
