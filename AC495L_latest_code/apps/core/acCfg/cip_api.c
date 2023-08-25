/****************************************************************************
*                                                                            
*    	Company: 		AudioCodes Ltd.	                                        
*                                                                            
*    	Project: 			                                             
*                                                                            
*    	Hardware Module: 	                                                  
*                                                                            
*    	File Name: 			cip_api.c                        
*                                                                            
*	Creation Date:    	      13.2.07                                              
*    	                                                                        
*    	Last Update By:		                                                       
*                                                                            
*****************************************************************************
*                                                                            
* 	DESCRIPTION:  	Configuration Infrastructure Protocol Low level API's Source File                                                             
*                                                                            
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>  
#include <sys/un.h>

#include "cip_def.h"
#include "cip_api.h"

#ifdef CIP_DEBUG
unsigned char	debugStr[256]="";
#endif


/****************************************************************************
*
*	Name:		cip_begin_config
*----------------------------------------------------------------------------
*	Abstract:	 	send a BEGIN_CONFIG message 
*----------------------------------------------------------------------------
*	Input:		int 				   SocketFd		-  socket file descriptor	
*				struct sockaddr_un	   remote_addr		-  remote address
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 in case of error
******************************************************************************/
int cip_begin_config(int SocketFd, struct sockaddr_un remote_addr)
{
	ssize_t 	num_bytes;
	size_t 	size;
	int 		message = (int)BEGIN_CONFIG;

	size = sizeof(int);


#ifdef CIP_DEBUG
	printf("\ncip_begin_config::sending to socketFd = %d, size of message = %d  message = %d\n", 
					SocketFd, size, message);
#endif	

	if((num_bytes = sendto(SocketFd, &message, size, 0, (struct sockaddr *)&remote_addr, 
								sizeof(remote_addr))) == -1){
							
  	  	perror("cip_begin_config::sendto()");
		return (-1);
  	}

	return 0;
}

/****************************************************************************
*
*	Name:		cip_begin_config_ack
*----------------------------------------------------------------------------
*	Abstract:	 	send a BEGIN_CONFIG_ACK message 
*----------------------------------------------------------------------------
*	Input:		int 				   SocketFd	-  socket file descriptor	
*				struct sockaddr_un	   remote_addr		-  remote address
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 in case of error
******************************************************************************/
int cip_begin_config_ack(int SocketFd, struct sockaddr_un remote_addr)
{
	ssize_t 	num_bytes;
	size_t 	size;
	int 		message = (int)BEGIN_CONFIG_ACK;

	size = sizeof(int);


#ifdef CIP_DEBUG	
	printf("\ncip_begin_config_ack::sending to socketFd = %d, size of message = %d  message = %d\n", 
					SocketFd, size, message);
#endif	
	
	if((num_bytes = sendto(SocketFd, &message, size, 0, (struct sockaddr *)&remote_addr, 
								sizeof(remote_addr))) == -1){
							
  	  	perror("cip_begin_config_ack::sendto()");
		exit(1);
  	}

	return 0;
}

/****************************************************************************
*
*	Name:		cip_config
*----------------------------------------------------------------------------
*	Abstract:	 	send a CONFIG message with segment number
*----------------------------------------------------------------------------
*	Input:		int				segment_number 	-  segment number of the message.
*				char 				*data		    	-  configuration data to be send
*				int				data_size		    	-  size of configuration data.
*				int 				SocketFd			-  socket file descriptor	
*				struct sockaddr_un	remote_addr		-  remote address
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 in case of error
******************************************************************************/
int cip_config(int segment_number, char *data, int data_size, 
				int SocketFd, struct sockaddr_un remote_addr)
{
#ifdef CIP_DEBUG
	int		i=0;
#endif	

	ssize_t 	num_bytes;
	size_t 	size;
	cfg_msg 	message;

	segment_data_msg_st	segment_data_msg;

	message.opcode = CONFIG;
	segment_data_msg.segment = segment_number;
	memcpy(segment_data_msg.data, data, data_size);

	size = sizeof(int)+data_size;

	memcpy(message.info, &segment_data_msg, size);

	size += sizeof(CIP_OPCODES);

#ifdef CIP_DEBUG
	memcpy(debugStr, &message, size);

	printf("\ncip_config::sending to socketFd = %d, message.opcode = %d, size of message = %d\n", 
					SocketFd, message.opcode, size);
	printf("message = ");
	for(i=0; i<size; i++)
		printf("%02x", debugStr[i]);
	printf("\n");
#endif	

	if((num_bytes = sendto(SocketFd, &message, size, 0, (struct sockaddr *)&remote_addr, 
									sizeof(remote_addr))) == -1){
							
  	  	perror("cip_config::sendto()");
		return (-1);
  	}

	return 0;
}

/****************************************************************************
*
*	Name:		cip_config_ack
*----------------------------------------------------------------------------
*	Abstract:	 	send a CONFIG_ACK message with segment number
*----------------------------------------------------------------------------
*	Input:		int				segment_number  	-  segment number of the message 
*												    which the acknowledgment refers to.
*				int 				SocketFd			-  socket file descriptor	
*				struct sockaddr_un	remote_addr		-  remote address
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 in case of error
******************************************************************************/
int cip_config_ack(int segment_number, int SocketFd, struct sockaddr_un remote_addr)
{
#ifdef CIP_DEBUG
	int		i=0;
#endif	

	ssize_t 	num_bytes;
	size_t 	size;
	cfg_msg 	message;

	segment_msg_st  segment_msg;

	message.opcode = CONFIG_ACK;
	segment_msg.segment = segment_number;

	size = sizeof(segment_msg_st);

	memcpy(message.info, &segment_msg, size);

	size += sizeof(CIP_OPCODES);

#ifdef CIP_DEBUG
	memcpy(debugStr, &message, size);
	
	printf("\ncip_config_ack:: sending to socketFd = %d, message.opcode = %d, size of message = %d\n", 
					SocketFd, message.opcode, size);
	printf("message = ");
	for(i=0; i<size; i++)
		printf("%02x", debugStr[i]);
	printf("\n");
#endif	

	if((num_bytes = sendto(SocketFd, &message, size, 0, (struct sockaddr *)&remote_addr, 
									sizeof(remote_addr))) == -1){
							
  	  	perror("cip_config_ack::sendto()");
		return (-1);
  	}

	return 0;
}

/****************************************************************************
*
*	Name:		cip_end_config
*----------------------------------------------------------------------------
*	Abstract:	 	send a END_CONFIG message 
*----------------------------------------------------------------------------
*	Input:		int 				SocketFd			-  socket file descriptor	
*				struct sockaddr_un	remote_addr		-  remote address
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 in case of error
******************************************************************************/
int cip_end_config(int SocketFd, struct sockaddr_un remote_addr)
{
	ssize_t 	num_bytes;
	size_t 	size;
	int 		message	= (int)END_CONFIG;
	
	size = sizeof(int);

#ifdef CIP_DEBUG
	printf("\ncip_end_config::sending to socketFd = %d, message = %d, size of message = %d\n", 
					SocketFd, message, size);
#endif	

	if((num_bytes = sendto(SocketFd, &message, size, 0, (struct sockaddr *)&remote_addr, 
								sizeof(remote_addr))) == -1){
							
  	  	perror("cip_end_config::sendto()");
		return (-1);
  	}

	return 0;
}

/****************************************************************************
*
*	Name:		cip_end_config_ack
*----------------------------------------------------------------------------
*	Abstract:	 	send a END_CONFIG_ACK message 
*----------------------------------------------------------------------------
*	Input:		int 				SocketFd			-  socket file descriptor	
*				struct sockaddr_un	remote_addr		-  remote address
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 in case of error
******************************************************************************/
int cip_end_config_ack(int SocketFd, struct sockaddr_un remote_addr)
{
	ssize_t 	num_bytes;
	size_t 	size;
	int 		message	= (int)END_CONFIG_ACK;

	size = sizeof(int);


#ifdef CIP_DEBUG
	printf("\ncip_end_config_ack::sending to socketFd = %d, message = %d, size of message = %d\n", 
					SocketFd, message, size);
#endif	

	if((num_bytes = sendto(SocketFd, &message, size, 0, (struct sockaddr *)&remote_addr, 
								sizeof(remote_addr))) == -1){
							
  	  	perror("cip_end_config_ack::sendto()");
		return (-1);
  	}

	return 0;
}

/****************************************************************************
*
*	Name:		cip_get
*----------------------------------------------------------------------------
*	Abstract:	 send a GET status request message 
*----------------------------------------------------------------------------
*	Input:	CIP_STATUS_REQUESTS  		request  		-  the status request identifier
*			int 						SocketFd		-  socket file descriptor	
*			struct sockaddr_un			remote_addr	-  remote address
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 in case of error
******************************************************************************/
int cip_get(CIP_STATUS_REQUESTS request, int SocketFd, struct sockaddr_un remote_addr)
{
#ifdef CIP_DEBUG
	int		i=0;
#endif	

	ssize_t 	num_bytes;
	size_t 	size;
	cfg_msg 	message;

	statusRequest_msg_st  statusRequest_msg;

	message.opcode = GET;
	statusRequest_msg.statusRequest = request;

	size = sizeof(statusRequest_msg_st);

	memcpy(message.info, &statusRequest_msg, size);

	size += sizeof(CIP_OPCODES);

#ifdef CIP_DEBUG
	memcpy(debugStr, &message, size);
	
	printf("\ncip_get::sending to socketFd = %d, message.opcode = %d, size of message = %d\n", 
					SocketFd, message.opcode, size);
	printf("message = ");
	for(i=0; i<size; i++)
		printf("%02x", debugStr[i]);
	printf("\n");
#endif	

	if((num_bytes = sendto(SocketFd, &message, size, 0, (struct sockaddr *)&remote_addr, 
								sizeof(remote_addr))) == -1){
							
  	  	perror("cip_get::sendto()");
		return (-1);
  	}

	return 0;
}

/****************************************************************************
*
*	Name:		cip_get_response
*----------------------------------------------------------------------------
*	Abstract:	 	send a GET_RESPONSE message 
*----------------------------------------------------------------------------
*	Input:		CIP_OPCODES 		 opcode   		-  opcode
*				CIP_STATUS_REQUESTS   request  		-  the status request identifier
*				char 				  	 *value	    	-  the value of the "GET" status request
*				int 					SocketFd		-  socket file descriptor	
*				struct sockaddr_un		remote_addr	-  remote address
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 in case of error
******************************************************************************/
int cip_get_response(CIP_OPCODES opcode, CIP_STATUS_REQUESTS request, 
						char *value, int SocketFd, struct sockaddr_un remote_addr)
{
#ifdef CIP_DEBUG
	int		i=0;
#endif	

	ssize_t 	num_bytes;
	size_t 	size = 0;
	cfg_msg 	message;

	statusRequest_value_msg_st  statusRequest_value_msg;

	memset(&statusRequest_value_msg, 0, sizeof(statusRequest_value_msg));
	
	message.opcode= opcode;

	statusRequest_value_msg.statusRequest = request;
	
	if(opcode == GET_RESPONSE_INTEGER)
	{
		memcpy(statusRequest_value_msg.value, (char *)value, sizeof(int));
		size = sizeof(CIP_STATUS_REQUESTS) + sizeof(int);
	}
	else if(opcode == GET_RESPONSE_STRING)
		{
			memcpy(statusRequest_value_msg.value, (char *)value, strlen(value));
			size = sizeof(CIP_STATUS_REQUESTS) + strlen(value);
		}
	
	if(size != 0)
		memcpy(message.info, &statusRequest_value_msg, size);
	else
	{
  	  	printf("Trying to memcpy with size %d", size);
		return (-1);
	}
	
	size += sizeof(CIP_OPCODES);

#ifdef CIP_DEBUG
	memcpy(debugStr, &message, size);
	
	printf("\ncip_get_response::sending to socketFd = %d, message.opcode = %d, size of message = %d\n", 
					SocketFd, message.opcode, size);
	printf("message = ");
	for(i=0; i<size; i++)
		printf("%02x", debugStr[i]);
	printf("\n");
#endif	

	if((num_bytes = sendto(SocketFd, &message, size, 0, (struct sockaddr *)&remote_addr, 
								sizeof(remote_addr))) == -1){
							
  	  	perror("cip_get_response::sendto()");
		return (-1);
  	}

	return 0;
}

/****************************************************************************
*
*	Name:		cip_get_n
*----------------------------------------------------------------------------
*	Abstract:	 	send a GET_N status request message 
*----------------------------------------------------------------------------
*	Input:		CIP_STATUS_REQUESTS   request   -  the status request identifier
*				int					channel   -  the channel from which the status request refers to.
*				int 					SocketFd	-  socket file descriptor	
*				struct sockaddr_un		remote_addr	-  remote address
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 in case of error
******************************************************************************/
int cip_get_n(CIP_STATUS_REQUESTS request, int channel, int SocketFd, struct sockaddr_un remote_addr)
{
#ifdef CIP_DEBUG
	int		i=0;
#endif	

	ssize_t 	num_bytes;
	size_t 	size;
	cfg_msg 	message;
	
	statusRequest_channel_msg_st  statusRequest_channel_msg;

	message.opcode= GET_N;
	statusRequest_channel_msg.statusRequest = request;
	statusRequest_channel_msg.channel = channel;

	size = sizeof(statusRequest_channel_msg_st);

	memcpy(message.info, &statusRequest_channel_msg, size);

	size += sizeof(CIP_OPCODES);

#ifdef CIP_DEBUG
	memcpy(debugStr, &message, size);
	
	printf("\ncip_get_n::sending to socketFd = %d, message.opcode = %d, size of message = %d\n", 
					SocketFd, message.opcode, size);
	printf("message = ");
	for(i=0; i<size; i++)
		printf("%02x", debugStr[i]);
	printf("\n");	
#endif	

	if((num_bytes = sendto(SocketFd, &message, size, 0, (struct sockaddr *)&remote_addr, 
									sizeof(remote_addr))) == -1){
							
  	  	perror("cip_get_n::sendto()");
		return (-1);
  	}

	return 0;
}

/****************************************************************************
*
*	Name:		cip_get_n_response
*----------------------------------------------------------------------------
*	Abstract:	 	send a GET_N_RESPONSE  message 
*----------------------------------------------------------------------------
*	Input:		CIP_OPCODES 		opcode 		-  opcode
*				CIP_STATUS_REQUESTS  request   		-  the status request identifier
*				int					channel  		-  the channel from which the status request refers to.
*				char					*value     		-  the value of the "GET_N" status request
*				int 					SocketFd		-  socket file descriptor	
*				struct sockaddr_un		remote_addr	-  remote address
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 in case of error
******************************************************************************/
int cip_get_n_response(CIP_OPCODES opcode, CIP_STATUS_REQUESTS request, int channel, 
					char *value, int SocketFd, struct sockaddr_un remote_addr)
{
#ifdef CIP_DEBUG
	int		i=0;
#endif	

	ssize_t 	num_bytes;
	size_t 	size = 0;
	cfg_msg 	message;

	statusRequest_channel_value_msg_st  statusRequest_channel_value_msg;

	memset(&statusRequest_channel_value_msg, 0, sizeof(statusRequest_channel_value_msg));

	message.opcode= opcode;
	statusRequest_channel_value_msg.statusRequest = request;
	statusRequest_channel_value_msg.channel = channel;

	if(opcode == GET_N_RESPONSE_INTEGER)
	{
		memcpy(statusRequest_channel_value_msg.value, (char *)value, sizeof(int));
		size = sizeof(CIP_STATUS_REQUESTS)+sizeof(int) + sizeof(int);
	}
	else if(opcode == GET_N_RESPONSE_STRING)
		{
			memcpy(statusRequest_channel_value_msg.value, (char *)value, strlen(value));
			size = sizeof(CIP_STATUS_REQUESTS)+sizeof(int) + strlen(value);
		}

	if(size != 0)
		memcpy(message.info, &statusRequest_channel_value_msg, size);
	else
	{
  	  	printf("Trying to memcpy with size %d", size);
		return (-1);
	}

	size += sizeof(CIP_OPCODES);

#ifdef CIP_DEBUG
	memcpy(debugStr, &message, size);
	
	printf("\ncip_get_n_response::sending to socketFd = %d, message.opcode = %d, size of message = %d\n", 
					SocketFd, message.opcode, size);
	printf("message = ");
	for(i=0; i<size; i++)
		printf("%02x", debugStr[i]);
	printf("\n");
#endif	

	if((num_bytes = sendto(SocketFd, &message, size, 0, (struct sockaddr *)&remote_addr, 
								sizeof(remote_addr))) == -1){
							
  	  	perror("cip_get_n_response::sendto()");
		return (-1);
  	}

	return 0;
}

/****************************************************************************
*
*	Name:		cip_set
*----------------------------------------------------------------------------
*	Abstract:	 	send a SET status command message 
*----------------------------------------------------------------------------
*	Input:		CIP_OPCODES 		   	opcode		-  opcode
*				CIP_STATUS_COMMANDS    	command   	-  the status command identifier
*				char					   	*value	 	-  the value to set
*				int 						SocketFd		-  socket file descriptor	
*				struct sockaddr_un			remote_addr	-  remote address
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 in case of error
******************************************************************************/
int cip_set(CIP_OPCODES opcode, CIP_STATUS_COMMANDS command, char *value, 
					int SocketFd, struct sockaddr_un remote_addr)
{
#ifdef CIP_DEBUG
	int		i=0;
#endif	

	ssize_t 	num_bytes;
	size_t 	size = 0;
	cfg_msg 	message;

	statusCommand_value_msg_st  statusCommand_value_msg;

	memset(&statusCommand_value_msg, 0, sizeof(statusCommand_value_msg));
	
	message.opcode= opcode;
	statusCommand_value_msg.statusCommand = command;

	if(opcode == SET_INTEGER)
	{
		memcpy(statusCommand_value_msg.value, (char *)value, sizeof(int));
		size = sizeof(CIP_STATUS_COMMANDS) + sizeof(int);
	}
	else if(opcode == SET_STRING)
		{
			memcpy(statusCommand_value_msg.value, (char *)value, strlen(value));
			size = sizeof(CIP_STATUS_COMMANDS) + strlen(value);
		}

	if(size != 0)
		memcpy(message.info, &statusCommand_value_msg, size);
	else
	{
  	  	printf("Trying to memcpy with size %d", size);
		return (-1);
	}

	size += sizeof(CIP_OPCODES);

#ifdef CIP_DEBUG
	memcpy(debugStr, &message, size);
	
	printf("\ncip_set::sending to socketFd = %d, message.opcode = %d, size of message = %d\n", 
					SocketFd, message.opcode, size);
	printf("message = ");
	for(i=0; i<size; i++)
		printf("%02x", debugStr[i]);
	printf("\n");	
#endif	

	if((num_bytes = sendto(SocketFd, &message, size, 0, (struct sockaddr *)&remote_addr, 
										sizeof(remote_addr))) == -1){
							
  	  	perror("cip_set::sendto()");
		return (-1);
  	}

	return 0;
}

/****************************************************************************
*
*	Name:		cip_set_response
*----------------------------------------------------------------------------
*	Abstract:	 	send a SET_RESPONSE  message 
*----------------------------------------------------------------------------
*	Input:		CIP_OPCODES				opcode		-  opcode
*				CIP_STATUS_COMMANDS      	command 	-  the status command identifier
*				char 						*value    		-   the value which was set due to 
*											    	  		 "SET" status command.
*				int 						SocketFd		-  socket file descriptor	
*				struct sockaddr_un			remote_addr	-  remote address
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 in case of error
******************************************************************************/
int cip_set_response(CIP_OPCODES opcode, CIP_STATUS_COMMANDS command, char *value, 
					int SocketFd, struct sockaddr_un remote_addr)
{
#ifdef CIP_DEBUG
	int		i=0;
#endif	

	ssize_t 	num_bytes;
	size_t 	size = 0;
	cfg_msg 	message;

	statusCommand_value_msg_st  statusCommand_value_msg;

	memset(&statusCommand_value_msg, 0, sizeof(statusCommand_value_msg));
	
	message.opcode= opcode;
	statusCommand_value_msg.statusCommand = command;

	if(opcode == SET_RESPONSE_INTEGER)
	{
		memcpy(statusCommand_value_msg.value, (char *)value, sizeof(int));
		size = sizeof(CIP_STATUS_COMMANDS) + sizeof(int);
	}
	else if(opcode == SET_RESPONSE_STRING)
		{
			memcpy(statusCommand_value_msg.value, (char *)value, strlen(value));
			size = sizeof(CIP_STATUS_COMMANDS) + strlen(value);
		}

	if(size != 0)
		memcpy(message.info, &statusCommand_value_msg, size);
	else
	{
  	  	printf("Trying to memcpy with size %d", size);
		return (-1);
	}

	size += sizeof(CIP_OPCODES);

#ifdef CIP_DEBUG
	memcpy(debugStr, &message, size);
	
	printf("\ncip_set_response::sending to socketFd = %d, message.opcode = %d, size of message = %d\n", 
					SocketFd, message.opcode, size);
	printf("message = ");
	for(i=0; i<size; i++)
		printf("%02x", debugStr[i]);
	printf("\n");
#endif	

	if((num_bytes = sendto(SocketFd, &message, size, 0, (struct sockaddr *)&remote_addr, 
									sizeof(remote_addr))) == -1){
							
  	  	perror("cip_set_response::sendto()");
		return (-1);
  	}

	return 0;
}

/****************************************************************************
*
*	Name:		cip_set_n
*----------------------------------------------------------------------------
*	Abstract:	 	send a SET_N  status command message 
*----------------------------------------------------------------------------
*	Input:		CIP_OPCODES 		    opcode		-  opcode
*				CIP_STATUS_COMMANDS    command   	-  the status command identifier
*				int					    channel	-  the channel from which the status 
*											    	    command refers to.
*				char 				 	   *value        	-  the value to set
*				int 					   SocketFd	-  socket file descriptor	
*				struct sockaddr_un		  remote_addr	-  remote address
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 in case of error
******************************************************************************/
int cip_set_n(CIP_OPCODES opcode, CIP_STATUS_COMMANDS command, int channel, 
			 char *value, int SocketFd, struct sockaddr_un remote_addr)
{
#ifdef CIP_DEBUG
	int		i=0;
#endif	

	ssize_t 	num_bytes;
	size_t 	size = 0;
	cfg_msg 	message;

	statusCommand_channel_value_msg_st  statusCommand_channel_value_msg;

	memset(&statusCommand_channel_value_msg, 0, sizeof(statusCommand_channel_value_msg));
	
	message.opcode= opcode;
	statusCommand_channel_value_msg.statusCommand = command;
	statusCommand_channel_value_msg.channel = channel;

	if(opcode == SET_N_INTEGER)
	{
		memcpy(statusCommand_channel_value_msg.value, (char *)value, sizeof(int));
		size = sizeof(CIP_STATUS_COMMANDS) + sizeof(int) + sizeof(int);
	}
	else if(opcode == SET_N_STRING)
		{
			memcpy(statusCommand_channel_value_msg.value, (char *)value, strlen(value));
			size = sizeof(CIP_STATUS_COMMANDS) + sizeof(int) + strlen(value);
		}

	if(size != 0)
		memcpy(message.info, &statusCommand_channel_value_msg, size);
	else
	{
  	  	printf("Trying to memcpy with size %d", size);
		return (-1);
	}

	size += sizeof(CIP_OPCODES);

#ifdef CIP_DEBUG
	memcpy(debugStr, &message, size);
	
	printf("\ncip_set_n::sending to socketFd = %d, message.opcode = %d, size of message = %d\n", 
					SocketFd, message.opcode, size);
	printf("message = ");
	for(i=0; i<size; i++)
		printf("%02x", debugStr[i]);
	printf("\n");	
#endif	

	if((num_bytes = sendto(SocketFd, &message, size, 0, (struct sockaddr *)&remote_addr, 
								sizeof(remote_addr))) == -1){
							
  	  	perror("cip_set_n::sendto()");
		return (-1);
  	}

	return 0;
}

/****************************************************************************
*
*	Name:		cip_set_n_response
*----------------------------------------------------------------------------
*	Abstract:	 	send a SET_N_RESPONSE  message 
*----------------------------------------------------------------------------
*	Input:		CIP_OPCODES			    opcode		-  opcode
*				CIP_STATUS_COMMANDS  command  	-  the status command identifier
*				int					    channel	-  the channel from which the status 
*											   	    command refers to.
*				char					    *value        	-  the value which was set due to 
*											   	    "SET_N" status command.
*				int 					   SocketFd	-  socket file descriptor	
*				struct sockaddr_un		  remote_addr	-  remote address
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 in case of error
******************************************************************************/
int cip_set_n_response(CIP_OPCODES opcode, CIP_STATUS_COMMANDS command, int channel, 
					 char *value, int SocketFd, struct sockaddr_un remote_addr)
{
#ifdef CIP_DEBUG
	int		i=0;
#endif	

	ssize_t 	num_bytes;
	size_t 	size = 0;
	cfg_msg 	message;

	statusCommand_channel_value_msg_st  statusCommand_channel_value_msg;

	memset(&statusCommand_channel_value_msg, 0, sizeof(statusCommand_channel_value_msg));
	
	message.opcode= opcode;
	statusCommand_channel_value_msg.statusCommand = command;
	statusCommand_channel_value_msg.channel = channel;

	if(opcode == SET_N_RESPONSE_INTEGER)
	{	
		memcpy(statusCommand_channel_value_msg.value, (char *)value, sizeof(int));
		size = sizeof(CIP_STATUS_COMMANDS) + sizeof(int) + sizeof(int);
	}
	else if(opcode == SET_N_RESPONSE_STRING)
		{
			memcpy(statusCommand_channel_value_msg.value, (char *)value, strlen(value));
			size = sizeof(CIP_STATUS_COMMANDS) + sizeof(int) + strlen(value);
		}

	if(size != 0)
		memcpy(message.info, &statusCommand_channel_value_msg, size);
	else
	{
  	  	printf("Trying to memcpy with size %d", size);
		return (-1);
	}

	size += sizeof(CIP_OPCODES);

#ifdef CIP_DEBUG
	memcpy(debugStr, &message, size);
	
	printf("\ncip_set_n_response::sending to socketFd = %d, message.opcode = %d, size of message = %d\n", 
					SocketFd, message.opcode, size);
	printf("message = ");
	for(i=0; i<size; i++)
		printf("%02x", debugStr[i]);
	printf("\n");
#endif	

	if((num_bytes = sendto(SocketFd, &message, size, 0, (struct sockaddr *)&remote_addr, 
									sizeof(remote_addr))) == -1){							
  	  	perror("cip_set_n_response::sendto()");
		return (-1);
	}

	return 0;
}


/****************************************************************************
*
*	Name:		cip_send_general
*----------------------------------------------------------------------------
*	Abstract:	 	send a general message 
*----------------------------------------------------------------------------
*	Input:		char 				*pBuf		-  the requested buffer to send
*				int 				SocketFd		-  socket file descriptor	
*				struct sockaddr_un	remote_addr	-  remote address
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 in case of error
******************************************************************************/
int cip_send_general(char *pBuf, int SocketFd, struct sockaddr_un remote_addr)
{

	ssize_t 	num_bytes;
	size_t 	size;
	char 		buf[MAX_LEN]	= "";

	if(strlen(pBuf) < sizeof(buf))
		strcpy(buf, pBuf);
	else
	{
   	 	printf("[%s:%d] String length of pBuf %d is equal or bigger then sizeof buf %d\r\n",
					__FUNCTION__, __LINE__, strlen(pBuf), sizeof(buf));
		return (-1);
	}

	size = strlen(buf);

#ifdef CIP_DEBUG
	printf("\ncip_send_general::sending to socketFd = %d, buf = %s, size of buf = %d\n", 
					SocketFd, buf, size);
#endif	

	if((num_bytes = sendto(SocketFd, &buf, size, 0, (struct sockaddr *)&remote_addr, 
								sizeof(remote_addr))) == -1){							
  	  	perror("cip_send_general::sendto()");
		return (-1);
	}

	return 0;
	
}

/****************************************************************************
*
*	Name:		cip_error
*----------------------------------------------------------------------------
*	Abstract:	 	send a ERROR message 
*----------------------------------------------------------------------------
*	Input:		CIP_ERROR_MESSAGES   error_message   	-  the error message identifier
*				int 					SocketFd			-  socket file descriptor	
*				struct sockaddr_un		remote_addr		-  remote address
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 in case of error
******************************************************************************/
int cip_error(CIP_ERROR_MESSAGES error_message, int SocketFd, struct sockaddr_un remote_addr)
{
#ifdef CIP_DEBUG
	int		i=0;
#endif	

	ssize_t 	num_bytes;
	size_t 	size;
	cfg_msg 	message;

	memset(&message, 0, sizeof(message));

	error_msg_st  error_msg;

	message.opcode= ERROR;

	size = sizeof(error_msg_st);

	error_msg.message = error_message;

	size += sizeof(CIP_OPCODES);

#ifdef CIP_DEBUG
	memcpy(debugStr, &message, size);
	
	printf("\ncip_error::sending to socketFd = %d, message.opcode = %d, size of message = %d\n", 
					SocketFd, message.opcode, size);
	printf("message = ");
	for(i=0; i<size; i++)
		printf("%02x", debugStr[i]);
	printf("\n");
#endif	

	if((num_bytes = sendto(SocketFd, &message, size, 0, (struct sockaddr *)&remote_addr, 
									sizeof(remote_addr))) == -1){							
  	  	perror("cip_error::sendto()");
		return (-1);
	}

	return 0;
}

