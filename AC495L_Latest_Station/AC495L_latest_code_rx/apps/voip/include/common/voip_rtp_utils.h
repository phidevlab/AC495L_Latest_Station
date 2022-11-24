/********************************************************************
 * AudioCodes improved application rtp utils                        *
 *                                                                  *
 * File Name:	voip_rtp_utils.h                                    *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 16/02/08 - Coding started.                                       *
 *																	*
 ********************************************************************/

#ifndef _VOIP_RTP_UTILS_H_
#define _VOIP_RTP_UTILS_H_

/*---------------------------------------------------------------------------------------*/
/*                                          INCLUDES                                     */
/*---------------------------------------------------------------------------------------*/

#include "acl_common.h"
//#include "ipphone.h"
#include "acl_line_connection.h"
#include "voip_utils.h"
#include "acl_voip_conf.h"

#include <arpa/inet.h>


/**************************/
/**** Forward Declarations ****/
/**************************/
int payload_type_enum_to_id(int enum_val);
int payload_type_id_to_enum(int id);
int payload_type_enum_to_id_update(int enum_val, int new_id_val);
void get_remote_dtmf_payload_number(int rtp_fd, int *remoteDtmfPayloadNumber);
void get_remote_current_codec_payload_number(int rtp_fd, int *remoteCurrentCodecPayloadNumber);

/*************************/
/**** Structure Definitions ****/
/*************************/

typedef struct {
    char 	enc;
    char 	dec;
    int 	packets_sent;
    int 	packets_received;
    int 	bytes_sent;
    int 	bytes_received;
// ER06 - Take these values from the DSP instead
//    int packets_lost;
//    int packet_loss_percentage;
    int 	duration;    
} rtp_statistics_t;//060530 Avner phase 1

typedef struct codec_payload_type_enum_to_id_t {
    int 	enum_default_value;
    int 	new_id_value;
} codec_payload_type_enum_to_id_t;

/* definition of the structures for receive Stun data from RTP module.*/
typedef struct {
   unsigned char  buff[128]; /* The stun response buff - will be send to stun client application*/
   int            buff_len;  /* The stun response buff length */
} rtp_stun_res_t;


/*---------------------------------------------------------------------------------------*/
/*                                          DEFINES                                      */
/*---------------------------------------------------------------------------------------*/
#define RTP_SEND 	0x01 /* all data, received from media or user, must be
			      * sent to network */
#define RTP_RECEIVE 	0x02 /* all data. received from network, must be
			      * forwarded to media or user */
#define RTP_SENDRECEIVE (RTP_SEND | RTP_RECEIVE)

#define RTCP_CNAME_MAX_SIZE 	256


#define FIRST_DYNAMIC_PAYLOAD   96
#define LAST_DYNAMIC_PAYLOAD   	127

/* IPC event handler set macro */
#define AC_EVENT_HANDLER_SET(fd, type, func, data)	/*ipc_event_fd_set(fd, type, func, data)*/
#define AC_IPC_VOLUME_SET(params)	                	/*ipc_volume_set(params)*/

#define AC_PAYLOAD_TYPE_ENUM_TO_ID(enum_val)	    payload_type_enum_to_id(enum_val)

void aclVoipRtpUtils_advancedChConf(voip_dsp_advanced_channel_configuration_t *advanced_channel_configuration);

/*networking DB functions*/
void aclVoipRtpUtils_resetNetworkingDBs();
void aclVoipRtpUtils_freeNetworkingDbs();
int aclVoipRtpUtils_createSockets();

/*RTP functions*/
void rtp_mode_set(acl_call_t *call, int flash);
rtp_session_t *rtp_get(char *cname, codec_t *codecs, acl_call_t *pCall);
void rtp_free(rtp_session_t *rtp);
void rtp_unbind(int dspChannel);
void rtp_connect(acl_call_t *pCall, in_addr_t addr, u16 port);
u32 rtp_id_prepare(acl_call_t *pCall);
void rtp_add_conf_mate(acl_call_t *pCall, int absCh);
void rtp_remove_conf_mate(int absCh);
void rtp_rcv(int fd, void *context);
void map_dynamic_payload(int rtp_fd, char media_pl, char rtp_pl);
void rtp_t38_set(acl_call_t *call, int isT38);
void rtp_base_local_port_set(int rtpBaseLocalPort);
int rtp_base_local_port_get(void);
void rtp_statistics_get(acl_line_t *line, rtp_statistics_t *rtpstat);
void stun_rtp_req( int localPort, char *msg, int msgSz, int remotePort, char *remoteAddress);
void stun_rtcp_req( int localPort, char *msg, int msgSz, int remotePort, char *remoteAddress);
void stun_response_handle(acl_line_t *line, phone_event_t *pEvent);

#endif /* _VOIP_RTP_UTILS_H_ */
