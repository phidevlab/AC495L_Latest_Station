/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	voip_main.c                                         *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 28/02/08 - Coding started.                                       *
 *																	*
 ********************************************************************/

#include <unistd.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include "acl_MsgQ.h"
#include "voip_main.h"
#include "acl_ui_commands.h"
#include "acl_line_connection.h"
#include "acl_voip_conf.h"
#include "voip_utils.h"
#include "voip_rtp_utils.h"
#include "acl_dsp_telephony_commands.h"
#include "acl_log.h"
#include "voip_status_handler.h"

#include "cip_def.h"
#include "cip_api.h"
#include "cfg_api.h"
#include "server_coms.h"
#include "config_manager.h"
#include "keypad_interface.h"
#include "acl_call_mngr.h"
#include "cip_def.h"
#include "cip_api.h"
#include "cfg_api.h"
#include "iphone_paging_pub.h"
#include "lcd_messages.h"
#include "ui.h"

#if defined(AC494_SDP_IPPHONE) || defined(AC49_ORCHID_VOIP_TOOLKIT)

#ifdef CONFIG_GTEK
#include "IPPMessageQueue.h"
#else
#include "iphone_hwKeypad.h"

#ifdef NANOX
//#include "nanox_hwLcd.h"
#else
#include "iphone_hwLcd.h"
#endif

#endif

#endif

/*#define  VOIP_MAIN_DEBUG*/
#define  VOIP_MAIN_DEBUG 1
#ifdef VOIP_MAIN_DEBUG
#define DPRINTK(format,args...) printf("[%s:%4d] " format, __FUNCTION__, __LINE__, ##args)
#else
#define DPRINTK(format,args...)
#endif
#define MAX_LINE_LENGTH           20
#define MAX_VALID_KEY_SEQ         12
#define MAX_IP_LENGTH             30
#define MAX_STN_ID_LENGTH         5
#define MAX_APP_FDS               100


static pthread_mutex_t  		event_loop_mut;
static pthread_cond_t 		event_loop_cond;

static int			needToSendWaitForBufferMessage;
static int			isFirstLoading;
static int			waitUntilCfgBufArrives = 1;
static int			waitUntilVoipTaskReadyForReconfig = 1;

int						acl_main_Voip_Task_AgentSocketFd;


struct sockaddr_un    g_cli_addr;
struct sockaddr_un    g_app_ui_addr;
struct sockaddr_un    radv_sock_addr;
struct sockaddr_un    radv_sock_cli_addr;
struct sockaddr_un    acl_main_Voip_Task_Agent_Remote_Addr;
struct sockaddr_in    udp_g_cServer_addr;
struct sockaddr_in    udp_comm_addr;
struct sockaddr_in    reg_addr;
struct sockaddr_in    addr;
struct sigaction      action;
struct sigaction      info;

fd_set                app_readset;
fd_set                app_testset;
sigset_t              pselect_set;

pthread_mutex_t       lock_icom_contr;
pthread_mutex_t       lock_call_type;
pthread_mutex_t       lock_group_contr;
acl_call_mngr_ops_t   *call_mgt;
rtp_session_t         *rtp_sessions;
u8                    rtp_tos;

int                    	DspFirmwareName;
char			acl_cfgfilename[CONFIGURATION_FILE_NAME_SIZE];
/*the configuration buffer that will hold the configuration data */
extern char receivedCfgBuf[CONFIGURATION_BUFFER_MAX_LEN];

#ifdef CONFIG_RG_VOIP_RV_SIP
extern int IppThreadSleep(int sec, int msec);
#endif
int i_n;
int i_p;
extern TableEnt *Get_Table_Entry(CIP_OPCODES opcode);

int mss_clamping_limit;

static u16 local_host_port;
static u16 local_host_range;
static u8 *port_used;
/* Parameters defined here and kept in underlying layers until voip task stops
 */
static acl_call_mngr_params_t call_mngr_params;
static voip_dsp_advanced_channel_configuration_t   AdvancedChannelConfiguration;


/* Prototypes */
static void event_loop_init();
static void event_loop_uninit();
static void event_loop();
static void load_dsp_kernel_modules();
#ifdef CONFIG_RG_ATA
static void load_slic_related_kernel_modules();
#endif /*CONFIG_RG_ATA*/


//////Phi declarations//////////////

unsigned int          g_uiMul_sig_addrlen;
unsigned int          g_uiUdp_comm_addrlen;
unsigned int          g_uiServ_udp_comm_fd;
unsigned int          g_uiRadv_sock_cli_addrlen;
unsigned int          g_uiRadv_addrlen;
unsigned int          g_uiApp_ui_addrlen;
unsigned int          g_uiReg_addrlen;
unsigned int          g_uiRv_sock_cli_fd=CLEAR;
unsigned int          g_uiClilen;
unsigned int          g_uiPvt_call_timer;
//unsigned int          g_uiPvt_call_timer_ui_lcd;

unsigned int          g_uiPage_call_timer;
unsigned int          g_uiPage_call_timer_lcd;
unsigned int          g_uiPage_call_timer_frlcd_ini;
unsigned int          g_uiPlay_tone_timer;
unsigned int          g_uiFsu_event;
unsigned int          g_uiFsu_state;
unsigned int          g_uiUi_socket_fd;
unsigned int          g_uiApp_timer_fd;
unsigned int          g_uiApp_diag_timer_fd;
unsigned int          g_uiVoipapp_wait_timer;
unsigned int          g_uiServer_fd;
unsigned int          g_uiClient_connection_fd;
unsigned int          g_uiIncoming_pg_port;
unsigned int          g_uiIcom_led_on_flag;
unsigned int          p2p_call_process_timer_flag;   ///ADDED BY AMBARISHK
unsigned int          g_uiGroup_active_contr_flag=CLEAR;
unsigned int          g_uiPvt_diagnostic_timer;
unsigned int          g_uiSpk_diagnostic_timer;
unsigned int          g_uiDiagnostic_start_flag;
unsigned int          g_uiServer_comm_fd;
unsigned int          g_uiBroadcast_fd;
unsigned int          g_uiPjsip_fd;
unsigned int          g_uiUi_socket_fd;
unsigned int          g_uiRadv_sock_fd;
unsigned int          g_uiMulticast_signalling_fd;
unsigned int          g_uiServer_live_status_timer;
unsigned int          g_uiKeep_alive_timer;
unsigned int          g_uiConfig_Received=CLEAR;
unsigned int          g_uiSpeech_detection_timer;

int                   g_iAim_mem_dropped=-1;
int                   g_iIcom_mem_dropped=-1;
int                   g_iDiagnostic_pvt_status;
int                   g_iDiagnostic_spk_status;
int                   g_iDiagnostic_mic_status;
int                   g_iDiagnostic_nw_status;
int                   g_iGpio_fd;
int                   g_iIcom_hop_flag;
int                   g_iHandset_lifted_signal=CLEAR;
int                   g_iAcl_main_Voip_Task_AgentSocketFd;
int                   g_iDspFirmwareName;
int                   g_imss_clamping_limit;

char                  g_cLine1_buf[MAX_LINE_LENGTH];
char                  g_cLine2_buf[MAX_LINE_LENGTH];
char                  g_cLine3_buf[MAX_LINE_LENGTH];
char                  g_cLine4_buf[MAX_LINE_LENGTH];
char                  g_cReceived_key_seq[MAX_VALID_KEY_SEQ];
char                  g_cCall_type[MAX_STN_ID_LENGTH];
char                  g_cCaller[MAX_STN_ID_LENGTH];
char                  g_cCalled_station[MAX_STN_ID_LENGTH];
char                  g_cServer_addr[MAX_IP_LENGTH];
char                  g_cMul_signalling_ip[MAX_IP_LENGTH];
char                  g_cSip_ok_flag=CLEAR;
char                  g_cInit_app=SET;
char                  g_cMulticast_tx_address[MAX_IP_LENGTH];
char				  g_cAcl_cfgfilename[CONFIGURATION_FILE_NAME_SIZE];
/*the configuration buffer that will hold the configuration data */



static int            g_siFsu_initialized=CLEAR;


int first_broadcast_flag=ZERO;


void launch_cfg_listening_socket_thread();
void *cfg_listening_socket(void *dummy);

int                   g_iGpio_fd;

char		      g_cAcl_cfgfilename[CONFIGURATION_FILE_NAME_SIZE];

char io_led[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
char io_led_clear[]={0x04};

///ADDED BY AMBARISHK start
unsigned int last_req_p2p_sent_to_server;
unsigned int p2p_req_retransmission_count;

unsigned int last_req_sent_to_server;
unsigned int req_retransmission_count;
///ADDED BY AMBARISHK end

#include "iphone_localNetTerm.h"
/**************************************************************************************
* Function name: g_siLocal_host_ports_alloc                                               *
* Input arg    : short port,short range                                               *
* Output arg   : NONE                                                                 *
* Description  : Allocates the memory for the port range                              *
* Developer-Note: Unmodified Source block. Received as a part of AUDIOCODES library used as it is without change.
***************************************************************************************/

static int g_siLocal_host_ports_alloc(u16 port, u16 range)
{
    nfree(port_used);
    if (!(port_used = AC_ZALLOC_L(range)))
	return -1;

    local_host_port = port;
    local_host_range = range;

    return 0;
}


/**************************************************************************************
* Function name: g_siLocal_host_ports_free                                                *
* Input arg    : void                                                                 *
* Output arg   : NONE                                                                 *
* Description  : Free the memory allocated for the port                               *
* Developer-Note: Unmodified Source block. Received as a part of AUDIOCODES library used as it is without change.                                                                                   *
***************************************************************************************/

static void g_siLocal_host_ports_free(void)
{
    nfree(port_used);
}

/**************************************************************************************
* Function name: reserved_port_get                                                    *
* Input arg    : void                                                                 *
* Output arg   : short                                                                 *
* Description  : Get the list of reserved ports in the system                         *
* Developer-Note: Unmodified Source block. Received as a part of AUDIOCODES library used as it is without change.                                                                                    *
***************************************************************************************/

u16 reserved_port_get(void)
{
    u16 idx;
    /* make sure we cycle through the whole range, instead of returning the same
     * ports again and again, by keeping a static pointer.
     * This is important since radvision keeps lingering sockets. */
    static u16 next_to_use = 0;
    int cur_pos = 0;

    /* port must be predefined only in case the firewall is present, otherwise
     * we may use any free port */
    if (!vcCONFIG_RG_FIREWALL)
	return 0;

    /* loop through the port array, starting from the place marked by
     * next_to_use. 'idx' is used to count the number of ports searched,
     * to avoid an infinite loop */
    for (idx = 0; idx < local_host_range; idx++)
    {
	cur_pos = (next_to_use + idx) % local_host_range;
	if (!port_used[cur_pos])
 	{
	    port_used[cur_pos] = 1;
	    /* the next time start search from the port following this one */
	    next_to_use = (cur_pos + 1) % local_host_range;
	    break;
	}
    }

    return idx == local_host_range ? 0 : local_host_port + cur_pos;
}

/**************************************************************************************
* Function name: reserved_port_free                                                   *
* Input arg    : short port                                                           *
* Output arg   : NONE                                                                 *
* Description  : Frees reserved port in the system                                    *
* Developer-Note: Unmodified Source block. Received as a part of AUDIOCODES library used as it is without change.                                                                                    *
***************************************************************************************/

void reserved_port_free(u16 port)
{
    u16 idx = port - local_host_port;

    if (vcCONFIG_RG_FIREWALL && idx < local_host_range)
	port_used[idx] = 0;
}

/**************************************************************************************
* Function name: rtp_ports_free                                                       *
* Input arg    : void                                                                 *
* Output arg   : NONE                                                                 *
* Description  : Frees RTP ports in the system                                        *
* Developer-Note: Unmodified Source block. Received as a part of AUDIOCODES library used as it is without change.                                                                                    *
***************************************************************************************/

static void rtp_ports_free(void)
{
    rtp_session_t *next;

    while (rtp_sessions)
    {
	next = rtp_sessions->next;
	free(rtp_sessions);
	rtp_sessions = next;
    }
}

/*************************************************************************************
* Function name: voip_stack_stop                                                     *
* Input arg    : void                                                                *
* Output arg   : NONE                                                                *
* Description  :stop to voip stack. Called before restart of the stack or if         *
                application is going do
*Developer-Note: Unmodified Source block. Received as a part of AUDIOCODES library used as it is without change.
***************************************************************************************/

static void voip_stack_stop(void)
{
   DPRINTK("voip_stack_stop: Enter.\n");
    call_mgt->uninit();
   g_siLocal_host_ports_free();
    rtp_ports_free();
    acl_line_connection_uninit();
    destroyVoipStatusHandler();
    free(call_mngr_params.lines);
   DPRINTK("voip_stack_stop: Exit.\n");
}

/**************************************************************************************
* Function name: catch_term                                                           *
* Input arg    : int                                                                  *
* Output arg   : NONE                                                                   *
* Description  : catches the SIGTERM signal
* Developer-Note: Unmodified Source block. Received as a part of AUDIOCODES library used as it is without change.                                        *
***************************************************************************************/

static void catch_term(int dummy)
{
   	DPRINTK("catch_term\n");
    printf("SIGNAL HANDLER FOR SIGTERM\n");
    g_cInit_app=CLEAR;


}

/**************************************************************************************
* Function name: catch_term1                                                          *
* Input arg    : int signal no                                                        *
* Output arg   : NONE                                                                   *
* Description  : catches the SIGINT signal
* Developer-Note: Unmodified Source block. Received as a part of AUDIOCODES library used as it is without change.                                           *
***************************************************************************************/
static void catch_term1(int dummy)
{
    printf("SIGNAL HANDLER FOR SIGINT\n");
	g_cInit_app=CLEAR;
}

/**************************************************************************************
* Function name: catch_term2                                                          *
* Input arg    : int signal no                                                        *
* Output arg   : NONE                                                                   *
* Description  : catches the SIGSEGV signal
* Developer-Note: Unmodified Source block. Received as a part of AUDIOCODES library used as it is without change.                                         *
***************************************************************************************/
static void catch_term2(int dummy)
{
    printf("SIGNAL HANDLER FOR SIGSEGV\n");

   g_cInit_app=CLEAR;
}





/**************************************************************************************
* Function name: voip_stack_start                                                      *
* Input arg    : void                                                                  *
* Output arg   : NONE                                                                    *
* Description  : Initialize the SIP stack
* Developer-Note: Unmodified Source block. Received as a part of AUDIOCODES library used as it is without change.                                         *
***************************************************************************************/


/* Called in case receiving internet connection or in case changing primary
 * parameters. These parameters are configuration parameters of the voip stack
 */
void voip_stack_start(void)
{
    int i;
    /* XXX static values, because params is not copied by the call manager */
    static codec_t codecs[NUMBER_OF_CODECS];
    /* ACL NB  061102 */

	memset(&codecs, 0, sizeof(codecs));
	for (i=0; i<NUMBER_OF_CODECS; i++)
	    	codecs[i].payload = -1;

	memset(&call_mngr_params, 0, sizeof(call_mngr_params));
	printf("voip_stack_start: Enter.\n");

	call_mngr_params.ep_num  = voip_conf_get_number_of_lines(ANALOG_PORT_TYPE_FXO_FXS);

    /* need to stop, only if we've ever been started */
    if (call_mgt)
	{
		voip_stack_stop();
		DPRINTK("voip_stack_start(after call to voip_stack_stop): line=%d.\n", __LINE__);
	}

	DPRINTK("voip_stack_start: line=%d  call_mngr_params.lines = 0x%08x\n", __LINE__, (unsigned int)call_mngr_params.lines);

	/* Initialize proprietary message queue mechanism */
//	init_acMsgQDB();

	if(AC_TIMER_INIT() <0 )
		printf("AC_TIMER_INIT failed");

    	DPRINTK("voip_stack_start: line=%d, NumOfLines=%d.\n", __LINE__, call_mngr_params.ep_num);
	call_mngr_params.lines = AC_ZALLOC_L(sizeof(line_conf_t)*call_mngr_params.ep_num);

	DPRINTK("voip_stack_start: line=%d  call_mngr_params.lines = 0x%08x\n", __LINE__, (unsigned int)call_mngr_params.lines);
    /* Get WAN IP address from conf */
    voip_conf_local_address_get(call_mngr_params.local_ip);
    DPRINTK("voip_stack_start: line=%d  call_mngr_params.local_ip = %s\n", __LINE__, call_mngr_params.local_ip);
    /* Get signalling parameters */
    voip_conf_signalling_conf_get(&call_mngr_params);
    DPRINTK("voip_stack_start: line=%d.\n", __LINE__);

	/*NirM  080303, no need to allocate rtp ports */
//    if (rtp_ports_alloc(call_mngr_params.media_port) < 0)
//	return;

    rtp_base_local_port_set(call_mngr_params.media_port);

    rtp_tos = call_mngr_params.media_tos;
    mss_clamping_limit = call_mngr_params.mss_clamping_limit;

    if (vcCONFIG_RG_FIREWALL &&
	g_siLocal_host_ports_alloc(call_mngr_params.local_host_port,
	call_mngr_params.local_host_port_range) < 0)
    {
	return;
    }
    DPRINTK("voip_stack_start: line=%d.\n", __LINE__);
    if (call_mngr_params.protocol == VOIP_PROT_MGCP)
	call_mgt = &mgcp_call_mngr_ops;
    else
	call_mgt = &term_call_mngr_ops;

    if (!call_mgt->init) /* stub */
    {
	printf("\nUnsupported protocol - %d\n", call_mngr_params.protocol);
    }

    /* ACL NB 070528 get dialing parameters*/
    voip_conf_dialing_param_get(&call_mngr_params);


    DPRINTK("voip_stack_start: line=%d.\n", __LINE__);
    /* Get line's configurable parameters */
    for (i = 0; i < call_mngr_params.ep_num; i++)
		voip_conf_line_conf_get(i, &call_mngr_params.lines[i]);

    DPRINTK("voip_stack_start: line=%d.\n", __LINE__);

    for (i=0; i<NUMBER_OF_CODECS; i++)
	    voip_conf_codec_info_get(i, &codecs[i]);

	DPRINTK("voip_stack_start: line=%d.\n", __LINE__);

    voip_conf_dynamic_payloads_info_get();//NirM,  configure payload type - update payload type parameters

    DPRINTK("voip_stack_start: line=%d.\n", __LINE__);

	/* ACL NB 060723 support DTMF via SIP  */
    /* acl_line_connection_init(codecs, call_mngr_params.send_dtmf_out_of_band); */
    if (call_mngr_params.dtmf_transport_method == DTMF_RFC2833 )
        acl_line_connection_init(codecs, 1);
    else
        acl_line_connection_init(codecs, 0);

	DPRINTK("voip_stack_start: line=%d   call_mngr_params.local_ip = %s\n", __LINE__, call_mngr_params.local_ip);
	/* AlexChen 20090331 SDK upgrade */
	expandDigitMap( );

    /* Initialize termination manager */
    call_mgt->init(&call_mngr_params, &call_mngr_cbs);

    DPRINTK("voip_stack_start: line=%d.\n", __LINE__);

	DPRINTK("voip_stack_start: The End line=%d.\n", __LINE__);

}





/***********************************************************************************
 * Function name: voip_stack_uninit                                                *
 * Input arg    : void                                                             *
 * Output arg   : NONE                                                             *
 * Description  : uninitialization and unload voip toolkit
 * Developer-Note: Unmodified Source block. Received as a part of AUDIOCODES library used as it is without change.                      *
 **********************************************************************************/

/* uninitialization and unload voip toolkit */
static void voip_stack_uninit(void)
{
	if(AC_TIMER_DESTROY() < CLEAR )
		printf("AC_TIMER_DESTROY failed\n");
}

/***********************************************************************************
 * Function name: voip_stack_init                                                  *
 * Input arg    : void                                                             *
 * Output arg   : NONE                                                             *
 * Description  : Initialization of the voip toolkit. Execute once on the start of *
 * application and don't needed reinitialization on case change any parameters     *
 * Developer-Note: Unmodified Source block. Received as a part of AUDIOCODES library used as it is without change.
 ***********************************************************************************/

/* Initialization of the voip toolkit. Execute once on the start of the
 * application and don't needed reinitialization on case change any parameters
 */
static int voip_stack_init(void)
{
    return CLEAR;
}

void getTime()
{
    int hr,min,sec;

    time_t T = time(NULL);
    struct tm tm = *localtime(&T);
    printf("Time is : %02d:%02d:%02d\n", tm.tm_hour,tm.tm_min,tm.tm_sec);
}



/************************************************************************************
 * Function name: process_events_listen                                             *
 * Input arg    : void                                                              *
 * Output arg   : NONE                                                              *
 * Description  : This function uses the select function which will monitor all the *
                  all file descriptors created by the application. Whenever there is*
                  an activity on particular Fd it raises related fsu event.         *
*************************************************************************************/
void process_events_listen()
{
    struct timeval timeout;
    int res,result,fd,ret;
    uint64_t expirations,diag_expirations;
    static i_pin_low_cntr,i_pin_high_cntr;
    int i_n,i_ret;

    app_testset = app_readset;
    timeout.tv_sec = 2;
    timeout.tv_usec = 500000;
    result=select(FD_SETSIZE,&app_testset,NULL,NULL,&timeout);
   // result=select(FD_SETSIZE,&app_testset,NULL,NULL,NULL);




    if(result<SET)
    {
     // perror(" app server1");
        return CLEAR;
    }

    for(fd=CLEAR ; fd < MAX_APP_FDS;fd++)
    {
        if((FD_ISSET(fd,&app_testset)))
        {
            if(fd == g_uiRadv_sock_fd)
            {
                g_uiClilen = sizeof(g_cli_addr);
                g_uiClient_connection_fd = accept(g_uiRadv_sock_fd,CLEAR,CLEAR);

                if(g_uiClient_connection_fd<CLEAR)
                {
                    perror("pjsua accept failed\n");
                }

                FD_SET(g_uiClient_connection_fd,&app_readset);
                printf("adding server client on fd %d\n", g_uiClient_connection_fd);
                if(g_uiClient_connection_fd < CLEAR)
                {
                    perror("----scVOIPAPP ERROR on accept\n");
                }
            }
            else
            {
                if (fd == g_uiApp_timer_fd)
                {
                    int i_res;
                    i_res=read(g_uiApp_timer_fd,&expirations,sizeof(expirations));
                     //  printf("in voipapp Timer fd  \n");
                    if(i_res < CLEAR)
                    {
                        perror("Server-read() error");
                        close(fd);
                        FD_CLR(fd, &app_readset);
                    }
                    else
                    {

                        /***************Check ICOM contributor table is full*************/
                        if(g_iIcom_mem_dropped==SET &&stn_config.icom_live_dead_status[stn_config.default_intercom-MIN_OFFSET]==LIVE)
                        {
                            if(pvt_call.type==INTERCOM_RX)
                            {
                                if(icom_contributor_status_info[pvt_call.curr_icom_no].cont1!=CLEAR&&
                                icom_contributor_status_info[pvt_call.curr_icom_no].cont2!=CLEAR)
                                {

                                  //  operate_gpio(ICOM_STATUS,WRITE,GPIO_ON);
                                    // operate_gpio(PFC_ACT,WRITE,GPIO_ON);
                                }
                                else
                                {
                                    //operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);
                                //  operate_gpio(PFC_ACT,WRITE,GPIO_OFF);
                                }
                            }
                        }

                        /******PLAY TONE TIMER****************/
                        if(g_uiPlay_tone_timer)
                        {
                            g_uiPlay_tone_timer--;
                            if(g_uiPlay_tone_timer==CLEAR)
                            {
                                acgPlayTone_phi(CLEAR,VOIP_SIGNAL_RING_TONE,STOP_TONE,CLEAR);
                                set_volume(PG_CHANNEL,MAX_MUSIC_VOLUME);
                            }
                        }

                        /******SERVER LIVE STATUS TIMER****************/
                        if(g_uiServer_live_status_timer)
                        {
                            g_uiServer_live_status_timer--;
                          //  printf("live_status_timer=%d....\n",g_uiServer_live_status_timer);
                            if(g_uiServer_live_status_timer==CLEAR)
                            {
                                 char lan_led_clear[]={0x00};
                                printf("g_uiServer_live_status_timer is 0....\n");
                                getTime();
                               // g_uiFsu_event=WAKE_UP_EVENT;
                                g_uiFsu_event = BROADCAST_TIMEOUT_EVENT;
                                 send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,LED_LAN_SERV,CLEAR,&lan_led_clear[ZERO],IO_ADDRESS,
                                 LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);

                            }
                        }
                            /******KEEP ALIVE TIMER****************/
                        if(g_uiKeep_alive_timer)
                        {
                            g_uiKeep_alive_timer--;
                             printf("KEEP alive  TImer =%d\n",g_uiKeep_alive_timer);
                            if(g_uiKeep_alive_timer==CLEAR)
                            {
                                printf("keep alive frame is send to server....\n");
                                getTime();
                                send_req_msg_to_server(g_uiFsu_state,KEEP_ALIVE_FRAME,KEEP_ALIVE_FRAME_LENGTH,'0','0');
                                g_uiKeep_alive_timer=KEEP_ALIVE_TIMEOUT;
                            }

                        }


                        /******SPEECH DETECTION TIMER FOR CHANNEL/GROUP CALL****************/

                      /*  if(g_uiSpeech_detection_timer)
                        {
                            g_uiSpeech_detection_timer--;
                             printf("speech detection  alive  TImer =%d\n",g_uiSpeech_detection_timer);
                            if(g_uiSpeech_detection_timer==CLEAR)
                            {

                                getTime();
                                if(pvt_call.type==INTERCOM_TX_RX)
                                {
                                    send_req_msg_to_server(0,ICOM_PTT_RELEASE_REQ,ICOM_CALL_FRAME_LENGTH,'1','0');

                                    printf("PTT release frame is send to server....\n");
                                }
                                else if(pvt_call.type == GROUP)
                                {
                                    send_req_msg_to_server(0,GROUP_PTT_RELEASE,GROUP_CALL_FRAME_LENGTH,'1','0');

                                }

                            }

                        }*/


                        /******ICOM LED BLINK TIMER****************/
                        pthread_mutex_lock(&lock_call_type);
                        if(pvt_call.type==INTERCOM_TX_RX )
                        {

                            if(g_uiIcom_led_on_flag==SET)
                            {

                                g_uiIcom_led_on_flag=CLEAR;
                               // operate_gpio(ICOM_STATUS,WRITE,GPIO_ON);
                              //  operate_gpio(PFC_ACT,WRITE,GPIO_ON);

                            }
                            else
                            {
                               // operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);
                              //  operate_gpio(PFC_ACT,WRITE,GPIO_OFF);
                                g_uiIcom_led_on_flag=SET;
                            }

                        }

                        pthread_mutex_unlock(&lock_call_type);

                         /******ICOM LED BLINK TIMER FOR GROUP CALL****************/
                        pthread_mutex_lock(&lock_call_type);
                        //if(pWork_m.private_state == PRIVATE_OUTGOING_ONGOING)
                       // {
                        if((pvt_call.type==GROUP_TX_RX))
                         //if((pvt_call.type==GROUP))
                          {
                             if(g_uiIcom_led_on_flag==SET)
                             {

                                 g_uiIcom_led_on_flag=CLEAR;
                                // operate_gpio(ICOM_STATUS,WRITE,GPIO_ON);
                                // operate_gpio(PFC_ACT,WRITE,GPIO_ON);

                             }
                             else
                             {
                                 //operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);
                                 //operate_gpio(PFC_ACT,WRITE,GPIO_OFF);
                                 g_uiIcom_led_on_flag=SET;
                             }

                           }
                         //}

                        pthread_mutex_unlock(&lock_call_type);

                        /******VOIP APP WAIT TIMER****************/
                        if(g_uiVoipapp_wait_timer)
                        {
                            g_uiVoipapp_wait_timer--;
                               //  printf("voip app timer=%d \n",g_uiVoipapp_wait_timer);
                            if(g_uiVoipapp_wait_timer == CLEAR)
                            {
                                g_uiFsu_event = APP_TIMEOUT_EVENT;
                            }

                        }

                        /******PVT CALL TIMER****************/
                        if(pvt_call.pvt_call_busy)
                        {

                            if(g_uiPvt_call_timer)
                            {
                               // printf("********* g_uiPvt_call_timer %d\n",g_uiPvt_call_timer);

                              /*  printf("********g_uiPvt_call_timer  %d\n",g_uiPvt_call_timer);

                                  PvtCallTimer[PvtCallTimerSec] = (g_uiPvt_call_timer/2);
                                  if(PvtCallTimerSec%2 == 0)
                                   {
                                      minite = (PvtCallTimer[PvtCallTimerSec]/60);
                                      second = (PvtCallTimer[PvtCallTimerSec] % 60);


                                      sprintf(&g_arcPvtCallTimer[ZERO],"%d",0);
                                      sprintf(&g_arcPvtCallTimer[ONE],"%d",minite);
                                      sprintf(&g_arcPvtCallTimer[TWO],"%s",":");
                                      sprintf(&g_arcPvtCallTimer[THREE],"%d",second);

                                     // strcat(g_arcPvtCallTimer,":");

                                      printf("g_arcPvtCallTimer %s\n",g_arcPvtCallTimer);
                                      //send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_arcPvtCallTimer,LCD_CLR_11_POSITION,11);
                                     //update_lcd_msg(g_arcPvtCallTimer,LINE2,NO_LCD_CLR,11);
                                       if(g_uiFsu_state == P2P_STATE)
                                       {
                                          //send_msg_ui(P2P_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_arcPvtCallTimer,LCD_CLR_11_POSITION,11);

                                       }
                                       if( g_uiFsu_state == CONFERENCE_STATE)
                                       {

                                          // send_msg_ui(CONFERENCE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_arcPvtCallTimer,LCD_CLR_11_POSITION,11);

                                       }

                                       if(g_uiFsu_state == GROUP_STATE)
                                       {
                                            //send_msg_ui(GROUP_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_arcPvtCallTimer,LCD_CLR_11_POSITION,11);
                                       }

                                       if(g_uiFsu_state == GROUP_CONTRIBUTOR_STATE)
                                       {
                                            //send_msg_ui(GROUP_CONTRIBUTOR_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_arcPvtCallTimer,LCD_CLR_11_POSITION,11);
                                       }


                                  }

                                PvtCallTimerSec++;

                                */
                                g_uiPvt_call_timer--;
                                if(g_uiPvt_call_timer == CLEAR)
                                {
                                    g_uiFsu_event=PVT_CALL_TIMEOUT;
                                    printf("********* PVT_CALL_TIMEOUT is done\n");
                                }
                            }

                            //for Global call and zone call Initiator

                           //  if(g_uiPage_call_timer)
                           /* else if(g_uiPage_call_timer_for_initiator)
                             {
                                  PvtCallTimer_fr_pg_init[PvtCallTimerSec_fr_pg_init] = (g_uiPage_call_timer_for_initiator/2);
                                  if(PvtCallTimerSec_fr_pg_init%2 == 0)
                                   {
                                      minite_fr_pg_init = (PvtCallTimer_fr_pg_init[PvtCallTimerSec_fr_pg_init]/60);
                                      second_fr_pg_init = (PvtCallTimer_fr_pg_init[PvtCallTimerSec_fr_pg_init] % 60);

                                      // printf("lcd_timer.min :lcd_timer.sec%d:%d\n",lcd_timer.min,lcd_timer.sec);
                                      sprintf(&g_arcPvtCallTimer_fr_pg_init[ZERO],"%d",0);
                                      sprintf(&g_arcPvtCallTimer_fr_pg_init[ONE],"%d",minite_fr_pg_init);
                                      sprintf(&g_arcPvtCallTimer_fr_pg_init[TWO],"%s",":");
                                      sprintf(&g_arcPvtCallTimer_fr_pg_init[THREE],"%d",second_fr_pg_init);
                                       //strcat(g_arcPvtCallTimer,":");

                                        printf("inside PAGE CALL TIMER receiver g_arcPvtCallTimer %s\n",g_arcPvtCallTimer_fr_pg_init);
                                      //printf("g_arcPvtCallTimer %s\n",g_arcPvtCallTimer);
                                      //send_msg_ui(PAGE_INITIATOR_STATE,LCD,LINE1,CONTINUE_DISPLAY,g_arcPvtCallTimer_fr_pg_init,LCD_CLR_11_POSITION,11);

                                    }

                                PvtCallTimerSec_fr_pg_init++;


                                g_uiPage_call_timer_for_initiator--;
                                if(g_uiPage_call_timer_for_initiator == CLEAR)
                                {
                                    g_uiFsu_event=PVT_CALL_TIMEOUT;
                                }

                             }*/


                       }

                        /******PAGE CALL TIMER****************/
                        if(pg_call.pg_call_busy)
                        {

                            if(g_uiPage_call_timer)
                            {

                                /*printf("********g_uiPage_call_timer  %d\n",g_uiPage_call_timer);

                                 PgCallTimer[PgCallTimerSec] = (g_uiPage_call_timer/2);
                                  if(PgCallTimerSec%2 == 0)
                                   {
                                        pg_min = (PgCallTimer[PgCallTimerSec]/60);
                                        pg_sec = (PgCallTimer[PgCallTimerSec] % 60);

                                         // printf("lcd_timer.min :lcd_timer.sec%d:%d\n",lcd_timer.min,lcd_timer.sec);
                                        sprintf(&g_arcPgCallTimer[ZERO],"%d",0);
                                        sprintf(&g_arcPgCallTimer[ONE],"%d",pg_min);
                                        sprintf(&g_arcPgCallTimer[TWO],"%s",":");
                                        sprintf(&g_arcPgCallTimer[THREE],"%d",pg_sec);
                                         //strcat(g_arcPvtCallTimer,":");


                                        printf("inside PAGE CALL TIMER receiver g_arcPgCallTimer %s\n",g_arcPgCallTimer);
                                        send_msg_ui(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,g_arcPgCallTimer,LCD_CLR_11_POSITION,11);


                                  }

                                PgCallTimerSec++;
                               */

                                g_uiPage_call_timer--;
                                    //sprintf(g_cLine2_buf,"PG:%02d:%02d  ",g_uiPage_call_timer/60, g_uiPage_call_timer%60);
                                if(g_uiPage_call_timer == CLEAR)
                                {
                                    g_uiFsu_event=PG_CALL_TIMEOUT;
                                }

                            }


                        }
                    }

                } //if timer_fd*/


                if(fd==g_uiApp_diag_timer_fd)
                {
                    int i_result;
                    i_result=read(g_uiApp_diag_timer_fd,&diag_expirations,sizeof(diag_expirations));
                    if(i_result < CLEAR)
                    {
                        perror("Server-read() error");
                        close(fd);
                        FD_CLR(fd, &app_readset);
                    }
                    else
                    {
                       /* if(g_uiDiagnostic_start_flag)
                        {
                            if(g_uiPvt_diagnostic_timer)
                            {
                                g_uiPvt_diagnostic_timer--;
                                i_n=operate_gpio(PVT_RX_CHK,READ,CLEAR);

                                if(i_n==CLEAR)
                                {
                                    if(i_pin_low_cntr<DIAGNOSTIC_PIN_CHECK_CNTR)
                                    i_pin_low_cntr++;
                                    printf("pvt diagnostic  low counter=%d\n",i_pin_low_cntr);
                                }
                                if(i_n==1)
                                {
                                    if(i_pin_high_cntr<DIAGNOSTIC_PIN_CHECK_CNTR)
                                    i_pin_high_cntr++;
                                    printf("pvt diagnostic  high counter=%d\n",i_pin_high_cntr);
                                }

                                if(i_pin_high_cntr==DIAGNOSTIC_PIN_CHECK_CNTR &&i_pin_low_cntr==
                                DIAGNOSTIC_PIN_CHECK_CNTR)
                                {
                                    i_pin_low_cntr=CLEAR;
                                    i_pin_high_cntr=CLEAR;
                                    g_uiPvt_diagnostic_timer=CLEAR;
                                    g_uiFsu_event=DIAGNOSTIC_PVT_OK;
                                }
                                else if(g_uiPvt_diagnostic_timer==CLEAR &&(i_pin_high_cntr!=CLEAR || i_pin_low_cntr!=
                                CLEAR))
                                {
                                    i_pin_low_cntr=CLEAR;
                                    i_pin_high_cntr=CLEAR;
                                    g_uiFsu_event=DIAGNOSTIC_PVT_NOT_OK;
                                }

                            }*/


                         /*   if(g_uiSpk_diagnostic_timer)
                            {
                                g_uiSpk_diagnostic_timer--;
                                i_n=operate_gpio(PAGE_RX_CHK,READ,CLEAR);

                                if(i_n==CLEAR)
                                {
                                    if(i_pin_low_cntr<DIAGNOSTIC_PIN_CHECK_CNTR)
                                    i_pin_low_cntr++;
                                    printf("spk diagnostic low counter=%d\n",i_pin_low_cntr);
                                }
                               else
                                {
                                    if(i_pin_high_cntr<DIAGNOSTIC_PIN_CHECK_CNTR)
                                    i_pin_high_cntr++;
                                    printf("spk diagnostic high counter=%d\n",i_pin_high_cntr);
                                }

                                if(i_pin_high_cntr==DIAGNOSTIC_PIN_CHECK_CNTR &&
                                 i_pin_low_cntr==DIAGNOSTIC_PIN_CHECK_CNTR)
                                {
                                    i_pin_low_cntr=CLEAR;
                                    i_pin_high_cntr=CLEAR;
                                    g_uiSpk_diagnostic_timer=CLEAR;
                                    g_uiFsu_event=DIAGNOSTIC_SPK_OK;
                                }

                                if(g_uiSpk_diagnostic_timer==CLEAR && (i_pin_high_cntr!=CLEAR ||                                                                      i_pin_low_cntr!=CLEAR))
                                {
                                    i_pin_low_cntr=CLEAR;
                                    i_pin_high_cntr=CLEAR;
                                    g_uiFsu_event=DIAGNOSTIC_SPK_NOT_OK;
                                }

                            }*/

                        }
                    }

                }

                if(fd == g_uiUi_socket_fd)
                {
                    char c_msgbuf[UI_MSG_LENGTH];
                    int i_nbytes;
                    printf("in UI SOCKET fd  \n");
                    g_uiApp_ui_addrlen=sizeof(g_app_ui_addr);
                    i_nbytes=recv(g_uiUi_socket_fd,c_msgbuf,MAX_ETH_FRAME_LENGTH,CLEAR);
                    if(i_nbytes>0)
                    {
                        memcpy(g_cReceived_key_seq,c_msgbuf,sizeof(c_msgbuf)) ;
                    }
                    else
                    {
                        FD_CLR(fd, &app_readset);
                        printf("PTT failed \n");
                    }

                        g_uiFsu_event=VALID_KEY_SEQ_EVENT;
                        printf("msgbuf %s\n",c_msgbuf);

                }//g_uiUi_socket_fd*/


                if(fd == g_uiBroadcast_fd)
                {
                    int i_n;
                    char c_buf[MAX_ETH_FRAME_LENGTH];
                    char c_Buffer[MAX_ETH_FRAME_LENGTH];
                    int  i_len=CLEAR;
                    unsigned int ui_Index = CLEAR;
                    unsigned int ui_cmd = CLEAR;
                    i_n = recvfrom(g_uiBroadcast_fd,c_buf,MAX_ETH_FRAME_LENGTH,CLEAR,
                    	(struct sockaddr *)&reg_addr,&g_uiReg_addrlen);
                    	printf("Broadcast frame is received....\n");
                        if (i_n < CLEAR)
                        {
                            error("recvfrom");

                        }
                        i_len =  validate_frame(c_buf);
                        memcpy(c_Buffer,c_buf + (PRTCL_HDR_LEN + 2),i_len);
                                //.
                        if(i_len>1)
                        {

                            ui_cmd = *(c_Buffer) & 0xFF | *(c_Buffer + 1) << 8 & 0xFF00;

                            if(ui_cmd == SERVER_REG_BCAST)
                            {
                                if(first_broadcast_flag==0)
                                {
                                    printf("*********led is on\n**********");
                                    send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,LED_LAN_SERV,SET,&io_led[TWO],IO_ADDRESS,
                                    LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                                    first_broadcast_flag=ONE;
                                }
                                g_uiServer_live_status_timer=SERVER_LIVE_TIMEOUT;
                                printf("SERVER_REG_BCAST ********* cmd_id = %d,%d\n", ui_cmd,g_uiServer_live_status_timer);
                                getTime();
                            }
                            parse_commands(ui_cmd,c_Buffer);
                        }
                }

                if(fd==g_uiMulticast_signalling_fd )
                {
                    int  i_n;
                    char c_buf[MAX_ETH_FRAME_LENGTH];
                    char c_Buffer[MAX_ETH_FRAME_LENGTH];
                    int  i_len=CLEAR;
                    unsigned int ui_Index =CLEAR;
                    unsigned int ui_cmd = CLEAR;
                    i_n = recvfrom(g_uiMulticast_signalling_fd ,c_buf,MAX_ETH_FRAME_LENGTH,0,
                        (struct sockaddr *)&mul_sig_addr,&g_uiMul_sig_addrlen);
                         printf(">>>>>>>>>>>>>>>>>>>c_buf is %s \n",c_buf);
                    i_len =  validate_frame(c_buf);

                    if(i_len>1)
                    {
                        memcpy(c_Buffer,c_buf + (PRTCL_HDR_LEN + 2),i_len);
                        if(i_n == -1)
                        {
                            printf("broadcast ERROR, Protocol Header Incorrect\n");
                        }
                        else
                        {
                            printf("Multicast Protocol Header OK\n");
                            ui_cmd = *(c_Buffer) & 0xFF | *(c_Buffer + 1) << 8 & 0xFF00;
                            printf("Multicast cmd is=%i frame is %s\n",ui_cmd,c_buf);
                            parse_commands(ui_cmd,c_Buffer);
                        }
                    }
                }


                if(fd == g_uiServ_udp_comm_fd)
                {
                    int i_n;
                    char c_buf[MAX_ETH_FRAME_LENGTH];
                    char c_Buf[10];
                    int i_len=CLEAR;
                    unsigned int Index = CLEAR;
                    unsigned int cmd = CLEAR;
                    printf("in udp server communication fd \n");
                    i_n = recvfrom(g_uiServ_udp_comm_fd,c_buf,MAX_ETH_FRAME_LENGTH,CLEAR,
                    (struct sockaddr *)&udp_comm_addr,&g_uiUdp_comm_addrlen);
                    if (i_n < CLEAR)
                    {
                        error("recvfrom");

                    }
                    printf("buf is %s \n",c_buf);
                    sprintf(c_Buf,"%c%c",c_buf[9],c_buf[10]);
                    i_len =  validate_frame(c_buf);
                    cmd =atoi(c_Buf);
                    printf("udp cmd is.............%i %d \n",cmd,i_len);
                    conf_cmd = cmd;
                    parse_commands(cmd,c_buf);

                }

                if(fd == g_uiClient_connection_fd)
                {
                    int i_n;
                    char c_Buffer[50];

                    i_n = recv(g_uiClient_connection_fd,c_Buffer,MAX_ETH_FRAME_LENGTH,0);
                    printf("recived packet form Radvision %d %s \n",i_n,c_Buffer);

                    if(strstr(c_Buffer,"SIP_REGISTERED"))
                    {
                        printf("fsu event sip registed \n");
                       // g_cSip_ok_flag = SET;
                        g_uiFsu_event=VALID_SIP_REGISTER;
                    }

                    if(strstr(c_Buffer,"RINGING_STATE"))
                    {
                        g_uiFsu_event=RINGING_OFF_HOOK_EVENT;
                        printf("fsu event OFF_HOOK_EVENT \n");
                    }

                    if(strstr(c_Buffer,"CALL_ANSWERED"))
                    {
                        g_cCaller[0] = c_Buffer[13];
                        g_cCaller[1] = c_Buffer[14];
                        g_cCaller[2] = c_Buffer[15];
                        g_uiFsu_event=P2P_CALL_EVENT;
                        printf("fsu event P2P CALL \n");
                    }

                    if(strstr(c_Buffer,"CALL_OFF_HOOK_PASS"))
                    {
                        g_uiFsu_event=CALL_OFF_HOOK_EVENT;
                        printf("fsu event call hook CALL \n");
                    }

                    if(strstr(c_Buffer,"TERMINATED_FOR_CONF_CALL"))
                    {
                          g_uiFsu_event = CONF_ADD_EVENT;
                          printf("fsu event CONF ADD EVENT \n");
                    }
                    else if(strstr(c_Buffer,"DISCONNECTED"))
                    {
                        g_uiFsu_event=P2P_RESET_EVENT;
                        printf("fsu event Reset CALL \n");
                    }

                }

            } //else of fdset
        }
    } //for loop




/************************************************************************************
 * Function name: send_msg_ui                                                       *
 * Input arg    : char state char para,short line_no,short cmd,char* text short flash_cntr *
 * Output arg   : NONE                                                               *
 * Description  : This function used to keep updating UI thread about the FSU states,*
 *                 and also used to UI thread message to be displayed on the LCD      *
*************************************************************************************/

void send_msg_ui_lcd(char state,char parameter,short int line_no,short int display,char *text,int position,char addr,short int cmd)
{
    int i_res;
    printf("text is 0x%x \n",*text);
    memset(&ui_msg,0,sizeof(ui_msg));

    strcpy(ui_msg.text,text);////msg


	printf("msg is %s \n",ui_msg.text);
    ui_msg.addr = addr;
    printf("ui_msg.addr is %x \n",ui_msg.addr);
    ui_msg.parameter=parameter;////lcd or io

    ui_msg.line_no = line_no;

    ui_msg.cmd = cmd;

    ui_msg.state = state;

    ui_msg.display=display;

    ui_msg.position = position;

   	//i_res=write(g_uiUi_socket_fd, c_msg, sizeof(char));

   	i_res = send(g_uiUi_socket_fd,&ui_msg,sizeof(struct UI_msg),0);
   	if(i_res == -1)
   	{
   	    printf(" send() failed in send_msg_ui\n");
   	    return ;
   	}
   	 printf("send() in send_msg_ui success...\n");
}

void send_msg_ui_io(char state,char parameter, short int display,short int pin_no,short int pin_status,char *text,char addr,short int led_on_time,short int led_off_time,short int cmd,short int flash_counter)
{
    int i_res;
    printf("text is 0x%x \n",*text);
    memset(&ui_msg,0,sizeof(ui_msg));

    strcpy(ui_msg.text,text);////msg


	printf("msg is %s \n",ui_msg.text);
    ui_msg.addr = addr;
    printf("ui_msg.addr is %x \n",ui_msg.addr);
    ui_msg.parameter=parameter;////lcd or io
    ui_msg.cmd = cmd;

    ui_msg.pin_no=pin_no;
    printf("ui_msg.pin_no=%d\n",ui_msg.pin_no);
    ui_msg.pin_status=pin_status;
    printf("ui_msg.pin_status=%d\n",ui_msg.pin_status);
    ui_msg.state = state;

    ui_msg.flash_cntr=flash_counter;
  	ui_msg.display=display; /////contineous or flashing
   	ui_msg.led_on_time=led_on_time;
    ui_msg.led_off_time=led_off_time;

     	//i_res=write(g_uiUi_socket_fd, c_msg, sizeof(char));

   	i_res = send(g_uiUi_socket_fd,&ui_msg,sizeof(struct UI_msg),0);
   	if(i_res == -1)
   	{
   	    printf(" send() failed in send_msg_ui\n");
   	    return ;
   	}
   	 printf("send() in send_msg_ui success...\n");
}

/*************************************************************************************
 * Function name: create_app_timer                                                   *
 * Input arg    : void                                                               *
 * Output arg   : NONE                                                               *
 * Description  : This function used to generate One second timer                    *
 *                                                                                    *
*************************************************************************************/

void create_app_timer()
{
    int i_res;
    struct itimerspec app_timspec;

    g_uiApp_timer_fd=timerfd_create(CLOCK_MONOTONIC,CLEAR);

    if(g_uiApp_timer_fd<CLEAR)
    {
        perror("app_timerfd create:");
    }
    FD_SET(g_uiApp_timer_fd,&app_readset);
    //  printf("app timerfd created \n");
    bzero(&app_timspec,sizeof(app_timspec));

    app_timspec.it_interval.tv_sec=CLEAR;//msg->attr;
    app_timspec.it_interval.tv_nsec=500000000;   //500ms timer count

    app_timspec.it_value.tv_sec=CLEAR;//msg->attr;
    app_timspec.it_value.tv_nsec=500000000;

    i_res=timerfd_settime(g_uiApp_timer_fd,CLEAR,&app_timspec,CLEAR);

    if(i_res<CLEAR)
    {
        perror("timerfd settime:");
    }
}

/*************************************************************************************
 * Function name: create_diagnostic_timer                                            *
 * Input arg    : void                                                               *
 * Output arg   : NONE                                                               *
 * Description  : This function used to generate 20ms timer                          *
                                                                                     *
*************************************************************************************/

void create_diagnostic_timer()
{
    int i_res;
    struct itimerspec app_timspec;

    g_uiApp_diag_timer_fd=timerfd_create(CLOCK_MONOTONIC,CLEAR);
    if(g_uiApp_timer_fd<CLEAR)
    {
        perror("app_timerfd create:");
    }

    //  printf("app timerfd created \n");
    bzero(&app_timspec,sizeof(app_timspec));
    app_timspec.it_interval.tv_sec=CLEAR;//msg->attr;
    app_timspec.it_interval.tv_nsec=20000000;   //20 ms timer count

    app_timspec.it_value.tv_sec=CLEAR;//msg->attr;
    app_timspec.it_value.tv_nsec=20000000;

    i_res=timerfd_settime(g_uiApp_diag_timer_fd,0,&app_timspec,0);

    if(i_res<CLEAR)
    {
        perror("timerfd settime:");
    }



}


/*************************************************************************************
 * Function name: UI_socket_create                                                   *
 * Input arg    : void                                                               *
 * Output arg   : NONE                                                               *
 * Description  : This function used to create unix socket between main and UI thread*
                                                                                     *
*************************************************************************************/
void UI_socket_create()
{
    char uc_socket_name[TEN]={"UI"};

    printf("UI create socket \n");

    g_uiUi_socket_fd = socket(PF_UNIX, SOCK_STREAM, CLEAR);
    if(g_uiUi_socket_fd < CLEAR)
    {
        perror("ui socket failed\n");
    }
    else
    {
        printf("socket success!!!!!!!!\n");
    }

    g_app_ui_addr.sun_family = AF_UNIX;
    g_uiApp_ui_addrlen = sizeof(g_app_ui_addr.sun_family) +
    sprintf(g_app_ui_addr.sun_path,"%s",uc_socket_name);


    if(connect(g_uiUi_socket_fd, (struct sockaddr *) &g_app_ui_addr, g_uiApp_ui_addrlen) != CLEAR)
    {
        perror("g_uiUi_socket_fd Connect Error= %s !!!!!!!!!! \n");
    }
    else
    {
        printf("connect succes \n");
    }
    FD_SET(g_uiUi_socket_fd,&app_readset);


}

/*************************************************************************************
 * Function name: create_radvision_communication_socket                              *
 * Input arg    : void                                                               *
 * Output arg   : NONE                                                               *
 * Description  : This function used to create unix socket between main and
                  Radvision thread
                                                                                     *
*************************************************************************************/
void create_radvision_communication_socket()
{

    int res,result,fd;
   //  void *msg;
    char uc_socket_name[TEN]={"RADV"};

    printf("RADVISION create socket \n");
    unlink(uc_socket_name);
    g_uiRadv_sock_fd = socket(PF_UNIX, SOCK_STREAM, CLEAR);
    if(g_uiRadv_sock_fd < CLEAR)
    {
        perror("ui socket failed\n");
    }
    else
    {
        printf("socket success!!!!!!!!\n");
    }

    radv_sock_addr.sun_family = AF_UNIX;
    g_uiRadv_addrlen = sizeof(radv_sock_addr.sun_family) +
        sprintf(radv_sock_addr.sun_path,"%s",uc_socket_name);

    if(bind(g_uiRadv_sock_fd,&radv_sock_addr,g_uiRadv_addrlen)!=CLEAR)
    {
        perror("radv bind failed\n");
    }

	listen(g_uiRadv_sock_fd,MAX_CLIENTS);
    FD_SET(g_uiRadv_sock_fd,&app_readset);


}
/*************************************************************************************
 * Function name: init_radvision_and_dsp                                             *
 * Input arg    : void                                                               *
 * Output arg   : NONE                                                               *
 * Description  : This function initializes the Radvison stack and DSP               *
                                                                                     *
*************************************************************************************/

void init_radvision_and_dsp()
{
    regional_settings_params_t *regionalSettings = NULL;
    printf("inside init_radvision_and_dsp\n");
    /* NirM 080304 - Load DSP kernel modules */
//	load_dsp_kernel_modules();
    voip_conf_init();

#if defined(AC494_SDP_IPPHONE) || defined(AC49_ORCHID_VOIP_TOOLKIT)
#ifdef CONFIG_GTEK
	init_message_queues(S2U,U2S);
#else
	startKeypad();
#ifndef NANOX
	//lcdInit();
#endif
#endif
#endif
	initVoipStatusHandler();
    /* called once on the start of the application */
    if (voip_stack_init())
    {
        printf("voip stack init failed\n");

    }

	if(voip_conf_util_load_cfg_file(receivedCfgBuf) == -1)
	{
        printf("voip_conf_util_load_cfg_file failed\n");

	}


/* Start stack for default state of the device. On receive signals stack
	 * will be restarted */
	voip_stack_start();
	/* NirM 080303, Initializing DSP */
    voip_conf_dsp_params_get(&regionalSettings, &AdvancedChannelConfiguration);
	dsp_init(regionalSettings, &AdvancedChannelConfiguration);
	//waitUntilVoipTaskReadyForReconfig = 0;  /* voip_task is up, can be reconfiguraed */


}

/*************************************************************************************
 * Function name: create_server_udp_comm_socket                                      *
 * Input arg    : void                                                               *
 * Output arg   : NONE                                                               *
 * Description  : This function is used to create socket for  unicast communication  *
                  between station and server.                                        *
*************************************************************************************/

void create_server_udp_comm_socket()
{
   /* set up socket */
    g_uiServ_udp_comm_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (g_uiServ_udp_comm_fd < CLEAR)
    {
        perror("server udp comm socket:\n");
        exit(1);
    }
    else
    {
        printf("create_server_udp_comm_socket reg created \n");
    }
    bzero((char *)&reg_addr, sizeof(reg_addr));
    udp_comm_addr.sin_family = AF_INET;
    udp_comm_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    udp_comm_addr.sin_port = htons(atoi(UNICAST_PORT));

  // g_uiReg_addrlen = sizeof(reg_addr);       /* multicast request structure *//*@P*/
    g_uiUdp_comm_addrlen = sizeof(udp_comm_addr);


     /* receive */
    if (bind(g_uiServ_udp_comm_fd, (struct sockaddr *) &udp_comm_addr,
        sizeof(udp_comm_addr)) < CLEAR)
    {
        perror("bind create_server_udp_comm_socket:");
        exit(1);
    }
    FD_SET(g_uiServ_udp_comm_fd,&app_readset);

}


/********************************************************************************************************
 * Function name: reset_hopped_intercom                                                                 *
 * Input arg    : void                                                                                  *
 * Output arg   : NONE                                                                                  *
 * Description  : This function resets the channel hopped call on reset key pressed or cradle put action*
*********************************************************************************************************/

void reset_hopped_intercom()
{
	printf("ICOM HOP RESET \n");
	//operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
	pvt_call.curr_icom_no=stn_config.default_intercom-MIN_OFFSET;
	if(stn_config.icom_live_dead_status[stn_config.default_intercom-MIN_OFFSET]==LIVE)
	{
		add_membership(g_Iicom_fds[stn_config.default_intercom-MIN_OFFSET]);
	}
	pthread_mutex_lock(&lock_call_type);
	pvt_call.type=INTERCOM_RX;
	pthread_mutex_unlock(&lock_call_type);
	g_iIcom_hop_flag=CLEAR;
}
/***************************************************************************************
 * Function name: reset_all                                                            *
 * Input arg    : void                                                                 *
 * Output arg   : NONE                                                                 *
 * Description  : This function kills the main process after receiving wake up event   *
                                                                                       *
****************************************************************************************/
void reset_all()
{
	g_cInit_app=CLEAR;
}

/***************************************************************************************
 * Function name: Restart_Broadcast_Socket                                             *
 * Input arg    : void                                                                 *
 * Output arg   : NONE                                                                 *
 * Description  : This function close and restart Broadcast socket after receiving     *
                  Broadcast_timeout event                                              *
                                                                                       *
****************************************************************************************/

void Restart_Broadcast_Socket()
{
        printf("INSIDE Restart_Broadcast_Socket \n");
        close(g_uiBroadcast_fd);
        printf("Broadcast Socket is closed\n");
        create_broadcast_reg_socket();

}
/*************************************************************************************
 * Function name: operate_gpio                                                       *
 * Input arg    : int gpio_no,int command,int action                                 *
 * Output arg   : int gpio_value                                                     *
 * Description  : This is used to operate GPIO as per requirement                    *
*************************************************************************************/
int operate_gpio(int gpio_no,int command,int action)
{

    char c_buf[FIFTY];
    int n;
    struct gpio_operations gp;
    gp.gpio_no=gpio_no;
    gp.gpio_command=command; //1=write 0=read
    gp.gpio_value=action;

    memset(c_buf,CLEAR,sizeof(gp));

    switch(command)
    {
        case SET:
                memcpy(c_buf,&gp,sizeof(gp));
                write(g_iGpio_fd,&c_buf,sizeof(gp));
        break;

        case CLEAR:
                memcpy(&c_buf,&gp,sizeof(gp));
                n=read(g_iGpio_fd,&c_buf,sizeof(gp));

                memcpy(&gp,&c_buf,sizeof(gp));

        break;

        default: break;

    }

    return gp.gpio_value;
}

/*************************************************************************************
 * Function name: init_gpio                                                          *
 * Input arg    : void                                                               *
 * Output arg   : NONE                                                               *
 * Description  : This is used to g_iGpio_fd and to create gpio_thread                  *
*************************************************************************************/
void init_gpio()
 {

    //pthread_t g_thread;

    g_iGpio_fd=open("/dev/phiGPIO",O_RDWR);
    if(g_iGpio_fd<CLEAR)
    {
        perror("Failed to open gpio fd\n");
        return;
    }

    //pthread_create(&g_thread,NULL,gpio_input_thread,CLEAR);
 }
/***************************************************************************************
 * Function name: start_aim_receive_call                                               *
 * Input arg    : void                                                                 *
 * Output arg   : NONE                                                                 *
 * Description  : This function used to open DSP channel no 1 to receive AIM call      *
****************************************************************************************/
void start_aim_receive_call()
{
    printf("AIM Starting \n");

    pg_call.type=AIM;

    pWork_m.page_state=PAGE_INCOMING_ONGOING;
    select_codec(PG_CHANNEL,NONE);
   // operate_gpio(MUSIC_VOL_BYPASS,WRITE,GPIO_OFF);
    multicast_prepare_dsp(PG_CHANNEL);
    select_codec(PG_CHANNEL,SPEAKER);
    set_volume(PG_CHANNEL,stn_config.music_max);

}

/***************************************************************************************
 * Function name: stop_aim_receive_call                                                *
 * Input arg    : void                                                                 *
 * Output arg   : NONE                                                                 *
 * Description  : This function used to close DSP channel no 1 to stop receive AIM call*
****************************************************************************************/
void stop_aim_receive_call()
{
    pWork_m.page_state=PAGE_IDLE;

    pg_call.type=CLEAR;

    printf("AIM STOPPED\n");
    close_channel(PG_CHANNEL);
    select_codec(PG_CHANNEL,NONE);

}

/***************************************************************************************
 * Function name: add_route_to_system                                                  *
 * Input arg    : char *address                                                        *
 * Output arg   : NONE                                                                 *
 * Description  : This function adds multicast address in systems routing table         *
****************************************************************************************/
void add_route_to_system(char *address)
{
    char c_buf[MAX_BUFFER_SIZE];
    sprintf(c_buf,route_add_msg,address);
    system(c_buf);
}

/***************************************************************************************
 * Function name: del_route_from_system                                                  *
 * Input arg    : char *address                                                        *
 * Output arg   : NONE                                                                 *
 * Description  : This function removes multicast address from systems routing table    *
****************************************************************************************/
void del_route_from_system(char *address)
{
    char c_buf[MAX_BUFFER_SIZE];
    sprintf(c_buf,route_del_msg,address);
    system(c_buf);

}


/***************************************************************************************
 * Function name: establish_pg_call_in_backgroud                                       *
 * Input arg    : int event                                                            *
 * Output arg   : NONE                                                                 *
 * Description  : This function receives and process incoming page call event here only*
                  no need to maintain fsu state so handled in background.              *
****************************************************************************************/
void handle_pg_call_in_backgroud(int event)
{

    switch(event)
    {


    	case INCOMING_ZONE_CALL_RX_EVENT:
    			drop_membership(g_iAim_fds[stn_config.default_AIM-1]);
                stop_aim_receive_call();
                /*operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_ON);
                operate_gpio(MUSIC_VOL_BYPASS,WRITE,GPIO_OFF);
                operate_gpio(PFC_ACT,WRITE,GPIO_ON);*/
                send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,LED_CALL_THROUGH,SET,&io_led[ONE],IO_ADDRESS,
                LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,PAGE_SPEAKER,SET,&io_led[FIVE],IO_ADDRESS,
                LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                add_membership(g_izone_fds[pg_call.zone_no]);
                //   send_req_msg_to_server(ICOM_PTT_RELEASE_REQ,ICOM_CALL_FRAME_LENGTH,'1','0');
                printf("INCOMING_ZONE_CALL_RX_EVENT call event received\n");
               // send_msg_ui(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,g_cLine3_buf,0);
                sprintf(g_cLine3_buf,zone_call_msg,lcd_zone_call_init);
                // send_msg_ui(PAGE_INITIATOR_STATE,LCD,LINE3,CONTINUE_DISPLAY,global_call_msg,CLEAR,0);
                //send_msg_ui(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,g_cLine3_buf,LCD_CLR_FIRST_LINE,0);
                send_msg_ui_lcd(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                g_uiPage_call_timer=stn_config.pg_call_timeout*TWO;
                g_uiPage_call_timer_lcd = stn_config.pg_call_timeout*10;
                pg_call.pg_call_busy = SET;
                pg_call.type=ZONE;
                pWork_m.page_state=PAGE_INCOMING_ONGOING;
                operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);
                select_codec(PG_CHANNEL,NONE);
                multicast_prepare_dsp(PG_CHANNEL);
                select_codec(PG_CHANNEL,SPEAKER);
                set_volume(PG_CHANNEL,MAX_PG_VOLUME);
                sleep(ONE);
                set_volume(PG_CHANNEL,MAX_PG_VOLUME-ONE);
                // start_alarm(incoming_alm_no);
                g_uiFsu_event=CLEAR;
    	break;


    	case INCOMING_ZONE_ALM_RX_EVENT:
    			drop_membership(g_iAim_fds[stn_config.default_AIM-1]);
                stop_aim_receive_call();
                /*operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_ON);
                operate_gpio(MUSIC_VOL_BYPASS,WRITE,GPIO_OFF);
                operate_gpio(PFC_ACT,WRITE,GPIO_ON);*/
                send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,LED_CALL_THROUGH,SET,&io_led[ONE],IO_ADDRESS,
                LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,PAGE_SPEAKER,SET,&io_led[FIVE],IO_ADDRESS,
                LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                add_membership(g_izone_fds[pg_call.zone_no]);
                //   send_req_msg_to_server(ICOM_PTT_RELEASE_REQ,ICOM_CALL_FRAME_LENGTH,'1','0');
                printf("INCOMING_ZONE_ALM_RX_EVENT call event received\n");
                //send_msg_ui(IDLE_STATE,LCD,LINE3,CONTINUE_DISPLAY,g_cLine3_buf,0);
                sprintf(g_cLine3_buf,zone_alm_msg,lcd_alm_zone_no);
                send_msg_ui_lcd(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                //send_msg_ui(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,g_cLine3_buf,LCD_CLR_FIRST_LINE,0);
               // g_uiPage_call_timer=stn_config.pg_call_timeout*TWO;
                g_uiPage_call_timer=stn_config.alarm_call_timeout*TWO;
                g_uiPage_call_timer_lcd = stn_config.alarm_call_timeout*10;
                printf("****** g_uiPage_call_timer in INCOMING_ZONE_ALM_RX_EVENT %d\n",g_uiPage_call_timer);
                pg_call.pg_call_busy = SET;
                pg_call.type=ZONE_ALARM;
                pWork_m.page_state=PAGE_INCOMING_ONGOING;
                operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);
                select_codec(PG_CHANNEL,NONE);
                multicast_prepare_dsp(PG_CHANNEL);
                select_codec(PG_CHANNEL,SPEAKER);
                set_volume(PG_CHANNEL,MAX_PG_VOLUME);
                sleep(ONE);
                set_volume(PG_CHANNEL,MAX_PG_VOLUME-ONE);
                //start_alarm(incoming_alm_no);
		//stop_alarm(pg_call.alm_no);
              //  start_alarm(pg_call.alm_no);
                g_uiFsu_event=CLEAR;
    	break;

        case GLOBAL_RX_ALM_EVENT:
                drop_membership(g_iAim_fds[stn_config.default_AIM-1]);
                stop_aim_receive_call();
                /*operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_ON);
                operate_gpio(MUSIC_VOL_BYPASS,WRITE,GPIO_OFF);
                operate_gpio(PFC_ACT,WRITE,GPIO_ON);*/
                send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,LED_CALL_THROUGH,SET,&io_led[ONE],IO_ADDRESS,
                LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,PAGE_SPEAKER,SET,&io_led[FIVE],IO_ADDRESS,
                LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                //   send_req_msg_to_server(ICOM_PTT_RELEASE_REQ,ICOM_CALL_FRAME_LENGTH,'1','0');
                printf("GLOBAL_RX_ALM_EVENT call event received");
                //send_msg_ui(IDLE_STATE,LCD,LINE3,CONTINUE_DISPLAY,g_cLine3_buf,0);
                sprintf(g_cLine3_buf,global_alm_msg,lcd_global_alm_init);
                send_msg_ui_lcd(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                //send_msg_ui(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,g_cLine3_buf,LCD_CLR_FIRST_LINE,0);

               // g_uiPage_call_timer=stn_config.pg_call_timeout*TWO;
                g_uiPage_call_timer=stn_config.alarm_call_timeout*TWO;
                g_uiPage_call_timer_lcd = stn_config.alarm_call_timeout*10;
                printf("****** g_uiPage_call_timer in GLOBAL_RX_ALM_EVENT %d\n",g_uiPage_call_timer);
                pg_call.pg_call_busy = SET;
                pg_call.type=GLOBAL_ALARM;
                pWork_m.page_state=PAGE_INCOMING_ONGOING;
               // operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);
                select_codec(PG_CHANNEL,NONE);
                multicast_prepare_dsp(PG_CHANNEL);
                select_codec(PG_CHANNEL,SPEAKER);
                set_volume(PG_CHANNEL,MAX_PG_VOLUME);
                sleep(ONE);
                set_volume(PG_CHANNEL,MAX_PG_VOLUME-ONE);
                // start_alarm(incoming_alm_no);
		//stop_alarm(pg_call.alm_no);
                //start_alarm(pg_call.alm_no);
                g_uiFsu_event=CLEAR;

        break;

        case GLOBAL_RX_CALL_EVENT:
                drop_membership(g_iAim_fds[stn_config.default_AIM-ONE]);
                stop_aim_receive_call();
             //   send_req_msg_to_server(ICOM_PTT_RELEASE_REQ,ICOM_CALL_FRAME_LENGTH,'1','0');
                /*operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_ON);
                operate_gpio(MUSIC_VOL_BYPASS,WRITE,GPIO_OFF);
                operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);
                operate_gpio(PFC_ACT,WRITE,GPIO_ON);*/
                send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,LED_CALL_THROUGH,SET,&io_led[ONE],IO_ADDRESS,
                LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,PAGE_SPEAKER,SET,&io_led[FIVE],IO_ADDRESS,
                LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                pg_call.type=GLOBAL;
                printf("GLOBAL_RX_CALL_EVENT call event received");
               // send_msg_ui(IDLE_STATE,LCD,LINE3,CONTINUE_DISPLAY,g_cLine3_buf,0);
                sprintf(g_cLine3_buf,global_call_msg,lcd_global_call_init);
                send_msg_ui_lcd(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                // send_msg_ui(PAGE_INITIATOR_STATE,LCD,LINE3,CONTINUE_DISPLAY,global_call_msg,CLEAR);
                //send_msg_ui(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,g_cLine3_buf,LCD_CLR_FIRST_LINE,0);
                g_uiPage_call_timer=stn_config.pg_call_timeout*TWO;
                g_uiPage_call_timer_lcd = stn_config.pg_call_timeout*10;
                pWork_m.page_state=PAGE_INCOMING_ONGOING;
                pg_call.pg_call_busy = SET;
                multicast_prepare_dsp(PG_CHANNEL);
                select_codec(PG_CHANNEL,NONE);
                select_codec(PG_CHANNEL,SPEAKER);
                set_volume(PG_CHANNEL,MAX_PG_VOLUME);
                sleep(ONE);
                set_volume(PG_CHANNEL,MAX_PG_VOLUME-ONE);
                g_uiFsu_event=CLEAR;
        break;

        case GLOBAL_RESET_RX_EVENT:
                close_channel(PG_CHANNEL);
                select_codec(PG_CHANNEL,NONE);
                pg_call.type=CLEAR;

                //operate_gpio(PFC_ACT,WRITE,GPIO_OFF);

                printf("GLOBAL_RESET_RX_EVENT call event received\n");
                if(pvt_call.pvt_call_busy!=SET)
                {
                    send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,LED_CALL_THROUGH,CLEAR,&io_led_clear[ZERO],IO_ADDRESS,
                    LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);

                }
                pg_call.alm_no=CLEAR;
               // send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,"                   ",0);
                pWork_m.page_state=PAGE_IDLE;
                pg_call.pg_call_busy = CLEAR;
                g_uiPage_call_timer=CLEAR;

				printf("busy %d  signal %d \n",pvt_call.pvt_call_busy,g_iHandset_lifted_signal);
                if(pvt_call.pvt_call_busy!=CLEAR && g_iHandset_lifted_signal==CLEAR)
                {
                    send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,PAGE_SPEAKER,CLEAR,&io_led_clear[ZERO],IO_ADDRESS,
                    LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                }
                else
                {
                    send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,PAGE_SPEAKER,SET,&io_led[FIVE],IO_ADDRESS,
                    LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                    printf("MUSIC VOLUME BYPASS ON \n");
                }

                if(g_iAim_mem_dropped==SET && stn_config.default_AIM!=CLEAR)
                {
                    add_membership(g_iAim_fds[stn_config.default_AIM-ONE]);
                    start_aim_receive_call();
                }
                g_uiFsu_event=CLEAR;
                //send_msg_ui(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,global_terminate,LCD_CLR_FIRST_LINE,0);
                send_msg_ui_lcd(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,global_terminate,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                sleep(0.5);
                sprintf(g_cLine4_buf,"%s          ",lcd_fcs_desc);
                send_msg_ui_lcd(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,g_cLine4_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                //send_msg_ui(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,lcd_fcs_desc,LCD_CLR_FIRST_LINE,0);
                if(lcd_fcs_default_icom == 0)
                {
                    sprintf(g_cLine3_buf,IDLE_msg,stn_config.logical_id);
                    send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                }
                else
                {
                    sprintf(g_cLine3_buf,Channel_msg,stn_config.logical_id,lcd_fcs_default_icom);
                    send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                }

        break;

        case ZONE_RESET_RX_EVENT:
                close_channel(PG_CHANNEL);
                select_codec(PG_CHANNEL,NONE);
                pg_call.type=CLEAR;
                drop_membership(g_izone_fds[pg_call.zone_no]);

                //operate_gpio(PFC_ACT,WRITE,GPIO_OFF);

                printf("ZONE RESET Rx call event received %c \n",pvt_call.pvt_call_busy);
                if(pvt_call.pvt_call_busy!=SET)
                {
                    send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,LED_CALL_THROUGH,CLEAR,&io_led_clear[ZERO],IO_ADDRESS,
                    LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                }
                pg_call.zone_no=CLEAR;
                pg_call.alm_no = CLEAR;

               // send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,"                   ",0);
                pWork_m.page_state=PAGE_IDLE;
                pg_call.pg_call_busy = CLEAR;
                g_uiPage_call_timer=CLEAR;

				printf("busy %d  signal %d \n",pvt_call.pvt_call_busy,g_iHandset_lifted_signal);
                if(pvt_call.pvt_call_busy!=CLEAR && g_iHandset_lifted_signal==CLEAR)
                {
                    send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,PAGE_SPEAKER,CLEAR,&io_led_clear[ZERO],IO_ADDRESS,
                    LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                }
                else
                {
                    send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,PAGE_SPEAKER,SET,&io_led[FIVE],IO_ADDRESS,
                    LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                    printf("MUSIC VOLUME BYPASS ON \n");
                }

                if(g_iAim_mem_dropped==SET && stn_config.default_AIM!=CLEAR)
                {
                    add_membership(g_iAim_fds[stn_config.default_AIM-ONE]);
                    start_aim_receive_call();
                }
                g_uiFsu_event=CLEAR;

                // send_msg_ui(PAGE_INITIATOR_STATE,LCD,LINE3,CONTINUE_DISPLAY,global_call_msg,CLEAR);
                //send_msg_ui(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,zone_terminate,LCD_CLR_FIRST_LINE,0);
                send_msg_ui_lcd(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,zone_terminate,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                sleep(0.5);
                sprintf(g_cLine4_buf,"%s          ",lcd_fcs_desc);
                send_msg_ui_lcd(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,g_cLine4_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                  if(lcd_fcs_default_icom == 0)
                  {
                        sprintf(g_cLine3_buf,IDLE_msg,stn_config.logical_id);
                        send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                  }
                  else
                  {
                        sprintf(g_cLine3_buf,Channel_msg,stn_config.logical_id,lcd_fcs_default_icom);
                        send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                  }
        break;

        default:
        break;
    }

}




/***************************************************************************************
 * Function name: start_intercom                                                       *
 * Input arg    : void                                                                 *
 * Output arg   : NONE                                                                 *
 * Description  : This function Opens DSP channel 0 & 2 and do conferncing between them*
****************************************************************************************/
void start_intercom()
{
    select_codec(PVT_CHANNEL,NONE);
    multicast_prepare_dsp(PVT_CHANNEL);
    multicast_prepare_dsp(CONF_CHANNEL);
    //  pvt_call.pvt_call_busy=SET;
    select_codec(PVT_CHANNEL,HANDSET);
    //select_codec(PVT_CHANNEL,SPEAKER);///
    pthread_mutex_lock(&lock_call_type);
    pvt_call.type=INTERCOM_RX;
    pthread_mutex_unlock(&lock_call_type);
    // start_alarm(10);

    acgStart3WayConference(PVT_CHANNEL,CONF_CHANNEL);

    g_itransmit_fd_index=-1;

}

/***************************************************************************************
 * Function name: reset_intercom                                                       *
 * Input arg    : void                                                                 *
 * Output arg   : NONE                                                                 *
 * Description  : This function used to close DSP channel no 0 & 2.                      *
****************************************************************************************/
void reset_intercom()
{

    //operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);

    select_codec(PVT_CHANNEL,NONE);
    //select_codec(PVT_CHANNEL,HANDSET);

   // acgStop3WayConference(PVT_CHANNEL,CONF_CHANNEL);
    close_channel(PVT_CHANNEL);
    //close_channel(CONF_CHANNEL);
    pthread_mutex_lock(&lock_call_type);
    pvt_call.type=CLEAR;
    pthread_mutex_unlock(&lock_call_type);
    pWork_m.private_state=PRIVATE_IDLE;
    g_itransmit_fd_index=-1;

}

/***************************************************************************************
 * Function name: initialize_fsu                                                       *
 * Input arg    : void                                                                 *
 * Output arg   : NONE                                                                 *
 * Description  : This function creates UI thread and all sockets used for the communication*
****************************************************************************************/

void initialize_fsu()
 {

    pthread_t m_thread;

    pthread_create(&m_thread,NULL,ui_thread,CLEAR);//usleep(500000);
    create_app_timer();
    create_diagnostic_timer();
    init_config_manager();
    UI_socket_create();
    create_broadcast_reg_socket();
    create_server_udp_comm_socket();
    create_radvision_communication_socket();
	init_gpio();
	//operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
	//operate_gpio(LCD_BACKLIGHT,WRITE,GPIO_OFF);
	g_uiServer_live_status_timer=SERVER_LIVE_TIMEOUT;

    if (pthread_mutex_init(&lock_icom_contr, NULL) != CLEAR)
    {
        printf("\n mutex init has failed\n");

    }

    if (pthread_mutex_init(&lock_call_type, NULL) != CLEAR)
    {
        printf("\n mutex init has failed\n");

    }


 }


/***************************************************************************************
 * Function name: save_server_ip_in_config_file                                        *
 * Input arg    : void                                                                 *
 * Output arg   : NONE                                                                 *
 * Description  : This function saves the multicast start addresses received from server*
****************************************************************************************/
 void save_server_ip_in_config_file()
 {

    FILE *org_file,*temp_file;
	int i_line_no=FOURTY_THREE,i_cntr=CLEAR;
	char str[MAX_BUFFER_SIZE];
    char ip_string[MAX_BUFFER_SIZE],ip_str1[MAX_BUFFER_SIZE];
    sprintf(&ip_string[0],"%s%s",SERVER_IP_STRING,&g_cServer_addr);
    sprintf(&ip_str1[0],"%s%s","voip/signalling/sip/sip_registrar/addr=",&g_cServer_addr);
   // strcpy(ip_string,SERVER_IP_STRING);

	org_file = fopen(CONFIGURATION_FILE_NAME, FL_RD);
	temp_file = fopen("temp.txt",FL_WR);

	while(!feof(org_file))
	{
        fgets(str,MAX_BUFFER_SIZE,org_file);
        if(!feof(org_file))
        {
            i_cntr++;
            if(i_cntr==i_line_no)
            {
                fprintf(temp_file,"%s",ip_string);
                fprintf(temp_file,"\n");
            }
            else if(i_cntr==FOURTY_NINE)
            {
                fprintf(temp_file,"%s",ip_str1);
                fprintf(temp_file,"\n");
            }
            else
            {
                fprintf(temp_file,"%s",str);
            }

        }
    }
    fclose(org_file);
    fclose(temp_file);
    remove(org_file);
    rename("temp.txt",CONFIGURATION_FILE_NAME);
 }



/***************************************************************************************
 * Function name: load_config_file                                                     *
 * Input arg    : void                                                                 *
 * Output arg   : NONE                                                                 *
 * Description  : This function loads the configuration file which is used by radvision     stack                                                                                   *
****************************************************************************************/
void load_config_file()
{
    char	*uc_pConfigFileName;
	uc_pConfigFileName = AC_ZALLOC_L(CONFIGURATION_FILE_NAME_SIZE);
	strcpy(uc_pConfigFileName,CONFIGURATION_FILE_NAME);

	memset(g_cAcl_cfgfilename, CLEAR, CONFIGURATION_FILE_NAME_SIZE);
	strcpy(g_cAcl_cfgfilename, uc_pConfigFileName);


    if(cfg_util_load_file(receivedCfgBuf, sizeof(receivedCfgBuf), uc_pConfigFileName) != 0)
    {
        printf("ERROR: Failed to read %s File\n", uc_pConfigFileName);

    }

    acl_log_set_debug_level_param(1);
	if(uc_pConfigFileName)
	{
        nfree(uc_pConfigFileName);
    }


}


/***************************************************************************************
 * Function name: main                                                                 *
 * Input arg    : int argc,char **argv                                                 *
 * Output arg   : NONE                                                                 *
 * Description  : This function controls main call logic and process events received   *
                  from various therads.
****************************************************************************************/
void main(int argc, char **argv)
{

    unsigned char uc_buf[TWENTY];
    unsigned int  ui_pressed_key;

    signal(SIGTERM, catch_term);
    //signal(SIGINT,catch_term1);
    signal(SIGSEGV,catch_term2);






    printf("VoIP task started %x\n", addr);
    printf("VoIP is running...\n");
    char a[]={0x00};
    /////phi implementation///////////////////
    g_uiFsu_state = INIT_STATE;
    g_uiFsu_event = INIT_EVENT;

    while(g_cInit_app)

    {
         process_events_listen();
         if(g_uiFsu_event)
         {
              handle_pg_call_in_backgroud(g_uiFsu_event);
              switch(g_uiFsu_state)
              {
                    case INIT_STATE:
                        switch(g_uiFsu_event)
                        {
               			 case INIT_EVENT:

                                g_uiFsu_event = CLEAR;
                                printf("inside INIT_EVENT\n");
                                initialize_fsu();
                                printf("inside VCCS_REGISTERING_STATE \n" );
                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,"  VoIP System   ",ZERO,
                                LCD_ADDRESS,IOCTL_PRINT);

                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,"  Phi Audiocom  ",ZERO,
                                LCD_ADDRESS,IOCTL_PRINT);

                                g_uiFsu_state=VCCS_REGISTERING_STATE;
                            break;

                        }
                        g_uiFsu_event=CLEAR;
                break;

                case VCCS_REGISTERING_STATE:
                        switch(g_uiFsu_event)
                        {

                            case SERVER_REG_BCAST_EVENT:
                                g_uiFsu_event=CLEAR;
                                save_server_ip_in_config_file();
                                load_config_file();
                                send_req_msg_to_server(0,CONFIG_REQUEST,CONFIG_REQUEST_FRAME_LENGTH,'0','0');
                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,"                ",ZERO,
                                LCD_ADDRESS,IOCTL_PRINT);
                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,"  REGISTERING  ",ZERO,
                                LCD_ADDRESS,IOCTL_PRINT);

                                sleep(1);
                                printf("broadcast event received \n");
                                g_uiVoipapp_wait_timer=CONFIGURATION_RESPONSE_TIMEOUT;
                                g_uiFsu_state=CONFIGURING_STATE;
                            break;
                        }
                        g_uiFsu_event=CLEAR;
                break;


                case CONFIGURING_STATE:
                        switch(g_uiFsu_event)
                        {
                            case CONFIG_RECEIVED:
                                g_uiFsu_event = CLEAR;
                                printf("CONFIG_RECEIVED SUCCESSFULLY \n");
                                g_uiVoipapp_wait_timer=CLEAR;
                                send_req_msg_to_server(0,CONFIG_OK_RESPONSE,CONFIG_OK_RESPONSE_FRAME_LENGTH,
                                '0','0');
                                send_msg_ui_lcd(VCCS_REGISTERING_STATE,LCD,LINE2,CONTINUE_DISPLAY,"  CONFIGURING  ",ZERO,
                                LCD_ADDRESS,IOCTL_PRINT);

                                sleep(2);
                                g_uiVoipapp_wait_timer=CONFIGURATION_RESPONSE_TIMEOUT;
                                g_uiFsu_state=DSP_RADV_INIT_STATE;
                            break;

                            case APP_TIMEOUT_EVENT:
                                printf("CONFIGURING_STATE STATE TIMEOUT \n");
                                g_uiFsu_event=CLEAR;
                                send_msg_ui_lcd(VCCS_REGISTERING_STATE,LCD,LINE2,CONTINUE_DISPLAY,"  REGISTERING  ",ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                g_uiFsu_state=VCCS_REGISTERING_STATE;
                            break;
                        }
                        g_uiFsu_event=CLEAR;
                break;


                /*case DSP_RADV_INIT_STATE:
                        switch(g_uiFsu_event)
                        {
                            case ACK_RESPONSE:
                                g_uiVoipapp_wait_timer=CLEAR;
                                init_radvision_and_dsp();
                                g_uiFsu_state=SIP_REGISTERING_STATE;
                            break;

                            case APP_TIMEOUT_EVENT:
                                printf("CONFIGURING_STATE STATE TIMEOUT \n");
                                g_uiFsu_event=CLEAR;
                                g_uiFsu_state=VCCS_REGISTERING_STATE;
                            break;

                        }
                        g_uiFsu_event=CLEAR;
                break;*/

                case DSP_RADV_INIT_STATE:
                        switch(g_uiFsu_event)
                        {
                            case ACK_RESPONSE:
                                g_uiVoipapp_wait_timer=CLEAR;
                                init_radvision_and_dsp();
                                send_msg_ui_lcd(INIT_STATE,LCD,LINE1,CONTINUE_DISPLAY,lcd_fcs_desc,ZERO,
                                LCD_ADDRESS,IOCTL_PRINT);
                                if(lcd_fcs_default_icom == 0)
                                {
                                    sprintf(g_cLine3_buf,IDLE_msg,stn_config.logical_id);
                                    send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                }
                                else
                                {
                                    sprintf(g_cLine3_buf,Channel_msg,stn_config.logical_id,lcd_fcs_default_icom);
                                    send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                }

                                printf("state changed to IDLE STATE  \n");

                                create_multicast_signalling_socket();
                                save_mul_starting_address();
                                Initialize_ssrc(stn_config.logical_id);
                                acl_init_paging();
                                send_req_msg_to_server(g_uiFsu_state,KEEP_ALIVE_FRAME,KEEP_ALIVE_FRAME_LENGTH,'0','0');
                                g_uiKeep_alive_timer= KEEP_ALIVE_TIMEOUT;
                                printf("g_uiKeep_alive_timer:%d\n",g_uiKeep_alive_timer);

                                if(stn_config.icom_live_dead_status[stn_config.default_intercom-MIN_OFFSET]==LIVE)
                                {
                                    pvt_call.curr_icom_no=stn_config.default_intercom-MIN_OFFSET;
                                    add_membership(g_Iicom_fds[pvt_call.curr_icom_no]);
                                    start_intercom();
                                }
                                icom_contributor_status_info[ZERO].cont1=111;

                                icom_contributor_status_info[ZERO].cont2=114;

                                if(stn_config.default_AIM)
                                {
                                    //operate_gpio(MUSIC_VOL_BYPASS,WRITE,GPIO_OFF);
                                    /*send_msg_ui(IDLE_STATE,IO,LINE2,CONTINUE_DISPLAY,&io_led[5],ZERO,IO_ADDRESS,
                                    LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);*/
                                    add_membership(g_iAim_fds[stn_config.default_AIM-1]);
                                    start_aim_receive_call();
                                }

                                acgPlayTone_phi(PVT_CHANNEL,VOIP_SIGNAL_STUTTER_TONE,START_TONE,CLEAR);
                                //operate_gpio(MUSIC_VOL_BYPASS,WRITE,GPIO_OFF);
                                g_uiPlay_tone_timer=THREE;
                                g_uiFsu_state=IDLE_STATE;
                                //send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,&io_led[ONE],IO_ADDRESS,LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);



                               // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                                printf("Muting Page Speaker.......///////////\n");

                            break;

                            case APP_TIMEOUT_EVENT:
                                printf("DSP_RADV_INIT_STATE  TIMEOUT \n");
                                send_msg_ui_lcd(VCCS_REGISTERING_STATE,LCD,LINE2,CONTINUE_DISPLAY,"  REGISTERING  ",ZERO,
                                LCD_ADDRESS,IOCTL_PRINT);
                                g_uiFsu_state=VCCS_REGISTERING_STATE;
                            break;
                        }
                        g_uiFsu_event=CLEAR;
                break;

                case IDLE_STATE:
                       //printf("g_uiKeep_alive_timer:%d\n",g_uiKeep_alive_timer);
                       // sprintf(g_cMulticast_tx_address,"%s%d",gc_icom_mul_start_addr,g_iIcom_mul_addr_start_offset+pvt_call.curr_icom_no);
                       // printf("ICOM multicast tx address %s\n",g_cMulticast_tx_address);
                       //     add_route_to_system(g_cMulticast_tx_address);
                        switch(g_uiFsu_event)
                        {
                            case WAKE_UP_EVENT:
                                 printf("____________IDLE_STATE_______WAKE_UP_EVENT___\n");
                                 reset_all();
                                // g_uiFsu_state=VCCS_REGISTERING_STATE;
                            break;
                            case BROADCAST_TIMEOUT_EVENT:
                                 printf("Inside BROADCAST_TIMEOUT_EVENT\n");
                                 Restart_Broadcast_Socket();
                            break;

                            case DIAGNOSTIC_EVENT:
                                FD_SET(g_uiApp_diag_timer_fd,&app_readset);
                                drop_membership(g_iAim_fds[stn_config.default_AIM-1]);
                                drop_membership(g_Iicom_fds[stn_config.default_intercom-MIN_OFFSET]);
                                // reset_intercom();
                                //   stop_aim_receive_call();
                                select_codec(PVT_CHANNEL,NONE);
                                select_codec(PVT_CHANNEL,HANDSET);
                                //operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
                                g_uiDiagnostic_start_flag=SET;

                                set_volume(PVT_CHANNEL,MAX_PVT_VOLUME);
                                g_uiVoipapp_wait_timer=TWENTY_FIVE;
                                start_alarm(TEN);
                                g_uiPvt_diagnostic_timer=DIAGNOSTIC_TIMER_PVT_PG;
                                g_uiFsu_state=DIAGNOSTICS_STATE;
                            break;

                            case RINGING_OFF_HOOK_EVENT:
                                drop_membership(g_Iicom_fds[pvt_call.curr_icom_no]);
                                // reset_intercom();
                                send_to_phone_input_cb(1,PHONE_INPUT_HOOK_OFF);
                                printf("OFF HOOK EVENT RECEIVED \n");
                            break;

                            case P2P_RESET_EVENT:

                                if(pg_call.pg_call_busy!=SET && stn_config.default_AIM==CLEAR)
                                {
                                	//operate_gpio(MUSIC_VOL_BYPASS,WRITE,GPIO_ON);
                                }
                                if(pg_call.pg_call_busy!=SET && g_iIcom_hop_flag==CLEAR)
                                {
                                    //operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                                }
                                send_to_phone_input_cb(1,PHONE_INPUT_HOOK_ON);

                                if(lcd_fcs_default_icom == 0)
                                {
                                    sprintf(g_cLine3_buf,IDLE_msg,stn_config.logical_id);
                                    send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                }
                                else
                                {
                                    sprintf(g_cLine3_buf,Channel_msg,stn_config.logical_id,lcd_fcs_default_icom);
                                    send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                }
                                g_uiPvt_call_timer=CLEAR;
                                pthread_mutex_lock(&lock_call_type);
                                pvt_call.type=CLEAR;
                                pthread_mutex_unlock(&lock_call_type);
                                pvt_call.pvt_call_busy=CLEAR;
                                g_uiFsu_state=IDLE_STATE;
                                if(g_iIcom_mem_dropped==SET&& stn_config.icom_live_dead_status[stn_config.default_intercom-MIN_OFFSET]==LIVE)
                                {
                                    add_membership(g_Iicom_fds[stn_config.default_intercom-MIN_OFFSET]);
                                    reset_intercom();
                                    start_intercom();
                                }

                                acgPlayTone_phi(PVT_CHANNEL,VOIP_SIGNAL_STUTTER_TONE,START_TONE,CLEAR);
                                g_uiPlay_tone_timer=THREE;
                                printf("P2P Reset in IDLE STATE received\n");
                            break;

                            case PVT_CALL_TIMEOUT:
                                 printf("PVT_CALL_TIMEOUT in IDLE state \n");
                                if(pg_call.pg_call_busy!=SET)
                                {
                                    //operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                                }
                                if(pg_call.pg_call_busy!=SET && stn_config.default_AIM==CLEAR)
                                {
                                	//operate_gpio(MUSIC_VOL_BYPASS,WRITE,GPIO_ON);
                                }
                              /// send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,"PVT_CALL_TIMEOUT",LCD_CLR_SECOND_LINE,0);
                                drop_membership(g_Iicom_fds[pvt_call.curr_icom_no]);
                                g_uiPvt_call_timer=CLEAR;
                                pvt_call.pvt_call_busy=CLEAR;
                                pvt_call.curr_icom_no=stn_config.default_intercom-MIN_OFFSET;
                                if(g_iIcom_mem_dropped==SET&&stn_config.icom_live_dead_status[stn_config.default_intercom-MIN_OFFSET]==LIVE)
                                {
                                    add_membership(g_Iicom_fds[stn_config.default_intercom-MIN_OFFSET]);
                                }

                                acgPlayTone_phi(PVT_CHANNEL,VOIP_SIGNAL_STUTTER_TONE,START_TONE,CLEAR);
                                g_uiPlay_tone_timer=THREE;
                                g_uiFsu_state=IDLE_STATE;

                                if(lcd_fcs_default_icom == 0)
                                {
                                    sprintf(g_cLine3_buf,IDLE_msg,stn_config.logical_id);
                                    send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                }
                                else
                                {
                                    sprintf(g_cLine3_buf,Channel_msg,stn_config.logical_id,lcd_fcs_default_icom);
                                    send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                }
                            break;

                            case INCOMING_GROUP_CALL_EVENT:
                                printf("INSIDE INCOMING_GROUP_CALL_EVENT OF IDLE_STATE\n");
                                drop_membership(g_Iicom_fds[pvt_call.curr_icom_no]);
                                // drop_membership(g_iAim_fds[stn_config.default_AIM-1]);
                                add_membership(g_igroup_fds[pvt_call.group_no]);
                              //  operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_ON);
                                if(g_iHandset_lifted_signal==CLEAR)
                                {
                                   // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_OFF);
                                    //operate_gpio(MUSIC_VOL_BYPASS,WRITE,GPIO_OFF);
                                }
                                g_uiVoipapp_wait_timer=CLEAR;
                                pthread_mutex_lock(&lock_call_type);
                                pvt_call.type=GROUP;
                                pthread_mutex_unlock(&lock_call_type);

                                printf("GROUP call event received\n");

                                sprintf(g_cLine3_buf,group_msg,lcd_group_call_no,lcd_group_call_init);
                               /// send_msg_ui(GROUP_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,LCD_CLR_SECOND_LINE,0);

                                pvt_call.pvt_call_busy=SET;
                                select_codec(PVT_CHANNEL,HANDSET);
                                set_volume(PVT_CHANNEL,MAX_PVT_VOLUME);
                                //pWork_m.private_state=PRIVATE_OUTGOING_ONGOING;
                               ///send_msg_ui(GROUP_STATE,LED,LED1,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR,0);
                                g_uiPvt_call_timer=stn_config.pvt_call_timeout*TWO;
                                g_uiPvt_call_timer_ui_lcd = stn_config.pvt_call_timeout*10;
                                g_uiPage_call_timer_frlcd_ini = 0;
                                acgPlayTone_phi(PVT_CHANNEL,VOIP_SIGNAL_STUTTER_TONE,START_TONE,CLEAR);
                                g_uiPlay_tone_timer=THREE;
                                g_uiFsu_state=GROUP_STATE;


                            break;


                            case VALID_KEY_SEQ_EVENT:
                                g_uiFsu_event=CLEAR;
                                g_uiVoipapp_wait_timer=CLEAR;
                                printf("valid key %c call \n",g_cReceived_key_seq[0]);

                                switch(g_cReceived_key_seq[0])
                                {

                                    case P2P:
                                        printf("in p2p dialling call \n");
                                        g_cCalled_station[0]=g_cReceived_key_seq[1];
                                        g_cCalled_station[1]=g_cReceived_key_seq[2];
                                        g_cCalled_station[2]=g_cReceived_key_seq[3];
                                        short stn_id;
                                        stn_id=atoi(g_cCalled_station);
                                        printf("stn_id is : %d",stn_id);
                                        if(stn_config.logical_id == stn_id)
                                        {
                                            printf("Please dial another number");
                                            clear_validation_processing();

                                            if(lcd_fcs_default_icom == 0)
                                            {
                                                sprintf(g_cLine3_buf,IDLE_msg,stn_config.logical_id);
                                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,
                                                LCD_ADDRESS,IOCTL_PRINT);
                                            }
                                            else
                                            {
                                                sprintf(g_cLine3_buf,Channel_msg,stn_config.logical_id,lcd_fcs_default_icom);
                                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,
                                                LCD_ADDRESS,IOCTL_PRINT);
                                            }
                                        }
                                        else
                                        {
                                            send_to_phone_input_cb(1,PHONE_INPUT_HOOK_OFF);
                                            sprintf(g_cLine2_buf,"DIALLING TO %s  ",g_cCalled_station);
                                            usleep(500000);
                                            send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,ZERO,
                                            LCD_ADDRESS,IOCTL_PRINT);


                                            g_uiVoipapp_wait_timer = THIRTY;
                                            g_uiFsu_state=P2P_OFF_HOOK_STATE;
                                        }

                                    break;

                                    case GROUP:
                                        printf("INSIDE GROUP state of valid_key_seq of IDLE_STATE \n");
                                        sprintf(uc_buf,"%c%c",g_cReceived_key_seq[1],g_cReceived_key_seq[2]);
                                        printf("group_no %s",uc_buf);
                                        ui_pressed_key=atoi(uc_buf);
                                        if(stn_config.group_initiator[ui_pressed_key-MIN_OFFSET]=='1')
                                        {
                                            pvt_call.group_no=ui_pressed_key-MIN_OFFSET;

                                            g_uiVoipapp_wait_timer=SERVER_RESPONSE_TIMEOUT;
                                            send_req_msg_to_server(0,GROUP_CALL_REQUEST,GROUP_CALL_FRAME_LENGTH,
                                            g_cReceived_key_seq[1],g_cReceived_key_seq[2]);
                                            sprintf(g_cLine2_buf,"DIALLING GNO:%s  ",uc_buf);
                                           /// send_msg_ui(RINGBACK_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,LCD_CLR_SECOND_LINE,0);
                                            g_uiFsu_state=RINGBACK_STATE;
                                            printf(" fsu_state is going to be GROUP state\n");
                                        }
                                        else
                                        {
                                            if( g_uiPlay_tone_timer==CLEAR)
                                            {
                                                acgPlayTone_phi(PVT_CHANNEL,VOIP_SIGNAL_STUTTER_TONE,START_TONE,CLEAR);
                                                g_uiPlay_tone_timer=SIX;
                                            }
                                        }
                                    break;

                                    case EPBX:

                                    break;

                                    case INTERCOM_RX:
                                        printf("in intercom dialling call \n");
                                        sprintf(uc_buf,"%c%c",g_cReceived_key_seq[1],g_cReceived_key_seq[2]);
                                        ui_pressed_key=atoi(uc_buf);

                                       if((stn_config.default_intercom!=ui_pressed_key)
                                        &&(stn_config.icom_live_dead_status[ui_pressed_key-MIN_OFFSET] == LIVE) )
                                        {

                                           // sprintf(hop_buf,"%c%c",g_cReceived_key_seq[2],g_cReceived_key_seq[1]);
                                            sprintf(g_cLine2_buf,Channel_msg,stn_config.logical_id,ui_pressed_key);
                                         ///    send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,LCD_CLR_SECOND_LINE,0);
                                            //operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_ON);
                                            drop_membership(g_Iicom_fds[stn_config.default_intercom-MIN_OFFSET]);
                                            pvt_call.curr_icom_no=ui_pressed_key-MIN_OFFSET;
                                            add_membership(g_Iicom_fds[pvt_call.curr_icom_no]);
                                            g_uiPvt_call_timer=stn_config.pvt_call_timeout*TWO;
                                            //pvt_call.pvt_call_busy=SET;
                                            g_iIcom_hop_flag=SET;
                                        }
                                        else
                                        {

                                            if( g_uiPlay_tone_timer==CLEAR)
                                            {
                                                acgPlayTone_phi(PVT_CHANNEL,VOIP_SIGNAL_BUSY_TONE,START_TONE,0);
                                                g_uiPlay_tone_timer=SIX;
                                            }
                                        }

                                    break;

                                    case GLOBAL:
                                        if(stn_config.call_permission_global_call==SET)
                                        {
                                            printf("in global dialling call\n");
                                            send_req_msg_to_server(0,GLOBAL_CALL_REQ,GLOBAL_CALL_FRAME_LENGTH,'0','0');
                                            g_uiVoipapp_wait_timer=SERVER_RESPONSE_TIMEOUT;
                                            send_msg_ui_lcd(RINGBACK_STATE,LCD,LINE1,CONTINUE_DISPLAY,"DIALLING GBL CL ",ZERO,
                                            LCD_ADDRESS,IOCTL_PRINT);
                                            g_uiFsu_state = RINGBACK_STATE;
                                        }
                                        else
                                        {
                                            if( g_uiPlay_tone_timer==CLEAR)
                                            {
                                                acgPlayTone_phi(PVT_CHANNEL,VOIP_SIGNAL_BUSY_TONE,START_TONE,CLEAR);
                                                g_uiPlay_tone_timer=SIX;
                                            }
                                        }
                                    break;

                                    case GLOBAL_ALARM:
                                        if(stn_config.call_permission_global_alarm_call==SET)
                                        {
                                            printf("in global alarm dialling call \n");
                                            send_req_msg_to_server(0,GLOBAL_ALARM_REQ,GLOBAL_ALARM_FRAME_LENGTH,
                                            g_cReceived_key_seq[1],g_cReceived_key_seq[2]);
                                            //send_msg_ui(RINGBACK_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,CLEAR);
                                            g_uiVoipapp_wait_timer=SERVER_RESPONSE_TIMEOUT;
                                            send_msg_ui_lcd(RINGBACK_STATE,LCD,LINE1,CONTINUE_DISPLAY,"DIALLING GBL ALM",ZERO,
                                            LCD_ADDRESS,IOCTL_PRINT);
                                            g_uiFsu_state = RINGBACK_STATE;
                                        }
                                        else
                                        {
                                            if( g_uiPlay_tone_timer==CLEAR)
                                            {
                                                acgPlayTone_phi(PVT_CHANNEL,VOIP_SIGNAL_BUSY_TONE,START_TONE,CLEAR);
                                                g_uiPlay_tone_timer=SIX;
                                            }
                                        }
                                    break;

                                    case ZONE:
                                    	sprintf(uc_buf,"%c%c",g_cReceived_key_seq[1],g_cReceived_key_seq[2]);
                                        ui_pressed_key=atoi(uc_buf);
                                        if(stn_config.zone_initiator[ui_pressed_key-MIN_OFFSET]=='1')
                                        {
                                            pg_call.zone_no=ui_pressed_key-MIN_OFFSET;
                                            g_uiVoipapp_wait_timer=SERVER_RESPONSE_TIMEOUT;
                                            send_req_msg_to_server(0,ZONE_CALL_REQUEST,ZONE_CALL_FRAME_LENGTH,g_cReceived_key_seq[1],g_cReceived_key_seq[2]);
                                            //send_msg_ui(RINGBACK_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,CLEAR);
                                            sprintf(g_cLine2_buf,"CALLING ZONE %d",lcd_zone_call_no);
                                            ///send_msg_ui(RINGBACK_STATE,LCD,LINE1,CONTINUE_DISPLAY,g_cLine2_buf,LCD_CLR_FIRST_LINE,0);
                                            send_msg_ui_lcd(P2P_STATE,LCD,LINE1,CONTINUE_DISPLAY,g_cLine2_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                            sleep(1);
                                            g_uiFsu_state=RINGBACK_STATE;
                                        }
                                        else
                                        {
                                            if( g_uiPlay_tone_timer==CLEAR)
                                            {
                                                acgPlayTone_phi(PVT_CHANNEL,VOIP_SIGNAL_BUSY_TONE,START_TONE,CLEAR);
                                                g_uiPlay_tone_timer=SIX;
                                            }
                                        }
                                    break;

                                    case ZONE_ALARM:
                                        sprintf(uc_buf,"%c%c",g_cReceived_key_seq[1],g_cReceived_key_seq[2]);
                                        ui_pressed_key=atoi(uc_buf);
                                        if(stn_config.zone_initiator[ui_pressed_key-MIN_OFFSET]=='1')
                                        {
                                            pg_call.zone_no=ui_pressed_key-MIN_OFFSET;
                                            g_uiVoipapp_wait_timer=SERVER_RESPONSE_TIMEOUT;
                                            send_req_msg_to_server(0,ZONE_ALM_REQUEST,ZONE_CALL_FRAME_LENGTH,g_cReceived_key_seq[3],g_cReceived_key_seq[4]);
                                            //send_msg_ui(RINGBACK_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,CLEAR);
                                            sprintf(g_cLine2_buf,zone_alm_msg,lcd_alm_zone_no);
                                           /// send_msg_ui(RINGBACK_STATE,LCD,LINE1,CONTINUE_DISPLAY,g_cLine2_buf,LCD_CLR_FIRST_LINE,0);
                                            sleep(1);
                                            g_uiFsu_state=RINGBACK_STATE;
                                        }
                                        else
                                        {
                                            if( g_uiPlay_tone_timer==CLEAR)
                                            {
                                                acgPlayTone_phi(PVT_CHANNEL,VOIP_SIGNAL_BUSY_TONE,START_TONE,CLEAR);
                                                g_uiPlay_tone_timer=SIX;
                                            }
                                        }
                                    break;

                                    default:
                                            sprintf(uc_buf,"%c%c",g_cReceived_key_seq[1],g_cReceived_key_seq[2]);
                                            ui_pressed_key=atoi(uc_buf);
                                            if(ui_pressed_key==PTT_KEY)
                                            {
                                                if(icom_contributor_status_info[pvt_call.curr_icom_no].cont1==CLEAR ||
                                                    icom_contributor_status_info[pvt_call.curr_icom_no].cont2==CLEAR)
                                                {
                                                    printf("IN PTT send \n");
                                                    g_uiVoipapp_wait_timer=SERVER_RESPONSE_TIMEOUT;
                                                    send_req_msg_to_server(0,ICOM_PTT_SEND_REQ,ICOM_CALL_FRAME_LENGTH,'1','0');
                                                    //send_msg_ui(RINGBACK_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,CLEAR);
                                                    // send_msg_ui(RINGBACK_STATE,LCD,LINE2,CONTINUE_DISPLAY,pttkey_msg,LCD_CLR_SECOND_LINE,0);
                                                     //sleep(0.5);
                                                    g_uiFsu_state=RINGBACK_STATE;

                                                }
                                                else
                                                {
                                                    if( g_uiPlay_tone_timer==CLEAR)
                                                    {
                                                        acgPlayTone_phi(PVT_CHANNEL,VOIP_SIGNAL_BUSY_TONE,START_TONE,CLEAR);
                                                        g_uiPlay_tone_timer=SIX;
                                                    }
                                                }
                                            }

                                            if(ui_pressed_key==RESET_KEY)
                                            {
                                            	if(g_iIcom_hop_flag==SET)
												{
													reset_hopped_intercom();

                                                if(lcd_fcs_default_icom == 0)
                                                {
                                                    sprintf(g_cLine3_buf,IDLE_msg,stn_config.logical_id);
                                                    send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,
                                                    LCD_ADDRESS,IOCTL_PRINT);
                                                }
                                                else
                                                {
                                                    sprintf(g_cLine3_buf,Channel_msg,stn_config.logical_id,lcd_fcs_default_icom);
                                                    send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,
                                                    LCD_ADDRESS,IOCTL_PRINT);
                                                }

												}

                                            }
                                    break;
                                    }
                            break;

                            case P2P_CALL_EVENT:
                           // case CALL_OFF_HOOK_EVENT:
                                g_uiVoipapp_wait_timer=CLEAR;
                                send_msg_ui_io(P2P_STATE,IO,CONTINUE_DISPLAY,LED_CALL_THROUGH,SET,&io_led[ONE],IO_ADDRESS,
                                LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                                usleep(100000);
                                /*if(g_iHandset_lifted_signal==SET)
                                {
                                   // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_OFF);
                                   send_msg_ui_io(P2P_STATE,IO,CONTINUE_DISPLAY,PAGE_SPEAKER,SET,&io_led[FIVE],IO_ADDRESS,
                                   LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);

                                }
                                */
                                pvt_call.pvt_call_busy=SET;
                                if(g_iHandset_lifted_signal==CLEAR)
                                {
                                    select_codec(PVT_CHANNEL,NONE);
                                    select_codec(PVT_CHANNEL,SPEAKER);
                                }
                                else
                                {
                                    select_codec(PVT_CHANNEL,NONE);
                                    select_codec(PVT_CHANNEL,HANDSET);
                                }
                                pthread_mutex_lock(&lock_call_type);
                                pvt_call.type=P2P;
                                pthread_mutex_unlock(&lock_call_type);
                                printf("P2p call event received In IDLE\n");
                                printf("g_cCaller %s stn id %d \n",g_cCaller,stn_config.logical_id);
                                sprintf(g_cLine3_buf,P2P_msg,g_cCaller);
                                printf("line3buf=%s\n",g_cLine3_buf);
                                /*select_codec(PVT_CHANNEL,NONE);
                                select_codec(PVT_CHANNEL,HANDSET);*/


                                sprintf(g_cLine3_buf,P2P_msg,g_cCaller);
                                send_msg_ui_lcd(P2P_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);

                                g_uiPvt_call_timer=stn_config.pvt_call_timeout*TWO;
                                g_uiPvt_call_timer_ui_lcd = stn_config.pvt_call_timeout*TEN
;
                                g_uiPage_call_timer_frlcd_ini = 0;
                                acgPlayTone_phi(PVT_CHANNEL,VOIP_SIGNAL_STUTTER_TONE,START_TONE,CLEAR);
                                g_uiPlay_tone_timer=THREE;


                                set_volume(PVT_CHANNEL,MAX_PVT_VOLUME);

                                if(strcmp("ano",g_cCaller)==0)
                                {
                                    printf("*************Inside ano ........\n");
                                    //sprintf(g_cLine3_buf,CONF_msg,g_cCaller);
                                   // send_msg_ui(CONFERENCE_STATE,LCD,LINE2,CONTINUE_DISPLAY,CONFERENCE_msg,LCD_CLR_SECOND_LINE,0);
                                  //  send_msg_ui(CONFERENCE_STATE,LED,LED1,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR,0);
                                    g_uiFsu_state=CONFERENCE_STATE;
                                }
                                else
                                {
                                    printf("****In else of ano ........\n");
                                    sprintf(g_cLine3_buf,P2P_msg,g_cCaller);
                                 //   send_msg_ui(P2P_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,LCD_CLR_SECOND_LINE,0);
                                //    send_msg_ui(P2P_STATE,LED,LED1,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR,0);
                                    g_uiFsu_state=P2P_STATE;
                                }

                            break;


                        }
                g_uiFsu_event=CLEAR;
                break;

                    //break;

	case P2P_OFF_HOOK_STATE:
                    switch(g_uiFsu_event)
                    {

                        case WAKE_UP_EVENT:
                            printf("____________p2p_OFF_HOOK_STATE_______WAKE_UP_EVENT___\n");
                            reset_all();
                           // g_uiFsu_state=VCCS_REGISTERING_STATE;

                        break;

                        case BROADCAST_TIMEOUT_EVENT:
                             printf("Inside BROADCAST_TIMEOUT_EVENT\n");
                             Restart_Broadcast_Socket();
                        break;

                        case CALL_OFF_HOOK_EVENT:
                            drop_membership(g_Iicom_fds[pvt_call.curr_icom_no]);
							reset_intercom();
                            printf("CALL_OFF_HOOK_EVENT received");
                            sendCompleteString(g_cCalled_station);
                            g_uiVoipapp_wait_timer = THIRTY;
                            g_uiFsu_state=RINGBACK_STATE;
                        break;

                        case INCOMING_GROUP_CALL_EVENT:
                            send_to_phone_input_cb(1,PHONE_INPUT_HOOK_ON);
                            drop_membership(g_Iicom_fds[pvt_call.curr_icom_no]);
                            // drop_membership(g_iAim_fds[stn_config.default_AIM-1]);
                            //operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_ON);
                            //  if(g_iHandset_lifted_signal!=SET)
                            if(g_iHandset_lifted_signal==CLEAR)
                            {
                              //  operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_OFF);
                               // operate_gpio(MUSIC_VOL_BYPASS,WRITE,GPIO_OFF);
                            }
                           // operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);
                            g_uiVoipapp_wait_timer=CLEAR;
                            pthread_mutex_lock(&lock_call_type);
                            pvt_call.type=GROUP;
                            pthread_mutex_unlock(&lock_call_type);
                            printf("GROUP call event received");

                            sprintf(g_cLine2_buf,group_msg,lcd_group_call_no,lcd_group_call_init);
                           /// send_msg_ui(GROUP_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,LCD_CLR_SECOND_LINE,0);

                            pvt_call.pvt_call_busy=SET;
                            select_codec(PVT_CHANNEL,HANDSET);
                            set_volume(PVT_CHANNEL,MAX_PVT_VOLUME);
                           /// send_msg_ui(GROUP_STATE,LED,LED1,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR,0);
                            g_uiPvt_call_timer=stn_config.pvt_call_timeout*TWO;
                            g_uiPvt_call_timer_ui_lcd = stn_config.pvt_call_timeout*10;
                            g_uiPage_call_timer_frlcd_ini = 0;
                            acgPlayTone_phi(PVT_CHANNEL,VOIP_SIGNAL_STUTTER_TONE,START_TONE,CLEAR);
                            g_uiPlay_tone_timer=THREE;
                            g_uiFsu_state=GROUP_STATE;

                        break;

                        case P2P_RESET_EVENT:
                            if(pg_call.pg_call_busy!=SET)
                            {
                                //operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                            }
                            if(pg_call.pg_call_busy!=SET && stn_config.default_AIM==CLEAR)
							{
                               //	operate_gpio(MUSIC_VOL_BYPASS,WRITE,GPIO_ON);
                            }
                           // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
                            g_uiVoipapp_wait_timer=CLEAR;
                            pthread_mutex_lock(&lock_call_type);
                            pvt_call.type=CLEAR;
                            pthread_mutex_unlock(&lock_call_type);
                            send_to_phone_input_cb(1,PHONE_INPUT_HOOK_ON);
                            //send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,CLEAR);
                            ///send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,callterminated_msg,LCD_CLR_SECOND_LINE,0);

                            g_uiPvt_call_timer=CLEAR;
                            pvt_call.pvt_call_busy=CLEAR;
                            printf("OFF hook state p2p reset_EVENT received");
                            g_uiFsu_state=IDLE_STATE;
                            if(g_iIcom_mem_dropped==SET &&
                            stn_config.icom_live_dead_status[stn_config.default_intercom-MIN_OFFSET]==LIVE)
                            {
                                add_membership(g_Iicom_fds[stn_config.default_intercom-MIN_OFFSET]);
                                reset_intercom();
                                start_intercom();
                            }


                           if(lcd_fcs_default_icom == 0)
                           {
                                sprintf(g_cLine3_buf,IDLE_msg,stn_config.logical_id);
                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,
                                LCD_ADDRESS,IOCTL_PRINT);
                           }
                           else
                           {
                                sprintf(g_cLine3_buf,Channel_msg,stn_config.logical_id,lcd_fcs_default_icom);
                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,
                                LCD_ADDRESS,IOCTL_PRINT);
                           }

                            acgPlayTone_phi(PVT_CHANNEL,VOIP_SIGNAL_STUTTER_TONE,START_TONE,CLEAR);
                            g_uiPlay_tone_timer=THREE;
                        break;

                         case APP_TIMEOUT_EVENT:
                            printf("OFF_HOOK STATE TIMEOUT \n");
                            if(g_cReceived_key_seq[0]==P2P)
                            {
                                send_to_phone_input_cb(ONE,PHONE_INPUT_HOOK_ON);
                            }
                            if(lcd_fcs_default_icom == 0)
                            {
                                sprintf(g_cLine3_buf,IDLE_msg,stn_config.logical_id);
                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                            }
                            else
                            {
                                sprintf(g_cLine3_buf,Channel_msg,stn_config.logical_id,lcd_fcs_default_icom);
                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                            }
                             g_uiFsu_state=IDLE_STATE;
                        break;


                    }
                g_uiFsu_event=CLEAR;
                break;

                case RINGBACK_STATE:
                      printf("************inside RINGBACK_STATE\n");
                    switch(g_uiFsu_event)
                    {

                        case WAKE_UP_EVENT:
                        printf("____________RINGBACK_STATE_______WAKE_UP_EVENT___\n");
                        reset_all();
                        g_uiFsu_state=VCCS_REGISTERING_STATE;

                        break;

                        case BROADCAST_TIMEOUT_EVENT:
                             printf("Inside BROADCAST_TIMEOUT_EVENT\n");
                             Restart_Broadcast_Socket();
                        break;

                        case INCOMING_ZONE_CALL_EVENT:
							g_uiVoipapp_wait_timer=CLEAR;
                            send_msg_ui_io(PAGE_INITIATOR_STATE,IO,CONTINUE_DISPLAY,LED_CALL_THROUGH,SET,&io_led[ONE],IO_ADDRESS,
                            LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                            drop_membership(g_iAim_fds[stn_config.default_AIM-ONE]);
                            drop_membership(g_Iicom_fds[pvt_call.curr_icom_no]);
                            //stop_aim_receive_call();
                            reset_intercom();
                            pg_call.type=CLEAR;
                            g_uiVoipapp_wait_timer=CLEAR;
                            pthread_mutex_lock(&lock_call_type);
                            pvt_call.type=ZONE;
                            pthread_mutex_unlock(&lock_call_type);
                            pWork_m.private_state=PRIVATE_OUTGOING_ONGOING;
                            g_itransmit_fd_index=ZONE_OFFSET+pg_call.zone_no;
                            sprintf(g_cLine3_buf,zone_call_msg,lcd_zone_call_init);
                            send_msg_ui_lcd(PAGE_INITIATOR_STATE,LCD,LINE1,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                            sprintf(g_cMulticast_tx_address,"%s%d",gc_zone_mul_start_addr,g_izone_mul_addr_start_offset+pg_call.zone_no);
                            printf("ZOne multicast tx address %s\n",g_cMulticast_tx_address);
                            add_route_to_system(g_cMulticast_tx_address);
                            printf("INCOMING_ZONE_CALL_EVENT call event received\n");
                            g_uiPage_call_timer=stn_config.pg_call_timeout*2;

                            g_uiPage_call_timer_frlcd_ini = stn_config.pg_call_timeout*10;
                            g_uiPvt_call_timer_ui_lcd = 0;
                            g_uiFsu_state=PAGE_INITIATOR_STATE;
                            pvt_call.pvt_call_busy=SET;
                            multicast_prepare_dsp(PVT_CHANNEL);
                            //select_codec(PG_CHANNEL,NONE);
                            select_codec(PVT_CHANNEL,NONE);
                            select_codec(PVT_CHANNEL,HANDSET);

                        break;

                        case INCOMING_ZONE_ALM_EVENT:
							g_uiVoipapp_wait_timer=CLEAR;
                            //operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_ON);
                            send_msg_ui_io(PAGE_INITIATOR_STATE,IO,CONTINUE_DISPLAY,LED_CALL_THROUGH,SET,&io_led[ONE],IO_ADDRESS,
                            LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                            drop_membership(g_iAim_fds[stn_config.default_AIM-ONE]);
                            drop_membership(g_Iicom_fds[pvt_call.curr_icom_no]);
                            add_membership(g_izone_fds[pg_call.zone_no]);
                            //stop_aim_receive_call();
                            reset_intercom();
                            pg_call.type=CLEAR;
                            g_uiVoipapp_wait_timer=CLEAR;
                            pthread_mutex_lock(&lock_call_type);
                            pg_call.type=ZONE_ALARM;
                            pthread_mutex_unlock(&lock_call_type);
                           // pWork_m.private_state=PRIVATE_OUTGOING_ONGOING;
                           // g_itransmit_fd_index=ZONE_OFFSET+pg_call.zone_no;
                            sprintf(g_cLine3_buf,zone_alm_msg,lcd_alm_zone_no);
                            send_msg_ui_lcd(PAGE_INITIATOR_STATE,LCD,LINE1,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);

                          //  sprintf(g_cMulticast_tx_address,"%s%d",gc_zone_mul_start_addr,g_izone_mul_addr_start_offset+pg_call.zone_no);
                          //  printf("ZOne multicast tx address %s\n",g_cMulticast_tx_address);
                          //  add_route_to_system(g_cMulticast_tx_address);
                            printf("INCOMING_ZONE_ALM_CALL_EVENT call event received\n");
                            //g_uiPage_call_timer=stn_config.pg_call_timeout*2;
                            g_uiPage_call_timer=stn_config.alarm_call_timeout*2;
                            g_uiPage_call_timer_lcd =stn_config.alarm_call_timeout*10;
                            printf("********g_uiPage_call_timer in INCOMING_ZONE_ALM_EVENT %d \n",g_uiPage_call_timer);
                            //****added***//
                            //g_uiPage_call_timer_for_initiator = stn_config.pg_call_timeout*2;
                            g_uiFsu_state=PAGE_INITIATOR_STATE;
                            pg_call.pg_call_busy = SET;
                            multicast_prepare_dsp(PG_CHANNEL);
                            select_codec(PG_CHANNEL,NONE);
                           // select_codec(PG_CHANNEL,NONE);
                            select_codec(PG_CHANNEL,SPEAKER);
                           // stop_alarm(pg_call.alm_no);
                            //start_alarm(pg_call.alm_no);

                        break;


                        case P2P_CALL_EVENT:
                            printf("*****inside P2P_CALL_EVENT of ringback state\n");
                           // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_ON);
                            send_msg_ui_io(P2P_STATE,IO,CONTINUE_DISPLAY,LED_CALL_THROUGH,SET,&io_led[ONE],IO_ADDRESS,
                            LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                          //  stop_aim_receive_call();
                            g_uiVoipapp_wait_timer=CLEAR;
                            //  if(g_iHandset_lifted_signal!=SET)
                            usleep(100000);
                            send_msg_ui_io(P2P_STATE,IO,CONTINUE_DISPLAY,PAGE_SPEAKER,SET,&io_led[FIVE],IO_ADDRESS,
                            LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                            pthread_mutex_lock(&lock_call_type);
                            pvt_call.type=P2P;
                            pthread_mutex_unlock(&lock_call_type);
                            printf("P2p call event received\n");
                            //sprintf(g_cLine3_buf,"P2P:%s %2d",g_cCaller,stn_config.logical_id);
                            sprintf(g_cLine3_buf,P2P_msg,g_cCaller);
                            send_msg_ui_lcd(P2P_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                            pvt_call.pvt_call_busy=SET;
                            select_codec(PVT_CHANNEL,NONE);
                            select_codec(PVT_CHANNEL,HANDSET);
                            set_volume(PVT_CHANNEL,MAX_PVT_VOLUME);
                           // send_msg_ui(P2P_STATE,LED,LED1,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR,0);
                            g_uiPvt_call_timer=stn_config.pvt_call_timeout*TWO;
                            g_uiPvt_call_timer_ui_lcd = stn_config.pvt_call_timeout*10;
                            g_uiPage_call_timer_frlcd_ini = 0;
                            printf("g_uiPvt_call_timer %d\n",stn_config.pvt_call_timeout);
                            //send_msg_ui(P2P_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_uiPvt_call_timer,LCD_CLR_SECOND_LINE);
                            acgPlayTone_phi(PVT_CHANNEL,VOIP_SIGNAL_STUTTER_TONE,START_TONE,CLEAR);
                            g_uiPlay_tone_timer=THREE;
                            g_uiFsu_state=P2P_STATE;
                        break;

                        case VALID_KEY_SEQ_EVENT:
                            send_to_phone_input_cb(ONE,PHONE_INPUT_HOOK_ON);
                            g_uiFsu_state=IDLE_STATE;
                        break;

                        case INCOMING_GROUP_CALL_EVENT:
                             printf("INSIDE INCOMING_GROUP_CALL_EVENT of RINGBACK state\n");
                            send_to_phone_input_cb(ONE,PHONE_INPUT_HOOK_ON);
                           // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_ON);
                            send_msg_ui_io(P2P_STATE,IO,CONTINUE_DISPLAY,LED_CALL_THROUGH,SET,&io_led[ONE],IO_ADDRESS,
                            LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                            //drop_membership(g_iAim_fds[stn_config.default_AIM-1]);
                            drop_membership(g_Iicom_fds[pvt_call.curr_icom_no]);
                            add_membership(g_igroup_fds[pvt_call.group_no]);
                            g_uiVoipapp_wait_timer=CLEAR;
                            pthread_mutex_lock(&lock_call_type);
                            pvt_call.type=GROUP;
                            pthread_mutex_unlock(&lock_call_type);
                            g_itransmit_fd_index=GROUP_OFFSET+pvt_call.group_no;
                            sprintf(g_cMulticast_tx_address,"%s%d",gc_group_mul_start_addr,g_igroup_mul_addr_start_offset+pvt_call.group_no);
                            add_route_to_system(g_cMulticast_tx_address);
                            printf("GROUP call event received address is %s \n",g_cMulticast_tx_address);
                            sprintf(g_cLine2_buf,GRP_CL_msg,lcd_group_call_no,lcd_group_call_init);
                            //pWork_m.private_state=PRIVATE_OUTGOING_ONGOING;
                            ///send_msg_ui(GROUP_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,LCD_CLR_SECOND_LINE,0);
                            pvt_call.pvt_call_busy=SET;
                            select_codec(PVT_CHANNEL,HANDSET);
                            set_volume(PVT_CHANNEL,MAX_PVT_VOLUME);
                            ///send_msg_ui(GROUP_STATE,LED,LED1,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR,0);
                            g_uiPvt_call_timer=stn_config.pvt_call_timeout*TWO;
                            g_uiPvt_call_timer_ui_lcd = stn_config.pvt_call_timeout*10;
                            g_uiPage_call_timer_frlcd_ini =0;
                            acgPlayTone_phi(PVT_CHANNEL,VOIP_SIGNAL_STUTTER_TONE,START_TONE,CLEAR);
                            g_uiPlay_tone_timer=THREE;
                            g_uiFsu_state=GROUP_STATE;
                           // g_uiSpeech_detection_timer = SPEECH_DETECTION_TIMEOUT;

                        break;



                        case PTT_RESPONSE_EVENT:
                            g_uiVoipapp_wait_timer=CLEAR;
                            //operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);
                            sprintf(g_cMulticast_tx_address,"%s%d",gc_icom_mul_start_addr,g_iIcom_mul_addr_start_offset+
                            pvt_call.curr_icom_no);
                            add_route_to_system(g_cMulticast_tx_address);
                            printf("multicast address mul ofset %s  %d\n",g_cMulticast_tx_address,
                            g_iIcom_mul_addr_start_offset+pvt_call.curr_icom_no);

                            //sprintf(g_cLine2_buf,intercom_msg,lcd_icom_no);
                            ///send_msg_ui(INTERCOM_STATE,LCD,LINE3,CONTINUE_DISPLAY,g_cLine2_buf,LCD_CLR_SECOND_LINE,0);
                            pthread_mutex_lock(&lock_call_type);
                            pvt_call.type=INTERCOM_TX_RX;
                            g_itransmit_fd_index = ICOM_OFFSET+pvt_call.curr_icom_no;
                            // pvt_call.pvt_call_busy=SET;

                            pthread_mutex_unlock(&lock_call_type);
                            pWork_m.private_state=PRIVATE_OUTGOING_ONGOING;

                            g_uiFsu_state=INTERCOM_STATE;
                            g_uiSpeech_detection_timer = SPEECH_DETECTION_TIMEOUT;


                        break;

                        case P2P_RESET_EVENT:
                            g_uiVoipapp_wait_timer=CLEAR;
                           // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
                            pthread_mutex_lock(&lock_call_type);
                            pvt_call.type=CLEAR;
                            pthread_mutex_unlock(&lock_call_type);
                            if(pg_call.pg_call_busy!=SET)
                            {
                                send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,LED_CALL_THROUGH,CLEAR,&io_led_clear[ZERO],
                                IO_ADDRESS,LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                            }
                            send_to_phone_input_cb(ONE,PHONE_INPUT_HOOK_ON);


                            // send_msg_ui(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,lcd_fcs_desc,LCD_CLR_FIRST_LINE);
                            if(lcd_fcs_default_icom == 0)
                            {
                                sprintf(g_cLine3_buf,IDLE_msg,stn_config.logical_id);
                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                            }
                            else
                            {
                                sprintf(g_cLine3_buf,Channel_msg,stn_config.logical_id,lcd_fcs_default_icom);
                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                            }
                            g_uiPvt_call_timer=CLEAR;
                            pvt_call.pvt_call_busy=CLEAR;
                            printf("RINGBACK_STATE reset_EVENT received");
                            g_uiFsu_state=IDLE_STATE;
                            if(g_iIcom_mem_dropped==SET &&stn_config.icom_live_dead_status[stn_config.default_intercom-MIN_OFFSET]==LIVE)
                            {
                                add_membership(g_Iicom_fds[stn_config.default_intercom-MIN_OFFSET]);
                                reset_intercom();
                                start_intercom();
                            }
                            if(pg_call.pg_call_busy!=SET && stn_config.default_AIM==CLEAR)
							{
								//operate_gpio(MUSIC_VOL_BYPASS,WRITE,GPIO_ON);
							}

                            if(g_uiPlay_tone_timer==CLEAR)
                            {

                                acgPlayTone_phi(PVT_CHANNEL,VOIP_SIGNAL_BUSY_TONE,START_TONE,1);
                                g_uiPlay_tone_timer=SIX;
                            }
                            // start_aim_receive_call();
                        break;

                        case NEG_RES_EVENT:
                            printf("NEGATIVE RESPONSE EVENT Generated\n");
                            g_uiVoipapp_wait_timer=CLEAR;
                            g_uiFsu_state=IDLE_STATE;
                             if(g_iIcom_mem_dropped==SET &&stn_config.icom_live_dead_status[stn_config.default_intercom-MIN_OFFSET]==LIVE)
                            {
                                add_membership(g_Iicom_fds[stn_config.default_intercom-MIN_OFFSET]);
                                reset_intercom();
                                start_intercom();
                            }
                            if(pg_call.pg_call_busy!=SET && stn_config.default_AIM==CLEAR)
							{
								//operate_gpio(MUSIC_VOL_BYPASS,WRITE,GPIO_ON);
							}


                            if(g_uiPlay_tone_timer==CLEAR)
                            {

                                printf("................PLAYING BUSY TONE ...............\n");
                                acgPlayTone_phi(PVT_CHANNEL,VOIP_SIGNAL_BUSY_TONE,START_TONE,1);
                                g_uiPlay_tone_timer=SIX;
                            }

                            if((g_cReceived_key_seq[0] == GLOBAL) || (g_cReceived_key_seq[0] == GLOBAL_ALARM) || (g_cReceived_key_seq[0] == ZONE) || (g_cReceived_key_seq[0] == ZONE_ALARM))
                            {
                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY," negative resp ",ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                sleep(1);

                                if(lcd_fcs_default_icom == 0)
                                {
                                    sprintf(g_cLine3_buf,IDLE_msg,stn_config.logical_id);
                                    send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                }
                                else
                                {
                                    sprintf(g_cLine3_buf,Channel_msg,stn_config.logical_id,lcd_fcs_default_icom);
                                    send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                }

                            }

                            else
                            {
                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY," negative resp ",ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                 sleep(1);

                                if(lcd_fcs_default_icom == 0)
                                {
                                    sprintf(g_cLine3_buf,IDLE_msg,stn_config.logical_id);
                                    send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                }
                                else
                                {
                                    sprintf(g_cLine3_buf,Channel_msg,stn_config.logical_id,lcd_fcs_default_icom);
                                    send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                }
                            }

                           // g_uiFsu_state=IDLE_STATE;

                        break;

                        case GLOBAL_CALL_EVENT:
                            g_uiVoipapp_wait_timer=CLEAR;
                            send_msg_ui_io(PAGE_INITIATOR_STATE,IO,CONTINUE_DISPLAY,LED_CALL_THROUGH,SET,&io_led[ONE],
                            IO_ADDRESS,LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                            usleep(100000);
                            send_msg_ui_io(PAGE_INITIATOR_STATE,IO,CONTINUE_DISPLAY,PAGE_SPEAKER,SET,&io_led[FIVE],
                            IO_ADDRESS,LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                            drop_membership(g_iAim_fds[stn_config.default_AIM-ONE]);
                            drop_membership(g_Iicom_fds[pvt_call.curr_icom_no]);
                            //stop_aim_receive_call();
                            reset_intercom();
                            g_uiVoipapp_wait_timer=CLEAR;
                            pthread_mutex_lock(&lock_call_type);
                            pvt_call.type=GLOBAL;
                            pthread_mutex_unlock(&lock_call_type);
                            pWork_m.private_state=PRIVATE_OUTGOING_ONGOING;
                            g_itransmit_fd_index=GLOBAL_OFFSET;
                            //send_msg_ui(PAGE_INITIATOR_STATE,LCD,LINE1,CONTINUE_DISPLAY,global_call_msg,LCD_CLR_FIRST_LINE);
                            sprintf(g_cLine3_buf,global_call_msg,lcd_global_call_init);
                            send_msg_ui_lcd(PAGE_INITIATOR_STATE,LCD,LINE1,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                            sprintf(g_cMulticast_tx_address,"%s%d",gc_global_mul_start_addr,g_iglobal_mul_addr_start_offset);
                            add_route_to_system(g_cMulticast_tx_address);
                            printf("GLOBAL_CALL_EVENT call event received \n");
                            g_uiPage_call_timer=stn_config.pg_call_timeout*TWO;
                            //****added****//
                            g_uiPage_call_timer_frlcd_ini = stn_config.pg_call_timeout*10;
                            g_uiPvt_call_timer_ui_lcd = 0;
                            g_uiFsu_state=PAGE_INITIATOR_STATE;
                            pvt_call.pvt_call_busy = SET;
                            multicast_prepare_dsp(PVT_CHANNEL);
                            //select_codec(PG_CHANNEL,NONE);
                            select_codec(PVT_CHANNEL,NONE);
                            select_codec(PVT_CHANNEL,HANDSET);

                        break;

                        case GLOBAL_ALM_EVENT:
                            g_uiVoipapp_wait_timer=CLEAR;
                            //operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_ON);
                            send_msg_ui_io(PAGE_INITIATOR_STATE,IO,CONTINUE_DISPLAY,LED_CALL_THROUGH,SET,&io_led[ONE],
                            IO_ADDRESS,LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                            usleep(100000);
                            send_msg_ui_io(PAGE_INITIATOR_STATE,IO,CONTINUE_DISPLAY,PAGE_SPEAKER,SET,&io_led[FIVE],
                            IO_ADDRESS,LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                            drop_membership(g_iAim_fds[stn_config.default_AIM-ONE]);
                            drop_membership(g_Iicom_fds[pvt_call.curr_icom_no]);

                            //stop_aim_receive_call();
							reset_intercom();
                            g_uiVoipapp_wait_timer=CLEAR;
                            pg_call.type=GLOBAL_ALARM;

                            sprintf(g_cLine3_buf,global_alm_msg,lcd_global_alm_no);
                            send_msg_ui_lcd(PAGE_INITIATOR_STATE,LCD,LINE1,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                            printf("GLOABAL_ALARM_CALL_EVENT call event received\n");

                           // g_uiPage_call_timer=stn_config.pg_call_timeout*TWO;
                            g_uiPage_call_timer = stn_config.alarm_call_timeout*TWO;
                            g_uiPage_call_timer_lcd = stn_config.alarm_call_timeout*TEN;
                             printf("********g_uiPage_call_timer in GLOBAL_ALM_EVENT %d \n",g_uiPage_call_timer);
                            //****added***//
                            //g_uiPage_call_timer_for_initiator = stn_config.pg_call_timeout*2;
                            g_uiFsu_state=PAGE_INITIATOR_STATE;
                            pg_call.pg_call_busy = SET;
                            ///PG_CHANNEL replace by PVT_CHANNEL
                            multicast_prepare_dsp(PG_CHANNEL);

                            select_codec(PG_CHANNEL,NONE);
                            select_codec(PG_CHANNEL,SPEAKER);
                           // stop_alarm(pg_call.alm_no);
                           // start_alarm(pg_call.alm_no);

                        break;



                        case GLOBAL_RESET_EVENT:
                            printf("INSIDE GLOBAL_RESET_EVENT\n");
                            g_uiVoipapp_wait_timer=CLEAR;
                            pg_call.alm_no=CLEAR;
                            send_msg_ui_io(P2P_STATE,IO,CONTINUE_DISPLAY,LED_CALL_THROUGH,CLEAR,&io_led_clear[ZERO],
                            IO_ADDRESS,LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);

                            send_msg_ui_lcd(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,global_terminate,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                            sleep(0.5);
                            sprintf(g_cLine4_buf,"%s          ",lcd_fcs_desc);
                            send_msg_ui_lcd(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,g_cLine4_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                            pWork_m.page_state=PAGE_IDLE;
                            pg_call.type=CLEAR;
                            pg_call.pg_call_busy = CLEAR;
                            g_uiPage_call_timer=CLEAR;

                            if(pg_call.pg_call_busy!=SET && stn_config.default_AIM==CLEAR)
							{
                               //	operate_gpio(MUSIC_VOL_BYPASS,WRITE,GPIO_ON);
                            }
                            pvt_call.curr_icom_no=stn_config.default_intercom-MIN_OFFSET;
                            if(g_iIcom_mem_dropped==SET &&stn_config.icom_live_dead_status[stn_config.default_intercom-MIN_OFFSET]==LIVE)
                            {
                                add_membership(g_Iicom_fds[stn_config.default_intercom-MIN_OFFSET]);
                            }
                            if(stn_config.default_AIM)
                            {
                                add_membership(g_iAim_fds[stn_config.default_AIM-ONE]);
                                start_aim_receive_call();
                            }
                            //  start_intercom();

                        break;

                        case APP_TIMEOUT_EVENT:
                            printf("RINGBACK STATE TIMEOUT \n");
                            if(g_cReceived_key_seq[0]==P2P)
                            {
                                send_to_phone_input_cb(ONE,PHONE_INPUT_HOOK_ON);
                            }
                          ///  send_msg_ui(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,lcd_fcs_desc,LCD_CLR_FIRST_LINE,0);
                            if(lcd_fcs_default_icom == 0)
                            {
                                sprintf(g_cLine3_buf,IDLE_msg,stn_config.logical_id);
                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                            }
                            else
                            {
                                sprintf(g_cLine3_buf,Channel_msg,stn_config.logical_id,lcd_fcs_default_icom);
                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                            }

                            g_uiFsu_state=IDLE_STATE;
                        break;
                    }
                g_uiFsu_event = CLEAR;
                break;

		case P2P_STATE:
                switch(g_uiFsu_event)
                {


                    case WAKE_UP_EVENT:
                         printf("____________P2P_STATE______WAKE_UP_EVENT___\n");
                         reset_all();
                         g_uiVoipapp_wait_timer = CLEAR;
                         g_uiFsu_state=VCCS_REGISTERING_STATE;
                    break;

                    case BROADCAST_TIMEOUT_EVENT:
                         printf("Inside BROADCAST_TIMEOUT_EVENT\n");
                         Restart_Broadcast_Socket();
                    break;


                    case VALID_KEY_SEQ_EVENT:
                         printf("%c valid key in the p2p state \n",g_cReceived_key_seq[0]);
                         switch(g_cReceived_key_seq[0])
                         {
                             case CONFERENCE:
                                 printf("CONFERENCE REQ FRAME SENT \n");
                                 bzero(g_cCalled_station, sizeof(g_cCalled_station));
                                 g_cCalled_station[0]=g_cReceived_key_seq[1];
                                 g_cCalled_station[1]=g_cReceived_key_seq[2];
                                 g_cCalled_station[2]=g_cReceived_key_seq[3];
                                 send_req_msg_to_server(0,CONF_CALL_REQ,CONF_CALL_FRAME_LENGTH,'0','0');
                                 g_uiVoipapp_wait_timer=SERVER_RESPONSE_TIMEOUT;
                               //   g_uiFsu_state=CONF_INIT_STATE;

                            break;

                            case RESET:
                                 printf("Reset key is pressed in P2P_STATE....\n");
                               /// send_msg_ui(P2P_RESET_RINGBACK_STATE,LCD,LINE2,CONTINUE_DISPLAY,reset_msg,LCD_CLR_SECOND_LINE,0);

                                send_msg_ui_lcd(P2P_RESET_RINGBACK_STATE,LCD,LINE2,CONTINUE_DISPLAY,"   Resetting    ",ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                sleep(1);
                                send_to_phone_input_cb(1,PHONE_INPUT_HOOK_ON);
                                g_uiFsu_state = P2P_RESET_RINGBACK_STATE;
                                g_uiVoipapp_wait_timer = SERVER_RESPONSE_TIMEOUT;


                            break;
                        }

                    break;

                    case CONF_ADD_EVENT:
                         g_uiVoipapp_wait_timer = CLEAR;
                        printf("p2p reset IN P2P state event changing state to IDLE\n");
                        ///operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
                        if(pg_call.pg_call_busy!=SET)
                        {
                           /// operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                        }
                        if(pg_call.pg_call_busy==CLEAR)
                        {
                           /// operate_gpio(MUSIC_VOL_BYPASS,WRITE,GPIO_OFF);
                        }
                        pthread_mutex_lock(&lock_call_type);
                        pvt_call.type=CLEAR;
                        pthread_mutex_unlock(&lock_call_type);
                        // send_to_phone_input_cb(1,PHONE_INPUT_HOOK_ON);
                        if(pg_call.pg_call_busy!=SET && stn_config.default_AIM==CLEAR)
                        {
                           /// operate_gpio(MUSIC_VOL_BYPASS,WRITE,GPIO_ON);
                        }

                        g_uiPvt_call_timer=CLEAR;
//                        select_codec(PVT_CHANNEL,NONE);
                        close_channel(PVT_CHANNEL);
                        pvt_call.pvt_call_busy=CLEAR;
                        g_uiFsu_state=CONF_INIT_STATE;
                        g_uiVoipapp_wait_timer = SERVER_RESPONSE_TIMEOUT;
                        sprintf(g_cLine3_buf,CONF_msg,g_cCalled_station);
                        ///send_msg_ui(CONF_INIT_STATE,LCD,LINE3,CONTINUE_DISPLAY,g_cLine3_buf,LCD_CLR_FIRST_10_POSITION,0);

                    break;



                    case PVT_CALL_TIMEOUT:
                        printf("PVT_CALL_TIMEOUT in P2P state \n");
                        g_uiVoipapp_wait_timer = CLEAR;
                      //  operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                        send_msg_ui_io(P2P_STATE,IO,CONTINUE_DISPLAY,LED_CALL_THROUGH,CLEAR,&io_led_clear[ZERO],
                        IO_ADDRESS,LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                        pthread_mutex_lock(&lock_call_type);
                        pvt_call.type=CLEAR;
                        pthread_mutex_unlock(&lock_call_type);
                        send_to_phone_input_cb(ONE,PHONE_INPUT_HOOK_ON);
                        printf("PVT_CALL_TIMEOUT changing state to IDLE\n");
                        g_uiPvt_call_timer=CLEAR;
                        pvt_call.pvt_call_busy=CLEAR;
                        acgPlayTone_phi(PVT_CHANNEL,VOIP_SIGNAL_STUTTER_TONE,START_TONE,CLEAR);
                        g_uiPlay_tone_timer=THREE;
                        g_uiFsu_state=IDLE_STATE;
                        if(pg_call.pg_call_busy!=SET && stn_config.default_AIM==CLEAR)
                        {
                          	//send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,&io_led[FIVE],IO_ADDRESS,LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                        }
                        if(g_iIcom_mem_dropped==SET)
                        {
                            add_membership(g_Iicom_fds[stn_config.default_intercom-MIN_OFFSET]);
                            reset_intercom();
                            start_intercom();
                            if(g_iHandset_lifted_signal==CLEAR)
                            {
                                printf("//////////ICOM_MIX_ACt is off after icom membership add in call timeout.......\n");
                                //operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_OFF);
                            }
                        }

                         send_msg_ui_lcd(INIT_STATE,LCD,LINE2,CONTINUE_DISPLAY,callterminated_msg,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                         usleep(100000);
                        if(lcd_fcs_default_icom == 0)
                        {
                            sprintf(g_cLine3_buf,IDLE_msg,stn_config.logical_id);
                            send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                        }
                        else
                        {
                            sprintf(g_cLine3_buf,Channel_msg,stn_config.logical_id,lcd_fcs_default_icom);
                            send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                        }
                    break;

                     case P2P_RESET_EVENT:
                            printf("inside  P2P_STATE event is P2P_RESET_EVENT \n");
                            g_uiVoipapp_wait_timer = CLEAR;
                            printf("p2p reset IN P2P state event changing state to IDLE\n");



                            if(pg_call.pg_call_busy!=SET)
                            {
                                send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,LED_CALL_THROUGH,CLEAR,&io_led_clear[ZERO],
                                IO_ADDRESS,LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                            }
                            usleep(100000);
                            if(pg_call.pg_call_busy==CLEAR)
                            {
                                 printf("....................MUSIC_VOl_BYPASS is OFF when no page call........\n");
                               send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,PAGE_SPEAKER,CLEAR,&io_led_clear[ZERO],
                               IO_ADDRESS,LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                            }
                            pthread_mutex_lock(&lock_call_type);
                            pvt_call.type=CLEAR;
                            pthread_mutex_unlock(&lock_call_type);
                            // send_to_phone_input_cb(1,PHONE_INPUT_HOOK_ON);
                            if(pg_call.pg_call_busy!=SET && stn_config.default_AIM==CLEAR)
                            {
                                printf("MUSIC_VOl_BYPASS is ON after P2p reset........\n");
                               /// operate_gpio(MUSIC_VOL_BYPASS,WRITE,GPIO_ON);
                            }
                            if(g_iIcom_mem_dropped==SET)
                            {
                                add_membership(g_Iicom_fds[stn_config.default_intercom-MIN_OFFSET]);
                                reset_intercom();
                                start_intercom();
                            }

                            g_uiPvt_call_timer=CLEAR;
                            pvt_call.pvt_call_busy=CLEAR;
                            g_uiFsu_state=IDLE_STATE;

                            send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,callterminated_msg,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                            usleep(100000);
                           if(lcd_fcs_default_icom == 0)
                            {
                                sprintf(g_cLine3_buf,IDLE_msg,stn_config.logical_id);
                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                            }
                            else
                            {
                                sprintf(g_cLine3_buf,Channel_msg,stn_config.logical_id,lcd_fcs_default_icom);
                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                            }
                            acgPlayTone_phi(PVT_CHANNEL,VOIP_SIGNAL_STUTTER_TONE,START_TONE,CLEAR);
                            g_uiPlay_tone_timer=THREE;

                            if(g_iHandset_lifted_signal==CLEAR)
                            {
                               printf("//////////ICOM_MIX_ACt is off after icom membership add.......\n");
                             //  operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_OFF);

                            }
                            else
                            {
                               //  operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
                            }
                        break;

                    case APP_TIMEOUT_EVENT:
                        printf("APP timeout event received in P2P state\n");
                        g_uiVoipapp_wait_timer = CLEAR;
                        bzero(g_cCalled_station,sizeof(g_cCalled_station));

                        if(lcd_fcs_default_icom == 0)
                        {
                            sprintf(g_cLine3_buf,IDLE_msg,stn_config.logical_id);
                            send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                        }
                        else
                        {
                            sprintf(g_cLine3_buf,Channel_msg,stn_config.logical_id,lcd_fcs_default_icom);
                            send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                        }
                        ///send_msg_ui(P2P_STATE,LCD,LINE2,CONTINUE_DISPLAY,CONFERENCE_msg,LCD_CLR_FIRST_10_POSITION,0);
                    break;

                    case NEG_RES_EVENT:
                         printf("APP timeout event received in P2P state\n");
                         g_uiVoipapp_wait_timer = CLEAR;
                         bzero(g_cCalled_station,sizeof(g_cCalled_station));
                        /// send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,negative_response_msg,LCD_CLR_SECOND_LINE,0);
                          if(g_uiPlay_tone_timer==CLEAR)
                          {
                               printf("................PLAYING BUSY TONE ...............\n");
                               acgPlayTone_phi(PVT_CHANNEL,VOIP_SIGNAL_BUSY_TONE,START_TONE,1);
                               g_uiPlay_tone_timer=SIX;
                          }

                            if(lcd_fcs_default_icom == 0)
                            {
                                sprintf(g_cLine3_buf,IDLE_msg,stn_config.logical_id);
                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                            }
                            else
                            {
                                sprintf(g_cLine3_buf,Channel_msg,stn_config.logical_id,lcd_fcs_default_icom);
                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                            }
                    break;
                }
                g_uiFsu_event=CLEAR;
                break;

                case P2P_RESET_RINGBACK_STATE:

                    switch(g_uiFsu_event)
                    {
                        case WAKE_UP_EVENT:
                            printf("____________P2P_RESET_RINGBACK_STATE_______WAKE_UP_EVENT___\n");
                            reset_all();
                            g_uiFsu_state=VCCS_REGISTERING_STATE;
                        break;

                        case BROADCAST_TIMEOUT_EVENT:
                             printf("Inside BROADCAST_TIMEOUT_EVENT\n");
                             Restart_Broadcast_Socket();
                        break;

                        case P2P_RESET_EVENT:
                        case APP_TIMEOUT_EVENT:
                             printf("inside  P2P_RESET_RINGBACK_STATE event is P2P_RESET_EVENT \n");
                             g_uiVoipapp_wait_timer = CLEAR;
                             printf("p2p reset IN P2P state event changing state to IDLE\n");
                             send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,LED_CALL_THROUGH,CLEAR,&io_led_clear[ZERO],
                             IO_ADDRESS,LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                             if(pg_call.pg_call_busy!=SET)
                             {
                                printf("..........pg_call.pg_call_busy!=SET\n...................");
                                send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,PAGE_SPEAKER,CLEAR,&io_led_clear[ZERO],
                                IO_ADDRESS,LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                             }

                            pthread_mutex_lock(&lock_call_type);
                            pvt_call.type=CLEAR;
                            pthread_mutex_unlock(&lock_call_type);
                            //send_to_phone_input_cb(1,PHONE_INPUT_HOOK_ON);


                            if(g_iIcom_mem_dropped==SET)
                            {
                                add_membership(g_Iicom_fds[stn_config.default_intercom-MIN_OFFSET]);
                                reset_intercom();
                                start_intercom();
                            }

                            if(pg_call.pg_call_busy!=SET && stn_config.default_AIM==CLEAR)
                            {
                                printf("MUSIC_VOl_BYPASS is ON after P2p reset........\n");
                               // send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,&io_led[FIVE],IO_ADDRESS,LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                            }
                            g_uiPvt_call_timer=CLEAR;
                            pvt_call.pvt_call_busy=CLEAR;
                            g_uiFsu_state=IDLE_STATE;

                            send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,callterminated_msg,ZERO,LCD_ADDRESS,IOCTL_PRINT);

                            if(lcd_fcs_default_icom == 0)
                            {
                                sprintf(g_cLine3_buf,IDLE_msg,stn_config.logical_id);
                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                            }
                            else
                            {
                                sprintf(g_cLine3_buf,Channel_msg,stn_config.logical_id,lcd_fcs_default_icom);
                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                            }
                            //sleep(0.5);
                            acgPlayTone_phi(PVT_CHANNEL,VOIP_SIGNAL_STUTTER_TONE,START_TONE,CLEAR);
                            g_uiPlay_tone_timer=THREE;

                            if(g_iHandset_lifted_signal==CLEAR)
                            {
                                printf("//////////ICOM_MIX_ACt is off after icom membership add.......\n");
                               // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_OFF);
                            }
                            else
                            {
                             //   operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
                            }
                        break;

                        default:
                        break;
                }

                g_uiFsu_event=CLEAR;

                break;
                case PAGE_INITIATOR_STATE:
                         printf("************INSIDE PAGE_INITIATOR_STATE\ng_uiFsu_event is ...%d\n", g_uiFsu_event);

                    switch(g_uiFsu_event)
                    {
                        case WAKE_UP_EVENT:
                             printf("____________PAGE_INITIATOR_STATE______WAKE_UP_EVENT___\n");
                             reset_all();
                             g_uiFsu_state=VCCS_REGISTERING_STATE;
                        break;

                        case BROADCAST_TIMEOUT_EVENT:
                             printf("Inside BROADCAST_TIMEOUT_EVENT\n");
                             Restart_Broadcast_Socket();
                        break;


                        case VALID_KEY_SEQ_EVENT:
                            sprintf(uc_buf,"%c%c",g_cReceived_key_seq[1],g_cReceived_key_seq[2]);
                            ui_pressed_key=atoi(uc_buf);
                            printf("pressed key in page_initiator_state =%d\n",ui_pressed_key);

                            switch(ui_pressed_key)
                            {
                                case RESET_KEY:

                                    if(pvt_call.type==GLOBAL || pg_call.type==GLOBAL_ALARM)
                                    {
                                        send_req_msg_to_server(0,GLOBAL_RESET,GLOBAL_RESET_FRAME_LENGTH,'0','0');

                                    }
                                    else if(pvt_call.type=ZONE || pg_call.type == ZONE_ALARM)
                                    {
                                        send_req_msg_to_server(0,ZONE_RESET_REQ,GLOBAL_RESET_FRAME_LENGTH,'0','0');
                                    }
                                    g_uiVoipapp_wait_timer = SERVER_RESPONSE_TIMEOUT;
                                    g_uiFsu_state = PAGE_RESET_RINGBACK_STATE;
                                    send_msg_ui_lcd(PAGE_RESET_RINGBACK_STATE,LCD,LINE1,CONTINUE_DISPLAY,"   Resetting    ",ZERO,
                                    LCD_ADDRESS,IOCTL_PRINT);
                                    sleep(1);
                                break;

                                default:
                                break;
                          }

                        break;


                        case GLOBAL_ALM_EVENT:
                            close_channel(PG_CHANNEL);
                            select_codec(PG_CHANNEL,NONE);
                            g_itransmit_fd_index=-1;
                            pthread_mutex_lock(&lock_call_type);
                            pg_call.type=GLOBAL_ALARM;
                            pthread_mutex_unlock(&lock_call_type);
                            ///operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_ON);
                            send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,LED_CALL_THROUGH,SET,&io_led[ONE],
                            IO_ADDRESS,LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                            stop_aim_receive_call();
                            ///operate_gpio(MUSIC_VOL_BYPASS,WRITE,GPIO_OFF);
                            printf("GLOABAL_ALM_EVENT call event received");
                            sprintf(g_cLine3_buf,global_alm_msg,lcd_global_alm_init);
                            send_msg_ui_lcd(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                           // send_msg_ui(IDLE_STATE,LCD,LINE3,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR);
                            g_uiPage_call_timer = stn_config.pg_call_timeout*TWO;
                            g_uiPage_call_timer_lcd = stn_config.alarm_call_timeout*TEN;
                           // g_uiPage_call_timer_for_initiator = stn_config.pg_call_timeout*2;
                            pWork_m.page_state=PAGE_INCOMING_ONGOING;
                            pg_call.pg_call_busy = CLEAR;
                            g_uiPage_call_timer=CLEAR;
                            select_codec(PG_CHANNEL,NONE);
                            multicast_prepare_dsp(PG_CHANNEL);
                            select_codec(PG_CHANNEL,SPEAKER);
                            stop_alarm(pg_call.alm_no);
                            start_alarm(pg_call.alm_no);
                            g_uiFsu_state=IDLE_STATE;

                        break;

                          case GLOBAL_RESET_EVENT:
                                g_uiVoipapp_wait_timer = CLEAR;
                                //printf("Global reset event received [%s %d]\n",__FILE__,__LINE__);
                                printf("inside  PAGE_INITIATOR_STATE event is GLOBAL_RESET_EVENT \n");
                                send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,LED_CALL_THROUGH,CLEAR,&io_led_clear[ZERO],
                                IO_ADDRESS,LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                                pg_call.alm_no=CLEAR;
                                pvt_call.type=CLEAR;
                                pg_call.type=CLEAR;
                                close_channel(PG_CHANNEL);
                                pg_call.pg_call_busy = CLEAR;
                                g_uiPage_call_timer=CLEAR;
                                pvt_call.pvt_call_busy = CLEAR;
                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,"   Resetting    ",ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,global_terminate,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                sleep(0.5);
                                sprintf(g_cLine4_buf,"%s          ",lcd_fcs_desc);
                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,g_cLine4_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                pWork_m.private_state=PRIVATE_IDLE;
                                g_itransmit_fd_index=-1;
                                g_uiFsu_state=IDLE_STATE;

                                pvt_call.curr_icom_no=stn_config.default_intercom-MIN_OFFSET;
                                if(stn_config.icom_live_dead_status[stn_config.default_intercom-MIN_OFFSET]==LIVE)
                                {
                                    add_membership(g_Iicom_fds[stn_config.default_intercom-MIN_OFFSET]);
                                    start_intercom();
                                }

                                if(stn_config.default_AIM)
                                {
                                    ///operate_gpio(MUSIC_VOL_BYPASS,WRITE,GPIO_OFF);
                                    send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,PAGE_SPEAKER,CLEAR,&io_led_clear[ZERO],
                                    IO_ADDRESS,LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                                    add_membership(g_iAim_fds[stn_config.default_AIM-ONE]);
                                    start_aim_receive_call();
                                }
                                else
                                {
                                    send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,PAGE_SPEAKER,SET,&io_led[FIVE],
                                    IO_ADDRESS,LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                                    ///operate_gpio(MUSIC_VOL_BYPASS,WRITE,GPIO_ON);
                                }
                                  if(lcd_fcs_default_icom == 0)
                                  {
                                        sprintf(g_cLine3_buf,IDLE_msg,stn_config.logical_id);
                                        send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                  }
                                  else
                                  {
                                        sprintf(g_cLine3_buf,Channel_msg,stn_config.logical_id,lcd_fcs_default_icom);
                                        send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                  }

                            break;


                        case ZONE_RESET_EVENT:
                                 g_uiVoipapp_wait_timer = CLEAR;
                                // printf("zone reset event received [%s %d]\n",__FILE__,__LINE__);
                                printf("inside  PAGE_INITIATOR_STATE event is ZONE_RESET_EVENT \n");
                                send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,LED_CALL_THROUGH,CLEAR,&io_led_clear[ZERO],
                                IO_ADDRESS,LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                                pg_call.alm_no=CLEAR;
                                pvt_call.type=CLEAR;
                                pg_call.type=CLEAR;
                                //close_channel(PG_CHANNEL);
                                pg_call.pg_call_busy = CLEAR;
                                g_uiPage_call_timer=CLEAR;
                                pvt_call.pvt_call_busy = CLEAR;
                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,"   Resetting    ",ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,zone_terminate,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                sleep(0.5);
                                sprintf(g_cLine4_buf,"%s          ",lcd_fcs_desc);
                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,g_cLine4_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                pWork_m.private_state=PRIVATE_IDLE;
                                g_itransmit_fd_index=-1;
                                g_uiFsu_state=IDLE_STATE;

                                pvt_call.curr_icom_no=stn_config.default_intercom-MIN_OFFSET;
                                if(stn_config.icom_live_dead_status[stn_config.default_intercom-MIN_OFFSET]==LIVE)
                                {
                                    add_membership(g_Iicom_fds[stn_config.default_intercom-MIN_OFFSET]);
                                    start_intercom();
                                }

                                if(stn_config.default_AIM)
                                {
                                    ///operate_gpio(MUSIC_VOL_BYPASS,WRITE,GPIO_OFF);
                                    send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,PAGE_SPEAKER,CLEAR,&io_led_clear[ZERO],
                                    IO_ADDRESS,LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                                    add_membership(g_iAim_fds[stn_config.default_AIM-ONE]);
                                    start_aim_receive_call();
                                }
                                else
                                {
                                    send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,PAGE_SPEAKER,SET,&io_led[FIVE],
                                    IO_ADDRESS,LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                                    ///operate_gpio(MUSIC_VOL_BYPASS,WRITE,GPIO_ON);
                                }
                                if(lcd_fcs_default_icom == 0)
                                  {
                                        sprintf(g_cLine3_buf,IDLE_msg,stn_config.logical_id);
                                        send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                  }
                                  else
                                  {
                                        sprintf(g_cLine3_buf,Channel_msg,stn_config.logical_id,lcd_fcs_default_icom);
                                        send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                  }
                            break;



                    }
                    g_uiFsu_event=CLEAR;
                    break;

                    case PAGE_RESET_RINGBACK_STATE:
                         printf("________INSIDE PAGE_RESET_RINGBACK_STATE \n");
                        switch(g_uiFsu_event)
                        {
                            case WAKE_UP_EVENT:
                                printf("____________PAGE_RESET_RINGBACK_STATE______WAKE_UP_EVENT___\n");
                                reset_all();
                                g_uiVoipapp_wait_timer = CLEAR;
                                g_uiFsu_state=VCCS_REGISTERING_STATE;
                            break;

                            case BROADCAST_TIMEOUT_EVENT:
                                 printf("Inside BROADCAST_TIMEOUT_EVENT\n");
                                 Restart_Broadcast_Socket();
                            break;

                            case ZONE_RESET_EVENT:
                                g_uiVoipapp_wait_timer = CLEAR;
                               // printf("zone reset event received [%s %d]\n",__FILE__,__LINE__);
                                printf("inside  PAGE_RESET_RINGBACK_STATE event is ZONE_RESET_EVENT \n");
                                send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,LED_CALL_THROUGH,CLEAR,&io_led_clear[ZERO],
                                IO_ADDRESS,LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                                pg_call.alm_no=CLEAR;
                                pvt_call.type=CLEAR;
                                pg_call.type=CLEAR;
                                //close_channel(PG_CHANNEL);
                                pg_call.pg_call_busy = CLEAR;
                                g_uiPage_call_timer=CLEAR;
                                pvt_call.pvt_call_busy = CLEAR;
                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,zone_terminate,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                sleep(0.5);
                                sprintf(g_cLine4_buf,"%s          ",lcd_fcs_desc);
                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,g_cLine4_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                pWork_m.private_state=PRIVATE_IDLE;
                                g_itransmit_fd_index=-1;
                                g_uiFsu_state=IDLE_STATE;
                                ///operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);

                                pvt_call.curr_icom_no=stn_config.default_intercom-MIN_OFFSET;
                                if(stn_config.icom_live_dead_status[stn_config.default_intercom-MIN_OFFSET]==LIVE)
                                {
                                    add_membership(g_Iicom_fds[stn_config.default_intercom-MIN_OFFSET]);
                                    start_intercom();
                                }
                                if(stn_config.default_AIM)
                                {
                                    ///operate_gpio(MUSIC_VOL_BYPASS,WRITE,GPIO_OFF);
                                    send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,PAGE_SPEAKER,CLEAR,&io_led_clear[ZERO],
                                    IO_ADDRESS,LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                                    add_membership(g_iAim_fds[stn_config.default_AIM-ONE]);
                                    start_aim_receive_call();
                                }
                                else
                                {
                                    send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,PAGE_SPEAKER,SET,&io_led[FIVE],
                                    IO_ADDRESS,LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                                    ///operate_gpio(MUSIC_VOL_BYPASS,WRITE,GPIO_ON);
                                }
                                if(lcd_fcs_default_icom == 0)
                                {
                                    sprintf(g_cLine3_buf,IDLE_msg,stn_config.logical_id);
                                    send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                }
                                else
                                {
                                    sprintf(g_cLine3_buf,Channel_msg,stn_config.logical_id,lcd_fcs_default_icom);
                                    send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                }
                            break;

                            case GLOBAL_RESET_EVENT:
                                g_uiVoipapp_wait_timer = CLEAR;
                                //printf("Global reset event received [%s %d]\n",__FILE__,__LINE__);
                                printf("inside  PAGE_RESET_RINGBACK_STATE event is GLOBAL_RESET_EVENT \n");
                                send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,LED_CALL_THROUGH,CLEAR,&io_led_clear[ZERO],
                                IO_ADDRESS,LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                                pg_call.alm_no=CLEAR;
                                pvt_call.type=CLEAR;
                                pg_call.type=CLEAR;
                                close_channel(PG_CHANNEL);
                                pg_call.pg_call_busy = CLEAR;
                                g_uiPage_call_timer=CLEAR;
                                pvt_call.pvt_call_busy = CLEAR;
                               // send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,global_terminate,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                sleep(0.5);
                                sprintf(g_cLine4_buf,"%s          ",lcd_fcs_desc);
                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,g_cLine4_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                pWork_m.private_state=PRIVATE_IDLE;
                                g_itransmit_fd_index=-1;
                                g_uiFsu_state=IDLE_STATE;
                                operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);

                                pvt_call.curr_icom_no=stn_config.default_intercom-MIN_OFFSET;
                                if(stn_config.icom_live_dead_status[stn_config.default_intercom-MIN_OFFSET]==LIVE)
                                {
                                    add_membership(g_Iicom_fds[stn_config.default_intercom-MIN_OFFSET]);
                                    start_intercom();
                                }

                                if(stn_config.default_AIM)
                                {
                                    ///operate_gpio(MUSIC_VOL_BYPASS,WRITE,GPIO_OFF);
                                    send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,PAGE_SPEAKER,CLEAR,&io_led_clear[ZERO],
                                    IO_ADDRESS,LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                                    add_membership(g_iAim_fds[stn_config.default_AIM-ONE]);
                                    start_aim_receive_call();
                                }
                                else
                                {
                                    send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,PAGE_SPEAKER,SET,&io_led[FIVE],
                                    IO_ADDRESS,LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                                    ///operate_gpio(MUSIC_VOL_BYPASS,WRITE,GPIO_ON);
                                }
                                if(lcd_fcs_default_icom == 0)
                                  {
                                        sprintf(g_cLine3_buf,IDLE_msg,stn_config.logical_id);
                                        send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                  }
                                  else
                                  {
                                        sprintf(g_cLine3_buf,Channel_msg,stn_config.logical_id,lcd_fcs_default_icom);
                                        send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                  }
                            break;

                            case APP_TIMEOUT_EVENT:
                                printf("APP timeout event received in PAGE_RESET_RINGBACK_STATE state\n");
                                g_uiVoipapp_wait_timer = CLEAR;
                                g_uiFsu_state=IDLE_STATE;
                            break;

                            default:
                            break;

                        }
                        g_uiFsu_event=CLEAR;
                        break;



                //default:
                //break;
            }///switch
        }///if(g_uiFsu_event)
    }///while(1)


















	DPRINTK("Voip task stopping\n");

    voip_stack_stop();
	DPRINTK("[%s:%d]\n", __FUNCTION__, __LINE__);

	DPRINTK("FILE=%s, LINE=%d Calling dsp_uninit - voip task is out......\n", __FILE__, __LINE__);
    dsp_uninit();
#ifdef CONFIG_RG_ATA
    slic_uninit();

#ifdef FXO_SILABS
    slac_uninit();
#endif /*FXO_SILABS */

#elif CONFIG_RG_IPPHONE
#ifndef CONFIG_GTEK

#ifndef NANOX
 	//lcdClose();
#endif

	closeKeypad();
#endif
#endif


	DPRINTK("[%s:%d]\n", __FUNCTION__, __LINE__);
//    phone_output_uninit();
	DPRINTK("[%s:%d]\n", __FUNCTION__, __LINE__);

//Error_output_init:
    voip_stack_uninit();
//Error_stack_init:
    voip_conf_uninit();
//    ipc_uninit();
//Error_event_loop:
    //event_loop_uninit();

    printf("Voip task stopped\n");//Avner August PS
   //return ret_val;
}



//060530 Avner phase 1
/*int is_line_enabled(int line)
{
    return call_mngr_params.lines[line].enabled;
}
//060530 Avner phase 1
int is_using_proxy()
{
    return call_mngr_params.protocol == VOIP_PROT_SIP &&
       	call_mngr_params.sp.sip.use_proxy;
}

static void event_loop_init()
{
	if(pthread_cond_init(&event_loop_cond, NULL) < 0)
	{
		printf("error at pthread_cond_init\r\n");
		return;
	}

	if(pthread_mutex_init(&event_loop_mut, NULL) < 0)
	{
		printf("error at pthread_mutex_init\r\n");
		return;
	}
}

static void event_loop_uninit()
{
	if(pthread_cond_destroy(&event_loop_cond) < 0)
	{
		printf("error at pthread_cond_destroy\r\n");
	}

	if(pthread_mutex_destroy(&event_loop_mut) < 0)
	{
		printf("error at pthread_mutex_destroy\r\n");
	}
}

static void event_loop()
{
	pthread_mutex_lock(&event_loop_mut);
	pthread_cond_wait(&event_loop_cond, &event_loop_mut);
	pthread_mutex_unlock(&event_loop_mut);
}

static void load_dsp_kernel_modules()
{
	if(isFirstLoading)	/* first time loading - load the kernel modules */
	//{
/*#if defined(I_PLATFORM)
	//  	system("insmod /usr/bin/audiocodes/slic/tdmspicd.ko");
	//   	system("insmod /usr/bin/audiocodes/slic/tdmspipd.ko");
#endif

#if defined(P_PLATFORM)
		system("insmod /var/ftp/init.ko");
		system("insmod /var/ftp/ac48dsp.ko");
//		system("insmod /usr/bin/audiocodes/dsp/init.ko");
//		system("insmod /usr/bin/audiocodes/dsp/ac48dsp.ko");
#elif defined(I_PLATFORM)
	//	system("insmod /usr/bin/audiocodes/dsp/ac48dsp.ko");

#elif defined(B_PLATFORM)||defined(GROCX_PLATFORM)
		system("insmod /audiocodes/dsp/init.ko");
		system("/audiocodes/scripts/ac48dsp_load.sh");
#elif defined(B_CEVA_PLATFORM)
		system("insmod /audiocodes/dsp/init.ko");
		system("/audiocodes/scripts/ceva_dsp_load.sh");
#elif defined(S_PLATFORM)
		//system("insmod init.ko");
		system("insmod ac49x_dsp.ko");
#endif
	}



}*/

#ifdef CONFIG_RG_ATA
static void load_slic_related_kernel_modules()
{
	if(isFirstLoading)	/* first time loading - load the kernel modules */
	{

#ifdef FXS_LEGERITY
#if defined(P_PLATFORM)
		system("insmod /var/ftp/le88drv.ko");
//		system("insmod /usr/bin/audiocodes/slic/le88drv.ko");

#elif defined(I_PLATFORM)
	//  	system("insmod /usr/bin/audiocodes/slic/le88drv.ko");

#elif defined(B_PLATFORM)||defined(B_CEVA_PLATFORM)||defined(GROCX_PLATFORM)
		system("/audiocodes/scripts/le88drv_load.sh");
#elif defined(S_PLATFORM)
	//	system("insmod ac49x_mpi.o");
		system("insmod /tmp/ac49x_mpi.ko");
		system("insmod /tmp/le88.ko");
#endif
#endif /*FXS_LEGERITY*/



#if defined(FXS_SILABS) || defined(FXO_SILABS)
#if defined(S_PLATFORM)
       system("insmod /tmp/ac49x_mpi.ko");
	system("insmod /tmp/silabs_daa_3050_drv_mod.ko");
	system("insmod /tmp/silabs_fxs_3215_drv_mod.ko");
#endif
#endif/*FXS_SILABS*/

	}
}

#endif /*CONFIG_RG_ATA*/


/*void launch_cfg_listening_socket_thread()
{
	pthread_t				tid;
	struct sched_param		sched;
	pthread_attr_t			attr;

	memset(&sched, 0, sizeof(sched));

	/* create the cfg_listening_socket thread */
	/*if( (pthread_attr_init(&attr) != 0) )
	{
		printf("pthread_attr_init");
		exit(1);
	}

	if( (pthread_attr_setschedpolicy(&attr, SCHED_RR) != 0) )
	{
		printf("pthread_attr_setschedpolicy");
		exit(1);
	}

	sched.sched_priority = 69;

	if( (pthread_attr_setschedparam(&attr, &sched) != 0) )
	{
		printf("pthread_attr_setschedparam");
		exit(1);
	}

	if( (pthread_create( &tid, &attr, cfg_listening_socket, 0) != 0) )
	{
		printf("pthread_create - cfg_listening_socket");
		exit(1);
	}

}


void *cfg_listening_socket(void *dummy)
{
	int					buf_len;
   	char             			buf[MAX_LEN];

	struct  sockaddr_un 	from;
	socklen_t				fromlen;

	cfg_msg 				*pReceivedBuf;
	TableEnt				*pTableEntry;


	fromlen = sizeof(struct sockaddr_un);
	memset(buf, 0, sizeof(buf));

	waitUntilVoipTaskReadyForReconfig = 1;

	DPRINTK("FILE=%s, LINE=%d .\n", __FILE__, __LINE__);

	if(cfg_init_agent_socket(&acl_main_Voip_Task_AgentSocketFd, ACL_MAIN_VOIP_TASK_AGENT_SOCKET_NAME,
			ACL_MAIN_VOIP_TASK_MANAGER_SOCKET_NAME, &acl_main_Voip_Task_Agent_Remote_Addr) == -1)
	{
		perror("\ncfg_init_agent_socket() failure\n");
		return NULL;
	}

	if(needToSendWaitForBufferMessage)
	{
		if(cip_send_general(WAITING_FOR_BUFFER, acl_main_Voip_Task_AgentSocketFd,
							acl_main_Voip_Task_Agent_Remote_Addr) == -1)
		{
			perror("\ncip_send_general()");
			return NULL;
		}
	}

	for(;;)
	{
		if ((buf_len = recvfrom(acl_main_Voip_Task_AgentSocketFd, buf, sizeof(buf), 0,
									(struct sockaddr *)&from, &fromlen)) < 0)
		{
			perror("\nrecvfrom::create_command_socket");
			continue;
		}

		DPRINTK("FILE=%s, LINE=%d .\n", __FILE__, __LINE__);

		buf[buf_len] = '\0';

		pReceivedBuf = (cfg_msg *)buf;

		if( (pReceivedBuf->opcode) > NUMBER_OF_OPCODES )
		{
			printf("\nUnrecognized opcode");
			return NULL;
		}

		if(pReceivedBuf->opcode == END_CONFIG) /* the configuration buffer
											    transmittion is finished */
		/*{
			strcat(receivedCfgBuf, "\0");

			waitUntilCfgBufArrives = 0;

			while(waitUntilVoipTaskReadyForReconfig) /*need to wait until voip_task is ready for reconfiguring */
			/*{
				IppThreadSleep(0,200);
			}

			DPRINTK("FILE=%s, LINE=%d .\n", __FILE__, __LINE__);

		}

		pTableEntry = (TableEnt *)Get_Table_Entry(pReceivedBuf->opcode);

		pTableEntry->opHandler(pReceivedBuf->info);

	} /* for(;;) */

	//return NULL;

//}
