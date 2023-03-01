
/**********************************************************************
 *                                                                    *
 * File Name:	server_coms.c                                           *
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
#include <fcntl.h>
#include <termios.h>
#include <pty.h>
#include <utmp.h>
#include <pthread.h>
#include <linux/input.h>
#include <time.h>
#include <poll.h>
#include <sys/time.h>
#include <sched.h>

#include "voip_main.h"
#include "server_coms.h"
#include "config_manager.h"
//#include "voipapp.h"
//#include "general_defs.h"


//extern int Call_cmdid;
struct sockaddr_in mul_sig_addr;
struct server_reg_bcast
{
    short int ipr[FOUR];
    short int port_no;
    short int size;

};

/*struct header{
    short int marker[4];
    short int version;
    short int length;

}header;*/

//int       system_size;
int       g_iIcom_mul_addr_start_offset;
int       g_igroup_mul_addr_start_offset;
int       g_iglobal_mul_addr_start_offset;
int       g_izone_mul_addr_start_offset;
int       g_iaim_mul_addr_start_offset;

int destinationid;
char delim[]="$";
//char mul_sinalling_ip[30];
char       gc_icom_mul_start_addr[TWENTY];
char       gc_group_mul_start_addr[TWENTY];
char       gc_global_mul_start_addr[TWENTY];
char       gc_zone_mul_start_addr[TWENTY];
char       gc_aim_mul_start_addr[TWENTY];



//static int server_tcp_port_no = CLEAR;
static int gSi_mul_singalling_port = CLEAR;

/*******************************************************************************************
 * Function name: save_mul_starting_address                                                *
 * Input arg    : void                                                                     *
 * Output arg   : NONE                                                                     *
 * Description  : This function saves multicast address received from configuration        *
 *                                                                                         *
********************************************************************************************/

void save_mul_starting_address()
{
    int i_cntr=ZERO;
    int i_k=ZERO;
    char *c_res=NULL;
    char c_del[] = ".";
    char c_buf1[THIRTY];
    char c_buf2[THIRTY];
    char c_buffer[THIRTY];


for(i_cntr=ZERO;i_cntr<SEVEN;i_cntr++)
{
          switch(i_cntr)
          {
            case ZERO: c_res = strtok(stn_config.icom_multicast_start_addr, c_del);
                    bzero(c_buffer,sizeof(c_buffer));
                    while(c_res != NULL)
                    {
                        printf("RES IS %d , %s \n",i_k,c_res);
                        switch(i_k)
                        {
                            case ZERO:strcpy(c_buffer,c_res); strcat(c_buffer,".");
                             printf("buffer0 %s\n",c_buffer); break;
                            case ONE:strcat(c_buffer,c_res); strcat(c_buffer,".");
                            printf("buffer1 %s\n",c_buffer); break;
                            case TWO:strcat(c_buffer,c_res); strcat(c_buffer,".");
                            strcpy(gc_icom_mul_start_addr,c_buffer);
                                   printf("gc_icom_mul_start_addr %s\n",
                                   gc_icom_mul_start_addr);break;
                            case THREE:g_iIcom_mul_addr_start_offset=atoi(c_res);
                                   printf("g_iIcom_mul_addr_start_offset %d\n"
                                   ,g_iIcom_mul_addr_start_offset);
                            break;
                            default: printf("default\n");break;
                        }
                        c_res = strtok( NULL, c_del);
                        i_k++;
                    }


            break;

            case ONE:
                    c_res = strtok(stn_config.group_multicast_start_addr, c_del);
                    bzero(c_buffer,sizeof(c_buffer));
                    i_k=ZERO;
                    while(c_res != NULL)
                    {
                        printf("RES IS %d , %s \n",i_k,c_res);
                        switch(i_k)
                        {
                            case ZERO:strcpy(c_buffer,c_res); strcat(c_buffer,".");
                             printf("buffer0 %s\n",c_buffer); break;
                            case ONE:strcat(c_buffer,c_res); strcat(c_buffer,".");
                            printf("buffer1 %s\n",c_buffer); break;
                            case TWO:strcat(c_buffer,c_res); strcat(c_buffer,".");
                            strcpy(gc_group_mul_start_addr,c_buffer);
                                   printf("gc_group_mul_start_addr %s\n"
                                   ,gc_group_mul_start_addr);break;
                            case THREE:g_igroup_mul_addr_start_offset=atoi(c_res);
                                   printf("g_igroup_mul_addr_start_offset %d\n"
                                   ,g_igroup_mul_addr_start_offset);
                            break;
                            default: printf("default\n");break;
                        }
                        c_res = strtok( NULL, c_del);
                        i_k++;
                    }


            break;
            case TWO:
                    c_res = strtok(stn_config.zone_multicast_start_addr, c_del);
                    bzero(c_buffer,sizeof(c_buffer));
                    i_k=ZERO;
                    while(c_res != NULL)
                    {
                        printf("RES IS %d , %s \n",i_k,c_res);
                        switch(i_k)
                        {
                            case ZERO:strcpy(c_buffer,c_res); strcat(c_buffer,".");
                             printf("buffer0 %s\n",c_buffer); break;
                            case ONE:strcat(c_buffer,c_res); strcat(c_buffer,".");
                            printf("buffer1 %s\n",c_buffer); break;
                            case TWO:strcat(c_buffer,c_res); strcat(c_buffer,".");
                             strcpy(gc_zone_mul_start_addr,c_buffer);
                                   printf("gc_zone_mul_start_addr %s\n"
                                   ,gc_zone_mul_start_addr);break;
                            case THREE:g_izone_mul_addr_start_offset=atoi(c_res);
                                   printf("g_izone_mul_addr_start_offset %d\n"
                                   ,g_izone_mul_addr_start_offset);
                            break;
                            default: printf("default\n");break;
                        }
                        c_res = strtok( NULL, c_del);
                        i_k++;
                    }

            break;
           case THREE:
                   c_res = strtok(stn_config.aim_multicast_start_addr, c_del);
                    bzero(c_buffer,sizeof(c_buffer));
                    i_k=ZERO;
                    while(c_res != NULL)
                    {
                        printf("RES IS %d , %s \n",i_k,c_res);
                        switch(i_k)
                        {
                            case ZERO:strcpy(c_buffer,c_res); strcat(c_buffer,".");
                            printf("buffer0 %s\n",c_buffer); break;
                            case ONE:strcat(c_buffer,c_res); strcat(c_buffer,".");
                            printf("buffer1 %s\n",c_buffer); break;
                            case TWO:strcat(c_buffer,c_res); strcat(c_buffer,".");
                            strcpy(gc_aim_mul_start_addr,c_buffer);
                            printf("gc_aim_mul_start_addr %s\n",gc_aim_mul_start_addr);break;
                            case THREE:g_iaim_mul_addr_start_offset=atoi(c_res);
                                   printf("g_iaim_mul_addr_start_offset %d\n"
                                   ,g_iaim_mul_addr_start_offset);
                            break;
                            default: printf("default\n");break;
                        }
                        c_res = strtok( NULL, c_del);
                        i_k++;
                    }


           break;
           case FOUR: c_res = strtok(stn_config.global_multicast_start_addr, c_del);
                    bzero(c_buffer,sizeof(c_buffer));
                    i_k=ZERO;
                    while(c_res != NULL)
                    {
                        printf("RES IS %d , %s \n",i_k,c_res);
                        switch(i_k)
                        {
                            case ZERO:strcpy(c_buffer,c_res); strcat(c_buffer,".");
                             printf("buffer0 %s\n",c_buffer); break;
                            case ONE:strcat(c_buffer,c_res); strcat(c_buffer,".");
                            printf("buffer1 %s\n",c_buffer); break;
                            case TWO:strcat(c_buffer,c_res); strcat(c_buffer,".");
                            strcpy(gc_global_mul_start_addr,       c_buffer);
                                   printf("gc_global_mul_start_addr %s\n"
                                   ,gc_global_mul_start_addr);break;
                            case THREE:g_iglobal_mul_addr_start_offset=atoi(c_res);
                                   printf("g_iglobal_mul_addr_start_offset %d\n"
                                   ,g_iglobal_mul_addr_start_offset);
                            break;
                            default: printf("default\n");break;
                        }
                        c_res = strtok( NULL, c_del);
                        i_k++;
                    }


        break;
        case FIVE: c_res = strtok(stn_config.icom_contributor_details, c_del);
                bzero(c_buffer,sizeof(c_buffer));
                i_k=ZERO;
                while(c_res != NULL)
                {
                        printf("contributor res string is %d , %s \n",i_k,c_res);
                        switch(i_k)
                        {
                         default:
                         sprintf(c_buf1,"%c%c%c",*(c_res+ZERO),*(c_res+ONE),*(c_res+TWO));
                         sprintf(c_buf2,"%c%c%c",*(c_res+THREE),*(c_res+FOUR),*(c_res+FIVE));
                          pthread_mutex_lock(&lock_icom_contr);
                         icom_contributor_status_info[i_k].cont1=atoi(c_buf1);
                         icom_contributor_status_info[i_k].cont2=atoi(c_buf2);

                         printf("icom_contributor_status_info[%d].cont1=%d",
                         i_k,icom_contributor_status_info[i_k].cont1);
                         printf("icom_contributor_status_info[%d].cont2=%d\n",
                         i_k,icom_contributor_status_info[i_k].cont2);
                           pthread_mutex_unlock(&lock_icom_contr);
                         break;

                        }
                        c_res = strtok( NULL, c_del);
                        i_k++;
                }


          break;
          case SIX:
                c_res = strtok(stn_config.direct_call_keys_info   , c_del);
                bzero(c_buffer,sizeof(c_buffer));
                i_k=ZERO;
                while(c_res != NULL)
                {
                        printf("dir_key info res string IS %d , %s \n",i_k,c_res);
                        switch(i_k)
                        {
                         default:
                            dir_info[i_k].call_type[0]=*(c_res+0);
                           //sprintf(dir_info[k].call_type[0],"%c",*(res+0));
                        printf("dir_info[%d].call_type=%c\n",i_k,dir_info[i_k].call_type[0]);
                           switch(dir_info[i_k].call_type[0])
                           {
                             case 'P':  //P2P call
                                     dir_info[i_k].g_cCaller[0]=*(c_res+ONE);
                                     dir_info[i_k].g_cCaller[1]=*(c_res+TWO);
                                     dir_info[i_k].g_cCaller[2]=*(c_res+THREE);
                                     printf("P2P dir_info[%d].g_cCaller=%s\n",i_k,
                                     dir_info[i_k].g_cCaller);
                             break;

                             case 'G':  //GROUP CALL
                                     dir_info[i_k].number[0]=*(c_res+ONE);
                                     dir_info[i_k].number[1]=*(c_res+TWO);
                                     printf(" GROUP dir_info[%d].number=%s\n",i_k,
                                     dir_info[i_k].number);
                             break;

                             case 'Z': //ZONE CALL
                                     dir_info[i_k].number[0]=*(c_res+ONE);
                                     dir_info[i_k].number[1]=*(c_res+TWO);
                                     printf(" ZONE call dir_info[%d].number=%s\n",i_k,
                                     dir_info[i_k].number);
                             break;

                             case 'A': //GLOBAL ALARM
                                     dir_info[i_k].number[0]=*(c_res+ONE);
                                     dir_info[i_k].number[1]=*(c_res+TWO);
                                     printf("GLOBAL ALARM dir_info[%d].number=%s\n",
                                     i_k,dir_info[i_k].number);
                             break;

                             case 'I': //INTERCOM
                                     dir_info[i_k].number[0]=*(c_res+ONE);
                                     dir_info[i_k].number[1]=*(c_res+TWO);
                                     printf("Intercom No dir_info[%d].number=%s\n",
                                     i_k,dir_info[i_k].number);

                             case 'L': //GLOBAL PAGE
                                     printf("GLOBAL PAGE dir_info[%d]=\n",i_k);
                                    //sprintf(buf1,"%c%c",*(res+1),*(res+2));
                                    //dir_info[k].number=atoi(buf1);
                             break;

                             case 'E':
                                    dir_info[i_k].number[0]=*(c_res+ONE);
                                    dir_info[i_k].number[1]=*(c_res+TWO);
                                    dir_info[i_k].number[2]=*(c_res+THREE);
                                    dir_info[i_k].number[3]=*(c_res+FOUR);
                                     printf(" ZONE alarm call dir_info[%d].number=%s\n",i_k,
                                     dir_info[i_k].number);
                             break;

                           }

                          break;

                       }
                        c_res = strtok( NULL, c_del);
                        i_k++;
                }

            break;
          }



}
}


/*******************************************************************************************
 * Function name: parse_broadcast_commands                                                           *
 * Input arg    : int , void*                                                              *
 * Output arg   : NONE                                                                     *
 * Description  : This function extracts broadcast frames received from server and generated approriate events                                                                                     *
 *                                                                                         *
********************************************************************************************/

void parse_broadcast_commands (int cmd_id, void* data)
{

 struct server_coms_param_registration_bcast_t *reg=NULL;
 char  c_str[TWENTY],c_str1[TWENTY];


 switch(cmd_id)
 {

        case SERVER_REG_BCAST:

            reg = (struct server_coms_param_registration_bcast_t *) data;
            bzero(c_str,sizeof(c_str));
            bzero(c_str1,sizeof(c_str1));

            sprintf(c_str1,"%d.%d.%d.%d",
			reg->mul_sig_ip[0]& 0xff,
			reg->mul_sig_ip[1]& 0xff,
			reg->mul_sig_ip[2]& 0xff,
			reg->mul_sig_ip[3]& 0xff);
            gSi_mul_singalling_port = reg->port;
            sprintf(c_str, "%d.%d.%d.%d",
			reg->ip[0] & 0xff,
			reg->ip[1] & 0xff,
			reg->ip[2] & 0xff,
			reg->ip[3] & 0xff);


            strcpy(g_cMul_signalling_ip, c_str1);

         //   printf("port is,......................................%i\n",gSi_mul_singalling_port);
            strcpy(g_cServer_addr, c_str);

         //   printf("the received ipaddr is %s %s\n",str,str1);

            g_uiFsu_Broadcast_event=SERVER_REG_BCAST_EVENT;


            break;

            case WAKE_UP_FRAME:
             printf("cmd_id in parse_commands is WAKE_UP_FRAME....\n");
             g_uiFsu_Broadcast_event=WAKE_UP_EVENT;
            break;

            case SHUTDOWN_FRAME:
          //   printf("cmd_id in parse_commands is SHUTDOWN_EVENT....\n");
             g_uiFsu_Broadcast_event=SHUTDOWN_EVENT;
            break;

            default:
            printf("Inside default of SERVER_REG_BCAST\n");
            break;
 }

}



/*******************************************************************************************
 * Function name: parse_multicast_commands                                                           *
 * Input arg    : int , void*                                                              *
 * Output arg   : NONE                                                                     *
 * Description  : This function extracts multicast frames received from server and generated approriate events                                                                                     *
 *                                                                                         *
********************************************************************************************/

void parse_multicast_commands (int cmd_id, void* data)
{
    struct server_coms_global_call *gbl=NULL;
    struct server_coms_group_call_status *grp=NULL;
    struct server_coms_icom_break_in_group_frame *icom_grp=NULL;
    struct server_coms_icom_call *icom=NULL;
    struct server_coms_icom_cont_status *icom_stat=NULL;
    struct server_coms_icom_start_stop_frame *icom_start_stop=NULL;
     struct server_coms_global_alarm *alm=NULL;
     struct server_coms_zone_call_status *zone=NULL;
 struct server_coms_zone_alarm	*zone_alm=NULL;

    int   i_cntr=ZERO,i_j=ZERO,i_k=ZERO;
    switch(cmd_id)
    {
         case INCOMING_GLOBAL_CALL:
            gbl = (struct server_coms_global_call *) data;
            incoming_global_call.cmd=gbl->cmd;
            incoming_global_call.initiator_id=gbl->initiator_id;
              printf("GLOBAL INCOMING_GLOBAL_CALL FRAME RECEIVED %d \n",
              incoming_global_call.initiator_id);
            if(incoming_global_call.initiator_id==stn_config.logical_id)
             g_uiFsu_mcast_event = GLOBAL_CALL_EVENT;
            else
             g_uiFsu_mcast_event = GLOBAL_RX_CALL_EVENT;


        break;

        case GLOBAL_RESET:
             gbl = (struct server_coms_global_call *) data;
             incoming_global_call.cmd=gbl->cmd;
             incoming_global_call.initiator_id=gbl->initiator_id;
             printf("GLOBAL RESET EVENT FRAME RECEIVED %d \n",incoming_global_call.initiator_id);
             if(incoming_global_call.initiator_id==stn_config.logical_id)
             {
                g_uiFsu_mcast_event=GLOBAL_RESET_EVENT;
                printf("**************  INSIDE GLOBAL_RESET_EVENT\n");
              }
             else
             g_uiFsu_mcast_event = GLOBAL_RESET_RX_EVENT;

        break;

        case GROUP_INCOMING_CALL:
         printf("GROUP_INCOMING_CALL\n");

             i_cntr=ZERO;
             grp = (struct server_coms_group_call_status *) data;
             printf("GROUP_no %d\ngrp.initiator_stn%d\n",grp->group_no,grp->initiator_stn);
             lcd_group_call_init = grp->initiator_stn;
             lcd_group_call_no = grp->group_no;


             if((stn_config.group_member_info[grp->group_no-MIN_OFFSET]=='1') || ((stn_config.group_initiator[grp->group_no-MIN_OFFSET]=='1')&&(RINGBACK_STATE==g_uiFsu_state)))
             {
                 pvt_call.group_no = grp->group_no-MIN_OFFSET;
                 pthread_mutex_lock(&lock_group_contr);
                 group_contributor_status_info[pvt_call.group_no].cont1=grp->cont1;
                 group_contributor_status_info[pvt_call.group_no].cont2=grp->cont2;
                 pthread_mutex_unlock(&lock_group_contr);
                 printf("cont1 %d \n",server_coms_group_call_info[pvt_call.group_no].cont1=grp->cont1);
                 printf("cont2 %d \n",server_coms_group_call_info[pvt_call.group_no].cont1=grp->cont2);
                 g_uiFsu_mcast_event=INCOMING_GROUP_CALL_EVENT;
             }
             else if((stn_config.group_initiator[grp->group_no-MIN_OFFSET]=='1')&&(RINGBACK_STATE==g_uiFsu_state))
             {
             pvt_call.group_no = grp->group_no-MIN_OFFSET;
              pthread_mutex_lock(&lock_group_contr);
             group_contributor_status_info[pvt_call.group_no].cont1=grp->cont1;
             group_contributor_status_info[pvt_call.group_no].cont2=grp->cont2;
			 pthread_mutex_unlock(&lock_group_contr);
             printf("cont1 %d \n",server_coms_group_call_info[pvt_call.group_no].cont1=grp->cont1);
             printf("cont2 %d \n",server_coms_group_call_info[pvt_call.group_no].cont1=grp->cont2);
             g_uiFsu_event=INCOMING_GROUP_CALL_EVENT;

             }

        break;


        case GROUP_CONT_STATUS_UPDATE:
             i_cntr=ZERO;
             grp = (struct server_coms_group_call_status *) data;
             i_cntr=grp->group_no-MIN_OFFSET;
             if(grp->initiator_stn==stn_config.logical_id)
             {
             	printf("PTTT RESPONSE EVENT genrerated\n");
             	if(grp->response==CLEAR)
             	{
                  g_uiFsu_mcast_event=GRP_PTT_REQUEST_EVENT;
                }
                else
                {
                  g_uiFsu_mcast_event=GRP_PTT_RELEASE_EVENT;
                }
             }
              pthread_mutex_lock(&lock_group_contr);
             group_contributor_status_info[i_cntr].cont1=grp->cont1;
             group_contributor_status_info[i_cntr].cont2=grp->cont2;

             printf("server_coms_group_call_info[%d].cont2= %d\n",i_cntr,group_contributor_status_info[i_cntr].cont2);
             pthread_mutex_unlock(&lock_group_contr);
        break;

        case GROUP_INTERCOM_BREAK_IN_FRAME:
              printf("***********************INSIDE GROUP_INTERCOM_BREAK_IN_FRAME\n");
            icom_grp=(struct server_coms_icom_break_in_group_frame *) data;

             for(i_cntr=ZERO;i_cntr<THIRTY_TWO;i_cntr++)
             {
              // j=i;

                printf("............icom_grp->contributor_list[i_j] %d\n",icom_grp->contributor_list[i_j]);
               icom_contributor_status_info[i_cntr].cont1=icom_grp->contributor_list[i_j];
               i_j++;
                printf(".............icom_grp->contributor_list[i_j] %d\n",icom_grp->contributor_list[i_j]);
               icom_contributor_status_info[i_cntr].cont2=icom_grp->contributor_list[i_j];
               i_j++;
               printf("ICOM[%d]  cont1=%d cont2=%d\n",i_cntr+MIN_OFFSET
               , icom_contributor_status_info[i_cntr].cont1,
                icom_contributor_status_info[i_cntr].cont2);

             }


        break;


        case GROUP_RESET:
             grp = (struct server_coms_group_call_status *) data;
           /*  if(stn_config.group_member_info[grp->group_no-MIN_OFFSET]=='1')
             {
				g_uiFsu_event=GROUP_RESET_EVENT;
			 }
			 else if((stn_config.group_initiator[grp->group_no-MIN_OFFSET]=='1')&&(GROUP_STATE==g_uiFsu_state))
			 {
			 	g_uiFsu_event=GROUP_RESET_EVENT;
			 }*/
			printf("Group no %d grp  no %d \n",pvt_call.group_no,grp->group_no);
			if((pvt_call.group_no+MIN_OFFSET) == grp->group_no)
			{
                printf("GROUP_RESET_EVENT is set \n");
				g_uiFsu_mcast_event=GROUP_RESET_EVENT;
			}
             printf("GROUP_RESET EVENT received \n");
        break;

        case ICOM_CONT_STATUS_UPDATE:
             i_cntr=ZERO;
             icom_stat=(struct server_coms_icom_cont_status *) data;
              i_cntr=icom_stat->icom_no-MIN_OFFSET;
            printf("icom-stat initiator %d %d \n",icom_stat->initiator_stn,i_cntr);
             if(icom_stat->initiator_stn==stn_config.logical_id)
             {
                printf("PTTT RESPONSE EVENT genrerated\n");
                g_uiFsu_mcast_event=PTT_RESPONSE_EVENT;
             }

               pthread_mutex_lock(&lock_icom_contr);
             icom_contributor_status_info[i_cntr].cont1=icom_stat->cont1;

             icom_contributor_status_info[i_cntr].cont2=icom_stat->cont2;
               printf("icom_contributor_status_info[%d].cont1= %d\n",i_cntr,icom_contributor_status_info[i_cntr].cont1);
               pthread_mutex_unlock(&lock_icom_contr);

             printf("............... g_uiFsu_state :%d \n",g_uiFsu_state);

			if(g_uiFsu_state == IDLE_STATE || g_uiFsu_state==INTERCOM_STATE)
			{

				if((i_cntr==pvt_call.curr_icom_no) &&
               ((icom_contributor_status_info[i_cntr].cont1!=CLEAR) ||
               (icom_contributor_status_info[i_cntr].cont2 != CLEAR)))
               {

                   printf("Inside IDLE state of ICOM\n");
                 if(g_iHandset_lifted_signal==CLEAR)
                 {
                    printf("g_iHandset_lifted_signal is CLEAR.......\n");
                    operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_OFF);
                    operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                 }
               }
               else
               {
               	if(i_cntr==pvt_call.curr_icom_no && g_iHandset_lifted_signal==CLEAR)
               	{
                     printf("In else of ICOM state ......\n");
					operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
					if(stn_config.default_AIM)
					{
						operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
					}
					else
					{
						operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
					}
                }
               }
			}

        break;

        case INTERCOM_STATE_CHANGE_FRAME:

             printf("***********************INSIDE INTERCOM_STATE_CHANGE_FRAME\n");
             icom_start_stop = (struct server_coms_icom_start_stop_frame *) data;
             for(i_cntr=ZERO;i_cntr<THIRTY_TWO;i_cntr++)
             {
                if(icom_start_stop->icom_no[i_cntr]>ZERO)
                {

                    if(stn_config.icom_live_dead_status[icom_start_stop->icom_no[i_cntr]-
                    MIN_OFFSET==LIVE])
                    {
                         stn_config.icom_live_dead_status[icom_start_stop->icom_no[i_cntr]
                         -MIN_OFFSET] = DEAD;
                          if(stn_config.default_intercom==icom_start_stop->icom_no[i_cntr])
                          {
                           g_uiFsu_mcast_event=ICOM_STATE_CHANGE_EVENT;
                          }
                    }
                    else
                    {
                      stn_config.icom_live_dead_status[icom_start_stop->icom_no[i_cntr]-
                      MIN_OFFSET] = LIVE;
                       if((stn_config.default_intercom)==icom_start_stop->icom_no[i_cntr])
                       {
                        add_membership(g_Iicom_fds[stn_config.default_intercom-MIN_OFFSET]);
                       }

                    }

            icom_contributor_status_info[icom_start_stop->icom_no[i_cntr]-MIN_OFFSET].cont1=CLEAR;
            icom_contributor_status_info[icom_start_stop->icom_no[i_cntr]-MIN_OFFSET].cont2=CLEAR;

                }
             }
        break;

        case INCOMING_GLOBAL_ALARM:

            alm = (struct server_coms_global_alarm *) data;
            incoming_global_alm_call.cmd=alm->cmd;
            incoming_global_alm_call.initiator_id=alm->initiator_id;
            incoming_global_alm_call.alarm_no=alm->alarm_no;

            pg_call.alm_no=alm->alarm_no;
             printf("INCOMING_GLOBAL_ALARM FRAME received %d \n",incoming_global_alm_call.initiator_id);

             lcd_global_alm_init = incoming_global_alm_call.initiator_id;
             lcd_global_alm_no = incoming_global_alm_call.alarm_no=alm->alarm_no;

            if(incoming_global_alm_call.initiator_id==stn_config.logical_id)
            g_uiFsu_mcast_event = GLOBAL_ALM_EVENT;
            else
            g_uiFsu_mcast_event = GLOBAL_RX_ALM_EVENT;


        break;
        case ZONE_INCOMING_CALL:
            printf("inside ZONE_INCOMING_CALL**************\n");
			zone = (struct server_coms_zone_call_status *) data;
			printf("fcs id is: %d, alarmid : %d, zone no: %d ", zone->initiator_id, zone->alarm_module_id,zone->zone_no);
			lcd_zone_call_init = zone->initiator_id;
			lcd_zone_call_no = zone->zone_no;
			printf("Inside zone call %d",zone->zone_no);
						printf("Inside zone call --2 %d",lcd_zone_call_no);

            //lcd_zone_call_no_show = zone->zone_no;

			if(stn_config.zone_initiator[zone->zone_no-MIN_OFFSET]=='1')
			{
                printf("inside ZONE_INCOMING_CALL_if_1**************\n");
			    if(g_uiFsu_state==RINGBACK_STATE)
            	{
                    printf("inside ZONE_INCOMING_CALL_if_2**************\n");
					pg_call.zone_no = zone->zone_no-MIN_OFFSET;
            		g_uiFsu_event=INCOMING_ZONE_CALL_EVENT;
            		break;
				}
			}

           if(stn_config.zone_member_info[zone->zone_no-MIN_OFFSET]=='1')
            {

                pg_call.zone_no = zone->zone_no-MIN_OFFSET;
                printf("ZOne no is %d \n",pg_call.zone_no);
                g_uiFsu_mcast_event = INCOMING_ZONE_CALL_RX_EVENT;

			}
        break;

        case ZONE_ALM_INCOMING_CALL:
			zone_alm = (struct server_coms_zone_alarm *) data;
            printf("zone alarm no %d zone alarm no %d \n",pg_call.zone_no,zone_alm->zone_no);
            lcd_zone_alm_init = zone_alm->initiator_id;
            lcd_alm_zone_no = zone_alm->zone_no;

			if(stn_config.zone_initiator[zone_alm->zone_no-MIN_OFFSET]=='1')
			{
			    if(g_uiFsu_state==RINGBACK_STATE)
            	{
					pg_call.zone_no = zone_alm->zone_no-MIN_OFFSET;
					pg_call.alm_no  = zone_alm->alarm_no;
                    g_uiFsu_event = INCOMING_ZONE_ALM_EVENT;
                    break;

				}
			}

            if(stn_config.zone_member_info[zone_alm->zone_no-MIN_OFFSET]=='1')
            {

				pg_call.zone_no = zone_alm->zone_no-MIN_OFFSET;
				pg_call.alm_no  = zone_alm->alarm_no;
                g_uiFsu_mcast_event = INCOMING_ZONE_ALM_RX_EVENT;

			}
        break;

        case ZONE_RESET:
             zone = (struct server_coms_zone_call_status *) data;

			printf("zone no %d zone  no %d \n",pg_call.zone_no,zone->zone_no);

			/* if((pg_call.zone_no == zone->zone_no-MIN_OFFSET) &&
                        ((pg_call.pg_call_busy == SET) || (pvt_call.pvt_call_busy == SET)))
                        {
                printf("Inside if(pg_call.zone_no == zone->zone_no-MIN_OFFSET");
              if((stn_config.zone_initiator[zone->zone_no-MIN_OFFSET]=='1') || (g_uiFsu_state==PAGE_RESET_RINGBACK_STATE))
                {
                    printf("Initiator ZONE_RESET_EVENT is set\n");
                     g_uiFsu_event = ZONE_RESET_EVENT;
                }
                else
                {
                    printf("Receiver ZONE_RESET_RX_EVENT is set\n");
                     g_uiFsu_mcast_event = ZONE_RESET_RX_EVENT;
                }
                        }*/

                if((pg_call.zone_no == zone->zone_no-MIN_OFFSET) &&
			((pg_call.pg_call_busy == SET) || (pvt_call.pvt_call_busy == SET)))
			{
                printf("Inside if(pg_call.zone_no == zone->zone_no-MIN_OFFSET\n");
                if((stn_config.zone_initiator[zone->zone_no-MIN_OFFSET]=='1') || (stn_config.zone_member_info[zone->zone_no-MIN_OFFSET]=='1'))
                {
                    printf("Inside zone_initiator || zone_member\n");
                    if((stn_config.zone_initiator[zone->zone_no-MIN_OFFSET]=='1') && ((g_uiFsu_state == PAGE_INITIATOR_STATE) || (g_uiFsu_state ==PAGE_RESET_RINGBACK_STATE)))
                    {
                        printf("Initiator ZONE_RESET_EVENT is set\n");
                        g_uiFsu_event = ZONE_RESET_EVENT;
                    }
                    else
                    {
                        printf("Receiver ZONE_RESET_RX_EVENT is set\n");
                        g_uiFsu_mcast_event = ZONE_RESET_RX_EVENT;

                    }

                }
            }
		/*	if(stn_config.zone_member_info[zone->zone_no-MIN_OFFSET]=='1' )
			{
                //if(g_uiFsu_state==PAGE_RESET_RINGBACK_STATE)// originally added by shraddha madam

                 printf("zone->initiator_id%d\n",zone->initiator_id);
                 //reseted by time out
                 if((g_uiFsu_state==PAGE_INITIATOR_STATE) && (pg_call.zone_no==zone->zone_no) )
                   {
                     printf("inside g_uiFsu_state==PAGE_INITIATOR_STATE\n");
                     g_uiFsu_event = ZONE_RESET_EVENT;
                   }
                    //reseted by reset key
                  else if(g_uiFsu_state==PAGE_RESET_RINGBACK_STATE)
                   {
                    printf("inside g_uiFsu_state==PAGE_RESET_RINGBACK_STATE \n");
                    g_uiFsu_event = ZONE_RESET_EVENT;
                   }
                  else//zone reset for receiver fcs which will handle in background.
                   {
                    g_uiFsu_mcast_event = ZONE_RESET_RX_EVENT;
                   }
            }
            if(stn_config.zone_initiator[zone->zone_no-MIN_OFFSET]=='1' && stn_config.zone_member_info[zone->zone_no-MIN_OFFSET]=='0')
			{
                printf("Checking zone initiator\n");

                    if((g_uiFsu_state==PAGE_INITIATOR_STATE) && (pg_call.zone_no == zone->zone_no))
                   {
                     printf("inside g_uiFsu_state==PAGE_INITIATOR_STATE\n");
                     g_uiFsu_event = ZONE_RESET_EVENT;
                   }
                    //reseted by reset key
                  else if(g_uiFsu_state==PAGE_RESET_RINGBACK_STATE)
                   {
                    printf("**********inside g_uiFsu_state==PAGE_RESET_RINGBACK_STATE************ \n");
                    g_uiFsu_event = ZONE_RESET_EVENT;
                   }
                   else//zone reset for receiver fcs which will handle in background.
                   {
                    g_uiFsu_mcast_event = ZONE_RESET_RX_EVENT;
                   }

			} */
             printf("ZONE_RESET event %dreceived \n",g_uiFsu_event);
        break;

         default :
         printf("error in parse command default\n");

    }
}
/*******************************************************************************************
 * Function name: parse_commands                                                           *
 * Input arg    : int , void*                                                              *
 * Output arg   : NONE                                                                     *
 * Description  : This function extracts frames received from server and generated approriate events                                                                                     *
 *                                                                                         *
********************************************************************************************/

void parse_commands (int cmd_id, void* data)
{

// struct server_coms_param_registration_bcast_t *reg=NULL;
 //struct server_coms_global_alarm *alm=NULL;
 //struct server_coms_global_call *gbl=NULL;
 //struct server_coms_icom_call *icom=NULL;
 //struct server_coms_icom_cont_status *icom_stat=NULL;
 //struct server_coms_group_call_status *grp=NULL;
 //struct server_coms_zone_call_status *zone=NULL;
 //struct server_coms_zone_alarm	*zone_alm=NULL;
 struct server_coms_group_call_reset_individual *grp_rst=NULL;
 //struct server_coms_icom_start_stop_frame *icom_start_stop=NULL;
 //struct server_coms_icom_break_in_group_frame *icom_grp=NULL;
 struct server_coms_diagnostics_frame *diag=NULL;
 struct station_configuration  *cfg=NULL;
 //struct server_coms_ack_response_frame *ack_response=NULL;

 short *ptr=NULL;
 char  c_str[TWENTY],c_str1[TWENTY];
 char  *result=NULL;

 char  c_del[] = ".";
 char  c_delim[] = "$";
 int   i_cntr=ZERO,i_j=ZERO,i_k=ZERO ;


	switch(cmd_id)
	{

       /* case SERVER_REG_BCAST:

            reg = (struct server_coms_param_registration_bcast_t *) data;
            bzero(c_str,sizeof(c_str));
            bzero(c_str1,sizeof(c_str1));

            sprintf(c_str1,"%d.%d.%d.%d",
			reg->mul_sig_ip[0]& 0xff,
			reg->mul_sig_ip[1]& 0xff,
			reg->mul_sig_ip[2]& 0xff,
			reg->mul_sig_ip[3]& 0xff);
            gSi_mul_singalling_port = reg->port;
            sprintf(c_str, "%d.%d.%d.%d",
			reg->ip[0] & 0xff,
			reg->ip[1] & 0xff,
			reg->ip[2] & 0xff,
			reg->ip[3] & 0xff);


            strcpy(g_cMul_signalling_ip, c_str1);

         //   printf("port is,......................................%i\n",gSi_mul_singalling_port);
            strcpy(g_cServer_addr, c_str);

         //   printf("the received ipaddr is %s %s\n",str,str1);

            g_uiFsu_event=SERVER_REG_BCAST_EVENT;


            break;*/

       /* case WAKE_UP_FRAME:
             printf("cmd_id in parse_commands is WAKE_UP_FRAME....\n");
             g_uiFsu_event=WAKE_UP_EVENT;
        break;*/

        case FCS_RESTART_FRAME:
             printf("*********cmd_id in parse_commands is FCS_RESTART_FRAME....\n");
             g_uiFsu_event=WAKE_UP_EVENT;
        break;

        case NEG_RESPONSE_FRAME_CMD:
             g_uiFsu_event=NEG_RES_EVENT;
        break;


        case INITIAL_CONFIGURATION_DOWNLOAD:

        printf("config received %s'''''''''''''\n",data);
        result=strtok(data,c_delim);

        while(result != NULL)
        {
            printf("result %d is %s\n",i_cntr,result);
            switch(i_cntr)
            {

                case ONE:
                    stn_config.data_length=atoi(result);
                    printf("data_length %d\n",stn_config.data_length);
                break;

                case TWO:
                    stn_config.cmd_id=atoi(result);
                    printf("stn_config.cmd_id %d\n",stn_config.cmd_id);
                break;

                case THREE:
                    stn_config.logical_id=atoi(result);
                    printf("stn_config.logical_id %d\n",stn_config.logical_id);
                break;

                case FOUR:
                    for(i_j=ZERO;i_j<EIGHTEEN;i_j++)
                    {
                        stn_config.fcs_desc[i_j]=*(result+i_j);
                    }
                    printf("fcs_desc %s\n",stn_config.fcs_desc);
                    //lcd_fcs_desc = stn_config.fcs_desc;
                break;

                case FIVE:
                    stn_config.fcs_password=atoi(result);
                    printf("fcs password is %d\n",stn_config.fcs_password);
                break;

                case SIX:
                    stn_config.default_intercom=atoi(result);
                     printf("stn_config.default_intercom is %d\n",stn_config.default_intercom);
                     lcd_fcs_default_icom = stn_config.default_intercom;
                break;

             /*   case 7:
                    for(j=0;j<32;j++){
                    stn_config.def_icom_rtp_addr[j]=*(result+j);

                    }
                    printf("stn_config.def_icom_rtp_addr %s\n",stn_config.def_icom_rtp_addr);
                break;*/

              /*  case 8:
                    stn_config.def_icom_port=atoi(result);
                    printf("stn_config.def_icom_port %d\n",stn_config.def_icom_port);

                break;*/
                case SEVEN:
                    stn_config.default_AIM=atoi(result);
                     printf("stn_config.default_AIM is %d\n",stn_config.default_AIM);
                break;

            /*    case 10:
                    for(j=0;j<32;j++){
                    stn_config.def_aim_rtp_addr[j]=*(result+j);

                    }
                     printf("stn_config.def_aim_rtp_addr %s\n",stn_config.def_aim_rtp_addr);
                break;*/

               /* case 11:
                    stn_config.def_aim_port=atoi(result);
                    printf("stn_config.def_aim_port %d\n",stn_config.def_aim_port);
                break;*/

                case EIGHT:
                for(i_j=ZERO;i_j<TWENTY;i_j++){
                    stn_config.icom_multicast_start_addr[i_j]=*(result+i_j);
                    }

                break;

                case NINE:
                for(i_j=ZERO;i_j<TWENTY;i_j++){
                    stn_config.group_multicast_start_addr[i_j]=*(result+i_j);
                    }


                break;

                case TEN:
                for(i_j=ZERO;i_j<TWENTY;i_j++){
                    stn_config.zone_multicast_start_addr[i_j]=*(result+i_j);

                    }

                break;


                case ELEVEN:
                for(i_j=ZERO;i_j<TWENTY;i_j++){
                    stn_config.aim_multicast_start_addr[i_j]=*(result+i_j);
                }

                break;

                case TWELVE:
                for(i_j=ZERO;i_j<TWENTY;i_j++){
                    stn_config.global_multicast_start_addr[i_j]=*(result+i_j);

                    }

                break;


               /* case 17:
                    stn_config.mic_min=atoi(result);
                    printf("mic_min IS %d\n",stn_config.mic_min);
                    break;

                case 18:
                    stn_config.mic_max=atoi(result);
                    printf("mic_max IS %d\n",stn_config.mic_max);
                     break;
                case 19:
                    stn_config.pvt_min=atoi(result);
                    printf("pvt_min IS %d\n",stn_config.pvt_min);
                   break;
                case 20:
                    stn_config.pvt_max=atoi(result);
                    printf("pvt_max IS %d\n",stn_config.pvt_max);
                   break;

                case 21:
                   stn_config.pg_min=atoi(result);
                    printf("pg_min IS %d\n",stn_config.pg_min);
                   break;

                case 22:
                    stn_config.pg_max=atoi(result);
                    printf("pg_max IS %d\n",stn_config.pg_max);
                   break;*/
                case THIRTEEN:
                    stn_config.music_min=atoi(result);
                    printf("music_min ID IS %d\n",stn_config.music_min);
                   break;
                case FOURTEEN:
                    stn_config.music_max=atoi(result);

                    printf("music_max ID IS %d\n",stn_config.music_max);
                    break;

                case FIFTEEN:
                    stn_config.call_permission_channel_hop=atoi(result);
                    printf("stn_config.call_permission_channel_hop %d\n",
                    stn_config.call_permission_channel_hop);
                    break;

                case SIXTEEN:
                    stn_config.call_permission_group_call=atoi(result);
                    printf("stn_config.call_permission_group_call %d\n",
                    stn_config.call_permission_group_call);
                    break;

                case SEVENTEEN:
                    stn_config.call_permission_zone_call=atoi(result);
                    printf("call_permission_zone_call IS %d\n",
                    stn_config.call_permission_zone_call);
                  break;

                case EIGHTEEN:
                    stn_config.call_permission_global_call=atoi(result);
                    printf("call_permission_global_call IS %d\n",
                    stn_config.call_permission_zone_call);
                   break;

                case NINETEEN:
                    stn_config.call_permission_zone_alarm_call=atoi(result);
                    printf("call_permission_zone_alarm_call IS %d\n",
                    stn_config.call_permission_zone_call);
                   break;

                case TWENTY:
                    stn_config.call_permission_global_alarm_call=atoi(result);
                    printf("call_permission_global_alarm_call IS %d\n",
                    stn_config.call_permission_global_alarm_call);
                    break;


                case TWENTY_ONE:
                    stn_config.call_permission_internal_epabx=atoi(result);
                    printf("stn_config.call_permission_internal_epabx %d\n",
                     stn_config.call_permission_internal_epabx);
                    break;

                case TWENTY_TWO:
                    stn_config.call_permission_external_epabx=atoi(result);
                    printf("stn_config.call_permission_external_epabx %d\n",
                    stn_config.call_permission_external_epabx);
                    break;


               /* case 33:
                    stn_config.comm_mode_type=atoi(result);
                    printf("communication mode type  %d\n",stn_config.comm_mode_type);
                    break;*/

                case TWENTY_THREE:
                    for(i_j=ZERO;i_j<THIRTY_TWO;i_j++){
                    stn_config.icom_live_dead_status[i_j]=*(result+i_j);
                    }
                     printf("stn_config.icom_live_dead_status %s\n",
                     stn_config.icom_live_dead_status);
                   break;

                case TWENTY_FOUR:
                    for(i_j=ZERO;i_j<MAX_ICOM_DETAILS_SIZE;i_j++){
                    stn_config.icom_contributor_details[i_j]=*(result+i_j);
                    }
                    printf("stn_config.icom contributor details %s\n"
                    ,stn_config.icom_contributor_details);
                    break;


               /* case 36:
                    stn_config.no_of_configured_groups=atoi(result);
                    printf("stn_config.no_of_configured_groups %d\n",stn_config.no_of_configured_groups);
                   break;*/

                case TWENTY_FIVE:
                  for(i_j=ZERO;i_j<THIRTY_TWO;i_j++){
                    stn_config.group_initiator[i_j]=*(result+i_j);

                    }
                      printf("stn_config.group_initiator %s\n",stn_config.group_initiator);
                    break;

                case TWENTY_SIX:
                  for(i_j=ZERO;i_j<THIRTY_TWO;i_j++){
                    stn_config.group_member_info[i_j]=*(result+i_j);

                    }
                    printf("stn_config.group_member_info %s\n",stn_config.group_member_info);
                    break;

               /* case 39:
                    stn_config.no_of_configured_zones=atoi(result);
                    printf("no_of_configured_zones %d\n",stn_config.no_of_configured_zones);
                   break;*/

                case TWENTY_SEVEN:
                    for(i_j=ZERO;i_j<THIRTY_TWO;i_j++){
                    stn_config.zone_initiator[i_j]=*(result+i_j);

                    }
                    printf("stn_config.zone_initiator %s\n",stn_config.zone_initiator);
                   break;

                case TWENTY_EIGHT:
                    for(i_j=ZERO;i_j<THIRTY_TWO;i_j++){
                    stn_config.zone_member_info[i_j]=*(result+i_j);

                    }
                     printf("stn_config.zone_member_info %s\n",stn_config.zone_member_info);
                    break;

                case TWENTY_NINE:
                    for(i_j=ZERO;i_j<THIRTY_TWO;i_j++)
                    {
                        stn_config.direct_call_key_details[i_j]=*(result+i_j);

                    }
                     printf("direct_call_key_details %s\n",stn_config.direct_call_key_details);
                   break;

                 case THIRTY:
                    for(i_j=ZERO;i_j<MAX_DIRECT_CALL_KEY_INFO_SIZE;i_j++)
                    {
                        stn_config.direct_call_keys_info[i_j]=*(result+i_j);
                    }
                    printf("direct_call_keys_info %s\n",stn_config.direct_call_keys_info);
                   break;

                case THIRTY_ONE:
                    stn_config.recordflag=atoi(result);
                    printf(" record flag is %d\n",stn_config.recordflag);
                    break;

                case THIRTY_TWO:
                    stn_config.pvt_call_timeout=atoi(result);
                    stn_config.pvt_call_timeout = stn_config.pvt_call_timeout * APP_TIMER_MULTIPLIER;
                    printf("pvt_call_timeout is %d\n",stn_config.pvt_call_timeout);
                    break;

                case THIRTY_THREE:
                    stn_config.pg_call_timeout=atoi(result);
                    stn_config.pg_call_timeout=   stn_config.pg_call_timeout * APP_TIMER_MULTIPLIER;
                    printf("pg_call_timeout IS %d\n",stn_config.pg_call_timeout);
                    break;


                case THIRTY_FOUR:
                    stn_config.alarm_call_timeout=atoi(result);
                    printf("pg_call_timeout IS %d\n",stn_config.alarm_call_timeout);
                    break;

                case THIRTY_FIVE:
                    stn_config.dial_key_lim_internal=atoi(result);;
                    printf("digit in internal epbx IS %d\n",stn_config.dial_key_lim_internal);
                    break;

                case THIRTY_SIX:
                    stn_config.dial_key_lim_external=atoi(result);
                    printf("digit in external epbx %d\n",stn_config.dial_key_lim_external);
                    break;

              /*  case 50:
                    stn_config.c8_mode=atoi(result);
                    printf("C8 mode is %d\n",stn_config.c8_mode);
                    break;*/

                case THIRTY_SEVEN:
                    for(i_j=ZERO;i_j<THIRTY_TWO;i_j++)
                    {
                        stn_config.timetick[i_j]=*(result+i_j);
                    }
                   // set_current_time_date(stn_config.timetick);
                    printf("timetick %s\n",stn_config.timetick);
                    break;

               /* case 38:
                  for(j=0;j<11;j++)
                    {
                        char s[12];
                        s[j]=*(result+j);
                        stn_config.Reserved[j]=atoi(&s[j]);
                        printf("Reserved %d\n",stn_config.Reserved[j]);
                    }
                    break;
                */
                case THIRTY_EIGHT:

                  for(i_j=ZERO;i_j<THREE;i_j++)
                    {
                        stn_config.footer[i_j]=*(result+i_j);
                        printf("footer %c\n",stn_config.footer[i_j]);
                    }
                    printf("stn_config.footer==%s\n",stn_config.footer);
                    if(strcmp(&stn_config.footer,"End")==ZERO)
                    {
                        g_uiFsu_event=CONFIG_RECEIVED;
                    }
                    break;

                default:printf("in default\n");
                    break;
            }
            result = strtok( NULL, c_delim);
		    i_cntr++;
        }

        break;

        case ACK_RESPONSE_FRAME:
            printf("ACK_RESPONSE_FRAME  received....\n");
            g_uiFsu_event=ACK_RESPONSE;
        break;


        case P2P_CALL_REQUEST:
            printf("P2P_CALL_REQUEST Frame  received....%d\n",g_igroup_fds[0]);
           // result = "192.168.1.135";
             result=strtok(data,c_delim);
            while(result!=NULL)
            {
                switch(i_cntr)
                {
                case FOUR:
                    sprintf(pvt_call.initiator_ip,"%s",result);
                    printf("InitiatorIP=%s groupfd is : %d\n",pvt_call.initiator_ip, g_igroup_fds[0]);
                    printf(" groupfd is : %d\n",g_igroup_fds[0]);
                break;

                }
             result = strtok( NULL, c_delim);
		    i_cntr++;
            }
             printf("group fd is ....%d\n",g_igroup_fds[0]);
            g_uiFsu_event=P2P_CALL_EVENT;
        break;

         case P2P_CALL_ACK:
            printf("P2P_CALL_ACK Frame  received....\n");
            result=strtok(data,c_delim);
            while(result!=NULL)
            {
                switch(i_cntr)
                {
                case FOUR:
                     sprintf(pvt_call.destination_ip,"%s",result);
                    printf("destinationIP=%s\n",pvt_call.destination_ip);
                break;

                }
             result = strtok( NULL, c_delim);
		    i_cntr++;
            }
             g_uiFsu_event=P2P_CALL_EVENT;
        break;

        case P2P_CALL_RESET_REQ:
            printf("P2P_CALL_RESET Frame  received\n");

            g_uiFsu_event=P2P_RESET_EVENT;
        break;

        case P2P_CALL_RESET_PVTTIMEOUT_REQ:
            printf("P2P_CALL_RESET Frame  received from server....\n");
            g_uiFsu_event=P2P_RESET_PVTTIMEOUT_EVENT;
        break;
        case P2P_CALL_RESET_ACK:
            printf("P2P_CALL_RESET_ACK Frame  received....\n");
            printf("............... g_uiFsu_state :%d \n",g_uiFsu_state);
            g_uiFsu_event=P2P_RESET_EVENT;
        break;

        case P2P_NO_RESPONSE:
            printf("P2P_NO_RESPONSE Frame  received....\n");
            g_uiFsu_event=NO_P2P_RESPONSE_EVENT;
        break;

        case P2P_BRK_CALL_RESET_REQ:
         printf("P2P_BRK_CALL_RESET_REQ Frame  received....\n");
           g_uiFsu_event=P2P_RESET_EVENT;
        break;


        /*case INCOMING_GLOBAL_ALARM:

            alm = (struct server_coms_global_alarm *) data;
            incoming_global_alm_call.cmd=alm->cmd;
            incoming_global_alm_call.initiator_id=alm->initiator_id;
            incoming_global_alm_call.alarm_no=alm->alarm_no;

            pg_call.alm_no=alm->alarm_no;
             printf("INCOMING_GLOBAL_ALARM FRAME received %d \n",incoming_global_alm_call.initiator_id);
            if(incoming_global_alm_call.initiator_id==stn_config.logical_id)
            g_uiFsu_event = GLOBAL_ALM_EVENT;
            else
            g_uiFsu_event = GLOBAL_RX_ALM_EVENT;


        break;*/

       /* case ONLINE_LOCAL_CONFIG_DOWNLOAD:
             cfg=(struct station_configuration *) data;

             stn_config.data_length        =cfg->data_length;
             stn_config.cmd_id             =cfg->cmd_id;
             stn_config.logical_id         =cfg->logical_id;
             strcpy(stn_config.fcs_desc,cfg->fcs_desc);
             stn_config.fcs_password       =cfg->fcs_password;
             stn_config.default_intercom   =cfg->default_intercom;
             stn_config.default_AIM        =cfg->default_AIM;
             strcpy(stn_config.icom_multicast_start_addr,cfg->icom_multicast_start_addr);
             strcpy(stn_config.group_multicast_start_addr,cfg->group_multicast_start_addr);
             strcpy(stn_config.zone_multicast_start_addr,cfg->zone_multicast_start_addr);
             strcpy(stn_config.aim_multicast_start_addr,cfg->aim_multicast_start_addr);
             strcpy(stn_config.global_multicast_start_addr,cfg->global_multicast_start_addr);
             stn_config.music_min                        =cfg->music_min;
             stn_config.music_max                        =cfg->music_max;
             stn_config.call_permission_channel_hop      =cfg->call_permission_channel_hop;
             stn_config.call_permission_group_call       =cfg->call_permission_group_call;
             stn_config.call_permission_zone_call        =cfg->call_permission_zone_call;
             stn_config.call_permission_global_call      =cfg->call_permission_global_call;
             stn_config.call_permission_zone_alarm_call  =cfg->call_permission_zone_alarm_call;
             stn_config.call_permission_global_alarm_call=cfg->call_permission_global_alarm_call;
             stn_config.call_permission_internal_epabx   =cfg->call_permission_internal_epabx;
             stn_config.call_permission_external_epabx   =cfg->call_permission_external_epabx;
             strcpy(stn_config.icom_live_dead_status,cfg->icom_live_dead_status);
             strcpy(stn_config.icom_contributor_details,cfg->icom_contributor_details);
             strcpy(stn_config.group_initiator,cfg->group_initiator);
             strcpy(stn_config.group_member_info,cfg->group_member_info);
             strcpy(stn_config.zone_initiator,cfg->zone_initiator);
             strcpy(stn_config.zone_member_info,cfg->zone_member_info);
             strcpy(stn_config.direct_call_key_details,cfg->direct_call_key_details);
             strcpy(stn_config.direct_call_keys_info,cfg->direct_call_keys_info);
             stn_config.recordflag                      =cfg->recordflag;
             stn_config.pvt_call_timeout                =cfg->pvt_call_timeout;
             stn_config.pg_call_timeout                 =cfg->pg_call_timeout;
             stn_config.alarm_call_timeout              =cfg->alarm_call_timeout;
             stn_config.dial_key_lim_internal           =cfg->dial_key_lim_internal;
             stn_config.dial_key_lim_external           =cfg->dial_key_lim_external;
             strcpy(stn_config.timetick,cfg->timetick);
            // set_current_time_date(stn_config.timetick);
             strcpy(stn_config.footer,cfg->footer);
        break;*/

       /* case GROUP_INCOMING_CALL:

             i_cntr=ZERO;
             grp = (struct server_coms_group_call_status *) data;
             if(stn_config.group_member_info[grp->group_no-MIN_OFFSET]=='1')
             {
             pvt_call.group_no = grp->group_no-MIN_OFFSET;
              pthread_mutex_lock(&lock_group_contr);
             group_contributor_status_info[pvt_call.group_no].cont1=grp->cont1;
             group_contributor_status_info[pvt_call.group_no].cont2=grp->cont2;
              pthread_mutex_unlock(&lock_group_contr);
             printf("cont1 %d \n",server_coms_group_call_info[pvt_call.group_no].cont1=grp->cont1);
             printf("cont2 %d \n",server_coms_group_call_info[pvt_call.group_no].cont1=grp->cont2);
             g_uiFsu_event=INCOMING_GROUP_CALL_EVENT;
             }
             else if((stn_config.group_initiator[grp->group_no-MIN_OFFSET]=='1')&&(RINGBACK_STATE==g_uiFsu_state))
             {
             pvt_call.group_no = grp->group_no-MIN_OFFSET;
              pthread_mutex_lock(&lock_group_contr);
             group_contributor_status_info[pvt_call.group_no].cont1=grp->cont1;
             group_contributor_status_info[pvt_call.group_no].cont2=grp->cont2;
			 pthread_mutex_unlock(&lock_group_contr);
             printf("cont1 %d \n",server_coms_group_call_info[pvt_call.group_no].cont1=grp->cont1);
             printf("cont2 %d \n",server_coms_group_call_info[pvt_call.group_no].cont1=grp->cont2);
             g_uiFsu_event=INCOMING_GROUP_CALL_EVENT;

             }
        break;*/

/*
        case ZONE_INCOMING_CALL:
			zone = (struct server_coms_zone_call_status *) data;
			printf("fcs id is: %d, alarmid : %d, zone no: %d ", zone->initiator_id, zone->alarm_module_id,zone->zone_no);
			if(stn_config.zone_initiator[zone->zone_no-MIN_OFFSET]=='1')
			{
			    if(g_uiFsu_state==RINGBACK_STATE)
            	{
					pg_call.zone_no = zone->zone_no-MIN_OFFSET;
            		g_uiFsu_event=INCOMING_ZONE_CALL_EVENT;
            		break;
				}
			}

           if(stn_config.zone_member_info[zone->zone_no-MIN_OFFSET]=='1')
            {

                pg_call.zone_no = zone->zone_no-MIN_OFFSET;
                printf("ZOne no is %d \n",pg_call.zone_no);
                g_uiFsu_event   = INCOMING_ZONE_CALL_RX_EVENT;

			}
        break;

        case ZONE_ALM_INCOMING_CALL:
			zone_alm = (struct server_coms_zone_alarm *) data;
            printf("zone alarm no %d zone alarm no %d \n",pg_call.zone_no,zone_alm->zone_no);
			if(stn_config.zone_initiator[zone_alm->zone_no-MIN_OFFSET]=='1')
			{
			    if(g_uiFsu_state==RINGBACK_STATE)
            	{
					pg_call.zone_no = zone_alm->zone_no-MIN_OFFSET;
					pg_call.alm_no  = zone_alm->alarm_no;
            		g_uiFsu_event=INCOMING_ZONE_ALM_EVENT;
            		break;
				}
			}

            if(stn_config.zone_member_info[zone_alm->zone_no-MIN_OFFSET]=='1')
            {

				pg_call.zone_no = zone_alm->zone_no-MIN_OFFSET;
				pg_call.alm_no  = zone_alm->alarm_no;
                g_uiFsu_event   = INCOMING_ZONE_ALM_RX_EVENT;

			}
        break;*/

       /* case GROUP_RESET:
             grp = (struct server_coms_group_call_status *) data;
           /*  if(stn_config.group_member_info[grp->group_no-MIN_OFFSET]=='1')
             {
				g_uiFsu_event=GROUP_RESET_EVENT;
			 }
			 else if((stn_config.group_initiator[grp->group_no-MIN_OFFSET]=='1')&&(GROUP_STATE==g_uiFsu_state))
			 {
			 	g_uiFsu_event=GROUP_RESET_EVENT;
			 }*/
			/*printf("Group no %d grp  no %d \n",pvt_call.group_no,grp->group_no);
			if((pvt_call.group_no+MIN_OFFSET) == grp->group_no)
			{
                printf("GROUP_RESET_EVENT is set \n");
				g_uiFsu_event=GROUP_RESET_EVENT;
			}
             printf("GROUP_RESET EVENT received \n");
        break;*/

/*
        case ZONE_RESET:
             zone = (struct server_coms_zone_call_status *) data;

			printf("zone no %d zone  no %d \n",pg_call.zone_no,zone->zone_no);
			if(stn_config.zone_member_info[zone->zone_no-MIN_OFFSET]=='1' )
			{
                //if(g_uiFsu_state==PAGE_RESET_RINGBACK_STATE)// originally added by shraddha madam

                 printf("zone->initiator_id%d\n",zone->initiator_id);
                 //reseted by time out
                 if(g_uiFsu_state==PAGE_INITIATOR_STATE)
                   {
                     printf("inside g_uiFsu_state==PAGE_INITIATOR_STATE\n");
                     g_uiFsu_event = ZONE_RESET_EVENT;
                   }
                    //reseted by reset key
                  else if(g_uiFsu_state==PAGE_RESET_RINGBACK_STATE)
                   {
                    printf("inside g_uiFsu_state==PAGE_RESET_RINGBACK_STATE \n");
                    g_uiFsu_event = ZONE_RESET_EVENT;
                   }
                  else//zone reset for receiver fcs which will handle in background.
                   {
                    g_uiFsu_event = ZONE_RESET_RX_EVENT;
                   }
            }
            if(stn_config.zone_initiator[zone->zone_no-MIN_OFFSET]=='1' && stn_config.zone_member_info[zone->zone_no-MIN_OFFSET]=='0')
			{
                printf("Checking zone initiator\n");

                    if(g_uiFsu_state==PAGE_INITIATOR_STATE)
                   {
                     printf("inside g_uiFsu_state==PAGE_INITIATOR_STATE\n");
                     g_uiFsu_event = ZONE_RESET_EVENT;
                   }
                    //reseted by reset key
                  else if(g_uiFsu_state==PAGE_RESET_RINGBACK_STATE)
                   {
                    printf("**********inside g_uiFsu_state==PAGE_RESET_RINGBACK_STATE************ \n");
                    g_uiFsu_event = ZONE_RESET_EVENT;
                   }
                   else//zone reset for receiver fcs which will handle in background.
                   {
                    g_uiFsu_event = ZONE_RESET_RX_EVENT;
                   }

			}
             printf("ZONE_RESET event %dreceived \n",g_uiFsu_event);
        break;
*/
        /*case GROUP_CONT_STATUS_UPDATE:
             i_cntr=ZERO;
             grp = (struct server_coms_group_call_status *) data;
             i_cntr=grp->group_no-MIN_OFFSET;
             if(grp->initiator_stn==stn_config.logical_id)
             {
             	printf("PTTT RESPONSE EVENT genrerated\n");
             	if(grp->response==CLEAR)
             	{
                  g_uiFsu_event=GRP_PTT_REQUEST_EVENT;
                }
                else
                {
                  g_uiFsu_event=GRP_PTT_RELEASE_EVENT;
                }
             }
              pthread_mutex_lock(&lock_group_contr);
             group_contributor_status_info[i_cntr].cont1=grp->cont1;
             group_contributor_status_info[i_cntr].cont2=grp->cont2;

             printf("server_coms_group_call_info[%d].cont2= %d\n",i_cntr,group_contributor_status_info[i_cntr].cont2);
             pthread_mutex_unlock(&lock_group_contr);
        break;*/


		case GROUP_INDIVIDUAL_RESET_FSU:
            	printf("GRoup reset individual genrerated\n");
        	    g_uiFsu_event=GRP_INDIVIDUAL_RESET_EVENT;
        break;

        /*case ICOM_CONT_STATUS_UPDATE:
             i_cntr=ZERO;
             icom_stat=(struct server_coms_icom_cont_status *) data;
              i_cntr=icom_stat->icom_no-MIN_OFFSET;
            printf("icom-stat initiator %d %d \n",icom_stat->initiator_stn,i_cntr);
             if(icom_stat->initiator_stn==stn_config.logical_id)
             {
                printf("PTTT RESPONSE EVENT genrerated\n");
                g_uiFsu_event=PTT_RESPONSE_EVENT;
             }

               pthread_mutex_lock(&lock_icom_contr);
             icom_contributor_status_info[i_cntr].cont1=icom_stat->cont1;

             icom_contributor_status_info[i_cntr].cont2=icom_stat->cont2;
               printf("icom_contributor_status_info[%d].cont1= %d\n",i_cntr,icom_contributor_status_info[i_cntr].cont1);
               pthread_mutex_unlock(&lock_icom_contr);

             printf("............... g_uiFsu_state :%d \n",g_uiFsu_state);

			if(g_uiFsu_state == IDLE_STATE || g_uiFsu_state==INTERCOM_STATE)
			{

				if((i_cntr==pvt_call.curr_icom_no) &&
               ((icom_contributor_status_info[i_cntr].cont1!=CLEAR) ||
               (icom_contributor_status_info[i_cntr].cont2 != CLEAR)))
               {

                   printf("Inside IDLE state of ICOM\n");
                 if(g_iHandset_lifted_signal==CLEAR)
                 {
                    printf("g_iHandset_lifted_signal is CLEAR.......\n");
                    operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_OFF);
                    operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
                 }
               }
               else
               {
               	if(i_cntr==pvt_call.curr_icom_no && g_iHandset_lifted_signal==CLEAR)
               	{
                     printf("In else of ICOM state ......\n");
					operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);
					if(stn_config.default_AIM)
					{
						operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF);
					}
					else
					{
						operate_gpio(MB_AMP_MUTE,WRITE,GPIO_ON);
					}
                }
               }
			}

        break;*/

        case DIAGNOSTICS_FRAME:
             diag=(struct server_coms_diagnostics_frame *) data;
             g_uiFsu_event=DIAGNOSTIC_EVENT;

        break;

        case CONF_TIMER_UPDATE_FRAME:
             g_uiFsu_event=CONF_TIMER_UPDATE_EVENT;
        break;

       /* case INCOMING_GLOBAL_CALL:
            gbl = (struct server_coms_global_call *) data;
            incoming_global_call.cmd=gbl->cmd;
            incoming_global_call.initiator_id=gbl->initiator_id;
              printf("GLOBAL INCOMING_GLOBAL_CALL FRAME RECEIVED %d \n",
              incoming_global_call.initiator_id);
            if(incoming_global_call.initiator_id==stn_config.logical_id)
             g_uiFsu_event = GLOBAL_CALL_EVENT;
            else
             g_uiFsu_event = GLOBAL_RX_CALL_EVENT;


        break;*/

        /*case GROUP_INTERCOM_BREAK_IN_FRAME:
              printf("***********************INSIDE GROUP_INTERCOM_BREAK_IN_FRAME\n");
            icom_grp=(struct server_coms_icom_break_in_group_frame *) data;

             for(i_cntr=ZERO;i_cntr<THIRTY_TWO;i_cntr++)
             {
              // j=i;

                printf("............icom_grp->contributor_list[i_j] %d\n",icom_grp->contributor_list[i_j]);
               icom_contributor_status_info[i_cntr].cont1=icom_grp->contributor_list[i_j];
               i_j++;
                printf(".............icom_grp->contributor_list[i_j] %d\n",icom_grp->contributor_list[i_j]);
               icom_contributor_status_info[i_cntr].cont2=icom_grp->contributor_list[i_j];
               i_j++;
               printf("ICOM[%d]  cont1=%d cont2=%d\n",i_cntr+MIN_OFFSET
               , icom_contributor_status_info[i_cntr].cont1,
                icom_contributor_status_info[i_cntr].cont2);

             }


        break;*/

        /*case GLOBAL_RESET:
             gbl = (struct server_coms_global_call *) data;
             incoming_global_call.cmd=gbl->cmd;
             incoming_global_call.initiator_id=gbl->initiator_id;
             printf("GLOBAL RESET EVENT FRAME RECEIVED %d \n",incoming_global_call.initiator_id);
             if(incoming_global_call.initiator_id==stn_config.logical_id)
             {
                g_uiFsu_event=GLOBAL_RESET_EVENT;
                printf("**************  INSIDE GLOBAL_RESET_EVENT\n");
              }
             else
             g_uiFsu_event = GLOBAL_RESET_RX_EVENT;

        break;*/

       /* case INTERCOM_STATE_CHANGE_FRAME:

             printf("***********************INSIDE INTERCOM_STATE_CHANGE_FRAME\n");
             icom_start_stop = (struct server_coms_icom_start_stop_frame *) data;
             for(i_cntr=ZERO;i_cntr<THIRTY_TWO;i_cntr++)
             {
                if(icom_start_stop->icom_no[i_cntr]>ZERO)
                {

                    if(stn_config.icom_live_dead_status[icom_start_stop->icom_no[i_cntr]-
                    MIN_OFFSET==LIVE])
                    {
                         stn_config.icom_live_dead_status[icom_start_stop->icom_no[i_cntr]
                         -MIN_OFFSET] = DEAD;
                          if(stn_config.default_intercom==icom_start_stop->icom_no[i_cntr])
                          {
                           g_uiFsu_event=ICOM_STATE_CHANGE_EVENT;
                          }
                    }
                    else
                    {
                      stn_config.icom_live_dead_status[icom_start_stop->icom_no[i_cntr]-
                      MIN_OFFSET] = LIVE;
                       if((stn_config.default_intercom)==icom_start_stop->icom_no[i_cntr])
                       {
                        add_membership(g_Iicom_fds[stn_config.default_intercom-MIN_OFFSET]);
                       }

                    }

            icom_contributor_status_info[icom_start_stop->icom_no[i_cntr]-MIN_OFFSET].cont1=CLEAR;
            icom_contributor_status_info[icom_start_stop->icom_no[i_cntr]-MIN_OFFSET].cont2=CLEAR;

                }
             }
        break;*/

        default:
        break;

    }
}

/*******************************************************************************************
 * Function name: send_req_msg_to_server                                                   *
 * Input arg    : int ,int ,char,char                                                      *
 * Output arg   : NONE                                                                     *
 * Description  : This function send request frames to server                              *
 *                                                                                         *
********************************************************************************************/


void send_req_msg_to_server(unsigned int fsu_state,int cmd_id, int frame_length,char offset1,char offset2)
 {
    char dial_string[TWENTY];
    char frame_message[THIRTY];
    char server_message[FIFTY];
    char p2p_call_ack_message[FIFTY];
    bzero(dial_string,sizeof(dial_string));
    bzero(frame_message,sizeof(frame_message));
    bzero(server_message,sizeof(server_message));

/****Prepare and dial instant message header***********/
     udp_g_cServer_addr.sin_family = AF_INET;
     inet_aton(g_cServer_addr,&udp_g_cServer_addr.sin_addr.s_addr);
     // udp_g_cServer_addr.sin_addr.s_addr = inet_address(g_cServer_addr);
     udp_g_cServer_addr.sin_port = htons(atoi(UNICAST_PORT));


   /* if(cmd_id!= 51)
    {
        Call_cmdid = cmd_id;
    }*/
   // printf("****** Call_cmdid = %d\n",Call_cmdid);
    sprintf(server_message,"%s$%d$%d$%d$",FRAME_HEADER,frame_length,cmd_id,stn_config.logical_id);

    switch(cmd_id)
    {

        case CONFIG_REQUEST:
            sprintf(frame_message,"%s$\n",gc_station_ip);
            strcat(server_message,frame_message);
            printf("sendig config requset message to server \n");
            printf("sendig message %s\n",server_message);

            if(sendto(g_uiServ_udp_comm_fd,server_message,strlen(server_message),ZERO,
            &udp_g_cServer_addr,
            sizeof(udp_g_cServer_addr))<ZERO)
            {
                perror("write failed \n");
            }

        break;

        case DIAGNOSTICS_RESPONSE:
            sprintf(frame_message,"%d$%d$%d$%d$\n",g_iDiagnostic_nw_status,
            g_iDiagnostic_spk_status,g_iDiagnostic_pvt_status,g_iDiagnostic_mic_status);
            strcat(server_message,frame_message);
            printf("sendig config requset message to server \n");
            printf("sendig message %s\n",server_message);

            if(sendto(g_uiServ_udp_comm_fd,server_message,strlen(server_message),ZERO,
            &udp_g_cServer_addr,sizeof(udp_g_cServer_addr))<ZERO)
            {
                perror("write failed \n");
            }
        break;

        case CONFIG_OK_RESPONSE:
            sprintf(frame_message,"1$\n");
            strcat(server_message,frame_message);
            printf("sendig message %s\n",server_message);

            if(sendto(g_uiServ_udp_comm_fd,server_message,strlen(server_message),ZERO,
            &udp_g_cServer_addr,sizeof(udp_g_cServer_addr))<ZERO)
            {
                perror("write failed \n");
            }
            printf("IN make CONFIG_OK call \n");
        break;

        case GLOBAL_ALARM_REQ:
            sprintf(frame_message,"%c%c$",offset1,offset2);
            strcat(server_message,frame_message);
            printf("sending message %s \n",server_message);
            if(sendto(g_uiServ_udp_comm_fd,server_message,strlen(server_message),ZERO,
            &udp_g_cServer_addr,sizeof(udp_g_cServer_addr))<ZERO)
            {
                perror("write failed \n");
            }
        break;

        case GLOBAL_CALL_REQ:
            strcat(server_message,frame_message);
            printf("sending message %s \n",server_message);
            if(sendto(g_uiServ_udp_comm_fd,server_message,strlen(server_message),ZERO,
            &udp_g_cServer_addr,sizeof(udp_g_cServer_addr))<ZERO)
            {
                perror("write failed \n");
            }
        break;

        case GLOBAL_RESET:
            strcat(server_message,frame_message);
            printf("sending message %s \n",server_message);
            if(sendto(g_uiServ_udp_comm_fd,server_message,strlen(server_message),ZERO,
            &udp_g_cServer_addr,sizeof(udp_g_cServer_addr))<ZERO)
            {
                perror("write failed \n");
            }
        break;



        case ICOM_PTT_SEND_REQ:
            sprintf(frame_message,"%d$",pvt_call.curr_icom_no+MIN_OFFSET);
            printf("...............pvt_call.curr_icom_no is %d \nframe_message is %s\n",pvt_call.curr_icom_no,frame_message);
            strcat(server_message,frame_message);
            printf("sending message %s \n",server_message);
            if(sendto(g_uiServ_udp_comm_fd,server_message,strlen(server_message),ZERO,
            &udp_g_cServer_addr,sizeof(udp_g_cServer_addr))<ZERO)
            {
                perror("write failed \n");
            }
        break;

        case CONF_RESET_FRAME:
            sprintf(frame_message,"0$");
            strcat(server_message,frame_message);
            printf("sending message %s \n",server_message);
            if(sendto(g_uiServ_udp_comm_fd,server_message,strlen(server_message),ZERO,
            &udp_g_cServer_addr,sizeof(udp_g_cServer_addr))<ZERO)
            {
                perror("write failed \n");
            }
        break;

        case ICOM_PTT_RELEASE_REQ:
            sprintf(frame_message,"%d$",pvt_call.curr_icom_no+MIN_OFFSET);
            strcat(server_message,frame_message);
            printf("sending message %s \n",server_message);
            if(sendto(g_uiServ_udp_comm_fd,server_message,strlen(server_message),ZERO,
            &udp_g_cServer_addr,sizeof(udp_g_cServer_addr))<ZERO)
            {
                perror("write failed \n");
            }
        break;

        case P2P_CALL_REQUEST:

            sprintf(frame_message,"%s$\n",g_cCalled_station);
            strcat(server_message,frame_message);
            printf("sending message %s \n",server_message);
            if(sendto(g_uiServ_udp_comm_fd,server_message,strlen(server_message),ZERO,
            &udp_g_cServer_addr,sizeof(udp_g_cServer_addr))<ZERO)
            {
                perror("write failed \n");
            }
        break;

        case P2P_CALL_ACK:
            //sprintf(p2p_call_ack_message,"%s$%d$%d$%d$",FRAME_HEADER,frame_length,cmd_id,stn_config.logical_id);
           //sprintf(frame_message,"%s$\n",g_cCalled_station);
            //strcat(server_message,frame_message);
            printf("sending message %s \n",server_message);
            if(sendto(g_uiServ_udp_comm_fd,server_message,strlen(server_message),ZERO,
            &udp_g_cServer_addr,sizeof(udp_g_cServer_addr))<ZERO)
            {
                perror("write failed \n");
            }
        break;

        case P2P_NO_RESPONSE:
            printf("sending message %s \n",server_message);
            if(sendto(g_uiServ_udp_comm_fd,server_message,strlen(server_message),ZERO,
            &udp_g_cServer_addr,sizeof(udp_g_cServer_addr))<ZERO)
            {
                perror("write failed \n");
            }
        break;

        case P2P_CALL_RESET_REQ:
            //sprintf(frame_message,"%s$\n",g_cCalled_station);
            //strcat(server_message,frame_message);
            printf("sending message %s \n",server_message);
            if(sendto(g_uiServ_udp_comm_fd,server_message,strlen(server_message),ZERO,
            &udp_g_cServer_addr,sizeof(udp_g_cServer_addr))<ZERO)
            {
                perror("write failed \n");
            }
        break;

         case P2P_CALL_RESET_ACK:
            printf("sending message %s \n",server_message);
            if(sendto(g_uiServ_udp_comm_fd,server_message,strlen(server_message),ZERO,
            &udp_g_cServer_addr,sizeof(udp_g_cServer_addr))<ZERO)
            {
                perror("write failed \n");
            }
        break;


        case GROUP_CALL_REQUEST:
            sprintf(frame_message,"%c%c$0$",offset1,offset2);
            strcat(server_message,frame_message);
            printf("sending message %s \n",server_message);
            if(sendto(g_uiServ_udp_comm_fd,server_message,strlen(server_message),ZERO,
            &udp_g_cServer_addr,sizeof(udp_g_cServer_addr))<ZERO)
            {
                perror("write failed \n");
            }
        break;

        case ZONE_CALL_REQUEST:
            sprintf(frame_message,"%c%c$0$",offset1,offset2);
            strcat(server_message,frame_message);
            printf("sending message %s \n",server_message);
            if(sendto(g_uiServ_udp_comm_fd,server_message,strlen(server_message),ZERO,
            &udp_g_cServer_addr,sizeof(udp_g_cServer_addr))<ZERO)
            {
                perror("write failed \n");
            }
        break;

        case ZONE_ALM_REQUEST:
            sprintf(frame_message,"%d$%c%c$0$",pg_call.zone_no+MIN_OFFSET,offset1,offset2);
            strcat(server_message,frame_message);
            printf("sending message %s \n",server_message);
            if(sendto(g_uiServ_udp_comm_fd,server_message,strlen(server_message),ZERO,
            &udp_g_cServer_addr,sizeof(udp_g_cServer_addr))<ZERO)
            {
                perror("write failed \n");
            }
        break;

        case GROUP_CALL_RESET_REQ:
            sprintf(frame_message,"%d$%c$1$",pvt_call.group_no+MIN_OFFSET,offset1);
            strcat(server_message,frame_message);
            printf("sending message %s \n",server_message);
            if(sendto(g_uiServ_udp_comm_fd,server_message,strlen(server_message),ZERO,
            &udp_g_cServer_addr,sizeof(udp_g_cServer_addr))<ZERO)
            {
                perror("write failed \n");
            }
        break;

         case ZONE_RESET_REQ:
            sprintf(frame_message,"%d$1$",pg_call.zone_no+MIN_OFFSET);
            strcat(server_message,frame_message);
            printf("sending message %s \n",server_message);
            if(sendto(g_uiServ_udp_comm_fd,server_message,strlen(server_message),ZERO,
            &udp_g_cServer_addr,sizeof(udp_g_cServer_addr))<ZERO)
            {
                perror("write failed \n");
            }
        break;

        case GROUP_PTT_REQ:
            sprintf(frame_message,"%d$%c$0$",pvt_call.group_no+MIN_OFFSET,offset1);
            strcat(server_message,frame_message);
            printf("sending message %s \n",server_message);
            if(sendto(g_uiServ_udp_comm_fd,server_message,strlen(server_message),ZERO,
            &udp_g_cServer_addr,sizeof(udp_g_cServer_addr))<ZERO)
            {
                perror("write failed \n");
            }
        break;

         case GROUP_PTT_RELEASE:
            sprintf(frame_message,"%d$%c$1$",pvt_call.group_no+MIN_OFFSET,offset1);
            strcat(server_message,frame_message);
            printf("sending message %s \n",server_message);
            if(sendto(g_uiServ_udp_comm_fd,server_message,strlen(server_message),ZERO,
            &udp_g_cServer_addr,sizeof(udp_g_cServer_addr))<ZERO)
            {
                perror("write failed \n");
            }
        break;

        case CONF_CALL_REQ:
            printf("conf req sending \n");
            sprintf(frame_message,"%c%c%c$",g_cCalled_station[0],g_cCalled_station[1],g_cCalled_station[2]);
            strcat(server_message,frame_message);
            printf("sending message %s \n",server_message);
            if(sendto(g_uiServ_udp_comm_fd,server_message,strlen(server_message),ZERO,
            &udp_g_cServer_addr,sizeof(udp_g_cServer_addr))<ZERO)
            {
                perror("write failed \n");
            }
        break;

         case KEEP_ALIVE_FRAME:
            sprintf(frame_message,"%c$%s$\n",fsu_state,gc_station_ip);
            strcat(server_message,frame_message);
            printf("sending message %s \n",server_message);
            if(sendto(g_uiServ_udp_comm_fd,server_message,strlen(server_message),ZERO,
            &udp_g_cServer_addr,sizeof(udp_g_cServer_addr))<ZERO)
            {
                perror("write failed \n");
            }
        break;


        default:
        break;


    }


}

/*******************************************************************************************
 * Function name: stn_shutdown                                                             *
 * Input arg    : void                                                                     *
 * Output arg   : NONE                                                                     *
 * Description  : calling this function results shutdown of station                        *
 *                                                                                         *
********************************************************************************************/
int stn_shutdown(void)
{
	FILE *in;
	extern FILE *popen();

	char str[MAX_BUFFER_SIZE];

	bzero(str,sizeof(str));
	sprintf(str, "%s", DEVICE_POWEROFF);
	/*	popen creates a pipe so we can read the output
		of the program we are invoking */
	if (!(in = popen(str, "r"))) {
		return 0;
	}

	// close pipe
	pclose(in);
	//sleep(1);
}
/*******************************************************************************************
 * Function name: reboot                                                                   *
 * Input arg    : void                                                                     *
 * Output arg   : NONE                                                                     *
 * Description  : calling this function results reboot of station                          *
 *                                                                                         *
********************************************************************************************/
int reboot(void)
{
	FILE *in;
	extern FILE *popen();

	char str[MAX_BUFFER_SIZE];

	bzero(str,sizeof(str));
	sprintf(str, "%s", DEVICE_REBOOT);
	/*	popen creates a pipe so we can read the output
		of the program we are invoking */
	if (!(in = popen(str, "r"))) {
		return 0;
	}

	// close pipe
	pclose(in);
//	sleep(1);
}

/*******************************************************************************************
 * Function name: create_broadcast_reg_socket                                              *
 * Input arg    : void                                                                     *
 * Output arg   : NONE                                                                     *
 * Description  : creates socket to receive broacast frames from server                    *
 *                                                                                         *
********************************************************************************************/

void create_broadcast_reg_socket()
{

    int ret, optval = 1;
    printf("inside create_broadcast_reg_socket function...\n");
   /* set up socket */
   g_uiBroadcast_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
   printf("*************** Broadcast socket check %d\n",g_uiBroadcast_fd);
   if (g_uiBroadcast_fd < 0)
   {
     perror("create broadcast socket create failed:\n");
     exit(1);
   }
   else
   {
     printf("broadcast socket reg created \n");
   }

   bzero((char *)&reg_addr, sizeof(reg_addr));
   reg_addr.sin_family = AF_INET;
   //reg_addr.sin_addr.s_addr = htonl(INADDR_ANY);
   reg_addr.sin_addr.s_addr = inet_addr("192.168.1.255");
   reg_addr.sin_port = htons(atoi(BROADCAST_PORT));
  // g_uiReg_addrlen = sizeof(reg_addr);       /* multicast request structure *//*@P*/
    g_uiReg_addrlen = sizeof(reg_addr);

     //reg_addr.sin_addr.s_addr = inet_addr(REG_MULTICAST_ADDRESS);
    setsockopt(g_uiBroadcast_fd, SOL_SOCKET, SO_BROADCAST,
	     (const void *)&optval , sizeof(int));
     /* receive */
      if (bind(g_uiBroadcast_fd, (struct sockaddr *) &reg_addr, sizeof(reg_addr)) < 0)
      {
          perror("bind broadcast socket:\n");
         // exit(1);
      }

      FD_SET(g_uiBroadcast_fd,&app_readset);
}

/*******************************************************************************************
 * Function name: validate_frame                                                           *
 * Input arg    : char*                                                                    *
 * Output arg   : NONE                                                                     *
 * Description  : this function valideates header in received frame from server            *
 *                                                                                         *
********************************************************************************************/

unsigned int validate_frame(char *source)
{
	char incoming_hdr[PRTCL_HDR_LEN + 1] = {0};
	char buf[5];
	unsigned int frame_len = 0;

	incoming_hdr[ZERO] = *(source + ZERO );
	incoming_hdr[ONE] = *(source + ONE );
	incoming_hdr[TWO] = *(source + TWO);
	incoming_hdr[THREE] = *(source + THREE );

	if (strcmp(incoming_hdr,PRTCL_HDR) == 0){
		//incoming_ver = *(source + PRTCL_HDR_LEN ) & 0xFF | *(source + PRTCL_HDR_LEN + 1) << 8 & 0xFF00;
		//printf("the ver is %i\n", incoming_ver);                            //.
		//printf("----sc....Protocol Header Matched\n");
		//sprintf(buf,"%c%c",*(source+4),*(source+5));
		frame_len=*(source+FOUR) & 0xFF | *(source + FIVE) << EIGHT & 0xFF00;
		//printf("buf length====%d\n",frame_len);


		return frame_len;                     //.
		//if(incoming_ver == PRTCL_VER)
    }
	else{
		//fprintf(stderr,"ERROR, Protocol Header Incorrect\n");
		return -1;
	}
}

/*******************************************************************************************
 * Function name: create_multicast_signalling_socket                                       *
 * Input arg    : NONE                                                                     *
 * Output arg   : NONE                                                                     *
 * Description  : this function creates socket to receive multicast frames from server     *
 *                                                                                         *
********************************************************************************************/

void create_multicast_signalling_socket()
{

   int cnt,ret;
   struct ip_mreq mreq;
   char message[FIFTY];

   /* ret = close(g_uiMulticast_signalling_fd);
   if(ret == 0 )
   {
        printf("Socket is closed....\n");
        //close(g_uiMulticast_signalling_fd);
   }*/


   /* set up socket */
  // g_uiMulticast_signalling_fd_sample  = socket(AF_INET, SOCK_DGRAM, 0);
   g_uiMulticast_signalling_fd  = socket(AF_INET, SOCK_DGRAM, 0);
   printf("##############g_uiMulticast_signalling_fd is = %d\n", g_uiMulticast_signalling_fd);
   if (g_uiMulticast_signalling_fd  < 0) {
     perror("mul sig socket create:\n");
    // exit(1);
   }
   else{printf("multicast signalling socket reg created \n");}
   bzero((char *)&mul_sig_addr, sizeof(mul_sig_addr));
   mul_sig_addr.sin_family = AF_INET;
   mul_sig_addr.sin_addr.s_addr = htonl(INADDR_ANY);
   mul_sig_addr.sin_port = htons(gSi_mul_singalling_port);
  // g_uiReg_addrlen = sizeof(reg_addr);       /* multicast request structure *//*@P*/
    g_uiMul_sig_addrlen = sizeof(mul_sig_addr);

    mul_sig_addr.sin_addr.s_addr = inet_addr(g_cMul_signalling_ip);



     /* receive */
      if (bind(g_uiMulticast_signalling_fd , (struct sockaddr *) &mul_sig_addr, sizeof(mul_sig_addr)) < 0) {
          perror("bind multicast");
         // exit(1);
      }
      else{printf("multicast bind created %s \n",gc_station_ip);}
      mreq.imr_multiaddr.s_addr = inet_addr(g_cMul_signalling_ip);
      mreq.imr_interface.s_addr = inet_addr(gc_station_ip);

      if (setsockopt(g_uiMulticast_signalling_fd , IPPROTO_IP, IP_ADD_MEMBERSHIP,
		     &mreq, sizeof(mreq)) < 0) {
	 perror("setsockopt mreq");
	 //exit(1);
      }
      else{printf("multicast setsockopt created \n");}



   	 FD_SET(g_uiMulticast_signalling_fd ,&app_readset);



}


/*void initialize_server_communication()
{

    /*int  portno;

	g_uiServer_comm_fd = socket(PF_UNIX, SOCK_STREAM, 0);

	if (g_uiServer_comm_fd < 0){
		error("----sc....ERROR opening socket\n");
	}
	else{printf("server comm socket reg created \n");}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = server_tcp_port_no;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	serv_addr.sin_port = htons(portno);

	setsockopt(g_uiServer_comm_fd,SOL_SOCKET,SO_REUSEADDR,NULL,0);

	if (bind(g_uiServer_comm_fd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
		error("----sc....ERROR on binding\n");
	}else{printf("server comm bind reg created \n");}

	listen(g_uiServer_comm_fd,5);
	printf("starting server thread\n");

	 FD_SET(g_uiServer_comm_fd,&app_readset);


	 *


}*/
