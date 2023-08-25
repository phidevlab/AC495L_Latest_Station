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

/*@*****************************************************************************
 * Package: RvMtfVideoPkg (root)
 * -----------------------------------------------------------------------------
 * Title: Video
 *
 * General: This section contains functions and type definitions that are used 
 *  		for sending and receiving Fast Update commands.
 ****************************************************************************@*/

/**********************************************************************************
					V I D E O	A P I		D E F I N I T I O N S
**********************************************************************************/

/*@*****************************************************************************
 * RvIppMdmVideoFastUpdatePictureCB (RvMtfVideoPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         This callback is called when a Fast Update Picture is received
 *		   from the remote party. It enables the application to pass the
 *         parameters to the decoder.
 *
 * Arguments:
 *
 * Input:	ephemeralTerminalHndl   - Handle to the terminal representing
 *                                    the media stream.
 *			uiTerminalHndl			- Handle to the terminal representing
 *                                    the user interface.
 *
 * Return Value: None.
 ****************************************************************************@*/
typedef void (*RvIppMdmVideoFastUpdatePictureCB)(
									 RvIppTerminalHandle	    ephemeralTerminalHndl,
									 RvIppTerminalHandle	    uiTerminalHndl);

/*@*****************************************************************************
 * RvIppMdmVideoFastUpdateGobCB (RvMtfVideoPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         This callback is called when a Fast Update GOB (Group Of Blocks)
 *		   is received from the remote party. It enables the application to pass
 *         the parameters to the decoder.
 *
 * Arguments:
 *
 * Input:	ephemeralTerminalHndl   - Handle to the terminal representing the
 *                                    media stream.
 *			uiTerminalHndl			- Handle to the terminal representing the
 *                                    user interface.
 *			firstGOB				- The first GOB (Group Of Blocks).
 *          numOfGOB				- The number of GOBs (Group Of Blocks).
 *
 * Return Value: None.
 ****************************************************************************@*/
typedef void (*RvIppMdmVideoFastUpdateGobCB)(
									RvIppTerminalHandle	    ephemeralTerminalHndl,
									RvIppTerminalHandle	uiTerminalHndl,
									RvUint					firstGOB,
									RvUint					numOfGOB);

/*@*****************************************************************************
 * RvIppMdmVideoFastUpdateMbCB (RvMtfVideoPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         This callback is called when a Fast Update MB (Multi Blocks)
 *		   is received from the remote party. It enables the application to pass
 *         the parameters to the decoder.
 *
 * Arguments:
 *
 * Input:	ephemeralTerminalHndl   - Handle to the terminal representing the
 *                                    media stream.
 *			uiTerminalHndl			- Handle to the terminal representing the
 *                                    user interface.
 *			firstGOB				- The first GOB (Group Of Blocks).
 *          firstMV					- The first MB (Multi Blocks).
 *          numOfMBs				- The number of MB (Multi Blocks) to be
 *                                    updated.
 *
 * Return Value: None.
 ****************************************************************************@*/
typedef void (*RvIppMdmVideoFastUpdateMbCB)(
									RvIppTerminalHandle	    ephemeralTerminalHndl,
									RvIppTerminalHandle		uiTerminalHndl,
									RvUint					firstGOB,
									RvUint					firstMB,
									RvUint					numOfMB);

/*@*****************************************************************************
 * RvIppMdmVideoFastUpdatePictureFreezeCB (RvMtfVideoPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         This callback is called when a Fast Update Picture Freeze is received
 *		   from the remote party. It enables the application to pass the
 *         parameters to the decoder.
 *
 * Arguments:
 *
 * Input:  ephemeralTerminalHndl  - Handle to the terminal representing the
 *                                  media stream.
 *         uiTerminalHndl		  - Handle to the terminal representing the
 *                                  user interface.
 *
 * Return Value: None.
 ****************************************************************************@*/
typedef void (*RvIppMdmVideoFastUpdatePictureFreezeCB)(
									RvIppTerminalHandle	    ephemeralTerminalHndl,
									 RvIppTerminalHandle	uiTerminalHndl);


/*@*****************************************************************************
 * Type: RvIppMdmVideoExtClbks (RvMtfVideoPkg)
 * -----------------------------------------------------------------------------
 * This structure includes video callbacks for sending and receiving a Fast
 * Update.
 ****************************************************************************@*/
typedef struct RvIppMdmVideoExtClbks_ {
	RvIppMdmVideoFastUpdatePictureCB		fastUpdatePicture;
	RvIppMdmVideoFastUpdateGobCB		    fastUpdateGob;
	RvIppMdmVideoFastUpdateMbCB				fastUpdateMb;
	RvIppMdmVideoFastUpdatePictureFreezeCB	fastUpdatePictureFreeze;
} RvIppMdmVideoExtClbks;

/*@*****************************************************************************
 * rvIppMdmVideoRegisterExtClbks (RvMtfVideoPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         Registers video callbacks to the MTF. It should be called after
 *         rvMtfSipConstruct() is called.
 *
 * Arguments:
 *
 * Input:  clbks     - Pointer to a structure containing video callbacks for
 *                     Fast Update.
 *
 * Return Value: None.
 ****************************************************************************@*/
RVAPI void RVCALLCONV rvIppMdmVideoRegisterExtClbks(
						IN RvIppMdmVideoExtClbks* clbks);



/*@*****************************************************************************
 * rvIppMdmSendFastUpdatePicture (RvMtfVideoPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         Sends a FastUpdatePicture command to the remote party.
 *
 * Arguments:
 *
 * Input:  uiTerminalHndl     - Handle to the termination.
 *
 * Return Value: RV_OK if successful, other if not.
 ****************************************************************************@*/
RVAPI RvStatus rvIppMdmSendFastUpdatePicture(
									RvIppTerminalHandle	uiTerminalHndl);

/*@*****************************************************************************
 * rvIppMdmSendFastUpdateGOB (RvMtfVideoPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         Sends a Fast Update GOB (Group Of Blocks) command to the remote party.
 *
 * Arguments:
 *
 * Input:  uiTerminalHndl   -  Handle to the termination.
 *         firstGob			-  The first block number.
 *         numGobs			-  The number of blocks to be updated.
 *
 * Return Value: RV_OK if successful, other if not.
 ****************************************************************************@*/
RVAPI RvStatus rvIppMdmSendFastUpdateGOB(
									RvIppTerminalHandle		uiTerminalHndl,
									RvUint					firstGob,
									RvUint					numGobs);

/*@*****************************************************************************
 * rvIppMdmSendFastUpdateMB (RvMtfVideoPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         Sends a Fast Update Macro Blocks command to the remote party.
 *
 * Arguments:
 *
 * Input:  uiTerminalHndl   -  Handle to the termination.
 *         firstGob			-  The first block number to be updated.
 *         firstMb			-  The first multi block.
 *         numGobs			-  The number of macro blocks to be updated.
 *
 * Return Value: RV_OK if successful, other if not.
 ****************************************************************************@*/
RVAPI RvStatus rvIppMdmSendFastUpdateMB(
									RvIppTerminalHandle		uiTerminalHndl,
									RvUint					firstGob,
									RvUint					firstMb,
									RvUint					numMbs);

/*@*****************************************************************************
 * rvIppMdmSendFastUpdatePictureFreeze (RvMtfVideoPkg)
 * -----------------------------------------------------------------------------
 * General:
 *         Sends a FastUpdatePictureFreeze command to the remote party.
 *
 * Arguments:
 *
 * Input:  uiTerminalHndl   -  Handle to the termination.
 *
 * Return Value: RV_OK if successful, other if not.
 ****************************************************************************@*/
RVAPI RvStatus rvIppMdmSendFastUpdatePictureFreeze(
									RvIppTerminalHandle	uiTerminalHndl);


#endif /*RV_MDMCONTROL_API_H*/
