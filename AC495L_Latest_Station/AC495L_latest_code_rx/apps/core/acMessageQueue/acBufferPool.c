/*
* ----------------------------------------------------------------------------
* ----------------------------------------------------------------------------
* --                                                                        --
* --  ACStdLib - AudioCodes Standard Library - Copyright 2005 - AudioCodes  --
* --                                                                        --
* ----------------------------------------------------------------------------
* ----------------------------------------------------------------------------
* -- 
* --	File			:	acBufferPool.c
* --
* --	Description		:	A pool of equal-size buffers.
* --					
* --	Author			:	Itzik Ben-Dov
* --	
* --	Limitations		:	None.
* --
* --	Known Problems	:	None.
* --	
* --	Notes			:   None.
* --	
* --	Revision History:
* --	
* --	  - 12/4/05 - Itzik - First version
* --	
* ----------------------------------------------------------------------------
* ----------------------------------------------------------------------------
*/
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include "quLinkedQueue.h"
#include "acBufferPool.h"
#include "acStdLibHal.h"
#include "acBufferPoolHal.h"
#include "acStdLib.h"

/* Buffer Header */
typedef struct PBuf{
  unsigned int   Application;
  int            AuditTime;
  unsigned int          LR;
}BufHdrRec;

/* User Buffer start at header end */
#define BUF_START_OFFSET  (sizeof(BufHdrRec))

#define BUF_FULL_SIZE(pool)  (pool->BuffSize + BUF_START_OFFSET + sizeof(unsigned int))

#define Index2Ptr(pool,index) ((void*)((unsigned int)pool->StartAdr + (index * BUF_FULL_SIZE(pool))))

#define Ptr2Index(pool,Ptr)   (((unsigned int)Ptr - (unsigned int)pool->StartAdr) / BUF_FULL_SIZE(pool))

#define POOL_AUDIT_TIME_MASK      POOL_AUDIT_TIME_HOUR
#define POOL_AUDIT_ACT_MASK       (POOL_AUDIT_ACT_FREE | POOL_AUDIT_ACT_ERROR | POOL_AUDIT_ACT_STATISTIC)

#define BUF_END_MAGIC  0x454E4442 /* "ENDB" */
#define BUF_FREE_MAGIC 0x46524545 /* "FREE" */

/******************************************************************************
*	Function:	 acBuffCreatePool
*
*	Description: Create a new buffers pool
*
*	Parameters:	 [I] char PoolName[] - User Name to the new pool 
*                [I] unsigned int NumBuff - Pool number of buffers
*                [I] unsigned int BuffSize - Buffer byte size
*                [I] unsigned int Attributes - Pool attributes (macros above)
*                [O] unsigned int *PoolId - The ID of the newly created pool
*
*	Returns:	 int - AC_STDLIB_OK (0) or AC_STDLIB_ERROR (-1)
*
*******************************************************************************/
int acBuffCreatePool(char *PoolName,unsigned int NumBuff, unsigned int BuffSize,
					 unsigned int Attributes, unsigned int *PoolId)
{
	int              rc=AC_STDLIB_OK,FullBuffSize;
	void             *(*alloc)(size_t);
	acBuffPoolDescriptor   *Pool;
	void             *PoolStartAdr;
	quTElementHead*  quElements;
	
	if (Attributes & POOL_DELETE_ENABLE)
		alloc = malloc;
	else

/* NirM 151106, Working as WIN32 in AC48 CPE VTK */
#if 0
#if WIN32
		alloc = malloc;
#else
	alloc = salloc;
#endif
#endif

	alloc = malloc;

	
	Pool = (acBuffPoolDescriptor *)(*alloc)(sizeof(acBuffPoolDescriptor));
	
	FullBuffSize = (BuffSize + sizeof(BufHdrRec) + sizeof(unsigned int));
	
	PoolStartAdr = (*alloc)( NumBuff * FullBuffSize );
	
	quElements = (quTElementHead *)(*alloc)(sizeof(quTElementHead)*NumBuff);
	
	if (Pool == NULL || PoolStartAdr == NULL || quElements == NULL)
	{
		acStdLib_ErrorHandler(RecoverableMsg,0,__FILE__,__LINE__,-1,
			"acBuffCreatePool - New Buff Pool - No Memory");
		
		if (Attributes & POOL_DELETE_ENABLE)
		{
			free((void*)Pool);
			free((void*)PoolStartAdr);
			free((void*)quElements);
		}
		
		rc = AC_STDLIB_ERROR;
	}	
	else
	{
		strncpy(Pool->Name,PoolName,AC_STDLIB_MAX_OBJECT_NAME);
		Pool->Name[AC_STDLIB_MAX_OBJECT_NAME] = '\0';
		Pool->StartAdr = PoolStartAdr;
		Pool->quElements = quElements;
		Pool->EndAdr = (char *)PoolStartAdr + (NumBuff * FullBuffSize);
		Pool->BuffSize = BuffSize;
		Pool->TotalBuf = NumBuff;
		Pool->Attributes = Attributes;
		Pool->BuffMaxTime = 0;
		Pool->BuffAverageTime = 0;
		Pool->BuffMaxUsage = 0;
		Pool->MagicNumber = AC_STD_MAGIC;
		
		switch (Attributes & POOL_AUDIT_TIME_MASK)
		{
		case POOL_AUDIT_TIME_SECOND:
			Pool->AuditTime = 1;
			break;
			
		case POOL_AUDIT_TIME_MINUTE:
			Pool->AuditTime = 60;
			break;
			
		case POOL_AUDIT_TIME_HOUR:
			Pool->AuditTime = 3600;
			break;
			
		}
		
		/* Init buffer manager for 1 queue of busy buffers */
		QuInitQueue(NumBuff, 1, Pool->quHead, Pool->quElements);
		
		RegisterPoolAuditDebug(Pool);
	}

	if (rc==AC_STDLIB_OK)
		*PoolId = (unsigned int)Pool;
	
	return rc;
}

/******************************************************************************
*	Function:	 acBuffDeletePool
*
*	Description: Delete the Pool
*
*	Parameters:	 [I] unsigned int PoolId - the pool ID
*
*	Returns:	 int - AC_STDLIB_OK (0) or AC_STDLIB_ERROR (-1)
*
*******************************************************************************/
int acBuffDeletePool(unsigned int PoolId)
{
  acBuffPoolDescriptor   *Pool;
  unsigned int     FreeBuff;
  
  Pool = (acBuffPoolDescriptor*)PoolId;
  
  if ((Pool->Attributes & POOL_DELETE_ENABLE) == 0)
	{
	  acStdLib_ErrorHandler(RecoverableMsg,0,__FILE__,__LINE__,-1,
			"acBuffDeletePool - Pool %s delete failed - delete disabled.",Pool->Name);
	  return AC_STDLIB_ERROR;
	}

  FreeBuff = quGetLen(Pool->quHead,QUEUE_EMPTY_INDEX);

  if (((Pool->Attributes & POOL_DELETE_FORCED) == POOL_DELETE_IF_EMPTY) && (FreeBuff != Pool->TotalBuf))
	{
	  acStdLib_ErrorHandler(RecoverableMsg,0,__FILE__,__LINE__,-1,
			"acBuffDeletePool - Pool %s delete failed, not all buffers are free.",Pool->Name);
	  return AC_STDLIB_ERROR;
	}

  DeRegisterPool(Pool);
  
  free(Pool->quElements);
  free(Pool->StartAdr);
  free(Pool);

  return AC_STDLIB_OK;
}
  
/******************************************************************************
*	Function:	 acBuffAlloc
*
*	Description: Allocate a buffer from pool
*
*	Parameters:	 [I] unsigned int PoolId - the pool ID
*                [I] int ApplicationID - An ID to be used for debugging
*
*	Returns:	 void * - Pointer to the new buffer or NULL
*
*******************************************************************************/
void *acBuffAlloc(unsigned int PoolId,unsigned int Application)
{
  acBuffPoolDescriptor   *Pool;
  unsigned int             lr=0;
  int               index,rc;
  BufHdrRec        *Buff;

/* NirM 151106, Working as WIN32 in AC48 CPE VTK */
#if 0
#ifndef WIN32
  lr = GetLinkRegister();
#endif  
#endif

  Pool = (acBuffPoolDescriptor*)PoolId;

	if(!AC_STD_OBJECT_VALIDATE(Pool))
	{
		acStdLib_ErrorHandler(RecoverableMsg,0,__FILE__,__LINE__,-1,
			"acBuffAlloc - Invalid Pool ID.");
		return NULL; 
	}

  if (Pool->Attributes & POOL_ENABLE_EXTERNAL_LISTS)
	  /* If external lists of allocated buffers are used, do not add element to allocated list */
	  rc = quAllocBuf(Pool->quHead,Pool->quElements,&index);
  else
          rc = quAllocPutBuf(Pool->quHead,Pool->quElements,&index, 1 /*instead of default value*/);  

  if (rc != QUEUE_OK )
	{
	  if ((Application & AC_STDLIB_INTERNAL_APPLICATION) == 0)
		acStdLib_ErrorHandler(RecoverableMsg,0,__FILE__,__LINE__,-1,
							  "acBuffAlloc - Pool %s Application %d Fail: %d, LR %x",Pool->Name,Application,rc,lr);  
	  return NULL;
        }

  Buff = (BufHdrRec *)Index2Ptr(Pool,index);   
        
  *(unsigned int *)((unsigned int)Buff + BUF_START_OFFSET + Pool->BuffSize) = BUF_END_MAGIC;
  
  Buff->AuditTime = Pool->AuditTime;
  
  Buff->Application = Application;
  
  Buff->LR = lr;
  
  /* return to caller the buffer after it's header */
  return ((void*)((unsigned int)Buff+BUF_START_OFFSET));
}

/******************************************************************************
*	Function:	 acBuffFree
*
*	Description: Free a buffer - return it to the pool
*
*	Parameters:	 [I] unsigned int PoolId - the pool ID
*                [I] void *Buff - Buffer pointer.
*
*	Returns:	 
*
*******************************************************************************/
void acBuffFree(unsigned int PoolId, void *BufPtr)
{
  acBuffPoolDescriptor   *Pool;
  int        				Index;
  unsigned int            *MagicPtr;
  BufHdrRec*       BufHdr;

  Pool = (acBuffPoolDescriptor*)PoolId;

	if(!AC_STD_OBJECT_VALIDATE(Pool))
	{
		acStdLib_ErrorHandler(RecoverableMsg,0,__FILE__,__LINE__,-1,
					   "acBuffFree - Invalid Pool ID");
		return; 
	}

  BufHdr = (BufHdrRec *)((unsigned int)BufPtr - BUF_START_OFFSET);

  Index = Ptr2Index(Pool,BufPtr);
  
  if (Index < 0 || Index >= (int)Pool->TotalBuf)
	{
	  acStdLib_ErrorHandler(RecoverableMsg,0,__FILE__,__LINE__,-1,
			"acBuffFree %s Fail: Buff %x, index %d",Pool->Name,BufPtr,Index);  
	  return;
	}

  MagicPtr = (unsigned int *)((unsigned int)BufPtr + Pool->BuffSize);

  if (*MagicPtr != BUF_END_MAGIC)
	{
	  acStdLib_ErrorHandler(RecoverableMsg,0,__FILE__,__LINE__,-1,
			"acBuffFree %s Fail: Buff %x, Magic %x, LR %x",Pool->Name,BufPtr,
					   *MagicPtr,BufHdr->LR);  
	  return;
	}
	
  *MagicPtr = BUF_FREE_MAGIC;

	if (Pool->Attributes & POOL_ENABLE_EXTERNAL_LISTS)
		/* If external lists of allocated buffers are used, do not remove element from allocated list */
		quFreeBuf(Pool->quHead,Pool->quElements,Index); 
	else
		quFreeBufPut(Pool->quHead,Pool->quElements,Index); 
 
}

/******************************************************************************
*	Function:	 acBuffQueryPoolTotal
*
*	Description: Get the total number of buffers in a pool
*
*	Parameters:	 [I] unsigned int PoolId - the pool ID
*
*	Returns:	 int - The total number of buffers in a pool
*
*******************************************************************************/
int acBuffQueryPoolTotal(unsigned int PoolId)
{
  acBuffPoolDescriptor   *Pool;
  
  Pool = (acBuffPoolDescriptor*)PoolId;

	if(!AC_STD_OBJECT_VALIDATE(Pool))
	{
		acStdLib_ErrorHandler(RecoverableMsg,0,__FILE__,__LINE__,-1,
					   "acBuffQueryPoolTotal - Invalid Pool ID");
		return AC_STDLIB_ERROR; 
	}

  return Pool->TotalBuf;
}

/******************************************************************************
*	Function:	 acBuffQueryPoolFree
*
*	Description: Get the number of free buffers in a pool
*
*	Parameters:	 [I] unsigned int PoolId - the pool ID
*
*	Returns:	 int - The number of free buffers in a pool
*
*******************************************************************************/
int acBuffQueryPoolFree(unsigned int PoolId)
{
  acBuffPoolDescriptor   *Pool;
  
  Pool = (acBuffPoolDescriptor*)PoolId;

	if(!AC_STD_OBJECT_VALIDATE(Pool))
	{
		acStdLib_ErrorHandler(RecoverableMsg,0,__FILE__,__LINE__,-1,
					   "acBuffQueryPoolFree - Invalid Pool ID");
		return AC_STDLIB_ERROR; 
	}

  return quGetLen(Pool->quHead,QUEUE_EMPTY_INDEX);
}

/******************************************************************************
*	Function:	 acBuffQueryPoolAllocated
*
*	Description: Get the number of allocated buffers in a pool
*
*	Parameters:	 [I] unsigned int PoolId - the pool ID
*
*	Returns:	 int - The number of allocated buffers in a pool
*
*******************************************************************************/
int acBuffQueryPoolAllocated(unsigned int PoolId)
{
  acBuffPoolDescriptor   *Pool;
  
  Pool = (acBuffPoolDescriptor*)PoolId;

	if(!AC_STD_OBJECT_VALIDATE(Pool))
	{
		acStdLib_ErrorHandler(RecoverableMsg,0,__FILE__,__LINE__,-1,
					   "acBuffQueryPoolAllocated - Invalid Pool ID");
		return AC_STDLIB_ERROR; 
	}

  return Pool->TotalBuf - quGetLen(Pool->quHead,QUEUE_EMPTY_INDEX);
}

/******************************************************************************
*	Function:	 acBufferGetBufSize
*
*	Description: Get the max buffer size in a pool
*
*	Parameters:	 [I] unsigned int PoolId - the pool ID
*
*	Returns:	 int - The max buffer size in a pool or AC_STDLIB_ERROR (-1)
*
*******************************************************************************/
int acBufferGetBufSize(unsigned int PoolId)
{
  acBuffPoolDescriptor   *Pool;
  
  Pool = (acBuffPoolDescriptor*)PoolId;

	if(!AC_STD_OBJECT_VALIDATE(Pool))
	{
		acStdLib_ErrorHandler(RecoverableMsg,0,__FILE__,__LINE__,-1,
					   "acBufferGetBufSize - Invalid Pool ID");
		return AC_STDLIB_ERROR; 
	}

  return Pool->BuffSize;
}

/******************************************************************************
*	Function:	 acBuffGetListElements
*
*	Description: Get a pointer to a pool quElements
*
*	Parameters:	 [I] unsigned int PoolId - the pool ID
*
*	Returns:	 void * - pointer to a pool quElements or NULL
*
*******************************************************************************/
void *acBuffGetListElements(unsigned int PoolId)
{
  acBuffPoolDescriptor   *Pool;
  
  Pool = (acBuffPoolDescriptor*)PoolId;

  	if(!AC_STD_OBJECT_VALIDATE(Pool))
	{
		acStdLib_ErrorHandler(RecoverableMsg,0,__FILE__,__LINE__,-1,
					   "acBuffGetListElements - Invalid Pool ID");
		return NULL; 
	}

	return (void *)(Pool->quElements);
}


/******************************************************************************
*	Function:	 acBuffAuditPool
*
*	Description: Audit buffers of a pool for integrity and buffer leakage 
*
*	Parameters:	 [I] unsigned int PoolId - the pool ID
*
*	Returns:	
*
*******************************************************************************/
void acBuffAuditPool(acBuffPoolDescriptor *Pool)
{
  int Index;
  BufHdrRec        *Buff;
  int              MinValue,TotalTime=0;
  int              BusyBuffers;
  unsigned int     Terminator;
  
    	if(!AC_STD_OBJECT_VALIDATE(Pool))
	{
		acStdLib_ErrorHandler(RecoverableMsg,0,__FILE__,__LINE__,-1,
					   "acBuffAuditPool - Invalid Pool ID");
		return; 
	}


MinValue = Pool->AuditTime;

  // get first buffer index at busy queue
  Index = quFirstElem(Pool->quHead);

  while (Index != QUEUE_NULL_POINTER)
	{
	  Buff = (BufHdrRec *)Index2Ptr(Pool,Index);

	  if (Buff->AuditTime > Pool->AuditTime)
		acStdLib_ErrorHandler(FatalInLabMsg,
						 0,__FILE__,__LINE__,-1,
						 "Pool %s HEADER ERROR,Buf %x Application %d AuditTime %d, LR %x",
						 Pool->Name,Buff,Buff->Application,Buff->AuditTime,Buff->LR);
	  
	  else 
		{
		  Terminator = *(unsigned int*)((unsigned int)Buff + BUF_START_OFFSET + Pool->BuffSize);
		  if (Terminator != BUF_END_MAGIC)
			acStdLib_ErrorHandler(FatalInLabMsg,
								  0,__FILE__,__LINE__,-1,
								  "Pool %s TERMINATOR ERROR,Buf %x Application %d, LR %x, Terminator %x",
								  Pool->Name,Buff,Buff->Application,Buff->LR,Terminator);
		}

	  if (Pool->Attributes & POOL_AUDIT_TIME_MASK)
		{
		  if (Buff->AuditTime == 0)
			{
			  switch (Pool->Attributes & POOL_AUDIT_ACT_MASK)
				{
				case POOL_AUDIT_ACT_FREE:
				  acStdLib_ErrorHandler(DebugMsg,
								   0,__FILE__,__LINE__,-1,
								   "Pool %s Audit free Buf Application %d, LR %x",
								   Pool->Name,Buff->Application,Buff->LR);
				  
				  acBuffFree((unsigned int)Pool,(void*)((unsigned int)Buff + BUF_START_OFFSET));
				  break;
				  
				case POOL_AUDIT_ACT_ERROR:
				  acStdLib_ErrorHandler(RecoverableMsg,
								   0,__FILE__,__LINE__,-1,
								   "Pool %s Audit Not free Buf, Application %d, LR %x",
								   Pool->Name,Buff->Application,Buff->LR);
				  
				  break;
				}
			}
		
		  if (Buff->AuditTime < MinValue)
			MinValue = Buff->AuditTime;

		  TotalTime += Buff->AuditTime;
		  
		  Buff->AuditTime--; 
		}
	  
	  // get next buffer index at busy queue
	  Index = quNextElem(Pool->quHead, Pool->quElements,Index); 
	}
  
  BusyBuffers = quGetLen(Pool->quHead,QUEUE_MIN_PRIORITY);
  
  if (Pool->Attributes & POOL_AUDIT_TIME_MASK)
	{	  
	  Pool->BuffMaxTime = Pool->AuditTime - MinValue;

	  if (BusyBuffers > 0)
		Pool->BuffAverageTime = Pool->AuditTime - (TotalTime/BusyBuffers);
	  else
		Pool->BuffAverageTime = 0;	
	}

  if (Pool->BuffMaxUsage < BusyBuffers)
	Pool->BuffMaxUsage = BusyBuffers;
}

/******************************************************************************
*	Function:	 acBuffFree
*
*	Description: Is Buffer Pointer Valid
*
*	Parameters:	 [I] unsigned int PoolId - the pool ID
*                [I] void *Buff - Buffer pointer.
*
*	Returns:	 int - TRUE - Buffer pointer valid, FALSE - Buffer pointer not valid
*
*******************************************************************************/
int acBuffIsPtrValid(unsigned int PoolId, void *BufPtr)
{  
  int            	  rc=AC_STDLIB_OK;
  int      		  Index;
  BufHdrRec*          BufHdr;
  acBuffPoolDescriptor   *Pool;
  
  BufHdr = (BufHdrRec *)((unsigned int)BufPtr - BUF_START_OFFSET);
  
  Pool = (acBuffPoolDescriptor*)PoolId;
  
  Index = Ptr2Index(Pool,BufHdr);
  
  if (Index < 0 || Index >= (int)Pool->TotalBuf)
	rc = AC_STDLIB_ERROR;
  else
	if (((unsigned int)BufHdr - (unsigned int)Pool->StartAdr) % BUF_FULL_SIZE(Pool) != 0)
	  rc = AC_STDLIB_ERROR;

  return rc;
}

/******************************************************************************
*	Function:	 acBuffPtr2Index
*
*	Description: Get buffer index from its address
*
*	Parameters:	 [I] acBuffPoolDescriptor * Pool - the pool Descriptor
*                [I] void *Buff - Buffer pointer.
*
*	Returns:	 int - index of buffer at pool
*
*******************************************************************************/
extern int acBuffPtr2Index(acBuffPoolDescriptor *Pool, void *Buff)
{
	return Ptr2Index(Pool, Buff);
}

/******************************************************************************
*	Function:	 acBuffIndex2Ptr
*
*	Description: Get buffer address from its index
*
*	Parameters:	 [I] acBuffPoolDescriptor * Pool - the pool Descriptor
*                [I] int Index - Buffer index.
*
*	Returns:	 void * - Address of buffer
*
*******************************************************************************/
extern void *acBuffIndex2Ptr(acBuffPoolDescriptor *Pool, int Index)
{
	char *Ptr = (char *)Index2Ptr(Pool, Index);

	return (void *)(Ptr+BUF_START_OFFSET);
}

/******************************************************************************
*	Function:	 acBufferGetName
*
*	Description: Get buffer address from its index
*
*	Parameters:	 [I] unsigned int PoolId - the pool ID
*
*	Returns:	 char * - Pool name or NULL
*
*******************************************************************************/
char *acBufferGetName(unsigned int PoolID)
{
	acBuffPoolDescriptor	 *Pool = (acBuffPoolDescriptor*)PoolID;
	
	if(!AC_STD_OBJECT_VALIDATE(Pool))
	{
		acStdLib_ErrorHandler(RecoverableMsg,0,__FILE__,__LINE__,-1,
			"acBufferGetName - Invalid Pool ID");
		return NULL; 
	}
	
	return Pool->Name;
}

