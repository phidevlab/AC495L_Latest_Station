/** \file sys_service.c
 * sys_service.c
 *
 *  This file implements the required system services for the API-II using a
 * Linux OS running on the UVB.  The user should replace the functions provided
 * here with the equivalent based on their OS and hardware.
 *
 * Copyright (c) 2005, Legerity Inc.
 * All rights reserved
 *
 * This software is the property of Legerity , Inc. Please refer to the
 * Non Disclosure Agreement (NDA) that you have signed for more information
 * on legal obligations in using, modifying or distributing this file.
 */
#include "vp_api_types.h"
#include "sys_service.h"
#include <linux/kernel.h>

#include <linux/sched.h>
//#include <asm/semaphore.h>
#include <linux/delay.h>
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


typedef struct semaphore sem_t;

typedef struct{
	int runningPid;
	int cnt;
	sem_t criticalSectionSem;
	sem_t sectionTypeSem;
} CRITICAL_SECTION_PARAMS;

CRITICAL_SECTION_PARAMS le88CriticalSec[VP_NUM_CRITICAL_SEC_TYPES];

uint8 InitCriticalSectionDB(void)
{
	int i;
	for(i=0; i<VP_NUM_CRITICAL_SEC_TYPES; i++)
	{
		le88CriticalSec[i].runningPid = -1;
		le88CriticalSec[i].cnt = 0;	
		sema_init( &le88CriticalSec[i].criticalSectionSem, 1);
		sema_init( &le88CriticalSec[i].sectionTypeSem, 1);
	}
       return 1;
}

int fake_printf(const char *format, ...) {
	/* don't do anything */
    return 0;
}

void VpSysWait( uint8 time )  /* Time specified in increments of 125uS (e.g. 4 = 500uS) */
{
	udelay(125 * time);
}


uint8 VpSysEnterCritical( VpDeviceIdType deviceId, VpCriticalSecType criticalSecType )
{	
	down(&le88CriticalSec[criticalSecType].sectionTypeSem); 

	if( (le88CriticalSec[criticalSecType].runningPid == -1)||
		(le88CriticalSec[criticalSecType].runningPid == current->pid) )
	{
		if(le88CriticalSec[criticalSecType].runningPid == -1)
		{
			le88CriticalSec[criticalSecType].runningPid = current->pid;
 			down(&le88CriticalSec[criticalSecType].criticalSectionSem);
		}
			
		le88CriticalSec[criticalSecType].cnt++;
		up( &le88CriticalSec[criticalSecType].sectionTypeSem ); 

	}
	else {

		up( &le88CriticalSec[criticalSecType].sectionTypeSem ); 

		printk("\n ***** reject current->pid = %d  criticalSecType = %d  cnt = %d *****\n", 
				current->pid, criticalSecType, le88CriticalSec[criticalSecType].cnt);

		/* new task waits on semaphore */
 		down(&le88CriticalSec[criticalSecType].criticalSectionSem);

		down(&le88CriticalSec[criticalSecType].sectionTypeSem);

		le88CriticalSec[criticalSecType].runningPid = current->pid;
		le88CriticalSec[criticalSecType].cnt++;

		up( &le88CriticalSec[criticalSecType].sectionTypeSem ); 
	}

  return 1; 
  
} /* VpSysEnterCritical() */


uint8 VpSysExitCritical( VpDeviceIdType deviceId, VpCriticalSecType criticalSecType )
{
	down(&le88CriticalSec[criticalSecType].sectionTypeSem);

	/* In case VpSysExitCritical() was called overly */
	if(le88CriticalSec[criticalSecType].cnt <= 0)
	{
		printk("\n***** VpSysExitCritical() was called overly  criticalSecType = %d*****", criticalSecType);
		up( &le88CriticalSec[criticalSecType].sectionTypeSem ); 
			
		return 1;
	}

	le88CriticalSec[criticalSecType].cnt--;

	if(le88CriticalSec[criticalSecType].cnt == 0)
	{
		le88CriticalSec[criticalSecType].runningPid = -1;
		up( &le88CriticalSec[criticalSecType].criticalSectionSem ); 
	}
	else {
		/* nothing to do */
	}

	up( &le88CriticalSec[criticalSecType].sectionTypeSem ); 

  return 1;
  
} /* VpSysExitCritical() */
/**
 * VpSysDisableInt(), VpSysEnableInt(), and VpSysTestInt()
 *
 *  These functions are used by the CSLAC device family for interrupt driven
 * polling modes. These are called by the API to detect when a non-masked
 * device status has changed.  If using SIMPLE_POLL mode, these functions do not
 * require implementation.
 *
 * Preconditions:
 *  None. The implementation of these functions is architecture dependent.
 *
 * Postconditions:
 *  VpSysDisableInt() - The interrupt associated with the deviceId passed is
 * disabled.
 *
 * VpSysEnableInt() - The interrupt associated with the deviceId passed is
 * enabled.
 *
 * VpSysTestInt() - The return value is TRUE if an interrupt occurred, otherwise
 * return FALSE.
 *
 * These functions are needed only for CSLAC devices
 * (880, 790). For other devices these functions could be commented.
 *
 */
void
VpSysDisableInt(
    VpDeviceIdType deviceId)
{
    return;
}
void
VpSysEnableInt(
    VpDeviceIdType deviceId)
{
    return;
}
bool
VpSysTestInt(
    VpDeviceIdType deviceId)
{
    return FALSE;
}
/**
 * VpSysDtmfDetEnable(), VpSysDtmfDetDisable()
 *
 *  These functions are used by the CSLAC device family for devices that do not
 * internally detect DTMF. It is used for Caller ID type-II and is provided to
 * enable external DTMF detection.
 *
 * Preconditions:
 *  None. The implementation of these functions is application dependent.
 *
 * Postconditions:
 *  VpSysDtmfDetEnable() - The device/channel resource for DTMF detection is
 * enabled.
 *
 *  VpSysDtmfDetDisable() - The device/channel resource for DTMF detection is
 * disabled.
 *
 * These functions are needed only for CSLAC devices
 * (880, 790). For other devices these functions could be commented.
 *
 */
void
VpSysDtmfDetEnable(
    VpDeviceIdType deviceId,
    uint8 channelId)
{
}
void
VpSysDtmfDetDisable(
    VpDeviceIdType deviceId,
    uint8 channelId)
{
}
/*
 * The following functions VpSysTestHeapAcquire(),  VpSysTestHeapRelease()
 * VpSysPcmCollectAndProcess() and are needed only for CSLAC devices
 * (880). For other devices these functions could be commented. Please see
 * the LineTest API documentation for function requirements.
 */
void *
VpSysTestHeapAcquire(
    uint8 *pHeapId)
{
    return VP_NULL;
} /* VpSysTestHeapAcquire() */
bool
VpSysTestHeapRelease(
    uint8 heapId)
{
    return TRUE;
} /* VpSysTestHeapRelease() */
void
VpSysPcmCollectAndProcess(
    void *pLineCtx,
    VpDeviceIdType deviceId,
    uint8 channelId,
    uint8 startTimeslot,
    uint16 operationTime,
    uint16 settlingTime,
    uint16 operationMask)
{
} /* VpSysPcmCollectAndProcess() */

