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


pthread_t  	startFxsTid;
int 	startFxsTerminate;


/*******************************************************************************
 * Function:    FxsDeviceInit
 * Description: init fxs device
 * Argument:    void
 * Return:      ACG_SUCCESS/ACG_FAILURE
 ******************************************************************************/
int FxsDeviceInit(void *arg)
{
	acgTFXSInitializationParameters	FXSInitializationParameters;

	FXSInitializationParameters.coefficients = arg;
		
	if(acgInitializeFXS(&FXSInitializationParameters) != ACG_SUCCESS )
	{
		printf("error - acgInitializeFXS()\r\n");
		return -1;
	}

	return ACG_SUCCESS;

} /* end of FxsDeviceInit() */

/****************************************************************************
*
*	Name:		startFxsLegerity
*----------------------------------------------------------------------------
*	Abstract:	
*----------------------------------------------------------------------------
*	Input:		none
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	success = 0
******************************************************************************/
void *FxsThreadFunction(void *arg)
{
	acgTEventInfo 		TelephonyEvent;
	acl_line_t		    	*pCurrentLine=NULL;
	voip_event_info_t 	voip_event_info;

	startFxsTerminate = 0;

	if(FxsDeviceInit(arg) == ACG_FAILURE)
		return NULL;

  	while(1)
	{  	/* forever */
		if(acgGetTelephonyEvent(&TelephonyEvent, ACG_POLLING_METHOD__BLOCKING) != ACG_SUCCESS )
		{
			printf("error - acgGetTelephonyEvent()\r\n");
		}
		else
		{
			if(TelephonyEvent.EventType == ACG_EVENT_TYPE__NO_EVENT)
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

                     	voip_event_info.phone_input_event = SLIC_PHONE_INPUT_EVENT;
                     	voip_event_info.info = &TelephonyEvent;
			phone_input_cb(&voip_event_info, pCurrentLine);		
		}

		/* If the application about to be terminated, exit thread gracefully */
		if(startFxsTerminate){
			pthread_exit(NULL);
		}
	} /* while */
} /* end of FxsThreadFunction() */

/****************************************************************************
*
*	Name:		startFxs
*----------------------------------------------------------------------------
*	Abstract:	
*----------------------------------------------------------------------------
*	Input:		void *arg
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	
******************************************************************************/
int startFxs(void *arg)
{
	pthread_t			tid;
	struct sched_param	sched;
	pthread_attr_t		attr;

	memset(&sched, 0, sizeof(sched));

	/* create the startFxsLegerity thread */
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

	if( (pthread_create( &tid, &attr, FxsThreadFunction, (void *)arg) != 0) )
	{
		acl_log(ACL_LOG_ERROR,"pthread_create - startFxsLegerity");
		exit(1);
	}
	startFxsTid = tid;
	

    	return 0;
}

