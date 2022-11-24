/****************************************************************************
*                                                                            
*	Company:			Audiocodes Ltd.                                                      
*                                                                            
*   Project:			AC48 CPE VoIP Toolkit                                                    
*                                                                          
*   Hardware Module: 	AC48 CPE VoIP Toolkit Evaluation Board                                                  
*                                                                           
*   File Name: 			le88_drv.c                                                  
*                                                                            
*                                                                            
******************************************************************************
*                                                                            
* 	DESCRIPTION:		This file wrap the le88 driver to a char device		  		                                                             
*                                                                            
******************************************************************************/


#include <linux/config.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/ioport.h>
#include <linux/fcntl.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/proc_fs.h>
#include <asm/io.h>
#include <asm/uaccess.h>
//#include <asm/system.h>
#include <linux/init.h>
#include <linux/devfs_fs_kernel.h>
#include <linux/timer.h>
#include <linux/param.h>
//#if defined(S_PLATFORM)
/*linux/moduleparam.h is a 2.6 file - currently does not exist in S_PLATFORM*/
//#else
#include <linux/moduleparam.h>
//#endif

//#define CONFIG_NUM_SLIC_DEVICES 1 /*ACL A.L temp for Tulip*/
/*#define CONFIG_NUM_SLIC_DEVICES 4*/ /*ACL A.L temp for SMB*/

#include "vp_api_option.h"
#include "vp_api.h"
#include "regdump.h"

#include "le88_drv.h"

#include "profiles8.h"


//#include <linux/tqueue.h>
//#include <asm/smplock.h>
//#include <pal.h>
#include <linux/delay.h>
#include "../ac49x_mpi/mpi_drv.h"

#define LE88_DEV_NAME		"le88drv"


MODULE_DESCRIPTION("Le88 kernel module");
MODULE_AUTHOR("Legerity");
MODULE_LICENSE("GPL");

/* Need to be compatible with the timer interval of the timerFunction */ 
#define LE88_POLLING_INTERVAL 10000   


/* global variables */
static VpDevCtxType DevCtx[NUM_DEVICES];
static Vp880DeviceObjectType DevObj[NUM_DEVICES];
static VpLineCtxType pLineCtx[MAX_LINES];
static Vp880LineObjectType pLineObj[MAX_LINES];
static int gsLE88InitComplete[MAX_DEVICES]={0,0,0,0};
static VpLineStateType FxsSetStates[MAX_LINES];

/* since hook state is tested on timer, we do not test it while the mpi is locked */
int is_hook_state_testable = 1;


static telephonyEventBuffer_t  TelephonyEventBuffer[MAX_DEVICES][NUM_LINES];

static int flashTimer;
static int hook_state_condition;
static wait_queue_head_t hook_state_wait;
static spinlock_t hook_state_lock;
static enum_ring_cadence_profile_t CUR_RING_CADENCE = SLIC_RING_CADENCE_PROFILE_1;


typedef struct semaphore sem_t;
static struct timer_list 				le88Timer;


/* timer callback function */
static void timerFunction( unsigned long );

uint8 InitCriticalSectionDB(void);

/****************************************************************************
*                                                                            
*	Name:		set_slic_state	 			                                                     
*----------------------------------------------------------------------------                                                                            
*	Abstract:	set slic state
*----------------------------------------------------------------------------                                                                            
*	Input:		Channel 	- slic channel to be set
*			State		- state of the channel
*----------------------------------------------------------------------------                                                                            
*  	Output:		none			                                                  
*----------------------------------------------------------------------------                                                                            
*	Returns: 	case of error = -1                              
******************************************************************************/
int set_slic_state(int Channel, VpLineStateType State)
{
    int ret = 0;
	
	VpSetLineState(&pLineCtx[Channel], State);
#if 0
    switch (State)
	{    	    
	case mtRINGING:
		//printk("<1>mtRINGING: Start ringing on channel %d\n", Channel);
		VpSetLineState(&pLineCtx[Channel], VP_LINE_RINGING);
		break;
	case mtACTIVE:
//					printk("<1>Setting the channel to active mode  - active -talk (channel %d)\n", Channel);
		VpSetLineState(&pLineCtx[Channel], VP_LINE_TALK/*VP_LINE_ACTIVE*/);
		break;
	case mtSTANDBY:
//					printk("<1>Setting the channel to standby mode (channel %d)\n", Channel);
		VpSetLineState(&pLineCtx[Channel], VP_LINE_STANDBY);
		break;
	default:
		ret = (-1);
		break;

    }	/*switch (State)*/
#endif
    return ret;
}


/*--------------------------------------------------------------------
 * ROUTINE:  InitSettings
 *
 * DESCRIPTION:
 *    Function used to initialize the device and lines of the device
 *    to settings not provided by default in the API.  This function 
 *    should be the first function called after a device init complete
 *    event occurs.
 *
 * ARGUMENTS:
 *    VpEventType *pEvent - captured event pointer
 *
 * RESULTS:
 *   return void
 *--------------------------------------------------------------------*/
static void InitSettings(VpEventType *pEvent)
{
	VpDevCtxType *pDevCtx = pEvent->pDevCtx;
	VpDeviceInfoType deviceInfo;
	uint16 handle = 0;
	int i;
	/* 
	 * Check the deviceId of the device that just completed initialization to 
	 * determine if it was the FXO device or FXS device
	 */
	deviceInfo.pDevCtx = pDevCtx;
	deviceInfo.pLineCtx = VP_NULL;
	VpGetDeviceInfo(&deviceInfo);

	/* 
	 * Setup the non default settings required to run this application on the
	 * FXS line.
	 */
/*	
	VpInitRing(&pLineCtx[2*deviceInfo.deviceId], RING_CAD_STD, VP_PTABLE_NULL);
	VpInitRing(&pLineCtx[2*deviceInfo.deviceId + 1], RING_CAD_STD, VP_PTABLE_NULL);
	
	VpSetLineState(&pLineCtx[2*deviceInfo.deviceId], VP_LINE_STANDBY);
	VpSetLineState(&pLineCtx[2*deviceInfo.deviceId + 1], VP_LINE_STANDBY);
*/
	for (i=0; i<NUM_LINES;i++){
//	timeslot.rx =  	2*deviceInfo.deviceId+i;
//	timeslot.tx =	2*deviceInfo.deviceId+i;
//	VpSetOption(&pLineCtx[2*deviceInfo.deviceId+i], VP_NULL, VP_OPTION_ID_TIMESLOT, &timeslot);
		VpInitRing(&pLineCtx[2*deviceInfo.deviceId+i], RING_CAD_STD, VP_PTABLE_NULL);
		VpSetLineState(&pLineCtx[2*deviceInfo.deviceId+i], VP_LINE_TALK);
	}
	/* 
	 * Read the event mask contents so we can unmask only the FXS signaling
	 * events and the FXO events. The device that the event returns for will
	 * indicate which unmasking that needs to occur.
	 */
	VpGetOption(VP_NULL, pDevCtx, VP_OPTION_ID_EVENT_MASK, handle);

	printk("<1>LE88[%x] driver: InitSettings: completed successfully !!! \n", deviceInfo.deviceId);
	return;
}


/*--------------------------------------------------------------------
 * ROUTINE:  InitReadOptions
 *
 * DESCRIPTION:
 *    Function used to initialize the device event masks to only
 *    allow those used by this application.
 *
 * ARGUMENTS:
 *    VpEventType *pEvent - captured event pointer
 *
 * RESULTS:
 *   return void
 *--------------------------------------------------------------------*/
static void InitReadOptions(VpEventType *pEvent)
{
	VpDevCtxType *pDevCtx = pEvent->pDevCtx;
	VpOptionEventMaskType eventMask;
	VpStatusType status;
	VpOptionTimeslotType timeslot;
	int i;

	VpGetResults(pEvent, &eventMask);

	eventMask.signaling &= ~(VP_LINE_EVID_HOOK_OFF | VP_LINE_EVID_HOOK_ON);
	eventMask.signaling &= ~(VP_LINE_EVID_FLASH);
	eventMask.process &= ~(VP_LINE_EVID_SIGNAL_CMP);
	status = VpSetOption(VP_NULL, pDevCtx, VP_OPTION_ID_EVENT_MASK, &eventMask);

	for (i=0; i<NUM_LINES;i++){
	timeslot.rx =  	2*pEvent->deviceId+i;
	timeslot.tx =	2*pEvent->deviceId+i;
	status += VpSetOption(&pLineCtx[2*pEvent->deviceId+i], VP_NULL, VP_OPTION_ID_TIMESLOT, &timeslot);
//	printk("Line[%x] set rx[%x], tx[%x]\n", 2*pEvent->deviceId+i, timeslot.tx, timeslot.rx);
	}

	if (status != VP_STATUS_SUCCESS)
			printk("<1>LE88 driver (InitReadOptions): VpSetOption failed status=%d\n", status);

	printk("<1>LE88[%x] driver: InitReadOptions: completed successfully !!! \n\n", pEvent->deviceId);
}

/*--------------------------------------------------------------------
 * ROUTINE:  ProcessFXSEvent
 *
 * DESCRIPTION:
 *    Function used to perform some action on the FXS lines when an
 *    event occurs on the FXS line.
 *
 * ARGUMENTS:
 *    VpEventType *pEvent - captured event pointer
 *
 * RESULTS:
 *   return void
 *--------------------------------------------------------------------*/
static void ProcessFXSEvent(VpEventType *pEvent)
{
	VpEventCategoryType eventCategory = pEvent->eventCategory;
	uint16 eventId = pEvent->eventId;
	uint8 chanId = pEvent->channelId;
	int deviceId = pEvent->deviceId;
	int LED_State,temp,i,j;
/*	DBG_PRINT("chanId=%d,eventCategory:%d,eventId:0x%X\n", chanId, eventCategory, eventId);*/

	/* 
	 * Ignore all other type of events except signaling on the FXS device.  For
	 * those, we will look at the data and determine the digit (or hook, or 
	 * flash) that caused the event.  Depending on the specific event, we will 
	 * send a tone or send the digit on the FXO line.  If we send the digit on
	 * the FXO line, we have a choice between DTMF or Pulse. 
	 *
	 * Note:  We specified the Pulse digit generated from the FXO line via the 
	 * profiles, not via Options.
	 */    

	switch(eventCategory) {

		case VP_EVCAT_SIGNALING:
			switch (eventId) {
				case VP_LINE_EVID_HOOK_OFF:
					//VpSetLineState(&pLineCtx[chanId+deviceId*2], VP_LINE_TALK);
					/* AlexChen 20090211 */
					spin_lock(&hook_state_lock);
					FxsSetStates[chanId+deviceId*2] = VP_LINE_TALK;
					TelephonyEventBuffer[deviceId][chanId].fxsOnHook=FALSE;
					spin_unlock(&hook_state_lock);

					break;					
				case VP_LINE_EVID_HOOK_ON:
					/* AlexChen 20090211 */
					//VpSetLineState(&pLineCtx[chanId+deviceId*2], VP_LINE_STANDBY);
					spin_lock(&hook_state_lock);
					FxsSetStates[chanId+deviceId*2] = VP_LINE_STANDBY;
					TelephonyEventBuffer[deviceId][chanId].fxsOnHook = TRUE;
					spin_unlock(&hook_state_lock);

					break;
				default:
					break;
			}
#if 0
		case VP_EVCAT_FXO:
			switch (eventId) {
				case VP_LINE_EVID_POLREV:
					printk("<1>\nChannel %d polarity reversal has occurred, device[%x]\n", chanId + deviceId*2, deviceId);
					spin_lock(&hook_state_lock);
					TelephonyEventBuffer[deviceId][chanId].polarityReversal = TRUE;
					spin_unlock(&hook_state_lock);
					hook_state_condition += 1;
					wake_up_interruptible(&hook_state_wait);

					break;					
				case VP_LINE_EVID_RING_ON:
					printk("<1>\nChannel %d Ringing detected, deviceId[%x]\n", chanId + deviceId*2, deviceId);
					spin_lock(&hook_state_lock);
					TelephonyEventBuffer[deviceId][chanId].fxoRingStart = TRUE;
					spin_unlock(&hook_state_lock);

					hook_state_condition += 1;
					wake_up_interruptible(&hook_state_wait);
					
					break;
				case VP_LINE_EVID_RING_OFF:
					printk("<1>\nChannel %d Ringing end, deviceId[%x]\n", chanId + deviceId*2, deviceId);
					spin_lock(&hook_state_lock);
					TelephonyEventBuffer[deviceId][chanId].fxoRingEnd = TRUE;
					spin_unlock(&hook_state_lock);

					hook_state_condition += 1;
					wake_up_interruptible(&hook_state_wait);
					
					break;
				default:
					break;
			}
#endif		
			//for led on/off
			LED_State = 0xFFFFFFFF;
		    temp = 0;
		    for( i = 3; i >= 0; i--)
        		for( j = 1; j >= 0; j--){
//		            if(fxsOnHook[i][j] != 0)
//        		        fxsOnHook[i][j] = 1;
		            temp = temp + (1 - TelephonyEventBuffer[i][j].fxsOnHook);
        		    temp = temp << 1;
		        }
			temp = temp << 7;
			LED_State &= ~temp;
			LED_State |= 0xFFFF00FF;
#if !defined(CONFIG_ORCHID) &&  !defined(CONFIG_ORCHID_ATA) 
			*(volatile unsigned int*)(KSEG1ADDR(0x08610908)) |= 0x0000FF00;
			mb();
		    *(volatile unsigned int*)(KSEG1ADDR(0x08610908)) &= LED_State;
			mb();
#endif
			break;
		default:
			break;
	}
}

/*--------------------------------------------------------------------
 * ROUTINE:  eventHandle
 *
 * DESCRIPTION:
 *    This function is called at a periodic rate to perform all 
 *    required API operations.  It implements the functional 
 *    requirements of the application mentioned in the header.
 *
 * ARGUMENTS:
 *    void
 *
 * RESULTS:
 *   return void
 *--------------------------------------------------------------------*/
static void eventHandle(void)
{
    bool deviceEventStatus[NUM_DEVICES];
    VpEventType pEvent[NUM_DEVICES];
    VpStatusType status[NUM_DEVICES];
	int  deviceId;
    /* 
     * when an event is found (deviceEventStatus = TRUE), it will parse 
	 * the event and perform further operations.
     */

	for (deviceId=0; deviceId<NUM_DEVICES; deviceId++){

		if(!gsLE88InitComplete[deviceId]) continue;
			
		deviceEventStatus[deviceId] = FALSE;
		status[deviceId] = VpApiTick(&DevCtx[deviceId], &deviceEventStatus[deviceId]);

		if (status[deviceId] != VP_STATUS_SUCCESS) 
	{
		static int iDebugCounter=0;
		if (iDebugCounter++<20)//make sure debug prints do not overload the system....
		{
				printk("<1>LE88 driver: VpApiTick failed, status=%d\n", status[deviceId]);
		}
		else
		{
			iDebugCounter=21;
		}
	}

		if (deviceEventStatus[deviceId] != TRUE) {
//			return;
			goto ENDLOOP;
	}
		while(VpGetEvent(&DevCtx[deviceId], &pEvent[deviceId])) {
		/* 
		 * If the event was a Response:Device Init Complete event,
		 * we need to initialize the device and associated lines, so
		 * send the event structure (pointer) to the Init function for
		 * handling.  Otherwise, it's an event we can process.
		 */
			if (pEvent[deviceId].eventCategory == VP_EVCAT_RESPONSE) {
				
				if(pEvent[deviceId].eventId == VP_DEV_EVID_DEV_INIT_CMP) {
					InitSettings(&pEvent[deviceId]);
				} else if (pEvent[deviceId].eventId == VP_LINE_EVID_RD_OPTION) {
					InitReadOptions(&pEvent[deviceId]);
			} else {
				/* Do nothing */
			}
			/* We're initializing if a response event occurred. */
				goto ENDLOOP;
//				return;
			}
			ProcessFXSEvent(&pEvent[deviceId]);
		}
ENDLOOP:
	;
	}
    return;
}



/****************************************************************************
*                                                                            
*	Name:		le88Dev_read	 			                                                     
*----------------------------------------------------------------------------                                                                            
*	Abstract:	read routine from the device
*----------------------------------------------------------------------------                                                                            
*	Input:		file 	- file decriptor
*				buf		- buf to fill
*				count	- size of buf
*				ppos	- flags
*----------------------------------------------------------------------------                                                                            
*  	Output:		buf	- read data			                                                  
*----------------------------------------------------------------------------                                                                            
*	Returns: 	success = LE88_OK_E                              
******************************************************************************/
static ssize_t le88Dev_read( struct file * file, char * buf, size_t count, loff_t *ppos )
{
	int ret;
	int channel;
	int deviceId=0;
	int isEventFound = 0;
	
	acgTEventInfo 			TelephonyEvent;
	acgTCommand			*pAcgCmd=NULL;
	acgEPollingMethod 		PollingMethod;

	acgTGetEventInfo		*pGetEventInfo=NULL;

	/* cast the pointer */
	pAcgCmd = (acgTCommand *)buf;

	switch(pAcgCmd->Command) {


		case ACG_SLIC_GET_TELEPHONY_EVENT_CMD:

			pGetEventInfo = (acgTGetEventInfo *)pAcgCmd->data;
			PollingMethod = pGetEventInfo->PollingMethod;
			
			TelephonyEvent.Channel = -1;
			TelephonyEvent.EventType = ACG_EVENT_TYPE__NO_EVENT;

			if(PollingMethod == ACG_POLLING_METHOD__BLOCKING)
			{	
				wait_event_interruptible(hook_state_wait,  hook_state_condition > 0);
				
				hook_state_condition --;
			}

			for(deviceId = 0; (deviceId < NUM_DEVICES) && !isEventFound; deviceId++)
			{
 				for(channel=0 ; (channel < NUM_LINES) && !isEventFound ; channel++)
				{
					spin_lock(&hook_state_lock);
					
					if(TRUE == TelephonyEventBuffer[deviceId][channel].isHookStateChange)
					{
						TelephonyEventBuffer[deviceId][channel].isHookStateChange = FALSE;
						
						TelephonyEvent.Channel = channel +NUM_LINES*deviceId;
						
						if(TelephonyEventBuffer[deviceId][channel].fxsOnHook){
							TelephonyEvent.EventType = ACG_EVENT_TYPE__ON_HOOK;
							}
						else{
							TelephonyEvent.EventType = ACG_EVENT_TYPE__OFF_HOOK;
							}

						isEventFound = 1;					
					}

					if(TRUE == TelephonyEventBuffer[deviceId][channel].fxsFlash)
					{
						TelephonyEventBuffer[deviceId][channel].fxsFlash = FALSE;			
						TelephonyEvent.Channel = channel +NUM_LINES*deviceId;
						TelephonyEvent.EventType = ACG_EVENT_TYPE__FLASH_HOOK;
						isEventFound = 1;
					}

					if(TRUE == TelephonyEventBuffer[deviceId][channel].polarityReversal)
					{
						TelephonyEventBuffer[deviceId][channel].polarityReversal = FALSE;			
						TelephonyEvent.Channel = channel +NUM_LINES*deviceId;
						TelephonyEvent.EventType = ACG_EVENT_TYPE__POLARITY_REVERSAL;
						isEventFound = 1;
					}

					if(TRUE == TelephonyEventBuffer[deviceId][channel].fxoRingStart)
					{
						TelephonyEventBuffer[deviceId][channel].fxoRingStart = FALSE;			
						TelephonyEvent.Channel = channel +NUM_LINES*deviceId;
						TelephonyEvent.EventType = ACG_EVENT_TYPE__RING_START;
						isEventFound = 1;
					}

					if(TRUE == TelephonyEventBuffer[deviceId][channel].fxoRingEnd)
					{
						TelephonyEventBuffer[deviceId][channel].fxoRingEnd = FALSE;			
						TelephonyEvent.Channel = channel +NUM_LINES*deviceId;
						TelephonyEvent.EventType = ACG_EVENT_TYPE__RING_END;
						isEventFound = 1;
					}
					
					spin_unlock(&hook_state_lock);

 				}
 			}

			if(copy_to_user(pGetEventInfo->Buff, &TelephonyEvent, sizeof(acgTEventInfo))) {
				ret = -EFAULT; 
			}
			else
			{
				ret = LE88_OK_E;
			}		

//			printk("fxsFlash: %x, deviceid[%x], channel[%x]\n",
//				HookState.state, deviceId, Channel-2*deviceId);	
			/*copy_to_user((int *)arg, &hook_state[ch], sizeof(int));*/

			break;


		default:
			ret = -EINVAL;
			break;	
	} /*end switch*/

	return ret;

}	/* end of le88Dev_read() */


/****************************************************************************
*                                                                            
*	Name:		le88Dev_write	 			                                                     
*----------------------------------------------------------------------------                                                                            
*	Abstract:	write routine to the device
*----------------------------------------------------------------------------                                                                            
*	Input:		file 	- file decriptor
*				buf		- buf to fill
*				count	- size of buf
*				ppos	- flags
*----------------------------------------------------------------------------                                                                            
*  	Output:		none			                                                  
*----------------------------------------------------------------------------                                                                            
*	Returns: 	case of error = 0                              
******************************************************************************/
static ssize_t le88Dev_write( struct file * file, char * buf, size_t count, loff_t *ppos )
{

	int ret = LE88_OK_E;
	int Channel;
	int lineNum;
	int i, j;
    	VpStatusType status;
//	VpOptionTimeslotType timeslot;
//	VpDeviceIdType deviceId;

	acgTCommand			*pAcgCmd=NULL;

	/* cast the pointer */
	pAcgCmd = (acgTCommand *)buf;


	switch(pAcgCmd->Command) {

#if 0
		case SLIC_SET_LINE_STATE_CMD:

			pLineState = (LE88_LINE_STATE_STCT *)buf;   /* cast the pointer */
			Channel = pLineState->channel;
			State = pLineState->state;

			for (i = 0; i<NUM_DEVICES; i++) /*number of device*/
				if (!gsLE88InitComplete[i])
			{
				printk("<1>LE88 drv: SLIC is not initialized yet, ignoring state %d, channel %d\n", State, Channel);
				ret = LE88_ERROR_E;
				break;
			}

			is_hook_state_testable = 0;

  			switch (State)
    			{    	    
    	    
				case mtRINGING:
					printk("<1>mtRINGING: Start ringing on channel %d\n", Channel);
					VpSetLineState(&pLineCtx[Channel], VP_LINE_RINGING);
					break;
				case mtACTIVE:
					printk("<1>Setting the channel to active mode  - active -talk (channel %d)\n", Channel);
					VpSetLineState(&pLineCtx[Channel], VP_LINE_TALK/*VP_LINE_ACTIVE*/);
					break;
				case mtSTANDBY:
					printk("<1>Setting the channel to standby mode (channel %d)\n", Channel);
					VpSetLineState(&pLineCtx[Channel], VP_LINE_STANDBY);
					break;
				default:
					ret = -EINVAL;
					break;
	    
			}	/*switch (State)*/

			is_hook_state_testable = 1;

			break;
#endif

		case ACG_SLIC_START_RING_CMD:

			Channel = pAcgCmd->Channel;
			
			if (!gsLE88InitComplete[Channel/2])
			{
				printk("<1>LE88 drv: SLIC is not initialized yet, ignoring request for ringing\n");
				ret = LE88_ERROR_E;
				break;
			}
#if 0
			is_hook_state_testable = 0;

//			printk("<1>Start ringing on channel %d\n", Channel);
			VpSetLineState(&pLineCtx[Channel], VP_LINE_RINGING);

			is_hook_state_testable = 1;
#endif
			/* AlexChen 20090211 */
			spin_lock(&hook_state_lock);
			FxsSetStates[Channel] = VP_LINE_RINGING;
			spin_unlock(&hook_state_lock);
			break;


		case ACG_SLIC_STOP_RING_CMD:

			Channel = pAcgCmd->Channel;
			
		
			if (!gsLE88InitComplete[Channel/2])
			{
				printk("<1>LE88 drv: SLIC is not initialized yet, ignoring request for ringing\n");
				ret = LE88_ERROR_E;
				break;
			}
#if 0
			is_hook_state_testable = 0;

//			printk("<1>Stop ringing on channel %d\n", Channel);
			VpSetLineState(&pLineCtx[Channel], VP_LINE_TALK/*VP_LINE_ACTIVE*/);

			is_hook_state_testable = 1;
#endif
			/* AlexChen 20090211 */
			spin_lock(&hook_state_lock);
			FxsSetStates[Channel] = VP_LINE_TALK;
			spin_unlock(&hook_state_lock);
			break;


		case ACG_SLIC_INIT_CMD:

			for (i=0; i<NUM_DEVICES; i++)
			{
				if(gsLE88InitComplete[i])
				{
					printk("<1>\nLegerity SLIC is already initialized\n");
					return LE88_OK_E;
				}
			}

			printk("<1>\nLegerity SLIC Initialization v1. NUM_DEVICES = %d\n", NUM_DEVICES);
			
			for (i=0; i<NUM_DEVICES; i++)
			{
				VpMakeDeviceObject(VP_DEV_880_SERIES, i, &DevCtx[i], &DevObj[i]);
				/* Make the Line Objects */
				for (lineNum = 0; lineNum < 2; lineNum++) 
				{
					status = VpMakeLineObject(VP_TERM_FXS_GENERIC, lineNum, &pLineCtx[lineNum+2*i], 
											&pLineObj[lineNum+2*i], &DevCtx[i]);
					if (status != VP_STATUS_SUCCESS) 
					{
						printk("<1>LE88 driver: Error make line object, status=%d\n", status);
						ret = LE88_ERROR_E;
					}
				}

				/* Initialize the Device */
				status = VpInitDevice(&DevCtx[i], DEV_PROFILE, AC_600R_FXS, DC_23mA_45V, RING_25HZ_SINE, 
									VP_PTABLE_NULL /*AC_600R_FXO*/, VP_PTABLE_NULL /*FXO_DIALING*/);

				if (status != VP_STATUS_SUCCESS) 
				{
					printk("<1>\nLE88 driver[%x]: Device Did Not Initialize, status=%d\n", i, status);
		
				}
				else 
				{
					printk("<1>\nLE88 driver[%x] driver: Device Initialized.\n", i);
					gsLE88InitComplete[i]=1;
				}
				
		

			}
			
			ret = LE88_OK_E;
			add_timer( &le88Timer );	
			
			break;
#if 0
		case SLIC_SET_TIME_SLOT_CMD:
			
			pTimeSlot = (LE88_TIME_SLOT_STCT *)buf;   /* cast the pointer */
			switch(pTimeSlot->channel){
				case 0:
				case 1:
					deviceId = (VpDeviceIdType)0;
					break;
				case 2:
				case 3:
					deviceId = (VpDeviceIdType)1;			
					break;
				case 4:
       			        case 5:
                    			deviceId = (VpDeviceIdType)2;
					break;
		                case 6:
		                case 7:
                    			deviceId = (VpDeviceIdType)3;
					break;
			}

			timeslot.rx = pTimeSlot->rxSlot;
			timeslot.tx = pTimeSlot->txSlot;
			is_hook_state_testable = 0;
//            status = Vp880SetTimeSlot(pLineCtx[pTimeSlot->channel], 
//								pTimeSlot->txSlot, pTimeSlot->rxSlot);
			status = VpSetOption(&pLineCtx[pTimeSlot->channel], VP_NULL, 
							VP_OPTION_ID_TIMESLOT, &timeslot);


//			 status = VpSetOption(VP_NULL, &DevCtx[deviceId],
 //                         VP_OPTION_ID_TIMESLOT, (void*) timeslot);

			if (status != VP_STATUS_SUCCESS) {
                        printk("<1>LE88 driver: Error setTimeSlot, status=%d\n", status);
                        ret = LE88_ERROR_E;
                    }
			is_hook_state_testable = 1;

			break;
#endif
		case ACG_UPDATE_FXS_CADENCE_PROFILE:
			{
			enum_ring_cadence_profile_t enum_ring_cadence_profile = (enum_ring_cadence_profile_t)pAcgCmd->data;
			
			if (enum_ring_cadence_profile == CUR_RING_CADENCE)
				return LE88_OK_E; /* no change of candence */

			spin_lock(&hook_state_lock);
			
			/* update CUR_RING_CADENCE */
			CUR_RING_CADENCE = enum_ring_cadence_profile;
			
			//update the FXS parameters: coefficients and cadence
			for (i=0; i<NUM_DEVICES; i++){
				for (i=0; i<NUM_LINES; i++)
				{
					switch(enum_ring_cadence_profile) {
					case SLIC_RING_CADENCE_PROFILE_1:
	                    status=VpInitRing(&pLineCtx[i], RING_CAD_STD, VP_PTABLE_NULL);
					    if (status != VP_STATUS_SUCCESS) {
						    printk("LE88 driver(file line=%d, fxs line=%d): Error VpInitRing failed status=%d\n", __LINE__, i, status);
					    }
						ret = LE88_ERROR_E;
						break;
					case SLIC_RING_CADENCE_PROFILE_2:
	                    status=VpInitRing(&pLineCtx[i], RING_CAD_2, VP_PTABLE_NULL);
					    if (status != VP_STATUS_SUCCESS) {
						    printk("LE88 driver(file line=%d, fxs line=%d): Error VpInitRing failed status=%d\n", __LINE__, i, status);
					    }
						ret = LE88_ERROR_E;
						break;
					case SLIC_RING_CADENCE_PROFILE_3:
	                    status=VpInitRing(&pLineCtx[i], RING_CAD_3, VP_PTABLE_NULL);
					    if (status != VP_STATUS_SUCCESS) {
						    printk("LE88 driver(file line=%d, fxs line=%d): Error VpInitRing failed status=%d\n", __LINE__, i, status);
					    }
						ret = LE88_ERROR_E;
						break;
					default:
				        printk("slic_ioctl: file=%s, line=%d, got SLICIOC_UPDATE_FXS_CADENCE_PROFILE enum_ring_cadence_profile =%d INVALID .\r\n", __FILE__, __LINE__, enum_ring_cadence_profile);
						ret = LE88_ERROR_E;
	                    break;
					}
				}
			}
			spin_unlock(&hook_state_lock);
		}
		break;	

		default:
			ret = -EINVAL;
			break;

	} 	

    return ret;

}	/* end of le88Dev_write() */




/*********************************************************************************************/

static int le88Dev_ioctl(struct file *fp, 
								unsigned int cmd, 
								unsigned long arg)		
{    
	return 0;
}

/*********************************************************************************************/

static int le88Dev_open( 	struct inode 	*inode, 
							struct file 	*file )
{
	hook_state_condition = MAX_DEVICES*NUM_LINES;

	return 0;
}

/*********************************************************************************************/

static int le88Dev_release( struct inode 	*inode, 
							struct file 	*file )
{
	return 0;
}
/*********************************************************************************************/

int le88Dev_fasync(	int 		fd, 
					struct file *file, 
					int 		mode)
{
	return 0;
}

/*********************************************************************************************/

static int le88Dev_flush(struct file *file)
{
	return 0;
}

/*********************************************************************************************/


/* set the device operations to the functions pointers */
struct file_operations le88Dev_fops = {
	owner:  THIS_MODULE,
	read:   le88Dev_read,
	write:	le88Dev_write,
	unlocked_ioctl:	le88Dev_ioctl,
	fasync: le88Dev_fasync,
	flush:  le88Dev_flush,
	open:	le88Dev_open,
	release:  le88Dev_release
};


/****************************************************************************
*                                                                            
*	Name:		le88Dev_init	 			                                                     
*----------------------------------------------------------------------------                                                                            
*	Abstract:	initialize the device (call with insmod)
*----------------------------------------------------------------------------                                                                            
*	Input:		none
*----------------------------------------------------------------------------                                                                            
*  	Output:		none			                                                  
*----------------------------------------------------------------------------                                                                            
*	Returns: 	case of success = 0, else -1                              
******************************************************************************/
int __init le88Dev_init(void)
{

	int ret = 0;
	int i, j;
	unsigned char rcn_pcn[2][2];
	volatile unsigned int regValue;
	
	printk("LE88: device init start.\n");

	for (i = 0; i < MAX_DEVICES; i++)
	{
		for (j = 0; j < NUM_LINES; j++)
		{
			TelephonyEventBuffer[i][j].fxsOnHook = TRUE;
			TelephonyEventBuffer[i][j].isHookStateChange = FALSE;
			TelephonyEventBuffer[i][j].fxsFlash = FALSE;
			TelephonyEventBuffer[i][j].flashCheck = TRUE;
			TelephonyEventBuffer[i][j].onHookCount = 0;
			TelephonyEventBuffer[i][j].hookStateArray = TRUE;
			TelephonyEventBuffer[i][j].polarityReversal = FALSE;
			TelephonyEventBuffer[i][j].fxoRingStart = FALSE;
			TelephonyEventBuffer[i][j].fxoRingEnd = FALSE;
		}

		gsLE88InitComplete[i]=0;
	}

	/* AlexChen 20090211 */
	for(i=0; i<MAX_LINES; i++)
		FxsSetStates[i] = VP_LINE_STATE_ENUM_SIZE;
	flashTimer = FLASH_COUNTER / (LE88_POLLING_INTERVAL/1000);
	hook_state_condition = 0;
	
	CUR_RING_CADENCE = SLIC_RING_CADENCE_PROFILE_1;
//	devfs_register( NULL, LE88_DEV_NAME, DEVFS_FL_AUTO_DEVNUM, 0, 0,
//		S_IFCHR | S_IRUGO | S_IWUGO, &le88Dev_fops,NULL);
#if !defined(CONFIG_ORCHID) && !defined(CONFIG_ORCHID_ATA) 
/*for Orchid, the DSP_RESET_DSBL_CTRL register doesn't exists*/ 
	regValue = *(volatile unsigned int *)(KSEG1ADDR(DSP_RESET_DSBL_CTRL));
	mb();

	printk("%x \n", regValue);
	if(regValue & 0x00000004)
		printk("MPI interface is ready.\n");
	else
	{
		printk(	"MPI interface not found!\n"
			"Please activate it before loading the Le88 driver.\n");
		return -ENOMEM;
	}
#endif
	/* YairE */
	{
		ret = register_chrdev( SLIC_MAJOR, ACL_FXS_DRV_FD_NAME_STR, &le88Dev_fops );
		printk("register rc=%d\n", ret);
	}

#if defined(CONFIG_ORCHID) || defined(CONFIG_ORCHID_ATA)
#warning "It is a LE88_FOR_ORCHID"
    /*pin system reset*/
    printk("AC494E--MPI(CS#1=GPIO43;CS#2=GPIO46;CS#3=GPIO45;CS#4=GPIO48)\n");

    /*PINSEL: 15#25-24:GPIO43, 15#11-10:GPIO46, 15#7-6:GPIO45, 15#13-12:GPIO48*/
	*(volatile unsigned int*)(KSEG1ADDR(0x08611644)) |= 0x02001440;
	mb();
	printk("GPIO Select\n", ret);
	
	/*IO direction*/
	*(volatile unsigned int*)(KSEG1ADDR(0x08610914)) &= 0xE97FF;
	mb();
	printk("IO direction\n", ret);
	
	/*GPIO ENABLE*/
	*(volatile unsigned int*)(KSEG1ADDR(0x0861091C)) |= 0x16800;
	mb();
	printk("GPIO ENABLE\n", ret);
	
	/*Data Output*/
	*(volatile unsigned int*)(KSEG1ADDR(0x0861090C)) |= 0x16800;
	mb();
	printk("Data Output\n", ret);
#else    
    printk("AC494--MPI(CS#1=GPIO44;CS#2=GPIO45;CS#3=GPIO46;CS#4=GPIO47)\n");

    /**** BW: I donot know why it resets bit8-bit16, they are keypad 00-08 ****/
    
    /*PINSEL: 13#25-24:GPIO44, 13#27-26:GPIO45, 13#29-28:GPIO46, 13#31-30:GPIO47*/
	/*pin system reset*/
	*(volatile unsigned int*)(KSEG1ADDR(0x0861163C)) |= 0x0F000000;
	mb();
	printk("pin system reset\n", ret);
	/*reset IO8~IO15*/
	*(volatile unsigned int*)(KSEG1ADDR(0x08611624)) |= 0xFCFFC000;
	mb();
	printk("reset IO8~IO15\n", ret);
	/*reset IO16*/
	*(volatile unsigned int*)(KSEG1ADDR(0x08611628)) |= 0x00000003;
	mb();
	printk("reset IO16\n", ret);
	/*IO direction*/
	*(volatile unsigned int*)(KSEG1ADDR(0x08610914)) &= 0xF0FFF;
	mb();
	printk("IO direction\n", ret);
	/*IO direction (IO8~IO16)*/
	*(volatile unsigned int*)(KSEG1ADDR(0x08610910)) &= 0xFFFE00FF;
	mb();
	printk("IO direction (IO8~IO16)\n", ret);
	/*GPIO ENABLE*/
	*(volatile unsigned int*)(KSEG1ADDR(0x0861091C)) |= 0xF000;
	mb();
	printk("GPIO ENABLE\n", ret);
	/*GPIO ENABLE (IO8~IO16)*/
	*(volatile unsigned int*)(KSEG1ADDR(0x08610918)) |= 0x0001FF00;
	mb();
	printk("GPIO ENABLE (IO8~IO16)\n", ret);
	/*Data Output*/
	*(volatile unsigned int*)(KSEG1ADDR(0x0861090C)) |= 0xF000;
	mb();
	printk("Data Output\n", ret);
	/*Data Output (IO8~15)*/
	*(volatile unsigned int*)(KSEG1ADDR(0x08610908)) |= 0x0000FF00;
	mb();
	printk("Data Output (IO8~15)\n", ret);
#endif

	VpMpiCmd(0, 0, 0x73, 2, rcn_pcn[0]);
	printk("RCN:%x PCN:%x\n", rcn_pcn[0][0], rcn_pcn[0][1]);
	VpMpiCmd(1, 0, 0x73, 2, rcn_pcn[1]);
	printk("RCN:%x PCN:%x\n", rcn_pcn[1][0], rcn_pcn[1][1]);
	
	printk("LE88_DRV: Le88XXX driver installed.\n");

    
	InitCriticalSectionDB();

	init_timer ( &le88Timer );
	le88Timer.expires = jiffies + (HZ/100);
	le88Timer.function = timerFunction;

	init_waitqueue_head(&hook_state_wait);
	spin_lock_init(&hook_state_lock);

    	return 0;

} /* end of le88Dev_init() */


void le88Dev_exit(void)
{
	unregister_chrdev(SLIC_MAJOR, ACL_FXS_DRV_FD_NAME_STR);

	del_timer(&le88Timer);

	return;
}

/* this is the timer function */
static void timerFunction( unsigned long dummy )
{	
	int channel;
	int tmpHook;
	int deviceId = 0;

	le88Timer.expires += (HZ/100);
    	add_timer( &le88Timer );	
	

	//if (is_hook_state_testable )  
	{	
		/* using polling method to periodically call eventHandle. */
		eventHandle();
	}
	for(deviceId = 0; deviceId < NUM_DEVICES; deviceId++)
	{
		/* on-hook, off-hook and flash detection and calculation */
		for(channel=0 ; channel < NUM_LINES ; channel++) 
		{
			spin_lock(&hook_state_lock);
			tmpHook = 1 - TelephonyEventBuffer[deviceId][channel].fxsOnHook;
			spin_unlock(&hook_state_lock);

			if(!tmpHook) 
			{
				if(TelephonyEventBuffer[deviceId][channel].flashCheck) 
				{
					TelephonyEventBuffer[deviceId][channel].onHookCount++;
					if(TelephonyEventBuffer[deviceId][channel].onHookCount > flashTimer) 
					{
						TelephonyEventBuffer[deviceId][channel].flashCheck = 0;

						/*** line is hook on ***/
						TelephonyEventBuffer[deviceId][channel].isHookStateChange = TRUE;

						hook_state_condition += 1;
						wake_up_interruptible(&hook_state_wait);
					}
				}
			}							

			if(TelephonyEventBuffer[deviceId][channel].hookStateArray != tmpHook) 
			{
				TelephonyEventBuffer[deviceId][channel].hookStateArray = tmpHook;

				if(TelephonyEventBuffer[deviceId][channel].hookStateArray) 
				{				
					if(TelephonyEventBuffer[deviceId][channel].onHookCount < flashTimer) 
					{
						/*** flash detected ***/
						spin_lock(&hook_state_lock);
							TelephonyEventBuffer[deviceId][channel].fxsFlash = TRUE;	
						spin_unlock(&hook_state_lock);
				
						hook_state_condition += 1;
						wake_up_interruptible(&hook_state_wait);						
					}
					else
					{
						/*** line is hook off ***/
						TelephonyEventBuffer[deviceId][channel].isHookStateChange = TRUE;

						hook_state_condition += 1;
						wake_up_interruptible(&hook_state_wait);
					}
						TelephonyEventBuffer[deviceId][channel].flashCheck=0;						
				}
				else 
				{
					TelephonyEventBuffer[deviceId][channel].onHookCount=0;
					TelephonyEventBuffer[deviceId][channel].flashCheck=1;
				}				
			}	
		}
	}

	/* AlexChen 20090211 */
	for(channel=0 ; channel < NUM_LINES*NUM_DEVICES ; channel++) {
		if (FxsSetStates[channel] != VP_LINE_STATE_ENUM_SIZE){
        	//set_slic_state(channel, FxsSetStates[channel]);
        	if( !( ( VP_LINE_RINGING == FxsSetStates[channel] ) 
        	    && ( FALSE == TelephonyEventBuffer[channel/NUM_LINES][channel%NUM_LINES].fxsOnHook )
        	     )
        	  )/* BW-2011-03-24: if the line is off-hook, it does not allow to operate the ring */
			    VpSetLineState(&pLineCtx[channel], FxsSetStates[channel]);
        	FxsSetStates[channel] = VP_LINE_STATE_ENUM_SIZE;
		}
        }

	
}


module_init(le88Dev_init);
module_exit(le88Dev_exit);

