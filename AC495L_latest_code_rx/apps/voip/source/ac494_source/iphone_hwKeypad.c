/****************************************************************************
*
*	Company:			Audiocodes Ltd.
*
*   Project:			VoIP SoC
*
*   Hardware Module: 	AC4xx
*
*   File Name: 			iphone_hwKeypad.c
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

#include <sys/types.h>
#include <sys/stat.h>

#include "dsp_drv.h"


//#include "iphone.h"

#include <keypad.h>
#include "acl_keypad_adaptation_layer.h"
#include "acl_call_mngr.h"
#include "acl_log.h"

#ifdef NANOX
#define MWINCLUDECOLORS
#include "nano-X.h"
#include "nanox_hwLcd.h"
#endif

#define MAX_EVENTS_TO_READ 256
#define HOOK_STATUS 1

int keypadFd;
int hookFd = -1;
pthread_t  	startHookTid;
int 	startHookTerminate = 0;

#if  defined(KEYMAP_SDP)||defined(KEYMAP_ORCHID_MAIN)
/*notice that the order is col,row.*/
static keypad_keys digitToKeyArr[4][6] =
{
 	{KEYPAD_KEY_DIGIT_1,KEYPAD_KEY_DIGIT_2,KEYPAD_KEY_DIGIT_3,KEYPAD_KEY_LINE_1,KEYPAD_KEY_LINE_2,KEYPAD_KEY_HOLD},
 	{KEYPAD_KEY_DIGIT_4,KEYPAD_KEY_DIGIT_5,KEYPAD_KEY_DIGIT_6,KEYPAD_KEY_TRANSFER,KEYPAD_KEY_INVALID,KEYPAD_KEY_SPEAKER},
 	{KEYPAD_KEY_DIGIT_7,KEYPAD_KEY_DIGIT_8,KEYPAD_KEY_DIGIT_9,KEYPAD_KEY_VOLUME_UP,KEYPAD_KEY_VOLUME_DOWN,KEYPAD_KEY_OFF_HOOK},
 	{KEYPAD_KEY_ASTERIKS,KEYPAD_KEY_DIGIT_0,KEYPAD_KEY_POUND,KEYPAD_KEY_CONFERENCE,KEYPAD_KEY_MUTE,KEYPAD_KEY_ON_HOOK}
};
#elif defined(KEYMAP_310HD)
static keypad_keys digitToKeyArr[8][8]
	= {	{KEYPAD_KEY_DIGIT_1,KEYPAD_KEY_DIGIT_2,	KEYPAD_KEY_DIGIT_3,	KEYPAD_KEY_DIGIT_4,	KEYPAD_KEY_DIGIT_5,	KEYPAD_KEY_DIGIT_6, KEYPAD_KEY_DIGIT_7, KEYPAD_KEY_INVALID},
		{KEYPAD_KEY_DIGIT_8,KEYPAD_KEY_DIGIT_9,	KEYPAD_KEY_DIGIT_0,	KEYPAD_KEY_ASTERIKS,KEYPAD_KEY_POUND,   KEYPAD_KEY_SPEAKER, KEYPAD_KEY_INVALID, KEYPAD_KEY_INVALID},
		{KEYPAD_KEY_LINE_2, KEYPAD_KEY_HOLD,	KEYPAD_KEY_TRANSFER,KEYPAD_KEY_INVALID,	KEYPAD_KEY_INVALID,	KEYPAD_KEY_INVALID, KEYPAD_KEY_INVALID, KEYPAD_KEY_INVALID},
		{KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,	KEYPAD_KEY_INVALID,	KEYPAD_KEY_LINE_1,	KEYPAD_KEY_INVALID,	KEYPAD_KEY_MUTE,    KEYPAD_KEY_HEADSET, KEYPAD_KEY_INVALID},
		{KEYPAD_KEY_CONFERENCE,KEYPAD_KEY_INVALID,KEYPAD_KEY_VOLUME_UP,KEYPAD_KEY_VOLUME_DOWN,	KEYPAD_KEY_INVALID,	KEYPAD_KEY_INVALID, KEYPAD_KEY_INVALID, KEYPAD_KEY_INVALID},
		{KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,	KEYPAD_KEY_INVALID,	KEYPAD_KEY_INVALID,	KEYPAD_KEY_INVALID,	KEYPAD_KEY_INVALID, KEYPAD_KEY_INVALID, KEYPAD_KEY_INVALID},
		{KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,	KEYPAD_KEY_INVALID,	KEYPAD_KEY_INVALID,	KEYPAD_KEY_INVALID,	KEYPAD_KEY_INVALID, KEYPAD_KEY_INVALID, KEYPAD_KEY_INVALID},
		{KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,	KEYPAD_KEY_INVALID,	KEYPAD_KEY_INVALID,	KEYPAD_KEY_INVALID,	KEYPAD_KEY_INVALID, KEYPAD_KEY_INVALID, KEYPAD_KEY_INVALID},
};
#elif defined(KEYMAP_320HD)
static keypad_keys digitToKeyArr[8][8]
	= {	{KEYPAD_KEY_DIGIT_1,KEYPAD_KEY_DIGIT_7,	KEYPAD_KEY_LINE_1,	KEYPAD_KEY_INVALID,	KEYPAD_KEY_INVALID,	KEYPAD_KEY_INVALID, KEYPAD_KEY_INVALID, KEYPAD_KEY_INVALID},
		{KEYPAD_KEY_DIGIT_2,KEYPAD_KEY_DIGIT_8,	KEYPAD_KEY_LINE_2,	KEYPAD_KEY_INVALID, KEYPAD_KEY_TRANSFER,KEYPAD_KEY_INVALID, KEYPAD_KEY_INVALID, KEYPAD_KEY_INVALID},
		{KEYPAD_KEY_DIGIT_3,KEYPAD_KEY_DIGIT_9, KEYPAD_KEY_VOLUME_UP, KEYPAD_KEY_INVALID,KEYPAD_KEY_CONFERENCE,	KEYPAD_KEY_INVALID, KEYPAD_KEY_INVALID, KEYPAD_KEY_INVALID},
		{KEYPAD_KEY_DIGIT_4,KEYPAD_KEY_DIGIT_0,	KEYPAD_KEY_VOLUME_DOWN,	KEYPAD_KEY_INVALID,	KEYPAD_KEY_INVALID,	KEYPAD_KEY_INVALID, KEYPAD_KEY_INVALID, KEYPAD_KEY_INVALID},
		{KEYPAD_KEY_DIGIT_5,KEYPAD_KEY_ASTERIKS, KEYPAD_KEY_SPEAKER, KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,	KEYPAD_KEY_INVALID, 	KEYPAD_KEY_INVALID, KEYPAD_KEY_INVALID},
		{KEYPAD_KEY_DIGIT_6,KEYPAD_KEY_POUND,	KEYPAD_KEY_HEADSET,	KEYPAD_KEY_INVALID,	KEYPAD_KEY_INVALID,	KEYPAD_KEY_HOLD, KEYPAD_KEY_MUTE, KEYPAD_KEY_INVALID},
		{KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,	KEYPAD_KEY_INVALID,	KEYPAD_KEY_INVALID,	KEYPAD_KEY_INVALID,	KEYPAD_KEY_INVALID, KEYPAD_KEY_INVALID, KEYPAD_KEY_INVALID},
		{KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,	KEYPAD_KEY_INVALID,	KEYPAD_KEY_INVALID,	KEYPAD_KEY_INVALID,	KEYPAD_KEY_INVALID, KEYPAD_KEY_INVALID, KEYPAD_KEY_INVALID},

};
#elif defined(KEYMAP_405HD)||defined(KEYMAP_M495L)
static keypad_keys digitToKeyArr[8][8] =
{
 	{KEYPAD_KEY_LINE_2,KEYPAD_KEY_INVALID,KEYPAD_KEY_VOLUME_UP,KEYPAD_KEY_SPEAKER,KEYPAD_KEY_HOLD,KEYPAD_KEY_ON_HOOK,KEYPAD_KEY_OFF_HOOK,KEYPAD_KEY_INVALID},
	{KEYPAD_KEY_LINE_1,KEYPAD_KEY_INVALID,KEYPAD_KEY_ASTERIKS,KEYPAD_KEY_DIGIT_7,KEYPAD_KEY_DIGIT_4,KEYPAD_KEY_DIGIT_1,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID},
	{KEYPAD_KEY_CONFERENCE,KEYPAD_KEY_INVALID,KEYPAD_KEY_DIGIT_0,KEYPAD_KEY_DIGIT_8,KEYPAD_KEY_DIGIT_5,KEYPAD_KEY_DIGIT_2,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID},
 	{KEYPAD_KEY_TRANSFER,KEYPAD_KEY_INVALID,KEYPAD_KEY_POUND,KEYPAD_KEY_DIGIT_9,KEYPAD_KEY_DIGIT_6,KEYPAD_KEY_DIGIT_3,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID},
	{KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_VOLUME_DOWN,KEYPAD_KEY_MUTE,KEYPAD_KEY_HEADSET,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID},
	{KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID},
	{KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID},
	{KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID},
};
#elif defined(KEYMAP_420HD)||defined(KEYMAP_430HD)||defined(KEYMAP_440HD)||defined(KEYMAP_450HD)
static keypad_keys digitToKeyArr[8][8] =
{
 	{KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID},
	{KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID},
	{KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID},
 	{KEYPAD_KEY_INVALID,KEYPAD_KEY_DIGIT_1,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID},
	{KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID},
	{KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_DIGIT_8,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID},
	{KEYPAD_KEY_SPEAKER,KEYPAD_KEY_INVALID,KEYPAD_KEY_DIGIT_0,KEYPAD_KEY_POUND,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID},
	{KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID,KEYPAD_KEY_INVALID},
};

#else
#error "No keymap was defined"
#endif

/****************************************************************************
*
*	Name:		keypad_sig_handler
*----------------------------------------------------------------------------
*	Abstract:	keypad isr
*----------------------------------------------------------------------------
*	Input:
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 in case of error
******************************************************************************/
/*void keypad_sig_handler(int arg)
{
	KEY_EVENT 	key[MAX_EVENTS_TO_READ];
//	int digit;
	int 		events,current=0;

	int pressed;
	keypad_keys keypad_key;
	events = (read(keypadFd,(char *)&key,MAX_EVENTS_TO_READ*sizeof(KEY_EVENT)) / sizeof(KEY_EVENT)) ;

	while (current<events)
	{
		keypad_key=digitToKeyArr[ key[current].row][key[current].column ];
		pressed=key[current].key_press_time;

		printf("row=%d,col=%d,key=%d\n", key[current].row, key[current].column, keypad_key);

		if(keypad_report(keypad_key,pressed) < 0)
			printf("Error in keypad_report\n ");

		current++;
	}//end while

} /* end of keypad_sig_handler() */
/******************************************************************************/



/****************************************************************************
*
*	Name:		startKeypad
*----------------------------------------------------------------------------
*	Abstract:	connect the keypad irq to isr
*----------------------------------------------------------------------------
*	Input:      none
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 in case of error
******************************************************************************/
/*void startKeypad_signal(void)
{
	int 	oflags;

    /* register keypad signal handler */
	/*if(signal(SIGIO,&keypad_sig_handler) == SIG_ERR) {
		acl_log(ACL_LOG_ERROR,"keypad_sig_handler not registered\r\n");
		return;
	}

	/* open char driver */
	/*if((keypadFd = open("/dev/keypad0",O_RDWR))== -1) {
		acl_log(ACL_LOG_ERROR,"can't open keypad device\r\n");
		return;
	}


	/* code asynchronous notification */
	/*fcntl(keypadFd, F_SETOWN, getpid());
	oflags = fcntl(keypadFd, F_GETFL);
	fcntl(keypadFd, F_SETFL, oflags | FASYNC);
}*/

/******************************************************************************/

/****************************************************************************
*
*	Name:		closeKeypad
*----------------------------------------------------------------------------
*	Abstract:	close keypad file descriptor
*----------------------------------------------------------------------------
*	Input:      none
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 in case of error
******************************************************************************/
void closeKeypad(void)
{
	if (keypadFd<-1)
		printf ("ERROR : [%s,%d] keypad is not initialized \n ",__FUNCTION__,__LINE__);
	close( keypadFd);
	keypadFd=-1;
}



/****************************************************************************
*
*	Name:		OpenHookDevice
*----------------------------------------------------------------------------
*	Abstract:	open hook device
*----------------------------------------------------------------------------
*	Input:      none
*----------------------------------------------------------------------------
*  	Output:		int
*----------------------------------------------------------------------------
*	Returns: 	-1 in case of error, 0 in case of success
******************************************************************************/

int  OpenHookDevice(void)
{
	/* open char driver */
	if((hookFd = open("/dev/hkswitch0",O_RDWR))== -1) {
		acl_log(ACL_LOG_ERROR,"can't open hook device\r\n");
		return -1;
	}
	return 0;
}

/****************************************************************************
*
*	Name:		CloseHookDevice
*----------------------------------------------------------------------------
*	Abstract:	close hook device
*----------------------------------------------------------------------------
*	Input:      none
*----------------------------------------------------------------------------
*  	Output:		non

******************************************************************************/
int CloseHookDevice()
{
	if (hookFd<=-1)
		printf ("ERROR : [%s,%d] hook is not initialized \n ",__FUNCTION__,__LINE__);
	close( hookFd);
	hookFd=-1;
}
void *HookThreadFunction(void *arg)
{
	static int	keypad_hookState = KEYPAD_KEY_ON_HOOK;
	int current_hookstatus;

	startHookTerminate = 0;

	if(OpenHookDevice( ) == -1)
		return NULL;

  	while(1)
	{
		current_hookstatus = read(hookFd, NULL, 0);
		 if(current_hookstatus != keypad_hookState)
	 	 {
		    keypad_hookState =current_hookstatus;
		    if (current_hookstatus)
			{
				//printf("hook off\n");
		 		keypad_report(KEYPAD_KEY_OFF_HOOK ,1);
			}
			else
			{
				//printf("hook on\n");
				keypad_report(KEYPAD_KEY_ON_HOOK ,1);
			}
	  	}
		usleep(10000);
		/* If the application about to be terminated, exit thread gracefully */
		if(startHookTerminate){
			pthread_exit(NULL);
		}
	} /* while */

	return NULL;
} /* end of FxsThreadFunction() */


/****************************************************************************
*
*	Name:		startFxs
*----------------------------------------------------------------------------
*	Abstract:
*----------------------------------------------------------------------------
*	Input:		void *arg
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns:
******************************************************************************/
int startHook_thread(void *arg)
{
	pthread_t			tid;
	struct sched_param	sched;
	pthread_attr_t		attr;

	memset(&sched, 0, sizeof(sched));

	/* create the startFxsLegerity thread */
	if( (pthread_attr_init(&attr) != 0) )
	{
		acl_log(ACL_LOG_ERROR,"pthread_attr_init");
		exit(1);
	}
	if( (pthread_attr_setschedpolicy(&attr, SCHED_RR) != 0) )
	{
		acl_log(ACL_LOG_ERROR,"pthread_attr_setschedpolicy");
		exit(1);
	}
  	sched.sched_priority = 73;
 	if( (pthread_attr_setschedparam(&attr, &sched) != 0) )
	{
		acl_log(ACL_LOG_ERROR,"pthread_attr_setschedparam");
		exit(1);
	}

	if( (pthread_create( &tid, &attr, HookThreadFunction, (void *)arg) != 0) )
	{
		acl_log(ACL_LOG_ERROR,"pthread_create - startFxsLegerity");
		exit(1);
	}
	startHookTid = tid;


    	return 0;
}

/*void startKeypad(void)
{
	startKeypad_signal();
	startHook_thread(NULL);
} /* end of startKeypad() */


