/****************************************************************************
*                                                                            
*    	Company: 			Audiocodes Ltd.                                                      
*                                                                            
*    	Project: 			VoPP SoC                                                     
*                                                                            
*    	Hardware Module: 	AC4xx.                                                   
*                                                                            
*    	File Name:			iphone_fxsKatri.c 			                          
*                                                                            
*                                                                            
*****************************************************************************
*                                                                            
* 		DESCRIPTION: 		This file contains the API and threads routines fore the katri_fxs module
*							This code handlling two major tasks:
*							- Hook state recognition
*							- Ringing
*							The hook state recognition is done by an interrupt and an interrupt handler
*							The ringing is done by a toggle thread that is invoke by a signal
*							 		                                                             
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

//#include "iphone.h"
#include "acl_log.h"


#ifdef FXS_KATRI

#include <linux/delay.h>
#include <sched.h> 
unsigned long loops_per_jiffy;

#include "katri_fxsfxo_drv.h"
#include "iphone_fxsKatri.h"


int                 fxsfxoFd; /* fxsfxo file descriptor */
AC494_FXFSFXO_STCT		fxsfxoTable[AC494_FXSFXO_NUM_OF_MODULES];

/* the modules layout by channel id */
AC494_FXSFXO_MODULE_TYPE_ENT	fxsfxoModulesLayout[AC494_FXSFXO_NUM_OF_MODULES];

int	updateFxsRingFlag = 0;

/*#define  ACL_DEBUG*/

#ifdef ACL_DEBUG
#define ACL_LogDebug printf
#else
#define ACL_LogDebug
#endif


/****************************************************************************
*
*	Name:		startFxsKatri
*----------------------------------------------------------------------------
*	Abstract:	start a thread with forever loop to detect on/off hook (by interrupt)
*----------------------------------------------------------------------------
*	Input:		none
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	
******************************************************************************/
void *startFxsKatri(void)
{
    int oflags;
	pthread_t			tid;
	struct sched_param	sched;
	pthread_attr_t		attr;

    ACL_LogDebug("---------- %s %s line-%d ------- \r\n",__FILE__, __FUNCTION__, __LINE__);
	/* create the startFxsLegerity thread */
	if( (pthread_attr_init(&attr) != 0) ){
		acl_log(ACL_LOG_ERROR,"pthread_attr_init");
		exit(1);
	}
	if( (pthread_attr_setschedpolicy(&attr, SCHED_RR) != 0) ){
		acl_log(ACL_LOG_ERROR,"pthread_attr_setschedpolicy");
		exit(1);
	}	
  	sched.sched_priority = 73;	
 	if( (pthread_attr_setschedparam(&attr, &sched) != 0) ){
		acl_log(ACL_LOG_ERROR,"pthread_attr_setschedparam");
		exit(1);
	}
 	if( (pthread_create( &tid, &attr, (void *)fxsfxo_ringingThread, NULL) != 0) ){
		acl_log(ACL_LOG_ERROR,"pthread_create - fxsfxo_ringingThread");
		exit(1);
	}
    ACL_LogDebug("---------- %s %s line-%d ------- \r\n",__FILE__, __FUNCTION__, __LINE__);
    /*----------------------------------------------------*/
	/* connect to the hw interrupt (hook state) */

	/* register fxs signal handler */
	if(signal(SIGIO,&fxsfxo_sig_handler) == SIG_ERR) {
		printf("fxsfxo_sig_handler not registered\r\n");
		return;
	}
    ACL_LogDebug("---------- %s %s line-%d ------- \r\n",__FILE__, __FUNCTION__, __LINE__);
	/* open char driver */
    if((fxsfxoFd = open("/dev/fxsfxo/0",O_RDWR))== -1) {
		perror("can't open fxsfxo device\r\n");
		return;
	}
    ACL_LogDebug("---------- %s %s line-%d ------- \r\n",__FILE__, __FUNCTION__, __LINE__);
   	/* code asynchronous notification */
	fcntl(fxsfxoFd, F_SETOWN, getpid());
	oflags = fcntl(fxsfxoFd, F_GETFL);
	fcntl(fxsfxoFd, F_SETFL, oflags | FASYNC);
    /*----------------------------------------------------*/

} /* end of startFxsKatri() */
/******************************************************************************/



/****************************************************************************
*
*	Name:		fxsfxo_ringingThread
*----------------------------------------------------------------------------
*	Abstract:   loop forever loop, call the toggle routine to start a ringing, update the state by a signal
*----------------------------------------------------------------------------
*	Input:      none
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	none
******************************************************************************/

void fxsfxo_ringingThread(void)
{
	int	i;

	/* connect to a interrupt signal for a ringing change */
    ACL_LogDebug("---------- %s %s line-%d ------- \r\n",__FILE__, __FUNCTION__, __LINE__);

	for(;;) {
        
		while(updateFxsRingFlag) { /* check for a fxsfxo change */

			updateFxsRingFlag = 2;

			if(updateFxsRingFlag == 2) {
				updateFxsRingFlag = 0;
			}
		}
    

        /* check if there is a ring up state and call the toggle rotuine */
		for(i=0 ; i < AC494_FXSFXO_NUM_OF_MODULES ; i++) {
             if(fxsfxoTable[i].ringState)
				fxsfxoToggleRing(i);
		}
        usleep(10000);

	}

} /* end of fxsfxo_ringingThread() */
/******************************************************************************/



/****************************************************************************
*
*	Name:		fxsfxoToggleRing
*----------------------------------------------------------------------------
*	Abstract:	this is the toggle ring rouitne, its handle to timers that sets the ring up and down at a toggle mode
*----------------------------------------------------------------------------
*	Input:		none
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	success = 0
******************************************************************************/
int fxsfxoToggleRing(int line)
{
	static 	AC494_FXSFXO_TOGGLE_MODE_STCT	fxofxoToggleMode[AC494_FXSFXO_NUM_OF_MODULES];
	static	int	firstCall=1;
	int		i;

	if(line < 0 || line >= AC494_FXSFXO_NUM_OF_MODULES)
		return -1;

	if(firstCall) {


		for(i=0 ; i < AC494_FXSFXO_NUM_OF_MODULES ; i++) {
         	fxofxoToggleMode[i].onTimer =   	0;
        	fxofxoToggleMode[i].offTimer =  	1;
			fxofxoToggleMode[i].onUpTimer =		0;
			fxofxoToggleMode[i].onDownTimer =   1;
 		}

		firstCall = 0;
	}

	if(fxofxoToggleMode[line].onTimer) {
		fxofxoToggleMode[line].onTimer--;

		if(!fxofxoToggleMode[line].onTimer) {
			fxofxoToggleMode[line].offTimer = TOGGLE_OFF_T;
			fxsfxoCmd(line+1, 0); 
		}
	}
	if(fxofxoToggleMode[line].offTimer) {
		fxofxoToggleMode[line].offTimer--;
		if(!fxofxoToggleMode[line].offTimer) {
			fxofxoToggleMode[line].onTimer = TOGGLE_ON_T;
			fxofxoToggleMode[line].onUpTimer =		0;
			fxofxoToggleMode[line].onDownTimer =	1;
			fxsfxoCmd(line+1, 1); 
		}
	}		

	return 0;

} /* end of fxsfxoToggleRing() */
/******************************************************************************/



/****************************************************************************
*
*	Name:		fxsfxo_sig_handler
*----------------------------------------------------------------------------
*	Abstract:	handler for an fxsfxo signal (interrupt)
*----------------------------------------------------------------------------
*	Input:		none
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	success = 0
******************************************************************************/
void fxsfxo_sig_handler(int arg)
{

	AC494_FXSFXO_GET_STATUS_STCT 				fxsfxoStatus;

	int line;
	int mode;
	static int lineStatusOld = 0xFFFFFFFF;
	int	exlusiveOrLineStatus;
	char *phyName;
	int	analogId;

    ACL_LogDebug("---------- %s %s line-%d ------- \r\n",__FILE__, __FUNCTION__, __LINE__);
	fxsfxoStatus.command = FXSFXO_GET_INFO_CMD_E;

	/* read the fxsfxo status */
	if(read(fxsfxoFd,(char *)&fxsfxoStatus,sizeof(AC494_FXSFXO_GET_STATUS_STCT)) != AC494_FXSFXO_OK_E) {
		perror("error at read FXSFXO_GET_INFO_CMD_E\r\n");
		return;
	}


    /* compare the new status with the old one */
	exlusiveOrLineStatus = fxsfxoStatus.fxsfxoInfo.lineStatus ^ lineStatusOld;

	/* check if there was a change */

	if(exlusiveOrLineStatus & LINE_1_STATUS_MASK) {
    	if((analogId = getLineIdByPcmLineId(0))==-1) {
			printf("can't find id ANALOG_0_LINE_NAME\r\n");
		} else {
			mode = fxsfxoStatus.fxsfxoInfo.lineStatus & LINE_1_STATUS_MASK;

			if((phyName = getPhyNameByPcmLineId(0)) != NULL) {

				if(fxsfxoModulesLayout[0]==AC494_FXS_MODULE_E) {
					if(mode) {
						/* fxsfxoCmd(1, 0); */
						printf("on hook analogId %d\r\n",analogId);
						hw_report_event(phyName,"hu");
					}
					else {
						fxsfxoCmd(1, 0);
						printf("off hook analogId %d\r\n",analogId);
						hw_report_event(phyName,"hd");
					}

				}
				else if(fxsfxoModulesLayout[0]==AC494_FXO_MODULE_E) {
       				if(mode)
						printf("ring off line %d\r\n",line);
					else
						printf("ring on line %d\r\n",line);
				}
			}
		}
   	}





	if(exlusiveOrLineStatus & LINE_2_STATUS_MASK) {
    	if((analogId = getLineIdByPcmLineId(1))==-1) {
			printf("can't find id ANALOG_1_LINE_NAME\r\n");
		} else {
			mode = fxsfxoStatus.fxsfxoInfo.lineStatus & LINE_2_STATUS_MASK;

			if((phyName = getPhyNameByPcmLineId(1)) != NULL) {

				if(fxsfxoModulesLayout[1]==AC494_FXS_MODULE_E) {
					if(mode) {
						/* fxsfxoCmd(2, 0); */
						printf("on hook analogId %d\r\n",analogId);
						hw_report_event(phyName,"hu");
					}
					else {
						fxsfxoCmd(2, 0);
						printf("off hook analogId %d\r\n",analogId);
						hw_report_event(phyName,"hd");
					}

				}
				else if(fxsfxoModulesLayout[1]==AC494_FXO_MODULE_E) {
       				if(mode)
						printf("ring off line %d\r\n",line);
					else
						printf("ring on line %d\r\n",line);
				}
			}
		}
   	}




	if(exlusiveOrLineStatus & LINE_3_STATUS_MASK) {
    	if((analogId = getLineIdByPcmLineId(2))==-1) {
			printf("can't find id ANALOG_2_LINE_NAME\r\n");
		} else {
			mode = fxsfxoStatus.fxsfxoInfo.lineStatus & LINE_3_STATUS_MASK;

			if((phyName = getPhyNameByPcmLineId(2)) != NULL) {

				if(fxsfxoModulesLayout[2]==AC494_FXS_MODULE_E) {
					if(mode) {
						/* fxsfxoCmd(3, 0);  */
						printf("on hook analogId %d\r\n",analogId);
						hw_report_event(phyName,"hu");
					}
					else {
						fxsfxoCmd(3, 0);
						printf("off hook analogId %d\r\n",analogId);
						hw_report_event(phyName,"hd");
					}

				}
				else if(fxsfxoModulesLayout[2]==AC494_FXO_MODULE_E) {
       				if(mode)
						printf("ring off line %d\r\n",line);
					else
						printf("ring on line %d\r\n",line);
				}
			}
		}
   	}



	if(exlusiveOrLineStatus & LINE_4_STATUS_MASK) {
    	if((analogId = getLineIdByPcmLineId(3))==-1) {
			printf("can't find id ANALOG_3_LINE_NAME\r\n");
		} else {
			mode = fxsfxoStatus.fxsfxoInfo.lineStatus & LINE_4_STATUS_MASK;

			if((phyName = getPhyNameByPcmLineId(3)) != NULL) {

				if(fxsfxoModulesLayout[3]==AC494_FXS_MODULE_E) {
					if(mode) {
						/* fxsfxoCmd(4, 0); */
						printf("on hook analogId %d\r\n",analogId);
						hw_report_event(phyName,"hu");
					}
					else {
						fxsfxoCmd(4, 0);
						printf("off hook analogId %d\r\n",analogId);
						hw_report_event(phyName,"hd");
					}

				}
				else if(fxsfxoModulesLayout[3]==AC494_FXO_MODULE_E) {
       				if(mode)
						printf("ring off line %d\r\n",line);
					else
						printf("ring on line %d\r\n",line);
				}
			}
		}
   	}



   lineStatusOld = fxsfxoStatus.fxsfxoInfo.lineStatus;


} /* end of fxsfxo_sig_handler() */
/******************************************************************************/




/****************************************************************************
*
*	Name:		fxsfxoCmd
*----------------------------------------------------------------------------
*	Abstract:	set a cmd to the analog telephone
*----------------------------------------------------------------------------
*	Input:		line - telephone index to ring
*				mode - 0 = off, 1 = on
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	success = 0
******************************************************************************/
void fxsfxoCmd(int line, int mode)
{

	int cmd;
	int arg;

	int	fxsfxoFd1;

	/* open char driver */
   	if((fxsfxoFd1 = open("/dev/fxsfxo/0",O_RDWR))== -1) {
		perror("can't open fxsfxo device\r\n");
		return;
	}

	switch(line) {

		case 1:
			cmd = FXSFXO_LINE_1_CMD_E;
			break;

		case 2:
			cmd = FXSFXO_LINE_2_CMD_E;
			break;

		case 3:
			cmd = FXSFXO_LINE_3_CMD_E;
			break;

		case 4:
			cmd = FXSFXO_LINE_4_CMD_E;
			break;

		default:
			break;

	}

	if(mode)
		arg = FXSFXO_ON_CMD_E;
	else
		arg = FXSFXO_OFF_CMD_E;


	printf("debug - fxsfxoCmd cmd = %d arg = %d",cmd,arg);

	ioctl(fxsfxoFd1, cmd, arg);   	/* send the command to the fxsfxo driver */

	mdelay(1);

	close(fxsfxoFd1);

} /* end of fxsfxoCmd() */
/******************************************************************************/




#endif /* FXS_KATRI */





