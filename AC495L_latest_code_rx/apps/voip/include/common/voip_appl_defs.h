/********************************************************************
 * AudioCodes improved application definitions                      *
 *                                                                  *
 * File Name:	voip_appl_defs.h                                    *
 *                                                                  *
 * Copyright (C) 2007 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 30/12/07 - Coding started.                                       *
 *																	*
 ********************************************************************/

#ifndef	_VOIP_APPL_DEFS_H
#define	_VOIP_APPL_DEFS_H

#include "enums.h"
#include "acl_common.h"

#include "acGatewayDefs.h"

/********************************************************************
 * Constant Values Definitions												*
 ********************************************************************/

/*****   General definitions  *****/


#define CALLER_ID_DELAY 					2900

/* ACL NB 061214 */
#define FAX_METHOD_MASK  				0x0F
#define DATA_METHOD_MASK 				0xF0

#define ACL_723_LOW_BITRATE_BIT     		0x10000000
#define ACL_723_HIGH_BITRATE_BIT    		0x20000000
#define ACL_723_BITRATE_MASK        		0x30000000


/******************************************************************** 
 * Enumerated Type Definitions											*
 ********************************************************************/


/********************************************************************* 

 * Structure Definitions     												  *

 *********************************************************************/

typedef struct {

    phone_state_t phone;
    sip_reg_state_t sip_reg;
    int next_sip_reg;
    call_state_t call;
    call_origin_t origin;
    char remote_number[MAX_DIAL_STRING_LEN];
    char remote_id[MAX_CALLER_ID_LEN];
    char remote_addr[AC_DOTTED_IP_LEN];
    int duration;
    call_type_t type;
    rtp_payload_type_t enc;
    rtp_payload_type_t dec;
    int packets_sent;
    int packets_received;
    int bytes_sent;
    int bytes_received;
    int packets_lost;
    int packet_loss_percentage;
    int jitter;
    int round_trip;
} voip_state_t;//060530 Avner phase 1


typedef struct {

	int id; 
	int conferenceId; 
	int is_channel_open;
/* no need *///	int is_t38; 			/* ACL NB - Indicates channel is in t38 mode. */
/* no need *///	int is_nonVoice; 		/* ACL NB - Indicates channel is in bypass mode. */
} channel_t;


typedef struct {

	channel_t ch[NUMBER_OF_CHANNELS_PER_LINE];
/* no need *///	wait_queue_head_t event_wq;
/* no need *///	event_q_t event_q;
/* no need *///	event_q_t hook_q;
/* no need *///	kos_timer_t hook_timer;
/* no need *///	int	 caller_id_timer;
//	phone_ring_params_t ringParams; /* ACL NB 070624 - distRing*/
/* no need *///	int is_off_hook;
/* no need *///	int is_init_complete;
	int is_caller_id_timer_active;
	int is_waiting_for_ring_end_event; /* ER06 */
/* no need *///	int is_hook_timer_active;
	int is_ringing;//060530 Avner phase 1
	int rtcp_jitter; //ER06
	int rtcp_round_trip_delay; //ER06
	int rtcp_packets_lost; //ER06
	int rtcp_fraction_loss; //ER06
/* no need *///	int last_digit_pressed; //NIRP 061228
} lines_t;


typedef struct {

	//int channel; /* 0 - on local slic; >0 - from remote channel (added 1) */
	int intData;
	phone_input_event_t/*phone_key_t*/ key;
	int pressed;

} phone_event_t;


typedef struct {

	int size;
	char message[MAX_SIP_LOG_MSG_SIZE];

} sip_log_message_t;


#endif /*_VOIP_APPL_DEFS_H */

