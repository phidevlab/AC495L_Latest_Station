/* -- 
* --	File			:	acl_MsgQ.h
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

#ifndef	_ACL_MSGQ_H
#define	_ACL_MSGQ_H

#include <pthread.h>

#define	MAXIMUM_NUM_OF_MSG_QUEUES 					5

#define IPHONE_MANAGER_Q_MAXIMUM_NUMBER_OF_MESSAGES	60
#define HW_TRANSMIT_Q_MAXIMUM_NUMBER_OF_MESSAGES	40

#define IPHONE_MANAGER_Q_MESSAGE_SIZE				500
#define HW_TRANSMIT_Q_MESSAGE_SIZE					1500

#define MSGQ_INDEX_EMPTY	0
#define MSGQ_INDEX_USED		1


typedef struct
{
	unsigned int 	 	poolId;
	unsigned int		bufferSize;
	unsigned int	 	listId;
	char			 	MsgQueueName[128];
	volatile char		*tmpMsgp;
	pthread_mutex_t		msgQSndRcvMut;
	pthread_mutex_t  	mut;
	pthread_cond_t 		cond;

} idList;


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
extern int acMsgQueueCreate(char MsgQueueName[], unsigned int NumBuff, unsigned int BuffSize);



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
extern int acMsgQueueDelete(int MsgQId);



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
extern int acMsgsnd(int msqid, void *msgp, int msgsz);



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
*						   queue. 
*				msgp   -   Points to a user-defined buffer. 
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	0 - success     -1 in case of error 
******************************************************************************/
extern int acMsgrcv(int msqid, char *msgp);


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
extern int init_acMsgQDB(void);


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
void destroy_acMsgQDB(void);


#endif	/* _ACL_MSGQ_H */

