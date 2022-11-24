/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	iphone_fxs.c                                        *
 *                                                                  *
 * Copyright (C) 2006 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 27/04/06 - Coding started.                                       *
 *																	*
 ********************************************************************/

/*****************************************************************************
*                                                                            
* 		DESCRIPTION: 		 		                                                             
*                                                                            
******************************************************************************/
#include <sys/stat.h>  
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h> 

#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <sys/ipc.h>
#include <sys/msg.h>

#include <signal.h>

#include <sys/types.h>
#include <sys/stat.h>  
#include <semaphore.h>

#include "acl_dsp_telephony_events.h"
#include "voip_status_handler.h"

#include "acl_log.h"

#include "acGatewayAPI.h"
#include "iphone_fxs.h"
#include <sched.h> 


pthread_t  	startFxoTid;
int startFxoTerminate;


/*******************************************************************************
 * Function:    FxoDeviceInit
 * Description: init fxo device
 * Argument:    void
 * Return:      ACG_SUCCESS/ACG_FAILURE
 ******************************************************************************/
int FxoDeviceInit(void *arg)
{

	if(acgInitializeDAA() != ACG_SUCCESS )
	{
		printf("error - acgInitializeFXO()\r\n");
		return ACG_FAILURE;
	}

	return ACG_SUCCESS;

} /* end of FxoDeviceInit() */

/****************************************************************************
*
*	Name:		FxoThreadFunction
*----------------------------------------------------------------------------
*	Abstract:	
*----------------------------------------------------------------------------
*	Input:		none
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	success = 0
******************************************************************************/
void *FxoThreadFunction(void *arg)
{
	acgTEventInfo 		TelephonyEvent;
	acl_line_t		    	*pCurrentLine=NULL;
	voip_event_info_t 	voip_event_info;

	
	startFxoTerminate = 0;

	if(FxoDeviceInit(arg) == ACG_FAILURE)
		return NULL;


  	while(1)
	{  	/* forever */
		if(acgGetDAAEvent(&TelephonyEvent, ACG_POLLING_METHOD__BLOCKING) != ACG_SUCCESS )
		{
			printf("error - acgGetDAAEvent()\r\n");
		}
		else
		{   	
				if((TelephonyEvent.Channel == -1) || (TelephonyEvent.EventType == ACG_EVENT_TYPE__NO_EVENT))
				continue;

			/* If line is locked, ignore it */
			if(getSpecificPortStatus(TelephonyEvent.Channel) == PORT_LOCK)
			{
				continue;
			}

			/* Channel is actually the line - in SLIC point of view */
			pCurrentLine = get_line(TelephonyEvent.Channel);
			if(!pCurrentLine)
				continue;
			
                     	voip_event_info.phone_input_event = DAA_PHONE_INPUT_EVENT;
                     	voip_event_info.info = &TelephonyEvent;
			phone_input_cb(&voip_event_info, pCurrentLine);		

		}

		/* If the application about to be terminated, exit thread gracefully */
		if(startFxoTerminate){
			pthread_exit(NULL);
		}
	} /* while */
} /* end of FxoThreadFunction() */

/****************************************************************************
*
*	Name:		startFxo
*----------------------------------------------------------------------------
*	Abstract:	
*----------------------------------------------------------------------------
*	Input:		void *arg
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	
******************************************************************************/
int startFxo(void *arg)
{
	pthread_t			tid;
	struct sched_param	sched;
	pthread_attr_t		attr;

	memset(&sched, 0, sizeof(sched));

	/* create the startFxoSilabs thread */
	if( (pthread_attr_init(&attr) != 0) )
	{
		acl_log(ACL_LOG_ERROR,"pthread_attr_init");
		exit(1);
	}
	if( (pthread_attr_setschedpolicy(&attr, SCHED_RR) != 0) )
	{
		acl_log(ACL_LOG_ERROR,"pthread_attr_setschedpolicy");
		exit(1);
	}	
  	sched.sched_priority = 73;	
 	if( (pthread_attr_setschedparam(&attr, &sched) != 0) )
	{
		acl_log(ACL_LOG_ERROR,"pthread_attr_setschedparam");
		exit(1);
	}


 	if( (pthread_create( &tid, &attr, FxoThreadFunction, (void *)arg) != 0) )
	{
		acl_log(ACL_LOG_ERROR,"pthread_create - startFxsSilabs");
		exit(1);
	}
	startFxoTid = tid;

    	return 0;
}


