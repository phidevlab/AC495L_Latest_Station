#ifndef _SERVER_COMS_H
#define _SERVER_COMS_H

#include "voip_main.h"


#define BROADCAST_PORT		"5001"
#define UNICAST_PORT        "5002"

#define FRAME_HEADER            "vccs"
#define PRTCL_HDR						"vccs"
#define PRTCL_HDR_LEN					4
#define PRTCL_VER                       1
#define PRTCL_VER_LEN           		2

#define DEVICE_REBOOT 	"reboot"
#define DEVICE_POWEROFF "poweroff"
#define SET_TIME_CMD 	"date -s"

#define WAKE_UP_FRAME                   00
#define SERVER_REG_BCAST                10
#define CONFIG_REQUEST                  11
#define INITIAL_CONFIGURATION_DOWNLOAD  12
#define GLOBAL_CONFIG_DOWNLOAD          13
#define CONFIG_OK_RESPONSE              14
#define GLOBAL_ALARM_REQ                15
#define INCOMING_GLOBAL_ALARM           16
#define GLOBAL_CALL_REQ                 17
#define INCOMING_GLOBAL_CALL            18
#define GLOBAL_RESET                    19
#define INTERCOM_STATE_CHANGE_FRAME     20
#define GROUP_INTERCOM_BREAK_IN_FRAME   22
#define ONLINE_LOCAL_CONFIG_DOWNLOAD    23
#define ONLINE_GLOBAL_CONFIG_DOWNLOAD   24
#define FSU_RESTART_FRAME               98

#define ICOM_PTT_SEND_REQ               30
#define ICOM_PTT_RELEASE_REQ            31
#define ICOM_CONT_STATUS_UPDATE         32
#define GROUP_CALL_REQUEST              33
#define GROUP_CALL_RESET_REQ            34
#define GROUP_INCOMING_CALL             35
#define GROUP_RESET                     36
#define GROUP_PTT_REQ                   37
#define GROUP_PTT_RELEASE               38
#define GROUP_CONT_STATUS_UPDATE        39
#define GROUP_INDIVIDUAL_RESET_FSU      40
#define DIAGNOSTICS_FRAME               41
#define DIAGNOSTICS_RESPONSE            42
#define ZONE_CALL_REQUEST 		        43
#define ZONE_ALM_REQUEST                44
#define ZONE_INCOMING_CALL		        45
#define ZONE_ALM_INCOMING_CALL          46
#define ZONE_RESET_REQ			        47
#define ZONE_RESET                      48

#define NEG_RESPONSE_FRAME_CMD          50
#define KEEP_ALIVE_FRAME                51
#define CONF_CALL_REQ                   62

#define ACK_RESPONSE_FRAME		        61
#define CONF_TIMER_UPDATE_FRAME         63
#define CONF_RESET_FRAME                64


#define SERVER_RECEIVE_REBOOT           44
#define SERVER_RECEIVE_SHUTDOWN         45



/************FRAME LENGTHS********************/
#define CONFIG_REQUEST_FRAME_LENGTH      20
#define CONFIG_OK_RESPONSE_FRAME_LENGTH  14
#define GLOBAL_ALARM_FRAME_LENGTH        20
#define GLOBAL_CALL_FRAME_LENGTH         20
#define GLOBAL_RESET_FRAME_LENGTH        20
#define ICOM_CALL_FRAME_LENGTH           20
#define GROUP_CALL_FRAME_LENGTH          20
#define KEEP_ALIVE_FRAME_LENGTH          25
#define DIAGNOSTICS_FRAME_LENGTH         20
#define ZONE_CALL_FRAME_LENGTH           20
#define CONF_CALL_FRAME_LENGTH           20





/********************************************************************************************************
* Structure name: server_coms_param_registration_bcast_t                     		      		     	*
* Description   : server ip and signalling multicast ip is received in broadcast frame from server      *
*                 to save thate info this structure is used                                             *
*				  														        	                    *
*********************************************************************************************************/

struct server_coms_param_registration_bcast_t {
	short cmd;
	short fsu;
	short mul_sig_ip[FOUR];
	short port;
	short ip[FOUR];


} __attribute__((packed)) server_coms_param_registration_bcast;

/********************************************************************************************************
* Structure name: server_coms_zone_alarm                     		      		     	                *
* Description   : This structure is used to extract infomation from incoming zone alarm frame           *
*				  														        	                    *
*********************************************************************************************************/

struct server_coms_zone_alarm{
    short cmd;
    short initiator_id;
    short alarm_module_id;
    short zone_no;
    short alarm_no;
    short channel_no;

} __attribute__((packed)) incoming_zone_alm_call;

/********************************************************************************************************
* Structure name: server_coms_global_alarm                     		      		     	                *
* Description   : This structure is used to extract infomation from incoming global alarm frame         *
*				  														        	                    *
*********************************************************************************************************/

struct server_coms_global_alarm{
    short cmd;
    short initiator_id;
    short alarm_module_id;
    short alarm_no;
    short channel_no;

} __attribute__((packed)) incoming_global_alm_call;

/********************************************************************************************************
* Structure name: server_coms_global_call                     		      		     	                *
* Description   : This structure is used to extract infomation from incoming global call  frame         *
*				  														        	                    *
*********************************************************************************************************/

struct server_coms_global_call{
    short cmd;
    short initiator_id;
 } __attribute__((packed)) incoming_global_call;


/********************************************************************************************************
* Structure name: server_coms_icom_call                     		      		     	                *
* Description   : This structure is used to extract infomation from ptt response  frame                 *
*				  														        	                    *
*********************************************************************************************************/
struct server_coms_icom_call
{
    short cmd;
    short initiator_id;
    short res;
}__attribute__((packed)) incoming_icom_res;

/********************************************************************************************************
* Structure name: server_coms_icom_call                     		      		     	                *
* Description   : This structure is used to extract infomation current icom contributor list            *
*				  														        	                    *
*********************************************************************************************************/
struct server_coms_icom_cont_status
{
    short cmd;
    short initiator_stn;
    short icom_no;
    short cont1;
    short cont2;
};

/********************************************************************************************************
* Structure name: server_coms_icom_start_stop_frame                     		      		            *
* Description   : This structure is used to extract infomation current icom no state                    *
*				  														        	                    *
*********************************************************************************************************/
struct server_coms_icom_start_stop_frame
{
    short cmd;
    short initiator_stn;
    short icom_no[THIRTY_TWO];
};

/********************************************************************************************************
* Structure name: server_coms_group_call_status                     		      		            *
* Description   : This structure is used to extract infomation current group call status                *
*				  														        	                    *
*********************************************************************************************************/
struct server_coms_group_call_status
{
    short cmd;
    short initiator_stn;
    short group_no;
    short cont1;
    short cont2;
    short response;
}server_coms_group_call_info[THIRTY_TWO];

/*
struct server_coms_conferance_call
{
   // short cmd;
    short initiator_stn;
    //short group_no;
//short cont1;
    //short cont2;
    //short response;

};
*/


/********************************************************************************************************
* Structure name: server_coms_zone_call_status                     		      		                    *
* Description   : This structure is used to extract infomation current zone call status                 *
*				  														        	                    *
*********************************************************************************************************/
struct server_coms_zone_call_status
{

    short cmd;
    short initiator_id;
    short alarm_module_id;
    short zone_no;
    short alarm_no;
    short channel_no;
}server_coms_zone_call_info[THIRTY_TWO];


/********************************************************************************************************
* Structure name: server_coms_group_call_reset_individual                     		      		            *
* Description   : This structure is used to extract infomation which station is has to reset from group *
*				  														        	                    *
*********************************************************************************************************/
struct server_coms_group_call_reset_individual
{
    short cmd;
    short initiator_stn;
    short group_no;

};

/***************************************************************************************************************
* Structure name: server_coms_icom_break_in_group_frame                     		      		               *
* Description   : This structure is used to extract infomation of which intercom has to reset after group call is
                  received.                                                                                    *
*				  														        	                           *
****************************************************************************************************************/
struct server_coms_icom_break_in_group_frame
{
    short cmd;
    short initiator;
    short contributor_list[64];
};



/***************************************************************************************************************
* Structure name: server_coms_diagnostics_frame                     		      		                       *
* Description   : This structure is used to extract infomation of diagnostics frame
                  received.                                                                                    *
*				  														        	                           *
****************************************************************************************************************/
struct server_coms_diagnostics_frame
{
    short cmd;
    short stn_id;
    short nw_staus;
    short pg_spk_status;
    short pvt_spk_status;
    short mic_status;
};

extern unsigned int    validate_frame(char *source);
extern void            construct_server_tx_request(int cmd_id);

#endif // _SERVER_COMS_H
