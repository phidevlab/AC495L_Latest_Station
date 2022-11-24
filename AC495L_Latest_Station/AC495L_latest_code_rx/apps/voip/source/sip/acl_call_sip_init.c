/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	acl_call_sip_init.c                                 *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 26/02/08 - Coding started.                                       *
 *																	*
 ********************************************************************/
#include "acl_call_mngr.h"
#include "rvphone_common.h"
#include <acl_telephony_adv_sip_features.h> /* ACL NB 060927 */

/*start of memory consumption*/
#include "rvccoreglobals.h"
#include "rvmemory.h"
/*EA - 071225 Enabling Radvision's logs through acl_conf */
#include "acl_voip_conf.h"

//#include <openssl/ssl.h>

#include "voip_utils.h"

/*  EA - 071218 - VI#59630 support SIP over TLS */
#ifdef RV_CFLAG_TLS
#include <rvSipTlsApi.h>

#define TLS_CERTIFICATE_DEPTH               5
#define TLS_NUM_OF_ADDRESSES                1
#define TLS_TRUSTED_CERT_AUTH_FILE_NAME     "cacert.pem"
#define TLS_PRIVATE_KEY_FILE_NAME           "localhost.key-cert.pem"

#endif /* RV_CFLAG_TLS */

static RvInt32 allocatedByMtfFromOSCurr = 0;
static RvInt32 allocatedByXMLParserCurr = 0;
static RvInt32 allocatedBySipCurr = 0;
static RvInt32 allocatedBySdpCurr = 0;
static RvInt32 allocatedByCommonCoreCurr = 0;
static RvInt32 allocatedByAdsCurr = 0;
static RvInt32 totalAllocatedByAllCurr = 0;
static RvInt32 totalAllocatedByAllGrossCurr = 0;

static RvInt32 allocatedByMtfFromOSPrev = 0;
static RvInt32 allocatedByXMLParserPrev = 0;
static RvInt32 allocatedBySipPrev = 0;
static RvInt32 allocatedBySdpPrev = 0;
static RvInt32 allocatedByCommonCorePrev = 0;
static RvInt32 allocatedByAdsPrev = 0;
static RvInt32 totalAllocatedByAllPrev = 0;
static RvInt32 totalAllocatedByAllGrossPrev = 0;

#define RV_MEMORY_BOUNDRY_SIZE 16 /* minumum size */

/*EA - 071225 Enabling Radvision's logs through acl_conf */
/*  the log filter will be set acording to a mask of these bits 
    there are the following combinations:
    0 = NONE
    1 = MTF + USR (note that MTF already includes USR)
    2 = SIP
    3 = MTF + USR + SIP
    4 = USR
    5 = MTF + USR == 1
    6 = SIP + USR
    7 = MTF + USR + SIP == 3
*/
#define MDM_RV_MTF_LOG_FILTER               0x1 /* MTF includes USR logs too */
#define MDM_RV_SIP_LOG_FILTER               0x2 /* only SIP stack logs */
#define MDM_RV_USR_LOG_FILTER               0x4 /* only USR logs*/
#define MDM_RV_LOG_FILTER_BITS              (MDM_RV_MTF_LOG_FILTER | MDM_RV_SIP_LOG_FILTER | MDM_RV_USR_LOG_FILTER)

#define MDM_RV_IS_SET(val, filter)          ((val & MDM_RV_LOG_FILTER_BITS) & filter)  


typedef struct {
    RvMemory *region; /* always needed */
#if (RV_MEMORY_DEBUGINFO == RV_YES)
    RvObjListElement alloclistelem;
    RvInt line;
    const RvChar *filename;
    RvThread *thread;
    RvThreadId id;
#endif
#if (RV_MEMORY_DEBUGCHECK == RV_YES)
    RvBool freed;
    size_t reqsize;
    RvUint8 boundry[RV_MEMORY_BOUNDRY_SIZE]; /* Must be last item in structure */
#endif
} RvMemoryAllocHead;
/*end of memory consumption*/

/*#define  ACL_SIP_CALL_MNGR_DEBUG */

#ifdef ACL_SIP_CALL_MNGR_DEBUG
//#include <misc_funcs.h>
#define DBG_PRINT(a1,a2...)	IppLogMessage(RV_FALSE, "[%s:%4d] " a1,__FUNCTION__, __LINE__, ##a2)
#else
#define DBG_PRINT(a1,a2...)
#endif

/* AlexChen 20090402 SDK upgrade */
enum {
	e_PRIMARY_PROXY,
	e_REDUNDANT_PROXY,
	e_REGISTRAR,
	e_DONT_USE_REGISTRAR	
}RegistrarProxyEntinty_e;

static char *g_KA_OptionEntityIp = NULL;
static void SetKAoptionIpEntity(acl_call_mngr_params_t *call_params);
static void SetRegistrarCfg(acl_call_mngr_params_t *call_params, RvIppSipPhoneCfg *cfg);
static int GetRegistrarEntity(acl_call_mngr_params_t *call_params);

/*EA - 080429 - AC63453 - Code validation*/
extern int acl_validation_init(void);

/***************************************************************************************
* allocations_per_toolkit_get
* -------------------------------------------------------------------------------------
* General:   This function is using Common Core global object for calculating the amount of 
*            memory allocations used by each toolkit. The function works only if MTF is compiled
*            with RV_MEMORY_DEBUGCHECK.
*              
* Return Value: None
* -------------------------------------------------------------------------------------
* Arguments:
* Input:       None
* Output: 	    allocatedByMtf - memory allocated by MTF (will include XML parser too)
*              allocatedByXMLParser - memory allocated by XML parser
*              allocatedBySip - memory allocated by SIP
*              allocatedBySdp - memory allocated by SDP
*              allocatedByCommonCore - memory allocated by Common Core
*              allocatedByAds - memory allocated by ADS
**************************************************************************************/
#if RV_MEMORY_DEBUGCHECK == RV_YES
static void allocations_per_toolkit_get(
                                     OUT RvInt32* allocatedByMtf,
                                     OUT RvInt32* allocatedByXMLParser,
                                     OUT RvInt32* allocatedBySip,
                                     OUT RvInt32* allocatedBySdp,
                                     OUT RvInt32* allocatedByCommonCore,
                                     OUT RvInt32* allocatedByAds)
{
    RvMemory* curRegion;
    RvObjList* regionList;
    RV_USE_CCORE_GLOBALS; /*This line provides us access to RvRegionList */
    
    regionList = &RvRegionList;
    
    /* Go through each memory region */
    /* ----------------------------- */
    curRegion = (RvMemory*)RvObjListGetNext(regionList, NULL, RV_OBJLIST_LEAVE);
    while (curRegion != NULL)
    {
        //   if (RvLockGet(&curRegion->listlock, IppLogMgr()) == RV_OK) /* removed since it's causing deadlock */
        {
            RvMemoryAllocHead *cur;
            
            /* Go through each allocation in region */
            /* ------------------------------------ */            
            cur = (RvMemoryAllocHead *)RvObjListGetNext(&curRegion->alloclist, NULL, RV_OBJLIST_LEAVE);
            while (cur != NULL)
            {
                if ((strstr(cur->filename, "\\mtf\\")) || (strstr(cur->filename, "Mtf/")))
                {
                    *allocatedByMtf += cur->reqsize;                
                }
                
                if ((strstr(cur->filename, "\\mtf\\xmlparser\\")) || (strstr(cur->filename, "Mtf/xmlparser/")))
                {
                    *allocatedByXMLParser += cur->reqsize;                
                }
                
                if ((strstr(cur->filename, "\\sip\\")) || (strstr(cur->filename, "sip/")))
                {
                    *allocatedBySip += cur->reqsize;                
                }
                
                if ((strstr(cur->filename, "\\sdp\\")) || (strstr(cur->filename, "sdp/")))
                {
                    *allocatedBySdp += cur->reqsize;                
                }
                
                if ((strstr(cur->filename, "\\common\\")) || (strstr(cur->filename, "common/")))
                {
                    *allocatedByCommonCore += cur->reqsize;                
                }
                
                if ((strstr(cur->filename, "\\ads\\")) || (strstr(cur->filename, "ads/")))
                {
                    *allocatedByAds += cur->reqsize;                
                }
                
                cur = (RvMemoryAllocHead *)RvObjListGetNext(&curRegion->alloclist, cur, RV_OBJLIST_LEAVE);
            }
        }
        curRegion = (RvMemory *)RvObjListGetNext(regionList, curRegion, RV_OBJLIST_LEAVE);
    }
}
#endif /* RV_MEMORY_DEBUGCHECK */

/***************************************************************************************
 * memory_resources_print
 * -------------------------------------------------------------------------------------
 * General:   This function prints details of the resources used by MTF and other toolkits, 
 *            including amount of allocated memory and OS resources (timers etc.).
 *              
 * Return Value: None
 * -------------------------------------------------------------------------------------
 * Arguments:
 * Input:       None
 * Output: 	    None
 **************************************************************************************/
void memory_resources_print(void)
{
    RvMemoryInfo memInfo;
    RvInt32 allocatedByMtfFromOS = 0;
    RvInt32 allocatedByXMLParser = 0;
    RvInt32 allocatedBySip = 0;
    RvInt32 allocatedBySdp = 0;
    RvInt32 allocatedByCommonCore = 0;
    RvInt32 allocatedByAds = 0;
    RvInt32 totalAllocatedByAll = 0;
    RvInt32 totalAllocatedByAllGross = 0;

    /* Print titles */
    /* ------------ */

    DBG_PRINT("\r\n---------Resources After Startup---------\r\n\r\n"); 
    DBG_PRINT("Allocated Memory:\r\n-----------------\r\n");
    
#if RV_MEMORY_DEBUGCHECK == RV_YES

    /* Print allocations for each toolkit */
    /* ---------------------------------- */
    
    allocatedByMtfFromOSCurr = 0;
    allocatedByXMLParserCurr = 0;
    allocatedBySipCurr = 0;
    allocatedBySdpCurr = 0;
    allocatedByCommonCoreCurr = 0;
    allocatedByAdsCurr = 0;

    allocations_per_toolkit_get(
        &allocatedByMtfFromOSCurr,  
        &allocatedByXMLParserCurr,
        &allocatedBySipCurr,  
        &allocatedBySdpCurr,  
        &allocatedByCommonCoreCurr,
        &allocatedByAdsCurr);

    allocatedByMtfFromOS = allocatedByMtfFromOSCurr - allocatedByMtfFromOSPrev;
    allocatedByXMLParser = allocatedByXMLParserCurr - allocatedByXMLParserPrev;
    allocatedBySip = allocatedBySipCurr - allocatedBySipPrev;
    allocatedBySdp = allocatedBySdpCurr - allocatedBySdpPrev;
    allocatedByCommonCore = allocatedByCommonCoreCurr - allocatedByCommonCorePrev;
    allocatedByAds = allocatedByAdsCurr - allocatedByAdsPrev;

    DBG_PRINT("Allocated by MTF:                 %.1lfKB (%.1lf)\r\n", (double)allocatedByMtfFromOSCurr/1024, 
                                                                            (double)(allocatedByMtfFromOS-allocatedByXMLParser)/1024);/*allocatedByMtfFromOS includes XML parser allocations */    
    DBG_PRINT("Allocated by XML Parser:          %.1lfKB (%.1lf)\r\n", (double)allocatedByXMLParserCurr/1024,
                                                                            (double)allocatedByXMLParser/1024);
    DBG_PRINT("Allocated by SIP:                 %.1lfKB (%.1lf)\r\n", (double)allocatedBySipCurr/1024,
                                                                            (double)allocatedBySip/1024);
    DBG_PRINT("Allocated by SDP:                 %.1lfKB (%.1lf)\r\n", (double)allocatedBySdpCurr/1024,
                                                                            (double)allocatedBySdp/1024);
    DBG_PRINT("Allocated by Common Core:         %.1lfKB (%.1lf)\r\n", (double)allocatedByCommonCoreCurr/1024,
                                                                            (double)allocatedByCommonCore/1024);
    DBG_PRINT("Allocated by ADS:                 %.1lfKB (%.1lf)\r\n", (double)allocatedByAdsCurr/1024,
                                                                            (double)allocatedByAds/1024);
#else
    DBG_PRINT("<You are not compiled with RV_DEBUG>");
    
    RV_UNUSED_ARG(allocatedByMtfFromOS);
    RV_UNUSED_ARG(allocatedByXMLParser);
    RV_UNUSED_ARG(allocatedBySip);
    RV_UNUSED_ARG(allocatedBySdp);
    RV_UNUSED_ARG(allocatedByCommonCore);
    RV_UNUSED_ARG(allocatedByAds);
    RV_UNUSED_ARG(totalAllocatedByAll);
    RV_UNUSED_ARG(totalAllocatedByAllGross);
#endif /* RV_MEMORY_DEBUGCHECK */
    
    /* Print total allocation */
    /* ---------------------- */

    RvMemoryGetInfo(NULL, NULL, &memInfo);
    totalAllocatedByAllCurr = memInfo.bytes_requested_byuser;
    totalAllocatedByAllGrossCurr = memInfo.bytes_requested_total;

    totalAllocatedByAll = totalAllocatedByAllCurr - totalAllocatedByAllPrev;
    totalAllocatedByAllGross = totalAllocatedByAllGrossCurr - totalAllocatedByAllGrossPrev;

    DBG_PRINT("                                  -----------------\r\n");
    DBG_PRINT("Total allocated by ALL (net):     %.1lfKB (%.1lfKB)\r\n", (double)totalAllocatedByAllCurr/1024,
                                                                            (double)totalAllocatedByAll/1024);     
    DBG_PRINT("Total allocated by ALL (gross):   %.1lfKB (%.1lfKB)\r\n", (double)totalAllocatedByAllGrossCurr/1024,
                                                                            (double)totalAllocatedByAllGross/1024);  
    /* Store last allocation count */
    /* --------------------------- */

    allocatedByMtfFromOSPrev = allocatedByMtfFromOSCurr;
    allocatedByXMLParserPrev = allocatedByXMLParserCurr;
    allocatedBySipPrev = allocatedBySipCurr;
    allocatedBySdpPrev = allocatedBySdpCurr;
    allocatedByCommonCorePrev = allocatedByCommonCoreCurr;
    allocatedByAdsPrev = allocatedByAdsCurr;
    totalAllocatedByAllPrev = totalAllocatedByAllCurr;
    totalAllocatedByAllGrossPrev = totalAllocatedByAllGrossCurr;
}

#ifdef RV_CFLAG_TLS
/*  EA - 071218 - VI#59630 support SIP over TLS */
/*  I had to insert this stub because we had problems linking radvision's MTF 
    with the openssl library
*/


void sip_tls_params_set(acl_call_mngr_params_t *params)
{
    /*  set defaults - I hold this structure inside voip_call_mgt_params_t
        for possible use in the future. basically today only tls port will be configured*/
    /* RvIppTransportTlsCfg struct*/
    params->sp.sip.transportTlsCfg.stackTlsAddress = params->local_ip;
    /*  EA - 080210 VI#61153 SIP TLS is workng only on port 5061
        I commented this line - no wonder why TLS was working only with 5061*/
    /*params->sp.sip.transportTlsCfg.stackTlsPort = 5061;*/
    params->sp.sip.transportTlsCfg.certDepth = TLS_CERTIFICATE_DEPTH;
    params->sp.sip.transportTlsCfg.privateKeyType = RVSIP_TRANSPORT_PRIVATE_KEY_TYPE_RSA_KEY; /*0*/
    params->sp.sip.transportTlsCfg.tlsMethod = RVSIP_TRANSPORT_TLS_METHOD_TLS_V1; /*2*/
    params->sp.sip.transportTlsCfg.tlsPostConnectAssertFlag = 1; /*RV_TRUE*/
    /* acl_sip_tls_key_cfg_t struct*/
    strcpy(params->sp.sip.keyTlsCfg.caCertFileName[0], TLS_TRUSTED_CERT_AUTH_FILE_NAME);
    strcpy(params->sp.sip.keyTlsCfg.privateKeyFileName, TLS_PRIVATE_KEY_FILE_NAME);
    /*If TLS was set change TLS to TCP because TLS will be ignored by the MTF*/
    if (VOIP_SIP_TRANSPORT_TLS == params->sp.sip.sip_transport_protocol)
    {
        params->sp.sip.sip_transport_protocol = VOIP_SIP_TRANSPORT_TCP;
        params->sp.sip.transportTlsCfg.stackNumOfTlsAddresses = TLS_NUM_OF_ADDRESSES;
    }
    else
    {
        /*  EA - 080130 - VI#60866 - Voip Crash occur when changing sip mode to TLS and TCP
            the MTF knows only 2 transport types UDP or TCP and if the MTF is compiled with
            RV_CFLAG_TLS ON then TCP turns into TLS. So transport type ==  TCP and RV_CFLAG_TLS is ON 
            you must disable the TLS by setting stackNumOfTlsAddresses to 0*/
        params->sp.sip.transportTlsCfg.stackNumOfTlsAddresses = 0; /*TLS is disabled*/
    }
}
#endif /* RV_CFLAG_TLS */ 

int call_sip_init(acl_call_mngr_params_t *call_mngr_params, void *term_mgr)
{
    RvSipStackHandle stack_handle;
    RvIppSipPhoneCfg cfg;
    acl_sip_mwi_cfg_t     subsCfg; /* ACL NB 060926 - MWI support */
    int              rvLogFilter = 0;

    AC_CODE2CODE_MAP sip_transport_protocols[] = {
	{ VOIP_SIP_TRANSPORT_UDP, RVSIP_TRANSPORT_UDP },
	{ VOIP_SIP_TRANSPORT_TCP, RVSIP_TRANSPORT_TCP },
	{ VOIP_SIP_TRANSPORT_TLS, RVSIP_TRANSPORT_TLS },
	{ -1 , -1}
    };

    if(acl_validation_init() != 0)
    {
	   printf("error at acl_validation_init\r\n");
	   exit(0);
    }

    /* Regsiter SIP */
    AC_RG_COMPONENT_REGISTER(RG_COMP_RV_SIP);

    /* init sip */
    rvIppSipSystemInit();
    
    MZERO(cfg);

    /* ACL NB 060521 Radvision2.0 */
    /* init sip Log !! should be after rvIppSipSystemInit and before rvIppSipStackInitialize */
    /*EA - 071225 Enabling Radvision's logs through acl_conf */
    rvLogFilter = voip_conf_rv_log_filter_get();

    /*NirM 080904 - VI#60671 - for Setting Syslog IP address for application logs through GUI*/
    if (acl_voip_mtf_log_construct( MDM_RV_IS_SET(rvLogFilter, MDM_RV_MTF_LOG_FILTER), 
                                    MDM_RV_IS_SET(rvLogFilter, MDM_RV_USR_LOG_FILTER)) != RV_OK)
    {
		return 1; /*Stop the application otherwise it will crash when trying to print !!!*/
    }
 
    /* Initialize IPP configuration structure with default values*/
    rvIppSipInitConfig(&cfg);

    /* ACL NB 061012 - relocation of rvIppSipRegisterExtClbks - Should be called before rvIppSipStackInitialize*/
    sip_register_ext_clbks((void *)&cfg);
    
    /*  EA - 071218 - VI#59630 support SIP over TLS */
    /* Initialize TLS */

#ifdef RV_CFLAG_TLS
    sip_tls_register_ext_clbks();
    /* Initialize TLS part of IPP configuration structure with default values*/
    rvIppSipTlsInitConfig(&cfg);
    /* set TLS configuration parameters to the mdm_params structure*/
    sip_tls_params_set(call_mngr_params);
    /* set TLS configuration parameters from GUI to IPP configuration structure*/
    sip_tls_mtf_config_params_set(&cfg);
#else /* RV_CFLAG_TLS */
    DBG_PRINT("TLS is not supported\r\n");
#endif /* RV_CFLAG_TLS */

    /* ACL NB 060815 - Open SIP logs */
    /*EA - 071225 Enabling Radvision's logs through acl_conf */
    acl_voip_sip_log_construct((acl_voip_ipp_sip_cfg_t *)&cfg, 
                                MDM_RV_IS_SET(rvLogFilter, MDM_RV_SIP_LOG_FILTER));
           
    cfg.transportType = AC_CODE_TO_VALUE(sip_transport_protocols,
	call_mngr_params->sp.sip.sip_transport_protocol);

    if (cfg.transportType == RVSIP_TRANSPORT_TCP)
	cfg.stackTcpPort = call_mngr_params->sp.sip.sip_port;
    else if (cfg.transportType == RVSIP_TRANSPORT_UDP)
	cfg.stackUdpPort = call_mngr_params->sp.sip.sip_port;
   DBG_PRINT("cfg.stackUdpPort %d, stackTcpPort %d\r\n", cfg.stackUdpPort, cfg.stackTcpPort);
   DBG_PRINT("cfg.transportType %d\r\n", cfg.transportType);
    
    cfg.autoRegister = 0;
    cfg.localAddress = call_mngr_params->local_ip;
    DBG_PRINT("RV conf using localAddress %s\n", cfg.localAddress);//Avner August PS
    
    /* Disable the dialTone timeout feature (use our implementation instead) */
    cfg.dialToneDuration = 0;

    // by itzik - 20 June 2006 - add outbound proxy
	if (call_mngr_params->sp.sip.use_outbound_proxy)
	{
		cfg.outboundProxyAddress = call_mngr_params->sp.sip.outbound_proxy_address;
  		cfg.outboundProxyPort = call_mngr_params->sp.sip.outbound_proxy_port;
	}

#if 0    /* AlexChen 20090402 SDK upgrade */
	if(call_mngr_params->sp.sip.use_proxy_port_ip_for_registrar &&
        (call_mngr_params->sp.sip.use_proxy)) {

		cfg.autoRegister = 1;
		cfg.registrarAddress = call_mngr_params->sp.sip.proxy_address;
		cfg.registrarPort    = call_mngr_params->sp.sip.proxy_port;
		cfg.registrationExpire = call_mngr_params->sp.sip.proxy_registration_timeout;

	}
	else {
		if (call_mngr_params->sp.sip.use_registrar)
		{

			cfg.autoRegister = 1;
			cfg.registrarAddress = call_mngr_params->sp.sip.registrar_address;
			cfg.registrarPort    = call_mngr_params->sp.sip.registrar_port;
			cfg.registrationExpire = call_mngr_params->sp.sip.proxy_registration_timeout;

		}
 	   else {
  	     	cfg.autoRegister = 0;
		   	cfg.registrarAddress = "";
	    }
	}
#endif

		/* AlexChen 20090402 SDK upgrade */
	cfg.registrationExpire = call_mngr_params->sp.sip.proxy_registration_timeout;
	RedundantProxy_Init(call_mngr_params);
    if(RedundantProxy_IsEnabled())
    {
		SetKAoptionIpEntity(call_mngr_params);    
    }
	SetRegistrarCfg(call_mngr_params, &cfg);

/* by itzik - 20 June 2006 - authentication is now per line
    if (mdm_params->sp.sip.use_proxy)
    {
	cfg.username = mdm_params->sp.sip.proxy_user_name;
	cfg.password = mdm_params->sp.sip.proxy_password;
	cfg.registrationExpire =
	    mdm_params->sp.sip.proxy_registration_timeout;
    }
*/
	cfg.username = NULL;
	cfg.password = NULL;


   /* ACL NB 060521 Radvision2.0 */
   if (call_mngr_params->sp.sip.user_doamin[0])
      cfg.userDomain = call_mngr_params->sp.sip.user_doamin;
   else
	  cfg.userDomain = call_mngr_params->local_ip; 
    DBG_PRINT("RV conf using userDomain %s\n", cfg.userDomain);//Avner August PS

	cfg.unregistrationExpire = call_mngr_params->sp.sip.uregistration_timeout;
	cfg.watchdogTimeout  = call_mngr_params->sp.sip.watchdog_timeout;
	cfg.callWaitingReply = call_mngr_params->sp.sip.callWaitingReply;
	/*ACL JL 59875 limits number of registrations to proxy to  a configurable number RV patch*/
	cfg.maxAuthenticateRetries = call_mngr_params->sp.sip.auth_retries;
    /*EA - 071115 - connectMediaOn180*/
    cfg.connectMediaOn180 = call_mngr_params->sp.sip.connectMediaOn180;

    // by itzik - 20 june 2006 - move init function, after setting cfg
	DBG_PRINT("calling rvIppSipStackInitialize\n");//Avner August PS
    if (rvIppSipStackInitialize(&stack_handle, &cfg) == rvFalse)
		return 1;
    DBG_PRINT("rvIppSipStackInitialize returned OK\n");//Avner August PS
/* ACL NB 060926 
 * After stack was initialized, we can use stack handle in order to 
 * construct SipSubsMgr object - responsible for SUBSCRIBE/NOTIFY maintanance 
   and some more  */
   

		subsCfg.stackHandle			= stack_handle;
		subsCfg.localAddress		= call_mngr_params->local_ip;
		subsCfg.registrarAddress	= cfg.registrarAddress; // use cfg becuase there are some manipulation on the registrar see above!!!
		subsCfg.registrarPort		= cfg.registrarPort;
		subsCfg.stackUdpPort		= call_mngr_params->sp.sip.sip_port;

      acl_sip_advanced_init( &subsCfg, call_mngr_params );

    /* ACL NB 060718 - DTMF Via SIP */
    if (call_mngr_params->dtmf_transport_method == DTMF_VIA_SIP )
	    cfg.outOfBandDtmf = rvTrue;  
    else
        cfg.outOfBandDtmf =  rvFalse;
    rvIppSipPhoneConstruct(term_mgr, &cfg, stack_handle);

    /*start of memory consumption*/
    /*memory_resources_print();*/

    /*RvMemoryPrintDbg(NULL, IppLogMgr());*/
    /*end of memory consumption*/

  	 acl_sip_advanced_start();


    rvMdmTermMgrSetUserData(term_mgr, (void *)stack_handle);
    
    return 0;
}

void call_sip_uninit(void *stack_handle)
{
    RvSipStackDestruct(stack_handle);
}

//060530 Avner phase 1
sip_reg_state_t mdm_sip_registration_status(int epn)
{
//    RvSipControl *sip_mgr = rvCCSipPhoneGetSipMgr();

    /* XXX test registration status */
    return SIP_REG_ERROR;
}


/***************************************************************************************
 * SetKAoptionIpEntity
 * -------------------------------------------------------------------------------------
 * General:   sets the ip address for the keep-alive OPTION message 
 *              
 * Return Value: None
 * -------------------------------------------------------------------------------------
 * Arguments:
 * Input:       mdm_params - the user configuration input
 * Output: 	    none
 **************************************************************************************/
/*AlexChen 20090402 SDK upgrade */
static void SetKAoptionIpEntity(acl_call_mngr_params_t *call_params)
{
	int IsSimetric = RedundantProxy_RegistrarIsSymmetric();
	int IsRedundantRegistrar = RedundantProxy_RegistrarIsRedundant();
	char Str[10] = {0};
	DBG_PRINT("->\r\n");
	if(IsSimetric)
	{
		sprintf(Str, "symmetric");
		if(IsRedundantRegistrar)
			g_KA_OptionEntityIp = call_params->sp.sip.redundant_proxy_address;
		else
			g_KA_OptionEntityIp = call_params->sp.sip.proxy_address;
	}else
	{
		g_KA_OptionEntityIp = call_params->sp.sip.proxy_address;
		sprintf(Str, "a-symmetric");
	}
	DBG_PRINT("keep-alive ip entity is %s\r\n", g_KA_OptionEntityIp);
	DBG_PRINT("redundant proxy is in %s mode\r\n", Str);
	DBG_PRINT("<-\r\n");
}

/***************************************************************************************
 * GetKAoptionIpEntity
 * -------------------------------------------------------------------------------------
 * General:   returns the ip address for the keep-alive OPTION message 
 *              
 * Return Value: the static global variable ip address for the keep-alive OPTION message 
 * -------------------------------------------------------------------------------------
 * Arguments:
 * Input:       none
 * Output: 	    none
 **************************************************************************************/
/*AlexChen 20090402 SDK upgrade */
char *GetKAoptionIpEntity(void)
{
	DBG_PRINT("->\r\n");
	return g_KA_OptionEntityIp;
	DBG_PRINT("<-\r\n");
}


/***************************************************************************************
 * SetRegistrarCfg
 * -------------------------------------------------------------------------------------
 * General:   set the registrar configuration according to user input
 *              
 * Return Value: None
 * -------------------------------------------------------------------------------------
 * Arguments:
 * Input:       mdm_params - the user configuration input
 * Output: 	    cfg - the configuration for radvison stack
 **************************************************************************************/
/*JL VI # 62393 redundant proxy feature */
static void SetRegistrarCfg(acl_call_mngr_params_t *call_params, RvIppSipPhoneCfg *cfg)
{
		
	int RegistrarEntity = GetRegistrarEntity(call_params);
	
	DBG_PRINT("->\r\n");

	switch(RegistrarEntity) {
	case e_PRIMARY_PROXY:
			cfg->registrarAddress = call_params->sp.sip.proxy_address;
			cfg->registrarPort    = call_params->sp.sip.proxy_port;
			cfg->autoRegister = 1;
		break;
	case e_REDUNDANT_PROXY:
			cfg->registrarAddress = call_params->sp.sip.redundant_proxy_address;
			cfg->registrarPort    = call_params->sp.sip.redundant_proxy_port;
			cfg->autoRegister = 1;
		break;
	case e_REGISTRAR:
			cfg->registrarAddress = call_params->sp.sip.registrar_address;
			cfg->registrarPort    = call_params->sp.sip.registrar_port;
			cfg->autoRegister = 1;
		break;

	default:
		cfg->registrarAddress = "";
        break;

	}
	printf("autoRegister(%d) registrarAddress(%s) registrarPort(%d)\r\n", 
		cfg->autoRegister, cfg->registrarAddress, cfg->registrarPort);
	printf("<-\r\n");
}

/***************************************************************************************
 * GetRegistrarEntity
 * -------------------------------------------------------------------------------------
 * General:   Returns the registrar entity to be used according to user input.
 *			  In case the reduandant proxy feature is enabled we also check
 *			  which proxy to register - redundant or primary.
 *              
 * Return Value: enum that represents the registrar entity to be used
 * -------------------------------------------------------------------------------------
 * Arguments:
 * Input:       mdm_params - the user configuration input
 * Output: 	    none
 **************************************************************************************/
/*JL VI # 62393 redundant proxy feature */
static int GetRegistrarEntity(acl_call_mngr_params_t *call_params)
{

	int IsRedundatRegistrar = RedundantProxy_RegistrarIsRedundant();
    DBG_PRINT("->\r\n");
	if(call_params->sp.sip.use_redundant_proxy && IsRedundatRegistrar)
	{
		DBG_PRINT("e_REDUNDANT_PROXY \r\n");
		return e_REDUNDANT_PROXY;
	}else
	{
		if(call_params->sp.sip.use_proxy_port_ip_for_registrar &&
        (call_params->sp.sip.use_proxy)) 
		{
			DBG_PRINT("e_PRIMARY_PROXY \r\n");
			return e_PRIMARY_PROXY;
		}
		else 
		{
			if (call_params->sp.sip.use_registrar)
			{
				DBG_PRINT("e_REGISTRAR \r\n");
				return e_REGISTRAR;
			}
		}
	}
	DBG_PRINT("e_DONT_USE_REGISTRAR \r\n");
	return e_DONT_USE_REGISTRAR;
}

