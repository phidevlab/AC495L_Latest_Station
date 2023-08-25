/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	gw_api_localNetRtcpTerm.c                           *
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

#include "gw_api_localNetTerm.h"
#include "gw_api_localNetRtcpRx.h"


static channelInfo_s networkingRtcpChannelInfoDB[ACG_NUMBER_OF_DSP_CHANNEL];
static int			 socket2AbsChDB[FD_SETSIZE];

pthread_t  acl_RTCPrxJobTid;

/****************************************************************************
*
*	Name:		gw_api_createRtcpRxThread
*----------------------------------------------------------------------------
*	Abstract:		launch acl_RTCPrxJob thread
*----------------------------------------------------------------------------
*	Input:
*----------------------------------------------------------------------------
*  	Output:
*----------------------------------------------------------------------------
*	Returns: 	case of error -1 else 0
******************************************************************************/
int gw_api_createRtcpRxThread(void)
{
	pthread_t			tid;
	struct sched_param	sched;
	pthread_attr_t		attr;

	memset(&sched, 0, sizeof(sched));

	/* create the acl_RTCPrxJob thread */
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
  	sched.sched_priority = 70;
 	if( (pthread_attr_setschedparam(&attr, &sched) != 0) )
	{
		printf("\npthread_attr_setschedparam");
		exit(1);
	}
 	if( (pthread_create( &tid, &attr, acl_RTCPrxJob, 0) != 0) )
	{
		printf("\npthread_create - acl_RTCPrxJob");
		exit(1);
	}

	acl_RTCPrxJobTid = tid;

	return 0;

} /* end of gw_api_createRtcpRxThread() */

int gw_api_createSocketRtcp(int localPort)
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
    		printf("\nsocket");
		return -1;
	}

 	/* bind the socket to the local ip and the port */
	if(bind(sFd,(struct sockaddr *)&serverAddr, sockAddrSize) < 0)
	{
    		printf("\nbind");
		close(sFd);
		return -1;
	}

	/*add the sFd to the readfds set*/
	networking_rtcpSfdAdd(sFd);

	return sFd;

} /* end of gw_api_createSocketRtcp() */

void networking_rtcpChannelInfoDB_reset(void)
{
	int i;

	for(i = 0 ;i < ACG_NUMBER_OF_DSP_CHANNEL ;i++)
	{
		memset(networkingRtcpChannelInfoDB[i].remoteNetTermInfo.address, 0, ACL_ADDRESS_STR_MAX_LEN);
		networkingRtcpChannelInfoDB[i].remoteNetTermInfo.port = 0;
		networkingRtcpChannelInfoDB[i].chId = -1;
		networkingRtcpChannelInfoDB[i].sFd = -1;
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

	if ((0 > channel) || (ACG_NUMBER_OF_DSP_CHANNEL <= channel))
	{
		printf("\nchannel must be between 0 to %d\r\n", ACG_NUMBER_OF_DSP_CHANNEL -1);
		return -1;
	}
	channelInfo = &networkingRtcpChannelInfoDB[channel];

	/* set the channelInfo DB */
	/*IP addr*/
	memcpy(channelInfo->remoteNetTermInfo.address, addr, ACL_ADDRESS_STR_MAX_LEN);
	/*IP port*/
	channelInfo->remoteNetTermInfo.port = port + 1;

    return 1;
}

int networking_rtcpChannelConnect(int channel, int sFd)
{
	channelInfo_s *channelInfo;

	if ((0 > channel) || (ACG_NUMBER_OF_DSP_CHANNEL <= channel))
	{
		printf("\nchannel must be between 0 to %d\r\n", ACG_NUMBER_OF_DSP_CHANNEL -1);
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
	if ((0 > channel) || (ACG_NUMBER_OF_DSP_CHANNEL <= channel))
	{
		printf("\nchannel must be between 0 to %d\r\n", ACG_NUMBER_OF_DSP_CHANNEL -1);
		return NULL;
	}

   	return &networkingRtcpChannelInfoDB[channel];
}

int networking_rtcpChannelInfoDelete(int channel)
{
	channelInfo_s *channelInfo;

	if ((0 > channel) || (ACG_NUMBER_OF_DSP_CHANNEL <= channel))
	{
		printf("\nchannel must be between 0 to %d\r\n", ACG_NUMBER_OF_DSP_CHANNEL -1);
		return (-1);
	}

	channelInfo = &networkingRtcpChannelInfoDB[channel];

	/*clear the DB*/
	/*IP addr*/
	memset (channelInfo->remoteNetTermInfo.address, 0, ACL_ADDRESS_STR_MAX_LEN);
	/*IP port*/
	channelInfo->remoteNetTermInfo.port = 0;
	/*channel id*/
	channelInfo->chId = -1;

	/*do this first ! set the "other side" DB*/
	socket2AbsChDB[channelInfo->sFd] = -1;
	/*sFd*/
	channelInfo->sFd = -1;

	return 0;
}

int networking_rtcpAbsChannelBySfdGet(int sFd)
{
	return socket2AbsChDB[sFd];
}

/******************************************************************************/


/* end of gw_api_localNetRtcpTerm.c */
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
