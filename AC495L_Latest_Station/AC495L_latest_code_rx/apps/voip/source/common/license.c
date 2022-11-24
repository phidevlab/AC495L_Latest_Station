/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	license.c                                           *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 12/10/2006 - Coding started.                                     *
 *																	*
 ********************************************************************/
/*****************************************************************************
*                                                                            
* 		DESCRIPTION:  		                                                             
*                                                                            
******************************************************************************/
#include <string.h>
#include "license.h"


void get_license_key(char *license_key)
{
	strcpy(license_key, LICENSE_KEY);
}

