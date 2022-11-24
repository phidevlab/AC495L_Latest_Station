/***********************************************************************
        Copyright (c) 2003 RADVISION Ltd.
************************************************************************
NOTICE:
This document contains information that is confidential and proprietary
to RADVISION Ltd.. No part of this document may be reproduced in any
form whatsoever without written prior approval by RADVISION Ltd..

RADVISION Ltd. reserve the right to revise this publication and make
changes without obligation to notify any person of such revisions or
changes.
***********************************************************************/

#ifndef _RV_IPPEVEXCHANGE_H
#define _RV_IPPEVEXCHANGE_H



#ifdef __cplusplus
extern "C" {
#endif
	

RvStatus createMsgSockets(void);

/* data==NULL indicates that the message sockets should be terminated */
RvStatus sendEventToMsgSocket(IN void* data);

	

	
#ifdef __cplusplus
}
#endif

#endif  /* _RV_IPPEVEXCHANGE_H */


