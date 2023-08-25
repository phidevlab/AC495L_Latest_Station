/********************************************************************
 * AudioCodes improved application media handler                    *
 *                                                                  *
 * File Name:	voip_media_handler.c                                *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 26/02/08 - Coding started.                                       *
 *																	*
 ********************************************************************/
/******************************************************************************
*
* 	DESCRIPTION:		Handling DSP related media (RTP, RTCP, T.38) and DSP events
*
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>

//#include <linux/delay.h>

#include "voip_rtp_utils.h"
#include "acl_dsp_telephony_events.h"
#include "voip_media_handler.h"
#include "acGatewayAPI.h"
#include "iphone_localNetTerm.h"
#include "iphone_localNetRtcpTx.h"
#include "acl_log.h"
#include "voip_status_handler.h"
#include "voip_main.h"

#include <sched.h>
#include <errno.h>
#include "iphone_paging_pub.h"


/*#define AC_VOIP_MEDIA_HANDLER_DEBUG*/

#ifdef AC_VOIP_MEDIA_HANDLER_DEBUG
//#include <misc_funcs.h>
//#include <../mgt/lib/mgt_misc.h> /* ACL NB 070410 */
/*071016 - EA did this change during work on VI#57767 in order to syncronize our prntings with RV*/
//#define DBG_PRINT(a1,a2...)		IppLogMessage(RV_FALSE, "[%s:%4d] " a1,__FUNCTION__, __LINE__, ##a2)
#define DBG_PRINT(a1,a2...)		printf( "[%s:%4d] " a1,__FUNCTION__, __LINE__, ##a2)
#else
#define DBG_PRINT(a1,a2...)
#endif

/* ER08 - Added macro for error messages (not regular debug messages) that shouls go into the RV logs */
#define DBG_PRINT_ERR(a1,a2...)	IppLogMessage(RV_TRUE, "[%s:%4d] " a1,__FUNCTION__, __LINE__, ##a2)

#ifdef CONFIG_RG_ATA

extern int 		startFxsTerminate;
#ifdef FXO_SILABS
extern int 		startFxoTerminate;
#endif /*FXO_SILABS */

#endif /* CONFIG_RG_ATA */
static void thread_catch_term(int dummy);

pthread_t receiverTid;
pthread_t receiverEventTid;

static int receiverTerminate;
static int receiverEventTerminate;
static int acl_UDPrxJobTerminate;
static int acl_RTCPrxJobTerminate;

static char	tx_message[ACL_MSG_SIZE];  /* CR Change */


static void thread_catch_term(int dummy)
{
	/* Trigger all the below thread to terminate them selves gracefully */
	receiverTerminate = 1;
	receiverEventTerminate = 1;
	acl_UDPrxJobTerminate = 1;
	acl_RTCPrxJobTerminate = 1;

#ifdef CONFIG_RG_ATA
	startFxsTerminate = 1;

#ifdef FXO_SILABS
	startFxoTerminate = 1;
#endif /*FXO_SILABS */

#endif /* CONFIG_RG_ATA */

}

/****************************************************************************
*
*	Name:		transmitter
*----------------------------------------------------------------------------
*	Abstract:		pass the incomming packet to the DSP
*----------------------------------------------------------------------------
*	Input:		fd - the dsp descriptor
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	none
******************************************************************************/
void transmitter(char* buff, int len, int channel, acgEProtocol protocol)
{

	acgTDecodeMedia			DecodeMedia;

	memset(&DecodeMedia, 0, sizeof(DecodeMedia));

	DecodeMedia.Buff = buff;
	DecodeMedia.Len = len;
	DecodeMedia.MediaProtocol = protocol;


	/* If the application about to be terminated, exit thread gracefully */
	if(acl_UDPrxJobTerminate||acl_RTCPrxJobTerminate){
		pthread_exit(NULL);
	}
    printf("#############TRANSMITTE\n");
	/* send to dsp */
	if(acgDecodeMedia(channel, &DecodeMedia) != ACG_SUCCESS )
	{
		printf("error - acgDecodeMedia()\r\n");
		return ;
	}
	//printf("decode media\n");

} /* end of transmitter() */

/****************************************************************************
*
*	Name:		receiverStunRes
*----------------------------------------------------------------------------
*	Abstract:		receive stun response from network
*----------------------------------------------------------------------------
*	Input:		sFd - the socket descriptor
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	none
******************************************************************************/
void receiverStunRes( int sFd )
{
   int chId;
   acl_line_t       *pCurrentLine=NULL;
   voip_event_info_t voip_event_info;
   voip_misc_phone_event_info stunEvent;

   stunEvent.EventType = MISC_PHONE_EVENT_TYPE_STUN;
   stunEvent.EventInfo.Stun.Sfd = sFd;

   chId = networking_rtpAbsChannelBySfdGet(sFd);

   pCurrentLine = (acl_line_t *)get_line(LineAndVoiceChannel2DspChannelConversionTable[chId].line);

   voip_event_info.phone_input_event = MISC_PHONE_INPUT_EVENT;
   voip_event_info.info = &stunEvent;

   phone_input_cb(&voip_event_info, pCurrentLine);

}

/******************************************************************************/



/****************************************************************************
*
*	Name:		receiver
*----------------------------------------------------------------------------
*	Abstract:		polled data from the dsp and send it to the suitable socket
*----------------------------------------------------------------------------
*	Input:		fd - the dsp descriptor
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	none
******************************************************************************/
void *receiver(void* arg)
{

	int 						status;
    char call_type;
	acgTGetMedia 				GetMedia;

	memset(&GetMedia, 0, sizeof(GetMedia));
    int ret;
	GetMedia.Buff = tx_message;


	 /* loop forever and polled data from the dsp */
	for(;;)
	{
		GetMedia.PollingMethod = ACG_POLLING_METHOD__BLOCKING;

		/* get a media packet from the dsp */
		status = acgGetMedia(&GetMedia);
         pthread_mutex_lock(&lock_call_type);
        call_type=pvt_call.type ;
        pthread_mutex_unlock(&lock_call_type);
		switch(status)
		{
			case ACG_SUCCESS:

				if((GetMedia.MediaProtocol == ACG_PROTOCOL__RTP)||(GetMedia.MediaProtocol == ACG_PROTOCOL__FAX))
				{
                       // printf("***********case ACG_SUCCESS:\n");
                       // printf("************call_type:%c",call_type);
                        if(call_type==P2P/*(VOIP_CONN_MODE_RECV != networking_rtpModeGet(GetMedia.Channel))  && (VOIP_CONN_MODE_NOT_SET != networking_rtpModeGet(GetMedia.Channel))*/)
			            {
			              	status = networking_rtpSocketSend(tx_message,GetMedia.Len,GetMedia.Channel);
			              	printf("getting media form channel %d\n",GetMedia.Channel);
			            }
			        	else
			        	{

			               	//	printf("receiver (RTP) - in VOIP_CONN_MODE_RECV   channel = %d   mode = %d", GetMedia.Channel, networking_rtpModeGet(GetMedia.Channel));
			        	}
                        paging_send_packet(tx_message,GetMedia.Len,GetMedia.Channel);
				}
				else if(GetMedia.MediaProtocol == ACG_PROTOCOL__RTCP)
				{
			                if(VOIP_CONN_MODE_RECV != networking_rtcpModeGet(GetMedia.Channel))
			                {
			                    status = networking_rtcpSocketSend(tx_message,
			                                                       GetMedia.Len,
			                                                       GetMedia.Channel);
			                }
			                else
			                {
			                	DBG_PRINT("receiver (RTCP) - in VOIP_CONN_MODE_RECV");
			                }
                            //paging_send_packet(tx_message,GetMedia.Len,GetMedia.Channel);
				}
				else if(GetMedia.MediaProtocol == ACG_PROTOCOL__PLAYBACK_COMMAND)
				{   /* = AC5X_PROTOCOL__VOICE_TDM_PLAYBACK */
                    //read data from file and sent to dsp, please ref dsp driver spec ,
				    int datasize;
				    Tac49xPlaybackPayload *p;
					p = (Tac49xPlaybackPayload *)tx_message;
					datasize = p->u.Command.u.Dsp.AvailableBufferSpace_msec_Msb*256 + p->u.Command.u.Dsp.AvailableBufferSpace_msec_Lsb;
					//tasize =( tx_message[5] /10) *80 + tx_message[4]*256; //for A-Law ,80bytes/10 msec ,
					acgPlaybackDataGet(datasize, GetMedia.Channel, NULL);
				}

				break;

			case ACG_FAILURE:
				acl_log(ACL_LOG_ERROR,"receiverEvent - status = %d :: failure\n",status);

				break;

			case ACG_NO_PACKET:
//				printf("ACG_NO_PACKET\n");

				break;

			default:
				acl_log(ACL_LOG_ERROR,"receiverEvent - status = %d unknown\n",status);
				break;
		}


		/* If the application about to be terminated, exit thread gracefully */
		if(receiverTerminate){
			pthread_exit(NULL);
		}
    	} /* end for(;;) */

	return NULL;

}

void *receiverEvent(void* arg)
{
	int 	status;

	acgTEventInfo 			DspEvent;
	voip_event_info_t voip_event_info;

	acl_line_t		*pCurrentLine=NULL;

	for(;;)
	{
		/* get a media packet from the dsp */
		status = acgGetEvent(&DspEvent, ACG_POLLING_METHOD__BLOCKING);

		switch(status)
		{
			case ACG_SUCCESS:

				/* If line is locked, ignore it */
				if(getSpecificPortStatus(LineAndVoiceChannel2DspChannelConversionTable[DspEvent.Channel].line) == PORT_LOCK)
				{
					continue;
				}

				pCurrentLine = (acl_line_t *)get_line(LineAndVoiceChannel2DspChannelConversionTable[DspEvent.Channel].line);
				if(!pCurrentLine)
					continue;

				voip_event_info.info=(void*)&DspEvent;
				voip_event_info.phone_input_event=DSP_PHONE_INPUT_EVENT;
				phone_input_cb(&voip_event_info, pCurrentLine);

				break;

			case ACG_FAILURE:
				acl_log(ACL_LOG_ERROR,"receiverEvent - status = %d :: failure\n",status);

				break;

			case ACG_NO_PACKET:

				break;

			default:
				acl_log(ACL_LOG_ERROR,"receiverEvent - status = %d unknown\n",status);
				break;
		}


		/* If the application about to be terminated, exit thread gracefully */
		if(receiverEventTerminate){
			pthread_exit(NULL);
		}
	}

	return NULL;

} /* end of receiverEvent() */
/******************************************************************************/


/****************************************************************************
*
*	Name:		create_media_threads
*----------------------------------------------------------------------------
*	Abstract:		create the receiver and receiverEvent threads
*----------------------------------------------------------------------------
*	Input:		none
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	case of error -1 else 0
******************************************************************************/
void create_media_threads()
{
	pthread_t			tid;
	struct sched_param	sched;
	pthread_attr_t		attr;

	memset(&sched, 0, sizeof(sched));

	struct sigaction act;
	int signum = SIGUSR1;

	receiverTerminate = 0;
  	receiverEventTerminate = 0;
  	acl_UDPrxJobTerminate = 0;
	acl_RTCPrxJobTerminate = 0;


	/* Set up signal handler: */
	sigfillset(&act.sa_mask);
	act.sa_flags = 0;
	act.sa_handler = thread_catch_term;
	sigaction(signum, &act, NULL);


	/* create the receiver thread */
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
  	sched.sched_priority = 76;/*68;	*/
 	if( (pthread_attr_setschedparam(&attr, &sched) != 0) )
	{
		acl_log(ACL_LOG_ERROR,"pthread_attr_setschedparam");
		exit(1);
	}

 	if( (pthread_create( &tid, &attr, receiver, 0 ) != 0) )
	{
		printf("pthread_create - receiver[%s:%d]\n",__FUNCTION__,__LINE__);
		//exit(1);
	}

	receiverTid = tid;

	/* create the receiverEvent thread */
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
  	sched.sched_priority = 76;/*73;	*/
 	if( (pthread_attr_setschedparam(&attr, &sched) != 0) )
	{
		printf("pthread_attr_setschedparam[%s:%d]\n",__FUNCTION__,__LINE__);
		//exit(1);
	}

 	if( (pthread_create( &tid, &attr, receiverEvent, 0 ) != 0) )
	{
		printf("pthread_create - receiverEvent[%s:%d]\n",__FUNCTION__,__LINE__);
		//exit(1);
	}

	receiverEventTid = tid;

	return;

} /* end of start() */
/******************************************************************************/




/* end of voip_media_handler.c */
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/


