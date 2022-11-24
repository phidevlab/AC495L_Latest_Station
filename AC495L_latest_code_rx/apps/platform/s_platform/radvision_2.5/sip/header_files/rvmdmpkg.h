/******************************************************************************
Filename   : rvmdmpkg.h
Description: Package,event and signal information for Media Device Manager
******************************************************************************
                Copyright (c) 1999 RADVision Inc.
************************************************************************
NOTICE:
This document contains information that is proprietary to RADVision LTD.
No part of this publication may be reproduced in any form whatsoever 
without written prior approval by RADVision LTD..

RADVision LTD. reserves the right to revise this publication and make 
changes without obligation to notify any person of such revisions or 
changes.
******************************************************************************
$Revision:$
$Date:1.9.01$
$Author: D.Elbert$
******************************************************************************/


#ifndef RV_MDMPKG_H
#define RV_MDMPKG_H

#include "rvalloc.h"
#include "rvmap.h"
#include "rvstring.h"
#include "rvtypes.h"
#include "rvmdmobjects.h"

/* Forward declarations */
struct RvMdmTerm_;
struct RvMdmTermMgr_;

typedef void RvMdmMediaStream;

/*------------------------------------------------------------------------------*/
/*$
{type:
	{name: RvMdmEventData}
	{include: rvmdm.h}
	{description: {p: Describes the characteristics of an event.}
				  {p: Use to register an event with a package.}
	}
}
$*/


/*------------------------------------------------------------------------------*/
/*$
{type:
	{name: RvMdmSignalData}
	{include: rvmdm.h}
	{description: {p: Describes the characteristics of a signal.}
				   {p: Use to register a signal with a package.}}
}
$*/

typedef struct
{
	RvString            id;
	RvMdmParameterList  args;
	RvMdmSignalType     type;
	RvUint32            timeout;
} RvMdmSignalData;


void rvMdmSignalDataConstructA(RvMdmSignalData* x,const char* id,RvMdmSignalType type,RvAlloc* a);
void rvMdmSignalDataDestruct(RvMdmSignalData* x);
void rvMdmSignalDataSetTimeout(RvMdmSignalData* x,RvMilliseconds timeout);

/*------------------------------------------------------------------------------*/
/* Definitions used to register packages supporting digit maps and to generate  */
/* completion events                                                            */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*$
{callback:
	{name: RvMdmDigitMapBuildCompletionEvCB}	
	{include: rvmdm.h}
	{description:
		{p: Template for a callback function that is used to build a digitmap 
			completion event for a given package.
			This callback will be called by the Media Device Manager (when it generates 
			a digitMap completion event) to add package specific information. }

	}
	{proto: void(*RvMdmDigitMapBuildCompletionEvCB)(OUT RvMdmParameterList* parameters,
													const char* digitString,
													RvMdmDigitMapMatchType matchType,
													void* userData);}
	{params:
		{param: {n:parameters} {d:Add output parameters for the completion event to this list.}}
		{param: {n:digitString}  {d:The string of digits accumulated according to a digitmap.}}
		{param: {n:matchType} {d:The status of the match between the string and the digitmap.}}
		{param: {n:userData} {d:User data set in the RvMdmDigitMapData constructor.}}
	}
	{notes:
		{note: 
			Register only for packages which support digitmap events.
		}
	}
}
$*/
typedef void(*RvMdmDigitMapBuildCompletionEvCB)(RvMdmParameterList* parameters,
												 const char* digitString,
												 RvMdmDigitMapMatchType matchType,
												 void* userData);

/* Return mapped character or zero */
/*------------------------------------------------------------------------------*/
/*$
{callback:
	{name: RvMdmDigitMapTranslateEventCB}	
	{include: rvmdm.h}
	{description:
		{p: Translates (maps) the events of the package to valid digitmap 
		    symbols ('0'-'9', 'A'-'F'). 
		 }
	}
	{proto: char (*RvMdmDigitMapTranslateEventCB)(const char* eventName, 
												  const RvMdmParameterList* parameters);}
	{params:
		{param: {n:eventName} {d:The name of the event.}}
		{param: {n:parameters} {d:The parameter list of the event.}}
	}
	{returns: 
		The digitmap symbol for this event or RV_MDM_NOMATCH if the event 
		doesn't map to a digitmap symbol, RV_MDMEVENT_IGNORE if the event is to 
		be ignored.
	}
	{notes:
		{note: 
			This callback will be called by the Media Device Manager when it receives
			an event from the package.
		}
		{note: 
			Register only for packages which support digitmap events.
		}
	}
}
$*/
typedef unsigned char (*RvMdmDigitMapTranslateEventCB)(const char* eventName, 
											  const RvMdmParameterList* parameters);

/*$
{type:
	{name: RvMdmDigitMapData}
	{include: rvmdm.h}
	{description: {p: Defines the package specific handling of digitmap events,including the 
					  mapping of package events to digitmap symbols and the production of the
					  digitmap completion event.}}
	{notes:
		{note:  Use only for packages implementing digitmaps events. }
	}
}
$*/


typedef struct RvMdmDigitMapData_
{
	RvMdmPackageItem				  pkgItem;
	RvMdmDigitMapBuildCompletionEvCB  evCompleteF;
	RvMdmDigitMapTranslateEventCB	  translateF;
	RvAlloc * alloc;
	void * userData;
} RvMdmDigitMapData;

void rvMdmDigitMapDataConstruct(RvMdmDigitMapData* dmData,
								const char* completionEventName,
								RvMdmDigitMapBuildCompletionEvCB completeEvF,
								RvMdmDigitMapTranslateEventCB    translateF,
								void * userData);
void rvMdmDigitMapDataDestruct(RvMdmDigitMapData* dmData);

void rvMdmDigitMapDataCopyConstruct(RvMdmDigitMapData* d,RvMdmDigitMapData* s,RvAlloc* a);

/*---------------------------------------------------------------------------------*/
/* Internal objects related to package information                                 */
/*---------------------------------------------------------------------------------*/

typedef struct
{
	RvMdmSignalType type;
	RvMilliseconds  duration;
} RvMdmSignalInfo;


#define RvMdmSignalInfoDestruct rvDefaultDestruct
#define RvMdmSignalInfoConstructCopy rvDefaultConstructCopy

rvDeclareMap(RvIString, RvMdmSignalInfo)

/*------------------------------------------------------------------------------*/

/*
registerTermStateProperty(name,value)
registerLocalControlProperty(name,value)
registerStatistic(name)
registerDigitMapInfo
*/

/*$
{type:
	{name: RvMdmPackage}
	{include: rvmdm.h}
	{description: {p: Defines a package (group of signals/events/properties/statistics).}}
}
$*/
/* Save in the package only the information that will be needed later */
typedef struct RvMdmPackage_ {
	struct RvMdmTermMgr_*					mgr;
	RvString								name;
	RvMdmSignalsDescriptor				signals;
	RvMdmEventsDescriptor				events;
	RvMap(RvIString, RvMdmSignalInfo)		signalInfo;
	RvAlloc*								alloc;
	size_t refs; 
} RvMdmPackage;

void rvMdmPackageRegisterSignal(RvMdmPackage* x,RvMdmSignalData* signal);
void rvMdmPackageRegisterEvent(RvMdmPackage* x,const char* eventId);

void rvMdmPackageConstruct_(RvMdmPackage* x,const char* name,struct RvMdmTermMgr_* mgr,RvAlloc* a);
void rvMdmPackageDestruct_(RvMdmPackage* x);

/* Optional */
/* Note: DigitMap data is not actually saved in the package but in the separated 
   digit map database */
void rvMdmPackageRegisterDigitMapData(RvMdmPackage* x,RvMdmDigitMapData* data);

/* Pkg data declaration */
typedef struct RvMdmPackage_ *RvMdmPackagePtr;
rvDeclareMap(RvIString,RvMdmPackagePtr)
typedef RvMap(RvIString,RvMdmPackagePtr) RvMdmPackageData;
#define rvMdmPackageDataConstruct rvMapConstruct(RvIString, RvMdmPackagePtr)
#define rvMdmPackageDataDestruct rvMapDestruct


#endif /* RV_MDMPKG_H */

