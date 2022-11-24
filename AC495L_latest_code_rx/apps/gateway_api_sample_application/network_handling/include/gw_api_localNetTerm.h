/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	gw_api_localNetTerm.h                               *
 *                                                                  *
 * Copyright (C) 2006 AudioCodes Ltd.								*
 *																	*
 * Modifications :													*
 *																	*
 * 30/10/06 - Coding started.										*
 *																	*
 ********************************************************************/
/*****************************************************************************
*                                                                            
* 		DESCRIPTION:  		                                                             
*                                                                            
******************************************************************************/

#ifndef	__GW_API_LOCAL_NET_TERM_h
#define	__GW_API_LOCAL_NET_TERM_h

#include "gw_api_localNetRx.h"
#include "gw_api_localNetTx.h"

#include "general_defs.h"


typedef struct
{
	int			chId; 
	int              	sFd; 
	ipAddrInfo_s	remoteNetTermInfo;
} channelInfo_s;

struct localNetParams
{
	int 	sFd;
	int 	phyId;
};


/****************************************************************************
*
*	Name:		iphone_createSocket
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
int gw_api_createRtpRxThread();
int gw_api_createRtcpRxThread();
int gw_api_createSocket(int localPort);
int gw_api_createSocketRtcp(int localPort);

void networking_rtpChannelInfoDB_reset(void);
void networking_rtcpChannelInfoDB_reset(void);
void networking_rtpSocket2AbsChDB_reset(void);
void networking_rtcpSocket2AbsChDB_reset(void);

/*RTP*/
int networking_rtpChannelAddrSet(char *addr, short port, int channel);
int networking_rtpChannelConnect(int channel, int sFd);
int networking_rtpChannelInfoDelete(int channel);

channelInfo_s *networking_rtpChannelInfoGet(int channel);
int networking_rtpAbsChannelBySfdGet(int sFd);
void networking_rtpSfdAdd(int sFd);
void networking_rtpSfdReset();
void networking_rtpSfdFree();

/*RTCP*/
int networking_rtcpChannelAddrSet(char *addr, short port, int channel);
int networking_rtcpChannelConnect(int channel, int sFd);
int networking_rtcpChannelInfoDelete(int channel);

channelInfo_s *networking_rtcpChannelInfoGet(int channel);
int networking_rtcpAbsChannelBySfdGet(int sFd);
void networking_rtcpSfdAdd(int sFd);
void networking_rtcpSfdReset();
void networking_rtcpSfdFree();

#endif /* __GW_API_LOCAL_NET_TERM_h */




