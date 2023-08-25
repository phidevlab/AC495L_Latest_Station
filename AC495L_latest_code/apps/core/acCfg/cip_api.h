/****************************************************************************
*                                                                            
*    	Company: 		AudioCodes Ltd.	                                        
*                                                                            
*    	Project: 			                                             
*                                                                            
*    	Hardware Module: 	                                                  
*                                                                            
*    	File Name: 			cip_api.h                         
*                                                                            
*	Creation Date:    	      13.2.07                                              
*    	                                                                        
*    	Last Update By:		                                                       
*                                                                            
*****************************************************************************
*                                                                            
* 	DESCRIPTION:  	Configuration Infrastructure Protocol Low level API's Include File	                                                             
*                                                                            
******************************************************************************/

#ifndef	__CIP_API_h
#define	__CIP_API_h

#include "cip_def.h"


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
int cip_begin_config(int SocketFd, struct sockaddr_un remote_addr);

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
int cip_begin_config_ack(int SocketFd, struct sockaddr_un remote_addr);

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
				int SocketFd, struct sockaddr_un remote_addr);

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
int cip_config_ack(int segment_number, int SocketFd, struct sockaddr_un remote_addr);

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
int cip_end_config(int SocketFd, struct sockaddr_un remote_addr);

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
int cip_end_config_ack(int SocketFd, struct sockaddr_un remote_addr);

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
int cip_get(CIP_STATUS_REQUESTS request, int SocketFd, struct sockaddr_un remote_addr);

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
						char *value, int SocketFd, struct sockaddr_un remote_addr);

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
int cip_get_n(CIP_STATUS_REQUESTS request, int channel, int SocketFd, struct sockaddr_un remote_addr);

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
					char *value, int SocketFd, struct sockaddr_un remote_addr);

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
					int SocketFd, struct sockaddr_un remote_addr);

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
					int SocketFd, struct sockaddr_un remote_addr);

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
			 char *value, int SocketFd, struct sockaddr_un remote_addr);

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
					 char *value, int SocketFd, struct sockaddr_un remote_addr);

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
int cip_send_general(char *pBuf, int SocketFd, struct sockaddr_un remote_addr);

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
int cip_error(CIP_ERROR_MESSAGES error_message, int SocketFd, struct sockaddr_un remote_addr);



#endif /* __CIP_API_h */
