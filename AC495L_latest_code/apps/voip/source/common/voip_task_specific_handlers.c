/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	voip_task_specific_handlers.c                       *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 26/02/08 - Coding started.                                       *
 *																	*
 ********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "voip_status_handler.h"


extern int					g_iAcl_main_Voip_Task_AgentSocketFd;
extern struct sockaddr_un 	acl_main_Voip_Task_Agent_Remote_Addr;


int MyBeginConfigHandler(unsigned char *);
int MyBeginConfigAckHandler(unsigned char *);
int MyConfigHandler(unsigned char *);
int MyConfigAckHandler(unsigned char *);
int MyEndConfigHandler(unsigned char *);
int MyEndConfigAckHandler(unsigned char *);
int MyVoipTaskGetHandler(unsigned char *);
int MyGetResponseIntegerHandler(unsigned char *);
int MyGetResponseStringHandler(unsigned char *);
int MyVoipTaskGetNHandler(unsigned char *);
int MyGetNResponseIntegerHandler(unsigned char *);
int MyGetNResponseStringHandler(unsigned char *);
int MySetIntegerHandler(unsigned char *);
int MySetStringHandler(unsigned char *);
int MySetResponseIntegerHandler(unsigned char *);
int MySetResponseStringHandler(unsigned char *);
int MyVoipTaskSetNIntegerHandler(unsigned char *);
int MySetNStringHandler(unsigned char *);
int MySetNResponseIntegerHandler(unsigned char *);
int MySetNResponseStringHandler(unsigned char *);
int MyErrorHandler(unsigned char *);


TableEnt pVoipTaskTable[] = {
	{BEGIN_CONFIG, 			MyBeginConfigHandler			},
	{BEGIN_CONFIG_ACK, 		MyBeginConfigAckHandler			},
	{CONFIG, 					MyConfigHandler					},
	{CONFIG_ACK, 				MyConfigAckHandler				},
	{END_CONFIG, 				MyEndConfigHandler				},
	{END_CONFIG_ACK, 			MyEndConfigAckHandler			},
	{GET,					MyVoipTaskGetHandler			},
	{GET_RESPONSE_INTEGER,	MyGetResponseIntegerHandler	},
	{GET_RESPONSE_STRING,		MyGetResponseStringHandler		},
	{GET_N, 					MyVoipTaskGetNHandler			},
	{GET_N_RESPONSE_INTEGER,	MyGetNResponseIntegerHandler	},
	{GET_N_RESPONSE_STRING,	MyGetNResponseStringHandler	},
	{SET_INTEGER,				MySetIntegerHandler				},
	{SET_STRING,				MySetStringHandler				},
	{SET_RESPONSE_INTEGER,	MySetResponseIntegerHandler	},
	{SET_RESPONSE_STRING,		MySetResponseStringHandler		},
	{SET_N_INTEGER,			MyVoipTaskSetNIntegerHandler	},
	{SET_N_STRING,			MySetNStringHandler				},
	{SET_N_RESPONSE_INTEGER,	MySetNResponseIntegerHandler	},
	{SET_N_RESPONSE_STRING,	MySetNResponseStringHandler	},
	{ERROR, 					MyErrorHandler					}
};

TableEnt *Get_Table_Entry(CIP_OPCODES opcode)
{
	return &pVoipTaskTable[opcode];
}

int MyVoipTaskGetHandler(unsigned char *pInfo)
{
	statusRequest_msg_st	*pStatusRequest_msg;

	int portCount = getNumberOfPortExists();


	pStatusRequest_msg = (statusRequest_msg_st *)pInfo;

	switch(pStatusRequest_msg->statusRequest)
	{
		case GET_PORT_COUNT:
				if(cip_get_response(GET_RESPONSE_INTEGER, GET_PORT_COUNT, (char *)&portCount,
					g_iAcl_main_Voip_Task_AgentSocketFd, acl_main_Voip_Task_Agent_Remote_Addr) == -1)
					{
						printf("\nMyVoipTaskGetHandler::cip_get_response()");
						return (-1);
					}

				break;
		default:
				printf("\nMyVoipTaskGetHandler::Unrecognized status request");
				return (-1);
				break;
	}

	return 0;

}

int MyVoipTaskGetNHandler(unsigned char *pInfo)
{
	statusRequest_channel_msg_st	*pStatusRequest_channel_msg;
	PORT_STATUS portStatus;


	pStatusRequest_channel_msg = (statusRequest_channel_msg_st *)pInfo;

	switch(pStatusRequest_channel_msg->statusRequest)
	{
		case GET_PORT_STATUS_N:

				portStatus = getSpecificPortStatus(pStatusRequest_channel_msg->channel);
				if(cip_get_n_response(GET_N_RESPONSE_INTEGER,
					GET_PORT_STATUS_N, pStatusRequest_channel_msg->channel, (char *)&portStatus,
					g_iAcl_main_Voip_Task_AgentSocketFd, acl_main_Voip_Task_Agent_Remote_Addr) == -1)
					{
						printf("\ncip_get_n_response()");
						return (-1);
					}

				break;
		default:
				printf("\nUnrecognized status request");
				return (-1);
				break;
	}

	return 0;

}

int MyVoipTaskSetNIntegerHandler(unsigned char *pInfo)
{
	statusCommand_channel_value_msg_st	*pStatusCommand_channel_value_msg;
	PORT_STATUS							portStatus;

	pStatusCommand_channel_value_msg = (statusCommand_channel_value_msg_st *)pInfo;

	switch(pStatusCommand_channel_value_msg->statusCommand)
	{
		case SET_PORT_STATUS_N:

				memcpy(&portStatus, pStatusCommand_channel_value_msg->value, sizeof(portStatus));

				if(setSpecificPortStatus(pStatusCommand_channel_value_msg->channel,  portStatus) == -1)
					{
						printf("\nMyVoipTaskSetNIntegerHandler::setSpecificPortStatus()");
						return (-1);
					}

				if(cip_set_n_response(SET_N_RESPONSE_INTEGER, SET_PORT_STATUS_N,
					pStatusCommand_channel_value_msg->channel, (char *)&portStatus,
					g_iAcl_main_Voip_Task_AgentSocketFd, acl_main_Voip_Task_Agent_Remote_Addr) == -1)
					{
						printf("\nMyVoipTaskSetNIntegerHandler::cip_set_n_response()");
						return (-1);
					}

				break;
		default:
				printf("\nMyVoipTaskSetNIntegerHandler:;Unrecognized status command");
				return (-1);
				break;
	}

	return 0;

}

