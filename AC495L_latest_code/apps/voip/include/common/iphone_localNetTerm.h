/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	iphone_localNetTerm.h                               *
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

#ifndef	__IPHONE_LOCAL_NET_TERM_h
#define	__IPHONE_LOCAL_NET_TERM_h

#include "acl_call_mngr.h"
#include "iphone_localNetRx.h"
#include "iphone_localNetTx.h"

typedef struct 
{
	int  port;
	char address[ADDRESS_STR_MAX_LEN];
} ipAddrInfo_s;

typedef struct
{
	int				 chId; 
	int              sFd; 
	voip_conn_mode_t rtpMode;
	ipAddrInfo_s	 remoteNetTermInfo;
} channelInfo_s;

struct localNetParams
{
	int sFd;
	int phyId;
};

typedef struct
{
	int   	        localPort;
	int   	        rtpSfd;
	int   	        rtcpSfd;
    	//acl_call_t    *pCall;
	ipAddrInfo_s	remoteNetTermInfo;
	int		remoteDtmfPayloadNumber;
	int		remoteCurrentCodecPayloadNumber;
} socketInfo;
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
int iphone_createRtpRxThread();
int iphone_createRtcpRxThread();
int iphone_createSocket(int localPort);
int iphone_createSocketRtcp(int localPort);

void networking_rtpChannelInfoDB_reset(void);
void networking_rtcpChannelInfoDB_reset(void);
void networking_rtpSocket2AbsChDB_reset(void);
void networking_rtcpSocket2AbsChDB_reset(void);

/*RTP*/
int networking_rtpChannelAddrSet(char *addr, short port, int channel);
int networking_rtpChannelConnect(int channel, int sFd);
int networking_rtpChannelInfoDelete(int channel);
int networking_rtpModeSet(int channel, voip_conn_mode_t rtpMode);

channelInfo_s *networking_rtpChannelInfoGet(int channel);
voip_conn_mode_t networking_rtpModeGet(int channel);
int networking_rtpAbsChannelBySfdGet(int sFd);
void networking_rtpSfdAdd(int sFd);
void networking_rtpSfdReset();
void networking_rtpSfdFree();
void networking_get_rtp_stun_res( unsigned char *buff,int *buff_len,int max_buff_size );
void networking_stun_rtp_set( int sFd,int stun_rtp );

/*RTCP*/
int networking_rtcpChannelAddrSet(char *addr, short port, int channel);
int networking_rtcpChannelConnect(int channel, int sFd);
int networking_rtcpChannelInfoDelete(int channel);
int networking_rtcpModeSet(int channel, voip_conn_mode_t rtpMode);

channelInfo_s *networking_rtcpChannelInfoGet(int channel);
voip_conn_mode_t networking_rtcpModeGet(int channel);
int networking_rtcpAbsChannelBySfdGet(int sFd);
void networking_rtcpSfdAdd(int sFd);
void networking_rtcpSfdReset();
void networking_rtcpSfdFree();
void networking_get_rtcp_stun_res( unsigned char *buff,int *buff_len,int max_buff_size );
void networking_stun_rtcp_set( int sFd,int stun_rtcp );

#endif /* __IPHONE_LOCAL_NET_TERM_h */




