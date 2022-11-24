/********************************************************************
 * AudioCodes improved application                                  *
 *                                                                  *
 * File Name:	acl_rv_mgcp_api.c                                   *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 26/02/08 - Coding started.                                       *
 *																	*
 ********************************************************************/
 #include <rvthread.h>

#include "acl_rv_mgcp_api.h"
#include <rvlog.h>
#include <rvmutex.h>
#include <rvsdp.h>
#include <stdio.h>
#include <rvsdpenc.h>
#include <rvmgcpobjects.h>

/*#define  AC_RV_MGCP_API_DEBUG*/

#define DBG_PRINT_ERR(a1,a2...)	printf("[%s:%4d] ERROR: " a1,__FUNCTION__, __LINE__, ##a2)

#ifdef AC_RV_MGCP_API_DEBUG
#define DBG_PRINT(a1,a2...)	printf("[%s:%4d] " a1,__FUNCTION__, __LINE__, ##a2)
#else /* AC_RV_MGCP_API_DEBUG */
#define DBG_PRINT(a1,a2...)
#endif /* AC_RV_MGCP_API_DEBUG */

extern 	RvLog rvLog;

/* AlexChen SDK upgrade 20090409 */
MgcpPayloadType stdMgcpPayloadTypes[] =
{
	{0, "PCMU", 8000}, {1, "1016", 8000}, {2, "G726-32", 8000}, {3, "GSM",  8000},
	{4, "G723", 8000}, {5, "DVI4", 8000}, {6, "DVI4", 16000}, {7, "LPC", 8000},
	{8, "PCMA", 8000}, {9, "G722", 8000}, {10, "L16", 44100}, {11, "L16", 44100}, 
	{12, "QCELP", 8000}, {14, "MPA", 90000}, {15, "G728", 8000}, {16, "DVI4", 11025},
	{17, "DVI4", 22050}, {18, "G729", 8000}, {25, "CelB", 90000}, {26, "JPEG", 90000},
	{28, "nv", 90000}, {31, "H261", 90000}, {32, "MPV", 90000}, {33, "MP2T", 90000},
	{34, "H263", 90000}
};

static void acl_mgcp_log_output(const RvLogRecord* rec, void* data);

void acl_mgcp_log_construct(unsigned char isLogOn)
{
	static RvLogListener MainListener;
	
	if(NULL == rvLogConstruct(&rvLog))//prepare a log object to be used.
	{
		DBG_PRINT_ERR("rvLogConstruct failed\r\n");
		return;
	}
	//rvLogSetMask(&rvLog, RV_LOGMASK_ALL);//set the log mask filter

	if(NULL == rvLogListenerConstruct(&MainListener, RV_LOGLEVEL_SEND|RV_LOGLEVEL_RECEIVE|
		RV_LOGLEVEL_FATAL|RV_LOGLEVEL_ERROR|RV_LOGLEVEL_WARNING
		|RV_LOGLEVEL_INFO1|RV_LOGLEVEL_INFO2|RV_LOGLEVEL_INFO3|RV_LOGLEVEL_INFO4, acl_mgcp_log_output, stderr))
	{
		DBG_PRINT_ERR("rvLogListenerConstruct failed\r\n");
		return;
	}
	/*$ Register the Log Listener with the Log */
	rvLogRegisterListener(&rvLog, &MainListener);
	
}

static void acl_mgcp_log_output(const RvLogRecord* rec, void* data) 
{
//	FILE* fp = (FILE*)data;
	/* Print the Log Record to the stream specified by the file
	descriptor passed in through the “data” argument */
	DBG_PRINT_ERR("%s %s %s\r\n",
	rvLogRecordGetThreadName(rec),
	rvLogRecordGetLevelString(rec),
	rvLogRecordGetText(rec));



}

#if 0// this function are not used JL
void acl_voip_mutex_construct(acl_voip_mutex_t *m)
{
    rvMutexConstruct((RvMutex *)m);
}

void acl_voip_mutex_destruct(acl_voip_mutex_t *m)
{
    rvMutexDestruct((RvMutex *)m);
}

void acl_voip_mutex_lock(acl_voip_mutex_t *m)
{
	rvMutexLock((RvMutex *)m);	
}

void acl_voip_mutex_unlock(acl_voip_mutex_t *m)
{
	rvMutexUnlock((RvMutex *)m);
}

acl_voip_sdp_attr_t *acl_voip_sdp_media_descr_attr_get(acl_voip_sdp_media_descr_t *media,
    int i)
{
    return (acl_voip_sdp_attr_t *)rvSdpMediaDescrGetAttribute(
	(RvSdpMediaDescr *)media, i);
}

char *acl_voip_sdp_attr_name_get(acl_voip_sdp_attr_t *attr)
{
    RvSdpAttribute *attr2 = (RvSdpAttribute*)attr;
    return (char *)rvSdpAttributeGetName(attr2);
}                                           

char *acl_voip_sdp_attr_value_get(acl_voip_sdp_attr_t *attr)
{
    return (char *)rvSdpAttributeGetValue((RvSdpAttribute *)attr);
}

/* ACL NB 060810 */
int acl_voip_sdp_media_descr_port_set(acl_voip_sdp_media_descr_t *media, int port)
{
    return rvSdpMediaDescrSetPort((RvSdpMediaDescr *)media, port);
}

// ACL NB  060406 
int acl_voip_buf_to_sdpList( char* bufSdp, acl_voip_sdp_msg_list_t *sdpList )
{
    RvSdpMsg            sdpMsg;
    const char version[] = "0";
    /* 070809 - EA - added setting of sdp media attributes*/   
    {
        RvSdpMediaDescr *rvMedia;
        acl_voip_sdp_media_descr_t *acMedia;        

        rvSdpMsgConstruct(&sdpMsg);
        rvSdpMsgSetVersionN(&sdpMsg, version, strlen(version));
        /* owner/creator and session identifier */
        rvSdpMsgSetOrigin(&sdpMsg, "ac494", "$", "$", RV_SDPNETTYPE_IN, RV_SDPADDRTYPE_IP4, "$");
        /* session information */
        rvSdpMsgSetSessionName(&sdpMsg, "-");
        /* connection information */
        rvSdpMsgSetConnection(&sdpMsg, RV_SDPNETTYPE_IN, RV_SDPADDRTYPE_IP4, "$");
        /* time the session is active */
        rvSdpMsgAddSessionTime(&sdpMsg, 0, 0);
        rvMedia = rvSdpMsgGetMediaDescr(&sdpMsg, 0);
        rvSdpMediaDescrAddFormat(rvMedia, "t38");

        acMedia = (acl_voip_sdp_media_descr_t *)rvMedia;
        acl_voip_sdp_media_descr_attr_add(acMedia, "T38FaxVersion", "0");
        acl_voip_sdp_media_descr_attr_add(acMedia, "T38FaxMaxBuffer", "1024");
        acl_voip_sdp_media_descr_attr_add(acMedia, "T38FaxMaxDatagram", "122");
        acl_voip_sdp_media_descr_attr_add(acMedia, "T38FaxRateManagement", "transferredTCF");
        acl_voip_sdp_media_descr_attr_add(acMedia, "T38FaxUdpEC", "t38UDPRedundancy");
    }
    printSdp((char *)&sdpMsg,"acl_voip_buf_to_sdpList: \n ********* t38MediaCap SDP *********"); 

    return 1;
}

#endif 

acl_voip_sdp_media_descr_t *acl_voip_sdp_msg_media_descr_get(acl_voip_sdp_msg_t *sdp,
    int i)
{
    /* 070729 - EA - merge to rv 2.5*/ 
    return (acl_voip_sdp_media_descr_t *)rvSdpMsgGetMediaDescr(
	(RvSdpMsg *)sdp, i);
}

void acl_voip_sdp_msg_media_descr_clear(acl_voip_sdp_msg_t *sdp)
{
    /* 070729 - EA - merge to rv 2.5*/ 
    rvSdpMsgClearMediaDescr((RvSdpMsg *)sdp);
}

void acl_voip_sdp_msg_media_descr_add(acl_voip_sdp_msg_t *sdp, int port)
{
	/* 070729 - EA - merge to rv 2.5*/ 
    rvSdpMsgAddMediaDescr((RvSdpMsg *)sdp, RV_SDPMEDIATYPE_AUDIO, port,
	RV_SDPPROTOCOL_RTP);
}

void acl_voip_sdp_media_descr_payload_add(acl_voip_sdp_media_descr_t *media, int pl)
{
    rvSdpMediaDescrAddPayloadNumber((RvSdpMediaDescr *)media, pl);
}

void acl_voip_sdp_media_descr_rtp_map_add(acl_voip_sdp_media_descr_t *media, int pl,
    char *plname, int rate)
{
    rvSdpMediaDescrAddRtpMap((RvSdpMediaDescr *)media, pl, plname, rate);
}

void acl_voip_sdp_media_descr_attr_add(acl_voip_sdp_media_descr_t *media, char *name,
    char *val)
{
    rvSdpMediaDescrAddAttr((RvSdpMediaDescr *)media, name, val);
}

int acl_voip_sdp_media_descr_payload_num_get(acl_voip_sdp_media_descr_t *media)
{
    return rvSdpMediaDescrGetNumOfPayloads((RvSdpMediaDescr *)media);
}

int acl_voip_sdp_media_descr_payload_get(acl_voip_sdp_media_descr_t *media, int i)
{
    return rvSdpMediaDescrGetPayloadNumber((RvSdpMediaDescr *)media, i);
}

int acl_voip_sdp_media_descr_rtp_map_num_get(acl_voip_sdp_media_descr_t *media)
{
    return rvSdpMediaDescrGetNumOfRtpMap((RvSdpMediaDescr *)media);
}

acl_voip_sdp_rtp_map_t *acl_voip_sdp_media_descr_rtp_map_get(
    acl_voip_sdp_media_descr_t *media, int i)
{
    return (acl_voip_sdp_rtp_map_t *)rvSdpMediaDescrGetRtpMap(
	(RvSdpMediaDescr *)media, i);
}

int acl_voip_sdp_rtp_map_payload_get(acl_voip_sdp_rtp_map_t *map)
{
    return rvSdpRtpMapGetPayload((RvSdpRtpMap *)map);
}

char *acl_voip_sdp_rtp_map_encoding_name_get(acl_voip_sdp_rtp_map_t *map)
{
    return (char *)rvSdpRtpMapGetEncodingName((RvSdpRtpMap *)map);
}

int acl_voip_sdp_msg_media_descr_num_get(acl_voip_sdp_msg_t *sdp)
{
    /* 070729 - EA - merge to rv 2.5*/ 
    return rvSdpMsgGetNumOfMediaDescr((RvSdpMsg *)sdp);
}

int acl_voip_sdp_media_descr_attr_num_get(acl_voip_sdp_media_descr_t *media)
{
    return rvSdpMediaDescrGetNumOfAttr((RvSdpMediaDescr *)media);
}

acl_voip_sdp_conn_t *acl_voip_sdp_media_descr_conn_get(acl_voip_sdp_media_descr_t *media)
{
    return (acl_voip_sdp_conn_t *)rvSdpMediaDescrGetConnection(
	(RvSdpMediaDescr *)media);
}

acl_voip_sdp_conn_t *acl_voip_sdp_msg_conn_get(acl_voip_sdp_msg_t *sdp)
{
	/* 070729 - EA - merge to rv 2.5*/ 
    return (acl_voip_sdp_conn_t *)rvSdpMsgGetConnection((RvSdpMsg *)sdp);
}

char *acl_voip_sdp_conn_addr_get(acl_voip_sdp_conn_t *conn)
{
    return (char *)rvSdpConnectionGetAddress((RvSdpConnection *)conn);
}

int acl_voip_sdp_media_descr_port_get(acl_voip_sdp_media_descr_t *media)
{
    return rvSdpMediaDescrGetPort((RvSdpMediaDescr *)media);
}

int acl_voip_sdp_msg_media_descr_type_get(acl_voip_sdp_media_descr_t *media)
{
   return rvSdpMediaDescrGetMediaType((RvSdpMediaDescr *)media); 
}

acl_voip_sdp_attr_t *acl_voip_sdp_media_descr_attr_get(acl_voip_sdp_media_descr_t *media, int i)
{
    return (acl_voip_sdp_attr_t *)rvSdpMediaDescrGetAttribute(
	(RvSdpMediaDescr *)media, i);
}

char *acl_voip_sdp_attr_name_get(acl_voip_sdp_attr_t *attr)
{
    RvSdpAttribute *attr2 = (RvSdpAttribute*)attr;
    return (char *)rvSdpAttributeGetName(attr2);
}  

char *acl_voip_sdp_attr_value_get(acl_voip_sdp_attr_t *attr)
{
    return (char *)rvSdpAttributeGetValue((RvSdpAttribute *)attr);
}

/* AlexChen SDK upgrade 20090409 */
int acl_voip_mgcpCapabilitiesIsCodecSupported(const acl_voip_mgcp_capabilities_t *caps, const char *codec, unsigned int rate)
{
	const RvMgcpRange *range = rvMgcpCapabilitiesGetBandwidth((RvMgcpCapabilities *)caps);
	if( (range->begin == RV_MGCPCONNOPT_FIELDNOTSET) || (rate >= range->begin && rate <= range->end) )
	{
		size_t i;
		for(i=0; i<rvMgcpCapabilitiesGetNumCodecs((RvMgcpCapabilities *)caps); ++i)
		{
		   
			if(!rvStrIcmp(codec, rvMgcpCapabilitiesGetCodec((RvMgcpCapabilities *)caps, i)))
				return rvTrue;
		}
	}
	return rvFalse;
}

int acl_voip_mgcpCapabilityListIsCodecSupported(const acl_voip_mgcp_capabilityList_t *capList, const char *codec, unsigned int rate)
{
	size_t i;
	for(i=0; i<rvMgcpCapabilityListGetSize((RvMgcpCapabilityList *)capList); ++i)
	{
		if(acl_voip_mgcpCapabilitiesIsCodecSupported((acl_voip_mgcp_capabilities_t *)rvMgcpCapabilityListGetElem((RvMgcpCapabilityList *)capList, i), codec, rate))
			return rvTrue;
	}
	return rvFalse;
}

acl_voip_mgcp_payload_type_t *mgcp_getStandardPayloadType(int payload)
{
	size_t i;
	for(i=0; i<sizeof(stdMgcpPayloadTypes)/sizeof(stdMgcpPayloadTypes[0]); ++i)
		if(stdMgcpPayloadTypes[i].payload == payload) 
			return (acl_voip_mgcp_payload_type_t *)(&stdMgcpPayloadTypes[i]);
	return NULL;
}

void acl_voip_mgcpCapabilityListGetAllRtpMaps(const acl_voip_mgcp_capabilityList_t *capList, acl_voip_mgcp_ptrList_t *rtpMapList)
{
	size_t i;
	for(i=0; i<rvMgcpCapabilityListGetSize((RvMgcpCapabilityList *)capList); ++i)
	{
		RvMgcpCapabilities *caps = rvMgcpCapabilityListGetElem((RvMgcpCapabilityList *)capList, i);
		size_t j;
		for(j=0; j<rvMgcpCapabilitiesGetNumCodecs(caps); ++j)
		{
			const char *codecName = rvMgcpCapabilitiesGetCodec(caps, j);
			int payloadType = rvMgcpCapabilitiesGetDynamicCodecPayloadType(caps, j);
			if(payloadType >= FIRST_DYNAMIC_PAYLOAD_TYPE)
			{
				MgcpPayloadType *map = rvAllocAllocate(&rvDefaultAlloc, sizeof(MgcpPayloadType));
				map->payload = payloadType;
				map->codec = codecName;
				map->rate = rvMgcpCapabilitiesGetDynamicCodecRate(caps, j);
				rvPtrListPushBack((RvPtrList *)rtpMapList, map);
			}
			else
			{	
				const RvMgcpRange *range = rvMgcpCapabilitiesGetBandwidth(caps);
				size_t m;
				for(m=0; m<sizeof(stdMgcpPayloadTypes)/sizeof(stdMgcpPayloadTypes[0]); ++m)
					if(!rvStrIcmp(stdMgcpPayloadTypes[m].codec, codecName) &&
					   (range->begin == RV_MGCPCONNOPT_FIELDNOTSET ||
					   (stdMgcpPayloadTypes[m].rate >= range->begin && stdMgcpPayloadTypes[m].rate <= range->end)))
						rvPtrListPushBack((RvPtrList *)rtpMapList, &stdMgcpPayloadTypes[m]);
			}
		}
	}
}


void IppThreadSleep(int sec, int msec)
{
	unsigned int clacMSec = 0;

    if (0 > sec)
        sec = 0;
    if (0 > msec)
        msec = 0;

    clacMSec = sec*1000+msec;
    rvThreadSleep(clacMSec);
}

