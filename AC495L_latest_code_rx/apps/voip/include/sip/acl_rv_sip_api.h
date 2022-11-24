/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	acl_rv_sip_api.h                                    *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 26/02/08 - Coding started.                                       *
 *																	*
 ********************************************************************/

#ifndef _ACL_RV_SIP_API_H_
#define _ACL_RV_SIP_API_H_

typedef int acl_voip_mutex_t;
typedef int acl_voip_sdp_media_descr_t;
typedef int acl_voip_sdp_rtp_map_t;
typedef int acl_voip_sdp_conn_t;
typedef int acl_voip_sdp_msg_t;
typedef int acl_voip_sdp_attr_t;
typedef int acl_voip_sdp_msg_list_t;/* ACL NB 060406 */
typedef int acl_voip_ipp_sip_cfg_t;/* ACL NB 060406 */
typedef int acl_voip_msg_t;/* ACL NB 060406 */

#define ACL_RTPPORT_ANY (-1)

/*EA - 071225 Enabling Radvision's logs through acl_conf */
/*EA 080122 - VI#60671 - for Setting Syslog IP address for application logs through GUI*/
int acl_voip_mtf_log_construct(unsigned char isMtfLogOn, unsigned char isUsrLogOn);
/* ACL NB 060814 */
void acl_voip_sip_log_construct(acl_voip_ipp_sip_cfg_t *pCfg, unsigned char isSipLogOn);

void acl_voip_mutex_construct(acl_voip_mutex_t *m);
void acl_voip_mutex_destruct(acl_voip_mutex_t *m);
void acl_voip_mutex_lock(acl_voip_mutex_t *m);
void acl_voip_mutex_unlock(acl_voip_mutex_t *m);

void acl_voip_sdp_msg_media_descr_add(acl_voip_sdp_msg_t *sdp, int port);
acl_voip_sdp_media_descr_t *acl_voip_sdp_msg_media_descr_get(acl_voip_sdp_msg_t *sdp, int i);
void acl_voip_sdp_msg_media_descr_clear(acl_voip_sdp_msg_t *sdp);
int acl_voip_sdp_msg_media_descr_num_get(acl_voip_sdp_msg_t *sdp);
int acl_voip_sdp_msg_media_descr_type_get(acl_voip_sdp_media_descr_t *media);/* ACL NB 060409 */
void acl_voip_sdp_media_descr_payload_add(acl_voip_sdp_media_descr_t *media, int pl);
int acl_voip_sdp_media_descr_payload_get(acl_voip_sdp_media_descr_t *media, int i);
int acl_voip_sdp_media_descr_payload_num_get(acl_voip_sdp_media_descr_t *media);
void acl_voip_sdp_media_descr_rtp_map_add(acl_voip_sdp_media_descr_t *media, int pl, char *plname, int rate);
acl_voip_sdp_rtp_map_t *acl_voip_sdp_media_descr_rtp_map_get(acl_voip_sdp_media_descr_t *media, int i);
int acl_voip_sdp_media_descr_rtp_map_num_get(acl_voip_sdp_media_descr_t *media);
int acl_voip_sdp_media_descr_port_get(acl_voip_sdp_media_descr_t *media);
int acl_voip_sdp_media_descr_port_set(acl_voip_sdp_media_descr_t *media, int port);/* ACL NB 060810 */
void acl_voip_sdp_media_descr_attr_add(acl_voip_sdp_media_descr_t *media, char *name, char *val);
void acl_voip_sdp_media_descr_attr_remove(acl_voip_sdp_media_descr_t *media, int index);/* ACL NB 060713 */
acl_voip_sdp_attr_t * acl_voip_sdp_media_descr_attr_get(acl_voip_sdp_media_descr_t *media, int i);
int acl_voip_sdp_media_descr_attr_num_get(acl_voip_sdp_media_descr_t *media);
acl_voip_sdp_conn_t *acl_voip_sdp_media_descr_conn_get(acl_voip_sdp_media_descr_t *media);
int acl_voip_sdp_rtp_map_payload_get(acl_voip_sdp_rtp_map_t *map);
char *acl_voip_sdp_rtp_map_encoding_name_get(acl_voip_sdp_rtp_map_t *map);
char *acl_voip_sdp_attr_name_get(acl_voip_sdp_attr_t *attr);
char *acl_voip_sdp_attr_value_get(acl_voip_sdp_attr_t *attr);
acl_voip_sdp_conn_t *acl_voip_sdp_msg_conn_get(acl_voip_sdp_msg_t *sdp);
char *acl_voip_sdp_conn_addr_get(acl_voip_sdp_conn_t *conn);

int acl_voip_buf_to_sdpList( char* bufSdp, acl_voip_sdp_msg_list_t *sdpList );/* ACL NB 060406 */

const char *acl_voip_cc_modify_media_reason_text(int reason);/* ACL NB 060411 */
const char *acl_voip_cc_conn_state_text(int state);/* ACL NB 060628 */
const char *acl_voip_cc_event_text(int eventId); /* ACL NB 061115 */

void acl_voip_sdp_print( char *sdp, const char* title);/* ACL NB 060706 */

int acl_voip_sip_msg_method_type_get(acl_voip_msg_t *msg);/* ACL NB 070524 */

#endif /*_ACL_RV_SIP_API_H_*/
