/******************************************************************************
Filename:    rvcctext.h
Description: Maps states and events id to text
*******************************************************************************
                Copyright (c) 2001 RADVISION
*******************************************************************************
NOTICE:
This document contains information that is proprietary to RADVISION.
No part of this publication may be reproduced in any form whatsoever
without written prior approval by RADVISION.

RADVISION reserves the right to revise this publication and make changes
without obligation to notify any person of such revisions or changes.
******************************************************************************/

#ifndef RVCCTEXT_H
#define RVCCTEXT_H

#include "rvccapi.h"
#include "rvcccall.h"
#include "rvccCfwMgr.h"


/********************************************************************************************
 *                                rvcctext.h
 *
 * This module is responsible for changing various IPP enumerations to strings.
 *
 *
 ********************************************************************************************/


/******************************************************************************
*  rvCCTextCause
*  ----------------------------
*  General :      Returns a string representing the Event cause
*
*  Return Value:  constant string
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          id                  RvCCEventCause
*       .
*
*  Output          None.
*
*
*
******************************************************************************/
RVAPI const RvChar* rvCCTextCause(IN RvCCEventCause id);


/******************************************************************************
*  rvCCTextEvent
*  ----------------------------
*  General :       Returns a string representing the TerminalEvent
*
*  Return Value:   constant string
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          id                 Terminal Event
*       .
*
*  Output          None.
*
*
*
******************************************************************************/
RVAPI const RvChar* rvCCTextEvent(IN RvCCTerminalEvent id);



/******************************************************************************
*  rvCCTextConnState
*  ----------------------------
*  General :       Returns a string representing the ConnState
*
*  Return Value:   constant string
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          id                 Connection State
*       .
*
*  Output          None.
*
*
*
******************************************************************************/
RVAPI const RvChar* rvCCTextConnState(IN RvCCConnState id);



/******************************************************************************
*  RvCCTermConnState
*  ----------------------------
*  General :       Returns a string representing the TermConnState
*
*  Return Value:   constant string
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          id                 Terminal ConnectionState
*       .
*
*  Output          None.
*
*
*
******************************************************************************/
RVAPI const RvChar* rvCCTextTermConnState(IN RvCCTermConnState id);


/******************************************************************************
*  rvCCTextCallState
*  ----------------------------
*  General :       Returns a string representing the Call state
*
*  Return Value:   constant string
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          id                 Call State
*       .
*
*  Output          None.
*
*
*
******************************************************************************/
RVAPI const RvChar* rvCCTextCallState(IN RvCCCallState id);



/******************************************************************************
*  rvCCTextTermType
*  ----------------------------
*  General :       Returns a string representing the Terminal type
*
*  Return Value:   constant string
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          id                 Terminal Type
*       .
*
*  Output          None.
*
*
*
******************************************************************************/
RVAPI const RvChar* rvCCTextTermType(IN RvCCTerminalType id);

/******************************************************************************
*  rvCCTextConnectionType
*  ----------------------------
*  General :       Returns a string representing the Connection type
*
*  Return Value:   constant string
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          id                 Connection Type
*       .
*
*  Output          None.
*
*
*
******************************************************************************/
RVAPI const RvChar* rvCCTextConnectionType(IN RvCCConnType id);



/******************************************************************************
*  rvCCTextMediaState
*  ----------------------------
*  General :       Returns a string representing the MediaState
*
*  Return Value:   constant string
*
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          id                 Media State
*       .
*
*  Output          None.
*
*
*
******************************************************************************/
RVAPI const RvChar* rvCCTextMediaState(IN RvCCMediaState state);

/******************************************************************************
*  rvCCTextModifyMediaReason
*  ----------------------------
*  General :       Returns a string representing the Reason of Modify Media
*
*  Return Value:   constant string
*
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          id                 Modify Media Reason
*       .
*
*  Output          None.
*
*
*
******************************************************************************/
RVAPI const RvChar* rvCCTextModifyMediaReason(IN RvMdmTermReasonModifyMedia id);


/******************************************************************************
*  rvCCTextTerminalAudioType
*  ----------------------------
*  General :       Returns a string representing the terminal Audio Type
*  Return Value:   constant string
*
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          atType                 Audio Terminal Type
*       .
*
*  Output          None.
*
*
*
******************************************************************************/
RVAPI const RvChar* rvCCTextTerminalAudioType(IN RvCCTerminalAudioType atType);

/******************************************************************************
*  rvCCTextConnTransferState
*  ----------------------------
*  General :       Returns a string representing the connection Transfer State
*  Return Value:   constant string
*
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          transState                 connection Transfer State
*       .
*
*  Output          None.
*
*
*
******************************************************************************/
RVAPI const RvChar* rvCCTextConnTransferState(IN RvCCConnTransferState transState);

/******************************************************************************
*  rvCCTextCfwType
*  ----------------------------
*  General :       Returns a string representing the Call Forward type
*  Return Value:   constant string
*
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          type                 Call Forward type
*       .
*
*  Output          None.
*
*
*
******************************************************************************/
RVAPI const RvChar* rvCCTextCfwType(IN RvIppCfwType type);

/******************************************************************************
*  rvCCTextCfwState
*  ----------------------------
*  General :       Returns a string representing the Call Forward state
*  Return Value:   constant string
*
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          state                 Call Forward state
*       .
*
*  Output          None.
*
*
*
******************************************************************************/
RVAPI const RvChar* rvCCTextCfwState(IN RvCCCfwState state);

/******************************************************************************
*  rvCCTextCfwReason
*  ----------------------------
*  General :       Returns a string representing the Call Forward reason
*  Return Value:   constant string
*
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          reason                 Call Forward reason
*       .
*
*  Output          None.
*
*
*
******************************************************************************/
RVAPI const RvChar* rvCCTextCfwReason(IN RvIppCfwReturnReasons reason);

/******************************************************************************
*  rvCCTextStreamMode
*  ----------------------------
*  General :       Returns a string representing the stream mode
*  Return Value:   constant string
*
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          mode                 stream mode
*       .
*
*  Output          None.
*
*
*
******************************************************************************/
RVAPI const RvChar* rvCCTextStreamMode(IN RvMdmStreamMode mode);


#endif /*RVCCTEXT_H*/





















