
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

#include "acl_log.h"     

#include "acTimersAPI.h"

#include "acBufferPool.h"


int global=0;

void cb1(void *data);
void cb2(void *data);


int main()
{
	TimerUserParams 	myUserParam1, myUserParam2, myUserParam3, myUserParam4;
	int				myId1=0, myId2=0, myId3=0, myId4=0, myId5=0;
	unsigned long		num1=11, num2=22, num3=33, num4=44;

	myUserParam1.Type = TIMER_THROWAWAY;
	myUserParam1.Period = 2000;
	myUserParam1.ExpireFunction = cb1;
	myUserParam1.data = &num1;
	


	myUserParam2.Type = TIMER_THROWAWAY;
	myUserParam2.Period = 3000;
	myUserParam2.ExpireFunction = cb1;
	myUserParam2.data = &num2;

	myUserParam3.Type = TIMER_THROWAWAY;
	myUserParam3.Period = 4000;
	myUserParam3.ExpireFunction = cb2;
	myUserParam3.data = &num3;

	myUserParam4.Type = TIMER_THROWAWAY;
	myUserParam4.Period = 5000;
	myUserParam4.ExpireFunction = cb2;
	myUserParam4.data = &num4;

	acl_log_set_debug_level_param(1);


	
	if(AcTimerInit() <0 )
		acl_log(ACL_LOG_ERROR,"AcTimerInit");

	if(AcTimerDestroy() != TIMER_OK)
        	printf("\nAcTimerDestroy failed\r\n");
    	
	if(AcTimerInit() <0 )
		acl_log(ACL_LOG_ERROR,"AcTimerInit");

	if(AcTimerDestroy() != TIMER_OK)
        	printf("\nAcTimerDestroy failed\r\n");

	if(AcTimerInit() <0 )
		acl_log(ACL_LOG_ERROR,"AcTimerInit");

	if(AcTimerDestroy() != TIMER_OK)
        	printf("\nAcTimerDestroy failed\r\n");

	if(AcTimerInit() <0 )
		acl_log(ACL_LOG_ERROR,"AcTimerInit");

	if(AcTimerDestroy() != TIMER_OK)
        	printf("\nAcTimerDestroy failed\r\n");

	if(AcTimerInit() <0 )
		acl_log(ACL_LOG_ERROR,"AcTimerInit");

	if(AcTimerDestroy() != TIMER_OK)
        	printf("\nAcTimerDestroy failed\r\n");

	if(AcTimerInit() <0 )
		acl_log(ACL_LOG_ERROR,"AcTimerInit");




/*	if(AcDeleteTimer(myId4) != TIMER_OK)
        	printf("\nAcDeleteTimer failed   i = %d\r\n", myId4);
*/
	
	if(AcArmTimer(&myUserParam1, &myId1) != TIMER_OK)
        	printf("\nAcDeleteTimer failed   i = %d\r\n", myId1);

	if(AcArmTimer(&myUserParam3, &myId3) != TIMER_OK)
        	printf("\nAcDeleteTimer failed   i = %d\r\n", myId3);

	if(AcArmTimer(&myUserParam4, &myId4) != TIMER_OK)
        	printf("\nAcDeleteTimer failed   i = %d\r\n", myId4);
	
	if(AcArmTimer(&myUserParam2, &myId2) != TIMER_OK)
        	printf("\nAcDeleteTimer failed   i = %d\r\n", myId2);


        printf("\nmyId1 = %d  [%s:%d]\r\n",myId1, __FUNCTION__, __LINE__);
        printf("\nmyId2 = %d  [%s:%d]\r\n",myId2, __FUNCTION__, __LINE__);
        printf("\nmyId3 = %d  [%s:%d]\r\n",myId3, __FUNCTION__, __LINE__);
        printf("\nmyId4 = %d  [%s:%d]\r\n",myId4, __FUNCTION__, __LINE__);

	
	
	if(AcDeleteTimer(myId1) != TIMER_OK)
        	printf("\nAcDeleteTimer failed   i = %d\r\n", myId1);
	if(AcDeleteTimer(myId2) != TIMER_OK)
        	printf("\nAcDeleteTimer failed   i = %d\r\n", myId2);
	if(AcDeleteTimer(myId3) != TIMER_OK)
        	printf("\nAcDeleteTimer failed   i = %d\r\n", myId3);
	if(AcDeleteTimer(myId4) != TIMER_OK)
        	printf("\nAcDeleteTimer failed   i = %d\r\n", myId4);




	if(AcArmTimer(&myUserParam1, &myId1) != TIMER_OK)
        	printf("\nAcDeleteTimer failed   i = %d\r\n", myId1);

	if(AcArmTimer(&myUserParam3, &myId3) != TIMER_OK)
        	printf("\nAcDeleteTimer failed   i = %d\r\n", myId3);

	if(AcArmTimer(&myUserParam4, &myId4) != TIMER_OK)
        	printf("\nAcDeleteTimer failed   i = %d\r\n", myId4);
	
	if(AcArmTimer(&myUserParam2, &myId2) != TIMER_OK)
        	printf("\nAcDeleteTimer failed   i = %d\r\n", myId2);


        printf("\nmyId1 = %d  [%s:%d]\r\n",myId1, __FUNCTION__, __LINE__);
        printf("\nmyId2 = %d  [%s:%d]\r\n",myId2, __FUNCTION__, __LINE__);
        printf("\nmyId3 = %d  [%s:%d]\r\n",myId3, __FUNCTION__, __LINE__);
        printf("\nmyId4 = %d  [%s:%d]\r\n",myId4, __FUNCTION__, __LINE__);
		
	pause(); /* We don't want the process to be terminated */

	return 0;
}

void cb1(void *data)
{
	global++;

        printf("\n\n\ncall back function 1 !!!!!!!    [%s:%d]   data=%d \r\n\n",__FUNCTION__, __LINE__, (*(int *)data));
	printf("global = %d\n\n\n", global);
}

void cb2(void *data)
{
	global++;
	
        printf("\n\n\ncall back function 2 !!!!!!!    [%s:%d]   data=%d \r\n\n",__FUNCTION__, __LINE__, (*(int *)data));
	printf("global = %d\n\n\n", global);
}

