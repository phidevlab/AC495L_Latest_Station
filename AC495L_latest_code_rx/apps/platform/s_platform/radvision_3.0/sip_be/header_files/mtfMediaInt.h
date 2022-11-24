/******************************************************************************
Filename:    mtfMediaInt.h
Description: This file includes internal functions for calling deprecated media
			 callbacks.
*******************************************************************************
                Copyright (c) 2008 RADVISION
*******************************************************************************
NOTICE:
This document contains information that is proprietary to RADVISION.
No part of this publication may be reproduced in any form whatsoever
without written prior approval by RADVISION.

RADVISION reserves the right to revise this publication and make changes
without obligation to notify any person of such revisions or changes.
******************************************************************************/
#ifndef RV_RVMGRMGRMEDIA_INT_H
#define RV_RVMGRMGRMEDIA_INT_H

#include "rvmdm.h"

void rvModifyMediaCompletedIntCB(
              struct RvMdmTerm_*		    term,
              RvBool					    status,
              RvMdmMediaDescriptor*	        media,
              RvMdmMediaStreamDescr*	    streamDescr,
              RvMdmTermReasonModifyMedia	reason);

RvBool rvRtpConnectIntCB(
						RvMdmTermMgr *mgr,
						RvMdmTerm *source,
						RvMdmMediaStream *m1,
						RvMdmTerm *target,
						RvMdmMediaStream *m2,
	                    RvMdmStreamDirection direction,
                        RvMdmError *mdmError);

RvBool rvRtpDisconnectIntCB(   RvMdmTermMgr *mgr,
	                        RvMdmTerm *source,
                            RvMdmMediaStream *m1,
                            RvMdmTerm *target,
                            RvMdmMediaStream *m2,
	                        RvMdmError *mdmError);


RvBool rvRtpCreateMediaIntCB(
             struct RvMdmTerm_*         term,
             RvMdmMediaStream*          media,
             RvMdmMediaStreamDescr*     streamDescr,
             OUT RvMdmError*            mdmError);

RvBool rvRtpModifyMediaIntCB(
                 struct RvMdmTerm_* term,
                 RvMdmMediaStream* media,
                 RvMdmMediaStreamDescr* streamDescr,
                 RvMdmError* mdmError);

RvBool rvRtpDestroyMediaIntCB(
         RvMdmTerm*         term,
         RvMdmMediaStream*  media,
         OUT RvMdmError*    mdmError);

RvBool rvPhysCreateMediaIntCB(
          struct RvMdmTerm_* term,
          RvMdmMediaStream* media,
          RvMdmMediaStreamDescr* streamDescr,
          OUT RvMdmError* mdmError);

RvBool rvPhysDestroyMediaIntCB(
           RvMdmTerm*         term,
           RvMdmMediaStream*  media,
           OUT RvMdmError*    mdmError);




#endif /*RV_RVMGRMGRMEDIA_INT_H*/

