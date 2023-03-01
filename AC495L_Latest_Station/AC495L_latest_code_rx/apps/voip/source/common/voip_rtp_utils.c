/********************************************************************
 * AudioCodes improved application rtp utils                        *
 *                                                                  *
 * File Name:	voip_rtp_utils.c                                    *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 26/02/08 - Coding started.                                       *
 *																	*
 ********************************************************************/

/*---------------------------------------------------------------------------------------*/
/*                                          INCLUDES                                     */
/*---------------------------------------------------------------------------------------*/

#include "voip_rtp_utils.h"

#include "acl_line_connection.h"
#include "acl_voip_conf.h"
#include "iphone_localNetTerm.h"


/*---------------------------------------------------------------------------------------*/
/*                                     DEFINES                                           */
/*---------------------------------------------------------------------------------------*/
#define MAX_RETRIES         10
#define DTMF_AUTO_CANCEL    500

/*#define AC_VOIP_RTP_UTILS_DEBUG*/

#ifdef CONFIG_RG_VOIP_RV_SIP
#include <ipplog.h>
/* ER08 - Added macro for error messages (not regular debug messages) that shouls go into the RV logs */
#define DBG_PRINT_ERR(a1,a2...)	IppLogMessage(RV_TRUE, "[%s:%4d] " a1,__FUNCTION__, __LINE__, ##a2)
#else /* CONFIG_RG_VOIP_RV_SIP */
#define DBG_PRINT_ERR(a1,a2...)	printf("[%s:%4d] ERROR: " a1,__FUNCTION__, __LINE__, ##a2)
#endif /* CONFIG_RG_VOIP_RV_SIP */


#ifdef AC_VOIP_RTP_UTILS_DEBUG
#ifdef CONFIG_RG_VOIP_RV_SIP
/*071016 - EA did this change during work on VI#57767 in order to syncronize our prntings with RV*/
#define DBG_PRINT(a1,a2...)	IppLogMessage(RV_FALSE, "[%s:%4d] " a1,__FUNCTION__, __LINE__, ##a2)
#else /* CONFIG_RG_VOIP_RV_SIP */
#define DBG_PRINT(a1,a2...)	printf("[%s:%4d] " a1,__FUNCTION__, __LINE__, ##a2)
#endif /* CONFIG_RG_VOIP_RV_SIP */
#else /* AC_VOIP_RTP_UTILS_DEBUG */
#define DBG_PRINT(a1,a2...)
#endif /* AC_VOIP_RTP_UTILS_DEBUG */


/*---------------------------------------------------------------------------------------*/
/*                                     EXTERNALS                                         */
/*---------------------------------------------------------------------------------------*/

extern rtp_session_t *rtp_sessions;	/* ER08 - ORG */
extern u8 rtp_tos; /* ER08 - ORG */
extern int mss_clamping_limit; /* ER08 - ORG */

/*---------------------------------------------------------------------------------------*/
/*                                     STRUCTURES                                        */
/*---------------------------------------------------------------------------------------*/

typedef struct
{
	int   	        localPort;
	int   	        rtpSfd;
	int   	        rtcpSfd;
    acl_call_t    *pCall;
	ipAddrInfo_s	remoteNetTermInfo;
	int		remoteDtmfPayloadNumber;
	int		remoteCurrentCodecPayloadNumber;
} socketInfo_s;

/*---------------------------------------------------------------------------------------*/
/*                                 STATIC VARIABLES                                      */
/*---------------------------------------------------------------------------------------*/
//Nirp 060709 allow WBM configuration of the dynamic payload type
static codec_payload_type_enum_to_id_t codec_payload_type_enum_to_id[]={
    {RTP_PAYLOAD_G726_16,   RTP_PAYLOAD_G726_16},
    {RTP_PAYLOAD_CN,        RTP_PAYLOAD_CN},
    {RTP_PAYLOAD_DTMF,      RTP_PAYLOAD_DTMF},
    {-1,                    0},
};

/* NirM, 080331 - Defined but not used */
#if 0
static AC_CODE2CODE_MAP voip_conn_to_rtp[] =
{
    { VOIP_CONN_MODE_SEND, RTP_SEND },
    { VOIP_CONN_MODE_RECV, RTP_RECEIVE },
    { VOIP_CONN_MODE_SEND_RECV, RTP_SENDRECEIVE },
    { VOIP_CONN_MODE_CONFERENCE, RTP_SENDRECEIVE },
    { -1, 0}
};
#endif

static int g_faxBypassPayloadT;  /* ACL NB 0611102 */
static int g_dataBypassPayloadT; /* ACL NB 0611102 */
static int g_bypassCoder;        /* ACL NB 061213 */

//static socketInfo_s  socketDB[NUMBER_OF_DSP_CHANNELS];
socketInfo_s  socketDB[NUMBER_OF_DSP_CHANNELS];

static int MediaBaseLocalPort = 0;

void aclVoipRtpUtils_ChannelAddrSet(char* addr, unsigned short port, int channel);
void aclVoipRtpUtils_ChannelConnect_phi(int channel);

/*---------------------------------------------------------------------------------------*/
/*                                     STATIC FUNCTIONS                                  */
/*---------------------------------------------------------------------------------------*/
#if 0 /*NirM - need to implement*/
static void clear_payloads(int rtp_fd)
{
    if (ioctl(rtp_fd, RTP_PAYLOAD, NULL))
	rg_error(LERR, "RTP_PAYLOAD clear error: %m");
}

static void map_payload(int rtp_fd, char active_codec)
{
    rtp_ioctl_payload_t params;

    //DBG_PRINT("active_codec=%d .\r\n", active_codec);

    params.media_payload_type = params.rtp_payload_type = active_codec;
    if (ioctl(rtp_fd, RTP_PAYLOAD, &params))
	rg_error(LERR, "RTP_PAYLOAD add error: %m");
}
#endif

static void aclVoipRtpUtils_getLocalPortBySockSfd( int sockSfd,int *localPort )
{
    int curChannel;

    for (curChannel = 0; curChannel < NUMBER_OF_DSP_CHANNELS; curChannel++)
    {
        if( sockSfd == socketDB[curChannel].rtpSfd )
        {
                *localPort = socketDB[curChannel].localPort;
                return;
        }
        else if( sockSfd == socketDB[curChannel].rtcpSfd )
        {
                *localPort = socketDB[curChannel].localPort+1;
                return;
        }
    }

}

static int aclVoipRtpUtils_usedChannelGet(acl_call_t *pCall)
{
    DBG_PRINT("pCall->phone->active_call = %p\n", pCall->phone->active_call);
    DBG_PRINT("pCall->phone->active_call1 = %p\n", pCall->phone->active_call1);
    if (pCall->phone->active_call == pCall)
        return 0;
    if (pCall->phone->active_call1 == pCall)
        return 1;
    else
        return -1;
}

void aclVoipRtpUtils_ChannelAddrSet(char* addr, unsigned short port, int channel)
{
	networking_rtpChannelAddrSet(addr, port, channel);
	networking_rtcpChannelAddrSet(addr, port, channel);
}

void aclVoipRtpUtils_ChannelConnect_phi(int channel)
{
	networking_rtpChannelConnect(channel, socketDB[channel].rtpSfd);
	networking_rtcpChannelConnect(channel, socketDB[channel].rtcpSfd);
}


void aclVoipRtpUtils_ChannelConnect(int channel, int rtpSfd, int rtcpSfd)
{
	networking_rtpChannelConnect(channel, rtpSfd);
	networking_rtcpChannelConnect(channel, rtcpSfd);
}

static void aclVoipRtpUtils_ChannelInfoDelete(int channel)
{
	networking_rtpChannelInfoDelete(channel);
	networking_rtcpChannelInfoDelete(channel);
}

static void aclVoipRtpUtils_ModeSet(int channel, voip_conn_mode_t rtpMode)
{
	networking_rtpModeSet(channel, rtpMode);
	networking_rtcpModeSet(channel, rtpMode);
}

static void aclVoipRtpUtils_getSfds(acl_call_t *pCall, int *rtpSfd, int *rtcpSfd)
{
    socketInfo_s *curSocketRecord = (socketInfo_s *)pCall->rtp->fd;

    if (NULL != rtpSfd)
        *rtpSfd = curSocketRecord->rtpSfd;
    if (NULL != rtcpSfd)
        *rtcpSfd = curSocketRecord->rtcpSfd;
}

static void aclVoipRtpUtils_getRemoteAddr(acl_call_t *pCall, ipAddrInfo_s *remoteAddrInfo)
{
    socketInfo_s *curSocketRecord = (socketInfo_s *)pCall->rtp->fd;


    if (NULL != remoteAddrInfo)
    {
        strcpy(remoteAddrInfo->address, curSocketRecord->remoteNetTermInfo.address);
        remoteAddrInfo->port = curSocketRecord->remoteNetTermInfo.port;
    }
}

static void aclVoipRtpUtils_freeLocalPort(socketInfo_s *curSocketRecord)
{
	DBG_PRINT("pCall = %p localPort = %d\n\r", curSocketRecord->pCall, curSocketRecord->localPort);

    curSocketRecord->pCall = 0;
}

/*---------------------------------------------------------------------------------------*/
/*                                     DYNAMIC FUNCTIONS                                  */
/*---------------------------------------------------------------------------------------*/
int payload_type_enum_to_id(int enum_val)
{
	codec_payload_type_enum_to_id_t* list = codec_payload_type_enum_to_id;

	for (; list->enum_default_value != -1 && list->enum_default_value  != enum_val; list++);
	if (-1 == list->enum_default_value)
	{
//		printf("[%s:%d] ENTRY WAS NOT FOUND !!!. enum_val=%d.\n", __FUNCTION__, __LINE__, enum_val);
		return -1;
	}

	return list->new_id_value;
}

int payload_type_id_to_enum(int id)
{
	codec_payload_type_enum_to_id_t* list = codec_payload_type_enum_to_id;

	for (; list->new_id_value != -1 && list->new_id_value  != id; list++)
	if (-1 == list->new_id_value)
	{
		DBG_PRINT("ENTRY WAS NOT FOUND !!!. id=%d.\n", id);
		return -1;
	}

	return list->enum_default_value;
}

int payload_type_enum_to_id_update(int enum_val, int new_id_val)
{
    	codec_payload_type_enum_to_id_t* list= codec_payload_type_enum_to_id;

	for (; list->enum_default_value != -1 && list->enum_default_value  != enum_val; list++);
	if (-1 == list->enum_default_value)
	{
		DBG_PRINT("updating enum_val=%d, to new id value of %d ENTRY WAS NOT FOUND !!!.\n", enum_val, new_id_val);
		return -1;
	}
	if (list->new_id_value!=new_id_val)
	{
		DBG_PRINT("updating enum_val=%d, to new id value of %d .\n", enum_val, new_id_val);
		list->new_id_value=new_id_val;
	}
	return 0;
}

/* ACL NB 061102 Fax and Data Bypass payload types  and bypassCoder*/
void aclVoipRtpUtils_advancedChConf(voip_dsp_advanced_channel_configuration_t *advanced_channel_configuration)
{
	g_faxBypassPayloadT  = advanced_channel_configuration->faxBypassPayloadType;  /* ACL NB 061105 */
	g_dataBypassPayloadT = advanced_channel_configuration->dataBypassPayloadType; /* ACL NB 061105 */
	g_bypassCoder        = advanced_channel_configuration->bypassCoder;           /* ACL NB 061213 */
}

int aclVoipRtpUtils_createSockets()
{
    int sfd;
    int curChannel;

    if(0 == MediaBaseLocalPort)
    {
    	printf("\nMediaBaseLocalPort = %d", MediaBaseLocalPort);
    	return -1;
    }

    rtp_session_t **rtp_curr = &rtp_sessions;

    DBG_PRINT("rtp_sessions = %p\n", rtp_sessions);
    for (curChannel = 0; curChannel < NUMBER_OF_DSP_CHANNELS; curChannel++)
    {
        /*initialize sockets DB*/
        socketDB[curChannel].pCall = 0;
        socketDB[curChannel].localPort = MediaBaseLocalPort;
        memset(socketDB[curChannel].remoteNetTermInfo.address, 0, ADDRESS_STR_MAX_LEN);
        socketDB[curChannel].remoteNetTermInfo.port = 0;

        sfd = iphone_createSocket(MediaBaseLocalPort);
        if ( sfd == -1 )
            return -1;

        socketDB[curChannel].rtpSfd = sfd;

        /*initialize the rtp_session_t linked list*/
        /*allocate rtp_session_t*/
       	*rtp_curr = AC_ZALLOC_L(sizeof(rtp_session_t));
        if (!*rtp_curr)
        {
            return -1;
        }
        (*rtp_curr)->port = MediaBaseLocalPort;
        (*rtp_curr)->fd = (int)&socketDB[curChannel];

        printf("*rtp_curr = %p\n", *rtp_curr);
        printf("(*rtp_curr)->port = %d\n", (*rtp_curr)->port);
        printf("(*rtp_curr)->fd = 0x%x\n", (*rtp_curr)->fd);

        rtp_curr = &(*rtp_curr)->next;

        sfd = iphone_createSocketRtcp(MediaBaseLocalPort + 1);
        if ( sfd == -1 )
            return -1;

        socketDB[curChannel].rtcpSfd = sfd;
        MediaBaseLocalPort += 2;

        printf("&socketDB[%d] = %p\n", curChannel, &socketDB[curChannel]);
        printf("socketDB[%d].pCall = %p\n", curChannel, socketDB[curChannel].pCall);
        printf("socketDB[%d].localPort = %d\n", curChannel, socketDB[curChannel].localPort);
        DBG_PRINT("socketDB[%d].rtpSfd = %d\n", curChannel, socketDB[curChannel].rtpSfd);
        DBG_PRINT("socketDB[%d].rtcpSfd = %d\n", curChannel, socketDB[curChannel].rtcpSfd);
        DBG_PRINT("socketDB[%d].remoteNetTermInfo.address = %s\n", curChannel, socketDB[curChannel].remoteNetTermInfo.address);
        DBG_PRINT("socketDB[%d].remoteNetTermInfo.port = %d\n", curChannel, socketDB[curChannel].remoteNetTermInfo.port);
    }
	Channel0_rtpsfd=socketDB[0].rtpSfd;
    return 0;
}

void aclVoipRtpUtils_resetNetworkingDBs()
{
	networking_rtpChannelInfoDB_reset();
	networking_rtpSocket2AbsChDB_reset();
	networking_rtcpChannelInfoDB_reset();
	networking_rtcpSocket2AbsChDB_reset();
}

void aclVoipRtpUtils_freeNetworkingDbs()
{
	networking_rtpSfdFree();
	networking_rtcpSfdFree();
}

/******************************************************************************/
/******************************************************************************/

void rtp_mode_set(acl_call_t *call, int flash)
{
    socketInfo_s *cur_socket_record;
    int           absCh;

    DBG_PRINT("call->mode = %d\n\n", call->mode);
    /*do nothing*/
    DBG_PRINT("call->rtp = %p\n", call->rtp);
    DBG_PRINT("call->rtp->fd = %p\n", call->rtp->fd);
    cur_socket_record = (socketInfo_s *)call->rtp->fd;
    if (cur_socket_record)
    {
        DBG_PRINT("cur_socket_record->pCall = %p\n", cur_socket_record->pCall);
        DBG_PRINT("cur_socket_record->rtpSfd = %d\n", cur_socket_record->rtpSfd);
    }
    else
    {
        DBG_PRINT("cur_socket_record is NULL\n");
	return;
    }
    absCh = networking_rtpAbsChannelBySfdGet(cur_socket_record->rtpSfd);

	if(absCh == -1)
		return;

	DBG_PRINT("absCh = %d\n", absCh);
    aclVoipRtpUtils_ModeSet(absCh, call->mode);
}

rtp_session_t *rtp_get(char *cname, codec_t *codecs, acl_call_t *pCall)
{
    rtp_session_t *cur_rtp_session = rtp_sessions;
    socketInfo_s  *cur_socket_record = NULL;

    DBG_PRINT("pCall = %p\n", pCall);
    DBG_PRINT("cur_rtp_session = %p\n", cur_rtp_session);
    while (cur_rtp_session && cur_rtp_session->fd)
    {
        cur_socket_record = (socketInfo_s *)cur_rtp_session->fd;
        DBG_PRINT("cur_socket_record = %p\n", cur_socket_record);
        if (0 == cur_socket_record->pCall)
        {
			cur_socket_record->pCall = pCall;
            DBG_PRINT("cur_socket_record->pCall = %p\n", cur_socket_record->pCall);
            cur_rtp_session->port = cur_socket_record->localPort;
            DBG_PRINT("cur_rtp_session->port = %d\n", cur_rtp_session->port);
            DBG_PRINT("cur_socket_record->localPort = %d\n", cur_socket_record->localPort);
            break;
        }
        cur_rtp_session = cur_rtp_session->next;
    }

    if (!cur_socket_record)
        return NULL;

    return cur_rtp_session;
}

void rtp_unbind(int dspChannel)
{
    DBG_PRINT("dspChannel = %d\n", dspChannel);
    if ((0 <= dspChannel) && (NUMBER_OF_DSP_CHANNELS > dspChannel))
    {
        aclVoipRtpUtils_ChannelInfoDelete(dspChannel);
    }
}

void rtp_free(rtp_session_t *rtp)
{
    socketInfo_s    *curSocketRecord = (socketInfo_s *)rtp->fd;

    DBG_PRINT("rtp = %p\n", rtp);
    DBG_PRINT("curSocketRecord = %p\n", curSocketRecord);
    /*break the link between pCall and its rtp session*/
    curSocketRecord->pCall->rtp = 0;
    /*free the port from the rtp session structure*/
    rtp->port = 0;
    /*break the link between the socket DB and the pCall*/
    aclVoipRtpUtils_freeLocalPort(curSocketRecord);
    /*clear remote info*/
    memset(curSocketRecord->remoteNetTermInfo.address, 0, ADDRESS_STR_MAX_LEN);
    curSocketRecord->remoteNetTermInfo.port = 0;

    /*rtp->fd = 0;*/ /* dont break the linkage between the rtp_session_t and the socket record it points to*/
}

void rtp_connect(acl_call_t *pCall, in_addr_t addr, u16 port)
{
    int             fd = pCall->rtp->fd;
    socketInfo_s    *curSocketRecord = (socketInfo_s *)fd;
    char            *strAddr;
    struct in_addr  in;

    DBG_PRINT("curSocketRecord = %p\n", curSocketRecord);

    in.s_addr = addr;
    strAddr = inet_ntoa(in);

    DBG_PRINT("curSocketRecord->rtpSfd = %d\n", curSocketRecord->rtpSfd);
    DBG_PRINT("curSocketRecord->localPort = %d\n", curSocketRecord->localPort);
    DBG_PRINT("addr.s_addr = %s\n", strAddr);
    DBG_PRINT("port = %d\n", port);

	if(strlen(strAddr) < sizeof(curSocketRecord->remoteNetTermInfo.address))
    		strcpy(curSocketRecord->remoteNetTermInfo.address, strAddr);
	else
	{
		printf("String length of strAddr %d is equal or bigger then sizeof address %d\r\n",
				strlen(strAddr), sizeof(curSocketRecord->remoteNetTermInfo.address));
		return;
	}

    curSocketRecord->remoteNetTermInfo.port = port;

    /*ACL BW 20110518- it cannot update the remote port of the current connection */
    {
    	int             line;
    	int             channel;
    	int             absCh;

        DBG_PRINT("[update the remote]new port = %d\n", port);
        if(!pCall->phone)
        {
            DBG_PRINT("The call is not connected\n");
            return;
        }

        line = pCall->phone->num;
    	DBG_PRINT("line = %d\n", line);
    	channel = aclVoipRtpUtils_usedChannelGet(pCall);
    	DBG_PRINT("channel = %d\n", channel);
    	if (-1 == channel)
    	{
    		DBG_PRINT_ERR("channel is not primary or secondary ?!?!\n");
    		return ;
    	}
#if defined(AC494_TULIP2_21)
    	if (line == 2)
    		absCh = line;
    	else
    		absCh = getDspChannelByLineAndVoiceChannel(line, channel);
#else
    	absCh = getDspChannelByLineAndVoiceChannel(line, channel);
#endif
    	DBG_PRINT("absCh = %d\n", absCh);

    	if(absCh != -1)
    	{
    		aclVoipRtpUtils_ChannelAddrSet(strAddr, port, absCh);
    	}
	}
}

u32 rtp_id_prepare(acl_call_t *pCall)
{
	int             rtpSfd;
	int             rtcpSfd;
	int             line;
	int             channel;
	int             absCh;
	u32             id = 0;
	ipAddrInfo_s    remoteAddr;

	DBG_PRINT("pCall = %p\n", pCall);
	aclVoipRtpUtils_getSfds(pCall, &rtpSfd, &rtcpSfd);
	DBG_PRINT("rtpSfd = %d\n", rtpSfd);
	DBG_PRINT("rtcpSfd = %d\n", rtcpSfd);
	aclVoipRtpUtils_getRemoteAddr(pCall, &remoteAddr);
	DBG_PRINT("remoteAddr.address = %s\n", remoteAddr.address);
	DBG_PRINT("remoteAddr.port = %d\n", remoteAddr.port);
	line = pCall->phone->num;
	DBG_PRINT("line = %d\n", line);
	channel = aclVoipRtpUtils_usedChannelGet(pCall);
	DBG_PRINT("channel = %d\n", channel);
	if (-1 == channel)
	{
		DBG_PRINT_ERR("channel is not primary or secondary ?!?!\n");
		return 0;
	}
#if defined(AC494_TULIP2_21)
	if (line == 2)
		absCh = line;
	else
		absCh = getDspChannelByLineAndVoiceChannel(line, channel);
#else
	absCh = getDspChannelByLineAndVoiceChannel(line, channel);
#endif
	DBG_PRINT("absCh = %d\n", absCh);

	if(absCh != -1)
	{
		aclVoipRtpUtils_ChannelConnect(absCh, rtpSfd, rtcpSfd);
		aclVoipRtpUtils_ChannelAddrSet(remoteAddr.address, remoteAddr.port, absCh);
		aclVoipRtpUtils_ModeSet(absCh, pCall->mode);
	}

	id = (u32)(pCall->rtp);
	DBG_PRINT("pCall->rtp = %p\n", pCall->rtp);

	return id;
}

void rtp_rcv(int fd, void *context)
{
    /*do nothing*/
}

void rtp_add_conf_mate(acl_call_t *pCall, int absCh)
{
	int             rtpSfd;
	int             rtcpSfd;

	ipAddrInfo_s    remoteAddr;

    	DBG_PRINT(">>\n");

	aclVoipRtpUtils_getSfds(pCall, &rtpSfd, &rtcpSfd);
	aclVoipRtpUtils_getRemoteAddr(pCall, &remoteAddr);

	aclVoipRtpUtils_ChannelConnect(absCh, rtpSfd, rtcpSfd);
	aclVoipRtpUtils_ChannelAddrSet(remoteAddr.address, remoteAddr.port, absCh);
	aclVoipRtpUtils_ModeSet(absCh, pCall->mode);

    	DBG_PRINT("<<\n");

}

void rtp_remove_conf_mate(int absCh)
{
    	DBG_PRINT(">>\n");

	rtp_unbind(absCh);

	DBG_PRINT("<<\n");
}


void map_dynamic_payload(int rtp_fd, char media_pl, char rtp_pl)
{
	socketInfo_s    *curSocketRecord = (socketInfo_s *)rtp_fd;
	int 			enum_code = -1;

	enum_code = payload_type_id_to_enum(media_pl);
	if(enum_code != -1)
	{
		/* For dynamic payload type */
	}
	else
	{
		enum_code = media_pl;	/* Not dynamic payload type  - use the original value*/
	}

	switch(enum_code)
	{
		case RTP_PAYLOAD_DTMF:
			curSocketRecord->remoteDtmfPayloadNumber = rtp_pl;
			break;

		case RTP_PAYLOAD_G726_16:
			curSocketRecord->remoteCurrentCodecPayloadNumber = rtp_pl;
			break;

		default:
			printf("[%s:%d] unsupported basic rtp payload type %d\n", __FUNCTION__, __LINE__, media_pl);
			break;
	}
}

void rtp_t38_set(acl_call_t *call, int isT38)
{
    /*do nothing*/
}

void rtp_base_local_port_set(int mediaBaseLocalPort)
{
	MediaBaseLocalPort = mediaBaseLocalPort;
}

void rtp_statistics_get(acl_line_t *line, rtp_statistics_t *rtpstat)
{
    /*do nothing*/
}

static rtp_session_t *rtp_get_by_port(unsigned short port)
{
   rtp_session_t *curr = rtp_sessions;

   //rg_error(LERR, "\n[rtp_get_by_port (%d)] port %d", __LINE__, port);
   DBG_PRINT("[rtp_get_by_port (%d)] port %d\n", __LINE__, port);

   while (curr && curr->fd)
   {
      if (curr->port == port)
         return curr;

      curr = curr->next;
   }

   //rg_error(LERR, "\n[rtp_get_by_port (%d)] failed", __LINE__);
   DBG_PRINT_ERR("[rtp_get_by_port (%d)] failed\n", __LINE__);
   return NULL;
}

void stun_rtp_req( int localPort,
                               char *msg,
                               int msgSz,
                               int     remotePort,
                               char    *remoteAddress)
{
   rtp_session_t  *rtp=NULL;
   socketInfo_s  *cur_socket = NULL;

   rtp = rtp_get_by_port(localPort);

   if (!rtp )
   {
      DBG_PRINT_ERR(" \n stun_rtp_req failed - no rtp session\r\n");
      return;
   }

   cur_socket = (socketInfo_s *)rtp->fd;
   DBG_PRINT("cur_socket_record = %p\n", cur_socket);

   networking_stun_rtp_set( cur_socket->rtpSfd,1 );

   networking_transmitStunReq( msg,
                                                     msgSz,
                                                     cur_socket->rtpSfd,
                                                     remoteAddress,
                                                     remotePort);

}

void stun_rtcp_req( int localPort,
                               char *msg,
                               int msgSz,
                               int     remotePort,
                               char    *remoteAddress)
{
   rtp_session_t  *rtp=NULL;
   socketInfo_s  *cur_socket = NULL;

   rtp = rtp_get_by_port(localPort-1);

   if (!rtp )
   {
      DBG_PRINT_ERR(" \n stun_rtp_req failed - no rtp session\r\n");
      return;
   }

   cur_socket = (socketInfo_s *)rtp->fd;
   DBG_PRINT("cur_socket_record = %p\n", cur_socket);

   networking_stun_rtcp_set( cur_socket->rtcpSfd,1 );

   networking_transmitStunReq( msg,
                                                     msgSz,
                                                     cur_socket->rtcpSfd,
                                                     remoteAddress,
                                                     remotePort);
}

void stun_response_handle(acl_line_t *line, phone_event_t *pEvent)
{
   //unsigned char  buff[128]; /*TBD ETH_DATA_LEN ?*/
	int            status=0;
   rtp_session_t  *rtp=NULL;
   int            port=-1;
   int sFd;
   rtp_stun_res_t stun_res;
   u32            mappedIp;
   u16            mappedPort;
   char           isRTCP=0;
   socketInfo_s  *cur_socket = NULL;

   DBG_PRINT(">>\r\n");

   sFd = pEvent->intData;
   aclVoipRtpUtils_getLocalPortBySockSfd( sFd,&port );

   DBG_PRINT("port=%d \r\n", port);

   if (port % 2)
   {
      DBG_PRINT("This is RTCP Port port=%d \r\n", port);
      isRTCP = 1;
   }
   if (!isRTCP)
      rtp = rtp_get_by_port(port);
   else
      rtp = rtp_get_by_port(port-1);

   if (!rtp )
   {
      DBG_PRINT_ERR(" \n stun_response_handle failed - no rtp session\r\n");
      return;
   }

   cur_socket = (socketInfo_s *)rtp->fd;


   memset(stun_res.buff, 0 , sizeof(stun_res.buff));

   DBG_PRINT(" ****\r\n");

   /* Get stun buffer */
   if (!isRTCP)
   {
      networking_stun_rtp_set( cur_socket->rtpSfd,0 );
      networking_get_rtp_stun_res( stun_res.buff,&stun_res.buff_len,sizeof(stun_res.buff) );
   }
   else
   {
      networking_stun_rtcp_set( cur_socket->rtcpSfd,0 );
      networking_get_rtcp_stun_res( stun_res.buff,&stun_res.buff_len,sizeof(stun_res.buff) );
   }

   if ( stun_res.buff_len > 0)
   {
      status = call_mgt->stun_client_reply_received((char *)stun_res.buff, stun_res.buff_len, &mappedIp, &mappedPort );
      if ( status != 0)
      {
         DBG_PRINT_ERR("stun_response_handle failed\r\n");
      }
      else
      {
         DBG_PRINT("mappedIp=0x%x mappedPort=%d\r\n", mappedIp , mappedPort);
      }
   }
   else
      DBG_PRINT_ERR(" \n stun_response_handle failed - no valid stun buffer\r\n");
   DBG_PRINT("<<\r\n");

  return;
}

void get_remote_dtmf_payload_number(int rtp_fd, int *remoteDtmfPayloadNumber)
{
	socketInfo_s    *curSocketRecord = (socketInfo_s *)rtp_fd;

	if (NULL != remoteDtmfPayloadNumber)
	{
		*remoteDtmfPayloadNumber = curSocketRecord->remoteDtmfPayloadNumber;
	}
}

void get_remote_current_codec_payload_number(int rtp_fd, int *remoteCurrentCodecPayloadNumber)
{
	socketInfo_s    *curSocketRecord = (socketInfo_s *)rtp_fd;

	if (NULL != remoteCurrentCodecPayloadNumber)
	{
 		*remoteCurrentCodecPayloadNumber = curSocketRecord->remoteCurrentCodecPayloadNumber;
	}
}


