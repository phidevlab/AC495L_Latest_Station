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

#ifndef	__CFG_API_h
#define	__CFG_API_h

#include "cip_api.h"

/****************************************************************************
*
*	Name:		cfg_init_manager_socket
*----------------------------------------------------------------------------
*	Abstract:	 	initialization of the manager socket
*----------------------------------------------------------------------------
*	Input:		int 				 *SocketFd			-  socket file descriptor
*				char				 *my_sun_path     		-  my socket name
*				char				 *remote_sun_path     	-  remote socket name
*				struct sockaddr_un	 *remote_addr			-  remote address
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 in case of error
******************************************************************************/
int cfg_init_manager_socket(int *SocketFd, char *my_sun_path, char *remote_sun_path, 
								struct sockaddr_un *remote_addr);

/****************************************************************************
*
*	Name:		cfg_init_agent_socket
*----------------------------------------------------------------------------
*	Abstract:	 	initialization of the agent socket
*----------------------------------------------------------------------------
*	Input:		int 				  *SocketFd  			-  socket file descriptor
*				char				 *my_sun_path     		-  my socket name
*				char				 *remote_sun_path     	-  remote socket name
*				struct sockaddr_un	  *remote_addr		-  remote address
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 in case of error
******************************************************************************/
int cfg_init_agent_socket(int *SocketFd, char *my_sun_path, char *remote_sun_path, 
								struct sockaddr_un *remote_addr);

/****************************************************************************
*
*	Name:		cfg_set_config
*----------------------------------------------------------------------------
*	Abstract:	 	set configuration
*----------------------------------------------------------------------------
*	Input:		char				   *cfgBuf			- configuration buffer
*				int 				   SocketFd   		-  socket file descriptor
*				struct sockaddr_un	  remote_addr		-  remote address
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 in case of error
******************************************************************************/
int cfg_set_config(char *cfgBuf, int SocketFd, struct sockaddr_un remote_addr);

/****************************************************************************
*
*	Name:		cfg_get_port_count
*----------------------------------------------------------------------------
*	Abstract:	 	Get the number of exist VoIP ports
*----------------------------------------------------------------------------
*	Input:		int				   *portCount		- number of exist VoIP ports
*				int 				   SocketFd   		-  socket file descriptor
*				struct sockaddr_un	   remote_addr		-  remote address
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 in case of error
******************************************************************************/
int cfg_get_port_count( int *portCount, int SocketFd, struct sockaddr_un remote_addr);

/****************************************************************************
*
*	Name:		cfg_get_port_status
*----------------------------------------------------------------------------
*	Abstract:	 	Get port status (locked/unlocked)
*----------------------------------------------------------------------------
*	Input:		int				   portNumber		- port number
*				int				   *portStatus		- port status
*				int 				   SocketFd   		-  socket file descriptor
*				struct sockaddr_un	   remote_addr		-  remote address
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 in case of error
******************************************************************************/
int cfg_get_port_status(int portNumber,  int *portStatus, int SocketFd, struct sockaddr_un remote_addr);

/****************************************************************************
*
*	Name:		cfg_set_port_status
*----------------------------------------------------------------------------
*	Abstract:	 	Set port status (locked/unlocked)
*----------------------------------------------------------------------------
*	Input:		int				   portNumber		- port number
*				int 				   portStatus		- port status(0-lock, 1-unlock)
*				int 				   SocketFd   		-  socket file descriptor
*				struct sockaddr_un	   remote_addr		-  remote address
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 in case of error
******************************************************************************/
int cfg_set_port_status(int portNumber,  int portStatus, int SocketFd, struct sockaddr_un remote_addr);

/****************************************************************************
*
*	Name:		cfg_wait_for_ready_response
*----------------------------------------------------------------------------
*	Abstract:	 	waiting for response
*----------------------------------------------------------------------------
*	Input:		char				   *response		- the requested response for waiting
*				int 				   SocketFd   		-  socket file descriptor
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 		-1 in case of error
******************************************************************************/
int cfg_wait_for_ready_response(char *response, int SocketFd);


/****************************************************************************
*
*	Name:		cfg_util_load_file
*----------------------------------------------------------------------------
*	Abstract:	 	utility to load file to a local buffer
*----------------------------------------------------------------------------
*	Input:		char				   *fileName		- the file name from which the data should be read
*				int 				   bufsize   		- buffer maximum size
*----------------------------------------------------------------------------
*  	Output:		char				   *buf			- pointer to a buffer	
*----------------------------------------------------------------------------
*	Returns: 		-1 in case of error
******************************************************************************/
int cfg_util_load_file(char* buf, size_t bufsize, char* fileName);

#endif /* __CFG_API_h */
