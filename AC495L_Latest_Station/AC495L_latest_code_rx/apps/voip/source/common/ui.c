

/**********************************************************************
 *                                                                    *
 * File Name:	ui.c                                           *
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
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <pty.h>
#include <utmp.h>
#include <pthread.h>
#include <linux/input.h>
#include <time.h>
#include <poll.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/timerfd.h>
#include <stdint.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <signal.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <signal.h>
#include "keypad.h"
#include "iphone_hwLcd.h"
#include "ui.h"
//#include "ui_gpio_interface.h"
#include "keypad_interface.h"
#include "voip_main.h"
//#include "server_coms.h"
//#include "lcd_task.h"

sigset_t              pselect_set;
#define ON 3
#define OFF 4
int keypadFd=-1;
int lcd_io_Fd=-1;
int led_on_timer,led_off_timer=0,flash_counter=0 ,param=0;
int  flash=0,pg_mute_on=1;
char arr[1];
char lcd[]={0x27};
char io[]={0x24};
pthread_mutex_t ioctl_io_lcd;
static int line1_state,line2_state;

struct sockaddr_un  address;
struct itimerspec   timspec;
struct sockaddr_un  ui_address;

static keypad_keys digitToKeyArr[FIVE][SEVEN] =
{
 	{D1_KEY,	D2_KEY,		D3_KEY,	 	KEYPAD_KEY_ASTERIKS,      EPBX_KEY,	 	RESET_KEY,	PAGE_KEY},
 	{D4_KEY,	D5_KEY,	 	D6_KEY,	   	GROUP_KEY,		  DIGIT_0, 	  	PTT_KEY, 	CHANNEL_KEY},
 	{D7_KEY,	D8_KEY,  	D9_KEY, 	DIGIT_7,	    	  DIGIT_8, 		DIGIT_9,	ZONE_KEY},
 	{D10_KEY,	D11_KEY, 	D12_KEY,  	DIGIT_4,     		  DIGIT_5, 		DIGIT_6, 	GLOBAL_KEY},
 	{D13_KEY,	D14_KEY, 	D15_KEY,  	DIGIT_1,	    	  DIGIT_2, 		DIGIT_3,	CONF_KEY}

};
/*static keypad_keys digitToKeyArr[FIVE][SEVEN] =
{
 	{DIGIT_4,	DIGIT_5,		DIGIT_6,	 	KEYPAD_KEY_ASTERIKS,      EPBX_KEY,	 	RESET_KEY,	PAGE_KEY},
 	{DIGIT_2,	D5_KEY,	 	D6_KEY,	   	GROUP_KEY,		  DIGIT_0, 	  	PTT_KEY, 	CHANNEL_KEY},
 	{D7_KEY,	D8_KEY,  	D9_KEY, 	DIGIT_7,	    	  DIGIT_8, 		DIGIT_9,	ZONE_KEY},
 	{D10_KEY,	D11_KEY, 	D12_KEY,  	D1_KEY,     		  D2_KEY, 		D3_KEY, 	GLOBAL_KEY},
 	{D13_KEY,	D14_KEY, 	D15_KEY,  	DIGIT_1,	    	  D4_KEY, 		DIGIT_3,	CONF_KEY}

};*/
//fd_set readset,testset;
uint64_t expirations_line1;

socklen_t ui_address_length;

int                 lcd_fd;
int                 gp_pvt;
int                 gp_pg;
int                 led_timer_flag;
int                 lcd_timer_flag;
int                 g_iKeyEventFD;
int                 g_itimer_fd;

int                 g_iserver_socket_fd;
int                 g_iconnection_fd;
int                 g_iIcom_hop_flag;
unsigned int        g_uiSpeech_detection_timer;

int PvtCallTimeArr[10000];
int PvtCallTimeArrIndex = 0;
void lcd_initialization();
void io_initialization();

char                c_Fsu_state;



/*******************************************************************************************
 * Function name: open_lcd                                                          *
 * Input arg    : void                                                                     *
 * Output arg   : NONE                                                                     *
 * Description  : This function open lcd driver    						*
 *                                                                                         *
********************************************************************************************/
void open_lcd_io()
{
    printf("***************OPENING LCD DRIVER ****************\n");

     if((lcd_io_Fd = open("/dev/klcd_io",O_RDWR))== -1) {
		printf("[%s , %d ] can't open lcd device\r\n",__FUNCTION__,__LINE__);
		return;
	 }
    	lcd_initialization();
}


/*******************************************************************************************
 * Function name: lcd_init                                                          *
 * Input arg    : void                                                                     *
 * Output arg   : NONE                                                                     *
 * Description  : This function initialize lcd     						*
 *                                                                                         *
********************************************************************************************/
void lcd_initialization()
{
printf("In lcd_initialization\n");
    char buf[] = {0x30,0x30,0x30,0x20,0x28,0x08,0x01,0x06,0x0C};
    char addr = LCD_ADDRESS;
    printf("inside lcd_init %x\n ", addr);
    msg.i2c_device_address=addr;
	msg.cmd=IOCTL_LCD_INITIALIZATION;
	strcpy(msg.kbuf,buf);
	//memcpy(msg.kbuf,buf,9);
	msg.lineNumber=ONE;
	msg.start_position=ZERO;

    update_lcd_msg(msg);
}
/*******************************************************************************************
 * Function name: io_init                                                          *
 * Input arg    : void                                                                     *
 * Output arg   : NONE                                                                     *
 * Description  : This function initialize io expander 						 *
 *                                                                                         *
********************************************************************************************/
void io_initialization()
{

    char data[]={0x00};
    char addr = IO_ADDRESS;
    printf("inside io_init %x\n ", addr);
    io_msg.cmd=IOCTL_IO_INITIALIZATION;
	io_msg.i2c_device_address=addr;
	strcpy(io_msg.kbuf,data);

    update_io_msg(io_msg);
}


/*******************************************************************************************
 * Function name: create_timer_fd                                                          *
 * Input arg    : void                                                                     *
 * Output arg   : NONE                                                                     *
 * Description  : This function creates timer fd to monitor various timely acitivities.    *
 *                                                                                         *
********************************************************************************************/

void create_timer_fd()
{
   //fd_set readset;
   int i_res;
   struct timeval timeout;
    //fd_set readset,testset;
   g_itimer_fd=timerfd_create(CLOCK_MONOTONIC,0);

    FD_SET(g_itimer_fd,&readset);

    bzero(&timspec,sizeof(timspec));

    timspec.it_interval.tv_sec=0;//msg->attr;
    timspec.it_interval.tv_nsec=100000000; ////100ms timer instead of 1sec

    timspec.it_value.tv_sec=0;//msg->attr;
    timspec.it_value.tv_nsec=100000000;////100ms timer instead of 1sec

    i_res=timerfd_settime(g_itimer_fd,0,&timspec,0);
    printf("ui_timer_created %d\n",g_itimer_fd);
    if(i_res<0)
    {
     perror("timerfd settime:");
    }

}



/*******************************************************************************************
 * Function name: update_lcd_msg                                                           *
 * Input arg    : struct ioctl_lcd_mesg                                                    *
 * Output arg   : NONE                                                                     *
 * Description  : This function update message on LCD display           		   *
 *                                                                                         *
********************************************************************************************/

void update_lcd_msg(struct ioctl_lcd_mesg msg)
{
switch(msg.cmd)
{
case IOCTL_CLEAR_DISPLAY:
    	ioctl(lcd_io_Fd, LCD,&msg );
	break;

case IOCTL_PRINT:
        ioctl(lcd_io_Fd, LCD,&msg );
	break;

case IOCTL_LCD_INITIALIZATION:
        ioctl(lcd_io_Fd, LCD,&msg );
	break;

default:
   printf("Invalid state in update_lcd_msg\n");
   break;
}

}


/*******************************************************************************************
 * Function name: update_io_msg                                                           *
 * Input arg    : struct ioctl_io_mesg                                                   *
 * Output arg   : NONE                                                                     *
 * Description  : This function to operate LED           		   		*
 *                                                                                         *
********************************************************************************************/

void update_io_msg(struct ioctl_io_mesg io_msg)
{
switch(io_msg.cmd)
{
case IOCTL_IO_INITIALIZATION:
	ioctl(lcd_io_Fd,IO,&io_msg );
	break;

case IOCTL_SET_BIT_IO_EXPANDER:
	ioctl(lcd_io_Fd,IO,&io_msg );
	break;

case IOCTL_GET_BIT_IO_EXPANDER:
	ioctl(lcd_io_Fd,IO,arr );
        break;

default:
   printf("Invalid state in update_lcd_msg\n");
   break;
}

}



/*******************************************************************************************
 * Function name: UI_sock_create                                                           *
 * Input arg    : void                                                                     *
 * Output arg   : NONE                                                                     *
 * Description  : This function creates socket to communicate Voip_task process.           *
 *                                                                                         *
********************************************************************************************/

void UI_sock_create ()
{

 	char c_socket_name[TEN]={"UI"};
 	g_iserver_socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(g_iserver_socket_fd < 0)
	{
		printf("Sockfd listen Error= %s !!!!!!!!!! \n",strerror(errno));
    return;
 	}
	 unlink(c_socket_name);
	 ui_address.sun_family = AF_UNIX;
	 ui_address_length = sizeof(ui_address.sun_family) +
		          sprintf(ui_address.sun_path,"%s",c_socket_name);
	 if(bind(g_iserver_socket_fd, (struct sockaddr *) &ui_address, ui_address_length) != 0)
	 {
		 perror("bind() failed\n");

     }

        if(listen(g_iserver_socket_fd, 5) != 0)
        {
          printf("listen() failed\n");

        }
        FD_ZERO(&readset);
        FD_SET(g_iserver_socket_fd,&readset);
        FD_SET(g_iKeyEventFD,&readset);
}

/*******************************************************************************************
 * Function name: keypad_sig_handler                                                       *
 * Input arg    : void                                                                     *
 * Output arg   : NONE                                                                     *
 * Description  : This function collects pressed key.                                      *
 *                                                                                         *
********************************************************************************************/

void keypad_sig_handler(void)
{

  int i_events,i_current=0;
  KEY_EVENT key[MAX_BUFFER_SIZE];
  int i_pressed;
  keypad_keys keypad_key;

	i_events =(read(g_iKeyEventFD,(char *)&key,MAX_EVENTS_TO_READ*sizeof(KEY_EVENT))/
	sizeof(KEY_EVENT)) ;


	   if(i_events>0)
    	{

        keypad_key=digitToKeyArr[key[i_current].row][key[i_current].column];
        i_pressed=key[i_current].key_press_time;

        printf("row=%d,col=%d,key=%d\n",key[i_current].row, key[i_current].column, keypad_key);

        keypad_report(keypad_key,i_pressed);
        //if(keypad_report(keypad_key,i_pressed) < 0)
        //printf("Error in keypad_report\n ");
        }
        printf("row=%d,col=%d\n",key[i_current].row, key[i_current].column);

}

/*******************************************************************************************
 * Function name: startKeypad                                                              *
 * Input arg    : void                                                                     *
 * Output arg   : NONE                                                                     *
 * Description  : This function opens the keypad character driver and initializes.         *
 *                                                                                         *
********************************************************************************************/
void startKeypad(void)
{
  int 	i_oflags;

  action.sa_handler = keypad_sig_handler;
  action.sa_flags = 0;
  sigaction(SIGIO,&action,&info);
  sigemptyset(&pselect_set);
  sigaddset(&pselect_set, SIGIO);

    /* register keypad signal handler */
	if(signal(SIGIO,&keypad_sig_handler) == SIG_ERR) {
		//acl_log(ACL_LOG_ERROR,"keypad_sig_handler not registered\r\n");
		return;
	}

	/* open char driver*/
	if((g_iKeyEventFD = open("/dev/keypad0",O_RDWR))== -1)
	{
    printf("can't open keypad device\r\n");
    return;
	}


	/* code asynchronous notification*/
	fcntl(g_iKeyEventFD, F_SETOWN, getpid());
	i_oflags = fcntl(g_iKeyEventFD, F_GETFL);
	fcntl(g_iKeyEventFD, F_SETFL, i_oflags | FASYNC);


} /* end of startKeypad() */



/*******************************************************************************************
 * Function name: ui_thread                                                                *
 * Input arg    : void*                                                                    *
 * Output arg   : NONE                                                                    *
 * Description  : This is UI_thread function which uses select system call monitror        *  timer fd,keypad_signal etc                                                                 *
*******************************************************************************************/
void *ui_thread(void *data)
{char time_string[FOURTY];
  struct timeval timeout;
	struct UI_msg lcd_msg;
  int   i_fd;
  int   i_res;
  int   i_result;
  int   i_rc;
  uint64_t expirations;
  char  c_buffer[TWENTY_FIVE];

  int i_ret,i_ptt_pressed=CLEAR,i_cnt=CLEAR;
  unsigned char uc_call_type;
  volatile int i_ret1=0;char ret;
  char buf[FIVE],call_type[FOUR];
  int  n;
  UI_sock_create();
  create_timer_fd();
  open_lcd_io();
  io_initialization();


while(1)
{
    testset = readset;
    timeout.tv_sec = 1;
    timeout.tv_usec = 500000;
    i_result=pselect(FD_SETSIZE,&testset,NULL,NULL,&timeout,&pselect_set);

     if(i_result<1)
     {
        perror("ui server1");
        return 0;
     }
      for(i_fd=ZERO;i_fd<TWENTY_FIVE;i_fd++)
      {

        if((FD_ISSET(i_fd,&testset)))
        {
          if(i_fd == g_iserver_socket_fd)
          {
            g_iconnection_fd = accept(g_iserver_socket_fd,(struct sockaddr *) &ui_address,
                                   (socklen_t *)&ui_adress_length);
            FD_SET(g_iconnection_fd,&readset);
            printf("adding client on fd %d\n", g_iconnection_fd);
          }
          else
          {
            if((FD_ISSET(i_fd,&testset)))
            {
              if (i_fd == g_itimer_fd)
              {
                int i_res;
                i_res=read(g_itimer_fd,&expirations,sizeof(expirations));
                if(c_Fsu_state >= UI_IDLE_STATE)
                {
                  struct timeval tv;
                  struct tm* ptm;
                  //char time_string[FOURTY];
                  long milliseconds;
                  gettimeofday (&tv, NULL);
                  ptm = localtime (&tv.tv_sec);
                  strftime (time_string, sizeof (time_string), "%d-%m-%Y %H:%M:%S", ptm);
                  milliseconds = tv.tv_usec / THOUSAND;
    //            send_data_to_lcd(LINE1, time_string);
                }

                if(i_res < 0)
                {
                  perror("Server-read() error");
                  close(i_fd);
                  FD_CLR(i_fd, &readset);
                  // exit (-1);
                }
                else
                {
                  if(g_iInterdigit_timer)
                  {
                        g_iInterdigit_timer--;

                        if(g_iInterdigit_timer==CLEAR)
                        {
                            clear_validation_processing();

                            if(lcd_fcs_default_icom == 0)
                            {
                                sprintf(g_cLine3_buf,"STN_NO:%d  IDLE",stn_config.logical_id);

                                msg.cmd=IOCTL_PRINT;
                                msg.start_position=ZERO;
                                msg.lineNumber=LINE2;
                                msg.i2c_device_address=LCD_ADDRESS;
                                strcpy(msg.kbuf,g_cLine3_buf);
                                update_lcd_msg(msg);
                            }
                            else
                            {
                                sprintf(g_cLine3_buf,"STN_NO:%d CH:%d ",stn_config.logical_id,lcd_fcs_default_icom);

                                msg.cmd=IOCTL_PRINT;
                                msg.start_position=ZERO;
                                msg.lineNumber=LINE2;
                                msg.i2c_device_address=LCD_ADDRESS;
                                strcpy(msg.kbuf,g_cLine3_buf);
                                update_lcd_msg(msg);
                            }

                        }
                    }

                    //to print private call timeout on LCD

                    if(pvt_call.pvt_call_busy)
                    {
                        if(g_uiPvt_call_timer_ui_lcd)
                        {

                            g_uiPvt_call_time_lcd = (g_uiPvt_call_timer_ui_lcd / 10);
                           if((g_uiPvt_call_timer_ui_lcd%10) == 0)
                           {

                                PvtCallTime_min = (g_uiPvt_call_time_lcd/60);
                                PvtCallTime_sec = (g_uiPvt_call_time_lcd%60);

                                if( PvtCallTime_sec > NINE)
                                {
                                     sprintf(&g_arcPvtCallTimer[ZERO],"%d",0);
                                     sprintf(&g_arcPvtCallTimer[ONE],"%d",PvtCallTime_min);
                                     sprintf(&g_arcPvtCallTimer[TWO],"%s",":");
                                     sprintf(&g_arcPvtCallTimer[THREE],"%d",PvtCallTime_sec);
                                }
                                else
                                {
                                     sprintf(&g_arcPvtCallTimer[ZERO],"%d",0);
                                     sprintf(&g_arcPvtCallTimer[ONE],"%d",PvtCallTime_min);
                                     sprintf(&g_arcPvtCallTimer[TWO],"%s",":");
                                     sprintf(&g_arcPvtCallTimer[THREE],"%d",0);
                                     sprintf(&g_arcPvtCallTimer[FOUR],"%d",PvtCallTime_sec);
                                }
                                msg.cmd=IOCTL_PRINT;
                                msg.start_position=ELEVEN;
                                msg.lineNumber=LINE2;
                                msg.i2c_device_address=LCD_ADDRESS;
                                strcpy(msg.kbuf,g_arcPvtCallTimer);
                                update_lcd_msg(msg);

                            }
                            g_uiPvt_call_timer_ui_lcd--;

                        }

                        //Page_call_init_timeout
                        else if(g_uiPage_call_timer_frlcd_ini)
                        {
                            g_uipg_call_time_lcd_init = (g_uiPage_call_timer_frlcd_ini / 10);

                            if((g_uiPage_call_timer_frlcd_ini%10) == 0)
                            {
                                PgCallIniTime_min = (g_uipg_call_time_lcd_init/60);
                                PgCallIniTime_sec = (g_uipg_call_time_lcd_init%60);

                                if( PgCallIniTime_sec >NINE)
                                {
                                    sprintf(&g_arcPvtCallTimer_fr_pg_init[ZERO],"%d",0);
                                    sprintf(&g_arcPvtCallTimer_fr_pg_init[ONE],"%d",PgCallIniTime_min);
                                    sprintf(&g_arcPvtCallTimer_fr_pg_init[TWO],"%s",":");
                                    sprintf(&g_arcPvtCallTimer_fr_pg_init[THREE],"%d",PgCallIniTime_sec);
                                }
                                else
                                {
                                     sprintf(&g_arcPvtCallTimer_fr_pg_init[ZERO],"%d",0);
                                     sprintf(&g_arcPvtCallTimer_fr_pg_init[ONE],"%d",PgCallIniTime_min);
                                     sprintf(&g_arcPvtCallTimer_fr_pg_init[TWO],"%s",":");
                                     sprintf(&g_arcPvtCallTimer_fr_pg_init[THREE],"%d",0);
                                     sprintf(&g_arcPvtCallTimer_fr_pg_init[FOUR],"%d",PgCallIniTime_sec);
                                }
                                msg.cmd=IOCTL_PRINT;
                                msg.start_position=ELEVEN;
                                msg.lineNumber=LINE1;
                                msg.i2c_device_address=LCD_ADDRESS;
                                strcpy(msg.kbuf,g_arcPvtCallTimer_fr_pg_init);
                                update_lcd_msg(msg);
                            }
                            g_uiPage_call_timer_frlcd_ini--;
                        }
                    }

                    if(pg_call.pg_call_busy)
                    {
                        if(g_uiPage_call_timer_lcd)
                        {
                             g_uipg_call_time_lcd = (g_uiPage_call_timer_lcd / 10);

                             if((g_uiPage_call_timer_lcd % 10) == 0)
                             {
                                PgCallTime_min = (g_uipg_call_time_lcd/60);
                                PgCallTime_sec = (g_uipg_call_time_lcd%60);

                                if(PgCallTime_sec > NINE)
                                {
                                    sprintf(&g_arcPgCallTimer[ZERO],"%d",0);
                                    sprintf(&g_arcPgCallTimer[ONE],"%d",PgCallTime_min);
                                    sprintf(&g_arcPgCallTimer[TWO],"%s",":");
                                    sprintf(&g_arcPgCallTimer[THREE],"%d",PgCallTime_sec);
                                }
                                else
                                {
                                    sprintf(&g_arcPgCallTimer[ZERO],"%d",0);
                                    sprintf(&g_arcPgCallTimer[ONE],"%d",PgCallTime_min);
                                    sprintf(&g_arcPgCallTimer[TWO],"%s",":");
                                    sprintf(&g_arcPgCallTimer[THREE],"%d",0);
                                    sprintf(&g_arcPgCallTimer[FOUR],"%d",PgCallTime_sec);
                                }
                                msg.cmd=IOCTL_PRINT;
                                msg.start_position=ELEVEN;
                                msg.lineNumber=LINE1;
                                msg.i2c_device_address=LCD_ADDRESS;
                                strcpy(msg.kbuf,g_arcPgCallTimer);
                                update_lcd_msg(msg);
                            }
                            g_uiPage_call_timer_lcd--;
                        }
                    }



                    ///////////////////gpio thread is removed and gpio is monitored here//////////////////

                    pthread_mutex_lock(&lock_call_type);
                    uc_call_type=pvt_call.type;
                    pthread_mutex_unlock(&lock_call_type);

                    if(pvt_call.pvt_call_busy==SET )//|| uc_call_type==INTERCOM_RX||uc_call_type==INTERCOM_TX_RX)
                    {
                        i_ret=operate_gpio(PTT_SW,READ,CLEAR);
                        printf("***************i_ret=%d\n",i_ret);

                        if(i_ret==SET)
                        {
                            printf("In SET\n");
                            printf("g_iHandset_lifted_signal=%d\n",g_iHandset_lifted_signal);
                            if(g_iHandset_lifted_signal==SET)
                            {
                                printf("In g_iHandset_lifted_signal\n");
                                //operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_OFF);
                                g_iHandset_lifted_signal=CLEAR;
                                switch(uc_call_type)
                                {
                                    case INTERCOM_RX:
                                    case INTERCOM_TX_RX:
                                    case GLOBAL:
                                    case ZONE:
                                    case GROUP:
                                    case GROUP_TX_RX:
                                    case P2P:
                                    case CONFERENCE:
                                        g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = RESET;
                                        g_ucDialed_Sequence_No++;
                                        g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = HANDSET_RESET_KEY;
                                        printf("%d\n",g_iarcDialed_Key_Array[0]);
                                        if(g_iarcDialed_Key_Array[0] > 0)
                                        snprintf(call_type,FOUR,"%c",(char)g_iarcDialed_Key_Array[0]);//-'0');
                                        sprintf(buf,"%s%d%d%d%d",call_type,g_iarcDialed_Key_Array[1],g_iarcDialed_Key_Array[2],
                                        g_iarcDialed_Key_Array[3],g_iarcDialed_Key_Array[4],g_iarcDialed_Key_Array[5]);
                                        printf("sent data on socket is %s \n",buf);
                                        n=send(g_iconnection_fd,buf,sizeof(buf),0);
                                        if(n>0)
                                        {
                                            printf("successfully sent data \n");
                                        }
                                        else
                                        {
                                            perror("");
                                            printf("failed to send data \n");
                                        }
                                        pg_mute_on=ONE;
                                        clear_validation_processing();
                                        printf("Handset on cradle \n");
                                    break;

                                }
                            }

                        }
                        else //handset lifted
                        {//printf("In else\n");
                            g_iHandset_lifted_signal=SET;
                            printf("in else *****g_iHandset_lifted_signal=%d\n",g_iHandset_lifted_signal);
                       // operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);  ///Pvt and page audio mixing

                            if(/*pg_call.pg_call_busy!=SET && stn_config.default_AIM==CLEAR &&*/ pg_mute_on)
                            {
                                pg_mute_on=ZERO;
                                 ///page speaker is muted when handset is lifted
                                /*io_msg.cmd=IOCTL_SET_BIT_IO_EXPANDER;
                                io_msg.i2c_device_address=IO_ADDRESS;
                                io_msg.pin_no=FIVE;
                                io_msg.pin_status=SET;
                                strcpy(io_msg.kbuf,&io_led[FIVE]);///io_led value is 0x20
                                printf("----------io_msg.kbuf=%x----------\n",io_msg.kbuf);
                                update_io_msg(io_msg);*/
                                //multicast_prepare_dsp(PVT_CHANNEL);
                                select_codec(PVT_CHANNEL,NONE);
                                select_codec(PVT_CHANNEL,HANDSET);

                            }
                        }
                    }


                    /*if(pWork_m.private_state==PRIVATE_OUTGOING_ONGOING)
                    {
                        if(pvt_call.type==INTERCOM_TX_RX || pvt_call.type == GROUP_TX_RX)
                         // if(pvt_call.type==INTERCOM_TX_RX || pvt_call.type == GROUP)
                        {
                            i_ret1=operate_gpio(PTT_SPEECH,READ,CLEAR);

                            if(i_ret1==CLEAR)
                            {
                                if(g_uiSpeech_detection_timer >CLEAR)
                                {
                                    g_uiSpeech_detection_timer--;
                                }
                                printf("speech detection  alive  Timer =%d\n",g_uiSpeech_detection_timer);
                                if(g_uiSpeech_detection_timer==CLEAR)
                                {
                                    getTime();
                                    g_uiFsu_event = PTT_RELEASE_EVENT;
                                    printf("PTT release Event is set...\n");
                                }
                            }
                            else
                            {
                                 g_uiSpeech_detection_timer = SPEECH_DETECTION_TIMEOUT;

                            }
                        }
                    }*/

            }

            if(param==IO)
            {

                if(flash==ZERO)//if(flash==TWO && flag==1)   //contineous display
                {
                    printf("In  if(flash==ZERO)\n");
                    update_io_msg(io_msg);
                    flash=TWO;
                }
                else if(flash==ONE)////TO BLINK LED
                {
                    if(flash_counter > 0)
                    {
                        if(led_on_timer>0)
                        {
                            led_on_timer--;
                            if(led_on_timer==0)
                            {
                                update_io_msg(io_msg);
                                led_off_timer=(lcd_msg.led_off_time*10);
                            }
                        }

                        if(led_off_timer>0)
                        {
                            led_off_timer--;
                            if(led_off_timer==0)
                            {
                                char a[]={0x01};
                                strcpy(io_msg.kbuf,a);getTime();
                                update_io_msg(io_msg);
                                led_on_timer=(lcd_msg.led_on_time*10);
                                flash_counter--;
                            }
                        }
                    }
                }

            }

        }

    } //timer fd

    if(i_fd == g_iconnection_fd) //if(g_uiUi_socket_fd)
    {

        int rd;
        //struct UI_msg lcd_msg;
        rd = read(g_iconnection_fd, (void*)&lcd_msg,sizeof(struct UI_msg));
        if(rd ==-1)
        {
            printf("read() failed...\n");
        }
        printf("read() success...\n");

        printf("In ui.c\n");
        c_Fsu_state=lcd_msg.state;
        if(lcd_msg.parameter == LCD)
        {
            printf("In ui lcd case\n");
            msg.i2c_device_address=lcd_msg.addr;
            msg.cmd=lcd_msg.cmd;
            strcpy(msg.kbuf,lcd_msg.text);
            msg.lineNumber=lcd_msg.line_no;
            msg.start_position=lcd_msg.position;
            update_lcd_msg(msg);
        }
        else
        {
            printf("In ui io case\n");
            io_msg.cmd=lcd_msg.cmd;
            io_msg.i2c_device_address=lcd_msg.addr;
            strcpy(io_msg.kbuf,lcd_msg.text);
            io_msg.pin_no=lcd_msg.pin_no;
            io_msg.pin_status=lcd_msg.pin_status;
            param=lcd_msg.parameter;
            led_on_timer=(lcd_msg.led_on_time*10);
            flash=lcd_msg.display;
            flash_counter=lcd_msg.flash_cntr;
        }
    }

} //master if(fdset)

}//master for loop


}//while(1)

}



}







