/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	call_mgt_stubs.c                                    *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 26/02/08 - Coding started.                                       *
 *																	*
 ********************************************************************/

#include "acl_call_mngr.h"

#ifndef CONFIG_RG_VOIP_RV_MGCP
acl_call_mngr_ops_t mgcp_call_mngr_ops;
#endif

#if !defined(CONFIG_RG_VOIP_RV_SIP) && !defined(CONFIG_RG_VOIP_RV_H323)
acl_call_mngr_ops_t term_call_mngr_ops;
#endif

#ifndef CONFIG_RG_VOIP_RV_H323
int mdm_h323_init(acl_call_mngr_params_t *call_mngr_params, void *term_mgr)
{
    return 0;
}

void h323_alias_free(acl_call_mngr_params_t *call_mngr_params)
{
}
#endif

#ifndef CONFIG_RG_VOIP_RV_SIP
int call_sip_init(acl_call_mngr_params_t *call_mngr_params, void *term_mgr)
{
    return 0;
}

void call_sip_uninit(void *stack_handle)
{
}
#endif

