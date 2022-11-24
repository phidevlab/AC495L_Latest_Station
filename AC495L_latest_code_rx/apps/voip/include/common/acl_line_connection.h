/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	acl_line_connection.h                               *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 25/02/08 - Coding started.                                       *
 *																	*
 ********************************************************************/

#ifndef _PHONE_CONN_H_
#define _PHONE_CONN_H_

//#include <event.h>
#include "acl_common.h"
//#include "voip_common_const.h"//060530 Avner phase 1
//#include "ipphone.h"
#include "acl_call_mngr.h"
//#include "voip_rtp_utils.h"
#include "voip_appl_defs.h"


#ifdef CONFIG_RG_VOIP_TUTORIAL
#include "dsp.h"
#endif

typedef struct acl_call_t acl_call_t;

// ACL NB 061210 #define MAX_FLASH_STRING_LEN 3

typedef struct rtp_session_t {
	struct rtp_session_t *next;
	int fd;
	u16 port;
} rtp_session_t;

/* AlexChen SDK upgrade 20090413 */
/*EA - 080807 - VI65537 - RFC 4240 - interop - dealing with remote parties that do not support REFER inside INVITE message*/
#ifdef CONFIG_RG_VOIP_RV_SIP
typedef struct
{
    char                        *referredByHdrBuf;
    char                        *referToHdrBuf;
    int                         didReferFail;
    int                         unholdInProgress;
} refer_info_t;
#endif /* CONFIG_RG_VOIP_RV_SIP */

typedef struct {
    acl_call_t *active_call;
    acl_call_t *active_call1; /* For conference. Temporary (I hope) design */
    int num;
    int is_off_hook;
    int is_ringing;
#ifdef CONFIG_RG_ATA
     /* The following fields are managed by acl_dsp_telephony_events */
    int is_flash_input_state;
    int last_flash_event;
    char flash_string[MAX_KEY_SEQ_STRING_LEN+1];/* ACL NB 061210 changed MAX_FLASH_STRING_LEN to MAX_KEY_SEQ_STRING_LEN */
    int flash_string_pos;
/* ACL NB 061113 The following fields are managed by acl_dsp_telephony_events. some fields maybe merge with flash's fileds */   
    int  is_suppl_input_state;
    char suppl_string[MAX_KEY_SEQ_STRING_LEN+1];/* ACL NB 061210 changed MAX_FLASH_STRING_LEN to MAX_KEY_SEQ_STRING_LEN */
    int  suppl_string_pos;
#endif

    /*ACL NirS 081028*/
    int  is_mwi;
    int  stutter_tone_dur;
/* ACL NB 070411 */
    int  is_cfw_activated;
    int  is_cfw_on;   // simulate a key CFW on or off !!
    int is_illegal_for_reinvite; // Paul, 2013-0401
    char user_cfw_KeySeq[MAX_KEY_SEQ_STRING_LEN+1];  /* ACL NB 070103 */

	/* AlexChen 04072009 SDK upgrade for dnd features*/
    char user_dnd_KeySeq[MAX_KEY_SEQ_STRING_LEN+1]; 
    char is_dnd_activated;
    char is_dnd_on;
	
	// by itzik - 13072006
	int conference;
	int second_call_orig;/* ACL NB 060912 */
   int keys_sequence;   /* ACL NB 060913 - for now we can't set it per line but per gateway */
   char blindTransferEnabled; /* is blind transfer enabled ? */
      int number_of_channels_per_line;/* NIRP 070813 */
} acl_line_t;

typedef enum {
    NO_DTMF  = 0, 
    LOCAL_DTMF_RELAY  = 1, 
    REMOTE_DTMF_PLAY  = 2,
} dtmf_selector_t; 

struct acl_call_t {
    acl_line_t *phone;
    rtp_session_t *rtp;
    rtp_payload_type_t active_codec;
    int active_codec_remote_code;
    dtmf_selector_t dtmf_select; 
    int dtmf_method; /* ACL NB 060724 */
    voip_conn_mode_t mode;
    int isT38; /* ACL NB 060420 */
    int isVBD; /* ACL NB 061217 */
	int isFaxFallback;     /* AlexChen SDK upgrade 20090408 Fax Fallback */
	int is_fax_relay_detected;      /*AlexChen SDK upgrade 20090408 Fax Fallback*/
    int is_fax_relay_detected_wait_for_ok;/*AlexChen SDK upgrade 20090408 ACL XY 080917 - VI66797: problems with T.38 fax transmissions*/        
	/*EA - 080807 - VI65537 - RFC 4240 - interop - dealing with remote parties that do not support REFER inside INVITE message*/
#ifdef CONFIG_RG_VOIP_RV_SIP
    refer_info_t refer_info;
#endif /* CONFIG_RG_VOIP_RV_SIP */
};

#ifdef CONFIG_RG_IPPHONE

typedef struct{
    char callNumber[MAX_CALLER_ID_SIZE];
    char callDescription[MAX_CALLER_ID_SIZE];
}acl_call_info_t;
#endif

int acl_line_get_dtmf_method(acl_call_t * call);/*JL VI # 64097*/
int acl_line_get_rtp_id(acl_call_t * call);/*JL VI # 64097*/
int acl_line_get_active_codec(acl_call_t * call);/*JL VI # 64097*/
int acl_line_get_active_codec_ptime(acl_call_t * call);/*JL VI # 64097*/

void acl_line_connection_input_event_handle(acl_line_t *line,
    phone_input_event_t input_event, phone_event_t *pEvent);
int acl_line_is_in_active_call(acl_line_t *line);

char *phone_conn_get_active_codec_name(acl_line_t *line);

/* set rtp session mode according to state, recieved from signalling (send,
 * receive, sendreceive) or according to flash state (after click on flash
 * button, we don't have to translate voice and dtmf tones) */
void rtp_mode_set(acl_call_t *call, int flash);

#define NUMBER_OF_CODECS 10 /* XXX this value should be sync with rg task */
#define DEFAULT_CODEC VOIP_CODEC_PCMU

void acl_line_connection_init(codec_t *conf_codecs, int out_of_band_dtmf);
void acl_line_connection_uninit(void);
codec_t *codec_get_by_payload(int payload);

/* XXX need to understand how to distinguish between G.723 bit rates */
int is_723_low(void);

/* Convert a dial string as kept in the RV digit map to the original keys:
 * 'E' to '*', 'F' to '#'. Changes the given string. */
void dtmf_convert(char *dialed);

void dtmf_stop(void *data);

int acl_line_fax_relay_deteceted_get(acl_line_t *phoneConn);
void acl_line_fax_relay_detected_wait_for_ok_set(acl_line_t *phoneConn,int val);
int acl_line_fax_relay_detected_wait_for_ok_get(acl_line_t *phoneConn);

void acl_line_voip_state_get(int line, voip_state_t *voip_state);//060530 Avner phase 1
void acl_line_log_level_set(int level); // ER07

void fax_bypass_detect_event_handle(acl_line_t *line);

#ifdef CONFIG_RG_VOIP_TUTORIAL
void phone_conn_tx_poll(tx_poll_data_t dsp_data);
#endif

/*EA - 080807 - VI65537 - RFC 4240 - interop - dealing with remote parties that do not support REFER inside INVITE message*/
#ifdef CONFIG_RG_VOIP_RV_SIP
void acl_line_refer_info_headers_str_set(acl_line_t *phoneConn, 
                                           char* strReferToHeader, 
                                           char* strReferredByHeader);
void acl_line_refer_info_headers_str_get(acl_line_t *phoneConn, 
                                           char** pStrReferToHeader, 
                                           char** pStrReferredByHeader);
void acl_line_refer_info_failure_flag_set(acl_line_t *phoneConn, int val);
int acl_line_refer_info_failure_flag_get(acl_line_t *phoneConn);
void acl_line_refer_info_unhold_flag_set(acl_line_t *phoneConn, int val);
int acl_line_refer_info_unhold_flag_get(acl_line_t *phoneConn);
#endif /* CONFIG_RG_VOIP_RV_SIP */

#endif
