/************************************************************************************************************************
NOTICE:
This document contains information that is proprietary to RADVision LTD..
No part of this publication may be reproduced in any form whatsoever without
written prior approval by RADVision LTD..

RADVision LTD. reserves the right to revise this publication and make changes
without obligation to notify any person of such revisions or changes.

************************************************************************************************************************/



/*********************************************************************************
 *                              TransportPreemption.h
 *
 * provides preemptions calls and convertions
 *
 *    Author                         Date
 *    ------                        ------
 *    Udi Tir0sh                    6.7.2004
 *********************************************************************************/


#ifndef TRANSPORT_PREEMPTION_H
#define TRANSPORT_PREEMPTION_H

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------*/
/*                        INCLUDE HEADER FILES                           */
/*-----------------------------------------------------------------------*/
#include "RV_SIP_DEF.h"
#include "TransportMgrObject.h"

/*-----------------------------------------------------------------------*/
/*                        INCLUDE HEADER FILES                           */
/*-----------------------------------------------------------------------*/
#define PreemptionBaseMsg           128
#define PreemptionMaxMsg            3
#define TransportPreemptionCreatePmsg(_msg,_index) ((_msg)+((_index)*PreemptionMaxMsg)+PreemptionBaseMsg)

/*-----------------------------------------------------------------------*/
/*                          TYPE DEFINITIONS                             */
/*-----------------------------------------------------------------------*/
/*
 * PreemptionUsers
 * ----------------------
 * usesrs - number of users in this struct.
 * pManagers - pointers to transport Managers
 */
#define MAX_CUNCURENT_SIP_STACKS 10
typedef struct 
{
    RvInt32         usesrs;
    TransportMgr*   pManagers[MAX_CUNCURENT_SIP_STACKS];
}PreemptionUsers;

/*-----------------------------------------------------------------------*/
/*                          FUNCTIONS HEADERS                            */
/*-----------------------------------------------------------------------*/
/***************************************************************************
 * TransportPreemptionConstruct
 * ------------------------------------------------------------------------
 * General:  Creates the preemption callback and initializezs the thread 
 *           variable for the preemption users array
 * Return Value: 
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:   pMgr    - transport manager.
 ***************************************************************************/
RvStatus RVCALLCONV  TransportPreemptionConstruct(IN    TransportMgr*    pMgr);

/***************************************************************************
 * TransportPreemptionDestruct
 * ------------------------------------------------------------------------
 * General:  Creates the preemption callback and initializezs the thread 
 *           variable for the preemption users array
 * Return Value: 
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:   pMgr    - transport manager.
 ***************************************************************************/
void RVCALLCONV  TransportPreemptionDestruct(IN    TransportMgr*    pMgr);


#ifdef __cplusplus
}
#endif

#endif /*TRANSPORT_PREEMPTION_H*/

