/******************************************************************************
Filename   : rvmdm.h
Description: Header file for Media Device Manager. 
			 This file includes deprecated APIs (since version 3.0).
		
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
$Date:11.15.00$
$Author: D.Elbert$
******************************************************************************/
#ifndef RV_MDM_H
#define RV_MDM_H

#ifdef __cplusplus
extern "C" {
#endif


#include "rvansi.h"
#include "rvtypes.h"
#include "rvptrlist.h"
#include "rvalloc.h"
#include "rvsdp.h"
#include "rvmdmobjects.h"
#include "rvmdmpkg.h"

#define  RV_MDM_ADDRESS_SIZE   64
#define  RV_MDM_PHONENUMBERS_SIZE   3
#define  RV_MDMEVENT_IGNORE         (unsigned char)255  /* digitmap event is to be ignored*/
#define  RV_MDMEVENT_NOTFOUND       (unsigned char)0    /* digitmap event could not be translated */

/*------------------------------------------------------------------------------*/
/*  Forward declarations                                                        */
/*------------------------------------------------------------------------------*/

struct RvMdmTerm_;
struct RvMdmTermMgr_;
struct RvMdmTermDefaultProperties_;
struct RvMdmDigitMapData_;

typedef void RvMdmServiceChange; /* Unused, but here for backward compatibility */


/*------------------------------------------------------------------------------*/
/*$
{type:
    {name: RvMdmStreamDirection}
    {include: rvmdm.h}
    {description:
        {p: Describes the direction of connected media flows:}
        {p: RV_MDMSTREAMDIRECTION_ISOLATE - No media flow.}
        {p: RV_MDMSTREAMDIRECTION_BOTHWAYS - Bidirectional media flow.}
        {p: RV_MDMSTREAMDIRECTION_SOURCE2TARGET - Unidirectional flow, from source to target.}
    }
}
$*/

typedef enum
{
    RV_MDMSTREAMDIRECTION_NULL, /* Not used */
    RV_MDMSTREAMDIRECTION_ISOLATE, /* Not used */
    RV_MDMSTREAMDIRECTION_BOTHWAYS,
    RV_MDMSTREAMDIRECTION_SOURCE2TARGET, /* Not used */
    RV_MDMSTREAMDIRECTION_TARGET2SOURCE /* Not used */
} RvMdmStreamDirection;

/*------------------------------------------------------------------------------*/
/*$
{type:
    {name: RvMdmTermType}
    {include: rvmdm.h}
    {description:
        {p: Describes the termination type:}
        {p: RV_MDMTERMTYPE_UNKNOWN   - Undefined or unresolved type (for temporary termination).}
        {p: RV_MDMTERMTYPE_PHYSICAL  - Physical termination.}
        {p: RV_MDMTERMTYPE_EPHEMERAL - Ephemeral termination.}
    }
}
$*/

typedef enum
{
    RV_MDMTERMTYPE_UNKNOWN,
    RV_MDMTERMTYPE_PHYSICAL,
    RV_MDMTERMTYPE_EPHEMERAL
} RvMdmTermType;


typedef enum
{
    RV_MDMTERMREASON_UNKNOWN,        /* 0 */
    RV_MDMTERMREASON_SUCCESS,        /* 1 */
    RV_MDMTERMREASON_IN_PROCESS,     /* 2 - a previous process of modify media has not ended yet*/
    RV_MDMTERMREASON_REMOTE_REJECTED,/* 3 - remote party rejected the request (outgoing)*/
    RV_MDMTERMREASON_LOCAL_FAILED    /* 4 - local failure */
} RvMdmTermReasonModifyMedia;


typedef void RvMdmXTermMgr;
typedef void RvMdmXTerm;
typedef void* RvMdmError;


/*------------------------------------------------------------------------------*/
/*---------------------     RvMdmMediaStreamDescr   ----------------------------*/
/*------------------------------------------------------------------------------*/

typedef enum {
    RV_MDMMEDIASTREAM_REPORTLOCAL   = 1,
    RV_MDMMEDIASTREAM_REPORTREMOTE  = 2,
    RV_MDMMEDIASTREAM_REPORTCONTROL = 4
} RvMdmMediaStreamDescrReport;

typedef enum
{
    RVMDM_CMD_IDLE,
    RVMDM_CMD_CREATE,
    RVMDM_CMD_MUTE_ALL,
    RVMDM_CMD_UNMUTE_ALL,
    RVMDM_CMD_HOLD_LOCAL,
    RVMDM_CMD_HOLD_REMOTE,
    RVMDM_CMD_HOLD_REMOTE_TO_INACTIVE,
    RVMDM_CMD_HOLD_LOCAL_TO_INACTIVE,
    RVMDM_CMD_UNHOLD_LOCAL,
    RVMDM_CMD_UNHOLD_REMOTE,
    RVMDM_CMD_UNHOLD_INACTIVE_TO_LOCAL,
    RVMDM_CMD_UNHOLD_INACTIVE_TO_REMOTE,
    RVMDM_CMD_NORMAL,
    RVMDM_CMD_UNKNOWN
}RvMdmMediaCommand;

typedef union
{
    /*
    * all below struct must start with RvMdmMediaCommand field
    */
    RvMdmMediaCommand cmd;

    struct  {
        RvMdmMediaCommand cmd;
        RvSdpMsg*   remoteSdp;
    }mute_all;                        /*for termConnState = RVMDM_CMD_MUTE*/

    struct  {
        RvMdmMediaCommand cmd;
        RvSdpMsg*   localSdp;
    }hold_local;                        /*for termConnState = RVMDM_CMD_HOLD_LOCAL*/

    struct  {
        RvMdmMediaCommand cmd;
        RvSdpMsg*   remoteSdp;
    }hold_remote;                        /*for termConnState = RVMDM_CMD_HOLD_REMOTE */

    struct  {
        RvMdmMediaCommand cmd;
        RvSdpMsg*   localSdp;
        RvSdpMsg*   remoteSdp;
        RvSdpMsg*   localCapsSdp;
    }normal;                        /*for default: termConnState = RVMDM_CMD_TALKING etc.*/

}RvMdmMediaParam;

typedef struct
{
    RvSdpMsgList*               localDescr;
    RvSdpMsgList*               remoteDescr;
    RvMdmStreamMode             mode;
    RvMdmMediaStreamDescrReport reportFlag;
    RvMdmMediaParam             param;
} RvMdmMediaStreamDescr;

RVAPI RvMdmStreamMode rvMdmMediaStreamDescrGetMode(RvMdmMediaStreamDescr* x);
RVAPI RvSdpMsgList * rvMdmMediaStreamDescrGetLocalDescr(RvMdmMediaStreamDescr* x);
RVAPI RvSdpMsgList * rvMdmMediaStreamDescrGetRemoteDescr(RvMdmMediaStreamDescr* x);
void rvMdmMediaStreamDescrReportLocalDescr(RvMdmMediaStreamDescr* x);
void rvMdmMediaStreamDescrReportRemoteDescr(RvMdmMediaStreamDescr* x);

RvMdmMediaStreamDescr*rvMdmMediaStreamDescrConstruct_(RvMdmMediaStreamDescr* x,
                                           RvMdmStreamMode mode,
                                           RvSdpMsgList * localDescr,
                                           RvSdpMsgList * remoteDescr);

#define rvMdmMediaStreamDescrIsReportRemote_(x) ((int)(x)->reportFlag & (int)RV_MDMMEDIASTREAM_REPORTREMOTE )
#define rvMdmMediaStreamDescrIsReportLocal_(x) ((int)(x)->reportFlag & (int)RV_MDMMEDIASTREAM_REPORTLOCAL )

/*------------------------------------------------------------------------------*/
/*------------------------------        RvMdmSignal ----------------------------*/
/*------------------------------------------------------------------------------*/

/*$
{type:
    {name: RvMdmSignal}
    {include: rvmdm.h}
    {description: {p: Describes a signal applied to a termination.}}
    {protected_methods:
        {method: void rvMdmSignalConstruct_(RvMdmSignal * signal,
                          const char* id,const char* pkg,const RvMdmParameterList* args,
                          RvMdmMediaStream* mediaStream);}
    }
}
$*/

typedef struct
{
    const RvChar*               id;
    const RvChar*               pkg;
    const RvMdmParameterList*   params;
} RvMdmSignal;

void rvMdmSignalConstruct_(RvMdmSignal * signal,
                          const char* id,const char* pkg,const RvMdmParameterList* args);

/*Public:*/
RVAPI const RvChar* RVCALLCONV rvMdmSignalGetId(IN const RvMdmSignal * signal);
RVAPI const RvChar* RVCALLCONV rvMdmSignalGetPkg(IN const RvMdmSignal * signal);
RVAPI const RvMdmParameterList* RVCALLCONV rvMdmSignalGetArguments(IN const RvMdmSignal *signal);



/*------------------------------------------------------------------------------*/
/*------------------    T E R M  M G R  C A L L B A C K S   --------------------*/
/*------------------------------------------------------------------------------*/

/*$
{callback:
    {name: RvMdmTermMgrSelectTerminationCB}
    {include: rvmdm.h}
    {description:
        {p: Called to select a termination (either physical or ephemeral).}

    }
    {proto: RvMdmTerm* (*RvMdmTermMgrSelectTerminationCB)(RvMdmTermMgr* mgr,RvMdmTerm* tempTerm);}
    {params:
        {param: {n:mgr} {d:The termination manager.}}
        {param: {n:tempTerm} {d:A "placeholder" termination used to get information about the requested
                                termination like media requirements,partial name or active context.}}
    }
    {notes:
        {note:
            To select an "ephemeral" termination, first registered it by calling
            rvMdmTermMgrRegisterEphemeralTerm().
            The user can obtain information needed to resolve the type of or the specific
            termination to be selected by querying the tempTerm. For example, if the partial
            name is "rtp/<wildcard>" the user will know that the selected termination
            must be an rtp termination.
        }
    }
}
$*/
typedef struct RvMdmTerm_* (*RvMdmTermMgrSelectTerminationCB)(struct RvMdmTermMgr_* mgr,struct RvMdmTerm_* tempTerm);

/******************************************************************************
 * RvMdmTermMgrDeleteEphTermCB
 * ----------------------------------------------------------------------------
 * General:
 *  This function is called to notify the application that the resources used
 *  for an ephemeral termination can be released.
 *  Returning from this callback, the termination is no longer registered with
 *  the termination manager, and should not be accessed by the application.
 * Arguments:
 * Input:  mgr      - The termination manager.
 *         ephTerm  - The ephemeral termination.
 * Output: None.
 *
 * Return Value: None
 *****************************************************************************/
typedef void (*RvMdmTermMgrDeleteEphTermCB)(
    IN struct RvMdmTermMgr_*    mgr,
    IN struct RvMdmTerm_*       ephTerm);


/*$
{callback:
    {name: rvMdmTermMgrMapAddressToTermCB}
    {include: rvmdm.h}
    {description:
        {p: Maps an address to a termination.}
    }
    {proto: char* (*rvMdmTermMgrMapAddressToTermCB)(struct RvMdmTermMgr_* mgr, char* address);}
    {params:
        {param: {n:mgr} {d:The termination manager.}}
        {param: {n:address} {d:The address.}}
    }
    {returns:
        The termination Id.
    }

}
$*/
typedef char* (*RvMdmTermMgrMapAddressToTermCB)(struct RvMdmTermMgr_* mgr, const char* address);

/*------------------------------------------------------------------------------*/
/*$
{callback:
    {name: RvMdmTermStartSignalCB}
    {include: rvmdm.h}
    {description:
        {p: Called to start a signal in a termination.
            This signal will usually not stop by itself but only when the callback registred
            with rvMdmTermClassRegisterStopSignalCB() is called. }
    }
    {proto: RvBool (*RvMdmTermStartSignalCB)(RvMdmTerm* term,RvMdmSignal * s,OUT RvMdmError* mdmError);}
    {params:
        {param: {n:term} {d:The termination.}}
        {param: {n:s} {d:The signal.}}
        {param: {n:mdmError} {d:(Optional) Use to set error information.}}
    }
    {returns:
        Return "rvFalse" if it fails. In this case mdmError can be set. If is not set, a default error value will be assigned.
    }
    {notes:
        {note:
            Call rvMdmTermGetUserData() to get the user data associated with term.
        }
    }
}
$*/
typedef RvBool (*RvMdmTermStartSignalCB)(
    IN  struct RvMdmTerm_*  term,
    IN  RvMdmSignal*        s,
    OUT RvMdmError*         mdmError);

/*------------------------------------------------------------------------------*/
/*$
{callback:
    {name: RvMdmTermStopSignalCB}
    {include: rvmdm.h}
    {description:
        {p: Called to stop a signal in a termination.}
    }
    {proto: RvBool (*RvMdmTermStopSignalCB)(RvMdmTerm* term,RvMdmSignal * s,OUT RvMdmError* mdmError);}
    {params:
        {param: {n:term} {d:The termination.}}
        {param: {n:s} {d:The signal.}}
        {param: {n:mdmError} {d:(Optional) Use to set error information.}}
    }
    {returns:
        Return "rvFalse" if it fails. In this case mdmError can be set. If is not set, a default error value will be assigned.
    }
    {notes:
        {note:
            Call rvMdmTermGetUserData() to get the user data associated with term.
        }
    }
}
$*/
typedef RvBool (*RvMdmTermStopSignalCB)(struct RvMdmTerm_* term,RvMdmSignal * s,OUT RvMdmError* mdmError);


/*------------------------------------------------------------------------------*/
/*$
{callback:
    {name: RvMdmTermPlaySignalCB}
    {include: rvmdm.h}
    {description:
        {p: Called to play a signal on a termination. This signal is played to completion
            by the application and not stopped by the termination manager.}

    }
    {proto: RvBool (*RvMdmTermPlaySignalCB)(RvMdmTerm* term,RvMdmSignal * s,RvBool reportCompletion,
                                             OUT RvMdmError* mdmError);}
    {params:
        {param: {n:term} {d:The termination.}}
        {param: {n:s} {d:The signal.}}
        {param: {n:reportCompletion}
            {d:If rvTrue, the application must call rvMdmTermSignalCompleted when the signal ends.}}
        {param: {n:mdmError} {d:(Optional) Use to set error information.}}
    }
    {returns:
        Return "rvFalse" if it fails. In this case mdmError can be set. If is not set, a default error value will be assigned.
    }
    {notes:
        {note:
            Call rvMdmTermGetUserData() to get the user data associated with term.
        }
    }
}
$*/
typedef RvBool (*RvMdmTermPlaySignalCB)(
    IN  struct RvMdmTerm_*  term,
    IN  RvMdmSignal*        s,
    IN  RvBool              reportCompletion,
    OUT RvMdmError*         mdmError);

/*------------------------------------------------------------------------------*/
/*$
{callback:
    {name: RvMdmTermCreateMediaCB}
    {include: rvmdm.h}
    {description:
        {p: Functions that follow this template are called to create a media stream on a
            termination with the appropiate media parameters}

    }
    {proto: typedef RvBool (*RvMdmTermCreateMediaCB)(RvMdmTerm* term,RvMdmMediaStream* media,
                                                INOUT RvMdmMediaStreamDescr* streamDescr,
                                                OUT RvMdmError* mdmError);
    }
    {params:
        {param: {n:term} {d:Termination.}}
        {param: {n:media} {d:Pointer to the new media object.}}
        {param: {n:streamDescr} {d:(Input/Output) Properties of the media.}}
        {param: {n:mdmError} {d:(Optional) Use to set error information.}}
    }
    {returns:
        Return RvFalse if it fails. In this case mdmError can be set.
        If is not set, a default error value will be assigned.
        In return streamDescr must contain only the selected values (The application must fill
        underspecified values and choose from overspecified values).
        The application must call rvMdmMediaStreamDescrReportLocalDescr(), rvMdmMediaStreamDescrReportRemoteDescr(),
        in the streamDescr to indicate which fields were modified and have to be reported back to the MGC.
    }
}
$*/

typedef RvBool (*RvMdmTermCreateMediaCB)(
    IN    struct RvMdmTerm_*        term,
    IN    RvMdmMediaStream*         media,
    INOUT RvMdmMediaStreamDescr*    streamDescr,
    OUT   RvMdmError*               mdmError);


/*------------------------------------------------------------------------------*/
/*$
{callback:
    {name: RvMdmTermDestroyMediaCB}
    {include: rvmdm.h}
    {description:
        {p: Functions that follow this template are called to close a media stream
            on a termination and release resources.}

    }
    {proto: RvBool (*RvMdmTermDestroyMediaCB)(RvMdmTerm* term,RvMdmMediaStream* media,
                                               OUT RvMdmError* mdmError);
    }
    {params:
        {param: {n:term} {d:Termination.}}
        {param: {n:mediaStream} {d:user identifier of the media.}}
        {param: {n:mdmError} {d:(Optional) Use to set error information.}}
    }
    {returns:
        Return rvFalse of it fails. In this case mdmError can be set.
        If is not set, a default error value will be assigned.
    }
}
$*/
typedef RvBool (*RvMdmTermDestroyMediaCB)(struct RvMdmTerm_* term,RvMdmMediaStream* media,OUT RvMdmError* mdmError);

/*------------------------------------------------------------------------------*/
/*$
{callback:
    {name: RvMdmTermModifyMediaCB}
    {include: rvmdm.h}
    {description:
        {p: Functions that follow this template are called to modify the characteristics
            of a media stream on a termination.}

    }
    {proto: RvBool (*RvMdmTermModifyMediaCB)(RvMdmTerm* term,RvMdmMediaStream* media,
                                         INOUT RvMdmMediaStreamDescr* streamDescr,RvMdmError* mediaError);
    }
    {params:
        {param: {n:term} {d:Termination.}}
        {param: {n:mediaStream} {d:user identifier of the media stream.}}
        {param: {n:streamDescr} {d:(Input/Output) Properties of the media stream.}}
        {param: {n:mediaError} {d:(Optional) Use to set error information.}}
    }
    {returns:
        Return rvFalse of it fails. In this case mediaError can be set.
        If is not set, a default error value will be assigned.
        In return streamDescr must contain only the selected values (The application must fill
        underspecified values and choose from overspecified values).
        The application must call rvMdmMediaStreamDescrReportLocalDescr(), rvMdmMediaStreamDescrReportRemoteDescr(),
        in the streamDescr to indicate which fields were modified and have to be reported back to the MGC.
    }
}
$*/
typedef RvBool (*RvMdmTermModifyMediaCB)(struct RvMdmTerm_* term,
                                         RvMdmMediaStream* media,
                                         RvMdmMediaStreamDescr* streamDescr,
                                         RvMdmError* mediaError);


/*------------------------------------------------------------------------------*/
/*$
{callback:
    {name: RvMdmTermRegisterPhysTermCompletedCB}
    {include: rvmdm.h}
    {description:
        {p: Functions that follow this template are called to inform the application that the
            operation of registering a physical termination has been completed, if called asynchronous .}

    }
    {proto: void (*RvMdmTermRegisterPhysTermCompletedCB)(RvMdmTerm* term, RvMdmError* mdmError);
    }
    {params:
        {param: {n:term} {d:Termination.}}
        {param: {n:mdmTerm} {d:Mdm Termination.}}
        {param: {n:mediaError} {d:(Optional) Use to set error information.}}
    }

}
$*/
typedef void (*RvMdmTermRegisterPhysTermCompletedCB)(
    IN struct RvMdmTerm_*   term,
    IN RvMdmError*          mdmError);

/*------------------------------------------------------------------------------*/
/*$
{callback:
    {name: RvMdmTermUnregisterTermCompletedCB}
    {include: rvmdm.h}
    {description:
        {p: Functions that follow this template are called to inform the application that the
            operation of unregistering a termination has been completed, if called asynchronous .}

    }
    {proto: void (*RvMdmTermUnregisterTermCompletedCB)(RvMdmTerm* term, RvMdmError* mdmError);
    }
    {params:
        {param: {n:term} {d:Termination.}}
        {param: {n:mediaError} {d:(Optional) Use to set error information.}}
    }

}
$*/
typedef void (*RvMdmTermUnregisterTermCompletedCB)(
    IN struct RvMdmTerm_*   term,
    IN RvMdmError*          mdmError);


/*------------------------------------------------------------------------------*/
/*$
{callback:
    {name: RvMdmTermUnregisterTermFromNetworkCompletedCB}
    {include: rvmdm.h}
    {description:
        {p: Functions that follow this template are called to inform the application that the
            operation of unregistering a termination has been completed, if called asynchronous .}

    }
    {proto: void (*RvMdmTermUnregisterTermFromNetworkCompletedCB)(RvMdmTerm* term, RvMdmError* mdmError);
    }
    {params:
        {param: {n:term} {d:Termination.}}
        {param: {n:mediaError} {d:(Optional) Use to set error information.}}
    }

}
$*/
typedef void (*RvMdmTermUnregisterTermFromNetworkCompletedCB)(
    IN struct RvMdmTerm_*   term,
    IN RvMdmError*          mdmError);

/*------------------------------------------------------------------------------*/
/*$
{callback:
    {name: RvMdmTermModifyMediaCompletedCB}
    {include: rvmdm.h}
    {description:
        {p: Functions that follow this template are called to inform the application that the
            operation of modifying media has been completed.}

    }
    {proto: void (*RvMdmTermModifyMediaCompletedCB)(RvMdmTerm* term, RvMdmMediaStreamDescr* media, RvMdmStreamDescriptor* streamDescr, IN unsigned int status);
    }
    {params:
        {param: {n:term} {d:Termination.}}
        {param: {n:status} {d:Status of the process: True if succeeded, False if failed}}
        {param: {n:media} {d:new media parameters}}
        {param: {n:streamDescr} {d:media parameters returned by the user}}
        {param: {n:reason} {d:indicates the reason for the status}}
    }

}
$*/
typedef void (*RvMdmTermModifyMediaCompletedCB)(struct RvMdmTerm_* term,
                                               RvBool status,
                                               RvMdmMediaDescriptor* media,
                                               RvMdmMediaStreamDescr* streamDescr,
                                               RvMdmTermReasonModifyMedia reason);


/*------------------------------------------------------------------------------*/
/*$
{callback:
    {name: RvMdmTermMatchDialStringCB}
    {include: rvmdm.h}
    {description:
        {p: Checks for matching of the dialed string to a digitmap. The user can implement
            this function or use the default one.}
    }
    {proto: void (*RvMdmTermMatchDialStringCB)(struct RvMdmTerm_* term, const char *dialString, unsigned int *timerDuration);}
    {params:
        {param: {n:term} {d:The termination.}}
        {param: {n:dialString} {d:The dialed string.}}
        {param: {n:timeDuration} {d:Output time duration to wait for the next digit, or a timeout.}}
    }
    {returns:
        The matching result: full/partial/no match).
    }
}
$*/
typedef RvMdmDigitMapMatchType (*RvMdmTermMatchDialStringCB)(
    IN  struct RvMdmTerm_*  term,
    IN  const RvChar*       dialString,
    OUT RvUint*             timerDuration);

/*$
{callback:
    {name: rvMdmTermMgrMapDialStringToAddressCB}
    {include: rvmdm.h}
    {description:
        {p: Maps a dial string to an address.}
    }
    {proto: RvBool (*RvMdmTermMapDialStringToAddressCB)(struct RvMdmTerm_* term, const char* dialString, char* address);}
    {params:
        {param: {n:term} {d:The termination.}}
        {param: {n:dialString} {d:The dial string.}}
        {param: {n:address} {d:The destination address.}}
    }
    {returns:
        True if successful, False if failed.
    }

}
$*/
typedef RvBool (*RvMdmTermMapDialStringToAddressCB)(
    IN  struct RvMdmTerm_*  term,
    IN  const RvChar*       dialString,
    OUT RvChar*             address);




/*------------------------------------------------------------------------------*/
/*$
{callback:
    {name: RvMdmTermMgrConnectCB}
    {include: rvmdm.h}
    {description:
        {p: Called to connect a media stream in one termination to a media stream in another termination.
        }

    }
    {proto: RvBool (*RvMdmTermMgrConnectCB)(RvMdmTermMgr* mgr,RvMdmTerm* source,RvMdmMediaStream* m1,
                                         RvMdmTerm* target,RvMdmMediaStream* m2,
                                         RvMdmStreamDirection direction,OUT RvMdmError* mdmError);}
    {params:
        {param: {n:mgr} {d:The termination manager.}}
        {param: {n:source} {d:First termination.}}
        {param: {n:m1} {d:Media stream in first termination.}}
        {param: {n:target} {d:Second termination.}}
        {param: {n:m2} {d:Media stream in second termination.}}
        {param: {n:direction} {d:Direction of the media flow (RV_MDMSTREAMDIRECTION_BOTHWAYS or RV_MDMSTREAMDIRECTION_SOURCE2TARGET).}
        }
        {param: {n:mdmError} {d:(Optional) Use to set error information.}}
    }
    {returns:
        Return "rvFalse" if it fails. In this case mdmError can be set. If is not set, a default error value will be assigned.
    }
}
$*/
typedef RvBool (*RvMdmTermMgrConnectCB)(struct RvMdmTermMgr_* mgr,struct RvMdmTerm_* source,RvMdmMediaStream* m1,struct RvMdmTerm_* target,RvMdmMediaStream* m2,
                                     RvMdmStreamDirection direction,OUT RvMdmError* mdmError);

/*$
{callback:
    {name: RvMdmTermMgrDisconnectCB}
    {include: rvmdm.h}
    {description:
        {p: Called to disconnect a media stream in a termination from a media stream in another termination.
        }

    }
    {proto: RvBool (*RvMdmTermMgrDisconnectCB)(RvMdmTermMgr* mgr,RvMdmTerm* source,RvMdmMediaStream* m1,
                                               RvMdmTerm* target,RvMdmMediaStream* m2,
                                               OUT RvMdmError* mdmError);}
    {params:
        {param: {n:mgr} {d:The termination manager.}}
        {param: {n:source} {d:First termination.}}
        {param: {n:m1} {d:Media stream in first termination.}}
        {param: {n:target} {d:Second termination.}}
        {param: {n:m2} {d:Media stream in second termination.}}
        {param: {n:mdmError} {d:(Optional) Use to set error information.}}
    }
    {returns:
        Return "rvFalse" if it fails. In this case mdmError can be set.
        If is not set, a default error value will be assigned.
    }
}
$*/
typedef RvBool (*RvMdmTermMgrDisconnectCB)(struct RvMdmTermMgr_* mgr,struct RvMdmTerm_* source,RvMdmMediaStream* m1,
                                        struct RvMdmTerm_* target,RvMdmMediaStream* m2,
                                        OUT RvMdmError* mdmError);



/*------------------------------------------------------------------------------*/
/*----------------------    RvMdmTermClass  ------------------------------------*/
/*------------------------------------------------------------------------------*/

/*$
{type:
    {name: RvMdmTermClass}
    {include: rvmdm.h}
    {description:
        {p: Describes a type of terminations.}
        {p: All the terminations from the same class will share callbacks and capabilities.}
        {p: More than one class can be defined, to support different types of terminations
            with different capabilities and different callback implementations.
        }
    }
    {protected_methods:
        {method: void rvMdmTermClassConstructA_(RvMdmTermClass * c,RvAlloc * a);}
        {method: void rvMdmTermClassDestruct_(RvMdmTermClass * c);}
    }
}
$*/

typedef struct RvMdmTermClass_
{
    RvMdmTermStartSignalCB                  startSignalF;
    RvMdmTermStopSignalCB                   stopSignalF;
    RvMdmTermPlaySignalCB                   playSignalF;
    RvMdmTermCreateMediaCB                  createMediaF;
    RvMdmTermDestroyMediaCB                 destroyMediaF;
    RvMdmTermModifyMediaCB                  modifyMediaF;
    RvMdmTermRegisterPhysTermCompletedCB    registerPhysTermCompletedF;
    RvMdmTermUnregisterTermCompletedCB      unregisterTermCompletedF;
    RvMdmTermUnregisterTermFromNetworkCompletedCB unregisterTermFromNetworkCompletedF;
    RvMdmMediaDescriptor                    mediaCaps;
    RvMdmPackagesDescriptor                 pkgsCaps;
    RvMdmTermMatchDialStringCB              matchDialStringF;
    RvMdmTermMapDialStringToAddressCB       mapDialStringToAddressF;
    RvMdmTermModifyMediaCompletedCB         modifyMediaCompletedF;

    RvAlloc*                                a;
} RvMdmTermClass;

/* Signal callbacks registration */
RVAPI void RVCALLCONV rvMdmTermClassRegisterStartSignalCB(
    IN RvMdmTermClass*          c,
    IN RvMdmTermStartSignalCB   startSignalF);
RVAPI void RVCALLCONV rvMdmTermClassRegisterStopSignalCB(
    IN RvMdmTermClass*          c,
    IN RvMdmTermStopSignalCB    stopSignalF);
RVAPI void RVCALLCONV rvMdmTermClassRegisterPlaySignalCB(
    IN RvMdmTermClass*          c,
    IN RvMdmTermPlaySignalCB    playSignalF);

/* Register callback to call when creating or deleting the media stream */
RVAPI void RVCALLCONV rvMdmTermClassRegisterCreateMediaCB(
    IN RvMdmTermClass*          c,
    IN RvMdmTermCreateMediaCB   createMediaF);
RVAPI void RVCALLCONV rvMdmTermClassRegisterDestroyMediaCB(
    IN RvMdmTermClass*          c,
    IN RvMdmTermDestroyMediaCB  destroyMediaF);

/* Callback to use when modifying media stream properties */
RVAPI void RVCALLCONV rvMdmTermClassRegisterModifyMediaCB(
    IN RvMdmTermClass*          c,
    IN RvMdmTermModifyMediaCB   modifyMediaF);

/******************************************************************************
 * rvMdmTermClassRegisterRegisterPhysTermCompletedCB
 * ----------------------------------------------------------------------------
 * General:
 *  Set the callback to call when registering a physical termination is done,
 *  if called asynchronous.
 * Arguments:
 * Input:  c                            - A pointer to termination class object.
 *         registerPhysTermCompletedF   - The callback function.
 * Output: None.
 *
 * Return Value: None
 *****************************************************************************/
RVAPI void RVCALLCONV rvMdmTermClassRegisterRegisterPhysTermCompletedCB(
    IN RvMdmTermClass*                      c,
    IN RvMdmTermRegisterPhysTermCompletedCB registerPhysTermCompletedF);

/******************************************************************************
 * rvMdmTermClassRegisterUnregisterTermCompletedCB
 * ----------------------------------------------------------------------------
 * General:
 *  Set the callback to call when unregistering a physical termination is done,
 *  if called asynchronous.
 * Arguments:
 * Input:  c                            - A pointer to termination class object.
 *         unregisterPhysTermCompletedF - The callback function.
 * Output: None.
 *
 * Return Value: None
 *****************************************************************************/
RVAPI void RVCALLCONV rvMdmTermClassRegisterUnregisterTermCompletedCB(
    IN RvMdmTermClass*                      c,
    IN RvMdmTermUnregisterTermCompletedCB   unregisterTermCompletedF);


/******************************************************************************
 * rvMdmTermClassRegisterUnregisterTermFromNetworkCompletedCB
 * ----------------------------------------------------------------------------
 * General:
 *  Set the callback to call when unregistering a termination from network is
 *  done, if called asynchronous.
 * Arguments:
 * Input:  c                            - A pointer to termination class object.
 *         unregisterTermFromNetworkCompletedF - The callback function.
 * Output: None.
 *
 * Return Value: None
 *****************************************************************************/
RVAPI void RVCALLCONV rvMdmTermClassRegisterUnregisterTermFromNetworkCompletedCB(
    IN RvMdmTermClass*                                  c,
    IN RvMdmTermUnregisterTermFromNetworkCompletedCB    unregisterTermFromNetworkCompletedF);

RVAPI void rvMdmTermClassRegisterModifyMediaCompletedCB(RvMdmTermClass* c,RvMdmTermModifyMediaCompletedCB registerMediaModifiedF);

/* Capabilities */
RVAPI void RVCALLCONV rvMdmTermClassAddMediaCapabilities(
    IN RvMdmTermClass*              c,
    IN const RvSdpMsgList*          localDescr,
    IN const RvSdpMsgList*          remoteDescr,
    IN const RvMdmParameterList*    localProperties);

/******************************************************************************
 * rvMdmTermClassClearMediaCapabilities
 * ----------------------------------------------------------------------------
 * General:
 *  Clears all current media capabilities from a Termination class.
 *
 * Arguments:
 * Input:  c        - A pointer to the Termination class.
 * Output: None.
 *
 * Return Value: None.
 *****************************************************************************/
RVAPI void RVCALLCONV rvMdmTermClassClearMediaCapabilities(IN RvMdmTermClass* c);

RVAPI void RVCALLCONV rvMdmTermClassAddSupportedPkg(
    IN RvMdmTermClass*  c,
    IN const RvChar*    name,
    IN RvUint           version);

/* Disable event detection */
RVAPI void rvMdmTermClassRegisterMatchDialStringCB(
    IN RvMdmTermClass*              c,
    IN RvMdmTermMatchDialStringCB   matchDialStringF);
RVAPI void RVCALLCONV rvMdmTermClassRegisterMapDialStringToAddressCB(
    IN RvMdmTermClass*                      c,
    IN RvMdmTermMapDialStringToAddressCB    mapDialStringToAddressF);

/* Predefined classes - utility functions */
RVAPI void rvMdmTermClassSetIPPhoneUIPackages(RvMdmTermClass* c);
RVAPI void rvMdmTermClassSetAudioTransducerPackages(RvMdmTermClass* c);
RVAPI void rvMdmTermClassSetRtpTermPackages(RvMdmTermClass* c);
RVAPI void rvMdmTermClassSetAnalogLinePackages(RvMdmTermClass* c);


/*Private Functions:*/
/* Constructor */
void rvMdmTermClassConstructA_(RvMdmTermClass * c,RvAlloc * a);
void rvMdmTermClassDestruct_(RvMdmTermClass * c);

RvMdmMediaDescriptor* rvMdmTermClassGetMediaCapabilites_(RvMdmTermClass * c);


/*------------------------------------------------------------------------------*/
/*--------------------------    RvMdmTerm   ------------------------------------*/
/*------------------------------------------------------------------------------*/

/*$
{type:
    {name: RvMdmTerm}
    {include: rvmdm.h}
    {description: {p: Describes a  termination.}}
    {protected_methods:
        {method: RvBool rvMdmTermCreateMediaStream_(RvMdmTerm* term,RvMdmMediaStream* media,RvMdmMediaStreamDescr* streamDescr);}
        {method: RvBool rvMdmTermModifyMediaStream_(RvMdmTerm* term,RvMdmMediaStream* media,RvMdmMediaStreamDescr* streamDescr);}
        {method: RvBool rvMdmTermDestroyMediaStream_(RvMdmTerm* term,RvMdmMediaStream* mediaStream);}
        {method: void rvMdmTermConstruct_(RvMdmTerm* term,RvMdmTermClass* termClass,RvMdmXTerm* xTerm);}
        {method: void rvMdmTermDestruct_(RvMdmTerm* term);}
        {method: RvMdmXTerm * rvMdmTermGetXTerm_(RvMdmTerm* term);}
        {method: RvBool rvMdmTermStartSignal_(RvMdmTerm* term,RvMdmSignal * s);}
        {method: RvBool rvMdmTermStopSignal_(RvMdmTerm* term,RvMdmSignal * s);}
        {method: RvBool rvMdmTermPlaySignal_(RvMdmTerm* term,RvMdmSignal * s,RvBool reportCompletion);}
    }
}
$*/

typedef struct RvMdmTerm_ {
    RvMdmTermClass *             termClass;
    void *                       userData;
    RvMdmXTerm *                 xTerm;
} RvMdmTerm;


/*Public:*/
/* Events processing */
/*$
{function:
    {name: rvMdmTermProcessEvent}
    {class: RvMdmTerm}
    {include: rvmdm.h}
    {description:
        {p: Report an event detected in the termination.}
    }
    {proto: void rvMdmTermProcessEvent(RvMdmTerm* term,const char* pkg,const char* id,RvMdmMediaStream* media,RvMdmParameterList* args);}
    {params:
        {param: {n:term} {d:A pointer to termination object.}}
        {param: {n:pkg} {d:The package id.}}
        {param: {n:id} {d:The event id.}}
        {param: {n:media} {d:The user id of the media stream where the event was detected,
                           or 0 if the event is not related to any media.}}
        {param: {n:args} {d:The event parameters or NULL.}}
    }
}
$*/
RVAPI void RVCALLCONV rvMdmTermProcessEvent(
    IN RvMdmTerm*           term,
    IN const RvChar*        pkg,
    IN const RvChar*        id,
    IN RvMdmMediaStream*    media,
    IN RvMdmParameterList*  args);



/******************************************************************************
 * rvMdmTermSetPhoneNumber
 * ----------------------------------------------------------------------------
 * General:
 *  Add a phone number to a termination.
 *  Each termination can handle more than a single number.
 * Arguments:
 * Input:  term     - A pointer to termination object.
 *         number   - The phone number to add to list of supported numbers of
 *                    the termination.
 * Output: None.
 *
 * Return Value: RV_OK on success, other on failure.
 *****************************************************************************/
RVAPI RvStatus RVCALLCONV rvMdmTermSetPhoneNumber(
    IN RvMdmTerm*       term,
    IN const RvChar*    number);


/*$
{function:
    {name: rvMdmTermModifyMedia}
    {class: RvMdmTerm}
    {include: rvmdm.h}
    {description:
        {p: Request to modify parameters of existing media during a call by a sending reinvite request.}
    }
    {proto: void rvMdmTermModifyMedia(RvMdmTerm* term, RvSdpMsg* sdpMsg); }
    {params:
        {param: {n:term} {d:A pointer to termination object.}}
        {param: {n:sdpMsg} {d:A pointer to SDP message that includes new media parameters.}}
    }
}
$*/
RVAPI RvBool rvMdmTermModifyMedia(RvMdmTerm* term, RvSdpMsg* sdpMsg);

/*@*****************************************************************************
* rvMdmTermModifyMediaByUpdate 
* -----------------------------------------------------------------------------
* General:
*         Triggers sending an UPDATE request with the given SDP.
*
* Arguments:
*
* Input:  term             - Pointer to the MDM terminal object.
*         sdpMsg           - Pointer to the SDP to be attached to the UPDATE request.
*
* Return Value: Upon success RV_TRUE is returned.
 ****************************************************************************@*/
RVAPI RvBool rvMdmTermModifyMediaByUpdate( IN RvMdmTerm* term,
										   IN RvSdpMsg*  sdpMsg);

/*$
{function:
    {name: rvMdmTermGetUserData}
    {class: RvMdmTerm}
    {include: rvmdm.h}
    {description:
        {p: Returns the user data associated with the termination.}
    }
    {proto: void * rvMdmTermGetUserData(RvMdmTerm* term);}
    {params:
        {param: {n:term} {d:A pointer to termination object.}}
    }
    {returns:
        The termination user data or NULL if not set.
    }
}
$*/
RVAPI void* RVCALLCONV rvMdmTermGetUserData(IN RvMdmTerm* term);
/*$
{function:
    {name: rvMdmTermSetUserData}
    {class: RvMdmTerm}
    {include: rvmdm.h}
    {description:
        {p: Sets user data associated with the termination.}
    }
    {proto: void rvMdmTermSetUserData(RvMdmTerm* term,void* data);}
    {params:
        {param: {n:term} {d:A pointer to termination object.}}
        {param: {n:data} {d:The user data.}}
    }
    {notes:
        {note:
            Use this function to associate application data and memory with
            the termination. In this way the application may avoid the need to
            have his own database of terminations.
        }
    }
}
$*/
RVAPI void RVCALLCONV rvMdmTermSetUserData(
    IN RvMdmTerm*   term,
    IN void*        data);
/*$
{function:
    {name: rvMdmTermGetClass}
    {class: RvMdmTerm}
    {include: rvmdm.h}
    {description:
        {p: Gets the termination class}
    }
    {proto: RvMdmTermClass * rvMdmTermGetClass(RvMdmTerm* term);}
    {params:
        {param: {n:term} {d:A pointer to termination object.}}
    }
    {returns:
        The termination class or NULL if not set.
    }
    {notes:
        {note:

        }
    }
}
$*/
RvMdmTermClass* rvMdmTermGetClass(RvMdmTerm* term);

/*$
{function:
    {name: rvMdmTermGetType}
    {class: RvMdmTerm}
    {include: rvmdm.h}
    {description:
        {p: Gets the termination type.}
    }
    {proto: RvMdmTermType rvMdmTermGetType(RvMdmTerm* term);}
    {params:
        {param: {n:term} {d:A pointer to termination object.}}
    }
    {returns:
        The termination type (ephemeral,physical,undefined).
    }
}
$*/
RVAPI RvMdmTermType RVCALLCONV rvMdmTermGetType(IN RvMdmTerm* term);


/* Media related */
RvBool rvMdmTermCreateMediaStream_(RvMdmTerm* term,RvMdmMediaStream* media,RvMdmMediaStreamDescr* streamDescr,RvMdmError* mediaError);
RvBool rvMdmTermModifyMediaStream_(RvMdmTerm* term,RvMdmMediaStream* media,RvMdmMediaStreamDescr* streamDescr,RvMdmError* mediaError);
RvBool rvMdmTermDestroyMediaStream_(RvMdmTerm* term,RvMdmMediaStream* mediaStream,OUT RvMdmError* mdmError);
void rvMdmTermRegisterPhysTermDone_(RvMdmTerm* term);
void rvMdmTermUnregisterTermDone_(RvMdmTerm* term);
void rvMdmTermUnregisterTermFromNetworkDone_(RvMdmTerm* term);
void rvMdmTermModifyMediaDone_(RvMdmTerm*                   term,
                               RvBool                       status,
                               RvMdmMediaStream*            media,
                               RvMdmMediaStreamDescr*       streamDescr,
                               RvMdmTermReasonModifyMedia   reason);

/* Use for eph. termination inside callbacks */
RVAPI const RvChar* RVCALLCONV rvMdmTermGetId(IN RvMdmTerm* term);

/* Private: */
void rvMdmTermConstruct_(
    IN RvMdmTerm*       term,
    IN RvMdmTermClass*  termClass,
    IN RvMdmXTerm*      xTerm);
void rvMdmTermDestruct_(RvMdmTerm* term);

RvMdmXTerm * rvMdmTermGetXTerm_(RvMdmTerm* term);

RvBool rvMdmTermStartSignal_(RvMdmTerm* term,RvMdmSignal * s,OUT RvMdmError* mdmError);
RvBool rvMdmTermStopSignal_(RvMdmTerm* term,RvMdmSignal * s,OUT RvMdmError* mdmError);
RvBool rvMdmTermPlaySignal_(RvMdmTerm* term,RvMdmSignal * s,RvBool reportCompletion,OUT RvMdmError* mdmError);

RvBool rvMdmTermClassIsPkgSupported_(RvMdmTermClass* termClass,const char* pkg);
RvBool rvMdmTermIsPkgSupported_(RvMdmTerm* term,const char* pkg);
RvMdmPackagesDescriptor* rvMdmTermGetPackages_(RvMdmTerm* term);
RvMdmMediaDescriptor* rvMdmTermGetMediaCapabilites_(RvMdmTerm* term);

RvMdmDigitMapMatchType rvMdmTermMatchDialString_(struct RvMdmTerm_* term, const char *dialString, unsigned int *timerDuration);
RvBool rvMdmTermMapDialStringToAddress_(struct RvMdmTerm_* term, const char* dialString, char* address);


/*------------------------------------------------------------------------------*/
/*--------------------------    RvMdmTermMgrX   --------------------------------*/
/*------------------------------------------------------------------------------*/

/*$
{private_type:
    {name: RvMdmTermMgrX}
    {include: rvmdm.h}
    {description: {p:Interface to X callbacks for general mgr functions.}}
}
$*/

typedef RvBool (*RvMdmXTermMgrUnregisterTermCB)(IN RvMdmXTermMgr* x, IN RvMdmXTerm* xTerm);
typedef RvBool (*RvMdmXTermMgrUnregisterTermAsyncCB)(IN RvMdmXTermMgr* x, IN RvMdmXTerm* xTerm);
typedef RvMdmTerm* (*RvMdmXTermMgrFindTermCB)(IN RvMdmXTermMgr* x, IN const RvChar* id);

typedef RvMdmTerm* (*RvMdmXTermMgrRegisterPhysTermCB)(
    IN RvMdmXTermMgr*                       x,
    IN RvMdmTermClass*                      c,
    IN const RvChar*                        id,
    IN struct RvMdmTermDefaultProperties_*  termProperties);
typedef RvMdmTerm* (*RvMdmXTermMgrRegisterPhysTermAsyncCB)(
    IN RvMdmXTermMgr*                       x,
    IN RvMdmTermClass*                      c,
    IN const RvChar*                        id,
    IN struct RvMdmTermDefaultProperties_*  termProperties,
    IN void*                                userData);
typedef RvMdmTerm* (*RvMdmXTermMgrRegisterEphTermCB)(
    IN RvMdmXTermMgr*                       x,
    IN RvMdmTermClass*                      c,
    IN const RvChar*                        id,
    IN struct RvMdmTermDefaultProperties_*  termProperties);

typedef void (*RvMdmXTermMgrStartCB)(IN RvMdmXTermMgr* x);
typedef void (*RvMdmXTermMgrStopCB)(IN RvMdmXTermMgr* x);

typedef void (*RvMdmXTermMgrDestructCB)(struct RvMdmTermMgr_ * mgr);

/*------------------------------------------------------------------------------*/
/*--------------------  RvMdmTermMgrX   C A L L B A C K S  ---------------------*/
/*------------------------------------------------------------------------------*/

/*$
{callback:
    {name: RvMdmProcessEachTermCB}
    {include: rvmdm.h}
    {description:
        {p: Functions that follows this template are passed as an argument
            to rvMdmTermMgrForEachPhysicalTerm() or rvMdmContextForEachTerm().
            These functions are used to process a termination or execute some
            application task.}
    }
    {proto: RvBool (*RvMdmProcessEachTermCB)(RvMdmTerm* term,void* data);}
    {params:
        {param: {n:term} {d:The termination.}}
        {param: {n:data} {d:The argument to rvMdmTermMgrForEachPhysicalTerm() or
                            rvMdmContextForEachTerm(), use to pass any user data.}}
    }
    {returns:
        rvTrue if no further processing of terminations is required. rvFalse
        to keep processing terminations in the loop.
    }
}
$*/
typedef RvBool (*RvMdmProcessEachTermCB)(
    IN RvMdmTerm*       term,
    IN void*            data);

typedef RvBool (*RvMdmXTermMgrForEachPhysTermCB)(RvMdmXTermMgr* mgr,RvMdmProcessEachTermCB func,void* data);

typedef void (*RvMdmXTermMgrMediaCapsUpdatedCB)(RvMdmXTermMgr* mgr,RvMdmTermClass * c);

typedef RvBool (*RvMdmXTermMgrRegisterAllTermsToNetworkCB)(RvMdmXTermMgr* mgr);

typedef RvBool (*RvMdmXTermMgrRegisterTermToNetworkCB)(RvMdmXTermMgr* mgr, RvMdmTerm* term);

typedef RvBool (*RvMdmXTermMgrUnregisterTermFromNetworkCB)(RvMdmXTermMgr* mgr, RvMdmTerm* term);

typedef struct
{
    RvMdmXTermMgrRegisterPhysTermCB             registerPhysTermF;
    RvMdmXTermMgrRegisterPhysTermAsyncCB        registerPhysTermAsyncF;
    RvMdmXTermMgrRegisterEphTermCB              registerEphTermF;
    RvMdmXTermMgrUnregisterTermCB               unregisterTermF;
    RvMdmXTermMgrUnregisterTermAsyncCB          unregisterTermAsyncF;
    RvMdmXTermMgrFindTermCB                     findTermF;
    RvMdmXTermMgrFindTermCB                     getIdleTermF;
    RvMdmXTermMgrStartCB                        startF;
    RvMdmXTermMgrStopCB                         stopF;
    RvMdmXTermMgrDestructCB                     destructF;
    RvMdmXTermMgrForEachPhysTermCB              forEachPhysTermF;
    RvMdmXTermMgrMediaCapsUpdatedCB             mediaCapsUpdatedF;
    RvMdmXTermMgrRegisterAllTermsToNetworkCB    registerAllTermsToNetworkF;
    RvMdmXTermMgrRegisterTermToNetworkCB        registerTermToNetworkF;
    RvMdmXTermMgrUnregisterTermFromNetworkCB    unregisterTermFromNetworkF;
} RvMdmXTermMgrClbks;

/*------------------------------------------------------------------------------*/
/*---------------------------   RvMdmTermMgr  ----------------------------------*/
/*------------------------------------------------------------------------------*/

/*$
{type:
    {name: RvMdmTermMgr}
    {include: rvmdm.h}
    {description:
        {p:
            Manages a set of terminations and the signals and events
            that can be applied to them, for a given stack instance.
        }
    }
    {protected_methods:
        {method: void rvMdmTermMgrConstruct_(RvMdmTermMgr* mgr,RvAlloc * a);}
        {method: void rvMdmTermMgrRegisterXTermMgrClbks_(RvMdmTermMgr* mgr,RvMdmXTermMgrClbks * t);}
        {method: const RvMdmSignalInfo* rvMdmTermMgrGetSignalInfo_(RvMdmTermMgr* x,const RvString* pkgStr,const RvString* sigStr);}
        {method: RvBool rvMdmTermMgrConnectMediaStreams_(RvMdmTermMgr* mgr,RvMdmTerm* source,RvMdmMediaStream* m1,RvMdmTerm* target,
          RvMdmMediaStream* m2,RvMdmStreamDirection direction);}
        {method: RvBool rvMdmTermMgrDisconnectMediaStreams_(RvMdmTermMgr* mgr,RvMdmTerm* source,RvMdmMediaStream* m1,
          RvMdmTerm* target,RvMdmMediaStream* m2);}
    }
}
$*/

/* RvMdmTermMgr
 * ----------------------------------------------------------------------------
 * The MDM Termination Manager is the main object of the Multimedia Terminal
 * Framework. It holds the core logic of an MTF instance.
 *
 * The parameters of this struct should not be accessed directly by the
 * application - they should be accessed through the RvMdmTermMgr functions.
 */
typedef struct RvMdmTermMgr_
{
    RvMdmTermMgrSelectTerminationCB     selectF;
    RvMdmTermMgrDeleteEphTermCB         deleteEphF;

    RvMdmTermMgrConnectCB               connectF;
    RvMdmTermMgrDisconnectCB            disconnectF;

    RvMdmXTermMgrClbks*                 xTermMgrClbks;
    RvMdmXTermMgr*                      xTermMgr;
    /* I need another pointer for the app (phone)
       to avoid duplicating all mdmTermMgr callbacks */
    void*                               xApp;

    RvAlloc*                            a;
    RvPtrList                           termClassList;
    /* Packages */
    RvMdmPackageData                    packageData;
    /* Packages supporting digitmap */
    RvPtrList                           digitMapData;

    /* Max number of contexts and terms */
    RvUint32                            maxContexts;
    RvUint32                            maxTermPerContext;

    RvMdmTermMgrMapAddressToTermCB      mapAddressToTermF;

    void*                               userData;
} RvMdmTermMgr;


/* Constructor and destructor */
void rvMdmTermMgrConstruct_(RvMdmTermMgr* mgr,RvAlloc * a);
RVAPI void RVCALLCONV rvMdmTermMgrDestruct(IN RvMdmTermMgr* mgr);

/* Call after registering endpoints */
RVAPI void RVCALLCONV rvMdmTermMgrStart(
    IN RvMdmTermMgr*        mgr,
    IN RvMdmServiceChange*  sc,
    IN RvInt32              delay);

RVAPI void RVCALLCONV rvMdmTermMgrStop(
    IN RvMdmTermMgr*        mgr,
    IN RvMdmServiceChange*  sc);

/* Class construction */
RVAPI RvMdmTermClass* RVCALLCONV rvMdmTermMgrCreateTermClass(IN RvMdmTermMgr* mgr);

/* Termination registration */
RVAPI RvMdmTerm* RVCALLCONV rvMdmTermMgrRegisterPhysicalTermination(
    IN RvMdmTermMgr*                        mgr,
    IN RvMdmTermClass*                      c,
    IN const RvChar*                        id,
    IN struct RvMdmTermDefaultProperties_*  termProperties,
    IN RvMdmServiceChange*                  sc);


/******************************************************************************
 * rvMdmTermMgrRegisterPhysicalTerminationAsync
 * ----------------------------------------------------------------------------
 * General:
 *  Register a physical termination in an asynchronous manner.
 * Arguments:
 * Input:  mgr              - A pointer to the Termination Manager object.
 *         c                - A pointer to a previously created and initialized
 *                            Termination class.
 *         id               - The Termination id.
 *         termProperties   - Default properties of the Termination or NULL.
 *         userData         - The user data associated with the new termination.
 * Output: None.
 *
 * Notes:
 *  - If the function is called after calling rvMdmTermMgrStart(), it will
 *    signal that the new termination is coming up.
 *  - If sc is NULL then default values will be used.
 *
 * Return Value: Returns a pointer to the new Termination, or NULL if it fails.
 *****************************************************************************/
RVAPI RvMdmTerm* RVCALLCONV rvMdmTermMgrRegisterPhysicalTerminationAsync(
    IN RvMdmTermMgr*                        mgr,
    IN RvMdmTermClass*                      c,
    IN const RvChar*                        id,
    IN struct RvMdmTermDefaultProperties_*  termProperties,
    IN void*                                userData);

RVAPI RvMdmTerm* RVCALLCONV rvMdmTermMgrRegisterEphemeralTermination(
    IN RvMdmTermMgr*                        mgr,
    IN RvMdmTermClass*                      c,
    IN const RvChar*                        id,
    IN struct RvMdmTermDefaultProperties_*  termProperties);

RVAPI RvBool RVCALLCONV rvMdmTermMgrUnregisterTermination(
    IN RvMdmTermMgr*        mgr,
    IN RvMdmTerm*           term,
    IN RvMdmServiceChange*  sc);

/******************************************************************************
 * rvMdmTermMgrUnregisterTerminationAsync
 * ----------------------------------------------------------------------------
 * General:
 *  Register a physical termination in an asynchronous manner.
 * Arguments:
 * Input:  mgr      - A pointer to the Termination Manager object.
 *         mdmTerm  - A pointer to the termination.
 *         sc       - The service change parameters or NULL.
 * Output: None.
 *
 * Notes:
 *  - If the function is called after calling rvMdmTermMgrStart(), it will
 *    signal that the termination is going down.
 *  - If sc is NULL then default values will be used.
 *
 * Return Value: Returns a pointer to the new Termination, or NULL if it fails.
 *****************************************************************************/
RVAPI RvBool RVCALLCONV rvMdmTermMgrUnregisterTerminationAsync(
    IN RvMdmTermMgr*        mgr,
    IN RvMdmTerm*           mdmTerm,
    IN RvMdmServiceChange*  sc);

/* Connect two medias */
RVAPI void RVCALLCONV rvMdmTermMgrRegisterConnectCB(
    IN RvMdmTermMgr*            mgr,
    IN RvMdmTermMgrConnectCB    connectF);
RVAPI void RVCALLCONV rvMdmTermMgrRegisterDisconnectCB(
    IN RvMdmTermMgr*                mgr,
    IN RvMdmTermMgrDisconnectCB     disconnectF);

RVAPI RvMdmTerm* RVCALLCONV rvMdmTermMgrFindTermination(
    IN RvMdmTermMgr*    mgr,
    IN const RvChar*    id);

RVAPI RvMdmTerm* RVCALLCONV rvMdmTermMgrGetIdleTermination(
    IN RvMdmTermMgr*    mgr,
    IN const RvChar*    id);

/* Description: Function type for selecting an Termination to implement "any of" wildcard */
RVAPI void RVCALLCONV rvMdmTermMgrRegisterSelectTermCB(
    IN RvMdmTermMgr*                    mgr,
    IN RvMdmTermMgrSelectTerminationCB  selectF);
RVAPI void RVCALLCONV rvMdmTermMgrRegisterDeleteEphTermCB(
    IN RvMdmTermMgr*                mgr,
    IN RvMdmTermMgrDeleteEphTermCB  deleteEphF);

RVAPI RvMdmPackage* RVCALLCONV rvMdmTermMgrGetPackage(
    IN RvMdmTermMgr*    mgr,
    IN const RvChar*    name);

RVAPI void rvMdmTermMgrRegisterMapAddressToTermCB(RvMdmTermMgr* mgr, RvMdmTermMgrMapAddressToTermCB mapAddressToTermF);

RVAPI RvBool RVCALLCONV rvMdmTermMgrForEachPhysicalTerm(
    IN RvMdmTermMgr*            mgr,
    IN RvMdmProcessEachTermCB   func,
    IN void*                    data);

RVAPI void RVCALLCONV rvMdmTermMgrMediaCapabilitiesUpdated(
    IN RvMdmTermMgr*    mgr,
    IN RvMdmTermClass*  rtpClass);

void rvMdmTermMgrRegisterXTermMgrClbks_(RvMdmTermMgr* mgr,RvMdmXTermMgrClbks * t);
RvMdmTerm* rvMdmTermMgrSelectTermination_(RvMdmTermMgr* mgr,RvMdmTerm* tempTerm);

/* Get digitmap package info or NULL */
struct RvMdmDigitMapData_* rvMdmTermMgrGetDigitMapPkgInfo_(RvMdmTermMgr * mgr,const char * pkg);
void   rvMdmTermMgrSetDigitMapPkgInfo_(RvMdmTermMgr * mgr,struct RvMdmDigitMapData_ * dmData);

RvMdmPackage* rvMdmTermMgrGetPkgInfo_(RvMdmTermMgr *termMgr,const RvString *pkgStr);

const RvMdmSignalInfo* rvMdmTermMgrGetSignalInfo_(RvMdmTermMgr* x,const RvString* pkgStr,const RvString* sigStr);

void rvMdmTermMgrDeleteEphTerm_(RvMdmTermMgr* mgr,RvMdmTerm* ephTerm);

RvBool rvMdmTermMgrConnectMediaStreams_(RvMdmTermMgr* mgr,RvMdmTerm* source,RvMdmMediaStream* m1,RvMdmTerm* target,
                                        RvMdmMediaStream* m2,RvMdmStreamDirection direction,RvMdmError * error);
RvBool rvMdmTermMgrDisconnectMediaStreams_(RvMdmTermMgr* mgr,RvMdmTerm* source,RvMdmMediaStream* m1,RvMdmTerm* target,
                                            RvMdmMediaStream* m2,RvMdmError * error);


char* rvMdmTermMgrMapAddressToTermination_(RvMdmTermMgr* mgr, const char* address);

RVAPI void* RVCALLCONV rvMdmTermMgrGetUserData(IN RvMdmTermMgr* mgr);
RVAPI void RVCALLCONV rvMdmTermMgrSetUserData(IN RvMdmTermMgr* mgr, IN void* data);

RVAPI void rvMdmTermMgrRegisterIPPhoneUiPackages(RvMdmTermMgr* mgr);
RVAPI void rvMdmTermMgrRegisterUserSignal(RvMdmTermMgr* mgr, const char* pkg, const char* signal);
RVAPI void rvMdmTermMgrSetUserPackage(RvMdmTermClass* c, const char* pkg);

/*Sends a registration request to Network*/
RVAPI RvBool RVCALLCONV rvMdmTermMgrRegisterAllTermsToNetwork(IN RvMdmTermMgr* mgr);
#define rvMdmTermMgrRegisterAllTermsToNetwork_(_mgr) rvMdmTermMgrRegisterAllTermsToNetwork(_mgr)
RVAPI RvBool RVCALLCONV rvMdmTermMgrRegisterTermToNetwork(
    IN RvMdmTermMgr*    mgr,
    IN RvMdmTerm*       term);
#define rvMdmTermMgrRegisterTermToNetwork_(_mgr, _term) rvMdmTermMgrRegisterTermToNetwork(_mgr, _term)

/********************************************************************************************
 * rvMdmTermMgrUnregisterTermFromNetwork
 * purpose : This API manually sends an unregistration request from Network.
 *
 * input   : mgr        - mdm term manager
 *           term       - the termination to unregister from network.
 *
 * Returns: RvFalse: failed to unregister from network, RvTrue: otherwise
 * output  : None
 * note    :
 ********************************************************************************************/
RVAPI RvBool RVCALLCONV rvMdmTermMgrUnregisterTermFromNetwork(
    IN RvMdmTermMgr*    mgr,
    IN RvMdmTerm*       term);
#define rvMdmTermMgrUnregisterTermFromNetwork_(_mgr, _term) rvMdmTermMgrUnregisterTermFromNetwork(_mgr, _term)


unsigned char rvMdmKpDigitMapTranslateKeypadEv(const char * eventName, const RvMdmParameterList* args);
void  rvMdmKpDigitMapBuildEvComplete(RvMdmParameterList *parameters,
                                        const char * digitString,
                                        RvMdmDigitMapMatchType matchType,
                                        void* userData);


/*------------------------------------------------------------------------------*/
/*-------------------   RvMdmTermDefaultProperties  ----------------------------*/
/*------------------------------------------------------------------------------*/

/*$
{type:
    {name: RvMdmTermDefaultProperties}
    {include: rvmdm.h}
    {description: {p: Describes the termination default properties.}}
}
$*/
typedef struct RvMdmTermDefaultProperties_
{
    RvMdmDigitMapDescriptor         digitMap;
    RvChar                          username[RV_NAME_STR_SZ];
    RvChar                          password[RV_NAME_STR_SZ];
    RvMdmTermPhoneNumbers           phoneNumbers;
    RvMdmTermPresentationInfo       presentationInfo;
	char							registrarAddress[RV_MDM_ADDRESS_SIZE];
	RvUint16						registrarPort;
	char							outboundProxyAddress[RV_MDM_ADDRESS_SIZE];
	RvUint16						outboundProxyPort;
	RvUint16						registerExpires;
	RvInt			                transportType;  /* The values are as in RvSipTransport */
} RvMdmTermDefaultProperties;

void rvMdmTermDefaultPropertiesConstructA(RvMdmTermDefaultProperties* x,RvAlloc* alloc);

RVAPI void RVCALLCONV rvMdmTermDefaultPropertiesConstruct(
    IN RvMdmTermDefaultProperties*  x);
RVAPI void RVCALLCONV rvMdmTermDefaultPropertiesDestruct(
    IN RvMdmTermDefaultProperties*  x);

RVAPI void RVCALLCONV rvMdmTermDefaultPropertiesSetDigitMap(
    IN RvMdmTermDefaultProperties*  x,
    IN RvMdmDigitMap*               digitMap,
    IN const RvChar*                name);


/******************************************************************************
 * rvMdmTermDefaultPropertiesSetUsername
 * ----------------------------------------------------------------------------
 * General:
 *  Set the username for server authentication when required.
 * Arguments:
 * Input:  x        - The default properties object.
 *         username - The username to set.
 * Output: None.
 *
 * Return Value: None.
 *****************************************************************************/
RVAPI void RVCALLCONV rvMdmTermDefaultPropertiesSetUsername(
    IN RvMdmTermDefaultProperties*  x,
    IN const RvChar*                username);

/******************************************************************************
 * rvMdmTermDefaultPropertiesSetPassword
 * ----------------------------------------------------------------------------
 * General:
 *  Set the password for server authentication when required.
 * Arguments:
 * Input:  x        - The default properties object.
 *         username - The password to set.
 * Output: None.
 *
 * Return Value: None.
 *****************************************************************************/
RVAPI void RVCALLCONV rvMdmTermDefaultPropertiesSetPassword(
    IN RvMdmTermDefaultProperties*  x,
    IN const RvChar*                password);

/******************************************************************************
 * rvMdmTermPropertiesSetPresentationInfo
 * ----------------------------------------------------------------------------
 * General:
 *  Set the configuration of the presentation name and permissions to present
 *  it in any outgoing messages.
 * Arguments:
 * Input:  x                - The default properties object.
 *         presentationInfo - The presentation configuration to use.
 * Output: None.
 *
 * Return Value: RV_TRUE on success, RV_FALSE on failure.
 *****************************************************************************/
RVAPI RvBool RVCALLCONV rvMdmTermPropertiesSetPresentationInfo(
    IN RvMdmTermDefaultProperties*  x,
    IN RvMdmTermPresentationInfo*   presentationInfo);


/******************************************************************************
 * rvMdmTermPropertiesSetPhoneNumbers
 * ----------------------------------------------------------------------------
 * General:
 *  Set the list of phone numbers in a termination's properties.
 * Arguments:
 * Input:  x                - The default properties object.
 *         phoneNumbers     - Phone numbers to set.
 * Output: None.
 *
 * Return Value: None.
 *****************************************************************************/
RVAPI void RVCALLCONV rvMdmTermPropertiesSetPhoneNumbers(
    IN RvMdmTermDefaultProperties*  x,
    IN RvMdmTermPhoneNumbers*       phoneNumbers);

RVAPI void RVCALLCONV rvMdmTermDefaultPropertiesSetRegistrarAddress(
																	IN RvMdmTermDefaultProperties*  x,
														         	IN const RvChar*                registrarAddress);

RVAPI void RVCALLCONV rvMdmTermDefaultPropertiesSetRegistrarPort(
															 	 IN RvMdmTermDefaultProperties*  x,
																 IN RvUint16                     registrarPort);

RVAPI void RVCALLCONV rvMdmTermDefaultPropertiesSetOutboundProxyAddress(
																		IN RvMdmTermDefaultProperties*  x,
																     	IN const RvChar*                outboundProxyAddress);

RVAPI void RVCALLCONV rvMdmTermDefaultPropertiesSetOutboundProxyPort(
																	 IN RvMdmTermDefaultProperties*  x,
																     IN RvUint16                     outboundProxyPort);

RVAPI void RVCALLCONV rvMdmTermDefaultPropertiesSetRegisterExpires(
																	IN RvMdmTermDefaultProperties*  x,
																    IN RvUint16                     registerExpires);

RVAPI void RVCALLCONV rvMdmTermDefaultPropertiesSetTransportType(
																	IN RvMdmTermDefaultProperties*  x,
																	IN RvInt						transportTcp);

#ifdef __cplusplus
}
#endif

#endif /* RV_MDM_H */
