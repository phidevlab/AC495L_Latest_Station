/**********************************************************************
 *                                                                    *
 * File Name:	voip_main.c                                           *
 *
 * Author: S.K. SHINDE                                                *
 *
 * Copyright (c) by Phi Audiocom Systems Pvt. Ltd.                    *
 * This software is copyrighted by and is the sole property of        *
 * Phi Audiocom Systems Pvt. Ltd.                                     *
 *
 * Release : 4 Mar 2018                                               *
 *                                                                    *
 **********************************************************************/

#include <unistd.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

#include "acl_MsgQ.h"
#include "voip_main.h"
#include "acGatewayAPI.h"
#include "acl_ui_commands.h"
#include "acl_line_connection.h"
#include "acl_voip_conf.h"
#include "voip_utils.h"
#include "voip_rtp_utils.h"
#include "acl_dsp_telephony_commands.h"
#include "acl_log.h"
#include "voip_status_handler.h"
#include "server_coms.h"
#include "config_manager.h"
#include "keypad_interface.h"
#include "acl_call_mngr.h"
#include "cip_def.h"
#include "cip_api.h"
#include "cfg_api.h"
#include "iphone_paging_pub.h"
#include "lcd_messages.h"
#include "i2c.h"
#include "ui.h"

#if defined(AC494_SDP_IPPHONE) || defined(AC49_ORCHID_VOIP_TOOLKIT)

#ifdef CONFIG_GTEK
#include "IPPMessageQueue.h"
#else
#include "iphone_hwKeypad.h"

#ifdef NANOX
// #include "nanox_hwLcd.h"
#else
#include "iphone_hwLcd.h"
#endif

#endif

#endif

#define VOIP_MAIN_DEBUG 1
#ifdef VOIP_MAIN_DEBUG
#define DPRINTK(format, args...) printf("[%s:%4d] " format, __FUNCTION__, __LINE__, ##args)
#else
#define DPRINTK(format, args...)
#endif
#define MAX_IPC_INIT_RETRIES 5
#define MAX_LINE_LENGTH 20
#define MAX_VALID_KEY_SEQ 12
#define MAX_IP_LENGTH 30
#define MAX_STN_ID_LENGTH 5
#define MAX_APP_FDS 100

#define HANDSET_STN     1
#define PTT_STN         2
#define GOOSENECK_STN   3

int station_type = GOOSENECK_STN ;  //PTT_STN  HANDSET_STN GOOSENECK_STN

/* Prototypes */

// static void load_dsp_kernel_modules();

void scan_keyboard(int g_uiFsu_state);
extern char receivedCfgBuf[CONFIGURATION_BUFFER_MAX_LEN];
#ifdef CONFIG_RG_VOIP_RV_SIP
extern int IppThreadSleep(int sec, int msec);
#endif
extern TableEnt *Get_Table_Entry(CIP_OPCODES opcode);

struct sockaddr_un g_cli_addr;
struct sockaddr_un g_app_ui_addr;
struct sockaddr_un radv_sock_addr, radv_sock_address;
struct sockaddr_un radv_sock_cli_addr;
struct sockaddr_un acl_main_Voip_Task_Agent_Remote_Addr;
struct sockaddr_in udp_g_cServer_addr;
struct sockaddr_in udp_comm_addr;
struct sockaddr_in reg_addr;
struct sockaddr_in addr;
struct sigaction action;
struct sigaction info;

fd_set app_readset;
fd_set app_testset;
sigset_t pselect_set;

int debounceCounter = 0;
const int debounceDelay = 10;
int debounce_flag = SET;

int debounceCounterPtt = 0;
const int debounceDelayPtt = 10;
int ptt_pressed_flag = CLEAR;

// int sre_mute_flag;

pthread_t m_thread;

int sre_debounce_counter = PRESS_DEBOUNCE_TIMECOUNT;

unsigned int g_uiPvt_call_time_lcd;
unsigned int g_uipg_call_time_lcd_init;
unsigned int g_uipg_call_time_lcd;

int one_second_timeout = ONE_SEC_TIMEOUT;
// int one_second_timeout_1 = 50;

char g_arcPvtCallTimer_fr_pg_init[TWENTY];

char g_arcPgCallTimer[TWENTY];
char g_arcPgCallTimer[TWENTY];

pthread_mutex_t lock_icom_contr;
pthread_mutex_t lock_call_type;
pthread_mutex_t lock_group_contr;
acl_call_mngr_ops_t *call_mgt;
rtp_session_t *rtp_sessions;
u8 rtp_tos;
unsigned int g_uiPvt_call_timer_ui_lcd;
unsigned int g_ui_sip_p2p_timer;
unsigned int g_uiMul_sig_addrlen;
unsigned int g_uiUdp_comm_addrlen;
unsigned int g_uiServ_udp_comm_fd;
unsigned int g_uiRadv_sock_cli_addrlen;
unsigned int g_uiRadv_addrlen;
unsigned int g_uiApp_ui_addrlen;
unsigned int g_uiReg_addrlen;
unsigned int g_uiRv_sock_cli_fd = CLEAR;
unsigned int g_uiClilen;
unsigned int g_uiPvt_call_timer;
unsigned int g_uiPage_call_timer;
unsigned int g_uiPlay_tone_timer;
unsigned int g_uiFsu_event;

unsigned int g_uiPage_call_timer_lcd;
unsigned int g_uiPage_call_timer_frlcd_ini;

/// added by shraddha
unsigned int g_uiFsu_Broadcast_event;
unsigned int g_uiFsu_mcast_event;
unsigned int g_uiFsu_timer_event;
/// added by shraddha
unsigned int g_uiFsu_state;
unsigned int g_uiUi_socket_fd;
unsigned int g_uiApp_timer_fd;
unsigned int g_uiApp_diag_timer_fd;
unsigned int g_uiVoipapp_wait_timer;
unsigned int g_uiServer_fd;
unsigned int g_uiClient_connection_fd;
unsigned int g_uiIncoming_pg_port;
unsigned int g_uiIcom_led_on_flag;
unsigned int g_uiP2p_led_on_flag;
unsigned int p2p_call_process_timer_flag;
unsigned int g_uiGroup_active_contr_flag = CLEAR;
unsigned int g_uiGroup_PTT_flag = CLEAR;
unsigned int g_uiPvt_diagnostic_timer;
unsigned int g_uiSpk_diagnostic_timer;
unsigned int g_uiDiagnostic_start_flag;
unsigned int g_uiServer_comm_fd;
unsigned int g_uiBroadcast_fd;
unsigned int g_uiPjsip_fd;
unsigned int g_uiUi_socket_fd;
unsigned int g_uiRadv_sock_fd;
unsigned int g_uiMulticast_signalling_fd;
// unsigned int          g_uiMulticast_signalling_fd_sample;
unsigned int g_uiServer_live_status_timer;

unsigned int g_uiKeep_alive_timer;
unsigned int g_uiConfig_Received = CLEAR;
unsigned int g_uiSpeech_detection_timer;

int check_one = 60;
int check_var;

int g_iGlobal_mem_dropped = -1;
int g_iAim_mem_dropped = -1;
int g_iIcom_mem_dropped = -1;
int g_iDiagnostic_pvt_status;
int g_iDiagnostic_spk_status;
int g_iDiagnostic_mic_status;
int g_iDiagnostic_nw_status;
int g_iGpio_fd;
int g_iIcom_hop_flag;
int g_iHandset_lifted_signal = CLEAR;
int g_iAcl_main_Voip_Task_AgentSocketFd; // 20 jan 2023
int g_iDspFirmwareName;
int g_imss_clamping_limit;
char lcd_flag = CLEAR;
char g_cLine1_buf[MAX_LINE_LENGTH];
char g_cLine2_buf[MAX_LINE_LENGTH];
char g_cLine3_buf[MAX_LINE_LENGTH];
char g_cLine4_buf[MAX_LINE_LENGTH];
char g_cReceived_key_seq[MAX_VALID_KEY_SEQ];
char g_cCall_type[MAX_STN_ID_LENGTH];
char g_cCaller[MAX_STN_ID_LENGTH];
char g_cCalled_station[MAX_STN_ID_LENGTH];
char g_cServer_addr[MAX_IP_LENGTH];
char g_cMul_signalling_ip[MAX_IP_LENGTH];
char g_cSip_ok_flag = CLEAR;
char g_cInit_app = SET;
char g_cMulticast_tx_address[MAX_IP_LENGTH];
char g_cAcl_cfgfilename[CONFIGURATION_FILE_NAME_SIZE];
/*the configuration buffer that will hold the configuration data */
unsigned int aim_mem_drop_flag = CLEAR;

static int g_siFsu_initialized = CLEAR;
// int                   Call_cmdid;

/*static int			  g_siNeedToSendWaitForBufferMessage;
static int			  g_siIsFirstLoading;
static int			  g_siWaitUntilCfgBufArrives = 1;
static int			  g_siWaitUntilVoipTaskReadyForReconfig = 1;*/
static u16 g_siLocal_host_port;
static u16 g_siLocal_host_range;
static u8 *port_used;
static acl_call_mngr_params_t call_mngr_params;
static voip_dsp_advanced_channel_configuration_t AdvancedChannelConfiguration;

unsigned int last_req_p2p_sent_to_server;
unsigned int p2p_req_retransmission_count;

unsigned int last_req_sent_to_server;
unsigned int req_retransmission_count;

char buf[FIVE];
int n;
unsigned char uc_call_type;

#include "iphone_localNetTerm.h"
// extern socketInfo_s  socketDB[NUMBER_OF_DSP_CHANNELS];
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

    g_siLocal_host_port = port;
    g_siLocal_host_range = range;

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
    static u16 next_to_use = CLEAR;
    int cur_pos = CLEAR;

    /* VOIP-THIRD-PARTY-LIB Modfified by Phi Audiocom Start { */
    /* port must be predefined only in case the firewall is present, otherwise
     * we may use any free port */
    if (!vcCONFIG_RG_FIREWALL)
        return 0;
    /* } END VOIP-THIRD-PARTY-LIB Modfified by Phi Audiocom */

    /* loop through the port array, starting from the place marked by
     * next_to_use. 'idx' is used to count the number of ports searched,
     * to avoid an infinite loop */
    for (idx = CLEAR; idx < g_siLocal_host_range; idx++)
    {
        cur_pos = (next_to_use + idx) % g_siLocal_host_range;
        if (!port_used[cur_pos])
        {
            port_used[cur_pos] = SET;
            /* the next time start search from the port following this one */
            next_to_use = (cur_pos + SET) % g_siLocal_host_range;
            break;
        }
    }

    return idx == g_siLocal_host_range ? 0 : g_siLocal_host_port + cur_pos;
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
    u16 idx = port - g_siLocal_host_port;

    if (vcCONFIG_RG_FIREWALL && idx < g_siLocal_host_range)
        port_used[idx] = CLEAR;
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
    printf("SIGNAL HANDLER FOR SIGTERM\n");

    g_cInit_app = CLEAR;
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
    g_cInit_app = CLEAR;
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

    g_cInit_app = CLEAR;
}

/**************************************************************************************
 * Function name: voip_stack_start                                                      *
 * Input arg    : void                                                                  *
 * Output arg   : NONE                                                                    *
 * Description  : Initialize the SIP stack
 * Developer-Note: Unmodified Source block. Received as a part of AUDIOCODES library used as it is without change.                                         *
 ***************************************************************************************/

void voip_stack_start(void)
{
    int i;
    /* XXX static values, because params is not copied by the call manager */
    static codec_t codecs[NUMBER_OF_CODECS];
    /* ACL NB  061102 */

    memset(&codecs, CLEAR, sizeof(codecs));
    for (i = CLEAR; i < NUMBER_OF_CODECS; i++)
        codecs[i].payload = -1;

    memset(&call_mngr_params, CLEAR, sizeof(call_mngr_params));
    DPRINTK("voip_stack_start: Enter.\n");

    call_mngr_params.ep_num = voip_conf_get_number_of_lines(ANALOG_PORT_TYPE_FXO_FXS);

    /* need to stop, only if we've ever been started */
    if (call_mgt)
    {
        DPRINTK("voip_stack_start(after call to voip_stack_stop): line=%d.\n", __LINE__);
        voip_stack_stop();
    }

    DPRINTK("voip_stack_start: line=%d  call_mngr_params.lines = 0x%08x\n", __LINE__,
            (unsigned int)call_mngr_params.lines);

    /* Initialize proprietary message queue mechanism */
    //	init_acMsgQDB();

    if (AC_TIMER_INIT() < CLEAR)
        printf("AC_TIMER_INIT failed");

    DPRINTK("voip_stack_start: line=%d, NumOfLines=%d.\n", __LINE__,
            call_mngr_params.ep_num);
    call_mngr_params.lines = AC_ZALLOC_L(sizeof(line_conf_t) * call_mngr_params.ep_num);

    DPRINTK("voip_stack_start: line=%d  call_mngr_params.lines = 0x%08x\n", __LINE__,
            (unsigned int)call_mngr_params.lines);
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
    g_imss_clamping_limit = call_mngr_params.mss_clamping_limit;

    if (vcCONFIG_RG_FIREWALL &&
        g_siLocal_host_ports_alloc(call_mngr_params.local_host_port,
                                   call_mngr_params.local_host_port_range) < CLEAR)
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
    for (i = CLEAR; i < call_mngr_params.ep_num; i++)
        voip_conf_line_conf_get(i, &call_mngr_params.lines[i]);

    DPRINTK("voip_stack_start: line=%d.\n", __LINE__);

    for (i = CLEAR; i < NUMBER_OF_CODECS; i++)
        voip_conf_codec_info_get(i, &codecs[i]);

    DPRINTK("voip_stack_start: line=%d.\n", __LINE__);

    voip_conf_dynamic_payloads_info_get(); // NirM,  configure payload type - update payload type parameters

    DPRINTK("voip_stack_start: line=%d.\n", __LINE__);

    /* ACL NB 060723 support DTMF via SIP  */
    /* acl_line_connection_init(codecs, call_mngr_params.send_dtmf_out_of_band); */
    if (call_mngr_params.dtmf_transport_method == DTMF_RFC2833)
        acl_line_connection_init(codecs, SET);
    else
        acl_line_connection_init(codecs, CLEAR);

    DPRINTK("voip_stack_start: line=%d   call_mngr_params.local_ip = %s\n", __LINE__, call_mngr_params.local_ip);
    /* AlexChen 20090331 SDK upgrade */
    expandDigitMap();

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
static void voip_stack_uninit(void)
{
    if (AC_TIMER_DESTROY() < CLEAR)
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

static int voip_stack_init(void)
{
    return CLEAR;
}

void getTime()
{
    int hr, min, sec;

    time_t T = time(NULL);
    struct tm tm = *localtime(&T);
    printf("Time is : %02d:%02d:%02d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
}

/*     read_from_io(SRE_MUTE_ACT);
    sre_ret = pin_status;
//   printf("SRE_MUTE_ACT--------:0x%x  %d\n", sre_ret, sre_ret);


    if (sre_ret == 0) // SET)

    {
        if (stn_config.default_AIM) // if  AIM is configured.
        {
             printf("SRE_MUTE_FLAG--------  %d\n", sre_mute_flag);
            if (sre_mute_flag == SET) // if mute flag is set, i.e., if the speaker was previously muted.
            {
               // add_membership(g_iAim_fds[stn_config.default_AIM - 1]); // add AIM membership
               // start_aim_receive_call();                               // start AIM call
                sre_mute_flag = CLEAR;                                  // CLEAR mute flag
            }
            else
            {
               // drop_membership(g_iAim_fds[stn_config.default_AIM - 1]); // drop AIM membership
                sre_mute_flag = SET;                                     // SET mute flag
            }
        }
    } */

void toggle_sre_using_gpio()
{

    int sre_return_value;

        read_from_io(SRE_MUTE_ACT);
        sre_return_value = pin_status;
       // printf("sre_return_value:--------0x%x\n",sre_return_value);
        if (sre_return_value == SET) // if button is pressed or  pin is set
        {
            //debounceCounter++;
           // if(debounceCounter >= debounceDelay)
           // {

             if (stn_config.default_AIM)
             {
                 if (debounce_flag == CLEAR)
                {
                    //printf("SRE_MUTE_FLAG--------  %d\n", sre_mute_flag);
                    if (sre_mute_flag == SET)
                    {
                        printf("sre_mute_flag == SET----------------\n");
                        add_membership(g_iAim_fds[stn_config.default_AIM - 1]); // add AIM membership
                        start_aim_receive_call();                               // start AIM call
                        sre_mute_flag = CLEAR;
                    }
                    else
                    {
                        printf("sre_mute_flag CLEAR----------------\n");
                        drop_membership(g_iAim_fds[stn_config.default_AIM - 1]); // drop AIM membership i.e., mute music
                        sre_mute_flag = SET;

                    }

                    debounce_flag = SET;
                }
            }
        //}
        }
        else
        {
            //printf("else of sre mute------\n");
          //  debounceCounter = 0;
            debounce_flag = CLEAR;
        }
   // }
}


void operate_mic_ptt_sw()
{
 if(station_type == PTT_STN)
{


    char call_type[FOUR];
    int ptt_return_value;
    unsigned char uc_call_type;

    write_to_io(PVT_AMP_MUTE_OFF, IO_PIN_OFF);


    //debounceCounterPtt++;

    uc_call_type = pvt_call.type;

    read_from_io(MIC_PTT_SW);
    ptt_return_value = pin_status;

    //printf("ptt_return_value in ptt sw : %d +++++++++++\n",ptt_return_value);

        if (ptt_return_value == 1) // if button is pressed or  pin is set
        {
            write_to_diag_io(MIC_MUTE_OFF, IO_PIN_OFF);

            if (uc_call_type == GLOBAL || uc_call_type == P2P || uc_call_type == ZONE)
             {
                //select_codec(PVT_CHANNEL, HANDSET);


                write_to_diag_io(MIC_MUTE_OFF, IO_PIN_OFF);


                printf("Mic TX on ++++++++++++++++++ \n");

             }
                else if (pg_call.type == GLOBAL_ALARM || pg_call.type == ZONE_ALARM)
                {
                    printf("ALARM CALL ACTIVE BN*****---------******");
                }

                else
                {


                    if(ptt_pressed_flag == CLEAR)
                    {
                        write_to_diag_io(MIC_MUTE_OFF, IO_PIN_OFF);

                    printf("GPIO PTT_KEY KEY PRESSED  \n");
                    g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = '0';
                    g_ucDialed_Sequence_No++;
                    g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = PTT_KEY;

                    if (g_iarcDialed_Key_Array[0] > 0)
                    snprintf(call_type, FOUR, "%c", (char)g_iarcDialed_Key_Array[0]); //-'0');
                    sprintf(buf, "%s%d%d%d%d", call_type, g_iarcDialed_Key_Array[1], g_iarcDialed_Key_Array[2],
                    g_iarcDialed_Key_Array[3], g_iarcDialed_Key_Array[4], g_iarcDialed_Key_Array[5]);
                    printf("------------key_buf_is----------%s \n",buf);

                     memcpy(g_cReceived_key_seq, buf, sizeof(buf));
                     g_uiFsu_event = VALID_KEY_SEQ_EVENT;
                     clear_validation_processing();


                           /* if (icom_contributor_status_info[pvt_call.curr_icom_no].cont1 == CLEAR ||
                                icom_contributor_status_info[pvt_call.curr_icom_no].cont2 == CLEAR)
                            {
                                printf("IN PTT send \n");
                                g_uiVoipapp_wait_timer = SERVER_RESPONSE_TIMEOUT;
                                send_req_msg_to_server(0, ICOM_PTT_SEND_REQ, ICOM_CALL_FRAME_LENGTH, '1', '0');
                                // send_msg_ui(P2P_OFF_HOOK_STATE,LCD,LINE2,CONTINUE_DISPLAY, g_cLine2_buf,LCD_CLR_SECOND_LINE,0);
                                // send_msg_ui(RINGBACK_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,CLEAR);
                                g_uiFsu_state = RINGBACK_STATE;
                            }
                            else
                            {
                                if (g_uiPlay_tone_timer == CLEAR)
                                {
                                    acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_BUSY_TONE, START_TONE, CLEAR);
                                    g_uiPlay_tone_timer = SIX * APP_TIMER_MULTIPLIER;
                                }
                            }*/
                     }

                }
              // debounce_flag_ptt = CLEAR;

              ptt_pressed_flag = SET;
        }
        else
        {
            if(ptt_pressed_flag == SET)
            {
                 write_to_diag_io(MIC_MUTE_ON, IO_PIN_ON);

            if (uc_call_type == GLOBAL || uc_call_type == P2P || uc_call_type == ZONE)
                {
                 //   select_codec(PVT_CHANNEL, NONE);
                 write_to_diag_io(MIC_MUTE_ON, IO_PIN_ON);

                 printf("Mic TX OFF ++++++++++++++++++ \n");

                }

                else if (pg_call.type == GLOBAL_ALARM || pg_call.type == ZONE_ALARM)
                {
                    printf("ALARM CALL ACTIVE *****---------******");
                }

                else
                {
                  if(ptt_pressed_flag == SET)
                    {
                    printf("PTT RELEASE IN OPERATE MIC PTT SW+++++++\n");
                    ptt_pressed_flag = CLEAR;

                    write_to_diag_io(MIC_MUTE_ON, IO_PIN_ON);


                    g_uiFsu_event = PTT_RELEASE_EVENT;

                   }

                }


              //  debounce_flag_ptt = SET;
            }


                             // if( uc_call_type == INTERCOM_RX ) // IDLE INTERCOM RECEIVING CONDITION I.e., NOT A CONTRIBUTOR
                             // {
                             //     printf("uc_call_type == INTERCOM_RX mic ptt sw++++++++\n");
                             //      write_to_diag_io(MIC_MUTE_ON, IO_PIN_ON);
                             //      write_to_io(PVT_AMP_MUTE_OFF, IO_PIN_OFF);

                             // }


        }
        }

 }


/*void operate_mic_ptt_sw()
{
    char call_type[FOUR];
    int ptt_return_value;
    unsigned char uc_call_type;

    //debounceCounterPtt++;

    uc_call_type = pvt_call.type;

    /* if (debounceCounterPtt >= debounceDelayPtt)
    { */

     //   debounceCounterPtt = 0;

        /*read_from_io(MIC_PTT_SW);
        ptt_return_value = pin_status;

        if (ptt_return_value == 0) // if button is pressed or  pin is set
        {
            ptt_pressed_flag = SET;

             if (uc_call_type == GLOBAL || uc_call_type == P2P || uc_call_type == ZONE)
                {

/*
                    g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = PTT_KEY;
                    g_ucDialed_Sequence_No++;
                    g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = PTT_KEY;

                    if (g_iarcDialed_Key_Array[0] > 0)
                    snprintf(call_type, FOUR, "%c", (char)g_iarcDialed_Key_Array[0]); //-'0');
                sprintf(buf, "%s%d%d%d%d", call_type, g_iarcDialed_Key_Array[1], g_iarcDialed_Key_Array[2],
                        g_iarcDialed_Key_Array[3], g_iarcDialed_Key_Array[4], g_iarcDialed_Key_Array[5]);
                // printf("------------key_buf_is----------%s \n",buf);

                memcpy(g_cReceived_key_seq, buf, sizeof(buf));
                g_uiFsu_event = VALID_KEY_SEQ_EVENT;

                clear_validation_processing();

 */
               /* select_codec(PVT_CHANNEL, HANDSET);
                printf("Handset on cradle \n");

                }
                else if (pg_call.type == GLOBAL_ALARM || pg_call.type == ZONE_ALARM)
                {
                    printf("ALARM CALL ACTIVE BN*****---------******");
                }

                else
                {
                    /* if (icom_contributor_status_info[pvt_call.curr_icom_no].cont1 == CLEAR ||
                    icom_contributor_status_info[pvt_call.curr_icom_no].cont2 == CLEAR)
                {
                    g_uiSpeech_detection_timer = SPEECH_DETECTION_TIMEOUT;
                    printf("IN SPEECH send \n");
                    g_uiVoipapp_wait_timer = SERVER_RESPONSE_TIMEOUT;
                    send_req_msg_to_server(0, ICOM_PTT_SEND_REQ, ICOM_CALL_FRAME_LENGTH, '1', '0');

                    g_uiFsu_state = RINGBACK_STATE;
                }
                else
                {
                    if (g_uiPlay_tone_timer == CLEAR)
                    {
                        acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_BUSY_TONE, START_TONE, CLEAR);
                        g_uiPlay_tone_timer = SIX * APP_TIMER_MULTIPLIER;
                    }
                } */

               /*  printf("GPIO PTT_KEY KEY PRESSED  \n");
                    g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = '0';
                    g_ucDialed_Sequence_No++;
                    g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = PTT_KEY;

                     g_uiFsu_event = VALID_KEY_SEQ_EVENT;

                }
               // debounce_flag_ptt = CLEAR;
           // }

        }
        else
        {
            /* if (g_uiSpeech_detection_timer > CLEAR)
            {
                printf("g_uiSpeech_detection_timer >CLEAR---\n");
                g_uiSpeech_detection_timer--;
            } */
           // printf("speech detection  alive  Timer =%d\n", g_uiSpeech_detection_timer);
            //if (debounce_flag_ptt == CLEAR)
            //{
               /*  // getTime();
                // g_uiFsu_event = PTT_RELEASE_EVENT;
                g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = RESET;
                g_ucDialed_Sequence_No++;
                g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = HANDSET_RESET_KEY;

                // printf("g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]-------%d\n",
                // g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]);

                printf("%d\n", g_iarcDialed_Key_Array[0]);
                if (g_iarcDialed_Key_Array[0] > 0)
                    snprintf(call_type, FOUR, "%c", (char)g_iarcDialed_Key_Array[0]); //-'0');
                sprintf(buf, "%s%d%d%d%d", call_type, g_iarcDialed_Key_Array[1], g_iarcDialed_Key_Array[2],
                        g_iarcDialed_Key_Array[3], g_iarcDialed_Key_Array[4], g_iarcDialed_Key_Array[5]);
                // printf("------------key_buf_is----------%s \n",buf);

                memcpy(g_cReceived_key_seq, buf, sizeof(buf));
                g_uiFsu_event = VALID_KEY_SEQ_EVENT;

                clear_validation_processing();
                printf("PTT release Event is set...\n"); */


                /* if (uc_call_type == GLOBAL || uc_call_type == P2P)
                {
                    select_codec(PVT_CHANNEL, NONE);

                }

                else if (pg_call.type == GLOBAL_ALARM || pg_call.type == ZONE_ALARM)
                {
                    printf("ALARM CALL ACTIVE DBN*****---------******");
                }

                else
                {
                    if(ptt_pressed_flag == SET)
                        g_uiFsu_event = PTT_RELEASE_EVENT;
                   // printf("PTT release Event is set in P-E-L...\n");

                 /* g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = RESET;
                g_ucDialed_Sequence_No++;
                g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = HANDSET_RESET_KEY;

                printf("%d\n", g_iarcDialed_Key_Array[0]);
                if (g_iarcDialed_Key_Array[0] > 0)
                    snprintf(call_type, FOUR, "%c", (char)g_iarcDialed_Key_Array[0]); //-'0');
                sprintf(buf, "%s%d%d%d%d", call_type, g_iarcDialed_Key_Array[1], g_iarcDialed_Key_Array[2],
                        g_iarcDialed_Key_Array[3], g_iarcDialed_Key_Array[4], g_iarcDialed_Key_Array[5]);
                // printf("------------key_buf_is----------%s \n",buf);

                memcpy(g_cReceived_key_seq, buf, sizeof(buf));
                g_uiFsu_event = VALID_KEY_SEQ_EVENT;

                clear_validation_processing(); */



                //}
              //  debounce_flag_ptt = SET;

          //  }


       // }
   // }
//}

/************************************************************************************
 * Function name: process_events_listen                                             *
 * Input arg    : void                                                              *
 * Output arg   : NONE                                                              *
 * Description  : This function uses the select function which will monitor all the *
 all file descriptors created by the application. Whenever there is*
 an activity on particular Fd it raises related fsu event.
* *************************************************************************************/

void process_events_listen()
{
    struct timeval timeout;
    int res, result, fd, ret;
    uint64_t expirations, diag_expirations;
    static i_pin_low_cntr, i_pin_high_cntr;
    int i_n, i_ret, sre_ret, i_ret1;
    char call_type[FOUR];
    unsigned char uc_call_type;
    app_testset = app_readset;
    timeout.tv_sec = 2;
    timeout.tv_usec = 500000;
    result = select(FD_SETSIZE, &app_testset, NULL, NULL, &timeout);
    // result=select(FD_SETSIZE,&app_testset,NULL,NULL,NULL);

    if (result < SET)
    {
        // perror(" app server1");
        return CLEAR;
    }

    for (fd = CLEAR; fd < MAX_APP_FDS; fd++)
    {
        if ((FD_ISSET(fd, &app_testset)))
        {
            if(fd == g_uiRadv_sock_fd)
            {
                //g_uiClilen = sizeof(g_cli_addr);
                g_uiClilen = sizeof(radv_sock_address);
                //g_uiClient_connection_fd = accept(g_uiRadv_sock_fd,CLEAR,CLEAR);
                g_uiClient_connection_fd=accept(g_uiRadv_sock_fd,(struct sockaddr *)&radv_sock_address,(socklen_t *)&g_uiClilen);
                if(g_uiClient_connection_fd<CLEAR)
                {
                    perror("pjsua accept failed\n");
                }

                FD_SET(g_uiClient_connection_fd,&app_readset);
                printf("adding server client on fd %d\n", g_uiClient_connection_fd);
                if (g_uiClient_connection_fd < CLEAR)
                {
                    perror("----scVOIPAPP ERROR on accept\n");
                }
            }
            // else
            // {
            if (fd == g_uiApp_timer_fd)
            {
                //  printf("inside timerfd++++++");

                //   scan_keyboard();

                // printf("++++scan column %c",read_column());
                // read_column();

                int i_res;
                i_res = read(g_uiApp_timer_fd, &expirations, sizeof(expirations));
                //  printf("in voipapp Timer fd  \n");
                if (i_res < CLEAR)
                {
                    perror("Server-read() error");
                    close(fd);
                    FD_CLR(fd, &app_readset);
                }

                else
                {

                    /***************Check ICOM contributor table is full*************/
                    if (g_iIcom_mem_dropped == SET && stn_config.icom_live_dead_status[stn_config.default_intercom - MIN_OFFSET] == LIVE)
                    {
                        if (pvt_call.type != INTERCOM_TX_RX)
                        {
                            if (icom_contributor_status_info[pvt_call.curr_icom_no].cont1 != CLEAR &&
                                icom_contributor_status_info[pvt_call.curr_icom_no].cont2 != CLEAR)
                            {
                                // operate_gpio(ICOM_STATUS,WRITE,GPIO_ON);
                                write_to_io(ICOM_LED_ON, IO_PIN_ON);
                                //  //operate_gpio(PFC_ACT,WRITE,GPIO_ON);
                            }
                            else
                            {
                                // operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);
                                write_to_io(ICOM_LED_OFF, IO_PIN_OFF);
                                //   //operate_gpio(PFC_ACT,WRITE,GPIO_OFF);
                            }
                        }
                    }

                    if (g_iInterdigit_timer)
                    {
                        g_iInterdigit_timer--;

                        if (g_iInterdigit_timer == CLEAR)
                        {
                            clear_validation_processing();
                        }
                    }

                    /******PLAY TONE TIMER****************/
                    if (g_uiPlay_tone_timer)
                    {
                        g_uiPlay_tone_timer--;
                        if (g_uiPlay_tone_timer == CLEAR)
                        {
                            acgPlayTone_phi(CLEAR, VOIP_SIGNAL_STUTTER_TONE, STOP_TONE, CLEAR);
                            set_volume(PG_CHANNEL, MAX_MUSIC_VOLUME);
                        }
                    }

                    /******SERVER LIVE STATUS TIMER****************/
                    if (g_uiServer_live_status_timer)
                    {
                        g_uiServer_live_status_timer--;
                        //  printf("live_status_timer=%d....\n",g_uiServer_live_status_timer);
                        if (g_uiServer_live_status_timer == CLEAR)
                        {
                            printf("g_uiServer_live_status_timer is 0....\n");
                            getTime();
                            // g_uiFsu_event=WAKE_UP_EVENT;
                            g_uiFsu_timer_event = BROADCAST_TIMEOUT_EVENT;

                            // operate_gpio(LED_LAN_SERV,WRITE,GPIO_OFF);
                            write_to_io(LED_SERVER_IND_OFF, IO_PIN_OFF);
                            printf("server off LED_SERVER_IND_OFF 0x%x------\n",LED_SERVER_IND_OFF);
                        }
                    }
                    /******KEEP ALIVE TIMER****************/
                    if (g_uiKeep_alive_timer)
                    {
                        g_uiKeep_alive_timer--;
                        // printf("KEEP alive  TImer =%d\n",g_uiKeep_alive_timer);
                        if (g_uiKeep_alive_timer == CLEAR)
                        {
                            printf("keep alive frame is send to server....\n");
                            getTime();
                            // send_req_msg_to_server(g_uiFsu_state,KEEP_ALIVE_FRAME,KEEP_ALIVE_FRAME_LENGTH,'0','0');

                            send_req_msg_to_server(g_uiFsu_state, KEEP_ALIVE_FRAME, KEEP_ALIVE_FRAME_LENGTH, '0', '0');
                            g_uiKeep_alive_timer = KEEP_ALIVE_TIMEOUT;
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
                    if (pvt_call.type == INTERCOM_TX_RX)
                    {
                        printf("*****In pvt_call.type==INTERCOM_TX_RX********\n");

                        if (g_uiIcom_led_on_flag == SET)
                        {

                            // operate_gpio(ICOM_STATUS,WRITE,GPIO_ON);
                            write_to_io(ICOM_LED_ON, IO_PIN_ON);
                            one_second_timeout--;

                            if (one_second_timeout == 0)
                            {
                                g_uiIcom_led_on_flag = CLEAR;

                                write_to_io(ICOM_LED_ON, IO_PIN_ON);
                                // operate_gpio(PFC_ACT,WRITE,GPIO_ON);

                                one_second_timeout = 50;
                            }
                            write_to_io(PFC_ACT_ON, IO_PIN_ON);
                            printf("******In g_uiIcom_led_on_flag==SET**********");
                        }
                        else
                        {
                            printf("********In g_uiIcom_led_on_flag=CLEAR\n***********");
                            // operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);
                            write_to_io(ICOM_LED_OFF, IO_PIN_OFF);
                            one_second_timeout--;

                            if (one_second_timeout == 0)
                            {
                                g_uiIcom_led_on_flag = SET;

                                write_to_io(ICOM_LED_OFF, IO_PIN_OFF);
                                one_second_timeout = 50;
                            }
                            // operate_gpio(PFC_ACT,WRITE,GPIO_OFF);
                            write_to_io(PFC_ACT_OFF, IO_PIN_OFF);
                        }
                    }

                    pthread_mutex_unlock(&lock_call_type);

                    /******ICOM LED BLINK TIMER FOR GROUP CALL****************/
                    pthread_mutex_lock(&lock_call_type);
                    // if(pWork_m.private_state == PRIVATE_OUTGOING_ONGOING)
                    // {
                    if ((pvt_call.type == GROUP_TX_RX))
                    // if((pvt_call.type==GROUP))
                    {
                        if (g_uiIcom_led_on_flag == SET)
                        {
                            g_uiIcom_led_on_flag = CLEAR;
                            // operate_gpio(ICOM_STATUS,WRITE,GPIO_ON);
                            write_to_io(ICOM_LED_ON, IO_PIN_ON);
                            // operate_gpio(PFC_ACT,WRITE,GPIO_ON);
                            write_to_io(PFC_ACT_ON, IO_PIN_ON);
                        }
                        else
                        {
                            // operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);
                            write_to_io(ICOM_LED_OFF, IO_PIN_OFF);
                            // operate_gpio(PFC_ACT,WRITE,GPIO_OFF);
                            write_to_io(PFC_ACT_OFF, IO_PIN_OFF);
                            g_uiIcom_led_on_flag = SET;
                        }
                    }

                    //}

                    pthread_mutex_unlock(&lock_call_type);

                    /******P2P LED BLINK TIMER****************/
                    /*pthread_mutex_lock(&lock_call_type);
                    if (p2p_call_process_timer_flag == SET)
                    {

                         if (g_uiP2p_led_on_flag == SET)
                        {
                            g_uiP2p_led_on_flag = CLEAR;
                            // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_ON);
                            //write_to_io(CALL_THROUGH_LED_ON , IO_PIN_ON);
                            ////operate_gpio(PFC_ACT,WRITE,GPIO_ON);

                            printf("******In g_uiP2p_led_on_flag=SET**********");
                        }
                        else
                        {
                            printf("********In g_uiP2p_led_on_flag=CLEAR\n***********");
                            // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                            write_to_io(CALL_THROUGH_LED_OFF , IO_PIN_OFF);
                            ////operate_gpio(PFC_ACT,WRITE,GPIO_OFF);

                            g_uiP2p_led_on_flag = SET;
                        }
                    }
                    pthread_mutex_unlock(&lock_call_type);*/

                    /******VOIP APP WAIT TIMER****************/
                    if (g_uiVoipapp_wait_timer)
                    {
                        g_uiVoipapp_wait_timer--;
                        printf("voip app timer=%d \n", g_uiVoipapp_wait_timer);
                        if (g_uiVoipapp_wait_timer == CLEAR)
                        {
                            g_uiFsu_event = APP_TIMEOUT_EVENT;
                        }
                    }

                  /*  if (g_ui_sip_p2p_timer)
                    {
                        g_ui_sip_p2p_timer--;
                        printf(" g_ui_sip_p2p_timer timer=%d \n", g_ui_sip_p2p_timer);

                    }*/





                    /******PVT CALL TIMER****************/

                    if (pvt_call.pvt_call_busy)
                    {

                        if (g_uiPvt_call_timer_ui_lcd) // g_uiPvt_call_timer_ui_lcd
                        {

                            g_uiPvt_call_time_lcd = (g_uiPvt_call_timer_ui_lcd / 50);

                            if (g_uiPvt_call_timer_ui_lcd % 50 == 0)
                            {

                                PvtCallTime_min = (g_uiPvt_call_time_lcd / 60);

                                PvtCallTime_sec = (g_uiPvt_call_time_lcd % 60);

                                if (PvtCallTime_sec > NINE)
                                {
                                    sprintf(&g_arcPvtCallTimer[ZERO], "%d", 0);
                                    sprintf(&g_arcPvtCallTimer[ONE], "%d", PvtCallTime_min);
                                    sprintf(&g_arcPvtCallTimer[TWO], "%s", ":");
                                    sprintf(&g_arcPvtCallTimer[THREE], "%d", PvtCallTime_sec);
                                    print_msg_on_lcd(IOCTL_PRINT, g_arcPvtCallTimer, LINE2, ELEVEN, NO_LCD_CLR);
                                }
                                else
                                {
                                    sprintf(&g_arcPvtCallTimer[ZERO], "%d", 0);
                                    sprintf(&g_arcPvtCallTimer[ONE], "%d", PvtCallTime_min);
                                    sprintf(&g_arcPvtCallTimer[TWO], "%s", ":");
                                    sprintf(&g_arcPvtCallTimer[THREE], "%d", 0);
                                    sprintf(&g_arcPvtCallTimer[FOUR], "%d", PvtCallTime_sec);
                                    print_msg_on_lcd(IOCTL_PRINT, g_arcPvtCallTimer, LINE2, ELEVEN, NO_LCD_CLR);
                                }
                            }
                            g_uiPvt_call_timer_ui_lcd--;
                        }

                        if (g_uiPage_call_timer_lcd)
                        {
                            // g_uiPage_call_timer_frlcd_ini
                            g_uipg_call_time_lcd_init = (g_uiPage_call_timer_lcd / 50);

                            if (g_uiPage_call_timer_lcd % 50 == 0)
                            {

                                PgCallIniTime_min = (g_uipg_call_time_lcd_init / 60);
                                PgCallIniTime_sec = (g_uipg_call_time_lcd_init % 60);
                                if (PgCallIniTime_sec > NINE)
                                {
                                    sprintf(&g_arcPvtCallTimer_fr_pg_init[ZERO], "%d", 0);
                                    sprintf(&g_arcPvtCallTimer_fr_pg_init[ONE], "%d", PgCallIniTime_min);
                                    sprintf(&g_arcPvtCallTimer_fr_pg_init[TWO], "%s", ":");
                                    sprintf(&g_arcPvtCallTimer_fr_pg_init[THREE], "%d", PgCallIniTime_sec);
                                    print_msg_on_lcd(IOCTL_PRINT, g_arcPvtCallTimer_fr_pg_init, LINE1, ELEVEN, NO_LCD_CLR);
                                }
                                else
                                {
                                    sprintf(&g_arcPvtCallTimer_fr_pg_init[ZERO], "%d", 0);
                                    sprintf(&g_arcPvtCallTimer_fr_pg_init[ONE], "%d", PgCallIniTime_min);
                                    sprintf(&g_arcPvtCallTimer_fr_pg_init[TWO], "%s", ":");
                                    sprintf(&g_arcPvtCallTimer_fr_pg_init[THREE], "%d", 0);
                                    sprintf(&g_arcPvtCallTimer_fr_pg_init[FOUR], "%d", PgCallIniTime_sec);
                                    print_msg_on_lcd(IOCTL_PRINT, g_arcPvtCallTimer_fr_pg_init, LINE1, ELEVEN, NO_LCD_CLR);
                                }
                            }
                            g_uiPage_call_timer_lcd--;
                        }
                    }

                    /******PAGE CALL TIMER****************/
                    if (pg_call.pg_call_busy)
                    {
                        if (g_uiPage_call_timer_lcd)
                        {

                            g_uipg_call_time_lcd = (g_uiPage_call_timer_lcd / 50);

                            if (g_uiPage_call_timer_lcd % 50 == 0)
                            {

                                printf("g_uipg_call_time_lcd: for PG RECEIVER %d \n", g_uipg_call_time_lcd);

                                PgCallTime_min = (g_uipg_call_time_lcd / 60);
                                PgCallTime_sec = (g_uipg_call_time_lcd % 60);
                                if (PgCallTime_sec > NINE)
                                {
                                    sprintf(&g_arcPgCallTimer[ZERO], "%d", 0);
                                    sprintf(&g_arcPgCallTimer[ONE], "%d", PgCallTime_min);
                                    sprintf(&g_arcPgCallTimer[TWO], "%s", ":");
                                    sprintf(&g_arcPgCallTimer[THREE], "%d", PgCallTime_sec);
                                    print_msg_on_lcd(IOCTL_PRINT, g_arcPgCallTimer, LINE1, ELEVEN, NO_LCD_CLR);
                                }
                                else
                                {
                                    sprintf(&g_arcPgCallTimer[ZERO], "%d", 0);
                                    sprintf(&g_arcPgCallTimer[ONE], "%d", PgCallTime_min);
                                    sprintf(&g_arcPgCallTimer[TWO], "%s", ":");
                                    sprintf(&g_arcPgCallTimer[THREE], "%d", 0);
                                    sprintf(&g_arcPgCallTimer[FOUR], "%d", PgCallTime_sec);
                                    print_msg_on_lcd(IOCTL_PRINT, g_arcPgCallTimer, LINE1, ELEVEN, NO_LCD_CLR);
                                }
                            }
                            g_uiPage_call_timer_lcd--;

                            printf("g_uiPage_call_timer is : %d \n", g_uiPage_call_timer);

                            if (g_uiPage_call_timer == CLEAR)
                            {
                                g_uiFsu_event = PG_CALL_TIMEOUT;
                            }
                        }
                    }
                }

                /******************************HOOK_SW_SENSE_22_may_2023***********************/

                // pthread_mutex_lock(&lock_call_type);
               uc_call_type = pvt_call.type;
                // pthread_mutex_unlock(&lock_call_type);

                if (pvt_call.pvt_call_busy == SET || uc_call_type == INTERCOM_RX || uc_call_type == INTERCOM_TX_RX)

                {
            if(station_type ==  HANDSET_STN)
                        {
                        write_to_io(PVT_AMP_MUTE_OFF, IO_PIN_OFF);
                   // i_ret=operate_gpio(HOOK_SW_SENSE,READ,CLEAR);
                    read_from_io(HOOK_SW_SENSE);

                    i_ret = pin_status;
                  // printf("HOOK_SW_SENSE pin_status::: %d\n",i_ret);
                    if (i_ret == CLEAR)
                    {

                        write_to_io(ACT_PVT_PG_MIX_ON, IO_PIN_ON);
                        write_to_diag_io(MIC_MUTE_ON, IO_PIN_ON);
                         write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);


                        //printf("i_ret-----------%d\n", i_ret);
                        if (g_iHandset_lifted_signal == SET)
                        {
                            //operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_OFF);


                            g_iHandset_lifted_signal = CLEAR;
                            switch (uc_call_type)
                            {
                            case INTERCOM_RX:
                            case INTERCOM_TX_RX:
                            case GLOBAL:
                            case ZONE:
                            case GROUP:
                            case GROUP_TX_RX:
                            case P2P:
                            case CONFERENCE:

                            g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = RESET;
                            g_ucDialed_Sequence_No++;
                            g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = HANDSET_RESET_KEY;

                                // g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]);

                            printf("%d\n", g_iarcDialed_Key_Array[0]);
                            if (g_iarcDialed_Key_Array[0] > 0)
                            snprintf(call_type, FOUR, "%c", (char)g_iarcDialed_Key_Array[0]); //-'0');
                            sprintf(buf, "%s%d%d%d%d", call_type, g_iarcDialed_Key_Array[1], g_iarcDialed_Key_Array[2],
                            g_iarcDialed_Key_Array[3], g_iarcDialed_Key_Array[4], g_iarcDialed_Key_Array[5]);
                                // printf("------------key_buf_is----------%s \n",buf);

                            memcpy(g_cReceived_key_seq, buf, sizeof(buf));
                            g_uiFsu_event = VALID_KEY_SEQ_EVENT;

                            clear_validation_processing();
                            //printf("Handset on cradle \n");

                                break;
                            }
                        }
                    }

                    else // handset lifted
                    {

                            g_iHandset_lifted_signal = SET;

                            write_to_io(ACT_PVT_PG_MIX_OFF, IO_PIN_OFF);
                            write_to_diag_io(MIC_MUTE_OFF, IO_PIN_OFF);

                         /*    if( uc_call_type == INTERCOM_RX ) // IDLE INTERCOM RECEIVING CONDITION I.e., NOT A CONTRIBUTOR
                            {

                                 write_to_diag_io(MIC_MUTE_ON, IO_PIN_ON);

                            }
                            else
                            {
                                write_to_diag_io(MIC_MUTE_OFF, IO_PIN_OFF); // IF THE STATION IS AN INTERCOM CONTRIBUTOR

                            } */

                       //  write_to_diag_io(MIC_MUTE_OFF, IO_PIN_OFF);
                        if (pg_call.pg_call_busy != SET && stn_config.default_AIM == CLEAR)
                        {
                            // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF); ///on means page speaker mute
                            write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);

                        }
                        else
                        {
                            write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);

                        }
                    }
                }

            }

                /***********SRE_MUTE_ACTIVATION_23_MAY_2023****************************/

                /*  if(pWork_m.private_state==PRIVATE_OUTGOING_ONGOING)
                  {
                      printf("PRIVATE_OUTGOING_ONGOING in main---------\n");
                      if(pvt_call.type==INTERCOM_TX_RX || pvt_call.type == GROUP_TX_RX)

                      {*/
                // i_ret1=operate_gpio(PTT_SPEECH,READ,CLEAR);

                /*  read_from_io(SPEECH_DETECT); ///
                 i_ret1 = pin_status;
                 if (i_ret1 == CLEAR)
                 {

                     if (icom_contributor_status_info[pvt_call.curr_icom_no].cont1 == CLEAR ||
                         icom_contributor_status_info[pvt_call.curr_icom_no].cont2 == CLEAR)
                     {
                         printf("IN SPEECH send \n");
                         g_uiVoipapp_wait_timer = SERVER_RESPONSE_TIMEOUT;
                         send_req_msg_to_server(0, ICOM_PTT_SEND_REQ, ICOM_CALL_FRAME_LENGTH, '1', '0');
                         // send_msg_ui(P2P_OFF_HOOK_STATE,LCD,LINE2,CONTINUE_DISPLAY, g_cLine2_buf,LCD_CLR_SECOND_LINE,0);
                         // send_msg_ui(RINGBACK_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,CLEAR);
                         g_uiFsu_state = RINGBACK_STATE;
                     }
                     else
                     {
                         if (g_uiPlay_tone_timer == CLEAR)
                         {
                             acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_BUSY_TONE, START_TONE, CLEAR);
                             g_uiPlay_tone_timer = SIX * APP_TIMER_MULTIPLIER;
                         }
                     }

                     /*  if(g_uiSpeech_detection_timer >CLEAR)
                      {
                          printf("g_uiSpeech_detection_timer >CLEAR---\n");
                          g_uiSpeech_detection_timer--;
                      }
                      printf("speech detection  alive  Timer =%d\n",g_uiSpeech_detection_timer);
                      if(g_uiSpeech_detection_timer==CLEAR)
                      {
                          getTime();
                          g_uiFsu_event = PTT_RELEASE_EVENT;
                          printf("PTT release Event is set...\n");
                      }
                 } */
                /*else
                {
                     g_uiSpeech_detection_timer = SPEECH_DETECTION_TIMEOUT;

                } */
                //   }
                //}
/*********************************************SPEECH_DETECT************************************************************/

                 if(pWork_m.private_state==PRIVATE_OUTGOING_ONGOING)
                {
                   // printf("inside speech_detect---------\n");
                    if(pvt_call.type==INTERCOM_TX_RX || pvt_call.type == GROUP_TX_RX)
                     // if(pvt_call.type==INTERCOM_TX_RX || pvt_call.type == GROUP)
                    {
                        //printf("inside 2nd if of speech_detect-------\n");
                        //i_ret1=operate_gpio(PTT_SPEECH,READ,CLEAR);
                        //i_ret1= read_from_io(SPEECH_DETECT);///
                        read_from_io(SPEECH_DETECT);
                        i_ret1 = pin_status;
                        printf("i_ret1-----------------:0x%x\n",i_ret1);
                        if(i_ret1 == CLEAR)
                        {
                             //printf("inside i_ret1--------%d\n ");
                            if(g_uiSpeech_detection_timer >CLEAR)
                            {
                                g_uiSpeech_detection_timer--;
                            }
                           // printf("speech detection  alive  Timer =%d\n",g_uiSpeech_detection_timer);
                            if(g_uiSpeech_detection_timer==CLEAR)
                            {

                                g_uiFsu_event = PTT_RELEASE_EVENT;
                                printf("PTT release Event is set...\n");
                            }
                        }
                        else
                        {
                            // printf("inside else of speech_detect------\n");
                             g_uiSpeech_detection_timer = SPEECH_DETECTION_TIMEOUT;

                        }
                    }
                }

                               if(station_type == GOOSENECK_STN)
                                {
                                    write_to_diag_io(MIC_MUTE_OFF, IO_PIN_OFF);
                                    //printf("Gooseneck station mic mute off --------------\n");
                                    if(stn_config.icom_live_dead_status[stn_config.default_intercom-MIN_OFFSET]==LIVE)
                                    {
                                     write_to_io(PVT_AMP_MUTE_OFF, IO_PIN_OFF);
                                     
                                    }
                                    
                                }




 //printf("FSU State is %c\n",g_uiFsu_state);


    toggle_sre_using_gpio();
    operate_mic_ptt_sw();





                /****************** KEYBOARD SCANNING ******************************/

                keyboard_scan_in_isr(g_uiFsu_state);

            } // if timer_fd*/

            if (fd == g_uiApp_diag_timer_fd)
            {
                // printf("+++++diagnostic timer");
                int i_result;
                i_result = read(g_uiApp_diag_timer_fd, &diag_expirations, sizeof(diag_expirations));
                if (i_result < CLEAR)
                {
                    perror("Server-read() error");
                    close(fd);
                    FD_CLR(fd, &app_readset);
                }
                else
                {
                    if (g_uiDiagnostic_start_flag)
                    {
                        if (g_uiPvt_diagnostic_timer)
                        {
                            g_uiPvt_diagnostic_timer--;
                            // i_n=//operate_gpio(PVT_RX_CHK,READ,CLEAR);

                            if (i_n == CLEAR)
                            {
                                if (i_pin_low_cntr < DIAGNOSTIC_PIN_CHECK_CNTR)
                                    i_pin_low_cntr++;
                                printf("pvt diagnostic  low counter=%d\n", i_pin_low_cntr);
                            }
                            if (i_n == 1)
                            {
                                if (i_pin_high_cntr < DIAGNOSTIC_PIN_CHECK_CNTR)
                                    i_pin_high_cntr++;
                                printf("pvt diagnostic  high counter=%d\n", i_pin_high_cntr);
                            }

                            if (i_pin_high_cntr == DIAGNOSTIC_PIN_CHECK_CNTR && i_pin_low_cntr ==
                                                                                    DIAGNOSTIC_PIN_CHECK_CNTR)
                            {
                                i_pin_low_cntr = CLEAR;
                                i_pin_high_cntr = CLEAR;
                                g_uiPvt_diagnostic_timer = CLEAR;
                                g_uiFsu_event = DIAGNOSTIC_PVT_OK;
                            }
                            else if (g_uiPvt_diagnostic_timer == CLEAR && (i_pin_high_cntr != CLEAR || i_pin_low_cntr !=
                                                                                                           CLEAR))
                            {
                                i_pin_low_cntr = CLEAR;
                                i_pin_high_cntr = CLEAR;
                                g_uiFsu_event = DIAGNOSTIC_PVT_NOT_OK;
                            }
                        }

                        if (g_uiSpk_diagnostic_timer)
                        {
                            g_uiSpk_diagnostic_timer--;
                            // i_n=//operate_gpio(PAGE_RX_CHK,READ,CLEAR);

                            if (i_n == CLEAR)
                            {
                                if (i_pin_low_cntr < DIAGNOSTIC_PIN_CHECK_CNTR)
                                    i_pin_low_cntr++;
                                printf("spk diagnostic low counter=%d\n", i_pin_low_cntr);
                            }
                            else
                            {
                                if (i_pin_high_cntr < DIAGNOSTIC_PIN_CHECK_CNTR)
                                    i_pin_high_cntr++;
                                printf("spk diagnostic high counter=%d\n", i_pin_high_cntr);
                            }

                            if (i_pin_high_cntr == DIAGNOSTIC_PIN_CHECK_CNTR &&
                                i_pin_low_cntr == DIAGNOSTIC_PIN_CHECK_CNTR)
                            {
                                i_pin_low_cntr = CLEAR;
                                i_pin_high_cntr = CLEAR;
                                g_uiSpk_diagnostic_timer = CLEAR;
                                g_uiFsu_event = DIAGNOSTIC_SPK_OK;
                            }

                            if (g_uiSpk_diagnostic_timer == CLEAR && (i_pin_high_cntr != CLEAR || i_pin_low_cntr != CLEAR))
                            {
                                i_pin_low_cntr = CLEAR;
                                i_pin_high_cntr = CLEAR;
                                g_uiFsu_event = DIAGNOSTIC_SPK_NOT_OK;
                            }
                        }
                    }
                }
            }

            /*    if (fd == g_uiUi_socket_fd)
                {
                    char c_msgbuf[UI_MSG_LENGTH];
                    int i_nbytes;
                    printf("in UI SOCKET fd  \n");
                    g_uiApp_ui_addrlen = sizeof(g_app_ui_addr);
                    i_nbytes = recv(g_uiUi_socket_fd, c_msgbuf, MAX_ETH_FRAME_LENGTH, CLEAR);
                    if (i_nbytes > 0)
                    {
                        memcpy(g_cReceived_key_seq, c_msgbuf, sizeof(c_msgbuf));
                    }
                    else
                    {
                        FD_CLR(fd, &app_readset);
                        printf("PTT failed \n");
                    }

                   // g_uiFsu_event = VALID_KEY_SEQ_EVENT;
                   // printf("msgbuf %s\n", c_msgbuf);

                } // g_uiUi_socket_fd
    */
            if (fd == g_uiBroadcast_fd)
            {
                int i_n;
                char c_buf[MAX_ETH_FRAME_LENGTH];
                char c_Buffer[MAX_ETH_FRAME_LENGTH];
                int i_len = CLEAR;
                unsigned int ui_Index = CLEAR;
                unsigned int ui_cmd = CLEAR;
                i_n = recvfrom(g_uiBroadcast_fd, c_buf, MAX_ETH_FRAME_LENGTH, CLEAR,
                               (struct sockaddr *)&reg_addr, &g_uiReg_addrlen);
                printf("Broadcast frame is received....\n");
                if (i_n < CLEAR)
                {
                    error("recvfrom");
                }
                i_len = validate_frame(c_buf);
                memcpy(c_Buffer, c_buf + (PRTCL_HDR_LEN + 2), i_len);
                //.
                if (i_len > 1)
                {

                    ui_cmd = *(c_Buffer)&0xFF | *(c_Buffer + 1) << 8 & 0xFF00;

                    if (ui_cmd == SERVER_REG_BCAST)
                    {
                        // operate_gpio(LED_LAN_SERV,WRITE,GPIO_ON);

                        g_uiServer_live_status_timer = SERVER_LIVE_TIMEOUT;
                        printf("SERVER_REG_BCAST ********* cmd_id = %d,%d\n", ui_cmd, g_uiServer_live_status_timer);
                        getTime();
                        // gpio_lan_led();
                        write_to_io(LED_SERVER_IND_ON, IO_PIN_ON);
                    }

                    // parse_commands(ui_cmd,c_Buffer);
                    parse_broadcast_commands(ui_cmd, c_Buffer);
                }
            }

            if (fd == g_uiMulticast_signalling_fd)
            {
                printf("multicast frame received .....\n");
                int i_n;
                char c_buf[MAX_ETH_FRAME_LENGTH];
                char c_Buffer[MAX_ETH_FRAME_LENGTH];
                int i_len = CLEAR;
                unsigned int ui_Index = CLEAR;
                unsigned int ui_cmd = CLEAR;
                i_n = recvfrom(g_uiMulticast_signalling_fd, c_buf, MAX_ETH_FRAME_LENGTH, 0,
                               (struct sockaddr *)&mul_sig_addr, &g_uiMul_sig_addrlen);
                printf(">>>>>>>>>>>>>>>>>>>c_buf is %s \n", c_buf);
                i_len = validate_frame(c_buf);

                if (i_len > 1)
                {
                    memcpy(c_Buffer, c_buf + (PRTCL_HDR_LEN + 2), i_len);
                    if (i_n == -1)
                    {
                        printf("Multicast ERROR, Protocol Header Incorrect\n");
                    }
                    else
                    {
                        printf("Multicast Protocol Header OK\n");
                        ui_cmd = *(c_Buffer)&0xFF | *(c_Buffer + 1) << 8 & 0xFF00;
                        printf("Multicast cmd is=%i frame is %s\n", ui_cmd, c_buf);
                        parse_multicast_commands(ui_cmd, c_Buffer);
                    }
                }
            }

            if (fd == g_uiServ_udp_comm_fd)
            {
                int i_n;
                char c_buf[MAX_ETH_FRAME_LENGTH];
                char c_Buf[10];
                int i_len = CLEAR;
                unsigned int Index = CLEAR;
                unsigned int cmd = CLEAR;
                printf("in udp server communication fd \n");
                i_n = recvfrom(g_uiServ_udp_comm_fd, c_buf, MAX_ETH_FRAME_LENGTH, CLEAR,
                               (struct sockaddr *)&udp_comm_addr, &g_uiUdp_comm_addrlen);
                if (i_n < CLEAR)
                {
                    error("recvfrom");
                }
                printf("buf is %s\n", c_buf);
                sprintf(c_Buf, "%c%c", c_buf[9], c_buf[10]);
                i_len = validate_frame(c_buf);
                cmd = atoi(c_Buf);
                printf("udp cmd is.............%i %d \n", cmd, i_len);
                parse_commands(cmd, c_buf);
            }

            /* if (fd == g_uiClient_connection_fd)
            {
                int i_n;
                char c_Buffer[50];

                i_n = recv(g_uiClient_connection_fd, c_Buffer, MAX_ETH_FRAME_LENGTH, 0);
                printf("recived packet form Radvision %d %s \n", i_n, c_Buffer);

                if (strstr(c_Buffer, "SIP_REGISTERED"))
                {
                    printf("fsu event sip registed \n");
                    // g_cSip_ok_flag = SET;
                    g_uiFsu_event = VALID_SIP_REGISTER;
                }

                if (strstr(c_Buffer, "RINGING_STATE"))
                {
                    g_uiFsu_event = RINGING_OFF_HOOK_EVENT;
                    printf("fsu event OFF_HOOK_EVENT \n");
                }

                if (strstr(c_Buffer, "CALL_ANSWERED"))
                {
                    g_cCaller[0] = c_Buffer[13];
                    g_cCaller[1] = c_Buffer[14];
                    g_cCaller[2] = c_Buffer[15];
                    g_uiFsu_event = P2P_CALL_EVENT;
                    printf("fsu event P2P CALL \n");
                }

                if (strstr(c_Buffer, "CALL_OFF_HOOK_PASS"))
                {
                    g_uiFsu_event = CALL_OFF_HOOK_EVENT;
                    printf("fsu event call hook CALL \n");
                }

                if (strstr(c_Buffer, "TERMINATED_FOR_CONF_CALL"))
                {
                    g_uiFsu_event = CONF_ADD_EVENT;
                    printf("fsu event CONF ADD EVENT \n");
                }
                if (strstr(c_Buffer, "REMOTE_DISCONNECTED"))
                {
                    if (g_uiFsu_state == P2P_STATE || g_uiFsu_state == RINGBACK_STATE || g_uiFsu_state == CONFERENCE_STATE)
                    {
                        g_uiFsu_event = P2P_RESET_EVENT;
                        printf("fsu event Reset CALL \n");
                        getTime();
                    }
                }

                if (strstr(c_Buffer, "LOCAL_DISCONNECTED"))
                {
                    if (g_uiFsu_state == P2P_RESET_RINGBACK_STATE || g_uiFsu_state == CONF_RESET_RINGBACK_STATE)
                    {
                        g_uiFsu_event = P2P_RESET_EVENT;
                        printf("fsu event Reset CALL \n");
                        getTime();
                    }
                }
            } */

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

            //  } //else of fdset
        }
    } // for loop
}

/************************************************************************************
 * Function name: send_msg_ui                                                       *
 * Input arg    : char state char para,short line_no,short cmd,char* text short flash_cntr *
 * Output arg   : NONE                                                               *
 * Description  : This function used to keep updating UI thread about the FSU states,*
 *                 and also used to UI thread message to be displayed on the LCD      *
 *************************************************************************************/

void send_msg_ui(char state, char para, short int line_no, short int cmd, char *text, short int flash_cntr, int position)
/*{
    int i_res;
    char c_msg[2];
    bzero(c_msg, sizeof(c_msg));
    memcpy(c_msg,&state,sizeof(char));
    i_res=write(g_uiUi_socket_fd, c_msg, sizeof(char));
}

// void send_msg_ui(char state,char para, short int line_no,short int cmd,char *text,short int flash_cntr,int position)*/
{
    int i_res;
    memset(&ui_msg, 0, sizeof(ui_msg));

    strcpy(ui_msg.text, text);
    ui_msg.parameter = para;
    ui_msg.line_no = line_no;
    ui_msg.cmd = cmd;
    ui_msg.state = state;
    // ui_msg.flash_cntr = flash_cntr;
    ui_msg.lcd_clear_bit = flash_cntr;
    ui_msg.position = position;

    // ui_msg.position = position;

    char c_msg[2];
    bzero(c_msg, sizeof(c_msg));
    memcpy(c_msg, &state, sizeof(char));

    // i_res=write(g_uiUi_socket_fd, c_msg, sizeof(char));

    i_res = send(g_uiUi_socket_fd, &ui_msg, sizeof(struct UI_msg), 0);
    if (i_res == -1)
    {
        printf(" send() failed in send_msg_ui\n");
        return;
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

    g_uiApp_timer_fd = timerfd_create(CLOCK_MONOTONIC, CLEAR);

    if (g_uiApp_timer_fd < CLEAR)
    {
        perror("app_timerfd create:");
    }
    FD_SET(g_uiApp_timer_fd, &app_readset);
    //  printf("app timerfd created \n");
    bzero(&app_timspec, sizeof(app_timspec));

    app_timspec.it_interval.tv_sec = CLEAR;     // msg->attr;
    app_timspec.it_interval.tv_nsec = 20000000; // 20ms timer count

    app_timspec.it_value.tv_sec = CLEAR; // msg->attr;
    app_timspec.it_value.tv_nsec = 20000000;

    i_res = timerfd_settime(g_uiApp_timer_fd, CLEAR, &app_timspec, CLEAR);

    if (i_res < CLEAR)
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

    g_uiApp_diag_timer_fd = timerfd_create(CLOCK_MONOTONIC, CLEAR);
    if (g_uiApp_timer_fd < CLEAR)
    {
        perror("app_timerfd create:");
    }
    FD_SET(g_uiApp_diag_timer_fd, &app_readset); ///
    //  printf("app timerfd created \n");
    bzero(&app_timspec, sizeof(app_timspec));
    app_timspec.it_interval.tv_sec = CLEAR;     // msg->attr;
    app_timspec.it_interval.tv_nsec = 20000000; // 20 ms timer count

    app_timspec.it_value.tv_sec = CLEAR; // msg->attr;
    app_timspec.it_value.tv_nsec = 20000000;

    i_res = timerfd_settime(g_uiApp_diag_timer_fd, 0, &app_timspec, 0);

    if (i_res < CLEAR)
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
/*void UI_socket_create()
{
    char uc_socket_name[TEN] = {"UI"};

    printf("UI create socket \n");

    g_uiUi_socket_fd = socket(PF_UNIX, SOCK_STREAM, CLEAR);

    printf("+++++++++++++++UI CHECK create socket %d \n", g_uiUi_socket_fd);
    if (g_uiUi_socket_fd < CLEAR)
    {
        perror("ui socket failed\n");
    }
    else
    {
        printf("socket success!!!!!!!!\n");
    }

    g_app_ui_addr.sun_family = AF_UNIX;
    g_uiApp_ui_addrlen = sizeof(g_app_ui_addr.sun_family) +
                         sprintf(g_app_ui_addr.sun_path, "%s", uc_socket_name);

    if (connect(g_uiUi_socket_fd, (struct sockaddr *)&g_app_ui_addr, g_uiApp_ui_addrlen) != CLEAR)
    {
        perror("g_uiUi_socket_fd Connect Error= %s !!!!!!!!!! \n");
    }
    else
    {
        printf("connect succes \n");
    }
    FD_SET(g_uiUi_socket_fd, &app_readset);
}
*/
/*************************************************************************************
 * Function name: create_radvision_communication_socket                              *
 * Input arg    : void                                                               *
 * Output arg   : NONE                                                               *
 * Description  : This function used to create unix socket between main and
                  Radvision thread
                                                                                     *
*************************************************************************************/
/* void create_radvision_communication_socket()
{

    int res, result, fd; //  void *msg;
    char uc_socket_name[TEN] = {"RADV"};
    printf("RADVISION create socket \n");

    g_uiRadv_sock_fd = socket(AF_LOCAL, SOCK_STREAM, 0);

    printf(" +++++++++++++++ RADVision CHECK create socket %d \n", g_uiRadv_sock_fd);
    if (g_uiRadv_sock_fd < CLEAR)
    {
        perror("RADVISION socket create  failed\n");
    }
    else
    {
        printf("RADVISION socket in voip_main success!!!!!!!!\n");
    }
    // unlink(RADV_VOIP_MAIN_PATH);

    bzero(&radv_sock_address, sizeof(radv_sock_address));

    radv_sock_address.sun_family = AF_LOCAL;
    strncpy(&radv_sock_address.sun_path[1], uc_socket_name, sizeof(radv_sock_address.sun_path) - 2);
    // strcpy(radv_sock_address.sun_path,RADV_VOIP_MAIN_PATH);
    g_uiRadv_addrlen = sizeof(radv_sock_address);
    if (bind(g_uiRadv_sock_fd, (struct sockaddr *)&radv_sock_address, g_uiRadv_addrlen) != CLEAR)
    {
        perror("radvision in voip_main bind failed\n");
    }
    else
    {
        printf("RADVISION bind in voip_main success!!!!!!!\n");
    }
    listen(g_uiRadv_sock_fd, 5);
    printf("++++++++++++g_uiRadv_sock_fd:%d", g_uiRadv_sock_fd);
    FD_SET(g_uiRadv_sock_fd, &app_readset);
} */

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

void close_all_signalling_sockets()
{
    int indx;
    // closing UI socket
    int sock_status;

    int sock_arr[TEN] = {g_uiUi_socket_fd, g_uiBroadcast_fd, g_uiServ_udp_comm_fd,
                         g_uiRadv_sock_fd, g_uiMulticast_signalling_fd, g_uiRv_sock_cli_fd, g_iPaging_fd};
    //  ui_sock_status=close(g_uiUi_socket_fd);
    for (indx = CLEAR; indx < EIGHT; indx++)
    {
        sock_status = close(sock_arr[indx]);
        if (sock_status == 0)
        {
            printf("signalling socket %d succesfully closed \n----------------\n", indx);
        }
        else
        {
            printf("Error in closing signalling socket %d \n------------\n", indx);
        }
    }
}

void close_ui_thread()
{
    int ui_thread_status;
    ui_thread_status = pthread_kill(m_thread, SIGUSR1);

    if (ui_thread_status == 0)
    {
        printf("UI Thread Terminated succesfully \n");
    }
    else
    {
        printf("Error in UI Thread termination \n");
    }
}

void clear_all_fds()
{
    int index;
    int fd_arr[FIFTEEN] = {g_uiClient_connection_fd, g_uiApp_timer_fd, g_uiApp_diag_timer_fd,
                           g_uiUi_socket_fd, g_uiRadv_sock_fd, g_uiServ_udp_comm_fd,
                           g_uiMulticast_signalling_fd, g_uiRv_sock_cli_fd};

    for (index = CLEAR; index < NINE; index++)
    {
        FD_CLR(fd_arr[index], &app_readset);
    }
}

void close_all_icom_sockets()
{
    // g_Iicom_fds[i] g_igroup_fds
    int index;
    int icom_socket_status;

    for (index = CLEAR; index < 32; index++)
    {
        icom_socket_status = close(g_Iicom_fds[index]);
        {
            if (icom_socket_status == 0)
            {
                printf("ICOM Socket %d Successfully closed \n ------------------ \n", g_Iicom_fds[index]);
            }
            else
            {
                printf("ERROR closing ICOM Socket %d \n ------------------ \n", g_Iicom_fds[index]);
            }
        }

        FD_CLR(g_Iicom_fds[index], &pWork_m.pagingRtpReceivingFds); // clear ICOM FDs
    }
}

void close_all_group_sockets()
{
    // g_Iicom_fds[i] g_igroup_fds
    int index;
    int group_socket_status;

    for (index = CLEAR; index < 32; index++)
    {

        group_socket_status = close(g_igroup_fds[index]);
        {
            if (group_socket_status == 0)
            {
                printf("GROUP Socket %d Successfully closed \n ------------------ \n", g_igroup_fds[index]);
            }
            else
            {
                printf("ERROR closing GROUP Socket %d \n ------------------ \n", g_igroup_fds[index]);
            }
        }
        FD_CLR(g_igroup_fds[index], &pWork_m.pagingRtpReceivingFds); // clear GROUP FDs
    }
}

void close_all_zone_sockets()
{
    // g_Iicom_fds[i] g_igroup_fds
    int index;
    int zone_socket_status;

    for (index = CLEAR; index < 32; index++)
    {

        zone_socket_status = close(g_izone_fds[index]);
        {
            if (zone_socket_status == 0)
            {
                printf("ZONE Socket %d Successfully closed \n ------------------ \n", g_izone_fds[index]);
            }
            else
            {
                printf("ERROR closing ZONE Socket %d \n ------------------ \n", g_izone_fds[index]);
            }
        }

        FD_CLR(g_izone_fds[index], &pWork_m.pagingRtpReceivingFds); // clear ZONE FDs
    }
}

void close_all_aim_sockets()
{
    // g_Iicom_fds[i] g_igroup_fds
    int index;
    int aim_socket_status;

    for (index = CLEAR; index < 4; index++)
    {
        aim_socket_status = close(g_iAim_fds[index]);
        {
            if (aim_socket_status == 0)
            {
                printf("AIM Socket %d Successfully closed \n ------------------ \n", g_iAim_fds[index]);
            }
            else
            {
                printf("AIM closing ZONE Socket %d \n ------------------ \n", g_iAim_fds[index]);
            }
        }

        FD_CLR(g_iAim_fds[index], &pWork_m.pagingRtpReceivingFds); // Clear AIM FDs
    }
}

void close_all_sockets()
{
    close_all_signalling_sockets();
    close_all_icom_sockets();
    close_all_group_sockets();
    close_all_zone_sockets();
    close_all_aim_sockets();
}

/*************************************************************************************
 * Function name: init_radvision_and_dsp                                             *
 * Input arg    : void                                                               *
 * Output arg   : NONE                                                               *
 * Description  : This function initializes the Radvison stack and DSP               *
 *
 *************************************************************************************/

void init_dsp()
{
    regional_settings_params_t *regionalSettings = NULL;

    /* NirM 080304 - Load DSP kernel modules */
    //	load_dsp_kernel_modules();
    voip_conf_init();

#if defined(AC494_SDP_IPPHONE) || defined(AC49_ORCHID_VOIP_TOOLKIT)
#ifdef CONFIG_GTEK
    init_message_queues(S2U, U2S);
#else
    // startKeypad();     // 30 jan 2023
#ifndef NANOX
    // lcdInit();
#endif
#endif
#endif
    initVoipStatusHandler();
    /* called once on the start of the application */
    /* if (voip_stack_init())
      {
          printf("voip stack init failed\n");

      }*/

    if (voip_conf_util_load_cfg_file(receivedCfgBuf) == -1)
    {
        printf("voip_conf_util_load_cfg_file failed\n");
    }

    /* Start stack for default state of the device. On receive signals stack
     * will be restarted */
    voip_stack_start();
    /* NirM 080303, Initializing DSP */
    voip_conf_dsp_params_get(&regionalSettings, &AdvancedChannelConfiguration);
    dsp_init(regionalSettings, &AdvancedChannelConfiguration);
    // waitUntilVoipTaskReadyForReconfig = 0;  /* voip_task is up, can be reconfiguraed */
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
    printf(" +++++++++++++++ UDP CHECK create socket %d \n", g_uiServ_udp_comm_fd);
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
    if (bind(g_uiServ_udp_comm_fd, (struct sockaddr *)&udp_comm_addr,
             sizeof(udp_comm_addr)) < CLEAR)
    {
        perror("bind create_server_udp_comm_socket:");
        exit(1);
    }
    FD_SET(g_uiServ_udp_comm_fd, &app_readset);
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
    // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
    write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
    pvt_call.curr_icom_no = stn_config.default_intercom - MIN_OFFSET;
    if (stn_config.icom_live_dead_status[stn_config.default_intercom - MIN_OFFSET] == LIVE)
    {
        add_membership(g_Iicom_fds[stn_config.default_intercom - MIN_OFFSET]);
    }
    pthread_mutex_lock(&lock_call_type);
    pvt_call.type = INTERCOM_RX;
    pthread_mutex_unlock(&lock_call_type);
    g_iIcom_hop_flag = CLEAR;
}

/*************************************************************************************
 * Function name: gpio_input_thread                                                  *
 * Input arg    : void                                                               *
 * Output arg   : NONE                                                               *
 * Description  : This is thread function which reads the GPIO continuously          *
 *************************************************************************************/

/*void *gpio_input_thread()
{
    int i_ret,i_ptt_pressed=CLEAR,i_cnt=CLEAR;
    unsigned char uc_call_type;
    volatile int i_ret1;


    while(1)
    {

        pthread_mutex_lock(&lock_call_type);
        uc_call_type=pvt_call.type;
        pthread_mutex_unlock(&lock_call_type);
        //printf("uc_call_type is : %c\n",uc_call_type);
        if(pvt_call.pvt_call_busy==SET || uc_call_type==INTERCOM_RX||uc_call_type==INTERCOM_TX_RX)
    // if(pvt_call.pvt_call_busy==SET || uc_call_type==INTERCOM_TX_RX)
        {
            //printf("handset lifted in if ...%c\n",uc_call_type);
            i_ret=//operate_gpio(PTT_SW,READ,CLEAR);

            if(i_ret==SET)
            {
                //printf("...........INSIDE i_ret==SET if loop\n");
                if(g_iHandset_lifted_signal==SET)
                {
                 // printf("...........g_iHandset_lifted_signal==SET\n");
                    //operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_OFF);


                    g_iHandset_lifted_signal=CLEAR;
                    switch(uc_call_type)
                    {
                        case INTERCOM_RX:
                            if(g_iIcom_hop_flag==SET)
                            {
                                reset_hopped_intercom();
                            }
                        break;
                        case INTERCOM_TX_RX:
                            printf("ptt sw is set and icom release req is send......////\n");
                            send_req_msg_to_server(ICOM_PTT_RELEASE_REQ,ICOM_CALL_FRAME_LENGTH,'1','0');
                            if(g_iIcom_hop_flag==SET)
                            {
                                reset_hopped_intercom();
                            }
                        break;

                        case GROUP:
                                send_req_msg_to_server(GROUP_CALL_RESET_REQ,GROUP_CALL_FRAME_LENGTH,'1','0');
                        break;

                        case P2P:
                                printf("Handset on cradle \n");
                            send_to_phone_input_cb(1,PHONE_INPUT_HOOK_ON);
                        break;
                        case GLOBAL:
                              send_req_msg_to_server(GLOBAL_RESET,GLOBAL_RESET_FRAME_LENGTH,'0','0');
                        break;

                        case ZONE:
                              send_req_msg_to_server(ZONE_RESET_REQ,GLOBAL_RESET_FRAME_LENGTH,'0','0');
                        break;

                        //case GLOBAL_ALARM:


                    }
                }


            }
            else //handset lifted
            {
                //printf("handset lifted in else ...\n");
            //printf("...........INSIDE i_ret==SET else loop\n");
                g_iHandset_lifted_signal=SET;
                //operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);  ///Pvt and page audio mixing
                if(pg_call.pg_call_busy!=SET && stn_config.default_AIM==CLEAR)
                {
                    //operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON); ///on means page speaker mute
                }

            }
        }


        if(pWork_m.private_state==PRIVATE_OUTGOING_ONGOING)
        {
          //  i_ret1=//operate_gpio(PTT_SPEECH,READ,CLEAR);
       //  printf("i_ret1 =%d \n",i_ret1);
            if(i_ret1==SET)
            {
             //printf("...........INSIDE i_ret1==SET if loop\n");
           //  printf("speech detected \n");
                if(i_ptt_pressed==SET)
                {
                    // printf("i_ptt_pressed iss set %c \n",uc_call_type);
                    switch(uc_call_type)
                    {
                        case INTERCOM_TX_RX:
                                printf("ptt speech is set and icom release req is send......////");
                                send_req_msg_to_server(ICOM_PTT_RELEASE_REQ,ICOM_CALL_FRAME_LENGTH,'1','0');
                                pvt_call.type=INTERCOM_RX;
                                i_ptt_pressed=CLEAR;

                        break;

                        case GROUP:
                            // printf("Group ptt key pressed \n");
                                send_req_msg_to_server(GROUP_PTT_RELEASE,GROUP_CALL_FRAME_LENGTH,'1','0');
                                i_ptt_pressed=CLEAR;
                        break;

                    }

                }
            }
            else
            {
            //printf("...........INSIDE i_ret1==SET else loop\n");
                // printf("ptt key pressed \n");
                i_ptt_pressed=SET;
            }
        }

    }


}*/

/*************************************************************************************
 * Function name: operate_gpio                                                       *
 * Input arg    : int gpio_no,int command,int action                                 *
 * Output arg   : int gpio_value                                                     *
 * Description  : This is used to operate GPIO as per requirement                    *
 *************************************************************************************/
int operate_gpio(int gpio_no, int command, int action)
{
    /*  char c_buf[FIFTY];
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

      return gp.gpio_value;*/
}

/*************************************************************************************
 * Function name: init_gpio                                                          *
 * Input arg    : void                                                               *
 * Output arg   : NONE                                                               *
 * Description  : This is used to g_iGpio_fd and to create gpio_thread                  *
 *************************************************************************************/
void init_gpio()
{

    // pthread_t g_thread;

    /*  g_iGpio_fd=open("/dev/phiGPIO",O_RDWR);
      if(g_iGpio_fd<CLEAR)
      {
          perror("Failed to open gpio fd\n");
          return;
      }

      //pthread_create(&g_thread,NULL,gpio_input_thread,CLEAR);*/
}

/***************************************************************************************
 * Function name: start_intercom                                                       *
 * Input arg    : void                                                                 *
 * Output arg   : NONE                                                                 *
 * Description  : This function Opens DSP channel 0 & 2 and do conferncing between them*
 ****************************************************************************************/
void start_intercom()
{
    select_codec(PVT_CHANNEL, NONE);
    multicast_prepare_dsp(PVT_CHANNEL);
    multicast_prepare_dsp(CONF_CHANNEL);
    //  pvt_call.pvt_call_busy=SET;
    select_codec(PVT_CHANNEL, HANDSET);
    pthread_mutex_lock(&lock_call_type);
    pvt_call.type = INTERCOM_RX;
    pthread_mutex_unlock(&lock_call_type);
    // start_alarm(10);

   // acgStart3WayConference(PVT_CHANNEL, CONF_CHANNEL);

    g_itransmit_fd_index = -1;
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
    sprintf(c_buf, route_add_msg, address);
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
    sprintf(c_buf, route_del_msg, address);
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

    switch (event)
    {

    case INCOMING_ZONE_CALL_RX_EVENT:
        drop_membership(g_iAim_fds[stn_config.default_AIM - 1]);
        stop_aim_receive_call();
        // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_ON);
        write_to_io(CALL_THROUGH_LED_ON, IO_PIN_ON);
        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
        write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
        // operate_gpio(PFC_ACT,WRITE,GPIO_ON);
        write_to_io(PFC_ACT_ON, IO_PIN_ON);
        // operate_gpio(ICOM_AMP_MUTE,WRITE,GPIO_ON);
        write_to_io(PVT_AMP_MUTE_ON, IO_PIN_ON); ///
        add_membership(g_izone_fds[pg_call.zone_no]);
        //   send_req_msg_to_server(ICOM_PTT_RELEASE_REQ,ICOM_CALL_FRAME_LENGTH,'1','0');
        printf("INCOMING_ZONE_CALL_RX_EVENT call event received\n");
        // send_msg_ui(IDLE_STATE,LCD,LINE3,CONTINUE_DISPLAY,g_cLine3_buf,0);
        sprintf(g_cLine3_buf, zone_call_msg, lcd_zone_call_init);
        // send_msg_ui(PAGE_INITIATOR_STATE,LCD,LINE3,CONTINUE_DISPLAY,global_call_msg,CLEAR,0);
        // send_msg_ui(IDLE_STATE, LCD, LINE1, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_FIRST_LINE, 0);
        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE1, ZERO, LCD_CLR_FIRST_LINE);
        g_uiPage_call_timer = stn_config.pg_call_timeout * TWO;
        g_uiPage_call_timer_lcd = stn_config.pg_call_timeout * 50;
        pg_call.pg_call_busy = SET;
        pg_call.type = ZONE;
        pWork_m.page_state = PAGE_INCOMING_ONGOING;
        // operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);
        write_to_io(ICOM_LED_OFF, IO_PIN_OFF);
        select_codec(PG_CHANNEL, NONE);
        multicast_prepare_dsp(PG_CHANNEL);
        select_codec(PG_CHANNEL, SPEAKER);
        set_volume(PG_CHANNEL, MAX_PG_VOLUME);
        // sleep(ONE);
        set_volume(PG_CHANNEL, MAX_PG_VOLUME - ONE);
        // start_alarm(incoming_alm_no);
        g_uiFsu_event = CLEAR;
        break;

    case INCOMING_ZONE_ALM_RX_EVENT:
        drop_membership(g_iAim_fds[stn_config.default_AIM - 1]);
        stop_aim_receive_call();
        // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_ON);
        write_to_io(CALL_THROUGH_LED_ON, IO_PIN_ON);
        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
        write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
        // operate_gpio(PFC_ACT,WRITE,GPIO_ON);
        write_to_io(PFC_ACT_ON, IO_PIN_ON);
        // operate_gpio(ICOM_AMP_MUTE,WRITE,GPIO_ON);
        write_to_io(PVT_AMP_MUTE_ON, IO_PIN_ON); ///
        add_membership(g_izone_fds[pg_call.zone_no]);
        //   send_req_msg_to_server(ICOM_PTT_RELEASE_REQ,ICOM_CALL_FRAME_LENGTH,'1','0');
        printf("INCOMING_ZONE_ALM_RX_EVENT call event received\n");
        // send_msg_ui(IDLE_STATE,LCD,LINE3,CONTINUE_DISPLAY,g_cLine3_buf,0);
        sprintf(g_cLine3_buf, zone_alm_msg, lcd_alm_zone_no);
        // send_msg_ui(IDLE_STATE, LCD, LINE1, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_FIRST_LINE, 0);
        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE1, ZERO, LCD_CLR_FIRST_LINE);
        g_uiPage_call_timer = stn_config.alarm_call_timeout * TWO;
        g_uiPage_call_timer_lcd = stn_config.alarm_call_timeout * 50;
        pg_call.pg_call_busy = SET;
        pg_call.type = ZONE_ALARM;
        pWork_m.page_state = PAGE_INCOMING_ONGOING;
        // operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);
        write_to_io(ICOM_LED_OFF, IO_PIN_OFF);
        select_codec(PG_CHANNEL, NONE);
        multicast_prepare_dsp(PG_CHANNEL);
        select_codec(PG_CHANNEL, SPEAKER);
        set_volume(PG_CHANNEL, MAX_PG_VOLUME);
        // sleep(ONE);
        set_volume(PG_CHANNEL, MAX_PG_VOLUME - ONE);
        // start_alarm(incoming_alm_no);
     //   stop_alarm(pg_call.alm_no);
//        start_alarm(pg_call.alm_no);
        g_uiFsu_event = CLEAR;
        break;

    case GLOBAL_RX_ALM_EVENT:
        drop_membership(g_iAim_fds[stn_config.default_AIM - 1]);
        stop_aim_receive_call();
        // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_ON);
        write_to_io(CALL_THROUGH_LED_ON, IO_PIN_ON);
        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
        write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
        // operate_gpio(PFC_ACT,WRITE,GPIO_ON);
        write_to_io(PFC_ACT_ON, IO_PIN_ON);
        // operate_gpio(ICOM_AMP_MUTE,WRITE,GPIO_ON);
        write_to_io(PVT_AMP_MUTE_ON, IO_PIN_ON); ///
        add_membership(g_iPaging_fd);            /// global membership added
        //   send_req_msg_to_server(ICOM_PTT_RELEASE_REQ,ICOM_CALL_FRAME_LENGTH,'1','0');
        printf("GLOBAL_RX_ALM_EVENT call event received");
        // send_msg_ui(IDLE_STATE, LCD, LINE3, CONTINUE_DISPLAY, g_cLine3_buf, 0, 0);
        sprintf(g_cLine3_buf, global_alm_msg, lcd_global_alm_init);
        // send_msg_ui(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,g_cLine3_buf,0,0);
        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE1, ZERO, LCD_CLR_FIRST_LINE);
        g_uiPage_call_timer = stn_config.alarm_call_timeout * TWO;
        g_uiPage_call_timer_lcd = stn_config.alarm_call_timeout * 50;
        pg_call.pg_call_busy = SET;
        pg_call.type = GLOBAL_ALARM;
        pWork_m.page_state = PAGE_INCOMING_ONGOING;
        // operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);
        write_to_io(ICOM_LED_OFF, IO_PIN_OFF);
        select_codec(PG_CHANNEL, NONE);
        multicast_prepare_dsp(PG_CHANNEL);
        select_codec(PG_CHANNEL, SPEAKER);
        set_volume(PG_CHANNEL, MAX_PG_VOLUME);
        // sleep(ONE);
        set_volume(PG_CHANNEL, MAX_PG_VOLUME - ONE);
        //stop_alarm(pg_call.alm_no);
        //start_alarm(pg_call.alm_no);
        g_uiFsu_event = CLEAR;

        break;

    case GLOBAL_RX_CALL_EVENT:
        drop_membership(g_iAim_fds[stn_config.default_AIM - ONE]);
        stop_aim_receive_call();
        //   send_req_msg_to_server(ICOM_PTT_RELEASE_REQ,ICOM_CALL_FRAME_LENGTH,'1','0');
        // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_ON);
        write_to_io(CALL_THROUGH_LED_ON, IO_PIN_ON);
        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
        write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
        // operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);
        write_to_io(ICOM_LED_OFF, IO_PIN_OFF);
        // operate_gpio(PFC_ACT,WRITE,GPIO_ON);
        write_to_io(PFC_ACT_ON, IO_PIN_ON);
        // operate_gpio(ICOM_AMP_MUTE,WRITE,GPIO_ON);
        write_to_io(PVT_AMP_MUTE_ON, IO_PIN_ON); ///
        add_membership(g_iPaging_fd);            /// global membership added
        pg_call.type = GLOBAL;
        printf("GLOBAL_RX_CALL_EVENT call event received");
        // send_msg_ui(IDLE_STATE,LCD,LINE3,CONTINUE_DISPLAY,g_cLine3_buf,0);
        sprintf(g_cLine3_buf, global_call_msg, lcd_global_call_init);
        // send_msg_ui(PAGE_INITIATOR_STATE,LCD,LINE3,CONTINUE_DISPLAY,global_call_msg,CLEAR);
        // send_msg_ui(IDLE_STATE, LCD, LINE1, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_FIRST_LINE, 0);
        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE1, ZERO, LCD_CLR_FIRST_LINE);
        g_uiPage_call_timer = stn_config.pg_call_timeout * TWO;
        g_uiPage_call_timer_lcd = stn_config.pg_call_timeout * 50;
        pWork_m.page_state = PAGE_INCOMING_ONGOING;
        pg_call.pg_call_busy = SET;
        multicast_prepare_dsp(PG_CHANNEL);
        select_codec(PG_CHANNEL, SPEAKER);
        set_volume(PG_CHANNEL, MAX_PG_VOLUME);
        // sleep(ONE);
        set_volume(PG_CHANNEL, MAX_PG_VOLUME - ONE);
        acgPlayTone_phi(PG_CHANNEL, VOIP_SIGNAL_STUTTER_TONE, START_TONE, CLEAR); /// Changes for alert tone
        g_uiPlay_tone_timer = THREE * APP_TIMER_MULTIPLIER;
        g_uiFsu_mcast_event = CLEAR;
        break;

    case GLOBAL_RESET_RX_EVENT:
        close_channel(PG_CHANNEL);
        select_codec(PG_CHANNEL, NONE);
        pg_call.type = CLEAR;

        g_uiPage_call_timer_lcd = CLEAR;
        g_uiPage_call_timer_frlcd_ini = CLEAR;
        one_second_timeout = ONE_SEC_TIMEOUT;

        drop_membership(g_iPaging_fd);
        // operate_gpio(PFC_ACT,WRITE,GPIO_OFF);
        write_to_io(PFC_ACT_OFF, IO_PIN_OFF);
        // operate_gpio(ICOM_AMP_MUTE,WRITE,GPIO_OFF);
        write_to_io(PVT_AMP_MUTE_OFF, IO_PIN_OFF); ///
        printf("GLOBAL_RESET_RX_EVENT call event received  == %d\n", pvt_call.pvt_call_busy);
        if (pvt_call.pvt_call_busy != SET)
        {
            // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF)
            write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
        }
        pg_call.alm_no = CLEAR;
        // send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,"                   ",0);
        pWork_m.page_state = PAGE_IDLE;
        pg_call.pg_call_busy = CLEAR;
        g_uiPage_call_timer = CLEAR;

        printf("busy %d  signal %d \n", pvt_call.pvt_call_busy, g_iHandset_lifted_signal);
        if (pvt_call.pvt_call_busy != CLEAR && g_iHandset_lifted_signal == CLEAR)
        {
            // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
            write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
        }
        else
        {
            // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
            write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);
            printf("MUSIC VOLUME BYPASS ON \n");
        }

        if (g_iAim_mem_dropped == SET && stn_config.default_AIM != CLEAR)
        {
            add_membership(g_iAim_fds[stn_config.default_AIM - ONE]);
            start_aim_receive_call();
        }
        g_uiFsu_mcast_event = CLEAR;
        // send_msg_ui(IDLE_STATE, LCD, LINE1, CONTINUE_DISPLAY, global_terminate, LCD_CLR_FIRST_LINE, 0);
        print_msg_on_lcd(IOCTL_PRINT, global_terminate, LINE1, ZERO, LCD_CLR_FIRST_LINE);
        // usleep(500000);
        // send_msg_ui(IDLE_STATE, LCD, LINE1, CONTINUE_DISPLAY, stn_config.fcs_desc, LCD_CLR_FIRST_LINE, 0);
        print_msg_on_lcd(IOCTL_PRINT, stn_config.fcs_desc, LINE1, ZERO, LCD_CLR_FIRST_LINE);
        break;

    case ZONE_RESET_RX_EVENT:
        close_channel(PG_CHANNEL);
        select_codec(PG_CHANNEL, NONE);
        pg_call.type = CLEAR;
        drop_membership(g_izone_fds[pg_call.zone_no]);

        // operate_gpio(PFC_ACT,WRITE,GPIO_OFF);
        write_to_io(PFC_ACT_OFF, IO_PIN_OFF);
        // operate_gpio(ICOM_AMP_MUTE,WRITE,GPIO_OFF);
        write_to_io(PVT_AMP_MUTE_OFF, IO_PIN_OFF); ///
        printf("ZONE RESET Rx call event received %c \n", pvt_call.pvt_call_busy);
        if (pvt_call.pvt_call_busy != SET)
        {
            // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
            write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
        }
        pg_call.zone_no = CLEAR;
        pg_call.alm_no = CLEAR;

        g_uiPage_call_timer_lcd = CLEAR;
        g_uiPage_call_timer_frlcd_ini = CLEAR;
        one_second_timeout = ONE_SEC_TIMEOUT;

        // send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,"                   ",0);
        pWork_m.page_state = PAGE_IDLE;
        pg_call.pg_call_busy = CLEAR;
        g_uiPage_call_timer = CLEAR;

        printf("busy %d  signal %d \n", pvt_call.pvt_call_busy, g_iHandset_lifted_signal);
        if (pvt_call.pvt_call_busy != CLEAR && g_iHandset_lifted_signal == CLEAR)
        {
            // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
            write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
            // write_to_io(PVT_AMP_MUTE_ON, IO_PIN_ON);
        }
        else
        {
            // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
            write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);
            // write_to_io(PVT_AMP_MUTE_OFF, IO_PIN_OFF);
            printf("MUSIC VOLUME BYPASS ON \n");
        }

        if (g_iAim_mem_dropped == SET && stn_config.default_AIM != CLEAR)
        {
            add_membership(g_iAim_fds[stn_config.default_AIM - ONE]);
            start_aim_receive_call();
        }
        g_uiFsu_event = CLEAR;
        // send_msg_ui(IDLE_STATE, LCD, LINE1, CONTINUE_DISPLAY, zone_terminate, LCD_CLR_FIRST_LINE, 0);
        print_msg_on_lcd(IOCTL_PRINT, zone_terminate, LINE1, ZERO, LCD_CLR_FIRST_LINE);
        // usleep(500000);
        // send_msg_ui(IDLE_STATE, LCD, LINE1, CONTINUE_DISPLAY, stn_config.fcs_desc, LCD_CLR_FIRST_LINE, 0);
        print_msg_on_lcd(IOCTL_PRINT, stn_config.fcs_desc, LINE1, ZERO, LCD_CLR_FIRST_LINE);
        break;

    default:
        break;
    }
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

    /* switch(g_uiFsu_state)
     {
       case IDLE_STATE:

       break;

       case P2P_OFF_HOOK_STATE:
           send_to_phone_input_cb(ONE,PHONE_INPUT_HOOK_ON);

       break;


       case RINGBACK_STATE:
           pvt_call.group_no=CLEAR;
           pg_call.zone_no= CLEAR;
           send_to_phone_input_cb(ONE,PHONE_INPUT_HOOK_ON);
           drop_membership(g_igroup_fds[pvt_call.group_no]);
           //operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);


       break;

       case INTERCOM_STATE:
            drop_membership(g_igroup_fds[pvt_call.group_no]);
            //operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
            pWork_m.private_state=PRIVATE_IDLE;
            del_route_from_system(g_cMulticast_tx_address);
            g_uiPage_call_timer = CLEAR;
            drop_membership(g_izone_fds[pg_call.zone_no]);
            pthread_mutex_lock(&lock_call_type);
            pg_call.type=CLEAR;
            pthread_mutex_unlock(&lock_call_type);
            pg_call.pg_call_busy=CLEAR;
            g_uiSpeech_detection_timer = CLEAR;
            pWork_m.page_state=PAGE_IDLE;

       break;

       case INTERCOM_RESET_RINGBACK_STATE:

            drop_membership(g_igroup_fds[pvt_call.group_no]);
            //operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
            pWork_m.private_state=PRIVATE_IDLE;
            del_route_from_system(g_cMulticast_tx_address);
            g_uiPage_call_timer = CLEAR;
            drop_membership(g_izone_fds[pg_call.zone_no]);
            pthread_mutex_lock(&lock_call_type);
            pg_call.type=CLEAR;
            pthread_mutex_unlock(&lock_call_type);
            pg_call.pg_call_busy=CLEAR;
            g_uiSpeech_detection_timer = CLEAR;
            pWork_m.page_state=PAGE_IDLE;
            pvt_call.curr_icom_no = CLEAR;

       break;

       case P2P_STATE:
       case P2P_RESET_RINGBACK_STATE:
            send_to_phone_input_cb(ONE,PHONE_INPUT_HOOK_ON);
            drop_membership(g_igroup_fds[pvt_call.group_no]);
            //operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
            pWork_m.private_state=PRIVATE_IDLE;
            del_route_from_system(g_cMulticast_tx_address);
            g_uiPage_call_timer = CLEAR;
            drop_membership(g_izone_fds[pg_call.zone_no]);
            pthread_mutex_lock(&lock_call_type);
            pg_call.type=CLEAR;
            pthread_mutex_unlock(&lock_call_type);
            pg_call.pg_call_busy=CLEAR;
            g_uiSpeech_detection_timer = CLEAR;
            pWork_m.page_state=PAGE_IDLE;
            pvt_call.curr_icom_no = CLEAR;

       break;


       case CONF_INIT_STATE:
       case CONFERENCE_STATE:
       case CONF_RESET_RINGBACK_STATE:
            send_to_phone_input_cb(ONE,PHONE_INPUT_HOOK_ON);
            drop_membership(g_igroup_fds[pvt_call.group_no]);
            //operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
            pWork_m.private_state=PRIVATE_IDLE;
            del_route_from_system(g_cMulticast_tx_address);
            g_uiPage_call_timer = CLEAR;
            drop_membership(g_izone_fds[pg_call.zone_no]);
            pthread_mutex_lock(&lock_call_type);
            pg_call.type=CLEAR;
            pthread_mutex_unlock(&lock_call_type);
            pg_call.pg_call_busy=CLEAR;
            g_uiSpeech_detection_timer = CLEAR;
            pWork_m.page_state=PAGE_IDLE;
            pvt_call.curr_icom_no = CLEAR;
       break;

       case GROUP_STATE:
       case GROUP_CONTRIBUTOR_STATE:
       case GROUP_RESET_RINGBACK_STATE:
            drop_membership(g_igroup_fds[pvt_call.group_no]);
            //operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
            pWork_m.private_state=PRIVATE_IDLE;
            del_route_from_system(g_cMulticast_tx_address);
            g_uiPage_call_timer = CLEAR;
            drop_membership(g_izone_fds[pg_call.zone_no]);
            pthread_mutex_lock(&lock_call_type);
            pg_call.type=CLEAR;
            pthread_mutex_unlock(&lock_call_type);
            pg_call.pg_call_busy=CLEAR;
            g_uiSpeech_detection_timer = CLEAR;
            pWork_m.page_state=PAGE_IDLE;
            pvt_call.curr_icom_no = CLEAR;
            g_uiGroup_active_contr_flag=CLEAR;

       break;

       case DIAGNOSTICS_STATE:
            drop_membership(g_igroup_fds[pvt_call.group_no]);
            //operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
            pWork_m.private_state=PRIVATE_IDLE;
            del_route_from_system(g_cMulticast_tx_address);
            g_uiPage_call_timer = CLEAR;
            drop_membership(g_izone_fds[pg_call.zone_no]);
            pthread_mutex_lock(&lock_call_type);
            pg_call.type=CLEAR;
            pthread_mutex_unlock(&lock_call_type);
            pg_call.pg_call_busy=CLEAR;
            g_uiSpeech_detection_timer = CLEAR;
            pWork_m.page_state=PAGE_IDLE;
            pvt_call.curr_icom_no = CLEAR;
            g_uiGroup_active_contr_flag=CLEAR;
            g_iDiagnostic_pvt_status=CLEAR;
            stop_alarm(TEN);
            g_uiSpk_diagnostic_timer = CLEAR;
            g_iDiagnostic_spk_status=CLEAR;
            g_iDiagnostic_nw_status=CLEAR;
            g_iDiagnostic_mic_status=CLEAR;
            g_uiDiagnostic_start_flag=CLEAR;
       break;

       case PAGE_INITIATOR_STATE:
       case PAGE_RESET_RINGBACK_STATE:
            drop_membership(g_igroup_fds[pvt_call.group_no]);
            //operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
            pWork_m.private_state=PRIVATE_IDLE;
            del_route_from_system(g_cMulticast_tx_address);
            g_uiPage_call_timer = CLEAR;
            drop_membership(g_izone_fds[pg_call.zone_no]);
            pthread_mutex_lock(&lock_call_type);
            pg_call.type=CLEAR;
            pthread_mutex_unlock(&lock_call_type);
            pg_call.pg_call_busy=CLEAR;
            g_uiSpeech_detection_timer = CLEAR;
            pWork_m.page_state=PAGE_IDLE;
            pvt_call.curr_icom_no = CLEAR;
            pg_call.alm_no=CLEAR;
            pg_call.zone_no=CLEAR;
       break;

     }
           //////Clear Timers////////////////////////////
           g_uiVoipapp_wait_timer = CLEAR;
           g_uiPvt_call_timer =CLEAR;
           g_uiPlay_tone_timer = CLEAR;

           //////drop membership of Icom and Aim///////////
           drop_membership(g_Iicom_fds[stn_config.default_intercom-MIN_OFFSET]);
           reset_intercom();
           drop_membership(g_iAim_fds[stn_config.default_AIM-ONE]);
           stop_aim_receive_call();

           //////Close Page and Pvt channel///////////////
           close_channel(PG_CHANNEL);
           close_channel(PVT_CHANNEL);

           ///////Clear pvt and page call type////////
           pthread_mutex_lock(&lock_call_type);
           pvt_call.type=CLEAR;
           pthread_mutex_unlock(&lock_call_type);

           /////OFF the Gpios///////////////////////////
           //operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);

           pvt_call.pvt_call_busy=CLEAR;

           //printf("In resetall...\n");
          /* if (setsockopt(g_uiBroadcast_fd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &reg_addr, g_uiReg_addrlen) == -1)
           {
               perror( "Broadcast drop Group  Failed");
               //break;
           }
           else{*/

    // close(g_uiBroadcast_fd);
    // printf("Broadcast Socket is closed\n");
    //}
    // create_broadcast_reg_socket();

    g_cInit_app = CLEAR;
    // exit(0);
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

/***************************************************************************************
 * Function name: start_aim_receive_call                                               *
 * Input arg    : void                                                                 *
 * Output arg   : NONE                                                                 *
 * Description  : This function used to open DSP channel no 1 to receive AIM call      *
 ****************************************************************************************/
void start_aim_receive_call()
{
    printf("AIM Starting \n");

    pg_call.type = AIM;

    pWork_m.page_state = PAGE_INCOMING_ONGOING;
    select_codec(PG_CHANNEL, NONE);
    // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF); write_to_io(PG_AMP_MUTE_ON , IO_PIN_ON);
    write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
    // write_to_io(PVT_AMP_MUTE_ON, IO_PIN_ON);
    multicast_prepare_dsp(PG_CHANNEL);
    select_codec(PG_CHANNEL, SPEAKER);
    set_volume(PG_CHANNEL, stn_config.music_max);
}

/***************************************************************************************
 * Function name: reset_intercom                                                       *
 * Input arg    : void                                                                 *
 * Output arg   : NONE                                                                 *
 * Description  : This function used to close DSP channel no 0 & 2.                      *
 ****************************************************************************************/
void reset_intercom()
{

    // operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);
    write_to_io(ICOM_LED_OFF, IO_PIN_OFF);

    select_codec(PVT_CHANNEL, NONE);
    // select_codec(PVT_CHANNEL,HANDSET);

    acgStop3WayConference(PVT_CHANNEL, CONF_CHANNEL);
    close_channel(PVT_CHANNEL);
    close_channel(CONF_CHANNEL);
    pthread_mutex_lock(&lock_call_type);
    pvt_call.type = CLEAR;
    pthread_mutex_unlock(&lock_call_type);
    pWork_m.private_state = PRIVATE_IDLE;
    g_itransmit_fd_index = -1;
}

/***************************************************************************************
 * Function name: stop_aim_receive_call                                                *
 * Input arg    : void                                                                 *
 * Output arg   : NONE                                                                 *
 * Description  : This function used to close DSP channel no 1 to stop receive AIM call*
 ****************************************************************************************/
void stop_aim_receive_call()
{
    pWork_m.page_state = PAGE_IDLE;

    pg_call.type = CLEAR;

    printf("AIM STOPPED\n");
    close_channel(PG_CHANNEL);
    select_codec(PG_CHANNEL, NONE);
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

    // pthread_create(&m_thread, NULL, ui_thread, CLEAR);
    create_app_timer();
    create_diagnostic_timer();
    init_config_manager();
    // UI_socket_create();
    create_broadcast_reg_socket();
    create_server_udp_comm_socket();

    open_lcd_io();
    io_initialization();
    clear_keyboard_status();
    create_radvision_communication_socket();
    init_gpio();
    // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
    write_to_io(ACT_PVT_PG_MIX_ON, IO_PIN_ON);
    // operate_gpio(LCD_BACKLIGHT,WRITE,GPIO_OFF);
    g_uiServer_live_status_timer = SERVER_LIVE_TIMEOUT;

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

    FILE *org_file, *temp_file;
    int i_line_no = FOURTY_THREE, i_cntr = CLEAR;
    char str[MAX_BUFFER_SIZE];
    char ip_string[MAX_BUFFER_SIZE], ip_str1[MAX_BUFFER_SIZE];
    sprintf(&ip_string[0], "%s%s", SERVER_IP_STRING, &g_cServer_addr);
    sprintf(&ip_str1[0], "%s%s", "voip/signalling/sip/proxy_address=", &g_cServer_addr);
    // strcpy(ip_string,SERVER_IP_STRING);

    org_file = fopen(CONFIGURATION_FILE_NAME, FL_RD);
    temp_file = fopen("temp.txt", FL_WR);

    while (!feof(org_file))
    {
        fgets(str, MAX_BUFFER_SIZE, org_file);
        if (!feof(org_file))
        {
            i_cntr++;
            if (i_cntr == i_line_no)
            {
                fprintf(temp_file, "%s", ip_string);
                fprintf(temp_file, "\n");
            }
            else if (i_cntr == FOURTY_NINE)
            {
                fprintf(temp_file, "%s", ip_str1);
                fprintf(temp_file, "\n");
            }
            else
            {
                fprintf(temp_file, "%s", str);
            }
        }
    }
    fclose(org_file);
    fclose(temp_file);
    remove(org_file);
    rename("temp.txt", CONFIGURATION_FILE_NAME);
}

/***************************************************************************************
 * Function name: load_config_file                                                     *
 * Input arg    : void                                                                 *
 * Output arg   : NONE                                                                 *
 * Description  : This function loads the configuration file which is used by radvision     stack                                                                                   *
 ****************************************************************************************/
void load_config_file()
{
    char *uc_pConfigFileName;
    uc_pConfigFileName = AC_ZALLOC_L(CONFIGURATION_FILE_NAME_SIZE);
    strcpy(uc_pConfigFileName, CONFIGURATION_FILE_NAME);

    memset(g_cAcl_cfgfilename, CLEAR, CONFIGURATION_FILE_NAME_SIZE);
    strcpy(g_cAcl_cfgfilename, uc_pConfigFileName);

    if (cfg_util_load_file(receivedCfgBuf, sizeof(receivedCfgBuf), uc_pConfigFileName) != 0)
    {
        printf("ERROR: Failed to read %s File\n", uc_pConfigFileName);
    }

    if (g_uiConfig_Received == SET)
    {

        if (voip_conf_util_load_cfg_file(receivedCfgBuf) == -1) // #Shrikant-30/08/2020
        {
            printf("voip_conf_util_load_cfg_file failed\n");
        }
    }

    acl_log_set_debug_level_param(1);
    if (uc_pConfigFileName)
    {
        nfree(uc_pConfigFileName);
    }
}

/*void clear_lcd1()
{

    msg.i2c_device_address = LCD_ADDRESS;
    msg.cmd = IOCTL_CLEAR_DISPLAY;
    msg.lineNumber=LINE1;
    msg.start_position=ZERO;
    memcpy(&msg.kbuf,"                ",16);
    //strcpy(msg.kbuf,"                ");
    update_lcd_msg(msg);
}

void clear_lcd2()
{

    msg.i2c_device_address = LCD_ADDRESS;
    msg.cmd = IOCTL_CLEAR_DISPLAY;
    msg.lineNumber=LINE2;
    msg.start_position=ZERO;
     memcpy(&msg.kbuf,"                ",16);
    //strcpy(msg.kbuf,"                ");
     update_lcd_msg(msg);
}

*/

void update_lcd_msg(struct ioctl_lcd_mesg msg)
{
    switch (msg.cmd)
    {
    case IOCTL_CLEAR_DISPLAY:
        ioctl(lcd_io_Fd, LCD, &msg);
        break;

    case IOCTL_PRINT:
        ioctl(lcd_io_Fd, LCD, &msg);
        break;

    case IOCTL_LCD_INITIALIZATION:
        ioctl(lcd_io_Fd, LCD, &msg);
        break;

    default:
        printf("Invalid state in update_lcd_msg\n");
        break;
    }
}

void print_msg_on_lcd(short command, char message[50], unsigned int line_number, unsigned int start_position, unsigned int lcd_line_status)
{
    msg.i2c_device_address = LCD_ADDRESS;

    if (lcd_line_status == LCD_CLR_FIRST_LINE)
    {
        msg.cmd = command;
        strcpy(msg.kbuf, "                ");
        msg.lineNumber = ONE;
        msg.start_position = start_position;
        update_lcd_msg(msg);
    }

    else if (lcd_line_status == LCD_CLR_SECOND_LINE)
    {
        msg.cmd = command;
        strcpy(msg.kbuf, "                ");
        msg.lineNumber = TWO;
        msg.start_position = start_position;
        update_lcd_msg(msg);
    }

    else if (lcd_line_status == LCD_CLR_BOTH_LINE)
    {
        msg.cmd = IOCTL_CLEAR_DISPLAY;
        update_lcd_msg(msg);
    }

    else if (lcd_line_status = NO_LCD_CLR)
    {
        printf("Clear no lines\n"); // NO CLEARING OF LINES, KEEP LINES AS THEY ARE.
    }

    msg.cmd = command;
    strcpy(msg.kbuf, message);
    msg.lineNumber = line_number;
    msg.start_position = start_position;
    update_lcd_msg(msg);
}

void lcd_clear_both_lines(void)
{
    msg.i2c_device_address = LCD_ADDRESS;
    msg.cmd = IOCTL_CLEAR_DISPLAY;
    update_lcd_msg(msg);
}

/***************************************************************************************
 * Function name: main                                                                 *
 * Input arg    : int argc,char **argv                                                 *
 * Output arg   : NONE                                                                 *
 * Description  : This function controls main call logic and process events received   *
                  from various therads.
****************************************************************************************/
int main(int argc, char **argv)
{
    unsigned char uc_buf[TWENTY];
    unsigned int ui_pressed_key;
    int i_ret;

    socketInfo socketdb[THREE];
    signal(SIGTERM, catch_term);
    // signal(SIGINT,catch_term1);
    signal(SIGSEGV, catch_term2);

    g_uiFsu_state = INIT_STATE;
    g_uiFsu_event = INIT_EVENT;

       // initialize_fsu();










    while (g_cInit_app)

    {

        process_events_listen();

        if (g_uiFsu_Broadcast_event)
        {
            switch (g_uiFsu_Broadcast_event)
            {
            case SHUTDOWN_EVENT:
                printf("SHUTDOWN \n");
                g_cInit_app = CLEAR;

                break;
            case SERVER_REG_BCAST_EVENT:

                switch (g_uiFsu_state)
                {
                case VCCS_REGISTERING_STATE:
                    g_uiFsu_Broadcast_event = CLEAR;
                    //  system("insmod gpiodrv.ko");
                    save_server_ip_in_config_file();
                    load_config_file();
                    send_req_msg_to_server(0, CONFIG_REQUEST, CONFIG_REQUEST_FRAME_LENGTH, '0', '0');

                    print_msg_on_lcd(IOCTL_PRINT, registering_msg, LINE2, ZERO, LCD_CLR_BOTH_LINE);
                    // sleep(2);
                    printf("broadcast event received \n");
                    g_uiVoipapp_wait_timer = CONFIGURATION_RESPONSE_TIMEOUT;
                    g_uiFsu_state = CONFIGURING_STATE;
                    break;
                }
                g_uiFsu_Broadcast_event = CLEAR;
                break;

            case WAKE_UP_EVENT:

                switch (g_uiFsu_state)
                {
                case IDLE_STATE:
                case P2P_OFF_HOOK_STATE:
                case RINGBACK_STATE:
                case INTERCOM_STATE:
                case INTERCOM_RESET_RINGBACK_STATE:
                case P2P_STATE:
                case P2P_RESET_RINGBACK_STATE:
                case CONF_INIT_STATE:
                case CONFERENCE_STATE:
                case CONF_RESET_RINGBACK_STATE:
                case GROUP_STATE:
                case GROUP_CONTRIBUTOR_STATE:
                case GROUP_RESET_RINGBACK_STATE:
                case DIAGNOSTICS_STATE:
                case PAGE_INITIATOR_STATE:
                case PAGE_RESET_RINGBACK_STATE:

                    printf("____________IDLE_STATE_______WAKE_UP_EVENT___\n");
                    g_uiVoipapp_wait_timer = CLEAR;
                    reset_all();
                    g_uiFsu_state = VCCS_REGISTERING_STATE;
                    break;
                }
                g_uiFsu_Broadcast_event = CLEAR;
                break;
            default:

                break;
            }
        }

        if (g_uiFsu_mcast_event)
        {
            printf("inside g_uiFsu_mcast_event\n");
            handle_pg_call_in_backgroud(g_uiFsu_mcast_event);
            g_uiFsu_event = g_uiFsu_mcast_event;

            // struct server_coms_global_call glb;
            /*  printf("inside if g_uiFsu_mcast_event %d  %d\n",incoming_global_call.initiator_id,stn_config.logical_id);
              if (incoming_global_call.initiator_id != stn_config.logical_id)
              {
                   printf("inside if glb.initiator_id!=stn_config.logical_id \n");
                  handle_pg_call_in_backgroud(g_uiFsu_mcast_event);
              }
              else
              {
                  printf("in else of gbl.initiator_id!=stn_config.logical_id\n ");
                  g_uiFsu_event = g_uiFsu_mcast_event;
              }*/
            g_uiFsu_mcast_event = CLEAR;
        }

        if (g_uiFsu_timer_event)
        {
            switch (g_uiFsu_timer_event)
            {
            case BROADCAST_TIMEOUT_EVENT:
                switch (g_uiFsu_state)
                {
                case IDLE_STATE:
                case P2P_OFF_HOOK_STATE:
                case RINGBACK_STATE:
                case INTERCOM_STATE:
                case INTERCOM_RESET_RINGBACK_STATE:
                case P2P_STATE:
                case P2P_RESET_RINGBACK_STATE:
                case CONF_INIT_STATE:
                case CONFERENCE_STATE:
                case CONF_RESET_RINGBACK_STATE:
                case GROUP_STATE:
                case GROUP_CONTRIBUTOR_STATE:
                case GROUP_RESET_RINGBACK_STATE:
                case DIAGNOSTICS_STATE:
                case PAGE_INITIATOR_STATE:
                case PAGE_RESET_RINGBACK_STATE:
                    printf("Inside BROADCAST_TIMEOUT_EVENT %d\n", g_uiFsu_timer_event);

                    Restart_Broadcast_Socket();
                    break;
                }
                g_uiFsu_timer_event = CLEAR;
                break;
            }
        }

        if (g_uiFsu_event)
        {

            // handle_pg_call_in_backgroud(g_uiFsu_event);
            //  int call_processing_state_machine()
            switch (g_uiFsu_state)
            {
            case INIT_STATE:
                g_uiFsu_event = INIT_EVENT;
                printf("indide INIT_STATE\n");
                switch (g_uiFsu_event)
                {
                case INIT_EVENT:
                    printf("indide INIT_EVENT\n");
                    g_uiFsu_event = CLEAR;
                    initialize_fsu();

                    print_msg_on_lcd(IOCTL_PRINT, "  VoIP System   ", LINE1, ZERO, LCD_CLR_BOTH_LINE);

                    print_msg_on_lcd(IOCTL_PRINT, "  Phi Audiocom  ", LINE2, ZERO, NO_LCD_CLR);

                    g_uiFsu_state = VCCS_REGISTERING_STATE;
                    break;
                }
                g_uiFsu_event = CLEAR;
                break;

            case VCCS_REGISTERING_STATE:
                switch (g_uiFsu_event)
                {

                case SERVER_REG_BCAST_EVENT:
                    g_uiFsu_event = CLEAR;
                    //  system("insmod gpiodrv.ko");
                    save_server_ip_in_config_file();
                    load_config_file();
                    send_req_msg_to_server(0, CONFIG_REQUEST, CONFIG_REQUEST_FRAME_LENGTH, '0', '0');

                    print_msg_on_lcd(IOCTL_PRINT, registering_msg, LINE2, ZERO, LCD_CLR_BOTH_LINE);

                    // sleep(2);
                    printf("broadcast event received \n");
                    g_uiVoipapp_wait_timer = CONFIGURATION_RESPONSE_TIMEOUT;
                    g_uiFsu_state = CONFIGURING_STATE;
                    break;
                }
                g_uiFsu_event = CLEAR;
                break;

            case CONFIGURING_STATE:
                switch (g_uiFsu_event)
                {
                case CONFIG_RECEIVED:
                    g_uiFsu_event = CLEAR;
                    printf("CONFIG_RECEIVED SUCCESSFULLY \n");
                    g_uiVoipapp_wait_timer = CLEAR;
                    send_req_msg_to_server(0, CONFIG_OK_RESPONSE, CONFIG_OK_RESPONSE_FRAME_LENGTH, '0', '0');

                    print_msg_on_lcd(IOCTL_PRINT, configuring_msg, LINE2, ZERO, LCD_CLR_BOTH_LINE);

                    g_uiVoipapp_wait_timer = CONFIGURATION_RESPONSE_TIMEOUT;

                    // if(g_uiConfig_Received==CLEAR)
                    //{
                    printf("g_uiConfig_Received is clear");
                    g_uiFsu_state = DSP_RADV_INIT_STATE;
                    // g_uiConfig_Received = SET;
                    //}
                    // else
                    //{
                    // registration_to_sip_server();
                    // printf("g_uiConfig_Received is SET");
                    // g_uiFsu_state=VCCS_REGISTERING_STATE;
                    // g_uiFsu_state=UNDER_MANUAL_SIP_REGISTRATION_STATE;
                    //}

                    break;

                case APP_TIMEOUT_EVENT:
                    printf("CONFIGURING_STATE STATE TIMEOUT \n");
                    g_uiFsu_event = CLEAR;

                    print_msg_on_lcd(IOCTL_PRINT, registering_msg, LINE2, ZERO, LCD_CLR_BOTH_LINE);
                    g_uiFsu_state = VCCS_REGISTERING_STATE;
                    break;
                }
                g_uiFsu_event = CLEAR;
                break;

                /*case UNDER_MANUAL_SIP_REGISTRATION_STATE:
                     switch(g_uiFsu_event)
                     {
                        case ACK_RESPONSE:
                            g_uiVoipapp_wait_timer = CLEAR;
                            registration_to_sip_server();
                            printf("Config ok ack received....\n");
                            g_uiFsu_state=SIP_REGISTERING_STATE_MANUAL;
                            g_uiVoipapp_wait_timer=SERVER_RESPONSE_TIMEOUT;

                        break;
                        case APP_TIMEOUT_EVENT:
                                printf("UNDER_SIP_REGISTRATION_STATE STATE TIMEOUT \n");
                                g_uiFsu_event=CLEAR;
                                send_msg_ui(VCCS_REGISTERING_STATE,LCD,LINE2,CONTINUE_DISPLAY,registering_msg,CLEAR);
                                g_uiFsu_state=VCCS_REGISTERING_STATE;
                        break;
                     }
                     g_uiFsu_event=CLEAR;

                break;*/

            /* case DSP_RADV_INIT_STATE:
                switch (g_uiFsu_event)
                {

                case ACK_RESPONSE:
                    g_uiVoipapp_wait_timer = CLEAR;
                    init_dsp();

                    printf("state changed to IDLE STATE  \n");

                    print_msg_on_lcd(IOCTL_PRINT, stn_config.fcs_desc, LINE1, ZERO, LCD_CLR_FIRST_LINE);
                    printf("outer if******************************************\n");
                    if (lcd_fcs_default_icom == 0)
                    {
                        //   printf("inside if******************************************\n");
                        sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    else
                    {
                        //   printf("inside else******************************************\n");
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, lcd_fcs_default_icom);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    // printf("###########outside******************************************\n");

                    create_multicast_signalling_socket();
                    save_mul_starting_address();
                    Initialize_ssrc(stn_config.logical_id);
                    acl_init_paging();
                    send_req_msg_to_server(g_uiFsu_state, KEEP_ALIVE_FRAME, KEEP_ALIVE_FRAME_LENGTH, '0', '0');
                    g_uiKeep_alive_timer = KEEP_ALIVE_TIMEOUT;
                    //     printf("###########outside222222222222222222************************\n");

                    if (stn_config.icom_live_dead_status[stn_config.default_intercom - MIN_OFFSET] == LIVE)
                    {
                        pvt_call.curr_icom_no = stn_config.default_intercom - MIN_OFFSET;
                        add_membership(g_Iicom_fds[pvt_call.curr_icom_no]);
                        start_intercom();
                    }

                    if (stn_config.default_AIM)
                    {
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                        write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
                        add_membership(g_iAim_fds[stn_config.default_AIM - 1]);
                        start_aim_receive_call();
                    }
                    system("acgpio config");
                    acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_STUTTER_TONE, START_TONE, CLEAR);
                    g_uiPlay_tone_timer = THREE * APP_TIMER_MULTIPLIER;
                    printf("############g_igroup_fds[pvt_call.group_no] dsp_radvision =%d\n", g_igroup_fds[0]);
                    // io_initialization();
                    // lcd_flag=SET;
                    g_uiFsu_state = IDLE_STATE;

                    printf("Muting Page Speaker.......///////////\n");



                    // g_uiFsu_state=SIP_REGISTERING_STATE;
                    break;

                case APP_TIMEOUT_EVENT:
                    printf("CONFIGURING_STATE STATE TIMEOUT \n");
                    g_uiFsu_event = CLEAR;

                    print_msg_on_lcd(IOCTL_PRINT, registering_msg, LINE2, ZERO, LCD_CLR_BOTH_LINE);
                    g_uiFsu_state = VCCS_REGISTERING_STATE;
                    break;
                }
                g_uiFsu_event = CLEAR;
                break; */

                case DSP_RADV_INIT_STATE:
                        switch(g_uiFsu_event)
                        {
                            case ACK_RESPONSE:
                                g_uiVoipapp_wait_timer=CLEAR;
                                init_dsp();
                                g_uiFsu_state=SIP_REGISTERING_STATE;
                            break;

                            case APP_TIMEOUT_EVENT:
                                printf("CONFIGURING_STATE STATE TIMEOUT \n");
                                g_uiFsu_event=CLEAR;
                                g_uiFsu_state=VCCS_REGISTERING_STATE;
                            break;

                        }
                        g_uiFsu_event=CLEAR;
                break;

                case SIP_REGISTERING_STATE:
                        switch(g_uiFsu_event)
                        {
                        int channel;
                            case VALID_SIP_REGISTER:
                                g_uiVoipapp_wait_timer=CLEAR;
                               // send_msg_ui_lcd(INIT_STATE,LCD,LINE1,CONTINUE_DISPLAY,lcd_fcs_desc,ZERO,
                                //LCD_ADDRESS,IOCTL_PRINT);

                                print_msg_on_lcd(IOCTL_PRINT, stn_config.fcs_desc, LINE1, ZERO, LCD_CLR_FIRST_LINE);

                                if (lcd_fcs_default_icom == 0)
                    {
                        //   printf("inside if******************************************\n");
                        sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    else
                    {
                        //   printf("inside else******************************************\n");
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, lcd_fcs_default_icom);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }

                                printf("state changed to IDLE STATE  \n");

                                create_multicast_signalling_socket();
                                save_mul_starting_address();
                                Initialize_ssrc(stn_config.logical_id);
                                acl_init_paging();
                                send_req_msg_to_server(g_uiFsu_state,KEEP_ALIVE_FRAME,KEEP_ALIVE_FRAME_LENGTH,'0','0');
                                g_uiKeep_alive_timer= KEEP_ALIVE_TIMEOUT;

                                if(stn_config.icom_live_dead_status[stn_config.default_intercom-MIN_OFFSET]==LIVE)
                                {
                                    pvt_call.curr_icom_no=stn_config.default_intercom-MIN_OFFSET;
                                    add_membership(g_Iicom_fds[pvt_call.curr_icom_no]);
                                    start_intercom();
                                }

                                if(stn_config.default_AIM)
                                {
                                    //operate_gpio(MUSIC_VOL_BYPASS,WRITE,GPIO_OFF);
                                    /*send_msg_ui(IDLE_STATE,IO,LINE2,CONTINUE_DISPLAY,&io_led[5],ZERO,IO_ADDRESS,
                                    LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);*/
                                    write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);
                                    add_membership(g_iAim_fds[stn_config.default_AIM-1]);
                                    start_aim_receive_call();
                                }

                                /*channel=1;
                                acgReinitializeEchoCanceller(channel);

                                channel=2;
                                acgReinitializeEchoCanceller(channel);*/
                               /*  if(station_type == GOOSENECK_STN)
                                {
                                    printf("Gooseneck station mic mute off --------------\n");
                                    write_to_diag_io(MIC_MUTE_OFF, IO_PIN_OFF);
                                } */

                                acgPlayTone_phi(PVT_CHANNEL,VOIP_SIGNAL_STUTTER_TONE,START_TONE,CLEAR);
                                //operate_gpio(MUSIC_VOL_BYPASS,WRITE,GPIO_OFF);
                                g_uiPlay_tone_timer=THREE;
                                g_uiFsu_state=IDLE_STATE;
                                //send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,&io_led[ONE],IO_ADDRESS,LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);



                               // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                                printf("Muting Page Speaker.......///////////\n");

                            break;

                            case APP_TIMEOUT_EVENT:
                                printf("SIP_REGISTERING STATE TIMEOUT \n");
                                //send_msg_ui_lcd(VCCS_REGISTERING_STATE,LCD,LINE2,CONTINUE_DISPLAY,"  REGISTERING  ",ZERO,
                                print_msg_on_lcd(IOCTL_PRINT, registering_msg, LINE2, ZERO, LCD_CLR_BOTH_LINE);
                                //LCD_ADDRESS,IOCTL_PRINT);
                                g_uiFsu_state=VCCS_REGISTERING_STATE;
                            break;
                        }
                        g_uiFsu_event=CLEAR;
                break;

                /*case SIP_REGISTERING_STATE_MANUAL:
                        switch(g_uiFsu_event)
                        {
                           case VALID_SIP_REGISTER:
                                 printf("MANUAL SIP REGISTER RECEIVED \n");
                                 /*create_multicast_signalling_socket();
                                 save_mul_starting_address();
                                 Initialize_ssrc(stn_config.logical_id);
                                 acl_init_paging();
                                g_uiVoipapp_wait_timer=CLEAR;
                                send_msg_ui(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,stn_config.timetick,CLEAR);
                                sprintf(g_cLine3_buf,"IDLE %10d",stn_config.logical_id);
                                // send_msg_ui(INIT_STATE,LCD,LINE2,CONTINUE_DISPLAY,"                  ",CLEAR);
                                send_msg_ui(IDLE_STATE,LCD,LINE3,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR);
                                send_msg_ui(IDLE_STATE,LCD,LINE4,CONTINUE_DISPLAY,stn_config.fcs_desc,CLEAR);
                                printf("state changed to IDLE STATE  \n");

                                send_req_msg_to_server(g_uiFsu_state,KEEP_ALIVE_FRAME,KEEP_ALIVE_FRAME_LENGTH,'0','0');
                                g_uiKeep_alive_timer=KEEP_ALIVE_TIMEOUT;

                                if(stn_config.icom_live_dead_status[stn_config.default_intercom-MIN_OFFSET]==LIVE)
                                {
                                    pvt_call.curr_icom_no=stn_config.default_intercom-MIN_OFFSET;
                                    add_membership(g_Iicom_fds[pvt_call.curr_icom_no]);
                                    start_intercom();
                                }

                                if(stn_config.default_AIM)
                                {
                                    //operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                                    add_membership(g_iAim_fds[stn_config.default_AIM-1]);
                                    start_aim_receive_call();
                                }

                                acgPlayTone_phi(PVT_CHANNEL,VOIP_SIGNAL_STUTTER_TONE,START_TONE,CLEAR);
                                g_uiPlay_tone_timer=THREE;
                                g_uiFsu_state=IDLE_STATE;
                                //operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                                printf("Muting Page Speaker.......///////////\n");
                                ////operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                                 //g_uiFsu_state = IDLE_STATE;
                            break;

                            case APP_TIMEOUT_EVENT:
                                printf("SIP_REGISTERING STATE TIMEOUT \n");
                                send_msg_ui(VCCS_REGISTERING_STATE,LCD,LINE2,CONTINUE_DISPLAY,registering_msg,CLEAR);
                                g_uiFsu_state=VCCS_REGISTERING_STATE;
                            break;
                        }
                        g_uiFsu_event = CLEAR;
                break;

                case SIP_REGISTERING_STATE:
                        switch(g_uiFsu_event)
                        {
                            case VALID_SIP_REGISTER:
                                g_uiVoipapp_wait_timer=CLEAR;
                                send_msg_ui(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,stn_config.timetick,CLEAR);
                                sprintf(g_cLine3_buf,"IDLE %10d",stn_config.logical_id);
                                // send_msg_ui(INIT_STATE,LCD,LINE2,CONTINUE_DISPLAY,"                  ",CLEAR);
                                send_msg_ui(IDLE_STATE,LCD,LINE3,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR);
                                send_msg_ui(IDLE_STATE,LCD,LINE4,CONTINUE_DISPLAY,stn_config.fcs_desc,CLEAR);
                                printf("state changed to IDLE STATE  \n");

                                create_multicast_signalling_socket();
                                save_mul_starting_address();
                                Initialize_ssrc(stn_config.logical_id);
                                acl_init_paging();
                                send_req_msg_to_server(g_uiFsu_state,KEEP_ALIVE_FRAME,KEEP_ALIVE_FRAME_LENGTH,'0','0');
                                g_uiKeep_alive_timer=KEEP_ALIVE_TIMEOUT;

                                if(stn_config.icom_live_dead_status[stn_config.default_intercom-MIN_OFFSET]==LIVE)
                                {
                                    pvt_call.curr_icom_no=stn_config.default_intercom-MIN_OFFSET;
                                    add_membership(g_Iicom_fds[pvt_call.curr_icom_no]);
                                    start_intercom();
                                }

                                if(stn_config.default_AIM)
                                {
                                    //operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                                    add_membership(g_iAim_fds[stn_config.default_AIM-1]);
                                    start_aim_receive_call();
                                }

                                acgPlayTone_phi(PVT_CHANNEL,VOIP_SIGNAL_STUTTER_TONE,START_TONE,CLEAR);
                                g_uiPlay_tone_timer=THREE;

                                g_uiFsu_state=IDLE_STATE;
                                //operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                                printf("Muting Page Speaker.......///////////\n");
                                ////operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                            break;

                            case APP_TIMEOUT_EVENT:
                                printf("SIP_REGISTERING STATE TIMEOUT \n");
                                send_msg_ui(VCCS_REGISTERING_STATE,LCD,LINE2,CONTINUE_DISPLAY,registering_msg,CLEAR);
                                g_uiFsu_state=VCCS_REGISTERING_STATE;
                            break;
                        }
                        g_uiFsu_event=CLEAR;
                break;*/

            case IDLE_STATE:
            printf("g_uiFsu_event is : %d\n", g_uiFsu_event);
               switch (g_uiFsu_event)
                {
                case WAKE_UP_EVENT:
                    printf("____________IDLE_STATE_______WAKE_UP_EVENT___\n");
                    reset_all();

                    //  FD_CLR(g_uiApp_timer_fd,&app_testset);
                    // FD_ZERO(&app_testset);
                    // g_uiKeep_alive_timer=CLEAR;
                    // process_events_listen();

                    g_uiFsu_state = VCCS_REGISTERING_STATE;
                    break;
                    /* case BROADCAST_TIMEOUT_EVENT:
                          printf("Inside BROADCAST_TIMEOUT_EVENT\n");
                          Restart_Broadcast_Socket();
                     break;*/

                case DIAGNOSTIC_EVENT:
                    FD_SET(g_uiApp_diag_timer_fd, &app_readset);
                    drop_membership(g_iAim_fds[stn_config.default_AIM - 1]);
                    drop_membership(g_Iicom_fds[stn_config.default_intercom - MIN_OFFSET]);
                    // reset_intercom();
                    //   stop_aim_receive_call();
                    select_codec(PVT_CHANNEL, NONE);
                    select_codec(PVT_CHANNEL, HANDSET);
                    // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
                    write_to_io(ACT_PVT_PG_MIX_ON, IO_PIN_ON);
                    g_uiDiagnostic_start_flag = SET;

                    set_volume(PVT_CHANNEL, MAX_PVT_VOLUME);
                    g_uiVoipapp_wait_timer = TWENTY_FIVE * APP_TIMER_MULTIPLIER;
//                    start_alarm(TEN);
                    g_uiPvt_diagnostic_timer = DIAGNOSTIC_TIMER_PVT_PG;
                    g_uiFsu_state = DIAGNOSTICS_STATE;
                    break;

                case RINGING_OFF_HOOK_EVENT:
                    drop_membership(g_Iicom_fds[pvt_call.curr_icom_no]);
                    // reset_intercom();
                    send_to_phone_input_cb(1,PHONE_INPUT_HOOK_OFF);
                    printf("OFF HOOK EVENT RECEIVED \n");
                    break;

                case P2P_RESET_PVTTIMEOUT_EVENT:
                    printf("inside  IDLE_STATE event is P2P_RESET_PVTTIMEOUT_EVENT \n");
                    g_uiVoipapp_wait_timer = CLEAR;

                    if (pg_call.pg_call_busy != SET)
                    {
                        // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                        write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
                    }
                    if (pg_call.pg_call_busy == CLEAR)
                    {
                        printf("....................MUSIC_VOl_BYPASS is OFF when no page call........\n");
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                        write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
                    }
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = CLEAR;
                    pthread_mutex_unlock(&lock_call_type);

                    // networking_rtpChannelInfoDelete(PVT_CHANNEL);
                    p2p_dsp_stop_connection(PVT_CHANNEL);
                    select_codec(PVT_CHANNEL, NONE);

                    // send_to_phone_input_cb(1,PHONE_INPUT_HOOK_ON);
                    if (pg_call.pg_call_busy != SET && stn_config.default_AIM == CLEAR)
                    {
                        printf("MUSIC_VOl_BYPASS is ON after P2p reset........\n");
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                        write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);
                    }
                    if (g_iIcom_mem_dropped == SET)
                    {
                        add_membership(g_Iicom_fds[stn_config.default_intercom - MIN_OFFSET]);
                        reset_intercom();
                        start_intercom();
                    }
                    g_uiPvt_call_timer = CLEAR;
                    pvt_call.pvt_call_busy = CLEAR;
                    g_uiFsu_state = IDLE_STATE;
                    one_second_timeout = ONE_SEC_TIMEOUT;

                    g_uiPvt_call_timer_ui_lcd = CLEAR;

                    printf("p2p reset IN P2P state event changing state to IDLE\n");
                    /// send_req_msg_to_server(0,P2P_CALL_RESET_ACK,P2P_CALL_FRAME_LENGTH,'0','0');

                    // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine2_buf, CLEAR, 0);
                    print_msg_on_lcd(IOCTL_PRINT, g_cLine2_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_STUTTER_TONE, START_TONE, CLEAR);

                    g_uiPlay_tone_timer = THREE * APP_TIMER_MULTIPLIER;

                    if (g_iHandset_lifted_signal == CLEAR)
                    {
                        printf("//////////ICOM_MIX_ACt is off after icom membership add.......\n");
                        sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                        ////operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_OFF);
                        write_to_io(ACT_PVT_PG_MIX_OFF, IO_PIN_OFF);
                    }
                    else
                    {
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, stn_config.default_intercom);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                        ////operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
                        write_to_io(ACT_PVT_PG_MIX_ON, IO_PIN_ON);
                    }

                    printf("g_igroup_fds[pvt_call.group_no]=%d\n", g_igroup_fds[0]);
                    break;

                /* case P2P_RESET_EVENT:
                    printf("P2P Reset in IDLE STATE received");
                    ////operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
                    write_to_io(ACT_PVT_PG_MIX_ON, IO_PIN_ON);
                    if (pg_call.pg_call_busy != SET && stn_config.default_AIM == CLEAR)
                    {
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                        write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);
                    }
                    if (pg_call.pg_call_busy != SET && g_iIcom_hop_flag == CLEAR)
                    {
                        // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                        write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
                    }
                    // send_to_phone_input_cb(1,PHONE_INPUT_HOOK_ON);

                    // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine2_buf, CLEAR, 0);
                    print_msg_on_lcd(IOCTL_PRINT, g_cLine2_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    g_uiPvt_call_timer = CLEAR;
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = CLEAR;
                    pthread_mutex_unlock(&lock_call_type);
                    g_uiPvt_call_timer_ui_lcd = CLEAR;
                    one_second_timeout = ONE_SEC_TIMEOUT;
                    pvt_call.pvt_call_busy = CLEAR;
                    g_uiFsu_state = IDLE_STATE;
                    if (g_iIcom_mem_dropped == SET && stn_config.icom_live_dead_status[stn_config.default_intercom - MIN_OFFSET] == LIVE)
                    {
                        add_membership(g_Iicom_fds[stn_config.default_intercom - MIN_OFFSET]);
                        reset_intercom();
                        start_intercom();
                    }

                    acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_STUTTER_TONE, START_TONE, CLEAR);
                    g_uiPlay_tone_timer = THREE * APP_TIMER_MULTIPLIER;
                    // Call_cmdid=0;

                    break; */

                    case P2P_RESET_EVENT:

                                write_to_io(ACT_PVT_PG_MIX_ON, IO_PIN_ON);

                                if(pg_call.pg_call_busy!=SET && stn_config.default_AIM==CLEAR)
                                {
                                	//operate_gpio(MUSIC_VOL_BYPASS,WRITE,GPIO_ON);
                                	write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
                                }
                                if(pg_call.pg_call_busy!=SET && g_iIcom_hop_flag==CLEAR)
                                {
                                    //operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                                    write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
                                }
                                send_to_phone_input_cb(1,PHONE_INPUT_HOOK_ON);

                                if(lcd_fcs_default_icom == 0)
                                {
                                    sprintf(g_cLine3_buf,IDLE_msg,stn_config.logical_id);
                                   // send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                }
                                else
                                {
                                    sprintf(g_cLine3_buf,Channel_msg,stn_config.logical_id,lcd_fcs_default_icom);
                                   // send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
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
                                g_uiPlay_tone_timer = THREE * APP_TIMER_MULTIPLIER;
                                printf("P2P Reset in IDLE STATE received\n");
                            break;

                /* case PVT_CALL_TIMEOUT:
                    if (pg_call.pg_call_busy != SET)
                    {
                        // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                        write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
                    }
                    if (pg_call.pg_call_busy != SET && stn_config.default_AIM == CLEAR)
                    {
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                        write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);
                    }
                    // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, "PVT_CALL_TIMEOUT", LCD_CLR_SECOND_LINE, 0);

                    print_msg_on_lcd(IOCTL_PRINT, "PVT_CALL_TIMEOUT", LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    // send_msg_ui(IDLE_STATE,LCD,LINE3,CONTINUE_DISPLAY,registering_msg,CLEAR);
                    if (stn_config.icom_live_dead_status[stn_config.default_intercom - MIN_OFFSET] == LIVE)
                    {
                        drop_membership(g_Iicom_fds[pvt_call.curr_icom_no]);
                    }

                    g_uiPvt_call_timer = CLEAR;
                    g_uiPvt_call_timer_ui_lcd = CLEAR;
                    one_second_timeout = ONE_SEC_TIMEOUT;
                    pvt_call.pvt_call_busy = CLEAR;
                    pvt_call.curr_icom_no = stn_config.default_intercom - MIN_OFFSET;
                    if (g_iIcom_mem_dropped == SET && stn_config.icom_live_dead_status[stn_config.default_intercom - MIN_OFFSET] == LIVE)
                    {
                        add_membership(g_Iicom_fds[stn_config.default_intercom - MIN_OFFSET]);
                        reset_intercom();
                        start_intercom();
                    }

                    acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_STUTTER_TONE, START_TONE, CLEAR);
                    g_uiPlay_tone_timer = THREE * APP_TIMER_MULTIPLIER;
                    g_uiFsu_state = IDLE_STATE;
                    break;*/

                case INCOMING_GROUP_CALL_EVENT:
                    printf("g_igroup_fds[pvt_call.group_no]=%d\n", g_igroup_fds[0]);
                    printf("INSIDE INCOMING_GROUP_CALL_EVENT OF IDLE_STATE\n");
                    if (stn_config.icom_live_dead_status[stn_config.default_intercom - MIN_OFFSET] == LIVE)
                    {
                        drop_membership(g_Iicom_fds[pvt_call.curr_icom_no]);
                    }
                    // drop_membership(g_iAim_fds[stn_config.default_AIM-1]);
                    select_codec(PVT_CHANNEL, HANDSET);
                    set_volume(PVT_CHANNEL, MAX_PVT_VOLUME);
                    printf("g_igroup_fds[pvt_call.group_no]=%d\n", g_igroup_fds[0]);

                    add_membership(g_igroup_fds[pvt_call.group_no]);
                    // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_ON);
                    write_to_io(CALL_THROUGH_LED_ON, IO_PIN_ON);
                    if (g_iHandset_lifted_signal == CLEAR)
                    {
                        // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_OFF);
                        write_to_io(ACT_PVT_PG_MIX_OFF, IO_PIN_OFF);
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                        write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
                    }
                    g_uiVoipapp_wait_timer = CLEAR;
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = GROUP;
                    pthread_mutex_unlock(&lock_call_type);

                    printf("GROUP call event received\n");
                    // sprintf(g_cLine3_buf,"GROUP:   %8d",stn_config.logical_id);
                    // send_msg_ui(GROUP_STATE,LCD,LINE3,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR);
                    sprintf(g_cLine3_buf, group_msg, lcd_group_call_no, lcd_group_call_init);
                    send_msg_ui(GROUP_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                    pvt_call.pvt_call_busy = SET;
                    // multicast_prepare_dsp(PVT_CHANNEL);

                    // pWork_m.private_state=PRIVATE_OUTGOING_ONGOING;
                    send_msg_ui(GROUP_STATE, LED, LED1, CONTINUE_DISPLAY, g_cLine3_buf, CLEAR, 0);
                    g_uiPvt_call_timer = stn_config.pvt_call_timeout * TWO;
                    acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_STUTTER_TONE, START_TONE, CLEAR);
                    g_uiPlay_tone_timer = THREE * APP_TIMER_MULTIPLIER;
                    g_uiFsu_state = GROUP_STATE;
                    g_uiGroup_PTT_flag = SET; // Added by Yogita for Group_contri_state

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
                                   // send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                }
                                else
                                {
                                    sprintf(g_cLine3_buf,Channel_msg,stn_config.logical_id,lcd_fcs_default_icom);
                                  //  send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                                }
                            break;



               /* case VALID_SW_PTT_EVENT:

                    printf("inside VALID_SW_PTT_EVENT-----\n");
                     //sprintf(uc_buf, "%c%c", g_cReceived_key_seq[1], g_cReceived_key_seq[2]);
                       // ui_pressed_key = atoi(uc_buf);
                       // if (ui_pressed_key == PTT_KEY)
                       // {
                            if (icom_contributor_status_info[pvt_call.curr_icom_no].cont1 == CLEAR ||
                                icom_contributor_status_info[pvt_call.curr_icom_no].cont2 == CLEAR)
                            {
                                printf("IN MIC_PTT  send \n");
                                g_uiVoipapp_wait_timer = SERVER_RESPONSE_TIMEOUT;
                                send_req_msg_to_server(0, ICOM_PTT_SEND_REQ, ICOM_CALL_FRAME_LENGTH, '1', '0');
                                // send_msg_ui(P2P_OFF_HOOK_STATE,LCD,LINE2,CONTINUE_DISPLAY, g_cLine2_buf,LCD_CLR_SECOND_LINE,0);
                                // send_msg_ui(RINGBACK_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,CLEAR);
                                g_uiFsu_state = RINGBACK_STATE;
                            }
                           /* else
                            {
                                if (g_uiPlay_tone_timer == CLEAR)
                                {
                                    acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_BUSY_TONE, START_TONE, CLEAR);
                                    g_uiPlay_tone_timer = SIX * APP_TIMER_MULTIPLIER;
                                }
                            }
                       // }

                        if (ui_pressed_key == RESET_KEY)
                        {
                            if (g_iIcom_hop_flag == SET)
                            {
                                reset_hopped_intercom();
                                if (lcd_fcs_default_icom == 0)
                                {
                                    sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                                    // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                                    print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE1, ZERO, LCD_CLR_FIRST_LINE);
                                }
                                else
                                {
                                    sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, lcd_fcs_default_icom);
                                    // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                                    print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                                }
                            }
                        }
                        break;
*/

                case VALID_KEY_SEQ_EVENT:
                    g_uiFsu_event = CLEAR;
                    g_uiVoipapp_wait_timer = CLEAR;
                    printf("valid key %c call \n", g_cReceived_key_seq[0]);

                    switch (g_cReceived_key_seq[0])
                    {

                    /* case P2P:
                        // send_to_phone_input_cb(1,PHONE_INPUT_HOOK_OFF);
                        printf("in p2p dialling call \n");

                        g_cCalled_station[0] = g_cReceived_key_seq[1];
                        g_cCalled_station[1] = g_cReceived_key_seq[2];
                        g_cCalled_station[2] = g_cReceived_key_seq[3];
                        sprintf(g_cLine1_buf, "Connecting P2P  %s", g_cCalled_station);
                        send_req_msg_to_server(0, P2P_CALL_REQUEST, P2P_CALL_FRAME_LENGTH, 0, 0);
                        last_req_p2p_sent_to_server = P2P_CALL_REQUEST;
                        p2p_req_retransmission_count = 0;
                        // send_msg_ui(RINGBACK_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine1_buf, CLEAR, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine1_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);

                        g_uiVoipapp_wait_timer = SERVER_RESPONSE_TIMEOUT;
                        p2p_call_process_timer_flag = SET;

                        g_uiFsu_state = RINGBACK_STATE; // g_uiFsu_state=P2P_OFF_HOOK_STATE;

                        short stn_id;
                        stn_id = atoi(g_cCalled_station);
                        printf("stn_id is : %d", stn_id);
                        if (stn_config.logical_id == stn_id)
                        {
                            printf("Please dial another number");
                            clear_validation_processing();
                            if (lcd_fcs_default_icom == 0)
                            {
                                sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                                // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                                print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                            }
                            else
                            {

                                sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, stn_config.default_intercom);
                                // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                                print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                            }
                        }
                        else
                        {
                            // send_to_phone_input_cb(1, PHONE_INPUT_HOOK_OFF);
                            printf("else of p2p +++++++++++++++++++++\n");
                            sprintf(g_cLine2_buf, "DIALLING TO %s  ", g_cCalled_station);
                            // sleep(0.5);
                            // send_msg_ui(RINGBACK_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine2_buf, LCD_CLR_SECOND_LINE, 0);
                            print_msg_on_lcd(IOCTL_PRINT, g_cLine2_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                            // send_msg_ui(P2P_OFF_HOOK_STATE,LCD,LINE2,CONTINUE_DISPLAY,  g_cLine2_buf,CLEAR);
                            g_uiVoipapp_wait_timer = THIRTY * APP_TIMER_MULTIPLIER;
                            g_uiFsu_state = RINGBACK_STATE; ///
                        }
                        break; */

                        case P2P:
                                        printf("in p2p dialling call \n");
                                        g_cCalled_station[0]=g_cReceived_key_seq[1];
                                        g_cCalled_station[1]=g_cReceived_key_seq[2];
                                        g_cCalled_station[2]=g_cReceived_key_seq[3];

                                        sprintf(g_cLine1_buf, "Connecting P2P  %s", g_cCalled_station);
                                        print_msg_on_lcd(IOCTL_PRINT, g_cLine1_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);

                                        short stn_id;
                                        stn_id=atoi(g_cCalled_station);
                                        printf("stn_id is : %d",stn_id);
                                        if(stn_config.logical_id == stn_id)
                                        {
                                            printf("Please dial another number");
                                            clear_validation_processing();

                                             if (lcd_fcs_default_icom == 0)
                            {
                                sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                                // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                                print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                            }
                            else
                            {

                                sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, stn_config.default_intercom);
                                // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                                print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                            }
                                        }
                                        else
                                        {
                                            send_to_phone_input_cb(1,PHONE_INPUT_HOOK_OFF);
                                            sprintf(g_cLine2_buf,"DIALLING TO %s  ",g_cCalled_station);

                                            print_msg_on_lcd(IOCTL_PRINT, g_cLine2_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);

                                            g_uiVoipapp_wait_timer = THIRTY * APP_TIMER_MULTIPLIER;
                                            g_uiFsu_state=P2P_OFF_HOOK_STATE;
                                        }

                                    break;

                    case GROUP:
                        printf("INSIDE GROUP state of valid_key_seq of IDLE_STATE \n");
                        sprintf(uc_buf, "%c%c", g_cReceived_key_seq[1], g_cReceived_key_seq[2]);
                        // printf("###########group no1:  %s ",uc_buf);
                        ui_pressed_key = atoi(uc_buf);
                        if (stn_config.group_initiator[ui_pressed_key - MIN_OFFSET] == '1')
                        {
                            pvt_call.group_no = ui_pressed_key - MIN_OFFSET;
                            g_uiVoipapp_wait_timer = SERVER_RESPONSE_TIMEOUT;
                            send_req_msg_to_server(0, GROUP_CALL_REQUEST, GROUP_CALL_FRAME_LENGTH,
                                                   g_cReceived_key_seq[1], g_cReceived_key_seq[2]);

                            sprintf(g_cLine2_buf, "DIALLING GNO:%s  ", uc_buf);

                            // printf("###########group no2:  %s",g_cLine2_buf);
                            // send_msg_ui(RINGBACK_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine2_buf, LCD_CLR_SECOND_LINE, 0);
                            print_msg_on_lcd(IOCTL_PRINT, g_cLine2_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                            // send_msg_ui(RINGBACK_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,CLEAR);
                            g_uiFsu_state = RINGBACK_STATE;
                            printf(" fsu_state is going to be GROUP state\n");
                        }
                        else
                        {
                            if (g_uiPlay_tone_timer == CLEAR)
                            {
                                acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_STUTTER_TONE, START_TONE, CLEAR);
                                g_uiPlay_tone_timer = SIX * APP_TIMER_MULTIPLIER;
                            }
                        }
                        break;

                    case EPBX:

                        break;

                    case INTERCOM_RX:
                        printf("in intercom dialling call \n");
                        sprintf(uc_buf, "%c%c", g_cReceived_key_seq[1], g_cReceived_key_seq[2]);
                        ui_pressed_key = atoi(uc_buf);
                        if ((stn_config.default_intercom != ui_pressed_key) && (stn_config.icom_live_dead_status[ui_pressed_key - MIN_OFFSET] == LIVE))
                        {
                            sprintf(g_cLine2_buf, Channel_msg, stn_config.logical_id, ui_pressed_key);
                            // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine2_buf, LCD_CLR_SECOND_LINE, 0);
                            print_msg_on_lcd(IOCTL_PRINT, g_cLine2_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                            // send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,0);
                            // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_ON);
                            write_to_io(CALL_THROUGH_LED_ON, IO_PIN_ON);
                            drop_membership(g_Iicom_fds[stn_config.default_intercom - MIN_OFFSET]);
                            pvt_call.curr_icom_no = ui_pressed_key - MIN_OFFSET;
                            add_membership(g_Iicom_fds[pvt_call.curr_icom_no]);
                            g_uiPvt_call_timer = stn_config.pvt_call_timeout * TWO;
                            // pvt_call.pvt_call_busy=SET;
                            g_iIcom_hop_flag = SET;
                        }
                        else
                        {

                            if (g_uiPlay_tone_timer == CLEAR)
                            {
                                acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_BUSY_TONE, START_TONE, 0);
                                g_uiPlay_tone_timer = SIX * APP_TIMER_MULTIPLIER;
                            }
                        }

                        break;

                    case GLOBAL:
                        if (stn_config.call_permission_global_call == SET)
                        {
                            printf("in global dialling call\n");
                            send_req_msg_to_server(0, GLOBAL_CALL_REQ, GLOBAL_CALL_FRAME_LENGTH, '0', '0');
                            g_uiVoipapp_wait_timer = SERVER_RESPONSE_TIMEOUT;
                            // send_msg_ui(RINGBACK_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,CLEAR);
                            // send_msg_ui(RINGBACK_STATE, LCD, LINE1, CONTINUE_DISPLAY, "DIALLING GBL CL  ", LCD_CLR_FIRST_LINE, 0);
                            print_msg_on_lcd(IOCTL_PRINT, "DIALLING GBL CL", LINE1, ZERO, LCD_CLR_FIRST_LINE);
                            // sleep(1);
                            g_uiFsu_state = RINGBACK_STATE;
                        }
                        else
                        {
                            if (g_uiPlay_tone_timer == CLEAR)
                            {
                                acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_BUSY_TONE, START_TONE, CLEAR);
                                g_uiPlay_tone_timer = SIX * APP_TIMER_MULTIPLIER;
                            }
                        }
                        break;

                    case GLOBAL_ALARM:
                        if (stn_config.call_permission_global_alarm_call == SET)
                        {
                            printf("in global alarm dialling call \n");
                            send_req_msg_to_server(0, GLOBAL_ALARM_REQ, GLOBAL_ALARM_FRAME_LENGTH,
                                                   g_cReceived_key_seq[1], g_cReceived_key_seq[2]);
                            // send_msg_ui(RINGBACK_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,CLEAR);
                            g_uiVoipapp_wait_timer = SERVER_RESPONSE_TIMEOUT;
                            // send_msg_ui(RINGBACK_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,CLEAR);
                            send_msg_ui(RINGBACK_STATE, LCD, LINE1, CONTINUE_DISPLAY, "DIALLING GBL ALM", LCD_CLR_FIRST_LINE, 0);
                            print_msg_on_lcd(IOCTL_PRINT, "DIALLING GBL ALM", LINE1, ZERO, LCD_CLR_FIRST_LINE);
                            // sleep(1);
                            g_uiFsu_state = RINGBACK_STATE;
                        }
                        else
                        {
                            if (g_uiPlay_tone_timer == CLEAR)
                            {
                                acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_BUSY_TONE, START_TONE, CLEAR);
                                g_uiPlay_tone_timer = SIX * APP_TIMER_MULTIPLIER;
                            }
                        }
                        break;

                    case ZONE:
                        sprintf(uc_buf, "%c%c", g_cReceived_key_seq[1], g_cReceived_key_seq[2]);
                        ui_pressed_key = atoi(uc_buf);
                        printf("CHECKING FOR ZONE: %c%c", g_cReceived_key_seq[1], g_cReceived_key_seq[2]);

                        if (stn_config.zone_initiator[ui_pressed_key - MIN_OFFSET] == '1')
                        {
                            pg_call.zone_no = ui_pressed_key - MIN_OFFSET;
                            g_uiVoipapp_wait_timer = SERVER_RESPONSE_TIMEOUT;
                            send_req_msg_to_server(0, ZONE_CALL_REQUEST, ZONE_CALL_FRAME_LENGTH, g_cReceived_key_seq[1], g_cReceived_key_seq[2]);
                            // send_msg_ui(RINGBACK_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,CLEAR); g_zoneNum_buf

                            // sprintf(g_cLine2_buf,"CALLING ZONE %d ",lcd_zone_call_no);
                            sprintf(g_cLine2_buf, "CALLING ZONE %c%c ", g_cReceived_key_seq[1], g_cReceived_key_seq[2]);
                            // send_msg_ui(RINGBACK_STATE, LCD, LINE1, CONTINUE_DISPLAY, g_cLine2_buf, LCD_CLR_FIRST_LINE, 0);
                            print_msg_on_lcd(IOCTL_PRINT, g_cLine2_buf, LINE1, ZERO, LCD_CLR_FIRST_LINE);
                            ////usleep(500000);
                            sprintf(g_cLine2_buf, "ZONE: %c%c", g_cReceived_key_seq[1], g_cReceived_key_seq[2]);
                            print_msg_on_lcd(IOCTL_PRINT, g_cLine2_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);

                            ////usleep(500000);

                            if (lcd_fcs_default_icom == 0)
                            {
                                sprintf(g_cLine3_buf, "STN_NO:%d  IDLE", stn_config.logical_id);
                                // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                                print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                            }
                            else
                            {
                                sprintf(g_cLine3_buf, "STN_NO:%d CH:%d", stn_config.logical_id, lcd_fcs_default_icom);
                                // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                                print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                            }
                            g_uiFsu_state = RINGBACK_STATE;
                        }
                        else
                        {
                            if (g_uiPlay_tone_timer == CLEAR)
                            {
                                acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_BUSY_TONE, START_TONE, CLEAR);
                                g_uiPlay_tone_timer = SIX * APP_TIMER_MULTIPLIER;
                            }
                        }
                        break;

                    case ZONE_ALARM:
                        sprintf(uc_buf, "%c%c", g_cReceived_key_seq[1], g_cReceived_key_seq[2]);
                        ui_pressed_key = atoi(uc_buf);
                        if (stn_config.zone_initiator[ui_pressed_key - MIN_OFFSET] == '1')
                        {
                            pg_call.zone_no = ui_pressed_key - MIN_OFFSET;
                            g_uiVoipapp_wait_timer = SERVER_RESPONSE_TIMEOUT;
                            send_req_msg_to_server(0, ZONE_ALM_REQUEST, ZONE_CALL_FRAME_LENGTH, g_cReceived_key_seq[3], g_cReceived_key_seq[4]);
                            // send_msg_ui(RINGBACK_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,CLEAR);
                            // send_msg_ui(RINGBACK_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,CLEAR);
                            sprintf(g_cLine2_buf, zone_alm_msg, lcd_alm_zone_no);
                            // send_msg_ui(RINGBACK_STATE, LCD, LINE1, CONTINUE_DISPLAY, g_cLine2_buf, LCD_CLR_FIRST_LINE, 0);
                            print_msg_on_lcd(IOCTL_PRINT, g_cLine2_buf, LINE1, ZERO, LCD_CLR_FIRST_LINE);
                            // sleep(1);
                            g_uiFsu_state = RINGBACK_STATE;
                        }
                        else
                        {
                            if (g_uiPlay_tone_timer == CLEAR)
                            {
                                acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_BUSY_TONE, START_TONE, CLEAR);
                                g_uiPlay_tone_timer = SIX * APP_TIMER_MULTIPLIER;
                            }
                        }
                        break;

                    default:
                        sprintf(uc_buf, "%c%c", g_cReceived_key_seq[1], g_cReceived_key_seq[2]);
                        ui_pressed_key = atoi(uc_buf);
                        if (ui_pressed_key == PTT_KEY)
                        {
                            if (icom_contributor_status_info[pvt_call.curr_icom_no].cont1 == CLEAR ||
                                icom_contributor_status_info[pvt_call.curr_icom_no].cont2 == CLEAR)
                            {
                                printf("IN PTT send \n");
                                g_uiVoipapp_wait_timer = SERVER_RESPONSE_TIMEOUT;
                                send_req_msg_to_server(0, ICOM_PTT_SEND_REQ, ICOM_CALL_FRAME_LENGTH, '1', '0');
                                // send_msg_ui(P2P_OFF_HOOK_STATE,LCD,LINE2,CONTINUE_DISPLAY, g_cLine2_buf,LCD_CLR_SECOND_LINE,0);
                                // send_msg_ui(RINGBACK_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,CLEAR);
                                g_uiFsu_state = RINGBACK_STATE;
                            }
                            else
                            {
                                if (g_uiPlay_tone_timer == CLEAR)
                                {
                                    acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_BUSY_TONE, START_TONE, CLEAR);
                                    g_uiPlay_tone_timer = SIX * APP_TIMER_MULTIPLIER;
                                }
                            }
                        }

                        if (ui_pressed_key == RESET_KEY)
                        {
                            if (g_iIcom_hop_flag == SET)
                            {
                                reset_hopped_intercom();
                                if (lcd_fcs_default_icom == 0)
                                {
                                    sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                                    // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                                    print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE1, ZERO, LCD_CLR_FIRST_LINE);
                                }
                                else
                                {
                                    sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, lcd_fcs_default_icom);
                                    // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                                    print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                                }
                            }
                        }
                        break;
                    }
                    break;


              /// VALID_SW_SEQ: FOR MIC_PTT_SW





                /* case P2P_CALL_EVENT:
                    // case CALL_OFF_HOOK_EVENT: // IDLE STATE EVENT
                    printf("############g_igroup_fds[pvt_call.group_no] P2P_CALL_EVENT start =%d\n", g_igroup_fds[0]);
                    g_uiVoipapp_wait_timer = CLEAR;
                    if (stn_config.icom_live_dead_status[stn_config.default_intercom - MIN_OFFSET] == LIVE)
                    {
                        drop_membership(g_Iicom_fds[pvt_call.curr_icom_no]);
                        reset_intercom();
                    }
                    // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_ON);
                    write_to_io(CALL_THROUGH_LED_ON, IO_PIN_ON);
                    if (g_iHandset_lifted_signal == CLEAR)
                    {
                        // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_OFF);
                        write_to_io(ACT_PVT_PG_MIX_OFF, IO_PIN_OFF);
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                        write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
                    }
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = P2P;
                    pthread_mutex_unlock(&lock_call_type);
                    printf("P2p call event received In IDLE\n");
                    printf("g_cCaller %s stn id %d \n", g_cCalled_station, stn_config.logical_id);
                    // sprintf(g_cLine3_buf,P2P_msg,g_cCalled_station);
                    sprintf(g_cLine3_buf, "P2P:%s   %8d", g_cCalled_station, stn_config.logical_id);
                    printf("line3buf=%s\n", g_cLine3_buf);
                    // multicast_prepare_dsp(PVT_CHANNEL);
                    // select_codec(PVT_CHANNEL,NONE);
                    // select_codec(PVT_CHANNEL,HANDSET);
                    // send_msg_ui(P2P_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                    print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    send_msg_ui(P2P_STATE, LED, LED1, CONTINUE_DISPLAY, g_cLine3_buf, CLEAR, 0);

                    // send_msg_ui(P2P_STATE,LCD,LINE3,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR);
                    // send_msg_ui(P2P_STATE,LED,LED1,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR);
                    g_uiPvt_call_timer = stn_config.pvt_call_timeout * TWO;
                    g_uiPvt_call_timer_ui_lcd = stn_config.pvt_call_timeout * 50;
                    g_uiPage_call_timer_frlcd_ini = 0;

                    // acgPlayTone_phi(PVT_CHANNEL,VOIP_SIGNAL_STUTTER_TONE,START_TONE,CLEAR);

                    g_uiPlay_tone_timer = THREE * APP_TIMER_MULTIPLIER;
                    pvt_call.pvt_call_busy = SET;
                    set_volume(PVT_CHANNEL, MAX_PVT_VOLUME);

                    //  sleep(1);
                    //  set_volume(PVT_CHANNEL,14);
                    if (strcmp("ano", g_cCaller) == 0)
                    {
                        printf("*************Inside ano ........\n");
                        // send_msg_ui(CONFERENCE_STATE,LCD,LINE3,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR);
                        // send_msg_ui(CONFERENCE_STATE,LED,LED1,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR);
                        // send_msg_ui(CONFERENCE_STATE, LCD, LINE2, CONTINUE_DISPLAY, CONFERENCE_msg, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, CONFERENCE_msg, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                        send_msg_ui(CONFERENCE_STATE, LED, LED1, CONTINUE_DISPLAY, g_cLine3_buf, CLEAR, 0);
                        g_uiFsu_state = CONFERENCE_STATE;
                    }
                    else
                    {
                        printf("****In else of ano ........\n");
                        // send_msg_ui(P2P_STATE,LCD,LINE3,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR);
                        // send_msg_ui(P2P_STATE,LED,LED1,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR);

                        sprintf(g_cLine3_buf, P2P_msg, g_cCalled_station);
                        // send_msg_ui(P2P_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);

                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);

                        send_msg_ui(P2P_STATE, LED, LED1, CONTINUE_DISPLAY, g_cLine3_buf, CLEAR, 0);
                        send_req_msg_to_server(0, P2P_CALL_ACK, P2P_CALL_FRAME_LENGTH, '0', '0');
                        printf("########Channel0_rtpsfd=%d#######\n", Channel0_rtpsfd);
                        networking_rtpChannelAddrSet(pvt_call.initiator_ip, RTP_PORT, PVT_CHANNEL);
                        networking_rtpChannelConnect(PVT_CHANNEL, Channel0_rtpsfd);
                        p2p_dsp_start_connection(PVT_CHANNEL);
                        select_codec(PVT_CHANNEL, HANDSET);
                        acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_STUTTER_TONE, START_TONE, CLEAR);
                        g_uiFsu_state = P2P_STATE;
                    }
                    printf("g_igroup_fds[pvt_call.group_no] P2P_CALL_EVENT=%d\n", g_igroup_fds[0]);

                    break; */

                    case P2P_CALL_EVENT:
                           // case CALL_OFF_HOOK_EVENT: // IDLE STATE EVENT
                                g_uiVoipapp_wait_timer=CLEAR;
                               // send_msg_ui_io(P2P_STATE,IO,CONTINUE_DISPLAY,LED_CALL_THROUGH,SET,&io_led[ONE],IO_ADDRESS,
                              //  LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                              //  usleep(100000);


                    write_to_io(CALL_THROUGH_LED_ON, IO_PIN_ON);

                    write_to_io(PVT_AMP_MUTE_OFF, IO_PIN_OFF);


                                /*if(g_iHandset_lifted_signal==SET)
                                {
                                   // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_OFF);
                                   send_msg_ui_io(P2P_STATE,IO,CONTINUE_DISPLAY,PAGE_SPEAKER,SET,&io_led[FIVE],IO_ADDRESS,
                                   LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);

                                }
                                */
                                pvt_call.pvt_call_busy=SET;
                                /*if(g_iHandset_lifted_signal==CLEAR)
                                {
                                    select_codec(PVT_CHANNEL,NONE);
                                    select_codec(PVT_CHANNEL,SPEAKER);
                                }
                                else
                                {
                                    select_codec(PVT_CHANNEL,NONE);
                                    select_codec(PVT_CHANNEL,HANDSET);
                                }*/

                                if (g_iHandset_lifted_signal == CLEAR)
                                {
                                // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_OFF);
                                write_to_io(ACT_PVT_PG_MIX_OFF, IO_PIN_OFF);
                                // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                                write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);
                                }

                                select_codec(PVT_CHANNEL,NONE);
                                select_codec(PVT_CHANNEL,HANDSET);

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
                               // send_msg_ui_lcd(P2P_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);

                                g_uiPvt_call_timer=stn_config.pvt_call_timeout*TWO;
                                g_uiPvt_call_timer_ui_lcd = stn_config.pvt_call_timeout*50;
                                g_uiPage_call_timer_frlcd_ini = 0;
                                acgPlayTone_phi(PVT_CHANNEL,VOIP_SIGNAL_STUTTER_TONE,START_TONE,CLEAR);
                                g_uiPlay_tone_timer=THREE * APP_TIMER_MULTIPLIER;


                                set_volume(PVT_CHANNEL,MAX_PVT_VOLUME);

                                if(strcmp("ano",g_cCaller)==0)
                                {
                                    printf("*************Inside ano ........\n");
                                    print_msg_on_lcd(IOCTL_PRINT, CONFERENCE_msg, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                        send_msg_ui(CONFERENCE_STATE, LED, LED1, CONTINUE_DISPLAY, g_cLine3_buf, CLEAR, 0);
                        g_uiFsu_state = CONFERENCE_STATE;
                                }
                                else
                                {
                                    printf("****In else of ano ........\n");
                                    sprintf(g_cLine3_buf, P2P_msg, g_cCalled_station);
                                    // send_msg_ui(P2P_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);

                                    print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                                    g_uiFsu_state=P2P_STATE;
                                }

                            break;
                default:

                    break;
                    if (lcd_fcs_default_icom == 0)
                    {
                        sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    else
                    {
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, stn_config.default_intercom);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                }
                g_uiFsu_event = CLEAR;
                break;

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


                            if (lcd_fcs_default_icom == 0)
                            {
                                sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                                // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                                print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                            }
                            else
                            {

                                sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, stn_config.default_intercom);
                                // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                                print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
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
                              //  send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                            }
                            else
                            {
                                sprintf(g_cLine3_buf,Channel_msg,stn_config.logical_id,lcd_fcs_default_icom);
                              //  send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                            }
                             g_uiFsu_state=IDLE_STATE;
                        break;


                    }
                g_uiFsu_event=CLEAR;
                break;

                /* case P2P_OFF_HOOK_STATE:
                     switch(g_uiFsu_event)
                     {

                         /*case WAKE_UP_EVENT:
                             printf("____________p2p_OFF_HOOK_STATE_______WAKE_UP_EVENT___\n");
                             reset_all();
                             g_uiFsu_state=VCCS_REGISTERING_STATE;

                         break;*/

                /*case BROADCAST_TIMEOUT_EVENT:
                     printf("Inside BROADCAST_TIMEOUT_EVENT\n");
                     Restart_Broadcast_Socket();
                break;*/

                /*case CALL_OFF_HOOK_EVENT:
                    drop_membership(g_Iicom_fds[pvt_call.curr_icom_no]);
                    reset_intercom();
                    printf("CALL_OFF_HOOK_EVENT received");
                    sendCompleteString(g_cCalled_station);
                    g_uiVoipapp_wait_timer = FOURTY;///change by Rahul 4-05-21
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
                        //operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_OFF);
                        //operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                    }
                    //operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);
                    g_uiVoipapp_wait_timer=CLEAR;
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type=GROUP;
                    pthread_mutex_unlock(&lock_call_type);
                    printf("GROUP call event received");
                    sprintf(g_cLine3_buf,"GROUP:   %8d",stn_config.logical_id);
                    send_msg_ui(GROUP_STATE,LCD,LINE3,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR);
                    pvt_call.pvt_call_busy=SET;
                    select_codec(PVT_CHANNEL,HANDSET);
                    set_volume(PVT_CHANNEL,MAX_PVT_VOLUME);
                    send_msg_ui(GROUP_STATE,LED,LED1,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR);
                    g_uiPvt_call_timer=stn_config.pvt_call_timeout*TWO;
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
                        //operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                    }
                    //operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
                    g_uiVoipapp_wait_timer=CLEAR;
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type=CLEAR;
                    pthread_mutex_unlock(&lock_call_type);
                    //send_to_phone_input_cb(1,PHONE_INPUT_HOOK_ON);
                    send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,CLEAR);
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


                    acgPlayTone_phi(PVT_CHANNEL,VOIP_SIGNAL_STUTTER_TONE,START_TONE,CLEAR);
                    g_uiPlay_tone_timer=THREE;
                   // Call_cmdid=0;
                break;

                 case APP_TIMEOUT_EVENT:
                    printf("OFF_HOOK STATE TIMEOUT \n");
                    if(g_cReceived_key_seq[0]==P2P)
                    {
                        send_to_phone_input_cb(ONE,PHONE_INPUT_HOOK_ON);
                    }
                    send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                    g_uiFsu_state=IDLE_STATE;
                break;


            }
        g_uiFsu_event=CLEAR;
        break;*/

            case RINGBACK_STATE:
                switch (g_uiFsu_event)
                {

                case WAKE_UP_EVENT:
                    printf("____________RINGBACK_STATE_______WAKE_UP_EVENT___\n");
                    reset_all();
                    g_uiFsu_state = VCCS_REGISTERING_STATE;

                    break;

                /*case BROADCAST_TIMEOUT_EVENT:
                     printf("Inside BROADCAST_TIMEOUT_EVENT\n");
                     Restart_Broadcast_Socket();
                break;
*/
                case INCOMING_ZONE_CALL_EVENT:
                    g_uiVoipapp_wait_timer = CLEAR;
                    // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_ON);

                    // write_to_io(TWO , IO_PIN_ON);
                    write_to_io(CALL_THROUGH_LED_ON, IO_PIN_ON);

                    drop_membership(g_iAim_fds[stn_config.default_AIM - ONE]);
                    if (stn_config.icom_live_dead_status[stn_config.default_intercom - MIN_OFFSET] == LIVE)
                    {
                        drop_membership(g_Iicom_fds[pvt_call.curr_icom_no]);
                        reset_intercom();
                    }
                    add_membership(g_izone_fds[pg_call.zone_no]);
                    // stop_aim_receive_call();

                    pg_call.type = CLEAR;
                    g_uiVoipapp_wait_timer = CLEAR;
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = ZONE;
                    pthread_mutex_unlock(&lock_call_type);
                    pWork_m.private_state = PRIVATE_OUTGOING_ONGOING;
                    g_itransmit_fd_index = ZONE_OFFSET + pg_call.zone_no;
                    // send_msg_ui(PAGE_INITIATOR_STATE,LCD,LINE3,CONTINUE_DISPLAY,global_call_msg,CLEAR);
                    sprintf(g_cLine3_buf, zone_call_msg, lcd_zone_call_init);
                    // send_msg_ui(PAGE_INITIATOR_STATE,LCD,LINE3,CONTINUE_DISPLAY,global_call_msg,CLEAR);
                    // send_msg_ui(PAGE_INITIATOR_STATE, LCD, LINE1, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_FIRST_LINE, 0);
                    print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE1, ZERO, LCD_CLR_FIRST_LINE);

                    sprintf(g_cMulticast_tx_address, "%s%d", gc_zone_mul_start_addr, g_izone_mul_addr_start_offset + pg_call.zone_no);
                    printf("ZOne multicast tx address %s\n", g_cMulticast_tx_address);
                    add_route_to_system(g_cMulticast_tx_address);
                    printf("INCOMING_ZONE_CALL_EVENT call event received\n");
                    g_uiPage_call_timer = stn_config.pg_call_timeout * 2;
                    g_uiPage_call_timer_frlcd_ini = stn_config.pg_call_timeout;
                    g_uiPvt_call_timer_ui_lcd = 0;
                    g_uiFsu_state = PAGE_INITIATOR_STATE;
                    pvt_call.pvt_call_busy = SET;
                    multicast_prepare_dsp(PVT_CHANNEL);
                    // select_codec(PG_CHANNEL,NONE);
                    select_codec(PVT_CHANNEL, NONE);
                    select_codec(PVT_CHANNEL, HANDSET);

                    break;

                case INCOMING_ZONE_ALM_EVENT:
                    g_uiVoipapp_wait_timer = CLEAR;
                    // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_ON);
                    write_to_io(CALL_THROUGH_LED_ON, IO_PIN_ON);

                    drop_membership(g_iAim_fds[stn_config.default_AIM - ONE]);
                    if (stn_config.icom_live_dead_status[stn_config.default_intercom - MIN_OFFSET] == LIVE)
                    {
                        drop_membership(g_Iicom_fds[pvt_call.curr_icom_no]);
                        reset_intercom();
                    }

                    add_membership(g_izone_fds[pg_call.zone_no]);
                    // stop_aim_receive_call();
                    pg_call.type = CLEAR;
                    g_uiVoipapp_wait_timer = CLEAR;
                    pthread_mutex_lock(&lock_call_type);
                    pg_call.type = ZONE_ALARM;
                    pthread_mutex_unlock(&lock_call_type);
                    // pWork_m.private_state=PRIVATE_OUTGOING_ONGOING;
                    // g_itransmit_fd_index=ZONE_OFFSET+pg_call.zone_no;
                    // send_msg_ui(PAGE_INITIATOR_STATE,LCD,LINE3,CONTINUE_DISPLAY,global_call_msg,CLEAR);
                    sprintf(g_cLine3_buf, zone_alm_msg, lcd_alm_zone_no);
                    // send_msg_ui(PAGE_INITIATOR_STATE, LCD, LINE1, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_FIRST_LINE, 0);
                    print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE1, ZERO, LCD_CLR_FIRST_LINE);
                    //  sprintf(g_cMulticast_tx_address,"%s%d",gc_zone_mul_start_addr,g_izone_mul_addr_start_offset+pg_call.zone_no);
                    //  printf("ZOne multicast tx address %s\n",g_cMulticast_tx_address);
                    //  add_route_to_system(g_cMulticast_tx_address);
                    printf("INCOMING_ZONE_ALM_CALL_EVENT call event received\n");
                    // g_uiPage_call_timer=stn_config.alarm_call_timeout*TWO;
                    g_uiPage_call_timer = stn_config.alarm_call_timeout * 2;
                    g_uiPage_call_timer_lcd = stn_config.alarm_call_timeout * 50;
                    g_uiFsu_state = PAGE_INITIATOR_STATE;
                    pg_call.pg_call_busy = SET;
                    multicast_prepare_dsp(PG_CHANNEL);
                    // select_codec(PG_CHANNEL,NONE);
                    select_codec(PG_CHANNEL, NONE);
                    select_codec(PG_CHANNEL, SPEAKER);
                 //   stop_alarm(pg_call.alm_no);
//                    start_alarm(pg_call.alm_no);

                    break;

               /*  case P2P_CALL_EVENT:

                    p2p_call_process_timer_flag = CLEAR;
                    // printf("############g_igroup_fds[pvt_call.group_no] P2P_CALL_EVENT rbS =%d\n",g_igroup_fds[0]);
                    // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_ON);
                    write_to_io(CALL_THROUGH_LED_ON, IO_PIN_ON);

                    if (stn_config.icom_live_dead_status[stn_config.default_intercom - MIN_OFFSET] == LIVE)
                    {
                        drop_membership(g_Iicom_fds[pvt_call.curr_icom_no]);
                    }
                    // reset_intercom();
                    //   stop_aim_receive_call();
                    g_uiVoipapp_wait_timer = CLEAR;
                    //  if(g_iHandset_lifted_signal!=SET)
                    // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                    write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = P2P;
                    pthread_mutex_unlock(&lock_call_type);

                    printf("********In main.c pvt_call.type=%c\tP2P=%c********\n", pvt_call.type, P2P);
                    printf("P2p call event received\n");
                    // sprintf(g_cLine3_buf, "P2P:%s   %4d", g_cCalled_station, stn_config.logical_id);

                    sprintf(g_cLine3_buf, "P2P:%s   ", g_cCalled_station);

                    // send_msg_ui(P2P_STATE,LCD,LINE3,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR);
                    // sprintf(g_cLine3_buf,P2P_msg,g_cCaller);
                    // send_msg_ui(P2P_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);

                    print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);

                    // multicast_prepare_dsp(PVT_CHANNEL);
                    // select_codec(PVT_CHANNEL,NONE);
                    // select_codec(PVT_CHANNEL,HANDSET);
                    pvt_call.initiator = SET;
                    set_volume(PVT_CHANNEL, MAX_PVT_VOLUME);
                    // send_msg_ui(P2P_STATE,LED,LED1,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR);
                    // send_msg_ui(P2P_STATE, LED, LED1, CONTINUE_DISPLAY, g_cLine3_buf, CLEAR, 0);
                    pvt_call.pvt_call_busy = SET;
                    g_uiPvt_call_timer = stn_config.pvt_call_timeout * TWO;
                    g_uiPvt_call_timer_ui_lcd = stn_config.pvt_call_timeout * 50;
                    // g_uiPvt_call_timer_ui_lcd = stn_config.pvt_call_timeout ;

                    printf("g_uiPvt_call_timer_ui_lcd in P2P STATE --- %d", g_uiPvt_call_timer_ui_lcd);
                    g_uiPage_call_timer_frlcd_ini = 0;
                    // start_pvt_lcd_timer();
                    printf("########Channel0_rtpsfd=%d#######\n", Channel0_rtpsfd);
                    networking_rtpChannelAddrSet(pvt_call.destination_ip, RTP_PORT, PVT_CHANNEL);
                    networking_rtpChannelConnect(PVT_CHANNEL, Channel0_rtpsfd);
                    p2p_dsp_start_connection(PVT_CHANNEL);
                    select_codec(PVT_CHANNEL, HANDSET);
                    acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_STUTTER_TONE, START_TONE, CLEAR);
                    g_uiPlay_tone_timer = THREE * APP_TIMER_MULTIPLIER;
                    g_uiFsu_state = P2P_STATE;
                    break; */
                    /// send_msg_ui(P2P_STATE,LED,LED_CALL_THROUGH,LED_FLASH,g_cLine3_buf,CLEAR);
                    /*case VALID_KEY_SEQ_EVENT:
                        send_to_phone_input_cb(ONE,PHONE_INPUT_HOOK_ON);
                        g_uiFsu_state=IDLE_STATE;
                    break;*/

                    case P2P_CALL_EVENT:
                            printf("*****inside P2P_CALL_EVENT of ringback state\n");
                           // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_ON);
                            //send_msg_ui_io(P2P_STATE,IO,CONTINUE_DISPLAY,LED_CALL_THROUGH,SET,&io_led[ONE],IO_ADDRESS,
                           // LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                          //  stop_aim_receive_call();
                            write_to_io(CALL_THROUGH_LED_ON, IO_PIN_ON);
                            g_uiVoipapp_wait_timer=CLEAR;
                            //  if(g_iHandset_lifted_signal!=SET)
                           // usleep(100000);
                           // send_msg_ui_io(P2P_STATE,IO,CONTINUE_DISPLAY,PAGE_SPEAKER,SET,&io_led[FIVE],IO_ADDRESS,
                           // LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                            write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);
                            write_to_io(PVT_AMP_MUTE_OFF, IO_PIN_OFF);

                            pthread_mutex_lock(&lock_call_type);
                            pvt_call.type = P2P;
                            pthread_mutex_unlock(&lock_call_type);

                            pthread_mutex_lock(&lock_call_type);
                            pvt_call.type=P2P;
                            pthread_mutex_unlock(&lock_call_type);
                            printf("P2p call event received\n");
                            //sprintf(g_cLine3_buf,"P2P:%s %2d",g_cCaller,stn_config.logical_id);
                            sprintf(g_cLine3_buf, "P2P:%s   ", g_cCalled_station);

                            print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);

                            pvt_call.pvt_call_busy=SET;
                            select_codec(PVT_CHANNEL,NONE);
                            select_codec(PVT_CHANNEL,HANDSET);
                            set_volume(PVT_CHANNEL,MAX_PVT_VOLUME);
                           // send_msg_ui(P2P_STATE,LED,LED1,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR,0);
                            g_uiPvt_call_timer=stn_config.pvt_call_timeout*TWO;
                            g_uiPvt_call_timer_ui_lcd = stn_config.pvt_call_timeout*10*5;
                            g_uiPage_call_timer_frlcd_ini = 0;
                            printf("g_uiPvt_call_timer %d\n",stn_config.pvt_call_timeout);
                            //send_msg_ui(P2P_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_uiPvt_call_timer,LCD_CLR_SECOND_LINE);
                            acgPlayTone_phi(PVT_CHANNEL,VOIP_SIGNAL_STUTTER_TONE,START_TONE,CLEAR);
                            g_uiPlay_tone_timer=THREE * APP_TIMER_MULTIPLIER;
                            //g_ui_sip_p2p_timer = SIP_RESET_TIMEOUT;
                            g_uiFsu_state=P2P_STATE;
                        break;

                        /*case VALID_KEY_SEQ_EVENT:
                            send_to_phone_input_cb(ONE,PHONE_INPUT_HOOK_ON);
                            g_uiFsu_state=IDLE_STATE;*/
                        break;



                case INCOMING_GROUP_CALL_EVENT:
                    printf("INSIDE INCOMING_GROUP_CALL_EVENT of RINGBACK state\n");
                    send_to_phone_input_cb(ONE, PHONE_INPUT_HOOK_ON);
                    // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_ON);
                    write_to_io(CALL_THROUGH_LED_ON, IO_PIN_ON);
                    // drop_membership(g_iAim_fds[stn_config.default_AIM-1]);
                    drop_membership(g_Iicom_fds[pvt_call.curr_icom_no]);
                    add_membership(g_igroup_fds[pvt_call.group_no]);
                    g_uiVoipapp_wait_timer = CLEAR;
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = GROUP;
                    pthread_mutex_unlock(&lock_call_type);
                    g_itransmit_fd_index = GROUP_OFFSET + pvt_call.group_no;
                    sprintf(g_cMulticast_tx_address, "%s%d", gc_group_mul_start_addr, g_igroup_mul_addr_start_offset + pvt_call.group_no);
                    add_route_to_system(g_cMulticast_tx_address);
                    printf("GROUP call event received address is %s \n", g_cMulticast_tx_address);
                    // sprintf(g_cLine3_buf,"GROUP:  %8d",stn_config.logical_id);
                    sprintf(g_cLine2_buf, GRP_CL_msg, lcd_group_call_no, lcd_group_call_init);
                    // pWork_m.private_state=PRIVATE_OUTGOING_ONGOING;
                    // send_msg_ui(GROUP_STATE,LCD,LINE3,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR);
                    send_msg_ui(GROUP_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine2_buf, LCD_CLR_SECOND_LINE, 0);
                    pvt_call.pvt_call_busy = SET;
                    select_codec(PVT_CHANNEL, HANDSET);
                    set_volume(PVT_CHANNEL, MAX_PVT_VOLUME);
                    // send_msg_ui(GROUP_STATE,LED,LED1,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR);
                    send_msg_ui(GROUP_STATE, LED, LED1, CONTINUE_DISPLAY, g_cLine3_buf, CLEAR, 0);
                    g_uiPvt_call_timer = stn_config.pvt_call_timeout * TWO;
                    g_uiPvt_call_timer_ui_lcd = stn_config.pvt_call_timeout * 50;
                    g_uiPage_call_timer_frlcd_ini = 0;
                    acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_STUTTER_TONE, START_TONE, CLEAR);
                    g_uiPlay_tone_timer = THREE * APP_TIMER_MULTIPLIER;
                    g_uiFsu_state = GROUP_STATE;
                    g_uiGroup_PTT_flag = SET; // Added by Yogita for GRP_PTT_REQUEST_EVENT is not received from server.
                    // g_uiSpeech_detection_timer = SPEECH_DETECTION_TIMEOUT;

                    break;



///new_for_MIC_PTT_RESPONSE


             /*    case MIC_PTT_RESPONSE_EVENT :

                    printf("MIC_PTT_RESPONSE_EVENT----\n");
                    g_uiVoipapp_wait_timer = CLEAR;
                    // operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);
                    write_to_io(ICOM_LED_OFF, IO_PIN_OFF);
                    sprintf(g_cMulticast_tx_address, "%s%d", gc_icom_mul_start_addr, g_iIcom_mul_addr_start_offset + pvt_call.curr_icom_no);
                    add_route_to_system(g_cMulticast_tx_address);
                    printf("multicast address mul ofset %s  %d\n", g_cMulticast_tx_address, g_iIcom_mul_addr_start_offset + pvt_call.curr_icom_no);
                    // send_msg_ui(INTERCOM_STATE, LCD, LINE3, CONTINUE_DISPLAY, g_cLine2_buf, LCD_CLR_SECOND_LINE, 0);
                    // print_msg_on_lcd(IOCTL_PRINT, g_cLine2_buf, LINE2, ZERO,LCD_CLR_SECOND_LINE);
                    //  send_msg_ui(INTERCOM_STATE,LCD,LINE3,CONTINUE_DISPLAY,intercom_msg,CLEAR);
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = INTERCOM_TX_RX;
                    g_itransmit_fd_index = ICOM_OFFSET + pvt_call.curr_icom_no;
                    // pvt_call.pvt_call_busy=SET;
                    pthread_mutex_unlock(&lock_call_type);
                    pWork_m.private_state = PRIVATE_OUTGOING_ONGOING;

                    g_uiFsu_state = INTERCOM_STATE;
                    g_uiSpeech_detection_timer = SPEECH_DETECTION_TIMEOUT;

                    break;*/


                case PTT_RESPONSE_EVENT:
                    g_uiVoipapp_wait_timer = CLEAR;
                    // operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);
                    write_to_io(ICOM_LED_OFF, IO_PIN_OFF);
                    sprintf(g_cMulticast_tx_address, "%s%d", gc_icom_mul_start_addr, g_iIcom_mul_addr_start_offset + pvt_call.curr_icom_no);
                    add_route_to_system(g_cMulticast_tx_address);
                    printf("multicast address mul ofset %s  %d\n", g_cMulticast_tx_address, g_iIcom_mul_addr_start_offset + pvt_call.curr_icom_no);
                    // send_msg_ui(INTERCOM_STATE, LCD, LINE3, CONTINUE_DISPLAY, g_cLine2_buf, LCD_CLR_SECOND_LINE, 0);
                    // print_msg_on_lcd(IOCTL_PRINT, g_cLine2_buf, LINE2, ZERO,LCD_CLR_SECOND_LINE);
                    //  send_msg_ui(INTERCOM_STATE,LCD,LINE3,CONTINUE_DISPLAY,intercom_msg,CLEAR);
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = INTERCOM_TX_RX;
                    g_itransmit_fd_index = ICOM_OFFSET + pvt_call.curr_icom_no;
                    // pvt_call.pvt_call_busy=SET;
                    pthread_mutex_unlock(&lock_call_type);
                    pWork_m.private_state = PRIVATE_OUTGOING_ONGOING;

                    g_uiFsu_state = INTERCOM_STATE;
                    g_uiSpeech_detection_timer = SPEECH_DETECTION_TIMEOUT;

                    break;

                /* case P2P_RESET_EVENT:
                    g_uiVoipapp_wait_timer = CLEAR;
                    // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
                    write_to_io(ACT_PVT_PG_MIX_ON, IO_PIN_ON);
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = CLEAR;
                    pthread_mutex_unlock(&lock_call_type);
                    g_uiPvt_call_timer_ui_lcd = CLEAR;
                    one_second_timeout = ONE_SEC_TIMEOUT;
                    if (pg_call.pg_call_busy != SET)
                    {
                        // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                        write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
                    }
                    // send_to_phone_input_cb(ONE,PHONE_INPUT_HOOK_ON);

                    // send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,CLEAR);

                    // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, STATION_busy, LCD_CLR_SECOND_LINE, 0);

                    print_msg_on_lcd(IOCTL_PRINT, STATION_busy, LINE2, ZERO, LCD_CLR_SECOND_LINE);

                    // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, callterminated_msg, LCD_CLR_SECOND_LINE, 0);

                    print_msg_on_lcd(IOCTL_PRINT, callterminated_msg, LINE2, ZERO, LCD_CLR_SECOND_LINE);

                    // send_msg_ui(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,stn_config.fcs_desc,LCD_CLR_FIRST_LINE,0);
                    if (lcd_fcs_default_icom == 0)
                    {
                        printf("@@@@@@@@@@@@@@@@@P2P_RESET_EVENT:@@@@@@@@@@@@@@");
                        sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);

                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    else
                    {
                        printf("%%%%%%%%%%%%%%%%%%%P2P_RESET_EVENT:%%%%%%%%%%%%%%%%%%%%%%");
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, lcd_fcs_default_icom);
                        // sprintf(g_cLine3_buf,Channel_msg,stn_config.logical_id,stn_config.default_intercom); ///
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);

                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }

                    g_uiPvt_call_timer = CLEAR;
                    pvt_call.pvt_call_busy = CLEAR;
                    // Call_cmdid=0;
                    printf("RINGBACK_STATE reset_EVENT received");
                    g_uiFsu_state = IDLE_STATE;
                    if (g_iIcom_mem_dropped == SET && stn_config.icom_live_dead_status[stn_config.default_intercom - MIN_OFFSET] == LIVE)
                    {
                        add_membership(g_Iicom_fds[stn_config.default_intercom - MIN_OFFSET]);
                        reset_intercom();
                        start_intercom();
                    }
                    if (pg_call.pg_call_busy != SET && stn_config.default_AIM == CLEAR)
                    {
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                        write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);
                    }

                    if (g_uiPlay_tone_timer == CLEAR)
                    {
                        printf("************PLAYING BUSY TONE *****************[p2p res] %d\n", g_uiPlay_tone_timer);

                        acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_BUSY_TONE, START_TONE, 1);
                        g_uiPlay_tone_timer = SIX * APP_TIMER_MULTIPLIER;
                    }
                    // start_aim_receive_call();
                    break; */

                    case P2P_RESET_EVENT:
                            g_uiVoipapp_wait_timer=CLEAR;
                           // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
                            pthread_mutex_lock(&lock_call_type);
                            pvt_call.type=CLEAR;
                            pthread_mutex_unlock(&lock_call_type);
                            if(pg_call.pg_call_busy!=SET)
                            {
                               // send_msg_ui_io(IDLE_STATE,IO,CONTINUE_DISPLAY,LED_CALL_THROUGH,CLEAR,&io_led_clear[ZERO],
                              //  IO_ADDRESS,LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
                            }
                            send_to_phone_input_cb(ONE,PHONE_INPUT_HOOK_ON);


                            // send_msg_ui(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,lcd_fcs_desc,LCD_CLR_FIRST_LINE);
                            if (lcd_fcs_default_icom == 0)
                        {
                            sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                            // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);

                            print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                        }
                        else
                        {
                            sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, stn_config.default_intercom);
                            // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);

                            print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
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
                    p2p_call_process_timer_flag = CLEAR;
                    g_uiPvt_call_timer_ui_lcd = CLEAR;
                    one_second_timeout = ONE_SEC_TIMEOUT;
                    g_uiVoipapp_wait_timer = CLEAR;
                    // send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,negative_response_msg,0);
                    g_uiFsu_state = IDLE_STATE;
                    if (g_iIcom_mem_dropped == SET && stn_config.icom_live_dead_status[stn_config.default_intercom - MIN_OFFSET] == LIVE)
                    {
                        add_membership(g_Iicom_fds[stn_config.default_intercom - MIN_OFFSET]);
                        reset_intercom();
                        start_intercom();
                    }
                    if (pg_call.pg_call_busy != SET && stn_config.default_AIM == CLEAR)
                    {
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                        write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);
                    }

                    if (g_uiPlay_tone_timer == CLEAR)
                    {
                        printf("................PLAYING BUSY TONE ...............[rback]\n");
                        acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_BUSY_TONE, START_TONE, 1);
                        g_uiPlay_tone_timer = SIX * APP_TIMER_MULTIPLIER;
                    }
                    // g_uiFsu_state=IDLE_STATE;
                    /// lcd
                    if ((g_cReceived_key_seq[0] == GLOBAL) || (g_cReceived_key_seq[0] == GLOBAL_ALARM) || (g_cReceived_key_seq[0] == ZONE) || (g_cReceived_key_seq[0] == ZONE_ALARM))
                    {
                        printf("Received key sequence global----------------------\n");
                        // send_msg_ui(IDLE_STATE, LCD, LINE1, CONTINUE_DISPLAY, negative_response_msg, LCD_CLR_FIRST_LINE, 0);

                        print_msg_on_lcd(IOCTL_PRINT, negative_response_msg, LINE1, ZERO, LCD_CLR_FIRST_LINE);
                        // usleep(500000);
                        // send_msg_ui(IDLE_STATE, LCD, LINE1, CONTINUE_DISPLAY, lcd_fcs_desc, LCD_CLR_FIRST_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, lcd_fcs_desc, LINE1, ZERO, LCD_CLR_FIRST_LINE);
                    }

                    else
                    {
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, negative_response_msg, LCD_CLR_SECOND_LINE, 0);

                        print_msg_on_lcd(IOCTL_PRINT, negative_response_msg, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                        // sleep(1);
                        if (lcd_fcs_default_icom == 0)
                        {
                            sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                            // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);

                            print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                        }
                        else
                        {
                            sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, stn_config.default_intercom);
                            // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);

                            print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                        }
                    }

                    break;

                case GLOBAL_CALL_EVENT:
                    g_uiVoipapp_wait_timer = CLEAR;
                    // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_ON);
                    write_to_io(CALL_THROUGH_LED_ON, IO_PIN_ON);
                    drop_membership(g_iAim_fds[stn_config.default_AIM - ONE]);
                    if (stn_config.icom_live_dead_status[stn_config.default_intercom - MIN_OFFSET] == LIVE)
                    {
                        drop_membership(g_Iicom_fds[pvt_call.curr_icom_no]);
                        reset_intercom();
                    }
                    add_membership(g_iPaging_fd);
                    // stop_aim_receive_call();

                    g_uiVoipapp_wait_timer = CLEAR;
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = GLOBAL;
                    pthread_mutex_unlock(&lock_call_type);
                    pWork_m.private_state = PRIVATE_OUTGOING_ONGOING;
                    g_itransmit_fd_index = GLOBAL_OFFSET;
                    sprintf(g_cLine3_buf, global_call_msg, lcd_global_call_init);
                    // send_msg_ui(PAGE_INITIATOR_STATE, LCD, LINE1, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_FIRST_LINE, 0);
                    print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE1, ZERO, LCD_CLR_FIRST_LINE);
                    // send_msg_ui(PAGE_INITIATOR_STATE,LCD,LINE3,CONTINUE_DISPLAY,global_call_msg,CLEAR);
                    printf("++++++++gc_global_mul_start_addr:%s,g_iglobal_mul_addr_start_offset:%d\n", gc_global_mul_start_addr, g_iglobal_mul_addr_start_offset);
                    sprintf(g_cMulticast_tx_address, "%s%d", gc_global_mul_start_addr, g_iglobal_mul_addr_start_offset);
                    add_route_to_system(g_cMulticast_tx_address);
                    printf("GLOABAL_CALL_EVENT call event received \n");
                    g_uiPage_call_timer = stn_config.pg_call_timeout * 2;
                    g_uiPage_call_timer_lcd = stn_config.pg_call_timeout * 50;
                    g_uiPvt_call_timer_ui_lcd = 0;
                    g_uiFsu_state = PAGE_INITIATOR_STATE;
                    pvt_call.pvt_call_busy = SET; /////initiator station's pvt get busy
                    multicast_prepare_dsp(PVT_CHANNEL);
                    // select_codec(PG_CHANNEL,NONE);
                    select_codec(PVT_CHANNEL, NONE);
                    select_codec(PVT_CHANNEL, HANDSET);

                    break;

                case GLOBAL_ALM_EVENT:
                    g_uiVoipapp_wait_timer = CLEAR;
                    // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_ON);
                    write_to_io(CALL_THROUGH_LED_ON, IO_PIN_ON);
                    drop_membership(g_iAim_fds[stn_config.default_AIM - ONE]);
                    if (stn_config.icom_live_dead_status[stn_config.default_intercom - MIN_OFFSET] == LIVE)
                    {
                        drop_membership(g_Iicom_fds[pvt_call.curr_icom_no]);
                        reset_intercom();
                    }
                    // stop_aim_receive_call();

                    g_uiVoipapp_wait_timer = CLEAR;
                    pg_call.type = GLOBAL_ALARM;
                    sprintf(g_cLine3_buf, global_alm_msg, lcd_global_alm_no);
                    // send_msg_ui(PAGE_INITIATOR_STATE,LCD,LINE3,CONTINUE_DISPLAY,global_alm_msg,CLEAR);
                    // send_msg_ui(PAGE_INITIATOR_STATE, LCD, LINE1, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_FIRST_LINE, 0);
                    print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE1, ZERO, LCD_CLR_FIRST_LINE);
                    // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                    write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
                    printf("GLOABAL_ALARM_CALL_EVENT call event received\n");
                    g_uiPage_call_timer = stn_config.alarm_call_timeout * TWO;
                    g_uiPage_call_timer = stn_config.alarm_call_timeout * TWO;
                    g_uiPage_call_timer_lcd = stn_config.alarm_call_timeout * 50;
                    g_uiFsu_state = PAGE_INITIATOR_STATE;
                    pg_call.pg_call_busy = SET;
                    // pvt_call.pvt_call_busy=SET;
                    multicast_prepare_dsp(PG_CHANNEL);
                    // select_codec(PVT_CHANNEL,NONE);
                    select_codec(PG_CHANNEL, NONE);
                    // multicast_prepare_dsp(PG_CHANNEL);
                    select_codec(PG_CHANNEL, SPEAKER);
                    stop_alarm(pg_call.alm_no);
                    // sleep(1);
                    //start_alarm(pg_call.alm_no);

                    break;

                case GLOBAL_RESET_EVENT:
                    printf("INSIDE GLOBAL_RESET_EVENT\n");
                    g_uiVoipapp_wait_timer = CLEAR;
                    pg_call.alm_no = CLEAR;

                    g_uiPage_call_timer_lcd = CLEAR;
                    g_uiPage_call_timer_frlcd_ini = CLEAR;
                    one_second_timeout = ONE_SEC_TIMEOUT;
                    // send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                    // send_msg_ui(IDLE_STATE, LCD, LINE1, CONTINUE_DISPLAY, global_terminate, LCD_CLR_FIRST_LINE, 0);
                    print_msg_on_lcd(IOCTL_PRINT, global_terminate, LINE1, ZERO, LCD_CLR_FIRST_LINE);
                    // usleep(500000);
                    // send_msg_ui(IDLE_STATE, LCD, LINE1, CONTINUE_DISPLAY, lcd_fcs_desc, LCD_CLR_FIRST_LINE, 0);
                    print_msg_on_lcd(IOCTL_PRINT, lcd_fcs_desc, LINE1, ZERO, LCD_CLR_FIRST_LINE);
                    // send_msg_ui(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,stn_config.fcs_desc,LCD_CLR_FIRST_LINE,0);
                    pWork_m.page_state = PAGE_IDLE;
                    pg_call.type = CLEAR;
                    // Call_cmdid=0;
                    pg_call.pg_call_busy = CLEAR;
                    g_uiPage_call_timer = CLEAR;
                    // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                    write_to_io(CALL_THROUGH_LED_ON, IO_PIN_ON);
                    if (pg_call.pg_call_busy != SET && stn_config.default_AIM == CLEAR)
                    {
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                        write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);
                    }
                    pvt_call.curr_icom_no = stn_config.default_intercom - MIN_OFFSET;
                    if (g_iIcom_mem_dropped == SET && stn_config.icom_live_dead_status[stn_config.default_intercom - MIN_OFFSET] == LIVE)
                    {
                        add_membership(g_Iicom_fds[stn_config.default_intercom - MIN_OFFSET]);
                    }
                    if (stn_config.default_AIM)
                    {
                        add_membership(g_iAim_fds[stn_config.default_AIM - ONE]);
                        start_aim_receive_call();
                    }
                    //  start_intercom();

                    break;

                case NO_P2P_RESPONSE_EVENT:
                    printf("##########NO_P2P_RESPONSE_EVENT IN RINGBACK_STATE##########\n");
                    if (g_cReceived_key_seq[0] == P2P)
                    {
                        // send_to_phone_input_cb(ONE,PHONE_INPUT_HOOK_ON);
                        if (g_iIcom_mem_dropped == SET && stn_config.icom_live_dead_status[stn_config.default_intercom - MIN_OFFSET] == LIVE)
                        {
                            add_membership(g_Iicom_fds[stn_config.default_intercom - MIN_OFFSET]);
                            reset_intercom();
                            start_intercom();
                        }
                        p2p_call_process_timer_flag = CLEAR;
                        g_uiPvt_call_timer_ui_lcd = CLEAR;
                        one_second_timeout = ONE_SEC_TIMEOUT;
                        // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                        write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
                    }
                    // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                    print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    // send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                    g_uiFsu_state = IDLE_STATE;
                    //  printf("RINGBACK STATE TIMEOUT \n");
                    if (lcd_fcs_default_icom == 0)
                    {
                        sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    else
                    {
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, lcd_fcs_default_icom);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    break;

                case APP_TIMEOUT_EVENT:
                    printf("##########RINGBACK STATE TIMEOUT##########\n");

                    if (g_cReceived_key_seq[0] == P2P)
                    {

                        send_to_phone_input_cb(ONE,PHONE_INPUT_HOOK_ON);
                        printf("out of retransmission if condition\n");
                        if (g_iIcom_mem_dropped == SET && stn_config.icom_live_dead_status[stn_config.default_intercom - MIN_OFFSET] == LIVE)
                        {
                            add_membership(g_Iicom_fds[stn_config.default_intercom - MIN_OFFSET]);
                            reset_intercom();
                            start_intercom();
                        }
                        p2p_call_process_timer_flag = CLEAR;
                        g_uiPvt_call_timer_ui_lcd = CLEAR;
                        one_second_timeout = ONE_SEC_TIMEOUT;
                        // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                        write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
                        send_req_msg_to_server(0, P2P_NO_RESPONSE, P2P_CALL_FRAME_LENGTH, '0', '0');
                    }
                    // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                    print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    // send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                    g_uiFsu_state = IDLE_STATE;
                    //  printf("RINGBACK STATE TIMEOUT \n");
                    if (lcd_fcs_default_icom == 0)
                    {
                        sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    else
                    {
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, lcd_fcs_default_icom);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    break;
                }
                g_uiFsu_event = CLEAR;
                break;

            case INTERCOM_STATE:
                switch (g_uiFsu_event)
                {
                case WAKE_UP_EVENT:
                    printf("____________INTERCOM_STATE_______WAKE_UP_EVENT___\n");
                    reset_all();
                    g_uiFsu_state = VCCS_REGISTERING_STATE;

                    break;

                    /*case BROADCAST_TIMEOUT_EVENT:
                         printf("Inside BROADCAST_TIMEOUT_EVENT\n");
                         Restart_Broadcast_Socket();
                    break;*/

                case VALID_KEY_SEQ_EVENT:
                    switch (g_cReceived_key_seq[0])
                    {
                    case RESET:
                        send_req_msg_to_server(0, ICOM_PTT_RELEASE_REQ, ICOM_CALL_FRAME_LENGTH, '1', '0');
                        last_req_sent_to_server = ICOM_PTT_RELEASE_REQ;
                        req_retransmission_count = 0;
                        g_uiVoipapp_wait_timer = SERVER_RESPONSE_TIMEOUT;
                        g_uiFsu_state = INTERCOM_RESET_RINGBACK_STATE;
                        // send_msg_ui(INTERCOM_RESET_RINGBACK_STATE,LCD,LINE3,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR);
                        // send_msg_ui(INTERCOM_RESET_RINGBACK_STATE, LCD, LINE2, CONTINUE_DISPLAY, reset_msg, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, reset_msg, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                        // sleep(1);
                        break;

                    default:
                        break;
                    }

                    break;

                case PTT_RELEASE_EVENT:
                    send_req_msg_to_server(0, ICOM_PTT_RELEASE_REQ, ICOM_CALL_FRAME_LENGTH, '1', '0');
                    g_uiVoipapp_wait_timer = SERVER_RESPONSE_TIMEOUT;
                    break;

                case RINGING_OFF_HOOK_EVENT:
                    drop_membership(g_Iicom_fds[pvt_call.curr_icom_no]);
                    // reset_intercom();
                    // send_to_phone_input_cb(ONE,PHONE_INPUT_HOOK_OFF);
                    printf("OFF HOOK EVENT RECEIVED \n");

                    break;

                case PVT_CALL_TIMEOUT:
                case ICOM_STATE_CHANGE_EVENT:
                    printf("inside  INTERCOM_STATE event is PVT_CALL_TIMEOUT \n");
                    del_route_from_system(g_cMulticast_tx_address);
                    send_req_msg_to_server(0, ICOM_PTT_RELEASE_REQ, ICOM_CALL_FRAME_LENGTH, '1', '0');
                    // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                    write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
                    // send_msg_ui(IDLE_STATE,LCD,LINE3,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                    // send_msg_ui(IDLE_STATE, LCD, LINE3, CONTINUE_DISPLAY, reset_msg, CLEAR, 0);
                    print_msg_on_lcd(IOCTL_PRINT, reset_msg, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    if (stn_config.icom_live_dead_status[stn_config.default_intercom - MIN_OFFSET] == LIVE)
                    {
                        drop_membership(g_Iicom_fds[pvt_call.curr_icom_no]);
                    }
                    g_uiPvt_call_timer = CLEAR;
                    // operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);
                    write_to_io(ICOM_LED_OFF, IO_PIN_OFF);
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = INTERCOM_RX;
                    pthread_mutex_unlock(&lock_call_type);
                    pvt_call.pvt_call_busy = CLEAR;
                    g_uiPvt_call_timer_ui_lcd = CLEAR;
                    one_second_timeout = ONE_SEC_TIMEOUT;
                    pWork_m.private_state = PRIVATE_IDLE;
                    if (pg_call.pg_call_busy != SET && stn_config.default_AIM == CLEAR)
                    {
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                        write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);
                    }
                    pvt_call.curr_icom_no = stn_config.default_intercom - MIN_OFFSET;
                    if (stn_config.icom_live_dead_status[stn_config.default_intercom - MIN_OFFSET] == LIVE)
                    {
                        add_membership(g_Iicom_fds[stn_config.default_intercom - MIN_OFFSET]);
                    }
                    g_uiFsu_state = IDLE_STATE;
                    printf("INTERCOM PVT TIMEROUT \n");
                    acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_STUTTER_TONE, START_TONE, CLEAR);
                    g_uiPlay_tone_timer = THREE * APP_TIMER_MULTIPLIER;
                    break;

                case P2P_CALL_EVENT:
                    g_uiVoipapp_wait_timer = CLEAR;
                    g_uiPvt_call_timer = CLEAR;
                    // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_ON);
                    write_to_io(CALL_THROUGH_LED_ON, IO_PIN_ON);
                    //  if(g_iHandset_lifted_signal!=SET)
                    if (g_iHandset_lifted_signal == CLEAR)
                    {
                        // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_OFF);
                        write_to_io(ACT_PVT_PG_MIX_OFF, IO_PIN_OFF);
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                        write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
                    }
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = P2P;
                    pthread_mutex_unlock(&lock_call_type);
                    printf("P2p call event received - ICOM state \n");
                    printf("g_cCaller %s stn id %d \n", g_cCalled_station, stn_config.logical_id);
                    // sprintf(g_cLine3_buf,"P2P:%s   %8d\n",g_cCaller,stn_config.logical_id);
                    sprintf(g_cLine3_buf, P2P_msg, g_cCalled_station);
                    printf("line3buf=%s\n", g_cLine3_buf);
                    select_codec(PVT_CHANNEL, HANDSET);
                    set_volume(PVT_CHANNEL, MAX_PVT_VOLUME);
                    // send_msg_ui(P2P_STATE,LCD,LINE3,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR);
                    // send_msg_ui(P2P_STATE,LED,LED1,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR);
                    // send_msg_ui(P2P_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                    print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    send_msg_ui(P2P_STATE, LED, LED1, CONTINUE_DISPLAY, g_cLine3_buf, CLEAR, 0);
                    g_uiPvt_call_timer = stn_config.pvt_call_timeout * TWO;
                    g_uiPvt_call_timer_ui_lcd = stn_config.pvt_call_timeout * 50;
                    g_uiPage_call_timer_frlcd_ini = 0;
                    acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_STUTTER_TONE, START_TONE, CLEAR);
                    g_uiPlay_tone_timer = THREE * APP_TIMER_MULTIPLIER;
                    pvt_call.pvt_call_busy = SET;
                    g_uiFsu_state = P2P_STATE;
                    break;

                case INCOMING_GROUP_CALL_EVENT:
                    if (stn_config.icom_live_dead_status[stn_config.default_intercom - MIN_OFFSET] == LIVE)
                    {
                        drop_membership(g_Iicom_fds[pvt_call.curr_icom_no]);
                    }

                    // drop_membership(g_iAim_fds[stn_config.default_AIM-1]);
                    add_membership(g_igroup_fds[pvt_call.group_no]);
                    // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_ON);
                    write_to_io(CALL_THROUGH_LED_ON, IO_PIN_ON);
                    //  if(g_iHandset_lifted_signal!=SET)
                    if (g_iHandset_lifted_signal == CLEAR)
                    {
                        // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_OFF);
                        write_to_io(ACT_PVT_PG_MIX_OFF, IO_PIN_OFF);
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                        write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
                    }
                    // operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);
                    write_to_io(ICOM_LED_OFF, IO_PIN_OFF);
                    g_uiVoipapp_wait_timer = CLEAR;
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = GROUP;
                    pthread_mutex_unlock(&lock_call_type);
                    printf("GROUP call event received");
                    // sprintf(g_cLine3_buf,"GROUP:   %8d",stn_config.logical_id);
                    // send_msg_ui(GROUP_STATE,LCD,LINE3,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR);
                    sprintf(g_cLine3_buf, GRP_CL_msg, lcd_group_call_no, lcd_group_call_init);
                    // send_msg_ui(GROUP_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                    print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    pvt_call.pvt_call_busy = SET;
                    select_codec(PVT_CHANNEL, HANDSET);
                    set_volume(PVT_CHANNEL, MAX_PVT_VOLUME);
                    send_msg_ui(GROUP_STATE, LED, LED1, CONTINUE_DISPLAY, g_cLine3_buf, CLEAR, 0);

                    // send_msg_ui(GROUP_STATE,LED,LED1,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR);
                    g_uiPvt_call_timer = stn_config.pvt_call_timeout * TWO;
                    g_uiPvt_call_timer_ui_lcd = stn_config.pvt_call_timeout * 50;
                    g_uiPage_call_timer_frlcd_ini = 0;
                    acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_STUTTER_TONE, START_TONE, CLEAR);
                    g_uiPlay_tone_timer = THREE * APP_TIMER_MULTIPLIER;
                    g_uiFsu_state = GROUP_STATE;
                    g_uiSpeech_detection_timer = SPEECH_DETECTION_TIMEOUT;

                    break;

                case PTT_RESPONSE_EVENT:
                    printf("inside  Intercom_state event is PTT_RESPONSE_EVENT \n");
                    /*if(g_iIcom_hop_flag==SET)
                    {
                        printf("in ICOM reset dialling call \n");
                        //operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                        pvt_call.curr_icom_no=stn_config.default_intercom-MIN_OFFSET;
                        if(stn_config.icom_live_dead_status[stn_config.default_intercom-MIN_OFFSET]==LIVE)
                        add_membership(g_Iicom_fds[stn_config.default_intercom-MIN_OFFSET]);
                        g_iIcom_hop_flag=CLEAR;
                     }
                     del_route_from_system(g_cMulticast_tx_address);
                     //operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);
                     send_msg_ui(IDLE_STATE,LCD,LINE3,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                     bzero(g_cMulticast_tx_address,sizeof(g_cMulticast_tx_address));
                     pWork_m.private_state=PRIVATE_IDLE;
                     pthread_mutex_lock(&lock_call_type);
                     pvt_call.type=INTERCOM_RX;
                     pthread_mutex_unlock(&lock_call_type);
                     send_msg_ui(IDLE_STATE,LCD,LINE3,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                     g_uiFsu_state=IDLE_STATE;*/
                    g_uiVoipapp_wait_timer = CLEAR;
                    del_route_from_system(g_cMulticast_tx_address);
                    // //operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                    write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
                    // operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);
                    write_to_io(ICOM_LED_OFF, IO_PIN_OFF);
                    // send_msg_ui(IDLE_STATE, LCD, LINE3, CONTINUE_DISPLAY, reset_msg, LCD_CLR_SECOND_LINE, 0);
                    print_msg_on_lcd(IOCTL_PRINT, reset_msg, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    // send_msg_ui(IDLE_STATE,LCD,LINE3,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                    bzero(g_cMulticast_tx_address, sizeof(g_cMulticast_tx_address));
                    pWork_m.private_state = PRIVATE_IDLE;
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = INTERCOM_RX;
                    pthread_mutex_unlock(&lock_call_type);

                     if (lcd_fcs_default_icom == 0)
                    {
                        sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    else
                    {
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, lcd_fcs_default_icom);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }

                    /*  if(lcd_fcs_default_icom == 0)
                          {
                              sprintf(g_cLine3_buf,IDLE_msg,stn_config.logical_id);
                              send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,LCD_CLR_SECOND_LINE,0);
                          }
                         else
                          {
                              sprintf(g_cLine3_buf,Channel_msg,stn_config.logical_id,stn_config.default_intercom);
                              send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,LCD_CLR_SECOND_LINE,0);
                          }  */

                    // send_msg_ui(IDLE_STATE,LCD,LINE3,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                    // operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);
                    write_to_io(ICOM_LED_OFF, IO_PIN_OFF);

                    g_uiFsu_state = IDLE_STATE;

                    break;
                }
                g_uiFsu_event = CLEAR;
                break;

            case INTERCOM_RESET_RINGBACK_STATE:
                switch (g_uiFsu_event)
                {
                case WAKE_UP_EVENT:
                    printf("____________INTERCOM_RESET_RINGBACK_STATE_______WAKE_UP_EVENT___\n");
                    reset_all();
                    g_uiFsu_state = VCCS_REGISTERING_STATE;

                    break;

                    /* case BROADCAST_TIMEOUT_EVENT:
                          printf("Inside BROADCAST_TIMEOUT_EVENT\n");
                          Restart_Broadcast_Socket();
                     break;*/

                case PTT_RESPONSE_EVENT:
                    // case APP_TIMEOUT_EVENT:
                    g_uiVoipapp_wait_timer = CLEAR;
                    printf("inside  INTERCOM_RESET_RINGBACK_STATE event is PTT_RESPONSE_EVENT \n");
                    /*if(g_iIcom_hop_flag==SET)
                    {
                        printf("in ICOM reset dialling call \n");
                        //operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                        pvt_call.curr_icom_no=stn_config.default_intercom-MIN_OFFSET;
                        if(stn_config.icom_live_dead_status[stn_config.default_intercom-MIN_OFFSET]==LIVE)
                        add_membership(g_Iicom_fds[stn_config.default_intercom-MIN_OFFSET]);
                        g_iIcom_hop_flag=CLEAR;
                     }
                     del_route_from_system(g_cMulticast_tx_address);
                     //operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);
                     send_msg_ui(IDLE_STATE,LCD,LINE3,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                     bzero(g_cMulticast_tx_address,sizeof(g_cMulticast_tx_address));
                     pWork_m.private_state=PRIVATE_IDLE;
                     pthread_mutex_lock(&lock_call_type);
                     pvt_call.type=INTERCOM_RX;
                     pthread_mutex_unlock(&lock_call_type);
                     send_msg_ui(IDLE_STATE,LCD,LINE3,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                     g_uiFsu_state=IDLE_STATE;*/
                    if (g_iIcom_hop_flag == SET)
                    {
                        printf("in ICOM reset dialling call \n");

                        del_route_from_system(g_cMulticast_tx_address);
                        // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                        write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
                        // operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);
                        write_to_io(ICOM_LED_OFF, IO_PIN_OFF);
                        // send_msg_ui(IDLE_STATE,LCD,LINE3,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                        send_msg_ui(IDLE_STATE, LCD, LINE3, CONTINUE_DISPLAY, page_reset_msg, LCD_CLR_SECOND_LINE, CLEAR);
                        bzero(g_cMulticast_tx_address, sizeof(g_cMulticast_tx_address));
                        pWork_m.private_state = PRIVATE_IDLE;
                        pvt_call.curr_icom_no = stn_config.default_intercom - MIN_OFFSET;
                        if (stn_config.icom_live_dead_status[stn_config.default_intercom - MIN_OFFSET] == LIVE)
                            add_membership(g_Iicom_fds[stn_config.default_intercom - MIN_OFFSET]);
                        pthread_mutex_lock(&lock_call_type);
                        pvt_call.type = INTERCOM_RX;
                        pthread_mutex_unlock(&lock_call_type);
                        g_iIcom_hop_flag = CLEAR;
                        // send_msg_ui(IDLE_STATE,LCD,LINE3,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                        // send_msg_ui(IDLE_STATE, LCD, LINE3, CONTINUE_DISPLAY, page_reset_msg, LCD_CLR_SECOND_LINE, CLEAR);
                        print_msg_on_lcd(IOCTL_PRINT, page_reset_msg, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                        g_uiFsu_state = IDLE_STATE;
                    }
                    del_route_from_system(g_cMulticast_tx_address);
                    // operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);
                    write_to_io(ICOM_LED_OFF, IO_PIN_OFF);
                    // send_msg_ui(IDLE_STATE,LCD,LINE3,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                    // send_msg_ui(IDLE_STATE, LCD, LINE3, CONTINUE_DISPLAY, page_reset_msg, LCD_CLR_SECOND_LINE, CLEAR);
                    print_msg_on_lcd(IOCTL_PRINT, page_reset_msg, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    bzero(g_cMulticast_tx_address, sizeof(g_cMulticast_tx_address));
                    pWork_m.private_state = PRIVATE_IDLE;
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = INTERCOM_RX;
                    pthread_mutex_unlock(&lock_call_type);
                    // operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);
                    write_to_io(ICOM_LED_OFF, IO_PIN_OFF);
                    // send_msg_ui(IDLE_STATE,LCD,LINE3,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                    // send_msg_ui(IDLE_STATE, LCD, LINE3, CONTINUE_DISPLAY, page_reset_msg, LCD_CLR_SECOND_LINE, CLEAR);
                    print_msg_on_lcd(IOCTL_PRINT, page_reset_msg, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    g_uiFsu_state = IDLE_STATE;

                    if (lcd_fcs_default_icom == 0)
                    {
                        sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    else
                    {
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, lcd_fcs_default_icom);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    break;

                case APP_TIMEOUT_EVENT:
                    printf("Inside INTERCOM_RESET_RINGBACK_STATE APP_TIMEOUT_EVENT\n");
                    if (req_retransmission_count == 0)
                    {
                        send_req_msg_to_server(0, last_req_sent_to_server, P2P_CALL_FRAME_LENGTH, '0', '0');
                        printf("*****Retransmission of Reset frame to server******\n");
                        g_uiVoipapp_wait_timer = SERVER_RESPONSE_TIMEOUT;
                        req_retransmission_count = 1;
                        break;
                    }
                    g_uiPvt_call_timer_ui_lcd = CLEAR;
                    one_second_timeout = ONE_SEC_TIMEOUT;

                    g_uiVoipapp_wait_timer = CLEAR;
                    printf("inside  INTERCOM_RESET_RINGBACK_STATE event is APP_TIMEOUT_EVENT \n");
                    /*if(g_iIcom_hop_flag==SET)
                    {
                        printf("in ICOM reset dialling call \n");
                        //operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                        pvt_call.curr_icom_no=stn_config.default_intercom-MIN_OFFSET;
                        if(stn_config.icom_live_dead_status[stn_config.default_intercom-MIN_OFFSET]==LIVE)
                        add_membership(g_Iicom_fds[stn_config.default_intercom-MIN_OFFSET]);
                        g_iIcom_hop_flag=CLEAR;
                     }
                     del_route_from_system(g_cMulticast_tx_address);
                     //operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);

                     send_msg_ui(IDLE_STATE,LCD,LINE3,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                     bzero(g_cMulticast_tx_address,sizeof(g_cMulticast_tx_address));
                     pWork_m.private_state=PRIVATE_IDLE;
                     pthread_mutex_lock(&lock_call_type);
                     pvt_call.type=INTERCOM_RX;
                     pthread_mutex_unlock(&lock_call_type);
                     send_msg_ui(IDLE_STATE,LCD,LINE3,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                     g_uiFsu_state=IDLE_STATE;*/
                    if (g_iIcom_hop_flag == SET)
                    {
                        printf("in ICOM reset dialling call \n");

                        del_route_from_system(g_cMulticast_tx_address);
                        // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                        write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
                        // operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);
                        write_to_io(ICOM_LED_OFF, IO_PIN_OFF);
                        // send_msg_ui(IDLE_STATE,LCD,LINE3,CONTINUE_DISPLAY,page_reset_msg,LCD_CLR_SECOND_LINE,CLEAR);
                        // send_msg_ui(IDLE_STATE, LCD, LINE3, CONTINUE_DISPLAY, page_reset_msg, LCD_CLR_SECOND_LINE, CLEAR);
                        print_msg_on_lcd(IOCTL_PRINT, page_reset_msg, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                        bzero(g_cMulticast_tx_address, sizeof(g_cMulticast_tx_address));
                        pWork_m.private_state = PRIVATE_IDLE;
                        pvt_call.curr_icom_no = stn_config.default_intercom - MIN_OFFSET;
                        if (stn_config.icom_live_dead_status[stn_config.default_intercom - MIN_OFFSET] == LIVE)
                            add_membership(g_Iicom_fds[stn_config.default_intercom - MIN_OFFSET]);
                        pthread_mutex_lock(&lock_call_type);
                        pvt_call.type = INTERCOM_RX;
                        pthread_mutex_unlock(&lock_call_type);
                        g_iIcom_hop_flag = CLEAR;
                        // send_msg_ui(IDLE_STATE,LCD,LINE3,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                        // send_msg_ui(IDLE_STATE, LCD, LINE3, CONTINUE_DISPLAY, page_reset_msg, LCD_CLR_SECOND_LINE, CLEAR);
                        print_msg_on_lcd(IOCTL_PRINT, page_reset_msg, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                        g_uiFsu_state = IDLE_STATE;
                    }
                    del_route_from_system(g_cMulticast_tx_address);
                    // operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);
                    write_to_io(ICOM_LED_OFF, IO_PIN_OFF);
                    // send_msg_ui(IDLE_STATE,LCD,LINE3,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                    // send_msg_ui(IDLE_STATE, LCD, LINE3, CONTINUE_DISPLAY, page_reset_msg, LCD_CLR_SECOND_LINE, CLEAR);
                    print_msg_on_lcd(IOCTL_PRINT, page_reset_msg, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    bzero(g_cMulticast_tx_address, sizeof(g_cMulticast_tx_address));
                    pWork_m.private_state = PRIVATE_IDLE;
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = INTERCOM_RX;
                    pthread_mutex_unlock(&lock_call_type);
                    // operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);
                    write_to_io(ICOM_LED_OFF, IO_PIN_OFF);
                    // send_msg_ui(IDLE_STATE,LCD,LINE3,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                    // send_msg_ui(IDLE_STATE, LCD, LINE3, CONTINUE_DISPLAY, page_reset_msg, LCD_CLR_SECOND_LINE, CLEAR);
                    print_msg_on_lcd(IOCTL_PRINT, page_reset_msg, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    g_uiFsu_state = IDLE_STATE;

                    /*  if(lcd_fcs_default_icom == 0)
                         {
                             sprintf(g_cLine3_buf,IDLE_msg,stn_config.logical_id);
                             send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,LCD_CLR_SECOND_LINE,0);
                         }
                         else
                         {
                             sprintf(g_cLine3_buf,Channel_msg,stn_config.logical_id,lcd_fcs_default_icom);
                             send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,LCD_CLR_SECOND_LINE,0);

                         }  */

                    break;

                default:
                    break;
                }
                g_uiFsu_event = CLEAR;
                break;

            /* case P2P_STATE:

                switch (g_uiFsu_event)
                {

                case WAKE_UP_EVENT:
                    printf("____________P2P_STATE______WAKE_UP_EVENT___\n");
                    reset_all();
                    g_uiVoipapp_wait_timer = CLEAR;
                    g_uiFsu_state = VCCS_REGISTERING_STATE;
                    break;

                    /* case BROADCAST_TIMEOUT_EVENT:
                          printf("Inside BROADCAST_TIMEOUT_EVENT\n");
                          Restart_Broadcast_Socket();
                     break;

                case VALID_KEY_SEQ_EVENT:
                    printf("%c valid key in the p2p state \n", g_cReceived_key_seq[0]);
                    switch (g_cReceived_key_seq[0])
                    {
                    case CONFERENCE:
                        printf("CONFERENCE REQ FRAME SENT \n");
                        bzero(g_cCalled_station, sizeof(g_cCalled_station));
                        g_cCalled_station[0] = g_cReceived_key_seq[1];
                        g_cCalled_station[1] = g_cReceived_key_seq[2];
                        g_cCalled_station[2] = g_cReceived_key_seq[3];
                        send_req_msg_to_server(0, CONF_CALL_REQ, CONF_CALL_FRAME_LENGTH, '0', '0');
                        g_uiVoipapp_wait_timer = SERVER_RESPONSE_TIMEOUT;
                        g_uiFsu_state = CONF_INIT_STATE;
                        break;

                    case RESET:
                        printf("Reset key is pressed in P2P_STATE....\n");
                        send_req_msg_to_server(0, P2P_CALL_RESET_REQ, P2P_CALL_FRAME_LENGTH, '0', '0');
                        last_req_sent_to_server = P2P_CALL_RESET_REQ;
                        req_retransmission_count = 0;
                        g_uiPvt_call_timer_ui_lcd = CLEAR;
                        one_second_timeout = ONE_SEC_TIMEOUT;
                        p2p_call_process_timer_flag = SET;
                        // send_msg_ui(P2P_RESET_RINGBACK_STATE, LCD, LINE2, CONTINUE_DISPLAY, reset_msg, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, reset_msg, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                        // sleep(1);
                        print_msg_on_lcd(IOCTL_PRINT, page_reset_msg, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                        // send_msg_ui(P2P_RESET_RINGBACK_STATE,LCD,LINE2,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                        // send_to_phone_input_cb(1,PHONE_INPUT_HOOK_ON);
                        g_uiFsu_state = P2P_RESET_RINGBACK_STATE;
                        g_uiVoipapp_wait_timer = SERVER_RESPONSE_TIMEOUT;

                        break;

                        case PTT_KEY:
                        printf("IN GLOBAL INITIATOR PTT KEY PRESSED ----*-*-*-*-*-*-*--");
                        select_codec(PVT_CHANNEL, HANDSET);
                        break;
                    }

                    break;

                case CONF_ADD_EVENT:
                    g_uiVoipapp_wait_timer = CLEAR;
                    printf("p2p reset IN P2P state event changing state to IDLE\n");
                    // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
                    write_to_io(ACT_PVT_PG_MIX_ON, IO_PIN_ON);
                    if (pg_call.pg_call_busy != SET)
                    {
                        // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                        write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
                    }
                    if (pg_call.pg_call_busy == CLEAR)
                    {
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                        write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
                    }
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = CLEAR;
                    pthread_mutex_unlock(&lock_call_type);
                    // send_to_phone_input_cb(1,PHONE_INPUT_HOOK_ON);
                    if (pg_call.pg_call_busy != SET && stn_config.default_AIM == CLEAR)
                    {
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                        write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);
                    }

                    g_uiPvt_call_timer = CLEAR;
                    select_codec(PVT_CHANNEL, NONE);
                    close_channel(PVT_CHANNEL);
                    pvt_call.pvt_call_busy = CLEAR;
                    g_uiFsu_state = CONF_INIT_STATE;
                    g_uiVoipapp_wait_timer = SERVER_RESPONSE_TIMEOUT;
                    sprintf(g_cLine3_buf, CONF_msg, g_cCalled_station);
                    // send_msg_ui(CONF_INIT_STATE, LCD, LINE3, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_FIRST_10_POSITION, 0);
                    print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    // send_msg_ui(CONF_INIT_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,CLEAR);

                    break;


                    case PVT_CALL_TIMEOUT:
                        printf("###########PVT_CALL_TIMEOUT in P2P_STATE##########\n");


                        /*if(pvt_call.initiator)
                        {
                                send_req_msg_to_server(0,P2P_CALL_RESET_REQ,P2P_CALL_FRAME_LENGTH,'0','0');
                                send_msg_ui(P2P_RESET_RINGBACK_STATE,LCD,LINE2,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                                //send_to_phone_input_cb(1,PHONE_INPUT_HOOK_ON);
                                g_uiFsu_state = P2P_RESET_RINGBACK_STATE;
                                g_uiVoipapp_wait_timer = SERVER_RESPONSE_TIMEOUT;
                        }
                       // else
                       // {
                            //operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                            pthread_mutex_lock(&lock_call_type);
                            pvt_call.type=CLEAR;
                            pthread_mutex_unlock(&lock_call_type);
                            sprintf(g_cLine3_buf,"IDLE %10d",stn_config.logical_id);
                            send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                            send_msg_ui(IDLE_STATE,LCD,LINE3,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR);
                            send_msg_ui(IDLE_STATE,LCD,LINE4,CONTINUE_DISPLAY,stn_config.fcs_desc,CLEAR);
                            //send_to_phone_input_cb(ONE,PHONE_INPUT_HOOK_ON);
                            printf("PVT_CALL_TIMEOUT changing state to IDLE\n");
                            g_uiPvt_call_timer=CLEAR;
                            pvt_call.pvt_call_busy=CLEAR;
                            acgPlayTone_phi(PVT_CHANNEL,VOIP_SIGNAL_STUTTER_TONE,START_TONE,CLEAR);
                            g_uiPlay_tone_timer=THREE;
                            p2p_dsp_stop_connection(PVT_CHANNEL);
                            select_codec(PVT_CHANNEL,NONE);
                            g_uiFsu_state=IDLE_STATE;

                            if(pg_call.pg_call_busy!=SET && stn_config.default_AIM==CLEAR)
                            {
                                //operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                            }
                            if(g_iIcom_mem_dropped==SET)
                            {
                                add_membership(g_Iicom_fds[stn_config.default_intercom-MIN_OFFSET]);
                                reset_intercom();
                                start_intercom();
                            }
                            if(g_iHandset_lifted_signal==CLEAR)
                            {
                                printf("//////////ICOM_MIX_ACt is off after icom membership add in call timeout.......\n");
                                //operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_OFF);

                            }
                            else
                            {
                                //operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
                            }

                        //}


                    break;

                case P2P_RESET_PVTTIMEOUT_EVENT:
                    p2p_call_process_timer_flag = CLEAR;
                    printf("inside  P2P_STATE event is P2P_RESET_PVTTIMEOUT_EVENT \n");
                    g_uiVoipapp_wait_timer = CLEAR;

                    if (pg_call.pg_call_busy != SET)
                    {
                        // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                        write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
                    }
                    if (pg_call.pg_call_busy == CLEAR)
                    {
                        printf("....................MUSIC_VOl_BYPASS is OFF when no page call........\n");
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);///change
                        write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);
                    }
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = CLEAR;
                    pthread_mutex_unlock(&lock_call_type);

                    // networking_rtpChannelInfoDelete(PVT_CHANNEL);
                    p2p_dsp_stop_connection(PVT_CHANNEL);
                    select_codec(PVT_CHANNEL, NONE);

                    // send_to_phone_input_cb(1,PHONE_INPUT_HOOK_ON);
                    if (pg_call.pg_call_busy != SET && stn_config.default_AIM == CLEAR)
                    {
                        printf("MUSIC_VOl_BYPASS is ON after P2p reset........\n");
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                        write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);
                    }
                    if (g_iIcom_mem_dropped == SET)
                    {
                        add_membership(g_Iicom_fds[stn_config.default_intercom - MIN_OFFSET]);
                        reset_intercom();
                        start_intercom();
                    }
                    g_uiPvt_call_timer = CLEAR;

                    g_uiPvt_call_timer_ui_lcd = CLEAR;
                    one_second_timeout = ONE_SEC_TIMEOUT;
                    g_uiFsu_state = IDLE_STATE;
                    printf("p2p reset IN P2P state event changing state to IDLE\n");
                    /// send_req_msg_to_server(0,P2P_CALL_RESET_ACK,P2P_CALL_FRAME_LENGTH,'0','0');
                    // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine2_buf, LCD_CLR_SECOND_LINE, CLEAR);
                    //  print_msg_on_lcd(IOCTL_PRINT, g_cLine2_buf, LINE2, ZERO,LCD_CLR_SECOND_LINE);

                    // send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,CLEAR);

                    acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_STUTTER_TONE, START_TONE, CLEAR);

                    g_uiPlay_tone_timer = THREE * APP_TIMER_MULTIPLIER;

                    if (g_iHandset_lifted_signal == CLEAR)
                    {
                        printf("//////////ICOM_MIX_ACt is off after icom membership add.......\n");
                        // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_OFF);
                        write_to_io(ACT_PVT_PG_MIX_OFF, IO_PIN_OFF);
                    }
                    else
                    {
                        // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
                        write_to_io(ACT_PVT_PG_MIX_ON, IO_PIN_ON);
                    }

                    printf("g_igroup_fds[pvt_call.group_no]=%d\n", g_igroup_fds[0]);
                    pvt_call.pvt_call_busy = CLEAR;
                    if (lcd_fcs_default_icom == 0)
                    {
                        sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    else
                    {
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, lcd_fcs_default_icom);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }

                    break;

                case P2P_RESET_EVENT:
                    p2p_call_process_timer_flag = CLEAR;
                    printf("inside  P2P_STATE event is P2P_RESET_EVENT \n");
                    g_uiVoipapp_wait_timer = CLEAR;
                    g_uiPvt_call_timer_ui_lcd = CLEAR;
                    one_second_timeout = ONE_SEC_TIMEOUT;

                    if (pg_call.pg_call_busy != SET)
                    {
                        // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                        write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
                    }
                    if (pg_call.pg_call_busy == CLEAR)
                    {
                        printf("....................MUSIC_VOl_BYPASS is OFF when no page call........\n");
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);///change
                        write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);
                    }
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = CLEAR;
                    pthread_mutex_unlock(&lock_call_type);

                    // networking_rtpChannelInfoDelete(PVT_CHANNEL);
                    p2p_dsp_stop_connection(PVT_CHANNEL);
                    select_codec(PVT_CHANNEL, NONE);

                    // send_to_phone_input_cb(1,PHONE_INPUT_HOOK_ON);
                    if (pg_call.pg_call_busy != SET && stn_config.default_AIM == CLEAR)
                    {
                        printf("MUSIC_VOl_BYPASS is ON after P2p reset........\n");
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                        write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);
                    }
                    if (g_iIcom_mem_dropped == SET)
                    {
                        add_membership(g_Iicom_fds[stn_config.default_intercom - MIN_OFFSET]);
                        reset_intercom();
                        start_intercom();
                    }
                    g_uiPvt_call_timer = CLEAR;
                    pvt_call.pvt_call_busy = CLEAR;
                    g_uiFsu_state = IDLE_STATE;

                    printf("p2p reset IN P2P state event changing state to IDLE\n");
                    send_req_msg_to_server(0, P2P_CALL_RESET_ACK, P2P_CALL_FRAME_LENGTH, '0', '0');
                    // send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,LCD_CLR_SECOND_LINE,CLEAR);
                    // send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,CLEAR);
                    send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, callterminated_msg, LCD_CLR_SECOND_LINE, 0);
                    print_msg_on_lcd(IOCTL_PRINT, callterminated_msg, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    // usleep(500000);

                    acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_STUTTER_TONE, START_TONE, CLEAR);

                    g_uiPlay_tone_timer = THREE * APP_TIMER_MULTIPLIER;

                    if (g_iHandset_lifted_signal == CLEAR)
                    {
                        printf("//////////ICOM_MIX_ACt is off after icom membership add.......\n");
                        // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_OFF);
                        write_to_io(ACT_PVT_PG_MIX_OFF, IO_PIN_OFF);
                    }
                    else
                    {
                        // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
                        write_to_io(ACT_PVT_PG_MIX_ON, IO_PIN_ON);
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, lcd_fcs_default_icom);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }

                    printf("g_igroup_fds[pvt_call.group_no]=%d\n", g_igroup_fds[0]);

                    if (lcd_fcs_default_icom == 0)
                    {
                        sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    else
                    {
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, lcd_fcs_default_icom);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    break;

                case NO_P2P_RESPONSE_EVENT:
                    p2p_call_process_timer_flag = CLEAR;
                    printf("########## NO_P2P_RESPONSE_EVENT in P2P_STATE ##########\n");
                    g_uiVoipapp_wait_timer = CLEAR;
                    g_uiPvt_call_timer_ui_lcd = CLEAR;
                    one_second_timeout = ONE_SEC_TIMEOUT;

                    if (pg_call.pg_call_busy != SET)
                    {
                        // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                        write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
                    }
                    if (pg_call.pg_call_busy == CLEAR)
                    {
                        printf("....................MUSIC_VOl_BYPASS is OFF when no page call........\n");
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                        write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
                    }
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = CLEAR;
                    pthread_mutex_unlock(&lock_call_type);

                    // networking_rtpChannelInfoDelete(PVT_CHANNEL);
                    p2p_dsp_stop_connection(PVT_CHANNEL);
                    select_codec(PVT_CHANNEL, NONE);

                    // send_to_phone_input_cb(1,PHONE_INPUT_HOOK_ON);
                    if (pg_call.pg_call_busy != SET && stn_config.default_AIM == CLEAR)
                    {
                        printf("MUSIC_VOl_BYPASS is ON after P2p reset........\n");
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                        write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);
                    }

                    g_uiPvt_call_timer = CLEAR;
                    pvt_call.pvt_call_busy = CLEAR;
                    g_uiFsu_state = IDLE_STATE;
                    printf("p2p reset IN P2P state event changing state to IDLE\n");
                    // send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,CLEAR);
                    // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine2_buf, LCD_CLR_SECOND_LINE, CLEAR);
                    print_msg_on_lcd(IOCTL_PRINT, g_cLine2_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);

                    acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_STUTTER_TONE, START_TONE, CLEAR);

                    g_uiPlay_tone_timer = THREE * APP_TIMER_MULTIPLIER;

                    if (g_iHandset_lifted_signal == CLEAR)
                    {
                        printf("//////////ICOM_MIX_ACt is off after icom membership add.......\n");
                        // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_OFF);
                        write_to_io(ACT_PVT_PG_MIX_OFF, IO_PIN_OFF);
                    }
                    else
                    {
                        // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
                        write_to_io(ACT_PVT_PG_MIX_ON, IO_PIN_ON);
                    }

                    printf("g_igroup_fds[pvt_call.group_no]=%d\n", g_igroup_fds[0]);

                    if (g_iIcom_mem_dropped == SET && stn_config.icom_live_dead_status[stn_config.default_intercom - MIN_OFFSET] == LIVE)
                    {
                        add_membership(g_Iicom_fds[stn_config.default_intercom - MIN_OFFSET]);
                        reset_intercom();
                        start_intercom();
                    }
                    if (lcd_fcs_default_icom == 0)
                    {
                        sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    else
                    {
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, lcd_fcs_default_icom);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    break;

                case APP_TIMEOUT_EVENT:
                    printf("APP timeout event received in P2P state\n");
                    g_uiVoipapp_wait_timer = CLEAR;
                    g_uiPvt_call_timer_ui_lcd = CLEAR;
                    one_second_timeout = ONE_SEC_TIMEOUT;
                    bzero(g_cCalled_station, sizeof(g_cCalled_station));
                    send_msg_ui(P2P_STATE, LCD, LINE2, CONTINUE_DISPLAY, CONFERENCE_msg, LCD_CLR_FIRST_10_POSITION, 0);
                    if (lcd_fcs_default_icom == 0)
                    {
                        sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    else
                    {
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, lcd_fcs_default_icom);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    break;

                case NEG_RES_EVENT:
                    printf("APP timeout event received in P2P state\n");
                    g_uiVoipapp_wait_timer = CLEAR;
                    g_uiPvt_call_timer_ui_lcd = CLEAR;
                    one_second_timeout = ONE_SEC_TIMEOUT;
                    bzero(g_cCalled_station, sizeof(g_cCalled_station));
                    // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, negative_response_msg, LCD_CLR_SECOND_LINE, 0);
                    print_msg_on_lcd(IOCTL_PRINT, negative_response_msg, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    if (g_uiPlay_tone_timer == CLEAR)
                    {
                        printf("................PLAYING BUSY TONE ..............[neg res validkey].\n");
                        acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_BUSY_TONE, START_TONE, 1);
                        g_uiPlay_tone_timer = SIX * APP_TIMER_MULTIPLIER;
                    }
                    // send_msg_ui(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,lcd_fcs_desc,LCD_CLR_FIRST_LINE);
                    if (lcd_fcs_default_icom == 0)
                    {
                        sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    else
                    {
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, stn_config.default_intercom);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    break;
                }
                g_uiFsu_event = CLEAR;
                break; */

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


                           /*if(g_ui_sip_p2p_timer == 0)
                           {

                            send_to_phone_input_cb(1,PHONE_INPUT_HOOK_ON);
                            g_uiFsu_state = P2P_RESET_RINGBACK_STATE;

                           }*/
                             printf("Reset key is pressed in P2P_STATE....\n");


                        g_uiPvt_call_timer_ui_lcd = CLEAR;

                        // send_msg_ui(P2P_RESET_RINGBACK_STATE, LCD, LINE2, CONTINUE_DISPLAY, reset_msg, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, reset_msg, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                        // sleep(1);
                        print_msg_on_lcd(IOCTL_PRINT, page_reset_msg, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                        // send_msg_ui(P2P_RESET_RINGBACK_STATE,LCD,LINE2,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
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

                    case P2P_RESET_PVTTIMEOUT_EVENT:
                    p2p_call_process_timer_flag = CLEAR;
                    printf("inside  P2P_STATE event is P2P_RESET_PVTTIMEOUT_EVENT \n");
                    g_uiVoipapp_wait_timer = CLEAR;

                    if (pg_call.pg_call_busy != SET)
                    {
                        // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                        write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
                    }
                    if (pg_call.pg_call_busy == CLEAR)
                    {
                        printf("....................MUSIC_VOl_BYPASS is OFF when no page call........\n");
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);///change
                        write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
                    }
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = CLEAR;
                    pthread_mutex_unlock(&lock_call_type);

                    // networking_rtpChannelInfoDelete(PVT_CHANNEL);
                    //p2p_dsp_stop_connection(PVT_CHANNEL);
                    select_codec(PVT_CHANNEL, NONE);

                    send_to_phone_input_cb(1,PHONE_INPUT_HOOK_ON);
                    if (pg_call.pg_call_busy != SET && stn_config.default_AIM == CLEAR)
                    {
                        printf("MUSIC_VOl_BYPASS is ON after P2p reset........\n");
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                        write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
                    }
                    if (g_iIcom_mem_dropped == SET)
                    {
                        add_membership(g_Iicom_fds[stn_config.default_intercom - MIN_OFFSET]);
                        reset_intercom();
                        start_intercom();
                    }
                    g_uiPvt_call_timer = CLEAR;

                    g_uiPvt_call_timer_ui_lcd = CLEAR;
                    one_second_timeout = ONE_SEC_TIMEOUT;
                    g_uiFsu_state = IDLE_STATE;
                    printf("p2p reset IN P2P state event changing state to IDLE\n");
                    /// send_req_msg_to_server(0,P2P_CALL_RESET_ACK,P2P_CALL_FRAME_LENGTH,'0','0');
                    // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine2_buf, LCD_CLR_SECOND_LINE, CLEAR);
                    //  print_msg_on_lcd(IOCTL_PRINT, g_cLine2_buf, LINE2, ZERO,LCD_CLR_SECOND_LINE);

                    // send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,CLEAR);

                    acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_STUTTER_TONE, START_TONE, CLEAR);

                    g_uiPlay_tone_timer = THREE * APP_TIMER_MULTIPLIER;

                    if (g_iHandset_lifted_signal == CLEAR)
                    {
                        printf("//////////ICOM_MIX_ACt is off after icom membership add.......\n");
                        // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_OFF);
                       // write_to_io(ACT_PVT_PG_MIX_OFF, IO_PIN_OFF);
                         write_to_io(ACT_PVT_PG_MIX_ON, IO_PIN_ON);
                    }
                    else
                    {
                        // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
                      //  write_to_io(ACT_PVT_PG_MIX_ON, IO_PIN_ON);
                          write_to_io(ACT_PVT_PG_MIX_OFF, IO_PIN_OFF);
                    }

                    printf("g_igroup_fds[pvt_call.group_no]=%d\n", g_igroup_fds[0]);
                    pvt_call.pvt_call_busy = CLEAR;
                    if (lcd_fcs_default_icom == 0)
                    {
                        sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    else
                    {
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, lcd_fcs_default_icom);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }

                    break;



                    /*case PVT_CALL_TIMEOUT:
                        printf("PVT_CALL_TIMEOUT in P2P state \n");
                        g_uiVoipapp_wait_timer = CLEAR;
                      //  operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                       // send_msg_ui_io(P2P_STATE,IO,CONTINUE_DISPLAY,LED_CALL_THROUGH,CLEAR,&io_led_clear[ZERO],
                      //  IO_ADDRESS,LED_ON_TIME,LED_OFF_TIME,IOCTL_SET_BIT_IO_EXPANDER,FLASH_COUNTER);
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

                        // send_msg_ui_lcd(INIT_STATE,LCD,LINE2,CONTINUE_DISPLAY,callterminated_msg,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                         //usleep(100000);
                        if(lcd_fcs_default_icom == 0)
                        {
                            sprintf(g_cLine3_buf,IDLE_msg,stn_config.logical_id);
                            //send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                        }
                        else
                        {
                            sprintf(g_cLine3_buf,Channel_msg,stn_config.logical_id,lcd_fcs_default_icom);
                           // send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,LCD_ADDRESS,IOCTL_PRINT);
                        }
                    break;*/



                     case P2P_RESET_EVENT:
                   // p2p_call_process_timer_flag = CLEAR;
                    printf("inside  P2P_STATE event is P2P_RESET_EVENT \n");
                    g_uiVoipapp_wait_timer = CLEAR;
                    g_uiPvt_call_timer_ui_lcd = CLEAR;
                    //one_second_timeout = ONE_SEC_TIMEOUT;



                    if (pg_call.pg_call_busy != SET)
                    {
                        // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                        write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
                    }

                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = CLEAR;
                    pthread_mutex_unlock(&lock_call_type);

                    // networking_rtpChannelInfoDelete(PVT_CHANNEL);
                    //p2p_dsp_stop_connection(PVT_CHANNEL);
                    select_codec(PVT_CHANNEL, NONE);

                    // send_to_phone_input_cb(1,PHONE_INPUT_HOOK_ON);
                    if (pg_call.pg_call_busy != SET && stn_config.default_AIM == CLEAR)
                    {
                        printf("MUSIC_VOl_BYPASS is ON after P2p reset........\n");
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                        write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
                    }
                    if (g_iIcom_mem_dropped == SET)
                    {
                        add_membership(g_Iicom_fds[stn_config.default_intercom - MIN_OFFSET]);
                        reset_intercom();
                        start_intercom();
                    }
                    g_uiPvt_call_timer = CLEAR;
                    pvt_call.pvt_call_busy = CLEAR;
                    g_uiFsu_state = IDLE_STATE;

                    //printf("p2p reset IN P2P state event changing state to IDLE\n");
                  //  send_req_msg_to_server(0, P2P_CALL_RESET_ACK, P2P_CALL_FRAME_LENGTH, '0', '0');
                    // send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,LCD_CLR_SECOND_LINE,CLEAR);
                    // send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,CLEAR);
                  //  send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, callterminated_msg, LCD_CLR_SECOND_LINE, 0);
                    print_msg_on_lcd(IOCTL_PRINT, callterminated_msg, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    // usleep(500000);

                    acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_STUTTER_TONE, START_TONE, CLEAR);

                    g_uiPlay_tone_timer = THREE * APP_TIMER_MULTIPLIER;

                    if (g_iHandset_lifted_signal == CLEAR)
                    {
                        printf("//////////ICOM_MIX_ACt is off after icom membership add.......\n");
                        // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_OFF);
                       // write_to_io(ACT_PVT_PG_MIX_OFF, IO_PIN_OFF);

                         write_to_io(ACT_PVT_PG_MIX_ON, IO_PIN_ON);
                    }
                    else
                    {
                        // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
                       // write_to_io(ACT_PVT_PG_MIX_ON, IO_PIN_ON);

                        write_to_io(ACT_PVT_PG_MIX_OFF, IO_PIN_OFF);

                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, lcd_fcs_default_icom);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }

                    write_to_io(PVT_AMP_MUTE_ON, IO_PIN_ON);

                    write_to_io(PFC_ACT_OFF, IO_PIN_OFF);

                    printf("g_igroup_fds[pvt_call.group_no]=%d\n", g_igroup_fds[0]);

                    if (lcd_fcs_default_icom == 0)
                    {
                        sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    else
                    {
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, lcd_fcs_default_icom);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    break;


                    case NO_P2P_RESPONSE_EVENT:
                    //p2p_call_process_timer_flag = CLEAR;
                    printf("########## NO_P2P_RESPONSE_EVENT in P2P_STATE ##########\n");
                    g_uiVoipapp_wait_timer = CLEAR;
                    g_uiPvt_call_timer_ui_lcd = CLEAR;
                    //one_second_timeout = ONE_SEC_TIMEOUT;

                    if (pg_call.pg_call_busy != SET)
                    {
                        // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                        write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
                    }
                    if (pg_call.pg_call_busy == CLEAR)
                    {
                        printf("....................MUSIC_VOl_BYPASS is OFF when no page call........\n");
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                        write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);
                    }
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = CLEAR;
                    pthread_mutex_unlock(&lock_call_type);

                    // networking_rtpChannelInfoDelete(PVT_CHANNEL);

                    select_codec(PVT_CHANNEL, NONE);

                     send_to_phone_input_cb(1,PHONE_INPUT_HOOK_ON);
                    if (pg_call.pg_call_busy != SET && stn_config.default_AIM == CLEAR)
                    {
                        printf("MUSIC_VOl_BYPASS is ON after P2p reset........\n");
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                        write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
                    }

                    g_uiPvt_call_timer = CLEAR;
                    pvt_call.pvt_call_busy = CLEAR;
                    g_uiFsu_state = IDLE_STATE;
                    printf("p2p reset IN P2P state event changing state to IDLE\n");
                    // send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,CLEAR);
                    // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine2_buf, LCD_CLR_SECOND_LINE, CLEAR);
                    print_msg_on_lcd(IOCTL_PRINT, g_cLine2_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);

                    acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_STUTTER_TONE, START_TONE, CLEAR);

                    g_uiPlay_tone_timer = THREE * APP_TIMER_MULTIPLIER;

                    if (g_iHandset_lifted_signal == CLEAR)
                    {
                        printf("//////////ICOM_MIX_ACt is off after icom membership add.......\n");
                        // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_OFF);
                        write_to_io(ACT_PVT_PG_MIX_OFF, IO_PIN_OFF);
                    }
                    else
                    {
                        // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
                        write_to_io(ACT_PVT_PG_MIX_ON, IO_PIN_ON);
                    }

                    printf("g_igroup_fds[pvt_call.group_no]=%d\n", g_igroup_fds[0]);

                    if (g_iIcom_mem_dropped == SET && stn_config.icom_live_dead_status[stn_config.default_intercom - MIN_OFFSET] == LIVE)
                    {
                        add_membership(g_Iicom_fds[stn_config.default_intercom - MIN_OFFSET]);
                        reset_intercom();
                        start_intercom();
                    }
                    if (lcd_fcs_default_icom == 0)
                    {
                        sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    else
                    {
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, lcd_fcs_default_icom);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    break;

                    case APP_TIMEOUT_EVENT:
                    printf("APP timeout event received in P2P state\n");
                    g_uiVoipapp_wait_timer = CLEAR;
                    g_uiPvt_call_timer_ui_lcd = CLEAR;
                    //one_second_timeout = ONE_SEC_TIMEOUT;
                    bzero(g_cCalled_station, sizeof(g_cCalled_station));
                    //send_msg_ui(P2P_STATE, LCD, LINE2, CONTINUE_DISPLAY, CONFERENCE_msg, LCD_CLR_FIRST_10_POSITION, 0);
                    if (lcd_fcs_default_icom == 0)
                    {
                        sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    else
                    {
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, lcd_fcs_default_icom);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    break;

                    case NEG_RES_EVENT:
                          printf("APP timeout event received in P2P state\n");
                    g_uiVoipapp_wait_timer = CLEAR;
                    g_uiPvt_call_timer_ui_lcd = CLEAR;
                   // one_second_timeout = ONE_SEC_TIMEOUT;
                    bzero(g_cCalled_station, sizeof(g_cCalled_station));
                    // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, negative_response_msg, LCD_CLR_SECOND_LINE, 0);
                    print_msg_on_lcd(IOCTL_PRINT, negative_response_msg, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    if (g_uiPlay_tone_timer == CLEAR)
                    {
                        printf("................PLAYING BUSY TONE ..............[neg res validkey].\n");
                        acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_BUSY_TONE, START_TONE, 1);
                        g_uiPlay_tone_timer = SIX * APP_TIMER_MULTIPLIER;
                    }
                    // send_msg_ui(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,lcd_fcs_desc,LCD_CLR_FIRST_LINE);
                    if (lcd_fcs_default_icom == 0)
                    {
                        sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    else
                    {
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, stn_config.default_intercom);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    break;
                }
                g_uiFsu_event=CLEAR;
                break;

           /*  case P2P_RESET_RINGBACK_STATE:

                switch (g_uiFsu_event)
                {
                case WAKE_UP_EVENT:
                    printf("____________P2P_RESET_RINGBACK_STATE_______WAKE_UP_EVENT___\n");
                    reset_all();
                    g_uiFsu_state = VCCS_REGISTERING_STATE;
                    break;

                    case BROADCAST_TIMEOUT_EVENT:
                         printf("Inside BROADCAST_TIMEOUT_EVENT\n");
                         Restart_Broadcast_Socket();
                    break;

                case P2P_RESET_EVENT:
                    p2p_call_process_timer_flag = CLEAR;
                    printf("inside  P2P_RESET_RINGBACK_STATE event is P2P_RESET_EVENT \n");
                    g_uiVoipapp_wait_timer = CLEAR;
                    g_uiPvt_call_timer_ui_lcd = CLEAR;
                    one_second_timeout = ONE_SEC_TIMEOUT;
                    printf("p2p reset IN P2P state event changing state to IDLE\n");
                    // networking_rtpChannelInfoDelete(PVT_CHANNEL);
                    p2p_dsp_stop_connection(PVT_CHANNEL);
                    select_codec(PVT_CHANNEL, NONE);
                    if (pg_call.pg_call_busy != SET)
                    {
                        // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                        // write_to_io(TWO , IO_PIN_OFF);
                        write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
                    }
                    if (pg_call.pg_call_busy == CLEAR)
                    {
                        printf("....................MUSIC_VOl_BYPASS is OFF when no page call........\n");
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                        write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
                    }
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = CLEAR;
                    pvt_call.initiator = CLEAR;
                    // Call_cmdid=0;
                    pthread_mutex_unlock(&lock_call_type);
                    // send_to_phone_input_cb(1,PHONE_INPUT_HOOK_ON);
                    if (pg_call.pg_call_busy != SET && stn_config.default_AIM == CLEAR)
                    {
                        printf("MUSIC_VOl_BYPASS is ON after P2p reset........\n");
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                        write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);
                    }
                    if (g_iIcom_mem_dropped == SET)
                    {
                        add_membership(g_Iicom_fds[stn_config.default_intercom - MIN_OFFSET]);
                        reset_intercom();
                        start_intercom();
                    }

                    g_uiPvt_call_timer = CLEAR;
                    pvt_call.pvt_call_busy = CLEAR;
                    g_uiFsu_state = IDLE_STATE;
                    // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, callterminated_msg, LCD_CLR_SECOND_LINE, 0);
                    print_msg_on_lcd(IOCTL_PRINT, callterminated_msg, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    // usleep(500000);
                    //  send_msg_ui(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,lcd_fcs_desc,LCD_CLR_FIRST_LINE);
                    //  send_req_msg_to_server(0,P2P_CALL_RESET_SUCCESS,P2P_CALL_FRAME_LENGTH,'0','0');
                    //  send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,CLEAR);
                    acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_STUTTER_TONE, START_TONE, CLEAR);

                    g_uiPlay_tone_timer = THREE * APP_TIMER_MULTIPLIER;

                    if (g_iHandset_lifted_signal == CLEAR)
                    {
                        printf("//////////ICOM_MIX_ACt is off after icom membership add.......\n");
                        // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_OFF);
                        write_to_io(ACT_PVT_PG_MIX_OFF, IO_PIN_OFF);
                        // write_to_io(ACT_PVT_PG_MIX_OFF , IO_PIN_OFF);
                    }
                    else
                    {
                        // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON)
                        write_to_io(ACT_PVT_PG_MIX_ON, IO_PIN_ON);
                        // write_to_io(ACT_PVT_PG_MIX_ON , IO_PIN_ON);
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, stn_config.default_intercom);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    if (lcd_fcs_default_icom == 0)
                    {
                        sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    else
                    {
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, lcd_fcs_default_icom);
                        //  send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    break;

                case APP_TIMEOUT_EVENT:
                    printf("Inside P2P_RESET_RINGBACK_STATE APP_TIMEOUT_EVENT\n");
                    if (req_retransmission_count == 0)
                    {
                        send_req_msg_to_server(0, last_req_sent_to_server, P2P_CALL_FRAME_LENGTH, '0', '0');
                        printf("*****Retransmission of p2p Reset frame to server******\n");
                        g_uiVoipapp_wait_timer = SERVER_RESPONSE_TIMEOUT;
                        req_retransmission_count = 1;
                        break;
                    }
                    p2p_call_process_timer_flag = CLEAR;
                    printf("inside  P2P_RESET_RINGBACK_STATE event is APP_TIMEOUT_EVENT \n");
                    g_uiVoipapp_wait_timer = CLEAR;
                    // printf("p2p reset IN P2P state event changing state to IDLE\n");
                    // networking_rtpChannelInfoDelete(PVT_CHANNEL);
                    p2p_dsp_stop_connection(PVT_CHANNEL);
                    select_codec(PVT_CHANNEL, NONE);
                    if (pg_call.pg_call_busy != SET)
                    {
                        // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                        write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
                    }
                    if (pg_call.pg_call_busy == CLEAR)
                    {
                        printf("....................MUSIC_VOl_BYPASS is OFF when no page call........\n");
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                        write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
                    }
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = CLEAR;
                    pvt_call.initiator = CLEAR;
                    // Call_cmdid=0;
                    pthread_mutex_unlock(&lock_call_type);
                    // send_to_phone_input_cb(1,PHONE_INPUT_HOOK_ON);
                    if (pg_call.pg_call_busy != SET && stn_config.default_AIM == CLEAR)
                    {
                        printf("MUSIC_VOl_BYPASS is ON after P2p reset........\n");
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                        write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);
                    }
                    if (g_iIcom_mem_dropped == SET)
                    {
                        add_membership(g_Iicom_fds[stn_config.default_intercom - MIN_OFFSET]);
                        reset_intercom();
                        start_intercom();
                    }

                    g_uiPvt_call_timer = CLEAR;
                    pvt_call.pvt_call_busy = CLEAR;
                    g_uiFsu_state = IDLE_STATE;
                    // send_req_msg_to_server(0,P2P_CALL_RESET_SUCCESS,P2P_CALL_FRAME_LENGTH,'0','0');
                    // send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,CLEAR);
                    // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                    print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_STUTTER_TONE, START_TONE, CLEAR);

                    g_uiPlay_tone_timer = THREE * APP_TIMER_MULTIPLIER;

                    if (g_iHandset_lifted_signal == CLEAR)
                    {
                        printf("//////////ICOM_MIX_ACt is off after icom membership add.......\n");
                        // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_OFF);

                        write_to_io(ACT_PVT_PG_MIX_OFF, IO_PIN_OFF);
                    }
                    else
                    {
                        // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
                        write_to_io(ACT_PVT_PG_MIX_ON, IO_PIN_ON);
                    }

                    if (lcd_fcs_default_icom == 0)
                    {
                        sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    else
                    {
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, lcd_fcs_default_icom);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    break;

                default:
                    break;
                }

                g_uiFsu_event = CLEAR;

                break; */

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
                    g_uiPvt_call_timer_ui_lcd = CLEAR;
                    one_second_timeout = ONE_SEC_TIMEOUT;
                    printf("p2p reset IN P2P state event changing state to IDLE\n");
                    // networking_rtpChannelInfoDelete(PVT_CHANNEL);

                    select_codec(PVT_CHANNEL, NONE);

                    write_to_io(PVT_AMP_MUTE_ON, IO_PIN_ON);


                    if (pg_call.pg_call_busy != SET)
                    {
                        // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                        // write_to_io(TWO , IO_PIN_OFF);
                        write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
                    }
                    if (pg_call.pg_call_busy == CLEAR)
                    {
                        printf("....................MUSIC_VOl_BYPASS is OFF when no page call........\n");
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                        write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);
                    }
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = CLEAR;
                    pvt_call.initiator = CLEAR;
                    // Call_cmdid=0;
                    pthread_mutex_unlock(&lock_call_type);
                    // send_to_phone_input_cb(1,PHONE_INPUT_HOOK_ON);
                    if (pg_call.pg_call_busy != SET && stn_config.default_AIM == CLEAR)
                    {
                        printf("MUSIC_VOl_BYPASS is ON after P2p reset........\n");
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                        write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
                    }
                    if (g_iIcom_mem_dropped == SET)
                    {
                        add_membership(g_Iicom_fds[stn_config.default_intercom - MIN_OFFSET]);
                        reset_intercom();
                        start_intercom();
                    }

                    g_uiPvt_call_timer = CLEAR;
                    pvt_call.pvt_call_busy = CLEAR;
                    g_uiFsu_state = IDLE_STATE;


                    // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, callterminated_msg, LCD_CLR_SECOND_LINE, 0);
                    print_msg_on_lcd(IOCTL_PRINT, callterminated_msg, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    // usleep(500000);
                    //  send_msg_ui(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,lcd_fcs_desc,LCD_CLR_FIRST_LINE);
                    //  send_req_msg_to_server(0,P2P_CALL_RESET_SUCCESS,P2P_CALL_FRAME_LENGTH,'0','0');
                    //  send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,CLEAR);
                    acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_STUTTER_TONE, START_TONE, CLEAR);

                    g_uiPlay_tone_timer = THREE * APP_TIMER_MULTIPLIER;

                    if (g_iHandset_lifted_signal == CLEAR)
                    {
                        printf("//////////ICOM_MIX_ACt is off after icom membership add.......\n");
                        // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_OFF);
                          write_to_io(ACT_PVT_PG_MIX_ON, IO_PIN_ON);
                       // write_to_io(ACT_PVT_PG_MIX_OFF, IO_PIN_OFF);
                        // write_to_io(ACT_PVT_PG_MIX_OFF , IO_PIN_OFF);
                    }
                    else
                    {
                        // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON)
                      //  write_to_io(ACT_PVT_PG_MIX_ON, IO_PIN_ON);
                        write_to_io(ACT_PVT_PG_MIX_OFF, IO_PIN_OFF);
                        // write_to_io(ACT_PVT_PG_MIX_ON , IO_PIN_ON);
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, stn_config.default_intercom);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }



                    if (lcd_fcs_default_icom == 0)
                    {
                        sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    else
                    {
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, lcd_fcs_default_icom);
                        //  send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    break;

                        default:
                        break;
                }

                g_uiFsu_event=CLEAR;

                break;

            case CONF_INIT_STATE:
                switch (g_uiFsu_event)
                {
                case WAKE_UP_EVENT:
                    printf("____________CONF_INIT_STATE_______WAKE_UP_EVENT___\n");
                    reset_all();
                    g_uiFsu_state = VCCS_REGISTERING_STATE;
                    break;

                    /*case BROADCAST_TIMEOUT_EVENT:
                           printf("Inside BROADCAST_TIMEOUT_EVENT\n");
                           Restart_Broadcast_Socket();
                    break;*/

                case APP_TIMEOUT_EVENT: // Treat this event as P2P reset so do accordingly
                    printf("APP timeout event received in CONF INIT state\n");
                    g_uiVoipapp_wait_timer = CLEAR;
                    bzero(g_cCalled_station, sizeof(g_cCalled_station));
                    g_uiFsu_state = IDLE_STATE;
                    if (lcd_fcs_default_icom == 0)
                    {
                        sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                        send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                    }
                    else
                    {
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, stn_config.default_intercom);
                        send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                    }
                    break;

                case RINGING_OFF_HOOK_EVENT:
                    //  drop_membership(g_Iicom_fds[pvt_call.curr_icom_no]);
                    // reset_intercom();
                    // send_to_phone_input_cb(1,PHONE_INPUT_HOOK_OFF);
                    g_uiVoipapp_wait_timer = CLEAR;
                    printf("OFF HOOK EVENT RECEIVED \n");
                    break;

                case P2P_CALL_EVENT:
                    printf("P@P CALL EVENT REECEIVED\n");
                    g_uiVoipapp_wait_timer = CLEAR;
                    select_codec(PVT_CHANNEL, HANDSET);
                    set_volume(PVT_CHANNEL, MAX_PVT_VOLUME);
                    g_uiVoipapp_wait_timer = CLEAR;
                    g_uiPvt_call_timer = CLEAR;
                    // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_ON);
                    write_to_io(CALL_THROUGH_LED_ON, IO_PIN_ON);
                    if (g_iHandset_lifted_signal != SET)
                        if (g_iHandset_lifted_signal == CLEAR)
                        {
                            // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_OFF);
                            write_to_io(ACT_PVT_PG_MIX_OFF, IO_PIN_OFF);
                            // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                            write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
                        }
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = CONFERENCE;
                    pthread_mutex_unlock(&lock_call_type);
                    printf("P2p call event received");
                    printf("g_cCaller %s stn id %d \n", g_cCalled_station, stn_config.logical_id);
                    // sprintf(g_cLine3_buf,"P2P:%s   %8d",g_cCaller,stn_config.logical_id);
                    sprintf(g_cLine3_buf, CONF_msg, g_cCalled_station);
                    printf("line3buf=%s\n", g_cLine3_buf);
                    select_codec(PVT_CHANNEL, HANDSET);
                    set_volume(PVT_CHANNEL, MAX_PVT_VOLUME);
                    // send_msg_ui(CONFERENCE_STATE,LCD,LINE3,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR);
                    // send_msg_ui(CONFERENCE_STATE,LED,LED1,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR);
                    // send_msg_ui(CONFERENCE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_FIRST_10_POSITION, 0);

                    print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);

                    send_msg_ui(CONFERENCE_STATE, LED, LED1, CONTINUE_DISPLAY, g_cLine3_buf, CLEAR, 0);
                    g_uiPvt_call_timer = stn_config.pvt_call_timeout * TWO;
                    g_uiPvt_call_timer_ui_lcd = stn_config.pvt_call_timeout * 50;
                    g_uiPage_call_timer_frlcd_ini = 0;
                    acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_STUTTER_TONE, START_TONE, CLEAR);
                    g_uiPlay_tone_timer = THREE * APP_TIMER_MULTIPLIER;
                    pvt_call.pvt_call_busy = SET;
                    g_uiFsu_state = CONFERENCE_STATE;
                    break;
                }
                g_uiFsu_event = CLEAR;
                break;

            case CONFERENCE_STATE:
                switch (g_uiFsu_event)
                {
                case WAKE_UP_EVENT:
                    printf("____________CONFERENCE_STATE_______WAKE_UP_EVENT___\n");
                    reset_all();
                    g_uiFsu_state = VCCS_REGISTERING_STATE;
                    break;
                    /* case BROADCAST_TIMEOUT_EVENT:
                          printf("Inside BROADCAST_TIMEOUT_EVENT\n");
                          Restart_Broadcast_Socket();
                     break;*/

                case P2P_RESET_EVENT:
                    g_uiVoipapp_wait_timer = CLEAR;
                    g_uiPvt_call_timer_ui_lcd = CLEAR;
                    one_second_timeout = ONE_SEC_TIMEOUT;
                    printf(" CONFERENCE STATE RESET RECEIVED \n");
                    // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
                    write_to_io(ACT_PVT_PG_MIX_ON, IO_PIN_ON);
                    if (pg_call.pg_call_busy != SET)
                    {
                        // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                        write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
                    }
                    if (pg_call.pg_call_busy == CLEAR)
                    {
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                        write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
                    }
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = CLEAR;
                    // Call_cmdid=0;
                    pthread_mutex_unlock(&lock_call_type);
                    // send_to_phone_input_cb(1,PHONE_INPUT_HOOK_ON);
                    if (pg_call.pg_call_busy != SET && stn_config.default_AIM == CLEAR)
                    {
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                        write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);
                    }
                    if (g_iIcom_mem_dropped == SET)
                    {
                        add_membership(g_Iicom_fds[stn_config.default_intercom - MIN_OFFSET]);
                        reset_intercom();
                        start_intercom();
                    }

                    g_uiPvt_call_timer = CLEAR;
                    pvt_call.pvt_call_busy = CLEAR;
                    g_uiFsu_state = IDLE_STATE;
                    // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, callterminated_msg, LCD_CLR_SECOND_LINE, 0);
                    print_msg_on_lcd(IOCTL_PRINT, callterminated_msg, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    // sleep(0.5);
                    // send_msg_ui(IDLE_STATE, LCD, LINE1, CONTINUE_DISPLAY, stn_config.fcs_desc, LCD_CLR_FIRST_LINE, 0);
                    print_msg_on_lcd(IOCTL_PRINT, stn_config.fcs_desc, LINE1, ZERO, LCD_CLR_FIRST_LINE);
                    if (lcd_fcs_default_icom == 0)
                    {
                        sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    else
                    {
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, stn_config.default_intercom);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    // send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,CLEAR);
                    acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_STUTTER_TONE, START_TONE, CLEAR);
                    g_uiPlay_tone_timer = THREE * APP_TIMER_MULTIPLIER;
                    break;

                case CONF_TIMER_UPDATE_EVENT:
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = CONFERENCE;
                    pthread_mutex_unlock(&lock_call_type);
                    printf("CONF call event received");
                    printf("g_cCaller %s stn id %d \n", g_cCaller, stn_config.logical_id);
                    sprintf(g_cLine3_buf, "P2P:%s   %8d", g_cCaller, stn_config.logical_id);
                    printf("line3buf=%s\n", g_cLine3_buf);
                    //  select_codec(PVT_CHANNEL,HANDSET);
                    //   set_volume(PVT_CHANNEL,MAX_PVT_VOLUME);
                    // send_msg_ui(CONFERENCE_STATE,LCD,LINE3,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR);
                    // send_msg_ui(CONFERENCE_STATE,LED,LED1,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR);
                    send_msg_ui(CONFERENCE_STATE, LCD, LINE2, CONTINUE_DISPLAY, CONFERENCE_msg, LCD_CLR_FIRST_10_POSITION, 0);
                    send_msg_ui(CONFERENCE_STATE, LED, LED1, CONTINUE_DISPLAY, g_cLine3_buf, CLEAR, 0);
                    g_uiPvt_call_timer = stn_config.pvt_call_timeout * TWO;
                    g_uiPvt_call_timer_ui_lcd = stn_config.pvt_call_timeout * 50;
                    g_uiPage_call_timer_frlcd_ini = 0;

                    // g_uiFsu_state=CONFERENCE_STATE;
                    break;

                case VALID_KEY_SEQ_EVENT:
                    printf("%c valid key in the p2p state \n", g_cReceived_key_seq[0]);
                    switch (g_cReceived_key_seq[0])
                    {
                    case CONFERENCE:
                        printf("CONFERENCE REQ FRAME SENT \n");
                        bzero(g_cCalled_station, sizeof(g_cCalled_station));
                        g_cCalled_station[0] = g_cReceived_key_seq[1];
                        g_cCalled_station[1] = g_cReceived_key_seq[2];
                        g_cCalled_station[2] = g_cReceived_key_seq[3];
                        send_req_msg_to_server(0, CONF_CALL_REQ, CONF_CALL_FRAME_LENGTH, '0', '0');
                        g_uiVoipapp_wait_timer = SERVER_RESPONSE_TIMEOUT;

                        break;

                    case RESET:
                        // send_msg_ui(CONF_RESET_RINGBACK_STATE,LCD,LINE3,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR);
                        // send_to_phone_input_cb(ONE,PHONE_INPUT_HOOK_ON);
                        send_msg_ui(CONF_RESET_RINGBACK_STATE, LCD, LINE2, CONTINUE_DISPLAY, reset_msg, LCD_CLR_SECOND_LINE, 0);
                        // sleep(0.5);
                        g_uiVoipapp_wait_timer = SERVER_RESPONSE_TIMEOUT;
                        g_uiFsu_state = CONF_RESET_RINGBACK_STATE;
                        break;
                    }

                    break;

                case NEG_RES_EVENT:
                    // printf("APP timeout event received in P2P state\n");
                    g_uiVoipapp_wait_timer = CLEAR;
                    bzero(g_cCalled_station, sizeof(g_cCalled_station));
                    if (g_uiPlay_tone_timer == CLEAR)
                    {
                        printf("................PLAYING BUSY TONE [conf]...............\n");
                        acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_BUSY_TONE, START_TONE, 1);
                        g_uiPlay_tone_timer = SIX * APP_TIMER_MULTIPLIER;
                        if (conf_cmd == 50) // 50 is command id send my server when conf is full.
                        {

                            send_msg_ui(CONFERENCE_STATE, LCD, LINE2, CONTINUE_DISPLAY, CONFERENCE_full, LCD_CLR_FIRST_10_POSITION, 0);
                            // sleep(1);
                        }
                        send_msg_ui(CONFERENCE_STATE, LCD, LINE2, CONTINUE_DISPLAY, CONFERENCE_msg, LCD_CLR_FIRST_10_POSITION, 0);
                    }
                    break;
                }
                g_uiFsu_event = CLEAR;
                break;

            case CONF_RESET_RINGBACK_STATE:
                switch (g_uiFsu_event)
                {
                case WAKE_UP_EVENT:
                    printf("____________CONF_RESET_RINGBACK_STATE_______WAKE_UP_EVENT___\n");
                    reset_all();
                    g_uiFsu_state = VCCS_REGISTERING_STATE;
                    break;

                    /*  case BROADCAST_TIMEOUT_EVENT:
                           printf("Inside BROADCAST_TIMEOUT_EVENT\n");
                           Restart_Broadcast_Socket();
                      break;*/

                case P2P_RESET_EVENT:
                case APP_TIMEOUT_EVENT:
                    g_uiPvt_call_timer_ui_lcd = CLEAR;
                    one_second_timeout = ONE_SEC_TIMEOUT;
                    g_uiVoipapp_wait_timer = CLEAR;
                    printf(" CONF_RESET_RINGBACK_STATE RESET RECEIVED \n");
                    // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
                    write_to_io(ACT_PVT_PG_MIX_ON, IO_PIN_ON);
                    if (pg_call.pg_call_busy != SET)
                    {
                        // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                        write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
                    }
                    if (pg_call.pg_call_busy == CLEAR)
                    {
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                        write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
                    }
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = CLEAR;
                    // Call_cmdid=0;
                    pthread_mutex_unlock(&lock_call_type);
                    // send_to_phone_input_cb(1,PHONE_INPUT_HOOK_ON);
                    if (pg_call.pg_call_busy != SET && stn_config.default_AIM == CLEAR)
                    {
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                        write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);
                    }
                    if (g_iIcom_mem_dropped == SET)
                    {
                        add_membership(g_Iicom_fds[stn_config.default_intercom - MIN_OFFSET]);
                        reset_intercom();
                        start_intercom();
                    }

                    g_uiPvt_call_timer = CLEAR;
                    pvt_call.pvt_call_busy = CLEAR;
                    g_uiFsu_state = IDLE_STATE;
                    // send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,CLEAR);
                    send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, callterminated_msg, LCD_CLR_SECOND_LINE, 0);
                    // sleep(0.5);
                    send_msg_ui(IDLE_STATE, LCD, LINE1, CONTINUE_DISPLAY, stn_config.fcs_desc, LCD_CLR_FIRST_LINE, 0);
                    if (lcd_fcs_default_icom == 0)
                    {
                        sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                        send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                    }
                    else
                    {
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, stn_config.default_intercom);
                        send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                    }
                    acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_STUTTER_TONE, START_TONE, CLEAR);
                    g_uiPlay_tone_timer = THREE * APP_TIMER_MULTIPLIER;
                    break;

                default:
                    break;
                }

                break;

            case GROUP_STATE:
                printf("INSIDE GROUP_STATE\n");
                switch (g_uiFsu_event)
                {

                case WAKE_UP_EVENT:
                    printf("____________GROUP_STATE______WAKE_UP_EVENT___\n");
                    reset_all();
                    g_uiVoipapp_wait_timer = CLEAR;
                    g_uiFsu_state = VCCS_REGISTERING_STATE;
                    break;

                    /* case BROADCAST_TIMEOUT_EVENT:
                          printf("Inside BROADCAST_TIMEOUT_EVENT\n");
                          Restart_Broadcast_Socket();
                     break;*/

                    /*  case GRP_PTT_REQUEST_EVENT:

                              printf("grp active contributtor \n");
                              g_itransmit_fd_index=GROUP_OFFSET+pvt_call.group_no;
                              sprintf(g_cMulticast_tx_address,"%s%d",gc_group_mul_start_addr,
                              g_igroup_mul_addr_start_offset+pvt_call.group_no);
                              add_route_to_system(g_cMulticast_tx_address);
                              g_uiGroup_active_contr_flag=SET;
                              pWork_m.private_state=PRIVATE_OUTGOING_ONGOING;
                              g_uiSpeech_detection_timer = SPEECH_DETECTION_TIMEOUT;

                      break;*/

                    /*case GRP_PTT_RELEASE_EVENT:
                            sprintf(g_cMulticast_tx_address,"%s%d",gc_group_mul_start_addr,
                            g_igroup_mul_addr_start_offset+pvt_call.group_no);
                            del_route_from_system(g_cMulticast_tx_address);
                            g_uiGroup_active_contr_flag=CLEAR;
                            // //operate_gpio(LED_CALL_THROUGH,1,GPIO_OFF);
                            pWork_m.private_state=PRIVATE_IDLE;
                            g_itransmit_fd_index=-1;

                    break;*/

                case GROUP_RESET_EVENT:
                    printf("GROUP_RESET_EVENT of GROUP_STATE\n");
                case GRP_INDIVIDUAL_RESET_EVENT:
                    printf("inside  GROUP_STATE event is GROUP_RESET_EVENT \n");
                    printf("GROUP RESET %d \n", g_uiFsu_event);
                    // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
                    write_to_io(ACT_PVT_PG_MIX_ON, IO_PIN_ON);
                    if (pg_call.pg_call_busy != SET)
                        write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
                    g_itransmit_fd_index = -1;
                    // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, page_reset_msg, LCD_CLR_SECOND_LINE, CLEAR);
                    print_msg_on_lcd(IOCTL_PRINT, page_reset_msg, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    pWork_m.private_state = PRIVATE_IDLE;
                    pvt_call.pvt_call_busy = CLEAR;
                    g_uiPvt_call_timer_ui_lcd = CLEAR;
                    one_second_timeout = ONE_SEC_TIMEOUT;
                    g_uiPvt_call_timer = CLEAR;
                    // Call_cmdid=0;
                    g_uiGroup_active_contr_flag = CLEAR;
                    printf("******pg_call.pg_call_busy %d\n", pg_call.pg_call_busy);
                    if (pg_call.pg_call_busy != SET && stn_config.default_AIM == CLEAR)
                    {
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                        write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);
                    }
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = CLEAR;
                    pthread_mutex_unlock(&lock_call_type);
                    if (stn_config.default_AIM)
                    {
                        add_membership(g_iAim_fds[stn_config.default_AIM - ONE]);
                        start_aim_receive_call();
                    }
                    if (stn_config.icom_live_dead_status[stn_config.default_intercom - MIN_OFFSET] == LIVE)
                    {
                        add_membership(g_Iicom_fds[stn_config.default_intercom - MIN_OFFSET]);
                    }
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = INTERCOM_RX;
                    pthread_mutex_unlock(&lock_call_type);
                    drop_membership(g_igroup_fds[pvt_call.group_no]);
                    del_route_from_system(g_cMulticast_tx_address);
                    acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_STUTTER_TONE, START_TONE, CLEAR);
                    g_uiPlay_tone_timer = THREE * APP_TIMER_MULTIPLIER;
                    /* if(pg_call.pg_call_busy==CLEAR)
                     {
                         //operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                     }*/
                    g_uiFsu_state = IDLE_STATE;
                    // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, callterminated_msg, LCD_CLR_SECOND_LINE, 0);
                    print_msg_on_lcd(IOCTL_PRINT, callterminated_msg, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    // usleep(500000);
                    //  send_msg_ui(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,lcd_fcs_desc,LCD_CLR_FIRST_LINE);
                    if (lcd_fcs_default_icom == 0)
                    {
                        sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    else
                    {
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, lcd_fcs_default_icom);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    break;

                case VALID_KEY_SEQ_EVENT:
                    sprintf(uc_buf, "%c%c", g_cReceived_key_seq[1], g_cReceived_key_seq[2]);
                    ui_pressed_key = atoi(uc_buf);
                    printf("pressed key=%d\n", ui_pressed_key);

                    switch (ui_pressed_key)
                    {
                      case PTT_KEY:
                        printf("INSIDE of PTT of VALID_KEY_SEQ of GROUP_STATE\n");
                        if (group_contributor_status_info[pvt_call.group_no].cont1 == CLEAR ||
                            group_contributor_status_info[pvt_call.group_no].cont2 == CLEAR)
                        {
                            send_req_msg_to_server(0, GROUP_PTT_REQ, GROUP_CALL_FRAME_LENGTH, '0', '0');
                            /**  ADDED BY YOGITA  **/
                            g_uiFsu_state = GROUP_CONTRIBUTOR_STATE;
                            g_uiVoipapp_wait_timer = SERVER_RESPONSE_TIMEOUT;
                            // send_msg_ui(GROUP_CONTRIBUTOR_STATE, LCD, LINE3, CONTINUE_DISPLAY, reset_msg, CLEAR, 0);
                            print_msg_on_lcd(IOCTL_PRINT, reset_msg, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                            // send_msg_ui(GROUP_CONTRIBUTOR_STATE,LCD,LINE2,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                        }
                        else if (group_contributor_status_info[pvt_call.group_no].cont1 == stn_config.logical_id ||
                                 group_contributor_status_info[pvt_call.group_no].cont2 == stn_config.logical_id)
                        {
                            break;
                        }
                        else if (g_uiPlay_tone_timer == CLEAR)
                        {
                            acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_BUSY_TONE, START_TONE, CLEAR);
                            g_uiPlay_tone_timer = SIX * APP_TIMER_MULTIPLIER;
                        }
                        /*   COMMENTED BY YOGITA
                        g_uiFsu_state = GROUP_CONTRIBUTOR_STATE;
                        g_uiVoipapp_wait_timer = SERVER_RESPONSE_TIMEOUT;
                        send_msg_ui(GROUP_CONTRIBUTOR_STATE,LCD,LINE2,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                        */
                        break;

                    case RESET_KEY:
                        send_req_msg_to_server(0, GROUP_CALL_RESET_REQ, GROUP_CALL_FRAME_LENGTH, '1', '0');
                        g_uiVoipapp_wait_timer = SERVER_RESPONSE_TIMEOUT;
                        g_uiFsu_state = GROUP_RESET_RINGBACK_STATE;
                        send_msg_ui(GROUP_RESET_RINGBACK_STATE, LCD, LINE2, CONTINUE_DISPLAY, reset_msg, LCD_CLR_SECOND_LINE, 0);

                        if (lcd_fcs_default_icom == 0)
                        {
                            sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                            send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        }
                        else
                        {
                            sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, lcd_fcs_default_icom);
                            send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        }
                        // send_msg_ui(GROUP_RESET_RINGBACK_STATE,LCD,LINE2,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                        break;
                    }

                    break;
                }
                g_uiFsu_event = CLEAR;

                break;

            case GROUP_CONTRIBUTOR_STATE:
                printf("INSIDE GROUP_CONTRIBUTOR_STATE\n");
                // g_uiVoipapp_wait_timer = CLEAR;

                switch (g_uiFsu_event)
                {
                case WAKE_UP_EVENT:
                    printf("____________GROUP_CONTRIBUTOR_STATE______WAKE_UP_EVENT___\n");
                    reset_all();
                    g_uiVoipapp_wait_timer = CLEAR;
                    g_uiFsu_state = VCCS_REGISTERING_STATE;
                    break;
                    /*case BROADCAST_TIMEOUT_EVENT:
                         printf("Inside BROADCAST_TIMEOUT_EVENT\n");
                         Restart_Broadcast_Socket();
                    break;*/

                case PTT_RELEASE_EVENT:
                    send_req_msg_to_server(0, GROUP_PTT_RELEASE, GROUP_CALL_FRAME_LENGTH, '1', '0');
                    g_uiVoipapp_wait_timer = SERVER_RESPONSE_TIMEOUT;
                    break;
                case GRP_PTT_REQUEST_EVENT:
                    printf("****INSIDE GRP_PTT_REQUEST_EVENT\n");
                    printf("grp active contributtor \n");
                    g_uiVoipapp_wait_timer = CLEAR;
                    g_itransmit_fd_index = GROUP_OFFSET + pvt_call.group_no;
                    sprintf(g_cMulticast_tx_address, "%s%d", gc_group_mul_start_addr, g_igroup_mul_addr_start_offset + pvt_call.group_no);
                    add_route_to_system(g_cMulticast_tx_address);
                    g_uiGroup_active_contr_flag = SET;

                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = GROUP_TX_RX;
                    // pvt_call.type=GROUP;
                    pthread_mutex_unlock(&lock_call_type);

                    pWork_m.private_state = PRIVATE_OUTGOING_ONGOING;
                    g_uiSpeech_detection_timer = SPEECH_DETECTION_TIMEOUT;

                    break;

                case GRP_PTT_RELEASE_EVENT:
                    printf("*****INSIDE GRP_PTT_RELEASE_EVENT\n");
                    g_uiVoipapp_wait_timer = CLEAR;
                    sprintf(g_cMulticast_tx_address, "%s%d", gc_group_mul_start_addr,
                            g_igroup_mul_addr_start_offset + pvt_call.group_no);
                    del_route_from_system(g_cMulticast_tx_address);
                    g_uiGroup_active_contr_flag = CLEAR;
                    // //operate_gpio(LED_CALL_THROUGH,1,GPIO_OFF);
                    write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
                    pWork_m.private_state = PRIVATE_IDLE;
                    g_uiFsu_state = GROUP_STATE;
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = GROUP;
                    pthread_mutex_unlock(&lock_call_type);
                    g_itransmit_fd_index = -1;
                    send_msg_ui(GROUP_STATE, LCD, LINE3, CONTINUE_DISPLAY, reset_msg, CLEAR, 0);
                    // send_msg_ui(GROUP_STATE,LCD,LINE2,CONTINUE_DISPLAY,page_reset_msg,CLEAR);

                    break;

                case GROUP_RESET_EVENT:
                    printf("GROUP_RESET_EVENT of GROUP_CONTRIBUTOR_STATE\n");
                case GRP_INDIVIDUAL_RESET_EVENT:
                    printf("inside  GROUP_STATE event is GROUP_RESET_EVENT \n");
                    printf("GROUP RESET %d \n", g_uiFsu_event);
                    // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
                    write_to_io(ACT_PVT_PG_MIX_ON, IO_PIN_ON);
                    if (pg_call.pg_call_busy != SET)
                        // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                        write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
                    g_itransmit_fd_index = -1;
                    send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, page_reset_msg, LCD_CLR_SECOND_LINE, CLEAR);
                    pWork_m.private_state = PRIVATE_IDLE;
                    pvt_call.pvt_call_busy = CLEAR;
                    g_uiPvt_call_timer = CLEAR;
                    g_uiPvt_call_timer_ui_lcd = CLEAR;
                    one_second_timeout = ONE_SEC_TIMEOUT;
                    // Call_cmdid=0;
                    g_uiGroup_active_contr_flag = CLEAR;
                    printf("******pg_call.pg_call_busy %d\n", pg_call.pg_call_busy);
                    if (pg_call.pg_call_busy != SET && stn_config.default_AIM == CLEAR)
                    {

                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                        write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);
                    }
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = CLEAR;
                    pthread_mutex_unlock(&lock_call_type);
                    if (stn_config.default_AIM)
                    {
                        add_membership(g_iAim_fds[stn_config.default_AIM - ONE]);
                        start_aim_receive_call();
                    }
                    if (stn_config.icom_live_dead_status[stn_config.default_intercom - MIN_OFFSET] == LIVE)
                    {
                        add_membership(g_Iicom_fds[stn_config.default_intercom - MIN_OFFSET]);
                    }
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = INTERCOM_RX;
                    pthread_mutex_unlock(&lock_call_type);
                    drop_membership(g_igroup_fds[pvt_call.group_no]);
                    del_route_from_system(g_cMulticast_tx_address);
                    acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_STUTTER_TONE, START_TONE, CLEAR);
                    g_uiPlay_tone_timer = THREE * APP_TIMER_MULTIPLIER;
                    /* if(pg_call.pg_call_busy==CLEAR)
                     {
                         //operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                     }*/
                    // operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);
                    write_to_io(ICOM_LED_OFF, IO_PIN_OFF);
                    g_uiFsu_state = IDLE_STATE;
                    send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, callterminated_msg, LCD_CLR_SECOND_LINE, 0);
                    // sleep(0.5);
                    //  send_msg_ui(IDLE_STATE,LCD,LINE1,CONTINUE_DISPLAY,lcd_fcs_desc,LCD_CLR_FIRST_LINE);
                    if (lcd_fcs_default_icom == 0)
                    {
                        sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                        send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                    }
                    else
                    {
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, lcd_fcs_default_icom);
                        send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                    }
                    break;

                case VALID_KEY_SEQ_EVENT:
                    sprintf(uc_buf, "%c%c", g_cReceived_key_seq[1], g_cReceived_key_seq[2]);
                    ui_pressed_key = atoi(uc_buf);
                    printf("pressed key=%d\n", ui_pressed_key);
                    switch (ui_pressed_key)
                    {
                    case RESET_KEY:
                        send_req_msg_to_server(0, GROUP_CALL_RESET_REQ, GROUP_CALL_FRAME_LENGTH, '1', '0');
                        last_req_sent_to_server = GROUP_CALL_RESET_REQ;
                        req_retransmission_count = 0;
                        g_uiVoipapp_wait_timer = SERVER_RESPONSE_TIMEOUT;
                        g_uiFsu_state = GROUP_RESET_RINGBACK_STATE;
                        send_msg_ui(GROUP_RESET_RINGBACK_STATE, LCD, LINE2, CONTINUE_DISPLAY, reset_msg, LCD_CLR_SECOND_LINE, 0);

                        if (lcd_fcs_default_icom == 0)
                    {
                        sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                        send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                    }
                    else
                    {
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, lcd_fcs_default_icom);
                        send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                    }

                        /*    if(lcd_fcs_default_icom == 0)
                     {
                         sprintf(g_cLine3_buf,IDLE_msg,stn_config.logical_id);
                         send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,LCD_CLR_SECOND_LINE,0);
                     }
                     else
                     {
                         sprintf(g_cLine3_buf,Channel_msg,stn_config.logical_id,lcd_fcs_default_icom);
                         send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,LCD_CLR_SECOND_LINE,0);
                     } */
                        // send_msg_ui(GROUP_RESET_RINGBACK_STATE,LCD,LINE2,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                        break;

                    default:
                        break;
                    }
                    break;
                    //******Added by Yogita******//
                case APP_TIMEOUT_EVENT:
                    if (g_uiGroup_PTT_flag == SET)
                    {
                        printf("****INSIDE APP_TIMEOUT_EVENT for GRP_PTT_REQUEST_EVENT\n");
                        // g_uiGroup_PTT_flag = CLEAR;//Added by Yogita for GRP_PTT_REQUEST_EVENT is not received from server.
                        g_uiFsu_state = GROUP_STATE;
                        send_msg_ui(GROUP_STATE, LCD, LINE2, CONTINUE_DISPLAY, page_reset_msg, LCD_CLR_SECOND_LINE, CLEAR);
                    }
                    else if (g_uiGroup_active_contr_flag == SET)
                    {
                        printf("*****INSIDE APP_TIMEOUT for GRP_PTT_RELEASE_EVENT\n");
                        g_uiVoipapp_wait_timer = CLEAR;
                        sprintf(g_cMulticast_tx_address, "%s%d", gc_group_mul_start_addr,
                                g_igroup_mul_addr_start_offset + pvt_call.group_no);
                        del_route_from_system(g_cMulticast_tx_address);
                        g_uiGroup_active_contr_flag = CLEAR;
                        // //operate_gpio(LED_CALL_THROUGH,1,GPIO_OFF);
                        write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
                        pWork_m.private_state = PRIVATE_IDLE;
                        g_uiFsu_state = GROUP_STATE;
                        pthread_mutex_lock(&lock_call_type);
                        pvt_call.type = GROUP;
                        pthread_mutex_unlock(&lock_call_type);
                        g_itransmit_fd_index = -1;
                        send_msg_ui(GROUP_RESET_RINGBACK_STATE, LCD, LINE2, CONTINUE_DISPLAY, reset_msg, LCD_CLR_SECOND_LINE, 0);
                        // send_msg_ui(GROUP_STATE,LCD,LINE2,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                    }
                    else
                    {
                        printf("APP_TIMEOUT in GROUP_CONTRIBUTER_STATE\n");
                    }

                    /*   if(lcd_fcs_default_icom == 0)
                     {
                         sprintf(g_cLine3_buf,IDLE_msg,stn_config.logical_id);
                         send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,LCD_CLR_SECOND_LINE,0);
                     }
                     else
                     {
                         sprintf(g_cLine3_buf,Channel_msg,stn_config.logical_id,lcd_fcs_default_icom);
                         send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,LCD_CLR_SECOND_LINE,0);
                     }  */
                    break;

                default:
                    break;
                }
                g_uiFsu_event = CLEAR;

                break;

            case GROUP_RESET_RINGBACK_STATE:

                switch (g_uiFsu_event)
                {
                case WAKE_UP_EVENT:
                    printf("____________GROUP_RESET_RINGBACK_STATE______WAKE_UP_EVENT___\n");
                    reset_all();
                    g_uiVoipapp_wait_timer = CLEAR;
                    g_uiFsu_state = VCCS_REGISTERING_STATE;
                    break;

                    /* case BROADCAST_TIMEOUT_EVENT:
                          printf("Inside BROADCAST_TIMEOUT_EVENT\n");
                          Restart_Broadcast_Socket();
                     break;*/

                case GROUP_RESET_EVENT:
                    printf("INSIDE GROUP_RESET_EVENT GROUP_RESET_RINGBACK_STATE\n");
                case GRP_INDIVIDUAL_RESET_EVENT:
                    // case APP_TIMEOUT_EVENT:
                    g_uiVoipapp_wait_timer = CLEAR;
                    printf("inside  GROUP_RESET_RINGBACK_STATE event is GROUP_RESET_EVENT \n");
                    printf("GROUP RESET %d \n", g_uiFsu_event);

                    // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
                    write_to_io(ACT_PVT_PG_MIX_ON, IO_PIN_ON);

                    if (pg_call.pg_call_busy != SET)
                    {
                        printf("****** INSIDE LED_CALL_THROUGH OFF *******\n");
                        write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
                        // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                    }
                    g_itransmit_fd_index = -1;
                    send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, callterminated_msg, LCD_CLR_SECOND_LINE, 0);
                    // sleep(0.5);
                    //  send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                    pWork_m.private_state = PRIVATE_IDLE;
                    pvt_call.pvt_call_busy = CLEAR;
                    g_uiPvt_call_timer = CLEAR;
                    g_uiPvt_call_timer_ui_lcd = CLEAR;
                    one_second_timeout = ONE_SEC_TIMEOUT;
                    // Call_cmdid=0;
                    g_uiGroup_active_contr_flag = CLEAR;

                    printf("********pg_call.pg_call_busy!=SET %d", pg_call.pg_call_busy);
                    if (pg_call.pg_call_busy != SET && stn_config.default_AIM == CLEAR)
                    {
                        printf("****** INSIDE MUSIC_VOL_BYPASS ON *******\n");
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                        write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);
                    }
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = CLEAR;
                    pthread_mutex_unlock(&lock_call_type);
                    if (stn_config.default_AIM)
                    {
                        add_membership(g_iAim_fds[stn_config.default_AIM - ONE]);
                        start_aim_receive_call();
                    }
                    if (stn_config.icom_live_dead_status[stn_config.default_intercom - MIN_OFFSET] == LIVE)
                    {
                        add_membership(g_Iicom_fds[stn_config.default_intercom - MIN_OFFSET]);
                    }
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = INTERCOM_RX;
                    pthread_mutex_unlock(&lock_call_type);
                    drop_membership(g_igroup_fds[pvt_call.group_no]);
                    del_route_from_system(g_cMulticast_tx_address);
                    acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_STUTTER_TONE, START_TONE, CLEAR);
                    g_uiPlay_tone_timer = THREE * APP_TIMER_MULTIPLIER;
                    /* if(pg_call.pg_call_busy==CLEAR)
                     {
                         //operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                     }*/
                    // operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);
                    write_to_io(ICOM_LED_OFF, IO_PIN_OFF);
                    g_uiFsu_state = IDLE_STATE;

                    if (lcd_fcs_default_icom == 0)
                    {
                        sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                        send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                    }
                    else
                    {
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, lcd_fcs_default_icom);
                        send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                    }
                    break;

                case APP_TIMEOUT_EVENT:

                    printf("Inside GROUP_RESET_RINGBACK_STATE APP_TIMEOUT_EVENT\n");
                    if (req_retransmission_count == 0)
                    {
                        send_req_msg_to_server(0, last_req_sent_to_server, P2P_CALL_FRAME_LENGTH, '0', '0');
                        printf("*****Retransmission of group Reset frame to server******\n");
                        g_uiVoipapp_wait_timer = SERVER_RESPONSE_TIMEOUT;
                        req_retransmission_count = 1;
                        break;
                    }

                    g_uiVoipapp_wait_timer = CLEAR;
                    printf("inside  GROUP_RESET_RINGBACK_STATE event is APP_TIMEOUT_EVENT \n");
                    printf("GROUP RESET %d \n", g_uiFsu_event);

                    // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
                    write_to_io(ACT_PVT_PG_MIX_ON, IO_PIN_ON);

                    if (pg_call.pg_call_busy != SET)
                    {
                        printf("****** INSIDE LED_CALL_THROUGH OFF *******\n");
                        write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
                        // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                    }
                    g_itransmit_fd_index = -1;
                    // send_msg_ui(IDLE_STATE, LCD, LINE1, CONTINUE_DISPLAY, global_terminate, LCD_CLR_FIRST_LINE, 0);
                    print_msg_on_lcd(IOCTL_PRINT, global_terminate, LINE1, ZERO, LCD_CLR_FIRST_LINE);

                    // send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                    pWork_m.private_state = PRIVATE_IDLE;
                    pvt_call.pvt_call_busy = CLEAR;
                    g_uiPvt_call_timer = CLEAR;
                    g_uiPvt_call_timer_ui_lcd = CLEAR;
                    one_second_timeout = ONE_SEC_TIMEOUT;
                    // Call_cmdid=0;
                    g_uiGroup_active_contr_flag = CLEAR;

                    printf("********pg_call.pg_call_busy!=SET %d", pg_call.pg_call_busy);
                    if (pg_call.pg_call_busy != SET && stn_config.default_AIM == CLEAR)
                    {
                        printf("****** INSIDE MUSIC_VOL_BYPASS ON *******\n");
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                        write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);
                    }
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = CLEAR;
                    pthread_mutex_unlock(&lock_call_type);
                    if (stn_config.default_AIM)
                    {
                        add_membership(g_iAim_fds[stn_config.default_AIM - ONE]);
                        start_aim_receive_call();
                    }
                    if (stn_config.icom_live_dead_status[stn_config.default_intercom - MIN_OFFSET] == LIVE)
                    {
                        add_membership(g_Iicom_fds[stn_config.default_intercom - MIN_OFFSET]);
                    }
                    pthread_mutex_lock(&lock_call_type);
                    pvt_call.type = INTERCOM_RX;
                    pthread_mutex_unlock(&lock_call_type);
                    drop_membership(g_igroup_fds[pvt_call.group_no]);
                    del_route_from_system(g_cMulticast_tx_address);
                    acgPlayTone_phi(PVT_CHANNEL, VOIP_SIGNAL_STUTTER_TONE, START_TONE, CLEAR);
                    g_uiPlay_tone_timer = THREE * APP_TIMER_MULTIPLIER;
                    /* if(pg_call.pg_call_busy==CLEAR)
                     {
                         //operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                     }*/
                    // operate_gpio(ICOM_STATUS,WRITE,GPIO_OFF);
                    write_to_io(ICOM_LED_OFF, IO_PIN_OFF);
                    g_uiFsu_state = IDLE_STATE;

                    if (lcd_fcs_default_icom == 0)
                    {
                        sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                        send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                    }
                    else
                    {
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, lcd_fcs_default_icom);
                        send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                    }

                    break;

                default:
                    break;
                }

                g_uiFsu_event = CLEAR;
                break;

            case DIAGNOSTICS_STATE:
                switch (g_uiFsu_event)
                {
                case WAKE_UP_EVENT:
                    printf("____________DIAGNOSTICS_STATE______WAKE_UP_EVENT___\n");
                    reset_all();
                    g_uiVoipapp_wait_timer = CLEAR;
                    g_uiFsu_state = VCCS_REGISTERING_STATE;
                    break;

                    /* case BROADCAST_TIMEOUT_EVENT:
                          printf("Inside BROADCAST_TIMEOUT_EVENT\n");
                          Restart_Broadcast_Socket();
                     break;*/

                case DIAGNOSTIC_PVT_OK:
                    printf("DIAGNOSTIC_PVT_OK\n");
                    // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                    write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
                    // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_OFF);
                    write_to_io(ACT_PVT_PG_MIX_OFF, IO_PIN_OFF);
                    stop_alarm(TEN);
                    g_iDiagnostic_pvt_status = SET;
                    select_codec(PVT_CHANNEL, NONE);
                    select_codec(PVT_CHANNEL, SPEAKER);
                    set_volume(PVT_CHANNEL, MAX_PVT_VOLUME);
//                    start_alarm(TEN);
                    g_uiSpk_diagnostic_timer = DIAGNOSTIC_TIMER_PVT_PG;
                    break;

                case DIAGNOSTIC_PVT_NOT_OK:
                    printf("DIAGNOSTIC_PVT_NOT_OK\n");
                    // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                    write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
                    // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_OFF);
                    write_to_io(ACT_PVT_PG_MIX_OFF, IO_PIN_OFF);
                    stop_alarm(TEN);
                    g_iDiagnostic_pvt_status = CLEAR;
                    select_codec(PVT_CHANNEL, NONE);
                    select_codec(PVT_CHANNEL, SPEAKER);
                    set_volume(PVT_CHANNEL, MAX_PVT_VOLUME);
//                    start_alarm(TEN);
                    g_uiSpk_diagnostic_timer = DIAGNOSTIC_TIMER_PVT_PG;
                    break;

                case DIAGNOSTIC_SPK_OK:
                    printf("DIAGNOSTIC_SPK_OK\n");
                    g_uiVoipapp_wait_timer = CLEAR;

                    stop_alarm(TEN);
                    // close_channel(PVT_CHANNEL);
                    g_iDiagnostic_spk_status = SET;
                    g_iDiagnostic_nw_status = SET;
                    g_iDiagnostic_mic_status = SET;
                    if (stn_config.icom_live_dead_status[stn_config.default_intercom - MIN_OFFSET] == LIVE)
                    {
                        add_membership(g_Iicom_fds[stn_config.default_intercom - MIN_OFFSET]);
                    }
                    //  start_intercom();
                    if (stn_config.default_AIM)
                    {
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                        write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
                        add_membership(g_iAim_fds[stn_config.default_AIM - ONE]);
                        //    start_aim_receive_call();
                    }
                    else
                    {
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                        write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);
                    }
                    g_uiDiagnostic_start_flag = CLEAR;
                    FD_CLR(g_uiApp_diag_timer_fd, &app_readset);
                    send_req_msg_to_server(0, DIAGNOSTICS_RESPONSE, DIAGNOSTICS_FRAME_LENGTH, '0', '0');
                    g_uiFsu_state = IDLE_STATE;

                    break;

                case DIAGNOSTIC_SPK_NOT_OK:
                    printf("DIAGNOSTIC_SPK_NOT_OK\n");
                    g_uiVoipapp_wait_timer = CLEAR;
                    stop_alarm(TEN);
                    g_iDiagnostic_spk_status = CLEAR;
                    g_iDiagnostic_nw_status = SET;
                    g_iDiagnostic_mic_status = SET;
                    // close_channel(PVT_CHANNEL);
                    // start_intercom();
                    if (stn_config.default_AIM)
                    {
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                        write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
                        add_membership(g_iAim_fds[stn_config.default_AIM - ONE]);
                        // start_aim_receive_call();
                    }
                    else
                    {
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                        write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);
                    }
                    g_uiDiagnostic_start_flag = CLEAR;
                    FD_CLR(g_uiApp_diag_timer_fd, &app_readset);
                    send_req_msg_to_server(0, DIAGNOSTICS_RESPONSE, DIAGNOSTICS_FRAME_LENGTH, '0', '0');
                    g_uiFsu_state = IDLE_STATE;
                    break;

                case APP_TIMEOUT_EVENT:
                    stop_alarm(TEN);
                    g_iDiagnostic_spk_status = CLEAR;
                    g_iDiagnostic_nw_status = CLEAR;
                    g_iDiagnostic_mic_status = CLEAR;
                    close_channel(PVT_CHANNEL);
                    //  start_intercom();
                    if (stn_config.default_AIM)
                    {
                        start_aim_receive_call();
                    }
                    g_uiDiagnostic_start_flag = CLEAR;
                    FD_CLR(g_uiApp_diag_timer_fd, &app_readset);
                    send_req_msg_to_server(0, DIAGNOSTICS_RESPONSE, DIAGNOSTICS_FRAME_LENGTH, '0', '0');
                    g_uiFsu_state = IDLE_STATE;
                    break;
                }
                g_uiFsu_event = CLEAR;
                break;

            case PAGE_INITIATOR_STATE:
                printf("************INSIDE PAGE_INITIATOR_STATE\ng_uiFsu_event is ...%d\n", g_uiFsu_event);

                switch (g_uiFsu_event)
                {
                case WAKE_UP_EVENT:
                    printf("____________PAGE_INITIATOR_STATE______WAKE_UP_EVENT___\n");
                    reset_all();
                    g_uiFsu_state = VCCS_REGISTERING_STATE;
                    break;

                    /* case BROADCAST_TIMEOUT_EVENT:
                          printf("Inside BROADCAST_TIMEOUT_EVENT\n");
                          Restart_Broadcast_Socket();
                     break;*/

                case VALID_KEY_SEQ_EVENT:
                    sprintf(uc_buf, "%c%c", g_cReceived_key_seq[1], g_cReceived_key_seq[2]);
                    ui_pressed_key = atoi(uc_buf);
                    printf("pressed key in page_initiator_state =%d\n", ui_pressed_key);

                    switch (ui_pressed_key)
                    {
                    case RESET_KEY:

                        if (pvt_call.type == GLOBAL || pg_call.type == GLOBAL_ALARM)
                        {
                            send_req_msg_to_server(0, GLOBAL_RESET, GLOBAL_RESET_FRAME_LENGTH, '0', '0');
                            printf("###########global reset frame sent  to server\n");
                            last_req_sent_to_server = GLOBAL_RESET;
                            req_retransmission_count = 0;
                            getTime();
                        }
                        else if (pvt_call.type = ZONE || pg_call.type == ZONE_ALARM)
                        {
                            send_req_msg_to_server(0, ZONE_RESET_REQ, GLOBAL_RESET_FRAME_LENGTH, '0', '0');
                        }
                        g_uiVoipapp_wait_timer = SERVER_RESPONSE_TIMEOUT;

                        g_uiFsu_state = PAGE_RESET_RINGBACK_STATE;

                        g_uiPage_call_timer_lcd = CLEAR;
                        g_uiPage_call_timer_frlcd_ini = CLEAR;
                        one_second_timeout = ONE_SEC_TIMEOUT;
                        // send_msg_ui(PAGE_RESET_RINGBACK_STATE, LCD, LINE1, CONTINUE_DISPLAY, reset_msg, LCD_CLR_FIRST_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, reset_msg, LINE1, ZERO, LCD_CLR_FIRST_LINE);
                        // sleep(1);
                        if (lcd_fcs_default_icom == 0)
                        {
                            //   printf("inside if******************************************\n");
                            sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                            // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                            print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                        }
                        else
                        {
                            //   printf("inside else******************************************\n");
                            sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, lcd_fcs_default_icom);
                            // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                            print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                        }

                        // send_msg_ui(PAGE_RESET_RINGBACK_STATE,LCD,LINE3,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR);
                        break;

                        case PTT_KEY:
                        printf("IN GLOBAL INITIATOR PTT KEY PRESSED ----*-*-*-*-*-*-*--");
                        select_codec(PVT_CHANNEL, HANDSET);
                        break;

                    default:
                        break;
                    }

                    break;

                case GLOBAL_ALM_EVENT:
                    close_channel(PG_CHANNEL);
                    select_codec(PG_CHANNEL, NONE);
                    g_itransmit_fd_index = -1;
                    pthread_mutex_lock(&lock_call_type);
                    pg_call.type = GLOBAL_ALARM;
                    pthread_mutex_unlock(&lock_call_type);
                    // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_ON);
                    write_to_io(CALL_THROUGH_LED_ON, IO_PIN_ON);
                    stop_aim_receive_call();
                    // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                    write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
                    printf("GLOABAL_ALM_EVENT call event received");
                    sprintf(g_cLine3_buf, global_alm_msg, lcd_global_alm_init);
                    send_msg_ui(IDLE_STATE, LCD, LINE1, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_FIRST_LINE, 0);
                    // send_msg_ui(IDLE_STATE,LCD,LINE3,CONTINUE_DISPLAY,g_cLine3_buf,CLEAR);
                    g_uiPage_call_timer = stn_config.alarm_call_timeout * TWO;
                    pWork_m.page_state = PAGE_INCOMING_ONGOING;
                    pg_call.pg_call_busy = CLEAR;
                    g_uiPage_call_timer = CLEAR;
                    select_codec(PG_CHANNEL, NONE);
                    multicast_prepare_dsp(PG_CHANNEL);
                    select_codec(PG_CHANNEL, SPEAKER);
                    g_uiFsu_state = IDLE_STATE;

                    break;
                    //********Addede by yogita*****//
                    /*case GLOBAL_RESET_EVENT:
                          g_uiVoipapp_wait_timer = CLEAR;
                          //printf("Global reset event received [%s %d]\n",__FILE__,__LINE__);
                          printf("inside  PAGE_INITIATOR_STATE event is GLOBAL_RESET_EVENT \n");
                          pg_call.alm_no=CLEAR;
                          pvt_call.type=CLEAR;
                          pg_call.type=CLEAR;
                          close_channel(PG_CHANNEL);
                          pg_call.pg_call_busy = CLEAR;
                          g_uiPage_call_timer=CLEAR;
                          pvt_call.pvt_call_busy = CLEAR;
                          send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                          pWork_m.private_state=PRIVATE_IDLE;
                          g_itransmit_fd_index=-1;
                          g_uiFsu_state=IDLE_STATE;
                          //operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);

                          pvt_call.curr_icom_no=stn_config.default_intercom-MIN_OFFSET;
                          if(stn_config.icom_live_dead_status[stn_config.default_intercom-MIN_OFFSET]==LIVE)
                          {
                              add_membership(g_Iicom_fds[stn_config.default_intercom-MIN_OFFSET]);
                              start_intercom();
                          }

                          if(stn_config.default_AIM)
                          {
                              //operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                              add_membership(g_iAim_fds[stn_config.default_AIM-ONE]);
                              start_aim_receive_call();
                          }
                          else
                          {
                              //operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                          }
                      break;*/

                case ZONE_RESET_EVENT:
                    // case APP_TIMEOUT_EVENT:
                    g_uiVoipapp_wait_timer = CLEAR;
                    // printf("zone reset event received [%s %d]\n",__FILE__,__LINE__);
                    printf("inside  PAGE_INITIATOR_STATE event is ZONE_RESET_EVENT \n");
                    pg_call.alm_no = CLEAR;
                    pvt_call.type = CLEAR;
                    pg_call.type = CLEAR;
                    // close_channel(PG_CHANNEL);
                    pg_call.pg_call_busy = CLEAR;
                    g_uiPage_call_timer = CLEAR;
                    pvt_call.pvt_call_busy = CLEAR;

                    g_uiPage_call_timer_lcd = CLEAR;
                    g_uiPage_call_timer_frlcd_ini = CLEAR;
                    one_second_timeout = ONE_SEC_TIMEOUT;
                    // send_msg_ui(IDLE_STATE, LCD, LINE1, CONTINUE_DISPLAY, zone_terminate, LCD_CLR_FIRST_LINE, 0);
                    print_msg_on_lcd(IOCTL_PRINT, zone_terminate, LINE1, ZERO, LCD_CLR_FIRST_LINE);
                    ////usleep(500000);
                    // send_msg_ui(IDLE_STATE, LCD, LINE1, CONTINUE_DISPLAY, stn_config.fcs_desc, LCD_CLR_FIRST_LINE, 0);
                    print_msg_on_lcd(IOCTL_PRINT, stn_config.fcs_desc, LINE1, ZERO, LCD_CLR_FIRST_LINE);
                    // send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                    pWork_m.private_state = PRIVATE_IDLE;
                    g_itransmit_fd_index = -1;
                    g_uiFsu_state = IDLE_STATE;
                    // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                    write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
                    drop_membership(g_izone_fds[pg_call.zone_no]); ///
                    pvt_call.curr_icom_no = stn_config.default_intercom - MIN_OFFSET;
                    if (stn_config.icom_live_dead_status[stn_config.default_intercom - MIN_OFFSET] == LIVE)
                    {
                        add_membership(g_Iicom_fds[stn_config.default_intercom - MIN_OFFSET]);
                        start_intercom();
                    }

                    if (stn_config.default_AIM)
                    {
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                        write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
                        add_membership(g_iAim_fds[stn_config.default_AIM - ONE]);
                        start_aim_receive_call();
                    }
                    else
                    {
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                        write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);
                    }

                    if (lcd_fcs_default_icom == 0)
                    {
                        //   printf("inside if******************************************\n");
                        sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    else
                    {
                        //   printf("inside else******************************************\n");
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, lcd_fcs_default_icom);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    break;

                    // case ZONE_RESET_EVENT:
                case GLOBAL_RESET_EVENT:
                case APP_TIMEOUT_EVENT:
                    g_uiVoipapp_wait_timer = CLEAR;
                    // printf("Global reset event received [%s %d]\n",__FILE__,__LINE__);
                    printf("inside  PAGE_RESET_RINGBACK_STATE event is PAGE_RESET_EVENT \n");
                    pg_call.alm_no = CLEAR;
                    pvt_call.type = CLEAR;
                    pg_call.type = CLEAR;
                    close_channel(PG_CHANNEL);
                    pg_call.pg_call_busy = CLEAR;
                    g_uiPage_call_timer = CLEAR;

                    g_uiPage_call_timer_lcd = CLEAR;
                    g_uiPage_call_timer_frlcd_ini = CLEAR;
                    one_second_timeout = ONE_SEC_TIMEOUT;
                    // Call_cmdid=0;
                    pvt_call.pvt_call_busy = CLEAR;
                    // send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                    // send_msg_ui(IDLE_STATE, LCD, LINE1, CONTINUE_DISPLAY, global_terminate, LCD_CLR_FIRST_LINE, 0);
                    print_msg_on_lcd(IOCTL_PRINT, global_terminate, LINE1, ZERO, LCD_CLR_FIRST_LINE);
                    // usleep(500000);
                    // send_msg_ui(IDLE_STATE, LCD, LINE1, CONTINUE_DISPLAY, stn_config.fcs_desc, LCD_CLR_FIRST_LINE, 0);
                    print_msg_on_lcd(IOCTL_PRINT, stn_config.fcs_desc, LINE1, ZERO, LCD_CLR_FIRST_LINE);

                    pWork_m.private_state = PRIVATE_IDLE;
                    g_itransmit_fd_index = -1;
                    g_uiFsu_state = IDLE_STATE;
                    // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                    write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
                    drop_membership(g_iPaging_fd); ///
                    pvt_call.curr_icom_no = stn_config.default_intercom - MIN_OFFSET;
                    if (stn_config.icom_live_dead_status[stn_config.default_intercom - MIN_OFFSET] == LIVE)
                    {
                        add_membership(g_Iicom_fds[stn_config.default_intercom - MIN_OFFSET]);
                        start_intercom();
                    }

                    if (stn_config.default_AIM)
                    {
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                        write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
                        add_membership(g_iAim_fds[stn_config.default_AIM - ONE]);
                        start_aim_receive_call();
                    }
                    else
                    {
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                        write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);
                    }

                    if (lcd_fcs_default_icom == 0)
                    {
                        printf("inside if GLOBAL ---> lcd_fcs_default_icomn");

                        //   printf("inside if******************************************\n");
                        sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    else
                    {
                        printf("inside ELSE GLOBAL ---> lcd_fcs_default_icomn");

                        //   printf("inside else******************************************\n");
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, lcd_fcs_default_icom);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    break;
                }
                g_uiFsu_event = CLEAR;
                break;

            case PAGE_RESET_RINGBACK_STATE:
                printf("________INSIDE PAGE_RESET_RINGBACK_STATE \n");
                switch (g_uiFsu_event)
                {
                case WAKE_UP_EVENT:
                    printf("____________PAGE_RESET_RINGBACK_STATE______WAKE_UP_EVENT___\n");
                    reset_all();
                    g_uiVoipapp_wait_timer = CLEAR;
                    g_uiFsu_state = VCCS_REGISTERING_STATE;
                    break;

                    /*case BROADCAST_TIMEOUT_EVENT:
                         printf("Inside BROADCAST_TIMEOUT_EVENT\n");
                         Restart_Broadcast_Socket();
                    break;*/
                    //******** Added by Yogita******//
                    // problem: after entering in reset ringback state, station was not receving reset
                    // event of page call.So when APP_TIMEOUT_EVENT come station should go to idle.

                case ZONE_RESET_EVENT:
                    g_uiVoipapp_wait_timer = CLEAR;
                    write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
                    // printf("zone reset event received [%s %d]\n",__FILE__,__LINE__);
                    printf("inside  PAGE_RESET_RINGBACK_STATE event is ZONE_RESET_EVENT \n");
                    pg_call.alm_no = CLEAR;
                    pvt_call.type = CLEAR;
                    pg_call.type = CLEAR;
                    // close_channel(PG_CHANNEL);
                    pg_call.pg_call_busy = CLEAR;
                    g_uiPage_call_timer = CLEAR;
                    pvt_call.pvt_call_busy = CLEAR;

                    g_uiPage_call_timer_lcd = CLEAR;
                    g_uiPage_call_timer_frlcd_ini = CLEAR;
                    one_second_timeout = ONE_SEC_TIMEOUT;
                    // send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                    // send_msg_ui(IDLE_STATE, LCD, LINE1, CONTINUE_DISPLAY, zone_terminate, LCD_CLR_FIRST_LINE, 0);
                    print_msg_on_lcd(IOCTL_PRINT, zone_terminate, LINE1, ZERO, LCD_CLR_FIRST_LINE);
                    ////usleep(500000);
                    // send_msg_ui(IDLE_STATE, LCD, LINE1, CONTINUE_DISPLAY, stn_config.fcs_desc, LCD_CLR_FIRST_LINE, 0);
                    print_msg_on_lcd(IOCTL_PRINT, stn_config.fcs_desc, LINE1, ZERO, LCD_CLR_FIRST_LINE);
                    pWork_m.private_state = PRIVATE_IDLE;
                    g_itransmit_fd_index = -1;
                    g_uiFsu_state = IDLE_STATE;
                    // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);

                    drop_membership(g_izone_fds[pg_call.zone_no]); ///
                    pvt_call.curr_icom_no = stn_config.default_intercom - MIN_OFFSET;
                    if (stn_config.icom_live_dead_status[stn_config.default_intercom - MIN_OFFSET] == LIVE)
                    {
                        add_membership(g_Iicom_fds[stn_config.default_intercom - MIN_OFFSET]);
                        start_intercom();
                    }

                    if (stn_config.default_AIM)
                    {
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                        write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
                        add_membership(g_iAim_fds[stn_config.default_AIM - ONE]);
                        start_aim_receive_call();
                    }
                    else
                    {
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                        write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);
                    }
                    if (lcd_fcs_default_icom == 0)
                    {
                        printf("inside if ---> lcd_fcs_default_icomn");
                        sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    else
                    {
                        printf("inside else ---> lcd_fcs_default_icomn \n");
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, lcd_fcs_default_icom);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    break;
                    /*
                    case GLOBAL_RESET_EVENT:
                        g_uiVoipapp_wait_timer = CLEAR;
                        //printf("Global reset event received [%s %d]\n",__FILE__,__LINE__);
                        printf("inside  PAGE_RESET_RINGBACK_STATE event is GLOBAL_RESET_EVENT \n");
                        pg_call.alm_no=CLEAR;
                        pvt_call.type=CLEAR;
                        pg_call.type=CLEAR;
                        close_channel(PG_CHANNEL);
                        pg_call.pg_call_busy = CLEAR;
                        g_uiPage_call_timer=CLEAR;
                        pvt_call.pvt_call_busy = CLEAR;
                        send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                        pWork_m.private_state=PRIVATE_IDLE;
                        g_itransmit_fd_index=-1;
                        g_uiFsu_state=IDLE_STATE;
                        //operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);

                        pvt_call.curr_icom_no=stn_config.default_intercom-MIN_OFFSET;
                        if(stn_config.icom_live_dead_status[stn_config.default_intercom-MIN_OFFSET]==LIVE)
                        {
                            add_membership(g_Iicom_fds[stn_config.default_intercom-MIN_OFFSET]);
                            start_intercom();
                        }

                        if(stn_config.default_AIM)
                        {
                            //operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                            add_membership(g_iAim_fds[stn_config.default_AIM-ONE]);
                            start_aim_receive_call();
                        }
                        else
                        {
                            //operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                        }
                    break;

                    case APP_TIMEOUT_EVENT:
                        printf("APP timeout event received in PAGE_RESET_RINGBACK_STATE state\n");
                        g_uiVoipapp_wait_timer = CLEAR;
                        g_uiFsu_state=IDLE_STATE;
                    break;*/
                    // case ZONE_RESET_EVENT:

                case GLOBAL_RESET_EVENT:

                    // case APP_TIMEOUT_EVENT:
                    g_uiVoipapp_wait_timer = CLEAR;
                    // printf("###########APP_TIMEOUT_EVENT event in PAGE_RESET_RINGBACK_STATE \n");
                    getTime();
                    // printf("Global reset event received [%s %d]\n",__FILE__,__LINE__);
                    printf("inside  PAGE_RESET_RINGBACK_STATE event is PAGE_RESET_EVENT \n");
                    pg_call.alm_no = CLEAR;
                    pvt_call.type = CLEAR;
                    pg_call.type = CLEAR;
                    // Call_cmdid=0;
                    close_channel(PG_CHANNEL);
                    pg_call.pg_call_busy = CLEAR;
                    g_uiPage_call_timer = CLEAR;
                    pvt_call.pvt_call_busy = CLEAR;

                    g_uiPage_call_timer_lcd = CLEAR;
                    g_uiPage_call_timer_frlcd_ini = CLEAR;
                    one_second_timeout = ONE_SEC_TIMEOUT;
                    // send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,page_reset_msg,CLEAR);
                    // send_msg_ui(IDLE_STATE, LCD, LINE1, CONTINUE_DISPLAY, global_terminate, LCD_CLR_FIRST_LINE, 0);
                    print_msg_on_lcd(IOCTL_PRINT, global_terminate, LINE1, ZERO, LCD_CLR_FIRST_LINE);

                    // send_msg_ui(IDLE_STATE, LCD, LINE1, CONTINUE_DISPLAY, stn_config.fcs_desc, LCD_CLR_FIRST_LINE, 0);
                    print_msg_on_lcd(IOCTL_PRINT, stn_config.fcs_desc, LINE1, ZERO, LCD_CLR_FIRST_LINE);

                    pWork_m.private_state = PRIVATE_IDLE;
                    g_itransmit_fd_index = -1;
                    g_uiFsu_state = IDLE_STATE;
                    // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                    write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
                    drop_membership(g_iPaging_fd); ///
                    pvt_call.curr_icom_no = stn_config.default_intercom - MIN_OFFSET;
                    if (stn_config.icom_live_dead_status[stn_config.default_intercom - MIN_OFFSET] == LIVE)
                    {
                        add_membership(g_Iicom_fds[stn_config.default_intercom - MIN_OFFSET]);
                        start_intercom();
                    }

                    if (stn_config.default_AIM)
                    {
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                        write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
                        add_membership(g_iAim_fds[stn_config.default_AIM - ONE]);
                        start_aim_receive_call();
                    }
                    else
                    {
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                        write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);
                    }

                    if (lcd_fcs_default_icom == 0)
                    {
                        printf("inside if******************************************GLB\n");
                        sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    else
                    {
                        printf("inside else******************************************GLB\n");
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, lcd_fcs_default_icom);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }

                    break;

                case APP_TIMEOUT_EVENT:
                    printf("Inside APP_TIMEOUT_EVENT\n");

                    // if(pvt_call.type==GLOBAL || pg_call.type==GLOBAL_ALARM)
                    //{
                    if (req_retransmission_count == 0)
                    {
                        send_req_msg_to_server(0, last_req_sent_to_server, GLOBAL_RESET_FRAME_LENGTH, '0', '0');
                        printf("*****Retransmission of Reset frame to server******\n");
                        g_uiVoipapp_wait_timer = SERVER_RESPONSE_TIMEOUT;
                        req_retransmission_count = 1;
                        break;
                    }

                    //}
                    g_uiVoipapp_wait_timer = CLEAR;
                    printf("###########APP_TIMEOUT_EVENT event in PAGE_RESET_RINGBACK_STATE \n");
                    getTime();
                    // printf("Global reset event received [%s %d]\n",__FILE__,__LINE__);
                    pg_call.alm_no = CLEAR;
                    pvt_call.type = CLEAR;
                    pg_call.type = CLEAR;
                    // Call_cmdid=0;
                    close_channel(PG_CHANNEL);
                    pg_call.pg_call_busy = CLEAR;
                    g_uiPage_call_timer = CLEAR;
                    pvt_call.pvt_call_busy = CLEAR;

                    g_uiPage_call_timer_lcd = CLEAR;
                    g_uiPage_call_timer_frlcd_ini = CLEAR;
                    one_second_timeout = ONE_SEC_TIMEOUT;

                    // send_msg_ui(IDLE_STATE, LCD, LINE1, CONTINUE_DISPLAY, global_terminate, LCD_CLR_FIRST_LINE, 0);
                    print_msg_on_lcd(IOCTL_PRINT, global_terminate, LINE1, ZERO, LCD_CLR_FIRST_LINE);
                    pWork_m.private_state = PRIVATE_IDLE;
                    g_itransmit_fd_index = -1;
                    g_uiFsu_state = IDLE_STATE;
                    // operate_gpio(LED_CALL_THROUGH,WRITE,GPIO_OFF);
                    write_to_io(CALL_THROUGH_LED_OFF, IO_PIN_OFF);
                    drop_membership(g_iPaging_fd); ///
                    pvt_call.curr_icom_no = stn_config.default_intercom - MIN_OFFSET;
                    if (stn_config.icom_live_dead_status[stn_config.default_intercom - MIN_OFFSET] == LIVE)
                    {
                        add_membership(g_Iicom_fds[stn_config.default_intercom - MIN_OFFSET]);
                        start_intercom();
                    }

                    if (stn_config.default_AIM)
                    {
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                        write_to_io(PG_AMP_MUTE_ON, IO_PIN_ON);
                        add_membership(g_iAim_fds[stn_config.default_AIM - ONE]);
                        start_aim_receive_call();
                    }
                    else
                    {
                        // operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
                        write_to_io(PG_AMP_MUTE_OFF, IO_PIN_OFF);
                    }

                    if (lcd_fcs_default_icom == 0)
                    {
                        //   printf("inside if******************************************\n");
                        sprintf(g_cLine3_buf, IDLE_msg, stn_config.logical_id);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }
                    else
                    {
                        //   printf("inside else******************************************\n");
                        sprintf(g_cLine3_buf, Channel_msg, stn_config.logical_id, lcd_fcs_default_icom);
                        // send_msg_ui(IDLE_STATE, LCD, LINE2, CONTINUE_DISPLAY, g_cLine3_buf, LCD_CLR_SECOND_LINE, 0);
                        print_msg_on_lcd(IOCTL_PRINT, g_cLine3_buf, LINE2, ZERO, LCD_CLR_SECOND_LINE);
                    }

                    break;

                default:
                    break;
                }
                g_uiFsu_event = CLEAR;
                break;

            } // switch

        } // if(g_uiFsu_event)

    } // while(1)
    // printf("+++++++++++++++ CHECK END socket %d \n",g_uiUi_socket_fd);
    /*close_ui_thread();
    clear_all_fds();
    close_all_sockets();
    close_alarm_files();*/
    DPRINTK("Voip task stopping\n");
    voip_stack_stop();
    DPRINTK("[%s:%d]\n", __FUNCTION__, __LINE__);
    DPRINTK("FILE=%s, LINE=%d Calling dsp_uninit - voip task is out......\n", __FILE__,
            __LINE__);
    dsp_uninit();
#ifdef CONFIG_RG_ATA
    slic_uninit();

#ifdef FXO_SILABS
    slac_uninit();
#endif /*FXO_SILABS */

#elif CONFIG_RG_IPPHONE
#ifndef CONFIG_GTEK

#ifndef NANOX
//	lcdClose();
#endif

    closeKeypad();
#endif
#endif
    // operate_gpio(LED_LAN_SERV,WRITE,GPIO_OFF);
    write_to_io(LED_SERVER_IND_OFF, IO_PIN_OFF);
    DPRINTK("[%s:%d]\n", __FUNCTION__, __LINE__);
    DPRINTK("[%s:%d]\n", __FUNCTION__, __LINE__);

    // voip_stack_uninit();
    voip_conf_uninit();
    // ret_val = 0;

    printf("Voip task stopped\n"); // Avner August PS

    exit(0);

    return 0; // ret_val;
}

// 060530 Avner phase 1
/*int is_line_enabled(int line)
{
    return call_mngr_params.lines[line].enabled;
}
//060530 Avner phase 1
int is_using_proxy()
{
    return call_mngr_params.protocol == VOIP_PROT_SIP &&
        call_mngr_params.sp.sip.use_proxy;
}*/

/*static void load_dsp_kernel_modules()
{
    if(isFirstLoading)	/* first time loading - load the kernel modules *
    {
#if defined(I_PLATFORM)
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
    if (isFirstLoading) /* first time loading - load the kernel modules */
    {

#ifdef FXS_LEGERITY
#if defined(P_PLATFORM)
        system("insmod /var/ftp/le88drv.ko");
        //		system("insmod /usr/bin/audiocodes/slic/le88drv.ko");

#elif defined(I_PLATFORM)
        //  	system("insmod /usr/bin/audiocodes/slic/le88drv.ko");

#elif defined(B_PLATFORM) || defined(B_CEVA_PLATFORM) || defined(GROCX_PLATFORM)
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
#endif /*FXS_SILABS*/
    }
}

#endif /*CONFIG_RG_ATA*/
