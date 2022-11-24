/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	acl_mgcp_call_mngr.c                                   *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 26/02/08 - Coding started.                                       *
 *																	*
 ********************************************************************/

#include "acl_call_mngr.h"
#include "acl_rv_mgcp_api.h"
#include "rvendptmgr.h"
#include "rvmgcpsystem.h"
#include "voip_utils.h"
#include "voip_status_handler.h"
#include "acl_voip_conf.h"
#include "voip_appl_defs.h"

#define IS_DTMF_EVENT(v) (((v)>=VOIP_INPUT_0 && (v)<=VOIP_INPUT_9) || \
    (v) == VOIP_INPUT_ASTERISK || (v) == VOIP_INPUT_POUND)

#define IS_HOOK_EVENT(v) ((v)==VOIP_INPUT_ONHOOK || (v)==VOIP_INPUT_OFFHOOK || \
    (v) == VOIP_INPUT_FLASH)

#define IS_FAX_EVENT(v) ((v)==VOIP_INPUT_FAX_DETECT || (v)==VOIP_INPUT_FAX_BYPASS_DETECT || \
    (v) == VOIP_INPUT_SWITCH2VOICE_DETECT)

#define IS_FAX_END_EVENT(v) ((v)==VOIP_INPUT_SWITCH2VOICE_DETECT)

int SIPLogLevel = 0; // ACL JL - should be renamed after Eitan will renamed it 

/*#define  ACL_MGCP_CALL_MNGR_DEBUG*/

#define DBG_PRINT_ERR(a1,a2...)	printf("[%s:%4d] ERROR: \n" a1,__FUNCTION__, __LINE__, ##a2)

#ifdef ACL_MGCP_CALL_MNGR_DEBUG
#define DBG_PRINT(a1,a2...)	printf("[%s:%4d] \n" a1,__FUNCTION__, __LINE__, ##a2)
#else /* ACL_MGCP_CALL_MNGR_DEBUG */
#define DBG_PRINT(a1,a2...)
#endif /* ACL_MGCP_CALL_MNGR_DEBUG */

static acl_call_mngr_params_t *mgcp_params;
static acl_call_mngr_cbs_t *phone_cbs;

static RvMgcpStack mgcp_stack;
static RvMgcpEndptMgr ep_mgr;
static RvMgcpNotifiedEntity mgc_entity;
static RvMgcpEndpoint *mgcp_eps;
static int is_up;

/* AlexChen SDK upgrade 20090409 */
static RvSdpMsg g_mgcpLastCreateMsg;

typedef struct {
    void *context;
    void *created_call;
} acl_mgcp_ep_t;

static acl_mgcp_ep_t *eps_contexts;

void ep_params_print(void);
extern int acl_validation_init(void);

static AC_CODE2CODE_MAP mgcp_modes[] = {
    { RV_MGCPCONNMODE_INACTIVE, VOIP_CONN_MODE_INACTIVE },
    { RV_MGCPCONNMODE_SENDONLY, VOIP_CONN_MODE_SEND },
    { RV_MGCPCONNMODE_RECVONLY, VOIP_CONN_MODE_RECV },
    { RV_MGCPCONNMODE_SENDRECV, VOIP_CONN_MODE_SEND_RECV },
    { RV_MGCPCONNMODE_LOOPBACK, VOIP_CONN_MODE_LOOPBACK },
    { RV_MGCPCONNMODE_CONFRNCE, VOIP_CONN_MODE_CONFERENCE },
    { RV_MGCPCONNMODE_CONTTEST, VOIP_CONN_MODE_NOT_SET },
    { RV_MGCPCONNMODE_NETWLOOP, VOIP_CONN_MODE_NOT_SET },
    { RV_MGCPCONNMODE_NETWTEST, VOIP_CONN_MODE_NOT_SET },
    //{ RV_MGCPCONNMODE_DATA, VOIP_CONN_MODE_NOT_SET },
    { -1 , -1},
};

static AC_CODE2STR_MAP signals2codes[] = {
    {VOIP_SIGNAL_RING_TONE, "rt", 0, 0}, /* General package */
    {VOIP_SIGNAL_DIAL_TONE, "dl", 0, 0}, /* Line package */
    {VOIP_SIGNAL_DIAL_TONE, "sl", 0, 0}, /* Message waiting indication dialtone. For
				      now play a regular dialtone */
    {VOIP_SIGNAL_BUSY_TONE, "bz", 0, 0}, /* Line package */
    {VOIP_SIGNAL_WARNING_TONE, "ot", 0, 0}, /* Line package (not recorder warn) */
    {VOIP_SIGNAL_RING, "rg", 0, 0}, /* Line package */
    {VOIP_SIGNAL_RING, "r1", 0, 0}, /* Line package */
    {VOIP_SIGNAL_RING_SPLASH, "rs", 0, 0},/* AlexChen SDK upgrade 20090409 */
    {VOIP_SIGNAL_REORDER_TONE, "ro", 0, 0},
    {VOIP_SIGNAL_CALLER_ID, "ci", 0, 0},
    {VOIP_SIGNAL_CALLER_WAITING_TONE, "wt", 0, 0},/* AlexChen SDK upgrade 20090409 */
    {-1, 0, 0, 0}
};

static AC_CODE2STR_MAP codes2signals_dtmf[] = {
    {VOIP_INPUT_0, "0", 0, 0},
    {VOIP_INPUT_1, "1", 0, 0},
    {VOIP_INPUT_2, "2", 0, 0},
    {VOIP_INPUT_3, "3", 0, 0},
    {VOIP_INPUT_4, "4", 0, 0},
    {VOIP_INPUT_5, "5", 0, 0},
    {VOIP_INPUT_6, "6", 0, 0},
    {VOIP_INPUT_7, "7", 0, 0},
    {VOIP_INPUT_8, "8", 0, 0},
    {VOIP_INPUT_9, "9", 0, 0},
    {VOIP_INPUT_ASTERISK, "*", 0, 0},
    {VOIP_INPUT_POUND, "#", 0, 0},
    {-1 , 0, 0, 0}
};

static AC_CODE2STR_MAP codes2signals_line[] = {
    {VOIP_INPUT_OFFHOOK, "hd", 0, 0},
    {VOIP_INPUT_ONHOOK, "hu", 0, 0},
    {VOIP_INPUT_FLASH, "hf", 0, 0},
    {-1 , 0, 0, 0}
};

static AC_CODE2STR_MAP codes2signals_genericMedia[] = {
    {VOIP_INPUT_FAX_DETECT, "ft", 0, 0},
    {VOIP_INPUT_FAX_BYPASS_DETECT, "ft", 0, 0},
    {VOIP_INPUT_SWITCH2VOICE_DETECT, "fe", 0, 0},
    {-1, NULL}
};

#define GET_EP_INFO(ep) eps_contexts[(int)rvMgcpEndpointGetUserData(ep)]
#define GET_CONTEXT(ep) GET_EP_INFO(ep).context

/* AlexChen SDK upgrade 20090409 */
#define MGCP_KEY_ID_MAX_SIZE 7 /* greatest known key id value is 1999 and greatest known package is dd-end */

typedef struct mgcp_key_event_emulation_t {
    struct mgcp_key_event_emulation_t *next;
    RvMgcpEndpoint *ep;
    char package[MGCP_KEY_ID_MAX_SIZE];
    char event_id[MGCP_KEY_ID_MAX_SIZE];
    char connection_id[MGCP_KEY_ID_MAX_SIZE];
    RvMgcpStringList  paramList;
} mgcp_key_event_emulation_t;

/* AlexChen SDK upgrade 20090409 */
static mgcp_key_event_emulation_t *mgcp_key_event_emulation_queue;

/* This function allocates the space for the signal's data. */
static int mgcp_signal_map(const RvMgcpSignal *s, void **data)
{
    int sig_code;
    char *sig_id;
    
    /* Call strdup_e() so that we may convert the signal ID to lower case
     * without touching the RadVision signal ID table */
    sig_id = strdup_e((char *)rvMgcpSignalGetSignalId(s));
    str_tolower(&sig_id);
    sig_code = AC_STR_TO_CODE(signals2codes, sig_id);
    free(sig_id);
	
    switch (sig_code)
    {
		case VOIP_SIGNAL_RING:
	    case VOIP_SIGNAL_RING_SPLASH:
	    	{    
        		voip_siginfo_dist_ring_t *cadData;
			    if (!(cadData = AC_ZALLOC_L(sizeof(voip_siginfo_dist_ring_t))))
		        {
		            DBG_PRINT("ERROR: couldn't allocate enum_ring_cadence_profile_t");
		            return VOIP_SIGNAL_UNKNOWN;
		        }
		        *data = cadData;
		        if(VOIP_SIGNAL_RING_SPLASH == sig_code)
		        {
		            DEBUG_LOG("ring splash signal, setting SLIC_RING_CADENCE_PROFILE_2");
		            cadData->distRing = SLIC_RING_CADENCE_PROFILE_2;
		        }
		        else
		        {
		            DEBUG_LOG("ring signal, setting SLIC_RING_CADENCE_PROFILE_1");
		            cadData->distRing = SLIC_RING_CADENCE_PROFILE_1;
		        }
        		break;
    		}
	    case VOIP_SIGNAL_CALLER_ID:
			{
			    voip_siginfo_cid_t *sigdata;
			    if (!(sigdata = AC_ZALLOC_L(sizeof(voip_siginfo_cid_t))))
				return VOIP_SIGNAL_UNKNOWN;
			    *data = sigdata;

			    if (rvMgcpSignalGetNumParameters(s) > 0)
				sigdata->address = (char *)rvMgcpSignalGetParameter(s, 0);

			    if (rvMgcpSignalGetNumParameters(s) > 1)
				sigdata->number = (char *)rvMgcpSignalGetParameter(s, 1);

			    if (rvMgcpSignalGetNumParameters(s) > 2)
				sigdata->name = (char *)rvMgcpSignalGetParameter(s, 2);

			    break;
			}
		default:
		*data = NULL;
    }

    return sig_code;
}

static void start_signal_cb(RvMgcpEndpoint *ep, const RvMgcpSignal *s)
{
    void *context = ep ? GET_CONTEXT(ep) : NULL;
    void *data = NULL;
    voip_signal_t sig = mgcp_signal_map(s, &data);
    int (*cb)(void *, voip_signal_t, void *) = phone_cbs->start_signal;

    DBG_PRINT("rvcb: start-signal %d", sig);

    if (!context)
    {
	DBG_PRINT_ERR("RvMgcpEndpoint is null in start_signal_cb");
	goto Exit;
    }

    switch(sig)
    {
    case VOIP_SIGNAL_RING:
	phone_cbs->update_display(context, VOIP_DISPLAY_INCOMING_CALL, NULL);
	break;

    case VOIP_SIGNAL_RING_TONE:
	phone_cbs->update_display(context, VOIP_DISPLAY_RINGING_PARTY, NULL);
	break;

    case VOIP_SIGNAL_CALLER_ID:
	cb = phone_cbs->play_signal;
	break;
    default:
	break;
    }
    if (sig>=0)
	cb(context, sig, data);
Exit:
    nfree(data);
}

static void stop_signal_cb(RvMgcpEndpoint *ep, const RvMgcpSignal *signal)
{
    void *context = GET_CONTEXT(ep);
    void *data = NULL;
    voip_signal_t sig = mgcp_signal_map(signal, &data);

    DBG_PRINT("rvcb: stop-signal %d", sig);
    if (sig>=0)
	phone_cbs->stop_signal(context, sig, NULL);
    nfree(data);
}

static RvMgcpReturnCode create_conn_cb(RvMgcpEndpoint *ep, RvMgcpConnection
    *connection, RvMgcpCommandId *cmdId)
{
    void *conn_context;
    RvSdpMsg *local_sdp, *remote_sdp;
    voip_conn_mode_t mode;
    int unilateral = 1;

    local_sdp = rvMgcpConnectionGetLocalDescr(connection);
    remote_sdp = rvMgcpConnectionGetRemoteDescr(connection);
    mode = AC_CODE_TO_VALUE(mgcp_modes, rvMgcpConnectionGetMode(connection));

    DBG_PRINT("->\r\n create_conn (mode=%d)",mode);
    /* XXX check: is local_sdp empty in case we answer (as it is in mdm) ? */

    /* prepare the local_sdp now
     * Note that Hairpin connections are not supported yet */
    if (!(conn_context = phone_cbs->prepare_connection(
	(acl_voip_sdp_msg_t *)local_sdp, 1)))
    {
	DBG_PRINT("prepare_conn failed (loc_sdp*=%x)", (int)local_sdp);
	return RV_MGCPRETURNCODE_OUT_OF_RESOURCES;
    }

    /* create the connection only if we have remote sdp */
    if (remote_sdp && (unilateral = phone_cbs->create_connection(conn_context,
	(acl_voip_sdp_msg_t *)local_sdp, (acl_voip_sdp_msg_t *)remote_sdp, mode, mgcp_params->dtmf_transport_method)))
    {
	DBG_PRINT("create_conn: remote_sdp has no media desc");
	/* note: this is ok in case the connection is just a dial tone */
    }
	/* JL VI # 61985 on NCS dial tone is on RTP so create connection must be done with start voice	
	JL VI # 61985 on NCS dial tone is on RTP so create connection must be done with start voice */
#ifdef RV_MGCP_NCS	
	DEBUG_LOG("Start voice on CRCX \r\n");	
	if(phone_cbs->start_connection(GET_CONTEXT(ep), conn_context))
	{		
		DEBUG_LOG("start voice failed\r\n");	
	}
#endif
	
    rvSdpMsgSetConnection(local_sdp, RV_SDPNETTYPE_IN, RV_SDPADDRTYPE_IP4,
	mgcp_params->local_ip);

	/* AlexChen SDK upgrade 20090409 */
	rvSdpMsgDestruct( &g_mgcpLastCreateMsg );
    rvSdpMsgConstructCopy( &g_mgcpLastCreateMsg, local_sdp);

    /* associate the context with the connection */
    rvMgcpConnectionSetUserData(connection, conn_context);

    if (!unilateral)
    {
	/* Since we need reference to phone_call object when calling
	 * start_connection, which is called from mgcp_process_event - we need
	 * to save this object here. */
	GET_EP_INFO(ep).created_call = conn_context;
    }
	DEBUG_LOG("<-\r\n");
    return RV_MGCPRETURNCODE_OK;
    /* XXX support RV_MGCPRETURNCODE_EXECUTING (with FinalResponse timer) for
     * long signals? */
}

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
			DEBUG_LOG("configure_rate(%d) remote_fax_rate(%d)\n\r", configure_rate, remote_fax_rate);        
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

/* ACL NB 060404 */
/****************************************************************************
*
*	Name:		fax_local_sdp_build
*----------------------------------------------------------------------------
*	Abstract: called in order to build re-invite message.
*  New media is taken from additional[MediaParameters[n]] sections.
*----------------------------------------------------------------------------
*	Returns: 	1 in case of error
******************************************************************************/
/* ACL JL 60222 MaxBitRate negotiation - update DSP according to the SIP SDP attribute field called T38MaxBitRate */
int fax_local_sdp_build(RvSdpMsg* sdpMsg,int faxrate)
{
   acl_voip_sdp_media_descr_t *media;
   int           maxRate;
   char          *maxRateStr;
   AC_CODE2STR_MAP    maxRateCode2str[] = {
        {FAX_MAX_RATE_2_4_KBPS, "2400" },	    
        {FAX_MAX_RATE_4_8_KBPS, "4800" },
        {FAX_MAX_RATE_7_2_KBPS, "7200" },
        {FAX_MAX_RATE_9_6_KBPS, "9600" },
        {FAX_MAX_RATE_12_KBPS,  "12000"},
        {FAX_MAX_RATE_14_4_KBPS,"14400"},
	    {-1}
	};
   RvSdpMediaDescr         *rvMedia;
   acl_voip_sdp_media_descr_t *acMedia;        

   DEBUG_LOG("-> \r\n");
   
   /***********************************/
   /*FAX_MODEM_TRANSFER_METHOD__RELAY */
   /***********************************/
   DEBUG_LOG("RELAY(t.38) --\r\n");
      
 
   rvSdpMsgConstruct(sdpMsg);
 
   /* connection information */
   rvSdpMsgSetConnection(sdpMsg, RV_SDPNETTYPE_IN, RV_SDPADDRTYPE_IP4,"$");
 
   /* media name and transport address */
   rvSdpMsgAddMediaDescr(sdpMsg, RV_SDPMEDIATYPE_IMAGE, ACL_RTPPORT_ANY, RV_SDPPROTOCOL_UDP);
   
   rvMedia = rvSdpMsgGetMediaDescr(sdpMsg, 0);
   rvSdpMediaDescrAddFormat(rvMedia, "t38");
   
   acMedia = (acl_voip_sdp_media_descr_t *)rvMedia;
   //jungo_sdp_media_descr_attr_add(acMedia, "T38FaxVersion", "0");
   //jungo_sdp_media_descr_attr_add(acMedia, "T38FaxMaxBuffer", "1024");
   //jungo_sdp_media_descr_attr_add(acMedia, "T38FaxMaxDatagram", "122");
   //jungo_sdp_media_descr_attr_add(acMedia, "T38FaxRateManagement", "transferredTCF");
   //jungo_sdp_media_descr_attr_add(acMedia, "T38FaxUdpEC", "t38UDPRedundancy");
   //printSdp((char *)sdpMsg,"jungo_buf_to_sdpList: \n ********* t38MediaCap SDP *********");
  
   /* ACL NB 070718 - FaxMaxRate is configurable */
   voip_conf_fax_max_rate_get(&maxRate);
   media = acl_voip_sdp_msg_media_descr_get((acl_voip_sdp_msg_t *)sdpMsg, 0);
   /* ACL JL 60222 MaxBitRate negotiation - update DSP according to the SIP SDP attribute field called T38MaxBitRate */
   if(faxrate <0)
   {
      /* ACL NB 070718 - FaxMaxRate is configurable */
      maxRateStr = code2str(maxRateCode2str, maxRate);
   }
   else
   {
       maxRateStr = code2str(maxRateCode2str, faxrate);
   }
   //jungo_sdp_media_descr_attr_add(media, "T38MaxBitRate", maxRateStr);

   if( rvMedia)
   {
       RvSdpConnection* c = rvSdpMsgGetConnection( &g_mgcpLastCreateMsg );
       RvSdpConnection* connection = rvSdpMsgGetConnection( sdpMsg);
       
       if(connection != NULL)
       {
           rvSdpConnectionSetAddress(connection, c->address);
       }
       
       /********************************/
       /*Set media parameters          */
       /********************************/
       RvSdpMediaDescr*  m = rvSdpMsgGetMediaDescr( &g_mgcpLastCreateMsg, 0);
       RvSdpMediaDescr*  media = rvSdpMsgGetMediaDescr( sdpMsg, 0);
       if(media)
       {
           rvSdpMediaDescrSetPort( media, m->media.port );
           DEBUG_LOG("m->media.port %d\r\n",m->media.port);
       }
       else
           DEBUG_LOG("@@@ No media @@@ \r\n");   
    }

    DEBUG_LOG("<- \r\n");
    
    return 0;
}

static RvMgcpReturnCode connect_mod_cb(RvMgcpEndpoint *ep, RvMgcpConnection
    *connection, RvMgcpCommandId *cmdId)
{
    RvSdpMsg *local_sdp, *remote_sdp;
    voip_conn_mode_t mode;
    void *conn_context = rvMgcpConnectionGetUserData(connection);
	int found = 0;         /* ACL NB 060410 */
/* ACL JL 60222 MaxBitRate negotiation - update DSP according to the SIP SDP attribute field called T38MaxBitRate */
    int fax_rate = -1;

    DEBUG_LOG("->\r\n");
    /* XXX assume that the connection already exist here ? */
    local_sdp = rvMgcpConnectionGetLocalDescr(connection);
    remote_sdp = rvMgcpConnectionGetRemoteDescr(connection);
    mode = AC_CODE_TO_VALUE(mgcp_modes, rvMgcpConnectionGetMode(connection));

    /* XXX modify_connection should be called only in case the connection
     * was modified when the call is in progress, and currently we don't handle
     * this case except change to inactive mode */

    /* stop the current connection if the change mode was to inactive */
    if (mode == VOIP_CONN_MODE_INACTIVE)
    {
	DEBUG_LOG("VOIP_CONN_MODE_INACTIVE\r\n");
	if (!phone_cbs->stop_connection(GET_CONTEXT(ep), conn_context))
	    return RV_MGCPRETURNCODE_OK;
	else
	{
		DEBUG_LOG("RV_MGCPRETURNCODE_MISC_TRANSIENT_ERROR\r\n");
		return RV_MGCPRETURNCODE_MISC_TRANSIENT_ERROR;
	}
    }
  	DEBUG_LOG("creating and starting connection\r\n");
	if (remote_sdp)
    {
         int media_desc_num;
         int idx;
         //running over all SDP messages (usually we have only one)
        //DEBUG_LOG("i=%d msg_list_size  %d \r\n", i, msg_list_size);
        //ACL NB 060410

        media_desc_num = acl_voip_sdp_msg_media_descr_num_get((acl_voip_sdp_msg_t *)remote_sdp);
        //running over all Media description (sometimes we have Audio & Image Media descriptions)
        for (idx = 0; idx < media_desc_num; idx++)
        {
            DEBUG_LOG("idx =%d  media_desc_num %d \r\n", idx, media_desc_num);
            RvSdpMediaDescr * sdpMedia = rvSdpMsgGetMediaDescr(  remote_sdp, idx);
            
            if( rvSdpMediaDescrGetMediaType(sdpMedia) == RV_SDPMEDIATYPE_IMAGE )
            {
                const char  *sdpMediaFormat = rvSdpMediaDescrGetFormat(sdpMedia, 0);
                DEBUG_LOG("RV_SDPMEDIATYPE_IMAGE\r\n");
                if ( !strcmp(sdpMediaFormat, "t38") )
                {
                    DEBUG_LOG("RV_SDPMEDIATYPE_IMAGE\r\n");
                    /* ACL JL 60222 MaxBitRate negotiation - update DSP according to the SIP SDP attribute field called T38MaxBitRate */
                    /* ACL JL 60222 MaxBitRate negotiation - update DSP according to the SIP SDP attribute field called T38MaxBitRate */
                    if ( phone_cbs->start_fax(GET_CONTEXT(ep), (acl_voip_sdp_msg_t *)local_sdp,(acl_voip_sdp_msg_t *)remote_sdp, mode,fax_rate))
                    {
                         DEBUG_LOG("start_fax Failed !!! \r\n");
                         return rvFalse;
                    }

                    supported_fax_rate_get(  (acl_voip_sdp_media_descr_t *)sdpMedia ,&fax_rate );
                    
                    /* ACL JL 60222 MaxBitRate negotiation - update DSP according to the SIP SDP attribute field called T38MaxBitRate */                        
                    fax_local_sdp_build(local_sdp,fax_rate);

                    
                    
                    DEBUG_LOG("- \r\n");

                    found = 1;
                    break;  
                }
            }
            else
            {
               char faxEnabled = 0;
               int faxpayload = -1;

               voip_conf_fax_audio_startup_enable_get( &faxEnabled );
               voip_conf_fax_audio_startup_payload_get( &faxpayload );
                              

               /*use audio to start a t38 fax session*/
              if( faxEnabled 
                  &&  rvSdpMediaDescrGetNumOfFormats(sdpMedia) == 1
                  && atoi(rvSdpMediaDescrGetFormat(sdpMedia, 0)) == faxpayload )
               {
                    if ( phone_cbs->start_fax(GET_CONTEXT(ep), (acl_voip_sdp_msg_t *)NULL,(acl_voip_sdp_msg_t *)remote_sdp, mode,fax_rate))
                    {
                         DEBUG_LOG("start_fax Failed !!! \r\n");
                         return rvFalse;
                    }

                    found = 1;
                    break;                      
               }
            }
                
        }
    }  

	if (!found)
	{
		DEBUG_LOG("create connection and start connection\r\n");
		if (phone_cbs->create_connection(conn_context, (acl_voip_sdp_msg_t *)local_sdp,
			(acl_voip_sdp_msg_t *)remote_sdp, mode, mgcp_params->dtmf_transport_method) ||
			phone_cbs->start_connection(GET_CONTEXT(ep), conn_context))
	    {
			DEBUG_LOG("RV_MGCPRETURNCODE_MISC_TRANSIENT_ERROR\r\n");
			return RV_MGCPRETURNCODE_MISC_TRANSIENT_ERROR;
	    }
	}

    /* In case we initiated a call, the only way to know when the remote party
     * has picked up is by hooking here */
    phone_cbs->update_display(GET_CONTEXT(ep), VOIP_DISPLAY_CALL_IN_PROGRESS,
	NULL);
	DEBUG_LOG("<-\r\n");	
    return RV_MGCPRETURNCODE_OK;
}

static inline int get_endpoint_line_number(RvMgcpEndpoint *ep)
{

    int line = (int)rvMgcpEndpointGetUserData(ep);
    DBG_PRINT("[%s %d] line number is (%d)\n", __FILE__,__LINE__, line);
    return line;
}

static RvMgcpReturnCode connect_update_params_cb(RvMgcpEndpoint *ep,
    RvMgcpConnection *connection, RvMgcpCommandId *cmdId)
{
    RvMgcpConnParameters *params = rvMgcpConnectionGetParameters(connection);
    int line = get_endpoint_line_number(ep);
    voip_state_t state;
    
	DEBUG_LOG("->\r\n");
    
    acl_line_voip_state_get(line, &state);
	rvMgcpConnParametersSetPacketsSent(params, state.packets_sent); 
	rvMgcpConnParametersSetOctetsSent(params, state.bytes_sent);
	rvMgcpConnParametersSetPacketsReceived(params, state.packets_received);
	rvMgcpConnParametersSetOctetsReceived(params, state.bytes_received);
	rvMgcpConnParametersSetPacketsLost(params, state.packets_lost);
	rvMgcpConnParametersSetJitter(params, state.jitter);
	rvMgcpConnParametersSetAvgLatency(params, state.round_trip);

	DEBUG_LOG("<-\r\n");
    return RV_MGCPRETURNCODE_OK;
}

static RvMgcpReturnCode connect_del_cb(RvMgcpEndpoint *ep, RvMgcpConnection
    *connection, RvMgcpCommandId *cmdId)
{
    phone_cbs->stop_connection(GET_CONTEXT(ep),
	rvMgcpConnectionGetUserData(connection));

    phone_cbs->destroy_connection(rvMgcpConnectionGetUserData(connection));
    /* we need to update statistics about the connection before it is closed.
     * since rv won't call it, we do it explicitly */
    connect_update_params_cb(ep, connection, cmdId);
    GET_EP_INFO(ep).created_call = NULL;

    /* XXX May not work in conference call? */
    phone_cbs->update_display(GET_CONTEXT(ep), VOIP_DISPLAY_IDLE,
	mgcp_params->local_ip);

    return RV_MGCPRETURNCODE_CONN_DELETED;
}

static RvBool audit_ep_state_cb(RvMgcpEndpoint *ep, RvMgcpEvent *event)
{
    char *eventId = (char *)rvMgcpEventGetEventId(event);
    voip_input_event_t rg_event = AC_STR_TO_CODE(codes2signals_line, eventId);
    void *context = GET_CONTEXT(ep);
    RvBool rv;

    DBG_PRINT("eid=%s(%d) ctx=%d", eventId, rg_event, (int)context);
    if (!context)
	return rg_event==VOIP_INPUT_ONHOOK;
    rv = phone_cbs->is_state(context, rg_event) ? rvTrue : rvFalse ;
    DBG_PRINT("return %d", rv);
    return rv;
}

static void enum_static_codec_cb(char *codec_name, codec_t *codec, void *caps)
{
    rvMgcpCapabilitiesAddCodec((RvMgcpCapabilities *)caps, codec_name);
}

static void enum_dyn_codec_cb(char *codec_name, codec_t *codec, void *caps)
{
    rvMgcpCapabilitiesAddDynamicCodec((RvMgcpCapabilities *)caps, codec_name,
	codec->payload, codec->rate);
}
/* AlexChen SDK upgrade 20090409 */
static RvBool mgcpConnectionNegotiate(RvMgcpConnection *x, const RvMgcpCapabilityList *capList)
{
	RvSdpMsg *localDescr = rvMgcpConnectionGetLocalDescr(x);
	RvSdpMediaDescr *localMedia;
	RvSdpMediaDescr *remoteMedia = NULL;
	RvPtrList allCodecs, prefs;
	RvPtrListIter iter;
	size_t i;
       char faxEnabled = 0;
       int faxpayload = -1;

	/* Check if there are media descriptors in the remote sdp message */
	if(rvSdpMsgGetNumOfMediaDescr(rvMgcpConnectionGetRemoteDescr(x)))
	{
		remoteMedia = rvSdpMsgGetMediaDescr(rvMgcpConnectionGetRemoteDescr(x), 0);
	}

	if(rvSdpMsgGetNumOfMediaDescr(localDescr))
	{
		localMedia = rvSdpMsgGetMediaDescr(localDescr, 0);
		rvSdpMediaDescrClearFormat(localMedia);
		rvSdpMediaDescrClearRtpMap(localMedia);
	}
	else
	{
		/* Add a media descriptor with port 0, to hold the list of formats */
		localMedia = rvSdpMsgAddMediaDescr(localDescr, RV_SDPMEDIATYPE_AUDIO, 0, RV_SDPPROTOCOL_RTP);
	}

	if(remoteMedia!=NULL && rvSdpMediaDescrGetNumOfFormats(remoteMedia))
	{
		for(i=0; i<rvSdpMediaDescrGetNumOfFormats(remoteMedia); ++i)
		{
			const char *payloadType = rvSdpMediaDescrGetFormat(remoteMedia, i);
			int payloadNumber = atoi(payloadType);
			if(payloadNumber >= FIRST_DYNAMIC_PAYLOAD_TYPE)
			{
			       RvSdpRtpMap *map = NULL ;
                            size_t j;
                            for(j=0; j<rvSdpMediaDescrGetNumOfRtpMap(remoteMedia); ++j)
                            {
                                if(rvSdpRtpMapGetPayload(rvSdpMediaDescrGetRtpMap(remoteMedia, i)) == payloadNumber)
                                {   
                                    map = rvSdpMediaDescrGetRtpMap(remoteMedia, i);
                                    break;
                                }
                            }
                
				if(map != NULL)
				{
					const char *codecName = rvSdpRtpMapGetEncodingName(map);
					int rate = rvSdpRtpMapGetClockRate(map);
					if(acl_voip_mgcpCapabilityListIsCodecSupported((acl_voip_mgcp_capabilityList_t *)capList, codecName, rate))
					{
						rvSdpMediaDescrAddFormat(localMedia, payloadType);
						rvSdpMediaDescrAddRtpMap(localMedia, payloadNumber, codecName, rate);
						return rvTrue;
					}
				}
			}
			else
			{
				const MgcpPayloadType *map = (MgcpPayloadType *)mgcp_getStandardPayloadType(payloadNumber);
				if(map != NULL && acl_voip_mgcpCapabilityListIsCodecSupported((acl_voip_mgcp_capabilityList_t *)capList, map->codec, map->rate))
				{
					rvSdpMediaDescrAddFormat(localMedia, payloadType);
					return rvTrue;
				}
			}
		}

              /*check if there are rtpmap in the sdp messages,if not,we should check if fax_audio_startup flag is enabled.
                  if flag is enabled,fax session will be started.otherwise,response a 523 error.
                  we have two ways to start a fax session:
                  1.use image,m=image xxxx udptl t38
                  2.use audio, m=audio xxxx RTP/AVP 98,Although 98 is a dynamic payload,we use it as a flag.
                     thus,rtpmap attribute is not necessary
              */
              voip_conf_fax_audio_startup_enable_get( &faxEnabled );
              voip_conf_fax_audio_startup_payload_get( &faxpayload );
              if( faxEnabled 
                  &&  rvSdpMediaDescrGetNumOfFormats(remoteMedia) == 1
                  && atoi(rvSdpMediaDescrGetFormat(remoteMedia, 0)) == faxpayload )
              {
		     rvSdpMediaDescrAddFormat(localMedia, rvSdpMediaDescrGetFormat(remoteMedia, 0));
                   return rvTrue;
              }

		return rvFalse;
	}

	rvPtrListConstruct(&allCodecs, &rvDefaultAlloc);
	rvPtrListConstruct(&prefs, &rvDefaultAlloc);

	acl_voip_mgcpCapabilityListGetAllRtpMaps((acl_voip_mgcp_capabilityList_t *)capList, (acl_voip_mgcp_ptrList_t *)&allCodecs);
	
	/* sort codecs by local connection options */
	iter = rvPtrListBegin(&allCodecs);
	while(iter != rvPtrListEnd(&allCodecs))
	{
		MgcpPayloadType *map = (MgcpPayloadType *)rvPtrListIterData(iter); 
		if(acl_voip_mgcpCapabilitiesIsCodecSupported((acl_voip_mgcp_capabilities_t *)(&x->localConnOpts), map->codec, map->rate))
		{
			rvPtrListPushBack(&prefs, map);
			iter = rvPtrListErase(&allCodecs, iter);
		}
		else
			iter = rvPtrListIterNext(iter);		
	}
	rvPtrListSplice(&allCodecs, rvPtrListBegin(&allCodecs), &prefs, rvPtrListBegin(&prefs), rvPtrListEnd(&prefs));


	for(iter = rvPtrListBegin(&allCodecs); iter != rvPtrListEnd(&allCodecs); iter = rvPtrListIterNext(iter))
	{
		MgcpPayloadType *map = (MgcpPayloadType *)rvPtrListIterData(iter); 
		char payloadType[16];
		sprintf(payloadType, "%d", map->payload);
		rvSdpMediaDescrAddFormat(localMedia, payloadType);
		if(map->payload >= FIRST_DYNAMIC_PAYLOAD_TYPE)
		{
			rvSdpMediaDescrAddRtpMap(localMedia, map->payload, map->codec, map->rate);
			rvAllocDeallocate(&rvDefaultAlloc, sizeof(MgcpPayloadType), map);
		}
	}

	rvPtrListDestruct(&allCodecs);
	rvPtrListDestruct(&prefs);

	return rvSdpMediaDescrGetNumOfFormats(localMedia) != 0;
}

static int mgcp_eps_create(void)
{
    RvMgcpEndpointId id;
    RvMgcpCapabilities caps;
    int i;

    /* Following are the packages that will get events */
    rvMgcpCapabilitiesConstruct(&caps, "l"); /* default package is LINE */
    rvMgcpCapabilitiesAddSupportedPkg(&caps, "d"); /* add DTMF package */
    rvMgcpCapabilitiesAddSupportedPkg(&caps, "g"); /* add GENERIC package */
    phone_cbs->enum_codecs(enum_static_codec_cb, enum_dyn_codec_cb, &caps);

    mgcp_eps = AC_ZALLOC_L(mgcp_params->ep_num * sizeof(RvMgcpEndpoint));
    if (!mgcp_eps)
	return -1;
    eps_contexts = AC_ZALLOC_L(mgcp_params->ep_num * sizeof(acl_mgcp_ep_t));
    if (!eps_contexts)
	return -1;

    for (i=0; i < mgcp_params->ep_num; i++)
    {
		RvMgcpEndpoint *ep = &mgcp_eps[i];
		char mgcp_ep_name[MAX_CALLER_ID_SIZE+4];
		char mgcp_domain_name[AC_MAX_DOMAIN_NAME_LEN + 2];

		if (!mgcp_params->lines[i].enabled)
		{
			DBG_PRINT("line(%d) is disabled\r\n", i);
			continue;
		}

		/* NirM, Setting all lines to UNLOCK*/
    		SetLineStatus(i, PORT_UNLOCK);
		
		sprintf(mgcp_ep_name, "%s", mgcp_params->lines[i].id);
		/* create the end point */

		/* AlexChen SDK upgrade 20090409 */
		if (mgcp_params->sp.mgcp.user_domain[0])
			sprintf(mgcp_domain_name, "[%s]", mgcp_params->sp.mgcp.user_domain);
		else
			sprintf(mgcp_domain_name, "[%s]", mgcp_params->local_ip);
		rvMgcpEndpointIdConstruct(&id, mgcp_ep_name, mgcp_domain_name);
		rvMgcpEndpointConstruct(ep, &id, &mgc_entity, &caps);
		/* and set its cbs */
		rvMgcpEndpointRegisterStartSignalCB(ep, start_signal_cb);
		rvMgcpEndpointRegisterStopSignalCB(ep, stop_signal_cb);
		rvMgcpEndpointRegisterCreateConnCB(ep, create_conn_cb);
		rvMgcpEndpointRegisterModifyConnCB(ep, connect_mod_cb);
		rvMgcpEndpointRegisterDeleteConnCB(ep, connect_del_cb);
		rvMgcpEndpointRegisterUpdateConnParamsCB(ep, connect_update_params_cb);
		rvMgcpEndpointRegisterEventStateCB(ep, audit_ep_state_cb);
		rvMgcpEndpointRegisterNegotiateCB(ep, mgcpConnectionNegotiate);
		/* associate index and register the ep */
		rvMgcpEndpointSetUserData(ep, (void*)i);
		DBG_PRINT("Register ep#%d in the ep-mgr",i);
		rvMgcpEndptMgrRegisterEndpoint(&ep_mgr, ep);
		/* destroy the id object in order to reuse its memory */
		rvMgcpEndpointIdDestruct(&id);
		eps_contexts[i].context = phone_cbs->set_endpoint_context(i,
			mgcp_params->lines[i].id, mgcp_params->keys_sequence);

		phone_cbs->update_display(GET_CONTEXT(ep), VOIP_DISPLAY_IDLE,
			mgcp_params->local_ip);
    }
    rvMgcpCapabilitiesDestruct(&caps);

    return 0;
}

void ep_params_print(void)
{
#ifdef PRINT_DEBUG_LOG
	int i;
	for (i=0; i < mgcp_params->ep_num; i++)
    {
		RvMgcpEndpoint *ep = &mgcp_eps[i];

		DBG_PRINT("line num(%d)\r\n", i);
		DBG_PRINT("----------------\r\n\r\n");
		DBG_PRINT("auditStateCB(%d)\r\n", ep->auditState );
		DBG_PRINT("firstNotifySent (%d)\r\n", ep->firstNotifySent);
		DBG_PRINT("disconnectedAt(%d)\r\n", ep->disconnectedAt);
		DBG_PRINT("disconnectInProgress(%d)\r\n", ep->disconnectInProgress);
		DBG_PRINT("disconnectTimeout(%d)\r\n", ep->disconnectTimeout);
		DBG_PRINT("rsipTimer(%d)\r\n", ep->rsipTimer);
		DBG_PRINT("lastDisconnectRsipSentAt(%d)\r\n", ep->lastDisconnectRsipSentAt);
		DBG_PRINT("restartStatus(%d)\r\n", ep->restartStatus);
	}
#endif
}

static int acl_mgcp_call_mngr_init(acl_call_mngr_params_t *params, acl_call_mngr_cbs_t *cbs)
{
    RvPriority priority;

	DEBUG_LOG(">>\r\n");	
    /* Register MGCP protocl */
    AC_RG_COMPONENT_REGISTER(RG_COMP_RV_MGCP);

    /* we need to the params all over this module */
    mgcp_params = params;

    if(acl_validation_init() != 0)
    {
	   printf("error at acl_validation_init\r\n");
	   exit(0);
    }

    rvMgcpSystemInit();


    /* At this point the rvLog object was created and we can set the log */
	acl_mgcp_log_construct(0); 	/* JL @@@*/ 

	/* create the mgcp stack */  
	MZERO(mgcp_stack); 
	rvPriorityConstruct2(&priority, RV_PRIORITY_NORMAL);    
    rvMgcpStackConstruct(&mgcp_stack, RV_MGCPSTACKTYPE_MG, "AudioCodes Tulip",
	mgcp_params->local_ip, mgcp_params->sp.mgcp.mg_port, 1 /*one reciever
	 thread*/, priority);
	rvPriorityDestruct(&priority);
	MZERO(ep_mgr); 
	/* create the ep-mgr associated with the stack */ 
	rvMgcpEndptMgrConstruct(&ep_mgr, &mgcp_stack);  
	/* and the entity presenting the MGC */ 
	DEBUG_LOG("Construct notified entity (ip=%s port=%d)",	
                  mgcp_params->sp.mgcp.mgc_address, mgcp_params->sp.mgcp.mgc_port);
	rvMgcpNotifiedEntityConstruct(&mgc_entity, "ca",
	mgcp_params->sp.mgcp.mgc_address, mgcp_params->sp.mgcp.mgc_port);  
	/* startup the stack (and its underlying rcv thread) */


    /* store the phone_conn v-table */ 
    phone_cbs = cbs;
    rvMgcpSystemRegisterStack(&mgcp_stack);	
    /* create and associate endpoints to the stack */
    if (mgcp_eps_create() < 0)
	{
		DBG_PRINT("mgcp_eps_create return ERROR\r\n");
		return -1;
	}
	ep_params_print();
    is_up = 1;
	DBG_PRINT("device is up\r\n<<\r\n");
    return 0;
}

#if 0
/* NOTE: this #if 0 (and the one in acl_mgcp_call_mngr_uninit()) was added since calling 
 * acl_mgcp_call_mngr_uninit() causes openrg to hang. B23736 should solve this issue */
static RvBool ep_delete_cb(RvMgcpEndpoint *ep, void *unused)
{
    phone_cbs->free_endpoint_context(GET_CONTEXT(ep));
    rvMgcpEndpointDestruct(ep);
    return rvTrue;
}
#endif

static int acl_mgcp_call_mngr_uninit(void)
{
	
	DEBUG_LOG("->\r\n");
    AC_RG_COMPONENT_UNREGISTER(RG_COMP_RV_MGCP);
#if 0
    if (!is_up)
	return 0;

    rvMgcpSystemUnregisterStack(&mgcp_stack);
    rvMgcpEndptMgrProcessAllEndpoints(&ep_mgr, ep_delete_cb, NULL);
    rvMgcpNotifiedEntityDestruct(&mgc_entity);
    rvMgcpEndptMgrDestruct(&ep_mgr);
    rvMgcpStackDestruct(&mgcp_stack);
    rvMgcpSystemEnd();
    free(mgcp_eps);
    free(eps_contexts);
#endif
    is_up = 0;
   DEBUG_LOG("<-\r\n");
    return 0;
}

/* AlexChen SDK upgrade 20090409 */
static void mgcp_process_key_cb(void *o)
{
    mgcp_key_event_emulation_t *tmp;
    RvMgcpEvent event_obj;

    tmp = mgcp_key_event_emulation_queue;
    mgcp_key_event_emulation_queue = tmp->next;
    
    /* create the event object and process it*/
    rvMgcpEventConstruct(&event_obj, tmp->package, tmp->event_id,  tmp->connection_id);

    rvMgcpEndpointProcessEvent(tmp->ep, &event_obj);

    rvMgcpEventDestruct(&event_obj);
    
    
    free(tmp);

    if (mgcp_key_event_emulation_queue)
         AC_IPC_TIMER_SET(0, mgcp_process_key_cb, NULL);
}


static void mgcp_process_key(char *package, char *event_id, char *connection_id, RvMgcpEndpoint *ep)
{
   mgcp_key_event_emulation_t **next = &mgcp_key_event_emulation_queue;
   

   while (*next)
	   next = &(*next)->next;
    
   *next = AC_ZALLOC_L(sizeof(mgcp_key_event_emulation_t));
   if (!*next)
	   return;

   (*next)->ep= ep;
   strcpy((*next)->package, package);
   strcpy((*next)->event_id, event_id);
   strcpy((*next)->connection_id, connection_id);
   
   if (next == &mgcp_key_event_emulation_queue)
      AC_IPC_TIMER_SET(0, mgcp_process_key_cb, NULL);

}


static int mgcp_process_event(int epn, voip_input_event_t event, int down)
{
    char *pkg, *event_id;
    //RvMgcpEvent event_obj;
    RvMgcpEndpoint *ep = &mgcp_eps[epn];

    DBG_PRINT("cb: process event %d (epn=%d ep_id=(%s,%s)) down=%d", event, epn,
	ep->endpointId.localName,  ep->endpointId.domainName, down);
    /* translate the internal event code to MGCP event id */
    if (epn >= mgcp_params->ep_num)
	{
		DEBUG_LOG("ERROR: epn(%d)>=ep_num(%d)\r\n", epn, mgcp_params->ep_num);
		return -1;
	}
    /*NirM - For compilation warnings*/
    if (IS_DTMF_EVENT((int)event))
    {    
		if (!down)
			return 0; /* ignore key up events, action only on key down */
		pkg = "D";
		event_id = code2str(codes2signals_dtmf, event);
    }
    else if (IS_HOOK_EVENT(event))
    {
		pkg = "L";
		event_id = code2str(codes2signals_line, event);
    }
	else if (IS_FAX_EVENT(event)) /* AlexChen SDK upgrade 20090409 */
    {
       char faxEndReportEnabled = 0;
       
       voip_conf_fax_end_report_enable_get( &faxEndReportEnabled );

       /*ignore fax end event,fax end report is disabled*/
       if( faxEndReportEnabled == 0 && IS_FAX_END_EVENT(event) )
            return 0;
       
 		pkg = "G";    
        event_id = code2str(codes2signals_genericMedia, event);
    }
    else if(12 <= event && 15 >= event)
	{
		char signal_DTMF[] = "ABCD";
		DEBUG_LOG("signal DTMF %c was recieved\r\n", signal_DTMF[event-12]);
		return 0;
	}
    else
	{
		DBG_PRINT("ERROR\r\n");
		return -1;
	}

#if 0 // AlexChen SDK upgrade 20090410 */
    /* create the event object and process it*/
    rvMgcpEventConstruct(&event_obj, pkg, event_id, "");
    /* XXX should we cosider key-down in the event? */
    DBG_PRINT("call rv-process-event (id=%s)", event_id);
    rvMgcpEndpointProcessEvent(ep, &event_obj);
    rvMgcpEventDestruct(&event_obj);
#endif
	mgcp_process_key(pkg,event_id,"",ep);

/* JL VI # 61985 on NCS dial tone is on RTP so we already started voice when connection created	
	this flag is enabled/disabled in vendor/radvision_mgcp/radvision_mgcp_env.mak */
#ifndef RV_MGCP_NCS
    if (event==VOIP_INPUT_OFFHOOK && GET_EP_INFO(ep).created_call)
    {
	/* RV MGCP doesn't provide any special CB for starting the actual
	 * call (like MDM does), so we need to do it when we get off-hook
	 * and we have the call object (which is created during crcx) */
	DBG_PRINT("starting voice now");

	phone_cbs->update_display(GET_CONTEXT(ep),
	    VOIP_DISPLAY_CALL_IN_PROGRESS, NULL);

	phone_cbs->start_connection(GET_CONTEXT(ep),
	    GET_EP_INFO(ep).created_call);
    }
#endif
    /* If dialed number has changed, update display */
     /*NirM - For compilation warnings*/
   if (IS_DTMF_EVENT((int)event) && down)
    {
	phone_cbs->update_display(GET_CONTEXT(ep), VOIP_DISPLAY_DIALING,
	    ep->dialString.currentDialString);
    }

    DBG_PRINT("exit cb (id=%s)\r\n", event_id);
    return 0;
}

voip_protocols_t get_signalling_prot(void)
{
    return VOIP_PROT_MGCP;
}

/* Alex SDK upgrade 20090410 */
static int get_registration_status_cb(int epn)
{
    RvMgcpEndpoint *ep = &mgcp_eps[epn];

    return ep->restartStatus;
}

/* Alex SDK upgrade 20090410 */
static void *get_conn_by_line_cb(int line)
{
    return eps_contexts[line].context;
}

/* Alex SDK upgrade 20090410 */
static void get_call_state_cb(int epn, call_state_t *state, call_origin_t *orig)
{
    RvPtrListIter iter;
    RvMgcpEndpoint *ep = &mgcp_eps[epn];

    for(iter = rvPtrListBegin(&ep->connections);
       iter != rvPtrListEnd(&ep->connections);
       iter = rvPtrListIterNext(iter))
    {
       RvMgcpConnection *connection = rvPtrListIterData(iter);
       voip_conn_mode_t mode;
   
       mode = AC_CODE_TO_VALUE(mgcp_modes, rvMgcpConnectionGetMode(connection));

       if( mode == VOIP_CONN_MODE_SEND_RECV )
       {
       
           *state = CALL_STATE_CONNECTED;
       }
    }    
}

static unsigned char is_fxo_line_cb(int lineIdx)
{	
	return 0;/*JL mgcp is currently not supporting FXO line*/
}


acl_call_mngr_ops_t mgcp_call_mngr_ops = {
    init: acl_mgcp_call_mngr_init,
    uninit: acl_mgcp_call_mngr_uninit,
    get_active_protocol: get_signalling_prot,
    process_event: mgcp_process_event,
    get_registration_status: get_registration_status_cb,//060530 Avner phase 1
    get_conn_by_line: get_conn_by_line_cb,//060530 Avner phase 1
    get_call_state: get_call_state_cb,//060530 Avner phase 1
    is_fxo_line:is_fxo_line_cb/* JL phone_conn need to know if line is fxo*/
};

