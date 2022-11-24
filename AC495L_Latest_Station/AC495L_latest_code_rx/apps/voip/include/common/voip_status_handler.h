/********************************************************************
 * AudioCodes improved application status handler                   *
 *                                                                  *
 * File Name:	voip_status_handler.h                               *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 18/03/08 - Coding started.                                       *
 *																	*
 ********************************************************************/
#ifndef _VOIP_STATUS_HANDLER_H_
#define _VOIP_STATUS_HANDLER_H_

#include "acl_call_mngr.h"

#include "cip_def.h"
#include "cfg_api.h"
#include "cip_api.h"

void initVoipStatusHandler(void);

void destroyVoipStatusHandler(void);

void getGatewayLineStateInfo(int line, line_state_t *pGateway_lines_state);

void SetLineStatus(int line, PORT_STATUS status);




int getSpecificPortStatus(int line);

int setSpecificPortStatus(int line, PORT_STATUS status);

int getNumberOfPortExists();


#endif /* _VOIP_STATUS_HANDLER_H_ */

