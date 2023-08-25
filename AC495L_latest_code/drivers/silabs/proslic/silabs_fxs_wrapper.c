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

 /*
** System timer interface structure 
*/
typedef struct{
	U32 ticksPerSecond;
} systemTimer_S;

/*
** System time stamp
*/
typedef struct{
	U32 time;
} timeStamp;

typedef struct chanStatus_t chanStatus; //forward declaration 
typedef void (*procState) (chanStatus *pState, ProslicInt eInput);

/*
** structure to hold state information for pbx demo
*/
struct chanStatus_t { 
	proslicChanType *ProObj;
	timeStamp onHookTime;
	timeStamp offHookTime;
	procState currentState;
	uInt16 digitCount;
	uInt8 digits[20];
	uInt8 ringCount;
	uInt16 connectionWith;
	uInt16 powerAlarmCount;
	pulseDialType pulseDialData;
	BOOLEAN eventEnable;
}; 

/* Globals */
int old_jeffies = 0;
ctrl_S spiGciObj[NUM_SLIC_LINES]; //spi interface object
systemTimer_S timerObj[NUM_SLIC_LINES];    //timer object
controlInterfaceType *ProHWIntf[NUM_SLIC_LINES]; //hw interface
ProslicDeviceType *ProSLICDevices[NUM_SLIC_LINES]; //proslic device object
proslicChanType_ptr arrayOfProslicChans[NUM_SLIC_LINES]; //used for initialization only
chanStatus ports[NUM_SLIC_LINES];  //declare channel state structures
unsigned char ProSlicStates[NUM_SLIC_LINES];  

#ifdef FXS_3215_DRV_DEBUG
#define DBG_PRINT(a1,a2...)	printk("[%s:%d] " a1, __FUNCTION__, __LINE__, ##a2)
#else
#define DBG_PRINT(a1,a2...)
#endif

extern uInt8 fxs_read_reg (uInt16 portID, uInt8 channel, uInt8 regAddr);
extern void  fxs_write_reg (uInt16 portID, uInt8 channel, uInt8 regAddr, uInt8 data);
 

static int Si3215orSi3216(uInt16 portID,uInt8 chipNumber)
{
static int slicPreviouslyIdentified = 0;
static int isSi3215 = 0;

   if(!slicPreviouslyIdentified)
   {
	   isSi3215 = ((fxs_read_reg(portID,chipNumber, 1) & 0x80) == 0x80);
	   slicPreviouslyIdentified = 1;
   }
	return (isSi3215);
}


static uInt8 possibleAddressCorrect(uInt16 portID, uInt8 chipNumber, uInt8 address)
{
if (!Si3215orSi3216(portID,chipNumber)) 
		return (address);
		
	if ((address > 12) && (address < 41))
		return (address - 13);
	if ((address == 41) || (address == 43))
		return (address + 23);
	if ((address > 87)&&(address < 99))
		return (address - 13); 
	if ((address > 98) && (address < 105))
		return (address - 30);
	if (address >= 208)
		return address;

	return 0xFF;
	
}

static void RAMwait (uInt16 portID,unsigned short channel)
{
	unsigned char regVal; 
	regVal = fxs_read_reg (portID,channel,31);
	while (regVal)
	{   
	      	regVal = fxs_read_reg (portID,channel,31);             
	}//wait for indirect registers

}

static void WriteRam (uInt16 portID, uInt8 channel, uInt16 regAddr, uInt32 data){
	regAddr = possibleAddressCorrect(portID,channel,regAddr);
	if (regAddr == 0xff)
		return;
	RAMwait(portID,channel);
	fxs_write_reg(portID,channel,28,data & 0xFF);
	fxs_write_reg(portID,channel,29,(data & 0xFF00)>>8);
	fxs_write_reg(portID,channel,30,regAddr);
}

static uInt32 ReadRam (uInt16 portID, uInt8 channel, uInt16 regAddr){
	uInt16 RegVal;
    regAddr = possibleAddressCorrect(portID,channel,regAddr);
	RAMwait(portID,channel);
	fxs_write_reg(portID,channel,30,regAddr);
	RAMwait(portID,channel);
	RegVal = fxs_read_reg(portID,channel,28) | ( fxs_read_reg(portID,channel,29)  << 8 );
	return RegVal;
}

/*
** SPI/GCI register read 
**
** Description: 
** Reads a single ProSLIC register
**
** Input Parameters: 
** channel: ProSLIC channel to read from
** num: number of reads to perform
** regAddr: Address of register to read
** addr_inc: whether to increment address after each read
** data: data to read from register
**
** Return:
** none
*/
uInt8 ctrl_ReadRegisterWrapper (void *hSpiGci, uInt8 channel, uInt8 regAddr){

    ctrl_S *hspiGci=(ctrl_S *)hSpiGci;
	return fxs_read_reg(hspiGci->portID,channel,regAddr);
}


/*
** Function: spiGci_WriteRegisterWrapper 
**
** Description: 
** Writes a single ProSLIC register
**
** Input Parameters: 
** channel: ProSLIC channel to write to
** address: Address of register to write
** data: data to write to register
**
** Return:
** none
*/
int ctrl_WriteRegisterWrapper (void *hSpiGci, uInt8 channel, uInt8 regAddr, uInt8 data){
	
    ctrl_S *hspiGci=(ctrl_S *)hSpiGci;
    fxs_write_reg(hspiGci->portID,channel,regAddr, data);
	return 0;
}


/*
** Function: SPI_ReadRAMWrapper
**
** Description: 
** Reads a single ProSLIC RAM location
**
** Input Parameters: 
** channel: ProSLIC channel to read from
** address: Address of RAM location to read
** pData: data to read from RAM location
**
** Return:
** none
*/
ramData ctrl_ReadRAMWrapper (void *hSpiGci, uInt8 channel, uInt16 ramAddr){
	
    ctrl_S *hspiGci=(ctrl_S *)hSpiGci;
	return ReadRam(hspiGci->portID,channel,ramAddr);
}


/*
** Function: SPI_WriteRAMWrapper
**
** Description: 
** Writes a single ProSLIC RAM location
**
** Input Parameters: 
** channel: ProSLIC channel to write to
** address: Address of RAM location to write
** data: data to write to RAM location
**
** Return:
** none
*/
int ctrl_WriteRAMWrapper (void *hSpiGci, uInt8 channel, uInt16 ramAddr, ramData data){
	
    ctrl_S *hspiGci=(ctrl_S *)hSpiGci;
    WriteRam(hspiGci->portID,channel,ramAddr,data);
	return 0;
}


/*
** Function: SYSTEM_TimerInit
*/
void TimerInit (systemTimer_S *pTimerObj){
//	pTimerObj->ticksPerSecond=kos_tick_rate_get();
	pTimerObj->ticksPerSecond=TICKS_PER_SECOND;
    printk("TimerInit: pTimerObj->ticksPerSecond=0x%X .\n", (int)pTimerObj->ticksPerSecond);
}

/*
** Function: SYSTEM_Delay
*/
int time_DelayWrapper (void *hTimer, int timeInMs){
	mdelay(timeInMs);
    return 0;
}

/*
** Function: SYSTEM_TimeElapsed
*/
int time_TimeElapsedWrapper (void *hTimer, void *startTime, int *timeInMs){
    
    printk("=============================================================\n");
    printk("time_TimeElapsedWrapper: CURRENTLY NOT IMPLEMENTED ! ! ! ! ! ! \n");
    printk("=============================================================\n");
    return 0;
}

/*
** Function: SYSTEM_GetTime
*/
int time_GetTimeWrapper (void *hTimer, void *time){
	timeStamp * timestamp=(timeStamp *)time;
//    timestamp->time = kos_ticks_get();
	timestamp->time = jiffies - old_jeffies;
	old_jeffies = jiffies;
      return 0;
}


/*
** Function: spiGci_ResetWrapper
**
** Description: 
** Sets the reset pin of the ProSLIC
*/
int ctrl_ResetWrapper (void *hSpiGci, int status){
	
    //NIRP: if called from the driver - it may reset the FXO which is connected to the same reset line 
    printk("=============================================================\n");
    printk("ctrl_ResetWrapper: CURRENTLY NOT IMPLEMENTED  ! ! ! ! ! ! ! \n");
    printk("=============================================================\n");
    return 0;
}

/*
** Function: printChipDetails
**
** Description: 
** prints details of EVB
**
** Input Parameters: 
** pProslic: pointer to proslic data structure 
**
** Return:
** none
*/
static void printChipDetails(proslicChanType *pProslic)
{
	char* name = "";

	uInt8 data;
//	char* freqs[ ] = {"8192","4028","2048","1024","512","256","1536","768","32768"};

	//--------------------- NAME -----------------------------
	data = pProslic->deviceId->chipType;
	switch(data){
		case SI3210:
			printk("Si3210");
			break;
		case SI3215:
			printk("Si3215-16");
			break;
		case SI3211:
			printk("Si3211");
			break;
		case SI3212:
			printk("Si3212");
			break;
		case SI3210M:
			printk("Si3210 M");
			break;
		case SI3215M:
			printk("Si3215 M");
			break;
		case SI3216M:
			printk("Si3216 M");
			break;
	}
	//-------------------END OF NAME -------------------------

	//---------------------FREQUENCY--------------------------
//	ReadReg(pProHW, pProslic->channel, 1, 13, 0, &data);  /* Read the frequency */
//	data = data>>4;
	//-----------------END OF FREQUENCY-----------------------

	printk("%s Rev %c\n",name, pProslic->deviceId->chipRev);
	//printk("(%s KHz)\n", freqs[data]);
}

int fxs_initialize(void)
{
	int ret=0;
	uInt8 i;
	proslicChanType *pSlic;

	for (i=0;i<NUM_SLIC_LINES;i++)
	{
		spiGciObj[i].portID=i;//NIRP
		//initialize timer
		TimerInit(&timerObj[i]);  
		ProSLIC_createControlInterface(&ProHWIntf[i]);
		ProSLIC_createDevice (&(ProSLICDevices[i]));

		ProSLIC_createChannel(&(ports[i].ProObj));
#if defined(CONFIG_TULIP2_21) || defined(CONFIG_TULIP2_40)
		ProSLIC_SWInitChan (ports[i].ProObj, i, SI321X_TYPE, ProSLICDevices[i], ProHWIntf[i]);
#else
		ProSLIC_SWInitChan (ports[i].ProObj, 0, SI321X_TYPE, ProSLICDevices[i], ProHWIntf[i]);
#endif

	    ProSLIC_setSWDebugMode (ports[i].ProObj, 1);//NIRP

#if defined(CONFIG_TULIP2_21) || defined(CONFIG_TULIP2_40)
	    ProSLIC_setChannelEnable (ports[i].ProObj, 1);//NIRP
#else
		ProSLIC_setChannelEnable (ports[i].ProObj, 0);//NIRP
#endif

		ProSLIC_setControlInterfaceCtrlObj (ProHWIntf[i], &spiGciObj[i]);
		ProSLIC_setControlInterfaceReset (ProHWIntf[i], ctrl_ResetWrapper);
		ProSLIC_setControlInterfaceWriteRegister (ProHWIntf[i], ctrl_WriteRegisterWrapper);
		ProSLIC_setControlInterfaceReadRegister (ProHWIntf[i], ctrl_ReadRegisterWrapper);		
		ProSLIC_setControlInterfaceWriteRAM (ProHWIntf[i], ctrl_WriteRAMWrapper);
		ProSLIC_setControlInterfaceReadRAM (ProHWIntf[i], ctrl_ReadRAMWrapper);
		ProSLIC_setControlInterfaceTimerObj (ProHWIntf[i], &timerObj[i]);
		ProSLIC_setControlInterfaceDelay (ProHWIntf[i], time_DelayWrapper);
		ProSLIC_setControlInterfaceTimeElapsed (ProHWIntf[i], time_TimeElapsedWrapper);
		ProSLIC_setControlInterfaceGetTime (ProHWIntf[i], time_GetTimeWrapper);

		DBG_PRINT("-- \n");
		//ProSLIC_Reset((ports[0].ProObj));	//Reset the ProSLIC(s) before we begin

		//Initialize the channel state for each channel
 		arrayOfProslicChans[i] = (ports[i].ProObj); //create array of channel pointers (for broadcast init)

		if(0!=ProSLIC_Init(&arrayOfProslicChans[i], 1))
		{
			printk("ProSLIC board%d init falied\n", i);
			ProSlicStates[i] = FALSE;                    
		}
		else
		{
			printk("ProSLIC board%d init ok\n", i);
			ProSlicStates[i] = TRUE;
		}

	//NIRP 070731 PCM timeslot configuration - different for each channel
#if defined(CONFIG_TULIP2_21) || defined(CONFIG_TULIP2_40)
		switch(i) 
		{
			case 0:
				ProSLIC_PCMTimeSlotSetup (ports[i].ProObj, 0x0001, 0x0001);//NIRP 070731 - PCM time slot configuration - DSP channel 0
				break;
			case 1:
				if ((NUM_SLIC_LINES == 2) && (NUM_DAA_LINES == 0))
					ProSLIC_PCMTimeSlotSetup (ports[i].ProObj, 0x0011, 0x0011);//NIRP 070731 - PCM time slot configuration - DSP channel 1
				else
					ProSLIC_PCMTimeSlotSetup (ports[i].ProObj, 0x0009, 0x0009);
				break;
			case 2:
				ProSLIC_PCMTimeSlotSetup (ports[i].ProObj, 0x0011, 0x0011);//NIRP 070731 - PCM time slot configuration - DSP channel 2
				break;
			case 3:
				ProSLIC_PCMTimeSlotSetup (ports[i].ProObj, 0x0019, 0x0019);//NIRP 070731 - PCM time slot configuration - DSP channel 3
				break;
			default:
				printk("Need to configure PCM timeslot configuration for FXS channel %d \n", i);
		}
#else
		switch(i) 
		{
			case 0:
				ProSLIC_PCMTimeSlotSetup (ports[i].ProObj, 0x0001, 0x0001);//NIRP 070731 - PCM time slot configuration - DSP channel 0
				break;
			case 1:
				ProSLIC_PCMTimeSlotSetup (ports[i].ProObj, 0x0011, 0x0011);//NIRP 070731 - PCM time slot configuration - DSP channel 1
				break;
			case 2:
				ProSLIC_PCMTimeSlotSetup (ports[i].ProObj, 0x0021, 0x0021);//NIRP 070731 - PCM time slot configuration - DSP channel 2
				break;
			case 3:
				ProSLIC_PCMTimeSlotSetup (ports[i].ProObj, 0x0031, 0x0031);//NIRP 070731 - PCM time slot configuration - DSP channel 3
				break;
			default:
				printk("Need to configure PCM timeslot configuration for FXS channel %d \n", i);
		}
#endif

		ProSLIC_PCMSetup (ports[i].ProObj,0);//NIRP 070415: Set Default PCM
		ProSLIC_TXAudioGainSetup (ports[i].ProObj,0);//NIRP 070606 adjust the audio gain minus 3.5 db
		ProSLIC_RXAudioGainSetup (ports[i].ProObj,0);//NIRP 070606 adjust the audio gain minus 3.5 db
		ProSLIC_ZsynthSetup(ports[i].ProObj, 1);//NIRP 070628
		pSlic = (ports[i].ProObj);
		printChipDetails(pSlic);
	}
	
	DBG_PRINT("-- \n");    	
	return ret;
}

int fxs_PCMStart(int SlicNo)
{
     proslicChanType_ptr proslicChanType_ptr;
     proslicChanType_ptr = arrayOfProslicChans[(SlicNo)];   
     return ProSLIC_PCMStart(proslicChanType_ptr);
}
int fxs_PCMStop(int SlicNo)
{
     proslicChanType_ptr proslicChanType_ptr;
     proslicChanType_ptr = arrayOfProslicChans[SlicNo];   
     return ProSLIC_PCMStop(proslicChanType_ptr);
}


 int fxs_RingStart(int SlicNo)
{
     proslicChanType_ptr proslicChanType_ptr;
     proslicChanType_ptr = arrayOfProslicChans[(SlicNo)];   
     return ProSLIC_RingStart(proslicChanType_ptr);
}

 int fxs_RingStop(int SlicNo)
{
     proslicChanType_ptr proslicChanType_ptr;
     proslicChanType_ptr = arrayOfProslicChans[(SlicNo)];   
     return ProSLIC_RingStop(proslicChanType_ptr);
}

int  fxs_ReadHookStatus(int SlicNo,  unsigned char *p_hookStat)
{
     proslicChanType_ptr proslicChanType_ptr;
     proslicChanType_ptr = arrayOfProslicChans[SlicNo];   
     return ProSLIC_ReadHookStatus(proslicChanType_ptr, p_hookStat);
}

int IsValidSlic(int SlicNo)
{
       if((SlicNo>=0)&&(SlicNo<NUM_SLIC_LINES)&&(ProSlicStates[SlicNo]==TRUE)) 
           return 1;
    else
          return 0;
}
