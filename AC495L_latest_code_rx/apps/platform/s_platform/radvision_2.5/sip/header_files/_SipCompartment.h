
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
 *                      <_SipCompartment.h>
 *
 * This file defines the  compartment manager object,that holds all the
 *  Compartments resources including memory pool, pools of lists, locks and
 * more. Its main functionality is to manage the compartments list, to create new
 *  compartments and to associate stack objects to a specific compartment.
 * Moreover,the file contains Internal API functions of the  Compartment layer.
 *
 *
 *    Author                         Date
 *    ------                        ------
 *    Dikla Dror                  Nov 2003
 *********************************************************************************/

#ifndef SIP_COMPARTMENT_H
#define SIP_COMPARTMENT_H

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------*/
/*                        INCLUDE HEADER FILES                           */
/*-----------------------------------------------------------------------*/
#include "AdsRlist.h"
#include "_SipCommonUtils.h"
#include "rvlog.h"
#include "RV_SIP_DEF.h"
#include "RvSipResourcesTypes.h"
#include "RvSipTransportTypes.h"
#include "RvSipCompartmentTypes.h"

#ifdef RV_SIGCOMP_ON
#include "RvSigComp.h"

/*-----------------------------------------------------------------------*/
/*                          TYPE DEFINITIONS                             */
/*-----------------------------------------------------------------------*/
#define COMPARTMENT_DEFAULT_ALGORITHM NULL

typedef struct
{
    RvLogSource*              pLogSrc;
    RvLogMgr                 *pLogMgr;
    RvInt32                   maxNumOfCompartments;
} SipCompartmentMgrCfg;

/*-----------------------------------------------------------------------*/
/*                           MODULE VARIABLES                            */
/*-----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/*                     COMPARMENT MANAGER API                     */
/*-----------------------------------------------------------------------*/

/***************************************************************************
 * SipCompartmentMgrConstruct
 * ------------------------------------------------------------------------
 * General: Construct a new  Compartment manager. The compartment
 *          manager is in charge of all the  compartment. The manager
 *          holds a list of compartments and is used to create new
 *          compartments.
 * Return Value: RV_ERROR_NULLPTR     - The pointer to the compartment mgr is
 *                                   invalid.
 *               RV_ERROR_OUTOFRESOURCES - Not enough resources to create the
 *                                    Compartment manager.
 *               RV_OK        -  Compartment manager was
 *                                   created Successfully.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:     cfg           - A pointer to the configuration structure,
 *                          containing all the required init. information.
 *          hStack        - A handle to the stack instance.
 * Output: *phCompartmentMgr - Handle to the newly created
 *                                    compartment manager.
 ***************************************************************************/
RvStatus RVCALLCONV SipCompartmentMgrConstruct(
                IN  SipCompartmentMgrCfg      *cfg,
                IN  void                      *hStack,
                OUT RvSipCompartmentMgrHandle *phCompartmentMgr);

/***************************************************************************
 * SipCompartmentMgrDestruct
 * ------------------------------------------------------------------------
 * General: Destructs the  Compartment manager freeing all resources.
 * Return Value:  RV_OK -  Compartment manager was destructed.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: hCompartmentMgr - Handle to the manager to destruct.
 ***************************************************************************/
RvStatus RVCALLCONV SipCompartmentMgrDestruct(
                  IN RvSipCompartmentMgrHandle hCompartmentMgr);


/***************************************************************************
 * SipCompartmentMgrCreateCompartment
 * ------------------------------------------------------------------------
 * General: Creates a new  Compartment.
 * Return Value: RV_ERROR_INVALID_HANDLE - The handle to the manager is invalid.
 *               RV_ERROR_OUTOFRESOURCES - Failed to create a new compartment. Not
 *                                   enough resources.
 *               RV_OK - Success.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:   hCompartmentMgr - Handle to the  compartment manager
 *		    strAlgoName     - The algorithm name which will affect the 
 *		 				      compartment compression manner. The algorithm
 *						      name MUST be equal to the set algorithm name 
 *						      during algorithm initialization. If this
 *						      parameter value is NULL the default algo is used
 * Output: phCompartment    - sip stack handle to the new compartment
 ***************************************************************************/
RvStatus RVCALLCONV SipCompartmentMgrCreateCompartment(
                   IN  RvSipCompartmentMgrHandle hCompartmentMgr,
				   IN  RvChar					*strAlgoName,
                   OUT RvSipCompartmentHandle   *phCompartment);



/***************************************************************************
 * SipCompartmentMgrGetResourcesStatus
 * ------------------------------------------------------------------------
 * General: Returns a structure with the status of all resources used by
 *          the  Compartment module. It includes the Compartments
 *          list.
 * Return Value: RV_ERROR_INVALID_HANDLE - The handle to the manager is invalid.
 *               RV_ERROR_NULLPTR    - The pointer to the resource structure is
 *                                  NULL.
 *               RV_OK       - Resource status returned successfully.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:     hMgr       - Handle to the  Compartment manager.
 * Output:     pResources - Pointer to a  Compartment resource structure.
 ***************************************************************************/
RvStatus RVCALLCONV SipCompartmentMgrGetResourcesStatus (
                                 IN  RvSipCompartmentMgrHandle  hMgr,
                                 OUT RvSipCompartmentResources *pResources);

/***************************************************************************
 * SipCompartmentMgrResetMaxUsageResourcesStatus
 * ------------------------------------------------------------------------
 * General: Reset the counter that counts the max number of  compartments
 *          that were used ( at one time ) until the call to this routine.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:    hMgr - The  Compartment manager.
 ***************************************************************************/
void RVCALLCONV SipCompartmentMgrResetMaxUsageResourcesStatus (
                                 IN  RvSipCompartmentMgrHandle  hMgr);

/***************************************************************************
 * SipCompartmentMgrSetSigCompMgrHandle
 * ------------------------------------------------------------------------
 * General: Stores the SigComp manager handle in the compartment manager
 *
 * Return Value: RvStatus
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:    hCompartmentMgr - Handle to the stack  comp. manager.
 *           hSigCompMgr     - Handle of the SigComp manager handle to be 
 *                             stored.
 * Output:   -
 ***************************************************************************/
RvStatus RVCALLCONV SipCompartmentMgrSetSigCompMgrHandle(
                      IN   RvSipCompartmentMgrHandle hCompartmentMgr,
                      IN   RvSigCompMgrHandle        hSigCompMgr);

/***************************************************************************
 * SipCompartmentMgrDetachFromSigCompModule
 * ------------------------------------------------------------------------
 * General: Release all the relations between SipStack compartments and 
 *          SigComp internal compartments.
 *
 * Return Value: RvStatus
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:    hCompartmentMgr - Handle to the stack comp. manager.
 * Output:   -
 ***************************************************************************/
RvStatus RVCALLCONV SipCompartmentMgrDetachFromSigCompModule(
                      IN   RvSipCompartmentMgrHandle hCompartmentMgr);

/*-----------------------------------------------------------------------*/
/*                           COMPARMENT API                              */
/*-----------------------------------------------------------------------*/

/***************************************************************************
 * SipCompartmentAttach
 * ------------------------------------------------------------------------
 * General: Attaching to an existing compartment.
 * Return Value: The return value status.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: pCompartment - pointer to the  compartment.
 ***************************************************************************/
RvStatus RVCALLCONV SipCompartmentAttach(
                IN RvSipCompartmentHandle hCompartment);

/***************************************************************************
 * SipCompartmentRelateInternalCompHandleToId
 * ------------------------------------------------------------------------
 * General: Relate the internal Compartment handle, used by the SigComp dll
 *          to unique ID. The hCompartment might be NULL and cause for the 
 *          release of the SigComp unique ID.
 * Return Value: RvStatus
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:  hCompartment   - Handle to the Compartment. In case that the it's 
 *                          value is NULL the given unique ID is release.
 *         uniqueID       - The unique ID to be relate to internal 
 *                          compartment.
 ***************************************************************************/
RvStatus RVCALLCONV SipCompartmentRelateInternalCompHandleToId(
                   IN  RvSipCompartmentHandle hCompartment,
                   IN  RvUint32               sigCompUniqueID);

/***************************************************************************
 * SipCompartmentCompressMessage
 * ------------------------------------------------------------------------
 * General: Compress a message. To be called before sending.
 * Return Value: RvStatus
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:  hCompartment   - Handle to the Compartment. In case that the it's 
 *                          value is NULL the given unique ID is release.
 * Input/Output:
 *         pMessageInfo   - a pointer to 'SipSigCompMessageInfo' structure,
 *                          holds the compressed message + its size
 ***************************************************************************/
RvStatus RVCALLCONV SipCompartmentCompressMessage(
                   IN    RvSipCompartmentHandle  hCompartment,
                   INOUT RvSigCompMessageInfo   *pMessageInfo);

/***************************************************************************
 * SipCompartmentResetByteCodeFlag
 * ------------------------------------------------------------------------
 * General: Compress a message. To be called before sending. This function 
 *			is meaningless in the context of dynamic compression 
 * Return Value: RvStatus
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:  hCompartment   - Handle to the Compartment.
 ***************************************************************************/
RvStatus RVCALLCONV SipCompartmentResetByteCodeFlag(
                   IN    RvSipCompartmentHandle  hCompartment);

#ifdef PRINT_SIGCOMP_BYTE_STREAM
/***************************************************************************
 * SipCompartmentPrintMsgByteStream
 * ------------------------------------------------------------------------
 * General: Prints to the log a SigComp message byte stream. 
 * Return Value: RvStatus
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:  pStreamBuf  - The printed stream message. 
 *		   streamSize  - The printed stream message size. 
 *		   pLogSrc     - The log source (used for the printing). 
 *		   bInOrOut    - Indication if the stream is sent (was compressed) 
 *						 or received (before decompression). 
 ***************************************************************************/
void RVCALLCONV SipCompartmentPrintMsgByteStream(IN RvUint8     *pStreamBuf,
												 IN RvUint32     streamSize,
												 IN RvLogSource *pLogSrc,
												 IN RV_BOOL	     bInOrOut);
#endif /* PRINT_SIGCOMP_BYTE_STREAM */

#endif /* RV_SIGCOMP_ON */

#ifdef __cplusplus
}
#endif

#endif /* END OF: #ifndef SIP_COMPARTMENT_H*/


