/*--------------------------------------------------------------------
*
* Copyright (c) 2007 Outline Electronics Ltd.
*
* All rights reserved. Reproduction in whole
* or in part is prohibited without the written
* permission of the copyright owner.
*--------------------------------------------------------------------
* File: olStunData.h
*
* Description:
* Buffering of Stun data.
*
* Notes:
* 
*--------------------------------------------------------------------
*--------------------------------------------------------------------*/

#ifndef OL_STUN_DATA_H
#define OL_STUN_DATA_H

RV_DECLARE_HANDLE(OlStunDataHandle);
RV_DECLARE_HANDLE(OlStunBufferHandle);

OlStunBufferHandle olStunBufferConstruct(void);
void olStunBufferDestruct(OlStunBufferHandle);

OlStunDataHandle olStunDataConstruct(OlStunBufferHandle);
void olStunDataDestruct(OlStunDataHandle);

void olStunDataSetAddrData(OlStunDataHandle, void *reqAddrPtr);
void *olStunDataGetAddrData(OlStunDataHandle);

void olStunDataSetId(OlStunDataHandle, RvUint128 *id);
void olStunDataGetId(OlStunDataHandle, RvUint128 *id);

void olStunDataSetReqMsg(OlStunDataHandle, RvChar *msg);

void olStunDataSendCompletion(OlStunDataHandle);

OlStunDataHandle olStunDataSearchById(OlStunBufferHandle, RvUint128 *id);

#endif /* OL_STUN_DATA_H */
