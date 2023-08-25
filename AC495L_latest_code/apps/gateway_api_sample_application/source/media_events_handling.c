

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <sched.h>
#include <pthread.h>

#include "general_defs.h"
#include "acGatewayAPI.h"
#include "gw_api_localNetTerm.h"
#include "gw_api_localNetRtcpTx.h"


#define MEDIA_EVENTS_HANDLING_DEBUG_PRINT

#ifdef MEDIA_EVENTS_HANDLING_DEBUG_PRINT
#define DPRINTK(a1,a2...)	printf("\n" a1, ##a2)
#else
#define DPRINTK(a1,a2...)
#endif

static char	tx_message[ACL_MSG_SIZE];  /* CR Change */

static int 		hookState[NUMBER_OF_FXS_LINES];

static socketInfo_s  socketDB[ACG_NUMBER_OF_DSP_CHANNEL];
static int 		MediaBaseLocalPort = MEDIA_BASE_LOCAL_PORT;
static int 		RxJobsThreadsTerminate;


#if POLLING_METHOD__BLOCKING

static int 		FxoThreadTid;
static int 		FxsThreadTid;
static int 		ReceiverThreadTid;
static int 		ReceiverEventThreadTid;

static int 		FxoThreadTerminate;
static int 		FxsThreadTerminate;
static int 		ReceiverThreadTerminate;
static int 		ReceiverEventThreadTerminate;

#else

static int 		globalThreadThreadTid;
static int 		globalThreadTerminate;

pthread_cond_t 	mycond;
pthread_mutex_t 	mymutex;

#endif


static conection_mode_t	connectionMode[ACG_NUMBER_OF_DSP_CHANNEL];
static localModeChannelMapping_t	channelMapping[ACG_NUMBER_OF_DSP_CHANNEL];

#if POLLING_METHOD__BLOCKING
void *FxoThread(void *arg);
void *FxsThread(void *arg);
void *receiver(void* arg);
void *receiverEvent(void* arg);
#else
void *globalThread(void* arg);
#endif

void handleFXSEvents(acgTEventInfo *pTelephonyEvent);
void handleDSPEvents(acgTEventInfo *pTelephonyEvent);

void local_RtpRtcpSend(char* buff, int len, int channel, acgEProtocol protocol);
conection_mode_t getConnectionMode(int channel);



void initMediaEvents(void)
{
	memset(&hookState, 0, sizeof(hookState));

	memset(&channelMapping, -1, sizeof(channelMapping));

	RxJobsThreadsTerminate = 0;

#if POLLING_METHOD__BLOCKING
	FxoThreadTerminate = 0;
	FxsThreadTerminate = 0;
	ReceiverThreadTerminate = 0;
	ReceiverEventThreadTerminate = 0;

#else
	globalThreadTerminate = 0;

	if(pthread_mutex_init(&mymutex, NULL) < 0)
	{
		printf("error at pthread_mutex_init\r\n");
		return;
	}

	if(pthread_cond_init(&mycond, NULL) < 0)
	{
		printf("error at pthread_cond_init\r\n");
		return;
	}

#endif

}

void closeMediaEvents(void)
{
	memset(&hookState, 0, sizeof(hookState));

	RxJobsThreadsTerminate = 1;

#if POLLING_METHOD__BLOCKING
	FxoThreadTerminate = 1;
	FxsThreadTerminate = 1;
	ReceiverThreadTerminate = 1;
	ReceiverEventThreadTerminate = 1;

#else
	globalThreadTerminate = 1;
#endif


}

#if POLLING_METHOD__BLOCKING

int launchFxoThread(void)
{
	pthread_t				tid;
	struct sched_param		sched;
	pthread_attr_t			attr;

	memset(&sched, 0, sizeof(sched));

	if( (pthread_attr_init(&attr) != 0) )
	{
		DPRINTK("pthread_attr_init");
		exit(1);
	}
	if( (pthread_attr_setschedpolicy(&attr, SCHED_RR) != 0) )
	{
		DPRINTK("pthread_attr_setschedpolicy");
		exit(1);
	}
  	sched.sched_priority = 73;
 	if( (pthread_attr_setschedparam(&attr, &sched) != 0) )
	{
		DPRINTK("pthread_attr_setschedparam");
		exit(1);
	}
	if( (pthread_create( &tid, &attr, FxoThread, 0) != 0) )
	{
		DPRINTK("pthread_create - FxoThread");
		exit(1);
	}

	FxoThreadTid = tid;

	return 0;
}

int launchFxsThread(void)
{
	pthread_t				tid;
	struct sched_param		sched;
	pthread_attr_t			attr;

	memset(&sched, 0, sizeof(sched));

	if( (pthread_attr_init(&attr) != 0) )
	{
		DPRINTK("pthread_attr_init");
		exit(1);
	}
	if( (pthread_attr_setschedpolicy(&attr, SCHED_RR) != 0) )
	{
		DPRINTK("pthread_attr_setschedpolicy");
		exit(1);
	}
  	sched.sched_priority = 73;
 	if( (pthread_attr_setschedparam(&attr, &sched) != 0) )
	{
		DPRINTK("pthread_attr_setschedparam");
		exit(1);
	}
	if( (pthread_create( &tid, &attr, FxsThread, 0) != 0) )
	{
		DPRINTK("pthread_create - FxsThread");
		exit(1);
	}

	FxsThreadTid = tid;

	return 0;
}


void launchDSPThreads(void)
{
	pthread_t				tid;
	struct sched_param		sched;
	pthread_attr_t			attr;

	memset(&sched, 0, sizeof(sched));

	/* create the receiver thread */
	if( (pthread_attr_init(&attr) != 0) )
	{
		printf("\npthread_attr_init");
		exit(1);
	}
	if( (pthread_attr_setschedpolicy(&attr, SCHED_RR) != 0) )
	{
		printf("\npthread_attr_setschedpolicy");
		exit(1);
	}
  	sched.sched_priority = 76;
 	if( (pthread_attr_setschedparam(&attr, &sched) != 0) )
	{
		printf("\npthread_attr_setschedparam");
		exit(1);
	}

 	if( (pthread_create( &tid, &attr, receiver, 0 ) != 0) )
	{
		printf("\npthread_create - receiver");
		exit(1);
	}

	ReceiverThreadTid = tid;

	/* create the receiverEvent thread */
	if( (pthread_attr_init(&attr) != 0) )
	{
		printf("\npthread_attr_init");
		exit(1);
	}
	if( (pthread_attr_setschedpolicy(&attr, SCHED_RR) != 0) )
	{
		printf("\npthread_attr_setschedpolicy");
		exit(1);
	}
  	sched.sched_priority = 76;
 	if( (pthread_attr_setschedparam(&attr, &sched) != 0) )
	{
		printf("\npthread_attr_setschedparam");
		exit(1);
	}

 	if( (pthread_create( &tid, &attr, receiverEvent, 0 ) != 0) )
	{
		printf("\npthread_create - receiverEvent");
		exit(1);
	}

	ReceiverEventThreadTid = tid;

	return;

} /* end of start() */


void *FxsThread(void *arg)
{
	acgTEventInfo 		TelephonyEvent;

  	while(1)
	{
		/* forever */
		if(acgGetTelephonyEvent(&TelephonyEvent, ACG_POLLING_METHOD__BLOCKING) != ACG_SUCCESS )
		{
			printf("error - acgGetTelephonyEvent()\r\n");
		}
		else
		{
			if(TelephonyEvent.Channel == -1)
			{
				printf("Channel is -1\n");
				continue;
			}
			handleFXSEvents(&TelephonyEvent);
		}

		/* If the application about to be terminated, exit thread gracefully */
		if(FxsThreadTerminate){
			pthread_exit(NULL);
		}
	} /* while */
} /* end of startFxsLegerity() */


void *FxoThread(void *arg)
{
	acgTEventInfo 		TelephonyEvent;

  	while(1)
	{
		/* forever */
		if(acgGetDAAEvent(&TelephonyEvent, ACG_POLLING_METHOD__BLOCKING) != ACG_SUCCESS )
		{
			printf("error - acgGetDAAEvent()\r\n");
		}
		else
		{
			if(TelephonyEvent.Channel == -1)
			{
				printf("Channel is -1\n");
				continue;
			}
			//handleFXSEvents(&TelephonyEvent);
			printf("\nEvent: %d on FXO line %d\n", TelephonyEvent.EventType, TelephonyEvent.Channel);
		}

		/* If the application about to be terminated, exit thread gracefully */
		if(FxoThreadTerminate){
			pthread_exit(NULL);
		}
	} /* while */
} /* end of startFxsLegerity() */




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

	acgTGetMedia 				GetMedia;

	memset(&GetMedia, 0, sizeof(GetMedia));

	GetMedia.Buff = tx_message;

	 /* loop forever and polled data from the dsp */
	for(;;)
	{
		GetMedia.PollingMethod = ACG_POLLING_METHOD__BLOCKING;

		/* get a media packet from the dsp */
		status = acgGetMedia(&GetMedia);

		switch(status)
		{
			case ACG_SUCCESS:

				if((GetMedia.MediaProtocol == ACG_PROTOCOL__RTP)||(GetMedia.MediaProtocol == ACG_PROTOCOL__FAX))
				{
					if(CONN_MODE_LOCAL == getConnectionMode(GetMedia.Channel))
						local_RtpRtcpSend(tx_message, GetMedia.Len, GetMedia.Channel, GetMedia.MediaProtocol);
					else  /*CONN_MODE_REMOTE*/
						status = networking_rtpSocketSend(tx_message, GetMedia.Len, GetMedia.Channel);
				}
				else if(GetMedia.MediaProtocol == ACG_PROTOCOL__RTCP)
				{
					if(CONN_MODE_LOCAL == getConnectionMode(GetMedia.Channel))
						local_RtpRtcpSend(tx_message, GetMedia.Len, GetMedia.Channel, GetMedia.MediaProtocol);
					else  /*CONN_MODE_REMOTE*/
						status = networking_rtcpSocketSend(tx_message, GetMedia.Len, GetMedia.Channel);
				}
				else if(GetMedia.MediaProtocol == ACG_PROTOCOL__PLAYBACK_COMMAND)
				{
                    /*read data from file and sent to dsp, please ref dsp driver spec ,*/
				    int datasize;
				    Tac49xPlaybackPayload *p;
					p = (Tac49xPlaybackPayload *)tx_message;
					datasize = p->u.Command.u.Dsp.AvailableBufferSpace_msec_Msb*256 + p->u.Command.u.Dsp.AvailableBufferSpace_msec_Lsb;
					/*tasize =( tx_message[5] /10) *80 + tx_message[4]*256; //for A-Law ,80bytes/10 msec ,*/
					acgPlaybackDataGet(datasize, GetMedia.Channel, NULL);
				}
				else if(GetMedia.MediaProtocol == ACG_PROTOCOL__PLAYBACK_VOICE)
				{
				    printf("\nreceiver - cmd = %d\n",GetMedia.MediaProtocol);
				}
				break;

			case ACG_FAILURE:
				printf("\nreceiver - status = %d :: failure\n",status);

				break;

			case ACG_NO_PACKET:
				/*printf("ACG_NO_PACKET\n");*/

				break;

			default:
				printf("\nreceiver - status = %d unknown\n",status);
				break;
		}


		/* If the application about to be terminated, exit thread gracefully */
		if(ReceiverThreadTerminate){
			pthread_exit(NULL);
		}
	} /* end for(;;) */

	return NULL;

} /* end of receiver() */
/******************************************************************************/


void *receiverEvent(void* arg)
{
	int 	status;

	acgTEventInfo 		DspEvent;

	for(;;)
	{
		/* get a media packet from the dsp */
		status = acgGetEvent(&DspEvent, ACG_POLLING_METHOD__BLOCKING);

		switch(status)
		{
			case ACG_SUCCESS:
				handleDSPEvents(&DspEvent);
				break;

			case ACG_FAILURE:
				printf("\nreceiverEvent - status = %d :: failure\n",status);
				break;

			case ACG_NO_PACKET:
				/*printf("ACG_NO_PACKET\n");*/
				break;

			default:
				printf("\nreceiverEvent - status = %d unknown\n",status);
				break;
		}

		/* If the application about to be terminated, exit thread gracefully */
		if(ReceiverEventThreadTerminate){
			pthread_exit(NULL);
		}
	}

	return NULL;

} /* end of receiverEvent() */


#else	/* If not using blocking polling method */


int launchGlobalThread(void)
{
	pthread_t				tid;
	struct sched_param		sched;
	pthread_attr_t			attr;

	memset(&sched, 0, sizeof(sched));

	if( (pthread_attr_init(&attr) != 0) )
	{
		DPRINTK("pthread_attr_init");
		exit(1);
	}
	if( (pthread_attr_setschedpolicy(&attr, SCHED_RR) != 0) )
	{
		DPRINTK("pthread_attr_setschedpolicy");
		exit(1);
	}
  	sched.sched_priority = 77;
 	if( (pthread_attr_setschedparam(&attr, &sched) != 0) )
	{
		DPRINTK("pthread_attr_setschedparam");
		exit(1);
	}
	if( (pthread_create( &tid, &attr, globalThread, 0) != 0) )
	{
		DPRINTK("pthread_create - FxsThread");
		exit(1);
	}

	globalThreadThreadTid = tid;

	return 0;
}


void *globalThread(void* arg)
{
	int 				status;
	struct timespec 	ts;

	acgTEventInfo 		DspEvent;
	acgTEventInfo 		TelephonyEvent;
	acgTGetMedia 		GetMedia;

	ts.tv_sec = 0 ;
	ts.tv_nsec = 10000000;

	memset(&GetMedia, 0, sizeof(GetMedia));

	GetMedia.Buff = tx_message;

	for(;;)
	{
		/*** wait for 10 msec ***/
		pthread_mutex_lock (&mymutex);
		pthread_cond_timedwait (&mycond, &mymutex, &ts);
		pthread_mutex_unlock (&mymutex);

		GetMedia.PollingMethod = ACG_POLLING_METHOD__NON_BLOCKING;

		/* get a media packet from the dsp */
		status = acgGetMedia(&GetMedia);

		switch(status)
		{
			case ACG_SUCCESS:

				if((GetMedia.MediaProtocol == ACG_PROTOCOL__RTP)||(GetMedia.MediaProtocol == ACG_PROTOCOL__FAX))
				{
					if(CONN_MODE_LOCAL == getConnectionMode(GetMedia.Channel))
						local_RtpRtcpSend(tx_message, GetMedia.Len, GetMedia.Channel, GetMedia.MediaProtocol);
					else  /*CONN_MODE_REMOTE*/
			                	status = networking_rtpSocketSend(tx_message, GetMedia.Len, GetMedia.Channel);

				}
				else if(GetMedia.MediaProtocol == ACG_PROTOCOL__RTCP)
				{
					if(CONN_MODE_LOCAL == getConnectionMode(GetMedia.Channel))
						local_RtpRtcpSend(tx_message, GetMedia.Len, GetMedia.Channel, GetMedia.MediaProtocol);
			                else  /*CONN_MODE_REMOTE*/
						status = networking_rtcpSocketSend(tx_message, GetMedia.Len, GetMedia.Channel);
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
				else if(GetMedia.MediaProtocol == ACG_PROTOCOL__PLAYBACK_VOICE)
				{   /* = AC5X_PROTOCOL__VOICE_NET_PLAYBACK */
				    printf("\nglobalThread - cmd = %d \n",GetMedia.MediaProtocol);
				}

				break;

			case ACG_FAILURE:
				printf("\nreceiver - status = %d :: failure\n",status);

				break;

			case ACG_NO_PACKET:
//				printf("ACG_NO_PACKET\n");

				break;

			default:
				printf("\nreceiver - status = %d unknown\n",status);
				break;
		}


		/* get a media packet from the dsp */
		status = acgGetEvent(&DspEvent, ACG_POLLING_METHOD__NON_BLOCKING);

		switch(status)
		{
			case ACG_SUCCESS:

				handleDSPEvents(&DspEvent);

				break;

			case ACG_FAILURE:
				printf("\nreceiverEvent - status = %d :: failure\n",status);

				break;

			case ACG_NO_PACKET:
//				printf("ACG_NO_PACKET\n");

				break;

			default:
				printf("\nreceiverEvent - status = %d unknown\n",status);
				break;
		}


		/* forever */
		if(acgGetTelephonyEvent(&TelephonyEvent, ACG_POLLING_METHOD__NON_BLOCKING) != ACG_SUCCESS )
		{
			printf("error - acgGetTelephonyEvent()\r\n");
		}
		else
		{
			if(TelephonyEvent.Channel == -1)
				continue;

			handleFXSEvents(&TelephonyEvent);
		}


		/* If the application about to be terminated, exit thread gracefully */
		if(globalThreadTerminate){
			pthread_exit(NULL);
		}
	}

	return NULL;

} /* end of receiverEvent() */


#endif




void handleFXSEvents(acgTEventInfo *pTelephonyEvent)
{
	switch(pTelephonyEvent->EventType)
	{
		case ACG_EVENT_TYPE__ON_HOOK:
			printf("\nEvent: ACG_EVENT_TYPE__ON_HOOK on FXS line %d\n", pTelephonyEvent->Channel);

			hookState[pTelephonyEvent->Channel] = ON_HOOK;

			break;

		case ACG_EVENT_TYPE__OFF_HOOK:
			printf("\nEvent: ACG_EVENT_TYPE__OFF_HOOK on FXS line %d\n", pTelephonyEvent->Channel);

			hookState[pTelephonyEvent->Channel] = OFF_HOOK;

			break;

		case ACG_EVENT_TYPE__FLASH_HOOK:
			printf("\nEvent: ACG_EVENT_TYPE__FLASH_HOOK on FXS line %d\n", pTelephonyEvent->Channel);

			break;

		case ACG_EVENT_TYPE__RING_START:
			printf("\nEvent: ACG_EVENT_TYPE__RING_START detected on FXO line %d\n", pTelephonyEvent->Channel);
			break;

		case ACG_EVENT_TYPE__RING_END:
			printf("\nEvent: ACG_EVENT_TYPE__RING_END detected on FXO line %d\n", pTelephonyEvent->Channel);
			break;

		case ACG_EVENT_TYPE__POLARITY_REVERSAL:
			printf("\nEvent: ACG_EVENT_TYPE__POLARITY_REVERSAL detected on FXO line %d\n", pTelephonyEvent->Channel);
			break;

		case ACG_EVENT_TYPE__NO_EVENT:
			break;

		default:
			printf("\nUnrecognize EventType\n");
			break;
	}

}


void handleDSPEvents(acgTEventInfo *pTelephonyEvent)
{
	switch(pTelephonyEvent->EventType)
	{
		case ACG_EVENT_TYPE__DTMF_START:
			printf("\nEvent: ACG_EVENT_TYPE__DTMF_START on DSP channel %d detected from %s\n",
				pTelephonyEvent->Channel, (pTelephonyEvent->EventInfo.Tone.Direction ? "Network" : "Local TDM"));

			break;

		case ACG_EVENT_TYPE__DTMF_END:
			printf("\nEvent: ACG_EVENT_TYPE__DTMF_END on DSP channel %d detected from %s\n",
				pTelephonyEvent->Channel, (pTelephonyEvent->EventInfo.Tone.Direction ? "Network" : "Local TDM"));
			printf("ToneIndex=%d\n", pTelephonyEvent->EventInfo.Tone.ToneIndex);

			break;

		case ACG_EVENT_TYPE__FAX_START:
			acgStartFax(pTelephonyEvent->Channel, ACG_FAX_MODEM_TRANSPORT__RELAY);
			setT38(pTelephonyEvent->Channel);
			printf("\nEvent:ACG_EVENT_TYPE__FAX_START on DSP channel %d\n", pTelephonyEvent->Channel);
			printf("Initial Flag is %d\n",pTelephonyEvent->EventInfo.FaxModem.InitiatingFlag);
	      	printf("RIC is %d\n",pTelephonyEvent->EventInfo.FaxModem.ReasonIdentifierCode);

			break;

		case ACG_EVENT_TYPE__FAX_SIGNAL:
			printf("\nEvent:ACG_EVENT_TYPE__FAX_SIGNAL on DSP channel %d\n", pTelephonyEvent->Channel);
			printf("Initial Flag is %d\n",pTelephonyEvent->EventInfo.FaxModem.InitiatingFlag);
			printf("RIC is %d\n",pTelephonyEvent->EventInfo.FaxModem.ReasonIdentifierCode);
			break;

		case ACG_EVENT_TYPE__FAX_OR_MODEM_START:
			printf("\nEvent:ACG_EVENT_TYPE__FAX_OR_MODEM_START on DSP channel %d\n", pTelephonyEvent->Channel);
			printf("Initial Flag is %d\n",pTelephonyEvent->EventInfo.FaxModem.InitiatingFlag);
			printf("RIC is %d\n",pTelephonyEvent->EventInfo.FaxModem.ReasonIdentifierCode);

			break;

		case ACG_EVENT_TYPE__FAX_OR_MODEM_END:
			//need to stop fax
			acgStopFax(pTelephonyEvent->Channel, ACG_FAX_MODEM_TRANSPORT__RELAY);
			unsetT38(pTelephonyEvent->Channel);
			printf("\nEvent:ACG_EVENT_TYPE__FAX_OR_MODEM_END on DSP channel %d\n", pTelephonyEvent->Channel);
            printf("Initial Flag is %d\n",pTelephonyEvent->EventInfo.FaxModem.InitiatingFlag);
            printf("RIC is %d\n",pTelephonyEvent->EventInfo.FaxModem.ReasonIdentifierCode);

			break;

		case ACG_EVENT_TYPE__MODEM_START:
			printf("\nEvent:ACG_EVENT_TYPE__MODEM_START on DSP channel %d\n", pTelephonyEvent->Channel);
            printf("Initial Flag is %d\n",pTelephonyEvent->EventInfo.FaxModem.InitiatingFlag);
            printf("RIC is %d\n",pTelephonyEvent->EventInfo.FaxModem.ReasonIdentifierCode);
			break;

		case ACG_EVENT_TYPE__CALL_PROGRESS_TONE_START:
			printf("\nEvent:ACG_EVENT_TYPE__CALL_PROGRESS_TONE_START on DSP channel %d\n", pTelephonyEvent->Channel);
			printf("ToneIndex=%d\n", pTelephonyEvent->EventInfo.Tone.ToneIndex);
			break;

		case ACG_EVENT_TYPE__CALL_PROGRESS_TONE_END:
			printf("\nEvent:ACG_EVENT_TYPE__CALL_PROGRESS_TONE_END on DSP channel %d\n", pTelephonyEvent->Channel);
			printf("ToneIndex=%d\n", pTelephonyEvent->EventInfo.Tone.ToneIndex);
			break;

		case ACG_EVENT_TYPE__CALLER_ID_END:
			printf("\nEvent:ACG_EVENT_TYPE__CALLER_ID_END on DSP channel %d\n", pTelephonyEvent->Channel);
			break;

		case ACG_EVENT_TYPE__RTCP_INFO:                              /* RTCP information */
                        break;

                case ACG_EVENT_TYPE__RTCP_XR_INFO:                           /* RTCP-XR information */
			break;

		case ACG_EVENT_TYPE__NO_EVENT:
			break;

		default:
			printf("\nUnrecognize EventType\n");
			break;
	}

}


void transmitter(char* buff, int len, int channel, acgEProtocol protocol)
{
	acgTDecodeMedia			DecodeMedia;

	memset(&DecodeMedia, 0, sizeof(DecodeMedia));

	DecodeMedia.Buff = buff;
	DecodeMedia.Len = len;
	DecodeMedia.MediaProtocol = protocol;

	/* If the application about to be terminated, exit thread gracefully */
	if(RxJobsThreadsTerminate){
		pthread_exit(NULL);
	}
         printf("gw transmit \n");
	/* send to dsp */
	if(acgDecodeMedia(channel, &DecodeMedia) != ACG_SUCCESS )
	{
		printf("error - acgDecodeMedia()\r\n");
		return ;
	}

}


int get_hook_state(int line)
{
	return hookState[line];
}

void setConnectionMode(int channel, conection_mode_t mode)
{
	connectionMode[channel] = mode;
}

conection_mode_t getConnectionMode(int channel)
{
	return connectionMode[channel];
}


void setLocalConnectionChannelMapping(int firstChannel, int secondChannel)
{

	if (firstChannel == -1 || secondChannel == -1){
		memset(&channelMapping, -1, sizeof(channelMapping));
	}else{
	channelMapping[firstChannel].remoteChannel = secondChannel;
	channelMapping[secondChannel].remoteChannel = firstChannel;
}
}



int aclVoipRtpUtils_createSockets(void)
{
    int sfd;
    int curChannel;

    for (curChannel = 0; curChannel < ACG_NUMBER_OF_DSP_CHANNEL; curChannel++)
    {
        /*initialize sockets DB*/
        socketDB[curChannel].localPort = MediaBaseLocalPort;
        memset(socketDB[curChannel].remoteNetTermInfo.address, 0, ACL_ADDRESS_STR_MAX_LEN);
        socketDB[curChannel].remoteNetTermInfo.port = 0;

        sfd = gw_api_createSocket(MediaBaseLocalPort);
        if ( sfd == -1 )
            return -1;

        socketDB[curChannel].rtpSfd = sfd;

        sfd = gw_api_createSocketRtcp(MediaBaseLocalPort + 1);
        if ( sfd == -1 )
            return -1;

        socketDB[curChannel].rtcpSfd = sfd;
        MediaBaseLocalPort += 2;
	/*networking_rtpChannelConnect(curChannel, socketDB[curChannel].rtpSfd);*/
	/*networking_rtcpChannelConnect(curChannel, socketDB[curChannel].rtcpSfd);*/
/*
        DPRINTK("&socketDB[%d] = %p", curChannel, &socketDB[curChannel]);
        DPRINTK("socketDB[%d].localPort = %d", curChannel, socketDB[curChannel].localPort);
        DPRINTK("socketDB[%d].rtpSfd = %d", curChannel, socketDB[curChannel].rtpSfd);
        DPRINTK("socketDB[%d].rtcpSfd = %d", curChannel, socketDB[curChannel].rtcpSfd);
        DPRINTK("socketDB[%d].remoteNetTermInfo.address = %s", curChannel, socketDB[curChannel].remoteNetTermInfo.address);
        DPRINTK("socketDB[%d].remoteNetTermInfo.port = %d", curChannel, socketDB[curChannel].remoteNetTermInfo.port);
*/
    }
    return 0;
}


void aclVoipRtpUtils_ChannelAddrSet(char* addr, unsigned short port, int channel)
{
	networking_rtpChannelAddrSet(addr, port, channel);
	networking_rtcpChannelAddrSet(addr, port, channel);
}

void aclVoipRtpUtils_ChannelConnect(int channel)
{
	networking_rtpChannelConnect(channel, socketDB[channel].rtpSfd);
	networking_rtcpChannelConnect(channel, socketDB[channel].rtcpSfd);
}
/*
static void aclVoipRtpUtils_ChannelInfoDelete(int channel)
{
	networking_rtpChannelInfoDelete(channel);
	networking_rtcpChannelInfoDelete(channel);
}*/

void aclVoipRtpUtils_resetNetworkingDBs(void)
{
	networking_rtpChannelInfoDB_reset();
	networking_rtpSocket2AbsChDB_reset();
	networking_rtcpChannelInfoDB_reset();
	networking_rtcpSocket2AbsChDB_reset();
}

void aclVoipRtpUtils_freeNetworkingDbs(void)
{
	networking_rtpSfdFree();
	networking_rtcpSfdFree();
}


void local_RtpRtcpSend(char* buff, int len, int channel, acgEProtocol protocol)
{
	int remoteChannel = channelMapping[channel].remoteChannel;

	if(remoteChannel != -1)
	{
		/*printf("%d remote: %d\n", channel, remoteChannel);*/
		transmitter(buff, len, remoteChannel, protocol);
	}
}


/******************************************************************************/
