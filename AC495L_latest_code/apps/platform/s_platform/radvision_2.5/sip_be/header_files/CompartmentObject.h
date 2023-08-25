
/*
*********************************************************************************
*                                                                               *
* NOTICE:                                                                       *
* This document contains information that is confidential and proprietary to    *
* RADVision LTD.. No part of this publication may be reproduced in any form     *
* whatsoever without written prior approval by RADVision LTD..                  *
*                                                                               *
* RADVision LTD. reserves the right to revise this publication and make changes *
* without obligation to notify any person of such revisions or changes.         *
*********************************************************************************
*/


/*********************************************************************************
 *                              <CompartmentObject.h>
 *
 * This file defines the  compartment object, attributes and interface
 * functions. The comparment represents a  Compartment as defined in RFC3320.
 * This compartment unifies a group of SIP Stack objects such as CallLegs or
 * Transacions that is identify by a compartment ID when sending request
 * through a compressor entity.
 *
 *    Author                         Date
 *    ------                        ------
 *    Dikla Dror                  Nov 2003
 *********************************************************************************/

#ifndef COMPARTMENT_OBJECT_H
#define COMPARTMENT_OBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------*/
/*                        INCLUDE HEADER FILES                           */
/*-----------------------------------------------------------------------*/
#include "rvmutex.h"
#include "RV_SIP_DEF.h"
#include "CompartmentMgrObject.h"
#include "RvSipCompartmentTypes.h"

#ifdef RV_SIGCOMP_ON
#include "RvSigComp.h"

/*-----------------------------------------------------------------------*/
/*                          TYPE DEFINITIONS                             */
/*-----------------------------------------------------------------------*/

#define LOCK_MGR(pMgr)    RvMutexLock  (&pMgr->lock,pMgr->pLogMgr)
#define UNLOCK_MGR(pMgr)  RvMutexUnlock(&pMgr->lock,pMgr->pLogMgr)

/* Compartment
 * --------------------------------------------------------------------------
 * The Compartment defines an  Compartment as defined in RFC3320.
 * This compartment unifies a group of SIP Stack objects such as CallLegs or
 * Transacions that is identify by a compartment ID when sending request
 * through a compressor entity.
 *
 * lock            -- Lock object for protecting the compartment object in
 *                    multithreaded app.
 * referenceCount  -- A counter that contains the number of Stack object
 *                   that refer to the comparment object.
 * pMgr            -- Pointer to the compartment manager object.
 * hAppCompartment -- Application handle of the compartment.
 * hInternalComp   -- Handle to the internal SigComp dll comparmtent
 * algoName        -- The algorithm name which will affect the 
 *					  compartment compression manner. The algorithm
 *					  name MUST be equal to the set algorithm name 
 *					  during algorithm initialization. If this
 *					  parameter value is NULL the default algo is used
 */
typedef struct
{
    RvMutex                    lock;
    RvInt32                    referenceCount;
    CompartmentMgr            *pMgr;
    RvSipAppCompartmentHandle  hAppCompartment;
    RvSigCompCompartmentHandle hInternalComp;
	RvChar				       strAlgoName[RVSIGCOMP_ALGO_NAME];
}  Compartment;



/*-----------------------------------------------------------------------*/
/*                          FUNCTIONS HEADERS                            */
/*-----------------------------------------------------------------------*/

/***************************************************************************
* CompartmentLock
* ------------------------------------------------------------------------
* General: Locks a  compartment object.
* Return Value: -
* ------------------------------------------------------------------------
* Arguments:
* Input: pCompartment - Pointer to the  compartment structure
***************************************************************************/
void CompartmentLock(Compartment *pCompartment);

/***************************************************************************
* CompartmentUnlock
* ------------------------------------------------------------------------
* General: Unlocks a  compartment object.
* Return Value: -
* ------------------------------------------------------------------------
* Arguments:
* Input: pCompartment - Pointer to the  compartment structure
***************************************************************************/
void CompartmentUnlock(Compartment *pCompartment);


/***************************************************************************
 * CompartmentInitialize
 * ------------------------------------------------------------------------
 * General: Initialize a new compartment object. Sets all values to their
 *          initial values.
 * Return Value: RV_ERROR_UNKNOWN - Failed to initialize the compartment.
 *               RV_OK - Call-Id was generated successfully.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:     pCompartment - pointer to the new Compartment.
 *			  algoName     - The algorithm name which will affect the 
 *						     compartment compression manner. The algorithm
 *						     name MUST be equal to the set algorithm name 
 *						     during algorithm initialization. If this
 *						     parameter value is NULL the default algo is used
 *            pMgr         - pointer to the manager of this call.
 ***************************************************************************/
RvStatus RVCALLCONV CompartmentInitialize(
                            IN  Compartment     *pCompartment,
							IN  RvChar			*strAlgoName,
                            IN  CompartmentMgr  *pMgr);

/***************************************************************************
 * CompartmentAttach
 * ------------------------------------------------------------------------
 * General: Attaching to an existing compartment.
 *
 * Return Value: The return value status.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: pCompartment - pointer to the  compartment.
 ***************************************************************************/
RvStatus RVCALLCONV CompartmentAttach(IN Compartment *pCompartment);

/***************************************************************************
 * CompartmentDetach
 * ------------------------------------------------------------------------
 * General: Detaching from an existing compartment. In case that reference
 *          count becomes zero the compartment is terminated automatically,
 *          since the comparment is not used by any of the stack objects or
 *          by the application above.
 *
 * Return Value: The return value status.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: pCompartment - pointer to the  compartment.
 ***************************************************************************/
RvStatus RVCALLCONV CompartmentDetach(IN Compartment *pCompartment);

/***************************************************************************
 * CompartmentTerminate
 * ------------------------------------------------------------------------
 * General: Destructs a Compartment object, Free the resources taken
 *          by it and remove it from the manager compartments list.
 * Return Value: RV_OK - If the compartment is destructed successfully.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: pCompartment - pointer to the terminated  Compartment
 ***************************************************************************/
RvStatus RVCALLCONV CompartmentTerminate(
                           IN Compartment *pCompartment);

/***************************************************************************
 * IsCompartmentAccessible
 * ------------------------------------------------------------------------
 * General: The function checks if the compartment is accessible.
 * Return Value: RV_FALSE/RV_TRUE
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: pCompartment - pointer to the checked  Compartment
 ***************************************************************************/
RvBool RVCALLCONV CompartmentIsAccessible(
                              Compartment *pCompartment);

#endif /* RV_SIGCOMP_ON */

#ifdef __cplusplus
}
#endif

#endif /* END OF: #ifndef COMPARTMENT_OBJECT_H*/


