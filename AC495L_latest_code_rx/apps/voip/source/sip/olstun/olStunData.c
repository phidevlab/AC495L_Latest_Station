/*--------------------------------------------------------------------
*
* Copyright (c) 2007 Outline Electronics Ltd.
*
* All rights reserved. Reproduction in whole
* or in part is prohibited without the written
* permission of the copyright owner.
*--------------------------------------------------------------------
* File: olStunData.c
*
* Description:
* Buffering of Stun data.
*
* Notes:
* 
*--------------------------------------------------------------------
*--------------------------------------------------------------------*/

#include "IppStdInc.h"

#include "olStunTypes.h"
#include <rvSipStunApi.h> // ACL NB 080101

#define OL_MAX_STUN_DATA 20

typedef struct
{
	//void *reqAddrPtr; ACL NB 080101
   RvIppStunAddrData reqAddr;
	RvChar *reqMsg;
	RvUint128 id;
	RvBool used;
} OlStunData;

typedef struct 
{
	OlStunData buf[OL_MAX_STUN_DATA];
} OlStunBuffer;

void *olStunBufferConstruct(void)
{
	OlStunBuffer *ptr;
	
	ptr = malloc(sizeof(OlStunBuffer));
	if (ptr != NULL)
		memset(ptr, 0, sizeof(OlStunBuffer));
	return ptr;
}

void olStunBufferDestruct(void *ptr)
{
	free(ptr);
}

OlStunData *olStunDataConstruct(void *stunBuf)
{
	int i;
	OlStunBuffer *buf;
	
	buf = (OlStunBuffer *)stunBuf;
		
	for (i = 0; i < OL_MAX_STUN_DATA; i++) {
		if (buf->buf[i].used == 0) 
      {
			break;
		}
	}
	
	if (i < OL_MAX_STUN_DATA) {
		buf->buf[i].reqMsg = NULL;
		buf->buf[i].used = 1;
		return (&(buf->buf[i]));
	}
	else
   {
      return NULL;
   }
}

void olStunDataSendCompletion(OlStunData *data)
{
	if (data->reqMsg != NULL) {
		free(data->reqMsg);
		data->reqMsg = NULL;
	}
}

void olStunDataDestruct(OlStunData *data)
{
	olStunDataSendCompletion(data);	/* should discard the stun req message */
	data->used = 0;
}

void olStunDataSetAddrData(OlStunData *data, void *reqAddrPtr)
{
	//data->reqAddrPtr = reqAddrPtr; ACL NB 080101
   memcpy(&(data->reqAddr), reqAddrPtr, sizeof(data->reqAddr));
}

void *olStunDataGetAddrData(OlStunData *data)
{
	//return data->reqAddrPtr;
	return (void *)&(data->reqAddr);
}

void olStunDataSetId(OlStunData *data, RvUint128 *id)
{
	memcpy(&(data->id), id, sizeof(RvUint128));
}

void olStunDataGetId(OlStunData *data, RvUint128 *id)
{
	memcpy(id, &(data->id), sizeof(RvUint128));
}

void olStunDataSetReqMsg(OlStunData *data, RvChar *msg)
{
	data->reqMsg = msg;
}

OlStunData *olStunDataSearchById(void *stunBuf, RvUint128 *id)
{
	int i;
	OlStunBuffer *buf;
	
	buf = (OlStunBuffer *)stunBuf;
	
	for (i = 0; i < OL_MAX_STUN_DATA; i++) {
		if (buf->buf[i].used == 1) {
			if (strncmp((char *)id, (char *)&(buf->buf[i].id), sizeof(RvUint128)) == 0)
         {
				return (&(buf->buf[i]));
         }
		}
	}

	return NULL;
}

