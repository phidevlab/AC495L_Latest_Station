
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <signal.h> 
#include <sys/un.h>
#include <sys/wait.h>


#include "cip_def.h"
#include "cip_api.h"
#include "cfg_api.h"

TableEnt *Get_Table_Entry(CIP_OPCODES opcode);

/*the configuration buffer that will hold the configuration data */
extern char			receivedCfgBuf[CONFIGURATION_BUFFER_MAX_LEN]; 

int 					acl_main_Mng_AgentSocketFd;
struct sockaddr_un 		acl_main_Mng_Agent_Remote_Addr;

int					acl_main_Voip_Task_ManagerSocketFd;
struct sockaddr_un 		acl_main_Voip_Task_Manager_Remote_Addr;


/*#define  ACL_MAIN_DEBUG*/

#ifdef ACL_MAIN_DEBUG
#define DBG_PRINT(a1,a2...)		printf("[%s:%4d] " a1,__FUNCTION__, __LINE__, ##a2)
#else
#define DBG_PRINT(a1,a2...)
#endif

#define DBG_PRINT_ERR(a1,a2...) printf(RV_TRUE, "[%s:%4d] " a1,__FUNCTION__, __LINE__, ##a2)


#if defined(P_PLATFORM)
#define 	APP_PATH				"/var/ftp"
#elif defined(B_PLATFORM)||defined(B_CEVA_PLATFORM)||defined(GROCX_PLATFORM)
#define 	APP_PATH				"/audiocodes/apps"
#elif defined(I_PLATFORM)
#define 	APP_PATH				"/var/tmp"
#elif defined(S_PLATFORM)
#define 	APP_PATH				"/tmp"
#endif

#define 	VOIP_TASK_LOCATION	APP_PATH"/voip_task"

#if defined(I_PLATFORM)
#define	ACL_FORK				vfork()
#else
#define	ACL_FORK				fork()
#endif


int createVoipTask(pid_t *, int , int , char *);

int main(int argc, char *argv[])
{
	int				ret = 0;
   	unsigned char          buf[MAX_LEN];
	int				buf_len;

	struct  sockaddr_un from;       
	socklen_t			fromlen;

	int 				waitForCfgBuf=0;
	int				isFirstLoading = 1;
	int				isCfgFileAsInput=0;
	cfg_msg 			*pReceivedBuf;
	TableEnt			*pTableEntry;
	pid_t			voip_task_pid;


	fromlen = sizeof(struct sockaddr_un);
	memset(buf, 0, sizeof(buf));


	switch(argc)
	{

		case 2:
			if(!strcmp(argv[1],"-s")) {
				waitForCfgBuf = 1;
				isCfgFileAsInput = 0;
				printf("\nVoIP task is waiting for configuration\n");
			}
			else{
				printf("Usage: %s -c <*.cfg configuration file>  or -s \n", argv[0]);
				ret = -1;
			}
			break;

		case 3:
			if(!strcmp(argv[1],"-c")) {
				isCfgFileAsInput = 1;
				waitForCfgBuf = 0;
			}
			else{
				printf("Usage: %s -c <*.cfg configuration file> or -s \n", argv[0]);
				ret = -1;
			}
			break;

		default:
				printf("Usage: %s -c <*.cfg configuration file>  or -s \n", argv[0]);
				ret = -1;
				break;
	}


	if(ret != -1)
	{
		/**** towards the sample management application ****/	
		if(cfg_init_agent_socket(&acl_main_Mng_AgentSocketFd, MNG_ACL_MAIN_AGENT_SOCKET_NAME, 
				MNG_ACL_MAIN_MANAGER_SOCKET_NAME, &acl_main_Mng_Agent_Remote_Addr) == -1)
		{
			perror("\ncfg_init_agent_socket() failure\n");
			return (-1);
		}

		if(waitForCfgBuf)  /* waiting for configuration buffer */
		{
			/* sending WAITING_FOR_BUFFER for start receiving configuration buffer */
			if(cip_send_general(WAITING_FOR_BUFFER, acl_main_Mng_AgentSocketFd, 
										acl_main_Mng_Agent_Remote_Addr) == -1){
				perror("\ncip_send_general()");
				return (-1);
			}
		}

		while(waitForCfgBuf)  /* waiting for configuration buffer */
		{
		        if ((buf_len = recvfrom(acl_main_Mng_AgentSocketFd, buf, sizeof(buf), 0, 
									(struct sockaddr *)&from, &fromlen)) < 0){
  	 			perror("\nrecvfrom::main() acl_main");
				return (-1);
        		}

			buf[buf_len] = '\0';

			pReceivedBuf = (cfg_msg *)buf;
			
			if( (pReceivedBuf->opcode) > NUMBER_OF_OPCODES )
			{
				printf("\nUnrecognized opcode");
				return (-1);
			}

			if(pReceivedBuf->opcode == END_CONFIG) /* the configuration buffer 
												    transmittion is finished */
			{
				waitForCfgBuf = 0;
			}

			pTableEntry = (TableEnt *)Get_Table_Entry(pReceivedBuf->opcode);

			pTableEntry->opHandler(pReceivedBuf->info);

		}

		strcat(receivedCfgBuf, "\0");

		/* launching the voip_task */
		if(createVoipTask(&voip_task_pid, isFirstLoading, isCfgFileAsInput, argv[2]) == -1){
  	 		printf("\nlaunching voip_task failed");
			return (-1);
		}

		/*towards the voip_task */ 
		if(cfg_init_manager_socket(&acl_main_Voip_Task_ManagerSocketFd, ACL_MAIN_VOIP_TASK_MANAGER_SOCKET_NAME,
					ACL_MAIN_VOIP_TASK_AGENT_SOCKET_NAME, &acl_main_Voip_Task_Manager_Remote_Addr) == -1)
		{
			printf("\ncfg_init_manager_socket() failure\n");
		}

		/* the sip application wasn't executed with *.cfg file as input 
				and the voip task is waiting for configuration buffer */
		if(!strcmp(argv[1],"-s")) {

			/* wait for receiving WAITING_FOR_BUFFER response */
			if(cfg_wait_for_ready_response(WAITING_FOR_BUFFER, acl_main_Voip_Task_ManagerSocketFd) == -1)
				return (-1);
			if(cfg_set_config(receivedCfgBuf, acl_main_Voip_Task_ManagerSocketFd, acl_main_Voip_Task_Manager_Remote_Addr) == -1)
			{
				printf("\ncfg_set_config() failure\n");
				return (-1);
			}
		}

		isFirstLoading = 0; /* voip_task was already launched once */

		if(!strcmp(argv[1],"-s")) {

			/* sending CONFIGURATION_FINISHED and entering a forever loop */
			if(cip_send_general(CONFIGURATION_FINISHED, acl_main_Mng_AgentSocketFd, acl_main_Mng_Agent_Remote_Addr) == -1){
				perror("\ncip_send_general()");
				return (-1);
			}
		}
	
		/* waiting for re-configuration request or status commands/requests */
		for(;;)	
		{	
			memset(buf, 0, sizeof(buf));

			buf_len = recvfrom(acl_main_Mng_AgentSocketFd, buf, sizeof(buf), 0, 
								(struct sockaddr *)&from, &fromlen);

			buf[buf_len] = '\0';

			pReceivedBuf = (cfg_msg *)buf;

			if( (pReceivedBuf->opcode) > NUMBER_OF_OPCODES )
			{
				printf("\nUnrecognized opcode");
				return (-1);
			}

			pTableEntry = (TableEnt *)Get_Table_Entry(pReceivedBuf->opcode);

			pTableEntry->opHandler(pReceivedBuf->info);

			if(pReceivedBuf->opcode == END_CONFIG) /* the configuration buffer 
											  	    re-transmittion is finished */
			{

   				 /* Use SIGTERM instead of SIGKILL to kill the voip_task. we have 
   				     some work to do before dying. Also, kill only the main 
   				     thread (the others will follow it).	*/
#if defined(P_PLATFORM)||defined(I_PLATFORM)||defined(S_PLATFORM)||defined(GROCX_PLATFORM)
				kill(voip_task_pid+1, SIGTERM);
#elif defined(B_PLATFORM)||defined(B_CEVA_PLATFORM)
				kill(voip_task_pid, SIGTERM);
#endif

				printf("killed voip_task, waiting to collect it\n");
 				while (waitpid(voip_task_pid, NULL, 0) == -1);
  				printf("dead voip_task collected\n");

				/* launching the voip_task */
				if(createVoipTask(&voip_task_pid, isFirstLoading, 0, argv[2]) == -1){
  	 				printf("\nlaunching voip_task failed");
					return (-1);
				}

				/* wait for receiving WAITING_FOR_BUFFER response */
				if(cfg_wait_for_ready_response(WAITING_FOR_BUFFER, acl_main_Voip_Task_ManagerSocketFd) == -1)
					return (-1);

				if(cfg_set_config(receivedCfgBuf, acl_main_Voip_Task_ManagerSocketFd, acl_main_Voip_Task_Manager_Remote_Addr) == -1)
				{
					printf("\ncfg_set_config() failure\n");
					return (-1);
				}

				/* sending CONFIGURATION_FINISHED and entering a forever loop */
				if(cip_send_general(CONFIGURATION_FINISHED, acl_main_Mng_AgentSocketFd, acl_main_Mng_Agent_Remote_Addr) == -1){
					perror("\ncip_send_general()");
					return (-1);
				}

			}

		}		
		
	}	
	
	return ret;
	
} /* end of main() */


int createVoipTask(pid_t *voip_task_pid, int isFirstLoading, int isCfgFileAsInput, char *cfgFileLocation)
{
	int		retValue = 0;
	char		executeCommand[256];

	memset(executeCommand, 0, sizeof(executeCommand));

	if(isFirstLoading)	/*loading for the first time*/
		sprintf(executeCommand, "%s", VOIP_TASK_LOCATION);
	else  			/*re-loading*/
		sprintf(executeCommand, "%s -r", VOIP_TASK_LOCATION);

	if(isCfgFileAsInput)	/* the sip application was executed with *.cfg file as input */
	{		
		strcat(executeCommand, " ");

		if(!cfgFileLocation)
		{
			printf("\ncfgFileLocation string is empty\n");
			return (-1);
		}
		
		if((sizeof(executeCommand) - strlen(executeCommand)) > strlen(cfgFileLocation))
		{
			strcat(executeCommand, cfgFileLocation);
		}
		else
		{
			printf("\ncfgFileLocation string is to long %d\n", strlen(cfgFileLocation));
			return (-1);
		}
	}

#ifdef CIP_DEBUG
	printf("\nin acl_main_main::executeCommand = %s\n", executeCommand);
	/* executeCommand suppose to be: "voip_task <isFirstLoading> [<*.cfg>] &" */
#endif

	*voip_task_pid=ACL_FORK;

    if(*voip_task_pid == 0){
		retValue = execl("/bin/sh", "sh", "-c", executeCommand, NULL, (char *)0);
		_exit(127); /* execl error */
    	}
  	else if(*voip_task_pid < 0) {
		printf("ACL_FORK");
		retValue = -1;
	} 
	
	/* an unwanted process (voip_task_pid) is created and can be killed */
/*	sleep(1);
	kill(*voip_task_pid , SIGKILL);*/

	return retValue;
}

/******************************************************************************/
