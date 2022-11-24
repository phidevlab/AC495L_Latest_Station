/******************************************************************************
Filename:    rvmdmobjects.h
Description: MDM objects
*******************************************************************************
                Copyright (c) 2001 RADVISION Inc.
*******************************************************************************
NOTICE:
This document contains information that is proprietary to RADVISION Inc.
No part of this publication may be reproduced in any form whatsoever without
written prior approval by RADVISION Inc.

RADVISION Inc. reserves the right to revise this publication and make changes
without obligation to notify any person of such revisions or changes.
******************************************************************************/

#ifndef RV_MDMOBJECTS_H
#define RV_MDMOBJECTS_H

#include "ipp_inc_std.h"
#include "rvtypes.h"
#include "rvstring.h"
#include "rvvector.h"
#include "rvptrvector.h"
#include "rvlist.h"
#include "rvmap.h"
#include "rvsdp.h"
/*#include "rvutil.h"*/
/*#include "rvinet.h"*/


#if defined(__cplusplus)
extern "C" {
#endif

#define  RV_MDM_PHONENUMBERS_SIZE	3

/*@*****************************************************************************
 * Package: RvMtfDigitmapPkg (root)
 * -----------------------------------------------------------------------------
 * Title: Digitmaps
 *
 * General: This section contains functions and type definitions that are used 
 *  		for building and setting digitmaps.
 ****************************************************************************@*/


/*$
{type:
	{name: RvMdmPackageItem}
	{include: rvmdmobjects.h}
	{description:
		{p: Stores a package specific identifier.}
		{p: Used to store properties, events, signals and statistics.}
	}
	{methods:
		{method: RvMdmPackageItem *rvMdmPackageItemConstruct(RvMdmPackageItem *x, const char *pkg, const char *item);}
		{method: RvMdmPackageItem *rvMdmPackageItemConstructA(RvMdmPackageItem *x, const char *pkg, const char *item, RvAlloc *alloc);}
		{method: RvMdmPackageItem *rvMdmPackageItemConstructCopy(RvMdmPackageItem *d, const RvMdmPackageItem *s, RvAlloc *alloc);}
		{method: void rvMdmPackageItemDestruct(RvMdmPackageItem *x);}
		{method: RvMdmPackageItem *rvMdmPackageItemCopy(RvMdmPackageItem *d, const RvMdmPackageItem *s);}
		{method: RvBool rvMdmPackageItemEqual(const RvMdmPackageItem *a, const RvMdmPackageItem *b);}
		{method: const char *rvMdmPackageItemGetPackage(const RvMdmPackageItem *x);}
		{method: const char *rvMdmPackageItemGetItem(const RvMdmPackageItem *x);}
	}
}
$*/

typedef struct
{
	RvString    package;
    RvString    item;
} RvMdmPackageItem;

RVAPI RvMdmPackageItem* RVCALLCONV rvMdmPackageItemConstruct(
    IN RvMdmPackageItem*    x,
    IN const RvChar*        pkg,
    IN const RvChar*        item);
RVAPI RvMdmPackageItem* RVCALLCONV rvMdmPackageItemConstructA(
    IN RvMdmPackageItem*    x,
    IN const RvChar*        pkg,
    IN const RvChar*        item,
    IN RvAlloc*             alloc);
RVAPI RvMdmPackageItem* RVCALLCONV rvMdmPackageItemConstructCopy(
    IN RvMdmPackageItem*        d,
    IN const RvMdmPackageItem*  s,
    IN RvAlloc*                 alloc);
RVAPI void RVCALLCONV rvMdmPackageItemDestruct(IN RvMdmPackageItem* x);
RVAPI RvMdmPackageItem* RVCALLCONV rvMdmPackageItemCopy(
    IN RvMdmPackageItem*        d,
    IN const RvMdmPackageItem*  s);
RVAPI RvBool RVCALLCONV rvMdmPackageItemEqual(
    IN const RvMdmPackageItem*  a,
    IN const RvMdmPackageItem*  b);
RVAPI const RvChar* RVCALLCONV rvMdmPackageItemGetPackage(IN const RvMdmPackageItem *x);
RVAPI const RvChar* RVCALLCONV rvMdmPackageItemGetItem(IN const RvMdmPackageItem *x);




/*$
{type:
	{name: RvMdmParameterValue}
	{include: rvmdmobjects.h}
	{description:
		{p: Stores the value of a parameter.}
		{p: Supports fully specified, over-specified, and under-specified parameters.}
	}
	{methods:
		{method: RvMdmParameterValue *rvMdmParameterValueConstruct(RvMdmParameterValue *x, const char *val);}
		{method: RvMdmParameterValue *rvMdmParameterValueConstructA(RvMdmParameterValue *x, const char *val, RvAlloc *alloc);}
		{method: RvMdmParameterValue *rvMdmParameterValueConstructList(RvMdmParameterValue *x, RvMdmRelation type);}
		{method: RvMdmParameterValue *rvMdmParameterValueConstructListA(RvMdmParameterValue *x, RvMdmRelation type, RvAlloc *alloc);}
		{method: RvMdmParameterValue *rvMdmParameterValueConstructCopy(RvMdmParameterValue *d, const RvMdmParameterValue *s, RvAlloc *alloc);}
		{method: void rvMdmParameterValueDestruct(RvMdmParameterValue *x);}
		{method: void rvMdmParameterValueAddToList(RvMdmParameterValue *x, const char *val);}
		{method: RvMdmRelation rvMdmParameterValueGetType(const RvMdmParameterValue *x);}
		{method: const char *rvMdmParameterValueGetValue(const RvMdmParameterValue *x);}
		{method: size_t rvMdmParameterValueGetListSize(const RvMdmParameterValue *x);}
		{method: const char *rvMdmParameterValueGetListValue(const RvMdmParameterValue *x, size_t index);}
	}
}
$*/

/* RvMdmRelation
 * ------------------------------------------------------------------------
 * An interpretation of parameter value.
 */
// TODO: What is the purpose of equal?
typedef enum
{
	RV_MDM_RELATION_EQUAL,
	RV_MDM_RELATION_AND,
	RV_MDM_RELATION_OR
} RvMdmRelation;



typedef struct
{
	RvMdmRelation   type;
	RvStringList    values;
} RvMdmParameterValue;

RVAPI RvMdmParameterValue* RVCALLCONV rvMdmParameterValueConstruct(
    IN RvMdmParameterValue* x,
    IN const RvChar*        val);
RVAPI RvMdmParameterValue* RVCALLCONV rvMdmParameterValueConstructA(
    IN RvMdmParameterValue* x,
    IN const RvChar*        val,
    IN RvAlloc*             alloc);
RVAPI RvMdmParameterValue* RVCALLCONV rvMdmParameterValueConstructList(
    IN RvMdmParameterValue* x,
    IN RvMdmRelation        type);
RVAPI RvMdmParameterValue* RVCALLCONV rvMdmParameterValueConstructListA(
    IN RvMdmParameterValue* x,
    IN RvMdmRelation        type,
    IN RvAlloc*             alloc);
RVAPI RvMdmParameterValue* RVCALLCONV rvMdmParameterValueConstructCopy(
    IN RvMdmParameterValue*         d,
    IN const RvMdmParameterValue*   s,
    IN RvAlloc*                     alloc);
RVAPI void RVCALLCONV rvMdmParameterValueDestruct(IN RvMdmParameterValue* x);
RVAPI void RVCALLCONV rvMdmParameterValueAddToList(
    IN RvMdmParameterValue* x,
    IN const RvChar*        val);
void rvMdmParameterValueAddToListUnique(RvMdmParameterValue *x, const char *val);
void rvMdmParameterValueAnd(RvMdmParameterValue *x, const char *val);
void rvMdmParameterValueOr(RvMdmParameterValue *x, const char *val);
RVAPI RvMdmRelation RVCALLCONV rvMdmParameterValueGetType(
    IN const RvMdmParameterValue*   x);
RVAPI const RvChar* RVCALLCONV rvMdmParameterValueGetValue(IN const RvMdmParameterValue *x);
RVAPI RvSize_t RVCALLCONV rvMdmParameterValueGetListSize(IN const RvMdmParameterValue* x);
RVAPI const RvChar* RVCALLCONV rvMdmParameterValueGetListValue(
    IN const RvMdmParameterValue*   x,
    IN RvSize_t                     index);


rvDeclareMap(RvMdmPackageItem, RvMdmParameterValue)

typedef void (*RvMdmParameterFunc)(
    IN const RvMdmPackageItem*      name,
    IN const RvMdmParameterValue*   value,
    IN void*                        data);

/*$
{type:
	{name: RvMdmParameterList}
	{include: rvmdmobjects.h}
	{description: {p: A list of parameters.}}
	{methods:
		{method: RvMdmParameterList *rvMdmParameterListConstruct(RvMdmParameterList *x);}
		{method: RvMdmParameterList *rvMdmParameterListConstructA(RvMdmParameterList *x, RvAlloc *alloc);}
		{method: RvMdmParameterList *rvMdmParameterListConstructCopy(RvMdmParameterList *d, const RvMdmParameterList *s, RvAlloc *alloc);}
		{method: void rvMdmParameterListDestruct(RvMdmParameterList *x);}
		{method: RvMdmParameterList *rvMdmParameterListCopy(RvMdmParameterList *d, const RvMdmParameterList *s);}
		{method: void rvMdmParameterListSet(RvMdmParameterList *x, const RvMdmPackageItem *name, const RvMdmParameterValue *value);}
		{method: void rvMdmParameterListSet2(RvMdmParameterList *x, const char *name, const RvMdmParameterValue *value);}
		{method: const RvMdmParameterValue *rvMdmParameterListGet(const RvMdmParameterList *x, const RvMdmPackageItem *name);}
		{method: const RvMdmParameterValue *rvMdmParameterListGet2(const RvMdmParameterList *x, const char *name);}
		{method: void rvMdmParameterListForEach(const RvMdmParameterList *x, RvMdmParameterFunc f, void *data);}
		{method: RvBool rvMdmParameterListIsEmpty(const RvMdmParameterList *x);}
	}
}
$*/

typedef struct
{
	RvMap(RvMdmPackageItem, RvMdmParameterValue) list;
} RvMdmParameterList;

RVAPI RvMdmParameterList* RVCALLCONV rvMdmParameterListConstruct(IN RvMdmParameterList* x);
RVAPI RvMdmParameterList* RVCALLCONV rvMdmParameterListConstructA(
    IN RvMdmParameterList*  x,
    IN RvAlloc*             alloc);
RVAPI RvMdmParameterList* RVCALLCONV rvMdmParameterListConstructCopy(
    IN RvMdmParameterList*          d,
    IN const RvMdmParameterList*    s,
    IN RvAlloc*                     alloc);
RVAPI void RVCALLCONV rvMdmParameterListDestruct(IN RvMdmParameterList* x);
RVAPI RvMdmParameterList* RVCALLCONV rvMdmParameterListCopy(
    IN RvMdmParameterList*          d,
    IN const RvMdmParameterList*    s);
void rvMdmParameterListAnd(RvMdmParameterList *x, const RvMdmPackageItem *name, const char *value);
RVAPI void RVCALLCONV rvMdmParameterListOr(
    IN RvMdmParameterList*      x,
    IN const RvMdmPackageItem*  name,
    IN const RvChar*            value);
RVAPI void RVCALLCONV rvMdmParameterListSet(
    IN RvMdmParameterList*          x,
    IN const RvMdmPackageItem*      name,
    IN const RvMdmParameterValue*   value);
RVAPI void rvMdmParameterListSet2(RvMdmParameterList *x, const char *name, const RvMdmParameterValue *value);
RVAPI const RvMdmParameterValue* RVCALLCONV rvMdmParameterListGet(
    IN const RvMdmParameterList*    x,
    IN const RvMdmPackageItem*      name);
RVAPI const RvMdmParameterValue* RVCALLCONV rvMdmParameterListGet2(
    IN const RvMdmParameterList*    x,
    IN const RvChar*                name);
RVAPI void RVCALLCONV rvMdmParameterListForEach(
    IN const RvMdmParameterList*    x,
    IN RvMdmParameterFunc           f,
    IN void*                        data);
RVAPI RvBool RVCALLCONV rvMdmParameterListIsEmpty(IN const RvMdmParameterList* x);


/*@*****************************************************************************
* Enum: RvMdmDigitPositionTimerMode (RvMtfDigitmapPkg)
* -----------------------------------------------------------------------------
* Description: This enumerator indicates the mode of the timer to be used
*			   for the given digit position in dial string matching.
*****************************************************************************@*/
typedef enum
{
	RV_MDMDIGITPOSITION_NOCHANGE,
        /* Leaves the time mode unchanged, allowing it to be processed in
           the default manner */

	RV_MDMDIGITPOSITION_SHORTTIMER,
        /* Uses the short timer that is set for this digitmap string.
           The short timer is set by rvMdmDigitMapSetShortTimeout(). */

	RV_MDMDIGITPOSITION_LONGTIMER
        /* Uses the long timer that is set for this digitmap string.
           The long timer is set by rvMdmDigitMapSetLongTimeout(). */
} RvMdmDigitPositionTimerMode;


/*@*****************************************************************************
* Type: RvMdmDigitPosition (RvMtfDigitmapPkg)
* -----------------------------------------------------------------------------
* Description: This type holds the events to be matched sometime during dialing.
*****************************************************************************@*/
typedef struct
{
	int                         events;
	RvBool                      multiple;
	RvMdmDigitPositionTimerMode timerMode;
} RvMdmDigitPosition;


/*@*****************************************************************************
 * rvMdmDigitPositionConstruct (RvMtfDigitmapPkg)
 * -----------------------------------------------------------------------------
 * General:
 *        Constructs a new RvMdmDigitPosition object.
 *
 * Arguments:
 *
 * Input:  x - Pointer to the RvMdmDigitPosition object.
 *
 * Output: None.
 *
 * Return Value:  A pointer to the constructed object, or NULL if construction failed.
 ****************************************************************************@*/
RVAPI RvMdmDigitPosition* RVCALLCONV rvMdmDigitPositionConstruct(IN RvMdmDigitPosition* x);

/*@*****************************************************************************
 * rvMdmDigitPositionConstructCopy (RvMtfDigitmapPkg)
 * -----------------------------------------------------------------------------
 * General:
 *        Constructs a copy of a digit position object.
 *
 * Arguments:
 *
 * Input:	d - The destination digit position object.
 *			s - The digit position object to copy.
 *
 * Output: None.
 *
 * Return Value:  A pointer to the constructed object, or NULL if construction failed.
 ****************************************************************************@*/
#define rvMdmDigitPositionConstructCopy rvDefaultConstructCopy

/*@*****************************************************************************
 * rvMdmDigitPositionDestruct (RvMtfDigitmapPkg)
 * -----------------------------------------------------------------------------
 * General:
 *        Destructs a new RvMdmDigitPosition object.
 *
 * Arguments:
 *
 * Input:  x - Pointer to the RvMdmDigitPosition object.
 *
 * Output: None.
 *
 * Return Value:  None.
 ****************************************************************************@*/
#define rvMdmDigitPositionDestruct rvDefaultDestruct

/*@*****************************************************************************
 * rvMdmDigitPositionCopy (RvMtfDigitmapPkg)
 * -----------------------------------------------------------------------------
 * General:
 *        Copies the value of one digit position object to another.
 *
 * Arguments:
 *
 * Input:	d - The destination digit position object.
 *			s - The digit position object to copy.
 *
 * Output: None.
 *
 * Return Value:  A pointer to the destination object, or NULL if copying failed.
 ****************************************************************************@*/
#define rvMdmDigitPositionCopy rvDefaultCopy

/*@*****************************************************************************
 * rvMdmDigitPositionAddEvents (RvMtfDigitmapPkg)
 * -----------------------------------------------------------------------------
 * General:
 *        Adds a range of DTMF events to an RvMdmDigitPosition object.
 *
 * Arguments:
 *
 * Input:	x - Pointer to the RvMdmDigitPosition object.
 *			c1 - The first event in the range.
 *			c2 - The last event in the range.
 *
 * Output: None.
 *
 * Return Value:  None.
 ****************************************************************************@*/
RVAPI void RVCALLCONV rvMdmDigitPositionAddEvents(
    IN RvMdmDigitPosition*  x,
    IN RvChar               c1,
    IN RvChar               c2);

/*@*****************************************************************************
 * rvMdmDigitPositionSetMultipleFlag (RvMtfDigitmapPkg)
 * -----------------------------------------------------------------------------
 * General:
 *			Sets the multiple flag of the RvMdmDigitPosition object.
 *			Sets this flag to indicate that zero or more repetitions of the digit
 *			position are allowed at this point in the digit string.
 *
 * Arguments:
 *
 * Input:	x - Pointer to the RvMdmDigitPosition object.
 *			enable - The new value of the flag.
 *
 * Output: None.
 *
 * Return Value:  None.
 ****************************************************************************@*/
RVAPI void RVCALLCONV rvMdmDigitPositionSetMultipleFlag(
    IN RvMdmDigitPosition*  x,
    IN RvBool               enable);

/*@*****************************************************************************
 * rvMdmDigitPositionSetTimerMode (RvMtfDigitmapPkg)
 * -----------------------------------------------------------------------------
 * General:
 *			Sets the timer mode of the RvMdmDigitPosition object.
 *			Sets the timer to be used when matching events in the current position
 *			and subsequent positions.
 *
 * Arguments:
 *
 * Input:	x - Pointer to the RvMdmDigitPosition object.
 *			mode - The timer mode.
 *
 * Output: None.
 *
 * Return Value:  None.
 ****************************************************************************@*/
RVAPI void RVCALLCONV rvMdmDigitPositionSetTimerMode(
    IN RvMdmDigitPosition*          x,
    IN RvMdmDigitPositionTimerMode  mode);


RvBool rvMdmDigitPositionIsMultiple(const RvMdmDigitPosition *x);


rvDeclareVector(RvMdmDigitPosition)


/*@*****************************************************************************
* Type: RvMdmDigitString (RvMtfDigitmapPkg)
* -----------------------------------------------------------------------------
* Description: This type holds one possible dialing pattern in a dialing plan.
*****************************************************************************@*/

typedef struct
{
	RvVector(RvMdmDigitPosition) elements;
} RvMdmDigitString;

/*@*****************************************************************************
 * rvMdmDigitStringConstruct (RvMtfDigitmapPkg)
 * -----------------------------------------------------------------------------
 * General:
 *        Constructs a new RvMdmDigitString object.
 *
 * Arguments:
 *
 * Input:  x - Pointer to the RvMdmDigitString object.
 *
 * Output: None.
 *
 * Return Value:  A pointer to the constructed object.
 ****************************************************************************@*/
RVAPI RvMdmDigitString* RVCALLCONV rvMdmDigitStringConstruct(IN RvMdmDigitString* x);

RvMdmDigitString *rvMdmDigitStringConstructCopy(RvMdmDigitString *d, const RvMdmDigitString *s, RvAlloc *alloc);

/*@*****************************************************************************
 * rvMdmDigitStringDestruct (RvMtfDigitmapPkg)
 * -----------------------------------------------------------------------------
 * General:
 *        Destructs a new RvMdmDigitString object.
 *
 * Arguments:
 *
 * Input:  x - Pointer to the RvMdmDigitString object.
 *
 * Output: None.
 *
 * Return Value:  None.
 ****************************************************************************@*/
RVAPI void RVCALLCONV rvMdmDigitStringDestruct(IN RvMdmDigitString* x);

/*@*****************************************************************************
 * rvMdmDigitStringAddElement (RvMtfDigitmapPkg)
 * -----------------------------------------------------------------------------
 * General:
 *        Adds a new element to an RvMdmDigitString object.
 *
 * Arguments:
 *
 * Input:	x	- Pointer to the RvMdmDigitString object.
 *			pos	- The new element to be added.
 *
 * Output: None.
 *
 * Return Value:  None.
 ****************************************************************************@*/
RVAPI void RVCALLCONV rvMdmDigitStringAddElement(
    IN RvMdmDigitString*            x,
    IN const RvMdmDigitPosition*    pos);


void rvMdmDigitStringClear(RvMdmDigitString *x);



/*@*****************************************************************************
* Enum: RvMdmDigitMapMatchType (RvMtfDigitmapPkg)
* -----------------------------------------------------------------------------
* Description: This enumerator indicates a digitmap matching type, i.e., the
* result of matching the dial string to a digitmap pattern.
*****************************************************************************@*/
typedef enum
{
	RV_MDMDIGITMAP_NOMATCH,
		/* The dial string does not match a legal pattern, i.e., the user dialed a
		   wrong number or did not dial a complete phone number. */

	RV_MDMDIGITMAP_PARTIALMATCH,
		/* The dial string may match a legal pattern, i.e., the user dialed a partial number. */

	RV_MDMDIGITMAP_FULLMATCH,
		/* The dial string matches a legal pattern but might be ambiguous, i.e., more digits
		   can be collected and the dial string will still match a legal pattern. */

	RV_MDMDIGITMAP_UNAMBIGUOUSMATCH
		/* The dial string matches a legal pattern. */

} RvMdmDigitMapMatchType;


rvDeclareVector(RvMdmDigitString)


/*@*****************************************************************************
* Type: RvMdmDigitMap (RvMtfDigitmapPkg)
* -----------------------------------------------------------------------------
* Description: This type indicates a digitmap object that includes a list of
* patterns and timers to be used when the digitmap is active (during digit collection).
*****************************************************************************@*/
typedef struct
{
	RvVector(RvMdmDigitString) patterns;

	RvUint32    startTimeout;
		/* Start Timeout. The length of time allowed for dialing the first digit from the time the dial tone is applied. */

    RvUint32    shortTimeout;
		/* Short interdigit timer. Used when critical timing should be performed, for example, when
		   the dialed digits constitute a complete address but additional digits may constitute
		   a different complete address. */

    RvUint32    longTimeout;
		/* Long Interdigit Timer. The allowable time between digits if the short
		   interdigit timer has not been indicated in the digitmap. */

} RvMdmDigitMap;

/*@*****************************************************************************
 * rvMdmDigitMapConstruct (RvMtfDigitmapPkg)
 * -----------------------------------------------------------------------------
 * General:
 *        Constructs a new RvMdmDigitMap object and initializes it with default values.
 *
 * Arguments:
 *
 * Input:  x - pointer to RvMdmDigitMap object.
 *
 * Output: None.
 *
 * Return Value:  A pointer to the constructed object.
 ****************************************************************************@*/
RVAPI RvMdmDigitMap* RVCALLCONV rvMdmDigitMapConstruct(IN RvMdmDigitMap* x);

RvMdmDigitMap* rvMdmDigitMapConstructA(
								IN RvMdmDigitMap*   x,
								IN RvAlloc*         alloc);

RvMdmDigitMap *rvMdmDigitMapConstructCopy(RvMdmDigitMap *d, const RvMdmDigitMap *s, RvAlloc *alloc);

/*@*****************************************************************************
 * rvMdmDigitMapDestruct (RvMtfDigitmapPkg)
 * -----------------------------------------------------------------------------
 * General:
 *        Destructs an RvMdmDigitMap object and its list of patterns.
 *
 * Arguments:
 *
 * Input:  x - Pointer to RvMdmDigitMap object.
 *
 * Output: None.
 *
 * Return Value:  None.
 ****************************************************************************@*/
RVAPI void RVCALLCONV rvMdmDigitMapDestruct(IN RvMdmDigitMap* x);


size_t rvMdmDigitMapGetSize(const RvMdmDigitMap *x);

/*@*****************************************************************************
 * rvMdmDigitMapCopy (RvMtfDigitmapPkg)
 * -----------------------------------------------------------------------------
 * General:
 *        Copies one RvMdmDigitMap object to another.
 *
 * Arguments:
 *
 * Input:	d - Pointer to the destination RvMdmDigitMap object to which the source object will be copied.
 *			s - Pointer to the source RvMdmDigitMap object that should be copied to the destination object.
 *
 * Output: None.
 *
 * Return Value:  A pointer to the destination RvMdmDigitMap object.
 ****************************************************************************@*/
RVAPI RvMdmDigitMap* RVCALLCONV rvMdmDigitMapCopy(
    IN RvMdmDigitMap*       d,
    IN const RvMdmDigitMap* s);

/*@*****************************************************************************
 * rvMdmDigitMapAddPattern (RvMtfDigitmapPkg)
 * -----------------------------------------------------------------------------
 * General:
 *        Adds a new pattern to the RvMdmDigitMap object.
 *
 * Arguments:
 *
 * Input:	x - Pointer to the RvMdmDigitMap object.
 *			s - Pointer to a digitmap pattern.
 *
 * Output: None.
 *
 * Return Value:  None.
 ****************************************************************************@*/
RVAPI void RVCALLCONV rvMdmDigitMapAddPattern(
    IN RvMdmDigitMap*           x,
    IN const RvMdmDigitString*  s);

/*@*****************************************************************************
 * rvMdmDigitMapSetStartTimeout (RvMtfDigitmapPkg)
 * -----------------------------------------------------------------------------
 * General:
 *			Sets the start timeout for the digitmap object. The start timeout is the
 *			length of time allowed for dialing the first digit from the time the dial tone
 *			is applied.
 *
 * Arguments:
 *
 * Input:	x - Pointer to the RvMdmDigitMap object.
 *			val - Value of the start timeout, in seconds.
 *
 * Output: None.
 *
 * Return Value:  None.
 ****************************************************************************@*/
RVAPI void RVCALLCONV rvMdmDigitMapSetStartTimeout(
    IN RvMdmDigitMap*   x,
    IN RvUint32         val);

/*@*****************************************************************************
 * rvMdmDigitMapSetShortTimeout (RvMtfDigitmapPkg)
 * -----------------------------------------------------------------------------
 * General:
 *			Sets the short timeout for the digitmap object. The short timeout
 *			indicates when critical timing should be performed, such as when
 *			the dialed digits constitute a complete address but additional digits
 *			may constitute a different complete address.
 *
 * Arguments:
 *
 * Input:	x - Pointer to the RvMdmDigitMap object.
 *			val - Value of the short timeout, in seconds.
 *
 * Output: None.
 *
 * Return Value:  None.
 ****************************************************************************@*/
RVAPI void RVCALLCONV rvMdmDigitMapSetShortTimeout(
    IN RvMdmDigitMap*   x,
    IN RvUint32         val);

/*@*****************************************************************************
 * rvMdmDigitMapSetLongTimeout (RvMtfDigitmapPkg)
 * -----------------------------------------------------------------------------
 * General:
 *			Sets the long timeout for the digitmap object. The long timeout indicates
 *			the allowable time between digits if the short interdigit timer was not
 *			indicated in the digitmap.
 *
 * Arguments:
 *
 * Input:	x - Pointer to the RvMdmDigitMap object.
 *			val - Value of the long timeout, in seconds.
 *
 * Output: None.
 *
 * Return Value:  None.
 ****************************************************************************@*/
RVAPI void RVCALLCONV rvMdmDigitMapSetLongTimeout(
    IN RvMdmDigitMap*   x,
    IN RvUint32         val);


RvUint32 rvMdmDigitMapGetStartTimeout(IN const RvMdmDigitMap *x);
RvMdmDigitMapMatchType rvMdmDigitMapMatch(const RvMdmDigitMap *x, const char *dialString, unsigned int *timerDuration);
const RvMdmDigitString *rvMdmDigitMapGetElem(const RvMdmDigitMap *x, size_t index);

/*$
{type:
	{name: RvMdmDigitMapDescriptor}
	{include: rvmdmobjects.h}
	{description:	{p: A digit map descriptor may contain a digit map, a previously defined digit map name,
		or a new digit map name and its definition.}}
	{methods:
		{method: RvMdmDigitMapDescriptor *rvMdmDigitMapDescriptorConstruct(RvMdmDigitMapDescriptor *x, const char *name, const RvMdmDigitMap *map);}
		{method: RvMdmDigitMapDescriptor *rvMdmDigitMapDescriptorConstructA(RvMdmDigitMapDescriptor *x, const char *name, const RvMdmDigitMap *map, RvAlloc *alloc);}
		{method: RvMdmDigitMapDescriptor *rvMdmDigitMapDescriptorConstructCopy(RvMdmDigitMapDescriptor *d, const RvMdmDigitMapDescriptor *s, RvAlloc *alloc);}
		{method: void rvMdmDigitMapDescriptorDestruct(RvMdmDigitMapDescriptor *x);}
		{method: RvMdmDigitMapDescriptor *rvMdmDigitMapDescriptorCopy(RvMdmDigitMapDescriptor *d, const RvMdmDigitMapDescriptor *s);}
		{method: const char *rvMdmDigitMapDescriptorGetName(const RvMdmDigitMapDescriptor *x);}
		{method: const RvMdmDigitMap *rvMdmDigitMapDescriptorGetDigitMap(const RvMdmDigitMapDescriptor *x);}
		{method: RvBool rvMdmDigitMapDescriptorIsSet(const RvMdmDigitMapDescriptor *x);}
	}
	{notes:
		{note:  A digit map descriptor embedded in an events descriptor
			cannot contain both a name and a digit map.}
	}
}
$*/

typedef struct
{
	RvString        name;
	RvMdmDigitMap   digitMap;
} RvMdmDigitMapDescriptor;

RvMdmDigitMapDescriptor *rvMdmDigitMapDescriptorConstruct(RvMdmDigitMapDescriptor *x, const char *name, const RvMdmDigitMap *map);
RvMdmDigitMapDescriptor *rvMdmDigitMapDescriptorConstructA(RvMdmDigitMapDescriptor *x, const char *name, const RvMdmDigitMap *map, RvAlloc *alloc);
RvMdmDigitMapDescriptor *rvMdmDigitMapDescriptorConstructCopy(RvMdmDigitMapDescriptor *d, const RvMdmDigitMapDescriptor *s, RvAlloc *alloc);
void rvMdmDigitMapDescriptorDestruct(RvMdmDigitMapDescriptor *x);
RvMdmDigitMapDescriptor *rvMdmDigitMapDescriptorCopy(RvMdmDigitMapDescriptor *d, const RvMdmDigitMapDescriptor *s);
const char *rvMdmDigitMapDescriptorGetName(const RvMdmDigitMapDescriptor *x);
const RvMdmDigitMap *rvMdmDigitMapDescriptorGetDigitMap(const RvMdmDigitMapDescriptor *x);
RvBool rvMdmDigitMapDescriptorIsSet(const RvMdmDigitMapDescriptor *x);


typedef RvUint32 RvMdmStreamId;


/* RvMdmEvent
 * ------------------------------------------------------------------------
 * An MDM Event. The MDM operates through events that are used to handle the
 * various terminations and connections. Each such event has its own name
 * and parameters.
 * Events given by the MTF should be handled as read-only objects. The
 * application should not access the fields of this struct directly, but
 * through the relevant access functions.
 */
typedef struct
{
	RvMdmPackageItem name;
	RvMdmParameterList parameters;
} RvMdmEvent;

RvMdmEvent *rvMdmEventConstructA(RvMdmEvent *x, const RvMdmPackageItem *name, RvAlloc *alloc);
RvMdmEvent *rvMdmEventConstructCopy(RvMdmEvent *d, const RvMdmEvent *s, RvAlloc *alloc);
void rvMdmEventDestruct(RvMdmEvent *x);
void rvMdmEventSetParameter(RvMdmEvent *x, const char *name, const RvMdmParameterValue *value);

/******************************************************************************
 * rvMdmEventGetName
 * ----------------------------------------------------------------------------
 * General:
 *  Gets the name of the event object.
 * Arguments:
 * Input:  x    - The event object.
 * Output: None.
 *
 * Return Value: The name.
 *****************************************************************************/
RVAPI const RvMdmPackageItem* RVCALLCONV rvMdmEventGetName(IN const RvMdmEvent* x);

/******************************************************************************
 * rvMdmEventGetParameterList
 * ----------------------------------------------------------------------------
 * General:
 *  Gets the parameter list of the event object.
 * Arguments:
 * Input:  x    - The event object.
 * Output: None.
 *
 * Return Value: The parameter list.
 *****************************************************************************/
RVAPI const RvMdmParameterList* RVCALLCONV rvMdmEventGetParameterList(IN const RvMdmEvent *x);


void rvMdmEventSetParametersList(RvMdmEvent *x, RvMdmParameterList* params);

/*$
{type:
	{name: RvMdmSignalType}
	{include: rvmdmobjects.h}
	{description:
		{p: Signal type:}
		{p: RV_MDMSIGNAL_DEFAULTTYPE - uses the normal type for the given signal.}
		{p: RV_MDMSIGNAL_ONOFF - the signal lasts until it is turned off.}
		{p: RV_MDMSIGNAL_TIMEOUT - the signal lasts until it is turned off or a specific
				period of time elapses}
		{p: RV_MDMSIGNAL_BRIEF - the signal duration is so short that it will stop on its
				own unless a new signal is applied that causes it to stop}
	}
}
$*/

typedef enum
{
	RV_MDMSIGNAL_DEFAULTTYPE,
	RV_MDMSIGNAL_ONOFF,
	RV_MDMSIGNAL_TIMEOUT,
	RV_MDMSIGNAL_BRIEF
} RvMdmSignalType;

typedef enum
{
	RV_MDMSIGNAL_COMPLETED = 1,
	RV_MDMSIGNAL_GOTEVENT = 2,
	RV_MDMSIGNAL_NEWSIGDESCR = 4,
	RV_MDMSIGNAL_OTHERREASON = 8
} RvMdmSignalNotificationReasons;

/*$
{type:
	{name: RvMdmSignalEx}
	{include: rvmdmobjects.h}
	{description: {p: A signal.}}
	{methods:
		{method: RvMdmSignalEx *rvMdmSignalExConstructA(RvMdmSignalEx *x, const RvMdmPackageItem *name, RvAlloc *alloc);}
		{method: RvMdmSignalEx *rvMdmSignalExConstructCopy(RvMdmSignalEx *d, const RvMdmSignalEx *s, RvAlloc *alloc);}
		{method: void rvMdmSignalExDestruct(RvMdmSignalEx *x);}
		{method: void rvMdmSignalExSetType(RvMdmSignalEx *x, RvMdmSignalType type);}
		{method: void rvMdmSignalExSetDuration(RvMdmSignalEx *x, unsigned int duration);}
		{method: const RvMdmParameterList *rvMdmSignalExGetParameterList(const RvMdmSignalEx *x);}
		{method: const void rvMdmSignalExSetParameter(const RvMdmSignalEx *x, char* name, RvMdmParameterValue *value);}
		{method: const RvMdmPackageItem *rvMdmSignalExGetName(const RvMdmSignalEx *x);}
		{method: RvMdmSignalType rvMdmSignalExGetType(const RvMdmSignalEx *x);}
		{method: unsigned int rvMdmSignalExGetDuration(const RvMdmSignalEx *x);}
		{method: void rvMdmSignalExSetKeepActiveFlag(RvMdmSignalEx *x, RvBool enable);}
		{method: RvBool rvMdmSignalExGetKeepActiveFlag(const RvMdmSignalEx *x);}

	}
}
$*/
typedef struct
{
	RvMdmEvent event;
	RvMdmSignalType type;
	unsigned int duration;
} RvMdmSignalEx;

RvMdmSignalEx *rvMdmSignalExConstructA(RvMdmSignalEx *x, const RvMdmPackageItem *name, RvAlloc *alloc);
RvMdmSignalEx *rvMdmSignalExConstructCopy(RvMdmSignalEx *d, const RvMdmSignalEx *s, RvAlloc *alloc);
void rvMdmSignalExDestruct(RvMdmSignalEx *x);
void rvMdmSignalExSetType(RvMdmSignalEx *x, RvMdmSignalType type);
void rvMdmSignalExSetDuration(RvMdmSignalEx *x, unsigned int duration);
#define rvMdmSignalExGetParameterList(x) rvMdmEventGetParameterList(&(x)->event)
#define rvMdmSignalSetParameter(x, name, value) rvMdmEventSetParameter(&(x)->event, (name), (value))
#define rvMdmSignalExGetName(x) rvMdmEventGetName(&(x)->event)
RvMdmSignalType rvMdmSignalExGetType(const RvMdmSignalEx *x);
unsigned int rvMdmSignalExGetDuration(const RvMdmSignalEx *x);

rvDeclareVector(RvMdmSignalEx)

/*$
{type:
	{name: RvMdmSignalExList}
	{include: rvmdmobjects.h}
	{description: {p: A list of signals.}}
	{methods:
		{method: RvMdmSignalExList *rvMdmSignalExListConstructCopy(RvMdmSignalExList *d, const RvMdmSignalExList *s, RvAlloc *alloc);}
		{method: void rvMdmSignalExListDestruct(RvMdmSignalExList *x);}
		{method: unsigned int rvMdmSignalExListGetId(const RvMdmSignalExList *x);}
		{method: size_t rvMdmSignalExListGetSize(const RvMdmSignalExList *x);}
		{method: const RvMdmSignalEx *rvMdmSignalExListGetElem(const RvMdmSignalExList *x, size_t index);}

	}
}
$*/
typedef struct
{
	unsigned int id;
	RvVector(RvMdmSignalEx) signals;
} RvMdmSignalExList;

RvMdmSignalExList *rvMdmSignalExListConstructCopy(RvMdmSignalExList *d, const RvMdmSignalExList *s, RvAlloc *alloc);
void rvMdmSignalExListDestruct(RvMdmSignalExList *x);
unsigned int rvMdmSignalExListGetId(const RvMdmSignalExList *x);
size_t rvMdmSignalExListGetSize(const RvMdmSignalExList *x);
const RvMdmSignalEx *rvMdmSignalExListGetElem(const RvMdmSignalExList *x, size_t index);

rvDeclareVector(RvMdmSignalExList)

/*$
{type:
	{name: RvMdmSignalsDescriptor}
	{include: rvmdmobjects.h}
	{description: {p: A signals descriptor.}}
	{methods:
		{method: RvMdmSignalsDescriptor *rvMdmSignalsDescriptorConstructA(RvMdmSignalsDescriptor *x, RvAlloc *alloc);}
		{method: RvMdmSignalsDescriptor *rvMdmSignalsDescriptorConstructCopy(RvMdmSignalsDescriptor *d, const RvMdmSignalsDescriptor *s, RvAlloc *alloc);}
		{method: void rvMdmSignalsDescriptorDestruct(RvMdmSignalsDescriptor *x);}
		{method: RvMdmSignalsDescriptor *rvMdmSignalsDescriptorCopy(RvMdmSignalsDescriptor *d, const RvMdmSignalsDescriptor *s);}
		{method: void rvMdmSignalsDescriptorAddSignal(RvMdmSignalsDescriptor *x, const RvMdmSignalEx *signal);}
		{method: size_t rvMdmSignalsDescriptorGetNumSignals(const RvMdmSignalsDescriptor *x);}
		{method: const RvMdmSignalEx *rvMdmSignalsDescriptorGetSignal(const RvMdmSignalExsDescriptor *x, size_t index);}
		{method: size_t rvMdmSignalsDescriptorGetNumSignalLists(const RvMdmSignalsDescriptor *x);}
		{method: const RvMdmSignalExList *rvMdmSignalsDescriptorGetSignalList(const RvMdmSignalsDescriptor *x, size_t index);}

	}
}
$*/

typedef struct
{
	RvVector(RvMdmSignalExList) lists;
	RvVector(RvMdmSignalEx) signals;
} RvMdmSignalsDescriptor;

RvMdmSignalsDescriptor *rvMdmSignalsDescriptorConstructA(RvMdmSignalsDescriptor *x, RvAlloc *alloc);
RvMdmSignalsDescriptor *rvMdmSignalsDescriptorConstructCopy(RvMdmSignalsDescriptor *d, const RvMdmSignalsDescriptor *s, RvAlloc *alloc);
void rvMdmSignalsDescriptorDestruct(RvMdmSignalsDescriptor *x);
RvMdmSignalsDescriptor *rvMdmSignalsDescriptorCopy(RvMdmSignalsDescriptor *d, const RvMdmSignalsDescriptor *s);
void rvMdmSignalsDescriptorAddSignal(RvMdmSignalsDescriptor *x, const RvMdmSignalEx *signal);
size_t rvMdmSignalsDescriptorGetNumSignals(const RvMdmSignalsDescriptor *x);
const RvMdmSignalEx *rvMdmSignalsDescriptorGetSignal(const RvMdmSignalsDescriptor *x, size_t index);
size_t rvMdmSignalsDescriptorGetNumSignalLists(const RvMdmSignalsDescriptor *x);
const RvMdmSignalExList *rvMdmSignalsDescriptorGetSignalList(const RvMdmSignalsDescriptor *x, size_t index);



/*$
{type:
	{name: RvMdmRequestedEvent}
	{include: rvmdmobjects.h}
	{description: {p: A requested event.}}
	{methods:
		{method: RvMdmRequestedEvent *rvMdmRequestedEventConstruct(RvMdmRequestedEvent *x, const RvMdmPackageItem *name);}
		{method: RvMdmRequestedEvent *rvMdmRequestedEventConstructA(RvMdmRequestedEvent *x, const RvMdmPackageItem *name, RvAlloc *alloc);}
		{method: RvMdmRequestedEvent *rvMdmRequestedEventConstructCopy(RvMdmRequestedEvent *d, const RvMdmRequestedEvent *s, RvAlloc *alloc);}
		{method: void rvMdmRequestedEventDestruct(RvMdmRequestedEvent *x);}
		{method: void rvMdmRequestedEventSetParameter(RvMdmRequestedEvent *x, const char *name, const RvMdmParameterValue *value);}
		{method: const RvMdmPackageItem *rvMdmRequestedEventGetName(const RvMdmRequestedEvent *x);}
		{method: const RvMdmParameterList *rvMdmRequestedEventGetParameterList(const RvMdmRequestedEvent *x);}
		{method: const RvMdmDigitMapDescriptor *rvMdmRequestedEventGetEmbDigitMap(const RvMdmRequestedEvent *x);}
	}
}
$*/

typedef struct
{
	RvMdmEvent event;
    RvMdmDigitMapDescriptor         digitMap;
	RvMdmSignalsDescriptor signals;
	struct RvMdmEventsDescriptor_ *events;
} RvMdmRequestedEvent;

RvMdmRequestedEvent *rvMdmRequestedEventConstruct(RvMdmRequestedEvent *x, const RvMdmPackageItem *name);
RvMdmRequestedEvent *rvMdmRequestedEventConstructA(RvMdmRequestedEvent *x, const RvMdmPackageItem *name, RvAlloc *alloc);
RvMdmRequestedEvent *rvMdmRequestedEventConstructCopy(RvMdmRequestedEvent *d, const RvMdmRequestedEvent *s, RvAlloc *alloc);
void rvMdmRequestedEventDestruct(RvMdmRequestedEvent *x);
void rvMdmRequestedEventSetParameter(RvMdmRequestedEvent *x, const char *name, const RvMdmParameterValue *value);

#define rvMdmRequestedEventGetName(x) rvMdmEventGetName(&(x)->event)

#define rvMdmRequestedEventGetParameterList(x) rvMdmEventGetParameterList(&(x)->event)

/*$
{function:
	{name: rvMdmRequestedEventSetParameter}
	{class: RvMdmRequestedEvent}
	{include: rvmdmobjects.h}
	{description:
		{p: Sets a parameter in the requested event object.}
	}
	{proto: void rvMdmRequestedEventSetParameter(RvMdmRequestedEvent *x, const char *name, const RvMdmParameterValue *value);}
	{params:
		{param: {n:x} {d:The requested event object.}}
		{param: {n:name} {d:The parameter name.}}
		{param: {n:value} {d:The parameter value.}}
	}
}
$*/
#define rvMdmRequestedEventSetParameter(x, name, value) rvMdmEventSetParameter(&(x)->event, (name), (value))


const RvMdmDigitMapDescriptor *rvMdmRequestedEventGetEmbDigitMap(const RvMdmRequestedEvent *x);

rvDeclareVector(RvMdmRequestedEvent)


/*$
{type:
	{name: RvMdmEventsDescriptor}
	{include: rvmdmobjects.h}
	{description: {p: An events descriptor.}}
	{methods:
		{method: RvMdmEventsDescriptor *rvMdmEventsDescriptorConstruct(RvMdmEventsDescriptor *x, unsigned int id);}
		{method: RvMdmEventsDescriptor *rvMdmEventsDescriptorConstructA(RvMdmEventsDescriptor *x, unsigned int id, RvAlloc *alloc);}
		{method: RvMdmEventsDescriptor *rvMdmEventsDescriptorConstructCopy(RvMdmEventsDescriptor *d, const RvMdmEventsDescriptor *s, RvAlloc *alloc);}
		{method: RvMdmEventsDescriptor *rvMdmEventsDescriptorCopy(RvMdmEventsDescriptor *d, const RvMdmEventsDescriptor *s);}
		{method: void rvMdmEventsDescriptorDestruct(RvMdmEventsDescriptor *x);}
		{method: void rvMdmEventsDescriptorAddEvent(RvMdmEventsDescriptor *x, const RvMdmRequestedEvent *event);}
		{method: const RvMdmRequestedEvent *rvMdmEventsDescriptorGetEvent(const RvMdmEventsDescriptor *x, size_t index);}
		{method: size_t rvMdmEventsDescriptorGetNumEvents(const RvMdmEventsDescriptor *x);}
		{method: RvBool rvMdmEventsDescriptorIsSet(const RvMdmEventsDescriptor *x);}
		{method: void rvMdmEventsDescriptorClear(RvMdmEventsDescriptor *x);}
	}
}
$*/

typedef struct RvMdmEventsDescriptor_
{
	unsigned int requestId;
	RvVector(RvMdmRequestedEvent) events;
} RvMdmEventsDescriptor;

RvMdmEventsDescriptor *rvMdmEventsDescriptorConstruct(RvMdmEventsDescriptor *x, unsigned int id);
RvMdmEventsDescriptor *rvMdmEventsDescriptorConstructA(RvMdmEventsDescriptor *x, unsigned int id, RvAlloc *alloc);
RvMdmEventsDescriptor *rvMdmEventsDescriptorConstructCopy(RvMdmEventsDescriptor *d, const RvMdmEventsDescriptor *s, RvAlloc *alloc);
RvMdmEventsDescriptor *rvMdmEventsDescriptorCopy(RvMdmEventsDescriptor *d, const RvMdmEventsDescriptor *s);
void rvMdmEventsDescriptorDestruct(RvMdmEventsDescriptor *x);
void rvMdmEventsDescriptorAddEvent(RvMdmEventsDescriptor *x, const RvMdmRequestedEvent *event);
const RvMdmRequestedEvent *rvMdmEventsDescriptorGetEvent(const RvMdmEventsDescriptor *x, size_t index);
size_t rvMdmEventsDescriptorGetNumEvents(const RvMdmEventsDescriptor *x);
RvBool rvMdmEventsDescriptorIsSet(const RvMdmEventsDescriptor *x);
void rvMdmEventsDescriptorClear(RvMdmEventsDescriptor *x);


/* RvMdmStreamMode
 * ------------------------------------------------------------------------
 * Media streams can be either incoming or outgoing in terms of the media
 * being sent on them. They can also be bidirectional. This enumeration
 * enables the application to know what mode a specific stream uses,
 * allowing it to handle the connection of the different devices with the
 * RTP accordingly.
 */
typedef enum
{
	RV_MDMSTREAMMODE_NOTSET,
        /* The stream mode is yet unknown. */

	RV_MDMSTREAMMODE_INACTIVE,
        /* The stream is inactive. It can probably be closed. */

	RV_MDMSTREAMMODE_SENDONLY,
        /* The stream supports sending only. */

	RV_MDMSTREAMMODE_RECVONLY,
        /* The stream supports receiving only. */

	RV_MDMSTREAMMODE_SENDRECV,
        /* The stream supports both sending and receiving. */

	RV_MDMSTREAMMODE_LOOPBACK
        /* This enumeration value is reserved for future use. */
} RvMdmStreamMode;

extern const RvMdmStreamId rvMdmStreamIdDefault;


/*$
{type:
	{name: RvMdmStreamDescriptor}
	{include: rvmdmobjects.h}
	{description: {p: A stream descriptor.}}
	{methods:
		{method: RvMdmStreamDescriptor *rvMdmStreamDescriptorConstructA(RvMdmStreamDescriptor *x, RvMdmStreamId streamId, RvAlloc *alloc);}
		{method: RvMdmStreamDescriptor *rvMdmStreamDescriptorConstructCopy(RvMdmStreamDescriptor *d, const RvMdmStreamDescriptor *s, RvAlloc *alloc);}
		{method: void rvMdmStreamDescriptorDestruct(RvMdmStreamDescriptor *x);}
		{method: void rvMdmStreamDescriptorAddLocalDescriptor(RvMdmStreamDescriptor *x, const RvSdpMsg *msg);}
		{method: void rvMdmStreamDescriptorAddRemoteDescriptor(RvMdmStreamDescriptor *x, const RvSdpMsg *msg);}
		{method: RvMdmStreamId rvMdmStreamDescriptorGetId(const RvMdmStreamDescriptor *x);}
		{method: RvMdmStreamMode rvMdmStreamDescriptorGetMode(const RvMdmStreamDescriptor *x);}
		{method: void rvMdmStreamDescriptorSetMode(RvMdmStreamDescriptor *x, RvMdmStreamMode mode);}
		{method: size_t rvMdmStreamDescriptorGetNumLocalDescriptors(const RvMdmStreamDescriptor *x);}
		{method: size_t rvMdmStreamDescriptorGetNumRemoteDescriptors(const RvMdmStreamDescriptor *x);}
		{method: const RvSdpMsg *rvMdmStreamDescriptorGetLocalDescriptor(const RvMdmStreamDescriptor *x, size_t index);}
		{method: const RvSdpMsg *rvMdmStreamDescriptorGetRemoteDescriptor(const RvMdmStreamDescriptor *x, size_t index);}
		{method: RvBool rvMdmStreamDescriptorIsLocalDescriptorSet(const RvMdmStreamDescriptor *x);}
		{method: RvBool rvMdmStreamDescriptorIsRemoteDescriptorSet(const RvMdmStreamDescriptor *x);}
		{method: const RvMdmParameterValue *rvMdmStreamDescriptorGetParameter(RvMdmStreamDescriptor *x, const RvMdmPackageItem *name);}
		{method: const RvMdmParameterList *rvMdmStreamDescriptorGetParameterList(const RvMdmStreamDescriptor *x);}
		{method: RvBool rvMdmStreamDescriptorIsSet(const RvMdmStreamDescriptor *x);}
		{method: void rvMdmStreamDescriptorAddEmptyLocalDescriptor(RvMdmStreamDescriptor *x);}
		{method: void rvMdmStreamDescriptorAddEmptyRemoteDescriptor(RvMdmStreamDescriptor *x);}
		{method: void rvMdmStreamDescriptorClearLocalDescriptors(RvMdmStreamDescriptor *x);}
		{method: void rvMdmStreamDescriptorClearRemoteDescriptors(RvMdmStreamDescriptor *x);}
	}
}
$*/

typedef struct
{
	RvMdmStreamId       id;
	RvSdpMsgList        localDescriptors;
	RvSdpMsgList        remoteDescriptors;
	RvBool              isLocalSet;
    RvBool              isRemoteSet;
	RvMdmStreamMode     mode;
	RvMdmParameterList  parameters;
} RvMdmStreamDescriptor;

RvMdmStreamDescriptor *rvMdmStreamDescriptorConstructA(RvMdmStreamDescriptor *x, RvMdmStreamId streamId, RvAlloc *alloc);
RvMdmStreamDescriptor *rvMdmStreamDescriptorConstructCopy(RvMdmStreamDescriptor *d, const RvMdmStreamDescriptor *s, RvAlloc *alloc);
void rvMdmStreamDescriptorDestruct(RvMdmStreamDescriptor *x);
void rvMdmStreamDescriptorAddLocalDescriptor(RvMdmStreamDescriptor *x, const RvSdpMsg *msg);
void rvMdmStreamDescriptorAddRemoteDescriptor(RvMdmStreamDescriptor *x, const RvSdpMsg *msg);
RvMdmStreamId rvMdmStreamDescriptorGetId(const RvMdmStreamDescriptor *x);
void rvMdmStreamDescriptorSetId(RvMdmStreamDescriptor *x, RvMdmStreamId id);
RvMdmStreamMode rvMdmStreamDescriptorGetMode(const RvMdmStreamDescriptor *x);
void rvMdmStreamDescriptorSetMode(RvMdmStreamDescriptor *x, RvMdmStreamMode mode);
size_t rvMdmStreamDescriptorGetNumLocalDescriptors(const RvMdmStreamDescriptor *x);
size_t rvMdmStreamDescriptorGetNumRemoteDescriptors(const RvMdmStreamDescriptor *x);
const RvSdpMsg *rvMdmStreamDescriptorGetLocalDescriptor(const RvMdmStreamDescriptor *x, size_t index);
const RvSdpMsg *rvMdmStreamDescriptorGetRemoteDescriptor(const RvMdmStreamDescriptor *x, size_t index);
RvBool rvMdmStreamDescriptorIsLocalDescriptorSet(const RvMdmStreamDescriptor *x);
RvBool rvMdmStreamDescriptorIsRemoteDescriptorSet(const RvMdmStreamDescriptor *x);
const RvMdmParameterValue *rvMdmStreamDescriptorGetParameter(RvMdmStreamDescriptor *x, const RvMdmPackageItem *name);
const RvMdmParameterList *rvMdmStreamDescriptorGetParameterList(const RvMdmStreamDescriptor *x);
RvBool rvMdmStreamDescriptorIsSet(const RvMdmStreamDescriptor *x);
void rvMdmStreamDescriptorAddEmptyLocalDescriptor(RvMdmStreamDescriptor *x);
void rvMdmStreamDescriptorAddEmptyRemoteDescriptor(RvMdmStreamDescriptor *x);
void rvMdmStreamDescriptorClearLocalDescriptors(RvMdmStreamDescriptor *x);
void rvMdmStreamDescriptorClearRemoteDescriptors(RvMdmStreamDescriptor *x);

rvDeclareVector(RvMdmStreamDescriptor)


/*$
{type:
	{name: RvMdmMediaDescriptor}
	{include: rvmdmobjects.h}
	{description: {p: A media descriptor.}}
	{methods:
		{method: RvMdmMediaDescriptor *rvMdmMediaDescriptorConstructA(RvMdmMediaDescriptor *x, RvAlloc *alloc);}
		{method: void rvMdmMediaDescriptorDestruct(RvMdmMediaDescriptor *x);}
		{method: void rvMdmMediaDescriptorAddStream(RvMdmMediaDescriptor *x, const RvMdmStreamDescriptor *stream);}
		{method: void rvMdmMediaDescriptorClearStreams(RvMdmMediaDescriptor *x);}
		{method: size_t rvMdmMediaDescriptorGetNumStreams(const RvMdmMediaDescriptor *x);}
		{method: const RvMdmStreamDescriptor *rvMdmMediaDescriptorGetStream(const RvMdmMediaDescriptor *x, size_t index);}
	}
}
$*/

typedef struct
{
	RvVector(RvMdmStreamDescriptor) streams;
} RvMdmMediaDescriptor;

RvMdmMediaDescriptor *rvMdmMediaDescriptorConstructA(RvMdmMediaDescriptor *x, RvAlloc *alloc);
void rvMdmMediaDescriptorDestruct(RvMdmMediaDescriptor *x);
void rvMdmMediaDescriptorAddStream(RvMdmMediaDescriptor *x, const RvMdmStreamDescriptor *stream);
void rvMdmMediaDescriptorClearStreams(RvMdmMediaDescriptor *x);
size_t rvMdmMediaDescriptorGetNumStreams(const RvMdmMediaDescriptor *x);
const RvMdmStreamDescriptor *rvMdmMediaDescriptorGetStream(const RvMdmMediaDescriptor *x, size_t index);


rvDeclareVector(RvMdmEvent)



typedef struct
{
    RvVector(RvString) packages;
} RvMdmPackagesDescriptor;

RvMdmPackagesDescriptor *rvMdmPackagesDescriptorConstructA(RvMdmPackagesDescriptor *x, RvAlloc *alloc);
void rvMdmPackagesDescriptorDestruct(RvMdmPackagesDescriptor *x);
size_t rvMdmPackagesDescriptorGetNumPackages(const RvMdmPackagesDescriptor *x);
const char *rvMdmPackagesDescriptorGetPackageName(const RvMdmPackagesDescriptor *x, size_t index);
void rvMdmPackagesDescriptorAddPackage(
    IN RvMdmPackagesDescriptor* x,
    IN const RvChar*            name);


/* RvMdmTermPresentationType
 * ------------------------------------------------------------------------
 * The type of presentation that is used for either the caller ID or the called ID.
 * This information is used for privacy purposes.
 */
typedef enum
{
	RV_MDM_PRSENTATION_RESTRICTED	= 0,
        /* The name information should be restricted. */

	RV_MDM_PRSENTATION_ALLOWED		= 1,
        /* The name information can be published publicly. */

	RV_MDM_ADDRESS_NOT_AVAILABLE	= 2,
        /* The related address is unavailable, so no name can be shown. */

	RV_MDM_PRSENTATION_NONE			= 3
        /* No name exists. */
} RvMdmTermPresentationType;


/* RvMdmTermPresentationInfo
 * ------------------------------------------------------------------------
 * The Presentation Info object describes the presentation name to be displayed and
 * the associated permissions for displaying it.
 * This information is set using functions associated with it.
 */
typedef struct
{

    RvChar                          presentationName[RV_NAME_STR_SZ];
    RvMdmTermPresentationType       callerIdPermission;
    RvMdmTermPresentationType       calleeIdPermission;
} RvMdmTermPresentationInfo;


/******************************************************************************
 * rvMdmTermPresentationInfoConstruct
 * ----------------------------------------------------------------------------
 * General:
 * Constructs a new Presentation Info object.
 * By default, permissions for a new object are set to Restricted.
 * Arguments:
 * Input:  x    - The Presentation Info object to construct.
 * Output: None.
 *
 * Return Value: The pointer to the object on success. NULL on failure..
 *****************************************************************************/
RVAPI RvMdmTermPresentationInfo* RVCALLCONV rvMdmTermPresentationInfoConstruct(
    IN RvMdmTermPresentationInfo* x);


/******************************************************************************
 * rvMdmTermPresentationInfoGetName
 * ----------------------------------------------------------------------------
 * General:
 *  Returns the presentation name.
 * Arguments:
 * Input:  x        - The Presentation Info object.
 *         nameLen  - The maximum length, in bytes, of the name parameter.
 * Output: name     - The buffer holding the presentation name if the function
 *                    returns RV_TRUE.
 *
 * Return Value: RV_TRUE if successful, RV_FALSE if not.
 *****************************************************************************/
RVAPI RvBool RVCALLCONV rvMdmTermPresentationInfoGetName(
    IN  RvMdmTermPresentationInfo*  x,
    OUT RvChar*                     name,
    IN  RvInt32                     nameLen);


/******************************************************************************
 * rvMdmTermPresentationInfoGetCallerPermission
 * ----------------------------------------------------------------------------
 * General:
 * Returns the Caller ID permission.
 * Arguments:
 * Input:  x        - The Presentation Info object.
 * Output: None
 *
 * Return Value: The Caller ID permission. If x is NULL,
 *               RV_MDM_PRSENTATION_RESTRICTED is returned.
 *****************************************************************************/
RVAPI RvMdmTermPresentationType RVCALLCONV rvMdmTermPresentationInfoGetCallerPermission(
    IN RvMdmTermPresentationInfo* x);


/******************************************************************************
 * rvMdmTermPresentationInfoGetCalleePermission
 * ----------------------------------------------------------------------------
 * General:
 * Returns the connected line ID permission.
 * Arguments:
 * Input:  x        - The Presentation Info object.
 * Output: None.
 *
 * Return Value: The Callee's permission. If x is NULL,
 *               RV_MDM_PRSENTATION_RESTRICTED is returned.
 *****************************************************************************/
RVAPI RvMdmTermPresentationType RVCALLCONV rvMdmTermPresentationInfoGetCalleePermission(
    IN RvMdmTermPresentationInfo* x);


/******************************************************************************
 * rvMdmTermPresentationInfoSetName
 * ----------------------------------------------------------------------------
 * General:
 *  Sets the presentation name.
 * Arguments:
 * Input:  x        - The Presentation Info object.
 *         name     - The presentation name to set. The name will be truncated
 *                    if the given string is too long.
 * Output: None.
 *
 * Return Value: RV_TRUE if successful, RV_FALSE otherwise.
 *****************************************************************************/
RVAPI RvBool RVCALLCONV rvMdmTermPresentationInfoSetName(
    IN RvMdmTermPresentationInfo*   x,
    IN const RvChar*                name);


/******************************************************************************
 * rvMdmTermPresentationInfoSetCallerPermission
 * ----------------------------------------------------------------------------
 * General:
 *  Sets the Caller Presentation permission.
 * Arguments:
 * Input:  x            - The Presentation Info object.
 *         permission   - The presentation permission to set.
 * Output: None.
 *
 * Return Value: RV_TRUE if successful, RV_FALSE otherwise.
 *****************************************************************************/
RVAPI RvBool RVCALLCONV rvMdmTermPresentationInfoSetCallerPermission(
    IN RvMdmTermPresentationInfo*   x,
    IN RvMdmTermPresentationType    permission);


/******************************************************************************
 * rvMdmTermPresentationInfoSetCalleePermission
 * ----------------------------------------------------------------------------
 * General:
 *  Sets the Callee Presentation permission.
 * Arguments:
 * Input:  x            - The Presentation Info object.
 *         permission   - The presentation permission to set.
 * Output: None.
 *
 * Return Value: RV_TRUE if successful, RV_FALSE otherwise.
 *****************************************************************************/
RVAPI RvBool RVCALLCONV rvMdmTermPresentationInfoSetCalleePermission(
    IN RvMdmTermPresentationInfo*   x,
    IN RvMdmTermPresentationType    permission);



#define rvMdmTermPresentationInfoCopy(_d, _s) memcpy(_d, _s, sizeof(*(_d)))



/* RvMdmTermPhoneNumbers
 * ------------------------------------------------------------------------
 * List of phone numbers.
 * This object should not be accessed directly by the user application. The MTF
 * wraps it up with rvMdmTerm APIs instead.
 */
typedef struct
{
    RvChar      phoneNumbers[RV_MDM_PHONENUMBERS_SIZE][RV_NAME_STR_SZ];
    RvUint      phoneNumbersNum;

} RvMdmTermPhoneNumbers;

#define rvMdmTermPhoneNumbersConstruct(_x) memset(_x, 0, sizeof(*(_x)))
#define rvMdmTermPhoneNumbersClear(_x) memset(_x, 0, sizeof(*(_x)))
#define rvMdmTermPhoneNumbersCopy(_d, _s) memcpy(_d, _s, sizeof(*(_d)))
RvStatus rvMdmTermPhoneNumbersAdd(
    IN RvMdmTermPhoneNumbers*   x,
    IN const RvChar*            number);
const RvChar* rvMdmTermPhoneNumbersGetByIndex(
    IN RvMdmTermPhoneNumbers*   x,
    IN RvSize_t                 index);
#define rvMdmTermPhoneNumbersGetSize(_x) ((_x)->phoneNumbersNum)


#if defined(__cplusplus)
}
#endif

#endif /*RV_MDMOBJECTS_H*/
