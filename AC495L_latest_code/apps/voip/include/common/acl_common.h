/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	acl_common.h                                        *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 26/02/08 - Coding started.                                       *
 *																	*
 ********************************************************************/

#ifndef _COMMON_H_
#define _COMMON_H_

#include <enums.h>
#include "voip_gen_defs.h"
#include "iphone_paging_pub.h"
/**********************/
/** Should be moved *****/
/*********************/


#define MAX_KEY_SEQ_STRING_LEN 3 // ACL NB 061210 
#define MAX_USER_AGENT_STR_SIZE 128  // ACL NB 061226

#define MAX_CALLER_ID_SIZE   20 /* ID lenght is 19 plus zero termination */
#define MAX_USENAME_STR_SIZE 128 /* same as  RV_NAME_STR_SZ */
#define MAX_PSWD_STR_SIZE    128 /* same as  RV_NAME_STR_SZ */

#define AUDIO_CODEC_RATE 8000
/* AlexChen 20090331 SDK upgrade */
#define MAX_DIGITMAP_STRLEN   2048  /*quoting from rfc3435 it is RECOMMENDED that gateways support 
                                                                  digit maps up to at least 2048 bytes per endpoint*/
typedef enum { 
    VOIP_SIGNAL_UNKNOWN = -1,

    /* tone signals need to be contiguous for IS_TONE_SIGNAL */
    VOIP_SIGNAL_DIAL_TONE = 0,
    VOIP_SIGNAL_RING_TONE = 1,
    VOIP_SIGNAL_BUSY_TONE = 2,
    VOIP_SIGNAL_HOOK_OFF_TONE = 3,
    VOIP_SIGNAL_WARNING_TONE = 4,
    VOIP_SIGNAL_CALLER_WAITING_TONE = 5,        /* ACL NB 060607*/
    VOIP_SIGNAL_CALLER_WAITING_RINGBACK_TONE=6, /* ACL NB 060607*/
    VOIP_SIGNAL_STUTTER_TONE = 7,               /* ACL NB 070102*/         
    VOIP_SIGNAL_REORDER_TONE = 8,
    VOIP_SIGNAL_CALLER_WAITING_TONE_2 = 9,        /* ACL NB 060607*/
    
    VOIP_SIGNAL_RING = 10,
    VOIP_SIGNAL_INDICATOR = 11,
    VOIP_SIGNAL_CALLER_ID = 12,
    VOIP_SIGNAL_LINE_ACTIVE = 13,
    VOIP_SIGNAL_LINE_INACTIVE = 14,
    VOIP_SIGNAL_DTMF_TONE     =15, /* ACL NB 060607*/
    VOIP_SIGNAL_MWI           =16, /* ACL NB 061008*/
    VOIP_SIGNAL_RING_SPLASH   = 17
} voip_signal_t;

/* AlexChen for SDK upgrade */
typedef enum 
{
    DIGIT_0_MASK  = 0x00000001,
    DIGIT_1_MASK  = 0x00000002,
    DIGIT_2_MASK  = 0x00000004,
    DIGIT_3_MASK  = 0x00000008,
    DIGIT_4_MASK  = 0x00000010,
    DIGIT_5_MASK  = 0x00000020,
    DIGIT_6_MASK  = 0x00000040,
    DIGIT_7_MASK  = 0x00000080,
    DIGIT_8_MASK  = 0x00000100,
    DIGIT_9_MASK  = 0x00000200,
    DIGIT_A_MASK  = 0x00000400,
    DIGIT_B_MASK  = 0x00000800,
    DIGIT_C_MASK  = 0x00001000,
    DIGIT_D_MASK  = 0x00002000,
    DIGIT_S_MASK  = 0x00004000,
    DIGIT_O_MASK  = 0x00008000
} digit_mask_t; 

typedef struct {
    char *indid; /* this is a reference only, used immediately - no need for
		  * allocation or freeing. */ 
    int state_on;
    int distRing;/* ACL NB 070506 - added distinctive ring feature */
} voip_siginfo_ind_t;

/* ACL NB 070506 - added distinctive ring feature */
typedef struct {
    int distRing;
} voip_siginfo_dist_ring_t;

typedef struct {
    char *number; /* References only. Copy to buffer synchronuously */
    char *name;
    char *address;
} voip_siginfo_cid_t;

typedef struct {
    char *name; /* References only */
    int line_id;
} voip_siginfo_la_t;

/* ACL NB 060720 - support DTMF via SIP*/
typedef struct {
    int id; 
    int duration;
} voip_siginfo_gd_t;

/* ACL NB 061123 support MWI*/
typedef struct {
    int state_on;
} voip_siginfo_msg_t;

typedef enum {
	
    ILLEGAL_PHONE_INPUT         = -1,
	
    /* The digit keys */
    PHONE_INPUT_0         = 0,
    PHONE_INPUT_1         = 1,
    PHONE_INPUT_2         = 2,
    PHONE_INPUT_3         = 3,
    PHONE_INPUT_4         = 4,
    PHONE_INPUT_5         = 5,
    PHONE_INPUT_6         = 6,
    PHONE_INPUT_7         = 7,
    PHONE_INPUT_8         = 8,
    PHONE_INPUT_9         = 9,
    /* Other keys */
    PHONE_INPUT_ASTERISK  = 10, /* "*" key */
    PHONE_INPUT_POUND     = 11, /* "#" key */

	/* Eitan1207 - Add support for DTMF A-D */
    PHONE_INPUT_A         = 12, /* "A" key */
    PHONE_INPUT_B         = 13, /* "B" key */
    PHONE_INPUT_C         = 14, /* "C" key */
    PHONE_INPUT_D         = 15, /* "D" key */

    PHONE_INPUT_HOOK_ON   = 17,
    PHONE_INPUT_HOOK_OFF  = 18,
    PHONE_INPUT_SETTINGS  = 19,
    PHONE_INPUT_HOLD      = 20,
    PHONE_INPUT_TRANSFER  = 21,
    PHONE_INPUT_CONF      = 22,
    PHONE_INPUT_FLASH     = 23,
    PHONE_INPUT_CFW       = 24,/* ACL NB 061113 call forward*/ 

    PHONE_INPUT_FAX_DETECT       = 25, /* ACL NB 060404 It is not a key but it is an input event */
    PHONE_INPUT_FAX_BYPASS_DETECT= 26, /* ACL NB 060704 It is not a key but it is an input event */
    PHONE_INPUT_SWITCH2VOICE_DETECT= 27, /* ACL NB 070806 It is not a key but it is an input event */
    PHONE_INPUT_STUN_RES  = 28,          /* ACL NB 071202 It is not a key but it is an input event */ 

    PHONE_INPUT_BLINDT    = 29,/* ACL NB 070507  Blind Transfer */ 
    PHONE_INPUT_DAA_RING_START= 30, /* NIRP 070527 It is not a key but it is an input event */ 
    PHONE_INPUT_DAA_RING_STOP= 31, /* NIRP 070527 It is not a key but it is an input event */  
    PHONE_INPUT_DAA_FLASH_HOOK_RELAY= 32, /* NIRP 070607 */
/* NirS - Add support for IPP */
	PHONE_INPUT_LINE_1,
	PHONE_INPUT_LINE_2,
	PHONE_INPUT_SPEAKER,
	PHONE_INPUT_HEADSET,
	PHONE_INPUT_DND = 37,	/* AlexChen SDK upgrade dnd feature */
	PHONE_INPUT_PAGING_1,
	PHONE_INPUT_PAGING_2,
	PHONE_INPUT_PAGING_LINE_MAX = 47,
	PHONE_INPUT_PAGING_IGNORE,
	PHONE_INPUT_PAGING_SWITCH,
	PHONE_INPUT_PAGING_MAX = 50
} phone_input_event_t;

typedef struct {
    int enabled;
    char id[MAX_CALLER_ID_SIZE];
    char display_name[255];
    int send_caller_id;
    /* ACL NB 060322 */
    char usernameServerAuth[MAX_USENAME_STR_SIZE]; /*ACL NB username for server authentication */
    char pswdServerAuth[MAX_PSWD_STR_SIZE];        /*ACL NB password for server authentication */
} line_conf_t;

typedef struct {
    int ring_mute;
    int ring_vol;
    int handset_vol;
    int loudspeaker_vol;
} voip_volume_params_t;

typedef struct {
    rtp_payload_type_t payload;
    int rate;
    int ptime;
    int enabled;
    int g722_bit_rate;
} codec_t;
/* ACL NB 061203 - MWI configuration */
typedef struct {
   char  isEnabled;
   char  subscribe2MWI;
   char  subsServerName[AC_MAX_HOST_NAME_LEN + 1];
   u16   subsServerPort;
   int   expiraitionTimeout;
   
} voip_mwi_params_t;

/* ACL NB 061203 - MWI configuration */
typedef struct {
	char cfw_dest[AC_MAX_DOMAIN_NAME_LEN + 1];
}CFW_DEST;

typedef struct {
   char  isEnabled;
   int   cfwType;
   int   cfnrTimeout;
   char  keySequence[MAX_KEY_SEQ_STRING_LEN + 1];
   CFW_DEST  line[MAX_NUM_OF_LINES+1];
   
} voip_cfw_params_t;

/* AlexChen 04072009 SDK upgrade dnd features */
typedef struct {
	char isEnabled;
	char keySequence[MAX_KEY_SEQ_STRING_LEN + 1];
	char line_actived[MAX_NUM_OF_LINES+1];
} voip_dnd_params_t;

/* ACL NB 071106 - Stun Client configuration */
typedef struct {
	char                 stunServerAddress[64];  /* Address of the STUN server */
	char    			      stunNeedMask[64];
	unsigned short		   stunServerPort;         /* Port of the STUN server */
	unsigned short		   stunClientResponsePort; /* Port receiving STUN response messages */
} voip_stun_params_t;

/* AlexChen 20090331 SDK upgrade */

typedef enum
{
    DIGIT_MAP_OLD_STYLE = 0,
    DIGIT_MAP_MGCP_LIKE_STYLE = 1,
    
    DIGIT_MAP_UNKNOW_STYLE = 2,
} digit_map_type_t;

typedef struct
{
    int   rangeMap;             /* digit bit table£º0~9:number, 10:A, 11:B, 12:C, 13:D, 14:*, 15:#*/
    char    isVarLen;             /* Position(".") */ 
    char    isLastUnit;           /*last unit of one sub-scheme*/

    int  nextSubSchemeIndex;         /* the next sub-scheme index */
} digit_sub_scheme_t;

typedef struct tagDIGITMAP_CB 
{
    int     validUnitNum; 
    digit_sub_scheme_t digitSubScheme[MAX_DIGITMAP_STRLEN]; 
} digit_map_cb_t;

int send_to_phone_input_cb(int pressed, int event);

#define IS_TONE_SIGNAL(s) ((s)>=VOIP_SIGNAL_DIAL_TONE \
    && (s)<=VOIP_SIGNAL_REORDER_TONE)

#if 0
#define DEBUG_LOG(fmt, arg...) \
    console_printf("%s %d - " fmt "\n", __FUNCTION__, __LINE__, ##arg)
 #else
#define DEBUG_LOG(fmt, arg...)
#endif
#endif
