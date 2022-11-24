/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	iphone_localNetRtcpRx.c                             *
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

#include "iphone_localNetTerm.h"
#include "voip_media_handler.h"
#include "acl_call_mngr.h"
#include "acl_log.h"
   
static fd_set rtcpReceivingFds;
static int    rtcpMaxFd;
static int    sFdDB[FD_SETSIZE];
static int    sFdDBRealSize;

static char rtcp_rx_message[ACL_MSG_SIZE];
static int rtcp_rx_message_len;

static int         stun_rtcp_is_actived[FD_SETSIZE];

/****************************************************************************
*                                                                            
*	Name:		acl_RTCPrxJob			 			                                                     
*----------------------------------------------------------------------------                                                                            
*	Abstract:	the RTCP rx thread from the network to the dsp	 
*----------------------------------------------------------------------------                                                                            
*	Input:		 
*----------------------------------------------------------------------------                                                                            
*  	Output:		none			                                                  
*----------------------------------------------------------------------------                                                                            
*	Returns: 	case of error -1 else 0                           
******************************************************************************/   
void *acl_RTCPrxJob(void *arg)
{
	fd_set				readyFds; /* temp file descriptor list for select() */

	int					chId;
	int					msgSize;
	int					protocol;
	int					curFd;
	struct timeval      tv;
	struct timeval      *pTv = &tv;
	
	socklen_t			sockAddrSize;
	sockAddrSize = sizeof(struct sockaddr_in);
	
	/*see very important comment in iphone_localNetRx.c*/
//	tv.tv_sec = 5;
//    tv.tv_usec = 0;

	pTv = NULL;

	for(;;) 
	{
        readyFds = rtcpReceivingFds; /* copy it */
        if (select(rtcpMaxFd+1, &readyFds, NULL, NULL, pTv) == -1) 
		{
			acl_log(ACL_LOG_ERROR,"acl_RTCPrxJob:: select failed");
            		return NULL;
        	}

		/* run through the existing connections looking for data to read */
        for(curFd = 0; curFd < sFdDBRealSize; curFd++) 
		{
            if (FD_ISSET(sFdDB[curFd], &readyFds)) 
			{ /* we got one!! */
				if((rtcp_rx_message_len=recv(sFdDB[curFd], rtcp_rx_message, ACL_MSG_SIZE, 0)) < 0) 
				{
					acl_log(ACL_LOG_ERROR,"acl_RTCPrxJob:: recvfrom");
				}

                            if( rtcp_rx_message[0]  == 0x01 && rtcp_rx_message[1]  == 0x01 )
                            {
                                if( stun_rtcp_is_actived[curFd] )
                                {
                                    receiverStunRes( sFdDB[curFd] );
					continue;
				}
                            }

				chId = networking_rtcpAbsChannelBySfdGet(sFdDB[curFd]);
				if(chId != -1)
				{
					if(VOIP_CONN_MODE_SEND != networking_rtcpModeGet(chId)) 
					{   	
						protocol = ACG_PROTOCOL__RTCP;  
						msgSize = rtcp_rx_message_len; 

						transmitter(rtcp_rx_message, msgSize, chId, protocol);
					} /*if(VOIP_CONN_MODE_SEND != networking_rtcpModeGet(chId)) */
				} /* end if(chId != -1) */
			} /* if (FD_ISSET(curFd, &readyFds)) */  
		} /* for(curFd = 0; curFd <= maxFd; curFd++) */
	} /* end forever */
} /* end of acl_UDPrxJob() */

void networking_rtcpSfdAdd(int sFd)
{
	FD_SET(sFd, &rtcpReceivingFds);
	/*set the maximum fd to hold the newest fd*/
	if(sFd>rtcpMaxFd)
		rtcpMaxFd = sFd;

   	sFdDB[sFdDBRealSize] = sFd;
	sFdDBRealSize++;

}

void networking_rtcpSfdReset()
{
	/* Clear all entries from the set */
	FD_ZERO(&rtcpReceivingFds);
	
	memset(sFdDB, 0, sizeof(sFdDB));
	memset(stun_rtcp_is_actived, 0, sizeof(stun_rtcp_is_actived));

	rtcpMaxFd = 0;
	sFdDBRealSize = 0;
}

void networking_rtcpSfdFree()
{
	int					curFd;

        for(curFd = 0; curFd < sFdDBRealSize; curFd++) 
	{
		if(sFdDB[curFd])
			close(sFdDB[curFd]);
        }
}

void networking_get_rtcp_stun_res( unsigned char *buff,int *buff_len,int max_buff_size )
{

   if ( max_buff_size < rtcp_rx_message_len )
   {
      *buff_len = 0;   
   }
   else
   {
    memcpy(buff, rtcp_rx_message, rtcp_rx_message_len);
    *buff_len = rtcp_rx_message_len;
   }


    return ;
}

void networking_stun_rtcp_set( int sFd,int stun_rtcp )
{
	int					curFd;

        for(curFd = 0; curFd < sFdDBRealSize; curFd++) 
	 {
		if(sFdDB[curFd] == sFd )
                stun_rtcp_is_actived[curFd] = stun_rtcp;
        }
}


/******************************************************************************/


/* end of iphone_netRtcpRx.c */ 
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/



