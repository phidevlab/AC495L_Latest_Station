/*
* ----------------------------------------------------------------------------
* ----------------------------------------------------------------------------
* --                                                                        --
* --  ACStdLib - AudioCodes Standard Library - Copyright 2005 - AudioCodes  --
* --                                                                        --
* ----------------------------------------------------------------------------
* ----------------------------------------------------------------------------
* -- 
* --	File			:	acBufferList.h
* --
* --	Description		:	A linked list (FIFO/LIFO) of buffers from a buffer pool.
* --					
* --	Author			:	Eitan Richardson
* --	
* --	Limitations		:	None.
* --
* --	Known Problems	:	None.
* --	
* --	Notes			:   ** C++ users can use acBufferList.h - the C++ version. **
* --	
* --	                    acBufferList allocates and holds buffers from an
* --	                    acBufferPool object.
* --	
* --	                    Number of elements is limited to associated acBufferPool
* --	                    number of buffers.
* --	
* --	                    The associated acBufferPool must be initialized with the
* --	                    POOL_ENABLE_EXTERNAL_LISTS attribute.
* --	
* --	                    See usage example at end of this file. TBD
* --	
* --	Revision History:
* --	
* --	  - 12/4/05 - Eitan - First version
* --	
* ----------------------------------------------------------------------------
* ----------------------------------------------------------------------------
*/

#ifndef	_ACBUFFERLIST_H
#define	_ACBUFFERLIST_H


/******************************************************************************
*	Function:	 acBufferListCreate
*
*	Description: Create a new list (which will allocate buffers from PoolID)
*
*	Parameters:	 [I] unsigned int PoolID - The ID of the BufferPool to be used
*                [I] int ApplicationID - An ID to be used for debugging
*                [O] unsigned int *ListId - The ID of the newly created list
*
*	Returns:	 int - AC_STDLIB_OK (0) or AC_STDLIB_ERROR (-1)
*
*******************************************************************************/
extern int acBufferListCreate(unsigned int PoolID, int ApplicationID, unsigned int *ListId);

/******************************************************************************
*	Function:	 acBufferListDelete
*
*	Description: Delete the list
*
*	Parameters:	 [I] unsigned int ListId - the List ID
*
*	Returns:	 int - 
*
*******************************************************************************/
extern int acBufferListDelete(unsigned int ListId);

/******************************************************************************
*	Function:	 acBufferListAddFirst
*
*	Description: Allocate a buffer from the pool, add it to the top of the
*                list (push) and optionally initialize by copying from Source.
*
*	Parameters:	 [I] unsigned int ListId - the List ID
*                [I] void *Source - If not NULL - the source to copy from
*                [I] int Size - If >0 - how many bytes to copy
*
*	Returns:	 void * - Pointer to the new buffer or NULL
*
*******************************************************************************/
extern void *acBufferListAddFirst(unsigned int ListId, void *Source, int Size);

/******************************************************************************
*	Function:	 acBufferListAddLast
*
*	Description: Allocate a buffer from the pool, add it to the bottom 
*                of the list and optionally initialize by copying from Source.
*
*	Parameters:	 [I] unsigned int ListId - the List ID
*                [I] void *Source - If not NULL - the source to copy from
*                [I] int Size - If >0 - how many bytes to copy
*
*	Returns:	 void * - Pointer to the new buffer or NULL
*
*******************************************************************************/
extern void *acBufferListAddLast(unsigned int ListId, void *Source, int Size);

/******************************************************************************
*	Function:	 acBufferListInsertAfter
*
*	Description: Allocate a buffer from the pool, add after a specific location
*                in the list and optionally initialize by copying from Source.
*
*	Parameters:	 [I] unsigned int ListId - the List ID
*                [I] void *Previous - Pointer to the element to put the new one after
*                [I] void *Source - If not NULL - the source to copy from
*                [I] int Size - If >0 - how many bytes to copy
*
*	Returns:	 void * - Pointer to the new buffer or NULL
*
*******************************************************************************/
extern void *acBufferListInsertAfter(unsigned int ListId, void *Previous, void *Source, int Size);

/******************************************************************************
*	Function:	 acBufferListGetFirst
*
*	Description: Get a pointer to the first element (without removing it)
*
*	Parameters:	 [I] unsigned int ListId - the List ID
*
*	Returns:	 void * - pointer to first element or NULL
*
*******************************************************************************/
extern void *acBufferListGetFirst(unsigned int ListId);

/******************************************************************************
*	Function:	 acBufferListGetLast
*
*	Description: Get a pointer to the last element (without removing it)
*
*	Parameters:	 [I] unsigned int ListId - the List ID
*
*	Returns:	 void * - pointer to last element or NULL
*
*******************************************************************************/
extern void *acBufferListGetLast(unsigned int ListId);

/******************************************************************************
*	Function:	 acBufferListGetNext
*
*	Description: Gets the next element after a specific one (without removing it).
*
*	Parameters:	 [I] unsigned int ListId - the List ID
*                [I] void *Previous - Pointer to the previous element
*
*	Returns:	 void * - pointer to the required element or NULL
*
*******************************************************************************/
extern void *acBufferListGetNext(unsigned int ListId, void *Previous);

/******************************************************************************
*	Function:	 acBufferListPop
*
*	Description: Remove the first element from the list and free the buffer
*
*	Parameters:	 [I] unsigned int ListId - the List ID
*
*	Returns:	 int - AC_STDLIB_OK or AC_STDLIB_ERROR
*
*******************************************************************************/
extern int acBufferListPop(unsigned int ListId);

/******************************************************************************
*	Function:	 acBufferListRemove
*
*	Description: Remove a specific element and free the buffer.
*
*	Parameters:	 [I] unsigned int ListId - the List ID
*                [I] void *Buffer - Pointer to buffer to remove
*
*	Returns:	 int - AC_STDLIB_OK or AC_STDLIB_ERROR
*
*******************************************************************************/
extern int acBufferListRemove(unsigned int ListId, void *Buffer);

/******************************************************************************
*	Function:	 acBufferListRemoveAll
*
*	Description: Remove all elements from the list (empty the list) 
*
*	Parameters:	 [I] unsigned int ListId - the List ID
*
*	Returns:	 int - list size or AC_STDLIB_ERROR
*
*******************************************************************************/
int acBufferListRemoveAll(unsigned int ListId);

/******************************************************************************
*	Function:	 acBufferListGetSize
*
*	Description: Get the current number of elements in the list
*
*	Parameters:	 [I] unsigned int ListId - the List ID
*
*	Returns:	 int - list size or AC_STDLIB_ERROR
*
*******************************************************************************/
extern int acBufferListGetSize(unsigned int ListId);

#endif /* _ACBUFFERLIST_H */
