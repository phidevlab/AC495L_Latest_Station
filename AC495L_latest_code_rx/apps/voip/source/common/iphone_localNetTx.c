/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	iphone_localNetTx.c                                 *
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

#include "iphone_localNetTerm.h"

#include "acl_log.h"

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

	channelInfo = networking_rtpChannelInfoGet(channel);
	if (channelInfo == NULL)
	{
		acl_log(ACL_LOG_ERROR,"networking_UdpSocketSend: networking_channelInfoGet failed\n");
		return ACL_FAILURE;
	}
	
	sockAddrSize = sizeof(struct sockaddr_in);

	bzero((char *)&(clientAddr), sockAddrSize);
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(channelInfo->remoteNetTermInfo.port);
	clientAddr.sin_addr.s_addr = inet_addr(channelInfo->remoteNetTermInfo.address);

	if ((0 == clientAddr.sin_port) || (0 == clientAddr.sin_addr.s_addr))
		return 0;

	/* send packet to the network */
	rc = sendto(channelInfo->sFd, 
               			buff,
				len,
				0,
				(struct sockaddr *)&(clientAddr),
				sockAddrSize);

	return rc;
} /* end of networking_rtpSocketSend() */

int networking_transmitStunReq(char* buff,
                                                              int len,
                                                              int socketSfd,
                                                              char *remoteAddress,
                                                              int remotePort)
{
	int						rc;

	socklen_t				sockAddrSize;
	struct sockaddr_in		clientAddr;   /* one for active and one for conf */
	
	sockAddrSize = sizeof(struct sockaddr_in);

	bzero((char *)&(clientAddr), sockAddrSize);
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(remotePort);
	clientAddr.sin_addr.s_addr = inet_addr(remoteAddress);

	if ((0 == clientAddr.sin_port) || (0 == clientAddr.sin_addr.s_addr))
		return 0;

	/* send packet to the network */
	rc = sendto(socketSfd, 
               			buff,
				len,
				0,
				(struct sockaddr *)&(clientAddr),
				sockAddrSize);

	return rc;
}

/******************************************************************************/


/* end of iphone_netTx.c */ 
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

