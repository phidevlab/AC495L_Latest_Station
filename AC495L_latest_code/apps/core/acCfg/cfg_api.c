/****************************************************************************
*
*   Company:			Audiocodes Ltd.
*
*   Project:
*
*   Hardware Module:
*
*   File Name: 			cfg_api.c
*
*   Creation Date:    	13.2.07
*
*   Last Update By:
*
******************************************************************************
*
* 	DESCRIPTION:		Configuration Infrastructure Protocol High level API's Source File
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

#include "cfg_api.h"

/*#define  ACL_DEBUG*/

#ifdef ACL_DEBUG
#define ACL_LogDebug printf
#else
#define ACL_LogDebug
#endif

int					remoteSocketFd;
struct sockaddr_un		remoteAddr;

int cfg_init_socket(int *SocketFd, char *my_sun_path, char *remote_sun_path,
								struct sockaddr_un *remote_addr);

char* GetNextLine(char* buf,char** p);
char* StrFindFirstOf(char* str, char* delimiters);
char* StrFindFirstNotOf(char* str, char* delimiters);

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
								struct sockaddr_un *remote_addr)
{

	if(cfg_init_socket(SocketFd, my_sun_path, remote_sun_path, remote_addr) < 0)
	{
	   	printf("cfg_init_socket failure\n");
	 	return -1;
	}

	return 0;
}


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
								struct sockaddr_un *remote_addr)
{
	if(cfg_init_socket(SocketFd, my_sun_path, remote_sun_path, remote_addr) < 0)
	{
	   	printf("cfg_init_socket failure\n");
	 	return -1;
	}

	remoteSocketFd = *SocketFd;

	memcpy(&remoteAddr, remote_addr, sizeof(struct sockaddr_un));

	return 0;

}


static char* get_next_line(char** p)
{
	char *result = NULL;

	result = strtok( *p, "\n" );

	if( result == NULL )
	{
		return NULL;
	}
	else
	{
		*p +=(strlen(result) + strlen("\n"));
	}

	return result;

}

/****************************************************************************
*
*	Name:		cfg_set_config
*----------------------------------------------------------------------------
*	Abstract:	 	set configuration
*----------------------------------------------------------------------------
*	Input:		char				   *cfgBuf			- configuration buffer
*				int 				   SocketFd   		-  socket file descriptor
*				struct sockaddr_un	  remote_addr	-  remote address
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	-1 in case of error
******************************************************************************/
int cfg_set_config(char *cfgBuf, int SocketFd, struct sockaddr_un remote_addr)
{
	struct  sockaddr_un 			from;
	socklen_t						fromlen;

   	char             					buf[MAX_LEN];
	int							buf_len;
	cfg_msg						*pCfg_msg = NULL;
	segment_msg_st				*pSegment_msg = NULL;
	char   						*retString = NULL;
	char 							*ptr = NULL;

	int 							segment_number=0;
	char 							line[MAX_LEN];

	fromlen = sizeof(struct sockaddr_un);
	memset(buf, 0, sizeof(buf));
	memset(line, 0, sizeof(line));

	if(cip_begin_config(SocketFd, remote_addr) == -1){
		printf("\ncip_begin_config()");
		return (-1);
	}
	if ((buf_len = recvfrom(SocketFd, buf, sizeof(buf), 0, (struct sockaddr *)&from, &fromlen)) < 0){
  	 	perror("\nrecvfrom::cfg_set_config()");
		return (-1);
        }
	buf[buf_len] = '\0';

	pCfg_msg = (cfg_msg*)buf;

	if(pCfg_msg->opcode != BEGIN_CONFIG_ACK)
	{
		printf("\ncfg_set_config::opcode expected is BEGIN_CONFIG_ACK, received: %d",
					pCfg_msg->opcode);
		return (-1);
	}

	if(cfgBuf !=NULL)
	{
		ptr = cfgBuf;
	}
	else
	{
		printf("illegal configuration file - NULL\r\n");
		return (-1);
	}

	/* get line by line from the configuration buffer and send it to the remote */
	while((retString = get_next_line(&ptr))!=NULL)
	{
		if(strlen(retString) < sizeof(line))
	        	strcpy(line, retString);

#ifdef CIP_DEBUG
		printf("\nsending line = %s  sizeof(line) = %d\n", &(line[0]), strlen(line));
#endif
		if(cip_config(segment_number, &(line[0]), strlen(line)/*size*/, SocketFd, remote_addr) == -1){
			printf("\ncip_config()");
			return (-1);
		}

        	if ((buf_len = recvfrom(SocketFd, buf, sizeof(buf), 0, (struct sockaddr *)&from, &fromlen)) < 0){
  	 		perror("\nrecvfrom::cfg_get_port_count()");
			return (-1);
      		}

		buf[buf_len] = '\0';

		pCfg_msg = (cfg_msg*)buf;
		if(pCfg_msg->opcode == CONFIG_ACK)
		{
			pSegment_msg = (segment_msg_st*)pCfg_msg->info;

			if(pSegment_msg->segment != segment_number){
  	 			printf("\ncfg_set_config::wrong segment number in the received acknowledge message");
				return (-1);
			}
		}
		else{
  	 		printf("\ncfg_set_config::opcode expected is CONFIG_ACK, received %d",
						pCfg_msg->opcode);
			return (-1);
		}
		segment_number++;

	}

	/* all the configuration buffer has beed sent to the remote */
	if(cip_end_config(SocketFd, remote_addr) == -1){
		printf("\ncip_end_config()");
		return (-1);
	}

        if ((buf_len = recvfrom(SocketFd, buf, sizeof(buf), 0, (struct sockaddr *)&from, &fromlen)) < 0){
  	 		perror("\nrecvfrom::cfg_set_config()");
			return (-1);
        }

	buf[buf_len] = '\0';

	pCfg_msg = (cfg_msg*)buf;
	if(pCfg_msg->opcode != END_CONFIG_ACK)
	{
		printf("\ncfg_set_config::opcode expected is END_CONFIG_ACK, received: %d",
					pCfg_msg->opcode);
		return (-1);
	}

	return 0;

}
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
int cfg_get_port_count( int *portCount, int SocketFd, struct sockaddr_un remote_addr)
{

	struct  sockaddr_un 			from;
	socklen_t						fromlen;

   	char             					buf[MAX_LEN];
	int							buf_len;
	cfg_msg						*pCfg_msg;
	statusRequest_value_msg_st		*pStatusRequest_value_msg;


	fromlen = sizeof(struct sockaddr_un);
	memset(buf, 0, sizeof(buf));


	if(cip_get(GET_PORT_COUNT, SocketFd, remote_addr) == -1){
		printf("cip_get()::cfg_get_port_count");
		return (-1);
	}

        if ((buf_len = recvfrom(SocketFd, buf, sizeof(buf), 0, (struct sockaddr *)&from, &fromlen)) < 0){
  	 	perror("\nrecvfrom::cfg_get_port_count()");
		return (-1);
        }

	buf[buf_len] = '\0';

	pCfg_msg = (cfg_msg*)buf;

	if(pCfg_msg->opcode == GET_RESPONSE_INTEGER)
	{
		pStatusRequest_value_msg = (statusRequest_value_msg_st*)pCfg_msg->info;

		if(pStatusRequest_value_msg->statusRequest == GET_PORT_COUNT){

			if(*(int *)&pStatusRequest_value_msg->value[0] >= 0){
				*portCount = *(int *)&pStatusRequest_value_msg->value[0];
				return 0;
			}
			else	{
				printf("\nThe number of port exist can't be less then 0, received: %d",
						*(int *)&pStatusRequest_value_msg->value[0]);
				return (-1);
			}
		}
		else{
			printf("\nThe requested status was for GET_PORT_COUNT, response was received for status request no. %d",
						pStatusRequest_value_msg->statusRequest);
			return (-1);
		}
	}
	else{
		printf("\nThe opcode which suppose to be received is GET_RESPONSE_INTEGER , opcode received is %d",
					pCfg_msg->opcode);
		return (-1);
	}

	return 0;

}
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
int cfg_get_port_status(int portNumber,  int *portStatus, 	int SocketFd, struct sockaddr_un remote_addr)
{
	struct  sockaddr_un 					from;
	socklen_t								fromlen;

   	char             							buf[MAX_LEN];
	int									buf_len;
	cfg_msg								*pCfg_msg;
	statusRequest_channel_value_msg_st		*pStatusRequest_channel_value_msg;


	fromlen = sizeof(struct sockaddr_un);
	memset(buf, 0, sizeof(buf));

	if(cip_get_n(GET_PORT_STATUS_N, portNumber, SocketFd, remote_addr) == -1){
		printf("\ncip_get_n::cfg_get_port_status()");
		return (-1);
	}

        if ((buf_len = recvfrom(SocketFd, buf, sizeof(buf), 0, (struct sockaddr *)&from, &fromlen)) < 0){
  	 	perror("\nrecvfrom::cfg_get_port_status()");
		return (-1);
        }

	buf[buf_len] = '\0';

	pCfg_msg = (cfg_msg*)buf;

	if(pCfg_msg->opcode == GET_N_RESPONSE_INTEGER)
	{
		pStatusRequest_channel_value_msg = (statusRequest_channel_value_msg_st*)pCfg_msg->info;

		if(pStatusRequest_channel_value_msg->statusRequest == GET_PORT_STATUS_N){

			if(pStatusRequest_channel_value_msg->channel == portNumber){

				if(((*(int *)&pStatusRequest_channel_value_msg->value[0]) >= 0)&&
					((*(int *)&pStatusRequest_channel_value_msg->value[0]) <= 1))
				{
					*portStatus = *(int *)&pStatusRequest_channel_value_msg->value[0];
					return 0;
				}
				else{
					printf("\nPort status must be between 0 to 1, received: %d",
								*(int *)&pStatusRequest_channel_value_msg->value[0]);
					return (-1);
				}
			}
			else{
				printf("\nThe requested port status was for channel %d, response was received for channel: %d",
							portNumber, pStatusRequest_channel_value_msg->channel);
				return (-1);
			}
		}
		else{
			printf("\nThe requested status was for GET_PORT_STATUS_N, port number %d, response was received for status request no. %d",
						portNumber, pStatusRequest_channel_value_msg->statusRequest);
			return (-1);
		}
	}
	else{
		printf("\nThe opcode which suppose to be received is GET_N_RESPONSE_INTEGER , opcode received is %d",
					pCfg_msg->opcode);
		return (-1);
	}

	return 0;

}
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
int cfg_set_port_status(int portNumber,  int portStatus, int SocketFd, struct sockaddr_un remote_addr)
{

	struct  sockaddr_un 					from;
	socklen_t								fromlen;

   	char             							buf[MAX_LEN];
	int									buf_len;
	cfg_msg								*pCfg_msg;
	statusCommand_channel_value_msg_st	*pStatusCommand_channel_value_msg;

	fromlen = sizeof(struct sockaddr_un);
	memset(buf, 0, sizeof(buf));

	if(cip_set_n(SET_N_INTEGER, SET_PORT_STATUS_N, portNumber, (char *)&portStatus,
					SocketFd, remote_addr) == -1){
		printf("\ncip_set_n::cfg_set_port_status()");
		return (-1);
	}

        if ((buf_len = recvfrom(SocketFd, buf, sizeof(buf), 0, (struct sockaddr *)&from, &fromlen)) < 0){
  	 	perror("\nrecvfrom::cfg_set_port_status()");
		return (-1);
        }

	buf[buf_len] = '\0';

	pCfg_msg = (cfg_msg*)buf;

	if(pCfg_msg->opcode == SET_N_RESPONSE_INTEGER)
	{
		pStatusCommand_channel_value_msg = (statusCommand_channel_value_msg_st*)pCfg_msg->info;

		if(pStatusCommand_channel_value_msg->statusCommand== SET_PORT_STATUS_N){

			if(pStatusCommand_channel_value_msg->channel == portNumber){

				if(((*(int *)&pStatusCommand_channel_value_msg->value[0]) >= 0)&&
					((*(int *)&pStatusCommand_channel_value_msg->value[0]) <= 1))
				{
					return 0;
				}
				else{
					printf("\nPort status must be between 0 to 1, received: %d",
								*(int *)&pStatusCommand_channel_value_msg->value[0]);
					return (-1);
				}
			}
			else{
				printf("\nThe requested port status was for channel %d, response was received for channel: %d",
							portNumber, pStatusCommand_channel_value_msg->channel);
				return (-1);
			}
		}
		else{
			printf("\nThe status coMmand was for SET_PORT_STATUS_N, port number %d, response was received for status command no. %d",
						portNumber, pStatusCommand_channel_value_msg->statusCommand);
			return (-1);
		}
	}
	else{
		printf("\nThe opcode which suppose to be received is SET_N_RESPONSE_INTEGER , opcode received is %d",
					pCfg_msg->opcode);
		return (-1);
	}

	return 0;

}

/****************************************************************************
*
*	Name:		cfg_wait_for_ready_response
*----------------------------------------------------------------------------
*	Abstract:	 	waiting for response
*----------------------------------------------------------------------------
*	Input:		char				   *response		- the requested response for waiting
*				int 				   SocketFd   		- socket file descriptor
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 		-1 in case of error
******************************************************************************/
int cfg_wait_for_ready_response(char *response, int SocketFd)
{
	struct  sockaddr_un 	from;
	socklen_t				fromlen;

	char					buf[MAX_LEN];
	int					buf_len;

	fromlen = sizeof(struct sockaddr_un);
	memset(buf, 0, sizeof(buf));

	/* wait for receiving response */
        if ((buf_len = recvfrom(SocketFd, buf, sizeof(buf), 0, (struct sockaddr *)&from, &fromlen)) < 0){
  			perror("cfg_wait_for_ready_response::recvfrom");
			return (-1);
        }

	buf[buf_len] = '\0';

	if(strcmp(buf, response)) 	/* wrong response was received */
	{
		printf("\ncfg_wait_for_ready_response::received wrong response : '%s', expected: %s\n", buf, response);
		return (-1);
	}

#ifdef CIP_DEBUG
	printf("\nin cfg_wait_for_ready_response::response = %s\n", response);
#endif

	return 0;

}





/**************************************/
/*********  Utility Functions  ***************/
/**************************************/


/***************************************************************************************
 * GetNextLine
 * -------------------------------------------------------------------------------------
 * General:   Goes through the received buffer, and returns the beginning of the next line,
		     while skipping empty lines.
 * Return Value: Returns a pointer to the beggining of the next line
 * -------------------------------------------------------------------------------------
 * Arguments:
 * Input: 	 string - the buffer to be parsed
 * Output:   	 string pointer - pointer to the beggining of the next line
 **************************************************************************************/
char* GetNextLine(char* buf,char** p)
{
	if(*p==NULL) {
		*p = buf;
		return buf;
	}

	/* Find end of line */
	*p = (char*)StrFindFirstOf(*p,"\n");
	if(**p=='\0')
		return NULL;
	/* Skip blank lines */
	*p = (char*)StrFindFirstNotOf(*p,"\n ");
	if(**p=='\0')
		return NULL;

	return *p;
}

char* StrFindFirstOf(char* str, char* delimiters) {
    for (; *str != '\0' && strchr(delimiters, *str) == NULL; ++str)
        ;
    return str;
}

char* StrFindFirstNotOf(char* str, char* delimiters) {
    for (; *str != '\0' && strchr(delimiters, *str) != NULL; ++str)
        ;
    return str;
}

/***************************************************************************************
 * GetCurrentLine
 * -------------------------------------------------------------------------------------
 * General:   Goes through the received buffer, and copy it to a new buffer until "\r" or "\n"
 		     appears and switch them to '\0"
 * Return Value: Returns the size of the copied buffer
 * -------------------------------------------------------------------------------------
 * Arguments:
 * Input: 	 string - the buffer to be parsed
 * Output:   	 string pointer - pointer to the buffer to be filled
 **************************************************************************************/
int GetCurrentLine(char* receivedBuf, char* newBuf)
{
	int index=0;

	for(index=0; (*receivedBuf != ';')&&(*receivedBuf != '\r')&&(*receivedBuf != '\n')
					/*&&(*receivedBuf != '#')*/	; receivedBuf++, index++)
	{
		*newBuf = *receivedBuf;
		newBuf++;
	}

	*newBuf = '\0';

	return index;
}


/***************************************************************************************
 * cfg_util_load_file
 * -------------------------------------------------------------------------------------
 * General:   Opens a file and copies the content of the file to a buffer.
 * Return Value: True is succeeded, False if failed
 * -------------------------------------------------------------------------------------
 * Arguments:
 * Input: 	 	string - the buffer
 *		 	integer - buffer length
 *
 * Output:   	string - file name
 **************************************************************************************/
int cfg_util_load_file(char* buf, size_t bufsize, char* fileName)
{
	FILE* file;

	char *pLine;
//	char line[MAX_LEN]="";
	unsigned int cnt=0;
	printf("^^^^fileName:%s\n",fileName);///anuja

	if ((file = fopen(fileName, "r")) == NULL) {
		printf("ERROR: %s File Not Found\n",fileName);
		return (-1);
	}

	strcpy(buf,"");

	pLine = malloc(MAX_LEN);
	if(!pLine)
	{
		fclose(file);
		return(-1);
	}
	memset(pLine , 0, sizeof(MAX_LEN));

	while (fgets(pLine, MAX_LEN-1, file) != NULL)
	{
		/* Skip lines starting with ';' */
		if(pLine[0]==';')
			continue;
		if(cnt+strlen(pLine)<bufsize)
		{
			strcat(buf, pLine);
	        	cnt+=strlen(pLine);
		}
		else
		{
			printf("\n*** Buffer size is not large enough %d - It must be increased ***\n", CONFIGURATION_BUFFER_MAX_LEN);

			if(pLine)
				free(pLine);

			fclose(file);
			return (-1);
		}
	}

	if(pLine)
		free(pLine);

	fclose(file);

	return 0;

}


int cfg_init_socket(int *SocketFd, char *my_sun_path, char *remote_sun_path,
								struct sockaddr_un *remote_addr)
{
 	socklen_t 			myLen;
	struct sockaddr_un		my_addr;


 	/*******************************************************/
  	/* Create unix domain Socket (For Configuration Infrastucture )*/
   	/*******************************************************/


  	 if ((*SocketFd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0)
   	{
   	 	printf("[%s:%d] Creating UNIX Domain socket \r\n",__FUNCTION__, __LINE__);
    		return (-1);
  	}

  	memset((char *)&my_addr,0, sizeof(my_addr));
	my_addr.sun_family = AF_UNIX;

	if(strlen(my_sun_path) < sizeof(my_addr.sun_path))
		strcpy(my_addr.sun_path, my_sun_path);
	else
	{
   	 	printf("[%s:%d] String length of my_sun_path %d is equal or bigger then sizeof sun_path %d\r\n",
					__FUNCTION__, __LINE__, strlen(my_sun_path), sizeof(my_addr.sun_path));
    		return (-1);
	}
	unlink(my_addr.sun_path);

	myLen = strlen(my_addr.sun_path) + sizeof(my_addr.sun_family);

	if(bind(*SocketFd, (struct sockaddr *)&my_addr, myLen) < 0)
	{
	    	perror("\nbind");
	   	printf("[%s:%d] binding socket UNIX Domain socket \r\n",__FUNCTION__, __LINE__);
	 	return -1;
	}

#ifdef CIP_DEBUG
		printf("\ncfg_init_socket::SocketFd = %d  name = %s\n", *SocketFd, my_addr.sun_path);
#endif


  	memset((char *)remote_addr,0, sizeof(remote_addr));
	remote_addr->sun_family = AF_UNIX;

	if(strlen(remote_sun_path) < sizeof(remote_addr->sun_path))
		strcpy(remote_addr->sun_path, remote_sun_path);
	else
	{
   	 	printf("[%s:%d] String length of remote_sun_path %d is equal or bigger then sizeof sun_path %d\r\n",
					__FUNCTION__, __LINE__, strlen(remote_sun_path), sizeof(remote_addr->sun_path));
    		return (-1);
	}

	return 0;

}

