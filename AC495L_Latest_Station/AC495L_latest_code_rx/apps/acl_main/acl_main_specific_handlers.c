
#include <stdio.h>
#include <stdlib.h>

#include "cip_def.h"
#include "cfg_api.h"
#include "cip_api.h"


extern int 					acl_main_Mng_AgentSocketFd;
extern  struct sockaddr_un 		acl_main_Mng_Agent_Remote_Addr;

extern int						acl_main_Voip_Task_ManagerSocketFd;
extern struct sockaddr_un 		acl_main_Voip_Task_Manager_Remote_Addr;


int MyBeginConfigHandler(unsigned char *);
int MyBeginConfigAckHandler(unsigned char *);
int MyConfigHandler(unsigned char *);
int MyConfigAckHandler(unsigned char *);
int MyEndConfigHandler(unsigned char *);
int MyEndConfigAckHandler(unsigned char *);
int MyACL_MainGetHandler(unsigned char *);
int MyGetResponseIntegerHandler(unsigned char *);
int MyGetResponseStringHandler(unsigned char *);
int MyACL_MainGetNHandler(unsigned char *);
int MyGetNResponseIntegerHandler(unsigned char *);
int MyGetNResponseStringHandler(unsigned char *);
int MySetIntegerHandler(unsigned char *);
int MySetStringHandler(unsigned char *);
int MySetResponseIntegerHandler(unsigned char *);
int MySetResponseStringHandler(unsigned char *);
int MyACL_MainSetNIntegerHandler(unsigned char *);
int MySetNStringHandler(unsigned char *);
int MySetNResponseIntegerHandler(unsigned char *);
int MySetNResponseStringHandler(unsigned char *);
int MyErrorHandler(unsigned char *);


TableEnt pACL_MainTable[] = {
	{BEGIN_CONFIG, 			MyBeginConfigHandler				},
	{BEGIN_CONFIG_ACK, 		MyBeginConfigAckHandler			},
	{CONFIG, 					MyConfigHandler					},
	{CONFIG_ACK, 				MyConfigAckHandler				},
	{END_CONFIG, 				MyEndConfigHandler				},
	{END_CONFIG_ACK, 			MyEndConfigAckHandler			},
	{GET,					MyACL_MainGetHandler				},
	{GET_RESPONSE_INTEGER,	MyGetResponseIntegerHandler		},
	{GET_RESPONSE_STRING,		MyGetResponseStringHandler		},
	{GET_N, 					MyACL_MainGetNHandler			},
	{GET_N_RESPONSE_INTEGER,	MyGetNResponseIntegerHandler		},
	{GET_N_RESPONSE_STRING,	MyGetNResponseStringHandler		},
	{SET_INTEGER,				MySetIntegerHandler				},
	{SET_STRING,				MySetStringHandler				},
	{SET_RESPONSE_INTEGER,	MySetResponseIntegerHandler		},
	{SET_RESPONSE_STRING,		MySetResponseStringHandler		},
	{SET_N_INTEGER,			MyACL_MainSetNIntegerHandler		},
	{SET_N_STRING,			MySetNStringHandler				},
	{SET_N_RESPONSE_INTEGER,	MySetNResponseIntegerHandler		},
	{SET_N_RESPONSE_STRING,	MySetNResponseStringHandler		},
	{ERROR, 					MyErrorHandler					}
};

TableEnt *Get_Table_Entry(CIP_OPCODES opcode)
{
	return &pACL_MainTable[opcode];
}


int MyACL_MainGetHandler(unsigned char *pInfo)
{
	statusRequest_msg_st	*pStatusRequest_msg;
	int 					portCount;


	pStatusRequest_msg = (statusRequest_msg_st *)pInfo;

	switch(pStatusRequest_msg->statusRequest)
	{
		case GET_PORT_COUNT:

				if(cfg_get_port_count(&portCount, acl_main_Voip_Task_ManagerSocketFd, 
					acl_main_Voip_Task_Manager_Remote_Addr) == -1)
				{
					printf("\nMyACL_MainGetHandler::cfg_get_port_count() failure\n");
					return (-1);
				}

				if(cip_get_response(GET_RESPONSE_INTEGER, GET_PORT_COUNT, 
									(char *)&portCount, acl_main_Mng_AgentSocketFd, 
										acl_main_Mng_Agent_Remote_Addr) == -1)
					{
						printf("\nMyACL_MainGetHandler::cip_get_response()");
						return (-1);
					}

				break;
		default:
				printf("\nMyACL_MainGetHandler::Unrecognized status request");
				return (-1);
				break;
	}

	return 0;
	
}

int MyACL_MainGetNHandler(unsigned char *pInfo)
{
	statusRequest_channel_msg_st	*pStatusRequest_channel_msg;
	int/*PORT_STATUS*/			portStatus;


	pStatusRequest_channel_msg = (statusRequest_channel_msg_st *)pInfo;

	switch(pStatusRequest_channel_msg->statusRequest)
	{
		case GET_PORT_STATUS_N:

				if(cfg_get_port_status(pStatusRequest_channel_msg->channel, 
					(int *)&portStatus, acl_main_Voip_Task_ManagerSocketFd, 
					acl_main_Voip_Task_Manager_Remote_Addr) == -1)
				{
					printf("\nMyACL_MainGetNHandler::cfg_get_port_status() failure\n");
					return (-1);
				}

				if(cip_get_n_response(GET_N_RESPONSE_INTEGER, 
					GET_PORT_STATUS_N, pStatusRequest_channel_msg->channel, 
					(char *)&portStatus, acl_main_Mng_AgentSocketFd, 
					acl_main_Mng_Agent_Remote_Addr) == -1)
					{
						printf("\nMyACL_MainGetNHandler::cip_get_n_response()");
						return (-1);
					}

				break;
		default:
				printf("\nMyACL_MainGetNHandler::Unrecognized status request");
				return (-1);
				break;
	}
	
	return 0;

}

int MyACL_MainSetNIntegerHandler(unsigned char *pInfo)
{
	statusCommand_channel_value_msg_st	*pStatusCommand_channel_value_msg;
	PORT_STATUS							portStatus;

	pStatusCommand_channel_value_msg = (statusCommand_channel_value_msg_st *)pInfo;

	switch(pStatusCommand_channel_value_msg->statusCommand)
	{

		case SET_PORT_STATUS_N:

				memcpy(&portStatus, pStatusCommand_channel_value_msg->value, sizeof(portStatus));

				if(cfg_set_port_status(pStatusCommand_channel_value_msg->channel, portStatus, 
									acl_main_Voip_Task_ManagerSocketFd, 
									acl_main_Voip_Task_Manager_Remote_Addr) == -1)
				{
					printf("\nMyACL_MainSetNIntegerHandler::cfg_set_port_status() failure\n");
					return (-1);
				}

				if(cip_set_n_response(SET_N_RESPONSE_INTEGER, SET_PORT_STATUS_N, 
					pStatusCommand_channel_value_msg->channel, 
					(char *)&portStatus, acl_main_Mng_AgentSocketFd, 
					acl_main_Mng_Agent_Remote_Addr) == -1)
					{
						printf("\nMyACL_MainSetNIntegerHandler::cip_set_n_response()");
						return (-1);
					}

				break;
		default:
				printf("\nMyACL_MainSetNIntegerHandler::Unrecognized status command");
				return (-1);
				break;
	}

	return 0;

}

