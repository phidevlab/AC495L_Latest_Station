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

#include <arpa/inet.h>


#include "IppStdInc.h"
#include "inc.h"
#include "rvtimestamp.h"

#include "rvSipControlApi.h"

#include "RvSipResolver.h"
#include "RvSipTransmitter.h"

#include "rvSipStunApi.h"
#include "acl_voip_conf.h"
#include "voip_rtp_utils.h"
#include "olStunTypes.h"
#include "olStunData.h"
#include "olStunMsg.h"
#include "olStunClient.h"


RV_DECLARE_HANDLE(RvStunRequestHandle);

voip_stun_params_t          g_StunCfg;

RvUint32 g_myLocal;

RvIppStunMgrHandle	       g_stunMgrHndl;            /* MTF STUN object to use */

RvSipTransmitterMgrHandle   g_hTrxMgr;
RvSipTransportAddr          g_stunSrvSipAddr;

OlStunBufferHandle			g_stunBuf;

static RvStatus AppSendBufferFunc(
    IN void*                ctx,
    IN RvChar*              msg,
    IN RvInt                msgSz,
    IN RvStunRequestHandle  req);


#define  ACL_OLSTUN_CLIENT_DEBUG

#ifdef OLSTUN_DEBUG
#define DPRINTK printf
#else
#define DPRINTK printf
#endif

RvStatus stunClientProcessInput(IN  RvChar *buf,IN  RvSize_t	size,	RvUint32 *pMappedIp,	RvUint16 *pMappedPort)
{
     RvUint128 id;
     RvIppStunAddrData *addrData;
     OlStunDataHandle stunData;
     
     DPRINTK("  >>");               
      
      if (olStunParseForMappedAddr(buf, size, &id, pMappedIp, pMappedPort) == RV_FALSE) {
          DPRINTK("ERROR: this is not stun message\n");
          return RV_ERROR_UNKNOWN;
      }
  
      if ((stunData = olStunDataSearchById(g_stunBuf, &id)) != NULL) 
      {
          addrData = (RvIppStunAddrData *)olStunDataGetAddrData(stunData);
          addrData->outAddr.addrtype = RV_ADDRESS_TYPE_IPV4;

          addrData->outAddr.data.ipv4.ip = *pMappedIp;
          addrData->outAddr.data.ipv4.port = *pMappedPort;
          DPRINTK("stunClientGetMappingAddress %#x %d\n", *pMappedIp, *pMappedPort);

          //deactiveStunReq( RvAddressGetIpPort(&addrData->inAddr) );
      
          if (*pMappedIp != 0 && *pMappedPort != 0)
              RvIppStunAddressResolveComplete(addrData, RV_TRUE);
          else
              RvIppStunAddressResolveComplete(addrData, RV_FALSE);

              
          olStunDataDestruct(stunData);
      }
      else
      {
          DPRINTK("ERROR: cannot find the Stun Req\n");
          return RV_ERROR_UNKNOWN;
      }
      
     DPRINTK("  <<");               
  
      return RV_OK;
}

static RvStatus RVCALLCONV stunClientReplyReceived(IN  RvAddress*   addr,
                                                   IN  RvChar*      buf,
                                                   IN  RvSize_t     size,
                                                   OUT RvBool*      bDiscardMsg)
{
    RvStatus status;
    RvUint32 mappedIp;
    RvUint16 mappedPort;

   DPRINTK("  len=%d\n", size);				

    /* Well, if this is a STUN response, we will simply need to let the MTF know
       it needs to discard this message, as we'll be the ones handling it. */
    status = stunClientProcessInput(buf, size, &mappedIp, &mappedPort );
    
	*bDiscardMsg = (status == RV_OK); 
        
    return status/*RV_OK*/; //ACL NB 08101
}

static RvStatus RVCALLCONV stunClientSendRequest(IN RvIppStunAddrData* addrData)
{
    //    RvStunRequestHandle stunReq;
    RvStatus rv = RV_OK;
    RV_CHAR strIp[64];
    RV_CHAR *stunAddress;
//    RvUint intIp;
    RvUint32 fromAddr;
    //  RvUint16 fromPort;
    RvUint16 destPort;
//    SendMethod *method;
    RvChar *msg;
    RvInt msgSize;
    OlStunDataHandle stunReq;
    RvUint128 id;
    
    stunAddress = g_StunCfg.stunServerAddress;
    destPort    = g_StunCfg.stunServerPort;

    DPRINTK("stunClientSendRequest %x %d\n", addrData->inAddr.data.ipv4.ip, addrData->inAddr.data.ipv4.port);

    RvAddressGetString(&(addrData->inAddr), sizeof(strIp), strIp);
    fromAddr = inet_addr(strIp);
    if (fromAddr == INADDR_NONE)
    {
        DPRINTK("ERROR: invalid in_addr\n");
        rv = RV_ERROR_BADPARAM;
    }

    if (rv == RV_OK) 
    {
        // fromPort = RvAddressGetIpPort(&(addrData->inAddr));
        //      RvIppStunMgrGetSendMethod (g_gw->stunMgrHndl, &method);
        /* Build STUN query according to RFCstun */

        msg = olStunBuildRequest(RV_FALSE, RV_FALSE, RV_FALSE, &id, &msgSize);
        /* Send STUN query by SIP message sending function */
        //      method->sendMsgCB(method->usrData, stunAddress, destPort, &msg, sizeof(StunRequestSimple));
        stunReq = olStunDataConstruct(g_stunBuf);
        if (stunReq == NULL) {
            DPRINTK("ERROR: Stun buffer full\n");
            RvIppStunAddressResolveComplete(addrData, RV_FALSE);
            return RV_ERROR_UNKNOWN;
        }
        
        olStunDataSetAddrData(stunReq, addrData);
        olStunDataSetId(stunReq, &id);
        olStunDataSetReqMsg(stunReq, msg);
        
        rv = AppSendBufferFunc(addrData ,msg, msgSize, (RvStunRequestHandle)stunReq );
        if (rv != RV_OK)
        {
            DPRINTK("ERROR: send stun data\n");
            olStunDataDestruct(stunReq);
            RvIppStunAddressResolveComplete(addrData, RV_FALSE);
            return RV_ERROR_UNKNOWN;
        }
    }  

    DPRINTK("stunClientSendRequest <<\n");

    return rv;
}

   // for debug
static int is_ip_in_subnet(RvUint32 ip, RvUint32 netmask, RvUint32 subnet)
{
    return netmask && (ip & netmask)==subnet;
}

static RvBool RVCALLCONV stunIsAddressResolveNeededCB(IN RvAddress* addrDest)
{
    /* We check here if the destination address is inside the private network.
       We do that by checking the given address against an address mask, which
       is basically a string, with the possible 'x' character at its end,
       denoting a matching mask. */
    RvChar  szDest[64];
    RvChar* ptrDest;
    //RvChar* ptrMask;
    RvUint32 netmask;
	
    /* Convert to a string and start running on our strings */
    RvAddressGetString(addrDest, sizeof(szDest), szDest);
    ptrDest = szDest;
    //ptrMask = g_StunCfg.stunNeedMask;
    netmask = inet_addr(g_StunCfg.stunNeedMask);

   DPRINTK("stunIsAddressResolveNeededCB %s \n", ptrDest);

   if ( is_ip_in_subnet(inet_addr(ptrDest), netmask, g_myLocal&netmask) )
   {
      DPRINTK("is_ip_in_subnet True, stun address resolve not needed\n");
      return RV_FALSE;
   }
   else
   {
      DPRINTK("is_ip_in_subnet False, stun address resolve needed\n");
      return RV_TRUE;
   }   
}

/***************************************************************************
 * AppTrxStateChangedEv
 * ------------------------------------------------------------------------
 * General: Transmitter events callback.
 * Needed to notify STUN about completion of transmit operation.
 ***************************************************************************/
static void RVCALLCONV AppTrxStateChangedEv(
                        IN  RvSipTransmitterHandle          hTrx,
                        IN  RvSipAppTransmitterHandle       hAppTrx,
                        IN  RvSipTransmitterState           eState,
                        IN  RvSipTransmitterReason          eReason,
                        IN  RvSipMsgHandle                  hMsg,
                        IN  void*                           pExtraInfo)
{
    
    RV_UNUSED_ARG(eReason);
    RV_UNUSED_ARG(hMsg);
    RV_UNUSED_ARG(pExtraInfo);

    OlStunDataHandle stunReq = (OlStunDataHandle)hAppTrx;

    switch (eState)
    {
    case RVSIP_TRANSMITTER_STATE_MSG_SENT:
        RvSipTransmitterTerminate(hTrx);
        /* notify STUN the send operation was completed */
        //        RvStunRequestSendCompletion(stunReq);
        //olStunDataSendCompletion(stunReq);
        DPRINTK("Send stun req completed\n");
        break;

    case RVSIP_TRANSMITTER_STATE_MSG_SEND_FAILURE:
        RvSipTransmitterTerminate(hTrx);
        /* we may destroy STUN request here */
        //        RvStunRequestDestroy(stunReq, RV_TRUE);
        DPRINTK("ERROR: send stun req failure\n");
        olStunDataDestruct(stunReq);
        break;

    default:
        break;
    }
}


/***************************************************************************
 * transmitStunReqOverSip
 * ------------------------------------------------------------------------
 * This function send the stun request of SIP
 ***************************************************************************/
static RvStatus transmitStunReqOverSip(
    IN RvChar*              msg,
    IN RvInt                msgSz,
    IN RvStunRequestHandle  req)
{
   RvSipTransmitterEvHandlers  trxEvHandlers;        
   RvSipTransmitterHandle      hTrx = NULL;
   RvStatus                    status;
   
   DPRINTK("transmitStunReqOverSip\n");
   
   memset(&trxEvHandlers, 0, sizeof(RvSipTransmitterEvHandlers));
   trxEvHandlers.pfnStateChangedEvHandler = AppTrxStateChangedEv;
   
   /* create the Transmitter */
   status = RvSipTransmitterMgrCreateTransmitter(g_hTrxMgr, (RvSipAppTransmitterHandle)req,
      &trxEvHandlers, sizeof(trxEvHandlers), &hTrx);
   
   /* set the transmitter address as the address of STUN server */
   status = RvSipTransmitterSetDestAddress(hTrx, &g_stunSrvSipAddr,
      sizeof(g_stunSrvSipAddr), NULL, 0);
   
   /* perform the send */
   status = RvSipTransmitterSendBuffer(hTrx, msg, msgSz);
   
   return status;
}

/***************************************************************************
 * AclTransmitStunReqOverRtp
 * ------------------------------------------------------------------------
 ***************************************************************************/
/* receive all data, redirected from RTP to user mode */

RvStatus AclTransmitStunReqOverRtp(IN RvChar*              msg,
							              IN RvInt                msgSz,
							              RvIppStunAddrData*      pAddr,
							              IN RvStunRequestHandle  req)
{
   RvUint16            rtpPort;

   DPRINTK(">>\n");

   /* get the requested RTP port from ctx which contains data from the SDP m= line */
   rtpPort = RvAddressGetIpPort(&pAddr->inAddr);
  
   DPRINTK("rtpPort=%d\n", rtpPort);

   stun_rtp_req( rtpPort, 
                           msg, 
                           msgSz,
                           g_StunCfg.stunServerPort,
                           g_StunCfg.stunServerAddress);
    
   return RV_OK;

// Error:
//    return RV_ERROR_UNKNOWN;
}

/***************************************************************************
 * AclTransmitStunReqOverRtcp
 * ------------------------------------------------------------------------
 * Temporary STUB
 ***************************************************************************/
int AclTransmitStunReqOverRtcp(IN RvChar*              msg,
							          IN RvInt                msgSz,
							          IN RvIppStunAddrData*   pAddr,
							          IN RvStunRequestHandle  req)
{
   RvUint16            rtcpPort;

   DPRINTK(">>\n");

   rtcpPort = RvAddressGetIpPort(&pAddr->inAddr);

   //activeStunReq(rtcpPort);

   DPRINTK("rtcpPort=%d\n", rtcpPort);

   stun_rtcp_req( rtcpPort, 
                           msg, 
                           msgSz,
                           g_StunCfg.stunServerPort,
                           g_StunCfg.stunServerAddress);
   
#if 0
   pAddr->outAddr.addrtype = RV_ADDRESS_TYPE_IPV4;
	pAddr->outAddr.data.ipv4.ip   = 0x102210a/*rtp->mappedIp*/;
	pAddr->outAddr.data.ipv4.port = 5001/*rtp->mappedPort*/;

   RvIppStunAddressResolveComplete( pAddr, RV_TRUE);
	olStunDataDestruct((OlStunDataHandle)req);
   
      StunReq_api(msg, 
                           msgSz,
                           rtcpPort,
                           g_gw->stunServerPort,
                           g_gw->stunServerAddress,
                           RTCP_STUN_REQ_E);
#endif
   return 0;
}
/***************************************************************************/

/***************************************************************************
 * transmitStunReqOverRtp
 * ------------------------------------------------------------------------
 * It receives a msg supplied by the STUN client and sends it over RTP port 
 * to the STUN server.
 * The STUN server reply will be handled by the MTF STUN application which
 * will update the SDP relevant fields.
 ***************************************************************************/
static RvStatus transmitStunReqOverRtp(IN void*                ctx,
							                  IN RvChar*              msg,
							                  IN RvInt                msgSz,
							                  IN RvStunRequestHandle  req)
{
	RvIppStunAddrData*  pAddrData = (RvIppStunAddrData *)ctx;
   RvStatus ret;

	/* Get the STUN server address from the pAddr proc parameter	 */
	/* The pAddr parameter is mostly usefull when the STUN server is configured using its domain name.
	 * In this case the pAddr contains the server's IP address and port after the stun client has 
	 * discovered it */
	
   //g_StunCfg.stunServerAddress;
	//g_StunCfg.stunServerPort;

   
	/* TODO: send 'msg' with 'msgSz' over RTP 'rtpPort' to 'stunSrvaddr':'stunSrvPort' */ 
	/* In the meantime the Sip transmit proc is called otherwise the stun process gets stuck */

	ret = AclTransmitStunReqOverRtp(msg,msgSz, pAddrData, req);
   return ret ;
}

/***************************************************************************
 * transmitStunReqOverRtcp
 * ------------------------------------------------------------------------
 * This proc should be implemented by the user application. It receives a msg
 * supplied by the STUN client and sends it over RTCP port to the STUN server.
 * The STUN server reply will be handled by the MTF STUN application which
 * will update the SDP relevant fields.
 * The code below demonstrates how the RCTP port and the STUN server address
 * can be retreived from the proc arguments. This data should be used by the
 * user's RTP application when sending the msg out.
 * NOTE: the TODO line must be replaced by an RTCP send data proc.
 *
 *  This proc may reside in a more suitable file e.g. in the user's RTP application
 * 
 *  
 ***************************************************************************/
static RvStatus transmitStunReqOverRtcp(IN void*                ctx,
  							                   IN RvChar*              msg,
								                IN RvInt                msgSz,
								                IN RvStunRequestHandle  req)
{
	RvUint16    rtcpPort;
	RvIppStunAddrData*  addrData = (RvIppStunAddrData *)ctx;
  	RV_CHAR strIp[64];


	/* get the requested RTP port from ctx which contains data from the SDP m= line */
	rtcpPort = RvAddressGetIpPort(&addrData->inAddr);

       DPRINTK(">>\n");
	RvAddressGetString(&(addrData->inAddr), sizeof(strIp), strIp);

	/* Get the STUN server address from the pAddr proc parameter	 */
	/* The pAddr parameter is mostly usefull when the STUN server is configured using its domain name.
	 * In this case the pAddr contains the server's IP address and port after the stun client has 
	 * discovered it */
   //g_StunCfg.stunServerAddress;
	//g_StunCfg.stunServerPort;
	
	/* TODO: send 'msg' with 'msgSz' over RTP 'rtcpPort' to 'stunSrvaddr':'stunSrvPort' */ 
	/* In the meantime the Sip transmit proc is called otherwise the stun process gets stuck */

	AclTransmitStunReqOverRtcp(msg,msgSz, addrData, req);
   
   return RV_OK;
}




/***************************************************************************
 * AppSendBufferFunc
 * ------------------------------------------------------------------------
 * This function is given to STUN request as transmit function. That is STUN 
 * will use this function when it needs to send something to the STUN server.
 * The same function is used by all three STUN requests engaged (SIP,RTP and
 * RTCP). 
 ***************************************************************************/
static RvStatus AppSendBufferFunc(
    IN void*                ctx,
    IN RvChar*              msg,
    IN RvInt                msgSz,
    IN RvStunRequestHandle  req)
{
   RvIppStunAddrData*  addrData = (RvIppStunAddrData *)ctx;
   RvStatus ret;
   
   switch (addrData->type)
   {
   case RVIPP_SIP_VIA_FIELD:
   case RVIPP_SIP_CONTACT_FIELD:
   case RVIPP_SDP_ORIGIN_FIELD:
      ret= transmitStunReqOverSip(msg,msgSz, req);
      break;
      
   case RVIPP_SDP_MEDIA_CONN_FIELD:
   case RVIPP_SDP_MEDIA_FIELD:			
   case RVIPP_SDP_MEDIA_CLOSE_FIELD:
   case RVIPP_SDP_MEDIA_CONN_CLOSE_FIELD:
      ret= transmitStunReqOverRtp(ctx,msg,msgSz,req);
      break;
      
   case RVIPP_SDP_ATTRIBUTE_FIELD:
      ret=transmitStunReqOverRtcp(ctx,msg,msgSz,req);
      break;
      
   default:
      IppLogMessage(RV_TRUE, "AppSendBufferFunc:: unexpected address type %d", addrData->type);
 		ret =  RV_ERROR_UNKNOWN;

   }

   return ret;
}

/*-----------------------------------------------*/
/*-----------------------------------------------*/
/*-----------------------------------------------*/
RvStatus stunClientInit(RvSipStackHandle  stackHandle)
{
    RvIppStunMgrParam param;
    char local_ip[16];

   printf("stunClientInit\r\n");

   memset(&g_StunCfg, 0, sizeof(voip_stun_params_t));
    
   voip_conf_stun_get(&g_StunCfg);

    DPRINTK("stunServerAddress      %s\n", g_StunCfg.stunServerAddress);
    DPRINTK("stunServerPort         %d\n", g_StunCfg.stunServerPort );
    DPRINTK("stunNeedMask           %s\n", g_StunCfg.stunNeedMask );
    DPRINTK("stunClientResponsePort %d\n", g_StunCfg.stunClientResponsePort );
   
    /* Initialize the STUN client in the MTF. We mainly need to feed it with
       some callbacks... */

    param.isAddressResolveNeededCB = stunIsAddressResolveNeededCB;
    param.addressResolveReplyCB = stunClientReplyReceived;
    param.addressResolveStartCB = stunClientSendRequest;

    g_stunMgrHndl = RvIppStunMgrCreate(&param);
    DPRINTK("gw->stunMgrHndl %x\n", (unsigned int)g_stunMgrHndl);

    RvSipStackGetTransmitterMgrHandle(stackHandle, &g_hTrxMgr);
    
    memset(&g_stunSrvSipAddr, 0, sizeof(g_stunSrvSipAddr));
    strcpy(g_stunSrvSipAddr.strIP, g_StunCfg.stunServerAddress);        
    g_stunSrvSipAddr.port = g_StunCfg.stunServerPort;
    g_stunSrvSipAddr.eAddrType = RVSIP_TRANSPORT_ADDRESS_TYPE_IP;
    g_stunSrvSipAddr.eTransportType = RVSIP_TRANSPORT_UDP;
        
    g_stunBuf = olStunBufferConstruct();
    if (g_stunBuf == NULL)
        return RV_ERROR_UNKNOWN;

    voip_conf_local_address_get(local_ip);
    g_myLocal = inet_addr(local_ip);    
            
    return RV_OK;
}

#if 0
RvStatus stunClientEnd(IN RvIppSampleGateway* gw)
{
    if (g_stunBuf != NULL)
        olStunBufferDestruct(g_stunBuf);
        
    RvIppStunMgrDelete(g_stunMgrHndl);

    return RV_OK;
}
#endif


#else /* RV_MTF_STUN */

static int ippStunClient_dummy = 0;

#endif /* RV_MTF_STUN */

