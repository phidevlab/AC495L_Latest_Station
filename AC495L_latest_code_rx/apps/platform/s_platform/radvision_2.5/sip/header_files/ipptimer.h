/***********************************************************************
        Copyright (c) 2003 RADVISION Ltd.
************************************************************************
NOTICE:
This document contains information that is confidential and proprietary
to RADVISION Ltd.. No part of this document may be reproduced in any
form whatsoever without written prior approval by RADVISION Ltd..

RADVISION Ltd. reserve the right to revise this publication and make
changes without obligation to notify any person of such revisions or
changes.
************************************************************************
$Revision:$
$Date:	 16.06.04$
$Author: Mark Appel$
************************************************************************/

#ifndef _RV_IPPTIMER_H
#define _RV_IPPTIMER_H

#include "rvtypes.h"
#include "rverror.h"
#include "rvlog.h"
#include "rvtimer.h"
#include "rvtimer.h"

#ifdef __cplusplus
extern "C" {
#endif

	
#define IPP_MAX_NUM_TIMERS		5

/*
 * IppTimer - wrapper	of RvTimer allowing to store timer parameters and to reuse of this object
 * as it was in Inc-Core
 */
  /*temporary: private struct*/
/*  typedef enum
  {
    IPP_TIMER_STATE_NOTINUSE,
		IPP_TIMER_STATE_INUSE,
  }IppTimerState;
 */
  typedef struct
  {
	  RvTimer		timer;
/*	  RvBool        bTimerInUse;  temporary: private field*/
	  RvInt64	    delay;
	  RvTimerFunc	callback;
	  void			*userdata;
  }IppTimer;

  typedef enum
  {
    IPP_TIMER_DONT_CHECK_IF_STARTED,
    IPP_TIMER_RETURN_IF_STARTED,
    IPP_TIMER_RESTART_IF_STARTED
  }IppIfAlreadyStarted;

/********************************************************************************************
* IppTimerConstruct
*
* PURPOSE : initialize timer
* INPUT   :
*           RvUint32   ms, delay
*           RvTimerFunc	  callback
*           void			    *userdata
*
* OUTPUT  :
* RETURN  : RV_OK on success, other on failure
*
*
  ********************************************************************************************/
  RvStatus    IppTimerConstruct (
                                  IppTimer* pTimer,
                                  RvUint32   ms, /* delay*/
                                  RvTimerFunc	  callback,
                                  void			    *userdata
                                );


/********************************************************************************************
* IppTimerDestruct
*
* PURPOSE : clean and free all allocated data.
* INPUT   :
*           pTimer		- pointer to timer object.
*
* OUTPUT  :
* RETURN  : RV_OK on success, other on failure
* remarks :
*
*
*
  ********************************************************************************************/
  RvStatus    IppTimerDestruct  (IppTimer* pTimer);


/********************************************************************************************
* IppTimerStart
*
* PURPOSE : start timer
* INPUT   :
*           pTimer		- pointer to timer object.
*
* OUTPUT  :
* RETURN  : RV_OK on success, other on failure
* remarks :
*
*
********************************************************************************************/
RvStatus IppTimerStart(
    IN IppTimer*            t,
    IN IppIfAlreadyStarted  ifAlreadyStarted,
    IN RvUint32             msDelay); /* if 0 then t->delay is used */

/********************************************************************************************
* IppTimerStop
*
* PURPOSE : cancel underlaying RvTimer
* INPUT   :
*           pTimer		- pointer to timer object.
*
* OUTPUT  :
* RETURN  : RV_OK on success, other on failure
* remarks :
*
*
  ********************************************************************************************/
RvStatus    IppTimerStop(
                          IN  IppTimer*   pTimer
                          );


/********************************************************************************************
* IppTimerInit
*
* PURPOSE : initialize global pointer to RvTimerQueue object - g_timerQ
*					:	that used in timer enabling mechanism
* INPUT   :
* OUTPUT  :
* RETURN  : RV_OK on success, other on failure
* remarks : Currently mentioned RvTimerQueue object is got from selectEngine of stack
*           So this function should be called from stack thread after the stack is initialized.
*
*
********************************************************************************************/
RvStatus IppTimerInit(void);

/********************************************************************************************
* IppTimerEnd()
*
* PURPOSE : nulify global pointer to RvTimerQueue object - g_timerQ
*					:	that used in timer enabling mechanism
* INPUT   : none
* OUTPUT  : none
* RETURN  : none
* remarks :
*
*
********************************************************************************************/
void IppTimerEnd(void);


RVAPI RvTimerQueue* IppGetTimerQueue(void);

#ifdef __cplusplus
}
#endif

#endif  /* _RV_IPPTIMER_H */

