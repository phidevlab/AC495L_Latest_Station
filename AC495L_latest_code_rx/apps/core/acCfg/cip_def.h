/****************************************************************************
*                                                                            
*    	Company: 			                                        
*                                                                            
*    	Project: 			                                             
*                                                                            
*    	Hardware Module: 	                                                  
*                                                                            
*    	File Name: 			                          
*                                                                            
*	Creation Date:    	                                                    
*    	                                                                        
*    	Last Update By:		                                                       
*                                                                            
*****************************************************************************
*                                                                            
* 		DESCRIPTION:  		                                                             
*                                                                            
******************************************************************************/

#ifndef	__CIP_DEF_h
#define	__CIP_DEF_h


#include <sys/un.h>

/*#define CIP_DEBUG*/

#define  	MAX_LEN  									512
#define  	CONFIGURATION_BUFFER_MAX_LEN 				21500

#define	START_PORT_NUMBER_FOR_CONFIGURATION		6000
#define	WAITING_FOR_BUFFER						"waiting_for_buffer"
#define	CONFIGURATION_FINISHED					"Configuration finished"

#define 	MNG_ACL_MAIN_MANAGER_SOCKET_NAME			"/tmp/VoIP_mng_acl_main_manager_socket"
#define 	MNG_ACL_MAIN_AGENT_SOCKET_NAME			"/tmp/VoIP_mng_acl_main_agent_socket"
#define 	ACL_MAIN_VOIP_TASK_MANAGER_SOCKET_NAME	"/tmp/VoIP_acl_main_voip_task_manager_socket"
#define 	ACL_MAIN_VOIP_TASK_AGENT_SOCKET_NAME		"/tmp/VoIP_acl_main_voip_task_agent_socket"


typedef enum{

	BEGIN_CONFIG,
	BEGIN_CONFIG_ACK,
	CONFIG,
	CONFIG_ACK,
	END_CONFIG,
	END_CONFIG_ACK,
	GET,
	GET_RESPONSE_INTEGER,
	GET_RESPONSE_STRING,
	GET_N,
	GET_N_RESPONSE_INTEGER,
	GET_N_RESPONSE_STRING,
	SET_INTEGER,
	SET_STRING,
	SET_RESPONSE_INTEGER,
	SET_RESPONSE_STRING,
	SET_N_INTEGER,
	SET_N_STRING,
	SET_N_RESPONSE_INTEGER,
	SET_N_RESPONSE_STRING,
	ERROR,
	NUMBER_OF_OPCODES
		
} CIP_OPCODES;


typedef enum{

	GET_PORT_COUNT,
	GET_PORT_STATUS_N
		
} CIP_STATUS_REQUESTS;


typedef enum{

	SET_PORT_STATUS_N,
	SET_IP_ADDRESS_FOR_RECORDING

} CIP_STATUS_COMMANDS;


typedef enum{

	UNRECOGNIZED_OPCODE

} CIP_ERROR_MESSAGES;

typedef enum{

	PORT_LOCK,
	PORT_UNLOCK

} PORT_STATUS;

typedef struct{
	
	CIP_OPCODES		opcode;
	unsigned char		info[MAX_LEN];

} __attribute__((__packed__)) cfg_msg;

typedef struct{
	int  	 segment;
	char data[MAX_LEN-1];
}  __attribute__((__packed__)) segment_data_msg_st;

typedef struct{
	int segment;	
} __attribute__((__packed__)) segment_msg_st;

typedef struct{
	CIP_STATUS_REQUESTS statusRequest;	
} __attribute__((__packed__)) statusRequest_msg_st;

typedef struct{
	CIP_STATUS_REQUESTS statusRequest;	
	char			 		 value[MAX_LEN];
} __attribute__((__packed__)) statusRequest_value_msg_st;

typedef struct{
	CIP_STATUS_REQUESTS statusRequest;	
	int			 		 channel;
} __attribute__((__packed__)) statusRequest_channel_msg_st;

typedef struct{
	CIP_STATUS_REQUESTS statusRequest;	
	int			 		 channel;
	char					 value[MAX_LEN];
} __attribute__((__packed__)) statusRequest_channel_value_msg_st;

typedef struct{
	CIP_STATUS_COMMANDS statusCommand;	
	char					   value[MAX_LEN];
} __attribute__((__packed__)) statusCommand_value_msg_st;

typedef struct{
	CIP_STATUS_COMMANDS statusCommand;
	int					   channel;	
	char					   value[MAX_LEN];
} __attribute__((__packed__)) statusCommand_channel_value_msg_st;

typedef struct{
	CIP_ERROR_MESSAGES	 message;
} __attribute__((__packed__)) error_msg_st;


typedef struct 
{
	CIP_OPCODES 	opcode;
	int 				(*opHandler)(unsigned char *);
} __attribute__((__packed__)) TableEnt;

#endif /* __CIP_DEF_h */
