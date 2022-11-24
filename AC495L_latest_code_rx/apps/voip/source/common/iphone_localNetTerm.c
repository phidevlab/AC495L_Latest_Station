/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	iphone_localNetTerm.c                               *
 *                                                                  *
 * Copyright (C) 2005 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 25/09/05 - Coding started.                                       *
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

#include "iphone_localNetTerm.h"

#include "acl_log.h"

static channelInfo_s networkingRtpChannelInfoDB[NUMBER_OF_DSP_CHANNELS];
static int			 socket2AbsChDB[FD_SETSIZE];

pthread_t  acl_UDPrxJobTid;

/****************************************************************************
*
*	Name:		iphone_createRtpRxThread
*----------------------------------------------------------------------------
*	Abstract:	create a socket, bind it and add the tx and rx routines
*----------------------------------------------------------------------------
*	Input:     	localPort - socket port
*				phyId - physical termination id that is connected to this socket
*----------------------------------------------------------------------------
*  	Output:		
*----------------------------------------------------------------------------
*	Returns: 	case of error -1 else 0
******************************************************************************/
int iphone_createRtpRxThread()
{
	pthread_t			tid;
	struct sched_param	sched;
	pthread_attr_t		attr;

	memset(&sched, 0, sizeof(sched));

	/* create the acl_UDPrxJob thread */
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
  	sched.sched_priority = 76;	
 	if( (pthread_attr_setschedparam(&attr, &sched) != 0) )
	{
		acl_log(ACL_LOG_ERROR,"pthread_attr_setschedparam");
		exit(1);
	}
	if( (pthread_create(&tid, &attr, acl_UDPrxJob, 0) != 0) )
	{
		acl_log(ACL_LOG_ERROR,"pthread_create - acl_UDPrxJob");
		exit(1);
	}

	acl_UDPrxJobTid = tid;
		
	return 0;	

} /* end of iphone_createRxThread() */

int iphone_createSocket(int localPort)
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
	networking_rtpSfdAdd(sFd);
	
	return sFd;	

} /* end of iphone_createSocket() */

void networking_rtpChannelInfoDB_reset(void)
{
	int i;

	for(i = 0 ;i < NUMBER_OF_DSP_CHANNELS ;i++)
	{
		memset(networkingRtpChannelInfoDB[i].remoteNetTermInfo.address, 0, ADDRESS_STR_MAX_LEN);
		networkingRtpChannelInfoDB[i].remoteNetTermInfo.port = 0;
		networkingRtpChannelInfoDB[i].chId = -1;
		networkingRtpChannelInfoDB[i].sFd = -1;
		networkingRtpChannelInfoDB[i].rtpMode = VOIP_CONN_MODE_NOT_SET;
	}

	networking_rtpSfdReset();
	
} /* end of networking_rtpChannelInfoDB_reset() */

void networking_rtpSocket2AbsChDB_reset(void)
{
	int i;

	for(i = 0 ;i < FD_SETSIZE ;i++)
		socket2AbsChDB[i] = -1;
} /* end of networking_socket2AbsChDB_reset() */

int networking_rtpChannelAddrSet(char *addr, short port, int channel)
{
	channelInfo_s *channelInfo;
	
	if ((0 > channel) || (NUMBER_OF_DSP_CHANNELS <= channel))
	{
		acl_log(ACL_LOG_ERROR,"channel must be between 0 to %d\r\n", NUMBER_OF_DSP_CHANNELS -1);
		return ACL_FAILURE;
	}
	channelInfo = &networkingRtpChannelInfoDB[channel];

	/* set the channelInfo DB */
	/*IP addr*/
	memcpy(channelInfo->remoteNetTermInfo.address, addr, ADDRESS_STR_MAX_LEN);
	/*IP port*/
	channelInfo->remoteNetTermInfo.port = port;

	return ACL_SUCCESS;
}

int networking_rtpChannelConnect(int channel, int sFd)
{
	channelInfo_s *channelInfo;
	
	if ((0 > channel) || (NUMBER_OF_DSP_CHANNELS <= channel))
	{
		acl_log(ACL_LOG_ERROR,"channel must be between 0 to %d\r\n", NUMBER_OF_DSP_CHANNELS -1);
		return ACL_FAILURE;
	}
	channelInfo = &networkingRtpChannelInfoDB[channel];

	/* set the channelInfo DB */
	/*channel id*/
	channelInfo->chId = channel;
	/*sFd*/
	channelInfo->sFd = sFd;
	/*set the "other side" DB*/
	socket2AbsChDB[sFd] = channel;

	return ACL_SUCCESS;
}

channelInfo_s *networking_rtpChannelInfoGet(int channel)
{
	if ((0 > channel) || (NUMBER_OF_DSP_CHANNELS <= channel))
	{
		acl_log(ACL_LOG_ERROR,"channel must be between 0 to %d\r\n", NUMBER_OF_DSP_CHANNELS -1);
		return NULL;
	}
    
	return &networkingRtpChannelInfoDB[channel];
}

int networking_rtpChannelInfoDelete(int channel)
{
	channelInfo_s *channelInfo;

	if ((0 > channel) || (NUMBER_OF_DSP_CHANNELS <= channel))
	{
		acl_log(ACL_LOG_ERROR,"channel must be between 0 to %d\r\n", NUMBER_OF_DSP_CHANNELS -1);
		return ACL_FAILURE;
	}

	channelInfo = &networkingRtpChannelInfoDB[channel];

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

int networking_rtpModeSet(int channel, voip_conn_mode_t rtpMode)
{
	if ((0 > channel) || (NUMBER_OF_DSP_CHANNELS <= channel))
	{
		acl_log(ACL_LOG_ERROR,"channel must be between 0 to %d\r\n", NUMBER_OF_DSP_CHANNELS -1);
		return ACL_FAILURE;
	}
	networkingRtpChannelInfoDB[channel].rtpMode = rtpMode;

	return ACL_SUCCESS;
}

voip_conn_mode_t networking_rtpModeGet(int channel)
{
	if ((0 > channel) || (NUMBER_OF_DSP_CHANNELS  <= channel))
	{
		acl_log(ACL_LOG_ERROR,"channel must be between 0 to %d\r\n", NUMBER_OF_DSP_CHANNELS -1);
		return ACL_FAILURE;
	}

	return networkingRtpChannelInfoDB[channel].rtpMode;
}

int networking_rtpAbsChannelBySfdGet(int sFd)
{
	return socket2AbsChDB[sFd];
}

/******************************************************************************/


/* end of iphone_localNetTerm.c */
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
