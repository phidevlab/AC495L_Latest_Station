

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

#include "ui.h"
//#include "ui_gpio_interface.h"
#include "keypad_interface.h"
#include "voip_main.h"
#include "server_coms.h"
#include "lcd_task.h"


#define ON 3
#define OFF 4

static int line1_state,line2_state;


struct sockaddr_un  address;
struct itimerspec   timspec;
struct sockaddr_un  ui_address;
unsigned int          g_uiPvt_call_time_lcd;
unsigned int          g_uipg_call_time_lcd_init;
unsigned int          g_uipg_call_time_lcd;


static keypad_keys digitToKeyArr[EIGHT][EIGHT] =
{
 	{D1_KEY,	  D2_KEY,	    D3_KEY,	   D4_KEY,      D5_KEY,	 D6_KEY,	VOL_DN_KEY,	VOL_UP_KEY},
 	{BLANK,	    RESET_KEY,	BLANK,	   SRE_MUTE_KEY,DIGIT_0, PTT_KEY, BLANK,	    BLANK     },
 	{PRIO_KEY,	BLANK,      I_COM_KEY, DIGIT_7,	    DIGIT_8, DIGIT_9,	D26_KEY,	  D25_KEY   },
 	{AIM_KEY,	  ENTER_KEY,	EPBX_KEY,  DIGIT_4,     DIGIT_5, DIGIT_6, D19_KEY,    D18_KEY   },
 	{BLANK,	    ZONE_KEY,	  CONF_KEY,  DIGIT_1,	    DIGIT_2, DIGIT_3,	D12_KEY,    D11_KEY   },
 	{BLANK,	    D30_KEY,	  D29_KEY,   BLANK,	      BLANK,	 BLANK,   D28_KEY,	  BLANK     },
 	{D17_KEY,	  D16_KEY,	  D15_KEY,   D21_KEY,	    D22_KEY, D23_KEY,	D14_KEY,    D24_KEY   },
 	{D13_KEY,	  D20_KEY,	  D27_KEY,   D7_KEY,	    D8_KEY,	 D9_KEY,	BLANK,	    D10_KEY   }
};


/*static keypad_keys digitToKeyArr[FIVE][SEVEN] =
{
 	{D1_KEY,	D2_KEY,		D3_KEY,	 	KEYPAD_KEY_ASTERIKS,      EPBX_KEY,	 	RESET_KEY,	PAGE_KEY},
 	{D4_KEY,	D5_KEY,	 	D6_KEY,	   	GROUP_KEY,		  DIGIT_0, 	  	PTT_KEY, 	CHANNEL_KEY},
 	{D7_KEY,	D8_KEY,  	D9_KEY, 	DIGIT_7,	    	  DIGIT_8, 		DIGIT_9,	ZONE_KEY},
 	{D10_KEY,	D11_KEY, 	D12_KEY,  	DIGIT_4,     		  DIGIT_5, 		DIGIT_6, 	GLOBAL_KEY},
 	{D13_KEY,	D14_KEY, 	D15_KEY,  	DIGIT_1,	    	  DIGIT_2, 		DIGIT_3,	CONF_KEY}

};*/



fd_set readset,testset;
uint64_t expirations_line1;

socklen_t ui_address_length;


int                 lcd_fd;
int                 gp_pvt;
int                 gp_pg;
int                 led_timer_flag;
int                 lcd_timer_flag;
int                 g_iKeyEventFD;
int                 g_itimer_fd=0;

int                 g_iserver_socket_fd;
int                 g_iconnection_fd;
int                 g_iIcom_hop_flag;
unsigned int        g_uiSpeech_detection_timer;
extern unsigned int g_uiPage_call_timer_frlcd_ini;
extern unsigned int g_uiPage_call_timer_lcd;

int PvtCallTimeArr[10000];
int PvtCallTimeArrIndex = 0;

/*static int          line1_state;
static int          line2_state;
static int          line3_state;
static int          line4_state;
static int          line1_toggle_flag;
static int          line2_toggle_flag;
static int          line3_toggle_flag;
static int          line4_toggle_flag;
static int          led1_toggle_flag;
static int          led2_toggle_flag;
static int          led3_toggle_flag;
static int          led4_toggle_flag;
static int          led1_state;
static int          led2_state;
static int          led3_state;
static int          led4_state;*/

char                c_Fsu_state;




/*static keypad_keys digitToKeyArr[4][6] =
{
 	{KEYPAD_KEY_DIGIT_1,KEYPAD_KEY_DIGIT_2,KEYPAD_KEY_DIGIT_3,KEYPAD_KEY_LINE_1,KEYPAD_KEY_LINE_2,KEYPAD_KEY_HOLD},
 	{KEYPAD_KEY_DIGIT_4,KEYPAD_KEY_DIGIT_5,KEYPAD_KEY_DIGIT_6,KEYPAD_KEY_TRANSFER,KEYPAD_KEY_INVALID,KEYPAD_KEY_SPEAKER},
 	{KEYPAD_KEY_DIGIT_7,KEYPAD_KEY_DIGIT_8,KEYPAD_KEY_DIGIT_9,KEYPAD_KEY_VOLUME_UP,KEYPAD_KEY_VOLUME_DOWN,KEYPAD_KEY_OFF_HOOK},
 	{KEYPAD_KEY_ASTERIKS,KEYPAD_KEY_DIGIT_0,KEYPAD_KEY_POUND, KEYPAD_KEY_CONFERENCE,KEYPAD_KEY_MUTE,KEYPAD_KEY_ON_HOOK}
};*/




/*static void update_display(void *data)
{
	// take lcd mutex
//	pthread_mutex_lock(&lcd_mutex);
  struct lcd_message_t *lcd = (struct lcd_message_t *)data;

	set_lcd_line(0,lcd->line1_Msg);
	set_lcd_line(1,lcd->line2_Msg);
	set_lcd_line(2,lcd->line3_Msg);
	set_lcd_line(3,lcd->line4_Msg);

	lcd_out();

	//pthread_mutex_unlock(&lcd_mutex);
}*/


/*void update_led_state(void *data)
{
  struct ui_led_msg *msg= (struct ui_led_msg*)data;
  static int led_state;
  int led_no;
  led_no=msg->led_no;


switch(led_no)
{
   case LED1:

        led1_state=msg->cmd;
        switch (led1_state)
        {
            case CONTINUE_DISPLAY:
//                   set_gpio_pvt_on();
                   printf("LED1 ON");

            break;

            case LED_FLASH:
                 led_timer_flag=SET;
              if(led1_toggle_flag == 0)
                {
//                  reset_gpio_pvt_off();
                  led1_toggle_flag =1;
                 }
                 else
                 {
//                   set_gpio_pvt_on();
                   led1_toggle_flag =0;
                 }

            break;

            case LED_OFF:
//                reset_gpio_pvt_off();
                break;
            default:
            break;
        }
    break;

    case LED2:

        led2_state=msg->cmd;
        switch (led1_state)
        {
            case CONTINUE_DISPLAY:
                  led2_state = LED_FLASH;
                  led_timer_flag=SET;
            break;

            case LED_FLASH:
                 led_timer_flag=SET;
              if(led2_toggle_flag == 0)
                {
                   write(gp_pg,"1",1);
                   led2_toggle_flag =1;
                 }
                 else
                 {
                    write(gp_pg,"0",1);
                    led2_toggle_flag =0;
                 }

            break;

             case LED_OFF:
                 write(gp_pg,"0",1);
                 break;

            default:
            break;
        }
    break;


    case LED3:

        led3_state=msg->cmd;
        switch (led3_state)
        {
            case CONTINUE_DISPLAY:
                  led3_state = LED_FLASH;
                  led_timer_flag=SET;
            break;

            case LED_FLASH:
                 led_timer_flag=SET;
              if(led3_toggle_flag == 0)
                {
                   write(gp_pvt,"1",1);
                   led3_toggle_flag =1;
                 }
                 else
                 {
                    write(gp_pvt,"0",1);
                    led3_toggle_flag =0;
                 }

            break;

             case LED_OFF:
                 write(gp_pg,"0",1);
                 break;
            default:
            break;
        }
    break;

    case LED4:

        led4_state=msg->cmd;
        switch (led4_state)
        {
            case CONTINUE_DISPLAY:
                  led4_state = LED_FLASH;
                  led_timer_flag=SET;
            break;

            case LED_FLASH:
                 led_timer_flag=SET;
              if(led4_toggle_flag == 0)
                {
                   write(gp_pvt,"1",1);
                   led4_toggle_flag =1;
                 }
                 else
                 {
                    write(gp_pvt,"0",1);
                    led4_toggle_flag =0;
                 }

            break;

             case LED_OFF:
                 write(gp_pg,"0",1);
                 break;

            default:
            break;
        }
    break;

    default:
    break;

}
}*/


/*void update_lcd_state(void *data)
{
   struct ui_lcd_msg *msg1= (struct ui_lcd_msg*)data;
  int res,line_no;
  static int i;
  line_no=msg1->line_no;

  switch (line_no)
  {

   case LINE1:
   line1_state = msg1->cmd;

    switch(line1_state)
    {
      case MSG_FLASH:

                lcd_timer_flag=SET;
                printf("in case1 MSG_FLASH \n");

                    if(line1_toggle_flag == 0)
                    {
                    printf("LINE1 ON \n");
                    sprintf(lcd_message.line1_Msg,msg1->text);
//                    update_display(&lcd_message);
                    line1_toggle_flag =1;
                    }

                 else
                 {
                    printf("LINE1 OFF \n");
                    sprintf(lcd_message.line1_Msg,"     ");
//                    update_display(&lcd_message);
                    line1_toggle_flag =0;
                 }

      break;

    case CONTINUE_DISPLAY:
              //        printf("in case CONTINUE_DISPLAY \n");
                      sprintf(lcd_message.line1_Msg,msg1->text);
//                      update_display(&lcd_message);

                   break;


      default:
      break;
    }

   break;

   case LINE2:

    line2_state = msg1->cmd;


    switch(line2_state)
    {
      case MSG_FLASH:

                lcd_timer_flag=SET;
                printf("in case2 MSG_FLASH \n");
                if( line2_toggle_flag== 0)
                {
                   printf("LINE2 ON \n");
                   sprintf(lcd_message.line2_Msg,msg1->text);
//                   update_display(&lcd_message);
                   line2_toggle_flag =1;
                 }

                 else
                 {
                 //send_msg_to_lcd(led_no,OFF);
                    printf("LINE2 OFF \n");
                    sprintf(lcd_message.line2_Msg,"                   ");
//                    update_display(&lcd_message);
                   line2_toggle_flag=0;
                 }

      break;

    case CONTINUE_DISPLAY:
        //              printf("in case CONTINUE_DISPLAY \n");
                      sprintf(lcd_message.line2_Msg,msg1->text);
//                      update_display(&lcd_message);
                       printf("lcd_message.line1_Msg \n",lcd_message.line1_Msg);

                   break;


      default:
      break;
    }


   break;

   case LINE3:

   line3_state = msg1->cmd;


    switch(line3_state)
    {

      case MSG_FLASH:

                lcd_timer_flag=SET;
                printf("in case3 MSG_FLASH \n");
                if(line3_toggle_flag== 0)
                {
                   printf("LINE3 ON \n");
                   sprintf(lcd_message.line3_Msg,msg1->text);
//                   update_display(&lcd_message);
                   line3_toggle_flag=1;
                 }

                 else
                 {
                 //send_msg_to_lcd(led_no,OFF);
                    printf("LINE3 OFF \n");
                    sprintf(lcd_message.line3_Msg,"     ");
//                    update_display(&lcd_message);
                   line3_toggle_flag =0;
                 }

      break;

    case CONTINUE_DISPLAY:
        //              printf("in case CONTINUE_DISPLAY \n");
                      sprintf(lcd_message.line3_Msg,msg1->text);
//                      update_display(&lcd_message);

                   break;


      default:
      break;
    }

   break;

   case LINE4:

  //  printf("in case line1 \n");
 //   printf("msg1->line_no=%d \n",msg1->line_no);
     line4_state = msg1->cmd;


    switch(line4_state)
    {


      case MSG_FLASH:

                lcd_timer_flag=SET;
                printf("in case4 MSG_FLASH \n");
                if(line4_toggle_flag == 0)
                {
                   printf("LINE4 ON \n");
                   sprintf(lcd_message.line4_Msg,msg1->text);
//                   update_display(&lcd_message);
                   line4_toggle_flag=1;
                 }

                 else
                 {
                 //send_msg_to_lcd(led_no,OFF);
                    printf("LINE4 OFF \n");
                    sprintf(lcd_message.line4_Msg,"     ");
//                    update_display(&lcd_message);
                    line4_toggle_flag =0;
                 }

      break;

    case CONTINUE_DISPLAY:

                      sprintf(lcd_message.line4_Msg,msg1->text);
//                      update_display(&lcd_message);

                   break;


      default:
      break;
    }

   break;

   default:
   break;

}





}*/

int update_lcd_msg(char *data,int line_no,int clear_lcd,int position)
{
    /*int fd;

    printf("***************OPENING LCD DRIVER ****************\n");

    fd = open("/dev/klcd",O_WRONLY | O_NDELAY);
    if(fd < 0)
    {
        printf("ERR :unable to open LCD DEVICE ...\n");
        return -1;
    }*/
    struct ioctl_mesg msg;
    memset(&msg ,0,sizeof(struct ioctl_mesg));

    switch(clear_lcd)
    {
        case LCD_CLR_BOTH_LINE:
                    if(ioctl(lcd_fd ,(unsigned int)IOCTL_CLEAR_DISPLAY , &msg) < 0)
                    perror("[ERROR] IOCTL_CLEAR_DISPALY \n");
                 //   printf("....both line clear\n");
                    break;

        case LCD_CLR_FIRST_LINE:
                    msg.lineNumber =  1;
                    msg.nthCharacter = 0;
                    memcpy(&msg.kbuf,"                ",16);
                    printf("msg.kbuf =%s \n",msg.kbuf);
                    if(ioctl(lcd_fd, (unsigned int) IOCTL_PRINT_ON_FIRSTLINE,&msg) < 0 )
                    perror("[ERROR] IOCTL_PRINT_ON_FIRSTLINE \n");
                  //  printf("....first line clear\n");
                    break;

        case LCD_CLR_SECOND_LINE:
                    msg.lineNumber =  2;
                    msg.nthCharacter = 0;
                    memcpy(&msg.kbuf,"                ",16);
                    printf("msg.kbuf =%s \n",msg.kbuf);
                    if(ioctl(lcd_fd, (unsigned int) IOCTL_PRINT_ON_SECONDLINE,&msg) < 0 )
                    perror("[ERROR] IOCTL_PRINT_ON_SECONDLINE \n");
                   // printf("....second line clear\n");
                    break;

        case LCD_CLR_11_POSITION:
                    msg.lineNumber =  line_no;
                    msg.nthCharacter = position;
                    memcpy(&msg.kbuf,"                ",5);//16-11
                    printf("msg.kbuf =%s \n",msg.kbuf);
                    if(ioctl(lcd_fd, (unsigned int) IOCTL_PRINT_WITH_POSITION,&msg) < 0 )
                    perror("[ERROR] IOCTL_PRINT_WITH_POSITION \n");
                  //  printf("....LCD_CLR_11_POSITION line clear\n");
                    break;
       case LCD_CLR_FIRST_10_POSITION:
                    msg.lineNumber =  line_no;
                    msg.nthCharacter = 0;
                    memcpy(&msg.kbuf,"                ",10);//0-9
                    printf("msg.kbuf =%s \n",msg.kbuf);
                    if(ioctl(lcd_fd, (unsigned int) IOCTL_PRINT_WITH_POSITION,&msg) < 0 )
                    perror("[ERROR] IOCTL_PRINT_WITH_POSITION \n");
                   // printf(".... LCD_CLR_FIRST_10_POSITION line clear\n");
                    break;
    }
    if(position == 0)
    {

            if(line_no == 1)
            {

                msg.lineNumber = 1;
                msg.nthCharacter = 0;
                memcpy(&msg.kbuf,data,16);
                printf("msg.kbuf = %s \n",msg.kbuf);
                if(ioctl(lcd_fd, (unsigned int) IOCTL_PRINT_ON_FIRSTLINE,&msg) < 0)
                perror("[ERR]: IOCTL_PRINT_ON_FIRSTLINE \n");
            }
            else if((line_no == 2) && (clear_lcd ==LCD_CLR_FIRST_10_POSITION))
            {
              //  printf("*****inside LCD_CLR_FIRST_10_POSITION\n");
                msg.lineNumber = 2;
                msg.nthCharacter = 0;
                memcpy(&msg.kbuf,data,10);
                printf("msg.kbuf = %s \n",msg.kbuf);
                if(ioctl(lcd_fd, (unsigned int) IOCTL_PRINT_ON_SECONDLINE,&msg) < 0)
                perror("[ERR]: IOCTL_PRINT_ON_SECONDLINE \n");

            }
            else if(line_no == 2)
            {
               // printf("*****inside line_no == 2\n");
                msg.lineNumber = 2;
                msg.nthCharacter = 0;
                memcpy(&msg.kbuf,data,16);
                printf("msg.kbuf = %s \n",msg.kbuf);
                if(ioctl(lcd_fd, (unsigned int) IOCTL_PRINT_ON_SECONDLINE,&msg) < 0)
                perror("[ERR]: IOCTL_PRINT_ON_SECONDLINE \n");

            }


    }
    else
    {
      // printf("*****inside (position-16)\n");
        msg.lineNumber = line_no;
        msg.nthCharacter = position;
        memcpy(&msg.kbuf,data,(16-position));
        printf("msg.kbuf = %s \n",msg.kbuf);
        if(ioctl(lcd_fd, (unsigned int) IOCTL_PRINT_WITH_POSITION,&msg) < 0)
        perror("[ERR]: IOCTL_PRINT_WITH_POSITION \n");
    }


}
int update_lcd_state(void *data)
{
    struct ui_lcd_msg *msg1 = (struct ui_lcd_msg*)data;
    int res,line_no,position;
    static int i;
    int clear_bit_of_lcd = msg1->lcd_clr_bit;
    line_no = msg1->line_no;
    position = msg1->position;

//printf("msg1->line_no=%d\n",msg1->line_no);

    switch(line_no)
    {

        case LINE1:

             line1_state = msg1->cmd;
             switch(line1_state)
             {
                case MSG_FLASH:
                                break;

                case CONTINUE_DISPLAY:

                           sprintf(lcd_message.line1_Msg,msg1->text);
                           update_lcd_msg(lcd_message.line1_Msg,line_no,clear_bit_of_lcd,position);
                           break;
             }
             break;

       case LINE2:

             line2_state = msg1->cmd;
             switch(line2_state)
             {
                  case MSG_FLASH:
                        break;

                  case CONTINUE_DISPLAY:
                            sprintf(lcd_message.line2_Msg,msg1->text);
                            update_lcd_msg(lcd_message.line2_Msg,line_no,clear_bit_of_lcd,position);

                        break;
             }
             break;
    }

}

/*void open_lcd()
{


    printf("***************OPENING LCD DRIVER ****************\n");

    lcd_fd = open("/dev/klcd",O_WRONLY | O_NDELAY);
    if(lcd_fd < 0)
    {
        printf("ERR :unable to open LCD DEVICE ...\n");
        return -1;
    }
}*/

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

    if(g_itimer_fd<0)
    {
     perror("timerfd create:");
     printf("timer no created \n");
    }

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
 * Function name: UI_sock_create                                                           *
 * Input arg    : void                                                                     *
 * Output arg   : NONE                                                                     *
 * Description  : This function creates socket to communicate Voip_task process.           *
 *                                                                                         *
********************************************************************************************/

void UI_sock_create ()
{
printf("Inside UI SOCK");

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

    printf("i_events %d\n",i_events);
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
	/*if(signal(SIGIO,&keypad_sig_handler) == SIG_ERR) {
		acl_log(ACL_LOG_ERROR,"keypad_sig_handler not registered\r\n");
		return;
	}*

	/* open char driver */
	if((g_iKeyEventFD = open("/dev/keypad0",O_RDWR))== -1)
	{
    printf("can't open keypad device\r\n");
    return;
	}


	/* code asynchronous notification */
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
{
  struct timeval timeout;
  int   i_fd;
  int   i_res;
  int   i_result;
  int   i_rc;
  uint64_t expirations;
  char  c_buffer[TWENTY_FIVE];

  int i_ret,sre_ret,i_ptt_pressed=CLEAR,i_cnt=CLEAR;
  unsigned char uc_call_type;
  volatile int i_ret1=0;
  char buf[FIVE],call_type[FOUR];
  int  n;
  UI_sock_create();
  create_timer_fd();
   //open_lcd();


while(1)
{

  testset = readset;
  timeout.tv_sec = 1;
  timeout.tv_usec = 500000;

//result=select(FD_SETSIZE,&testset,NULL,NULL,&timeout);

  i_result=pselect(FD_SETSIZE,&testset,NULL,NULL,&timeout,&pselect_set);
          //  printf("adding client on fd %d\n",result);
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
              char time_string[FOURTY];
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
                }
              }

              //to print private call timeout on LCD

              if(pvt_call.pvt_call_busy)
              {
             // printf("************** pvt_call.pvt_call_busy*************\n");
                if(g_uiPvt_call_timer_ui_lcd)
                  {
                    //printf("************** g_uiPvt_call_timer_ui_lcd %d\n",g_uiPvt_call_timer_ui_lcd);
                    g_uiPvt_call_time_lcd = (g_uiPvt_call_timer_ui_lcd / 10);
                   //printf("************** g_uiPvt_call_time_lcd %d\n",g_uiPvt_call_time_lcd);
                   //if((g_uiPvt_call_timer_ui_lcd  % 2) == 0)
                    if((g_uiPvt_call_timer_ui_lcd%10) == 0)
                    {

                        // PvtCallTimeArr[PvtCallTimeArrIndex] = g_uiPvt_call_time_lcd;
                      // if((PvtCallTimeArrIndex%5) == 0)
                       // {
                            // printf("************** inside PVT CALL%d\n",g_uiPvt_call_timer_ui_lcd);

                             //PvtCallTime_min = ( PvtCallTimeArr[PvtCallTimeArrIndex]/60);
                             //PvtCallTime_sec = ( PvtCallTimeArr[PvtCallTimeArrIndex]% 60);

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
                             update_lcd_msg(g_arcPvtCallTimer,LINE2,LCD_CLR_11_POSITION,11);


                       // }
                       // PvtCallTimeArrIndex++;

                    }
                    g_uiPvt_call_timer_ui_lcd--;

                  }



                  //Page_call_init_timeout
                  else if(g_uiPage_call_timer_frlcd_ini)
                  {
                     g_uipg_call_time_lcd_init = (g_uiPage_call_timer_frlcd_ini / 10);

                     if((g_uiPage_call_timer_frlcd_ini%10) == 0)
                       {
                        //printf("************** inside PVT OF PG_CALL %d\n",g_uiPage_call_timer_frlcd_ini);

                             //PvtCallTime_min = ( PvtCallTimeArr[PvtCallTimeArrIndex]/60);
                             //PvtCallTime_sec = ( PvtCallTimeArr[PvtCallTimeArrIndex]% 60);

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
                             update_lcd_msg(g_arcPvtCallTimer_fr_pg_init,LINE1,LCD_CLR_11_POSITION,11);
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
                        // printf("************** inside PG CALL %d\n",g_uiPvt_call_timer_ui_lcd);

                             //PvtCallTime_min = ( PvtCallTimeArr[PvtCallTimeArrIndex]/60);
                             //PvtCallTime_sec = ( PvtCallTimeArr[PvtCallTimeArrIndex]% 60);

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
                             update_lcd_msg(g_arcPgCallTimer,LINE1,LCD_CLR_11_POSITION,11);


                       }
                       g_uiPage_call_timer_lcd--;

                  }
               }


             /* ///SRE MUTE activation 12-03-22

                sre_ret=operate_gpio(SRE_MUTE_ACT,READ,CLEAR);
               // printf("*******sre_ret=%d******\n",sre_ret);
                if(sre_ret==SET)
                {
                    g_uiFsu_event=SRE_MUTE_EVENT;
                    // printf("*******SRE_MUTE_EVENT is set in ui.c******\n");

                }
                else
                {
                     g_uiFsu_event=SRE_UNMUTE_EVENT;
                    // printf("*******SRE_UNMUTE_EVENT is set in ui.c******\n");
                }

*/
                ///////////////////gpio thread is removed and gpio is monitored here//////////////////

                pthread_mutex_lock(&lock_call_type);
                uc_call_type=pvt_call.type;
                pthread_mutex_unlock(&lock_call_type);

                if(pvt_call.pvt_call_busy==SET || uc_call_type==INTERCOM_RX||uc_call_type==INTERCOM_TX_RX)

                {
                     i_ret=operate_gpio(HOOK_SW_SENSE,READ,CLEAR);

                    if(i_ret== 0)
                    {
                        if(g_iHandset_lifted_signal==SET)
                        {
                            operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_OFF);
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
                                    clear_validation_processing();
                                    printf("Handset on cradle \n");

                                break;

                            }
                        }

                    }
                    else //handset lifted
                    {
                        g_iHandset_lifted_signal=SET;
                        operate_gpio(ICOM_MIX_ACT,WRITE,GPIO_ON);  ///Pvt and page audio mixing
                        if(pg_call.pg_call_busy!=SET && stn_config.default_AIM==CLEAR)
                        {
                            operate_gpio(MB_AMP_MUTE,WRITE,GPIO_OFF); ///on means page speaker mute
                        }
                    }
                }


                if(pWork_m.private_state==PRIVATE_OUTGOING_ONGOING)
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
                }

            }


                     /*if(led_timer_flag)
                      {
                       led_timer_flag=CLEAR;

                         switch(led_msg.led_no)
                        {
                         case LED1:
                          if(led_msg.flash_cntr[0]!=CLEAR)
                          update_led_state(&led_msg);
                          led_msg.flash_cntr[0]--;
                          break;

                        case LED2:
                        if(led_msg.flash_cntr[1]!=CLEAR)
                         update_led_state(&led_msg);
                         led_msg.flash_cntr[1]--;
                          break;

                        case LED3:
                           if(led_msg.flash_cntr[2]!=CLEAR)
                         update_led_state(&led_msg);
                         led_msg.flash_cntr[2]--;
                         break;

                        case LED4:
                         if(led_msg.flash_cntr[3]!=CLEAR)
                          update_led_state(&led_msg);
                          led_msg.flash_cntr[3]--;
                         break;
                        default:
                         break;
                        }

                    }*/

            }





          } //timer fd

        if(i_fd == g_iconnection_fd) //if(g_uiUi_socket_fd)
        {
         // i_rc= read(i_fd,&c_Fsu_state,sizeof(char));
          printf("UI FSU STATE IS = %c \n",c_Fsu_state);
          int rd;
            struct UI_msg rx_msg;
            rd = read(g_iconnection_fd, (void*)&rx_msg,sizeof(struct UI_msg));
            if(rd ==-1)
            {
                printf("read() failed...\n");
            }
            printf("read() success...\n");

            c_Fsu_state = rx_msg.state;
            //i_rc= read(i_fd,&c_Fsu_state,sizeof(char));
            printf("UI FSU STATE IS = %c \n",c_Fsu_state);

            if(rx_msg.parameter == LED)
            {
                printf("INSIDE LED...\n");

            }
            else
            {
               // printf("INSIDE LCD ... \n");
                lcd_msg.parameter = rx_msg.parameter;
                lcd_msg.line_no = rx_msg.line_no;
                lcd_msg.cmd = rx_msg.cmd;
                lcd_msg.lcd_clr_bit = rx_msg.lcd_clear_bit;
                lcd_msg.position = rx_msg.position;
                strcpy(lcd_msg.text,rx_msg.text);

                update_lcd_state(&lcd_msg);
            }


              /*      rc = read(fd,param,sizeof(char)*59);
                     if(rc < 0)
                      {
                       perror("Server-read() error");
                        close(fd);
                     FD_CLR(fd, &readset);
                     // exit (-1);
                    }
                    else if (rc == 0)
                     {
           //      printf("Client program has issued a close()\n");
                       close(fd);
                        FD_CLR(fd, &readset);
                    //   exit(-1);
                     }

                else
                {
                 struct UI_msg *p = (struct UI_msg *)param;
                 short int j;
                 if(p->parameter == LED)
                 {
                    printf("LED MESSAGE ON");
                    short int i;
                   led_msg.led_no = p->line_no;
                   led_msg.cmd=p->cmd;
                   led_msg.flash_cntr[(p->line_no)-1]=p->flash_cntr[(p->line_no)-1];
                  for(j=0; j<21;j++)
                  {
                     led_msg.text[j] = 0;
                  }
                update_led_state(&led_msg);
                }

                else
                {
                      short int i,j;
                      i=p->line_no;
                      lcd_msg.line_no = p->line_no;
                      lcd_msg.cmd = p->cmd;
                  //   ui_lcd_msg[0].attr = p->attr;
                    lcd_msg.flash_cntr[(p->line_no)-1] = p->flash_cntr[(p->line_no)-1];
                    for(j=0; j<21;j++)
                    {
                     lcd_msg.text[j] = p->text[j];
                    }
                  printf("message is=%s \n",lcd_msg.text);

                  update_lcd_state(&lcd_msg);

                   bzero(p,sizeof(p));
                 }

                                 //  printf("Server-read() is OK\n");
              }
               free(param);*/
        } //connection fd






      } //inner if(fdset)

    }//else fdset

  } //master if(fdset)

  }//master for loop


//}//while(1)


return 0;
}



