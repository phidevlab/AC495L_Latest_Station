#if defined(RV_SIP_AUTH_ON) && defined(RV_SIP_PRIMITIVES) && !defined(RV_SIP_IMS_DCS_HEADER_SUPPORT) && !defined(RV_SIP_IMS_HEADER_SUPPORT) && !defined(RV_SIP_EXTENDED_HEADER_SUPPORT) && !defined(RV_SIP_JSR32_SUPPORT) && !defined(RV_SIP_LIGHT) && defined(RV_SIP_TEL_URI_SUPPORT) && !defined(RV_SIP_SUBS_ON)
/*
 AnaGram Parsing Engine
 Copyright (c) 1993-1999, Parsifal Software.
 All Rights Reserved.
 Serial number 2P20671
 Registered to:
   Meir Fuchs
   RADVision Ltd.
*/
#ifndef PARSERENGINESIPPRIMITIVES_H_1165245729
#include ".\ParserEngineSipPrimitives.h"
#endif
#ifndef PARSERENGINESIPPRIMITIVES_H_1165245729
#error Mismatched header file
#endif
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define RULE_CONTEXT (&((PCB).cs[(PCB).ssx]))
#define ERROR_CONTEXT ((PCB).cs[(PCB).error_frame_ssx])
#define CONTEXT ((PCB).cs[(PCB).ssx])
#ifndef PCB_TYPE
#define PCB_TYPE SipParser_pcb_type
#endif
#define PCB (*pcb_pointer)
#define PCB_DECL PCB_TYPE *pcb_pointer
#define PCB_POINTER pcb_pointer
#ifndef CONVERT_CASE
static const char agCaseTable[31] = {
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,    0,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20
};
static int agConvertCase(int c) {
  if (c >= 'a' && c <= 'z') return c ^= 0x20;
  if (c >= 0xe0 && c < 0xff) c ^= agCaseTable[c-0xe0];
  return c;
}
#define CONVERT_CASE(c) agConvertCase(c)
#endif
#ifndef TAB_SPACING
#define TAB_SPACING 8
#endif
#define ag_rp_1(PCB_POINTER) (SET_HEADER_VALUE())
#define ag_rp_2(PCB_POINTER) (SIP_BEGIN_TOKEN())
#define ag_rp_3(PCB_POINTER) (SIP_BEGIN_TOKEN())
static void ag_rp_4(PCB_DECL) {
/* Line 300, RV SIP Stack */
        SIP_BEGIN_TOKEN();
        REMOVE_COMMA_BETWEEN_HEADERS();
    
}
#define ag_rp_5(PCB_POINTER) (SIP_BEGIN_TOKEN())
#define ag_rp_6(PCB_POINTER) (SIP_BEGIN_TOKEN())
#define ag_rp_7(PCB_POINTER) (SIP_BEGIN_TOKEN())
#define ag_rp_8(PCB_POINTER) (SIP_BEGIN_TOKEN())
#define ag_rp_9(PCB_POINTER) (SIP_BEGIN_TOKEN())
#define ag_rp_10(PCB_POINTER) (SIP_BEGIN_TOKEN())
#define ag_rp_11(PCB_POINTER) (SIP_BEGIN_TOKEN())
#define ag_rp_12(PCB_POINTER) (SIP_BEGIN_TOKEN())
#define ag_rp_13(PCB_POINTER) (SIP_BEGIN_TOKEN())
#define ag_rp_14(PCB_POINTER) (SIP_BEGIN_TOKEN())
#define ag_rp_15(PCB_POINTER) (CUR_STRING())
#define ag_rp_16(PCB_POINTER) (CUR_STRING())
#define ag_rp_17(PCB_POINTER) (CUR_STRING())
#define ag_rp_18(PCB_POINTER) (CUR_STRING())
#define ag_rp_19(PCB_POINTER) (CUR_STRING())
#define ag_rp_20(PCB_POINTER) (CUR_STRING())
#define ag_rp_21(PCB_POINTER) (CUR_METHOD(PARSER_METHOD_TYPE_INVITE))
#define ag_rp_22(PCB_POINTER) (CUR_METHOD(PARSER_METHOD_TYPE_ACK))
#define ag_rp_23(PCB_POINTER) (CUR_METHOD(PARSER_METHOD_TYPE_BYE))
#define ag_rp_24(PCB_POINTER) (CUR_METHOD(PARSER_METHOD_TYPE_CANCEL))
#define ag_rp_25(PCB_POINTER) (CUR_METHOD(PARSER_METHOD_TYPE_REGISTER))
static ParserMethod ag_rp_26(PCB_DECL, ParserBasicToken t) {
/* Line 490, RV SIP Stack */
 ParserMethod method;
                            method.type = PARSER_METHOD_TYPE_OTHER;
                            method.other = t;
                            RV_UNUSED_ARG(pcb_pointer);
                            return method;
                          
}
static void ag_rp_27(PCB_DECL, ParserCSeq cseq) {
/* Line 506, RV SIP Stack */
   
       RvStatus status;
       status = ParserInitCSeq(PCB.pParserMgr, PCB.pointer,&cseq,PCB.eHeaderType,PCB.pSipObject);
       if(RV_ERROR_ILLEGAL_SYNTAX == status)
       {
           PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
           SYNTAX_ERROR;
       }
       else if (RV_OK != status)
       {
           
           PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
           PCB.eStat = status;
       }
    
}
static ParserCSeq ag_rp_28(PCB_DECL, ParserSequenceNumber sequenceNumber, ParserMethod method) {
/* Line 527, RV SIP Stack */
        ParserCSeq cseq;
        cseq.method = method;
		cseq.sequenceNumber = sequenceNumber;
        RV_UNUSED_ARG(pcb_pointer);
        return(cseq);
	 
}
#define ag_rp_29(PCB_POINTER) (CUR_STRING())
static void ag_rp_30(PCB_DECL, ParserContentLength contentLength) {
/* Line 549, RV SIP Stack */
          RvStatus status;
          contentLength.isCompact = PCB.isCompactForm;
          status = ParserInitContentLength(PCB.pParserMgr,PCB.pointer,&contentLength,PCB.pSipObject);
          if(RV_ERROR_ILLEGAL_SYNTAX == status)
          {
              PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
              SYNTAX_ERROR;
          }
          else if (RV_OK != status)
          {
            
            PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
            PCB.eStat = status;
          }
     
}
static ParserContentLength ag_rp_31(PCB_DECL) {
/* Line 572, RV SIP Stack */
        ParserContentLength contentLength;
        contentLength.contentLenVal = CUR_STRING();
        return contentLength;
    
}
static void ag_rp_32(PCB_DECL, ParserCallId callId) {
/* Line 587, RV SIP Stack */
  
          RvStatus status;
          callId.isCompact = PCB.isCompactForm;
          status = ParserInitCallId(PCB.pParserMgr,PCB.pointer,&callId,PCB.pSipObject);
          if(RV_ERROR_ILLEGAL_SYNTAX == status)
          {
              PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
              SYNTAX_ERROR;
          }
          else if(RV_OK != status)
          {
            
            PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
            PCB.eStat = status;
          }
    
}
static ParserCallId ag_rp_33(PCB_DECL, ParserBasicToken token) {
/* Line 609, RV SIP Stack */
         ParserCallId callId;
         callId.callIdVal = token;
         callId.isCompact = PCB.isCompactForm;
         return callId;
    
}
static ParserCallId ag_rp_34(PCB_DECL, ParserBasicToken token, ParserBasicToken token2) {
/* Line 616, RV SIP Stack */
        ParserCallId callId;
        token.len = (token2.len +token.len + 1);
        callId.callIdVal = token;
        callId.isCompact = PCB.isCompactForm;
        return callId;
    
}
static void ag_rp_35(PCB_DECL) {
/* Line 634, RV SIP Stack */
        RvStatus status;
        PCB.contentType.isCompactForm = PCB.isCompactForm;
        status = ParserInitContentType(PCB.pParserMgr,PCB.pointer,&(PCB.contentType),PCB.pSipObject);
        if(RV_ERROR_ILLEGAL_SYNTAX == status)
        {
             PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
             SYNTAX_ERROR;
        }
        else if (RV_OK != status) 
        {
            
            PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
            PCB.eStat = status;
        }
        ParserCleanExtParams (PCB.pExtParams);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_CONTENT_TYPE, &PCB);
     
}
static void ag_rp_36(PCB_DECL, ParserMediaType t, ParserMediaSubType st) {
/* Line 663, RV SIP Stack */
 
        PCB.contentType.mediaType    = t;
        PCB.contentType.mediaSubType = st;
        PCB.contentType.genericParamList = NULL;
        
        if (0 != ((ParserExtensionString *)PCB.pExtParams)->size)
        {
             PCB.contentType.genericParamList=PCB.pExtParams;
        }
    
}
static void ag_rp_37(PCB_DECL) {
/* Line 676, RV SIP Stack */
 
               RV_UNUSED_ARG(pcb_pointer);
            
}
static void ag_rp_38(PCB_DECL) {
/* Line 716, RV SIP Stack */
        
        
        RV_UNUSED_ARG(pcb_pointer);
    
}
#define ag_rp_39(PCB_POINTER, t) (t)
#define ag_rp_40(PCB_POINTER, t) (t)
static void ag_rp_41(PCB_DECL, ParserBasicToken token1, ParserGenericParamDataVal token2) {
/* Line 820, RV SIP Stack */
      if (0!=((ParserExtensionString *)PCB.pExtParams)->size)
      {
          
          APPEND_DATA(";", 1,PCB.pExtParams);
          APPEND_DATA(token1.buf, token1.len,PCB.pExtParams);
          APPEND_DATA("=" , 1,PCB.pExtParams);
          APPEND_DATA(token2.buf, token2.len,PCB.pExtParams);            
      }
      else
      {      
          
          APPEND_DATA(token1.buf, token1.len,PCB.pExtParams);
          APPEND_DATA("=" , 1,PCB.pExtParams);
          APPEND_DATA(token2.buf, token2.len,PCB.pExtParams);            
      }
   
}
#define ag_rp_42(PCB_POINTER, token) (token)
#define ag_rp_43(PCB_POINTER, qs) (qs)
static void ag_rp_44(PCB_DECL, ParserBasicToken token) {
/* Line 858, RV SIP Stack */
      if (0!=((ParserExtensionString *)PCB.pExtParams)->size)
      {
          
          APPEND_DATA(";", 1,PCB.pExtParams);
          APPEND_DATA(token.buf,token.len, PCB.pExtParams);
      }
      else
      {      
          
          APPEND_DATA(token.buf,token.len,PCB.pExtParams);
      }
    
}
static void ag_rp_45(PCB_DECL, ParserBasicToken token1, ParserGenericParamDataVal token2) {
/* Line 872, RV SIP Stack */
        if (0!=((ParserExtensionString *)PCB.pExtParams)->size)
        {
          
          APPEND_DATA(";", 1,PCB.pExtParams);
          APPEND_DATA(token1.buf, token1.len,PCB.pExtParams);
          APPEND_DATA("=" , 1,PCB.pExtParams);
          APPEND_DATA(token2.buf, token2.len,PCB.pExtParams);            
        }
        else
        {      
          
          APPEND_DATA(token1.buf, token1.len,PCB.pExtParams);
          APPEND_DATA("=" , 1,PCB.pExtParams);
          APPEND_DATA(token2.buf, token2.len,PCB.pExtParams);            
        }
    
}
#define ag_rp_46(PCB_POINTER, token) (token)
#define ag_rp_47(PCB_POINTER, qs) (qs)
static void ag_rp_48(PCB_DECL, ParserSipVersion ver, ParserStatusCode status, ParserReasonPhrase reason) {
/* Line 906, RV SIP Stack */
        RvStatus        stat;
        ParserStatusLine statusLine;
        statusLine.sipVersion = ver;
        statusLine.statusCode = status;
        statusLine.reasonPhrase = reason;
        stat = ParserInitStatusLine(PCB.pParserMgr,&PCB, PCB.pointer, &statusLine, PCB.pSipObject);
        if (RV_ERROR_ILLEGAL_SYNTAX == stat)
        {
            PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
            SYNTAX_ERROR;
        }
        else if (RV_OK != stat)
        {
            
            PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
            PCB.eStat = stat;
        }
    
}
static void ag_rp_49(PCB_DECL, ParserSipVersion ver, ParserStatusCode status) {
/* Line 926, RV SIP Stack */
        RvStatus        stat;
        ParserStatusLine statusLine;
        statusLine.sipVersion = ver;
        statusLine.statusCode = status;       
        statusLine.reasonPhrase.buf=NULL;
        statusLine.reasonPhrase.len=0;
        stat = ParserInitStatusLine(PCB.pParserMgr,&PCB, PCB.pointer, &statusLine, PCB.pSipObject);
        if(RV_ERROR_ILLEGAL_SYNTAX == stat)
        {
            PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
            SYNTAX_ERROR;
        }
        else if (RV_OK != stat)
        {
            
            PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
            PCB.eStat = stat;
        }
    
}
#define ag_rp_50(PCB_POINTER) (CUR_STRING())
#define ag_rp_51(PCB_POINTER) (CUR_STRING())
static ParserStatusCode ag_rp_52(PCB_DECL, int D100, int D10, int D1) {
/* Line 964, RV SIP Stack */
        RvUint16 code;
        RV_UNUSED_ARG(pcb_pointer);
        code =  (RvUint16)((D100 - '0')*100 + (D10 - '0')*10 + (D1 - '0'));
        return code;
    
}
static void ag_rp_53(PCB_DECL) {
/* Line 1028, RV SIP Stack */
 
       RvStatus status;
       PCB.singleVia.isCompact    = PCB.isCompactForm;
       status = ParserInitVia(PCB.pParserMgr, &PCB, PCB.pointer, &(PCB.singleVia),PCB.eHeaderType, PCB.pSipObject);
       if(RV_ERROR_ILLEGAL_SYNTAX == status)
       {
           PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
           SYNTAX_ERROR;
       }
       else if (RV_OK != status && RV_ERROR_NOTSUPPORTED != status)
       {
            
            PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
            PCB.eStat = status;
       }
        
       ParserCleanExtParams (PCB.pExtParams);
       if(RV_ERROR_NOTSUPPORTED != status)
       {
           ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_SINGLE_VIA, &PCB);
           ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_SENT_PROTOCOL, &PCB);
           ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_VIA_SENT_BY, &PCB);
           
       }
       
    
}
static void ag_rp_54(PCB_DECL) {
/* Line 1056, RV SIP Stack */
 
       RvStatus status;
       PCB.singleVia.isCompact    = PCB.isCompactForm;
       status = ParserInitVia(PCB.pParserMgr,&PCB, PCB.pointer,&(PCB.singleVia),PCB.eHeaderType, PCB.pSipObject);
       if(RV_ERROR_ILLEGAL_SYNTAX == status)
       {
           PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
           SYNTAX_ERROR;
       }
       else if (RV_OK != status && RV_ERROR_NOTSUPPORTED != status)
       {
            
            PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
            PCB.eStat = status;
       }
        
       ParserCleanExtParams (PCB.pExtParams);
       if(RV_ERROR_NOTSUPPORTED != status)
       {
           ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_SINGLE_VIA, &PCB);
           ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_SENT_PROTOCOL, &PCB);
           ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_VIA_SENT_BY, &PCB);
       }
    
}
static void ag_rp_55(PCB_DECL) {
/* Line 1083, RV SIP Stack */
 
        PCB.singleVia.sentProtocol = PCB.sentProtocol;
        PCB.singleVia.sentBy       = PCB.viaSentBy;
         
        if (((ParserExtensionString *)PCB.pExtParams)->size !=0)
        {
            if (PCB.singleVia.params.isOtherParams==RV_FALSE)
            {
                PCB.singleVia.params.isOtherParams = RV_TRUE;
                PCB.singleVia.params.otherParam = PCB.pExtParams;
            }
        }                
    
}
static void ag_rp_56(PCB_DECL) {
/* Line 1100, RV SIP Stack */
 
        PCB.singleVia.params.hidden     = RV_FALSE;
        PCB.singleVia.params.alias     = RV_FALSE;
        PCB.singleVia.params.isTtl      = RV_FALSE;
        PCB.singleVia.params.isMaddr    = RV_FALSE;
	    PCB.singleVia.params.isReceived = RV_FALSE;
        PCB.singleVia.params.isBranch   = RV_FALSE;
        PCB.singleVia.params.isOtherParams=RV_FALSE;  
        PCB.singleVia.params.isRport	= RV_FALSE;
        PCB.singleVia.params.isComp     = RV_FALSE;
    
}
static void ag_rp_57(PCB_DECL, ParserViaParam p) {
/* Line 1114, RV SIP Stack */
        switch( p.paramType )
        {
            case PARSER_PARAM_TYPE_HIDDEN :
                PCB.singleVia.params.hidden = RV_TRUE; 
                break;
            case PARSER_PARAM_TYPE_ALIAS:
                PCB.singleVia.params.alias = RV_TRUE; 
                break;
            case PARSER_PARAM_TYPE_TTL :
                PCB.singleVia.params.isTtl = RV_TRUE;
                PCB.singleVia.params.ttl   = p.param.ttl;
                break;
            
            case PARSER_PARAM_TYPE_MADDR :
                PCB.singleVia.params.isMaddr = RV_TRUE;
                PCB.singleVia.params.maddr   = p.param.maddr;
                break;
            
            case PARSER_PARAM_TYPE_RECEIVED :
                PCB.singleVia.params.isReceived = RV_TRUE;
                PCB.singleVia.params.received   = p.param.received;
                break;
            
            case PARSER_PARAM_TYPE_BRANCH :
                PCB.singleVia.params.isBranch = RV_TRUE;
                PCB.singleVia.params.branch   = p.param.branch;
                break;
            case PARSER_PARAM_TYPE_RPORT:
                PCB.singleVia.params.isRport = RV_TRUE;
                PCB.singleVia.params.rPort   = p.param.rport;
                break;
                 
            case PARSER_PARAM_TYPE_COMP :
                PCB.singleVia.params.isComp  = RV_TRUE;
                PCB.singleVia.params.comp    = p.param.comp;
                break;   
            default:
                if (p.isOther == RV_TRUE)
                {
                     PCB.singleVia.params.isOtherParams = RV_TRUE;
                     PCB.singleVia.params.otherParam   = p.otherParam;
                }                   
                break;
        }       
    
}
static void ag_rp_58(PCB_DECL, ParserProtocolName pn, ParserProtocolVersion pv, ParserTransport t) {
/* Line 1166, RV SIP Stack */
 
        PCB.sentProtocol.protocolName    = pn;
        PCB.sentProtocol.protocolVersion = pv;
        PCB.sentProtocol.transport       = t;
    
}
#define ag_rp_59(PCB_POINTER, t) (t)
#define ag_rp_60(PCB_POINTER) (CUR_STRING())
#define ag_rp_61(PCB_POINTER, t) (t)
#define ag_rp_62(PCB_POINTER, t) (t)
static ParserTransport ag_rp_63(PCB_DECL) {
/* Line 1193, RV SIP Stack */
        ParserTransport t;
        t.transport    = RVSIP_TRANSPORT_UDP;
        t.strTransport = CUR_STRING();
        return t;
    
}
static ParserTransport ag_rp_64(PCB_DECL) {
/* Line 1200, RV SIP Stack */
        ParserTransport t;
        t.transport    = RVSIP_TRANSPORT_TCP;
        t.strTransport = CUR_STRING();
        return t;
    
}
static ParserTransport ag_rp_65(PCB_DECL) {
/* Line 1207, RV SIP Stack */
        ParserTransport t;
        t.transport    = RVSIP_TRANSPORT_SCTP;
        t.strTransport = CUR_STRING();
        return t;
    
}
static ParserTransport ag_rp_66(PCB_DECL) {
/* Line 1214, RV SIP Stack */
        ParserTransport t;
        t.transport    = RVSIP_TRANSPORT_TLS;
        t.strTransport = CUR_STRING();
        return t;
    
}
static ParserTransport ag_rp_67(PCB_DECL, ParserBasicToken token) {
/* Line 1221, RV SIP Stack */
        ParserTransport t;
        t.transport    = RVSIP_TRANSPORT_OTHER;
        t.strTransport = token;
        RV_UNUSED_ARG(pcb_pointer);
        return t;
    
}
#define ag_rp_68(PCB_POINTER) (CUR_STRING())
static void ag_rp_69(PCB_DECL, ParserBasicToken host) {
/* Line 1240, RV SIP Stack */
 
        
        PCB.viaSentBy.isPort   = RV_FALSE;
        PCB.viaSentBy.hostName = host;
     
}
static void ag_rp_70(PCB_DECL, ParserBasicToken host, ParserPort port) {
/* Line 1246, RV SIP Stack */
 
        
        PCB.viaSentBy.isPort   = RV_TRUE;
        PCB.viaSentBy.hostName = host;
        PCB.viaSentBy.port     = port;
      
}
static ParserViaParam ag_rp_71(PCB_DECL) {
/* Line 1260, RV SIP Stack */
 ParserViaParam viaParam;
          viaParam.paramType = PARSER_PARAM_TYPE_HIDDEN;                   
          viaParam.param.hidden = RV_TRUE;
	      viaParam.isOther = RV_FALSE;
          viaParam.otherParam = NULL;
          RV_UNUSED_ARG(pcb_pointer);
          return viaParam;
        
}
static ParserViaParam ag_rp_72(PCB_DECL) {
/* Line 1269, RV SIP Stack */
 ParserViaParam viaParam;
          viaParam.paramType = PARSER_PARAM_TYPE_ALIAS;                   
          viaParam.param.alias = RV_TRUE;
	      viaParam.isOther = RV_FALSE;
          viaParam.otherParam = NULL;
          RV_UNUSED_ARG(pcb_pointer);
          return viaParam;
        
}
static ParserViaParam ag_rp_73(PCB_DECL, ParserTTLParam t) {
/* Line 1278, RV SIP Stack */
 ParserViaParam viaParam;
          viaParam.paramType = PARSER_PARAM_TYPE_TTL;                   
          viaParam.param.ttl = t;
	      viaParam.isOther = RV_FALSE;
          viaParam.otherParam = NULL;
          RV_UNUSED_ARG(pcb_pointer);
          return viaParam;
        
}
static ParserViaParam ag_rp_74(PCB_DECL, ParserMaddrParam ma) {
/* Line 1287, RV SIP Stack */
 ParserViaParam viaParam;
          viaParam.paramType = PARSER_PARAM_TYPE_MADDR;                   
          viaParam.param.maddr = ma;
	      viaParam.isOther = RV_FALSE;
          viaParam.otherParam = NULL;
          RV_UNUSED_ARG(pcb_pointer);
          return viaParam;
        
}
static ParserViaParam ag_rp_75(PCB_DECL, ParserReceived r) {
/* Line 1296, RV SIP Stack */
 ParserViaParam viaParam;
          viaParam.paramType = PARSER_PARAM_TYPE_RECEIVED;                   
          viaParam.param.received = r;
	      viaParam.isOther = RV_FALSE;
          viaParam.otherParam = NULL;
          RV_UNUSED_ARG(pcb_pointer);
          return viaParam;
        
}
static ParserViaParam ag_rp_76(PCB_DECL, ParserBranch b) {
/* Line 1305, RV SIP Stack */
 ParserViaParam viaParam;
          viaParam.paramType = PARSER_PARAM_TYPE_BRANCH;                   
          viaParam.param.branch = b;
	      viaParam.isOther = RV_FALSE;
          viaParam.otherParam = NULL;
          RV_UNUSED_ARG(pcb_pointer);
          return viaParam;
        
}
static ParserViaParam ag_rp_77(PCB_DECL, ParserPort rport) {
/* Line 1315, RV SIP Stack */
          ParserViaParam viaParam;
          viaParam.paramType = PARSER_PARAM_TYPE_RPORT;                   
          viaParam.param.rport = rport;
	      viaParam.isOther = RV_FALSE;
          viaParam.otherParam = NULL;
          RV_UNUSED_ARG(pcb_pointer);
          return viaParam;
        
}
static ParserViaParam ag_rp_78(PCB_DECL, ParserCompParam comp) {
/* Line 1324, RV SIP Stack */
 ParserViaParam viaParam;
          viaParam.paramType  = PARSER_PARAM_TYPE_COMP;                   
          viaParam.param.comp = comp;
	      viaParam.isOther    = RV_FALSE;
          viaParam.otherParam = NULL;
          RV_UNUSED_ARG(pcb_pointer);
          return viaParam;
        
}
static ParserViaParam ag_rp_79(PCB_DECL) {
/* Line 1333, RV SIP Stack */
 ParserViaParam viaParam;
          viaParam.paramType = PARSER_PARAM_TYPE_OTHER; 
	      viaParam.isOther = RV_TRUE;
          viaParam.otherParam = PCB.pExtParams;
          return viaParam;    
        
}
static void ag_rp_80(PCB_DECL, ParserBasicToken token) {
/* Line 1350, RV SIP Stack */
        if (((ParserExtensionString *)PCB.pExtParams)->size !=0)
        {
            
            APPEND_DATA(";", 1, PCB.pExtParams);
            APPEND_DATA(token.buf, token.len,PCB.pExtParams);
        }
        else
        {      
            
            APPEND_DATA(token.buf, token.len,PCB.pExtParams);
        }
    
}
static void ag_rp_81(PCB_DECL, ParserBasicToken token, ParserBasicToken token2) {
/* Line 1365, RV SIP Stack */
        if (0!=((ParserExtensionString *)PCB.pExtParams)->size)
        {
            
            APPEND_DATA(";", 1,PCB.pExtParams);
            APPEND_DATA(token.buf, token.len,PCB.pExtParams);
            APPEND_DATA("=", 1,PCB.pExtParams);
            APPEND_DATA(token2.buf, token2.len,PCB.pExtParams);
        }
        else
        {      
            
            APPEND_DATA(token.buf, token.len,PCB.pExtParams);
            APPEND_DATA("=", 1,PCB.pExtParams);
            APPEND_DATA(token2.buf, token2.len,PCB.pExtParams);
        }
    
}
static void ag_rp_82(PCB_DECL, ParserBasicToken token, ParserQuotedString token2) {
/* Line 1384, RV SIP Stack */
        if (((ParserExtensionString *)PCB.pExtParams)->size !=0)
        {
            
            APPEND_DATA(";", 1,PCB.pExtParams);
            APPEND_DATA(token.buf, token.len,PCB.pExtParams);
            APPEND_DATA("=", 1,PCB.pExtParams);
            APPEND_DATA(token2.buf, token2.len,PCB.pExtParams);
        }
        else
        {      
            
            APPEND_DATA(token.buf, token.len,PCB.pExtParams);
            APPEND_DATA("=", 1,PCB.pExtParams);
            APPEND_DATA(token2.buf, token2.len,PCB.pExtParams);
        }
    
}
static void ag_rp_83(PCB_DECL, ParserBasicToken token, ParserBasicToken token2) {
/* Line 1407, RV SIP Stack */
        if (((ParserExtensionString *)PCB.pExtParams)->size !=0)
        {
            
            APPEND_DATA(";", 1,PCB.pExtParams);
            APPEND_DATA(token.buf, token.len,PCB.pExtParams);
            APPEND_DATA("=", 1,PCB.pExtParams);
            APPEND_DATA(token2.buf, token2.len,PCB.pExtParams);
        }
        else
        {      
            
            APPEND_DATA(token.buf, token.len,PCB.pExtParams);
            APPEND_DATA("=", 1,PCB.pExtParams);
            APPEND_DATA(token2.buf, token2.len,PCB.pExtParams);
        }
    
}
#define ag_rp_84(PCB_POINTER, t) (t)
#define ag_rp_85(PCB_POINTER, ttl) (ttl)
#define ag_rp_86(PCB_POINTER) (CUR_STRING())
#define ag_rp_87(PCB_POINTER) (CUR_STRING())
#define ag_rp_88(PCB_POINTER) (CUR_STRING())
#define ag_rp_89(PCB_POINTER, rport) (rport)
static ParserPort ag_rp_90(PCB_DECL) {
/* Line 1462, RV SIP Stack */
        ParserPort port;
        port.len = 0;
		port.buf = NULL;
        RV_UNUSED_ARG(pcb_pointer);
        return port;
    
}
#define ag_rp_91(PCB_POINTER, port) (port)
#define ag_rp_92(PCB_POINTER, ma) (ma)
#define ag_rp_93(PCB_POINTER, host) (host)
#define ag_rp_94(PCB_POINTER, host) (host)
#define ag_rp_95(PCB_POINTER, t) (t)
#define ag_rp_96(PCB_POINTER, comp) (comp)
static ParserCompParam ag_rp_97(PCB_DECL) {
/* Line 1499, RV SIP Stack */
        ParserCompParam comp;
        comp.compType     = RVSIP_COMP_SIGCOMP;
        comp.strCompParam = CUR_STRING();
        return comp;
    
}
static ParserCompParam ag_rp_98(PCB_DECL, ParserBasicToken token) {
/* Line 1506, RV SIP Stack */
        ParserCompParam comp;
        comp.compType     = RVSIP_COMP_OTHER;
        comp.strCompParam = CUR_STRING();
        RV_UNUSED_ARG(token.buf);
        return comp;
    
}
#define ag_rp_99(PCB_POINTER) (CUR_STRING())
static void ag_rp_100(PCB_DECL, ParserAbsoluteUri uri) {
/* Line 1737, RV SIP Stack */
       RvStatus status;
       status = ParserInitAbsUri(PCB.pParserMgr, &PCB, &uri, PCB.eHeaderType,PCB.pSipObject);
       if(RV_ERROR_ILLEGAL_SYNTAX == status)
       {
           PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
           SYNTAX_ERROR;
       }
       else if (status != RV_OK)
       {
            
            PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
            PCB.eStat = status;
       }
    
}
static ParserAbsoluteUri ag_rp_101(PCB_DECL, ParserBasicToken scheme, ParserBasicToken id) {
/* Line 1757, RV SIP Stack */
        ParserAbsoluteUri uri;
        uri.scheme = scheme;
        uri.idefntifier = id;
        RV_UNUSED_ARG(pcb_pointer);
        return uri;
    
}
#define ag_rp_102(PCB_POINTER) (CUR_STRING())
#define ag_rp_103(PCB_POINTER, displayName) (displayName)
#define ag_rp_104(PCB_POINTER) (CUR_STRING())
#define ag_rp_105(PCB_POINTER) (CUR_STRING())
static void ag_rp_106(PCB_DECL) {
/* Line 1798, RV SIP Stack */
        RvStatus status;
        status = ParserInitTelUri(PCB.pParserMgr, &PCB, &(PCB.telUri), PCB.eHeaderType,PCB.pSipObject);
        if(RV_ERROR_ILLEGAL_SYNTAX == status)
        {
            PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
            SYNTAX_ERROR;
        }
        else if (RV_OK != status)
        {
            
            PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
            PCB.eStat = status;
        }
		ParserCleanExtParams (PCB.pUrlExtParams);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_SIP_URL, &PCB);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_URL_PARAMETER, &PCB);
    
}
static void ag_rp_107(PCB_DECL) {
/* Line 1855, RV SIP Stack */
		SIP_BEGIN_TOKEN();
	
}
static void ag_rp_108(PCB_DECL) {
/* Line 1860, RV SIP Stack */
  
         
         if (0 != ((ParserExtensionString *)PCB.pUrlExtParams)->size)
         {
              PCB.telUri.strOtherParams.Params         = PCB.pUrlExtParams;
              PCB.telUri.strOtherParams.bIsSpecified   = RV_TRUE;
         }          
  	
}
static void ag_rp_109(PCB_DECL, ParserBasicToken num) {
/* Line 1872, RV SIP Stack */
		PCB.telUri.strPhoneNumber.bIsSpecified = RV_TRUE;
		PCB.telUri.strPhoneNumber.strToken     = num;
		PCB.telUri.bIsGlobalPhoneNumber        = RV_TRUE;
	
}
static void ag_rp_110(PCB_DECL, ParserBasicToken num) {
/* Line 1878, RV SIP Stack */
		PCB.telUri.strPhoneNumber.bIsSpecified = RV_TRUE;
		PCB.telUri.strPhoneNumber.strToken     = num;
		PCB.telUri.bIsGlobalPhoneNumber        = RV_FALSE;
	
}
static ParserBasicToken ag_rp_111(PCB_DECL) {
/* Line 1888, RV SIP Stack */
		ParserBasicToken val;
		
		val.buf = SIP_TOKEN_START+1;
		val.len = SIP_TOKEN_LENGTH-1;  
		return val;
	
}
#define ag_rp_112(PCB_POINTER) (CUR_STRING())
static void ag_rp_113(PCB_DECL) {
/* Line 1917, RV SIP Stack */
 
		PCB.telUri.strExtension.bIsSpecified   = RV_FALSE;
		PCB.telUri.strPostDial.bIsSpecified    = RV_FALSE;
		PCB.telUri.strIsdnSubAddr.bIsSpecified = RV_FALSE;
		PCB.telUri.strContext.bIsSpecified     = RV_FALSE;
		PCB.telUri.strOtherParams.bIsSpecified = RV_FALSE;
		PCB.telUri.eEnumdiType                 = ParserEnumdiParamUndefined;
    
}
static void ag_rp_114(PCB_DECL) {
/* Line 1931, RV SIP Stack */
 
        PCB.telUri.strExtension.bIsSpecified   = RV_FALSE;
		PCB.telUri.strPostDial.bIsSpecified    = RV_FALSE;
		PCB.telUri.strIsdnSubAddr.bIsSpecified = RV_FALSE;
		PCB.telUri.strContext.bIsSpecified     = RV_FALSE;
		PCB.telUri.strOtherParams.bIsSpecified = RV_FALSE;
		PCB.telUri.eEnumdiType                 = ParserEnumdiParamUndefined;
         								
        switch(PCB.telUriParameter.paramType)
        {
            case PARSER_TEL_URI_EXTENSION_PARAM: 
	            PCB.telUri.strExtension.bIsSpecified        = RV_TRUE;
	            PCB.telUri.strExtension.strToken            = PCB.telUriParameter.param.strExtension; 
	            break;
            case PARSER_TEL_URI_POST_DIAL_PARAM: 
	            PCB.telUri.strPostDial.bIsSpecified         = RV_TRUE;
	            PCB.telUri.strPostDial.strToken             = PCB.telUriParameter.param.strPostDial; 
	            break;
            case PARSER_TEL_URI_ISDN_SUB_ADDR_PARAM: 
	            PCB.telUri.strIsdnSubAddr.bIsSpecified      = RV_TRUE;
	            PCB.telUri.strIsdnSubAddr.strToken          = PCB.telUriParameter.param.strIsdnSubAddr; 
	            break;
            case PARSER_TEL_URI_CONTEXT_PARAM: 
	            PCB.telUri.strContext.bIsSpecified          = RV_TRUE;
	            PCB.telUri.strContext.strToken              = PCB.telUriParameter.param.strContext; 
	            break;
			case PARSER_TEL_URI_ENUMDI_PARAM: 
				PCB.telUri.eEnumdiType                      = PCB.telUriParameter.param.eEnumdiType;
            case PARSER_TEL_URI_OTHER_PARAM:    
	            break;
             default:
                 PCB.exit_flag = AG_SEMANTIC_ERROR_CODE;
                 PCB.eStat = RV_ERROR_ILLEGAL_SYNTAX;            
        } 	 														
    
}
static void ag_rp_115(PCB_DECL) {
/* Line 1967, RV SIP Stack */
 
         
        switch(PCB.telUriParameter.paramType)
        {
			case PARSER_TEL_URI_EXTENSION_PARAM: 
	            PCB.telUri.strExtension.bIsSpecified        = RV_TRUE;
	            PCB.telUri.strExtension.strToken            = PCB.telUriParameter.param.strExtension; 
	            break;
            case PARSER_TEL_URI_POST_DIAL_PARAM: 
	            PCB.telUri.strPostDial.bIsSpecified         = RV_TRUE;
	            PCB.telUri.strPostDial.strToken             = PCB.telUriParameter.param.strPostDial; 
	            break;
            case PARSER_TEL_URI_ISDN_SUB_ADDR_PARAM: 
	            PCB.telUri.strIsdnSubAddr.bIsSpecified      = RV_TRUE;
	            PCB.telUri.strIsdnSubAddr.strToken          = PCB.telUriParameter.param.strIsdnSubAddr; 
	            break;
            case PARSER_TEL_URI_CONTEXT_PARAM: 
	            PCB.telUri.strContext.bIsSpecified          = RV_TRUE;
	            PCB.telUri.strContext.strToken              = PCB.telUriParameter.param.strContext; 
	            break;
            case PARSER_TEL_URI_ENUMDI_PARAM: 
				PCB.telUri.eEnumdiType                      = PCB.telUriParameter.param.eEnumdiType;
            case PARSER_TEL_URI_OTHER_PARAM:    
                PCB.telUri.strOtherParams.bIsSpecified      = RV_TRUE;
		        break; 
            default:
                PCB.exit_flag = AG_SEMANTIC_ERROR_CODE;
                PCB.eStat = RV_ERROR_ILLEGAL_SYNTAX;                  
        }	 
    
}
static void ag_rp_116(PCB_DECL, ParserBasicToken ext) {
/* Line 2001, RV SIP Stack */
 
         PCB.telUriParameter.paramType = PARSER_TEL_URI_EXTENSION_PARAM;
         PCB.telUriParameter.param.strExtension = ext;
    
}
static void ag_rp_117(PCB_DECL, ParserBasicToken pstd) {
/* Line 2006, RV SIP Stack */
 
		PCB.telUriParameter.paramType = PARSER_TEL_URI_POST_DIAL_PARAM;
		PCB.telUriParameter.param.strPostDial = pstd;
	
}
static void ag_rp_118(PCB_DECL, ParserBasicToken isub) {
/* Line 2011, RV SIP Stack */
 
		PCB.telUriParameter.paramType = PARSER_TEL_URI_ISDN_SUB_ADDR_PARAM;
		PCB.telUriParameter.param.strIsdnSubAddr = isub;
	
}
static void ag_rp_119(PCB_DECL, ParserBasicToken cntx) {
/* Line 2016, RV SIP Stack */
 
		PCB.telUriParameter.paramType = PARSER_TEL_URI_CONTEXT_PARAM;
		PCB.telUriParameter.param.strContext = cntx;
	
}
static void ag_rp_120(PCB_DECL, ParserEnumdiParamType enumdi) {
/* Line 2021, RV SIP Stack */
 
		PCB.telUriParameter.paramType = PARSER_TEL_URI_ENUMDI_PARAM;
		PCB.telUriParameter.param.eEnumdiType   = enumdi;
	
}
static void ag_rp_121(PCB_DECL) {
/* Line 2027, RV SIP Stack */
	     PCB.telUriParameter.paramType = PARSER_TEL_URI_OTHER_PARAM;
    
}
#define ag_rp_122(PCB_POINTER, ext) (ext)
#define ag_rp_123(PCB_POINTER) (CUR_STRING())
#define ag_rp_124(PCB_POINTER, postd) (postd)
#define ag_rp_125(PCB_POINTER) (CUR_STRING())
#define ag_rp_126(PCB_POINTER, isub) (isub)
#define ag_rp_127(PCB_POINTER) (CUR_STRING())
#define ag_rp_128(PCB_POINTER, cntx) (cntx)
#define ag_rp_129(PCB_POINTER) (CUR_STRING())
static void ag_rp_130(PCB_DECL) {
/* Line 2105, RV SIP Stack */
		if (PCB.isTopLabel == RV_FALSE)
		{
			PCB.exit_flag = AG_SEMANTIC_ERROR_CODE;
			PCB.eStat = RV_ERROR_ILLEGAL_SYNTAX;
		}
	
}
static void ag_rp_131(PCB_DECL) {
/* Line 2115, RV SIP Stack */
		
		if (LAST_CHAR_IS_HYPHEN())
		{
			PCB.exit_flag = AG_SEMANTIC_ERROR_CODE;
			PCB.eStat = RV_ERROR_ILLEGAL_SYNTAX;
		}
		PCB.isTopLabel = RV_TRUE;
	
}
static void ag_rp_132(PCB_DECL) {
/* Line 2125, RV SIP Stack */
		
		if (LAST_CHAR_IS_HYPHEN())
		{
			PCB.exit_flag = AG_SEMANTIC_ERROR_CODE;
			PCB.eStat = RV_ERROR_ILLEGAL_SYNTAX;
		}
		PCB.isTopLabel = RV_FALSE;
	
}
#define ag_rp_133(PCB_POINTER) (ParserEnumdiParamEmpty)
static void ag_rp_134(PCB_DECL) {
/* Line 2158, RV SIP Stack */
        RvStatus status;
        status = ParserInitUrl(PCB.pParserMgr,&PCB,&(PCB.sipUrl), PCB.eHeaderType,PCB.pSipObject);
        if(RV_ERROR_ILLEGAL_SYNTAX == status)
        {
            PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
            SYNTAX_ERROR;
        }
        else if (RV_OK != status)
        {
            
            PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
            PCB.eStat = status;
        }
        ParserCleanExtParams (PCB.pUrlExtParams);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_SIP_URL, &PCB);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_URL_PARAMETER, &PCB);
    
}
#define ag_rp_135(PCB_POINTER, uri) (INIT_PRES_IM_STRUCT(uri))
#define ag_rp_136(PCB_POINTER, uri) (INIT_PRES_IM_STRUCT(uri))
static void ag_rp_137(PCB_DECL) {
/* Line 2209, RV SIP Stack */
        PCB.sipUrl.bIsSecure = RV_FALSE;
        SIP_BEGIN_TOKEN();
        DEFINE_URL_PREFIX();
    
}
static void ag_rp_138(PCB_DECL) {
/* Line 2215, RV SIP Stack */
        PCB.sipUrl.bIsSecure = RV_TRUE;
        SIP_BEGIN_TOKEN();
        DEFINE_URL_PREFIX();
    
}
#define ag_rp_139(PCB_POINTER, uri) (uri)
static ParserAbsoluteUri ag_rp_140(PCB_DECL) {
/* Line 2227, RV SIP Stack */
        ParserAbsoluteUri uri;
        
        uri.scheme.len = 0;
		uri.scheme.buf = NULL;
        uri.idefntifier.len = 0;
		uri.idefntifier.buf = NULL;
        RV_UNUSED_ARG(pcb_pointer)
        return uri;
    
}
#define ag_rp_141(PCB_POINTER) (SET_PARSER_PCB_BACK_TO_BUFF(RV_SIPPARSER_PREFIX_RV_ABS))
static void ag_rp_142(PCB_DECL) {
/* Line 2243, RV SIP Stack */
        SIP_BEGIN_TOKEN();
        DEFINE_PRES_IM_PREFIX(RV_FALSE);
    
}
#define ag_rp_143(PCB_POINTER, uri) (uri)
static ParserAbsoluteUri ag_rp_144(PCB_DECL) {
/* Line 2253, RV SIP Stack */
        ParserAbsoluteUri uri;
        
        uri.scheme.len = 0;
        uri.scheme.buf = NULL;
        uri.idefntifier.len = 0;
		uri.idefntifier.buf = NULL;
        RV_UNUSED_ARG(pcb_pointer)
        return uri;
    
}
static void ag_rp_145(PCB_DECL) {
/* Line 2266, RV SIP Stack */
        SIP_BEGIN_TOKEN();
        DEFINE_PRES_IM_PREFIX(RV_TRUE);
    
}
static void ag_rp_146(PCB_DECL, ParserOptionalHeaders hs) {
/* Line 2274, RV SIP Stack */
  
           
         PCB.sipUrl.optionalHeaders = hs;
         
         if (0 != ((ParserExtensionString *)PCB.pUrlExtParams)->size)
         {
              PCB.sipUrl.urlParameters.genericParamList= PCB.pUrlExtParams;
              PCB.sipUrl.urlParameters.isOtherParams   = RV_TRUE;
         }          
  	
}
static void ag_rp_147(PCB_DECL, ParserOptionalHeaders hs) {
/* Line 2285, RV SIP Stack */
  
         
       PCB.sipUrl.optionalHeaders = hs;
       
       if (0 != ((ParserExtensionString *)PCB.pUrlExtParams)->size)
       {
            PCB.sipUrl.urlParameters.genericParamList=PCB.pUrlExtParams;
            PCB.sipUrl.urlParameters.isOtherParams   = RV_TRUE;
       }          
	
}
static void ag_rp_148(PCB_DECL, ParserOptionalHeaders hs) {
/* Line 2315, RV SIP Stack */
 
        RESET_URI_PARAMS();
        PCB.sipUrl.optionalHeaders.isSpecified = RV_FALSE;
        PCB.sipUrl.optionalHeaders = hs;
        PCB.exUri.uriType = PARSER_ADDR_PARAM_TYPE_SIP_URL;
        PCB.exUri.ExUriInfo.SipUrl = PCB.sipUrl;
      
}
static void ag_rp_149(PCB_DECL, ParserOptionalHeaders hs) {
/* Line 2324, RV SIP Stack */
 
        RESET_URI_PARAMS();
        PCB.sipUrl.optionalHeaders.isSpecified = RV_FALSE;
        PCB.sipUrl.optionalHeaders = hs;
        
        PCB.exUri.uriType = PARSER_ADDR_PARAM_TYPE_SIP_URL;
        PCB.exUri.ExUriInfo.SipUrl = PCB.sipUrl;
      
}
static void ag_rp_150(PCB_DECL) {
/* Line 2337, RV SIP Stack */
		PCB.telUri.strExtension.bIsSpecified   = RV_FALSE;
		PCB.telUri.strPostDial.bIsSpecified    = RV_FALSE;
		PCB.telUri.strIsdnSubAddr.bIsSpecified = RV_FALSE;
		PCB.telUri.strContext.bIsSpecified     = RV_FALSE;
		PCB.telUri.strOtherParams.bIsSpecified = RV_FALSE;
		
		PCB.exUri.uriType = PARSER_ADDR_PARAM_TYPE_TEL_URI;
        PCB.exUri.ExUriInfo.telUri = PCB.telUri;
	
}
static void ag_rp_151(PCB_DECL, ParserOptionalHeaders hs) {
/* Line 2351, RV SIP Stack */
 
        RESET_URI_PARAMS();
        PCB.sipUrl.optionalHeaders.isSpecified = RV_FALSE;
        PCB.sipUrl.optionalHeaders = hs;
        PCB.exUri.uriType = PARSER_ADDR_PARAM_TYPE_PRES_URI;
        PCB.exUri.ExUriInfo.SipUrl = PCB.sipUrl;
      
}
static void ag_rp_152(PCB_DECL, ParserOptionalHeaders hs) {
/* Line 2360, RV SIP Stack */
 
        RESET_URI_PARAMS();
        PCB.sipUrl.optionalHeaders.isSpecified = RV_FALSE;
        PCB.sipUrl.optionalHeaders = hs;
        
        PCB.exUri.uriType = PARSER_ADDR_PARAM_TYPE_PRES_URI;
        PCB.exUri.ExUriInfo.SipUrl = PCB.sipUrl;
      
}
static void ag_rp_153(PCB_DECL, ParserOptionalHeaders hs) {
/* Line 2372, RV SIP Stack */
 
        RESET_URI_PARAMS();
        PCB.sipUrl.optionalHeaders.isSpecified = RV_FALSE;
        PCB.sipUrl.optionalHeaders = hs;
        PCB.exUri.uriType = PARSER_ADDR_PARAM_TYPE_IM_URI;
        PCB.exUri.ExUriInfo.SipUrl = PCB.sipUrl;
      
}
static void ag_rp_154(PCB_DECL, ParserOptionalHeaders hs) {
/* Line 2381, RV SIP Stack */
 
        RESET_URI_PARAMS();
        PCB.sipUrl.optionalHeaders.isSpecified = RV_FALSE;
        PCB.sipUrl.optionalHeaders = hs;
        
        PCB.exUri.uriType = PARSER_ADDR_PARAM_TYPE_IM_URI;
        PCB.exUri.ExUriInfo.SipUrl = PCB.sipUrl;
      
}
static void ag_rp_155(PCB_DECL, ParserDisplayName scheme, ParserBasicToken id) {
/* Line 2395, RV SIP Stack */
          PCB.exUri.ExUriInfo.absUri.scheme = scheme;
          PCB.exUri.ExUriInfo.absUri.idefntifier = id;
          PCB.exUri.uriType = PARSER_ADDR_PARAM_TYPE_ABS_URI;
      
}
static void ag_rp_156(PCB_DECL) {
/* Line 2405, RV SIP Stack */
        PCB.sipUrl.bIsUserInfo = RV_TRUE;
    
}
static void ag_rp_157(PCB_DECL) {
/* Line 2410, RV SIP Stack */
        PCB.sipUrl.bIsUserInfo = RV_FALSE;
    
}
static void ag_rp_158(PCB_DECL) {
/* Line 2417, RV SIP Stack */
        SET_PARSER_PCB_BACK_TO_BUFF(RV_SIPPARSER_PREFIX_USERINFO);
    
}
static void ag_rp_159(PCB_DECL, ParserBasicToken user) {
/* Line 2423, RV SIP Stack */
        PCB.sipUrl.urlUserInfo.userName   = user;
        PCB.sipUrl.urlUserInfo.isPassword = RV_FALSE;
    
}
static void ag_rp_160(PCB_DECL, ParserBasicToken user, ParserBasicToken pass) {
/* Line 2428, RV SIP Stack */
        PCB.sipUrl.urlUserInfo.userName   = user;
        PCB.sipUrl.urlUserInfo.isPassword = RV_TRUE;
        PCB.sipUrl.urlUserInfo.password   = pass;
     
}
#define ag_rp_161(PCB_POINTER, t) (t)
#define ag_rp_162(PCB_POINTER) (CUR_STRING())
#define ag_rp_163(PCB_POINTER) (CUR_STRING())
static void ag_rp_164(PCB_DECL) {
/* Line 2461, RV SIP Stack */
        PCB.sipUrl.bIsUserInfo = RV_FALSE;
    
}
static void ag_rp_165(PCB_DECL) {
/* Line 2467, RV SIP Stack */
        SET_PARSER_PCB_BACK_TO_BUFF(RV_SIPPARSER_PREFIX_HOST);
    
}
static void ag_rp_166(PCB_DECL, ParserBasicToken host) {
/* Line 2475, RV SIP Stack */
        PCB.sipUrl.urlHost.hostName = host;
        PCB.sipUrl.urlHost.isPort   = RV_FALSE;
    
}
static void ag_rp_167(PCB_DECL, ParserBasicToken host, ParserPort port) {
/* Line 2481, RV SIP Stack */
        PCB.sipUrl.urlHost.hostName = host;
        PCB.sipUrl.urlHost.isPort   = RV_TRUE;
        PCB.sipUrl.urlHost.port     = port;
    
}
#define ag_rp_168(PCB_POINTER, t) (t)
#define ag_rp_169(PCB_POINTER, ip6ref) (ip6ref)
#define ag_rp_170(PCB_POINTER) (CUR_STRING())
#define ag_rp_171(PCB_POINTER) (CUR_STRING())
#define ag_rp_172(PCB_POINTER) (CUR_STRING())
#define ag_rp_173(PCB_POINTER) (RESET_URI_PARAMS())
static void ag_rp_174(PCB_DECL) {
/* Line 2535, RV SIP Stack */
	
       PCB.sipUrl.urlParameters.isValid = RV_TRUE;
    
}
static void ag_rp_175(PCB_DECL) {
/* Line 2548, RV SIP Stack */
 
        RESET_URI_PARAMS();  
         								
        switch( PCB.urlParameter.paramType )
        {
            case PARSER_TRANSPORT_PARAM: 
	            PCB.sipUrl.urlParameters.isTransport        = RV_TRUE;
	            PCB.sipUrl.urlParameters.transport          = PCB.urlParameter.param.transport; 
	            break;
            case PARSER_USER_PARAM: 
	            PCB.sipUrl.urlParameters.isUserParam        = RV_TRUE;
	            PCB.sipUrl.urlParameters.userParam          = PCB.urlParameter.param.userParam; 
	            break;		
            case PARSER_TTL_PARAM: 
	            PCB.sipUrl.urlParameters.isTtlParam         = RV_TRUE;
	            PCB.sipUrl.urlParameters.ttlParam           = PCB.urlParameter.param.ttlParam; 
	            break;
            case PARSER_MADDR_PARAM: 
	            PCB.sipUrl.urlParameters.isMaddrParam       = RV_TRUE;
	            PCB.sipUrl.urlParameters.maddrParam         = PCB.urlParameter.param.maddrParam; 
	            break;
            case PARSER_METHOD_PARAM: 
	            PCB.sipUrl.urlParameters.isMethodParam       = RV_TRUE;
	            PCB.sipUrl.urlParameters.methodParam         = PCB.urlParameter.param.methodParam; 
	            break;
            case PARSER_LR_PARAM: 
	            PCB.sipUrl.urlParameters.lrParamType         = PCB.urlParameter.param.lrParamType;
	            break;
            case PARSER_COMP_PARAM:
                PCB.sipUrl.urlParameters.isCompParam         = RV_TRUE;
                PCB.sipUrl.urlParameters.compParam           = PCB.urlParameter.param.compParam;
                break;
			case PARSER_TOKENIZED_BY_PARAM: 
		        PCB.sipUrl.urlParameters.isTokenizedByParam	= RV_TRUE;
		        PCB.sipUrl.urlParameters.tokenizedByParam	= PCB.urlParameter.param.tokenizedByParam; 
		        break;
			case PARSER_ORIG_PARAM: 
		        PCB.sipUrl.urlParameters.bOrigParam			= PCB.urlParameter.param.bOrigParam;
		        break;
            case PARSER_OTHER_PARAM:    
	            break;
             default:
                 PCB.exit_flag = AG_SEMANTIC_ERROR_CODE;
                 PCB.eStat = RV_ERROR_ILLEGAL_SYNTAX;            
        } 	 														
    
}
static void ag_rp_176(PCB_DECL) {
/* Line 2595, RV SIP Stack */
 
         
        switch( PCB.urlParameter.paramType )
        {
	        case PARSER_TRANSPORT_PARAM: 
		        PCB.sipUrl.urlParameters.isTransport        = RV_TRUE;
		        PCB.sipUrl.urlParameters.transport          = PCB.urlParameter.param.transport; 
		        break;
	        case PARSER_USER_PARAM: 
		        PCB.sipUrl.urlParameters.isUserParam        = RV_TRUE;
		        PCB.sipUrl.urlParameters.userParam          = PCB.urlParameter.param.userParam; 
		        break;	
	        case PARSER_TTL_PARAM: 
		        PCB.sipUrl.urlParameters.isTtlParam         = RV_TRUE;
		        PCB.sipUrl.urlParameters.ttlParam           = PCB.urlParameter.param.ttlParam; 
		        break;
	        case PARSER_MADDR_PARAM: 
		        PCB.sipUrl.urlParameters.isMaddrParam       = RV_TRUE;
		        PCB.sipUrl.urlParameters.maddrParam         = PCB.urlParameter.param.maddrParam; 
		        break;  
            case PARSER_METHOD_PARAM: 
		        PCB.sipUrl.urlParameters.isMethodParam		= RV_TRUE;
		        PCB.sipUrl.urlParameters.methodParam		= PCB.urlParameter.param.methodParam; 
		        break; 
            case PARSER_LR_PARAM: 
		        PCB.sipUrl.urlParameters.lrParamType		= PCB.urlParameter.param.lrParamType;
		        break; 
            case PARSER_COMP_PARAM:
                PCB.sipUrl.urlParameters.isCompParam		= RV_TRUE;
                PCB.sipUrl.urlParameters.compParam			= PCB.urlParameter.param.compParam;
                break;
			case PARSER_TOKENIZED_BY_PARAM: 
		        PCB.sipUrl.urlParameters.isTokenizedByParam	= RV_TRUE;
		        PCB.sipUrl.urlParameters.tokenizedByParam	= PCB.urlParameter.param.tokenizedByParam; 
		        break;
			case PARSER_ORIG_PARAM: 
		        PCB.sipUrl.urlParameters.bOrigParam			= PCB.urlParameter.param.bOrigParam;
		        break;
            case PARSER_OTHER_PARAM:    
		        PCB.sipUrl.urlParameters.isOtherParams		= RV_TRUE;
		        break; 
            default:
                PCB.exit_flag = AG_SEMANTIC_ERROR_CODE;
                PCB.eStat = RV_ERROR_ILLEGAL_SYNTAX;                  
        }	 
    
}
static void ag_rp_177(PCB_DECL, ParserTransport t) {
/* Line 2645, RV SIP Stack */
 
         PCB.urlParameter.paramType = PARSER_TRANSPORT_PARAM;
         PCB.urlParameter.param.transport = t;
    
}
static void ag_rp_178(PCB_DECL, ParserUserParam u) {
/* Line 2650, RV SIP Stack */
 
	     PCB.urlParameter.paramType = PARSER_USER_PARAM;
		 PCB.urlParameter.param.userParam = u;
	
}
static void ag_rp_179(PCB_DECL, ParserTtlParam t) {
/* Line 2655, RV SIP Stack */
 
	     PCB.urlParameter.paramType = PARSER_TTL_PARAM;
		 PCB.urlParameter.param.ttlParam = t;
	
}
static void ag_rp_180(PCB_DECL, ParserMaddrParam ma) {
/* Line 2660, RV SIP Stack */
 
	     PCB.urlParameter.paramType = PARSER_MADDR_PARAM;
		 PCB.urlParameter.param.maddrParam = ma;
	
}
static void ag_rp_181(PCB_DECL, ParserMethod met) {
/* Line 2666, RV SIP Stack */
	     PCB.urlParameter.paramType = PARSER_METHOD_PARAM;
		 PCB.urlParameter.param.methodParam =met;
    
}
static void ag_rp_182(PCB_DECL, ParserLrParamType lrParam) {
/* Line 2671, RV SIP Stack */
	     PCB.urlParameter.paramType = PARSER_LR_PARAM;
		 PCB.urlParameter.param.lrParamType   = lrParam;
    
}
static void ag_rp_183(PCB_DECL, ParserCompParam comp) {
/* Line 2676, RV SIP Stack */
        PCB.urlParameter.paramType       = PARSER_COMP_PARAM;
        PCB.urlParameter.param.compParam = comp;
    
}
static void ag_rp_184(PCB_DECL, ParserTokenizedByParam tokenizedBy) {
/* Line 2681, RV SIP Stack */
        PCB.urlParameter.paramType       = PARSER_TOKENIZED_BY_PARAM;
        PCB.urlParameter.param.tokenizedByParam = tokenizedBy;
    
}
static void ag_rp_185(PCB_DECL, RvBool orig) {
/* Line 2686, RV SIP Stack */
        PCB.urlParameter.paramType       = PARSER_ORIG_PARAM;
        PCB.urlParameter.param.bOrigParam = orig;
    
}
static void ag_rp_186(PCB_DECL) {
/* Line 2696, RV SIP Stack */
	     PCB.urlParameter.paramType = PARSER_OTHER_PARAM;
    
}
#define ag_rp_187(PCB_POINTER, t) (t)
static ParserUserParam ag_rp_188(PCB_DECL, RvSipUserParam pt) {
/* Line 2711, RV SIP Stack */
 
        ParserUserParam userParam;
	    userParam.paramType = pt;
		userParam.otherUser.buf = NULL;
		userParam.otherUser.len = 0;
        RV_UNUSED_ARG(pcb_pointer);
	    return userParam;
    
}
static ParserUserParam ag_rp_189(PCB_DECL, ParserOtherUser ou) {
/* Line 2720, RV SIP Stack */
 
        ParserUserParam userParam;
        userParam.paramType = RVSIP_USERPARAM_OTHER;
	    userParam.otherUser = ou;
        RV_UNUSED_ARG(pcb_pointer);
	    return userParam;
    
}
#define ag_rp_190(PCB_POINTER) (RVSIP_USERPARAM_PHONE)
#define ag_rp_191(PCB_POINTER) (RVSIP_USERPARAM_IP)
#define ag_rp_192(PCB_POINTER, t) (t)
#define ag_rp_193(PCB_POINTER, ttl) (ttl)
#define ag_rp_194(PCB_POINTER, host) (host)
#define ag_rp_195(PCB_POINTER, met) (met)
#define ag_rp_196(PCB_POINTER) (ParserLrParam1)
#define ag_rp_197(PCB_POINTER) (ParserLrParamTrue)
#define ag_rp_198(PCB_POINTER) (ParserLrParamOn)
#define ag_rp_199(PCB_POINTER) (ParserLrParamEmpty)
#define ag_rp_200(PCB_POINTER, comp) (comp)
#define ag_rp_201(PCB_POINTER, hostname) (hostname)
#define ag_rp_202(PCB_POINTER) (RV_TRUE)
static void ag_rp_203(PCB_DECL, ParserBasicToken token) {
/* Line 2792, RV SIP Stack */
          if (0!=((ParserExtensionString *)PCB.pUrlExtParams)->size)
          {
              
              APPEND_DATA(";", 1,PCB.pUrlExtParams);
              APPEND_DATA(token.buf,token.len, PCB.pUrlExtParams);
          }
          else
          {      
              
              APPEND_DATA(token.buf,token.len,PCB.pUrlExtParams);
          }
    
}
static void ag_rp_204(PCB_DECL, ParserBasicToken token1, ParserBasicToken token2) {
/* Line 2806, RV SIP Stack */
          if (0!=((ParserExtensionString *)PCB.pUrlExtParams)->size)
          {
              
              APPEND_DATA(";", 1,PCB.pUrlExtParams);
              APPEND_DATA(token1.buf, token1.len,PCB.pUrlExtParams);
              APPEND_DATA("=" , 1,PCB.pUrlExtParams);
              APPEND_DATA(token2.buf, token2.len,PCB.pUrlExtParams);            
          }
          else
          {      
              
              APPEND_DATA(token1.buf, token1.len,PCB.pUrlExtParams);
              APPEND_DATA("=" , 1,PCB.pUrlExtParams);
              APPEND_DATA(token2.buf, token2.len,PCB.pUrlExtParams);            
          }
    
}
#define ag_rp_205(PCB_POINTER) (CUR_STRING())
#define ag_rp_206(PCB_POINTER, token) (token)
static ParserOptionalHeaders ag_rp_207(PCB_DECL) {
/* Line 2849, RV SIP Stack */
 
        ParserOptionalHeaders optionalHeaders;
        optionalHeaders.isSpecified = RV_FALSE;
		optionalHeaders.body.buf = NULL;
		optionalHeaders.body.len = 0;
        RV_UNUSED_ARG(pcb_pointer);
        return optionalHeaders;
    
}
static ParserOptionalHeaders ag_rp_208(PCB_DECL, ParserBasicToken headersList) {
/* Line 2858, RV SIP Stack */
 
        ParserOptionalHeaders optionalHeaders;
        optionalHeaders.isSpecified = RV_TRUE;
        optionalHeaders.body=headersList;
        RV_UNUSED_ARG(pcb_pointer);
        return optionalHeaders;
    
}
#define ag_rp_209(PCB_POINTER) (CUR_STRING())
#define ag_rp_210(PCB_POINTER) (CUR_STRING())
#define ag_rp_211(PCB_POINTER) (CUR_STRING())
#define ag_rp_212(PCB_POINTER) (CUR_STRING())
#define ag_rp_213(PCB_POINTER) (CUR_STRING())
static void ag_rp_214(PCB_DECL) {
/* Line 2894, RV SIP Stack */
        RvStatus status;
        status = ParserInitRequsetLine(PCB.pParserMgr,&PCB,PCB.pointer,&(PCB.requestLine), PCB.pSipObject);
        if(RV_ERROR_ILLEGAL_SYNTAX == status)
        {
            PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
            SYNTAX_ERROR;
        }
        else if (RV_OK != status)
        {
            
            PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
            PCB.eStat = status;
        }
        ParserCleanExtParams (PCB.pExtParams);
        ParserCleanExtParams (PCB.pUrlExtParams);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_REQUEST_LINE, &PCB);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_EXURI, &PCB);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_SIP_URL, &PCB);
        
    
}
static void ag_rp_215(PCB_DECL, ParserMethod met, ParserSipVersion ver) {
/* Line 2919, RV SIP Stack */
         PCB.requestLine.method = met;
         PCB.requestLine.sipVer = ver;
		 PCB.requestLine.exUri = PCB.exUri;         
    
}
#define ag_rp_216(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("m"))
#define ag_rp_217(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("i"))
#define ag_rp_218(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("c"))
#define ag_rp_219(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("v"))
#define ag_rp_220(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("t"))
#define ag_rp_221(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("f"))
#define ag_rp_222(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("r"))
#define ag_rp_223(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("b"))
#define ag_rp_224(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("k"))
#define ag_rp_225(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("x"))
#define ag_rp_226(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("o"))
#define ag_rp_227(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("u"))
#define ag_rp_228(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("l"))
#define ag_rp_229(PCB_POINTER, met) (met)
#define ag_rp_230(PCB_POINTER) (SET_URI_TYPE(PARSER_ADDR_PARAM_TYPE_SIP_URL, PCB.exUri.ExUriInfo.absUri))
#define ag_rp_231(PCB_POINTER, uri) (SET_URI_TYPE(PARSER_ADDR_PARAM_TYPE_PRES_URI, uri))
#define ag_rp_232(PCB_POINTER, uri) (SET_URI_TYPE(PARSER_ADDR_PARAM_TYPE_IM_URI, uri))
#define ag_rp_233(PCB_POINTER) (SET_URI_TYPE(PARSER_ADDR_PARAM_TYPE_TEL_URI, PCB.exUri.ExUriInfo.absUri))
#define ag_rp_234(PCB_POINTER, uri) (SET_URI_TYPE(PARSER_ADDR_PARAM_TYPE_ABS_URI, uri))
static void ag_rp_235(PCB_DECL) {
/* Line 2974, RV SIP Stack */
 
         RvStatus status;
         status = ParserInitParty(PCB.pParserMgr,PCB.pointer,&(PCB.party), PCB.eHeaderType, PCB.eWhichHeader, PCB.pSipObject);
         if(RV_ERROR_ILLEGAL_SYNTAX == status)
         {
             PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
             SYNTAX_ERROR;
         }
         else if (RV_OK != status)
         {
             
             PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
             PCB.eStat = status;
         }
         
        ParserCleanExtParams (PCB.pExtParams); 
        ParserCleanExtParams (PCB.pUrlExtParams);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_PARTY, &PCB);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_NAME_ADDR, &PCB);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_EXURI, &PCB);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_SIP_URL, &PCB);
	 
}
static void ag_rp_236(PCB_DECL) {
/* Line 3000, RV SIP Stack */
 
	  PCB.party.nameAddr = PCB.nameAddr;
	  PCB.party.partyParams.isTag = RV_FALSE;
	  PCB.party.partyParams.isAddrParams = RV_FALSE;
      PCB.party.isCompact = PCB.isCompactForm;
	
}
static void ag_rp_237(PCB_DECL, ParserPartyParams partyParams) {
/* Line 3009, RV SIP Stack */
 
	  PCB.party.nameAddr = PCB.nameAddr;
	  PCB.party.partyParams = partyParams; 
      PCB.party.isCompact = PCB.isCompactForm;
	
}
static void ag_rp_238(PCB_DECL) {
/* Line 3018, RV SIP Stack */
 
	  PCB.party.nameAddr.isDisplayName = RV_FALSE;
      PCB.party.partyParams.isTag = RV_FALSE;
	  PCB.party.partyParams.isAddrParams = RV_FALSE;
	  PCB.party.nameAddr.exUri = PCB.exUri;
      PCB.party.isCompact = PCB.isCompactForm;
	
}
static void ag_rp_239(PCB_DECL, ParserPartyParams partyParams) {
/* Line 3028, RV SIP Stack */
 
        PCB.party.nameAddr.isDisplayName = RV_FALSE;
        PCB.party.partyParams = partyParams;
		PCB.party.nameAddr.exUri = PCB.exUri;       
        PCB.party.isCompact = PCB.isCompactForm;
      
	
}
static void ag_rp_240(PCB_DECL, ParserDisplayName name) {
/* Line 3040, RV SIP Stack */
 
		PCB.nameAddr.exUri = PCB.exUri;    
        PCB.nameAddr.isDisplayName = RV_TRUE;  
	    PCB.nameAddr.name = name;
	
}
static void ag_rp_241(PCB_DECL) {
/* Line 3047, RV SIP Stack */
 
		PCB.nameAddr.exUri = PCB.exUri;
        PCB.nameAddr.isDisplayName = RV_FALSE;  
    
}
#define ag_rp_242(PCB_POINTER, qs) (qs)
#define ag_rp_243(PCB_POINTER, mtot) (mtot)
#define ag_rp_244(PCB_POINTER) (CUR_STRING())
#define ag_rp_245(PCB_POINTER) (CUR_STRING())
#define ag_rp_246(PCB_POINTER, t) (t)
#define ag_rp_247(PCB_POINTER, ppd) (ppd)
static ParserPartyParams ag_rp_248(PCB_DECL, ParserPartyParams ppl, ParserPartyParams ppd) {
/* Line 3078, RV SIP Stack */
        if (RV_TRUE == ppd.isTag)
        {
            ppl.isTag = RV_TRUE;
            ppl.tag = ppd.tag;
        }
        if (RV_TRUE == ppd.isAddrParams) 
        {
            ppl.isAddrParams = RV_TRUE;
            ppl.addrParams = ppd.addrParams;
        }
        RV_UNUSED_ARG(pcb_pointer);
        return ppl;
    
}
static ParserPartyParams ag_rp_249(PCB_DECL, ParserTagParam tag) {
/* Line 3097, RV SIP Stack */
        ParserPartyParams partyParams;
        partyParams.isTag = RV_TRUE;
        partyParams.tag = tag;
        partyParams.isAddrParams = RV_FALSE;
        partyParams.addrParams = NULL;
        RV_UNUSED_ARG(pcb_pointer);
        return partyParams; 
    
}
static ParserPartyParams ag_rp_250(PCB_DECL) {
/* Line 3107, RV SIP Stack */
        ParserPartyParams partyParams;
        partyParams.isAddrParams = RV_TRUE;
        partyParams.isTag = RV_FALSE;
        partyParams.addrParams = PCB.pExtParams;
		partyParams.tag.buf = NULL;
		partyParams.tag.len = 0;
        return partyParams; 
    
}
static void ag_rp_251(PCB_DECL) {
/* Line 3133, RV SIP Stack */
         RvStatus status;
         ParserContactHeaderValues contactHeaderValues;
         contactHeaderValues.isStar = RV_TRUE; 
         contactHeaderValues.isCompact = PCB.isCompactForm;
         status = ParserInitContactHeader(PCB.pParserMgr, &PCB, PCB.pointer,
                                          &contactHeaderValues,
                                          PCB.eHeaderType,
                                          PCB.pSipObject);
         if(RV_ERROR_ILLEGAL_SYNTAX == status)
         {
             PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
             SYNTAX_ERROR;
         }
         else if (RV_OK != status)
         {
             
             PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
             PCB.eStat = status;
         }
    
}
static void ag_rp_252(PCB_DECL) {
/* Line 3166, RV SIP Stack */
         RvStatus status;
         ParserContactHeaderValues  contactHeaderValues;
         contactHeaderValues.isStar        = RV_FALSE;
         contactHeaderValues.header        = PCB.contact;
         contactHeaderValues.isCompact     = PCB.isCompactForm;
         status = ParserInitContactHeader(PCB.pParserMgr,&PCB,PCB.pointer,
                                          &contactHeaderValues,
                                          PCB.eHeaderType,
                                          PCB.pSipObject);
         if(RV_ERROR_ILLEGAL_SYNTAX == status)
         {
             PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
             SYNTAX_ERROR;
         }
         else if (RV_OK != status)
         {
             
             PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
             PCB.eStat = status;
         }
         
         
        ParserCleanExtParams (PCB.pExtParams);
        ParserCleanExtParams (PCB.pUrlExtParams);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_CONTACT, &PCB);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_NAME_ADDR, &PCB);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_EXURI, &PCB);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_SIP_URL, &PCB);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_EXPIRES, &PCB);
    
}
static void ag_rp_253(PCB_DECL) {
/* Line 3199, RV SIP Stack */
         RvStatus                 status;
         ParserContactHeaderValues contactHeaderValues;
         contactHeaderValues.isStar = RV_FALSE;
         contactHeaderValues.header = PCB.contact;
         contactHeaderValues.isCompact = PCB.isCompactForm;
         status = ParserInitContactHeader(PCB.pParserMgr,&PCB, PCB.pointer,
                                          &contactHeaderValues,
                                          PCB.eHeaderType,
                                          PCB.pSipObject);
         if(RV_ERROR_ILLEGAL_SYNTAX == status)
         {
             PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
             SYNTAX_ERROR;
         }
         else if (RV_OK != status)
         {
             
             PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
             PCB.eStat = status;
         }
          
         ParserCleanExtParams (PCB.pExtParams);
         ParserCleanExtParams (PCB.pUrlExtParams);
         ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_CONTACT, &PCB);
         ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_NAME_ADDR, &PCB);
         ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_EXURI, &PCB);
         ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_SIP_URL, &PCB);
         ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_EXPIRES, &PCB);
    
}
static void ag_rp_254(PCB_DECL) {
/* Line 3234, RV SIP Stack */
 
	  PCB.contact.nameAddr = PCB.nameAddr;
      PCB.contact.isParams = RV_FALSE;
	
}
static void ag_rp_255(PCB_DECL) {
/* Line 3240, RV SIP Stack */
 
	  PCB.contact.nameAddr = PCB.nameAddr;
      PCB.contact.isParams = RV_TRUE;
      
	
}
static void ag_rp_256(PCB_DECL) {
/* Line 3250, RV SIP Stack */
 
	  PCB.contact.nameAddr.isDisplayName = RV_FALSE;  
      PCB.contact.isParams = RV_FALSE;
	  PCB.contact.nameAddr.exUri = PCB.exUri;      
	
}
static void ag_rp_257(PCB_DECL) {
/* Line 3257, RV SIP Stack */
 
      PCB.contact.nameAddr.isDisplayName = RV_FALSE;
	  PCB.contact.isParams = RV_TRUE;      
      PCB.contact.nameAddr.exUri = PCB.exUri;
	
}
static void ag_rp_258(PCB_DECL, ParserQVal q) {
/* Line 3277, RV SIP Stack */
 
        PCB.contact.params.isQ = RV_TRUE;
        PCB.contact.params.q = q;
    
}
static void ag_rp_259(PCB_DECL, ParserContactActionType action) {
/* Line 3283, RV SIP Stack */
 
        PCB.contact.params.isAction = RV_TRUE;
        PCB.contact.params.action = action;
    
}
static void ag_rp_260(PCB_DECL) {
/* Line 3289, RV SIP Stack */
 
        PCB.contact.params.isExpires = RV_TRUE;
        PCB.contact.params.expires = PCB.expires;
    
}
static void ag_rp_261(PCB_DECL) {
/* Line 3295, RV SIP Stack */
  
        PCB.contact.params.isExtention = RV_TRUE;
		PCB.contact.params.exten = PCB.pExtParams;
    
}
#define ag_rp_262(PCB_POINTER, qVal) (qVal)
#define ag_rp_263(PCB_POINTER) (CUR_STRING())
#define ag_rp_264(PCB_POINTER) (CUR_STRING())
#define ag_rp_265(PCB_POINTER, actionVal) (actionVal)
#define ag_rp_266(PCB_POINTER) (PARSER_CONTACT_ACTION_PROXY)
#define ag_rp_267(PCB_POINTER) (PARSER_CONTACT_ACTION_REDIRECT)
static void ag_rp_268(PCB_DECL) {
/* Line 3341, RV SIP Stack */
       RvStatus status;
       status = ParserInitExpiresHeader(PCB.pParserMgr,PCB.pointer,&(PCB.expires),PCB.eHeaderType,PCB.pSipObject);
       if(RV_ERROR_ILLEGAL_SYNTAX == status)
       {
           PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
           SYNTAX_ERROR;
       }
       else if (RV_OK != status)
       {
           
           PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
           PCB.eStat = status;
       }
       ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_EXPIRES, &PCB);
    
}
static void ag_rp_269(PCB_DECL, ParserDeltaSeconds delta) {
/* Line 3363, RV SIP Stack */
  
		PCB.expires.eFormat = RVSIP_EXPIRES_FORMAT_DELTA_SECONDS;
		PCB.expires.deltaSeconds = delta;
    
}
static void ag_rp_270(PCB_DECL, ParserSipDate date) {
/* Line 3369, RV SIP Stack */
   
	    PCB.expires.eFormat = RVSIP_EXPIRES_FORMAT_DATE;
	    PCB.expires.sipDate = date;
	
}
static void ag_rp_271(PCB_DECL, ParserDeltaSeconds delta) {
/* Line 3378, RV SIP Stack */
   
		PCB.expires.eFormat = RVSIP_EXPIRES_FORMAT_DELTA_SECONDS;
		PCB.expires.deltaSeconds = delta;
    
}
static void ag_rp_272(PCB_DECL, ParserSipDate date) {
/* Line 3384, RV SIP Stack */
   
	    PCB.expires.eFormat = RVSIP_EXPIRES_FORMAT_DATE;
	    PCB.expires.sipDate = date;
	
}
#define ag_rp_273(PCB_POINTER) (CUR_STRING())
static void ag_rp_274(PCB_DECL, ParserSipDate date) {
/* Line 3409, RV SIP Stack */
   
          RvStatus status;
          status = ParserInitDateHeader(PCB.pParserMgr,PCB.pointer,&date,PCB.eHeaderType,PCB.pSipObject);
          if(RV_ERROR_ILLEGAL_SYNTAX == status)
          {
              PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
              SYNTAX_ERROR;
          }
          else if (RV_OK != status)
          {
                
              PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
              PCB.eStat = status;
          }
    
}
static ParserSipDate ag_rp_275(PCB_DECL, ParserDDMMYY date, ParserTime t) {
/* Line 3431, RV SIP Stack */
  
        ParserSipDate sipDate;
        sipDate.eWeekDay = RVSIP_WEEKDAY_UNDEFINED;
		sipDate.ddmmyy   = date;
        sipDate.time     = t;
        RV_UNUSED_ARG(pcb_pointer);
        return sipDate;
    
}
#define ag_rp_276(PCB_POINTER, date) (date)
static ParserSipDate ag_rp_277(PCB_DECL, RvSipDateWeekDay day, ParserDDMMYY date, ParserTime t) {
/* Line 3446, RV SIP Stack */
   
        ParserSipDate sipDate;
        sipDate.eWeekDay = RVSIP_WEEKDAY_UNDEFINED;
	    sipDate.eWeekDay = day; 
		sipDate.ddmmyy   = date;
        sipDate.time     = t;
        RV_UNUSED_ARG(pcb_pointer);
        return sipDate;
    
}
#define ag_rp_278(PCB_POINTER) (RVSIP_WEEKDAY_SUN)
#define ag_rp_279(PCB_POINTER) (RVSIP_WEEKDAY_MON)
#define ag_rp_280(PCB_POINTER) (RVSIP_WEEKDAY_TUE)
#define ag_rp_281(PCB_POINTER) (RVSIP_WEEKDAY_WED)
#define ag_rp_282(PCB_POINTER) (RVSIP_WEEKDAY_THU)
#define ag_rp_283(PCB_POINTER) (RVSIP_WEEKDAY_FRI)
#define ag_rp_284(PCB_POINTER) (RVSIP_WEEKDAY_SAT)
static ParserDDMMYY ag_rp_285(PCB_DECL, ParserInt32 dd, RvSipDateMonth mm, ParserInt32 yy) {
/* Line 3471, RV SIP Stack */
        ParserDDMMYY ddmmyy;
        ddmmyy.day = dd;
        ddmmyy.eMonth = mm;
        ddmmyy.year = yy;
        RV_UNUSED_ARG(pcb_pointer);
        return ddmmyy;
    
}
static ParserInt32 ag_rp_286(PCB_DECL, int d) {
/* Line 3483, RV SIP Stack */
    	RV_UNUSED_ARG(pcb_pointer);
    	return d - '0';	
    
}
static ParserInt32 ag_rp_287(PCB_DECL, int d1, int d2) {
/* Line 3488, RV SIP Stack */
		RV_UNUSED_ARG(pcb_pointer);
	    return 10 * (d1-'0') + (d2-'0');
    
}
#define ag_rp_288(PCB_POINTER) (RVSIP_MONTH_JAN)
#define ag_rp_289(PCB_POINTER) (RVSIP_MONTH_FEB)
#define ag_rp_290(PCB_POINTER) (RVSIP_MONTH_MAR)
#define ag_rp_291(PCB_POINTER) (RVSIP_MONTH_APR)
#define ag_rp_292(PCB_POINTER) (RVSIP_MONTH_MAY)
#define ag_rp_293(PCB_POINTER) (RVSIP_MONTH_JUN)
#define ag_rp_294(PCB_POINTER) (RVSIP_MONTH_JUL)
#define ag_rp_295(PCB_POINTER) (RVSIP_MONTH_AUG)
#define ag_rp_296(PCB_POINTER) (RVSIP_MONTH_SEP)
#define ag_rp_297(PCB_POINTER) (RVSIP_MONTH_OCT)
#define ag_rp_298(PCB_POINTER) (RVSIP_MONTH_NOV)
#define ag_rp_299(PCB_POINTER) (RVSIP_MONTH_DEC)
#define ag_rp_300(PCB_POINTER, d1, d2) (2000 + 10 * (d1-'0') + (d2-'0'))
#define ag_rp_301(PCB_POINTER, d1, d2, d3) (2000 + 100 * (d1-'0') + 10 * (d2-'0') + (d3-'0'))
#define ag_rp_302(PCB_POINTER, d1, d2, d3, d4) (1000 * (d1-'0') + 100*(d2-'0') + 10*(d3-'0') + (d4-'0'))
static ParserTime ag_rp_303(PCB_DECL, ParserInt32 hour, ParserInt32 min, ParserInt32 second, ParserTimeZone zone) {
/* Line 3518, RV SIP Stack */
        ParserTime t;
        t.hour   = hour;
        t.minute = min;
        t.second = second;
        t.zone   = zone;
        RV_UNUSED_ARG(pcb_pointer);
        return t;        
    
}
#define ag_rp_304(PCB_POINTER, d1, d2) (10*(d1-'0') + (d2-'0'))
#define ag_rp_305(PCB_POINTER, d1, d2) (10*(d1-'0') + (d2-'0'))
#define ag_rp_306(PCB_POINTER, d1, d2) (10*(d1-'0') + (d2-'0'))
#define ag_rp_307(PCB_POINTER) (SIPPARSER_TIME_ZONE_GMT)
#define ag_rp_308(PCB_POINTER) (SIPPARSER_TIME_ZONE_UTC)
#define ag_rp_309(PCB_POINTER) (SIPPARSER_TIME_ZONE_GMST)
static void ag_rp_310(PCB_DECL, ParserMethod method) {
/* Line 3565, RV SIP Stack */
        RvStatus         status;
        ParserOtherHeader otherHeader;
        otherHeader.name = method.other;
        otherHeader.value.buf = NULL;
        otherHeader.value.len = 0;
        ParserMethodInOtherHeader(&method, &otherHeader);
        status = ParserInitOtherHeader(PCB.pParserMgr,&PCB,PCB.pointer,
                                       &otherHeader,
                                       PCB.eHeaderType,
                                       PCB.pSipObject);
        if(RV_ERROR_ILLEGAL_SYNTAX == status)
        {
             PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
             SYNTAX_ERROR;
        }
        else if (RV_OK != status)
        {
             
             PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
             PCB.eStat = status;
        }
        
    
}
static void ag_rp_311(PCB_DECL, ParserMethod method, ParserBasicToken fv) {
/* Line 3590, RV SIP Stack */
        RvStatus         status;
        ParserOtherHeader otherHeader;
        otherHeader.name = method.other;
        otherHeader.value = fv;
        ParserMethodInOtherHeader(&method, &otherHeader);
        status = ParserInitOtherHeader(PCB.pParserMgr,&PCB,PCB.pointer,
                                       &otherHeader,
                                       PCB.eHeaderType,
                                       PCB.pSipObject);
        if(RV_ERROR_ILLEGAL_SYNTAX == status)
        {
            PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
            SYNTAX_ERROR;
        }
        else if (RV_OK != status)
        {
            
            PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
            PCB.eStat = status;
        }
    
}
static void ag_rp_312(PCB_DECL) {
/* Line 3620, RV SIP Stack */
          RvStatus status; 
          status = ParserInitAuthentication(PCB.pParserMgr,&PCB,PCB.pointer,&(PCB.authentication),PCB.eHeaderType,PCB.eWhichHeader,PCB.pSipObject);
          if(RV_ERROR_ILLEGAL_SYNTAX == status)
          {
              PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
              SYNTAX_ERROR;
          }
          else if (RV_OK != status)
          {
               
               PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
               PCB.eStat = status;
          }
           
          ParserCleanExtParams (PCB.pExtParams);
          ParserCleanExtParams (PCB.pUrlExtParams);
          ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_AUTHENTICATION, &PCB);
          ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_SIP_URL, &PCB);
          ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_EXURI, &PCB);
          ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_DIGEST_CHALLENGE, &PCB);
    
}
static void ag_rp_313(PCB_DECL) {
/* Line 3650, RV SIP Stack */
    
        PCB.authentication.eAuthScheme = RVSIP_AUTH_SCHEME_DIGEST;
        PCB.authentication.isAuthParamList = RV_FALSE;
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_DIGEST_CHALLENGE, &PCB);        
    
}
static void ag_rp_314(PCB_DECL, ParserBasicToken authScheme) {
/* Line 3657, RV SIP Stack */
        
        PCB.authentication.eAuthScheme = RVSIP_AUTH_SCHEME_OTHER;
        PCB.authentication.authScheme  = authScheme;
        PCB.authentication.isRealm =     RV_FALSE;
        PCB.authentication.isDomain =    RV_FALSE;
        PCB.authentication.isNonce =     RV_FALSE;
        PCB.authentication.isOpaque =    RV_FALSE;
        PCB.authentication.isAuthParam = RV_FALSE;
        PCB.authentication.eStale =      RVSIP_AUTH_STALE_UNDEFINED;
        PCB.authentication.algorithm.eAlgorithm = RVSIP_AUTH_ALGORITHM_UNDEFINED;
		PCB.authentication.algorithm.isAKAVersion = RV_FALSE;
		PCB.authentication.qopValue.eAuthQop    = RVSIP_AUTH_QOP_UNDEFINED;
        PCB.authentication.qopValue.isOther = RV_FALSE;
        PCB.authentication.isAuthParamList = RV_TRUE;
		PCB.authentication.isIntegrityKey =	RV_FALSE;
		PCB.authentication.isCipherKey =	RV_FALSE;
        PCB.authentication.authParamList = PCB.pExtParams;
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_DIGEST_CHALLENGE, &PCB);        
    
}
static void ag_rp_315(PCB_DECL) {
/* Line 3685, RV SIP Stack */
        PCB.authentication.eAuthScheme = RVSIP_AUTH_SCHEME_UNDEFINED;
        PCB.authentication.isRealm =     RV_FALSE;
        PCB.authentication.isDomain =    RV_FALSE;
        PCB.authentication.isNonce =     RV_FALSE;
        PCB.authentication.isOpaque =    RV_FALSE;
        PCB.authentication.isAuthParam = RV_FALSE;
        PCB.authentication.eStale =      RVSIP_AUTH_STALE_UNDEFINED;
        PCB.authentication.algorithm.eAlgorithm = RVSIP_AUTH_ALGORITHM_UNDEFINED;
		PCB.authentication.algorithm.isAKAVersion = RV_FALSE;
        PCB.authentication.qopValue.eAuthQop    = RVSIP_AUTH_QOP_UNDEFINED;
        PCB.authentication.qopValue.isOther = RV_FALSE;
		PCB.authentication.isIntegrityKey =	RV_FALSE;
		PCB.authentication.isCipherKey =	RV_FALSE;
        switch (PCB.digestChallenge.eType)
        {
            case PARSER_DIGEST_TYPE_REALM:
            {
                PCB.authentication.isRealm = RV_TRUE;
                PCB.authentication.realm = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_DOMAIN:
            {
                PCB.authentication.isDomain = RV_TRUE;
                PCB.authentication.domain = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_NONCE:
            {
                PCB.authentication.isNonce = RV_TRUE;
                PCB.authentication.nonce = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_OPAQUE:
            {
                PCB.authentication.isOpaque = RV_TRUE;
                PCB.authentication.opaque = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_STALE:
            {
                PCB.authentication.eStale = PCB.digestChallenge.eStale;
                break;
            }
            case PARSER_DIGEST_TYPE_ALGORITHM:
            {
                PCB.authentication.algorithm = PCB.digestChallenge.algorithm;
                break;
            }
			case PARSER_DIGEST_TYPE_INTEGRITY_KEY:
            {
                PCB.authentication.integrityKey = PCB.digestChallenge.challenge;
                break;
            }
			case PARSER_DIGEST_TYPE_CIPHER_KEY:
            {
                PCB.authentication.cipherKey = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_OPQ_OPTIONS:
            {
                PCB.authentication.qopValue = PCB.digestChallenge.qopVal;
                break;
            }
            case PARSER_DIGEST_TYPE_AUTH_PARAM:
            {
                PCB.authentication.isAuthParam = RV_TRUE;
                PCB.authentication.authParam = PCB.digestChallenge.challenge;
                break;
            }
            default:
                    PCB.exit_flag = AG_SEMANTIC_ERROR_CODE;
                    PCB.eStat = RV_ERROR_ILLEGAL_SYNTAX;
        }
    
}
static void ag_rp_316(PCB_DECL) {
/* Line 3763, RV SIP Stack */
        if (PCB.isNewChallenge == RV_FALSE)
        {
            switch (PCB.digestChallenge.eType)
            {
                case PARSER_DIGEST_TYPE_REALM:
                {
                    PCB.authentication.isRealm = RV_TRUE;
                    PCB.authentication.realm = PCB.digestChallenge.challenge;
                    break;
                }
                case PARSER_DIGEST_TYPE_DOMAIN:
                {
                    PCB.authentication.isDomain = RV_TRUE;
                    PCB.authentication.domain = PCB.digestChallenge.challenge;
                    break;
                }
                case PARSER_DIGEST_TYPE_NONCE:
                {
                    PCB.authentication.isNonce = RV_TRUE;
                    PCB.authentication.nonce = PCB.digestChallenge.challenge;
                    break;
                }
                case PARSER_DIGEST_TYPE_OPAQUE:
                {
                    PCB.authentication.isOpaque = RV_TRUE;
                    PCB.authentication.opaque = PCB.digestChallenge.challenge;
                    break;
                }
                case PARSER_DIGEST_TYPE_STALE:
                {
                    PCB.authentication.eStale = PCB.digestChallenge.eStale;
                    break;
                }
                case PARSER_DIGEST_TYPE_ALGORITHM:
                {
                    PCB.authentication.algorithm = PCB.digestChallenge.algorithm;
                    break;
                }
				case PARSER_DIGEST_TYPE_INTEGRITY_KEY:
				{
					PCB.authentication.isIntegrityKey = RV_TRUE;
					PCB.authentication.integrityKey = PCB.digestChallenge.challenge;
					break;
				}
				case PARSER_DIGEST_TYPE_CIPHER_KEY:
				{
					PCB.authentication.isCipherKey = RV_TRUE;
					PCB.authentication.cipherKey = PCB.digestChallenge.challenge;
					break;
				}
                case PARSER_DIGEST_TYPE_OPQ_OPTIONS:
                {
                    PCB.authentication.qopValue = PCB.digestChallenge.qopVal;
                    break;
                }
                case PARSER_DIGEST_TYPE_AUTH_PARAM:
                {
                    PCB.authentication.isAuthParam = RV_TRUE;
                    PCB.authentication.authParam = PCB.digestChallenge.challenge;
                    break;
                }
                default:
                        PCB.exit_flag = AG_SEMANTIC_ERROR_CODE;
                            
                        PCB.eStat = RV_ERROR_ILLEGAL_SYNTAX;
            }
        }
    
}
#define ag_rp_317(PCB_POINTER, realm) (CUR_AUTH_CHALLENGE(PARSER_DIGEST_TYPE_REALM, realm))
#define ag_rp_318(PCB_POINTER, domain) (CUR_AUTH_CHALLENGE(PARSER_DIGEST_TYPE_DOMAIN, domain))
#define ag_rp_319(PCB_POINTER, nonce) (CUR_AUTH_CHALLENGE(PARSER_DIGEST_TYPE_NONCE, nonce))
#define ag_rp_320(PCB_POINTER, opaque) (CUR_AUTH_CHALLENGE(PARSER_DIGEST_TYPE_OPAQUE, opaque))
#define ag_rp_321(PCB_POINTER, ik) (CUR_AUTH_CHALLENGE(PARSER_DIGEST_TYPE_INTEGRITY_KEY, ik))
#define ag_rp_322(PCB_POINTER, ck) (CUR_AUTH_CHALLENGE(PARSER_DIGEST_TYPE_CIPHER_KEY, ck))
#define ag_rp_323(PCB_POINTER, authParam) (CUR_AUTH_CHALLENGE(PARSER_DIGEST_TYPE_AUTH_PARAM, authParam))
static void ag_rp_324(PCB_DECL, RvSipAuthStale stale) {
/* Line 3844, RV SIP Stack */
        PCB.digestChallenge.eType = PARSER_DIGEST_TYPE_STALE;
        PCB.digestChallenge.eStale = stale;
    
}
static void ag_rp_325(PCB_DECL, ParserAlgorithm algorithm) {
/* Line 3849, RV SIP Stack */
        PCB.digestChallenge.eType = PARSER_DIGEST_TYPE_ALGORITHM;
        
        if(PCB.digestChallenge.algorithm.eAlgorithm == RVSIP_AUTH_ALGORITHM_UNDEFINED)
        {
            PCB.digestChallenge.algorithm = algorithm;
        }
    
}
static void ag_rp_326(PCB_DECL, ParserQopValue qopOptions) {
/* Line 3858, RV SIP Stack */
        PCB.digestChallenge.eType = PARSER_DIGEST_TYPE_OPQ_OPTIONS;
        PCB.digestChallenge.qopVal = qopOptions;
    
}
static void ag_rp_327(PCB_DECL) {
/* Line 3864, RV SIP Stack */
        PCB.digestChallenge.eType = PARSER_DIGEST_TYPE_NEW_CHALLENGE;
        PCB.digestChallenge.challenge.buf = "NewChallenge";
        PCB.isNewChallenge = RV_TRUE;
    
}
#define ag_rp_328(PCB_POINTER, t) (t)
#define ag_rp_329(PCB_POINTER, t) (t)
#define ag_rp_330(PCB_POINTER) (CUR_LINE())
#define ag_rp_331(PCB_POINTER) (CUR_LINE())
static void ag_rp_332(PCB_DECL, ParserBasicToken token) {
/* Line 3900, RV SIP Stack */
        RV_UNUSED_ARG(pcb_pointer);
        RV_UNUSED_ARG(token.buf);
    
}
#define ag_rp_333(PCB_POINTER) (SIP_BEGIN_LINE())
#define ag_rp_334(PCB_POINTER) (SIP_BEGIN_LINE())
#define ag_rp_335(PCB_POINTER, t) (t)
#define ag_rp_336(PCB_POINTER, t) (t)
#define ag_rp_337(PCB_POINTER, t) (t)
#define ag_rp_338(PCB_POINTER, t) (t)
#define ag_rp_339(PCB_POINTER) (RVSIP_AUTH_STALE_TRUE)
#define ag_rp_340(PCB_POINTER) (RVSIP_AUTH_STALE_FALSE)
#define ag_rp_341(PCB_POINTER, algo) (algo)
static void ag_rp_342(PCB_DECL) {
/* Line 3962, RV SIP Stack */
		DEFINE_AKAV_PREFIX();
		SIP_BEGIN_TOKEN();			
	
}
static void ag_rp_343(PCB_DECL) {
/* Line 3967, RV SIP Stack */
		DEFINE_AKAV_PREFIX();
		SIP_BEGIN_TOKEN();			
	
}
static void ag_rp_344(PCB_DECL) {
/* Line 3972, RV SIP Stack */
		DEFINE_AKAV_PREFIX();
		SIP_BEGIN_TOKEN();			
	
}
static void ag_rp_345(PCB_DECL) {
/* Line 3977, RV SIP Stack */
		DEFINE_AKAV_PREFIX();
		SIP_BEGIN_TOKEN();			
	
}
static ParserAlgorithm ag_rp_346(PCB_DECL, ParserBasicToken version, ParserAlgorithm algo) {
/* Line 3985, RV SIP Stack */
		algo.isAKAVersion = RV_TRUE;
		algo.nAKAVersion  = version;
		RV_UNUSED_ARG(pcb_pointer);
		return algo;
	
}
static ParserAlgorithm ag_rp_347(PCB_DECL, ParserAlgorithm algo) {
/* Line 3992, RV SIP Stack */
		algo.isAKAVersion = RV_FALSE;
		RV_UNUSED_ARG(pcb_pointer);
		return algo;
	
}
#define ag_rp_348(PCB_POINTER) (SIP_BEGIN_TOKEN())
static ParserAlgorithm ag_rp_349(PCB_DECL) {
/* Line 4004, RV SIP Stack */
        ParserAlgorithm algo;
        algo.eAlgorithm = RVSIP_AUTH_ALGORITHM_MD5;
		algo.algorithm.buf = NULL;
		algo.algorithm.len = 0;
		algo.nAKAVersion.buf = NULL;
		algo.nAKAVersion.len = 0;
		algo.isAKAVersion = RV_FALSE;
        RV_UNUSED_ARG(pcb_pointer);
        return algo;
    
}
static ParserAlgorithm ag_rp_350(PCB_DECL, ParserBasicToken t) {
/* Line 4016, RV SIP Stack */
        ParserAlgorithm algo;
        algo.eAlgorithm = RVSIP_AUTH_ALGORITHM_OTHER;
        algo.algorithm = t;
		algo.nAKAVersion.buf = NULL;
		algo.nAKAVersion.len = 0;
		algo.isAKAVersion = RV_FALSE;
        RV_UNUSED_ARG(pcb_pointer);
        return algo;
    
}
#define ag_rp_351(PCB_POINTER) (SET_PARSER_PCB_BACK_TO_BUFF(RV_SIPPARSER_PREFIX_AKAV))
#define ag_rp_352(PCB_POINTER) (SET_PARSER_PCB_BACK_TO_BUFF(RV_SIPPARSER_PREFIX_ALGORITHM))
#define ag_rp_353(PCB_POINTER) (CUR_STRING())
#define ag_rp_354(PCB_POINTER, qvList) (qvList)
static ParserQopValue ag_rp_355(PCB_DECL, ParserAuthQopOptions val) {
/* Line 4056, RV SIP Stack */
        ParserQopValue qop;
        qop.eAuthQop = RVSIP_AUTH_QOP_OTHER;
        qop.isOther  = RV_FALSE;
		qop.qop = NULL;
		
        if (RV_TRUE == val.isAuth)
        {
            qop.eAuthQop = RVSIP_AUTH_QOP_AUTH_ONLY;    
        }
        else if (RV_TRUE == val.isAuthInt)
        {
            qop.eAuthQop = RVSIP_AUTH_QOP_AUTHINT_ONLY;
        }
        else if (RV_TRUE == val.isOther)
        {
            if (0!=((ParserExtensionString *)PCB.pExtParams)->size)
            {
                APPEND_DATA(",", 1,PCB.pExtParams);
                
                APPEND_DATA(val.strOther.buf, val.strOther.len,PCB.pExtParams);
                qop.isOther = RV_TRUE;
                qop.qop =  PCB.pExtParams;
            }
            else
            {
                
                APPEND_DATA(val.strOther.buf, val.strOther.len,PCB.pExtParams);
                qop.isOther = RV_TRUE;
                qop.qop = PCB.pExtParams;
            }        
        }      
        return qop;        
    
}
static ParserQopValue ag_rp_356(PCB_DECL, ParserQopValue qop, ParserAuthQopOptions val) {
/* Line 4092, RV SIP Stack */
        if (RV_TRUE == val.isAuth && 
            RVSIP_AUTH_QOP_AUTHINT_ONLY == qop.eAuthQop)
        {
            qop.eAuthQop = RVSIP_AUTH_QOP_AUTH_AND_AUTHINT;    
        }
        else if (RV_TRUE == val.isAuth)
        {
            qop.eAuthQop = RVSIP_AUTH_QOP_AUTH_ONLY;    
        }
        else if (RV_TRUE == val.isAuthInt &&
                 RVSIP_AUTH_QOP_AUTH_ONLY == qop.eAuthQop)
        {
            qop.eAuthQop = RVSIP_AUTH_QOP_AUTH_AND_AUTHINT;
        }
        else if (RV_TRUE == val.isAuthInt)
        {
            qop.eAuthQop = RVSIP_AUTH_QOP_AUTHINT_ONLY;
        }
        else if (RV_TRUE == val.isOther)
        {
            if (0!=((ParserExtensionString *)PCB.pExtParams)->size)
            {
                APPEND_DATA(",", 1,PCB.pExtParams);
                
                APPEND_DATA(val.strOther.buf, val.strOther.len,PCB.pExtParams);
                qop.isOther = RV_TRUE;
                qop.qop =  PCB.pExtParams;
            }
            else
            {
                
                APPEND_DATA(val.strOther.buf, val.strOther.len,PCB.pExtParams);
                qop.isOther = RV_TRUE;
                qop.qop = PCB.pExtParams;
            }        
        }
        return qop;
    
}
static ParserAuthQopOptions ag_rp_357(PCB_DECL) {
/* Line 4137, RV SIP Stack */
        ParserAuthQopOptions qopOption;
        qopOption.isAuthInt = RV_TRUE;
        qopOption.isAuth    = RV_FALSE; 
        qopOption.isOther   = RV_FALSE;
		qopOption.strOther.buf = NULL;
		qopOption.strOther.len = 0;
        RV_UNUSED_ARG(pcb_pointer);
        return qopOption;
    
}
static ParserAuthQopOptions ag_rp_358(PCB_DECL) {
/* Line 4148, RV SIP Stack */
        ParserAuthQopOptions qopOption;
        qopOption.isAuth = RV_TRUE;
        qopOption.isAuthInt = RV_FALSE; 
        qopOption.isOther   = RV_FALSE;
		qopOption.strOther.buf = NULL;
		qopOption.strOther.len = 0;
        RV_UNUSED_ARG(pcb_pointer);
        return qopOption;
    
}
static ParserAuthQopOptions ag_rp_359(PCB_DECL, ParserBasicToken t) {
/* Line 4160, RV SIP Stack */
        ParserAuthQopOptions qopOption;
        qopOption.isOther = RV_TRUE;
        qopOption.isAuthInt = RV_FALSE;
        qopOption.isAuth    = RV_FALSE; 
        qopOption.strOther = t;
        RV_UNUSED_ARG(pcb_pointer);
        return qopOption;
    
}
#define ag_rp_360(PCB_POINTER, ik) (ik)
#define ag_rp_361(PCB_POINTER, ck) (ck)
#define ag_rp_362(PCB_POINTER) (CUR_STRING())
#define ag_rp_363(PCB_POINTER) (CUR_STRING())
#define ag_rp_364(PCB_POINTER) (CUR_STRING())
#define ag_rp_365(PCB_POINTER) (CUR_STRING())
#define ag_rp_366(PCB_POINTER) (CUR_STRING())
static void ag_rp_367(PCB_DECL, ParserBasicToken token1, ParserGenericParamDataVal token2) {
/* Line 4202, RV SIP Stack */
      if (0!=((ParserExtensionString *)PCB.pExtParams)->size)
      {
          
          APPEND_DATA(",", 1,PCB.pExtParams);
          APPEND_DATA(token1.buf, token1.len,PCB.pExtParams);
          APPEND_DATA("=" , 1,PCB.pExtParams);
          APPEND_DATA(token2.buf, token2.len,PCB.pExtParams);            
      }
      else
      {      
          
          APPEND_DATA(token1.buf, token1.len,PCB.pExtParams);
          APPEND_DATA("=" , 1,PCB.pExtParams);
          APPEND_DATA(token2.buf, token2.len,PCB.pExtParams);            
      }
   
}
static void ag_rp_368(PCB_DECL) {
/* Line 4229, RV SIP Stack */
   
          RvStatus status; 
          status = ParserInitAuthorization(PCB.pParserMgr,&PCB,PCB.pointer,&(PCB.authorization),PCB.eHeaderType,PCB.eWhichHeader,PCB.pSipObject);
          if(RV_ERROR_ILLEGAL_SYNTAX == status)
          {
              PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
              SYNTAX_ERROR;
          }
          else if (RV_OK != status)
          {
               
               PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
               PCB.eStat = status;
          }
          ParserCleanExtParams (PCB.pExtParams);
          ParserCleanExtParams (PCB.pUrlExtParams);
          ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_AUTHORIZATION, &PCB);
          ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_DIGEST_CHALLENGE, &PCB);
          ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_EXURI, &PCB);
          ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_SIP_URL, &PCB);
    
}
static void ag_rp_369(PCB_DECL) {
/* Line 4260, RV SIP Stack */
        
        PCB.authorization.eAuthScheme = RVSIP_AUTH_SCHEME_DIGEST;  
        PCB.authorization.isAuthParamList = RV_FALSE;
    
}
static void ag_rp_370(PCB_DECL, ParserBasicToken authScheme) {
/* Line 4266, RV SIP Stack */
        
        PCB.authorization.eAuthScheme			= RVSIP_AUTH_SCHEME_OTHER;
        PCB.authorization.authScheme			= authScheme; 
        PCB.authorization.isUsername			= RV_FALSE;
        PCB.authorization.isRealm				= RV_FALSE;
        PCB.authorization.isNonce				= RV_FALSE;
        PCB.authorization.isResponse			= RV_FALSE;
        PCB.authorization.algorithm.eAlgorithm	= RVSIP_AUTH_ALGORITHM_UNDEFINED;   
        PCB.authorization.isCnonce				= RV_FALSE;
        PCB.authorization.isOpaque				= RV_FALSE;
        PCB.authorization.isNonceCount			= RV_FALSE;
        PCB.authorization.eAuthQop				= RVSIP_AUTH_QOP_UNDEFINED;
        PCB.authorization.isAuthParam			= RV_FALSE;
		PCB.authorization.isAuts				= RV_FALSE;
		PCB.authorization.algorithm.isAKAVersion= RV_FALSE;
		PCB.authorization.eProtected			= RVSIP_AUTH_INTEGRITY_PROTECTED_UNDEFINED;
        PCB.authorization.isAuthParamList = RV_TRUE;
        PCB.authorization.authParamList = PCB.pExtParams;
    
}
static void ag_rp_371(PCB_DECL) {
/* Line 4296, RV SIP Stack */
      
        PCB.authorization.isUsername			= RV_FALSE;
        PCB.authorization.isRealm				= RV_FALSE;
        PCB.authorization.isNonce				= RV_FALSE;
        PCB.authorization.isResponse			= RV_FALSE;
        PCB.authorization.algorithm.eAlgorithm	= RVSIP_AUTH_ALGORITHM_UNDEFINED;   
        PCB.authorization.isCnonce				= RV_FALSE;
        PCB.authorization.isOpaque				= RV_FALSE;
        PCB.authorization.isNonceCount			= RV_FALSE;
        PCB.authorization.eAuthQop				= RVSIP_AUTH_QOP_UNDEFINED;
        PCB.authorization.isAuthParam			= RV_FALSE;
		PCB.authorization.isAuts				= RV_FALSE;
		PCB.authorization.algorithm.isAKAVersion= RV_FALSE;
		PCB.authorization.eProtected			= RVSIP_AUTH_INTEGRITY_PROTECTED_UNDEFINED;
        switch (PCB.digestChallenge.eType)
        {
             case PARSER_DIGEST_TYPE_USERNAME:
            {
                PCB.authorization.isUsername = RV_TRUE;
                PCB.authorization.userName = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_REALM:
            {
                PCB.authorization.isRealm = RV_TRUE;
                PCB.authorization.realm = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_NONCE:
            {
                PCB.authorization.isNonce = RV_TRUE;
                PCB.authorization.nonce = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_DIGEST_URI:
            {
                PCB.authorization.isUri = RV_TRUE;
                PCB.authorization.hDigestUri = PCB.digestChallenge.uri;
                break;
            }
            case PARSER_DIGEST_TYPE_DIGEST_RESPONSE:
            {
                PCB.authorization.isResponse = RV_TRUE;
                PCB.authorization.response = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_ALGORITHM:
            {
                PCB.authorization.algorithm = PCB.digestChallenge.algorithm;
                break;
            }
            case PARSER_DIGEST_TYPE_CNONCE:
            {
                PCB.authorization.isCnonce = RV_TRUE;
                PCB.authorization.cnonce = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_OPAQUE:
            {
                PCB.authorization.isOpaque = RV_TRUE;
                PCB.authorization.opaque = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_NONCE_COUNT:
            {
                PCB.authorization.isNonceCount = RV_TRUE;
                PCB.authorization.nonceCount = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_MESSAGE_QOP:
            {
                PCB.authorization.eAuthQop = PCB.digestChallenge.qopVal.eAuthQop;
                break;
            }
            case PARSER_DIGEST_TYPE_AUTH_PARAM:
            {
                PCB.authorization.isAuthParam = RV_TRUE;
                PCB.authorization.authParam = PCB.digestChallenge.challenge;
                break;
            }
			case PARSER_DIGEST_TYPE_AUTS:
            {
                PCB.authorization.isAuts = RV_TRUE;
                PCB.authorization.auts = PCB.digestChallenge.challenge;
                break;
            }
			case PARSER_DIGEST_TYPE_INTEGRITY_PROTECTED:
            {
                PCB.authorization.eProtected = PCB.digestChallenge.eProtected;
                break;
            }
            default:
				PCB.exit_flag = AG_SEMANTIC_ERROR_CODE;
				PCB.eStat = RV_ERROR_ILLEGAL_SYNTAX;
        }
    
}
static void ag_rp_372(PCB_DECL) {
/* Line 4395, RV SIP Stack */
        switch (PCB.digestChallenge.eType)
        {
             case PARSER_DIGEST_TYPE_USERNAME:
            {
                PCB.authorization.isUsername = RV_TRUE;
                PCB.authorization.userName = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_REALM:
            {
                PCB.authorization.isRealm = RV_TRUE;
                PCB.authorization.realm = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_NONCE:
            {
                PCB.authorization.isNonce = RV_TRUE;
                PCB.authorization.nonce = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_DIGEST_URI:
            {
                PCB.authorization.isUri = RV_TRUE;
                PCB.authorization.hDigestUri = PCB.digestChallenge.uri;
                break;
            }
            case PARSER_DIGEST_TYPE_DIGEST_RESPONSE:
            {
                PCB.authorization.isResponse = RV_TRUE;
                PCB.authorization.response = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_ALGORITHM:
            {
                PCB.authorization.algorithm = PCB.digestChallenge.algorithm;
                break;
            }
            case PARSER_DIGEST_TYPE_CNONCE:
            {
                PCB.authorization.isCnonce = RV_TRUE;
                PCB.authorization.cnonce = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_OPAQUE:
            {
                PCB.authorization.isOpaque = RV_TRUE;
                PCB.authorization.opaque = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_NONCE_COUNT:
            {
                PCB.authorization.isNonceCount = RV_TRUE;
                PCB.authorization.nonceCount = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_MESSAGE_QOP:
            {
                PCB.authorization.eAuthQop = PCB.digestChallenge.qopVal.eAuthQop;
                break;
            }
            case PARSER_DIGEST_TYPE_AUTH_PARAM:
            {
                PCB.authorization.isAuthParam = RV_TRUE;
                PCB.authorization.authParam = PCB.digestChallenge.challenge;
                break;
            }
			case PARSER_DIGEST_TYPE_AUTS:
            {
                PCB.authorization.isAuts = RV_TRUE;
                PCB.authorization.auts = PCB.digestChallenge.challenge;
                break;
            }
			case PARSER_DIGEST_TYPE_INTEGRITY_PROTECTED:
            {
                PCB.authorization.eProtected = PCB.digestChallenge.eProtected;
                break;
            }
            default:
                    PCB.exit_flag = AG_SEMANTIC_ERROR_CODE;
                    PCB.eStat = RV_ERROR_ILLEGAL_SYNTAX;
        }
    
}
#define ag_rp_373(PCB_POINTER, useranme) (CUR_AUTH_CHALLENGE(PARSER_DIGEST_TYPE_USERNAME, useranme))
#define ag_rp_374(PCB_POINTER, realm) (CUR_AUTH_CHALLENGE(PARSER_DIGEST_TYPE_REALM, realm))
#define ag_rp_375(PCB_POINTER, nonce) (CUR_AUTH_CHALLENGE(PARSER_DIGEST_TYPE_NONCE, nonce))
#define ag_rp_376(PCB_POINTER, response) (CUR_AUTH_CHALLENGE(PARSER_DIGEST_TYPE_DIGEST_RESPONSE, response))
#define ag_rp_377(PCB_POINTER, cnonce) (CUR_AUTH_CHALLENGE(PARSER_DIGEST_TYPE_CNONCE, cnonce))
#define ag_rp_378(PCB_POINTER, opaque) (CUR_AUTH_CHALLENGE(PARSER_DIGEST_TYPE_OPAQUE, opaque))
#define ag_rp_379(PCB_POINTER, nonceCount) (CUR_AUTH_CHALLENGE(PARSER_DIGEST_TYPE_NONCE_COUNT, nonceCount))
#define ag_rp_380(PCB_POINTER, authParam) (CUR_AUTH_CHALLENGE(PARSER_DIGEST_TYPE_AUTH_PARAM, authParam))
#define ag_rp_381(PCB_POINTER, auts) (CUR_AUTH_CHALLENGE(PARSER_DIGEST_TYPE_AUTS, auts))
static void ag_rp_382(PCB_DECL, RvSipAuthIntegrityProtected protected) {
/* Line 4490, RV SIP Stack */
        PCB.digestChallenge.eType = PARSER_DIGEST_TYPE_INTEGRITY_PROTECTED;
        PCB.digestChallenge.eProtected = protected;
    
}
static void ag_rp_383(PCB_DECL, RvSipAuthQopOption messageQop) {
/* Line 4495, RV SIP Stack */
        PCB.digestChallenge.eType = PARSER_DIGEST_TYPE_MESSAGE_QOP;
        PCB.digestChallenge.qopVal.eAuthQop = messageQop;
    
}
static void ag_rp_384(PCB_DECL) {
/* Line 4500, RV SIP Stack */
        PCB.digestChallenge.eType = PARSER_DIGEST_TYPE_DIGEST_URI;
        PCB.digestChallenge.uri = PCB.exUri;
    
}
static void ag_rp_385(PCB_DECL, ParserAlgorithm algorithm) {
/* Line 4505, RV SIP Stack */
        PCB.digestChallenge.eType = PARSER_DIGEST_TYPE_ALGORITHM;
        
        if(PCB.digestChallenge.algorithm.eAlgorithm == RVSIP_AUTH_ALGORITHM_UNDEFINED)
        {
            PCB.digestChallenge.algorithm = algorithm;
        }
    
}
#define ag_rp_386(PCB_POINTER, username) (username)
#define ag_rp_387(PCB_POINTER, t) (t)
static RvSipAuthQopOption ag_rp_388(PCB_DECL, ParserAuthQopOptions qopVal) {
/* Line 4544, RV SIP Stack */
        RV_UNUSED_ARG(pcb_pointer);
        if (RV_TRUE == qopVal.isAuth)
        {
            return RVSIP_AUTH_QOP_AUTH_ONLY;
        }
        else if (RV_TRUE == qopVal.isAuthInt)
        {
            return RVSIP_AUTH_QOP_AUTHINT_ONLY;
        }
        else if (RV_TRUE == qopVal.isOther)
        {
            return RVSIP_AUTH_QOP_OTHER;
        }
        else 
        {
             return RVSIP_AUTH_QOP_UNDEFINED;
        }
    
}
#define ag_rp_389(PCB_POINTER, cnonceVal) (cnonceVal)
#define ag_rp_390(PCB_POINTER, nonceVal) (nonceVal)
#define ag_rp_391(PCB_POINTER, ncVal) (ncVal)
#define ag_rp_392(PCB_POINTER) (CUR_STRING())
#define ag_rp_393(PCB_POINTER) (CUR_STRING())
#define ag_rp_394(PCB_POINTER) (CUR_STRING())
#define ag_rp_395(PCB_POINTER) (CUR_STRING())
#define ag_rp_396(PCB_POINTER) (CUR_STRING())
#define ag_rp_397(PCB_POINTER) (CUR_STRING())
#define ag_rp_398(PCB_POINTER) (CUR_STRING())
#define ag_rp_399(PCB_POINTER) (CUR_STRING())
#define ag_rp_400(PCB_POINTER, requestDigest) (requestDigest)
#define ag_rp_401(PCB_POINTER) (CUR_STRING())
#define ag_rp_402(PCB_POINTER) (CUR_STRING())
#define ag_rp_403(PCB_POINTER, autsValue) (autsValue)
#define ag_rp_404(PCB_POINTER) (CUR_STRING())
#define ag_rp_405(PCB_POINTER) (RVSIP_AUTH_INTEGRITY_PROTECTED_NO)
#define ag_rp_406(PCB_POINTER) (RVSIP_AUTH_INTEGRITY_PROTECTED_YES)
static void ag_rp_407(PCB_DECL) {
/* Line 4663, RV SIP Stack */
         RvStatus status;
         status = ParserInitRoute(PCB.pParserMgr,&PCB,PCB.pointer,&(PCB.route), PCB.eHeaderType, PCB.pSipObject);
         if(RV_ERROR_ILLEGAL_SYNTAX == status)
         {
             PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
             SYNTAX_ERROR;
         }
         else if (RV_OK != status)
         {
             
             PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
             PCB.eStat = status;
         }
          
         ParserCleanExtParams (PCB.pExtParams);
         ParserCleanExtParams (PCB.pUrlExtParams);
         ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_ROUTE, &PCB);
         ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_NAME_ADDR, &PCB);
         ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_EXURI, &PCB);
         ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_SIP_URL, &PCB);
    
}
static void ag_rp_408(PCB_DECL) {
/* Line 4686, RV SIP Stack */
 
        RvStatus status;
        status = ParserInitRoute(PCB.pParserMgr,&PCB,PCB.pointer,&(PCB.route), PCB.eHeaderType, PCB.pSipObject);
        if(RV_ERROR_ILLEGAL_SYNTAX == status)
        {
             PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
             SYNTAX_ERROR;
        }
        else if (RV_OK != status)
        {
             
             PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
             PCB.eStat = status;
        }
         
        ParserCleanExtParams (PCB.pExtParams);
        ParserCleanExtParams (PCB.pUrlExtParams);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_ROUTE, &PCB);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_NAME_ADDR, &PCB);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_EXURI, &PCB);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_SIP_URL, &PCB);
    
}
static void ag_rp_409(PCB_DECL) {
/* Line 4713, RV SIP Stack */
        PCB.route.nameAddr = PCB.nameAddr;
        PCB.route.isParams = RV_FALSE;
        PCB.route.eRouteType = PCB.eWhichHeader;
    
}
static void ag_rp_410(PCB_DECL) {
/* Line 4718, RV SIP Stack */
 
        PCB.route.nameAddr = PCB.nameAddr;
        PCB.route.isParams = RV_TRUE;
        PCB.route.eRouteType = PCB.eWhichHeader;
          
         if (((ParserExtensionString *)PCB.pExtParams)->size !=0)
         {
             PCB.route.routeParams = PCB.pExtParams;
         }                
     
}
static void ag_rp_411(PCB_DECL) {
/* Line 5971, RV SIP Stack */
   
		RvStatus status; 
		status = ParserInitAuthenticationInfo(PCB.pParserMgr,PCB.pointer,&(PCB.auth_info),PCB.eHeaderType,PCB.pSipObject);
		if(RV_ERROR_ILLEGAL_SYNTAX == status)
        {
            PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
            SYNTAX_ERROR;
        }
        else if (RV_OK != status)
		{
			
			PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
			PCB.eStat = status;
		}
		ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_AUTHENTICATION_INFO,&PCB);
    
}
static void ag_rp_412(PCB_DECL, ParserBasicToken nc) {
/* Line 5995, RV SIP Stack */
		PCB.auth_info.isNonceCount  = RV_TRUE;
		PCB.auth_info.strNonceCount = nc;
	
}
static void ag_rp_413(PCB_DECL, ParserBasicToken nv) {
/* Line 6000, RV SIP Stack */
		PCB.auth_info.isNextNonce   = RV_TRUE;
		PCB.auth_info.strNextNonce  = nv;
	
}
static void ag_rp_414(PCB_DECL, ParserBasicToken cn) {
/* Line 6005, RV SIP Stack */
		PCB.auth_info.isCNonce      = RV_TRUE;
		PCB.auth_info.strCNonce     = cn;
	
}
static void ag_rp_415(PCB_DECL, ParserBasicToken rd) {
/* Line 6010, RV SIP Stack */
		PCB.auth_info.isRspAuth     = RV_TRUE;
		PCB.auth_info.strRspAuth    = rd;
	
}
static void ag_rp_416(PCB_DECL) {
/* Line 6015, RV SIP Stack */
		PCB.auth_info.isMsgQop      = RV_TRUE;
	
}
static void ag_rp_417(PCB_DECL, ParserAuthQopOptions qopVal) {
/* Line 6023, RV SIP Stack */
		if (RV_TRUE == qopVal.isAuth)
        {
            PCB.auth_info.eMsgQop = RVSIP_AUTH_QOP_AUTH_ONLY;
        }
        else if (RV_TRUE == qopVal.isAuthInt)
        {
            PCB.auth_info.eMsgQop = RVSIP_AUTH_QOP_AUTHINT_ONLY;
        }
        else if (RV_TRUE == qopVal.isOther)
        {
            PCB.auth_info.eMsgQop = RVSIP_AUTH_QOP_OTHER;
        }
        else 
        {
	    PCB.auth_info.eMsgQop = RVSIP_AUTH_QOP_UNDEFINED;
        }
		
		PCB.auth_info.strMsgQop = qopVal.strOther;
	
}
#define ag_rp_418(PCB_POINTER, nv) (nv)
#define ag_rp_419(PCB_POINTER, val) (val)
#define ag_rp_420(PCB_POINTER) (CUR_STRING())
#define READ_COUNTS 
#define WRITE_COUNTS 
#undef V
#define V(i,t) (*(t *) (&(PCB).vs[(PCB).ssx + i]))
#undef VS
#define VS(i) (PCB).vs[(PCB).ssx + i]
#ifndef GET_CONTEXT
#define GET_CONTEXT CONTEXT = (PCB).input_context
#endif
typedef enum {
  ag_action_1,
  ag_action_2,
  ag_action_3,
  ag_action_4,
  ag_action_5,
  ag_action_6,
  ag_action_7,
  ag_action_8,
  ag_action_9,
  ag_action_10,
  ag_action_11,
  ag_action_12
} ag_parser_action;
#ifndef NULL_VALUE_INITIALIZER
#define NULL_VALUE_INITIALIZER = { 0 }
#endif
static SipParser_vs_type const ag_null_value NULL_VALUE_INITIALIZER;
static const unsigned short ag_rpx[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  1,  2,  3,  4,  0,  0,  5,  6,  7,  8,  9, 10, 11,
   12, 13, 14,  0,  0, 15,  0,  0, 16,  0,  0, 17,  0,  0,  0,  0,  0, 18,
    0,  0,  0,  0,  0, 19,  0,  0,  0,  0, 20, 21, 22, 23, 24, 25, 26, 27,
   28,  0,  0, 29, 30, 31, 32, 33, 34, 35, 36, 37,  0, 38,  0, 39, 40, 41,
   42, 43,  0,  0, 44, 45, 46, 47,  0,  0, 48, 49, 50,  0,  0,  0,  0,  0,
    0, 51, 52,  0, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66,
   67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,  0, 80, 81, 82, 83,
    0,  0, 84, 85, 86, 87, 88, 89,  0, 90, 91, 92, 93, 94, 95, 96, 97, 98,
   99,  0,100,101,  0,  0,102,103,  0,  0,104,  0,  0,105,106,  0,107,108,
  109,110,111,  0,  0,  0,  0,  0,  0,  0,  0,  0,112,  0,  0,  0,  0,  0,
    0,  0,  0,  0,113,  0,114,115,116,117,118,119,120,121,122,  0,  0,123,
  124,  0,  0,125,  0,  0,  0,126,  0,  0,127,  0,  0,  0,  0,128,129,  0,
    0,  0,  0,  0,  0,  0,130,  0,  0,  0,  0,  0,131,132,  0,  0,  0,133,
  134,135,136,  0,137,138,139,140,141,142,143,144,145,146,147,  0,  0,  0,
    0,  0,148,149,150,151,152,  0,153,154,  0,155,156,157,158,159,160,161,
    0,  0,162,163,  0,  0,  0,  0,  0,  0,164,165,166,167,168,169,170,  0,
    0,171,172,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,173,174,175,176,177,178,179,180,181,182,183,184,
  185,186,187,188,189,190,191,192,193,194,195,196,197,198,  0,  0,  0,  0,
  199,200,201,202,203,204,  0,  0,  0,  0,  0,205,206,207,208,209,210,211,
    0,  0,212,  0,  0,  0,  0,  0,213,214,215,216,217,218,219,220,221,222,
  223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,
  241,242,243,  0,  0,  0,  0,  0,244,245,246,247,248,249,250,  0,251,  0,
    0,252,253,254,255,256,257,  0,  0,258,259,260,261,262,  0,  0,263,264,
    0,  0,  0,  0,265,266,267,268,  0,269,270,271,272,273,274,275,276,277,
  278,279,280,281,282,283,284,285,286,287,288,289,290,291,292,293,294,295,
  296,297,298,299,300,301,302,303,304,305,306,307,308,309,310,311,312,  0,
  313,314,315,316,317,318,319,320,321,322,323,324,325,326,327,328,329,  0,
    0,330,331,  0,  0,332,333,334,  0,  0,  0,335,336,337,338,339,340,  0,
    0,341,342,343,344,345,346,347,348,349,350,351,352,353,354,355,356,357,
  358,359,360,361,362,363,364,365,366,  0,  0,  0,  0,367,368,  0,369,370,
  371,372,373,374,375,376,377,378,379,380,381,382,383,384,385,386,387,  0,
    0,  0,  0,388,389,390,391,392,393,394,395,396,397,398,399,400,401,402,
    0,  0,  0,403,  0,  0,  0,  0,404,  0,  0,405,406,  0,407,408,409,410,
  411,  0,  0,412,413,414,415,416,417,418,419,  0,  0,  0,  0,  0,420
};
static const unsigned char ag_key_itt[] = {
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0,
 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1,
 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0
};
static const unsigned short ag_key_pt[] = {
  0,  2,  0,  6,  0,  8,  0, 10,  0, 12,  0, 14,  0, 16,  0, 18,
  0, 20,  0, 22,  0, 24,  0, 26,  0, 30,  0, 91,  0, 92,  0, 93,
  0, 94,  0, 95,  0,132,  0,134,  0,135,  0,136,  0,137,  0,151,
  0,152,  0,154,  0,158,  0,160,  0,161,  0,163,  0,164,  0,166,
  0,213,  0,216,  0,222,  0,230,  0,246,  0,312,  0,313,  0,316,
  0,317,  0,318,  0,319,  0,320,  0,321,  0,322,  0,325,  0,326,
  0,346,  0,347,  0,348,  0,349,  0,350,  0,351,  0,352,  0,353,
  0,354,  0,355,  0,356,  0,357,  0,358,  0,368,  0,373,  0,380,
  0,386,  0,388,  0,389,  0,391,  0,399,  0,400,  0,401,  0,402,
  0,403,  0,404,  0,405,  0,409,  0,410,  0,411,  0,412,  0,413,
  0,414,  0,415,  0,416,  0,417,  0,418,  0,419,  0,420,  0,425,
  0,426,  0,427,  0,430,  0,445,  0,447,  0,456,  0,458,  0,460,
  0,461,  0,465,  0,474,  0,477,  0,480,  0,481,  0,482,  0,484,
  0,498,  0,500,  0,503,  0,505,  0,508,  0,513,  0,521,  0,522,
  0,523,  0,531,  0,532,0
};
static const unsigned char ag_key_ch[] = {
    0, 49, 50, 67, 73,255, 89,255, 65, 73, 76, 79, 83,255, 77, 78,255, 69,
  255, 82,255, 69, 72,255, 47, 58, 83,255, 80,255, 73,255, 69,255, 82,255,
   73,255, 65, 66, 67, 68, 69, 70, 73, 75, 76, 77, 79, 80, 82, 83, 84, 85,
   86, 88,255, 32,255, 72, 85,255, 72, 82, 85,255, 75, 84,255, 58, 82,255,
   79,255, 71, 73,255, 45,255, 72, 83,255, 71, 84,255, 67, 75, 76, 80, 85,
  255, 82, 89,255, 65, 75, 78, 79,255, 69, 73, 79,255, 80, 84,255, 78, 88,
  255, 65, 69, 82,255, 83, 84,255, 77,255, 84, 86,255, 75, 77, 78, 80, 83,
  255, 76, 78,255, 65, 85,255, 68, 82, 89,255, 65, 69, 79,255, 78, 86,255,
   67, 69, 79,255, 67, 80, 82,255, 45,255, 69,255, 78,255, 79,255, 69, 79,
  255, 72, 79, 82,255, 79,255, 65, 67, 68, 71, 83,255, 69, 80, 83,255, 58,
   83,255, 71, 80,255, 65, 67, 69, 73, 84, 85,255, 65, 85,255, 65, 67, 69,
   72, 76, 79, 82, 84, 85,255, 78,255, 82,255, 69,255, 68, 82, 83, 84,255,
   42, 58, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 76, 77, 78, 79, 80, 81,
   82, 83, 84, 85, 87, 89,255, 58, 83,255, 80,255, 73,255, 73, 80, 83, 84,
  255, 67, 69,255, 58, 83,255, 80,255, 65, 73, 85,255, 69, 72, 85,255, 42,
   67, 68, 70, 73, 77, 78, 80, 81, 82, 83, 84, 87,255, 67, 69,255, 67, 78,
   81, 82,255, 68,255, 83,255, 58, 83,255, 80,255, 73,255, 42, 73, 80, 83,
   84,255, 65, 85,255, 72, 85,255, 70, 77, 83, 84, 87,255, 78, 88,255, 72,
   79,255, 69, 73, 80,255, 58,255, 65, 69,255, 79, 82, 84,255, 67, 76, 77,
   79, 84, 85,255, 83,255, 76, 85,255, 67, 79,255, 65, 83,255, 69,255, 82,
   83,255, 65, 67, 73, 78, 79, 81, 82, 85,255, 65, 67, 68, 73, 78, 79, 81,
   82, 83,255, 65, 69, 81,255, 80, 85,255, 76, 78,255, 65, 85,255, 82, 89,
  255, 65,255, 65, 68, 70, 74, 77, 78, 79, 83,255, 84,255, 65, 66, 67, 73,
   82,255, 45,255, 72,255, 84,255, 85,255, 65,255, 75, 76,255, 65,255, 32,
   49, 79, 84,255, 77,255, 83,255, 73, 80,255, 67, 76,255, 83, 84, 85,255,
   78, 89,255, 70, 84,255, 69, 80,255, 65, 66, 67, 72, 77, 82, 84,255, 80,
   82,255, 58, 83,255, 80,255, 73,255, 83,255, 61,255, 61,255, 83, 84,255,
   77,255, 71, 85,255
};
static const unsigned char ag_key_act[] = {
  0,5,5,7,5,4,7,4,7,5,5,5,5,4,3,7,4,3,4,6,4,7,5,4,3,0,3,4,2,4,2,4,3,4,5,
  4,5,4,2,6,6,7,7,5,6,5,5,5,5,2,6,2,6,6,6,5,4,0,4,3,3,4,3,3,3,4,5,7,4,0,
  7,4,2,4,2,7,4,7,4,6,5,4,5,2,4,2,3,2,7,2,4,7,7,4,7,5,7,7,4,7,7,7,4,7,5,
  4,7,2,4,7,7,7,4,7,5,4,2,4,7,7,4,5,3,2,5,7,4,5,5,4,7,2,4,7,5,5,4,2,7,7,
  4,7,5,4,5,7,6,4,7,7,7,4,7,4,6,4,2,4,2,4,3,7,4,2,7,2,4,7,4,7,7,7,7,7,4,
  2,7,7,4,0,3,4,7,6,4,7,7,7,2,7,7,4,7,7,4,7,7,3,7,7,7,2,7,7,4,7,4,6,4,2,
  4,7,7,2,7,4,5,3,2,2,2,2,2,2,2,7,2,2,7,2,2,2,2,6,2,2,2,2,7,7,4,0,3,4,2,
  4,2,4,3,3,2,3,4,5,7,4,0,3,4,6,4,7,2,7,4,3,7,7,4,5,7,7,7,3,7,2,3,7,7,2,
  2,7,4,5,7,4,7,2,7,7,4,7,4,7,4,0,3,4,2,4,2,4,5,3,3,2,3,4,7,7,4,7,7,4,7,
  7,2,2,7,4,7,7,4,7,7,4,2,7,2,4,3,4,7,7,4,7,7,7,4,7,7,2,7,2,7,4,3,4,7,7,
  4,5,7,4,7,7,4,2,4,7,7,4,2,7,7,2,7,7,2,2,4,7,7,7,7,7,7,7,7,7,4,7,7,5,4,
  7,7,4,5,5,4,7,2,4,5,5,4,2,4,2,7,7,2,2,7,7,7,4,7,4,7,7,7,7,7,4,7,4,6,4,
  2,4,2,4,2,4,3,3,4,2,4,0,5,7,7,4,7,4,7,4,7,7,4,7,7,4,7,2,7,4,7,7,4,7,7,
  4,7,7,4,7,7,7,7,7,2,7,4,7,7,4,0,3,4,2,4,2,4,2,4,0,4,1,4,7,5,4,2,4,2,7,
  4
};
static const unsigned short ag_key_parm[] = {
    0, 18, 20, 28, 22,  0, 30,  0, 32, 10, 12,  8,  0,  0,259, 26,  0,257,
    0,  2,  0, 34, 16,  0,117,252,253,  0,  0,  0,  0,  0,185,  0, 24,  0,
   14,  0,  0,110,100,  6,  4,106, 98,112,120, 96,116,  0,108,  0,104,118,
  102,114,  0,455,  0,285,273,  0,285,256,273,  0, 28,128,  0,476,194,  0,
    0,  0,  0, 48,  0,200,  0,202,218,  0,164,  0,  0,  0,475,  0,156,  0,
    0, 58, 30,  0, 32,206,212, 60,  0,172,180,184,  0,134, 64,  0, 72,  0,
    0,192,152,146,  0,178,174,  0,  0,  0,220, 26,  0,204,259,  0, 80, 68,
    0,162,160,  0,150,  0,  0, 54,154,158,  0,  0, 82,138,  0,186,170,  0,
  214,226,222,  0,168,188, 94,  0, 70,  0, 78,  0,  0,  0,  0,  0,257,130,
    0,  0, 66,  0,  0,198,  0,182, 56,132, 34,216,  0,  0, 52,228,  0,252,
  253,  0, 62, 36,  0,148, 42,166,  0,190,136,  0, 74, 88,  0,122, 40,185,
  144, 44, 92,  0, 50,140,  0,208,  0, 76,  0,  0,  0, 38,210,  0,176,  0,
  124,295,  0,  0,  0,  0,  0,  0,  0, 46,  0,  0, 84,  0,  0,  0,  0,126,
    0,  0,  0,  0,142,224,  0,252,253,  0,  0,  0,  0,  0,259,257,  0,185,
    0,214,226,  0,252,253,  0, 36,  0,148,  0,136,  0,185,144,140,  0,124,
  212,180,146,259,138,  0,257,198,228,  0,  0,142,  0,214,226,  0,212,  0,
  198,228,  0,180,  0, 36,  0,252,253,  0,  0,  0,  0,  0,124,259,257,  0,
  185,  0,148,136,  0,144,140,  0,146,138,  0,  0,142,  0, 72, 64,  0, 70,
   66,  0,  0, 68,  0,  0,295,  0, 54, 82,  0, 92, 74, 50,  0, 60, 84,  0,
   94,  0, 76,  0,117,  0,194,218,  0,214,186,  0,182,216,  0,  0,  0,210,
  208,  0,  0,212,220,  0,188,198,  0,  0,  0,194,206,184,204,186,188,198,
  182,190,  0,128,134,126,  0,156,164,  0,162,160,  0,150,  0,  0,154,158,
    0,  0,  0,  0,172,152,  0,  0,170,168,166,  0,122,  0, 28, 30, 32, 26,
   34,  0,200,  0,202,  0,  0,  0,  0,  0,  0,  0,475,476,  0,  0,  0,455,
   86, 90, 88,  0,196,  0, 62,  0, 80, 78,  0, 40, 44,  0, 42,  0, 38,  0,
  222,224,  0,192, 88,  0, 56, 52,  0, 48, 58, 60, 46, 54,  0, 50,  0,130,
  132,  0,252,253,  0,  0,  0,  0,  0,  0,  0,520,  0,519,  0,178,174,  0,
    0,  0,  0,176,  0
};
static const unsigned short ag_key_jmp[] = {
    0,  0,  0,  0,  0,  0,  2,  0,  4,  0,  0,  0,  0,  0, 13, 15,  0, 20,
    0, 17,  0, 23,  0,  0, 30,  0, 34,  0, 24,  0, 28,  0, 36,  0,  0,  0,
    0,  0,  1,  6,  8,  9, 11,  0, 14,  0,  0,  0,  0, 19, 21, 30, 32, 34,
   36,  0,  0,  0,  0, 39, 44,  0, 53, 58, 65,  0,  0, 76,  0,  0, 86,  0,
   69,  0, 72, 91,  0, 96,  0, 77,  0,  0,  0, 79,  0, 66, 80, 74, 94, 82,
    0,100,105,  0,107,  0,112,117,  0,120,122,127,  0,137,  0,  0,132,103,
    0,142,146,148,  0,150,  0,  0,113,  0,160,177,  0,  0,158,118,  0,181,
    0,  0,  0,  0,184,127,  0,188,  0,  0,  0,133,191,196,  0,206,  0,  0,
    0,198,141,  0,209,211,216,  0,219,  0,152,  0,154,  0,156,  0,231,234,
    0,158,227,160,  0,237,  0,239,242,248,254,260,  0,169,266,270,  0,  0,
  288,  0,283,179,  0,276,278,281,182,290,294,  0,318,325,  0,296,298,300,
  303,305,307,192,327,329,  0,335,  0,205,  0,207,  0,331,333,209,339,  0,
    0, 74, 85, 91, 94, 99,106,109,116,152,121,130,186,137,144,148,163,167,
  175,185,195,211,341,344,  0,  0,355,  0,241,  0,244,  0,347,350,246,357,
    0,  0,382,  0,  0,407,  0,256,  0,405,259,409,  0,411,414,416,  0,  0,
  361,367,373,376,379,253,390,395,398,261,265,418,  0,  0,427,  0,421,283,
  435,438,  0,445,  0,451,  0,  0,462,  0,295,  0,298,  0,  0,454,457,300,
  464,  0,474,476,  0,478,480,  0,468,471,308,311,482,  0,485,490,  0,496,
  508,  0,320,492,323,  0,512,  0,520,524,  0,533,544,552,  0,514,518,332,
  529,335,554,  0,558,  0,565,573,  0,  0,601,  0,614,617,  0,354,  0,623,
  625,  0,348,576,582,351,605,611,357,359,  0,632,641,643,649,651,656,662,
  665,670,  0,675,681,  0,  0,688,690,  0,  0,  0,  0,698,388,  0,  0,  0,
    0,394,  0,385,692,695,391,397,700,703,706,  0,709,  0,712,715,718,724,
  730,  0,738,  0,416,  0,418,  0,420,  0,422,  0,742,748,  0,426,  0,  0,
    0,752,754,  0,758,  0,761,  0,768,770,  0,779,781,  0,775,443,783,  0,
  786,788,  0,791,796,  0,826,833,  0,800,805,811,815,821,456,837,  0,840,
  845,  0,  0,853,  0,470,  0,473,  0,475,  0,  0,  0,479,  0,855,  0,  0,
  483,  0,486,857,  0
};
static const unsigned short ag_key_index[] = {
   38, 57,  0, 59, 62, 62,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 57, 57,
    0,  0,216,  0,248,248,  0,269,  0,286,291,291,  0,293,  0,  0,302,314,
  314,248,  0,  0,  0,  0,  0,326,  0, 57,330, 57,  0,  0,  0,  0,339, 57,
   57, 57,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,248,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0, 62, 62,  0, 59,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,326,326,
   57,330,330,  0,330,  0,  0,  0,  0,  0,339,339,  0, 57,  0,  0,346,  0,
    0,  0,  0,  0,  0,  0,  0,  0,362,  0,371,  0,  0,  0,  0,  0,248,248,
    0,  0,  0,  0,293,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,381,  0,  0,
  248,  0,399,  0,408,  0,  0,410, 57, 57,  0,  0,  0,  0,326,330,  0,330,
    0,  0, 57,  0,  0,  0, 57,  0,  0,  0,  0,  0,339,  0,  0,  0,  0,  0,
    0,  0,424,  0,  0,  0,  0,286,  0,  0,  0,429,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,248,  0,
    0,  0,  0,  0,  0,293,  0,  0,  0,  0,  0,  0,  0,  0,381,381,  0,248,
    0,  0,  0,  0,408,408,  0, 57,  0,  0,  0,  0,  0,330,  0,  0,  0,  0,
   57,431,  0,  0,  0,  0,  0, 57,  0,  0,424,  0,  0,  0,  0,286,  0,  0,
    0,436,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,362,  0,  0,
    0,  0,  0,  0,371,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,381,  0,  0,  0,  0,  0,408,  0,  0,  0,  0,  0,  0,  0,  0,438,410,
    0,  0,440,446, 57,424,  0,  0,  0,  0,429,  0,  0,  0,424,450,  0,  0,
    0,  0,  0,  0,  0,  0,362,  0,453,  0,  0,  0,  0,371,  0,  0,  0,  0,
    0,459,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,438,
  410,  0,  0,440,446,  0,  0,  0,  0,  0,429,  0,  0,  0,  0,424,450,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,453,  0,  0,  0,  0,  0,  0,  0,
  459,  0,  0,467,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 57,
    0,  0,  0,  0,436,248,424,  0,  0,  0,  0,  0,424,  0,  0,477,  0,446,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,467,  0,  0,  0,  0,  0,
    0,  0,  0,  0, 57,  0,  0,  0,481,  0,  0,  0,  0,  0,477,477,  0,446,
    0,  0,  0,  0,  0,  0,  0,  0,  0,314,  0,  0,488,  0,  0,  0,  0, 57,
    0,  0,  0,  0,  0,438,  0,  0,330,314,  0,  0,  0,424,477,  0,  0,  0,
    0,438,  0,330,  0,  0,  0,  0,424,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0
};
static const unsigned char ag_key_ends[] = {
75,0, 69,0, 78,67,69,76,0, 84,0, 88,0, 58,0, 86,73,84,69,0, 
83,58,0, 71,73,83,84,69,82,0, 50,46,48,0, 58,0, 76,58,0, 
79,83,84,58,0, 83,69,82,73,78,70,79,58,0, 79,83,84,58,0, 
86,45,65,66,83,58,0, 83,69,82,73,78,70,79,58,0, 58,0, 73,79,78,0, 
65,86,69,82,58,0, 73,84,72,77,0, 65,83,0, 82,0, 73,78,84,0, 
65,78,67,72,0, 69,0, 78,67,69,76,0, 79,78,67,69,0, 77,80,0, 
67,0, 71,69,83,84,0, 77,65,73,78,0, 85,77,68,73,0, 73,82,69,83,0, 
76,83,69,0, 66,0, 73,0, 84,0, 73,68,68,69,78,0, 58,0, 
69,71,82,73,84,89,45,80,82,79,84,69,67,84,69,68,0, 73,84,69,0, 
85,66,0, 78,0, 82,0, 68,82,0, 84,72,79,68,0, 78,0, 
88,84,78,79,78,67,69,0, 67,69,0, 84,0, 65,81,85,69,0, 73,71,0, 
67,79,78,84,69,88,84,0, 83,84,68,0, 83,58,0, 88,89,0, 80,0, 
76,77,0, 69,73,86,69,68,0, 73,82,69,67,84,0, 73,83,84,69,82,0, 
80,79,78,83,69,0, 79,82,84,0, 80,65,85,84,72,0, 84,0, 84,80,0, 
80,0, 67,79,77,80,0, 58,0, 65,76,69,0, 78,0, 71,0, 80,0, 
76,58,0, 85,0, 83,0, 75,69,78,73,90,69,68,45,66,89,0, 
78,83,80,79,82,84,0, 69,0, 76,0, 69,0, 80,0, 73,0, 65,77,69,0, 
67,0, 69,68,0, 69,83,0, 77,58,0, 82,69,83,58,0, 58,0, 
69,76,58,0, 78,79,78,67,69,0, 73,71,69,83,84,0, 82,73,0, 77,58,0, 
79,78,0, 88,84,78,79,78,67,69,0, 82,69,83,58,0, 79,80,0, 
83,80,65,85,84,72,0, 84,0, 58,0, 78,0, 76,58,0, 85,0, 69,0, 
69,68,0, 78,79,78,67,69,0, 88,84,78,79,78,67,69,0, 79,80,0, 
83,80,65,85,84,72,0, 73,71,69,83,84,0, 73,80,0, 77,58,0, 
82,69,83,58,0, 58,0, 69,76,58,0, 82,73,0, 79,78,0, 84,0, 78,0, 
85,0, 69,0, 69,68,0, 85,77,68,73,0, 84,0, 83,85,66,0, 
79,78,69,45,67,79,78,84,69,88,84,0, 83,84,68,0, 58,0, 79,77,80,0, 
82,0, 68,68,82,0, 84,72,79,68,0, 82,73,71,0, 
75,69,78,73,90,69,68,45,66,89,0, 65,78,83,80,79,82,84,0, 76,0, 
83,69,82,0, 73,80,47,50,46,48,0, 71,79,82,73,84,72,77,0, 84,83,0, 
78,79,78,67,69,0, 
78,84,69,71,82,73,84,89,45,80,82,79,84,69,67,84,69,68,0, 78,67,69,0, 
80,65,81,85,69,0, 79,80,0, 76,77,0, 80,79,78,83,69,0, 73,0, 
69,82,78,65,77,69,0, 76,71,79,82,73,84,72,77,0, 75,0, 
79,77,65,73,78,0, 75,0, 79,78,67,69,0, 80,65,81,85,69,0, 79,80,0, 
69,65,76,77,0, 84,65,76,69,0, 67,84,73,79,78,0, 
88,80,73,82,69,83,0, 82,0, 71,0, 69,67,0, 69,66,0, 78,0, 
79,86,0, 67,84,0, 69,80,0, 65,71,0, 67,75,0, 89,69,0, 
65,78,67,69,76,0, 78,86,73,84,69,0, 69,71,73,83,84,69,82,0, 
73,78,84,0, 65,86,69,82,58,0, 71,79,58,0, 78,0, 82,85,69,0, 
68,53,0, 73,71,67,79,77,80,0, 80,0, 72,79,78,69,0, 67,84,80,0, 
80,0, 83,0, 68,80,0, 79,0, 69,83,0, 65,76,83,69,0, 82,85,69,0, 
76,73,65,83,0, 82,65,78,67,72,0, 79,77,80,0, 73,68,68,69,78,0, 
65,68,68,82,0, 67,69,73,86,69,68,0, 79,82,84,0, 84,76,0, 
82,79,88,89,0, 69,68,73,82,69,67,84,0, 58,0, 84,0, 84,67,0, 
};
#define AG_TCV(x) ag_tcv[(x)]
static const unsigned short ag_tcv[] = {
    5,552,552,552,552,552,552,552,552,537,  0,552,552,  0,552,552,552,552,
  552,552,552,552,552,552,552,552,552,552,552,552,552,552, 41,538, 61,549,
  550,229,335,538,546,546,539,191, 43, 63,244, 57,382,385,540,540,540,540,
  540,540,540,540, 37, 53, 59, 48,362, 55, 50,471,541,541,541,542,542,543,
  543,543,543,472,543,543,543,543,221,543,543,543,543,543,473,221,543,543,
  543,289, 85,290,551,544,545,471,541,541,541,542,542,543,543,543,543,472,
  543,543,543,543,221,543,543,543,543,543,473,221,543,543,543,547,551,547,
  538,552,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,
  551,548,548,551,551,551,551,551,548,551,551,551,551,551,551,551,551,551,
  551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,
  551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,
  551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,
  551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,
  551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,
  551,551,551,551
};
#ifndef SYNTAX_ERROR
#define SYNTAX_ERROR fprintf(stderr,"%s, line %d, column %d\n", \
  (PCB).error_message, (PCB).line, (PCB).column)
#endif
#ifndef FIRST_LINE
#define FIRST_LINE 1
#endif
#ifndef FIRST_COLUMN
#define FIRST_COLUMN 1
#endif
#ifndef PARSER_STACK_OVERFLOW
#define PARSER_STACK_OVERFLOW {fprintf(stderr, \
   "\nParser stack overflow, line %d, column %d\n",\
   (PCB).line, (PCB).column);}
#endif
#ifndef REDUCTION_TOKEN_ERROR
#define REDUCTION_TOKEN_ERROR {fprintf(stderr, \
    "\nReduction token error, line %d, column %d\n", \
    (PCB).line, (PCB).column);}
#endif
#ifndef INPUT_CODE
#define INPUT_CODE(T) (T)
#endif
typedef enum
  {ag_accept_key, ag_set_key, ag_jmp_key, ag_end_key, ag_no_match_key,
   ag_cf_accept_key, ag_cf_set_key, ag_cf_end_key} key_words;
static void ag_get_key_word(PCB_DECL, int ag_k) {
  int ag_save = (int) ((PCB).la_ptr - (PCB).pointer);
  const  unsigned char *ag_p;
  int ag_ch;
  while (1) {
    switch (ag_key_act[ag_k]) {
    case ag_cf_end_key: {
      const  unsigned char *sp = ag_key_ends + ag_key_jmp[ag_k];
      do {
        if ((ag_ch = *sp++) == 0) {
          int ag_k1 = ag_key_parm[ag_k];
          int ag_k2 = ag_key_pt[ag_k1];
          if (ag_key_itt[ag_k2 + CONVERT_CASE(*(PCB).la_ptr)]) goto ag_fail;
          (PCB).token_number = (SipParser_token_type) ag_key_pt[ag_k1 + 1];
          return;
        }
      } while (CONVERT_CASE(*(PCB).la_ptr++) == ag_ch);
      goto ag_fail;
    }
    case ag_end_key: {
      const  unsigned char *sp = ag_key_ends + ag_key_jmp[ag_k];
      do {
        if ((ag_ch = *sp++) == 0) {
          (PCB).token_number = (SipParser_token_type) ag_key_parm[ag_k];
          return;
        }
      } while (CONVERT_CASE(*(PCB).la_ptr++) == ag_ch);
    }
    case ag_no_match_key:
ag_fail:
      (PCB).la_ptr = (PCB).pointer + ag_save;
      return;
    case ag_cf_set_key: {
      int ag_k1 = ag_key_parm[ag_k];
      int ag_k2 = ag_key_pt[ag_k1];
      ag_k = ag_key_jmp[ag_k];
      if (ag_key_itt[ag_k2 + CONVERT_CASE(*(PCB).la_ptr)]) break;
      ag_save = (int) ((PCB).la_ptr - (PCB).pointer);
      (PCB).token_number = (SipParser_token_type) ag_key_pt[ag_k1+1];
      break;
    }
    case ag_set_key:
      ag_save = (int) ((PCB).la_ptr - (PCB).pointer);
      (PCB).token_number = (SipParser_token_type) ag_key_parm[ag_k];
    case ag_jmp_key:
      ag_k = ag_key_jmp[ag_k];
      break;
    case ag_accept_key:
      (PCB).token_number = (SipParser_token_type) ag_key_parm[ag_k];
      return;
    case ag_cf_accept_key: {
      int ag_k1 = ag_key_parm[ag_k];
      int ag_k2 = ag_key_pt[ag_k1];
      if (ag_key_itt[ag_k2 + CONVERT_CASE(*(PCB).la_ptr)])
        (PCB).la_ptr = (PCB).pointer + ag_save;
      else (PCB).token_number = (SipParser_token_type) ag_key_pt[ag_k1+1];
      return;
    }
    }
    ag_ch = CONVERT_CASE(*(PCB).la_ptr++);
    ag_p = &ag_key_ch[ag_k];
    while (*ag_p < ag_ch) ag_p++;
    if (*ag_p != ag_ch) {
      (PCB).la_ptr = (PCB).pointer + ag_save;
      return;
    }
    ag_k = (int) (ag_p - ag_key_ch);
  }
}
#ifndef AG_NEWLINE
#define AG_NEWLINE 10
#endif
#ifndef AG_RETURN
#define AG_RETURN 13
#endif
#ifndef AG_FORMFEED
#define AG_FORMFEED 12
#endif
#ifndef AG_TABCHAR
#define AG_TABCHAR 9
#endif
static void ag_track(PCB_DECL) {
  int ag_k = (int) ((PCB).la_ptr - (PCB).pointer);
  while (ag_k--) {
    switch (*(PCB).pointer++) {
    case AG_NEWLINE:
      (PCB).column = 1, (PCB).line++;
    case AG_RETURN:
    case AG_FORMFEED:
      break;
    case AG_TABCHAR:
      (PCB).column += (TAB_SPACING) - ((PCB).column - 1) % (TAB_SPACING);
      break;
    default:
      (PCB).column++;
    }
  }
}
static const unsigned short ag_tstt[] = {
545,544,543,542,541,540,539,538,473,472,471,385,382,358,357,356,355,354,353,
  352,351,350,349,348,347,346,259,257,253,252,244,229,221,191,185,117,95,
  94,93,92,91,63,30,26,24,22,20,18,16,14,12,10,8,6,2,0,1,28,29,32,33,34,
  35,36,64,66,90,113,182,183,247,248,249,250,254,258,344,345,
545,544,543,542,541,540,539,538,473,472,471,385,382,244,229,221,191,63,0,
549,546,542,541,540,539,471,385,382,244,191,63,0,184,186,188,189,190,197,
  198,199,
285,273,0,251,260,263,270,284,
285,273,256,0,251,255,260,263,270,284,
285,273,256,0,251,255,260,263,270,284,
537,41,37,0,38,39,40,46,
41,0,40,114,
5,0,
5,0,
5,0,
5,0,
5,0,
37,0,3,
5,0,
5,0,
37,0,3,
37,0,3,
37,0,3,
37,0,3,
37,0,3,
37,0,3,
37,0,3,
37,0,3,
37,0,3,
37,0,3,
37,0,3,
37,0,3,
549,546,542,541,540,539,471,385,382,244,63,0,199,
549,546,542,541,540,539,537,471,385,382,362,244,63,61,53,43,41,5,0,201,202,
  203,
546,540,385,382,244,63,0,197,
537,362,61,53,41,5,0,52,187,205,
543,542,541,540,473,472,471,385,382,289,221,0,139,150,272,286,
550,549,546,544,543,542,541,540,539,538,473,472,471,385,382,335,244,229,221,
  191,63,57,55,53,50,48,43,0,49,271,274,276,278,
537,455,362,61,55,53,41,5,0,52,261,301,
537,455,362,61,55,53,41,5,0,52,261,301,
543,542,541,540,473,472,471,385,382,244,221,191,63,0,170,171,174,
543,542,541,540,473,472,471,385,382,244,221,191,63,0,170,171,174,
537,41,0,
37,0,51,
543,542,541,540,537,473,472,471,385,382,259,257,253,252,244,221,191,185,63,
  41,37,0,
543,542,541,540,473,472,471,385,382,259,257,253,252,244,221,191,185,63,0,
  168,169,170,171,174,182,183,247,248,249,250,254,258,
540,385,382,41,0,40,115,118,
548,547,546,545,544,543,542,541,540,539,538,537,532,531,505,503,477,473,472,
  471,430,405,404,403,402,401,400,399,385,382,373,362,290,289,259,257,253,
  252,244,229,221,191,185,132,85,63,61,59,57,55,41,37,0,38,39,46,
543,542,541,540,473,472,471,385,382,244,221,191,63,0,31,170,171,174,
532,531,505,503,477,0,27,492,493,526,527,528,529,530,
545,544,543,542,541,540,539,538,473,472,471,430,385,382,244,229,221,191,63,
  0,25,64,66,486,487,
545,544,543,542,541,540,539,538,473,472,471,430,385,382,244,229,221,191,63,
  0,23,64,66,428,429,
545,544,543,542,541,540,539,538,473,472,471,385,382,244,229,221,191,63,61,
  59,0,21,58,79,176,360,363,524,525,
545,544,543,542,541,540,539,538,473,472,471,385,382,244,229,221,191,132,63,
  0,19,64,66,123,124,125,129,
540,385,382,0,17,99,100,
548,547,546,545,544,543,542,541,540,539,538,473,472,471,385,382,362,290,289,
  244,229,221,191,85,63,61,59,57,55,37,0,15,67,69,101,
545,544,543,542,541,540,539,538,473,472,471,385,382,373,259,257,253,252,244,
  229,221,191,185,63,61,59,0,13,58,79,175,176,183,250,254,258,264,265,266,
  267,268,269,360,363,370,371,372,374,
540,405,404,403,402,401,400,399,385,382,0,11,394,395,396,398,406,
540,405,404,403,402,401,400,399,385,382,0,9,99,390,393,394,398,
545,544,543,542,541,540,539,538,473,472,471,385,382,259,257,253,252,244,229,
  221,191,185,63,61,59,0,7,58,79,175,176,183,250,254,258,264,265,266,267,
  268,269,359,360,363,
540,385,382,0,4,96,97,99,
549,546,542,541,540,539,537,471,385,382,362,244,63,61,53,43,41,5,0,201,202,
  203,
549,546,542,541,540,539,471,385,382,244,63,0,201,
546,540,385,382,244,63,0,
546,540,537,385,382,362,244,63,61,53,43,41,5,0,193,194,195,
550,546,544,543,542,541,540,539,538,537,473,472,471,385,382,335,290,289,246,
  244,230,229,222,221,216,213,191,63,57,41,37,0,38,39,46,
53,0,52,
544,543,542,541,540,473,472,471,385,382,244,221,63,0,288,
542,541,540,471,385,382,295,0,167,293,294,297,
37,0,51,
543,542,541,540,473,472,471,385,382,289,221,0,139,150,272,286,
550,549,546,544,543,542,541,540,539,538,473,472,471,385,382,335,244,229,221,
  191,63,57,55,53,48,43,0,
37,0,51,
50,0,49,
550,546,544,543,542,541,540,539,538,537,473,472,471,385,382,335,326,325,319,
  318,313,312,290,289,244,229,221,191,164,160,154,63,57,41,37,0,38,39,46,
53,0,52,
537,455,362,61,55,41,5,0,54,262,333,
537,455,362,61,55,41,5,0,54,262,333,
543,542,541,540,473,472,471,385,382,244,221,191,63,0,
37,0,51,
552,551,550,549,548,547,546,545,544,543,542,541,540,539,538,537,473,472,471,
  385,382,362,335,290,289,244,229,221,191,85,63,61,59,57,55,53,50,48,43,
  41,37,5,0,38,39,46,
41,0,40,114,
540,385,382,0,
41,0,40,
5,0,
537,48,41,0,38,39,46,
537,48,41,0,38,39,46,
537,48,41,0,38,39,46,
537,48,41,0,38,39,46,
537,48,41,0,38,39,46,
537,43,41,5,0,38,39,46,
5,0,
537,41,0,38,46,
537,41,0,38,46,
5,0,
537,41,0,38,46,
537,41,0,38,46,
5,0,
545,544,543,542,541,540,539,538,537,473,472,471,385,382,244,229,221,191,63,
  59,41,37,0,364,365,366,
552,551,550,549,548,547,546,545,544,543,542,541,540,539,538,537,473,472,471,
  385,382,362,335,290,289,244,229,221,191,85,63,61,59,57,55,53,50,48,43,
  41,37,5,0,80,81,82,83,
545,544,543,542,541,540,539,538,473,472,471,385,382,244,229,221,191,63,0,
537,59,41,37,0,38,39,46,
543,542,541,540,537,473,472,471,385,382,259,257,253,252,244,221,191,185,63,
  41,0,38,39,46,
59,0,58,
537,53,43,41,5,0,38,39,46,
43,0,44,
5,0,
537,57,41,0,38,39,46,
537,41,0,38,46,
43,0,44,
5,0,
540,385,382,0,
537,41,5,0,38,39,46,
5,0,
548,547,546,545,544,543,542,541,540,539,538,473,472,471,385,382,362,290,289,
  244,229,221,191,85,63,61,59,57,55,37,0,
50,0,49,
537,41,5,0,38,39,46,
5,0,
37,0,51,
285,273,256,0,255,260,263,270,284,
285,273,256,0,255,260,263,270,284,
549,546,542,541,540,539,471,385,382,244,191,63,0,186,188,189,190,197,198,
  199,
285,273,0,260,263,270,284,
59,0,58,
53,0,52,375,
537,53,43,41,5,0,38,39,46,
43,0,44,
5,0,
540,385,382,0,
537,43,41,0,38,39,46,
537,41,0,38,46,
537,41,0,38,46,
537,41,5,0,38,39,46,
5,0,
540,385,382,0,
537,41,5,0,38,39,46,
5,0,
53,0,52,361,
537,53,41,5,0,38,39,46,
5,0,
540,385,382,0,
537,41,0,38,46,
537,41,5,0,38,39,46,
5,0,
546,540,537,385,382,362,244,63,61,53,43,41,5,0,193,194,195,
546,540,385,382,244,63,0,193,
550,546,544,543,542,541,540,539,538,473,472,471,385,382,335,290,289,246,244,
  230,229,222,221,216,213,191,63,57,37,0,206,207,208,209,210,211,212,327,
550,546,544,543,542,541,540,539,538,537,473,472,471,385,382,335,290,289,246,
  244,230,229,222,221,216,213,191,63,57,41,37,0,38,39,46,
544,543,542,541,540,473,472,471,385,382,244,221,63,0,
542,541,540,471,385,382,0,
537,295,290,244,53,43,41,37,5,0,298,299,300,
542,541,540,537,471,385,382,290,244,53,43,41,5,0,294,296,297,
295,0,
244,0,
290,0,
540,385,382,0,99,138,
550,546,544,543,542,541,540,539,538,473,472,471,385,382,335,244,229,221,191,
  63,50,48,43,0,275,279,281,282,283,
543,542,541,540,473,472,471,385,382,289,221,0,139,150,272,286,
550,546,544,543,542,541,540,539,538,473,472,471,385,382,335,326,325,319,318,
  313,312,290,289,244,229,221,191,164,160,154,63,57,37,0,212,302,303,304,
  305,306,307,308,309,310,311,327,
550,546,544,543,542,541,540,539,538,537,473,472,471,385,382,335,326,325,319,
  318,313,312,290,289,244,229,221,191,164,160,154,63,57,41,37,0,38,39,46,
550,546,544,543,542,541,540,539,538,473,472,471,385,382,290,289,244,229,221,
  191,63,57,55,37,0,334,336,339,
335,0,
550,549,546,544,543,542,541,540,539,538,537,473,472,471,385,382,335,290,289,
  244,229,221,191,63,57,55,53,50,48,43,41,37,0,38,39,46,
552,551,550,549,548,547,546,545,544,543,542,541,540,539,538,473,472,471,385,
  382,362,335,290,289,244,229,221,191,85,63,61,59,57,55,53,50,48,43,37,0,
  73,
117,41,0,40,113,
540,385,382,0,
551,550,549,548,547,546,545,544,543,542,541,540,539,538,473,472,471,385,382,
  362,335,290,289,244,229,221,191,85,63,61,59,57,55,53,50,48,43,41,37,5,0,
  116,120,121,122,
48,0,47,
48,0,47,
48,0,47,
48,0,47,
48,0,47,
43,0,42,
545,544,543,542,541,540,539,538,473,472,471,385,382,244,229,221,191,63,0,64,
  66,432,485,
545,544,543,542,541,540,539,538,521,513,508,505,503,500,498,477,473,472,471,
  465,458,456,445,385,382,244,229,221,191,63,0,66,434,436,437,440,442,462,
  488,489,490,491,492,493,494,495,496,497,
545,544,543,542,541,540,539,538,473,472,471,385,382,244,229,221,191,63,0,64,
  66,432,485,
545,544,543,542,541,540,539,538,484,482,477,473,472,471,465,460,458,456,447,
  445,385,382,244,229,221,191,63,0,66,431,433,434,435,436,437,438,439,440,
  441,442,443,462,
537,41,0,364,
552,551,550,549,548,547,546,545,544,543,542,541,540,539,538,537,473,472,471,
  385,382,362,335,290,289,244,229,221,191,85,63,61,59,57,55,53,50,48,43,
  41,37,5,0,
552,551,550,549,548,547,546,545,544,543,542,541,540,539,538,537,473,472,471,
  385,382,362,335,290,289,244,229,221,191,85,63,59,57,55,53,50,48,43,41,
  37,5,0,80,81,
61,0,
545,544,543,542,541,540,539,538,537,473,472,471,385,382,244,229,221,191,63,
  59,41,37,0,364,365,366,
543,542,541,540,473,472,471,385,382,259,257,253,252,244,221,191,185,63,0,
  169,170,171,174,182,183,247,248,249,250,254,258,
543,542,541,540,537,473,472,471,385,382,259,257,253,252,244,221,191,185,63,
  41,0,38,39,46,
53,0,52,110,
545,544,543,542,541,540,539,538,537,473,472,471,385,382,244,229,221,191,63,
  61,59,41,0,38,39,46,
57,0,56,
543,542,541,540,473,472,471,385,382,289,221,0,126,139,150,286,
545,544,543,542,541,540,539,538,537,473,472,471,385,382,244,229,221,191,132,
  63,41,0,38,39,46,
548,547,546,545,544,543,542,541,540,539,538,473,472,471,385,382,362,290,289,
  244,229,221,191,85,63,61,59,57,55,37,0,67,69,
550,549,546,544,543,542,541,540,539,538,473,472,471,385,382,335,290,289,244,
  229,221,191,63,57,50,48,37,0,179,181,
545,544,543,542,541,540,539,538,473,472,471,385,382,244,229,221,191,63,61,0,
  79,175,176,269,363,
537,55,53,43,41,5,0,38,39,46,
537,55,53,43,41,5,0,38,39,46,
545,544,543,542,541,540,539,538,473,472,471,385,382,244,229,221,191,63,61,0,
  79,175,176,269,363,
537,55,53,43,41,5,0,38,39,46,
537,55,53,43,41,5,0,38,39,46,
537,55,53,43,41,5,0,38,39,46,
537,55,53,43,41,5,0,38,39,46,
545,544,543,542,541,540,539,538,537,473,472,471,391,386,385,382,380,244,229,
  221,191,63,41,0,38,39,46,
53,0,52,
53,0,52,375,
545,544,543,542,541,540,539,538,537,473,472,471,385,382,259,257,253,252,244,
  229,221,191,185,63,61,59,41,0,38,39,46,
43,0,42,
420,419,418,417,416,415,414,413,412,411,410,409,0,407,
540,385,382,0,397,421,
545,544,543,542,541,540,539,538,537,473,472,471,385,382,368,244,229,221,191,
  63,41,0,38,39,46,
53,0,52,
53,0,52,361,
545,544,543,542,541,540,539,538,473,472,471,385,382,244,229,221,191,95,94,
  93,92,91,63,0,64,66,90,
550,546,544,543,542,541,540,539,538,537,473,472,471,455,385,382,362,335,290,
  289,244,229,221,191,63,61,57,55,53,48,41,37,5,0,330,331,332,
48,0,47,328,
537,48,41,0,38,39,46,
537,48,41,0,38,39,46,
537,48,41,0,38,39,46,
537,48,41,0,38,39,46,
550,546,544,543,542,541,540,539,538,473,472,471,385,382,335,290,289,246,244,
  230,229,222,221,216,213,191,63,57,37,0,206,207,208,209,210,211,212,327,
542,541,540,471,385,382,0,
542,541,540,471,385,382,0,297,
37,0,298,
542,541,540,537,471,385,382,290,244,53,43,41,5,0,294,296,297,
540,385,382,0,291,292,
540,385,382,0,
550,546,544,543,542,541,540,539,538,473,472,471,385,382,335,244,229,221,191,
  63,48,43,0,281,
537,48,41,0,38,39,46,
537,48,41,0,38,39,46,
545,544,543,542,541,540,539,538,537,473,472,471,455,385,382,362,244,229,221,
  191,63,61,55,53,48,41,5,0,47,323,
537,48,41,0,38,39,46,
537,48,41,0,38,39,46,
537,48,41,0,38,39,46,
537,48,41,0,38,39,46,
537,48,41,0,38,39,46,
550,546,544,543,542,541,540,539,538,473,472,471,385,382,335,326,325,319,318,
  313,312,290,289,244,229,221,191,164,160,154,63,57,37,0,212,302,303,304,
  305,306,307,308,309,310,311,327,
550,546,544,543,542,541,540,539,538,473,472,471,385,382,290,289,244,229,221,
  191,63,57,55,37,0,
48,0,
550,546,544,543,542,541,540,539,538,473,472,471,385,382,290,289,244,229,221,
  191,63,57,55,37,0,334,336,339,
550,549,546,544,543,542,541,540,539,538,473,472,471,385,382,335,290,289,244,
  229,221,191,63,57,55,53,50,48,43,37,0,172,178,
552,551,550,549,548,547,546,545,544,543,542,541,540,539,538,537,473,472,471,
  385,382,362,335,290,289,244,229,221,191,85,63,61,59,57,55,53,50,48,43,
  41,37,5,0,76,77,78,
537,41,5,0,38,39,46,
551,550,549,548,547,546,545,544,543,542,541,540,539,538,473,472,471,385,382,
  362,335,290,289,244,229,221,191,85,63,61,59,57,55,53,50,48,43,41,37,0,
  120,
545,544,543,542,541,540,539,538,537,481,480,473,472,471,385,382,244,229,221,
  191,63,61,41,0,38,39,46,
537,61,41,0,38,39,46,
537,61,41,0,38,39,46,
537,61,41,0,38,39,46,
542,541,540,537,471,385,382,41,0,38,39,46,
537,532,531,505,503,477,41,0,38,39,46,
537,48,41,0,38,39,46,
43,0,42,
537,48,41,0,38,39,46,
476,475,0,463,466,470,
537,48,41,0,38,39,46,
537,48,41,0,38,39,46,
537,48,41,0,38,39,46,
537,48,41,0,38,39,46,
545,544,543,542,541,540,539,538,537,473,472,471,385,382,244,229,221,191,63,
  48,41,0,46,
537,48,41,0,38,39,46,
537,48,41,0,38,39,46,
537,48,41,0,38,39,46,
537,48,41,0,38,39,46,
537,48,41,0,38,39,46,
537,43,41,5,0,38,39,46,
43,0,42,
43,0,42,
537,48,41,0,38,39,46,
537,48,41,0,38,39,46,
537,48,41,0,38,39,46,
537,48,41,0,38,39,46,
537,48,41,0,38,39,46,
545,544,543,542,541,540,539,538,537,473,472,471,385,382,244,229,221,191,63,
  48,41,0,46,
537,43,41,5,0,38,39,46,
43,0,42,
537,362,41,0,38,39,46,
543,542,541,540,473,472,471,385,382,259,257,253,252,244,221,191,185,63,0,
  169,170,171,174,182,183,247,248,249,250,254,258,
545,544,543,542,541,540,539,538,537,473,472,471,385,382,244,229,221,191,63,
  41,0,38,39,46,
53,0,52,
545,544,543,542,541,540,539,538,473,472,471,385,382,244,229,221,191,63,61,
  59,0,58,79,176,360,363,525,
545,544,543,542,541,540,539,538,537,473,472,471,385,382,244,229,221,191,63,
  41,0,38,39,46,
537,53,43,41,37,5,0,38,39,46,
53,43,5,0,127,
545,544,543,542,541,540,539,538,473,472,471,385,382,244,229,221,191,132,63,
  0,64,66,124,125,129,
550,549,546,544,543,542,541,540,539,538,473,472,471,385,382,335,290,289,244,
  229,221,191,63,57,50,48,37,0,
537,53,43,41,5,0,38,39,46,
55,53,43,5,0,54,262,333,
55,53,43,5,0,54,262,333,
55,53,43,5,0,54,262,333,
55,53,43,5,0,54,262,333,
55,53,43,5,0,54,262,333,
55,53,43,5,0,54,262,333,
545,544,543,542,541,540,539,538,473,472,471,391,386,385,382,380,244,229,221,
  191,63,0,64,66,149,376,377,378,379,
545,544,543,542,541,540,539,538,537,473,472,471,391,386,385,382,380,244,229,
  221,191,63,41,0,38,39,46,
53,0,52,
545,544,543,542,541,540,539,538,473,472,471,385,382,259,257,253,252,244,229,
  221,191,185,63,61,59,0,58,79,175,176,183,250,254,258,264,265,266,267,
  268,269,360,363,374,
540,537,385,382,41,0,38,39,46,
537,41,0,38,46,
540,385,382,0,
37,0,51,
545,544,543,542,541,540,539,538,473,472,471,385,382,368,244,229,221,191,63,
  0,64,66,149,367,369,
545,544,543,542,541,540,539,538,537,473,472,471,385,382,368,244,229,221,191,
  63,41,0,38,39,46,
53,0,52,
550,546,544,543,542,541,540,539,538,473,472,471,385,382,335,290,289,244,229,
  221,191,63,57,37,0,330,
550,546,544,543,542,541,540,539,538,537,473,472,471,385,382,335,290,289,244,
  229,221,191,63,57,41,37,0,38,39,46,
48,0,47,
48,0,47,
48,0,47,
48,0,47,
542,541,540,471,385,382,0,
540,385,382,0,
244,0,
48,0,47,
48,0,47,
545,544,543,542,541,540,539,538,537,473,472,471,455,385,382,362,244,229,221,
  191,63,61,55,53,41,5,0,64,66,324,
322,321,320,0,
48,0,47,
48,0,47,
48,0,47,
48,0,47,
48,0,47,
550,546,544,543,542,541,540,539,538,537,473,472,471,455,385,382,362,335,290,
  289,244,229,221,191,63,61,57,55,53,50,43,41,37,5,0,337,341,342,343,
550,549,546,544,543,542,541,540,539,538,473,472,471,385,382,335,290,289,244,
  229,221,191,63,57,55,53,50,48,43,37,0,
552,551,550,549,548,547,546,545,544,543,542,541,540,539,538,537,473,472,471,
  385,382,362,335,290,289,244,229,221,191,85,63,61,59,57,55,53,50,48,43,
  41,37,0,76,
545,544,543,542,541,540,539,538,481,480,473,472,471,385,382,244,229,221,191,
  63,61,0,60,502,
61,0,533,
61,0,79,457,504,
61,0,79,457,
542,541,540,471,385,382,0,506,507,512,
532,531,505,503,477,0,492,493,526,527,528,529,530,
48,0,47,112,
545,544,543,542,541,540,539,538,537,473,472,471,385,382,244,229,221,191,63,
  41,0,38,39,46,
48,0,47,
545,544,543,542,541,540,539,538,474,473,472,471,385,382,244,229,221,191,63,
  0,64,66,469,
471,0,467,
537,61,43,41,5,0,464,
48,0,47,
48,0,47,
48,0,47,
48,0,47,
537,48,41,0,
545,544,543,542,541,540,539,538,537,473,472,471,385,382,244,229,221,191,63,
  61,41,0,364,365,366,
48,0,47,
48,0,47,
48,0,47,
48,0,47,
48,0,47,
545,544,543,542,541,540,539,538,537,521,513,508,505,503,500,498,477,473,472,
  471,465,458,456,445,385,382,244,229,221,191,63,41,0,38,39,46,
48,0,47,
48,0,47,
48,0,47,
48,0,47,
48,0,47,448,
545,544,543,542,541,540,539,538,537,473,472,471,385,382,244,229,221,191,63,
  48,41,0,66,444,
545,544,543,542,541,540,539,538,537,484,482,477,473,472,471,465,460,458,456,
  447,445,385,382,244,229,221,191,63,41,0,38,39,46,
362,0,
537,362,41,0,38,39,46,
545,544,543,542,541,540,539,538,473,472,471,385,382,244,229,221,191,63,0,64,
  66,111,
545,544,543,542,541,540,539,538,537,473,472,471,385,382,244,229,221,191,63,
  41,0,38,39,46,
545,544,543,542,541,540,539,538,473,472,471,385,382,244,229,221,191,63,0,64,
  66,130,
37,0,51,
53,0,52,
537,53,48,43,41,5,0,38,39,46,
537,48,41,0,38,39,46,
537,48,41,0,38,39,46,
537,48,41,0,38,39,46,
537,53,43,41,5,0,38,39,46,
537,53,43,41,5,0,38,39,46,
537,53,43,41,5,0,38,39,46,
545,544,543,542,541,540,539,538,473,472,471,391,386,385,382,380,244,229,221,
  191,63,0,64,66,149,376,377,378,379,
540,385,382,0,396,406,
540,385,382,0,408,
540,385,382,0,422,
537,48,41,0,38,39,46,
537,53,41,5,0,38,39,46,
545,544,543,542,541,540,539,538,473,472,471,385,382,368,244,229,221,191,63,
  0,64,66,149,367,369,
550,546,544,543,542,541,540,539,538,473,472,471,385,382,335,290,289,244,229,
  221,191,63,57,37,0,327,
543,542,541,540,537,473,472,471,385,382,221,191,41,0,38,39,46,
550,546,544,543,542,541,540,539,538,537,473,472,471,385,382,335,244,229,221,
  191,63,57,55,50,48,43,41,37,0,38,39,46,
549,546,541,540,539,537,471,385,382,244,221,63,41,0,38,39,46,
546,540,537,385,382,244,63,41,0,38,39,46,
540,385,382,0,
540,385,382,0,292,
543,542,541,540,537,473,472,471,385,382,289,221,41,0,38,39,46,
545,544,543,542,541,540,539,538,537,473,472,471,385,382,244,229,221,191,166,
  63,41,0,38,39,46,
545,544,543,542,541,540,539,538,537,473,472,471,385,382,244,229,221,191,95,
  94,93,92,91,63,41,0,38,39,46,
543,542,541,540,537,473,472,471,385,382,289,221,41,0,38,39,46,
540,537,385,382,41,0,38,39,46,
545,544,543,542,541,540,539,538,537,473,472,471,385,382,317,316,244,229,221,
  191,63,41,0,38,39,46,
545,544,543,542,541,540,539,538,537,473,472,471,385,382,244,229,221,191,137,
  136,135,134,63,41,0,38,39,46,
550,546,544,543,542,541,540,539,538,473,472,471,385,382,290,289,244,229,221,
  191,63,57,55,50,37,0,341,
545,544,543,542,541,540,539,538,481,480,473,472,471,385,382,244,229,221,191,
  63,0,64,66,479,
542,541,540,471,385,382,61,0,507,512,534,535,536,
542,541,540,471,385,382,0,507,512,
545,544,543,542,541,540,539,538,537,473,472,471,385,382,244,229,221,191,63,
  61,41,0,38,39,46,
545,544,543,542,541,540,539,538,473,472,471,385,382,244,229,221,191,63,0,64,
  66,485,
537,61,41,0,38,46,60,
472,0,
540,385,382,0,99,468,
537,61,41,0,38,39,46,
545,544,543,542,541,540,539,538,537,481,480,473,472,471,385,382,244,229,221,
  191,63,61,41,0,38,39,46,
537,523,522,41,0,38,39,46,
537,61,41,0,38,39,46,
545,544,543,542,541,540,539,538,537,473,472,471,385,382,244,229,221,191,63,
  61,41,0,364,365,366,
545,544,543,542,541,540,539,538,473,472,471,385,382,244,229,221,191,63,61,0,
  66,
537,61,41,0,38,39,46,
537,61,41,0,38,39,46,
537,61,41,0,38,39,46,
537,61,41,0,38,39,46,
537,61,41,0,38,39,46,
545,544,543,542,541,540,539,538,521,513,508,505,503,500,498,477,473,472,471,
  465,458,456,445,385,382,244,229,221,191,63,0,66,434,436,437,440,442,462,
  489,490,491,492,493,494,495,496,497,
537,61,41,0,38,39,46,
537,461,321,41,0,38,39,46,
61,0,483,
61,0,483,
537,61,41,0,38,39,46,449,
545,544,543,542,541,540,539,538,537,473,472,471,385,382,244,229,221,191,63,
  48,41,0,364,365,366,
545,544,543,542,541,540,539,538,484,482,477,473,472,471,465,460,458,456,447,
  445,385,382,244,229,221,191,63,0,66,433,434,435,436,437,438,439,440,441,
  442,443,462,
362,0,
537,53,48,43,41,5,0,38,39,46,
545,544,543,542,541,540,539,538,473,472,471,385,382,244,229,221,191,63,0,64,
  66,111,
537,57,41,0,38,39,46,
540,537,385,382,41,0,38,39,46,
545,544,543,542,541,540,539,538,537,473,472,471,385,382,244,229,221,191,164,
  163,161,160,158,154,152,151,63,41,0,38,39,46,
48,0,47,
48,0,47,
48,0,47,
48,0,47,
537,41,0,38,46,
540,385,382,0,
540,385,382,0,
37,0,51,
48,0,47,
543,542,541,540,473,472,471,385,382,221,191,0,190,231,232,233,234,
550,546,544,543,542,541,540,539,538,473,472,471,385,382,335,244,229,221,191,
  63,57,55,50,48,43,37,0,223,224,225,228,
549,546,541,540,539,471,385,382,244,221,63,0,217,218,219,
546,540,385,382,244,63,0,214,215,
244,0,
543,542,541,540,473,472,471,385,382,289,221,0,139,150,286,
545,544,543,542,541,540,539,538,473,472,471,385,382,244,229,221,191,166,63,
  0,64,66,165,
545,544,543,542,541,540,539,538,473,472,471,385,382,244,229,221,191,95,94,
  93,92,91,63,0,64,66,90,
543,542,541,540,473,472,471,385,382,289,221,0,139,150,286,
540,385,382,0,155,
545,544,543,542,541,540,539,538,473,472,471,385,382,317,316,244,229,221,191,
  63,0,64,66,314,315,
545,544,543,542,541,540,539,538,473,472,471,385,382,244,229,221,191,137,136,
  135,134,63,0,64,66,133,
537,61,43,41,5,0,464,
542,541,540,471,385,382,0,507,512,534,
61,0,
542,541,540,471,385,382,0,507,512,
545,544,543,542,541,540,539,538,473,472,471,385,382,244,229,221,191,63,61,0,
  64,66,79,
61,0,60,
471,0,
540,385,382,0,
63,0,62,
61,0,60,501,
545,544,543,542,541,540,539,538,481,480,473,472,471,385,382,244,229,221,191,
  63,61,0,60,502,
523,522,0,
61,0,514,
545,544,543,542,541,540,539,538,473,472,471,385,382,244,229,221,191,63,61,0,
  66,
552,551,550,549,548,547,546,545,544,543,542,541,540,539,538,537,473,472,471,
  385,382,362,335,290,289,244,229,221,191,85,63,61,59,57,55,53,50,48,43,
  41,37,5,0,80,81,82,83,
545,544,543,542,541,540,539,538,473,472,471,385,382,244,229,221,191,63,0,
61,0,79,459,
61,0,509,
61,0,79,457,
61,0,79,446,
61,0,79,499,
537,43,41,5,0,38,39,46,
61,0,60,
461,321,0,
542,541,540,471,385,382,0,87,89,
61,0,
48,0,
537,43,41,5,0,38,39,46,
48,0,47,112,
57,0,56,
540,385,382,0,99,138,
545,544,543,542,541,540,539,538,473,472,471,385,382,244,229,221,191,164,163,
  161,160,158,154,152,151,63,0,64,66,128,140,141,142,143,144,145,146,147,
  148,149,153,156,159,
545,544,543,542,541,540,539,538,537,473,472,471,385,382,289,244,229,221,191,
  63,61,41,0,38,39,46,
540,537,385,382,61,41,0,38,39,46,
537,389,388,41,0,38,39,46,
537,385,382,41,0,38,39,46,
540,385,382,0,397,421,
540,385,382,0,
540,385,382,0,423,
545,544,543,542,541,540,539,538,537,473,472,471,385,382,244,229,221,191,63,
  41,0,38,39,46,
543,542,541,540,537,473,472,471,385,382,362,244,221,63,61,53,41,5,0,240,241,
  242,243,
543,542,541,540,537,473,472,471,385,382,362,244,221,63,61,53,41,5,0,240,241,
  242,243,
537,362,244,61,53,41,5,0,235,236,237,238,
542,541,540,471,385,382,0,
550,546,544,543,542,541,540,539,538,473,472,471,385,382,335,244,229,221,191,
  63,57,55,50,48,43,37,0,224,228,
549,546,541,540,539,471,385,382,244,221,63,0,218,
546,540,385,382,244,63,0,
540,385,382,0,292,
540,385,382,0,
542,541,540,471,385,382,0,507,512,
537,53,43,41,5,0,38,39,46,
537,53,43,41,5,0,38,39,46,
473,0,
545,544,543,542,541,540,539,538,474,473,472,471,385,382,244,229,221,191,63,
  0,64,66,469,
543,542,541,540,473,472,471,385,382,259,257,253,252,244,221,191,185,63,0,
  168,169,170,171,174,182,183,247,248,249,250,254,258,
545,544,543,542,541,540,539,538,481,480,473,472,471,385,382,244,229,221,191,
  63,0,64,66,479,
543,542,541,540,473,472,471,385,382,221,191,57,0,516,
552,551,550,549,548,547,546,545,544,543,542,541,540,539,538,537,473,472,471,
  385,382,362,335,290,289,244,229,221,191,85,63,61,59,57,55,53,50,48,43,
  41,37,5,0,80,81,82,83,
545,544,543,542,541,540,539,538,473,472,471,385,382,244,229,221,191,63,0,
61,0,
542,541,540,537,471,385,382,61,41,0,38,39,46,507,510,512,
545,544,543,542,541,540,539,538,481,480,473,472,471,385,382,244,229,221,191,
  63,0,64,66,478,479,
542,541,540,471,385,382,0,
61,0,
545,544,543,542,541,540,539,538,537,473,472,471,385,382,253,252,244,229,221,
  191,63,61,41,0,46,450,
545,544,543,542,541,540,539,538,537,473,472,471,385,382,244,229,221,191,63,
  61,41,0,364,365,366,
545,544,543,542,541,540,539,538,537,473,472,471,385,382,244,229,221,191,137,
  136,135,134,63,41,0,38,39,46,
537,53,43,41,5,0,38,39,46,
537,53,48,43,41,5,0,38,39,46,
537,48,41,0,38,39,46,
537,48,41,0,38,39,46,
537,48,41,0,38,39,46,
53,48,43,5,0,47,157,
537,48,41,0,38,39,46,
537,48,41,0,38,39,46,
537,53,43,41,5,0,38,39,46,
537,53,43,41,5,0,38,39,46,
545,544,543,542,541,540,539,538,473,472,471,385,382,289,244,229,221,191,63,
  61,0,64,66,79,150,
540,385,382,61,0,60,99,392,393,
389,388,0,387,
385,382,0,381,
540,385,382,0,
540,385,382,0,
537,41,0,38,46,
545,544,543,542,541,540,539,538,473,472,471,385,382,244,229,221,191,63,0,64,
  66,
543,542,541,540,473,472,471,385,382,221,63,0,240,241,
543,542,541,540,473,472,471,385,382,221,0,234,
244,0,235,236,
542,541,540,471,385,382,0,
540,385,382,0,
542,541,540,471,385,382,0,507,512,
61,0,
537,61,43,41,5,0,464,
543,542,541,540,520,519,473,472,471,385,382,221,191,61,57,0,517,518,
61,0,
61,0,
542,541,540,471,385,382,0,507,512,
61,0,
537,61,43,41,0,38,39,46,
537,41,0,
545,544,543,542,541,540,539,538,473,472,471,385,382,253,252,244,229,221,191,
  63,61,0,64,66,247,250,451,452,453,
545,544,543,542,541,540,539,538,473,472,471,385,382,244,229,221,191,63,61,0,
  66,
545,544,543,542,541,540,539,538,473,472,471,385,382,244,229,221,191,137,136,
  135,134,63,0,64,66,131,133,
48,0,47,
48,0,47,
48,0,47,
540,537,385,382,41,0,38,39,46,
48,0,47,
48,0,47,
537,53,43,41,5,0,38,39,46,
537,53,43,41,5,0,38,39,46,
537,53,43,41,5,0,38,39,46,
537,405,404,403,402,401,400,399,41,0,38,39,46,
537,244,53,43,41,5,0,384,
537,244,53,43,41,5,0,384,
427,426,425,0,424,
542,541,540,471,385,382,0,507,512,
61,0,
542,541,540,471,385,382,0,507,512,
43,0,42,
455,0,454,
537,61,41,0,46,450,
552,551,550,549,548,547,546,545,544,543,542,541,540,539,538,537,473,472,471,
  385,382,362,335,290,289,244,229,221,191,85,63,61,59,57,55,53,50,48,43,
  41,37,5,0,80,81,82,83,
545,544,543,542,541,540,539,538,473,472,471,385,382,244,229,221,191,63,0,
543,542,541,540,537,473,472,471,385,382,289,221,41,0,38,39,46,
540,537,385,382,41,0,38,39,46,
545,544,543,542,541,540,539,538,537,473,472,471,385,382,244,229,221,191,166,
  63,41,0,38,39,46,
540,385,382,0,99,138,
545,544,543,542,541,540,539,538,537,473,472,471,385,382,244,229,221,191,63,
  41,0,38,39,46,
542,541,540,537,471,385,382,295,41,0,38,39,46,
405,404,403,402,401,400,399,0,394,398,
540,537,385,382,53,43,41,5,0,383,
542,541,540,471,385,382,0,507,512,
542,541,540,471,385,382,0,507,512,
545,544,543,542,541,540,539,538,537,481,480,473,472,471,385,382,244,229,221,
  191,63,41,0,38,39,46,
253,252,0,247,250,
61,0,
61,0,
543,542,541,540,473,472,471,385,382,289,221,0,139,150,286,
540,385,382,0,155,
545,544,543,542,541,540,539,538,473,472,471,385,382,244,229,221,191,166,63,
  0,64,66,165,
545,544,543,542,541,540,539,538,473,472,471,385,382,244,229,221,191,63,0,64,
  66,
542,541,540,471,385,382,295,0,162,167,293,294,297,
537,61,41,0,38,39,46,
540,385,382,0,
542,541,540,471,385,382,0,507,512,
542,541,540,471,385,382,0,507,512,
545,544,543,542,541,540,539,538,481,480,473,472,471,385,382,244,229,221,191,
  63,0,64,66,479,
537,53,43,41,5,0,38,39,46,
537,53,43,41,5,0,38,39,46,
537,53,43,41,5,0,38,39,46,
537,53,43,41,5,0,38,39,46,
537,53,43,41,5,0,38,39,46,
61,0,
540,385,382,0,
542,541,540,471,385,382,0,507,512,
542,541,540,471,385,382,0,507,512,
542,541,540,471,385,382,0,507,512,
542,541,540,471,385,382,0,507,512,
542,541,540,471,385,382,0,507,512,
542,541,540,471,385,382,0,507,512,
542,541,540,471,385,382,0,507,512,
542,541,540,471,385,382,0,507,512,
542,541,540,471,385,382,0,507,512,
542,541,540,471,385,382,0,507,512,
542,541,540,471,385,382,0,507,512,
542,541,540,471,385,382,0,507,512,
542,541,540,471,385,382,0,507,512,
542,541,540,471,385,382,0,507,512,
542,541,540,471,385,382,0,507,512,
542,541,540,471,385,382,0,507,512,
542,541,540,471,385,382,0,507,512,
542,541,540,471,385,382,0,507,512,
542,541,540,471,385,382,0,507,512,
542,541,540,471,385,382,0,507,512,
542,541,540,471,385,382,0,507,512,
542,541,540,471,385,382,0,507,512,
542,541,540,471,385,382,0,507,512,
542,541,540,471,385,382,0,507,512,
542,541,540,471,385,382,0,507,512,
542,541,540,471,385,382,0,507,512,
542,541,540,471,385,382,0,507,512,
};
static unsigned const char ag_astt[7582] = {
  1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,2,2,2,
  2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,0,1,1,1,1,1,1,1,2,1,2,1,2,1,2,2,2,1,
  1,1,2,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,4,1,1,1,1,1,1,1,1,1,1,1,1,7,3,
  1,2,2,2,1,2,1,2,2,7,3,1,1,1,1,2,2,2,7,2,1,1,1,1,1,2,2,2,7,2,1,1,1,1,1,1,1,
  8,7,1,1,1,1,2,7,1,1,3,7,3,7,3,7,3,7,3,7,1,7,1,3,7,3,7,1,7,1,1,7,1,1,7,1,1,
  7,1,1,7,1,1,7,1,1,7,1,1,7,1,1,7,1,1,7,1,1,7,1,1,7,1,1,9,1,1,1,1,1,1,1,9,9,
  7,1,1,1,1,1,1,1,5,1,1,1,5,1,1,5,5,5,5,5,7,1,1,3,1,1,1,1,1,1,7,1,4,4,4,2,4,
  4,7,1,2,1,1,1,1,1,1,1,1,1,1,1,1,7,1,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,2,1,1,7,1,1,1,2,1,4,4,4,4,4,2,4,4,7,1,1,1,4,4,4,4,4,2,4,4,
  7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,2,1,
  1,9,9,4,2,7,1,4,4,4,4,5,4,4,4,4,4,4,4,4,4,4,4,4,4,4,5,5,7,1,1,1,1,1,1,1,1,
  1,2,2,2,2,1,1,1,2,1,7,1,1,2,1,1,2,1,2,2,2,1,1,1,1,1,1,2,7,3,1,1,5,5,5,5,5,
  5,5,5,5,5,5,1,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
  5,5,5,5,5,5,5,5,1,5,7,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,1,2,1,1,1,1,1,1,1,
  7,1,2,2,2,1,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,1,1,1,2,2,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,1,1,1,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,2,7,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,7,1,2,1,
  1,2,1,1,1,1,1,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,1,1,1,1,2,1,1,2,7,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,2,1,2,2,2,2,2,2,2,1,1,7,1,2,1,1,1,1,
  1,2,2,2,2,2,2,2,1,1,7,1,1,1,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,1,1,1,
  1,2,1,1,2,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,7,1,1,1,1,1,1,1,1,1,
  1,5,1,1,1,5,1,1,5,5,5,5,5,7,1,1,3,9,9,9,9,9,9,9,9,9,9,9,5,3,9,1,1,1,9,9,7,
  1,1,5,1,1,5,1,1,5,5,5,5,5,7,1,1,3,8,8,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,8,8,
  8,8,8,8,8,8,8,8,8,1,8,7,1,1,1,2,5,1,1,1,1,1,1,1,1,1,1,1,1,1,1,4,1,1,1,1,1,
  1,1,1,7,1,1,1,1,2,4,1,1,1,1,1,1,1,1,1,1,1,1,7,1,2,2,2,9,9,9,9,9,9,9,9,9,9,
  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,4,2,4,1,2,7,1,8,8,8,8,8,8,8,8,8,1,8,8,8,8,
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,1,8,7,1,1,1,2,4,1,4,4,4,4,2,4,4,7,1,
  2,1,4,4,4,4,2,4,4,7,1,2,1,9,9,9,9,9,9,9,9,9,9,9,9,9,4,2,7,1,8,8,8,8,8,8,8,
  8,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,1,8,5,7,1,
  1,1,2,7,1,1,1,1,1,7,2,4,1,3,7,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,
  8,1,7,1,1,1,1,8,1,7,1,1,1,1,8,1,5,7,1,1,1,3,7,1,1,7,1,1,1,1,7,1,1,3,7,1,1,
  7,1,1,1,1,7,1,1,3,7,5,5,5,5,5,5,5,5,1,5,5,5,5,5,5,5,5,5,5,5,1,5,7,1,1,2,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,8,1,1,1,1,1,1,
  1,1,1,1,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,4,1,5,1,5,7,2,2,1,8,
  8,8,8,1,8,8,8,8,8,8,8,8,8,8,8,8,8,8,1,7,1,1,1,2,7,1,1,8,5,1,5,7,1,1,1,2,5,
  1,3,7,1,8,1,7,1,1,1,1,1,7,1,1,2,5,1,3,7,9,9,9,4,1,1,5,7,2,2,1,3,7,9,9,9,9,
  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,4,2,4,1,1,1,5,7,2,2,1,
  3,7,2,7,1,2,2,2,7,1,1,1,1,1,2,2,2,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,2,
  2,2,2,1,2,1,2,2,7,1,1,1,1,2,4,1,2,4,1,1,1,8,5,1,5,7,1,1,1,2,5,1,3,7,2,2,2,
  4,1,8,1,7,1,1,1,1,1,7,1,1,1,1,7,1,1,1,1,5,7,2,2,1,3,7,9,9,9,4,1,1,5,7,2,2,
  1,3,7,2,4,1,1,1,8,1,5,7,1,1,1,3,7,9,9,9,4,1,1,7,1,1,1,1,5,7,2,2,1,3,7,1,1,
  5,1,1,5,1,1,5,5,5,5,5,7,1,1,3,9,9,9,9,9,9,5,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,7,2,2,2,2,2,2,2,1,8,8,8,8,8,8,8,8,8,1,8,8,8,
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,1,8,7,1,1,1,9,9,9,9,9,9,9,9,9,9,9,9,9,4,1,
  1,1,1,1,1,5,5,5,5,5,5,5,5,1,5,7,1,1,3,1,1,1,5,1,1,1,5,5,5,5,5,5,7,3,3,1,1,
  5,1,5,2,7,1,1,1,7,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,1,1,7,2,2,
  1,1,2,1,1,1,1,1,1,1,1,1,1,1,7,1,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,2,2,2,2,2,2,2,2,2,2,2,1,8,8,8,8,8,8,8,8,8,
  1,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,1,8,7,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,2,1,1,1,4,8,8,8,8,8,8,8,8,8,8,1,8,8,8,
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,1,8,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,4,2,2,2,7,3,1,2,2,2,7,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  5,7,2,1,1,2,2,7,1,2,7,1,2,7,1,2,7,1,2,7,1,2,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,7,1,2,2,2,2,2,1,1,1,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,1,1,
  1,2,2,2,2,2,2,2,2,2,2,1,9,9,5,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,7,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
  9,9,9,9,9,9,9,9,9,9,9,9,9,9,1,9,9,9,9,9,9,9,9,9,9,9,5,3,3,2,7,5,5,5,5,5,5,
  5,5,1,5,5,5,5,5,5,5,5,5,5,5,1,5,7,1,1,2,1,1,1,1,1,1,1,1,1,2,2,2,2,1,1,1,2,
  1,7,1,2,1,1,2,1,2,2,2,1,1,1,8,8,8,8,1,8,8,8,8,8,8,8,8,8,8,8,8,8,8,1,7,1,1,
  1,2,4,1,1,8,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,8,8,8,8,1,7,1,1,1,2,7,1,1,1,1,
  1,1,1,1,1,1,1,1,7,1,1,2,2,8,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,8,8,8,1,7,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,2,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,7,1,1,2,3,1,1,8,5,5,1,5,7,1,1,1,1,8,5,5,1,5,7,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,1,1,2,3,1,1,8,5,5,1,5,7,1,1,1,1,8,5,5,
  1,5,7,1,1,1,1,8,5,5,1,5,7,1,1,1,1,8,5,5,1,5,7,1,1,1,8,8,8,8,8,8,8,8,1,8,8,
  8,8,8,8,8,8,8,8,8,8,8,1,7,1,1,1,2,4,1,2,4,1,1,8,8,8,8,8,8,8,8,1,8,8,8,8,8,
  8,8,8,8,8,8,8,8,8,8,8,8,1,7,1,1,1,2,7,1,2,2,2,2,2,2,2,2,2,2,2,2,7,1,1,1,1,
  7,2,1,8,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,8,8,8,1,7,1,1,1,2,4,1,2,4,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,1,7,2,1,2,1,1,1,1,1,1,1,1,1,5,1,1,
  1,5,1,1,5,1,1,1,1,1,1,1,1,5,1,5,5,5,5,1,5,7,1,1,2,2,4,1,2,1,8,1,7,1,1,1,1,
  8,1,7,1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  2,1,1,1,1,1,1,1,1,1,1,1,7,2,2,2,2,2,2,2,1,1,1,1,1,1,1,5,1,1,1,1,1,1,7,3,1,
  5,3,1,1,1,5,1,1,1,5,5,5,5,5,5,7,3,3,1,1,1,1,7,3,1,9,9,9,4,9,9,9,9,9,9,9,9,
  9,9,9,9,9,9,9,9,9,9,9,9,9,9,5,3,1,8,1,7,1,1,1,1,8,1,7,1,1,1,8,8,8,8,8,8,8,
  8,5,8,8,8,5,8,8,5,8,8,8,8,8,5,5,5,2,5,5,7,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,
  1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,2,2,2,2,2,2,2,2,2,2,2,1,9,9,9,9,9,9,9,
  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,4,1,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,7,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,7,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,5,7,1,1,2,1,1,5,7,2,2,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,5,3,5,5,5,5,5,5,5,5,1,5,
  5,5,5,5,5,5,5,5,5,5,5,8,1,7,1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,
  1,8,8,8,1,8,8,8,1,7,1,1,1,1,8,8,8,8,8,1,7,1,1,1,1,8,1,7,1,1,1,2,4,1,1,8,1,
  7,1,1,1,2,2,7,1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,
  1,9,9,9,9,9,9,9,9,1,9,9,9,9,9,9,9,9,9,9,1,1,7,1,1,8,1,7,1,1,1,1,8,1,7,1,1,
  1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,5,1,5,7,2,2,1,2,4,1,2,4,1,1,
  8,1,7,1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,9,9,9,
  9,9,9,9,9,1,9,9,9,9,9,9,9,9,9,9,1,1,7,1,1,5,1,5,7,2,2,1,2,4,1,1,8,1,7,1,1,
  1,1,1,1,1,1,1,1,1,1,2,2,2,2,1,1,1,2,1,7,1,2,1,1,2,1,2,2,2,1,1,1,8,8,8,8,8,
  8,8,8,1,8,8,8,8,8,8,8,8,8,8,1,7,1,1,1,2,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,2,7,1,1,1,1,1,2,8,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,8,8,1,7,1,1,1,1,
  5,5,1,8,5,7,1,1,1,4,4,4,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,7,2,1,2,
  1,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,4,1,5,5,1,5,7,2,
  2,1,2,4,4,4,7,1,2,1,2,4,4,4,7,1,2,1,2,4,4,4,7,1,2,1,2,4,4,4,7,1,2,1,2,4,4,
  4,7,1,2,1,2,4,4,4,7,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,1,1,
  2,3,1,1,1,8,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,8,8,8,8,8,1,7,1,1,1,2,4,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,1,1,1,1,2,1,1,2,7,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,2,8,1,8,8,1,7,1,1,1,1,1,7,1,1,2,2,2,7,2,7,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,7,1,1,2,1,2,8,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,8,8,8,1,7,1,1,
  1,2,4,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,5,3,8,8,8,8,8,8,8,
  8,8,1,8,8,8,8,8,8,8,8,8,8,8,8,8,8,1,8,7,1,1,1,2,7,1,2,7,1,2,7,1,2,7,1,3,3,
  3,3,3,3,5,1,1,1,5,1,7,2,7,1,2,7,1,1,1,1,1,1,1,1,1,5,1,1,1,5,1,1,5,1,1,1,1,
  1,5,5,5,5,5,7,2,1,2,2,2,2,5,2,7,1,2,7,1,2,7,1,2,7,1,2,7,1,1,1,1,1,1,1,1,1,
  1,5,1,1,1,5,1,1,5,5,1,1,1,1,1,1,1,5,1,1,5,1,5,5,1,5,7,2,1,1,2,9,9,9,9,9,9,
  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,4,9,9,9,9,9,9,9,9,9,9,9,9,
  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,5,3,8,8,8,8,8,8,
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,2,7,1,1,1,7,2,1,7,2,2,2,1,7,2,2,1,1,1,1,1,1,7,
  2,1,1,1,1,1,1,1,7,2,2,3,1,2,2,2,2,7,1,2,8,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,
  8,8,1,7,1,1,1,2,7,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,7,2,1,2,1,7,1,5,
  2,5,5,5,7,2,2,7,1,2,7,1,2,7,1,2,7,1,9,1,9,7,8,8,8,8,8,8,8,8,1,8,8,8,8,8,8,
  8,8,8,8,8,1,7,1,1,1,2,7,1,2,7,1,2,7,1,2,7,1,2,7,1,8,8,8,8,8,8,8,8,1,8,8,8,
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,1,7,1,1,1,2,7,1,2,7,1,2,7,1,2,7,1,2,
  7,2,1,1,1,1,1,1,1,1,1,9,1,1,1,1,1,1,1,1,1,1,1,9,7,1,2,8,8,8,8,8,8,8,8,1,8,
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,1,7,1,1,1,2,7,1,8,1,7,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,7,1,1,3,8,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,8,8,1,
  7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,2,1,1,2,4,1,2,4,1,1,5,8,5,1,
  5,7,1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,5,5,1,5,7,2,2,1,1,5,
  5,1,5,7,2,2,1,1,5,5,1,5,7,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  7,1,1,2,3,1,1,1,1,1,1,7,1,1,1,1,1,7,2,1,1,1,7,1,1,8,1,7,1,1,1,1,5,1,5,7,2,
  2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,1,1,2,1,2,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,2,8,8,8,8,1,8,8,8,8,8,8,8,1,7,1,1,1,8,8,8,8,
  8,8,8,8,8,1,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,1,8,7,1,1,1,8,8,8,8,8,1,8,8,8,
  8,8,8,1,7,1,1,1,8,8,1,8,8,8,8,1,7,1,1,1,3,3,3,5,1,1,1,7,1,8,8,8,8,1,8,8,8,
  8,8,8,8,1,7,1,1,1,8,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,8,8,8,1,7,1,1,1,8,8,8,
  8,8,8,8,8,1,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,1,7,1,1,1,8,8,8,8,1,8,8,8,8,8,8,
  8,1,7,1,1,1,8,1,8,8,1,7,1,1,1,8,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,8,8,8,8,1,
  7,1,1,1,8,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,8,8,8,8,8,8,1,7,1,1,1,9,9,9,9,9,
  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,5,3,1,1,1,1,1,1,1,1,2,2,1,1,1,1,1,
  1,1,1,1,1,7,2,1,1,1,1,1,1,1,1,8,7,1,1,1,1,1,1,1,1,1,1,1,4,1,1,8,8,8,8,8,8,
  8,8,1,8,8,8,8,8,8,8,8,8,8,8,1,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  7,1,1,3,1,2,1,4,1,1,2,1,7,1,1,1,7,1,1,1,8,1,7,1,1,1,5,5,5,5,5,5,5,5,1,5,5,
  5,5,5,5,5,5,5,5,5,5,8,1,7,1,1,1,1,8,8,1,7,1,1,1,1,8,1,7,1,1,1,8,8,8,8,8,8,
  8,8,1,8,8,8,8,8,8,8,8,8,8,8,1,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,7,1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,1,2,2,2,2,
  2,1,1,2,2,2,2,2,2,2,2,1,8,1,7,1,1,1,1,8,8,1,7,1,1,1,1,7,2,1,7,2,1,5,1,7,2,
  2,1,1,9,9,9,9,9,9,9,9,1,9,9,9,9,9,9,9,9,9,9,8,1,7,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,1,1,2,2,2,2,2,2,2,2,2,2,1,2,7,1,5,8,
  5,1,5,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,1,1,3,1,8,1,7,1,1,1,8,
  1,8,8,1,7,1,1,1,8,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,1,7,
  1,1,1,2,4,1,2,7,1,2,7,1,2,7,1,1,1,7,1,1,1,1,1,7,2,2,2,7,2,7,1,2,7,1,1,1,1,
  1,1,1,1,1,1,1,1,7,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,7,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,2,1,1,1,1,1,1,1,1,7,2,1,1,7,1,1,1,
  1,1,1,1,1,1,1,1,7,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,7,2,1,2,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,1,7,2,1,2,1,1,1,1,1,1,1,1,1,1,1,7,
  2,2,2,1,1,1,7,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,1,1,1,1,1,7,2,1,2,2,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,1,7,2,1,2,5,2,5,5,5,7,2,9,9,9,9,9,9,5,3,
  3,3,2,7,1,1,1,1,1,1,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,1,1,1,2,
  4,2,1,7,9,9,9,4,2,7,1,2,7,1,3,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,2,7,
  1,1,2,2,7,1,7,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,8,1,1,1,1,1,1,1,1,1,1,7,1,1,
  1,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,4,1,7,2,2,1,7,2,1,7,2,2,1,7,2,2,1,
  7,2,2,1,5,1,5,7,2,2,1,2,7,1,2,2,7,1,1,1,1,1,1,7,1,1,1,7,1,7,1,5,1,5,7,2,2,
  1,2,4,1,2,2,7,1,1,1,1,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,7,1,1,2,2,2,2,2,2,2,2,2,2,2,2,1,2,8,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,
  8,8,8,8,1,7,1,1,1,8,1,8,8,8,1,7,1,1,1,1,8,8,1,7,1,1,1,1,8,8,1,7,1,1,1,1,1,
  1,7,2,1,1,1,1,4,1,1,1,7,1,8,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,8,8,1,7,1,1,1,
  1,1,1,1,5,1,1,1,1,1,5,5,1,1,5,5,5,5,7,1,1,1,2,1,1,1,1,5,1,1,1,1,1,5,5,1,1,
  5,5,5,5,7,1,1,1,2,5,5,1,5,5,5,5,7,1,1,1,2,1,1,1,1,1,1,7,9,9,9,9,9,9,9,9,9,
  9,9,9,9,9,9,9,1,9,9,9,9,9,9,9,9,9,4,3,3,9,9,9,9,9,9,9,9,9,9,9,4,3,9,9,9,9,
  9,9,4,1,1,1,7,3,1,1,1,4,1,1,1,1,1,1,4,1,1,1,5,5,1,5,7,2,2,1,1,5,5,1,5,7,2,
  2,1,2,7,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,7,2,1,2,1,1,1,1,1,1,1,1,1,2,
  2,2,2,1,1,1,2,1,7,1,1,2,1,1,2,1,2,2,2,1,1,1,1,1,1,1,1,1,1,1,2,2,1,1,1,1,1,
  1,1,1,1,1,7,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,8,1,1,1,1,1,1,1,1,1,1,7,1,1,1,1,9,9,9,9,
  9,9,9,9,9,9,9,9,9,9,9,9,9,9,4,2,7,1,1,1,1,1,1,1,8,1,7,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,2,2,1,1,1,1,1,1,1,1,1,1,7,2,1,1,2,9,9,9,9,9,9,4,2,7,8,8,8,8,8,8,8,
  8,1,8,8,8,8,8,8,8,8,8,8,8,8,8,1,7,1,1,8,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,8,
  8,8,1,7,1,1,1,8,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,8,8,8,8,8,8,1,7,1,1,1,1,5,
  5,1,5,7,2,2,1,1,5,5,5,1,5,7,3,3,1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,
  1,4,2,4,4,7,1,2,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,5,5,1,5,7,3,3,1,1,5,5,1,5,7,
  3,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,2,7,1,1,3,2,
  2,2,7,2,1,1,7,2,2,2,2,4,2,2,2,7,1,1,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,7,2,1,9,9,9,9,9,9,9,9,9,9,9,5,3,3,1,1,1,1,1,1,1,1,1,1,7,3,1,5,3,3,3,3,
  3,3,3,3,7,2,2,2,4,1,1,1,1,1,1,4,1,1,3,7,5,2,5,5,5,7,2,9,9,9,9,1,1,9,9,9,9,
  9,9,9,8,9,7,1,1,2,7,2,7,1,1,1,1,1,1,7,1,1,2,7,1,2,8,1,7,1,1,1,9,9,5,1,1,1,
  1,1,1,1,1,1,1,1,1,1,2,2,1,1,1,1,1,2,7,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,1,7,2,1,2,2,
  2,7,1,2,7,1,2,7,1,8,1,8,8,1,7,1,1,1,2,7,1,2,7,1,1,5,5,1,5,7,2,2,1,1,5,5,1,
  5,7,2,2,1,1,5,5,1,5,7,2,2,1,1,8,8,8,8,8,8,8,1,7,1,1,1,5,1,5,5,5,5,7,2,5,1,
  5,5,5,5,7,2,2,2,2,7,2,1,1,1,1,1,1,4,1,1,2,7,1,1,1,1,1,1,7,1,1,2,7,1,1,5,1,
  1,8,1,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  8,1,1,1,1,1,1,1,1,1,1,7,1,1,1,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,5,8,8,
  8,8,1,8,8,8,8,8,8,8,1,7,1,1,1,8,1,8,8,1,7,1,1,1,8,8,8,8,8,8,8,8,1,8,8,8,8,
  8,8,8,8,8,8,8,1,7,1,1,1,1,1,1,7,1,2,8,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,8,8,
  1,7,1,1,1,8,8,8,1,8,8,8,8,1,7,1,1,1,2,2,2,2,2,2,2,7,1,1,1,5,1,1,5,5,5,5,7,
  3,1,1,1,1,1,1,4,1,1,1,1,1,1,1,1,7,1,1,8,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,8,
  8,8,8,1,7,1,1,1,2,2,7,3,1,2,7,3,7,1,1,1,1,1,1,1,1,1,1,1,7,1,2,2,1,1,1,7,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,7,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,7,1,1,1,1,1,1,1,1,1,7,1,2,1,1,1,1,8,1,7,1,1,1,1,1,1,5,2,2,2,2,2,2,
  4,2,2,1,1,1,1,1,1,7,1,1,1,1,1,1,1,1,1,1,2,2,1,1,1,1,1,1,1,1,1,1,7,2,1,2,1,
  5,5,1,5,7,2,2,1,1,5,5,1,5,7,2,2,1,1,5,5,1,5,7,2,2,1,1,5,5,1,5,7,2,2,1,1,5,
  5,1,5,7,2,2,1,2,7,3,3,3,5,1,1,1,1,1,1,7,1,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,1,
  7,1,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,1,7,
  1,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,1,7,1,
  1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,1,7,1,1,
  1,1,1,1,1,1,7,1,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,1,7,1,1,1,
  1,1,1,1,1,7,1,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,1,7,1,1,1,1,
  1,1,1,1,7,1,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,1,7,1,1,3,3,3,3,3,3,7,3,3
};
static const unsigned short ag_pstt[] = {
1,1,1,1,1,1,1,1,1,1,1,1,1,401,400,399,398,397,396,395,394,393,392,391,390,
  389,264,261,257,256,1,1,1,1,178,102,69,68,67,66,65,1,13,16,17,18,19,20,
  21,22,23,24,25,26,27,0,0,15,14,12,11,10,9,8,70,1,402,7,176,2,252,253,
  254,3,5,4,387,6,
40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,41,
29,28,29,29,29,29,29,29,29,28,30,28,2,177,31,180,181,182,28,192,29,
299,284,3,255,35,34,33,32,
299,284,260,4,263,36,35,34,33,32,
299,284,260,5,259,37,35,34,33,32,
38,40,39,6,39,39,41,38,
24,7,42,42,
20,8,
19,9,
18,10,
17,11,
16,12,
43,13,44,
14,14,
13,15,
43,16,45,
43,17,46,
43,18,47,
43,19,48,
43,20,49,
43,21,50,
43,22,51,
43,23,52,
43,24,53,
43,25,54,
43,26,55,
43,27,56,
57,190,57,57,57,57,57,57,57,190,190,28,57,
58,58,58,58,58,58,196,58,58,58,196,58,58,196,196,196,196,196,29,58,58,198,
59,60,60,60,59,59,30,59,
202,202,202,33,202,202,31,61,179,62,
63,63,63,63,63,63,63,63,63,64,63,32,65,303,298,302,
67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,31,
  67,67,33,66,69,68,287,67,
330,330,330,330,330,33,330,330,34,70,72,71,
330,330,330,330,330,33,330,330,35,70,73,71,
74,74,74,74,74,74,74,74,74,74,74,74,74,36,262,75,74,
74,74,74,74,74,74,74,74,74,74,74,74,74,37,258,75,74,
28,28,29,
32,39,76,
24,24,24,24,27,24,24,24,24,24,24,24,24,24,24,24,24,24,24,27,27,40,
74,74,74,74,74,74,74,74,74,264,261,257,256,74,74,74,178,74,41,77,77,407,75,
  74,406,2,403,404,405,3,5,4,
78,78,78,24,42,99,79,79,
21,21,21,21,21,21,21,21,21,21,21,38,21,21,21,21,21,21,21,21,21,21,21,21,21,
  21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,
  21,38,21,43,23,23,38,
74,74,74,74,74,74,74,74,74,74,74,74,74,44,80,164,75,74,
82,84,85,83,81,45,87,635,633,630,86,634,636,637,
1,1,1,1,1,1,1,1,1,1,1,89,1,1,1,1,1,1,1,46,90,88,1,572,572,
1,1,1,1,1,1,1,1,1,1,1,92,1,1,1,1,1,1,1,47,93,91,1,502,502,
94,94,94,94,94,94,94,94,94,94,94,94,94,94,94,94,94,94,95,36,48,102,98,97,99,
  100,96,101,626,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,119,1,49,106,118,1,105,112,104,103,
107,107,107,50,109,107,108,
110,110,110,110,110,110,110,110,110,110,110,110,110,110,110,110,110,110,110,
  110,110,110,110,110,110,110,110,110,110,110,51,113,111,110,112,
94,94,94,94,94,94,94,94,94,94,94,94,94,430,264,261,257,256,94,94,94,94,178,
  94,95,36,52,123,98,97,114,119,117,118,116,115,120,120,120,120,120,120,
  121,96,123,430,122,433,
124,474,473,472,471,470,469,468,124,124,53,129,466,128,127,125,126,
130,474,473,472,471,470,469,468,130,130,54,132,130,131,461,462,125,
94,94,94,94,94,94,94,94,94,94,94,94,94,264,261,257,256,94,94,94,94,178,94,
  95,36,55,135,98,97,114,119,117,118,116,115,133,133,133,133,133,133,408,
  134,96,
136,136,136,56,139,138,137,136,
58,58,58,58,58,58,196,58,58,58,196,58,58,196,196,196,196,196,57,58,58,199,
195,195,195,195,195,195,195,195,195,195,195,197,195,
190,140,140,140,190,190,59,
141,141,186,141,141,186,141,141,186,186,186,186,186,60,141,141,188,
142,142,142,142,142,142,142,142,142,38,142,142,142,142,142,142,142,142,142,
  142,142,142,142,142,142,142,142,142,142,38,142,61,142,142,38,
33,203,143,
144,144,144,144,144,144,144,144,144,144,144,144,144,304,144,
145,145,145,145,145,145,147,64,150,149,148,146,
32,300,151,
63,63,63,63,63,63,63,63,63,64,63,66,65,303,283,302,
289,289,289,289,289,289,289,289,289,289,289,289,289,289,289,289,289,289,289,
  289,289,289,289,289,289,289,290,
32,285,152,
31,69,153,
154,154,154,154,154,154,154,154,154,38,154,154,154,154,154,154,154,154,154,
  154,154,154,154,154,154,154,154,154,154,154,154,154,154,38,154,70,154,
  154,38,
33,331,155,
373,373,373,373,34,373,373,72,156,266,157,
373,373,373,373,34,373,373,73,156,265,157,
167,167,167,167,167,167,167,167,167,167,167,167,167,168,
32,75,158,
159,159,159,159,159,159,159,159,159,159,159,159,159,159,159,38,159,159,159,
  159,159,159,159,159,159,159,159,159,159,159,159,159,159,159,159,159,159,
  159,159,38,159,21,76,159,159,38,
24,77,160,160,
161,161,161,78,
24,101,162,
15,80,
38,163,38,81,163,163,38,
38,164,38,82,164,164,38,
38,165,38,83,165,165,38,
38,166,38,84,166,166,38,
38,167,38,85,167,167,38,
38,168,38,631,86,168,168,38,
12,87,
38,38,88,169,38,
38,38,89,170,38,
11,90,
38,38,91,171,38,
38,38,92,172,38,
10,93,
420,420,420,420,420,420,420,420,173,420,420,420,420,420,420,420,420,420,420,
  420,173,420,94,173,173,423,
175,175,175,175,175,175,175,175,175,175,175,175,175,175,175,175,175,175,175,
  175,175,175,175,175,175,175,175,175,175,174,175,176,175,175,175,175,175,
  175,175,175,175,175,95,175,175,175,176,
177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,416,
38,21,38,21,97,415,415,38,
178,178,178,178,38,178,178,178,178,178,178,178,178,178,178,178,178,178,178,
  38,98,178,178,38,
36,99,179,
38,180,21,38,21,100,180,180,38,
26,625,181,
9,102,
38,182,38,103,182,182,38,
38,38,104,183,38,
26,111,184,
8,106,
74,74,74,77,
38,38,21,108,76,76,38,
7,109,
43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,
  43,43,43,43,43,44,
31,79,185,
38,38,21,112,78,78,38,
6,113,
32,114,186,
299,284,260,115,187,189,188,33,32,
299,284,260,116,190,192,191,33,32,
29,28,29,29,29,29,29,29,29,28,30,28,117,274,180,181,182,28,192,29,
299,284,118,194,193,33,32,
36,169,179,
33,437,195,196,
38,197,21,38,21,121,197,197,38,
26,431,198,
5,123,
477,477,477,476,
38,199,38,125,199,199,38,
38,38,126,200,38,
38,38,127,201,38,
38,38,21,128,464,464,38,
4,129,
74,74,74,463,
38,38,21,131,457,457,38,
3,132,
33,411,202,203,
38,204,38,21,134,204,204,38,
2,135,
74,74,74,75,
38,38,137,205,38,
38,38,21,138,71,71,38,
1,139,
141,141,186,141,141,186,141,141,186,186,186,186,186,140,141,141,191,
185,185,185,185,185,185,187,185,
206,206,206,206,206,206,206,206,206,206,206,206,206,206,206,206,206,251,206,
  208,206,209,206,210,211,206,206,206,206,142,204,206,207,208,209,210,211,
  207,
212,212,212,212,212,212,212,212,212,38,212,212,212,212,212,212,212,212,212,
  212,212,212,212,212,212,212,212,212,212,38,212,143,212,212,38,
306,306,306,306,306,306,306,306,306,306,306,306,306,307,
213,213,213,213,213,213,326,
323,323,323,323,323,323,323,214,323,146,215,215,325,
145,145,145,316,145,145,145,316,316,316,316,316,316,147,319,319,146,
216,315,
217,313,
308,150,
218,218,218,151,218,301,
219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,
  219,295,219,219,152,286,291,219,219,291,
63,63,63,63,63,63,63,63,63,64,63,153,65,303,282,302,
206,206,206,206,206,206,206,206,206,206,206,206,206,206,206,363,220,222,223,
  226,227,206,206,206,206,206,206,221,224,225,206,206,206,154,343,332,334,
  335,336,337,338,339,340,341,342,207,
228,228,228,228,228,228,228,228,228,38,228,228,228,228,228,228,228,228,228,
  228,228,228,228,228,228,228,228,228,228,228,228,228,228,38,228,155,228,
  228,38,
229,229,229,229,229,229,229,229,229,229,229,229,229,229,229,229,229,229,229,
  229,229,229,229,229,156,375,230,229,
231,374,
232,232,232,232,232,232,232,232,232,232,38,232,232,232,232,232,232,232,232,
  232,232,232,232,232,232,232,232,232,232,232,38,232,158,232,232,38,
233,233,233,233,233,233,233,233,233,233,233,233,233,233,233,233,233,233,233,
  233,233,233,233,233,233,233,233,233,233,233,233,233,233,233,233,233,233,
  233,233,500,501,
102,24,160,99,234,
110,110,110,161,
235,235,235,235,235,235,235,235,235,235,235,235,235,235,235,235,235,235,235,
  235,235,235,235,235,235,235,235,235,235,235,235,235,235,235,235,235,235,
  235,235,107,162,100,235,235,109,
30,163,236,
30,164,237,
30,165,238,
30,166,239,
30,167,240,
25,168,241,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,169,242,1,243,243,
250,250,250,250,250,250,250,250,248,249,252,85,83,246,255,247,250,250,250,
  244,251,253,254,250,250,250,250,250,250,250,170,250,579,580,583,585,590,
  245,257,256,578,581,582,584,586,587,588,589,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,171,242,1,258,258,
264,264,264,264,264,264,264,264,261,262,259,264,264,264,244,260,251,253,263,
  254,264,264,264,264,264,264,264,172,264,266,265,508,509,510,511,512,513,
  514,515,516,517,245,
419,419,421,419,
61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,
  61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,174,
56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,
  56,56,56,56,174,56,56,56,56,56,56,56,56,56,56,56,58,56,56,
59,176,
420,420,420,420,420,420,420,420,173,420,420,420,420,420,420,420,420,420,420,
  420,173,420,177,173,173,422,
74,74,74,74,74,74,74,74,74,264,261,257,256,74,74,74,178,74,178,267,407,75,
  74,406,2,403,404,405,3,5,4,
268,268,268,268,38,268,268,268,268,268,268,268,268,268,268,268,268,268,268,
  38,179,268,268,38,
33,628,269,270,
271,271,271,271,271,271,271,271,38,271,271,271,271,271,271,271,271,271,271,
  271,271,38,181,271,271,38,
35,182,272,
63,63,63,63,63,63,63,63,63,64,63,183,274,273,303,302,
275,275,275,275,275,275,275,275,38,275,275,275,275,275,275,275,275,275,275,
  275,38,184,275,275,38,
110,110,110,110,110,110,110,110,110,110,110,110,110,110,110,110,110,110,110,
  110,110,110,110,110,110,110,110,110,110,110,185,80,110,
276,276,276,276,276,276,276,276,276,276,276,276,276,276,276,276,276,276,276,
  276,276,276,276,276,276,276,276,186,277,276,
94,94,94,94,94,94,94,94,94,94,94,94,94,94,94,94,94,94,95,187,97,114,169,280,
  96,
38,278,21,21,38,21,188,278,278,38,
38,279,21,21,38,21,189,279,279,38,
94,94,94,94,94,94,94,94,94,94,94,94,94,94,94,94,94,94,95,190,97,114,169,277,
  96,
38,280,21,21,38,21,191,280,280,38,
38,281,21,21,38,21,192,281,281,38,
38,282,21,21,38,21,193,282,282,38,
38,283,21,21,38,21,194,283,283,38,
284,284,284,284,284,284,284,284,38,284,284,284,284,284,284,284,284,284,284,
  284,284,284,38,195,284,284,38,
33,438,285,
33,435,195,286,
287,287,287,287,287,287,287,287,38,287,287,287,287,287,287,287,287,287,287,
  287,287,287,287,287,287,287,38,198,287,287,38,
25,199,288,
489,488,487,486,485,484,483,482,481,480,479,478,200,289,
290,290,290,201,465,291,
292,292,292,292,292,292,292,292,38,292,292,292,292,292,292,292,292,292,292,
  292,38,202,292,292,38,
33,412,293,
33,409,202,294,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,69,68,67,66,65,1,205,70,1,72,
295,295,295,295,295,295,295,295,295,369,295,295,295,369,295,295,369,295,295,
  295,295,295,295,295,295,369,295,369,369,369,369,295,369,206,295,295,371,
30,364,296,365,
38,297,38,208,297,297,38,
38,298,38,209,298,298,38,
38,299,38,210,299,299,38,
38,300,38,211,300,300,38,
206,206,206,206,206,206,206,206,206,206,206,206,206,206,206,206,206,251,206,
  208,206,209,206,210,211,206,206,206,206,212,205,206,207,208,209,210,211,
  207,
301,301,301,301,301,301,327,
145,145,145,145,145,145,214,320,
214,324,322,
145,145,145,316,145,145,145,316,316,316,316,316,316,216,318,318,146,
302,302,302,217,314,303,
74,74,74,127,
294,294,294,294,294,294,294,294,294,294,294,294,294,294,294,294,294,294,294,
  294,294,294,296,294,
38,304,38,220,304,304,38,
38,305,38,221,305,305,38,
306,306,306,306,306,306,306,306,356,306,306,306,356,306,306,356,306,306,306,
  306,306,356,356,356,30,356,356,222,307,306,
38,308,38,223,308,308,38,
38,309,38,224,309,309,38,
38,310,38,225,310,310,38,
38,311,38,226,311,311,38,
38,312,38,227,312,312,38,
206,206,206,206,206,206,206,206,206,206,206,206,206,206,206,363,220,222,223,
  226,227,206,206,206,206,206,206,221,224,225,206,206,206,228,343,333,334,
  335,336,337,338,339,340,341,342,207,
379,379,379,379,379,379,379,379,379,379,379,379,379,379,379,379,379,379,379,
  379,379,379,379,379,380,
313,230,
229,229,229,229,229,229,229,229,229,229,229,229,229,229,229,229,229,229,229,
  229,229,229,229,229,231,376,230,229,
314,314,314,314,314,314,314,314,314,314,314,314,314,314,314,314,314,314,314,
  314,314,314,314,314,314,314,314,314,314,314,232,165,314,
315,315,315,315,315,315,315,315,315,315,315,315,315,315,315,315,315,315,315,
  315,315,315,315,315,315,315,315,315,315,315,315,315,315,315,315,315,315,
  315,315,315,315,51,233,315,315,53,
38,38,21,234,388,388,38,
106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,
  106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,
  106,106,108,106,
21,21,21,21,21,21,21,21,38,21,21,21,21,21,21,21,21,21,21,21,21,316,38,236,
  316,316,38,
38,317,38,237,317,317,38,
38,318,38,238,318,318,38,
38,319,38,239,319,319,38,
320,320,320,38,320,320,320,38,240,320,320,38,
38,321,321,321,321,321,38,241,321,321,38,
38,322,38,242,322,322,38,
25,575,323,
38,324,38,244,324,324,38,
552,551,245,327,326,325,
38,328,38,246,328,328,38,
38,329,38,247,329,329,38,
38,330,38,248,330,330,38,
38,331,38,249,331,331,38,
40,40,40,40,40,40,40,40,332,40,40,40,40,40,40,40,40,40,40,333,332,250,332,
38,334,38,251,334,334,38,
38,335,38,252,335,335,38,
38,336,38,253,336,336,38,
38,337,38,254,337,337,38,
38,338,38,255,338,338,38,
38,21,38,21,256,576,576,38,
25,574,339,
25,505,323,
38,340,38,259,340,340,38,
38,341,38,260,341,341,38,
38,342,38,261,342,342,38,
38,343,38,262,343,343,38,
38,344,38,263,344,344,38,
40,40,40,40,40,40,40,40,345,40,40,40,40,40,40,40,40,40,40,333,345,264,345,
38,21,38,21,265,506,506,38,
25,504,346,
38,347,38,267,347,347,38,
74,74,74,74,74,74,74,74,74,264,261,257,256,74,74,74,178,74,268,348,407,75,
  74,406,2,403,404,405,3,5,4,
349,349,349,349,349,349,349,349,38,349,349,349,349,349,349,349,349,349,349,
  38,269,349,349,38,
33,629,350,
94,94,94,94,94,94,94,94,94,94,94,94,94,94,94,94,94,94,95,36,271,98,97,99,
  100,96,627,
351,351,351,351,351,351,351,351,38,351,351,351,351,351,351,351,351,351,351,
  38,272,351,351,38,
38,21,21,38,352,21,273,352,352,38,
115,115,115,274,353,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,119,1,275,118,1,113,104,103,
174,174,174,174,174,174,174,174,174,174,174,174,174,174,174,174,174,174,174,
  174,174,174,174,174,174,174,174,175,
38,21,21,38,21,277,281,281,38,
34,373,373,373,278,156,279,157,
34,373,373,373,279,156,278,157,
34,373,373,373,280,156,276,157,
34,373,373,373,281,156,275,157,
34,373,373,373,282,156,273,157,
34,373,373,373,283,156,272,157,
1,1,1,1,1,1,1,1,1,1,1,355,356,1,1,357,1,1,1,1,1,284,354,1,444,439,360,359,
  358,
361,361,361,361,361,361,361,361,38,361,361,361,361,361,361,361,361,361,361,
  361,361,361,38,285,361,361,38,
33,436,285,
94,94,94,94,94,94,94,94,94,94,94,94,94,264,261,257,256,94,94,94,94,178,94,
  95,36,287,98,97,114,119,117,118,116,115,120,120,120,120,120,120,121,96,
  434,
362,38,362,362,38,288,362,362,38,
38,38,289,363,38,
494,494,494,290,
32,291,364,
1,1,1,1,1,1,1,1,1,1,1,1,1,365,1,1,1,1,1,292,354,1,428,366,425,
367,367,367,367,367,367,367,367,38,367,367,367,367,367,367,367,367,367,367,
  367,38,293,367,367,38,
33,410,293,
368,368,368,368,368,368,368,368,368,368,368,368,368,368,368,368,368,368,368,
  368,368,368,368,368,370,368,
368,368,368,368,368,368,368,368,368,38,368,368,368,368,368,368,368,368,368,
  368,368,368,368,368,38,368,296,368,368,38,
30,297,369,
30,298,370,
30,299,371,
30,300,372,
329,329,329,329,329,329,328,
373,373,373,310,
374,303,
30,304,375,
30,305,376,
1,1,1,1,1,1,1,1,358,1,1,1,358,1,1,358,1,1,1,1,1,358,358,358,358,358,306,360,
  1,360,
355,354,353,357,
30,308,377,
30,309,378,
30,310,379,
30,311,380,
30,312,381,
382,382,382,382,382,382,382,382,382,384,382,382,382,384,382,382,384,384,382,
  382,382,382,382,382,382,384,382,382,384,382,384,384,382,384,313,377,382,
  382,386,
171,171,171,171,171,171,171,171,171,171,171,171,171,171,171,171,171,171,171,
  171,171,171,171,171,171,171,171,171,171,171,172,
50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,
  50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,52,50,
383,383,383,383,383,383,383,383,383,383,383,383,383,383,383,383,383,383,383,
  383,37,316,383,383,
384,317,640,
95,318,534,599,598,
95,319,534,639,
385,385,385,385,385,385,320,600,385,385,
82,84,85,83,81,321,635,633,632,86,634,636,637,
30,322,386,571,
387,387,387,387,387,387,387,387,38,387,387,387,387,387,387,387,387,387,387,
  38,323,387,387,38,
30,324,388,
1,1,1,1,1,1,1,1,549,1,1,1,1,1,1,1,1,1,1,325,550,1,547,
389,326,390,
539,541,539,539,539,327,541,
30,328,391,
30,329,392,
30,330,393,
30,331,394,
28,395,28,332,
396,396,396,396,396,396,396,396,173,396,396,396,396,396,396,396,396,396,396,
  396,173,333,173,173,396,
30,334,397,
30,335,398,
30,336,399,
30,337,400,
30,338,401,
402,402,402,402,402,402,402,402,38,402,402,402,402,402,402,402,402,402,402,
  402,402,402,402,402,402,402,402,402,402,402,402,38,339,402,402,38,
30,340,403,
30,341,404,
30,342,405,
30,343,406,
30,344,529,407,
408,408,408,408,408,408,408,408,28,408,408,408,408,408,408,408,408,408,408,
  395,28,345,408,518,
409,409,409,409,409,409,409,409,38,409,409,409,409,409,409,409,409,409,409,
  409,409,409,409,409,409,409,409,409,38,346,409,409,38,
414,347,
38,410,38,348,410,410,38,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,349,411,1,92,
412,412,412,412,412,412,412,412,38,412,412,412,412,412,412,412,412,412,412,
  38,350,412,412,38,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,351,120,1,413,
32,128,414,
33,114,415,
38,21,416,21,38,21,354,416,416,38,
38,417,38,355,417,417,38,
38,418,38,356,418,418,38,
38,419,38,357,419,419,38,
38,21,21,38,21,358,443,443,38,
38,21,21,38,21,359,442,442,38,
38,21,21,38,21,360,441,441,38,
1,1,1,1,1,1,1,1,1,1,1,355,356,1,1,357,1,1,1,1,1,361,354,1,444,440,360,359,
  358,
124,124,124,362,420,126,
421,421,421,363,475,
422,422,422,364,423,
38,424,38,365,424,424,38,
38,21,38,21,366,427,427,38,
1,1,1,1,1,1,1,1,1,1,1,1,1,365,1,1,1,1,1,367,354,1,428,366,426,
206,206,206,206,206,206,206,206,206,206,206,206,206,206,206,206,206,206,206,
  206,206,206,206,206,368,372,
425,425,425,425,38,425,425,425,425,425,425,425,38,369,425,425,38,
426,426,426,426,426,426,426,426,426,38,426,426,426,426,426,426,426,426,426,
  426,426,426,426,426,426,426,38,426,370,426,426,38,
427,427,427,427,427,38,427,427,427,427,427,427,38,371,427,427,38,
428,428,38,428,428,428,428,38,372,428,428,38,
312,312,312,311,
302,302,302,374,429,
430,430,430,430,38,430,430,430,430,430,430,430,38,375,430,430,38,
431,431,431,431,431,431,431,431,38,431,431,431,431,431,431,431,431,431,431,
  431,38,376,431,431,38,
432,432,432,432,432,432,432,432,38,432,432,432,432,432,432,432,432,432,432,
  432,432,432,432,432,38,377,432,432,38,
433,433,433,433,38,433,433,433,433,433,433,433,38,378,433,433,38,
434,38,434,434,38,379,434,434,38,
435,435,435,435,435,435,435,435,38,435,435,435,435,435,435,435,435,435,435,
  435,435,38,380,435,435,38,
436,436,436,436,436,436,436,436,38,436,436,436,436,436,436,436,436,436,436,
  436,436,436,436,38,381,436,436,38,
383,383,383,383,383,383,383,383,383,383,383,383,383,383,383,383,383,383,383,
  383,383,383,383,383,383,385,383,
1,1,1,1,1,1,1,1,558,557,1,1,1,1,1,1,1,1,1,1,383,559,1,437,
438,438,438,438,438,438,439,384,438,438,438,438,439,
440,440,440,440,440,440,601,440,440,
441,441,441,441,441,441,441,441,38,441,441,441,441,441,441,441,441,441,441,
  441,38,386,441,441,38,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,387,242,1,570,
38,37,38,542,442,38,543,
443,389,
444,444,444,390,444,445,
38,446,38,391,446,446,38,
21,21,21,21,21,21,21,21,38,21,21,21,21,21,21,21,21,21,21,21,21,447,38,392,
  447,447,38,
38,448,448,38,393,448,448,38,
38,449,38,394,449,449,38,
450,450,450,450,450,450,450,450,173,450,450,450,450,450,450,450,450,450,450,
  450,173,395,173,173,450,
452,452,452,452,452,452,452,452,452,452,452,452,452,452,452,452,452,452,451,
  396,452,
38,453,38,397,453,453,38,
38,454,38,398,454,454,38,
38,455,38,399,455,455,38,
38,456,38,400,456,456,38,
38,457,38,401,457,457,38,
250,250,250,250,250,250,250,250,248,249,252,85,83,246,255,247,250,250,250,
  244,251,253,254,250,250,250,250,250,250,250,402,250,579,580,583,585,590,
  245,458,578,581,582,584,586,587,588,589,
38,459,38,403,459,459,38,
38,460,460,38,404,460,460,38,
461,405,561,
461,406,560,
38,21,38,407,528,528,38,462,
40,40,40,40,40,40,40,40,173,40,40,40,40,40,40,40,40,40,40,463,173,408,173,
  173,463,
264,264,264,264,264,264,264,264,261,262,259,264,264,264,244,260,251,253,263,
  254,264,264,264,264,264,264,264,409,264,464,508,509,510,511,512,513,514,
  515,516,517,245,
413,410,
38,21,465,21,38,21,411,465,465,38,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,412,411,1,93,
38,466,38,413,466,466,38,
467,38,467,467,38,414,467,467,38,
468,468,468,468,468,468,468,468,38,468,468,468,468,468,468,468,468,468,468,
  468,468,468,468,468,468,468,468,38,415,468,468,38,
30,140,469,
30,417,470,
30,418,471,
30,419,472,
38,38,420,473,38,
474,474,474,421,
495,495,495,422,
32,423,475,
30,424,476,
478,478,478,477,478,478,478,477,477,478,30,425,232,231,232,232,479,
481,481,481,481,481,481,481,481,481,481,481,481,481,481,481,481,480,481,481,
  481,481,481,481,481,481,481,426,223,481,481,481,
482,482,482,482,482,482,482,482,482,482,482,427,216,482,482,
483,483,483,483,483,483,428,212,483,
484,429,
63,63,63,63,63,63,63,63,63,64,63,430,362,303,302,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,160,1,431,161,1,361,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,69,68,67,66,65,1,432,70,1,352,
63,63,63,63,63,63,63,63,63,64,63,433,351,303,302,
485,485,485,434,350,
1,1,1,1,1,1,1,1,1,1,1,1,1,348,347,1,1,1,1,1,435,349,1,345,346,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,125,124,123,122,1,436,126,1,344,
539,638,539,539,539,437,638,
643,643,643,643,643,643,645,643,643,643,
646,439,
486,486,486,486,486,486,602,486,486,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,95,441,488,1,487,
37,544,545,
489,443,
74,74,74,553,
38,445,490,
37,446,491,593,
492,492,492,492,492,492,492,492,492,492,492,492,492,492,492,492,492,492,492,
  492,37,447,492,492,
624,623,448,
493,449,615,
495,495,495,495,495,495,495,495,495,495,495,495,495,495,495,495,495,495,494,
  450,495,
175,175,175,175,175,175,175,175,175,175,175,175,175,175,175,175,175,175,175,
  175,175,175,175,175,175,175,175,175,175,174,175,496,175,175,175,175,175,
  175,175,175,175,175,451,175,175,175,496,
40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,563,
95,453,536,535,
497,454,609,
95,455,534,533,
95,456,520,519,
95,457,592,591,
38,21,38,21,458,577,577,38,
37,459,498,
538,537,460,
499,499,499,499,499,499,461,500,499,
501,462,
502,463,
38,21,38,21,464,507,507,38,
30,94,386,95,
35,466,503,
218,218,218,467,218,504,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,508,510,511,506,505,507,512,513,1,468,354,
  1,116,130,131,132,133,134,135,136,137,138,138,146,509,155,
514,514,514,514,514,514,514,514,38,514,514,514,514,514,514,514,514,514,514,
  514,514,38,469,514,514,38,
515,38,515,515,515,38,470,515,515,38,
38,516,516,38,471,516,516,38,
38,517,517,38,472,517,517,38,
290,290,290,473,467,291,
518,518,518,490,
519,519,519,475,520,
521,521,521,521,521,521,521,521,38,521,521,521,521,521,521,521,521,521,521,
  38,476,521,521,38,
522,522,522,522,244,522,522,522,522,522,244,244,522,522,244,244,244,244,477,
  522,522,522,247,
522,522,522,522,244,522,522,522,522,522,244,244,522,522,244,244,244,244,478,
  522,522,522,246,
238,238,523,238,238,238,238,479,524,524,524,240,
525,525,525,525,525,525,480,
225,225,225,225,225,225,225,225,225,225,225,225,225,225,225,225,480,225,225,
  225,225,225,225,225,225,225,226,225,225,
218,218,218,218,218,218,218,218,218,218,218,219,218,
214,214,214,214,214,214,215,
302,302,302,484,309,
526,526,526,148,
527,527,527,527,527,527,603,527,527,
38,21,21,38,21,487,97,97,38,
38,21,21,38,21,488,96,96,38,
548,489,
1,1,1,1,1,1,1,1,549,1,1,1,1,1,1,1,1,1,1,490,550,1,546,
74,74,74,74,74,74,74,74,74,264,261,257,256,74,74,74,178,74,491,528,528,407,
  75,74,406,2,403,404,405,3,5,4,
1,1,1,1,1,1,1,1,558,557,1,1,1,1,1,1,1,1,1,1,492,559,1,529,
530,530,530,530,530,530,530,530,530,530,530,530,493,530,
175,175,175,175,175,175,175,175,175,175,175,175,175,175,175,175,175,175,175,
  175,175,175,175,175,175,175,175,175,175,174,175,531,175,175,175,175,175,
  175,175,175,175,175,494,175,175,175,531,
40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,565,
564,496,
533,533,533,38,533,533,533,532,38,497,532,532,38,533,534,533,
1,1,1,1,1,1,1,1,558,557,1,1,1,1,1,1,1,1,1,1,498,559,1,535,555,
63,63,63,63,63,63,64,
562,500,
537,537,537,537,537,537,537,537,536,537,537,537,537,537,537,537,537,537,537,
  537,537,537,536,501,536,537,
538,538,538,538,538,538,538,538,173,538,538,538,538,538,538,538,538,538,538,
  538,173,502,173,173,538,
539,539,539,539,539,539,539,539,38,539,539,539,539,539,539,539,539,539,539,
  539,539,539,539,38,503,539,539,38,
38,21,21,38,21,504,129,129,38,
38,21,21,21,38,21,505,152,152,38,
38,540,38,506,540,540,38,
38,541,38,507,541,541,38,
38,542,38,508,542,542,38,
153,30,153,153,509,543,151,
38,544,38,510,544,544,38,
38,545,38,511,545,545,38,
38,21,21,38,21,512,145,145,38,
38,21,21,38,21,513,144,144,38,
1,1,1,1,1,1,1,1,1,1,1,1,1,64,1,1,1,1,1,95,514,548,1,547,546,
130,130,130,37,515,549,130,458,459,
456,455,516,454,
550,551,517,445,
492,492,492,491,
496,496,496,519,
38,38,520,552,38,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,521,424,1,
243,243,243,243,243,243,243,243,243,243,243,245,243,243,
478,478,478,477,478,478,478,477,477,478,523,248,
523,239,237,237,
230,230,230,230,230,230,525,
150,150,150,149,
553,553,553,553,553,553,604,553,553,
594,528,
539,597,539,539,539,529,597,
617,617,617,617,554,554,617,617,617,617,617,617,617,554,617,530,554,554,
566,531,
611,532,
555,555,555,555,555,555,533,555,555,
610,534,
38,554,556,38,535,556,556,38,
28,28,522,
1,1,1,1,1,1,1,1,1,1,1,1,1,257,256,1,1,1,1,1,523,537,527,1,557,3,558,557,558,
560,560,560,560,560,560,560,560,560,560,560,560,560,560,560,560,560,560,559,
  538,560,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,125,124,123,122,1,539,126,1,117,121,
30,540,561,
30,541,562,
30,542,563,
564,38,564,564,38,543,564,564,38,
30,544,565,
30,545,566,
38,21,21,38,21,546,143,143,38,
38,21,21,38,21,547,142,142,38,
38,21,21,38,21,548,141,141,38,
38,567,567,567,567,567,567,567,38,549,567,567,38,
446,568,446,446,446,446,550,449,
446,568,446,446,446,446,551,448,
499,498,497,552,493,
569,569,569,569,569,569,605,569,569,
620,554,
570,570,570,570,570,570,555,570,570,
25,556,571,
572,525,572,
536,573,536,558,536,573,
175,175,175,175,175,175,175,175,175,175,175,175,175,175,175,175,175,175,175,
  175,175,175,175,175,175,175,175,175,175,174,175,574,175,175,175,175,175,
  175,175,175,175,175,559,175,175,175,574,
40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,567,
575,575,575,575,38,575,575,575,575,575,575,575,38,561,575,575,38,
576,38,576,576,38,562,576,576,38,
577,577,577,577,577,577,577,577,38,577,577,577,577,577,577,577,577,577,577,
  577,38,563,577,577,38,
218,218,218,564,218,154,
578,578,578,578,578,578,578,578,38,578,578,578,578,578,578,578,578,578,578,
  38,565,578,578,38,
579,579,579,38,579,579,579,579,38,566,579,579,38,
474,473,472,471,470,469,468,567,580,125,
581,450,581,581,450,450,450,450,568,447,
582,582,582,582,582,582,606,582,582,
583,583,583,583,583,583,570,583,583,
584,584,584,584,584,584,584,584,38,584,584,584,584,584,584,584,584,584,584,
  584,584,38,571,584,584,38,
257,256,572,531,3,
524,573,
568,574,
63,63,63,63,63,63,63,63,63,64,63,575,585,303,302,
485,485,485,576,586,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,160,1,577,161,1,587,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,578,588,1,
145,145,145,145,145,145,147,579,589,162,149,148,146,
38,590,38,580,590,590,38,
591,591,591,451,
608,608,608,608,608,608,607,608,608,
592,592,592,592,592,592,583,592,592,
1,1,1,1,1,1,1,1,558,557,1,1,1,1,1,1,1,1,1,1,584,559,1,556,
38,21,21,38,21,585,156,156,38,
38,21,21,38,21,586,147,147,38,
38,21,21,38,21,587,159,159,38,
38,21,21,38,21,588,158,158,38,
38,21,21,38,21,589,157,157,38,
460,590,
453,453,453,452,
593,593,593,593,593,593,592,593,593,
594,594,594,594,594,594,593,594,594,
595,595,595,595,595,595,594,595,595,
596,596,596,596,596,596,595,596,596,
597,597,597,597,597,597,596,597,597,
598,598,598,598,598,598,597,598,598,
599,599,599,599,599,599,598,599,599,
600,600,600,600,600,600,599,600,600,
601,601,601,601,601,601,600,601,601,
602,602,602,602,602,602,601,602,602,
603,603,603,603,603,603,602,603,603,
604,604,604,604,604,604,603,604,604,
605,605,605,605,605,605,604,605,605,
606,606,606,606,606,606,605,606,606,
607,607,607,607,607,607,606,607,607,
608,608,608,608,608,608,607,608,608,
609,609,609,609,609,609,608,609,609,
610,610,610,610,610,610,609,610,610,
611,611,611,611,611,611,610,611,611,
612,612,612,612,612,612,611,612,612,
613,613,613,613,613,613,612,613,613,
614,614,614,614,614,614,613,614,614,
615,615,615,615,615,615,614,615,615,
616,616,616,616,616,616,615,616,616,
617,617,617,617,617,617,616,617,617,
618,618,618,618,618,618,617,618,618,
612,612,612,612,612,612,618,612,612,
};
static const unsigned short ag_sbt[] = {
     0,  78,  97, 118, 126, 136, 146, 154, 158, 160, 162, 164, 166, 168,
   171, 173, 175, 178, 181, 184, 187, 190, 193, 196, 199, 202, 205, 208,
   211, 224, 246, 254, 264, 280, 313, 325, 337, 354, 371, 374, 377, 399,
   431, 439, 495, 513, 527, 552, 577, 606, 633, 640, 675, 723, 740, 757,
   801, 809, 831, 844, 851, 868, 903, 906, 921, 933, 936, 952, 979, 982,
   985,1024,1027,1038,1049,1063,1066,1112,1116,1120,1123,1125,1132,1139,
  1146,1153,1160,1168,1170,1175,1180,1182,1187,1192,1194,1220,1267,1286,
  1294,1318,1321,1330,1333,1335,1342,1347,1350,1352,1356,1363,1365,1396,
  1399,1406,1408,1411,1420,1429,1449,1456,1459,1463,1472,1475,1477,1481,
  1488,1493,1498,1505,1507,1511,1518,1520,1524,1532,1534,1538,1543,1550,
  1552,1569,1577,1615,1650,1664,1671,1684,1701,1703,1705,1707,1713,1742,
  1758,1804,1843,1871,1873,1909,1950,1955,1959,2004,2007,2010,2013,2016,
  2019,2022,2045,2093,2116,2158,2162,2205,2249,2251,2277,2308,2332,2336,
  2362,2365,2381,2406,2439,2469,2494,2504,2514,2539,2549,2559,2569,2579,
  2606,2609,2613,2644,2647,2661,2667,2692,2695,2699,2726,2763,2767,2774,
  2781,2788,2795,2833,2840,2848,2851,2868,2874,2878,2902,2909,2916,2946,
  2953,2960,2967,2974,2981,3027,3052,3054,3082,3115,3161,3168,3209,3236,
  3243,3250,3257,3269,3280,3287,3290,3297,3303,3310,3317,3324,3331,3354,
  3361,3368,3375,3382,3389,3397,3400,3403,3410,3417,3424,3431,3438,3461,
  3469,3472,3479,3510,3534,3537,3564,3588,3598,3603,3628,3656,3665,3673,
  3681,3689,3697,3705,3713,3742,3769,3772,3815,3824,3829,3833,3836,3861,
  3886,3889,3915,3945,3948,3951,3954,3957,3964,3968,3970,3973,3976,4006,
  4010,4013,4016,4019,4022,4025,4064,4095,4138,4162,4165,4170,4174,4184,
  4197,4201,4225,4228,4251,4254,4261,4264,4267,4270,4273,4277,4302,4305,
  4308,4311,4314,4317,4353,4356,4359,4362,4365,4369,4393,4426,4428,4435,
  4457,4481,4503,4506,4509,4519,4526,4533,4540,4549,4558,4567,4596,4602,
  4607,4612,4619,4627,4652,4678,4695,4727,4744,4756,4760,4765,4782,4807,
  4836,4853,4862,4888,4916,4943,4967,4980,4989,5014,5036,5043,5045,5051,
  5058,5085,5093,5100,5125,5146,5153,5160,5167,5174,5181,5228,5235,5243,
  5246,5249,5257,5282,5323,5325,5335,5357,5364,5373,5405,5408,5411,5414,
  5417,5422,5426,5430,5433,5436,5453,5484,5499,5508,5510,5525,5548,5575,
  5590,5595,5620,5646,5653,5663,5665,5674,5697,5700,5702,5706,5709,5713,
  5737,5740,5743,5764,5811,5830,5834,5837,5841,5845,5849,5857,5860,5863,
  5872,5874,5876,5884,5888,5891,5897,5940,5966,5976,5984,5992,5998,6002,
  6007,6031,6054,6077,6089,6096,6125,6138,6145,6150,6154,6163,6172,6181,
  6183,6206,6238,6262,6276,6323,6342,6344,6360,6385,6392,6394,6420,6445,
  6473,6482,6492,6499,6506,6513,6520,6527,6534,6543,6552,6577,6586,6590,
  6594,6598,6602,6607,6628,6642,6654,6658,6665,6669,6678,6680,6687,6705,
  6707,6709,6718,6720,6728,6731,6760,6781,6808,6811,6814,6817,6826,6829,
  6832,6841,6850,6859,6872,6880,6888,6893,6902,6904,6913,6916,6919,6925,
  6972,6991,7008,7017,7042,7048,7072,7085,7095,7105,7114,7123,7149,7154,
  7156,7158,7173,7178,7201,7222,7235,7242,7246,7255,7264,7288,7297,7306,
  7315,7324,7333,7335,7339,7348,7357,7366,7375,7384,7393,7402,7411,7420,
  7429,7438,7447,7456,7465,7474,7483,7492,7501,7510,7519,7528,7537,7546,
  7555,7564,7573,7582
};
static const unsigned short ag_sbe[] = {
    55,  96, 109, 120, 129, 139, 149, 155, 159, 161, 163, 165, 167, 169,
   172, 174, 176, 179, 182, 185, 188, 191, 194, 197, 200, 203, 206, 209,
   222, 242, 252, 260, 275, 307, 321, 333, 350, 367, 373, 375, 398, 417,
   435, 491, 508, 518, 546, 571, 597, 625, 636, 670, 701, 733, 750, 782,
   804, 827, 842, 850, 864, 899, 904, 919, 928, 934, 947, 978, 980, 983,
  1020,1025,1034,1045,1062,1064,1108,1113,1119,1121,1124,1128,1135,1142,
  1149,1156,1164,1169,1172,1177,1181,1184,1189,1193,1216,1262,1285,1290,
  1314,1319,1326,1331,1334,1338,1344,1348,1351,1355,1359,1364,1395,1397,
  1402,1407,1409,1414,1423,1441,1451,1457,1460,1468,1473,1476,1480,1484,
  1490,1495,1501,1506,1510,1514,1519,1521,1528,1533,1537,1540,1546,1551,
  1565,1575,1606,1646,1663,1670,1680,1697,1702,1704,1706,1710,1736,1753,
  1791,1839,1867,1872,1905,1948,1952,1958,1999,2005,2008,2011,2014,2017,
  2020,2040,2075,2111,2143,2160,2204,2246,2250,2273,2295,2328,2333,2358,
  2363,2376,2402,2436,2466,2488,2500,2510,2533,2545,2555,2565,2575,2602,
  2607,2610,2640,2645,2659,2664,2688,2693,2696,2722,2759,2764,2770,2777,
  2784,2791,2824,2839,2846,2849,2864,2871,2877,2900,2905,2912,2943,2949,
  2956,2963,2970,2977,3014,3051,3053,3078,3112,3157,3164,3207,3232,3239,
  3246,3253,3265,3276,3283,3288,3293,3299,3306,3313,3320,3327,3352,3357,
  3364,3371,3378,3385,3393,3398,3401,3406,3413,3420,3427,3434,3459,3465,
  3470,3475,3497,3530,3535,3557,3584,3594,3601,3622,3655,3661,3669,3677,
  3685,3693,3701,3709,3734,3765,3770,3797,3820,3826,3832,3834,3855,3882,
  3887,3913,3941,3946,3949,3952,3955,3963,3967,3969,3971,3974,4002,4009,
  4011,4014,4017,4020,4023,4059,4094,4136,4159,4163,4166,4171,4180,4189,
  4198,4221,4226,4247,4252,4259,4262,4265,4268,4271,4276,4298,4303,4306,
  4309,4312,4315,4349,4354,4357,4360,4363,4366,4390,4422,4427,4431,4453,
  4477,4499,4504,4507,4515,4522,4529,4536,4545,4554,4563,4588,4599,4605,
  4610,4615,4623,4646,4676,4691,4723,4740,4752,4759,4763,4778,4803,4832,
  4849,4858,4884,4912,4941,4963,4974,4986,5010,5032,5039,5044,5048,5054,
  5081,5089,5096,5121,5144,5149,5156,5163,5170,5177,5211,5231,5239,5244,
  5247,5252,5278,5309,5324,5331,5353,5360,5369,5401,5406,5409,5412,5415,
  5419,5425,5429,5431,5434,5447,5479,5495,5505,5509,5521,5544,5571,5586,
  5593,5615,5642,5651,5659,5664,5671,5693,5698,5701,5705,5707,5710,5734,
  5739,5741,5762,5806,5829,5831,5835,5838,5842,5846,5853,5858,5862,5869,
  5873,5875,5880,5885,5889,5894,5923,5962,5972,5980,5988,5995,6001,6005,
  6027,6049,6072,6084,6095,6122,6136,6144,6148,6153,6160,6168,6177,6182,
  6202,6224,6258,6274,6318,6341,6343,6353,6380,6391,6393,6417,6441,6469,
  6478,6488,6495,6502,6509,6517,6523,6530,6539,6548,6572,6581,6588,6592,
  6597,6601,6604,6625,6639,6652,6655,6664,6668,6675,6679,6685,6702,6706,
  6708,6715,6719,6724,6730,6752,6779,6803,6809,6812,6815,6822,6827,6830,
  6837,6846,6855,6868,6878,6886,6891,6899,6903,6910,6914,6917,6922,6967,
  6990,7004,7013,7038,7045,7068,7081,7092,7103,7111,7120,7145,7151,7155,
  7157,7169,7176,7197,7219,7229,7238,7245,7252,7261,7284,7293,7302,7311,
  7320,7329,7334,7338,7345,7354,7363,7372,7381,7390,7399,7408,7417,7426,
  7435,7444,7453,7462,7471,7480,7489,7498,7507,7516,7525,7534,7543,7552,
  7561,7570,7579,7582
};
static const unsigned char ag_fl[] = {
  1,4,4,4,4,4,4,4,4,4,4,4,4,2,2,4,2,2,2,2,2,0,1,2,1,1,1,1,2,1,1,1,1,1,1,
  1,1,1,1,1,2,1,1,2,1,1,2,1,1,1,2,0,1,2,1,1,2,0,1,3,1,2,1,2,1,1,1,1,1,1,
  1,2,3,1,2,1,2,1,2,1,3,1,5,0,1,3,4,1,1,3,4,4,3,4,2,3,4,4,1,2,5,3,1,1,1,
  1,2,0,1,1,3,1,1,4,4,0,4,9,1,1,1,1,1,1,1,1,1,1,2,6,1,1,1,1,1,1,1,1,1,1,
  2,6,6,6,2,2,1,6,1,2,3,2,2,0,3,1,6,6,6,6,1,1,1,1,1,4,1,2,1,1,1,2,1,1,2,
  1,1,2,1,2,1,1,1,1,1,2,0,1,3,1,2,4,1,1,1,2,0,1,2,3,1,1,0,1,3,4,1,1,1,1,
  1,1,5,1,2,1,5,1,2,1,1,1,1,5,1,2,1,1,1,1,3,5,1,1,1,1,1,2,0,1,2,1,1,2,0,
  1,2,2,2,1,1,1,1,1,1,2,1,1,3,2,1,1,3,2,1,3,3,1,1,1,1,1,4,4,2,4,4,3,4,4,
  3,4,4,3,1,1,3,1,1,2,1,1,1,1,2,0,1,1,2,1,1,3,1,1,1,1,2,2,3,5,1,2,3,1,3,
  1,0,1,3,2,2,1,2,0,1,2,1,2,3,4,0,1,3,4,1,1,1,1,1,1,1,1,1,1,5,5,5,1,1,1,
  5,5,5,3,3,3,0,1,0,1,3,5,5,1,1,2,1,1,2,0,1,2,3,0,1,2,3,3,1,2,1,1,1,2,0,
  1,1,1,6,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,3,1,2,6,5,2,1,1,1,2,
  0,1,3,2,5,3,4,2,1,1,1,1,1,1,4,2,3,1,2,3,4,2,2,2,1,5,0,2,2,2,0,1,2,3,5,
  1,1,2,5,1,5,1,1,1,2,3,1,7,1,1,1,1,1,1,1,5,1,2,1,1,1,1,1,1,1,1,1,1,1,1,
  2,3,4,7,2,2,2,1,1,1,4,5,1,1,3,3,2,5,1,1,1,1,1,1,1,1,1,1,3,5,1,0,1,7,9,
  1,1,1,1,1,1,3,1,5,1,5,1,5,5,0,1,3,3,4,4,5,5,2,4,1,1,1,1,1,7,1,5,1,1,1,
  4,4,3,4,6,5,7,5,7,1,4,3,1,1,3,3,2,5,1,1,1,1,1,1,1,1,1,1,1,1,1,5,1,5,3,
  0,1,7,5,1,5,1,2,3,4,5,6,7,8,5,3,3,32,1,1,5,1,2,0,1,4,1,1,5,5,1,1,4,2,3,
  1,1,5,1,1,1,1,1,7,5,5,1,1,2,0,1,3
};
static const unsigned short ag_ptt[] = {
    0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1, 39, 39,  3, 40, 42, 44, 46, 46, 38, 47, 49, 51, 52, 54, 56,
   58, 60, 62, 66, 66, 64, 69, 69, 67, 72, 72, 70, 76, 77, 77, 78, 78, 73,
   81, 82, 82, 83, 83, 79, 80, 80, 89, 89, 87, 90, 90, 90, 90, 90, 90,  4,
   96, 99, 99, 97, 17,100, 15,101,101,102,103,106,106,107,107,104,105,108,
  109,109,110,110,111,111,112,112,114,114, 28, 28,113,115,120,121,121,122,
  122,116,118, 19,123,123,124,127,127,125,129,129,130,131,133,133,133,133,
  133,138,126,126,128,128,128,128,128,128,128,128,128,148,149,149,149,149,
  140,141,142,153,155,155,155,146,156,157,157,143,159,144,145,147,165,165,
  162,168, 31,170,174,174,171,175,178,178,172,181,181,179, 35,182,183,184,
  186,186,188,193,194,194,195,195,190,197,197,190,189,201,202,202,203,203,
  198,198,199,199,187,187,205,205,206,206,206,206,206,206,207,215,215,214,
  208,219,219,217,218,218,218,209,225,225,223,224,224,224,228,210,231,232,
  232,236,237,237,238,238,233,241,242,242,243,243,234,234,235,240,240,211,
   34, 32, 33,247,250,250,248,248,255,254,249,249,258,251,251,264,264,264,
  264,264,265,265,266,267,267,267,268,268,268,269,260,260,270,271,271,274,
  278,278,276,275,281,282,282,283,283,279,263,284,272,272,139,139,286,288,
  288,286,150,291,292,292,292,167,167,293,296,296,293,293,298,299,299,300,
  300,294,297,297,297,297,261,261,301,301,302,302,302,302,302,302,302,302,
  302,302,303,304,304,314,314,315,305,306,307,308,308,308,323,323,324,324,
  308,309,310,311,212,212,330,331,331,332,332,327,328,262,262,333,333,334,
  339,339,336,341,342,342,343,343,337, 29,344,345,345,345,345,345,345,345,
  345,345,345,345,345,345,345,169,169,169,169,169,  7,359,359,359,359,360,
  360,176,176,364,365,365,366,366,363,363,367,361,361,369,369, 13,370,370,
  371,372,372,374,374,374,374,375,375,376,376,376,376,377,384,384,381,381,
  383,383,383,383,378,387,387,  9,379,392,392,390,390,393, 11,395,395,394,
  398,398,398,398,398,398,398,396,406,406,407,407,407,407,407,407,407,407,
  407,407,407,407,408,408,408,397,421,422,423,424,424,424, 36, 36, 23,428,
  429,429,431,431,433,433,433,433,433,433,433,433,433,433,433,434,446,450,
  450,435,435,451,451,453,449,448,452,452,454,436,457,437,459,441,441,464,
  464,442,462,462,462,462,463,463,467,469,469,466,470,468,443,478,478,479,
  479,479,438,439,483,440,440,440,440,444,444,432,432,485, 25,486,487,487,
  488,488,489,489,489,489,489,489,489,489,489,489,489,489,489,490,499,497,
  501,502,502,496,492,504,493,506,506,506,506,506,506,506,506,491,509,509,
  510,512,507,494,516,516,518,518,514,517,517,495,495, 21,524,524,525,525,
   27,526,526,527,527,527,527,527,530,528,529,534,535,535,536,536,533
};
static void ag_ra(PCB_DECL)
{
  switch(ag_rpx[(PCB).ag_ap]) {
    case 1: ag_rp_1(PCB_POINTER); break;
    case 2: ag_rp_2(PCB_POINTER); break;
    case 3: ag_rp_3(PCB_POINTER); break;
    case 4: ag_rp_4(PCB_POINTER); break;
    case 5: ag_rp_5(PCB_POINTER); break;
    case 6: ag_rp_6(PCB_POINTER); break;
    case 7: ag_rp_7(PCB_POINTER); break;
    case 8: ag_rp_8(PCB_POINTER); break;
    case 9: ag_rp_9(PCB_POINTER); break;
    case 10: ag_rp_10(PCB_POINTER); break;
    case 11: ag_rp_11(PCB_POINTER); break;
    case 12: ag_rp_12(PCB_POINTER); break;
    case 13: ag_rp_13(PCB_POINTER); break;
    case 14: ag_rp_14(PCB_POINTER); break;
    case 15: V(0,ParserBasicToken) = ag_rp_15(PCB_POINTER); break;
    case 16: V(0,ParserBasicToken) = ag_rp_16(PCB_POINTER); break;
    case 17: V(0,ParserBasicToken) = ag_rp_17(PCB_POINTER); break;
    case 18: V(0,ParserBasicToken) = ag_rp_18(PCB_POINTER); break;
    case 19: V(0,ParserQuotedString) = ag_rp_19(PCB_POINTER); break;
    case 20: V(0,ParserBasicToken) = ag_rp_20(PCB_POINTER); break;
    case 21: V(0,ParserMethod) = ag_rp_21(PCB_POINTER); break;
    case 22: V(0,ParserMethod) = ag_rp_22(PCB_POINTER); break;
    case 23: V(0,ParserMethod) = ag_rp_23(PCB_POINTER); break;
    case 24: V(0,ParserMethod) = ag_rp_24(PCB_POINTER); break;
    case 25: V(0,ParserMethod) = ag_rp_25(PCB_POINTER); break;
    case 26: V(0,ParserMethod) = ag_rp_26(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 27: ag_rp_27(PCB_POINTER, V(0,ParserCSeq)); break;
    case 28: V(0,ParserCSeq) = ag_rp_28(PCB_POINTER, V(0,ParserSequenceNumber), V(2,ParserMethod)); break;
    case 29: V(0,ParserSequenceNumber) = ag_rp_29(PCB_POINTER); break;
    case 30: ag_rp_30(PCB_POINTER, V(0,ParserContentLength)); break;
    case 31: V(0,ParserContentLength) = ag_rp_31(PCB_POINTER); break;
    case 32: ag_rp_32(PCB_POINTER, V(0,ParserCallId)); break;
    case 33: V(0,ParserCallId) = ag_rp_33(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 34: V(0,ParserCallId) = ag_rp_34(PCB_POINTER, V(0,ParserBasicToken), V(2,ParserBasicToken)); break;
    case 35: ag_rp_35(PCB_POINTER); break;
    case 36: ag_rp_36(PCB_POINTER, V(0,ParserMediaType), V(2,ParserMediaSubType)); break;
    case 37: ag_rp_37(PCB_POINTER); break;
    case 38: ag_rp_38(PCB_POINTER); break;
    case 39: V(0,ParserMediaType) = ag_rp_39(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 40: V(0,ParserMediaSubType) = ag_rp_40(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 41: ag_rp_41(PCB_POINTER, V(0,ParserBasicToken), V(2,ParserGenericParamDataVal)); break;
    case 42: V(0,ParserGenericParamDataVal) = ag_rp_42(PCB_POINTER, V(2,ParserBasicToken)); break;
    case 43: V(0,ParserGenericParamDataVal) = ag_rp_43(PCB_POINTER, V(2,ParserQuotedString)); break;
    case 44: ag_rp_44(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 45: ag_rp_45(PCB_POINTER, V(0,ParserBasicToken), V(2,ParserGenericParamDataVal)); break;
    case 46: V(0,ParserGenericParamDataVal) = ag_rp_46(PCB_POINTER, V(2,ParserBasicToken)); break;
    case 47: V(0,ParserGenericParamDataVal) = ag_rp_47(PCB_POINTER, V(2,ParserQuotedString)); break;
    case 48: ag_rp_48(PCB_POINTER, V(0,ParserSipVersion), V(2,ParserStatusCode), V(4,ParserReasonPhrase)); break;
    case 49: ag_rp_49(PCB_POINTER, V(0,ParserSipVersion), V(2,ParserStatusCode)); break;
    case 50: V(0,ParserSipVersion) = ag_rp_50(PCB_POINTER); break;
    case 51: V(0,ParserReasonPhrase) = ag_rp_51(PCB_POINTER); break;
    case 52: V(0,ParserStatusCode) = ag_rp_52(PCB_POINTER, V(0,int), V(1,int), V(2,int)); break;
    case 53: ag_rp_53(PCB_POINTER); break;
    case 54: ag_rp_54(PCB_POINTER); break;
    case 55: ag_rp_55(PCB_POINTER); break;
    case 56: ag_rp_56(PCB_POINTER); break;
    case 57: ag_rp_57(PCB_POINTER, V(3,ParserViaParam)); break;
    case 58: ag_rp_58(PCB_POINTER, V(0,ParserProtocolName), V(4,ParserProtocolVersion), V(8,ParserTransport)); break;
    case 59: V(0,ParserProtocolName) = ag_rp_59(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 60: V(0,ParserProtocolName) = ag_rp_60(PCB_POINTER); break;
    case 61: V(0,ParserProtocolVersion) = ag_rp_61(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 62: V(0,ParserTransport) = ag_rp_62(PCB_POINTER, V(0,ParserTransport)); break;
    case 63: V(0,ParserTransport) = ag_rp_63(PCB_POINTER); break;
    case 64: V(0,ParserTransport) = ag_rp_64(PCB_POINTER); break;
    case 65: V(0,ParserTransport) = ag_rp_65(PCB_POINTER); break;
    case 66: V(0,ParserTransport) = ag_rp_66(PCB_POINTER); break;
    case 67: V(0,ParserTransport) = ag_rp_67(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 68: V(0,ParserPort) = ag_rp_68(PCB_POINTER); break;
    case 69: ag_rp_69(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 70: ag_rp_70(PCB_POINTER, V(0,ParserBasicToken), V(4,ParserPort)); break;
    case 71: V(0,ParserViaParam) = ag_rp_71(PCB_POINTER); break;
    case 72: V(0,ParserViaParam) = ag_rp_72(PCB_POINTER); break;
    case 73: V(0,ParserViaParam) = ag_rp_73(PCB_POINTER, V(0,ParserTTLParam)); break;
    case 74: V(0,ParserViaParam) = ag_rp_74(PCB_POINTER, V(0,ParserMaddrParam)); break;
    case 75: V(0,ParserViaParam) = ag_rp_75(PCB_POINTER, V(0,ParserReceived)); break;
    case 76: V(0,ParserViaParam) = ag_rp_76(PCB_POINTER, V(0,ParserBranch)); break;
    case 77: V(0,ParserViaParam) = ag_rp_77(PCB_POINTER, V(0,ParserPort)); break;
    case 78: V(0,ParserViaParam) = ag_rp_78(PCB_POINTER, V(0,ParserCompParam)); break;
    case 79: V(0,ParserViaParam) = ag_rp_79(PCB_POINTER); break;
    case 80: ag_rp_80(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 81: ag_rp_81(PCB_POINTER, V(0,ParserBasicToken), V(4,ParserBasicToken)); break;
    case 82: ag_rp_82(PCB_POINTER, V(0,ParserBasicToken), V(4,ParserQuotedString)); break;
    case 83: ag_rp_83(PCB_POINTER, V(0,ParserBasicToken), V(4,ParserBasicToken)); break;
    case 84: V(0,ParserTTLParam) = ag_rp_84(PCB_POINTER, V(0,ParserTtlParam)); break;
    case 85: V(0,ParserTtlParam) = ag_rp_85(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 86: V(0,ParserBasicToken) = ag_rp_86(PCB_POINTER); break;
    case 87: V(0,ParserBasicToken) = ag_rp_87(PCB_POINTER); break;
    case 88: V(0,ParserBasicToken) = ag_rp_88(PCB_POINTER); break;
    case 89: V(0,ParserPort) = ag_rp_89(PCB_POINTER, V(1,ParserPort)); break;
    case 90: V(0,ParserPort) = ag_rp_90(PCB_POINTER); break;
    case 91: V(0,ParserPort) = ag_rp_91(PCB_POINTER, V(2,ParserPort)); break;
    case 92: V(0,ParserMaddrParam) = ag_rp_92(PCB_POINTER, V(0,ParserMaddrParam)); break;
    case 93: V(0,ParserMaddrParam) = ag_rp_93(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 94: V(0,ParserReceived) = ag_rp_94(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 95: V(0,ParserBranch) = ag_rp_95(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 96: V(0,ParserCompParam) = ag_rp_96(PCB_POINTER, V(4,ParserCompParam)); break;
    case 97: V(0,ParserCompParam) = ag_rp_97(PCB_POINTER); break;
    case 98: V(0,ParserCompParam) = ag_rp_98(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 99: V(0,ParserBasicToken) = ag_rp_99(PCB_POINTER); break;
    case 100: ag_rp_100(PCB_POINTER, V(0,ParserAbsoluteUri)); break;
    case 101: V(0,ParserAbsoluteUri) = ag_rp_101(PCB_POINTER, V(0,ParserBasicToken), V(3,ParserBasicToken)); break;
    case 102: V(0,ParserBasicToken) = ag_rp_102(PCB_POINTER); break;
    case 103: V(0,ParserDisplayName) = ag_rp_103(PCB_POINTER, V(0,ParserDisplayName)); break;
    case 104: V(0,ParserBasicToken) = ag_rp_104(PCB_POINTER); break;
    case 105: V(0,ParserBasicToken) = ag_rp_105(PCB_POINTER); break;
    case 106: ag_rp_106(PCB_POINTER); break;
    case 107: ag_rp_107(PCB_POINTER); break;
    case 108: ag_rp_108(PCB_POINTER); break;
    case 109: ag_rp_109(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 110: ag_rp_110(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 111: V(0,ParserBasicToken) = ag_rp_111(PCB_POINTER); break;
    case 112: V(0,ParserBasicToken) = ag_rp_112(PCB_POINTER); break;
    case 113: ag_rp_113(PCB_POINTER); break;
    case 114: ag_rp_114(PCB_POINTER); break;
    case 115: ag_rp_115(PCB_POINTER); break;
    case 116: ag_rp_116(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 117: ag_rp_117(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 118: ag_rp_118(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 119: ag_rp_119(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 120: ag_rp_120(PCB_POINTER, V(0,ParserEnumdiParamType)); break;
    case 121: ag_rp_121(PCB_POINTER); break;
    case 122: V(0,ParserBasicToken) = ag_rp_122(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 123: V(0,ParserBasicToken) = ag_rp_123(PCB_POINTER); break;
    case 124: V(0,ParserBasicToken) = ag_rp_124(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 125: V(0,ParserBasicToken) = ag_rp_125(PCB_POINTER); break;
    case 126: V(0,ParserBasicToken) = ag_rp_126(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 127: V(0,ParserBasicToken) = ag_rp_127(PCB_POINTER); break;
    case 128: V(0,ParserBasicToken) = ag_rp_128(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 129: V(0,ParserBasicToken) = ag_rp_129(PCB_POINTER); break;
    case 130: ag_rp_130(PCB_POINTER); break;
    case 131: ag_rp_131(PCB_POINTER); break;
    case 132: ag_rp_132(PCB_POINTER); break;
    case 133: V(0,ParserEnumdiParamType) = ag_rp_133(PCB_POINTER); break;
    case 134: ag_rp_134(PCB_POINTER); break;
    case 135: ag_rp_135(PCB_POINTER, V(0,ParserAbsoluteUri)); break;
    case 136: ag_rp_136(PCB_POINTER, V(0,ParserAbsoluteUri)); break;
    case 137: ag_rp_137(PCB_POINTER); break;
    case 138: ag_rp_138(PCB_POINTER); break;
    case 139: V(0,ParserAbsoluteUri) = ag_rp_139(PCB_POINTER, V(2,ParserAbsoluteUri)); break;
    case 140: V(0,ParserAbsoluteUri) = ag_rp_140(PCB_POINTER); break;
    case 141: ag_rp_141(PCB_POINTER); break;
    case 142: ag_rp_142(PCB_POINTER); break;
    case 143: V(0,ParserAbsoluteUri) = ag_rp_143(PCB_POINTER, V(2,ParserAbsoluteUri)); break;
    case 144: V(0,ParserAbsoluteUri) = ag_rp_144(PCB_POINTER); break;
    case 145: ag_rp_145(PCB_POINTER); break;
    case 146: ag_rp_146(PCB_POINTER, V(2,ParserOptionalHeaders)); break;
    case 147: ag_rp_147(PCB_POINTER, V(2,ParserOptionalHeaders)); break;
    case 148: ag_rp_148(PCB_POINTER, V(3,ParserOptionalHeaders)); break;
    case 149: ag_rp_149(PCB_POINTER, V(3,ParserOptionalHeaders)); break;
    case 150: ag_rp_150(PCB_POINTER); break;
    case 151: ag_rp_151(PCB_POINTER, V(3,ParserOptionalHeaders)); break;
    case 152: ag_rp_152(PCB_POINTER, V(3,ParserOptionalHeaders)); break;
    case 153: ag_rp_153(PCB_POINTER, V(3,ParserOptionalHeaders)); break;
    case 154: ag_rp_154(PCB_POINTER, V(3,ParserOptionalHeaders)); break;
    case 155: ag_rp_155(PCB_POINTER, V(0,ParserDisplayName), V(2,ParserBasicToken)); break;
    case 156: ag_rp_156(PCB_POINTER); break;
    case 157: ag_rp_157(PCB_POINTER); break;
    case 158: ag_rp_158(PCB_POINTER); break;
    case 159: ag_rp_159(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 160: ag_rp_160(PCB_POINTER, V(0,ParserBasicToken), V(2,ParserBasicToken)); break;
    case 161: V(0,ParserBasicToken) = ag_rp_161(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 162: V(0,ParserBasicToken) = ag_rp_162(PCB_POINTER); break;
    case 163: V(0,ParserBasicToken) = ag_rp_163(PCB_POINTER); break;
    case 164: ag_rp_164(PCB_POINTER); break;
    case 165: ag_rp_165(PCB_POINTER); break;
    case 166: ag_rp_166(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 167: ag_rp_167(PCB_POINTER, V(0,ParserBasicToken), V(2,ParserPort)); break;
    case 168: V(0,ParserBasicToken) = ag_rp_168(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 169: V(0,ParserBasicToken) = ag_rp_169(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 170: V(0,ParserBasicToken) = ag_rp_170(PCB_POINTER); break;
    case 171: V(0,ParserBasicToken) = ag_rp_171(PCB_POINTER); break;
    case 172: V(0,ParserBasicToken) = ag_rp_172(PCB_POINTER); break;
    case 173: ag_rp_173(PCB_POINTER); break;
    case 174: ag_rp_174(PCB_POINTER); break;
    case 175: ag_rp_175(PCB_POINTER); break;
    case 176: ag_rp_176(PCB_POINTER); break;
    case 177: ag_rp_177(PCB_POINTER, V(0,ParserTransport)); break;
    case 178: ag_rp_178(PCB_POINTER, V(0,ParserUserParam)); break;
    case 179: ag_rp_179(PCB_POINTER, V(0,ParserTtlParam)); break;
    case 180: ag_rp_180(PCB_POINTER, V(0,ParserMaddrParam)); break;
    case 181: ag_rp_181(PCB_POINTER, V(0,ParserMethod)); break;
    case 182: ag_rp_182(PCB_POINTER, V(0,ParserLrParamType)); break;
    case 183: ag_rp_183(PCB_POINTER, V(0,ParserCompParam)); break;
    case 184: ag_rp_184(PCB_POINTER, V(0,ParserTokenizedByParam)); break;
    case 185: ag_rp_185(PCB_POINTER, V(0,RvBool)); break;
    case 186: ag_rp_186(PCB_POINTER); break;
    case 187: V(0,ParserTransport) = ag_rp_187(PCB_POINTER, V(4,ParserTransport)); break;
    case 188: V(0,ParserUserParam) = ag_rp_188(PCB_POINTER, V(4,RvSipUserParam)); break;
    case 189: V(0,ParserUserParam) = ag_rp_189(PCB_POINTER, V(4,ParserOtherUser)); break;
    case 190: V(0,RvSipUserParam) = ag_rp_190(PCB_POINTER); break;
    case 191: V(0,RvSipUserParam) = ag_rp_191(PCB_POINTER); break;
    case 192: V(0,ParserOtherUser) = ag_rp_192(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 193: V(0,ParserTtlParam) = ag_rp_193(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 194: V(0,ParserMaddrParam) = ag_rp_194(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 195: V(0,ParserMethod) = ag_rp_195(PCB_POINTER, V(4,ParserMethod)); break;
    case 196: V(0,ParserLrParamType) = ag_rp_196(PCB_POINTER); break;
    case 197: V(0,ParserLrParamType) = ag_rp_197(PCB_POINTER); break;
    case 198: V(0,ParserLrParamType) = ag_rp_198(PCB_POINTER); break;
    case 199: V(0,ParserLrParamType) = ag_rp_199(PCB_POINTER); break;
    case 200: V(0,ParserCompParam) = ag_rp_200(PCB_POINTER, V(4,ParserCompParam)); break;
    case 201: V(0,ParserTokenizedByParam) = ag_rp_201(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 202: V(0,RvBool) = ag_rp_202(PCB_POINTER); break;
    case 203: ag_rp_203(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 204: ag_rp_204(PCB_POINTER, V(0,ParserBasicToken), V(1,ParserBasicToken)); break;
    case 205: V(0,ParserBasicToken) = ag_rp_205(PCB_POINTER); break;
    case 206: V(0,ParserBasicToken) = ag_rp_206(PCB_POINTER, V(2,ParserBasicToken)); break;
    case 207: V(0,ParserOptionalHeaders) = ag_rp_207(PCB_POINTER); break;
    case 208: V(0,ParserOptionalHeaders) = ag_rp_208(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 209: V(0,ParserBasicToken) = ag_rp_209(PCB_POINTER); break;
    case 210: V(0,ParserBasicToken) = ag_rp_210(PCB_POINTER); break;
    case 211: V(0,ParserBasicToken) = ag_rp_211(PCB_POINTER); break;
    case 212: V(0,ParserBasicToken) = ag_rp_212(PCB_POINTER); break;
    case 213: V(0,ParserBasicToken) = ag_rp_213(PCB_POINTER); break;
    case 214: ag_rp_214(PCB_POINTER); break;
    case 215: ag_rp_215(PCB_POINTER, V(0,ParserMethod), V(4,ParserSipVersion)); break;
    case 216: V(0,ParserMethod) = ag_rp_216(PCB_POINTER); break;
    case 217: V(0,ParserMethod) = ag_rp_217(PCB_POINTER); break;
    case 218: V(0,ParserMethod) = ag_rp_218(PCB_POINTER); break;
    case 219: V(0,ParserMethod) = ag_rp_219(PCB_POINTER); break;
    case 220: V(0,ParserMethod) = ag_rp_220(PCB_POINTER); break;
    case 221: V(0,ParserMethod) = ag_rp_221(PCB_POINTER); break;
    case 222: V(0,ParserMethod) = ag_rp_222(PCB_POINTER); break;
    case 223: V(0,ParserMethod) = ag_rp_223(PCB_POINTER); break;
    case 224: V(0,ParserMethod) = ag_rp_224(PCB_POINTER); break;
    case 225: V(0,ParserMethod) = ag_rp_225(PCB_POINTER); break;
    case 226: V(0,ParserMethod) = ag_rp_226(PCB_POINTER); break;
    case 227: V(0,ParserMethod) = ag_rp_227(PCB_POINTER); break;
    case 228: V(0,ParserMethod) = ag_rp_228(PCB_POINTER); break;
    case 229: V(0,ParserMethod) = ag_rp_229(PCB_POINTER, V(0,ParserMethod)); break;
    case 230: ag_rp_230(PCB_POINTER); break;
    case 231: ag_rp_231(PCB_POINTER, V(0,ParserAbsoluteUri)); break;
    case 232: ag_rp_232(PCB_POINTER, V(0,ParserAbsoluteUri)); break;
    case 233: ag_rp_233(PCB_POINTER); break;
    case 234: ag_rp_234(PCB_POINTER, V(0,ParserAbsoluteUri)); break;
    case 235: ag_rp_235(PCB_POINTER); break;
    case 236: ag_rp_236(PCB_POINTER); break;
    case 237: ag_rp_237(PCB_POINTER, V(2,ParserPartyParams)); break;
    case 238: ag_rp_238(PCB_POINTER); break;
    case 239: ag_rp_239(PCB_POINTER, V(1,ParserPartyParams)); break;
    case 240: ag_rp_240(PCB_POINTER, V(0,ParserDisplayName)); break;
    case 241: ag_rp_241(PCB_POINTER); break;
    case 242: V(0,ParserDisplayName) = ag_rp_242(PCB_POINTER, V(0,ParserQuotedString)); break;
    case 243: V(0,ParserDisplayName) = ag_rp_243(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 244: V(0,ParserBasicToken) = ag_rp_244(PCB_POINTER); break;
    case 245: V(0,ParserBasicToken) = ag_rp_245(PCB_POINTER); break;
    case 246: V(0,ParserTagParam) = ag_rp_246(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 247: V(0,ParserPartyParams) = ag_rp_247(PCB_POINTER, V(2,ParserPartyParams)); break;
    case 248: V(0,ParserPartyParams) = ag_rp_248(PCB_POINTER, V(0,ParserPartyParams), V(3,ParserPartyParams)); break;
    case 249: V(0,ParserPartyParams) = ag_rp_249(PCB_POINTER, V(0,ParserTagParam)); break;
    case 250: V(0,ParserPartyParams) = ag_rp_250(PCB_POINTER); break;
    case 251: ag_rp_251(PCB_POINTER); break;
    case 252: ag_rp_252(PCB_POINTER); break;
    case 253: ag_rp_253(PCB_POINTER); break;
    case 254: ag_rp_254(PCB_POINTER); break;
    case 255: ag_rp_255(PCB_POINTER); break;
    case 256: ag_rp_256(PCB_POINTER); break;
    case 257: ag_rp_257(PCB_POINTER); break;
    case 258: ag_rp_258(PCB_POINTER, V(0,ParserQVal)); break;
    case 259: ag_rp_259(PCB_POINTER, V(0,ParserContactActionType)); break;
    case 260: ag_rp_260(PCB_POINTER); break;
    case 261: ag_rp_261(PCB_POINTER); break;
    case 262: V(0,ParserQVal) = ag_rp_262(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 263: V(0,ParserBasicToken) = ag_rp_263(PCB_POINTER); break;
    case 264: V(0,ParserBasicToken) = ag_rp_264(PCB_POINTER); break;
    case 265: V(0,ParserContactActionType) = ag_rp_265(PCB_POINTER, V(4,ParserContactActionType)); break;
    case 266: V(0,ParserContactActionType) = ag_rp_266(PCB_POINTER); break;
    case 267: V(0,ParserContactActionType) = ag_rp_267(PCB_POINTER); break;
    case 268: ag_rp_268(PCB_POINTER); break;
    case 269: ag_rp_269(PCB_POINTER, V(0,ParserDeltaSeconds)); break;
    case 270: ag_rp_270(PCB_POINTER, V(2,ParserSipDate)); break;
    case 271: ag_rp_271(PCB_POINTER, V(0,ParserDeltaSeconds)); break;
    case 272: ag_rp_272(PCB_POINTER, V(0,ParserSipDate)); break;
    case 273: V(0,ParserDeltaSeconds) = ag_rp_273(PCB_POINTER); break;
    case 274: ag_rp_274(PCB_POINTER, V(0,ParserSipDate)); break;
    case 275: V(0,ParserSipDate) = ag_rp_275(PCB_POINTER, V(0,ParserDDMMYY), V(2,ParserTime)); break;
    case 276: V(0,ParserSipDate) = ag_rp_276(PCB_POINTER, V(0,ParserSipDate)); break;
    case 277: V(0,ParserSipDate) = ag_rp_277(PCB_POINTER, V(0,RvSipDateWeekDay), V(4,ParserDDMMYY), V(6,ParserTime)); break;
    case 278: V(0,RvSipDateWeekDay) = ag_rp_278(PCB_POINTER); break;
    case 279: V(0,RvSipDateWeekDay) = ag_rp_279(PCB_POINTER); break;
    case 280: V(0,RvSipDateWeekDay) = ag_rp_280(PCB_POINTER); break;
    case 281: V(0,RvSipDateWeekDay) = ag_rp_281(PCB_POINTER); break;
    case 282: V(0,RvSipDateWeekDay) = ag_rp_282(PCB_POINTER); break;
    case 283: V(0,RvSipDateWeekDay) = ag_rp_283(PCB_POINTER); break;
    case 284: V(0,RvSipDateWeekDay) = ag_rp_284(PCB_POINTER); break;
    case 285: V(0,ParserDDMMYY) = ag_rp_285(PCB_POINTER, V(0,ParserInt32), V(2,RvSipDateMonth), V(4,ParserInt32)); break;
    case 286: V(0,ParserInt32) = ag_rp_286(PCB_POINTER, V(0,int)); break;
    case 287: V(0,ParserInt32) = ag_rp_287(PCB_POINTER, V(0,int), V(1,int)); break;
    case 288: V(0,RvSipDateMonth) = ag_rp_288(PCB_POINTER); break;
    case 289: V(0,RvSipDateMonth) = ag_rp_289(PCB_POINTER); break;
    case 290: V(0,RvSipDateMonth) = ag_rp_290(PCB_POINTER); break;
    case 291: V(0,RvSipDateMonth) = ag_rp_291(PCB_POINTER); break;
    case 292: V(0,RvSipDateMonth) = ag_rp_292(PCB_POINTER); break;
    case 293: V(0,RvSipDateMonth) = ag_rp_293(PCB_POINTER); break;
    case 294: V(0,RvSipDateMonth) = ag_rp_294(PCB_POINTER); break;
    case 295: V(0,RvSipDateMonth) = ag_rp_295(PCB_POINTER); break;
    case 296: V(0,RvSipDateMonth) = ag_rp_296(PCB_POINTER); break;
    case 297: V(0,RvSipDateMonth) = ag_rp_297(PCB_POINTER); break;
    case 298: V(0,RvSipDateMonth) = ag_rp_298(PCB_POINTER); break;
    case 299: V(0,RvSipDateMonth) = ag_rp_299(PCB_POINTER); break;
    case 300: V(0,ParserInt32) = ag_rp_300(PCB_POINTER, V(0,int), V(1,int)); break;
    case 301: V(0,ParserInt32) = ag_rp_301(PCB_POINTER, V(0,int), V(1,int), V(2,int)); break;
    case 302: V(0,ParserInt32) = ag_rp_302(PCB_POINTER, V(0,int), V(1,int), V(2,int), V(3,int)); break;
    case 303: V(0,ParserTime) = ag_rp_303(PCB_POINTER, V(0,ParserInt32), V(2,ParserInt32), V(4,ParserInt32), V(6,ParserTimeZone)); break;
    case 304: V(0,ParserInt32) = ag_rp_304(PCB_POINTER, V(0,int), V(1,int)); break;
    case 305: V(0,ParserInt32) = ag_rp_305(PCB_POINTER, V(0,int), V(1,int)); break;
    case 306: V(0,ParserInt32) = ag_rp_306(PCB_POINTER, V(0,int), V(1,int)); break;
    case 307: V(0,ParserTimeZone) = ag_rp_307(PCB_POINTER); break;
    case 308: V(0,ParserTimeZone) = ag_rp_308(PCB_POINTER); break;
    case 309: V(0,ParserTimeZone) = ag_rp_309(PCB_POINTER); break;
    case 310: ag_rp_310(PCB_POINTER, V(0,ParserMethod)); break;
    case 311: ag_rp_311(PCB_POINTER, V(0,ParserMethod), V(4,ParserBasicToken)); break;
    case 312: ag_rp_312(PCB_POINTER); break;
    case 313: ag_rp_313(PCB_POINTER); break;
    case 314: ag_rp_314(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 315: ag_rp_315(PCB_POINTER); break;
    case 316: ag_rp_316(PCB_POINTER); break;
    case 317: ag_rp_317(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 318: ag_rp_318(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 319: ag_rp_319(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 320: ag_rp_320(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 321: ag_rp_321(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 322: ag_rp_322(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 323: ag_rp_323(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 324: ag_rp_324(PCB_POINTER, V(0,RvSipAuthStale)); break;
    case 325: ag_rp_325(PCB_POINTER, V(0,ParserAlgorithm)); break;
    case 326: ag_rp_326(PCB_POINTER, V(0,ParserQopValue)); break;
    case 327: ag_rp_327(PCB_POINTER); break;
    case 328: V(0,ParserBasicToken) = ag_rp_328(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 329: V(0,ParserBasicToken) = ag_rp_329(PCB_POINTER, V(0,ParserQuotedString)); break;
    case 330: V(0,ParserBasicToken) = ag_rp_330(PCB_POINTER); break;
    case 331: V(0,ParserBasicToken) = ag_rp_331(PCB_POINTER); break;
    case 332: ag_rp_332(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 333: ag_rp_333(PCB_POINTER); break;
    case 334: ag_rp_334(PCB_POINTER); break;
    case 335: V(0,ParserBasicToken) = ag_rp_335(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 336: V(0,ParserBasicToken) = ag_rp_336(PCB_POINTER, V(0,ParserQuotedString)); break;
    case 337: V(0,ParserBasicToken) = ag_rp_337(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 338: V(0,ParserBasicToken) = ag_rp_338(PCB_POINTER, V(0,ParserQuotedString)); break;
    case 339: V(0,RvSipAuthStale) = ag_rp_339(PCB_POINTER); break;
    case 340: V(0,RvSipAuthStale) = ag_rp_340(PCB_POINTER); break;
    case 341: V(0,ParserAlgorithm) = ag_rp_341(PCB_POINTER, V(1,ParserAlgorithm)); break;
    case 342: ag_rp_342(PCB_POINTER); break;
    case 343: ag_rp_343(PCB_POINTER); break;
    case 344: ag_rp_344(PCB_POINTER); break;
    case 345: ag_rp_345(PCB_POINTER); break;
    case 346: V(0,ParserAlgorithm) = ag_rp_346(PCB_POINTER, V(2,ParserBasicToken), V(4,ParserAlgorithm)); break;
    case 347: V(0,ParserAlgorithm) = ag_rp_347(PCB_POINTER, V(1,ParserAlgorithm)); break;
    case 348: ag_rp_348(PCB_POINTER); break;
    case 349: V(0,ParserAlgorithm) = ag_rp_349(PCB_POINTER); break;
    case 350: V(0,ParserAlgorithm) = ag_rp_350(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 351: ag_rp_351(PCB_POINTER); break;
    case 352: ag_rp_352(PCB_POINTER); break;
    case 353: V(0,ParserBasicToken) = ag_rp_353(PCB_POINTER); break;
    case 354: V(0,ParserQopValue) = ag_rp_354(PCB_POINTER, V(5,ParserQopValue)); break;
    case 355: V(0,ParserQopValue) = ag_rp_355(PCB_POINTER, V(0,ParserAuthQopOptions)); break;
    case 356: V(0,ParserQopValue) = ag_rp_356(PCB_POINTER, V(0,ParserQopValue), V(4,ParserAuthQopOptions)); break;
    case 357: V(0,ParserAuthQopOptions) = ag_rp_357(PCB_POINTER); break;
    case 358: V(0,ParserAuthQopOptions) = ag_rp_358(PCB_POINTER); break;
    case 359: V(0,ParserAuthQopOptions) = ag_rp_359(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 360: V(0,ParserBasicToken) = ag_rp_360(PCB_POINTER, V(3,ParserBasicToken)); break;
    case 361: V(0,ParserBasicToken) = ag_rp_361(PCB_POINTER, V(3,ParserBasicToken)); break;
    case 362: V(0,ParserBasicToken) = ag_rp_362(PCB_POINTER); break;
    case 363: V(0,ParserBasicToken) = ag_rp_363(PCB_POINTER); break;
    case 364: V(0,ParserBasicToken) = ag_rp_364(PCB_POINTER); break;
    case 365: V(0,ParserBasicToken) = ag_rp_365(PCB_POINTER); break;
    case 366: V(0,ParserBasicToken) = ag_rp_366(PCB_POINTER); break;
    case 367: ag_rp_367(PCB_POINTER, V(0,ParserBasicToken), V(2,ParserGenericParamDataVal)); break;
    case 368: ag_rp_368(PCB_POINTER); break;
    case 369: ag_rp_369(PCB_POINTER); break;
    case 370: ag_rp_370(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 371: ag_rp_371(PCB_POINTER); break;
    case 372: ag_rp_372(PCB_POINTER); break;
    case 373: ag_rp_373(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 374: ag_rp_374(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 375: ag_rp_375(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 376: ag_rp_376(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 377: ag_rp_377(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 378: ag_rp_378(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 379: ag_rp_379(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 380: ag_rp_380(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 381: ag_rp_381(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 382: ag_rp_382(PCB_POINTER, V(0,RvSipAuthIntegrityProtected)); break;
    case 383: ag_rp_383(PCB_POINTER, V(0,RvSipAuthQopOption)); break;
    case 384: ag_rp_384(PCB_POINTER); break;
    case 385: ag_rp_385(PCB_POINTER, V(0,ParserAlgorithm)); break;
    case 386: V(0,ParserBasicToken) = ag_rp_386(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 387: V(0,ParserBasicToken) = ag_rp_387(PCB_POINTER, V(0,ParserQuotedString)); break;
    case 388: V(0,RvSipAuthQopOption) = ag_rp_388(PCB_POINTER, V(5,ParserAuthQopOptions)); break;
    case 389: V(0,ParserBasicToken) = ag_rp_389(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 390: V(0,ParserBasicToken) = ag_rp_390(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 391: V(0,ParserBasicToken) = ag_rp_391(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 392: V(0,ParserBasicToken) = ag_rp_392(PCB_POINTER); break;
    case 393: V(0,ParserBasicToken) = ag_rp_393(PCB_POINTER); break;
    case 394: V(0,ParserBasicToken) = ag_rp_394(PCB_POINTER); break;
    case 395: V(0,ParserBasicToken) = ag_rp_395(PCB_POINTER); break;
    case 396: V(0,ParserBasicToken) = ag_rp_396(PCB_POINTER); break;
    case 397: V(0,ParserBasicToken) = ag_rp_397(PCB_POINTER); break;
    case 398: V(0,ParserBasicToken) = ag_rp_398(PCB_POINTER); break;
    case 399: V(0,ParserBasicToken) = ag_rp_399(PCB_POINTER); break;
    case 400: V(0,ParserBasicToken) = ag_rp_400(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 401: V(0,ParserBasicToken) = ag_rp_401(PCB_POINTER); break;
    case 402: V(0,ParserBasicToken) = ag_rp_402(PCB_POINTER); break;
    case 403: V(0,ParserBasicToken) = ag_rp_403(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 404: V(0,ParserBasicToken) = ag_rp_404(PCB_POINTER); break;
    case 405: V(0,RvSipAuthIntegrityProtected) = ag_rp_405(PCB_POINTER); break;
    case 406: V(0,RvSipAuthIntegrityProtected) = ag_rp_406(PCB_POINTER); break;
    case 407: ag_rp_407(PCB_POINTER); break;
    case 408: ag_rp_408(PCB_POINTER); break;
    case 409: ag_rp_409(PCB_POINTER); break;
    case 410: ag_rp_410(PCB_POINTER); break;
    case 411: ag_rp_411(PCB_POINTER); break;
    case 412: ag_rp_412(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 413: ag_rp_413(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 414: ag_rp_414(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 415: ag_rp_415(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 416: ag_rp_416(PCB_POINTER); break;
    case 417: ag_rp_417(PCB_POINTER, V(5,ParserAuthQopOptions)); break;
    case 418: V(0,ParserBasicToken) = ag_rp_418(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 419: V(0,ParserBasicToken) = ag_rp_419(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 420: V(0,ParserBasicToken) = ag_rp_420(PCB_POINTER); break;
  }
  (PCB).la_ptr = (PCB).pointer;
}
static int ag_action_1_r_proc(PCB_DECL);
static int ag_action_2_r_proc(PCB_DECL);
static int ag_action_3_r_proc(PCB_DECL);
static int ag_action_4_r_proc(PCB_DECL);
static int ag_action_1_s_proc(PCB_DECL);
static int ag_action_3_s_proc(PCB_DECL);
static int ag_action_1_proc(PCB_DECL);
static int ag_action_2_proc(PCB_DECL);
static int ag_action_3_proc(PCB_DECL);
static int ag_action_4_proc(PCB_DECL);
static int ag_action_5_proc(PCB_DECL);
static int ag_action_6_proc(PCB_DECL);
static int ag_action_7_proc(PCB_DECL);
static int ag_action_8_proc(PCB_DECL);
static int ag_action_9_proc(PCB_DECL);
static int ag_action_10_proc(PCB_DECL);
static int ag_action_11_proc(PCB_DECL);
static int ag_action_8_proc(PCB_DECL);
static int (*const  ag_r_procs_scan[])(PCB_DECL) = {
  ag_action_1_r_proc,
  ag_action_2_r_proc,
  ag_action_3_r_proc,
  ag_action_4_r_proc
};
static int (*const  ag_s_procs_scan[])(PCB_DECL) = {
  ag_action_1_s_proc,
  ag_action_2_r_proc,
  ag_action_3_s_proc,
  ag_action_4_r_proc
};
static int (*const  ag_gt_procs_scan[])(PCB_DECL) = {
  ag_action_1_proc,
  ag_action_2_proc,
  ag_action_3_proc,
  ag_action_4_proc,
  ag_action_5_proc,
  ag_action_6_proc,
  ag_action_7_proc,
  ag_action_8_proc,
  ag_action_9_proc,
  ag_action_10_proc,
  ag_action_11_proc,
  ag_action_8_proc
};
static int ag_action_10_proc(PCB_DECL) {
  int ag_t = (PCB).token_number;
  do {
    ag_track(PCB_POINTER);
    (PCB).token_number = (SipParser_token_type) AG_TCV(INPUT_CODE(*(PCB).la_ptr));
    (PCB).la_ptr++;
    if (ag_key_index[(PCB).sn]) {
      unsigned ag_k = ag_key_index[(PCB).sn];
      int ag_ch = CONVERT_CASE(INPUT_CODE(*(PCB).pointer));
      while (ag_key_ch[ag_k] < ag_ch) ag_k++;
      if (ag_key_ch[ag_k] == ag_ch) ag_get_key_word((PCB_TYPE *)PCB_POINTER, ag_k);
    }
  } while ((PCB).token_number == (SipParser_token_type) ag_t);
  (PCB).la_ptr =  (PCB).pointer;
  return 1;
}
static int ag_action_11_proc(PCB_DECL) {
  int ag_t = (PCB).token_number;
  do {
    (*(int *) &(PCB).vs[(PCB).ssx]) = *(PCB).pointer;
    (PCB).ssx--;
    ag_track(PCB_POINTER);
    ag_ra(PCB_POINTER);
    if ((PCB).exit_flag != AG_RUNNING_CODE) return 0;
    (PCB).ssx++;
    (PCB).token_number = (SipParser_token_type) AG_TCV(INPUT_CODE(*(PCB).la_ptr));
    (PCB).la_ptr++;
    if (ag_key_index[(PCB).sn]) {
      unsigned ag_k = ag_key_index[(PCB).sn];
      int ag_ch = CONVERT_CASE(INPUT_CODE(*(PCB).pointer));
      while (ag_key_ch[ag_k] < ag_ch) ag_k++;
      if (ag_key_ch[ag_k] == ag_ch) ag_get_key_word((PCB_TYPE *)PCB_POINTER, ag_k);
    }
  }
  while ((PCB).token_number == (SipParser_token_type) ag_t);
  (PCB).la_ptr =  (PCB).pointer;
  return 1;
}
static int ag_action_3_r_proc(PCB_DECL) {
  int ag_sd = ag_fl[(PCB).ag_ap] - 1;
  if (ag_sd) (PCB).sn = (PCB).ss[(PCB).ssx -= ag_sd];
  (PCB).reduction_token = (SipParser_token_type) ag_ptt[(PCB).ag_ap];
  ag_ra(PCB_POINTER);
  return (PCB).exit_flag == AG_RUNNING_CODE;
}
static int ag_action_3_s_proc(PCB_DECL) {
  int ag_sd = ag_fl[(PCB).ag_ap] - 1;
  if (ag_sd) (PCB).sn = (PCB).ss[(PCB).ssx -= ag_sd];
  (PCB).reduction_token = (SipParser_token_type) ag_ptt[(PCB).ag_ap];
  ag_ra(PCB_POINTER);
  return (PCB).exit_flag == AG_RUNNING_CODE;;
}
static int ag_action_4_r_proc(PCB_DECL) {
  int ag_sd = ag_fl[(PCB).ag_ap] - 1;
  if (ag_sd) (PCB).sn = (PCB).ss[(PCB).ssx -= ag_sd];
  (PCB).reduction_token = (SipParser_token_type) ag_ptt[(PCB).ag_ap];
  return 1;
}
static int ag_action_2_proc(PCB_DECL) {
  if ((PCB).ssx >= 43) {
    (PCB).exit_flag = AG_STACK_ERROR_CODE;
    PARSER_STACK_OVERFLOW;
  }
  (*(int *) &(PCB).vs[(PCB).ssx]) = *(PCB).pointer;
  (PCB).ss[(PCB).ssx] = (PCB).sn;
  (PCB).ssx++;
  (PCB).sn = (PCB).ag_ap;
  ag_track(PCB_POINTER);
  return 0;
}
static int ag_action_9_proc(PCB_DECL) {
  if ((PCB).ssx >= 43) {
    (PCB).exit_flag = AG_STACK_ERROR_CODE;
    PARSER_STACK_OVERFLOW;
  }
  (PCB).vs[(PCB).ssx] = ag_null_value;
  (PCB).ss[(PCB).ssx] = (PCB).sn;
  (PCB).ssx++;
  (PCB).sn = (PCB).ag_ap;
  (PCB).la_ptr =  (PCB).pointer;
  return (PCB).exit_flag == AG_RUNNING_CODE;
}
static int ag_action_2_r_proc(PCB_DECL) {
  (PCB).ssx++;
  (PCB).sn = (PCB).ag_ap;
  return 0;
}
static int ag_action_7_proc(PCB_DECL) {
  --(PCB).ssx;
  (PCB).la_ptr =  (PCB).pointer;
  (PCB).exit_flag = AG_SUCCESS_CODE;
  return 0;
}
static int ag_action_1_proc(PCB_DECL) {
  ag_track(PCB_POINTER);
  (PCB).exit_flag = AG_SUCCESS_CODE;
  return 0;
}
static int ag_action_1_r_proc(PCB_DECL) {
  (PCB).exit_flag = AG_SUCCESS_CODE;
  return 0;
}
static int ag_action_1_s_proc(PCB_DECL) {
  (PCB).exit_flag = AG_SUCCESS_CODE;
  return 0;
}
static int ag_action_4_proc(PCB_DECL) {
  int ag_sd = ag_fl[(PCB).ag_ap] - 1;
  (PCB).reduction_token = (SipParser_token_type) ag_ptt[(PCB).ag_ap];
  (*(int *) &(PCB).vs[(PCB).ssx]) = *(PCB).pointer;
  if (ag_sd) (PCB).sn = (PCB).ss[(PCB).ssx -= ag_sd];
  else (PCB).ss[(PCB).ssx] = (PCB).sn;
  ag_track(PCB_POINTER);
  while ((PCB).exit_flag == AG_RUNNING_CODE) {
    unsigned ag_t1 = ag_sbe[(PCB).sn] + 1;
    unsigned ag_t2 = ag_sbt[(PCB).sn+1] - 1;
    do {
      unsigned ag_tx = (ag_t1 + ag_t2)/2;
      if (ag_tstt[ag_tx] < (const unsigned short)(PCB).reduction_token) ag_t1 = ag_tx + 1;
      else ag_t2 = ag_tx;
    } while (ag_t1 < ag_t2);
    (PCB).ag_ap = ag_pstt[ag_t1];
    if ((ag_s_procs_scan[ag_astt[ag_t1]])(PCB_POINTER) == 0) break;
  }
  return 0;
}
static int ag_action_3_proc(PCB_DECL) {
  int ag_sd = ag_fl[(PCB).ag_ap] - 1;
  (*(int *) &(PCB).vs[(PCB).ssx]) = *(PCB).pointer;
  if (ag_sd) (PCB).sn = (PCB).ss[(PCB).ssx -= ag_sd];
  else (PCB).ss[(PCB).ssx] = (PCB).sn;
  ag_track(PCB_POINTER);
  (PCB).reduction_token = (SipParser_token_type) ag_ptt[(PCB).ag_ap];
  ag_ra(PCB_POINTER);
  while ((PCB).exit_flag == AG_RUNNING_CODE) {
    unsigned ag_t1 = ag_sbe[(PCB).sn] + 1;
    unsigned ag_t2 = ag_sbt[(PCB).sn+1] - 1;
    do {
      unsigned ag_tx = (ag_t1 + ag_t2)/2;
      if (ag_tstt[ag_tx] < (const unsigned short)(PCB).reduction_token) ag_t1 = ag_tx + 1;
      else ag_t2 = ag_tx;
    } while (ag_t1 < ag_t2);
    (PCB).ag_ap = ag_pstt[ag_t1];
    if ((ag_s_procs_scan[ag_astt[ag_t1]])(PCB_POINTER) == 0) break;
  }
  return 0;
}
static int ag_action_8_proc(PCB_DECL) {
  (PCB).la_ptr =  (PCB).pointer;
  (PCB).exit_flag = AG_SYNTAX_ERROR_CODE;
  SYNTAX_ERROR;
  {(PCB).la_ptr = (PCB).pointer + 1; ag_track(PCB_POINTER);}
  return (PCB).exit_flag == AG_RUNNING_CODE;
}
static int ag_action_5_proc(PCB_DECL) {
  int ag_sd = ag_fl[(PCB).ag_ap];
  if (ag_sd) (PCB).sn = (PCB).ss[(PCB).ssx -= ag_sd];
  else {
    if ((PCB).ssx >= 43) {
      (PCB).exit_flag = AG_STACK_ERROR_CODE;
      PARSER_STACK_OVERFLOW;
    }
    (PCB).ss[(PCB).ssx] = (PCB).sn;
  }
  (PCB).la_ptr =  (PCB).pointer;
  (PCB).reduction_token = (SipParser_token_type) ag_ptt[(PCB).ag_ap];
  ag_ra(PCB_POINTER);
  while ((PCB).exit_flag == AG_RUNNING_CODE) {
    unsigned ag_t1 = ag_sbe[(PCB).sn] + 1;
    unsigned ag_t2 = ag_sbt[(PCB).sn+1] - 1;
    do {
      unsigned ag_tx = (ag_t1 + ag_t2)/2;
      if (ag_tstt[ag_tx] < (const unsigned short)(PCB).reduction_token) ag_t1 = ag_tx + 1;
      else ag_t2 = ag_tx;
    } while (ag_t1 < ag_t2);
    (PCB).ag_ap = ag_pstt[ag_t1];
    if ((ag_r_procs_scan[ag_astt[ag_t1]])(PCB_POINTER) == 0) break;
  }
  return (PCB).exit_flag == AG_RUNNING_CODE;
}
static int ag_action_6_proc(PCB_DECL) {
  int ag_sd = ag_fl[(PCB).ag_ap];
  (PCB).reduction_token = (SipParser_token_type) ag_ptt[(PCB).ag_ap];
  if (ag_sd) {
    (PCB).sn = (PCB).ss[(PCB).ssx -= ag_sd];
  }
  else {
    if ((PCB).ssx >= 43) {
      (PCB).exit_flag = AG_STACK_ERROR_CODE;
      PARSER_STACK_OVERFLOW;
    }
    (PCB).vs[(PCB).ssx] = ag_null_value;
    (PCB).ss[(PCB).ssx] = (PCB).sn;
  }
  (PCB).la_ptr =  (PCB).pointer;
  while ((PCB).exit_flag == AG_RUNNING_CODE) {
    unsigned ag_t1 = ag_sbe[(PCB).sn] + 1;
    unsigned ag_t2 = ag_sbt[(PCB).sn+1] - 1;
    do {
      unsigned ag_tx = (ag_t1 + ag_t2)/2;
      if (ag_tstt[ag_tx] < (const unsigned short)(PCB).reduction_token) ag_t1 = ag_tx + 1;
      else ag_t2 = ag_tx;
    } while (ag_t1 < ag_t2);
    (PCB).ag_ap = ag_pstt[ag_t1];
    if ((ag_r_procs_scan[ag_astt[ag_t1]])(PCB_POINTER) == 0) break;
  }
  return (PCB).exit_flag == AG_RUNNING_CODE;
}
void init_SipParser(SipParser_pcb_type *PCB_POINTER) {
  (PCB).la_ptr = (PCB).pointer;
  (PCB).error_message = "Syntax Error";
  (PCB).ss[0] = (PCB).sn = (PCB).ssx = 0;
  (PCB).exit_flag = AG_RUNNING_CODE;
  (PCB).line = FIRST_LINE;
  (PCB).column = FIRST_COLUMN;
}
void SipParser(SipParser_pcb_type *PCB_POINTER) {
  init_SipParser(PCB_POINTER);
  (PCB).exit_flag = AG_RUNNING_CODE;
  while ((PCB).exit_flag == AG_RUNNING_CODE) {
    unsigned ag_t1 = ag_sbt[(PCB).sn];
    if (ag_tstt[ag_t1]) {
      unsigned ag_t2 = ag_sbe[(PCB).sn] - 1;
      (PCB).token_number = (SipParser_token_type) AG_TCV(INPUT_CODE(*(PCB).la_ptr));
      (PCB).la_ptr++;
      if (ag_key_index[(PCB).sn]) {
        unsigned ag_k = ag_key_index[(PCB).sn];
        int ag_ch = CONVERT_CASE(INPUT_CODE(*(PCB).pointer));
        while (ag_key_ch[ag_k] < ag_ch) ag_k++;
        if (ag_key_ch[ag_k] == ag_ch) ag_get_key_word((PCB_TYPE *)PCB_POINTER, ag_k);
      }
      do {
        unsigned ag_tx = (ag_t1 + ag_t2)/2;
        if (ag_tstt[ag_tx] > (const unsigned short)(PCB).token_number)
          ag_t1 = ag_tx + 1;
        else ag_t2 = ag_tx;
      } while (ag_t1 < ag_t2);
      if (ag_tstt[ag_t1] != (const unsigned short)(PCB).token_number)
        ag_t1 = ag_sbe[(PCB).sn];
    }
    (PCB).ag_ap = ag_pstt[ag_t1];
    (ag_gt_procs_scan[ag_astt[ag_t1]])((PCB_TYPE *)PCB_POINTER);
  }
}
#elif defined(RV_SIP_AUTH_ON) && defined(RV_SIP_PRIMITIVES) && !defined(RV_SIP_IMS_DCS_HEADER_SUPPORT) && !defined(RV_SIP_IMS_HEADER_SUPPORT) && !defined(RV_SIP_EXTENDED_HEADER_SUPPORT) && !defined(RV_SIP_JSR32_SUPPORT) && !defined(RV_SIP_LIGHT) && !defined(RV_SIP_TEL_URI_SUPPORT) && !defined(RV_SIP_SUBS_ON)
/*
 AnaGram Parsing Engine
 Copyright (c) 1993-1999, Parsifal Software.
 All Rights Reserved.
 Serial number 2P20671
 Registered to:
   Meir Fuchs
   RADVision Ltd.
*/
#ifndef PARSERENGINESIPPRIMITIVES_H_1165245732
#include ".\ParserEngineSipPrimitives.h"
#endif
#ifndef PARSERENGINESIPPRIMITIVES_H_1165245732
#error Mismatched header file
#endif
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define RULE_CONTEXT (&((PCB).cs[(PCB).ssx]))
#define ERROR_CONTEXT ((PCB).cs[(PCB).error_frame_ssx])
#define CONTEXT ((PCB).cs[(PCB).ssx])
#ifndef PCB_TYPE
#define PCB_TYPE SipParser_pcb_type
#endif
#define PCB (*pcb_pointer)
#define PCB_DECL PCB_TYPE *pcb_pointer
#define PCB_POINTER pcb_pointer
#ifndef CONVERT_CASE
static const char agCaseTable[31] = {
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,    0,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20
};
static int agConvertCase(int c) {
  if (c >= 'a' && c <= 'z') return c ^= 0x20;
  if (c >= 0xe0 && c < 0xff) c ^= agCaseTable[c-0xe0];
  return c;
}
#define CONVERT_CASE(c) agConvertCase(c)
#endif
#ifndef TAB_SPACING
#define TAB_SPACING 8
#endif
#define ag_rp_1(PCB_POINTER) (SET_HEADER_VALUE())
#define ag_rp_2(PCB_POINTER) (SIP_BEGIN_TOKEN())
#define ag_rp_3(PCB_POINTER) (SIP_BEGIN_TOKEN())
static void ag_rp_4(PCB_DECL) {
/* Line 300, RV SIP Stack */
        SIP_BEGIN_TOKEN();
        REMOVE_COMMA_BETWEEN_HEADERS();
    
}
#define ag_rp_5(PCB_POINTER) (SIP_BEGIN_TOKEN())
#define ag_rp_6(PCB_POINTER) (SIP_BEGIN_TOKEN())
#define ag_rp_7(PCB_POINTER) (SIP_BEGIN_TOKEN())
#define ag_rp_8(PCB_POINTER) (SIP_BEGIN_TOKEN())
#define ag_rp_9(PCB_POINTER) (SIP_BEGIN_TOKEN())
#define ag_rp_10(PCB_POINTER) (SIP_BEGIN_TOKEN())
#define ag_rp_11(PCB_POINTER) (SIP_BEGIN_TOKEN())
#define ag_rp_12(PCB_POINTER) (SIP_BEGIN_TOKEN())
#define ag_rp_13(PCB_POINTER) (SIP_BEGIN_TOKEN())
#define ag_rp_14(PCB_POINTER) (SIP_BEGIN_TOKEN())
#define ag_rp_15(PCB_POINTER) (CUR_STRING())
#define ag_rp_16(PCB_POINTER) (CUR_STRING())
#define ag_rp_17(PCB_POINTER) (CUR_STRING())
#define ag_rp_18(PCB_POINTER) (CUR_STRING())
#define ag_rp_19(PCB_POINTER) (CUR_STRING())
#define ag_rp_20(PCB_POINTER) (CUR_STRING())
#define ag_rp_21(PCB_POINTER) (CUR_METHOD(PARSER_METHOD_TYPE_INVITE))
#define ag_rp_22(PCB_POINTER) (CUR_METHOD(PARSER_METHOD_TYPE_ACK))
#define ag_rp_23(PCB_POINTER) (CUR_METHOD(PARSER_METHOD_TYPE_BYE))
#define ag_rp_24(PCB_POINTER) (CUR_METHOD(PARSER_METHOD_TYPE_CANCEL))
#define ag_rp_25(PCB_POINTER) (CUR_METHOD(PARSER_METHOD_TYPE_REGISTER))
static ParserMethod ag_rp_26(PCB_DECL, ParserBasicToken t) {
/* Line 490, RV SIP Stack */
 ParserMethod method;
                            method.type = PARSER_METHOD_TYPE_OTHER;
                            method.other = t;
                            RV_UNUSED_ARG(pcb_pointer);
                            return method;
                          
}
static void ag_rp_27(PCB_DECL, ParserCSeq cseq) {
/* Line 506, RV SIP Stack */
   
       RvStatus status;
       status = ParserInitCSeq(PCB.pParserMgr, PCB.pointer,&cseq,PCB.eHeaderType,PCB.pSipObject);
       if(RV_ERROR_ILLEGAL_SYNTAX == status)
       {
           PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
           SYNTAX_ERROR;
       }
       else if (RV_OK != status)
       {
           
           PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
           PCB.eStat = status;
       }
    
}
static ParserCSeq ag_rp_28(PCB_DECL, ParserSequenceNumber sequenceNumber, ParserMethod method) {
/* Line 527, RV SIP Stack */
        ParserCSeq cseq;
        cseq.method = method;
		cseq.sequenceNumber = sequenceNumber;
        RV_UNUSED_ARG(pcb_pointer);
        return(cseq);
	 
}
#define ag_rp_29(PCB_POINTER) (CUR_STRING())
static void ag_rp_30(PCB_DECL, ParserContentLength contentLength) {
/* Line 549, RV SIP Stack */
          RvStatus status;
          contentLength.isCompact = PCB.isCompactForm;
          status = ParserInitContentLength(PCB.pParserMgr,PCB.pointer,&contentLength,PCB.pSipObject);
          if(RV_ERROR_ILLEGAL_SYNTAX == status)
          {
              PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
              SYNTAX_ERROR;
          }
          else if (RV_OK != status)
          {
            
            PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
            PCB.eStat = status;
          }
     
}
static ParserContentLength ag_rp_31(PCB_DECL) {
/* Line 572, RV SIP Stack */
        ParserContentLength contentLength;
        contentLength.contentLenVal = CUR_STRING();
        return contentLength;
    
}
static void ag_rp_32(PCB_DECL, ParserCallId callId) {
/* Line 587, RV SIP Stack */
  
          RvStatus status;
          callId.isCompact = PCB.isCompactForm;
          status = ParserInitCallId(PCB.pParserMgr,PCB.pointer,&callId,PCB.pSipObject);
          if(RV_ERROR_ILLEGAL_SYNTAX == status)
          {
              PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
              SYNTAX_ERROR;
          }
          else if(RV_OK != status)
          {
            
            PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
            PCB.eStat = status;
          }
    
}
static ParserCallId ag_rp_33(PCB_DECL, ParserBasicToken token) {
/* Line 609, RV SIP Stack */
         ParserCallId callId;
         callId.callIdVal = token;
         callId.isCompact = PCB.isCompactForm;
         return callId;
    
}
static ParserCallId ag_rp_34(PCB_DECL, ParserBasicToken token, ParserBasicToken token2) {
/* Line 616, RV SIP Stack */
        ParserCallId callId;
        token.len = (token2.len +token.len + 1);
        callId.callIdVal = token;
        callId.isCompact = PCB.isCompactForm;
        return callId;
    
}
static void ag_rp_35(PCB_DECL) {
/* Line 634, RV SIP Stack */
        RvStatus status;
        PCB.contentType.isCompactForm = PCB.isCompactForm;
        status = ParserInitContentType(PCB.pParserMgr,PCB.pointer,&(PCB.contentType),PCB.pSipObject);
        if(RV_ERROR_ILLEGAL_SYNTAX == status)
        {
             PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
             SYNTAX_ERROR;
        }
        else if (RV_OK != status) 
        {
            
            PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
            PCB.eStat = status;
        }
        ParserCleanExtParams (PCB.pExtParams);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_CONTENT_TYPE, &PCB);
     
}
static void ag_rp_36(PCB_DECL, ParserMediaType t, ParserMediaSubType st) {
/* Line 663, RV SIP Stack */
 
        PCB.contentType.mediaType    = t;
        PCB.contentType.mediaSubType = st;
        PCB.contentType.genericParamList = NULL;
        
        if (0 != ((ParserExtensionString *)PCB.pExtParams)->size)
        {
             PCB.contentType.genericParamList=PCB.pExtParams;
        }
    
}
static void ag_rp_37(PCB_DECL) {
/* Line 676, RV SIP Stack */
 
               RV_UNUSED_ARG(pcb_pointer);
            
}
static void ag_rp_38(PCB_DECL) {
/* Line 716, RV SIP Stack */
        
        
        RV_UNUSED_ARG(pcb_pointer);
    
}
#define ag_rp_39(PCB_POINTER, t) (t)
#define ag_rp_40(PCB_POINTER, t) (t)
static void ag_rp_41(PCB_DECL, ParserBasicToken token1, ParserGenericParamDataVal token2) {
/* Line 820, RV SIP Stack */
      if (0!=((ParserExtensionString *)PCB.pExtParams)->size)
      {
          
          APPEND_DATA(";", 1,PCB.pExtParams);
          APPEND_DATA(token1.buf, token1.len,PCB.pExtParams);
          APPEND_DATA("=" , 1,PCB.pExtParams);
          APPEND_DATA(token2.buf, token2.len,PCB.pExtParams);            
      }
      else
      {      
          
          APPEND_DATA(token1.buf, token1.len,PCB.pExtParams);
          APPEND_DATA("=" , 1,PCB.pExtParams);
          APPEND_DATA(token2.buf, token2.len,PCB.pExtParams);            
      }
   
}
#define ag_rp_42(PCB_POINTER, token) (token)
#define ag_rp_43(PCB_POINTER, qs) (qs)
static void ag_rp_44(PCB_DECL, ParserBasicToken token) {
/* Line 858, RV SIP Stack */
      if (0!=((ParserExtensionString *)PCB.pExtParams)->size)
      {
          
          APPEND_DATA(";", 1,PCB.pExtParams);
          APPEND_DATA(token.buf,token.len, PCB.pExtParams);
      }
      else
      {      
          
          APPEND_DATA(token.buf,token.len,PCB.pExtParams);
      }
    
}
static void ag_rp_45(PCB_DECL, ParserBasicToken token1, ParserGenericParamDataVal token2) {
/* Line 872, RV SIP Stack */
        if (0!=((ParserExtensionString *)PCB.pExtParams)->size)
        {
          
          APPEND_DATA(";", 1,PCB.pExtParams);
          APPEND_DATA(token1.buf, token1.len,PCB.pExtParams);
          APPEND_DATA("=" , 1,PCB.pExtParams);
          APPEND_DATA(token2.buf, token2.len,PCB.pExtParams);            
        }
        else
        {      
          
          APPEND_DATA(token1.buf, token1.len,PCB.pExtParams);
          APPEND_DATA("=" , 1,PCB.pExtParams);
          APPEND_DATA(token2.buf, token2.len,PCB.pExtParams);            
        }
    
}
#define ag_rp_46(PCB_POINTER, token) (token)
#define ag_rp_47(PCB_POINTER, qs) (qs)
static void ag_rp_48(PCB_DECL, ParserSipVersion ver, ParserStatusCode status, ParserReasonPhrase reason) {
/* Line 906, RV SIP Stack */
        RvStatus        stat;
        ParserStatusLine statusLine;
        statusLine.sipVersion = ver;
        statusLine.statusCode = status;
        statusLine.reasonPhrase = reason;
        stat = ParserInitStatusLine(PCB.pParserMgr,&PCB, PCB.pointer, &statusLine, PCB.pSipObject);
        if (RV_ERROR_ILLEGAL_SYNTAX == stat)
        {
            PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
            SYNTAX_ERROR;
        }
        else if (RV_OK != stat)
        {
            
            PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
            PCB.eStat = stat;
        }
    
}
static void ag_rp_49(PCB_DECL, ParserSipVersion ver, ParserStatusCode status) {
/* Line 926, RV SIP Stack */
        RvStatus        stat;
        ParserStatusLine statusLine;
        statusLine.sipVersion = ver;
        statusLine.statusCode = status;       
        statusLine.reasonPhrase.buf=NULL;
        statusLine.reasonPhrase.len=0;
        stat = ParserInitStatusLine(PCB.pParserMgr,&PCB, PCB.pointer, &statusLine, PCB.pSipObject);
        if(RV_ERROR_ILLEGAL_SYNTAX == stat)
        {
            PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
            SYNTAX_ERROR;
        }
        else if (RV_OK != stat)
        {
            
            PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
            PCB.eStat = stat;
        }
    
}
#define ag_rp_50(PCB_POINTER) (CUR_STRING())
#define ag_rp_51(PCB_POINTER) (CUR_STRING())
static ParserStatusCode ag_rp_52(PCB_DECL, int D100, int D10, int D1) {
/* Line 964, RV SIP Stack */
        RvUint16 code;
        RV_UNUSED_ARG(pcb_pointer);
        code =  (RvUint16)((D100 - '0')*100 + (D10 - '0')*10 + (D1 - '0'));
        return code;
    
}
static void ag_rp_53(PCB_DECL) {
/* Line 1028, RV SIP Stack */
 
       RvStatus status;
       PCB.singleVia.isCompact    = PCB.isCompactForm;
       status = ParserInitVia(PCB.pParserMgr, &PCB, PCB.pointer, &(PCB.singleVia),PCB.eHeaderType, PCB.pSipObject);
       if(RV_ERROR_ILLEGAL_SYNTAX == status)
       {
           PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
           SYNTAX_ERROR;
       }
       else if (RV_OK != status && RV_ERROR_NOTSUPPORTED != status)
       {
            
            PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
            PCB.eStat = status;
       }
        
       ParserCleanExtParams (PCB.pExtParams);
       if(RV_ERROR_NOTSUPPORTED != status)
       {
           ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_SINGLE_VIA, &PCB);
           ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_SENT_PROTOCOL, &PCB);
           ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_VIA_SENT_BY, &PCB);
           
       }
       
    
}
static void ag_rp_54(PCB_DECL) {
/* Line 1056, RV SIP Stack */
 
       RvStatus status;
       PCB.singleVia.isCompact    = PCB.isCompactForm;
       status = ParserInitVia(PCB.pParserMgr,&PCB, PCB.pointer,&(PCB.singleVia),PCB.eHeaderType, PCB.pSipObject);
       if(RV_ERROR_ILLEGAL_SYNTAX == status)
       {
           PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
           SYNTAX_ERROR;
       }
       else if (RV_OK != status && RV_ERROR_NOTSUPPORTED != status)
       {
            
            PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
            PCB.eStat = status;
       }
        
       ParserCleanExtParams (PCB.pExtParams);
       if(RV_ERROR_NOTSUPPORTED != status)
       {
           ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_SINGLE_VIA, &PCB);
           ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_SENT_PROTOCOL, &PCB);
           ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_VIA_SENT_BY, &PCB);
       }
    
}
static void ag_rp_55(PCB_DECL) {
/* Line 1083, RV SIP Stack */
 
        PCB.singleVia.sentProtocol = PCB.sentProtocol;
        PCB.singleVia.sentBy       = PCB.viaSentBy;
         
        if (((ParserExtensionString *)PCB.pExtParams)->size !=0)
        {
            if (PCB.singleVia.params.isOtherParams==RV_FALSE)
            {
                PCB.singleVia.params.isOtherParams = RV_TRUE;
                PCB.singleVia.params.otherParam = PCB.pExtParams;
            }
        }                
    
}
static void ag_rp_56(PCB_DECL) {
/* Line 1100, RV SIP Stack */
 
        PCB.singleVia.params.hidden     = RV_FALSE;
        PCB.singleVia.params.alias     = RV_FALSE;
        PCB.singleVia.params.isTtl      = RV_FALSE;
        PCB.singleVia.params.isMaddr    = RV_FALSE;
	    PCB.singleVia.params.isReceived = RV_FALSE;
        PCB.singleVia.params.isBranch   = RV_FALSE;
        PCB.singleVia.params.isOtherParams=RV_FALSE;  
        PCB.singleVia.params.isRport	= RV_FALSE;
        PCB.singleVia.params.isComp     = RV_FALSE;
    
}
static void ag_rp_57(PCB_DECL, ParserViaParam p) {
/* Line 1114, RV SIP Stack */
        switch( p.paramType )
        {
            case PARSER_PARAM_TYPE_HIDDEN :
                PCB.singleVia.params.hidden = RV_TRUE; 
                break;
            case PARSER_PARAM_TYPE_ALIAS:
                PCB.singleVia.params.alias = RV_TRUE; 
                break;
            case PARSER_PARAM_TYPE_TTL :
                PCB.singleVia.params.isTtl = RV_TRUE;
                PCB.singleVia.params.ttl   = p.param.ttl;
                break;
            
            case PARSER_PARAM_TYPE_MADDR :
                PCB.singleVia.params.isMaddr = RV_TRUE;
                PCB.singleVia.params.maddr   = p.param.maddr;
                break;
            
            case PARSER_PARAM_TYPE_RECEIVED :
                PCB.singleVia.params.isReceived = RV_TRUE;
                PCB.singleVia.params.received   = p.param.received;
                break;
            
            case PARSER_PARAM_TYPE_BRANCH :
                PCB.singleVia.params.isBranch = RV_TRUE;
                PCB.singleVia.params.branch   = p.param.branch;
                break;
            case PARSER_PARAM_TYPE_RPORT:
                PCB.singleVia.params.isRport = RV_TRUE;
                PCB.singleVia.params.rPort   = p.param.rport;
                break;
                 
            case PARSER_PARAM_TYPE_COMP :
                PCB.singleVia.params.isComp  = RV_TRUE;
                PCB.singleVia.params.comp    = p.param.comp;
                break;   
            default:
                if (p.isOther == RV_TRUE)
                {
                     PCB.singleVia.params.isOtherParams = RV_TRUE;
                     PCB.singleVia.params.otherParam   = p.otherParam;
                }                   
                break;
        }       
    
}
static void ag_rp_58(PCB_DECL, ParserProtocolName pn, ParserProtocolVersion pv, ParserTransport t) {
/* Line 1166, RV SIP Stack */
 
        PCB.sentProtocol.protocolName    = pn;
        PCB.sentProtocol.protocolVersion = pv;
        PCB.sentProtocol.transport       = t;
    
}
#define ag_rp_59(PCB_POINTER, t) (t)
#define ag_rp_60(PCB_POINTER) (CUR_STRING())
#define ag_rp_61(PCB_POINTER, t) (t)
#define ag_rp_62(PCB_POINTER, t) (t)
static ParserTransport ag_rp_63(PCB_DECL) {
/* Line 1193, RV SIP Stack */
        ParserTransport t;
        t.transport    = RVSIP_TRANSPORT_UDP;
        t.strTransport = CUR_STRING();
        return t;
    
}
static ParserTransport ag_rp_64(PCB_DECL) {
/* Line 1200, RV SIP Stack */
        ParserTransport t;
        t.transport    = RVSIP_TRANSPORT_TCP;
        t.strTransport = CUR_STRING();
        return t;
    
}
static ParserTransport ag_rp_65(PCB_DECL) {
/* Line 1207, RV SIP Stack */
        ParserTransport t;
        t.transport    = RVSIP_TRANSPORT_SCTP;
        t.strTransport = CUR_STRING();
        return t;
    
}
static ParserTransport ag_rp_66(PCB_DECL) {
/* Line 1214, RV SIP Stack */
        ParserTransport t;
        t.transport    = RVSIP_TRANSPORT_TLS;
        t.strTransport = CUR_STRING();
        return t;
    
}
static ParserTransport ag_rp_67(PCB_DECL, ParserBasicToken token) {
/* Line 1221, RV SIP Stack */
        ParserTransport t;
        t.transport    = RVSIP_TRANSPORT_OTHER;
        t.strTransport = token;
        RV_UNUSED_ARG(pcb_pointer);
        return t;
    
}
#define ag_rp_68(PCB_POINTER) (CUR_STRING())
static void ag_rp_69(PCB_DECL, ParserBasicToken host) {
/* Line 1240, RV SIP Stack */
 
        
        PCB.viaSentBy.isPort   = RV_FALSE;
        PCB.viaSentBy.hostName = host;
     
}
static void ag_rp_70(PCB_DECL, ParserBasicToken host, ParserPort port) {
/* Line 1246, RV SIP Stack */
 
        
        PCB.viaSentBy.isPort   = RV_TRUE;
        PCB.viaSentBy.hostName = host;
        PCB.viaSentBy.port     = port;
      
}
static ParserViaParam ag_rp_71(PCB_DECL) {
/* Line 1260, RV SIP Stack */
 ParserViaParam viaParam;
          viaParam.paramType = PARSER_PARAM_TYPE_HIDDEN;                   
          viaParam.param.hidden = RV_TRUE;
	      viaParam.isOther = RV_FALSE;
          viaParam.otherParam = NULL;
          RV_UNUSED_ARG(pcb_pointer);
          return viaParam;
        
}
static ParserViaParam ag_rp_72(PCB_DECL) {
/* Line 1269, RV SIP Stack */
 ParserViaParam viaParam;
          viaParam.paramType = PARSER_PARAM_TYPE_ALIAS;                   
          viaParam.param.alias = RV_TRUE;
	      viaParam.isOther = RV_FALSE;
          viaParam.otherParam = NULL;
          RV_UNUSED_ARG(pcb_pointer);
          return viaParam;
        
}
static ParserViaParam ag_rp_73(PCB_DECL, ParserTTLParam t) {
/* Line 1278, RV SIP Stack */
 ParserViaParam viaParam;
          viaParam.paramType = PARSER_PARAM_TYPE_TTL;                   
          viaParam.param.ttl = t;
	      viaParam.isOther = RV_FALSE;
          viaParam.otherParam = NULL;
          RV_UNUSED_ARG(pcb_pointer);
          return viaParam;
        
}
static ParserViaParam ag_rp_74(PCB_DECL, ParserMaddrParam ma) {
/* Line 1287, RV SIP Stack */
 ParserViaParam viaParam;
          viaParam.paramType = PARSER_PARAM_TYPE_MADDR;                   
          viaParam.param.maddr = ma;
	      viaParam.isOther = RV_FALSE;
          viaParam.otherParam = NULL;
          RV_UNUSED_ARG(pcb_pointer);
          return viaParam;
        
}
static ParserViaParam ag_rp_75(PCB_DECL, ParserReceived r) {
/* Line 1296, RV SIP Stack */
 ParserViaParam viaParam;
          viaParam.paramType = PARSER_PARAM_TYPE_RECEIVED;                   
          viaParam.param.received = r;
	      viaParam.isOther = RV_FALSE;
          viaParam.otherParam = NULL;
          RV_UNUSED_ARG(pcb_pointer);
          return viaParam;
        
}
static ParserViaParam ag_rp_76(PCB_DECL, ParserBranch b) {
/* Line 1305, RV SIP Stack */
 ParserViaParam viaParam;
          viaParam.paramType = PARSER_PARAM_TYPE_BRANCH;                   
          viaParam.param.branch = b;
	      viaParam.isOther = RV_FALSE;
          viaParam.otherParam = NULL;
          RV_UNUSED_ARG(pcb_pointer);
          return viaParam;
        
}
static ParserViaParam ag_rp_77(PCB_DECL, ParserPort rport) {
/* Line 1315, RV SIP Stack */
          ParserViaParam viaParam;
          viaParam.paramType = PARSER_PARAM_TYPE_RPORT;                   
          viaParam.param.rport = rport;
	      viaParam.isOther = RV_FALSE;
          viaParam.otherParam = NULL;
          RV_UNUSED_ARG(pcb_pointer);
          return viaParam;
        
}
static ParserViaParam ag_rp_78(PCB_DECL, ParserCompParam comp) {
/* Line 1324, RV SIP Stack */
 ParserViaParam viaParam;
          viaParam.paramType  = PARSER_PARAM_TYPE_COMP;                   
          viaParam.param.comp = comp;
	      viaParam.isOther    = RV_FALSE;
          viaParam.otherParam = NULL;
          RV_UNUSED_ARG(pcb_pointer);
          return viaParam;
        
}
static ParserViaParam ag_rp_79(PCB_DECL) {
/* Line 1333, RV SIP Stack */
 ParserViaParam viaParam;
          viaParam.paramType = PARSER_PARAM_TYPE_OTHER; 
	      viaParam.isOther = RV_TRUE;
          viaParam.otherParam = PCB.pExtParams;
          return viaParam;    
        
}
static void ag_rp_80(PCB_DECL, ParserBasicToken token) {
/* Line 1350, RV SIP Stack */
        if (((ParserExtensionString *)PCB.pExtParams)->size !=0)
        {
            
            APPEND_DATA(";", 1, PCB.pExtParams);
            APPEND_DATA(token.buf, token.len,PCB.pExtParams);
        }
        else
        {      
            
            APPEND_DATA(token.buf, token.len,PCB.pExtParams);
        }
    
}
static void ag_rp_81(PCB_DECL, ParserBasicToken token, ParserBasicToken token2) {
/* Line 1365, RV SIP Stack */
        if (0!=((ParserExtensionString *)PCB.pExtParams)->size)
        {
            
            APPEND_DATA(";", 1,PCB.pExtParams);
            APPEND_DATA(token.buf, token.len,PCB.pExtParams);
            APPEND_DATA("=", 1,PCB.pExtParams);
            APPEND_DATA(token2.buf, token2.len,PCB.pExtParams);
        }
        else
        {      
            
            APPEND_DATA(token.buf, token.len,PCB.pExtParams);
            APPEND_DATA("=", 1,PCB.pExtParams);
            APPEND_DATA(token2.buf, token2.len,PCB.pExtParams);
        }
    
}
static void ag_rp_82(PCB_DECL, ParserBasicToken token, ParserQuotedString token2) {
/* Line 1384, RV SIP Stack */
        if (((ParserExtensionString *)PCB.pExtParams)->size !=0)
        {
            
            APPEND_DATA(";", 1,PCB.pExtParams);
            APPEND_DATA(token.buf, token.len,PCB.pExtParams);
            APPEND_DATA("=", 1,PCB.pExtParams);
            APPEND_DATA(token2.buf, token2.len,PCB.pExtParams);
        }
        else
        {      
            
            APPEND_DATA(token.buf, token.len,PCB.pExtParams);
            APPEND_DATA("=", 1,PCB.pExtParams);
            APPEND_DATA(token2.buf, token2.len,PCB.pExtParams);
        }
    
}
static void ag_rp_83(PCB_DECL, ParserBasicToken token, ParserBasicToken token2) {
/* Line 1407, RV SIP Stack */
        if (((ParserExtensionString *)PCB.pExtParams)->size !=0)
        {
            
            APPEND_DATA(";", 1,PCB.pExtParams);
            APPEND_DATA(token.buf, token.len,PCB.pExtParams);
            APPEND_DATA("=", 1,PCB.pExtParams);
            APPEND_DATA(token2.buf, token2.len,PCB.pExtParams);
        }
        else
        {      
            
            APPEND_DATA(token.buf, token.len,PCB.pExtParams);
            APPEND_DATA("=", 1,PCB.pExtParams);
            APPEND_DATA(token2.buf, token2.len,PCB.pExtParams);
        }
    
}
#define ag_rp_84(PCB_POINTER, t) (t)
#define ag_rp_85(PCB_POINTER, ttl) (ttl)
#define ag_rp_86(PCB_POINTER) (CUR_STRING())
#define ag_rp_87(PCB_POINTER) (CUR_STRING())
#define ag_rp_88(PCB_POINTER) (CUR_STRING())
#define ag_rp_89(PCB_POINTER, rport) (rport)
static ParserPort ag_rp_90(PCB_DECL) {
/* Line 1462, RV SIP Stack */
        ParserPort port;
        port.len = 0;
		port.buf = NULL;
        RV_UNUSED_ARG(pcb_pointer);
        return port;
    
}
#define ag_rp_91(PCB_POINTER, port) (port)
#define ag_rp_92(PCB_POINTER, ma) (ma)
#define ag_rp_93(PCB_POINTER, host) (host)
#define ag_rp_94(PCB_POINTER, host) (host)
#define ag_rp_95(PCB_POINTER, t) (t)
#define ag_rp_96(PCB_POINTER, comp) (comp)
static ParserCompParam ag_rp_97(PCB_DECL) {
/* Line 1499, RV SIP Stack */
        ParserCompParam comp;
        comp.compType     = RVSIP_COMP_SIGCOMP;
        comp.strCompParam = CUR_STRING();
        return comp;
    
}
static ParserCompParam ag_rp_98(PCB_DECL, ParserBasicToken token) {
/* Line 1506, RV SIP Stack */
        ParserCompParam comp;
        comp.compType     = RVSIP_COMP_OTHER;
        comp.strCompParam = CUR_STRING();
        RV_UNUSED_ARG(token.buf);
        return comp;
    
}
#define ag_rp_99(PCB_POINTER) (CUR_STRING())
static void ag_rp_100(PCB_DECL, ParserAbsoluteUri uri) {
/* Line 1737, RV SIP Stack */
       RvStatus status;
       status = ParserInitAbsUri(PCB.pParserMgr, &PCB, &uri, PCB.eHeaderType,PCB.pSipObject);
       if(RV_ERROR_ILLEGAL_SYNTAX == status)
       {
           PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
           SYNTAX_ERROR;
       }
       else if (status != RV_OK)
       {
            
            PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
            PCB.eStat = status;
       }
    
}
static ParserAbsoluteUri ag_rp_101(PCB_DECL, ParserBasicToken scheme, ParserBasicToken id) {
/* Line 1757, RV SIP Stack */
        ParserAbsoluteUri uri;
        uri.scheme = scheme;
        uri.idefntifier = id;
        RV_UNUSED_ARG(pcb_pointer);
        return uri;
    
}
#define ag_rp_102(PCB_POINTER) (CUR_STRING())
#define ag_rp_103(PCB_POINTER, displayName) (displayName)
#define ag_rp_104(PCB_POINTER) (CUR_STRING())
#define ag_rp_105(PCB_POINTER) (CUR_STRING())
static void ag_rp_106(PCB_DECL) {
/* Line 2158, RV SIP Stack */
        RvStatus status;
        status = ParserInitUrl(PCB.pParserMgr,&PCB,&(PCB.sipUrl), PCB.eHeaderType,PCB.pSipObject);
        if(RV_ERROR_ILLEGAL_SYNTAX == status)
        {
            PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
            SYNTAX_ERROR;
        }
        else if (RV_OK != status)
        {
            
            PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
            PCB.eStat = status;
        }
        ParserCleanExtParams (PCB.pUrlExtParams);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_SIP_URL, &PCB);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_URL_PARAMETER, &PCB);
    
}
#define ag_rp_107(PCB_POINTER, uri) (INIT_PRES_IM_STRUCT(uri))
#define ag_rp_108(PCB_POINTER, uri) (INIT_PRES_IM_STRUCT(uri))
static void ag_rp_109(PCB_DECL) {
/* Line 2209, RV SIP Stack */
        PCB.sipUrl.bIsSecure = RV_FALSE;
        SIP_BEGIN_TOKEN();
        DEFINE_URL_PREFIX();
    
}
static void ag_rp_110(PCB_DECL) {
/* Line 2215, RV SIP Stack */
        PCB.sipUrl.bIsSecure = RV_TRUE;
        SIP_BEGIN_TOKEN();
        DEFINE_URL_PREFIX();
    
}
#define ag_rp_111(PCB_POINTER, uri) (uri)
static ParserAbsoluteUri ag_rp_112(PCB_DECL) {
/* Line 2227, RV SIP Stack */
        ParserAbsoluteUri uri;
        
        uri.scheme.len = 0;
		uri.scheme.buf = NULL;
        uri.idefntifier.len = 0;
		uri.idefntifier.buf = NULL;
        RV_UNUSED_ARG(pcb_pointer)
        return uri;
    
}
#define ag_rp_113(PCB_POINTER) (SET_PARSER_PCB_BACK_TO_BUFF(RV_SIPPARSER_PREFIX_RV_ABS))
static void ag_rp_114(PCB_DECL) {
/* Line 2243, RV SIP Stack */
        SIP_BEGIN_TOKEN();
        DEFINE_PRES_IM_PREFIX(RV_FALSE);
    
}
#define ag_rp_115(PCB_POINTER, uri) (uri)
static ParserAbsoluteUri ag_rp_116(PCB_DECL) {
/* Line 2253, RV SIP Stack */
        ParserAbsoluteUri uri;
        
        uri.scheme.len = 0;
        uri.scheme.buf = NULL;
        uri.idefntifier.len = 0;
		uri.idefntifier.buf = NULL;
        RV_UNUSED_ARG(pcb_pointer)
        return uri;
    
}
static void ag_rp_117(PCB_DECL) {
/* Line 2266, RV SIP Stack */
        SIP_BEGIN_TOKEN();
        DEFINE_PRES_IM_PREFIX(RV_TRUE);
    
}
static void ag_rp_118(PCB_DECL, ParserOptionalHeaders hs) {
/* Line 2274, RV SIP Stack */
  
           
         PCB.sipUrl.optionalHeaders = hs;
         
         if (0 != ((ParserExtensionString *)PCB.pUrlExtParams)->size)
         {
              PCB.sipUrl.urlParameters.genericParamList= PCB.pUrlExtParams;
              PCB.sipUrl.urlParameters.isOtherParams   = RV_TRUE;
         }          
  	
}
static void ag_rp_119(PCB_DECL, ParserOptionalHeaders hs) {
/* Line 2285, RV SIP Stack */
  
         
       PCB.sipUrl.optionalHeaders = hs;
       
       if (0 != ((ParserExtensionString *)PCB.pUrlExtParams)->size)
       {
            PCB.sipUrl.urlParameters.genericParamList=PCB.pUrlExtParams;
            PCB.sipUrl.urlParameters.isOtherParams   = RV_TRUE;
       }          
	
}
static void ag_rp_120(PCB_DECL, ParserOptionalHeaders hs) {
/* Line 2315, RV SIP Stack */
 
        RESET_URI_PARAMS();
        PCB.sipUrl.optionalHeaders.isSpecified = RV_FALSE;
        PCB.sipUrl.optionalHeaders = hs;
        PCB.exUri.uriType = PARSER_ADDR_PARAM_TYPE_SIP_URL;
        PCB.exUri.ExUriInfo.SipUrl = PCB.sipUrl;
      
}
static void ag_rp_121(PCB_DECL, ParserOptionalHeaders hs) {
/* Line 2324, RV SIP Stack */
 
        RESET_URI_PARAMS();
        PCB.sipUrl.optionalHeaders.isSpecified = RV_FALSE;
        PCB.sipUrl.optionalHeaders = hs;
        
        PCB.exUri.uriType = PARSER_ADDR_PARAM_TYPE_SIP_URL;
        PCB.exUri.ExUriInfo.SipUrl = PCB.sipUrl;
      
}
static void ag_rp_122(PCB_DECL, ParserOptionalHeaders hs) {
/* Line 2351, RV SIP Stack */
 
        RESET_URI_PARAMS();
        PCB.sipUrl.optionalHeaders.isSpecified = RV_FALSE;
        PCB.sipUrl.optionalHeaders = hs;
        PCB.exUri.uriType = PARSER_ADDR_PARAM_TYPE_PRES_URI;
        PCB.exUri.ExUriInfo.SipUrl = PCB.sipUrl;
      
}
static void ag_rp_123(PCB_DECL, ParserOptionalHeaders hs) {
/* Line 2360, RV SIP Stack */
 
        RESET_URI_PARAMS();
        PCB.sipUrl.optionalHeaders.isSpecified = RV_FALSE;
        PCB.sipUrl.optionalHeaders = hs;
        
        PCB.exUri.uriType = PARSER_ADDR_PARAM_TYPE_PRES_URI;
        PCB.exUri.ExUriInfo.SipUrl = PCB.sipUrl;
      
}
static void ag_rp_124(PCB_DECL, ParserOptionalHeaders hs) {
/* Line 2372, RV SIP Stack */
 
        RESET_URI_PARAMS();
        PCB.sipUrl.optionalHeaders.isSpecified = RV_FALSE;
        PCB.sipUrl.optionalHeaders = hs;
        PCB.exUri.uriType = PARSER_ADDR_PARAM_TYPE_IM_URI;
        PCB.exUri.ExUriInfo.SipUrl = PCB.sipUrl;
      
}
static void ag_rp_125(PCB_DECL, ParserOptionalHeaders hs) {
/* Line 2381, RV SIP Stack */
 
        RESET_URI_PARAMS();
        PCB.sipUrl.optionalHeaders.isSpecified = RV_FALSE;
        PCB.sipUrl.optionalHeaders = hs;
        
        PCB.exUri.uriType = PARSER_ADDR_PARAM_TYPE_IM_URI;
        PCB.exUri.ExUriInfo.SipUrl = PCB.sipUrl;
      
}
static void ag_rp_126(PCB_DECL, ParserDisplayName scheme, ParserBasicToken id) {
/* Line 2395, RV SIP Stack */
          PCB.exUri.ExUriInfo.absUri.scheme = scheme;
          PCB.exUri.ExUriInfo.absUri.idefntifier = id;
          PCB.exUri.uriType = PARSER_ADDR_PARAM_TYPE_ABS_URI;
      
}
static void ag_rp_127(PCB_DECL) {
/* Line 2405, RV SIP Stack */
        PCB.sipUrl.bIsUserInfo = RV_TRUE;
    
}
static void ag_rp_128(PCB_DECL) {
/* Line 2410, RV SIP Stack */
        PCB.sipUrl.bIsUserInfo = RV_FALSE;
    
}
static void ag_rp_129(PCB_DECL) {
/* Line 2417, RV SIP Stack */
        SET_PARSER_PCB_BACK_TO_BUFF(RV_SIPPARSER_PREFIX_USERINFO);
    
}
static void ag_rp_130(PCB_DECL, ParserBasicToken user) {
/* Line 2423, RV SIP Stack */
        PCB.sipUrl.urlUserInfo.userName   = user;
        PCB.sipUrl.urlUserInfo.isPassword = RV_FALSE;
    
}
static void ag_rp_131(PCB_DECL, ParserBasicToken user, ParserBasicToken pass) {
/* Line 2428, RV SIP Stack */
        PCB.sipUrl.urlUserInfo.userName   = user;
        PCB.sipUrl.urlUserInfo.isPassword = RV_TRUE;
        PCB.sipUrl.urlUserInfo.password   = pass;
     
}
#define ag_rp_132(PCB_POINTER, t) (t)
#define ag_rp_133(PCB_POINTER) (CUR_STRING())
#define ag_rp_134(PCB_POINTER) (CUR_STRING())
static void ag_rp_135(PCB_DECL) {
/* Line 2461, RV SIP Stack */
        PCB.sipUrl.bIsUserInfo = RV_FALSE;
    
}
static void ag_rp_136(PCB_DECL) {
/* Line 2467, RV SIP Stack */
        SET_PARSER_PCB_BACK_TO_BUFF(RV_SIPPARSER_PREFIX_HOST);
    
}
static void ag_rp_137(PCB_DECL, ParserBasicToken host) {
/* Line 2475, RV SIP Stack */
        PCB.sipUrl.urlHost.hostName = host;
        PCB.sipUrl.urlHost.isPort   = RV_FALSE;
    
}
static void ag_rp_138(PCB_DECL, ParserBasicToken host, ParserPort port) {
/* Line 2481, RV SIP Stack */
        PCB.sipUrl.urlHost.hostName = host;
        PCB.sipUrl.urlHost.isPort   = RV_TRUE;
        PCB.sipUrl.urlHost.port     = port;
    
}
#define ag_rp_139(PCB_POINTER, t) (t)
#define ag_rp_140(PCB_POINTER, ip6ref) (ip6ref)
#define ag_rp_141(PCB_POINTER) (CUR_STRING())
#define ag_rp_142(PCB_POINTER) (CUR_STRING())
#define ag_rp_143(PCB_POINTER) (CUR_STRING())
#define ag_rp_144(PCB_POINTER) (RESET_URI_PARAMS())
static void ag_rp_145(PCB_DECL) {
/* Line 2535, RV SIP Stack */
	
       PCB.sipUrl.urlParameters.isValid = RV_TRUE;
    
}
static void ag_rp_146(PCB_DECL) {
/* Line 2548, RV SIP Stack */
 
        RESET_URI_PARAMS();  
         								
        switch( PCB.urlParameter.paramType )
        {
            case PARSER_TRANSPORT_PARAM: 
	            PCB.sipUrl.urlParameters.isTransport        = RV_TRUE;
	            PCB.sipUrl.urlParameters.transport          = PCB.urlParameter.param.transport; 
	            break;
            case PARSER_USER_PARAM: 
	            PCB.sipUrl.urlParameters.isUserParam        = RV_TRUE;
	            PCB.sipUrl.urlParameters.userParam          = PCB.urlParameter.param.userParam; 
	            break;		
            case PARSER_TTL_PARAM: 
	            PCB.sipUrl.urlParameters.isTtlParam         = RV_TRUE;
	            PCB.sipUrl.urlParameters.ttlParam           = PCB.urlParameter.param.ttlParam; 
	            break;
            case PARSER_MADDR_PARAM: 
	            PCB.sipUrl.urlParameters.isMaddrParam       = RV_TRUE;
	            PCB.sipUrl.urlParameters.maddrParam         = PCB.urlParameter.param.maddrParam; 
	            break;
            case PARSER_METHOD_PARAM: 
	            PCB.sipUrl.urlParameters.isMethodParam       = RV_TRUE;
	            PCB.sipUrl.urlParameters.methodParam         = PCB.urlParameter.param.methodParam; 
	            break;
            case PARSER_LR_PARAM: 
	            PCB.sipUrl.urlParameters.lrParamType         = PCB.urlParameter.param.lrParamType;
	            break;
            case PARSER_COMP_PARAM:
                PCB.sipUrl.urlParameters.isCompParam         = RV_TRUE;
                PCB.sipUrl.urlParameters.compParam           = PCB.urlParameter.param.compParam;
                break;
			case PARSER_TOKENIZED_BY_PARAM: 
		        PCB.sipUrl.urlParameters.isTokenizedByParam	= RV_TRUE;
		        PCB.sipUrl.urlParameters.tokenizedByParam	= PCB.urlParameter.param.tokenizedByParam; 
		        break;
			case PARSER_ORIG_PARAM: 
		        PCB.sipUrl.urlParameters.bOrigParam			= PCB.urlParameter.param.bOrigParam;
		        break;
            case PARSER_OTHER_PARAM:    
	            break;
             default:
                 PCB.exit_flag = AG_SEMANTIC_ERROR_CODE;
                 PCB.eStat = RV_ERROR_ILLEGAL_SYNTAX;            
        } 	 														
    
}
static void ag_rp_147(PCB_DECL) {
/* Line 2595, RV SIP Stack */
 
         
        switch( PCB.urlParameter.paramType )
        {
	        case PARSER_TRANSPORT_PARAM: 
		        PCB.sipUrl.urlParameters.isTransport        = RV_TRUE;
		        PCB.sipUrl.urlParameters.transport          = PCB.urlParameter.param.transport; 
		        break;
	        case PARSER_USER_PARAM: 
		        PCB.sipUrl.urlParameters.isUserParam        = RV_TRUE;
		        PCB.sipUrl.urlParameters.userParam          = PCB.urlParameter.param.userParam; 
		        break;	
	        case PARSER_TTL_PARAM: 
		        PCB.sipUrl.urlParameters.isTtlParam         = RV_TRUE;
		        PCB.sipUrl.urlParameters.ttlParam           = PCB.urlParameter.param.ttlParam; 
		        break;
	        case PARSER_MADDR_PARAM: 
		        PCB.sipUrl.urlParameters.isMaddrParam       = RV_TRUE;
		        PCB.sipUrl.urlParameters.maddrParam         = PCB.urlParameter.param.maddrParam; 
		        break;  
            case PARSER_METHOD_PARAM: 
		        PCB.sipUrl.urlParameters.isMethodParam		= RV_TRUE;
		        PCB.sipUrl.urlParameters.methodParam		= PCB.urlParameter.param.methodParam; 
		        break; 
            case PARSER_LR_PARAM: 
		        PCB.sipUrl.urlParameters.lrParamType		= PCB.urlParameter.param.lrParamType;
		        break; 
            case PARSER_COMP_PARAM:
                PCB.sipUrl.urlParameters.isCompParam		= RV_TRUE;
                PCB.sipUrl.urlParameters.compParam			= PCB.urlParameter.param.compParam;
                break;
			case PARSER_TOKENIZED_BY_PARAM: 
		        PCB.sipUrl.urlParameters.isTokenizedByParam	= RV_TRUE;
		        PCB.sipUrl.urlParameters.tokenizedByParam	= PCB.urlParameter.param.tokenizedByParam; 
		        break;
			case PARSER_ORIG_PARAM: 
		        PCB.sipUrl.urlParameters.bOrigParam			= PCB.urlParameter.param.bOrigParam;
		        break;
            case PARSER_OTHER_PARAM:    
		        PCB.sipUrl.urlParameters.isOtherParams		= RV_TRUE;
		        break; 
            default:
                PCB.exit_flag = AG_SEMANTIC_ERROR_CODE;
                PCB.eStat = RV_ERROR_ILLEGAL_SYNTAX;                  
        }	 
    
}
static void ag_rp_148(PCB_DECL, ParserTransport t) {
/* Line 2645, RV SIP Stack */
 
         PCB.urlParameter.paramType = PARSER_TRANSPORT_PARAM;
         PCB.urlParameter.param.transport = t;
    
}
static void ag_rp_149(PCB_DECL, ParserUserParam u) {
/* Line 2650, RV SIP Stack */
 
	     PCB.urlParameter.paramType = PARSER_USER_PARAM;
		 PCB.urlParameter.param.userParam = u;
	
}
static void ag_rp_150(PCB_DECL, ParserTtlParam t) {
/* Line 2655, RV SIP Stack */
 
	     PCB.urlParameter.paramType = PARSER_TTL_PARAM;
		 PCB.urlParameter.param.ttlParam = t;
	
}
static void ag_rp_151(PCB_DECL, ParserMaddrParam ma) {
/* Line 2660, RV SIP Stack */
 
	     PCB.urlParameter.paramType = PARSER_MADDR_PARAM;
		 PCB.urlParameter.param.maddrParam = ma;
	
}
static void ag_rp_152(PCB_DECL, ParserMethod met) {
/* Line 2666, RV SIP Stack */
	     PCB.urlParameter.paramType = PARSER_METHOD_PARAM;
		 PCB.urlParameter.param.methodParam =met;
    
}
static void ag_rp_153(PCB_DECL, ParserLrParamType lrParam) {
/* Line 2671, RV SIP Stack */
	     PCB.urlParameter.paramType = PARSER_LR_PARAM;
		 PCB.urlParameter.param.lrParamType   = lrParam;
    
}
static void ag_rp_154(PCB_DECL, ParserCompParam comp) {
/* Line 2676, RV SIP Stack */
        PCB.urlParameter.paramType       = PARSER_COMP_PARAM;
        PCB.urlParameter.param.compParam = comp;
    
}
static void ag_rp_155(PCB_DECL, ParserTokenizedByParam tokenizedBy) {
/* Line 2681, RV SIP Stack */
        PCB.urlParameter.paramType       = PARSER_TOKENIZED_BY_PARAM;
        PCB.urlParameter.param.tokenizedByParam = tokenizedBy;
    
}
static void ag_rp_156(PCB_DECL, RvBool orig) {
/* Line 2686, RV SIP Stack */
        PCB.urlParameter.paramType       = PARSER_ORIG_PARAM;
        PCB.urlParameter.param.bOrigParam = orig;
    
}
static void ag_rp_157(PCB_DECL) {
/* Line 2696, RV SIP Stack */
	     PCB.urlParameter.paramType = PARSER_OTHER_PARAM;
    
}
#define ag_rp_158(PCB_POINTER, t) (t)
static ParserUserParam ag_rp_159(PCB_DECL, RvSipUserParam pt) {
/* Line 2711, RV SIP Stack */
 
        ParserUserParam userParam;
	    userParam.paramType = pt;
		userParam.otherUser.buf = NULL;
		userParam.otherUser.len = 0;
        RV_UNUSED_ARG(pcb_pointer);
	    return userParam;
    
}
static ParserUserParam ag_rp_160(PCB_DECL, ParserOtherUser ou) {
/* Line 2720, RV SIP Stack */
 
        ParserUserParam userParam;
        userParam.paramType = RVSIP_USERPARAM_OTHER;
	    userParam.otherUser = ou;
        RV_UNUSED_ARG(pcb_pointer);
	    return userParam;
    
}
#define ag_rp_161(PCB_POINTER) (RVSIP_USERPARAM_PHONE)
#define ag_rp_162(PCB_POINTER) (RVSIP_USERPARAM_IP)
#define ag_rp_163(PCB_POINTER, t) (t)
#define ag_rp_164(PCB_POINTER, ttl) (ttl)
#define ag_rp_165(PCB_POINTER, host) (host)
#define ag_rp_166(PCB_POINTER, met) (met)
#define ag_rp_167(PCB_POINTER) (ParserLrParam1)
#define ag_rp_168(PCB_POINTER) (ParserLrParamTrue)
#define ag_rp_169(PCB_POINTER) (ParserLrParamOn)
#define ag_rp_170(PCB_POINTER) (ParserLrParamEmpty)
#define ag_rp_171(PCB_POINTER, comp) (comp)
#define ag_rp_172(PCB_POINTER, hostname) (hostname)
#define ag_rp_173(PCB_POINTER) (RV_TRUE)
static void ag_rp_174(PCB_DECL, ParserBasicToken token) {
/* Line 2792, RV SIP Stack */
          if (0!=((ParserExtensionString *)PCB.pUrlExtParams)->size)
          {
              
              APPEND_DATA(";", 1,PCB.pUrlExtParams);
              APPEND_DATA(token.buf,token.len, PCB.pUrlExtParams);
          }
          else
          {      
              
              APPEND_DATA(token.buf,token.len,PCB.pUrlExtParams);
          }
    
}
static void ag_rp_175(PCB_DECL, ParserBasicToken token1, ParserBasicToken token2) {
/* Line 2806, RV SIP Stack */
          if (0!=((ParserExtensionString *)PCB.pUrlExtParams)->size)
          {
              
              APPEND_DATA(";", 1,PCB.pUrlExtParams);
              APPEND_DATA(token1.buf, token1.len,PCB.pUrlExtParams);
              APPEND_DATA("=" , 1,PCB.pUrlExtParams);
              APPEND_DATA(token2.buf, token2.len,PCB.pUrlExtParams);            
          }
          else
          {      
              
              APPEND_DATA(token1.buf, token1.len,PCB.pUrlExtParams);
              APPEND_DATA("=" , 1,PCB.pUrlExtParams);
              APPEND_DATA(token2.buf, token2.len,PCB.pUrlExtParams);            
          }
    
}
#define ag_rp_176(PCB_POINTER) (CUR_STRING())
#define ag_rp_177(PCB_POINTER, token) (token)
static ParserOptionalHeaders ag_rp_178(PCB_DECL) {
/* Line 2849, RV SIP Stack */
 
        ParserOptionalHeaders optionalHeaders;
        optionalHeaders.isSpecified = RV_FALSE;
		optionalHeaders.body.buf = NULL;
		optionalHeaders.body.len = 0;
        RV_UNUSED_ARG(pcb_pointer);
        return optionalHeaders;
    
}
static ParserOptionalHeaders ag_rp_179(PCB_DECL, ParserBasicToken headersList) {
/* Line 2858, RV SIP Stack */
 
        ParserOptionalHeaders optionalHeaders;
        optionalHeaders.isSpecified = RV_TRUE;
        optionalHeaders.body=headersList;
        RV_UNUSED_ARG(pcb_pointer);
        return optionalHeaders;
    
}
#define ag_rp_180(PCB_POINTER) (CUR_STRING())
#define ag_rp_181(PCB_POINTER) (CUR_STRING())
#define ag_rp_182(PCB_POINTER) (CUR_STRING())
#define ag_rp_183(PCB_POINTER) (CUR_STRING())
#define ag_rp_184(PCB_POINTER) (CUR_STRING())
static void ag_rp_185(PCB_DECL) {
/* Line 2894, RV SIP Stack */
        RvStatus status;
        status = ParserInitRequsetLine(PCB.pParserMgr,&PCB,PCB.pointer,&(PCB.requestLine), PCB.pSipObject);
        if(RV_ERROR_ILLEGAL_SYNTAX == status)
        {
            PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
            SYNTAX_ERROR;
        }
        else if (RV_OK != status)
        {
            
            PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
            PCB.eStat = status;
        }
        ParserCleanExtParams (PCB.pExtParams);
        ParserCleanExtParams (PCB.pUrlExtParams);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_REQUEST_LINE, &PCB);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_EXURI, &PCB);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_SIP_URL, &PCB);
        
    
}
static void ag_rp_186(PCB_DECL, ParserMethod met, ParserSipVersion ver) {
/* Line 2919, RV SIP Stack */
         PCB.requestLine.method = met;
         PCB.requestLine.sipVer = ver;
		 PCB.requestLine.exUri = PCB.exUri;         
    
}
#define ag_rp_187(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("m"))
#define ag_rp_188(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("i"))
#define ag_rp_189(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("c"))
#define ag_rp_190(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("v"))
#define ag_rp_191(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("t"))
#define ag_rp_192(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("f"))
#define ag_rp_193(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("r"))
#define ag_rp_194(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("b"))
#define ag_rp_195(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("k"))
#define ag_rp_196(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("x"))
#define ag_rp_197(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("o"))
#define ag_rp_198(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("u"))
#define ag_rp_199(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("l"))
#define ag_rp_200(PCB_POINTER, met) (met)
#define ag_rp_201(PCB_POINTER) (SET_URI_TYPE(PARSER_ADDR_PARAM_TYPE_SIP_URL, PCB.exUri.ExUriInfo.absUri))
#define ag_rp_202(PCB_POINTER, uri) (SET_URI_TYPE(PARSER_ADDR_PARAM_TYPE_PRES_URI, uri))
#define ag_rp_203(PCB_POINTER, uri) (SET_URI_TYPE(PARSER_ADDR_PARAM_TYPE_IM_URI, uri))
#define ag_rp_204(PCB_POINTER, uri) (SET_URI_TYPE(PARSER_ADDR_PARAM_TYPE_ABS_URI, uri))
static void ag_rp_205(PCB_DECL) {
/* Line 2974, RV SIP Stack */
 
         RvStatus status;
         status = ParserInitParty(PCB.pParserMgr,PCB.pointer,&(PCB.party), PCB.eHeaderType, PCB.eWhichHeader, PCB.pSipObject);
         if(RV_ERROR_ILLEGAL_SYNTAX == status)
         {
             PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
             SYNTAX_ERROR;
         }
         else if (RV_OK != status)
         {
             
             PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
             PCB.eStat = status;
         }
         
        ParserCleanExtParams (PCB.pExtParams); 
        ParserCleanExtParams (PCB.pUrlExtParams);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_PARTY, &PCB);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_NAME_ADDR, &PCB);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_EXURI, &PCB);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_SIP_URL, &PCB);
	 
}
static void ag_rp_206(PCB_DECL) {
/* Line 3000, RV SIP Stack */
 
	  PCB.party.nameAddr = PCB.nameAddr;
	  PCB.party.partyParams.isTag = RV_FALSE;
	  PCB.party.partyParams.isAddrParams = RV_FALSE;
      PCB.party.isCompact = PCB.isCompactForm;
	
}
static void ag_rp_207(PCB_DECL, ParserPartyParams partyParams) {
/* Line 3009, RV SIP Stack */
 
	  PCB.party.nameAddr = PCB.nameAddr;
	  PCB.party.partyParams = partyParams; 
      PCB.party.isCompact = PCB.isCompactForm;
	
}
static void ag_rp_208(PCB_DECL) {
/* Line 3018, RV SIP Stack */
 
	  PCB.party.nameAddr.isDisplayName = RV_FALSE;
      PCB.party.partyParams.isTag = RV_FALSE;
	  PCB.party.partyParams.isAddrParams = RV_FALSE;
	  PCB.party.nameAddr.exUri = PCB.exUri;
      PCB.party.isCompact = PCB.isCompactForm;
	
}
static void ag_rp_209(PCB_DECL, ParserPartyParams partyParams) {
/* Line 3028, RV SIP Stack */
 
        PCB.party.nameAddr.isDisplayName = RV_FALSE;
        PCB.party.partyParams = partyParams;
		PCB.party.nameAddr.exUri = PCB.exUri;       
        PCB.party.isCompact = PCB.isCompactForm;
      
	
}
static void ag_rp_210(PCB_DECL, ParserDisplayName name) {
/* Line 3040, RV SIP Stack */
 
		PCB.nameAddr.exUri = PCB.exUri;    
        PCB.nameAddr.isDisplayName = RV_TRUE;  
	    PCB.nameAddr.name = name;
	
}
static void ag_rp_211(PCB_DECL) {
/* Line 3047, RV SIP Stack */
 
		PCB.nameAddr.exUri = PCB.exUri;
        PCB.nameAddr.isDisplayName = RV_FALSE;  
    
}
#define ag_rp_212(PCB_POINTER, qs) (qs)
#define ag_rp_213(PCB_POINTER, mtot) (mtot)
#define ag_rp_214(PCB_POINTER) (CUR_STRING())
#define ag_rp_215(PCB_POINTER) (CUR_STRING())
#define ag_rp_216(PCB_POINTER, t) (t)
#define ag_rp_217(PCB_POINTER, ppd) (ppd)
static ParserPartyParams ag_rp_218(PCB_DECL, ParserPartyParams ppl, ParserPartyParams ppd) {
/* Line 3078, RV SIP Stack */
        if (RV_TRUE == ppd.isTag)
        {
            ppl.isTag = RV_TRUE;
            ppl.tag = ppd.tag;
        }
        if (RV_TRUE == ppd.isAddrParams) 
        {
            ppl.isAddrParams = RV_TRUE;
            ppl.addrParams = ppd.addrParams;
        }
        RV_UNUSED_ARG(pcb_pointer);
        return ppl;
    
}
static ParserPartyParams ag_rp_219(PCB_DECL, ParserTagParam tag) {
/* Line 3097, RV SIP Stack */
        ParserPartyParams partyParams;
        partyParams.isTag = RV_TRUE;
        partyParams.tag = tag;
        partyParams.isAddrParams = RV_FALSE;
        partyParams.addrParams = NULL;
        RV_UNUSED_ARG(pcb_pointer);
        return partyParams; 
    
}
static ParserPartyParams ag_rp_220(PCB_DECL) {
/* Line 3107, RV SIP Stack */
        ParserPartyParams partyParams;
        partyParams.isAddrParams = RV_TRUE;
        partyParams.isTag = RV_FALSE;
        partyParams.addrParams = PCB.pExtParams;
		partyParams.tag.buf = NULL;
		partyParams.tag.len = 0;
        return partyParams; 
    
}
static void ag_rp_221(PCB_DECL) {
/* Line 3133, RV SIP Stack */
         RvStatus status;
         ParserContactHeaderValues contactHeaderValues;
         contactHeaderValues.isStar = RV_TRUE; 
         contactHeaderValues.isCompact = PCB.isCompactForm;
         status = ParserInitContactHeader(PCB.pParserMgr, &PCB, PCB.pointer,
                                          &contactHeaderValues,
                                          PCB.eHeaderType,
                                          PCB.pSipObject);
         if(RV_ERROR_ILLEGAL_SYNTAX == status)
         {
             PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
             SYNTAX_ERROR;
         }
         else if (RV_OK != status)
         {
             
             PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
             PCB.eStat = status;
         }
    
}
static void ag_rp_222(PCB_DECL) {
/* Line 3166, RV SIP Stack */
         RvStatus status;
         ParserContactHeaderValues  contactHeaderValues;
         contactHeaderValues.isStar        = RV_FALSE;
         contactHeaderValues.header        = PCB.contact;
         contactHeaderValues.isCompact     = PCB.isCompactForm;
         status = ParserInitContactHeader(PCB.pParserMgr,&PCB,PCB.pointer,
                                          &contactHeaderValues,
                                          PCB.eHeaderType,
                                          PCB.pSipObject);
         if(RV_ERROR_ILLEGAL_SYNTAX == status)
         {
             PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
             SYNTAX_ERROR;
         }
         else if (RV_OK != status)
         {
             
             PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
             PCB.eStat = status;
         }
         
         
        ParserCleanExtParams (PCB.pExtParams);
        ParserCleanExtParams (PCB.pUrlExtParams);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_CONTACT, &PCB);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_NAME_ADDR, &PCB);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_EXURI, &PCB);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_SIP_URL, &PCB);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_EXPIRES, &PCB);
    
}
static void ag_rp_223(PCB_DECL) {
/* Line 3199, RV SIP Stack */
         RvStatus                 status;
         ParserContactHeaderValues contactHeaderValues;
         contactHeaderValues.isStar = RV_FALSE;
         contactHeaderValues.header = PCB.contact;
         contactHeaderValues.isCompact = PCB.isCompactForm;
         status = ParserInitContactHeader(PCB.pParserMgr,&PCB, PCB.pointer,
                                          &contactHeaderValues,
                                          PCB.eHeaderType,
                                          PCB.pSipObject);
         if(RV_ERROR_ILLEGAL_SYNTAX == status)
         {
             PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
             SYNTAX_ERROR;
         }
         else if (RV_OK != status)
         {
             
             PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
             PCB.eStat = status;
         }
          
         ParserCleanExtParams (PCB.pExtParams);
         ParserCleanExtParams (PCB.pUrlExtParams);
         ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_CONTACT, &PCB);
         ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_NAME_ADDR, &PCB);
         ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_EXURI, &PCB);
         ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_SIP_URL, &PCB);
         ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_EXPIRES, &PCB);
    
}
static void ag_rp_224(PCB_DECL) {
/* Line 3234, RV SIP Stack */
 
	  PCB.contact.nameAddr = PCB.nameAddr;
      PCB.contact.isParams = RV_FALSE;
	
}
static void ag_rp_225(PCB_DECL) {
/* Line 3240, RV SIP Stack */
 
	  PCB.contact.nameAddr = PCB.nameAddr;
      PCB.contact.isParams = RV_TRUE;
      
	
}
static void ag_rp_226(PCB_DECL) {
/* Line 3250, RV SIP Stack */
 
	  PCB.contact.nameAddr.isDisplayName = RV_FALSE;  
      PCB.contact.isParams = RV_FALSE;
	  PCB.contact.nameAddr.exUri = PCB.exUri;      
	
}
static void ag_rp_227(PCB_DECL) {
/* Line 3257, RV SIP Stack */
 
      PCB.contact.nameAddr.isDisplayName = RV_FALSE;
	  PCB.contact.isParams = RV_TRUE;      
      PCB.contact.nameAddr.exUri = PCB.exUri;
	
}
static void ag_rp_228(PCB_DECL, ParserQVal q) {
/* Line 3277, RV SIP Stack */
 
        PCB.contact.params.isQ = RV_TRUE;
        PCB.contact.params.q = q;
    
}
static void ag_rp_229(PCB_DECL, ParserContactActionType action) {
/* Line 3283, RV SIP Stack */
 
        PCB.contact.params.isAction = RV_TRUE;
        PCB.contact.params.action = action;
    
}
static void ag_rp_230(PCB_DECL) {
/* Line 3289, RV SIP Stack */
 
        PCB.contact.params.isExpires = RV_TRUE;
        PCB.contact.params.expires = PCB.expires;
    
}
static void ag_rp_231(PCB_DECL) {
/* Line 3295, RV SIP Stack */
  
        PCB.contact.params.isExtention = RV_TRUE;
		PCB.contact.params.exten = PCB.pExtParams;
    
}
#define ag_rp_232(PCB_POINTER, qVal) (qVal)
#define ag_rp_233(PCB_POINTER) (CUR_STRING())
#define ag_rp_234(PCB_POINTER) (CUR_STRING())
#define ag_rp_235(PCB_POINTER, actionVal) (actionVal)
#define ag_rp_236(PCB_POINTER) (PARSER_CONTACT_ACTION_PROXY)
#define ag_rp_237(PCB_POINTER) (PARSER_CONTACT_ACTION_REDIRECT)
static void ag_rp_238(PCB_DECL) {
/* Line 3341, RV SIP Stack */
       RvStatus status;
       status = ParserInitExpiresHeader(PCB.pParserMgr,PCB.pointer,&(PCB.expires),PCB.eHeaderType,PCB.pSipObject);
       if(RV_ERROR_ILLEGAL_SYNTAX == status)
       {
           PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
           SYNTAX_ERROR;
       }
       else if (RV_OK != status)
       {
           
           PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
           PCB.eStat = status;
       }
       ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_EXPIRES, &PCB);
    
}
static void ag_rp_239(PCB_DECL, ParserDeltaSeconds delta) {
/* Line 3363, RV SIP Stack */
  
		PCB.expires.eFormat = RVSIP_EXPIRES_FORMAT_DELTA_SECONDS;
		PCB.expires.deltaSeconds = delta;
    
}
static void ag_rp_240(PCB_DECL, ParserSipDate date) {
/* Line 3369, RV SIP Stack */
   
	    PCB.expires.eFormat = RVSIP_EXPIRES_FORMAT_DATE;
	    PCB.expires.sipDate = date;
	
}
static void ag_rp_241(PCB_DECL, ParserDeltaSeconds delta) {
/* Line 3378, RV SIP Stack */
   
		PCB.expires.eFormat = RVSIP_EXPIRES_FORMAT_DELTA_SECONDS;
		PCB.expires.deltaSeconds = delta;
    
}
static void ag_rp_242(PCB_DECL, ParserSipDate date) {
/* Line 3384, RV SIP Stack */
   
	    PCB.expires.eFormat = RVSIP_EXPIRES_FORMAT_DATE;
	    PCB.expires.sipDate = date;
	
}
#define ag_rp_243(PCB_POINTER) (CUR_STRING())
static void ag_rp_244(PCB_DECL, ParserSipDate date) {
/* Line 3409, RV SIP Stack */
   
          RvStatus status;
          status = ParserInitDateHeader(PCB.pParserMgr,PCB.pointer,&date,PCB.eHeaderType,PCB.pSipObject);
          if(RV_ERROR_ILLEGAL_SYNTAX == status)
          {
              PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
              SYNTAX_ERROR;
          }
          else if (RV_OK != status)
          {
                
              PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
              PCB.eStat = status;
          }
    
}
static ParserSipDate ag_rp_245(PCB_DECL, ParserDDMMYY date, ParserTime t) {
/* Line 3431, RV SIP Stack */
  
        ParserSipDate sipDate;
        sipDate.eWeekDay = RVSIP_WEEKDAY_UNDEFINED;
		sipDate.ddmmyy   = date;
        sipDate.time     = t;
        RV_UNUSED_ARG(pcb_pointer);
        return sipDate;
    
}
#define ag_rp_246(PCB_POINTER, date) (date)
static ParserSipDate ag_rp_247(PCB_DECL, RvSipDateWeekDay day, ParserDDMMYY date, ParserTime t) {
/* Line 3446, RV SIP Stack */
   
        ParserSipDate sipDate;
        sipDate.eWeekDay = RVSIP_WEEKDAY_UNDEFINED;
	    sipDate.eWeekDay = day; 
		sipDate.ddmmyy   = date;
        sipDate.time     = t;
        RV_UNUSED_ARG(pcb_pointer);
        return sipDate;
    
}
#define ag_rp_248(PCB_POINTER) (RVSIP_WEEKDAY_SUN)
#define ag_rp_249(PCB_POINTER) (RVSIP_WEEKDAY_MON)
#define ag_rp_250(PCB_POINTER) (RVSIP_WEEKDAY_TUE)
#define ag_rp_251(PCB_POINTER) (RVSIP_WEEKDAY_WED)
#define ag_rp_252(PCB_POINTER) (RVSIP_WEEKDAY_THU)
#define ag_rp_253(PCB_POINTER) (RVSIP_WEEKDAY_FRI)
#define ag_rp_254(PCB_POINTER) (RVSIP_WEEKDAY_SAT)
static ParserDDMMYY ag_rp_255(PCB_DECL, ParserInt32 dd, RvSipDateMonth mm, ParserInt32 yy) {
/* Line 3471, RV SIP Stack */
        ParserDDMMYY ddmmyy;
        ddmmyy.day = dd;
        ddmmyy.eMonth = mm;
        ddmmyy.year = yy;
        RV_UNUSED_ARG(pcb_pointer);
        return ddmmyy;
    
}
static ParserInt32 ag_rp_256(PCB_DECL, int d) {
/* Line 3483, RV SIP Stack */
    	RV_UNUSED_ARG(pcb_pointer);
    	return d - '0';	
    
}
static ParserInt32 ag_rp_257(PCB_DECL, int d1, int d2) {
/* Line 3488, RV SIP Stack */
		RV_UNUSED_ARG(pcb_pointer);
	    return 10 * (d1-'0') + (d2-'0');
    
}
#define ag_rp_258(PCB_POINTER) (RVSIP_MONTH_JAN)
#define ag_rp_259(PCB_POINTER) (RVSIP_MONTH_FEB)
#define ag_rp_260(PCB_POINTER) (RVSIP_MONTH_MAR)
#define ag_rp_261(PCB_POINTER) (RVSIP_MONTH_APR)
#define ag_rp_262(PCB_POINTER) (RVSIP_MONTH_MAY)
#define ag_rp_263(PCB_POINTER) (RVSIP_MONTH_JUN)
#define ag_rp_264(PCB_POINTER) (RVSIP_MONTH_JUL)
#define ag_rp_265(PCB_POINTER) (RVSIP_MONTH_AUG)
#define ag_rp_266(PCB_POINTER) (RVSIP_MONTH_SEP)
#define ag_rp_267(PCB_POINTER) (RVSIP_MONTH_OCT)
#define ag_rp_268(PCB_POINTER) (RVSIP_MONTH_NOV)
#define ag_rp_269(PCB_POINTER) (RVSIP_MONTH_DEC)
#define ag_rp_270(PCB_POINTER, d1, d2) (2000 + 10 * (d1-'0') + (d2-'0'))
#define ag_rp_271(PCB_POINTER, d1, d2, d3) (2000 + 100 * (d1-'0') + 10 * (d2-'0') + (d3-'0'))
#define ag_rp_272(PCB_POINTER, d1, d2, d3, d4) (1000 * (d1-'0') + 100*(d2-'0') + 10*(d3-'0') + (d4-'0'))
static ParserTime ag_rp_273(PCB_DECL, ParserInt32 hour, ParserInt32 min, ParserInt32 second, ParserTimeZone zone) {
/* Line 3518, RV SIP Stack */
        ParserTime t;
        t.hour   = hour;
        t.minute = min;
        t.second = second;
        t.zone   = zone;
        RV_UNUSED_ARG(pcb_pointer);
        return t;        
    
}
#define ag_rp_274(PCB_POINTER, d1, d2) (10*(d1-'0') + (d2-'0'))
#define ag_rp_275(PCB_POINTER, d1, d2) (10*(d1-'0') + (d2-'0'))
#define ag_rp_276(PCB_POINTER, d1, d2) (10*(d1-'0') + (d2-'0'))
#define ag_rp_277(PCB_POINTER) (SIPPARSER_TIME_ZONE_GMT)
#define ag_rp_278(PCB_POINTER) (SIPPARSER_TIME_ZONE_UTC)
#define ag_rp_279(PCB_POINTER) (SIPPARSER_TIME_ZONE_GMST)
static void ag_rp_280(PCB_DECL, ParserMethod method) {
/* Line 3565, RV SIP Stack */
        RvStatus         status;
        ParserOtherHeader otherHeader;
        otherHeader.name = method.other;
        otherHeader.value.buf = NULL;
        otherHeader.value.len = 0;
        ParserMethodInOtherHeader(&method, &otherHeader);
        status = ParserInitOtherHeader(PCB.pParserMgr,&PCB,PCB.pointer,
                                       &otherHeader,
                                       PCB.eHeaderType,
                                       PCB.pSipObject);
        if(RV_ERROR_ILLEGAL_SYNTAX == status)
        {
             PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
             SYNTAX_ERROR;
        }
        else if (RV_OK != status)
        {
             
             PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
             PCB.eStat = status;
        }
        
    
}
static void ag_rp_281(PCB_DECL, ParserMethod method, ParserBasicToken fv) {
/* Line 3590, RV SIP Stack */
        RvStatus         status;
        ParserOtherHeader otherHeader;
        otherHeader.name = method.other;
        otherHeader.value = fv;
        ParserMethodInOtherHeader(&method, &otherHeader);
        status = ParserInitOtherHeader(PCB.pParserMgr,&PCB,PCB.pointer,
                                       &otherHeader,
                                       PCB.eHeaderType,
                                       PCB.pSipObject);
        if(RV_ERROR_ILLEGAL_SYNTAX == status)
        {
            PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
            SYNTAX_ERROR;
        }
        else if (RV_OK != status)
        {
            
            PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
            PCB.eStat = status;
        }
    
}
static void ag_rp_282(PCB_DECL) {
/* Line 3620, RV SIP Stack */
          RvStatus status; 
          status = ParserInitAuthentication(PCB.pParserMgr,&PCB,PCB.pointer,&(PCB.authentication),PCB.eHeaderType,PCB.eWhichHeader,PCB.pSipObject);
          if(RV_ERROR_ILLEGAL_SYNTAX == status)
          {
              PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
              SYNTAX_ERROR;
          }
          else if (RV_OK != status)
          {
               
               PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
               PCB.eStat = status;
          }
           
          ParserCleanExtParams (PCB.pExtParams);
          ParserCleanExtParams (PCB.pUrlExtParams);
          ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_AUTHENTICATION, &PCB);
          ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_SIP_URL, &PCB);
          ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_EXURI, &PCB);
          ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_DIGEST_CHALLENGE, &PCB);
    
}
static void ag_rp_283(PCB_DECL) {
/* Line 3650, RV SIP Stack */
    
        PCB.authentication.eAuthScheme = RVSIP_AUTH_SCHEME_DIGEST;
        PCB.authentication.isAuthParamList = RV_FALSE;
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_DIGEST_CHALLENGE, &PCB);        
    
}
static void ag_rp_284(PCB_DECL, ParserBasicToken authScheme) {
/* Line 3657, RV SIP Stack */
        
        PCB.authentication.eAuthScheme = RVSIP_AUTH_SCHEME_OTHER;
        PCB.authentication.authScheme  = authScheme;
        PCB.authentication.isRealm =     RV_FALSE;
        PCB.authentication.isDomain =    RV_FALSE;
        PCB.authentication.isNonce =     RV_FALSE;
        PCB.authentication.isOpaque =    RV_FALSE;
        PCB.authentication.isAuthParam = RV_FALSE;
        PCB.authentication.eStale =      RVSIP_AUTH_STALE_UNDEFINED;
        PCB.authentication.algorithm.eAlgorithm = RVSIP_AUTH_ALGORITHM_UNDEFINED;
		PCB.authentication.algorithm.isAKAVersion = RV_FALSE;
		PCB.authentication.qopValue.eAuthQop    = RVSIP_AUTH_QOP_UNDEFINED;
        PCB.authentication.qopValue.isOther = RV_FALSE;
        PCB.authentication.isAuthParamList = RV_TRUE;
		PCB.authentication.isIntegrityKey =	RV_FALSE;
		PCB.authentication.isCipherKey =	RV_FALSE;
        PCB.authentication.authParamList = PCB.pExtParams;
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_DIGEST_CHALLENGE, &PCB);        
    
}
static void ag_rp_285(PCB_DECL) {
/* Line 3685, RV SIP Stack */
        PCB.authentication.eAuthScheme = RVSIP_AUTH_SCHEME_UNDEFINED;
        PCB.authentication.isRealm =     RV_FALSE;
        PCB.authentication.isDomain =    RV_FALSE;
        PCB.authentication.isNonce =     RV_FALSE;
        PCB.authentication.isOpaque =    RV_FALSE;
        PCB.authentication.isAuthParam = RV_FALSE;
        PCB.authentication.eStale =      RVSIP_AUTH_STALE_UNDEFINED;
        PCB.authentication.algorithm.eAlgorithm = RVSIP_AUTH_ALGORITHM_UNDEFINED;
		PCB.authentication.algorithm.isAKAVersion = RV_FALSE;
        PCB.authentication.qopValue.eAuthQop    = RVSIP_AUTH_QOP_UNDEFINED;
        PCB.authentication.qopValue.isOther = RV_FALSE;
		PCB.authentication.isIntegrityKey =	RV_FALSE;
		PCB.authentication.isCipherKey =	RV_FALSE;
        switch (PCB.digestChallenge.eType)
        {
            case PARSER_DIGEST_TYPE_REALM:
            {
                PCB.authentication.isRealm = RV_TRUE;
                PCB.authentication.realm = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_DOMAIN:
            {
                PCB.authentication.isDomain = RV_TRUE;
                PCB.authentication.domain = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_NONCE:
            {
                PCB.authentication.isNonce = RV_TRUE;
                PCB.authentication.nonce = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_OPAQUE:
            {
                PCB.authentication.isOpaque = RV_TRUE;
                PCB.authentication.opaque = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_STALE:
            {
                PCB.authentication.eStale = PCB.digestChallenge.eStale;
                break;
            }
            case PARSER_DIGEST_TYPE_ALGORITHM:
            {
                PCB.authentication.algorithm = PCB.digestChallenge.algorithm;
                break;
            }
			case PARSER_DIGEST_TYPE_INTEGRITY_KEY:
            {
                PCB.authentication.integrityKey = PCB.digestChallenge.challenge;
                break;
            }
			case PARSER_DIGEST_TYPE_CIPHER_KEY:
            {
                PCB.authentication.cipherKey = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_OPQ_OPTIONS:
            {
                PCB.authentication.qopValue = PCB.digestChallenge.qopVal;
                break;
            }
            case PARSER_DIGEST_TYPE_AUTH_PARAM:
            {
                PCB.authentication.isAuthParam = RV_TRUE;
                PCB.authentication.authParam = PCB.digestChallenge.challenge;
                break;
            }
            default:
                    PCB.exit_flag = AG_SEMANTIC_ERROR_CODE;
                    PCB.eStat = RV_ERROR_ILLEGAL_SYNTAX;
        }
    
}
static void ag_rp_286(PCB_DECL) {
/* Line 3763, RV SIP Stack */
        if (PCB.isNewChallenge == RV_FALSE)
        {
            switch (PCB.digestChallenge.eType)
            {
                case PARSER_DIGEST_TYPE_REALM:
                {
                    PCB.authentication.isRealm = RV_TRUE;
                    PCB.authentication.realm = PCB.digestChallenge.challenge;
                    break;
                }
                case PARSER_DIGEST_TYPE_DOMAIN:
                {
                    PCB.authentication.isDomain = RV_TRUE;
                    PCB.authentication.domain = PCB.digestChallenge.challenge;
                    break;
                }
                case PARSER_DIGEST_TYPE_NONCE:
                {
                    PCB.authentication.isNonce = RV_TRUE;
                    PCB.authentication.nonce = PCB.digestChallenge.challenge;
                    break;
                }
                case PARSER_DIGEST_TYPE_OPAQUE:
                {
                    PCB.authentication.isOpaque = RV_TRUE;
                    PCB.authentication.opaque = PCB.digestChallenge.challenge;
                    break;
                }
                case PARSER_DIGEST_TYPE_STALE:
                {
                    PCB.authentication.eStale = PCB.digestChallenge.eStale;
                    break;
                }
                case PARSER_DIGEST_TYPE_ALGORITHM:
                {
                    PCB.authentication.algorithm = PCB.digestChallenge.algorithm;
                    break;
                }
				case PARSER_DIGEST_TYPE_INTEGRITY_KEY:
				{
					PCB.authentication.isIntegrityKey = RV_TRUE;
					PCB.authentication.integrityKey = PCB.digestChallenge.challenge;
					break;
				}
				case PARSER_DIGEST_TYPE_CIPHER_KEY:
				{
					PCB.authentication.isCipherKey = RV_TRUE;
					PCB.authentication.cipherKey = PCB.digestChallenge.challenge;
					break;
				}
                case PARSER_DIGEST_TYPE_OPQ_OPTIONS:
                {
                    PCB.authentication.qopValue = PCB.digestChallenge.qopVal;
                    break;
                }
                case PARSER_DIGEST_TYPE_AUTH_PARAM:
                {
                    PCB.authentication.isAuthParam = RV_TRUE;
                    PCB.authentication.authParam = PCB.digestChallenge.challenge;
                    break;
                }
                default:
                        PCB.exit_flag = AG_SEMANTIC_ERROR_CODE;
                            
                        PCB.eStat = RV_ERROR_ILLEGAL_SYNTAX;
            }
        }
    
}
#define ag_rp_287(PCB_POINTER, realm) (CUR_AUTH_CHALLENGE(PARSER_DIGEST_TYPE_REALM, realm))
#define ag_rp_288(PCB_POINTER, domain) (CUR_AUTH_CHALLENGE(PARSER_DIGEST_TYPE_DOMAIN, domain))
#define ag_rp_289(PCB_POINTER, nonce) (CUR_AUTH_CHALLENGE(PARSER_DIGEST_TYPE_NONCE, nonce))
#define ag_rp_290(PCB_POINTER, opaque) (CUR_AUTH_CHALLENGE(PARSER_DIGEST_TYPE_OPAQUE, opaque))
#define ag_rp_291(PCB_POINTER, ik) (CUR_AUTH_CHALLENGE(PARSER_DIGEST_TYPE_INTEGRITY_KEY, ik))
#define ag_rp_292(PCB_POINTER, ck) (CUR_AUTH_CHALLENGE(PARSER_DIGEST_TYPE_CIPHER_KEY, ck))
#define ag_rp_293(PCB_POINTER, authParam) (CUR_AUTH_CHALLENGE(PARSER_DIGEST_TYPE_AUTH_PARAM, authParam))
static void ag_rp_294(PCB_DECL, RvSipAuthStale stale) {
/* Line 3844, RV SIP Stack */
        PCB.digestChallenge.eType = PARSER_DIGEST_TYPE_STALE;
        PCB.digestChallenge.eStale = stale;
    
}
static void ag_rp_295(PCB_DECL, ParserAlgorithm algorithm) {
/* Line 3849, RV SIP Stack */
        PCB.digestChallenge.eType = PARSER_DIGEST_TYPE_ALGORITHM;
        
        if(PCB.digestChallenge.algorithm.eAlgorithm == RVSIP_AUTH_ALGORITHM_UNDEFINED)
        {
            PCB.digestChallenge.algorithm = algorithm;
        }
    
}
static void ag_rp_296(PCB_DECL, ParserQopValue qopOptions) {
/* Line 3858, RV SIP Stack */
        PCB.digestChallenge.eType = PARSER_DIGEST_TYPE_OPQ_OPTIONS;
        PCB.digestChallenge.qopVal = qopOptions;
    
}
static void ag_rp_297(PCB_DECL) {
/* Line 3864, RV SIP Stack */
        PCB.digestChallenge.eType = PARSER_DIGEST_TYPE_NEW_CHALLENGE;
        PCB.digestChallenge.challenge.buf = "NewChallenge";
        PCB.isNewChallenge = RV_TRUE;
    
}
#define ag_rp_298(PCB_POINTER, t) (t)
#define ag_rp_299(PCB_POINTER, t) (t)
#define ag_rp_300(PCB_POINTER) (CUR_LINE())
#define ag_rp_301(PCB_POINTER) (CUR_LINE())
static void ag_rp_302(PCB_DECL, ParserBasicToken token) {
/* Line 3900, RV SIP Stack */
        RV_UNUSED_ARG(pcb_pointer);
        RV_UNUSED_ARG(token.buf);
    
}
#define ag_rp_303(PCB_POINTER) (SIP_BEGIN_LINE())
#define ag_rp_304(PCB_POINTER) (SIP_BEGIN_LINE())
#define ag_rp_305(PCB_POINTER, t) (t)
#define ag_rp_306(PCB_POINTER, t) (t)
#define ag_rp_307(PCB_POINTER, t) (t)
#define ag_rp_308(PCB_POINTER, t) (t)
#define ag_rp_309(PCB_POINTER) (RVSIP_AUTH_STALE_TRUE)
#define ag_rp_310(PCB_POINTER) (RVSIP_AUTH_STALE_FALSE)
#define ag_rp_311(PCB_POINTER, algo) (algo)
static void ag_rp_312(PCB_DECL) {
/* Line 3962, RV SIP Stack */
		DEFINE_AKAV_PREFIX();
		SIP_BEGIN_TOKEN();			
	
}
static void ag_rp_313(PCB_DECL) {
/* Line 3967, RV SIP Stack */
		DEFINE_AKAV_PREFIX();
		SIP_BEGIN_TOKEN();			
	
}
static void ag_rp_314(PCB_DECL) {
/* Line 3972, RV SIP Stack */
		DEFINE_AKAV_PREFIX();
		SIP_BEGIN_TOKEN();			
	
}
static void ag_rp_315(PCB_DECL) {
/* Line 3977, RV SIP Stack */
		DEFINE_AKAV_PREFIX();
		SIP_BEGIN_TOKEN();			
	
}
static ParserAlgorithm ag_rp_316(PCB_DECL, ParserBasicToken version, ParserAlgorithm algo) {
/* Line 3985, RV SIP Stack */
		algo.isAKAVersion = RV_TRUE;
		algo.nAKAVersion  = version;
		RV_UNUSED_ARG(pcb_pointer);
		return algo;
	
}
static ParserAlgorithm ag_rp_317(PCB_DECL, ParserAlgorithm algo) {
/* Line 3992, RV SIP Stack */
		algo.isAKAVersion = RV_FALSE;
		RV_UNUSED_ARG(pcb_pointer);
		return algo;
	
}
#define ag_rp_318(PCB_POINTER) (SIP_BEGIN_TOKEN())
static ParserAlgorithm ag_rp_319(PCB_DECL) {
/* Line 4004, RV SIP Stack */
        ParserAlgorithm algo;
        algo.eAlgorithm = RVSIP_AUTH_ALGORITHM_MD5;
		algo.algorithm.buf = NULL;
		algo.algorithm.len = 0;
		algo.nAKAVersion.buf = NULL;
		algo.nAKAVersion.len = 0;
		algo.isAKAVersion = RV_FALSE;
        RV_UNUSED_ARG(pcb_pointer);
        return algo;
    
}
static ParserAlgorithm ag_rp_320(PCB_DECL, ParserBasicToken t) {
/* Line 4016, RV SIP Stack */
        ParserAlgorithm algo;
        algo.eAlgorithm = RVSIP_AUTH_ALGORITHM_OTHER;
        algo.algorithm = t;
		algo.nAKAVersion.buf = NULL;
		algo.nAKAVersion.len = 0;
		algo.isAKAVersion = RV_FALSE;
        RV_UNUSED_ARG(pcb_pointer);
        return algo;
    
}
#define ag_rp_321(PCB_POINTER) (SET_PARSER_PCB_BACK_TO_BUFF(RV_SIPPARSER_PREFIX_AKAV))
#define ag_rp_322(PCB_POINTER) (SET_PARSER_PCB_BACK_TO_BUFF(RV_SIPPARSER_PREFIX_ALGORITHM))
#define ag_rp_323(PCB_POINTER) (CUR_STRING())
#define ag_rp_324(PCB_POINTER, qvList) (qvList)
static ParserQopValue ag_rp_325(PCB_DECL, ParserAuthQopOptions val) {
/* Line 4056, RV SIP Stack */
        ParserQopValue qop;
        qop.eAuthQop = RVSIP_AUTH_QOP_OTHER;
        qop.isOther  = RV_FALSE;
		qop.qop = NULL;
		
        if (RV_TRUE == val.isAuth)
        {
            qop.eAuthQop = RVSIP_AUTH_QOP_AUTH_ONLY;    
        }
        else if (RV_TRUE == val.isAuthInt)
        {
            qop.eAuthQop = RVSIP_AUTH_QOP_AUTHINT_ONLY;
        }
        else if (RV_TRUE == val.isOther)
        {
            if (0!=((ParserExtensionString *)PCB.pExtParams)->size)
            {
                APPEND_DATA(",", 1,PCB.pExtParams);
                
                APPEND_DATA(val.strOther.buf, val.strOther.len,PCB.pExtParams);
                qop.isOther = RV_TRUE;
                qop.qop =  PCB.pExtParams;
            }
            else
            {
                
                APPEND_DATA(val.strOther.buf, val.strOther.len,PCB.pExtParams);
                qop.isOther = RV_TRUE;
                qop.qop = PCB.pExtParams;
            }        
        }      
        return qop;        
    
}
static ParserQopValue ag_rp_326(PCB_DECL, ParserQopValue qop, ParserAuthQopOptions val) {
/* Line 4092, RV SIP Stack */
        if (RV_TRUE == val.isAuth && 
            RVSIP_AUTH_QOP_AUTHINT_ONLY == qop.eAuthQop)
        {
            qop.eAuthQop = RVSIP_AUTH_QOP_AUTH_AND_AUTHINT;    
        }
        else if (RV_TRUE == val.isAuth)
        {
            qop.eAuthQop = RVSIP_AUTH_QOP_AUTH_ONLY;    
        }
        else if (RV_TRUE == val.isAuthInt &&
                 RVSIP_AUTH_QOP_AUTH_ONLY == qop.eAuthQop)
        {
            qop.eAuthQop = RVSIP_AUTH_QOP_AUTH_AND_AUTHINT;
        }
        else if (RV_TRUE == val.isAuthInt)
        {
            qop.eAuthQop = RVSIP_AUTH_QOP_AUTHINT_ONLY;
        }
        else if (RV_TRUE == val.isOther)
        {
            if (0!=((ParserExtensionString *)PCB.pExtParams)->size)
            {
                APPEND_DATA(",", 1,PCB.pExtParams);
                
                APPEND_DATA(val.strOther.buf, val.strOther.len,PCB.pExtParams);
                qop.isOther = RV_TRUE;
                qop.qop =  PCB.pExtParams;
            }
            else
            {
                
                APPEND_DATA(val.strOther.buf, val.strOther.len,PCB.pExtParams);
                qop.isOther = RV_TRUE;
                qop.qop = PCB.pExtParams;
            }        
        }
        return qop;
    
}
static ParserAuthQopOptions ag_rp_327(PCB_DECL) {
/* Line 4137, RV SIP Stack */
        ParserAuthQopOptions qopOption;
        qopOption.isAuthInt = RV_TRUE;
        qopOption.isAuth    = RV_FALSE; 
        qopOption.isOther   = RV_FALSE;
		qopOption.strOther.buf = NULL;
		qopOption.strOther.len = 0;
        RV_UNUSED_ARG(pcb_pointer);
        return qopOption;
    
}
static ParserAuthQopOptions ag_rp_328(PCB_DECL) {
/* Line 4148, RV SIP Stack */
        ParserAuthQopOptions qopOption;
        qopOption.isAuth = RV_TRUE;
        qopOption.isAuthInt = RV_FALSE; 
        qopOption.isOther   = RV_FALSE;
		qopOption.strOther.buf = NULL;
		qopOption.strOther.len = 0;
        RV_UNUSED_ARG(pcb_pointer);
        return qopOption;
    
}
static ParserAuthQopOptions ag_rp_329(PCB_DECL, ParserBasicToken t) {
/* Line 4160, RV SIP Stack */
        ParserAuthQopOptions qopOption;
        qopOption.isOther = RV_TRUE;
        qopOption.isAuthInt = RV_FALSE;
        qopOption.isAuth    = RV_FALSE; 
        qopOption.strOther = t;
        RV_UNUSED_ARG(pcb_pointer);
        return qopOption;
    
}
#define ag_rp_330(PCB_POINTER, ik) (ik)
#define ag_rp_331(PCB_POINTER, ck) (ck)
#define ag_rp_332(PCB_POINTER) (CUR_STRING())
#define ag_rp_333(PCB_POINTER) (CUR_STRING())
#define ag_rp_334(PCB_POINTER) (CUR_STRING())
#define ag_rp_335(PCB_POINTER) (CUR_STRING())
#define ag_rp_336(PCB_POINTER) (CUR_STRING())
static void ag_rp_337(PCB_DECL, ParserBasicToken token1, ParserGenericParamDataVal token2) {
/* Line 4202, RV SIP Stack */
      if (0!=((ParserExtensionString *)PCB.pExtParams)->size)
      {
          
          APPEND_DATA(",", 1,PCB.pExtParams);
          APPEND_DATA(token1.buf, token1.len,PCB.pExtParams);
          APPEND_DATA("=" , 1,PCB.pExtParams);
          APPEND_DATA(token2.buf, token2.len,PCB.pExtParams);            
      }
      else
      {      
          
          APPEND_DATA(token1.buf, token1.len,PCB.pExtParams);
          APPEND_DATA("=" , 1,PCB.pExtParams);
          APPEND_DATA(token2.buf, token2.len,PCB.pExtParams);            
      }
   
}
static void ag_rp_338(PCB_DECL) {
/* Line 4229, RV SIP Stack */
   
          RvStatus status; 
          status = ParserInitAuthorization(PCB.pParserMgr,&PCB,PCB.pointer,&(PCB.authorization),PCB.eHeaderType,PCB.eWhichHeader,PCB.pSipObject);
          if(RV_ERROR_ILLEGAL_SYNTAX == status)
          {
              PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
              SYNTAX_ERROR;
          }
          else if (RV_OK != status)
          {
               
               PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
               PCB.eStat = status;
          }
          ParserCleanExtParams (PCB.pExtParams);
          ParserCleanExtParams (PCB.pUrlExtParams);
          ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_AUTHORIZATION, &PCB);
          ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_DIGEST_CHALLENGE, &PCB);
          ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_EXURI, &PCB);
          ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_SIP_URL, &PCB);
    
}
static void ag_rp_339(PCB_DECL) {
/* Line 4260, RV SIP Stack */
        
        PCB.authorization.eAuthScheme = RVSIP_AUTH_SCHEME_DIGEST;  
        PCB.authorization.isAuthParamList = RV_FALSE;
    
}
static void ag_rp_340(PCB_DECL, ParserBasicToken authScheme) {
/* Line 4266, RV SIP Stack */
        
        PCB.authorization.eAuthScheme			= RVSIP_AUTH_SCHEME_OTHER;
        PCB.authorization.authScheme			= authScheme; 
        PCB.authorization.isUsername			= RV_FALSE;
        PCB.authorization.isRealm				= RV_FALSE;
        PCB.authorization.isNonce				= RV_FALSE;
        PCB.authorization.isResponse			= RV_FALSE;
        PCB.authorization.algorithm.eAlgorithm	= RVSIP_AUTH_ALGORITHM_UNDEFINED;   
        PCB.authorization.isCnonce				= RV_FALSE;
        PCB.authorization.isOpaque				= RV_FALSE;
        PCB.authorization.isNonceCount			= RV_FALSE;
        PCB.authorization.eAuthQop				= RVSIP_AUTH_QOP_UNDEFINED;
        PCB.authorization.isAuthParam			= RV_FALSE;
		PCB.authorization.isAuts				= RV_FALSE;
		PCB.authorization.algorithm.isAKAVersion= RV_FALSE;
		PCB.authorization.eProtected			= RVSIP_AUTH_INTEGRITY_PROTECTED_UNDEFINED;
        PCB.authorization.isAuthParamList = RV_TRUE;
        PCB.authorization.authParamList = PCB.pExtParams;
    
}
static void ag_rp_341(PCB_DECL) {
/* Line 4296, RV SIP Stack */
      
        PCB.authorization.isUsername			= RV_FALSE;
        PCB.authorization.isRealm				= RV_FALSE;
        PCB.authorization.isNonce				= RV_FALSE;
        PCB.authorization.isResponse			= RV_FALSE;
        PCB.authorization.algorithm.eAlgorithm	= RVSIP_AUTH_ALGORITHM_UNDEFINED;   
        PCB.authorization.isCnonce				= RV_FALSE;
        PCB.authorization.isOpaque				= RV_FALSE;
        PCB.authorization.isNonceCount			= RV_FALSE;
        PCB.authorization.eAuthQop				= RVSIP_AUTH_QOP_UNDEFINED;
        PCB.authorization.isAuthParam			= RV_FALSE;
		PCB.authorization.isAuts				= RV_FALSE;
		PCB.authorization.algorithm.isAKAVersion= RV_FALSE;
		PCB.authorization.eProtected			= RVSIP_AUTH_INTEGRITY_PROTECTED_UNDEFINED;
        switch (PCB.digestChallenge.eType)
        {
             case PARSER_DIGEST_TYPE_USERNAME:
            {
                PCB.authorization.isUsername = RV_TRUE;
                PCB.authorization.userName = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_REALM:
            {
                PCB.authorization.isRealm = RV_TRUE;
                PCB.authorization.realm = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_NONCE:
            {
                PCB.authorization.isNonce = RV_TRUE;
                PCB.authorization.nonce = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_DIGEST_URI:
            {
                PCB.authorization.isUri = RV_TRUE;
                PCB.authorization.hDigestUri = PCB.digestChallenge.uri;
                break;
            }
            case PARSER_DIGEST_TYPE_DIGEST_RESPONSE:
            {
                PCB.authorization.isResponse = RV_TRUE;
                PCB.authorization.response = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_ALGORITHM:
            {
                PCB.authorization.algorithm = PCB.digestChallenge.algorithm;
                break;
            }
            case PARSER_DIGEST_TYPE_CNONCE:
            {
                PCB.authorization.isCnonce = RV_TRUE;
                PCB.authorization.cnonce = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_OPAQUE:
            {
                PCB.authorization.isOpaque = RV_TRUE;
                PCB.authorization.opaque = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_NONCE_COUNT:
            {
                PCB.authorization.isNonceCount = RV_TRUE;
                PCB.authorization.nonceCount = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_MESSAGE_QOP:
            {
                PCB.authorization.eAuthQop = PCB.digestChallenge.qopVal.eAuthQop;
                break;
            }
            case PARSER_DIGEST_TYPE_AUTH_PARAM:
            {
                PCB.authorization.isAuthParam = RV_TRUE;
                PCB.authorization.authParam = PCB.digestChallenge.challenge;
                break;
            }
			case PARSER_DIGEST_TYPE_AUTS:
            {
                PCB.authorization.isAuts = RV_TRUE;
                PCB.authorization.auts = PCB.digestChallenge.challenge;
                break;
            }
			case PARSER_DIGEST_TYPE_INTEGRITY_PROTECTED:
            {
                PCB.authorization.eProtected = PCB.digestChallenge.eProtected;
                break;
            }
            default:
				PCB.exit_flag = AG_SEMANTIC_ERROR_CODE;
				PCB.eStat = RV_ERROR_ILLEGAL_SYNTAX;
        }
    
}
static void ag_rp_342(PCB_DECL) {
/* Line 4395, RV SIP Stack */
        switch (PCB.digestChallenge.eType)
        {
             case PARSER_DIGEST_TYPE_USERNAME:
            {
                PCB.authorization.isUsername = RV_TRUE;
                PCB.authorization.userName = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_REALM:
            {
                PCB.authorization.isRealm = RV_TRUE;
                PCB.authorization.realm = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_NONCE:
            {
                PCB.authorization.isNonce = RV_TRUE;
                PCB.authorization.nonce = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_DIGEST_URI:
            {
                PCB.authorization.isUri = RV_TRUE;
                PCB.authorization.hDigestUri = PCB.digestChallenge.uri;
                break;
            }
            case PARSER_DIGEST_TYPE_DIGEST_RESPONSE:
            {
                PCB.authorization.isResponse = RV_TRUE;
                PCB.authorization.response = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_ALGORITHM:
            {
                PCB.authorization.algorithm = PCB.digestChallenge.algorithm;
                break;
            }
            case PARSER_DIGEST_TYPE_CNONCE:
            {
                PCB.authorization.isCnonce = RV_TRUE;
                PCB.authorization.cnonce = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_OPAQUE:
            {
                PCB.authorization.isOpaque = RV_TRUE;
                PCB.authorization.opaque = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_NONCE_COUNT:
            {
                PCB.authorization.isNonceCount = RV_TRUE;
                PCB.authorization.nonceCount = PCB.digestChallenge.challenge;
                break;
            }
            case PARSER_DIGEST_TYPE_MESSAGE_QOP:
            {
                PCB.authorization.eAuthQop = PCB.digestChallenge.qopVal.eAuthQop;
                break;
            }
            case PARSER_DIGEST_TYPE_AUTH_PARAM:
            {
                PCB.authorization.isAuthParam = RV_TRUE;
                PCB.authorization.authParam = PCB.digestChallenge.challenge;
                break;
            }
			case PARSER_DIGEST_TYPE_AUTS:
            {
                PCB.authorization.isAuts = RV_TRUE;
                PCB.authorization.auts = PCB.digestChallenge.challenge;
                break;
            }
			case PARSER_DIGEST_TYPE_INTEGRITY_PROTECTED:
            {
                PCB.authorization.eProtected = PCB.digestChallenge.eProtected;
                break;
            }
            default:
                    PCB.exit_flag = AG_SEMANTIC_ERROR_CODE;
                    PCB.eStat = RV_ERROR_ILLEGAL_SYNTAX;
        }
    
}
#define ag_rp_343(PCB_POINTER, useranme) (CUR_AUTH_CHALLENGE(PARSER_DIGEST_TYPE_USERNAME, useranme))
#define ag_rp_344(PCB_POINTER, realm) (CUR_AUTH_CHALLENGE(PARSER_DIGEST_TYPE_REALM, realm))
#define ag_rp_345(PCB_POINTER, nonce) (CUR_AUTH_CHALLENGE(PARSER_DIGEST_TYPE_NONCE, nonce))
#define ag_rp_346(PCB_POINTER, response) (CUR_AUTH_CHALLENGE(PARSER_DIGEST_TYPE_DIGEST_RESPONSE, response))
#define ag_rp_347(PCB_POINTER, cnonce) (CUR_AUTH_CHALLENGE(PARSER_DIGEST_TYPE_CNONCE, cnonce))
#define ag_rp_348(PCB_POINTER, opaque) (CUR_AUTH_CHALLENGE(PARSER_DIGEST_TYPE_OPAQUE, opaque))
#define ag_rp_349(PCB_POINTER, nonceCount) (CUR_AUTH_CHALLENGE(PARSER_DIGEST_TYPE_NONCE_COUNT, nonceCount))
#define ag_rp_350(PCB_POINTER, authParam) (CUR_AUTH_CHALLENGE(PARSER_DIGEST_TYPE_AUTH_PARAM, authParam))
#define ag_rp_351(PCB_POINTER, auts) (CUR_AUTH_CHALLENGE(PARSER_DIGEST_TYPE_AUTS, auts))
static void ag_rp_352(PCB_DECL, RvSipAuthIntegrityProtected protected) {
/* Line 4490, RV SIP Stack */
        PCB.digestChallenge.eType = PARSER_DIGEST_TYPE_INTEGRITY_PROTECTED;
        PCB.digestChallenge.eProtected = protected;
    
}
static void ag_rp_353(PCB_DECL, RvSipAuthQopOption messageQop) {
/* Line 4495, RV SIP Stack */
        PCB.digestChallenge.eType = PARSER_DIGEST_TYPE_MESSAGE_QOP;
        PCB.digestChallenge.qopVal.eAuthQop = messageQop;
    
}
static void ag_rp_354(PCB_DECL) {
/* Line 4500, RV SIP Stack */
        PCB.digestChallenge.eType = PARSER_DIGEST_TYPE_DIGEST_URI;
        PCB.digestChallenge.uri = PCB.exUri;
    
}
static void ag_rp_355(PCB_DECL, ParserAlgorithm algorithm) {
/* Line 4505, RV SIP Stack */
        PCB.digestChallenge.eType = PARSER_DIGEST_TYPE_ALGORITHM;
        
        if(PCB.digestChallenge.algorithm.eAlgorithm == RVSIP_AUTH_ALGORITHM_UNDEFINED)
        {
            PCB.digestChallenge.algorithm = algorithm;
        }
    
}
#define ag_rp_356(PCB_POINTER, username) (username)
#define ag_rp_357(PCB_POINTER, t) (t)
static RvSipAuthQopOption ag_rp_358(PCB_DECL, ParserAuthQopOptions qopVal) {
/* Line 4544, RV SIP Stack */
        RV_UNUSED_ARG(pcb_pointer);
        if (RV_TRUE == qopVal.isAuth)
        {
            return RVSIP_AUTH_QOP_AUTH_ONLY;
        }
        else if (RV_TRUE == qopVal.isAuthInt)
        {
            return RVSIP_AUTH_QOP_AUTHINT_ONLY;
        }
        else if (RV_TRUE == qopVal.isOther)
        {
            return RVSIP_AUTH_QOP_OTHER;
        }
        else 
        {
             return RVSIP_AUTH_QOP_UNDEFINED;
        }
    
}
#define ag_rp_359(PCB_POINTER, cnonceVal) (cnonceVal)
#define ag_rp_360(PCB_POINTER, nonceVal) (nonceVal)
#define ag_rp_361(PCB_POINTER, ncVal) (ncVal)
#define ag_rp_362(PCB_POINTER) (CUR_STRING())
#define ag_rp_363(PCB_POINTER) (CUR_STRING())
#define ag_rp_364(PCB_POINTER) (CUR_STRING())
#define ag_rp_365(PCB_POINTER) (CUR_STRING())
#define ag_rp_366(PCB_POINTER) (CUR_STRING())
#define ag_rp_367(PCB_POINTER) (CUR_STRING())
#define ag_rp_368(PCB_POINTER) (CUR_STRING())
#define ag_rp_369(PCB_POINTER) (CUR_STRING())
#define ag_rp_370(PCB_POINTER, requestDigest) (requestDigest)
#define ag_rp_371(PCB_POINTER) (CUR_STRING())
#define ag_rp_372(PCB_POINTER) (CUR_STRING())
#define ag_rp_373(PCB_POINTER, autsValue) (autsValue)
#define ag_rp_374(PCB_POINTER) (CUR_STRING())
#define ag_rp_375(PCB_POINTER) (RVSIP_AUTH_INTEGRITY_PROTECTED_NO)
#define ag_rp_376(PCB_POINTER) (RVSIP_AUTH_INTEGRITY_PROTECTED_YES)
static void ag_rp_377(PCB_DECL) {
/* Line 4663, RV SIP Stack */
         RvStatus status;
         status = ParserInitRoute(PCB.pParserMgr,&PCB,PCB.pointer,&(PCB.route), PCB.eHeaderType, PCB.pSipObject);
         if(RV_ERROR_ILLEGAL_SYNTAX == status)
         {
             PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
             SYNTAX_ERROR;
         }
         else if (RV_OK != status)
         {
             
             PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
             PCB.eStat = status;
         }
          
         ParserCleanExtParams (PCB.pExtParams);
         ParserCleanExtParams (PCB.pUrlExtParams);
         ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_ROUTE, &PCB);
         ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_NAME_ADDR, &PCB);
         ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_EXURI, &PCB);
         ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_SIP_URL, &PCB);
    
}
static void ag_rp_378(PCB_DECL) {
/* Line 4686, RV SIP Stack */
 
        RvStatus status;
        status = ParserInitRoute(PCB.pParserMgr,&PCB,PCB.pointer,&(PCB.route), PCB.eHeaderType, PCB.pSipObject);
        if(RV_ERROR_ILLEGAL_SYNTAX == status)
        {
             PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
             SYNTAX_ERROR;
        }
        else if (RV_OK != status)
        {
             
             PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
             PCB.eStat = status;
        }
         
        ParserCleanExtParams (PCB.pExtParams);
        ParserCleanExtParams (PCB.pUrlExtParams);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_ROUTE, &PCB);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_NAME_ADDR, &PCB);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_EXURI, &PCB);
        ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_SIP_URL, &PCB);
    
}
static void ag_rp_379(PCB_DECL) {
/* Line 4713, RV SIP Stack */
        PCB.route.nameAddr = PCB.nameAddr;
        PCB.route.isParams = RV_FALSE;
        PCB.route.eRouteType = PCB.eWhichHeader;
    
}
static void ag_rp_380(PCB_DECL) {
/* Line 4718, RV SIP Stack */
 
        PCB.route.nameAddr = PCB.nameAddr;
        PCB.route.isParams = RV_TRUE;
        PCB.route.eRouteType = PCB.eWhichHeader;
          
         if (((ParserExtensionString *)PCB.pExtParams)->size !=0)
         {
             PCB.route.routeParams = PCB.pExtParams;
         }                
     
}
static void ag_rp_381(PCB_DECL) {
/* Line 5971, RV SIP Stack */
   
		RvStatus status; 
		status = ParserInitAuthenticationInfo(PCB.pParserMgr,PCB.pointer,&(PCB.auth_info),PCB.eHeaderType,PCB.pSipObject);
		if(RV_ERROR_ILLEGAL_SYNTAX == status)
        {
            PCB.exit_flag = AG_SYNTAX_ERROR_CODE;
            SYNTAX_ERROR;
        }
        else if (RV_OK != status)
		{
			
			PCB.exit_flag = AG_REDUCTION_ERROR_CODE;
			PCB.eStat = status;
		}
		ParserInitializePCBStructs(SIP_PARSE_PCBSTRUCT_AUTHENTICATION_INFO,&PCB);
    
}
static void ag_rp_382(PCB_DECL, ParserBasicToken nc) {
/* Line 5995, RV SIP Stack */
		PCB.auth_info.isNonceCount  = RV_TRUE;
		PCB.auth_info.strNonceCount = nc;
	
}
static void ag_rp_383(PCB_DECL, ParserBasicToken nv) {
/* Line 6000, RV SIP Stack */
		PCB.auth_info.isNextNonce   = RV_TRUE;
		PCB.auth_info.strNextNonce  = nv;
	
}
static void ag_rp_384(PCB_DECL, ParserBasicToken cn) {
/* Line 6005, RV SIP Stack */
		PCB.auth_info.isCNonce      = RV_TRUE;
		PCB.auth_info.strCNonce     = cn;
	
}
static void ag_rp_385(PCB_DECL, ParserBasicToken rd) {
/* Line 6010, RV SIP Stack */
		PCB.auth_info.isRspAuth     = RV_TRUE;
		PCB.auth_info.strRspAuth    = rd;
	
}
static void ag_rp_386(PCB_DECL) {
/* Line 6015, RV SIP Stack */
		PCB.auth_info.isMsgQop      = RV_TRUE;
	
}
static void ag_rp_387(PCB_DECL, ParserAuthQopOptions qopVal) {
/* Line 6023, RV SIP Stack */
		if (RV_TRUE == qopVal.isAuth)
        {
            PCB.auth_info.eMsgQop = RVSIP_AUTH_QOP_AUTH_ONLY;
        }
        else if (RV_TRUE == qopVal.isAuthInt)
        {
            PCB.auth_info.eMsgQop = RVSIP_AUTH_QOP_AUTHINT_ONLY;
        }
        else if (RV_TRUE == qopVal.isOther)
        {
            PCB.auth_info.eMsgQop = RVSIP_AUTH_QOP_OTHER;
        }
        else 
        {
	    PCB.auth_info.eMsgQop = RVSIP_AUTH_QOP_UNDEFINED;
        }
		
		PCB.auth_info.strMsgQop = qopVal.strOther;
	
}
#define ag_rp_388(PCB_POINTER, nv) (nv)
#define ag_rp_389(PCB_POINTER, val) (val)
#define ag_rp_390(PCB_POINTER) (CUR_STRING())
#define READ_COUNTS 
#define WRITE_COUNTS 
#undef V
#define V(i,t) (*(t *) (&(PCB).vs[(PCB).ssx + i]))
#undef VS
#define VS(i) (PCB).vs[(PCB).ssx + i]
#ifndef GET_CONTEXT
#define GET_CONTEXT CONTEXT = (PCB).input_context
#endif
typedef enum {
  ag_action_1,
  ag_action_2,
  ag_action_3,
  ag_action_4,
  ag_action_5,
  ag_action_6,
  ag_action_7,
  ag_action_8,
  ag_action_9,
  ag_action_10,
  ag_action_11,
  ag_action_12
} ag_parser_action;
#ifndef NULL_VALUE_INITIALIZER
#define NULL_VALUE_INITIALIZER = { 0 }
#endif
static SipParser_vs_type const ag_null_value NULL_VALUE_INITIALIZER;
static const unsigned short ag_rpx[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  1,  2,  3,  4,  0,  0,  5,  6,  7,  8,  9, 10, 11, 12,
   13, 14,  0,  0, 15,  0,  0, 16,  0,  0, 17,  0,  0,  0,  0,  0, 18,  0,
    0,  0,  0,  0, 19,  0,  0,  0,  0, 20, 21, 22, 23, 24, 25, 26, 27, 28,
    0,  0, 29, 30, 31, 32, 33, 34, 35, 36, 37,  0, 38,  0, 39, 40, 41, 42,
   43,  0,  0, 44, 45, 46, 47,  0,  0, 48, 49, 50,  0,  0,  0,  0,  0,  0,
   51, 52,  0, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67,
   68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,  0, 80, 81, 82, 83,  0,
    0, 84, 85, 86, 87, 88, 89,  0, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
    0,100,101,  0,  0,102,103,  0,  0,104,  0,  0,105,106,107,108,  0,109,
  110,111,112,113,114,115,116,117,118,119,  0,  0,  0,  0,120,121,122,123,
    0,124,125,  0,126,127,128,129,130,131,132,  0,  0,133,134,  0,  0,  0,
    0,  0,  0,135,136,137,138,139,140,141,  0,  0,142,143,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,144,
  145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,
  163,164,165,166,167,168,169,  0,  0,  0,  0,170,171,172,173,174,175,  0,
    0,  0,  0,  0,176,177,178,179,180,181,182,  0,  0,183,  0,  0,  0,  0,
    0,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,
  201,202,203,204,205,206,207,208,209,210,211,212,213,  0,  0,  0,  0,  0,
  214,215,216,217,218,219,220,  0,221,  0,  0,222,223,224,225,226,227,  0,
    0,228,229,230,231,232,  0,  0,233,234,  0,  0,  0,  0,235,236,237,238,
    0,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,
  256,257,258,259,260,261,262,263,264,265,266,267,268,269,270,271,272,273,
  274,275,276,277,278,279,280,281,282,  0,283,284,285,286,287,288,289,290,
  291,292,293,294,295,296,297,298,299,  0,  0,300,301,  0,  0,302,303,304,
    0,  0,  0,305,306,307,308,309,310,  0,  0,311,312,313,314,315,316,317,
  318,319,320,321,322,323,324,325,326,327,328,329,330,331,332,333,334,335,
  336,  0,  0,  0,  0,337,338,  0,339,340,341,342,343,344,345,346,347,348,
  349,350,351,352,353,354,355,356,357,  0,  0,  0,  0,358,359,360,361,362,
  363,364,365,366,367,368,369,370,371,372,  0,  0,  0,373,  0,  0,  0,  0,
  374,  0,  0,375,376,  0,377,378,379,380,381,  0,  0,382,383,384,385,386,
  387,388,389,  0,  0,  0,  0,  0,390
};
static const unsigned char ag_key_itt[] = {
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0,
 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1,
 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0
};
static const unsigned short ag_key_pt[] = {
  0,  2,  0,  6,  0,  8,  0, 10,  0, 12,  0, 14,  0, 16,  0, 18,
  0, 20,  0, 22,  0, 24,  0, 26,  0, 30,  0, 90,  0, 91,  0, 92,
  0, 93,  0, 94,  0,131,  0,133,  0,134,  0,135,  0,136,  0,150,
  0,151,  0,153,  0,157,  0,159,  0,160,  0,162,  0,163,  0,165,
  0,248,  0,249,  0,252,  0,253,  0,254,  0,255,  0,256,  0,257,
  0,258,  0,261,  0,262,  0,282,  0,283,  0,284,  0,285,  0,286,
  0,287,  0,288,  0,289,  0,290,  0,291,  0,292,  0,293,  0,294,
  0,304,  0,309,  0,316,  0,322,  0,324,  0,325,  0,327,  0,335,
  0,336,  0,337,  0,338,  0,339,  0,340,  0,341,  0,345,  0,346,
  0,347,  0,348,  0,349,  0,350,  0,351,  0,352,  0,353,  0,354,
  0,355,  0,356,  0,361,  0,362,  0,363,  0,366,  0,381,  0,383,
  0,392,  0,394,  0,396,  0,397,  0,401,  0,410,  0,413,  0,416,
  0,417,  0,418,  0,420,  0,434,  0,436,  0,439,  0,441,  0,444,
  0,449,  0,457,  0,458,  0,459,  0,467,  0,468,0
};
static const unsigned char ag_key_ch[] = {
    0, 49, 50, 67, 73,255, 89,255, 65, 73, 76, 79, 83,255, 77, 78,255, 69,
  255, 82,255, 69, 72,255, 47, 58, 83,255, 80,255, 73,255, 82,255, 73,255,
   65, 66, 67, 68, 69, 70, 73, 75, 76, 77, 79, 80, 82, 83, 84, 85, 86, 88,
  255, 32,255, 72, 85,255, 72, 82, 85,255, 75, 84,255, 58, 82,255, 79,255,
   71, 73,255, 45,255, 72, 83,255, 71, 84,255, 67, 75, 76, 80, 85,255, 82,
   89,255, 65, 75, 78, 79,255, 69, 73, 79,255, 65, 69, 82,255, 83, 84,255,
   77,255, 84, 86,255, 75, 77, 78, 80,255, 76, 78,255, 65, 85,255, 68, 82,
   89,255, 65, 69, 79,255, 78, 86,255, 67, 69, 79,255, 67, 80, 82,255, 69,
   79,255, 72, 82,255, 79,255, 65, 67, 68, 71, 83,255, 69, 80, 83,255, 58,
   83,255, 71, 80,255, 65, 67, 69, 73, 84, 85,255, 65, 85,255, 65, 67, 72,
   76, 79, 82, 84, 85,255, 78,255, 82,255, 69,255, 68, 82, 83, 84,255, 42,
   58, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 76, 77, 78, 79, 80, 81, 82,
   83, 84, 85, 87, 89,255, 58, 83,255, 80,255, 73,255, 73, 80, 83,255, 67,
   69,255, 58, 83,255, 80,255, 65, 73, 85,255, 72, 85,255, 42, 67, 68, 70,
   73, 77, 78, 80, 81, 82, 83, 84, 87,255, 67, 69,255, 67, 78, 81, 82,255,
   68,255, 83,255, 58, 83,255, 80,255, 73,255, 42, 73, 80, 83,255, 65, 85,
  255, 72, 85,255, 70, 77, 83, 84, 87,255, 58,255, 65, 69,255, 79, 82, 84,
  255, 67, 76, 77, 79, 84, 85,255, 83,255, 76, 85,255, 67, 79,255, 65, 83,
  255, 69,255, 82, 83,255, 65, 67, 73, 78, 79, 81, 82, 85,255, 65, 67, 68,
   73, 78, 79, 81, 82, 83,255, 65, 69, 81,255, 80, 85,255, 76, 78,255, 65,
   85,255, 82, 89,255, 65,255, 65, 68, 70, 74, 77, 78, 79, 83,255, 84,255,
   65, 66, 67, 73, 82,255, 45,255, 72,255, 84,255, 85,255, 65,255, 75, 76,
  255, 65,255, 32, 49, 79, 84,255, 77,255, 83,255, 73, 80,255, 67, 76,255,
   83, 84, 85,255, 78, 89,255, 70, 84,255, 69, 80,255, 65, 66, 67, 72, 77,
   82, 84,255, 80, 82,255, 58, 83,255, 80,255, 73,255, 83,255, 61,255, 61,
  255, 83, 84,255, 77,255, 71, 85,255
};
static const unsigned char ag_key_act[] = {
  0,5,5,7,5,4,7,4,7,5,5,5,5,4,3,7,4,3,4,6,4,7,5,4,3,0,3,4,2,4,2,4,5,4,5,
  4,2,6,6,7,7,5,6,5,5,5,5,2,6,2,5,6,6,5,4,0,4,3,3,4,3,3,3,4,5,7,4,0,7,4,
  2,4,2,7,4,7,4,6,5,4,5,2,4,2,3,2,7,2,4,7,7,4,7,5,7,7,4,7,7,7,4,7,7,7,4,
  7,5,4,2,4,7,7,4,5,3,2,5,4,5,5,4,7,2,4,7,5,5,4,2,7,7,4,7,5,4,5,7,6,4,7,
  7,7,4,3,7,4,7,2,4,7,4,7,7,7,7,7,4,2,7,7,4,0,3,4,7,6,4,7,7,7,2,7,7,4,7,
  7,4,7,7,7,7,7,2,7,7,4,7,4,6,4,2,4,7,7,2,7,4,5,3,2,2,2,2,7,2,2,7,2,2,7,
  2,2,2,2,6,2,2,2,2,7,7,4,0,3,4,2,4,2,4,3,3,2,4,5,7,4,0,3,4,6,4,7,2,7,4,
  7,7,4,5,7,7,7,3,7,2,3,7,7,2,2,7,4,5,7,4,7,2,7,7,4,7,4,7,4,0,3,4,2,4,2,
  4,5,3,3,2,4,7,7,4,7,7,4,7,7,2,2,7,4,3,4,7,7,4,7,7,7,4,7,7,2,7,2,7,4,3,
  4,7,7,4,5,7,4,7,7,4,2,4,7,7,4,2,7,7,2,7,7,2,2,4,7,7,7,7,7,7,7,7,7,4,7,
  7,5,4,7,7,4,5,5,4,7,2,4,5,5,4,2,4,2,7,7,2,2,7,7,7,4,7,4,7,7,7,7,7,4,7,
  4,6,4,2,4,2,4,2,4,3,3,4,2,4,0,5,7,7,4,7,4,7,4,7,7,4,7,7,4,7,2,7,4,7,7,
  4,7,7,4,7,7,4,7,7,7,7,7,2,7,4,7,7,4,0,3,4,2,4,2,4,2,4,0,4,1,4,7,5,4,2,
  4,2,7,4
};
static const unsigned short ag_key_parm[] = {
    0, 18, 20, 28, 22,  0, 30,  0, 32, 10, 12,  8,  0,  0,193, 26,  0,191,
    0,  2,  0, 34, 16,  0,116,186,187,  0,  0,  0,  0,  0, 24,  0, 14,  0,
    0,100, 90,  6,  4, 96, 88,102,110, 86,106,  0, 98,  0, 94,108, 92,104,
    0,391,  0,218,206,  0,218,190,206,  0, 28,118,  0,412,184,  0,  0,  0,
    0, 48,  0,190,  0,192,208,  0,154,  0,  0,  0,411,  0,146,  0,  0, 58,
   30,  0, 32,196,202, 60,  0,162,170,174,  0,182,142,136,  0,168,164,  0,
    0,  0,210, 26,  0,194,193,  0, 70,  0,152,150,  0,140,  0,  0, 54,144,
  148,  0,  0, 72,128,  0,176,160,  0,204,216,212,  0,158,178, 84,  0,191,
  120,  0, 68,  0,  0,188,  0,172, 56,122, 34,206,  0,  0, 52,218,  0,186,
  187,  0, 62, 36,  0,138, 42,156,  0,180,126,  0, 64, 78,  0,112, 40,134,
   44, 82,  0, 50,130,  0,198,  0, 66,  0,  0,  0, 38,200,  0,166,  0,114,
  230,  0,  0,  0,  0,124,  0,  0, 46,  0,  0, 74,  0,  0,  0,  0,116,  0,
    0,  0,  0,132,214,  0,186,187,  0,  0,  0,  0,  0,193,191,  0,  0,204,
  216,  0,186,187,  0, 36,  0,138,  0,126,  0,134,130,  0,114,202,170,136,
  193,128,  0,191,188,218,  0,  0,132,  0,204,216,  0,202,  0,188,218,  0,
  170,  0, 36,  0,186,187,  0,  0,  0,  0,  0,114,193,191,  0,  0,138,126,
    0,134,130,  0,136,128,  0,  0,132,  0,230,  0, 54, 72,  0, 82, 64, 50,
    0, 60, 74,  0, 84,  0, 66,  0,116,  0,184,208,  0,204,176,  0,172,206,
    0,  0,  0,200,198,  0,  0,202,210,  0,178,188,  0,  0,  0,184,196,174,
  194,176,178,188,172,180,  0,118,124,116,  0,146,154,  0,152,150,  0,140,
    0,  0,144,148,  0,  0,  0,  0,162,142,  0,  0,160,158,156,  0,112,  0,
   28, 30, 32, 26, 34,  0,190,  0,192,  0,  0,  0,  0,  0,  0,  0,411,412,
    0,  0,  0,391, 76, 80, 78,  0,186,  0, 62,  0, 70, 68,  0, 40, 44,  0,
   42,  0, 38,  0,212,214,  0,182, 78,  0, 56, 52,  0, 48, 58, 60, 46, 54,
    0, 50,  0,120,122,  0,186,187,  0,  0,  0,  0,  0,  0,  0,456,  0,455,
    0,168,164,  0,  0,  0,  0,166,  0
};
static const unsigned short ag_key_jmp[] = {
    0,  0,  0,  0,  0,  0,  2,  0,  4,  0,  0,  0,  0,  0, 13, 15,  0, 20,
    0, 17,  0, 23,  0,  0, 30,  0, 34,  0, 24,  0, 28,  0,  0,  0,  0,  0,
    1,  6,  8,  9, 11,  0, 14,  0,  0,  0,  0, 19, 21, 30,  0, 32, 34,  0,
    0,  0,  0, 36, 41,  0, 50, 55, 62,  0,  0, 73,  0,  0, 83,  0, 67,  0,
   70, 88,  0, 93,  0, 75,  0,  0,  0, 77,  0, 64, 77, 72, 91, 80,  0, 97,
  102,  0,104,  0,109,114,  0,117,119,124,  0,136,140,142,  0,144,  0,  0,
  105,  0,154,171,  0,  0,152,110,  0,  0,  0,  0,  0,175,118,  0,179,  0,
    0,  0,124,182,187,  0,197,  0,  0,  0,189,132,  0,200,202,207,  0,214,
  217,  0,210,143,  0,220,  0,222,225,231,237,243,  0,151,249,253,  0,  0,
  271,  0,266,161,  0,259,261,264,164,273,277,  0,298,305,  0,279,281,283,
  285,287,174,307,309,  0,315,  0,186,  0,188,  0,311,313,190,319,  0,  0,
   71, 83, 89, 92, 97,129,101,108,146,113,121,177,128,135,139,146,149,157,
  167,177,192,321,324,  0,  0,335,  0,222,  0,225,  0,327,330,227,  0,  0,
  358,  0,  0,383,  0,236,  0,381,239,385,  0,387,389,  0,  0,337,343,349,
  352,355,233,366,371,374,241,245,391,  0,  0,400,  0,394,262,408,411,  0,
  418,  0,424,  0,  0,435,  0,274,  0,277,  0,  0,427,430,279,  0,443,445,
    0,447,449,  0,437,440,286,289,451,  0,454,  0,462,466,  0,475,486,494,
    0,456,460,300,471,303,496,  0,500,  0,507,515,  0,  0,543,  0,556,559,
    0,322,  0,565,567,  0,316,518,524,319,547,553,325,327,  0,574,583,585,
  591,593,598,604,607,612,  0,617,623,  0,  0,630,632,  0,  0,  0,  0,640,
  356,  0,  0,  0,  0,362,  0,353,634,637,359,365,642,645,648,  0,651,  0,
  654,657,660,666,672,  0,680,  0,384,  0,386,  0,388,  0,390,  0,684,690,
    0,394,  0,  0,  0,694,696,  0,700,  0,703,  0,710,712,  0,721,723,  0,
  717,411,725,  0,728,730,  0,733,738,  0,768,775,  0,742,747,753,757,763,
  424,779,  0,782,787,  0,  0,795,  0,438,  0,441,  0,443,  0,  0,  0,447,
    0,797,  0,  0,451,  0,454,799,  0
};
static const unsigned short ag_key_index[] = {
   36, 55, 57, 60, 60,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 55, 55,  0,  0,197,  0,229,229,
    0,248,  0,265,270,270,  0,272,  0,  0,281,292,292,229,  0, 55,298, 55,
    0,  0,  0,  0,307, 55, 55, 55,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,229,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 60, 60, 57,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0, 55,298,298,  0,298,  0,  0,  0,  0,  0,307,307,  0, 55,  0,  0,314,
    0,  0,  0,  0,  0,  0,  0,  0,  0,330,  0,339,  0,  0,  0,  0,  0,229,
  229,  0,  0,  0,  0,272,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,349,  0,
    0,229,  0,367,  0,376,  0,  0,378,298,  0,298,  0,  0, 55,  0, 55, 55,
    0,  0, 55,  0,  0,  0,  0,  0,307,  0,  0,  0,  0,  0,  0,  0,392,  0,
    0,  0,  0,265,  0,  0,  0,397,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,229,  0,  0,  0,  0,  0,
    0,272,  0,  0,  0,  0,  0,  0,  0,  0,349,349,  0,229,  0,  0,  0,  0,
  376,376,  0,298,  0,  0, 55,  0,  0,  0, 55,399,  0,  0,  0,  0,  0, 55,
    0,  0,392,  0,  0,  0,  0,265,  0,  0,  0,404,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,330,  0,  0,  0,  0,  0,  0,339,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,349,  0,  0,  0,  0,  0,376,
    0,  0,  0,  0,406,378,  0,  0,408,414, 55,392,  0,  0,  0,  0,397,  0,
    0,  0,392,418,  0,  0,  0,  0,  0,  0,  0,  0,330,  0,421,  0,  0,  0,
    0,339,  0,  0,  0,  0,  0,427,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,406,378,  0,  0,408,414,  0,  0,  0,  0,  0,397,  0,  0,  0,  0,392,
  418,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,421,  0,  0,  0,  0,  0,
    0,  0,427,  0,  0,435,  0,  0,  0,  0,  0,  0, 55,  0,  0,  0,  0,404,
  229,392,  0,  0,  0,  0,  0,392,  0,  0,445,  0,414,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,435,  0,  0,  0,  0,  0, 55,  0,  0,  0,449,
    0,  0,  0,  0,  0,445,445,  0,414,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  292,  0,  0,456,  0,  0,  0,  0, 55,  0,  0,  0,  0,  0,406,  0,  0,298,
  292,  0,  0,  0,392,445,  0,  0,  0,  0,406,  0,298,  0,  0,  0,  0,392,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};
static const unsigned char ag_key_ends[] = {
75,0, 69,0, 78,67,69,76,0, 84,0, 88,0, 58,0, 86,73,84,69,0, 
83,58,0, 71,73,83,84,69,82,0, 50,46,48,0, 58,0, 79,83,84,58,0, 
83,69,82,73,78,70,79,58,0, 79,83,84,58,0, 86,45,65,66,83,58,0, 
83,69,82,73,78,70,79,58,0, 58,0, 73,79,78,0, 65,86,69,82,58,0, 
73,84,72,77,0, 65,83,0, 82,0, 73,78,84,0, 65,78,67,72,0, 69,0, 
78,67,69,76,0, 79,78,67,69,0, 77,80,0, 67,0, 71,69,83,84,0, 
77,65,73,78,0, 88,80,73,82,69,83,0, 76,83,69,0, 66,0, 73,0, 
84,0, 73,68,68,69,78,0, 58,0, 
69,71,82,73,84,89,45,80,82,79,84,69,67,84,69,68,0, 73,84,69,0, 
78,0, 82,0, 68,82,0, 84,72,79,68,0, 78,0, 
88,84,78,79,78,67,69,0, 67,69,0, 84,0, 65,81,85,69,0, 73,71,0, 
79,78,69,0, 83,58,0, 88,89,0, 80,0, 76,77,0, 69,73,86,69,68,0, 
73,82,69,67,84,0, 73,83,84,69,82,0, 80,79,78,83,69,0, 79,82,84,0, 
80,65,85,84,72,0, 84,0, 84,80,0, 80,0, 67,79,77,80,0, 58,0, 
65,76,69,0, 78,0, 71,0, 80,0, 85,0, 83,0, 
75,69,78,73,90,69,68,45,66,89,0, 78,83,80,79,82,84,0, 69,0, 76,0, 
69,0, 80,0, 73,0, 65,77,69,0, 67,0, 69,68,0, 69,83,0, 77,58,0, 
82,69,83,58,0, 58,0, 78,79,78,67,69,0, 73,71,69,83,84,0, 82,73,0, 
77,58,0, 79,78,0, 88,84,78,79,78,67,69,0, 82,69,83,58,0, 79,80,0, 
83,80,65,85,84,72,0, 84,0, 58,0, 78,0, 85,0, 69,0, 69,68,0, 
78,79,78,67,69,0, 88,84,78,79,78,67,69,0, 79,80,0, 
83,80,65,85,84,72,0, 73,71,69,83,84,0, 73,80,0, 77,58,0, 
82,69,83,58,0, 58,0, 82,73,0, 79,78,0, 84,0, 78,0, 85,0, 69,0, 
69,68,0, 58,0, 79,77,80,0, 82,0, 68,68,82,0, 84,72,79,68,0, 
82,73,71,0, 75,69,78,73,90,69,68,45,66,89,0, 
65,78,83,80,79,82,84,0, 76,0, 83,69,82,0, 73,80,47,50,46,48,0, 
71,79,82,73,84,72,77,0, 84,83,0, 78,79,78,67,69,0, 
78,84,69,71,82,73,84,89,45,80,82,79,84,69,67,84,69,68,0, 78,67,69,0, 
80,65,81,85,69,0, 79,80,0, 76,77,0, 80,79,78,83,69,0, 73,0, 
69,82,78,65,77,69,0, 76,71,79,82,73,84,72,77,0, 75,0, 
79,77,65,73,78,0, 75,0, 79,78,67,69,0, 80,65,81,85,69,0, 79,80,0, 
69,65,76,77,0, 84,65,76,69,0, 67,84,73,79,78,0, 
88,80,73,82,69,83,0, 82,0, 71,0, 69,67,0, 69,66,0, 78,0, 
79,86,0, 67,84,0, 69,80,0, 65,71,0, 67,75,0, 89,69,0, 
65,78,67,69,76,0, 78,86,73,84,69,0, 69,71,73,83,84,69,82,0, 
73,78,84,0, 65,86,69,82,58,0, 71,79,58,0, 78,0, 82,85,69,0, 
68,53,0, 73,71,67,79,77,80,0, 80,0, 72,79,78,69,0, 67,84,80,0, 
80,0, 83,0, 68,80,0, 79,0, 69,83,0, 65,76,83,69,0, 82,85,69,0, 
76,73,65,83,0, 82,65,78,67,72,0, 79,77,80,0, 73,68,68,69,78,0, 
65,68,68,82,0, 67,69,73,86,69,68,0, 79,82,84,0, 84,76,0, 
82,79,88,89,0, 69,68,73,82,69,67,84,0, 58,0, 84,0, 84,67,0, 
};
#define AG_TCV(x) ag_tcv[(x)]
static const unsigned short ag_tcv[] = {
    5,487,487,487,487,487,487,487,487,473,  0,487,487,  0,487,487,487,487,
  487,487,487,487,487,487,487,487,487,487,487,487,487,487, 40,474, 60,484,
  485,474,271,474,481,481,474,475, 42, 62,227, 56,318,321,476,476,476,476,
  476,476,476,476, 36, 52, 58, 47,298, 54, 49,407,477,477,477,477,477,478,
  478,478,478,408,478,478,478,478,478,478,478,478,478,478,409,478,478,478,
  478,223, 84,224,486,479,480,407,477,477,477,477,477,478,478,478,478,408,
  478,478,478,478,478,478,478,478,478,478,409,478,478,478,478,482,486,482,
  474,487,486,486,486,486,486,486,486,486,486,486,486,486,486,486,486,486,
  486,483,483,486,486,486,486,486,483,486,486,486,486,486,486,486,486,486,
  486,486,486,486,486,486,486,486,486,486,486,486,486,486,486,486,486,486,
  486,486,486,486,486,486,486,486,486,486,486,486,486,486,486,486,486,486,
  486,486,486,486,486,486,486,486,486,486,486,486,486,486,486,486,486,486,
  486,486,486,486,486,486,486,486,486,486,486,486,486,486,486,486,486,486,
  486,486,486,486,486,486,486,486,486,486,486,486,486,486,486,486,486,486,
  486,486,486,486
};
#ifndef SYNTAX_ERROR
#define SYNTAX_ERROR fprintf(stderr,"%s, line %d, column %d\n", \
  (PCB).error_message, (PCB).line, (PCB).column)
#endif
#ifndef FIRST_LINE
#define FIRST_LINE 1
#endif
#ifndef FIRST_COLUMN
#define FIRST_COLUMN 1
#endif
#ifndef PARSER_STACK_OVERFLOW
#define PARSER_STACK_OVERFLOW {fprintf(stderr, \
   "\nParser stack overflow, line %d, column %d\n",\
   (PCB).line, (PCB).column);}
#endif
#ifndef REDUCTION_TOKEN_ERROR
#define REDUCTION_TOKEN_ERROR {fprintf(stderr, \
    "\nReduction token error, line %d, column %d\n", \
    (PCB).line, (PCB).column);}
#endif
#ifndef INPUT_CODE
#define INPUT_CODE(T) (T)
#endif
typedef enum
  {ag_accept_key, ag_set_key, ag_jmp_key, ag_end_key, ag_no_match_key,
   ag_cf_accept_key, ag_cf_set_key, ag_cf_end_key} key_words;
static void ag_get_key_word(PCB_DECL, int ag_k) {
  int ag_save = (int) ((PCB).la_ptr - (PCB).pointer);
  const  unsigned char *ag_p;
  int ag_ch;
  while (1) {
    switch (ag_key_act[ag_k]) {
    case ag_cf_end_key: {
      const  unsigned char *sp = ag_key_ends + ag_key_jmp[ag_k];
      do {
        if ((ag_ch = *sp++) == 0) {
          int ag_k1 = ag_key_parm[ag_k];
          int ag_k2 = ag_key_pt[ag_k1];
          if (ag_key_itt[ag_k2 + CONVERT_CASE(*(PCB).la_ptr)]) goto ag_fail;
          (PCB).token_number = (SipParser_token_type) ag_key_pt[ag_k1 + 1];
          return;
        }
      } while (CONVERT_CASE(*(PCB).la_ptr++) == ag_ch);
      goto ag_fail;
    }
    case ag_end_key: {
      const  unsigned char *sp = ag_key_ends + ag_key_jmp[ag_k];
      do {
        if ((ag_ch = *sp++) == 0) {
          (PCB).token_number = (SipParser_token_type) ag_key_parm[ag_k];
          return;
        }
      } while (CONVERT_CASE(*(PCB).la_ptr++) == ag_ch);
    }
    case ag_no_match_key:
ag_fail:
      (PCB).la_ptr = (PCB).pointer + ag_save;
      return;
    case ag_cf_set_key: {
      int ag_k1 = ag_key_parm[ag_k];
      int ag_k2 = ag_key_pt[ag_k1];
      ag_k = ag_key_jmp[ag_k];
      if (ag_key_itt[ag_k2 + CONVERT_CASE(*(PCB).la_ptr)]) break;
      ag_save = (int) ((PCB).la_ptr - (PCB).pointer);
      (PCB).token_number = (SipParser_token_type) ag_key_pt[ag_k1+1];
      break;
    }
    case ag_set_key:
      ag_save = (int) ((PCB).la_ptr - (PCB).pointer);
      (PCB).token_number = (SipParser_token_type) ag_key_parm[ag_k];
    case ag_jmp_key:
      ag_k = ag_key_jmp[ag_k];
      break;
    case ag_accept_key:
      (PCB).token_number = (SipParser_token_type) ag_key_parm[ag_k];
      return;
    case ag_cf_accept_key: {
      int ag_k1 = ag_key_parm[ag_k];
      int ag_k2 = ag_key_pt[ag_k1];
      if (ag_key_itt[ag_k2 + CONVERT_CASE(*(PCB).la_ptr)])
        (PCB).la_ptr = (PCB).pointer + ag_save;
      else (PCB).token_number = (SipParser_token_type) ag_key_pt[ag_k1+1];
      return;
    }
    }
    ag_ch = CONVERT_CASE(*(PCB).la_ptr++);
    ag_p = &ag_key_ch[ag_k];
    while (*ag_p < ag_ch) ag_p++;
    if (*ag_p != ag_ch) {
      (PCB).la_ptr = (PCB).pointer + ag_save;
      return;
    }
    ag_k = (int) (ag_p - ag_key_ch);
  }
}
#ifndef AG_NEWLINE
#define AG_NEWLINE 10
#endif
#ifndef AG_RETURN
#define AG_RETURN 13
#endif
#ifndef AG_FORMFEED
#define AG_FORMFEED 12
#endif
#ifndef AG_TABCHAR
#define AG_TABCHAR 9
#endif
static void ag_track(PCB_DECL) {
  int ag_k = (int) ((PCB).la_ptr - (PCB).pointer);
  while (ag_k--) {
    switch (*(PCB).pointer++) {
    case AG_NEWLINE:
      (PCB).column = 1, (PCB).line++;
    case AG_RETURN:
    case AG_FORMFEED:
      break;
    case AG_TABCHAR:
      (PCB).column += (TAB_SPACING) - ((PCB).column - 1) % (TAB_SPACING);
      break;
    default:
      (PCB).column++;
    }
  }
}
static const unsigned short ag_tstt[] = {
480,479,478,477,476,475,474,409,408,407,321,318,294,293,292,291,290,289,288,
  287,286,285,284,283,282,227,193,191,187,186,116,94,93,92,91,90,62,30,26,
  24,22,20,18,16,14,12,10,8,6,2,0,1,28,29,32,33,34,35,63,65,89,112,181,
  182,183,184,188,192,280,281,
480,479,478,477,476,475,474,409,408,407,321,318,227,62,0,
218,206,0,185,194,197,203,217,
218,206,190,0,185,189,194,197,203,217,
218,206,190,0,185,189,194,197,203,217,
473,40,36,0,37,38,39,45,
40,0,39,113,
5,0,
5,0,
5,0,
5,0,
36,0,3,
5,0,
5,0,
36,0,3,
36,0,3,
36,0,3,
36,0,3,
36,0,3,
36,0,3,
36,0,3,
36,0,3,
36,0,3,
36,0,3,
36,0,3,
36,0,3,
478,477,476,409,408,407,321,318,223,0,138,149,205,219,
485,484,481,479,478,477,476,475,474,409,408,407,321,318,271,227,62,56,54,52,
  49,47,42,0,48,204,207,209,211,
473,391,298,60,54,52,40,5,0,51,195,236,
473,391,298,60,54,52,40,5,0,51,195,236,
478,477,476,475,409,408,407,321,318,227,62,0,169,170,173,
478,477,476,475,409,408,407,321,318,227,62,0,169,170,173,
473,40,0,
36,0,50,
478,477,476,475,473,409,408,407,321,318,227,193,191,187,186,62,40,36,0,
478,477,476,475,409,408,407,321,318,227,193,191,187,186,62,0,167,168,169,
  170,173,181,182,183,184,188,192,
476,321,318,40,0,39,114,117,
483,482,481,480,479,478,477,476,475,474,473,468,467,441,439,413,409,408,407,
  366,341,340,339,338,337,336,335,321,318,309,298,227,224,223,193,191,187,
  186,131,84,62,60,58,56,54,40,36,0,37,38,45,
478,477,476,475,409,408,407,321,318,227,62,0,31,169,170,173,
468,467,441,439,413,0,27,428,429,462,463,464,465,466,
480,479,478,477,476,475,474,409,408,407,366,321,318,227,62,0,25,63,65,422,
  423,
480,479,478,477,476,475,474,409,408,407,366,321,318,227,62,0,23,63,65,364,
  365,
480,479,478,477,476,475,474,409,408,407,321,318,227,62,60,58,0,21,57,78,175,
  296,299,460,461,
480,479,478,477,476,475,474,409,408,407,321,318,227,131,62,0,19,63,65,122,
  123,124,128,
476,321,318,0,17,98,99,
483,482,481,480,479,478,477,476,475,474,409,408,407,321,318,298,227,224,223,
  84,62,60,58,56,54,36,0,15,66,68,100,
480,479,478,477,476,475,474,409,408,407,321,318,309,227,193,191,187,186,62,
  60,58,0,13,57,78,174,175,184,188,192,198,199,200,201,202,296,299,306,
  307,308,310,
476,341,340,339,338,337,336,335,321,318,0,11,330,331,332,334,342,
476,341,340,339,338,337,336,335,321,318,0,9,98,326,329,330,334,
480,479,478,477,476,475,474,409,408,407,321,318,227,193,191,187,186,62,60,
  58,0,7,57,78,174,175,184,188,192,198,199,200,201,202,295,296,299,
476,321,318,0,4,95,96,98,
479,478,477,476,409,408,407,321,318,227,62,0,222,
477,476,407,321,318,230,0,166,228,229,232,
36,0,50,
478,477,476,409,408,407,321,318,223,0,138,149,205,219,
485,484,481,479,478,477,476,475,474,409,408,407,321,318,271,227,62,56,54,52,
  47,42,0,
36,0,50,
49,0,48,
485,481,479,478,477,476,475,474,473,409,408,407,321,318,271,262,261,255,254,
  249,248,227,224,223,163,159,153,62,56,40,36,0,37,38,45,
52,0,51,
473,391,298,60,54,40,5,0,53,196,269,
473,391,298,60,54,40,5,0,53,196,269,
478,477,476,475,409,408,407,321,318,227,62,0,
36,0,50,
487,486,485,484,483,482,481,480,479,478,477,476,475,474,473,409,408,407,321,
  318,298,271,227,224,223,84,62,60,58,56,54,52,49,47,42,40,36,5,0,37,38,
  45,
40,0,39,113,
476,321,318,0,
40,0,39,
5,0,
473,47,40,0,37,38,45,
473,47,40,0,37,38,45,
473,47,40,0,37,38,45,
473,47,40,0,37,38,45,
473,47,40,0,37,38,45,
473,42,40,5,0,37,38,45,
5,0,
473,40,0,37,45,
473,40,0,37,45,
5,0,
473,40,0,37,45,
473,40,0,37,45,
5,0,
480,479,478,477,476,475,474,473,409,408,407,321,318,227,62,58,40,36,0,300,
  301,302,
487,486,485,484,483,482,481,480,479,478,477,476,475,474,473,409,408,407,321,
  318,298,271,227,224,223,84,62,60,58,56,54,52,49,47,42,40,36,5,0,79,80,
  81,82,
480,479,478,477,476,475,474,409,408,407,321,318,227,62,0,
473,58,40,36,0,37,38,45,
478,477,476,475,473,409,408,407,321,318,227,193,191,187,186,62,40,0,37,38,
  45,
58,0,57,
473,52,42,40,5,0,37,38,45,
42,0,43,
5,0,
473,56,40,0,37,38,45,
473,40,0,37,45,
42,0,43,
5,0,
476,321,318,0,
473,40,5,0,37,38,45,
5,0,
483,482,481,480,479,478,477,476,475,474,409,408,407,321,318,298,227,224,223,
  84,62,60,58,56,54,36,0,
49,0,48,
473,40,5,0,37,38,45,
5,0,
36,0,50,
218,206,190,0,189,194,197,203,217,
218,206,190,0,189,194,197,203,217,
218,206,0,194,197,203,217,
58,0,57,
52,0,51,311,
473,52,42,40,5,0,37,38,45,
42,0,43,
5,0,
476,321,318,0,
473,42,40,0,37,38,45,
473,40,0,37,45,
473,40,0,37,45,
473,40,5,0,37,38,45,
5,0,
476,321,318,0,
473,40,5,0,37,38,45,
5,0,
52,0,51,297,
473,52,40,5,0,37,38,45,
5,0,
476,321,318,0,
473,40,0,37,45,
473,40,5,0,37,38,45,
5,0,
479,478,477,476,409,408,407,321,318,227,62,0,
477,476,407,321,318,0,
473,230,227,224,52,42,40,36,5,0,233,234,235,
477,476,473,407,321,318,227,224,52,42,40,5,0,229,231,232,
230,0,
227,0,
224,0,
476,321,318,0,98,137,
485,481,479,478,477,476,475,474,409,408,407,321,318,271,227,62,49,47,42,0,
  208,212,214,215,216,
478,477,476,409,408,407,321,318,223,0,138,149,205,219,
485,481,479,478,477,476,475,474,409,408,407,321,318,271,262,261,255,254,249,
  248,227,224,223,163,159,153,62,56,36,0,237,238,239,240,241,242,243,244,
  245,246,247,263,
485,481,479,478,477,476,475,474,473,409,408,407,321,318,271,262,261,255,254,
  249,248,227,224,223,163,159,153,62,56,40,36,0,37,38,45,
485,481,479,478,477,476,475,474,409,408,407,321,318,227,224,223,62,56,54,36,
  0,270,272,275,
271,0,
485,484,481,479,478,477,476,475,474,473,409,408,407,321,318,271,227,224,223,
  62,56,54,52,49,47,42,40,36,0,37,38,45,
487,486,485,484,483,482,481,480,479,478,477,476,475,474,409,408,407,321,318,
  298,271,227,224,223,84,62,60,58,56,54,52,49,47,42,36,0,72,
116,40,0,39,112,
476,321,318,0,
486,485,484,483,482,481,480,479,478,477,476,475,474,409,408,407,321,318,298,
  271,227,224,223,84,62,60,58,56,54,52,49,47,42,40,36,5,0,115,119,120,121,
47,0,46,
47,0,46,
47,0,46,
47,0,46,
47,0,46,
42,0,41,
480,479,478,477,476,475,474,409,408,407,321,318,227,62,0,63,65,368,421,
480,479,478,477,476,475,474,457,449,444,441,439,436,434,413,409,408,407,401,
  394,392,381,321,318,227,62,0,65,370,372,373,376,378,398,424,425,426,427,
  428,429,430,431,432,433,
480,479,478,477,476,475,474,409,408,407,321,318,227,62,0,63,65,368,421,
480,479,478,477,476,475,474,420,418,413,409,408,407,401,396,394,392,383,381,
  321,318,227,62,0,65,367,369,370,371,372,373,374,375,376,377,378,379,398,
473,40,0,300,
487,486,485,484,483,482,481,480,479,478,477,476,475,474,473,409,408,407,321,
  318,298,271,227,224,223,84,62,60,58,56,54,52,49,47,42,40,36,5,0,
487,486,485,484,483,482,481,480,479,478,477,476,475,474,473,409,408,407,321,
  318,298,271,227,224,223,84,62,58,56,54,52,49,47,42,40,36,5,0,79,80,
60,0,
480,479,478,477,476,475,474,473,409,408,407,321,318,227,62,58,40,36,0,300,
  301,302,
478,477,476,475,409,408,407,321,318,227,193,191,187,186,62,0,168,169,170,
  173,181,182,183,184,188,192,
478,477,476,475,473,409,408,407,321,318,227,193,191,187,186,62,40,0,37,38,
  45,
52,0,51,109,
480,479,478,477,476,475,474,473,409,408,407,321,318,227,62,60,58,40,0,37,38,
  45,
56,0,55,
478,477,476,409,408,407,321,318,223,0,125,138,149,219,
480,479,478,477,476,475,474,473,409,408,407,321,318,227,131,62,40,0,37,38,
  45,
483,482,481,480,479,478,477,476,475,474,409,408,407,321,318,298,227,224,223,
  84,62,60,58,56,54,36,0,66,68,
485,484,481,479,478,477,476,475,474,409,408,407,321,318,271,227,224,223,62,
  56,49,47,36,0,178,180,
480,479,478,477,476,475,474,409,408,407,321,318,227,62,60,0,78,174,175,202,
  299,
473,54,52,42,40,5,0,37,38,45,
473,54,52,42,40,5,0,37,38,45,
480,479,478,477,476,475,474,409,408,407,321,318,227,62,60,0,78,174,175,202,
  299,
473,54,52,42,40,5,0,37,38,45,
473,54,52,42,40,5,0,37,38,45,
473,54,52,42,40,5,0,37,38,45,
473,54,52,42,40,5,0,37,38,45,
480,479,478,477,476,475,474,473,409,408,407,327,322,321,318,316,227,62,40,0,
  37,38,45,
52,0,51,
52,0,51,311,
480,479,478,477,476,475,474,473,409,408,407,321,318,227,193,191,187,186,62,
  60,58,40,0,37,38,45,
42,0,41,
356,355,354,353,352,351,350,349,348,347,346,345,0,343,
476,321,318,0,333,357,
480,479,478,477,476,475,474,473,409,408,407,321,318,304,227,62,40,0,37,38,
  45,
52,0,51,
52,0,51,297,
480,479,478,477,476,475,474,409,408,407,321,318,227,94,93,92,91,90,62,0,63,
  65,89,
477,476,407,321,318,0,
477,476,407,321,318,0,232,
36,0,233,
477,476,473,407,321,318,227,224,52,42,40,5,0,229,231,232,
476,321,318,0,225,226,
476,321,318,0,
485,481,479,478,477,476,475,474,409,408,407,321,318,271,227,62,47,42,0,214,
485,481,479,478,477,476,475,474,473,409,408,407,391,321,318,298,271,227,224,
  223,62,60,56,54,52,47,40,36,5,0,266,267,268,
47,0,46,264,
473,47,40,0,37,38,45,
473,47,40,0,37,38,45,
480,479,478,477,476,475,474,473,409,408,407,391,321,318,298,227,62,60,54,52,
  47,40,5,0,46,259,
473,47,40,0,37,38,45,
473,47,40,0,37,38,45,
473,47,40,0,37,38,45,
473,47,40,0,37,38,45,
473,47,40,0,37,38,45,
485,481,479,478,477,476,475,474,409,408,407,321,318,271,262,261,255,254,249,
  248,227,224,223,163,159,153,62,56,36,0,237,238,239,240,241,242,243,244,
  245,246,247,263,
485,481,479,478,477,476,475,474,409,408,407,321,318,227,224,223,62,56,54,36,
  0,
47,0,
485,481,479,478,477,476,475,474,409,408,407,321,318,227,224,223,62,56,54,36,
  0,270,272,275,
485,484,481,479,478,477,476,475,474,409,408,407,321,318,271,227,224,223,62,
  56,54,52,49,47,42,36,0,171,177,
487,486,485,484,483,482,481,480,479,478,477,476,475,474,473,409,408,407,321,
  318,298,271,227,224,223,84,62,60,58,56,54,52,49,47,42,40,36,5,0,75,76,
  77,
473,40,5,0,37,38,45,
486,485,484,483,482,481,480,479,478,477,476,475,474,409,408,407,321,318,298,
  271,227,224,223,84,62,60,58,56,54,52,49,47,42,40,36,0,119,
480,479,478,477,476,475,474,473,417,416,409,408,407,321,318,227,62,60,40,0,
  37,38,45,
473,60,40,0,37,38,45,
473,60,40,0,37,38,45,
473,60,40,0,37,38,45,
477,476,473,407,321,318,40,0,37,38,45,
473,468,467,441,439,413,40,0,37,38,45,
473,47,40,0,37,38,45,
42,0,41,
473,47,40,0,37,38,45,
412,411,0,399,402,406,
473,47,40,0,37,38,45,
473,47,40,0,37,38,45,
473,47,40,0,37,38,45,
473,47,40,0,37,38,45,
480,479,478,477,476,475,474,473,409,408,407,321,318,227,62,47,40,0,45,
473,47,40,0,37,38,45,
473,47,40,0,37,38,45,
473,47,40,0,37,38,45,
473,47,40,0,37,38,45,
473,47,40,0,37,38,45,
473,42,40,5,0,37,38,45,
42,0,41,
42,0,41,
473,47,40,0,37,38,45,
473,47,40,0,37,38,45,
473,47,40,0,37,38,45,
473,47,40,0,37,38,45,
473,47,40,0,37,38,45,
480,479,478,477,476,475,474,473,409,408,407,321,318,227,62,47,40,0,45,
473,42,40,5,0,37,38,45,
42,0,41,
473,298,40,0,37,38,45,
478,477,476,475,409,408,407,321,318,227,193,191,187,186,62,0,168,169,170,
  173,181,182,183,184,188,192,
480,479,478,477,476,475,474,473,409,408,407,321,318,227,62,40,0,37,38,45,
52,0,51,
480,479,478,477,476,475,474,409,408,407,321,318,227,62,60,58,0,57,78,175,
  296,299,461,
480,479,478,477,476,475,474,473,409,408,407,321,318,227,62,40,0,37,38,45,
473,52,42,40,36,5,0,37,38,45,
52,42,5,0,126,
480,479,478,477,476,475,474,409,408,407,321,318,227,131,62,0,63,65,123,124,
  128,
485,484,481,479,478,477,476,475,474,409,408,407,321,318,271,227,224,223,62,
  56,49,47,36,0,
473,52,42,40,5,0,37,38,45,
54,52,42,5,0,53,196,269,
54,52,42,5,0,53,196,269,
54,52,42,5,0,53,196,269,
54,52,42,5,0,53,196,269,
54,52,42,5,0,53,196,269,
54,52,42,5,0,53,196,269,
480,479,478,477,476,475,474,409,408,407,327,322,321,318,316,227,62,0,63,65,
  148,312,313,314,315,
480,479,478,477,476,475,474,473,409,408,407,327,322,321,318,316,227,62,40,0,
  37,38,45,
52,0,51,
480,479,478,477,476,475,474,409,408,407,321,318,227,193,191,187,186,62,60,
  58,0,57,78,174,175,184,188,192,198,199,200,201,202,296,299,310,
476,473,321,318,40,0,37,38,45,
473,40,0,37,45,
476,321,318,0,
36,0,50,
480,479,478,477,476,475,474,409,408,407,321,318,304,227,62,0,63,65,148,303,
  305,
480,479,478,477,476,475,474,473,409,408,407,321,318,304,227,62,40,0,37,38,
  45,
52,0,51,
477,476,407,321,318,0,
476,321,318,0,
227,0,
485,481,479,478,477,476,475,474,409,408,407,321,318,271,227,224,223,62,56,
  36,0,266,
485,481,479,478,477,476,475,474,473,409,408,407,321,318,271,227,224,223,62,
  56,40,36,0,37,38,45,
47,0,46,
47,0,46,
480,479,478,477,476,475,474,473,409,408,407,391,321,318,298,227,62,60,54,52,
  40,5,0,63,65,260,
258,257,256,0,
47,0,46,
47,0,46,
47,0,46,
47,0,46,
47,0,46,
485,481,479,478,477,476,475,474,473,409,408,407,391,321,318,298,271,227,224,
  223,62,60,56,54,52,49,42,40,36,5,0,273,277,278,279,
485,484,481,479,478,477,476,475,474,409,408,407,321,318,271,227,224,223,62,
  56,54,52,49,47,42,36,0,
487,486,485,484,483,482,481,480,479,478,477,476,475,474,473,409,408,407,321,
  318,298,271,227,224,223,84,62,60,58,56,54,52,49,47,42,40,36,0,75,
480,479,478,477,476,475,474,417,416,409,408,407,321,318,227,62,60,0,59,438,
60,0,469,
60,0,78,393,440,
60,0,78,393,
477,476,407,321,318,0,442,443,448,
468,467,441,439,413,0,428,429,462,463,464,465,466,
47,0,46,111,
480,479,478,477,476,475,474,473,409,408,407,321,318,227,62,40,0,37,38,45,
47,0,46,
480,479,478,477,476,475,474,410,409,408,407,321,318,227,62,0,63,65,405,
407,0,403,
473,60,42,40,5,0,400,
47,0,46,
47,0,46,
47,0,46,
47,0,46,
473,47,40,0,
480,479,478,477,476,475,474,473,409,408,407,321,318,227,62,60,40,0,300,301,
  302,
47,0,46,
47,0,46,
47,0,46,
47,0,46,
47,0,46,
480,479,478,477,476,475,474,473,457,449,444,441,439,436,434,413,409,408,407,
  401,394,392,381,321,318,227,62,40,0,37,38,45,
47,0,46,
47,0,46,
47,0,46,
47,0,46,
47,0,46,384,
480,479,478,477,476,475,474,473,409,408,407,321,318,227,62,47,40,0,65,380,
480,479,478,477,476,475,474,473,420,418,413,409,408,407,401,396,394,392,383,
  381,321,318,227,62,40,0,37,38,45,
298,0,
473,298,40,0,37,38,45,
480,479,478,477,476,475,474,409,408,407,321,318,227,62,0,63,65,110,
480,479,478,477,476,475,474,473,409,408,407,321,318,227,62,40,0,37,38,45,
480,479,478,477,476,475,474,409,408,407,321,318,227,62,0,63,65,129,
36,0,50,
52,0,51,
473,52,47,42,40,5,0,37,38,45,
473,47,40,0,37,38,45,
473,47,40,0,37,38,45,
473,47,40,0,37,38,45,
473,52,42,40,5,0,37,38,45,
473,52,42,40,5,0,37,38,45,
473,52,42,40,5,0,37,38,45,
480,479,478,477,476,475,474,409,408,407,327,322,321,318,316,227,62,0,63,65,
  148,312,313,314,315,
476,321,318,0,332,342,
476,321,318,0,344,
476,321,318,0,358,
473,47,40,0,37,38,45,
473,52,40,5,0,37,38,45,
480,479,478,477,476,475,474,409,408,407,321,318,304,227,62,0,63,65,148,303,
  305,
476,321,318,0,
476,321,318,0,226,
485,481,479,478,477,476,475,474,409,408,407,321,318,271,227,224,223,62,56,
  36,0,263,
478,477,476,473,409,408,407,321,318,223,40,0,37,38,45,
480,479,478,477,476,475,474,473,409,408,407,321,318,227,165,62,40,0,37,38,
  45,
480,479,478,477,476,475,474,473,409,408,407,321,318,227,94,93,92,91,90,62,
  40,0,37,38,45,
478,477,476,473,409,408,407,321,318,223,40,0,37,38,45,
476,473,321,318,40,0,37,38,45,
480,479,478,477,476,475,474,473,409,408,407,321,318,253,252,227,62,40,0,37,
  38,45,
480,479,478,477,476,475,474,473,409,408,407,321,318,227,136,135,134,133,62,
  40,0,37,38,45,
485,481,479,478,477,476,475,474,409,408,407,321,318,227,224,223,62,56,54,49,
  36,0,277,
480,479,478,477,476,475,474,417,416,409,408,407,321,318,227,62,0,63,65,415,
477,476,407,321,318,60,0,443,448,470,471,472,
477,476,407,321,318,0,443,448,
480,479,478,477,476,475,474,473,409,408,407,321,318,227,62,60,40,0,37,38,45,
480,479,478,477,476,475,474,409,408,407,321,318,227,62,0,63,65,421,
473,60,40,0,37,45,59,
408,0,
476,321,318,0,98,404,
473,60,40,0,37,38,45,
480,479,478,477,476,475,474,473,417,416,409,408,407,321,318,227,62,60,40,0,
  37,38,45,
473,459,458,40,0,37,38,45,
473,60,40,0,37,38,45,
480,479,478,477,476,475,474,473,409,408,407,321,318,227,62,60,40,0,300,301,
  302,
480,479,478,477,476,475,474,409,408,407,321,318,227,62,60,0,65,
473,60,40,0,37,38,45,
473,60,40,0,37,38,45,
473,60,40,0,37,38,45,
473,60,40,0,37,38,45,
473,60,40,0,37,38,45,
480,479,478,477,476,475,474,457,449,444,441,439,436,434,413,409,408,407,401,
  394,392,381,321,318,227,62,0,65,370,372,373,376,378,398,425,426,427,428,
  429,430,431,432,433,
473,60,40,0,37,38,45,
473,397,257,40,0,37,38,45,
60,0,419,
60,0,419,
473,60,40,0,37,38,45,385,
480,479,478,477,476,475,474,473,409,408,407,321,318,227,62,47,40,0,300,301,
  302,
480,479,478,477,476,475,474,420,418,413,409,408,407,401,396,394,392,383,381,
  321,318,227,62,0,65,369,370,371,372,373,374,375,376,377,378,379,398,
298,0,
473,52,47,42,40,5,0,37,38,45,
480,479,478,477,476,475,474,409,408,407,321,318,227,62,0,63,65,110,
473,56,40,0,37,38,45,
476,473,321,318,40,0,37,38,45,
480,479,478,477,476,475,474,473,409,408,407,321,318,227,163,162,160,159,157,
  153,151,150,62,40,0,37,38,45,
47,0,46,
47,0,46,
47,0,46,
47,0,46,
473,40,0,37,45,
476,321,318,0,
476,321,318,0,
36,0,50,
47,0,46,
227,0,
478,477,476,409,408,407,321,318,223,0,138,149,219,
480,479,478,477,476,475,474,409,408,407,321,318,227,165,62,0,63,65,164,
480,479,478,477,476,475,474,409,408,407,321,318,227,94,93,92,91,90,62,0,63,
  65,89,
478,477,476,409,408,407,321,318,223,0,138,149,219,
476,321,318,0,154,
480,479,478,477,476,475,474,409,408,407,321,318,253,252,227,62,0,63,65,250,
  251,
480,479,478,477,476,475,474,409,408,407,321,318,227,136,135,134,133,62,0,63,
  65,132,
473,60,42,40,5,0,400,
477,476,407,321,318,0,443,448,470,
60,0,
477,476,407,321,318,0,443,448,
480,479,478,477,476,475,474,409,408,407,321,318,227,62,60,0,63,65,78,
60,0,59,
407,0,
476,321,318,0,
62,0,61,
60,0,59,437,
480,479,478,477,476,475,474,417,416,409,408,407,321,318,227,62,60,0,59,438,
459,458,0,
60,0,450,
480,479,478,477,476,475,474,409,408,407,321,318,227,62,60,0,65,
487,486,485,484,483,482,481,480,479,478,477,476,475,474,473,409,408,407,321,
  318,298,271,227,224,223,84,62,60,58,56,54,52,49,47,42,40,36,5,0,79,80,
  81,82,
480,479,478,477,476,475,474,409,408,407,321,318,227,62,0,
60,0,78,395,
60,0,445,
60,0,78,393,
60,0,78,382,
60,0,78,435,
473,42,40,5,0,37,38,45,
60,0,59,
397,257,0,
477,476,407,321,318,0,86,88,
60,0,
47,0,
473,42,40,5,0,37,38,45,
47,0,46,111,
56,0,55,
476,321,318,0,98,137,
480,479,478,477,476,475,474,409,408,407,321,318,227,163,162,160,159,157,153,
  151,150,62,0,63,65,127,139,140,141,142,143,144,145,146,147,148,152,155,
  158,
480,479,478,477,476,475,474,473,409,408,407,321,318,227,223,62,60,40,0,37,
  38,45,
476,473,321,318,60,40,0,37,38,45,
473,325,324,40,0,37,38,45,
473,321,318,40,0,37,38,45,
476,321,318,0,333,357,
476,321,318,0,
476,321,318,0,359,
480,479,478,477,476,475,474,473,409,408,407,321,318,227,62,40,0,37,38,45,
476,321,318,0,226,
476,321,318,0,
477,476,407,321,318,0,443,448,
473,52,42,40,5,0,37,38,45,
473,52,42,40,5,0,37,38,45,
409,0,
480,479,478,477,476,475,474,410,409,408,407,321,318,227,62,0,63,65,405,
478,477,476,475,409,408,407,321,318,227,193,191,187,186,62,0,167,168,169,
  170,173,181,182,183,184,188,192,
480,479,478,477,476,475,474,417,416,409,408,407,321,318,227,62,0,63,65,415,
478,477,476,475,409,408,407,321,318,56,0,452,
487,486,485,484,483,482,481,480,479,478,477,476,475,474,473,409,408,407,321,
  318,298,271,227,224,223,84,62,60,58,56,54,52,49,47,42,40,36,5,0,79,80,
  81,82,
480,479,478,477,476,475,474,409,408,407,321,318,227,62,0,
60,0,
477,476,473,407,321,318,60,40,0,37,38,45,443,446,448,
480,479,478,477,476,475,474,417,416,409,408,407,321,318,227,62,0,63,65,414,
  415,
477,476,407,321,318,0,
60,0,
480,479,478,477,476,475,474,473,409,408,407,321,318,227,187,186,62,60,40,0,
  45,386,
480,479,478,477,476,475,474,473,409,408,407,321,318,227,62,60,40,0,300,301,
  302,
480,479,478,477,476,475,474,473,409,408,407,321,318,227,136,135,134,133,62,
  40,0,37,38,45,
473,52,42,40,5,0,37,38,45,
473,52,47,42,40,5,0,37,38,45,
473,47,40,0,37,38,45,
473,47,40,0,37,38,45,
473,47,40,0,37,38,45,
52,47,42,5,0,46,156,
473,47,40,0,37,38,45,
473,47,40,0,37,38,45,
473,52,42,40,5,0,37,38,45,
473,52,42,40,5,0,37,38,45,
480,479,478,477,476,475,474,409,408,407,321,318,227,223,62,60,0,63,65,78,
  149,
476,321,318,60,0,59,98,328,329,
325,324,0,323,
321,318,0,317,
476,321,318,0,
476,321,318,0,
473,40,0,37,45,
480,479,478,477,476,475,474,409,408,407,321,318,227,62,0,63,65,
476,321,318,0,
477,476,407,321,318,0,443,448,
60,0,
473,60,42,40,5,0,400,
478,477,476,475,456,455,409,408,407,321,318,60,56,0,453,454,
60,0,
60,0,
477,476,407,321,318,0,443,448,
60,0,
473,60,42,40,0,37,38,45,
473,40,0,
480,479,478,477,476,475,474,409,408,407,321,318,227,187,186,62,60,0,63,65,
  181,184,387,388,389,
480,479,478,477,476,475,474,409,408,407,321,318,227,62,60,0,65,
480,479,478,477,476,475,474,409,408,407,321,318,227,136,135,134,133,62,0,63,
  65,130,132,
47,0,46,
47,0,46,
47,0,46,
476,473,321,318,40,0,37,38,45,
47,0,46,
47,0,46,
473,52,42,40,5,0,37,38,45,
473,52,42,40,5,0,37,38,45,
473,52,42,40,5,0,37,38,45,
473,341,340,339,338,337,336,335,40,0,37,38,45,
473,227,52,42,40,5,0,320,
473,227,52,42,40,5,0,320,
363,362,361,0,360,
477,476,407,321,318,0,443,448,
60,0,
477,476,407,321,318,0,443,448,
42,0,41,
391,0,390,
473,60,40,0,45,386,
487,486,485,484,483,482,481,480,479,478,477,476,475,474,473,409,408,407,321,
  318,298,271,227,224,223,84,62,60,58,56,54,52,49,47,42,40,36,5,0,79,80,
  81,82,
480,479,478,477,476,475,474,409,408,407,321,318,227,62,0,
478,477,476,473,409,408,407,321,318,223,40,0,37,38,45,
476,473,321,318,40,0,37,38,45,
480,479,478,477,476,475,474,473,409,408,407,321,318,227,165,62,40,0,37,38,
  45,
476,321,318,0,98,137,
480,479,478,477,476,475,474,473,409,408,407,321,318,227,62,40,0,37,38,45,
477,476,473,407,321,318,230,40,0,37,38,45,
341,340,339,338,337,336,335,0,330,334,
476,473,321,318,52,42,40,5,0,319,
477,476,407,321,318,0,443,448,
477,476,407,321,318,0,443,448,
480,479,478,477,476,475,474,473,417,416,409,408,407,321,318,227,62,40,0,37,
  38,45,
187,186,0,181,184,
60,0,
60,0,
478,477,476,409,408,407,321,318,223,0,138,149,219,
476,321,318,0,154,
480,479,478,477,476,475,474,409,408,407,321,318,227,165,62,0,63,65,164,
480,479,478,477,476,475,474,409,408,407,321,318,227,62,0,63,65,
477,476,407,321,318,230,0,161,166,228,229,232,
473,60,40,0,37,38,45,
476,321,318,0,
477,476,407,321,318,0,443,448,
477,476,407,321,318,0,443,448,
480,479,478,477,476,475,474,417,416,409,408,407,321,318,227,62,0,63,65,415,
473,52,42,40,5,0,37,38,45,
473,52,42,40,5,0,37,38,45,
473,52,42,40,5,0,37,38,45,
473,52,42,40,5,0,37,38,45,
473,52,42,40,5,0,37,38,45,
60,0,
476,321,318,0,
477,476,407,321,318,0,443,448,
477,476,407,321,318,0,443,448,
477,476,407,321,318,0,443,448,
477,476,407,321,318,0,443,448,
477,476,407,321,318,0,443,448,
477,476,407,321,318,0,443,448,
477,476,407,321,318,0,443,448,
477,476,407,321,318,0,443,448,
477,476,407,321,318,0,443,448,
477,476,407,321,318,0,443,448,
477,476,407,321,318,0,443,448,
477,476,407,321,318,0,443,448,
477,476,407,321,318,0,443,448,
477,476,407,321,318,0,443,448,
477,476,407,321,318,0,443,448,
477,476,407,321,318,0,443,448,
477,476,407,321,318,0,443,448,
477,476,407,321,318,0,443,448,
477,476,407,321,318,0,443,448,
477,476,407,321,318,0,443,448,
477,476,407,321,318,0,443,448,
477,476,407,321,318,0,443,448,
477,476,407,321,318,0,443,448,
477,476,407,321,318,0,443,448,
477,476,407,321,318,0,443,448,
477,476,407,321,318,0,443,448,
477,476,407,321,318,0,443,448,
};
static unsigned const char ag_astt[6250] = {
  1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,1,2,2,2,2,2,2,2,2,2,2,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,7,0,1,1,1,1,1,1,2,1,2,1,2,2,2,1,1,1,2,1,9,9,9,9,
  9,9,9,9,9,9,9,9,9,9,4,2,2,7,3,1,1,1,1,2,2,2,7,2,1,1,1,1,1,2,2,2,7,2,1,1,1,
  1,1,1,1,8,7,1,1,1,1,2,7,1,1,3,7,3,7,3,7,3,7,1,7,1,3,7,3,7,1,7,1,1,7,1,1,7,
  1,1,7,1,1,7,1,1,7,1,1,7,1,1,7,1,1,7,1,1,7,1,1,7,1,1,7,1,1,1,1,1,1,1,1,1,1,
  7,1,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,7,1,1,1,2,1,4,4,4,
  4,4,2,4,4,7,1,1,1,4,4,4,4,4,2,4,4,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,2,1,1,1,
  1,1,1,1,1,1,1,1,1,1,7,2,1,1,9,9,4,2,7,1,4,4,4,4,5,4,4,4,4,4,4,4,4,4,4,4,5,
  5,7,1,1,1,1,1,1,1,1,1,1,2,2,2,2,1,7,1,1,2,1,1,2,2,2,1,1,1,1,1,1,2,7,3,1,1,
  5,5,5,5,5,5,5,5,5,5,1,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
  5,5,5,5,5,5,5,5,1,5,7,2,2,1,1,1,1,1,1,1,1,1,1,1,1,7,1,2,1,1,1,1,1,1,1,7,1,
  2,2,2,1,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,1,1,1,2,2,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,7,1,1,1,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,7,1,1,1,1,1,1,1,2,
  1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,7,1,2,1,1,2,1,1,1,1,1,7,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,
  1,2,2,2,2,1,1,2,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,2,1,2,2,2,2,2,2,2,1,
  1,7,1,2,1,1,1,1,1,2,2,2,2,2,2,2,1,1,7,1,1,1,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,2,2,2,2,1,1,2,7,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,7,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,4,1,1,1,1,1,1,1,7,1,1,1,1,2,4,1,1,1,1,1,1,1,1,1,1,7,1,2,2,2,
  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,4,2,4,1,2,7,1,8,8,8,8,8,8,8,8,
  1,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,1,8,7,1,1,1,2,4,1,4,4,4,4,2,4,4,
  7,1,2,1,4,4,4,4,2,4,4,7,1,2,1,9,9,9,9,9,9,9,9,9,9,9,4,2,7,1,8,8,8,8,8,8,8,
  8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,1,8,5,7,1,1,1,2,7,
  1,1,1,1,1,7,2,4,1,3,7,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,
  1,1,1,8,1,7,1,1,1,1,8,1,5,7,1,1,1,3,7,1,1,7,1,1,1,1,7,1,1,3,7,1,1,7,1,1,1,
  1,7,1,1,3,7,5,5,5,5,5,5,5,1,5,5,5,5,5,5,5,5,1,5,7,1,1,2,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,8,1,1,1,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,4,1,5,1,5,7,2,2,1,8,8,8,8,1,8,8,8,8,8,8,8,8,8,8,8,1,
  7,1,1,1,2,7,1,1,8,5,1,5,7,1,1,1,2,5,1,3,7,1,8,1,7,1,1,1,1,1,7,1,1,2,5,1,3,
  7,9,9,9,4,1,1,5,7,2,2,1,3,7,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
  9,9,9,4,2,4,1,1,1,5,7,2,2,1,3,7,2,7,1,2,2,2,7,1,1,1,1,1,2,2,2,7,1,1,1,1,1,
  2,2,7,1,1,1,1,2,4,1,2,4,1,1,1,8,5,1,5,7,1,1,1,2,5,1,3,7,2,2,2,4,1,8,1,7,1,
  1,1,1,1,7,1,1,1,1,7,1,1,1,1,5,7,2,2,1,3,7,9,9,9,4,1,1,5,7,2,2,1,3,7,2,4,1,
  1,1,8,1,5,7,1,1,1,3,7,9,9,9,4,1,1,7,1,1,1,1,5,7,2,2,1,3,7,9,9,9,9,9,9,9,9,
  9,9,9,4,1,1,1,1,1,5,5,5,5,5,5,5,5,1,5,7,1,1,3,1,1,5,1,1,1,5,5,5,5,5,5,7,3,
  3,1,1,5,1,5,2,7,1,1,1,7,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,1,1,7,2,2,1,
  1,2,1,1,1,1,1,1,1,1,1,7,1,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,7,2,2,2,2,2,2,2,2,2,2,2,1,8,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,
  8,8,8,8,8,8,8,8,8,8,8,8,8,1,8,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,7,2,1,1,1,4,8,8,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,1,8,7,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,4,2,2,2,7,3,1,2,2,2,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,5,7,2,1,1,2,2,7,1,2,7,1,2,7,1,2,7,1,2,7,1,2,7,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,7,1,2,2,2,2,2,1,1,1,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,1,1,1,2,2,2,2,2,
  2,2,2,2,2,1,9,9,5,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
  3,3,3,3,3,3,3,3,3,3,3,7,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
  1,9,9,9,9,9,9,9,9,9,9,9,5,3,3,2,7,5,5,5,5,5,5,5,1,5,5,5,5,5,5,5,5,1,5,7,1,
  1,2,1,1,1,1,1,1,1,1,1,1,2,2,2,2,1,7,1,2,1,1,2,2,2,1,1,1,8,8,8,8,1,8,8,8,8,
  8,8,8,8,8,8,8,1,7,1,1,1,2,4,1,1,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,8,1,7,1,1,
  1,2,7,1,1,1,1,1,1,1,1,1,1,7,1,1,2,2,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,1,7,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,2,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,1,
  1,2,3,1,1,8,5,5,1,5,7,1,1,1,1,8,5,5,1,5,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,7,1,1,2,3,1,1,8,5,5,1,5,7,1,1,1,1,8,5,5,1,5,7,1,1,1,1,8,5,5,1,5,7,1,1,
  1,1,8,5,5,1,5,7,1,1,1,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,8,8,1,7,1,1,1,2,4,1,
  2,4,1,1,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,8,8,8,8,8,1,7,1,1,1,2,7,1,2,2,2,2,
  2,2,2,2,2,2,2,2,7,1,1,1,1,7,2,1,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,1,7,1,1,1,
  2,4,1,2,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,1,7,2,1,2,1,1,1,1,1,5,1,
  1,1,1,1,7,3,1,5,3,1,1,5,1,1,1,5,5,5,5,5,5,7,3,3,1,1,1,1,7,3,1,9,9,9,4,9,9,
  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,5,3,1,1,1,1,1,1,1,1,5,1,1,1,5,1,1,5,1,1,1,
  1,1,5,1,5,5,5,5,1,5,7,1,1,2,2,4,1,2,1,8,1,7,1,1,1,1,8,1,7,1,1,1,8,8,8,8,8,
  8,8,5,8,8,8,5,8,8,5,8,8,5,5,5,2,5,5,7,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,8,
  1,7,1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,7,2,2,2,2,2,2,2,2,2,2,2,1,9,9,9,9,9,9,9,9,9,9,9,9,9,
  9,9,9,9,9,9,9,4,1,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,2,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,2,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,7,1,1,2,1,1,5,7,2,2,
  1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,5,
  3,5,5,5,5,5,5,5,1,5,5,5,5,5,5,5,5,5,8,1,7,1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,
  1,1,8,1,7,1,1,1,8,8,1,8,8,8,1,7,1,1,1,1,8,8,8,8,8,1,7,1,1,1,1,8,1,7,1,1,1,
  2,4,1,1,8,1,7,1,1,1,2,2,7,1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,
  1,8,1,7,1,1,1,9,9,9,9,9,9,9,1,9,9,9,9,9,9,9,1,1,7,1,1,8,1,7,1,1,1,1,8,1,7,
  1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,5,1,5,7,2,2,1,2,4,1,2,4,
  1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,9,
  9,9,9,9,9,9,1,9,9,9,9,9,9,9,1,1,7,1,1,5,1,5,7,2,2,1,2,4,1,1,8,1,7,1,1,1,1,
  1,1,1,1,1,1,1,1,1,2,2,2,2,1,7,1,2,1,1,2,2,2,1,1,1,8,8,8,8,8,8,8,1,8,8,8,8,
  8,8,8,1,7,1,1,1,2,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,7,1,1,1,1,1,2,8,8,8,
  8,8,8,8,1,8,8,8,8,8,8,8,1,7,1,1,1,1,5,5,1,8,5,7,1,1,1,4,4,4,7,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,2,1,7,2,1,2,1,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
  9,9,4,1,5,5,1,5,7,2,2,1,2,4,4,4,7,1,2,1,2,4,4,4,7,1,2,1,2,4,4,4,7,1,2,1,2,
  4,4,4,7,1,2,1,2,4,4,4,7,1,2,1,2,4,4,4,7,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,7,1,1,2,3,1,1,1,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,8,8,1,7,1,1,1,2,4,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,1,1,2,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,8,
  1,8,8,1,7,1,1,1,1,1,7,1,1,2,2,2,7,2,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,1,
  1,2,1,2,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,1,7,1,1,1,2,4,1,3,3,3,3,3,5,1,1,1,
  5,1,7,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,5,3,8,8,8,8,8,8,8,8,1,8,8,8,
  8,8,8,8,8,8,8,8,1,8,7,1,1,1,2,7,1,2,7,1,1,1,1,1,1,1,1,5,1,1,1,5,1,1,5,1,1,
  5,5,5,5,5,7,2,1,2,2,2,2,5,2,7,1,2,7,1,2,7,1,2,7,1,2,7,1,1,1,1,1,1,1,1,1,5,
  1,1,1,5,1,1,5,5,1,1,1,1,5,1,1,5,1,5,5,1,5,7,2,1,1,2,9,9,9,9,9,9,9,9,9,9,9,
  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,4,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,5,3,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,2,7,1,
  1,1,7,2,1,7,2,2,2,1,7,2,2,1,1,1,1,1,7,2,1,1,1,1,1,1,1,7,2,2,3,1,2,2,2,2,7,
  1,2,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,1,7,1,1,1,2,7,1,1,1,1,1,1,1,1,2,1,1,1,1,
  1,1,1,7,2,1,2,1,7,1,5,2,5,5,5,7,2,2,7,1,2,7,1,2,7,1,2,7,1,9,1,9,7,8,8,8,8,
  8,8,8,1,8,8,8,8,8,8,8,8,1,7,1,1,1,2,7,1,2,7,1,2,7,1,2,7,1,2,7,1,8,8,8,8,8,
  8,8,1,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,1,7,1,1,1,2,7,1,2,7,1,2,7,1,2,
  7,1,2,7,2,1,1,1,1,1,1,1,1,9,1,1,1,1,1,1,1,1,9,7,1,2,8,8,8,8,8,8,8,1,8,8,8,
  8,8,8,8,8,8,8,8,8,8,8,8,8,1,7,1,1,1,2,7,1,8,1,7,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,7,1,1,3,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,1,7,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,7,2,1,1,2,4,1,2,4,1,1,5,8,5,1,5,7,1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,
  1,1,1,8,1,7,1,1,1,1,5,5,1,5,7,2,2,1,1,5,5,1,5,7,2,2,1,1,5,5,1,5,7,2,2,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,1,1,2,3,1,1,1,1,1,1,7,1,1,1,1,1,7,2,1,1,
  1,7,1,1,8,1,7,1,1,1,1,5,1,5,7,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,1,1,2,
  1,2,3,3,3,5,1,1,1,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,2,8,8,8,1,
  8,8,8,8,8,8,1,7,1,1,1,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,1,7,1,1,1,8,8,8,8,8,
  8,8,1,8,8,8,8,8,8,8,8,8,8,8,8,1,7,1,1,1,8,8,8,1,8,8,8,8,8,8,1,7,1,1,1,8,1,
  8,8,1,7,1,1,1,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,8,1,7,1,1,1,8,8,8,8,8,8,8,1,
  8,8,8,8,8,8,8,8,8,8,8,1,7,1,1,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
  5,3,1,1,1,1,1,1,1,2,2,1,1,1,1,1,1,1,7,2,1,1,1,1,1,1,1,8,7,1,1,1,1,1,1,1,1,
  1,1,4,1,1,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,1,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,7,1,1,3,1,2,1,4,1,1,2,1,7,1,1,1,7,1,1,1,8,1,7,1,1,1,5,5,5,5,5,5,5,1,
  5,5,5,5,5,5,5,5,5,8,1,7,1,1,1,1,8,8,1,7,1,1,1,1,8,1,7,1,1,1,8,8,8,8,8,8,8,
  1,8,8,8,8,8,8,8,8,1,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,1,1,8,1,7,1,1,
  1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,1,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,1,8,
  1,7,1,1,1,1,8,8,1,7,1,1,1,1,7,2,1,7,2,1,5,1,7,2,2,1,1,9,9,9,9,9,9,9,1,9,9,
  9,9,9,9,9,8,1,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,1,1,
  2,2,2,2,2,2,2,2,2,2,1,2,7,1,5,8,5,1,5,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  7,1,1,3,1,8,1,7,1,1,1,8,1,8,8,1,7,1,1,1,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,8,
  8,8,8,8,8,8,1,7,1,1,1,2,4,1,2,7,1,2,7,1,2,7,1,1,1,7,1,1,1,1,1,7,2,2,2,7,2,
  7,1,2,7,1,1,7,1,1,1,1,1,1,1,1,1,7,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,7,2,
  1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,1,7,2,1,2,1,1,1,1,1,1,1,1,1,7,2,2,
  2,1,1,1,7,2,1,1,1,1,1,1,1,1,1,1,1,1,2,2,1,1,7,2,1,2,2,1,1,1,1,1,1,1,1,1,1,
  1,1,1,2,2,2,2,1,7,2,1,2,5,2,5,5,5,7,2,9,9,9,9,9,5,3,3,3,2,7,1,1,1,1,1,4,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,1,1,1,2,4,2,1,7,9,9,9,4,2,7,1,2,7,1,3,8,
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,2,7,1,1,2,2,7,1,7,2,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,8,1,1,1,1,
  1,1,1,1,1,1,7,1,1,1,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,4,1,7,2,2,1,7,2,1,7,2,2,
  1,7,2,2,1,7,2,2,1,5,1,5,7,2,2,1,2,7,1,2,2,7,1,1,1,1,1,7,1,1,1,7,1,7,1,5,1,
  5,7,2,2,1,2,4,1,2,2,7,1,1,1,1,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,7,1,1,2,2,2,2,2,2,2,2,2,2,2,2,1,2,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,8,
  1,7,1,1,1,8,1,8,8,8,1,7,1,1,1,1,8,8,1,7,1,1,1,1,8,8,1,7,1,1,1,1,1,1,7,2,1,
  1,1,1,4,1,1,1,7,1,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,1,7,1,1,1,1,1,1,7,3,1,1,1,
  4,1,1,1,1,1,4,1,1,1,5,5,1,5,7,2,2,1,1,5,5,1,5,7,2,2,1,2,7,1,1,1,1,1,1,1,2,
  1,1,1,1,1,1,1,7,2,1,2,1,1,1,1,1,1,1,1,1,1,2,2,2,2,1,7,1,1,2,1,1,2,2,2,1,1,
  1,1,1,1,1,1,1,1,2,2,1,1,1,1,1,1,1,7,2,1,1,1,1,1,1,1,1,1,1,1,1,7,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,8,1,1,1,1,1,1,1,1,1,1,7,1,1,
  1,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,4,2,7,1,1,1,1,1,1,8,1,7,1,1,1,1,1,1,1,1,1,
  1,1,1,1,2,2,1,1,1,1,1,1,1,7,2,1,1,2,9,9,9,9,9,4,2,7,8,8,8,8,8,8,8,1,8,8,8,
  8,8,8,8,8,8,8,1,7,1,1,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,1,7,1,1,1,8,8,8,8,8,
  8,8,1,8,8,8,8,8,8,8,8,8,8,8,1,7,1,1,1,1,5,5,1,5,7,2,2,1,1,5,5,5,1,5,7,3,3,
  1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,4,2,4,4,7,1,2,1,8,1,7,1,1,1,1,
  8,1,7,1,1,1,1,5,5,1,5,7,3,3,1,1,5,5,1,5,7,3,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,7,1,1,1,1,1,1,1,2,7,1,1,3,2,2,2,7,2,1,1,7,2,2,2,2,4,2,2,2,7,1,1,7,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,2,1,2,2,2,4,1,1,1,1,1,4,1,1,3,7,5,2,5,5,5,
  7,2,9,9,9,9,1,1,9,9,9,9,9,8,9,7,1,1,2,7,2,7,1,1,1,1,1,7,1,1,2,7,1,2,8,1,7,
  1,1,1,9,9,5,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,1,2,7,2,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,1,7,2,1,2,2,2,7,1,
  2,7,1,2,7,1,8,1,8,8,1,7,1,1,1,2,7,1,2,7,1,1,5,5,1,5,7,2,2,1,1,5,5,1,5,7,2,
  2,1,1,5,5,1,5,7,2,2,1,1,8,8,8,8,8,8,8,1,7,1,1,1,5,1,5,5,5,5,7,2,5,1,5,5,5,
  5,7,2,2,2,2,7,2,1,1,1,1,1,4,1,1,2,7,1,1,1,1,1,7,1,1,2,7,1,1,5,1,1,8,1,7,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,8,1,1,1,1,1,1,1,1,
  1,1,7,1,1,1,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,5,8,8,8,1,8,8,8,8,8,8,1,7,1,1,1,
  8,1,8,8,1,7,1,1,1,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,1,7,1,1,1,1,1,1,7,1,2,8,
  8,8,8,8,8,8,1,8,8,8,8,8,8,8,1,7,1,1,1,8,8,1,8,8,8,8,1,7,1,1,1,2,2,2,2,2,2,
  2,7,1,1,1,5,1,1,5,5,5,5,7,3,1,1,1,1,1,4,1,1,1,1,1,1,1,7,1,1,8,8,8,8,8,8,8,
  1,8,8,8,8,8,8,8,8,8,1,7,1,1,1,2,2,7,3,1,2,7,3,7,1,1,1,1,1,1,1,1,1,7,1,2,2,
  1,1,1,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,7,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,7,1,1,1,1,1,1,1,1,7,1,2,1,1,1,1,8,1,7,1,1,1,1,1,1,5,2,2,2,2,2,4,2,2,1,1,
  1,1,1,7,1,1,1,1,1,1,1,1,1,2,2,1,1,1,1,1,1,1,7,2,1,2,1,5,5,1,5,7,2,2,1,1,5,
  5,1,5,7,2,2,1,1,5,5,1,5,7,2,2,1,1,5,5,1,5,7,2,2,1,1,5,5,1,5,7,2,2,1,2,7,3,
  3,3,5,1,1,1,1,1,7,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,7,1,1,1,1,
  1,1,1,7,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,7,1,
  1,1,1,1,1,1,7,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,7,1,1,1,1,1,1,
  1,7,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,7,1,1,1,
  1,1,1,1,7,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,7,
  1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,7,1,1,3,3,3,3,3,7,3,3
};
static const unsigned short ag_pstt[] = {
1,1,1,1,1,1,1,1,1,1,1,1,322,321,320,319,318,317,316,315,314,313,312,311,310,
  1,187,184,180,179,101,68,67,66,65,64,1,11,14,15,16,17,18,19,20,21,22,23,
  24,25,0,0,13,12,10,9,8,7,69,1,323,6,175,176,177,2,4,3,308,5,
39,39,39,39,39,39,39,39,39,39,39,39,39,39,40,
220,205,2,178,29,28,27,26,
220,205,183,3,186,30,29,28,27,26,
220,205,183,4,182,31,29,28,27,26,
32,34,33,5,33,33,35,32,
23,6,36,36,
19,7,
18,8,
17,9,
16,10,
37,11,38,
14,12,
13,13,
37,14,39,
37,15,40,
37,16,41,
37,17,42,
37,18,43,
37,19,44,
37,20,45,
37,21,46,
37,22,47,
37,23,48,
37,24,49,
37,25,50,
51,51,51,51,51,51,51,51,52,26,53,224,219,223,
55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,30,55,55,27,54,
  57,56,208,55,
251,251,251,251,251,32,251,251,28,58,60,59,
251,251,251,251,251,32,251,251,29,58,61,59,
62,62,62,62,62,62,62,62,62,62,62,30,185,63,62,
62,62,62,62,62,62,62,62,62,62,62,31,181,63,62,
27,27,28,
31,33,64,
23,23,23,23,26,23,23,23,23,23,23,23,23,23,23,23,26,26,34,
62,62,62,62,62,62,62,62,62,62,187,184,180,179,62,35,65,65,327,63,62,324,325,
  326,2,4,3,
66,66,66,23,36,98,67,67,
20,20,20,20,20,20,20,20,20,20,32,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
  20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,32,20,37,22,
  22,32,
62,62,62,62,62,62,62,62,62,62,62,38,68,163,63,62,
70,72,73,71,69,39,75,555,553,550,74,554,556,557,
1,1,1,1,1,1,1,1,1,1,77,1,1,1,1,40,78,76,1,492,492,
1,1,1,1,1,1,1,1,1,1,80,1,1,1,1,41,81,79,1,422,422,
82,82,82,82,82,82,82,82,82,82,82,82,82,82,83,35,42,90,86,85,87,88,84,89,546,
1,1,1,1,1,1,1,1,1,1,1,1,1,118,1,43,94,117,1,93,111,92,91,
95,95,95,44,97,95,96,
98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,
  98,45,101,99,98,100,
82,82,82,82,82,82,82,82,82,82,82,82,350,82,187,184,180,179,82,83,35,46,110,
  86,85,102,106,105,104,103,107,107,107,107,107,108,84,110,350,109,353,
111,394,393,392,391,390,389,388,111,111,47,116,386,115,114,112,113,
117,394,393,392,391,390,389,388,117,117,48,119,117,118,381,382,112,
82,82,82,82,82,82,82,82,82,82,82,82,82,187,184,180,179,82,83,35,49,122,86,
  85,102,106,105,104,103,120,120,120,120,120,328,121,84,
123,123,123,50,126,125,124,123,
127,127,127,127,127,127,127,127,127,127,127,225,127,
128,128,128,128,128,130,52,133,132,131,129,
31,221,134,
51,51,51,51,51,51,51,51,52,54,53,224,204,223,
210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,
  210,210,210,211,
31,206,135,
30,57,136,
137,137,137,137,137,137,137,137,32,137,137,137,137,137,137,137,137,137,137,
  137,137,137,137,137,137,137,137,137,137,32,137,58,137,137,32,
32,252,138,
294,294,294,294,33,294,294,60,139,189,140,
294,294,294,294,33,294,294,61,139,188,140,
166,166,166,166,166,166,166,166,166,166,166,167,
31,63,141,
142,142,142,142,142,142,142,142,142,142,142,142,142,142,32,142,142,142,142,
  142,142,142,142,142,142,142,142,142,142,142,142,142,142,142,142,32,142,
  20,64,142,142,32,
23,65,143,143,
144,144,144,66,
23,100,145,
15,68,
32,146,32,69,146,146,32,
32,147,32,70,147,147,32,
32,148,32,71,148,148,32,
32,149,32,72,149,149,32,
32,150,32,73,150,150,32,
32,151,32,551,74,151,151,32,
12,75,
32,32,76,152,32,
32,32,77,153,32,
11,78,
32,32,79,154,32,
32,32,80,155,32,
10,81,
340,340,340,340,340,340,340,156,340,340,340,340,340,340,340,340,156,340,82,
  156,156,343,
158,158,158,158,158,158,158,158,158,158,158,158,158,158,158,158,158,158,158,
  158,158,158,158,158,158,157,158,159,158,158,158,158,158,158,158,158,158,
  158,83,158,158,158,159,
160,160,160,160,160,160,160,160,160,160,160,160,160,160,336,
32,20,32,20,85,335,335,32,
161,161,161,161,32,161,161,161,161,161,161,161,161,161,161,161,32,86,161,
  161,32,
35,87,162,
32,163,20,32,20,88,163,163,32,
25,545,164,
9,90,
32,165,32,91,165,165,32,
32,32,92,166,32,
25,110,167,
8,94,
73,73,73,76,
32,32,20,96,75,75,32,
7,97,
42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,
  42,43,
30,78,168,
32,32,20,100,77,77,32,
6,101,
31,102,169,
220,205,183,103,170,172,171,27,26,
220,205,183,104,173,175,174,27,26,
220,205,105,177,176,27,26,
35,168,162,
32,357,178,179,
32,180,20,32,20,108,180,180,32,
25,351,181,
5,110,
397,397,397,396,
32,182,32,112,182,182,32,
32,32,113,183,32,
32,32,114,184,32,
32,32,20,115,384,384,32,
4,116,
73,73,73,383,
32,32,20,118,377,377,32,
3,119,
32,331,185,186,
32,187,32,20,121,187,187,32,
2,122,
73,73,73,74,
32,32,124,188,32,
32,32,20,125,70,70,32,
1,126,
227,227,227,227,227,227,227,227,227,227,227,228,
189,189,189,189,189,247,
244,244,244,244,244,244,244,190,244,129,191,191,246,
128,128,237,128,128,128,237,237,237,237,237,237,130,240,240,129,
192,236,
193,234,
229,133,
194,194,194,134,194,222,
195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,216,195,195,
  135,207,212,195,195,212,
51,51,51,51,51,51,51,51,52,136,53,224,203,223,
196,196,196,196,196,196,196,196,196,196,196,196,196,196,284,198,200,201,204,
  205,196,196,196,199,202,203,196,196,196,137,253,255,256,257,258,259,260,
  261,262,263,264,197,
206,206,206,206,206,206,206,206,32,206,206,206,206,206,206,206,206,206,206,
  206,206,206,206,206,206,206,206,206,206,32,206,138,206,206,32,
207,207,207,207,207,207,207,207,207,207,207,207,207,207,207,207,207,207,207,
  207,139,296,208,207,
209,295,
210,210,210,210,210,210,210,210,210,32,210,210,210,210,210,210,210,210,210,
  210,210,210,210,210,210,210,32,210,141,210,210,32,
211,211,211,211,211,211,211,211,211,211,211,211,211,211,211,211,211,211,211,
  211,211,211,211,211,211,211,211,211,211,211,211,211,211,211,211,420,421,
101,23,143,98,212,
109,109,109,144,
213,213,213,213,213,213,213,213,213,213,213,213,213,213,213,213,213,213,213,
  213,213,213,213,213,213,213,213,213,213,213,213,213,213,213,213,106,145,
  99,213,213,108,
29,146,214,
29,147,215,
29,148,216,
29,149,217,
29,150,218,
24,151,219,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,152,220,1,221,221,
228,228,228,228,228,228,228,226,227,230,73,71,224,233,225,228,228,228,222,
  229,231,232,228,228,228,228,153,228,499,500,503,505,510,223,235,234,498,
  501,502,504,506,507,508,509,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,154,220,1,236,236,
242,242,242,242,242,242,242,239,240,237,242,242,242,222,238,229,231,241,232,
  242,242,242,242,155,242,244,243,428,429,430,431,432,433,434,435,436,437,
  223,
339,339,341,339,
60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,
  60,60,60,60,60,60,60,60,60,60,60,60,60,157,
55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,
  157,55,55,55,55,55,55,55,55,55,55,55,57,55,55,
58,159,
340,340,340,340,340,340,340,156,340,340,340,340,340,340,340,340,156,340,160,
  156,156,342,
62,62,62,62,62,62,62,62,62,62,187,184,180,179,62,161,245,327,63,62,324,325,
  326,2,4,3,
246,246,246,246,32,246,246,246,246,246,246,246,246,246,246,246,32,162,246,
  246,32,
32,548,247,248,
249,249,249,249,249,249,249,32,249,249,249,249,249,249,249,249,249,32,164,
  249,249,32,
34,165,250,
51,51,51,51,51,51,51,51,52,166,252,251,224,223,
253,253,253,253,253,253,253,32,253,253,253,253,253,253,253,253,32,167,253,
  253,32,
98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,
  98,168,79,98,
254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,
  254,254,254,254,169,255,254,
82,82,82,82,82,82,82,82,82,82,82,82,82,82,83,170,85,102,168,201,84,
32,256,20,20,32,20,171,256,256,32,
32,257,20,20,32,20,172,257,257,32,
82,82,82,82,82,82,82,82,82,82,82,82,82,82,83,173,85,102,168,198,84,
32,258,20,20,32,20,174,258,258,32,
32,259,20,20,32,20,175,259,259,32,
32,260,20,20,32,20,176,260,260,32,
32,261,20,20,32,20,177,261,261,32,
262,262,262,262,262,262,262,32,262,262,262,262,262,262,262,262,262,262,32,
  178,262,262,32,
32,358,263,
32,355,178,264,
265,265,265,265,265,265,265,32,265,265,265,265,265,265,265,265,265,265,265,
  265,265,32,181,265,265,32,
24,182,266,
409,408,407,406,405,404,403,402,401,400,399,398,183,267,
268,268,268,184,385,269,
270,270,270,270,270,270,270,32,270,270,270,270,270,270,270,270,32,185,270,
  270,32,
32,332,271,
32,329,185,272,
1,1,1,1,1,1,1,1,1,1,1,1,1,68,67,66,65,64,1,188,69,1,71,
273,273,273,273,273,248,
128,128,128,128,128,190,241,
190,245,243,
128,128,237,128,128,128,237,237,237,237,237,237,192,239,239,129,
274,274,274,193,235,275,
73,73,73,126,
215,215,215,215,215,215,215,215,215,215,215,215,215,215,215,215,215,215,217,
  215,
276,276,276,276,276,276,276,276,290,276,276,276,290,276,276,290,276,276,276,
  276,276,290,276,290,290,290,290,276,290,196,276,276,292,
29,285,277,286,
32,278,32,198,278,278,32,
32,279,32,199,279,279,32,
280,280,280,280,280,280,280,277,280,280,280,277,280,280,277,280,280,277,277,
  277,29,277,277,200,281,280,
32,282,32,201,282,282,32,
32,283,32,202,283,283,32,
32,284,32,203,284,284,32,
32,285,32,204,285,285,32,
32,286,32,205,286,286,32,
196,196,196,196,196,196,196,196,196,196,196,196,196,196,284,198,200,201,204,
  205,196,196,196,199,202,203,196,196,196,206,254,255,256,257,258,259,260,
  261,262,263,264,197,
300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
  300,301,
287,208,
207,207,207,207,207,207,207,207,207,207,207,207,207,207,207,207,207,207,207,
  207,209,297,208,207,
288,288,288,288,288,288,288,288,288,288,288,288,288,288,288,288,288,288,288,
  288,288,288,288,288,288,288,210,164,288,
289,289,289,289,289,289,289,289,289,289,289,289,289,289,289,289,289,289,289,
  289,289,289,289,289,289,289,289,289,289,289,289,289,289,289,289,289,289,
  50,211,289,289,52,
32,32,20,212,309,309,32,
105,105,105,105,105,105,105,105,105,105,105,105,105,105,105,105,105,105,105,
  105,105,105,105,105,105,105,105,105,105,105,105,105,105,105,105,107,105,
20,20,20,20,20,20,20,32,20,20,20,20,20,20,20,20,20,290,32,214,290,290,32,
32,291,32,215,291,291,32,
32,292,32,216,292,292,32,
32,293,32,217,293,293,32,
294,294,32,294,294,294,32,218,294,294,32,
32,295,295,295,295,295,32,219,295,295,32,
32,296,32,220,296,296,32,
24,495,297,
32,298,32,222,298,298,32,
472,471,223,301,300,299,
32,302,32,224,302,302,32,
32,303,32,225,303,303,32,
32,304,32,226,304,304,32,
32,305,32,227,305,305,32,
39,39,39,39,39,39,39,306,39,39,39,39,39,39,39,307,306,228,306,
32,308,32,229,308,308,32,
32,309,32,230,309,309,32,
32,310,32,231,310,310,32,
32,311,32,232,311,311,32,
32,312,32,233,312,312,32,
32,20,32,20,234,496,496,32,
24,494,313,
24,425,297,
32,314,32,237,314,314,32,
32,315,32,238,315,315,32,
32,316,32,239,316,316,32,
32,317,32,240,317,317,32,
32,318,32,241,318,318,32,
39,39,39,39,39,39,39,319,39,39,39,39,39,39,39,307,319,242,319,
32,20,32,20,243,426,426,32,
24,424,320,
32,321,32,245,321,321,32,
62,62,62,62,62,62,62,62,62,62,187,184,180,179,62,246,322,327,63,62,324,325,
  326,2,4,3,
323,323,323,323,323,323,323,32,323,323,323,323,323,323,323,32,247,323,323,
  32,
32,549,324,
82,82,82,82,82,82,82,82,82,82,82,82,82,82,83,35,249,86,85,87,88,84,547,
325,325,325,325,325,325,325,32,325,325,325,325,325,325,325,32,250,325,325,
  32,
32,20,20,32,326,20,251,326,326,32,
114,114,114,252,327,
1,1,1,1,1,1,1,1,1,1,1,1,1,118,1,253,117,1,112,92,91,
173,173,173,173,173,173,173,173,173,173,173,173,173,173,173,173,173,173,173,
  173,173,173,173,174,
32,20,20,32,20,255,202,202,32,
33,294,294,294,256,139,200,140,
33,294,294,294,257,139,199,140,
33,294,294,294,258,139,197,140,
33,294,294,294,259,139,196,140,
33,294,294,294,260,139,195,140,
33,294,294,294,261,139,194,140,
1,1,1,1,1,1,1,1,1,1,329,330,1,1,331,1,1,262,328,1,364,359,334,333,332,
335,335,335,335,335,335,335,32,335,335,335,335,335,335,335,335,335,335,32,
  263,335,335,32,
32,356,263,
82,82,82,82,82,82,82,82,82,82,82,82,82,187,184,180,179,82,83,35,265,86,85,
  102,106,105,104,103,107,107,107,107,107,108,84,354,
336,32,336,336,32,266,336,336,32,
32,32,267,337,32,
414,414,414,268,
31,269,338,
1,1,1,1,1,1,1,1,1,1,1,1,339,1,1,270,328,1,348,340,345,
341,341,341,341,341,341,341,32,341,341,341,341,341,341,341,341,32,271,341,
  341,32,
32,330,271,
250,250,250,250,250,249,
342,342,342,231,
343,275,
289,289,289,289,289,289,289,289,289,289,289,289,289,289,289,289,289,289,289,
  289,291,289,
344,344,344,344,344,344,344,344,32,344,344,344,344,344,344,344,344,344,344,
  344,32,344,277,344,344,32,
29,278,345,
29,279,346,
1,1,1,1,1,1,1,279,1,1,1,279,1,1,279,1,1,279,279,279,279,279,280,281,1,281,
276,275,274,278,
29,282,347,
29,283,348,
29,284,349,
29,285,350,
29,286,351,
352,352,352,352,352,352,352,352,305,352,352,352,305,352,352,305,305,352,352,
  352,352,305,352,352,305,352,305,305,352,305,287,298,352,352,307,
170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,
  170,170,170,170,170,170,170,171,
49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,
  49,49,49,49,49,49,49,49,49,49,49,49,51,49,
353,353,353,353,353,353,353,353,353,353,353,353,353,353,353,353,36,290,353,
  353,
354,291,560,
83,292,454,519,518,
83,293,454,559,
355,355,355,355,355,294,520,355,355,
70,72,73,71,69,295,555,553,552,74,554,556,557,
29,296,356,491,
357,357,357,357,357,357,357,32,357,357,357,357,357,357,357,32,297,357,357,
  32,
29,298,358,
1,1,1,1,1,1,1,469,1,1,1,1,1,1,1,299,470,1,467,
359,300,360,
459,461,459,459,459,301,461,
29,302,361,
29,303,362,
29,304,363,
29,305,364,
27,365,27,306,
366,366,366,366,366,366,366,156,366,366,366,366,366,366,366,366,156,307,156,
  156,366,
29,308,367,
29,309,368,
29,310,369,
29,311,370,
29,312,371,
372,372,372,372,372,372,372,32,372,372,372,372,372,372,372,372,372,372,372,
  372,372,372,372,372,372,372,372,32,313,372,372,32,
29,314,373,
29,315,374,
29,316,375,
29,317,376,
29,318,449,377,
378,378,378,378,378,378,378,27,378,378,378,378,378,378,378,365,27,319,378,
  438,
379,379,379,379,379,379,379,32,379,379,379,379,379,379,379,379,379,379,379,
  379,379,379,379,379,32,320,379,379,32,
334,321,
32,380,32,322,380,380,32,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,323,381,1,91,
382,382,382,382,382,382,382,32,382,382,382,382,382,382,382,32,324,382,382,
  32,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,325,119,1,383,
31,127,384,
32,113,385,
32,20,386,20,32,20,328,386,386,32,
32,387,32,329,387,387,32,
32,388,32,330,388,388,32,
32,389,32,331,389,389,32,
32,20,20,32,20,332,363,363,32,
32,20,20,32,20,333,362,362,32,
32,20,20,32,20,334,361,361,32,
1,1,1,1,1,1,1,1,1,1,329,330,1,1,331,1,1,335,328,1,364,360,334,333,332,
111,111,111,336,390,113,
391,391,391,337,395,
392,392,392,338,393,
32,394,32,339,394,394,32,
32,20,32,20,340,347,347,32,
1,1,1,1,1,1,1,1,1,1,1,1,339,1,1,341,328,1,348,340,346,
233,233,233,232,
274,274,274,343,395,
196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,
  196,344,293,
396,396,396,32,396,396,396,396,396,396,32,345,396,396,32,
397,397,397,397,397,397,397,32,397,397,397,397,397,397,397,397,32,346,397,
  397,32,
398,398,398,398,398,398,398,32,398,398,398,398,398,398,398,398,398,398,398,
  398,32,347,398,398,32,
399,399,399,32,399,399,399,399,399,399,32,348,399,399,32,
400,32,400,400,32,349,400,400,32,
401,401,401,401,401,401,401,32,401,401,401,401,401,401,401,401,401,32,350,
  401,401,32,
402,402,402,402,402,402,402,32,402,402,402,402,402,402,402,402,402,402,402,
  32,351,402,402,32,
304,304,304,304,304,304,304,304,304,304,304,304,304,304,304,304,304,304,304,
  304,304,306,304,
1,1,1,1,1,1,1,478,477,1,1,1,1,1,1,1,353,479,1,403,
404,404,404,404,404,405,354,404,404,404,404,405,
406,406,406,406,406,521,406,406,
407,407,407,407,407,407,407,32,407,407,407,407,407,407,407,407,32,356,407,
  407,32,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,357,220,1,490,
32,36,32,462,408,32,463,
409,359,
410,410,410,360,410,411,
32,412,32,361,412,412,32,
20,20,20,20,20,20,20,32,20,20,20,20,20,20,20,20,20,413,32,362,413,413,32,
32,414,414,32,363,414,414,32,
32,415,32,364,415,415,32,
416,416,416,416,416,416,416,156,416,416,416,416,416,416,416,416,156,365,156,
  156,416,
418,418,418,418,418,418,418,418,418,418,418,418,418,418,417,366,418,
32,419,32,367,419,419,32,
32,420,32,368,420,420,32,
32,421,32,369,421,421,32,
32,422,32,370,422,422,32,
32,423,32,371,423,423,32,
228,228,228,228,228,228,228,226,227,230,73,71,224,233,225,228,228,228,222,
  229,231,232,228,228,228,228,372,228,499,500,503,505,510,223,424,498,501,
  502,504,506,507,508,509,
32,425,32,373,425,425,32,
32,426,426,32,374,426,426,32,
427,375,481,
427,376,480,
32,20,32,377,448,448,32,428,
39,39,39,39,39,39,39,156,39,39,39,39,39,39,39,429,156,378,156,156,429,
242,242,242,242,242,242,242,239,240,237,242,242,242,222,238,229,231,241,232,
  242,242,242,242,379,242,430,428,429,430,431,432,433,434,435,436,437,223,
333,380,
32,20,431,20,32,20,381,431,431,32,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,382,381,1,92,
32,432,32,383,432,432,32,
433,32,433,433,32,384,433,433,32,
434,434,434,434,434,434,434,32,434,434,434,434,434,434,434,434,434,434,434,
  434,434,434,434,32,385,434,434,32,
29,139,435,
29,387,436,
29,388,437,
29,389,438,
32,32,390,439,32,
440,440,440,391,
415,415,415,392,
31,393,441,
29,394,442,
443,395,
51,51,51,51,51,51,51,51,52,396,283,224,223,
1,1,1,1,1,1,1,1,1,1,1,1,1,159,1,397,160,1,282,
1,1,1,1,1,1,1,1,1,1,1,1,1,68,67,66,65,64,1,398,69,1,273,
51,51,51,51,51,51,51,51,52,399,272,224,223,
444,444,444,400,271,
1,1,1,1,1,1,1,1,1,1,1,1,269,268,1,1,401,270,1,266,267,
1,1,1,1,1,1,1,1,1,1,1,1,1,124,123,122,121,1,402,125,1,265,
459,558,459,459,459,403,558,
563,563,563,563,563,565,563,563,563,
566,405,
445,445,445,445,445,522,445,445,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,83,407,447,1,446,
36,464,465,
448,409,
73,73,73,473,
37,411,449,
36,412,450,513,
451,451,451,451,451,451,451,451,451,451,451,451,451,451,451,451,36,413,451,
  451,
544,543,414,
452,415,535,
454,454,454,454,454,454,454,454,454,454,454,454,454,454,453,416,454,
158,158,158,158,158,158,158,158,158,158,158,158,158,158,158,158,158,158,158,
  158,158,158,158,158,158,157,158,455,158,158,158,158,158,158,158,158,158,
  158,417,158,158,158,455,
39,39,39,39,39,39,39,39,39,39,39,39,39,39,483,
83,419,456,455,
456,420,529,
83,421,454,453,
83,422,440,439,
83,423,512,511,
32,20,32,20,424,497,497,32,
36,425,457,
458,457,426,
458,458,458,458,458,427,459,458,
460,428,
461,429,
32,20,32,20,430,427,427,32,
29,93,356,94,
34,432,462,
194,194,194,433,194,463,
1,1,1,1,1,1,1,1,1,1,1,1,1,467,469,470,465,464,466,471,472,1,434,328,1,115,
  129,130,131,132,133,134,135,136,137,137,145,468,154,
473,473,473,473,473,473,473,32,473,473,473,473,473,473,473,473,473,32,435,
  473,473,32,
474,32,474,474,474,32,436,474,474,32,
32,475,475,32,437,475,475,32,
32,476,476,32,438,476,476,32,
268,268,268,439,387,269,
477,477,477,410,
478,478,478,441,479,
480,480,480,480,480,480,480,32,480,480,480,480,480,480,480,32,442,480,480,
  32,
274,274,274,443,230,
481,481,481,147,
482,482,482,482,482,523,482,482,
32,20,20,32,20,446,96,96,32,
32,20,20,32,20,447,95,95,32,
468,448,
1,1,1,1,1,1,1,469,1,1,1,1,1,1,1,449,470,1,466,
62,62,62,62,62,62,62,62,62,62,187,184,180,179,62,450,483,483,327,63,62,324,
  325,326,2,4,3,
1,1,1,1,1,1,1,478,477,1,1,1,1,1,1,1,451,479,1,484,
485,485,485,485,485,485,485,485,485,485,452,485,
158,158,158,158,158,158,158,158,158,158,158,158,158,158,158,158,158,158,158,
  158,158,158,158,158,158,157,158,486,158,158,158,158,158,158,158,158,158,
  158,453,158,158,158,486,
39,39,39,39,39,39,39,39,39,39,39,39,39,39,485,
484,455,
488,488,32,488,488,488,487,32,456,487,487,32,488,489,488,
1,1,1,1,1,1,1,478,477,1,1,1,1,1,1,1,457,479,1,490,475,
62,62,62,62,62,63,
482,459,
492,492,492,492,492,492,492,491,492,492,492,492,492,492,492,492,492,492,491,
  460,491,492,
493,493,493,493,493,493,493,156,493,493,493,493,493,493,493,493,156,461,156,
  156,493,
494,494,494,494,494,494,494,32,494,494,494,494,494,494,494,494,494,494,494,
  32,462,494,494,32,
32,20,20,32,20,463,128,128,32,
32,20,20,20,32,20,464,151,151,32,
32,495,32,465,495,495,32,
32,496,32,466,496,496,32,
32,497,32,467,497,497,32,
152,29,152,152,468,498,150,
32,499,32,469,499,499,32,
32,500,32,470,500,500,32,
32,20,20,32,20,471,144,144,32,
32,20,20,32,20,472,143,143,32,
1,1,1,1,1,1,1,1,1,1,1,1,1,52,1,83,473,503,1,502,501,
117,117,117,36,474,504,117,378,379,
376,375,475,374,
505,506,476,365,
412,412,412,411,
416,416,416,478,
32,32,479,507,32,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,480,344,1,
149,149,149,148,
508,508,508,508,508,524,508,508,
514,483,
459,517,459,459,459,484,517,
537,537,537,537,509,509,537,537,537,537,537,509,537,485,509,509,
486,486,
531,487,
510,510,510,510,510,488,510,510,
530,489,
32,474,511,32,490,511,511,32,
27,27,442,
1,1,1,1,1,1,1,1,1,1,1,1,1,180,179,1,443,492,447,1,512,2,513,512,513,
515,515,515,515,515,515,515,515,515,515,515,515,515,515,514,493,515,
1,1,1,1,1,1,1,1,1,1,1,1,1,124,123,122,121,1,494,125,1,116,120,
29,495,516,
29,496,517,
29,497,518,
519,32,519,519,32,498,519,519,32,
29,499,520,
29,500,521,
32,20,20,32,20,501,142,142,32,
32,20,20,32,20,502,141,141,32,
32,20,20,32,20,503,140,140,32,
32,522,522,522,522,522,522,522,32,504,522,522,32,
366,523,366,366,366,366,505,369,
366,523,366,366,366,366,506,368,
419,418,417,507,413,
524,524,524,524,524,525,524,524,
540,509,
525,525,525,525,525,510,525,525,
24,511,526,
527,445,527,
491,528,491,513,491,528,
158,158,158,158,158,158,158,158,158,158,158,158,158,158,158,158,158,158,158,
  158,158,158,158,158,158,157,158,529,158,158,158,158,158,158,158,158,158,
  158,514,158,158,158,529,
39,39,39,39,39,39,39,39,39,39,39,39,39,39,487,
530,530,530,32,530,530,530,530,530,530,32,516,530,530,32,
531,32,531,531,32,517,531,531,32,
532,532,532,532,532,532,532,32,532,532,532,532,532,532,532,532,32,518,532,
  532,32,
194,194,194,519,194,153,
533,533,533,533,533,533,533,32,533,533,533,533,533,533,533,32,520,533,533,
  32,
534,534,32,534,534,534,534,32,521,534,534,32,
394,393,392,391,390,389,388,522,535,112,
536,370,536,536,370,370,370,370,523,367,
537,537,537,537,537,526,537,537,
538,538,538,538,538,525,538,538,
539,539,539,539,539,539,539,32,539,539,539,539,539,539,539,539,539,32,526,
  539,539,32,
180,179,527,451,2,
444,528,
488,529,
51,51,51,51,51,51,51,51,52,530,540,224,223,
444,444,444,531,541,
1,1,1,1,1,1,1,1,1,1,1,1,1,159,1,532,160,1,542,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,533,543,1,
128,128,128,128,128,130,534,544,161,132,131,129,
32,545,32,535,545,545,32,
546,546,546,371,
528,528,528,528,528,527,528,528,
547,547,547,547,547,538,547,547,
1,1,1,1,1,1,1,478,477,1,1,1,1,1,1,1,539,479,1,476,
32,20,20,32,20,540,155,155,32,
32,20,20,32,20,541,146,146,32,
32,20,20,32,20,542,158,158,32,
32,20,20,32,20,543,157,157,32,
32,20,20,32,20,544,156,156,32,
380,545,
373,373,373,372,
548,548,548,548,548,547,548,548,
549,549,549,549,549,548,549,549,
550,550,550,550,550,549,550,550,
551,551,551,551,551,550,551,551,
552,552,552,552,552,551,552,552,
553,553,553,553,553,552,553,553,
554,554,554,554,554,553,554,554,
555,555,555,555,555,554,555,555,
556,556,556,556,556,555,556,556,
557,557,557,557,557,556,557,557,
558,558,558,558,558,557,558,558,
559,559,559,559,559,558,559,559,
560,560,560,560,560,559,560,560,
561,561,561,561,561,560,561,561,
562,562,562,562,562,561,562,562,
563,563,563,563,563,562,563,563,
564,564,564,564,564,563,564,564,
565,565,565,565,565,564,565,565,
566,566,566,566,566,565,566,566,
567,567,567,567,567,566,567,567,
568,568,568,568,568,567,568,568,
569,569,569,569,569,568,569,569,
570,570,570,570,570,569,570,570,
571,571,571,571,571,570,571,571,
572,572,572,572,572,571,572,572,
573,573,573,573,573,572,573,573,
532,532,532,532,532,573,532,532,
};
static const unsigned short ag_sbt[] = {
     0,  70,  85,  93, 103, 113, 121, 125, 127, 129, 131, 133, 136, 138,
   140, 143, 146, 149, 152, 155, 158, 161, 164, 167, 170, 173, 176, 190,
   219, 231, 243, 258, 273, 276, 279, 298, 325, 333, 384, 400, 414, 435,
   456, 481, 504, 511, 542, 583, 600, 617, 654, 662, 675, 686, 689, 703,
   726, 729, 732, 767, 770, 781, 792, 804, 807, 849, 853, 857, 860, 862,
   869, 876, 883, 890, 897, 905, 907, 912, 917, 919, 924, 929, 931, 953,
   996,1011,1019,1040,1043,1052,1055,1057,1064,1069,1072,1074,1078,1085,
  1087,1114,1117,1124,1126,1129,1138,1147,1154,1157,1161,1170,1173,1175,
  1179,1186,1191,1196,1203,1205,1209,1216,1218,1222,1230,1232,1236,1241,
  1248,1250,1262,1268,1281,1297,1299,1301,1303,1309,1334,1348,1390,1425,
  1449,1451,1483,1520,1525,1529,1570,1573,1576,1579,1582,1585,1588,1607,
  1651,1670,1708,1712,1751,1791,1793,1815,1841,1862,1866,1888,1891,1905,
  1926,1955,1981,2002,2012,2022,2043,2053,2063,2073,2083,2106,2109,2113,
  2139,2142,2156,2162,2183,2186,2190,2213,2219,2226,2229,2245,2251,2255,
  2275,2308,2312,2319,2326,2352,2359,2366,2373,2380,2387,2429,2450,2452,
  2476,2505,2547,2554,2591,2614,2621,2628,2635,2646,2657,2664,2667,2674,
  2680,2687,2694,2701,2708,2727,2734,2741,2748,2755,2762,2770,2773,2776,
  2783,2790,2797,2804,2811,2830,2838,2841,2848,2874,2894,2897,2920,2940,
  2950,2955,2976,3000,3009,3017,3025,3033,3041,3049,3057,3082,3105,3108,
  3144,3153,3158,3162,3165,3186,3207,3210,3216,3220,3222,3244,3270,3273,
  3276,3302,3306,3309,3312,3315,3318,3321,3356,3383,3422,3442,3445,3450,
  3454,3463,3476,3480,3500,3503,3522,3525,3532,3535,3538,3541,3544,3548,
  3569,3572,3575,3578,3581,3584,3616,3619,3622,3625,3628,3632,3652,3681,
  3683,3690,3708,3728,3746,3749,3752,3762,3769,3776,3783,3792,3801,3810,
  3835,3841,3846,3851,3858,3866,3887,3891,3896,3918,3933,3954,3979,3994,
  4003,4025,4049,4072,4092,4104,4112,4133,4151,4158,4160,4166,4173,4196,
  4204,4211,4232,4249,4256,4263,4270,4277,4284,4327,4334,4342,4345,4348,
  4356,4377,4414,4416,4426,4444,4451,4460,4488,4491,4494,4497,4500,4505,
  4509,4513,4516,4519,4521,4534,4553,4576,4589,4594,4615,4637,4644,4653,
  4655,4663,4682,4685,4687,4691,4694,4698,4718,4721,4724,4741,4784,4799,
  4803,4806,4810,4814,4818,4826,4829,4832,4840,4842,4844,4852,4856,4859,
  4865,4904,4926,4936,4944,4952,4958,4962,4967,4987,4992,4996,5004,5013,
  5022,5024,5043,5070,5090,5102,5145,5160,5162,5177,5198,5204,5206,5228,
  5249,5273,5282,5292,5299,5306,5313,5320,5327,5334,5343,5352,5373,5382,
  5386,5390,5394,5398,5403,5420,5424,5432,5434,5441,5457,5459,5461,5469,
  5471,5479,5482,5507,5524,5547,5550,5553,5556,5565,5568,5571,5580,5589,
  5598,5611,5619,5627,5632,5640,5642,5650,5653,5656,5662,5705,5720,5735,
  5744,5765,5771,5791,5803,5813,5823,5831,5839,5861,5866,5868,5870,5883,
  5888,5907,5924,5936,5943,5947,5955,5963,5983,5992,6001,6010,6019,6028,
  6030,6034,6042,6050,6058,6066,6074,6082,6090,6098,6106,6114,6122,6130,
  6138,6146,6154,6162,6170,6178,6186,6194,6202,6210,6218,6226,6234,6242,
  6250
};
static const unsigned short ag_sbe[] = {
    50,  84,  87,  96, 106, 116, 122, 126, 128, 130, 132, 134, 137, 139,
   141, 144, 147, 150, 153, 156, 159, 162, 165, 168, 171, 174, 185, 213,
   227, 239, 254, 269, 275, 277, 297, 313, 329, 380, 395, 405, 429, 450,
   472, 496, 507, 537, 563, 593, 610, 637, 657, 673, 681, 687, 698, 725,
   727, 730, 763, 768, 777, 788, 803, 805, 845, 850, 856, 858, 861, 865,
   872, 879, 886, 893, 901, 906, 909, 914, 918, 921, 926, 930, 949, 991,
  1010,1015,1036,1041,1048,1053,1056,1060,1066,1070,1073,1077,1081,1086,
  1113,1115,1120,1125,1127,1132,1141,1149,1155,1158,1166,1171,1174,1178,
  1182,1188,1193,1199,1204,1208,1212,1217,1219,1226,1231,1235,1238,1244,
  1249,1261,1267,1277,1293,1298,1300,1302,1306,1328,1343,1377,1421,1445,
  1450,1479,1518,1522,1528,1565,1571,1574,1577,1580,1583,1586,1602,1633,
  1665,1693,1710,1750,1788,1792,1811,1830,1858,1863,1884,1889,1900,1922,
  1952,1978,1996,2008,2018,2037,2049,2059,2069,2079,2102,2107,2110,2135,
  2140,2154,2159,2179,2184,2187,2209,2218,2224,2227,2241,2248,2254,2273,
  2304,2309,2315,2322,2349,2355,2362,2369,2376,2383,2416,2449,2451,2472,
  2502,2543,2550,2589,2610,2617,2624,2631,2642,2653,2660,2665,2670,2676,
  2683,2690,2697,2704,2725,2730,2737,2744,2751,2758,2766,2771,2774,2779,
  2786,2793,2800,2807,2828,2834,2839,2844,2863,2890,2895,2913,2936,2946,
  2953,2970,2999,3005,3013,3021,3029,3037,3045,3053,3074,3101,3106,3128,
  3149,3155,3161,3163,3180,3203,3208,3215,3219,3221,3242,3266,3271,3274,
  3298,3305,3307,3310,3313,3316,3319,3351,3382,3420,3439,3443,3446,3451,
  3459,3468,3477,3496,3501,3518,3523,3530,3533,3536,3539,3542,3547,3565,
  3570,3573,3576,3579,3582,3612,3617,3620,3623,3626,3629,3649,3677,3682,
  3686,3704,3724,3742,3747,3750,3758,3765,3772,3779,3788,3797,3806,3827,
  3838,3844,3849,3854,3862,3881,3890,3894,3916,3929,3950,3975,3990,3999,
  4021,4045,4070,4088,4098,4109,4129,4147,4154,4159,4163,4169,4192,4200,
  4207,4228,4247,4252,4259,4266,4273,4280,4310,4330,4338,4343,4346,4351,
  4373,4400,4415,4422,4440,4447,4456,4484,4489,4492,4495,4498,4502,4508,
  4512,4514,4517,4520,4530,4549,4572,4585,4592,4610,4633,4642,4649,4654,
  4660,4678,4683,4686,4690,4692,4695,4715,4720,4722,4739,4779,4798,4800,
  4804,4807,4811,4815,4822,4827,4831,4837,4841,4843,4848,4853,4857,4862,
  4887,4922,4932,4940,4948,4955,4961,4965,4983,4990,4995,5001,5009,5018,
  5023,5039,5058,5086,5100,5140,5159,5161,5170,5193,5203,5205,5225,5245,
  5269,5278,5288,5295,5302,5309,5317,5323,5330,5339,5348,5368,5377,5384,
  5388,5393,5397,5400,5417,5423,5429,5433,5439,5454,5458,5460,5466,5470,
  5475,5481,5499,5522,5542,5548,5551,5554,5561,5566,5569,5576,5585,5594,
  5607,5617,5625,5630,5637,5641,5647,5651,5654,5659,5700,5719,5731,5740,
  5761,5768,5787,5799,5810,5821,5828,5836,5857,5863,5867,5869,5879,5886,
  5903,5921,5930,5939,5946,5952,5960,5979,5988,5997,6006,6015,6024,6029,
  6033,6039,6047,6055,6063,6071,6079,6087,6095,6103,6111,6119,6127,6135,
  6143,6151,6159,6167,6175,6183,6191,6199,6207,6215,6223,6231,6239,6247,6250
};
static const unsigned char ag_fl[] = {
  1,4,4,4,4,4,4,4,4,4,4,4,4,2,2,4,2,2,2,2,0,1,2,1,1,1,1,2,1,1,1,1,1,1,1,
  1,1,1,1,2,1,1,2,1,1,2,1,1,1,2,0,1,2,1,1,2,0,1,3,1,2,1,2,1,1,1,1,1,1,1,
  2,3,1,2,1,2,1,2,1,3,1,5,0,1,3,4,1,1,3,4,4,3,4,2,3,4,4,1,2,5,3,1,1,1,1,
  2,0,1,1,3,1,1,4,4,0,4,9,1,1,1,1,1,1,1,1,1,1,2,6,1,1,1,1,1,1,1,1,1,1,2,
  6,6,6,2,2,1,6,1,2,3,2,2,0,3,1,6,6,6,6,1,1,1,1,1,4,1,2,1,1,1,2,1,1,2,1,
  1,1,1,2,1,1,3,2,1,1,3,2,1,3,3,1,1,1,1,4,4,4,4,3,4,4,3,4,4,3,1,1,3,1,1,
  2,1,1,1,1,2,0,1,1,2,1,1,3,1,1,1,1,2,2,3,5,1,2,3,1,3,1,0,1,3,2,2,1,2,0,
  1,2,1,2,3,4,0,1,3,4,1,1,1,1,1,1,1,1,1,1,5,5,5,1,1,1,5,5,5,3,3,3,0,1,0,
  1,3,5,5,1,1,2,1,1,2,0,1,2,3,0,1,2,3,3,1,2,1,1,1,2,0,1,1,1,6,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,3,1,2,6,5,2,1,1,1,2,0,1,3,2,5,3,4,2,1,1,
  1,1,1,1,4,2,3,1,2,3,4,2,2,2,1,5,0,2,2,2,0,1,2,3,5,1,1,2,5,1,5,1,1,1,2,
  3,1,7,1,1,1,1,1,1,1,5,1,2,1,1,1,1,1,1,1,1,1,1,1,1,2,3,4,7,2,2,2,1,1,1,
  4,5,1,1,3,3,2,5,1,1,1,1,1,1,1,1,1,1,3,5,1,0,1,7,9,1,1,1,1,1,1,3,1,5,1,
  5,1,5,5,0,1,3,3,4,4,5,5,2,4,1,1,1,1,1,7,1,5,1,1,1,4,4,3,4,6,5,7,5,7,1,
  4,3,1,1,3,3,2,5,1,1,1,1,1,1,1,1,1,1,1,1,1,5,1,5,3,0,1,7,5,1,5,1,2,3,4,
  5,6,7,8,5,3,3,32,1,1,5,1,2,0,1,4,1,1,5,5,1,1,4,2,3,1,1,5,1,1,1,1,1,7,5,
  5,1,1,2,0,1,3
};
static const unsigned short ag_ptt[] = {
    0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1, 38, 38,  3, 39, 41, 43, 45, 45, 37, 46, 48, 50, 51, 53, 55, 57,
   59, 61, 65, 65, 63, 68, 68, 66, 71, 71, 69, 75, 76, 76, 77, 77, 72, 80,
   81, 81, 82, 82, 78, 79, 79, 88, 88, 86, 89, 89, 89, 89, 89, 89,  4, 95,
   98, 98, 96, 17, 99, 15,100,100,101,102,105,105,106,106,103,104,107,108,
  108,109,109,110,110,111,111,113,113, 28, 28,112,114,119,120,120,121,121,
  115,117, 19,122,122,123,126,126,124,128,128,129,130,132,132,132,132,132,
  137,125,125,127,127,127,127,127,127,127,127,127,147,148,148,148,148,139,
  140,141,152,154,154,154,145,155,156,156,142,158,143,144,146,164,164,161,
  167, 31,169,173,173,170,174,177,177,171,180,180,178, 34, 32, 33,181,184,
  184,182,182,189,188,183,183,192,185,185,198,198,198,198,199,199,200,200,
  200,201,201,201,202,194,194,203,204,204,207,211,211,209,208,214,215,215,
  216,216,212,197,217,205,205,138,138,219,222,222,219,149,225,226,226,226,
  166,166,228,231,231,228,228,233,234,234,235,235,229,232,232,232,232,195,
  195,236,236,237,237,237,237,237,237,237,237,237,237,238,239,239,250,250,
  251,240,241,242,243,243,243,259,259,260,260,243,244,245,246,247,247,266,
  267,267,268,268,263,264,196,196,269,269,270,275,275,272,277,278,278,279,
  279,273, 29,280,281,281,281,281,281,281,281,281,281,281,281,281,281,281,
  168,168,168,168,  7,295,295,295,295,296,296,175,175,300,301,301,302,302,
  299,299,303,297,297,305,305, 13,306,306,307,308,308,310,310,310,310,311,
  311,312,312,312,312,313,320,320,317,317,319,319,319,319,314,323,323,  9,
  315,328,328,326,326,329, 11,331,331,330,334,334,334,334,334,334,334,332,
  342,342,343,343,343,343,343,343,343,343,343,343,343,343,344,344,344,333,
  357,358,359,360,360,360, 35, 35, 23,364,365,365,367,367,369,369,369,369,
  369,369,369,369,369,369,369,370,382,386,386,371,371,387,387,389,385,384,
  388,388,390,372,393,373,395,377,377,400,400,378,398,398,398,398,399,399,
  403,405,405,402,406,404,379,414,414,415,415,415,374,375,419,376,376,376,
  376,380,380,368,368,421, 25,422,423,423,424,424,425,425,425,425,425,425,
  425,425,425,425,425,425,425,426,435,433,437,438,438,432,428,440,429,442,
  442,442,442,442,442,442,442,427,445,445,446,448,443,430,452,452,454,454,
  450,453,453,431,431, 21,460,460,461,461, 27,462,462,463,463,463,463,463,
  466,464,465,470,471,471,472,472,469
};
static void ag_ra(PCB_DECL)
{
  switch(ag_rpx[(PCB).ag_ap]) {
    case 1: ag_rp_1(PCB_POINTER); break;
    case 2: ag_rp_2(PCB_POINTER); break;
    case 3: ag_rp_3(PCB_POINTER); break;
    case 4: ag_rp_4(PCB_POINTER); break;
    case 5: ag_rp_5(PCB_POINTER); break;
    case 6: ag_rp_6(PCB_POINTER); break;
    case 7: ag_rp_7(PCB_POINTER); break;
    case 8: ag_rp_8(PCB_POINTER); break;
    case 9: ag_rp_9(PCB_POINTER); break;
    case 10: ag_rp_10(PCB_POINTER); break;
    case 11: ag_rp_11(PCB_POINTER); break;
    case 12: ag_rp_12(PCB_POINTER); break;
    case 13: ag_rp_13(PCB_POINTER); break;
    case 14: ag_rp_14(PCB_POINTER); break;
    case 15: V(0,ParserBasicToken) = ag_rp_15(PCB_POINTER); break;
    case 16: V(0,ParserBasicToken) = ag_rp_16(PCB_POINTER); break;
    case 17: V(0,ParserBasicToken) = ag_rp_17(PCB_POINTER); break;
    case 18: V(0,ParserBasicToken) = ag_rp_18(PCB_POINTER); break;
    case 19: V(0,ParserQuotedString) = ag_rp_19(PCB_POINTER); break;
    case 20: V(0,ParserBasicToken) = ag_rp_20(PCB_POINTER); break;
    case 21: V(0,ParserMethod) = ag_rp_21(PCB_POINTER); break;
    case 22: V(0,ParserMethod) = ag_rp_22(PCB_POINTER); break;
    case 23: V(0,ParserMethod) = ag_rp_23(PCB_POINTER); break;
    case 24: V(0,ParserMethod) = ag_rp_24(PCB_POINTER); break;
    case 25: V(0,ParserMethod) = ag_rp_25(PCB_POINTER); break;
    case 26: V(0,ParserMethod) = ag_rp_26(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 27: ag_rp_27(PCB_POINTER, V(0,ParserCSeq)); break;
    case 28: V(0,ParserCSeq) = ag_rp_28(PCB_POINTER, V(0,ParserSequenceNumber), V(2,ParserMethod)); break;
    case 29: V(0,ParserSequenceNumber) = ag_rp_29(PCB_POINTER); break;
    case 30: ag_rp_30(PCB_POINTER, V(0,ParserContentLength)); break;
    case 31: V(0,ParserContentLength) = ag_rp_31(PCB_POINTER); break;
    case 32: ag_rp_32(PCB_POINTER, V(0,ParserCallId)); break;
    case 33: V(0,ParserCallId) = ag_rp_33(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 34: V(0,ParserCallId) = ag_rp_34(PCB_POINTER, V(0,ParserBasicToken), V(2,ParserBasicToken)); break;
    case 35: ag_rp_35(PCB_POINTER); break;
    case 36: ag_rp_36(PCB_POINTER, V(0,ParserMediaType), V(2,ParserMediaSubType)); break;
    case 37: ag_rp_37(PCB_POINTER); break;
    case 38: ag_rp_38(PCB_POINTER); break;
    case 39: V(0,ParserMediaType) = ag_rp_39(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 40: V(0,ParserMediaSubType) = ag_rp_40(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 41: ag_rp_41(PCB_POINTER, V(0,ParserBasicToken), V(2,ParserGenericParamDataVal)); break;
    case 42: V(0,ParserGenericParamDataVal) = ag_rp_42(PCB_POINTER, V(2,ParserBasicToken)); break;
    case 43: V(0,ParserGenericParamDataVal) = ag_rp_43(PCB_POINTER, V(2,ParserQuotedString)); break;
    case 44: ag_rp_44(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 45: ag_rp_45(PCB_POINTER, V(0,ParserBasicToken), V(2,ParserGenericParamDataVal)); break;
    case 46: V(0,ParserGenericParamDataVal) = ag_rp_46(PCB_POINTER, V(2,ParserBasicToken)); break;
    case 47: V(0,ParserGenericParamDataVal) = ag_rp_47(PCB_POINTER, V(2,ParserQuotedString)); break;
    case 48: ag_rp_48(PCB_POINTER, V(0,ParserSipVersion), V(2,ParserStatusCode), V(4,ParserReasonPhrase)); break;
    case 49: ag_rp_49(PCB_POINTER, V(0,ParserSipVersion), V(2,ParserStatusCode)); break;
    case 50: V(0,ParserSipVersion) = ag_rp_50(PCB_POINTER); break;
    case 51: V(0,ParserReasonPhrase) = ag_rp_51(PCB_POINTER); break;
    case 52: V(0,ParserStatusCode) = ag_rp_52(PCB_POINTER, V(0,int), V(1,int), V(2,int)); break;
    case 53: ag_rp_53(PCB_POINTER); break;
    case 54: ag_rp_54(PCB_POINTER); break;
    case 55: ag_rp_55(PCB_POINTER); break;
    case 56: ag_rp_56(PCB_POINTER); break;
    case 57: ag_rp_57(PCB_POINTER, V(3,ParserViaParam)); break;
    case 58: ag_rp_58(PCB_POINTER, V(0,ParserProtocolName), V(4,ParserProtocolVersion), V(8,ParserTransport)); break;
    case 59: V(0,ParserProtocolName) = ag_rp_59(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 60: V(0,ParserProtocolName) = ag_rp_60(PCB_POINTER); break;
    case 61: V(0,ParserProtocolVersion) = ag_rp_61(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 62: V(0,ParserTransport) = ag_rp_62(PCB_POINTER, V(0,ParserTransport)); break;
    case 63: V(0,ParserTransport) = ag_rp_63(PCB_POINTER); break;
    case 64: V(0,ParserTransport) = ag_rp_64(PCB_POINTER); break;
    case 65: V(0,ParserTransport) = ag_rp_65(PCB_POINTER); break;
    case 66: V(0,ParserTransport) = ag_rp_66(PCB_POINTER); break;
    case 67: V(0,ParserTransport) = ag_rp_67(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 68: V(0,ParserPort) = ag_rp_68(PCB_POINTER); break;
    case 69: ag_rp_69(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 70: ag_rp_70(PCB_POINTER, V(0,ParserBasicToken), V(4,ParserPort)); break;
    case 71: V(0,ParserViaParam) = ag_rp_71(PCB_POINTER); break;
    case 72: V(0,ParserViaParam) = ag_rp_72(PCB_POINTER); break;
    case 73: V(0,ParserViaParam) = ag_rp_73(PCB_POINTER, V(0,ParserTTLParam)); break;
    case 74: V(0,ParserViaParam) = ag_rp_74(PCB_POINTER, V(0,ParserMaddrParam)); break;
    case 75: V(0,ParserViaParam) = ag_rp_75(PCB_POINTER, V(0,ParserReceived)); break;
    case 76: V(0,ParserViaParam) = ag_rp_76(PCB_POINTER, V(0,ParserBranch)); break;
    case 77: V(0,ParserViaParam) = ag_rp_77(PCB_POINTER, V(0,ParserPort)); break;
    case 78: V(0,ParserViaParam) = ag_rp_78(PCB_POINTER, V(0,ParserCompParam)); break;
    case 79: V(0,ParserViaParam) = ag_rp_79(PCB_POINTER); break;
    case 80: ag_rp_80(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 81: ag_rp_81(PCB_POINTER, V(0,ParserBasicToken), V(4,ParserBasicToken)); break;
    case 82: ag_rp_82(PCB_POINTER, V(0,ParserBasicToken), V(4,ParserQuotedString)); break;
    case 83: ag_rp_83(PCB_POINTER, V(0,ParserBasicToken), V(4,ParserBasicToken)); break;
    case 84: V(0,ParserTTLParam) = ag_rp_84(PCB_POINTER, V(0,ParserTtlParam)); break;
    case 85: V(0,ParserTtlParam) = ag_rp_85(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 86: V(0,ParserBasicToken) = ag_rp_86(PCB_POINTER); break;
    case 87: V(0,ParserBasicToken) = ag_rp_87(PCB_POINTER); break;
    case 88: V(0,ParserBasicToken) = ag_rp_88(PCB_POINTER); break;
    case 89: V(0,ParserPort) = ag_rp_89(PCB_POINTER, V(1,ParserPort)); break;
    case 90: V(0,ParserPort) = ag_rp_90(PCB_POINTER); break;
    case 91: V(0,ParserPort) = ag_rp_91(PCB_POINTER, V(2,ParserPort)); break;
    case 92: V(0,ParserMaddrParam) = ag_rp_92(PCB_POINTER, V(0,ParserMaddrParam)); break;
    case 93: V(0,ParserMaddrParam) = ag_rp_93(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 94: V(0,ParserReceived) = ag_rp_94(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 95: V(0,ParserBranch) = ag_rp_95(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 96: V(0,ParserCompParam) = ag_rp_96(PCB_POINTER, V(4,ParserCompParam)); break;
    case 97: V(0,ParserCompParam) = ag_rp_97(PCB_POINTER); break;
    case 98: V(0,ParserCompParam) = ag_rp_98(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 99: V(0,ParserBasicToken) = ag_rp_99(PCB_POINTER); break;
    case 100: ag_rp_100(PCB_POINTER, V(0,ParserAbsoluteUri)); break;
    case 101: V(0,ParserAbsoluteUri) = ag_rp_101(PCB_POINTER, V(0,ParserBasicToken), V(3,ParserBasicToken)); break;
    case 102: V(0,ParserBasicToken) = ag_rp_102(PCB_POINTER); break;
    case 103: V(0,ParserDisplayName) = ag_rp_103(PCB_POINTER, V(0,ParserDisplayName)); break;
    case 104: V(0,ParserBasicToken) = ag_rp_104(PCB_POINTER); break;
    case 105: V(0,ParserBasicToken) = ag_rp_105(PCB_POINTER); break;
    case 106: ag_rp_106(PCB_POINTER); break;
    case 107: ag_rp_107(PCB_POINTER, V(0,ParserAbsoluteUri)); break;
    case 108: ag_rp_108(PCB_POINTER, V(0,ParserAbsoluteUri)); break;
    case 109: ag_rp_109(PCB_POINTER); break;
    case 110: ag_rp_110(PCB_POINTER); break;
    case 111: V(0,ParserAbsoluteUri) = ag_rp_111(PCB_POINTER, V(2,ParserAbsoluteUri)); break;
    case 112: V(0,ParserAbsoluteUri) = ag_rp_112(PCB_POINTER); break;
    case 113: ag_rp_113(PCB_POINTER); break;
    case 114: ag_rp_114(PCB_POINTER); break;
    case 115: V(0,ParserAbsoluteUri) = ag_rp_115(PCB_POINTER, V(2,ParserAbsoluteUri)); break;
    case 116: V(0,ParserAbsoluteUri) = ag_rp_116(PCB_POINTER); break;
    case 117: ag_rp_117(PCB_POINTER); break;
    case 118: ag_rp_118(PCB_POINTER, V(2,ParserOptionalHeaders)); break;
    case 119: ag_rp_119(PCB_POINTER, V(2,ParserOptionalHeaders)); break;
    case 120: ag_rp_120(PCB_POINTER, V(3,ParserOptionalHeaders)); break;
    case 121: ag_rp_121(PCB_POINTER, V(3,ParserOptionalHeaders)); break;
    case 122: ag_rp_122(PCB_POINTER, V(3,ParserOptionalHeaders)); break;
    case 123: ag_rp_123(PCB_POINTER, V(3,ParserOptionalHeaders)); break;
    case 124: ag_rp_124(PCB_POINTER, V(3,ParserOptionalHeaders)); break;
    case 125: ag_rp_125(PCB_POINTER, V(3,ParserOptionalHeaders)); break;
    case 126: ag_rp_126(PCB_POINTER, V(0,ParserDisplayName), V(2,ParserBasicToken)); break;
    case 127: ag_rp_127(PCB_POINTER); break;
    case 128: ag_rp_128(PCB_POINTER); break;
    case 129: ag_rp_129(PCB_POINTER); break;
    case 130: ag_rp_130(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 131: ag_rp_131(PCB_POINTER, V(0,ParserBasicToken), V(2,ParserBasicToken)); break;
    case 132: V(0,ParserBasicToken) = ag_rp_132(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 133: V(0,ParserBasicToken) = ag_rp_133(PCB_POINTER); break;
    case 134: V(0,ParserBasicToken) = ag_rp_134(PCB_POINTER); break;
    case 135: ag_rp_135(PCB_POINTER); break;
    case 136: ag_rp_136(PCB_POINTER); break;
    case 137: ag_rp_137(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 138: ag_rp_138(PCB_POINTER, V(0,ParserBasicToken), V(2,ParserPort)); break;
    case 139: V(0,ParserBasicToken) = ag_rp_139(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 140: V(0,ParserBasicToken) = ag_rp_140(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 141: V(0,ParserBasicToken) = ag_rp_141(PCB_POINTER); break;
    case 142: V(0,ParserBasicToken) = ag_rp_142(PCB_POINTER); break;
    case 143: V(0,ParserBasicToken) = ag_rp_143(PCB_POINTER); break;
    case 144: ag_rp_144(PCB_POINTER); break;
    case 145: ag_rp_145(PCB_POINTER); break;
    case 146: ag_rp_146(PCB_POINTER); break;
    case 147: ag_rp_147(PCB_POINTER); break;
    case 148: ag_rp_148(PCB_POINTER, V(0,ParserTransport)); break;
    case 149: ag_rp_149(PCB_POINTER, V(0,ParserUserParam)); break;
    case 150: ag_rp_150(PCB_POINTER, V(0,ParserTtlParam)); break;
    case 151: ag_rp_151(PCB_POINTER, V(0,ParserMaddrParam)); break;
    case 152: ag_rp_152(PCB_POINTER, V(0,ParserMethod)); break;
    case 153: ag_rp_153(PCB_POINTER, V(0,ParserLrParamType)); break;
    case 154: ag_rp_154(PCB_POINTER, V(0,ParserCompParam)); break;
    case 155: ag_rp_155(PCB_POINTER, V(0,ParserTokenizedByParam)); break;
    case 156: ag_rp_156(PCB_POINTER, V(0,RvBool)); break;
    case 157: ag_rp_157(PCB_POINTER); break;
    case 158: V(0,ParserTransport) = ag_rp_158(PCB_POINTER, V(4,ParserTransport)); break;
    case 159: V(0,ParserUserParam) = ag_rp_159(PCB_POINTER, V(4,RvSipUserParam)); break;
    case 160: V(0,ParserUserParam) = ag_rp_160(PCB_POINTER, V(4,ParserOtherUser)); break;
    case 161: V(0,RvSipUserParam) = ag_rp_161(PCB_POINTER); break;
    case 162: V(0,RvSipUserParam) = ag_rp_162(PCB_POINTER); break;
    case 163: V(0,ParserOtherUser) = ag_rp_163(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 164: V(0,ParserTtlParam) = ag_rp_164(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 165: V(0,ParserMaddrParam) = ag_rp_165(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 166: V(0,ParserMethod) = ag_rp_166(PCB_POINTER, V(4,ParserMethod)); break;
    case 167: V(0,ParserLrParamType) = ag_rp_167(PCB_POINTER); break;
    case 168: V(0,ParserLrParamType) = ag_rp_168(PCB_POINTER); break;
    case 169: V(0,ParserLrParamType) = ag_rp_169(PCB_POINTER); break;
    case 170: V(0,ParserLrParamType) = ag_rp_170(PCB_POINTER); break;
    case 171: V(0,ParserCompParam) = ag_rp_171(PCB_POINTER, V(4,ParserCompParam)); break;
    case 172: V(0,ParserTokenizedByParam) = ag_rp_172(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 173: V(0,RvBool) = ag_rp_173(PCB_POINTER); break;
    case 174: ag_rp_174(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 175: ag_rp_175(PCB_POINTER, V(0,ParserBasicToken), V(1,ParserBasicToken)); break;
    case 176: V(0,ParserBasicToken) = ag_rp_176(PCB_POINTER); break;
    case 177: V(0,ParserBasicToken) = ag_rp_177(PCB_POINTER, V(2,ParserBasicToken)); break;
    case 178: V(0,ParserOptionalHeaders) = ag_rp_178(PCB_POINTER); break;
    case 179: V(0,ParserOptionalHeaders) = ag_rp_179(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 180: V(0,ParserBasicToken) = ag_rp_180(PCB_POINTER); break;
    case 181: V(0,ParserBasicToken) = ag_rp_181(PCB_POINTER); break;
    case 182: V(0,ParserBasicToken) = ag_rp_182(PCB_POINTER); break;
    case 183: V(0,ParserBasicToken) = ag_rp_183(PCB_POINTER); break;
    case 184: V(0,ParserBasicToken) = ag_rp_184(PCB_POINTER); break;
    case 185: ag_rp_185(PCB_POINTER); break;
    case 186: ag_rp_186(PCB_POINTER, V(0,ParserMethod), V(4,ParserSipVersion)); break;
    case 187: V(0,ParserMethod) = ag_rp_187(PCB_POINTER); break;
    case 188: V(0,ParserMethod) = ag_rp_188(PCB_POINTER); break;
    case 189: V(0,ParserMethod) = ag_rp_189(PCB_POINTER); break;
    case 190: V(0,ParserMethod) = ag_rp_190(PCB_POINTER); break;
    case 191: V(0,ParserMethod) = ag_rp_191(PCB_POINTER); break;
    case 192: V(0,ParserMethod) = ag_rp_192(PCB_POINTER); break;
    case 193: V(0,ParserMethod) = ag_rp_193(PCB_POINTER); break;
    case 194: V(0,ParserMethod) = ag_rp_194(PCB_POINTER); break;
    case 195: V(0,ParserMethod) = ag_rp_195(PCB_POINTER); break;
    case 196: V(0,ParserMethod) = ag_rp_196(PCB_POINTER); break;
    case 197: V(0,ParserMethod) = ag_rp_197(PCB_POINTER); break;
    case 198: V(0,ParserMethod) = ag_rp_198(PCB_POINTER); break;
    case 199: V(0,ParserMethod) = ag_rp_199(PCB_POINTER); break;
    case 200: V(0,ParserMethod) = ag_rp_200(PCB_POINTER, V(0,ParserMethod)); break;
    case 201: ag_rp_201(PCB_POINTER); break;
    case 202: ag_rp_202(PCB_POINTER, V(0,ParserAbsoluteUri)); break;
    case 203: ag_rp_203(PCB_POINTER, V(0,ParserAbsoluteUri)); break;
    case 204: ag_rp_204(PCB_POINTER, V(0,ParserAbsoluteUri)); break;
    case 205: ag_rp_205(PCB_POINTER); break;
    case 206: ag_rp_206(PCB_POINTER); break;
    case 207: ag_rp_207(PCB_POINTER, V(2,ParserPartyParams)); break;
    case 208: ag_rp_208(PCB_POINTER); break;
    case 209: ag_rp_209(PCB_POINTER, V(1,ParserPartyParams)); break;
    case 210: ag_rp_210(PCB_POINTER, V(0,ParserDisplayName)); break;
    case 211: ag_rp_211(PCB_POINTER); break;
    case 212: V(0,ParserDisplayName) = ag_rp_212(PCB_POINTER, V(0,ParserQuotedString)); break;
    case 213: V(0,ParserDisplayName) = ag_rp_213(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 214: V(0,ParserBasicToken) = ag_rp_214(PCB_POINTER); break;
    case 215: V(0,ParserBasicToken) = ag_rp_215(PCB_POINTER); break;
    case 216: V(0,ParserTagParam) = ag_rp_216(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 217: V(0,ParserPartyParams) = ag_rp_217(PCB_POINTER, V(2,ParserPartyParams)); break;
    case 218: V(0,ParserPartyParams) = ag_rp_218(PCB_POINTER, V(0,ParserPartyParams), V(3,ParserPartyParams)); break;
    case 219: V(0,ParserPartyParams) = ag_rp_219(PCB_POINTER, V(0,ParserTagParam)); break;
    case 220: V(0,ParserPartyParams) = ag_rp_220(PCB_POINTER); break;
    case 221: ag_rp_221(PCB_POINTER); break;
    case 222: ag_rp_222(PCB_POINTER); break;
    case 223: ag_rp_223(PCB_POINTER); break;
    case 224: ag_rp_224(PCB_POINTER); break;
    case 225: ag_rp_225(PCB_POINTER); break;
    case 226: ag_rp_226(PCB_POINTER); break;
    case 227: ag_rp_227(PCB_POINTER); break;
    case 228: ag_rp_228(PCB_POINTER, V(0,ParserQVal)); break;
    case 229: ag_rp_229(PCB_POINTER, V(0,ParserContactActionType)); break;
    case 230: ag_rp_230(PCB_POINTER); break;
    case 231: ag_rp_231(PCB_POINTER); break;
    case 232: V(0,ParserQVal) = ag_rp_232(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 233: V(0,ParserBasicToken) = ag_rp_233(PCB_POINTER); break;
    case 234: V(0,ParserBasicToken) = ag_rp_234(PCB_POINTER); break;
    case 235: V(0,ParserContactActionType) = ag_rp_235(PCB_POINTER, V(4,ParserContactActionType)); break;
    case 236: V(0,ParserContactActionType) = ag_rp_236(PCB_POINTER); break;
    case 237: V(0,ParserContactActionType) = ag_rp_237(PCB_POINTER); break;
    case 238: ag_rp_238(PCB_POINTER); break;
    case 239: ag_rp_239(PCB_POINTER, V(0,ParserDeltaSeconds)); break;
    case 240: ag_rp_240(PCB_POINTER, V(2,ParserSipDate)); break;
    case 241: ag_rp_241(PCB_POINTER, V(0,ParserDeltaSeconds)); break;
    case 242: ag_rp_242(PCB_POINTER, V(0,ParserSipDate)); break;
    case 243: V(0,ParserDeltaSeconds) = ag_rp_243(PCB_POINTER); break;
    case 244: ag_rp_244(PCB_POINTER, V(0,ParserSipDate)); break;
    case 245: V(0,ParserSipDate) = ag_rp_245(PCB_POINTER, V(0,ParserDDMMYY), V(2,ParserTime)); break;
    case 246: V(0,ParserSipDate) = ag_rp_246(PCB_POINTER, V(0,ParserSipDate)); break;
    case 247: V(0,ParserSipDate) = ag_rp_247(PCB_POINTER, V(0,RvSipDateWeekDay), V(4,ParserDDMMYY), V(6,ParserTime)); break;
    case 248: V(0,RvSipDateWeekDay) = ag_rp_248(PCB_POINTER); break;
    case 249: V(0,RvSipDateWeekDay) = ag_rp_249(PCB_POINTER); break;
    case 250: V(0,RvSipDateWeekDay) = ag_rp_250(PCB_POINTER); break;
    case 251: V(0,RvSipDateWeekDay) = ag_rp_251(PCB_POINTER); break;
    case 252: V(0,RvSipDateWeekDay) = ag_rp_252(PCB_POINTER); break;
    case 253: V(0,RvSipDateWeekDay) = ag_rp_253(PCB_POINTER); break;
    case 254: V(0,RvSipDateWeekDay) = ag_rp_254(PCB_POINTER); break;
    case 255: V(0,ParserDDMMYY) = ag_rp_255(PCB_POINTER, V(0,ParserInt32), V(2,RvSipDateMonth), V(4,ParserInt32)); break;
    case 256: V(0,ParserInt32) = ag_rp_256(PCB_POINTER, V(0,int)); break;
    case 257: V(0,ParserInt32) = ag_rp_257(PCB_POINTER, V(0,int), V(1,int)); break;
    case 258: V(0,RvSipDateMonth) = ag_rp_258(PCB_POINTER); break;
    case 259: V(0,RvSipDateMonth) = ag_rp_259(PCB_POINTER); break;
    case 260: V(0,RvSipDateMonth) = ag_rp_260(PCB_POINTER); break;
    case 261: V(0,RvSipDateMonth) = ag_rp_261(PCB_POINTER); break;
    case 262: V(0,RvSipDateMonth) = ag_rp_262(PCB_POINTER); break;
    case 263: V(0,RvSipDateMonth) = ag_rp_263(PCB_POINTER); break;
    case 264: V(0,RvSipDateMonth) = ag_rp_264(PCB_POINTER); break;
    case 265: V(0,RvSipDateMonth) = ag_rp_265(PCB_POINTER); break;
    case 266: V(0,RvSipDateMonth) = ag_rp_266(PCB_POINTER); break;
    case 267: V(0,RvSipDateMonth) = ag_rp_267(PCB_POINTER); break;
    case 268: V(0,RvSipDateMonth) = ag_rp_268(PCB_POINTER); break;
    case 269: V(0,RvSipDateMonth) = ag_rp_269(PCB_POINTER); break;
    case 270: V(0,ParserInt32) = ag_rp_270(PCB_POINTER, V(0,int), V(1,int)); break;
    case 271: V(0,ParserInt32) = ag_rp_271(PCB_POINTER, V(0,int), V(1,int), V(2,int)); break;
    case 272: V(0,ParserInt32) = ag_rp_272(PCB_POINTER, V(0,int), V(1,int), V(2,int), V(3,int)); break;
    case 273: V(0,ParserTime) = ag_rp_273(PCB_POINTER, V(0,ParserInt32), V(2,ParserInt32), V(4,ParserInt32), V(6,ParserTimeZone)); break;
    case 274: V(0,ParserInt32) = ag_rp_274(PCB_POINTER, V(0,int), V(1,int)); break;
    case 275: V(0,ParserInt32) = ag_rp_275(PCB_POINTER, V(0,int), V(1,int)); break;
    case 276: V(0,ParserInt32) = ag_rp_276(PCB_POINTER, V(0,int), V(1,int)); break;
    case 277: V(0,ParserTimeZone) = ag_rp_277(PCB_POINTER); break;
    case 278: V(0,ParserTimeZone) = ag_rp_278(PCB_POINTER); break;
    case 279: V(0,ParserTimeZone) = ag_rp_279(PCB_POINTER); break;
    case 280: ag_rp_280(PCB_POINTER, V(0,ParserMethod)); break;
    case 281: ag_rp_281(PCB_POINTER, V(0,ParserMethod), V(4,ParserBasicToken)); break;
    case 282: ag_rp_282(PCB_POINTER); break;
    case 283: ag_rp_283(PCB_POINTER); break;
    case 284: ag_rp_284(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 285: ag_rp_285(PCB_POINTER); break;
    case 286: ag_rp_286(PCB_POINTER); break;
    case 287: ag_rp_287(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 288: ag_rp_288(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 289: ag_rp_289(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 290: ag_rp_290(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 291: ag_rp_291(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 292: ag_rp_292(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 293: ag_rp_293(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 294: ag_rp_294(PCB_POINTER, V(0,RvSipAuthStale)); break;
    case 295: ag_rp_295(PCB_POINTER, V(0,ParserAlgorithm)); break;
    case 296: ag_rp_296(PCB_POINTER, V(0,ParserQopValue)); break;
    case 297: ag_rp_297(PCB_POINTER); break;
    case 298: V(0,ParserBasicToken) = ag_rp_298(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 299: V(0,ParserBasicToken) = ag_rp_299(PCB_POINTER, V(0,ParserQuotedString)); break;
    case 300: V(0,ParserBasicToken) = ag_rp_300(PCB_POINTER); break;
    case 301: V(0,ParserBasicToken) = ag_rp_301(PCB_POINTER); break;
    case 302: ag_rp_302(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 303: ag_rp_303(PCB_POINTER); break;
    case 304: ag_rp_304(PCB_POINTER); break;
    case 305: V(0,ParserBasicToken) = ag_rp_305(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 306: V(0,ParserBasicToken) = ag_rp_306(PCB_POINTER, V(0,ParserQuotedString)); break;
    case 307: V(0,ParserBasicToken) = ag_rp_307(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 308: V(0,ParserBasicToken) = ag_rp_308(PCB_POINTER, V(0,ParserQuotedString)); break;
    case 309: V(0,RvSipAuthStale) = ag_rp_309(PCB_POINTER); break;
    case 310: V(0,RvSipAuthStale) = ag_rp_310(PCB_POINTER); break;
    case 311: V(0,ParserAlgorithm) = ag_rp_311(PCB_POINTER, V(1,ParserAlgorithm)); break;
    case 312: ag_rp_312(PCB_POINTER); break;
    case 313: ag_rp_313(PCB_POINTER); break;
    case 314: ag_rp_314(PCB_POINTER); break;
    case 315: ag_rp_315(PCB_POINTER); break;
    case 316: V(0,ParserAlgorithm) = ag_rp_316(PCB_POINTER, V(2,ParserBasicToken), V(4,ParserAlgorithm)); break;
    case 317: V(0,ParserAlgorithm) = ag_rp_317(PCB_POINTER, V(1,ParserAlgorithm)); break;
    case 318: ag_rp_318(PCB_POINTER); break;
    case 319: V(0,ParserAlgorithm) = ag_rp_319(PCB_POINTER); break;
    case 320: V(0,ParserAlgorithm) = ag_rp_320(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 321: ag_rp_321(PCB_POINTER); break;
    case 322: ag_rp_322(PCB_POINTER); break;
    case 323: V(0,ParserBasicToken) = ag_rp_323(PCB_POINTER); break;
    case 324: V(0,ParserQopValue) = ag_rp_324(PCB_POINTER, V(5,ParserQopValue)); break;
    case 325: V(0,ParserQopValue) = ag_rp_325(PCB_POINTER, V(0,ParserAuthQopOptions)); break;
    case 326: V(0,ParserQopValue) = ag_rp_326(PCB_POINTER, V(0,ParserQopValue), V(4,ParserAuthQopOptions)); break;
    case 327: V(0,ParserAuthQopOptions) = ag_rp_327(PCB_POINTER); break;
    case 328: V(0,ParserAuthQopOptions) = ag_rp_328(PCB_POINTER); break;
    case 329: V(0,ParserAuthQopOptions) = ag_rp_329(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 330: V(0,ParserBasicToken) = ag_rp_330(PCB_POINTER, V(3,ParserBasicToken)); break;
    case 331: V(0,ParserBasicToken) = ag_rp_331(PCB_POINTER, V(3,ParserBasicToken)); break;
    case 332: V(0,ParserBasicToken) = ag_rp_332(PCB_POINTER); break;
    case 333: V(0,ParserBasicToken) = ag_rp_333(PCB_POINTER); break;
    case 334: V(0,ParserBasicToken) = ag_rp_334(PCB_POINTER); break;
    case 335: V(0,ParserBasicToken) = ag_rp_335(PCB_POINTER); break;
    case 336: V(0,ParserBasicToken) = ag_rp_336(PCB_POINTER); break;
    case 337: ag_rp_337(PCB_POINTER, V(0,ParserBasicToken), V(2,ParserGenericParamDataVal)); break;
    case 338: ag_rp_338(PCB_POINTER); break;
    case 339: ag_rp_339(PCB_POINTER); break;
    case 340: ag_rp_340(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 341: ag_rp_341(PCB_POINTER); break;
    case 342: ag_rp_342(PCB_POINTER); break;
    case 343: ag_rp_343(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 344: ag_rp_344(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 345: ag_rp_345(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 346: ag_rp_346(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 347: ag_rp_347(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 348: ag_rp_348(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 349: ag_rp_349(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 350: ag_rp_350(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 351: ag_rp_351(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 352: ag_rp_352(PCB_POINTER, V(0,RvSipAuthIntegrityProtected)); break;
    case 353: ag_rp_353(PCB_POINTER, V(0,RvSipAuthQopOption)); break;
    case 354: ag_rp_354(PCB_POINTER); break;
    case 355: ag_rp_355(PCB_POINTER, V(0,ParserAlgorithm)); break;
    case 356: V(0,ParserBasicToken) = ag_rp_356(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 357: V(0,ParserBasicToken) = ag_rp_357(PCB_POINTER, V(0,ParserQuotedString)); break;
    case 358: V(0,RvSipAuthQopOption) = ag_rp_358(PCB_POINTER, V(5,ParserAuthQopOptions)); break;
    case 359: V(0,ParserBasicToken) = ag_rp_359(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 360: V(0,ParserBasicToken) = ag_rp_360(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 361: V(0,ParserBasicToken) = ag_rp_361(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 362: V(0,ParserBasicToken) = ag_rp_362(PCB_POINTER); break;
    case 363: V(0,ParserBasicToken) = ag_rp_363(PCB_POINTER); break;
    case 364: V(0,ParserBasicToken) = ag_rp_364(PCB_POINTER); break;
    case 365: V(0,ParserBasicToken) = ag_rp_365(PCB_POINTER); break;
    case 366: V(0,ParserBasicToken) = ag_rp_366(PCB_POINTER); break;
    case 367: V(0,ParserBasicToken) = ag_rp_367(PCB_POINTER); break;
    case 368: V(0,ParserBasicToken) = ag_rp_368(PCB_POINTER); break;
    case 369: V(0,ParserBasicToken) = ag_rp_369(PCB_POINTER); break;
    case 370: V(0,ParserBasicToken) = ag_rp_370(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 371: V(0,ParserBasicToken) = ag_rp_371(PCB_POINTER); break;
    case 372: V(0,ParserBasicToken) = ag_rp_372(PCB_POINTER); break;
    case 373: V(0,ParserBasicToken) = ag_rp_373(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 374: V(0,ParserBasicToken) = ag_rp_374(PCB_POINTER); break;
    case 375: V(0,RvSipAuthIntegrityProtected) = ag_rp_375(PCB_POINTER); break;
    case 376: V(0,RvSipAuthIntegrityProtected) = ag_rp_376(PCB_POINTER); break;
    case 377: ag_rp_377(PCB_POINTER); break;
    case 378: ag_rp_378(PCB_POINTER); break;
    case 379: ag_rp_379(PCB_POINTER); break;
    case 380: ag_rp_380(PCB_POINTER); break;
    case 381: ag_rp_381(PCB_POINTER); break;
    case 382: ag_rp_382(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 383: ag_rp_383(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 384: ag_rp_384(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 385: ag_rp_385(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 386: ag_rp_386(PCB_POINTER); break;
    case 387: ag_rp_387(PCB_POINTER, V(5,ParserAuthQopOptions)); break;
    case 388: V(0,ParserBasicToken) = ag_rp_388(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 389: V(0,ParserBasicToken) = ag_rp_389(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 390: V(0,ParserBasicToken) = ag_rp_390(PCB_POINTER); break;
  }
  (PCB).la_ptr = (PCB).pointer;
}
static int ag_action_1_r_proc(PCB_DECL);
static int ag_action_2_r_proc(PCB_DECL);
static int ag_action_3_r_proc(PCB_DECL);
static int ag_action_4_r_proc(PCB_DECL);
static int ag_action_1_s_proc(PCB_DECL);
static int ag_action_3_s_proc(PCB_DECL);
static int ag_action_1_proc(PCB_DECL);
static int ag_action_2_proc(PCB_DECL);
static int ag_action_3_proc(PCB_DECL);
static int ag_action_4_proc(PCB_DECL);
static int ag_action_5_proc(PCB_DECL);
static int ag_action_6_proc(PCB_DECL);
static int ag_action_7_proc(PCB_DECL);
static int ag_action_8_proc(PCB_DECL);
static int ag_action_9_proc(PCB_DECL);
static int ag_action_10_proc(PCB_DECL);
static int ag_action_11_proc(PCB_DECL);
static int ag_action_8_proc(PCB_DECL);
static int (*const  ag_r_procs_scan[])(PCB_DECL) = {
  ag_action_1_r_proc,
  ag_action_2_r_proc,
  ag_action_3_r_proc,
  ag_action_4_r_proc
};
static int (*const  ag_s_procs_scan[])(PCB_DECL) = {
  ag_action_1_s_proc,
  ag_action_2_r_proc,
  ag_action_3_s_proc,
  ag_action_4_r_proc
};
static int (*const  ag_gt_procs_scan[])(PCB_DECL) = {
  ag_action_1_proc,
  ag_action_2_proc,
  ag_action_3_proc,
  ag_action_4_proc,
  ag_action_5_proc,
  ag_action_6_proc,
  ag_action_7_proc,
  ag_action_8_proc,
  ag_action_9_proc,
  ag_action_10_proc,
  ag_action_11_proc,
  ag_action_8_proc
};
static int ag_action_10_proc(PCB_DECL) {
  int ag_t = (PCB).token_number;
  do {
    ag_track(PCB_POINTER);
    (PCB).token_number = (SipParser_token_type) AG_TCV(INPUT_CODE(*(PCB).la_ptr));
    (PCB).la_ptr++;
    if (ag_key_index[(PCB).sn]) {
      unsigned ag_k = ag_key_index[(PCB).sn];
      int ag_ch = CONVERT_CASE(INPUT_CODE(*(PCB).pointer));
      while (ag_key_ch[ag_k] < ag_ch) ag_k++;
      if (ag_key_ch[ag_k] == ag_ch) ag_get_key_word((PCB_TYPE *)PCB_POINTER, ag_k);
    }
  } while ((PCB).token_number == (SipParser_token_type) ag_t);
  (PCB).la_ptr =  (PCB).pointer;
  return 1;
}
static int ag_action_11_proc(PCB_DECL) {
  int ag_t = (PCB).token_number;
  do {
    (*(int *) &(PCB).vs[(PCB).ssx]) = *(PCB).pointer;
    (PCB).ssx--;
    ag_track(PCB_POINTER);
    ag_ra(PCB_POINTER);
    if ((PCB).exit_flag != AG_RUNNING_CODE) return 0;
    (PCB).ssx++;
    (PCB).token_number = (SipParser_token_type) AG_TCV(INPUT_CODE(*(PCB).la_ptr));
    (PCB).la_ptr++;
    if (ag_key_index[(PCB).sn]) {
      unsigned ag_k = ag_key_index[(PCB).sn];
      int ag_ch = CONVERT_CASE(INPUT_CODE(*(PCB).pointer));
      while (ag_key_ch[ag_k] < ag_ch) ag_k++;
      if (ag_key_ch[ag_k] == ag_ch) ag_get_key_word((PCB_TYPE *)PCB_POINTER, ag_k);
    }
  }
  while ((PCB).token_number == (SipParser_token_type) ag_t);
  (PCB).la_ptr =  (PCB).pointer;
  return 1;
}
static int ag_action_3_r_proc(PCB_DECL) {
  int ag_sd = ag_fl[(PCB).ag_ap] - 1;
  if (ag_sd) (PCB).sn = (PCB).ss[(PCB).ssx -= ag_sd];
  (PCB).reduction_token = (SipParser_token_type) ag_ptt[(PCB).ag_ap];
  ag_ra(PCB_POINTER);
  return (PCB).exit_flag == AG_RUNNING_CODE;
}
static int ag_action_3_s_proc(PCB_DECL) {
  int ag_sd = ag_fl[(PCB).ag_ap] - 1;
  if (ag_sd) (PCB).sn = (PCB).ss[(PCB).ssx -= ag_sd];
  (PCB).reduction_token = (SipParser_token_type) ag_ptt[(PCB).ag_ap];
  ag_ra(PCB_POINTER);
  return (PCB).exit_flag == AG_RUNNING_CODE;;
}
static int ag_action_4_r_proc(PCB_DECL) {
  int ag_sd = ag_fl[(PCB).ag_ap] - 1;
  if (ag_sd) (PCB).sn = (PCB).ss[(PCB).ssx -= ag_sd];
  (PCB).reduction_token = (SipParser_token_type) ag_ptt[(PCB).ag_ap];
  return 1;
}
static int ag_action_2_proc(PCB_DECL) {
  if ((PCB).ssx >= 43) {
    (PCB).exit_flag = AG_STACK_ERROR_CODE;
    PARSER_STACK_OVERFLOW;
  }
  (*(int *) &(PCB).vs[(PCB).ssx]) = *(PCB).pointer;
  (PCB).ss[(PCB).ssx] = (PCB).sn;
  (PCB).ssx++;
  (PCB).sn = (PCB).ag_ap;
  ag_track(PCB_POINTER);
  return 0;
}
static int ag_action_9_proc(PCB_DECL) {
  if ((PCB).ssx >= 43) {
    (PCB).exit_flag = AG_STACK_ERROR_CODE;
    PARSER_STACK_OVERFLOW;
  }
  (PCB).vs[(PCB).ssx] = ag_null_value;
  (PCB).ss[(PCB).ssx] = (PCB).sn;
  (PCB).ssx++;
  (PCB).sn = (PCB).ag_ap;
  (PCB).la_ptr =  (PCB).pointer;
  return (PCB).exit_flag == AG_RUNNING_CODE;
}
static int ag_action_2_r_proc(PCB_DECL) {
  (PCB).ssx++;
  (PCB).sn = (PCB).ag_ap;
  return 0;
}
static int ag_action_7_proc(PCB_DECL) {
  --(PCB).ssx;
  (PCB).la_ptr =  (PCB).pointer;
  (PCB).exit_flag = AG_SUCCESS_CODE;
  return 0;
}
static int ag_action_1_proc(PCB_DECL) {
  ag_track(PCB_POINTER);
  (PCB).exit_flag = AG_SUCCESS_CODE;
  return 0;
}
static int ag_action_1_r_proc(PCB_DECL) {
  (PCB).exit_flag = AG_SUCCESS_CODE;
  return 0;
}
static int ag_action_1_s_proc(PCB_DECL) {
  (PCB).exit_flag = AG_SUCCESS_CODE;
  return 0;
}
static int ag_action_4_proc(PCB_DECL) {
  int ag_sd = ag_fl[(PCB).ag_ap] - 1;
  (PCB).reduction_token = (SipParser_token_type) ag_ptt[(PCB).ag_ap];
  (*(int *) &(PCB).vs[(PCB).ssx]) = *(PCB).pointer;
  if (ag_sd) (PCB).sn = (PCB).ss[(PCB).ssx -= ag_sd];
  else (PCB).ss[(PCB).ssx] = (PCB).sn;
  ag_track(PCB_POINTER);
  while ((PCB).exit_flag == AG_RUNNING_CODE) {
    unsigned ag_t1 = ag_sbe[(PCB).sn] + 1;
    unsigned ag_t2 = ag_sbt[(PCB).sn+1] - 1;
    do {
      unsigned ag_tx = (ag_t1 + ag_t2)/2;
      if (ag_tstt[ag_tx] < (const unsigned short)(PCB).reduction_token) ag_t1 = ag_tx + 1;
      else ag_t2 = ag_tx;
    } while (ag_t1 < ag_t2);
    (PCB).ag_ap = ag_pstt[ag_t1];
    if ((ag_s_procs_scan[ag_astt[ag_t1]])(PCB_POINTER) == 0) break;
  }
  return 0;
}
static int ag_action_3_proc(PCB_DECL) {
  int ag_sd = ag_fl[(PCB).ag_ap] - 1;
  (*(int *) &(PCB).vs[(PCB).ssx]) = *(PCB).pointer;
  if (ag_sd) (PCB).sn = (PCB).ss[(PCB).ssx -= ag_sd];
  else (PCB).ss[(PCB).ssx] = (PCB).sn;
  ag_track(PCB_POINTER);
  (PCB).reduction_token = (SipParser_token_type) ag_ptt[(PCB).ag_ap];
  ag_ra(PCB_POINTER);
  while ((PCB).exit_flag == AG_RUNNING_CODE) {
    unsigned ag_t1 = ag_sbe[(PCB).sn] + 1;
    unsigned ag_t2 = ag_sbt[(PCB).sn+1] - 1;
    do {
      unsigned ag_tx = (ag_t1 + ag_t2)/2;
      if (ag_tstt[ag_tx] < (const unsigned short)(PCB).reduction_token) ag_t1 = ag_tx + 1;
      else ag_t2 = ag_tx;
    } while (ag_t1 < ag_t2);
    (PCB).ag_ap = ag_pstt[ag_t1];
    if ((ag_s_procs_scan[ag_astt[ag_t1]])(PCB_POINTER) == 0) break;
  }
  return 0;
}
static int ag_action_8_proc(PCB_DECL) {
  (PCB).la_ptr =  (PCB).pointer;
  (PCB).exit_flag = AG_SYNTAX_ERROR_CODE;
  SYNTAX_ERROR;
  {(PCB).la_ptr = (PCB).pointer + 1; ag_track(PCB_POINTER);}
  return (PCB).exit_flag == AG_RUNNING_CODE;
}
static int ag_action_5_proc(PCB_DECL) {
  int ag_sd = ag_fl[(PCB).ag_ap];
  if (ag_sd) (PCB).sn = (PCB).ss[(PCB).ssx -= ag_sd];
  else {
    if ((PCB).ssx >= 43) {
      (PCB).exit_flag = AG_STACK_ERROR_CODE;
      PARSER_STACK_OVERFLOW;
    }
    (PCB).ss[(PCB).ssx] = (PCB).sn;
  }
  (PCB).la_ptr =  (PCB).pointer;
  (PCB).reduction_token = (SipParser_token_type) ag_ptt[(PCB).ag_ap];
  ag_ra(PCB_POINTER);
  while ((PCB).exit_flag == AG_RUNNING_CODE) {
    unsigned ag_t1 = ag_sbe[(PCB).sn] + 1;
    unsigned ag_t2 = ag_sbt[(PCB).sn+1] - 1;
    do {
      unsigned ag_tx = (ag_t1 + ag_t2)/2;
      if (ag_tstt[ag_tx] < (const unsigned short)(PCB).reduction_token) ag_t1 = ag_tx + 1;
      else ag_t2 = ag_tx;
    } while (ag_t1 < ag_t2);
    (PCB).ag_ap = ag_pstt[ag_t1];
    if ((ag_r_procs_scan[ag_astt[ag_t1]])(PCB_POINTER) == 0) break;
  }
  return (PCB).exit_flag == AG_RUNNING_CODE;
}
static int ag_action_6_proc(PCB_DECL) {
  int ag_sd = ag_fl[(PCB).ag_ap];
  (PCB).reduction_token = (SipParser_token_type) ag_ptt[(PCB).ag_ap];
  if (ag_sd) {
    (PCB).sn = (PCB).ss[(PCB).ssx -= ag_sd];
  }
  else {
    if ((PCB).ssx >= 43) {
      (PCB).exit_flag = AG_STACK_ERROR_CODE;
      PARSER_STACK_OVERFLOW;
    }
    (PCB).vs[(PCB).ssx] = ag_null_value;
    (PCB).ss[(PCB).ssx] = (PCB).sn;
  }
  (PCB).la_ptr =  (PCB).pointer;
  while ((PCB).exit_flag == AG_RUNNING_CODE) {
    unsigned ag_t1 = ag_sbe[(PCB).sn] + 1;
    unsigned ag_t2 = ag_sbt[(PCB).sn+1] - 1;
    do {
      unsigned ag_tx = (ag_t1 + ag_t2)/2;
      if (ag_tstt[ag_tx] < (const unsigned short)(PCB).reduction_token) ag_t1 = ag_tx + 1;
      else ag_t2 = ag_tx;
    } while (ag_t1 < ag_t2);
    (PCB).ag_ap = ag_pstt[ag_t1];
    if ((ag_r_procs_scan[ag_astt[ag_t1]])(PCB_POINTER) == 0) break;
  }
  return (PCB).exit_flag == AG_RUNNING_CODE;
}
void init_SipParser(SipParser_pcb_type *PCB_POINTER) {
  (PCB).la_ptr = (PCB).pointer;
  (PCB).error_message = "Syntax Error";
  (PCB).ss[0] = (PCB).sn = (PCB).ssx = 0;
  (PCB).exit_flag = AG_RUNNING_CODE;
  (PCB).line = FIRST_LINE;
  (PCB).column = FIRST_COLUMN;
}
void SipParser(SipParser_pcb_type *PCB_POINTER) {
  init_SipParser(PCB_POINTER);
  (PCB).exit_flag = AG_RUNNING_CODE;
  while ((PCB).exit_flag == AG_RUNNING_CODE) {
    unsigned ag_t1 = ag_sbt[(PCB).sn];
    if (ag_tstt[ag_t1]) {
      unsigned ag_t2 = ag_sbe[(PCB).sn] - 1;
      (PCB).token_number = (SipParser_token_type) AG_TCV(INPUT_CODE(*(PCB).la_ptr));
      (PCB).la_ptr++;
      if (ag_key_index[(PCB).sn]) {
        unsigned ag_k = ag_key_index[(PCB).sn];
        int ag_ch = CONVERT_CASE(INPUT_CODE(*(PCB).pointer));
        while (ag_key_ch[ag_k] < ag_ch) ag_k++;
        if (ag_key_ch[ag_k] == ag_ch) ag_get_key_word((PCB_TYPE *)PCB_POINTER, ag_k);
      }
      do {
        unsigned ag_tx = (ag_t1 + ag_t2)/2;
        if (ag_tstt[ag_tx] > (const unsigned short)(PCB).token_number)
          ag_t1 = ag_tx + 1;
        else ag_t2 = ag_tx;
      } while (ag_t1 < ag_t2);
      if (ag_tstt[ag_t1] != (const unsigned short)(PCB).token_number)
        ag_t1 = ag_sbe[(PCB).sn];
    }
    (PCB).ag_ap = ag_pstt[ag_t1];
    (ag_gt_procs_scan[ag_astt[ag_t1]])((PCB_TYPE *)PCB_POINTER);
  }
}
#endif
