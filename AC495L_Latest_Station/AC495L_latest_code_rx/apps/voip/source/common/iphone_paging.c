/******************************************************************************

  Copyright (C), 2001-2011, DCN Co., Ltd.

 ******************************************************************************
  File Name     : iphone_paging.c
  Version       : Initial Draft
  Author        : Andy.Deng
  Created       : 2017/6/20
  Last Modified :
  Description   : paging
  Function List :acl_create_paging_thread
  History       :
  1.Date        : 2017/6/20
    Author      : Andy.Deng
    Modification: Created file

******************************************************************************/

#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

//#include "iphone_paging_in.h"
#include "voip_main.h"
#include "config_manager.h"
#include "server_coms.h"
#include "enums.h"
#include "iphone_localNetTerm.h"


#define EMERGENCY_ALARM   "Emergency.wav"
#define ALL_CLEAR_ALARM   "All_clear.wav"
#define CUT0              "CUT0.wav"
#define CUT1              "CUT1.wav"
#define CUT2              "CUT2.wav"
#define CUT3              "CUT3.wav"
#define CUT4              "CUT4.wav"
#define CUT5              "CUT5.wav"
#define CUT6              "CUT6.wav"
#define DIAGNOSTIC_TONE   "square.wav"


acgTChannelParameters ChannelParameters[CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES][NUMBER_OF_CHANNELS_PER_LINE];
socketInfo  socketDB_phi[3];
conection_mode_t	connectionMode[3];

void drop_membership(int fd);
void add_membership(int fd);
void multicast_prepare_dsp(int channel);
int CreateConnection_phi(int ch,char *buf);

struct ip_mreq pg_mreq,aim_mreq[4],icom_mreq[32],group_mreq[32],zone_mreq[32];


int  g_itransmit_fd_index;
int  g_Iicom_fds[32];
int  g_igroup_fds[32];
int  g_izone_fds[32];
int  g_iAim_fds[4];
int  g_iPaging_fd;

char incoming_pg_rtp_addr[20];


static PAGING_CONFIG    pConfig;
static PAGING_WORKING   pWork;



 //struct ip_mreq aim_mreq[4];
 //struct ip_mreq icom_mreq[32];
 //struct ip_mreq group_mreq[32];
 //struct ip_mreq zone_mreq[32];

int isValidDSPChannel(int dspChannel)
{
	if( (dspChannel>=0) && (dspChannel<ACG_NUMBER_OF_DSP_CHANNEL) )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
int isConnectionwithRemoteIP(char *str, char *pRemoteAddress, int *pRemotePort)
{
	char temp[200] = "";
	int delimiter = ':';
	char *pToCharacter;

	strcpy(temp, str);

	pToCharacter = strchr(temp, delimiter) ;

	if(pToCharacter != NULL )
	{
		*pToCharacter = 0;	/* Replace the delimiter with NULL to device into two strings */

		strcpy(pRemoteAddress, temp);
		*pRemotePort = atoi(pToCharacter+1);

		return 1;
	}
	else
	{
		return 0;
	}
}

void setConnectionMode(int channel, conection_mode_t mode)
{
	connectionMode[channel] = mode;
}

int CreateConnection_phi(int ch,char *buf)
{
	conection_mode_t 	mode;
	char 				remoteAddress[128];
	short 				remotePort, ret = -1;




	ret = isConnectionwithRemoteIP(buf, remoteAddress, &remotePort);

	if(ret)
	{
		if(!isValidDSPChannel(ch))
		{
			printf("Invalid DSP channel %d\r\n", ch);
			return (-1);
		}

		mode = CONN_MODE_REMOTE;
		setConnectionMode(ch, mode);


		aclVoipRtpUtils_ChannelAddrSet(remoteAddress, remotePort,ch);


		aclVoipRtpUtils_ChannelConnect_phi(ch);

		printf("Create connection between DSP channel %d to remote IP Address: %s and port %d\n", ch, remoteAddress, remotePort);


       /* networking_rtpChannelAddrSet(remoteAddress, remotePort, ch);
	    networking_rtcpChannelAddrSet(remoteAddress, remotePort, ch);

		//aclVoipRtpUtils_ChannelAddrSet(remoteAddress, remotePort, ch);
        socketDB_phi[ch].rtpSfd=pWork_m.pagingFdDB[0];
        //socketDB_phi[ch].rtcpSfd=pWork_m.pagingFdDB[0]+1;
        networking_rtpChannelConnect(ch, socketDB_phi[ch].rtpSfd);
	    networking_rtcpChannelConnect(ch, socketDB_phi[ch].rtcpSfd);
		//aclVoipRtpUtils_ChannelConnect(ch);*/

		//printf("Create connection between DSP channel %d to remote IP Address: %s and port %d\n", ch, remoteAddress, remotePort);


	}
	else   /* Connection between two local DSP channels */
	{
		if(!isValidDSPChannel(ch) && !isValidDSPChannel(ch))
		{
			printf("Invalid DSP channels %d %d\r\n", ch, ch);
			return (-1);
		}

		mode = CONN_MODE_LOCAL;
		setConnectionMode(ch, mode);

		//setLocalConnectionChannelMapping(ch, ch);

		printf("Set local connection between DSP channel %d to %d\n", ch, ch);
	}

	return 0;

}

static int paging_allow_packet()
{
    unsigned int ssrc = 0;
    RTPHeader *rtpHeader   = (RTPHeader *) pWork.pagingBuff;
    RTCPHeader *rtcpHeader = (RTCPHeader*) pWork.pagingBuff;

    ssrc = rtpHeader->SSRC;

    // check the payload is we need or not
    if (rtpHeader->PT != pConfig.payload)
    {
      //  printf("so this maybe a RTCP packets\n");
        ssrc = rtcpHeader->ssrc;
    }

    // check the SSRC correct or not
    if (ssrc != pWork.currentSSRC)
    {
        // printf("SSRC NOT MATCHING\n");
       // return 0;
    }

    // check we are incoming paging state or not
  //  if (pWork.state != P_INCOMING_ONGOING)
 //       return 0;

    return 1; // allow this packet by default.
}

void select_codec(ch,codec)
{
 printf("selecting channel %d codec =%d \n",ch,codec);
 acIPPSetAudioDevice_gw(ch,codec);
 //acgOpenChannel(channel, &ChannelParameters);
 //acgActivateRTP(channel);
}




static void *paging_listen(void *arg)
{
	fd_set				readyFds;
    struct timeval      pTv;
	int					chId;
	int					msgSize;
	int					curFd;
	char buf[32];
	int cont1;
	int cont2;

    char call_type;




    pTv.tv_sec = 0;
    pTv.tv_usec = 500000;
    unsigned int curr_ssrc;
    struct icom_contributor_details icom[32];
	while(1)
	{


     pthread_mutex_lock(&lock_call_type);
     call_type=pvt_call.type;
	 pthread_mutex_unlock(&lock_call_type);



	 if(call_type==GROUP || call_type==GROUP_TX_RX)
	 {
         pthread_mutex_lock(&lock_group_contr);
        cont1=group_contributor_status_info[pvt_call.group_no].cont1;
        cont2=group_contributor_status_info[pvt_call.group_no].cont2;
         pthread_mutex_unlock(&lock_group_contr);
        //printf("GROUP call type=%c cont1=%d cont2=%d \n",call_type,cont1,cont2);
	 }
	 else if(call_type==INTERCOM_TX_RX || call_type==INTERCOM_RX)
	 {
         pthread_mutex_lock(&lock_icom_contr);
         cont1=icom_contributor_status_info[pvt_call.curr_icom_no].cont1;
         cont2=icom_contributor_status_info[pvt_call.curr_icom_no].cont2;
         pthread_mutex_unlock(&lock_icom_contr);
         // printf("ICOM call type=%c cont1=%d cont2=%d \n",call_type,cont1,cont2);

	 }





        msgSize = 0;
        readyFds = pWork.pagingRtpReceivingFds;

        if (select(pWork.pagingRtpMaxFd+1, &readyFds, NULL, NULL, &pTv) == -1)
		{
			printf("acl_paging_listen:: select failed");
			return NULL;
        }



        for(curFd = 0; curFd < pWork.pagingFdDBRealSize; curFd++)
       	{
			if (FD_ISSET(pWork.pagingFdDB[curFd], &readyFds))
			{
              //  printf("in paging listen %d  \n",pWork.pagingFdDB[curFd]);
				if((msgSize=recv(pWork.pagingFdDB[curFd], pWork.pagingBuff, PAGING_BUFF_LEN, 0)) < 0)
				{
					printf("acl_paging_listen:: recvfrom");
				}
                //  RTPHeader *rtpHeader = (RTPHeader *)pWork.pagingBuff;
              //    pWork.currentSSRC = rtpHeader->SSRC;
                paging_received_packet(curFd);
              //  paging_allow_packet();
              //  if((pWork_m.page_state!=PAGE_INCOMING_ONGOING)&&(private_call_busy!=SET))
              //  {
                sprintf(buf,"%d\n",(pWork.currentSSRC>>24));
                curr_ssrc=atoi(buf);
              //  }*/
               // printf("string %d %d\n",stn_config.logical_id,curr_ssrc);

				// printf("cnt1=%d \n",server_coms_group_call_info[pvt_call.group_no].cont1);
				//  printf("cnt2=%d \n",server_coms_group_call_info[pvt_call.group_no].cont2);

				if(curr_ssrc!=stn_config.logical_id)
				{

				//	printf("curr ssrc %d\n",curr_ssrc);
						if(call_type==GROUP || call_type==GROUP_TX_RX)
						{
							if(cont1==curr_ssrc)
							{
								transmitter(pWork.pagingBuff, msgSize, PVT_CHANNEL, ACG_PROTOCOL__RTP);
							}

							if(cont2==curr_ssrc)
							{
								transmitter(pWork.pagingBuff, msgSize, CONF_CHANNEL, ACG_PROTOCOL__RTP);
							}
						}
						else if((call_type==INTERCOM_TX_RX)||(call_type==INTERCOM_RX))
						{
						  if(cont1==curr_ssrc)
						  transmitter(pWork.pagingBuff, msgSize, PVT_CHANNEL, ACG_PROTOCOL__RTP);

							if(cont2==curr_ssrc)
							transmitter(pWork.pagingBuff, msgSize, CONF_CHANNEL, ACG_PROTOCOL__RTP);
						}


						if((pWork.pagingFdDB[curFd]==g_iPaging_fd)||(pWork.pagingFdDB[curFd]==g_iAim_fds[0])
                            ||(pWork.pagingFdDB[curFd]==g_izone_fds[pg_call.zone_no]))
						{
                             printf(" ************INSIDE pg_call.type=%c \n",pg_call.type);
						  transmitter(pWork.pagingBuff, msgSize, PG_CHANNEL, ACG_PROTOCOL__RTP);
						}
				}
			}

		}
	}
}


static void create_paging_thread()
{
	pthread_t			tid;
	struct sched_param	sched;
	pthread_attr_t		attr;

	memset(&sched, 0, sizeof(sched));

	/* create the paging_listen thread */
	if( (pthread_attr_init(&attr) != 0) )
	{
		printf("create paging thread pthread_attr_init");
		exit(1);
	}
	if( (pthread_attr_setschedpolicy(&attr, SCHED_RR) != 0) )
	{
		printf("pthread_attr_setschedpolicy");
		exit(1);
	}
  	sched.sched_priority = 76;
 	if( (pthread_attr_setschedparam(&attr, &sched) != 0) )
	{
		printf("pthread_attr_setschedparam");
		exit(1);
	}
	if( (pthread_create(&tid,&attr,paging_listen, 0) != 0) )
	{
		printf("pthread_create - paging listen[%s:%d]\n",__FUNCTION__,__LINE__);
		//exit(1);
	}

printf("\n paging_listen therad created \n");
	pWork.pagingTid = tid;
	return ;
}


/*static int paging_is_active_calls()
{
    return acl_is_in_active_call();
}

static void paging_disconnect_all_calls()
{
    acl_disconnect_all_calls();
}
static void paging_display_all_streams()
{
    int i,total = sizeof(pWork.stream)/sizeof(PAGING_STREAM);
    int is_empty = 1;

    printf("\n====stream group SSRC    \tpackets====\n");
    for (i=0;i<total;i++)
    {
        if (pWork.stream[i].SSRC != 0)
        {
            is_empty = 0;
            printf("    %d      %d     %08X\t%d \n",i,pWork.stream[i].group,pWork.stream[i].SSRC,pWork.stream[i].count);
        }
    }
    if (is_empty) printf("   No stream.\n");
    printf("===========================================\n");
}*/
/*static PAGING_STREAM * paging_get_stream(unsigned int ssrc, int group)
{
    int i,total = sizeof(pWork.stream)/sizeof(PAGING_STREAM);

    for (i=0;i<total;i++)
    {
        if (pWork.stream[i].SSRC != 0 && pWork.stream[i].SSRC == ssrc)
        {
            return &pWork.stream[i];
        }
    }

    // if running here, no matched find, return a new one.
    for (i=0;i<total;i++)
    {
        if (pWork.stream[i].SSRC == 0)
        {
            pWork.stream[i].SSRC  = ssrc;
            pWork.stream[i].group = group;
            paging_display_all_streams();
            return &pWork.stream[i];
        }
    }

    // if runing here, no enough resoure.
    acl_log(ACL_LOG_ERROR,"There is no enough place to save stream, please expand the value of MAX_PAGING_STREAM");
    return NULL;
}*/


static void create_icom_socket()
{
	struct sockaddr_in icom_addr[32];
    char buf[20];
    unsigned  char loop=0;

	int i,true_value=0;

    char icom_ip_addr[30];

    for(i=0;i<32;i++)
    {
        //close(g_Iicom_fds[i]);
	    g_Iicom_fds[i] = socket (AF_INET, SOCK_DGRAM, 0);
	    if (g_Iicom_fds[i] < 0)
	    {
	        printf("socket creating failed for paging %d\n", i);
	        return;
	    }
	    sprintf(icom_ip_addr,"%s%d",gc_icom_mul_start_addr,g_iIcom_mul_addr_start_offset+i);
	   // printf("ICOM IP ADDRESS=%s\n",icom_ip_addr);
      //  printf("Create ICOM Socket for group %d sockfd =%d,at %s:%d\n",i,g_Iicom_fds[i],icom_ip_addr,stn_config.def_icom_port);


         if (setsockopt(g_Iicom_fds[i], SOL_SOCKET, SO_REUSEADDR, &true_value, sizeof(true_value)) < 0) {
            perror("setsockopt reuse failed.......");
           // exit(1);
        }

	    bzero(&icom_mreq[i], sizeof (struct ip_mreq));
		memset (&icom_addr[i], 0, sizeof(struct sockaddr_in));
	    icom_addr[i].sin_family = AF_INET;
	    icom_addr[i].sin_port = htons(PVT_CALL_PORT);
	    inet_pton(AF_INET,icom_ip_addr,&icom_addr[i].sin_addr);

	    bcopy (&icom_addr[i].sin_addr.s_addr, &icom_mreq[i].imr_multiaddr.s_addr, sizeof (struct in_addr));

	    icom_mreq[i].imr_interface.s_addr = inet_addr(gc_station_ip);

		if (bind(g_Iicom_fds[i], (struct sockaddr *) &icom_addr[i],sizeof (struct sockaddr_in)) == -1)
	    {
	        perror( "Bind error:\n");
	       // return;
	    }

	    if (setsockopt(g_Iicom_fds[i], IPPROTO_IP, IP_MULTICAST_LOOP, &loop,sizeof (loop)) == -1)
	    {
	        printf( "Join Group %d Failed", i);
			//break;
	    }

		pWork.pagingFdDB[i] = g_Iicom_fds[i];
		pWork.pagingFdDBRealSize++;
	printf("pwork.pagingFdDB[%d]=%d ,pWork.pagingFdDBRealSize=%d  \n",i,pWork.pagingFdDB[i],pWork.pagingFdDBRealSize);
        if (g_Iicom_fds[i] > pWork.pagingRtpMaxFd)
            pWork.pagingRtpMaxFd = g_Iicom_fds[i];
        FD_SET(g_Iicom_fds[i], &pWork.pagingRtpReceivingFds);
       //  sprintf(buf,"route add -net %s netmask 255.255.255.255 br-lan \n",icom_ip_addr);
      // system(buf);
 }

}

static void create_group_socket()
{
	struct sockaddr_in group_addr[32];
    char buf[20];

	int i,true_value=0;
	unsigned char loop=0;
    char group_ip_addr[30];

    for(i=0;i<32;i++)
    {
        //close(g_igroup_fds[i]);
	    g_igroup_fds[i] = socket (AF_INET, SOCK_DGRAM, 0);
	    if (g_igroup_fds[i] < 0)
	    {
	        printf("socket creating failed for paging %d\n", i);
	        return;
	    }
	    sprintf(group_ip_addr,"%s%d",gc_group_mul_start_addr,g_igroup_mul_addr_start_offset+i);
	   // printf("GROUP IP ADDRESS=%s\n",group_ip_addr);
       // printf("Create GROUP Socket for group %d sockfd =%d,at %s:%d\n",i,g_igroup_fds[i],group_ip_addr,stn_config.def_icom_port);


         if (setsockopt(g_igroup_fds[i], SOL_SOCKET, SO_REUSEADDR, &true_value, sizeof(true_value)) < 0) {
            perror("setsockopt reuse failed.......");
           // exit(1);
        }

	    bzero(&group_mreq[i], sizeof (struct ip_mreq));
		memset (&group_addr[i], 0, sizeof(struct sockaddr_in));
	    group_addr[i].sin_family = AF_INET;
	    group_addr[i].sin_port = htons(PVT_CALL_PORT);
	    inet_pton(AF_INET,group_ip_addr,&group_addr[i].sin_addr);

	    bcopy (&group_addr[i].sin_addr.s_addr, &group_mreq[i].imr_multiaddr.s_addr, sizeof (struct in_addr));

	    group_mreq[i].imr_interface.s_addr = inet_addr(gc_station_ip);

		if (bind(g_igroup_fds[i], (struct sockaddr *) &group_addr[i],sizeof (struct sockaddr_in)) == -1)
	    {
	        printf( "Bind error\n");
	       // return;
	    }

	    if (setsockopt(g_igroup_fds[i], IPPROTO_IP, IP_MULTICAST_LOOP, &loop,sizeof (loop)) == -1)
	    {
	        printf( "Join Group %d Failed", i);
			//break;
	    }

		pWork.pagingFdDB[GROUP_OFFSET+i] = g_igroup_fds[i];
		pWork.pagingFdDBRealSize++;
		printf("pwork.pagingFdDB[%d]=%d ,pWork.pagingFdDBRealSize=%d  \n",32+i,pWork.pagingFdDB[32+i],pWork.pagingFdDBRealSize);
        if (g_igroup_fds[i] > pWork.pagingRtpMaxFd)
            pWork.pagingRtpMaxFd = g_igroup_fds[i];
        FD_SET(g_igroup_fds[i], &pWork.pagingRtpReceivingFds);
       //  sprintf(buf,"route add -net %s netmask 255.255.255.255 br-lan \n",icom_ip_addr);
      // system(buf);
 }

}

static void create_zone_socket()
{
	struct sockaddr_in zone_addr[32];
    char buf[20];
    unsigned char loop=0;
	int i,true_value=0;
    struct server_coms_global_alarm alarm;
    char zone_ip_addr[30];

    for(i=0;i<32;i++)
    {
       //close(g_izone_fds[i]);
	    g_izone_fds[i] = socket (AF_INET, SOCK_DGRAM, 0);
	    if (g_izone_fds[i] < 0)
	    {
	        printf("socket creating failed for paging %d\n", i);
	        return;
	    }
	    sprintf(zone_ip_addr,"%s%d",gc_zone_mul_start_addr,g_izone_mul_addr_start_offset+i);
	   // printf("ZONE IP ADDRESS=%s\n",zone_ip_addr);
       // printf("Create zone Socket for group %d sockfd =%d,at %s:%d\n",i,g_izone_fds[i],zone_ip_addr,stn_config.def_icom_port);


         if (setsockopt(g_izone_fds[i], SOL_SOCKET, SO_REUSEADDR, &true_value, sizeof(true_value)) < 0) {
            perror("setsockopt reuse failed.......\n");
           // exit(1);
        }

        printf("Create zone Socket for zone %d sockfd =%d,at %s:%d\n",i,g_izone_fds[i],zone_ip_addr,PAGE_CALL_PORT);
	    bzero(&zone_mreq[i], sizeof (struct ip_mreq));
		memset (&zone_addr[i], 0, sizeof(struct sockaddr_in));
	    zone_addr[i].sin_family = AF_INET;
	    zone_addr[i].sin_port = htons(PAGE_CALL_PORT);
	    inet_pton(AF_INET,zone_ip_addr,&zone_addr[i].sin_addr);

	    bcopy (&zone_addr[i].sin_addr.s_addr, &zone_mreq[i].imr_multiaddr.s_addr, sizeof (struct in_addr));

	    zone_mreq[i].imr_interface.s_addr = inet_addr(gc_station_ip);

		if (bind(g_izone_fds[i], (struct sockaddr *) &zone_addr[i],sizeof (struct sockaddr_in)) == -1)
	    {
	        printf( "Bind error\n");
	       // return;
	    }

	  if (setsockopt(g_izone_fds[i], IPPROTO_IP, IP_MULTICAST_LOOP, &loop,sizeof (loop)) == -1)
	    {
	        printf( "Join Group %d Failed", i);
			//break;
	    }

		pWork.pagingFdDB[ZONE_OFFSET+i] = g_izone_fds[i];
		pWork.pagingFdDBRealSize++;
		printf("pwork.pagingFdDB[%d]=%d ,pWork.pagingFdDBRealSize=%d  \n",ZONE_OFFSET+i,pWork.pagingFdDB[ZONE_OFFSET+i],pWork.pagingFdDBRealSize);
        if (g_izone_fds[i] > pWork.pagingRtpMaxFd)
            pWork.pagingRtpMaxFd = g_izone_fds[i];
        FD_SET(g_izone_fds[i], &pWork.pagingRtpReceivingFds);
       //  sprintf(buf,"route add -net %s netmask 255.255.255.255 br-lan \n",icom_ip_addr);
      // system(buf);
 }

}



static void create_aim_socket()
{
	struct sockaddr_in aim_addr[4];

    //struct ip_mreq aim_mreq;
	int i,true_value=0;
    struct server_coms_global_alarm alarm;
    char aim_ip_addr[30];

    for(i=0;i<4;i++)
    {
        //close(g_iAim_fds[i]);
	    g_iAim_fds[i] = socket (AF_INET, SOCK_DGRAM, 0);
	    if (g_iAim_fds[i] < 0)
	    {
	        printf("socket creating failed for paging %d\n", i);
	        return;
	    }
	     sprintf(aim_ip_addr,"%s%d",gc_aim_mul_start_addr,g_iaim_mul_addr_start_offset+i);
        printf("Create AIM Socket for group %d sockfd =%d,at %s:%d\n",i,g_iAim_fds[i],aim_ip_addr,PAGE_CALL_PORT);


         if (setsockopt(g_iAim_fds[i], SOL_SOCKET, SO_REUSEADDR, &true_value, sizeof(true_value)) < 0) {
            perror("setsockopt reuse failed.......");
          //  exit(1);
        }

	    bzero(&aim_mreq[i], sizeof (struct ip_mreq));
		memset (&aim_addr[i], 0, sizeof(struct sockaddr_in));
	    aim_addr[i].sin_family = AF_INET;
	    aim_addr[i].sin_port = htons(PAGE_CALL_PORT);
	    inet_pton(AF_INET,aim_ip_addr,&aim_addr[i].sin_addr);

	    bcopy (&aim_addr[i].sin_addr.s_addr, &aim_mreq[i].imr_multiaddr.s_addr, sizeof (struct in_addr));

	    aim_mreq[i].imr_interface.s_addr = inet_addr(gc_station_ip);

		if (bind(g_iAim_fds[i], (struct sockaddr *) &aim_addr[i],sizeof (struct sockaddr_in)) == -1)
	    {
	        printf( "Bind error\n");
	       // return;
	    }



		pWork.pagingFdDB[AIM_OFFSET+i] = g_iAim_fds[i];
		pWork.pagingFdDBRealSize++;
		printf("pwork.pagingFdDB[%d]=%d ,pWork.pagingFdDBRealSize=%d  \n",96+i,pWork.pagingFdDB[96+i],pWork.pagingFdDBRealSize);
        if (g_iAim_fds[i] > pWork.pagingRtpMaxFd)
            pWork.pagingRtpMaxFd = g_iAim_fds[i];
        FD_SET(g_iAim_fds[i], &pWork.pagingRtpReceivingFds);
}
}




void crate_global_paging_socket()
{
	struct sockaddr_in paging_addr;
	int true_value=0;
	unsigned char loop=0;
	int i,ret;
	 char buf[100];
	 char global_ip_addr[30];

        g_iPaging_fd = socket (AF_INET, SOCK_DGRAM, 0);
	    if (g_iPaging_fd < 0)
	    {
	        printf("socket creating failed for paging \n");
	        return;
	    }
	    sprintf(global_ip_addr,"%s%d",gc_global_mul_start_addr,g_iglobal_mul_addr_start_offset);
        printf("Create Paging Socket for group sockfd =%d,at %s:%d\n",g_iPaging_fd,global_ip_addr,
        PAGE_CALL_PORT);


         if (setsockopt(g_iPaging_fd, SOL_SOCKET, SO_REUSEADDR, &true_value, sizeof(true_value)) < 0) {
            perror("setsockopt reuse failed.......");
           // exit(1);
        }

	    bzero(&pg_mreq, sizeof (struct ip_mreq));
		memset (&paging_addr, 0, sizeof(struct sockaddr_in));
	    paging_addr.sin_family = AF_INET;
	    paging_addr.sin_port = htons(PAGE_CALL_PORT);
	    inet_pton(AF_INET,global_ip_addr,&paging_addr.sin_addr);

	    bcopy (&paging_addr.sin_addr.s_addr, &pg_mreq.imr_multiaddr.s_addr, sizeof (struct in_addr));

	    pg_mreq.imr_interface.s_addr = inet_addr(gc_station_ip);

		if (bind(g_iPaging_fd, (struct sockaddr *) &paging_addr,sizeof (struct sockaddr_in)) == -1)
	    {
	        printf( "Bind error in g_iPaging_fd---------- \n");
	       // return;
	    }
	    else{
            printf("Bind Success---------------\n");
	    }

	   /* if (setsockopt(g_iPaging_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &pg_mreq,sizeof (struct ip_mreq)) == -1)
	    {
	        printf( "Join Group  Failed\n");
			//break;
	    }*/

	     if (setsockopt(g_iPaging_fd, IPPROTO_IP, IP_MULTICAST_LOOP, &loop,sizeof (loop)) == -1)
	    {
	        printf( "Join Group %d Failed", i);
			//break;
	    }

		pWork.pagingFdDB[GLOBAL_OFFSET] = g_iPaging_fd;
		pWork.pagingFdDBRealSize++;
		printf("pwork.pagingFdDB[%d]=%d ,pWork.pagingFdDBRealSize=%d  \n",100,pWork.pagingFdDB[100],pWork.pagingFdDBRealSize);
        if (g_iPaging_fd > pWork.pagingRtpMaxFd)
            pWork.pagingRtpMaxFd = g_iPaging_fd;
        FD_SET(g_iPaging_fd, &pWork.pagingRtpReceivingFds);
        sprintf(buf,"route add -net %s netmask 255.255.255.255 eth0 \n",global_ip_addr);
       system(buf);

}




void drop_membership(int fd)
{
///added
    if(fd==g_iPaging_fd)
    {
        g_iGlobal_mem_dropped=1;

        if (setsockopt(fd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &pg_mreq,sizeof (struct ip_mreq)) == -1)
        {
	        printf( "Join Global  Failed\n");
			//break;
        }
         else{ printf("GLOBAL MEMBERSHIP DROPPED\n");}
    }


    if(fd==g_iAim_fds[stn_config.default_AIM-1])
    {
         g_iAim_mem_dropped=1;

     if (setsockopt(fd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &aim_mreq[stn_config.default_AIM-1],sizeof (struct ip_mreq)) == -1)
	    {
	        perror( "AIM drop Group  Failed");
			//break;
	    }
	    else{printf("AIM MEMBERSHIP DROPPED\n");}
   }

    if(fd==g_Iicom_fds[pvt_call.curr_icom_no]){
        g_iIcom_mem_dropped=1;

        if (setsockopt(fd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &icom_mreq[pvt_call.curr_icom_no],sizeof (struct ip_mreq)) == -1)
	    {
	        perror( "ICOM drop Group  Failed");
			//break;
	    }
	    else{ printf("ICOM MEMBERSHIP DROPPED\n");}
    }

    if(fd==g_igroup_fds[pvt_call.group_no]){
    //g_iIcom_mem_dropped=0;

        if (setsockopt(fd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &group_mreq[pvt_call.group_no],sizeof (struct ip_mreq)) == -1)
	    {
	        perror( "GROUP add Group  Failed\n");
			//break;
	    }
	    else{ printf("GROUP dropped MEMBERSHIP \n");}
    }


    if(fd==g_izone_fds[pg_call.zone_no]){
    //g_iIcom_mem_dropped=0;

        if (setsockopt(fd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &zone_mreq[pg_call.zone_no],sizeof (struct ip_mreq)) == -1)
	    {
	        perror( "zone add Group  Failed\n");
			//break;
	    }
	    else{ printf("zone dropped MEMBERSHIP \n");}
    }

}


void add_membership(int fd)
{
///added
    printf("////////////////////////add_membership(int fd)\n");
    if(fd==g_iPaging_fd)
    {
        printf("////////////////////////inside fd match\n");
        g_iGlobal_mem_dropped=0;

            ///getsockopt(int sockfd, int level, int optname,void *optval, socklen_t *optlen);
        if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &pg_mreq,sizeof (struct ip_mreq)) == -1)
        {
	        printf( "Join Global  Failed\n");
			//break;
        }
         else{ printf("GLOBAL MEMBERSHIP ADDED\n");}
    }
    if(fd==g_iAim_fds[stn_config.default_AIM-1])
    {
        g_iAim_mem_dropped=0;

     if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &aim_mreq[0],sizeof (struct ip_mreq)) == -1)
	    {
	        perror( "AIM add Group  Failed\n");
			//break;
	    }
	    else{ printf("AIM MEMBERSHIP ADDED\n");}
   }

    if((fd==g_Iicom_fds[pvt_call.curr_icom_no]) && (stn_config.icom_live_dead_status[pvt_call.curr_icom_no] ==LIVE)){
    g_iIcom_mem_dropped=0;

        if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &icom_mreq[pvt_call.curr_icom_no],sizeof (struct ip_mreq)) == -1)
	    {
	        perror( "ICOM add Group  Failed\n");
			//break;
	    }
	    else{ printf("icom MEMBERSHIP ADDED\n");}
    }

    if(fd==g_igroup_fds[pvt_call.group_no]){
    //g_iIcom_mem_dropped=0;

        if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &group_mreq[pvt_call.group_no],sizeof (struct ip_mreq)) == -1)
	    {
	        perror( "GROUP add Group  Failed\n");
			//break;
	    }
	    else{  printf("GROUP MEMBERSHIP ADDED\n");}
    }

    if(fd==g_izone_fds[pg_call.zone_no]){
    //g_iIcom_mem_dropped=0;

        printf("ZOne fd is %d       %d",g_izone_fds[pg_call.zone_no],fd);

        if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &zone_mreq[pg_call.zone_no],sizeof (struct ip_mreq)) == -1)
	    {
	        perror( "zone add Group  Failed\n");
			//break;
	    }
	    else{  printf("zone MEMBERSHIP ADDED\n");}
    }

}

void paging_received_packet(int group)
{
    RTPHeader *rtpHeader = (RTPHeader *)pWork.pagingBuff;
    if (rtpHeader->PT != pConfig.payload)
    {
       // printf("RTP Header payload not matching \n");
      //  return;
    }

    pWork.currentSSRC = rtpHeader->SSRC;


}


void paging_send_packet(char* buff, unsigned int len,int channel)
{
    if(channel==PVT_CHANNEL)
    {
    if (pWork_m.private_state == PRIVATE_OUTGOING_ONGOING)
    {
        int						rc;

    	socklen_t				sockAddrSize;
    	struct sockaddr_in		clientAddr;   /* one for active and one for conf */
        int port;
        char buf[30];
    	char rtp_addr[30];
    if(g_itransmit_fd_index==GLOBAL_OFFSET || (g_itransmit_fd_index>=ZONE_OFFSET && g_itransmit_fd_index < AIM_OFFSET))
    	{
            port=PAGE_CALL_PORT;//stn_config.def_aim_port;
           // sprintf(buf,"%s%d",gc_global_mul_start_addr,g_iglobal_mul_addr_start_offset);
           strcpy(rtp_addr,g_cMulticast_tx_address);
           // strcpy(rtp_addr,buf);
             printf("IF rtp addr %s\n",rtp_addr);
    	}
    else {
            port=PVT_CALL_PORT;//stn_config.def_icom_port;

            strcpy(rtp_addr,g_cMulticast_tx_address);
            printf("Else rtp addr %s\n",rtp_addr);
    	}


    	sockAddrSize = sizeof(struct sockaddr_in);

    	bzero((char *)&(clientAddr), sockAddrSize);
    	clientAddr.sin_family = AF_INET;
    	clientAddr.sin_port = htons(port);
    	clientAddr.sin_addr.s_addr = inet_addr(rtp_addr);

    	if ((0 == clientAddr.sin_port) || (0 == clientAddr.sin_addr.s_addr))
    		return;

    	rc = sendto(pWork.pagingFdDB[g_itransmit_fd_index],
                   	buff,
    				len,
    				0,
    				(struct sockaddr *)&(clientAddr),
    				sockAddrSize);
        if(rc<0)
        {
            perror("SENDING PACKET FAILED\n");
        }


    	return;

    }
    }
}



int paging_is_paging_now()
{
    return 0;//(pWork.page_state != PAGE_IDLE);
}
///added
void p2p_dsp_stop_connection(int channel)
{

	close_channel(channel);
}

void p2p_dsp_start_connection(int channel)
{
	int payload;
	payload=RTP_PAYLOAD_G722;
	 setchannelparamters_Phi(channel,payload,PVT_PTIME,0,0);
}

void multicast_prepare_dsp(int channel)
{
    if(channel==PVT_CHANNEL && g_uiDiagnostic_start_flag!=SET)
    {
      pConfig.payload=RTP_PAYLOAD_G722;
      printf("Inside G722 codec PVT Channel...\n");
    }
    else if((pg_call.type==GLOBAL_ALARM) || (pg_call.type==ZONE_ALARM) ||(g_uiDiagnostic_start_flag==SET))
    {
      pConfig.payload=RTP_PAYLOAD_PCMU;
        printf("Inside PCMU codec...\n");
    }
    else
    {
      pConfig.payload=RTP_PAYLOAD_G722;
        printf("Inside else G722 codec...\n");
    }
    setchannelparamters_Phi(channel,pConfig.payload,PAGING_PTIME,0,0);
}





void stop_alarm(int alarm_no)
{

 printf("PLAyback ended \n");
 acgPlaybackEnd(PG_CHANNEL);

}

void start_alarm(int alarm_no)
{
    char* File_name;
    switch(alarm_no)
    {
        case 10:
            File_name = DIAGNOSTIC_TONE;
        break;

        case 11:
            File_name = EMERGENCY_ALARM;
        break;

        case 12:
            File_name = CUT0;
        break;

        case 13:
            File_name = CUT1;
        break;

        case 14:
            File_name = CUT2;
        break;

        case 15:
            File_name = CUT3;
        break;

        case 16:
            File_name = CUT4;
        break;

        case 17:
            File_name = CUT5;
        break;

        case 18:
            File_name = CUT6;
        break;

        default:
        break;


    }
    acgPlaybackStart(PG_CHANNEL,0,4,File_name);

}

void close_channel(channel)
{
    printf("deselecting channel %d \n",channel);
     //acl_rtp_unbind(channel);
    acgDeactivateRTP(channel);
  //  acgCloseChannel(channel, &ChannelParameters);
     acgCloseChannel(channel);//, &ChannelParameters);


}

void acl_init_paging()
{
   // paging_check_config();

        create_icom_socket();
        create_group_socket();
        create_zone_socket();
        create_aim_socket();
        crate_global_paging_socket();
/*if(g_uiConfig_Received == CLEAR)
        {*/
          create_paging_thread();
       // }

}


