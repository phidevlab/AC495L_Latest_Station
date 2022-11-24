/********************************************************************
 * AudioCodes improved application Initialization of MGCP stack     *
 *                                                                  *
 * File Name:	acl_mgcp_call_init.c                                *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 30/04/08 - Coding started.                                       *
 *																	*
 ********************************************************************/
#include "acl_call_mngr.h"
#include "rvendptmgr.h"
#include "voip_utils.h"

#define  ACL_MGCP_CALL_INIT_DEBUG

#define DBG_PRINT_ERR(a1,a2...)	printf("[%s:%4d] ERROR: \n" a1,__FUNCTION__, __LINE__, ##a2)

#ifdef ACL_MGCP_CALL_INIT_DEBUG
#define DBG_PRINT(a1,a2...)	printf("[%s:%4d] \n" a1,__FUNCTION__, __LINE__, ##a2)
#else /* ACL_MGCP_CALL_INIT_DEBUG */
#define DBG_PRINT(a1,a2...)
#endif /* ACL_MGCP_CALL_INIT_DEBUG */

static RvMgcpStack mgcp_stack; /*used only here*/

RvMgcpEndptMgr ep_mgr; /*initialized here and used also in acl_mgcp_call_mngr.c*/
RvMgcpNotifiedEntity mgc_entity; /*initialized here and used also in acl_mgcp_call_mngr.c*/

extern RvMgcpEndpoint *mgcp_eps; /*initialized in acl_mgcp_call_mngr.c and used also here*/

extern int acl_validation_val_get(char	*regVal);

int call_mgcp_validation()
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

int call_mgcp_init(acl_call_mngr_params_t *params, acl_call_mngr_cbs_t *cbs)
{
    RvPriority priority;

	DBG_PRINT(">>\r\n");

    if(acl_validation_init() != 0)
    {
	   printf("error at acl_validation_init\r\n");
	   exit(0);
    }

    if (call_mgcp_validation())
    {
	exit(0);
    }

    rvMgcpSystemInit();

    /* At this point the rvLog object was created and we can set the log */
    /*    acl_voip_log_construct(RV_LOGLEVEL_ERROR | RV_LOGLEVEL_WARNING,
	RV_LOGMASK_NONE, 0);*/
	acl_mgcp_log_construct(0); /* JL @@@*/

    /* create the mgcp stack */
    MZERO(mgcp_stack);
    rvPriorityConstruct2(&priority, RV_PRIORITY_NORMAL);
    rvMgcpStackConstruct(&mgcp_stack, RV_MGCPSTACKTYPE_MG, "AudioCodes Gateway",
	params->local_ip, params->sp.mgcp.mg_port, 1 /*one reciever
	 thread*/, priority);
    rvPriorityDestruct(&priority);
    MZERO(ep_mgr);
    /* create the ep-mgr associated with the stack */
    rvMgcpEndptMgrConstruct(&ep_mgr, &mgcp_stack);
    /* and the entity presenting the MGC */
    DBG_PRINT("Construct notified entity (ip=%s port=%d)",
	params->sp.mgcp.mgc_address, params->sp.mgcp.mgc_port);
    rvMgcpNotifiedEntityConstruct(&mgc_entity, "ca",
	params->sp.mgcp.mgc_address, params->sp.mgcp.mgc_port);
    /* startup the stack (and its underlying rcv thread) */
    
	rvMgcpSystemRegisterStack(&mgcp_stack);

    return 0;
}

int call_mgcp_uninit(void)
{
    AC_RG_COMPONENT_UNREGISTER(RG_COMP_RV_MGCP);
#if 0
    rvMgcpSystemUnregisterStack(&mgcp_stack);
    rvMgcpEndptMgrProcessAllEndpoints(&ep_mgr, ep_delete_cb, NULL);
    rvMgcpNotifiedEntityDestruct(&mgc_entity);
    rvMgcpEndptMgrDestruct(&ep_mgr);
    rvMgcpStackDestruct(&mgcp_stack);
    rvMgcpSystemEnd();
    free(mgcp_eps);
    free(eps_contexts);
#endif
    return 0;
}

