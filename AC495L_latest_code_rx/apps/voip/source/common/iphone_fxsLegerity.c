/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	iphone_fxsLegerity.c                                *
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

extern int							slicFd;	/* SLIC file descriptor */

int 	startFxsLegerityTerminate;

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
void *startFxsLegerity(void *arg)
{
	acgTEventInfo 		TelephonyEvent;
	acl_line_t		    *pCurrentLine=NULL;
       voip_event_info_t voip_event_info;
	
	startFxsLegerityTerminate = 0;

	if(le88_init(arg) == ACG_FAILURE)
		return NULL;

  	while(1)
	{  	/* forever */
		if(acgGetTelephonyEvent(&TelephonyEvent, ACG_POLLING_METHOD__BLOCKING) != ACG_SUCCESS )
		{
			printf("error - acgGetTelephonyEvent()\r\n");
		}
		else
		{
			if(TelephonyEvent.Channel == -1)
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
			
			voip_event_info.info=(void*)&TelephonyEvent;
			voip_event_info.phone_input_event=SLIC_PHONE_INPUT_EVENT;

			phone_input_cb(&voip_event_info, pCurrentLine);		
		}

		/* If the application about to be terminated, exit thread gracefully */
		if(startFxsLegerityTerminate){
			pthread_exit(NULL);
		}
	} /* while */
} /* end of startFxsLegerity() */
/******************************************************************************/


 
/*******************************************************************************
 * Function:    le88_init
 * Description: init le88 device
 * Argument:    void
 * Return:      ACG_SUCCESS/ACG_FAILURE
 ******************************************************************************/
int le88_init(void *arg)
{
	acgTFXSInitializationParameters	FXSInitializationParameters;

	FXSInitializationParameters.coefficients = arg;
		
	if(acgInitializeFXS(&FXSInitializationParameters) != ACG_SUCCESS )
	{
		printf("error - acgInitializeFXS()\r\n");
		return -1;
	}

	return ACG_SUCCESS;

} /* end of le88_init() */
/******************************************************************************/

