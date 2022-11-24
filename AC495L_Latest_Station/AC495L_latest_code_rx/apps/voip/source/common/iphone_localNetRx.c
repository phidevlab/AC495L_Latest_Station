/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	iphone_localNetRx.c                                 *
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
  
#include "acl_log.h"
#include "acl_call_mngr.h"
#include "iphone_localNetTerm.h"
#include "voip_media_handler.h"
#include "acl_dsp_telephony_events.h"
#include "voip_utils.h"

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
static int         media_rx_message_len;

static int         stun_rtp_is_actived[FD_SETSIZE];
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
	socklen_t				sockAddrSize;

	acl_line_t				*pLine=NULL;

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
			acl_log(ACL_LOG_ERROR,"acl_UDPrxJob:: select failed");
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

				if((media_rx_message_len=recv(sFdDB[curFd], media_rx_message, ACL_MSG_SIZE, 0)) < 0) 
				{
					acl_log(ACL_LOG_ERROR,"acl_UDPrxJob:: recvfrom");
				}

				if( media_rx_message[0]  == 0x01 && media_rx_message[1]  == 0x01 )
				{
					if( stun_rtp_is_actived[curFd] )
					{
						receiverStunRes( sFdDB[curFd] );
						continue;
					}
				}

				chId = networking_rtpAbsChannelBySfdGet(sFdDB[curFd]);
				if(-1 != chId) 
				{
					if(VOIP_CONN_MODE_SEND != networking_rtpModeGet(chId))
					{   	
						pLine = (acl_line_t *)get_line(LineAndVoiceChannel2DspChannelConversionTable[chId].line);	
						if(!pLine)
							continue;

						if(pLine->active_call)
						{
							if (pLine->active_call->isT38)
							{
								protocol = ACG_PROTOCOL__FAX;  
							}
							else
							{
								protocol = ACG_PROTOCOL__RTP; 
							}
						}
						else
						{
							/* No need to decide whether T38 is active */
							/* This scenario is relevant when 3wc is activated on AC488xx chip */
						}
								
						msgSize = media_rx_message_len;  
						//printf("###############%d %d############## \n", chId, protocol);
						transmitter(media_rx_message, msgSize, chId, protocol);

					} /* end if(VOIP_CONN_MODE_SEND != networking_rtpModeGet(chId)) */	
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
	memset(stun_rtp_is_actived, 0, sizeof(stun_rtp_is_actived));

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

void networking_get_rtp_stun_res( unsigned char *buff,int *buff_len,int max_buff_size )
{
   if ( max_buff_size < media_rx_message_len )
   {
      *buff_len = 0;   
   }
   else
   {
    memcpy(buff, media_rx_message, media_rx_message_len);
    *buff_len = media_rx_message_len;
   }
}

void networking_stun_rtp_set( int sFd,int stun_rtp )
{
	int					curFd;

        for(curFd = 0; curFd < sFdDBRealSize; curFd++) 
	 {
		if(sFdDB[curFd] == sFd )
                stun_rtp_is_actived[curFd] = stun_rtp;
        }
}

/******************************************************************************/


/* end of iphone_localNetRx.c */ 
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/



