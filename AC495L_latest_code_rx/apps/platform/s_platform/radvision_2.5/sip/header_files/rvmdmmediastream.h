/******************************************************************************
Filename   : rvmdmmediastream.h
Description: Structures and functions to save digitmap information
******************************************************************************
                Copyright (c) 2001 RADVision Inc.
************************************************************************
NOTICE:
This document contains information that is proprietary to RADVision LTD.
No part of this publication may be reproduced in any form whatsoever 
without written prior approval by RADVision LTD..

RADVision LTD. reserves the right to revise this publication and make 
changes without obligation to notify any person of such revisions or 
changes.
******************************************************************************/
#ifndef RV_MDMMEDIASTREAM_H
#define RV_MDMMEDIASTREAM_H

#include "rvmdmobjects.h"
#include "rvsdp.h"

struct RvCCTerminalMdm_;

#define RV_MDM_STREAMID_NONE		0

typedef enum
{
    RV_MDMSTREAM_NEW,
    RV_MDMSTREAM_CONNECTED
} RvMdmStreamStatus;

typedef struct
{
	void*                  userData;
	RvMdmStreamDescriptor  streamDescr;
	RvMdmStreamStatus       status;
	RvAlloc * alloc;
} RvMdmMediaStreamInfo;

/*MOVE TO rvterminalmdm.h*/
rvDeclareVector(RvMdmMediaStreamInfo)


/*MOVE TO rvterminalmdm.c*/

#define RvMdmMediaStreamInfoConstructCopy		rvMdmMediaStreamInfoConstructCopy
#define RvMdmMediaStreamInfoDestruct			rvMdmMediaStreamInfoDestruct
#define RvMdmMediaStreamInfoEqual				rvMdmMediaStreamInfoEqual




void rvMdmMediaStreamInfoConstructA(RvMdmMediaStreamInfo * x/*, const RvMdmStreamDescriptor * descr*/, RvMdmStreamId streamId,RvAlloc * alloc);
void rvMdmMediaStreamInfoDestruct(RvMdmMediaStreamInfo * x) ;

RvBool rvMdmMediaStreamInfoMatchId(const RvMdmMediaStreamInfo * x,RvUint32 streamId);
RVAPI RvUint32 rvMdmMediaStreamInfoGetId(const RvMdmMediaStreamInfo * x);
void rvMdmMediaStreamInfoSetId(RvMdmMediaStreamInfo * x, RvUint32 id);

#define rvMdmMediaStreamInfoGetStreamDescriptor(x) (&(x)->streamDescr)

#define rvMdmMediaStreamInfoIsStatusNew(x) ((x)->status==RV_MDMSTREAM_NEW)
#define rvMdmMediaStreamInfoSetStatusConnected(x) ((x)->status=RV_MDMSTREAM_CONNECTED)
#define rvMdmMediaStreamInfoSetStatusNew(x) ((x)->status=RV_MDMSTREAM_NEW)

/* Call callbacks and build response */
RvBool rvMdmMediaStreamInfoUpdateMdmMedia(RvMdmMediaStreamInfo * x,struct RvCCTerminalMdm_ * term, RvMdmMediaDescriptor * rspMedia, RvSdpMsg* sdpMsgCurrent);
RvBool rvMdmMediaStreamInfoCreateMdmMedia(RvMdmMediaStreamInfo * x,struct RvCCTerminalMdm_ * term, RvMdmMediaDescriptor * rspMedia);
void rvMdmMediaStreamInfoReleaseMdmMedia(RvMdmMediaStreamInfo * x,struct RvCCTerminalMdm_ * term);


RvBool rvMdmMediaStreamInfoModifyMediaDone(RvMdmMediaStreamInfo * x, 
                                          struct RvCCTerminalMdm_* term, 
										  RvBool status,
                                          RvMdmMediaDescriptor * rspMedia,
										  RvMdmTermReasonModifyMedia reason);

/* Required for the vector */
RvMdmMediaStreamInfo* rvMdmMediaStreamInfoConstructCopy(RvMdmMediaStreamInfo* d,const RvMdmMediaStreamInfo * s,RvAlloc* alloc);
RvBool rvMdmMediaStreamInfoEqual(const RvMdmMediaStreamInfo* d,const RvMdmMediaStreamInfo * s);

RVAPI RvBool rvMdmMediaStreamInfoIsLocalDescriptorSet(RvMdmMediaStreamInfo* x);
RVAPI RvBool rvMdmMediaStreamInfoIsRemoteDescriptorSet(RvMdmMediaStreamInfo* x);
void rvMdmMediaStreamInfoAddLocalDescriptor(RvMdmMediaStreamInfo* x, RvSdpMsg *msg);
void rvMdmMediaStreamInfoAddRemoteDescriptor(RvMdmMediaStreamInfo* x, RvSdpMsg *msg);
void rvMdmMediaStreamInfoClearLocal(RvMdmMediaStreamInfo* x);
void rvMdmMediaStreamInfoClearRemote(RvMdmMediaStreamInfo* x);
const RvSdpMsg* rvMdmMediaStreamInfoGetLocalDescriptor(RvMdmMediaStreamInfo* x, RvMdmStreamId streamId);
const RvSdpMsg* rvMdmMediaStreamInfoGetRemoteDescriptor(RvMdmMediaStreamInfo* x, RvMdmStreamId streamId);

RvBool RvMdmMediaStreamInfoDoesMediaTypeExist(RvMdmMediaStreamInfo* x, 
											  RvSdpMediaType		mediaType);

#endif /* RV_MDMMEDIASTREAM_H */

