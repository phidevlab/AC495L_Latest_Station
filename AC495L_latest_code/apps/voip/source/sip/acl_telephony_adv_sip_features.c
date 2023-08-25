/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	acl_telephony_adv_sip_features.c                    *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 26/02/08 - Coding started.                                       *
 *																	*
 ********************************************************************/

/******************************************************************************
Description: Telephoney Advanced features for SIP Phone
             1. Message waiting Indication for SIP Phone


				a. Register application subscription callbacks.
				b. Implement subscription application callback.
				c. Initiate an independent subscription, using SUBSCRIBE request.
				d. Send a notify request after accepting the subscription.
				e. Refresh the subscription, when alert is given at first time.
				f. Unsubscribe the subscription, when alert is given at second time.
				g. Send a notify (terminated) request, when accepting the unsubscribe request.
            ACL NB
            h. Received instruction from Radvision by email "RE: Incident #127654: NOTIFY unsupported" at Nov-16th-200
               "in case that you receive Notify without first Subscribing, you will need to treat the Notify
               at the transaction level - it will be an independent incoming transaction."


*******************************************************************************
******************************************************************************/

/*-----------------------------------------------------------------------*/
/*                        INCLUDE HEADER FILES                           */
/*-----------------------------------------------------------------------*/
#define LOGSRC	LOGSRC_USERAPP
#include <IppStdInc.h>

#include <RV_SIP_DEF.h>

#include <rvstring.h>
#include <RvSipStackTypes.h>
#include <RvSipMsgTypes.h>
#include <rvSipControlApi.h>
#include <rvMdmControlApi.h>
#include <rvmdmtermevent.h>
#include <RvSipBody.h>
#include <rvlog.h>

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

/*ACL NB RvCCTerminal */
extern int AclCfwHandle(RvCCTerminal *xTerm,
     			            RvIppCfwType typeNumber,
				            RvUint		 timeoutValue,
				            char		    *destAddress,
				            int     	    destAddressLen);
extern void sip_refer_received_remote_accepted_handler(RvMdmTerm *pMdmTerm);

extern int isdigit(IN RvInt CharIn);

int call_sip_validation(void);

RvStatus RVCALLCONV SipTransactionGetMethod(
                                          IN RvSipTranscHandle        hTransc,
                                          OUT SipTransactionMethod *peMethod);

#define  ACL_ADV_SIP_FEATURES_DEBUG 1

#ifdef ACL_ADV_SIP_FEATURES_DEBUG
#define DBG_PRINT(a1,a2...)	IppLogMessage(RV_FALSE, "[%s:%4d] " a1,__FUNCTION__, __LINE__, ##a2)
#else
#define DBG_PRINT(a1,a2...)
#endif

#define DBG_PRINT_ERR(a1,a2...)	IppLogMessage(RV_TRUE, "[%s:%4d] " a1,__FUNCTION__, __LINE__, ##a2)


/*EA - 080114 - VI#60461 - Bezeq Interop*/
#define ACL_DONT_CALL_RV_CB     	0
#define ACL_CALL_RV_CB          		1

/*-----------------------------------------------------------------------*/
/*                           INTERNAL TYPES                              */
/*-----------------------------------------------------------------------*/
typedef struct
{
	RvSipStackHandle     stackHandle;
    RvSipTranscMgrHandle hTranscMgr;
    char			localAddress[64];
    char			registrarAddress[64];
    RvUint16		registrarPort;
    int			stackUdpPort;
    char		   subsServerName[128];
    int         CSeq;
    /*  get instruction from Radvision by email "RE: Incident #127654: NOTIFY unsupported" at Nov-16th-2006 */
    RvSipTransactionStateChangedEv old_pfnTransEvStateChanged;
    /* Support advanced INFO message */
    RvSipCallLegTrancRequestRcvdEv old_pfnTranscRequestRcvdEvHandler;
    RvSipControlCallLegReferStateChangedEv old_pfnReferStateChangedEvHandler;
    /*received instruction from Radvision by email " Incident #130103: REGISTRATION" at 11-Jul-07 - updated on 20-Mar-2008*/
    /*RvSipRegClientMsgReceivedEv    old_pfnMsgReceivedEvHandler; it was replaced with old_pfnStateChangedEvHandler */
    RvSipRegClientStateChangedEv  old_pfnStateChangedEvHandler;
	/* AlexChen SDK upgrade 20090413 */
	RvSipTransportBadSyntaxMsgEv   old_pfnEvBadSyntaxMsg;       /*ACL XY 090112 VI67392 Anatel certification: fix issue SIP_CC_OE_CE_V_031*/
	RvSipRegClientMsgToSendEv     old_pfnMsgToSendEvHandler; /* ACL NB 080811 */
    /*EA - 080722 - RFC 4240 - interop - dealing with remote parties that do not support REFER inside INVITE message*/
    RvSipCallLegMsgToSendEv             old_pfnCallLegMsgToSendEvHandler;
    RvSipCallLegMsgReceivedEv           old_pfnCallLegMsgReceivedEvHandler;
    RvSipCallLegStateChangedEv          old_pfnCallLegModifyStateChangedEvHandler;

} acl_adv_sip_cfg_internal_t;

typedef struct
{
	RvMdmTerm*		 term;
	RvSipSubsHandle hSubs;
	char			    from[128];
	char			    to[128];
   int             state;
}info_per_terminal_t;

/* AlexChen 20090401 SDK update redundant proxy feature */
typedef struct {
	char IpAddress[64];
	int PortNumber;
	unsigned char enabled;
	int KA_OptionRate;//keep alive rate in ms
	unsigned char IsSymmetric;
    unsigned char IsRedundant;
    unsigned char IsInit;
	RvSipTranscHandle	KATranscHandle;
} RedundantProxyCfg_t;

static RedundantProxyCfg_t RedundantProxyCfg = {{0},0,0,0,0,0,0,NULL};
/*-----------------------------------------------------------------------*/
/*                           DEFINITIONS                                 */
/*-----------------------------------------------------------------------*/
/*Define the To, From, and Event headers values as strings*/
#define EVENT "message-summary"

/*Define the expires header value to use in initial subscribe and in refresh */
#define EXPIRES 100
#define REFRESH_EXPIRES 50

/*-----------------------------------------------------------------------*/
/*                        GLOBAL VARIABLES                               */
/*-----------------------------------------------------------------------*/
static voip_mwi_params_t    g_mwiParams;


static acl_adv_sip_cfg_internal_t   g_cfg;

static info_per_terminal_t  		g_infoPerTerm[CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES];
/*Handle to the subscription manager. You can get this handle by calling
  RvSipStackGetSubsMgrHandle. You should supply this handle when
  using the subscription manager API functions.*/
static RvSipSubsMgrHandle g_hSubsMgr  = NULL;

/*Handle to the log-module. You can get this handle by calling
  RvSipStackGetLogHandle. You need this value in order to construct the application
  memory pool.*/
static RV_LOG_Handle         g_hLog         = NULL;

/*Handle to the application memory pool. The application should construct its own
  memory using rpool API. The pool is needed for encoding messages or message
  parts. (See acl_message_print() )*/
static HRPOOL                g_appPool      = NULL;

/*Boolean indicates wether it is time to send an unsubscribe request.
  it is set to true, after first refresh subscribe is sent. */
static RV_BOOL               bSendUnsubscribe = RV_FALSE;
/*-----------------------------------------------------------------------*/
/*                        STATIC FUNCTIONS PROTOTYPES                    */
/*-----------------------------------------------------------------------*/
static void sample_subscribe( info_per_terminal_t* subsPerTerm);
static RvStatus broadsoft_body_parse( IN	RvSipAppCallLegHandle   hAppCallLeg, IN char* buffer,IN RvUint bodyLength);
static RvStatus sscc_body_parse(IN	RvSipAppCallLegHandle hAppCallLeg, IN char* buffer, IN RvUint bodyLength);

/*---- E V E N T    H A N D L E R S   I M P L M E N T A T I O N ----------*/
/*  get instruction from Radvision by email "RE: Incident #127654: NOTIFY unsupported" at Nov-16th-2006 */
static void RVCALLCONV  acl_trans_state_changed_ev_handler(
         IN RvSipTranscHandle                 hTransc,
         IN RvSipTranscOwnerHandle            hTranscOwner,
         IN RvSipTransactionState             eState,
         IN RvSipTransactionStateChangeReason eReason);


static void RVCALLCONV acl_call_leg_refer_state_shanged_ev_handler(
                                                        IN  RvSipCallLegHandle             hCallLeg,
                                                        IN  RvSipAppCallLegHandle          hAppCallLeg,
                                                        IN  RvSipCallLegReferState         eState,
                                                        IN  RvSipCallLegStateChangeReason  eReason);

static void RVCALLCONV acl_call_leg_transc_request_rcvd_ev_handler(
                                                               IN  RvSipCallLegHandle     hCallLeg,
                                                               IN  RvSipAppCallLegHandle  hAppCallLeg,
                                                               IN  RvSipTranscHandle      hTransc,
                                                               IN  RvSipAppTranscHandle   hAppTransc);

/* AlexChen SDK upgrade 20090413 */
/*EA - 080807 - VI65537 - RFC 4240 - interop - dealing with remote parties that do not support REFER inside INVITE message*/
static RvStatus RVCALLCONV acl_app_call_leg_MsgToSendEvHandler(
                                    IN  RvSipCallLegHandle            hCallLeg,
                                    IN  RvSipAppCallLegHandle         hAppCallLeg,
                                    IN  RvSipMsgHandle                hMsg);

static RvStatus RVCALLCONV acl_app_call_leg_MsgReceivedEvHandler(
                                    IN  RvSipCallLegHandle            hCallLeg,
                                    IN  RvSipAppCallLegHandle         hAppCallLeg,
                                    IN  RvSipMsgHandle                hMsg);

static void RVCALLCONV  acl_app_call_leg_ModifyStateChangedEv(
                                    IN  RvSipCallLegHandle            hCallLeg,
                                    IN  RvSipAppCallLegHandle         hAppCallLeg,
                                    IN  RvSipCallLegModifyState       eModifyState,
                                    IN  RvSipCallLegStateChangeReason eReason);

/*ACL AL 081112 VI68005 Parsing error in 401 message caused the MP202 to ignore this message*/
/*ACL XY 090112 VI67392 Anatel certification: fix issue SIP_CC_OE_CE_V_019*/
static RV_Status RVCALLCONV acl_sip_transport_bad_syntax_msgEv(
                      IN    RvSipTransportMgrHandle    hTransportMgr,
                      IN    RvSipAppTransportMgrHandle hAppTransportMgr,
                      IN    RvSipMsgHandle             hMsgReceived,
                      OUT   RvSipTransportBsAction    *peAction);

static void RVCALLCONV acl_subs_created_ev_handler(
                                               IN  RvSipSubsHandle    hSubs,
                                               IN  RvSipCallLegHandle hCallLeg,
                                               IN  RvSipAppCallLegHandle hAppCallLeg,
                                               OUT RvSipAppSubsHandle *phAppSubs);

static void RVCALLCONV acl_subs_state_changed_ev_handler(
                                    IN  RvSipSubsHandle            hSubs,
                                    IN  RvSipAppSubsHandle         hAppSubs,
                                    IN  RvSipSubsState             eState,
                                    IN  RvSipSubsStateChangeReason eReason);

static void RVCALLCONV acl_subs_notify_ev_handler( IN  RvSipSubsHandle    hSubs,
                                       IN  RvSipAppSubsHandle hAppSubs,
                                       IN  RvSipNotifyHandle  hNotification,
                                       IN  RvSipAppNotifyHandle  hAppNotification,
                                       IN  RvSipSubsNotifyStatus eNotifyStatus,
                                       IN  RvSipSubsNotifyReason eNotifyReason,
                                       IN  RvSipMsgHandle     hNotifyMsg);

static RvStatus RVCALLCONV acl_subs_msg_received_ev_handler(
                                    IN  RvSipSubsHandle      hSubs,
                                    IN  RvSipAppSubsHandle   hAppSubs,
                                    IN  RvSipNotifyHandle    hNotify,
                                    IN  RvSipAppNotifyHandle hAppNotify,
                                    IN  RvSipMsgHandle       hMsg);

static RvStatus RVCALLCONV acl_subs_msg_to_send_ev_handler(
                                  IN  RvSipSubsHandle      hSubs,
                                  IN  RvSipAppSubsHandle   hAppSubs,
                                  IN  RvSipNotifyHandle    hNotify,
                                  IN  RvSipAppNotifyHandle hAppNotify,
                                  IN  RvSipMsgHandle       hMsg);
/*
static void RVCALLCONV AppSubsSubscriptionExpiredEvHandler(
                                    IN  RvSipSubsHandle            hSubs,
                                    IN  RvSipAppSubsHandle         hAppSubs);
*/
static void RVCALLCONV acl_subs_expiration_alert_ev_handler(
                                    IN  RvSipSubsHandle            hSubs,
                                    IN  RvSipAppSubsHandle         hAppSubs);


static RV_Status notification_handle(RvSipMsgHandle		hMsg);

/* ACL NB 070712 */
/*=========================================================================*/
/*received instruction from Radvision by email " Incident #130103: REGISTRATION" at 11-Jul-07 */
void acl_pfn_state_changed_ev_handler(RvSipRegClientHandle            hRegClient,
								RvSipAppRegClientHandle         hAppRegClient,
								RvSipRegClientState             eState,
								RvSipRegClientStateChangeReason eReason);

/* AlexChen SDK upgrade 20090413 */
/* ACL NB 080811 - added allow header filed in REGISTER Message*/
RvStatus Acl_pfnMsgToSendEvHandler(RvSipRegClientHandle          hRegClient,
                                   RvSipAppRegClientHandle       hAppRegClient,
                                   RvSipMsgHandle                hMsg);

/*--------------------U T I L I T Y   F U N C T I O N S -------------------*/
#ifdef ACL_ADV_SIP_FEATURES_DEBUG
static const RvChar*  acl_subs_state_name_get (
                           IN  RvSipSubsState  eState);
#endif

static RvMdmTerm*	acl_get_term_by_term_id ( char *termId);

/* AlexChen SDK upgrade 20090413 */
/*EA - 080807 - VI65537 - RFC 4240 - interop - dealing with remote parties that do not support REFER inside INVITE message*/
static RvMdmTerm*	AclAppGetTermByAppCallLeg (RvSipAppCallLegHandle hAppCallLeg);

void acl_message_print(IN RvSipMsgHandle hMsg, char*out, int maxsize);

extern int acl_validation_val_get(char *regVal);

/*===============================================================================*/
/*===================    S U B S C R I B E	  F U N C T I O N S    ==============*/
/*===============================================================================*/


/***************************************************************************
 * acl_sip_mwi_set_stack_cfg
 * ------------------------------------------------------------------------
 * General: Initializing the stack and allocating the application memory pool.
 *          When initializing we first set the configuration struct to the
 *          default values and then change some of the configuration parameters.
 *          We then use RvSipStackConstruct to initialize the stack.
 ***************************************************************************/
void acl_sip_mwi_set_stack_cfg( RvSipStackCfg* stackCfg)
{
   DBG_PRINT("\r\n");
	stackCfg->maxSubscriptions = 5;
	stackCfg->subsAutoRefresh  = RV_FALSE;
}

/***************************************************************************
 * acl_sip_advanced_init
 * ------------------------------------------------------------------------
 * General: Set application call back functions in the subscription manager.
 *			Also initializes a few internal handles
 ***************************************************************************/
void acl_sip_advanced_init( acl_sip_mwi_cfg_t* subsCfg, acl_call_mngr_params_t *call_mngr_params )
{
	RV_Status            rv;
	RvSipSubsEvHandlers  appEvHandlers;
	int                  i, iNumOfLines;

	DBG_PRINT(">> \r\n");

   iNumOfLines=voip_conf_get_number_of_lines(ANALOG_PORT_TYPE_FXO_FXS);
	/* Clear terminations */
	for (i= 0; i <iNumOfLines; i++ )
	{
	 	g_infoPerTerm[i].term = NULL;
	}


	/*store cfg data*/
	g_cfg.stackHandle				   = subsCfg->stackHandle;

	if(strlen(subsCfg->localAddress) < sizeof(g_cfg.localAddress))
	{
		strcpy(g_cfg.localAddress,		subsCfg->localAddress);
	}
	else
	{
   	 	printf("String length of localAddress %d is equal or bigger then sizeof localAddress %d\r\n",
					strlen(subsCfg->localAddress), sizeof(g_cfg.localAddress));
		return;
	}

	if(strlen(subsCfg->registrarAddress) < sizeof(g_cfg.registrarAddress))
	{
		strcpy(g_cfg.registrarAddress,	subsCfg->registrarAddress);
	}
	else
	{
   	 	printf("String length of registrarAddress %d is equal or bigger then sizeof registrarAddress %d\r\n",
					strlen(subsCfg->registrarAddress), sizeof(g_cfg.registrarAddress));
		return;
	}

	g_cfg.registrarPort				= subsCfg->registrarPort;
	g_cfg.stackUdpPort				= subsCfg->stackUdpPort;
	strcpy(g_cfg.subsServerName,	subsCfg->subsServerName);

   g_cfg.CSeq = 0;
   /**********************************************************/
   /* Get more configuration from acl_conf and store the data */
   /**********************************************************/
   voip_conf_mwi_get(&g_mwiParams);

   if (g_mwiParams.isEnabled)
   {
      DBG_PRINT("SipMwi is ENABLED \r\n");

      DBG_PRINT("localAddress     = %s\r\n",g_cfg.localAddress );
      DBG_PRINT("registrarAddress = %s\r\n",g_cfg.registrarAddress );
      DBG_PRINT("registrarPort    = %d\r\n",g_cfg.registrarPort );
      DBG_PRINT("stackUdpPort     = %d\r\n",g_cfg.stackUdpPort );

      if (g_mwiParams.subscribe2MWI)
      {
         if  ( strcmp(g_mwiParams.subsServerName, "")       &&
               strcmp(g_mwiParams.subsServerName, "0")      &&
               strcmp(g_mwiParams.subsServerName, "0.0.0.0")
             )
            sprintf( g_cfg.subsServerName ,	"%s:%d", g_mwiParams.subsServerName, g_mwiParams.subsServerPort );
         else
            /* If subsServer wasn't configure use the Proxy IP instead */
            sprintf( g_cfg.subsServerName ,	"%s:%d", call_mngr_params->sp.sip.proxy_address, call_mngr_params->sp.sip.proxy_port );

         DBG_PRINT("SipMwi is ENABLED \r\n");
         DBG_PRINT("subsServerName   = %s\r\n",g_cfg.subsServerName );
      }
      else
      {
         g_cfg.subsServerName[0] = '\0';
         DBG_PRINT("subscribe to MWI is DISABLED \r\n");
      }
   }
   else
   {
   	DBG_PRINT("SipMwi is DISABLED \r\n");
   }
   /*getting handles for the internal modules*/
	RvSipStackGetSubsMgrHandle(g_cfg.stackHandle, &g_hSubsMgr);

	RvSipStackGetLogHandle(g_cfg.stackHandle,&g_hLog);


	/*Construct a pool of memory for the application.
	* We're forced to construct one here because there is no access to such an object
	* constructed in sipMgr.c file of Toolkit
	*/
	g_appPool = RPOOL_Construct(1024,10,g_hLog,RV_FALSE,"ApplicationPool");


	/*Reset the appEvHandlers since not all callbacks are set by this sample*/
	memset(&appEvHandlers,0,sizeof(RvSipSubsEvHandlers));

	/*Set application callbacks in the structure*/
	appEvHandlers.pfnSubsCreatedEvHandler  = acl_subs_created_ev_handler;
	appEvHandlers.pfnStateChangedEvHandler = acl_subs_state_changed_ev_handler;
	appEvHandlers.pfnMsgReceivedEvHandler  = acl_subs_msg_received_ev_handler;
	appEvHandlers.pfnMsgToSendEvHandler    = acl_subs_msg_to_send_ev_handler;
	appEvHandlers.pfnNotifyEvHandler       = acl_subs_notify_ev_handler;
	appEvHandlers.pfnExpirationAlertEvHandler = acl_subs_expiration_alert_ev_handler;

	/*Set the structure in the subscription manager*/
	rv = RvSipSubsMgrSetEvHandlers(g_hSubsMgr,
			                          &appEvHandlers,
				   					  sizeof(RvSipSubsEvHandlers));

   /* Clear voip/ext_conf  */
   voip_conf_ext_sync_check_set(0);

   if(rv != RV_OK)
	{
		DBG_PRINT_ERR("Failed to set application callbacks");
	}

   DBG_PRINT("<< \r\n");

}

void acl_sip_advanced_start(void)
{
    /*received instruction from Radvision by emai "RE: Incident #127654: NOTIFY unsupported" at Nov 16th 2006 */
    RvSipControl*           sipMngrH = NULL;
    RvIppSipStackCallbacks *p_stackCallbacks = NULL;
    RvSipCallLegMgrHandle	hCallLegMgr;
    RvSipRegClientMgrHandle hRegClientMgr;
	RvSipTransportMgrHandle hTransportMgr; /* ER08 - Mini VPN */
    int                     rv;
   char                    stun_enabled;

    DBG_PRINT("acl_sip_advanced_start\r\n");

    sipMngrH = rvCCSipPhoneGetSipMgr();
    p_stackCallbacks = rvIppSipControlGetStackCallbacks( (RvIppSipControlHandle)sipMngrH);

    if (sipMngrH && p_stackCallbacks )
    {
		/* AlexChen SDK upgrade 20090413    */
        /* Register SIP Transport callbacks */
        /* -------------------------------- */
        /* If callback is not NULL, it means that MTF registered its implementation to this callback.
           If we proceed, we will override MTF implementation, which is not recommended.*/
        if(p_stackCallbacks->sipTransportEvHandlers.pfnEvBadSyntaxMsg != NULL)
        {
            IppLogMessage(RV_TRUE, "overrides MTF transport callback (BadSyntaxMsg)");
        }

   		/*ACL AL 081112 VI68005 Parsing error in 401 message caused the MP202 to ignore this message*/
         /* store the old callbackes in order to call them inside overriding callback */
        g_cfg.old_pfnEvBadSyntaxMsg = p_stackCallbacks->sipTransportEvHandlers.pfnEvBadSyntaxMsg;
        p_stackCallbacks->sipTransportEvHandlers.pfnEvBadSyntaxMsg = acl_sip_transport_bad_syntax_msgEv;
        RvSipStackGetTransportMgrHandle( g_cfg.stackHandle, &hTransportMgr);
        rv = RvSipTransportMgrSetEvHandlers( hTransportMgr,
                                              NULL,
                                              &p_stackCallbacks->sipTransportEvHandlers,
                                              sizeof(RvSipTransportMgrEvHandlers));

        if (rv != RV_OK)
        {
            IppLogMessage(RV_TRUE, "Failed to set application transport callbacks");
        }
		else
		    IppLogMessage(RV_TRUE, "Set application transport callbacks");

        /* get instruction from Radvision by emai "RE: Incident #127654: NOTIFY unsupported" at Nov 16th 2006 */
        RvSipStackGetTransactionMgrHandle( g_cfg.stackHandle, &(g_cfg.hTranscMgr));
        if (g_cfg.hTranscMgr)
        {
         DBG_PRINT(">> Set Transaction callbacks \r\n");
            /* store the old callbackes in order to call them inside overriding callback */
            g_cfg.old_pfnTransEvStateChanged = p_stackCallbacks->sipTransEvHandlers.pfnEvStateChanged;

            /* Set Transaction callbacks in the structure*/
            p_stackCallbacks->sipTransEvHandlers.pfnEvStateChanged = acl_trans_state_changed_ev_handler;

            rv = RvSipTransactionMgrSetEvHandlers( g_cfg.hTranscMgr,
                                                   (void *)g_hSubsMgr,/*NULL*/
                                                   &p_stackCallbacks->sipTransEvHandlers,
                                                   sizeof(RvSipTransactionEvHandlers));
            if(rv != RV_OK)
            {
                DBG_PRINT_ERR("ERROR: Failed to set application transaction callbacks");
            }
            else
            {
                DBG_PRINT("<< Set Transaction callbacks in the structure (g_cfg.hTranscMgr 0x%x)\r\n", g_cfg.hTranscMgr);
            }
        }
        else
            DBG_PRINT_ERR("ERROR: failed to get g_cfg.hTranscMgr\r\n");


        RvSipStackGetCallLegMgrHandle( g_cfg.stackHandle, &hCallLegMgr);
        if (hCallLegMgr)
        {
            DBG_PRINT(">> Set CallLeg callbacks \r\n");

            /* store the old callbackes in order to call them inside overriding callback */
            g_cfg.old_pfnReferStateChangedEvHandler = p_stackCallbacks->sipCallLegEvHandlers.pfnReferStateChangedEvHandler;
            g_cfg.old_pfnTranscRequestRcvdEvHandler = p_stackCallbacks->sipCallLegEvHandlers.pfnTranscRequestRcvdEvHandler;
            /* AlexChen SDK upgrade 20090413 */
			/*EA - 080722 - RFC 4240 - interop - dealing with remote parties that do not support REFER inside INVITE message*/
            g_cfg.old_pfnCallLegMsgToSendEvHandler= p_stackCallbacks->sipCallLegEvHandlers.pfnMsgToSendEvHandler;
            g_cfg.old_pfnCallLegMsgReceivedEvHandler = p_stackCallbacks->sipCallLegEvHandlers.pfnMsgReceivedEvHandler;
            g_cfg.old_pfnCallLegModifyStateChangedEvHandler = p_stackCallbacks->sipCallLegEvHandlers.pfnModifyStateChangedEvHandler;
            /*Set CallLeg callbacks in the structure*/
            p_stackCallbacks->sipCallLegEvHandlers.pfnReferStateChangedEvHandler = acl_call_leg_refer_state_shanged_ev_handler;
            p_stackCallbacks->sipCallLegEvHandlers.pfnTranscRequestRcvdEvHandler = acl_call_leg_transc_request_rcvd_ev_handler;
            /* AlexChen SDK upgrade 20090413 */
			/*EA - 080722 - RFC 4240 - interop - dealing with remote parties that do not support REFER inside INVITE message*/
            p_stackCallbacks->sipCallLegEvHandlers.pfnMsgToSendEvHandler = acl_app_call_leg_MsgToSendEvHandler;
            p_stackCallbacks->sipCallLegEvHandlers.pfnMsgReceivedEvHandler = acl_app_call_leg_MsgReceivedEvHandler;
            p_stackCallbacks->sipCallLegEvHandlers.pfnModifyStateChangedEvHandler = acl_app_call_leg_ModifyStateChangedEv;

            rv = RvSipCallLegMgrSetEvHandlers( hCallLegMgr, &p_stackCallbacks->sipCallLegEvHandlers, sizeof(RvSipCallLegEvHandlers));
            if(rv != RV_OK)
            {
                DBG_PRINT_ERR("ERROR: Failed to set application CallLeg callbacks");
            }
            else
            {
                DBG_PRINT("<< Set CallLeg callbacks in the structure (hCallLegMgr 0x%x)\r\n", hCallLegMgr);
            }

        }
        else
            DBG_PRINT_ERR("ERROR: failed to get hCallLegMgr\r\n");

        RvSipStackGetRegClientMgrHandle(g_cfg.stackHandle,  &hRegClientMgr);
        if (hRegClientMgr)
        {
            DBG_PRINT(">> Set RegClientMgr callbacks \r\n");

            /* store the old callbackes in order to call them inside overriding callback */
            g_cfg.old_pfnStateChangedEvHandler = p_stackCallbacks->sipRegClientEvHandlers.pfnStateChangedEvHandler;
            /* AlexChen SDK upgrade 20090413 */
			g_cfg.old_pfnMsgToSendEvHandler    = p_stackCallbacks->sipRegClientEvHandlers.pfnMsgToSendEvHandler;  /* ACL NB 080811 */
            /*Set RegClientMgr callbacks in the structure*/
            p_stackCallbacks->sipRegClientEvHandlers.pfnStateChangedEvHandler = acl_pfn_state_changed_ev_handler;
        	/* AlexChen SDK upgrade 20090413 */
			p_stackCallbacks->sipRegClientEvHandlers.pfnMsgToSendEvHandler    = Acl_pfnMsgToSendEvHandler;       /* ACL NB 080811 */

            rv = RvSipRegClientMgrSetEvHandlers( hRegClientMgr,
                                                 &p_stackCallbacks->sipRegClientEvHandlers,
                                                 sizeof(RvSipRegClientEvHandlers));

            if(rv != RV_OK)
            {
                DBG_PRINT_ERR("ERROR: Failed to set application RegClientMgr callbacks");
            }
            else
            {
                DBG_PRINT("<< Set RegClientMgr callbacks in the structure (RegClientMgr 0x%x)\r\n", hRegClientMgr);
            }

        }
        else
            DBG_PRINT_ERR("ERROR: failed to get hRegClientMgr\r\n");

    }
    else
        DBG_PRINT_ERR("ERROR:failed to get sipMngrH or p_stackCallbacks\r\n");
   /*Added Stun Client*/
   voip_conf_stun_enable_get(&stun_enabled);

#ifdef RV_MTF_STUN
   if (stun_enabled)
   {
      DBG_PRINT("STUN is ENABLED \r\n");
      stunClientInit( g_cfg.stackHandle );
   }
 #endif

    return;
}


/*
 *	Start subscribtion per terminal.
 *  Can support maximum 1 terminal currently
 */
void acl_adv_sip_register_mdm_term( int termIndex, RvMdmTerm*	termination)
{
   char              *destCfw=NULL;
   //char              myDestCfw[RV_IPP_ADDRESS_SIZE];
   int               myDestCfwLen;
   voip_cfw_params_t cfwParams;

   /**********************************************************/
   /* Get more configuration from acl_conf and store the data */
   /**********************************************************/
   voip_conf_cfw_type_get(&cfwParams);

	if(termination)
	{
      DBG_PRINT("Termination %s \r\n", rvMdmTermGetId(termination));

      g_infoPerTerm[termIndex].term = termination;
      if (g_mwiParams.isEnabled)
      {
         //ACL NB 061107
         g_infoPerTerm[termIndex].state= 0;
         if (g_mwiParams.subscribe2MWI)
         {
  	         if(strcmp(g_cfg.registrarAddress,""))
		         sprintf( g_infoPerTerm[termIndex].from, "sip:%s@%s:%d", rvMdmTermGetId(termination),g_cfg.registrarAddress, ((g_cfg.registrarPort)? g_cfg.registrarPort :5060) );
	         else
		         sprintf( g_infoPerTerm[termIndex].from, "sip:%s@%s:%d", rvMdmTermGetId(termination),g_cfg.localAddress, g_cfg.stackUdpPort );

            sprintf( g_infoPerTerm[termIndex].to, "sip:%s", g_cfg.subsServerName);

             /*3. If need to Subscribe for MWI - Create a subscription.*/
            sample_subscribe( &g_infoPerTerm[termIndex]);
         }
         else
         {
            strcpy( g_infoPerTerm[termIndex].from , "");
            strcpy( g_infoPerTerm[termIndex].to , "");
         }
      }
      else
      {
         DBG_PRINT("MWI is DISABELD\r\n");
      }

      DBG_PRINT("CFW params \r\n cfwType=%d, cfnrTimeout %d, keySequence %s\n ",cfwParams.cfwType, cfwParams.cfnrTimeout,cfwParams.keySequence);
      if ( voip_conf_ext_cfw_dest_get(termIndex, &destCfw) != 0 )
      {
         DBG_PRINT("voip_conf_ext_cfw_dest_get Failed \r\n") ;
         return;
      }
      myDestCfwLen = strlen(destCfw);
      DBG_PRINT("voip_conf_ext_cfw_dest_get dest=%s strlen(destCfw)=%d \r\n",destCfw , myDestCfwLen);

      if (cfwParams.isEnabled)
      {

         if (myDestCfwLen == 0)
         {
            DBG_PRINT("CFW is not active\r\n");
         }
         else
         {
            RvCCTerminal *xTerm = (RvCCTerminal*)termination->xTerm;

            if ( AclCfwHandle( xTerm, cfwParams.cfwType, cfwParams.cfnrTimeout, destCfw, myDestCfwLen) != 0)
            {
               DBG_PRINT("AclCfwHandle failed\r\n");
            }
         }
      }
      else
      {
         DBG_PRINT("CFW is DISABELD\r\n");
         if (myDestCfwLen != 0)
         {
            DBG_PRINT("CFW data should be cleared\r\n");

            // Clear from acl_conf
            if ( voip_conf_ext_cfw_dest_set(termIndex, "") != 0)
            {
               DBG_PRINT("Warning CFW destination could not be updated in ext_conf to re-use after re-start (termIndex=%d) \r\n", termIndex);
            }
         }
      }

	}
}


/***************************************************************************
 * sample_subscribe
 * ------------------------------------------------------------------------
 * General: Create a subscription.
 *          To Create a subscription the application should:
 *          1. create a new subscription using RvSipSubsMgrCreateSubscription().
 *          2. call RvSipSubsInitStr(), to set mandatory parameters.
 *          3. call RvSipSubsSubscribe(). This will cause the subscribe message
 *             to be sent to the destination.
 *----------------------------------------------------------------------
 *
 ***************************************************************************/
static void sample_subscribe( info_per_terminal_t* pSubsPerTerm)
{
	RvSipSubsHandle         hSubs;
    RvStatus               rv;

	/*--------------------------
	  creating a new subscription
	----------------------------*/
	rv = RvSipSubsMgrCreateSubscription(g_hSubsMgr, NULL, (RvSipAppSubsHandle)pSubsPerTerm, &hSubs);
    if(rv != RV_OK)
	{
		DBG_PRINT_ERR("Failed to create new subscription");
		return;
	}
	DBG_PRINT("Subscriber subscription %x was created for 0x%x\n\n",(int)hSubs, pSubsPerTerm);

	/*------------------------------------------------------------
	  Call the init function with the To and From addresses, event
	  and expires values, in order to init and send the subscription.
	-------------------------------------------------------------*/
	DBG_PRINT("Subscriber subscription from %s to %s\n\n",pSubsPerTerm->from, pSubsPerTerm->to);
	rv = RvSipSubsInitStr(hSubs, pSubsPerTerm->from, pSubsPerTerm->to, g_mwiParams.expiraitionTimeout , EVENT);
    if(rv != RV_OK)
	{
		DBG_PRINT_ERR("Subscription init failed. rv= %d", rv);
		return;
	}
    DBG_PRINT("Sending SUBSCRIBE: \n\t%s -> %s\n\n",pSubsPerTerm->from,pSubsPerTerm->to);
    rv = RvSipSubsSubscribe(hSubs);
    if(rv != RV_OK)
    {
        DBG_PRINT_ERR("subscribing failed.");
		return;
    }
    pSubsPerTerm->state = 1;
	 pSubsPerTerm->hSubs = hSubs;

}

/***************************************************************************
 * RedundantProxy_Init
 * ------------------------------------------------------------------------
 * General:			init redundant proxy data base recieved from user.
 * ------------------------------------------------------------------------
 * Arguments:		pMdmCfg - the input configuration from web.
 *
 * return value:	none.
 ***************************************************************************/
/* AlexChen 20090401 SDK upgrade redundant proxy feature api */
void RedundantProxy_Init(IN acl_call_mngr_params_t *pMdmCfg)
{
	DBG_PRINT("->\r\n");
    if(pMdmCfg->sp.sip.use_redundant_proxy &&
        (pMdmCfg->sp.sip.use_registrar||(pMdmCfg->sp.sip.use_proxy && pMdmCfg->sp.sip.use_proxy_port_ip_for_registrar)))
    {
        RedundantProxyCfg.enabled = 1;
        RedundantProxyCfg.enabled = pMdmCfg->sp.sip.use_redundant_proxy;
        AC_STRNCPY(RedundantProxyCfg.IpAddress, pMdmCfg->sp.sip.redundant_proxy_address, sizeof(RedundantProxyCfg.IpAddress) - 1);
        RedundantProxyCfg.IsSymmetric = pMdmCfg->sp.sip.redundant_proxy_is_symmetric;
        RedundantProxyCfg.KA_OptionRate = pMdmCfg->sp.sip.redundant_proxy_ka_period*1000;
        RedundantProxyCfg.PortNumber = pMdmCfg->sp.sip.redundant_proxy_port;
        RedundantProxyCfg.IsRedundant = voip_conf_ext_is_redundant_proxy_get();
    }else
    {
        RedundantProxyCfg.enabled = 0;
        strcpy(RedundantProxyCfg.IpAddress, "0.0.0.0");
        RedundantProxyCfg.IsRedundant = 0;
        RedundantProxyCfg.IsSymmetric = 0;
        RedundantProxyCfg.KA_OptionRate = 0;
        RedundantProxyCfg.PortNumber = 0;
        RedundantProxyCfg.KATranscHandle = NULL;
        voip_conf_ext_is_redundant_proxy_set(0);
    }
    RedundantProxyCfg.IsInit = 1;
	DBG_PRINT("<-\r\n");

}
/***************************************************************************
 * RedundantProxy_SwitchRegistrar
 * ------------------------------------------------------------------------
 * General:			switch the current registrar proxy to alternate
 *					(primary or redundant)
 * ------------------------------------------------------------------------
 * Arguments:		none.
 *
 * return value:	none.
 ***************************************************************************/
/* AlexChen 20090401 SDK upgrade redundant proxy feature api */
void RedundantProxy_SwitchRegistrar(void)
{	/* it can take up to 10 seconds before the switch is actually executed.
	   during that time we want to avoid farther switching of proxy.
	   when the switch is executed, the voip task restarts and this value is reset */
	DBG_PRINT("->\r\n");
	/* at first time there is no such path Sis_redundat_proxy so we get 0 which means primary proxy by default*/
	int IsRedundatProxy = RedundantProxy_RegistrarIsRedundant();
	DBG_PRINT("switching to %s proxy\r\n", IsRedundatProxy?"primary":"redundant");
	voip_conf_ext_is_redundant_proxy_set(!IsRedundatProxy);
	//voip_conf_ext_voip_restart_needed_set(1);
	/* in sdk we run voip_task by ourself so just exit */
	printf("restart voip_task by ./voip_task -r cfgfile\n");
	exit(0);
	DBG_PRINT("<-\r\n");
}
/***************************************************************************
 * RedundantProxy_HandleSwitchRegistrar
 * ------------------------------------------------------------------------
 * General:			this is the feature's controller, implementing the
 *                  feature's state machine. Receive events from RV
 *                  and decide the action to be done according the event and
 *                  the feature current configuration. a full description of
 *                  the state machine can be found at pages 3-4 at the
 *                  Redundant Proxy design documment.
 * ------------------------------------------------------------------------
 * Arguments:		event - Redundant proxy state machine event.
 *
 * return value:	none.
 ***************************************************************************/
void RedundantProxy_HandleSwitchRegistrar(redundant_proxy_events_e event)
{
#ifdef ACL_ADV_SIP_FEATURES_DEBUG
    static code2str_t RP_code2str_events[] = {
        {e_REGISTRAR_KA_RESPONSED, "Registrar responded to KA message", 0, 0},
        {e_REGISTRAR_KA_NOT_RESPONDED, "Registrar not responded to KA message", 0, 0},
        {e_REGISTRAR_REGISTRATION_NOT_RESPONDED, "Registrar registration failed", 0, 0},
        {-1}
    };
#endif
    int IsSymmetric = RedundantProxy_RegistrarIsSymmetric();
    int IsRedundant = RedundantProxy_RegistrarIsRedundant();

    if(!RedundantProxyCfg.IsInit)
    {
        printf("[%s %d] ERROR:redundant proxy feature hasn't been initialiezed yet\r\n", __FUNCTION__, __LINE__);
        return;
    }
    DBG_PRINT("->\r\n");
    DBG_PRINT("Registrar (%s) Mode (%s) Event (%s)\r\n",
        IsRedundant?"Redundant":"Primary", IsSymmetric?"Symmetric":"A-symmetric",code2str(RP_code2str_events, event));
    switch(event) {

    case e_REGISTRAR_KA_RESPONSED:
        if(IsRedundant && !IsSymmetric)
            RedundantProxy_SwitchRegistrar();
        else
            ;//do nothing
    	break;

    case e_REGISTRAR_KA_NOT_RESPONDED:
        if((!IsSymmetric && IsRedundant))
            ;//do nothing
        else
            RedundantProxy_SwitchRegistrar();
    	break;

    case e_REGISTRAR_REGISTRATION_NOT_RESPONDED:
            RedundantProxy_SwitchRegistrar();
            break;

    default:
        DBG_PRINT("ERROR: Invalid event!!!\r\n");
        break;

    }
    DBG_PRINT("<-\r\n");
}

/***************************************************************************
 * RedundantProxy_RegistrarIsRedundant
 * ------------------------------------------------------------------------
 * General:			return the registrar entity - primary or redundant proxy
 * ------------------------------------------------------------------------
 * Arguments:		none.
 *
 * return value:	0 - primary proxy 1 - redundant proxy
 *                  -1 feature not initiated
 ***************************************************************************/
int RedundantProxy_RegistrarIsRedundant(void)
{
    int IsRedundant = RedundantProxyCfg.IsRedundant;

    if(!RedundantProxyCfg.IsInit)
    {
        printf("[%s %d] ERROR:redundant proxy feature hasn't been initialiezed yet\r\n", __FUNCTION__, __LINE__);
        return -1;
    }

	/* DBG_PRINT("registrar is %s\r\n", IsRedundant?"redundant":"primary"); */
	return IsRedundant;
}

/***************************************************************************
 * RedundantProxy_GetKAOptionRate
 * ------------------------------------------------------------------------
 * General:			return keep alive option rate
 * ------------------------------------------------------------------------
 * Arguments:		none.
 *
 * return value:	keep alive option rate in ms
 *                  -1 feature not initiated
 ***************************************************************************/
int RedundantProxy_GetKAOptionRate(void)
{
    int KA_period = RedundantProxyCfg.KA_OptionRate;

    if(!RedundantProxyCfg.IsInit)
    {
        printf("[%s %d] ERROR:redundant proxy feature hasn't been initialiezed yet\r\n", __FUNCTION__, __LINE__);
        return -1;
    }

	/* DBG_PRINT("KAOptionRate is %d\r\n", KA_period); */
	return KA_period;
}

/***************************************************************************
 * RedundantProxy_RegistrarIsSymmetric
 * ------------------------------------------------------------------------
 * General:			return the redundnat proxy mode
 * ------------------------------------------------------------------------
 * Arguments:		none.
 *
 * return value:	0 - assymtric mode 1 - symmetric mode
 *                  -1 feature not initiated
 ***************************************************************************/
int RedundantProxy_RegistrarIsSymmetric(void)
{
    int IsSymmetric = RedundantProxyCfg.IsSymmetric;

    if(!RedundantProxyCfg.IsInit)
    {
        printf("[%s %d] ERROR:redundant proxy feature hasn't been initialiezed yet\r\n", __FUNCTION__, __LINE__);
        return -1;
    }

	/* DBG_PRINT("registrar is in %s mode\r\n", IsSymmetric?"symmetric":"a-symmetric"); */
	return IsSymmetric;
}

/***************************************************************************
 * RedundantProxy_IsEnabled
 * ------------------------------------------------------------------------
 * General:			Return the Redundant Proxy feature mode: disabled/enbaled
 * ------------------------------------------------------------------------
 * Arguments:		none.
 *
 * return value:	0 - disabled 1 - enabled
 *                  -1 feature not initiated
 ***************************************************************************/
int RedundantProxy_IsEnabled(void)
{
    int IsEnabled = RedundantProxyCfg.enabled;

    if(!RedundantProxyCfg.IsInit)
    {
        printf("[%s %d] ERROR:redundant proxy feature hasn't been initialiezed yet\r\n", __FUNCTION__, __LINE__);
        return -1;
    }

	/* DBG_PRINT("redundant proxy is %s \r\n", IsEnabled?"enabled":"disabled"); */

    return IsEnabled;
}
/***************************************************************************
 * RedundantProxy_getPort
 * ------------------------------------------------------------------------
 * General:			Return the Redundant Proxy port number
 * ------------------------------------------------------------------------
 * Arguments:		none.
 *
 * return value:	int - redundant proxy port number
 *                  -1 feature not initiated
 ***************************************************************************/
int RedundantProxy_getPort(void)
{
    int port = RedundantProxyCfg.PortNumber;

    if(!RedundantProxyCfg.IsInit)
    {
        printf("[%s %d] ERROR:redundant proxy feature hasn't been initialiezed yet\r\n", __FUNCTION__, __LINE__);
        return -1;
    }

    	/* DBG_PRINT("redundant proxy port is %d \r\n", port); */
	return port;
}
/***************************************************************************
 * RedundantProxy_getAdrr
 * ------------------------------------------------------------------------
 * General:			Return the Redundant Proxy address
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:           none
 * output           none
 *
 * return value:	pointer to redundant proxy address string
 *                  NULL if the feature is not initiated
 ***************************************************************************/
const char *RedundantProxy_getAdrr(void)
{
    char *Address = RedundantProxyCfg.IpAddress;

    if(!RedundantProxyCfg.IsInit)
    {
        printf("[%s %d] ERROR:redundant proxy feature hasn't been initialiezed yet\r\n", __FUNCTION__, __LINE__);
        Address = NULL;
    }

     /* DBG_PRINT("redundant proxy address is %s \r\n", Address); */
     return Address;

}
/*EA - 080114 - VI#60461 - Bezeq Interop*/
static int aclInfoMessageHandler(  RvSipCallLegHandle     hCallLeg,
							RvSipAppCallLegHandle  hAppCallLeg,
							RvSipTranscHandle      hTransc,
							RvSipMsgHandle         hMsg,
							char *methodStr)
{
	char        	*buf = NULL;
	int 	    	bodylen;
	unsigned int 	    	actlen = 0;
	RvStatus  rv;

	bodylen = RvSipMsgGetStringLength(hMsg, RVSIP_MSG_BODY);
	if(bodylen == 0)
	{
		DBG_PRINT(" No BODY in INFO message %p",hMsg);
	}
	else if ((buf = (RV_CHAR*)rvAllocAllocate(prvDefaultAlloc, bodylen*sizeof(char))) == NULL)
	{
		DBG_PRINT("Failed to allocate memory to get a BODY of INFO message %p",hMsg);
	}
	else
	{
		rv = RvSipMsgGetBody(hMsg, buf, bodylen, &actlen);
		if ((rv == RV_OK) && (actlen != 0))
		{
			char    contentTypeBuf[30];
			unsigned int		len = 0;

			/* Process the body of the message */
			rv = RvSipMsgGetContentTypeHeader(hMsg, contentTypeBuf, sizeof(contentTypeBuf), &len);
			if ((rv == RV_OK) && (len != 0))
			{
				if(!strncmp(contentTypeBuf,"application/dtmf-relay", len))
				{
					/* dtmf-relay is handled by RV pfnTranscRequestRcvdEvHandler */
					return ACL_CALL_RV_CB;
				}
				if(!strncmp(contentTypeBuf,"application/broadsoft", len))
				{
					rv = broadsoft_body_parse(hAppCallLeg, buf, bodylen);
				}
				else if (!strncmp(contentTypeBuf,"application/sscc", len))
				{
					rv = sscc_body_parse(hAppCallLeg, buf, bodylen);
				}
				if (rv != RV_OK)
				{
					DBG_PRINT_ERR("parsing failed (hAppCallLeg=%p,hTransc=%p", hAppCallLeg, hTransc);
				}
				else
				{
					/*send the response from the calling function*/
				}
			}
		}

		/* Release allocated memory */
		if (buf != NULL)
			rvAllocDeallocate(prvDefaultAlloc, bodylen*sizeof(char), buf);
	}
	return ACL_DONT_CALL_RV_CB;
}

/*===============================================================================*/
/*==============    C A L L B A C K		I M P L E M E N T A T I O N S  ==========*/
/*===============================================================================*/
/*  get instruction from Radvision by email "RE: Incident #127654: NOTIFY unsupported" at Nov-16th-2006 */
/*  get instruction from Radvision by email "Incident #128424:  OPTIONS request method" 08-Jan-07 */
void RVCALLCONV  acl_trans_state_changed_ev_handler(
         IN RvSipTranscHandle                 hTransc,
         IN RvSipTranscOwnerHandle            hTranscOwner,
         IN RvSipTransactionState             eState,
         IN RvSipTransactionStateChangeReason eReason)
{
   RvStatus rv        =  RV_OK;
   SipTransactionMethod eMethod;
   RV_CHAR methodStr[20] = {0};
   int                  responeCode=0;


 /*
  * do my work first
  */
   DBG_PRINT("trans state change %d %d %p\n", eState, eReason, hTransc);
 	SipTransactionGetMethod(hTransc,&eMethod);
	RvSipTransactionGetMethodStr(hTransc,20,methodStr);
   switch(eState)
   {
      case  RVSIP_TRANSC_STATE_SERVER_GEN_REQUEST_RCVD:
         //SipTransactionGetMethod(hTransc,&eMethod);
  	      DBG_PRINT("state %d reason  %d 0x%x hTranscOwner 0x%x eMethod=%d \r\n",eState, eReason, hTransc,hTranscOwner, eMethod);
         switch(eMethod)
         {
            /******************************************************************/
            /*             SIP_TRANSACTION_METHOD_NOTIFY                      */
            /******************************************************************/
            case SIP_TRANSACTION_METHOD_NOTIFY:
               DBG_PRINT("get METHOD_NOTIFY \r\n");
               RvSipMsgHandle     hNotifyMsg;
               rv = RvSipTransactionGetReceivedMsg(hTransc, &hNotifyMsg);
               if (hNotifyMsg)
               {
                  rv= notification_handle( hNotifyMsg);
                  if(rv != RV_OK)
                  {
                     DBG_PRINT("notification_handle-  failure\r\n");
                     responeCode = 400; // bad request
                  }
                  else
                     responeCode = 200; // OK
               }
               /*-------------------------
                 Send the response message.
                --------------------------*/
               rv = RvSipTransactionRespond(hTransc,responeCode,NULL);
               if (rv != RV_OK)
               {
                  DBG_PRINT("SubsNotifyRespond -  Failed to send 200 response (status %d)", rv);
               }
               return;
               break;

            /******************************************************************/
            /*             SIP_TRANSACTION_METHOD_OTHER                       */
            /******************************************************************/
            case SIP_TRANSACTION_METHOD_OTHER:
				DBG_PRINT("get METHOD_OTHER \r\n");

				RvSipTransactionGetMethodStr(hTransc,20,methodStr);
				if ( strcmp( methodStr,"OPTIONS" ) == 0 )
				{
				  /* retrieve information? */
				  rv = RvSipTransactionRespond(hTransc,200,NULL);

				}/* AlexChen SDK upgrade 20090413 */
				else
				{
					rv = RvSipTransactionRespond(hTransc,RV_SIPCTRL_STATUS_NOTIMPLEMENT,NULL);
				}

				if(rv != RV_OK)
                {
                    DBG_PRINT("Failed to response to OPTIONS request");
                }
                return;
               break;

            default:
               break;
         }
         break;
		 /* AlexChen 20090402 SDK upgrade JL check if those are all the cases!!!*/
	case RVSIP_TRANSC_STATE_CLIENT_GEN_FINAL_RESPONSE_RCVD: //I have to change this code to make method other more generic JL @~@#
	case RVSIP_TRANSC_STATE_CLIENT_GEN_PROCEEDING:
		if(SIP_TRANSACTION_METHOD_OTHER == eMethod)
		{
			DBG_PRINT("get METHOD_OTHER \r\n");
			if ( strcmp( methodStr,"OPTIONS" ) == 0 )
			{/*JL VI # 62393 redundant proxy feature */
				DBG_PRINT("recieved response to  OPTIONS message hTransc(0x%x) redundant proxy hTransc(0x%x)\r\n",
					hTransc, RedundantProxyCfg.KATranscHandle);
				if(hTransc == RedundantProxyCfg.KATranscHandle)
				{
					DBG_PRINT("Redundant proxy keep-alive message \r\n");
                    RedundantProxy_HandleSwitchRegistrar(e_REGISTRAR_KA_RESPONSED);
                }
			}
		}
	break;
	case RVSIP_TRANSC_STATE_TERMINATED:
		DBG_PRINT("RVSIP_TRANSC_STATE_TERMINATED \r\n");
		if(SIP_TRANSACTION_METHOD_OTHER == eMethod)
		{
			DBG_PRINT("get METHOD_OTHER \r\n");
			if ( strcmp( methodStr,"OPTIONS" ) == 0 )
			{/*JL VI # 62393 redundant proxy feature */
				DBG_PRINT("OPTIONS transaction\r\n");
				if(hTransc == RedundantProxyCfg.KATranscHandle)
				{
					DBG_PRINT("redundant proxy transaction\r\n");
					if(RVSIP_TRANSC_REASON_TIME_OUT == eReason)
					{
                        if(RedundantProxy_IsEnabled())
                            RedundantProxy_HandleSwitchRegistrar(e_REGISTRAR_KA_NOT_RESPONDED);
                    }
				}
			}
		}
      default:
         break;
   }

   if( g_cfg.old_pfnTransEvStateChanged)
   {
      //DBG_PRINT("***** old_pfnTransEvStateChanged \r\n");
      g_cfg.old_pfnTransEvStateChanged( hTransc, hTranscOwner, eState, eReason);
   }

   return;
}


static void RVCALLCONV acl_call_leg_refer_state_shanged_ev_handler(
                                                        IN  RvSipCallLegHandle             hCallLeg,
                                                        IN  RvSipAppCallLegHandle          hAppCallLeg,
                                                        IN  RvSipCallLegReferState         eState,
                                                        IN  RvSipCallLegStateChangeReason  eReason)
{
    RvCCConnection* c = (RvCCConnection*)hAppCallLeg;
    RvCCTerminal* term   = rvCCConnSipGetTerminal(c);
    RvCCTerminalSip* terminal=NULL;
    RvMdmTerm* mdmTerm;
    char *termId;

    terminal = rvCCTerminalSipGetImpl(term);
    if (terminal== NULL)
    {
        DBG_PRINT("failed to get terminal\r\n");
            goto Error;
    }

    /*
        EA - 071025 - fix for VI 58204 "blind transfer is't working on speed dial"
        In some scenarios (usually on direct calling) the mdm terminal field in the
        RvCCTerminalSip is already cleared bacause the connection was already destroyed
        we still have to get the mdmTerm in order to simulate the onhook.
        we get the mdmterm using the termId.
    */
    termId = rvCCTerminalSipGetTermId(terminal);
    mdmTerm = acl_get_term_by_term_id(termId);

    DBG_PRINT("call-leg %x - Refer State changed to %d, reason:%d termId=%s \r\n", (int)hCallLeg,eState, eReason, termId);

    switch (eState)
    {
    case RVSIP_CALL_LEG_REFER_STATE_REFER_IDLE:
        // If we receive remote accept and key sequence is flash only
        // we should sim on Hook event
        DBG_PRINT("refer idle\r\n");

        if (RVSIP_CALL_LEG_REASON_REMOTE_ACCEPTED == eReason)
        {
            sip_refer_received_remote_accepted_handler(mdmTerm);
            DBG_PRINT("refer idle- RVSIP_CALL_LEG_REASON_REMOTE_ACCEPTED\r\n");
        }
        break;
    default:
        break;

    }

Error:
    if( g_cfg.old_pfnReferStateChangedEvHandler)
    {
        DBG_PRINT("Calling old_pfnReferStateChangedEvHandler \r\n");
        g_cfg.old_pfnReferStateChangedEvHandler( hCallLeg, hAppCallLeg, eState, eReason);
    }

    return;

}

/***************************************************************************
 * acl_call_leg_transc_request_rcvd_ev_handler
 * ------------------------------------------------------------------------
 * General: handle special "INFO" behavior else calls RV AppCallLegTranscRequestRcvdEvHandler
 * Return Value: (-)
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:     hCallLeg -    The sip stack call-leg handle
 *            hAppCallLeg - The application handle for this call-leg.
 *            hTransc -     The stack handle to the transaction.
 *          hAppTransc -  The application handle to the transaction.
 *          eTranscState - The new state of the call-leg transaction.
 *          eReason      - The reason for the new state.
 * Output: (-).
 ***************************************************************************/
static void RVCALLCONV acl_call_leg_transc_request_rcvd_ev_handler(
                                   IN  RvSipCallLegHandle                hCallLeg,
                                   IN  RvSipAppCallLegHandle             hAppCallLeg,
                                   IN  RvSipTranscHandle                 hTransc,
                                   IN  RvSipAppTranscHandle              hAppTransc)


{
	RvStatus             rv;
	RvChar         methodStr[20] = {0};
	RvSipMsgHandle hMsg;


    RV_UNUSED_ARG(hAppTransc);
	rv = RvSipTransactionGetMethodStr(hTransc,20,(RvChar *)methodStr);

	if (rv != RV_OK)
   {
      DBG_PRINT("RvSipTransactionGetMethodStr failed with err %d",rv);
      return ;
	}

	rv = RvSipTransactionGetReceivedMsg(hTransc, &hMsg);
	if (rv != RV_OK)
   {
	   DBG_PRINT(" RvSipTransactionGetReceivedMsg failed with err %d",rv);
	   return ;
	}

	/*process specific INFO message only*/
	if (!strcmp(methodStr,"INFO"))
	{
       		/*EA - 080114 - VI#60461 - Bezeq Interop*/
		rv = aclInfoMessageHandler(hCallLeg, hAppCallLeg, hTransc, hMsg, methodStr);
		if (ACL_DONT_CALL_RV_CB == rv)
		{
			rv = RvSipCallLegTranscResponse(hCallLeg, hTransc,200);
			if (rv != RV_OK)
				DBG_PRINT_ERR("RvSipCallLegTranscResponse(hCallLeg=%p,hTransc=%p)=%d", hCallLeg, hTransc, rv);
			return;
		}
	}

   /* All other Method or INFO that wasn't handled by ACL will be handled by RV*/
   if (g_cfg.old_pfnTranscRequestRcvdEvHandler)
   {
      g_cfg.old_pfnTranscRequestRcvdEvHandler(hCallLeg, hAppCallLeg, hTransc, hAppTransc);
   }

   return;
}

/* AlexChen SDK upgrade 20090413 */
/*ACL XY 090115 VI69874 parsing error due to a missing comma in the OK message*/
static void acl_date_header_bad_syntax_handler(  IN RvSipMsgHandle    hMsgReceived,
                                            IN RvSipDateHeaderHandle hDate,
                                            OUT   RvSipTransportBsAction    *peAction )
{
    int strLen = 0;
    char date[128];
    int actualLen = 0;
    RvStatus   rv = RV_OK;

    strLen = RvSipDateHeaderGetStringLength(hDate,RVSIP_DATE_BAD_SYNTAX);

    if( strLen < 128 )
    {
        rv = RvSipDateHeaderGetStrBadSyntax(hDate,date,strLen,&actualLen);
        if ((rv == RV_OK) && (actualLen!= 0))
        {
            char *p;
            /*the regular "date" line:
            Date: Wed, 14 Jan 2009 12:02:38 GMT
            if get the date line without comma,we accept such Date header field format*/
            p = strchr(date, ',');

            if(!p)
              *peAction = RVSIP_TRANSPORT_BS_ACTION_CONTINUE_PROCESS;
        }
    }

    return;
}

/*ACL AL 081112 VI68005 Parsing error in 401 message caused the MP202 to ignore this message*/
/*ACL XY 090115 VI69874 parsing error due to a missing comma in the OK message*/
/***************************************************************************
 * acl_sip_transport_bad_syntax_msgEv
 * ------------------------------------------------------------------------
 * General:  This is an implementation of SIP callback RvSipTransportBadSyntaxMsgEv().
 *			 This callback notifies the application that a new bad-syntax message is received.
 *           The application can fix the message in this callback and no later.
 *           The application should use the eAction parameter to decide how
 *           stack will handle this message: discard it, continue with message
 *           processing, or send 400 response (in case of request message).
 *           see the RvSipTransportBsAction definition for more details.
 *           If application did not implemented this callback, the bad-syntax
 *           message will be discard.
 * Return Value: RV_Success.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: 	hTransportMgr    - A handle to the transport manager object.
 *   	      hAppTransportMgr - The application handle. You supply this handle
 *                             when setting the event handles
 *          hMsgReveived     - The received bad-syntax message.
 * Output:  peAction         - User decision of stack way of handling this message.
 ***************************************************************************/
static RV_Status RVCALLCONV acl_sip_transport_bad_syntax_msgEv(
                      IN    RvSipTransportMgrHandle    hTransportMgr,
                      IN    RvSipAppTransportMgrHandle hAppTransportMgr,
                      IN    RvSipMsgHandle             hMsgReceived,
                      OUT   RvSipTransportBsAction    *peAction)
{
    RvSipAuthenticationHeaderHandle authHeader;
    RvSipHeaderListElemHandle       hListElem;
    RvUint                          strLen      = 0;
    RvUint                          actualLen   = 0;
    RvStatus                        rv = RV_OK;
    char                            *buf = NULL;
    char                            *pStale = NULL;
    char                            require[128+8];
    int                             requireLen = strlen("Require:");
    RvSipOtherHeaderHandle          hRequire;
    RvSipDateHeaderHandle hDate; /*ACL XY 090115 VI69874*/

    DBG_PRINT(">>");

	authHeader = RvSipMsgGetHeaderByTypeExt(hMsgReceived, RVSIP_HEADERTYPE_AUTHENTICATION,RVSIP_FIRST_HEADER,RVSIP_MSG_HEADERS_OPTION_ALL, &hListElem);

	if(authHeader != NULL)
	{
		strLen = RvSipAuthenticationHeaderGetStringLength(authHeader,RVSIP_AUTHENTICATION_BAD_SYNTAX);
		if( strLen == 0 )
		{
			DBG_PRINT(" No bad syntax in Authentication header");
		}
		else if ((buf = (RV_CHAR*)rvAllocAllocate(prvDefaultAlloc, strLen*sizeof(char))) == NULL)
		{
			DBG_PRINT("Failed to allocate memory");
		}
		else
		{
			rv = RvSipAuthenticationHeaderGetStrBadSyntax(authHeader,buf,strLen,&actualLen);
			DBG_PRINT("rv = %d, actualLen = %d>>", rv, actualLen);
			if ((rv == RV_OK) && (actualLen!= 0))
			{
				DBG_PRINT("rv = %d, actualLen = %d>>", rv, actualLen);
                pStale = strstr(buf,"stale=\"true\"");
                if( pStale )
				{
                    char *pStaleReplaceStr="stale=TRUE,  ";
					strncpy(pStale,pStaleReplaceStr,strlen(pStaleReplaceStr));
					RvSipAuthenticationHeaderFix(authHeader,buf);
					*peAction = RVSIP_TRANSPORT_BS_ACTION_CONTINUE_PROCESS;
					DBG_PRINT("peAction = RVSIP_TRANSPORT_BS_ACTION_CONTINUE_PROCESS\r\n");
				}
                else
				{
					*peAction = RVSIP_TRANSPORT_BS_ACTION_DISCARD_MSG;
					DBG_PRINT("peAction = RVSIP_TRANSPORT_BS_ACTION_DISCARD_MSG\r\n");
				}
			}
		}
	}
	else
		DBG_PRINT("authHeader == NULL\r\n");

	/* Release allocated memory */
	if (buf != NULL)
		rvAllocDeallocate(prvDefaultAlloc, strLen*sizeof(char), buf);


    /*ACL XY 090112 VI67392 Anatel certification: fix issue SIP_CC_TE_CE_V_031
      allow all headers,include bad-syntax*/
    hRequire = RvSipMsgGetHeaderByNameExt(hMsgReceived,
                                        "Require",
                                        RVSIP_FIRST_HEADER,
                                        RVSIP_MSG_HEADERS_OPTION_ALL,
                                        &hListElem);

    /*RFC 3261 (8.2.2.3):
     ... If a UAS does not understand an option tag listed in a Require header field, it MUST respond by
     generating a response with status code 420 (Bad Extension). The UAS MUST add an Unsupported
     header field, and list in it those options it does not understand amongst those in the Require header
     field of the request */

    /*ACL XY 081204 VI 67392  - Anatel certification,Ensure that the IUT on receipt of an INVITE
    request with a Require header set to an option value that the IUT does not support, sends a Bad
    Extension (420 Bad Extension) response including those options in the Unsupported header.*/

    if( hRequire != NULL )
    {
        strLen = RvSipOtherHeaderGetStringLength(hRequire,RVSIP_OTHER_BAD_SYNTAX);

        if( strLen < 128 )
        {
            strcpy(require,"Require:");
            rv = RvSipOtherHeaderGetStrBadSyntax(hRequire,require+requireLen,strLen,&actualLen);
            if ((rv == RV_OK) && (actualLen!= 0))
            {

                /*fill the  require header,RV will go over the message require headers and add any unsupported
                   option to the unsupported list in the transaction,If the list is not empty the transaction will respond with 420
                   and will add all unsupported headers in the response message*/
                RvSipOtherHeaderParse(hRequire,require);

                *peAction = RVSIP_TRANSPORT_BS_ACTION_CONTINUE_PROCESS;
            }
        }
    }

    hDate = RvSipMsgGetHeaderByTypeExt(hMsgReceived,
                                        RVSIP_HEADERTYPE_DATE,
                                        RVSIP_FIRST_HEADER,
                                        RVSIP_MSG_HEADERS_OPTION_ALL,
                                        &hListElem);

    /*ACL XY 090115 VI69874 parsing error due to a missing comma in the OK message*/
    if(hDate != NULL)
       acl_date_header_bad_syntax_handler(hMsgReceived,hDate,peAction);

    if( g_cfg.old_pfnEvBadSyntaxMsg)
    {
       g_cfg.old_pfnEvBadSyntaxMsg( hTransportMgr, hAppTransportMgr, hMsgReceived, peAction);
    }

	return RV_Success;
}



void ReferToHeaderStrAllocNCopy(IN  RvSipReferToHeaderHandle    hReferToHeader,
                                OUT RvChar                      **hdrBuf)
{
    HPAGE       hPage;
    RvStatus    rv;
    RvUint32    length;

    rv = RvSipReferToHeaderEncode(hReferToHeader, g_appPool, &hPage, &length);
    if (rv != RV_OK)
    {
        DBG_PRINT_ERR("RvSipReferToHeaderEncode failed (err %d)\n", rv);
        return;
    }
    *hdrBuf = (RvChar *)AC_ZALLOC_L(length+1);

    rv = RPOOL_CopyToExternal(g_appPool, hPage, 0, *hdrBuf, length);
    /*EA - 081106 - VI67573 - There is a crash in call transfer (+ VI67811 + VI67407 duplicated)*/
    (*hdrBuf)[length] = '\0';
    if(rv != RV_OK)
    {
        DBG_PRINT_ERR("RPOOL_CopyToExternal failed (err %d)\n", rv);
        /*free the page the encode function allocated*/
        RPOOL_FreePage(g_appPool, hPage);
        nfree(hdrBuf);
    }
    /*perhaps free the page !!!*/
    RPOOL_FreePage(g_appPool, hPage);
}

void ReferredByHeaderStrAllocNCopy(IN  RvSipReferredByHeaderHandle      hReferredByHeader,
                                   OUT RvChar                           **hdrBuf)
{
    HPAGE       hPage;
    RvStatus    rv;
    RvUint32    length;

    rv = RvSipReferredByHeaderEncode(hReferredByHeader, g_appPool, &hPage, &length);
    if (rv != RV_OK)
    {
        DBG_PRINT_ERR("RvSipReferredByHeaderEncode failed (err %d)\n", rv);
        return;
    }
    *hdrBuf = (RvChar *)AC_ZALLOC_L(length+1);

    rv = RPOOL_CopyToExternal(g_appPool, hPage, 0, *hdrBuf, length);
    /*EA - 081106 - VI67573 - There is a crash in call transfer (+ VI67811 + VI67407 duplicated)*/
    (*hdrBuf)[length] = '\0';
    if(rv != RV_OK)
    {
        DBG_PRINT_ERR("RPOOL_CopyToExternal failed (err %d)\n", rv);
        /*free the page the encode function allocated*/
        RPOOL_FreePage(g_appPool, hPage);
        nfree(hdrBuf);
    }
    /*perhaps free the page !!!*/
    RPOOL_FreePage(g_appPool, hPage);
}

static RvStatus RVCALLCONV acl_app_call_leg_MsgToSendEvHandler(
                                    IN  RvSipCallLegHandle            hCallLeg,
                                    IN  RvSipAppCallLegHandle         hAppCallLeg,
                                    IN  RvSipMsgHandle                hMsg)
{
    RvSipMethodType                 msgType;
    RvSipReferredByHeaderHandle     hSrcReferredByHeader = NULL;
    RvSipReferToHeaderHandle        hSrcReferToHeader = NULL;
    RvSipHeaderListElemHandle       hElement = NULL;
    RvMdmTerm                       *mdmTerm = NULL;
    char                            *referToHeaderStr = NULL;
    char                            *referredByHeaderStr = NULL;
    int                             remoteConference = 0;

    /*EA - 081106 - VI67573 - There is a crash in call transfer (+ VI67811 + VI67407 duplicated) - added additional protection
    this code should be executed only when remote conference is configured*/
    voip_conf_remote_conference_get(&remoteConference);

    if (1 == remoteConference)
    {
        msgType = RvSipMsgGetRequestMethod(hMsg);
        DBG_PRINT("msgType = %d \n", msgType);
        if (RVSIP_METHOD_REFER == msgType)
        {
            MsgMessage* msg;

            /*keep the refer message headers*/
            hSrcReferredByHeader    = (RvSipReferredByHeaderHandle)RvSipMsgGetHeaderByType(hMsg, RVSIP_HEADERTYPE_REFERRED_BY,
                RVSIP_FIRST_HEADER, &hElement);

            hSrcReferToHeader       = (RvSipReferToHeaderHandle)RvSipMsgGetHeaderByType(hMsg,RVSIP_HEADERTYPE_REFER_TO,
                RVSIP_FIRST_HEADER,&hElement);
            msg = (MsgMessage*)hMsg;

            ReferToHeaderStrAllocNCopy(hSrcReferToHeader, &referToHeaderStr);
            ReferredByHeaderStrAllocNCopy(hSrcReferredByHeader, &referredByHeaderStr);
            DBG_PRINT("referToHeaderStr = %s\n", referToHeaderStr);
            DBG_PRINT("referredByHeaderStr = %s\n", referredByHeaderStr);

            /*update refer info structure*/
            mdmTerm = AclAppGetTermByAppCallLeg(hAppCallLeg);
            call_sip_mngr_refer_info_headers_str_set(mdmTerm, referToHeaderStr, referredByHeaderStr);
        }
    }

    /* All other Method or INFO that wasn't handled by ACL will be handled by RV*/
    if (g_cfg.old_pfnCallLegMsgToSendEvHandler)
    {
        DBG_PRINT("old_pfnCallLegMsgToSendEvHandler\n");
        g_cfg.old_pfnCallLegMsgToSendEvHandler(hCallLeg, hAppCallLeg, hMsg);
    }

    DBG_PRINT("<<<< \n\n");
    return 0;
}

static RvStatus RVCALLCONV acl_app_call_leg_MsgReceivedEvHandler(
                                    IN  RvSipCallLegHandle            hCallLeg,
                                    IN  RvSipAppCallLegHandle         hAppCallLeg,
                                    IN  RvSipMsgHandle                hMsg)
{
    int         msgCode;
    RvMdmTerm   *mdmTerm = NULL;
    int         remoteConference = 0;

    /*EA - 081106 - VI67573 - There is a crash in call transfer (+ VI67811 + VI67407 duplicated) - added additional protection
    this code should be executed only when remote conference is configured*/
    voip_conf_remote_conference_get(&remoteConference);

    if (1 == remoteConference)
    {
        msgCode = RvSipMsgGetStatusCode(hMsg);
        DBG_PRINT("msgCode = %d \n", msgCode);
        if (491 == msgCode)
        {
            /*indicate that we have a failure*/
            mdmTerm = AclAppGetTermByAppCallLeg(hAppCallLeg);
            call_sip_mngr_refer_info_failure_flag_set(mdmTerm, 1);
        }
    }
    /* All other response codes that werent handled by ACL will be handled by RV*/
    if (g_cfg.old_pfnCallLegMsgReceivedEvHandler)
    {
        DBG_PRINT_ERR("old_pfnCallLegMsgReceivedEvHandler\n");
        g_cfg.old_pfnCallLegMsgReceivedEvHandler(hCallLeg, hAppCallLeg, hMsg);
    }

    DBG_PRINT("<<<< \n");
    return 0;
}

static void RVCALLCONV  acl_app_call_leg_ModifyStateChangedEv(
                                    IN  RvSipCallLegHandle            hCallLeg,
                                    IN  RvSipAppCallLegHandle         hAppCallLeg,
                                    IN  RvSipCallLegModifyState       eModifyState,
                                    IN  RvSipCallLegStateChangeReason eReason)
{
    RvMdmTerm                       *mdmTerm = NULL;
    RvStatus                        rv;
    char                            *referToHeaderStr;
    char                            *referredByHeaderStr;
    int                             remote_conf_state = 0;
    int                             unholdInProgress = 0;
    int                             remoteConference = 0;

    /*EA - 081106 - VI67573 - There is a crash in call transfer (+ VI67811 + VI67407 duplicated) - added additional protection
    this code should be executed only when remote conference is configured*/
    voip_conf_remote_conference_get(&remoteConference);

    if (1 == remoteConference)
    {
        DBG_PRINT("eModifyState = %d\n", eModifyState);

        /*check for the re-invite transaction completion == eModifyState is idle*/
        if (RVSIP_CALL_LEG_MODIFY_STATE_IDLE == eModifyState)
        {
            mdmTerm = AclAppGetTermByAppCallLeg(hAppCallLeg);
            if (mdmTerm == NULL)
            {
                DBG_PRINT_ERR("AclAppGetTermByAppCallLeg failed to get mdmTerm");
                return;
            }

            /*check if there was a "refer failure"*/
            remote_conf_state = call_sip_mngr_refer_info_failure_flag_get(mdmTerm);
            if (1 == remote_conf_state)
            {
                /*get the headers you kept*/
                call_sip_mngr_refer_info_headers_str_get(mdmTerm, &referToHeaderStr, &referredByHeaderStr);
                DBG_PRINT("referToHeaderStr = %s\n", referToHeaderStr);
                DBG_PRINT("referredByHeaderStr = %s\n", referredByHeaderStr);

                rv = RvSipCallLegReferStr(hCallLeg, referToHeaderStr, referredByHeaderStr, NULL);
                if (rv != RV_OK)
                {
                    DBG_PRINT_ERR("RvSipCallLegTranscRequest failed with error=%d", rv);
                    return;
                }

                /*clear the refer info structure - you already hadled the failure*/
                nfree(referToHeaderStr);
                nfree(referredByHeaderStr);
                call_sip_mngr_refer_info_headers_str_set(mdmTerm, NULL, NULL);
                call_sip_mngr_refer_info_failure_flag_set(mdmTerm, 0);
            }
            /*  handle the second kbt - we have to wait for the unhold to finish
            this is almost the same problem of having a re-invite message nested inside a re-invite message*/
            unholdInProgress = call_sip_mngr_refer_info_unhold_flag_get(mdmTerm);
            if(1 == unholdInProgress)
            {
                call_sip_mngr_process_remote_conference(mdmTerm);
                call_sip_mngr_refer_info_unhold_flag_set(mdmTerm, 0);
            }
        }
    }

    if (g_cfg.old_pfnCallLegModifyStateChangedEvHandler)
    {
        DBG_PRINT("old_pfnCallLegModifyStateChangedEvHandler\n");
        g_cfg.old_pfnCallLegModifyStateChangedEvHandler(hCallLeg, hAppCallLeg, eModifyState, eReason);
    }

    DBG_PRINT("<<<< \n");
}




/***************************************************************************
 * acl_subs_created_ev_handler
 * ------------------------------------------------------------------------
 * General:  Notifies that a new subscription was created.
 *           This application does not exchange handles with the subscription.
 * Return Value: (-)
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: 	hSubs - The new sip stack subscription handle
 * Output: 	phAppSubs - The application handle for this subscription.
 ***************************************************************************/
static void RVCALLCONV acl_subs_created_ev_handler(
                                               IN  RvSipSubsHandle    hSubs,
                                               IN  RvSipCallLegHandle hCallLeg,
                                               IN  RvSipAppCallLegHandle hAppCallLeg,
                                               OUT RvSipAppSubsHandle *phAppSubs)
{
	DBG_PRINT("Notifier subscription %x was created\n\n",(int)hSubs);
	*phAppSubs = NULL;  /*the application handle is set to NULL*/
    RV_UNUSED_ARG(hCallLeg);
    RV_UNUSED_ARG(hAppCallLeg);
}


/***************************************************************************
 * acl_subs_state_changed_ev_handler
 * ------------------------------------------------------------------------
 * General: Notifies the application of a subscription state change.
 *          For all states indicating a new incoming requests (subs_rcvd,
 *          refresh_rcvd, ubsubscribe_rcvd) - accept the request, and send
 *          a notify request immediately.
 * Return Value: (-)
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: 	hSubs -       The sip stack subscription handle
 *			hAppSubs -    The application handle for this subscription.
 *			eState -      The new subscription state
 *			eReason -     The reason for the state change.
 ***************************************************************************/
static void RVCALLCONV acl_subs_state_changed_ev_handler(
                                    IN  RvSipSubsHandle            hSubs,
                                    IN  RvSipAppSubsHandle         hAppSubs,
                                    IN  RvSipSubsState             eState,
                                    IN  RvSipSubsStateChangeReason eReason)
{
   RvStatus  rv;
   info_per_terminal_t *pSubsPerTerm = (info_per_terminal_t *)hAppSubs;

   DBG_PRINT("-->\r\n");
	/*print the new state on screen*/
	DBG_PRINT("Subscription %x - State changed to %s\r\n", (int)hSubs,acl_subs_state_name_get(eState));

	switch(eState)
	{
      case RVSIP_SUBS_STATE_UNAUTHENTICATED:
      if (pSubsPerTerm->state > 1)
      {
#if 0
         RvSipSubsTerminate(hSubs);
         DBG_PRINT("Terminating SUBSCRIBE: \n");
#endif
      }
      else
      {
         rv = RvSipSubsSubscribe(hSubs);
         if(rv != RV_OK)
         {
            DBG_PRINT_ERR("subscribing failed.");
		      return;
         }
         pSubsPerTerm->state += 1;
         DBG_PRINT("Re-sending SUBSCRIBE: \n");
      }

      break;

   case RVSIP_SUBS_STATE_REFRESHING:
        DBG_PRINT("RVSIP_SUBS_STATE_REFRESHING\n");
    /*-------------------------------------------------------------------
	  set bSendUnsubscribe to true, so next request will be an unsubscribe.
	  -------------------------------------------------------------------*/
	    /* never unsubscribe
			bSendUnsubscribe = RV_TRUE;
			*/
        break;

    default:
        break;
    }
    RV_UNUSED_ARG(eReason);
}

/***************************************************************************
 * acl_subs_notify_ev_handler
 * ------------------------------------------------------------------------
 * General: Application implementation to the SubsNotifyEv event handler.
 *          If the new status is request_rcvd - accept the notify request
 *          with RvSipNotifyAccept().
 * Return Value: (-)
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: 	hSubs         - The sip stack subscription handle
 *			hAppSubs      - The application handle for this subscription.
 *          hNotification - The new created notification object handle.
 *          hAppNotification - The application handle for this notification.
 *          eNotifyStatus - Status of the notification object.
 *          eNotifyReason - Reason to the indicated status.
 *          hNotifyMsg    - The received notify msg.
 ***************************************************************************/
static void RVCALLCONV acl_subs_notify_ev_handler(
                                     IN  RvSipSubsHandle        hSubs,
                                     IN  RvSipAppSubsHandle     hAppSubs,
                                     IN  RvSipNotifyHandle      hNotification,
                                     IN  RvSipAppNotifyHandle   hAppNotification,
                                     IN  RvSipSubsNotifyStatus  eNotifyStatus,
                                     IN  RvSipSubsNotifyReason  eNotifyReason,
                                     IN  RvSipMsgHandle         hNotifyMsg)
{
    RvStatus rv;
   DBG_PRINT("-->\r\n");

    switch (eNotifyStatus)
    {
        case RVSIP_SUBS_NOTIFY_STATUS_REQUEST_RCVD:
        /*----------------------
	      Accept notify request.
	      ----------------------*/
	        DBG_PRINT("Notify received - Accepting the notify request\n\n");
			notification_handle(hNotifyMsg);
            rv = RvSipNotifyAccept(hNotification);
            if(rv != RV_OK)
            {
                DBG_PRINT_ERR("Failed to accept the notify\n");
            }
            break;
        case RVSIP_SUBS_NOTIFY_STATUS_2XX_RCVD:
            DBG_PRINT("2xx on Notify %x received \n\n", (int)hNotification);
            break;
        case RVSIP_SUBS_NOTIFY_STATUS_REJECT_RCVD:
        case RVSIP_SUBS_NOTIFY_STATUS_REDIRECTED:
        case RVSIP_SUBS_NOTIFY_STATUS_UNAUTHENTICATED:
            DBG_PRINT("non - 2xx on Notify %x received \n\n", (int)hNotification);
            break;
        case RVSIP_SUBS_NOTIFY_STATUS_TERMINATED:
            DBG_PRINT("Notify %x was terminated \n\n", (int)hNotification);
            break;
        default:
            break;
    }
    RV_UNUSED_ARG(hSubs);
    RV_UNUSED_ARG(hAppSubs);
    RV_UNUSED_ARG(hAppNotification);
    RV_UNUSED_ARG(eNotifyReason);
    RV_UNUSED_ARG(hNotifyMsg);
}


/***************************************************************************
* acl_subs_expiration_alert_ev_handler
* ------------------------------------------------------------------------
* General: Application implementation to the Expiration Alert event handler.
*          If no refresh was sent, send a refresh using RvSipSubsRefresh(),
*          Otherwise - send an unsubscribe using RvSipSubsRefresh() with
*          expires header value 0.
* Return Value: (-)
* ------------------------------------------------------------------------
* Arguments:
* Input: 	hSubs    - The sip stack subscription handle
*			hAppSubs - The application handle for this subscription.
***************************************************************************/
static void RVCALLCONV acl_subs_expiration_alert_ev_handler(
                                        IN  RvSipSubsHandle            hSubs,
                                        IN  RvSipAppSubsHandle         hAppSubs)
{
    RvStatus             rv;
    RvSipSubscriptionType eType;

    DBG_PRINT("Subscription %x Expiration alert!!! \n",(int)hSubs);
    rv = RvSipSubsGetSubsType(hSubs, &eType);

    if(eType == RVSIP_SUBS_TYPE_SUBSCRIBER)
    {
        if(bSendUnsubscribe == RV_FALSE)
        {
            /*----------------------
	          Send a refresh request.
	          ----------------------*/
            DBG_PRINT("AlertEv - Subscription %x - send REFRESH \n\n",(int)hSubs);
            rv = RvSipSubsRefresh(hSubs, REFRESH_EXPIRES);
            if(rv != RV_OK)
            {
                DBG_PRINT_ERR("Subscription %x - Failed to send refresh \n", (int)hSubs);
            }
        }
        else /* send unsubscribe */
        {
            /*---------------------------
	          Send an unsubscribe request.
	          ---------------------------*/
            DBG_PRINT("Unsubscribing the call\n\n");
            rv = RvSipSubsUnsubscribe(hSubs);
            if(rv != RV_OK)
            {
                DBG_PRINT_ERR("Subscription %x - Failed to send unsubscribe \n");
            }
        }
    }
    RV_UNUSED_ARG(hAppSubs);
}

/***************************************************************************
 * acl_subs_msg_received_ev_handler
 * ------------------------------------------------------------------------
 * General: Application implementation to the message received event handler.
 *          Here we only print the message that was received.
 * Return Value: RV_OK
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: 	hSubs -       The sip stack subscription handle
 *			hAppSubs -    The application handle for this subscription.
 *			hMsg -        Handle to the outgoing message.
 ***************************************************************************/
static RV_Status RVCALLCONV acl_subs_msg_received_ev_handler(
                                        IN  RvSipSubsHandle      hSubs,
                                        IN  RvSipAppSubsHandle   hAppSubs,
                                        IN  RvSipNotifyHandle    hNotify,
                                        IN  RvSipAppNotifyHandle hAppNotify,
                                        IN  RvSipMsgHandle       hMsg)
{
	DBG_PRINT("<-- Message Received (subscription %x)\n",(int)hSubs);
	acl_message_print(hMsg, NULL, 0);
    RV_UNUSED_ARG(hAppSubs);
    RV_UNUSED_ARG(hAppNotify);
    RV_UNUSED_ARG(hNotify);
    return RV_OK;
}

/***************************************************************************
 * acl_subs_msg_to_send_ev_handler
 * ------------------------------------------------------------------------
 * General: Application implementation to the message to send event handler.
 *          Here we only print the message that is about to be sent.
 * Return Value: RV_OK
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: 	hSubs -       The sip stack subscription handle
 *			hAppSubs -    The application handle for this subscription.
 *			hMsg -        Handle to the outgoing message.
 ***************************************************************************/
static RvStatus RVCALLCONV acl_subs_msg_to_send_ev_handler(
                                          IN    RvSipSubsHandle      hSubs,
                                          IN    RvSipAppSubsHandle   hAppSubs,
                                          IN    RvSipNotifyHandle    hNotify,
                                          IN    RvSipAppNotifyHandle hAppNotify,
                                          IN    RvSipMsgHandle       hMsg)
{
	DBG_PRINT("--> Message Sent (subscription %x)\n",(int)hSubs);
	acl_message_print(hMsg, NULL, 0);
    RV_UNUSED_ARG(hAppSubs);
    RV_UNUSED_ARG(hAppNotify);
    RV_UNUSED_ARG(hNotify);
    return RV_OK;
}

/*===============================================================================*/
/*===================    U T I L I T Y	  F U N C T I O N S    ==================*/
/*===============================================================================*/


/***************************************************************************
 * acl_message_print
 * ------------------------------------------------------------------------
 * General: Prints a message on the screen. For doing this we need to
 *          encode the message and then copy the result to a consecutive
 *          buffer.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: 	hMsg -  Handle to the message.
 ***************************************************************************/
void acl_message_print(IN RvSipMsgHandle hMsg, char *out, int maxsize)
{
    RvStatus   rv;
    HPAGE       hPage;
    RvChar     *msgBuf;
    RvUint32   msgSize;

	/* getting the encoded message on an rpool page.*/
	rv = RvSipMsgEncode(hMsg, g_appPool, &hPage, &msgSize);
    if (rv != RV_OK)
    {
		DBG_PRINT_ERR("[acl_message_print:%d]::Message encoding failed (err %d)\r\n", __LINE__,rv );
      return;
    }
	/*allocate a consecutive buffer - use UTILS since malloc doesn't work on all OS's */
    msgBuf = (RvChar *)AC_ZALLOC_L(msgSize+1);

    /* copy the encoded message to an external consecutive buffer*/
    rv = RPOOL_CopyToExternal(g_appPool,
        hPage,
        0,
        (void*)msgBuf,
        msgSize);
    /*terminate the buffer with null*/
    msgBuf[msgSize] = '\0';
    if(rv != RV_OK)
    {
		  DBG_PRINT_ERR("[acl_message_print:%d]::Message encoding failed (err %d)\r\n", __LINE__,rv);
        /*free the page the encode function allocated*/
        RPOOL_FreePage(g_appPool, hPage);
        nfree(msgBuf);
        return;
    }
	if(out)
	{
		strncpy(out,msgBuf,maxsize);
		out[maxsize-1]=(char)0;
	}
	else
	{
		DBG_PRINT("%s",msgBuf);
	}

    /*free the page the encode function allocated*/
    RPOOL_FreePage(g_appPool, hPage);
    nfree(msgBuf);

    return;
}


void	acl_sip_msg_event_header_get(RvSipMsgHandle hMsg ,char  *eventStr, int len)
{

   RvStatus rv;
   HPAGE    hPage;
   RvChar   *msgBuf;
   RvUint32 msgSize;
	RV_CHAR	*szTok;
   int      tmpLen;

	/* getting the encoded message on an rpool page.*/
	rv = RvSipMsgEncode(hMsg, g_appPool, &hPage, &msgSize);
   if (rv != RV_OK)
   {
		DBG_PRINT("Message encoding failed (err %d)\r\n", rv );
      return;
   }
	/*allocate a consecutive buffer - use UTILS since malloc doesn't work on all OS's */
   msgBuf = (RvChar *)AC_ZALLOC_L(msgSize+1);

   /* copy the encoded message to an external consecutive buffer*/
   rv = RPOOL_CopyToExternal(g_appPool, hPage, 0, (void*)msgBuf, msgSize);
   /*terminate the buffer with null*/
   msgBuf[msgSize] = '\0';
   if(rv != RV_OK)
   {
		DBG_PRINT("::Message encoding failed (err %d)\r\n", rv);
      /*free the page the encode function allocated*/
      RPOOL_FreePage(g_appPool, hPage);
      nfree(msgBuf);
      return;
   }

   for( szTok = strtok( msgBuf, "\t\n "); szTok; szTok = strtok( NULL, "\t\n "))
	{
      if(strcmp( szTok, "Event:") ==0)
      {
   	   szTok = strtok( NULL, "\t\n ");
         tmpLen = strlen(szTok);
   		DBG_PRINT("szTok (len=%d)  %s  \r\n",tmpLen, szTok );
         if ( len> tmpLen )
         {
            strcpy(eventStr,szTok);
            eventStr[tmpLen] = 0;
         }
         else
         {
            strncpy(eventStr,szTok,len);
            eventStr[len] = 0;
         }
         break;
      }
   }

   nfree(msgBuf);
   return;
}

/***************************************************************************
 * acl_get_term_by_term_id
 * ------------------------------------------------------------------------
 * General: Returns the Term of a given TermId. Look for the termination in
 *         info_per_terminal_t table.
 * Return Value: The pointer ofthe termination
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: 	termId - The termId
 ***************************************************************************/
static RvMdmTerm*	acl_get_term_by_term_id ( char *termId)
{
   int i, iNumOfLines;

   iNumOfLines=voip_conf_get_number_of_lines(ANALOG_PORT_TYPE_FXO_FXS);

   for (i= 0; i <iNumOfLines; i++ )
   {
     if ( g_infoPerTerm[i].term && !strcmp(termId, rvMdmTermGetId(g_infoPerTerm[i].term)))
	        return(g_infoPerTerm[i].term);
   }

   DBG_PRINT_ERR("ERROR:NO Terminal was found %s\r\n", termId);//sleep(1);
   return(NULL);
}
#ifdef ACL_ADV_SIP_FEATURES_DEBUG
/***************************************************************************
 * acl_subs_state_name_get
 * ------------------------------------------------------------------------
 * General: Returns the name of a given state
 * Return Value: The string with the state name.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: 	eState - The state as enum
 ***************************************************************************/
static const RvChar*  acl_subs_state_name_get (
                          IN  RvSipSubsState  eState)
{

    switch(eState)
    {
    case RVSIP_SUBS_STATE_IDLE:
        return "Idle";
    case RVSIP_SUBS_STATE_SUBS_SENT:
        return "Subs Sent";
    case RVSIP_SUBS_STATE_REDIRECTED:
        return "Redirected";
    case RVSIP_SUBS_STATE_UNAUTHENTICATED:
        return "Unauthenticated";
    case RVSIP_SUBS_STATE_NOTIFY_BEFORE_2XX_RCVD:
        return "Notify Before 2xx Rcvd";
    case RVSIP_SUBS_STATE_2XX_RCVD:
        return "2xx Rcvd";
    case RVSIP_SUBS_STATE_REFRESHING:
        return "Refreshing";
    case RVSIP_SUBS_STATE_REFRESH_RCVD:
        return "Refresh Rcvd";
    case RVSIP_SUBS_STATE_UNSUBSCRIBING:
        return "Unsubscribing";
    case RVSIP_SUBS_STATE_UNSUBSCRIBE_RCVD:
        return "Unsubscribe Rcvd";
    case RVSIP_SUBS_STATE_UNSUBSCRIBE_2XX_RCVD:
        return "Unsubscribe 2xx Rcvd";
    case RVSIP_SUBS_STATE_SUBS_RCVD:
        return "Subs Rcvd";
    case RVSIP_SUBS_STATE_ACTIVATED:
        return "Subs Activated";
    case RVSIP_SUBS_STATE_TERMINATING:
        return "Subs Terminating";
    case RVSIP_SUBS_STATE_PENDING:
        return "Subs Pending";
    case RVSIP_SUBS_STATE_ACTIVE:
        return "Subs Active";
    case RVSIP_SUBS_STATE_TERMINATED:
        return "Subs Terminated";
    default:
        return "Undefined";
    }
}
#endif

static void signal_param_add(RvMdmParameterList* params,const char* id,const char* val)
{
	RvMdmParameterValue value;

	rvMdmParameterValueConstruct(&value, val);
	rvMdmParameterListSet2(params, id, &value);
	rvMdmParameterValueDestruct(&value);

}



static RvStatus to_header_get(RvSipPartyHeaderHandle hTo, char* toUser)
{
	RvSipAddressHandle hAddress = RvSipPartyHeaderGetAddrSpec(hTo);
	RV_UINT len = 0;

	/*Get user name*/
	len = RvSipAddrGetStringLength(hAddress, RVSIP_ADDRESS_USER);
	return RvSipAddrUrlGetUser(hAddress, toUser, len, &len);
}



RV_Status messages_waiting_handle(RvSipMsgHandle hMsg, RV_CHAR *buff)
{
	RvBool               	bNewMsg = rvFalse;
	RvBool                  res;
   RvSipPartyHeaderHandle  hToHeader;
   char                    strUserName[64];
	RV_Status               rc;

   DBG_PRINT(">> \r\n");

   if (!g_mwiParams.isEnabled)
   {
      DBG_PRINT("SipMwi is DISABLED \r\n");
      return 1;//Error
   }
   bNewMsg = (strncmp(buff,"yes",3)==0) ? rvTrue: rvFalse;
	/*
	 *	Sends packages to terminal directly
	 */

	/* find the real terminal that linked to current NOTIFY message*/
   hToHeader = RvSipMsgGetToHeader(hMsg);
   if (hToHeader == NULL)
   {
      DBG_PRINT_ERR("[%s:%d]RvSipMsgGetToHeader failed",__FUNCTION__, __LINE__ );
      return RV_Success;
   }


   rc = to_header_get(hToHeader, strUserName);

   if (rc != RV_OK)
   {
      DBG_PRINT_ERR("[%s:%d]failed to user from ToHeader\r\n",__FUNCTION__,__LINE__);
      return(1); // Error
   }


	RvMdmTerm* term = acl_get_term_by_term_id(strUserName);
   if (term == NULL )
   {
      DBG_PRINT_ERR("[%s:%d]failed to find the Terminal for %s\r\n",__FUNCTION__,__LINE__, strUserName);
      return(1); // Error
   }

	RvMdmXTerm* xTerm = term->xTerm;
	RvIppTerminalHandle t = (RvIppTerminalHandle)xTerm;

   if(bNewMsg)
	{
		/*1st indication*/
      DBG_PRINT("New Msg: yes \r\n");
		//rvIppMdmTerminalSetDisplay( t, "New Msg: yes", 1, 0);

		/*2st indication*/
		{
			RvMdmParameterList params;
			rvMdmParameterListConstruct(&params);
			signal_param_add(&params, "state", "on");
			res = rvIppMdmTerminalStartUserSignalUI( t, "msg", "MWI", &params);
			rvMdmParameterListDestruct(&params);
		}
		/*3st indication*/
		{
         //DBG_PRINT("ind, is \r\n");
			RvMdmParameterList params;
			rvMdmParameterListConstruct(&params);
			signal_param_add(&params, "state", "on");
			signal_param_add(&params, "Indid", "im");
			res = rvIppMdmTerminalStartUserSignalAT( t, "ind", "is", &params);
			rvMdmParameterListDestruct(&params);
		}

		DBG_PRINT("IPP-->Start Signal: MWI on ,term %s,\r\n",strUserName);
	}
   else
	{
		/*1st indication*/
      DBG_PRINT("New Msg: no \r\n");
		//rvIppMdmTerminalSetDisplay( t, "New Msg: no ", 1, 0);
		/*2st indication*/
		{
         DBG_PRINT("msg, qq \r\n");
			RvMdmParameterList params;
			rvMdmParameterListConstruct(&params);
			signal_param_add(&params, "state", "off");
			res = rvIppMdmTerminalStartUserSignalUI( t, "msg", "MWI", &params);
			rvMdmParameterListDestruct(&params);
		}
		/*3st indication*/
		{
			RvMdmParameterList params;
			rvMdmParameterListConstruct(&params);
			signal_param_add(&params, "state", "off");
			signal_param_add(&params, "Indid", "im");
			res = rvIppMdmTerminalStartUserSignalAT( t, "ind", "is", &params);
			rvMdmParameterListDestruct(&params);
		}
		DBG_PRINT("IPP-->Start Signal: MWI off,term %s,\r\n",strUserName);
	}

   DBG_PRINT("<< \r\n");
   return RV_Success;
}



RV_Status check_sync_handle(void)
{
   DBG_PRINT(">> \r\n");
   // update voip/ext_conf that syn is needed
   voip_conf_ext_sync_check_set(1);
   DBG_PRINT("<< \r\n");
   return RV_Success;

}


/***************************************************************************
 * notification_handle
 * ------------------------------------------------------------------------
 * General: Application implementation to the SubsNotifyEv event handler.
 *          If the new status is request_rcvd - accept the notify request
 *          with RvSipNotifyAccept().
 * Return Value: (-)
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: 	   hMsg    - The received notify msg.
 ***************************************************************************/
static RV_Status notification_handle( RvSipMsgHandle hMsg)
{
	RV_Status         rc = RV_Success;
	RvSipBodyHandle   hBody;
	RV_CHAR				*rawBuffer, *szTok;
	RV_UINT32         length;
   char              eventStr[64]; /* TBD */

   DBG_PRINT("-- \n");

   acl_message_print(hMsg, NULL, 0);

   /* Check for event in the header - for example Event:sync-check */
	acl_sip_msg_event_header_get(hMsg, eventStr, sizeof(eventStr));

   DBG_PRINT("-- eventStr (len=%d) = [%s]\n", strlen(eventStr), eventStr);

   if ( !strncmp( eventStr, "check-sync", strlen("check-sync")))
	{
      DBG_PRINT("notification_handle get check-sync\n");
      rc = check_sync_handle();
		return rc;
   }

   hBody = RvSipMsgGetBodyObject( hMsg);

   if(hBody != NULL)
   {

      DBG_PRINT("-- \n");

	   length =  RvSipBodyGetBodyStrLength( hBody);

   /*	rawBuffer = (RV_CHAR *)rvAllocAllocate(userDefaultAlloc, length+1); ACL NB 060927 */
	   rawBuffer = (RV_CHAR *)AC_ZALLOC_L(length+1);


	   rc = RvSipBodyGetBodyStr( hBody, rawBuffer, length, &length);
      if ( rc != RV_OK)
      {
         DBG_PRINT("notification_handle failed to get body str \n");
         nfree(rawBuffer);
         return rc;
      }

      DBG_PRINT("-- \n");


	   for( szTok = strtok( rawBuffer, "\t\n "); szTok; szTok = strtok( NULL, "\t\n "))
	   {
         DBG_PRINT("-- \n");
		   if(strcmp( szTok, "Messages-Waiting:") ==0)
		   {
			   szTok = strtok( NULL, "\t\n ");
			   if(szTok)
			   {
               DBG_PRINT("notification_handle get Messages-Waiting \n");
               rc = messages_waiting_handle(hMsg, szTok);
			   }
			   break;
		   }
	   }
	   /* rvAllocDeallocate(userDefaultAlloc, sizeof(rawBuffer), rawBuffer); ACL NB 060927 */
	   nfree(rawBuffer);
   }

   DBG_PRINT("<< \r\n");
	return rc;
}



/******************************************************************************
*  acl_make_call
*  -------------------------------------
*  General :        This function initiates outgoing call to the destination
*                   address on the specified line and terminal.
*
*  Return Value:    RV_OK if event was sent successfully, other if not.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:           terminalHndl -          terminal Handle
*                   address -               destination address
*  Output:          None
****************************************************************************/
RvStatus RVCALLCONV acl_make_call(RvIppTerminalHandle  terminalHndl,
                                        const RvChar*    address )
{
    RvMdmTerm*  mdmTerm;
    RvChar      limitedAddress[RV_MEDIUM_STR_SZ];
    RvStatus    status = RV_ERROR_INVALID_HANDLE;

    DBG_PRINT("address=%s \r\n", ((address	!= NULL) ? address : "NULL"));

    if ((terminalHndl != NULL) && (address != NULL))
    {
        RvBool isDtmf = RV_TRUE;
        const RvChar *p = address;

        /* See if the address string we have is composed of DTMFs only. If this is the case,
           then we need to resolve the final address, otherwise - we pass it along as if an
           address has been resolved out of DTMFs already. */
        while (isDtmf && *p)
        {
            if (!isdigit((RvInt)*p))
                isDtmf = RV_FALSE; /* Not only DTMFs... */
            p++;
        }

        strncpy(limitedAddress, address, sizeof(limitedAddress)-1);
        limitedAddress[sizeof(limitedAddress)-1] = '\0';

        mdmTerm = rvIppMdmTerminalGetMdmTerm(terminalHndl);
/*-------------------------------------------------------*/
/*Note:the fully resolved address option doesn't work!!! */
/*-------------------------------------------------------*/
        if (isDtmf)
        {
            /* Build a dial string parameter for completed digit collection and use it */
           RvMdmParameterList args;
           rvMdmParameterListConstructA(&args, mdmTerm->termClass->a);
           rvMdmDigitMapBuildDDEvComplete(&args, limitedAddress, RV_MDMDIGITMAP_UNAMBIGUOUSMATCH, mdmTerm->termClass->a);
           rvMdmTermProcessEvent(mdmTerm, "kp", "ce", NULL, &args);
           rvMdmParameterListDestruct(&args);
        }
        else
        {
           /* Act as if address have been fully resolved  ! For now it doesn't work */
           rvMdmTermProcessEvent(mdmTerm, "makecall", limitedAddress, NULL, NULL);
        }

        status = RV_OK;
    }

    DBG_PRINT( "<<\r\n");

    return status;
}


/* ------------------------------------------------------*/
/* ACL NB 070516 - AC#53280 PATCH.Email received from RV */
/* Incident #129532: INFO message  16-May-07             */
/* ------------------------------------------------------*/
RvCCTerminalEvent acl_map_event_cb( const char*        pkg,
                                 const char*        id,
                                 RvMdmParameterList *args,
                                 char*              key)
{

    RV_UNUSED_ARG(args);
    RV_UNUSED_ARG(key);

    DBG_PRINT( ">>\r\n");

    if ((!strcmp(pkg, "user"))  && (!strcmp(id, "flash")))
    {
       DBG_PRINT( "Flash-Hook\r\n");
       return USER_CCTERMEVENT_FLASH;
    }
    /*Support Supplementary services on Analog*/
/*  return rvCCTerminalMdmMapEvent(pkg, id, args, key);*/

    return RV_CCTERMEVENT_NONE;
}

/* ------------------------------------------------------*/
/* ACL NB 070516 - AC#53280 PATCH.Email received from RV */
/* Incident #129532: INFO message  16-May-07             */
/* ------------------------------------------------------*/
/***************************************************************************
 * acl_flash_hook_send
 * ------------------------------------------------------------------------
 * General: Send  Flash-Hook OOB message .
 *
 * Return Value: RV_OK on success, Negative value on failure
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:
 *          hCallLeg    - The  handle for this call-leg.
 *
 ***************************************************************************/
RvStatus acl_flash_hook_send(IN RvSipCallLegHandle hCallLeg, char *strHeaderType, char *strBody)
{
   RvSipTranscHandle    hTransc;
   RvSipMsgHandle       hMsg;
   RvChar               rawBuffer[64];
   RvStatus rv;

   DBG_PRINT( ">>\r\n");

   rv =RvSipCallLegTranscCreate(hCallLeg,NULL, &hTransc);
   if (rv != RV_OK)
   {
      DBG_PRINT("RvSipCallLegTranscCreate failed with error=%d",rv);
      return rv;
   }

   rv = RvSipTransactionGetOutboundMsg(hTransc, &hMsg);
   if (rv != RV_OK)
   {
      DBG_PRINT("RvSipTransactionGetOutboundMsg failed with error=%d",rv);
      return rv;
   }

   //rv = RvSipMsgSetContentTypeHeader(hMsg, "application/flash-hook");
   /* Chnage due to email  received at 20-May-07 from Nir.Gazit@telrad.com*/
   rv = RvSipMsgSetContentTypeHeader(hMsg, strHeaderType/*"text/plain"*/);
   if (rv != RV_OK)
   {
      DBG_PRINT( "RvSipMsgSetContentTypeHeader failed with error=%d",rv);
      return rv;
   }

   //RvSnprintf(rawBuffer,sizeof(rawBuffer), "Signal=hf\n");
   /* Chnage due to email  received at 20-May-07 from Nir.Gazit@telrad.com*/
   RvSnprintf(rawBuffer,sizeof(rawBuffer), "%s\r\n", strBody/*"FLASH\n"*/);

   rv = RvSipMsgSetBody(hMsg, rawBuffer);
   if (rv != RV_OK)
   {
      DBG_PRINT( "RvSipMsgSetBody failed in error=%d",rv);
      return rv;
   }


   rv = RvSipCallLegTranscRequest(hCallLeg,"INFO", &hTransc);
   if (rv != RV_OK)
   {
      DBG_PRINT( "RvSipCallLegTranscRequest failed with error=%d",rv);
      return rv;
   }

   DBG_PRINT( "<<<  statue=%d",rv);
   return rv;
}

/*=========================================================================*/
void acl_sip_reg_auth_obj_Remove( RvSipAuthenticatorHandle hAuthenticator,
                             RvSipRegClientHandle hRegClient)
{
   RvStatus rv;
   RvSipAuthObjHandle hFirstAuthObj = NULL;
   RvSipAuthObjHandle hLastAuthObj= NULL;

   DBG_PRINT(">>\n");

   /* Gets the last auth-obj from the Reg-client. */
   rv = RvSipRegClientAuthObjGet(hRegClient, RVSIP_LAST_ELEMENT, NULL, &hLastAuthObj);
   if(rv != RV_OK || hLastAuthObj == NULL)
   {
   	/*It is not an error - there are register message without auth-obj  */
	DBG_PRINT("Client - no auth-obj in Reg Client\n");
	return;
   }

   /* Gets the first auth-obj from the Reg-client. */
   rv = RvSipRegClientAuthObjGet(hRegClient, RVSIP_FIRST_ELEMENT, NULL, &hFirstAuthObj);
   if(rv != RV_OK || hFirstAuthObj == NULL)
   {
      /*It is not an error - there are register message without auth-obj  */
          DBG_PRINT("Client - failed to get first\n");
	  return;
   }

   DBG_PRINT("first %p Last %p\n", hFirstAuthObj, hLastAuthObj);

   if (hFirstAuthObj != hLastAuthObj)
   {
   	RvSipRegClientAuthObjRemove (hRegClient,hFirstAuthObj);
	DBG_PRINT("Auth Obj Removed  \n");
   }

   DBG_PRINT("<<\n");


   return;
}


void  acl_pfn_state_changed_ev_handler(RvSipRegClientHandle          hRegClient,
                             					RvSipAppRegClientHandle         hAppRegClient,
                             					RvSipRegClientState             eState,
                             					RvSipRegClientStateChangeReason eReason)
{
   RvSipAuthenticatorHandle    hAuthenticator;

   DBG_PRINT(" eState = %d>>\r\n",eState);

    /*EA - 080429 - AC63453 - CVT Code validation*/
    if (call_sip_validation())
    {
        exit(0);
    }

   RvSipStackGetAuthenticatorHandle(g_cfg.stackHandle, &hAuthenticator);

   acl_sip_reg_auth_obj_Remove(hAuthenticator,hRegClient );

   if (g_cfg.old_pfnStateChangedEvHandler)
      	g_cfg.old_pfnStateChangedEvHandler( hRegClient,hAppRegClient, eState, eReason);

   DBG_PRINT("<<\r\n");

   return;

}

/* AlexChen SDK upgrade 20090413 */
RvStatus Acl_pfnMsgToSendEvHandler(RvSipRegClientHandle          hRegClient,
                                   RvSipAppRegClientHandle       hAppRegClient,
                                   RvSipMsgHandle                hMsg)
{
   RvSipOtherHeaderHandle hAllow;
   RvStatus ret;

   DBG_PRINT("-");

   RvSipMethodType msgType = RvSipMsgGetRequestMethod(hMsg);
   if ((msgType == RVSIP_METHOD_REGISTER) )
   {
      /* Add allow header to the message*/
      RvSipOtherHeaderConstructInMsg(hMsg, RV_FALSE, &hAllow);

      RvSipOtherHeaderSetName(hAllow, "Allow");

      RvSipOtherHeaderSetValue(hAllow, "REGISTER, INVITE, ACK, BYE, REFER, NOTIFY, CANCEL, INFO, OPTIONS, PRACK, SUBSCRIBE");
   }

   ret =  g_cfg.old_pfnMsgToSendEvHandler(hRegClient,hAppRegClient,hMsg);

   return ret;
}

/***************************************************************************
 * broadsoft_body_parse
 * ------------------------------------------------------------------------
 * General: Parses INFO message application/broadsoft from buffer.
 *
 * Return Value: RV_OK on success, Negative value on failure
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:
 *			hAppCallLeg - The application handle for this call-leg.
 *			buffer      - Buffer containing message body
 *			bodyLength -   length of message body.
 ***************************************************************************/
static RvStatus broadsoft_body_parse( IN	RvSipAppCallLegHandle   hAppCallLeg,
                                    IN	char*							buffer,
                                    IN	RvUint						bodyLength)
{
	/*EA - 080114 - VI#60461 - Bezeq Interop*/
	RvCCConnection* c = (RvCCConnection*)hAppCallLeg;
	RvCCTerminal*   term   = rvCCConnSipGetTerminal(c);
	RvCCTerminalSip *terminal=NULL;
	char            *termId;
	RvMdmTerm*      mdmTerm = NULL;
	RvChar* toneStr;
	RvStatus rv;

    DBG_PRINT(">>>>");
	rv = RV_OK;
	RV_UNUSED_ARG(bodyLength);

	terminal = rvCCTerminalSipGetImpl(term);
	if (terminal == NULL)
	{
		DBG_PRINT("failed to get terminal\r\n");
                return 1;
	}
	termId = rvCCTerminalSipGetTermId(terminal);
	if (termId == NULL)
	{
		DBG_PRINT("failed to get termId\r\n");
		return 1;
	}
	mdmTerm = acl_get_term_by_term_id(termId);
	if (mdmTerm == NULL)
	{
		DBG_PRINT("failed to get mdmTerm\r\n");
		return 1;
	}

   toneStr=strstr(buffer,"stop CallWaitingTone");
   if (toneStr)
   {
	 /*EA - 080114 - VI#60461 - Bezeq Interop*/
	acl_signal_stop(mdmTerm, VOIP_SIGNAL_CALLER_WAITING_TONE);
	DBG_PRINT("stop CallWaitingTone\r\n");
   }
   else
   {
      toneStr=strstr(buffer,"CallWaitingTone1");
      if (toneStr)
      {
		/*EA - 080114 - VI#60461 - Bezeq Interop*/
		acl_signal_start(mdmTerm, VOIP_SIGNAL_CALLER_WAITING_TONE);
		DBG_PRINT("CallWaitingTone 1\r\n");
      }
      else
      {
         toneStr=strstr(buffer,"CallWaitingTone2");
         if (toneStr)
         {
                /*EA - 080114 - VI#60461 - Bezeq Interop*/
		acl_signal_start(mdmTerm, VOIP_SIGNAL_CALLER_WAITING_TONE_2);
		DBG_PRINT("CallWaitingTone 2 \r\n");
         }
         else
         {
            DBG_PRINT("Not supported %s\r\n", buffer);
            rv= 1;
         }
      }
   }

   DBG_PRINT( "<<<<  statue=%d",rv);

   return rv;
}




/***************************************************************************
 * sscc_body_parse
 * ------------------------------------------------------------------------
 * General: Parses INFO message - application/sscc from buffer.
 *
 * Return Value: RV_OK on success, Negative value on failure
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:
 *			hAppCallLeg - The application handle for this call-leg.
 *			buffer      - Buffer containing message body
 *			bodyLength -   length of message body.
 ***************************************************************************/
static RvStatus sscc_body_parse(IN	RvSipAppCallLegHandle   hAppCallLeg,
                              IN	char*							buffer,
                              IN	RvUint						bodyLength)
{
   RvStatus rv;
   RvChar* strTone;

   RV_UNUSED_ARG(bodyLength);
   rv = RV_OK;

   strTone = strstr(buffer,"tone-type");

   if (strTone)
   {
      strTone=strstr(buffer,"beep");
      if ( strTone )
      {
         DBG_PRINT("beep\r\n");
      }
      else
      {
         DBG_PRINT("Not supported %s\r\n", buffer);
         rv = 1;
      }
   }

#if 0  /*example*/
   rvDtmfParametersInit(&dtmfParam);
   dtmfParam.digit = (char) dtmfNumber[0]; /* for now assuming there is 1 digit?*/
   dtmfParam.duration = duration;
   rvCCConnectionTermDtmfPressedCB((RvCCConnection *)hAppCallLeg, &dtmfParam);
#endif


   return rv;
}
/*********************************************************************************
 *   The following functions create a transaction
 *   that is releated to a term and not to a specific call-leg.
 *   It sends the OPTION request.
 *
 *********************************************************************************/
/***************************************************************************
 * acl_sip_send_option_msg_periodically
 * ------------------------------------------------------------------------
 * General: Send a request related to the term.
 *          1. create a new transaction using RvSipCallLegMgrCreateCallLeg().
 *          2. call the RvSipTransactionMake function with all needed parameters.
 ***************************************************************************/
void acl_option_msg_send(char *termId)
{
    RvSipTranscHandle  hTransc; /*handle to the call-leg*/
    RvStatus           rv;
	 char			        from[128];
	 char			        to[128];
	 char			        requestUri[128];

    g_cfg.CSeq++;
#if 0
    if(strcmp(g_cfg.registrarAddress,""))
       sprintf( from, "sip:%s@%s:%d", termId, g_cfg.registrarAddress, ((g_cfg.registrarPort)? g_cfg.registrarPort :5060) );
    else
       sprintf( from, "From:<sip:%s@%s:%d>", termId, g_cfg.localAddress, g_cfg.stackUdpPort );

    sprintf( to, "To:<sip:%s@%s:%d>", termId ,g_cfg.registrarAddress, ((g_cfg.registrarPort)? g_cfg.registrarPort :5060) );

    sprintf( requestUri, "sip:%s:%d", g_cfg.localAddress, 5060); // TBD if need my port!!!
#endif
    sprintf( from, "From:<sip:%s@%s>", termId, g_cfg.localAddress );

    sprintf( to, "To:<sip:%s@%s>", termId ,g_cfg.registrarAddress);

    sprintf( requestUri, "sip:%s", g_cfg.registrarAddress); // TBD if need my port!!!


    /*--------------------------
      creating a new Transaction
    ----------------------------*/


    rv = RvSipTranscMgrCreateTransaction(g_cfg.hTranscMgr,NULL,&hTransc);
    if(rv != RV_OK )
    {
        DBG_PRINT_ERR("Failed to create new transaction rv=%d\r\n", rv);
        return;
    }

    if ( !hTransc )
    {
        DBG_PRINT_ERR("Failed to create new transaction No hTransc \r\n");
        return;
    }

    DBG_PRINT("transction %p was created\n\n",hTransc);

    /*------------------------------------------------------------
     Send the request by calling the make function
    -------------------------------------------------------------*/
    DBG_PRINT("Sending a %s request: \n\t%s -> %s\n","OPTIONS",from,to);

    rv = RvSipTransactionMake(hTransc,from,to,requestUri, g_cfg.CSeq,"OPTIONS");

    if(rv != RV_OK)
    {
        DBG_PRINT_ERR("Transaction Make failed\r\n");
    }

    return;
}

/*********************************************************************************
 *   The following functions create a transaction
 *   that is releated to a term and not to a specific call-leg.
 *   It sends the OPTION request.
 *
 *********************************************************************************/
/***************************************************************************
 * AclSipSendOptionMsgPeriodically
 * ------------------------------------------------------------------------
 * General: Send a request related to the term.
 *          1. create a new transaction using RvSipCallLegMgrCreateCallLeg().
 *          2. call the RvSipTransactionMake function with all needed parameters.
 ***************************************************************************/
void AclSendOptionMsg(KA_Option_Handle_t *KA_Option_Handle)
{
    RvSipTranscHandle  hTransc; /*handle to the call-leg*/
    RvStatus           rv;
	 char			        from[128];
	 char			        to[128];
	 char			        requestUri[128];
    /*JL VI # 62393 redundant proxy feature */
    KA_Option_Handle->CSeq++;
#if 0
    if(strcmp(g_cfg.registrarAddress,""))
       sprintf( from, "sip:%s@%s:%d", KA_Option_Handle->TermId, g_cfg.registrarAddress, ((g_cfg.registrarPort)? g_cfg.registrarPort :5060) );
    else
       sprintf( from, "From:<sip:%s@%s:%d>", KA_Option_Handle->TermId, g_cfg.localAddress, g_cfg.stackUdpPort );

    sprintf( to, "To:<sip:%s@%s:%d>", KA_Option_Handle->TermId ,KA_Option_Handle->IpAddress, ((g_cfg.registrarPort)? g_cfg.registrarPort :5060) );

    sprintf( requestUri, "sip:%s:%d", KA_Option_Handle->IpAddress, 5060); // TBD if need my port!!!
#endif
    sprintf( from, "From:<sip:%s@%s>", KA_Option_Handle->TermId, g_cfg.localAddress );

    sprintf( to, "To:<sip:%s@%s>", KA_Option_Handle->TermId ,KA_Option_Handle->IpAddress);

    sprintf( requestUri, "sip:%s", KA_Option_Handle->IpAddress); // TBD if need my port!!!


    /*--------------------------
      creating a new Transaction
    ----------------------------*/


    rv = RvSipTranscMgrCreateTransaction(g_cfg.hTranscMgr,NULL,&hTransc);
    if(rv != RV_OK )
    {
        printf("Failed to create new transaction rv=%d\r\n", rv);
        return;
    }

    if ( !hTransc )
    {
        printf("Failed to create new transaction No hTransc \r\n");
        return;
    }/*JL VI # 62393 redundant proxy feature */
	if(KA_Option_Handle->IsRedundantProxy){
		RedundantProxyCfg.KATranscHandle = hTransc;
	    DBG_PRINT("transction %p was created\n\n",hTransc);
	}

    /*------------------------------------------------------------
     Send the request by calling the make function
    -------------------------------------------------------------*/
    DBG_PRINT("Sending a %s request: \n\t%s -> %s\n","OPTIONS",from,to);

    rv = RvSipTransactionMake(hTransc,from,to,requestUri, KA_Option_Handle->CSeq,"OPTIONS");

    if(rv != RV_OK)
    {
        printf("Transaction Make failed\r\n");
    }

    return;
}

void AclSipSendOptionMsgPeriodically(void *data)
{
    /*JL VI # 62393 redundant proxy feature */
	KA_Option_Handle_t *KA_Option_Handle = (KA_Option_Handle_t *)data;

	DBG_PRINT(">>\r\n");

	DBG_PRINT("Start for handle(0x%x) TermId(%s)  proxy ip(%s)\r\n",
		        KA_Option_Handle, KA_Option_Handle->TermId , KA_Option_Handle->IpAddress);

	AclSendOptionMsg(KA_Option_Handle);

	AC_IPC_TIMER_SET( KA_Option_Handle->timeout, AclSipSendOptionMsgPeriodically, KA_Option_Handle);
	DBG_PRINT("<<\r\n");

}

void AclSipStopSendOptionMsgPeriodically(void *data)
{
    /*JL VI # 62393 redundant proxy feature */

	KA_Option_Handle_t *KA_Option_Handle = (KA_Option_Handle_t *)data;

	DBG_PRINT(">>\r\n");

	DBG_PRINT("Stop for handle(0x%x) TermId(%s)  proxy ip(%s)\r\n",
		KA_Option_Handle, KA_Option_Handle->TermId , KA_Option_Handle->IpAddress);

	if (AC_IPC_TIMER_EXISTS(AclSipSendOptionMsgPeriodically, KA_Option_Handle))
   {
      DBG_PRINT("delete existing timer \r\n");
		AC_IPC_TIMER_DEL(AclSipSendOptionMsgPeriodically, KA_Option_Handle);
   }

   DBG_PRINT("<<\r\n");




}


void acl_sip_send_option_msg_periodically(void *termId)
{
   int timeout;

   DBG_PRINT(">>\r\n");

   DBG_PRINT("Start for %s \r\n",(char *)termId );

   acl_option_msg_send((char *)termId);

   voip_conf_ka_options_timeout_get(&timeout);

   AC_IPC_TIMER_SET( timeout, acl_sip_send_option_msg_periodically, termId);
   DBG_PRINT("<<\r\n");

}


void acl_sip_stop_send_option_msg_periodically(void *termId)
{

   DBG_PRINT(">>\r\n");

   DBG_PRINT("Stop for %s \r\n",(char *)termId );

   if (AC_IPC_TIMER_EXISTS(acl_sip_send_option_msg_periodically, termId))
   {
	DBG_PRINT("delete existing timer \r\n");
	AC_IPC_TIMER_DEL(acl_sip_send_option_msg_periodically, termId);
   }

   DBG_PRINT("<<\r\n");

}

/***************************************************************************
 * GetRegistrarIP
 * ------------------------------------------------------------------------
 * General:			Return the registrar whom we are connected to
                    (or trying to connect to)
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:           none
 * output           none
 *
 * return value:	a pointer to the registrar ip
 ***************************************************************************/

char *GetRegistrarIP(void)
{
	DBG_PRINT("->\r\n");
	return g_cfg.registrarAddress;
	DBG_PRINT("<-\r\n");
}

int acl_stun_client_reply_received(RvChar* buf, RvSize_t size,RvUint32 *pMappedIp,	RvUint16 *pMappedPort)
{
	RvStatus status = RV_OK;


   DBG_PRINT("  len=%d\n", size);

#ifdef RV_MTF_STUN
	status = stunClientProcessInput(buf, size, pMappedIp,	pMappedPort);
#endif

	if (status == RV_OK)
      return 0;

   return -1;
}

int acl_handle_491_reply_received(IN RvSipMsgHandle   hMsg, OUT RvMdmTerm **term)
{
	RvSipPartyHeaderHandle		hFromHeader;
	char                  			strUserName[64]="";
	RV_Status         			rc = RV_Success;

	hFromHeader = RvSipMsgGetFromHeader(hMsg);
	if (hFromHeader == NULL)
	{
		printf("[%s:%d]RvSipMsgGetToHeader failed\r\n",__FUNCTION__, __LINE__ );
		return RV_Success;
	}

	rc = to_header_get(hFromHeader, strUserName);
	if (rc != RV_OK)
	{
		printf("[%s:%d]failed to user from ToHeader\r\n",__FUNCTION__,__LINE__);
		return(1); // Error
	}

	*term = acl_get_term_by_term_id(strUserName);
	if (*term == NULL )
	{
		printf("[%s:%d]failed to find the Terminal for %s\r\n",__FUNCTION__,__LINE__, strUserName);
		return(1); // Error
	}

	return RV_Success;

}


/*EA - 080429 - AC63453 - Code validation*/
int call_sip_validation()
{
    char             readVal[64];
    int              retVal = -1;

    memset(readVal, 0, sizeof(readVal));

    retVal = acl_validation_val_get(readVal);

    if (0 != retVal)
        return retVal;

#if defined(P_PLATFORM)
	{
	    	char   *keyVal_1,  *keyVal_2;

		keyVal_1 = "6201";
		keyVal_2 = "6301";

	    	if ( !strcmp(keyVal_1, readVal) )
	    	{
   			return 0;
	    	}
		else if( !strcmp(keyVal_2, readVal) )
		{
   			return 0;
		}
		else
	    	{
			printf("\n**** You are not running on the licensed platform ****\n");
			return 1;
	    	}
	}
#elif defined(B_PLATFORM)

	    	char   *keyVal;

		keyVal = "B2000100";

	    	if (!strcmp(keyVal, readVal))
	    	{
	        	return 0;
	    	}
		else
		{
			printf("\n**** You are not running on the licensed platform ****\n");
			return 1;
		}

#elif defined(B_CEVA_PLATFORM)

	    	char   *keyVal;

		keyVal = "B2348100";

	    	if (!strcmp(keyVal, readVal))
	    	{
	        	return 0;
	    	}
		else
		{
			printf("\n**** You are not running on the licensed platform ****\n");
			return 1;
		}

#elif defined(GROCX_PLATFORM)

	    	char   *keyVal;

		keyVal = "SUCCESS";

	    	if (!strcmp(keyVal, readVal))
	    	{
	        	return 0;
	    	}
		else
		{
			printf("\n**** You are not running on the licensed platform ****\n");
			return 1;
		}

#elif defined(S_PLATFORM)

	    	char   *keyVal;

		keyVal = "SUCCESS";

	    	if (!strcmp(keyVal, readVal))
	    	{
	        	return 0;
	    	}
		else
		{
			printf("\n**** You are not running on the licensed platform ****\n");
			return 1;
		}



#else

    printf("[%s:%d]  Platfrom Unsupporte\r\n", __FUNCTION__, __LINE__);


#endif

    return 0;

}

/* AlexChen SDK upgrade 20090413 */
static RvMdmTerm*	AclAppGetTermByAppCallLeg (RvSipAppCallLegHandle hAppCallLeg)
{
    RvCCConnection  *c          = (RvCCConnection*)hAppCallLeg;
    RvCCTerminal    *term       = NULL;
    RvCCTerminalSip *terminal   = NULL;
    char            *termId     = NULL;
    RvMdmTerm       *mdmTerm    = NULL;

    DBG_PRINT(">>>> \n");


    if (c == NULL)
    {
        DBG_PRINT_ERR("hAppCallLeg is NULL\r\n");
        return NULL;
    }
    term = rvCCConnSipGetTerminal(c);
    if (term == NULL)
    {
        DBG_PRINT_ERR("failed to get term\r\n");
        return NULL;
    }

    terminal = rvCCTerminalSipGetImpl(term);
    if (terminal == NULL)
    {
        DBG_PRINT_ERR("failed to get terminal\r\n");
        return NULL;
    }
    termId = rvCCTerminalSipGetTermId(terminal);
    if (termId == NULL)
    {
        DBG_PRINT_ERR("failed to get termId\r\n");
        return NULL;
    }
    DBG_PRINT("termId = %s \n", termId);
    mdmTerm = acl_get_term_by_term_id(termId);

    return mdmTerm;
}

