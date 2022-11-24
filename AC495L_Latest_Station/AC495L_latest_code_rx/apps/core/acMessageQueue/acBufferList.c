/*
* ----------------------------------------------------------------------------
* ----------------------------------------------------------------------------
* --                                                                        --
* --  ACStdLib - AudioCodes Standard Library - Copyright 2005 - AudioCodes  --
* --                                                                        --
* ----------------------------------------------------------------------------
* ----------------------------------------------------------------------------
* -- 
* --	File			:	acBufferList.c
* --
* --	Description		:	A linked list (FIFO/LIFO) of buffers from a buffer pool.
* --					
* --	Author			:	Eitan Richardson
* --	
* --	Limitations		:	None.
* --
* --	Known Problems	:	None.
* --	
* --	Notes			:   None.
* --	
* --	Revision History:
* --	
* --	  - 12/4/05 - Eitan - First version
* --	
* ----------------------------------------------------------------------------
* ----------------------------------------------------------------------------
*/

#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include "quLinkedQueue.h"
#include "acBufferList.h"
#include "acStdLibHal.h"
#include "acBufferListHal.h"
#include "acBufferPool.h"
#include "acBufferPoolHal.h"
#include "acStdLib.h"

/* Use some acBufferPool non-API functions */
extern int acBuffPtr2Index(acBuffPoolDescriptor *Pool, void *Buff);
extern void *acBuffIndex2Ptr(acBuffPoolDescriptor *Pool, int Index);
extern void *acBuffGetListElements(unsigned int PoolID);
extern char *acBufferGetName(unsigned int PoolID);

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
int acBufferListCreate(unsigned int PoolID, int ApplicationID, unsigned int *ListId)
{
	quTElementHead*		quElements;
	acBufferListDescriptor*	List;

	/* Allocate memory for the list descriptor */
	List = (acBufferListDescriptor *)malloc(sizeof(acBufferListDescriptor));

	if(List == NULL)
	{
		acStdLib_ErrorHandler(RecoverableMsg,0,__FILE__,__LINE__,-1,
				   "acBufferListCreate (%d) - No Memory.\n", ApplicationID );
		*ListId = 0;
		return AC_STDLIB_ERROR;
	}

	/* Get list queue elements */
	quElements = (quTElementHead *)acBuffGetListElements(PoolID);

	if( quElements == NULL )
	{
		acStdLib_ErrorHandler(RecoverableMsg,0,__FILE__,__LINE__,-1,
				   "acBufferListCreate (%d) - Invalid PoolID.\n", ApplicationID );

		free(List);
		*ListId = 0;
		return AC_STDLIB_ERROR;
	}

	List->quElements = quElements;
	List->MaxSize = acBuffQueryPoolTotal(PoolID);
	List->MaxBufSize = acBufferGetBufSize(PoolID);

	if(strlen(acBufferGetName(PoolID)) < sizeof(List->Name))
		strcpy(List->Name, acBufferGetName(PoolID));
	else
	{
		acStdLib_ErrorHandler(RecoverableMsg,0,__FILE__,__LINE__,-1,
				   "String length of acBufferGetName(%d) %d is equal or bigger then sizeof Name %d.\n", 
				   PoolID, strlen(acBufferGetName(PoolID)), sizeof(List->Name));

		free(List);
		*ListId = 0;
		return AC_STDLIB_ERROR;
	}

	List->MagicNumber = AC_STD_MAGIC;
	List->PoolId = PoolID;
	List->ApplicationID = ApplicationID;

	/* Init the queue without initializing the elements */
	QuInitQueue(0, 1, List->quHead, (quTElementHead *)NULL);

	*ListId = (unsigned int)List;
	return AC_STDLIB_OK;
}

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
int acBufferListDelete(unsigned int ListId)
{
	acBufferListDescriptor *List = (acBufferListDescriptor *)ListId;

	if(!AC_STD_OBJECT_VALIDATE(List))
	{
		acStdLib_ErrorHandler(RecoverableMsg,0,__FILE__,__LINE__,-1,
					   "acBufferListDelete - Invalid list ID.\n");
		return AC_STDLIB_ERROR;
	}

	/* Remove and free all elements */
	acBufferListRemoveAll(ListId);

	/* Free the list descriptor */
	free(List);

	return AC_STDLIB_OK;
}

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
void *acBufferListAddFirst(unsigned int ListId, void *Source, int Size)
{
	return acBufferListInsertAfter(ListId, NULL, Source, Size);
}

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
void *acBufferListAddLast(unsigned int ListId, void *Source, int Size)
{
	int Index;
    void *Buffer;
	acBufferListDescriptor *List = (acBufferListDescriptor *)ListId;

	if(!AC_STD_OBJECT_VALIDATE(List))
	{
		acStdLib_ErrorHandler(RecoverableMsg,0,__FILE__,__LINE__,-1,
					   "acBufferListAddLast - Invalid list ID.\n");
		return NULL;
	}
	
	Index = quLastElem(List->quHead,1);

	/* NirM 151106, Bug fix - Could not add last if list is empty */
/*	void *Buffer = acBuffIndex2Ptr( ((acBuffPoolDescriptor *)(List->PoolId)),Index);*/

	Buffer = (Index == QUEUE_NULL_POINTER) ? NULL : 
			acBuffIndex2Ptr( ((acBuffPoolDescriptor *)(List->PoolId)),Index);
			
	return acBufferListInsertAfter(ListId, Buffer, Source, Size);
}

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
void *acBufferListInsertAfter(unsigned int ListId, void *Previous, void *Source, int Size)
{
	int Iterator;
    char *buff;
    int Index;
	acBufferListDescriptor *List = (acBufferListDescriptor *)ListId;

	if(!AC_STD_OBJECT_VALIDATE(List))
	{
		acStdLib_ErrorHandler(RecoverableMsg,0,__FILE__,__LINE__,-1,
					   "acBufferListInsertAfter - Invalid list ID.\n");
		return NULL;
	}

	if(Size>(List->MaxBufSize))
	{
		acStdLib_ErrorHandler(RecoverableMsg,0,__FILE__,__LINE__,-1,
					   "acBufferListInsertAfter (%s/%d) - Size is %d and cannot exceed %d.\n",
					   List->Name, List->ApplicationID, Size, List->MaxBufSize);
		return NULL;
	}

	if( (Size>0) && (Source == NULL) )
	{
		acStdLib_ErrorHandler(RecoverableMsg,0,__FILE__,__LINE__,-1,
					   "acBufferListInsertAfter (%s/%d) - NULL source pointer.\n",
					   List->Name, List->ApplicationID);
		return NULL;
	}
	

	if(Previous == NULL)
		Iterator = QUEUE_NULL_POINTER;
	else
		Iterator = acBuffPtr2Index(((acBuffPoolDescriptor *)(List->PoolId)),Previous);

	if( (Iterator < (-1)) || (Iterator>(List->MaxSize)) )
	{
		acStdLib_ErrorHandler(RecoverableMsg,0,__FILE__,__LINE__,-1,
					   "acBufferListInsertAfter (%s/%d) - Invalid previous pointer.\n",
					   List->Name, List->ApplicationID);
		return NULL;
	}

	buff = (char *)acBuffAlloc(List->PoolId,List->ApplicationID);

	if(buff == NULL)
	{
		acStdLib_ErrorHandler(RecoverableMsg,0,__FILE__,__LINE__,-1,
					   "acBufferListInsertAfter (%s/%d) - Could not allocate a new buffer.\n",
					   List->Name, List->ApplicationID);
		return NULL;
	}

	// Initialize the buffer
	if(Size>0)
		memcpy(buff,Source,Size);

	Index = acBuffPtr2Index( ((acBuffPoolDescriptor *)(List->PoolId)) ,buff);

	// Add the buffer index to the list
	if( quInsertAfter(List->quHead,List->quElements,Iterator,Index, 1 /*instead of default value*/) != QUEUE_OK)
	{
		acStdLib_ErrorHandler(RecoverableMsg,0,__FILE__,__LINE__,-1,
					   "acBufferListInsertAfter (%s/%d) - Could not add buffer to list.\n",
					   List->Name, List->ApplicationID);
		// Free the buffer
		acBuffFree(List->PoolId, buff);
		return NULL;
	}

	return (void *)buff;
}

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
void *acBufferListGetFirst(unsigned int ListId)
{
	int Index;
	acBufferListDescriptor *List = (acBufferListDescriptor *)ListId;

	if(!AC_STD_OBJECT_VALIDATE(List))
	{
		acStdLib_ErrorHandler(RecoverableMsg,0,__FILE__,__LINE__,-1,
					   "acBufferListGetFirst - Invalid list ID");
		return NULL;
	}

	Index = quFirstElem(List->quHead);

	if(Index != QUEUE_NULL_POINTER)
		return acBuffIndex2Ptr( ((acBuffPoolDescriptor *)(List->PoolId)),Index);

	return NULL;
}

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
void *acBufferListGetLast(unsigned int ListId)
{
	int Index;
	acBufferListDescriptor *List = (acBufferListDescriptor *)ListId;

	if(!AC_STD_OBJECT_VALIDATE(List))
	{
		acStdLib_ErrorHandler(RecoverableMsg,0,__FILE__,__LINE__,-1,
					   "acBufferListGetLast - Invalid list ID");
		return NULL;
	}

	Index = quLastElem(List->quHead,1);

	if(Index != QUEUE_NULL_POINTER)
		return acBuffIndex2Ptr( ((acBuffPoolDescriptor *)(List->PoolId)),Index);

	return NULL;
}

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
void *acBufferListGetNext(unsigned int ListId, void *Previous)
{
	int Iterator;
    int Index;
	acBufferListDescriptor *List = (acBufferListDescriptor *)ListId;

	if(!AC_STD_OBJECT_VALIDATE(List))
	{
		acStdLib_ErrorHandler(RecoverableMsg,0,__FILE__,__LINE__,-1,
					   "acBufferListGetLast - Invalid list ID");
		return NULL;
	}

	Iterator = acBuffPtr2Index(((acBuffPoolDescriptor *)(List->PoolId)),Previous);

	if( (Iterator < 0) || (Iterator>List->MaxSize) )
	{
		acStdLib_ErrorHandler(RecoverableMsg,0,__FILE__,__LINE__,-1,
					   "acBufferListGetNext (%s/%d) - Invalid pointer.\n",
					   List->Name, List->ApplicationID);
		return NULL;
	}

	Index = quNextElem(List->quHead, List->quElements, Iterator);
	
	if(Index != QUEUE_NULL_POINTER)
		return acBuffIndex2Ptr( ((acBuffPoolDescriptor *)(List->PoolId)),Index);

	return NULL;
}



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
int acBufferListPop(unsigned int ListId)
{
	int Iterator;
	acBufferListDescriptor *List = (acBufferListDescriptor *)ListId;

	if(!AC_STD_OBJECT_VALIDATE(List))
	{
		acStdLib_ErrorHandler(RecoverableMsg,0,__FILE__,__LINE__,-1,
					   "acBufferListRemove - Invalid list ID.\n");
		return AC_STDLIB_ERROR;
	}

	/* Find the first element */
	Iterator = quFirstElem(List->quHead);

	/* Check if list is empty (no need to report an error...) */
	if(Iterator == QUEUE_NULL_POINTER)
		return AC_STDLIB_ERROR;

	/* Free this element from the list */
	quFreeFromList(List->quHead,List->quElements,Iterator);

	/* Free the buffer */
	acBuffFree(List->PoolId, acBuffIndex2Ptr(((acBuffPoolDescriptor *)(List->PoolId)),Iterator));

	return AC_STDLIB_OK;
}

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
int acBufferListRemove(unsigned int ListId, void *Buffer)
{
	int Iterator;
	acBufferListDescriptor *List = (acBufferListDescriptor *)ListId;

	if(!AC_STD_OBJECT_VALIDATE(List))
	{
		acStdLib_ErrorHandler(RecoverableMsg,0,__FILE__,__LINE__,-1,
					   "acBufferListRemove - Invalid list ID.\n");
		return AC_STDLIB_ERROR;
	}

	Iterator = acBuffPtr2Index(((acBuffPoolDescriptor *)(List->PoolId)),Buffer);

	if( (Iterator < 0) || (Iterator>List->MaxSize) )
	{
		acStdLib_ErrorHandler(RecoverableMsg,0,__FILE__,__LINE__,-1,
					   "acBufferListRemove (%s/%d) - Invalid pointer.\n",
					   List->Name, List->ApplicationID);
		return AC_STDLIB_ERROR;
	}

	quFreeFromList(List->quHead,List->quElements,Iterator);

	acBuffFree(List->PoolId, acBuffIndex2Ptr(((acBuffPoolDescriptor *)(List->PoolId)),Iterator));
	return AC_STDLIB_OK;
}

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
int acBufferListRemoveAll(unsigned int ListId)
{
	void *Ptr;
	acBufferListDescriptor *List = (acBufferListDescriptor *)ListId;

	if(!AC_STD_OBJECT_VALIDATE(List))
	{
		acStdLib_ErrorHandler(RecoverableMsg,0,__FILE__,__LINE__,-1,
					   "acBufferListRemove - Invalid list ID");
		return AC_STDLIB_ERROR;
	}
/* NirM  151106, changed due to compilation error: `for' loop initial declaration used outside C99 mode. */	
#if 0
	/* Browse the list and remove each element */
	for(Ptr = acBufferListGetFirst(ListId) ; Ptr != NULL ;
		Ptr = acBufferListGetNext(ListId, Ptr) )
	{
		acBufferListRemove(ListId, Ptr);
	}
#endif

	Ptr = acBufferListGetFirst(ListId); 
	/* Browse the list and remove each element */
	for(; Ptr != NULL ;	Ptr = acBufferListGetNext(ListId, Ptr) )
	{
		acBufferListRemove(ListId, Ptr);
	}

	return AC_STDLIB_OK;
}

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
int acBufferListGetSize(unsigned int ListId)
{
	acBufferListDescriptor *List = (acBufferListDescriptor *)ListId;

	if(!AC_STD_OBJECT_VALIDATE(List))
	{
		acStdLib_ErrorHandler(RecoverableMsg,0,__FILE__,__LINE__,-1,
					   "acBufferListGetSize - Invalid list ID");
		return AC_STDLIB_ERROR;
	}

	return quGetLen(List->quHead,1);
}

