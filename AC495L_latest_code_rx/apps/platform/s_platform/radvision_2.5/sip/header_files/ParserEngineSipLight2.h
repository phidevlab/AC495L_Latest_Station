#if defined(RV_SIP_LIGHT) && defined(RV_SIP_PRIMITIVES) && !defined(RV_SIP_IMS_DCS_HEADER_SUPPORT) && !defined(RV_SIP_IMS_HEADER_SUPPORT) && !defined(RV_SIP_EXTENDED_HEADER_SUPPORT) && !defined(RV_SIP_JSR32_SUPPORT) && !defined(RV_SIP_TEL_URI_SUPPORT) && !defined(RV_SIP_AUTH_ON) && !defined(RV_SIP_SUBS_ON)
/*
 AnaGram Parsing Engine
 Copyright (c) 1993-1999, Parsifal Software.
 All Rights Reserved.
 Serial number 2P20671
 Registered to:
   Meir Fuchs
   RADVision Ltd.
*/
#ifndef PARSERENGINESIPLIGHT_H_1165245721
#include ".\ParserEngineSipLight.h"
#endif
#ifndef PARSERENGINESIPLIGHT_H_1165245721
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
#define ag_rp_14(PCB_POINTER) (CUR_STRING())
#define ag_rp_15(PCB_POINTER) (CUR_STRING())
#define ag_rp_16(PCB_POINTER) (CUR_STRING())
#define ag_rp_17(PCB_POINTER) (CUR_STRING())
#define ag_rp_18(PCB_POINTER) (CUR_STRING())
#define ag_rp_19(PCB_POINTER) (CUR_METHOD(PARSER_METHOD_TYPE_INVITE))
#define ag_rp_20(PCB_POINTER) (CUR_METHOD(PARSER_METHOD_TYPE_ACK))
#define ag_rp_21(PCB_POINTER) (CUR_METHOD(PARSER_METHOD_TYPE_BYE))
#define ag_rp_22(PCB_POINTER) (CUR_METHOD(PARSER_METHOD_TYPE_CANCEL))
#define ag_rp_23(PCB_POINTER) (CUR_METHOD(PARSER_METHOD_TYPE_REGISTER))
static ParserMethod ag_rp_24(PCB_DECL, ParserBasicToken t) {
/* Line 490, RV SIP Stack */
 ParserMethod method;
                            method.type = PARSER_METHOD_TYPE_OTHER;
                            method.other = t;
                            RV_UNUSED_ARG(pcb_pointer);
                            return method;
                          
}
static void ag_rp_25(PCB_DECL, ParserContentLength contentLength) {
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
static ParserContentLength ag_rp_26(PCB_DECL) {
/* Line 572, RV SIP Stack */
        ParserContentLength contentLength;
        contentLength.contentLenVal = CUR_STRING();
        return contentLength;
    
}
static void ag_rp_27(PCB_DECL, ParserBasicToken token) {
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
static void ag_rp_28(PCB_DECL, ParserBasicToken token1, ParserGenericParamDataVal token2) {
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
#define ag_rp_29(PCB_POINTER, token) (token)
#define ag_rp_30(PCB_POINTER, qs) (qs)
static void ag_rp_31(PCB_DECL, ParserSipVersion ver, ParserStatusCode status, ParserReasonPhrase reason) {
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
static void ag_rp_32(PCB_DECL, ParserSipVersion ver, ParserStatusCode status) {
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
#define ag_rp_33(PCB_POINTER) (CUR_STRING())
#define ag_rp_34(PCB_POINTER) (CUR_STRING())
static ParserStatusCode ag_rp_35(PCB_DECL, int D100, int D10, int D1) {
/* Line 964, RV SIP Stack */
        RvUint16 code;
        RV_UNUSED_ARG(pcb_pointer);
        code =  (RvUint16)((D100 - '0')*100 + (D10 - '0')*10 + (D1 - '0'));
        return code;
    
}
static void ag_rp_36(PCB_DECL) {
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
static void ag_rp_37(PCB_DECL) {
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
static void ag_rp_38(PCB_DECL) {
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
static void ag_rp_39(PCB_DECL) {
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
static void ag_rp_40(PCB_DECL, ParserViaParam p) {
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
static void ag_rp_41(PCB_DECL, ParserProtocolName pn, ParserProtocolVersion pv, ParserTransport t) {
/* Line 1166, RV SIP Stack */
 
        PCB.sentProtocol.protocolName    = pn;
        PCB.sentProtocol.protocolVersion = pv;
        PCB.sentProtocol.transport       = t;
    
}
#define ag_rp_42(PCB_POINTER, t) (t)
#define ag_rp_43(PCB_POINTER) (CUR_STRING())
#define ag_rp_44(PCB_POINTER, t) (t)
#define ag_rp_45(PCB_POINTER, t) (t)
static ParserTransport ag_rp_46(PCB_DECL) {
/* Line 1193, RV SIP Stack */
        ParserTransport t;
        t.transport    = RVSIP_TRANSPORT_UDP;
        t.strTransport = CUR_STRING();
        return t;
    
}
static ParserTransport ag_rp_47(PCB_DECL) {
/* Line 1200, RV SIP Stack */
        ParserTransport t;
        t.transport    = RVSIP_TRANSPORT_TCP;
        t.strTransport = CUR_STRING();
        return t;
    
}
static ParserTransport ag_rp_48(PCB_DECL) {
/* Line 1207, RV SIP Stack */
        ParserTransport t;
        t.transport    = RVSIP_TRANSPORT_SCTP;
        t.strTransport = CUR_STRING();
        return t;
    
}
static ParserTransport ag_rp_49(PCB_DECL) {
/* Line 1214, RV SIP Stack */
        ParserTransport t;
        t.transport    = RVSIP_TRANSPORT_TLS;
        t.strTransport = CUR_STRING();
        return t;
    
}
static ParserTransport ag_rp_50(PCB_DECL, ParserBasicToken token) {
/* Line 1221, RV SIP Stack */
        ParserTransport t;
        t.transport    = RVSIP_TRANSPORT_OTHER;
        t.strTransport = token;
        RV_UNUSED_ARG(pcb_pointer);
        return t;
    
}
#define ag_rp_51(PCB_POINTER) (CUR_STRING())
static void ag_rp_52(PCB_DECL, ParserBasicToken host) {
/* Line 1240, RV SIP Stack */
 
        
        PCB.viaSentBy.isPort   = RV_FALSE;
        PCB.viaSentBy.hostName = host;
     
}
static void ag_rp_53(PCB_DECL, ParserBasicToken host, ParserPort port) {
/* Line 1246, RV SIP Stack */
 
        
        PCB.viaSentBy.isPort   = RV_TRUE;
        PCB.viaSentBy.hostName = host;
        PCB.viaSentBy.port     = port;
      
}
static ParserViaParam ag_rp_54(PCB_DECL) {
/* Line 1260, RV SIP Stack */
 ParserViaParam viaParam;
          viaParam.paramType = PARSER_PARAM_TYPE_HIDDEN;                   
          viaParam.param.hidden = RV_TRUE;
	      viaParam.isOther = RV_FALSE;
          viaParam.otherParam = NULL;
          RV_UNUSED_ARG(pcb_pointer);
          return viaParam;
        
}
static ParserViaParam ag_rp_55(PCB_DECL) {
/* Line 1269, RV SIP Stack */
 ParserViaParam viaParam;
          viaParam.paramType = PARSER_PARAM_TYPE_ALIAS;                   
          viaParam.param.alias = RV_TRUE;
	      viaParam.isOther = RV_FALSE;
          viaParam.otherParam = NULL;
          RV_UNUSED_ARG(pcb_pointer);
          return viaParam;
        
}
static ParserViaParam ag_rp_56(PCB_DECL, ParserTTLParam t) {
/* Line 1278, RV SIP Stack */
 ParserViaParam viaParam;
          viaParam.paramType = PARSER_PARAM_TYPE_TTL;                   
          viaParam.param.ttl = t;
	      viaParam.isOther = RV_FALSE;
          viaParam.otherParam = NULL;
          RV_UNUSED_ARG(pcb_pointer);
          return viaParam;
        
}
static ParserViaParam ag_rp_57(PCB_DECL, ParserMaddrParam ma) {
/* Line 1287, RV SIP Stack */
 ParserViaParam viaParam;
          viaParam.paramType = PARSER_PARAM_TYPE_MADDR;                   
          viaParam.param.maddr = ma;
	      viaParam.isOther = RV_FALSE;
          viaParam.otherParam = NULL;
          RV_UNUSED_ARG(pcb_pointer);
          return viaParam;
        
}
static ParserViaParam ag_rp_58(PCB_DECL, ParserReceived r) {
/* Line 1296, RV SIP Stack */
 ParserViaParam viaParam;
          viaParam.paramType = PARSER_PARAM_TYPE_RECEIVED;                   
          viaParam.param.received = r;
	      viaParam.isOther = RV_FALSE;
          viaParam.otherParam = NULL;
          RV_UNUSED_ARG(pcb_pointer);
          return viaParam;
        
}
static ParserViaParam ag_rp_59(PCB_DECL, ParserBranch b) {
/* Line 1305, RV SIP Stack */
 ParserViaParam viaParam;
          viaParam.paramType = PARSER_PARAM_TYPE_BRANCH;                   
          viaParam.param.branch = b;
	      viaParam.isOther = RV_FALSE;
          viaParam.otherParam = NULL;
          RV_UNUSED_ARG(pcb_pointer);
          return viaParam;
        
}
static ParserViaParam ag_rp_60(PCB_DECL, ParserPort rport) {
/* Line 1315, RV SIP Stack */
          ParserViaParam viaParam;
          viaParam.paramType = PARSER_PARAM_TYPE_RPORT;                   
          viaParam.param.rport = rport;
	      viaParam.isOther = RV_FALSE;
          viaParam.otherParam = NULL;
          RV_UNUSED_ARG(pcb_pointer);
          return viaParam;
        
}
static ParserViaParam ag_rp_61(PCB_DECL, ParserCompParam comp) {
/* Line 1324, RV SIP Stack */
 ParserViaParam viaParam;
          viaParam.paramType  = PARSER_PARAM_TYPE_COMP;                   
          viaParam.param.comp = comp;
	      viaParam.isOther    = RV_FALSE;
          viaParam.otherParam = NULL;
          RV_UNUSED_ARG(pcb_pointer);
          return viaParam;
        
}
static ParserViaParam ag_rp_62(PCB_DECL) {
/* Line 1333, RV SIP Stack */
 ParserViaParam viaParam;
          viaParam.paramType = PARSER_PARAM_TYPE_OTHER; 
	      viaParam.isOther = RV_TRUE;
          viaParam.otherParam = PCB.pExtParams;
          return viaParam;    
        
}
static void ag_rp_63(PCB_DECL, ParserBasicToken token) {
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
static void ag_rp_64(PCB_DECL, ParserBasicToken token, ParserBasicToken token2) {
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
static void ag_rp_65(PCB_DECL, ParserBasicToken token, ParserQuotedString token2) {
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
static void ag_rp_66(PCB_DECL, ParserBasicToken token, ParserBasicToken token2) {
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
#define ag_rp_67(PCB_POINTER, t) (t)
#define ag_rp_68(PCB_POINTER, ttl) (ttl)
#define ag_rp_69(PCB_POINTER) (CUR_STRING())
#define ag_rp_70(PCB_POINTER) (CUR_STRING())
#define ag_rp_71(PCB_POINTER) (CUR_STRING())
#define ag_rp_72(PCB_POINTER, rport) (rport)
static ParserPort ag_rp_73(PCB_DECL) {
/* Line 1462, RV SIP Stack */
        ParserPort port;
        port.len = 0;
		port.buf = NULL;
        RV_UNUSED_ARG(pcb_pointer);
        return port;
    
}
#define ag_rp_74(PCB_POINTER, port) (port)
#define ag_rp_75(PCB_POINTER, ma) (ma)
#define ag_rp_76(PCB_POINTER, host) (host)
#define ag_rp_77(PCB_POINTER, host) (host)
#define ag_rp_78(PCB_POINTER, t) (t)
#define ag_rp_79(PCB_POINTER, comp) (comp)
static ParserCompParam ag_rp_80(PCB_DECL) {
/* Line 1499, RV SIP Stack */
        ParserCompParam comp;
        comp.compType     = RVSIP_COMP_SIGCOMP;
        comp.strCompParam = CUR_STRING();
        return comp;
    
}
static ParserCompParam ag_rp_81(PCB_DECL, ParserBasicToken token) {
/* Line 1506, RV SIP Stack */
        ParserCompParam comp;
        comp.compType     = RVSIP_COMP_OTHER;
        comp.strCompParam = CUR_STRING();
        RV_UNUSED_ARG(token.buf);
        return comp;
    
}
#define ag_rp_82(PCB_POINTER) (CUR_STRING())
static void ag_rp_83(PCB_DECL, ParserAbsoluteUri uri) {
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
static ParserAbsoluteUri ag_rp_84(PCB_DECL, ParserBasicToken scheme, ParserBasicToken id) {
/* Line 1757, RV SIP Stack */
        ParserAbsoluteUri uri;
        uri.scheme = scheme;
        uri.idefntifier = id;
        RV_UNUSED_ARG(pcb_pointer);
        return uri;
    
}
#define ag_rp_85(PCB_POINTER) (CUR_STRING())
#define ag_rp_86(PCB_POINTER, displayName) (displayName)
#define ag_rp_87(PCB_POINTER) (CUR_STRING())
#define ag_rp_88(PCB_POINTER) (CUR_STRING())
static void ag_rp_89(PCB_DECL) {
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
#define ag_rp_90(PCB_POINTER, uri) (INIT_PRES_IM_STRUCT(uri))
#define ag_rp_91(PCB_POINTER, uri) (INIT_PRES_IM_STRUCT(uri))
static void ag_rp_92(PCB_DECL) {
/* Line 2209, RV SIP Stack */
        PCB.sipUrl.bIsSecure = RV_FALSE;
        SIP_BEGIN_TOKEN();
        DEFINE_URL_PREFIX();
    
}
static void ag_rp_93(PCB_DECL) {
/* Line 2215, RV SIP Stack */
        PCB.sipUrl.bIsSecure = RV_TRUE;
        SIP_BEGIN_TOKEN();
        DEFINE_URL_PREFIX();
    
}
#define ag_rp_94(PCB_POINTER, uri) (uri)
static ParserAbsoluteUri ag_rp_95(PCB_DECL) {
/* Line 2227, RV SIP Stack */
        ParserAbsoluteUri uri;
        
        uri.scheme.len = 0;
		uri.scheme.buf = NULL;
        uri.idefntifier.len = 0;
		uri.idefntifier.buf = NULL;
        RV_UNUSED_ARG(pcb_pointer)
        return uri;
    
}
#define ag_rp_96(PCB_POINTER) (SET_PARSER_PCB_BACK_TO_BUFF(RV_SIPPARSER_PREFIX_RV_ABS))
static void ag_rp_97(PCB_DECL) {
/* Line 2243, RV SIP Stack */
        SIP_BEGIN_TOKEN();
        DEFINE_PRES_IM_PREFIX(RV_FALSE);
    
}
#define ag_rp_98(PCB_POINTER, uri) (uri)
static ParserAbsoluteUri ag_rp_99(PCB_DECL) {
/* Line 2253, RV SIP Stack */
        ParserAbsoluteUri uri;
        
        uri.scheme.len = 0;
        uri.scheme.buf = NULL;
        uri.idefntifier.len = 0;
		uri.idefntifier.buf = NULL;
        RV_UNUSED_ARG(pcb_pointer)
        return uri;
    
}
static void ag_rp_100(PCB_DECL) {
/* Line 2266, RV SIP Stack */
        SIP_BEGIN_TOKEN();
        DEFINE_PRES_IM_PREFIX(RV_TRUE);
    
}
static void ag_rp_101(PCB_DECL, ParserOptionalHeaders hs) {
/* Line 2274, RV SIP Stack */
  
           
         PCB.sipUrl.optionalHeaders = hs;
         
         if (0 != ((ParserExtensionString *)PCB.pUrlExtParams)->size)
         {
              PCB.sipUrl.urlParameters.genericParamList= PCB.pUrlExtParams;
              PCB.sipUrl.urlParameters.isOtherParams   = RV_TRUE;
         }          
  	
}
static void ag_rp_102(PCB_DECL, ParserOptionalHeaders hs) {
/* Line 2285, RV SIP Stack */
  
         
       PCB.sipUrl.optionalHeaders = hs;
       
       if (0 != ((ParserExtensionString *)PCB.pUrlExtParams)->size)
       {
            PCB.sipUrl.urlParameters.genericParamList=PCB.pUrlExtParams;
            PCB.sipUrl.urlParameters.isOtherParams   = RV_TRUE;
       }          
	
}
static void ag_rp_103(PCB_DECL, ParserOptionalHeaders hs) {
/* Line 2315, RV SIP Stack */
 
        RESET_URI_PARAMS();
        PCB.sipUrl.optionalHeaders.isSpecified = RV_FALSE;
        PCB.sipUrl.optionalHeaders = hs;
        PCB.exUri.uriType = PARSER_ADDR_PARAM_TYPE_SIP_URL;
        PCB.exUri.ExUriInfo.SipUrl = PCB.sipUrl;
      
}
static void ag_rp_104(PCB_DECL, ParserOptionalHeaders hs) {
/* Line 2324, RV SIP Stack */
 
        RESET_URI_PARAMS();
        PCB.sipUrl.optionalHeaders.isSpecified = RV_FALSE;
        PCB.sipUrl.optionalHeaders = hs;
        
        PCB.exUri.uriType = PARSER_ADDR_PARAM_TYPE_SIP_URL;
        PCB.exUri.ExUriInfo.SipUrl = PCB.sipUrl;
      
}
static void ag_rp_105(PCB_DECL, ParserOptionalHeaders hs) {
/* Line 2351, RV SIP Stack */
 
        RESET_URI_PARAMS();
        PCB.sipUrl.optionalHeaders.isSpecified = RV_FALSE;
        PCB.sipUrl.optionalHeaders = hs;
        PCB.exUri.uriType = PARSER_ADDR_PARAM_TYPE_PRES_URI;
        PCB.exUri.ExUriInfo.SipUrl = PCB.sipUrl;
      
}
static void ag_rp_106(PCB_DECL, ParserOptionalHeaders hs) {
/* Line 2360, RV SIP Stack */
 
        RESET_URI_PARAMS();
        PCB.sipUrl.optionalHeaders.isSpecified = RV_FALSE;
        PCB.sipUrl.optionalHeaders = hs;
        
        PCB.exUri.uriType = PARSER_ADDR_PARAM_TYPE_PRES_URI;
        PCB.exUri.ExUriInfo.SipUrl = PCB.sipUrl;
      
}
static void ag_rp_107(PCB_DECL, ParserOptionalHeaders hs) {
/* Line 2372, RV SIP Stack */
 
        RESET_URI_PARAMS();
        PCB.sipUrl.optionalHeaders.isSpecified = RV_FALSE;
        PCB.sipUrl.optionalHeaders = hs;
        PCB.exUri.uriType = PARSER_ADDR_PARAM_TYPE_IM_URI;
        PCB.exUri.ExUriInfo.SipUrl = PCB.sipUrl;
      
}
static void ag_rp_108(PCB_DECL, ParserOptionalHeaders hs) {
/* Line 2381, RV SIP Stack */
 
        RESET_URI_PARAMS();
        PCB.sipUrl.optionalHeaders.isSpecified = RV_FALSE;
        PCB.sipUrl.optionalHeaders = hs;
        
        PCB.exUri.uriType = PARSER_ADDR_PARAM_TYPE_IM_URI;
        PCB.exUri.ExUriInfo.SipUrl = PCB.sipUrl;
      
}
static void ag_rp_109(PCB_DECL, ParserDisplayName scheme, ParserBasicToken id) {
/* Line 2395, RV SIP Stack */
          PCB.exUri.ExUriInfo.absUri.scheme = scheme;
          PCB.exUri.ExUriInfo.absUri.idefntifier = id;
          PCB.exUri.uriType = PARSER_ADDR_PARAM_TYPE_ABS_URI;
      
}
static void ag_rp_110(PCB_DECL) {
/* Line 2405, RV SIP Stack */
        PCB.sipUrl.bIsUserInfo = RV_TRUE;
    
}
static void ag_rp_111(PCB_DECL) {
/* Line 2410, RV SIP Stack */
        PCB.sipUrl.bIsUserInfo = RV_FALSE;
    
}
static void ag_rp_112(PCB_DECL) {
/* Line 2417, RV SIP Stack */
        SET_PARSER_PCB_BACK_TO_BUFF(RV_SIPPARSER_PREFIX_USERINFO);
    
}
static void ag_rp_113(PCB_DECL, ParserBasicToken user) {
/* Line 2423, RV SIP Stack */
        PCB.sipUrl.urlUserInfo.userName   = user;
        PCB.sipUrl.urlUserInfo.isPassword = RV_FALSE;
    
}
static void ag_rp_114(PCB_DECL, ParserBasicToken user, ParserBasicToken pass) {
/* Line 2428, RV SIP Stack */
        PCB.sipUrl.urlUserInfo.userName   = user;
        PCB.sipUrl.urlUserInfo.isPassword = RV_TRUE;
        PCB.sipUrl.urlUserInfo.password   = pass;
     
}
#define ag_rp_115(PCB_POINTER, t) (t)
#define ag_rp_116(PCB_POINTER) (CUR_STRING())
#define ag_rp_117(PCB_POINTER) (CUR_STRING())
static void ag_rp_118(PCB_DECL) {
/* Line 2461, RV SIP Stack */
        PCB.sipUrl.bIsUserInfo = RV_FALSE;
    
}
static void ag_rp_119(PCB_DECL) {
/* Line 2467, RV SIP Stack */
        SET_PARSER_PCB_BACK_TO_BUFF(RV_SIPPARSER_PREFIX_HOST);
    
}
static void ag_rp_120(PCB_DECL, ParserBasicToken host) {
/* Line 2475, RV SIP Stack */
        PCB.sipUrl.urlHost.hostName = host;
        PCB.sipUrl.urlHost.isPort   = RV_FALSE;
    
}
static void ag_rp_121(PCB_DECL, ParserBasicToken host, ParserPort port) {
/* Line 2481, RV SIP Stack */
        PCB.sipUrl.urlHost.hostName = host;
        PCB.sipUrl.urlHost.isPort   = RV_TRUE;
        PCB.sipUrl.urlHost.port     = port;
    
}
#define ag_rp_122(PCB_POINTER, t) (t)
#define ag_rp_123(PCB_POINTER, ip6ref) (ip6ref)
#define ag_rp_124(PCB_POINTER) (CUR_STRING())
#define ag_rp_125(PCB_POINTER) (CUR_STRING())
#define ag_rp_126(PCB_POINTER) (CUR_STRING())
#define ag_rp_127(PCB_POINTER) (RESET_URI_PARAMS())
static void ag_rp_128(PCB_DECL) {
/* Line 2535, RV SIP Stack */
	
       PCB.sipUrl.urlParameters.isValid = RV_TRUE;
    
}
static void ag_rp_129(PCB_DECL) {
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
static void ag_rp_130(PCB_DECL) {
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
static void ag_rp_131(PCB_DECL, ParserTransport t) {
/* Line 2645, RV SIP Stack */
 
         PCB.urlParameter.paramType = PARSER_TRANSPORT_PARAM;
         PCB.urlParameter.param.transport = t;
    
}
static void ag_rp_132(PCB_DECL, ParserUserParam u) {
/* Line 2650, RV SIP Stack */
 
	     PCB.urlParameter.paramType = PARSER_USER_PARAM;
		 PCB.urlParameter.param.userParam = u;
	
}
static void ag_rp_133(PCB_DECL, ParserTtlParam t) {
/* Line 2655, RV SIP Stack */
 
	     PCB.urlParameter.paramType = PARSER_TTL_PARAM;
		 PCB.urlParameter.param.ttlParam = t;
	
}
static void ag_rp_134(PCB_DECL, ParserMaddrParam ma) {
/* Line 2660, RV SIP Stack */
 
	     PCB.urlParameter.paramType = PARSER_MADDR_PARAM;
		 PCB.urlParameter.param.maddrParam = ma;
	
}
static void ag_rp_135(PCB_DECL, ParserMethod met) {
/* Line 2666, RV SIP Stack */
	     PCB.urlParameter.paramType = PARSER_METHOD_PARAM;
		 PCB.urlParameter.param.methodParam =met;
    
}
static void ag_rp_136(PCB_DECL, ParserLrParamType lrParam) {
/* Line 2671, RV SIP Stack */
	     PCB.urlParameter.paramType = PARSER_LR_PARAM;
		 PCB.urlParameter.param.lrParamType   = lrParam;
    
}
static void ag_rp_137(PCB_DECL, ParserCompParam comp) {
/* Line 2676, RV SIP Stack */
        PCB.urlParameter.paramType       = PARSER_COMP_PARAM;
        PCB.urlParameter.param.compParam = comp;
    
}
static void ag_rp_138(PCB_DECL, ParserTokenizedByParam tokenizedBy) {
/* Line 2681, RV SIP Stack */
        PCB.urlParameter.paramType       = PARSER_TOKENIZED_BY_PARAM;
        PCB.urlParameter.param.tokenizedByParam = tokenizedBy;
    
}
static void ag_rp_139(PCB_DECL, RvBool orig) {
/* Line 2686, RV SIP Stack */
        PCB.urlParameter.paramType       = PARSER_ORIG_PARAM;
        PCB.urlParameter.param.bOrigParam = orig;
    
}
static void ag_rp_140(PCB_DECL) {
/* Line 2696, RV SIP Stack */
	     PCB.urlParameter.paramType = PARSER_OTHER_PARAM;
    
}
#define ag_rp_141(PCB_POINTER, t) (t)
static ParserUserParam ag_rp_142(PCB_DECL, RvSipUserParam pt) {
/* Line 2711, RV SIP Stack */
 
        ParserUserParam userParam;
	    userParam.paramType = pt;
		userParam.otherUser.buf = NULL;
		userParam.otherUser.len = 0;
        RV_UNUSED_ARG(pcb_pointer);
	    return userParam;
    
}
static ParserUserParam ag_rp_143(PCB_DECL, ParserOtherUser ou) {
/* Line 2720, RV SIP Stack */
 
        ParserUserParam userParam;
        userParam.paramType = RVSIP_USERPARAM_OTHER;
	    userParam.otherUser = ou;
        RV_UNUSED_ARG(pcb_pointer);
	    return userParam;
    
}
#define ag_rp_144(PCB_POINTER) (RVSIP_USERPARAM_PHONE)
#define ag_rp_145(PCB_POINTER) (RVSIP_USERPARAM_IP)
#define ag_rp_146(PCB_POINTER, t) (t)
#define ag_rp_147(PCB_POINTER, ttl) (ttl)
#define ag_rp_148(PCB_POINTER, host) (host)
#define ag_rp_149(PCB_POINTER, met) (met)
#define ag_rp_150(PCB_POINTER) (ParserLrParam1)
#define ag_rp_151(PCB_POINTER) (ParserLrParamTrue)
#define ag_rp_152(PCB_POINTER) (ParserLrParamOn)
#define ag_rp_153(PCB_POINTER) (ParserLrParamEmpty)
#define ag_rp_154(PCB_POINTER, comp) (comp)
#define ag_rp_155(PCB_POINTER, hostname) (hostname)
#define ag_rp_156(PCB_POINTER) (RV_TRUE)
static void ag_rp_157(PCB_DECL, ParserBasicToken token) {
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
static void ag_rp_158(PCB_DECL, ParserBasicToken token1, ParserBasicToken token2) {
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
#define ag_rp_159(PCB_POINTER) (CUR_STRING())
#define ag_rp_160(PCB_POINTER, token) (token)
static ParserOptionalHeaders ag_rp_161(PCB_DECL) {
/* Line 2849, RV SIP Stack */
 
        ParserOptionalHeaders optionalHeaders;
        optionalHeaders.isSpecified = RV_FALSE;
		optionalHeaders.body.buf = NULL;
		optionalHeaders.body.len = 0;
        RV_UNUSED_ARG(pcb_pointer);
        return optionalHeaders;
    
}
static ParserOptionalHeaders ag_rp_162(PCB_DECL, ParserBasicToken headersList) {
/* Line 2858, RV SIP Stack */
 
        ParserOptionalHeaders optionalHeaders;
        optionalHeaders.isSpecified = RV_TRUE;
        optionalHeaders.body=headersList;
        RV_UNUSED_ARG(pcb_pointer);
        return optionalHeaders;
    
}
#define ag_rp_163(PCB_POINTER) (CUR_STRING())
#define ag_rp_164(PCB_POINTER) (CUR_STRING())
#define ag_rp_165(PCB_POINTER) (CUR_STRING())
#define ag_rp_166(PCB_POINTER) (CUR_STRING())
#define ag_rp_167(PCB_POINTER) (CUR_STRING())
static void ag_rp_168(PCB_DECL) {
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
static void ag_rp_169(PCB_DECL, ParserMethod met, ParserSipVersion ver) {
/* Line 2919, RV SIP Stack */
         PCB.requestLine.method = met;
         PCB.requestLine.sipVer = ver;
		 PCB.requestLine.exUri = PCB.exUri;         
    
}
#define ag_rp_170(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("m"))
#define ag_rp_171(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("i"))
#define ag_rp_172(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("c"))
#define ag_rp_173(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("v"))
#define ag_rp_174(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("t"))
#define ag_rp_175(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("f"))
#define ag_rp_176(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("r"))
#define ag_rp_177(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("b"))
#define ag_rp_178(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("k"))
#define ag_rp_179(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("x"))
#define ag_rp_180(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("o"))
#define ag_rp_181(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("u"))
#define ag_rp_182(PCB_POINTER) (CUR_REQUEST_COMPACT_METHOD("l"))
#define ag_rp_183(PCB_POINTER, met) (met)
#define ag_rp_184(PCB_POINTER) (SET_URI_TYPE(PARSER_ADDR_PARAM_TYPE_SIP_URL, PCB.exUri.ExUriInfo.absUri))
#define ag_rp_185(PCB_POINTER, uri) (SET_URI_TYPE(PARSER_ADDR_PARAM_TYPE_PRES_URI, uri))
#define ag_rp_186(PCB_POINTER, uri) (SET_URI_TYPE(PARSER_ADDR_PARAM_TYPE_IM_URI, uri))
#define ag_rp_187(PCB_POINTER, uri) (SET_URI_TYPE(PARSER_ADDR_PARAM_TYPE_ABS_URI, uri))
static void ag_rp_188(PCB_DECL, ParserDisplayName name) {
/* Line 3040, RV SIP Stack */
 
		PCB.nameAddr.exUri = PCB.exUri;    
        PCB.nameAddr.isDisplayName = RV_TRUE;  
	    PCB.nameAddr.name = name;
	
}
static void ag_rp_189(PCB_DECL) {
/* Line 3047, RV SIP Stack */
 
		PCB.nameAddr.exUri = PCB.exUri;
        PCB.nameAddr.isDisplayName = RV_FALSE;  
    
}
#define ag_rp_190(PCB_POINTER, qs) (qs)
#define ag_rp_191(PCB_POINTER, mtot) (mtot)
#define ag_rp_192(PCB_POINTER) (CUR_STRING())
#define ag_rp_193(PCB_POINTER) (CUR_STRING())
#define ag_rp_194(PCB_POINTER, t) (t)
#define ag_rp_195(PCB_POINTER, ppd) (ppd)
static ParserPartyParams ag_rp_196(PCB_DECL, ParserPartyParams ppl, ParserPartyParams ppd) {
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
static ParserPartyParams ag_rp_197(PCB_DECL, ParserTagParam tag) {
/* Line 3097, RV SIP Stack */
        ParserPartyParams partyParams;
        partyParams.isTag = RV_TRUE;
        partyParams.tag = tag;
        partyParams.isAddrParams = RV_FALSE;
        partyParams.addrParams = NULL;
        RV_UNUSED_ARG(pcb_pointer);
        return partyParams; 
    
}
static ParserPartyParams ag_rp_198(PCB_DECL) {
/* Line 3107, RV SIP Stack */
        ParserPartyParams partyParams;
        partyParams.isAddrParams = RV_TRUE;
        partyParams.isTag = RV_FALSE;
        partyParams.addrParams = PCB.pExtParams;
		partyParams.tag.buf = NULL;
		partyParams.tag.len = 0;
        return partyParams; 
    
}
static void ag_rp_199(PCB_DECL, ParserMethod method) {
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
static void ag_rp_200(PCB_DECL, ParserMethod method, ParserBasicToken fv) {
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
static void ag_rp_201(PCB_DECL) {
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
static void ag_rp_202(PCB_DECL) {
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
static void ag_rp_203(PCB_DECL) {
/* Line 4713, RV SIP Stack */
        PCB.route.nameAddr = PCB.nameAddr;
        PCB.route.isParams = RV_FALSE;
        PCB.route.eRouteType = PCB.eWhichHeader;
    
}
static void ag_rp_204(PCB_DECL) {
/* Line 4718, RV SIP Stack */
 
        PCB.route.nameAddr = PCB.nameAddr;
        PCB.route.isParams = RV_TRUE;
        PCB.route.eRouteType = PCB.eWhichHeader;
          
         if (((ParserExtensionString *)PCB.pExtParams)->size !=0)
         {
             PCB.route.routeParams = PCB.pExtParams;
         }                
     
}
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
static const unsigned char ag_rpx[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  0,  0,
    5,  6,  7,  8,  9, 10, 11, 12, 13,  0,  0, 14,  0,  0, 15,  0,  0, 16,
    0,  0,  0,  0,  0, 17,  0,  0,  0,  0,  0, 18,  0,  0, 19, 20, 21, 22,
   23, 24, 25,  0,  0, 26,  0,  0, 27, 28, 29, 30,  0,  0, 31, 32, 33,  0,
    0,  0,  0,  0,  0, 34, 35,  0, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45,
   46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62,  0,
   63, 64, 65, 66,  0,  0, 67, 68, 69, 70, 71, 72,  0, 73, 74, 75, 76, 77,
   78, 79, 80, 81, 82,  0, 83, 84,  0,  0, 85, 86,  0,  0, 87,  0,  0, 88,
   89, 90, 91,  0, 92, 93, 94, 95, 96, 97, 98, 99,100,101,102,  0,  0,  0,
    0,103,104,105,106,  0,107,108,  0,109,110,111,112,113,114,115,  0,  0,
  116,117,  0,  0,  0,  0,  0,  0,118,119,120,121,122,123,124,  0,  0,125,
  126,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,127,128,129,130,131,132,133,134,135,136,137,138,139,140,
  141,142,143,144,145,146,147,148,149,150,151,152,  0,  0,  0,  0,153,154,
  155,156,157,158,  0,  0,  0,  0,  0,159,160,161,162,163,164,165,  0,  0,
  166,  0,  0,  0,  0,  0,167,168,169,170,171,172,173,174,175,176,177,178,
  179,180,181,182,183,184,185,186,187,188,189,190,191,  0,  0,  0,  0,  0,
  192,193,194,195,196,197,198,199,200,  0,201,202,203,204
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
  0,  2,  0,  6,  0, 10,  0, 65,  0, 66,  0, 67,  0, 68,  0, 69,
  0, 96,  0, 98,  0, 99,  0,100,  0,101,  0,115,  0,116,  0,118,
  0,122,  0,124,  0,125,  0,127,  0,128,  0,130,  0,214,  0,215,
  0,218,  0,219,  0,220,  0,221,  0,222,  0,223,  0,224,  0,227,
  0,228,  0,248,  0,249,  0,250,  0,251,  0,252,  0,253,  0,254,
  0,255,  0,256,  0,257,  0,258,  0,259,  0,260,  0,268,0
};
static const unsigned char ag_key_ch[] = {
    0, 89,255, 65,255, 77, 78,255, 69, 72,255, 47, 58, 83,255, 80,255, 73,
  255, 82,255, 73,255, 65, 66, 67, 70, 73, 75, 76, 77, 79, 80, 82, 83, 84,
   85, 86, 88,255, 72, 85,255, 72, 82, 85,255, 67, 76,255, 82, 89,255, 65,
   79,255, 77, 78, 80,255, 65, 69,255, 72, 82,255, 67, 71,255, 69, 80,255,
   58, 83,255, 71, 80,255, 67, 73,255, 67, 76, 79, 82, 84,255, 68, 83,255,
   58, 65, 66, 67, 72, 73, 76, 77, 79, 80, 82, 83, 84, 85,255, 58, 83,255,
   80,255, 73,255, 73, 80, 83,255, 83,255, 58,255, 65, 69,255, 79, 82, 84,
  255, 67, 76, 77, 79, 84, 85,255, 83,255, 49, 79, 84,255, 83,255, 65, 66,
   67, 73, 82,255, 73, 80,255, 67, 76,255, 83, 84, 85,255, 69, 80,255, 65,
   66, 67, 72, 77, 82, 84,255
};
static const unsigned char ag_key_act[] = {
  0,7,4,7,4,3,7,4,7,5,4,3,0,3,4,2,4,2,4,5,4,5,4,7,6,6,5,6,5,5,5,5,3,6,2,
  5,6,6,5,4,3,3,4,3,3,3,4,7,7,4,7,7,4,7,7,4,3,7,5,4,7,7,4,7,3,4,7,7,4,2,
  7,4,0,3,4,7,6,4,7,2,4,7,7,7,7,7,4,7,7,4,3,2,2,2,7,2,7,2,7,2,2,2,2,2,4,
  0,3,4,2,4,2,4,3,3,2,4,7,4,3,4,7,7,4,7,7,7,4,7,7,2,7,2,7,4,3,4,5,7,7,4,
  7,4,7,7,7,7,7,4,7,7,4,7,7,4,7,2,7,4,7,7,4,7,7,7,7,7,2,7,4
};
static const unsigned short ag_key_parm[] = {
    0, 10,  0, 12,  0,158,  6,  0, 14,  2,  0, 81,151,152,  0,  0,  0,  0,
    0,  4,  0,  0,  0,  8, 80, 70, 76, 68, 82, 90, 66, 86,156, 78,  0, 74,
   88, 72, 84,  0,183,171,  0,183,155,171,  0,  8, 28,  0, 38, 10,  0, 12,
   40,  0,158,  6, 50,  0, 34, 52,  0, 48,156,  0, 36, 14,  0,  0, 32,  0,
  151,152,  0, 42, 16,  0, 22,  0,  0, 20, 24, 62, 44, 30,  0, 18, 46,  0,
  195,  0,  0,  0, 26,  0, 54,  0, 64,  0,  0,  0,  0,  0,  0,151,152,  0,
    0,  0,  0,  0,158,156,  0,  0, 16,  0,195,  0, 34, 52,  0, 62, 44, 30,
    0, 40, 54,  0, 64,  0, 46,  0, 81,  0, 56, 60, 58,  0, 42,  0,  8, 10,
   12,  6, 14,  0, 50, 48,  0, 20, 24,  0, 22,  0, 18,  0, 36, 32,  0, 28,
   38, 40, 26, 34,  0, 30,  0
};
static const unsigned short ag_key_jmp[] = {
    0,  3,  0,  5,  0, 10, 12,  0, 22,  0,  0, 29,  0, 33,  0, 11,  0, 15,
    0,  0,  0,  0,  0,  0,  1,  3,  0,  5,  0,  0,  0,  0, 17,  8, 17,  0,
   19, 21,  0,  0, 35, 40,  0, 49, 54, 61,  0, 72, 74,  0, 78, 83,  0, 85,
   90,  0, 99,101,  0,  0,108,112,  0,121,125,  0,129,135,  0, 66,141,  0,
    0,153,  0,148, 72,  0,145, 75,  0,155,157,159,170,178,  0,180,182,  0,
   70, 47, 50, 53, 93, 56,106, 60,117, 63, 69, 78, 81, 87,  0,  0,193,  0,
  105,  0,108,  0,185,188,110,  0,195,  0,198,  0,206,210,  0,219,230,238,
    0,200,204,120,215,123,240,  0,244,  0,  0,251,253,  0,257,  0,264,267,
  270,276,282,  0,290,292,  0,301,303,  0,297,151,305,  0,334,341,  0,308,
  313,319,323,329,158,345,  0
};
static const unsigned short ag_key_index[] = {
   23,  0, 40, 43, 43,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0, 90,  0,112,112,  0,116,  0,  0,116,  0,118,  0,  0,  0,
    0,  0,127,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,112,
    0,  0,  0,  0,  0,  0,  0,  0,  0,118,118,  0,118,  0,  0,  0,  0,  0,
  127,127,  0,  0,  0,  0,134,  0,  0,  0,  0,  0,  0,  0,112,112,  0,  0,
    0,  0,116,118,  0,118,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,127,  0,  0,  0,  0,  0,  0,  0,  0,112,  0,  0,  0,  0,  0,  0,
  116,118,  0,  0,  0,  0,  0,  0,  0,136,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,140,142,  0,  0,148,154,  0,
    0,  0,  0,  0,  0,161,  0,  0,140,142,  0,  0,148,154,  0,  0,  0,161,
    0,  0,  0,154,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,154,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,140,  0,  0,118,  0,  0,
    0,140,  0,118,  0,  0,  0,  0,  0,  0,  0,  0
};
static const unsigned char ag_key_ends[] = {
67,75,0, 69,0, 78,67,69,76,0, 58,0, 86,73,84,69,0, 
82,69,83,58,0, 71,73,83,84,69,82,0, 50,46,48,0, 58,0, 
79,83,84,58,0, 83,69,82,73,78,70,79,58,0, 79,83,84,58,0, 
86,45,65,66,83,58,0, 83,69,82,73,78,70,79,58,0, 58,0, 75,0, 
73,65,83,0, 65,78,67,72,0, 69,0, 78,67,69,76,0, 77,80,0, 
73,68,68,69,78,0, 58,0, 86,73,84,69,0, 82,0, 68,68,82,0, 
84,72,79,68,0, 82,73,71,0, 79,78,69,0, 69,83,58,0, 
69,73,86,69,68,0, 73,83,84,69,82,0, 79,82,84,0, 84,80,0, 
67,79,77,80,0, 58,0, 80,0, 83,0, 75,69,78,73,90,69,68,45,66,89,0, 
65,78,83,80,79,82,84,0, 76,0, 80,0, 69,82,0, 77,58,0, 
82,69,83,58,0, 58,0, 73,80,0, 58,0, 79,77,80,0, 82,0, 
68,68,82,0, 84,72,79,68,0, 82,73,71,0, 
75,69,78,73,90,69,68,45,66,89,0, 65,78,83,80,79,82,84,0, 76,0, 
83,69,82,0, 73,80,47,50,46,48,0, 78,0, 82,85,69,0, 
73,71,67,79,77,80,0, 67,75,0, 89,69,0, 65,78,67,69,76,0, 
78,86,73,84,69,0, 69,71,73,83,84,69,82,0, 80,0, 72,79,78,69,0, 
67,84,80,0, 80,0, 83,0, 68,80,0, 76,73,65,83,0, 
82,65,78,67,72,0, 79,77,80,0, 73,68,68,69,78,0, 65,68,68,82,0, 
67,69,73,86,69,68,0, 79,82,84,0, 84,76,0, 
};
#define AG_TCV(x) ag_tcv[(x)]
static const unsigned short ag_tcv[] = {
    5,287,287,287,287,287,287,287,287,273,  0,287,287,  0,287,287,287,287,
  287,287,287,287,287,287,287,287,287,287,287,287,287,287, 20,274, 40,284,
  285,274,237,274,281,281,274,275, 22,276,192, 36, 72, 72, 72, 72, 72, 72,
   72, 72, 72, 72, 16, 32, 38, 27,262, 34, 29,277,277,277,277,277,277,278,
  278,278,278,278,278,278,278,278,278,278,278,278,278,278,278,278,278,278,
  278,188, 62,189,286,279,280,277,277,277,277,277,277,278,278,278,278,278,
  278,278,278,278,278,278,278,278,278,278,278,278,278,278,278,282,286,282,
  274,287,286,286,286,286,286,286,286,286,286,286,286,286,286,286,286,286,
  286,283,283,286,286,286,286,286,283,286,286,286,286,286,286,286,286,286,
  286,286,286,286,286,286,286,286,286,286,286,286,286,286,286,286,286,286,
  286,286,286,286,286,286,286,286,286,286,286,286,286,286,286,286,286,286,
  286,286,286,286,286,286,286,286,286,286,286,286,286,286,286,286,286,286,
  286,286,286,286,286,286,286,286,286,286,286,286,286,286,286,286,286,286,
  286,286,286,286,286,286,286,286,286,286,286,286,286,286,286,286,286,286,
  286,286,286,286
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
280,279,278,277,276,275,274,260,259,258,257,256,255,254,253,252,251,250,249,
  248,192,158,156,152,151,81,72,69,68,67,66,65,10,6,2,0,1,8,9,12,13,14,15,
  41,43,64,77,146,147,148,149,153,157,246,247,
280,279,278,277,276,275,274,192,72,0,
183,171,0,150,159,162,168,182,
183,171,155,0,150,154,159,162,168,182,
183,171,155,0,150,154,159,162,168,182,
273,20,16,0,17,18,19,25,
20,0,19,78,
5,0,
5,0,
5,0,
5,0,
16,0,3,
5,0,
5,0,
16,0,3,
16,0,3,
278,277,188,72,0,103,114,170,184,
285,284,281,279,278,277,276,275,274,237,192,72,36,34,32,29,27,22,0,28,169,
  172,174,176,
273,262,34,32,20,5,0,31,160,202,
273,262,34,32,20,5,0,31,160,202,
278,277,276,275,192,72,0,134,135,138,
278,277,276,275,192,72,0,134,135,138,
273,20,0,
16,0,30,
278,277,276,275,273,192,158,156,152,151,72,20,16,0,
278,277,276,275,192,158,156,152,151,72,0,132,133,134,135,138,146,147,148,
  149,153,157,
72,20,0,19,79,82,
280,279,278,277,276,275,274,273,192,96,72,40,38,20,0,17,18,25,
278,277,276,275,192,72,0,11,134,135,138,
280,279,278,277,276,275,274,192,72,40,38,0,7,37,56,140,261,263,271,272,
280,279,278,277,276,275,274,192,96,72,0,4,41,43,87,88,89,93,
279,278,277,276,192,72,0,187,
277,195,72,0,131,193,194,197,
16,0,30,
278,277,188,72,0,103,114,170,184,
285,284,281,279,278,277,276,275,274,237,192,72,36,34,32,27,22,0,
16,0,30,
29,0,28,
285,281,279,278,277,276,275,274,273,237,228,227,221,220,215,214,192,189,188,
  128,124,118,72,36,20,16,0,17,18,25,
32,0,31,
273,262,34,20,5,0,33,161,235,
273,262,34,20,5,0,33,161,235,
278,277,276,275,192,72,0,
16,0,30,
287,286,285,284,283,282,281,280,279,278,277,276,275,274,273,262,237,192,189,
  188,72,62,40,38,36,34,32,29,27,22,20,16,5,0,17,18,25,
20,0,19,78,
72,0,
20,0,19,
5,0,
280,279,278,277,276,275,274,273,192,72,38,20,0,264,265,266,
287,286,285,284,283,282,281,280,279,278,277,276,275,274,273,262,237,192,189,
  188,72,62,40,38,36,34,32,29,27,22,20,16,5,0,57,58,59,60,
280,279,278,277,276,275,274,192,72,0,
273,38,20,0,17,18,25,
278,277,276,275,273,192,158,156,152,151,72,20,0,17,18,25,
38,0,37,
273,32,22,20,5,0,17,18,25,
22,0,23,
5,0,
273,36,20,0,17,18,25,
273,20,0,17,25,
22,0,23,
5,0,
279,278,277,276,192,72,0,
277,72,0,
273,195,192,189,32,22,20,16,5,0,198,199,200,
277,273,192,189,72,32,22,20,5,0,194,196,197,
195,0,
192,0,
189,0,
72,0,73,102,
285,281,279,278,277,276,275,274,237,192,72,29,27,22,0,173,177,179,180,181,
278,277,188,72,0,103,114,170,184,
285,281,279,278,277,276,275,274,237,228,227,221,220,215,214,192,189,188,128,
  124,118,72,36,16,0,203,204,205,206,207,208,209,210,211,212,213,229,
285,281,279,278,277,276,275,274,273,237,228,227,221,220,215,214,192,189,188,
  128,124,118,72,36,20,16,0,17,18,25,
285,281,279,278,277,276,275,274,192,189,188,72,36,34,16,0,236,238,241,
237,0,
285,284,281,279,278,277,276,275,274,273,237,192,189,188,72,36,34,32,29,27,
  22,20,16,0,17,18,25,
287,286,285,284,283,282,281,280,279,278,277,276,275,274,262,237,192,189,188,
  72,62,40,38,36,34,32,29,27,22,16,0,50,
81,20,0,19,77,
72,0,
286,285,284,283,282,281,280,279,278,277,276,275,274,262,237,192,189,188,72,
  62,40,38,36,34,32,29,27,22,20,16,5,0,80,84,85,86,
273,20,0,264,
287,286,285,284,283,282,281,280,279,278,277,276,275,274,273,262,237,192,189,
  188,72,62,40,38,36,34,32,29,27,22,20,16,5,0,
287,286,285,284,283,282,281,280,279,278,277,276,275,274,273,262,237,192,189,
  188,72,62,38,36,34,32,29,27,22,20,16,5,0,57,58,
40,0,
280,279,278,277,276,275,274,273,192,72,38,20,0,264,265,266,
278,277,276,275,192,158,156,152,151,72,0,133,134,135,138,146,147,148,149,
  153,157,
278,277,276,275,273,192,158,156,152,151,72,20,0,17,18,25,
32,0,31,74,
280,279,278,277,276,275,274,273,192,72,40,38,20,0,17,18,25,
36,0,35,
278,277,188,72,0,90,103,114,184,
280,279,278,277,276,275,274,273,192,96,72,20,0,17,18,25,
277,72,0,
277,72,0,197,
16,0,198,
277,273,192,189,72,32,22,20,5,0,194,196,197,
72,0,190,191,
72,0,
285,281,279,278,277,276,275,274,237,192,72,27,22,0,179,
285,281,279,278,277,276,275,274,273,262,237,192,189,188,72,36,34,32,27,20,
  16,5,0,232,233,234,
27,0,26,230,
273,27,20,0,17,18,25,
273,27,20,0,17,18,25,
280,279,278,277,276,275,274,273,262,192,72,34,32,27,20,5,0,26,225,
273,27,20,0,17,18,25,
273,27,20,0,17,18,25,
273,27,20,0,17,18,25,
273,27,20,0,17,18,25,
273,27,20,0,17,18,25,
285,281,279,278,277,276,275,274,237,228,227,221,220,215,214,192,189,188,128,
  124,118,72,36,16,0,203,204,205,206,207,208,209,210,211,212,213,229,
285,281,279,278,277,276,275,274,192,189,188,72,36,34,16,0,
27,0,
285,281,279,278,277,276,275,274,192,189,188,72,36,34,16,0,236,238,241,
285,284,281,279,278,277,276,275,274,237,192,189,188,72,36,34,32,29,27,22,16,
  0,136,142,
287,286,285,284,283,282,281,280,279,278,277,276,275,274,273,262,237,192,189,
  188,72,62,40,38,36,34,32,29,27,22,20,16,5,0,53,54,55,
273,20,5,0,17,18,25,
286,285,284,283,282,281,280,279,278,277,276,275,274,262,237,192,189,188,72,
  62,40,38,36,34,32,29,27,22,20,16,0,84,
273,262,20,0,17,18,25,
278,277,276,275,192,158,156,152,151,72,0,133,134,135,138,146,147,148,149,
  153,157,
280,279,278,277,276,275,274,273,192,72,20,0,17,18,25,
32,0,31,
280,279,278,277,276,275,274,192,72,40,38,0,37,56,140,261,263,272,
280,279,278,277,276,275,274,273,192,72,20,0,17,18,25,
273,32,22,20,16,5,0,17,18,25,
32,22,5,0,91,
280,279,278,277,276,275,274,192,96,72,0,41,43,88,89,93,
277,72,0,
72,0,
192,0,
285,281,279,278,277,276,275,274,237,192,189,188,72,36,16,0,232,
285,281,279,278,277,276,275,274,273,237,192,189,188,72,36,20,16,0,17,18,25,
27,0,26,
27,0,26,
280,279,278,277,276,275,274,273,262,192,72,34,32,20,5,0,41,43,226,
224,223,222,0,
27,0,26,
27,0,26,
27,0,26,
27,0,26,
27,0,26,
285,281,279,278,277,276,275,274,273,262,237,192,189,188,72,36,34,29,20,16,5,
  0,239,243,244,245,
285,284,281,279,278,277,276,275,274,237,192,189,188,72,36,34,32,29,27,22,16,
  0,
287,286,285,284,283,282,281,280,279,278,277,276,275,274,273,262,237,192,189,
  188,72,62,40,38,36,34,32,29,27,22,20,16,0,53,
262,0,
273,262,20,0,17,18,25,
280,279,278,277,276,275,274,192,72,0,41,43,75,
280,279,278,277,276,275,274,273,192,72,20,0,17,18,25,
280,279,278,277,276,275,274,192,72,0,41,43,94,
16,0,30,
32,0,31,
72,0,
72,0,191,
285,281,279,278,277,276,275,274,237,192,189,188,72,36,16,0,229,
278,277,273,188,72,20,0,17,18,25,
280,279,278,277,276,275,274,273,192,130,72,20,0,17,18,25,
280,279,278,277,276,275,274,273,192,72,69,68,67,66,65,20,0,17,18,25,
278,277,273,188,72,20,0,17,18,25,
273,72,20,0,17,18,25,
280,279,278,277,276,275,274,273,219,218,192,72,20,0,17,18,25,
280,279,278,277,276,275,274,273,192,101,100,99,98,72,20,0,17,18,25,
285,281,279,278,277,276,275,274,192,189,188,72,36,34,29,16,0,243,
262,0,
273,32,27,22,20,5,0,17,18,25,
280,279,278,277,276,275,274,192,72,0,41,43,75,
273,36,20,0,17,18,25,
273,72,20,0,17,18,25,
280,279,278,277,276,275,274,273,192,128,127,125,124,122,118,116,115,72,20,0,
  17,18,25,
192,0,
278,277,188,72,0,103,114,184,
280,279,278,277,276,275,274,192,130,72,0,41,43,129,
280,279,278,277,276,275,274,192,72,69,68,67,66,65,0,41,43,64,
278,277,188,72,0,103,114,184,
72,0,119,
280,279,278,277,276,275,274,219,218,192,72,0,41,43,216,217,
280,279,278,277,276,275,274,192,101,100,99,98,72,0,41,43,97,
27,0,26,76,
36,0,35,
72,0,73,102,
280,279,278,277,276,275,274,192,128,127,125,124,122,118,116,115,72,0,41,43,
  92,104,105,106,107,108,109,110,111,112,113,117,120,123,
72,0,191,
72,0,
280,279,278,277,276,275,274,273,192,72,40,20,0,17,18,25,
280,279,278,277,276,275,274,273,192,101,100,99,98,72,20,0,17,18,25,
273,32,22,20,5,0,17,18,25,
273,32,27,22,20,5,0,17,18,25,
273,32,27,22,20,5,0,17,18,25,
273,27,20,0,17,18,25,
273,27,20,0,17,18,25,
273,27,20,0,17,18,25,
32,27,22,5,0,26,121,
273,27,20,0,17,18,25,
273,27,20,0,17,18,25,
273,32,22,20,5,0,17,18,25,
273,32,22,20,5,0,17,18,25,
72,0,
280,279,278,277,276,275,274,192,72,40,0,41,43,56,
280,279,278,277,276,275,274,192,101,100,99,98,72,0,41,43,95,97,
27,0,26,
27,0,26,
27,0,26,
27,0,26,
273,72,20,0,17,18,25,
27,0,26,
27,0,26,
273,32,22,20,5,0,17,18,25,
273,32,22,20,5,0,17,18,25,
280,279,278,277,276,275,274,273,192,188,72,40,20,0,17,18,25,
278,277,273,188,72,20,0,17,18,25,
273,72,20,0,17,18,25,
280,279,278,277,276,275,274,273,192,130,72,20,0,17,18,25,
72,0,73,102,
280,279,278,277,276,275,274,273,192,72,20,0,17,18,25,
277,273,195,72,20,0,17,18,25,
280,279,278,277,276,275,274,192,188,72,40,0,41,43,56,114,
278,277,188,72,0,103,114,184,
72,0,119,
280,279,278,277,276,275,274,192,130,72,0,41,43,129,
280,279,278,277,276,275,274,192,72,0,41,43,
277,195,72,0,126,131,193,194,197,
273,32,22,20,5,0,17,18,25,
273,32,22,20,5,0,17,18,25,
273,32,22,20,5,0,17,18,25,
273,32,22,20,5,0,17,18,25,
273,32,22,20,5,0,17,18,25,
273,32,22,20,5,0,17,18,25,
273,32,22,20,5,0,17,18,25,
273,32,22,20,5,0,17,18,25,
};
static unsigned const char ag_astt[2441] = {
  1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,1,2,2,2,2,2,1,2,2,2,2,2,1,1,1,7,0,
  1,1,1,1,1,1,2,1,2,1,2,2,2,1,1,1,2,1,9,9,9,9,9,9,9,9,9,4,2,2,7,3,1,1,1,1,2,
  2,2,7,2,1,1,1,1,1,2,2,2,7,2,1,1,1,1,1,1,1,8,7,1,1,1,1,2,7,1,1,3,7,3,7,3,7,
  3,7,1,7,1,3,7,3,7,1,7,1,1,7,1,1,1,1,1,7,1,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,2,1,1,7,1,1,1,2,1,4,4,4,2,4,4,7,1,1,1,4,4,4,2,4,4,7,1,1,1,1,1,1,1,1,1,
  7,2,1,1,1,1,1,1,1,1,7,2,1,1,9,9,4,2,7,1,4,4,4,4,5,4,4,4,4,4,4,5,5,7,1,1,1,
  1,1,2,2,2,2,1,7,1,1,2,1,1,2,2,2,1,1,1,1,2,7,3,1,1,5,5,5,5,5,5,5,1,5,5,5,5,
  5,1,7,2,2,1,1,1,1,1,1,1,7,1,2,1,1,1,1,1,1,1,1,1,1,1,1,2,7,1,1,1,1,1,1,1,2,
  1,1,1,1,1,1,1,1,2,1,7,1,2,1,1,2,1,1,1,1,1,1,1,1,4,1,1,1,1,7,1,1,1,1,2,4,1,
  1,1,1,1,7,1,2,2,2,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,4,2,4,1,2,7,1,8,8,8,8,
  8,8,8,8,1,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,1,8,7,1,1,1,2,4,1,4,4,2,4,4,7,1,2,
  1,4,4,2,4,4,7,1,2,1,9,9,9,9,9,9,4,2,7,1,8,8,8,8,8,8,8,8,8,8,8,8,8,8,1,8,8,
  8,8,8,8,8,8,8,8,8,8,8,8,8,1,8,5,7,1,1,1,2,7,1,1,1,7,2,4,1,3,7,5,5,5,5,5,5,
  5,1,5,5,5,1,7,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,8,1,1,1,1,
  1,1,1,1,1,1,7,1,1,1,1,1,1,1,1,1,1,1,1,1,4,1,5,1,7,2,2,1,8,8,8,8,1,8,8,8,8,
  8,8,1,7,1,1,1,2,7,1,1,8,5,1,5,7,1,1,1,2,5,1,3,7,1,8,1,7,1,1,1,1,1,7,1,1,2,
  5,1,3,7,9,9,9,9,9,9,4,1,1,5,5,5,5,5,5,5,5,1,5,7,1,1,3,1,5,5,5,1,5,5,5,5,7,
  3,3,1,1,5,1,5,2,7,1,7,1,2,1,1,1,1,1,1,1,1,1,1,1,5,1,1,7,2,2,1,1,2,1,1,1,1,
  7,1,2,2,2,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,2,2,2,2,2,2,2,
  2,2,2,2,1,8,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,1,8,7,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,7,2,1,1,1,4,8,8,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,
  8,8,8,1,8,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,4,2,2,2,7,3,1,2,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,5,7,2,1,1,2,9,9,5,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
  3,3,3,3,3,3,3,3,3,3,7,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,1,9,9,9,9,
  9,9,9,9,9,9,5,3,3,2,7,5,5,5,5,5,5,5,1,5,5,5,1,7,1,1,2,1,1,1,1,1,2,2,2,2,1,
  7,1,2,1,1,2,2,2,1,1,1,8,8,8,8,1,8,8,8,8,8,8,1,7,1,1,1,2,4,1,1,8,8,8,8,8,8,
  8,1,8,8,8,8,1,7,1,1,1,2,7,1,1,1,1,1,7,1,1,2,2,8,8,8,8,8,8,8,1,8,8,8,1,7,1,
  1,1,1,1,5,1,1,7,3,1,5,3,1,5,5,5,1,5,5,5,5,7,3,3,1,1,7,3,1,9,4,9,9,9,9,9,9,
  9,9,9,9,9,9,9,5,3,1,1,1,1,1,1,1,1,5,5,1,1,1,1,1,1,5,5,5,5,1,5,7,1,1,2,2,4,
  1,2,1,8,1,7,1,1,1,1,8,1,7,1,1,1,8,8,8,8,8,8,8,5,5,8,8,5,5,2,5,5,7,1,1,1,8,
  1,7,1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,1,1,1,
  1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,2,2,2,2,2,2,2,2,2,2,2,1,9,9,9,9,
  9,9,9,9,9,9,9,9,9,9,9,4,1,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,2,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,7,1,1,2,1,1,5,7,2,2,1,9,9,9,9,9,9,9,9,9,9,
  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,5,3,1,8,1,7,1,1,1,1,1,1,1,1,2,2,2,
  2,1,7,1,2,1,1,2,2,2,1,1,1,8,8,8,8,8,8,8,1,8,8,1,7,1,1,1,2,4,1,1,1,1,1,1,1,
  1,1,1,1,2,7,1,1,1,1,1,2,8,8,8,8,8,8,8,1,8,8,1,7,1,1,1,1,5,5,1,8,5,7,1,1,1,
  4,4,4,7,1,1,1,1,1,1,1,1,1,2,1,7,2,1,2,1,1,3,3,5,1,5,1,7,9,9,9,9,9,9,9,9,9,
  9,9,9,9,9,9,5,3,8,8,8,8,8,8,8,8,1,8,8,8,8,8,8,1,8,7,1,1,1,2,7,1,2,7,1,1,1,
  1,1,1,1,1,5,5,1,1,5,5,5,5,7,2,1,2,2,2,2,5,2,7,1,2,7,1,2,7,1,2,7,1,2,7,1,1,
  1,1,1,1,1,1,1,5,5,5,1,1,1,1,1,1,1,5,1,5,7,2,1,1,2,9,9,9,9,9,9,9,9,9,9,9,9,
  9,9,9,9,9,9,9,9,9,4,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
  9,9,9,9,9,5,3,2,7,1,8,1,7,1,1,1,1,1,1,1,1,1,1,1,1,7,1,1,3,8,8,8,8,8,8,8,1,
  8,8,1,7,1,1,1,1,1,1,1,1,1,1,1,1,7,2,1,1,2,4,1,2,4,1,3,5,1,7,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,7,2,8,8,1,8,8,1,7,1,1,1,8,8,8,8,8,8,8,1,8,8,8,1,7,1,1,1,
  8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,1,7,1,1,1,8,8,1,8,8,1,7,1,1,1,1,8,1,7,1,1,1,
  8,8,8,8,8,8,8,1,8,8,8,8,1,7,1,1,1,8,8,8,8,8,8,8,1,8,8,8,8,8,8,1,7,1,1,1,9,
  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,5,3,2,7,1,5,8,5,1,5,7,1,1,1,1,1,1,1,1,1,1,1,
  1,7,1,1,3,1,8,1,7,1,1,1,1,8,1,7,1,1,1,8,8,8,8,8,8,8,1,8,8,8,8,8,8,8,8,8,8,
  1,7,1,1,1,1,7,1,1,1,1,7,2,2,2,1,1,1,1,1,1,1,1,2,1,7,2,1,2,1,1,1,1,1,1,1,1,
  1,2,2,2,2,2,7,2,1,2,1,1,1,1,7,2,2,2,1,7,2,1,1,1,1,1,1,1,2,2,1,1,7,2,1,2,2,
  1,1,1,1,1,1,1,1,2,2,2,2,1,7,2,1,2,2,4,1,2,2,7,1,1,7,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,7,1,1,2,2,2,2,2,2,2,2,2,2,2,2,1,2,1,7,3,1,4,8,8,8,8,8,8,8,
  1,8,8,8,1,7,1,1,1,8,8,8,8,8,8,8,1,8,8,8,8,8,8,1,7,1,1,1,1,5,5,1,5,7,2,2,1,
  1,5,8,5,1,5,7,1,1,1,1,5,5,5,1,5,7,3,3,1,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,8,1,
  7,1,1,1,4,2,4,4,7,1,2,1,8,1,7,1,1,1,1,8,1,7,1,1,1,1,5,5,1,5,7,3,3,1,1,5,5,
  1,5,7,3,3,1,2,4,1,1,1,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,1,7,2,
  1,2,2,2,4,1,2,7,1,2,7,1,2,7,1,1,8,1,7,1,1,1,2,7,1,2,7,1,1,5,5,1,5,7,2,2,1,
  1,5,5,1,5,7,2,2,1,8,8,8,8,8,8,8,1,8,8,8,8,1,7,1,1,1,8,8,1,8,8,1,7,1,1,1,1,
  8,1,7,1,1,1,8,8,8,8,8,8,8,1,8,8,8,1,7,1,1,1,1,7,1,2,8,8,8,8,8,8,8,1,8,8,1,
  7,1,1,1,8,1,8,8,1,7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,1,7,1,2,2,
  1,7,1,1,1,1,1,1,1,1,1,2,1,7,2,1,1,1,1,1,1,1,1,1,1,1,7,1,1,1,1,1,7,1,2,1,1,
  1,1,5,5,1,5,7,2,2,1,1,5,5,1,5,7,2,2,1,1,5,5,1,5,7,2,2,1,1,5,5,1,5,7,2,2,1,
  1,5,5,1,5,7,2,2,1,1,5,5,1,5,7,2,2,1,1,5,5,1,5,7,2,2,1,1,5,5,1,5,7,2,2,1
};
static const unsigned short ag_pstt[] = {
1,1,1,1,1,1,1,291,290,289,288,287,286,285,284,283,282,281,280,279,1,156,153,
  149,148,70,1,54,53,52,51,50,11,14,15,0,0,13,12,10,9,8,7,55,1,292,6,144,
  145,146,2,4,3,277,5,
28,28,28,28,28,28,28,28,28,29,
189,174,2,147,19,18,17,16,
189,174,152,3,155,20,19,18,17,16,
189,174,152,4,151,21,19,18,17,16,
22,24,23,5,23,23,25,22,
13,6,26,26,
9,7,
8,8,
7,9,
6,10,
27,11,28,
4,12,
3,13,
27,14,29,
27,15,30,
31,31,32,31,16,33,193,188,192,
35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,20,35,35,17,34,37,36,177,35,
220,220,220,22,220,220,18,38,40,39,
220,220,220,22,220,220,19,38,41,39,
42,42,42,42,42,42,20,154,43,42,
42,42,42,42,42,42,21,150,43,42,
17,17,18,
21,23,44,
13,13,13,13,16,13,13,13,13,13,13,16,16,24,
42,42,42,42,42,156,153,149,148,42,25,45,45,296,43,42,293,294,295,2,4,3,
46,13,26,67,47,47,
10,10,10,10,10,10,10,22,10,10,10,10,10,22,27,12,12,22,
42,42,42,42,42,42,28,48,132,43,42,
49,49,49,49,49,49,49,49,49,50,25,29,57,53,52,54,55,51,56,316,
1,1,1,1,1,1,1,1,87,1,30,61,86,1,60,80,59,58,
62,62,62,62,62,62,194,62,
63,65,63,32,68,67,66,64,
21,190,69,
31,31,32,31,34,33,193,173,192,
179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,180,
21,175,70,
20,37,71,
72,72,72,72,72,72,72,72,22,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,22,
  72,38,72,72,22,
22,221,73,
263,263,23,263,263,40,74,158,75,
263,263,23,263,263,41,74,157,75,
135,135,135,135,135,135,136,
21,43,76,
77,77,77,77,77,77,77,77,77,77,77,77,77,77,22,77,77,77,77,77,77,77,77,77,77,
  77,77,77,77,77,22,77,10,44,77,77,22,
13,45,78,78,
79,46,
13,69,80,
5,48,
304,304,304,304,304,304,304,81,304,304,304,81,49,81,81,307,
83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,82,84,83,83,
  83,83,83,83,83,83,83,83,50,83,83,83,84,
85,85,85,85,85,85,85,85,85,300,
22,10,22,52,299,299,22,
86,86,86,86,22,86,86,86,86,86,86,22,53,86,86,22,
25,54,87,
22,88,10,22,10,55,88,88,22,
15,315,89,
2,57,
22,90,22,58,90,90,22,
22,22,59,91,22,
15,79,92,
1,61,
196,196,196,196,196,196,197,
93,93,216,
213,213,213,213,213,213,213,94,213,64,95,95,215,
63,206,206,206,63,206,206,206,206,65,209,209,64,
96,205,
97,203,
198,68,
98,69,98,191,
99,99,99,99,99,99,99,99,99,99,99,185,99,99,70,176,181,99,99,181,
31,31,32,31,71,33,193,172,192,
100,100,100,100,100,100,100,100,100,253,102,104,105,108,109,100,100,100,103,
  106,107,100,100,100,72,222,224,225,226,227,228,229,230,231,232,233,101,
110,110,110,110,110,110,110,110,22,110,110,110,110,110,110,110,110,110,110,
  110,110,110,110,110,22,110,73,110,110,22,
111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,74,265,112,111,
113,264,
114,114,114,114,114,114,114,114,114,22,114,114,114,114,114,114,114,114,114,
  114,114,22,114,76,114,114,22,
115,115,115,115,115,115,115,115,115,115,115,115,115,115,115,115,115,115,115,
  115,115,115,115,115,115,115,115,115,115,115,313,314,
70,13,78,67,116,
78,79,
117,117,117,117,117,117,117,117,117,117,117,117,117,117,117,117,117,117,117,
  117,117,117,117,117,117,117,117,117,117,117,75,80,68,117,117,77,
303,303,305,303,
49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,
  49,49,49,49,49,49,49,49,82,
44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,82,44,44,44,
  44,44,44,44,44,44,44,46,44,44,
47,84,
304,304,304,304,304,304,304,81,304,304,304,81,85,81,81,306,
42,42,42,42,42,156,153,149,148,42,86,118,296,43,42,293,294,295,2,4,3,
119,119,119,119,22,119,119,119,119,119,119,22,87,119,119,22,
22,318,120,121,
122,122,122,122,122,122,122,22,122,122,122,122,22,89,122,122,22,
24,90,123,
31,31,32,31,91,125,124,193,192,
126,126,126,126,126,126,126,22,126,126,126,22,92,126,126,22,
127,127,217,
63,63,94,210,
94,214,212,
63,206,206,206,63,206,206,206,206,96,208,208,64,
128,97,204,129,
58,95,
184,184,184,184,184,184,184,184,184,184,184,184,184,186,184,
130,130,130,130,130,130,130,130,259,259,130,130,130,130,130,130,259,259,259,
  259,130,259,100,130,130,261,
19,254,131,255,
22,132,22,102,132,132,22,
22,133,22,103,133,133,22,
134,134,134,134,134,134,134,246,246,134,134,246,246,19,246,246,104,135,134,
22,136,22,105,136,136,22,
22,137,22,106,137,137,22,
22,138,22,107,138,138,22,
22,139,22,108,139,139,22,
22,140,22,109,140,140,22,
100,100,100,100,100,100,100,100,100,253,102,104,105,108,109,100,100,100,103,
  106,107,100,100,100,110,223,224,225,226,227,228,229,230,231,232,233,101,
269,269,269,269,269,269,269,269,269,269,269,269,269,269,269,270,
141,112,
111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,113,266,112,111,
142,142,142,142,142,142,142,142,142,142,142,142,142,142,142,142,142,142,142,
  142,142,114,133,142,
143,143,143,143,143,143,143,143,143,143,143,143,143,143,143,143,143,143,143,
  143,143,143,143,143,143,143,143,143,143,143,143,143,39,115,143,143,41,
22,22,10,116,278,278,22,
74,74,74,74,74,74,74,74,74,74,74,74,74,74,74,74,74,74,74,74,74,74,74,74,74,
  74,74,74,74,74,76,74,
22,144,22,118,144,144,22,
42,42,42,42,42,156,153,149,148,42,119,145,296,43,42,293,294,295,2,4,3,
146,146,146,146,146,146,146,22,146,146,22,120,146,146,22,
22,319,147,
49,49,49,49,49,49,49,49,49,50,25,122,53,52,54,55,51,317,
148,148,148,148,148,148,148,22,148,148,22,123,148,148,22,
22,10,10,22,149,10,124,149,149,22,
83,83,83,125,150,
1,1,1,1,1,1,1,1,87,1,126,86,1,81,59,58,
219,219,218,
151,200,
152,129,
258,258,258,258,258,258,258,258,258,258,258,258,258,258,258,260,258,
153,153,153,153,153,153,153,153,22,153,153,153,153,153,153,22,153,131,153,
  153,22,
19,132,154,
19,133,155,
1,1,1,1,1,1,1,248,248,1,1,248,248,248,248,134,250,1,250,
245,244,243,247,
19,136,156,
19,137,157,
19,138,158,
19,139,159,
19,140,160,
161,161,161,161,161,161,161,161,274,274,274,161,161,161,161,161,161,161,274,
  161,274,141,267,161,161,276,
139,139,139,139,139,139,139,139,139,139,139,139,139,139,139,139,139,139,139,
  139,139,140,
38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,
  38,38,38,38,38,38,38,40,38,
298,144,
22,162,22,145,162,162,22,
1,1,1,1,1,1,1,1,1,146,163,1,60,
164,164,164,164,164,164,164,22,164,164,22,147,164,164,22,
1,1,1,1,1,1,1,1,1,148,88,1,165,
21,96,166,
22,82,167,
202,201,
128,152,168,
100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,153,262,
169,169,22,169,169,22,154,169,169,22,
170,170,170,170,170,170,170,22,170,170,170,22,155,170,170,22,
171,171,171,171,171,171,171,22,171,171,171,171,171,171,171,22,156,171,171,
  22,
172,172,22,172,172,22,157,172,172,22,
22,173,22,158,173,173,22,
174,174,174,174,174,174,174,22,174,174,174,174,22,159,174,174,22,
175,175,175,175,175,175,175,22,175,175,175,175,175,175,22,160,175,175,22,
273,273,273,273,273,273,273,273,273,273,273,273,273,273,273,273,275,273,
297,162,
22,10,176,10,22,10,163,176,176,22,
1,1,1,1,1,1,1,1,1,164,163,1,61,
22,177,22,165,177,177,22,
22,178,22,166,178,178,22,
179,179,179,179,179,179,179,22,179,179,179,179,179,179,179,179,179,179,22,
  167,179,179,22,
180,168,
31,31,32,31,169,252,193,192,
1,1,1,1,1,1,1,1,128,1,170,129,1,251,
1,1,1,1,1,1,1,1,1,54,53,52,51,50,171,55,1,242,
31,31,32,31,172,241,193,192,
181,173,240,
1,1,1,1,1,1,1,238,237,1,1,174,239,1,235,236,
1,1,1,1,1,1,1,1,93,92,91,90,1,175,94,1,234,
19,62,182,63,
24,177,183,
98,178,98,184,
1,1,1,1,1,1,1,1,189,191,192,187,186,188,193,194,1,179,185,1,84,98,99,100,
  101,102,103,104,105,106,106,114,190,123,
128,180,199,
195,116,
196,196,196,196,196,196,196,22,196,196,196,22,182,196,196,22,
197,197,197,197,197,197,197,22,197,197,197,197,197,197,22,183,197,197,22,
22,10,10,22,10,184,97,97,22,
22,10,198,10,22,10,185,198,198,22,
22,10,10,10,22,10,186,120,120,22,
22,199,22,187,199,199,22,
22,200,22,188,200,200,22,
22,201,22,189,201,201,22,
121,19,121,121,190,202,119,
22,203,22,191,203,203,22,
22,204,22,192,204,204,22,
22,10,10,22,10,193,113,113,22,
22,10,10,22,10,194,112,112,22,
118,117,
1,1,1,1,1,1,1,1,1,50,196,206,1,205,
1,1,1,1,1,1,1,1,93,92,91,90,1,197,94,1,85,89,
19,108,207,
19,199,208,
19,200,209,
19,201,210,
22,211,22,202,211,211,22,
19,203,212,
19,204,213,
22,10,10,22,10,205,65,65,22,
22,10,10,22,10,206,64,64,22,
214,214,214,214,214,214,214,22,214,214,214,214,22,207,214,214,22,
215,215,22,215,215,22,208,215,215,22,
22,216,22,209,216,216,22,
217,217,217,217,217,217,217,22,217,217,217,22,210,217,217,22,
98,211,98,122,
218,218,218,218,218,218,218,22,218,218,22,212,218,218,22,
219,22,219,219,22,213,219,219,22,
1,1,1,1,1,1,1,1,32,1,50,214,222,1,221,220,
31,31,32,31,215,223,193,192,
181,216,224,
1,1,1,1,1,1,1,1,128,1,217,129,1,225,
1,1,1,1,1,1,1,1,1,218,226,1,
63,65,63,219,227,130,67,66,64,
22,10,10,22,10,220,111,111,22,
22,10,10,22,10,221,110,110,22,
22,10,10,22,10,222,109,109,22,
22,10,10,22,10,223,124,124,22,
22,10,10,22,10,224,115,115,22,
22,10,10,22,10,225,127,127,22,
22,10,10,22,10,226,126,126,22,
22,10,10,22,10,227,125,125,22,
};
static const unsigned short ag_sbt[] = {
     0,  55,  65,  73,  83,  93, 101, 105, 107, 109, 111, 113, 116, 118,
   120, 123, 126, 135, 159, 169, 179, 189, 199, 202, 205, 219, 241, 247,
   265, 276, 296, 314, 322, 330, 333, 342, 360, 363, 366, 396, 399, 408,
   417, 424, 427, 464, 468, 470, 473, 475, 491, 529, 539, 546, 562, 565,
   574, 577, 579, 586, 591, 594, 596, 603, 606, 619, 632, 634, 636, 638,
   642, 662, 671, 708, 738, 757, 759, 786, 818, 823, 825, 861, 865, 899,
   934, 936, 952, 973, 989, 993,1010,1013,1022,1038,1041,1045,1048,1061,
  1065,1067,1082,1108,1112,1119,1126,1145,1152,1159,1166,1173,1180,1217,
  1233,1235,1254,1278,1315,1322,1354,1361,1382,1397,1400,1418,1433,1443,
  1448,1464,1467,1469,1471,1488,1509,1512,1515,1534,1538,1541,1544,1547,
  1550,1553,1579,1601,1635,1637,1644,1657,1672,1685,1688,1691,1693,1696,
  1713,1723,1739,1759,1769,1776,1793,1812,1830,1832,1842,1855,1862,1869,
  1892,1894,1902,1916,1934,1942,1945,1961,1978,1982,1985,1989,2023,2026,
  2028,2044,2063,2072,2082,2092,2099,2106,2113,2120,2127,2134,2143,2152,
  2154,2168,2186,2189,2192,2195,2198,2205,2208,2211,2220,2229,2246,2256,
  2263,2279,2283,2298,2307,2323,2331,2334,2348,2360,2369,2378,2387,2396,
  2405,2414,2423,2432,2441
};
static const unsigned short ag_sbe[] = {
    35,  64,  67,  76,  86,  96, 102, 106, 108, 110, 112, 114, 117, 119,
   121, 124, 130, 153, 165, 175, 185, 195, 201, 203, 218, 229, 243, 261,
   271, 287, 306, 320, 325, 331, 337, 359, 361, 364, 392, 397, 404, 413,
   423, 425, 460, 465, 469, 471, 474, 487, 524, 538, 542, 558, 563, 570,
   575, 578, 582, 588, 592, 595, 602, 605, 615, 628, 633, 635, 637, 639,
   656, 666, 695, 734, 753, 758, 782, 816, 820, 824, 856, 863, 898, 931,
   935, 948, 962, 985, 990,1006,1011,1017,1034,1040,1043,1046,1057,1062,
  1066,1080,1104,1109,1115,1122,1142,1148,1155,1162,1169,1176,1204,1232,
  1234,1250,1275,1311,1318,1352,1357,1371,1393,1398,1411,1429,1439,1446,
  1458,1466,1468,1470,1486,1505,1510,1513,1530,1537,1539,1542,1545,1548,
  1551,1574,1600,1633,1636,1640,1653,1668,1681,1686,1689,1692,1694,1711,
  1719,1735,1755,1765,1772,1789,1808,1828,1831,1838,1851,1858,1865,1888,
  1893,1898,1912,1930,1938,1943,1956,1974,1979,1983,1986,2006,2024,2027,
  2040,2059,2068,2078,2088,2095,2102,2109,2117,2123,2130,2139,2148,2153,
  2164,2181,2187,2190,2193,2196,2201,2206,2209,2216,2225,2242,2252,2259,
  2275,2280,2294,2303,2318,2327,2332,2344,2357,2363,2374,2383,2392,2401,
  2410,2419,2428,2437,2441
};
static const unsigned char ag_fl[] = {
  1,4,4,2,2,4,2,2,2,2,0,1,2,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,2,1,1,2,1,1,2,
  1,1,1,2,0,1,2,1,1,2,0,1,3,1,2,1,1,1,1,1,1,2,1,2,1,3,4,2,3,4,4,1,2,5,3,
  1,1,1,1,2,0,1,1,3,1,1,4,4,0,4,9,1,1,1,1,1,1,1,1,1,1,2,6,1,1,1,1,1,1,1,
  1,1,1,2,6,6,6,2,2,1,6,1,2,3,2,2,0,3,1,6,6,6,6,1,1,1,1,1,4,1,2,1,1,1,2,
  1,1,2,1,1,1,1,2,1,1,3,2,1,1,3,2,1,3,3,1,1,1,1,4,4,4,4,3,4,4,3,4,4,3,1,
  1,3,1,1,2,1,1,1,1,2,0,1,1,2,1,1,3,1,1,1,1,2,2,3,5,1,2,3,1,3,1,0,1,3,2,
  2,1,2,0,1,2,1,2,3,4,0,1,3,4,1,1,1,1,1,1,1,1,1,1,5,5,5,1,1,1,5,5,5,3,3,
  3,0,1,0,1,3,5,5,1,1,2,1,1,2,0,1,2,3,0,1,2,3,3,1,2,1,1,1,2,0,1,1,1,6,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,5,2,1,1,1,2,0,1,3,2,5,3,4,2,1,4,5,
  1,1,4,2,3
};
static const unsigned short ag_ptt[] = {
    0,  1,  1,  1,  1,  1,  1,  1,  1,  1, 18, 18,  3, 19, 21, 23, 25, 25,
   17, 26, 28, 30, 31, 33, 35, 37, 39, 43, 43, 41, 46, 46, 44, 49, 49, 47,
   53, 54, 54, 55, 55, 50, 58, 59, 59, 60, 60, 56, 57, 57, 64, 64, 64, 64,
   64, 64, 70, 73, 73, 71, 74, 74, 75, 75, 76, 76, 78, 78,  8,  8, 77, 79,
   84, 85, 85, 86, 86, 80, 82,  4, 87, 87, 88, 91, 91, 89, 93, 93, 94, 95,
   97, 97, 97, 97, 97,102, 90, 90, 92, 92, 92, 92, 92, 92, 92, 92, 92,112,
  113,113,113,113,104,105,106,117,119,119,119,110,120,121,121,107,123,108,
  109,111,129,129,126,132, 11,134,138,138,135,139,142,142,136,145,145,143,
   14, 12, 13,146,149,149,147,147,154,153,148,148,157,150,150,163,163,163,
  163,164,164,165,165,165,166,166,166,167,159,159,168,169,169,172,176,176,
  174,173,179,180,180,181,181,177,162,182,170,170,103,103,184,187,187,184,
  114,190,191,191,191,131,131,193,196,196,193,193,198,199,199,200,200,194,
  197,197,197,197,160,160,202,202,203,203,203,203,203,203,203,203,203,203,
  204,205,205,216,216,217,206,207,208,209,209,209,225,225,226,226,209,210,
  211,212,213,213,232,233,233,234,234,229,230,161,161,235,235,236,241,241,
  238,243,244,244,245,245,239,  9,246,247,247,247,247,247,247,247,247,247,
  247,247,247,247,247,133,133,133,133,261,261,140,140,264,265,265,266,266,
  263,263,267,269,269,270,270, 15, 15,  7,271,271,272,272
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
    case 14: V(0,ParserBasicToken) = ag_rp_14(PCB_POINTER); break;
    case 15: V(0,ParserBasicToken) = ag_rp_15(PCB_POINTER); break;
    case 16: V(0,ParserBasicToken) = ag_rp_16(PCB_POINTER); break;
    case 17: V(0,ParserBasicToken) = ag_rp_17(PCB_POINTER); break;
    case 18: V(0,ParserQuotedString) = ag_rp_18(PCB_POINTER); break;
    case 19: V(0,ParserMethod) = ag_rp_19(PCB_POINTER); break;
    case 20: V(0,ParserMethod) = ag_rp_20(PCB_POINTER); break;
    case 21: V(0,ParserMethod) = ag_rp_21(PCB_POINTER); break;
    case 22: V(0,ParserMethod) = ag_rp_22(PCB_POINTER); break;
    case 23: V(0,ParserMethod) = ag_rp_23(PCB_POINTER); break;
    case 24: V(0,ParserMethod) = ag_rp_24(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 25: ag_rp_25(PCB_POINTER, V(0,ParserContentLength)); break;
    case 26: V(0,ParserContentLength) = ag_rp_26(PCB_POINTER); break;
    case 27: ag_rp_27(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 28: ag_rp_28(PCB_POINTER, V(0,ParserBasicToken), V(2,ParserGenericParamDataVal)); break;
    case 29: V(0,ParserGenericParamDataVal) = ag_rp_29(PCB_POINTER, V(2,ParserBasicToken)); break;
    case 30: V(0,ParserGenericParamDataVal) = ag_rp_30(PCB_POINTER, V(2,ParserQuotedString)); break;
    case 31: ag_rp_31(PCB_POINTER, V(0,ParserSipVersion), V(2,ParserStatusCode), V(4,ParserReasonPhrase)); break;
    case 32: ag_rp_32(PCB_POINTER, V(0,ParserSipVersion), V(2,ParserStatusCode)); break;
    case 33: V(0,ParserSipVersion) = ag_rp_33(PCB_POINTER); break;
    case 34: V(0,ParserReasonPhrase) = ag_rp_34(PCB_POINTER); break;
    case 35: V(0,ParserStatusCode) = ag_rp_35(PCB_POINTER, V(0,int), V(1,int), V(2,int)); break;
    case 36: ag_rp_36(PCB_POINTER); break;
    case 37: ag_rp_37(PCB_POINTER); break;
    case 38: ag_rp_38(PCB_POINTER); break;
    case 39: ag_rp_39(PCB_POINTER); break;
    case 40: ag_rp_40(PCB_POINTER, V(3,ParserViaParam)); break;
    case 41: ag_rp_41(PCB_POINTER, V(0,ParserProtocolName), V(4,ParserProtocolVersion), V(8,ParserTransport)); break;
    case 42: V(0,ParserProtocolName) = ag_rp_42(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 43: V(0,ParserProtocolName) = ag_rp_43(PCB_POINTER); break;
    case 44: V(0,ParserProtocolVersion) = ag_rp_44(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 45: V(0,ParserTransport) = ag_rp_45(PCB_POINTER, V(0,ParserTransport)); break;
    case 46: V(0,ParserTransport) = ag_rp_46(PCB_POINTER); break;
    case 47: V(0,ParserTransport) = ag_rp_47(PCB_POINTER); break;
    case 48: V(0,ParserTransport) = ag_rp_48(PCB_POINTER); break;
    case 49: V(0,ParserTransport) = ag_rp_49(PCB_POINTER); break;
    case 50: V(0,ParserTransport) = ag_rp_50(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 51: V(0,ParserPort) = ag_rp_51(PCB_POINTER); break;
    case 52: ag_rp_52(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 53: ag_rp_53(PCB_POINTER, V(0,ParserBasicToken), V(4,ParserPort)); break;
    case 54: V(0,ParserViaParam) = ag_rp_54(PCB_POINTER); break;
    case 55: V(0,ParserViaParam) = ag_rp_55(PCB_POINTER); break;
    case 56: V(0,ParserViaParam) = ag_rp_56(PCB_POINTER, V(0,ParserTTLParam)); break;
    case 57: V(0,ParserViaParam) = ag_rp_57(PCB_POINTER, V(0,ParserMaddrParam)); break;
    case 58: V(0,ParserViaParam) = ag_rp_58(PCB_POINTER, V(0,ParserReceived)); break;
    case 59: V(0,ParserViaParam) = ag_rp_59(PCB_POINTER, V(0,ParserBranch)); break;
    case 60: V(0,ParserViaParam) = ag_rp_60(PCB_POINTER, V(0,ParserPort)); break;
    case 61: V(0,ParserViaParam) = ag_rp_61(PCB_POINTER, V(0,ParserCompParam)); break;
    case 62: V(0,ParserViaParam) = ag_rp_62(PCB_POINTER); break;
    case 63: ag_rp_63(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 64: ag_rp_64(PCB_POINTER, V(0,ParserBasicToken), V(4,ParserBasicToken)); break;
    case 65: ag_rp_65(PCB_POINTER, V(0,ParserBasicToken), V(4,ParserQuotedString)); break;
    case 66: ag_rp_66(PCB_POINTER, V(0,ParserBasicToken), V(4,ParserBasicToken)); break;
    case 67: V(0,ParserTTLParam) = ag_rp_67(PCB_POINTER, V(0,ParserTtlParam)); break;
    case 68: V(0,ParserTtlParam) = ag_rp_68(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 69: V(0,ParserBasicToken) = ag_rp_69(PCB_POINTER); break;
    case 70: V(0,ParserBasicToken) = ag_rp_70(PCB_POINTER); break;
    case 71: V(0,ParserBasicToken) = ag_rp_71(PCB_POINTER); break;
    case 72: V(0,ParserPort) = ag_rp_72(PCB_POINTER, V(1,ParserPort)); break;
    case 73: V(0,ParserPort) = ag_rp_73(PCB_POINTER); break;
    case 74: V(0,ParserPort) = ag_rp_74(PCB_POINTER, V(2,ParserPort)); break;
    case 75: V(0,ParserMaddrParam) = ag_rp_75(PCB_POINTER, V(0,ParserMaddrParam)); break;
    case 76: V(0,ParserMaddrParam) = ag_rp_76(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 77: V(0,ParserReceived) = ag_rp_77(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 78: V(0,ParserBranch) = ag_rp_78(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 79: V(0,ParserCompParam) = ag_rp_79(PCB_POINTER, V(4,ParserCompParam)); break;
    case 80: V(0,ParserCompParam) = ag_rp_80(PCB_POINTER); break;
    case 81: V(0,ParserCompParam) = ag_rp_81(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 82: V(0,ParserBasicToken) = ag_rp_82(PCB_POINTER); break;
    case 83: ag_rp_83(PCB_POINTER, V(0,ParserAbsoluteUri)); break;
    case 84: V(0,ParserAbsoluteUri) = ag_rp_84(PCB_POINTER, V(0,ParserBasicToken), V(3,ParserBasicToken)); break;
    case 85: V(0,ParserBasicToken) = ag_rp_85(PCB_POINTER); break;
    case 86: V(0,ParserDisplayName) = ag_rp_86(PCB_POINTER, V(0,ParserDisplayName)); break;
    case 87: V(0,ParserBasicToken) = ag_rp_87(PCB_POINTER); break;
    case 88: V(0,ParserBasicToken) = ag_rp_88(PCB_POINTER); break;
    case 89: ag_rp_89(PCB_POINTER); break;
    case 90: ag_rp_90(PCB_POINTER, V(0,ParserAbsoluteUri)); break;
    case 91: ag_rp_91(PCB_POINTER, V(0,ParserAbsoluteUri)); break;
    case 92: ag_rp_92(PCB_POINTER); break;
    case 93: ag_rp_93(PCB_POINTER); break;
    case 94: V(0,ParserAbsoluteUri) = ag_rp_94(PCB_POINTER, V(2,ParserAbsoluteUri)); break;
    case 95: V(0,ParserAbsoluteUri) = ag_rp_95(PCB_POINTER); break;
    case 96: ag_rp_96(PCB_POINTER); break;
    case 97: ag_rp_97(PCB_POINTER); break;
    case 98: V(0,ParserAbsoluteUri) = ag_rp_98(PCB_POINTER, V(2,ParserAbsoluteUri)); break;
    case 99: V(0,ParserAbsoluteUri) = ag_rp_99(PCB_POINTER); break;
    case 100: ag_rp_100(PCB_POINTER); break;
    case 101: ag_rp_101(PCB_POINTER, V(2,ParserOptionalHeaders)); break;
    case 102: ag_rp_102(PCB_POINTER, V(2,ParserOptionalHeaders)); break;
    case 103: ag_rp_103(PCB_POINTER, V(3,ParserOptionalHeaders)); break;
    case 104: ag_rp_104(PCB_POINTER, V(3,ParserOptionalHeaders)); break;
    case 105: ag_rp_105(PCB_POINTER, V(3,ParserOptionalHeaders)); break;
    case 106: ag_rp_106(PCB_POINTER, V(3,ParserOptionalHeaders)); break;
    case 107: ag_rp_107(PCB_POINTER, V(3,ParserOptionalHeaders)); break;
    case 108: ag_rp_108(PCB_POINTER, V(3,ParserOptionalHeaders)); break;
    case 109: ag_rp_109(PCB_POINTER, V(0,ParserDisplayName), V(2,ParserBasicToken)); break;
    case 110: ag_rp_110(PCB_POINTER); break;
    case 111: ag_rp_111(PCB_POINTER); break;
    case 112: ag_rp_112(PCB_POINTER); break;
    case 113: ag_rp_113(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 114: ag_rp_114(PCB_POINTER, V(0,ParserBasicToken), V(2,ParserBasicToken)); break;
    case 115: V(0,ParserBasicToken) = ag_rp_115(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 116: V(0,ParserBasicToken) = ag_rp_116(PCB_POINTER); break;
    case 117: V(0,ParserBasicToken) = ag_rp_117(PCB_POINTER); break;
    case 118: ag_rp_118(PCB_POINTER); break;
    case 119: ag_rp_119(PCB_POINTER); break;
    case 120: ag_rp_120(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 121: ag_rp_121(PCB_POINTER, V(0,ParserBasicToken), V(2,ParserPort)); break;
    case 122: V(0,ParserBasicToken) = ag_rp_122(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 123: V(0,ParserBasicToken) = ag_rp_123(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 124: V(0,ParserBasicToken) = ag_rp_124(PCB_POINTER); break;
    case 125: V(0,ParserBasicToken) = ag_rp_125(PCB_POINTER); break;
    case 126: V(0,ParserBasicToken) = ag_rp_126(PCB_POINTER); break;
    case 127: ag_rp_127(PCB_POINTER); break;
    case 128: ag_rp_128(PCB_POINTER); break;
    case 129: ag_rp_129(PCB_POINTER); break;
    case 130: ag_rp_130(PCB_POINTER); break;
    case 131: ag_rp_131(PCB_POINTER, V(0,ParserTransport)); break;
    case 132: ag_rp_132(PCB_POINTER, V(0,ParserUserParam)); break;
    case 133: ag_rp_133(PCB_POINTER, V(0,ParserTtlParam)); break;
    case 134: ag_rp_134(PCB_POINTER, V(0,ParserMaddrParam)); break;
    case 135: ag_rp_135(PCB_POINTER, V(0,ParserMethod)); break;
    case 136: ag_rp_136(PCB_POINTER, V(0,ParserLrParamType)); break;
    case 137: ag_rp_137(PCB_POINTER, V(0,ParserCompParam)); break;
    case 138: ag_rp_138(PCB_POINTER, V(0,ParserTokenizedByParam)); break;
    case 139: ag_rp_139(PCB_POINTER, V(0,RvBool)); break;
    case 140: ag_rp_140(PCB_POINTER); break;
    case 141: V(0,ParserTransport) = ag_rp_141(PCB_POINTER, V(4,ParserTransport)); break;
    case 142: V(0,ParserUserParam) = ag_rp_142(PCB_POINTER, V(4,RvSipUserParam)); break;
    case 143: V(0,ParserUserParam) = ag_rp_143(PCB_POINTER, V(4,ParserOtherUser)); break;
    case 144: V(0,RvSipUserParam) = ag_rp_144(PCB_POINTER); break;
    case 145: V(0,RvSipUserParam) = ag_rp_145(PCB_POINTER); break;
    case 146: V(0,ParserOtherUser) = ag_rp_146(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 147: V(0,ParserTtlParam) = ag_rp_147(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 148: V(0,ParserMaddrParam) = ag_rp_148(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 149: V(0,ParserMethod) = ag_rp_149(PCB_POINTER, V(4,ParserMethod)); break;
    case 150: V(0,ParserLrParamType) = ag_rp_150(PCB_POINTER); break;
    case 151: V(0,ParserLrParamType) = ag_rp_151(PCB_POINTER); break;
    case 152: V(0,ParserLrParamType) = ag_rp_152(PCB_POINTER); break;
    case 153: V(0,ParserLrParamType) = ag_rp_153(PCB_POINTER); break;
    case 154: V(0,ParserCompParam) = ag_rp_154(PCB_POINTER, V(4,ParserCompParam)); break;
    case 155: V(0,ParserTokenizedByParam) = ag_rp_155(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 156: V(0,RvBool) = ag_rp_156(PCB_POINTER); break;
    case 157: ag_rp_157(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 158: ag_rp_158(PCB_POINTER, V(0,ParserBasicToken), V(1,ParserBasicToken)); break;
    case 159: V(0,ParserBasicToken) = ag_rp_159(PCB_POINTER); break;
    case 160: V(0,ParserBasicToken) = ag_rp_160(PCB_POINTER, V(2,ParserBasicToken)); break;
    case 161: V(0,ParserOptionalHeaders) = ag_rp_161(PCB_POINTER); break;
    case 162: V(0,ParserOptionalHeaders) = ag_rp_162(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 163: V(0,ParserBasicToken) = ag_rp_163(PCB_POINTER); break;
    case 164: V(0,ParserBasicToken) = ag_rp_164(PCB_POINTER); break;
    case 165: V(0,ParserBasicToken) = ag_rp_165(PCB_POINTER); break;
    case 166: V(0,ParserBasicToken) = ag_rp_166(PCB_POINTER); break;
    case 167: V(0,ParserBasicToken) = ag_rp_167(PCB_POINTER); break;
    case 168: ag_rp_168(PCB_POINTER); break;
    case 169: ag_rp_169(PCB_POINTER, V(0,ParserMethod), V(4,ParserSipVersion)); break;
    case 170: V(0,ParserMethod) = ag_rp_170(PCB_POINTER); break;
    case 171: V(0,ParserMethod) = ag_rp_171(PCB_POINTER); break;
    case 172: V(0,ParserMethod) = ag_rp_172(PCB_POINTER); break;
    case 173: V(0,ParserMethod) = ag_rp_173(PCB_POINTER); break;
    case 174: V(0,ParserMethod) = ag_rp_174(PCB_POINTER); break;
    case 175: V(0,ParserMethod) = ag_rp_175(PCB_POINTER); break;
    case 176: V(0,ParserMethod) = ag_rp_176(PCB_POINTER); break;
    case 177: V(0,ParserMethod) = ag_rp_177(PCB_POINTER); break;
    case 178: V(0,ParserMethod) = ag_rp_178(PCB_POINTER); break;
    case 179: V(0,ParserMethod) = ag_rp_179(PCB_POINTER); break;
    case 180: V(0,ParserMethod) = ag_rp_180(PCB_POINTER); break;
    case 181: V(0,ParserMethod) = ag_rp_181(PCB_POINTER); break;
    case 182: V(0,ParserMethod) = ag_rp_182(PCB_POINTER); break;
    case 183: V(0,ParserMethod) = ag_rp_183(PCB_POINTER, V(0,ParserMethod)); break;
    case 184: ag_rp_184(PCB_POINTER); break;
    case 185: ag_rp_185(PCB_POINTER, V(0,ParserAbsoluteUri)); break;
    case 186: ag_rp_186(PCB_POINTER, V(0,ParserAbsoluteUri)); break;
    case 187: ag_rp_187(PCB_POINTER, V(0,ParserAbsoluteUri)); break;
    case 188: ag_rp_188(PCB_POINTER, V(0,ParserDisplayName)); break;
    case 189: ag_rp_189(PCB_POINTER); break;
    case 190: V(0,ParserDisplayName) = ag_rp_190(PCB_POINTER, V(0,ParserQuotedString)); break;
    case 191: V(0,ParserDisplayName) = ag_rp_191(PCB_POINTER, V(0,ParserBasicToken)); break;
    case 192: V(0,ParserBasicToken) = ag_rp_192(PCB_POINTER); break;
    case 193: V(0,ParserBasicToken) = ag_rp_193(PCB_POINTER); break;
    case 194: V(0,ParserTagParam) = ag_rp_194(PCB_POINTER, V(4,ParserBasicToken)); break;
    case 195: V(0,ParserPartyParams) = ag_rp_195(PCB_POINTER, V(2,ParserPartyParams)); break;
    case 196: V(0,ParserPartyParams) = ag_rp_196(PCB_POINTER, V(0,ParserPartyParams), V(3,ParserPartyParams)); break;
    case 197: V(0,ParserPartyParams) = ag_rp_197(PCB_POINTER, V(0,ParserTagParam)); break;
    case 198: V(0,ParserPartyParams) = ag_rp_198(PCB_POINTER); break;
    case 199: ag_rp_199(PCB_POINTER, V(0,ParserMethod)); break;
    case 200: ag_rp_200(PCB_POINTER, V(0,ParserMethod), V(4,ParserBasicToken)); break;
    case 201: ag_rp_201(PCB_POINTER); break;
    case 202: ag_rp_202(PCB_POINTER); break;
    case 203: ag_rp_203(PCB_POINTER); break;
    case 204: ag_rp_204(PCB_POINTER); break;
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
  if ((PCB).ssx >= 26) {
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
  if ((PCB).ssx >= 26) {
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
    if ((PCB).ssx >= 26) {
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
    if ((PCB).ssx >= 26) {
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
