#if defined(RV_SIP_AUTH_ON) && defined(RV_SIP_PRIMITIVES) && !defined(RV_SIP_IMS_DCS_HEADER_SUPPORT) && !defined(RV_SIP_IMS_HEADER_SUPPORT) && !defined(RV_SIP_EXTENDED_HEADER_SUPPORT) && !defined(RV_SIP_JSR32_SUPPORT) && !defined(RV_SIP_LIGHT) && defined(RV_SIP_TEL_URI_SUPPORT) && !defined(RV_SIP_SUBS_ON)
#ifndef PARSERENGINESIPPRIMITIVES_H_1165245729
#define PARSERENGINESIPPRIMITIVES_H_1165245729

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
  char ag_vt_13[sizeof(ParserGenericParamDataVal)];
  char ag_vt_14[sizeof(ParserSipVersion)];
  char ag_vt_15[sizeof(ParserStatusCode)];
  char ag_vt_16[sizeof(ParserReasonPhrase)];
  char ag_vt_17[sizeof(ParserProtocolName)];
  char ag_vt_18[sizeof(ParserProtocolVersion)];
  char ag_vt_19[sizeof(ParserTransport)];
  char ag_vt_20[sizeof(ParserPort)];
  char ag_vt_21[sizeof(ParserViaParam)];
  char ag_vt_22[sizeof(ParserHidden)];
  char ag_vt_23[sizeof(ParserAlias)];
  char ag_vt_24[sizeof(ParserTTLParam)];
  char ag_vt_25[sizeof(ParserTtlParam)];
  char ag_vt_26[sizeof(ParserMaddrParam)];
  char ag_vt_27[sizeof(ParserReceived)];
  char ag_vt_28[sizeof(ParserBranch)];
  char ag_vt_29[sizeof(ParserCompParam)];
  char ag_vt_30[sizeof(ParserAbsoluteUri)];
  char ag_vt_31[sizeof(ParserDisplayName)];
  char ag_vt_32[sizeof(ParserEnumdiParamType)];
  char ag_vt_33[sizeof(ParserUserParam)];
  char ag_vt_34[sizeof(RvSipUserParam)];
  char ag_vt_35[sizeof(ParserOtherUser)];
  char ag_vt_36[sizeof(ParserLrParamType)];
  char ag_vt_37[sizeof(ParserTokenizedByParam)];
  char ag_vt_38[sizeof(RvBool)];
  char ag_vt_39[sizeof(ParserOptionalHeaders)];
  char ag_vt_40[sizeof(ParserTagParam)];
  char ag_vt_41[sizeof(ParserPartyParams)];
  char ag_vt_42[sizeof(ParserQVal)];
  char ag_vt_43[sizeof(ParserContactActionType)];
  char ag_vt_44[sizeof(ParserDeltaSeconds)];
  char ag_vt_45[sizeof(ParserSipDate)];
  char ag_vt_46[sizeof(RvSipDateWeekDay)];
  char ag_vt_47[sizeof(ParserDDMMYY)];
  char ag_vt_48[sizeof(ParserInt32)];
  char ag_vt_49[sizeof(RvSipDateMonth)];
  char ag_vt_50[sizeof(ParserTime)];
  char ag_vt_51[sizeof(ParserTimeZone)];
  char ag_vt_52[sizeof(RvSipAuthStale)];
  char ag_vt_53[sizeof(ParserAlgorithm)];
  char ag_vt_54[sizeof(ParserQopValue)];
  char ag_vt_55[sizeof(ParserAuthQopOptions)];
  char ag_vt_56[sizeof(RvSipAuthQopOption)];
  char ag_vt_57[sizeof(RvSipAuthIntegrityProtected)];
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
  SipParser_AUTHENTICATION_INFO_HEADER_token = 27,
  SipParser_STATUS_LINE_token, SipParser_REQUEST_LINE_token,
  SipParser_ABSOLUTE_URI_HEADER_token = 31, SipParser_PRES_URI_HEADER_token,
  SipParser_IM_URI_HEADER_token, SipParser_SIP_URL_HEADER_token,
  SipParser_TEL_URI_HEADER_token, SipParser_OTHER_HEADER_token,
  SipParser_WHITE_token = 38, SipParser_SP_token = 40,
  SipParser_COMMA_token = 42, SipParser_COMMA_BETWEEN_HEADERS_token = 44,
  SipParser_EQUAL_token = 47, SipParser_AT_token = 49,
  SipParser_COLON_token = 51, SipParser_SEMI_COLON_token,
  SipParser_MARK_SIGN_token = 54, SipParser_SLASH_token = 56,
  SipParser_LEFT_ANGLE_BRACKET_token = 58,
  SipParser_LEFT_QUOTATION_token = 60, SipParser_HYPHEN_token = 62,
  SipParser_TOKEN_token = 64, SipParser_TOKEN_CHAR_token,
  SipParser_CALL_ID_WORD_token = 67, SipParser_CALL_ID_WORD_CHAR_token,
  SipParser_TOKEN_FOR_CONTENT_TYPE_token = 70,
  SipParser_TOKEN_CHAR_FOR_CONTENT_TYPE_token,
  SipParser_FIELD_VALUE_token = 73, SipParser_QUOTED_STRING_token = 79,
  SipParser_QUOTED_CHAR_token, SipParser_HEX_VALUE_token = 87,
  SipParser_HEX_token, SipParser_METHOD_token = 90,
  SipParser_CSEQ_VAL_token = 96, SipParser_SEQUNCE_NUMBER_token,
  SipParser_DIGIT_token, SipParser_CONTENT_LENGTH_VAL_token = 100,
  SipParser_CALL_ID_token, SipParser_CONTENT_TYPE_HEADER_token,
  SipParser_MEDIA_TYPE_token, SipParser_MEDIA_TYPE_TYPE_token,
  SipParser_MEDIA_TYPE_SUB_TYPE_token,
  SipParser_OPTIONAL_CONTENT_TYPE_PARAM_LIST_token,
  SipParser_CONTENT_TYPE_PARAM_LIST_token,
  SipParser_CONTENT_TYPE_GENERIC_PARAM_DATA_token,
  SipParser_CONTENT_TYPE_GENERIC_PARAM_DATA_VAL_token,
  SipParser_GENERIC_PARAM_LIST_token, SipParser_GENERIC_PARAM_DATA_token,
  SipParser_GENERIC_PARAM_DATA_VAL_token, SipParser_SIP_VERSION_token,
  SipParser_STATUS_CODE_token = 115, SipParser_REASON_PHRASE_token,
  SipParser_EXTENSION_CODE_token = 118, SipParser_REASONE_TEXT_UTF8_token,
  SipParser_VIA_LIST_token = 123, SipParser_SINGLE_VIA_token,
  SipParser_SENT_PROTOCOL_token, SipParser_SENT_BY_token,
  SipParser_VIA_PARAM_LIST_token, SipParser_VIA_PARAM_token,
  SipParser_PROTOCOL_NAME_token, SipParser_PROTOCOL_VERSION_token,
  SipParser_TRANSPORT_token, SipParser_TRANSPORT_TYPE_token = 133,
  SipParser_PORT_token = 138, SipParser_HOST_NAME_token,
  SipParser_VIA_HIDDEN_token, SipParser_VIA_ALIAS_token,
  SipParser_VIA_TTL_token, SipParser_VIA_MADDR_token,
  SipParser_VIA_RECEIVED_token, SipParser_VIA_BRANCH_token,
  SipParser_RESPONSE_PORT_token, SipParser_VIA_COMPRESSION_token,
  SipParser_OTHER_PARAM_token, SipParser_OTHER_PARAM_EXPR_token,
  SipParser_IPV_6_REFERENCE_token, SipParser_TTL_PARAM_token = 153,
  SipParser_TTL_token = 155, SipParser_RPORT_NAME_token,
  SipParser_RPORT_VAL_token, SipParser_MADDR_PARAM_token = 159,
  SipParser_HOST_NAME_FOR_RECEIVED_token = 162,
  SipParser_COMPRESSION_TYPE_token = 165,
  SipParser_IPV_6_ADDRESS_token = 167, SipParser_REQUEST_URI_token,
  SipParser_ADDR_SPEC_token, SipParser_ABSOLUTE_URI_token,
  SipParser_ABS_URI_SCHEME_token, SipParser_URI_IDENTIFIER_token,
  SipParser_ABS_URI_SCHEME_CHAR_token,
  SipParser_URI_SCHEME_OR_DISPLAY_NAME_token = 175,
  SipParser_DISPLAY_NAME_token, SipParser_URI_IDENTIFIER_CHAR_token,
  SipParser_URI_IDENTIFIER_WITHOUT_SEPARATORS_token = 179,
  SipParser_TEL_URI_token = 182, SipParser_TEL_PREFIX_token,
  SipParser_TEL_token, SipParser_GLOBAL_OR_LOCAL_NUMBER_token = 186,
  SipParser_OPTIONAL_TEL_URI_PARAMETERS_token,
  SipParser_GLOBAL_NUMBER_token, SipParser_LOCAL_NUMBER_token,
  SipParser_GLOBAL_NUMBER_DIGITS_token, SipParser_PHONEDIGIT_token = 192,
  SipParser_VISUAL_SEPERATOR_token = 196,
  SipParser_LOCAL_NUMBER_DIGITS_token = 198,
  SipParser_HEX_OR_PHONECHAR_token, SipParser_PHONEDIGIT_HEX_token,
  SipParser_PHONECHAR_token = 204, SipParser_TEL_URI_PARAMETERS_token,
  SipParser_TEL_URI_PARAMETER_token,
  SipParser_TEL_URI_EXTENSION_PARAM_token,
  SipParser_TEL_URI_POST_DIAL_PARAM_token,
  SipParser_TEL_URI_ISDN_SUB_ADDR_PARAM_URL_token,
  SipParser_TEL_URI_CONTEXT_PARAM_token,
  SipParser_TEL_URI_ENUMDI_PARAM_token,
  SipParser_URL_GENERIC_PARAM_DATA_token,
  SipParser_TEL_URI_EXTENTION_VALUE_token = 214,
  SipParser_TEL_URI_POST_DIAL_VALUE_token = 217,
  SipParser_TEL_URI_POST_DIAL_CHAR_token, SipParser_DTMFDIGIT_token = 220,
  SipParser_PAUSE_CHAR_token,
  SipParser_TEL_URI_ISDN_SUB_ADDR_VALUE_token = 223, SipParser_URIC_token,
  SipParser_RESERVED_FOR_URIC_token = 226, SipParser_UNRESERVED_token,
  SipParser_PCT_ENCODED_token, SipParser_TEL_URI_CONTEXT_VALUE_token = 231,
  SipParser_DESCRIPTOR_token, SipParser_DOMAINNAME_token,
  SipParser_LABEL_token, SipParser_NEXT_LABEL_token,
  SipParser_ALPHA_token = 239, SipParser_ALPHANUM_OR_HYPHEN_token,
  SipParser_ALPHANUM_token = 245, SipParser_SIP_URL_token = 247,
  SipParser_PRES_URI_token, SipParser_IM_URI_token,
  SipParser_SIP_PREFIX_token, SipParser_URL_token,
  SipParser_PRES_PREFIX_token = 254, SipParser_RV_ABS_PREFIX_token,
  SipParser_IM_PREFIX_token = 258, SipParser_URL_USERINFO_token = 260,
  SipParser_OPTIONAL_URL_PARAMETERS_token, SipParser_OPTIONAL_HEADERS_token,
  SipParser_URL_HOST_token, SipParser_GENERAL_URL_WITHOUT_PARAM_token,
  SipParser_SIP_URL_WITHOUT_PARAMS_token,
  SipParser_TEL_URL_WITHOUT_PARAMS_token,
  SipParser_PRES_URL_WITHOUT_PARAMS_token,
  SipParser_IM_URL_WITHOUT_PARAMS_token,
  SipParser_ABS_URI_WITHOUT_PARAMS_token,
  SipParser_URL_USERINFO_PREFIX_token, SipParser_URL_USERINFO_VAL_token,
  SipParser_HOSTPORT_token, SipParser_USER_INFO_TOKEN_token = 274,
  SipParser_PASSWORD_token, SipParser_TOKEN_USER_INFO_token,
  SipParser_TOKEN_USER_INFO_CHAR_token,
  SipParser_PASSWORD_STRING_token = 279,
  SipParser_PASSWORD_STRING_CHAR_token,
  SipParser_URL_HOST_PREFIX_token = 284, SipParser_TOKEN_HOST_token = 286,
  SipParser_TOKEN_HOST_CHAR_token, SipParser_IPV_4_ADDRESS_token = 291,
  SipParser_IPV_4_PART_token, SipParser_HEXPART_token,
  SipParser_HEXSEQ_token, SipParser_HEX4_token = 297,
  SipParser_URL_PARAMETERS_token = 301, SipParser_URL_PARAMETER_token,
  SipParser_TRANSPORT_PARAM_token, SipParser_USER_PARAM_token,
  SipParser_TTL_PARAM_URL_token, SipParser_MADDR_PARAM_URL_token,
  SipParser_METHOD_PARAM_URL_token, SipParser_LR_PARAM_URL_token,
  SipParser_COMPRESSION_PARAM_URL_token,
  SipParser_TOKENIZED_BY_PARAM_URL_token, SipParser_ORIG_PARAM_URL_token,
  SipParser_USER_PARAM_TYPE_token = 314, SipParser_OTHER_USER_token,
  SipParser_URL_OTHER_PARAM_VAL_token = 327, SipParser_PVALUE_token,
  SipParser_PARAMCHAR_token, SipParser_HEADERS_LIST_token = 333,
  SipParser_HEADER_token, SipParser_HNAME_token = 336,
  SipParser_HVALUE_token, SipParser_REQUEST_LINE_VALUE_token = 344,
  SipParser_REQUEST_METHOD_token, SipParser_PARTY_VALUE_token = 359,
  SipParser_NAME_ADDR_token, SipParser_PARTY_PARAMS_LIST_token,
  SipParser_MORE_THAN_ONE_TOKEN_token = 363,
  SipParser_TAG_PARAM_token = 367, SipParser_PARTY_PARAM_DATA_token = 369,
  SipParser_CONTACT_HEADER_VALUES_token,
  SipParser_CONTACT_HEADER_OTHER_token,
  SipParser_CONTACT_HEADER_VALUE_LIST_token,
  SipParser_CONTACT_HEADER_VALUE_token = 374,
  SipParser_CONTACT_PARAMS_token, SipParser_CONTACT_PARAM_token,
  SipParser_Q_token, SipParser_ACTION_token,
  SipParser_CONTACT_EXPIRES_token, SipParser_Q_VALUE_token = 381,
  SipParser_Q_VALUE_OPTIONAL_token = 383,
  SipParser_ACTION_VALUE_token = 387, SipParser_EXPIRES_VALUE_token = 390,
  SipParser_EXPIRES_VALUE_FOR_CONTACT_token = 392,
  SipParser_DELTA_SECONDS_token, SipParser_SIP_DATE_token,
  SipParser_SIP_DATE_HEADER_VAL_token, SipParser_DATE_token,
  SipParser_TIME_token, SipParser_DAY_token, SipParser_DD_token = 406,
  SipParser_MM_token, SipParser_YY_token, SipParser_HOUR_token = 421,
  SipParser_MINUTE_token, SipParser_SECOND_token, SipParser_TIME_ZONE_token,
  SipParser_AUTHENTICATION_VAL_token = 428, SipParser_CHALLENGE_VALUE_token,
  SipParser_DIGEST_CHALLENGE_LIST_token = 431,
  SipParser_AUTH_PARAM_LIST_token, SipParser_DIGEST_CHALLENGE_token,
  SipParser_REALM_token, SipParser_DOMAIN_token, SipParser_NONCE_token,
  SipParser_OPAQUE_token, SipParser_INTEGRITY_KEY_token,
  SipParser_CIPHER_KEY_token, SipParser_AUTH_PARAM_token,
  SipParser_STALE_token, SipParser_ALGORITHM_token,
  SipParser_QOP_OPTIONS_token, SipParser_PARAM_IN_NEXT_CHALLENGE_token,
  SipParser_REALM_VALUE_token = 446, SipParser_DOMAIN_EQUAL_token = 448,
  SipParser_DOMAIN_WHITE_token, SipParser_DOMAIN_VAL_token = 451,
  SipParser_URI_LIST_token, SipParser_STRING_DOMAIN_PATCH_token,
  SipParser_DOMAIN_SPACE_token, SipParser_NONCE_VALUE_token = 457,
  SipParser_OPAQUE_VALUE_token = 459,
  SipParser_ALGORITHM_DECLARATION_token = 462,
  SipParser_ALGORITHM_AND_AKAV_token, SipParser_AKAV_PREFIX_token = 466,
  SipParser_AKAV_token, SipParser_AKA_VERSION_VAL_token,
  SipParser_ALGORITHM_VAL_token, SipParser_ALGORITHM_PREFIX_token,
  SipParser_QOP_VALUE_LIST_token = 478, SipParser_QOP_VALUE_token,
  SipParser_QUOTED_HEX_VALUE_token = 483,
  SipParser_AUTH_PARAM_DATA_token = 485, SipParser_AUTHORIZATION_VAL_token,
  SipParser_CREDENTIALS_VALUE_token, SipParser_DIGEST_RESPONSE_LIST_token,
  SipParser_DIGEST_RESPONSE_token, SipParser_USERNAME_token,
  SipParser_RESPONSE_token, SipParser_CNONCE_token,
  SipParser_NONCE_COUNT_token, SipParser_AUTS_PARAM_token,
  SipParser_INTEGRITY_PROTECTED_token, SipParser_MESSAGE_QOP_token,
  SipParser_DIGEST_URI_token, SipParser_USERNAME_VALUE_token = 499,
  SipParser_DIGEST_URI_VALUE_token = 501,
  SipParser_CNONCE_VALUE_token = 504, SipParser_NC_VALUE_token = 506,
  SipParser_LHEX_token, SipParser_REQUEST_DIGEST_token = 509,
  SipParser_THIRTY_TWO_LHEX_token,
  SipParser_SIXTY_FOUR_BASE_ENCODING_token = 514,
  SipParser_SIXTY_FOUR_BASE_ENCODING_CHAR_token,
  SipParser_ONE_OR_TWO_EQUAL_SIGNS_token = 517,
  SipParser_ROUTE_LIST_token = 524, SipParser_SINGLE_ROUTE_token,
  SipParser_AUTHENTICATION_INFO_HEADER_LIST_token,
  SipParser_AUTHENTICATION_INFO_HEADER_PARAM_token,
  SipParser_NEXT_NONCE_token, SipParser_RESPONSE_AUTH_token,
  SipParser_MSG_QOP_token, SipParser_RESPONSE_AUTH_VAL_token = 533
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

		ParserTelUri            telUri;         
        ParserTelUriParameter   telUriParameter;
		RvBool                  isTopLabel;     


		ParserSentProtocol      sentProtocol;   

		ParserAuthorizationHeader  authorization; 
        ParserAuthenticationHeader authentication;
        ParserDigestChallenge      digestChallenge;
        ParserAuthenticationInfoHeader auth_info;		
















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

#elif defined(RV_SIP_AUTH_ON) && defined(RV_SIP_PRIMITIVES) && !defined(RV_SIP_IMS_DCS_HEADER_SUPPORT) && !defined(RV_SIP_IMS_HEADER_SUPPORT) && !defined(RV_SIP_EXTENDED_HEADER_SUPPORT) && !defined(RV_SIP_JSR32_SUPPORT) && !defined(RV_SIP_LIGHT) && !defined(RV_SIP_TEL_URI_SUPPORT) && !defined(RV_SIP_SUBS_ON)
#ifndef PARSERENGINESIPPRIMITIVES_H_1165245732
#define PARSERENGINESIPPRIMITIVES_H_1165245732

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
  char ag_vt_13[sizeof(ParserGenericParamDataVal)];
  char ag_vt_14[sizeof(ParserSipVersion)];
  char ag_vt_15[sizeof(ParserStatusCode)];
  char ag_vt_16[sizeof(ParserReasonPhrase)];
  char ag_vt_17[sizeof(ParserProtocolName)];
  char ag_vt_18[sizeof(ParserProtocolVersion)];
  char ag_vt_19[sizeof(ParserTransport)];
  char ag_vt_20[sizeof(ParserPort)];
  char ag_vt_21[sizeof(ParserViaParam)];
  char ag_vt_22[sizeof(ParserHidden)];
  char ag_vt_23[sizeof(ParserAlias)];
  char ag_vt_24[sizeof(ParserTTLParam)];
  char ag_vt_25[sizeof(ParserTtlParam)];
  char ag_vt_26[sizeof(ParserMaddrParam)];
  char ag_vt_27[sizeof(ParserReceived)];
  char ag_vt_28[sizeof(ParserBranch)];
  char ag_vt_29[sizeof(ParserCompParam)];
  char ag_vt_30[sizeof(ParserAbsoluteUri)];
  char ag_vt_31[sizeof(ParserDisplayName)];
  char ag_vt_32[sizeof(ParserUserParam)];
  char ag_vt_33[sizeof(RvSipUserParam)];
  char ag_vt_34[sizeof(ParserOtherUser)];
  char ag_vt_35[sizeof(ParserLrParamType)];
  char ag_vt_36[sizeof(ParserTokenizedByParam)];
  char ag_vt_37[sizeof(RvBool)];
  char ag_vt_38[sizeof(ParserOptionalHeaders)];
  char ag_vt_39[sizeof(ParserTagParam)];
  char ag_vt_40[sizeof(ParserPartyParams)];
  char ag_vt_41[sizeof(ParserQVal)];
  char ag_vt_42[sizeof(ParserContactActionType)];
  char ag_vt_43[sizeof(ParserDeltaSeconds)];
  char ag_vt_44[sizeof(ParserSipDate)];
  char ag_vt_45[sizeof(RvSipDateWeekDay)];
  char ag_vt_46[sizeof(ParserDDMMYY)];
  char ag_vt_47[sizeof(ParserInt32)];
  char ag_vt_48[sizeof(RvSipDateMonth)];
  char ag_vt_49[sizeof(ParserTime)];
  char ag_vt_50[sizeof(ParserTimeZone)];
  char ag_vt_51[sizeof(RvSipAuthStale)];
  char ag_vt_52[sizeof(ParserAlgorithm)];
  char ag_vt_53[sizeof(ParserQopValue)];
  char ag_vt_54[sizeof(ParserAuthQopOptions)];
  char ag_vt_55[sizeof(RvSipAuthQopOption)];
  char ag_vt_56[sizeof(RvSipAuthIntegrityProtected)];
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
  SipParser_AUTHENTICATION_INFO_HEADER_token = 27,
  SipParser_STATUS_LINE_token, SipParser_REQUEST_LINE_token,
  SipParser_ABSOLUTE_URI_HEADER_token = 31, SipParser_PRES_URI_HEADER_token,
  SipParser_IM_URI_HEADER_token, SipParser_SIP_URL_HEADER_token,
  SipParser_OTHER_HEADER_token, SipParser_WHITE_token = 37,
  SipParser_SP_token = 39, SipParser_COMMA_token = 41,
  SipParser_COMMA_BETWEEN_HEADERS_token = 43, SipParser_EQUAL_token = 46,
  SipParser_AT_token = 48, SipParser_COLON_token = 50,
  SipParser_SEMI_COLON_token, SipParser_MARK_SIGN_token = 53,
  SipParser_SLASH_token = 55, SipParser_LEFT_ANGLE_BRACKET_token = 57,
  SipParser_LEFT_QUOTATION_token = 59, SipParser_HYPHEN_token = 61,
  SipParser_TOKEN_token = 63, SipParser_TOKEN_CHAR_token,
  SipParser_CALL_ID_WORD_token = 66, SipParser_CALL_ID_WORD_CHAR_token,
  SipParser_TOKEN_FOR_CONTENT_TYPE_token = 69,
  SipParser_TOKEN_CHAR_FOR_CONTENT_TYPE_token,
  SipParser_FIELD_VALUE_token = 72, SipParser_QUOTED_STRING_token = 78,
  SipParser_QUOTED_CHAR_token, SipParser_HEX_VALUE_token = 86,
  SipParser_HEX_token, SipParser_METHOD_token = 89,
  SipParser_CSEQ_VAL_token = 95, SipParser_SEQUNCE_NUMBER_token,
  SipParser_DIGIT_token, SipParser_CONTENT_LENGTH_VAL_token = 99,
  SipParser_CALL_ID_token, SipParser_CONTENT_TYPE_HEADER_token,
  SipParser_MEDIA_TYPE_token, SipParser_MEDIA_TYPE_TYPE_token,
  SipParser_MEDIA_TYPE_SUB_TYPE_token,
  SipParser_OPTIONAL_CONTENT_TYPE_PARAM_LIST_token,
  SipParser_CONTENT_TYPE_PARAM_LIST_token,
  SipParser_CONTENT_TYPE_GENERIC_PARAM_DATA_token,
  SipParser_CONTENT_TYPE_GENERIC_PARAM_DATA_VAL_token,
  SipParser_GENERIC_PARAM_LIST_token, SipParser_GENERIC_PARAM_DATA_token,
  SipParser_GENERIC_PARAM_DATA_VAL_token, SipParser_SIP_VERSION_token,
  SipParser_STATUS_CODE_token = 114, SipParser_REASON_PHRASE_token,
  SipParser_EXTENSION_CODE_token = 117, SipParser_REASONE_TEXT_UTF8_token,
  SipParser_VIA_LIST_token = 122, SipParser_SINGLE_VIA_token,
  SipParser_SENT_PROTOCOL_token, SipParser_SENT_BY_token,
  SipParser_VIA_PARAM_LIST_token, SipParser_VIA_PARAM_token,
  SipParser_PROTOCOL_NAME_token, SipParser_PROTOCOL_VERSION_token,
  SipParser_TRANSPORT_token, SipParser_TRANSPORT_TYPE_token = 132,
  SipParser_PORT_token = 137, SipParser_HOST_NAME_token,
  SipParser_VIA_HIDDEN_token, SipParser_VIA_ALIAS_token,
  SipParser_VIA_TTL_token, SipParser_VIA_MADDR_token,
  SipParser_VIA_RECEIVED_token, SipParser_VIA_BRANCH_token,
  SipParser_RESPONSE_PORT_token, SipParser_VIA_COMPRESSION_token,
  SipParser_OTHER_PARAM_token, SipParser_OTHER_PARAM_EXPR_token,
  SipParser_IPV_6_REFERENCE_token, SipParser_TTL_PARAM_token = 152,
  SipParser_TTL_token = 154, SipParser_RPORT_NAME_token,
  SipParser_RPORT_VAL_token, SipParser_MADDR_PARAM_token = 158,
  SipParser_HOST_NAME_FOR_RECEIVED_token = 161,
  SipParser_COMPRESSION_TYPE_token = 164,
  SipParser_IPV_6_ADDRESS_token = 166, SipParser_REQUEST_URI_token,
  SipParser_ADDR_SPEC_token, SipParser_ABSOLUTE_URI_token,
  SipParser_ABS_URI_SCHEME_token, SipParser_URI_IDENTIFIER_token,
  SipParser_ABS_URI_SCHEME_CHAR_token,
  SipParser_URI_SCHEME_OR_DISPLAY_NAME_token = 174,
  SipParser_DISPLAY_NAME_token, SipParser_URI_IDENTIFIER_CHAR_token,
  SipParser_URI_IDENTIFIER_WITHOUT_SEPARATORS_token = 178,
  SipParser_SIP_URL_token = 181, SipParser_PRES_URI_token,
  SipParser_IM_URI_token, SipParser_SIP_PREFIX_token, SipParser_URL_token,
  SipParser_PRES_PREFIX_token = 188, SipParser_RV_ABS_PREFIX_token,
  SipParser_IM_PREFIX_token = 192, SipParser_URL_USERINFO_token = 194,
  SipParser_OPTIONAL_URL_PARAMETERS_token, SipParser_OPTIONAL_HEADERS_token,
  SipParser_URL_HOST_token, SipParser_GENERAL_URL_WITHOUT_PARAM_token,
  SipParser_SIP_URL_WITHOUT_PARAMS_token,
  SipParser_PRES_URL_WITHOUT_PARAMS_token,
  SipParser_IM_URL_WITHOUT_PARAMS_token,
  SipParser_ABS_URI_WITHOUT_PARAMS_token,
  SipParser_URL_USERINFO_PREFIX_token, SipParser_URL_USERINFO_VAL_token,
  SipParser_HOSTPORT_token, SipParser_USER_INFO_TOKEN_token = 207,
  SipParser_PASSWORD_token, SipParser_TOKEN_USER_INFO_token,
  SipParser_TOKEN_USER_INFO_CHAR_token,
  SipParser_PASSWORD_STRING_token = 212,
  SipParser_PASSWORD_STRING_CHAR_token,
  SipParser_URL_HOST_PREFIX_token = 217, SipParser_TOKEN_HOST_token = 219,
  SipParser_ALPHANUM_token, SipParser_TOKEN_HOST_CHAR_token,
  SipParser_IPV_4_ADDRESS_token = 225, SipParser_IPV_4_PART_token,
  SipParser_HEXPART_token = 228, SipParser_HEXSEQ_token,
  SipParser_HEX4_token = 232, SipParser_URL_PARAMETERS_token = 236,
  SipParser_URL_PARAMETER_token, SipParser_TRANSPORT_PARAM_token,
  SipParser_USER_PARAM_token, SipParser_TTL_PARAM_URL_token,
  SipParser_MADDR_PARAM_URL_token, SipParser_METHOD_PARAM_URL_token,
  SipParser_LR_PARAM_URL_token, SipParser_COMPRESSION_PARAM_URL_token,
  SipParser_TOKENIZED_BY_PARAM_URL_token, SipParser_ORIG_PARAM_URL_token,
  SipParser_URL_GENERIC_PARAM_DATA_token,
  SipParser_USER_PARAM_TYPE_token = 250, SipParser_OTHER_USER_token,
  SipParser_URL_OTHER_PARAM_VAL_token = 263, SipParser_PVALUE_token,
  SipParser_PARAMCHAR_token, SipParser_HEADERS_LIST_token = 269,
  SipParser_HEADER_token, SipParser_HNAME_token = 272,
  SipParser_HVALUE_token, SipParser_REQUEST_LINE_VALUE_token = 280,
  SipParser_REQUEST_METHOD_token, SipParser_PARTY_VALUE_token = 295,
  SipParser_NAME_ADDR_token, SipParser_PARTY_PARAMS_LIST_token,
  SipParser_MORE_THAN_ONE_TOKEN_token = 299,
  SipParser_TAG_PARAM_token = 303, SipParser_PARTY_PARAM_DATA_token = 305,
  SipParser_CONTACT_HEADER_VALUES_token,
  SipParser_CONTACT_HEADER_OTHER_token,
  SipParser_CONTACT_HEADER_VALUE_LIST_token,
  SipParser_CONTACT_HEADER_VALUE_token = 310,
  SipParser_CONTACT_PARAMS_token, SipParser_CONTACT_PARAM_token,
  SipParser_Q_token, SipParser_ACTION_token,
  SipParser_CONTACT_EXPIRES_token, SipParser_Q_VALUE_token = 317,
  SipParser_Q_VALUE_OPTIONAL_token = 319,
  SipParser_ACTION_VALUE_token = 323, SipParser_EXPIRES_VALUE_token = 326,
  SipParser_EXPIRES_VALUE_FOR_CONTACT_token = 328,
  SipParser_DELTA_SECONDS_token, SipParser_SIP_DATE_token,
  SipParser_SIP_DATE_HEADER_VAL_token, SipParser_DATE_token,
  SipParser_TIME_token, SipParser_DAY_token, SipParser_DD_token = 342,
  SipParser_MM_token, SipParser_YY_token, SipParser_HOUR_token = 357,
  SipParser_MINUTE_token, SipParser_SECOND_token, SipParser_TIME_ZONE_token,
  SipParser_AUTHENTICATION_VAL_token = 364, SipParser_CHALLENGE_VALUE_token,
  SipParser_DIGEST_CHALLENGE_LIST_token = 367,
  SipParser_AUTH_PARAM_LIST_token, SipParser_DIGEST_CHALLENGE_token,
  SipParser_REALM_token, SipParser_DOMAIN_token, SipParser_NONCE_token,
  SipParser_OPAQUE_token, SipParser_INTEGRITY_KEY_token,
  SipParser_CIPHER_KEY_token, SipParser_AUTH_PARAM_token,
  SipParser_STALE_token, SipParser_ALGORITHM_token,
  SipParser_QOP_OPTIONS_token, SipParser_PARAM_IN_NEXT_CHALLENGE_token,
  SipParser_REALM_VALUE_token = 382, SipParser_DOMAIN_EQUAL_token = 384,
  SipParser_DOMAIN_WHITE_token, SipParser_DOMAIN_VAL_token = 387,
  SipParser_URI_LIST_token, SipParser_STRING_DOMAIN_PATCH_token,
  SipParser_DOMAIN_SPACE_token, SipParser_NONCE_VALUE_token = 393,
  SipParser_OPAQUE_VALUE_token = 395,
  SipParser_ALGORITHM_DECLARATION_token = 398,
  SipParser_ALGORITHM_AND_AKAV_token, SipParser_AKAV_PREFIX_token = 402,
  SipParser_AKAV_token, SipParser_AKA_VERSION_VAL_token,
  SipParser_ALGORITHM_VAL_token, SipParser_ALGORITHM_PREFIX_token,
  SipParser_QOP_VALUE_LIST_token = 414, SipParser_QOP_VALUE_token,
  SipParser_QUOTED_HEX_VALUE_token = 419,
  SipParser_AUTH_PARAM_DATA_token = 421, SipParser_AUTHORIZATION_VAL_token,
  SipParser_CREDENTIALS_VALUE_token, SipParser_DIGEST_RESPONSE_LIST_token,
  SipParser_DIGEST_RESPONSE_token, SipParser_USERNAME_token,
  SipParser_RESPONSE_token, SipParser_CNONCE_token,
  SipParser_NONCE_COUNT_token, SipParser_AUTS_PARAM_token,
  SipParser_INTEGRITY_PROTECTED_token, SipParser_MESSAGE_QOP_token,
  SipParser_DIGEST_URI_token, SipParser_USERNAME_VALUE_token = 435,
  SipParser_DIGEST_URI_VALUE_token = 437,
  SipParser_CNONCE_VALUE_token = 440, SipParser_NC_VALUE_token = 442,
  SipParser_LHEX_token, SipParser_REQUEST_DIGEST_token = 445,
  SipParser_THIRTY_TWO_LHEX_token,
  SipParser_SIXTY_FOUR_BASE_ENCODING_token = 450,
  SipParser_SIXTY_FOUR_BASE_ENCODING_CHAR_token,
  SipParser_ONE_OR_TWO_EQUAL_SIGNS_token = 453,
  SipParser_ROUTE_LIST_token = 460, SipParser_SINGLE_ROUTE_token,
  SipParser_AUTHENTICATION_INFO_HEADER_LIST_token,
  SipParser_AUTHENTICATION_INFO_HEADER_PARAM_token,
  SipParser_NEXT_NONCE_token, SipParser_RESPONSE_AUTH_token,
  SipParser_MSG_QOP_token, SipParser_RESPONSE_AUTH_VAL_token = 469
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
