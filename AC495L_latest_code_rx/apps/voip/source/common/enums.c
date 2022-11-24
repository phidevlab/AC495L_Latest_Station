/********************************************************************
 * AudioCodes improved application enumerators definitions          *
 *                                                                  *
 * File Name:	enums.c                                             *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 25/02/08 - Coding started.                                       *
 *																	*
 ********************************************************************/

#include "enums.h"

static char *pcmu_str[] = {"PCMU", "pcmu", "g711u", "G711U", "g.711u", "G.711U", NULL};
static char *g726_32_str[] = {"g726-32", "G726-32", "g.726-32", "G.726-32", "32kadpcm", NULL};
static char *g723_str[] = {"g723", "G723", "g.723", "G.723", NULL};
static char *pcma_str[] = {"PCMA", "pcma", "g711a", "G711A", "g.711a", "G.711A", NULL};
static char *g722_str[] = {"G722", "g722", "G.722", "g.722", NULL};
static char *cn_str[] = {"CN", "cn", NULL};
static char *g728_str[] = {"G728", "g728", "G.728", "g.728", NULL};
static char *g729_str[] = {"G729", "g729", "G.729", "g.729", NULL};
static char *g726_16_str[] = {"g726-16", "G726-16", "g.726-16", "G.726-16", NULL};
static char *dtmf_str[] = {"telephone-event", NULL};


code2str_ex_t rtp_payload_type_t_str[]=
{
	{RTP_PAYLOAD_PCMU, 		pcmu_str},
	{RTP_PAYLOAD_G726_32,		g726_32_str},
	{RTP_PAYLOAD_G723, 		g723_str},
	{RTP_PAYLOAD_PCMA, 		pcma_str},
	{RTP_PAYLOAD_G722, 		g722_str},
	{RTP_PAYLOAD_CN, 			cn_str},
	{RTP_PAYLOAD_G728, 		g728_str},
	{RTP_PAYLOAD_G729, 		g729_str},
	{RTP_PAYLOAD_G726_16, 		g726_16_str},
	{RTP_PAYLOAD_DTMF, 		dtmf_str},
	{-1, 0}
};

static char *sip_transport_udp_str[] = {"udp", "UDP", NULL}; 
static char *sip_transport_tcp_str[] = {"tcp", "TCP", NULL}; 
static char *sip_transport_tls_str[] = {"tls", "TLS", NULL};

code2str_ex_t voip_sip_transport_prot_t_str[]= {
        {VOIP_SIP_TRANSPORT_UDP,    sip_transport_udp_str},
        {VOIP_SIP_TRANSPORT_TCP,    sip_transport_tcp_str},
        {VOIP_SIP_TRANSPORT_TLS,    sip_transport_tls_str},
	{-1, 0}
};

static char *voip_protocol_sip_str[]    = {"sip", "SIP", NULL}; 
static char *voip_protocol_h323_str[]   = {"h.323", "H.323", NULL}; 
static char *voip_protocol_mgcp_str[]   = {"mgcp", "MGCP", NULL};

code2str_ex_t voip_protocols_t_str[]= {
        {VOIP_PROT_SIP,     voip_protocol_sip_str},
        {VOIP_PROT_H323,    voip_protocol_h323_str},
        {VOIP_PROT_MGCP,    voip_protocol_mgcp_str},
	{-1, 0}
};

static char *dtmf_inband_str[]  = {"inband", NULL}; 
static char *dtmf_rfc2833_str[] = {"rfc2833", NULL}; 
static char *dtmf_via_sip_str[] = {"via SIP", NULL};

code2str_ex_t dtmf_transport_mode_t_str[]= {
        {DTMF_INBAND,     dtmf_inband_str},
        {DTMF_RFC2833,    dtmf_rfc2833_str},
        {DTMF_VIA_SIP,    dtmf_via_sip_str},
	{-1, 0}
};

static char *key_seq_flash_only_str[]       = {"Flash only", NULL}; 
static char *key_seq_flash_and_digits_str[] = {"Flash + digits", NULL}; 
static char *key_seq_remote_flash[]         = {"Send Flash Hook via SIP", NULL};

code2str_ex_t key_sequence_style_t_str[]= {
        {KEY_SEQ_FLASH_ONLY,        key_seq_flash_only_str},
        {KEY_SEQ_FLASH_AND_DIGITS,  key_seq_flash_and_digits_str},
        {KEY_SEQ_REMOTE_FLASH,      key_seq_remote_flash},
	{-1, 0}
};

static char *analog_port_type_fxs_str[]         = {"Analog Port Type FXS", NULL}; 
static char *analog_port_type_fxo_str[] 	   = {"Analog Port Type FXO", NULL}; 
static char *analog_port_type_fxo_fxs_str[]  = {"Analog Port Type FXO and FXS", NULL};

code2str_ex_t analog_port_type_t_str[]= {
        {ANALOG_PORT_TYPE_FXS,        	analog_port_type_fxs_str},
        {ANALOG_PORT_TYPE_FXO,  		analog_port_type_fxo_str},
        {ANALOG_PORT_TYPE_FXO_FXS,      	analog_port_type_fxo_fxs_str},
	{-1, 0}
};

static char *cw_sip_reply_ringing_str[] = {"Ringing", NULL}; 
static char *cw_sip_reply_queued_str[]  = {"Queued", NULL}; 

code2str_ex_t cw_sip_reply_t_str[]= {
        {CW_SIP_REPLY_RINGING,  cw_sip_reply_ringing_str},
        {CW_SIP_REPLY_QUEUED,   cw_sip_reply_queued_str},
	{-1, 0}
};

static char *conf_local_str[]   = {"Local", NULL}; 
static char *conf_remote_str[]  = {"Remote", NULL}; 

code2str_ex_t conf_mode_t_str[]= {
        {CONF_LOCAL,    conf_local_str},
        {CONF_REMOTE,   conf_remote_str},
	{-1, 0}
};

static char *cfw_cfu_str[]  = {"Unconditional", NULL}; 
static char *cfw_cfb_str[]  = {"Busy", NULL}; 
static char *cfw_cfnr_str[] = {"No-Reply", NULL};

code2str_ex_t cfw_type_t_str[]= {
        {CFW_CFU,   cfw_cfu_str},
        {CFW_CFB,   cfw_cfb_str},
        {CFW_CFNR,  cfw_cfnr_str},
	{-1, 0}
};

static char *out_of_service_tone_none_str[]     = {"No Tone", NULL}; 
static char *out_of_service_reorder_tone_str[]  = {"Reorder Tone", NULL}; 

code2str_ex_t out_of_service_tone_t_str[]= {
        {OUT_OF_SERVICE_TONE_NONE,      out_of_service_tone_none_str},
        {OUT_OF_SERVICE_REORDER_TONE,   out_of_service_reorder_tone_str},
	{-1, 0}
};

static char *fax_max_rate_2_4_kbps_str[]  = {"2.4 Kbps", NULL}; 
static char *fax_max_rate_4_8_kbps_str[]  = {"4.8 Kbps", NULL}; 
static char *fax_max_rate_7_2_kbps_str[] = {"7.2 Kbps", NULL};
static char *fax_max_rate_9_6_kbps_str[] = {"9.6 Kbps", NULL};
static char *fax_max_rate_12_kbps_str[] = {"12 Kbps", NULL};
static char *fax_max_rate_14_4_kbps_str[] = {"14.4 Kbps", NULL};

code2str_ex_t fax_max_rate_t_str[]= {
        {FAX_MAX_RATE_2_4_KBPS,     fax_max_rate_2_4_kbps_str},
        {FAX_MAX_RATE_4_8_KBPS,     fax_max_rate_4_8_kbps_str},
        {FAX_MAX_RATE_7_2_KBPS,     fax_max_rate_7_2_kbps_str},
        {FAX_MAX_RATE_9_6_KBPS,     fax_max_rate_9_6_kbps_str},
        {FAX_MAX_RATE_12_KBPS,      fax_max_rate_12_kbps_str},
        {FAX_MAX_RATE_14_4_KBPS,    fax_max_rate_14_4_kbps_str},
	{-1, 0}
};

static char *t38_transport_disable_str[]    = {"Transparent", NULL}; 
static char *t38_transport_relay_str[]      = {"T.38Relay", NULL}; 
static char *t38_voice_band_data_str[]      = {"Voice-Band-Data", NULL};
static char *t38_transport_bypass_str[]     = {"Bypass", NULL};

code2str_ex_t t38_transport_mode_t_str[]= {
        {T38_TRANSPORT_DISABLE,     t38_transport_disable_str},
        {T38_TRANSPORT_RELAY,       t38_transport_relay_str},
        {T38_VOICE_BAND_DATA,       t38_voice_band_data_str},
        {T38_TRANSPORT_BYPASS,      t38_transport_bypass_str},
	{-1, 0}
};

static char *data_transport_disable_str[]    = {"Transparent", NULL}; 
static char *data_voice_band_data_str[]      = {"Voice-Band-Data", NULL};
static char *data_transport_bypass_str[]     = {"Bypass", NULL};

code2str_ex_t data_transport_mode_t_str[]= {
        {DATA_TRANSPORT_DISABLE,    data_transport_disable_str},
        {DATA_VOICE_BAND_DATA,      data_voice_band_data_str},
        {DATA_TRANSPORT_BYPASS,     data_transport_bypass_str},
	{-1, 0}
};

static char *voip_destination_type_proxy_str[]  = {"proxy", "VOIP_DESTINATION_TYPE_PROXY", NULL}; 
static char *voip_destination_type_local_str[]  = {"local", "VOIP_DESTINATION_TYPE_LOCAL", NULL};
static char *voip_destination_type_direct_str[] = {"direct", "VOIP_DESTINATION_TYPE_DIRECT", NULL};

code2str_ex_t voip_destination_type_t_str[]= {
        {VOIP_DESTINATION_TYPE_PROXY,   voip_destination_type_proxy_str},
        {VOIP_DESTINATION_TYPE_LOCAL,   voip_destination_type_local_str},
        {VOIP_DESTINATION_TYPE_DIRECT,  voip_destination_type_direct_str},
	{-1, 0}
};

static char *auto_gain_ctl_mode_location_remote_str[] = {"For Remote User", "AUTO_GAIN_CTL_REMOTE", NULL};
static char *auto_gain_ctl_mode_location_local_str[]  = {"For Local User", "AUTO_GAIN_CTL_LOCAL", NULL};

code2str_ex_t auto_gain_ctl_mode_location_t_str[]= {
        {AUTO_GAIN_CTL_REMOTE,  auto_gain_ctl_mode_location_remote_str},
        {AUTO_GAIN_CTL_LOCAL,   auto_gain_ctl_mode_location_local_str},
	{-1, 0}
};

