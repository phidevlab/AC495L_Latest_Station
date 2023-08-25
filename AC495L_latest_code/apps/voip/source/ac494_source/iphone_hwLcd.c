/****************************************************************************
*
*	Company:			Audiocodes Ltd.
*
*   Project:			VoIP SoC
*
*   Hardware Module: 	AC4xx
*
*   File Name: 			iphone_hwLcd.c
*
*	Creation Date:
*
*   Last Update By:
*
******************************************************************************
*
* 	DESCRIPTION:
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
#include <sys/shm.h>
#include <signal.h>
#include <sys/ioctl.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "dsp_drv.h"

#include "iphone_hwLcd.h"
#include "lcd.h"

#define MAX_BUF_LENGTH 50
#define MAX_BUF_LEN    52

/*
struct ioctl_mesg{				// a structure to be passed to ioctl argument
	char kbuf[MAX_BUF_LENGTH];
	unsigned int lineNumber;
	unsigned int start_position;
	unsigned int i2c_device_address;
}msg;

struct ioctl_io_mesg{				// a structure to be passed to ioctl argument
	char kbuf[MAX_BUF_LENGTH];
 	unsigned int i2c_device_address;
}io_msg;*/
//#include "iphone.h"
/*
#define IOCTL_PRINT                     '1'
#define IOCTL_LCD_INITIALIZATION        '2'
#define IOCTL_IO_INITIALIZATION		    '3'
#define IOCTL_SET_BIT_IO_EXPANDER	    '4'
#define IOCTL_GET_BIT_IO_EXPANDER       '5'*/
/*typedef struct lcd_pulse_arg {
	unsigned char is_up;
	unsigned char cnt;
} LCD_PULSE_ARG;*/

/****************************************************************************
*
*	Name:		lcdString
*----------------------------------------------------------------------------
*	Abstract:	send a string to the lcd
*----------------------------------------------------------------------------
*	Input:      message - string to display
*				size	- string len
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 in case of error
******************************************************************************/
void lcdString(char *message, int size)
{
	//write(lcdFd, message, size);
} /* end of lcdString() */
/******************************************************************************/



/****************************************************************************
*
*	Name:		lcdClear
*----------------------------------------------------------------------------
*	Abstract:	clear the LCD display
*----------------------------------------------------------------------------
*	Input:      none
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 in case of error
******************************************************************************/
void lcdClear(void)
{
	LCD_POS pos;
	pos.row=pos.column=0;
	//ioctl(lcdFd, TI_LIDD_GOTO_XY, (int)&pos);
	//ioctl(lcdFd, TI_LIDD_CLEAR_SCREEN, 0);

	//NirS : I used goto 0,0 instead of cursor home (because it doesn`t supplay the desired result)

	//ioctl(lcdFd, TI_LIDD_CURSOR_HOME, 0);
} /* end of lcdClear() */
/******************************************************************************/



/****************************************************************************
*
*	Name:		lcdMoveCursor
*----------------------------------------------------------------------------
*	Abstract:	Change the cursor position.
*----------------------------------------------------------------------------
*	Input:      none
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 in case of error
******************************************************************************/
void lcdMoveCursor(int row , int column)
{

	LCD_POS pos;
	pos.row=row;
	pos.column=column;
	//ioctl(lcdFd, TI_LIDD_GOTO_XY, (int)&pos);

}



/****************************************************************************
*
*	Name:		lcdInit
*----------------------------------------------------------------------------
*	Abstract:	init the lcd
*----------------------------------------------------------------------------
*	Input:      none
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 in case of error
******************************************************************************/
/*void lcdInit(void)
{
	/* open char driver */
    /*char buf[] = {0x30,0x30,0x30,0x20,0x28,0x08,0x01,0x06,0x0C};///lcd_init_cmd_seq
    char buff[]= "WELCOME";
     if((lcdFd = open("/dev/klcd",O_RDWR))== -1) {
		printf("[%s , %d ] can't open lcd device\r\n",__FUNCTION__,__LINE__);
		return;
	 }



	 printf("klcd device is opened....\n");
     memset(&msg ,0,sizeof(struct ioctl_mesg));
     msg.i2c_device_address=0x27;
     memcpy(&msg.kbuf,buf,sizeof(buf));
     ioctl(lcdFd, (unsigned int) IOCTL_LCD_INITIALIZATION,&msg);


    memset(&msg ,0,sizeof(struct ioctl_mesg));
    msg.lineNumber =  2;
    msg.start_position = 6;
    msg.i2c_device_address=0x27;
    memcpy(&msg.kbuf,buff,sizeof(buff));
    ioctl(lcdFd, (unsigned int) IOCTL_PRINT ,&msg);*/

    /*msg.lineNumber =  1;
     msg.nthCharacter = 10;
     msg.addr=0x27;
     memcpy(&msg.kbuf,"                ",6);//16-11
     memcpy(&msg.kbuf,buff,(16-12));
     printf("msg.kbuf =%s \n",msg.kbuf);
     if(ioctl(lcdFd, (unsigned int) IOCTL_ADDR,&msg) < 0 )
     perror("[ERROR] IOCTL_PRINT_WITH_POSITION \n");

     char buf[] = {0x4F};//,0x30,0x30,0x20,0x28,0x08,0x01,0x06,0x0C};///lcd_init_cmd_seq
     char buff[]= {0x8F};
     char arr[1];
    int ret_val;
     if((lcdFd = open("/dev/klcd",O_RDWR))== -1) {
		printf("[%s , %d ] can't open lcd device\r\n",__FUNCTION__,__LINE__);
		return;
	 }

	 printf("klcd device is opened....\n");
     memset(&io_msg ,0,sizeof(struct ioctl_io_mesg));
     io_msg.i2c_device_address=0x24;
     memcpy(&io_msg.kbuf,buf,sizeof(buf));
     printf("i2c_device_address=%x\n",io_msg.i2c_device_address);
     ioctl(lcdFd, (unsigned int) IOCTL_IO_INITIALIZATION,&io_msg);


    /*  printf("klcd device is opened....\n");
     memset(&msg ,0,sizeof(struct ioctl_mesg));
     msg.i2c_device_address=0x24;
     memcpy(&msg.kbuf,buf,sizeof(buf));
     printf("i2c_device_address=%x\n",msg.i2c_device_address);
     ioctl(lcdFd, (unsigned int) IOCTL_LCD_INITIALIZATION,&msg);



     memset(&io_msg ,0,sizeof(struct ioctl_io_mesg));
     io_msg.i2c_device_address=0x24;
     memcpy(&io_msg.kbuf,buff,sizeof(buff));
      ioctl(lcdFd, (unsigned int) IOCTL_SET_BIT_IO_EXPANDER,&io_msg);

    //  memset(&io_msg ,0,sizeof(struct ioctl_io_mesg));
   //  io_msg.i2c_device_address=0x24;
   //  memcpy(&io_msg.kbuf,buff,sizeof(buff));
   // while(1){
     ret_val= ioctl(lcdFd,(unsigned int) IOCTL_GET_BIT_IO_EXPANDER,arr);
     if (ret_val < 0) {
		printf("ioctl_get_msg failed:%d\n", ret_val);
		exit(-1);
	}
      printf("arr is %x\n", arr[0]);
//}
	 close(lcdFd);

} *//* end of lcdInit() */
/******************************************************************************/




/****************************************************************************
*
*	Name:		lcdClose
*----------------------------------------------------------------------------
*	Abstract:	close the lcd file descriptor
*----------------------------------------------------------------------------
*	Input:      none
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 in case of error
******************************************************************************/
/*void lcdClose(void)
{
	if (lcdFd<0)
		printf ("[%s,%d] the LCD file descriptor has not been initialized \n ",__FUNCTION__,__LINE__);
	else
	{
		close(lcdFd);
		lcdFd=-1;
	}


}*/ /* end of lcdClose() */
/******************************************************************************/


