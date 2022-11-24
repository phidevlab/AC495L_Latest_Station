/*-----------------------------------------------------------
*
*       Copyright (C) 2000 AudioCodes Ltd.
* 
*       All rights reserved. Reproduction in whole
*       or in part is prohibited without the written
*       permission of the copyright owner.
*
*------------------------------------------------------------
*------------------------------------------------------------
*
*   FILE:           daa_if.c
*   
*   DESCRIPTION:    correlates between TPApp and DAA driver.
*
*   NOTES:          None.
*
*----------------------------------------------------------*/
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/poll.h>
#include <linux/ioctl.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/proc_fs.h>
#include <linux/devfs_fs_kernel.h>
#include <linux/param.h>
#include <linux/fs.h>

#include "daa_if.h"
//#include <AC49xDrv_Definitions.h>

#include "../silabs_def.h"
#include "../silabs_drv.h"
#include "../dsp_debug_cmd.h"
#include "../../ac49x_mpi/mpi_drv.h"
#include "AnalogDriver.h"
#include "daa_driver.h"
#include "daaioc.h"
/***********************/
/*  D E  B  U  G        */
/***********************/
#define SILAB_FXO_DRV_DEBUG
#ifdef SILAB_FXO_DRV_DEBUG
#define DBG_PRINT(a1,a2...)	printk("[%s:%d] " a1, __FUNCTION__, __LINE__, ##a2)
#else
#define DBG_PRINT(a1,a2...)
#endif

typedef struct{
	char 		polarityReversal;
	char 		fxoRingStart;
	char 		fxoRingEnd;

}DaaEventBuffer_t;

static int						ring_state_condition;
static wait_queue_head_t		ring_state_wait;
static spinlock_t 				ring_state_lock;
static struct timer_list 			daa_timer;
struct semaphore		semToReadWriteFromMPI;
int                                     mpi_polling_disable=0;         
int gs3050InitComplete = 0;
unsigned int ConsecutiveRingDetections[NUM_DAA_LINES];
unsigned char  RingState[NUM_DAA_LINES];
unsigned char  RingDetectFlag;
static DaaEventBuffer_t  TelephonyEventBuffer[NUM_DAA_LINES];
static void init_fxo_status(void);

unsigned char daa_byte_read(unsigned char address,int CID)
{
	unsigned char tmp,WriteReg;
	int ret_code;
	//static int iNirpTempTest=0;

	SetDAACS(CS_UP,CID);
	//printk("daa_byte_read(CID=%d): address is 0x%X \n", CID, address);

	WriteReg = 0x60;				/*control byte: Bit7=0 no braodcast, Bit6=1 read, Bit5= 1 reserved, Bit4= 0 reserved, Bit3:0=CID */
	spi8xx_byte_write(WriteReg);
	udelay(50);//NIRP T.B.D
	tmp = spi8xx_byte_write_read(address, &ret_code);
	//printk("daa_byte_read(CID=%d): data = 0x%X \n",CID,  tmp);

	SetDAACS(CS_DOWN,CID);
	return(tmp);
}
  
void daa_byte_write(unsigned char address, unsigned char data,int CID)
{	
	
	unsigned char WriteReg;

   	SetDAACS(CS_UP,CID);
      // printk("daa_byte_write(CID=%d): Register=%d, data=0x%X . \n", CID, address, data);

       WriteReg = 0x20;				/*control byte: Bit7=0 no braodcast, Bit6=0 write, Bit5= 1 reserved, Bit4= 0 reserved, Bit3:0=CID */
//	if (CID%2)WriteReg = WriteReg | 0x08;
	spi8xx_byte_write(WriteReg);	/*Prepare to Write to index register*/
	udelay(50);
	spi8xx_byte_write(address);		/*address*/
	udelay(50);
	spi8xx_byte_write(data);		/*write data*/

       udelay(50);
     	SetDAACS(CS_DOWN,CID);
}


void DoDAADumpRegs(unsigned char ch)
{
    int i;
    printk("DoDAADumpRegs>>>>>> \n");

    for (i=1; i<=59;i++)
        printk("Register %d = 0x%X \n", i, daa_byte_read(i, ch));
}

/*------------------------------*/
static int daa_ioctl(struct file *fp, unsigned int cmd, unsigned long arg)
{

    int ret = 0;
   //printk("daa_ioctl>>>>>> Enter cmd = (0x%X).\n", cmd);
     switch (cmd)
    {

    case DAAIOC_R_FXO_READ_DEFAULT_VAL:
		{
            int DefaultVal=0;
            DefaultVal = daa_byte_read(3, 0);
			copy_to_user((int *)arg, &DefaultVal, sizeof(int));
		}
	    break;
        
    case DAAIOC_DEBUG_CMD:
		{
			voip_dsp_debug_cmd_t* parg_dsp_debug_cmd=(voip_dsp_debug_cmd_t*)arg;
			// DBG_PRINT("daa_ioctl: line=%d, got DAAIOC_DEBUG_CMD: cmd len=%d param0=%d, param1=0x%X, param2=0x%X \r\n", __LINE__, parg_dsp_debug_cmd->cmd_len, 
			// parg_dsp_debug_cmd->param[0], parg_dsp_debug_cmd->param[1], parg_dsp_debug_cmd->param[2]);
			switch(parg_dsp_debug_cmd->dsp_debug_cmd) 
			{
			case DSP_DEBUG_CMD_LE88_REG_DUMP:
				DoDAADumpRegs((unsigned char)parg_dsp_debug_cmd->param[1]);
				break;
            		case DSP_DEBUG_CMD_MPI_READ:
                		printk("daa_ioctl: DSP_DEBUG_CMD_MPI_READ: Reg 0x%X = 0x%X \n", parg_dsp_debug_cmd->param[2], daa_byte_read(parg_dsp_debug_cmd->param[2], parg_dsp_debug_cmd->param[1]));
				break;
			case DSP_DEBUG_CMD_MPI_WRITE:
                	{
                    		printk("DSP_DEBUG_CMD_MPI_WRITE: CH=%d, Reg=0x%X, Data=0x%X \n", parg_dsp_debug_cmd->param[1], parg_dsp_debug_cmd->param[2], parg_dsp_debug_cmd->param[3]);
	                    daa_byte_write(parg_dsp_debug_cmd->param[2], parg_dsp_debug_cmd->param[3], parg_dsp_debug_cmd->param[1]);
       	             printk("DSP_DEBUG_CMD_MPI_WRITE: Verify read>>>>>>>>> \n");
              	      printk("CH=%d, Reg 0x%X = 0x%02X \n", parg_dsp_debug_cmd->param[1], parg_dsp_debug_cmd->param[2], daa_byte_read(parg_dsp_debug_cmd->param[2], parg_dsp_debug_cmd->param[1]));
			}
			break;
			default:
				printk("slic_ioctl: file=%s, line=%d, ERROR INVALID COMMAND .\r\n", __FILE__, __LINE__);
			}
		}
		break;
	default:
        printk("daa_ioctl>>>>>> Error unknown cmd (%d).\n", cmd);
		ret = -EINVAL;
        break;
    }//switch (cmd)

    return ret;
}  
static int daa_read( struct file * file, char * buf, size_t count, loff_t *ppos)
{	
	int ret = SILAB_OK_E;
	int i=0;
	int isEventFound = 0;
	
	acgTEventInfo 			TelephonyEvent;
	acgTCommand			*pAcgCmd=NULL;
	acgEPollingMethod 		PollingMethod;

	acgTGetEventInfo		*pGetEventInfo=NULL;
	
	/* cast the pointer */
	pAcgCmd = (acgTCommand *)buf;
	if (!gs3050InitComplete)
	{
		printk("Silabs drv: DAA is not initialized yet, return error\n");
		return SILAB_ERROR_E;
	}
	switch(pAcgCmd->Command) 
	{
		case ACG_DAA_GET_TELEPHONY_EVENT_CMD:

			pGetEventInfo = (acgTGetEventInfo *)pAcgCmd->data;	
			PollingMethod = pGetEventInfo->PollingMethod;
			
			TelephonyEvent.Channel = 0;
			TelephonyEvent.EventType = ACG_EVENT_TYPE__NO_EVENT;

			if(PollingMethod == ACG_POLLING_METHOD__BLOCKING)
			{	
				wait_event_interruptible(ring_state_wait,  ring_state_condition > 0);
				ring_state_condition --;
			}

			for(i = 0; ((i < NUM_DAA_LINES) && (!isEventFound)); i++)
			{			     
				TelephonyEvent.Channel =  DAA_TO_CH(i);   
				spin_lock(&ring_state_lock);

				if(TRUE == TelephonyEventBuffer[i].fxoRingEnd)
				{
					TelephonyEventBuffer[i].fxoRingEnd = FALSE;			
					TelephonyEvent.EventType = ACG_EVENT_TYPE__RING_END;
					isEventFound = 1;
				}
                            
				if(TRUE == TelephonyEventBuffer[i].fxoRingStart)
				{
					TelephonyEventBuffer[i].fxoRingStart = FALSE;			
					TelephonyEvent.EventType = ACG_EVENT_TYPE__RING_START;
					isEventFound = 1;
				}				
				spin_unlock(&ring_state_lock);
			}
            
			if(copy_to_user(pGetEventInfo->Buff, &TelephonyEvent, sizeof(acgTEventInfo))) 
			{
				ret = -EFAULT; 
			}
			else
			{
				ret = SILAB_OK_E;
			}
			break;
		default:
		     ret = SILAB_ERROR_E;
		     break;
	}
	return ret;
}
 
static int daa_write( struct file * file, const char * buf, size_t count, loff_t *ppos)
{
       int ret = SILAB_OK_E;
       int  Channel;
       acgTCommand			*pAcgCmd=NULL;
       int HookState;
       int device;
	/* cast the pointer */
	pAcgCmd = (acgTCommand *)buf;

        if ((!gs3050InitComplete)&&(pAcgCmd->Command!=ACG_DAA_INIT_CMD) )
        {
        	printk("Silabs drv: DAA is not initialized yet, return error\n");
        	return SILAB_ERROR_E;
        } 
        
        
       down_interruptible(&semToReadWriteFromMPI); 
       mpi_polling_disable = 1;
#if defined(CONFIG_TULIP2_21) || defined(CONFIG_TULIP2_40)
	PAL_sysGpioOutBit(44, 0);
#endif
	switch(pAcgCmd->Command) {  
              case ACG_DAA_INIT_CMD:
			{
                        	// DBG_PRINT("daa_write: line=%d, got DAAIOC_INIT_DAA \r\n", __LINE__);
                        	if(gs3050InitComplete)
                            {
                                    printk("Silabs SLAC is already initialized\n");
                                    break;
                            }
                        	init_fxo_status();
                            if(AnalogIFInitDAA())
                            {
                                gs3050InitComplete = 1;  
                                add_timer(&daa_timer);   
                            }   
                            else
                            {
                                gs3050InitComplete = 0;                                
                             }   
			}
	            break;  
	        case ACG_DAA_GENERATE_FLASH_CMD:
			{
                             Channel = pAcgCmd->Channel;
                             device = CH_TO_DAA(Channel);
                             if(IsValidDaaPort(device))
                                AnalogIFDaaSetFlash(device);
                             else
                                ret = SILAB_ERROR_E;
			}
		    break;	    
	        case ACG_DAA_SEIZE_LINE_CMD:
                    {	
                           	Channel = pAcgCmd->Channel;
                            HookState = pAcgCmd->Command;
                            device = CH_TO_DAA(Channel);
                            // DBG_PRINT("mtChangeStateToOffHook change channel state to off hook (%d) .\n", Channel);    
                              if(IsValidDaaPort(device)){
                                AnalogIFDaaSeizeLine(device);
				RingState[0] = 0;
				RingDetectFlag = 0;
			      }
			      else
                                ret = SILAB_ERROR_E;
			}
                    break;
                case ACG_DAA_RELEASE_LINE_CMD:
			{	
				Channel = pAcgCmd->Channel;
                            HookState = pAcgCmd->Command;
                            device = CH_TO_DAA(Channel);                                                       
        		        // DBG_PRINT("mtChangeStateToOnHook change ch annel state to on hook (%d) .\n", Channel);    
                              if(IsValidDaaPort(device)){
                                AnalogIFDaaReleaseLine(device);       
				RingState[0] = 0;
				RingDetectFlag = 1;
			      }
                              else
                                 ret = SILAB_ERROR_E;
			}
	              break;
		    default:
			ret = SILAB_ERROR_E;
			break;

	}
       mpi_polling_disable = 0;
       up(&semToReadWriteFromMPI); 			
	return ret;
}
static int daa_open( struct inode * inode, struct file * file )
{
    printk("\nsilabs daa opened\n\n");
    return 0;
}

static int daa_close( struct inode * inode, struct file * file )
{
    return 0;
}

struct file_operations daa_fops = {
	owner:		THIS_MODULE,
	read:		daa_read,
	write:		daa_write,
    unlocked_ioctl:    daa_ioctl,
    open:     daa_open,
    release:  daa_close
};

/* this is the timer function */ 
static void daa_timerFunction( unsigned long dummy )
{	
       int i;
	unsigned char CurrRingStatus;
        
        
       daa_timer.expires += (HZ/100);
   	add_timer( &daa_timer );	
	
        if(gs3050InitComplete==0) return;
        if(mpi_polling_disable) return;
	if (RingDetectFlag == 0) return;
            
        for (i=0; i<NUM_DAA_LINES; i++)
        {
                if(!IsValidDaaPort(i))continue;

           
                  CurrRingStatus = AnalogIFRingStatus(i);

		//printk("ringstatus %d\n", CurrRingStatus);
                   switch(RingState[i])
                  {
                  case 0:
                        if(CurrRingStatus)
                                ConsecutiveRingDetections[i]++;
                        else
                                ConsecutiveRingDetections[i] = 0;
                        
                        if(ConsecutiveRingDetections[i]>10)
                        {
                               RingState[i]=1;
                               TelephonyEventBuffer[i].fxoRingStart = TRUE;
                               ConsecutiveRingDetections[i] = 0;                               
                        }
                        break;
                  case 1:
                        if(!CurrRingStatus)
                                ConsecutiveRingDetections[i]++;
                        else{
                                ConsecutiveRingDetections[i] = 0;
			}

                         if(ConsecutiveRingDetections[i]>550)
                        {
                               TelephonyEventBuffer[i].fxoRingEnd = TRUE;
                               RingState[i]=0;
                               ConsecutiveRingDetections[i] = 0;                             
                        }
                         break;
                    
                  }

                    spin_lock(&ring_state_lock) ; 
                  if((TelephonyEventBuffer[i].fxoRingStart == TRUE)||(TelephonyEventBuffer[i].fxoRingEnd == TRUE))
                 {
                         ring_state_condition++;
                         wake_up_interruptible(&ring_state_wait);  
                  }   
                    spin_unlock(&ring_state_lock) ;
       }

        return;
      
}

static void init_fxo_status()
{
    int i;
    gs3050InitComplete = 0;
	RingDetectFlag = 1;
   for(i=0; i<NUM_DAA_LINES; i++)
    {
         ConsecutiveRingDetections[i]=0;
         TelephonyEventBuffer[i].polarityReversal = FALSE;
         TelephonyEventBuffer[i].fxoRingStart = FALSE;
         TelephonyEventBuffer[i].fxoRingEnd = FALSE;	            
         RingState[i]=0;
   }
}
int __init silabs_daa_init(void)
{
       int ret;
       volatile unsigned int regValue;
       printk("DAA Driver init_module !!! \n");

       ring_state_condition = 0;
#if 1
	ret = register_chrdev( DAA_MAJOR, DAA_NAME, &daa_fops );
	if(ret) goto init_err;

	printk("register rc=%d\n", ret);
#endif
       sema_init(&semToReadWriteFromMPI,1);
	init_waitqueue_head(&ring_state_wait);
	spin_lock_init(&ring_state_lock);

	init_timer ( &daa_timer );

	daa_timer.expires = jiffies + (HZ/100);
	daa_timer.function = daa_timerFunction;

     
#if defined(CONFIG_TULIP) || defined(CONFIG_SMB)
         /*pin system reset*/
        /*PINSEL: 13#31-30:GPIO47, 13#29-28:GPIO46, 13#27-26:GPIO45, 13#25-24:GPIO44*/
        *(volatile unsigned int*)(KSEG1ADDR(0x0861163C)) |= 0x0F000000;//I think it should be 0xFF000000;
        mb();
        /*reset IO8~IO15*/
        *(volatile unsigned int*)(KSEG1ADDR(0x08611624)) |= 0xFCFFC000;
        mb();
        /*reset IO16*/ /*PINSEL: 8#01-00:IO16 General LED*/
        *(volatile unsigned int*)(KSEG1ADDR(0x08611628)) |= 0x00000003;
        mb();
        /*IO direction*/
        *(volatile unsigned int*)(KSEG1ADDR(0x08610914)) &= 0xF0FFF;
        mb();
        /*IO direction (IO8~IO16)*/
        *(volatile unsigned int*)(KSEG1ADDR(0x08610910)) &= 0xFFFE00FF;
        mb();
        /*GPIO ENABLE*/
        *(volatile unsigned int*)(KSEG1ADDR(0x0861091C)) |= 0xF000;
        mb();
        /*GPIO ENABLE (IO8~IO16)*/
        *(volatile unsigned int*)(KSEG1ADDR(0x08610918)) |= 0x0001FF00;
        mb();
        /*Data Output*/
        *(volatile unsigned int*)(KSEG1ADDR(0x0861090C)) |= 0xF000;
        mb();
        /*Data Output (IO8~15)*/
        *(volatile unsigned int*)(KSEG1ADDR(0x08610908)) |= 0x0000FF00;
        mb();
#elif defined(CONFIG_ORCHID) || defined(CONFIG_ORCHID_ATA)
#warning "It is a SILABS_FOR_ORCHID"
    /*pin system reset*/
    printk("AC494E--MPI(CS#1=GPIO43;CS#2=GPIO46;CS#3=GPIO45;CS#4=GPIO48)\n");

    /*PINSEL: 15#25-24:GPIO43, 15#11-10:GPIO46, 15#7-6:GPIO45, 15#13-12:GPIO48*/
    regValue = *(volatile unsigned int*)(KSEG1ADDR(0x08611644));
    printk("0x08611644=0X%X\n", regValue);
	regValue &= ~0x3003CC0;
	regValue |= 0x02001440;	
	*(volatile unsigned int*)(KSEG1ADDR(0x08611644)) |= regValue;
    printk("0x08611644=0X%X\n", regValue);
	mb();
	//printk("GPIO Select\n");

	/*IO direction*/
	*(volatile unsigned int*)(KSEG1ADDR(0x08610914)) &= 0xE97FF;
	mb();
	//printk("IO direction\n");

	/*GPIO ENABLE*/
	*(volatile unsigned int*)(KSEG1ADDR(0x0861091C)) |= 0x16800;
	mb();
	//printk("GPIO ENABLE\n");

	/*Data Output*/
	*(volatile unsigned int*)(KSEG1ADDR(0x0861090C)) |= 0x16800;
	mb();
	//printk("Data Output\n");
#endif
	//DAAReset();
	return 0;

init_err:
	return -ENOMEM;
}

void silabs_daa_exit(void)
{
      unregister_chrdev(DAA_MAJOR, DAA_NAME);
      del_timer(&daa_timer);
     return;
}



module_init(silabs_daa_init);
module_exit(silabs_daa_exit);

EXPORT_SYMBOL(semToReadWriteFromMPI);
EXPORT_SYMBOL(mpi_polling_disable);
