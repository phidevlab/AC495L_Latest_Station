
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>  
#include <errno.h>
#include <sys/un.h>

#include "cip_def.h"
#include "cip_api.h"
#include "cfg_api.h"


#define MAX_CMD_LEN			20
#define MAX_DESC_LEN		150
#define MAX_SYNTAX_LEN		150
#define MAX_ARGS			10
#define EXIT_CODE			99
#define MAX_ARG_LEN			50

#define MAX_CMD_LINE_LEN	200


#if defined(P_PLATFORM)
#define 		APP_PATH	"/var/ftp"
#elif defined(B_PLATFORM)||defined(B_CEVA_PLATFORM)||defined(GROCX_PLATFORM)
#define 		APP_PATH	"/audiocodes/apps"
#elif defined(I_PLATFORM)
#define 		APP_PATH	"/var/tmp"
#elif defined(S_PLATFORM)
#define 		APP_PATH	"./"
#endif


#define 		DefaultCFGFileLocation 		APP_PATH"/VoIPCfgFile_Proxy_cvt_improved_appl.cfg"

#if defined(AC494_TULIP_FXSS)
#define 		ACL_MAIN_APPL_LOCATION	APP_PATH"/ac494main_tulip_ATA -s"
#elif defined(AC494_SDP_IPPHONE)
#define 		ACL_MAIN_APPL_LOCATION	APP_PATH"/ac494main_sdp_IPP -s"
#elif defined(AC494_SDP_ATA)
#define 		ACL_MAIN_APPL_LOCATION	APP_PATH"/ac494main_sdp_ATA -s"
#elif defined(AC49_SMB_VOIP_TOOLKIT)
#define 		ACL_MAIN_APPL_LOCATION	APP_PATH"/ac494main_SMB -s"
#elif defined(AC49_ORCHID_VOIP_TOOLKIT)
#define 		ACL_MAIN_APPL_LOCATION	APP_PATH"/ac494main_orchid -s"
#elif defined(AC49_ORCHID_ATA_VOIP_TOOLKIT)
#define 		ACL_MAIN_APPL_LOCATION	APP_PATH"/ac494main_orchid_ATA -s"
#elif defined(AC494_TULIP2_40)
#define			ACL_MAIN_APPL_LOCATION	APP_PATH"/ac494main_TULIP2_40 -s"
#elif defined(AC494_TULIP2_21)
#define			ACL_MAIN_APPL_LOCATION	APP_PATH"/ac494main_TULIP2_21 -s"
#else	/*AC48x CPE VoIP Toolkit Platorms */
#define 		ACL_MAIN_APPL_LOCATION	APP_PATH"/acl_main -s"
#endif 


#if defined(I_PLATFORM)
#define		ACL_FORK				vfork()
#else
#define		ACL_FORK				fork()
#endif




static int 				mng_acl_main_ManagerSocketFd;
struct sockaddr_un 		mng_acl_main_Manager_remote_Addr;

/*the configuration buffer that will hold the configuration data */
static char			configBuf[CONFIGURATION_BUFFER_MAX_LEN]; 
static char 			latestBuf[CONFIGURATION_BUFFER_MAX_LEN];

static int				isVoIPTaskRunning = 0;
static int				openConnection = 0;

int LoadFileToBuf(char* buf,size_t bufsize,char* fileName);

int open_cmd(int argc, char* argv[]);
int run_cmd(int argc, char* argv[]);
int config_cmd(int argc, char* argv[]);
int get_port_count_cmd(int argc, char* argv[]);
int set_port_status_cmd(int argc, char* argv[]);
int get_port_status_cmd(int argc, char* argv[]);
int config_single_param_cmd(int argc, char *argv[]);
int exit_cmd(int argc, char* argv[]);
int help_cmd(int argc, char* argv[]);

int ParseCmdLine(char *cmd_lin, int *ret_argc, char *ret_argv[]);
int ExecuteCommand(int argc, char *argv[]);
int RunCLI();


typedef struct
{
	char name[MAX_CMD_LEN];
	char description[MAX_DESC_LEN];
	char syntax[MAX_SYNTAX_LEN];
	int (*func)(int argc, char* argv[]);
} CmdList;


CmdList Commands[]={
	{"open", "open the control to the VoIP application.", "Usage: open", open_cmd},
	{"run", "Run the VoIP application.", "Usage: run [<*.cfg>]", run_cmd},
	{"config", "Reconfigure the VoIP application.", "Usage: config [<*.cfg>]", config_cmd},
	{"config_single_param", "Configure single parameter for the VoIP application.", "Usage: config_single_param <name=value>\nUsage: config_single_param (Without input --> Reconfigure the application)", config_single_param_cmd},
	{"get_port_count","Get number of existing ports.", "Usage: get_port_count", get_port_count_cmd},
	{"set_port_status","Set port status (lock/unlock).", "Usage: set_port_status <LineId (1 to number of lines)> <status (0-lock, 1-unlock)>", set_port_status_cmd},
	{"get_port_status","Get port status (lock/unlock).", "Usage: get_port_status <LineId (1 to number of lines)>", get_port_status_cmd},
	{"exit","Exit this sample application.", "Usage: exit", exit_cmd},
	{"help","Getting help on available commands.", "Usage: help", help_cmd},
	{"","","",NULL}
	};



int open_cmd(int argc, char* argv[])
{

	switch(argc)
	{
		case 1:
				break;

		default:
				printf("\nUsage: %s\n", argv[0]);
				return (-1);
				break;
	}

	if(openConnection) /*if connection if already open close the file descriptors */
	{
		close(mng_acl_main_ManagerSocketFd);
	}

	/* towards the acl_sip application */
	if(cfg_init_manager_socket(&mng_acl_main_ManagerSocketFd, 
					MNG_ACL_MAIN_MANAGER_SOCKET_NAME, 	MNG_ACL_MAIN_AGENT_SOCKET_NAME, 
					&mng_acl_main_Manager_remote_Addr) == -1)
	{
		printf("\ncfg_init_manager_socket() failure\n");
		return (-1);
	}

	openConnection = 1;

	printf("open connection\n");
	
	return 0;
	
}


int run_cmd(int argc, char* argv[])
{
	char				voip_appl_execution[256] = "";

	char				configFileName[256]="";

	char				*config_argv[MAX_ARGS+1];
	char				config_buf[] = "config";

	int				ret;
	pid_t			pid;
	


	if(!openConnection)
	{
		printf("connection was not open; use 'open' command'\n");
		return (-1);
	}

/*	if(isVoIPTaskRunning)
	{
		printf("VoIP task is already running; can't use 'run' command; for re-configuring use 'config'\n");
		return (-1);
	}
*/

	switch(argc)
	{
		case 1:
				/* use default location of the configuration file */
				strcpy(configFileName,DefaultCFGFileLocation);
				break;

		case 2:
				/* user defined location of the configuration file */
				if(strlen(argv[1]) < sizeof(configFileName))
					strcpy(configFileName, argv[1]);
				else
				{
			   	 	printf("String length of argv[1] %d is equal or bigger then sizeof configFileName %d\r\n",
								strlen(argv[1]), sizeof(configFileName));
					return (-1);
				}

				break;

		default:
				printf("\nUsage: %s [<cfg file>]\n", argv[0]);
				return (-1);
				break;
	}


	/****************************************/
	/********   execute the voip application   ********/
	/****************************************/

	sprintf(voip_appl_execution, "%s", ACL_MAIN_APPL_LOCATION);

#ifdef CIP_DEBUG
	printf("\nvoip_appl_execution command = %s\n", voip_appl_execution);
#endif	

	pid=ACL_FORK;

    	if(pid == 0){
		execl("/bin/sh", "sh", "-c", voip_appl_execution, NULL, (char *)0);
		_exit(127); /* execl error */
    	}
  	else if(pid < 0) {
		printf("ACL_FORK");
		return (-1);
	} 

	/* an unwanted process (pid) is created and can be killed */
/*	sleep(1);
	kill(pid, SIGKILL);*/


	/* wait for receiving WAITING_FOR_BUFFER response */
	if(cfg_wait_for_ready_response(WAITING_FOR_BUFFER, mng_acl_main_ManagerSocketFd) == -1)
		return (-1);

	isVoIPTaskRunning = 1;

	/* now that the VoIP task is running, configure it */
	config_argv[0] = config_buf;
	config_argv[1] = configFileName;

	ret = config_cmd(2, config_argv);

	if(ret<0)
	{
		printf("Command config_cmd returned with error code %d.\n", ret);
		return (-1);
	}

	return 0;
	
}


int config_cmd(int argc, char* argv[])
{
	char				configFileName[128];


	if(!openConnection)
	{
		printf("connection was not open; use 'open' command'\n");
		return (-1);
	}

/*	if(!isVoIPTaskRunning)
	{
		printf("VoIP task is not running; can't use 'config' command; to run the VoIP application use 'run'\n");
		return (-1);
	}
*/
	memset(configBuf, 0, sizeof(configBuf));
	memset(latestBuf, 0, sizeof(latestBuf));

	switch(argc)
	{
		case 1:
				/* use default location of the configuration file */
				strcpy(configFileName,DefaultCFGFileLocation);
				break;

		case 2:
				/* user defined location of the configuration file */
				if(strlen(argv[1]) < sizeof(configFileName))
					strcpy(configFileName, argv[1]);
				else
				{
			   	 	printf("String length of argv[1] %d is equal or bigger then sizeof configFileName %d\r\n",
								strlen(argv[1]), sizeof(configFileName));
					return (-1);
				}
				break;

		default:
				printf("\nUsage: %s [<cfg file>]\n", argv[0]);
				return (-1);
				break;
	}

	if(cfg_util_load_file(configBuf, sizeof(configBuf), configFileName) != 0)
	{
		printf("ERROR: Failed to read %s File\n", configFileName);
		return (-1);
	}

	strcpy(latestBuf, configBuf);

	if(cfg_set_config(configBuf, mng_acl_main_ManagerSocketFd, mng_acl_main_Manager_remote_Addr) == -1)
	{
		printf("\ncfg_set_config() failure\n");
	}

	/* wait for receiving CONFIGURATION_FINISHED response */
	if(cfg_wait_for_ready_response(CONFIGURATION_FINISHED, mng_acl_main_ManagerSocketFd) == -1)
		return (-1);

	printf("Configuration ended successfully\n");
	
	return 0;
	
}

int get_port_count_cmd(int argc, char* argv[])
{
	int				portCount;

	if(!openConnection)
	{
		printf("connection was not open; use 'open' command'\n");
		return (-1);
	}

/*	if(!isVoIPTaskRunning)
	{
		printf("VoIP task is not running; can't use 'get_port_count_cmd' command; to run the VoIP application use 'run'\n");
		return (-1);
	}
*/
	switch(argc)
	{
		case 1:
				break;

		default:
				printf("\nUsage: %s\n", argv[0]);
				return (-1);
				break;
	}


	if(cfg_get_port_count(&portCount, mng_acl_main_ManagerSocketFd, mng_acl_main_Manager_remote_Addr) == -1)
	{
		printf("\ncfg_get_port_count() failure\n");
		return (-1);
	}

	printf("Number of existing ports is: %d\n", portCount);

	return 0;
}

int set_port_status_cmd(int argc, char* argv[])
{
	int				channel;
	PORT_STATUS	portStatus;
	int				portCount;

	if(!openConnection)
	{
		printf("connection was not open; use 'open' command'\n");
		return (-1);
	}


/*	if(!isVoIPTaskRunning)
	{
		printf("VoIP task is not running; can't use 'set_port_status_cmd' command; to run the VoIP application use 'run'\n");
		return (-1);
	}
*/
	switch(argc)
	{
	
		case 3:
				channel = atoi(argv[1]);	
				portStatus = atoi(argv[2]);
				break;

		default:
				printf("\nUsage: %s <LineId (1 to number of lines)> <Status>\n", argv[0]);
				return (-1);
				break;
	}

	if( (portStatus != PORT_LOCK) && (portStatus != PORT_UNLOCK) ){
		printf("\nWrong input, status is illegal: %d\n", portStatus);
		return (-1);
	}

	if(cfg_get_port_count(&portCount, mng_acl_main_ManagerSocketFd, mng_acl_main_Manager_remote_Addr) == -1)
	{
		printf("\ncfg_get_port_count() failure\n");
		return (-1);
	}

	if( (channel < 1) || (channel > portCount) ){
		printf("\nWrong input, valid ports are: 1..%d , received port: %d\n",
					portCount, channel);
		return (-1);
	}

	/* The user refers to channel numbers from 1 to n,
	     DSP and SLIC modules refers to channel numbers from 0 to n-1 */
	if(cfg_set_port_status(channel - 1, portStatus, mng_acl_main_ManagerSocketFd, mng_acl_main_Manager_remote_Addr) == -1)
	{
		printf("\ncfg_set_port_status() failure\n");
		return (-1);
	}

	printf("Setting port status of channel %d to: %s\n", 
			channel, (portStatus==PORT_LOCK) ? "PORT_LOCK" : "PORT_UNLOCK");

	return 0;
	
}

int get_port_status_cmd(int argc, char* argv[])
{
	int					channel;
	int/*PORT_STATUS*/	portStatus = -1;
	int					portCount;

	if(!openConnection)
	{
		printf("connection was not open; use 'open' command'\n");
		return (-1);
	}

/*	if(!isVoIPTaskRunning)
	{
		printf("VoIP task is not running; can't use 'get_port_status_cmd' command; to run the VoIP application use 'run'\n");
		return (-1);
	}
*/
	switch(argc)
	{
	
		case 2:
				channel = atoi(argv[1]);
				break;

		default:
				printf("\nUsage: %s <LineId (1 to number of lines)>\n", argv[0]);
				return (-1);
				break;
	}


	if(cfg_get_port_count(&portCount, mng_acl_main_ManagerSocketFd, mng_acl_main_Manager_remote_Addr) == -1)
	{
		printf("\ncfg_get_port_count() failure\n");
		return (-1);
	}

	if( (channel < 1) || (channel > portCount) ){
		printf("\nWrong input, valid ports are: 1..%d , received port: %d\n",
					portCount, channel);
		return (-1);
	}


	/* The user refers to channel numbers from 1 to n,
		DSP and SLIC refers to channel numbers from 0 to n-1 */
	if(cfg_get_port_status((channel -1), &portStatus, mng_acl_main_ManagerSocketFd, mng_acl_main_Manager_remote_Addr) == -1)
	{
		printf("\ncfg_get_port_status() failure\n");
		return (-1);
	}

	if(portStatus == -1){
		printf("\nPort status was not received properly\n");
		return (-1);
	}
	
	printf("Port status of channel %d is: %s\n", 
			channel, (portStatus==PORT_LOCK) ? "PORT_LOCK" : "PORT_UNLOCK");

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

int config_single_param_cmd(int argc, char *argv[])
{
	char *str, *ptr;
	char *newParam, *oldParam;
	int  npLen, opLen, bufLen;
	
	if(!isVoIPTaskRunning)
	{
		printf("VoIP task is not running; can't use 'config_single_param_cmd' command; to run the VoIP application use 'run'\n");
		return (-1);
	}

	switch (argc){
		
		case 1:
			printf("Send Configuration to VoIP Application\n");
			if(cfg_set_config(configBuf, mng_acl_main_ManagerSocketFd, mng_acl_main_Manager_remote_Addr) == -1)
        		{
		                printf("\ncfg_set_config() failure\n");
		        }

		        /* wait for receiving CONFIGURATION_FINISHED response */
		        if(cfg_wait_for_ready_response(CONFIGURATION_FINISHED, mng_acl_main_ManagerSocketFd) == -1)
		                return (-1);
			printf("Configuration finished successfully\n");
			return 0;
			break;
	
		case 2:
			break;

		default:
	                printf("\nUsage: %s <name=value>\n", argv[0]);
			return -1;
			break;
	}

	if (strlen(argv[1]) >= (MAX_LEN-2)) {
		/* if input is to long */
		printf("Input is to long\n");
		return -1;
	}
	/* check if the requested parameter is already configured in the configBuf */

	memset(configBuf, 0, sizeof(configBuf));
	strcpy(configBuf, latestBuf);/* copy latestbuf to configBuf for process */

	newParam = (char *)malloc(MAX_LEN);
        if (!newParam){
                printf("not enough memory!\n");
                return -1;
        }
        memset(newParam, 0, MAX_LEN);
        sprintf(newParam, "%s\n", argv[1]);

	ptr = strtok(argv[1], "=");
	if (!ptr) return -1;
	str = strstr(configBuf, ptr);
	if (!str){
		printf("Line %s can't be found in the configBuf, please check it\n", argv[1]);
		return -1;
	}

	bufLen = strlen(configBuf);
	/* make ptr pointer to the rest buf */
	ptr = str;
	oldParam = get_next_line(&ptr);
	if (!oldParam){
		printf("In right way, you will never see this\n");
		free(newParam);
		return -1;
	}
	
	/* update the params in the configBuf */
	opLen = strlen(oldParam);
	npLen = strlen(newParam);

	if ((npLen - opLen) > (CONFIGURATION_BUFFER_MAX_LEN - bufLen - 1)){
		printf("CONFIGURATION_BUFFER_MAX_LEN is not enough, please increase it\n");
		free(newParam);
		return -1;
	}

	if (opLen == (npLen - 1)){/* have the same length */
		memcpy(oldParam, newParam, npLen);
	}
	else if (opLen > (npLen - 1)){/* need to reconstruction the configBuf */
		memcpy(oldParam, newParam, npLen);
		oldParam += npLen;
		strcpy(latestBuf, ptr);
		strcpy(oldParam, latestBuf);
	}
	else{/* reconstruction */
		strcpy(latestBuf, ptr);
		memcpy(oldParam, newParam, npLen);
		oldParam += npLen;
		strcpy(oldParam, latestBuf);
	}
	free(newParam);
	
	/* update the latestBuf */
	strcpy(latestBuf, configBuf);
	
	printf("Single parameter was configured successfully\n");
	
	return 0;
	
	
		
}

int exit_cmd(int argc, char* argv[])
{	
	printf("Exiting sample application\n");
	close(mng_acl_main_ManagerSocketFd);

	return EXIT_CODE;
}

int help_cmd(int argc, char* argv[])
{
	int 		i;
		
	switch(argc)
	{
	
		case 1:
				printf("\nThe following commands are available:\n\n");
				break;
		case 2:
				break;

		default:
				printf("\nUsage: %s [<command>]\n", argv[0]);
				return (-1);
				break;
	}

	for(i=0; strlen(Commands[i].name)>0 ; i++)
		if( (argc==1) || !strcmp(argv[1],Commands[i].name) )
			printf("%s - %s\n%s\n\n",Commands[i].name, Commands[i].description, Commands[i].syntax);

	return 0;
	
}


int main(int argc, char* argv[])
{

	if(argc==1)
		RunCLI();
/*	else
		ExecuteCommand(argc-1, argv+1);
*/
	return 0;
}


int RunCLI()
{
	char cmd_lin[200];
	int argc;
	char *argv[MAX_ARGS+1];
	int ret=0;

	memset(cmd_lin, 0, sizeof(cmd_lin));

	while(ret!=EXIT_CODE)
	{
		printf("VoIP >> ");

		fgets( cmd_lin, MAX_CMD_LINE_LEN, stdin );

		if( cmd_lin[ strlen(cmd_lin) - 1 ] == '\n' )
			cmd_lin[ strlen(cmd_lin) - 1 ] = 0; /* take the newline character off the filename */

		if((cmd_lin[0] == '\n')||(cmd_lin[0] == '\r')||
							(cmd_lin[0] == '\t')||(cmd_lin[0] == '\0'))
		{			
			continue;
		}		

		ParseCmdLine(cmd_lin, &argc, argv);
		ret = ExecuteCommand(argc, argv);
		if(ret<0)
		{
			printf("Command '%s' returned with error code %d.\n",argv[0],ret);
		}
	}
	
	return 0;
	
}

int ParseCmdLine(char *cmd_lin, int *ret_argc, char *ret_argv[])
{
	char seps[]=" \t";
	char **argv=ret_argv;
	char *tok;
	int argc=0;

	/* Parse the command line */

	*ret_argc=0;
	tok=strtok(cmd_lin,seps);
	if(!tok)
		return -1;

	while(tok)
	{
		argv[argc++]=tok;
		tok=strtok(NULL,seps);
	}

	*ret_argc=argc;
	return 0;
}

int ExecuteCommand(int argc, char *argv[])
{
	int i;

	for(i=0; strlen(Commands[i].name)>0 ; i++)
	{
		if(!strcmp(Commands[i].name,argv[0]))
			return Commands[i].func(argc,argv);
	}

	printf("Unknown command - %s\n",argv[0]);
	return -1;
		
}


int LoadFileToBuf(char* buf,size_t bufsize,char* fileName) {

	FILE* file;
	char line[512]="";
	unsigned cnt=0;

	memset(buf, 0, bufsize);

	if ((file = fopen(fileName, "r")) == NULL) {
		printf("ERROR: %s File Not Found\n",fileName);
		return (-1);
	}

	strcpy(buf,"");

	while (fgets(line,sizeof(line)-1,file) != NULL)
	{
		/* Skip lines starting with ';' */
		if((line[0]==';')||(line[0]=='#'))
			continue;
		if(cnt+strlen(line)<bufsize)
		{
			strcat(buf,line);
	        	cnt+=strlen(line);
		}
		else
		{
			fclose(file);
			return (-1);
		}
	}

	fclose(file);
	
	return 0;
}

/******************************************************************************/
