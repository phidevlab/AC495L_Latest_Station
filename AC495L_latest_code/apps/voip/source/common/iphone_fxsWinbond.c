/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	iphone_fxsWinbond.c                                 *
 *                                                                  *
 * Copyright (C) 2007 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 15/04/07 - Coding started.                                       *
 *																	*
 ********************************************************************/
/******************************************************************************
*                                                                            
* 		DESCRIPTION: 		 		                                                             
*                                                                            
******************************************************************************/

#include <sys/stat.h>  
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <sys/ipc.h>
#include <sys/msg.h>

#include <signal.h>

#include <sys/types.h>
#include <sys/stat.h>  
#include <semaphore.h>
    
#include "acl_log.h"

#ifdef FXS_WINBOND

#include "iphone_fxs.h"
#include <linux/delay.h>
#include <sched.h> 

/*#define  ACL_DEBUG*/

#ifdef ACL_DEBUG
#define ACL_LogDebug printf
#else
#define ACL_LogDebug
#endif

#define W682388_DRV_DEVICE_NAME_STR	"w682388drv"
#define W682388_DRV_FD_NAME_STR		"/dev/w682388drv"

int 		w682388Fd;   /* w682388Fd file descriptor */
int		hookState[NUM_OF_PHY_TERMS];
static 	LINE_LOCK_UNLOCK_STATE myLineLockUnlockState[NUM_OF_PHY_TERMS];

int 	startFxsWinbondTerminate;

/****************************************************************************
*
*	Name:		startFxsWinbond
*----------------------------------------------------------------------------
*	Abstract:	
*----------------------------------------------------------------------------
*	Input:		none
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	success = 0
******************************************************************************/
void *startFxsWinbond(void *arg)
{
	int	isFirstLoading= (int)arg;

	int prevHookState[NUM_OF_PHY_TERMS];
	int 	channel;
	int	isFlash = 0;	

	startFxsWinbondTerminate = 0;
		
	if((w682388Fd = open(W682388_DRV_FD_NAME_STR, O_RDWR)) == -1) {
			perror("open /dev/w682388drv\r\n");
			return;
	}

	if(isFirstLoading)
	{
		if(w682388_init()==-1)
			return;
	}

		if(w682388_initLine(0) == -1)
			return;

		if(w682388_initLine(1) == -1)
			return;

	/* Set lines states to UNLOCK_LINE */
	for(channel=0; channel<NUM_OF_PHY_TERMS; channel++)
	{
		myLineLockUnlockState[channel] = UNLOCK_LINE;
	}
	
	/* Set initial hook states to off hook */
	for(channel=0 ; channel < NUM_OF_PHY_TERMS ; channel++) {
		prevHookState[channel] = 0;
	}

      	while(1) {   /* forever */

		for(channel=0 ; channel < NUM_OF_PHY_TERMS ; channel++) {

			hookState[channel] = w682388_get_hook_state(w682388Fd, channel, &isFlash);

			/* If line is locked, ignore it */
			if(myLineLockUnlockState[channel] == LOCK_LINE){
				continue;
			}

			if(isFlash){
				if(fxsFlash_api(channel) == -1) {
					acl_log(ACL_LOG_DEBUG,"fxsHookState_api Failed"); 
				}	
			}
			else{
				if(prevHookState[channel] != hookState[channel])     /* Change in hook state */
				{
					prevHookState[channel]  = hookState[channel];

					if(hookState[channel] == 0) {
						/*** line is hook on ***/
						ACL_LogDebug("---------- %s %s line-%d line is hook on  ------- \r\n",__FILE__, __FUNCTION__, __LINE__);
						if(w682388_lineState(channel, 0/*STANDBY*/) == -1) {
							acl_log(ACL_LOG_DEBUG,"w682388_lineState Failed"); 
						}
						if(fxsHookState_api(channel,0) == -1) {
							acl_log(ACL_LOG_DEBUG,"fxsHookState_api Failed"); 
						}					
					}
					else{
						ACL_LogDebug("---------- %s %s line-%d line is hook off  ------- \r\n",__FILE__, __FUNCTION__, __LINE__);
						/*** line is hook off ***/
						if(w682388_lineState(channel, 2/*ACTIVE*/) == -1) {
							acl_log(ACL_LOG_DEBUG,"w682388_lineState Failed"); 
						}
						if(fxsHookState_api(channel,1) == -1) {
							acl_log(ACL_LOG_DEBUG,"fxsHookState_api Failed"); 
	}
					}

				}

			}

		}	
		/* If the application about to be terminated, exit thread gracefully */
		if(startFxsWinbondTerminate){
			pthread_exit(NULL);
		}
	} /* for() */

} /* end of startFxsWinbond() */
/******************************************************************************/


 
/*******************************************************************************
 * Function:    w682388_init
 * Description: init w682388 device
 * Argument:    void
 * Return:      OK/ERROR
 ******************************************************************************/
int w682388_init(void)
{
	int status;
	W682388_INIT_STCT initDevice;

 	initDevice.command = W682388_INIT_CMD;

	if((status = (write(w682388Fd, (char *)&initDevice, sizeof(W682388_INIT_STCT)))) != W682388_OK_E) {
		perror("error at write W682388_INIT_CMD \r\n");
		return -1;
	}

	return 0;

} /* end of w682388_init() */
/******************************************************************************/



/*******************************************************************************
 * Function:    w682388_initLine
 * Description: init w682388 line for a channel
 * Argument:    channel - channel to be init
 * Return:      OK/ERROR
 ******************************************************************************/
int w682388_initLine(int channel)
{
	int status;
	W682388_INIT_LINE_STCT	initLine;

 	initLine.command = W682388_INIT_LINE_CMD;
	initLine.channel = channel;

	if((status = (write(w682388Fd, (char *)&initLine, sizeof(W682388_INIT_LINE_STCT)))) != W682388_OK_E) {
		perror("error at write W682388_INIT_LINE_CMD \r\n");
		return -1;
	}

	return 0;

} /* end of w682388_initLine() */
/******************************************************************************/


/*******************************************************************************
 * Function:    w682388_get_hook_state
 * Description: 
 * Argument:    
 * Return:      0 = hook off, 1 = hook on, -1 = error
 ******************************************************************************/
int w682388_get_hook_state(int	w682388Fd, int channel, int* isFlash)
{
	W682388_HOOK_STATE_STCT	hookState;

 	hookState.command = W682388_GET_HOOK_STATE_CMD;
	hookState.channel = channel;

	if((read(w682388Fd, (char *)&hookState, sizeof(W682388_HOOK_STATE_STCT))) != W682388_OK_E) {
		perror("error at reading W682388 hook state \r\n");
		return -1;
	}
	*isFlash = hookState.flashCheck;

	return (hookState.state);

} /* end of w682388_get_hook_state() */
/******************************************************************************/


/*******************************************************************************
 * Function:    w682388_lineState
 * Description:
 * Argument:    channel - channel to be init ,
				state - STANDBY/TIP_OPEN/ACTIVE/OHT/DISCONNECT/RINGING/OHT_PR/ACTIVE_PR/MODEM_OPT/LOOP_OPEN/LOOP_CLOSE/RING_GND
 * Return:      OK/ERROR
 ******************************************************************************/
int w682388_lineState(int channel, int state)
{

	int status;
	W682388_LINE_STATE_STCT	lineState;

/*	printf("w682388_lineState: channel=%d state=%d\r\n",channel,state);*/


 	lineState.command = W682388_SET_LINE_STATE_CMD;
	lineState.channel = channel;
	lineState.state = state;

	if((status = (write(w682388Fd, (char *)&lineState, sizeof(W682388_LINE_STATE_STCT)))) != W682388_OK_E) {
		perror("error at writing W682388_SET_LINE_STATE_CMD \r\n");
		return -1;
	}

	return 0;

} /* end of w682388_lineState() */

/*******************************************************************************
 * Function:   	w682388_lockUnlockChannel
 * Description:	set line state (lock/unlock)
 * Argument:  	channel - channel to be init
 *				state - LOCK_LINE, UNLOCK_LINE
 * Return:      
 ******************************************************************************/
void w682388_lockUnlockLine(int channel, LINE_LOCK_UNLOCK_STATE state)
{
	myLineLockUnlockState[channel] = state;
}


/****************************************************************************
*
*	Name:		setIphoneW6823Parameters
*----------------------------------------------------------------------------
*	Abstract:	setting w682388 parameters
*----------------------------------------------------------------------------
*	Input:
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	success = 0
******************************************************************************/

void setIphoneW6823Parameters(IPHONE_PARAMETERS *iphoneParameters)
{
//	w682388PollingInterval = iphoneParameters->w682388PollingDelay;
}

/*******************************************************************************
 * Function:    w682388_get_reg
 * Description: 
 * Argument:    
 * Return:      
 ******************************************************************************/
int w682388_get_reg(int address)
{
	W682388_GET_REG_STCT	getReg;

    printf("w682388_get_reg: address=%d \r\n",address);

  	getReg.command = W682388_GET_REG_CMD;
	getReg.address = address;

	if((read(w682388Fd, (char *)&getReg, sizeof(W682388_GET_REG_STCT))) != W682388_OK_E) {
		perror("error at read W682388_GET_REG_CMD \r\n");
		return -1;
	}

 	return (getReg.data);

} /* end of w682388_get_reg() */
/******************************************************************************/

/*******************************************************************************
 * Function:    w682388_set_reg
 * Description: 
 * Argument:    
 * Return:      
 ******************************************************************************/
int w682388_set_reg(int address, int data)
{
    int status;
	W682388_SET_REG_STCT	setReg;

    printf("w682388_set_reg: address=%d, data=%d \r\n",address,data);


 	setReg.command = W682388_SET_REG_CMD;
	setReg.address = address;
    setReg.data    = data;

	if((status = (write(w682388Fd, (char *)&setReg, sizeof(W682388_SET_REG_STCT)))) != W682388_OK_E) {
		perror("error at write W682388_SET_REG_CMD \r\n");
		return -1;
	}

	return 0;

} /* end of w682388_set_reg() */
/******************************************************************************/

/*******************************************************************************
 * Function:    w682388_set_country
 * Description: 
 * Argument:    
 * Return:      
 ******************************************************************************/
int w682388_set_country(int country)
{
	int status;
    W682388_SET_COUNTRY_STCT	setCountry;

    printf("w682388_set_country: country=%d\r\n",country);

 	setCountry.command = W682388_SET_COUNTRY_CMD;
	setCountry.country = country;

	if((status = (write(w682388Fd, (char *)&setCountry, sizeof(W682388_SET_COUNTRY_STCT)))) != W682388_OK_E) {
		perror("error at write W682388_SET_COUNTRY_CMD \r\n");
		return -1;
	}

	return 0;

} /* end of w682388_set_country() */
/******************************************************************************/


#endif /* FXS_WINBOND */




