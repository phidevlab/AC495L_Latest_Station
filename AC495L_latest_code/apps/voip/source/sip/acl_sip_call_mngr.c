/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	acl_sip_call_mngr.c                                 *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 26/02/08 - Coding started.                                       *
 *																	*
 ********************************************************************/

#include <rvmdmobjects.h>
#include <rvccprovidermdm.h>
#include <rvccterminalmdm.h>
#include <RvSipOtherHeader.h>
#include <RvSipMsgFieldDispatcher.h>
#include <MsgFieldDispatcher.h>
#include <rvcctext.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <RvSipMsg.h>
#include <rvstring.h>
#include <RvSipStackTypes.h>
#include <RvSipMsgTypes.h>
#include <rvSipControlApi.h>
#include <rvMdmControlApi.h>
#include <rvmdmtermevent.h>
#include <RvSipBody.h>
#include <rvlog.h>
#include <_SipTransactionTypes.h>

#include <RvSipStackTypes.h>
#include <RvSipStack.h>
#include <RvSipSubscriptionTypes.h>
#include <RvSipSubscription.h>
#include <RvSipSubscriptionStateHeader.h>
#include <sipphone.h>



#include <RvSipStackTypes.h>
#include <RvSipStack.h>
#include <RvSipSubscriptionTypes.h>
#include <RvSipSubscription.h>
#include <RvSipSubscriptionStateHeader.h>
#include <sipphone.h>
#ifdef STUN
#include  <sipStun.h>
#endif
#include <RvSipBodyPart.h>
#include <acl_common.h>
#include <acl_call_mngr.h>
//#include <dsp/phone.h>
#include <acl_voip_conf.h>
#include <acl_telephony_adv_sip_features.h>
#include <RvSipTransaction.h>
#include <_SipTransactionTypes.h>
//#include <olStunClient.h>
#include <rvmdmobjects.h>
#include <rvccprovidermdm.h>

#include "voip_utils.h"

/* AlexChen SDK upgrade 20090413 */
/*EA - 080807 - VI65537 - RFC 4240 - interop - dealing with remote parties that do not support REFER inside INVITE message*/
#include <RvSipReferredByHeader.h>
#include <RvSipDateHeader.h> /*ACL XY 090115 VI69874  */
#include <RvSipReferToHeader.h>
#include <MsgTypes.h>
#include <rvccconnsip.h>
#include <RvSipOtherHeader.h>



#include "acl_call_mngr.h"
#include "acl_line_connection.h"
//#include "ipphone.h"
#include "voip_main.h"
#include "rvphone_common.h"
#include "voip_lcd.h"
#include "acl_ui_commands.h"
#include <rvlog.h>
#include "acl_rv_sip_api.h"
#include "led.h"
#include "voip_status_handler.h"
#include "iphone_fxs.h"

#include <rvMdmControlApi.h>
#include "acl_voip_conf.h"//Nirp 060719
#include <acl_telephony_adv_sip_features.h> /* ACL NB 060927 */
#include <rvIppCfwApi.h> /* ACL NB 061115 */
#include <rvmdmtermevent.h>

/*  EA - 071218 - VI#59630 support SIP over TLS */
#ifdef RV_CFLAG_TLS
#if (RV_TLS_TYPE == RV_TLS_OPENSSL)
#include <openssl/ssl.h>
#endif
#include <rvSipTlsApi.h>
#endif /* RV_CFLAG_TLS */

#include "voip_utils.h"

#include "iphone_paging_pub.h"

#ifdef CONFIG_RG_IPPHONE
#include <acl_LCD_adaptation_layer.h>
static acl_call_info_t callInfo[2]; /*2=maximum number of calls (in Orchid)*/
#endif /*CONFIG_RG_IPPHONE*/
static void onhook_key_simulate(RvMdmTerm  *pMdmTerm);
// by itzik - 19072006 - numberRules
static int next_rule_get(char *numberRules,int *ruleLen, int *keyStart, int *keyLen, int *patternStart, int *patternLen);
static int dial_str_2_key_compare(char *dialString, char *key, int keyLen);
static int addr_build(char *dialString, char *key, int keyLen, char *pattern, int patternLen, char *address);
static int sip_addr_from_number_rules_get(char *numberRules, char *dialString, char *sipAddr);
void sip_refer_received_remote_accepted_handler(RvMdmTerm *pMdmTerm);

// ER06
int dsp_msg_log(sip_log_message_t *logmsg);
sip_log_message_t logmsg;
void sip_param_add(RvMdmParameterList* list, char* paramName, char* paramValue);/* ACL NB 061113 */
void sip_cfw_param_add(RvMdmParameterList* pParamList, char* paramName, char* paramValue);/* ACL NB 061210 */
extern void acl_sip_call_mngr_user_agent_get(char *);/* ACL NB 061225 */
#ifdef CONFIG_RG_ATA
static void acl_sip_call_mngr_blind_transfer_process(void  *pMdmTerm);
#endif
static int dist_ring_type_get(char* distRingVal);

#ifdef AC488_CPE_VOIP_TOOLKIT
static int isOtherTermInIdleState(void);
#endif /*AC488_CPE_VOIP_TOOLKIT*/

/*EA - 080526 - VI64130 - in order to return to first call (held call)
	after second call didnt answer you have to press flash twice.*/
int acl_does_other_held_conn_exist(RvMdmTerm *pMdmTerm);
int acl_is_during_active_call(RvMdmTerm *pMdmTerm);
/*  EA - 080207 VI#60671 Setting Syslog IP address for application logs through GUI
	added this function to let RV receive UDP server address from us*/
#define SYSLOG_PRINT_TO_UDP         0x1
#define SYSLOG_PRINT_TO_TERMINAL    0x2

extern void acl_sip_call_mngr_udp_g_cServer_addr_get(int *serverPortForAppSyslog, char **serverAddrForAppSyslog, int *printToUdp, int *printToTerminal);
/* Temporary global varable  070628 */
static char isBlindTrans;

 char buffer[15]="CALL_ANSWERED";
    char remote_string[32];

#define  ACL_SIP_CALL_MNGR_DEBUG 1

#ifndef ACL_SIP_APPLICATION
#include <misc_funcs.h>
#include <../mgt/lib/mgt_misc.h> /* ACL NB 070410 */
#endif

#ifdef ACL_SIP_CALL_MNGR_DEBUG
//ACL NB 070405
/*071016 - EA did this change during work on VI#57767 in order to syncronize our prntings with RV*/
#define DBG_PRINT(a1,a2...)	IppLogMessage(RV_FALSE, "[%s:%4d] " a1,__FUNCTION__, __LINE__, ##a2)
#else
#define DBG_PRINT(a1,a2...)
#endif

#define DBG_PRINT_ERR(a1,a2...) IppLogMessage(RV_TRUE, "[%s:%4d] " a1,__FUNCTION__, __LINE__, ##a2)

#ifndef CONFIG_RG_VOIP_RV_SIP /* need to define it in case H is not included */
#define RV_SIPCTRL_ADDRESS_SIZE 64
#endif

/*ACL NB 060406 */
char  *t38MediaCap = {
"v=0\r\n\
o=ac494 $ $ IN IP4 $\r\n\
s=-\r\n\
c=IN IP4 $\r\n\
t=0 0\r\n\
m=image $ UDPTL t38\r\n\
a=T38FaxVersion:0\r\n\
a=T38FaxMaxBuffer:1024\r\n\
a=T38FaxMaxDatagram:122\r\n\
a=T38FaxRateManagement:transferredTCF\r\n\
a=T38FaxUdpEC:t38UDPRedundancy\r\n\
"};

/*ACL NB 060704 Fax bypass */
char  *FaxBypassMediaCap = {
"v=0\r\n\
o=ac494 $ $ IN IP4 $\r\n\
s=-\r\n\
c=IN IP4 $\r\n\
t=0 0\r\n\
"};
/*  Nirp 060719 media descriptor and rtpmap are constructed dynamically.
    gpmd - is added via dedicated SDP build API
m=audio $ RTP/AVP 0\r\n\
a=rtpmap:0 pcmu/8000\r\n\
a=gpmd:0 vbd=yes;ecan=on\r\n\
*/

typedef struct {
    RvMdmTerm *ui;
    RvMdmTerm *at;
} term_ep_ipphone_t;

typedef struct {
    RvMdmTerm *al;
} term_ep_ata_t;

typedef union {
    term_ep_ipphone_t   ipphone;
    term_ep_ata_t       ata;
} term_ep_general_t;

/* EA - 071203 - implementing remote conference RFC 4240*/
#define REMOTE_CONF_ADDR_MAX_LEN 64

/*AlexChen 20090402 SDK upgrade enum for keep-alive OPTION message type*/
typedef enum{
		e_REDUNDANT_PROXY_TYPE,
		e_REGULAR_TYPE
}KAOptionType_e;

typedef enum {
    REMOTE_CONF_NOT_IN_PROGRESS,
    REMOTE_CONF_FIRST_CALL_TRANS,
    REMOTE_CONF_SECOND_CALL_TRANS,
    REMOTE_CONF_DISCONNECT_FIRST,
    REMOTE_CONF_DISCONNECT_SECOND,
    REMOTE_CONF_IS_ON,
} voip_remote_conf_state_t;

typedef struct {
    voip_remote_conf_state_t remoteConferenceState;
    char remoteConferenceId[REMOTE_CONF_ADDR_MAX_LEN];
} remote_conference_info_t;

typedef struct {
    term_ep_general_t line;
    void *data;
    int send_caller_id;
    remote_conference_info_t *remoteConferenceInfo;
    analog_port_type_t enum_analog_port_type;//ACL NIRP 071128
} acl_analog_line_t;

typedef struct {
    RvMdmTerm *eph;
    void *data;
    acl_analog_line_t *analogLine;
} acl_sip_call_t;

static RvMdmTermMgr term_mgr;
#ifdef CONFIG_RG_ATA
static RvMdmTermClass *al_class;
#endif
#ifdef CONFIG_RG_IPPHONE
static RvMdmTermClass *ui_class, *at_class;
#endif
static RvMdmTermClass *rtp_class;

static acl_analog_line_t *analog_lines;
static acl_analog_line_t *analogLine1;
static int is_up;

static acl_call_mngr_cbs_t *term_call_mngr_cbs;
static acl_call_mngr_params_t *call_mngr_params;

/* ACL NB 060404  globals for re-invite (fax ) */
//static RvSdpMsg g_LastCreateMsg;
static RvSdpMsgList g_sdp_list;

static IppTimer		myTimer;

/* ACL NB 061210 global for CFW */
static voip_cfw_params_t    g_cfwParams;
/* ACL NB 061225 definitions for user-Agent */
#define MAX_USER_AGENT 128
static char g_userAgentStr[MAX_USER_AGENT+1]; // null terminated string

static int sip_fax_local_sdp_build(RvMdmTerm *term, RvSdpMsg* sdpMsg, int  faxrate,int faxMethod);
int SIPLogLevel = 0; // ER07
static void key_event_process(char *package, char *id, char *key_id, RvMdmTerm *term);
static int line_get_by_term(RvCCTerminal *term);
/*EA - 071119 - fix sdp for vbd*/
static int isVbd(RvSdpMediaDescr* descr);
int registration_to_sip_server();

#ifdef CONFIG_RG_ATA
static int ata_process_event(int event, RvMdmTerm *term, int down);
#endif
static int active_line_Id_get(RvMdmTerm *term);
static int connection_state_get(RvMdmTerm *term);
static void remote_conference_process(RvMdmTerm *term);
static void key_sw_event_process(char *package, char *id, char *key_id, RvMdmTerm *term);
static RvBool myProcessTimer( void* data);

/*AlexChen 20090402 SDK upgrade */
static void KAMessageHandler(int lineIdx, RvCCTerminal *xTerm);
static void SetKAMessageHandle(KAOptionType_e KAOptionType, RvCCTerminal *xTerm, KA_Option_Handle_t *KA_Option_Handle);

/*#define KEY_ID_MAX_SIZE 5 *//* greatest known key id value is 1999 */
/*ACL NB 060723 -  package  is dd-end  since RADVISION  Incident #126956: Out-of-band DTMF for analog */
#define KEY_ID_MAX_SIZE 7 /* greatest known key id value is 1999 and greatest known package is dd-end */

typedef struct key_event_emulation_t {
    struct key_event_emulation_t *next;
    RvMdmTerm *term;
    char package[KEY_ID_MAX_SIZE];
    char id[KEY_ID_MAX_SIZE];
    char key_id[KEY_ID_MAX_SIZE];
    /* ACL NB - 061114 - we need also to pass parameter list of a key */
    RvMdmParameterList  paramList;
} key_event_emulation_t;

static key_event_emulation_t *key_event_emulation_queue;

//ER06 - Array containing some line status info (may be deleted after merge with Avner's code).
typedef struct line_status_t {
	int line_registration_state;
	call_origin_t active_call_origin;
   char secondary_dial_tone_on;
   char regretCallSimHookOn;	/* AlexChen SDK upgrade 20090414 */
   int  authRetriesCounter;		/* AlexChen SDK upgrade 20090414 */
} line_status_t;

static line_status_t *line_status;

/* AlexChen SDK upgrade 20090414 */
/*ACL AL 081029 - VI67678 LastCreateMsg struct should be per line*/
/* ACL AL 081119 VI57702 - 491 occurs when reinvites are sent simultaneously. add switchToVoice_localSDP */
typedef struct lastCreateMsg_t {
	acl_sip_call_t *termUserData;
	RvSdpMsg lastCreateMsg;
	RvSdpMsg switchToVoice_localSDP;
} lastCreateMsg_t;

static lastCreateMsg_t *lastCreateMsg;
static int g_lastCreateMsg_Entries=0;


RvBool rvCCTerminalSendg_cCallerId(
    IN RvCCTerminal*    x,
    IN const RvChar*    g_cCallerName,
    IN const RvChar*    g_cCallerNumber,
    IN const RvChar*    g_cCallerAddress,
    IN const RvChar*    g_cCallerId);

RVAPI RvBool rvMdmTermSetPresentationInfo(RvMdmTerm* term, RvMdmTermPresentationInfo* presentationInfo);
static void addSignalParam(RvMdmParameterList* params,const char* id,const char* val);

//#define RV_3_0
#if defined(RV_3_0)
/*Paul 2013-0401, Upgrade RV2.5 to RV3.0*/
RvBool rvCCTerminalSendg_cCallerId(
    IN RvCCTerminal*    x,
    IN const RvChar*    g_cCallerName,
    IN const RvChar*    g_cCallerNumber,
    IN const RvChar*    g_cCallerAddress,
    IN const RvChar*    g_cCallerId)
{
    RvMdmParameterList params;
    RvBool res;

    if (x == NULL)
        return RV_FALSE;

    rvMdmParameterListConstruct(&params);

    addSignalParam(&params, "name", g_cCallerName ? g_cCallerName : "");
    addSignalParam(&params, "number", g_cCallerNumber ? g_cCallerNumber : "");
    addSignalParam(&params, "address", g_cCallerAddress ? g_cCallerAddress : "");
    addSignalParam(&params, "id", g_cCallerId ? g_cCallerId : "");

    res = rvCCTerminalMdmStartSignalUI(x, "rvcc", "g_cCallerid", &params);

    rvMdmParameterListDestruct(&params);

    return res;
}

/*MarK 20130403*/
RvBool rvMdmTermSetPresentationInfo(RvMdmTerm* x,
                                             RvMdmTermPresentationInfo* presentationInfo)
{
    RvCCTerminal*       t   = NULL;
    RvCCTerminalMdm*    term = NULL;

    if ((x == NULL) || (presentationInfo == NULL))
        return RV_FALSE;

    t = (RvCCTerminal *)rvMdmTermGetXTerm_(x);

    if ((term = rvCCTerminalMdmGetImpl(t)) == NULL)
        return RV_FALSE;

    rvMdmTermPresentationInfoCopy(&(term->presentationInfo), presentationInfo);

    return RV_TRUE;
}

/*Paul 2013-0401, Upgrade RV2.5 to RV3.0*/
static void addSignalParam(RvMdmParameterList* params,const char* id,const char* val)
{
    RvMdmParameterValue value;

    rvMdmParameterValueConstruct(&value, val);
    rvMdmParameterListSet2(params, id, &value);
    rvMdmParameterValueDestruct(&value);

}


/*Paul 2013-0401, Upgrade RV2.5 to RV3.0*/
RvBool  acl_IsStateLegalForReInviteHandling(RvCCConnection  *rvConn,int callIndex)
{
     RvSipCallLegHandle sipCallLeg = NULL;
     RvSipTranscHandle activeTranHandle = NULL;
     RvIppTerminalHandle termHndl = rvIppMdmConnGetTerminal((RvIppConnectionHandle)rvConn);
     RvMdmTerm*  mdmTerm = rvIppMdmTerminalGetMdmTerm(termHndl);
     RvCCTerminal   *xTerm = (RvCCTerminal*)mdmTerm->xTerm;
     RvSipCallLegState state;

    if (xTerm)
    {
        RvCCConnection* conn=rvCCTerminalGetConnectionByIndex(xTerm,callIndex-1);

        if (conn)
        {
            RvIppConnectionHandle sipConnHndl = rvIppMdmConnGetConnectParty((RvIppConnectionHandle)conn);

            if (sipConnHndl)
            {
                sipCallLeg = rvIppSipControlGetCallLeg(sipConnHndl);

                if (sipCallLeg)
                {
                    RvSipCallLegGetCurrentState(sipCallLeg, &state);

                    DBG_PRINT("call leg state %d conn index %d\n",state,callIndex-1);

                    if( state == RVSIP_CALL_LEG_STATE_CONNECTED )
                       activeTranHandle = SipCallLegGetActiveTransaction(sipCallLeg);
                }
            }
        }
    }

	acl_line_t *p_line = get_line(0);
   /*make sure there is no other pending re-invite transaction.otherwise,unhold operation will be failed*/
   if(  p_line->is_illegal_for_reinvite == 1 || activeTranHandle != NULL)
   {
       RvCCConnection *conn1=rvCCTerminalGetConnectionByIndex(xTerm,0);
       RvCCConnection *conn2=rvCCTerminalGetConnectionByIndex(xTerm,1);
       RvCCConnection *conn3=rvCCTerminalGetConnectionByIndex(xTerm,2);
       RvCCTermConnState state1 = RV_CCTERMCONSTATE_IDLE;
       RvCCTermConnState state2 = RV_CCTERMCONSTATE_IDLE;
       RvCCTermConnState state3 = RV_CCTERMCONSTATE_IDLE;

       p_line->is_illegal_for_reinvite = 0;

       if( conn1 )
           state1=rvCCConnectionGetTermState(conn1);

       if( conn2 )
           state2=rvCCConnectionGetTermState(conn2);

       if( conn3 )
           state3=rvCCConnectionGetTermState(conn3);

       if( (state1==RV_CCTERMCONSTATE_IDLE && state2==RV_CCTERMCONSTATE_IDLE)
          || (state1==RV_CCTERMCONSTATE_IDLE && state3==RV_CCTERMCONSTATE_IDLE)
          || (state2==RV_CCTERMCONSTATE_IDLE && state3==RV_CCTERMCONSTATE_IDLE) )
       {
           DBG_PRINT("only one active call,do not check if it is legal for reinvite\n");
       }
       else
       {
           DBG_PRINT("ignore line event activeTranHandle %p\n",activeTranHandle);
           return RV_FALSE;
       }
   }

   return RV_TRUE;
}
/*Paul 2013-0401, Upgrade RV2.5 to RV3.0*/
int voip_conf_get_number_of_terms(void)
{	/*Just for MRD project, we return the number of terms directly, for other project this function must be re-implemented*/
	return 1;
}

RvBool acCCTerminalMdmOtherHeldConnExist(RvCCTerminal* t, RvCCConnection* currConn)
{
	return rvCCTerminalMdmOtherHeldConnExist(t, currConn, NULL);
}
#else
RvBool acCCTerminalMdmOtherHeldConnExist(RvCCTerminal* t, RvCCConnection* currConn)
{
	return rvCCTerminalMdmOtherHeldConnExist(t, currConn);
}
#endif




#ifdef CONFIG_RG_IPPHONE

static void remove_illegal_description(char* description)
{
	/*removes description that contain only white spaces*/
	unsigned int i;
	for (i=0;i<strlen(description);i++)
	{
		if (description[i]!=' ')
			return;
	}
	description[0]=0;
}
static void remove_description_commas(char *description)
{
	int len;

	if (description!=NULL)
	{
		/* convert from "description" to description*/
		len=strlen(description)-1;
		if (description[0]=='"' && description[len]=='"')
		{
			description[len]=0;
			strcpy(description,description+1);
		}
	}
}

static void set_call_info(int callIndex,const char* call_number,const char *call_description)
{
	callIndex--;
	if (call_description!=NULL)
	{
		strcpy (callInfo[callIndex].callDescription,call_description);
		remove_description_commas(callInfo[callIndex].callDescription );
		remove_illegal_description(callInfo[callIndex].callDescription );
	}
	else
	{
		strcpy (callInfo[callIndex].callDescription,"");
	}
	if (call_number)
	{
		strcpy(callInfo[callIndex].callNumber,call_number);
	}
	else
	{
		strcpy(callInfo[callIndex].callNumber,"");
	}
}
static char* get_call_number(int callIndex)
{
	callIndex--;
	return callInfo[callIndex].callNumber;
}
static char* get_call_description(int callIndex)
{
	callIndex--;
	return callInfo[callIndex].callDescription;
}
#endif

/*  EA - 071218 - VI#59630 support SIP over TLS */
#ifdef RV_CFLAG_TLS
static RvBool sip_tls_get_buffer_cb(IN   RvIppTlsBufferType bufferType,
                                  OUT	RvChar			*tlsBuffer,
                                  OUT	RvUint32		*tlsBufferLen);

static void sip_tls_post_connection_assertion(IN    RvSipTransportConnectionHandle             hConnection,
									          IN    RvSipTransportConnectionAppHandle          hAppConnection,
									          IN    RvChar*                                    strHostName,
									          IN    RvSipMsgHandle                             hMsg,
									          OUT   RvBool*                                    pbAsserted);

RvIppSipTlsExtClbks mdm_SipTlsExtClbks =
{
	/* RvIppSipTlsGetBufferCB				tlsGetBufferF; */
    sip_tls_get_buffer_cb,
	/* RvIppSipTlsPostConnectionAssertionCB	tlsPostConnectionAssertF; */
    sip_tls_post_connection_assertion
};
#endif /* RV_CFLAG_TLS */
/* EA - 071203 - implementing remote conference RFC 4240*/
/*
    if remote conference is configured and a conference was initialized
    1. allocate the remote conference structure.
    2. set the remote conf state to REMOTE_CONF_NOT_IN_PROGRESS.
    3. build up the remote conference unique Id.
*/

/* AlexChen SDK upgrade 20090414 */
/*ACL AL 081029 - VI67678 LastCreateMsg struct should be per line*/
/****************************************/
/*  LastCreateMsg Database mengment API */
/****************************************/
static pthread_mutex_t LastCreateMsg_mutex;

#define LastCreateMsg_lock_init() pthread_mutex_init(&LastCreateMsg_mutex, NULL)
//#define LastCreateMsg_lock_uninit() pthread_mutex_destroy(&LastCreateMsg_mutex)

#define LastCreateMsg_lock() {int iCounter=0; \
      while (1){  \
      if (0 == pthread_mutex_trylock(&LastCreateMsg_mutex)) \
         break; \
      if (!(++iCounter%10000)) \
      { \
         DBG_PRINT("%s: waiting in line %d \n",__FUNCTION__, __LINE__); \
         break; \
      } \
      } }

#define LastCreateMsg_unlock() pthread_mutex_unlock(&LastCreateMsg_mutex)


static int lastCreateMsg_initDB(int numOfLines )
{
   int i;

   LastCreateMsg_lock_init();

   g_lastCreateMsg_Entries = numOfLines*3 + 2;

   if (!(lastCreateMsg = AC_ZALLOC_E(sizeof(lastCreateMsg_t)*g_lastCreateMsg_Entries)))
   {
	   DBG_PRINT_ERR("lastCreateMsg_initDB  failed to allocate memory\n");
	   return 1;
   }

   for(i=0 ; i < g_lastCreateMsg_Entries ; i++)
   {
      lastCreateMsg[i].termUserData = NULL;
   }


   return 0;
}

static int lastCreateMsg_add(RvSdpMsgList *pLocal_list, RvMdmTerm *pTerm)
{
   int i, ret = -1;

   LastCreateMsg_lock();

   for(i=0 ; i < g_lastCreateMsg_Entries; i++)
   {
	   DBG_PRINT("lastCreateMsg[i].termUserData = %p\r\n", lastCreateMsg[i].termUserData);
	   if (lastCreateMsg[i].termUserData == NULL)
	   {
		   DBG_PRINT("i=%d, found empty place!!\r\n", i);
		   rvSdpMsgDestruct( &lastCreateMsg[i].lastCreateMsg );
		   rvSdpMsgConstructCopy( &lastCreateMsg[i].lastCreateMsg, rvSdpMsgListGetElement( pLocal_list, 0));
		   lastCreateMsg[i].termUserData = (acl_sip_call_t *)rvMdmTermGetUserData(pTerm);
		   DBG_PRINT("lastCreateMsg[i].termUserData = %p\r\n", lastCreateMsg[i].termUserData);
		   ret = i;
		   break;
	   }
   }

   LastCreateMsg_unlock();

   if ( ret == -1)
      DBG_PRINT_ERR("lastCreateMsg DB is Full\n");

   return ret;
}


static int lastCreateMsg_delete(RvMdmTerm *pTerm)
{
   int i;
   int ret = -1;
   acl_sip_call_t *pUserData = NULL;

   pUserData = (acl_sip_call_t *)rvMdmTermGetUserData(pTerm);

   if (pUserData == NULL)
   {
	   DBG_PRINT_ERR("No user data\n");
	   return -1;
   }

   LastCreateMsg_lock();

   for(i=0 ; i < g_lastCreateMsg_Entries; i++)
   {
	   DBG_PRINT("lastCreateMsg[i].termUserData = %p, pTerm = %p, pUserData = %p\r\n", lastCreateMsg[i].termUserData, pTerm, pUserData);
	   if (lastCreateMsg[i].termUserData == pUserData)
	   {
		   DBG_PRINT("i=%d, lastCreateMsg[i].termUserData == pUserData!!! deleting!!!\r\n", i);
		   lastCreateMsg[i].termUserData = NULL;
		   ret = i;
		   break;
	   }
   }

   LastCreateMsg_unlock();

   if ( ret == -1)
	   DBG_PRINT_ERR("lastCreateMsg doesn't exist in DB\n");

   return ret;
}

static int lastCreateMsg_getByTerm(RvMdmTerm *pTerm)
{
   int i;
   acl_sip_call_t     *connUserData;
   acl_analog_line_t	*endpointUserData;

   for(i=0 ; i < g_lastCreateMsg_Entries; i++)
   {
	  DBG_PRINT("i=%d, lastCreateMsg[i].termUserData = %p\r\n", i, lastCreateMsg[i].termUserData);
      if (rvMdmTermGetType(pTerm) == RV_MDMTERMTYPE_PHYSICAL)
      {
         endpointUserData = (acl_analog_line_t *)rvMdmTermGetUserData(pTerm);
         DBG_PRINT("endpointUserData = %p\r\n", endpointUserData);
         if ((lastCreateMsg[i].termUserData) && (lastCreateMsg[i].termUserData->analogLine == endpointUserData))
         {
            DBG_PRINT("i=%d, lastCreateMsg[i].termUserData->ep = %p found pTerm!!!\r\n", i, lastCreateMsg[i].termUserData->eph);
			return(i);
         }


      }
      else if (rvMdmTermGetType(pTerm) == RV_MDMTERMTYPE_EPHEMERAL)
      {
         connUserData = (acl_sip_call_t *)rvMdmTermGetUserData(pTerm);
         DBG_PRINT("connUserData = %p\r\n", connUserData);
         if ((lastCreateMsg[i].termUserData) && (lastCreateMsg[i].termUserData == connUserData))
         {
            DBG_PRINT("i=%d, lastCreateMsg[i].termUserData = %p found pTerm!!!\r\n", i, lastCreateMsg[i].termUserData);
            return(i);
         }
      }
   }
   DBG_PRINT_ERR("lastCreateMsg doesn't exist in DB\n");
   return -1;
}

static void lastCreateMsg_delete_switchToVoice_localSDP( int index)
{

   DBG_PRINT(">>\r\n");

	LastCreateMsg_lock();
	if (&lastCreateMsg[index].switchToVoice_localSDP != NULL)
   {
	   DBG_PRINT("lastCreateMsg[index].switchToVoice_localSDP != NULL, deleting...\r\n");
		rvSdpMsgDestruct(&lastCreateMsg[index].switchToVoice_localSDP);
	}
	LastCreateMsg_unlock();

   DBG_PRINT(">>\r\n");
}

#ifdef CONFIG_RG_ATA
static void remote_conference_init(RvMdmTerm *term)
{
    acl_analog_line_t  *analogLine;
    RvCCTerminal    *xTerm      = (RvCCTerminal*)term->xTerm;
    char            *confAddr   = NULL;
    int             line;

    analogLine = rvMdmTermGetUserData(term);

    /*allocate the remote conference structure and go into the handler function*/
    if (analogLine->remoteConferenceInfo == NULL)
    {
        analogLine->remoteConferenceInfo = AC_ZALLOC_L(sizeof(remote_conference_info_t));
        memset(analogLine->remoteConferenceInfo->remoteConferenceId, 0, REMOTE_CONF_ADDR_MAX_LEN);
        analogLine->remoteConferenceInfo->remoteConferenceState = REMOTE_CONF_NOT_IN_PROGRESS;
        /*build the conf Id*/
        voip_conf_remote_conference_conf_addr_get(&confAddr);
        line = line_get_by_term(xTerm);
        if ((confAddr != NULL) && (line != -1))
        {
            sprintf(analogLine->remoteConferenceInfo->remoteConferenceId,
                    "conf=%s%02d@%s",
                    voip_conf_remote_conference_mac_addr_get(),
                    line, confAddr);
        }
    }
    if (analogLine->remoteConferenceInfo->remoteConferenceState != REMOTE_CONF_NOT_IN_PROGRESS)
    {
        DBG_PRINT_ERR("incorrect state = %d\n", analogLine->remoteConferenceInfo->remoteConferenceState);
    }
}
#endif /*CONFIG_RG_ATA*/

static void remote_conference_end(RvMdmTerm *term)
{
    acl_analog_line_t *analogLine;

    analogLine = rvMdmTermGetUserData(term);
    analogLine->remoteConferenceInfo->remoteConferenceState = REMOTE_CONF_NOT_IN_PROGRESS;
}

static voip_remote_conf_state_t remote_conference_state_get(RvMdmTerm *pMdmTerm)
{
    acl_analog_line_t *analogLine;

    if (NULL == pMdmTerm)
        return REMOTE_CONF_NOT_IN_PROGRESS;
    analogLine = rvMdmTermGetUserData(pMdmTerm);
    /*  in case conference is configured to local ep->remoteConferenceInfo == NULL and
        REMOTE_CONF_NOT_IN_PROGRESS will be returned*/
    if ((NULL == analogLine) || (NULL == analogLine->remoteConferenceInfo))
        return REMOTE_CONF_NOT_IN_PROGRESS;
    return analogLine->remoteConferenceInfo->remoteConferenceState;
}

static void remote_conference_state_set(RvMdmTerm *pMdmTerm, voip_remote_conf_state_t rcState)
{
    acl_analog_line_t *analogLine;

    analogLine = rvMdmTermGetUserData(pMdmTerm);
    if ((NULL == analogLine) || (NULL == analogLine->remoteConferenceInfo))
    {
	   analogLine->remoteConferenceInfo->remoteConferenceState = rcState;
	   DBG_PRINT("rcState = %d\r\n", analogLine->remoteConferenceInfo->remoteConferenceState);
    }
}
#ifdef CONFIG_RG_ATA
static char *remote_conference_Id_get(RvMdmTerm *pMdmTerm)
{
    acl_analog_line_t *analogLine;

    analogLine = rvMdmTermGetUserData(pMdmTerm);
    if ((NULL == analogLine) || (NULL == analogLine->remoteConferenceInfo))
        return NULL;
    else
        return analogLine->remoteConferenceInfo->remoteConferenceId;
}

static void remote_conference_dialtone_handler(RvMdmTerm *pMdmTerm)
{
    char *limitedAddress = NULL;
    RvMdmParameterList args;
    voip_remote_conf_state_t rcState;

    rcState = remote_conference_state_get(pMdmTerm);

    DBG_PRINT("rcState = %d", rcState);
    if ((REMOTE_CONF_FIRST_CALL_TRANS == rcState) ||
        (REMOTE_CONF_SECOND_CALL_TRANS == rcState) ||
        (REMOTE_CONF_DISCONNECT_SECOND == rcState))
    {
        rvMdmParameterListConstructA(&args, pMdmTerm->termClass->a);
        limitedAddress = remote_conference_Id_get(pMdmTerm);
        if (NULL != limitedAddress)
        {
            DBG_PRINT("limitedAddress = %s", limitedAddress);
            rvMdmDigitMapBuildDDEvComplete(&args, limitedAddress, RV_MDMDIGITMAP_UNAMBIGUOUSMATCH, pMdmTerm->termClass->a);
            /*send a "DIALCOMPLETED" event to end the dialing phase*/
            rvMdmTermProcessEvent(pMdmTerm, "kp", "ce", NULL, &args);
            rvMdmParameterListDestruct(&args);
        }
        if (REMOTE_CONF_DISCONNECT_SECOND == rcState)
        {
            /*  only for this state change the state here baecuase only in this case it is important
                to move to the next step only after sending DIALCOMPLETED*/
            remote_conference_state_set(pMdmTerm, REMOTE_CONF_IS_ON);
        }
    }
}

#endif /*CONFIG_RG_ATA*/

static void remote_conference_blind_transfer_complete(RvMdmTerm *pMdmTerm)
{
    voip_remote_conf_state_t rcState;

    rcState = remote_conference_state_get(pMdmTerm);

    if (rcState == REMOTE_CONF_FIRST_CALL_TRANS)
    {
    /*   step 2 - we reach here once the blind transfer ends - now take the first call out of hold
        and make it your active call    */
        int lineId;
        int connState;
        connState = connection_state_get(pMdmTerm);
        lineId = active_line_Id_get(pMdmTerm);
        /*if ( connState == RV_CCCONNSTATE_CONNECTED ) ACL NB 070613 */
        if (( connState != RV_CCCONNSTATE_CALL_DELIVERED ) &&
            ( connState != RV_CCCONNSTATE_INPROCESS      )   )
        {
            key_sw_event_process("kf", "ku", "kl", pMdmTerm);
            DBG_PRINT("APP-->Sending Event: HOLDKEY");
            key_sw_event_process("kf", "ku", lineId==1 ? "l001" : "l002", pMdmTerm);
            DBG_PRINT("APP-->Sending Event: LINE");
        }
        else
        {
            DBG_PRINT("inside else\r\n");
            /*
            key_event_process("al", "on", NULL, pMdmTerm);
            key_event_process("kf", "ku", lineId==1 ? "l002" : "l001", pMdmTerm);
            */
        }
    }
    else if (rcState == REMOTE_CONF_SECOND_CALL_TRANS)
    {
        /*step 4 - release second line*/
        remote_conference_process(pMdmTerm);
    }
}

#ifdef CONFIG_RG_ATA

static void remote_conference_onhook_handler(RvMdmTerm *pMdmTerm)
{
    voip_remote_conf_state_t rcState;
    /*  basically you can go onhook in three cases
        1. conference is already on.
        2. during creation of this conference when disconnecting other 2 parties i.e.
           REMOTE_CONF_DISCONNECTING_PARTIES or REMOTE_CONF_THIRD_CALL_MAKE
        3. during creation of a conference all other cases. this means that the creator wishes to
           draw out and therefore it'll be mapped to REMOTE_CONF_IS_ON in order
           to disconnect and release all resources.
    */
    rcState = remote_conference_state_get(pMdmTerm);

    switch (rcState)
    {
    case REMOTE_CONF_FIRST_CALL_TRANS:
    case REMOTE_CONF_SECOND_CALL_TRANS:
        /* in these cases we already sent one or two blind transfer events.
           due to a bug in Rv there is a need to disconnect a call after carrying out
           a blind transfer.
           if we had only one BT then the other call is active an the HW onhook event
           will disconnect this call
           if we had two BT then both calls need to be disconnected. the HW event
           will disconnect the second BT. in both cases we simulate another onhook event
           in order to disconnect the first call that was BT.
        */
        onhook_key_simulate(pMdmTerm);
        remote_conference_state_set(pMdmTerm, REMOTE_CONF_IS_ON);
        remote_conference_process(pMdmTerm);
        break;
    case REMOTE_CONF_IS_ON:
    case REMOTE_CONF_DISCONNECT_FIRST:
    case REMOTE_CONF_DISCONNECT_SECOND:
        /*simply go and destruct all remote conference related stuff*/
        remote_conference_process(pMdmTerm);
        break;
    case REMOTE_CONF_NOT_IN_PROGRESS:
        DBG_PRINT("you shouldnt be here...\r\n");
    }
}

#endif /*CONFIG_RG_ATA*/

#if 0
static void line_info_print(RvMdmTerm *pMdmTerm)
{
    int heldLine = 0;
    RvIppTerminalHandle termHndl = rvIppMdmTerminalGetHandle(pMdmTerm);
    RvIppConnectionHandle connHndl = NULL;

    connHndl = rvIppMdmTerminalGetActiveConnection(termHndl);
    DBG_PRINT("*********************************************************************\r\n");
    DBG_PRINT("*********************************************************************\r\n");
    DBG_PRINT("isRemoteConfInProgress = %d\r\n", isRemoteConfInProgress);
    DBG_PRINT("active_line_Id_get(pMdmTerm) = %d\r\n", active_line_Id_get(pMdmTerm));
    DBG_PRINT("rvIppMdmConnGetLineId(connHndl) = %d\r\n", rvIppMdmConnGetLineId(connHndl));
    DBG_PRINT("rvIppMdmConnGetTermState(connHndl) = %d\r\n", rvIppMdmConnGetTermState(connHndl));
    DBG_PRINT("connection_state_get(pMdmTerm) = %d\r\n", connection_state_get(pMdmTerm));
    DBG_PRINT("rvIppMdmTerminalGetNumActiveConnections(termHndl) = %d\r\n",
        rvIppMdmTerminalGetNumActiveConnections(termHndl));
    DBG_PRINT("rvIppMdmTerminalOtherHeldConnExist(termHndl, connHndl) = %d\r\n",
        rvIppMdmTerminalOtherHeldConnExist(termHndl, connHndl));
    rvIppMdmTerminalGetHeldConn(termHndl, &heldLine);
    DBG_PRINT("heldLine = %d\r\n", heldLine);
    DBG_PRINT("*********************************************************************\r\n");
    DBG_PRINT("*********************************************************************\r\n");
}
#endif
static void media_capabilities_load(void)
{
    RvSdpMsgList sdp_list;
    RvSdpMsg *sdp_msg;

    rvSdpMsgListConstructA(&sdp_list, &rvDefaultAlloc);
    sdp_msg = rvSdpMsgListAddMsg(&sdp_list);

    rvSdpMsgAddSessionTime(sdp_msg, 0, 0);

    rvSdpMsgSetConnection(sdp_msg, RV_SDPNETTYPE_IN, RV_SDPADDRTYPE_IP4,
	call_mngr_params->local_ip);

    /* This is needed for H323 initialization :-( */
    term_call_mngr_cbs->prepare_connection((acl_voip_sdp_msg_t *)sdp_msg, 0);

    /* associate capabilities with classes */
    rvMdmTermClassAddMediaCapabilities(rtp_class, &sdp_list, &sdp_list, NULL);
    rvMdmTermMgrMediaCapabilitiesUpdated(&term_mgr, rtp_class);
#ifdef CONFIG_RG_ATA
    rvMdmTermClassAddMediaCapabilities(al_class, &sdp_list, &sdp_list, NULL);
#endif
#ifdef CONFIG_RG_IPPHONE
    rvMdmTermClassAddMediaCapabilities(at_class, &sdp_list, &sdp_list, NULL);
    rvMdmTermClassAddMediaCapabilities(ui_class, &sdp_list, &sdp_list, NULL);
#endif

    /* 070729 - EA - merge to rv 2.5*/
    rvSdpMsgListConstructCopyA(&g_sdp_list, &sdp_list , &rvDefaultAlloc ); /* ACL NB */

    rvSdpMsgListDestruct(&sdp_list);
}

struct RvMdmTerm_ *sip_select_term_cb(RvMdmTermMgr *mgr, RvMdmTerm *temp_term)
{
	return rvMdmTermMgrRegisterEphemeralTermination(mgr, rtp_class, "rtp", NULL);
}

static RvCCConnection *active_connection_get(RvMdmTerm *term)
{
    RvCCTerminal *xTerm = (RvCCTerminal*)term->xTerm;

    /* This code replaces the call rvCCTerminalGetActiveConnection() that
     * performs lock that can (and does..) lead to deadlock because of our IPC
     * mechanism.
     * It seems that there is a chance that the other thread will delete the
     * connection before modifying the activeConnection just after this thread
     * fetches the activeConnection but before it reads the connection pointer
     * from the array. But the chance of this to happen is so low that it can be
     * ignored.  On the other hand, RV lock doesn't help much in case connection
     * is deleted after its reference has been given to application ..  Anyway,
     * after the implementation of B11959, deadlock problem will not exist, and
     * it will be possible to use RV safer function */
    return xTerm->connections[xTerm->activeConnection];
}

static int active_line_Id_get(RvMdmTerm *term)
{
   /* ACL NB 060612 For debug */
   int lineId;

   lineId = rvCCConnectionGetLineId(active_connection_get(term));

    //DBG_PRINT("lineId %d \r\n",lineId);

    return lineId;
}


static int connection_state_get(RvMdmTerm *term)
{
	RvCCConnection *connection = active_connection_get(term);
	if (connection)
		return rvCCConnectionGetState(connection);
	else
		return 0;
}

static void sip_delete_ephemeral_term_cb(RvMdmTermMgr *mgr, RvMdmTerm *eph_term)
{
   //DBG_PRINT("-> \r\n");
    /* Do nothing */
}

static int term_connect_parameters(RvMdmTerm *source, RvMdmTerm *target,
    acl_analog_line_t **analogLine, acl_sip_call_t **sipCall)
{
    if (rvMdmTermGetType(source) == RV_MDMTERMTYPE_PHYSICAL &&
	rvMdmTermGetType(target) == RV_MDMTERMTYPE_EPHEMERAL)
    {
	*analogLine = rvMdmTermGetUserData(source);
	*sipCall = rvMdmTermGetUserData(target);

    }
    else if (rvMdmTermGetType(source) == RV_MDMTERMTYPE_EPHEMERAL &&
	rvMdmTermGetType(target) == RV_MDMTERMTYPE_PHYSICAL)
    {
	*analogLine = rvMdmTermGetUserData(target);
	*sipCall = rvMdmTermGetUserData(source);

    }
    else
    {
	/* XXX: We don't support both ephemeral (conference) yet.
	 * But user may detect it by realizing that we deal with the same
	 * endpoint */

	// by itzik - 11072006 - 3wc
/*
	*ep = rvMdmTermGetUserData(source);
	*conn = rvMdmTermGetUserData(target);


*/
	*analogLine = NULL;
	*sipCall = rvMdmTermGetUserData(source);

    DBG_PRINT("3WC\r\n");

	return -1;
    }
    return 0;
}

static RvBool sip_connect_cb(RvMdmTermMgr *mgr, RvMdmTerm *source,
    RvMdmMediaStream *m1, RvMdmTerm *target, RvMdmMediaStream *m2,
    RvMdmStreamDirection direction, RvMdmError *mdm_error)
{
    acl_analog_line_t *analogLine;
    acl_sip_call_t *sipCall;
	int ret,n;


    printf("-> [%s %d] \r\n",__FUNCTION__,__LINE__);

    if (term_connect_parameters(source, target, &analogLine, &sipCall))
    {
        /* 3 Way Conference */
        ret = term_call_mngr_cbs->start_3wc(sipCall->analogLine->data) ? rvFalse : rvTrue;
        return ret;
    }

    sipCall->analogLine = analogLine;

    ret = term_call_mngr_cbs->start_connection(analogLine->data, sipCall->data) ? rvFalse : rvTrue;

    if ( ret ==  rvFalse )
       DBG_PRINT_ERR("Warning: sip_connect_cb::start_connection failed \r\n");

    printf("-> [%s %d] \r\n",__FUNCTION__,__LINE__);
    //n=send(g_uiRv_sock_cli_fd,"P2P_CALL",32,0);
    return ret;
}

static RvBool sip_disconnect_cb(struct RvMdmTermMgr_ *mgr,
    struct RvMdmTerm_ *source, RvMdmMediaStream *m1, struct RvMdmTerm_ *target,
    RvMdmMediaStream *m2, RvMdmError *mdm_error)
{
    int ret;
    acl_analog_line_t *analogLine;
    acl_sip_call_t *sipCall;

     printf("-> [%s %d] \r\n",__FUNCTION__,__LINE__);

    if (term_connect_parameters(source, target, &analogLine, &sipCall) || !sipCall)
	   return rvTrue; /* this is not an error. we just ignore it */
     printf("-> [%s %d] \r\n",__FUNCTION__,__LINE__);

	/* AlexChen SDK upgrade 20090414 */
	if ( !analogLine || !sipCall)
    {
       return rvTrue; /* we just ignore it */
    }
    if ( !analogLine->data || !sipCall->data)
    {
       DBG_PRINT("Already disconnected \r\n", __LINE__);
       return rvTrue; /* we just ignore it */
    }
    ret = term_call_mngr_cbs->stop_connection(analogLine->data, sipCall->data);
 send_to_phone_input_cb(1,PHONE_INPUT_HOOK_ON);
     printf("-> [%s %d] \r\n",__FUNCTION__,__LINE__);
    return ret ? rvFalse : rvTrue;
}

static RvBool sip_map_dial_string_to_addr_cb(RvMdmTerm *term, const char *dialstr,
    char *addr)
{

	char sipAddr[AC_MAX_HOST_NAME_LEN];

	int usingProxy=0;
	int sipProxyPort=0;
	int numberInAddrBook=0;
	int numberInNumberRules=0;
    acl_analog_line_t *analogLine = rvMdmTermGetUserData(term);
    char cid[MAX_CALLER_ID_SIZE] = "", dial_str[MAX_CALLER_ID_SIZE];
    //char destAddr[AC_MAX_HOST_NAME_LEN + 1] = "";
    char dest[AC_MAX_HOST_NAME_LEN + 1] = "";

   DBG_PRINT("->\r\n");

	if(strlen(dialstr) >= MAX_CALLER_ID_SIZE )
	{
		DBG_PRINT("\ndialstr string is to long %d\n", strlen(dialstr));
		return rvFalse;
	}

   if (isBlindTrans == 1)
   {
      AC_STRNCPY(dial_str, "*98", MAX_CALLER_ID_SIZE - 1);  // TEMP *98 should be configurable

	if( (sizeof(dial_str) - strlen(dialstr)) > strlen(dialstr))
	  	strcat(dial_str, (char *)dialstr);
	else
	{
		DBG_PRINT("\ndialstr string is to long %d\n", strlen(dialstr));
		return rvFalse;
	}

      DBG_PRINT("[dial_str] = %s\r\n", dial_str);
   }
   else if (REMOTE_CONF_NOT_IN_PROGRESS != remote_conference_state_get(term))
   {
       /*   take the dial string as is. it is created upon initialization an it should not be touched.
            definately do not do the regular mapping you do for regular calls !!!
       */
       AC_STRNCPY(addr, (char *)dialstr, REMOTE_CONF_ADDR_MAX_LEN);
       DBG_PRINT("remote conference addr = %s\r\n", addr);
       /*we should think of a way to avoid exiting the function from here*/
       return rvTrue;
   }
   else
    AC_STRNCPY(dial_str, (char *)dialstr, MAX_CALLER_ID_SIZE - 1);

    dtmf_convert(dial_str);

#if 1//NIRP 070808 VI 56120 incoming FXO call
	DBG_PRINT("---------------> dialstr=%s \r\n", dialstr);
	if (!strcmp("DialingFXO2FXS", dialstr))
	{
		snprintf(addr, RV_SIPCTRL_ADDRESS_SIZE, "%s@%s", call_mngr_params->lines[0].id, call_mngr_params->local_ip);
		DBG_PRINT("---------------> addr=%s \r\n", addr);
		return rvTrue;
	}
#endif

    /* Try translating dial string to an address using our phone book */
    numberInAddrBook = !(term_call_mngr_cbs->dialstring_to_addr(analogLine->data, dial_str, cid, dest));

    if ((!numberInAddrBook))
    {
		numberInNumberRules = !(sip_addr_from_number_rules_get(call_mngr_params->sp.sip.numberRules,dial_str,sipAddr));
		if(!numberInNumberRules)
			usingProxy=1;
	}


	if (usingProxy)
    {
	/* If we failed and we're using a proxy/gatekeeper, try letting them
	 * translate the dial string */
	if (call_mngr_params->protocol==VOIP_PROT_H323 &&
	    call_mngr_params->sp.h323.gk_registry)
	{
	    strncpy(cid, dial_str, sizeof(cid));
	}
	else if (call_mngr_params->protocol==VOIP_PROT_SIP &&
	    call_mngr_params->sp.sip.use_proxy)
	{
	    strncpy(cid, dial_str, sizeof(cid));

		/* AlexChen 20090402 SDK upgrade */
		if(RedundantProxy_RegistrarIsRedundant())
        {
            const char *redundant_proxy_addr = RedundantProxy_getAdrr();
            if(redundant_proxy_addr!=NULL)
                strncpy(dest, redundant_proxy_addr, sizeof(dest));
            else
	            return rvFalse;
        }
        else
	    	strncpy(dest, call_mngr_params->sp.sip.proxy_address, sizeof(dest));
	    	//strncpy(dest, g_cServer_addr, sizeof(dest));
        sipProxyPort = call_mngr_params->sp.sip.proxy_port;
		// by itzik - 20 June 2006, add port support
       	// snprintf(dest,AC_MAX_HOST_NAME_LEN + 1,"%s:%d",dest,call_mngr_params->sp.sip.proxy_port);
	}
	else
	    return rvFalse;
    }

    if (call_mngr_params->protocol==VOIP_PROT_SIP)
    {
		if (usingProxy)
      {
         /* ACL NB  070422 */
         /******************************************************************
          * Patch received from RV by email at 22-Apr-07
          * Incident #129357: Audiocodes Registration inter-op issue
          * Added the conditions  if ((port != RVSIPCTRL_NO_PORT) && (port != RVSIPCTRL_DEFAULT_PORT))
          *******************************************************************/
/*  EA - 071218 - VI#59630 support SIP over TLS */
#ifdef RV_CFLAG_TLS
         if (1 == voip_conf_is_transport_protocol_tls())
         {
             if ((sipProxyPort != RVSIPCTRL_NO_PORT) && (sipProxyPort != RVSIPCTRL_DEFAULT_PORT))             /*ACL NB  070422*/
                 snprintf(addr, RV_SIPCTRL_ADDRESS_SIZE, "sips:%s@%s:%d", cid, dest,sipProxyPort);
             else
                 snprintf(addr, RV_SIPCTRL_ADDRESS_SIZE, "sips:%s@%s", cid, dest);
         }
         else
         {
             if ((sipProxyPort != RVSIPCTRL_NO_PORT) && (sipProxyPort != RVSIPCTRL_DEFAULT_PORT))             /*ACL NB  070422*/
                 snprintf(addr, RV_SIPCTRL_ADDRESS_SIZE, "%s@%s:%d", cid, dest,sipProxyPort);
             else
                 snprintf(addr, RV_SIPCTRL_ADDRESS_SIZE, "%s@%s", cid, dest);
         }
#else
         if ((sipProxyPort != RVSIPCTRL_NO_PORT) && (sipProxyPort != RVSIPCTRL_DEFAULT_PORT))             /*ACL NB  070422*/
			snprintf(addr, RV_SIPCTRL_ADDRESS_SIZE, "%s@%s:%d", cid, dest,sipProxyPort);
         else
            snprintf(addr, RV_SIPCTRL_ADDRESS_SIZE, "%s@%s", cid, dest);
#endif /* RV_CFLAG_TLS */

         //DBG_PRINT("[%s:%d] addr= %s\r\n",__FUNCTION__,__LINE__, addr);
		}
		else
#ifdef RV_CFLAG_TLS
            if (1 == voip_conf_is_transport_protocol_tls())
            {
                if(numberInNumberRules)
                {
                    snprintf(addr, RV_SIPCTRL_ADDRESS_SIZE, "sips:%s", sipAddr);
                }
                else
                    snprintf(addr, RV_SIPCTRL_ADDRESS_SIZE, "sips:%s@%s", cid, dest);
            }
            else
            {
                if(numberInNumberRules)
                {
                    snprintf(addr, RV_SIPCTRL_ADDRESS_SIZE, "%s", sipAddr);
		        }
		        else
			        snprintf(addr, RV_SIPCTRL_ADDRESS_SIZE, "%s@%s", cid, dest);
            }
#else
        if(numberInNumberRules) {
			snprintf(addr, RV_SIPCTRL_ADDRESS_SIZE, "%s", sipAddr);
		}
		else
			snprintf(addr, RV_SIPCTRL_ADDRESS_SIZE, "%s@%s", cid, dest);
#endif /* RV_CFLAG_TLS */

	}
    else if (call_mngr_params->protocol==VOIP_PROT_H323)
    {
	/* In H323, addr points to an array defined to be 64 byte long. Unlike
	 * in SIP, the array definition doesn't use a define for the length.
	 * Therefore we use AC_MAX_HOST_NAME_LEN + 1 in the following snprintf's */
	if (!call_mngr_params->sp.h323.gk_registry)
	    snprintf(addr, AC_MAX_HOST_NAME_LEN + 1, "TA:%s,NAME:%s", dest, cid);
	else
	    snprintf(addr, AC_MAX_HOST_NAME_LEN + 1, "TEL:%s", cid);
    }

    DBG_PRINT("<< addr =%s\r\n",addr);
    return rvTrue;
}

static RvMdmDigitMapMatchType sip_collect_dial_string_cb(RvMdmTerm *term,
    const char *dialstr, unsigned int *timer_dur)
{
	char sipAddr[AC_MAX_HOST_NAME_LEN];
	char *tempDialStr = (char *)dialstr;

    acl_analog_line_t *analogLine = rvMdmTermGetUserData(term);
    voip_dial_match_t r = term_call_mngr_cbs->dialstring_is_complete(analogLine->data,
	(char *)dialstr, (long *)timer_dur);
    int try_proxy = ((call_mngr_params->protocol==VOIP_PROT_SIP &&
	call_mngr_params->sp.sip.use_proxy) ||
	(call_mngr_params->protocol==VOIP_PROT_H323 &&
	call_mngr_params->sp.h323.gk_registry));

    DBG_PRINT(">>\r\n");

    switch (r)
    {
    case MATCH_DIAL_UNAMBIGUOUSMATCH:
	/* XXX need to use an RVCC callback to trigger the LED changes */
	return RV_MDMDIGITMAP_UNAMBIGUOUSMATCH;

    case MATCH_DIAL_MATCH:
	return RV_MDMDIGITMAP_FULLMATCH;

    case MATCH_DIAL_NOMATCH:
	/* When using a proxy/gatekeeper, in case we can't find the dial string
	 * in our phone book, we send it to them */
	if (try_proxy) {
	    return RV_MDMDIGITMAP_UNAMBIGUOUSMATCH;
	}

	if(!(sip_addr_from_number_rules_get(call_mngr_params->sp.sip.numberRules,tempDialStr,sipAddr))) {
	    return RV_MDMDIGITMAP_UNAMBIGUOUSMATCH;
	}

	/* XXX need to use an RVCC callback to trigger the LED changes */
	term_call_mngr_cbs->start_signal(analogLine->data, VOIP_SIGNAL_WARNING_TONE, NULL);
	return RV_MDMDIGITMAP_NOMATCH;

    case MATCH_DIAL_PARTIALMATCH:
	return try_proxy ? RV_MDMDIGITMAP_FULLMATCH :
	    RV_MDMDIGITMAP_PARTIALMATCH;
    }

    return RV_MDMDIGITMAP_NOMATCH;
}

static char *term_param_value_get(RvMdmParameterList *list, char *paramName)
{
    const RvMdmParameterValue* value;

    value = rvMdmParameterListGet2(list, paramName);
    return value ? (char *)rvMdmParameterValueGetValue(value) : "";
}

static voip_signal_t ind_signal_map(RvMdmSignal *s, void **sigdata)
{
    RvMdmParameterList *params;
    voip_siginfo_ind_t *data;
   char* distRingVal;

   DBG_PRINT("-> \r\n");

    if (strcmp(s->id, "is"))
	return VOIP_SIGNAL_UNKNOWN;
    if (!(data = AC_ZALLOC_L(sizeof(voip_siginfo_ind_t))))
	return VOIP_SIGNAL_UNKNOWN;

    /* this casting (RvMdmParameterList *) needs to cancel 'const' type */
    params = (RvMdmParameterList *)rvMdmSignalGetArguments(s);
    data->indid = term_param_value_get(params, "indid");
    data->state_on = strcmp(term_param_value_get(params, "state"), "off");
    distRingVal = (char *)term_param_value_get(params, "distRing"); /* ACL NB 070506 added distinctive ring feature*/
    DBG_PRINT("distRing %d. \r\n",data->distRing);
    data->distRing = dist_ring_type_get(distRingVal);
    *sigdata = data;

    DBG_PRINT("<- \r\n");
    return VOIP_SIGNAL_INDICATOR;
}

/* ACL NB 070506 - added distinctive ring feature */
static voip_signal_t dist_ring_signal_map(RvMdmSignal *s, void **sigdata)
{
   RvMdmParameterList *params;
   voip_siginfo_dist_ring_t *data;
   char* distRingVal;
   voip_signal_t  ret;

   DBG_PRINT("-> \r\n");

   if ( (strcmp(s->pkg, "al") || strcmp(s->id, "ri") ) &&
        (strcmp(s->pkg, "cg") || strcmp(s->id, "cw") )     )
	   return VOIP_SIGNAL_UNKNOWN;
   if (!(data = AC_ZALLOC_L(sizeof(voip_siginfo_ind_t))))
	   return VOIP_SIGNAL_UNKNOWN;


   /* this casting (RvMdmParameterList *) needs to cancel 'const' type */
   params = (RvMdmParameterList *)rvMdmSignalGetArguments(s);

   distRingVal = (char *)term_param_value_get(params, "distRing");

   data->distRing = dist_ring_type_get(distRingVal);
   *sigdata = data;

   if (!strcmp(s->id, "cw"))
      ret =  VOIP_SIGNAL_CALLER_WAITING_TONE;
   else
      ret = VOIP_SIGNAL_RING;

   DBG_PRINT(" <-  ret=%d\r\n", ret );
   return ret;

}
static voip_signal_t rvcc_signal_map(RvMdmSignal *s, void **sigdata)
{
    RvMdmParameterList *params;

    if (!strcmp(s->id, "g_cCallerid"))
    {
	voip_siginfo_cid_t *data;

	if (!(data = AC_ZALLOC_L(sizeof(voip_siginfo_cid_t))))
	    return VOIP_SIGNAL_UNKNOWN;

	/* this casting (RvMdmParameterList *) needs to cancel 'const' type */
	params = (RvMdmParameterList *)rvMdmSignalGetArguments(s);
	data->number = term_param_value_get(params, "number");
	data->name = term_param_value_get(params, "name");
	data->address = term_param_value_get(params, "address");
	*sigdata = data;
	return VOIP_SIGNAL_CALLER_ID;
    }
    else if (!strcmp(s->id, "la"))
    {
	voip_siginfo_la_t *data;

	if (!(data = AC_ZALLOC_L(sizeof(voip_siginfo_la_t))))
	    return VOIP_SIGNAL_UNKNOWN;

	params = (RvMdmParameterList *)rvMdmSignalGetArguments(s);
	data->name = term_param_value_get(params, "LineId");
	sscanf(data->name, "l%3d", &data->line_id);
	*sigdata = data;
	return VOIP_SIGNAL_LINE_ACTIVE;
    }

    return VOIP_SIGNAL_UNKNOWN;
}
/* ACL NB 060720 support DTMF via SIP */
static voip_signal_t dg_signal_map(RvMdmSignal *s, void **sigdata)
{
    RvMdmParameterList *params;
    voip_siginfo_gd_t  *data;
    const char          *strVal;

    if (!(data = AC_ZALLOC_L(sizeof(voip_siginfo_gd_t))))
	return VOIP_SIGNAL_UNKNOWN;

    /* this casting (RvMdmParameterList *) needs to cancel 'const' type */
    params = (RvMdmParameterList *)rvMdmSignalGetArguments(s);
    strVal = term_param_value_get(params, "duration");
    /* convert string to int */
	sscanf(strVal,"%d",&data->duration);
	sscanf(s->id, "d%1d", &data->id);

    //DBG_PRINT("dg_signal_map ,id=%d  duration=%d \r\n", data->id, data->duration  );

    *sigdata = data;

    return VOIP_SIGNAL_DTMF_TONE;
}
/* ACL NB 061123 */
static voip_signal_t msg_signal_map(RvMdmSignal *s, void **sigdata)
{
   RvMdmParameterList *params;
   voip_siginfo_msg_t *data;

   if (strcmp(s->id, "MWI"))
	    return VOIP_SIGNAL_UNKNOWN;
   if (!(data = AC_ZALLOC_L(sizeof(voip_siginfo_msg_t))))
	   return VOIP_SIGNAL_UNKNOWN;

   /* this casting (RvMdmParameterList *) needs to cancel 'const' type */
   params = (RvMdmParameterList *)rvMdmSignalGetArguments(s);

   if ( !strcmp(term_param_value_get(params, "state"), "on") )
      data->state_on = 1;
   else
      data->state_on = 0;

   //DBG_PRINT("msg_signal_map  state=%s data->state_on%= d\r\n",term_param_value_get(params, "state"), data->state_on);

   *sigdata = data;

   return VOIP_SIGNAL_MWI;
}

static voip_signal_t mdm_signal_map(RvMdmSignal *s, void **sigdata)
{
    DBG_PRINT("mdm_signal_map  pkg=%s,id=%s\r\n",s->pkg, s->id  );

    if (!strcmp(s->pkg, "cg"))
    {
	if (!strcmp(s->id, "rt"))
	    return VOIP_SIGNAL_RING_TONE;

	else if (!strcmp(s->id, "dt"))
	    return VOIP_SIGNAL_DIAL_TONE;
	else if (!strcmp(s->id, "bt"))
	    return VOIP_SIGNAL_BUSY_TONE;
	else if (!strcmp(s->id, "wt"))
	    return VOIP_SIGNAL_WARNING_TONE;
   /*ACL NB 060607 - added call waiting signala*/
   else if (!strcmp(s->id, "cw"))
   {
      if (call_mngr_params->sp.sip.callWaitingEnabled) /* ACL NB 070404 */
         /* ACL NB 070809  Added distinctive ringing feature*/
	      return dist_ring_signal_map(s, sigdata);
   }
   else if (!strcmp(s->id, "cr"))
      return VOIP_SIGNAL_CALLER_WAITING_RINGBACK_TONE;

    }
    else if (!strcmp(s->pkg, "al") && !strcmp(s->id, "ri"))
      /* ACL NB 070612  Added distinctive ringing feature*/
	   return dist_ring_signal_map(s, sigdata);
    else if (!strcmp(s->pkg, "ind")) /* Indicator package */
	return ind_signal_map(s, sigdata);
    else if (!strcmp(s->pkg, "rvcc"))
	return rvcc_signal_map(s, sigdata);
/* ACL NB 060719 - support DTMF via SIP */
    else if (!strcmp(s->pkg, "dg"))
	    return dg_signal_map(s, sigdata);
/* ACL NB 061123 Support MWI */
    else if (!strcmp(s->pkg, "msg"))
         return msg_signal_map(s, sigdata);

    return VOIP_SIGNAL_UNKNOWN;
}

//ACL NB 070607
static void offhook_tone_timeout(void *data)
{
    DBG_PRINT("- \r\n");
    term_call_mngr_cbs->stop_signal(data, VOIP_SIGNAL_HOOK_OFF_TONE, NULL);
}


//ACL NB 070606
static void warning_tone_timeout(void *data)
{
    DBG_PRINT("\r\n");
    term_call_mngr_cbs->stop_signal(data, VOIP_SIGNAL_WARNING_TONE, NULL);
    term_call_mngr_cbs->start_signal(data, VOIP_SIGNAL_HOOK_OFF_TONE, NULL);
    /* set timer to stop the offhook tone */
    AC_IPC_TIMER_SET( voip_conf_tone_timeout_get(VOIP_SIGNAL_HOOK_OFF_TONE), offhook_tone_timeout, data);
}

/* Small improvement for Radvision's MDM functionality. If reorder tone is played
 * for more than x2 seconds, we change this tone to VOIP_SIGNAL_HOOK_OFF_TONE */
static voip_signal_t sip_warning_tone_workaround(void *data, int start)
{

   DBG_PRINT(">> start =%d .\r\n", start);

   if (start)
   {
      AC_IPC_TIMER_SET( voip_conf_tone_timeout_get(VOIP_SIGNAL_REORDER_TONE), warning_tone_timeout, data);
      return VOIP_SIGNAL_REORDER_TONE;
   }
   /* If we were instructed to stop the tone before our tone timeout
   * expired, then we're still playing the tone, and we have to stop
   * it. */
   else if (AC_IPC_TIMER_EXISTS(warning_tone_timeout, data))
   {
      DBG_PRINT("- \r\n");
      AC_IPC_TIMER_DEL(warning_tone_timeout, data);
      return VOIP_SIGNAL_REORDER_TONE;
   }

   /* If we were instructed to stop the warning tone after our tone timeout
   * expired, then we're already playing the off-hook warning tone and now we
   * have to stop it. */
   else if (AC_IPC_TIMER_EXISTS(offhook_tone_timeout, data))
   {
      DBG_PRINT("- \r\n");
      AC_IPC_TIMER_DEL(offhook_tone_timeout, data);
      return VOIP_SIGNAL_HOOK_OFF_TONE;
   }

   return VOIP_SIGNAL_REORDER_TONE;

}


static void dial_tone_timeout(void *data)
{
    DBG_PRINT("- \r\n");
    term_call_mngr_cbs->stop_signal(data, VOIP_SIGNAL_DIAL_TONE, NULL);
    term_call_mngr_cbs->start_signal(data, VOIP_SIGNAL_WARNING_TONE, NULL);
    sip_warning_tone_workaround(data,1);
}


/*
#define DIAL_TONE_TIMEOUT 45000 //Nirp 060720 - not used anymore - instead the value is read from acl_conf
                                //by calling voip_conf_dialtone_timeout_get()
*/

/* Small improvement for Radvision's MDM functionality. If dial tone is played
 * for more than x seconds, we change this tone to VOIP_SIGNAL_HOOK_OFF_TONE */
static voip_signal_t sip_off_hook_warning_tone_workaround(void *data, int start)
{
    DBG_PRINT(">> start =%d .\r\n", start);

    if (start)
    {
	AC_IPC_TIMER_SET(/*Nirp 060720*/voip_conf_dialtone_timeout_get(), dial_tone_timeout, data);
	return VOIP_SIGNAL_DIAL_TONE;
    }
    /* If we were instructed to stop the dial tone before our dial tone timeout
     * expired, then we're still playing the dial tone, and we have to stop
     * it. */
    if (AC_IPC_TIMER_EXISTS(dial_tone_timeout, data))
    {
       DBG_PRINT("-- \r\n");
	AC_IPC_TIMER_DEL(dial_tone_timeout, data);
	return VOIP_SIGNAL_DIAL_TONE;

    }
   /* If we were instructed to stop the tone before our tone timeout
   * expired, then we're still playing the tone, and we have to stop
   * it. */
   else if (AC_IPC_TIMER_EXISTS(warning_tone_timeout, data))
   {
       DBG_PRINT("-- \r\n");
      AC_IPC_TIMER_DEL(warning_tone_timeout, data);
      return VOIP_SIGNAL_REORDER_TONE;
   }


    /* If we were instructed to stop the dial tone after our dial tone timeout
     * expired, then we're already playing the off-hook warning tone and now we
     * have to stop it. */
    else if (AC_IPC_TIMER_EXISTS(offhook_tone_timeout, data))
    {
       DBG_PRINT("-- \r\n");
       AC_IPC_TIMER_DEL(offhook_tone_timeout, data);
	return VOIP_SIGNAL_HOOK_OFF_TONE;
    }

    return VOIP_SIGNAL_DIAL_TONE;
}


// ACL NB 0170611
static void  unanswered_call_timeout(void *pMdmTerm)
{
    int             lineId;

    /*EA - 080526 - VI64130 - in order to return to first call (held call) after second
    	call didnt answer you have to press flash twice.
    	we divide into 2 cases. if the unanswered call is the only call simulate onhook to disconnect it
    	if there is another held call switch back to the held call
*/
	if (acl_does_other_held_conn_exist(pMdmTerm))
	{
		lineId = active_line_Id_get(pMdmTerm);

		key_event_process("kf", "ku", "kl", pMdmTerm);
		key_event_process("kf", "ku", lineId==1 ? "l002" : "l001", pMdmTerm);
	}
	else
	{
   		onhook_key_simulate((RvMdmTerm *)pMdmTerm);
	}

	DBG_PRINT("<<\r\n");
}

static void  unanswered_call_type_2_timeout(void *pMdmTerm)
{
	int             lineId;

	if (acl_does_other_held_conn_exist(pMdmTerm))
	{
		lineId = active_line_Id_get(pMdmTerm);

		key_event_process("kf", "ku", "kl", pMdmTerm);
		key_event_process("kf", "ku", lineId==1 ? "l002" : "l001", pMdmTerm);
	}
	else
	{
   		onhook_key_simulate((RvMdmTerm *)pMdmTerm);
	}
	DBG_PRINT("<<\r\n");
}

/* AlexChen SDK upgrade 20090414 */
static void  aclRegretCallTimeout(void *pMdmTerm)
{
   RvMdmTerm *term = (RvMdmTerm *)pMdmTerm;
   RvCCTerminal *xTerm = (RvCCTerminal*)term->xTerm;
   int line;

   if ( (line = line_get_by_term(xTerm)) != -1)
      line_status[line].regretCallSimHookOn=1;

   onhook_key_simulate((RvMdmTerm *)pMdmTerm);
   DBG_PRINT("line=%d \r\n", line);
}

/* ACL AL 081119 VI57702 - 491 occurs when reinvites are sent simultaneously. We will prevent it by using this timer */
/******************************************************************************
*  switchToVoice_timeout_cb
******************************************************************************/
static void switchToVoice_timeout_cb(void *pTerm)
{
	int i, index = 0;
	int mdm_endpoints_index = 0;
	//RvMdmTerm *temp;
    acl_analog_line_t	*endpointUserData;
	acl_sip_call_t		*connUserData;
	RvMdmTerm *term = (RvMdmTerm *)pTerm;
	DBG_PRINT(">>\r\n");

	/*get mdm_endpoints_index for mdm_endpoints[mdm_endpoints_index].line.ata.al*/
	for(i=0 ; i < call_mngr_params->ep_num; i++)
	{
		endpointUserData = (acl_analog_line_t *)rvMdmTermGetUserData(analog_lines[i].line.ata.al);
		connUserData = (acl_sip_call_t *)term;
		DBG_PRINT("i=%d, endpointUserData = %p, connUserData = %p, connUserData-->ep = %p\r\n", i, endpointUserData, connUserData, connUserData->analogLine);
		/*if ((RvMdmTerm *)endpointUserData == term)*/
		if (endpointUserData == connUserData->analogLine)
		{
			mdm_endpoints_index = i;
			DBG_PRINT("mdm_endpoints_index=%d, found term: endpointUserData = %p, term = %p\r\n", mdm_endpoints_index, endpointUserData);
			break;
		}
	}

	/*get index for lastCreateMsg[index].switchToVoice_localSDP*/
	for(i=0 ; i < g_lastCreateMsg_Entries; i++)
	{
		DBG_PRINT("i=%d, lastCreateMsg[i].termUserData = %p, term = %p\r\n", i, (RvMdmTerm *)lastCreateMsg[i].termUserData, term);
		if ((RvMdmTerm *)lastCreateMsg[i].termUserData == term)
		{
			index = i;
			DBG_PRINT("index=%d, found term: term = %p, \r\n", index, term);
			break;
		}
	}

	/*	RvMdmTerm *temp =  mdm_endpoints[index].line.ata.al;*/
	DBG_PRINT("lastCreateMsg index = %d,, mdm_endpoints index = %d, term = %p, mdm_endpoints[index].line.ata.al  = %p,\r\n", index, mdm_endpoints_index, term, analog_lines[mdm_endpoints_index].line.ata.al);
    rvMdmTermModifyMedia( analog_lines[mdm_endpoints_index].line.ata.al, &lastCreateMsg[index].switchToVoice_localSDP);

   lastCreateMsg_delete_switchToVoice_localSDP(index);

	DBG_PRINT("<< \r\n");
}


/* Small improvement for Radvision's MDM functionality. If reorder tone is played
 * for more than x2 seconds, we change this tone to VOIP_SIGNAL_HOOK_OFF_TONE */
static voip_signal_t acl_ringback_tone_workaround(void *data, int start, voip_signal_t sig)
{
    /* EA - 080522 - VI64129 - when key sequence is set to flash + digits - warning tone is heard
    	after returning to first call when second didnt answer.
    	I added these parameters because the warning_tone_timeout and offhook_tone_timeout functions
    	expect an phone_conn_t type input
*/
    	acl_analog_line_t *analogLine = rvMdmTermGetUserData((RvMdmTerm *)data);
	acl_line_t      *line = analogLine->data;

    DBG_PRINT(">> start =%d tone=%d\r\n", start, sig);

   if (start)
   {
      if ( sig == VOIP_SIGNAL_RING_TONE )
         AC_IPC_TIMER_SET( voip_conf_unansweredCall_timeout_get(), unanswered_call_timeout, data);
      else
         AC_IPC_TIMER_SET( voip_conf_unansweredCall_timeout_get(), unanswered_call_type_2_timeout, data);

      return sig;
   }

   /* If we were instructed to stop the tone before our tone timeout
   * expired, then we're still playing the tone, and we have to stop
   * it. */
   if (AC_IPC_TIMER_EXISTS(unanswered_call_timeout, data))
   {
      AC_IPC_TIMER_DEL(unanswered_call_timeout, data);
      return VOIP_SIGNAL_RING_TONE;
   }
   /* If we were instructed to stop the g_cCaller_WAITING_RINGBACK tone before its timeout
   * expired, then we're still playing the tone, and we have to stop
   * it. */
   if (AC_IPC_TIMER_EXISTS(unanswered_call_type_2_timeout, data))
   {
      AC_IPC_TIMER_DEL(unanswered_call_type_2_timeout, data);
      return VOIP_SIGNAL_CALLER_WAITING_RINGBACK_TONE;
   }


    /*  EA - 080522 - VI64129 - when key sequence is set to flash + digits - warning tone is heard
    	after returning to first call when second didnt answer.
    	I added these parameters because the warning_tone_timeout and offhook_tone_timeout functions
    	expect an phone_conn_t type input. because data was passed instead of conn the warning tone timeout
    	wasnt detected and a warning tone was heard on the following case:
    	A calls B. A puts B on hold. A calls C. C doesnt answer. A returns to the call with B.
*/
   else if (AC_IPC_TIMER_EXISTS(warning_tone_timeout, data))
   {
      AC_IPC_TIMER_DEL(warning_tone_timeout, data);
      return VOIP_SIGNAL_REORDER_TONE;
   }
   /* If we were instructed to stop the warning tone after our tone timeout
   * expired, then we're already playing the off-hook warning tone and now we
   * have to stop it. */

    /*  EA - 080522 - VI64129 - when key sequence is set to flash + digits - warning tone is heard
    	after returning to first call when second didnt answer.
    	I added these parameters because the warning_tone_timeout and offhook_tone_timeout functions
    	expect an phone_conn_t type input*/
    else if (AC_IPC_TIMER_EXISTS(offhook_tone_timeout, data))
   {
      AC_IPC_TIMER_DEL(offhook_tone_timeout, data);
      return VOIP_SIGNAL_HOOK_OFF_TONE;
   }

//Patch
	if  ((1 == line->is_off_hook) &&
		(0 == acl_is_during_active_call((RvMdmTerm *)data)))
	{
		DBG_PRINT("inside patch\n");
		term_call_mngr_cbs->stop_signal(analogLine->data, VOIP_SIGNAL_RING_TONE, NULL);
		term_call_mngr_cbs->start_signal(analogLine->data, VOIP_SIGNAL_WARNING_TONE, NULL);
		sip_warning_tone_workaround(analogLine->data, 1);
	}

   return VOIP_SIGNAL_UNKNOWN;

}

static RvBool signal_process(int (*cb)(void *, voip_signal_t, void *),
    RvMdmTerm *term, RvMdmSignal *s)
{
    acl_analog_line_t *analogLine = rvMdmTermGetUserData(term);
    void *sigdata = NULL;
    voip_signal_t sig = mdm_signal_map(s, &sigdata);
    RvBool ret = rvFalse;
    RvCCTerminal *xTerm = (RvCCTerminal*)term->xTerm;/* NIRP 061219 */
    int lineIDFromTerm=line_get_by_term(xTerm);/* NIRP 061219 */
    RvCCConnState state = connection_state_get(term);
    //DBG_PRINT("--> !!!!!!!!!!!  sig = %d.\r\n", sig);
    DBG_PRINT("lineIDFromTerm= %d, sig = %d, connection state = %d .\r\n",lineIDFromTerm, sig, state);
    if (sig == VOIP_SIGNAL_UNKNOWN)
	goto Exit;

    if (ANALOG_PORT_TYPE_FXO == analog_lines[lineIDFromTerm].enum_analog_port_type)//ACL NIRP 071129
    {
    	DBG_PRINT("GOT A SIGNAL on the FXO line -----> sig = %d .\r\n", sig);
		if ((cb == term_call_mngr_cbs->stop_signal) && (VOIP_SIGNAL_RING_TONE == sig) && (RV_CCCONNSTATE_CALL_DELIVERED == state))
		{

			DBG_PRINT("---------------> \r\n");//nirp temp remove

			dsp_send_daa_commands_args_t arg = {
				DSP_DAA_COMMANDS_CHANGE_HOOK_STATE_TO_OFF_HOOK, //command
				lineIDFromTerm,                                 //line
				0                                               //ch
			};

			/* NIRP 0705277 instruct the DAA to initiate Send a command to the DAA */
			ret = term_call_mngr_cbs->send_daa_commands((void*)&arg) ? rvFalse : rvTrue;
			if(ret)
			{
				set_phone_led_to_state(e_LED_FXO, e_GREEN_LED_STATE_ON);
			}
		}
		if ((cb == term_call_mngr_cbs->start_signal) && (VOIP_SIGNAL_WARNING_TONE == sig) && (RV_CCCONNSTATE_DISCONNECTED == state))
		{
			DBG_PRINT("---------------> \r\n");//nirp temp remove

			rvMdmTermProcessEvent(term, "al","on",  NULL, NULL);

			dsp_send_daa_commands_args_t arg = {
				DSP_DAA_COMMANDS_CHANGE_HOOK_STATE_TO_ON_HOOK, //command
				lineIDFromTerm,                                 //line
				0                                               //ch
			};

			/* NIRP 0705277 instruct the DAA to initiate Send a command to the DAA */
			ret = term_call_mngr_cbs->send_daa_commands((void*)&arg) ? rvFalse : rvTrue;
			if(ret)
			{
				set_phone_led_to_state(e_LED_FXO, e_LED_STATE_OFF);
			}
		}
		if ((cb == term_call_mngr_cbs->start_signal) && (VOIP_SIGNAL_RING == sig) && (RV_CCCONNSTATE_OFFERED == state))
		{

			DBG_PRINT("---------------> \r\n");//nirp temp remove

			//simulate off hook to the RV and send off hook command to the FXO
			rvMdmTermProcessEvent(term, "al","of",  NULL, NULL);

			dsp_send_daa_commands_args_t arg = {
				DSP_DAA_COMMANDS_CHANGE_HOOK_STATE_TO_OFF_HOOK, //command
				lineIDFromTerm,                                 //line
				0                                               //ch
			};

			/* NIRP 0705277 instruct the DAA to initiate Send a command to the DAA */
			ret = term_call_mngr_cbs->send_daa_commands((void*)&arg) ? rvFalse : rvTrue;
			if(ret)
			{
				set_phone_led_to_state(e_LED_FXO, e_GREEN_LED_STATE_ON);
			}
		}

		return rvTrue;
	}

    DBG_PRINT("sig = %d \n " ,sig);
    switch (sig)
    {
    case VOIP_SIGNAL_DIAL_TONE:
    {
       //DBG_PRINT("VOIP_SIGNAL_DIAL_TONE \r\n");
        int lineToCheck=line_get_by_term(xTerm);/* NIRP 061219 */

/* ACL NB 070515 */
        /*Incase of automatic dialling without timeout no Dial tone and No ring back should be played */
        if ((call_mngr_params->is_auto_dial) && (!call_mngr_params->auto_dial_timeout))
        {
           DBG_PRINT("Auto_dial (to=%d) no Dial tone \r\n", call_mngr_params->auto_dial_timeout );
           goto Exit;
        }

        /* NIRP 061219 VI 44246 - play Dial tone only after SIP proxy registration succeeds */
        /* NIRP 070329  AC51493 - need to play reorder tone upon network disconnect..... the out of servie tone is not configurable */
	/* AMIL 080206   - check the use_proxy_port_ip_for_registrar & use_registrar, not the Proxy*/
//     if ((-1 != lineToCheck) && (call_mngr_params->sp.sip.use_proxy) && (line_status[lineToCheck].line_registration_state != SIP_REG_REGISTERED))
       if ( (-1 != lineToCheck) &&
			((call_mngr_params->sp.sip.use_proxy_port_ip_for_registrar && call_mngr_params->sp.sip.use_proxy) || (call_mngr_params->sp.sip.use_registrar)) &&
            ((line_status[lineToCheck].line_registration_state == SIP_REG_UNREGISTERED) ||
             (line_status[lineToCheck].line_registration_state == SIP_REG_ERROR)))
	{
            int out_of_service_tone;
            voip_conf_out_of_service_tone_get(&out_of_service_tone);

            DBG_PRINT("out of service on line %d: use_proxy=%d, registration state=%d, out_of_service_tone=%d, sig=%d .\r\n", lineToCheck, call_mngr_params->sp.sip.use_proxy, line_status[lineToCheck].line_registration_state, out_of_service_tone, sig);
	        if (OUT_OF_SERVICE_TONE_NONE == out_of_service_tone)
            {
                //NIRP 070419 in some cases (for example when pulling out the WAN cable while a dial tone
                //is heard, we can get hear during voip task init, and we must stop the dial tone
                term_call_mngr_cbs->stop_signal(analogLine->data, VOIP_SIGNAL_DIAL_TONE, NULL);
                //goto Exit;
                /* No tone should be played or stopped - but timer should be start/stop */
                sig = VOIP_SIGNAL_UNKNOWN;
            }
	        if (OUT_OF_SERVICE_REORDER_TONE == out_of_service_tone)
            {
                sig = VOIP_SIGNAL_REORDER_TONE;
            }
			 /*EA - 081124 - AC68289 - Field request from Ziraatbank; Turkey: they want to use Regular Dial tone; for out of service behavior*/
            else if (OUT_OF_SERVICE_DIAL_TONE == out_of_service_tone)
            {

                sig = VOIP_SIGNAL_DIAL_TONE;
            }

			/* Use the workaround to start/stop timers for reorder tone and howler tone ACL NB 081231 VI69344 */
           	sip_off_hook_warning_tone_workaround(analogLine->data, cb == term_call_mngr_cbs->start_signal);
        }
        else
        {
			/* RADVISION's MDM doesn't implement a off-hook warning tone in case the
			* phone is off-hook for a long period of time. We use a workaround that
			* resolves this by substituting the dial tone with a off-hook warning
			* tone when necessary. */
	    	sig = sip_off_hook_warning_tone_workaround(analogLine->data, cb == term_call_mngr_cbs->start_signal);
    	}
    }
      break;
    case VOIP_SIGNAL_WARNING_TONE:
   /*ACL NB 070606 - support howler tone */
        /* RADVISION's MDM doesn't implement a off-hook warning tone in case the
	     * phone is off-hook for a long period of time. We use a workaround that
	     * resolves this by substituting the warning tone with a off-hook warning
	     * tone when necessary. */
	    sig = sip_warning_tone_workaround(analogLine->data, cb == term_call_mngr_cbs->start_signal);
      break;
    case VOIP_SIGNAL_RING_TONE:
    case VOIP_SIGNAL_CALLER_WAITING_RINGBACK_TONE:
       /*Incase of automatic dialling without timeout no Dial tone and No ring back should be played */
       if ((call_mngr_params->is_auto_dial) && (!call_mngr_params->auto_dial_timeout))
       {
          DBG_PRINT("Auto_dial (to=%d) no Ringback tone \r\n", call_mngr_params->auto_dial_timeout );
          goto Exit;
       }

        /* RADVISION's MDM doesn't implement stop the "ringback"  on onhook */
	    sig = acl_ringback_tone_workaround(term, cb == term_call_mngr_cbs->start_signal, sig);
       break;

    case VOIP_SIGNAL_BUSY_TONE:
    /*ACL NB 070628 - support howler tone */
	    sig = sip_warning_tone_workaround(analogLine->data, cb == term_call_mngr_cbs->start_signal);
       break;


    default:
       break;
   }
	if (VOIP_SIGNAL_UNKNOWN != sig )
    ret = cb(analogLine->data, sig, sigdata) ? rvFalse : rvTrue;
Exit:
    nfree(sigdata);
    return ret;
}

static RvBool sip_start_signal_cb(RvMdmTerm *term, RvMdmSignal *s, RvMdmError *err)
{

    DBG_PRINT("--> .\r\n");

   /* ACL NB 060628 - debug
     RvCCConnState state;

     state = connection_state_get(term);
     DBG_PRINT_ERR("sip_start_signal_cb pkg=%s,id=%s  %s\r\n", s->pkg, s->id, acl_voip_cc_conn_state_text(state) );
   */
    return signal_process(term_call_mngr_cbs->start_signal, term, s);
}

static RvBool sip_stop_signal_cb(RvMdmTerm *term, RvMdmSignal *s, RvMdmError *err)
{
    DBG_PRINT("--> .\r\n");

   /* ACL NB 060628 - debug  */
   /*
     RvCCConnState state;

     state = connection_state_get(term);
     DBG_PRINT_ERR("sip_stop_signal_cb pkg=%s,id=%s  %s\r\n", s->pkg, s->id, acl_voip_cc_conn_state_text(state) );
   */

    return signal_process(term_call_mngr_cbs->stop_signal, term, s);
}

static RvBool sip_play_signal_cb(RvMdmTerm *term, RvMdmSignal *s,
    RvBool report_completion, RvMdmError *mdm_error)
{

    DBG_PRINT("-->\r\n");

    return signal_process(term_call_mngr_cbs->play_signal, term, s);
}

static RvBool sip_line_create_media_cb(RvMdmTerm *term, RvMdmMediaStream *media,
    RvMdmMediaStreamDescr *stream_descr, RvMdmError *err)
{
    /* Here we should have signalled to set DSP parameters for the session, but
     * we rather that sip_connect_cb will imply it, since this sits better with the
     * MGCP implementation */
    return rvTrue;
}

/* ACL NB - 060528 */
/* This callback must be implemented and registered otherwise the voip task crash*/
#ifdef CONFIG_RG_ATA
static void sip_unreg_phys_term_completed_cb(RvMdmTerm* term, RvMdmError* mdmError)
{
}
#endif


static AC_CODE2CODE_MAP mdm_mode[] = {
    { RV_MDMSTREAMMODE_NOTSET, VOIP_CONN_MODE_NOT_SET },
    { RV_MDMSTREAMMODE_INACTIVE, VOIP_CONN_MODE_INACTIVE },
    { RV_MDMSTREAMMODE_SENDONLY, VOIP_CONN_MODE_SEND },
    { RV_MDMSTREAMMODE_RECVONLY, VOIP_CONN_MODE_RECV },
    { RV_MDMSTREAMMODE_SENDRECV, VOIP_CONN_MODE_SEND_RECV },
    { RV_MDMSTREAMMODE_LOOPBACK, VOIP_CONN_MODE_LOOPBACK },
    { -1 , -1},
};

static int create_connection_from_list(void *conn_context, RvSdpMsg *local,
    RvSdpMsgList *remote_list, voip_conn_mode_t mode)
{
    int i, msg_list_size = rvSdpMsgListGetSize(remote_list);

    /* Go over the list of SDP messages in the remote descriptor.
     * Stop after the first one that contains the information */
    for (i = 0; i < msg_list_size; i++)
    {
	if (!term_call_mngr_cbs->create_connection(conn_context, (acl_voip_sdp_msg_t *)local,
	    (acl_voip_sdp_msg_t *)rvSdpMsgListGetElement(remote_list, i), mode, call_mngr_params->dtmf_transport_method )) /* ACL NB 060724 added dtmf_method */
	{
	    return 0;
	}
    }
    return -1;
}

static RvBool sip_create_media_cb(RvMdmTerm *term, RvMdmMediaStream *media,
    RvMdmMediaStreamDescr *stream_descr, RvMdmError *err)
{
    acl_sip_call_t *sipCall;
    void *conn_context;
    RvSdpMsgList *local_list, *remote_list;
    RvSdpMsg *local;
    voip_conn_mode_t mode;
    printf("-> [%s %d] \r\n",__FUNCTION__,__LINE__);

    if (!(sipCall = (acl_sip_call_t *)AC_ZALLOC_L(sizeof(acl_sip_call_t))))
	goto Error;
    local_list = rvMdmMediaStreamDescrGetLocalDescr(stream_descr);
    remote_list = rvMdmMediaStreamDescrGetRemoteDescr(stream_descr);
    mode = AC_CODE_TO_VALUE(mdm_mode, rvMdmMediaStreamDescrGetMode(stream_descr));

    local = rvSdpMsgListGetElement(local_list, 0);
    if (!(conn_context = term_call_mngr_cbs->prepare_connection((acl_voip_sdp_msg_t *)local,
	1)))
    {
	goto Error;
    }

/* ACL NB 060404 */
/* TEMPORARY     */
/*
 *	for outgoing INVITE store its SDP in global variable
 * in order to use DIALOG parameters afterward
 */
 #if 0 // AlexChen SDK upgrade 20090414 */
	rvSdpMsgDestruct( &g_LastCreateMsg );
	rvSdpMsgConstructCopy( &g_LastCreateMsg, rvSdpMsgListGetElement( local_list, 0));
#endif
    if (remote_list)
    {
	/* In SIP - if we get here, ALL capabilities are lost :-( */
/*  EA - 081301 this line would cause 2 "connection" lines in the 200 OK SDP.
    Eventually some platforms would reject the call. */
/*
    rvSdpMsgSetConnection(local, RV_SDPNETTYPE_IN, RV_SDPADDRTYPE_IP4,
	    call_mngr_params->local_ip);
*/
	if (create_connection_from_list(conn_context, local, remote_list, mode))
	    goto Error;
    }

    rvMdmMediaStreamDescrReportLocalDescr(stream_descr);

    sipCall->data = conn_context;
    sipCall->eph = term;
    rvMdmTermSetUserData(term, sipCall);

	/* AlexChen SDK upgrade 20090414 */
	/*ACL AL 081029 - VI67678 LastCreateMsg struct should be per line*/
    if ( lastCreateMsg_add(local_list, term ) < 0)
	    goto Error;



    return rvTrue;
Error:
    /*nfree(conn); ACL NB 060621 */
     printf("-> [%s %d] \r\n",__FUNCTION__,__LINE__);
    return rvFalse;
}
/* ACL JL 60222 MaxBitRate negotiation - update DSP according to the SIP SDP attribute field called T38MaxBitRate */
/************************************************************************
* function:		supported_fax_rate_get
* parameters:	media - media description as described in SIP SDP
*				fax_rate - the new fax rate value (ENUM)
* description:	returns a new fax rate value acording to the callee fax
*				rate (remote fax rate) as the following description
*						call				callee
*						 A-------------------->B
*
*				1. A fax rate <= B fax rate  fax rate = -1 (unchanged)
*				2. A fax rate > B fax rate  fax rate = B fax rate
*
************************************************************************/

static void supported_fax_rate_get( IN acl_voip_sdp_media_descr_t *media ,OUT int *fax_rate )
{

	int rmt_rate = -1;
    int configure_rate = -1;
    acl_voip_sdp_attr_t *attr;
    int attr_num = 0;
    int attrIdx;
	int remote_fax_rate = 0;

    attr_num = acl_voip_sdp_media_descr_attr_num_get(media);

    voip_conf_fax_max_rate_get(&configure_rate);

	for (attrIdx = 0; attrIdx < attr_num; attrIdx++)
	{
		attr = acl_voip_sdp_media_descr_attr_get(media, attrIdx);
		if (attr && !strcmp(acl_voip_sdp_attr_name_get(attr), "T38MaxBitRate"))
		{
			char  *attr_value = acl_voip_sdp_attr_value_get(attr);
			rmt_rate = atoi(attr_value);
			remote_fax_rate = (rmt_rate-2400)/2400;
			DBG_PRINT("configure_rate(%d) remote_fax_rate(%d)\n\r", configure_rate, remote_fax_rate);
			if( configure_rate > remote_fax_rate)
			{
				*fax_rate = remote_fax_rate;
			}else
			{
				*fax_rate = -1;
			}
		}
	}
}

static RvBool sip_modify_media_cb(RvMdmTerm *term, RvMdmMediaStream *media,
    RvMdmMediaStreamDescr *stream_descr, RvMdmError *err)
{
    acl_sip_call_t *sipCall = rvMdmTermGetUserData(term);
    RvSdpMsgList *local_list, *remote_list;
    RvSdpMsg *local;
    voip_conn_mode_t mode;
    RvSdpMsg *sdpMsgRmt;   /* ACL NB 060410 */
    int found = 0;         /* ACL NB 060410 */
    int numOfAttr;
    char *attr;
    int attrIdx;
/* ACL JL 60222 MaxBitRate negotiation - update DSP according to the SIP SDP attribute field called T38MaxBitRate */
    int fax_rate = -1;
	int index;		/* AlexChen 20090414 */

     printf("-> [%s %d] \r\n",__FUNCTION__,__LINE__);
    local_list = rvMdmMediaStreamDescrGetLocalDescr(stream_descr);
    remote_list = rvMdmMediaStreamDescrGetRemoteDescr(stream_descr);
    mode = AC_CODE_TO_VALUE(mdm_mode, rvMdmMediaStreamDescrGetMode(stream_descr));
    local = rvSdpMsgListGetElement(local_list, 0);

    /* In SIP and H.323-fast-start a call may not exist yet. Create a connection
     * if a remote SDP list exists. */
    if (!sipCall->analogLine && remote_list)
    {
       //DBG_PRINT("- \r\n");
	   if (create_connection_from_list(sipCall->data, local, remote_list, mode))
	      return rvFalse;
      /* ACL NB 060410 */
      else
         found = 1;

    }
    /* We are modifying an existing session */
    else
    {
      //DBG_PRINT("- \r\n");
      /* ACL NB 060410 */

      /* Usually, we need to change our behaviour according to the description
      * of the remote peer... */
      if (remote_list)
      {
         int i, msg_list_size = rvSdpMsgListGetSize(remote_list);
		 int media_desc_num;
		 int idx;
		 //running over all SDP messages (usually we have only one)
         for (i = 0; i<msg_list_size && !found; i++)
         {
            //DBG_PRINT("i=%d msg_list_size  %d \r\n", i, msg_list_size);
            //ACL NB 060410
            sdpMsgRmt = rvSdpMsgListGetElement( remote_list, i);

		    media_desc_num = acl_voip_sdp_msg_media_descr_num_get((acl_voip_sdp_msg_t *)sdpMsgRmt);
			//running over all Media description (sometimes we have Audio & Image Media descriptions)
		    for (idx = 0; idx < media_desc_num; idx++)
			{
                 printf("-> [%s %d] \r\n",__FUNCTION__,__LINE__);
				printf("idx =%d  media_desc_num %d \r\n", idx, media_desc_num);
				RvSdpMediaDescr * sdpMedia = rvSdpMsgGetMediaDescr(  sdpMsgRmt, idx);

				if( rvSdpMediaDescrGetMediaType(sdpMedia) == RV_SDPMEDIATYPE_IMAGE )
				{
               const char  *sdpMediaFormat = rvSdpMediaDescrGetFormat(sdpMedia, 0);
					if ( !strcmp(sdpMediaFormat, "t38") )
					{
/* ACL JL 60222 MaxBitRate negotiation - update DSP according to the SIP SDP attribute field called T38MaxBitRate */
                    supported_fax_rate_get(  (acl_voip_sdp_media_descr_t *)sdpMedia ,&fax_rate );
                  // Patch
/* ACL JL 60222 MaxBitRate negotiation - update DSP according to the SIP SDP attribute field called T38MaxBitRate */
                    sip_fax_local_sdp_build(term, rvSdpMsgListGetElement(local_list, 0),fax_rate, 1);
						local = rvSdpMsgListGetElement(&g_sdp_list, 0);
/* ACL JL 60222 MaxBitRate negotiation - update DSP according to the SIP SDP attribute field called T38MaxBitRate */
                    if ( term_call_mngr_cbs->start_fax(sipCall->analogLine->data, (acl_voip_sdp_msg_t *)local,(acl_voip_sdp_msg_t *)sdpMsgRmt, mode,fax_rate))
                  {
							printf("start_fax Failed !!! \r\n");
                     return rvFalse;
                  }
				        printf("- \r\n");

                  found = 1;
						break;
               }
            }
            ////////////////////////////////////////////////////////
            /* ACL NB 060711 */
            else
            {
               printf("-> [%s %d] \r\n",__FUNCTION__,__LINE__);
               /* look for special cases as for attr= gpmd vbd=on */
               numOfAttr = acl_voip_sdp_media_descr_attr_num_get((acl_voip_sdp_media_descr_t *)sdpMedia);
               attr = NULL;

               for ( attrIdx = 0; attrIdx < numOfAttr; attrIdx++)
               {
                  attr = (char *)acl_voip_sdp_media_descr_attr_get((acl_voip_sdp_media_descr_t *)sdpMedia, attrIdx);
                  //DBG_PRINT("Attr name = %s \r\n",acl_voip_sdp_attr_name_get((acl_voip_sdp_attr_t *)attr));
                  if ( attr && !strcmp(acl_voip_sdp_attr_name_get((acl_voip_sdp_attr_t *)attr), "gpmd"))
                  {

                     char   *attr_value=acl_voip_sdp_attr_value_get((acl_voip_sdp_attr_t *)attr);
                     int    bypassCoder;

                     voip_conf_bypassCoder_get(&bypassCoder);
							//DBG_PRINT("Attr val = %s, atoi(Attr val)=%d, bypassCoder=%d .\r\n", attr_value, atoi(attr_value), bypassCoder);
							//DBG_PRINT("[%s:%d] start vbd (fax)  bypassCoder=%d !!! \r\n", __FUNCTION__, __LINE__, bypassCoder);
                     //Nirp 060719 check if we have a match in Attr val (PCMU/PCMA)
                     if (atoi(attr_value) != bypassCoder)
                     {
                        printf("[%s:%d]: No Match !!! Attr val = %s, bypassCoder=%d .\r\n",  __FUNCTION__, __LINE__, attr_value, bypassCoder);
                        return rvFalse;
                     }
                     if ( term_call_mngr_cbs->start_bypass(sipCall->analogLine->data))
                     {
						printf("[%s:%d] start_bypass Failed !!! \r\n", __FUNCTION__, __LINE__);
                        return rvFalse;
                     }
                     // Patch
/* ACL JL 60222 MaxBitRate negotiation - update DSP according to the SIP SDP attribute field called T38MaxBitRate */
                     sip_fax_local_sdp_build(term, rvSdpMsgListGetElement(local_list, 0),-1, 2);
                     local = rvSdpMsgListGetElement(&g_sdp_list, 0);
                     found = 1;
                     break;
                  }
               }
               if ( !found )
               {
					/* AlexChen SDK upgrade 20090414 */
					/* ACL AL 081119 VI57702 - 491 occurs when reinvites are sent simultaneously. We will prevent it by using this timer */
					/* No need to check if mdm_params->is_auto_dial becuse we check if timer exists*/
					printf("going to delete switchToVoice_timeout timer, term = %p\r\n", term);
					index = lastCreateMsg_getByTerm(term);
					if ( index < 0 )
					{
						printf("NO Last msg\n");
						return -1;
					}
					printf("index= %d, term = %p lastCreateMsg[index].termUserData = %p\r\n", index, term, lastCreateMsg[index].termUserData);
				//	DBG_PRINT("conn->data = %p, conn->ep->data = %p \r\n", conn->data, conn->ep->data);

					if (AC_IPC_TIMER_EXISTS(switchToVoice_timeout_cb, (void *)lastCreateMsg[index].termUserData))
					{
						printf("deleted switchToVoice_timeout timer\r\n");
						AC_IPC_TIMER_DEL(switchToVoice_timeout_cb, (void *)lastCreateMsg[index].termUserData);

						lastCreateMsg_delete_switchToVoice_localSDP(index);
					}

					if (!term_call_mngr_cbs->modify_connection(sipCall->data,
					                    (acl_voip_sdp_msg_t *)local,
					                    (acl_voip_sdp_msg_t *)sdpMsgRmt,
																mode,
											idx ))// ACL NB 070627 added media descr index
					{
						printf("- \r\n");
						found = 1;
					}
               }
               /* ACL NB 060808 */
               if ( !found )
               {
						printf("[%s:%d] sip_modify_media_cb  not found for mediae desc num %d !!! \r\n", __FUNCTION__, __LINE__, i);
						// return rvFalse;
					}
				}
			}
         }

      }
      /* ACL NB 060808 in case of empty invite !!!! */
      /* There is no remote description             */
      else
      {
          RvSdpMediaDescr * sdpMedia = rvSdpMsgGetMediaDescr(local, 0);
          int isT38 = 0;

          printf("sip_modify_media_cb: NO SDP\n");

          /*
            071007 - EA - patch for incoming fax on RV2.5.
            On RV2.5 rvCCCallModifyMediaCB first calls rvCCConnectionModifyMedia on the local party
            if connection type == RV_CCCONNTYPE_MDM (which is our case)
            then calls rvCCConnectionModifyMedia on the remote party. this would result such that we would set
            a local audio sdp instead of setting fax sdp.
            so in the case that remote doesnt exists and we are t38 we dont enter the modify_connection callback.
          */
          if( rvSdpMediaDescrGetMediaType(sdpMedia) == RV_SDPMEDIATYPE_IMAGE )
              isT38 = 1;
          /*EA - 071119 - fix sdp for vbd*/
          if ((0 == isT38) && (0 == isVbd(sdpMedia)))
          {
          if ( term_call_mngr_cbs->modify_connection(sipCall->data, (acl_voip_sdp_msg_t *)local, NULL, mode, 0))// ACL NB 070627 added media descr index - in this case N/A
          {
              printf(" Warning: sip_modify_media_cb: modify_connection failed \n");
              return rvFalse;
          }
          }

          acl_voip_sdp_print((char *)local,"sip_modify_media_cb: \n ********* local SDP *********");
          found = 1;
          printf("sip_modify_media_cb: NO SDP \n");
      }
    }

    rvMdmMediaStreamDescrReportLocalDescr(stream_descr);

    if ( !found )
	{
		printf("[%s:%d] sip_modify_media_cb Failed !!! \r\n", __FUNCTION__, __LINE__);
		return rvFalse;
	}

	printf("<-\n\r");
    return rvTrue;
}

static RvBool sip_destroy_media_cb(RvMdmTerm *term, RvMdmMediaStream *media,
    RvMdmError *err)
{
    acl_sip_call_t *sipCall = rvMdmTermGetUserData(term);
    int ret= rvFalse;

    DBG_PRINT("-> \r\n");

	/* AlexChen SDK upgrade 20090414 */
	/*ACL AL 081029 - VI67678 LastCreateMsg struct should be per line*/
    lastCreateMsg_delete(term);

    if ( sipCall )
    {
       ret = term_call_mngr_cbs->destroy_connection(sipCall->data);
       //ACL NB 061010 !!!! free(conn);
    }
    printf("<-\n\r");

    return ret ? rvFalse : rvTrue;
}
               /****************************/
               /* ACL NB - 060404          */
               /* Support FaxRelay (T.38)  */
               /****************************/
static void sip_modify_media_completed_cb(struct RvMdmTerm_*		    term,
											     RvBool						    status,
											     RvMdmMediaDescriptor*		 media,
											     RvMdmMediaStreamDescr*		 streamDescr,
											     RvMdmTermReasonModifyMedia reason)
{
    /* 070729 - EA - merge to rv 2.5*/
    /*RvMdmError mdmError;*/
	//ACL NB 060411
   RvSdpMsgList	 *sdpListLocal = rvMdmMediaStreamDescrGetLocalDescr(streamDescr);
	RvSdpMsgList	 *sdpListRemote = rvMdmMediaStreamDescrGetRemoteDescr(streamDescr);
   RvSdpMsg		    *sdpMsg=NULL;
   RvSdpMediaDescr *sdpMedia;
   const char      *sdpMediaFormat;
   acl_sip_call_t      *sipCall = rvMdmTermGetUserData(term);
   voip_conn_mode_t mode;

   printf("-> \r\n");

   RV_UNUSED_ARG(media);
   /* 070729 - EA - merge to rv 2.5*/
   /*rvMdmErrorConstruct_(&mdmError);*/

   mode = AC_CODE_TO_VALUE(mdm_mode, rvMdmMediaStreamDescrGetMode(streamDescr));

   /******************************************************/
   /* In case of success - should update remote Net Term */
   /* For now we support only modify of mediaType -      */
   /* activating Fax relay !!!                           */
   /******************************************************/
   if ( status == 1 )
   {
      printf("-- \r\n");

	   if( sdpListRemote)
      {
         sdpMsg = rvSdpMsgListGetElement( sdpListRemote, 0);
   		sdpMedia = rvSdpMsgGetMediaDescr(  sdpMsg, 0);

         if ( sdpMedia )
         {
                  printf("-- \r\n");

            /* look for change in media type. (FaxRelay) */
            /* NOTE:For now no support in deactivate     */
            if( rvSdpMediaDescrGetMediaType(sdpMedia) == RV_SDPMEDIATYPE_IMAGE )
            {
               sdpMediaFormat = rvSdpMediaDescrGetFormat(sdpMedia, 0);
               if ( !strcmp(sdpMediaFormat, "t38") )
               {
/* ACL JL 60222 MaxBitRate negotiation - update DSP according to the SIP SDP attribute field called T38MaxBitRate */
                  term_call_mngr_cbs->start_fax(sipCall->analogLine->data,
                                     (acl_voip_sdp_msg_t *)rvSdpMsgListGetElement(sdpListLocal, 0),
                                     (acl_voip_sdp_msg_t *)sdpMsg,
                                     mode,-1);

               }

            }
         }
      }
   }

   /* ACL NB 060816 DBG */
#if 0
   if (sdpListLocal)
   {
       DBG_PRINT("-- \r\n");

       sdpMsg = NULL;
       sdpMsg = rvSdpMsgListGetElement( sdpListLocal, 0);
       if (sdpMsg)
          acl_voip_sdp_print((char *)sdpMsg,"*************** sip_modify_media_completed_cb: \n **********************");
   }
#endif
	printf("sip_modify_media_completed_cb: status=%d, reason=%s \r\n", status, acl_voip_cc_modify_media_reason_text((int)reason));
   printf("<- \r\n");
}

/* ACL NB 060404 */
/****************************************************************************
*
*	Name:		sip_fax_local_sdp_build
*----------------------------------------------------------------------------
*	Abstract: called in order to build re-invite message.
*  New media is taken from additional[MediaParameters[n]] sections.
*----------------------------------------------------------------------------
*	Returns: 	1 in case of error
******************************************************************************/
/* ACL JL 60222 MaxBitRate negotiation - update DSP according to the SIP SDP attribute field called T38MaxBitRate */
static int sip_fax_local_sdp_build(RvMdmTerm *term, RvSdpMsg* sdpMsg,int faxrate, int faxMethod)
{
    RvSdpOrigin*  origin;
    RvSdpMsg*     myLastSdpMsg;
    int           ret = 0;
   acl_voip_sdp_media_descr_t *media;
   int                      bypassCoder;
    RvSdpOrigin*  sdpMsgOrig = NULL;
    RvInt32       verInt;
    char          verStr[48];
   int           maxRate;
   char          *maxRateStr;
	int			index=0;	/* AlexChen SDK upgrade 20090414 */
   AC_CODE2STR_MAP    maxRateCode2str[] = {
        {FAX_MAX_RATE_2_4_KBPS, "2400" , 0, 0},
        {FAX_MAX_RATE_4_8_KBPS, "4800" ,0, 0},
        {FAX_MAX_RATE_7_2_KBPS, "7200" , 0, 0},
        {FAX_MAX_RATE_9_6_KBPS, "9600" , 0, 0},
        {FAX_MAX_RATE_12_KBPS,  "12000" , 0, 0},
        {FAX_MAX_RATE_14_4_KBPS,"14400" , 0, 0},
	    {-1 , 0, 0, 0}
	};

   DBG_PRINT("-> \r\n");

   switch (faxMethod)
   {
      case 1:
         /***********************************/
         /*FAX_MODEM_TRANSFER_METHOD__RELAY */
         /***********************************/
         DBG_PRINT("RELAY(t.38) --\r\n");
        {

            RvSdpMediaDescr         *rvMedia;
            acl_voip_sdp_media_descr_t *acMedia;
            /*EA - 080521 - AC64079 - New feature request for Starhub - T38FaxMaxBuffer should be ... configurable...*/
	    int                     faxMaxBuffer = 0;
	    int                     faxMaxDatagram = 0;

            rvSdpMsgConstruct(sdpMsg);

            /* connection information */
            rvSdpMsgSetConnection(sdpMsg, RV_SDPNETTYPE_IN, RV_SDPADDRTYPE_IP4, "$");

		    /* ACL AL 080317 - Add the session timer to the T.38 SIP OK message*/
			rvSdpMsgAddSessionTime(sdpMsg, 0, 0);

            /* media name and transport address */
            rvSdpMsgAddMediaDescr(sdpMsg, RV_SDPMEDIATYPE_IMAGE, RV_SDP_PORT_ANY, RV_SDPPROTOCOL_UDP_T38);

            rvMedia = rvSdpMsgGetMediaDescr(sdpMsg, 0);
            rvSdpMediaDescrAddFormat(rvMedia, "t38");

            acMedia = (acl_voip_sdp_media_descr_t *)rvMedia;
            acl_voip_sdp_media_descr_attr_add(acMedia, "T38FaxVersion", "0");
            /*EA - 080521 - AC64079 - New feature request for Starhub - T38FaxMaxBuffer should be ... configurable...*/
	    voip_conf_fax_max_buffer_get(&faxMaxBuffer);
	    acl_voip_sdp_media_descr_attr_add(acMedia, "T38FaxMaxBuffer", itoa(faxMaxBuffer));
	    voip_conf_fax_max_datagram_get(&faxMaxDatagram);
	    acl_voip_sdp_media_descr_attr_add(acMedia, "T38FaxMaxDatagram", itoa(faxMaxDatagram));

            acl_voip_sdp_media_descr_attr_add(acMedia, "T38FaxRateManagement", "transferredTCF");
            acl_voip_sdp_media_descr_attr_add(acMedia, "T38FaxUdpEC", "t38UDPRedundancy");
        }
        /*acl_voip_sdp_print((char *)sdpMsg,"acl_voip_buf_to_sdpList: \n ********* t38MediaCap SDP *********");*/

         /* ACL NB 070718 - FaxMaxRate is configurable */
         voip_conf_fax_max_rate_get(&maxRate);
         media = acl_voip_sdp_msg_media_descr_get((acl_voip_sdp_msg_t *)sdpMsg, 0);
/* ACL JL 60222 MaxBitRate negotiation - update DSP according to the SIP SDP attribute field called T38MaxBitRate */
         if(faxrate <0)
         {
            /* ACL NB 070718 - FaxMaxRate is configurable */
            maxRateStr = AC_CODE_TO_STR(maxRateCode2str, maxRate);
         }
        else
        {
            maxRateStr = AC_CODE_TO_STR(maxRateCode2str, faxrate);
        }
         acl_voip_sdp_media_descr_attr_add(media, "T38MaxBitRate", maxRateStr);

      break;

      case 2:
         /***********************************/
         /*FAX_MODEM_TRANSFER_METHOD__BYPASS*/
         /***********************************/
        DBG_PRINT("FAX_MODEM_TRANSFER_METHOD__BYPASS --\r\n");
         rvSdpMsgConstruct(sdpMsg);

        /* connection information */
        rvSdpMsgSetConnection(sdpMsg, RV_SDPNETTYPE_IN, RV_SDPADDRTYPE_IP4, "$");
		/* AlexChen SDK upgrade 20090414 */
		/* ACL AL 080731 - VI 65765: Add the session timer to the VBD SIP OK message*/
		rvSdpMsgAddSessionTime(sdpMsg, 0, 0);
/*
Nirp 060719 when working in VBD fax mode, the fax bypass cocec which is configured by the user is added into the SDP message
*/
        voip_conf_bypassCoder_get(&bypassCoder);
        DBG_PRINT("-> bypassCoder=%d .\r\n", bypassCoder);
        acl_voip_sdp_msg_media_descr_add((acl_voip_sdp_msg_t *)sdpMsg, 0);
        media = acl_voip_sdp_msg_media_descr_get((acl_voip_sdp_msg_t *)sdpMsg, 0);
        acl_voip_sdp_media_descr_payload_add(media, bypassCoder);
	acl_voip_sdp_media_descr_rtp_map_add(media, bypassCoder, AC_RTP_PT_TO_NAME(bypassCoder), AUDIO_CODEC_RATE);
	// ACL NB VI68103 Embratel: change ecan=off
	/* AlexChen SDK upgrade 20090414 */
#if 1
       if (RTP_PAYLOAD_PCMU == bypassCoder)
        {
           /*acl_voip_sdp_media_descr_attr_add(media, "gpmd", "0 vbd=yes;ecan=on");*/
           acl_voip_sdp_media_descr_attr_add(media, "gpmd", "0 vbd=yes");
        }
        else
        {
            /*acl_voip_sdp_media_descr_attr_add(media, "gpmd", "8 vbd=yes;ecan=on");*/
           acl_voip_sdp_media_descr_attr_add(media, "gpmd", "8 vbd=yes");
        }
        acl_voip_sdp_media_descr_attr_add(media, "silenceSupp", "off - - - -");
        acl_voip_sdp_media_descr_attr_add(media, "ecan", "fb off -");
#else //an example if we will need to do it in another format
       if (RTP_PAYLOAD_PCMU == bypassCoder)
        {
           acl_voip_sdp_media_descr_attr_add(media, "gpmd", "0 vbd=yes;ecan=off");
        }
        else
        {
           acl_voip_sdp_media_descr_attr_add(media, "gpmd", "8 vbd=yes;ecan=off");
        }

#endif

        //DBG_PRINT("-> \r\n");sleep(1);
#ifdef ACL_SIP_CALL_MNGR_DEBUG
            acl_voip_sdp_print((char *)sdpMsg,"*************** sip_fax_local_sdp_build: \n **********************");
#endif
      break;
		/* AlexChen SDK upgrade 20090414 */
	  case 3:
        /*ACL XY 081218  support fax fallback*/
       {
             DBG_PRINT("Fax Fallback --\r\n");
             rvSdpMsgConstruct(sdpMsg);

            /* connection information */
            rvSdpMsgSetConnection(sdpMsg, RV_SDPNETTYPE_IN, RV_SDPADDRTYPE_IP4, "$");
            rvSdpMsgAddSessionTime(sdpMsg, 0, 0);

            acl_voip_sdp_msg_media_descr_add((acl_voip_sdp_msg_t *)sdpMsg, 0);
            media = acl_voip_sdp_msg_media_descr_get((acl_voip_sdp_msg_t *)sdpMsg, 0);

            voip_conf_bypassCoder_get(&bypassCoder);


            acl_voip_sdp_media_descr_payload_add(media, bypassCoder);
            acl_voip_sdp_media_descr_rtp_map_add(media, bypassCoder, code2str_ex(rtp_payload_type_t_str, bypassCoder), AUDIO_CODEC_RATE);
       }
       break;

      default:
         DBG_PRINT_ERR("[%s:%d]  incorrect FAX method %d \r\n", __FUNCTION__, __LINE__ );
         return 1;
      break;
   }
   /*
	 *	set SDP parameters from primary INVITE message
	 */


	/************/
	/*set origin*/
	/************/
   /* AlexChen SDK upgrade 20090414 */
   /*ACL AL 081029 - VI67678 lastCreateMsg struct is per line*/
   index = lastCreateMsg_getByTerm(term);
   if ( index < 0 )
   {
	   DBG_PRINT_ERR("NO Last msg\n");
	   return 1;
   }
   myLastSdpMsg = &lastCreateMsg[index].lastCreateMsg;
	origin = rvSdpMsgGetOrigin(myLastSdpMsg);
    //sdpMsgOrig = rvSdpMsgGetOrigin(sdpMsg);

//    if (origin == NULL || sdpMsgOrig ==  NULL )
    if (origin == NULL )
    {
        ret = 1;
        DBG_PRINT_ERR("[%s:%d]  NO origin !!\r\n", __FUNCTION__, __LINE__);
    }
    else
    {
        rvSdpMsgSetSessionName(sdpMsg, "-");//NIRP ACL 071008 added the session name field
        /* 070729 - EA - merge to rv 2.5*/
        rvSdpMsgSetOrigin(sdpMsg,
                          rvSdpOriginGetUsername(origin),
                          rvSdpOriginGetSessionId(origin),
                          rvSdpOriginGetVersion(origin),
                          rvSdpOriginGetNetType(origin),
                          rvSdpOriginGetAddressType(origin),
                          rvSdpOriginGetAddress(origin));

	sdpMsgOrig = rvSdpMsgGetOrigin(sdpMsg);

	/* increment the version number by one and store it in the outgoing message and
	    in the media caps   */
	sscanf(rvSdpOriginGetVersion(origin), "%d", &verInt);
	sprintf(verStr,"%u", verInt + 1);

	if (sdpMsgOrig == NULL )
	{
	    ret = 1;
	    DBG_PRINT_ERR("[%s:%d]  No sdp msg origin !!\r\n", __FUNCTION__, __LINE__);
	}
	else
	{
	    rvSdpOriginSetVersion(sdpMsgOrig, verStr);
	}

	rvSdpOriginSetVersion(origin,verStr);

        /***********************************/
        /*Set connection parameters: address*/
        /***********************************/
		/* AlexChen SDK upgrade 20090414 */
		/*ACL AL 081029 - VI67678 lastCreateMsg struct is per line*/
        RvSdpConnection* c = rvSdpMsgGetConnection( &lastCreateMsg[index].lastCreateMsg);
        RvSdpConnection* connection = rvSdpMsgGetConnection( sdpMsg);
        if(connection != NULL)
        {
            rvSdpConnectionSetAddress(connection, c->iAddress);
        }

        /********************************/
        /*Set media parameters          */
        /********************************/
		/* AlexChen SDK upgrade 20090414 */
		/*ACL AL 081029 - VI67678 lastCreateMsg struct is per line*/
        RvSdpMediaDescr*  m = rvSdpMsgGetMediaDescr( &lastCreateMsg[index].lastCreateMsg, 0);
        RvSdpMediaDescr*  media = rvSdpMsgGetMediaDescr( sdpMsg, 0);
        if(media)
        {
            rvSdpMediaDescrSetPort( media, m->iPort);
            DBG_PRINT("m->media.port %d\r\n",m->iPort);
        }
        else
            DBG_PRINT_ERR("[%s:%d] @@@ No media @@@ \r\n", __FUNCTION__, __LINE__);

    }
    DBG_PRINT("<- \r\n");

    return(ret);
}

#ifdef CONFIG_RG_ATA

/****************************************************************************
*
*	Name:		local_sdp_build
*----------------------------------------------------------------------------
*	Abstract: called in order to build re-invite message.
*  New media is taken from g_sdp_list.
*----------------------------------------------------------------------------
*	Returns: 	1 in case of error
******************************************************************************/
static int local_sdp_build(RvMdmTerm *term, RvSdpMsg* sdpMsg)
{
    RvSdpOrigin*  origin;
    RvSdpMsg*     myLastSdpMsg;
    int           ret = 0;
    RvSdpMsg*     dbgSdpMsg;
    RvSdpOrigin*  sdpMsgOrig = NULL;
	RvInt32       verInt;
	char          verStr[48];
	int			  index=0;	/* AlexChen SDK upgrade 20090414 */
    DBG_PRINT("-> \r\n");

    dbgSdpMsg = rvSdpMsgListGetElement(&g_sdp_list, 0 );
    if (dbgSdpMsg)
    {
        rvSdpMsgConstructCopy( sdpMsg, dbgSdpMsg);
    }
    else
    {
        DBG_PRINT_ERR("[%s:%d]  failed to get sdpMsg for voice\r\n", __FUNCTION__, __LINE__ );
        return 1;
    }

    if (term_call_mngr_cbs->prepare_connection((acl_voip_sdp_msg_t *)sdpMsg, 0 ))
    {
        DBG_PRINT_ERR("Error: switch_2_voice_detect_event_handler line_connection_prepare_cb failed\r\n");
        return( 1 );
    }

    /*
    *	set SDP parameters from primary INVITE message
    */

    /************/
    /*set origin*/
    /************/
	/* AlecChen SDK upgrade 20090414 */
	/*ACL AL 081029 - VI67678 lastCreateMsg struct is per line*/
	index = lastCreateMsg_getByTerm(term);
	if ( index < 0 )
	{
		DBG_PRINT_ERR("NO Last msg\n");
		return 1;
	}
    myLastSdpMsg = &lastCreateMsg[index].lastCreateMsg;
    origin = rvSdpMsgGetOrigin(myLastSdpMsg);

	if (origin == NULL)
   {
      ret = 1;
      DBG_PRINT_ERR("[%s:%d]  NO origin !!\r\n", __FUNCTION__, __LINE__);
   }
   else
   {
        rvSdpMsgSetSessionName(sdpMsg, "-");//NIRP ACL 071008 added the session name field
        /* 070729 - EA - merge to rv 2.5*/
        rvSdpMsgSetOrigin(sdpMsg,
                          rvSdpOriginGetUsername(origin),
                          rvSdpOriginGetSessionId(origin),
                          rvSdpOriginGetVersion(origin),
                          rvSdpOriginGetNetType(origin),
                          rvSdpOriginGetAddressType(origin),
                          rvSdpOriginGetAddress(origin));

   	sdpMsgOrig = rvSdpMsgGetOrigin(sdpMsg);

        /* increment the version number by one and store it in the outgoing message and
        in the media caps   */
        sscanf(rvSdpOriginGetVersion(origin), "%d", &verInt);
        sprintf(verStr,"%u", verInt + 1);

	if (sdpMsgOrig == NULL)
   	{
		DBG_PRINT_ERR("[%s:%d]  No sdp msg origin !!!\r\n", __FUNCTION__, __LINE__);
   	}
	else
	{
	        rvSdpOriginSetVersion(sdpMsgOrig, verStr);
        }

        rvSdpOriginSetVersion(origin,verStr);

        /***********************************/
        /*Set connection parameters: address*/
        /***********************************/
		/* AlecChen SDK upgrade 20090414 */
		/*ACL AL 081029 - VI67678 lastCreateMsg struct is per line*/
        RvSdpConnection* c = rvSdpMsgGetConnection(&lastCreateMsg[index].lastCreateMsg);
        RvSdpConnection* connection = rvSdpMsgGetConnection( sdpMsg);
        if(connection != NULL)
        {
            rvSdpConnectionSetAddress(connection, c->iAddress);
        }

        /********************************/
        /*Set media parameters          */
        /********************************/
		/* AlecChen SDK upgrade 20090414 */
		/*ACL AL 081029 - VI67678 lastCreateMsg struct is per line*/
        RvSdpMediaDescr*  m = rvSdpMsgGetMediaDescr( &lastCreateMsg[index].lastCreateMsg, 0);
        RvSdpMediaDescr*  media = rvSdpMsgGetMediaDescr( sdpMsg, 0);
        if(media)
        {
            rvSdpMediaDescrSetPort( media,  m->iPort );
            DBG_PRINT("m->media.port %d\r\n",m->iPort);
        }
        else
            DBG_PRINT_ERR("[%s:%d] @@@ No media @@@ \r\n", __FUNCTION__, __LINE__);

    }

   DBG_PRINT("<- \r\n");

   return(ret);
}


/* ACL NB 060404 */
/****************************************************************************
*
*	Name:		fax_t38_detect_event_handler
*----------------------------------------------------------------------------
*	Abstract: Request to modify parameters of existing media during a call
*           it will send re-invite message.
*           and also start Fax Relay
*----------------------------------------------------------------------------
*	Input:	term
*----------------------------------------------------------------------------
*  Output:	none
*----------------------------------------------------------------------------
*	Returns: 	1 in case of error
******************************************************************************/
static RvBool fax_t38_detect_event_handler(RvMdmTerm *term)
{
   RvSdpMsg	sdpMsg;
   RvCCTerminal *xTerm;
   int          line = 0;

   DBG_PRINT("-> \r\n");

   if (term == NULL)
   {
      DBG_PRINT_ERR("mdm_ModifyMedia - no termination\r\n");
      return( 1 );
   }

	/************************************************************************/
   /*  ACL NB VI 64841                                                     */
   /*  if CNG detection disabled and re-Invite on remote side in case of   */
   /*  outgoing call we should ignore the fax detection                    */
   /************************************************************************/

   if (voip_conf_remote_fax_side_reinvite_get())
   {
      xTerm  = (RvCCTerminal*)term->xTerm;
      line   = line_get_by_term(xTerm);
      DBG_PRINT("mdm_faxDetect - on line = %d \r\n", line);

      /* Assumption - fax call can be only on active call!!!*/
      if  (line_status[line].active_call_origin == CALL_ORIGIN_OUTGOING )
      {
         DBG_PRINT("mdm_faxDetect - Ignored \r\n");
      	return( 0 );
      }
   }
/*ACL XY 080917 - VI66797: problems with T.38 fax transmissions*/
   /*make sure that the Re-Invite will be sent, only when the 200 OK arrives*/
   if ((connection_state_get(term) == RV_CCCONNSTATE_CONNECTED) &&
        (rvCCConnectionGetMediaState(active_connection_get(term)) != RV_CCMEDIASTATE_MODIFYING))
   {

              /* ACL JL 60222 MaxBitRate negotiation - update DSP according to the SIP SDP attribute field called T38MaxBitRate */
	/*ACL AL 081029 - VI67678 send the line to BuildLocalSdpForFax*/
	DBG_PRINT("mdm_faxDetect call BuildLocalSdpForFax, term = %p\r\n", term);
	sip_fax_local_sdp_build(term, &sdpMsg, -1,1 );

	rvMdmTermModifyMedia( term, &sdpMsg);
	rvSdpMsgDestruct(&sdpMsg);
   }
   else
   {
      acl_analog_line_t *ep;
      acl_line_t     *conn = NULL;

      ep = rvMdmTermGetUserData(term);
      if( ep )
         conn         =(acl_line_t *)ep->data;
      DBG_PRINT("set to 1\r\n");

      acl_line_fax_relay_detected_wait_for_ok_set(conn,1);
   }

	DBG_PRINT("<-\r\n");
  	return( 0 );

#if 0 // AlexChen SDK upgrade 20090408
/* ACL JL 60222 MaxBitRate negotiation - update DSP according to the SIP SDP attribute field called T38MaxBitRate */
 	sip_fax_local_sdp_build( &sdpMsg, -1,1 );

   rvMdmTermModifyMedia( term, &sdpMsg);
	rvSdpMsgDestruct(&sdpMsg);

	DBG_PRINT("<-\r\n");
  	return( 0 );
#endif
}

/******************************/




/* ACL NB 060704 */
/****************************************************************************
*
*	Name:		fax_bypass_detect_event_handler
*----------------------------------------------------------------------------
*	Abstract: Request to modify parameters of existing media during a call
*           it will send re-invite message.
*----------------------------------------------------------------------------
*	Input:	term
*----------------------------------------------------------------------------
*  Output:	none
*----------------------------------------------------------------------------
*	Returns: 	1 in case of error
******************************************************************************/
static RvBool fax_bypass_detect_event_handler(RvMdmTerm *term)
{

	RvSdpMsg	sdpMsg;
	int 		ret;

	DBG_PRINT("-> \r\n");
	if (term == NULL)
	{
		DBG_PRINT_ERR("mdm_ModifyMedia - no termination\r\n");
		return( 1 );
	}

	ret = sip_fax_local_sdp_build(term, &sdpMsg, -1,2 );
	if (ret){
		DBG_PRINT("ERROR:[%s:%d] - BuildLocalSdpForFax failed\r\n", __FUNCTION__, __LINE__);
		return( 1 );
	}

	rvMdmTermModifyMedia( term, &sdpMsg);

	rvSdpMsgDestruct(&sdpMsg);

	DBG_PRINT("<- \r\n");
	return( 0 );
}
/* ACL NB 070806 */
/****************************************************************************
*
*	Name:		switch_2_voice_detect_event_handler
*----------------------------------------------------------------------------
*	Abstract: Request to modify parameters of existing media during a call
*           it will send re-invite message.
*           (when finish t.38 session)
*----------------------------------------------------------------------------
*	Input:	term
*----------------------------------------------------------------------------
*  Output:	none
*----------------------------------------------------------------------------
*	Returns: 	1 in case of error
******************************************************************************/
static RvBool switch_2_voice_detect_event_handler(RvMdmTerm *term)
{
	RvSdpMsg	      local;
	int            ret, index;
	unsigned int switchToVoice_timeout;
	int 				firstNumberInRange = 500;
	int 				secondNumberInRange = 6000;

	DBG_PRINT(">> \r\n");

	srand((unsigned int)clock());

	if (term == NULL)
	{
		DBG_PRINT("mdm_ModifyMedia - no termination\r\n");
		return( 1 );
	}

	/*ACL AL 081029 - VI67678 find and send the term to BuildLocalSdp*/
	DBG_PRINT("mdm_Switch2VoiceDetect call BuildLocalSdp, term = %p\r\n", term);
	ret = local_sdp_build( term, &local );
	if (ret)
	{
		DBG_PRINT("ERROR:[%s:%d] - BuildLocalSdp failed\r\n", __FUNCTION__, __LINE__);
		return( 1 );
	}

	/* ACL AL 081119 VI57702 - 491 occurs when reinvites are sent simultaneously. We will prevent it by using a timer */
	DBG_PRINT("term = %p>> \r\n", term);
	index = lastCreateMsg_getByTerm(term);
	if ( index < 0 )
	{
		DBG_PRINT_ERR("NO Last msg\n");
		return 1;
	}

	rvSdpMsgConstructCopy( &lastCreateMsg[index].switchToVoice_localSDP, &local);

	switchToVoice_timeout = firstNumberInRange + (rand() % (secondNumberInRange-firstNumberInRange+1));

	DBG_PRINT("lastCreateMsg[index].termUserData = %p timeout = %d >> \r\n", lastCreateMsg[index].termUserData->data,switchToVoice_timeout );
	AC_IPC_TIMER_SET( switchToVoice_timeout, switchToVoice_timeout_cb, (void *)lastCreateMsg[index].termUserData);

	rvSdpMsgDestruct(&local); // ???

	DBG_PRINT("<< \r\n");
	return( 0 );
#if 0
    RvSdpMsg	      local;
    acl_analog_line_t *analogLine;

    DBG_PRINT(">> \r\n");

    analogLine = rvMdmTermGetUserData(term);

   if (term == NULL)
   {
      DBG_PRINT_ERR("mdm_ModifyMedia - no termination\r\n");
      return( 1 );
   }

    /* stop fax on dsp channel (Relevant both to T38 and VBD)*/
    term_call_mngr_cbs->stop_fax(analogLine->data);

    local_sdp_build( &local );
    rvMdmTermModifyMedia( term, &local);
    rvSdpMsgDestruct(&local);
    DBG_PRINT("<< \r\n");

    return( 0 );
#endif
}

/******************************/

#endif /*CONFIG_RG_ATA*/

/* ER06 - Find the line number associated with a RV terminal */
/* Note: This method will not work with every RvCCTerminal type. */
static int line_get_by_term(RvCCTerminal *term)
{
	acl_analog_line_t *analogLine;
    RvMdmTerm *t;

	if(!term || !(RvCCTerminalMdm *)term->terminal)
	{
		DBG_PRINT_ERR("line_get_by_term - invalid terminal.");
		return -1;
	}

	t = &(((RvCCTerminalMdm *)term->terminal)->mdmTerm);
	if(t && (analogLine = rvMdmTermGetUserData(t)) )
		return analogLine - analog_lines; /* ER06 - Use pointer arithmetics ... (Ugly) */

	DBG_PRINT_ERR("line_get_by_term - line not found.");
	return -1;
}

static void sip_rvcc_display(RvIppConnectionHandle _conn, RvIppTerminalHandle
    _term, RvCCTerminalEvent event, RvCCEventCause cause, void *data)
{
	int line;
    RvCCConnection *conn = (RvCCConnection *)_conn;
    RvCCTerminal *term = (RvCCTerminal *)_term;

#if defined(CONFIG_RG_IPPHONE)
    RvMdmTermPresentationInfo* presentationInfo = rvCCTerminalMdmGetPresentationInfo(term);
#endif

    printf("[%s %d]",__FUNCTION__,__LINE__);
    RvCCConnState state = rvCCConnectionGetState(conn);
    voip_display_state_t display_state = VOIP_DISPLAY_INVALID;

/* ACL NB - 0600627  call waiting only if the active call state is connected !!*/
    RvCCConnState active_call_state;

#if defined(CONFIG_RG_IPPHONE)
	/*ACL NirS 091201: additional display adaptations */
    void *pInfo = NULL;
    acl_line_t *pLine=get_line(0);
    int call_num= rvIppMdmConnGetLineId(_conn);
    int active_call_num = rvCCTerminalGetActiveConnectionId(term)+1;
    CallInfoData myCallInfoData;
    memset(&myCallInfoData, 0, sizeof(myCallInfoData));
    strcpy(myCallInfoData.myLineId, rvCCTerminalMdmGetTermId(term));
    rvMdmTermPresentationInfoGetName(presentationInfo, myCallInfoData.myLineDescription,
												sizeof(myCallInfoData.myLineDescription));
    void *context = ((acl_analog_line_t *)rvMdmTermGetUserData(
	    &(((RvCCTerminalMdm *)term->terminal)->mdmTerm)))->data;

    if (pLine!=NULL)
    {

		if (pLine->active_call!=NULL)
		{
			myCallInfoData.numOfActiveCalls++;
			myCallInfoData.isHDcall = (pLine->active_call->active_codec==RTP_PAYLOAD_G722);
		}
		if (pLine->active_call1!=NULL)
		{
			myCallInfoData.numOfActiveCalls++;
		}
    }

     myCallInfoData.callIndex=call_num-1;

    if (event==RV_CCTERMEVENT_UNKNOWN)
    {
    	/*Ignore unknown events*/
	return;
    }
#else
    char *extra_info = NULL;
#endif

    switch (state)
    {
    case RV_CCCONNSTATE_IDLE:
    	display_state = VOIP_DISPLAY_IDLE;
#if !defined(CONFIG_RG_IPPHONE)
    	extra_info = term->funcs->getIdF(term);
#else
    	/*send call end display followed be the needed display (if needed) */
    	strcpy(myCallInfoData.otherPartyDescription,get_call_description(call_num) );
    	strcpy(myCallInfoData.otherPartyId,get_call_number(call_num) );
		set_call_info(call_num,"",""); /*reset*/
		myCallInfoData.display_state = VOIP_DISPLAY_CALL_END;
    	pInfo = (void*)&myCallInfoData;
    	/*send call end*/
    	term_call_mngr_cbs->update_display(context, VOIP_DISPLAY_CALL_END, pInfo);
    	if (pLine)
    	{
    		if (pLine->active_call!=NULL)
    		{
    			/*if there is active call convert the idle display to connected display */
    			display_state = VOIP_DISPLAY_CALL_IN_PROGRESS;
    		}
    		else
    		{
    			/*if there is no active call ignore disconnection of inactive calls
    			    (disconnection of inactive call create idle state )*/
    			    if (call_num != active_call_num )
        				if (event==RV_CCTERMEVENT_REMOTE_DISCONNECTED)
        				{
        					return;
        				}

    				/* else send idle  display*/
    		}

    	}
#endif
        /* ER06 - Store the call origin in the line status array */
        if ( (line = line_get_by_term(term)) != -1)
        {
           line_status[line].active_call_origin = CALL_ORIGIN_UNKNOWN;
    		/* ACL 060912 support special key sequence for 012   */
    		/* (for now we need to know only if it is out going) */
    		void *context = ((acl_analog_line_t *)rvMdmTermGetUserData(&(((RvCCTerminalMdm *)term->terminal)->mdmTerm)))->data;

    		term_call_mngr_cbs->set_call_origin(context, CALL_ORIGIN_UNKNOWN );
        }
    	break;

    case RV_CCCONNSTATE_INITIATED:
    	if (event == RV_CCTERMEVENT_OFFHOOK)
    	{
#if defined(CONFIG_RG_IPPHONE)
    	    display_state = VOIP_DISPLAY_OFFHOOK;
    	    strcpy(myCallInfoData.otherPartyId, (char *)rvCCTerminalMdmGetDialString(term));
#else
			display_state = VOIP_DISPLAY_IDLE;
			extra_info = term->funcs->getIdF(term);
#endif
    	}
    	break;

    case RV_CCCONNSTATE_DIALING:
    	if (event == RV_CCTERMEVENT_DIGITS)
    	{
    	    display_state = VOIP_DISPLAY_DIALING;
#if defined(CONFIG_RG_IPPHONE)
    	    strcpy(myCallInfoData.otherPartyId, (char *)rvCCTerminalMdmGetDialString(term));
#else
	    	extra_info = (char *)rvCCTerminalGetDialString(term);
#endif
            //ACL NB 061017 - relocated - only when digit is pressed
    		/* ACL 060912 support special key sequence for 012   */
    		/* (for now we need to know only if it is out going) */
    		void *context = ((acl_analog_line_t *)rvMdmTermGetUserData(&(((RvCCTerminalMdm *)term->terminal)->mdmTerm)))->data;

    		term_call_mngr_cbs->set_call_origin(context, CALL_ORIGIN_OUTGOING );

        }

        /* ER06 - Store the call origin in the line status array */
        if ( (line = line_get_by_term(term)) != -1)
    	{
    		line_status[line].active_call_origin = CALL_ORIGIN_OUTGOING;

    	}
    	break;

    case RV_CCCONNSTATE_CALL_DELIVERED:
        if (event == RV_CCTERMEVENT_RINGBACK)
        {
#if defined(CONFIG_RG_IPPHONE)
			char RemoteId[128]="";
			RvCCConnMdm* mdmConn = rvCCConnMdmGetIns((RvCCConnection*)conn);
			display_state = VOIP_DISPLAY_RINGING_PARTY;
			strncpy(RemoteId, rvStringGetData(&mdmConn->dialString), sizeof(RemoteId));
			RemoteId[sizeof(RemoteId)-1] = '\0';
			strcpy(myCallInfoData.otherPartyId, RemoteId);

			set_call_info(call_num,RemoteId,NULL);
#else
	    	display_state = VOIP_DISPLAY_RINGING_PARTY;
#endif
        }
	break;

    case RV_CCCONNSTATE_CONNECTED:
	switch (event){
		case RV_CCTERMEVENT_CALLANSWERED:
		case RV_CCTERMEVENT_UNHOLD:
			display_state = VOIP_DISPLAY_CALL_IN_PROGRESS;
			break;
		case RV_CCTERMEVENT_HOLD:
			display_state=VOIP_DISPLAY_HOLD;
			break;
		case RV_CCTERMEVENT_TRANSFER:
			display_state = VOIP_DISPLAY_TRANSFER;
			break;
		case RV_CCTERMEVENT_CONFERENCE:
			display_state = VOIP_DISPLAY_CONFERENCE;
			break;
		default:
			return;
		}  /*switch(event)*/
	break;

    case RV_CCCONNSTATE_DISCONNECTED:
    	if (event == RV_CCTERMEVENT_REMOTE_DISCONNECTED)
    	    display_state = VOIP_DISPLAY_PARTY_HUNG_UP;
	    break;

    case RV_CCCONNSTATE_FAILED:
    	if (event == RV_CCTERMEVENT_DIALCOMPLETED)
		{
			display_state = VOIP_DISPLAY_WRONG_NUMBER;
		}
		break;

    case RV_CCCONNSTATE_ALERTING:

/* ACL NB - 0600627  call waiting only if the active call state is connected !!*/
       active_call_state = connection_state_get(&(((RvCCTerminalMdm *)term->terminal)->mdmTerm));

    	// ER06 - Handles call-waiting g_cCaller-id as well
    	if (event == RV_CCTERMEVENT_RINGING &&
    	    (cause == RV_CCCAUSE_INCOMING_CALL ||
           ( cause == RV_CCCAUSE_CALL_WAITING && active_call_state == RV_CCCONNSTATE_CONNECTED)) )

    	{
    	    RvCCConnection *party = rvCCConnectionGetConnectParty(conn);

           /* ACL NB 060629 - Fixed bug in case that g_cCaller id is NULL  */
           const char*	 g_cCallerId  =NULL;
           const char*	 g_cCallerAddr=NULL;
           const char*	 g_cCallerName=NULL;

    	    display_state = VOIP_DISPLAY_INCOMING_CALL;
           /* ACL NB 060629 - Fixed bug in case that g_cCaller id is NULL  */
           g_cCallerId   =  rvCCConnectionGetCallerId(party);
           if (g_cCallerId == NULL)
              g_cCallerId = "Private";
           g_cCallerAddr =  rvCCConnectionGetCallerAddress(party);
           if (g_cCallerAddr == NULL)
              g_cCallerAddr = "";
           g_cCallerName = rvCCConnectionGetCallerName(party);
           if (g_cCallerName == NULL )
             g_cCallerName = "No Name";
    	    /* rvCCConnectionGetg_cCallerName() returns g_cCaller ID, whereas for SIP
    	     * rvCCConnectionGetg_cCallerId() returns the display name */
           /* ACL NB 060629
    	    extra_info = (char *)(call_mngr_params->protocol==VOIP_PROT_SIP ?
    		rvCCConnectionGetg_cCallerId(party) :
    		rvCCConnectionGetg_cCallerName(party));
           */

#if !defined(CONFIG_RG_IPPHONE)
       		extra_info = (char *)g_cCallerId ;
#endif
    	    /* raise an rvcc/g_cCallerid signal */
    	   rvCCTerminalSendCallerId(term, g_cCallerId, g_cCallerName ,g_cCallerAddr, "");

    	   /* ER06 - Store the call origin in the line status array */
    	   if ( (line = line_get_by_term(term)) != -1)
    		   line_status[line].active_call_origin = CALL_ORIGIN_INCOMING;

#ifdef CONFIG_RG_IPPHONE
			set_call_info(call_num,g_cCallerName, g_cCallerId);
#endif
    	}
    	break;

    default:
	break;
    }

#if defined(CONFIG_RG_IPPHONE)
   switch (display_state)
   	/*update the call information and data*/
   {
   	case VOIP_DISPLAY_CALL_IN_PROGRESS:
		strcpy(myCallInfoData.otherPartyDescription,get_call_description(active_call_num) );
	 	strcpy(myCallInfoData.otherPartyId,get_call_number(active_call_num) );
		break;
	case VOIP_DISPLAY_WRONG_NUMBER:
	case VOIP_DISPLAY_PARTY_HUNG_UP:
	case VOIP_DISPLAY_BUSY:
	case VOIP_DISPLAY_INCOMING_CALL:
	case VOIP_DISPLAY_HOLD:
		strcpy(myCallInfoData.otherPartyDescription,get_call_description(call_num) );
	 	strcpy(myCallInfoData.otherPartyId,get_call_number(call_num) );
		break;

	case VOIP_DISPLAY_CONFERENCE:
		myCallInfoData.isHDcall &= (pLine->active_call1->active_codec == RTP_PAYLOAD_G722);
		/*this implemenatation is good only for 2 lines architecture!
		the conference is always between line1 and line2
		in N lines support we need to know which lines are active in the conference*/
		strcpy(myCallInfoData.myLineDescription,get_call_description(1)  );
		strcpy(myCallInfoData.myLineId,get_call_number(1)  );
		strcpy(myCallInfoData.otherPartyDescription,get_call_description(2)  );
		strcpy(myCallInfoData.otherPartyId,get_call_number(2)  );
		break;
	default:
		break;

   }
#endif

    /* If display should be updated... */
    if (display_state != VOIP_DISPLAY_INVALID)
    {
#if defined(CONFIG_RG_IPPHONE)
printf("\ndisplay_state = %d  myLineId = %s  myLineDescription=%s  otherPartyId=%s  otherPartyDescription=%s  numOfActiveCalls=%d line no %d\n",
			display_state, myCallInfoData.myLineId, myCallInfoData.myLineDescription,
			myCallInfoData.otherPartyId, myCallInfoData.otherPartyDescription, myCallInfoData.numOfActiveCalls,__LINE__);

		myCallInfoData.display_state = display_state;
		pInfo = (void*)&myCallInfoData;

        if(myCallInfoData.display_state == VOIP_DISPLAY_CALL_IN_PROGRESS && myCallInfoData.otherPartyId !=0)
        {
            int n;
            strcpy(remote_string,buffer);
            strcat(remote_string,myCallInfoData.otherPartyId);
            printf("Remote string is %s \n",remote_string);
            n=send(g_uiRv_sock_cli_fd,remote_string,32,0);
        }

		term_call_mngr_cbs->update_display(context, display_state, pInfo);

#else
		void *context = ((acl_analog_line_t *)rvMdmTermGetUserData(
	    	&(((RvCCTerminalMdm *)term->terminal)->mdmTerm)))->data;

		term_call_mngr_cbs->update_display(context, display_state, extra_info);
#endif
    }
}

/* ACL NB 070102 - simulate onHook key for flash only key sequence style      */
/* This is used to simulate hook on after transer that was invoked by hook on */
/* It is relevant only to Flash only key sequence style                       */
/******************************************************************************
*  onhook_key_simulate
*  ----------------------------
*
*  Return Value:    None.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          RvMdmTerm*          pointer to mdm Term.
*
*
*  Output          None.
*
******************************************************************************/
static void onhook_key_simulate(RvMdmTerm  *pMdmTerm)
{

	/* If onhook pressed and active line is not default line, before going
	 * on hook we return to the default line, so it will close properly */
   DBG_PRINT(">>============  send Hook ON\r\n");
#if 0 /* ACL NB 070628 VI 54474 */
   if (active_line_Id_get(pMdmTerm) == 2)
   {
       key_event_process("kf", "ku", "l001", pMdmTerm);
   }
#endif
   key_event_process("al", "on", NULL, pMdmTerm);


   printf("<<============  send Hook ON\r\n");

   return;
}

/* ACL NB 070514-     */
/******************************************************************************
*  sip_make_call_cb
*
******************************************************************************/
static void sip_make_call_cb(void *term)
{
   RvIppTerminalHandle termHndl = (RvIppTerminalHandle)term;

   DBG_PRINT(">> To:%s\r\n",call_mngr_params->auto_dial_dest);

   if (call_mngr_params->is_auto_dial)
      acl_make_call(termHndl,call_mngr_params->auto_dial_dest);

}

#ifdef CONFIG_RG_ATA

/* ACL NIRP 070610 - the FXS polarity reversal mechanism is using this function before turning off */
/* the polarity reversal when the last (single) call is disconnected */
/***************************************************************************************************/
static RvBool is_single_active_call(int LineID, int ActiveConnId, RvBool boolOtherHeldCallsExist)
{
    if (((LineID == ActiveConnId) && (boolOtherHeldCallsExist)) || (LineID != ActiveConnId))
        return rvFalse;
    else
        return rvTrue;
}
#endif  /* CONFIG_RG_ATA */

/* ACL NB 061115 - Implementation of the (RvIppMdmExtClbks) postProcessEvent callback */
/* This callback is used to preform hook on after transer that was invoked by hook on */
/* It is relevant only to Flash only key sequence style                               */
/******************************************************************************
*  RvIppMdmPostProcessEventCB
*  ----------------------------
*  General :        Prototype for PostProcessEvent CallBack.
*
*  Return Value:    None.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          connHndl           connection handle.
*                  eventId            Terminal event ID
*                  reason             event reason
*
*
*  Output          None.
*
******************************************************************************/
static void sip_post_process_event(RvIppConnectionHandle connHndl,
										   RvCCTerminalEvent	    eventId,
										   RvCCEventCause        reason)
{
#ifdef CONFIG_RG_ATA


   RvCCConnection  *rvConn = (RvCCConnection *)connHndl;
   RvCCTerminal*    RVccterm = rvCCConnectionGetTerminal(rvConn);//NIRP 070610 VI 54097 needed for rvCCTerminalMdmOtherHeldConnExist
   RvCCConnState   state;
   RvCCCall        *pRvCall;
   RvMdmTerm       *mdmTerm;
   RvCCTerminalMdm *pTerm;
   int              lineId=0, ActiveConnID;
   RvBool           boolOtherHeldCallsExist, boolIsSingleActiveCall;
   acl_analog_line_t   *analogLine;
   acl_line_t      *line;
   char             IsPolarityReversalEnabled;

   DBG_PRINT(">>\r\n");

    voip_conf_fxs_polarity_revasal_enable_get(&IsPolarityReversalEnabled);
    DBG_PRINT("============  Received Event(id-%d) : %s \r\n", eventId, acl_voip_cc_event_text((int)eventId));
    state = rvCCConnectionGetState(rvConn); //nirp temp remove
    lineId = rvCCConnectionGetLineId(rvConn)-1;
    DBG_PRINT("============  State %s  line %d \r\n", acl_voip_cc_conn_state_text(state), lineId );//nirp temp remove
    pRvCall = rvCCConnectionGetCall(rvConn);
    state = rvCCConnectionGetState(rvConn);
    boolOtherHeldCallsExist = acCCTerminalMdmOtherHeldConnExist(RVccterm, rvConn);
    pTerm     = rvCCConnMdmGetTerminalImpl(rvConn);
    mdmTerm   = &(pTerm->mdmTerm);
    analogLine = rvMdmTermGetUserData(mdmTerm);
   line         =(acl_line_t*)analogLine->data;
   RvIppTerminalHandle termHndl = rvIppMdmConnGetTerminal(connHndl);

#ifdef CONFIG_RG_IPPHONE
    RvMdmTermPresentationInfo* presentationInfo = rvCCTerminalMdmGetPresentationInfo(RVccterm);

    CallInfoData myCallInfoData;

    memset(&myCallInfoData, 0, sizeof(myCallInfoData));

    strcpy(myCallInfoData.myLineId, rvCCTerminalMdmGetTermId(RVccterm));

    rvMdmTermPresentationInfoGetName(presentationInfo, myCallInfoData.myLineDescription,
												sizeof(myCallInfoData.myLineDescription));
#endif

   DBG_PRINT("Received Event(id-%d):%s, line->num=%d, \r\n", eventId, acl_voip_cc_event_text((int)eventId), line->num );
   switch(eventId)
   {
		/* AlexChen SDK upgrade 20090414 */
		case RV_CCTERMEVENT_DISCONNECTED:
			DBG_PRINT("[%s:%d]. \r\n", __FUNCTION__, __LINE__);
			rvIppMdmConnSetUserData(connHndl,(void *)"");//Clean the user data
			break;

        case  RV_CCTERMEVENT_CALLANSWERED://NIRP 070610 VI 54097
                if ((RV_CCCONNSTATE_CONNECTED == state) && (IsPolarityReversalEnabled))
         {
            dsp_adaptation_layer_general_command_args_t arg;

            arg.command      = DSP_ADAPTATION_LAYER_GENERAL_COMMAND_SET_FXS_POLARITY_REVERSAL;
            arg.TurnPolRevOn = 1;
            arg.line         = line->num;
            DBG_PRINT("send_general_command  SET_FXS_POLARITY_REVERSAL on\r\n");
            term_call_mngr_cbs->adaptation_layer_send_general_command((void*)&arg);
         }
         break;


      case  RV_CCTERMEVENT_ONHOOK:
            if (REMOTE_CONF_NOT_IN_PROGRESS != remote_conference_state_get(mdmTerm))
            {
               // remote_conference_onhook_handler(mdmTerm);
            }
        /*-------  Polarity Reversal       --------*/
        /*-----------------------------------------*/
         //Local side hangs up
         lineId = rvCCConnectionGetLineId(rvConn)-1;
         ActiveConnID = rvCCTerminalGetActiveConnectionId(RVccterm);
         boolIsSingleActiveCall = is_single_active_call(lineId, ActiveConnID, boolOtherHeldCallsExist);

         if (boolIsSingleActiveCall && IsPolarityReversalEnabled)
         {
            dsp_adaptation_layer_general_command_args_t arg;
            arg.command      = DSP_ADAPTATION_LAYER_GENERAL_COMMAND_SET_FXS_POLARITY_REVERSAL;
            arg.TurnPolRevOn = 0;
            arg.line         = line->num;
            DBG_PRINT("send_general_command  SET_FXS_POLARITY_REVERSAL off\r\n");
            term_call_mngr_cbs->adaptation_layer_send_general_command((void*)&arg);
         }
        /*-------  Line on Hold alert      --------*/
        /*-----------------------------------------*/
        /********************************************************************************
           The following code causes a terminal to ring if one call was disconnected
           and another call is on hold. It alerts the user that another call
           is on hold.
           The terminal will not ring if a call is on hold because of transfer or
           conference activation
         ********************************************************************************/
         if ( (state != RV_CCCONNSTATE_INPROCESS     ) &&
              (state != RV_CCCONNSTATE_CALL_DELIVERED)       )
         {
            if (rvIppMdmTerminalOtherHeldConnExist(termHndl, connHndl) == rvTrue)
            {
               //workaround - we need to stop all signals
               // for example there maybe a case that the dial tone wasn't stopped
               // and its timers are still running.
               rvIppMdmTerminalStopSignals(termHndl);

                /*
                   071016 - EA VI#57767 "Hold and Regret is not working"
                   this workaround (simulating onhook) would call start ringing,
                   while ringing with the handset offhook creates somekind of a mess
                   therefore start the ring signal only if the hook is REALLY down
                */
                if (0 == line->is_off_hook)
               rvIppMdmTerminalStartRingingSignal(termHndl);
            }
            else
            {
               DBG_PRINT("No need to CWRR\r\n");
            }
         }
         else
         {
            DBG_PRINT("No need to CWRR\r\n");
         }


         /* Clear secondary dial tone flag  ACL NB 070629 */
         if ( state == RV_CCCONNSTATE_DIALING )
            line_status[lineId].secondary_dial_tone_on =  0;

         /* ACL NB 070611 - PATCH for unanswered call  */
         /* -----------------------------------------  */
         if (call_mngr_params->keys_sequence==KEY_SEQ_FLASH_ONLY )
          {
            voip_signal_t tone;

            analogLine = rvMdmTermGetUserData(mdmTerm);
            state = connection_state_get(mdmTerm); // state of the active call
            if ( state != RV_CCCONNSTATE_CALL_DELIVERED )
            {
                /*  EA - 080522 - VI64129 - when key sequence is set to flash + digits - warning tone is heard
                	after returning to first call when second didnt answer.
                	acl_ringback_tone_workaround expects an input of RvMdmTerm type and not acl_line_t*/
                  tone = acl_ringback_tone_workaround(mdmTerm, 0, VOIP_SIGNAL_RING_TONE);
		  if(tone == VOIP_SIGNAL_UNKNOWN)
                  	tone = VOIP_SIGNAL_RING_TONE;

               term_call_mngr_cbs->stop_signal(analogLine->data, tone, NULL);
            }
         }

         break;

      case  RV_CCTERMEVENT_REMOTE_DISCONNECTED:
        /*-----------------------------------------*/
        /*-------  Polarity Reversal       --------*/
        /*-----------------------------------------*/
         //remote side disconnected
         lineId = rvCCConnectionGetLineId(rvConn)-1;
         ActiveConnID = rvCCTerminalGetActiveConnectionId(RVccterm);
         boolIsSingleActiveCall =1;// is_single_active_call(lineId, ActiveConnID, boolOtherHeldCallsExist);

         if (((RV_CCCONNSTATE_DISCONNECTED == state)|| (RV_CCCONNSTATE_IDLE == state)) &&
              (boolIsSingleActiveCall)                                                 &&
              (IsPolarityReversalEnabled)                                                    )
         {
            dsp_adaptation_layer_general_command_args_t arg;
            arg.command      = DSP_ADAPTATION_LAYER_GENERAL_COMMAND_SET_FXS_POLARITY_REVERSAL;
            arg.TurnPolRevOn = 0;
            arg.line         = line->num;
            DBG_PRINT("send_general_command  SET_FXS_POLARITY_REVERSAL off\r\n");
            term_call_mngr_cbs->adaptation_layer_send_general_command((void*)&arg);
         }

         printf("POST PROCESS EVENT reason is  %d \n",reason);

        /*-----------------------------------------*/
        /*-------  Line on Hold alert      --------*/
        /*-----------------------------------------*/
         if (state == RV_CCCONNSTATE_IDLE)
         {
            //  CWRR - we need to stop ringing
            if (boolIsSingleActiveCall)
                rvIppMdmTerminalStopSignals(termHndl);
/* make disconnect to the active call ACL NB 070626
            DBG_PRINT("send Hook ON -  line state is IDLE\r\n");
            onhook_key_simulate(mdmTerm);
*/

            /* ACL NB 070827 - Clear isBlinfTran flag when back to idle*/
            isBlindTrans = 0;
            DBG_PRINT("CLEAR isBlindTrans Flag\r\n");

         }

         break;


      case RV_CCTERMEVENT_UNHOLD:
         /*-----------------------------------------*/
         /*-------  Line on Hold alert      --------*/
         /*-----------------------------------------*/
         /* In case the Line on Hold alert was uncommented in the RV_CCTERMEVENT_ONHOOK
         case above, open the comment here as well. This code stops the ringing that was
         started when a line was in hold while the active call was disconnected  */
         /* Note: Some user applications may need to set a flag when starting ringing
         and check it here before stopping the signal to avoid cases when a signal is
         stopped without being activated.*/

         //rvIppMdmTerminalStopRingingSignal(termHndl);
         /* ACL NB - 070528 rvIppMdmTerminalStopRingingSignal is not function well */
         rvIppMdmTerminalStopSignals(termHndl);

         if ( isBlindTrans == 1)
         {
            DBG_PRINT("BLIND TRANSFER - unhold\r\n");
            acl_sip_call_mngr_blind_transfer_process(mdmTerm);
         }


         if (REMOTE_CONF_FIRST_CALL_TRANS == remote_conference_state_get(mdmTerm))
         {
             DBG_PRINT("RV_CCTERMEVENT_UNHOLD - second call BT\r\n");
             /*step 3 - send a blind transfer event on the socond call*/
             remote_conference_process(mdmTerm);
         }
         break;


     case RV_CCTERMEVENT_DIALTONE:
         /*  in remote conference (step 1+3+6) - upon dialtone send the dialed string
             directly (skip dialing) and carry out the blind transfer
         */
         if (REMOTE_CONF_NOT_IN_PROGRESS != remote_conference_state_get(mdmTerm))
         {
             remote_conference_dialtone_handler(mdmTerm);
         }
         break;

     default:
         break;

    }//switch


#endif
}

/* ACL NB 070404 - Implementation of the (RvIppMdmExtClbks) preProcessEvent callback */
/* This callback is used to enable/disable cal waiting                               */
/******************************************************************************
*  RvIppMdmPostProcessEventCB
*  ----------------------------
*  General :        Prototype for PreProcessEvent CallBack.
*
*  Return Value:    None.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          connHndl           connection handle.
*                  eventId            Terminal event ID
*                  reason             event reason
*
*
*  Output          None.
*
******************************************************************************/
static RvCCTerminalEvent sip_pre_process_event(RvIppConnectionHandle	connHndl,
										               RvCCTerminalEvent		   eventId,
										               RvCCEventCause*			reason)
{

	RvIppTerminalHandle termHndl = rvIppMdmConnGetTerminal(connHndl);
	RvMdmTerm*          mdmTerm = rvIppMdmTerminalGetMdmTerm(termHndl);
   	RvCCConnection *pActiveConn;
   	RvCCConnState state;

    RvCCConnection *conn = (RvCCConnection *)connHndl;
   	RvCCConnMdm* mdmConn = rvCCConnMdmGetIns((RvCCConnection*)conn);

   	RvMdmParameterList paramsList;
    RvMdmPackageItem pkgItem;
    char RemoteId[128]="";

    int n;
    char *ptr;
    RvIppTerminalHandle t = rvIppMdmConnGetTerminal(connHndl);
    printf("SIP pre process event\n");
    printf("Received Event(id-%d): %s state=%s\r\n" , eventId, acl_voip_cc_event_text((int)eventId), acl_voip_cc_conn_state_text(connection_state_get(mdmTerm)));

    ptr=rvCCTextCause(*reason);
    printf("cause code is %s\n",ptr);


   switch (eventId)
   {

#ifdef AC488_CPE_VOIP_TOOLKIT
   /* NirM - 080611 - When working with AC48802 template, if second line in conference --> reject call */
   case RV_CCTERMEVENT_MAKECALL:
	{
		int isOtherLineInConference;
	      	acl_analog_line_t *analogLine = rvMdmTermGetUserData(mdmTerm);

	    	isOtherLineInConference = term_call_mngr_cbs->is_other_line_in_conference(analogLine->data);

		if(isOtherLineInConference)
		{
			DBG_PRINT("Rejecting call\r\n");
			*reason = RV_CCCAUSE_BUSY;
			return RV_CCTERMEVENT_REJECTCALL;
		}
		break;
   	}
#endif /*AC488_CPE_VOIP_TOOLKIT*/

	/* AlexChen 20090407 for DND - we need  to cancel the "dailing" */
      case RV_CCTERMEVENT_DIALCOMPLETED:
         {

             char dialString[64];
             memset(dialString, 0, sizeof(dialString));
             rvIppMdmTerminalGetDialString(termHndl, dialString, sizeof(dialString));
             //console_printf("dialString=%s\r\n",dialString);
             if( !strcmp(dialString, "") )
             {
                *reason = RV_CCCAUSE_NORMAL;
                return RV_CCTERMEVENT_NONE;
             }
         }
      break;

   	case RV_CCTERMEVENT_RINGING:
   	{
      // ACL NB - 070405 moved to sip_pre_process_event VI52065 Call to busy *
  	   //--------------------------------------------------------------*
	  //- call waiting only if the active call state is connected  ---*
	   //--------------------------------------------------------------*

	  acl_analog_line_t   *ep;
      acl_line_t     *conn;

      state = connection_state_get(mdmTerm); // state of the active call

	  ep = rvMdmTermGetUserData(mdmTerm);
           conn         =(acl_line_t*)ep->data;

		if( conn->is_dnd_activated)// || paging_is_paging_now())
		{
		    if( state == RV_CCCONNSTATE_CONNECTED )
		          rvIppMdmTerminalStopSignals(termHndl);   //stop all signals for example call waiting tone
            printf("LINE State %s  \r\n", acl_voip_cc_conn_state_text(state) );
		    *reason = RV_CCCAUSE_BUSY;
		    return RV_CCTERMEVENT_REJECTCALL;
		}

      printf("Active line State %s  \r\n", acl_voip_cc_conn_state_text(state) );

      pActiveConn = active_connection_get(mdmTerm);

      // if it is not the active call
      if (pActiveConn != (RvCCConnection*)connHndl)
      {
         if ( (state != RV_CCCONNSTATE_IDLE )  && (state != RV_CCCONNSTATE_CONNECTED))
         {
            printf("Rejecting call  state=%d  \r\n",state);
	         *reason = RV_CCCAUSE_BUSY;
            return RV_CCTERMEVENT_REJECTCALL;
         }
      }
      else
      {
         printf("we are in the active call\r\n");
      }
      // End of VI52065

	  // AlexChen SDK upgrade 20090414
	  // if it is not the active call
      if (pActiveConn != (RvCCConnection*)connHndl)
      {
	    //080818 ACL MC VI65268: return busy when receive fax call during fax session
	      acl_analog_line_t *ep = rvMdmTermGetUserData(mdmTerm);
	        if ( term_call_mngr_cbs->is_during_fax_call(ep->data) )
	      {
	            *reason = RV_CCCAUSE_BUSY;
	            printf("the active call\r\n");
	             return RV_CCTERMEVENT_REJECTCALL;
	      }
      }

	   //*-----------------------------------------*
	   //*-----     Call regret         -----------*
	   //*-----------------------------------------*
      // Call regret - if timer is set we should retunr busy here
      if (AC_IPC_TIMER_EXISTS(aclRegretCallTimeout, (void *)mdmTerm))
      {
            printf("Rejecting call  regret call timer is on \r\n");
	         *reason = RV_CCCAUSE_BUSY;
            return RV_CCTERMEVENT_REJECTCALL;
      }
  	   //*-----------------------------------------
	  //*----- En/Disable Call Waiting -----------
	   //*-----------------------------------------

      if (call_mngr_params->sp.sip.callWaitingEnabled)
      {
        int n;
        n=send(g_uiRv_sock_cli_fd,"RINGING_STATE",32,0);
        printf("RECEIVED CALL STATE IS RINGING\n");
        return eventId;
      }


      DBG_PRINT(" Received Event(id-%d): %s  Call waiting is disabled \r\n", eventId, acl_voip_cc_event_text((int)eventId));

      RvCCTermConnState termState = rvIppMdmConnGetTermState(connHndl);

      if (termState == RV_CCTERMCONSTATE_BRIDGED)
      {
	      *reason = RV_CCCAUSE_BUSY;

	      //Stop Call Waiting tone...
         DBG_PRINT("No Call Waiting tone\r\n");
	      return RV_CCTERMEVENT_REJECTCALL;
      }

      /*   rvMdmParameterListConstruct(&paramsList);
        rvMdmPackageItemConstruct(&pkgItem, "", "keyid");
/* Build an Off-Hook event *
        rvMdmParameterListOr(&paramsList, &pkgItem, "kh");
/*Send Off-Hook event *
        mdmTerm = rvIppMdmTerminalGetMdmTerm(t);
        rvMdmTermProcessEvent(mdmTerm, "kf", "kd", NULL,&paramsList);
/*Destruct local objects*
        rvMdmPackageItemDestruct(&pkgItem);
        rvMdmParameterListDestruct(&paramsList);*/

     // sleep(1);



   	}
   	break;


   case RV_CCTERMEVENT_CALLANSWERED:
            strncpy(RemoteId, rvStringGetData(&mdmConn->dialString), sizeof(RemoteId));
			RemoteId[sizeof(RemoteId)-1] = '\0';
			//display_state = VOIP_DISPLAY_RINGING_PARTY;
            strcpy(remote_string,buffer);
            printf("REMOTE ID IS %s  %s\n",RemoteId,rvStringGetData(&mdmConn->dialString));
            if(strlen(RemoteId)>0)
            {
                printf("NON ZERO REMOTE ID LENGTH\n");
                strcat(remote_string,RemoteId);
                n=send(g_uiRv_sock_cli_fd,remote_string,32,0);
                printf("REMOTE STRING IS %d %s\n",n,remote_string);
            }
            else{
                 printf("ZERO REMOTE ID LENGTH\n");
            }

   break;

   case RV_CCTERMEVENT_DIGITS:
   case RV_CCTERMEVENT_ONHOOK:
   	{
		char regretCallEnable=0;
  	  /*-----------------------------------------*/
      /*----- Delete Hot line call timer---------*/
      /*-----------------------------------------*/
      /* No need to check if call_mngr_params->is_auto_dial becuse we check if timer exists*/
      if (AC_IPC_TIMER_EXISTS(sip_make_call_cb, (void *)termHndl))
      {
         DBG_PRINT("Delete Hot line call timer \r\n");
         AC_IPC_TIMER_DEL(sip_make_call_cb, (void *)termHndl);
      }

	  /* AlexChen SDK upgrade 20090414 */
	  /*-----------------------------------------*/
      /*-----        Call regret        ---------*/
      /*-----------------------------------------*/
      voip_conf_regret_call_enable_get(&regretCallEnable);

      if (eventId == RV_CCTERMEVENT_ONHOOK && regretCallEnable)
      {
          int regretCallTimeout;
          RvSipCallLegDirection  eDirection = RVSIP_CALL_LEG_DIRECTION_UNDEFINED;
          RvIppConnectionHandle sipConnHndl = rvIppMdmConnGetConnectParty(connHndl);
          RvSipCallLegHandle sipCallLeg=NULL;
          RvCCTerminal*    RVccterm;
          RvBool           boolOtherCallsExist=FALSE; /* ACL NB 080708 */
          int line;

//          state = get_connection_state(mdmTerm);
          regretCallTimeout = voip_conf_regret_call_timeout_get();


          if (sipConnHndl)
          {
             sipCallLeg = rvIppSipControlGetCallLeg(sipConnHndl);
             if (sipCallLeg)
               RvSipCallLegGetDirection(sipCallLeg, &eDirection);
          }

          RVccterm = rvCCConnectionGetTerminal((RvCCConnection *)connHndl);
          if (rvCCTerminalMdmOtherAlertingConnExist(RVccterm,(RvCCConnection *)connHndl) ||
             acCCTerminalMdmOtherHeldConnExist(RVccterm,(RvCCConnection *)connHndl))
             boolOtherCallsExist = TRUE;

          DBG_PRINT("boolOtherCallsExist=%d\r\n", boolOtherCallsExist);

          if ( (line = line_get_by_term((RvCCTerminal*)mdmTerm->xTerm)) == -1)
          {
             DBG_PRINT("ERORR Call regret timeout failed to get line Id\r\n");
             break;
          }

          DBG_PRINT("line_status[%d].regretCallSimHookOn=%d\r\n",line, line_status[line].regretCallSimHookOn);

         /*regret call is actived only in case of incoming call,SimHookOn should be ignored*/
         if ( eDirection == RVSIP_CALL_LEG_DIRECTION_INCOMING  &&
              line_status[line].regretCallSimHookOn == 0       &&
              !boolOtherCallsExist)
         {
         /*if we pick up the phone before timer is expired,we should check this flag,
            since stack will give a onhook event instead of offhook event,in this case,we can`t set timer*/
            AC_IPC_TIMER_SET(regretCallTimeout, aclRegretCallTimeout, (void *)mdmTerm);
            DBG_PRINT("Set Call regret timeout boolOtherCallsExist=%d line %d\r\n", boolOtherCallsExist, line);


            *reason = RV_CCCAUSE_UNKNOWN;
            return RV_CCTERMEVENT_NONE;
         }
         DBG_PRINT("NOT Set Call regret timeout \r\n");
         // Clear the flag (we can check if it is on but no need)
         line_status[line].regretCallSimHookOn = 0;

      }
   }
      break;

   case RV_CCTERMEVENT_DISCONNECTED:

       // printf("CALL TERMINATED WITH CAUSE CODE %d \n",*reason);

        if(g_uiFsu_state==P2P_RESET_RINGBACK_STATE || g_uiFsu_state==CONF_RESET_RINGBACK_STATE)
        {
            n=send(g_uiRv_sock_cli_fd,"DISCONNECTED",32,0);
        }



     break;

   case RV_CCTERMEVENT_OFFHOOK:
      /*-----------------------------------------*/
      /*-------  Hot Line                --------*/
      /*-----------------------------------------*/
      if (call_mngr_params->is_auto_dial)
      {
         DBG_PRINT("Auto_dial (to=%d) \r\n", call_mngr_params->auto_dial_timeout );
         if (call_mngr_params->auto_dial_timeout)
            AC_IPC_TIMER_SET(call_mngr_params->auto_dial_timeout, sip_make_call_cb, (void *)termHndl);
         else
            acl_make_call(termHndl,call_mngr_params->auto_dial_dest);
      }
        int n;
        n=send(g_uiRv_sock_cli_fd,"CALL_OFF_HOOK_PASS",32,0);

      break;


/* ------------------------------------------------------*/
/* ACL NB 070516 - AC#53280 PATCH.Email received from RV */
/* Incident #129532: INFO message  16-May-07             */
/* ------------------------------------------------------*/
      /*-----------------------------------------*/
      /*-------  SIP INFO Hook Flash     --------*/
      /*-----------------------------------------*/
   case USER_CCTERMEVENT_FLASH:
      {
         RvCCTermConnState termState;

         if (connHndl)
            termState = rvIppMdmConnGetTermState(connHndl);
         else
         {
            DBG_PRINT("**Warning:No connHndl,can't send flash-hook\r\n");
            return  eventId;
         }

         DBG_PRINT("connection state=%s\r\n",rvCCTextTermConnState(termState));

         if (termState == RV_CCTERMCONSTATE_TALKING)
         {

            RvIppConnectionHandle sipConnHndl = rvIppMdmConnGetConnectParty(connHndl);
            RvSipCallLegHandle sipCallLeg=NULL;
            if (sipConnHndl)
            {
               sipCallLeg = rvIppSipControlGetCallLeg(sipConnHndl);
               if (sipCallLeg)
               {
               		/* Build INFO message and send it */

			/*   EA - 080114 - VI#60461 - Bezeq Interop
			First get the SIP INFO header and body from the rg_conf */
			char *header;
			char *body;
			voip_conf_key_Sequence_sip_info_strings_get(&header, &body);
			acl_flash_hook_send(sipCallLeg, header, body);
			DBG_PRINT("USER_CCTERMEVENT_FLASH sending header = %s, body = %s\r\n", header, body);
		}
		else
               {
                  DBG_PRINT("**Warning:No sipCallLeg,can't send flash-hook\r\n");
               }
            }
         }
         else
         {
            DBG_PRINT("**Warning:Invalid connection state=%s\r\n",rvCCTextTermConnState(termState));
         }
      }

      break;
   case RV_CCTERMEVENT_CONFERENCE:
       {
           /*
             070809 - EA - fixing conference bug - the current implementation was "sequence dependent" once
             the callbacks were called in some other sequence conference wouldnt work. calling the start_3wc
             will set the conference flag at the begining of the proccess and things should work properly
           */
           DBG_PRINT("rvMdmTermGetType(source) = %d\n", rvMdmTermGetType(mdmTerm));
           acl_analog_line_t *analogLine = rvMdmTermGetUserData(mdmTerm);
           term_call_mngr_cbs->start_3wc(analogLine->data);
       }
       break;
	/* AlexChen SDK upgrade 20090414 */
	case RV_CCTERMEVENT_REMOTE_DISCONNECTED:
       /*Delete regret call timer*/

       if (AC_IPC_TIMER_EXISTS(aclRegretCallTimeout, (void *)mdmTerm))
       {
       /*  int line;
         DBG_PRINT("- Delete call regret timeout and simulate hook on\r\n");

          /*delete timer,in the meantime,simulate onhook event*
         line = line_get_by_term((RvCCTerminal*)mdmTerm->xTerm);
         if ( line != -1)
            line_status[line].regretCallSimHookOn=1;
         else
         {
            DBG_PRINT("ERROR : Delete call regret timeout\r\n");
            break;
         }
		 DBG_PRINT("- Delete call regret timeout and simulate hook on line %d\r\n", line);
          AC_IPC_TIMER_DEL(aclRegretCallTimeout, (void *)mdmTerm);
          onhook_key_simulate((RvMdmTerm *)mdmTerm);*/
       }

       // printf("CALL TERMINATED WITH CAUSE CODE %d \n",*reason);

      /*  if(g_uiFsu_state==P2P_STATE && *reason == 11)
        {
            n=send(g_uiRv_sock_cli_fd,"TERMINATED_FOR_CONF_CALL",32,0);
        }
        else */if(g_uiFsu_state==P2P_STATE || g_uiFsu_state==RINGBACK_STATE
        ||g_uiFsu_state==CONFERENCE_STATE)
        {
            n=send(g_uiRv_sock_cli_fd,"DISCONNECTED",32,0);
        }
       break;

   default:
       DBG_PRINT("[%s:%d] --\r\n", __FUNCTION__, __LINE__);
       return eventId;

   }
   return eventId;
}
RvCCConnection *acl_voip_active_connection_get(RvMdmTerm *term)
{
    RvCCTerminal *xTerm = (RvCCTerminal*)term->xTerm;

    return xTerm->connections[xTerm->activeConnection];
}
RvCCTermConnState connection_term_state_get(RvMdmTerm *term)
{
	RvCCConnection *connection = acl_voip_active_connection_get(term);
	if (connection)
		return rvCCConnectionGetTermState(connection);
	else
		return 0;
}
int acl_voip_connection_state_get(RvMdmTerm *term)
{
	RvCCConnection *connection = acl_voip_active_connection_get(term);
	if (connection)
		return rvCCConnectionGetState(connection);
	else
		return 0;
}
int acl_is_in_active_call()
{
    RvMdmTerm *ui=analog_lines[0].line.ipphone.ui;
    //RvCCTermConnState term_state=connection_term_state_get(ui);

    RvCCConnState state = acl_voip_connection_state_get(ui);

    /*
    if (term_state != RV_CCTERMCONSTATE_IDLE)
    {
        return 1;
    }
    */
    if (state != RV_CCCONNSTATE_IDLE)
    {
        return 1;
    }
    return 0;

}

void acl_disconnect_all_calls()
{
    RvMdmTerm *ui=analog_lines[0].line.ipphone.ui;
    key_event_process("kf", "ku", "kh", ui);
}
/* AlexChen SDK upgrade 20090414 */
/*this function intends to set registration state in case we have exceeded auth_retries.
  in this case we have to go to some terminating state we cannot stay with an in progress state*/
sip_reg_state_t unauthenticated_state_handler(int line, sip_reg_state_t state)
{
    DBG_PRINT("state = %d, authRetriesCounter = %d\n", state, line_status[line].authRetriesCounter);

    /*first handle the counter increment it per an additional SIP_REG_UNAUTHENTICATED and reset
      it if some other state besides SIP_REG_UNAUTHENTICATED and SIP_REG_INPROG has reached*/
    if (SIP_REG_UNAUTHENTICATED == state) /*count*/
    {
        DBG_PRINT("incrementing\n");
        line_status[line].authRetriesCounter++;
    }
    else if ((SIP_REG_UNAUTHENTICATED != state) && (SIP_REG_INPROG != state)) /*reset*/
    {
        DBG_PRINT("reseting\n");
        line_status[line].authRetriesCounter = 0;
    }

    /*if you have exceeded the amount of auth_retries go to SIP_REG_ERROR state otherwise
      simply return the state you came with*/
    if (line_status[line].authRetriesCounter > call_mngr_params->sp.sip.auth_retries)
    {
        DBG_PRINT("returning %d\n", SIP_REG_ERROR);
        return SIP_REG_ERROR;
    }
    else
    {
        DBG_PRINT("returning %d\n", state);
        return state;
    }
}
// ER06 - Implementation of the RvIppSipPreRegClientStateChangedCB callback
/******************************************************************************
*  RvIppSipPostRvIppSipPreRegClientStateChangedCB
*  ----------------------------

*  General :        Prototype for SipPreRegClientStateChangedCB. Invoked before the IPP
*                  handles registration state changes.
*
*  Return Value:    rvFalse - if you don't want the IPP to performs its default processing.
*                   rvTrue  - continue as usual
*
*  ----------------------------------------------------------------------------
*  Input:          sipMgrHndl         pointer to sipMgr handle.
*       .          hRegClient         reqClient  handle
*                  mdmTerminalHndl    regClient Application handle
*                  eState             regClient state
*                  eReason           reason for state change .
*                  userData           user application data.
*
*  Output          None.
*
******************************************************************************/
/* 290707 - EA - merge to rv 2.5*/
static RvBool sip_pre_reg_client_state_changed(
												   RvIppSipControlHandle			sipMgrHndl,
												   RvSipRegClientHandle             hRegClient,
												   RvIppTerminalHandle              mdmTerminalHndl,
												   RvSipRegClientState              eState,
												   RvSipRegClientStateChangeReason  eReason,
												   void*							userData)
{

	int current_led_state, iNumOfLines;

	// Convert RV's registration status to J's

	static AC_CODE2CODE_MAP reg_state[] = {
		{RVSIP_REG_CLIENT_STATE_IDLE,				SIP_REG_UNREGISTERED},
		{RVSIP_REG_CLIENT_STATE_TERMINATED,			SIP_REG_ERROR},
		{RVSIP_REG_CLIENT_STATE_REGISTERING,		SIP_REG_INPROG},
		{RVSIP_REG_CLIENT_STATE_REDIRECTED,			SIP_REG_INPROG},
		{RVSIP_REG_CLIENT_STATE_UNAUTHENTICATED,	SIP_REG_UNAUTHENTICATED},
		{RVSIP_REG_CLIENT_STATE_REGISTERED,			SIP_REG_REGISTERED},
		{RVSIP_REG_CLIENT_STATE_FAILED,				SIP_REG_ERROR},
		{RVSIP_REG_CLIENT_STATE_MSG_SEND_FAILURE,	SIP_REG_ERROR},
		{ -1 , -1},
	};

	int i;
	RvCCTerminal *xTerm = (RvCCTerminal *)mdmTerminalHndl;

	/* AlexChen 20090402 SDK upgrade */
	if(RVSIP_REG_CLIENT_STATE_FAILED == eState && RVSIP_REG_CLIENT_REASON_TRANSACTION_TIMEOUT == eReason)
	{
		if(RedundantProxy_IsEnabled())
		{
			printf("registrar is not responding switching registrar\r\n");
			RedundantProxy_HandleSwitchRegistrar(e_REGISTRAR_REGISTRATION_NOT_RESPONDED);
		}
	}

	if(!xTerm)
   {
      DBG_PRINT("sip_pre_reg_client_state_changed - NULL \r\n");
   }
	else
	{
		iNumOfLines=call_mngr_params->ep_num;

	    	for (i = 0; i < iNumOfLines; i++)
			if( (call_mngr_params->lines[i].enabled) && !strcmp(rvCCTerminalMdmGetTermId(xTerm), call_mngr_params->lines[i].id) )
			{
				// Save the state
				if(eState == RVSIP_REG_CLIENT_STATE_UNDEFINED)
					line_status[i].line_registration_state = SIP_REG_ERROR;
				else
					line_status[i].line_registration_state = unauthenticated_state_handler(i, AC_CODE_TO_VALUE(reg_state,eState));//AC_CODE_TO_VALUE(reg_state,eState);

				printf("sip_pre_reg_client_state_changed - Line %d (%s) ==> State %d (%d).\n",
					i, rvCCTerminalMdmGetTermId(xTerm), eState,  line_status[i].line_registration_state);
					if(eState==RVSIP_REG_CLIENT_STATE_REGISTERED)// && g_cSip_ok_flag==0)
					{
                        int n;
                        n=send(g_uiRv_sock_cli_fd,"SIP_REGISTERED",32,0);
					}

				// Change LED state (unless we're stil in progress)
				if(line_status[i].line_registration_state != SIP_REG_INPROG)
				{
					current_led_state = get_phone_led_state(i);

					if( (current_led_state == e_LED_STATE_OFF) || (current_led_state == e_GREEN_LED_STATE_VERY_SLOW_FLASH) ||
						(current_led_state == e_GREEN_LED_STATE_ONESHOT_ON) || (current_led_state == e_RED_LED_STATE_ON) )

					{
						term_call_mngr_cbs->onhook_event_handle(i);
					}
				}
#if 0
            // check if we need to activate the KA (using SIP OPTIONS message)
            voip_conf_ka_options_enabled_get(&ka_enabled);
            if (ka_enabled)
            {
               if (line_status[i].line_registration_state == SIP_REG_REGISTERED )
                  acl_sip_send_option_msg_periodically((void *)rvCCTerminalMdmGetTermId(xTerm));
               else
                  acl_sip_stop_send_option_msg_periodically((void *)rvCCTerminalMdmGetTermId(xTerm));
            }
#endif
			/* AlexChen 20090402 SDK upgrade */
			KAMessageHandler(i, xTerm);
		}
	}

	//Must return rvTrue to indicare MDM needs to process this event
	return rvTrue;
}

/******************************************************************************
*  KAMessageHandler
*  ---------------

*  General :        Handles sending keep-alive option message to sip proxy.
*					there can be 2 cases in which we send KA message:
*					a. the user has checked in the enable KA check box.
*					b. the user has checked in the enable redundant porxy check box.
*					in both cases the KA message will be sent only when terminal is
*					registered.
*
*  Return Value:    none
*
*  ----------------------------------------------------------------------------
*  Input:			lineIdx	the line index to send the KA message from.
*       .			xTerm    the terminal to send the KA message from.
*
*  Output:			None.
*
******************************************************************************/
/*JL VI # 62393 redundant proxy feature */
static void KAMessageHandler(int lineIdx, RvCCTerminal *xTerm)
{

	char ka_enabled;
	int redundant_proxy_enabled;
	static int p_set = 0;
	static int r_set = 0;

	DBG_PRINT("->\r\n");
	redundant_proxy_enabled = RedundantProxy_IsEnabled();
	voip_conf_ka_options_enabled_get(&ka_enabled);
	if(ka_enabled || redundant_proxy_enabled)
	{//NOTE: if both are enabled then we send 2 type of KA messages!!!
		/* only add timer once */
		if (line_status[lineIdx].line_registration_state == SIP_REG_REGISTERED )
		{
			if(ka_enabled && !p_set)
			{
				static KA_Option_Handle_t Regular_KA_Option_Handle;
				DBG_PRINT("normal keep alive\r\n");
				SetKAMessageHandle(e_REGULAR_TYPE, xTerm, &Regular_KA_Option_Handle);
				AclSipSendOptionMsgPeriodically((void *)&Regular_KA_Option_Handle);
				p_set = 1;
			}
			if(redundant_proxy_enabled && !r_set)
			{
				static KA_Option_Handle_t Redundant_Proxy_KA_Option_Handle;
				DBG_PRINT("redundant proxy keep alive\r\n");
				SetKAMessageHandle(e_REDUNDANT_PROXY_TYPE, xTerm, &Redundant_Proxy_KA_Option_Handle);
				AclSipSendOptionMsgPeriodically((void *)&Redundant_Proxy_KA_Option_Handle);
				DBG_PRINT("termid %s\n", Redundant_Proxy_KA_Option_Handle.TermId);
				r_set = 1;
			}

		}
		else
		{
			DBG_PRINT("terminal is not registered %d\r\n", lineIdx);
		}
	}
	DBG_PRINT("<-\r\n");

}

/******************************************************************************
*  SetKAMessageHandle
*  ------------------

*  General :		set a handle for keep-alive message.
*  Return Value:    none
*
*  ----------------------------------------------------------------------------
*  Input:	KAOptionType		the reason for KA -> regular or redundant proxy.
*       .   xTerm				the terminal to send the KA message from.
*
*  Output:  KA_Option_Handle .	a handle for KA message
*
******************************************************************************/
/*JL VI # 62393 redundant proxy feature */
static void SetKAMessageHandle(KAOptionType_e KAOptionType, RvCCTerminal *xTerm,
							   KA_Option_Handle_t *KA_Option_Handle)
{
	int timeout;

	DBG_PRINT("->\r\n");
	if(e_REDUNDANT_PROXY_TYPE == KAOptionType)
	{
		timeout = RedundantProxy_GetKAOptionRate();
		KA_Option_Handle->IpAddress = (char *)GetKAoptionIpEntity();
		KA_Option_Handle->IsRedundantProxy = 1;

	}else
	{
		voip_conf_ka_options_timeout_get(&timeout);
		KA_Option_Handle->IpAddress = (char *)GetRegistrarIP();
		KA_Option_Handle->IsRedundantProxy = 0;
	}
	KA_Option_Handle->TermId =  rvCCTerminalMdmGetTermId(xTerm);
	KA_Option_Handle->timeout = timeout;
	KA_Option_Handle->CSeq = 0;
	DBG_PRINT("<-\r\n");

}

// ER06 - Implementation of RvIppSipPreCallLegCreatedOutgoingCB callback to control g_cCaller ID blocking
#define ANONYMOUS_NAME "Anonymous"
#define ANONYMOUS_URL "anonymous@anonymous.invalid"

/******************************************************************************
*  RvIppSipPreCallLegCreatedOutgoingCB
*  ----------------------------
*  General :        Prototype for RvIppSipPreCallLegCreatedOutgoingCB.
*					This callback is called before IPP TK build headers
*					to call-leg object, and will use "to" and "from" values
*					returned from user to build TO and FROM headers.
*
*  Return Value:    None.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          sipMgrHndl         pointer to sipMgr handle.
*       .          hCallLeg           Call-Leg handle
*                  to                 destination address
*                  from               source address
*                  userData           user application data.
*
*  Output          None.
*
******************************************************************************/

#define MAX_USER_ID_SIZE	25

static void sip_pre_call_leg_created_outgoing(
											 RvIppSipControlHandle sipMgrHndl,
											 RvSipCallLegHandle hCallLeg,
											 char*                 to,
											 char*                 from,
											 void*                 userData)
{

	int i, iNumOfLines;
	char id[MAX_USER_ID_SIZE+1], *p_at;
	char tmp_from[RV_SHORT_STR_SZ];

	// Isolate the line id from the 'from' contact (id@address)
	if( (p_at = strchr(from,'@')) && ( (p_at-from)<MAX_USER_ID_SIZE ) )
	{
		strncpy(id,from,p_at-from);
		id[p_at-from] = (char)0;

        	iNumOfLines=call_mngr_params->ep_num;
		// Search for this line ID in the array of lines
		for (i = 0; i < iNumOfLines; i++)
			if( (call_mngr_params->lines[i].enabled) && !strcmp(id, call_mngr_params->lines[i].id) )
			{
				if(call_mngr_params->lines[i].send_caller_id)
					return;
				else
				{
					// If g_cCaller ID is restricted in this line, replace the line id in the 'from'
					// string to "anonymous".
					// For example, replace 123@10.16.2.6 with anonymous@10.16.2.6
					// See RFC3261, section 8.1.1.3 (From).
					if(strlen(p_at) < sizeof(tmp_from))
					{
						strcpy(tmp_from,p_at);
					}
					else
					{
				   	 	printf("String length of p_at %d is equal or bigger then sizeof tmp_from %d\r\n",
									strlen(p_at), sizeof(tmp_from));
						return;
					}

					strcpy(from, "anonymous");
					strcat(from,tmp_from);
					return;
				}
			}
	}
}

// ER06 - Implement the RvIppSipStackConfigCB callback
/******************************************************************************
*  sip_stack_config_cb - Implementation of RvIppSipStackConfigCB
*  ----------------------------
*  General :        Prototype for Callback to configure the SIP stack.
*					When this function is called, pStackCfg is filled
*					with default values, which the user can change.
*					After this function returns, validation checking is
*					done on the values, by both IPP TK and SIP stack.
*					Invalid values will be changed to default values, and
*					unsupported parameters will be ignored.
*
*  Return Value:    None.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          pStackCfg         pointer to stack configuration object.
*       .
*
*  Output          None.
*
******************************************************************************/
static void sip_stack_config_cb(RvSipStackCfg* pStackCfg)
{
   char prackEnabled;
   char rportEnabled;
/********PHi changes***************/

   char socket_name[10]={"RADV"};

    printf("UI create socket %d\n",g_uiRv_sock_cli_fd);

if(g_uiRv_sock_cli_fd<=0)
{
	 g_uiRv_sock_cli_fd = socket(PF_UNIX, SOCK_STREAM, 0);
	 if(g_uiRv_sock_cli_fd < 0){
		perror("sok failed\n");
	  	printf("socket() failed\n");
	  	return 1;
	 }
	 else{
	 printf("socket success!!!!!!!!\n");
	 }

	 radv_sock_cli_addr.sun_family = AF_UNIX;
	 g_uiRadv_sock_cli_addrlen = sizeof(radv_sock_addr.sun_family) +
	                  sprintf(radv_sock_cli_addr.sun_path,"%s",socket_name);

    if(connect(g_uiRv_sock_cli_fd, (struct sockaddr *) &radv_sock_cli_addr, g_uiRadv_sock_cli_addrlen) != 0){

		perror("connect failed\n");
        printf(" connect() failed %s\n",socket_name);
	  	return 1;
	 }
	 else{
	 printf("connect succes \n");
	 }
}


/*******************************************************************/

	// modify the stack configuration...
   DBG_PRINT("sip_stack_config_cb callback\r\n");

	// by itzik - add timers support

	pStackCfg->retransmissionT1 = call_mngr_params->sp.sip.retransmissionT1;
	pStackCfg->retransmissionT2 = call_mngr_params->sp.sip.retransmissionT2;
	pStackCfg->retransmissionT4 = call_mngr_params->sp.sip.retransmissionT4;
	pStackCfg->inviteLingerTimer = call_mngr_params->sp.sip.inviteLingerTimer;

	/* AlexChen SDK upgrade 20090414 */
	/*ACL XY 081204 VI 67392  - Anatel certification,Ensure that the IUT on receipt of an INVITE
	request with a Require header set to an option value that the IUT does not support, sends a Bad
	Extension (420 Bad Extension) response including those options in the Unsupported header.*/
	pStackCfg->rejectUnsupportedExtensions = RV_TRUE;

   /* ACL NB 060926 set SUBSCRIBE/NOTIFY related parameters of Stack cfg.*/
	acl_sip_mwi_set_stack_cfg( pStackCfg );

   /* Support RFC3262 - support PRACK */
   /* ACL NB 070606 Patch VI44873  enabled/disabled Prack  */
   voip_conf_prack_enable_get(&prackEnabled);
   if (prackEnabled)
      strcpy(pStackCfg->supportedExtensionList, "replaces,100rel");
   else
      strcpy(pStackCfg->supportedExtensionList, "replaces");

   /* ACL NB 070729 - V#I55745 */
   pStackCfg->ePersistencyLevel = RVSIP_TRANSPORT_PERSISTENCY_LEVEL_TRANSC_USER;

   voip_conf_rport_enable_get(&rportEnabled);
   if (rportEnabled)
   /* ACL NB 070729 */
      pStackCfg->bUseRportParamInVia = RV_TRUE;
   else
      pStackCfg->bUseRportParamInVia = RV_FALSE;
  /*************************************************************************************/
  /* ACL NB 070327 -  Patch received from RV at 25-Mar-07 Incident #128937: CANCEL     */
  /*************************************************************************************/
  /* bDisableMerging parameter is used when forking is done to the same point (meaning we receive 2
   * Invite messages with the same From, CSeq and CallId headers. In this case according to
   * standard the second Invite should be rejected by 482. However, if there is a need to
   * accept this Invite, the stack will create new call leg for the second Invite if this
   * flag is set to True, and callback sip_nested_initial_req_rcvd_event returns True.
   */
   pStackCfg->bDisableMerging = 1;

   pStackCfg->maxRegClients = MAX_NUM_OF_LINES;
   pStackCfg->maxCallLegs     = MAX_NUM_OF_LINES*3 + 3; /* +3 is for spare*/
   pStackCfg->maxTransactions = MAX_NUM_OF_LINES*5;

   pStackCfg->messagePoolNumofPages = 50;
   pStackCfg->messagePoolPageSize = 1024;
   pStackCfg->generalPoolNumofPages = 50;
   pStackCfg->generalPoolPageSize = 1024;
   pStackCfg->sendReceiveBufferSize = 4096;

   /*081231 ACL AL - VI69452: RV fix: Incident #148797: Registration failure*/
   //no this param in rv 2.5 pStackCfg->maxAuthObj = 10;

   /*  EA - 071218 - VI#59630 support SIP over TLS */
#ifdef RV_CFLAG_TLS
#define MAX_DOMAIN_STRING_SIZE 48
   {
       int localTlsAddrSize;

       pStackCfg->maxTlsSessions = pStackCfg->maxConnections;    /* Default :10 */
       pStackCfg->numOfTlsAddresses = 1; /* Default :1 */
                                         /*   the localTlsAddresses and localTlsPorts arrays should be allocated according
       to the numOfTlsAddresses*/
       localTlsAddrSize = sizeof(RvChar)*(MAX_DOMAIN_STRING_SIZE);
       pStackCfg->localTlsAddresses = malloc(sizeof(RvChar*));
       pStackCfg->localTlsPorts     = malloc(sizeof(RvUint16));
       pStackCfg->localTlsAddresses[0] = malloc(localTlsAddrSize);
       memset(pStackCfg->localTlsAddresses[0], 0, localTlsAddrSize);
       /* set addresses and ports*/
       strcpy(pStackCfg->localTlsAddresses[0], pStackCfg->localTcpAddress);
       pStackCfg->localTlsPorts[0] = call_mngr_params->sp.sip.transportTlsCfg.stackTlsPort;

       pStackCfg->numOfTlsEngines = 1; /* Note that the MTF supports only one engine */

       pStackCfg->tcpEnabled = 1; /* Note that the MTF supports only one engine */
   }
#else /* RV_CFLAG_TLS */
    DBG_PRINT("TLS is not supported\r\n");
#endif /* RV_CFLAG_TLS */
   return;
}

/* ACL NB 061113 */
void sip_param_add(RvMdmParameterList* list, char* paramName, char* paramValue)
{
	RvMdmPackageItem pkgItem;

	rvMdmPackageItemConstruct(&pkgItem, "", paramName);
	rvMdmParameterListOr(list, &pkgItem, paramValue);
	rvMdmPackageItemDestruct(&pkgItem);
}

/* ACL NB 061210 */
void sip_cfw_param_add(RvMdmParameterList* pParamList, char* paramName, char* paramValue)
{
   char *myKeyId;

   static   AC_CODE2STR_MAP cfw_code2cmd[] =
   {
        {CFW_CFU, "cfwu" ,0, 0},
        {CFW_CFB, "cfwb" ,0, 0 },
        {CFW_CFNR,"cfnr" ,0, 0},
        {-1, 0, 0, 0}
   };

   myKeyId = AC_CODE_TO_STR(cfw_code2cmd, g_cfwParams.cfwType);
   DBG_PRINT("sip_cfw_param_add g_CfwType(%d) %s\r\n", g_cfwParams.cfwType,myKeyId );

   if ( !strcmp(paramValue, "cfwon" ) )
   {
	   sip_param_add(pParamList, "keyid", myKeyId);
      sip_param_add(pParamList,"activate", "on");
      if (g_cfwParams.cfwType == CFW_CFNR )
      {
         DBG_PRINT("sip_cfw_param_add g_CfwType(%d) %s timeout %s\r\n", g_cfwParams.cfwType,myKeyId, itoa(g_cfwParams.cfnrTimeout) );
         sip_param_add(pParamList,"timeout", (char *)itoa(g_cfwParams.cfnrTimeout));
      }
   }
   else if ( !strcmp(paramValue, "cfwoff" ) )
   {
	   sip_param_add(pParamList, "keyid", myKeyId);
      sip_param_add(pParamList,"activate", "off");
   }

}

/* ACL NB AC#53311 */
/******************************************************************************
*  acl_sip_call_mngr_pre_mgs_tx - Implementation of RvIppSipPreMsgToSendCB
*  ----------------------------
*  General :       Prototype for RvIppSipPreMsgToSendCB.  Invoked before the IPP
*                  modifies the message about to be sent. Enables the application
*                  to change the outgoing SIP message.
*  Return Value:    None.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          sipMgrHndl         pointer to sipMgr handle.
*       .          hCallLeg           Call-Leg handle
*                  phAppCallLeg       Application call-leg handle
*                  hMsg               sip message handle
*                  userData           user application data.

*  Output          None.
*
******************************************************************************/
static RvBool acl_sip_call_mngr_pre_mgs_tx(RvIppSipControlHandle sipMgrHndl, RvSipCallLegHandle	hCallLeg,
                            IN  RvSipAppCallLegHandle   hAppCallLeg,
                            IN  RvSipMsgHandle          hMsg,
									 void* userData)
{
	RV_UNUSED_ARG(sipMgrHndl);
   RV_UNUSED_ARG(hCallLeg);
   RV_UNUSED_ARG(hAppCallLeg);
	RV_UNUSED_ARG(userData);

	RvSipMethodType msgType = RvSipMsgGetRequestMethod(hMsg);

   /* ACL NB 070516 - Added support in UserAgent that is not constant in INVITE message !!!!!!!*/

   //DBG_PRINT(">>\r\n");
	if ((msgType == RVSIP_METHOD_INVITE) )
	{
      RvSipOtherHeaderHandle otherHeader;

      char buf[129]; // 128+1 = max Acl User-Agent + 1 for null terminatino

      //DBG_PRINT("RVSIP_METHOD_INVITE\r\n");

      RvSipOtherHeaderConstructInMsg(hMsg,RV_FALSE, &otherHeader);

      memset(buf, 0, sizeof(buf));
      acl_sip_call_mngr_user_agent_get(buf);
      if ( buf[0] )
      {
         RvSipOtherHeaderParse(otherHeader,buf);
      }
      else
      {
         DBG_PRINT("NO User-Agent\r\n");
      }

   }
   //DBG_PRINT("<<\r\n");

	return rvTrue;
}

extern void acl_message_print(IN RvSipMsgHandle hMsg, char *out, int maxsize);
#define LOG_SEPARATOR_START_RX "----------   RX SIP MESSAGE   ----------\n"
#define LOG_SEPARATOR_START_TX "----------   TX SIP MESSAGE   ----------\n"
#define LOG_SEPARATOR_END_RX      "---------- END OF RX SIP MESSAGE ----------\n"
#define LOG_SEPARATOR_END_TX      "---------- END OF TX SIP MESSAGE ----------\n"

// ER06 - Adding callbacks for logging of SIP messages
/******************************************************************************
*  acl_sip_call_mngr_post_mgs_tx - Implementation of RvIppSipPostMsgToSendCB
*  ----------------------------
*  General :        Prototype for SipPostMsgToSendCB.  Invoked before the IPP
*                  modifies the message about to be sent. Enables the application
*                  to change the outgoing SIP message.
*
*  Return Value:    None.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          sipMgrHndl         pointer to sipMgr handle.
*       .          hCallLeg           Call-Leg handle
*                  phAppCallLeg       Application call-leg handle
*                  hMsg               sip message handle
*                  userData           user application data.

*  Output          None.
*
******************************************************************************/
/*static void acl_sip_call_mngr_post_mgs_tx(  removed static becuase it's used in sipRegClient.c ACL NB 070118 */
void acl_sip_call_mngr_post_mgs_tx(
									    RvIppSipControlHandle		sipMgrHndl,
										RvSipCallLegHandle			hCallLeg,
										IN  RvSipAppCallLegHandle   hAppCallLeg,
										IN  RvSipMsgHandle          hMsg,
										void* userData)
{	/* AlexChen SDK upgrade 20090408 */
	RvSipCallLegState state;
    RvUint16 					responseCode;
    RvSipMethodType method;


	responseCode = RvSipMsgGetStatusCode(hMsg);

	if(SIPLogLevel>0)
	{
		strcpy(logmsg.message,LOG_SEPARATOR_START_TX);
		acl_message_print(hMsg, logmsg.message+strlen(LOG_SEPARATOR_START_TX),
			MAX_SIP_LOG_MSG_SIZE-strlen(LOG_SEPARATOR_START_TX)-strlen(LOG_SEPARATOR_END_TX));
		strcat(logmsg.message,LOG_SEPARATOR_END_TX);
		logmsg.size = strlen(logmsg.message);

		dsp_msg_log(&logmsg);
	}

	RvSipCallLegGetCurrentState(hCallLeg, &state);

	if( responseCode==486 && state == RVSIP_CALL_LEG_STATE_OFFERING )
	{
		RvIppTerminalHandle termHndl = rvIppSipControlGetMdmTerminal(hAppCallLeg);
		RvMdmTerm*    mdmTerm = rvIppMdmTerminalGetMdmTerm(termHndl);
		acl_analog_line_t *ep;
		acl_line_t *conn;

		ep = rvMdmTermGetUserData(mdmTerm);
		conn = (acl_line_t *)ep->data;

		if( conn->is_dnd_activated )
		{
		   RvSipOtherHeaderHandle otherHeader;

		   RvSipOtherHeaderConstructInMsg(hMsg, RV_FALSE, &otherHeader);
		   RvSipOtherHeaderSetName(otherHeader, "Reason");
		   RvSipOtherHeaderSetValue(otherHeader, "Q.850 ;cause=16");
		   RvSipMsgSetStatusCode(hMsg,603,RV_TRUE);
		}
	}

	/*ACL XY 081204 VI 67392  - Anatel certification,Ensure that the IUT, once a dialog has been
       established, on receipt of a BYE request including maddr parameters in the From and To headers
       ignores them and sends a Success (200 OK) response without maddr parameter.*/
       method = (RvSipMethodType)acl_voip_sip_msg_method_type_get((acl_voip_msg_t *)hMsg);

       if (method == RVSIP_METHOD_BYE)
       {
          if (responseCode == 200)
          {
             RvSipPartyHeaderHandle  hFromHeader;
             RvSipPartyHeaderHandle  hToHeader;
             RvSipAddressHandle hAddress;

             hFromHeader = RvSipMsgGetFromHeader(hMsg);
             hToHeader = RvSipMsgGetFromHeader(hMsg);

             if (hFromHeader == NULL || hToHeader == NULL)
             {
                DBG_PRINT("[%s:%d]RvSipMsgGetHeader failed",__FILE__, __LINE__ );
                return ;
             }

             /*According to Table 1, section 19.1 from RFC 3261, the maddr parameter is not allowed
                in the From header of BYE*/
             hAddress = RvSipPartyHeaderGetAddrSpec(hFromHeader);
             RvSipAddrUrlSetMaddrParam(hAddress, NULL);

             hAddress = RvSipPartyHeaderGetAddrSpec(hToHeader);
             RvSipAddrUrlSetMaddrParam(hAddress, NULL);

             return ;
          }

       }
	//acl_message_print(hMsg, NULL, 0);

}

/******************************************************************************
*  acl_sip_call_mngr_pre_mgs_rx - Implementation of RvIppSipPreMsgReceivedCB
*  ----------------------------
*  General :        Prototype for SipPreMsgReceivedCB. Invoked before the IPP
*                  modifies the message received. Enables the application
*                  to change the incomming SIP message.
*
*  Return Value:    RV_MTF_IGNORE_BY_STACK - if you don't want the IPP to performs its default processing.
*                   RV_MTF_IGNORE_BY_MTF - This value indicates to the MTF to ignore the message, but message will still be processed by the SIP Stack.
*                   RV_MTF_DONT_IGNORE - This value indicates both SIP Stack and MTF to process the message.
*
*
*  ----------------------------------------------------------------------------
*  Input:          sipMgrHndl         pointer to sipMgr handle.
*       .          hCallLeg           Call-Leg handle
*                  phAppCallLeg       Application call-leg handle
*                  hMsg               sip message handle
*                  userData           user application data.

*  Output          None.
*
******************************************************************************/
/* static RvBool acl_sip_call_mngr_pre_mgs_rx(  removed static becuase it's used in sipRegClient.c ACL NB 070118 */
/*--------------------------------------------------------------*/
/* Incident #129208: Implementation of RvIppSipPreMsgReceivedCB */
/* received 12-Apr-07 ACL NB 070619 */
RvMtfMsgProcessType acl_sip_call_mngr_pre_mgs_rx(
										RvIppSipControlHandle		sipMgrHndl,
										RvSipCallLegHandle			hCallLeg,
										IN  RvSipAppCallLegHandle   hAppCallLeg,
										IN  RvSipMsgHandle          hMsg,
										void* userData)
{
	RvUint16 					responseCode;
    char *ptr;
    int n;
    char sipbuffer[1024];


	responseCode = RvSipMsgGetStatusCode(hMsg);
	RVAPI RvInt16 RVCALLCONV RvSipMsgGetStatusCode(IN RvSipMsgHandle hSipMsg);
	printf(" *********************************IN SIP CALL manager pre msg %d \n",responseCode);




	if(SIPLogLevel>0)
	{
		strcpy(logmsg.message,LOG_SEPARATOR_START_RX);
		acl_message_print(hMsg, logmsg.message+strlen(LOG_SEPARATOR_START_RX),
			MAX_SIP_LOG_MSG_SIZE-strlen(LOG_SEPARATOR_START_RX)-strlen(LOG_SEPARATOR_END_RX));
		strcat(logmsg.message,LOG_SEPARATOR_END_RX);
		logmsg.size = strlen(logmsg.message);
        strcpy(sipbuffer,logmsg.message);

        if(strstr(sipbuffer,"BYE sip:")>0)
        {
            if(strstr(sipbuffer,"Asterisk-HangupCauseCode: 21")>0)
            {
                printf("PHIIIIIIIIIIII %s\n",sipbuffer);
                n=send(g_uiRv_sock_cli_fd,"TERMINATED_FOR_CONF_CALL",32,0);

            }

        }

		dsp_msg_log(&logmsg);
	}



	srand(time(NULL));

//      	printf("\n\nresponseCode=%d\r\n\n",responseCode);

	if(responseCode == 491)
	{
		RvMdmTerm 		*pTerm=NULL;

		RvInt32			msecDelay;
		int 				firstNumberInRange = 0;
		int 				secondNumberInRange = 100;

		if(acl_handle_491_reply_received(hMsg, &pTerm) != RV_Success)
		{
   			return RV_MTF_DONT_IGNORE;
		}

		msecDelay = firstNumberInRange + (rand( ) % (secondNumberInRange-firstNumberInRange+1));
//		printf("\nmsecDelay = %d\n", msecDelay);

		IppTimerConstruct(&myTimer, msecDelay, myProcessTimer, pTerm);

        	IppTimerStart(&myTimer, IPP_TIMER_RESTART_IF_STARTED, 0);

	}

	/* AlexChen SDK upgrade 20090415 */
	/*ACL XY 081230  pre-condition:local side fax transport mode is configured to Fax Relay,
      mode transport mode is configured to transparent.Remote side configures to transparent.
      Due to remote side does`t support T38 mode,we got 488 error here   */
   if( responseCode == 488 )
   {
      RvIppTerminalHandle termHndl = rvIppSipControlGetMdmTerminal(hAppCallLeg);
      RvMdmTerm*          mdmTerm = rvIppMdmTerminalGetMdmTerm(termHndl);
      acl_analog_line_t *ep=NULL;
      acl_line_t     *conn = NULL;
      int is_fax_relay_detected;

      ep = rvMdmTermGetUserData(mdmTerm);
      if( ep )
         conn         =(acl_line_t *)ep->data;

      is_fax_relay_detected = acl_line_fax_relay_deteceted_get(conn);

      if(is_fax_relay_detected == 1)
      {
          RvSdpMsg  sdpMsg;

          /*ACL XY 081218 - remote side does`t support T38,we send a re-invite with audio+711*/
          sip_fax_local_sdp_build(mdmTerm, &sdpMsg, -1,3 );

          rvMdmTermModifyMedia( mdmTerm, &sdpMsg);
          rvSdpMsgDestruct(&sdpMsg);

          /*ACL XY 081218 - configure the DSP for transparent.*/
          term_call_mngr_cbs->start_fax_fallback(ep->data);
      }
   }

#if 0  // we don't need to changed or behavior ACL 070620
   RvSipMethodType method;
   RvUint16 responseCode;

   /* ACL NB 070524 - VI 53515 */
   method = (RvSipMethodType)acl_voip_sip_msg_method_type_get((acl_voip_msg_t *)hMsg);
   DBG_PRINT("method=%d\r\n", method);
   if (method == RVSIP_METHOD_INVITE)
   {
      responseCode = RvSipMsgGetStatusCode(hMsg);

      DBG_PRINT("RVSIP_METHOD_INVITE responseCode=%d\r\n",responseCode);

      if (responseCode == 400)
      {
         RvIppTerminalHandle termHndl = rvIppSipControlGetMdmTerminal(hAppCallLeg);
        	RvMdmTerm*          mdmTerm = rvIppMdmTerminalGetMdmTerm(termHndl);

         return RV_MTF_DONT_IGNORE;/*RV_MTF_IGNORE_BY_STACK*/
      }

   }
#endif

   //acl_message_print(hMsg, NULL, 0);

   return RV_MTF_DONT_IGNORE;
}
/*AlexChen 20090409 SDK upgrade ACL XY 080917 - VI66797: problems with T.38 fax transmissions: there are times that the fax is detected before the OK message from the remote.
In this case, we will use a timer to check when the OK message arrives. Then, we will send the Image Re-Invite*/
void acl_sip_call_mngr_post_mgs_rx(IN RvIppSipControlHandle sipMgrHndl,
						   IN RvSipCallLegHandle	hCallLeg,
						   IN RvSipAppCallLegHandle hAppCallLeg,
						   IN RvSipMsgHandle        hMsg,
						   IN void*					userData)
{
		RvUint16                    responseCode;

		responseCode = RvSipMsgGetStatusCode(hMsg);
printf(" &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& IN SIP CALL manager pre msg %d \n",responseCode);

		if( responseCode == 200 )
		{

			RvIppTerminalHandle termHndl = rvIppSipControlGetMdmTerminal(hAppCallLeg);
			RvMdmTerm*          mdmTerm = rvIppMdmTerminalGetMdmTerm(termHndl);
			acl_analog_line_t		*ep=NULL;
			acl_line_t     *conn = NULL;
			int is_fax_relay_detected_wait_for_ok;

			ep = rvMdmTermGetUserData(mdmTerm);
			if( ep )
				conn         =(acl_line_t *)ep->data;

          is_fax_relay_detected_wait_for_ok = acl_line_fax_relay_detected_wait_for_ok_get(conn);
          DBG_PRINT("check if is_fax_relay_detected_wait_for_ok = 1\r\n");
          if(is_fax_relay_detected_wait_for_ok == 1)
          {
             RvSdpMsg	sdpMsg;
			 DBG_PRINT("is_fax_relay_detected_wait_for_ok = 1\r\n");
             /*mdm_faxDetect will failed to be sent re-invite if did`t get 200 ok,
                in this case,we should send re-invite again.*/
             sip_fax_local_sdp_build(mdmTerm, &sdpMsg, -1,1 );

             rvMdmTermModifyMedia( mdmTerm, &sdpMsg);
             rvSdpMsgDestruct(&sdpMsg);
             DBG_PRINT("set to 0\r\n");
             acl_line_fax_relay_detected_wait_for_ok_set(conn,0);
          }
       }
}

/***************************************************************/
/* ACL NB 070327 - the following 3 functions are part of Patch */
/* received from RV at 25-Mar-07 Incident #128937: CANCEL      */
/***************************************************************/

/***************************************************************************
 * RvSipCallLegNestedInitialReqRcvdEv
 * ------------------------------------------------------------------------
 * General: This callback indicates that a nested (second or more) INVITE request
 *          was received, on a call-leg that is not connected yet.
 *          (The second INVITE request has a different cseq value,
 *          so it was not rejected by the transaction layer, according to
 *          transaction merging rules).
 *          When this callback is called the application can order the stack
 *          whether to create a new call-leg for this nested request,
 *          or not, and then the request will be rejected by the stack.
 *
 * Return Value: RvStatus. If the application returns a value other then
 *               RV_OK the message will not be sent.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:     hExistCallLeg     - The exists call-leg handle.
 *            hExistAppCallLeg  - The application handle for the exists call-leg.
 *            hRcvdMsg          - The handle to the nested INVITE message.
 * Output:    pbCreateCallLeg   - RV_TRUE will cause the creation of a new call-leg.
 *                                RV_FALSE will cause the rejection of the nested INVITE.
 ***************************************************************************/
static void RVCALLCONV sip_nested_initial_req_rcvd_event(
                         IN  RvSipCallLegHandle     hExistCallLeg,
                         IN  RvSipAppCallLegHandle  hExistAppCallLeg,
                         IN  RvSipMsgHandle         hRcvdMsg,
                         OUT RvBool                 *pbCreateCallLeg)
{
    RV_UNUSED_ARG(hExistCallLeg);
    RV_UNUSED_ARG(hExistAppCallLeg);
    RV_UNUSED_ARG(hRcvdMsg);

    /* When forking is done to the same point (meaning we receive 2 Invite messages
       with the same From, CSeq and CallId headers. In this case according to
       standard the second Invite should be rejected by 482. However, if there is a
       need to accept this Invite, the stack will create new call leg for the second
       Invite if bDisableMerging is set to True, and this parameter in this callback
       returns True.*/
    *pbCreateCallLeg = RV_TRUE;
}

static void callLeg_callback_functions_set(RvIppSipControlHandle sipMgrHndl)
{
	RV_Status               rv;
	RvSipCallLegMgrHandle   hCallLegMgr;
	RvSipStackHandle        hStack = rvIppSipControlGetStackHandle (sipMgrHndl);

	/*Set application callbacks in the structure
	* We use toolkit's structure containings all used stack clbks, in order
	* to add new callbacks and not overrun existing callbacks*/
	RvIppSipStackCallbacks* clbks = rvIppSipControlGetStackCallbacks( sipMgrHndl);

	if(clbks->sipCallLegEvHandlers.pfnNestedInitialReqRcvdEvHandler !=NULL)
	{
		DBG_PRINT("Override application call-leg callbacks");
	}
	clbks->sipCallLegEvHandlers.pfnNestedInitialReqRcvdEvHandler =	sip_nested_initial_req_rcvd_event;

	RvSipStackGetCallLegMgrHandle( hStack, &hCallLegMgr);
	rv = RvSipCallLegMgrSetEvHandlers( hCallLegMgr, &clbks->sipCallLegEvHandlers, sizeof(RvSipCallLegEvHandlers));

	if(rv != RV_Success)
	{
		DBG_PRINT("ERROR: Failed to set application call-leg callbacks");
		return;
	}

   return;
}

/******************************************************************************
*  sip_register_stack_events - Implementation of RvIppSipRegisterStackEventsCB
*  ----------------------------
*  General :       Prototype for RvIppSipRegisterStackEventsCB. I
*
*  Return Value:   None (void)
*
*
*  ----------------------------------------------------------------------------
*  Input:          sipMgrHndl         pointer to sipMgr handle.

*  Output          None.
*
******************************************************************************/
static void sip_register_stack_events(IN RvIppSipControlHandle sipMgrHndl)
{
    callLeg_callback_functions_set(sipMgrHndl);
}


static void key_event_process_cb(void *o)
{
    key_event_emulation_t *tmp;

    tmp = key_event_emulation_queue;
    key_event_emulation_queue = tmp->next;

    printf ("calling :  rvMdmTermProcessEvent(0x%p,%s,%s,%c ) \n ",tmp->term, tmp->package, tmp->id, tmp->key_id[0] );
    rvMdmTermProcessEvent(tmp->term, tmp->package, tmp->id, NULL, tmp->key_id[0] ? &(tmp->paramList) : NULL);

    if (tmp->key_id[0])
	   rvMdmParameterListDestruct(&tmp->paramList);

    free(tmp);

    if (key_event_emulation_queue)
         AC_TIMER_SET(0, key_event_process_cb, NULL);
}

static void key_event_process(char *package, char *id, char *key_id, RvMdmTerm *term)
{
   key_event_emulation_t **next = &key_event_emulation_queue;
   RvMdmParameterList *pParamList;

   printf("key_event_process  package=%s id=%s keyid %s\r\n", package, id, key_id );

   if((strlen(package) >= KEY_ID_MAX_SIZE) ||
      (strlen(id) >= KEY_ID_MAX_SIZE))
   {
        DBG_PRINT_ERR("Invalid input\n");
	return;
   }

   while (*next)
	   next = &(*next)->next;

   *next = AC_ZALLOC_L(sizeof(key_event_emulation_t));
   if (!*next)
	   return;

   (*next)->term = term;
   strcpy((*next)->package, package);
   strcpy((*next)->id, id);
   if (key_id)
   {
      strcpy((*next)->key_id, key_id);

      pParamList = &(*next)->paramList;
      rvMdmParameterListConstruct(pParamList);

      if ( !strncmp(key_id, "cfw", 3 ) )
         sip_cfw_param_add(pParamList, "keyid", key_id);
      else
	      sip_param_add(pParamList, "keyid", key_id);
   }
   if (next == &key_event_emulation_queue)
      AC_TIMER_SET(0, key_event_process_cb, NULL);

   printf("returned \n ");

}

static void key_sw_event_process_cb(void *o)
{
    key_event_emulation_t *tmp;


    tmp = key_event_emulation_queue;
    key_event_emulation_queue = tmp->next;


    DBG_PRINT(">>>> package = \"%s\", id = \"%s\"", tmp->package, tmp->id);
    rvMdmTermProcessEvent(tmp->term, tmp->package, tmp->id, NULL, tmp->key_id[0] ? &(tmp->paramList) : NULL);

    //DBG_PRINT("key_event_process_cb tmp->package=%s id=%s %d\r\n", tmp->package, tmp->id , __LINE__);

    if (tmp->key_id[0])
	   rvMdmParameterListDestruct(&tmp->paramList);

    free(tmp);

    if (key_event_emulation_queue)
    {
        DBG_PRINT("key_event_emulation_queue not empty");
        AC_IPC_TIMER_SET(300, key_sw_event_process_cb, NULL);
    }
    DBG_PRINT("<<<<");
}


static void key_sw_event_process(char *package, char *id, char *key_id, RvMdmTerm *term)
{
   key_event_emulation_t **next = &key_event_emulation_queue;
   RvMdmParameterList *pParamList;

   //DBG_PRINT("key_event_process  package=%s id=%s keyid %s\r\n", package, id, key_id );

   DBG_PRINT(">>>> package = \"%s\", key_id = \"%s\"", package, key_id);

   if((strlen(package) >= KEY_ID_MAX_SIZE) ||
      (strlen(id) >= KEY_ID_MAX_SIZE) )
   {
        DBG_PRINT_ERR("Invalid input\n");
	return;
   }

   while (*next)
	   next = &(*next)->next;

   *next = AC_ZALLOC_L(sizeof(key_event_emulation_t));
   if (!*next)
	   return;

   (*next)->term = term;
   strcpy((*next)->package, package);
   strcpy((*next)->id, id);
   if (key_id)
   {
      strcpy((*next)->key_id, key_id);

      pParamList = &(*next)->paramList;
      rvMdmParameterListConstruct(pParamList);

      if ( !strncmp(key_id, "cfw", 3 ) )
         sip_cfw_param_add(pParamList, "keyid", key_id);
      else
	      sip_param_add(pParamList, "keyid", key_id);
   }
   if (next == &key_event_emulation_queue)
   {
      DBG_PRINT("first element in key_event_emulation_queue");
      AC_IPC_TIMER_SET(300, key_sw_event_process_cb, NULL);
   }
   DBG_PRINT("<<<< package = \"%s\", key_id = \"%s\"", package, key_id);

}


#ifdef CONFIG_RG_ATA
static void local_conference_process(RvMdmTerm *term)
{
    /* deliver transfer event, unless we are in line2 without an active
    * connection yet; in this case we just go back to line1 */
    int connState;

    DBG_PRINT(">>>> \r\n");

#ifdef ACL_SIP_CALL_MNGR_DEBUG
    int lineId = active_line_Id_get(term);
#endif

#ifdef AC488_CPE_VOIP_TOOLKIT
	if(!isOtherTermInIdleState())
	{
    		printf("Can't perform local conferencing since the other line is not in idle state\r\n");
		return;
	}
#endif

    /* ACL NB 060619 - fixd bug of semi attended transfer*/
    connState = connection_state_get(term);
    DBG_PRINT("state %d active line=%d\r\n", connState, lineId);
    /* ACL NB 070123 - in case of conference we can't and we should go back to the other line*/
    if ( connState == RV_CCCONNSTATE_CONNECTED ||
         connState == RV_CCCONNSTATE_CALL_DELIVERED)
    {
        key_event_process("kf", "ku", "kc", term);
    }
    else
    {
        /* ACL NB - 061116 -  Flash only keys sequence style !!!                             */
        /* we should check the call state there are states that we can't process Transfer    */
        /*   but we must process ONHOOK that useually we simulate at the end of the transfer */
        /*   process.                                                                        */
        if (call_mngr_params->keys_sequence==KEY_SEQ_FLASH_ONLY)
        {
            if ( !( connState == RV_CCCONNSTATE_INPROCESS ) )/* ACL NB 070123 - we can't make conf and we should go back to the other line*/
            {
                key_event_process("al", "on", NULL, term);
                DBG_PRINT("conf \r\n");
            }
			else
            {
               key_event_process("kf", "ku","l001", term);
               DBG_PRINT("regret \r\n");
            }
        }
        else
            key_event_process("kf", "ku","l001", term);
    }
}

#endif /*CONFIG_RG_ATA*/

static void remote_conference_process(RvMdmTerm *term)
{
    DBG_PRINT(">>>> rcState = %d\r\n", remote_conference_state_get(term));
    /*
        Remote conference is having a 3 way conferecne using a third party media server for the mixing session.
        It can be carried out on the same conditions as the local conference (what we have today) i.e. 2 calls: one
        on hold and one active.
        The remote conference procedure include 3 logical steps and in fact 6:
        1. Blind transfer the second call (active call to C) to the media server.
        2. Take the held call (call with B) out of hold and make it your active call.
        3. Blind transfer the first call (active call to B) to the media server.
        Due to a bug in Rv where they dont disconnect the call that was blind transferred do 4 and 5.
        4. Simulate onhook on the call with B. (the call with C becomes your active).
        5. Simulate onhook on the call with C.
        6. Make a call to the media server.
    */
    /*we handle the state machine inside this function*/
    switch (remote_conference_state_get(term))
    {
    case REMOTE_CONF_NOT_IN_PROGRESS:
        /*step 1 - send a Blind tranfer event on first call*/
        key_sw_event_process("kf", "ku", "kbt", term);
        DBG_PRINT("APP-->Sending Event: BLIND_TRANSFER");
        remote_conference_state_set(term, REMOTE_CONF_FIRST_CALL_TRANS);
        break;
    case REMOTE_CONF_FIRST_CALL_TRANS:
        /*step 3 - send a Blind tranfer event on second call*/
        key_sw_event_process("kf", "ku", "kbt", term);
        DBG_PRINT("APP-->Sending Event: BLIND_TRANSFER");
        remote_conference_state_set(term, REMOTE_CONF_SECOND_CALL_TRANS);
        break;
    case REMOTE_CONF_SECOND_CALL_TRANS:
        /*step 4 - release first line*/
        key_sw_event_process("al", "on", NULL, term);
        DBG_PRINT("APP-->Sending Event: ONHOOK");
        remote_conference_state_set(term, REMOTE_CONF_DISCONNECT_FIRST);
        break;
    case REMOTE_CONF_DISCONNECT_FIRST:
        /*step 5 - release second line*/
        key_sw_event_process("al", "on", NULL, term);
        DBG_PRINT("APP-->Sending Event: ONHOOK");
        remote_conference_state_set(term, REMOTE_CONF_DISCONNECT_SECOND);
        break;
    case REMOTE_CONF_DISCONNECT_SECOND:
        /*step 6 - send an offhook event for calling the media server == putting yourself in the conference */
        key_sw_event_process("al", "of", NULL, term);
        DBG_PRINT("APP-->Sending Event: OFFHOOK");
        break;
    case REMOTE_CONF_IS_ON:
        /*  this will probably happen when the call is disconnected from the conference - in this case
            go back to the begining == no remote conference */
        remote_conference_end(term);
        break;
    default:
        DBG_PRINT("unsupported state\r\n");
    }
    DBG_PRINT("<<<< rcState = %d\r\n", remote_conference_state_get(term));
}

#ifdef CONFIG_RG_ATA
static void acl_sip_call_mngr_conference_process(RvMdmTerm *term)
{
    int remoteConference;

    DBG_PRINT(" CONFERENCE\r\n");
    voip_conf_remote_conference_get(&remoteConference);
    if (1 == remoteConference)
    {
        remote_conference_init(term);
        remote_conference_process(term);
    }
    else /*local conference*/
        local_conference_process(term);
}

static int ata_process_event(int event, RvMdmTerm *term, int down)
{
    DBG_PRINT("ata_process_event  event = %d\r\n", event);
#ifdef CONFIG_RG_ATA
    char id[KEY_ID_MAX_SIZE];
    char *package = "dd";
    int connState;// ACL NB 061116
    RvCCTerminal *xTerm = (RvCCTerminal*)term->xTerm;/* NIRP 070607 */
    int lineIDFromTerm=line_get_by_term(xTerm);/* NIRP 070607 */
    int lineId = active_line_Id_get(term);
    int lineIdx = lineId- 1;

    switch (event)
    {
    case VOIP_INPUT_0:
    case VOIP_INPUT_1:
    case VOIP_INPUT_2:
    case VOIP_INPUT_3:
    case VOIP_INPUT_4:
    case VOIP_INPUT_5:
    case VOIP_INPUT_6:
    case VOIP_INPUT_7:
    case VOIP_INPUT_8:
    case VOIP_INPUT_9:
	    if (!down)
    	    return 0;
/* ACL NB 070521 - Secondary Dial Tone */
      if (call_mngr_params->use_secondary_dial_tone)
      {
         acl_analog_line_t *analogLine;
         char           *dialStr;
         RvCCTerminal   *xTerm = (RvCCTerminal*)term->xTerm;

         analogLine = rvMdmTermGetUserData(term);
    	   dialStr = (char *)rvCCTerminalGetDialString(xTerm);

         DBG_PRINT("on=%d  dialStr = %s(len=%d) lineId=%d\r\n", line_status[lineIdx].secondary_dial_tone_on, dialStr, strlen(dialStr),lineId );
         if ( !line_status[lineIdx].secondary_dial_tone_on  &&  (!dialStr || !strlen(dialStr) ))
        	{
            if (call_mngr_params->secondary_dial_tone_key== event)
            {
               term_call_mngr_cbs->start_signal(analogLine->data, VOIP_SIGNAL_DIAL_TONE, NULL); // TBD secondary dial tone !!!
               line_status[lineIdx].secondary_dial_tone_on = 1;
               return 0;
            }
         }
         else
            line_status[lineIdx].secondary_dial_tone_on =  0;

      }
	    sprintf(id, "d%d", event);
	break;
    case VOIP_INPUT_ASTERISK:
	if (!down)
	    return 0;
	strcpy(id, "ds");
	break;
    case VOIP_INPUT_POUND:
	if (!down)
	    return 0;
	strcpy(id, "do");
	break;
	/* Eitan1207 - Add support for DTMF A-D */
    case VOIP_INPUT_A:
    case VOIP_INPUT_B:
    case VOIP_INPUT_C:
    case VOIP_INPUT_D:
 		if (!down)
			return 0;
		sprintf(id, "d%c",event-VOIP_INPUT_A+'a');
		break;
    case VOIP_INPUT_OFFHOOK:
		strcpy(id, "of");
		package = "al";
		DBG_PRINT("OFFHOOK \r\n");
		/* ACL NB 070405 VI#51173 */
		/* If there was a call waiting we should go back to it (in the future  */
		/* we will add CWRR that will remind that there is a call              */
		connState = connection_state_get(term);
		DBG_PRINT("State %s  \r\n", acl_voip_cc_conn_state_text(connState) );

		if (AC_IPC_TIMER_EXISTS(aclRegretCallTimeout, (void *)term))
		{
			DBG_PRINT("- Delete call regret timeout\r\n");

			AC_IPC_TIMER_DEL(aclRegretCallTimeout, (void *)term);
			//regretCallHookOff=1;
			return 0;
		}
		else if ( connState == RV_CCCONNSTATE_CONNECTED )
		{
			key_event_process("kf", "ku", lineId==2 ? "l002" : "l001", term);
		}

		break;
    case VOIP_INPUT_ONHOOK:
       DBG_PRINT("ONHOOK \r\n");
	strcpy(id, "on");
    	package = "al";

	break;
    case VOIP_INPUT_FLASH:
       /* ------------------------------------------------------*/
       /*   ACL NB 070516 - AC#53280 Send flash-hook to proxy   */
       /* ------------------------------------------------------*/
       DBG_PRINT("Hook-flash\r\n");
       key_event_process("user", "flash", NULL, term);
       return 0;
       break;

    case VOIP_INPUT_HOLD:

        /* if you are in a conference ignore the flash key*/
        if (REMOTE_CONF_NOT_IN_PROGRESS != remote_conference_state_get(term))
        {
            DBG_PRINT("During 3 way conference \r\n");
            return 0;
        }

        DBG_PRINT("HOLD active line=%d \r\n",lineId);
       /* ACL NB 061116 hold can be premormed only on connected call - */
       connState = connection_state_get(term);
       /*if ( connState == RV_CCCONNSTATE_CONNECTED ) ACL NB 070613 */
       if (( connState != RV_CCCONNSTATE_CALL_DELIVERED ) &&
           ( connState != RV_CCCONNSTATE_INPROCESS ) )
       {
	       key_event_process("kf", "ku", "kl", term);
	       key_event_process("kf", "ku", lineId==1 ? "l002" : "l001", term);
       }
       else
       {
            DBG_PRINT("Call is not connected yet. connState=%s.. \r\n", acl_voip_cc_conn_state_text(connState));
         key_event_process("al", "on", NULL, term);
        key_event_process("kf", "ku", lineId==1 ? "l002" : "l001", term);
       }
	 return 0;

    case VOIP_INPUT_TRANSFER:
       /* ACL NB  060628 - cannot perform transfer if there are already 2 active calls */
       DBG_PRINT("TRANSFER \r\n");
	/* deliver transfer event, unless we are in line2 without an active
	 * connection yet; in this case we just go back to line1 */

       connState = connection_state_get(term);
       DBG_PRINT(" state %d active line=%d\r\n", connState, lineId);
       /* ACL NB 070123 - in case of conference we can't and we should go back to the other line*/
       if ( connState == RV_CCCONNSTATE_CONNECTED      ||
            connState == RV_CCCONNSTATE_INPROCESS      ||
            connState == RV_CCCONNSTATE_CALL_DELIVERED)
       {
          key_event_process("kf", "ku", "kt", term);
       }
       else
       {
          /* ACL NB - 061116 -  Flash only keys sequence style !!!                             */
          /* we should check the call state there are states that we can't process Transfer    */
          /*   but we must process ONHOOK that useually we simulate at the end of the transfer */
          /*   process.                                                                        */
          if (call_mngr_params->keys_sequence != KEY_SEQ_FLASH_ONLY)
             key_event_process("kf", "ku","l001", term);
          else
          {
             DBG_PRINT("NO TRANSFER \r\n");
             key_event_process("al", "on", NULL, term);
          }
       }
       return 0;
      break;

    case VOIP_INPUT_CONF:
        acl_sip_call_mngr_conference_process(term);
	return 0;

   /* ACL NB 060404 */
   case VOIP_INPUT_FAX_DETECT:
      /* start fax in the dsp and will cause re-invite */
      DBG_PRINT("VOIP_INPUT_FAX_DETECT \r\n");
      fax_t38_detect_event_handler(term);
      return 0;

   /* ACL NB 060704 */
   case VOIP_INPUT_FAX_BYPASS_DETECT:

      DBG_PRINT("VOIP_INPUT_FAX_BYPASS_DETECT \r\n");
      /* will cause re-invite */
      fax_bypass_detect_event_handler(term);
      return 0;

    case VOIP_INPUT_CFW_ON: /* ACL NB 061113 call forward*/
       if (g_cfwParams.isEnabled )
       {
          DBG_PRINT(" ************* CFW  \r\n");
          /* ACL NB 061114 - The following line is a kind of PATCH !!             */
          /* --------------------------------------------------------             */
          /*   RADVISION activating CFW should be done in hook on state           */
          /*   means in idle state and not in  dialing state so we ne to simulate */
          /*   it by sending hook on event before the CFW event                   */
          key_event_process("al","on", NULL, term);
          key_event_process("kf", "ku","cfwon", term);
       }
       else
       {
          DBG_PRINT(" ************* CFW (on) is DISABLED \r\n");
       }

  	    return 0;
    case VOIP_INPUT_CFW_OFF: /* ACL NB 061113 call forward*/
       if (g_cfwParams.isEnabled )
       {
          DBG_PRINT(" ************* CFW  OFF \r\n");
          key_event_process("kf", "ku","cfwoff", term);
       }
       else
       {
          DBG_PRINT(" ************* CFW (off) is DISABLED \r\n");
       }

   	return 0;

    case VOIP_INPUT_DAA_RING_DETECT_START:
	{
		RvCCTerminal *pTerm = term->xTerm;
		DBG_PRINT("VOIP_INPUT_DAA_RING_DETECT_START: calling AclTerminalMakeCall ....\r\n");
		rvMdmTermProcessEvent(term, "al","of",  NULL, NULL);
#if 0
		AclTerminalMakeCall((RvIppTerminalHandle)pTerm ,mdm_params->lines[0].id);
#else
		//NIRP 070808 VI 56120: use a hard coded dial string which will be replaced in the
		//map_dial_string_to_addr_cb callback. The purpose is to avoid the need of
		//configuring a speed dial from line 2 (FXO) to line 1 (FXS)
		acl_make_call((RvIppTerminalHandle)pTerm ,"DialingFXO2FXS");
#endif
		//led signal fxo line ringing
		set_phone_led_to_state(e_LED_FXO, e_GREEN_LED_STATE_SLOW_FLASH);
	}

	return 0;

	case VOIP_INPUT_DAA_RING_DETECT_STOP:
	{
#ifdef ACL_SIP_CALL_MNGR_DEBUG
			RvCCTerminal *pTerm = term->xTerm;
#endif
		DBG_PRINT("VOIP_INPUT_DAA_RING_DETECT_STOP: simulating on hook .... %s, active line =%d .\r\n",
							rvCCTerminalMdmGetTermId(pTerm), active_line_Id_get(term));
		key_event_process("kf", "ku", "l002", term);//nirp T.B.D
		key_event_process("al", "on", NULL, term);
		/*JL VI # 62536 g_cCaller has regreted and hunged up - set led off */
		set_phone_led_to_state(e_LED_FXO, e_LED_STATE_OFF);

		return 0;
	}

	case PHONE_INPUT_DAA_FLASH_HOOK_RELAY://NIRP 070607
	{
		dsp_send_daa_commands_args_t arg;
		arg.command_enum=DSP_DAA_COMMANDS_FLASH_HOOK;
		arg.line=lineIDFromTerm;
		arg.channel=0;

		DBG_PRINT("PHONE_INPUT_DAA_FLASH_HOOK_RELAY: send a daa command .... .\r\n");
		term_call_mngr_cbs->send_daa_commands((void*)&arg);

		return 0;
	}


       /* ACL NB 070507 Blind Transfer */
    case VOIP_INPUT_BLINDT:
       {
          DBG_PRINT(" Blind Transfer active line=%d\r\n", lineId);
	       key_event_process("kf", "ku", "kl", term);
	       key_event_process("kf", "ku", lineId==1 ? "l002" : "l001", term);

          //isBlindTrans = 1;
          AC_TIMER_SET(900, acl_sip_call_mngr_blind_transfer_process, term);


          return 0;
#if 0 /*EA - 071217 I decided to leave this - it is for implementing blind transfer*/
            key_event_process("kf", "ku", "kbt", term);
            return 0;
#endif
       }

       /* ACL NB 070806 */
    case VOIP_INPUT_SWITCH2VOICE_DETECT:
       /* stop fax in the dsp and  cause re-invite */
       switch_2_voice_detect_event_handler(term);
       return 0;
	case VOIP_INPUT_DND:
       DBG_PRINT("[%s:%d] VOIP_INPUT_DND\r\n", __FUNCTION__, __LINE__);
       rvIppMdmTerminalResetDialString((RvIppTerminalHandle)term->xTerm);
       return 0;
    default:
      DBG_PRINT("unkonow key event %d \r\n",event);
		return -1;
      break;
    }

    key_event_process(package, id, NULL, term);
    if (!strcmp(package,"dd") )
        /*   !!!!!  ACL NB 060723 - to support DTMF via SIP              */
        /*  by RADVISION - Incident #126956: Out-of-band DTMF for analog */
        key_event_process("dd-end", id, NULL, term);

#endif
    return 0;
}

#elif CONFIG_RG_IPPHONE /*CONFIG_RG_ATA*/

static int ipphone_process_event(int event, RvMdmTerm *ui, int down)
{
    DBG_PRINT("ipphone_process_event  event = %d\r\n", event);
    char key_id[KEY_ID_MAX_SIZE];
    char *package = "kp";

    switch (event)
    {
    case VOIP_INPUT_0:
    case VOIP_INPUT_1:
    case VOIP_INPUT_2:
    case VOIP_INPUT_3:
    case VOIP_INPUT_4:
    case VOIP_INPUT_5:
    case VOIP_INPUT_6:
    case VOIP_INPUT_7:
    case VOIP_INPUT_8:
    case VOIP_INPUT_9:
	sprintf(key_id, "k%d", event);
	break;
    case VOIP_INPUT_ASTERISK:
	strcpy(key_id, "ks");
	break;
    case VOIP_INPUT_POUND:
	strcpy(key_id, "ko");
	break;
    /*NirS:*/
    case VOIP_INPUT_LINE_1:
		if (!down)
		{
			if (active_line_Id_get(ui)==2)
			{
				//simulate hold press (hold line 2)
				key_event_process("kf","kd" ,"kl",ui);
				key_event_process("kf","ku","kl",ui);
			}
		}
		strcpy(key_id,"l001");
		package="kf";
		break;

    case VOIP_INPUT_LINE_2:
		if (!down)
			if (active_line_Id_get(ui)==1)
			{
				//simulate hold on line 1
				key_event_process("kf","kd","kl",ui);
				key_event_process("kf","ku","kl",ui);
			}
		package="kf";
		strcpy(key_id,"l002");
		break;
    /*End NirS*/

    case VOIP_INPUT_ONHOOK:
	/* If onhook pressed, and active line is not default line, so before
	 * needs return to default line */
	if (active_line_Id_get(ui) == 2)
	{
	    key_event_process("kf", "kd", "l001", ui);
	    key_event_process("kf", "ku", "l001", ui);
	}
	/* falls into */
    case VOIP_INPUT_OFFHOOK:
	strcpy(key_id, "kh");
	package = "kf";
	break;

    case VOIP_INPUT_HOLD:
	strcpy(key_id, "kl");
	package = "kf";

       key_event_process(package, down ? "kd" : "ku", key_id, ui);

	if (connection_state_get(ui)!=RV_CCCONNSTATE_IDLE)
	/*there are more states in addition to IDLE that we should not simulate line press!!!*/
	{
       	if (!down)
       	{
			 if (active_line_Id_get(ui) == 1)
	       	{
	  			key_event_process("kf", "kd", "l002", ui);
	  			key_event_process("kf", "ku", "l002", ui);
	      	 	}
		 	if (active_line_Id_get(ui) == 2)
		 	{

	  			key_event_process("kf", "kd", "l001", ui);
	  			key_event_process("kf", "ku", "l001", ui);
		 	}
       	}
	}

	return 0;


    case VOIP_INPUT_TRANSFER:
		strcpy(key_id, "kt" );
		package = "kf";
		break;

    case VOIP_INPUT_CONF:
	{
	    /* Radvision's toolkit build in that way, that if voice connection
	     * with third partner is not established (or disconnected), user
	     * must click button with line number 1 (default line), to return
	     * to previous partner.
	     * In ipphone user operates only one button. So if user clicks key
	     * "transfer", we are testing current active line.
	     * If line is not 2, then transfer process is not started yet, and
	     * we translate this click as "kf\kh".
	     * If line is 2, then we are testing current state of the
	     * connection. If state is "connected", we translate this click
	     * as "kf\kh" to complete transfer. In other case we
	     * translate this click as "kf\l001" to return to default
	     * line
	     */
	    if (active_line_Id_get(ui) != 2 ||
		connection_state_get(ui) == RV_CCCONNSTATE_CONNECTED)
	    {
		strcpy(key_id, "kc");
	    }
	    else
		strcpy(key_id, "l001");
	}
	package = "kf";
	break;

    case VOIP_INPUT_SPEAKER:
		strcpy(key_id,"hf");
		package="kf";
		break;
    case VOIP_INPUT_HEADSET:
		strcpy(key_id,"ht");
		package="kf";
		break;

    default:
	/* No processing for other events */
	return -1;
    }

    key_event_process(package, down ? "kd" : "ku", key_id, ui);

    return 0;
}

#endif /*CONFIG_RG_IPPHONE*/

static int sip_process_event(int epn, voip_input_event_t event, int down)
{
    analogLine1 = analog_lines+epn;

    if (epn >= call_mngr_params->ep_num)
	return -1;

#ifdef CONFIG_RG_ATA
	return ata_process_event(event, analogLine1->line.ata.al, down);
#elif CONFIG_RG_IPPHONE
	return ipphone_process_event(event, analogLine1->line.ipphone.ui, down);
#else
	return -1;
#endif

}

#ifdef CONFIG_RG_IPPHONE
static void default_screen_show(RvMdmTerm *ui)
{
    RvCCTerminal *term = ui->xTerm;
    RvCCConnection *conn = rvCCTerminalGetActiveConnection(term);

    /* connection must be on state RV_CCCONNSTATE_IDLE, so parameters 'event'
     * and 'cause' are not used */
    sip_rvcc_display((RvIppConnectionHandle)conn, (RvIppTerminalHandle)term,
	0, 0, NULL);
}
#endif

static void single_analog_line_create(int i)
{
    RvMdmTermDefaultProperties term_properties;
    /* ACL NB - 060521 */
    RvMdmTermPresentationInfo  myInfo;


    printf("-> single_analog_line_create(%d) \n", i);


    /* skip disabled endpoints */
    if (!call_mngr_params->lines[i].enabled)
	return;

    /* create analog line termination. set user data to ep */
    rvMdmTermDefaultPropertiesConstruct(&term_properties);


    /* ACL NB  060322 - support server authentication per line*/
    /*DBG_PRINT("username %s, pswd %s \r\n", call_mngr_params->lines[i].usernameServerAuth, call_mngr_params->lines[i].pswdServerAuth); sleep(1);*/
    rvMdmTermDefaultPropertiesSetUsername(&term_properties, call_mngr_params->lines[i].usernameServerAuth);
    rvMdmTermDefaultPropertiesSetPassword(&term_properties, call_mngr_params->lines[i].pswdServerAuth);

    printf("Setting line %d to status %d\r\n", i, PORT_UNLOCK);

    SetLineStatus(i, PORT_UNLOCK);

#ifdef CONFIG_RG_ATA  //NirS

    analog_lines[i].line.ata.al = rvMdmTermMgrRegisterPhysicalTermination( &term_mgr,
                                                                     al_class, call_mngr_params->lines[i].id,&term_properties, NULL);
    rvMdmTermSetPhoneNumber(analog_lines[i].line.ata.al,call_mngr_params->lines[i].id);

    /* ACL NB - 060521 Radvision2.0 */
    memset(&myInfo, 0, sizeof(RvMdmTermPresentationInfo));
    myInfo.calleeIdPermission = RV_MDM_PRSENTATION_ALLOWED;

	/* ER - If g_cCaller id is disabled, change the display name to "Anonymous"*/
	if(call_mngr_params->lines[i].send_g_cCaller_id)
	{
		myInfo.g_cCallerIdPermission = RV_MDM_PRSENTATION_ALLOWED;
		strcpy( myInfo.presentationName,call_mngr_params->lines[i].display_name);
	}
	else
	{
		myInfo.g_cCallerIdPermission = RV_MDM_PRSENTATION_RESTRICTED;
		strcpy( myInfo.presentationName,ANONYMOUS_NAME);
	}

    rvMdmTermSetPresentationInfo(analog_lines[i].line.ata.al,	&myInfo );
    rvMdmTermSetUserData(analog_lines[i].line.ata.al, analog_lines+i);

    /* ACL NB  060926 added MWI support*/
    acl_adv_sip_register_mdm_term(i, analog_lines[i].line.ata.al);

    /* ACL NIRP 071128 add analog type (FXS/FXO) support */
    /* if FXO is present - it will always be the last line (termination) */
/*#ifdef ACL_SIP_APPLICATION*/
	/* only SMB's fxs/fxo in way 1fxs 1 fxo 1 fxs 1 fxo AlexChen 20090206 */
#if defined(AC49_SMB_VOIP_TOOLKIT) || defined(AC49_ORCHID_ATA_VOIP_TOOLKIT)
   if ( voip_conf_get_number_of_lines(ANALOG_PORT_TYPE_FXO) && (i%2==1) )
#else
	/* fxo will always be the last line AlexChen 20090206*/
   if (voip_conf_get_number_of_lines(ANALOG_PORT_TYPE_FXO) && (i == voip_conf_get_number_of_lines(ANALOG_PORT_TYPE_FXS)) )
#endif
    {
    	DBG_PRINT("-> single_analog_line_create fxo(%d) \n", i);
		analog_lines[i].enum_analog_port_type=ANALOG_PORT_TYPE_FXO;
    }
    else
    {
    	DBG_PRINT("-> single_analog_line_create fxs(%d) \n", i);
		analog_lines[i].enum_analog_port_type=ANALOG_PORT_TYPE_FXS;
		/* AlexChen SDK upgrade 20090415 */
		if(( call_mngr_params->sp.sip.use_proxy_port_ip_for_registrar && call_mngr_params->sp.sip.use_proxy) || call_mngr_params->sp.sip.use_registrar)
               rvMdmTermMgrRegisterTermToNetwork(&term_mgr, analog_lines[i].line.ata.al);
    }

#endif
#ifdef CONFIG_RG_IPPHONE
    analog_lines[i].line.ipphone.ui = rvMdmTermMgrRegisterPhysicalTermination(&term_mgr, ui_class,
	call_mngr_params->lines[i].id, &term_properties, NULL);

    rvMdmTermSetPhoneNumber(analog_lines[i].line.ipphone.ui,call_mngr_params->lines[i].id);

    rvMdmTermSetPresentationInfo(analog_lines[i].line.ipphone.ui,	&myInfo );

    analog_lines[i].line.ipphone.at =
	rvMdmTermMgrRegisterPhysicalTermination(&term_mgr, at_class,
	"at/hs", &term_properties, NULL);

    rvMdmTermSetUserData(analog_lines[i].line.ipphone.ui, analog_lines+i);
    rvMdmTermSetUserData(analog_lines[i].line.ipphone.at, analog_lines+i);

    /* ACL NB  060926 added MWI support*/
    acl_adv_sip_register_mdm_term(i, analog_lines[i].line.ipphone.ui);
    analog_lines[i].enum_analog_port_type=ANALOG_PORT_TYPE_FXS;

    default_screen_show(analog_lines[i].line.ipphone.ui);
    printf("IN CONFIG_RG_IPPHONE \n");

#endif
    rvMdmTermDefaultPropertiesDestruct(&term_properties);
    analog_lines[i].data = term_call_mngr_cbs->set_endpoint_context(i,
	call_mngr_params->lines[i].id, call_mngr_params->keys_sequence);
    analog_lines[i].send_caller_id = call_mngr_params->lines[i].send_caller_id;
    /* EA - 071203 - implementing remote conference RFC 4240*/
    analog_lines[i].remoteConferenceInfo = NULL;
}

int registration_to_sip_server()
{
    int  i=0;
    RvMdmTermDefaultProperties term_properties;
    RvMdmTerm *mdmterm, *mdmterm1;
    acl_analog_line_t *analogLine;




   /* rvMdmTermDefaultPropertiesConstruct(&term_properties);

    rvMdmTermDefaultPropertiesSetUsername(&term_properties, call_mngr_params->lines[i].usernameServerAuth);
    rvMdmTermDefaultPropertiesSetPassword(&term_properties, call_mngr_params->lines[i].pswdServerAuth);

    analog_lines[i].line.ipphone.ui = rvMdmTermMgrRegisterPhysicalTermination(&term_mgr, ui_class,call_mngr_params->lines[i].id,
                                      &term_properties, NULL);


    analog_lines[i].line.ipphone.at = rvMdmTermMgrRegisterPhysicalTermination(&term_mgr, at_class,
	"at/hs", &term_properties, NULL);
	 mdmterm = &analog_lines[i].line.ipphone.ui;
    // mdmterm1 = &analog_lines[i].line.ipphone.at;
   //  mdmterm1= &analog_lines[i].line.ipphone.ui;

    rvMdmTermSetUserData(mdmterm , analog_lines+i);
    rvMdmTermSetUserData(mdmterm , mdmterm1);

    //rvMdmTermSetUserData(mdmterm1 ,analog_lines+i);
    // acl_adv_sip_register_mdm_term(i, mdmterm);*/
    mdmterm = rvMdmTermGetUserData(&analogLine);

	printf("SENDING SIP REGISTRATION MANUALLY\n");
	//rvMdmTermMgrUnregisterTermFromNetwork(&term_mgr,&mdmterm);

    //rvMdmTermMgrRegisterTermToNetwork(&term_mgr,&mdmterm);
    //printf("below rvMdmTermMgrRegisterTermToNetwork ...........................\n");
    rvMdmTermMgrStart(&term_mgr, NULL, 0);
     printf("Below rvMdmTermMgrStart...................\n");
    // rvMdmTermMgrRegisterTermToNetwork(&term_mgr,&mdmterm1);
    return 0;


}


static int sip_analog_lines_create(void)
{
    int i;

    if (vcCONFIG_RG_IPPHONE && call_mngr_params->ep_num!=1)
	return -1;

    analog_lines = AC_ZALLOC_L(call_mngr_params->ep_num * sizeof(acl_analog_line_t));
    if (!analog_lines)
	return -1;

    for (i = 0; i < call_mngr_params->ep_num; i++)
	single_analog_line_create(i);
    return 0;
}

/* ACL NB 061115 - The following callback is called
   when the CFW activation process has been completed */
static void sip_cfw_activate_completed_cb( RvIppTerminalHandle   term,
                                 RvIppCfwType          cfwType,
                                 char*                 cfwDestination,
                                 RvIppCfwReturnReasons returnCode)
{
   RvCCTerminal *pTerm = (RvCCTerminal *)term;
   RvMdmTerm      *pMdmTerm;
   acl_analog_line_t *analogLine;
   int            line= 0;

   DBG_PRINT("CFW activation !!!!\r\n");

   if (!is_up)
   {
      DBG_PRINT("Debug: CFW activation line %d, after restart\r\n",line );
      return;
   }

	pMdmTerm= &(((RvCCTerminalMdm *)pTerm->terminal)->mdmTerm);

   /* The following 2 lines there is a use  of pointers arithmetics ... (Ugly) */
   analogLine = rvMdmTermGetUserData(pMdmTerm);

   line = (int)(analogLine - analog_lines);

   DBG_PRINT("Debug: CFW activation line %d, returnCode=%d\r\n",line , returnCode);
   if (cfwDestination)
   {
      if ( voip_conf_ext_cfw_dest_set(line, cfwDestination) != 0)
         DBG_PRINT_ERR("Warning CFW destination %s could not be kept in ext_conf to re-use after re-start \r\n",cfwDestination );
   }


   // the user should get any indication !!!
   if ( analogLine )
   {
      term_call_mngr_cbs->activate_cfw(analogLine->data, 1);
      term_call_mngr_cbs->start_signal(analogLine->data, VOIP_SIGNAL_STUTTER_TONE, NULL);
   }

   return;
}

/* ACL NB 061115 - The following callback is called when
   the CFW deactivation process has been completed       */
static void sip_cfw_deactivate_completed_cb(RvIppTerminalHandle   term,
                                         RvIppCfwType          cfwType,
                                         RvIppCfwReturnReasons returnCode)
{
   RvCCTerminal *pTerm = (RvCCTerminal *)term;
   RvMdmTerm      *pMdmTerm;
   acl_analog_line_t *analogLine;
   int            line= 0;

	pMdmTerm= &(((RvCCTerminalMdm *)pTerm->terminal)->mdmTerm);
   /* The following 2 lines there is a use  of pointers arithmetics ... (Ugly) */
   analogLine = rvMdmTermGetUserData(pMdmTerm);
   line = (int)(analogLine - analog_lines);

   DBG_PRINT("CFW deactivation line %d, returnCode=%d\r\n",line , returnCode);
   if ( voip_conf_ext_cfw_dest_set(line, NULL) != 0)
      DBG_PRINT_ERR("Warning: CFW destination could not be cleared \r\n" );

   /* the user should get any indication !!! */
   if ( analogLine )
   {
      term_call_mngr_cbs->activate_cfw(analogLine->data, 0);
      term_call_mngr_cbs->start_signal(analogLine->data, VOIP_SIGNAL_STUTTER_TONE, NULL);
   }

   return;
}

/*  EA - 071218 - VI#59630 support SIP over TLS
    callbacks implementation
*/
#ifdef RV_CFLAG_TLS
#if (RV_TLS_TYPE == RV_TLS_OPENSSL)
static RvBool ca_file_name_get(IN char* fileNameStr, OUT char* caFileName)
{
	caFileName[0] = '\0';
	if (fileNameStr[0] == '\0')
		return rvFalse;

    /* the name is not empty */
	strncpy(caFileName, fileNameStr, FILENAME_LEN);
	caFileName[FILENAME_LEN-1]='\0';

	return rvTrue;
}

static RvBool sip_tls_get_buffer_cb(IN   RvIppTlsBufferType bufferType,
                                  OUT	RvChar			*tlsBuffer,
                                  OUT	RvUint32		*tlsBufferLen)
{
    static RvUint32 caFileIndex = 0;

    BIO*			inKey          = NULL;
    BIO*			inCert         = NULL;
    EVP_PKEY*		pkey           = NULL;
    X509*			x509           = NULL;
    RvChar*			privKey        = NULL;
    RvChar*			keyEnd;
    RvInt			privKeyLen     = 0;
    RvChar*			cert          = NULL;
    RvChar*			certEnd;
    RvInt			certLen        = 0;
    char			caCertFileName[FILENAME_LEN];
    char			*privateKeyFileName = call_mngr_params->sp.sip.keyTlsCfg.privateKeyFileName;
    /*char			*pCaCertFileName = call_mngr_params->sp.sip.keyTlsCfg.caCertFileName[0]; */

    *tlsBufferLen = 0;

    /* loading key */
    switch (bufferType)
    {
    case IPP_TLS_SERVER_KEY_BUFFER:
        privKey = tlsBuffer;
        keyEnd = privKey;
        inKey=BIO_new(BIO_s_file_internal());
        if (BIO_read_filename(inKey, privateKeyFileName) <= 0)
        {
            IppLogMessage(RV_TRUE, "BIO_read_filename() failed in IPP_TLS_SERVER_KEY_BUFFER for file %s", privateKeyFileName);
            return rvFalse;
        }
        IppLogMessage(RV_FALSE, "BIO_read_filename() succeeded in IPP_TLS_SERVER_KEY_BUFFER for file %s", privateKeyFileName);

        pkey=PEM_read_bio_PrivateKey(inKey,NULL,NULL,NULL);
        if (NULL == pkey)
        {
            IppLogMessage(RV_TRUE, "PEM_read_bio_PrivateKey() failed in IPP_TLS_SERVER_KEY_BUFFER for file %s", privateKeyFileName);
            return rvFalse;
        }
        IppLogMessage(RV_FALSE, "PEM_read_bio_PrivateKey() succeeded in IPP_TLS_SERVER_KEY_BUFFER for file %s", privateKeyFileName);
        privKeyLen = i2d_PrivateKey(pkey,NULL);
        privKeyLen = i2d_PrivateKey(pkey,(unsigned char **)&keyEnd);
        BIO_free(inKey);
        *tlsBufferLen = privKeyLen;
        break;
    case IPP_TLS_SERVER_CA_BUFFER:
        cert = tlsBuffer;
        certEnd=cert;

        inCert=BIO_new(BIO_s_file_internal());
        if (BIO_read_filename(inCert, privateKeyFileName) <= 0)
        {
            IppLogMessage(RV_TRUE, "BIO_read_filename() failed in IPP_TLS_SERVER_CA_BUFFER for file %s", privateKeyFileName);
            return rvFalse;
        }
        IppLogMessage(RV_FALSE, "BIO_read_filename() succeeded in IPP_TLS_SERVER_CA_BUFFER for file %s", privateKeyFileName);

        x509=PEM_read_bio_X509(inCert,NULL,NULL,NULL);
        if (NULL == x509)
        {
            IppLogMessage(RV_TRUE, "PEM_read_bio_X509() failed in IPP_TLS_SERVER_CA_BUFFER for file %s", privateKeyFileName);
            return rvFalse;
        }
        IppLogMessage(RV_FALSE, "PEM_read_bio_X509() succeeded in IPP_TLS_SERVER_CA_BUFFER for file %s", privateKeyFileName);

        certLen = i2d_X509(x509,NULL);
        certLen = i2d_X509(x509,(unsigned char **)&certEnd);
        BIO_free(inCert);
        *tlsBufferLen = certLen;
        break;

    case IPP_TLS_CLIENT_KEY_BUFFER:
        break;

    case IPP_TLS_CA_BUFFER:
    /*
    get a CA filenames from the GW list of names
        */
        if ((!ca_file_name_get(call_mngr_params->sp.sip.keyTlsCfg.caCertFileName[caFileIndex], (char *)&caCertFileName))
            || (caFileIndex == TLS_MAX_CA))
        {
            return rvFalse;
        }
        IppLogMessage(RV_FALSE, "caCertFileName = %s", caCertFileName);

        /* for adding the approving CA to the list of certificates */
        cert = tlsBuffer;
        certEnd=cert;
        certLen        = 0;
        x509           = NULL;
        memset(cert,0,sizeof(cert));

        /* loading certificate */
        inCert=BIO_new(BIO_s_file_internal());

        if (BIO_read_filename(inCert, caCertFileName) <= 0)
        {
            IppLogMessage(RV_TRUE, "Can not load CA certificate from %s", caCertFileName);
            return rvFalse ;
        }
        IppLogMessage(RV_FALSE, "BIO_read_filename() succeeded in IPP_TLS_CA_BUFFER for file %s", caCertFileName);

        x509=PEM_read_bio_X509(inCert,NULL,NULL,NULL);
        IppLogMessage(RV_FALSE, "PEM_read_bio_X509() returned in IPP_TLS_CA_BUFFER %p", x509);

        certLen = i2d_X509(x509,NULL);
        certLen = i2d_X509(x509,(unsigned char **)&certEnd);
        BIO_free(inCert);
        *tlsBufferLen = certLen;
        caFileIndex++;

        break;
    default:
        ;
    }

    return rvTrue;
}
#endif /* (RV_TLS_TYPE == RV_TLS_OPENSSL) */

static void sip_tls_post_connection_assertion(IN    RvSipTransportConnectionHandle             hConnection,
									          IN    RvSipTransportConnectionAppHandle          hAppConnection,
									          IN    RvChar*                                    strHostName,
									          IN    RvSipMsgHandle                             hMsg,
									          OUT   RvBool*                                    pbAsserted)
{
	RV_UNUSED_ARG(hConnection);
    RV_UNUSED_ARG(hAppConnection);
    RV_UNUSED_ARG(strHostName);
	RV_UNUSED_ARG(hMsg);

	/* behavior as defined in the configuration file.*/
	*pbAsserted = call_mngr_params->sp.sip.transportTlsCfg.tlsPostConnectAssertFlag;
}

#endif /* RV_CFLAG_TLS */

void sip_register_ext_clbks(void *pCfg)
{
  RvIppSipPhoneCfg *pMyCfg = (RvIppSipPhoneCfg *)pCfg; /* ACL NB 061115 */

   /*ER06 - Register SIP extension callbacks (in addition to the MDM callbacks)*/
	RvIppSipExtClbks rvsipCallbacks;
/*---------------------------------------------------------------------------*/
/* ACL NB 061011 */
/* rvIppSipRegisterExtClbks Should be called before rvIppSipStackInitialize()*/
/*---------------------------------------------------------------------------*/
    MZERO(rvsipCallbacks);
    // ER06 - Register a SIP extension callback for obtaining registration status
	rvsipCallbacks.PreRegClientStateChangedF = sip_pre_reg_client_state_changed;

	// ER06 - Register a SIP extension callback for controlling g_cCaller-ID
	rvsipCallbacks.preCallLegCreatedOutgoingF = sip_pre_call_leg_created_outgoing;

	// ER06 - Register a SIP extension callback for modfying SIP configuration
	rvsipCallbacks.stackConfigF = sip_stack_config_cb;

	// ER06 - Register a SIP extension callback to capture sent SIP messages
	rvsipCallbacks.postMsgToSendF = acl_sip_call_mngr_post_mgs_tx;
	/* AlexChen 20090409 SDK upgrade */
	rvsipCallbacks.postMsgReceivedF = acl_sip_call_mngr_post_mgs_rx;

	// ER06 - Register a SIP extension callback for to capture received SIP messages
	rvsipCallbacks.preMsgReceivedF = acl_sip_call_mngr_pre_mgs_rx;

   /* ACL NB 070327 -  Patch received from RV at 25-Mar-07 Incident #128937: CANCEL */
   rvsipCallbacks.registerStackEventsF = sip_register_stack_events;

   // ACL NB 070516 AC#53311 - Register a SIP extension callback for adding User-Agent in INVITE
   rvsipCallbacks.preMsgToSendF = acl_sip_call_mngr_pre_mgs_tx;

	rvIppSipRegisterExtClbks(&rvsipCallbacks);

   /*ACL NB 061115 - CFW callbacks. */
   pMyCfg->cfwCallCfg.cfwCallBacks.activateCompleted   = sip_cfw_activate_completed_cb;
   pMyCfg->cfwCallCfg.cfwCallBacks.deactivateCompleted = sip_cfw_deactivate_completed_cb;

}

#ifdef RV_CFLAG_TLS
void sip_tls_register_ext_clbks(void)
{
	rvIppSipTlsRegisterExtClbks(&mdm_SipTlsExtClbks);
}
#endif /* RV_CFLAG_TLS */


static int acl_sip_call_mngr_init(acl_call_mngr_params_t *params, acl_call_mngr_cbs_t *cbs)
{
    RvIppMdmExtClbks rvccCallbacks;
   int i, iNumOfLines;

    call_mngr_params = params;
    term_call_mngr_cbs = cbs;
    /*EA - 080512 - this printing cannot use RV log mechanism since
      it is only initialized in the call_sip_init function*/
    printf("\r\n");

    if ((call_mngr_params->protocol==VOIP_PROT_SIP )&& (call_sip_init(call_mngr_params, &term_mgr)))
    {
   	printf("[%s:%d] call_sip_init() failed\n", __FUNCTION__, __LINE__);
	return -1;
    }
    if((call_mngr_params->protocol==VOIP_PROT_H323) && (mdm_h323_init(call_mngr_params, &term_mgr)))
    {
   	printf("[%s:%d] mdm_h323_init() failed\n", __FUNCTION__, __LINE__);
	return -1;
    }

    /* init term manager */
    rvMdmTermMgrRegisterSelectTermCB(&term_mgr, sip_select_term_cb);
    rvMdmTermMgrRegisterDeleteEphTermCB(&term_mgr, sip_delete_ephemeral_term_cb);
    rvMdmTermMgrRegisterConnectCB(&term_mgr, sip_connect_cb);
    rvMdmTermMgrRegisterDisconnectCB(&term_mgr, sip_disconnect_cb);
    /* Needed in both IpPhone and ATA (to make use of rvcc package) */
    rvMdmTermMgrRegisterIPPhoneUiPackages(&term_mgr);

    /* init termination classes: rtp, al; register callbacks.
     * we support only one class for each device type */
    rtp_class = rvMdmTermMgrCreateTermClass(&term_mgr);
    rvMdmTermClassSetRtpTermPackages(rtp_class);
    rvMdmTermClassRegisterCreateMediaCB(rtp_class, sip_create_media_cb);
    rvMdmTermClassRegisterModifyMediaCB(rtp_class, sip_modify_media_cb);
    rvMdmTermClassRegisterDestroyMediaCB(rtp_class, sip_destroy_media_cb);
	 rvMdmTermClassRegisterModifyMediaCompletedCB(rtp_class, sip_modify_media_completed_cb); /* ACL NB 060404 */


    MZERO(rvccCallbacks);
#ifdef CONFIG_RG_ATA
    al_class = rvMdmTermMgrCreateTermClass(&term_mgr);
    rvMdmTermClassSetAnalogLinePackages(al_class);
    rvMdmTermClassRegisterMapDialStringToAddressCB(al_class, 	sip_map_dial_string_to_addr_cb);
    rvMdmTermClassRegisterMatchDialStringCB(al_class, sip_collect_dial_string_cb);
    rvMdmTermClassRegisterStartSignalCB(al_class, sip_start_signal_cb);
    rvMdmTermClassRegisterPlaySignalCB(al_class, sip_play_signal_cb);
    rvMdmTermClassRegisterStopSignalCB(al_class, sip_stop_signal_cb);
    rvMdmTermClassRegisterCreateMediaCB(al_class, sip_line_create_media_cb);

    /* ACL NB 060528 */
    /* This callback must be implemented and registered otherwise the voip task crash*/
    rvMdmTermClassRegisterUnregisterTermCompletedCB(al_class,sip_unreg_phys_term_completed_cb);

    /* To support supplementary services in ATA we will map user events */
    // ACL NB 070415 rvccCallbacks.mapUserEvent = rvCCTerminalMdmMapEvent;

   /* ACL NB 070516 - AC#53280 PATCH.Email received from RV */
   /* Incident #129532: INFO message  16-May-07             */
   rvccCallbacks.mapUserEvent = acl_map_event_cb;

	    /* ACL NB 061008 added support of VMWI */
    rvMdmTermMgrRegisterUserSignal(&term_mgr, "msg", "MWI");
   rvMdmTermMgrSetUserPackage(al_class, "msg");
#endif
    /* ACL NB - 061115*/
    rvccCallbacks.postProcessEvent = sip_post_process_event;
    /* ACL NB - 070405 */
    rvccCallbacks.preProcessEvent = sip_pre_process_event;
#ifdef CONFIG_RG_IPPHONE
    ui_class = rvMdmTermMgrCreateTermClass(&term_mgr);
    rvMdmTermClassSetIPPhoneUIPackages(ui_class);
    rvMdmTermClassRegisterStartSignalCB(ui_class, sip_start_signal_cb);
    rvMdmTermClassRegisterPlaySignalCB(ui_class, sip_play_signal_cb);
    rvMdmTermClassRegisterStopSignalCB(ui_class, sip_stop_signal_cb);
    rvMdmTermClassRegisterMapDialStringToAddressCB(ui_class,
	sip_map_dial_string_to_addr_cb);
    rvMdmTermClassRegisterMatchDialStringCB(ui_class,
	sip_collect_dial_string_cb);

    at_class = rvMdmTermMgrCreateTermClass(&term_mgr);
    rvMdmTermClassSetAudioTransducerPackages(at_class);
    rvMdmTermClassRegisterStartSignalCB(at_class, sip_start_signal_cb);
    rvMdmTermClassRegisterPlaySignalCB(at_class, sip_play_signal_cb);
    rvMdmTermClassRegisterStopSignalCB(at_class, sip_stop_signal_cb);
    rvMdmTermClassRegisterCreateMediaCB(at_class, sip_line_create_media_cb);
#endif
    rvccCallbacks.display = sip_rvcc_display;
    rvIppMdmRegisterExtClbks(&rvccCallbacks);

   iNumOfLines=call_mngr_params->ep_num;
   // ER06 - Initialize the line status database (ER06 - Fix bug found at 1.5.0 build 14)
   if (!(line_status = AC_ZALLOC_L(sizeof(line_status_t)*iNumOfLines)))
            return -1;

   for(i=0 ; i<iNumOfLines; i++)
   {

      line_status[i].line_registration_state = SIP_REG_UNREGISTERED;

	  line_status[i].active_call_origin = CALL_ORIGIN_UNKNOWN;
	  line_status[i].secondary_dial_tone_on = 0; /* ACL N 070528 */
	  line_status[i].regretCallSimHookOn = 0;	/* AlexChen SDK upgrade 20090414 */
	  line_status[i].authRetriesCounter = 0;	/* AlexChen SDK upgrade 20090414 */
   }

   /* AlexChen SDK upgrade 20090415 */
   /*ACL AL 081029 - VI67678 LastCreateMsg struct should be per line*/
   if(lastCreateMsg_initDB(iNumOfLines))
      return -1;

   isBlindTrans = 0; /* ACL NB 070628 */

    /* load media capabilities of classes */
    media_capabilities_load();

    voip_conf_ua_get( g_userAgentStr );  /* ACL NB 061225 */

    voip_conf_voip_log_lvl_get( &SIPLogLevel );  /* ACL NB 070118 */


    /* endpoints creation might start proxy registration process, thus we need
     * to make sure that stack is fully started at that stage */
    rvMdmTermMgrStart(&term_mgr, NULL, 0);

    printf("[%s:%d]--->  CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES=%d, iNumOfLines=%d.\n", __FILE__, __LINE__, CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES, call_mngr_params->ep_num);
    /* Now create the endpoints */
    if (sip_analog_lines_create())
    {
	rvMdmTermMgrDestruct(&term_mgr);
	return -1;
    }

    /* ACL NB 061210 - get CFW parameters */
   voip_conf_cfw_type_get( &g_cfwParams );
   if (g_cfwParams.isEnabled)
   {
      printf("CFW params \r\n cfwType=%d, cfnrTimeout %d, keySequence %s\n",g_cfwParams.cfwType, g_cfwParams.cfnrTimeout,g_cfwParams.keySequence);
   }
   else
   {
      DBG_PRINT("CFW is DISABELD \r\n");
   }

   is_up = 1;

   return 0;
}

static int acl_sip_call_mngr_uninit(void)
{
    int i;

    if (!is_up)
	return 0;

    for (i = 0; i<call_mngr_params->ep_num; i++)
    {
	if (!call_mngr_params->lines[i].enabled)
	    continue;

	   /* ACL NB 060528 remove reference to term_mgr from termination */
   	/* rvMdmTermSetUserData(analog_lines[i].line.ata.al,NULL); */
	   rvMdmTermMgrUnregisterTermination(&term_mgr, analog_lines[i].line.ata.al, NULL);

    	   DBG_PRINT("Setting line %d to status %d\r\n", i, PORT_LOCK);

	   SetLineStatus(i, PORT_LOCK);
    }
    /* This function is never called in normal way (i.e. restarting of voip
     * stack within this process). If we are here, it means we are going to get
     * killed in a moment. So skip all the rest of this function (that does not
     * work well anyway) */
#if 0
    stack_handle = rvMdmTermMgrGetUserData(&term_mgr);

    for (i = 0; i<call_mngr_params->ep_num; i++)
	term_call_mngr_cbs->free_endpoint_context(analog_lines[i].data);

    free(analog_lines);
    analog_lines = NULL;
    rvMdmTermMgrStop(&term_mgr, NULL);
    rvMdmTermMgrDestruct(&term_mgr);

    if (call_mngr_params->protocol==VOIP_PROT_SIP)
	call_sip_uninit(stack_handle);

    if (vcCONFIG_RG_VOIP_RV_H323 && call_mngr_params->protocol==VOIP_PROT_H323)
	h323_alias_free(call_mngr_params);

    is_up = 0;
#endif

    return 0;
}


int acl_sip_call_mngr_uninit_per_line(int line)
{
    if( (line >= 0) && (line < call_mngr_params->ep_num) && (call_mngr_params->lines[line].enabled) )
    {
	   /* ACL NB 060528 remove reference to term_mgr from termination */
   	   /* rvMdmTermSetUserData(analog_lines[line].line.ata.al,NULL); */
	   rvMdmTermMgrUnregisterTermination(&term_mgr, analog_lines[line].line.ata.al, NULL);

	   DBG_PRINT("line %d \r\n", line);
    }

    return 0;
}

int acl_sip_call_mngr_init_per_line(int line)
{

    if( (line >= 0) && (line < call_mngr_params->ep_num) && (call_mngr_params->lines[line].enabled) )
    {
		single_analog_line_create(line);

	   	DBG_PRINT("line %d \r\n", line);
    }

    return 0;
}

static voip_protocols_t sip_get_signalling_protocol_cb(void)
{
    return call_mngr_params->protocol;
}

//060530 Avner phase 1
static int sip_get_registration_status_cb(int epn)
{
    /* XXX should we manipulate the line according to the .enabled endpoints? */
    if (call_mngr_params->protocol == VOIP_PROT_SIP)
		return line_status[epn].line_registration_state;

    return SIP_REG_ERROR;
}

//060530 Avner phase 1
static void *sip_get_line_data_by_analog_line_cb(int epn)
{
    return analog_lines[epn].data;
}

//060530 Avner phase 1
static void sip_get_call_state_cb(int epn, call_state_t *state, call_origin_t *orig)
{
    RvCCConnState st = connection_state_get(analog_lines[epn].line.ata.al);

    /* XXX test these and add */
    switch (st)
    {
    case RV_CCCONNSTATE_DISCONNECTED:
	*state = CALL_STATE_DISCONNECTED; break;
    case RV_CCCONNSTATE_INITIATED:
    case RV_CCCONNSTATE_DIALING:
    case RV_CCCONNSTATE_ADDRESS_ANALYZE:
	*state = CALL_STATE_DIALING; break;
    case RV_CCCONNSTATE_INPROCESS:
    case RV_CCCONNSTATE_CALL_DELIVERED:
    case RV_CCCONNSTATE_OFFERED:
    case RV_CCCONNSTATE_ALERTING:
	*state = CALL_STATE_RINGING; break;
    case RV_CCCONNSTATE_CONNECTED:
	*state = CALL_STATE_CONNECTED; break;
    case RV_CCCONNSTATE_FAILED:
    case RV_CCCONNSTATE_REJECTED:
	*state = CALL_STATE_REORDER; break;
    case RV_CCCONNSTATE_IDLE:
    case RV_CCCONNSTATE_TRANSFER_INIT:
    case RV_CCCONNSTATE_TRANSFER_INPROCESS:
    case RV_CCCONNSTATE_TRANSFER_DELIVERED:
    case RV_CCCONNSTATE_TRANSFER_OFFERED:
    case RV_CCCONNSTATE_TRANSFER_ALERTING:
    case RV_CCCONNSTATE_ALERTING_REJECTED:
    case RV_CCCONNSTATE_UNKNOWN:
    default:
	*state = CALL_STATE_IDLE; break;
    }

	*orig = line_status[epn].active_call_origin;
}


static int sip_stun_client_reply_received_cb(char* buf, size_t size, u32 *pMappedIp,	u16 *pMappedPort)
{
   return acl_stun_client_reply_received(buf, size, pMappedIp, pMappedPort);
}

static unsigned char is_fxo_line_cb(int lineIdx)
{
	return (ANALOG_PORT_TYPE_FXO == analog_lines[lineIdx].enum_analog_port_type);
}

acl_call_mngr_ops_t term_call_mngr_ops = {
    init: acl_sip_call_mngr_init,
    uninit: acl_sip_call_mngr_uninit,
    get_active_protocol: sip_get_signalling_protocol_cb,
    process_event: sip_process_event,
    get_registration_status: sip_get_registration_status_cb,//060530 Avner phase 1
    get_conn_by_line: sip_get_line_data_by_analog_line_cb,//060530 Avner phase 1
    get_call_state: sip_get_call_state_cb,//060530 Avner phase 1
    stun_client_reply_received:sip_stun_client_reply_received_cb, /* ACL NB 080206 */
    is_fxo_line:is_fxo_line_cb/* JL phone_conn need to know if line is fxo*/
};




// by itzik - 19072006 - digit map
/*=========================================================================================================*/

/****************************************************************************
*
*	Name:		sip_addr_from_number_rules_get
*----------------------------------------------------------------------------
*	Author:		Itzik F
*----------------------------------------------------------------------------
*	Abstract:	scan the number rules and look for a match with the dial string, if there is a match then return the
*				sip address
*----------------------------------------------------------------------------
*	Input:		numberRules -
*				dialString -
*----------------------------------------------------------------------------
*   Output:		sipAddr -
*----------------------------------------------------------------------------
*	Returns: 	on success = 0, else -1
******************************************************************************/
static int sip_addr_from_number_rules_get(char *numberRules, char *dialString, char *sipAddr)
{
   	int keyStart;
	int keyLen;
	int	patternStart;
	int patternLen;
	int ruleLen;
	int ret=-1;
	char address[50]="";

	while(next_rule_get(numberRules, &ruleLen, &keyStart, &keyLen, &patternStart, &patternLen) == 0) {
		if(dial_str_2_key_compare(dialString, numberRules+keyStart, keyLen) == 0) {
    		addr_build(dialString, numberRules+keyStart , keyLen , numberRules+patternStart , patternLen, address);
			ret=0;
			break;
    	}
		if((size_t)ruleLen == strlen(numberRules))
			break;
		numberRules = numberRules + ruleLen + 1;
	}
	strcpy(sipAddr,address);
	return ret;
} /* end of sip_addr_from_number_rules_get */
/******************************************************************************/


/****************************************************************************
*
*	Name:		next_rule_get
*----------------------------------------------------------------------------
*	Author:		Itzik F
*----------------------------------------------------------------------------
*	Abstract:	return the next rule key and pattern indexs
*----------------------------------------------------------------------------
*	Input:		numberRules -
*				ruleLen -
*----------------------------------------------------------------------------
*   Output:		keyStart -
*               keyLen -
*				patternStart -
*				patternLen -
*----------------------------------------------------------------------------
*	Returns: 	on success = 0, else -1
******************************************************************************/
static int next_rule_get(char *numberRules,int *ruleLen, int *keyStart, int *keyLen, int *patternStart, int *patternLen)
{
	int i;
	int len;

	if(numberRules == NULL)
		return -1;

	len = strlen(numberRules);

	*keyStart = 0;
	for(i=0, *keyLen=0; i<len && numberRules[i] != '='; i++, (*keyLen)++)
    	;
	*ruleLen = i;
	if(i==len)
		return -1;
	i++;
	*patternStart = i;
	for(*patternLen=0; numberRules[i] && numberRules[i] != ';'; i++, (*patternLen)++)
		;
	*ruleLen = i;

	return 0;

} /* end of next_rule_get */
/******************************************************************************/


/****************************************************************************
*
*	Name:		dial_str_2_key_compare
*----------------------------------------------------------------------------
*	Author:		Itzik F
*----------------------------------------------------------------------------
*	Abstract:	compare the dial string to a single key
*----------------------------------------------------------------------------
*	Input:		key - a pointer to the start of the key
*				keyLen -
*----------------------------------------------------------------------------
*   Output:		none
*----------------------------------------------------------------------------
*	Returns: 	on success = 0, else -1
******************************************************************************/
static int dial_str_2_key_compare(char *dialString, char *key, int keyLen)
{
	int i;
	int ret=0;

	if(strlen(dialString) != (size_t)keyLen)
		return -1;

	for (i=0; i < keyLen ;i++)
	{
		switch(key[i])
		{
		case '0':case '1':case '2':case '3':case '4':
		case '5':case '6':case '7':case '8':case '9':
		case '*':case '#':
			if (dialString[i]==key[i])
				continue;
			ret = -1;
			break;
		case 'x':
			continue;
			break;
		}
		break;
	}

	return ret;

} /* end of dial_str_2_key_compare */
/******************************************************************************/


/****************************************************************************
*
*	Name:		addr_build
*----------------------------------------------------------------------------
*	Author:		Itzik F
*----------------------------------------------------------------------------
*	Abstract:	get the address from the key and the pattern while comparing to the dial string
*----------------------------------------------------------------------------
*	Input:		dialString -
*				key - a pointer to the start of the key
*				keyLen -
*				pattern - a pointer to the start of the pattern
*               patternLen -
*----------------------------------------------------------------------------
*   Output:		address -
*----------------------------------------------------------------------------
*	Returns: 	on success = 0, else -1
******************************************************************************/
static int addr_build(char *dialString, char *key, int keyLen, char *pattern, int patternLen, char *address)
{
	int keyI;
	int patternI;
	int dialStringI;
	int addressI;

	for (keyI=0,patternI=0,addressI=0,dialStringI=0 ; keyI < keyLen ;keyI++) {   // for keyI
		if (key[keyI]=='x') {
			for (; patternI < patternLen ; patternI++,addressI++) {
				if (pattern[patternI]=='\\') {
					address[addressI]=dialString[dialStringI];
                  	patternI++;
					addressI++;
					dialStringI++;
					break;
				}
				else {
					address[addressI]=pattern[patternI];
				}

			}  // end for patternI

		}  // end if (key[keyI]=='x')

		else {
			dialStringI++;
		}

	} // end for keyI

	for (;patternI < patternLen;patternI++,addressI++) {

		if (pattern[patternI]=='\\') {
			address[addressI]='x';
			break;
		}
		else
			address[addressI]=pattern[patternI];
	}

	address[addressI]='\0';

	return 0;

} /* end of addr_build */
/******************************************************************************/
/****************************************************************************
*
*	Name:		acl_sip_call_mngr_user_agent_get
*----------------------------------------------------------------------------
*	Abstract:	get the User-Agent
*----------------------------------------------------------------------------
*----------------------------------------------------------------------------
*   Output:		strUserAgent - User-Agent  description
*----------------------------------------------------------------------------
******************************************************************************/
void acl_sip_call_mngr_user_agent_get(char *strUserAgent)/* ACL NB 061225 */
{
   strcpy(strUserAgent,"User-Agent:");
   strcat(strUserAgent, g_userAgentStr);

   //DBG_PRINT("acl_sip_call_mngr_user_agent_get %s\r\n", g_userAgentStr);
   return;
}

#ifdef CONFIG_RG_ATA
static void acl_sip_call_mngr_blind_transfer_process(void  *data)
{
   RvIppTerminalHandle termHndl;
   RvMdmTerm* pMdmTerm;

   pMdmTerm = (RvMdmTerm* )data;

   termHndl = (RvIppTerminalHandle)pMdmTerm->xTerm;

   DBG_PRINT("acl_sip_call_mngr_blind_transfer_process\r\n");

   key_event_process("kf", "ku", "kbt", pMdmTerm);
   return;

}
#endif

/****************************************************************************
*
*	Name:		dist_ring_type_get
*----------------------------------------------------------------------------
*	Abstract:	convert string to distRring type
*
*----------------------------------------------------------------------------
*----------------------------------------------------------------------------
*   Output:
*----------------------------------------------------------------------------
******************************************************************************/
// TEMPORARY - should use configurable string !!!!
static int dist_ring_type_get(char* distRingVal)
{
   char *myTempStr;

   DBG_PRINT("dist_ring_type_get\r\n");

   myTempStr = strstr(distRingVal,"group");
   if (myTempStr)
      return(SLIC_RING_CADENCE_PROFILE_2);

   myTempStr = strstr(distRingVal,"internal");
   if (myTempStr)
      return(SLIC_RING_CADENCE_PROFILE_3);

   myTempStr = strstr(distRingVal,"Bellcore-dr2");
   if (myTempStr)
      return(SLIC_RING_CADENCE_PROFILE_2);

   return(SLIC_RING_CADENCE_PROFILE_1);

}
/*=========================================================================================================*/
void sip_refer_received_remote_accepted_handler(RvMdmTerm *pMdmTerm)
{
   acl_analog_line_t *analogLine;

   DBG_PRINT(">>\r\n");

   if (isBlindTrans)
   {
      isBlindTrans =0;
      analogLine = rvMdmTermGetUserData(pMdmTerm);

      DBG_PRINT("Blind TRansfer Remote accepted\r\n");
      if ( analogLine )
      {
         term_call_mngr_cbs->start_signal(analogLine->data, VOIP_SIGNAL_WARNING_TONE, NULL);
         sip_warning_tone_workaround(analogLine->data,1);
      }
      else
         DBG_PRINT_ERR("ERROR:no valid term\r\n");
   }

   else if (REMOTE_CONF_NOT_IN_PROGRESS != remote_conference_state_get(pMdmTerm))
           remote_conference_blind_transfer_complete(pMdmTerm);

   else
   {
       	   acl_analog_line_t *analogLine = rvMdmTermGetUserData(pMdmTerm);
       	   acl_line_t *line = analogLine->data;

	   /*   EA - 080517 - VI#64659 Call Transfer on BS and Asterisks isn't working properly - actually I returned the check
	   	if ((mdm_params->keys_sequence == KEY_SEQ_FLASH_ONLY) that I commented in the past (VI64127) becuase when
	   	key sequence is set to flash only the onhook is interpreted into transfer so rv stack never got the onhook event,
	   	we must send another onhook event. In addition, in semi transfer we must send an onhook event otherwise
	   	the transfer isnt completed and we hear one way voice.       */

	   /* NirM, 071008 - This patch is mandatory for the Improved Application */
	   if (/*(call_mngr_params->keys_sequence == KEY_SEQ_FLASH_ONLY) ||*/ (NULL == line->active_call))
           		onhook_key_simulate(pMdmTerm);

      /*
            071011 - EA - patch for stoping the ringback tone after executing semi transfer
            in flash + digits configuration, and playing fast busy tone.
            background - semi transfer in the flash only scenarion is done by putting the setting the
            hook onhook. upon receiving the onhook event we receive a "stop signal" for the ringback tone.
            this does not happen in the flash + digits configuration -> nothing stops the ringback tone.
            this patch stops the ring tone and plays fast busy to indicate to the user that his transfer
            was successfully carried out.
      */
      if ( call_mngr_params->keys_sequence == KEY_SEQ_FLASH_AND_DIGITS)
      {

         term_call_mngr_cbs->stop_signal(analogLine->data, VOIP_SIGNAL_RING_TONE, NULL);
          /*EA - 080512 - I commented the following two lines because the correct behaviour is that
          the transferer should hear quiet after completing semi att transfer*/
          /*term_call_mngr_cbs->start_signal(analogLine->data, VOIP_SIGNAL_WARNING_TONE, NULL);*/
          /*sip_warning_tone_workaround(analogLine->data,1);*/
      }
    }

   DBG_PRINT("<<\r\n");
}

/*EA - 071119 - fix sdp for vbd*/
static int isVbd(RvSdpMediaDescr* sdpMedia)
{
    int numOfAttr = 0;
    int attrCnt;
    acl_voip_sdp_attr_t *jSdpAttr = NULL;
    char *attrName = NULL;
    char *attrVal = NULL;
    char *bypassCoderVal = NULL;
    int rc = 0;
    int bypassCoder;

    numOfAttr = acl_voip_sdp_media_descr_attr_num_get((acl_voip_sdp_media_descr_t *)sdpMedia);

    for (attrCnt = 0; attrCnt < numOfAttr; attrCnt++)
    {
        jSdpAttr = acl_voip_sdp_media_descr_attr_get((acl_voip_sdp_media_descr_t *)sdpMedia, attrCnt);
        attrName = acl_voip_sdp_attr_name_get(jSdpAttr);
        DBG_PRINT("strcmp(attrName, \"gpmd\") = %d\r\n", strcmp(attrName, "gpmd"));
        attrVal = acl_voip_sdp_attr_value_get(jSdpAttr);
        DBG_PRINT("strstr(attrVal, \"vbd\") = %d\r\n", strstr(attrVal, "vbd"));
        bypassCoderVal = (char*)attrVal;
        DBG_PRINT("bypassCoderVal = %s, bypassCoder = %d, atoi(bypassCoderVal) = %d\r\n", bypassCoderVal, bypassCoder, atoi(bypassCoderVal));
        voip_conf_bypassCoder_get(&bypassCoder);
        /*if (atoi(bypassCoderVal) != bypassCoder)*/

        if ((0 == strcmp(attrName, "gpmd")) &&
            /*(NULL != strstr(attrVal, "vbd")))*/
            (atoi(bypassCoderVal) == bypassCoder))
            rc = 1;
    }
    DBG_PRINT("<< rc = %d\r\n", rc);
    return rc;
}

/*  EA - 071218 - VI#59630 support SIP over TLS */
#ifdef RV_CFLAG_TLS
void sip_tls_mtf_config_params_set(void *pCfg)
{
    RvIppSipPhoneCfg *pMyCfg = (RvIppSipPhoneCfg *)pCfg; /* ACL NB 061115 */

    DBG_PRINT(">>>>\r\n");
    if (call_mngr_params->sp.sip.transportTlsCfg.stackTlsPort)
        pMyCfg->transportTlsCfg.stackTlsPort = call_mngr_params->sp.sip.transportTlsCfg.stackTlsPort;

    if (call_mngr_params->sp.sip.transportTlsCfg.stackTlsAddress)
        pMyCfg->transportTlsCfg.stackTlsAddress = call_mngr_params->sp.sip.transportTlsCfg.stackTlsAddress;

    if (call_mngr_params->sp.sip.transportTlsCfg.certDepth)
        pMyCfg->transportTlsCfg.certDepth = call_mngr_params->sp.sip.transportTlsCfg.certDepth;

    pMyCfg->transportTlsCfg.stackNumOfTlsAddresses = call_mngr_params->sp.sip.transportTlsCfg.stackNumOfTlsAddresses;

    pMyCfg->transportTlsCfg.privateKeyType = call_mngr_params->sp.sip.transportTlsCfg.privateKeyType;

    if(call_mngr_params->sp.sip.transportTlsCfg.tlsMethod != RVSIP_TRANSPORT_TLS_METHOD_UNDEFINED)
        pMyCfg->transportTlsCfg.tlsMethod = call_mngr_params->sp.sip.transportTlsCfg.tlsMethod;

    pMyCfg->transportTlsCfg.tlsPostConnectAssertFlag = call_mngr_params->sp.sip.transportTlsCfg.tlsPostConnectAssertFlag;

    DBG_PRINT("<<<<\r\n");
}
#endif /* RV_CFLAG_TLS */

/*EA - 080114 - VI#60461 - Bezeq Interop*/
void acl_signal_start(void *pMdmTerm, voip_signal_t signal)
{
       	acl_analog_line_t *analogLine = rvMdmTermGetUserData(pMdmTerm);
	term_call_mngr_cbs->start_signal(analogLine->data, signal, NULL);
}

void acl_signal_stop(void *pMdmTerm, voip_signal_t signal)
{
        acl_analog_line_t *analogLine = rvMdmTermGetUserData(pMdmTerm);
	term_call_mngr_cbs->stop_signal(analogLine->data, signal, NULL);
}

/*  EA - 080207 VI#60671 Setting Syslog IP address for application logs through GUI
	added this function to let RV receive UDP server address from us*/
void AclGetUdpServerAddr(int *serverPortForAppSyslog, char **serverAddrForAppSyslog, int *printToUdp, int *printToTerminal)
{
	int udpTerminalFlag;
	voip_conf_syslog_server_info_get(serverPortForAppSyslog, serverAddrForAppSyslog, &udpTerminalFlag);

	*printToUdp = SYSLOG_PRINT_TO_UDP & udpTerminalFlag;
	*printToTerminal = SYSLOG_PRINT_TO_TERMINAL & udpTerminalFlag;
}

/*EA - 080526 - VI64130 - in order to return to first call (held call)
	after second call didnt answer you have to press flash twice.*/
int acl_does_other_held_conn_exist(RvMdmTerm *pMdmTerm)
{
	RvIppTerminalHandle     termHndl = NULL;
	RvIppConnectionHandle   connHndl = NULL;
	int                     heldCall = -1;

	DBG_PRINT(">>>>\r\n");
	termHndl = rvIppMdmTerminalGetHandle(pMdmTerm);
	if (termHndl)
		connHndl = rvIppMdmTerminalGetActiveConnection(termHndl);

	if (connHndl)
		heldCall = rvIppMdmTerminalOtherHeldConnExist(termHndl, connHndl);

	DBG_PRINT("<<<<\r\n");

	return heldCall;
}

/*EA - 080618 - 64724 - The Call after Call waiting is getting   fast busy when making a second call */
int acl_is_during_active_call(RvMdmTerm *pMdmTerm)
{
	RvIppTerminalHandle     termHndl = NULL;
	int                     connExist = 0; /*some other error*/

	DBG_PRINT(">>>>connection_state = %d\r\n", connection_state_get(pMdmTerm));

	termHndl = rvIppMdmTerminalGetHandle(pMdmTerm);

	if (termHndl)
	{
		if ( rvIppMdmTerminalGetActiveConnection(termHndl) &&
			(connection_state_get(pMdmTerm) != RV_CCCONNSTATE_DISCONNECTED ))
			connExist = 1;
	}

	DBG_PRINT("<<<< returned %d\r\n", connExist);

	return connExist;
}

/* AlexChen SDK upgrade 20090413 */
/*EA - 080807 - VI65537 - RFC 4240 - interop - dealing with remote parties that do not support REFER inside INVITE message*/
void call_sip_mngr_refer_info_headers_str_set(void *pMdmTerm,
                                             char* strReferToHeader,
                                             char* strReferredByHeader)
{
    RvMdmTerm       *mdmTerm    = (RvMdmTerm*)pMdmTerm;
    acl_analog_line_t *ep         = NULL;
    acl_line_t    *phoneConn  = NULL;

    if (mdmTerm == NULL)
    {
        DBG_PRINT_ERR("mdmTerm is NULL\r\n");
        return;
    }

    ep = rvMdmTermGetUserData(mdmTerm);
    if (ep == NULL)
    {
        DBG_PRINT_ERR("failed to get ep\r\n");
        return;
    }
    phoneConn = (acl_line_t*)ep->data;

    acl_line_refer_info_headers_str_set(phoneConn, strReferToHeader, strReferredByHeader);
}

void call_sip_mngr_refer_info_headers_str_get(void *pMdmTerm,
                                             char** pStrReferToHeader,
                                             char** pStrReferredByHeader)
{
    RvMdmTerm       *mdmTerm    = (RvMdmTerm*)pMdmTerm;
    acl_analog_line_t	*ep         = NULL;
    acl_line_t    *phoneConn  = NULL;

    if (mdmTerm == NULL)
    {
        DBG_PRINT_ERR("mdmTerm is NULL\r\n");
        return;
    }

    ep = rvMdmTermGetUserData(mdmTerm);
    if (ep == NULL)
    {
        DBG_PRINT_ERR("failed to get ep\r\n");
        return;
    }
    phoneConn = (acl_line_t *)ep->data;

    acl_line_refer_info_headers_str_get(phoneConn, pStrReferToHeader, pStrReferredByHeader);
}

void call_sip_mngr_refer_info_failure_flag_set(void *pMdmTerm, int val)
{
    RvMdmTerm       *mdmTerm    = (RvMdmTerm*)pMdmTerm;
    acl_analog_line_t  *ep         = NULL;
    acl_line_t    *phoneConn  = NULL;

    if (mdmTerm == NULL)
    {
        DBG_PRINT_ERR("mdmTerm is NULL\r\n");
        return;
    }

    ep = rvMdmTermGetUserData(mdmTerm);
    if (ep == NULL)
    {
        DBG_PRINT_ERR("failed to get ep\r\n");
        return;
    }
    phoneConn = (acl_line_t *)ep->data;

    acl_line_refer_info_failure_flag_set(phoneConn, val);
}

int call_sip_mngr_refer_info_failure_flag_get(void *pMdmTerm)
{
    RvMdmTerm       *mdmTerm        = (RvMdmTerm*)pMdmTerm;
    acl_analog_line_t	*ep             = NULL;
    acl_line_t    *phoneConn      = NULL;
    int             didReferFail    = 0;

    if (mdmTerm == NULL)
    {
        DBG_PRINT_ERR("mdmTerm is NULL\r\n");
        return -1;
    }

    ep = rvMdmTermGetUserData(mdmTerm);
    if (ep == NULL)
    {
        DBG_PRINT_ERR("failed to get ep\r\n");
        return -1;
    }
    phoneConn = (acl_line_t*)ep->data;

    didReferFail = acl_line_refer_info_failure_flag_get(phoneConn);
    return didReferFail;
}

/*  EA - 080807 - I had to split the flags because we had a very nasty situation where the other proxy
    didnt support nested transactions within an INVITE message, and we had 2 different nested transactions
    one is the REFER that comes right after the HOLD when sending Blind Transfer and the second is
    when sending B UNHOLD in order to send Blind Transfer - I couldnt use the same flag because both nested
    transactions wait for an INVITE transaction to finish and the run over each other...*/
void call_sip_mngr_refer_info_unhold_flag_set(void *pMdmTerm, int val)
{
    RvMdmTerm       *mdmTerm    = (RvMdmTerm*)pMdmTerm;
    acl_analog_line_t	*ep         = NULL;
    acl_line_t    *phoneConn  = NULL;

    if (mdmTerm == NULL)
    {
        DBG_PRINT_ERR("mdmTerm is NULL\r\n");
        return;
    }

    ep = rvMdmTermGetUserData(mdmTerm);
    if (ep == NULL)
    {
        DBG_PRINT_ERR("failed to get ep\r\n");
        return;
    }
    phoneConn = (acl_line_t*)ep->data;

    acl_line_refer_info_unhold_flag_set(phoneConn, val);
}

int call_sip_mngr_refer_info_unhold_flag_get(void *pMdmTerm)
{
    RvMdmTerm       *mdmTerm = (RvMdmTerm*)pMdmTerm;
    acl_analog_line_t	*ep             = NULL;
    acl_line_t    *phoneConn      = NULL;
    int             unholdInProgress    = 0;

    if (mdmTerm == NULL)
    {
        DBG_PRINT_ERR("mdmTerm is NULL\r\n");
        return -1;
    }

    ep = rvMdmTermGetUserData(mdmTerm);
    if (ep == NULL)
    {
        DBG_PRINT_ERR("failed to get ep\r\n");
        return -1;
    }
    phoneConn = (acl_line_t*)ep->data;

    unholdInProgress = acl_line_refer_info_unhold_flag_get(phoneConn);
    return unholdInProgress;
}

/*so the acl_telephonyAdvSipFeatures.c will be able to call it*/
void call_sip_mngr_process_remote_conference(void *pMdmTerm)
{
    RvMdmTerm *term = (RvMdmTerm*)pMdmTerm;
    remote_conference_process(term);
}



/*  EA - 080207 VI#60671 Setting Syslog IP address for application logs through GUI
    added this function to let RV receive UDP server address from us*/
void acl_sip_call_mngr_udp_g_cServer_addr_get(int *serverPortForAppSyslog, char **serverAddrForAppSyslog, int *printToUdp, int *printToTerminal)
{
    int udpTerminalFlag;
    voip_conf_syslog_server_info_get(serverPortForAppSyslog, serverAddrForAppSyslog, &udpTerminalFlag);

    *printToUdp = SYSLOG_PRINT_TO_UDP & udpTerminalFlag;
    *printToTerminal = SYSLOG_PRINT_TO_TERMINAL & udpTerminalFlag;
}

#ifdef AC488_CPE_VOIP_TOOLKIT
static int isOtherTermInIdleState(void)
{
	int i;

	RvCCConnState state;
	RvMdmTerm *pTerm;

	/* Go over all the terminations and check whether one of
		the terminations connection state is RV_CCCONNSTATE_IDLE */
   	for (i = 0; i < call_mngr_params->ep_num; i++)
   	{
		pTerm = analog_lines[i].line.ata.al;
		state = connection_state_get(pTerm);

		if(RV_CCCONNSTATE_IDLE == state)
			return 1;
   	}

	return 0;

}
#endif /*AC488_CPE_VOIP_TOOLKIT*/

static RvBool myProcessTimer( void* data)
{
	RvMdmTerm	*term = (RvMdmTerm*)data;
   	acl_line_t 	*line;
   	acl_call_t 		*call;

        acl_analog_line_t *analogLine;

	analogLine = rvMdmTermGetUserData(term);

   	line = (acl_line_t *)analogLine->data;

   	call = line->active_call;

	if(!call->isVBD)
	{
		call->isVBD = 1;
		fax_bypass_detect_event_handle(line);
	}

	IppTimerDestruct(&myTimer);

	return rvTrue;
}

void sendCompleteString(char *string){


        printf("\n%s string \n",string);
        RvMdmParameterList params;
        /*Construct internal object*/
        rvMdmParameterListConstruct(&params);
        /*Build the Completion event*/
        rvMdmKpDigitMapBuildEvComplete(&params,string, RV_MDMDIGITMAP_UNAMBIGUOUSMATCH, &rvDefaultAlloc);
        /*Send the event to Multimedia Terminal Framework*/
        rvMdmTermProcessEvent(analogLine1->line.ipphone.ui, "kp", "ce", NULL, &params);
        /*Destruct internal object*/
        rvMdmParameterListDestruct(&params);

}

