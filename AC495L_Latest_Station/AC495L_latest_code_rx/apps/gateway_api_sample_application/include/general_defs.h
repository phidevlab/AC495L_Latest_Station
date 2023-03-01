/********************************************
 * AudioCodes improved application                        	*
 *                                                                 			*
 * File Name:	genral_defs.h                            	        *
 *                                                                  			*
 * Copyright (C) 2006 AudioCodes Ltd.				*
 *											*
 * Modifications :								*
 *											*
 * 30/10/06 - Coding started.					*
 *											*
 ********************************************/
/*******************************************
*
* 		DESCRIPTION:
*
********************************************/

#ifndef	__GENERAL_DEFS_h
#define	__GENERAL_DEFS_h



#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "acGatewayDriversDefs.h"   	/* AudioCodes Gateway Definitions */
#include "acGatewayDefs.h"
#if defined(AC49_SMB_VOIP_TOOLKIT) || defined(AC49_ORCHID_ATA_VOIP_TOOLKIT)
#define NUMBER_OF_FXS_LINES			8
#elif defined(AC494_TULIP2_40) || defined(AC494_TULIP2_21)
#define NUMBER_OF_FXS_LINES			4
#else
#define NUMBER_OF_FXS_LINES			2
#endif

#define MEDIA_BASE_LOCAL_PORT		4000

#define ON_HOOK		0
#define OFF_HOOK	1

#define ACL_ADDRESS_STR_MAX_LEN	128
#define ACL_MSG_SIZE					1500


#define MAX_CMD_LEN			40
#define MAX_DESC_LEN		140
#define MAX_SYNTAX_LEN		1270
#define MAX_ARGS			20
#define EXIT_CODE			99
#define MAX_ARG_LEN			64
#define MAX_USAGE_LEN		128

#define MAX_CMD_LINE_LEN		200

#define CONFIGURATION_FILE_NAME_SIZE	128
#define CONFIGURATION_FILE_NAME         "VoIPCfgFile_Proxy_improved_appl.cfg.txt"
#define CONFIGURATION_BUFFER_MAX_LEN 	40960

#ifdef S_PLATFORM

#define FIRMWARE_PATH					"/tmp"

#if defined(AC494_SDP_IPPHONE) || defined(AC494_SDP_ATA)
//#define DSP_FIRMWARE_NAME				FIRMWARE_PATH"494003ce4.680.07"
#define DSP_FIRMWARE_NAME				FIRMWARE_PATH"/494002ce4.720.20"
#elif defined(AC49_ORCHID_VOIP_TOOLKIT)
//#define DSP_FIRMWARE_NAME       	    FIRMWARE_PATH"/494E002ce4.720.25"
#define DSP_FIRMWARE_NAME				FIRMWARE_PATH"/494002ce4.720.20"
#elif defined(AC49_ORCHID_ATA_VOIP_TOOLKIT)
#define DSP_FIRMWARE_NAME       	    FIRMWARE_PATH"/494E004ce2.680.07"
#else
#define DSP_FIRMWARE_NAME				FIRMWARE_PATH"/494004ce3.680.07"
#endif
#define SECOND_DSP_FIRMWARE_NAME        FIRMWARE_PATH"/498004ce1.680.07"

#endif /* S_PLATFORM */
/********************************************
***********	          Macros               ***************
*********************************************/
#define nfree(buff) 		free(buff)

/********************************************
***********	          Prototypes         ***************
*********************************************/

void *AC_ZALLOC_L(size_t size);

/********************************************
********		     Enums            ***************
*********************************************/
typedef enum {

    CONN_MODE_LOCAL,
    CONN_MODE_REMOTE,

} conection_mode_t;




/********************************************
********		     Structures            *************
*********************************************/
typedef struct appl_code2code_s
{
   	int code;
   	int val;
} appl_code2code_t;


typedef struct
{
	int remoteChannel;

} localModeChannelMapping_t;


typedef struct
{
	int  port;
	char address[ACL_ADDRESS_STR_MAX_LEN];
} ipAddrInfo_s;


typedef struct
{
	int   	        localPort;
	int   	        rtpSfd;
	int   	        rtcpSfd;
//    	acl_call_t    *pCall;
	ipAddrInfo_s	remoteNetTermInfo;
	int		remoteDtmfPayloadNumber;
	int		remoteCurrentCodecPayloadNumber;
} socketInfo_s;

typedef struct {

	int fxs_line;
	int conferenceMate;
	int is_channel_open;
	int is_rtp_activated;

} channel_t;

typedef struct
{
	int is_default_init_device_parameters;
	int is_dsp_device_initialized;
	int is_slic_device_initialized;
	int is_default_channel_parameters;

} ApplStatus;


typedef struct
{
	char name[MAX_CMD_LEN];
	char description[MAX_DESC_LEN];
	char syntax[MAX_SYNTAX_LEN];
	int (*func)(int argc, char* argv[]);
} CmdList;

typedef enum {
    /*    Signaling cmds*/
    E_DTMFTransferMode=1,
	E_CPTTransferMode,
    E_DetectCallProgress,
    E_DetectDTMFRegister,
    E_IBSRedirection,
    E_IBSLevel,
    E_DetectLongDTMF,
    /*    Gain cmds*/
    E_InputGain,
    E_VoiceOutputGain,
    /*g_cCallerID cmds*/
    E_CIDOnHook,
    E_CIDOffHook,
    E_CIDStandard,
    /*   JitterBuffer cmds*/
    E_VoiceJBMin,
    E_VoiceJBMax,
    E_JBOptimizationFactor,
    /*EchoCanceller cmds*/
    E_ECNLP,
    E_ECMode,
    E_EchoCancellerLength,
    /*FaxModem cmds*/
    E_FaxMethod,
    E_FaxErrorCorrectionMode,
    E_EnableCNG,
    E_FaxMaxRate,
    E_CEDTransferMode,
    E_V34TransferMethod,
    E_V32TransferMethod,
    E_V23TransferMethod,
    E_V22TransferMethod,
    E_V21TransferMethod,
    E_Bell103TransferMethod,
    E_V34FaxTransferMethod,
    E_BypassCoder,
    E_BypassJitter,
    E_DataBypassOutputGain,
    E_FaxBypassOutputGain,
    /*RTPStream cmds*/
    E_Coder,
    E_PTime,
    E_SilenceCompressionMode,
    E_FaxBypassPT,
    E_ModemBypassPT,
    E_IBSRedundancy,
    E_RTPRedundancy,
    E_RFC2833TxPT,
    E_RFC2833RxPT,
    E_RFC2198PT,
    E_RTCPMeanTxInterval,
    E_CName,
    E_TxRtcpFilter,
    E_RxRtcpFilter,
    /*Miscellaneous*/
    E_TestMode,

    E_ERROR,
} sub_cmd_t;

typedef struct
{
    sub_cmd_t cmd_type;
	char subgroup[MAX_CMD_LEN];
	char param[MAX_CMD_LEN];
	char usage[MAX_USAGE_LEN];
} GroupCmdList;

/*******************************PAGING*****************************************************/

#define PAGING_BUFF_LEN 1024

#define PAGING_END_TIMEOUT	1500  // the time we find stream lost.
#define PAGING_START_TIMEOUT	50  // every time we wait and check all call is disconnect or not when try to paging outgoing.
#define PAGING_PTIME        20

#define UNUSED(a) a=a

#define NUM_OF_PAGING 1
#define PAGING_IP_LENGTH 16
#define MAX_PAGING_STREAM 10

/* this is for outside config define, define it as your wish.
   save the config yourself and define the function:
   paging_get_config()
   to return it for thread use.
*/

typedef struct
{
      unsigned char V:2;
	  unsigned char P:1;
	  unsigned char X:1;
	  unsigned char CC:4;
	  unsigned char PT:7;
	  unsigned char M:1;
	  unsigned short SN;
	  unsigned int TimeStamp;
	  unsigned int SSRC;
} RTPHeader;

typedef struct
{
   unsigned int bits;
   unsigned int ssrc;
} RTCPHeader;

typedef enum{
	P_IDLE,
	P_INCOMING_WAITING,
	P_INCOMING_ONGOING,
	P_OUTGOING_WAITING,
	P_OUTGOING_ONGOING,
	P_STATE_MAX
}PAGINGSTATE;

typedef struct
{
    char    enable;
    char    address[PAGING_IP_LENGTH];
    int     port;
}PAGING_GROUP;

typedef struct
{
    char    enable;
    int     payload;
    PAGING_GROUP group[NUM_OF_PAGING];
}PAGING_CONFIG;

typedef struct
{
    unsigned int     SSRC;
    unsigned int     group;
    unsigned int     count;
    unsigned int     timer_record_count;
    char             ignore;
}PAGING_STREAM;

typedef struct
{
    fd_set           pagingRtpReceivingFds;
    int              pagingRtpMaxFd;
    int              pagingFdDB[110]; //RTP/RTCP
    int              pagingFdDBRealSize;
    char	         pagingBuff[PAGING_BUFF_LEN];
    pthread_t        pagingTid;

    unsigned int     currentSSRC;
    unsigned int     currentGID;
    PAGINGSTATE      state;
    PAGING_STREAM    stream[MAX_PAGING_STREAM];
}PAGING_WORKING;

typedef enum
{
    E_PAGING_OUTGOING_LINE,
    E_PAGING_OUTGOING_START,
    E_PAGING_INCOMING_START,
    E_PAGING_INCOMING_TIMEOUT,
    E_PAGING_INCOMING_IGNORE,
    E_PAGING_INCOMING_SWITCH,
    E_PAGING_END
}PAGING_EVENT;

typedef struct {
   char  isEnabled;
   char  address[NUM_OF_PAGING][PAGING_IP_LENGTH];
   int   port[NUM_OF_PAGING];
} voip_paging_params_t;

/* function internal */


/* function external */














/******************************PAGING******************************************************/



#endif /* __GENERAL_DEFS_h */
