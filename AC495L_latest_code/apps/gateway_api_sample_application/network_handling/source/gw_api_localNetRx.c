/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	gw_api_localNetRx.c                                 *
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
* 	DESCRIPTION:	This file holds the RX thread for a socket (phy termination)				  		                                                             
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

/*#define  ACL_DEBUG*/

#ifdef ACL_DEBUG
#define ACL_LogDebug printf
#else
#define ACL_LogDebug
#endif

static fd_set rtpReceivingFds;
static int    rtpMaxFd;
static int    sFdDB[FD_SETSIZE];
static int    sFdDBRealSize;

static char	media_rx_message[ACL_MSG_SIZE];
int isT38[ACG_NUMBER_OF_DSP_CHANNEL] = {0};
//static int 	isT38 = 0;

void transmitter(char* buff, int len, int channel, acgEProtocol protocol);

void setT38(int Channel)
{
	isT38[Channel] = 1;
}

void unsetT38(int Channel)
{
	isT38[Channel] = 0;
}

int IST38(int Channel)
{
	return isT38[Channel];	
}

/****************************************************************************
*                                                                            
*	Name:		acl_UDPrxJob			 			                                                     
*----------------------------------------------------------------------------                                                                            
*	Abstract:	the rx thread from the network to the dsp	 
*----------------------------------------------------------------------------                                                                            
*	Input:		 
*----------------------------------------------------------------------------                                                                            
*  	Output:		none			                                                  
*----------------------------------------------------------------------------                                                                            
*	Returns: 	case of error -1 else 0                           
******************************************************************************/   
void *acl_UDPrxJob(void *arg)
{
	fd_set				readyFds; /* temp file descriptor list for select() */
	int					buf_len;
	socklen_t				sockAddrSize;

	int					chId;
	int					msgSize;
	int					protocol = ACG_PROTOCOL__RTP;
	int					curFd;
	struct timeval      tv;
	struct timeval      *pTv = &tv;
	
	sockAddrSize = sizeof(struct sockaddr_in);
	
	/*
		usually select would have been called with the fifth parameter as NULL.
		I had to do this nasty trick with the timeval struct becasue the acl_UDPrxJob
		comes up before the sFd's are added to the set and rtpMaxFd is set to the 
		correct value. This sequence caused rtpMaxFd == 0 and the select gets stuck 
		forever. With the timeval, the timer timeouts and let the select go. and on 
		the next loop rtpMaxFd is already set so we dont need the timer anymore.
	*/
//	tv.tv_sec = 5;
//   	tv.tv_usec = 0;

	pTv = NULL;

	for(;;) 
	{

	        readyFds = rtpReceivingFds; /* copy it */
	        if (select(rtpMaxFd+1, &readyFds, NULL, NULL, pTv) == -1) 
		{
			printf("\nacl_UDPrxJob:: select failed");
			return NULL;
	        }
			
	        /* 
			run through the existing connections looking for data to read.
			here we run through the actual set of existing sfds. sFdDB[] is set each
			time a new sfd is added and sFdDBRealSize is incremented correspondingly.
			we assume that all sfds are created upon initialization in createNewTerm
			and that there is no delete term action. this way sFdDB is set upon creation 
			and is never changed
		*/

	        for(curFd = 0; curFd < sFdDBRealSize; curFd++) 
		{
          		if (FD_ISSET(sFdDB[curFd], &readyFds)) 
			{ /* we got one!! */

				if((buf_len=recv(sFdDB[curFd], media_rx_message, ACL_MSG_SIZE, 0)) < 0) 
				{
					printf("\nacl_UDPrxJob:: recvfrom");
				}

				chId = networking_rtpAbsChannelBySfdGet(sFdDB[curFd]);
				if(-1 != chId) 
				{
					if (IST38(chId)){
						protocol = ACG_PROTOCOL__FAX;
					}
					else{
						protocol = ACG_PROTOCOL__RTP;
					}
					
					msgSize = buf_len;  

					transmitter(media_rx_message, msgSize, chId, protocol);

				} /* if(-1 != chId) */
			} /* if (FD_ISSET(curFd, &readyFds)) */  
		} /* for(curFd = 0; curFd <= maxFd; curFd++) */
	} /* end forever */
} /* end of acl_UDPrxJob() */

void networking_rtpSfdAdd(int sFd)
{
	FD_SET(sFd, &rtpReceivingFds);

	/*set the maximum fd to hold the newest fd*/
	if(sFd>rtpMaxFd)
		rtpMaxFd = sFd;

	sFdDB[sFdDBRealSize] = sFd;
	sFdDBRealSize++;
}

void networking_rtpSfdReset()
{
	/* Clear all entries from the set */
	FD_ZERO(&rtpReceivingFds);
	
	memset(sFdDB, 0, sizeof(sFdDB));

	rtpMaxFd = 0;
	sFdDBRealSize = 0;
}

void networking_rtpSfdFree()
{
	int					curFd;

        for(curFd = 0; curFd < sFdDBRealSize; curFd++) 
	{
		if(sFdDB[curFd])
			close(sFdDB[curFd]);
        }
}

/******************************************************************************/


/* end of gw_api_localNetRx.c */ 
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/



