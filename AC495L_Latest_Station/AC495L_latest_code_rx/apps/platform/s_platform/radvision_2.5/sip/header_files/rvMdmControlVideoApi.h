/******************************************************************************
Filename:    rvMdmControlVideoApi.h
Description: MDM Control VideoAPIs
*******************************************************************************
                Copyright (c) 2005 RADVISION
*******************************************************************************
NOTICE:
This document contains information that is proprietary to RADVISION.
No part of this publication may be reproduced in any form whatsoever
without written prior approval by RADVISION.

RADVISION reserves the right to revise this publication and make changes
without obligation to notify any person of such revisions or changes.
******************************************************************************/
#ifndef RV_MDMCONTROLVIDEO_API_H
#define RV_MDMCONTROLVIDEO_API_H

#include "rvCallControlApi.h"
#include "rvmdmmediastream.h"


/**********************************************************************************
				E X T E N S I O N	VIDEO	A P I		D E F I N I T I O N S
**********************************************************************************/

/******************************************************************************
*  RvIppMdmVideoFastUpdatePictureCB
*  ----------------------------
*  General :     Prototype for Fast Update Picture CallBack.
*                Enables user application to pass Fast Update Picture command to encoder.
*
*  Return Value: None.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:        connHndl			connection Handle
*                terminalHndl		terminal   Handle
******************************************************************************/
typedef void (*RvIppMdmVideoFastUpdatePictureCB)(
									RvIppTerminalHandle	    uiTerminalHndl,
									RvIppTerminalHandle		terminalHndl);


/******************************************************************************
*  RvIppMdmVideoFastUpdateGobCB
*  ----------------------------
*  General :     Prototype for Fast Update GOB (Group Of Blocks) CallBack.
*                Enables user application to pass Fast Update GOB command to encoder.
*
*  Return Value: None.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:        connHndl			connection Handle
*                terminalHndl		terminal   Handle
*                firstGOB			first GOB 			
*                numOfGOBs			number of GOB to update
******************************************************************************/
typedef void (*RvIppMdmVideoFastUpdateGobCB)(
									RvIppTerminalHandle	    uiTerminalHndl,
									RvIppTerminalHandle		terminalHndl,
									RvUint					firstGOB,
									RvUint					numOfGOB);


/******************************************************************************
*  RvIppMdmVideoFastUpdateMbCB
*  ----------------------------
*  General :     Prototype for Fast Update MB (Multi Blocks) CallBack.
*                Enables user application to pass Fast Update GB command to encoder.
*
*  Return Value: None.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:        connHndl			connection Handle
*                terminalHndl		terminal   Handle
*                firstGOB			first GOB 			
*                firstMV			first MB 			
*                numOfMBs			number of MB to update
******************************************************************************/
typedef void (*RvIppMdmVideoFastUpdateMbCB)(
									RvIppTerminalHandle	    uiTerminalHndl,
									RvIppTerminalHandle		terminalHndl,
									RvUint					firstGOB,
									RvUint					firstMB,
									RvUint					numOfMB);


/******************************************************************************
*  RvIppMdmVideoFastUpdatePictureFreezeCB
*  ----------------------------
*  General :     Prototype for Fast Update Picture Freeze CallBack.
*                Enables user application to pass Fast Update Picture Freeze
*                command to decoder.
*
*  Return Value: None.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:        connHndl			connection Handle
*                terminalHndl		terminal   Handle
******************************************************************************/
typedef void (*RvIppMdmVideoFastUpdatePictureFreezeCB)(
									RvIppTerminalHandle	    uiTerminalHndl,
									RvIppTerminalHandle		terminalHndl);


/**********************************************************************************
				E X T E N S I O N	VIDEO	A P I		S T R U C T U R E
**********************************************************************************/

typedef struct RvIppMdmVideoExtClbks_ {
	RvIppMdmVideoFastUpdatePictureCB		fastUpdatePicture;
	RvIppMdmVideoFastUpdateGobCB		    fastUpdateGob;
	RvIppMdmVideoFastUpdateMbCB				fastUpdateMb;
	RvIppMdmVideoFastUpdatePictureFreezeCB	fastUpdatePictureFreeze;
} RvIppMdmVideoExtClbks;

/******************************************************************************
*  rvIppMdmVideoRegisterExtClbks
*  ----------------------------
*  General :        Function used for registering MDM Video extension callbacks.
*
*
*  Return Value:    None.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          clbks			pointer to extension callback structure
*
*
*  Output          None.
*
******************************************************************************/
RVAPI void rvIppMdmVideoRegisterExtClbks(RvIppMdmVideoExtClbks* clbks);


/**********************************************************************************
					P R O V I D E R			A P I s
**********************************************************************************/

/******************************************************************************
*  rvIppMdmSendFastUpdatePicture
*  -------------------------------------
*  General :        Send FastUpdatePicture command to "other" side camera 
*
*  Return Value:    RV_OK if the command was send successfully,
*					RV_ERROR if not.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          	uiTerminalHndl -		UI Terminal Handle
****************************************************************************/
RVAPI RvStatus rvIppMdmSendFastUpdatePicture(RvIppTerminalHandle	uiTerminalHndl);

/******************************************************************************
*  rvIppMdmSendFastUpdateGOB
*  -------------------------------------
*  General :        Send FastUpdate Group Of Blocks command to "other" side camera 
*
*  Return Value:    RV_OK if the command was send successfully,
*					RV_ERROR if not.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          	uiTerminalHndl -		UI Terminal Handle
*                   firstGob -  first block number
*                   numGobs  -  the number of blocks to update
****************************************************************************/
RVAPI RvStatus rvIppMdmSendFastUpdateGOB(RvIppTerminalHandle	uiTerminalHndl,
									RvUint firstGob,
									RvUint numGobs);

/******************************************************************************
*  rvIppMdmSendFastUpdateMB
*  -------------------------------------
*  General :        Send FastUpdate Macro Blocks command to "other" side camera 
*
*  Return Value:    RV_OK if the command was send successfully,
*					RV_ERROR if not.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          	uiTerminalHndl -		UI Terminal Handle
*                   firstGob -  first block number to update
*                   firstMb  -  first multi block 
*                   numGobs  -  the number of macro blocks to update
****************************************************************************/
RVAPI RvStatus rvIppMdmSendFastUpdateMB(RvIppTerminalHandle	uiTerminalHndl,
									RvUint firstGob,
									RvUint firstMb,
									RvUint numMbs);

/******************************************************************************
*  rvIppMdmSendFastUpdatePictureFreeze
*  -------------------------------------
*  General :        Send FastUpdatePictureFreeze command to "other" side screen 
*
*  Return Value:    RV_OK if the command was send successfully,
*					RV_ERROR, if not.
*
*  ----------------------------------------------------------------------------
*  Arguments:
*  Input:          	uiTerminalHndl -		UI Terminal Handle
****************************************************************************/
RVAPI RvStatus rvIppMdmSendFastUpdatePictureFreeze(RvIppTerminalHandle	uiTerminalHndl);


#endif /*RV_MDMCONTROL_API_H*/
