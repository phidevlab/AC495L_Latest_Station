
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <signal.h> 
#include <sys/time.h>
#include <pthread.h> 

#include <errno.h>

#include <time.h>
#include <semaphore.h>

#include <sys/stat.h>  

#include <sched.h> 

#include "acl_log.h"     

#include "acTimersAPI.h"

#include "acBufferPool.h"
#include "acBufferList.h"


/*#define  ACL_DEBUG*/

#ifdef ACL_DEBUG
#define ACL_LogDebug printf
#else
#define ACL_LogDebug
#endif


// Static varibles
static pthread_t     		gTimerTaskID;
static unsigned long 	gNumTimers;
static unsigned int  		gTimerPoolId;
static unsigned int  		gTimerListId;
static sem_t		   	gTimerListSema4;
static int           		g_clientSocketFd;
static int           		g_serverSocketFd;
static int           		isAlreadyInitialized = 0;


//#define ACT_MAX_EVENT_MSG_LEN  128 // TBD 
#define FALSE 0
#define TRUE  1

#undef NULL
#define NULL  0

typedef struct
{
   int  event;
   //char data[ACT_MAX_EVENT_MSG_LEN];
} ACT_MSG_STRCT;


typedef enum {
   ACT_UNKNOWN_MSG=0,
   ACT_NEW_TIMER_MSG,   
} ACT_MSG_ID;


typedef enum {
   ACT_GEN_ERROR = -100,
   ACT_TIMEOUT_ERR,
   ACT_INCOMPLETE_MSG_ERR,
   ACT_SUCCESS = 0,
} ACT_ERROR_CODE;


void initClient();
void initServer();

static int ConfigTimer(TimerRecord *Timer, TimerUserParams *pUserTimerConfig);
static void sighand(int signo);

/*--------------------------------------------------------------------*/
/*--------------------------------------------------------------------*/
//ACL NB 070311
unsigned long aclGetCurrTime()
{
   struct timeval myTimeval;
 
   // TBD per platform
   gettimeofday( &myTimeval, NULL);

   return((myTimeval.tv_sec*1000) + (myTimeval.tv_usec / 1000));
}
/*--------------------------------------------------------------------*/
/*--------------------------------------------------------------------*/
//ACL NB 070328
int ActSendNewTimerMsg()
{
   int 			numByteSent=0;
   ACT_MSG_STRCT 	timerMsg;
   int 			msgLen;

   msgLen = sizeof(timerMsg);
   timerMsg.event = ACT_NEW_TIMER_MSG;

   numByteSent = send(g_clientSocketFd, &timerMsg, msgLen, 0);

   if (numByteSent != msgLen)
 	printf("[%s:%d] send failed. num of byte sent = %d \r\n",__FUNCTION__, __LINE__,numByteSent);

   return(numByteSent);
}


/*--------------------------------------------------------------------*/
/*--------------------------------------------------------------------*/
//ACL NB 070328
int  ActReceiveMsg(ACT_MSG_STRCT *pEventMsg, int msgLen, int timeoutMS )
{
   fd_set 			readSet;
   int    			numBytesRead=0;
   int    			numBytesToRead;
   int    			readResult;
   struct timeval 	tv;
   int    			selectRes;
   int    			ret = ACT_SUCCESS;


   if (timeoutMS> 0)
   {
	   tv.tv_sec  = timeoutMS / 1000;
	   tv.tv_usec = (timeoutMS % 1000) * 1000;
   }
   while (1)
   {
      	FD_ZERO(&readSet);
      	FD_SET(g_serverSocketFd, &readSet);

      	selectRes = select(g_serverSocketFd+1, &readSet, NULL, NULL, timeoutMS>0 ? &tv: NULL);

	if (selectRes < 0)
      	{
	         /* Select failed */
	         ret = ACT_GEN_ERROR;
	      		printf("[%s:%d] Select failed res=%d\r\n",__FUNCTION__, __LINE__,selectRes );
	         break;
      	}
      	// time out
      	if (selectRes == 0)
      	{
        	if (numBytesRead == 0)
	        {
	            	ret = ACT_TIMEOUT_ERR;
	        }
	        else
	        {
	             	printf("[%s:%d] incomplete message \r\n",__FUNCTION__, __LINE__);
	            	ret = ACT_GEN_ERROR;
	        }
	        break;
      	}

      	numBytesToRead = msgLen - numBytesRead;
      
      	readResult = read(g_serverSocketFd, pEventMsg+numBytesRead, numBytesToRead);
      	if ( readResult <= 0)
      	{
		printf("[%s:%d] Read error sock=%d  Error opening file: %s\n" ,__FUNCTION__, __LINE__,g_serverSocketFd, strerror( errno ) );
         	ret = ACT_GEN_ERROR;
         	break;
      	}

      	numBytesRead += readResult;

      	if (msgLen >= numBytesRead)
      	{
         	ret = ACT_SUCCESS;
         	break;
      	}
      	else if (msgLen < numBytesRead)
      	{
         	printf("[%s:%d] numBytesRead=%d Msg Too Big\r\n",__FUNCTION__, __LINE__,numBytesRead);
         	break;
      	}	
   }

   return ret;
}

/*--------------------------------------------------------------------*/
/*--------------------------------------------------------------------*/
int InsertTimerToList(TimerRecord *Timer, unsigned long TaskLevel, int *pTid)
{
   int            		sizeofTimer;
   unsigned long  	CurrentTime;
   TimerRecord 		*pNextTimer = NULL, *pPreviousTimer =NULL ;
   int 			rc = TIMER_OK;
   int 			found = 0;


   if(gNumTimers == ACL_MAX_NUMBER_OF_TIMERS)
   {
	return TIMER_ERROR_NO_FREE;
   }

   gNumTimers++;
   sizeofTimer = sizeof(TimerRecord);

   // calculate the expire time
   CurrentTime = aclGetCurrTime(&CurrentTime);
   Timer->ExpireTime = CurrentTime + Timer->UserParams.Period;

   // insert at the right place of the list -   
   pNextTimer = (TimerRecord *)acBufferListGetFirst(gTimerListId);
   while (pNextTimer && !found )
   {
         if ( Timer->ExpireTime > pNextTimer->ExpireTime )
	 {
	       	pPreviousTimer = pNextTimer;
		pNextTimer = (TimerRecord *)acBufferListGetNext(gTimerListId, pPreviousTimer);
	 }
	 else
	  {	         	
            	// we find the right place so we need to insert it after the previous timer, if previous is NULL add first */
    		rc = (int)acBufferListInsertAfter(gTimerListId, pPreviousTimer, Timer, sizeofTimer);
		*pTid = rc;

            	found = 1;
	  }
   }
      
   // insert at the end 
   if ( !found )
   {		
	rc = (int)acBufferListAddLast(gTimerListId, Timer, sizeofTimer);		
	*pTid = rc;	
    }

   if ( rc == 0 )
   	rc = TIMER_ERROR_GENERAL_ERR;
   else
      	rc = TIMER_OK;
   
   return rc;
}

/*--------------------------------------------------------------------*/
/*--------------------------------------------------------------------*/
int RemoveTimerFromList(TimerRecord *Timer, unsigned long TaskLevel)
{
   int 		rc = TIMER_OK;
   
   // list is empty
   if (gTimerListId == NULL)
   {
   	rc = TIMER_ERROR_STOP_FREE_TIMER;
   }
   else
   {
	 rc = acBufferListRemove(gTimerListId,(void *)Timer );
   }
      
   if (rc == AC_STDLIB_OK)
   {
      	gNumTimers--;
   }
   else
   {
   	printf("[%s:%d]  acBufferListRemove failed.  gTimerListId = %d\r\n",__FUNCTION__, __LINE__, gTimerListId);
   }
   
   return rc;
}


/*--------------------------------------------------------------------*/
/*--------------------------------------------------------------------*/
void TimerTask(unsigned long TaskLevel)
{
   int           		Expire;
   unsigned long 	CurrentTime;
   TimerRecord    	*Timer;
   unsigned long  	next_expiration_to,expiration_to;
   ACT_MSG_STRCT  recvMsg;
   int            		ret;
   struct sigaction    actions;

   actions.sa_flags = 0;
   actions.sa_handler = sighand;

   sigaction(SIGALRM,&actions,NULL);


   next_expiration_to = 0; // Forever (actually until the first timer will be armed)

   /* initialize the receiver side in the unix domain sockets implementation */	
   initServer();

   while (1)
   {
      ret = ActReceiveMsg(&recvMsg, sizeof(recvMsg),next_expiration_to);  //  ACL NB 070311  wakeup after minimum to or if new timer was arm or
      if ( ret < ACT_SUCCESS  && ret != ACT_TIMEOUT_ERR )
      {
	      printf("[%s:%d] Unknown event ret=%d\r\n",__FUNCTION__, __LINE__, ret);
	      continue;
      }

	// handle the regular timers
      	/***************************/
      Expire = TRUE;
      next_expiration_to =  0xffffffff; // we look for minimum so we need to start with  high value.
                              // but if there is no timer we shold wait forever that means next_expiration_to=0
      
      CurrentTime = aclGetCurrTime();

      sem_wait(&gTimerListSema4); 

      Timer = (TimerRecord *)acBufferListGetFirst(gTimerListId);

      sem_post(&gTimerListSema4); 

      while (Timer != NULL && Expire == TRUE)
      {

         if (CurrentTime >= Timer->ExpireTime)
         {          

#if 0 // periodic timer not supported 
            if (Timer->UserParams.Type == TIMER_PERIODIC)
            {
               // re-arm periodic timer
               if (RemoveTimerFromList(Timer,TIMER_2_TASK_LEVEL(Timer)) == TIMER_OK)
               {
                  InsertTimerToList(Timer,TIMER_2_TASK_LEVEL(Timer));
               }
               else
               {
                  acl_log(ACL_LOG_ERROR,"[%s:%d] ERROR RemoveTimerFromList TIMER_PERIODIC \r\n",__FUNCTION__, __LINE__);
               }
            }
#endif // periodic timer not supported  

            (*Timer->UserParams.ExpireFunction)(Timer->UserParams.data);

            if (Timer->UserParams.Type == TIMER_THROWAWAY && Timer->TimerMagic == TIMER_MAGIC)
            {	      	
  			sem_wait(&gTimerListSema4); 

            		if(RemoveTimerFromList(Timer,TIMER_2_TASK_LEVEL(Timer)) !=TIMER_OK)
            		{
	      			printf("[%s:%d] Unknown event ret=%d\r\n",__FUNCTION__, __LINE__, ret);
				break;
            		}

		      	sem_post(&gTimerListSema4); 
           }
         }
         else
         {
	        Expire = FALSE;
	        // find the min to left 
	        expiration_to = Timer->ExpireTime - CurrentTime;

	        if (next_expiration_to > expiration_to)
	           	next_expiration_to = expiration_to;
        }	

		sem_wait(&gTimerListSema4); 

	     	Timer = (TimerRecord *)acBufferListGetFirst(gTimerListId);
			
		sem_post(&gTimerListSema4); 

      }//while
   
      if (next_expiration_to == 0xffffffff)
         next_expiration_to = 0; // Means no timeout, we should wait untill we will get arm timer request!!


   }//while (1)

}

/*--------------------------------------------------------------------*/
/*--------------------------------------------------------------------*/

/******************************************************************************
*	Function:	 AcTimerDestroy
*
*	Description: Free timer resources and kill relative thread
*
*	Returns:	 int - TIMER_OK (0) or TIMER_ERROR_GENERAL_ERR (13) or TIMER_NOT_INIT_YET(10)
*
*******************************************************************************/
int AcTimerDestroy(void)
{

	if(isAlreadyInitialized == 0)
	{
	     printf("[%s:%d]  Timer mechanism was not initialized,  not need to invoke AcTimerDestroy()\r\n",__FUNCTION__, __LINE__);
	     return  TIMER_NOT_INIT_YET;
	}

	if( acBufferListDelete(gTimerListId) == AC_STDLIB_ERROR)
	{
		printf("acBufferListDelete  gTimerPoolId = %d", gTimerPoolId);
		return TIMER_ERROR_GENERAL_ERR;
	}

	if( acBuffDeletePool(gTimerPoolId) == AC_STDLIB_ERROR)
	{
		printf("acBuffDeletePool  gTimerPoolId = %d", gTimerPoolId);
		return TIMER_ERROR_GENERAL_ERR;
	}

	if(sem_destroy(&gTimerListSema4) < 0)
	{
		perror("sem_destroy::gTimerListSema4\r\n");
		return TIMER_ERROR_GENERAL_ERR;
	}

	isAlreadyInitialized = 0;

	pthread_kill(gTimerTaskID, SIGALRM);

	return TIMER_OK;
}



void initClient()
{
	  struct sockaddr_un 	actServ_addr;
	  int                			servlen;
	  int					connectFailureDelay = 10000;   /* [usec] */
	  static int				connectFailureCounter = 0;
	  
	   /***************************************************/
	   /* Create unix domain Socket (Client for TIMERS )*/
	   /***************************************************/
	   if ((g_clientSocketFd = socket(AF_UNIX, SOCK_STREAM,0)) < 0)
	   {
	      acl_log(ACL_LOG_ERROR,"[%s:%d] TIMERS Creating UNIX Domain socket \r\n",__FUNCTION__, __LINE__);
	      return;
	   }
	   
	   memset((char *)&actServ_addr,0, sizeof(actServ_addr));
	   actServ_addr.sun_family = AF_UNIX;  
	   
	   strcpy(actServ_addr.sun_path, TIMER_SUN_PATH"MY_TIMERS_SRV");
	   
	   servlen = strlen(actServ_addr.sun_path) + sizeof(actServ_addr.sun_family);

	   while(connect(g_clientSocketFd, (struct sockaddr *) &actServ_addr, servlen) < 0)
	   {
		usleep(connectFailureDelay);
		if((connectFailureCounter*connectFailureDelay*1000000) ==  3)	/* if fail more then 3 seconds exit() */ 
		{
			perror("connect");

			acl_log(ACL_LOG_ERROR,"[%s:%d] TIMERS Connecting UNIX Domain socket client\r\n",__FUNCTION__, __LINE__);
	      	        exit(1);
		}
		connectFailureCounter++;
	   }

}

void initServer()
{

  int                			sockfd;
  struct sockaddr_un 	actCli_addr;
  struct sockaddr_un 	actServ_addr;
  int                			clilen, servlen;

   /********************************************/
   /* Create unix domain Socket Manager TIMERS  */
   /********************************************/

   if ((sockfd = socket(AF_UNIX, SOCK_STREAM,0)) < 0)
   {
	   acl_log(ACL_LOG_ERROR,"[%s:%d] TIMERS Creating UNIX Domain socket \r\n",__FUNCTION__, __LINE__);
	   return;
   }

   memset((char *)&actServ_addr,0, sizeof(actServ_addr));
   actServ_addr.sun_family = AF_UNIX;  
   strcpy(actServ_addr.sun_path, TIMER_SUN_PATH"MY_TIMERS_SRV");
   unlink(actServ_addr.sun_path);
	
   servlen = strlen(actServ_addr.sun_path) + sizeof(actServ_addr.sun_family);

   if(bind(sockfd,(struct sockaddr *)&actServ_addr, servlen) < 0)
   {
	   acl_log(ACL_LOG_ERROR,"[%s:%d] TIMERS binding socket UNIX Domain socket \r\n",__FUNCTION__, __LINE__);
	   return;
   }

   clilen = sizeof(actCli_addr);

   if (listen(sockfd,5) == -1) {
           perror("listen");
           exit(1);
   }

   g_serverSocketFd = accept(sockfd,(struct sockaddr *)&actCli_addr,(socklen_t *)&clilen);

   if (g_serverSocketFd < 0) 
   {
        perror("accept");
      	acl_log(ACL_LOG_ERROR,"[%s:%d] TIMERS accepting socket UNIX Domain socket \r\n",__FUNCTION__, __LINE__);
      	return ;
   }

}


/*--------------------------------------------------------------------*/
/*--------------------------------------------------------------------*/
void AcStartTimers()
{
	static struct sched_param	sched;
	static pthread_attr_t		   attr;

	memset(&sched, 0, sizeof(sched));

   	//For now we only support on high priority timers thread 

	/* create the receiver thread */
	if( (pthread_attr_init(&attr) != 0) ){
		acl_log(ACL_LOG_ERROR,"pthread_attr_init");
		pthread_exit(NULL);
	}
	if( (pthread_attr_setschedpolicy(&attr, SCHED_RR) != 0) )
   	{
		acl_log(ACL_LOG_ERROR,"pthread_attr_setschedpolicy");
		pthread_exit(NULL);
	}	
  	sched.sched_priority = ACL_TIMER_PRIORITY;	
 	if( (pthread_attr_setschedparam(&attr, &sched) != 0) )
   	{
		acl_log(ACL_LOG_ERROR,"pthread_attr_setschedparam");
		pthread_exit(NULL);
	}

   	if( (pthread_create( &gTimerTaskID, &attr, (void *)TimerTask, (void *)1 ) != 0) )
   	{
		acl_log(ACL_LOG_ERROR,"pthread_create - aclTimers");
		pthread_exit(NULL);
	}

   	/* initialize the sender side in the unix domain sockets implementation */	
	initClient();

  	return;
}
/*--------------------------------------------------------------------*/
/*--------------------------------------------------------------------*/

/******************************************************************************
*	Function:	 AcTimerInit
*
*	Description: Allocates resources for timer mechanism and exeutes the TimerTask thread
*
*	Returns:	 int - TIMER_OK (0) or TIMER_ERROR_GENERAL_ERR (13)
*
*******************************************************************************/
int AcTimerInit(void)
{
 
   unsigned int 			NumPool = ACL_MAX_NUMBER_OF_TIMERS;
   unsigned int 			PoolBuffSize = sizeof(TimerRecord);   // TBD


   if(isAlreadyInitialized == 1)
   {
      printf("[%s:%d]  AcTimerInit() was already invoked\r\n",__FUNCTION__, __LINE__);
      return  TIMER_ERROR_GENERAL_ERR;
   }

   gTimerListId =  NULL;
   gNumTimers = 0;

   // ACL NB 070311
   if ( acBuffCreatePool("TimerMechanism",NumPool,PoolBuffSize,POOL_ENABLE_EXTERNAL_LISTS|POOL_DELETE_ENABLE|POOL_DELETE_FORCED, &gTimerPoolId) == AC_STDLIB_ERROR )
   {
		printf("acBuffCreatePool %s", "TimerMechanism");
		return TIMER_ERROR_GENERAL_ERR;
   }

   if(acBufferListCreate(gTimerPoolId, -1, &gTimerListId) == AC_STDLIB_ERROR)
   {
		printf("acBufferListCreate");
		return TIMER_ERROR_GENERAL_ERR;
   }

   if(sem_init(&gTimerListSema4, 0, 1) < 0)
   {
		printf("error at sem_init\r\n");
		return TIMER_ERROR_GENERAL_ERR;
   }

   AcStartTimers();

   isAlreadyInitialized = 1;
	
   return TIMER_OK;
}

/*--------------------------------------------------------------------*/
/*--------------------------------------------------------------------*/
static int RunTimer(TimerRecord *Timer, int *pTid)
{
   int rc = TIMER_OK;

   sem_wait(&gTimerListSema4); 
   
   if (Timer->TimerMagic != TIMER_MAGIC)
      rc = TIMER_ILLEGAL_TID;
   else
   {
      if (Timer->ExpireTime != 0)
      {
         rc = TIMER_ERROR_CONFIG_RUN_TIMER;
         acl_log(ACL_LOG_ERROR,"[%s:%d] Timer already running. Unable to run it again. ReturnCode=%d\r\n",__FUNCTION__, __LINE__, rc);
         
      }
      else
      {
         	if(InsertTimerToList(Timer,TIMER_2_TASK_LEVEL(Timer), pTid) != TIMER_OK)
         	{
      			printf("[%s:%d]  InsertTimerToList failed.\r\n",__FUNCTION__, __LINE__);
		   	sem_post(&gTimerListSema4); 
			return TIMER_ERROR_NO_FREE;
         	}
      }      
   }

   sem_post(&gTimerListSema4); 
 
   return rc;  
}

/*--------------------------------------------------------------------*/
/*--------------------------------------------------------------------*/
/******************************************************************************
*	Function:	 AcArmTimer
*
*	Description: Creating a timer with the relevant parameters and running it
*
*	Parameters:	[I] TimerUserParams *pUserTimerConfig - The user timer configuration
*                		[O] int *pTid - The ID of the newly created timer
*
*	Returns:	 int - TIMER_OK (0) or TIMER_NOT_INIT_YET (10) or TIMER_ILLEGAL_PARAM(9)
*
*******************************************************************************/
int AcArmTimer(TimerUserParams *pUserTimerConfig,int *pTid)
{
   TimerRecord     	NewTimer;
   int 			rc = TIMER_OK;

   if(isAlreadyInitialized == 0)
   {
      printf("[%s:%d]  Timer mechanism was not initialized,  AcTimerInit() should be invoked\r\n",__FUNCTION__, __LINE__);
      return  TIMER_NOT_INIT_YET;
   }
   
   // For now we support only TIMER_THROWAWAY 
   if ( pUserTimerConfig->Type != TIMER_THROWAWAY )
   {
      printf("[%s:%d]  Timer type not supported type=%d>\r\n",__FUNCTION__, __LINE__, pUserTimerConfig->Type);
      return  TIMER_ILLEGAL_PARAM;
   }

   if(pUserTimerConfig->Period < 0)
   {
      printf("[%s:%d]  Period can't be negative\r\n",__FUNCTION__, __LINE__);
      return  TIMER_ILLEGAL_PARAM;
   }

   NewTimer.TimerMagic 			= TIMER_MAGIC;
   NewTimer.ExpireTime 			= 0;
   NewTimer.UserParams.Period	= 0;

   if (rc  == TIMER_OK)
   {
      rc = ConfigTimer(&NewTimer, pUserTimerConfig);

      if (rc == TIMER_OK)
      {
         rc = RunTimer(&NewTimer, pTid);
         if (rc == TIMER_OK )
             // ACL NB 070311
            ActSendNewTimerMsg(); // to wake up the task
      }
   }

   return rc;  
}


/*--------------------------------------------------------------------*/
/*--------------------------------------------------------------------*/
/******************************************************************************
*	Function:	 AcDeleteTimer
*
*	Description: Delete a specific timer
*
*	Parameters:	[I] int Tid - The timer id to be deleted
*
*	Returns:	 int - TIMER_OK (0) or TIMER_NOT_INIT_YET (10) or TIMER_ILLEGAL_TID(8) 
*				 or TIMER_ERROR_GENERAL_ERR(13)
*
*******************************************************************************/
int AcDeleteTimer(int Tid)
{
   int rc = TIMER_OK;
   TimerRecord *Timer = (TimerRecord *)Tid;


   if(isAlreadyInitialized == 0)
   {
      printf("[%s:%d]  Timer mechanism was not initialized,  AcTimerInit() should be invoked\r\n",__FUNCTION__, __LINE__);
      return  TIMER_NOT_INIT_YET;
   }

   sem_wait(&gTimerListSema4); 

   if(Timer == NULL)
      rc = TIMER_ILLEGAL_TID;
   else if (Timer->TimerMagic != TIMER_MAGIC)
      rc = TIMER_ERROR_GENERAL_ERR;
   else
   {
	rc =RemoveTimerFromList(Timer,TIMER_2_TASK_LEVEL(Timer));
	if(rc != TIMER_OK)
	{
		printf("[%s:%d] Unknown event \r\n",__FUNCTION__, __LINE__);
	}
   }

   sem_post(&gTimerListSema4); 

   return rc;  
}

	  
/*--------------------------------------------------------------------*/
/*--------------------------------------------------------------------*/
static int ConfigTimer(TimerRecord *Timer, TimerUserParams *pUserTimerConfig)
{ 
   int rc = TIMER_OK;

  sem_wait(&gTimerListSema4); 
   
   if (Timer->TimerMagic != TIMER_MAGIC)
   	rc = TIMER_ILLEGAL_TID;
   else
   {
/*
      if ((UserTimerConfig.Type == TIMER_PERIODIC_ROUNDED) &&
         ((UserTimerConfig.RoundedPeriod > ROUNDED_DAY || UserTimerConfig.RoundedPeriod == ROUNDED_NULL)))
         rc = TIMER_ILLEGAL_PARAM;
*/
      if (pUserTimerConfig->Type == TIMER_PERIODIC_ROUNDED)  
      {
         rc = TIMER_ILLEGAL_PARAM;
         acl_log(ACL_LOG_DEBUG,"[%s:%d] ERROR Periodic timer not supported yet !!!\r\n",__FUNCTION__, __LINE__);
      }
      else
      {
         if (Timer->ExpireTime != 0)
            rc = TIMER_ERROR_CONFIG_RUN_TIMER;
         else
         {
            memcpy((char *)&Timer->UserParams,(char *)pUserTimerConfig,sizeof(TimerUserParams));
         }
      }
   }

   sem_post(&gTimerListSema4); 

   return rc;  
}


static void sighand(int signo)
{
	pthread_exit(NULL);
}


/*--------------------------------------------------------------------*/
/*--------------------------------------------------------------------*/
#if 0
int AcChangeTimerPeriod(int Tid, unsigned long NewPeriod)
{
   int rc = TIMER_OK;
   TimerRecord *Timer = (TimerRecord *)Tid;

   acl_log(ACL_LOG_DEBUG,"[%s:%d] >>\r\n",__FUNCTION__, __LINE__);

   if (Timer->TimerMagic != TIMER_MAGIC)
      rc = TIMER_ILLEGAL_TID;
   else
   {
      if ((Timer->UserParams.Type == TIMER_PERIODIC_ROUNDED) &&
         ((NewPeriod > ROUNDED_DAY || NewPeriod == ROUNDED_NULL)))
         rc = TIMER_ILLEGAL_PARAM;	  
      else
      {
         if (RemoveTimerFromList(Timer,TIMER_2_TASK_LEVEL(Timer)) == TIMER_OK)
         {
            Timer->UserParams.Period = NewPeriod;
            InsertTimerToList(Timer,TIMER_2_TASK_LEVEL(Timer));
             // ACL NB 070311
            ActSendNewTimerMsg(); // to wake up the task

         }
         else
         {
            acl_log(ACL_LOG_ERROR,"[%s:%d] ERROR RemoveTimerFromList\r\n",__FUNCTION__, __LINE__);
         }
      }
   }
   
   return rc;  
}

/*--------------------------------------------------------------------*/
/*--------------------------------------------------------------------*/
int  AcChangeTimerExpireFunction(int Tid, TypeExpire ExpireFunction,unsigned long Params[])
{
   TimerRecord *Timer = (TimerRecord *)Tid;
   TimerRecord tmpTimer;
   
   acl_log(ACL_LOG_DEBUG,"[%s:%d] >>\r\n",__FUNCTION__, __LINE__);
   
   if (Timer->TimerMagic != TIMER_MAGIC)
      rc = TIMER_ILLEGAL_TID;
   else
   {
      memcpy(&tmpTimer,Timer, sizeof(tmpTimer))
      if (RemoveTimerFromList(Timer,TIMER_2_TASK_LEVEL(Timer)) == TIMER_OK)
      {
         tmpTimer.UserParams.ExpireFunction = ExpireFunction;
         tmpTimer.UserParams.FunctionParams[0] = Params[0];
         tmpTimer.UserParams.FunctionParams[1] = Params[1];
         tmpTimer.UserParams.FunctionParams[2] = Params[2];
         tmpTimer.UserParams.FunctionParams[3] = Params[3];
         InsertTimerToList(tmpTimer,TIMER_2_TASK_LEVEL(Timer));
         // ACL NB 070311
         ActSendNewTimerMsg(); // to wake up the task
         
      }
      else
      {
         acl_log(ACL_LOG_ERROR,"[%s:%d] ERROR RemoveTimerFromList\r\n",__FUNCTION__, __LINE__);
      }
   }
   
   return rc;  
   
}

#endif
