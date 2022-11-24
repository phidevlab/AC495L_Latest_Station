#if !defined(RV_SIP_IMS_DCS_HEADER_SUPPORT) && !defined(RV_SIP_IMS_HEADER_SUPPORT) && !defined(RV_SIP_EXTENDED_HEADER_SUPPORT) && !defined(RV_SIP_JSR32_SUPPORT) && !defined(RV_SIP_LIGHT) && !defined(RV_SIP_TEL_URI_SUPPORT) && !defined(RV_SIP_PRIMITIVES) && !defined(RV_SIP_AUTH_ON) && defined(RV_SIP_SUBS_ON)
#ifndef PARSERENGINECLASSIC_H_1165245778
#define PARSERENGINECLASSIC_H_1165245778

typedef union {
  long alignment;
  char ag_vt_2[sizeof(int)];
  char ag_vt_4[sizeof(ParserBasicToken)];
  char ag_vt_5[sizeof(ParserQuotedString)];
  char ag_vt_6[sizeof(ParserMethod)];
  char ag_vt_7[sizeof(ParserCSeq)];
  char ag_vt_8[sizeof(ParserSequenceNumber)];
  char ag_vt_9[sizeof(ParserContentLength)];
  char ag_vt_10[sizeof(ParserCallId)];
  char ag_vt_11[sizeof(ParserMediaType)];
  char ag_vt_12[sizeof(ParserMediaSubType)];
  char ag_vt_13[sizeof(ParserBoundary)];
  char ag_vt_14[sizeof(ParserBase)];
  char ag_vt_15[sizeof(ParserGenericParamDataVal)];
  char ag_vt_16[sizeof(ParserSipVersion)];
  char ag_vt_17[sizeof(ParserStatusCode)];
  char ag_vt_18[sizeof(ParserReasonPhrase)];
  char ag_vt_19[sizeof(ParserProtocolName)];
  char ag_vt_20[sizeof(ParserProtocolVersion)];
  char ag_vt_21[sizeof(ParserTransport)];
  char ag_vt_22[sizeof(ParserPort)];
  char ag_vt_23[sizeof(ParserViaParam)];
  char ag_vt_24[sizeof(ParserHidden)];
  char ag_vt_25[sizeof(ParserAlias)];
  char ag_vt_26[sizeof(ParserTTLParam)];
  char ag_vt_27[sizeof(ParserTtlParam)];
  char ag_vt_28[sizeof(ParserMaddrParam)];
  char ag_vt_29[sizeof(ParserReceived)];
  char ag_vt_30[sizeof(ParserBranch)];
  char ag_vt_31[sizeof(ParserCompParam)];
  char ag_vt_32[sizeof(ParserAbsoluteUri)];
  char ag_vt_33[sizeof(ParserDisplayName)];
  char ag_vt_34[sizeof(ParserUserParam)];
  char ag_vt_35[sizeof(RvSipUserParam)];
  char ag_vt_36[sizeof(ParserOtherUser)];
  char ag_vt_37[sizeof(ParserLrParamType)];
  char ag_vt_38[sizeof(ParserTokenizedByParam)];
  char ag_vt_39[sizeof(RvBool)];
  char ag_vt_40[sizeof(ParserOptionalHeaders)];
  char ag_vt_41[sizeof(ParserTagParam)];
  char ag_vt_42[sizeof(ParserPartyParams)];
  char ag_vt_43[sizeof(ParserQVal)];
  char ag_vt_44[sizeof(ParserContactActionType)];
  char ag_vt_45[sizeof(ParserDeltaSeconds)];
  char ag_vt_46[sizeof(ParserSipDate)];
  char ag_vt_47[sizeof(RvSipDateWeekDay)];
  char ag_vt_48[sizeof(ParserDDMMYY)];
  char ag_vt_49[sizeof(ParserInt32)];
  char ag_vt_50[sizeof(RvSipDateMonth)];
  char ag_vt_51[sizeof(ParserTime)];
  char ag_vt_52[sizeof(ParserTimeZone)];
  char ag_vt_53[sizeof(ParserRetryParam)];
  char ag_vt_54[sizeof(ParserRSeq)];
  char ag_vt_55[sizeof(ParserResponseNum)];
  char ag_vt_56[sizeof(ParserEventType)];
  char ag_vt_57[sizeof(ParserSubsStateValue)];
  char ag_vt_58[sizeof(ParserSubsStateReason)];
  char ag_vt_59[sizeof(ParserDisposition)];
  char ag_vt_60[sizeof(ParserDispositionParams)];
  char ag_vt_61[sizeof(ParserDispositionParam)];
  char ag_vt_62[sizeof(ParserSessionExpiresHeader)];
  char ag_vt_63[sizeof(RvSipSessionExpiresRefresherType)];
  char ag_vt_64[sizeof(ParserMinSEHeader)];
  char ag_vt_65[sizeof(ParserReplacesEarlyFlagParamType)];
} SipParser_vs_type;

typedef enum {
  SipParser_START_token = 1, SipParser_FIRST_COLON_token = 3,
  SipParser_CSEQ_HEADER_token, SipParser_NULL_token,
  SipParser_PARTY_HEADER_token = 7, SipParser_EXPIRES_HEADER_token = 9,
  SipParser_SIP_DATE_HEADER_token = 11, SipParser_CONTACT_HEADER_token = 13,
  SipParser_CALL_ID_HEADER_token = 15, SipParser_CONTENT_LENGTH_token = 17,
  SipParser_VIA_HEADER_token = 19, SipParser_ROUTE_HOP_HEADER_token = 21,
  SipParser_ALLOW_HEADER_token = 23, SipParser_RACK_HEADER_token = 25,
  SipParser_RSEQ_HEADER_token = 27, SipParser_SUPPORTED_HEADER_token = 29,
  SipParser_OPTION_TAG_BASE_HEADER_token = 31,
  SipParser_CONTENT_DISPOSITION_HEADER_token = 33,
  SipParser_CONTENT_TYPE_HEADER_token = 35,
  SipParser_RETRY_AFTER_HEADER_token = 37,
  SipParser_SESSION_EXPIRES_HEADER_token = 39,
  SipParser_MIN_SE_HEADER_token = 41, SipParser_REPLACES_HEADER_token = 43,
  SipParser_REFER_TO_HEADER_token = 45,
  SipParser_REFERRED_BY_HEADER_token = 47,
  SipParser_EVENT_HEADER_token = 49,
  SipParser_ALLOW_EVENTS_HEADER_token = 51,
  SipParser_SUBSCRIPTION_STATE_HEADER_token = 53,
  SipParser_STATUS_LINE_token, SipParser_REQUEST_LINE_token,
  SipParser_ABSOLUTE_URI_HEADER_token = 57, SipParser_PRES_URI_HEADER_token,
  SipParser_IM_URI_HEADER_token, SipParser_SIP_URL_HEADER_token,
  SipParser_OTHER_HEADER_token, SipParser_WHITE_token = 63,
  SipParser_SP_token = 65, SipParser_COMMA_token = 67,
  SipParser_COMMA_BETWEEN_HEADERS_token = 69, SipParser_DOT_token,
  SipParser_EQUAL_token = 74, SipParser_AT_token = 76,
  SipParser_COLON_token = 78, SipParser_SEMI_COLON_token,
  SipParser_MARK_SIGN_token = 81, SipParser_SLASH_token = 83,
  SipParser_LEFT_PARENTHESIS_token = 85,
  SipParser_LEFT_ANGLE_BRACKET_token = 87,
  SipParser_LEFT_QUOTATION_token = 89, SipParser_TOKEN_token = 91,
  SipParser_TOKEN_CHAR_token, SipParser_CALL_ID_WORD_token = 94,
  SipParser_CALL_ID_WORD_CHAR_token,
  SipParser_TOKEN_FOR_CONTENT_TYPE_token = 97,
  SipParser_TOKEN_CHAR_FOR_CONTENT_TYPE_token,
  SipParser_TOKEN_NO_DOT_token = 100, SipParser_TOKEN_CHAR_NO_DOT_token,
  SipParser_FIELD_VALUE_token = 103, SipParser_QUOTED_STRING_token = 109,
  SipParser_QUOTED_CHAR_token, SipParser_METHOD_token = 117,
  SipParser_CSEQ_VAL_token = 127, SipParser_SEQUNCE_NUMBER_token,
  SipParser_DIGIT_token, SipParser_CONTENT_LENGTH_VAL_token = 131,
  SipParser_CALL_ID_token, SipParser_MEDIA_TYPE_token,
  SipParser_MEDIA_TYPE_TYPE_token, SipParser_MEDIA_TYPE_SUB_TYPE_token,
  SipParser_OPTIONAL_CONTENT_TYPE_PARAM_LIST_token,
  SipParser_CONTENT_TYPE_PARAM_LIST_token, SipParser_BOUNDARY_PARAM_token,
  SipParser_VERSION_PARAM_token, SipParser_BASE_PARAM_token,
  SipParser_CONTENT_TYPE_GENERIC_PARAM_DATA_token,
  SipParser_CONTENT_TYPE_GENERIC_PARAM_DATA_VAL_token = 161,
  SipParser_GENERIC_PARAM_LIST_token, SipParser_GENERIC_PARAM_DATA_token,
  SipParser_GENERIC_PARAM_DATA_VAL_token, SipParser_SIP_VERSION_token,
  SipParser_STATUS_CODE_token = 167, SipParser_REASON_PHRASE_token,
  SipParser_EXTENSION_CODE_token = 170, SipParser_REASONE_TEXT_UTF8_token,
  SipParser_ALLOW_VAL_token = 175, SipParser_VIA_LIST_token,
  SipParser_SINGLE_VIA_token, SipParser_SENT_PROTOCOL_token,
  SipParser_SENT_BY_token, SipParser_VIA_PARAM_LIST_token,
  SipParser_VIA_PARAM_token, SipParser_PROTOCOL_NAME_token,
  SipParser_PROTOCOL_VERSION_token, SipParser_TRANSPORT_token,
  SipParser_TRANSPORT_TYPE_token = 186, SipParser_PORT_token = 191,
  SipParser_HOST_NAME_token, SipParser_VIA_HIDDEN_token,
  SipParser_VIA_ALIAS_token, SipParser_VIA_TTL_token,
  SipParser_VIA_MADDR_token, SipParser_VIA_RECEIVED_token,
  SipParser_VIA_BRANCH_token, SipParser_RESPONSE_PORT_token,
  SipParser_VIA_COMPRESSION_token, SipParser_OTHER_PARAM_token,
  SipParser_OTHER_PARAM_EXPR_token, SipParser_IPV_6_REFERENCE_token,
  SipParser_TTL_PARAM_token = 206, SipParser_TTL_token = 208,
  SipParser_RPORT_NAME_token, SipParser_RPORT_VAL_token,
  SipParser_MADDR_PARAM_token = 212,
  SipParser_HOST_NAME_FOR_RECEIVED_token = 215,
  SipParser_COMPRESSION_TYPE_token = 218,
  SipParser_IPV_6_ADDRESS_token = 220, SipParser_TEST_COMMENT_token,
  SipParser_COMMENT_TEXT_token, SipParser_END_COMMENT_token = 224,
  SipParser_REQUEST_URI_token = 227, SipParser_ADDR_SPEC_token,
  SipParser_ABSOLUTE_URI_token, SipParser_ABS_URI_SCHEME_token,
  SipParser_URI_IDENTIFIER_token, SipParser_ABS_URI_SCHEME_CHAR_token,
  SipParser_URI_SCHEME_OR_DISPLAY_NAME_token = 234,
  SipParser_DISPLAY_NAME_token, SipParser_URI_IDENTIFIER_CHAR_token,
  SipParser_URI_IDENTIFIER_WITHOUT_SEPARATORS_token = 238,
  SipParser_SIP_URL_token = 241, SipParser_PRES_URI_token,
  SipParser_IM_URI_token, SipParser_SIP_PREFIX_token, SipParser_URL_token,
  SipParser_PRES_PREFIX_token = 248, SipParser_RV_ABS_PREFIX_token,
  SipParser_IM_PREFIX_token = 252, SipParser_URL_USERINFO_token = 254,
  SipParser_OPTIONAL_URL_PARAMETERS_token, SipParser_OPTIONAL_HEADERS_token,
  SipParser_URL_HOST_token, SipParser_GENERAL_URL_WITHOUT_PARAM_token,
  SipParser_SIP_URL_WITHOUT_PARAMS_token,
  SipParser_PRES_URL_WITHOUT_PARAMS_token,
  SipParser_IM_URL_WITHOUT_PARAMS_token,
  SipParser_ABS_URI_WITHOUT_PARAMS_token,
  SipParser_URL_USERINFO_PREFIX_token, SipParser_URL_USERINFO_VAL_token,
  SipParser_HOSTPORT_token, SipParser_USER_INFO_TOKEN_token = 267,
  SipParser_PASSWORD_token, SipParser_TOKEN_USER_INFO_token,
  SipParser_TOKEN_USER_INFO_CHAR_token,
  SipParser_PASSWORD_STRING_token = 272,
  SipParser_PASSWORD_STRING_CHAR_token,
  SipParser_URL_HOST_PREFIX_token = 277, SipParser_TOKEN_HOST_token = 279,
  SipParser_ALPHANUM_token, SipParser_TOKEN_HOST_CHAR_token,
  SipParser_IPV_4_ADDRESS_token = 285, SipParser_IPV_4_PART_token,
  SipParser_HEXPART_token, SipParser_HEXSEQ_token,
  SipParser_HEX4_token = 291, SipParser_HEX_token = 295,
  SipParser_URL_PARAMETERS_token, SipParser_URL_PARAMETER_token,
  SipParser_TRANSPORT_PARAM_token, SipParser_USER_PARAM_token,
  SipParser_TTL_PARAM_URL_token, SipParser_MADDR_PARAM_URL_token,
  SipParser_METHOD_PARAM_URL_token, SipParser_LR_PARAM_URL_token,
  SipParser_COMPRESSION_PARAM_URL_token,
  SipParser_TOKENIZED_BY_PARAM_URL_token, SipParser_ORIG_PARAM_URL_token,
  SipParser_URL_GENERIC_PARAM_DATA_token,
  SipParser_USER_PARAM_TYPE_token = 310, SipParser_OTHER_USER_token,
  SipParser_URL_OTHER_PARAM_VAL_token = 323, SipParser_PVALUE_token,
  SipParser_PARAMCHAR_token, SipParser_HEADERS_LIST_token = 329,
  SipParser_HEADER_token, SipParser_HNAME_token = 332,
  SipParser_HVALUE_token, SipParser_REQUEST_LINE_VALUE_token = 340,
  SipParser_REQUEST_METHOD_token, SipParser_PARTY_VALUE_token = 355,
  SipParser_NAME_ADDR_token, SipParser_PARTY_PARAMS_LIST_token,
  SipParser_MORE_THAN_ONE_TOKEN_token = 359,
  SipParser_TAG_PARAM_token = 363, SipParser_PARTY_PARAM_DATA_token = 365,
  SipParser_CONTACT_HEADER_VALUES_token,
  SipParser_CONTACT_HEADER_OTHER_token,
  SipParser_CONTACT_HEADER_VALUE_LIST_token,
  SipParser_CONTACT_HEADER_VALUE_token = 370,
  SipParser_CONTACT_PARAMS_token, SipParser_CONTACT_PARAM_token,
  SipParser_Q_token, SipParser_ACTION_token,
  SipParser_CONTACT_EXPIRES_token, SipParser_Q_VALUE_token = 377,
  SipParser_Q_VALUE_OPTIONAL_token = 379,
  SipParser_ACTION_VALUE_token = 383, SipParser_EXPIRES_VALUE_token = 386,
  SipParser_EXPIRES_VALUE_FOR_CONTACT_token = 388,
  SipParser_DELTA_SECONDS_token, SipParser_SIP_DATE_token,
  SipParser_SIP_DATE_HEADER_VAL_token, SipParser_DATE_token,
  SipParser_TIME_token, SipParser_DAY_token, SipParser_DD_token = 402,
  SipParser_MM_token, SipParser_YY_token, SipParser_HOUR_token = 417,
  SipParser_MINUTE_token, SipParser_SECOND_token, SipParser_TIME_ZONE_token,
  SipParser_ROUTE_LIST_token = 424, SipParser_SINGLE_ROUTE_token,
  SipParser_RETRY_AFTER_VAL_token, SipParser_RETRY_AFTER_COMMENT_token,
  SipParser_RETRY_PARAM_token = 429, SipParser_DURATION_PARAM_token,
  SipParser_RSEQ_VAL_token = 432, SipParser_RESPONSE_NUM_token,
  SipParser_RACK_VAL_token, SipParser_OPTION_TAG_token,
  SipParser_OPTION_TAG_LIST_token, SipParser_SUPPORTED_OPTION_TAG_token,
  SipParser_REFER_TO_HEADER_VALUE_token,
  SipParser_REFERRED_BY_HEADER_VALUE_token = 440,
  SipParser_REFERRER_URI_token, SipParser_REFERRED_BY_PARAMS_token,
  SipParser_REFERRED_BY_PARAM_token = 444,
  SipParser_REFERRED_BY_PARAM_CID_token,
  SipParser_CID_PARAM_STRING_token = 447, SipParser_DOT_ATOM_token,
  SipParser_ATOM_CHAR_token, SipParser_DOT_ATOM_CHAR_token,
  SipParser_EVENT_HEADER_VAL_token = 453, SipParser_EVENT_TYPE_token,
  SipParser_EVENT_PARAMS_token, SipParser_EVENT_PACKAGE_token,
  SipParser_EVENT_TEMPLATE_token, SipParser_EVENT_PARAM_token,
  SipParser_EVENT_ID_token, SipParser_ALLOW_EVENTS_HEADER_VAL_token = 461,
  SipParser_SUBSCRIPTION_STATE_HEADER_VAL_token,
  SipParser_SUBSTATE_VALUE_token, SipParser_SUBSTATE_PARAMS_token,
  SipParser_SUBSTATE_PARAM_token = 468,
  SipParser_SUBSTATE_PARAM_REASON_token,
  SipParser_SUBSTATE_PARAM_RETRY_AFTER_token,
  SipParser_SUBSTATE_PARAM_EXPIRES_token,
  SipParser_SUBSTATE_PARAM_REASON_VAL_token = 474,
  SipParser_DISPOSITION_VAL_token = 481, SipParser_DISPOSITION_TYPE_token,
  SipParser_DISPOSITION_PARAMS_token, SipParser_HANDLING_PARAM_token = 489,
  SipParser_SESSION_EXPIRES_VALUE_token = 493,
  SipParser_REFRESHER_TYPE_token, SipParser_REFRESHER_TYPE_NAME_token,
  SipParser_REFRESHER_TYPE_VALUE_token, SipParser_MIN_SE_VALUE_token = 500,
  SipParser_REPLACES_HEADER_VALUE_token, SipParser_REPLACES_PARAMS_token,
  SipParser_REPLACES_PARAM_token, SipParser_TO_TAG_PARAM_token,
  SipParser_FROM_TAG_PARAM_token, SipParser_EARLY_FLAG_PARAM_token
} SipParser_token_type;

typedef struct SipParser_pcb_struct{
  SipParser_token_type token_number, reduction_token, error_frame_token;
  int input_code;
  int input_value;
  int line, column;
  int ssx, sn, error_frame_ssx;
  int drt, dssx, dsn;
  int ss[28];
  SipParser_vs_type vs[28];
  int ag_ap;
  char *error_message;
  char read_flag;
  char exit_flag;
  unsigned char * pointer;
  unsigned char * la_ptr;
  const unsigned char *key_sp;
  int save_index, key_state;
  const  int *ag_dtl;
  int ag_dsn;
/*  Line 67, RV SIP Stack */
        ParserMgr              *pParserMgr;     
        void                   *pSipObject;     


        SipParseType            eHeaderType;    



        SipParseType            eWhichHeader;   



        RvChar                 *pBuffer;        


                  
        RvStatus                eStat;          
        RvChar                 *pCurToken;      
        void                   *pExtParams;     

        void                   *pUrlExtParams;  
                                              
        RvChar                 *pCurLine;       
		RvInt32					lineNumber;     
       
        RvBool                 isCompactForm;  

        RvBool                 isNewChallenge; 

        RvUint32               parenCount;     
        
        








        ParserContactHeader     contact;        
        ParserExpiresHeader     expires;        
        ParserPartyHeader       party;          

        ParserRequestLine       requestLine;    
        ParserRouteHeader       route;          
        ParserNameAddr          nameAddr;       
        ParserExUri             exUri;          
        ParserSipUrl            sipUrl;         
		ParserSingleVia         singleVia;      
        ParserContentType       contentType;    
        ParserUrlParameter      urlParameter;   






		ParserSentProtocol      sentProtocol;   









        ParserReferredByHeader     referredBy;  

        ParserReferToHeader        referTo;     
		ParserEvent				   event;       
        ParserSubscriptionState    subsState;   

        ParserRetryAfter           retryAfter;  
        ParserContentDisposition   contentDisposition; 
        ParserRack                 rack;        
        ParserReplacesParams       replacesParams; 
        ParserReplacesParam        replacesParam;  

        ParserSentByInVia          viaSentBy;       




























	} SipParser_pcb_type;

#ifndef PRULE_CONTEXT
#define PRULE_CONTEXT(pcb)  (&((pcb).cs[(pcb).ssx]))
#define PERROR_CONTEXT(pcb) ((pcb).cs[(pcb).error_frame_ssx])
#define PCONTEXT(pcb)       ((pcb).cs[(pcb).ssx])
#endif

#ifndef AG_RUNNING_CODE_CODE
/* PCB.exit_flag values */
#define AG_RUNNING_CODE         0
#define AG_SUCCESS_CODE         1
#define AG_SYNTAX_ERROR_CODE    2
#define AG_REDUCTION_ERROR_CODE 3
#define AG_STACK_ERROR_CODE     4
#define AG_SEMANTIC_ERROR_CODE  5
#endif
void init_SipParser(SipParser_pcb_type *);
void SipParser(SipParser_pcb_type *);
#endif

#elif !defined(RV_SIP_IMS_DCS_HEADER_SUPPORT) && !defined(RV_SIP_IMS_HEADER_SUPPORT) && !defined(RV_SIP_EXTENDED_HEADER_SUPPORT) && !defined(RV_SIP_JSR32_SUPPORT) && !defined(RV_SIP_LIGHT) && !defined(RV_SIP_TEL_URI_SUPPORT) && !defined(RV_SIP_PRIMITIVES) && defined(RV_SIP_AUTH_ON) && !defined(RV_SIP_SUBS_ON)
#ifndef PARSERENGINECLASSIC_H_1165245782
#define PARSERENGINECLASSIC_H_1165245782

typedef union {
  long alignment;
  char ag_vt_2[sizeof(int)];
  char ag_vt_4[sizeof(ParserBasicToken)];
  char ag_vt_5[sizeof(ParserQuotedString)];
  char ag_vt_6[sizeof(ParserMethod)];
  char ag_vt_7[sizeof(ParserCSeq)];
  char ag_vt_8[sizeof(ParserSequenceNumber)];
  char ag_vt_9[sizeof(ParserContentLength)];
  char ag_vt_10[sizeof(ParserCallId)];
  char ag_vt_11[sizeof(ParserMediaType)];
  char ag_vt_12[sizeof(ParserMediaSubType)];
  char ag_vt_13[sizeof(ParserBoundary)];
  char ag_vt_14[sizeof(ParserBase)];
  char ag_vt_15[sizeof(ParserGenericParamDataVal)];
  char ag_vt_16[sizeof(ParserSipVersion)];
  char ag_vt_17[sizeof(ParserStatusCode)];
  char ag_vt_18[sizeof(ParserReasonPhrase)];
  char ag_vt_19[sizeof(ParserProtocolName)];
  char ag_vt_20[sizeof(ParserProtocolVersion)];
  char ag_vt_21[sizeof(ParserTransport)];
  char ag_vt_22[sizeof(ParserPort)];
  char ag_vt_23[sizeof(ParserViaParam)];
  char ag_vt_24[sizeof(ParserHidden)];
  char ag_vt_25[sizeof(ParserAlias)];
  char ag_vt_26[sizeof(ParserTTLParam)];
  char ag_vt_27[sizeof(ParserTtlParam)];
  char ag_vt_28[sizeof(ParserMaddrParam)];
  char ag_vt_29[sizeof(ParserReceived)];
  char ag_vt_30[sizeof(ParserBranch)];
  char ag_vt_31[sizeof(ParserCompParam)];
  char ag_vt_32[sizeof(ParserAbsoluteUri)];
  char ag_vt_33[sizeof(ParserDisplayName)];
  char ag_vt_34[sizeof(ParserUserParam)];
  char ag_vt_35[sizeof(RvSipUserParam)];
  char ag_vt_36[sizeof(ParserOtherUser)];
  char ag_vt_37[sizeof(ParserLrParamType)];
  char ag_vt_38[sizeof(ParserTokenizedByParam)];
  char ag_vt_39[sizeof(RvBool)];
  char ag_vt_40[sizeof(ParserOptionalHeaders)];
  char ag_vt_41[sizeof(ParserTagParam)];
  char ag_vt_42[sizeof(ParserPartyParams)];
  char ag_vt_43[sizeof(ParserQVal)];
  char ag_vt_44[sizeof(ParserContactActionType)];
  char ag_vt_45[sizeof(ParserDeltaSeconds)];
  char ag_vt_46[sizeof(ParserSipDate)];
  char ag_vt_47[sizeof(RvSipDateWeekDay)];
  char ag_vt_48[sizeof(ParserDDMMYY)];
  char ag_vt_49[sizeof(ParserInt32)];
  char ag_vt_50[sizeof(RvSipDateMonth)];
  char ag_vt_51[sizeof(ParserTime)];
  char ag_vt_52[sizeof(ParserTimeZone)];
  char ag_vt_53[sizeof(RvSipAuthStale)];
  char ag_vt_54[sizeof(ParserAlgorithm)];
  char ag_vt_55[sizeof(ParserQopValue)];
  char ag_vt_56[sizeof(ParserAuthQopOptions)];
  char ag_vt_57[sizeof(RvSipAuthQopOption)];
  char ag_vt_58[sizeof(RvSipAuthIntegrityProtected)];
  char ag_vt_59[sizeof(ParserRetryParam)];
  char ag_vt_60[sizeof(ParserRSeq)];
  char ag_vt_61[sizeof(ParserResponseNum)];
  char ag_vt_62[sizeof(ParserDisposition)];
  char ag_vt_63[sizeof(ParserDispositionParams)];
  char ag_vt_64[sizeof(ParserDispositionParam)];
  char ag_vt_65[sizeof(ParserSessionExpiresHeader)];
  char ag_vt_66[sizeof(RvSipSessionExpiresRefresherType)];
  char ag_vt_67[sizeof(ParserMinSEHeader)];
  char ag_vt_68[sizeof(ParserReplacesEarlyFlagParamType)];
} SipParser_vs_type;

typedef enum {
  SipParser_START_token = 1, SipParser_FIRST_COLON_token = 3,
  SipParser_CSEQ_HEADER_token, SipParser_NULL_token,
  SipParser_PARTY_HEADER_token = 7, SipParser_EXPIRES_HEADER_token = 9,
  SipParser_SIP_DATE_HEADER_token = 11, SipParser_CONTACT_HEADER_token = 13,
  SipParser_CALL_ID_HEADER_token = 15, SipParser_CONTENT_LENGTH_token = 17,
  SipParser_VIA_HEADER_token = 19, SipParser_ROUTE_HOP_HEADER_token = 21,
  SipParser_AUTHENTICATION_HEADER_token = 23,
  SipParser_AUTHORIZATION_HEADER_token = 25,
  SipParser_ALLOW_HEADER_token = 27, SipParser_RACK_HEADER_token = 29,
  SipParser_RSEQ_HEADER_token = 31, SipParser_SUPPORTED_HEADER_token = 33,
  SipParser_OPTION_TAG_BASE_HEADER_token = 35,
  SipParser_CONTENT_DISPOSITION_HEADER_token = 37,
  SipParser_CONTENT_TYPE_HEADER_token = 39,
  SipParser_RETRY_AFTER_HEADER_token = 41,
  SipParser_SESSION_EXPIRES_HEADER_token = 43,
  SipParser_MIN_SE_HEADER_token = 45, SipParser_REPLACES_HEADER_token = 47,
  SipParser_AUTHENTICATION_INFO_HEADER_token = 49,
  SipParser_STATUS_LINE_token, SipParser_REQUEST_LINE_token,
  SipParser_ABSOLUTE_URI_HEADER_token = 53, SipParser_PRES_URI_HEADER_token,
  SipParser_IM_URI_HEADER_token, SipParser_SIP_URL_HEADER_token,
  SipParser_OTHER_HEADER_token, SipParser_WHITE_token = 59,
  SipParser_SP_token = 61, SipParser_COMMA_token = 63,
  SipParser_COMMA_BETWEEN_HEADERS_token = 65, SipParser_EQUAL_token = 68,
  SipParser_AT_token = 70, SipParser_COLON_token = 72,
  SipParser_SEMI_COLON_token, SipParser_MARK_SIGN_token = 75,
  SipParser_SLASH_token = 77, SipParser_LEFT_PARENTHESIS_token = 79,
  SipParser_LEFT_ANGLE_BRACKET_token = 81,
  SipParser_LEFT_QUOTATION_token = 83, SipParser_HYPHEN_token = 85,
  SipParser_TOKEN_token = 87, SipParser_TOKEN_CHAR_token,
  SipParser_CALL_ID_WORD_token = 90, SipParser_CALL_ID_WORD_CHAR_token,
  SipParser_TOKEN_FOR_CONTENT_TYPE_token = 93,
  SipParser_TOKEN_CHAR_FOR_CONTENT_TYPE_token,
  SipParser_FIELD_VALUE_token = 96, SipParser_QUOTED_STRING_token = 102,
  SipParser_QUOTED_CHAR_token, SipParser_HEX_VALUE_token = 110,
  SipParser_HEX_token, SipParser_METHOD_token = 113,
  SipParser_CSEQ_VAL_token = 120, SipParser_SEQUNCE_NUMBER_token,
  SipParser_DIGIT_token, SipParser_CONTENT_LENGTH_VAL_token = 124,
  SipParser_CALL_ID_token, SipParser_MEDIA_TYPE_token,
  SipParser_MEDIA_TYPE_TYPE_token, SipParser_MEDIA_TYPE_SUB_TYPE_token,
  SipParser_OPTIONAL_CONTENT_TYPE_PARAM_LIST_token,
  SipParser_CONTENT_TYPE_PARAM_LIST_token, SipParser_BOUNDARY_PARAM_token,
  SipParser_VERSION_PARAM_token, SipParser_BASE_PARAM_token,
  SipParser_CONTENT_TYPE_GENERIC_PARAM_DATA_token,
  SipParser_CONTENT_TYPE_GENERIC_PARAM_DATA_VAL_token = 154,
  SipParser_GENERIC_PARAM_LIST_token, SipParser_GENERIC_PARAM_DATA_token,
  SipParser_GENERIC_PARAM_DATA_VAL_token, SipParser_SIP_VERSION_token,
  SipParser_STATUS_CODE_token = 160, SipParser_REASON_PHRASE_token,
  SipParser_EXTENSION_CODE_token = 163, SipParser_REASONE_TEXT_UTF8_token,
  SipParser_ALLOW_VAL_token = 168, SipParser_VIA_LIST_token,
  SipParser_SINGLE_VIA_token, SipParser_SENT_PROTOCOL_token,
  SipParser_SENT_BY_token, SipParser_VIA_PARAM_LIST_token,
  SipParser_VIA_PARAM_token, SipParser_PROTOCOL_NAME_token,
  SipParser_PROTOCOL_VERSION_token, SipParser_TRANSPORT_token,
  SipParser_TRANSPORT_TYPE_token = 179, SipParser_PORT_token = 184,
  SipParser_HOST_NAME_token, SipParser_VIA_HIDDEN_token,
  SipParser_VIA_ALIAS_token, SipParser_VIA_TTL_token,
  SipParser_VIA_MADDR_token, SipParser_VIA_RECEIVED_token,
  SipParser_VIA_BRANCH_token, SipParser_RESPONSE_PORT_token,
  SipParser_VIA_COMPRESSION_token, SipParser_OTHER_PARAM_token,
  SipParser_OTHER_PARAM_EXPR_token, SipParser_IPV_6_REFERENCE_token,
  SipParser_TTL_PARAM_token = 199, SipParser_TTL_token = 201,
  SipParser_RPORT_NAME_token, SipParser_RPORT_VAL_token,
  SipParser_MADDR_PARAM_token = 205,
  SipParser_HOST_NAME_FOR_RECEIVED_token = 208,
  SipParser_COMPRESSION_TYPE_token = 211,
  SipParser_IPV_6_ADDRESS_token = 213, SipParser_TEST_COMMENT_token,
  SipParser_COMMENT_TEXT_token, SipParser_END_COMMENT_token = 217,
  SipParser_REQUEST_URI_token = 220, SipParser_ADDR_SPEC_token,
  SipParser_ABSOLUTE_URI_token, SipParser_ABS_URI_SCHEME_token,
  SipParser_URI_IDENTIFIER_token, SipParser_ABS_URI_SCHEME_CHAR_token,
  SipParser_URI_SCHEME_OR_DISPLAY_NAME_token = 227,
  SipParser_DISPLAY_NAME_token, SipParser_URI_IDENTIFIER_CHAR_token,
  SipParser_URI_IDENTIFIER_WITHOUT_SEPARATORS_token = 231,
  SipParser_SIP_URL_token = 234, SipParser_PRES_URI_token,
  SipParser_IM_URI_token, SipParser_SIP_PREFIX_token, SipParser_URL_token,
  SipParser_PRES_PREFIX_token = 241, SipParser_RV_ABS_PREFIX_token,
  SipParser_IM_PREFIX_token = 245, SipParser_URL_USERINFO_token = 247,
  SipParser_OPTIONAL_URL_PARAMETERS_token, SipParser_OPTIONAL_HEADERS_token,
  SipParser_URL_HOST_token, SipParser_GENERAL_URL_WITHOUT_PARAM_token,
  SipParser_SIP_URL_WITHOUT_PARAMS_token,
  SipParser_PRES_URL_WITHOUT_PARAMS_token,
  SipParser_IM_URL_WITHOUT_PARAMS_token,
  SipParser_ABS_URI_WITHOUT_PARAMS_token,
  SipParser_URL_USERINFO_PREFIX_token, SipParser_URL_USERINFO_VAL_token,
  SipParser_HOSTPORT_token, SipParser_USER_INFO_TOKEN_token = 260,
  SipParser_PASSWORD_token, SipParser_TOKEN_USER_INFO_token,
  SipParser_TOKEN_USER_INFO_CHAR_token,
  SipParser_PASSWORD_STRING_token = 265,
  SipParser_PASSWORD_STRING_CHAR_token,
  SipParser_URL_HOST_PREFIX_token = 270, SipParser_TOKEN_HOST_token = 272,
  SipParser_ALPHANUM_token, SipParser_TOKEN_HOST_CHAR_token,
  SipParser_IPV_4_ADDRESS_token = 278, SipParser_IPV_4_PART_token,
  SipParser_HEXPART_token = 281, SipParser_HEXSEQ_token,
  SipParser_HEX4_token = 285, SipParser_URL_PARAMETERS_token = 289,
  SipParser_URL_PARAMETER_token, SipParser_TRANSPORT_PARAM_token,
  SipParser_USER_PARAM_token, SipParser_TTL_PARAM_URL_token,
  SipParser_MADDR_PARAM_URL_token, SipParser_METHOD_PARAM_URL_token,
  SipParser_LR_PARAM_URL_token, SipParser_COMPRESSION_PARAM_URL_token,
  SipParser_TOKENIZED_BY_PARAM_URL_token, SipParser_ORIG_PARAM_URL_token,
  SipParser_URL_GENERIC_PARAM_DATA_token,
  SipParser_USER_PARAM_TYPE_token = 303, SipParser_OTHER_USER_token,
  SipParser_URL_OTHER_PARAM_VAL_token = 316, SipParser_PVALUE_token,
  SipParser_PARAMCHAR_token, SipParser_HEADERS_LIST_token = 322,
  SipParser_HEADER_token, SipParser_HNAME_token = 325,
  SipParser_HVALUE_token, SipParser_REQUEST_LINE_VALUE_token = 333,
  SipParser_REQUEST_METHOD_token, SipParser_PARTY_VALUE_token = 348,
  SipParser_NAME_ADDR_token, SipParser_PARTY_PARAMS_LIST_token,
  SipParser_MORE_THAN_ONE_TOKEN_token = 352,
  SipParser_TAG_PARAM_token = 356, SipParser_PARTY_PARAM_DATA_token = 358,
  SipParser_CONTACT_HEADER_VALUES_token,
  SipParser_CONTACT_HEADER_OTHER_token,
  SipParser_CONTACT_HEADER_VALUE_LIST_token,
  SipParser_CONTACT_HEADER_VALUE_token = 363,
  SipParser_CONTACT_PARAMS_token, SipParser_CONTACT_PARAM_token,
  SipParser_Q_token, SipParser_ACTION_token,
  SipParser_CONTACT_EXPIRES_token, SipParser_Q_VALUE_token = 370,
  SipParser_Q_VALUE_OPTIONAL_token = 372,
  SipParser_ACTION_VALUE_token = 376, SipParser_EXPIRES_VALUE_token = 379,
  SipParser_EXPIRES_VALUE_FOR_CONTACT_token = 381,
  SipParser_DELTA_SECONDS_token, SipParser_SIP_DATE_token,
  SipParser_SIP_DATE_HEADER_VAL_token, SipParser_DATE_token,
  SipParser_TIME_token, SipParser_DAY_token, SipParser_DD_token = 395,
  SipParser_MM_token, SipParser_YY_token, SipParser_HOUR_token = 410,
  SipParser_MINUTE_token, SipParser_SECOND_token, SipParser_TIME_ZONE_token,
  SipParser_AUTHENTICATION_VAL_token = 417, SipParser_CHALLENGE_VALUE_token,
  SipParser_DIGEST_CHALLENGE_LIST_token, SipParser_AUTH_PARAM_LIST_token,
  SipParser_DIGEST_CHALLENGE_token, SipParser_REALM_token,
  SipParser_DOMAIN_token, SipParser_NONCE_token, SipParser_OPAQUE_token,
  SipParser_INTEGRITY_KEY_token, SipParser_CIPHER_KEY_token,
  SipParser_AUTH_PARAM_token, SipParser_STALE_token,
  SipParser_ALGORITHM_token, SipParser_QOP_OPTIONS_token,
  SipParser_PARAM_IN_NEXT_CHALLENGE_token,
  SipParser_REALM_VALUE_token = 434, SipParser_DOMAIN_EQUAL_token = 436,
  SipParser_DOMAIN_WHITE_token, SipParser_DOMAIN_VAL_token = 439,
  SipParser_URI_LIST_token, SipParser_STRING_DOMAIN_PATCH_token,
  SipParser_DOMAIN_SPACE_token, SipParser_NONCE_VALUE_token = 445,
  SipParser_OPAQUE_VALUE_token = 447,
  SipParser_ALGORITHM_DECLARATION_token = 450,
  SipParser_ALGORITHM_AND_AKAV_token, SipParser_AKAV_PREFIX_token = 454,
  SipParser_AKAV_token, SipParser_AKA_VERSION_VAL_token,
  SipParser_ALGORITHM_VAL_token, SipParser_ALGORITHM_PREFIX_token,
  SipParser_QOP_VALUE_LIST_token = 466, SipParser_QOP_VALUE_token,
  SipParser_QUOTED_HEX_VALUE_token = 471,
  SipParser_AUTH_PARAM_DATA_token = 473, SipParser_AUTHORIZATION_VAL_token,
  SipParser_CREDENTIALS_VALUE_token, SipParser_DIGEST_RESPONSE_LIST_token,
  SipParser_DIGEST_RESPONSE_token, SipParser_USERNAME_token,
  SipParser_RESPONSE_token, SipParser_CNONCE_token,
  SipParser_NONCE_COUNT_token, SipParser_AUTS_PARAM_token,
  SipParser_INTEGRITY_PROTECTED_token, SipParser_MESSAGE_QOP_token,
  SipParser_DIGEST_URI_token, SipParser_USERNAME_VALUE_token = 487,
  SipParser_DIGEST_URI_VALUE_token = 489,
  SipParser_CNONCE_VALUE_token = 492, SipParser_NC_VALUE_token = 494,
  SipParser_LHEX_token, SipParser_REQUEST_DIGEST_token = 497,
  SipParser_THIRTY_TWO_LHEX_token,
  SipParser_SIXTY_FOUR_BASE_ENCODING_token = 502,
  SipParser_SIXTY_FOUR_BASE_ENCODING_CHAR_token,
  SipParser_ONE_OR_TWO_EQUAL_SIGNS_token = 505,
  SipParser_ROUTE_LIST_token = 512, SipParser_SINGLE_ROUTE_token,
  SipParser_RETRY_AFTER_VAL_token, SipParser_RETRY_AFTER_COMMENT_token,
  SipParser_RETRY_PARAM_token = 517, SipParser_DURATION_PARAM_token,
  SipParser_RSEQ_VAL_token = 520, SipParser_RESPONSE_NUM_token,
  SipParser_RACK_VAL_token, SipParser_OPTION_TAG_token,
  SipParser_OPTION_TAG_LIST_token, SipParser_SUPPORTED_OPTION_TAG_token,
  SipParser_DISPOSITION_VAL_token, SipParser_DISPOSITION_TYPE_token,
  SipParser_DISPOSITION_PARAMS_token, SipParser_HANDLING_PARAM_token = 534,
  SipParser_SESSION_EXPIRES_VALUE_token = 538,
  SipParser_REFRESHER_TYPE_token, SipParser_REFRESHER_TYPE_NAME_token = 541,
  SipParser_REFRESHER_TYPE_VALUE_token, SipParser_MIN_SE_VALUE_token = 546,
  SipParser_REPLACES_HEADER_VALUE_token, SipParser_REPLACES_PARAMS_token,
  SipParser_REPLACES_PARAM_token, SipParser_TO_TAG_PARAM_token,
  SipParser_FROM_TAG_PARAM_token, SipParser_EARLY_FLAG_PARAM_token,
  SipParser_AUTHENTICATION_INFO_HEADER_LIST_token = 556,
  SipParser_AUTHENTICATION_INFO_HEADER_PARAM_token,
  SipParser_NEXT_NONCE_token, SipParser_RESPONSE_AUTH_token,
  SipParser_MSG_QOP_token, SipParser_RESPONSE_AUTH_VAL_token = 563
} SipParser_token_type;

typedef struct SipParser_pcb_struct{
  SipParser_token_type token_number, reduction_token, error_frame_token;
  int input_code;
  int input_value;
  int line, column;
  int ssx, sn, error_frame_ssx;
  int drt, dssx, dsn;
  int ss[43];
  SipParser_vs_type vs[43];
  int ag_ap;
  char *error_message;
  char read_flag;
  char exit_flag;
  unsigned char * pointer;
  unsigned char * la_ptr;
  const unsigned char *key_sp;
  int save_index, key_state;
  const  int *ag_dtl;
  int ag_dsn;
/*  Line 67, RV SIP Stack */
        ParserMgr              *pParserMgr;     
        void                   *pSipObject;     


        SipParseType            eHeaderType;    



        SipParseType            eWhichHeader;   



        RvChar                 *pBuffer;        


                  
        RvStatus                eStat;          
        RvChar                 *pCurToken;      
        void                   *pExtParams;     

        void                   *pUrlExtParams;  
                                              
        RvChar                 *pCurLine;       
		RvInt32					lineNumber;     
       
        RvBool                 isCompactForm;  

        RvBool                 isNewChallenge; 

        RvUint32               parenCount;     
        
        








        ParserContactHeader     contact;        
        ParserExpiresHeader     expires;        
        ParserPartyHeader       party;          

        ParserRequestLine       requestLine;    
        ParserRouteHeader       route;          
        ParserNameAddr          nameAddr;       
        ParserExUri             exUri;          
        ParserSipUrl            sipUrl;         
		ParserSingleVia         singleVia;      
        ParserContentType       contentType;    
        ParserUrlParameter      urlParameter;   






		ParserSentProtocol      sentProtocol;   

		ParserAuthorizationHeader  authorization; 
        ParserAuthenticationHeader authentication;
        ParserDigestChallenge      digestChallenge;
        ParserAuthenticationInfoHeader auth_info;		










        ParserRetryAfter           retryAfter;  
        ParserContentDisposition   contentDisposition; 
        ParserRack                 rack;        
        ParserReplacesParams       replacesParams; 
        ParserReplacesParam        replacesParam;  

        ParserSentByInVia          viaSentBy;       




























	} SipParser_pcb_type;

#ifndef PRULE_CONTEXT
#define PRULE_CONTEXT(pcb)  (&((pcb).cs[(pcb).ssx]))
#define PERROR_CONTEXT(pcb) ((pcb).cs[(pcb).error_frame_ssx])
#define PCONTEXT(pcb)       ((pcb).cs[(pcb).ssx])
#endif

#ifndef AG_RUNNING_CODE_CODE
/* PCB.exit_flag values */
#define AG_RUNNING_CODE         0
#define AG_SUCCESS_CODE         1
#define AG_SYNTAX_ERROR_CODE    2
#define AG_REDUCTION_ERROR_CODE 3
#define AG_STACK_ERROR_CODE     4
#define AG_SEMANTIC_ERROR_CODE  5
#endif
void init_SipParser(SipParser_pcb_type *);
void SipParser(SipParser_pcb_type *);
#endif

#elif !defined(RV_SIP_IMS_DCS_HEADER_SUPPORT) && !defined(RV_SIP_IMS_HEADER_SUPPORT) && !defined(RV_SIP_EXTENDED_HEADER_SUPPORT) && !defined(RV_SIP_JSR32_SUPPORT) && !defined(RV_SIP_LIGHT) && !defined(RV_SIP_TEL_URI_SUPPORT) && !defined(RV_SIP_PRIMITIVES) && defined(RV_SIP_AUTH_ON) && defined(RV_SIP_SUBS_ON)
#ifndef PARSERENGINECLASSIC_H_1165245787
#define PARSERENGINECLASSIC_H_1165245787

typedef union {
  long alignment;
  char ag_vt_2[sizeof(int)];
  char ag_vt_4[sizeof(ParserBasicToken)];
  char ag_vt_5[sizeof(ParserQuotedString)];
  char ag_vt_6[sizeof(ParserMethod)];
  char ag_vt_7[sizeof(ParserCSeq)];
  char ag_vt_8[sizeof(ParserSequenceNumber)];
  char ag_vt_9[sizeof(ParserContentLength)];
  char ag_vt_10[sizeof(ParserCallId)];
  char ag_vt_11[sizeof(ParserMediaType)];
  char ag_vt_12[sizeof(ParserMediaSubType)];
  char ag_vt_13[sizeof(ParserBoundary)];
  char ag_vt_14[sizeof(ParserBase)];
  char ag_vt_15[sizeof(ParserGenericParamDataVal)];
  char ag_vt_16[sizeof(ParserSipVersion)];
  char ag_vt_17[sizeof(ParserStatusCode)];
  char ag_vt_18[sizeof(ParserReasonPhrase)];
  char ag_vt_19[sizeof(ParserProtocolName)];
  char ag_vt_20[sizeof(ParserProtocolVersion)];
  char ag_vt_21[sizeof(ParserTransport)];
  char ag_vt_22[sizeof(ParserPort)];
  char ag_vt_23[sizeof(ParserViaParam)];
  char ag_vt_24[sizeof(ParserHidden)];
  char ag_vt_25[sizeof(ParserAlias)];
  char ag_vt_26[sizeof(ParserTTLParam)];
  char ag_vt_27[sizeof(ParserTtlParam)];
  char ag_vt_28[sizeof(ParserMaddrParam)];
  char ag_vt_29[sizeof(ParserReceived)];
  char ag_vt_30[sizeof(ParserBranch)];
  char ag_vt_31[sizeof(ParserCompParam)];
  char ag_vt_32[sizeof(ParserAbsoluteUri)];
  char ag_vt_33[sizeof(ParserDisplayName)];
  char ag_vt_34[sizeof(ParserUserParam)];
  char ag_vt_35[sizeof(RvSipUserParam)];
  char ag_vt_36[sizeof(ParserOtherUser)];
  char ag_vt_37[sizeof(ParserLrParamType)];
  char ag_vt_38[sizeof(ParserTokenizedByParam)];
  char ag_vt_39[sizeof(RvBool)];
  char ag_vt_40[sizeof(ParserOptionalHeaders)];
  char ag_vt_41[sizeof(ParserTagParam)];
  char ag_vt_42[sizeof(ParserPartyParams)];
  char ag_vt_43[sizeof(ParserQVal)];
  char ag_vt_44[sizeof(ParserContactActionType)];
  char ag_vt_45[sizeof(ParserDeltaSeconds)];
  char ag_vt_46[sizeof(ParserSipDate)];
  char ag_vt_47[sizeof(RvSipDateWeekDay)];
  char ag_vt_48[sizeof(ParserDDMMYY)];
  char ag_vt_49[sizeof(ParserInt32)];
  char ag_vt_50[sizeof(RvSipDateMonth)];
  char ag_vt_51[sizeof(ParserTime)];
  char ag_vt_52[sizeof(ParserTimeZone)];
  char ag_vt_53[sizeof(RvSipAuthStale)];
  char ag_vt_54[sizeof(ParserAlgorithm)];
  char ag_vt_55[sizeof(ParserQopValue)];
  char ag_vt_56[sizeof(ParserAuthQopOptions)];
  char ag_vt_57[sizeof(RvSipAuthQopOption)];
  char ag_vt_58[sizeof(RvSipAuthIntegrityProtected)];
  char ag_vt_59[sizeof(ParserRetryParam)];
  char ag_vt_60[sizeof(ParserRSeq)];
  char ag_vt_61[sizeof(ParserResponseNum)];
  char ag_vt_62[sizeof(ParserEventType)];
  char ag_vt_63[sizeof(ParserSubsStateValue)];
  char ag_vt_64[sizeof(ParserSubsStateReason)];
  char ag_vt_65[sizeof(ParserDisposition)];
  char ag_vt_66[sizeof(ParserDispositionParams)];
  char ag_vt_67[sizeof(ParserDispositionParam)];
  char ag_vt_68[sizeof(ParserSessionExpiresHeader)];
  char ag_vt_69[sizeof(RvSipSessionExpiresRefresherType)];
  char ag_vt_70[sizeof(ParserMinSEHeader)];
  char ag_vt_71[sizeof(ParserReplacesEarlyFlagParamType)];
} SipParser_vs_type;

typedef enum {
  SipParser_START_token = 1, SipParser_FIRST_COLON_token = 3,
  SipParser_CSEQ_HEADER_token, SipParser_NULL_token,
  SipParser_PARTY_HEADER_token = 7, SipParser_EXPIRES_HEADER_token = 9,
  SipParser_SIP_DATE_HEADER_token = 11, SipParser_CONTACT_HEADER_token = 13,
  SipParser_CALL_ID_HEADER_token = 15, SipParser_CONTENT_LENGTH_token = 17,
  SipParser_VIA_HEADER_token = 19, SipParser_ROUTE_HOP_HEADER_token = 21,
  SipParser_AUTHENTICATION_HEADER_token = 23,
  SipParser_AUTHORIZATION_HEADER_token = 25,
  SipParser_ALLOW_HEADER_token = 27, SipParser_RACK_HEADER_token = 29,
  SipParser_RSEQ_HEADER_token = 31, SipParser_SUPPORTED_HEADER_token = 33,
  SipParser_OPTION_TAG_BASE_HEADER_token = 35,
  SipParser_CONTENT_DISPOSITION_HEADER_token = 37,
  SipParser_CONTENT_TYPE_HEADER_token = 39,
  SipParser_RETRY_AFTER_HEADER_token = 41,
  SipParser_SESSION_EXPIRES_HEADER_token = 43,
  SipParser_MIN_SE_HEADER_token = 45, SipParser_REPLACES_HEADER_token = 47,
  SipParser_REFER_TO_HEADER_token = 49,
  SipParser_REFERRED_BY_HEADER_token = 51,
  SipParser_EVENT_HEADER_token = 53,
  SipParser_ALLOW_EVENTS_HEADER_token = 55,
  SipParser_SUBSCRIPTION_STATE_HEADER_token = 57,
  SipParser_AUTHENTICATION_INFO_HEADER_token = 59,
  SipParser_STATUS_LINE_token, SipParser_REQUEST_LINE_token,
  SipParser_ABSOLUTE_URI_HEADER_token = 63, SipParser_PRES_URI_HEADER_token,
  SipParser_IM_URI_HEADER_token, SipParser_SIP_URL_HEADER_token,
  SipParser_OTHER_HEADER_token, SipParser_WHITE_token = 69,
  SipParser_SP_token = 71, SipParser_COMMA_token = 73,
  SipParser_COMMA_BETWEEN_HEADERS_token = 75, SipParser_DOT_token,
  SipParser_EQUAL_token = 80, SipParser_AT_token = 82,
  SipParser_COLON_token = 84, SipParser_SEMI_COLON_token,
  SipParser_MARK_SIGN_token = 87, SipParser_SLASH_token = 89,
  SipParser_LEFT_PARENTHESIS_token = 91,
  SipParser_LEFT_ANGLE_BRACKET_token = 93,
  SipParser_LEFT_QUOTATION_token = 95, SipParser_HYPHEN_token = 97,
  SipParser_TOKEN_token = 99, SipParser_TOKEN_CHAR_token,
  SipParser_CALL_ID_WORD_token = 102, SipParser_CALL_ID_WORD_CHAR_token,
  SipParser_TOKEN_FOR_CONTENT_TYPE_token = 105,
  SipParser_TOKEN_CHAR_FOR_CONTENT_TYPE_token,
  SipParser_TOKEN_NO_DOT_token = 108, SipParser_TOKEN_CHAR_NO_DOT_token,
  SipParser_FIELD_VALUE_token = 111, SipParser_QUOTED_STRING_token = 117,
  SipParser_QUOTED_CHAR_token, SipParser_HEX_VALUE_token = 125,
  SipParser_HEX_token, SipParser_METHOD_token = 128,
  SipParser_CSEQ_VAL_token = 138, SipParser_SEQUNCE_NUMBER_token,
  SipParser_DIGIT_token, SipParser_CONTENT_LENGTH_VAL_token = 142,
  SipParser_CALL_ID_token, SipParser_MEDIA_TYPE_token,
  SipParser_MEDIA_TYPE_TYPE_token, SipParser_MEDIA_TYPE_SUB_TYPE_token,
  SipParser_OPTIONAL_CONTENT_TYPE_PARAM_LIST_token,
  SipParser_CONTENT_TYPE_PARAM_LIST_token, SipParser_BOUNDARY_PARAM_token,
  SipParser_VERSION_PARAM_token, SipParser_BASE_PARAM_token,
  SipParser_CONTENT_TYPE_GENERIC_PARAM_DATA_token,
  SipParser_CONTENT_TYPE_GENERIC_PARAM_DATA_VAL_token = 172,
  SipParser_GENERIC_PARAM_LIST_token, SipParser_GENERIC_PARAM_DATA_token,
  SipParser_GENERIC_PARAM_DATA_VAL_token, SipParser_SIP_VERSION_token,
  SipParser_STATUS_CODE_token = 178, SipParser_REASON_PHRASE_token,
  SipParser_EXTENSION_CODE_token = 181, SipParser_REASONE_TEXT_UTF8_token,
  SipParser_ALLOW_VAL_token = 186, SipParser_VIA_LIST_token,
  SipParser_SINGLE_VIA_token, SipParser_SENT_PROTOCOL_token,
  SipParser_SENT_BY_token, SipParser_VIA_PARAM_LIST_token,
  SipParser_VIA_PARAM_token, SipParser_PROTOCOL_NAME_token,
  SipParser_PROTOCOL_VERSION_token, SipParser_TRANSPORT_token,
  SipParser_TRANSPORT_TYPE_token = 197, SipParser_PORT_token = 202,
  SipParser_HOST_NAME_token, SipParser_VIA_HIDDEN_token,
  SipParser_VIA_ALIAS_token, SipParser_VIA_TTL_token,
  SipParser_VIA_MADDR_token, SipParser_VIA_RECEIVED_token,
  SipParser_VIA_BRANCH_token, SipParser_RESPONSE_PORT_token,
  SipParser_VIA_COMPRESSION_token, SipParser_OTHER_PARAM_token,
  SipParser_OTHER_PARAM_EXPR_token, SipParser_IPV_6_REFERENCE_token,
  SipParser_TTL_PARAM_token = 217, SipParser_TTL_token = 219,
  SipParser_RPORT_NAME_token, SipParser_RPORT_VAL_token,
  SipParser_MADDR_PARAM_token = 223,
  SipParser_HOST_NAME_FOR_RECEIVED_token = 226,
  SipParser_COMPRESSION_TYPE_token = 229,
  SipParser_IPV_6_ADDRESS_token = 231, SipParser_TEST_COMMENT_token,
  SipParser_COMMENT_TEXT_token, SipParser_END_COMMENT_token = 235,
  SipParser_REQUEST_URI_token = 238, SipParser_ADDR_SPEC_token,
  SipParser_ABSOLUTE_URI_token, SipParser_ABS_URI_SCHEME_token,
  SipParser_URI_IDENTIFIER_token, SipParser_ABS_URI_SCHEME_CHAR_token,
  SipParser_URI_SCHEME_OR_DISPLAY_NAME_token = 245,
  SipParser_DISPLAY_NAME_token, SipParser_URI_IDENTIFIER_CHAR_token,
  SipParser_URI_IDENTIFIER_WITHOUT_SEPARATORS_token = 249,
  SipParser_SIP_URL_token = 252, SipParser_PRES_URI_token,
  SipParser_IM_URI_token, SipParser_SIP_PREFIX_token, SipParser_URL_token,
  SipParser_PRES_PREFIX_token = 259, SipParser_RV_ABS_PREFIX_token,
  SipParser_IM_PREFIX_token = 263, SipParser_URL_USERINFO_token = 265,
  SipParser_OPTIONAL_URL_PARAMETERS_token, SipParser_OPTIONAL_HEADERS_token,
  SipParser_URL_HOST_token, SipParser_GENERAL_URL_WITHOUT_PARAM_token,
  SipParser_SIP_URL_WITHOUT_PARAMS_token,
  SipParser_PRES_URL_WITHOUT_PARAMS_token,
  SipParser_IM_URL_WITHOUT_PARAMS_token,
  SipParser_ABS_URI_WITHOUT_PARAMS_token,
  SipParser_URL_USERINFO_PREFIX_token, SipParser_URL_USERINFO_VAL_token,
  SipParser_HOSTPORT_token, SipParser_USER_INFO_TOKEN_token = 278,
  SipParser_PASSWORD_token, SipParser_TOKEN_USER_INFO_token,
  SipParser_TOKEN_USER_INFO_CHAR_token,
  SipParser_PASSWORD_STRING_token = 283,
  SipParser_PASSWORD_STRING_CHAR_token,
  SipParser_URL_HOST_PREFIX_token = 288, SipParser_TOKEN_HOST_token = 290,
  SipParser_ALPHANUM_token, SipParser_TOKEN_HOST_CHAR_token,
  SipParser_IPV_4_ADDRESS_token = 296, SipParser_IPV_4_PART_token,
  SipParser_HEXPART_token, SipParser_HEXSEQ_token,
  SipParser_HEX4_token = 302, SipParser_URL_PARAMETERS_token = 306,
  SipParser_URL_PARAMETER_token, SipParser_TRANSPORT_PARAM_token,
  SipParser_USER_PARAM_token, SipParser_TTL_PARAM_URL_token,
  SipParser_MADDR_PARAM_URL_token, SipParser_METHOD_PARAM_URL_token,
  SipParser_LR_PARAM_URL_token, SipParser_COMPRESSION_PARAM_URL_token,
  SipParser_TOKENIZED_BY_PARAM_URL_token, SipParser_ORIG_PARAM_URL_token,
  SipParser_URL_GENERIC_PARAM_DATA_token,
  SipParser_USER_PARAM_TYPE_token = 320, SipParser_OTHER_USER_token,
  SipParser_URL_OTHER_PARAM_VAL_token = 333, SipParser_PVALUE_token,
  SipParser_PARAMCHAR_token, SipParser_HEADERS_LIST_token = 339,
  SipParser_HEADER_token, SipParser_HNAME_token = 342,
  SipParser_HVALUE_token, SipParser_REQUEST_LINE_VALUE_token = 350,
  SipParser_REQUEST_METHOD_token, SipParser_PARTY_VALUE_token = 365,
  SipParser_NAME_ADDR_token, SipParser_PARTY_PARAMS_LIST_token,
  SipParser_MORE_THAN_ONE_TOKEN_token = 369,
  SipParser_TAG_PARAM_token = 373, SipParser_PARTY_PARAM_DATA_token = 375,
  SipParser_CONTACT_HEADER_VALUES_token,
  SipParser_CONTACT_HEADER_OTHER_token,
  SipParser_CONTACT_HEADER_VALUE_LIST_token,
  SipParser_CONTACT_HEADER_VALUE_token = 380,
  SipParser_CONTACT_PARAMS_token, SipParser_CONTACT_PARAM_token,
  SipParser_Q_token, SipParser_ACTION_token,
  SipParser_CONTACT_EXPIRES_token, SipParser_Q_VALUE_token = 387,
  SipParser_Q_VALUE_OPTIONAL_token = 389,
  SipParser_ACTION_VALUE_token = 393, SipParser_EXPIRES_VALUE_token = 396,
  SipParser_EXPIRES_VALUE_FOR_CONTACT_token = 398,
  SipParser_DELTA_SECONDS_token, SipParser_SIP_DATE_token,
  SipParser_SIP_DATE_HEADER_VAL_token, SipParser_DATE_token,
  SipParser_TIME_token, SipParser_DAY_token, SipParser_DD_token = 412,
  SipParser_MM_token, SipParser_YY_token, SipParser_HOUR_token = 427,
  SipParser_MINUTE_token, SipParser_SECOND_token, SipParser_TIME_ZONE_token,
  SipParser_AUTHENTICATION_VAL_token = 434, SipParser_CHALLENGE_VALUE_token,
  SipParser_DIGEST_CHALLENGE_LIST_token, SipParser_AUTH_PARAM_LIST_token,
  SipParser_DIGEST_CHALLENGE_token, SipParser_REALM_token,
  SipParser_DOMAIN_token, SipParser_NONCE_token, SipParser_OPAQUE_token,
  SipParser_INTEGRITY_KEY_token, SipParser_CIPHER_KEY_token,
  SipParser_AUTH_PARAM_token, SipParser_STALE_token,
  SipParser_ALGORITHM_token, SipParser_QOP_OPTIONS_token,
  SipParser_PARAM_IN_NEXT_CHALLENGE_token,
  SipParser_REALM_VALUE_token = 451, SipParser_DOMAIN_EQUAL_token = 453,
  SipParser_DOMAIN_WHITE_token, SipParser_DOMAIN_VAL_token = 456,
  SipParser_URI_LIST_token, SipParser_STRING_DOMAIN_PATCH_token,
  SipParser_DOMAIN_SPACE_token, SipParser_NONCE_VALUE_token = 462,
  SipParser_OPAQUE_VALUE_token = 464,
  SipParser_ALGORITHM_DECLARATION_token = 467,
  SipParser_ALGORITHM_AND_AKAV_token, SipParser_AKAV_PREFIX_token = 471,
  SipParser_AKAV_token, SipParser_AKA_VERSION_VAL_token,
  SipParser_ALGORITHM_VAL_token, SipParser_ALGORITHM_PREFIX_token,
  SipParser_QOP_VALUE_LIST_token = 483, SipParser_QOP_VALUE_token,
  SipParser_QUOTED_HEX_VALUE_token = 488,
  SipParser_AUTH_PARAM_DATA_token = 490, SipParser_AUTHORIZATION_VAL_token,
  SipParser_CREDENTIALS_VALUE_token, SipParser_DIGEST_RESPONSE_LIST_token,
  SipParser_DIGEST_RESPONSE_token, SipParser_USERNAME_token,
  SipParser_RESPONSE_token, SipParser_CNONCE_token,
  SipParser_NONCE_COUNT_token, SipParser_AUTS_PARAM_token,
  SipParser_INTEGRITY_PROTECTED_token, SipParser_MESSAGE_QOP_token,
  SipParser_DIGEST_URI_token, SipParser_USERNAME_VALUE_token = 504,
  SipParser_DIGEST_URI_VALUE_token = 506,
  SipParser_CNONCE_VALUE_token = 509, SipParser_NC_VALUE_token = 511,
  SipParser_LHEX_token, SipParser_REQUEST_DIGEST_token = 514,
  SipParser_THIRTY_TWO_LHEX_token,
  SipParser_SIXTY_FOUR_BASE_ENCODING_token = 519,
  SipParser_SIXTY_FOUR_BASE_ENCODING_CHAR_token,
  SipParser_ONE_OR_TWO_EQUAL_SIGNS_token = 522,
  SipParser_ROUTE_LIST_token = 529, SipParser_SINGLE_ROUTE_token,
  SipParser_RETRY_AFTER_VAL_token, SipParser_RETRY_AFTER_COMMENT_token,
  SipParser_RETRY_PARAM_token = 534, SipParser_DURATION_PARAM_token,
  SipParser_RSEQ_VAL_token = 537, SipParser_RESPONSE_NUM_token,
  SipParser_RACK_VAL_token, SipParser_OPTION_TAG_token,
  SipParser_OPTION_TAG_LIST_token, SipParser_SUPPORTED_OPTION_TAG_token,
  SipParser_REFER_TO_HEADER_VALUE_token,
  SipParser_REFERRED_BY_HEADER_VALUE_token = 545,
  SipParser_REFERRER_URI_token, SipParser_REFERRED_BY_PARAMS_token,
  SipParser_REFERRED_BY_PARAM_token = 549,
  SipParser_REFERRED_BY_PARAM_CID_token,
  SipParser_CID_PARAM_STRING_token = 552, SipParser_DOT_ATOM_token,
  SipParser_ATOM_CHAR_token, SipParser_DOT_ATOM_CHAR_token,
  SipParser_EVENT_HEADER_VAL_token = 558, SipParser_EVENT_TYPE_token,
  SipParser_EVENT_PARAMS_token, SipParser_EVENT_PACKAGE_token,
  SipParser_EVENT_TEMPLATE_token, SipParser_EVENT_PARAM_token,
  SipParser_EVENT_ID_token, SipParser_ALLOW_EVENTS_HEADER_VAL_token = 566,
  SipParser_SUBSCRIPTION_STATE_HEADER_VAL_token,
  SipParser_SUBSTATE_VALUE_token, SipParser_SUBSTATE_PARAMS_token,
  SipParser_SUBSTATE_PARAM_token = 573,
  SipParser_SUBSTATE_PARAM_REASON_token,
  SipParser_SUBSTATE_PARAM_RETRY_AFTER_token,
  SipParser_SUBSTATE_PARAM_EXPIRES_token,
  SipParser_SUBSTATE_PARAM_REASON_VAL_token = 579,
  SipParser_DISPOSITION_VAL_token = 586, SipParser_DISPOSITION_TYPE_token,
  SipParser_DISPOSITION_PARAMS_token, SipParser_HANDLING_PARAM_token = 594,
  SipParser_SESSION_EXPIRES_VALUE_token = 598,
  SipParser_REFRESHER_TYPE_token, SipParser_REFRESHER_TYPE_NAME_token,
  SipParser_REFRESHER_TYPE_VALUE_token, SipParser_MIN_SE_VALUE_token = 605,
  SipParser_REPLACES_HEADER_VALUE_token, SipParser_REPLACES_PARAMS_token,
  SipParser_REPLACES_PARAM_token, SipParser_TO_TAG_PARAM_token,
  SipParser_FROM_TAG_PARAM_token, SipParser_EARLY_FLAG_PARAM_token,
  SipParser_AUTHENTICATION_INFO_HEADER_LIST_token = 615,
  SipParser_AUTHENTICATION_INFO_HEADER_PARAM_token,
  SipParser_NEXT_NONCE_token, SipParser_RESPONSE_AUTH_token,
  SipParser_MSG_QOP_token, SipParser_RESPONSE_AUTH_VAL_token = 622
} SipParser_token_type;

typedef struct SipParser_pcb_struct{
  SipParser_token_type token_number, reduction_token, error_frame_token;
  int input_code;
  int input_value;
  int line, column;
  int ssx, sn, error_frame_ssx;
  int drt, dssx, dsn;
  int ss[43];
  SipParser_vs_type vs[43];
  int ag_ap;
  char *error_message;
  char read_flag;
  char exit_flag;
  unsigned char * pointer;
  unsigned char * la_ptr;
  const unsigned char *key_sp;
  int save_index, key_state;
  const  int *ag_dtl;
  int ag_dsn;
/*  Line 67, RV SIP Stack */
        ParserMgr              *pParserMgr;     
        void                   *pSipObject;     


        SipParseType            eHeaderType;    



        SipParseType            eWhichHeader;   



        RvChar                 *pBuffer;        


                  
        RvStatus                eStat;          
        RvChar                 *pCurToken;      
        void                   *pExtParams;     

        void                   *pUrlExtParams;  
                                              
        RvChar                 *pCurLine;       
		RvInt32					lineNumber;     
       
        RvBool                 isCompactForm;  

        RvBool                 isNewChallenge; 

        RvUint32               parenCount;     
        
        








        ParserContactHeader     contact;        
        ParserExpiresHeader     expires;        
        ParserPartyHeader       party;          

        ParserRequestLine       requestLine;    
        ParserRouteHeader       route;          
        ParserNameAddr          nameAddr;       
        ParserExUri             exUri;          
        ParserSipUrl            sipUrl;         
		ParserSingleVia         singleVia;      
        ParserContentType       contentType;    
        ParserUrlParameter      urlParameter;   






		ParserSentProtocol      sentProtocol;   

		ParserAuthorizationHeader  authorization; 
        ParserAuthenticationHeader authentication;
        ParserDigestChallenge      digestChallenge;
        ParserAuthenticationInfoHeader auth_info;		




        ParserReferredByHeader     referredBy;  

        ParserReferToHeader        referTo;     
		ParserEvent				   event;       
        ParserSubscriptionState    subsState;   

        ParserRetryAfter           retryAfter;  
        ParserContentDisposition   contentDisposition; 
        ParserRack                 rack;        
        ParserReplacesParams       replacesParams; 
        ParserReplacesParam        replacesParam;  

        ParserSentByInVia          viaSentBy;       




























	} SipParser_pcb_type;

#ifndef PRULE_CONTEXT
#define PRULE_CONTEXT(pcb)  (&((pcb).cs[(pcb).ssx]))
#define PERROR_CONTEXT(pcb) ((pcb).cs[(pcb).error_frame_ssx])
#define PCONTEXT(pcb)       ((pcb).cs[(pcb).ssx])
#endif

#ifndef AG_RUNNING_CODE_CODE
/* PCB.exit_flag values */
#define AG_RUNNING_CODE         0
#define AG_SUCCESS_CODE         1
#define AG_SYNTAX_ERROR_CODE    2
#define AG_REDUCTION_ERROR_CODE 3
#define AG_STACK_ERROR_CODE     4
#define AG_SEMANTIC_ERROR_CODE  5
#endif
void init_SipParser(SipParser_pcb_type *);
void SipParser(SipParser_pcb_type *);
#endif

#elif !defined(RV_SIP_IMS_DCS_HEADER_SUPPORT) && !defined(RV_SIP_IMS_HEADER_SUPPORT) && !defined(RV_SIP_EXTENDED_HEADER_SUPPORT) && !defined(RV_SIP_JSR32_SUPPORT) && !defined(RV_SIP_LIGHT) && !defined(RV_SIP_TEL_URI_SUPPORT) && !defined(RV_SIP_PRIMITIVES) && !defined(RV_SIP_AUTH_ON) && !defined(RV_SIP_SUBS_ON)
#ifndef PARSERENGINECLASSIC_H_1165245791
#define PARSERENGINECLASSIC_H_1165245791

typedef union {
  long alignment;
  char ag_vt_2[sizeof(int)];
  char ag_vt_4[sizeof(ParserBasicToken)];
  char ag_vt_5[sizeof(ParserQuotedString)];
  char ag_vt_6[sizeof(ParserMethod)];
  char ag_vt_7[sizeof(ParserCSeq)];
  char ag_vt_8[sizeof(ParserSequenceNumber)];
  char ag_vt_9[sizeof(ParserContentLength)];
  char ag_vt_10[sizeof(ParserCallId)];
  char ag_vt_11[sizeof(ParserMediaType)];
  char ag_vt_12[sizeof(ParserMediaSubType)];
  char ag_vt_13[sizeof(ParserBoundary)];
  char ag_vt_14[sizeof(ParserBase)];
  char ag_vt_15[sizeof(ParserGenericParamDataVal)];
  char ag_vt_16[sizeof(ParserSipVersion)];
  char ag_vt_17[sizeof(ParserStatusCode)];
  char ag_vt_18[sizeof(ParserReasonPhrase)];
  char ag_vt_19[sizeof(ParserProtocolName)];
  char ag_vt_20[sizeof(ParserProtocolVersion)];
  char ag_vt_21[sizeof(ParserTransport)];
  char ag_vt_22[sizeof(ParserPort)];
  char ag_vt_23[sizeof(ParserViaParam)];
  char ag_vt_24[sizeof(ParserHidden)];
  char ag_vt_25[sizeof(ParserAlias)];
  char ag_vt_26[sizeof(ParserTTLParam)];
  char ag_vt_27[sizeof(ParserTtlParam)];
  char ag_vt_28[sizeof(ParserMaddrParam)];
  char ag_vt_29[sizeof(ParserReceived)];
  char ag_vt_30[sizeof(ParserBranch)];
  char ag_vt_31[sizeof(ParserCompParam)];
  char ag_vt_32[sizeof(ParserAbsoluteUri)];
  char ag_vt_33[sizeof(ParserDisplayName)];
  char ag_vt_34[sizeof(ParserUserParam)];
  char ag_vt_35[sizeof(RvSipUserParam)];
  char ag_vt_36[sizeof(ParserOtherUser)];
  char ag_vt_37[sizeof(ParserLrParamType)];
  char ag_vt_38[sizeof(ParserTokenizedByParam)];
  char ag_vt_39[sizeof(RvBool)];
  char ag_vt_40[sizeof(ParserOptionalHeaders)];
  char ag_vt_41[sizeof(ParserTagParam)];
  char ag_vt_42[sizeof(ParserPartyParams)];
  char ag_vt_43[sizeof(ParserQVal)];
  char ag_vt_44[sizeof(ParserContactActionType)];
  char ag_vt_45[sizeof(ParserDeltaSeconds)];
  char ag_vt_46[sizeof(ParserSipDate)];
  char ag_vt_47[sizeof(RvSipDateWeekDay)];
  char ag_vt_48[sizeof(ParserDDMMYY)];
  char ag_vt_49[sizeof(ParserInt32)];
  char ag_vt_50[sizeof(RvSipDateMonth)];
  char ag_vt_51[sizeof(ParserTime)];
  char ag_vt_52[sizeof(ParserTimeZone)];
  char ag_vt_53[sizeof(ParserRetryParam)];
  char ag_vt_54[sizeof(ParserRSeq)];
  char ag_vt_55[sizeof(ParserResponseNum)];
  char ag_vt_56[sizeof(ParserDisposition)];
  char ag_vt_57[sizeof(ParserDispositionParams)];
  char ag_vt_58[sizeof(ParserDispositionParam)];
  char ag_vt_59[sizeof(ParserSessionExpiresHeader)];
  char ag_vt_60[sizeof(RvSipSessionExpiresRefresherType)];
  char ag_vt_61[sizeof(ParserMinSEHeader)];
  char ag_vt_62[sizeof(ParserReplacesEarlyFlagParamType)];
} SipParser_vs_type;

typedef enum {
  SipParser_START_token = 1, SipParser_FIRST_COLON_token = 3,
  SipParser_CSEQ_HEADER_token, SipParser_NULL_token,
  SipParser_PARTY_HEADER_token = 7, SipParser_EXPIRES_HEADER_token = 9,
  SipParser_SIP_DATE_HEADER_token = 11, SipParser_CONTACT_HEADER_token = 13,
  SipParser_CALL_ID_HEADER_token = 15, SipParser_CONTENT_LENGTH_token = 17,
  SipParser_VIA_HEADER_token = 19, SipParser_ROUTE_HOP_HEADER_token = 21,
  SipParser_ALLOW_HEADER_token = 23, SipParser_RACK_HEADER_token = 25,
  SipParser_RSEQ_HEADER_token = 27, SipParser_SUPPORTED_HEADER_token = 29,
  SipParser_OPTION_TAG_BASE_HEADER_token = 31,
  SipParser_CONTENT_DISPOSITION_HEADER_token = 33,
  SipParser_CONTENT_TYPE_HEADER_token = 35,
  SipParser_RETRY_AFTER_HEADER_token = 37,
  SipParser_SESSION_EXPIRES_HEADER_token = 39,
  SipParser_MIN_SE_HEADER_token = 41, SipParser_REPLACES_HEADER_token = 43,
  SipParser_STATUS_LINE_token, SipParser_REQUEST_LINE_token,
  SipParser_ABSOLUTE_URI_HEADER_token = 47, SipParser_PRES_URI_HEADER_token,
  SipParser_IM_URI_HEADER_token, SipParser_SIP_URL_HEADER_token,
  SipParser_OTHER_HEADER_token, SipParser_WHITE_token = 53,
  SipParser_SP_token = 55, SipParser_COMMA_token = 57,
  SipParser_COMMA_BETWEEN_HEADERS_token = 59, SipParser_EQUAL_token = 62,
  SipParser_AT_token = 64, SipParser_COLON_token = 66,
  SipParser_SEMI_COLON_token, SipParser_MARK_SIGN_token = 69,
  SipParser_SLASH_token = 71, SipParser_LEFT_PARENTHESIS_token = 73,
  SipParser_LEFT_ANGLE_BRACKET_token = 75,
  SipParser_LEFT_QUOTATION_token = 77, SipParser_TOKEN_token = 79,
  SipParser_TOKEN_CHAR_token, SipParser_CALL_ID_WORD_token = 82,
  SipParser_CALL_ID_WORD_CHAR_token,
  SipParser_TOKEN_FOR_CONTENT_TYPE_token = 85,
  SipParser_TOKEN_CHAR_FOR_CONTENT_TYPE_token,
  SipParser_FIELD_VALUE_token = 88, SipParser_QUOTED_STRING_token = 94,
  SipParser_QUOTED_CHAR_token, SipParser_METHOD_token = 102,
  SipParser_CSEQ_VAL_token = 109, SipParser_SEQUNCE_NUMBER_token,
  SipParser_DIGIT_token, SipParser_CONTENT_LENGTH_VAL_token = 113,
  SipParser_CALL_ID_token, SipParser_MEDIA_TYPE_token,
  SipParser_MEDIA_TYPE_TYPE_token, SipParser_MEDIA_TYPE_SUB_TYPE_token,
  SipParser_OPTIONAL_CONTENT_TYPE_PARAM_LIST_token,
  SipParser_CONTENT_TYPE_PARAM_LIST_token, SipParser_BOUNDARY_PARAM_token,
  SipParser_VERSION_PARAM_token, SipParser_BASE_PARAM_token,
  SipParser_CONTENT_TYPE_GENERIC_PARAM_DATA_token,
  SipParser_CONTENT_TYPE_GENERIC_PARAM_DATA_VAL_token = 143,
  SipParser_GENERIC_PARAM_LIST_token, SipParser_GENERIC_PARAM_DATA_token,
  SipParser_GENERIC_PARAM_DATA_VAL_token, SipParser_SIP_VERSION_token,
  SipParser_STATUS_CODE_token = 149, SipParser_REASON_PHRASE_token,
  SipParser_EXTENSION_CODE_token = 152, SipParser_REASONE_TEXT_UTF8_token,
  SipParser_ALLOW_VAL_token = 157, SipParser_VIA_LIST_token,
  SipParser_SINGLE_VIA_token, SipParser_SENT_PROTOCOL_token,
  SipParser_SENT_BY_token, SipParser_VIA_PARAM_LIST_token,
  SipParser_VIA_PARAM_token, SipParser_PROTOCOL_NAME_token,
  SipParser_PROTOCOL_VERSION_token, SipParser_TRANSPORT_token,
  SipParser_TRANSPORT_TYPE_token = 168, SipParser_PORT_token = 173,
  SipParser_HOST_NAME_token, SipParser_VIA_HIDDEN_token,
  SipParser_VIA_ALIAS_token, SipParser_VIA_TTL_token,
  SipParser_VIA_MADDR_token, SipParser_VIA_RECEIVED_token,
  SipParser_VIA_BRANCH_token, SipParser_RESPONSE_PORT_token,
  SipParser_VIA_COMPRESSION_token, SipParser_OTHER_PARAM_token,
  SipParser_OTHER_PARAM_EXPR_token, SipParser_IPV_6_REFERENCE_token,
  SipParser_TTL_PARAM_token = 188, SipParser_TTL_token = 190,
  SipParser_RPORT_NAME_token, SipParser_RPORT_VAL_token,
  SipParser_MADDR_PARAM_token = 194,
  SipParser_HOST_NAME_FOR_RECEIVED_token = 197,
  SipParser_COMPRESSION_TYPE_token = 200,
  SipParser_IPV_6_ADDRESS_token = 202, SipParser_TEST_COMMENT_token,
  SipParser_COMMENT_TEXT_token, SipParser_END_COMMENT_token = 206,
  SipParser_REQUEST_URI_token = 209, SipParser_ADDR_SPEC_token,
  SipParser_ABSOLUTE_URI_token, SipParser_ABS_URI_SCHEME_token,
  SipParser_URI_IDENTIFIER_token, SipParser_ABS_URI_SCHEME_CHAR_token,
  SipParser_URI_SCHEME_OR_DISPLAY_NAME_token = 216,
  SipParser_DISPLAY_NAME_token, SipParser_URI_IDENTIFIER_CHAR_token,
  SipParser_URI_IDENTIFIER_WITHOUT_SEPARATORS_token = 220,
  SipParser_SIP_URL_token = 223, SipParser_PRES_URI_token,
  SipParser_IM_URI_token, SipParser_SIP_PREFIX_token, SipParser_URL_token,
  SipParser_PRES_PREFIX_token = 230, SipParser_RV_ABS_PREFIX_token,
  SipParser_IM_PREFIX_token = 234, SipParser_URL_USERINFO_token = 236,
  SipParser_OPTIONAL_URL_PARAMETERS_token, SipParser_OPTIONAL_HEADERS_token,
  SipParser_URL_HOST_token, SipParser_GENERAL_URL_WITHOUT_PARAM_token,
  SipParser_SIP_URL_WITHOUT_PARAMS_token,
  SipParser_PRES_URL_WITHOUT_PARAMS_token,
  SipParser_IM_URL_WITHOUT_PARAMS_token,
  SipParser_ABS_URI_WITHOUT_PARAMS_token,
  SipParser_URL_USERINFO_PREFIX_token, SipParser_URL_USERINFO_VAL_token,
  SipParser_HOSTPORT_token, SipParser_USER_INFO_TOKEN_token = 249,
  SipParser_PASSWORD_token, SipParser_TOKEN_USER_INFO_token,
  SipParser_TOKEN_USER_INFO_CHAR_token,
  SipParser_PASSWORD_STRING_token = 254,
  SipParser_PASSWORD_STRING_CHAR_token,
  SipParser_URL_HOST_PREFIX_token = 259, SipParser_TOKEN_HOST_token = 261,
  SipParser_ALPHANUM_token, SipParser_TOKEN_HOST_CHAR_token,
  SipParser_IPV_4_ADDRESS_token = 267, SipParser_IPV_4_PART_token,
  SipParser_HEXPART_token = 270, SipParser_HEXSEQ_token,
  SipParser_HEX4_token = 274, SipParser_HEX_token = 278,
  SipParser_URL_PARAMETERS_token, SipParser_URL_PARAMETER_token,
  SipParser_TRANSPORT_PARAM_token, SipParser_USER_PARAM_token,
  SipParser_TTL_PARAM_URL_token, SipParser_MADDR_PARAM_URL_token,
  SipParser_METHOD_PARAM_URL_token, SipParser_LR_PARAM_URL_token,
  SipParser_COMPRESSION_PARAM_URL_token,
  SipParser_TOKENIZED_BY_PARAM_URL_token, SipParser_ORIG_PARAM_URL_token,
  SipParser_URL_GENERIC_PARAM_DATA_token,
  SipParser_USER_PARAM_TYPE_token = 293, SipParser_OTHER_USER_token,
  SipParser_URL_OTHER_PARAM_VAL_token = 306, SipParser_PVALUE_token,
  SipParser_PARAMCHAR_token, SipParser_HEADERS_LIST_token = 312,
  SipParser_HEADER_token, SipParser_HNAME_token = 315,
  SipParser_HVALUE_token, SipParser_REQUEST_LINE_VALUE_token = 323,
  SipParser_REQUEST_METHOD_token, SipParser_PARTY_VALUE_token = 338,
  SipParser_NAME_ADDR_token, SipParser_PARTY_PARAMS_LIST_token,
  SipParser_MORE_THAN_ONE_TOKEN_token = 342,
  SipParser_TAG_PARAM_token = 346, SipParser_PARTY_PARAM_DATA_token = 348,
  SipParser_CONTACT_HEADER_VALUES_token,
  SipParser_CONTACT_HEADER_OTHER_token,
  SipParser_CONTACT_HEADER_VALUE_LIST_token,
  SipParser_CONTACT_HEADER_VALUE_token = 353,
  SipParser_CONTACT_PARAMS_token, SipParser_CONTACT_PARAM_token,
  SipParser_Q_token, SipParser_ACTION_token,
  SipParser_CONTACT_EXPIRES_token, SipParser_Q_VALUE_token = 360,
  SipParser_Q_VALUE_OPTIONAL_token = 362,
  SipParser_ACTION_VALUE_token = 366, SipParser_EXPIRES_VALUE_token = 369,
  SipParser_EXPIRES_VALUE_FOR_CONTACT_token = 371,
  SipParser_DELTA_SECONDS_token, SipParser_SIP_DATE_token,
  SipParser_SIP_DATE_HEADER_VAL_token, SipParser_DATE_token,
  SipParser_TIME_token, SipParser_DAY_token, SipParser_DD_token = 385,
  SipParser_MM_token, SipParser_YY_token, SipParser_HOUR_token = 400,
  SipParser_MINUTE_token, SipParser_SECOND_token, SipParser_TIME_ZONE_token,
  SipParser_ROUTE_LIST_token = 407, SipParser_SINGLE_ROUTE_token,
  SipParser_RETRY_AFTER_VAL_token, SipParser_RETRY_AFTER_COMMENT_token,
  SipParser_RETRY_PARAM_token = 412, SipParser_DURATION_PARAM_token,
  SipParser_RSEQ_VAL_token = 415, SipParser_RESPONSE_NUM_token,
  SipParser_RACK_VAL_token, SipParser_OPTION_TAG_token,
  SipParser_OPTION_TAG_LIST_token, SipParser_SUPPORTED_OPTION_TAG_token,
  SipParser_DISPOSITION_VAL_token, SipParser_DISPOSITION_TYPE_token,
  SipParser_DISPOSITION_PARAMS_token, SipParser_HANDLING_PARAM_token = 429,
  SipParser_SESSION_EXPIRES_VALUE_token = 433,
  SipParser_REFRESHER_TYPE_token, SipParser_REFRESHER_TYPE_NAME_token = 436,
  SipParser_REFRESHER_TYPE_VALUE_token, SipParser_MIN_SE_VALUE_token = 441,
  SipParser_REPLACES_HEADER_VALUE_token, SipParser_REPLACES_PARAMS_token,
  SipParser_REPLACES_PARAM_token, SipParser_TO_TAG_PARAM_token,
  SipParser_FROM_TAG_PARAM_token, SipParser_EARLY_FLAG_PARAM_token
} SipParser_token_type;

typedef struct SipParser_pcb_struct{
  SipParser_token_type token_number, reduction_token, error_frame_token;
  int input_code;
  int input_value;
  int line, column;
  int ssx, sn, error_frame_ssx;
  int drt, dssx, dsn;
  int ss[28];
  SipParser_vs_type vs[28];
  int ag_ap;
  char *error_message;
  char read_flag;
  char exit_flag;
  unsigned char * pointer;
  unsigned char * la_ptr;
  const unsigned char *key_sp;
  int save_index, key_state;
  const  int *ag_dtl;
  int ag_dsn;
/*  Line 67, RV SIP Stack */
        ParserMgr              *pParserMgr;     
        void                   *pSipObject;     


        SipParseType            eHeaderType;    



        SipParseType            eWhichHeader;   



        RvChar                 *pBuffer;        


                  
        RvStatus                eStat;          
        RvChar                 *pCurToken;      
        void                   *pExtParams;     

        void                   *pUrlExtParams;  
                                              
        RvChar                 *pCurLine;       
		RvInt32					lineNumber;     
       
        RvBool                 isCompactForm;  

        RvBool                 isNewChallenge; 

        RvUint32               parenCount;     
        
        








        ParserContactHeader     contact;        
        ParserExpiresHeader     expires;        
        ParserPartyHeader       party;          

        ParserRequestLine       requestLine;    
        ParserRouteHeader       route;          
        ParserNameAddr          nameAddr;       
        ParserExUri             exUri;          
        ParserSipUrl            sipUrl;         
		ParserSingleVia         singleVia;      
        ParserContentType       contentType;    
        ParserUrlParameter      urlParameter;   






		ParserSentProtocol      sentProtocol;   















        ParserRetryAfter           retryAfter;  
        ParserContentDisposition   contentDisposition; 
        ParserRack                 rack;        
        ParserReplacesParams       replacesParams; 
        ParserReplacesParam        replacesParam;  

        ParserSentByInVia          viaSentBy;       




























	} SipParser_pcb_type;

#ifndef PRULE_CONTEXT
#define PRULE_CONTEXT(pcb)  (&((pcb).cs[(pcb).ssx]))
#define PERROR_CONTEXT(pcb) ((pcb).cs[(pcb).error_frame_ssx])
#define PCONTEXT(pcb)       ((pcb).cs[(pcb).ssx])
#endif

#ifndef AG_RUNNING_CODE_CODE
/* PCB.exit_flag values */
#define AG_RUNNING_CODE         0
#define AG_SUCCESS_CODE         1
#define AG_SYNTAX_ERROR_CODE    2
#define AG_REDUCTION_ERROR_CODE 3
#define AG_STACK_ERROR_CODE     4
#define AG_SEMANTIC_ERROR_CODE  5
#endif
void init_SipParser(SipParser_pcb_type *);
void SipParser(SipParser_pcb_type *);
#endif

#endif
