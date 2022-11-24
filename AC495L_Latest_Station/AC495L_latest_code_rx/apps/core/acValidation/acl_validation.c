/**********************************************************************
 * acl_validation.c - AudioCodes Code validation module               * 
 *               													  *
 * platform and chip validation implementation				      * 	
 *																	  *																	
 * File Name:	acl_validation.c									  *
 *																	  *
 * Copyright (C) 2007 AudioCodes Ltd.								  * 	
 *																	  *  
 * Modifications :													  * 	
 *																	  * 
 * 29/04/08 - Coding started.										  * 	
 *																	  * 
 **********************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <fcntl.h>
#include <sys/ioctl.h>

#include "acGatewayDefs.h"

#ifdef P_PLATFORM
#include "PASONU/PASONU_expo.h"
#endif

//#define ACL_VALIDATION_PRINT(a1,a2...)		printf("[%s:%d] " a1, __FUNCTION__, __LINE__, ##a2);
#define ACL_VALIDATION_PRINT(a1,a2...)		

#define ACL_VALIDATION_ERROR_PRINT(a1,a2...)		{	printf("[%s:%d] " a1, __FUNCTION__, __LINE__, ##a2); \
								 	                    perror("Error message "); \
								                    }

static int acl_chip_validation();
static int acl_validation_platform_val_get(char *regVal);

extern int 	dspFd;	/* DSP file descriptor */

/******************************************************************************
*	Function:	 acl_validation_initialization
*
*	Description: Initialize the validation mechanism.
*
*	Parameters:
*
*	Returns:	 0 - Success or (-1) - Error
*
*******************************************************************************/
int acl_validation_init(void)
{
	ACL_VALIDATION_PRINT(">>\r\n");

	if(dspFd == -1)
	{
		printf("acGatewayAPI version is: %s\n", ACG_VERSION);

		if((dspFd = open(ACL_DSP_DRV_FD_NAME_STR, O_RDWR)) == ACG_FAILURE) 
		{
			ACL_VALIDATION_ERROR_PRINT("can't open dsp device\r\n");
			return (-1);
		}
	}
    
	ACL_VALIDATION_PRINT("<<\r\n");

	return 0;
}

/******************************************************************************
*	Function:	 acl_validation_val_get
*
*	Description: .
*
*	Parameters:	 
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
int acl_validation_val_get(char *regVal)
{
	int retValue = ACG_SUCCESS;

	if(dspFd == -1)
	{
		ACL_VALIDATION_ERROR_PRINT("DSP file descriptor was not initialized\r\n");
		return ACG_FAILURE;
	}

	/* Verify that we are working with AudioCodes DSP */
	retValue = acl_chip_validation();

	if (ACG_SUCCESS != retValue)
	{
		printf("\n**** You are not running on AudioCodes DSP ****\n");
		return ACG_FAILURE;
	}

	retValue = acl_validation_platform_val_get(regVal);

	ACL_VALIDATION_PRINT(" returned %s\r\n", regVal);

	return (retValue);
}

/******************************************************************************
*	Function:	 acl_chip_validation
*
*	Description: .
*
*	Parameters:	 
*
*	Returns:	 int - ACG_SUCCESS (0) or ACG_FAILURE (-1)
*
*******************************************************************************/
static int acl_chip_validation()
{
	int 		retValue = ACG_SUCCESS;
	char	 	regVal[64]="";


	retValue = ioctl(dspFd, ACG_VALIDATION_CHIP_GET_CMD, regVal);

	strcpy(regVal, "SUCCESS");

	if (ACG_SUCCESS != retValue)
	{
		ACL_VALIDATION_ERROR_PRINT("error at ioctl ACG_VALIDATION_CHIP_GET_CMD\r\n");
	}

	if (0 != strcmp("SUCCESS", regVal))
	       	return ACG_FAILURE;


	return (retValue);
}

static int acl_validation_platform_val_get(char *regVal)
{

#if defined(P_PLATFORM)
{
	PASONU_version_t	 	versionInfo;

	PASONU_get_versions(&versionInfo);

  	sprintf(regVal, "%X", versionInfo.hw_major_ver);
}
#elif defined(B_PLATFORM)||defined(B_CEVA_PLATFORM)||defined(GROCX_PLATFORM)||defined(S_PLATFORM)
{
	int retValue = ACG_SUCCESS;

	retValue = ioctl(dspFd, ACG_VALIDATION_PLATFORM_GET_CMD, regVal);

	strcpy(regVal, "SUCCESS");

	if (ACG_SUCCESS != retValue)
	{
		ACL_VALIDATION_ERROR_PRINT("error at ioctl ACG_VALIDATION_PLATFORM_GET_CMD\r\n");
	}
}
#else
{
	printf("\nUnsupported Platform");
	return ACG_FAILURE;
}
#endif

	return 0;
}

