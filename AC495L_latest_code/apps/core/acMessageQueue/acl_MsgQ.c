/* -- 
* --	File			:	acl_MsgQ.c
* --
* --	Description		:	Implementing message queue mechanism.
* --					
* --	Author			:	Nir Michaeli
* --	
* --	Limitations		:	None.
* --
* --	Known Problems	:	None.
* --	
* --	
* --	Revision History:
* --	
* --	  - 15/11/06 - NirM - First version
* --	
* ----------------------------------------------------------------------------
* ----------------------------------------------------------------------------
*/

#include <stdlib.h>
#include <string.h>
#include <semaphore.h>

#include "acl_log.h"
#include "acl_MsgQ.h"
#include "acBufferPool.h"
#include "acBufferList.h"


idList 				msgQId[MAXIMUM_NUM_OF_MSG_QUEUES];
int 				msgQIdIndex[MAXIMUM_NUM_OF_MSG_QUEUES] = {MSGQ_INDEX_EMPTY};
pthread_mutex_t		msgQIdMut;

/******* Message Queues Id's Data base *******/
int					manager_Q_id;
int					msgqid_hw_tx;

/*#define  ACL_DEBUG*/

#ifdef ACL_DEBUG
#define ACL_LogDebug printf
#else
#define ACL_LogDebug
#endif


int getMsgQId();
int deleteMsgQId(unsigned int msgQId);


/****************************************************************************
*
*	Name:		acMsgQueueCreate
*----------------------------------------------------------------------------
*	Abstract:	Creating a message queue.
*----------------------------------------------------------------------------
*	Input:		char MsgQueueName[] - User Name to the new pool 
*               unsigned int NumBuff - Number of buffers
*               unsigned int BuffSize - Buffer byte size
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	msqid of the new message queue  (-1 in case of error)
******************************************************************************/
int acMsgQueueCreate(char MsgQueueName[], unsigned int NumBuff, unsigned int BuffSize)
{
	int currentMsgQId;

	if ((currentMsgQId = getMsgQId()) == -1)
	{
		acl_log(ACL_LOG_ERROR,"Exceeding the Maximum Message Queues Number - Need to increase the MAXIMUM_NUM_OF_MSG_QUEUES");
		return -1;
	}
	
	if(acBuffCreatePool(MsgQueueName, NumBuff, BuffSize, POOL_ENABLE_EXTERNAL_LISTS|POOL_DELETE_ENABLE|POOL_DELETE_FORCED, &msgQId[currentMsgQId].poolId) == AC_STDLIB_ERROR)
	{
		acl_log(ACL_LOG_ERROR,"acBuffCreatePool %s", MsgQueueName);
		return -1;
	}
	
	if(acBufferListCreate(msgQId[currentMsgQId].poolId, -1, &msgQId[currentMsgQId].listId) == AC_STDLIB_ERROR)
	{
		acl_log(ACL_LOG_ERROR,"acBufferListCreate %s", MsgQueueName);
		return -1;
	}

	if(strlen(MsgQueueName) < sizeof(msgQId[currentMsgQId].MsgQueueName))
		strcpy(msgQId[currentMsgQId].MsgQueueName, MsgQueueName);
	else
	{
		acl_log(ACL_LOG_ERROR,"String length of MsgQueueName %d is equal or bigger then sizeof MsgQueueName %d", 
			strlen(MsgQueueName), sizeof(msgQId[currentMsgQId].MsgQueueName));
		
		return (-1);
	}

	msgQId[currentMsgQId].bufferSize = BuffSize;

	return currentMsgQId;
	
}


/****************************************************************************
*
*	Name:		acMsgQueueDelete
*----------------------------------------------------------------------------
*	Abstract:	Delete a message queue.
*----------------------------------------------------------------------------
*	Input:		int MsgQId  -  Message Queue Identifier
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	0 - success     -1 in case of error 
******************************************************************************/
int acMsgQueueDelete(int MsgQId)
{
	if( acBufferListDelete(msgQId[MsgQId].listId) == AC_STDLIB_ERROR)
	{
		acl_log(ACL_LOG_ERROR,"acBufferListDelete  MsgQId = %d", MsgQId);
		return -1;
	}

	if( acBuffDeletePool(msgQId[MsgQId].poolId) == AC_STDLIB_ERROR)
	{
		acl_log(ACL_LOG_ERROR,"acBuffDeletePool  MsgQId = %d", MsgQId);
		return -1;
	}

	if(deleteMsgQId(MsgQId) == -1)
	{
		acl_log(ACL_LOG_ERROR,"deleteMsgQId  MsgQId = %d", MsgQId);
		return -1;
	}
	
	return 0;
}



/****************************************************************************
*
*	Name:		acMsgsnd
*----------------------------------------------------------------------------
*	Abstract:	Sends a message to the queue associated with message queue 
*				identifier msqid. 
*----------------------------------------------------------------------------
*	Input:		msqid  -  Is a unique positive integer that identifies a message 
*						  queue. 
*				msgp  -   Points to a user-defined buffer. 
*				msgsz -   The length of the message to be sent. 
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	0 - success     -1 in case of error 
******************************************************************************/
int acMsgsnd(int msqid, void *msgp, int msgsz)
{
	pthread_mutex_lock(&msgQId[msqid].msgQSndRcvMut); 

	if(acBufferListAddLast(msgQId[msqid].listId, msgp, msgsz) == NULL)
	{
		acl_log(ACL_LOG_ERROR,"acBufferListAddLast  msqid = %d", msqid);
		pthread_mutex_unlock(&msgQId[msqid].msgQSndRcvMut); 
		return -1;
	}

	pthread_mutex_lock(&msgQId[msqid].mut);	
	pthread_cond_signal(&msgQId[msqid].cond);	
	pthread_mutex_unlock(&msgQId[msqid].mut);

	pthread_mutex_unlock(&msgQId[msqid].msgQSndRcvMut); 

	return 0;
}


/****************************************************************************
*
*	Name:		acMsgrcv
*----------------------------------------------------------------------------
*	Abstract:	Reads a message from the queue associated with the message queue 
*				identifier that msqid specifies and places it in the user-defined 
*				structure that msgp points to.
*				This function is blocking and waiting for a message to arrive.
*----------------------------------------------------------------------------
*	Input:		msqid  -   Is a unique positive integer that identifies a message 
*						 queue. 
*				msgp   -   Points to a user-defined buffer. 
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	0 - success     -1 in case of error 
******************************************************************************/
int acMsgrcv(int msqid, char *msgp)
{
	pthread_mutex_lock(&msgQId[msqid].msgQSndRcvMut); 

	msgQId[msqid].tmpMsgp = acBufferListGetFirst(msgQId[msqid].listId);

	while(msgQId[msqid].tmpMsgp == NULL)
	{
		pthread_mutex_unlock(&msgQId[msqid].msgQSndRcvMut); 

		pthread_mutex_lock(&msgQId[msqid].mut);		
		pthread_cond_wait(&msgQId[msqid].cond, &msgQId[msqid].mut);		
		pthread_mutex_unlock(&msgQId[msqid].mut);

		pthread_mutex_lock(&msgQId[msqid].msgQSndRcvMut); 
		
		msgQId[msqid].tmpMsgp = acBufferListGetFirst(msgQId[msqid].listId);
		if(msgQId[msqid].tmpMsgp == NULL)
			acl_log(ACL_LOG_DEBUG,"acBufferListGetFirst returned unexpected NULL  msqid = %d", msqid);
	}

	memcpy(msgp, (char *)msgQId[msqid].tmpMsgp, msgQId[msqid].bufferSize);

	if(acBufferListPop(msgQId[msqid].listId) == AC_STDLIB_ERROR)
	{
		acl_log(ACL_LOG_ERROR,"acBufferListPop msqid = %d", msqid);
		pthread_mutex_unlock(&msgQId[msqid].msgQSndRcvMut); 
		return -1;
	}

	if(pthread_mutex_unlock(&msgQId[msqid].msgQSndRcvMut) < 0)
	{
		perror("\npthread_mutex_unlock");
	}

	return 0;  
	
}


/****************************************************************************
*
*	Name:		init_acMsgQDB
*----------------------------------------------------------------------------
*	Abstract:	Initializing the Message Queue Data Base
*----------------------------------------------------------------------------
*	Input:		
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	0 - success     -1 in case of error 
******************************************************************************/
int init_acMsgQDB(void)
{
	int i;
	
	if(pthread_mutex_init(&msgQIdMut, NULL) < 0)
	{
		acl_log(ACL_LOG_ERROR,"error at pthread_mutex_init\r\n");
		return -1;
	}

	for(i=0; i < MAXIMUM_NUM_OF_MSG_QUEUES; i++)
	{
		msgQIdIndex[i] = MSGQ_INDEX_EMPTY;
		
		if(pthread_cond_init(&msgQId[i].cond, NULL) < 0)
		{
			acl_log(ACL_LOG_ERROR,"error at pthread_cond_init\r\n");
			return -1;
		}

		if(pthread_mutex_init(&msgQId[i].mut, NULL) < 0)
		{
			acl_log(ACL_LOG_ERROR,"error at pthread_mutex_init\r\n");
			return -1;
		}

		if(pthread_mutex_init(&msgQId[i].msgQSndRcvMut, NULL) < 0)
		{
			acl_log(ACL_LOG_ERROR,"error at pthread_mutex_init\r\n");
			return -1;
		}

	}

	/* Create all the application message queues */ 
	manager_Q_id = acMsgQueueCreate("IPHONE_MANAGER_Q", 
		IPHONE_MANAGER_Q_MAXIMUM_NUMBER_OF_MESSAGES, 
		IPHONE_MANAGER_Q_MESSAGE_SIZE);
	msgqid_hw_tx = acMsgQueueCreate("HW_TRANSMIT_Q", 
		HW_TRANSMIT_Q_MAXIMUM_NUMBER_OF_MESSAGES,
		HW_TRANSMIT_Q_MESSAGE_SIZE);

    return 0;
	
}

/****************************************************************************
*
*	Name:		destroy_acMsgQDB
*----------------------------------------------------------------------------
*	Abstract:	     destroying the Message Queue Data Base
*----------------------------------------------------------------------------
*	Input:		
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	0 - success     -1 in case of error 
******************************************************************************/
void destroy_acMsgQDB(void)
{
	int i;

	if(pthread_mutex_destroy(&msgQIdMut) < 0)
	{
		acl_log(ACL_LOG_ERROR,"error at pthread_mutex_destroy\r\n");
	}

	for(i=0; i < MAXIMUM_NUM_OF_MSG_QUEUES; i++)
	{
		if(pthread_cond_destroy(&msgQId[i].cond) < 0)
		{
			acl_log(ACL_LOG_ERROR,"error at pthread_cond_destroy\r\n");
		}

		if(pthread_mutex_destroy(&msgQId[i].mut) < 0)
		{
			acl_log(ACL_LOG_ERROR,"error at pthread_mutex_destroy\r\n");
		}

		if(pthread_mutex_destroy(&msgQId[i].msgQSndRcvMut) < 0)
		{
			acl_log(ACL_LOG_ERROR,"error at pthread_mutex_destroy\r\n");
		}
	}

	if(acMsgQueueDelete(manager_Q_id) < 0)
	{
		acl_log(ACL_LOG_ERROR,"error at acMsgQueueDelete:: %d\r\n", manager_Q_id);
	}
	if(acMsgQueueDelete(msgqid_hw_tx) < 0)
	{
		acl_log(ACL_LOG_ERROR,"error at acMsgQueueDelete:: %d\r\n", msgqid_hw_tx);
	}
}

int getMsgQId()
{
	int i;

	pthread_mutex_lock(&msgQIdMut); 
	for(i=0; i < MAXIMUM_NUM_OF_MSG_QUEUES; i++)
	{
		if(msgQIdIndex[i] == MSGQ_INDEX_EMPTY)
		{
			msgQIdIndex[i] = MSGQ_INDEX_USED;
			pthread_mutex_unlock(&msgQIdMut); 
			return i;
		}
	}	
	pthread_mutex_unlock(&msgQIdMut); 

	return -1;
}

int deleteMsgQId(unsigned int msgQId)
{
	pthread_mutex_lock(&msgQIdMut); 
	if((msgQId > 0) && (msgQId < MAXIMUM_NUM_OF_MSG_QUEUES))
	{
		msgQIdIndex[msgQId] = MSGQ_INDEX_EMPTY;
	}
	else
	{
		acl_log(ACL_LOG_ERROR,"illegal value of msgQId");
		pthread_mutex_unlock(&msgQIdMut); 
		return -1;
	}	
	pthread_mutex_unlock(&msgQIdMut); 

	return 0;
}
