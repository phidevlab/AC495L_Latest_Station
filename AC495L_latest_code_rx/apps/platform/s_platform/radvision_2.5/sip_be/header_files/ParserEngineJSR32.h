#if defined(RV_SIP_AUTH_ON) && defined(RV_SIP_TEL_URI_SUPPORT) && defined(RV_SIP_SUBS_ON) && defined(RV_SIP_JSR32_SUPPORT) && defined(RV_SIP_EXTENDED_HEADER_SUPPORT) && !defined(RV_SIP_IMS_DCS_HEADER_SUPPORT) && !defined(RV_SIP_IMS_HEADER_SUPPORT) && !defined(RV_SIP_LIGHT) && !defined(RV_SIP_PRIMITIVES)
#ifndef PARSERENGINEJSR32_H_1165245724
#define PARSERENGINEJSR32_H_1165245724

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
  char ag_vt_32[sizeof(ParserDisplayName)];
  char ag_vt_33[sizeof(ParserAbsoluteUri)];
  char ag_vt_34[sizeof(ParserEnumdiParamType)];
  char ag_vt_35[sizeof(ParserUserParam)];
  char ag_vt_36[sizeof(RvSipUserParam)];
  char ag_vt_37[sizeof(ParserOtherUser)];
  char ag_vt_38[sizeof(ParserLrParamType)];
  char ag_vt_39[sizeof(ParserTokenizedByParam)];
  char ag_vt_40[sizeof(RvBool)];
  char ag_vt_41[sizeof(ParserOptionalHeaders)];
  char ag_vt_42[sizeof(ParserTagParam)];
  char ag_vt_43[sizeof(ParserPartyParams)];
  char ag_vt_44[sizeof(ParserQVal)];
  char ag_vt_45[sizeof(ParserContactActionType)];
  char ag_vt_46[sizeof(ParserDeltaSeconds)];
  char ag_vt_47[sizeof(ParserSipDate)];
  char ag_vt_48[sizeof(RvSipDateWeekDay)];
  char ag_vt_49[sizeof(ParserDDMMYY)];
  char ag_vt_50[sizeof(ParserInt32)];
  char ag_vt_51[sizeof(RvSipDateMonth)];
  char ag_vt_52[sizeof(ParserTime)];
  char ag_vt_53[sizeof(ParserTimeZone)];
  char ag_vt_54[sizeof(RvSipAuthStale)];
  char ag_vt_55[sizeof(ParserAlgorithm)];
  char ag_vt_56[sizeof(ParserQopValue)];
  char ag_vt_57[sizeof(ParserAuthQopOptions)];
  char ag_vt_58[sizeof(RvSipAuthQopOption)];
  char ag_vt_59[sizeof(RvSipAuthIntegrityProtected)];
  char ag_vt_60[sizeof(ParserRetryParam)];
  char ag_vt_61[sizeof(ParserRSeq)];
  char ag_vt_62[sizeof(ParserResponseNum)];
  char ag_vt_63[sizeof(ParserEventType)];
  char ag_vt_64[sizeof(ParserSubsStateValue)];
  char ag_vt_65[sizeof(ParserSubsStateReason)];
  char ag_vt_66[sizeof(ParserDisposition)];
  char ag_vt_67[sizeof(ParserDispositionParams)];
  char ag_vt_68[sizeof(ParserDispositionParam)];
  char ag_vt_69[sizeof(ParserSessionExpiresHeader)];
  char ag_vt_70[sizeof(RvSipSessionExpiresRefresherType)];
  char ag_vt_71[sizeof(ParserMinSEHeader)];
  char ag_vt_72[sizeof(ParserReplacesEarlyFlagParamType)];
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
  SipParser_REASON_HEADER_token = 61, SipParser_WARNING_HEADER_token = 63,
  SipParser_TIMESTAMP_HEADER_token = 65, SipParser_INFO_HEADER_token = 67,
  SipParser_ACCEPT_HEADER_token = 69,
  SipParser_ACCEPT_ENCODING_HEADER_token = 71,
  SipParser_ACCEPT_LANGUAGE_HEADER_token = 73,
  SipParser_REPLY_TO_HEADER_token = 75, SipParser_STATUS_LINE_token,
  SipParser_REQUEST_LINE_token, SipParser_URI_NAME_ADDR_token = 79,
  SipParser_OTHER_HEADER_token, SipParser_WHITE_token = 82,
  SipParser_SP_token = 84, SipParser_COMMA_token = 86,
  SipParser_COMMA_BETWEEN_HEADERS_token = 88, SipParser_DOT_token,
  SipParser_EQUAL_token = 93, SipParser_AT_token = 95,
  SipParser_COLON_token = 97, SipParser_SEMI_COLON_token,
  SipParser_MARK_SIGN_token = 100, SipParser_SLASH_token = 102,
  SipParser_LEFT_PARENTHESIS_token = 104,
  SipParser_LEFT_ANGLE_BRACKET_token = 106,
  SipParser_LEFT_QUOTATION_token = 108, SipParser_HYPHEN_token = 110,
  SipParser_TOKEN_token = 112, SipParser_TOKEN_CHAR_token,
  SipParser_CALL_ID_WORD_token = 115, SipParser_CALL_ID_WORD_CHAR_token,
  SipParser_TOKEN_FOR_CONTENT_TYPE_token = 118,
  SipParser_TOKEN_CHAR_FOR_CONTENT_TYPE_token,
  SipParser_TOKEN_NO_DOT_token = 121, SipParser_TOKEN_CHAR_NO_DOT_token,
  SipParser_FIELD_VALUE_token = 124, SipParser_QUOTED_STRING_token = 130,
  SipParser_QUOTED_CHAR_token, SipParser_HEX_VALUE_token = 138,
  SipParser_HEX_token, SipParser_METHOD_token = 141,
  SipParser_CSEQ_VAL_token = 151, SipParser_SEQUNCE_NUMBER_token,
  SipParser_DIGIT_token, SipParser_CONTENT_LENGTH_VAL_token = 155,
  SipParser_CALL_ID_token, SipParser_MEDIA_TYPE_token,
  SipParser_MEDIA_TYPE_TYPE_token, SipParser_MEDIA_TYPE_SUB_TYPE_token,
  SipParser_OPTIONAL_CONTENT_TYPE_PARAM_LIST_token,
  SipParser_CONTENT_TYPE_PARAM_LIST_token, SipParser_BOUNDARY_PARAM_token,
  SipParser_VERSION_PARAM_token, SipParser_BASE_PARAM_token,
  SipParser_CONTENT_TYPE_GENERIC_PARAM_DATA_token,
  SipParser_CONTENT_TYPE_GENERIC_PARAM_DATA_VAL_token = 185,
  SipParser_GENERIC_PARAM_LIST_token, SipParser_GENERIC_PARAM_DATA_token,
  SipParser_GENERIC_PARAM_DATA_VAL_token, SipParser_SIP_VERSION_token,
  SipParser_STATUS_CODE_token = 191, SipParser_REASON_PHRASE_token,
  SipParser_EXTENSION_CODE_token = 194, SipParser_REASONE_TEXT_UTF8_token,
  SipParser_ALLOW_VAL_token = 199, SipParser_VIA_LIST_token,
  SipParser_SINGLE_VIA_token, SipParser_SENT_PROTOCOL_token,
  SipParser_SENT_BY_token, SipParser_VIA_PARAM_LIST_token,
  SipParser_VIA_PARAM_token, SipParser_PROTOCOL_NAME_token,
  SipParser_PROTOCOL_VERSION_token, SipParser_TRANSPORT_token,
  SipParser_TRANSPORT_TYPE_token = 210, SipParser_PORT_token = 215,
  SipParser_HOST_NAME_token, SipParser_VIA_HIDDEN_token,
  SipParser_VIA_ALIAS_token, SipParser_VIA_TTL_token,
  SipParser_VIA_MADDR_token, SipParser_VIA_RECEIVED_token,
  SipParser_VIA_BRANCH_token, SipParser_RESPONSE_PORT_token,
  SipParser_VIA_COMPRESSION_token, SipParser_OTHER_PARAM_token,
  SipParser_OTHER_PARAM_EXPR_token, SipParser_IPV_6_REFERENCE_token,
  SipParser_TTL_PARAM_token = 230, SipParser_TTL_token = 232,
  SipParser_RPORT_NAME_token, SipParser_RPORT_VAL_token,
  SipParser_MADDR_PARAM_token = 236,
  SipParser_HOST_NAME_FOR_RECEIVED_token = 239,
  SipParser_COMPRESSION_TYPE_token = 242,
  SipParser_IPV_6_ADDRESS_token = 244, SipParser_TEST_COMMENT_token,
  SipParser_COMMENT_TEXT_token, SipParser_END_COMMENT_token = 248,
  SipParser_URI_WITH_DISPLAY_NAME_token = 251,
  SipParser_URI_WITHOUT_DISPLAY_NAME_token,
  SipParser_QUOTED_DISPLAY_NAME_token, SipParser_URI_token,
  SipParser_DISPLAY_NAME_TYPE_ONE_token = 256,
  SipParser_DISPLAY_NAME_TYPE_TWO_token,
  SipParser_DISPLAY_NAME_OR_ABS_URI_SCHEME_token,
  SipParser_CHAR_ONLY_FOR_DISPLAY_NAME_token,
  SipParser_MORE_THAN_ONE_TOKEN_token, SipParser_ABS_URI_SCHEME_CHAR_token,
  SipParser_ABS_URI_SCHEME_token = 263, SipParser_ABSOLUTE_URI_token,
  SipParser_SIP_URL_token, SipParser_TEL_URI_token,
  SipParser_REQUEST_URI_token, SipParser_ADDR_SPEC_token,
  SipParser_URI_IDENTIFIER_token,
  SipParser_URI_SCHEME_OR_DISPLAY_NAME_token, SipParser_DISPLAY_NAME_token,
  SipParser_URI_IDENTIFIER_CHAR_token,
  SipParser_URI_IDENTIFIER_WITHOUT_SEPARATORS_token = 274,
  SipParser_TEL_PREFIX_token = 277, SipParser_TEL_token,
  SipParser_GLOBAL_OR_LOCAL_NUMBER_token = 280,
  SipParser_OPTIONAL_TEL_URI_PARAMETERS_token,
  SipParser_GLOBAL_NUMBER_token, SipParser_LOCAL_NUMBER_token,
  SipParser_GLOBAL_NUMBER_DIGITS_token, SipParser_PHONEDIGIT_token = 286,
  SipParser_VISUAL_SEPERATOR_token = 290,
  SipParser_LOCAL_NUMBER_DIGITS_token = 292,
  SipParser_HEX_OR_PHONECHAR_token, SipParser_PHONEDIGIT_HEX_token,
  SipParser_PHONECHAR_token = 298, SipParser_TEL_URI_PARAMETERS_token,
  SipParser_TEL_URI_PARAMETER_token,
  SipParser_TEL_URI_EXTENSION_PARAM_token,
  SipParser_TEL_URI_POST_DIAL_PARAM_token,
  SipParser_TEL_URI_ISDN_SUB_ADDR_PARAM_URL_token,
  SipParser_TEL_URI_CONTEXT_PARAM_token,
  SipParser_TEL_URI_ENUMDI_PARAM_token,
  SipParser_URL_GENERIC_PARAM_DATA_token,
  SipParser_TEL_URI_EXTENTION_VALUE_token = 308,
  SipParser_TEL_URI_POST_DIAL_VALUE_token = 311,
  SipParser_TEL_URI_POST_DIAL_CHAR_token, SipParser_DTMFDIGIT_token = 314,
  SipParser_PAUSE_CHAR_token,
  SipParser_TEL_URI_ISDN_SUB_ADDR_VALUE_token = 317, SipParser_URIC_token,
  SipParser_RESERVED_FOR_URIC_token = 320, SipParser_UNRESERVED_token,
  SipParser_PCT_ENCODED_token, SipParser_TEL_URI_CONTEXT_VALUE_token = 325,
  SipParser_DESCRIPTOR_token, SipParser_DOMAINNAME_token,
  SipParser_LABEL_token, SipParser_NEXT_LABEL_token,
  SipParser_ALPHA_token = 333, SipParser_ALPHANUM_OR_HYPHEN_token,
  SipParser_ALPHANUM_token = 338, SipParser_SIP_PREFIX_token = 340,
  SipParser_URL_token, SipParser_URL_USERINFO_token = 344,
  SipParser_OPTIONAL_URL_PARAMETERS_token, SipParser_OPTIONAL_HEADERS_token,
  SipParser_URL_HOST_token, SipParser_GENERAL_URL_WITHOUT_PARAM_token,
  SipParser_SIP_URL_WITHOUT_PARAMS_token,
  SipParser_TEL_URL_WITHOUT_PARAMS_token,
  SipParser_ABS_URI_WITHOUT_PARAMS_token,
  SipParser_URL_USERINFO_PREFIX_token, SipParser_URL_USERINFO_VAL_token,
  SipParser_HOSTPORT_token, SipParser_USER_INFO_TOKEN_token = 356,
  SipParser_PASSWORD_token, SipParser_TOKEN_USER_INFO_token,
  SipParser_TOKEN_USER_INFO_CHAR_token,
  SipParser_PASSWORD_STRING_token = 361,
  SipParser_PASSWORD_STRING_CHAR_token,
  SipParser_URL_HOST_PREFIX_token = 366, SipParser_TOKEN_HOST_token = 368,
  SipParser_TOKEN_HOST_CHAR_token, SipParser_IPV_4_ADDRESS_token = 373,
  SipParser_IPV_4_PART_token, SipParser_HEXPART_token,
  SipParser_HEXSEQ_token, SipParser_HEX4_token = 379,
  SipParser_URL_PARAMETERS_token = 383, SipParser_URL_PARAMETER_token,
  SipParser_TRANSPORT_PARAM_token, SipParser_USER_PARAM_token,
  SipParser_TTL_PARAM_URL_token, SipParser_MADDR_PARAM_URL_token,
  SipParser_METHOD_PARAM_URL_token, SipParser_LR_PARAM_URL_token,
  SipParser_COMPRESSION_PARAM_URL_token,
  SipParser_TOKENIZED_BY_PARAM_URL_token, SipParser_ORIG_PARAM_URL_token,
  SipParser_USER_PARAM_TYPE_token = 396, SipParser_OTHER_USER_token,
  SipParser_URL_OTHER_PARAM_VAL_token = 409, SipParser_PVALUE_token,
  SipParser_PARAMCHAR_token, SipParser_HEADERS_LIST_token = 415,
  SipParser_HEADER_token, SipParser_HNAME_token = 418,
  SipParser_HVALUE_token, SipParser_REQUEST_LINE_VALUE_token = 426,
  SipParser_REQUEST_METHOD_token, SipParser_PARTY_VALUE_token = 441,
  SipParser_NAME_ADDR_token, SipParser_PARTY_PARAMS_LIST_token,
  SipParser_TAG_PARAM_token = 447, SipParser_PARTY_PARAM_DATA_token = 449,
  SipParser_CONTACT_HEADER_VALUES_token,
  SipParser_CONTACT_HEADER_OTHER_token,
  SipParser_CONTACT_HEADER_VALUE_LIST_token,
  SipParser_CONTACT_HEADER_VALUE_token = 454,
  SipParser_CONTACT_PARAMS_token, SipParser_CONTACT_PARAM_token,
  SipParser_Q_token, SipParser_ACTION_token,
  SipParser_CONTACT_EXPIRES_token, SipParser_Q_VALUE_token = 461,
  SipParser_Q_VALUE_OPTIONAL_token = 463,
  SipParser_ACTION_VALUE_token = 467, SipParser_EXPIRES_VALUE_token = 470,
  SipParser_EXPIRES_VALUE_FOR_CONTACT_token = 472,
  SipParser_DELTA_SECONDS_token, SipParser_SIP_DATE_HEADER_VAL_token,
  SipParser_DATE_token, SipParser_TIME_token, SipParser_SIP_DATE_token,
  SipParser_DAY_token, SipParser_DD_token = 486, SipParser_MM_token,
  SipParser_YY_token, SipParser_HOUR_token = 501, SipParser_MINUTE_token,
  SipParser_SECOND_token, SipParser_TIME_ZONE_token,
  SipParser_AUTHENTICATION_VAL_token = 508, SipParser_CHALLENGE_VALUE_token,
  SipParser_DIGEST_CHALLENGE_LIST_token, SipParser_AUTH_PARAM_LIST_token,
  SipParser_DIGEST_CHALLENGE_token, SipParser_REALM_token,
  SipParser_DOMAIN_token, SipParser_NONCE_token, SipParser_OPAQUE_token,
  SipParser_INTEGRITY_KEY_token, SipParser_CIPHER_KEY_token,
  SipParser_AUTH_PARAM_token, SipParser_STALE_token,
  SipParser_ALGORITHM_token, SipParser_QOP_OPTIONS_token,
  SipParser_PARAM_IN_NEXT_CHALLENGE_token,
  SipParser_REALM_VALUE_token = 525, SipParser_DOMAIN_EQUAL_token = 527,
  SipParser_DOMAIN_WHITE_token, SipParser_DOMAIN_VAL_token = 530,
  SipParser_URI_LIST_token, SipParser_STRING_DOMAIN_PATCH_token,
  SipParser_DOMAIN_SPACE_token, SipParser_NONCE_VALUE_token = 536,
  SipParser_OPAQUE_VALUE_token = 538,
  SipParser_ALGORITHM_DECLARATION_token = 541,
  SipParser_ALGORITHM_AND_AKAV_token, SipParser_AKAV_PREFIX_token = 545,
  SipParser_AKAV_token, SipParser_AKA_VERSION_VAL_token,
  SipParser_ALGORITHM_VAL_token, SipParser_ALGORITHM_PREFIX_token,
  SipParser_QOP_VALUE_LIST_token = 557, SipParser_QOP_VALUE_token,
  SipParser_QUOTED_HEX_VALUE_token = 562,
  SipParser_AUTH_PARAM_DATA_token = 564, SipParser_AUTHORIZATION_VAL_token,
  SipParser_CREDENTIALS_VALUE_token, SipParser_DIGEST_RESPONSE_LIST_token,
  SipParser_DIGEST_RESPONSE_token, SipParser_USERNAME_token,
  SipParser_RESPONSE_token, SipParser_CNONCE_token,
  SipParser_NONCE_COUNT_token, SipParser_AUTS_PARAM_token,
  SipParser_INTEGRITY_PROTECTED_token, SipParser_MESSAGE_QOP_token,
  SipParser_DIGEST_URI_token, SipParser_USERNAME_VALUE_token = 578,
  SipParser_DIGEST_URI_VALUE_token = 580,
  SipParser_CNONCE_VALUE_token = 583, SipParser_NC_VALUE_token = 585,
  SipParser_LHEX_token, SipParser_REQUEST_DIGEST_token = 588,
  SipParser_THIRTY_TWO_LHEX_token,
  SipParser_SIXTY_FOUR_BASE_ENCODING_token = 593,
  SipParser_SIXTY_FOUR_BASE_ENCODING_CHAR_token,
  SipParser_ONE_OR_TWO_EQUAL_SIGNS_token = 596,
  SipParser_ROUTE_LIST_token = 603, SipParser_SINGLE_ROUTE_token,
  SipParser_RETRY_AFTER_VAL_token, SipParser_RETRY_AFTER_COMMENT_token,
  SipParser_RETRY_PARAM_token = 608, SipParser_DURATION_PARAM_token,
  SipParser_RSEQ_VAL_token = 611, SipParser_RESPONSE_NUM_token,
  SipParser_RACK_VAL_token, SipParser_OPTION_TAG_token,
  SipParser_OPTION_TAG_LIST_token, SipParser_SUPPORTED_OPTION_TAG_token,
  SipParser_REFER_TO_HEADER_VALUE_token,
  SipParser_REFERRED_BY_HEADER_VALUE_token = 619,
  SipParser_REFERRER_URI_token, SipParser_REFERRED_BY_PARAMS_token,
  SipParser_REFERRED_BY_PARAM_token = 623,
  SipParser_REFERRED_BY_PARAM_CID_token,
  SipParser_CID_PARAM_STRING_token = 626, SipParser_DOT_ATOM_token,
  SipParser_ATOM_CHAR_token, SipParser_DOT_ATOM_CHAR_token,
  SipParser_EVENT_HEADER_VAL_token = 632, SipParser_EVENT_TYPE_token,
  SipParser_EVENT_PARAMS_token, SipParser_EVENT_PACKAGE_token,
  SipParser_EVENT_TEMPLATE_token, SipParser_EVENT_PARAM_token,
  SipParser_EVENT_ID_token, SipParser_ALLOW_EVENTS_HEADER_VAL_token = 640,
  SipParser_SUBSCRIPTION_STATE_HEADER_VAL_token,
  SipParser_SUBSTATE_VALUE_token, SipParser_SUBSTATE_PARAMS_token,
  SipParser_SUBSTATE_PARAM_token = 647,
  SipParser_SUBSTATE_PARAM_REASON_token,
  SipParser_SUBSTATE_PARAM_RETRY_AFTER_token,
  SipParser_SUBSTATE_PARAM_EXPIRES_token,
  SipParser_SUBSTATE_PARAM_REASON_VAL_token = 653,
  SipParser_DISPOSITION_VAL_token = 660, SipParser_DISPOSITION_TYPE_token,
  SipParser_DISPOSITION_PARAMS_token, SipParser_HANDLING_PARAM_token = 668,
  SipParser_SESSION_EXPIRES_VALUE_token = 672,
  SipParser_REFRESHER_TYPE_token, SipParser_REFRESHER_TYPE_NAME_token,
  SipParser_REFRESHER_TYPE_VALUE_token, SipParser_MIN_SE_VALUE_token = 679,
  SipParser_REPLACES_HEADER_VALUE_token, SipParser_REPLACES_PARAMS_token,
  SipParser_REPLACES_PARAM_token, SipParser_TO_TAG_PARAM_token,
  SipParser_FROM_TAG_PARAM_token, SipParser_EARLY_FLAG_PARAM_token,
  SipParser_AUTHENTICATION_INFO_HEADER_LIST_token = 689,
  SipParser_AUTHENTICATION_INFO_HEADER_PARAM_token,
  SipParser_NEXT_NONCE_token, SipParser_RESPONSE_AUTH_token,
  SipParser_MSG_QOP_token, SipParser_RESPONSE_AUTH_VAL_token = 696,
  SipParser_REASON_HEADERS_LIST_token = 700,
  SipParser_REASON_HEADER_VALUE_token, SipParser_REASON_PROTOCOL_token,
  SipParser_REASON_PARAMS_token, SipParser_REASON_PARAM_token = 706,
  SipParser_REASON_CAUSE_PARAM_token, SipParser_REASON_TEXT_PARAM_token,
  SipParser_REASON_CAUSE_PARAM_STRING_token = 710,
  SipParser_WARNING_HEADERS_LIST_token,
  SipParser_WARNING_HEADER_VALUE_token, SipParser_WARN_CODE_token,
  SipParser_WARN_AGENT_token, SipParser_WARN_TEXT_token,
  SipParser_WARN_AGENT_HOSTPORT_token, SipParser_WARN_AGENT_PSEUDONYM_token,
  SipParser_WARN_AGENT_HOST_token,
  SipParser_TOKEN_CHAR_THAT_CANNOT_BE_IN_HOST_token,
  SipParser_TOKEN_CHAR_THAT_IS_NOT_ALPHANUM_token,
  SipParser_TIMESTAMP_HEADER_VALUE_token, SipParser_TIMESTAMP_VAL_token,
  SipParser_DELAY_VAL_token, SipParser_TIMESTAMP_INT_VAL_token = 725,
  SipParser_TIMESTAMP_DEC_VAL_token, SipParser_DELAY_INT_VAL_token = 728,
  SipParser_DELAY_DEC_VAL_token, SipParser_INFO_HEADERS_LIST_token = 731,
  SipParser_INFO_HEADER_VALUE_token, SipParser_ACCEPT_HEADERS_LIST_token,
  SipParser_ACCEPT_HEADER_VALUE_token, SipParser_ACCEPT_MEDIA_token,
  SipParser_ACCEPT_PARAMS_token, SipParser_ACCEPT_PARAM_token,
  SipParser_ACCEPT_ENCODING_HEADERS_LIST_token,
  SipParser_ACCEPT_ENCODING_HEADER_VALUE_token, SipParser_CODING_token,
  SipParser_ACCEPT_ENCODING_PARAMS_token, SipParser_CODING_VAL_token,
  SipParser_ACCEPT_ENCODING_PARAM_token,
  SipParser_ACCEPT_LANGUAGE_HEADERS_LIST_token,
  SipParser_ACCEPT_LANGUAGE_HEADER_VALUE_token, SipParser_LANGUAGE_token,
  SipParser_ACCEPT_LANGUAGE_PARAMS_token, SipParser_LANGUAGE_VAL_token,
  SipParser_UP_TO_EIGHT_ALPHAS_token = 750, SipParser_LANGUAGE_RANGE_token,
  SipParser_ACCEPT_LANGUAGE_PARAM_token, SipParser_REPLY_TO_VALUE_token,
  SipParser_REPLY_TO_PARAMS_token
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

		ParserTelUri            telUri;         
        ParserTelUriParameter   telUriParameter;
		RvBool                  isTopLabel;     


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


		ParserReasonHeader             reasonHeader;
		ParserWarningHeader            warningHeader; 
		ParserTimestampHeader          timestampHeader; 
		ParserInfoHeader               infoHeader; 
		ParserAcceptHeader             acceptHeader; 
		ParserAcceptEncodingHeader     acceptEncodingHeader; 
		ParserAcceptLanguageHeader     acceptLanguageHeader; 
		ParserReplyToHeader            replyToHeader; 


















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
