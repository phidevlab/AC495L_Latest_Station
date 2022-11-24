/*--------------------------------------------------------------------
*
* Copyright (c) 2007 Outline Electronics Ltd.
*
* All rights reserved. Reproduction in whole
* or in part is prohibited without the written
* permission of the copyright owner.
*--------------------------------------------------------------------
* File: olStunMsg.h
*
* Description:
* Construct Stun request and process the Stun response. 
*
* Notes:
* Modified from the previous version.
*
*--------------------------------------------------------------------
*--------------------------------------------------------------------*/

#ifndef OL_STUN_MSG_H
#define OL_STUN_MSG_H

RvChar *
olStunBuildRequest(
		IN RvBool changePort,
		IN RvBool changeIp,
		IN RvBool discard, 
		OUT RvUint128 *resId, 
		OUT RvInt *size);

RvBool
olStunParseForMappedAddr(
			IN char *buf,
			IN unsigned int size,
			OUT RvUint128 *id,
			OUT RvUint32 *mappedIp,
			OUT RvUint16 *mappedPort);

#endif /* OL_STUN_MSG_H */
