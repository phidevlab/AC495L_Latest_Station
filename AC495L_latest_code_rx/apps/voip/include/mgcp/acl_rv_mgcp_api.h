/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	acl_rv_mgcp_api.h                                   *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 26/02/08 - Coding started.                                       *
 *																	*
 ********************************************************************/

#ifndef _ACL_MGCP_API_H_
#define _ACL_MGCP_API_H_

typedef int acl_voip_sdp_media_descr_t;
typedef int acl_voip_sdp_rtp_map_t;
typedef int acl_voip_sdp_conn_t;
typedef int acl_voip_sdp_msg_t;
typedef int acl_voip_sdp_attr_t;
typedef int acl_voip_mgcp_capabilities_t;
typedef int acl_voip_mgcp_capabilityList_t;
typedef int acl_voip_mgcp_payload_type_t;
typedef int acl_voip_mgcp_ptrList_t;

typedef struct
{
	int payload;
	const char *codec;
	unsigned int rate;
} MgcpPayloadType;

#define ACL_RTPPORT_ANY (-1)
#define FIRST_DYNAMIC_PAYLOAD_TYPE	96

void acl_mgcp_log_construct(unsigned char isLogOn);

acl_voip_sdp_media_descr_t *acl_voip_sdp_msg_media_descr_get(acl_voip_sdp_msg_t *sdp, int i);
void acl_voip_sdp_msg_media_descr_clear(acl_voip_sdp_msg_t *sdp);
void acl_voip_sdp_msg_media_descr_add(acl_voip_sdp_msg_t *sdp, int port);

void acl_voip_sdp_media_descr_payload_add(acl_voip_sdp_media_descr_t *media, int pl);
void acl_voip_sdp_media_descr_rtp_map_add(acl_voip_sdp_media_descr_t *media, int pl,
    char *plname, int rate);
void acl_voip_sdp_media_descr_attr_add(acl_voip_sdp_media_descr_t *media, char *name,
    char *val);
int acl_voip_sdp_media_descr_payload_num_get(acl_voip_sdp_media_descr_t *media);
int acl_voip_sdp_media_descr_payload_get(acl_voip_sdp_media_descr_t *media, int i);
int acl_voip_sdp_media_descr_rtp_map_num_get(acl_voip_sdp_media_descr_t *media);
acl_voip_sdp_rtp_map_t *acl_voip_sdp_media_descr_rtp_map_get(
    acl_voip_sdp_media_descr_t *media, int i);
int acl_voip_sdp_rtp_map_payload_get(acl_voip_sdp_rtp_map_t *map);
char *acl_voip_sdp_rtp_map_encoding_name_get(acl_voip_sdp_rtp_map_t *map);
int acl_voip_sdp_msg_media_descr_num_get(acl_voip_sdp_msg_t *sdp);
int acl_voip_sdp_media_descr_attr_num_get(acl_voip_sdp_media_descr_t *media);
acl_voip_sdp_conn_t *acl_voip_sdp_media_descr_conn_get(
    acl_voip_sdp_media_descr_t *media);
acl_voip_sdp_conn_t *acl_voip_sdp_msg_conn_get(acl_voip_sdp_msg_t *sdp);
char *acl_voip_sdp_conn_addr_get(acl_voip_sdp_conn_t *conn);
int acl_voip_sdp_media_descr_port_get(acl_voip_sdp_media_descr_t *media);
int acl_voip_sdp_msg_media_descr_type_get(acl_voip_sdp_media_descr_t *media);/* ACL NB 060409 */
acl_voip_sdp_attr_t *acl_voip_sdp_media_descr_attr_get(acl_voip_sdp_media_descr_t *media, int i);
char *acl_voip_sdp_attr_name_get(acl_voip_sdp_attr_t *attr);
char *acl_voip_sdp_attr_value_get(acl_voip_sdp_attr_t *attr);
/* AlexChen SDK upgrade 20090409 */
int acl_voip_mgcpCapabilitiesIsCodecSupported(const acl_voip_mgcp_capabilities_t *caps, const char *codec, unsigned int rate);
int acl_voip_mgcpCapabilityListIsCodecSupported(const acl_voip_mgcp_capabilityList_t *capList, const char *codec, unsigned int rate);
acl_voip_mgcp_payload_type_t *mgcp_getStandardPayloadType(int payload);
void acl_voip_mgcpCapabilityListGetAllRtpMaps(const acl_voip_mgcp_capabilityList_t *capList, acl_voip_mgcp_ptrList_t *rtpMapList);
void IppThreadSleep(int sec, int msec);

#endif
