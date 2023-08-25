/*--------------------------------------------------------------------
*
* Copyright (c) 2007 Outline Electronics Ltd.
*
*--------------------------------------------------------------------
* File: olStunClient.h
*
* Description:
* Provide Stun client functions to the RV SIP main process.
*
* Notes:
* 
*--------------------------------------------------------------------
*--------------------------------------------------------------------*/

#ifdef RV_MTF_STUN
#ifndef OL_STUN_CLIENT_H
#define OL_STUN_CLIENT_H

RvStatus stunClientInit(RvSipStackHandle  stackHandle);
//RvStatus stunClientEnd(IN RvIppSampleGateway* gw);

RvStatus stunClientProcessInput(IN  RvChar *buf,IN  RvSize_t	size,	RvUint32 *pMappedIp,	RvUint16 *pMappedPort);

#endif /* OL_STUN_CLIENT_H*/
#endif /* RV_MTF_STUN */

