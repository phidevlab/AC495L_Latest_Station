/*
** Copyright © 2007 by Silicon Laboratories
**
** $Id: proslic.h,v 1.17 2007/02/26 16:46:05 lajordan Exp $
**
** Proslic.h
**
** Author(s): 
** laj
**
** Distributed by: 
** Silicon Laboratories, Inc
**
** This file contains proprietary information.	 
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
** This is the header file for the ProSLIC driver.
**
** Dependancies:
** proslic_datatypes.h
**
*/
#ifndef PROSLIC_H
#define PROSLIC_H

//include all the required headers
#ifdef SI321X
#include "Si321x_Constants.h"
#endif
#include "proslic_datatypes.h"
#include "proslic_ctrl.h"
#include "proslic_timer_intf.h"

/*
** ----------------ProSLIC Generic DataTypes/Function Definitions----
**********************************************************************
*/




#define LOGPRINT printk

#define SI321X_TYPE 0
#define SI324X_TYPE 1
#define SI3220_TYPE 2

#define MAX_PROSLIC_IRQS 32

#define ON_HOOK_TIMEOUT 0xff
/*
** These are the error codes for ProSLIC failures
*/
typedef enum {
	NONE,
	POWERALARMQ1,
	POWERALARMQ2,
	POWERALARMQ3,
	POWERALARMQ4,
	POWERALARMQ5,
	POWERALARMQ6,
	SPIFAIL,
	POWERLEAK,
	TIMEOUTPOWERUP,
	FOREIGNSOURCE,
	TIMEOUTPOWERDOWN,
	TIPORRINGGROUNDSHORT,
	CM_CAL_ERR,
	RING_FAIL_INT,
	CAL_TIMEOUT,
	PATCH_ERR
} errorCode;

/*
** This is the main ProSLIC control interface object. do not access directly!
*/
typedef struct	{
		void *hCtrl;
		ctrl_Reset_fptr Reset_fptr;
		ctrl_WriteRegister_fptr WriteRegister_fptr;
		ctrl_ReadRegister_fptr ReadRegister_fptr;
		ctrl_WriteRAM_fptr WriteRAM_fptr;
		ctrl_ReadRAM_fptr ReadRAM_fptr;
		void *hTimer;
		system_delay_fptr Delay_fptr;
		system_timeElapsed_fptr timeElapsed_fptr;
		system_getTime_fptr getTime_fptr;
} controlInterfaceType;

/*
** This is the main ProSLIC device object
*/
typedef struct	{
		controlInterfaceType *ctrlInterface;
		int chipRev;
		int chipType;
} ProslicDeviceType;


/*
** This is the main ProSLIC interface object pointer
*/
typedef ProslicDeviceType *proslicDeviceType_ptr;

/*
** This is the main ProSLIC channel object
*/
typedef struct	{
		proslicDeviceType_ptr deviceId;
		uInt8 channel;
		uInt8 channelType;
		int error;
		int debugMode;
		int channelEnable;
} proslicChanType;


/*
** This is the main ProSLIC channel object pointer
*/
typedef proslicChanType *proslicChanType_ptr;

/*
** This is structure used to store pulse dial information
*/
typedef struct {
	uInt8 currentPulseDigit;
	void *onHookTime;
	void *offHookTime;
} pulseDialType;

/*
** Defines structure for configuring pulse dial detection
*/
typedef struct {
	uInt8 minOnHook;
	uInt8 maxOnHook;
	uInt8 minOffHook;
	uInt8 maxOffHook;
} pulseDial_Cfg;


typedef enum {
OSC1_T1,
OSC1_T2,
OSC2_T1,
OSC2_T2,
RING_T1,
RING_T2,
PM_T1,
PM_T2,
FSKBUF_AVAIL,
VBAT,
RING_TRIP,
LOOP_STATUS,
LONG_STAT,
VOC_TRACK,
DTMF,
INDIRECT,
TXMDM,
RXMDM,
PQ1,
PQ2,
PQ3,
PQ4,
PQ5,
PQ6,
RING_FAIL,
CM_BAL,
USER_IRQ0,
USER_IRQ1,
USER_IRQ2,
USER_IRQ3,
USER_IRQ4,
USER_IRQ5,
USER_IRQ6,
USER_IRQ7
}ProslicInt;

/*
** Defines structure of interrupt data
*/
typedef struct {
	ProslicInt *irqs;
	uInt8 number;
} proslicIntType;

/*
** Defines structure for configuring 1 oscillator
*/
typedef struct {
	ramData freq;
	ramData amp;
	ramData phas;
	uInt8 talo;
	uInt8 tahi;
	uInt8 tilo;
	uInt8 tihi;
} Oscillator_Cfg;


// Hook states
enum {
ONHOOK,
OFFHOOK
};

/*
** enumeration of the Proslic polarity reversal states
*/
enum {
	POLREV_STOP,
	POLREV_START,
	WINK_START,
	WINK_STOP
};

/*
** Defines initialization data structures
*/
typedef struct {
	uInt8 address;
	uInt8 initValue;
} ProslicRegInit;

typedef struct {
	uInt16 address;
	ramData initValue;
} ProslicRAMInit;


/*
** This is the ProSLIC patch object
*/
typedef struct {
	ramData *patchData; //1024 max
	uInt16 *patchEntries; // 8 max
} proslicPatch;

// Linefeed states
enum {
LF_OPEN,
LF_FWD_ACTIVE,
LF_FWD_OHT,
LF_TIP_OPEN,
LF_RINGING,
LF_REV_ACTIVE,
LF_REV_OHT,
LF_RING_OPEN
} ;


typedef enum {
	SI3210, 
	SI3215,
	SI3211, 	
	SI3212,
 	SI3210M,
 	SI3215M,
 	SI3216M,
	SI3240,
	SI3241,
	SI3242,
	SI3243,
	SI3245,
	SI3244,
	SI3246,
	SI3247,
	SI3220,
	SI3225,
	SI3226,
	SI3227
}partNumber;



/*
** Chip revision definition for easier readability in the source code
*/
typedef enum {
	A,
	B,
	C,
	D,
	E,
	F,
	G
}Revision ;





int ProSLIC_createControlInterface (controlInterfaceType **pCtrlIntf);
int ProSLIC_createDevice (ProslicDeviceType **pDevice);
int ProSLIC_createChannel (proslicChanType_ptr *hProslic);
int ProSLIC_destroyChannel (proslicChanType_ptr *hProslic);
int ProSLIC_destroyDevice (ProslicDeviceType **pDevice);
int ProSLIC_destroyControlInterface (controlInterfaceType **pCtrlIntf);
int ProSLIC_setControlInterfaceCtrlObj (controlInterfaceType *pCtrlIntf, void *hCtrl);
int ProSLIC_setControlInterfaceReset (controlInterfaceType *pCtrlIntf, ctrl_Reset_fptr Reset_fptr);
int ProSLIC_setControlInterfaceWriteRegister (controlInterfaceType *pCtrlIntf, ctrl_WriteRegister_fptr WriteRegister_fptr);
int ProSLIC_setControlInterfaceReadRegister (controlInterfaceType *pCtrlIntf, ctrl_ReadRegister_fptr ReadRegister_fptr);
int ProSLIC_setControlInterfaceWriteRAM (controlInterfaceType *pCtrlIntf, ctrl_WriteRAM_fptr WriteRAM_fptr);
int ProSLIC_setControlInterfaceReadRAM (controlInterfaceType *pCtrlIntf, ctrl_ReadRAM_fptr ReadRAM_fptr);
int ProSLIC_setControlInterfaceTimerObj (controlInterfaceType *pCtrlIntf, void *hTimer);
int ProSLIC_setControlInterfaceDelay (controlInterfaceType *pCtrlIntf, system_delay_fptr Delay_fptr);
int ProSLIC_setControlInterfaceTimeElapsed (controlInterfaceType *pCtrlIntf, system_timeElapsed_fptr timeElapsed_fptr);
int ProSLIC_setControlInterfaceGetTime (controlInterfaceType *pCtrlIntf, system_getTime_fptr getTime_fptr);
int ProSLIC_SWInitChan (proslicChanType_ptr hProslic,int channel,int chipType, ProslicDeviceType*deviceObj,controlInterfaceType *pCtrlIntf);
int ProSLIC_setSWDebugMode (proslicChanType_ptr hProslic, int debugEn);
int ProSLIC_getErrorFlag (proslicChanType_ptr hProslic, int*errorCode);
int ProSLIC_clearErrorFlag (proslicChanType_ptr hProslic);
int ProSLIC_setChannelEnable (proslicChanType_ptr hProslic, int chanEn);
int ProSLIC_getChannelEnable (proslicChanType_ptr hProslic, int* chanEn);
int ProSLIC_InitializeDialPulseDetect(pulseDialType *pPulse,void *offHookTime,void *onHookTime);


int ProSLIC_Reset (proslicChanType_ptr hProslic);

int ProSLIC_Init (proslicChanType_ptr *hProslic, int size);

int ProSLIC_Cal (proslicChanType_ptr *hProslic, int size);

int ProSLIC_LoadRegTables (proslicChanType_ptr *hProslic,ProslicRAMInit *pRamTable, ProslicRegInit *pRegTable, int size);

int ProSLIC_LoadPatch (proslicChanType_ptr hProslic,proslicPatch *pPatch);

int ProSLIC_EnableInterrupts (proslicChanType_ptr hProslic);

int ProSLIC_RingSetup (proslicChanType_ptr hProslic,int preset);

int ProSLIC_ToneGenSetup (proslicChanType_ptr hProslic,int preset);

int ProSLIC_FSKSetup (proslicChanType_ptr hProslic,int preset);

int ProSLIC_DTMFDecodeSetup (proslicChanType_ptr hProslic,int preset);

int ProSLIC_ZsynthSetup (proslicChanType_ptr hProslic,int preset);

int ProSLIC_GciCISetup (proslicChanType_ptr hProslic,int preset);

int ProSLIC_ModemDetSetup (proslicChanType_ptr hProslic,int preset);

int ProSLIC_TXAudioGainSetup (proslicChanType_ptr hProslic,int preset);

int ProSLIC_RXAudioGainSetup (proslicChanType_ptr hProslic,int preset);

int ProSLIC_HybridSetup (proslicChanType_ptr hProslic,int preset);

int ProSLIC_AudioEQSetup (proslicChanType_ptr hProslic,int preset);

int ProSLIC_DCFeedSetup (proslicChanType_ptr hProslic,int preset);

int ProSLIC_GPIOSetup (proslicChanType_ptr hProslic,int preset);

int ProSLIC_PulseMeterSetup (proslicChanType_ptr hProslic,int preset);

int ProSLIC_PCMSetup (proslicChanType_ptr hProslic,int preset);

int ProSLIC_PCMTimeSlotSetup (proslicChanType_ptr hProslic,uInt16 rxcount, uInt16 txcount);

int ProSLIC_FindInterruptChannels (proslicChanType_ptr *hProslic,uInt8 *pChannelNumbers);

int ProSLIC_GetInterrupts (proslicChanType_ptr hProslic,proslicIntType *pIntData);

int ProSLIC_ReadHookStatus (proslicChanType_ptr hProslic,uInt8 *pHookStat);

int ProSLIC_SetLinefeedStatus (proslicChanType_ptr hProslic,uInt8 newLinefeed);

int ProSLIC_PolRev (proslicChanType_ptr hProslic,uInt8 abrupt,uInt8 newPolRevState);

int ProSLIC_GPIOControl (proslicChanType_ptr hProslic,uInt8 *pGpioData, uInt8 read);

int ProSLIC_MWI (proslicChanType_ptr hProslic,uInt8 lampOn);

int ProSLIC_ToneGenStart (proslicChanType_ptr hProslic,uInt8 timerEn);

int ProSLIC_ToneGenStop (proslicChanType_ptr hProslic);

int ProSLIC_RingStart (proslicChanType_ptr hProslic);

int ProSLIC_RingStop (proslicChanType_ptr hProslic);

int ProSLIC_EnableCID (proslicChanType_ptr hProslic);

int ProSLIC_DisableCID (proslicChanType_ptr hProslic);

int ProSLIC_SendCID (proslicChanType_ptr hProslic, uInt8 *buffer, uInt8 numBytes);

int ProSLIC_PCMStart (proslicChanType_ptr hProslic);

int ProSLIC_PCMStop (proslicChanType_ptr hProslic);

/*
** Function: PROSLIC_DialPulseDetect
**
** Description: 
** implements pulse dial detection and should be called at every hook transistion
** pulse dial threshold timeout (between digits) should be handled using timeout function
*/
int ProSLIC_DialPulseDetect (proslicChanType *pProslic, pulseDial_Cfg *pPulsedialCfg, pulseDialType *pPulseDialData);
int ProSLIC_DialPulseDetectTimeout (proslicChanType *pProslic, pulseDial_Cfg *pPulsedialCfg, pulseDialType *pPulseDialData);

int ProSLIC_DTMFReadDigit (proslicChanType_ptr hProslic,uInt8 *pDigit);

int ProSLIC_PLLFreeRunStart (proslicChanType_ptr hProslic);

int ProSLIC_PLLFreeRunStop (proslicChanType_ptr hProslic);

int ProSLIC_PulseMeterStart (proslicChanType_ptr hProslic);

int ProSLIC_PulseMeterStop (proslicChanType_ptr hProslic);



#endif //end ifdef PROSLIC_H

/*
** $Log: proslic.h,v $
** Revision 1.17  2007/02/26 16:46:05  lajordan
** cleaned up some warnings
**
** Revision 1.16  2007/02/17 00:00:00  lajordan
** no message
**
** Revision 1.15  2007/02/16 23:54:57  lajordan
** no message
**
** Revision 1.14  2007/02/15 23:33:27  lajordan
** no message
**
** Revision 1.13  2007/02/15 19:56:09  lajordan
** no message
**
** Revision 1.12  2007/02/14 21:56:25  lajordan
** no message
**
** Revision 1.11  2007/02/14 17:54:53  lajordan
** added comments
**
** Revision 1.10  2006/11/22 21:37:58  lajordan
** no message
**
** Revision 1.9  2006/07/21 18:16:06  lajordan
** added si3220 include
**
** Revision 1.8  2006/07/21 04:13:46  lajordan
** change calc timer units
**
** Revision 1.7  2006/07/21 03:45:15  lajordan
** added tonecalc, ringcalc
**
** Revision 1.6  2006/07/20 22:46:22  lajordan
** added tonecalc
**
** Revision 1.5  2006/07/20 22:29:44  lajordan
** added ring choices
**
** Revision 1.4  2006/07/20 22:28:31  lajordan
** added ring choices
**
** Revision 1.3  2006/07/14 18:38:26  lajordan
** adding comments
**
** Revision 1.2  2006/07/14 18:37:19  lajordan
** adding comments
**
*/
