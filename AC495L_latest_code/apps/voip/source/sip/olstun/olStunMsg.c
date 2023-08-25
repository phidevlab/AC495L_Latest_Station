/*--------------------------------------------------------------------
*
* Copyright (c) 2007 Outline Electronics Ltd.
*
* All rights reserved. Reproduction in whole
* or in part is prohibited without the written
* permission of the copyright owner.
*--------------------------------------------------------------------
* File: olStunMsg.c
*
* Description:
* Construct Stun request and process the Stun response. 
*
* Notes:
* Modified from the previous version.
*
*--------------------------------------------------------------------
*--------------------------------------------------------------------*/
#define LOGSRC	LOGSRC_USERAPP

#include <netinet/in.h>
#include "IppStdInc.h"
#include "olStunTypes.h"


#define STUN_MAX_STRING 256
#define STUN_MAX_UNKNOWN_ATTRIBUTES 8
#define STUN_MAX_MESSAGE_SIZE 2048

/////////////////////
// Stun Basic Types
/////////////////////

typedef struct
{
    RvUint8 pad;
    RvUint8 family;
	RvUint16 port;
} StunAddrHdr;

typedef struct
{
    StunAddrHdr addrHdr;
    union
    {
	RvUint32  v4addr;
	RvUint128 v6addr;
    } addr;
} StunAddress;

typedef struct
{
      RvUint16 msgType;
      RvUint16 msgLength;
      RvUint128 id;
} StunMsgHdr;

typedef struct
{
      RvUint16 type;
      RvUint16 length;
} StunAtrHdr;

typedef struct
{
      RvUint16 port;
      RvUint32 addr;
} StunAddress4;

/////////////////////
// Stun Advance Types
/////////////////////

typedef struct
{
	RvUint16 type;
	RvUint16 length;
	StunAddress address;
} StunAtrAddress;

typedef struct
{
	RvUint16 type;
	RvUint16 length;
    StunAddrHdr addrHdr;
	RvUint32  v4addr;
} StunAtrAddress4;

typedef struct
{
	RvUint16 type;
	RvUint16 length;
    StunAddrHdr addrHdr;
    RvUint128 v6addr;
} StunAtrAddress6;


typedef struct
{
	RvUint16 type;
	RvUint16 length;
	RvUint32 value;
} StunAtrFlags;


typedef struct
{
	RvUint16 type;
	RvUint16 length;
	RvUint16 errorClass;
	RvUint16 number;
	//char         reason[1];
} StunAtrError;

typedef struct
{
	RvUint16 type;
	RvUint16 length;
	//char         value[1];
} StunAtrString;

typedef struct
{
	RvUint16 type;
	RvUint16 length;
	RvUint8 hash[20];
} StunAtrIntegrity;

// define the response msg
typedef struct
{
      StunMsgHdr      msgHdr;
 	  StunAtrFlags    flag;
 	  
	  StunAtrAddress4 mappedAddr;
      StunAtrAddress4 changeAddr;	// keith no change address for MIP
      StunAtrAddress4 sourceAddr;	// keith no source address for MIP
      StunAtrAddress4 responseAddr;
} Stun4Response;


// define the a simple request
typedef struct
{
      StunMsgHdr    msgHdr;
      StunAtrFlags  flag;
} StunRequestSimple;

// define  stun attribute
const RvUint16 MappedAddress = 0x0001;
const RvUint16 ResponseAddress = 0x0002;
const RvUint16 Flags = 0x0003;
const RvUint16 SourceAddress = 0x0004;
const RvUint16 ChangedAddress = 0x0005;
const RvUint16 Username = 0x0006;
const RvUint16 Password = 0x0007;
const RvUint16 MessageIntegrity = 0x0008;
const RvUint16 ErrorCode = 0x0009;

// define types for a stun message 
const RvUint16 BindRequestMsg          = 0x0001;
const RvUint16 BindResponseMsg         = 0x0101;
const RvUint16 BindErrorMsg            = 0x0111;
const RvUint16 SharedSecretRequestMsg  = 0x0002;
const RvUint16 SharedSecretResponseMsg = 0x0102;

// define  flags
const RvInt32 ChangeIpFlag   = 1<<0;
const RvInt32 ChangePortFlag = 1<<1;
const RvInt32 DiscardFlag    = 1<<2;

const RvUint8  IPv4Family = 0x01;
const RvUint8  IPv6Family = 0x02;


#define  ACL_OLSTUN_MSG_DEBUG

#ifdef ACL_OLSTUN_MSG_DEBUG 
#define DBG_PRINT(a1,a2...)	IppLogMessage(RV_FALSE, "[%s:%4d]" a1,__FUNCTION__, __LINE__, ##a2);
#else
#define DBG_PRINT(a1,a2...)
#endif


#if 0
static void printStunMsg(char *msg, int len)
{
	int i;
	
	for (i = 0; i < len; i++) {
		if ((i % 20) == 0)
			printf("\n");
		printf("%#x ",msg[i]);
	}
}
#endif

static void stunAddrGetIP4Addr(StunAddress *addr, RvUint32 *ip, RvUint16 *port)
{
	*ip = addr->addr.v4addr;
	*port = addr->addrHdr.port;
}

static RvBool stunParseAtrAddress( RvBool verbose, char* body, int hdrLen,  StunAddress* result )
{
	int i;
	
	if ( (unsigned int)hdrLen < sizeof(StunAtrAddress4)-4 )
	{
		IppLogMessage(RV_TRUE, "[STUN]: hdrLen wrong for Address");
		return RV_FALSE;
	}
	StunAtrAddress* attribute = (StunAtrAddress *)(body);

	if ( attribute->address.addrHdr.family == IPv4Family )
	{
		if ( hdrLen != sizeof(StunAtrAddress4)-4 )
		{
			IppLogMessage(RV_TRUE, "[STUN]: hdrLen wrong for v4 Address");
			return RV_FALSE;
		}

		StunAtrAddress4* attr = (StunAtrAddress4 *)(body);

		if (result)
		{
			result->addrHdr.port   = ntohs(attr->addrHdr.port);
			result->addrHdr.family = IPv4Family;
			//result->addr.v4addr    = ntohl(attr->v4addr); ACL NB 071108
			result->addr.v4addr    = attr->v4addr;
         DBG_PRINT("attr->v4addr        = 0x%x", attr->v4addr);  //ACL NB 071108 
         DBG_PRINT("result->addr.v4addr = 0x%x", result->addr.v4addr);  //ACL NB 071108 

		}

		return RV_TRUE;
	}

	if ( attribute->address.addrHdr.family == IPv6Family )
	{
		if ( hdrLen != sizeof(StunAtrAddress6)-4 )
		{
			IppLogMessage(RV_TRUE, "[STUN]: hdrLen wrong for v6 Address");
			return RV_FALSE;
		}

		StunAtrAddress6* attr = (StunAtrAddress6 *)(body);

		if (result)
		{
			result->addrHdr.port   = ntohs(attr->addrHdr.port);
			result->addrHdr.family = IPv6Family;
			for ( i=0; i<16; i++ )
			{
				result->addr.v6addr.octet[i] = attr->v6addr.octet[i];
			}
		}

		return RV_TRUE;
	}

	IppLogMessage(RV_TRUE, "[STUN]: must be IPv4 or IPv6 addr");
	return RV_FALSE;
}



// returns true if it is a valid message 
static RvBool
stunParseResponse( char *buf, unsigned int bufLen, RvBool verbose,
		   RvUint128 *id,
		   StunAddress *mappedAddr,
		   StunAddress *changedAddr,
		   StunAddress *sourceAddr,
		   StunAddress *responseAddr,
		   RvUint32 *flags,
		   RvUint16 *msgType)
{
	int i;
	
	if (id == NULL) {
		IppLogMessage(RV_TRUE, "[STUN]: No ID");
		return RV_FALSE;
   	}
	if (buf == NULL) {
		IppLogMessage(RV_TRUE, "[STUN]: Buf is null");
		return RV_FALSE;
	}
	
	if ( mappedAddr  ) memset(mappedAddr,  0, sizeof(StunAddress));
	if ( changedAddr ) memset(changedAddr, 0, sizeof(StunAddress));
	if ( sourceAddr  ) memset(sourceAddr,  0, sizeof(StunAddress));
	if ( responseAddr) memset(responseAddr,0, sizeof(StunAddress));
  
	if ( bufLen < sizeof(StunMsgHdr) )
	{
		IppLogMessage(RV_TRUE, "[STUN]: Unreasonable size (bufLen=%d) sizeof(StunMsgHdr=%d)", bufLen, sizeof(StunMsgHdr));
		return RV_FALSE;
	}

	StunMsgHdr* hdr = (StunMsgHdr *)(buf);
	
	RvUint16 mType = ntohs(hdr->msgType);

	if ( (mType != BindResponseMsg) && (mType != BindRequestMsg) )
	{
		IppLogMessage(RV_TRUE, "[STUN]: Incorrect mtype %d buf1 %s", mType,buf);
		return RV_FALSE;
	}
	
	if ( msgType )
	{
		*msgType = mType;
	}
	else
	{
		if ( mType != BindResponseMsg ) 
		{
			IppLogMessage(RV_TRUE, "[STUN]: Incorrect mtype %d buf1 %s", mType,buf);
			return RV_FALSE;
		}
	}

	if ( ntohs(hdr->msgLength)+sizeof(StunMsgHdr) > bufLen )
	{
		return RV_FALSE;
	}

	if (id)
	{
		for (i=0; i<16; i++)
		{
			id->octet[i] = hdr->id.octet[i];
		}
	}

	char* body        = buf + sizeof(StunMsgHdr);
	
	unsigned int size = ntohs(hdr->msgLength);

	while ( size > 0 )
	{
		if ( size < sizeof(StunAtrHdr) )
		{
			return RV_FALSE;
		}

		StunAtrHdr* attr = (StunAtrHdr *)(body);
		
		unsigned int hdrLen = ntohs(attr->length);
		RvUint16 atrType = ntohs(attr->type);

		/*NirM, The if statement will always return FALSE */
		/*if ( hdrLen < 0 ) 
		{
			IppLogMessage(RV_TRUE, "[STUN]: weird attribute header size (attribute type = %#x)", atrType);
			return RV_FALSE;
		}*/
		
		if ( hdrLen+4 > size ) 
		{
			IppLogMessage(RV_TRUE, "[STUN]: claims attribute is larger than size of message attribute type = %#x)", atrType);
			return RV_FALSE;
		}

		if  ( atrType == MappedAddress)
		{
         DBG_PRINT("mappedAddr = %s");  //ACL NB 071108 
         if ( stunParseAtrAddress(  verbose,  body,  hdrLen,  mappedAddr ) == RV_FALSE )
			{
		
				IppLogMessage(RV_TRUE, "[STUN]: problem parsing MappedAddress");		
		  		return RV_FALSE;
			}
		}
		else if ( atrType == ResponseAddress )
		{
		         DBG_PRINT("ResponseAddress = %s");  //ACL NB 071108 

			if ( stunParseAtrAddress(  verbose,  body,  hdrLen,  responseAddr )== RV_FALSE )
			{
				IppLogMessage(RV_TRUE, "[STUN]: problem parsing ResponseAddress");
				return RV_FALSE;
			}
		}
		else if ( atrType == SourceAddress )
		{
				         DBG_PRINT("SourceAddress = %s");  //ACL NB 071108 

			if ( stunParseAtrAddress(  verbose,  body,  hdrLen,  sourceAddr )== RV_FALSE )
			{
				IppLogMessage(RV_TRUE, "[STUN]: problem parsing SourceAddress");
				return RV_FALSE;
			}
		}
		else if ( atrType == ChangedAddress )
		{
		         DBG_PRINT("ChangedAddress = %s");  //ACL NB 071108 
		
			if ( stunParseAtrAddress(  verbose,  body,  hdrLen,  changedAddr )== RV_FALSE )
			{
				IppLogMessage(RV_TRUE, "[STUN]: problem parsing ChangedAddress");
				return RV_FALSE;
			}
		}
		else if ( atrType == Flags )
		{
			if ( hdrLen != sizeof(StunAtrFlags)-4 )
			{
				IppLogMessage(RV_TRUE, "[STUN]: incorrect Flags type");
				return RV_FALSE;
			}
		
			StunAtrFlags* attr = (StunAtrFlags *)(body);
		
			if ( flags )
			{
				*flags = ntohl(attr->value);
			}	    
		}
		else
		{
			if ( atrType <= 0x7FFF )
			{
				IppLogMessage(RV_TRUE, "[STUN]: incorrect atrType");
				return RV_FALSE;
			}
		}
		
		body += hdrLen+4;
		size -= hdrLen+4;
	}
    
   	return RV_TRUE;
}

RvBool
olStunParseForMappedAddr(
			IN char *buf,
			IN unsigned int size,
			OUT RvUint128 *id,
			OUT RvUint32 *mappedIp,
			OUT RvUint16 *mappedPort)
{
	StunAddress mappedAddr;
	StunAddress changedAddr;
	StunAddress sourceAddr;
	StunAddress responseAddr;
	RvUint32 flags;
	RvUint16 msgType;
	RvBool result;
	
	result = stunParseResponse(buf, size, RV_FALSE, id, &mappedAddr, 
				&changedAddr, &sourceAddr, &responseAddr, &flags, &msgType);
				
	if (result == RV_TRUE) {
		stunAddrGetIP4Addr(&mappedAddr, mappedIp, mappedPort);
	}

	return result;
}

static void stunBuildQuery( StunRequestSimple* msg,
		    	RvBool changePort, RvBool changeIp, RvBool discard, RvUint128 *resId)
{
	static RvBool init = RV_FALSE;
	int i;
	unsigned int id;
	
	if (init == RV_FALSE) {
		srand((unsigned int)clock());
		init = RV_TRUE;
	}

   msg->msgHdr.msgType = htons(BindRequestMsg);

   msg->msgHdr.msgLength = htons( sizeof(StunRequestSimple)-sizeof(StunMsgHdr) );
   

	for (i = 0; i < 16; i += 4)
	{
		id = rand();
		msg->msgHdr.id.octet[i]   = resId->octet[i]   = id; 
		msg->msgHdr.id.octet[i+1] = resId->octet[i+1] = id>>8;
		msg->msgHdr.id.octet[i+2] = resId->octet[i+2] = id>>16; 
		msg->msgHdr.id.octet[i+3] = resId->octet[i+3] = id>>24;
	}

   msg->flag.type = htons(Flags);
   msg->flag.length = htons( sizeof(StunAtrFlags)-sizeof(StunAtrHdr) );
   msg->flag.value = htonl( (changeIp?ChangeIpFlag:0) | 
                            (changePort?ChangePortFlag:0) | 
                            (discard?DiscardFlag:0) );
}

RvChar *olStunBuildRequest(RvBool changePort, RvBool changeIp, RvBool discard, RvUint128 *resId, RvInt *size)
{
	StunRequestSimple* msg;
	
	msg = (StunRequestSimple *)malloc(sizeof(StunRequestSimple));
	if (msg != NULL) {
		stunBuildQuery(msg, changePort, changeIp, discard, resId);
		*size = sizeof(StunRequestSimple);
	}
	else
		*size = 0;

	return (RvChar *)msg;
}

