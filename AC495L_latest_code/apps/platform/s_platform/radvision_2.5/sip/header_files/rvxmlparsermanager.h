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
 *                              <rvxmlparsermanager.h>
 *    This file contains the initialization of the RvXmlParser module.
 *
 *********************************************************************************/


#ifndef RVXMLPARSERMANAGER_H
#define RVXMLPARSERMANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "rvlog.h"
#include "rvxmlparser.h"

/*-----------------------------------------------------------------------*/
/*                          TYPE DEFINITIONS                             */
/*-----------------------------------------------------------------------*/

/* declaring the handle of the DNS list object */
RV_DECLARE_HANDLE(RvXmlParserMgrHandle);


/********************************************************************************************
 * RvXmlParserType enumeration.
 * There are three types of parser instances that can be used with a manager.
 * Low consumption memory type parser instances.
 * Medium memory consumption type parser instances and;
 * High memory consumptions type parser instances.
 * The semantics of Low, Medium and High memory consumption are derived from the 
 * initialization of the RvXmlParserMgrCfg instance used in order to construct the 
 * RvXmlParserMgr. This is up to the using application.
 ********************************************************************************************/
 /*enum RvXmlParserType RvXmlParserType;*/
typedef enum RvXmlParserType_ {
	RVXMLPARSER_TYPE_LOW_MEM = 0,
	RVXMLPARSER_TYPE_MEDIUM_MEM,
	RVXMLPARSER_TYPE_HIGH_MEM

} RvXmlParserType; /* enum RvXmlParserType */

#define RVXMLPARSER_MAX_PARSER_TYPES 3 /*Size of the RvXmlParserType enumeration.*/

/*********************************************************************** 
 * RvXmlParserMgrCfg is a configuration structure used to construct the 
 * RvXmlParserMgr.
 ***********************************************************************/
typedef struct RvXmlParserMgrCfg RvXmlParserMgrCfg;
struct RvXmlParserMgrCfg
{
    RvLogMgr*       pLogMgr; 
    RvLogSource*    pLogSrc;
	/*Array of RvXmlParserCfg instances for each type of parser instance 
	(Low, Medium and High memory consumption).*/
	RvXmlParserCfg	parserCfg[RVXMLPARSER_MAX_PARSER_TYPES];
	/*Number of low memory consumption parser instances to allocate.*/
    RvUint32		numOfLowMemParsers;
	/*Number of medium memory consumption parser instances to allocate.*/
	RvUint32		numOfMediumMemParsers;
	/*Number of high memory consumption parser instances to allocate.*/
	RvUint32		numOfHighMemParsers;
};



/***************************************************************************
 * RvXmlParserMgrConstruct
 * ------------------------------------------------------------------------
 * General: Construct a parser manager instance and allocate all related 
 *			resources.
 * Return Value: RvStatus  - return status RV_OK upon success and various error codes upon error.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:		sizeOfCfg - Disregarded.
 *				pCfg  - Pointer to a valid RvXmlParserMgrCfg instance with relevant configuration parameters.
 *				phParserMgr - Pointer to an uninitialized parser manager handle. Which will be
 *				initialized after a successful call to this function.
 * Output:		phParserMgr  - Pointer to an initialized parser manager handle.
 * Disposition: Synchronous.
 ***************************************************************************/
RVAPI RvStatus RVCALLCONV RvXmlParserMgrConstruct(	IN RvInt32 sizeOfCfg,
												IN RvXmlParserMgrCfg*     pCfg,
												OUT RvXmlParserMgrHandle* phParserMgr);

/***************************************************************************
 * RvXmlParserMgrDestruct
 * ------------------------------------------------------------------------
 * General: Destroys the parser manager instance and frees all resources owned by it.
 * Return Value: None.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:     hParserMgr - Handle to the parser manager retrieved from a successful call to 
 *			  RvXmlParserMgrConstruct.
 ***************************************************************************/
RVAPI void RVCALLCONV RvXmlParserMgrDestruct(IN RvXmlParserMgrHandle hParserMgr);

/***************************************************************************
 * RvXmlParserNumberAvailable
 * ------------------------------------------------------------------------
 * General: Gets the number of available parser instances of type eParserType
 *			in the current parser pool.
 * Return Value: RvUint32 - Number of available parser instances of type eParserType
 *			in the current parser pool.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:     hParserMgr - Handle to the parser manager retrieved from a successful call to 
 *			  RvXmlParserMgrConstruct.
 *			  eParserType - parser type
 ***************************************************************************/
RVAPI RvUint32 RVCALLCONV RvXmlParserNumberAvailable(IN RvXmlParserMgrHandle hParserMgr,
													 IN	RvXmlParserType eParserType);

/***************************************************************************
 * RvXmlParserGetParserHandle
 * ------------------------------------------------------------------------
 * General: Gets a handle to a specific parser instance. 
 * Return Value: RvStatus  - return status RV_OK upon success and various error codes upon error.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:	  hParserMgr - Handle to the parser manager retrieved from a successful call to 
 *			  RvXmlParserMgrConstruct.
 *			  eParserType - parser type.
 *			  phParser - Pointer to an uninitialized parser handle. Which will be
 *			  initialized after a successful call to this function.	  
 * Output:	  phParser  - initialized parser instance handle.
 ***************************************************************************/
RVAPI RvStatus RVCALLCONV RvXmlParserGetParserHandle(	IN RvXmlParserMgrHandle hParserMgr,
														IN RvXmlParserType eParserType,
														OUT RvXmlParserHandle* phParser);


/***************************************************************************
 * RvXmlParserReleaseParserHandle
 * ------------------------------------------------------------------------
 * General: Releases a give parser instance within the pool and renders it available again
 *			for future use within the life-time scope of the manager instance.
 * Return Value: RvStatus  - return status RV_OK upon success and various error codes upon error.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input:	  hParserMgr - Handle to the parser manager retrieved from a successful call to 
 *			  RvXmlParserMgrConstruct.
 *			  hParser - Handle to a parser instance retrieved by a successful call to RvXmlParserGetParserHandle.
 ***************************************************************************/
RVAPI RvStatus RVCALLCONV RvXmlParserReleaseParserHandle(IN RvXmlParserMgrHandle hParserMgr,
														 IN RvXmlParserHandle hParser);


#ifdef __cplusplus
}
#endif

#endif  /*END OF: define RVXMLPARSERMANAGER_H*/
