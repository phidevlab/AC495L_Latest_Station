/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	acl_telephony_adv_sip_features.h                    *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 26/02/08 - Coding started.                                       *
 *																	*
 ********************************************************************/
#ifndef ACL_MWI_H
#define ACL_MWI_H
#include <RvSipStackTypes.h>
#include <acl_call_mngr.h>

#ifdef ACL_SIP_APPLICATION
#include "RvSipCallLegTypes.h"
//#include "sipMgr.h"
#endif

/*Define new user event*/
/* ACL NB 070516 - AC#53280 PATCH.Email received from RV */
/* Incident #129532: INFO message  16-May-07             */
#define USER_CCTERMEVENT_FLASH          (RV_CCTERMEVENT_USER + 1)


typedef struct
{
	RvSipStackHandle	stackHandle;
	RvString				localAddress;
	RvString				registrarAddress;
	RvUint16				registrarPort;
	int					stackUdpPort;		
//	RvString				subsServerName; /*like MWI@172.20.1.244:5060*/
  	char		         subsServerName[128];/*like MWI@172.20.1.244:5060*/

} acl_sip_mwi_cfg_t;


/* Set subscription related paramerers of Sip stack */
void acl_sip_mwi_set_stack_cfg( RvSipStackCfg* stackCfg);

/*	Start subscribtion per terminal.
 *  Can support maximum 1 terminal currently */
void acl_adv_sip_register_mdm_term( int termIndex, RvMdmTerm*	termination);
void acl_sip_advanced_init( acl_sip_mwi_cfg_t* subsCfg, acl_call_mngr_params_t *call_mngr_params );
void acl_sip_advanced_start(void);

/* Initiates outgoing call to the destination address on the specified line and terminal.AC#53086*/
RvStatus RVCALLCONV acl_make_call(RvIppTerminalHandle  terminalHndl, const RvChar* address);

/* ------------------------------------------------------*/
/* ACL NB 070516 - AC#53280 PATCH.Email received from RV */
/* Incident #129532: INFO message  16-May-07             */
/* ------------------------------------------------------*/
RvCCTerminalEvent acl_map_event_cb(  const char*		   pkg, 
								          const char*		   id,
								          RvMdmParameterList	*args, 
								          char*				   key);

/* ------------------------------------------------------*/
/* ACL NB 070516 - AC#53280 PATCH.Email received from RV */
/* Incident #129532: INFO message  16-May-07             */
/* ------------------------------------------------------*/
RvStatus acl_flash_hook_send(IN RvSipCallLegHandle hCallLeg,char *strHeaderType, char *strBody);

/* ------------------------------------------------------*/
/* ACL NB 071010-VI#57086 KA-sending OPTIONS periodically*/
/* ------------------------------------------------------*/
void acl_sip_send_option_msg_periodically(void *term);

void acl_sip_stop_send_option_msg_periodically(void *termId);

/* AlexChen 20090402 SDK upgrade */
void AclSipSendOptionMsgPeriodically(void *data);

void AclSipStopSendOptionMsgPeriodically(void *data);

int acl_stun_client_reply_received(RvChar* buf, RvSize_t size,RvUint32 *pMappedIp,	RvUint16 *pMappedPort);

int acl_handle_491_reply_received(IN RvSipMsgHandle   hMsg, OUT RvMdmTerm **term);

/* AlexChen 20090402 SDK upgrade */

/*************************************************************************************** 
    the follwoing enum represent possible events triggering action 
    by RedundantProxy_HandleSwitchRegistrar fucntion:

    e_REGISTRAR_KA_RESPONSED -                  registrar responded to keep alive option massage.
    e_REGISTRAR_KA_NOT_RESPONDED -              registrar not responded to keep alive option massage.
    e_REGISTRAR_REGISTRATION_NOT_RESPONDED -    registrar not responding on registration.
    for more information:
    \\Netapp1\Vopp tech Support\CMBU R&D\Docs\Design Documents\MP20x\2.6.x\Redundant sip proxy Design Document.doc
*****************************************************************************************/
typedef enum 
{
    e_REGISTRAR_KA_RESPONSED,       
    e_REGISTRAR_KA_NOT_RESPONDED,   
    e_REGISTRAR_REGISTRATION_NOT_RESPONDED       
}redundant_proxy_events_e;

/***************************************************************************
 * RedundantProxy_Init
 * ------------------------------------------------------------------------
 * General:			init redundant proxy data base recieved from user.
 * ------------------------------------------------------------------------
 * Arguments:		pMdmCfg - the input configuration from web.
 * 
 * return value:	none.
 ***************************************************************************/
void RedundantProxy_Init(IN acl_call_mngr_params_t *pMdmCfg);
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
void RedundantProxy_SwitchRegistrar(void);
/***************************************************************************
 * RedundantProxy_RegistrarIsRedundant
 * ------------------------------------------------------------------------
 * General:			return the registrar entity - primary or redundant proxy
 * ------------------------------------------------------------------------
 * Arguments:		none.
 * 
 * return value:	0 - primary proxy 1 - redundant proxy
 ***************************************************************************/
int RedundantProxy_RegistrarIsRedundant(void);
/***************************************************************************
 * RedundantProxy_RegistrarIsSymmetric
 * ------------------------------------------------------------------------
 * General:			return the redundnat proxy mode 
 * ------------------------------------------------------------------------
 * Arguments:		none.
 * 
 * return value:	0 - assymtric mode 1 - symmetric mode
 ***************************************************************************/
int RedundantProxy_RegistrarIsSymmetric(void);
/***************************************************************************
 * RedundantProxy_IsEnabled
 * ------------------------------------------------------------------------
 * General:			Return the Redundant Proxy feature mode: disabled/enbaled
 * ------------------------------------------------------------------------
 * Arguments:		none.
 * 
 * return value:	0 - disabled 1 - enabled
 ***************************************************************************/
int RedundantProxy_IsEnabled(void);
/***************************************************************************
 * RedundantProxy_GetKAOptionRate
 * ------------------------------------------------------------------------
 * General:			return keep alive option rate
 * ------------------------------------------------------------------------
 * Arguments:		none.
 * 
 * return value:	keep alive option rate in ms
 ***************************************************************************/
int RedundantProxy_GetKAOptionRate(void);
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
void RedundantProxy_HandleSwitchRegistrar(redundant_proxy_events_e event);
/***************************************************************************
 * RedundantProxy_getPort
 * ------------------------------------------------------------------------
 * General:			Return the Redundant Proxy port number
 * ------------------------------------------------------------------------
 * Arguments:		none.
 * 
 * return value:	int - redundant proxy port number
 ***************************************************************************/
int RedundantProxy_getPort(void);
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
const char *RedundantProxy_getAdrr(void);

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
char *GetRegistrarIP(void);

#endif/*ACL_MWI_H*/
