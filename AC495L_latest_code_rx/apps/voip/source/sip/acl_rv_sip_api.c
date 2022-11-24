/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	acl_rv_sip_api.c                                    *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 26/02/08 - Coding started.                                       *
 *																	*
 ********************************************************************/
#include "acl_rv_sip_api.h"
#include "rvphone_common.h"
#include <rvlog.h>
#include <rvmutex.h>
#include <rvsdp.h>
#include <rvsdpenums.h>
#include <rvSipControlApi.h>
#include <rvloglistener.h>
#include <rvcctext.h>
#include <RvSipCSeqHeader.h>

#include "voip_utils.h"

/*  EA - 071225 Enabling Radvision's logs through acl_conf 
    changed the log filters to the correct types 
    from RVSIP_LOG_XXX_FILTER to RV_LOGLEVEL_XXX
*/ 
IppLogSourceFilterElm	g_ippLogOptions[] = {
   { "IPP_USERAPP",RV_LOGLEVEL_ERROR | RV_LOGLEVEL_EXCEP | RV_LOGLEVEL_WARNING },
   { "SOCKET",     RV_LOGLEVEL_ERROR | RV_LOGLEVEL_EXCEP | RV_LOGLEVEL_WARNING }, 
   { "THREAD",     RV_LOGLEVEL_ERROR | RV_LOGLEVEL_EXCEP | RV_LOGLEVEL_WARNING },
   { "MUTEX",      RV_LOGLEVEL_ERROR | RV_LOGLEVEL_EXCEP | RV_LOGLEVEL_WARNING },
   { "SEMA",       RV_LOGLEVEL_ERROR | RV_LOGLEVEL_EXCEP | RV_LOGLEVEL_WARNING },
   { "LOCK",       RV_LOGLEVEL_ERROR | RV_LOGLEVEL_EXCEP | RV_LOGLEVEL_WARNING },
   { "MEMORY",     RV_LOGLEVEL_ERROR | RV_LOGLEVEL_EXCEP | RV_LOGLEVEL_WARNING },
   { "TIMER",      RV_LOGLEVEL_ERROR | RV_LOGLEVEL_EXCEP | RV_LOGLEVEL_WARNING },
   { "CLOCK",      RV_LOGLEVEL_ERROR | RV_LOGLEVEL_EXCEP | RV_LOGLEVEL_WARNING },
   { "TIMESTAMP",  RV_LOGLEVEL_ERROR | RV_LOGLEVEL_EXCEP | RV_LOGLEVEL_WARNING },
   { "RPOOL",      RV_LOGLEVEL_ERROR | RV_LOGLEVEL_EXCEP | RV_LOGLEVEL_WARNING },
   { "RA",         RV_LOGLEVEL_ERROR | RV_LOGLEVEL_EXCEP | RV_LOGLEVEL_WARNING },
   { "IPP_UTIL",   RV_LOGLEVEL_ERROR | RV_LOGLEVEL_EXCEP | RV_LOGLEVEL_WARNING },
   { "IPP_CALLCON",RV_LOGLEVEL_ERROR | RV_LOGLEVEL_EXCEP | RV_LOGLEVEL_WARNING },
   { "IPP_MDM",    RV_LOGLEVEL_ERROR | RV_LOGLEVEL_EXCEP | RV_LOGLEVEL_WARNING },
   { "IPP_MDMCONT",RV_LOGLEVEL_ERROR | RV_LOGLEVEL_EXCEP | RV_LOGLEVEL_WARNING },
   { "IPP_SIPCONT",RV_LOGLEVEL_ERROR | RV_LOGLEVEL_EXCEP | RV_LOGLEVEL_WARNING },
   { "IPP_CFW",    RV_LOGLEVEL_ERROR | RV_LOGLEVEL_EXCEP | RV_LOGLEVEL_WARNING },
   { "",           0 } /* empty entry - mark the end of the table */
};
/* ACL NB 060814 added SIP log */
//RvIppSipModuleFilter	g_ippSipLogOptionsFilters[] = 
static RvIppSipLogOptions gIppSipLogOptions =
{  
    39,
    {
 { RVSIP_CALL,				RVSIP_LOG_ERROR_FILTER|RVSIP_LOG_EXCEP_FILTER|RVSIP_LOG_WARN_FILTER },
        { RVSIP_TRANSACTION,		RVSIP_LOG_ERROR_FILTER|RVSIP_LOG_EXCEP_FILTER|RVSIP_LOG_WARN_FILTER }, 
        { RVSIP_MESSAGE,			RVSIP_LOG_ERROR_FILTER|RVSIP_LOG_EXCEP_FILTER|RVSIP_LOG_WARN_FILTER },
        { RVSIP_TRANSPORT,			RVSIP_LOG_ERROR_FILTER|RVSIP_LOG_EXCEP_FILTER|RVSIP_LOG_WARN_FILTER },
        { RVSIP_PARSER,				RVSIP_LOG_ERROR_FILTER|RVSIP_LOG_EXCEP_FILTER|RVSIP_LOG_WARN_FILTER },
        { RVSIP_STACK,				RVSIP_LOG_ERROR_FILTER|RVSIP_LOG_EXCEP_FILTER|RVSIP_LOG_WARN_FILTER },
        { RVSIP_MSGBUILDER,		    RVSIP_LOG_ERROR_FILTER|RVSIP_LOG_EXCEP_FILTER|RVSIP_LOG_WARN_FILTER },
        { RVSIP_AUTHENTICATOR,	    RVSIP_LOG_ERROR_FILTER|RVSIP_LOG_EXCEP_FILTER|RVSIP_LOG_WARN_FILTER },
        { RVSIP_REGCLIENT,			RVSIP_LOG_ERROR_FILTER|RVSIP_LOG_EXCEP_FILTER|RVSIP_LOG_WARN_FILTER },
        { RVSIP_SUBSCRIPTION,		RVSIP_LOG_ERROR_FILTER|RVSIP_LOG_EXCEP_FILTER|RVSIP_LOG_WARN_FILTER },
        { RVSIP_COMPARTMENT,		RVSIP_LOG_ERROR_FILTER|RVSIP_LOG_EXCEP_FILTER|RVSIP_LOG_WARN_FILTER },
        { RVSIP_RESOLVER,		    RVSIP_LOG_ERROR_FILTER|RVSIP_LOG_EXCEP_FILTER|RVSIP_LOG_WARN_FILTER },
        { RVSIP_TRANSMITTER,		RVSIP_LOG_ERROR_FILTER|RVSIP_LOG_EXCEP_FILTER|RVSIP_LOG_WARN_FILTER },
        /*{ RVSIP_SECURITY,			0 },
        { RVSIP_SEC_AGREE,			0 },*/
        { RVSIP_ADS_RLIST,			0 },
        { RVSIP_ADS_RA,				0 },
        { RVSIP_ADS_RPOOL,			0 },
        { RVSIP_ADS_HASH,			0 },
        { RVSIP_ADS_PQUEUE,		    0 },
        { RVSIP_CORE_SEMAPHORE,	    0 },
        { RVSIP_CORE_MUTEX,		    0 },
        { RVSIP_CORE_LOCK,			0 },
        { RVSIP_CORE_MEMORY,		0 },
        { RVSIP_CORE_THREAD,		0 },
        { RVSIP_CORE_QUEUE,		    0 },
        { RVSIP_CORE_TIMER,		    0 },
        { RVSIP_CORE_TIMESTAMP,	    0 },
        { RVSIP_CORE_CLOCK,		    0 },
        { RVSIP_CORE_TM,			0 },
        { RVSIP_CORE_SOCKET,		0 },
        { RVSIP_CORE_PORTRANGE,	    0 },
        { RVSIP_CORE_SELECT,		0 },
        { RVSIP_CORE_HOST,			0 },
        { RVSIP_CORE_TLS,			0 },
        { RVSIP_CORE_ARES ,		    0 },
        { RVSIP_CORE_RCACHE ,		0 },
        { RVSIP_CORE_EHD ,		    0 },
        { RVSIP_CORE_IMSIPSEC ,		0 },
        { RVSIP_CORE,				0 },
        { RVSIP_ADS,				0 },
    }
};

/*#define  ACL_RV_SIP_API_DEBUG*/

#ifdef ACL_RV_SIP_API_DEBUG 
#define DBG_PRINT(a1,a2...)	IppLogMessage(RV_FALSE, "[%s:%4d] " a1,__FUNCTION__, __LINE__, ##a2)
#else
#define DBG_PRINT(a1,a2...)
#endif

#define DBG_PRINT_ERR(a1,a2...)	IppLogMessage(RV_TRUE, "[%s:%4d] " a1,__FUNCTION__, __LINE__, ##a2)

#define LOG_FILE_NAME "logFile" /* ACL NB - TBD */

RvLogSource*	userLogSource = NULL; /* ACL NB */

/*  EA - 080214 - VI#60750 Infrastructure for unified CVT application
    as part of moving to the improved appilcation I improved the log mechanism
    RV_LOGLEVEL_ERROR | RV_LOGLEVEL_EXCEP | RV_LOGLEVEL_WARNING will be set to all sources by defualts
    setting MTF or USR logs will add RV_LOGLEVEL_INFO | RV_LOGLEVEL_DEBUG to the appropriate source
    the function bellow loops over the filter table and sets RV_LOGLEVEL_INFO | RV_LOGLEVEL_DEBUG to
    IPP_XXX sources (excluding IPP_USERAPP).
*/
static void mtf_log_filter_set(void)
{
    IppLogSourceFilterElm   *cursrcFilterElm = &g_ippLogOptions[1]; /*start from index 1 to exclude IPP_USERAPP*/

    while (0 != strcmp("", cursrcFilterElm->logSrcName))
    {
        if (NULL != strstr(cursrcFilterElm->logSrcName, "IPP_"))
        {
            cursrcFilterElm->messageMask |= RV_LOGLEVEL_INFO | RV_LOGLEVEL_DEBUG;
        }
        cursrcFilterElm++;
    }
}
/*  EA - 080214 - VI#60750 Infrastructure for unified CVT application
    Same as in the MTF. by default RVSIP_LOG_ERROR_FILTER|RVSIP_LOG_EXCEP_FILTER|RVSIP_LOG_WARN_FILTER
    will be set for all SIP modules (this does not include ADS and CORE). once sip logs flag is set
    RVSIP_LOG_INFO_FILTER | RVSIP_LOG_DEBUG_FILTER will be added too.
*/
static void sip_log_filter_set(void)
{
    RvIppSipModuleFilter    *cursrcFilterElm = &gIppSipLogOptions.filters[0];

    while (cursrcFilterElm->moduleId <= RVSIP_TRANSMITTER) /*Find something better*/
    {
        cursrcFilterElm->filter |= RVSIP_LOG_INFO_FILTER | RVSIP_LOG_DEBUG_FILTER;
        cursrcFilterElm++;
    }
}
/*  EA - 071225 Enabling Radvision's logs through acl_conf 
    changed the prototype of this function to support two levels
    of logging within the MTF. Note that setting isMtfLogOn includes
    the user's logs too
*/
/*  EA 080122 - VI#60671 - for Setting Syslog IP address for application logs through GUI
    I added 2 parameters IP and port 
*/
int acl_voip_mtf_log_construct(unsigned char isMtfLogOn, unsigned char isUsrLogOn)
{
   //console_printf("acl_voip_mtf_log_construct: %d \r\n", isLogOn);

    if (isMtfLogOn)
        	mtf_log_filter_set();
    if (isUsrLogOn)
        	g_ippLogOptions[0].messageMask |= RV_LOGLEVEL_INFO | RV_LOGLEVEL_DEBUG;
    if ( IppLogInit( g_ippLogOptions  , LOG_FILE_NAME) != RV_OK )
    {
        /* using printf since constructing Rv logs failed - check is printf OK !!!  */
        printf("acl_voip_mtf_log_construct: IppLogInit failed (no filters)!!! \r\n");
        return RV_ERROR_UNKNOWN;
    }
    DBG_PRINT("acl_voip_mtf_log_construct: IppLogInit succeeded\r\n");
    return RV_OK;
}

/* ACL NB 060814 */
void acl_voip_sip_log_construct(acl_voip_ipp_sip_cfg_t *pCfg, unsigned char isSipLogOn)
{  
    if ( !pCfg )
    {
        DBG_PRINT("acl_voip_sip_log_construct: no CFG struct!!! \r\n");
        return;
    }

    ((RvIppSipPhoneCfg *)pCfg)->logOptions= &gIppSipLogOptions;
    if (isSipLogOn)
        sip_log_filter_set();
    DBG_PRINT("acl_voip_sip_log_construct  \r\n");

    return;
}


void acl_voip_mutex_construct(acl_voip_mutex_t *m)
{
    /* rvMutexConstruct((RvMutex *)m); */
    RvMutexConstruct(NULL,(RvMutex *)m);
}

void acl_voip_mutex_destruct(acl_voip_mutex_t *m)
{
    /* rvMutexDestruct((RvMutex *)m); */
    RvMutexDestruct((RvMutex *)m,NULL);
}

void acl_voip_mutex_lock(acl_voip_mutex_t *m)
{
    /* rvMutexLock((RvMutex *)m); */
    RvMutexLock((RvMutex *)m,NULL);
}

void acl_voip_mutex_unlock(acl_voip_mutex_t *m)
{
    /* rvMutexUnlock((RvMutex *)m); */
    RvMutexUnlock((RvMutex *)m,NULL);
}


void acl_voip_sdp_msg_media_descr_add(acl_voip_sdp_msg_t *sdp, int port)
{
    /* 070729 - EA - merge to rv 2.5*/ 
    rvSdpMsgAddMediaDescr((struct _RvSdpMsg *)sdp, RV_SDPMEDIATYPE_AUDIO, port,
	RV_SDPPROTOCOL_RTP);
}

acl_voip_sdp_media_descr_t *acl_voip_sdp_msg_media_descr_get(acl_voip_sdp_msg_t *sdp, int i)
{
    /* 070729 - EA - merge to rv 2.5*/ 
    return (acl_voip_sdp_media_descr_t *)rvSdpMsgGetMediaDescr(
	(struct _RvSdpMsg *)sdp, i);
}

void acl_voip_sdp_msg_media_descr_clear(acl_voip_sdp_msg_t *sdp)
{
    /* 070729 - EA - merge to rv 2.5*/ 
    rvSdpMsgClearMediaDescr((struct _RvSdpMsg *)sdp);
}

int acl_voip_sdp_msg_media_descr_num_get(acl_voip_sdp_msg_t *sdp)
{
    /* 070729 - EA - merge to rv 2.5*/ 
    return rvSdpMsgGetNumOfMediaDescr((struct _RvSdpMsg *)sdp);
}

int acl_voip_sdp_msg_media_descr_type_get(acl_voip_sdp_media_descr_t *media)
{
   return rvSdpMediaDescrGetMediaType((RvSdpMediaDescr *)media); 
}

void acl_voip_sdp_media_descr_payload_add(acl_voip_sdp_media_descr_t *media, int pl)
{
    rvSdpMediaDescrAddPayloadNumber((RvSdpMediaDescr *)media, pl);
}

int acl_voip_sdp_media_descr_payload_get(acl_voip_sdp_media_descr_t *media, int i)
{
    return rvSdpMediaDescrGetPayloadNumber((RvSdpMediaDescr *)media, i);
}

int acl_voip_sdp_media_descr_payload_num_get(acl_voip_sdp_media_descr_t *media)
{
    return rvSdpMediaDescrGetNumOfPayloads((RvSdpMediaDescr *)media);
}

void acl_voip_sdp_media_descr_rtp_map_add(acl_voip_sdp_media_descr_t *media, int pl, char *plname, int rate)
{
    rvSdpMediaDescrAddRtpMap((RvSdpMediaDescr *)media, pl, plname, rate);
}

acl_voip_sdp_rtp_map_t *acl_voip_sdp_media_descr_rtp_map_get(acl_voip_sdp_media_descr_t *media, int i)
{
    return (acl_voip_sdp_rtp_map_t *)rvSdpMediaDescrGetRtpMap((RvSdpMediaDescr *)media, i);
}

int acl_voip_sdp_media_descr_rtp_map_num_get(acl_voip_sdp_media_descr_t *media)
{
    return rvSdpMediaDescrGetNumOfRtpMap((RvSdpMediaDescr *)media);
}

int acl_voip_sdp_media_descr_port_get(acl_voip_sdp_media_descr_t *media)
{
    return rvSdpMediaDescrGetPort((RvSdpMediaDescr *)media);
}

/* ACL NB 060810 */
int acl_voip_sdp_media_descr_port_set(acl_voip_sdp_media_descr_t *media, int port)
{
    return rvSdpMediaDescrSetPort((RvSdpMediaDescr *)media, port);
}

void acl_voip_sdp_media_descr_attr_add(acl_voip_sdp_media_descr_t *media, char *name, char *val)
{
    rvSdpMediaDescrAddAttr((RvSdpMediaDescr *)media, name, val);
}

void acl_voip_sdp_media_descr_attr_remove(acl_voip_sdp_media_descr_t *media, int index)
{
	/* EA - 070729 changed rvSdpMediaDescrRemoveAttr to rvSdpMediaDescrRemoveAttribute2
       rvSdpMediaDescrRemoveAttr isnt supported any more*/
    rvSdpMediaDescrRemoveAttribute2((RvSdpMediaDescr *)media, (RvSize_t)index);
}

acl_voip_sdp_attr_t *acl_voip_sdp_media_descr_attr_get(acl_voip_sdp_media_descr_t *media, int i)
{
    return (acl_voip_sdp_attr_t *)rvSdpMediaDescrGetAttribute((RvSdpMediaDescr *)media, i);
}

int acl_voip_sdp_media_descr_attr_num_get(acl_voip_sdp_media_descr_t *media)
{
    return rvSdpMediaDescrGetNumOfAttr((RvSdpMediaDescr *)media);
}

acl_voip_sdp_conn_t *acl_voip_sdp_media_descr_conn_get(acl_voip_sdp_media_descr_t *media)
{
    return (acl_voip_sdp_conn_t *)rvSdpMediaDescrGetConnection((RvSdpMediaDescr *)media);
}

int acl_voip_sdp_rtp_map_payload_get(acl_voip_sdp_rtp_map_t *map)
{
    return rvSdpRtpMapGetPayload((RvSdpRtpMap *)map);
}

char *acl_voip_sdp_rtp_map_encoding_name_get(acl_voip_sdp_rtp_map_t *map)
{
    return (char *)rvSdpRtpMapGetEncodingName((RvSdpRtpMap *)map);
}

char *acl_voip_sdp_attr_name_get(acl_voip_sdp_attr_t *attr)
{
    RvSdpAttribute *attr2 = (RvSdpAttribute*)attr;
    return (char *)rvSdpAttributeGetName(attr2);
}                                           

char *acl_voip_sdp_attr_value_get(acl_voip_sdp_attr_t *attr)
{
    return (char *)rvSdpAttributeGetValue((RvSdpAttribute *)attr);
}

acl_voip_sdp_conn_t *acl_voip_sdp_msg_conn_get(acl_voip_sdp_msg_t *sdp)
{
    /* 070729 - EA - merge to rv 2.5*/ 
    return (acl_voip_sdp_conn_t *)rvSdpMsgGetConnection((struct _RvSdpMsg *)sdp);
}

char *acl_voip_sdp_conn_addr_get(acl_voip_sdp_conn_t *conn)
{
    return (char *)rvSdpConnectionGetAddress((RvSdpConnection *)conn);
}

/* ACL NB 060411 */
const char *acl_voip_cc_modify_media_reason_text(int reason)
{
   return(rvCCTextModifyMediaReason((RvMdmTermReasonModifyMedia) reason));
}

/* ACL NB 060628 */
const char *acl_voip_cc_conn_state_text(int state)
{
   return(rvCCTextConnState((RvCCConnState) state));
}

/* ACL NB 061115 */
const char *acl_voip_cc_event_text(int eventId)
{
   return(rvCCTextEvent((RvCCTerminalEvent)eventId));
}

/* ACL NB 060706*/
/*==================================================================================*/
void acl_voip_sdp_print( char *sdp, const char* title)
{
	char	buf[1024];
	int	len = sizeof(buf) -1;
	RvSdpStatus  stat;
	rvSdpMsgEncodeToBuf( (RvSdpMsg *)sdp, buf, len, &stat);
	if(stat != RV_SDPSTATUS_OK)
	{
		DBG_PRINT("%sacl_voip_sdp_print() failed", title);
		return;
	}
	DBG_PRINT("%s\n%s\n", title, buf);
	return;
}
/*==================================================================================*/

int acl_voip_sip_msg_method_type_get(acl_voip_msg_t *msg)
{
  	RvSipCSeqHeaderHandle hCSeq;
	RvSipMethodType       method;

  	hCSeq  = RvSipMsgGetCSeqHeader((RvSipMsgHandle)msg);
	method = RvSipCSeqHeaderGetMethodType(hCSeq);

   return((int)method);
}
