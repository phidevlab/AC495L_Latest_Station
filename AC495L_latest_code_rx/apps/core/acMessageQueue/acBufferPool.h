/*
* ----------------------------------------------------------------------------
* ----------------------------------------------------------------------------
* --                                                                        --
* --  ACStdLib - AudioCodes Standard Library - Copyright 2005 - AudioCodes  --
* --                                                                        --
* ----------------------------------------------------------------------------
* ----------------------------------------------------------------------------
* -- 
* --	File			:	acBufferPool.h
* --
* --	Description		:	A pool of equal-size buffers.
* --					
* --	Author			:	Itzik Ben-Dov
* --	
* --	Limitations		:	None.
* --
* --	Known Problems	:	None.
* --	
* --	Notes			:   Memory for the entire pool is allocated during
* --	                      initialization (from dynamic or static heaps).
* --	                    The pool is registered automatically to provide:
* --	                      - Command-Shell debugging
* --	                      - Auditing (for pool integrity and buffer leakage)
* --	                      - Performance monitoring
* --	                    The pool reports on the following error conditions:
* --	                      - Buffer overrun (write past end of buffer)
* --	                      - Deleteing a pool with allocated buffers
* --	                      - Freeing an invalid pointer
* --	                    See usage example at the end of this file. TBD
* --	
* --	Revision History:
* --	
* --	  - 12/4/05 - Itzik - First version
* --	
* ----------------------------------------------------------------------------
* ----------------------------------------------------------------------------
*/
#ifndef	_ACBUFFERPOOL_H
#define	_ACBUFFERPOOL_H

#include "acStdLib.h"

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef __size_t
#define	__size_t
typedef unsigned int size_t;
#endif

  
/******************************************************************************
* 				MACRO															 
*******************************************************************************/
  
/******************************************************************************
* 				Pool Attributes
*
*	POOL_AUDIT_TIME
*	POOL_AUDIT_ACT
*	POOL_DELETE
*******************************************************************************/

/******************************************************************************
*	POOL_AUDIT_TIME
*
*	Description: Audit buffers free after life time 
*
*	POOL_AUDIT_TIME_DISABLE -  Do not audit buffers free
*	POOL_AUDIT_TIME_SECOND  -  Audit buffers free - Buffer life time 1 second
*	POOL_AUDIT_TIME_MINUTE  -  Audit buffers free - Buffer life time 1 minute
*	POOL_AUDIT_TIME_HOUR    -  Audit buffers free - Buffer life time 1 hour
*
*******************************************************************************/
#define POOL_AUDIT_TIME_DISABLE   0x00000000
#define POOL_AUDIT_TIME_SECOND    0x00000001
#define POOL_AUDIT_TIME_MINUTE    0x00000002
#define POOL_AUDIT_TIME_HOUR      0x00000003
  
/******************************************************************************
*	POOL_AUDIT_ACT
*
*	Description: What audit do if buffer life time expired
*
*	POOL_AUDIT_ACT_FREE  -  Free the buffer, print error and maintain statistic
*	POOL_AUDIT_ACT_ERROR -  Print error and maintain statistic
*	POOL_AUDIT_ACT_STATISTIC  - Only maitain statistic
*
*******************************************************************************/
#define POOL_AUDIT_ACT_FREE       0x00000004
#define POOL_AUDIT_ACT_ERROR      0x00000008
#define POOL_AUDIT_ACT_STATISTIC  0x00000010
  
/******************************************************************************
*	POOL_DELETE
*
*	Description: Enable deletion of a pool
*
*	POOL_DELETE_ENABLE  -  Enable deletion of a pool
*	POOL_DELETE_DISABLE -  Disable deletion of a pool, pool life for ever
*
*******************************************************************************/
#define POOL_DELETE_ENABLE        0x00000020
#define POOL_DELETE_DISABLE       0x00000000
  
/******************************************************************************
*	POOL_DELETE limitation
*
*	Description: Pool deletion limitatiom (only if POOL_DELETE_ENABLE)
*
*	POOL_DELETE_FORCED  -  No limitation
*	POOL_DELETE_IF_EMPTY - Poll deletion is possible only if all its buffers free.
*
*******************************************************************************/
#define POOL_DELETE_FORCED        0x00000040
#define POOL_DELETE_IF_EMPTY      0x00000000
  
/******************************************************************************
*	POOL_EXTERNAL_LISTS
*
*	Description: Use external lists of buffers from pool
*
*	POOL_ENABLE_EXTERNAL_LISTS  - Enable external lists
*	POOL_NO_EXTERNAL_LISTS      - Disable external lists
*
*******************************************************************************/
#define POOL_ENABLE_EXTERNAL_LISTS	0x00000080  
#define POOL_NO_EXTERNAL_LISTS		0x00000000

  
/******************************************************************************
* 				FUNCTIONS													 
*******************************************************************************/
  
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
extern int   acBuffCreatePool(char PoolName[AC_STDLIB_MAX_OBJECT_NAME], unsigned int NumBuff, unsigned int BuffSize,
							  unsigned int Attributes, unsigned int *PoolId);
  
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
extern int   acBuffDeletePool(unsigned int PoolId);

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
extern void *acBuffAlloc(unsigned int PoolId,unsigned int ApplicationID);  

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
extern void  acBuffFree(unsigned int PoolId, void *Buff);

/******************************************************************************
*	Function:	 acBuffIsPtrValid
*
*	Description: Is Buffer Pointer Valid
*
*	Parameters:	 [I] unsigned int PoolId - the pool ID
*                [I] void *Buff - Buffer pointer.
*
*	Returns:	 int - TRUE - Buffer pointer valid, FALSE - Buffer pointer not valid
*
*******************************************************************************/
extern int acBuffIsPtrValid(unsigned int PoolId, void *Buff);

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
extern int acBuffQueryPoolTotal(unsigned int PoolId);

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
extern int acBuffQueryPoolAllocated(unsigned int PoolId);

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
extern int acBuffQueryPoolFree(unsigned int PoolId);

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
extern int acBufferGetBufSize(unsigned int PoolId);

#ifdef	__cplusplus
}
#endif

#endif /* _ACBUFFERPOOL_H */

