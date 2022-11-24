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
 *                              <_SipCommonUtils.h>
 *
 *  The file holds utils functions to be used in all stack layers.
 *    Author                         Date
 *    ------                        ------
 *********************************************************************************/
#ifndef SIP_COMMON_UTILS_H
#define SIP_COMMON_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif
/*-----------------------------------------------------------------------*/
/*                        INCLUDE HEADER FILES                           */
/*-----------------------------------------------------------------------*/
#include "RV_SIP_DEF.h"
#include "RV_ADS_DEF.h"
#include "_SipCommonTypes.h"
#include "rvlog.h"
#include "RvSipTransportTypes.h"
#include "rvrandomgenerator.h"
#include "AdsRpool.h"

/*-----------------------------------------------------------------------*/
/*                                 MACROS                               */
/*-----------------------------------------------------------------------*/
#define SIP_COMMON_ID_SIZE                 128

/*-----------------------------------------------------------------------*/
/*                           MODULE FUNCTIONS                            */
/*-----------------------------------------------------------------------*/

/***************************************************************************
 * SipCommonStricmp
 * ------------------------------------------------------------------------
 * General: The function does insensitive comparsion between 2 strings..
 * Return Value: RV_TRUE - if equal, RV_FALSE - if not equal.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: number - The integer value.
 *        buffer - The buffer that will contain the string of the integer.
 ***************************************************************************/
RvBool RVCALLCONV SipCommonStricmp(
                        IN RvChar* firstStr,
                        IN RvChar* secStr);

/***************************************************************************
 * SipCommonStricmpByLen
 * ------------------------------------------------------------------------
 * General: The function does insensitive comparsion between 2 strings which
 *          are not null terminated
 * Return Value: RV_TRUE - if equal, RV_FALSE - if not equal.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: firstStr - The first string.
 *        secStr   - The second string
 ***************************************************************************/
RvBool RVCALLCONV SipCommonStricmpByLen(
                        IN RvChar* firstStr,
                        IN RvChar* secStr,
                        IN RvInt32 len);

/***************************************************************************
 * SipCommonStrToLower
 * ------------------------------------------------------------------------
 * General: The function changes all chracters in a given string, to lower case
 * Return Value: RV_TRUE - if equal, RV_FALSE - if not equal.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: str - The given string.
 *        length  - The string length
 ***************************************************************************/
void RVCALLCONV SipCommonStrToLower(IN RvChar *str, IN RvInt32 length);


#if (RV_LOGMASK != RV_LOGLEVEL_NONE)
/********************************************************************************************
 * SipCommonStatus2Str
 * purpose : Convert a status into a string
 * input   : status             - Status
 * output  : none
 * return  : Status string
 ********************************************************************************************/
RvChar* RVCALLCONV SipCommonStatus2Str(IN RvStatus status);

/***************************************************************************
* RV2STR
* ------------------------------------------------------------------------
* General: Converts sip status string
* Return Value:RvStatus
* ------------------------------------------------------------------------
* Arguments: status code.
***************************************************************************/
#define RV2STR(_s) SipCommonStatus2Str(_s)

#else

#define RV2STR(_s)

#endif /*#if (RV_LOGMASK != RV_LOGLEVEL_NONE)*/

/********************************************************************************************
 * SipCommonObjectType2Str
 * purpose : Convert an object type into a string
 * input   : status             - Status
 * output  : none
 * return  : Status string
 ********************************************************************************************/
RvChar* RVCALLCONV SipCommonObjectType2Str(IN RvStatus status);

/***************************************************************************
 * SipCommonCopyStrFromPageToPage
 * ------------------------------------------------------------------------
 * General: Copy a string from a source rpool pointer to a destination.
 * Return Value: RvStatus
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:         pSrcPtr - The rpool pointer of the source string
 * Input Output:  pDestPtr- The rpool pointer of the destination string. The
 *                          string offset will be returned.
 ***************************************************************************/
RvStatus RVCALLCONV SipCommonCopyStrFromPageToPage(
                                     IN    RPOOL_Ptr       *pSrcPtr,
                                     INOUT RPOOL_Ptr       *pDestPtr);

/******************************************************************************
 * SipCommonConstructTripleLock
 * ----------------------------------------------------------------------------
 * General: Constructs locks of the triple lock and initializes the lock
 * Return Value: RvStatus
 * ----------------------------------------------------------------------------
 * Arguments:
 * Input:   pTripleLock - Pointer to the Triple Lock
 *          pLogMgr     - Pointer to the Log Manager, to be used for logging
 *****************************************************************************/
RvStatus RVCALLCONV SipCommonConstructTripleLock(
                                    IN SipTripleLock* pTripleLock,
                                    IN RvLogMgr*      pLogMgr);

/******************************************************************************
 * SipCommonDestructTripleLock
 * ----------------------------------------------------------------------------
 * General: Destructs locks of the triple lock and resets it's fields
 * Return Value: RvStatus
 * ----------------------------------------------------------------------------
 * Arguments:
 * Input:   pTripleLock - Pointer to the Triple Lock
 *          pLogMgr     - Pointer to the Log Manager, to be used for logging
 *****************************************************************************/
void RVCALLCONV SipCommonDestructTripleLock(
                                    IN SipTripleLock* pTripleLock,
                                    IN RvLogMgr*      pLogMgr);

/***************************************************************************
* SipCommonUnlockBeforeCallback
* ------------------------------------------------------------------------
* General:Prepare a handle for use in a callback to the app.
*         This function will make sure the handle is unlocked the necessary
*         number of times and initialize tripleLock->objLockThreadId to NULL
*         since we are currently unlocking the object lock.
*         SipCommonLockAfterCallback() should be called after the
*         callback.
* Return Value: void.
* ------------------------------------------------------------------------
* Arguments:
* input: pLogMgr       - log manager
*        pLogSrc       - log source
*        pTripleLock   - Pointer to the triplelock
* output:pNestedLock   - the number of locks need to be done after the cb.
*        pCurrThreadId - return thread id which will be retored after the cb
*        pThreadIdLockCount - the lock count of when the thread id was updated.
***************************************************************************/
void SipCommonUnlockBeforeCallback(IN    RvLogMgr      *pLogMgr,
                                   IN    RvLogSource   *pLogSrc,
                                   IN    SipTripleLock *pTripleLock,
                                   OUT   RvInt32       *pNestedLock,
                                   OUT   RvThreadId    *pCurrThreadId,
                                   OUT   RvInt32       *pThreadIdLockCount);

/***************************************************************************
* SipCommonReturnFromCallback
* ------------------------------------------------------------------------
* General:- This function will ensure that the element is locked again
*           with the specified number of times when the server returns from
*           callback.
*           Since we are locking back the object lock, the function restore
*           the tripleLock->objLockThreadId.
* ------------------------------------------------------------------------
* Arguments:
* input: pLogMgr       - logMgr
*        pTripleLock   - Handle to the object that need to be lock after
*                        the cb.
*        nestedLock    - the number of locks need to be done after the cb.
*        currThreadId  - thread id to restore.
*        threadIdLockCount - lock count to restore.
***************************************************************************/
void SipCommonLockAfterCallback(IN RvLogMgr      *pLogMgr,
                                IN SipTripleLock *pTripleLock,
                                IN RvInt32        nestedLock,
                                IN RvThreadId     currThreadId,
                                IN RvInt32        threadIdLockCount);

/***************************************************************************
* SipCommonFindStrInMemBuff
* ------------------------------------------------------------------------
* General:  This function finds a string in a memory buffer that doesn't 
*           necessarily terminate with '\0'.
* 
* Returns: The position of the string in the memory buffer or NULL in case
*          it's not found. 
* ------------------------------------------------------------------------
* Arguments:
* input: memBuff     - Pointer to the memory buffer.
*        buffLen     - The memory buffer length.
*        strSearched - Pointer to the searched string.
***************************************************************************/
RvChar *RVCALLCONV SipCommonFindStrInMemBuff(
                                IN RvChar   *memBuff,
                                IN RvUint32  buffLen,
                                IN RvChar   *strSearched);

/***************************************************************************
* SipCommonReverseFindStrInMemBuff
* ------------------------------------------------------------------------
* General:  This function finds reversely a string in a memory buffer
*            that doesn't necessarily terminate with '\0'.
* 
* Returns: The position of the string in the memory buffer or NULL in case
*          it's not found. 
* ------------------------------------------------------------------------
* Arguments:
* input: memBuff     - Pointer to the memory buffer.
*        buffLen     - The memory buffer length.
*        strSearched - Pointer to the searched string.
***************************************************************************/
RvChar *RVCALLCONV SipCommonReverseFindStrInMemBuff(
                                    IN RvChar   *memBuff,
                                    IN RvUint32  buffLen,
                                    IN RvChar   *strSearched);

/***************************************************************************
* SipCommonMemBuffExactlyMatchStr
* ------------------------------------------------------------------------
* General:  This function checks if the memory buffer exactly match the 
*           given string.
* 
* Returns: RvBool
* ------------------------------------------------------------------------
* Arguments:
* input: memBuff     - Pointer to the memory buffer.
*        buffLen     - The memory buffer length.
*        strCompared - Pointer to the compared string.
***************************************************************************/
RvBool RVCALLCONV SipCommonMemBuffExactlyMatchStr(
                                  IN RvChar   *memBuff,
                                  IN RvUint32  buffLen,
                                  IN RvChar   *strCompared);

/***************************************************************************
 * SipCommonPower
 * ------------------------------------------------------------------------
 * General: Computes 2 to the power of the retransmissions count. This
 *          result will be used to determine the next timer set.
 * Return Value: 2 to the power of retransmissionCount.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: retransmissionCount - The number of retransmissions already sent.
 ***************************************************************************/
RvInt32 RVCALLCONV SipCommonPower(RvUint32 retransmissionCount);

#ifdef __cplusplus
}
#endif


#endif /* END OF: #ifndef SIP_COMMON_UTILS_H */

