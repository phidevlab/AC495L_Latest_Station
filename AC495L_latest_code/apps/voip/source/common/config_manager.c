
/**********************************************************************
 *                                                                    *
 * File Name:	config_manager.c                                      *
 *
 * Author: S.K. SHINDE                                                *
 *
 * Copyright (c) by Phi Audiocom Systems Pvt. Ltd.                    *
 * This software is copyrighted by and is the sole property of        *
 * Phi Audiocom Systems Pvt. Ltd.                                     *
 *
 * Release : 4 MAR 2018                                               *
 *                                                                    *
 **********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <pty.h>
#include <utmp.h>
#include <pthread.h>
#include <linux/input.h>
#include <time.h>
#include <poll.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <linux/route.h>
#include <sched.h>
#include <linux/sockios.h>
#include <netinet/if_ether.h>
#include "config_manager.h"
#include "server_coms.h"
#include "ui.h"
#include "voip_main.h"
//#include "general_defs.h"

unsigned short gui_ip_address[FOUR];
unsigned short gui_mac_id[SIX];


char gc_network_type[TEN];
char gc_station_ip[TWENTY];
char gc_subnetmask[TWENTY];
char gc_gateway[TWENTY];
char gc_mac[TWENTY];


/*******************************************************************************************
 * Function name: set_static_user_nw_config                                                *
 * Input arg    : NONE                                                                     *
 * Output arg   : NONE                                                                     *
 * Description  : This function reads ip address from file and apply to the station        *
 *                                                                                         *
********************************************************************************************/

int set_static_user_nw_config()
{

    char str[MAX_BUFFER_SIZE];
    sprintf(str,"ifconfig eth0 %s",gc_station_ip);
    printf("Setting ip address  %s\n",str);
    system(str);

	/*int sockfd;
	struct ifreq ifr;
	struct sockaddr_in sin;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd == -1){
		fprintf(stderr, "Could not get socket.\n");
		return 0;
	}

	/* get interface name *
	strncpy(ifr.ifr_name, "br-lan", IFNAMSIZ);

	/* Read interface flags *
	if (ioctl(sockfd, SIOCGIFFLAGS, &ifr) < 0) {
		fprintf(stderr, "ifdown: shutdown ");
		perror(ifr.ifr_name);
		close(sockfd);
		return 0;
	}

	/*
	* Expected in <net/if.h> according to
	* "UNIX Network Programming".
	*
	#ifdef ifr_flags
	# define IRFFLAGS       ifr_flags
	#else   /* Present on kFreeBSD *
	# define IRFFLAGS       ifr_flagshigh
	#endif

	// If interface is down, bring it up
	if (ifr.IRFFLAGS | ~(IFF_UP)) {
		fprintf(stdout, "Device is currently down..setting up.-- %u\n",ifr.IRFFLAGS);
		ifr.IRFFLAGS |= IFF_UP;
		if (ioctl(sockfd, SIOCSIFFLAGS, &ifr) < 0) {
			fprintf(stderr, "ifup: failed ");
			perror(ifr.ifr_name);
			close(sockfd);
			return 0;
		}
	}

	sin.sin_family = AF_INET;

	// Convert IP from numbers and dots to binary notation
	inet_aton(gc_station_ip,(struct in_addr *)&sin.sin_addr.s_addr);
	memcpy(&ifr.ifr_addr, &sin, sizeof(struct sockaddr));

	// Set interface address
	if (ioctl(sockfd, SIOCSIFADDR, &ifr) < 0) {
		fprintf(stderr, "Cannot set IP address. ");
		perror(ifr.ifr_name);
		close(sockfd);
		return 0;
	}

	// Convert NETMASK from numbers and dots to binary notation
	inet_aton(gc_subnetmask,(struct in_addr *)&sin.sin_addr.s_addr);
	memcpy(&ifr.ifr_addr, &sin, sizeof(struct sockaddr));

	// Set interface address
	if (ioctl(sockfd, SIOCSIFNETMASK, &ifr) < 0) {
		fprintf(stderr, "Cannot set Netmask address. ");
		perror(ifr.ifr_name);
		close(sockfd);
		return 0;
	}

	// Convert Gaterway from numbers and dots to binary notation
	inet_aton(gc_gateway,(struct in_addr *)&sin.sin_addr.s_addr);
	memcpy(&ifr.ifr_addr, &sin, sizeof(struct sockaddr));

	// Set interface address
	if (ioctl(sockfd, SIOCSIFDSTADDR, &ifr) < 0) {
		fprintf(stderr, "Cannot set Gateway address. ");
		perror(ifr.ifr_name);
		close(sockfd);
		return 0;
	}

	#undef IRFFLAGS

	close(sockfd);

	sleep(1);*/


	return 1;
}

/*******************************************************************************************
 * Function name: update_fcs_config_file_backup                                            *
 * Input arg    : NONE                                                                     *
 * Output arg   : NONE                                                                     *
 * Description  : This function reads file and saves information in backup file            *
 *                                                                                         *
********************************************************************************************/

int update_fcs_config_file_backup(void)
{
	int i = 0;
	char ch = 0;
	char str[MAX_BUFFER_SIZE];
	FILE *source, *target;

	source = fopen(CONFIG_FILE_1, FL_RD);
	if( source == NULL )
	{
		return -1;
	}

	target = fopen(CONFIG_FILE_2, FL_WR);
	if( target == NULL )
	{
		fclose(source);
		return -1;
	}

	while ( 1 )
	{
		ch = fgetc(source);

		if ( ch == 0xFFU)
			return -2;
		else if ( ch == EOF)
			break ;
		else if(ch == END_OF_LINE){
			fputc(ch, target);

			if((strncmp(FCS_ID_STRING, &str[0], strlen(FCS_ID_STRING))) == 0){
                i = 0;
				bzero(&str, sizeof(str));
			}else if((strncmp(NW_TYPE_STRING, &str[0], strlen(NW_TYPE_STRING))) == 0){
				sprintf(gc_network_type, "%.6s", &str[strlen(NW_TYPE_STRING)] );
				i = 0;
				bzero(&str, sizeof(str));
			}else if((strncmp(NW_IP_STRING, &str[0], strlen(NW_IP_STRING))) == 0){
				sprintf(gc_station_ip, "%.15s", &str[strlen(NW_IP_STRING)] );
				i = 0;
				bzero(&str, sizeof(str));
			}else if((strncmp(NW_SN_STRING, &str[0], strlen(NW_SN_STRING))) == 0){
				sprintf(gc_subnetmask, "%.15s", &str[strlen(NW_SN_STRING)] );
				i = 0;
				bzero(&str, sizeof(str));
			}else if((strncmp(NW_GW_STRING, &str[0], strlen(NW_GW_STRING))) == 0){
				sprintf(gc_gateway, "%.15s", &str[strlen(NW_GW_STRING)] );
				i = 0;
			bzero(&str, sizeof(str));
            }/*else if((strncmp(NW_MAC_STRING, &str[0], strlen(NW_MAC_STRING))) == 0){
				sprintf(gc_mac, "%.17s", &str[strlen(NW_MAC_STRING)] );
				gc_mac[18] = '\0';
				i = 0;
				bzero(&str, sizeof(str));
				break;
			}*/

		}else{
			fputc(ch, target);
			str[i]=ch;
			i++;
			str[i] = '\0';
			ch = 0;
		}
	}



	printf("File updated successfully.\n");

	fclose(source);
	fclose(target);

	return 0;
}

/*******************************************************************************************
 * Function name: copy_fcs_config_file                                                     *
 * Input arg    : NONE                                                                     *
 * Output arg   : NONE                                                                     *
 * Description  : This function reads file and saves fcs id also copy in the backup file   *
 *                                                                                         *
********************************************************************************************/
int copy_fcs_config_file(void)
{
	int i = 0;
	char ch = 0;
	char str[MAX_BUFFER_SIZE];
	FILE *source, *target;

	source = fopen(CONFIG_FILE_1, FL_RD);
	if( source == NULL )
	{
		return -1;
	}

	target = fopen(CONFIG_FILE_2, FL_WR);
	if( target == NULL )
	{
		fclose(source);
		return -1;
	}

	while ( 1 )
	{
		ch = fgetc(source);
		if ( ch == EOF)
			break ;
		else if(ch == END_OF_LINE){
			fputc(ch, target);

			if((strncmp(FCS_ID_STRING, &str[0], strlen(FCS_ID_STRING))) == 0){
                i = 0;
				bzero(&str, sizeof(str));
			}else if((strncmp(NW_TYPE_STRING, &str[0], strlen(NW_TYPE_STRING))) == 0){
				sprintf(gc_network_type, "%.6s", &str[strlen(NW_TYPE_STRING)] );
				i = 0;
				bzero(&str, sizeof(str));
			}else if((strncmp(NW_IP_STRING, &str[0], strlen(NW_IP_STRING))) == 0){
				sprintf(gc_station_ip, "%.15s", &str[strlen(NW_IP_STRING)] );
				i = 0;
				bzero(&str, sizeof(str));
			}else if((strncmp(NW_SN_STRING, &str[0], strlen(NW_SN_STRING))) == 0){
				sprintf(gc_subnetmask, "%.15s", &str[strlen(NW_SN_STRING)] );
				i = 0;
				bzero(&str, sizeof(str));
			}else if((strncmp(NW_GW_STRING, &str[0], strlen(NW_GW_STRING))) == 0){
				sprintf(gc_gateway, "%.15s", &str[strlen(NW_GW_STRING)] );
				i = 0;

			bzero(&str, sizeof(str));

			}/*else if((strncmp(NW_MAC_STRING, &str[0], strlen(NW_MAC_STRING))) == 0){
				sprintf(gc_mac, "%.17s", &str[strlen(NW_MAC_STRING)] );
				mac[18] = '\0';
				i = 0;
				bzero(&str, sizeof(str));
				break;
			}*/

	}
	else{
			fputc(ch, target);
			str[i]=ch;
			i++;
			str[i] = '\0';
			ch = 0;
            }
    }

	printf("File copied successfully.\n");

	fclose(source);
	fclose(target);

	return 0;
}

/*******************************************************************************************
 * Function name: get_station_config                                                       *
 * Input arg    : NONE                                                                     *
 * Output arg   : NONE                                                                     *
 * Description  : This function reads file and saves network paramters                     *
 *                                                                                         *
********************************************************************************************/

int get_station_config (void)
{
	int i = 0,test,fcs_id;
	char ch = 0;
	char str[MAX_BUFFER_SIZE];
	FILE *fp;
	char read_fcsid[EIGHT];
	char delims[]=".";
	char delim[]=":";
	char *result=NULL;

	fp = fopen (CONFIG_FILE_1,FL_RD) ;
	if ( !fp )
	{
		printf("Cannot open fcs id file") ;
		return -1;
	}

	bzero(&str, sizeof(str));
	bzero(&read_fcsid, sizeof(read_fcsid));

	while ( 1 )
	{
		ch = fgetc (fp) ;

        if ( ch == EOF )
			break ;
		else if ( ch == END_OF_LINE){
			if((strncmp(FCS_ID_STRING, &str[0], strlen(FCS_ID_STRING))) == 0){
				sprintf(read_fcsid, "%.4s", &str[strlen(FCS_ID_STRING)] );
				i = 0;
				bzero(&str, sizeof(str));
				printf("comparing fcd id string %d \n",read_fcsid);

			}else if((strncmp(NW_TYPE_STRING, &str[0], strlen(NW_TYPE_STRING))) == 0){
				sprintf(gc_network_type, "%.6s", &str[strlen(NW_TYPE_STRING)] );
				i = 0;
				bzero(&str, sizeof(str));
			}else if((strncmp(NW_IP_STRING, &str[0], strlen(NW_IP_STRING))) == 0){
				sprintf(gc_station_ip, "%.15s", &str[strlen(NW_IP_STRING)] );
				i = 0;
				bzero(&str, sizeof(str));
			}else if((strncmp(NW_SN_STRING, &str[0], strlen(NW_SN_STRING))) == 0){
				sprintf(gc_subnetmask, "%.15s", &str[strlen(NW_SN_STRING)] );
				i = 0;
				bzero(&str, sizeof(str));
			}else if((strncmp(NW_GW_STRING, &str[0], strlen(NW_GW_STRING))) == 0){
				sprintf(gc_gateway, "%.15s", &str[strlen(NW_GW_STRING)] );
				i = 0;
			bzero(&str, sizeof(str));

			}/*else if((strncmp(NW_MAC_STRING, &str[0], strlen(NW_MAC_STRING))) == 0){
				sprintf(gc_mac, "%.17s", &str[strlen(NW_MAC_STRING)] );
				gc_mac[18] = '\0';
				i = 0;
				bzero(&str, sizeof(str));
				break;
			}*/

        }else{
			str[i]=ch;
			i++;
			str[i] = '\0';
			ch = 0;

		}


	}





        strcat(gc_station_ip,".");
        printf("my ip is.....%s\n",gc_station_ip);
        result = strtok( gc_station_ip, delims );
        while( result != NULL ) {
        printf( "result is \"%s\"\n", result );
        test = atoi(result);
        gui_ip_address[i++] = test;
        printf("%i\n",gui_ip_address[i-1]);

        result = strtok( NULL, delims );
        }

         printf("fcs id read from memory is %s\n",read_fcsid );
        if(atoi(read_fcsid)>0){
		fcs_id = atoi(read_fcsid);
        stn_config.logical_id=fcs_id;
		printf("fcs id read from memory is %i--\n",fcs_id);
		return 0;
        }
        else
        {
            return -2;
        }






	fclose(fp);
}

/*******************************************************************************************
 * Function name: set_current_time_date                                                    *
 * Input arg    : char *                                                                   *
 * Output arg   : NONE                                                                     *
 * Description  : Set time and date according received confiugration                       *
 *                                                                                         *
********************************************************************************************/

int set_current_time_date(char *buf)
{

	char str[MAX_BUFFER_SIZE];

	bzero(str,sizeof(str));
	sprintf(str, "%s %s", SET_TIME_CMD, buf);
	system(str);
	/*	popen creates a pipe so we can read the output
		of the program we are invoking */


}

/*******************************************************************************************
 * Function name: init_config_manager                                                      *
 * Input arg    : char *                                                                   *
 * Output arg   : NONE                                                                     *
 * Description  : Initializes config manager parameters                                    *
 *                                                                                         *
********************************************************************************************/

void init_config_manager()
{
	int fl_rd_status = 0;
	int fl_copy_status = 0;


    printf("IN config manager \n");
	fl_rd_status = get_station_config();
	printf("fl_rd_status=%d\n",fl_rd_status);
	if(fl_rd_status == -2 || fl_rd_status == -1)
	{
		fl_copy_status = copy_fcs_config_file();
		printf("config file read failed \n");
    }
	if(fl_rd_status == 0)
	{
		update_fcs_config_file_backup();

		 set_static_user_nw_config();
    }

}


