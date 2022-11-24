/******************************************************************************
Filename:    rvMtfHandles.h
Description: This files include handles for MTF to be used by application.
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

#ifndef RV_MTF_HANDLES_H
#define RV_MTF_HANDLES_H

#include "rvtypes.h"

/*@*****************************************************************************
 * Type: RvMtfHandle (RvMtfBasePkg)
 * -----------------------------------------------------------------------------
 * Description: The handle to the MTF instance. This handle is retrieved during
 * MTF initialization.
 ****************************************************************************@*/
RV_DECLARE_HANDLE(RvMtfHandle);

/*@*****************************************************************************
 * Type: RvMtfAppHandle (RvMtfBasePkg)
 * -----------------------------------------------------------------------------
 * Description: The application handle associated with the MTF instance.
 * This handle can be set by calling rvMtfSetApplicationHandle() and retrieved
 * by calling rvMtfGetApplicationHandle().
 ****************************************************************************@*/
RV_DECLARE_HANDLE(RvMtfAppHandle);

/*@*****************************************************************************
 * Type: RvIppTerminalHandle (RvMtfBasePkg)
 * -----------------------------------------------------------------------------
 * Description: A termination handle in the MTF. This handle indicates a
 * specific terminal in the MTF. The terminal may represent an analog line
 * for IAD applications, a client terminal for IP Phones, or a video device
 * for Videophones.
 * When the MTF is used as an IAD, there may be multiple terminations/terminals,
 * each able to hold more than one connection. When the MTF is used as a
 * client/terminal, there may be a single terminal/termination that may hold
 * more than one connection. Connections on such a termination are associated
 * to RvIppConnectionHandle.
 ****************************************************************************@*/
RV_DECLARE_HANDLE(RvIppTerminalHandle);

/*@*****************************************************************************
 * Type: RvIppConnectionHandle (RvMtfBasePkg)
 * -----------------------------------------------------------------------------
 * Description: A connection handle in the MTF. This handle indicates a
 * specific temporary call within the MTF on a specific termination.
 * Connections belong to specific terminations through RvIppTerminalHandle.
 ****************************************************************************@*/
RV_DECLARE_HANDLE(RvIppConnectionHandle);

/*@*****************************************************************************
 * Type: RvMtfConnAppHandle (RvMtfBasePkg)
 * -----------------------------------------------------------------------------
 * Description: An application handle associated with RvMtfConnectionHandle.
 ****************************************************************************@*/
RV_DECLARE_HANDLE(RvMtfConnAppHandle);

/*@*****************************************************************************
 * Type: RvMtfTerminalAppHandle (RvMtfBasePkg)
 * -----------------------------------------------------------------------------
 * Description: An application handle associated with RvMtfTerminalHandle.
 ****************************************************************************@*/
RV_DECLARE_HANDLE(RvMtfTerminalAppHandle);



#endif /*RV_MTF_HANDLES_H*/
