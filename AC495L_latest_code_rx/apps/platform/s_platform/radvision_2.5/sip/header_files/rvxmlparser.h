/***********************************************************************
Filename   : rvxmlparser.h
Description: rvXmlParser main header file

************************************************************************
      Copyright (c) 2005 RADVISION Inc. and RADVISION Ltd.
************************************************************************
NOTICE:
This document contains information that is confidential and proprietary
to RADVISION Inc. and RADVISION Ltd. No part of this document may be
reproduced in any form whatsoever without written prior approval by
RADVISION Inc. or RADVISION Ltd.

RADVISION Inc. and RADVISION Ltd. reserve the right to revise this
publication and make changes without obligation to notify any person of
such revisions or changes.
***********************************************************************/

#ifndef RV_XMLPARSER_H
#define RV_XMLPARSER_H

#include "rvtypes.h"
#include "rverror.h"

#if defined(__cplusplus)
extern "C" {
#endif 

#include "rvxmlparsertypes.h"
#include "rvxmlparserversion.h"

/*********************************************************************** 
 * RvXmlParser is a stream-oriented non persistent result parser. 
 * This means that the parser has two fundamental attributes design to make it
 * fast (single pass) and compact:
 * (1)	The parser does not allocate heap memory during the parsing procedure.
 *		All heap memory is pre-allocated during the parser initialization. The allocation size
 *      is based on two parameters - The maximal nesting level of the tags and the maximal amount
 *		of attributes per tag.
 * (2)	The parser does not maintain a persistent state after the parsing is over. This means that
 *		the parsing action does not produce a complex data structure containing the result this is up to 
 *		user application. During parsing the parser calls a set of registered callback functions
 *		Referred to as "Handlers", which have a predefined signature (defined below in this file) and are
 *		implemented and registered to the parser by the caller application. The application gives the handlers
 *		and the incoming handler arguments the relevant applicative semantics. The application can then build
 *		any data structure and perform any action it wishes based on the data which is returned to it by the parser
 *		via the handlers.
 ***********************************************************************/

	
/************************************ User API **********************************************/

/***********************************************************************
 *Callback Hander Interface prototype functions.

 NOTES: 

 -	The user application is required to implement the callback handlers and apply the relevant
	applicative semantics to the handler and the incoming data.
 -	The user application must also register the handlers with the XML parser instance after receiving a
	valid parser instance from RvXmlParserConstruct and prior to invoking the parsing function RvXmlParse.
	If the user application does not set a handler the parser will make no attempt to call it.
 -  A handler may be un-set by calling the appropriate handler setter function and providing a NULL pointer argument.
	None of the handler setting functions have a return value.
 -  All handler functions are synchronous, so it is up to the implementer to verify that the functions complete 
 -  promptly and do not hang or delay the parsing process.
 -	In order to be able to pass information between different handlers without using non constant global
	variables, which is a limitation for DLLs in certain mobile platforms (eg. EPOC/Symbian versions 5 to 8.0a)- 
	the user MAY define a data structure which contains stateful persistent application related data, which may
	be required and modified during the parsing process. The parser instance itself is agnostic to this
	data structure and only passes it as a void pointer "handle" (void* userData)to most of the handler callbacks
	as the first argument.
***********************************************************************/


/********************************************************************************************
 * RvXmlParserXmlErrorHandler
 * This handler (Callback) is called when an error occures. 
 * INPUT   : userData - Handle to user-defined data structure.
 *           err - The type of error, which occured.
 *           buf - Pointer to the location in the XML file where the error occured.
 *				   May be NULL in certain error scenarios, so caution should be taken when using this pointer.
 * NOTE    : - Argument No. 3 - buf - Is guaranteed to be null terminated. 
 *			 - The method implementer owns userData.
 *			 - Synchronous.			 
  */
typedef void (*RvXmlParserXmlErrorHandler) (IN void* userData, IN RvXmlParserErrors err, IN const RvChar* buf);

/********************************************************************************************
 * RvXmlParserSetXmlErrorHandler
 * Sets a handler that will be called for all errors.
 * INPUT   : hParser - A valid parser handle obtained by calling RvXmlParserConstruct. 
 *           handler - Pointer to an RvXmlParserXmlErrorHandler function implementation.
 * NOTE    : - The caller owns all arguments.
 *			 - Synchronous.		
  */
RVAPI void  RVCALLCONV RvXmlParserSetXmlErrorHandler(IN RvXmlParserHandle hParser, IN RvXmlParserXmlErrorHandler handler);

/********************************************************************************************
 * RvXmlParserStartElementHandler
 * A handler for start (and empty) tags - called when a start (or empty) tag is encountered.
 * INPUT   : userData - Handle to user-defined data structure
 *           name - The tag name.
 *           nameSize - size of tag name. 				
 *           atts - Arrary of attribute structures (RvXmlAttributeStruct) or NULL if no attributes found. 
 *           attsNum - number of elements in attr array.
 * NOTE    : - The caller owns only userData.
 *			 - Synchronous.	
 *			 - name is a pointer to the start location of the element name in the parser internal buffer.
 *			   It is not gauranteed to be NULL terminated hence the need for nameSize.
  */
typedef void (*RvXmlParserStartElementHandler)(IN void* userData, IN const RvChar* name, IN RvUint32 nameSize, 
									  IN const RvXmlAttributeStruct* atts, IN RvUint32 attsNum);

/********************************************************************************************
 * RvXmlParserEndElementHandler
 * A handler for end (and empty) tags - called when an end tag is encountered.
 * INPUT   : userData - Handle to user-defined data structure
 *           name - The tag name.
 *           nameSize - size of tag name. 				
 * NOTE    : - The caller owns only userData.
 *			 - Synchronous.	
 *			 - name is a pointer to the start location of the element name in the parser internal buffer.
 *			   It is not gauranteed to be NULL terminated hence the need for nameSize.
  */
typedef void (*RvXmlParserEndElementHandler)(IN void* userData, IN const RvChar* name, IN RvUint32 nameSize);


/********************************************************************************************
 * Set functions for Start/End element handlers
 */

/********************************************************************************************
 * RvXmlParserSetElementHandler
 * Sets handlers for both start and end elements.
 * INPUT   : hParser - A valid parser handle obtained by calling RvXmlParserConstruct. 
 *           start - Pointer to an RvXmlParserStartElementHandler function implementation.
 *			 end - Pointer to an RvXmlParserEndElementHandler function implementation.
 * NOTE    : - The caller owns all arguments.
 *			 - Synchronous.		
  */
RVAPI void  RVCALLCONV RvXmlParserSetElementHandler(IN RvXmlParserHandle hParser, IN RvXmlParserStartElementHandler start, IN RvXmlParserEndElementHandler end);

/********************************************************************************************
 * RvXmlParserSetStartElementHandler
 * Sets handler for start elements.
 * INPUT   : hParser - A valid parser handle obtained by calling RvXmlParserConstruct. 
 *           handler - Pointer to an RvXmlParserStartElementHandler function implementation.
 * NOTE    : - The caller owns all arguments.
 *			 - Synchronous.		
  */
RVAPI void  RVCALLCONV RvXmlParserSetStartElementHandler(IN RvXmlParserHandle hParser, IN RvXmlParserStartElementHandler handler);

/********************************************************************************************
 * RvXmlParserSetEndElementHandler
 * Sets handler for end elements.
 * INPUT   : hParser - A valid parser handle obtained by calling RvXmlParserConstruct. 
 *			 handler - Pointer to an RvXmlParserEndElementHandler function implementation.
 * NOTE    : - The caller owns all arguments.
 *			 - Synchronous.		
  */
RVAPI void  RVCALLCONV RvXmlParserSetEndElementHandler(IN RvXmlParserHandle hParser, IN RvXmlParserEndElementHandler handler);

/********************************************************************************************
 * RvXmlParserCharacterDataHandler
 * The main data callback handler. 
 * INPUT   : userData - Handle to user-defined data structure
 *           data - pointer to start of data.
 *           dataLen - length of data.
 * NOTE    : - The parser owns text.
 *			 - Synchronous.	
 *			 - The parser supports only text based mail safe encodings like UTF8, base64 and so forth so text is
 *			   always an ASCII text encoding of the data.
  */
typedef void (*RvXmlParserCharacterDataHandler) (IN void *userData, IN const RvChar* data, IN RvUint32 dataLen);

/********************************************************************************************
 * RvXmlParserSetCharacterDataHandler
 * Sets the RvXmlParserCharacterDataHandler handler.
 * INPUT   : hParser - A valid parser handle obtained by calling RvXmlParserConstruct. 
 *			 handler - Pointer to an RvXmlParserCharacterDataHandler function implementation.
 * NOTE    : - The caller owns all arguments.
 *			 - Synchronous.		
 */
RVAPI void  RVCALLCONV RvXmlParserSetCharacterDataHandler(RvXmlParserHandle hParser, IN RvXmlParserCharacterDataHandler handler);

/********************************************************************************************
 * RvXmlParserCommentHandler
 * The comment handler.
 * INPUT   : userData - Handle to user-defined data structure
 *           data - comment.
 *           dataLen - comment length.
  */
typedef void (*RvXmlParserCommentHandler) (IN void* userData, 
								  IN const RvChar* data, IN RvUint32 dataLen);

/********************************************************************************************
 * RvXmlParserSetCommentHandler
 * Sets the RvXmlParserCommentHandler handler.
 * INPUT   : hParser - A valid parser handle obtained by calling RvXmlParserConstruct. 
 *			 handler - Pointer to an RvXmlParserCommentHandler function implementation.
 * NOTE    : - The caller owns all arguments.
 *			 - Synchronous.		
 */
RVAPI void  RVCALLCONV RvXmlParserSetCommentHandler(IN RvXmlParserHandle hParser, IN RvXmlParserCommentHandler handler);

/********************************************************************************************
 * RvXmlParserProcessingInstructionHandler
 * A handler for Processing instruction data in the XML document.
 * INPUT   : userData - user-defined data structure
 *           name - Pointer to the start of the process instruction name.
 *           nameSize - Length of process instruction name in bytes.
 *           atts - Array of attribute structures (RvXmlAttributeStruct) representing name/value pairs
 *					or NULL if no attributes were found. 
 *           attsNum - number of elements in attrs array           .
 * EXAMPLE : When a processing instruction such as <?xml version="1.0" encoding="UTF-8" ?> is encountered
 *			 the callback function will be called with the following arguments:
 *			 name = xml, nameSize = 3 , atts will point to an array of two RvXmlAttributeStruct instances 
 *			 each representing a name/value pair. atts[0] will point to an instance encapsulating
 *			 (name = version, name_size=6 and value = 1.0, value_size = 3) and
 *			 atts[1] will point to an instance encapsulating
 *			 (name = encoding, name_size = 8 and value = UTF-8, value_size = 5), attsNum = 2. 
 * NOTE    : - The caller owns only userData.
 *			 - Synchronous.	
  */
typedef void (*RvXmlParserProcessingInstructionHandler)(IN void* userData, IN const RvChar* name, IN RvUint32 nameSize, 
									  IN const RvXmlAttributeStruct* atts, IN RvUint32 attsNum);

/********************************************************************************************
 * RvXmlParserSetProcessingInstructionHandler
 * Sets the RvXmlParserProcessingInstructionHandler handler implementation.
 * INPUT   : hParser - A valid parser handle obtained by calling RvXmlParserConstruct. 
 *			 handler - Pointer to an RvXmlParserProcessingInstructionHandler function implementation.
 * NOTE    : - The caller owns all arguments.
 *			 - Synchronous.		
 */
RVAPI void  RVCALLCONV RvXmlParserSetProcessingInstructionHandler(IN RvXmlParserHandle hParser, 
													   IN RvXmlParserProcessingInstructionHandler handler);


/********************************************************************************************
 * RvXmlParserStartDoctypeDeclHandler
 * A  handler that is called at the start of a DOCTYPE declaration.
 * INPUT   : userData - Handle to user-defined data structure.
 *           atts - Arrary of attribute structures (RvXmlAttributeStruct) or NULL if no attributes found. 
 *           attsNum - number of elements in attrs array           .
 * NOTE    : - The caller owns userData.
 *			 - Synchronous.		
  */
typedef void (*RvXmlParserStartDoctypeDeclHandler) (IN void *userData, IN const RvXmlAttributeStruct* atts, IN RvUint32 attsNum);

/********************************************************************************************
 * RvXmlParserEndDoctypeDeclHandler
 * A  handler that is called at the end of a DOCTYPE declaration.
 * INPUT   : userData - Handle to user-defined data structure.
 *           model - the body of DOCTYPE declaration as string.
 *           modelSize - model size.
 * NOTE    : - The caller owns userData.
 *			 - Synchronous.		
  */
typedef void (*RvXmlParserEndDoctypeDeclHandler) (IN void *userData, IN const RvChar* model, RvUint32 modelSize);

/********************************************************************************************
 * Set functions for Start/End Doctype Declaration handlers
 */

/********************************************************************************************
 * RvXmlParserSetStartDoctypeDeclHandler
 * Sets handlers for both the start and end of the declaration.
 * INPUT   : hParser - A valid parser handle obtained by calling RvXmlParserConstruct. 
 *           handler - Pointer to an RvXmlParserStartDoctypeDeclHandler function implementation.
 * NOTE    : - The caller owns all arguments.
 *			 - Synchronous.		
  */
RVAPI void  RVCALLCONV RvXmlParserSetStartDoctypeDeclHandler(IN RvXmlParserHandle hParser, IN RvXmlParserStartDoctypeDeclHandler handler);

/********************************************************************************************
 * RvXmlParserSetEndDoctypeDeclHandler
 * Sets a handler for the end of the declaration.
 * INPUT   : hParser - A valid parser handle obtained by calling RvXmlParserConstruct. 
 *			 handler - Pointer to an RvXmlParserEndDoctypeDeclHandler function implementation.
 * NOTE    : - The caller owns all arguments.
 *			 - Synchronous.		
  */
RVAPI void  RVCALLCONV RvXmlParserSetEndDoctypeDeclHandler(IN RvXmlParserHandle hParser, IN RvXmlParserEndDoctypeDeclHandler handler);

/********************************************************************************************
 * RvXmlParserSetDoctypeDeclHandler
 * Sets handlers for both the start and end of the declaration.
 * INPUT   : hParser - A valid parser handle obtained by calling RvXmlParserConstruct. 
 *           start - Pointer to an RvXmlParserStartDoctypeDeclHandler function implementation.
 *			 end - Pointer to an RvXmlParserEndDoctypeDeclHandler function implementation.
 * NOTE    : - The caller owns all arguments.
 *			 - Synchronous.		
  */
RVAPI void  RVCALLCONV RvXmlParserSetDoctypeDeclHandler(IN RvXmlParserHandle hParser, 
											 IN RvXmlParserStartDoctypeDeclHandler start, IN RvXmlParserEndDoctypeDeclHandler end);

/********************************************************************************************
 * RvXmlParserSetCallbackHandlers
 * Sets all callback handlers.
 * INPUT   : hParser - A valid parser handle obtained by calling RvXmlParserConstruct. 
 *           pCallBacks - Pointer to an RvXmlParserCallBacks structure.
 * NOTE    : - The caller owns all arguments.
 *			 - Synchronous.		
  */
RVAPI void  RVCALLCONV RvXmlParserSetCallbackHandlers(IN RvXmlParserHandle hParser, 
											 IN RvXmlParserCallBacks* pCallBacks);

/********************************************************************************************
 * RvXmlParserSetValidationFlag
 * Sets validation flag.
 * INPUT   : hParser - A valid parser handle obtained by calling RvXmlParserConstruct. 
 *           doValidation - validation flag either RV_FALSE (no validation) or RV_TRUE (do validation).
 * NOTE    : - The caller owns all arguments.
 *			 - Synchronous.		
  */
RVAPI void  RVCALLCONV RvXmlParserSetValidationFlag(IN RvXmlParserHandle hParser, 
									 IN RvBool doValidation);
/********************************************************************************************
 * RvXmlParserGetValidationFlag
 * Gets current validation flag.
 * INPUT   : hParser - A valid parser handle obtained by calling RvXmlParserConstruct. 
 * RETURNS  : - current validation flag.
 *			 - Synchronous.		
  */
RVAPI RvBool RVCALLCONV RvXmlParserGetValidationFlag(IN RvXmlParserHandle hParser);

/********************************************************************************************
 * RvXmlParserSetUserData
 * Sets user data pointer.
 * INPUT   : hParser - A valid parser handle obtained by calling RvXmlParserConstruct. 
 *           userData - Pointer to the user data object associated with the parser. 
 *			 This argument is opaque to the parser and the parser does not own this data.
 * NOTE    : - The caller owns all arguments.
 *			 - Synchronous.		
  */
RVAPI void  RVCALLCONV RvXmlParserSetUserData(	IN RvXmlParserHandle hParser, IN void* userData );

/********************************************************************************************
 * RvXmlParserGetUserData
 * Gets a pointer to the current user data associated with the parser.
 * INPUT   : hParser - A valid parser handle obtained by calling RvXmlParserConstruct. 
 * RETURNS  : - Pointer to the current user data object. This data is opaque to the parser and the caller
 *			    should know the relevant data semantics and be able to cast this pointer appropriately.
 *			 - Synchronous.	
  */
RVAPI void*  RVCALLCONV RvXmlParserGetUserData( IN RvXmlParserHandle hParser);


/********************************************************************************************
 * RvXmlParserReset
 * Resets an XML Parser object internal state.
 * INPUT: hParser - A valid parser handle obtained by calling RvXmlParserConstruct.
 * NOTE: - This function doesn't modifiy or release user application data associated with the parser instance,
 *		   by calling RvXmlParserSetUserData.
 *		 - Synchronous.	
 */
RVAPI void RVCALLCONV RvXmlParserReset(RvXmlParserHandle hParser);

/********************************************************************************************
 * RvXmlParse
 * Static single-pass in-place parsing of the given xml string.
 * INPUT: hParser - Pointer to a valid parser object instance created with RvXmlParserConstruc.
 *        xmlString - Pointer to XML-formatted null-terminated string.
 *        optmsk - Parse options mask.
 * RETURNS: RV_OK if the parsing is successfull, or RV_XMLPARSER_ERROR_XML_NOT_VALID otherwise. 
 * NOTE    : - The Caller owns all arguments. 
 *			 - If errors are encountered during parsing the error handlers are called during the parsing procedure.
 *			 - Synchronous.		
  */
RVAPI RvStatus RVCALLCONV RvXmlParse(IN RvXmlParserHandle hParser, IN RvChar*  xmlString, IN RvUint32 optmsk);


/********************************************************************************************
 * RvXmlParserDtdElementDeclHandler
 * A  handler that is called When a DTD element is encountered.
 * INPUT   : userData - Handle to user-defined data structure.
 *			 name - Pointer to the start of the DTD element name.
 *			 nameSize - The DTD element name length in bytes. 
 *           model - Pointer to the start of the body of the DTD element declaration string.
 *           modelSize - body length in bytes.
 * NOTE    : - The caller owns userData.
 *			 - Synchronous.		
  */

typedef void (* RvXmlParserDtdElementDeclHandler)(IN void *userData, IN const RvChar* name, RvUint32 nameSize, 
									  IN const RvChar* model, RvUint32 modelSize);

/********************************************************************************************
 * RvXmlParserSetDtdElementDeclHandler
 * Sets an RvXmlParserDtdElementDeclHandler handler for dtd elements.
 * INPUT   : hParser - A valid parser handle obtained by calling RvXmlParserConstruct. 
 *			 handler - Pointer to an RvXmlParserDtdElementDeclHandler function implementation.
 * NOTE    : - The caller owns all arguments.
 *			 - Synchronous.		
  */
RVAPI void  RVCALLCONV RvXmlParserSetDtdElementDeclHandler(IN RvXmlParserHandle hParser, IN RvXmlParserDtdElementDeclHandler handler);


/*********************************** End of Parser User API ************************************************/

/*********************************** Validation API ********************************************************/
#ifdef RVXML_WITH_VALIDATION
/* For Validation Only */

/********************************************************************************************
 * RvXmlValidateFunctionHandler  
 * A handler called in order to perform the actual XML validation. This handler function should be 
 * associated to an RvXmlValidator instance. 
 * INPUT   : hParser - A valid parser handle obtained by calling RvXmlParserConstruct. 
 *			 xml - Pointer to XML-formatted null-terminated string.
 * RETURNS : RvStatus - RV_OK if the document is valid.
 * NOTE    : - The caller owns all arguments.
 *			 - Synchronous.		
  */
typedef RvStatus (* RvXmlValidateFunctionHandler)(IN RvXmlParserHandle hParser, IN RvChar* xml);

/********************************************************************************************
 * RvXmlParserSetXmlValidator
 * Associates and XML validator with the parser.
 * INPUT   : hParser - A valid parser handle obtained by calling RvXmlParserConstruct. 
 *			 validatorObj - A handle to a an RvXmlValidator instance.
 * NOTE    : - The caller owns hParser.
 *			 - The parser does not own the validator.
 *			 - Synchronous.		
  */
RVAPI void  RVCALLCONV RvXmlParserSetXmlValidator(IN RvXmlParserHandle hParser, IN RvXmlValidatorHandle validatorObj);

/********************************************************************************************
 * GetXmlValidator
 * Retrieves the current validator handle associated with the parser.
 * INPUT   : hParser - A valid validator handle.
 * RETURNS  : - current validation object.
 *			 - Synchronous.	
  */
RVAPI RvXmlValidatorHandle RVCALLCONV GetXmlValidator(IN RvXmlParserHandle hParser);

/********************************************************************************************
 * RvXmlValidate  
 * A public function called by an application using RvXmlParser to validate a NULL terminated
 * XML formatted string according to the RvXmlValidator based validator plug-in implementation. 
 * INPUT   : hParser - A valid parser handle obtained by calling RvXmlParserConstruct. 
 *			 xml - Pointer to XML-formatted null-terminated string.
 * RETURNS : RvBool - Boolean - RV_TRUE if the document is valid, RV_FALSE if the document contains an
 *			 error.
 * NOTE    : - The caller owns all arguments.
 *			 - Synchronous.		
  */
RVAPI RvBool RVCALLCONV RvXmlValidate(IN RvXmlParserHandle hParser, IN RvChar* xml);

/********************************************************************************************
 * RvXmlValidator
 * Structure used as interface to an XML validator plug-in used by the parser.
  */
struct RvXmlValidator {
	RvXmlValidateFunctionHandler ValidateXmlFn; /*Pointer to a validation method using the RvXmlValidateHandler signature.*/
	void* validatorData; /*Pointer to any additional internal validator implementation specific data.*/
};



#endif /* #ifdef RVXML_WITH_VALIDATION */

/********************************************************************************************
 * RvXmlParserCallBacks
 * Structure used to set all the RvXmlParser callbacks at once.
  */
struct RvXmlParserCallBacks
{
	RvXmlParserStartElementHandler StartElementFn; 
	RvXmlParserEndElementHandler EndElementFn;
	RvXmlParserCharacterDataHandler CharacterDataFn;	
	RvXmlParserCommentHandler CommentFn;
	RvXmlParserStartDoctypeDeclHandler StartDoctypeFn;
	RvXmlParserEndDoctypeDeclHandler EndDoctypeFn;
	RvXmlParserXmlErrorHandler ErrorHandlerFn;
	RvXmlParserProcessingInstructionHandler ProcessingInstructionFn;
	RvXmlParserDtdElementDeclHandler ElementDeclFn;
};

#if defined(__cplusplus)
}
#endif 

#endif /* RV_XMLPARSER_H */
