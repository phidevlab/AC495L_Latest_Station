/********************************************************************
 * AudioCodes improved application                   	            *
 *																	*
 * File Name:	acl_log.h        									*
 *																	*
 * Copyright (C) 2006 AudioCodes Ltd.								*
 *																	*
 * Modifications :													*
 *																	*
 * 30/10/06 - Coding started.										*
 *																	*
 ********************************************************************/
/******************************************************************************
*                                                                            
* 	DESCRIPTION:
*		
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <string.h>


#include <sys/types.h>


#ifndef	__ACL_LOG_h
#define	__ACL_LOG_h


#define ACL_LOG_ERROR	0x000000001
#define ACL_LOG_DEBUG	0x000000002
#define ACL_LOG_INFO	0x000000004
#define ACL_LOG_EVENT_INFO	0x000000008


#define acl_logMask	ACL_LOG_ERROR | ACL_LOG_DEBUG | ACL_LOG_INFO | ACL_LOG_EVENT_INFO

void acl_log(int flag, char *fmt, ...);

void acl_log_set_debug_level_param(int debugLevel);


/*
extern acl_logMask;

void acl_log_open(int flag);

void acl_log_close(int flag);
*/

#endif /*__ACL_LOG_h*/





