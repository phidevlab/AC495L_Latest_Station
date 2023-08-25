/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	gw_api_localNetTx.c                                 *
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
* 	DESCRIPTION:	This file holds the TX thread for a socket (phy termination)
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

#include "gw_api_localNetTerm.h"

/****************************************************************************
*
*	Name:		networking_rtpSocketSend
*----------------------------------------------------------------------------
*	Abstract:		sending RTP+T.38 packets to network
*----------------------------------------------------------------------------
*	Input:
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	case of error -1 else 0
******************************************************************************/
int networking_rtpSocketSend(char* buff, int len, int channel)
{
	int						rc;

	socklen_t				sockAddrSize;
	struct sockaddr_in		clientAddr;   /* one for active and one for conf */
	channelInfo_s			*channelInfo=NULL;

	printf("\networking_rtpSocketSend:: channel = %d\n", channel);

	channelInfo = networking_rtpChannelInfoGet(channel);


		printf("\networking_rtpSocketSend:: channelInfo = %d\n", channelInfo);
	if (channelInfo == NULL)
	{
		printf("\networking_rtpSocketSend: networking_channelInfoGet failed\n");
		return (-1);
	}

//	printf("\nremote address = %s   remote port = %d\n",
//				channelInfo->remoteNetTermInfo.address, channelInfo->remoteNetTermInfo.port);

	sockAddrSize = sizeof(struct sockaddr_in);

	bzero((char *)&(clientAddr), sockAddrSize);
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(channelInfo->remoteNetTermInfo.port);
	clientAddr.sin_addr.s_addr = inet_addr(channelInfo->remoteNetTermInfo.address);

	if ((0 == clientAddr.sin_port) || (0 == clientAddr.sin_addr.s_addr))
		return 0;

//	printf("\nsendto:: sFd = %d  len = %d\n", channelInfo->sFd, len);

	/* send packet to the network */
	rc = sendto(channelInfo->sFd,
               			buff,
				len,
				0,
				(struct sockaddr *)&(clientAddr),
				sockAddrSize);

	return rc;
} /* end of networking_rtpSocketSend() */

/******************************************************************************/


/* end of gw_api_netTx.c */
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

