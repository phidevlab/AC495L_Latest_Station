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
***********************************************************************/

#ifndef _RV_IPPLOG_H
#define _RV_IPPLOG_H

#include "rvtypes.h"
#include "rverror.h"
#include "rvlog.h"

#ifdef __cplusplus
extern "C" {
#endif


    /* Prototypes and macros */



enum    IppLogSourceEnum
{
    LOGSRC_USERAPP      ,
    LOGSRC_CALLCONTROL  ,
    LOGSRC_MDM          ,
    LOGSRC_MDMCONTROL   ,
    LOGSRC_SIPCONTROL   ,
    LOGSRC_VIDEOPHONE   ,
    LOGSRC_UTIL         ,
    LOGSRC_RPOOL        ,
    LOGSRC_RA           ,
    LOGSRC_CODEC        ,
	LOGSRC_BASE			,
    LOGSRC_NUMBER
};

typedef struct
{
    RvLogSource*    logSrc;
    const char*     logSrcName;
} IppLogSourceElm;




/* IppLogSourceFilterElm
 * ------------------------------------------------------------------------
 * A log source filter to be show in the log file of the MTF.
 * This struct is used as an array of elements passed to IppLogInit().
 * When done in this manner, to indicate the size of the array, the last
 * element's logSrcName field should be set to an empty string.
 */
typedef struct
{
    RvChar              logSrcName[20];
        /* This field indicates the module name. For a list of possible
           names check the Logging chapter in the Programmer's Guide. */

    RvLogMessageType    messageMask;
        /* This field indicates the logging mask of the module. For a
           list of possible log masks check the Logging chapter in the
           Programmer's Guide. */
} IppLogSourceFilterElm;


/******************************************************************************
 * IppLogMgr
 * ----------------------------------------------------------------------------
 * General:
 *  Return MTF Log manager
 *
 * Arguments:
 * Input:  None
 * Output: None.
 *
 * Return Value: Log manager on success, NULL on failure.
 *****************************************************************************/
RVAPI RvLogMgr* IppLogMgr(void);

/******************************************************************************
 * IppLogInit
 * ----------------------------------------------------------------------------
 * General:
 *  Initialize the MTF's logging.
 *  This function should be called once, during the construction of an MTF
 *  instance.
 *
 * Arguments:
 * Input:  ippFilters   - An array of log filters to include in the log file.
 *                        The array must end with an element that has an empty
 *                        name for its log source.
 * Output: None.
 *
 * Return Value: RV_OK on success, other on failure.
 *****************************************************************************/
RVAPI RvStatus RVCALLCONV IppLogInit(
    IN IppLogSourceFilterElm*   ippFilters,
    IN const RvChar*            szLogFileName);

/******************************************************************************
 * IppLogEnd
 * ----------------------------------------------------------------------------
 * General:
 *  Ends the use of the log in the MTF.
 *  This function must be called last, after the MTF has been terminated.
 * Arguments:
 * Input:  None
 * Output: None.
 *
 * Return Value: RV_OK on success, other on failure.
 *****************************************************************************/
RVAPI RvStatus RVCALLCONV IppLogEnd(void);


/******************************************************************************
 * IppLogReload
 * ----------------------------------------------------------------------------
 * General:
 *  Reset the log sources that are used for logging.
 *  This function cannot be called before IppLogInit() is called.
 *
 * Arguments:
 * Input:  ippFilters   - An array of log filters to include in the log file.
 *                        The array must end with an element that has an empty
 *                        name for its log source.
 * Output: None.
 *
 * Return Value: RV_OK on success, other on failure.
 *****************************************************************************/
RVAPI RvStatus RVCALLCONV IppLogReload(IN IppLogSourceFilterElm* ippFilters);

/******************************************************************************
 * IppLogMessage
 * ----------------------------------------------------------------------------
 * General:
 *  Print a message into the MTF's log.
 *  The message will be printed under the IPP_USERAPP source.
 * Arguments:
 * Input:  isError  - RV_TRUE for an error message, RV_FALSE for an information
 *                    message.
 *         message  - The message itself. This string is handled in the same
 *                    manner of the ANSI C printf() function.
 * Output: None.
 *
 * Return Value: None.
 *****************************************************************************/
RVAPI void RVCALLCONV IppLogMessage(
    IN RvBool               isError,
    IN const RvChar*        message, ...);

#undef ippLogSource
#if (RV_LOGMASK != RV_LOGLEVEL_NONE)

#define ippLogSource &_logSrc[LOGSRC]
extern IppLogSourceElm logSrcTable[LOGSRC_NUMBER];
extern RvLogSource _logSrc[LOGSRC_NUMBER];

#else
#define ippLogSource NULL

#endif
/*
*   Every file should initialize pLogSource as a pointer to RvLogSource object of appropriate module
*/
#ifdef __cplusplus
}
#endif

#endif  /* _RV_IPPLOG_H */

