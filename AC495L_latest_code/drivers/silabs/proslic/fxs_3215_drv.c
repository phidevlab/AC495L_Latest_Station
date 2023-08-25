/**************************************************************************** 
 *  Copyright (c) 2002 Jungo LTD. All Rights Reserved.
 * 
 *  vendor/audiocodes/ac494/fxos/silabs/proslic/fxs_3215_drv.c
 *  This module includes the fxs driver ioctl and the wrapper functions implementation
 *
 *  Developed by AudioCodes CMBU (Nir Pardo).
 *
 *  This copyright notice should not be removed
 */

#include <linux/config.h>
#include <linux/module.h>
#include <linux/types.h>
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
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/devfs_fs_kernel.h>
#include <linux/spinlock.h> 
//#include <linux/wait.h>
//Linux 2.6 only
#include <linux/cdev.h>
#include <pal.h>

#include "../silabs_drv.h"
#include "../silabs_def.h"
#include "../dsp_debug_cmd.h"
#include "../../ac49x_mpi/mpi_drv.h"

#include "proslic.h"
#include "proslic_datatypes.h"
#include "proslic_ctrl.h"
#include "proslic_datatypes.h"
#include "proslic_timer_intf.h"
#include "SPI.h"
#include "silabs_fxs_wrapper.h"

#ifndef acl_min
#define acl_min(x, y)	(((x) < (y)) ? (x) : (y))
#endif


#ifndef acl_max
#define acl_max(x, y)	(((x) < (y)) ? (y) : (x))
#endif


#ifdef FXS_3215_DRV_DEBUG
#define DBG_PRINT(a1,a2...)	printk("[%s:%d] " a1, __FUNCTION__, __LINE__, ##a2)
#else
#define DBG_PRINT(a1,a2...)
#endif


typedef struct {

	char 		fxsOnHook;
	char 		isHookStateChange;	/* Indicating whether the hook state has changed */
	char 		onHookCount;
	char 		hookStateArray;

	char 		fxsFlash;
	char 		flashCheck;
}SlicEventBuffer_t;
static SlicEventBuffer_t  TelephonyEventBuffer[NUM_SLIC_LINES];
static int 						flashTimer;
static int						hook_state_condition;
static wait_queue_head_t		hook_state_wait;
static spinlock_t 				hook_state_lock;
static struct timer_list 			SilabsFxsTimer;
extern struct semaphore		semToReadWriteFromMPI;
extern int                                     mpi_polling_disable; 
static int gs3215InitComplete=0;
static void init_fxs_status(void);
void SetSLICCS(unsigned char state,int CID)
{

	int cs = -1;
    
     cs = CHIP_CS(CID);

    if(cs>=0)
    {
	PAL_sysGpioOutBit(SILABS_MODE,  SILABS_FXS_SELECTED);		
	if(state==CS_UP)
		PAL_sysGpioOutBit(cs, SLIC_SELECTED);
	else
		PAL_sysGpioOutBit(cs, SLIC_UNSELECTED);
	
    }
    return;
} 
uInt8 fxs_read_reg (uInt16 portID, uInt8 channel, uInt8 regAddr){
    uInt8 buf, ret_value;
    int ret_code;
#if defined(CONFIG_TULIP) || defined(CONFIG_SMB) || defined(CONFIG_ORCHID) || defined(CONFIG_ORCHID_ATA)
 	SetSLICCS(CS_UP ,portID);
#endif
#ifdef CONFIG_AC494_SILABS_FXOS_DAISY_CHAIN_MODE
    buf=1<<channel;
    spi8xx_byte_write(buf);
#endif
	buf=regAddr | 0x80;
	//printk("ReadReg: portID=%d, channel=%d, RegAddr=%d\n", portID, channel, regAddr);
    ret_value=spi8xx_byte_write_read(buf, &ret_code);
    //printk("ReadReg: portID=%d, channel=%d, RegAddr=%d, RegValue=0x%X \n", portID, channel, regAddr, ret_value);
#if defined(CONFIG_TULIP) || defined(CONFIG_SMB) || defined(CONFIG_ORCHID) || defined(CONFIG_ORCHID_ATA)
    SetSLICCS(CS_DOWN ,portID);
#endif
    return ret_value;
}
    
void fxs_write_reg (uInt16 portID, uInt8 channel, uInt8 regAddr, uInt8 data){

    uInt8 buf;
    //printk("WriteReg: portID=%d, channel=%d, RegAddr=%d, data=0x%X \n", portID, channel, regAddr, data);
#if defined(CONFIG_TULIP) || defined(CONFIG_SMB) || defined(CONFIG_ORCHID) || defined(CONFIG_ORCHID_ATA)
		SetSLICCS(CS_UP ,portID);
#endif

#ifdef CONFIG_AC494_SILABS_FXOS_DAISY_CHAIN_MODE
    buf=1<<channel;
    spi8xx_byte_write(buf);
#endif
    buf=regAddr;
    spi8xx_byte_write(buf);
    buf=data;
    spi8xx_byte_write(buf);

#if defined(CONFIG_TULIP) || defined(CONFIG_SMB) || defined(CONFIG_ORCHID) || defined(CONFIG_ORCHID_ATA)
    SetSLICCS(CS_DOWN ,portID);	
#endif
    return;
}

void DoDumpRegs(unsigned char ch)
{
    int i;
    printk("DoDumpRegs >>>>>> \n");

    for (i=0; i<=108;i++)
        printk("Register %d = 0x%X \n", i, fxs_read_reg (0/*NA*/, ch, i));
}



  
static ssize_t slic_read( struct file * file, char * buf, size_t count, loff_t *ppos )
{


	int ret;
	int i;
	int isEventFound = 0;
	
	acgTEventInfo 			TelephonyEvent;
	acgTCommand			*pAcgCmd=NULL;
	acgEPollingMethod 		PollingMethod;

	acgTGetEventInfo		*pGetEventInfo=NULL;

#if defined(CONFIG_TULIP2_21) || defined(CONFIG_TULIP2_40)
	PAL_sysGpioOutBit(44, 1);
#endif
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

		       
			for(i=0 ; (i < NUM_SLIC_LINES) && !isEventFound ; i++)
			{	
			        spin_lock(&hook_state_lock);  
				TelephonyEvent.Channel = SLIC_TO_CH(i);	
                            if(TRUE == TelephonyEventBuffer[i].fxsFlash)
				{
					TelephonyEventBuffer[i].fxsFlash = FALSE;			
					TelephonyEvent.EventType = ACG_EVENT_TYPE__FLASH_HOOK;
					isEventFound = 1;
				}
                            
                             if(TRUE == TelephonyEventBuffer[i].isHookStateChange)
                             {
                                    TelephonyEventBuffer[i].isHookStateChange = FALSE;
        				if(TelephonyEventBuffer[i].fxsOnHook==0)
        					TelephonyEvent.EventType = ACG_EVENT_TYPE__ON_HOOK;
        				else
        					TelephonyEvent.EventType = ACG_EVENT_TYPE__OFF_HOOK;
                                    isEventFound = 1;
                             }										
                             spin_unlock(&hook_state_lock);  

			}
			

			if(copy_to_user(pGetEventInfo->Buff, &TelephonyEvent, sizeof(acgTEventInfo))) {
				ret = -EFAULT; 
			}
			else
			{
				ret = SILAB_OK_E;
			}		

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
static ssize_t slic_write( struct file * file, const char * buf, size_t count, loff_t *ppos )
{
				int ret = SILAB_OK_E;
       int  Channel;
       int slic_index;
#if defined(CONFIG_TULIP2_21) || defined(CONFIG_TULIP2_40)
				static int iFirst = 1;
#endif
     

	acgTCommand			*pAcgCmd=NULL;
	/* cast the pointer */
	pAcgCmd = (acgTCommand *)buf;
	
#if defined(CONFIG_TULIP2_21) || defined(CONFIG_TULIP2_40)
	if (iFirst){
		PAL_sysGpioCtrl(44, GPIO_PIN, GPIO_OUTPUT_PIN);                                
	        PAL_sysGpioOutBit(44, 1);
        	//NIRP 070730 set all FXS's to daisy chain mode - broadcast
        	spi8xx_byte_write(0);
        	spi8xx_byte_write(0x80);
        	iFirst=0;
	}
#endif
        if( (!gs3215InitComplete)&&(pAcgCmd->Command!=ACG_SLIC_INIT_CMD))
	{
		printk("Silabs drv: SLIC is not initialized yet, return error\n");
		return SILAB_ERROR_E;
	}	

       down_interruptible(&semToReadWriteFromMPI); 
       mpi_polling_disable = 1;
#if defined(CONFIG_TULIP2_21) || defined(CONFIG_TULIP2_40)
	PAL_sysGpioOutBit(44, 1);
#endif
	switch(pAcgCmd->Command) {

		case ACG_SLIC_START_RING_CMD:
            
			Channel = pAcgCmd->Channel;
			//printk("<1>Start ringing on channel %d\n", Channel);	

                     slic_index = CH_TO_SLIC(Channel);
                     if(IsValidSlic(slic_index))
                     {
		            fxs_RingStart (slic_index);         
                     }
                     else
                        ret = SILAB_ERROR_E;
		       break;
		case ACG_SLIC_STOP_RING_CMD:
                     Channel = pAcgCmd->Channel;			
	
                      slic_index = CH_TO_SLIC(Channel); 
                      if(IsValidSlic(slic_index))
                       {
                            fxs_PCMStop(slic_index);
            		       fxs_RingStop (slic_index);
                       }
                      else
                        ret = SILAB_ERROR_E;
        		
			break;
		case ACG_SLIC_INIT_CMD:
                    if(gs3215InitComplete)
                    {
                            printk("Silabs SLIC is already initialized\n");
                            break;
                    }
                     init_fxs_status();
                     if(fxs_initialize()==0)  
                     {
                           gs3215InitComplete=1;
                           add_timer( &SilabsFxsTimer );
#if defined(CONFIG_TULIP2_21) || defined(CONFIG_TULIP2_40)	 
			   /* alex mp203 */
			   PAL_sysGpioCtrl(45, GPIO_PIN, GPIO_OUTPUT_PIN);                                
			   PAL_sysGpioOutBit(45, 0);//LifeLine
#endif
                     }
                     else 
                           gs3215InitComplete=0;               
			   /*DoDumpRegs(0);
			   DoDumpRegs(1);
		           DoDumpRegs(2);
 			   DoDumpRegs(3);*/
          		break;
		default:
			ret = -EINVAL;
			break;

	} 	
     mpi_polling_disable = 0;
     up(&semToReadWriteFromMPI); 
    return ret;


}	




/*********************************************************************************************/

static long slic_ioctl( 	struct inode 	*inode, 
							struct file 	*file, 
							unsigned int 	cmd, 
							unsigned long 	arg )
{    
	return 0;
}

/*********************************************************************************************/

static int slic_open( 	struct inode 	*inode, 
							struct file 	*file )
{

	hook_state_condition = NUM_SLIC_LINES;
	return 0;
}

/*********************************************************************************************/

static int slic_close( struct inode 	*inode, 
							struct file 	*file )
{
	return 0;
}



struct file_operations slic_fops = {
	owner:	THIS_MODULE,
	read:	slic_read,
	write:	slic_write,
    unlocked_ioctl:    slic_ioctl,
    open:    slic_open,
    release:  slic_close
};




static void timerFunction( unsigned long dummy )
{	
	int i;
	int tmpHook;
	unsigned char hookStat;
    
	SilabsFxsTimer.expires += (HZ/100);
       add_timer( &SilabsFxsTimer );	

       if(mpi_polling_disable)return;
       if(gs3215InitComplete==0) return;
       
        /* on-hook, off-hook and flash detection and calculation */
      for(i=0; i<NUM_SLIC_LINES; i++)
      {

            if(!IsValidSlic(i))continue;
             hookStat = 0;
                
        	fxs_ReadHookStatus(i, &hookStat);
              tmpHook =hookStat;
              spin_lock(&hook_state_lock);  

              switch(TelephonyEventBuffer[i].hookStateArray)
               {
                    case 0:   //hook on status
                        if(tmpHook==1)
                       {
                            TelephonyEventBuffer[i].onHookCount = 0;
                            TelephonyEventBuffer[i].hookStateArray = 1;
                       }
                        break;
                    case 1:  
                        TelephonyEventBuffer[i].onHookCount++;
                        if(tmpHook==0){
                            TelephonyEventBuffer[i].hookStateArray = 0;
			}
                        else if(TelephonyEventBuffer[i].onHookCount>flashTimer)
                        {
                             /*report hook off status*/   
                            TelephonyEventBuffer[i].fxsOnHook = tmpHook;     
                            TelephonyEventBuffer[i].isHookStateChange = TRUE;      
                             hook_state_condition += 1;
				 wake_up_interruptible(&hook_state_wait);   
                             TelephonyEventBuffer[i].onHookCount = 0;
                             TelephonyEventBuffer[i].hookStateArray = 2;      
                             fxs_PCMStart (i);
                        }
                        break;
                    case 2:  
                         if(tmpHook==0)
                          {
                               TelephonyEventBuffer[i].onHookCount = 0;
                              TelephonyEventBuffer[i].hookStateArray = 3;
                         }
                         break;
                     case 3:
                         TelephonyEventBuffer[i].onHookCount++;
                        if(tmpHook==1)
                        {
                             /*report flash status*/   
                             
                             TelephonyEventBuffer[i].fxsOnHook = tmpHook;
                             TelephonyEventBuffer[i].fxsFlash = TRUE;
                            
                             hook_state_condition += 1;
				 wake_up_interruptible(&hook_state_wait);  
                             TelephonyEventBuffer[i].hookStateArray = 2;
                        }
                       else if(TelephonyEventBuffer[i].onHookCount>flashTimer)
                        {
                             /*report hook on status*/                                  
                             TelephonyEventBuffer[i].fxsOnHook = tmpHook; 
                             TelephonyEventBuffer[i].isHookStateChange = TRUE; 
                             hook_state_condition += 1;
				 wake_up_interruptible(&hook_state_wait); 
                             TelephonyEventBuffer[i].hookStateArray = 0;  
                              fxs_RingStop (i);
                              fxs_PCMStop (i);
                        }
               } 
               spin_unlock(&hook_state_lock);  
        }

    	

}

static void init_fxs_status()
{   
    int i;
    gs3215InitComplete = 0;
    for (i = 0; i < NUM_SLIC_LINES; i++)
	{
		TelephonyEventBuffer[i].fxsOnHook = 0;
		TelephonyEventBuffer[i].isHookStateChange = FALSE;
		TelephonyEventBuffer[i].fxsFlash = FALSE;
		TelephonyEventBuffer[i].flashCheck = TRUE;
		TelephonyEventBuffer[i].onHookCount = 0;
		TelephonyEventBuffer[i].hookStateArray = 0;		
	}	
}
int  __init silabs_fxs_init(void)
{
#if !defined(CONFIG_ORCHID) && !defined(CONFIG_ORCHID_ATA)
	volatile unsigned int regValue;	
#endif
	int ret;
	
   	flashTimer = SILABS_FXS_FLASH_COUNTER / (SILABS_FXS_POLLING_INTERVAL/1000);
	hook_state_condition = 0;

	ret = register_chrdev( SLIC_MAJOR, SLIC_NAME, &slic_fops );
	printk("register rc=%d\n", ret);
	if(ret) goto init_err;

#if !defined(CONFIG_ORCHID) && !defined(CONFIG_ORCHID_ATA)
	/*for Orchid, the DSP_RESET_DSBL_CTRL register doesn't exists*/

	regValue = *(volatile unsigned int *)(KSEG1ADDR(DSP_RESET_DSBL_CTRL));
       mb();

      if(regValue & 0x00000004)
             printk("MPI interface is ready.\n");
      else
     {
            printk("MPI interface not found!        \
            Please activitiate it before loading the fxs driver.\n");
            return -ENOMEM;
      }
#endif

     //  sema_init(&semToReadWriteFromMPI,1);
	init_timer ( &SilabsFxsTimer );
	SilabsFxsTimer.expires = jiffies + (HZ/100);
	SilabsFxsTimer.function = timerFunction;
      
	init_waitqueue_head(&hook_state_wait);
	spin_lock_init(&hook_state_lock);

	printk("FXS Driver init_module !!!\n");	

       return 0;

init_err:
	return -ENOMEM;	
}

void  silabs_fxs_exit(void)
{
    unregister_chrdev(SLIC_MAJOR, SLIC_NAME);		
    del_timer(&SilabsFxsTimer);
    return;
}


module_init(silabs_fxs_init);
module_exit(silabs_fxs_exit);

