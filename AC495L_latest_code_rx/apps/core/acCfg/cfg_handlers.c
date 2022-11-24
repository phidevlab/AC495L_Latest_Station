/****************************************************************************
*                                                                            
*   Company:			Audiocodes Ltd.                                                      
*                                                                            
*   Project:			                                               
*                                                                          
*   Hardware Module: 	                                                  
*                                                                           
*   File Name: 		cfg_handlers.c	                                                 
*                                                                            
*   Creation Date:    	                                                     
*    	                                                                        
*   Last Update By:		                                                     
*                                                                            
******************************************************************************
*                                                                            
* 	DESCRIPTION:		Configuration Infrastructure Handlers		  		                                                             
*                                                                            
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/un.h>


#include <sys/types.h>
#include <sys/stat.h>  

#include "cip_def.h"
#include "cip_api.h"


TableEnt *Get_Table_Entry(CIP_OPCODES);
int MyBeginConfigHandler(unsigned char *);
int MyBeginConfigAckHandler(unsigned char *);
int MyConfigHandler(unsigned char *);
int MyConfigAckHandler(unsigned char *);
int MyEndConfigHandler(unsigned char *);
int MyEndConfigAckHandler(unsigned char *);
int MyGetResponseIntegerHandler(unsigned char *);
int MyGetResponseStringHandler(unsigned char *);
int MyGetNResponseIntegerHandler(unsigned char *);
int MyGetNResponseStringHandler(unsigned char *);
int MySetIntegerHandler(unsigned char *);
int MySetStringHandler(unsigned char *);
int MySetResponseIntegerHandler(unsigned char *);
int MySetResponseStringHandler(unsigned char *);
int MySetNStringHandler(unsigned char *);
int MySetNResponseIntegerHandler(unsigned char *);
int MySetNResponseStringHandler(unsigned char *);
int MyErrorHandler(unsigned char *);


extern int					remoteSocketFd;
extern struct sockaddr_un 	remoteAddr;

char 						receivedCfgBuf[CONFIGURATION_BUFFER_MAX_LEN] = "";


int MyBeginConfigHandler(unsigned char *pInfo)
{
//	receivedCfgBuf[0] = (char)NULL;	

	memset(receivedCfgBuf, 0, sizeof(receivedCfgBuf));	/* empty the buffer */

	if(cip_begin_config_ack(remoteSocketFd, remoteAddr) == -1)
	{
		printf("\ncip_begin_config_ack()");
		return (-1);
	}
	
	return 0;
}
int MyBeginConfigAckHandler(unsigned char *pInfo)
{
	return 0;
}
int MyConfigHandler(unsigned char *pInfo)
{

	segment_data_msg_st  *pSegment_data_msg;
	
	pSegment_data_msg = (segment_data_msg_st *)pInfo;

	strcat(receivedCfgBuf, pSegment_data_msg->data);
	strcat(receivedCfgBuf, "\r\n");

	if(cip_config_ack(pSegment_data_msg->segment, remoteSocketFd, remoteAddr) == -1)
	{
		printf("\ncip_config_ack()");
		return (-1);
	}
	
	return 0;

}
int MyConfigAckHandler(unsigned char *pInfo)
{
	return 0;
}
int MyEndConfigHandler(unsigned char *pInfo)
{
	strcat(receivedCfgBuf, "\0");
	
//	printf("\nreceivedCfgBuf = %s", receivedCfgBuf);

	if(cip_end_config_ack(remoteSocketFd, remoteAddr) == -1)
	{
		printf("\ncip_end_config_ack()");
		return (-1);
	}

	return 0;	
}

int MyEndConfigAckHandler(unsigned char *pInfo)
{
	return 0;
}

int MyGetResponseIntegerHandler(unsigned char *pInfo)
{
	return 0;
}

int MyGetResponseStringHandler(unsigned char *pInfo)
{
	return 0;
}

int MyGetNResponseIntegerHandler(unsigned char *pInfo)
{
	return 0;
}

int MyGetNResponseStringHandler(unsigned char *pInfo)
{
	return 0;
}


int MySetIntegerHandler(unsigned char *pInfo)
{
	return 0;
}


int MySetStringHandler(unsigned char *pInfo)
{
	statusCommand_value_msg_st	*pStatusCommand_value_msg;


	pStatusCommand_value_msg = (statusCommand_value_msg_st *)pInfo;

	switch(pStatusCommand_value_msg->statusCommand)
	{
		case SET_IP_ADDRESS_FOR_RECORDING:

				if(cip_set_response(SET_RESPONSE_STRING, 	SET_IP_ADDRESS_FOR_RECORDING, 
						"10.16.2.99", remoteSocketFd, remoteAddr) == -1)
					{
						printf("\ncip_set_response()");
						return (-1);
					}

				break;
		default:
				printf("\nUnrecognized status command");
				return (-1);
				break;
	}

	return 0;

}

int MySetResponseIntegerHandler(unsigned char *pInfo)
{
	return 0;
}

int MySetResponseStringHandler(unsigned char *pInfo)
{
	return 0;
}

int MySetNStringHandler(unsigned char *pInfo)
{
	return 0;
}

int MySetNResponseIntegerHandler(unsigned char *pInfo)
{
	return 0;
}

int MySetNResponseStringHandler(unsigned char *pInfo)
{
	return 0;
}

int MyErrorHandler(unsigned char *pInfo)
{
		return 0;
}

