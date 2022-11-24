/***********************************************************************
Filename   : _rvxmlparsertypes.h
Description: rvXmlParser internal types header file

************************************************************************
      Copyright (c) 2005 RADVISION Inc. and RADVISION Ltd.
************************************************************************
NOTICE:
This document contains information that is confidential and proprietary
to RADVISION Inc. and RADVISION Ltd.. No part of this document may be
reproduced in any form whatsoever without written prior approval by
RADVISION Inc. or RADVISION Ltd..

RADVISION Inc. and RADVISION Ltd. reserve the right to revise this
publication and make changes without obligation to notify any person of
such revisions or changes.
***********************************************************************/

#ifndef __RV_XMLPARSER_TYPES_H
#define __RV_XMLPARSER_TYPES_H

#include "rvtypes.h"
#include "rverror.h"
#include "rvlog.h"

#if defined(__cplusplus)
extern "C" {
#endif 

#include "rvxmlparser.h" 

typedef struct ParserStruct RvXmlParser; 
#define RVXMLPARSER( x ) ((RvXmlParser*) x )
 

/********************************************************************************************
 * ParserStruct
 *  An XML Parser object.
 *  This structure is private and is not externalized to the user application level.
 ********************************************************************************************/
typedef struct ParserStruct
{
	/*Callback functions pointers.*/
	RvXmlParserStartElementHandler StartElementFn; 
	RvXmlParserEndElementHandler EndElementFn;
	RvXmlParserCharacterDataHandler CharacterDataFn;	
	RvXmlParserCommentHandler CommentFn;
	RvXmlParserStartDoctypeDeclHandler StartDoctypeFn;
	RvXmlParserEndDoctypeDeclHandler EndDoctypeFn;
	RvXmlParserXmlErrorHandler ErrorHandlerFn;
	RvXmlParserProcessingInstructionHandler ProcessingInstructionFn;
	RvXmlParserDtdElementDeclHandler ElementDeclFn;
#ifdef RVXML_WITH_VALIDATION
	/*Pointer to an XML validator object of type RvXmlValidator. Optional - Can be NULL.*/
	RvXmlValidator* validatorObj; 
	RvBool doValidation;
#endif
	void* userData; /*Opeque user data.*/
	RvUint32 maxAttributesNum; /*Maximum numer of attributes per element.*/
	RvUint32 maxStackDepth; /*Maximum document depth.*/

	/* private parameters */
	RvXmlAttributeStruct* attributeSpace;
	RvUint32 AttributesNum;
	RvXmlNodeStruct* nodeStack;
	RvUint32 stackDepth;

	RvLogSource*                  pLogSrc;
	RvLogMgr*                     pLogMgr;
	
	RvUint32 inUse;
};

/********************************************************************************************
 * RvXmlParserFullReset
 * Resets an XML Parser object internal state including callback functions.
 * INPUT: hParser - A valid parser handle.
 *		 - Synchronous.	
 */
void RVCALLCONV RvXmlParserFullReset(RvXmlParserHandle hParser);

RvBool RVCALLCONV RvXmlIsParserInUse(RvXmlParserHandle hParser);

void RVCALLCONV RvXmlSetParserInUse(RvXmlParserHandle hParser);

void RVCALLCONV RvXmlSetParserFree(RvXmlParserHandle hParser); 

/********************************************************************************************
 * RvXmlParserConstruct
 * Constructs an XML Parser object and returns an RvXmlParserHandle. Must be called before any other parser API.
 * INPUT   : parserCfg - Pointer to an RvXmlParserCfg owned by the caller and prefilled with data. 
 * RETURNS : A Handle to the XML parser instance upon success and NULL upon failure. 
 * NOTE    : - The object returned by this function is valid XML parser handle used in other XML parser APIs.
 *			   If the application requires parsing of multiple XML documents it is required to 
 *			   to create a parser instance for each one by calling this function.
 *			 - The Caller owns the returning parser handle.
 *			 - The Caller should free the handle and resources owned by it by calling
 *			   RvXmlParserDestruct.
 *			 - Synchronous.		
 */
RvXmlParserHandle RVCALLCONV RvXmlParserConstruct(IN RvXmlParserCfg* parserCfg);

/********************************************************************************************
 * RvXmlParserDestruct
 * Destroys an XML Parser object and released the resources owned by the parser instance.
 * INPUT: hParser - A valid parser handle obtained by calling RvXmlParserConstruct.
 * NOTE: - The handle is no longer usable after calling this function. Its usage after calling
 *		   this function will produce unpredicted results.
 *		 - Calling this function doesn't modifiy or release user data associated with the parser using .
 *		 - Synchronous.	
 */
void RVCALLCONV RvXmlParserDestruct(IN RvXmlParserHandle hParser);


#if defined(__cplusplus)
}
#endif 

#endif /* __RV_XMLPARSER_TYPES_H */
