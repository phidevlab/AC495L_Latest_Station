/*      TIMER   TO   User  TimerAPI.h */
/*===============================================================*/
#ifndef AC_TIMERS_API_H
#define AC_TIMERS_API_H

#include "acTimers.h"
#include "acTimersUserDef.h"


 
/******************************************************************************
*	Function:	 AcTimerInit
*
*	Description: Allocates resources for timer mechanism and exeutes the TimerTask thread
*
*	Returns:	 int - TIMER_OK (0) or TIMER_ERROR_GENERAL_ERR (13)
*
*******************************************************************************/
int 		AcTimerInit(void);


/******************************************************************************
*	Function:	 AcTimerDestroy
*
*	Description: Free timer resources and kill relative thread
*
*	Returns:	 int - TIMER_OK (0) or TIMER_ERROR_GENERAL_ERR (13) or TIMER_NOT_INIT_YET(10)
*
*******************************************************************************/
int 		AcTimerDestroy(void);


/******************************************************************************
*	Function:	 AcArmTimer
*
*	Description: Creating a timer with the relevant parameters and running it
*
*	Parameters:	[I] TimerUserParams *pUserTimerConfig - The user timer configuration
*                		[O] int *pTid - The ID of the newly created timer
*
*	Returns:	 int - TIMER_OK (0) or TIMER_NOT_INIT_YET (10) or TIMER_ILLEGAL_PARAM(9)
*
*******************************************************************************/
int  		AcArmTimer(TimerUserParams *pUserTimerConfig, int *Tid);


/******************************************************************************
*	Function:	 AcDeleteTimer
*
*	Description: Delete a specific timer
*
*	Parameters:	[I] int Tid - The timer id to be deleted
*
*	Returns:	 int - TIMER_OK (0) or TIMER_NOT_INIT_YET (10) or TIMER_ILLEGAL_TID(8) 
*				 or TIMER_ERROR_GENERAL_ERR(13)
*
*******************************************************************************/
int  		AcDeleteTimer(int Tid);





//void 	AcStartTimers();
//int  AllocateAndRunTimer(TimerUserParams UserTimerConfig,int *Tid);
//int  RunTimer(int Tid);
//int  StopTimer(int Tid);  
// static int ConfigTimer(TimerRecord *Timer, TimerUserParams *pUserTimerConfig)
//int  GetTimerConfig(int Tid, TimerUserParams *TimerConfig);

// The following 3 functions are not supported because the LIst don't support re-insert with out new allocation 
//int  AcChangeTimerPeriod(int Tid, unsigned long NewPeriodMilli);
//int  AcChangeTimerExpireFunction(int Tid, TypeExpire ExpireFunction,unsigned long Params[]);
//int  QueryTimerTime2Expire(int *Tid,ULONG *Milli2Expire); 


//unsigned long TimeBaseDiffMilli(unsigned long currentTime, unsigned long expiredTime)

//typedef enum{ROUNDED_NULL,ROUNDED_QUARTER_HOUR,ROUNDED_HALF_HOUR,ROUNDED_HOUR,ROUNDED_DAY} RoundedType;



#endif //TIMER_TASK_API_H
