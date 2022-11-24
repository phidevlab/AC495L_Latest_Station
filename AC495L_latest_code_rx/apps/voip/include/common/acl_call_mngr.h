/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	acl_call_mngr.h                                     *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 26/02/08 - Coding started.                                       *
 *																	*
 ********************************************************************/

#ifndef _CALL_MGT_H_
#define _CALL_MGT_H_

#ifdef CONFIG_RG_VOIP_RV_SIP
#include "acl_rv_sip_api.h"
#endif
#ifdef CONFIG_RG_VOIP_RV_MGCP
#include "acl_rv_mgcp_api.h"
#endif
#include "acl_common.h"
/*  EA - 071218 - VI#59630 support SIP over TLS */
#ifdef RV_CFLAG_TLS
#include <RvSipStackTypes.h>
#include <rvstring.h>
#include <sipTls.h>
#endif /* RV_CFLAG_TLS */

/*AlexChen 20090402 SDK upgrade struct for keep-alive OPTION message*/
typedef struct _KA_Option_Handle_t
{
    char            *TermId;
	int				timeout;
	char			*IpAddress;
	int				CSeq;
	unsigned char	IsRedundantProxy;
}KA_Option_Handle_t;

typedef enum {
    VOIP_CONN_MODE_NOT_SET	= 0,
    VOIP_CONN_MODE_INACTIVE	= 1,
    VOIP_CONN_MODE_SEND		= 2,
    VOIP_CONN_MODE_RECV		= 3,
    VOIP_CONN_MODE_SEND_RECV	= 4,
    VOIP_CONN_MODE_LOOPBACK	= 5,
    VOIP_CONN_MODE_CONFERENCE 	= 6,
    VOIP_CONN_MODE_NETWORK_LOOP	= 7,
    VOIP_CONN_MODE_CONT_TEST	= 8,
    VOIP_CONN_MODE_NETWORK_TEST	= 9,
} voip_conn_mode_t;

typedef enum {
    VOIP_INPUT_0         = 0, /* values of the enums VOIP_INPUT_0..          */
    VOIP_INPUT_1         = 1, /* VOIP_INPUT_9 can't be changed, because used */
    VOIP_INPUT_2         = 2, /* as integer value                            */
    VOIP_INPUT_3         = 3,
    VOIP_INPUT_4         = 4,
    VOIP_INPUT_5         = 5,
    VOIP_INPUT_6         = 6,
    VOIP_INPUT_7         = 7,
    VOIP_INPUT_8         = 8,
    VOIP_INPUT_9         = 9,
    VOIP_INPUT_ASTERISK  = 10,
    VOIP_INPUT_POUND     = 11,
	/* Eitan1207 - Add support for DTMF A-D */
    VOIP_INPUT_A         = 12,
    VOIP_INPUT_B         = 13,
    VOIP_INPUT_C         = 14,
    VOIP_INPUT_D         = 15,
    /* VOIP_INPUT_PLUS      = 12, */
    /* VOIP_INPUT_MINUS     = 13, */
    VOIP_INPUT_OFFHOOK   = 16,
    VOIP_INPUT_ONHOOK    = 17,
    VOIP_INPUT_HOLD      = 18,
    VOIP_INPUT_TRANSFER  = 19,
    VOIP_INPUT_CONF      = 20,
    VOIP_INPUT_FLASH     = 21,
    VOIP_INPUT_CFW_ON    = 22, /* ACL NB 061113 call forward*/
    VOIP_INPUT_CFW_OFF   = 23, /* ACL NB 061113 call forward*/
    VOIP_INPUT_FAX_DETECT= 24, /* ACL NB 060404 */
    VOIP_INPUT_FAX_BYPASS_DETECT= 25, /* ACL NB 060704 */
    VOIP_INPUT_BLINDT    = 26, /* ACL NB 070507 Blind Transfer */
    VOIP_INPUT_SWITCH2VOICE_DETECT= 27, /* ACL NB 070806 */
    VOIP_INPUT_DAA_RING_DETECT_START= 28, /* NIRP 070527 */
    VOIP_INPUT_DAA_RING_DETECT_STOP= 29, /* NIRP 070527 */   
	//*NirS - Add support for IPP*/
     VOIP_INPUT_LINE_1,
     VOIP_INPUT_LINE_2,
     VOIP_INPUT_SPEAKER,
     VOIP_INPUT_HEADSET,
	/* AlexChen SDK upgrade 20090407 dnd feature */
	VOIP_INPUT_DND = 34,
    /* Output to be ignored */
    VOIP_INPUT_IGNORE    = 99,
} voip_input_event_t;

typedef enum {                       
    VOIP_DISPLAY_INVALID	  = -1,
    VOIP_DISPLAY_IDLE 		  = 0,
    VOIP_DISPLAY_DIALING 	  = 1,
    VOIP_DISPLAY_RINGING_PARTY 	  = 2,
    VOIP_DISPLAY_CALL_IN_PROGRESS = 3,
    VOIP_DISPLAY_PARTY_HUNG_UP    = 4,
    VOIP_DISPLAY_WRONG_NUMBER     = 5,
    VOIP_DISPLAY_INCOMING_CALL    = 6,
    VOIP_DISPLAY_HOLD           = 7,
    VOIP_DISPLAY_BUSY           = 8,
    VOIP_DISPLAY_CALL_END     	= 9,
    VOIP_DISPLAY_TRANSFER     	= 10,
    VOIP_DISPLAY_CONFERENCE     = 11,
    VOIP_DISPLAY_MUTE           = 12,
    VOIP_DISPLAY_LINE           = 13,
    VOIP_DISPLAY_OFFHOOK        = 14,
    VOIP_DISPLAY_ONHOOK         = 15,
    VOIP_DISPLAY_SPK_ON=16,
	VOIP_DISPLAY_SPK_OFF=17,
	VOIP_DISPLAY_VOLUME=18,
} voip_display_state_t;

/* return value for matching dialing string */ 
typedef enum {
    MATCH_DIAL_PARTIALMATCH = 0, /* string can't be matched and is not maximal
				  * size, user can continue to dial string 
				  */
    MATCH_DIAL_UNAMBIGUOUSMATCH = 1, /* number is matched */
    MATCH_DIAL_NOMATCH = 2, /* no match found and maximum length reached */
    MATCH_DIAL_MATCH = 3, /* match found but maybe others exist */
} voip_dial_match_t;

/* NOTE: We use pointers to data in this struct, and we count on that their
 * content is not changed by the application after we get them! */
/*  EA - 071218 - VI#59630 support SIP over TLS */
#if RV_CFLAG_TLS
#define TLS_MAX_CA			8
#define FILENAME_LEN		256

typedef struct  {
	char			privateKeyFileName[FILENAME_LEN]; 
	char			caCertFileName[TLS_MAX_CA][FILENAME_LEN]; /* Certification file name */
}acl_sip_tls_key_cfg_t;
#endif /* RV_CFLAG_TLS */


typedef struct {
    voip_protocols_t protocol;
    char local_ip[AC_DOTTED_IP_LEN];
    line_conf_t *lines;
    int ep_num;
    /*int send_dtmf_out_of_band;*/
    int dtmf_transport_method; /* ACL NB 060723 rename of send_dtmf_out_of_band */
    int keys_sequence;         /* ACL NB 060913 */
    u16 media_port;
    u8 media_tos; /* Type Of Service value for media packets */
    int mss_clamping_limit; /* Max MSS value for throughput tcp packets on the
			     * time of established voice connection. Zero value
			     * means no mss clamping */
    u16 local_host_port;
    u16 local_host_port_range;
    u8  is_auto_dial;                           /* ACL NB 070514 - automatic dialing param */
    int auto_dial_timeout;                      /* ACL NB 070514 - automatic dialing param */
    char auto_dial_dest[AC_MAX_DOMAIN_NAME_LEN + 1];/* ACL NB 070514 - automatic dialing param */
    char use_secondary_dial_tone;                /* ACL NB 070528 - secondary dila tone     */
    char secondary_dial_tone_key;                /* ACL NB 070528 - secondary dila tone     */

	char fax_audio_startup_enabled;				/* AlexChen SDK upgrade 20090409 fax */
	int	 fax_audio_startup_payload;				/* AlexChen SDK upgrade 20090409 fax */

	char rtp_mute_on_hold;						/* AlexChen SDK upgrade 20090415 */

	char regret_call_enabled;					/* AlexChen SDK upgrade 20090414 regret */
	int  regret_call_timeout;					/* AlexChen SDK upgrade 20090414 regret */

	char update_fax_to_transparent_enable;		/* AlexChen SDK upgrade 20090415 */

    union {
	struct {
	    char gk_address[AC_MAX_HOST_NAME_LEN + 1];
	    int gk_registry;
	    int fast_start;
	    char *e164_alias_list;
	    u16 signal_port;
	    u16 gk_port;
	} h323;
	struct {
	    voip_sip_transport_prot_t sip_transport_protocol;
	    int use_proxy;
    	char proxy_address[AC_MAX_HOST_NAME_LEN + 1];
        u16  proxy_port; /* ACL NB 060604 proxy port */ 

		/*AlexChen 20090401 SDK upgrade redundant proxy feature */
		int  use_redundant_proxy;
		char redundant_proxy_address[AC_MAX_HOST_NAME_LEN + 1];
		u16  redundant_proxy_port;
		u16  redundant_proxy_ka_period;
		int  redundant_proxy_is_symmetric;
		int  is_redundant_proxy;
		
		// by itzik - 20 June 2006  - add outbound proxy
	    int use_outbound_proxy;
    	char outbound_proxy_address[AC_MAX_HOST_NAME_LEN + 1];
        u16  outbound_proxy_port;

	    int use_registrar;
    	char registrar_address[AC_MAX_HOST_NAME_LEN + 1];
        u16  registrar_port;

    	u16 sip_port;
	    char proxy_user_name[AC_MAX_USERNAME_LEN + 1];
	    char proxy_password[AC_MAX_PASSWORD_LEN + 1];
	    u32 proxy_registration_timeout;
      /* Radvision2.0 */
		char user_doamin[AC_MAX_USERNAME_LEN + 1];
		u32 uregistration_timeout;
		u32 watchdog_timeout;
      u32 callWaitingEnabled; /* ACL NB 070404*/
		u32 callWaitingReply;
		u32	callWaitingType2Enabled;

		// by itzik - 22 June 2006 - add SIP timers support
		u32 retransmissionT1;
		u32 retransmissionT2;
		u32 retransmissionT4;
		u32 inviteLingerTimer;

	    int use_proxy_port_ip_for_registrar;
		/*ACL JL 59875 limits number of registrations to proxy to  a configurable number RV patch*/
		int auth_retries;

		char numberRules[1024]; // by itzik - 19072006 - digitmap

	    /*EA - 071115 - connectMediaOn180*/
        int connectMediaOn180;
        /*  EA - 071218 - VI#59630 support SIP over TLS */
#ifdef RV_CFLAG_TLS
	    RvIppTransportTlsCfg		transportTlsCfg;
	    acl_sip_tls_key_cfg_t				keyTlsCfg;
#endif /* RV_CFLAG_TLS */

		char remote_fax_side_reinvite; /* AlexChen 20090408 SDK upgrade for sip fax */

	} sip;
	struct {
		char user_domain[AC_MAX_DOMAIN_NAME_LEN + 1];
	    char mgc_address[AC_MAX_HOST_NAME_LEN + 1];
	    u16 mgc_port;
	    u16 mg_port;

		char fax_end_report; 	/* AlexChen 20090409 SDK upgrade for mgcp fax */
	} mgcp;
    } sp;
} acl_call_mngr_params_t;

typedef void (*enum_codec_cb)(char *, codec_t *, void *);

/* The following callbacks are implemented by the application and are called by
 * the call management abstraction */
typedef struct {
    /* Called when a signal should start, that plays for constant time */
    int (*play_signal)(void *ep_context, voip_signal_t signal, void *sigdata);

    /* Called when a signal should start playing */
    int (*start_signal)(void *ep_context, voip_signal_t signal, void *sigdata);

    /* Called when a signal should stop playing */
    int (*stop_signal)(void *ep_context, voip_signal_t signal, void *sigdata);

    /* Called when the display should be updated */
    void (*update_display)(void *ep_context, voip_display_state_t display_state,
        void *pInfo);

    /* Called to check status of current dial */
    voip_dial_match_t (*dialstring_is_complete)(void *ep_context,
	char *dial_string, long *timer);

    /* Called to convert dial to address. implementor should fill in the caller
     * id and the address. return 0 on success, -1 on error */
    int (*dialstring_to_addr)(void *ep_context, char *dial_string,
	char caller_id[], char addr[]);
    
    /* Called to indicate that a new connection should be created.
     * Open local port. Return connection context, or NULL if failed.
     * if !actual, this is a global preparation. don't create a connection,
     * only update local capabilities. */
    void *(*prepare_connection)(acl_voip_sdp_msg_t *local, int actual);

    /* Called to indicate that the characteristics of an existing connection
     * should be modified according to remote parameters. Update local, and
     * record remote parameters in context. Return 0 for success, -1 if no
     * match. XXX in the future we might want to update codecs from other
     * SDPs, and this will require special handling from the caller (in case
     * of working with a list of SDPs) */
    int (*create_connection)(void *conn_context, acl_voip_sdp_msg_t *local,
	acl_voip_sdp_msg_t *remote, voip_conn_mode_t mode, int dtmf_method); /* ACL NB 060724 added dtmf_method */

    /* Called to indicate that a call takes place on an existing connection */
	int (*start_connection)(void *ep_context, void *conn_context);

    /* Called to pause the current voice traffic */
	int (*stop_connection)(void *ep_context, void *conn_context);

    /* Called to indicate that a connection should be modified while a call is
     * in progress */
    int (*modify_connection)(void *conn_context, acl_voip_sdp_msg_t *local, 
	acl_voip_sdp_msg_t *remote, voip_conn_mode_t mode, int remoteMediaDesIdx); // ACL NB 070627 added media descr index
    
    /* Called to indicate that a connection should be closed */
    int (*destroy_connection)(void *conn_context);

    /* ACL NB 060404 */
    /* Called to strat fax Relay */
/* ACL JL 60222 MaxBitRate negotiation - update DSP according to the SIP SDP attribute field called T38MaxBitRate */
    int (*start_fax)(void *ep_context,acl_voip_sdp_msg_t *local, acl_voip_sdp_msg_t *remote,voip_conn_mode_t mode,int fax_rate);

    /* ACL NM 080403 */
    int (*stop_fax)(void *ep_context);

    /* ACL NB 061217 */
    /* Called to start fax/Data bypass*/
    int (*start_bypass)(void *ep_context);

	/* AlexChen SDK upgrade 20090415 */
	/*ACL XY 081218 Fax Fallback*/
    int (*start_fax_fallback)(void *ep_context);

    /* called to query the current state */
    int (*is_state)(void *conn_context, phone_input_event_t state);

#ifdef AC488_CPE_VOIP_TOOLKIT
    /* called to check if the other line is during conference */
    int (*is_other_line_in_conference)(void *conn_context);
#endif /*AC488_CPE_VOIP_TOOLKIT*/

    /* called to attach a context to an endpoint */
    void *(*set_endpoint_context)(int epn, char *name, int keys_sequence);

    /* called to free the context of an endpoint */
    void (*free_endpoint_context)(void *ep_context);
   
    /* called to perform some operation on the enabled codecs */
    void (*enum_codecs)(enum_codec_cb static_codec_cb,
	enum_codec_cb dyn_codec_cb, void *data);
/* ACLNB 060801 */
    /* Called to start 3wc */
	int (*start_3wc)(void *conn_context);

/* ACL NB 060912 */
    /* set call origin  */
	int (*set_call_origin)(void *conn_context, int origin);

   /* ACL NB 070411 */
	int (*activate_cfw)(void *conn_context, int state);

    /* NIRP 070527 */
    /* FXO commands */
   	int (*send_daa_commands)(void *data);
	
    /* NIRP 070610 VI 54097 */
    int (*adaptation_layer_send_general_command)(void *data);

    void(*onhook_event_handle)(int line);
	/* AlexChen SDK upgrade 20090414 */
	int (*is_during_fax_call)(void *conn);
  } acl_call_mngr_cbs_t;

/* The following operations are implemented for each call management */
typedef struct {
    /* Initializes the call management. 'params' contains protocol-specific 
     * parameters */
    int (*init)(acl_call_mngr_params_t *params, acl_call_mngr_cbs_t *cbs);

    /* Stops and uninitializes the call management */
    int (*uninit)(void);

    /* Notifies the call management of an event it should process */
    int (*process_event)(int epn, voip_input_event_t event, int down);

    /* Return the active signalling protocol */
    voip_protocols_t (*get_active_protocol)(void);

    /* called to query sip registration status of a line */
    int (*get_registration_status)(int epn);//060530 Avner phase 1

    /* called to retrieve connection associated with a line */
    void *(*get_conn_by_line)(int line);//060530 Avner phase 1

    /* called to retrieve current call state and origin on the given line */
    void (*get_call_state)(int line, call_state_t *state, call_origin_t *orig);//060530 Avner phase 1
    
    /* called the Stun client Stun with reply message  ACL NB 080206 */
    int (*stun_client_reply_received)(char* buf, size_t size, u32 *pMappedIp,	u16 *pMappedPort);

    /* JL phone need to know if line is FXO. fix bug when trying to activate slic on ring start on FXO line*/
    unsigned char (*is_fxo_line)(int LineIdx);
	
} acl_call_mngr_ops_t;

extern acl_call_mngr_ops_t *call_mgt;

extern acl_call_mngr_cbs_t call_mngr_cbs;

/* Here come declarations of protocol implementations */
extern acl_call_mngr_ops_t term_call_mngr_ops;
extern acl_call_mngr_ops_t mgcp_call_mngr_ops;

/* sip/h323 specific functions */
int mdm_h323_init(acl_call_mngr_params_t *call_mngr_params, void *term_mgr);
void h323_alias_free(acl_call_mngr_params_t *call_mngr_params);
int call_sip_init(acl_call_mngr_params_t *call_mngr_params, void *term_mgr);
void call_sip_uninit(void *stack_handle);
/*JL VI # 62393 redundant proxy feature */
char *GetKAoptionIpEntity(void);
int acl_sip_call_mngr_uninit_per_line(int line);
int acl_sip_call_mngr_init_per_line(int line);

int call_mgcp_init(acl_call_mngr_params_t *params, acl_call_mngr_cbs_t *cbs);
int call_mgcp_uninit(void);

sip_reg_state_t mdm_sip_registration_status(int epn);//060530 Avner phase 1

 /* ACL NB 061012 - relocation of rvIppSipRegisterExtClbks - Should be called before rvIppSipStackInitialize*/
void sip_register_ext_clbks(void *pCfg); /* ACL NB 061115  change prototype */

/*  EA - 071218 - VI#59630 support SIP over TLS */
#ifdef RV_CFLAG_TLS
void sip_tls_register_ext_clbks(void);
void sip_tls_mtf_config_params_set(void *pCfg);
#endif /* RV_CFLAG_TLS */

/*EA - 080114 - VI#60461 - Bezeq Interop*/
void acl_signal_start(void *pMdmTerm, voip_signal_t signal);
void acl_signal_stop(void *pMdmTerm, voip_signal_t signal);

/* AlexChen SDK upgrade 20090413 */
/*EA - 080807 - VI65537 - RFC 4240 - interop - dealing with remote parties that do not support REFER inside INVITE message*/
void call_sip_mngr_refer_info_headers_str_set(void *pMdmTerm, 
                                             char* strReferToHeader, 
                                             char* strReferredByHeader);
void call_sip_mngr_refer_info_headers_str_get(void *pMdmTerm, 
                                             char** pStrReferToHeader, 
                                             char** pStrReferredByHeader);
void call_sip_mngr_refer_info_failure_flag_set(void *pMdmTerm, int val);
int call_sip_mngr_refer_info_failure_flag_get(void *pMdmTerm);
void call_sip_mngr_refer_info_unhold_flag_set(void *pMdmTerm, int val);
int call_sip_mngr_refer_info_unhold_flag_get(void *pMdmTerm);
void call_sip_mngr_process_remote_conference(void *pMdmTerm);

#endif
