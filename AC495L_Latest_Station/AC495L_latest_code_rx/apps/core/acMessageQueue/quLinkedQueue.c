/*
* ----------------------------------------------------------------------------
* --							Copyright
* ----------------------------------------------------------------------------
* --
* --	Copyright 2000  audiocodes
* --
* ----------------------------------------------------------------------------
* ----------------------------------------------------------------------------
* -- 
* --	File			:	quLinkedQueue.c
* --
* --
* --	Description		:	Logical queue, priority based. Can be used also 
* --                        for linked list. The elements can be chained in
* --                        empty list or in a priority list.	
* --					
* --	Related Docs	: 
* -- 
* --	Author			:	Nurit Shenhar
* --	
* --	Limitations		:	Up to 2**16 elements in the queue.
* --                        Up to 10 priority values (0-9)
* --                        The queue is not multi-task safe.
* --
* --	Known Problems	:	None.
* --	
* --	Notes			:
* --	
* ----------------------------------------------------------------------------
* ----------------------------------------------------------------------------
*/
#ifdef __MG_MODULE_WIN_BUILD__
#include "stdafx.h"
#include "mgModule.h"
#endif

#include "quLinkedQueue.h"

/******************************************************************************
*	Function:	 QuInitQueue
*
*	Description: Initialize the queue header, the empty list and the 
*                priority lists. All the elements are chained to the empty
*                list (index 0).
*
*	Parameters:  [I] int            NumElements - Number of elements in queue.
*                [I] int            MaxPriority - Highest prioriry. Should be
*                                                 one less than the length of
*                                                 the next QuHead array!!!!.
*                [I] quTListHeaderEntry *QuHead - queue header array
*                [I] quTElementHead *QuElements - Points to per element header
*
*	Returns:     TRUE if success, else FALSE. FALSE means that number of elements
*                is too large for the queue (more than signed short).
*
*******************************************************************************/
quTBool QuInitQueue(int NumElements, int MaxPriority, quTListHeaderEntry *QuHead,
                quTElementHead *QuElements)
{
    quTElementHead *ElementHeader;
    int            i;

    QuHead[QUEUE_EMPTY_INDEX].PriorityListLen = NumElements;
    QuHead[QUEUE_EMPTY_INDEX].PriorityFirst   = 0;
    QuHead[QUEUE_EMPTY_INDEX].PriorityLast    = NumElements - 1;
    QuHead[QUEUE_EMPTY_INDEX].NextPriority    = 1;

    // Initialize priority headers
    for (i = QUEUE_MIN_PRIORITY ; i <= MaxPriority ; i++)
    {
        QuHead[i].PriorityListLen = 0;
        QuHead[i].PriorityFirst = QUEUE_NULL_POINTER;
        QuHead[i].PriorityLast = QUEUE_NULL_POINTER;
        QuHead[i].NextPriority = i+1;
    }

    QuHead[MaxPriority].NextPriority = QUEUE_NULL_POINTER;

    for (i = 0 ; i < NumElements ; i++)
    {
        ElementHeader = &QuElements[i];
        if (i == NumElements - 1)
            ElementHeader->Next = QUEUE_NULL_POINTER;
        else
            ElementHeader->Next     = i + 1;
        ElementHeader->Prev     = i - 1;
        ElementHeader->MyIndx   = i;
        ElementHeader->Priority = QUEUE_EMPTY_INDEX;
    }

    return QUEUE_TRUE;
}

/******************************************************************************
*	Function:	 QuNotEmpty
*
*	Description: Checks if there is anything in the queue 
*
*	Parameters:	 [I] quTListHeaderEntry *QuHead - Points to the queue header.
*
*	Returns:	 quTBool - QUEUE_TRUE or QUEUE_FALSE
*
*******************************************************************************/
quTBool quNotEmpty(quTListHeaderEntry *QuHead)
{
    int i = QUEUE_MIN_PRIORITY;

    while (i != QUEUE_NULL_POINTER)
    {
        if (QuHead[i].PriorityListLen > 0)
            return QUEUE_TRUE;
        i = QuHead[i].NextPriority;
    }
    return QUEUE_FALSE;
}

/******************************************************************************
*	Function:	 QuIsFull
*
*	Description: Checks if the queue is full
*
*	Parameters:	 [I] quTListHeaderEntry *QuHead - Points to the queue header.
*
*	Returns:	 quTBool - QUEUE_TRUE or QUEUE_FALSE
*
*******************************************************************************/
quTBool quIsFull(quTListHeaderEntry *QuHead)
{
    if (QuHead[QUEUE_EMPTY_INDEX].PriorityListLen == 0 || QuHead[QUEUE_EMPTY_INDEX].PriorityFirst == QUEUE_NULL_POINTER)
        return QUEUE_TRUE;

    return QUEUE_FALSE;
}

/******************************************************************************
*	Function:	 QuGet
*
*	Description: Gets the next highest priority element index. 
*
*	Parameters:	 [I] quTListHeaderEntry *QuHead - Points to the queue header
*                [I] quTElementHead *QuElements - Points to per element header
*                [O] int           *ElementIndx - Points to the returned index
*
*	Returns:     int - QUEUE_OK or QUEUE_EMPTY. 
*
*******************************************************************************/
int quGet(quTListHeaderEntry *QuHead, quTElementHead *QuElements, 
          int *ElementIndx)
{
    int            i = QUEUE_MIN_PRIORITY;
    quTElementHead *ElementHeader;

    while (i != QUEUE_NULL_POINTER)
    {
        if (QuHead[i].PriorityListLen > 0)
        {
            *ElementIndx = QuHead[i].PriorityFirst;
            ElementHeader = &QuElements[*ElementIndx];

            // Take this buffer out of the priority list
            QuHead[i].PriorityFirst = ElementHeader->Next;
            if (ElementHeader->Next != QUEUE_NULL_POINTER)
            {
                QuElements[ElementHeader->Next].Prev = QUEUE_NULL_POINTER;
            }
            ElementHeader->Next = QUEUE_NULL_POINTER;
            QuHead[i].PriorityListLen--;
            
            // Return the element address
            return QUEUE_OK;
        }
        
        i = QuHead[i].NextPriority ;
    }

    // Return NULL address
    *ElementIndx = QUEUE_NULL_POINTER;
    return QUEUE_EMPTY;
}

/******************************************************************************
*	Function:	 QuPut
*
*	Description: Gets the next highest priority element address. 
*
*	Parameters : [I] quTListHeaderEntry *QuHead - Points to the queue header
*                [I] quTElementHead *QuElements - Points to per element header
*                [I] int            ElementIndx - Index to put in queue
*                [I] int     Priority (optional)- The element priority (1 to
*                                                 MaxPriority from InitQueue)
*
*	Returns:     int - QUEUE_OK or QUEUE_FULL. 
*
*******************************************************************************/
int quPut(quTListHeaderEntry *QuHead, quTElementHead *QuElements, 
          int ElementIndx, int Priority)
{
    quTElementHead *ElementHeader, *TmpElementHeader;

//    if (quIsFull(QuHead))
//        return QUEUE_FULL;

    ElementHeader           = &QuElements[ElementIndx];
    ElementHeader->Priority = Priority;
 
    if (QuHead[Priority].PriorityListLen == 0)
    {
        ElementHeader->Prev = ElementHeader->Next = QUEUE_NULL_POINTER;

        QuHead[Priority].PriorityFirst = ElementIndx;
        QuHead[Priority].PriorityLast  = ElementIndx;
    }
    else
    {
        if (QuHead[Priority].PriorityFirst ==  QUEUE_NULL_POINTER)
            return QUEUE_FULL;
        TmpElementHeader = 
            &QuElements[QuHead[Priority].PriorityLast];
        TmpElementHeader->Next = ElementIndx;
        ElementHeader->Prev = QuHead[Priority].PriorityLast;
        ElementHeader->Next = QUEUE_NULL_POINTER;
        QuHead[Priority].PriorityLast = ElementIndx;
    }

    QuHead[Priority].PriorityListLen++;
    return QUEUE_OK;

}

/******************************************************************************
*	Function:	 QuAllocBuf
*
*	Description: Allocates an empty buffer. 
*
*	Parameters:	 [I] quTListHeaderEntry *QuHead - Points to the queue header
*                [I] quTElementHead *QuElements - Points to per element header
*                [O] int           *ElementIndx - Points to the returned index
*
*	Returns:     int - QUEUE_OK or QUEUE_FULL. 
*
*******************************************************************************/
int quAllocBuf(quTListHeaderEntry *QuHead, quTElementHead *QuElements, 
               int *ElementIndx)
{
    quTElementHead *ElementHeader, *TmpElementHeader;

    if (quIsFull(QuHead))
    {
        *ElementIndx = QUEUE_NULL_POINTER;
        return QUEUE_FULL;
    }

    // Get a buffer from the empty list
    *ElementIndx  = QuHead[QUEUE_EMPTY_INDEX].PriorityFirst;
    ElementHeader = &QuElements[*ElementIndx];

    if (ElementHeader->Next == QUEUE_NULL_POINTER)
    {
        QuHead[QUEUE_EMPTY_INDEX].PriorityFirst = QUEUE_NULL_POINTER;
        QuHead[QUEUE_EMPTY_INDEX].PriorityLast  = QUEUE_NULL_POINTER;
    }
    else
    {
        QuHead[QUEUE_EMPTY_INDEX].PriorityFirst = ElementHeader->Next;
        TmpElementHeader = &QuElements[ElementHeader->Next];
        TmpElementHeader->Prev = QUEUE_NULL_POINTER;
    }

    ElementHeader->Next = ElementHeader->Prev = QUEUE_NULL_POINTER;
    QuHead[QUEUE_EMPTY_INDEX].PriorityListLen--;

    return QUEUE_OK;
}

/******************************************************************************
*	Function:	 quAllocPutBuf
*
*	Description: Allocates an empty buffer, Put it on busy list. 
*
*	Parameters:	 [I] quTListHeaderEntry *QuHead - Points to the queue header
*                [I] quTElementHead *QuElements - Points to per element header
*                [O] int           *ElementIndx - Points to the returned index
*
*	Returns:     int - QUEUE_OK or QUEUE_FULL. 
*
*******************************************************************************/
int quAllocPutBuf(quTListHeaderEntry *QuHead, quTElementHead *QuElements, 
               int *ElementIndx, int Priority)
{
  quTElementHead *ElementHeader, *TmpElementHeader;
  int Index;

    if (QuHead[QUEUE_EMPTY_INDEX].PriorityListLen == 0 || QuHead[QUEUE_EMPTY_INDEX].PriorityFirst == QUEUE_NULL_POINTER)
    {
        *ElementIndx = QUEUE_NULL_POINTER;
        return QUEUE_FULL;
    }

    // Get a buffer from the empty list
    Index  = QuHead[QUEUE_EMPTY_INDEX].PriorityFirst;
    ElementHeader = &QuElements[Index];

    if (ElementHeader->Next == QUEUE_NULL_POINTER)
    {
        QuHead[QUEUE_EMPTY_INDEX].PriorityFirst = QUEUE_NULL_POINTER;
        QuHead[QUEUE_EMPTY_INDEX].PriorityLast  = QUEUE_NULL_POINTER;
    }
    else
    {
        QuHead[QUEUE_EMPTY_INDEX].PriorityFirst = ElementHeader->Next;
        TmpElementHeader = &QuElements[ElementHeader->Next];
        TmpElementHeader->Prev = QUEUE_NULL_POINTER;
    }

    ElementHeader->Next = ElementHeader->Prev = QUEUE_NULL_POINTER;
    QuHead[QUEUE_EMPTY_INDEX].PriorityListLen--;

    ElementHeader->Priority = Priority;
	
    if (QuHead[Priority].PriorityListLen == 0)
    {
        ElementHeader->Prev = ElementHeader->Next = QUEUE_NULL_POINTER;

        QuHead[Priority].PriorityFirst = Index;
        QuHead[Priority].PriorityLast  = Index;
    }
    else
    {
        if (QuHead[Priority].PriorityFirst ==  QUEUE_NULL_POINTER)
            return QUEUE_FULL;
        TmpElementHeader = 
            &QuElements[QuHead[Priority].PriorityLast];
        TmpElementHeader->Next = Index;
        ElementHeader->Prev = QuHead[Priority].PriorityLast;
        ElementHeader->Next = QUEUE_NULL_POINTER;
        QuHead[Priority].PriorityLast = Index;
    }

    QuHead[Priority].PriorityListLen++;

	*ElementIndx = Index;
	
    return QUEUE_OK;
}

/******************************************************************************
*	Function:	 QuFreeBuf
*
*	Description: Frees an unused buffer. 
*
*	Parameters:	 [I] quTListHeaderEntry  *QuHead - Points to the queue header
*                [I] quTElementHead  *QuElements - Points to per element header
*                [I] int             ElementIndx - Index to be freed
*
*
*	Returns:	 None.
*
*******************************************************************************/
void quFreeBuf(quTListHeaderEntry *QuHead, quTElementHead *QuElements, 
               int ElementIndx)
{
    quTElementHead *ElementHeader, *TmpElementHeader;

    ElementHeader    = &QuElements[ElementIndx];

    // Add the buffer to the empty list
    ElementHeader->Prev = QuHead[QUEUE_EMPTY_INDEX].PriorityLast;
    ElementHeader->Next = QUEUE_NULL_POINTER;
    if (QuHead[QUEUE_EMPTY_INDEX].PriorityLast != QUEUE_NULL_POINTER)
    {
        TmpElementHeader    = 
            &QuElements[QuHead[QUEUE_EMPTY_INDEX].PriorityLast];
        TmpElementHeader->Next = ElementIndx;
    }
    else
    {
        QuHead[QUEUE_EMPTY_INDEX].PriorityFirst = ElementIndx;
    }

    QuHead[QUEUE_EMPTY_INDEX].PriorityLast = ElementIndx;
    QuHead[QUEUE_EMPTY_INDEX].PriorityListLen++;
    ElementHeader->Priority = QUEUE_EMPTY_INDEX;
    
}

/******************************************************************************
*	Function:	 quFreeBufPut
*
*	Description: Frees an unused buffer, Put it on empty list
*
*	Parameters:	 [I] quTListHeaderEntry  *QuHead - Points to the queue header
*                [I] quTElementHead  *QuElements - Points to per element header
*                [I] int             ElementIndx - Index to be freed
*
*
*	Returns:	 None.
*
*******************************************************************************/
void quFreeBufPut(quTListHeaderEntry *QuHead, quTElementHead *QuElements, 
               int ElementIndx)
{
  quTElementHead *ElementHeader, *TmpElementHeader;
  int Priority;

    ElementHeader = &QuElements[ElementIndx];

	Priority = ElementHeader->Priority;
	
    if (QuHead[Priority].PriorityFirst == 
        ElementIndx)
    {
        // First in priority
        QuHead[Priority].PriorityFirst = 
            ElementHeader->Next;
        if (ElementHeader->Next != QUEUE_NULL_POINTER)
            QuElements[ElementHeader->Next].Prev = QUEUE_NULL_POINTER;

    }
    else
    {
        QuElements[ElementHeader->Prev].Next = ElementHeader->Next;
    }

    if (QuHead[Priority].PriorityLast == 
        ElementIndx)
    {
        // Last in priority
        QuHead[Priority].PriorityLast = 
            ElementHeader->Prev;
        if (ElementHeader->Prev != QUEUE_NULL_POINTER)
            QuElements[ElementHeader->Prev].Next = QUEUE_NULL_POINTER;
    }
    else
    {
        QuElements[ElementHeader->Next].Prev = ElementHeader->Prev;
    }

    QuHead[Priority].PriorityListLen--;

    // Add the buffer to the empty list
    ElementHeader->Prev = QuHead[QUEUE_EMPTY_INDEX].PriorityLast;
    ElementHeader->Next = QUEUE_NULL_POINTER;
    if (QuHead[QUEUE_EMPTY_INDEX].PriorityLast != QUEUE_NULL_POINTER)
    {
        TmpElementHeader    = 
            &QuElements[QuHead[QUEUE_EMPTY_INDEX].PriorityLast];
        TmpElementHeader->Next = ElementIndx;
    }
    else
    {
        QuHead[QUEUE_EMPTY_INDEX].PriorityFirst = ElementIndx;
    }

    QuHead[QUEUE_EMPTY_INDEX].PriorityLast = ElementIndx;
    QuHead[QUEUE_EMPTY_INDEX].PriorityListLen++;
    ElementHeader->Priority = QUEUE_EMPTY_INDEX;
    
}

/******************************************************************************
*	Function:	 quFreeFromList
*
*	Description: Frees element from list. (Used for linked list)
*
*	Parameters:	 [I] quTListHeaderEntry *QuHead - Points to the queue header.
*                [I] quTElementHead *QuElements - Points to per element header
*                [I] int            ElementIndx - Index of element in list
*
*	Returns:	 None
*
*******************************************************************************/
void quFreeFromList(quTListHeaderEntry *QuHead, quTElementHead *QuElements, 
          int ElementIndx)
{
    quTElementHead *ElementHeader;

    ElementHeader = &QuElements[ElementIndx];

    if (QuHead[ElementHeader->Priority].PriorityFirst == 
        ElementIndx)
    {
        // First in priority
        QuHead[ElementHeader->Priority].PriorityFirst = 
            ElementHeader->Next;
        if (ElementHeader->Next != QUEUE_NULL_POINTER)
            QuElements[ElementHeader->Next].Prev = QUEUE_NULL_POINTER;

    }
    else
    {
        QuElements[ElementHeader->Prev].Next = ElementHeader->Next;
    }

    if (QuHead[ElementHeader->Priority].PriorityLast == 
        ElementIndx)
    {
        // Last in priority
        QuHead[ElementHeader->Priority].PriorityLast = 
            ElementHeader->Prev;
        if (ElementHeader->Prev != QUEUE_NULL_POINTER)
            QuElements[ElementHeader->Prev].Next = QUEUE_NULL_POINTER;
    }
    else
    {
        QuElements[ElementHeader->Next].Prev = ElementHeader->Prev;
    }

    QuHead[ElementHeader->Priority].PriorityListLen--;

}

/******************************************************************************
*	Function:	 quFirstElem
*
*	Description: Get index of first element in list. (Used for linked list)
*
*	Parameters:	 [I] quTListHeaderEntry *QuHead  - Points to the queue header.
*
*	Returns:	 int - Element index or QUEUE_NULL_POINTER
*
*******************************************************************************/
int quFirstElem(quTListHeaderEntry *QuHead)
{
    int            i = QUEUE_MIN_PRIORITY;

    while (i != QUEUE_NULL_POINTER)
    {
        if (QuHead[i].PriorityListLen > 0)
        {
            return (QuHead[i].PriorityFirst);
        }

        i = QuHead[i].NextPriority ;
    }

    return QUEUE_NULL_POINTER;
}

/******************************************************************************
*	Function:	 quLastElem
*
*	Description: Get index of last element in list. (in a specific priority)
*
*	Parameters:	 [I] quTListHeaderEntry *QuHead  - Points to the queue header.
*                [I] int Priority - The priority number 
*
*	Returns:	 int - Element index or QUEUE_NULL_POINTER
*
*******************************************************************************/
int quLastElem(quTListHeaderEntry *QuHead, int Priority)
{
	if (QuHead[Priority].PriorityListLen > 0)
		return (QuHead[Priority].PriorityLast);

    return QUEUE_NULL_POINTER;
}


/******************************************************************************
*	Function:	 quNextElem
*
*	Description: Get index of next element in list. (Used for linked list)
*
*	Parameters:	 [I] quTListHeaderEntry *QuHead - Points to the queue header.
*                [I] quTElementHead *QuElements - Points to per element header
*                [I] int            ElementIndx - Index of current element
*
*	Returns:     int - Next element index or QUEUE_NULL_POINTER
*
*******************************************************************************/
int quNextElem(quTListHeaderEntry *QuHead, quTElementHead *QuElements, 
               int ElementIndx)
{
    int i;

    if (QuElements[ElementIndx].Next == QUEUE_NULL_POINTER)
    {
        i = QuHead[QuElements[ElementIndx].Priority].NextPriority ; 
        while (i != QUEUE_NULL_POINTER)
        {
            if (QuHead[i].PriorityListLen > 0)
            {
                return (QuHead[i].PriorityFirst);
            }

            i = QuHead[i].NextPriority ;

        }

        return QUEUE_NULL_POINTER;
    }
    else
        return (QuElements[ElementIndx].Next);
}

/******************************************************************************
*	Function:	 quGetLen
*
*	Description: Get length of a priority list. 
*
*	Parameters:	 [I] quTListHeaderEntry *QuHead - Points to the queue header.
*                [I] int     Priority           - The priority 
*
*	Returns:     int - length
*
*******************************************************************************/
int quGetLen(quTListHeaderEntry *QuHead, int Priority)
{
    return QuHead[Priority].PriorityListLen;
}

/******************************************************************************
*	Function:	 quInsertAfter
*
*	Description: Inserts new element to a list. (Used for linked list)
*                if the current element index is QUEUE_NULL_POINTER, it will 
*                be inserted at the head of the list.
*
*	Parameters:	 [I] quTListHeaderEntry *QuHead - Points to the queue header.
*                [I] quTElementHead *QuElements - Points to per element header
*                [I] int        PrevElementIndx - The element to insert after
*                [I] int            ElementIndx - Index of element in list
*                [I] int     Priority (optional)- The element priority (1 to
*                                                 MaxPriority from InitQueue)
*
*	Returns:	 QUEUE_OK or QUEUE_FULL
*
*******************************************************************************/
int quInsertAfter(quTListHeaderEntry *QuHead, quTElementHead *QuElements, 
          int PrevElementIndx, int ElementIndx, int Priority)
{
    quTElementHead *ElementHeader, *TmpElementHeader;

// Eitan - full check is not required here (can be full)
//    if (quIsFull(QuHead))
//        return QUEUE_FULL;

    ElementHeader           = &QuElements[ElementIndx];
    ElementHeader->Priority = Priority;
 
    if (QuHead[Priority].PriorityListLen == 0)
    {
        ElementHeader->Prev = ElementHeader->Next = QUEUE_NULL_POINTER;

        QuHead[Priority].PriorityFirst = ElementIndx;
        QuHead[Priority].PriorityLast  = ElementIndx;
    }
    else if (PrevElementIndx == QUEUE_NULL_POINTER)
    {
        ElementHeader->Prev = QUEUE_NULL_POINTER;
        ElementHeader->Next = QuHead[Priority].PriorityFirst;
        QuElements[ElementHeader->Next].Prev = ElementIndx;
        QuHead[Priority].PriorityFirst = ElementIndx;
    }
    else
    {
        TmpElementHeader = 
            &QuElements[PrevElementIndx];

        ElementHeader->Next = TmpElementHeader->Next;
        TmpElementHeader->Next = ElementIndx;

        ElementHeader->Prev = PrevElementIndx;

        if (ElementHeader->Next != QUEUE_NULL_POINTER)
            QuElements[ElementHeader->Next].Prev = ElementIndx;
        else
            QuHead[Priority].PriorityLast  = ElementIndx;
    }

    QuHead[Priority].PriorityListLen++;
    return QUEUE_OK;


}
/******************************************************************************
*	Function:	 quGetNextElemFromPriority
*
*	Description: Gets the next item in chain from linked list. 
*
*	Parameters:	 [I] quTListHeaderEntry *QuHead - Points to the queue header
*                [I] quTElementHead *QuElements - Points to per element header
*                [I] int Queue_Priority			- Index to requested priority
*                [O] int           *ElementIndx - Points to the returned index
*
*	Returns:     int - QUEUE_OK or QUEUE_EMPTY. 
*
*******************************************************************************/
int quGetNextElemFromPriority(quTListHeaderEntry *QuHead, quTElementHead *QuElements, 
							  int Queue_Priority,int *ElementIndx)
{
    quTElementHead *ElementHeader;


	if ((QuHead[Queue_Priority].PriorityListLen <= 0)||(Queue_Priority < 0))
	{
		*ElementIndx = QUEUE_NULL_POINTER;
		return QUEUE_EMPTY;
	}
	// if user did not supply the current element he is using
	// we will return the first in the list
	// this is good for loops were I wanted to run on all linked list records
	if (*ElementIndx == QUEUE_NULL_POINTER)
	{
		*ElementIndx = quFirstElemInList(QuHead,Queue_Priority);
		
		if (*ElementIndx != QUEUE_NULL_POINTER)
			return QUEUE_OK;
		else return QUEUE_EMPTY;
	}
	else // get the next linked list
	{
		ElementHeader = &QuElements[*ElementIndx];
		
		if (ElementHeader->Next != QUEUE_NULL_POINTER)
		{	
			*ElementIndx  = ElementHeader->Next;
			return QUEUE_OK;
		}
	}

	// Return NULL address
    *ElementIndx = QUEUE_NULL_POINTER;
    return QUEUE_EMPTY;
}

/******************************************************************************
*	Function:	 quFirstElemInList
*
*	Description: Get first index of element in Priority List.
*
*	Parameters:	 [I] quTListHeaderEntry *QuHead  - Points to the queue header.
*				 [I] int     Priority            - The priority 
*
*	Returns:	 int - Element index or QUEUE_NULL_POINTER
*
*******************************************************************************/
int quFirstElemInList(quTListHeaderEntry *QuHead,int Priority)
{
    return QuHead[Priority].PriorityFirst;
}

/******************************************************************************
*	Function:	 quNextElemIList
*
*	Description: Get next index of element in Priority List.
*
*	Parameters:	 [I] quTElementHead *QuElements - Points to per element header
*                [I] int            ElementIndx - Index of current element
*
*	Returns:     int - Next element index or QUEUE_NULL_POINTER
*
*******************************************************************************/
int quNextElemInList(quTElementHead *QuElements,int ElementIndx)
{
    return QuElements[ElementIndx].Next;
}
/******************************************************************************
*	Function:	 quIsInList
*
*	Description: Checks if the Index is in List according to Priority
*
*	Parameters:	 [I] quTListHeaderEntry *QuHead - Points to the queue header.
*                [I] int            ElementIndx - Index of current element
*				 [I] int     Priority           - The priority 
*
*	Returns:	 quTBool - QUEUE_TRUE or QUEUE_FALSE
*
*******************************************************************************/
quTBool quIsInList(quTElementHead *QuElements,int ElementIndx,int Priority)
{
	if(QuElements[ElementIndx].Priority == Priority)
		return QUEUE_TRUE;

	return QUEUE_FALSE;
    
}
