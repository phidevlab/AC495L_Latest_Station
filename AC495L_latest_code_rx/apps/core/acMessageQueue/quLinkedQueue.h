/*
* ----------------------------------------------------------------------------
* --							Copyright
* ----------------------------------------------------------------------------
* --
* --	Copyright 1999  audiocodes
* --
* ----------------------------------------------------------------------------
* ----------------------------------------------------------------------------
* -- 
* --	File			:	quLinkedQueue.h
* --
* --	Project			:	MEGACO_Module
* --
* --	Description		:	Logical queue, priority based. Can be used also 
* --                        for linked list. The elements can be chained in
* --                        empty list (priority 0) or in a priority list. 
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

#ifndef _QUEUE_PRIO_H_
#define _QUEUE_PRIO_H_

#define QUEUE_EMPTY_INDEX   0                     // Index 0 is the empty chain
#define QUEUE_MIN_PRIORITY  1
#define QUEUE_NULL_POINTER -1


#define CIRCIDS_EMPTY_LIST		0
#define CIRCIDS_BUSY_LIST		1

/* Return codes */
#define QUEUE_OK            0
#define QUEUE_EMPTY         1
#define QUEUE_FULL          2

#define QUEUE_TRUE          1
#define QUEUE_FALSE         0

/* Boolean type */
typedef unsigned char quTBool;

/* The header of the whole queue is an array of the following type: */
/* Each entry is header of the priority lists. (Priority 0 = empty list) */
typedef struct 
{
    int PriorityListLen;                     //#Elements in this priority
    int PriorityFirst;                       //Points to first element
    int PriorityLast;                        //Points to last element
    int NextPriority;                        //Index of next priority
} quTListHeaderEntry;


/* Per element pointers */
typedef struct
{
    int  MyIndx;
    int  Next;
    int  Prev;
    int  Priority;
} quTElementHead;


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
#ifdef __MG_MODULE_WIN_BUILD__
MGMODULE_API
#endif
quTBool QuInitQueue(int NumElements, int MaxPriority, quTListHeaderEntry *QuHead,
                quTElementHead *QuElements);

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
          int *ElementIndx);

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
#ifdef __MG_MODULE_WIN_BUILD__
MGMODULE_API
#endif
void quFreeBuf(quTListHeaderEntry *QuHead, quTElementHead *QuElements, 
               int ElementIndx);

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
               int ElementIndx);

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
#ifdef __MG_MODULE_WIN_BUILD__
MGMODULE_API
#endif
int quAllocBuf(quTListHeaderEntry *QuHead, quTElementHead *QuElements, 
               int *ElementIndx);

int quAllocPutBuf(quTListHeaderEntry *QuHead, quTElementHead *QuElements, 
               int *ElementIndx, int Priority /*= 1*/);

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
#ifdef __MG_MODULE_WIN_BUILD__
MGMODULE_API
#endif
int quPut(quTListHeaderEntry *QuHead, quTElementHead *QuElements, 
          int ElementIndx, int Priority /*= 1*/);

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
quTBool quNotEmpty(quTListHeaderEntry *QuHead);

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
quTBool quIsFull(quTListHeaderEntry *QuHead);

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
#ifdef __MG_MODULE_WIN_BUILD__
MGMODULE_API
#endif
void quFreeFromList(quTListHeaderEntry *QuHead, quTElementHead *QuElements, 
          int ElementIndx);
          
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
int quFirstElem(quTListHeaderEntry *QuHead);

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
int quLastElem(quTListHeaderEntry *QuHead, int Priority);


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
               int ElementIndx);

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
int quGetLen(quTListHeaderEntry *QuHead, int Priority);

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
          int PrevElementIndx, int ElementIndx, int Priority /*= 1*/);
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
int quFirstElemInList(quTListHeaderEntry *QuHead,int Priority);

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
int quNextElemInList(quTElementHead *QuElements,int ElementIndx);
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
quTBool quIsInList(quTElementHead *QuElements,int ElementIndx,int Priority);
          
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
							  int Queue_Priority,int *ElementIndx);
#endif
