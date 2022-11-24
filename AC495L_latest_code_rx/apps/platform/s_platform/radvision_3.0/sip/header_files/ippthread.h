/******************************************************************************
Filename   : ipptimer.c
Description: Implements Mdm Connection Object
******************************************************************************
                Copyright (c) 2001 RADVISION
************************************************************************
NOTICE:
This document contains information that is proprietary to RADVISION.
No part of this publication may be reproduced in any form whatsoever
without written prior approval by RADVISION.

RADVISION reserves the right to revise this publication and make
changes without obligation to notify any person of such revisions or
changes.
******************************************************************************
$Revision:$
$Date:	 23.02.05$
$Author: Assi Weinberger$
******************************************************************************/
#ifndef _RV_IPPTHREAD_H
#define _RV_IPPTHREAD_H


/***************************************************************************
 * IppThreadSleep
 * ------------------------------------------------------------------------
 * General: Suspends the current thread for the requested amount of time.
 *          
 * Return Value: RvStatus
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: 	sec		-  second 
 *			msec	-  milli second
 *
 *			example -  sec=1,nsec=50 the time will be 1 sec and 50 milli second
 ***************************************************************************/
RVAPI RvStatus IppThreadSleep(IN RvInt32 sec,
						IN RvInt32 msec);

#endif  /* _RV_IPPTHREAD_H */

