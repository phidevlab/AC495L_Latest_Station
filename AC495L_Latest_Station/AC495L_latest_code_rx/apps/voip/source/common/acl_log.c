/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	acl_log.c                                           *
 *                                                                  *
 * Copyright (C) 2005 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 30/10/05 - Coding started.                                       *
 *																	*
 ********************************************************************/
/******************************************************************************
*                                                                            
* 	DESCRIPTION:
*		
******************************************************************************/

#include <stdio.h>
#include <stdarg.h>
#include "acl_log.h"


static int acl_log_debug_level;

void acl_log_set_debug_level_param(int debugLevel)
{
	acl_log_debug_level = debugLevel;
}


void acl_log(int flag, char *fmt, ...)
{	
	
	va_list	args;
	char	str[128];

	if(!acl_log_debug_level)	/* don't print if prints are disabled */
		return;
	va_start(args, fmt);

	if(flag & ACL_LOG_ERROR) 
	{
    	vsprintf(str, fmt, args);
		perror(str);
	}
	else if(flag & (acl_logMask)) 
	{
		vprintf(fmt, args);
		printf("\r\n");
	}

	va_end(args);
	
}


/*
void acl_log_open(int flag)
{
	acl_logMask |= flag;
}

void acl_log_close(int flag)
{
	acl_logMask &= ~flag;
}
*/













