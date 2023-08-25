/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	voip_main.h                                        *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 28/02/08 - Coding started.                                       *
 *																	*
 ********************************************************************/

#ifndef _VOIP_MAIN_
#define _VOIP_MAIN_


#define ACL_VOIP_TOOLKIT_VERSION "2.6.2_b_1"

/* Called in case receiving internet connection or in case changing primary
 * parameters. These parameters are configuration parameters of the voip stack
 */

#define PAGING_BUFF_LEN 1024

#define PAGING_END_TIMEOUT	  1500  // the time we find stream lost.
#define PAGING_START_TIMEOUT  50  // every time we wait and check all call is disconnect or not when try to paging outgoing.
#define PAGING_PTIME          20
#define PVT_PTIME               20
#define PVT_CHANNEL           0
#define PG_CHANNEL            1
#define CONF_CHANNEL          2

#define NONE                  0
#define HANDSET               1
#define MIC                   3
#define HEADSET               2
#define SPEAKER               4

#define SET                   1
#define CLEAR                 0
#define WRITE                 1
#define READ                  0
#define START_TONE            1
#define STOP_TONE             0
#define DIAGNOSTIC_PIN_CHECK_CNTR 6

#define LIVE               '1'
#define DEAD               '0'
#define INVALID             3
#define VALID               2

#define NANOX               1
#define LED_OFF             0
#define LED_FLASH           1
#define LINE_BLANK          0
#define MSG_FLASH           1
#define CONTINUE_DISPLAY    2

#define LINE1               1
#define LINE2               2
#define LINE3               3
#define LINE4               4

#define LED1                1
#define LED2                2
#define LED3                3
#define LED4                4

#define LCD_CLR_FIRST_LINE                  1
#define LCD_CLR_SECOND_LINE                 2
#define LCD_CLR_BOTH_LINE                   3
#define LCD_CLR_11_POSITION                 4
#define LCD_CLR_FIRST_10_POSITION           5
#define NO_LCD_CLR                          0

#define LED                 1
#define LCD                 2

#define MIN_OFFSET         11
#define DIR_KEY_OFFSET     10
#define ICOM_OFFSET        00
#define GROUP_OFFSET       32
#define ZONE_OFFSET        64
#define AIM_OFFSET         96
#define GLOBAL_OFFSET      100


#define INIT_STATE              '1'// 1
#define VCCS_REGISTERING_STATE  '2'//2
#define SIP_REGISTERING_STATE   '3'//3
#define CONFIGURING_STATE       '4'//4
#define IDLE_STATE              '5'//5
#define P2P_OFF_HOOK_STATE          '6'//6
#define RINGBACK_STATE          '7'//7
#define P2P_STATE               '8'//8
#define PAGE_INITIATOR_STATE    '9'
#define P2P_RESET_STATE         'a'
#define INTERCOM_STATE          'b'
#define GROUP_STATE             'c'
#define ICOM_HOP_STATE          'd'
#define DIAGNOSTICS_STATE       'e'
#define DSP_RADV_INIT_STATE     'f'
#define SIP_REGISTERING_STATE_MANUAL  'g'
#define CONF_INIT_STATE         'h'
#define CONFERENCE_STATE        'i'
#define P2P_RESET_RINGBACK_STATE      'j'
#define GROUP_RESET_RINGBACK_STATE    'k'
#define ZONE_RESET_RINGBACK_STATE     'l'
#define PAGE_RESET_RINGBACK_STATE     'm'
#define INTERCOM_RESET_RINGBACK_STATE 'n'
#define CONF_RESET_RINGBACK_STATE     'o'
#define GROUP_CONTRIBUTOR_STATE       'p'
#define UNDER_MANUAL_SIP_REGISTRATION_STATE   'q'






/***********FSU EVENTS**************/
#define SHUTDOWN_EVENT          744
#define SHUTDOWN_FRAME         743
#define INIT_EVENT              1
#define APP_TIMEOUT_EVENT       2
#define VALID_FSU_REGISTER      3
#define VALID_SIP_REGISTER      4
#define PTT_SEND_EVENT          5
#define PTT_RESPONSE_EVENT      6
#define CONFIG_RECEIVED         7
#define SERVER_REG_BCAST_EVENT  8
#define VALID_KEY_SEQ_EVENT     9
#define P2P_CALL_EVENT          10
#define P2P_RESET_EVENT         11
#define WRONG_CONFIG            12
#define RINGING_OFF_HOOK_EVENT  13
#define CALL_OFF_HOOK_EVENT     14
#define PVT_CALL_TIMEOUT        15
#define GLOBAL_ALM_EVENT        16
#define PG_CALL_TIMEOUT         17
#define GLOBAL_CALL_EVENT       18
#define GLOBAL_RESET_EVENT      19
#define ICOM_CALL_EVENT             20
#define ICOM_RESET_EVENT            21
#define NEG_RES_EVENT               22
#define WAKE_UP_EVENT               23
#define INCOMING_GROUP_CALL_EVENT   24
#define GROUP_RESET_EVENT           25
#define GRP_PTT_REQUEST_EVENT       26
#define GLOBAL_RX_ALM_EVENT         27
#define GLOBAL_RX_CALL_EVENT        28
#define GLOBAL_RESET_RX_EVENT       29
#define GRP_INDIVIDUAL_RESET_EVENT  30
#define DIAGNOSTIC_EVENT            31
#define DIAGNOSTIC_PVT_OK           32
#define DIAGNOSTIC_PVT_NOT_OK       33
#define DIAGNOSTIC_SPK_OK           34
#define DIAGNOSTIC_SPK_NOT_OK       35
#define ICOM_STATE_CHANGE_EVENT     36
#define ACK_RESPONSE			    37
#define GRP_PTT_RELEASE_EVENT       38
#define INCOMING_ZONE_CALL_EVENT    39
#define INCOMING_ZONE_CALL_RX_EVENT 40
#define INCOMING_ZONE_ALM_EVENT     41
#define INCOMING_ZONE_ALM_RX_EVENT  42
#define ZONE_RESET_EVENT		    43
#define ZONE_RESET_RX_EVENT         44
#define CONF_ADD_EVENT              45
#define  CONF_TIMER_UPDATE_EVENT    46
#define PTT_RELEASE_EVENT           47
#define BROADCAST_TIMEOUT_EVENT     48
#define P2P_BRK_RESET_EVENT         49
#define NO_P2P_RESPONSE_EVENT       50
#define P2P_RESET_PVTTIMEOUT_EVENT  51
#define ONE_SEC_TIMEOUT             50
#define VALID_SW_PTT_EVENT          52

#define      SRE_MUTE_EVENT         52 ///
#define      SRE_UNMUTE_EVENT       53 ///
#define      HANDSET_RESET_KEY       47

#define      SRE_MUTE               48
#define     PTT_MIC_KEY             49
/**********CALL TYPES*****************/

#define         P2P             '1'
#define         GROUP           '2'
#define         INTERCOM_RX     '3'
#define         INTERCOM_TX_RX  '4'
#define         GLOBAL          '5'
#define         GLOBAL_ALARM    '6'
#define         ZONE            '7'
#define         ZONE_ALARM      '8'
#define         AIM             '9'
#define         EPBX            'e'
#define         CONFERENCE      'c'
#define         RESET           '0'
#define         GROUP_TX_RX     'd'



/**********************************/
#define MAX_ETH_FRAME_LENGTH 1600
#define UI_MSG_LENGTH        20
//#define SERVER_LIVE_TIMEOUT  30
#define SERVER_LIVE_TIMEOUT  (30 * APP_TIMER_MULTIPLIER)
#define MAX_PG_VOLUME        12
#define MAX_PVT_VOLUME       13
#define MAX_MUSIC_VOLUME     10
//#define KEEP_ALIVE_TIMEOUT    8 //60
#define KEEP_ALIVE_TIMEOUT    (8 * APP_TIMER_MULTIPLIER)
//#define CONFIGURATION_RESPONSE_TIMEOUT     20
#define CONFIGURATION_RESPONSE_TIMEOUT      (20 * APP_TIMER_MULTIPLIER)

//#define SERVER_RESPONSE_TIMEOUT        40
#define SERVER_RESPONSE_TIMEOUT        (40 * APP_TIMER_MULTIPLIER)
#define SPEECH_DETECTION_TIMEOUT       (40 * APP_TIMER_MULTIPLIER)

#define SIP_RESET_TIMEOUT        (4 * APP_TIMER_MULTIPLIER)

#define   ZERO            0
#define   ONE             1
#define   TWO             2
#define   THREE           3
#define   FOUR            4
#define   FIVE            5
#define   SIX             6
#define   SEVEN           7
#define   EIGHT           8
#define   NINE            9
#define   TEN             10
#define   ELEVEN          11
#define   TWELVE          12
#define   THIRTEEN        13
#define   FOURTEEN        14
#define   FIFTEEN         15
#define   SIXTEEN         16
#define   SEVENTEEN       17
#define   EIGHTEEN        18
#define   NINETEEN        19
#define   TWENTY          20
#define   TWENTY_ONE      21
#define   TWENTY_TWO      22
#define   TWENTY_THREE    23
#define   TWENTY_FOUR     24
#define   TWENTY_FIVE     25
#define   TWENTY_SIX      26
#define   TWENTY_SEVEN    27
#define   TWENTY_EIGHT    28
#define   TWENTY_NINE     29
#define   THIRTY          30
#define   THIRTY_ONE      31
#define   THIRTY_TWO      32
#define   THIRTY_THREE    33
#define   THIRTY_FOUR     34
#define   THIRTY_FIVE     35
#define   THIRTY_SIX      36
#define   THIRTY_SEVEN    37
#define   THIRTY_EIGHT    38
#define   THIRTY_NINE     39
#define   FOURTY          40
#define   FOURTY_ONE      41
#define   FOURTY_TWO      42
#define   FOURTY_THREE    43
#define   FOURTY_FOUR     44
#define   FOURTY_FIVE     45
#define   FOURTY_SIX      46
#define   FOURTY_SEVEN    47
#define   FOURTY_EIGHT    48
#define   FOURTY_NINE     49
#define   FIFTY           50
#define   MAX_DIRECT_CALL_KEY_INFO_SIZE  250
#define MAX_BUFFER_SIZE           256
#define MAX_ICOM_DETAILS_SIZE     270
#define MAX_CLIENTS               5
#define DIAGNOSTIC_TIMER_PVT_PG   500
#define THOUSAND                  1000


#define APP_TIMER_MULTIPLIER               25
//#define APP_TIMER_MULTIPLIER               1


/**********variable declear for LCD*********/
int lcd_zone_call_no;
int lcd_zone_call_init;

int lcd_alm_zone_no;
int lcd_zone_alm_init;

int lcd_global_call_no;
int lcd_global_call_init;

int lcd_global_alm_no;
int lcd_global_alm_init;

int lcd_group_call_no;
int lcd_group_call_init;

int lcd_icom_no;

int lcd_conf_init;
int lcd_fcs_desc;
int lcd_fcs_default_icom;

char g_arcPvtCallTimer[TWENTY];
char g_arcPvtCallTimer_fr_pg_init[TWENTY];
char g_arcPgCallTimer[TWENTY];
// extern int lcd_zone_call_no_show;


int conf_cmd;
unsigned int          g_uiPvt_call_timer_ui_lcd;


int PvtCallTime_min;
int PvtCallTime_sec;


int PgCallIniTime_min;
int PgCallIniTime_sec;

int PgCallTime_min;
int PgCallTime_sec;


/**************IO EXPANDER OUTPUTS**********/

#define  ICOM_LED_OFF            0xFE
#define  CALL_THROUGH_LED_OFF    0xFD
#define  LED_SERVER_IND_OFF      0xFB
#define  PFC_ACT_OFF             0xF7
#define  PVT_AMP_MUTE_OFF        0xEF
#define  PG_AMP_MUTE_OFF         0xDF
#define  ACT_PVT_PG_MIX_OFF      0xBF
#define  ACT_PG_PVT_MIX_OFF      0x7F

#define  ICOM_LED_ON           0x01
#define  CALL_THROUGH_LED_ON   0x02
#define  LED_SERVER_IND_ON     0x04
#define  PFC_ACT_ON            0x08
#define  PVT_AMP_MUTE_ON       0x10
#define  PG_AMP_MUTE_ON        0x20
#define  ACT_PVT_PG_MIX_ON     0x40
#define  ACT_PG_PVT_MIX_ON     0x80


/**************IO EXPANDER INPUTS**********/

/*************DIAGNOSTICS / MIC IC INPUTS*/


#define  MIC_MUTE_OFF            0xFE
#define  DIAG_MIC_OFF               0xFD
#define  UDIG_PVT_OFF               0xFB
#define  UDIG_PG_OFF                0xF7

#define  MIC_MUTE_ON                 0x01
#define  DIAG_MIC_ON               0x02
#define  UDIG_PVT_ON               0x04
#define  UDIG_PG_ON                0x08

/*************DIAGNOSTICS / MIC IC INPUTS*/

#define  MIC_PTT_SW              1
#define  SPEECH_DETECT           2
#define  HOOK_SW_SENSE           3
#define  AUDIO_OUT_CHK           4
#define  PVT_RX_CHK              5
#define  PAGE_RX_CHECK           6
#define  SRE_MUTE_ACT            7

/***********GPIO********************/
#define           GPIO_ON           1
#define           GPIO_OFF          0

//#define           PTT_SW            0
//#define               0 ///Changes as per new schematic on 26-02-22
#define           PTT_SPEECH        1
#define           ICOM_STATUS       3     //2
#define           LED_LAN_SERV      2      //3
#define           MB_AMP_MUTE       4 ///Changes as per new schematic on 26-02-22
 ///Changes as per new schematic on 26-02-22


#define           ICOM_MIX_ACT      27
//#define           MB_AMP_MUTE       6

#define           ICOM_AMP_MUTE     7
#define           PFC_ACT           8
#define           ICOM_MIX_ACT      27  ///Changes as per new schematic on 26-02-22

#define           PAGE_RX_CHK       31

#define           MIC_DIAG          33 ///Changes as per new schematic on 26-02-22
#define           SOFT_RESET        34
#define           LCD_BACKLIGHT     39

/*************************************/

#define PAGE_CALL_PORT 7002
#define PVT_CALL_PORT  7001

#define SERVER_IP_STRING "voip/signalling/sip/proxy_address="

extern int Channel0_rtpsfd;
void select_codec(int ch,int codec);
void volume_change(void);
void close_channel(channel);
void voip_stack_start(void);
void drop_membership(int fd);
void add_membership(int fd);
void start_alarm(int alarm_no);
void stop_alarm(int alarm_no);
void sendCompleteString(char *str);

extern void crate_global_paging_socket();
extern void multicast_prepare_dsp(int channel);
extern void *ui_thread(void *data);
extern void *gpio_input_thread();
extern void aclVoipRtpUtils_ChannelAddrSet(char* addr, unsigned short port, int channel);
extern void aclVoipRtpUtils_ChannelConnect_phi(int channel);
extern void ACL_sysGpioFuncSet(int pinSelReg,int shift,int func);
extern void acl_rtp_unbind(int dspChannel);
extern void save_mul_starting_address();

extern int setchannelparamters_Phi(int ch,int payload, int ptime, int dtmf_inband,int dtmfplayload);
extern int send_to_phone_input_cb(int pressed, int event);
extern int CreateConnection_phi(int ch,char *buf);
extern int registration_to_sip_server();

int is_line_enabled(int line);//060530 Avner phase 1
int is_using_proxy(void);//060530 Avner phase 1

extern struct sockaddr_un    g_cli_addr;
extern struct sockaddr_un    g_app_ui_addr;
extern struct sockaddr_un    radv_sock_addr;
extern struct sockaddr_un    radv_sock_cli_addr;

extern struct sockaddr_in    udp_g_cServer_addr;
extern struct sockaddr_in    reg_addr;
extern struct sockaddr_in    mul_sig_addr;

extern struct sigaction      action;
extern struct sigaction      info;

unsigned int g_uiConfig_Received;



/************************************************************************************
* Structure name: UI_msg															*
* Description   : Message from Main application is copied							*
*				  in this structure													*
*************************************************************************************/
struct UI_msg{

   short int parameter;
   short int addr;
   short int line_no;
   short int cmd;
   char text[TWENTY_ONE];
   char state;
   int lcd_clear_bit;
   int position;
   short int flash_cntr;//[FOUR];
   short int display;
}ui_msg;

/************************************************************************************
* Structure name: lcd_message										       			*
* Description   : Message to displayed according line no is copied					*
*				  in this structure													*
*************************************************************************************/
struct lcd_message_t {
	char line1_Msg[TWENTY_ONE];
	char line2_Msg[TWENTY_ONE];
	char line3_Msg[TWENTY_ONE];
	char line4_Msg[TWENTY_ONE];
}lcd_message;

/************************************************************************************
* Structure name: lcd_msg										       			*
* Description   : Message to displayed on the lcd screen is copied					*
*				  in this structure													*
*************************************************************************************/
struct ui_lcd_msg{

   short int parameter;
   int lcd_clr_bit;
   short int line_no;
 //  short int led_no;
   short int cmd;
   char text[TWENTY_ONE];
   short int position;
   short int flash_cntr[FOUR];
 }lcd_msg;

/************************************************************************************
* Structure name: led_msg										       			*
* Description   : LED glow information  is copied                					*
*				  in this structure													*
*************************************************************************************/
 /*struct ui_led_msg{

   short int parameter;
   short int led_no;
   short int cmd;
   char text[TWENTY_ONE]; //because from voipapp this is common so incase of led this field contains zero
   char state;
   short int flash_cntr;//[FOUR];
 }led_msg;*/

/************************************************************************************
* Structure name: RTPHeader								       		       			*
* Description   : Voice frame from network is copied              					*
*				  in this structure													*
*************************************************************************************/
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

/************************************************************************************
* Structure name: RTCPHeader								       		       		*
* Description   : Voice frame from network is copied              					*
*				  in this structure													*
*************************************************************************************/
typedef struct
{
   unsigned int bits;
   unsigned int ssrc;
} RTCPHeader;


/************************************************************************************
* Structure name: PAGINGSTATE								     	       			*
* Description   : Paging state are maintained in this enum       					*
*				  														        	*
*************************************************************************************/
typedef enum{
	PAGE_IDLE,
	PRIVATE_IDLE,
	PRIVATE_INCOMING_WAITING,
	PAGE_INCOMING_ONGOING,
	PRIVATE_INCOMING_ONGOING,
	PRIVATE_OUTGOING_ONGOING,
	PAGE_OUTGOING_ONGOING,
	P_STATE_MAX
}PAGINGSTATE;

/************************************************************************************
* Structure name: PAGING_GROUP								     	       			*
* Description   : Multicasting Paging group are maintained in this structure     	*
*				  														        	*
*************************************************************************************/
typedef struct
{
    char    enable;
    char    address[TWENTY];
    int     port;
}PAGING_GROUP;

/************************************************************************************
* Structure name: PAGING_CONFIG								     	       			*
* Description   : Multicasting Paging configuration is								*
*				  maintained in this structure     									*
*				  														        	*
*************************************************************************************/
typedef struct
{
    char    enable;
    int     payload;
    PAGING_GROUP group[TEN];
}PAGING_CONFIG;

/************************************************************************************
* Structure name: voip_paging_params_t								     	        *
* Description   : Multicasting group status is maintained                         	*
*				  														        	*
*************************************************************************************/
typedef struct {
   char  isEnabled;
   char  address[TEN][TWENTY];
   int   port[TEN];
} voip_paging_params_t;

/************************************************************************************
* Structure name: PAGING_STREAM								     	                *
* Description   : Received multicasting audio frame is saved in this structure    	*
*				  														        	*
*************************************************************************************/
typedef struct
{
    unsigned int     SSRC;
    unsigned int     group;
    unsigned int     count;
    unsigned int     timer_record_count;
    char             ignore;
}PAGING_STREAM;

/************************************************************************************
* Structure name: PAGING_STREAM								     	                *
* Description   : Received multicasting audio frame is saved in this structure    	*
*				  														        	*
*************************************************************************************/
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
    PAGINGSTATE      private_state;
    PAGINGSTATE      page_state;
    PAGING_STREAM    stream[FIVE];
}PAGING_WORKING;

/************************************************************************************
* Structure name: conection_mode_t								     	            *
* Description   : RTP connection mode maintained in this structure               	*
*				  														        	*
*************************************************************************************/

typedef enum {

    CONN_MODE_LOCAL,
    CONN_MODE_REMOTE,

} conection_mode_t;

/************************************************************************************
* Structure name: gpio_ops								          	       			*
* Description   : GPIOs controlled using this structure								*
*				  							     									*
*				  														        	*
*************************************************************************************/
struct gpio_operations{
   unsigned int gpio_no;        // GPIO No.
   unsigned int gpio_command;   // GPIO Command - 0- get Data and 1- set Data.
   unsigned int gpio_value;	// GPIO value.
}gpio_ops;

/************************************************************************************
* Structure name: station_configuration							  	       			*
* Description   : Configuration received from server is copied 						*
*				  in this structure			     									*
*				  														        	*
*************************************************************************************/
struct station_configuration{

    short data_length;
    short cmd_id;
    short logical_id;
    char  fcs_desc[SIXTEEN];
    short fcs_password;
    short default_intercom;
    short default_AIM;
    char  icom_multicast_start_addr[TWENTY];
    char  group_multicast_start_addr[TWENTY];
    char  zone_multicast_start_addr[TWENTY];
    char  aim_multicast_start_addr[TWENTY];
    char  global_multicast_start_addr[TWENTY];
    short mic_min;
    short mic_max;
    short pvt_min;
    short pvt_max;
    short pg_min;
    short pg_max;
    short music_min;
    short music_max;
    short call_permission_channel_hop;
    short call_permission_group_call;
    short call_permission_zone_call;
    short call_permission_global_call;
    short call_permission_zone_alarm_call;
    short call_permission_global_alarm_call;
    short call_permission_internal_epabx;
    short call_permission_external_epabx;
    short comm_mode_type;
    char  icom_live_dead_status[THIRTY_TWO];
    char  icom_contributor_details[MAX_ICOM_DETAILS_SIZE];
    short no_of_configured_groups;
    char  group_initiator[THIRTY_TWO];
    char  group_member_info[THIRTY_TWO];
    short no_of_configured_zones;
    char  zone_initiator[THIRTY_TWO];
    char  zone_member_info[THIRTY_TWO];
    char  direct_call_key_details[THIRTY_TWO];
    char  direct_call_keys_info[MAX_DIRECT_CALL_KEY_INFO_SIZE];
    short recordflag;
    short pvt_call_timeout;
    short pg_call_timeout;
    short alarm_call_timeout;
    short dial_key_lim_internal;
    short dial_key_lim_external;
    short c8_mode;
    char  timetick[THIRTY_TWO];
    char  Reserved[FIFTEEN];
    char  footer[FOUR];
    }stn_config;


/************************************************************************************
* Structure name: private_call_status							  	       			*
* Description   : PVT call current status is maintained      						*
*				  in this structure			     									*
*				  														        	*
*************************************************************************************/
struct private_call_status // STATUS_DATA_FOR_PRIVATE_CALL:EPBX,PTP,GROUP,CHANNEL_CALL
{
  unsigned char type;
  unsigned int  curr_icom_no;
  unsigned int  group_no;
  unsigned char pvt_call_busy;
   unsigned int initiator;
  char initiator_ip[15];
  char destination_ip[15];
}pvt_call;

/************************************************************************************
* Structure name: page_call_status							    	       			*
* Description   : PAGE call current status is maintained      						*
*				  in this structure			     									*
*				  														        	*
*************************************************************************************/
struct page_call_status // STATUS_DATA_FOR_PAGE_CALL: ZONE_CALL, ZONE_ALARM, GLOBAL_CALL, GLOBAL_ALARM
{
    unsigned char type;
    unsigned int  alm_no;
    unsigned int  zone_no;
    unsigned char pg_call_busy;
}pg_call;

/************************************************************************************
* Structure name: icom_contributor_details							    	 		*
* Description   : INTERCOM call current contributor station id are maintained		*
*				  in this structure			     									*
*				  														        	*
*************************************************************************************/
struct icom_contributor_details{
   int cont1;
   int cont2;
}icom_contributor_status_info[THIRTY_TWO];

/************************************************************************************
* Structure name: group_contributor_details							    			*
* Description   : GROUP call current contributor station id are maintained			*
*				  in this structure			     									*
*				  														        	*
*************************************************************************************/
struct group_contributor_details{
   int cont1;
   int cont2;

}group_contributor_status_info[THIRTY_TWO];

/************************************************************************************
* Structure name: dir_key_info							    	       		     	*
* Description   : Direct call key information is maintained      				    *
*				  in this structure	(i.e first dirckey=P101)	     				*
*				  														        	*
*************************************************************************************/
struct dir_key_info
{
    char call_type[TWO];
    char g_cCaller[FIVE];
    char number[FOUR];

}dir_info[THIRTY_TWO];





extern unsigned int g_uiMul_sig_addrlen;
extern unsigned int g_uiUdp_comm_addrlen;
extern unsigned int g_uiServ_udp_comm_fd;
extern unsigned int g_uiRadv_sock_cli_addrlen;
extern unsigned int g_uiRadv_addrlen;
extern unsigned int g_uiApp_ui_addrlen;
extern unsigned int g_uiReg_addrlen;
extern unsigned int g_uiRv_sock_cli_fd;
extern unsigned int g_uiClilen;
extern unsigned int g_uiPvt_call_timer;
extern unsigned int g_uiPage_call_timer;
extern unsigned int g_uiPage_call_timer_frlcd_ini;
extern unsigned int g_uiPage_call_timer_lcd;
extern unsigned int g_uiFsu_event;

///added by shraddha
extern unsigned int g_uiFsu_Broadcast_event;
extern unsigned int g_uiFsu_timer_event;
extern unsigned int g_uiFsu_mcast_event;
///added by shraddha

extern unsigned int g_uiFsu_state;
extern unsigned int g_uiUi_socket_fd;
extern unsigned int g_uiApp_timer_fd;
extern unsigned int g_uiVoipapp_wait_timer;
extern unsigned int g_uiServer_fd;
extern unsigned int g_uiClient_connection_fd;
extern unsigned int g_uiServer_comm_fd;
extern unsigned int g_uiBroadcast_fd;
extern unsigned int g_uiPjsip_fd;
extern unsigned int g_uiUi_socket_fd;
extern unsigned int g_uiRadv_sock_fd;
extern unsigned int g_uiIncoming_pg_port;
extern unsigned int g_uiDiagnostic_start_flag;
extern unsigned int g_uiApp_diag_timer_fd;
extern unsigned int g_uiMulticast_signalling_fd;
//extern unsigned int g_uiMulticast_signalling_fd_sample;
extern unsigned int g_uiFsu_event;
extern unsigned int g_uiFsu_state;
extern unsigned int g_uiUi_socket_fd;
extern unsigned int g_uiApp_timer_fd;
extern unsigned int g_uiVoipapp_wait_timer;
extern unsigned int g_uiServer_fd;
extern unsigned int g_uiClient_connection_fd;
extern unsigned int g_uiBroadcast_fd;
extern unsigned int g_uiPjsip_fd;
extern unsigned int g_uiPvt_call_timer;
extern unsigned int pjsua_recv_fd;

extern int 			g_iIcom_mul_addr_start_offset;
extern int          g_igroup_mul_addr_start_offset;
extern int          g_iglobal_mul_addr_start_offset;
extern int          g_izone_mul_addr_start_offset;
extern int          g_iaim_mul_addr_start_offset;
extern int          g_iPaging_fd;
extern int          g_Iicom_fds[THIRTY_TWO];
extern int          g_iGlobal_mem_dropped;
extern int          g_iAim_mem_dropped;
extern int          g_iIcom_mem_dropped;
extern int          g_iDiagnostic_pvt_status;
extern int          g_iDiagnostic_spk_status;
extern int          g_iDiagnostic_mic_status;
extern int          g_iDiagnostic_nw_status;
extern int          g_iAim_fds[FOUR];
extern int          g_igroup_fds[THIRTY_TWO];
extern int          g_izone_fds[THIRTY_TWO];
extern int          g_iAcl_main_Voip_Task_AgentSocketFd;  // 20 jan 2023
extern int          g_iHandset_lifted_signal;

extern char g_cLine1_buf[TWENTY];
extern char g_cLine2_buf[TWENTY];
extern char g_cLine3_buf[TWENTY];
extern char g_cLine4_buf[TWENTY];
extern char g_cReceived_key_seq[TWELVE];
extern char call_type[FIVE];
extern char g_cCaller[FIVE];
extern char called_station[FIVE];
extern char g_cCalled_station[5];
extern char g_cServer_addr[THIRTY];
extern char g_cMul_signalling_ip[THIRTY];
extern char g_cSip_ok_flag;
extern char voice_buf[MAX_ETH_FRAME_LENGTH];
extern char gc_icom_mul_start_addr[TWENTY];
extern char gc_group_mul_start_addr[TWENTY];
extern char gc_global_mul_start_addr[TWENTY];
extern char gc_zone_mul_start_addr[TWENTY];
extern char gc_aim_mul_start_addr[TWENTY];
extern char g_cMulticast_tx_address[THIRTY];


extern fd_set app_readset,app_testset;
extern pthread_mutex_t lock_icom_contr,lock_call_type,lock_group_contr;

sigset_t pselect_set;
int g_itransmit_fd_index;
PAGING_WORKING   pWork_m;


void toggle_sre_gpio();
void print_msg_on_lcd(short command, char message[50], unsigned int line_number, unsigned int start_position, unsigned int lcd_line_status);

#endif	/* _VOIP_MAIN_ */
