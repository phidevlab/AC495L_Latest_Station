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
#include <signal.h>
#include <sched.h> 

#include <sys/types.h>
#include <sys/stat.h>  
#include <sys/time.h>     /* struct timeval definition           */
#include <asm/errno.h>


#include <errno.h>
#include <sys/ioctl.h>

#include "dsp_drv.h"


//#include "iphone.h"
#include "acl_log.h"

#include "ssp_rtc.h"
#include "ssp_drv.h"

/*#define  ACL_DEBUG*/

#ifdef ACL_DEBUG
#define ACL_LogDebug(a1,a2...)	printf(a1, ##a2)
#else
 #define ACL_LogDebug(a1,a2...)
#endif

 
static int rtcFd;
pthread_mutex_t request_mutex;
pthread_cond_t got_request; 


char *getsn(char *buf, int n)
{

	int 	ch;
	char    *ptr = buf;

	while(((ch = getchar()) != '\n') && n--)
	{
		if(ch == EOF)
		{
			if(ptr == buf) return NULL;
			break;
		}

		*ptr++ = (char)ch;

	}

	*ptr = '\0';
	return buf;

}




int open_rtc( void )
{

  if( (rtcFd = open( "/dev/i2c/0", O_RDWR )) < 0 )
  {
     perror( "open(/dev/i2c/0) failed " );
     return -1;
  }

  return 0;
}

/* write a value to specified RTC register */
int write_rtc(unsigned char addr, unsigned char data)
{
  unsigned char write_data[2];

  write_data[0] = addr;
  write_data[1] = data;

  if( ioctl( rtcFd, I2C_SET_ADDR, RTC_ADDR ) < 0 )
  {
#ifdef DEBUG
     perror( "Error: Can't set address\n" );
#endif
     return -1;
  }

  if( write( rtcFd, write_data, 2 ) < 0 )
  {
#ifdef DEBUG
     perror( "Error: Can't write data\n" );
#endif
     return -1;
  }

  return 0;
}

/* read a value from RTC */
int read_rtc(unsigned char addr, unsigned char* data)
{
  if( !data )
     return -1;
  else
     data[ 0 ] = 0;

  if( ioctl( rtcFd, I2C_SET_ADDR, RTC_ADDR ) < 0 )
  {
  #ifdef DEBUG
       perror( "Error: Can't set address\n" );
  #endif
       return -1;
  }


  if( write( rtcFd, &addr, 1 ) < 0 )
  {
#ifdef DEBUG
     perror( "Error writing address\n" );
#endif
     return -1;
  }

  if( read( rtcFd, data, 1 ) < 0 )
  {
#ifdef DEBUG
     perror( "Error reading data\n" );
#endif
     return -1;
  }

  return 0;
}

unsigned char int_to_bcd(unsigned char integer)
{
  return ( (integer/10 << 4) | (integer % 10) );
}

unsigned char bcd_to_int(unsigned char bcd)
{
  return ( (bcd >> 4) * 10 ) + (bcd & 0xF);
}

static char day_table[][10] = {
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};

void rtc_action(int cmd)
{
  unsigned char year, month, date, day, minutes, seconds, hours;

  unsigned char input[20];
  unsigned char *tmp=NULL;
  char **ttt=NULL;


	RTC_INFO_STCT	rtcInfo;
	char			rtcString[32];
	char			rtcStringToken[32];


  if( cmd == RTC_CMD_SET )
  {

     /* set time and date in real time clock
     * just do 12 hour clock
     */

     printf( "Input time in the form HH:MM [AM|PM] -> " );
     getsn( input, 20 );

     /* Extract hours */
//     hours = int_to_bcd( strtol( input, (char**)&tmp, 10 ) );
     hours = int_to_bcd( (unsigned char)(strtol( input, ttt, 10 )) );
     hours |= RTC_HOUR_REG_12HR_MODE;  /* just do 12 hour mode */
     tmp++;

     /* Extract minutes */
     minutes = int_to_bcd( strtol( tmp, ttt, 10 ) );
     tmp=(unsigned char*)*ttt;
     tmp++;

     if( !strcmp( tmp,"AM" ) )
     {
        hours &= ~RTC_HOUR_REG_AM_PM;
     }
     else if( !strcmp( tmp,"PM" ) )
     {
        hours |= RTC_HOUR_REG_AM_PM;
     }

     seconds = 0;  /* reset seconds and enable clock */


     write_rtc( RTC_HOURS_REG, hours );
     write_rtc( RTC_MINUTES_REG, minutes );
     write_rtc( RTC_SECONDS_REG, seconds );

     /* set the day */
     printf( "Day (1=SUN, 2=MON, etc.) -> " );
     getsn( input, 20 );
     day = strtol( input, NULL, 10 );
     write_rtc( RTC_DAY_REG, day );

     /* set the date */
     printf( "Date (1-31) -> " );
     getsn( input, 20 );
     date = int_to_bcd( strtol( input, NULL, 10 ) );
     write_rtc( RTC_DATE_REG, date );

     /* set month */
     printf( "Month (1-12) -> " );
     getsn( input, 20 );
     month = int_to_bcd( strtol( input, NULL, 10 ) );
     write_rtc( RTC_MONTH_REG, month );

     /* set the year */
     printf( "Year (00-99) -> " );
     getsn( input, 20 );
     year = int_to_bcd( strtol( input, NULL, 10 ) );
     write_rtc( RTC_YEAR_REG,year );

  }
  else if( cmd == RTC_CMD_SHOW )
  {

     printf( "Current date and time is:\n" );

     /* print day */
     read_rtc( RTC_DAY_REG, &day );
    	rtcInfo.day = day;
	if( day ) 
       printf( day_table[day-1] );

     /* print month */
     read_rtc( RTC_MONTH_REG, &month );
		rtcInfo.month = month;	
     printf( " %02d", bcd_to_int( month & 0x1F ) );
     printf( "/" );

     /* print date */
     read_rtc( RTC_DATE_REG, &date );
		rtcInfo.date = date;	
     printf( "%02d", bcd_to_int( date & 0x3F ) );
     printf( "/" );

     /* print year */
     read_rtc( RTC_YEAR_REG, &year );
		rtcInfo.year = year;
     printf( "%02d", bcd_to_int( year ) );
     printf( "  " );

     /* print hours */
     read_rtc( RTC_HOURS_REG, &hours );
		rtcInfo.hours = hours;
     printf( "%02d", bcd_to_int( hours & 0x1F ) );
     printf( ":" );

     /* print minutes */
     read_rtc( RTC_MINUTES_REG, &minutes );
		rtcInfo.minutes = minutes;
     printf( "%02d", bcd_to_int( minutes & 0x7F ) );
     printf( ":" );

     /* print seconds */
     read_rtc( RTC_SECONDS_REG, &seconds );
		rtcInfo.seconds = seconds;
     printf( "%02d", bcd_to_int( seconds & 0x7F ) );

     if( hours & RTC_HOUR_REG_AM_PM )
     {
		rtcInfo.am = 0;
        printf( " PM" );
     }
     else
     {
		rtcInfo.am = 1;
        printf( " AM" );
     }
     printf( "\n" );


	sprintf(rtcString," ");
	sprintf(rtcStringToken," ");

	sprintf( rtcStringToken,day_table[day-1] );

	strcat(rtcString,rtcStringToken);

    sprintf( rtcStringToken," %02d", bcd_to_int( rtcInfo.month & 0x1F ) );
	strcat(rtcString,rtcStringToken);
	strcat(rtcString,"/");

    sprintf( rtcStringToken,"%02d", bcd_to_int( rtcInfo.date & 0x3F ) );
	strcat(rtcString,rtcStringToken);
	strcat(rtcString,"/");

    sprintf( rtcStringToken,"%02d", bcd_to_int( rtcInfo.year & 0x3F ) );
	strcat(rtcString,rtcStringToken);
	strcat(rtcString," ");

    sprintf( rtcStringToken,"%02d", bcd_to_int( rtcInfo.hours & 0x1F ) );
	strcat(rtcString,rtcStringToken);
	strcat(rtcString,":");

    sprintf( rtcStringToken,"%02d", bcd_to_int( rtcInfo.minutes & 0x7F ) );
	strcat(rtcString,rtcStringToken);
	strcat(rtcString,":");

    sprintf( rtcStringToken,"%02d", bcd_to_int( rtcInfo.seconds & 0x7F ) );
	strcat(rtcString,rtcStringToken);

    if(rtcInfo.am)
		strcat(rtcString,"am");
	else
		strcat(rtcString,"pm");


	printf("rtcString = %s\r\n",rtcString);
#if 0
#ifdef AC494_SDP_IPPHONE
	rtcLcdPhyTerm_api(	"at/hs", rtcString, 24);
#endif
#endif

  }
  else
  {
     printf( "Unknown command\n" );
  }
}














void rtc_lcdString(void)
{
  unsigned char year, month, date, day, minutes, seconds, hours;
/* NirM, 080331, not used */
//  unsigned char input[20];
//  unsigned char *tmp;


	RTC_INFO_STCT	rtcInfo;
	char			rtcString[32];
	char			rtcStringToken[32];

     /* print day */
     read_rtc( RTC_DAY_REG, &day );
    	rtcInfo.day = day;
 
     /* print month */
     read_rtc( RTC_MONTH_REG, &month );
		rtcInfo.month = month;
 
     /* print date */
     read_rtc( RTC_DATE_REG, &date );
		rtcInfo.date = date;

     /* print year */
     read_rtc( RTC_YEAR_REG, &year );
		rtcInfo.year = year;

     /* print hours */
     read_rtc( RTC_HOURS_REG, &hours );
		rtcInfo.hours = hours;
 
     /* print minutes */
     read_rtc( RTC_MINUTES_REG, &minutes );
		rtcInfo.minutes = minutes;

     /* print seconds */
     read_rtc( RTC_SECONDS_REG, &seconds );
		rtcInfo.seconds = seconds;

     if( hours & RTC_HOUR_REG_AM_PM )
     {
		rtcInfo.am = 0;
      }
     else
     {
		rtcInfo.am = 1;
     }


	sprintf(rtcString," ");
	sprintf(rtcStringToken," ");

	/*sprintf( rtcStringToken,day_table[day-1] );

	strcat(rtcString,rtcStringToken);  */

    sprintf( rtcStringToken," %02d", bcd_to_int( rtcInfo.month & 0x1F ) );
	strcat(rtcString,rtcStringToken);
	strcat(rtcString,"/");

    sprintf( rtcStringToken,"%02d", bcd_to_int( rtcInfo.date & 0x3F ) );
	strcat(rtcString,rtcStringToken);
	strcat(rtcString,"/");

    sprintf( rtcStringToken,"%02d", bcd_to_int( rtcInfo.year & 0x3F ) );
	strcat(rtcString,rtcStringToken);
	strcat(rtcString," ");

    sprintf( rtcStringToken,"%02d", bcd_to_int( rtcInfo.hours & 0x1F ) );
	strcat(rtcString,rtcStringToken);
	strcat(rtcString,":");

    sprintf( rtcStringToken,"%02d", bcd_to_int( rtcInfo.minutes & 0x7F ) );
	strcat(rtcString,rtcStringToken);

	/*strcat(rtcString,":");
    sprintf( rtcStringToken,"%02d", bcd_to_int( rtcInfo.seconds & 0x7F ) );
	strcat(rtcString,rtcStringToken);   */

    if(rtcInfo.am)
		strcat(rtcString,"am");
	else
		strcat(rtcString,"pm");

#if defined(AC494_SDP_IPPHONE) || defined(AC49_ORCHID_VOIP_TOOLKIT)
    ACL_LogDebug("---------- %s %s line-%d, rtcString %s ------- \r\n",__FILE__, __FUNCTION__, __LINE__, rtcString);
//	rtcLcdPhyTerm_api("at/hs", rtcString, 18);
#endif


}



void rtc_sleep(int seconds)
{

    
    struct timeval  now;            /* time when we started waiting        */
    struct timespec timeout;        /* timeout value for the wait function */
    int             done;           /* are we done waiting?                */

    /* first, lock the mutex */
    int rc = pthread_mutex_lock(&request_mutex);
    if (rc) { /* an error has occurred */
        perror("pthread_mutex_lock");
        exit(1);
    }
    /* mutex is now locked */
    ACL_LogDebug("---------- %s %s line-%d ------- \r\n",__FILE__, __FUNCTION__, __LINE__);
    /* get current time */ 
    gettimeofday(&now, NULL);
    /* prepare timeout value.              */
    /* Note that we need an absolute time. */
    timeout.tv_sec = now.tv_sec + seconds;
    timeout.tv_nsec = now.tv_usec * 1000; /* timeval uses micro-seconds.         */
                                          /* timespec uses nano-seconds.         */
                                          /* 1 micro-second = 1000 nano-seconds. */
    ACL_LogDebug("---------- %s %s line-%d ------- \r\n",__FILE__, __FUNCTION__, __LINE__);
    /* wait on the condition variable. */
    /* we use a loop, since a Unix signal might stop the wait before the timeout */
    done = 0;
    while (!done) {
        /* remember that pthread_cond_timedwait() unlocks the mutex on entrance */
        rc = pthread_cond_timedwait(&got_request, &request_mutex, &timeout);
        switch(rc) {
            case 0:  /* we were awakened due to the cond. variable being signaled */
                     /* the mutex was now locked again by pthread_cond_timedwait. */
                /* do your stuff here... */
                ACL_LogDebug("---------- %s %s line-%d ------- \r\n",__FILE__, __FUNCTION__, __LINE__);
                
                done = 1;
                break;
            default:        /* some error occurred (e.g. we got a Unix signal) */
                ACL_LogDebug("---------- %s %s line-%d ------- \r\n",__FILE__, __FUNCTION__, __LINE__);
                done = 1;
                /*if (errno == ETIMEDOUT) { 
                    done = 0;
                }*/
                break;      /* break this switch, but re-do the while loop.   */
        }
    }
    /* finally, unlock the mutex */
    pthread_mutex_unlock(&request_mutex);

}

void *hw_RTCJob(void* arg)
{
    if( open_rtc( ) == -1 ) {
	    printf("error at open_rtc \r\n");
        return NULL;
    }
    ACL_LogDebug("---------- %s %s line-%d ------- \r\n",__FILE__, __FUNCTION__, __LINE__);
    rtc_sleep(5);
    for(;;) {
	    rtc_lcdString();
        rtc_sleep(20);
    }

}

int rtcHandler(void)
{
	pthread_t			tid;
	struct sched_param	sched;
	pthread_attr_t		attr;


	if(pthread_cond_init(&got_request, NULL) < 0){
		acl_log(ACL_LOG_ERROR,"error at pthread_cond_init\r\n");
		return -1;
	}

	if(pthread_mutex_init(&request_mutex, NULL) < 0){
		acl_log(ACL_LOG_ERROR,"error at pthread_mutex_init\r\n");
		return -1;
	}


	/* create the hw_UDPtxJob thread */
	if( (pthread_attr_init(&attr) != 0) ){
		acl_log(ACL_LOG_ERROR,"pthread_attr_init");
		exit(1);
	}
	if( (pthread_attr_setschedpolicy(&attr, SCHED_RR) != 0) ){
		acl_log(ACL_LOG_ERROR,"pthread_attr_setschedpolicy");
		exit(1);
	}
    ACL_LogDebug("---------- %s %s line-%d ------- \r\n",__FILE__, __FUNCTION__, __LINE__);
  	sched.sched_priority = 72;	
 	if( (pthread_attr_setschedparam(&attr, &sched) != 0) ){
		acl_log(ACL_LOG_ERROR,"pthread_attr_setschedparam");
		exit(1);
	}
 	if( (pthread_create( &tid, &attr, hw_RTCJob, NULL) != 0) ){
		acl_log(ACL_LOG_ERROR,"pthread_create - hw_RTCJob");
		exit(1);
	}

    return 0;
}









#if 0
int main( int argc, char *argv[] )
{
  	int action;


 	if( open_rtc( ) == -1 ) {
		printf("error at open_rtc \r\n");
    	return -1;
	}


  for( ;; )
  {

     action = 0;

     while( action < 1 || action > 3 )
     {
        printf( "\nChoose action\n" );
        printf( "1 - Set RTC\n" );
        printf( "2 - Get RTC\n" );
        printf( "3 - Exit\n" );

        scanf( "%d", &action );
        getc( stdin );

     }

     switch( action )
     {
        case 1:
           rtc_action( RTC_CMD_SET );
           break;

        case 2:
           rtc_action( RTC_CMD_SHOW );	
           break;

        case 3:
           return 0;

        default:
           break;
     }
  }

	return 0;
}

#endif
