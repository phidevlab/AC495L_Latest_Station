/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	iphone_localNetRtcpTerm.c                           *
 *                                                                  *
 * Copyright (C) 2006 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 12/02/06 - Coding started.                                       *
 *																	*
 ********************************************************************/
/******************************************************************************
*
* 	DESCRIPTION:	This file holds the routines for controling the socket hold by the local network termination
*
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <sched.h>

#include "acl_log.h"
#include "iphone_localNetTerm.h"
#include "iphone_localNetRtcpRx.h"


static channelInfo_s networkingRtcpChannelInfoDB[NUMBER_OF_DSP_CHANNELS];
static int			 socket2AbsChDB[FD_SETSIZE];

pthread_t  acl_RTCPrxJobTid;

/****************************************************************************
*
*	Name:		iphone_createRtcpRxThread
*----------------------------------------------------------------------------
*	Abstract:		launch acl_RTCPrxJob thread
*----------------------------------------------------------------------------
*	Input:
*----------------------------------------------------------------------------
*  	Output:
*----------------------------------------------------------------------------
*	Returns: 	case of error -1 else 0
******************************************************************************/
int iphone_createRtcpRxThread()
{
	pthread_t			tid;
	struct sched_param	sched;
	pthread_attr_t		attr;

	memset(&sched, 0, sizeof(sched));

	/* create the acl_RTCPrxJob thread */
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
  	sched.sched_priority = 70;
 	if( (pthread_attr_setschedparam(&attr, &sched) != 0) )
	{
		acl_log(ACL_LOG_ERROR,"pthread_attr_setschedparam");
		exit(1);
	}
 	if( (pthread_create( &tid, &attr, acl_RTCPrxJob, 0) != 0) )
	{
        printf("pthread_create - iphone_createRtcpRxThread[%s:%d]\n",__FUNCTION__,__LINE__);
	//	exit(1);
	}

	acl_RTCPrxJobTid = tid;

	return 0;

} /* end of iphone_createRtcpRxThread() */

int iphone_createSocketRtcp(int localPort)
{
	struct sockaddr_in	serverAddr;
	socklen_t			sockAddrSize;
	int 				sFd;

	/* build the socket structure */
	sockAddrSize = sizeof(struct sockaddr_in);
	bzero((char *)&serverAddr, sockAddrSize);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(localPort);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

   	/* get the socket */
	if((sFd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
    	acl_log(ACL_LOG_ERROR,"socket");
		return -1;
	}

 	/* bind the socket to the local ip and the port */
	if(bind(sFd,(struct sockaddr *)&serverAddr, sockAddrSize) < 0)
	{
    	acl_log(ACL_LOG_ERROR,"bind");
		close(sFd);
		return -1;
	}

	/*add the sFd to the readfds set*/
	networking_rtcpSfdAdd(sFd);

	return sFd;

} /* end of iphone_createSocketRtcp() */

void networking_rtcpChannelInfoDB_reset(void)
{
	int i;

	for(i = 0 ;i < NUMBER_OF_DSP_CHANNELS ;i++)
	{
		memset(networkingRtcpChannelInfoDB[i].remoteNetTermInfo.address, 0, ADDRESS_STR_MAX_LEN);
		networkingRtcpChannelInfoDB[i].remoteNetTermInfo.port = 0;
		networkingRtcpChannelInfoDB[i].chId = -1;
		networkingRtcpChannelInfoDB[i].sFd = -1;
		networkingRtcpChannelInfoDB[i].rtpMode = VOIP_CONN_MODE_NOT_SET;
	}

	networking_rtcpSfdReset();

} /* end of networking_rtpChannelInfoDB_reset() */

void networking_rtcpSocket2AbsChDB_reset(void)
{
	int i;

	for(i = 0 ;i < FD_SETSIZE ;i++)
		socket2AbsChDB[i] = -1;
} /* end of networking_socket2AbsChDB_reset() */

int networking_rtcpChannelAddrSet(char *addr, short port, int channel)
{
	channelInfo_s *channelInfo;
	if ((0 > channel) || (NUMBER_OF_DSP_CHANNELS <= channel))
	{
		acl_log(ACL_LOG_ERROR,"[%s,%d] channel must be between 0 to %d\r\n",__FUNCTION__,__LINE__, NUMBER_OF_DSP_CHANNELS -1);
		return -1;
	}
	channelInfo = &networkingRtcpChannelInfoDB[channel];

	/* set the channelInfo DB */
	/*IP addr*/
	memcpy(channelInfo->remoteNetTermInfo.address, addr, ADDRESS_STR_MAX_LEN);
	/*IP port*/
	channelInfo->remoteNetTermInfo.port = port + 1;

    return 1;
}

int networking_rtcpChannelConnect(int channel, int sFd)
{
	channelInfo_s *channelInfo;

	if ((0 > channel) || (NUMBER_OF_DSP_CHANNELS <= channel))
	{
		acl_log(ACL_LOG_ERROR,"[%s,%d] channel must be between 0 to %d\r\n",__FUNCTION__,__LINE__, NUMBER_OF_DSP_CHANNELS -1);
		return -1;
	}
	channelInfo = &networkingRtcpChannelInfoDB[channel];

	/* set the channelInfo DB */
	/*channel id*/
	channelInfo->chId = channel;
	/*sFd*/
	channelInfo->sFd = sFd;
	/*set the "other side" DB*/
	socket2AbsChDB[sFd] = channel;

	return 1;
}

channelInfo_s *networking_rtcpChannelInfoGet(int channel)
{
	if ((0 > channel) || (NUMBER_OF_DSP_CHANNELS <= channel))
	{
		acl_log(ACL_LOG_ERROR,"[%s,%d] channel must be between 0 to %d\r\n",__FUNCTION__,__LINE__, NUMBER_OF_DSP_CHANNELS -1);
		return NULL;
	}

   	return &networkingRtcpChannelInfoDB[channel];
}

int networking_rtcpChannelInfoDelete(int channel)
{
	channelInfo_s *channelInfo;

	if ((0 > channel) || (NUMBER_OF_DSP_CHANNELS <= channel))
	{
		acl_log(ACL_LOG_ERROR,"[%s,%d] channel must be between 0 to %d\r\n",__FUNCTION__,__LINE__, NUMBER_OF_DSP_CHANNELS -1);
		return ACL_FAILURE;
	}

	channelInfo = &networkingRtcpChannelInfoDB[channel];

	/*clear the DB*/
	/*IP addr*/
	memset (channelInfo->remoteNetTermInfo.address, 0, ADDRESS_STR_MAX_LEN);
	/*IP port*/
	channelInfo->remoteNetTermInfo.port = 0;
	/*channel id*/
	channelInfo->chId = -1;
	/*connectino status */
	channelInfo->rtpMode = VOIP_CONN_MODE_NOT_SET;

	/*do this first ! set the "other side" DB*/
	socket2AbsChDB[channelInfo->sFd] = -1;
	/*sFd*/
	channelInfo->sFd = -1;

	return ACL_SUCCESS;
}
int networking_rtcpModeSet(int channel, voip_conn_mode_t rtpMode)
{
	if ((0 > channel) || (NUMBER_OF_DSP_CHANNELS <= channel))
	{
		acl_log(ACL_LOG_ERROR,"[%s,%d] channel must be between 0 to %d\r\n",__FUNCTION__,__LINE__, NUMBER_OF_DSP_CHANNELS -1);
		return ACL_FAILURE;
	}
	networkingRtcpChannelInfoDB[channel].rtpMode = rtpMode;

	return ACL_SUCCESS;
}


voip_conn_mode_t networking_rtcpModeGet(int channel)
{
	if ((0 > channel) || (NUMBER_OF_DSP_CHANNELS <= channel))
	{
		acl_log(ACL_LOG_ERROR,"[%s,%d] channel must be between 0 to %d\r\n",__FUNCTION__,__LINE__, NUMBER_OF_DSP_CHANNELS -1);
		return ACL_FAILURE;
	}

	return networkingRtcpChannelInfoDB[channel].rtpMode;
}

int networking_rtcpAbsChannelBySfdGet(int sFd)
{
	return socket2AbsChDB[sFd];
}

/******************************************************************************/


/* end of iphone_localNetRtcpTerm.c */
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
