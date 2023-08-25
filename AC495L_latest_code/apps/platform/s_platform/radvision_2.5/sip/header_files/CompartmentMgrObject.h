
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
 *                      <CompartmentMgrObject.h>
 *
 * This file defines the compartment manager object,that holds all the
 * Compartments resources including memory pool, pools of lists, locks and
 * more. Its main functionality is to manage the compartments list, to create new
 * compartments and to associate stack objects to a specific compartment.
 *
 *
 *    Author                         Date
 *    ------                        ------
 *    Dikla Dror                  Nov 2003
 *********************************************************************************/

#ifndef COMPARTMENT_MGR_OBJECT_H
#define COMPARTMENT_MGR_OBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------*/
/*                        INCLUDE HEADER FILES                           */
/*-----------------------------------------------------------------------*/
#include "rvmutex.h"
#include "AdsRlist.h"
#include "RV_SIP_DEF.h"
#include "RvSipTransportTypes.h"
#include "RvSipCompartmentTypes.h"
#include "_SipCompartment.h"

#ifdef RV_SIGCOMP_ON
#include "RvSigComp.h"
    
/*-----------------------------------------------------------------------*/
/*                          TYPE DEFINITIONS                             */
/*-----------------------------------------------------------------------*/


/* CompartmentMgr
 * ---------------------------------------------------------------------------
 * The Compartment manager object holds the compartment module
 * configuration and resources handles including the list of compartments,
 * the module memory pool,lock pool and more.
 *
 * hCompartmentsPool   -- Handle to a pool of compartments list. only one list is
 *                        allocated from this pool.
 * hCompartmentsList   -- Handle to the compartments list.
 * lock                -- A lock used to lock the manager object to avoid
 *                        threads collisions.
 * maxNumOfCompartment -- Max number of call in the call-leg list.
 * hStack              -- Handle to the containing stack instance.
 * hLog                -- Handle to the log module. Used for registering new object
 *                        to the log module.
 * pLogSrc               -- The module log-id. Used for printing messages.
 * pLogMgr             -- A pointer to the common core log manager
 */

typedef struct
{
    /*The compartments data-structure pool:*/
    RLIST_POOL_HANDLE       hCompartmentPool;
    /*A Compartments data-structure:*/
    RLIST_HANDLE            hCompartmentList;
    /* A lock object: */
    RvMutex                 lock;
    /* Handle to the containing stack instance*/
    void*                   hStack;
    /* The module log-id. Used for printing messages. */
    RvLogSource*            pLogSrc;
    /* A pointer to the common core log manager */
    RvLogMgr               *pLogMgr;
    /* Handle to the SigComp manager */
    RvSigCompMgrHandle      hSigCompMgr;
    /* Configuration parameters */
    RvInt32                 maxNumOfCompartments;
}CompartmentMgr;



/*-----------------------------------------------------------------------*/
/*                          FUNCTIONS HEADERS                            */
/*-----------------------------------------------------------------------*/

/***************************************************************************
 * CompartmentMgrConstruct
 * ------------------------------------------------------------------------
 * General: Construct a new Compartment manager. The compartment
 *          manager is in charge of all the compartments. The manager
 *          holds a list of compartments and is used to create new
 *          compartments.
 * Return Value: RV_ERROR_NULLPTR     - The pointer to the compartment mgr is
 *                                   invalid.
 *               RV_ERROR_OUTOFRESOURCES - Not enough resources to create the
 *                                   Compartment manager.
 *               RV_OK        - Compartment manager was
 *                                   created Successfully.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:     cfg              - A pointer to the configuration structure,
 *                             containing all the required init. information.
 *          hStack           - A handle to the stack instance.
 * Output:  phCompartmentMgr - Pointer to the newly created compartment manager.
 ***************************************************************************/
RvStatus RVCALLCONV CompartmentMgrConstruct(
                IN  SipCompartmentMgrCfg      *cfg,
                IN  void                      *hStack,
                OUT RvSipCompartmentMgrHandle *phCompartmentMgr);

/***************************************************************************
 * CompartmentMgrDestruct
 * ------------------------------------------------------------------------
 * General: Destructs the Compartment manager freeing all resources.
 * Return Value:  RV_OK - Compartment manager was destructed.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: hCompartmentMgr - Pointer to the manager to destruct.
 ***************************************************************************/
RvStatus RVCALLCONV CompartmentMgrDestruct(
                            IN RvSipCompartmentMgrHandle hCompartmentMgr);

/***************************************************************************
 * CompartmentMgrCreateCompartment
 * ------------------------------------------------------------------------
 * General: Creates a new Compartment.
 * Return Value: RV_ERROR_INVALID_HANDLE - The handle to the manager is invalid.
 *               RV_ERROR_OUTOFRESOURCES - Failed to create a new compartment. Not
 *                                   enough resources.
 *               RV_OK - Success.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:     pMgr           - Pointer to the compartment manager
 *			  strAlgoName    - The algorithm name which will affect the 
 *		 				       compartment compression manner. The algorithm
 *						       name MUST be equal to the set algorithm name 
 *						       during algorithm initialization. If this
 *						       parameter value is NULL the default algo is used
 * Output:    phCompartment  - sip stack handle to the new compartment
 ***************************************************************************/
RvStatus RVCALLCONV CompartmentMgrCreateCompartment(
                    IN  CompartmentMgr         *pMgr,
					IN  RvChar				   *strAlgoName,
                    OUT RvSipCompartmentHandle *phCompartment);

/***************************************************************************
 * CompartmentMgrDetachFromSigCompModule
 * ------------------------------------------------------------------------
 * General: Release all the relations between SipStack compartments and 
 *          SigComp internal compartments.
 *
 * Return Value: RvStatus
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:    pMgr - Pointer to the comp. manager.
 * Output:   -
 ***************************************************************************/
RvStatus RVCALLCONV CompartmentMgrDetachFromSigCompModule(
                                        IN  CompartmentMgr  *pMgr);


#endif /* RV_SIGCOMP_ON */

#ifdef __cplusplus
}
#endif

#endif /* END OF: #ifndef COMPARTMENT_MGR_OBJECT_H*/


