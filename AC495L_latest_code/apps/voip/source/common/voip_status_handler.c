/********************************************************************
 * AudioCodes improved application status handler                   *
 *                                                                  *
 * File Name:	voip_status_handler.c                               *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 18/03/08 - Coding started.                                       *
 *																	*
 ********************************************************************/

#include "voip_status_handler.h"
#include "acl_common.h"

/*#define AC_VOIP_STATUS_HANDLER_DEBUG*/

#ifdef AC_VOIP_STATUS_HANDLER_DEBUG
//#define DBG_PRINT(a1,a2...)	IppLogMessage(RV_FALSE, "[%s:%4d] " a1,__FUNCTION__, __LINE__, ##a2)
#define DBG_PRINT(a1,a2...)		printf( "[%s:%4d] " a1,__FUNCTION__, __LINE__, ##a2)
#else
#define DBG_PRINT(a1,a2...)
#endif

/* ER08 - Added macro for error messages (not regular debug messages) that shouls go into the RV logs */
#define DBG_PRINT_ERR(a1,a2...)	IppLogMessage(RV_TRUE, "[%s:%4d] " a1,__FUNCTION__, __LINE__, ##a2)

static sem_t			LineStatusSem;

static 	line_state_t	gateway_lines_state[CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES];


int lineValidityCheck(int line)
{
	DBG_PRINT(">>\r\n");
	if( (line >=0) && (line < CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES) )
	{
		return 0;
	}
	else
	{
		//printf("[%s:%d]  line %d is out of range\n", __FUNCTION__, __LINE__, line);
		return -1;
	}

	DBG_PRINT("<<\r\n");
}

void initVoipStatusHandler(void)
{
	int i;

	DBG_PRINT(">>\r\n");

	if(sem_init(&LineStatusSem, 0, 1) < 0)
	{
		printf("sem_init failed::LineStatusSem\r\n");
		return;
	}

	for(i=0; i<CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES; i++)
	{
		/* Do some initialization */
	}

	DBG_PRINT("<<\r\n");
}

void destroyVoipStatusHandler(void)
{
	DBG_PRINT(">>\r\n");

	if(sem_destroy(&LineStatusSem) < 0)
	{
		perror("sem_destroy::LineStatusSem\r\n");
	}

	DBG_PRINT("<<\r\n");
}

void getGatewayLineStateInfo(int line, line_state_t *pGateway_lines_state)
{
	DBG_PRINT(">>\r\n");

	if(lineValidityCheck(line) !=0)
		return;

	sem_wait(&LineStatusSem);

	memcpy(pGateway_lines_state, &gateway_lines_state[line], sizeof(line_state_t));

	sem_post(&LineStatusSem);

	DBG_PRINT("<<\r\n");
}


void SetLineStatus(int line, PORT_STATUS status)
{
	DBG_PRINT(">>\r\n");

	if(lineValidityCheck(line) !=0)
		return;

	sem_wait(&LineStatusSem);

	gateway_lines_state[line].portStatus = status;

	sem_post(&LineStatusSem);

	DBG_PRINT("<<\r\n");
}



int getSpecificPortStatus(int line)
{
	PORT_STATUS status;

	DBG_PRINT(">>\r\n");

	if(lineValidityCheck(line) !=0)
		return -1;

	sem_wait(&LineStatusSem);

	status = gateway_lines_state[line].portStatus;

	sem_post(&LineStatusSem);

	DBG_PRINT("<<\r\n");

	return status;
}

int setSpecificPortStatus(int line, PORT_STATUS status)
{
	int 			ret = 0;
	PORT_STATUS 	currentLineStatus;

	DBG_PRINT(">>\r\n");

	if(lineValidityCheck(line) !=0)
		return -1;

	sem_wait(&LineStatusSem);
	currentLineStatus = gateway_lines_state[line].portStatus;
	sem_post(&LineStatusSem);


	switch(status)
	{
		case PORT_LOCK:

			if(currentLineStatus != status)
			{
#if defined(ACL_SIP_APPLICATION)
				if(acl_sip_call_mngr_uninit_per_line(line) != 0){
					printf("\nError in acl_sip_call_mngr_uninit_per_line()\n");
					ret = -1;
					break;
				}
#endif /*ACL_SIP_APPLICATION*/

				sem_wait(&LineStatusSem);
				gateway_lines_state[line].portStatus = status;
				sem_post(&LineStatusSem);
			}
			break;

		case PORT_UNLOCK:

			if(currentLineStatus != status)
			{
#if defined(ACL_SIP_APPLICATION)
				if(acl_sip_call_mngr_init_per_line(line) != 0){
					printf("\nError in acl_sip_call_mngr_init_per_line()\n");
					ret = -1;
					break;
				}
#endif /*ACL_SIP_APPLICATION*/

				sem_wait(&LineStatusSem);
				gateway_lines_state[line].portStatus = status;
				sem_post(&LineStatusSem);
			}
			break;

		default:
			printf("\nUnsupported port status %d\n", status);
			break;

	}

	DBG_PRINT("<<\r\n");

	return ret;
}

int getNumberOfPortExists()
{
	DBG_PRINT(">>\r\n");

	return CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES;
}

