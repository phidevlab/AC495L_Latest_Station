/******************************************************************************
Filename:    ippcodec.h
Description: Ipp wrapper for codec parsing API
*******************************************************************************
                Copyright (c) 2001 RADVISION
*******************************************************************************
NOTICE:
This document contains information that is proprietary to RADVISION.
No part of this publication may be reproduced in any form whatsoever
without written prior approval by RADVISION.

RADVISION reserves the right to revise this publication and make changes
without obligation to notify any person of such revisions or changes.
******************************************************************************/

#ifndef _ippcodecs_h_
#define _ippcodecs_h_ 


#include <stdlib.h>
#include "rvsdp.h"
#include "rvsdpcodecs.h"
#include "rvlog.h"
#include "rvtypes.h"
#include "rvstrutils.h"

#ifdef RV_MTF_MEDIA
#include "CMFCodecEnum.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif
	
	
#define FIRST_DYNAMIC_PAYLOAD 96


#ifndef RV_MTF_MEDIA
	/* Note: these enumerators are duplicated from CMFCodecEnum.h, since it would 
	   create dependency of MTF in MF when MTF is running with no media*/ 
typedef enum {
		eMFNullPayloadCodecType = 0,
			enMFAudioBeginPayloadCodecType,
			eMFG711APayloadCodecType = enMFAudioBeginPayloadCodecType,
			eMFG711UPayloadCodecType,
			eMFG711Annex2CNCodecType,
			eMFG723PayloadCodecType,
			eMFG723SSPayloadCodecType,
			eMFG729PayloadCodecType,
			eMFG729ABPayloadCodecType,
			eMFAMRPayloadCodecType,
			eMFAMRWbPayloadCodecType,
			eMFG728PayloadCodecType,
			eMFG722PayloadCodecType,
			eMF8kLinearPayloadCodecType,
			eMF16kLinearPayloadCodecType,
			enMFAudioEndPayloadCodecType,
			enMFVideoBeginPayloadCodecType = enMFAudioEndPayloadCodecType,
			eMFH261PayloadCodecType = enMFVideoBeginPayloadCodecType,
			eMFH263PayloadCodecType,
			eMFH263aPayloadCodecType,
			eMFMP4PayloadCodecType,
			eMFH264PayloadCodecType,
			eMFH263PlusPayloadCodecType,  /* H263 RFC2429 propriatery Packetization*/
			eMFH263_2190_PayloadCodecType,       /* RFC 2190*/
			eMFH263_2190_short_packet_PayloadCodecType,
			eMFH263_2429_PayloadCodecType,       /* RFC 2429*/
			eMFH263_2429_follow_on_PayloadCodecType,
			
			eMFYUV420PayloadCodecType,
			eMFRGB24PayloadCodecType,
			eMFBGR24PayloadCodecType,
			
			enMFVideoEndPayloadCodecType,
			eMFRfc2833EventsPayloadCodecType = enMFVideoEndPayloadCodecType,
			eMFRfc2833TonesPayloadCodecType,
			eMFMaxPayloadCodecType
} enMFRtpPayloadCodecType_t; 


#endif /*RV_MTF_MEDIA*/	

#ifndef eMFFrameSizeFormat_t_AVOID_DOUBLE_DEFINITION
#define eMFFrameSizeFormat_t_AVOID_DOUBLE_DEFINITION
enum eMFFrameSizeFormat_t { 
    MF_FRAME_WIDTH_SQCIF  = 128,
		MF_FRAME_HEIGHT_SQCIF = 96,
		MF_FRAME_WIDTH_QCIF  = 176,
		MF_FRAME_HEIGHT_QCIF = 144,
		MF_FRAME_WIDTH_CIF   = MF_FRAME_WIDTH_QCIF*2,
		MF_FRAME_HEIGHT_CIF  = MF_FRAME_HEIGHT_QCIF*2,
		MF_FRAME_WIDTH_4CIF   = MF_FRAME_WIDTH_QCIF*4,
		MF_FRAME_HEIGHT_4CIF  = MF_FRAME_HEIGHT_QCIF*4,
		MF_FRAME_WIDTH_16CIF   = MF_FRAME_WIDTH_QCIF*8,
		MF_FRAME_HEIGHT_16CIF  = MF_FRAME_HEIGHT_QCIF*8,
		MF_FRAME_WIDTH_SPFMT  = 0,
		MF_FRAME_HEIGHT_SPFMT  = 0,
		MF_FRAME_WIDTH_QVGA  = 0,
		MF_FRAME_HEIGHT_QVGA  = 0
};


typedef enum  
{   /* 10 * frames per sec (for integral values)*/
    MF_FRAME_RATE_75 = 75,
		MF_FRAME_RATE_10 = 100,
		MF_FRAME_RATE_15 = 150,
		MF_FRAME_RATE_20 = 200,
		MF_FRAME_RATE_25 = 250,
		MF_FRAME_RATE_30 = 300
}enVideoFrameRate_t; 
 

typedef enum  
{
    MF_RESOLUTION_SQCIF, /*sub-QCIF*/
		MF_RESOLUTION_QCIF,
		MF_RESOLUTION_CIF,
		MF_RESOLUTION_4CIF,
		MF_RESOLUTION_16CIF,
		MF_RESOLUTION_SPFMT,
		MF_RESOLUTION_QVGA
}eMFResulutionFormat_t;  

#endif /*eMFFrameSizeFormat_t_AVOID_DOUBLE_DEFINITION*/

/*
 *	This function initializes the SDP Codec module;
 *  It needs to be called exactly once before any other function from
 *  the module
 */
RVAPI RvStatus IppCodecInitialize(RvLogMgr* logMgr);

/*
 *	This function de-initializes the SDP Codec module;
 *  It needs to be called exactly once at the end of module use.
 */
RVAPI RvStatus IppCodecEnd(void);

/***************************************************************************************
 * IppCodecGetEnum
 * -------------------------------------------------------------------------------------
 * General:   Parse RvSdpMediaDescr and return codec enum this descr speaks about  
 *				
 * Return Value: IppCodecEnum
 * -------------------------------------------------------------------------------------
 * Arguments:
 * Input: 	 
 *			 RvSdpMediaDescr*	descr	- media parameters.  
 *			 int				payload	- /if payload <0 than the 1st payload inside descr will be used /  
 *										 
 * Output:   None
 *			  
**************************************************************************************/
RVAPI enMFRtpPayloadCodecType_t	IppCodecGetEnum( IN RvSdpMediaDescr* descr, IN int payload);


/********************************************************
 *
 * API: SDP ----> Codec 
 *
 *******************************************************/
RvBool	IppCodecFindPayloadInMsg(IN RvSdpMsg *msg, 
							IN int payload, 
							OUT RvSdpMediaDescr** ppdescr, 
							OUT RvSdpRtpMap** pprtpMap);
					
RvBool	IppCodecFindRtpMapInMediaDescr(
							IN RvSdpMediaDescr	*media, 
							IN int payload, 
							OUT RvSdpRtpMap** pprtpMap);
							
 
/********************************************************
 *
 * API: Codec ----> SDP. Internal
 *
 *******************************************************/
typedef struct 
{
	char	buf[4096];
	char	delimeter;
	RvBool	bNotFirstParam;
}IppFmtpAttr;

void IppFmtpAttrConstruct( INOUT IppFmtpAttr* attr, IN int payLoad, IN char delimeter);

RvStatus IppFmtpAttrAddParamByName( INOUT IppFmtpAttr* attr, 
								  IN const char* param_name,  
								  IN const char* value);



/********************************************************
 *
 * API: Wrappers 
 *
 *******************************************************/

RvStatus IppSdpMediaGetRtpIP( 
							 IN RvSdpMsg*		msg, 
							 IN RvSdpMediaDescr*	descr, 
							 OUT RvUint32*		pIp,
							 OUT char**			pszIp);
							 
/************************************************************************/
/***********************************************************
 Function:    IppSdpMediaGetMediaIp ()

 Description: Get  IP from c= ... of descriptor if exists.

 Arguments:   media - includes sdp message

 Returns:     point to IP string or NULL if c= ... doesn't exists.
***********************************************************/
RVAPI const char* IppSdpMediaGetMediaIp(  IN RvSdpMediaDescr*	descr);
/************************************************************************/
RVAPI RvStatus IppSdpMediaGetRtpPort(
								IN RvSdpMediaDescr*	descr,
								OUT RvUint16* nIp);
/************************************************************************/
RvStatus IppSdpMediaGetRtcpPort( 
								IN RvSdpMediaDescr*	descr,
								OUT RvUint16* nIp);
/************************************************************************/
RVAPI RvStatus IppSdpMediaSetMediaIp( IN RvSdpMediaDescr* descr, 
									 IN RvSdpAddrType type, /* RV_SDPADDRTYPE_IP4 or RV_SDPADDRTYPE_IP6 */
									 IN const char* szAddr);
/************************************************************************/
RVAPI RvStatus IppSdpMediaSetRtpPort( IN RvSdpMediaDescr* descr, IN RvUint16 port);


/***********************************************************
 Function:    getStringAttr ()

 Description: Get  value of media descr attributes if exists.

 Arguments:   media - includes sdp message, OUT ptime - packet time

 Returns:     attr value or NULL if such an attribute doesn exists.
***********************************************************/
const char* getStringAttr(RvSdpMediaDescr *media, IN const char* szAttr);
/***********************************************************
 Function:    setStringAttr(), setIntAttr()

 Description: Set  SDP message attributes.

 Arguments:   media - includes sdp message, 

 Returns:     None.
***********************************************************/
void setStringAttr(RvSdpMediaDescr *media, IN const char* szAttr, IN const char* szValue);
void setIntAttr(RvSdpMediaDescr *media, IN const char* szAttr, IN RvUint32 nValue);
/*******************************************************************************/
void setPacketTime(RvSdpMediaDescr *media, IN RvUint32 ptime);
/*******************************************************************************/
RVAPI int 	IppCodecGetFirstPayload( IN RvSdpMediaDescr* descr );
/*******************************************************************************/
RVAPI RvStatus IppSerializeSdpDescrTo( OUT char* buf, IN RvUint32 bufsize, IN RvSdpMediaDescr* descr);
/***************************************************************************************/
RVAPI  RvStatus IppSerializeSdpDescrFrom( IN const char* buf, OUT RvSdpMediaDescr* pdescr);
/***************************************************************************************/
RVAPI const char* ippGetAttrByName(IN const char* attrName, IN RvSdpMediaDescr *descr);

/***************************************************************************
 * IppSdpUtil_itoa
 * ------------------------------------------------------------------------
 * General: The function convert integer to string.
 * Return Value: None.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: number - The integer value.
 *        str	 - The buffer that will contain the string of the integer.
 *		  
 ***************************************************************************/
RVAPI char *IppSdpUtil_itoa(int number, char *str);
#ifdef __cplusplus
}
#endif

#endif /*_ippcodecs_h_*/


 
