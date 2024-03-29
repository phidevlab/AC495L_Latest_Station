/*
** Copyright � 2007 by Silicon Laboratories
**
** $Id: ProSLIC.c,v 1.6 2007/02/26 16:46:05 lajordan Exp $
**
** Proslic.c
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
** This is the interface file for the ProSLIC drivers.
**
** Dependancies:
** proslic_datatypes.h
**
*/ 

#include <linux/slab.h>


#include "proslic_datatypes.h"
#include "proslic_ctrl.h"
#include "proslic_timer_intf.h"

#define SI321X/* NIRP 070301 proslic integration */

#ifdef SI321X
#include "Si321x.h"
#include "Si321x_Intf.h"
#endif
#ifdef SI324X
#include "Si324x.h"
#include "si324x_intf.h"
#endif
#ifdef SI3220
#include "Si3220.h"
#include "si3220_intf.h"
#endif


#define TimeElapsed pProslic->deviceId->ctrlInterface->timeElapsed_fptr
#define getTime pProslic->deviceId->ctrlInterface->getTime_fptr
#define pProTimer	pProslic->deviceId->ctrlInterface->hTimer

int ProSLIC_createControlInterface (controlInterfaceType **pCtrlIntf){
	*pCtrlIntf = kmalloc(sizeof(controlInterfaceType), GFP_KERNEL);
	memset (*pCtrlIntf,0,sizeof(controlInterfaceType));
	return 0;
}
int ProSLIC_createDevice (ProslicDeviceType **pDevice){
	*pDevice = kmalloc (sizeof(ProslicDeviceType), GFP_KERNEL);
	memset(*pDevice,0,sizeof(ProslicDeviceType));
	return 0;
}
int ProSLIC_createChannel (proslicChanType_ptr *hProslic){
	*hProslic = kmalloc(sizeof(proslicChanType), GFP_KERNEL);
	memset(*hProslic,0,sizeof(proslicChanType));
	return 0;
}
int ProSLIC_destroyChannel (proslicChanType_ptr *hProslic){
	kfree ((proslicChanType_ptr)*hProslic);
	return 0;
}
int ProSLIC_destroyDevice (ProslicDeviceType **pDevice){
	kfree ((ProslicDeviceType*)*pDevice);
	return 0;
}
int ProSLIC_destroyControlInterface (controlInterfaceType **pCtrlIntf){
	kfree ((controlInterfaceType*)*pCtrlIntf);
	return 0;
}
int ProSLIC_setControlInterfaceCtrlObj (controlInterfaceType *pCtrlIntf, void *hCtrl){
	pCtrlIntf->hCtrl = hCtrl;
	return 0;
}
int ProSLIC_setControlInterfaceReset (controlInterfaceType *pCtrlIntf, ctrl_Reset_fptr Reset_fptr){
	pCtrlIntf->Reset_fptr = Reset_fptr;
	return 0;
}
int ProSLIC_setControlInterfaceWriteRegister (controlInterfaceType *pCtrlIntf, ctrl_WriteRegister_fptr WriteRegister_fptr){
	pCtrlIntf->WriteRegister_fptr = WriteRegister_fptr;
	return 0;
}
int ProSLIC_setControlInterfaceReadRegister (controlInterfaceType *pCtrlIntf, ctrl_ReadRegister_fptr ReadRegister_fptr){
	pCtrlIntf->ReadRegister_fptr = ReadRegister_fptr;
	return 0;
}
int ProSLIC_setControlInterfaceWriteRAM (controlInterfaceType *pCtrlIntf, ctrl_WriteRAM_fptr WriteRAM_fptr){
	pCtrlIntf->WriteRAM_fptr = WriteRAM_fptr;
	return 0;
}
int ProSLIC_setControlInterfaceReadRAM (controlInterfaceType *pCtrlIntf, ctrl_ReadRAM_fptr ReadRAM_fptr){
	pCtrlIntf->ReadRAM_fptr = ReadRAM_fptr; 
	return 0;
}
int ProSLIC_setControlInterfaceTimerObj (controlInterfaceType *pCtrlIntf, void *hTimer){
	pCtrlIntf->hTimer = hTimer;
	return 0;
}
int ProSLIC_setControlInterfaceDelay (controlInterfaceType *pCtrlIntf, system_delay_fptr Delay_fptr){
	pCtrlIntf->Delay_fptr = Delay_fptr;
	return 0;
}
int ProSLIC_setControlInterfaceTimeElapsed (controlInterfaceType *pCtrlIntf, system_timeElapsed_fptr timeElapsed_fptr){
	pCtrlIntf->timeElapsed_fptr = timeElapsed_fptr;
	return 0;
}
int ProSLIC_setControlInterfaceGetTime (controlInterfaceType *pCtrlIntf, system_getTime_fptr getTime_fptr){
	pCtrlIntf->getTime_fptr = getTime_fptr;
	return 0;
}


int ProSLIC_SWInitChan (proslicChanType_ptr hProslic,int channel,int chipType, ProslicDeviceType*pDeviceObj, controlInterfaceType *pCtrlIntf){
	hProslic->channel = channel;
	hProslic->deviceId = pDeviceObj;
	hProslic->deviceId->ctrlInterface = pCtrlIntf;
	hProslic->channelEnable=1;
	hProslic->error = NONE;
	switch (chipType){
		case SI321X_TYPE:
			hProslic->deviceId->chipType = SI3210;
			hProslic->channelType = SI3210;
			break;
		case SI324X_TYPE:
			hProslic->deviceId->chipType = SI3240;
			hProslic->channelType = SI3240;
			break;
		case SI3220_TYPE:
			hProslic->deviceId->chipType = SI3220;
			hProslic->channelType = SI3220;
			break;
	}
	return 0;
}
int ProSLIC_setSWDebugMode (proslicChanType_ptr hProslic, int debugEn){
	hProslic->debugMode = debugEn;
	return 0;
}
int ProSLIC_getErrorFlag (proslicChanType_ptr hProslic, int*errorCode){
	*errorCode = hProslic->error;
	return 0;
}
int ProSLIC_clearErrorFlag (proslicChanType_ptr hProslic){
	hProslic->error = NONE;
	return 0;
}
int ProSLIC_setChannelEnable (proslicChanType_ptr hProslic, int chanEn){
	hProslic->channelEnable = chanEn;
	return 0;
}
int ProSLIC_getChannelEnable (proslicChanType_ptr hProslic, int* chanEn){
	*chanEn = hProslic->channelEnable;
	return 0;
}
int ProSLIC_InitializeDialPulseDetect(pulseDialType *pPulse,void *offHookTime,void *onHookTime){
	pPulse->offHookTime = offHookTime;
	pPulse->onHookTime = onHookTime;
	return 0;
}
int ProSLIC_Reset (proslicChanType_ptr hProslic){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_Reset(hProslic);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_Reset(hProslic);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_Reset(hProslic);
#endif
	return 1;
}

int ProSLIC_Init (proslicChanType_ptr *hProslic,int size){
#ifdef SI324X
	if ((*hProslic)->deviceId->chipType >= SI3240 && (*hProslic)->deviceId->chipType <= SI3247)
		return Si324x_Init(hProslic,size);
#endif
#ifdef SI321X
	if ((*hProslic)->deviceId->chipType >= SI3210 && (*hProslic)->deviceId->chipType <= SI3216M)
		return Si321x_Init(hProslic,size);
#endif
#ifdef SI3220
	if ((*hProslic)->deviceId->chipType >= SI3220 && (*hProslic)->deviceId->chipType <= SI3225)
		return Si3220_Init(hProslic,size);
#endif
	return 1;
}

int ProSLIC_Cal (proslicChanType_ptr *hProslic, int size){
#ifdef SI324X
	if ((*hProslic)->deviceId->chipType >= SI3240 && (*hProslic)->deviceId->chipType <= SI3247)
		return Si324x_Cal(hProslic,size);
#endif
#ifdef SI321X
	if ((*hProslic)->deviceId->chipType >= SI3210 && (*hProslic)->deviceId->chipType <= SI3216M)
		return Si321x_Cal(hProslic,size);
#endif
#ifdef SI3220
	if ((*hProslic)->deviceId->chipType >= SI3220 && (*hProslic)->deviceId->chipType <= SI3225)
		return Si3220_Cal(hProslic,size);
#endif
	return 1;
}

int ProSLIC_LoadRegTables (proslicChanType_ptr *hProslic,ProslicRAMInit *pRamTable, ProslicRegInit *pRegTable,int size){
#ifdef SI324X
	if ((*hProslic)->deviceId->chipType >= SI3240 && (*hProslic)->deviceId->chipType <= SI3247)
		return Si324x_LoadRegTables(hProslic,pRamTable,pRegTable,size);
#endif
#ifdef SI321X
	if ((*hProslic)->deviceId->chipType >= SI3210 && (*hProslic)->deviceId->chipType <= SI3216M)
		return Si321x_LoadRegTables(hProslic,pRamTable,pRegTable,size);
#endif
#ifdef SI3220
	if ((*hProslic)->deviceId->chipType >= SI3220 && (*hProslic)->deviceId->chipType <= SI3225)
		return Si3220_LoadRegTables(hProslic,pRamTable,pRegTable,size);
#endif
	return 1;
}

int ProSLIC_LoadPatch (proslicChanType_ptr hProslic,proslicPatch *pPatch){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_LoadPatch(hProslic,pPatch);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_LoadPatch(hProslic,pPatch);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_LoadPatch(hProslic,pPatch);
#endif
	return 1;
}

int ProSLIC_EnableInterrupts (proslicChanType_ptr hProslic){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_EnableInterrupts(hProslic);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_EnableInterrupts(hProslic);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_EnableInterrupts(hProslic);
#endif
	return 1;
}

int ProSLIC_RingSetup (proslicChanType_ptr hProslic,int preset){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_RingSetup(hProslic,preset);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_RingSetup(hProslic,preset);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_RingSetup(hProslic,preset);
#endif
	return 1;
}

int ProSLIC_ToneGenSetup (proslicChanType_ptr hProslic,int preset){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_ToneGenSetup(hProslic,preset);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_ToneGenSetup(hProslic,preset);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_ToneGenSetup(hProslic,preset);
#endif
	return 1;
}

int ProSLIC_FSKSetup (proslicChanType_ptr hProslic,int preset){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_FSKSetup(hProslic,preset);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_FSKSetup(hProslic,preset);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_FSKSetup(hProslic,preset);
#endif
	return 1;
}

int ProSLIC_DTMFDecodeSetup (proslicChanType_ptr hProslic,int preset){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_DTMFDecodeSetup(hProslic,preset);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_DTMFDecodeSetup(hProslic,preset);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_DTMFDecodeSetup(hProslic,preset);
#endif
	return 1;
}

int ProSLIC_ZsynthSetup (proslicChanType_ptr hProslic,int preset){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_ZsynthSetup(hProslic,preset);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_ZsynthSetup(hProslic,preset);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_ZsynthSetup(hProslic,preset);
#endif
	return 1;
}

int ProSLIC_GciCISetup (proslicChanType_ptr hProslic,int preset){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_GciCISetup(hProslic,preset);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_GciCISetup(hProslic,preset);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_GciCISetup(hProslic,preset);
#endif
	return 1;
}

int ProSLIC_ModemDetSetup (proslicChanType_ptr hProslic,int preset){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_ModemDetSetup(hProslic,preset);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_ModemDetSetup(hProslic,preset);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_ModemDetSetup(hProslic,preset);
#endif
	return 1;
}

int ProSLIC_TXAudioGainSetup (proslicChanType_ptr hProslic,int preset){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_TXAudioGainSetup(hProslic,preset);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_TXAudioGainSetup(hProslic,preset);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_TXAudioGainSetup(hProslic,preset);
#endif
	return 1;
}

int ProSLIC_RXAudioGainSetup (proslicChanType_ptr hProslic,int preset){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_RXAudioGainSetup(hProslic,preset);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_RXAudioGainSetup(hProslic,preset);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_RXAudioGainSetup(hProslic,preset);
#endif
	return 1;
}

int ProSLIC_HybridSetup (proslicChanType_ptr hProslic,int preset){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_HybridSetup(hProslic,preset);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_HybridSetup(hProslic,preset);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_HybridSetup(hProslic,preset);
#endif
	return 1;
}

int ProSLIC_AudioEQSetup (proslicChanType_ptr hProslic,int preset){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_AudioEQSetup(hProslic,preset);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_AudioEQSetup(hProslic,preset);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_AudioEQSetup(hProslic,preset);
#endif
	return 1;
}

int ProSLIC_DCFeedSetup (proslicChanType_ptr hProslic,int preset){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_DCFeedSetup(hProslic,preset);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_DCFeedSetup(hProslic,preset);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_DCFeedSetup(hProslic,preset);
#endif
	return 1;
}

int ProSLIC_GPIOSetup (proslicChanType_ptr hProslic,int preset){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_GPIOSetup(hProslic,preset);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_GPIOSetup(hProslic,preset);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_GPIOSetup(hProslic,preset);
#endif
	return 1;
}

int ProSLIC_PulseMeterSetup (proslicChanType_ptr hProslic,int preset){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_PulseMeterSetup(hProslic,preset);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_PulseMeterSetup(hProslic,preset);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_PulseMeterSetup(hProslic,preset);
#endif
	return 1;
}

int ProSLIC_PCMSetup (proslicChanType_ptr hProslic,int preset){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_PCMSetup(hProslic,preset);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_PCMSetup(hProslic,preset);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_PCMSetup(hProslic,preset);
#endif
	return 1;
}

int ProSLIC_PCMTimeSlotSetup (proslicChanType_ptr hProslic, uInt16 rxcount, uInt16 txcount){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_PCMTimeSlotSetup(hProslic,rxcount,txcount);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_PCMTimeSlotSetup(hProslic,rxcount,txcount);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_PCMTimeSlotSetup(hProslic,rxcount,txcount);
#endif
	return 1;
}

int ProSLIC_FindInterruptChannels (proslicChanType_ptr *hProslic,uInt8 *pChannelNumbers){
#ifdef SI324X
	if ((*hProslic)->deviceId->chipType >= SI3240 && (*hProslic)->deviceId->chipType <= SI3247)
		return Si324x_FindInterruptChannels(hProslic,pChannelNumbers);
#endif
#ifdef SI321X
	if ((*hProslic)->deviceId->chipType >= SI3210 && (*hProslic)->deviceId->chipType <= SI3216M)
		return Si321x_FindInterruptChannels(hProslic,pChannelNumbers);
#endif
#ifdef SI3220
	if ((*hProslic)->deviceId->chipType >= SI3220 && (*hProslic)->deviceId->chipType <= SI3225)
		return Si3220_FindInterruptChannels(hProslic,pChannelNumbers);
#endif
	return 1;
}

int ProSLIC_GetInterrupts (proslicChanType_ptr hProslic,proslicIntType *pIntData){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_GetInterrupts(hProslic,pIntData);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_GetInterrupts(hProslic,pIntData);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_GetInterrupts(hProslic,pIntData);
#endif
	return 1;
}

int ProSLIC_ReadHookStatus (proslicChanType_ptr hProslic,uInt8 *pHookStat){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_ReadHookStatus(hProslic,pHookStat);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
	{
		return Si321x_ReadHookStatus(hProslic,pHookStat);		
	}
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_ReadHookStatus(hProslic,pHookStat);
#endif
	return 1;
}

int ProSLIC_SetLinefeedStatus (proslicChanType_ptr hProslic,uInt8 newLinefeed){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_SetLinefeedStatus(hProslic,newLinefeed);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_SetLinefeedStatus(hProslic,newLinefeed);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_SetLinefeedStatus(hProslic,newLinefeed);
#endif
	return 1;
}

int ProSLIC_PolRev (proslicChanType_ptr hProslic,uInt8 abrupt,uInt8 newPolRevState){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_PolRev(hProslic,abrupt,newPolRevState);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_PolRev(hProslic,abrupt,newPolRevState);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_PolRev(hProslic,abrupt,newPolRevState);
#endif
	return 1;
}

int ProSLIC_GPIOControl (proslicChanType_ptr hProslic,uInt8 *pGpioData, uInt8 read){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_GPIOControl(hProslic,pGpioData,read);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_GPIOControl(hProslic,pGpioData,read);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_GPIOControl(hProslic,pGpioData,read);
#endif
	return 1;
}

int ProSLIC_MWI (proslicChanType_ptr hProslic,uInt8 lampOn){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_MWI(hProslic,lampOn);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_MWI(hProslic,lampOn);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_MWI(hProslic,lampOn);
#endif
	return 1;
}

int ProSLIC_ToneGenStart (proslicChanType_ptr hProslic,uInt8 timerEn){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_ToneGenStart(hProslic,timerEn);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_ToneGenStart(hProslic,timerEn);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_ToneGenStart(hProslic,timerEn);
#endif
	return 1;
}

int ProSLIC_ToneGenStop (proslicChanType_ptr hProslic){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_ToneGenStop(hProslic);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_ToneGenStop(hProslic);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_ToneGenStop(hProslic);
#endif
	return 1;
}

int ProSLIC_RingStart (proslicChanType_ptr hProslic){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_RingStart(hProslic);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_RingStart(hProslic);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_RingStart(hProslic);
#endif
	return 1;
}

int ProSLIC_RingStop (proslicChanType_ptr hProslic){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_RingStop(hProslic);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_RingStop(hProslic);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_RingStop(hProslic);
#endif
	return 1;
}

int ProSLIC_EnableCID (proslicChanType_ptr hProslic){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_EnableCID(hProslic);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_EnableCID(hProslic);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_EnableCID(hProslic);
#endif
	return 1;
}

int ProSLIC_DisableCID (proslicChanType_ptr hProslic){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_DisableCID(hProslic);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_DisableCID(hProslic);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_DisableCID(hProslic);
#endif
	return 1;
}

int ProSLIC_SendCID (proslicChanType_ptr hProslic, uInt8 *buffer, uInt8 numBytes){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_SendCID(hProslic,buffer,numBytes);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_SendCID(hProslic,buffer,numBytes);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_SendCID(hProslic,buffer,numBytes);
#endif
	return 1;
}

int ProSLIC_PCMStart (proslicChanType_ptr hProslic){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_PCMStart(hProslic);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_PCMStart(hProslic);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_PCMStart(hProslic);
#endif
	return 1;
}

int ProSLIC_PCMStop (proslicChanType_ptr hProslic){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_PCMStop(hProslic);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_PCMStop(hProslic);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_PCMStop(hProslic);
#endif
	return 1;
}

/*
** Function: PROSLIC_DialPulseDetect
**
** Description: 
** implements pulse dial detection and should be called at every hook transistion
*/
int ProSLIC_DialPulseDetect (proslicChanType *pProslic, pulseDial_Cfg *pPulsedialCfg, pulseDialType *pPulseDialData){
	uInt8 hookStat;
//	uInt8 digit=0;
	int breaktime;
	int offHk, onHk;
	 

	
	TimeElapsed(pProTimer,pPulseDialData->onHookTime,&onHk); //get onhook time
	
	TimeElapsed(pProTimer,pPulseDialData->offHookTime,&offHk); //get offhook time

	ProSLIC_ReadHookStatus(pProslic,&hookStat);
	if (hookStat == ONHOOK){
		//we are on-hook. 
		getTime(pProTimer,pPulseDialData->onHookTime); //set onhooktime
	}
	else{
		//we are off-hook.
		breaktime = onHk;
		if ((breaktime >= (pPulsedialCfg->minOnHook)) && (breaktime <= (pPulsedialCfg->maxOnHook)))
       		pPulseDialData->currentPulseDigit++;
		else 
			return 1;
		getTime(pProTimer,pPulseDialData->offHookTime); 
	}
	return 0; 
}
int ProSLIC_DialPulseDetectTimeout (proslicChanType *pProslic, pulseDial_Cfg *pPulsedialCfg, pulseDialType *pPulseDialData){
	/*Pulse dial detect handling code start*/
		uInt8 HkStat;
		int time; uInt8 digit=0;
		ProSLIC_ReadHookStatus(pProslic,&HkStat);
		if (HkStat == ONHOOK){
			TimeElapsed(pProTimer,pPulseDialData->onHookTime,&time);
			if (time >  pPulsedialCfg->maxOnHook)
				return ON_HOOK_TIMEOUT;
			    //we hung up
		}
		if (HkStat == OFFHOOK && pPulseDialData->currentPulseDigit > 0){
			TimeElapsed(pProTimer,pPulseDialData->offHookTime,&time);
			if(time > pPulsedialCfg->maxOffHook){
				digit = pPulseDialData->currentPulseDigit;
				pPulseDialData->currentPulseDigit = 0;
				return digit;
			}
		}
		/*Pulse dial detect handling code end*/
		return 0;
}

int ProSLIC_DTMFReadDigit (proslicChanType_ptr hProslic,uInt8 *pDigit){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_DTMFReadDigit (hProslic,pDigit);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_DTMFReadDigit (hProslic,pDigit);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_DTMFReadDigit (hProslic,pDigit);
#endif
	return 1;
}

int ProSLIC_PLLFreeRunStart (proslicChanType_ptr hProslic){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_PLLFreeRunStart (hProslic);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_PLLFreeRunStart (hProslic);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_PLLFreeRunStart (hProslic);
#endif
	return 1;
}

int ProSLIC_PLLFreeRunStop (proslicChanType_ptr hProslic){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_PLLFreeRunStop (hProslic);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_PLLFreeRunStop (hProslic);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_PLLFreeRunStop (hProslic);
#endif
	return 1;
}

int ProSLIC_PulseMeterStart (proslicChanType_ptr hProslic){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_PulseMeterStart (hProslic);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_PulseMeterStart (hProslic);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_PulseMeterStart (hProslic);
#endif
	return 1;
} 

int ProSLIC_PulseMeterStop (proslicChanType_ptr hProslic){
#ifdef SI324X
	if (hProslic->deviceId->chipType >= SI3240 && hProslic->deviceId->chipType <= SI3247)
		return Si324x_PulseMeterStop (hProslic);
#endif
#ifdef SI321X
	if (hProslic->deviceId->chipType >= SI3210 && hProslic->deviceId->chipType <= SI3216M)
		return Si321x_PulseMeterStop (hProslic);
#endif
#ifdef SI3220
	if (hProslic->deviceId->chipType >= SI3220 && hProslic->deviceId->chipType <= SI3225)
		return Si3220_PulseMeterStop (hProslic);
#endif
	return 1;
}
