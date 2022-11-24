/***********************************************************************
Filename   : rvxmlparsertypes.h
Description: rvXmlParser types header file

************************************************************************
      Copyright (c) 2001,2002, 2005 RADVISION Inc. and RADVISION Ltd.
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

#ifndef RV_XMLPARSER_TYPES_H
#define RV_XMLPARSER_TYPES_H

#include "rvtypes.h"
#include "rverror.h"
#include "rvlog.h"

#if defined(__cplusplus)
extern "C" {
#endif 

/*
 * RvXmlParserHandle
 * ----------------------
 * Handle of the XmlParser object
 */
RV_DECLARE_HANDLE(RvXmlParserHandle);

/* Error Codes */ 
#define RV_XMLPARSER_ERROR_XML_NOT_VALID -513

/*
 * RvXmlParserHandle
 * ----------------------
 * Handle of the XmlParser object
 */
RV_DECLARE_HANDLE(RvXmlValidatorHandle);

/******************** Forward Declarations ******************************/

/*typedef struct RvXmlParserCfg RvXmlParserCfg;*/
typedef struct RvXmlNodeStruct RvXmlNodeStruct;
/*typedef enum RvXmlParserErrors RvXmlParserErrors;*/
/*typedef struct RvXmlAttributeStruct RvXmlAttributeStruct;*/
typedef struct RvXmlParserCallBacks RvXmlParserCallBacks;

/********************************************************************************************
 * RvXmlAttributeStruct
 * A 'name=value' XML attribute structure.
 ********************************************************************************************/

typedef struct RvXmlAttributeStruct
{
	RvChar*		name;			/*Pointer to attribute name.*/
	RvUint32    name_size;		/*Length of element name.*/
	RvChar*		value;			/*Pointer to attribute value.*/
	RvUint32	value_size;		/*Length of element name.*/
} RvXmlAttributeStruct;

/********************************************************************************************
 * RvXmlParserErrors enumeration
 ********************************************************************************************/
typedef enum RvXmlParserErrors {
	RVXMLPARSER_ERROR_UNKNOWN = 0,
	RVXMLPARSER_ERROR_NODE_STACK_OVERFLOW,
	RVXMLPARSER_ERROR_ATTRIBUTE_SPACE_OVERFLOW,
	RVXMLPARSER_ERROR_GENERALAL_LOCATION_FAIL

} RvXmlParserErrors; /* enum RvXmlParserErrors */

/********************************************************************************************
 * RvXmlParserCfg structure - Structure used to initiate the parser instance. A pointer to
 * this structure is passed to RvXmlParserConstruct.
 ********************************************************************************************/
typedef struct RvXmlParserCfg {
	 /*maxAttributesNum - Maximum number of attributes in a tag. If the actual number of 
     attributes is larger than this value, the parser reports a RVXMLPARSER_ERROR_ATTRIBUTE_SPACE_OVERFLOW
	 runtime error.*/
	RvUint32 maxAttributesNum;

	/*maxStackDepth - the maximum depth of XML tree. If the actual the XML tree is deeper.The parser 
     reports an RVXMLPARSER_ERROR_NODE_STACK_OVERFLOW runtime error.*/

	RvUint32 maxStackDepth;
	
	RvLogSource*                  pLogSrc;
	RvLogMgr*                     pLogMgr;
} RvXmlParserCfg; /* end struct RvXmlParserCfg */

/*********************************** Parser Options ********************************************************/
#define RVXMLPARSER_OPTION_MINIMAL			0x00000000 /* Unset the following flags */
#define RVXMLPARSER_OPTION_PI				0x00000002 /* Parse '&lt;?...?&gt;' */
#define RVXMLPARSER_OPTION_DOCTYPE			0x00000004 /* Parse '&lt;!DOCTYPE ...&gt;' section, setting '[...]' as data member. */
#define RVXMLPARSER_OPTION_COMMENTS			0x00000008 /* Parse &lt;!--...--&gt;' */
#define RVXMLPARSER_OPTION_CDATA			0x00000010 /* Parse '&lt;![CDATA[...]]&gt;', and/or '&lt;![INCLUDE[...]]&gt;' */
#define RVXMLPARSER_OPTION_ESCAPES			0x00000020 /* Not implemented. */
#define RVXMLPARSER_OPTION_TRIM_PCDATA		0x00000040 /* Trim '&gt;...&lt;' */
#define RVXMLPARSER_OPTION_TRIM_ATTRIBUTE	0x00000080 /*Trim 'foo="..."'. */
#define RVXMLPARSER_OPTION_TRIM_CDATA		0x00000100 /* Trim '&lt;![CDATA[...]]&gt;', and/or '&lt;![INCLUDE[...]]&gt;' */
#define RVXMLPARSER_OPTION_TRIM_ENTITY		0x00000200 /* Trim '&lt;!ENTITY name ...&gt;', etc. */
#define RVXMLPARSER_OPTION_TRIM_DOCTYPE		0x00000400 /* Trim '&lt;!DOCTYPE [...]&gt;' */
#define RVXMLPARSER_OPTION_TRIM_COMMENT		0x00000800 /* Trim &lt;!--...--&gt;' */
#define RVXMLPARSER_OPTION_WNORM			0x00001000 /* Normalize all entities that are flagged to be trimmed. */
#define RVXMLPARSER_OPTION_DEFAULT			0x00001FFF 
#define RVXMLPARSER_OPTION_NOSET			0x80000000


/*********************************** Validation API ********************************************************/
#ifdef RVXML_WITH_VALIDATION
/* For Validation Only */

typedef struct RvXmlValidator RvXmlValidator;

#endif /* #ifdef RVXML_WITH_VALIDATION */

#if defined(__cplusplus)
}
#endif 

#endif /* RV_XMLPARSER_TYPES_H */
