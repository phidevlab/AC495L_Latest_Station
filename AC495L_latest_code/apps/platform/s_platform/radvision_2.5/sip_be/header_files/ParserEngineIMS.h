#if defined(RV_SIP_AUTH_ON) && defined(RV_SIP_TEL_URI_SUPPORT) && defined(RV_SIP_IMS_HEADER_SUPPORT) && !defined(RV_SIP_IMS_DCS_HEADER_SUPPORT) && !defined(RV_SIP_EXTENDED_HEADER_SUPPORT) && !defined(RV_SIP_JSR32_SUPPORT) && !defined(RV_SIP_LIGHT) && !defined(RV_SIP_PRIMITIVES) && defined(RV_SIP_SUBS_ON)
#ifndef PARSERENGINEIMS_H_1165245753
#define PARSERENGINEIMS_H_1165245753

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
  char ag_vt_73[sizeof(ParserAccess)];
  char ag_vt_74[sizeof(ParserMechanism)];
  char ag_vt_75[sizeof(ParserSecurityAlgorithmType)];
  char ag_vt_76[sizeof(ParserSecurityProtocolType)];
  char ag_vt_77[sizeof(ParserSecurityModeType)];
  char ag_vt_78[sizeof(ParserSecurityEncryptAlgorithmType)];
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
  SipParser_TEL_URI_HEADER_token, SipParser_P_URI_HEADER_token = 69,
  SipParser_P_VISITED_NETWORK_ID_HEADER_token = 71,
  SipParser_P_ACCESS_NETWORK_INFO_HEADER_token = 73,
  SipParser_P_CHARGING_FUNCTION_ADDRESSES_HEADER_token = 75,
  SipParser_P_CHARGING_VECTOR_HEADER_token = 77,
  SipParser_P_MEDIA_AUTHORIZATION_HEADER_token = 79,
  SipParser_SECURITY_HEADER_token = 81, SipParser_OTHER_HEADER_token,
  SipParser_WHITE_token = 84, SipParser_SP_token = 86,
  SipParser_COMMA_token = 88, SipParser_COMMA_BETWEEN_HEADERS_token = 90,
  SipParser_DOT_token, SipParser_EQUAL_token = 95, SipParser_AT_token = 97,
  SipParser_COLON_token = 99, SipParser_SEMI_COLON_token,
  SipParser_MARK_SIGN_token = 102, SipParser_SLASH_token = 104,
  SipParser_LEFT_PARENTHESIS_token = 106,
  SipParser_LEFT_ANGLE_BRACKET_token = 108,
  SipParser_LEFT_QUOTATION_token = 110, SipParser_HYPHEN_token = 112,
  SipParser_TOKEN_token = 114, SipParser_TOKEN_CHAR_token,
  SipParser_CALL_ID_WORD_token = 117, SipParser_CALL_ID_WORD_CHAR_token,
  SipParser_TOKEN_FOR_CONTENT_TYPE_token = 120,
  SipParser_TOKEN_CHAR_FOR_CONTENT_TYPE_token,
  SipParser_TOKEN_NO_DOT_token = 123, SipParser_TOKEN_CHAR_NO_DOT_token,
  SipParser_FIELD_VALUE_token = 126, SipParser_QUOTED_STRING_token = 132,
  SipParser_QUOTED_CHAR_token, SipParser_HEX_VALUE_token = 140,
  SipParser_HEX_token, SipParser_INT_NUMBER_token = 143,
  SipParser_DIGIT_token, SipParser_TOKEN_OR_QUOTED_STRING_token = 146,
  SipParser_METHOD_token, SipParser_CSEQ_VAL_token = 157,
  SipParser_SEQUNCE_NUMBER_token, SipParser_CONTENT_LENGTH_VAL_token,
  SipParser_CALL_ID_token, SipParser_MEDIA_TYPE_token,
  SipParser_MEDIA_TYPE_TYPE_token, SipParser_MEDIA_TYPE_SUB_TYPE_token,
  SipParser_OPTIONAL_CONTENT_TYPE_PARAM_LIST_token,
  SipParser_CONTENT_TYPE_PARAM_LIST_token, SipParser_BOUNDARY_PARAM_token,
  SipParser_VERSION_PARAM_token, SipParser_BASE_PARAM_token,
  SipParser_CONTENT_TYPE_GENERIC_PARAM_DATA_token,
  SipParser_CONTENT_TYPE_GENERIC_PARAM_DATA_VAL_token = 189,
  SipParser_GENERIC_PARAM_LIST_token, SipParser_GENERIC_PARAM_DATA_token,
  SipParser_GENERIC_PARAM_DATA_VAL_token, SipParser_SIP_VERSION_token,
  SipParser_STATUS_CODE_token = 195, SipParser_REASON_PHRASE_token,
  SipParser_EXTENSION_CODE_token = 198, SipParser_REASONE_TEXT_UTF8_token,
  SipParser_ALLOW_VAL_token = 203, SipParser_VIA_LIST_token,
  SipParser_SINGLE_VIA_token, SipParser_SENT_PROTOCOL_token,
  SipParser_SENT_BY_token, SipParser_VIA_PARAM_LIST_token,
  SipParser_VIA_PARAM_token, SipParser_PROTOCOL_NAME_token,
  SipParser_PROTOCOL_VERSION_token, SipParser_TRANSPORT_token,
  SipParser_TRANSPORT_TYPE_token = 214, SipParser_PORT_token = 219,
  SipParser_HOST_NAME_token, SipParser_VIA_HIDDEN_token,
  SipParser_VIA_ALIAS_token, SipParser_VIA_TTL_token,
  SipParser_VIA_MADDR_token, SipParser_VIA_RECEIVED_token,
  SipParser_VIA_BRANCH_token, SipParser_RESPONSE_PORT_token,
  SipParser_VIA_COMPRESSION_token, SipParser_OTHER_PARAM_token,
  SipParser_OTHER_PARAM_EXPR_token, SipParser_IPV_6_REFERENCE_token,
  SipParser_TTL_PARAM_token = 234, SipParser_TTL_token = 236,
  SipParser_RPORT_NAME_token, SipParser_RPORT_VAL_token,
  SipParser_MADDR_PARAM_token = 240,
  SipParser_HOST_NAME_FOR_RECEIVED_token = 243,
  SipParser_COMPRESSION_TYPE_token = 246,
  SipParser_IPV_6_ADDRESS_token = 248, SipParser_TEST_COMMENT_token,
  SipParser_COMMENT_TEXT_token, SipParser_END_COMMENT_token = 252,
  SipParser_REQUEST_URI_token = 255, SipParser_ADDR_SPEC_token,
  SipParser_ABSOLUTE_URI_token, SipParser_ABS_URI_SCHEME_token,
  SipParser_URI_IDENTIFIER_token, SipParser_ABS_URI_SCHEME_CHAR_token,
  SipParser_URI_SCHEME_OR_DISPLAY_NAME_token = 262,
  SipParser_DISPLAY_NAME_token, SipParser_URI_IDENTIFIER_CHAR_token,
  SipParser_URI_IDENTIFIER_WITHOUT_SEPARATORS_token = 266,
  SipParser_TEL_URI_token = 269, SipParser_TEL_PREFIX_token,
  SipParser_TEL_token, SipParser_GLOBAL_OR_LOCAL_NUMBER_token = 273,
  SipParser_OPTIONAL_TEL_URI_PARAMETERS_token,
  SipParser_GLOBAL_NUMBER_token, SipParser_LOCAL_NUMBER_token,
  SipParser_GLOBAL_NUMBER_DIGITS_token, SipParser_PHONEDIGIT_token = 279,
  SipParser_VISUAL_SEPERATOR_token = 283,
  SipParser_LOCAL_NUMBER_DIGITS_token = 285,
  SipParser_HEX_OR_PHONECHAR_token, SipParser_PHONEDIGIT_HEX_token,
  SipParser_PHONECHAR_token = 291, SipParser_TEL_URI_PARAMETERS_token,
  SipParser_TEL_URI_PARAMETER_token,
  SipParser_TEL_URI_EXTENSION_PARAM_token,
  SipParser_TEL_URI_POST_DIAL_PARAM_token,
  SipParser_TEL_URI_ISDN_SUB_ADDR_PARAM_URL_token,
  SipParser_TEL_URI_CONTEXT_PARAM_token,
  SipParser_TEL_URI_ENUMDI_PARAM_token,
  SipParser_URL_GENERIC_PARAM_DATA_token,
  SipParser_TEL_URI_EXTENTION_VALUE_token = 301,
  SipParser_TEL_URI_POST_DIAL_VALUE_token = 304,
  SipParser_TEL_URI_POST_DIAL_CHAR_token, SipParser_DTMFDIGIT_token = 307,
  SipParser_PAUSE_CHAR_token,
  SipParser_TEL_URI_ISDN_SUB_ADDR_VALUE_token = 310, SipParser_URIC_token,
  SipParser_RESERVED_FOR_URIC_token = 313, SipParser_UNRESERVED_token,
  SipParser_PCT_ENCODED_token, SipParser_TEL_URI_CONTEXT_VALUE_token = 318,
  SipParser_DESCRIPTOR_token, SipParser_DOMAINNAME_token,
  SipParser_LABEL_token, SipParser_NEXT_LABEL_token,
  SipParser_ALPHA_token = 326, SipParser_ALPHANUM_OR_HYPHEN_token,
  SipParser_ALPHANUM_token = 331, SipParser_SIP_URL_token = 333,
  SipParser_PRES_URI_token, SipParser_IM_URI_token,
  SipParser_SIP_PREFIX_token, SipParser_URL_token,
  SipParser_PRES_PREFIX_token = 340, SipParser_RV_ABS_PREFIX_token,
  SipParser_IM_PREFIX_token = 344, SipParser_URL_USERINFO_token = 346,
  SipParser_OPTIONAL_URL_PARAMETERS_token, SipParser_OPTIONAL_HEADERS_token,
  SipParser_URL_HOST_token, SipParser_GENERAL_URL_WITHOUT_PARAM_token,
  SipParser_SIP_URL_WITHOUT_PARAMS_token,
  SipParser_TEL_URL_WITHOUT_PARAMS_token,
  SipParser_PRES_URL_WITHOUT_PARAMS_token,
  SipParser_IM_URL_WITHOUT_PARAMS_token,
  SipParser_ABS_URI_WITHOUT_PARAMS_token,
  SipParser_URL_USERINFO_PREFIX_token, SipParser_URL_USERINFO_VAL_token,
  SipParser_HOSTPORT_token, SipParser_USER_INFO_TOKEN_token = 360,
  SipParser_PASSWORD_token, SipParser_TOKEN_USER_INFO_token,
  SipParser_TOKEN_USER_INFO_CHAR_token,
  SipParser_PASSWORD_STRING_token = 365,
  SipParser_PASSWORD_STRING_CHAR_token,
  SipParser_URL_HOST_PREFIX_token = 370, SipParser_TOKEN_HOST_token = 372,
  SipParser_TOKEN_HOST_CHAR_token, SipParser_IPV_4_ADDRESS_token = 377,
  SipParser_IPV_4_PART_token, SipParser_HEXPART_token,
  SipParser_HEXSEQ_token, SipParser_HEX4_token = 383,
  SipParser_URL_PARAMETERS_token = 387, SipParser_URL_PARAMETER_token,
  SipParser_TRANSPORT_PARAM_token, SipParser_USER_PARAM_token,
  SipParser_TTL_PARAM_URL_token, SipParser_MADDR_PARAM_URL_token,
  SipParser_METHOD_PARAM_URL_token, SipParser_LR_PARAM_URL_token,
  SipParser_COMPRESSION_PARAM_URL_token,
  SipParser_TOKENIZED_BY_PARAM_URL_token, SipParser_ORIG_PARAM_URL_token,
  SipParser_USER_PARAM_TYPE_token = 400, SipParser_OTHER_USER_token,
  SipParser_URL_OTHER_PARAM_VAL_token = 413, SipParser_PVALUE_token,
  SipParser_PARAMCHAR_token, SipParser_HEADERS_LIST_token = 419,
  SipParser_HEADER_token, SipParser_HNAME_token = 422,
  SipParser_HVALUE_token, SipParser_REQUEST_LINE_VALUE_token = 430,
  SipParser_REQUEST_METHOD_token, SipParser_PARTY_VALUE_token = 445,
  SipParser_NAME_ADDR_token, SipParser_PARTY_PARAMS_LIST_token,
  SipParser_MORE_THAN_ONE_TOKEN_token = 449,
  SipParser_TAG_PARAM_token = 453, SipParser_PARTY_PARAM_DATA_token = 455,
  SipParser_CONTACT_HEADER_VALUES_token,
  SipParser_CONTACT_HEADER_OTHER_token,
  SipParser_CONTACT_HEADER_VALUE_LIST_token,
  SipParser_CONTACT_HEADER_VALUE_token = 460,
  SipParser_CONTACT_PARAMS_token, SipParser_CONTACT_PARAM_token,
  SipParser_Q_token, SipParser_ACTION_token,
  SipParser_CONTACT_EXPIRES_token, SipParser_Q_VALUE_token = 467,
  SipParser_Q_VALUE_OPTIONAL_token = 469,
  SipParser_ACTION_VALUE_token = 473, SipParser_EXPIRES_VALUE_token = 476,
  SipParser_EXPIRES_VALUE_FOR_CONTACT_token = 478,
  SipParser_DELTA_SECONDS_token, SipParser_SIP_DATE_token,
  SipParser_SIP_DATE_HEADER_VAL_token, SipParser_DATE_token,
  SipParser_TIME_token, SipParser_DAY_token, SipParser_DD_token = 492,
  SipParser_MM_token, SipParser_YY_token, SipParser_HOUR_token = 507,
  SipParser_MINUTE_token, SipParser_SECOND_token, SipParser_TIME_ZONE_token,
  SipParser_AUTHENTICATION_VAL_token = 514, SipParser_CHALLENGE_VALUE_token,
  SipParser_DIGEST_CHALLENGE_LIST_token, SipParser_AUTH_PARAM_LIST_token,
  SipParser_DIGEST_CHALLENGE_token, SipParser_REALM_token,
  SipParser_DOMAIN_token, SipParser_NONCE_token, SipParser_OPAQUE_token,
  SipParser_INTEGRITY_KEY_token, SipParser_CIPHER_KEY_token,
  SipParser_AUTH_PARAM_token, SipParser_STALE_token,
  SipParser_ALGORITHM_token, SipParser_QOP_OPTIONS_token,
  SipParser_PARAM_IN_NEXT_CHALLENGE_token,
  SipParser_REALM_VALUE_token = 531, SipParser_DOMAIN_EQUAL_token = 533,
  SipParser_DOMAIN_WHITE_token, SipParser_DOMAIN_VAL_token = 536,
  SipParser_URI_LIST_token, SipParser_STRING_DOMAIN_PATCH_token,
  SipParser_DOMAIN_SPACE_token, SipParser_NONCE_VALUE_token = 542,
  SipParser_OPAQUE_VALUE_token = 544,
  SipParser_ALGORITHM_DECLARATION_token = 547,
  SipParser_ALGORITHM_AND_AKAV_token, SipParser_AKAV_PREFIX_token = 551,
  SipParser_AKAV_token, SipParser_AKA_VERSION_VAL_token,
  SipParser_ALGORITHM_VAL_token, SipParser_ALGORITHM_PREFIX_token,
  SipParser_QOP_VALUE_LIST_token = 563, SipParser_QOP_VALUE_token,
  SipParser_QUOTED_HEX_VALUE_token = 568,
  SipParser_AUTH_PARAM_DATA_token = 570, SipParser_AUTHORIZATION_VAL_token,
  SipParser_CREDENTIALS_VALUE_token, SipParser_DIGEST_RESPONSE_LIST_token,
  SipParser_DIGEST_RESPONSE_token, SipParser_USERNAME_token,
  SipParser_RESPONSE_token, SipParser_CNONCE_token,
  SipParser_NONCE_COUNT_token, SipParser_AUTS_PARAM_token,
  SipParser_INTEGRITY_PROTECTED_token, SipParser_MESSAGE_QOP_token,
  SipParser_DIGEST_URI_token, SipParser_USERNAME_VALUE_token = 584,
  SipParser_DIGEST_URI_VALUE_token = 586,
  SipParser_CNONCE_VALUE_token = 589, SipParser_NC_VALUE_token = 591,
  SipParser_LHEX_token, SipParser_REQUEST_DIGEST_token = 594,
  SipParser_THIRTY_TWO_LHEX_token,
  SipParser_SIXTY_FOUR_BASE_ENCODING_token = 599,
  SipParser_SIXTY_FOUR_BASE_ENCODING_CHAR_token,
  SipParser_ONE_OR_TWO_EQUAL_SIGNS_token = 602,
  SipParser_ROUTE_LIST_token = 609, SipParser_SINGLE_ROUTE_token,
  SipParser_RETRY_AFTER_VAL_token, SipParser_RETRY_AFTER_COMMENT_token,
  SipParser_RETRY_PARAM_token = 614, SipParser_DURATION_PARAM_token,
  SipParser_RSEQ_VAL_token = 617, SipParser_RESPONSE_NUM_token,
  SipParser_RACK_VAL_token, SipParser_OPTION_TAG_token,
  SipParser_OPTION_TAG_LIST_token, SipParser_SUPPORTED_OPTION_TAG_token,
  SipParser_REFER_TO_HEADER_VALUE_token,
  SipParser_REFERRED_BY_HEADER_VALUE_token = 625,
  SipParser_REFERRER_URI_token, SipParser_REFERRED_BY_PARAMS_token,
  SipParser_REFERRED_BY_PARAM_token = 629,
  SipParser_REFERRED_BY_PARAM_CID_token,
  SipParser_CID_PARAM_STRING_token = 632, SipParser_DOT_ATOM_token,
  SipParser_ATOM_CHAR_token, SipParser_DOT_ATOM_CHAR_token,
  SipParser_EVENT_HEADER_VAL_token = 638, SipParser_EVENT_TYPE_token,
  SipParser_EVENT_PARAMS_token, SipParser_EVENT_PACKAGE_token,
  SipParser_EVENT_TEMPLATE_token, SipParser_EVENT_PARAM_token,
  SipParser_EVENT_ID_token, SipParser_ALLOW_EVENTS_HEADER_VAL_token = 646,
  SipParser_SUBSCRIPTION_STATE_HEADER_VAL_token,
  SipParser_SUBSTATE_VALUE_token, SipParser_SUBSTATE_PARAMS_token,
  SipParser_SUBSTATE_PARAM_token = 653,
  SipParser_SUBSTATE_PARAM_REASON_token,
  SipParser_SUBSTATE_PARAM_RETRY_AFTER_token,
  SipParser_SUBSTATE_PARAM_EXPIRES_token,
  SipParser_SUBSTATE_PARAM_REASON_VAL_token = 659,
  SipParser_DISPOSITION_VAL_token = 666, SipParser_DISPOSITION_TYPE_token,
  SipParser_DISPOSITION_PARAMS_token, SipParser_HANDLING_PARAM_token = 674,
  SipParser_SESSION_EXPIRES_VALUE_token = 678,
  SipParser_REFRESHER_TYPE_token, SipParser_REFRESHER_TYPE_NAME_token,
  SipParser_REFRESHER_TYPE_VALUE_token, SipParser_MIN_SE_VALUE_token = 685,
  SipParser_REPLACES_HEADER_VALUE_token, SipParser_REPLACES_PARAMS_token,
  SipParser_REPLACES_PARAM_token, SipParser_TO_TAG_PARAM_token,
  SipParser_FROM_TAG_PARAM_token, SipParser_EARLY_FLAG_PARAM_token,
  SipParser_AUTHENTICATION_INFO_HEADER_LIST_token = 695,
  SipParser_AUTHENTICATION_INFO_HEADER_PARAM_token,
  SipParser_NEXT_NONCE_token, SipParser_RESPONSE_AUTH_token,
  SipParser_MSG_QOP_token, SipParser_RESPONSE_AUTH_VAL_token = 702,
  SipParser_P_URI_HEADERS_LIST_token = 706,
  SipParser_P_URI_HEADER_VALUE_token, SipParser_P_URI_HEADER_PARAMS_token,
  SipParser_P_VISITED_NETWORK_ID_HEADERS_LIST_token,
  SipParser_P_VISITED_NETWORK_ID_HEADER_VALUE_token,
  SipParser_VNETWORK_SPEC_token,
  SipParser_P_VISITED_NETWORK_ID_PARAMS_token,
  SipParser_P_ACCESS_NETWORK_INFO_HEADER_VALUE_token,
  SipParser_ACCESS_TYPE_token, SipParser_P_ACCESS_NETWORK_INFO_PARAMS_token,
  SipParser_ACCESS_token, SipParser_ACCESS_PARAM_token = 735,
  SipParser_CGI_3GPP_token, SipParser_UTRAN_CELL_ID_3GPP_token,
  SipParser_DSL_LOCATION_token, SipParser_CI_3GPP2_token,
  SipParser_P_CHARGING_FUNCTION_ADDRESSES_HEADER_VALUE_token = 744,
  SipParser_CHARGE_ADDR_PARAM_token, SipParser_CHARGE_ADDR_PARAMS_token,
  SipParser_CHARGE_ADDR_PARAM_CCF_token,
  SipParser_CHARGE_ADDR_PARAM_ECF_token,
  SipParser_GENERAL_VALUE_token = 750,
  SipParser_P_CHARGING_VECTOR_HEADER_VALUE_token = 752,
  SipParser_ICID_VALUE_token, SipParser_CHARGE_PARAMS_token,
  SipParser_CHARGE_PARAM_token = 756,
  SipParser_CHARGE_PARAM_ICID_GEN_ADDR_token,
  SipParser_CHARGE_PARAM_ORIG_IOI_token,
  SipParser_CHARGE_PARAM_TERM_IOI_token,
  SipParser_CHARGE_PARAM_GPRS_CHARGING_INFO_token,
  SipParser_CHARGE_PARAM_XDSL_CHARGING_INFO_token,
  SipParser_PDP_INFO_HIERARCHY_token,
  SipParser_DSL_BEARER_INFO_HIERARCHY_token,
  SipParser_I_WLAN_CHARGING_INFO_token, SipParser_GGSN_token = 768,
  SipParser_AUTH_TOKEN_token, SipParser_BRAS_token,
  SipParser_PDP_INFO_token = 775, SipParser_DSL_BEARER_INFO_token = 777,
  SipParser_SINGLE_PDP_INFO_token, SipParser_SINGLE_DSL_BEARER_INFO_token,
  SipParser_PDP_ITEM_token, SipParser_PDP_SIG_token,
  SipParser_PDP_CID_token, SipParser_FLOW_ID_token,
  SipParser_YES_OR_NO_token = 786, SipParser_DSL_BEARER_ITEM_token = 788,
  SipParser_DSL_BEARER_SIG_token, SipParser_DSL_BEARER_CID_token,
  SipParser_LPAREN_token = 795, SipParser_FLOW_ID_VALUE_token,
  SipParser_FLOW_ID_SYNTAX1_token, SipParser_FLOW_ID_SYNTAX2_token,
  SipParser_DIGITS_token = 802, SipParser_WHITE_IN_WORD_token,
  SipParser_P_MEDIA_AUTHORIZATION_VAL_token = 807,
  SipParser_P_MEDIA_AUTHORIZATION_TOKEN_token,
  SipParser_SECURITY_HEADERS_LIST_token = 810,
  SipParser_SECURITY_HEADER_VALUE_token, SipParser_MECHANISM_NAME_token,
  SipParser_MECHANISM_PARAMS_token, SipParser_MECHANISM_token,
  SipParser_MECHANISM_PARAM_token = 818, SipParser_DIGEST_ALGORITHM_token,
  SipParser_DIGEST_QOP_token, SipParser_DIGEST_VERIFY_token,
  SipParser_SECURITY_ALGORITHM_token, SipParser_SECURITY_PROTOCOL_token,
  SipParser_SECURITY_MODE_token, SipParser_SECURITY_ENCRYPT_ALGORITHM_token,
  SipParser_SECURITY_SPIC_token, SipParser_SECURITY_SPIS_token,
  SipParser_SECURITY_PORTC_token, SipParser_SECURITY_PORTS_token,
  SipParser_DIGEST_ALGORITHM_DECLARATION_token,
  SipParser_DIGEST_VERIFY_VALUE_token = 834,
  SipParser_SECURITY_ALGORITHM_VAL_token = 836,
  SipParser_SECURITY_PROTOCOL_VAL_token = 840,
  SipParser_SECURITY_MODE_VAL_token = 844,
  SipParser_SECURITY_ENCRYPT_ALGORITHM_VAL_token = 848,
  SipParser_TEN_DIGIT_token = 853
} SipParser_token_type;

typedef struct SipParser_pcb_struct{
  SipParser_token_type token_number, reduction_token, error_frame_token;
  int input_code;
  int input_value;
  int line, column;
  int ssx, sn, error_frame_ssx;
  int drt, dssx, dsn;
  int ss[46];
  SipParser_vs_type vs[46];
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












		ParserPUriHeader				puriHeader; 
		ParserPVisitedNetworkIDHeader	pvisitedNetworkIDHeader; 
		ParserPAccessNetworkInfoHeader	paccessNetworkInfoHeader; 
		ParserPChargingFunctionAddressesHeader	pchargingFunctionAddressesHeader; 
		ParserPChargingVectorHeader		pchargingVectorHeader; 
		ParserPMediaAuthorizationHeader	pmediaAuthorizationHeader; 
		ParserSecurityHeader			securityHeader; 









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

#elif defined(RV_SIP_AUTH_ON) && defined(RV_SIP_TEL_URI_SUPPORT) && defined(RV_SIP_IMS_HEADER_SUPPORT) && defined(RV_SIP_IMS_DCS_HEADER_SUPPORT) && !defined(RV_SIP_EXTENDED_HEADER_SUPPORT) && !defined(RV_SIP_JSR32_SUPPORT) && !defined(RV_SIP_LIGHT) && !defined(RV_SIP_PRIMITIVES) && !defined(RV_SIP_SUBS_ON)
#ifndef PARSERENGINEIMS_H_1165245759
#define PARSERENGINEIMS_H_1165245759

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
  char ag_vt_63[sizeof(ParserDisposition)];
  char ag_vt_64[sizeof(ParserDispositionParams)];
  char ag_vt_65[sizeof(ParserDispositionParam)];
  char ag_vt_66[sizeof(ParserSessionExpiresHeader)];
  char ag_vt_67[sizeof(RvSipSessionExpiresRefresherType)];
  char ag_vt_68[sizeof(ParserMinSEHeader)];
  char ag_vt_69[sizeof(ParserReplacesEarlyFlagParamType)];
  char ag_vt_70[sizeof(ParserAccess)];
  char ag_vt_71[sizeof(ParserMechanism)];
  char ag_vt_72[sizeof(ParserSecurityAlgorithmType)];
  char ag_vt_73[sizeof(ParserSecurityProtocolType)];
  char ag_vt_74[sizeof(ParserSecurityModeType)];
  char ag_vt_75[sizeof(ParserSecurityEncryptAlgorithmType)];
  char ag_vt_76[sizeof(ParserOSPSTag)];
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
  SipParser_TEL_URI_HEADER_token, SipParser_P_URI_HEADER_token = 59,
  SipParser_P_VISITED_NETWORK_ID_HEADER_token = 61,
  SipParser_P_ACCESS_NETWORK_INFO_HEADER_token = 63,
  SipParser_P_CHARGING_FUNCTION_ADDRESSES_HEADER_token = 65,
  SipParser_P_CHARGING_VECTOR_HEADER_token = 67,
  SipParser_P_MEDIA_AUTHORIZATION_HEADER_token = 69,
  SipParser_SECURITY_HEADER_token = 71,
  SipParser_P_DCS_TRACE_PARTY_ID_HEADER_token = 73,
  SipParser_P_DCS_OSPS_HEADER_token = 75,
  SipParser_P_DCS_BILLING_INFO_HEADER_token = 77,
  SipParser_P_DCS_LAES_HEADER_token = 79,
  SipParser_P_DCS_REDIRECT_HEADER_token = 81, SipParser_OTHER_HEADER_token,
  SipParser_WHITE_token = 84, SipParser_SP_token = 86,
  SipParser_COMMA_token = 88, SipParser_COMMA_BETWEEN_HEADERS_token = 90,
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
  SipParser_FIELD_VALUE_token = 121, SipParser_QUOTED_STRING_token = 127,
  SipParser_QUOTED_CHAR_token, SipParser_HEX_VALUE_token = 135,
  SipParser_HEX_token, SipParser_INT_NUMBER_token = 138,
  SipParser_DIGIT_token, SipParser_TOKEN_OR_QUOTED_STRING_token = 141,
  SipParser_METHOD_token, SipParser_CSEQ_VAL_token = 149,
  SipParser_SEQUNCE_NUMBER_token, SipParser_CONTENT_LENGTH_VAL_token,
  SipParser_CALL_ID_token, SipParser_MEDIA_TYPE_token,
  SipParser_MEDIA_TYPE_TYPE_token, SipParser_MEDIA_TYPE_SUB_TYPE_token,
  SipParser_OPTIONAL_CONTENT_TYPE_PARAM_LIST_token,
  SipParser_CONTENT_TYPE_PARAM_LIST_token, SipParser_BOUNDARY_PARAM_token,
  SipParser_VERSION_PARAM_token, SipParser_BASE_PARAM_token,
  SipParser_CONTENT_TYPE_GENERIC_PARAM_DATA_token,
  SipParser_CONTENT_TYPE_GENERIC_PARAM_DATA_VAL_token = 181,
  SipParser_GENERIC_PARAM_LIST_token, SipParser_GENERIC_PARAM_DATA_token,
  SipParser_GENERIC_PARAM_DATA_VAL_token, SipParser_SIP_VERSION_token,
  SipParser_STATUS_CODE_token = 187, SipParser_REASON_PHRASE_token,
  SipParser_EXTENSION_CODE_token = 190, SipParser_REASONE_TEXT_UTF8_token,
  SipParser_ALLOW_VAL_token = 195, SipParser_VIA_LIST_token,
  SipParser_SINGLE_VIA_token, SipParser_SENT_PROTOCOL_token,
  SipParser_SENT_BY_token, SipParser_VIA_PARAM_LIST_token,
  SipParser_VIA_PARAM_token, SipParser_PROTOCOL_NAME_token,
  SipParser_PROTOCOL_VERSION_token, SipParser_TRANSPORT_token,
  SipParser_TRANSPORT_TYPE_token = 206, SipParser_PORT_token = 211,
  SipParser_HOST_NAME_token, SipParser_VIA_HIDDEN_token,
  SipParser_VIA_ALIAS_token, SipParser_VIA_TTL_token,
  SipParser_VIA_MADDR_token, SipParser_VIA_RECEIVED_token,
  SipParser_VIA_BRANCH_token, SipParser_RESPONSE_PORT_token,
  SipParser_VIA_COMPRESSION_token, SipParser_OTHER_PARAM_token,
  SipParser_OTHER_PARAM_EXPR_token, SipParser_IPV_6_REFERENCE_token,
  SipParser_TTL_PARAM_token = 226, SipParser_TTL_token = 228,
  SipParser_RPORT_NAME_token, SipParser_RPORT_VAL_token,
  SipParser_MADDR_PARAM_token = 232,
  SipParser_HOST_NAME_FOR_RECEIVED_token = 235,
  SipParser_COMPRESSION_TYPE_token = 238,
  SipParser_IPV_6_ADDRESS_token = 240, SipParser_TEST_COMMENT_token,
  SipParser_COMMENT_TEXT_token, SipParser_END_COMMENT_token = 244,
  SipParser_REQUEST_URI_token = 247, SipParser_ADDR_SPEC_token,
  SipParser_ABSOLUTE_URI_token, SipParser_ABS_URI_SCHEME_token,
  SipParser_URI_IDENTIFIER_token, SipParser_ABS_URI_SCHEME_CHAR_token,
  SipParser_URI_SCHEME_OR_DISPLAY_NAME_token = 254,
  SipParser_DISPLAY_NAME_token, SipParser_URI_IDENTIFIER_CHAR_token,
  SipParser_URI_IDENTIFIER_WITHOUT_SEPARATORS_token = 258,
  SipParser_TEL_URI_token = 261, SipParser_TEL_PREFIX_token,
  SipParser_TEL_token, SipParser_GLOBAL_OR_LOCAL_NUMBER_token = 265,
  SipParser_OPTIONAL_TEL_URI_PARAMETERS_token,
  SipParser_GLOBAL_NUMBER_token, SipParser_LOCAL_NUMBER_token,
  SipParser_GLOBAL_NUMBER_DIGITS_token, SipParser_PHONEDIGIT_token = 271,
  SipParser_VISUAL_SEPERATOR_token = 275,
  SipParser_LOCAL_NUMBER_DIGITS_token = 277,
  SipParser_HEX_OR_PHONECHAR_token, SipParser_PHONEDIGIT_HEX_token,
  SipParser_PHONECHAR_token = 283, SipParser_TEL_URI_PARAMETERS_token,
  SipParser_TEL_URI_PARAMETER_token,
  SipParser_TEL_URI_EXTENSION_PARAM_token,
  SipParser_TEL_URI_POST_DIAL_PARAM_token,
  SipParser_TEL_URI_ISDN_SUB_ADDR_PARAM_URL_token,
  SipParser_TEL_URI_CONTEXT_PARAM_token,
  SipParser_TEL_URI_ENUMDI_PARAM_token,
  SipParser_URL_GENERIC_PARAM_DATA_token,
  SipParser_TEL_URI_EXTENTION_VALUE_token = 293,
  SipParser_TEL_URI_POST_DIAL_VALUE_token = 296,
  SipParser_TEL_URI_POST_DIAL_CHAR_token, SipParser_DTMFDIGIT_token = 299,
  SipParser_PAUSE_CHAR_token,
  SipParser_TEL_URI_ISDN_SUB_ADDR_VALUE_token = 302, SipParser_URIC_token,
  SipParser_RESERVED_FOR_URIC_token = 305, SipParser_UNRESERVED_token,
  SipParser_PCT_ENCODED_token, SipParser_TEL_URI_CONTEXT_VALUE_token = 310,
  SipParser_DESCRIPTOR_token, SipParser_DOMAINNAME_token,
  SipParser_LABEL_token, SipParser_NEXT_LABEL_token,
  SipParser_ALPHA_token = 318, SipParser_ALPHANUM_OR_HYPHEN_token,
  SipParser_ALPHANUM_token = 324, SipParser_SIP_URL_token = 326,
  SipParser_PRES_URI_token, SipParser_IM_URI_token,
  SipParser_SIP_PREFIX_token, SipParser_URL_token,
  SipParser_PRES_PREFIX_token = 333, SipParser_RV_ABS_PREFIX_token,
  SipParser_IM_PREFIX_token = 337, SipParser_URL_USERINFO_token = 339,
  SipParser_OPTIONAL_URL_PARAMETERS_token, SipParser_OPTIONAL_HEADERS_token,
  SipParser_URL_HOST_token, SipParser_GENERAL_URL_WITHOUT_PARAM_token,
  SipParser_SIP_URL_WITHOUT_PARAMS_token,
  SipParser_TEL_URL_WITHOUT_PARAMS_token,
  SipParser_PRES_URL_WITHOUT_PARAMS_token,
  SipParser_IM_URL_WITHOUT_PARAMS_token,
  SipParser_ABS_URI_WITHOUT_PARAMS_token,
  SipParser_URL_USERINFO_PREFIX_token, SipParser_URL_USERINFO_VAL_token,
  SipParser_HOSTPORT_token, SipParser_USER_INFO_TOKEN_token = 353,
  SipParser_PASSWORD_token, SipParser_TOKEN_USER_INFO_token,
  SipParser_TOKEN_USER_INFO_CHAR_token,
  SipParser_PASSWORD_STRING_token = 358,
  SipParser_PASSWORD_STRING_CHAR_token,
  SipParser_URL_HOST_PREFIX_token = 363, SipParser_TOKEN_HOST_token = 365,
  SipParser_TOKEN_HOST_CHAR_token, SipParser_IPV_4_ADDRESS_token = 370,
  SipParser_IPV_4_PART_token, SipParser_HEXPART_token,
  SipParser_HEXSEQ_token, SipParser_HEX4_token = 376,
  SipParser_URL_PARAMETERS_token = 380, SipParser_URL_PARAMETER_token,
  SipParser_TRANSPORT_PARAM_token, SipParser_USER_PARAM_token,
  SipParser_TTL_PARAM_URL_token, SipParser_MADDR_PARAM_URL_token,
  SipParser_METHOD_PARAM_URL_token, SipParser_LR_PARAM_URL_token,
  SipParser_COMPRESSION_PARAM_URL_token,
  SipParser_TOKENIZED_BY_PARAM_URL_token, SipParser_ORIG_PARAM_URL_token,
  SipParser_USER_PARAM_TYPE_token = 393, SipParser_OTHER_USER_token,
  SipParser_URL_OTHER_PARAM_VAL_token = 406, SipParser_PVALUE_token,
  SipParser_PARAMCHAR_token, SipParser_HEADERS_LIST_token = 412,
  SipParser_HEADER_token, SipParser_HNAME_token = 415,
  SipParser_HVALUE_token, SipParser_REQUEST_LINE_VALUE_token = 423,
  SipParser_REQUEST_METHOD_token, SipParser_PARTY_VALUE_token = 438,
  SipParser_NAME_ADDR_token, SipParser_PARTY_PARAMS_LIST_token,
  SipParser_MORE_THAN_ONE_TOKEN_token = 442,
  SipParser_TAG_PARAM_token = 446, SipParser_PARTY_PARAM_DATA_token = 448,
  SipParser_CONTACT_HEADER_VALUES_token,
  SipParser_CONTACT_HEADER_OTHER_token,
  SipParser_CONTACT_HEADER_VALUE_LIST_token,
  SipParser_CONTACT_HEADER_VALUE_token = 453,
  SipParser_CONTACT_PARAMS_token, SipParser_CONTACT_PARAM_token,
  SipParser_Q_token, SipParser_ACTION_token,
  SipParser_CONTACT_EXPIRES_token, SipParser_Q_VALUE_token = 460,
  SipParser_Q_VALUE_OPTIONAL_token = 462,
  SipParser_ACTION_VALUE_token = 466, SipParser_EXPIRES_VALUE_token = 469,
  SipParser_EXPIRES_VALUE_FOR_CONTACT_token = 471,
  SipParser_DELTA_SECONDS_token, SipParser_SIP_DATE_token,
  SipParser_SIP_DATE_HEADER_VAL_token, SipParser_DATE_token,
  SipParser_TIME_token, SipParser_DAY_token, SipParser_DD_token = 485,
  SipParser_MM_token, SipParser_YY_token, SipParser_HOUR_token = 500,
  SipParser_MINUTE_token, SipParser_SECOND_token, SipParser_TIME_ZONE_token,
  SipParser_AUTHENTICATION_VAL_token = 507, SipParser_CHALLENGE_VALUE_token,
  SipParser_DIGEST_CHALLENGE_LIST_token, SipParser_AUTH_PARAM_LIST_token,
  SipParser_DIGEST_CHALLENGE_token, SipParser_REALM_token,
  SipParser_DOMAIN_token, SipParser_NONCE_token, SipParser_OPAQUE_token,
  SipParser_INTEGRITY_KEY_token, SipParser_CIPHER_KEY_token,
  SipParser_AUTH_PARAM_token, SipParser_STALE_token,
  SipParser_ALGORITHM_token, SipParser_QOP_OPTIONS_token,
  SipParser_PARAM_IN_NEXT_CHALLENGE_token,
  SipParser_REALM_VALUE_token = 524, SipParser_DOMAIN_EQUAL_token = 526,
  SipParser_DOMAIN_WHITE_token, SipParser_DOMAIN_VAL_token = 529,
  SipParser_URI_LIST_token, SipParser_STRING_DOMAIN_PATCH_token,
  SipParser_DOMAIN_SPACE_token, SipParser_NONCE_VALUE_token = 535,
  SipParser_OPAQUE_VALUE_token = 537,
  SipParser_ALGORITHM_DECLARATION_token = 540,
  SipParser_ALGORITHM_AND_AKAV_token, SipParser_AKAV_PREFIX_token = 544,
  SipParser_AKAV_token, SipParser_AKA_VERSION_VAL_token,
  SipParser_ALGORITHM_VAL_token, SipParser_ALGORITHM_PREFIX_token,
  SipParser_QOP_VALUE_LIST_token = 556, SipParser_QOP_VALUE_token,
  SipParser_QUOTED_HEX_VALUE_token = 561,
  SipParser_AUTH_PARAM_DATA_token = 563, SipParser_AUTHORIZATION_VAL_token,
  SipParser_CREDENTIALS_VALUE_token, SipParser_DIGEST_RESPONSE_LIST_token,
  SipParser_DIGEST_RESPONSE_token, SipParser_USERNAME_token,
  SipParser_RESPONSE_token, SipParser_CNONCE_token,
  SipParser_NONCE_COUNT_token, SipParser_AUTS_PARAM_token,
  SipParser_INTEGRITY_PROTECTED_token, SipParser_MESSAGE_QOP_token,
  SipParser_DIGEST_URI_token, SipParser_USERNAME_VALUE_token = 577,
  SipParser_DIGEST_URI_VALUE_token = 579,
  SipParser_CNONCE_VALUE_token = 582, SipParser_NC_VALUE_token = 584,
  SipParser_LHEX_token, SipParser_REQUEST_DIGEST_token = 587,
  SipParser_THIRTY_TWO_LHEX_token,
  SipParser_SIXTY_FOUR_BASE_ENCODING_token = 592,
  SipParser_SIXTY_FOUR_BASE_ENCODING_CHAR_token,
  SipParser_ONE_OR_TWO_EQUAL_SIGNS_token = 595,
  SipParser_ROUTE_LIST_token = 602, SipParser_SINGLE_ROUTE_token,
  SipParser_RETRY_AFTER_VAL_token, SipParser_RETRY_AFTER_COMMENT_token,
  SipParser_RETRY_PARAM_token = 607, SipParser_DURATION_PARAM_token,
  SipParser_RSEQ_VAL_token = 610, SipParser_RESPONSE_NUM_token,
  SipParser_RACK_VAL_token, SipParser_OPTION_TAG_token,
  SipParser_OPTION_TAG_LIST_token, SipParser_SUPPORTED_OPTION_TAG_token,
  SipParser_DISPOSITION_VAL_token, SipParser_DISPOSITION_TYPE_token,
  SipParser_DISPOSITION_PARAMS_token, SipParser_HANDLING_PARAM_token = 624,
  SipParser_SESSION_EXPIRES_VALUE_token = 628,
  SipParser_REFRESHER_TYPE_token, SipParser_REFRESHER_TYPE_NAME_token = 631,
  SipParser_REFRESHER_TYPE_VALUE_token, SipParser_MIN_SE_VALUE_token = 636,
  SipParser_REPLACES_HEADER_VALUE_token, SipParser_REPLACES_PARAMS_token,
  SipParser_REPLACES_PARAM_token, SipParser_TO_TAG_PARAM_token,
  SipParser_FROM_TAG_PARAM_token, SipParser_EARLY_FLAG_PARAM_token,
  SipParser_AUTHENTICATION_INFO_HEADER_LIST_token = 646,
  SipParser_AUTHENTICATION_INFO_HEADER_PARAM_token,
  SipParser_NEXT_NONCE_token, SipParser_RESPONSE_AUTH_token,
  SipParser_MSG_QOP_token, SipParser_RESPONSE_AUTH_VAL_token = 653,
  SipParser_P_URI_HEADERS_LIST_token = 657,
  SipParser_P_URI_HEADER_VALUE_token, SipParser_P_URI_HEADER_PARAMS_token,
  SipParser_P_VISITED_NETWORK_ID_HEADERS_LIST_token,
  SipParser_P_VISITED_NETWORK_ID_HEADER_VALUE_token,
  SipParser_VNETWORK_SPEC_token,
  SipParser_P_VISITED_NETWORK_ID_PARAMS_token,
  SipParser_P_ACCESS_NETWORK_INFO_HEADER_VALUE_token,
  SipParser_ACCESS_TYPE_token, SipParser_P_ACCESS_NETWORK_INFO_PARAMS_token,
  SipParser_ACCESS_token, SipParser_ACCESS_PARAM_token = 686,
  SipParser_CGI_3GPP_token, SipParser_UTRAN_CELL_ID_3GPP_token,
  SipParser_DSL_LOCATION_token, SipParser_CI_3GPP2_token,
  SipParser_P_CHARGING_FUNCTION_ADDRESSES_HEADER_VALUE_token = 695,
  SipParser_CHARGE_ADDR_PARAM_token, SipParser_CHARGE_ADDR_PARAMS_token,
  SipParser_CHARGE_ADDR_PARAM_CCF_token,
  SipParser_CHARGE_ADDR_PARAM_ECF_token,
  SipParser_GENERAL_VALUE_token = 701,
  SipParser_P_CHARGING_VECTOR_HEADER_VALUE_token = 703,
  SipParser_ICID_VALUE_token, SipParser_CHARGE_PARAMS_token,
  SipParser_CHARGE_PARAM_token = 707,
  SipParser_CHARGE_PARAM_ICID_GEN_ADDR_token,
  SipParser_CHARGE_PARAM_ORIG_IOI_token,
  SipParser_CHARGE_PARAM_TERM_IOI_token,
  SipParser_CHARGE_PARAM_GPRS_CHARGING_INFO_token,
  SipParser_CHARGE_PARAM_XDSL_CHARGING_INFO_token,
  SipParser_PDP_INFO_HIERARCHY_token,
  SipParser_DSL_BEARER_INFO_HIERARCHY_token,
  SipParser_I_WLAN_CHARGING_INFO_token, SipParser_GGSN_token = 719,
  SipParser_AUTH_TOKEN_token, SipParser_BRAS_token,
  SipParser_PDP_INFO_token = 726, SipParser_DSL_BEARER_INFO_token = 728,
  SipParser_SINGLE_PDP_INFO_token, SipParser_SINGLE_DSL_BEARER_INFO_token,
  SipParser_PDP_ITEM_token, SipParser_PDP_SIG_token,
  SipParser_PDP_CID_token, SipParser_FLOW_ID_token,
  SipParser_YES_OR_NO_token = 737, SipParser_DSL_BEARER_ITEM_token = 739,
  SipParser_DSL_BEARER_SIG_token, SipParser_DSL_BEARER_CID_token,
  SipParser_LPAREN_token = 746, SipParser_FLOW_ID_VALUE_token,
  SipParser_FLOW_ID_SYNTAX1_token, SipParser_FLOW_ID_SYNTAX2_token,
  SipParser_DIGITS_token = 753, SipParser_WHITE_IN_WORD_token,
  SipParser_P_MEDIA_AUTHORIZATION_VAL_token = 758,
  SipParser_P_MEDIA_AUTHORIZATION_TOKEN_token,
  SipParser_SECURITY_HEADERS_LIST_token = 761,
  SipParser_SECURITY_HEADER_VALUE_token, SipParser_MECHANISM_NAME_token,
  SipParser_MECHANISM_PARAMS_token, SipParser_MECHANISM_token,
  SipParser_MECHANISM_PARAM_token = 769, SipParser_DIGEST_ALGORITHM_token,
  SipParser_DIGEST_QOP_token, SipParser_DIGEST_VERIFY_token,
  SipParser_SECURITY_ALGORITHM_token, SipParser_SECURITY_PROTOCOL_token,
  SipParser_SECURITY_MODE_token, SipParser_SECURITY_ENCRYPT_ALGORITHM_token,
  SipParser_SECURITY_SPIC_token, SipParser_SECURITY_SPIS_token,
  SipParser_SECURITY_PORTC_token, SipParser_SECURITY_PORTS_token,
  SipParser_DIGEST_ALGORITHM_DECLARATION_token,
  SipParser_DIGEST_VERIFY_VALUE_token = 785,
  SipParser_SECURITY_ALGORITHM_VAL_token = 787,
  SipParser_SECURITY_PROTOCOL_VAL_token = 791,
  SipParser_SECURITY_MODE_VAL_token = 795,
  SipParser_SECURITY_ENCRYPT_ALGORITHM_VAL_token = 799,
  SipParser_TEN_DIGIT_token = 804,
  SipParser_P_DCS_TRACE_PARTY_ID_HEADER_VALUE_token = 808,
  SipParser_P_DCS_OSPS_HEADER_VALUE_token, SipParser_OSPS_TAG_token,
  SipParser_P_DCS_BILLING_INFO_HEADER_VALUE_token = 814,
  SipParser_BILLING_CORRELATION_ID_token, SipParser_FEID_token,
  SipParser_FEID_HOST_token, SipParser_BILLING_INFO_PARAMS_token,
  SipParser_BILLING_CORRELATION_ID_VALUE_token,
  SipParser_FORTY_EIGHT_HEXDIG_token, SipParser_FEID_VALUE_token,
  SipParser_SIXTEEN_HEXDIG_token, SipParser_BILLING_INFO_PARAM_token,
  SipParser_P_DCS_LAES_HEADER_VALUE_token = 830, SipParser_LAES_SIG_token,
  SipParser_LAES_PARAMS_token, SipParser_LAES_PARAM_token,
  SipParser_LAES_CONTENT_token = 835,
  SipParser_P_DCS_REDIRECT_HEADER_VALUE_token = 837,
  SipParser_CALLED_ID_URI_token, SipParser_REDIRECT_PARAMS_token,
  SipParser_REDIRECT_PARAM_token
} SipParser_token_type;

typedef struct SipParser_pcb_struct{
  SipParser_token_type token_number, reduction_token, error_frame_token;
  int input_code;
  int input_value;
  int line, column;
  int ssx, sn, error_frame_ssx;
  int drt, dssx, dsn;
  int ss[49];
  SipParser_vs_type vs[49];
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










        ParserRetryAfter           retryAfter;  
        ParserContentDisposition   contentDisposition; 
        ParserRack                 rack;        
        ParserReplacesParams       replacesParams; 
        ParserReplacesParam        replacesParam;  

        ParserSentByInVia          viaSentBy;       












		ParserPUriHeader				puriHeader; 
		ParserPVisitedNetworkIDHeader	pvisitedNetworkIDHeader; 
		ParserPAccessNetworkInfoHeader	paccessNetworkInfoHeader; 
		ParserPChargingFunctionAddressesHeader	pchargingFunctionAddressesHeader; 
		ParserPChargingVectorHeader		pchargingVectorHeader; 
		ParserPMediaAuthorizationHeader	pmediaAuthorizationHeader; 
		ParserSecurityHeader			securityHeader; 


		ParserPDCSTracePartyIDHeader	pdcsTracePartyIDHeader; 
		ParserPDCSOSPSHeader			pdcsOSPSHeader; 
		ParserPDCSBillingInfoHeader		pdcsBillingInfoHeader; 
		ParserPDCSLAESHeader			pdcsLAESHeader; 
		ParserPDCSRedirectHeader		pdcsRedirectHeader; 


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

#elif defined(RV_SIP_AUTH_ON) && defined(RV_SIP_TEL_URI_SUPPORT) && defined(RV_SIP_IMS_HEADER_SUPPORT) && defined(RV_SIP_IMS_DCS_HEADER_SUPPORT) && !defined(RV_SIP_EXTENDED_HEADER_SUPPORT) && !defined(RV_SIP_JSR32_SUPPORT) && !defined(RV_SIP_LIGHT) && !defined(RV_SIP_PRIMITIVES) && defined(RV_SIP_SUBS_ON)
#ifndef PARSERENGINEIMS_H_1165245765
#define PARSERENGINEIMS_H_1165245765

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
  char ag_vt_73[sizeof(ParserAccess)];
  char ag_vt_74[sizeof(ParserMechanism)];
  char ag_vt_75[sizeof(ParserSecurityAlgorithmType)];
  char ag_vt_76[sizeof(ParserSecurityProtocolType)];
  char ag_vt_77[sizeof(ParserSecurityModeType)];
  char ag_vt_78[sizeof(ParserSecurityEncryptAlgorithmType)];
  char ag_vt_79[sizeof(ParserOSPSTag)];
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
  SipParser_TEL_URI_HEADER_token, SipParser_P_URI_HEADER_token = 69,
  SipParser_P_VISITED_NETWORK_ID_HEADER_token = 71,
  SipParser_P_ACCESS_NETWORK_INFO_HEADER_token = 73,
  SipParser_P_CHARGING_FUNCTION_ADDRESSES_HEADER_token = 75,
  SipParser_P_CHARGING_VECTOR_HEADER_token = 77,
  SipParser_P_MEDIA_AUTHORIZATION_HEADER_token = 79,
  SipParser_SECURITY_HEADER_token = 81,
  SipParser_P_DCS_TRACE_PARTY_ID_HEADER_token = 83,
  SipParser_P_DCS_OSPS_HEADER_token = 85,
  SipParser_P_DCS_BILLING_INFO_HEADER_token = 87,
  SipParser_P_DCS_LAES_HEADER_token = 89,
  SipParser_P_DCS_REDIRECT_HEADER_token = 91, SipParser_OTHER_HEADER_token,
  SipParser_WHITE_token = 94, SipParser_SP_token = 96,
  SipParser_COMMA_token = 98, SipParser_COMMA_BETWEEN_HEADERS_token = 100,
  SipParser_DOT_token, SipParser_EQUAL_token = 105,
  SipParser_AT_token = 107, SipParser_COLON_token = 109,
  SipParser_SEMI_COLON_token, SipParser_MARK_SIGN_token = 112,
  SipParser_SLASH_token = 114, SipParser_LEFT_PARENTHESIS_token = 116,
  SipParser_LEFT_ANGLE_BRACKET_token = 118,
  SipParser_LEFT_QUOTATION_token = 120, SipParser_HYPHEN_token = 122,
  SipParser_TOKEN_token = 124, SipParser_TOKEN_CHAR_token,
  SipParser_CALL_ID_WORD_token = 127, SipParser_CALL_ID_WORD_CHAR_token,
  SipParser_TOKEN_FOR_CONTENT_TYPE_token = 130,
  SipParser_TOKEN_CHAR_FOR_CONTENT_TYPE_token,
  SipParser_TOKEN_NO_DOT_token = 133, SipParser_TOKEN_CHAR_NO_DOT_token,
  SipParser_FIELD_VALUE_token = 136, SipParser_QUOTED_STRING_token = 142,
  SipParser_QUOTED_CHAR_token, SipParser_HEX_VALUE_token = 150,
  SipParser_HEX_token, SipParser_INT_NUMBER_token = 153,
  SipParser_DIGIT_token, SipParser_TOKEN_OR_QUOTED_STRING_token = 156,
  SipParser_METHOD_token, SipParser_CSEQ_VAL_token = 167,
  SipParser_SEQUNCE_NUMBER_token, SipParser_CONTENT_LENGTH_VAL_token,
  SipParser_CALL_ID_token, SipParser_MEDIA_TYPE_token,
  SipParser_MEDIA_TYPE_TYPE_token, SipParser_MEDIA_TYPE_SUB_TYPE_token,
  SipParser_OPTIONAL_CONTENT_TYPE_PARAM_LIST_token,
  SipParser_CONTENT_TYPE_PARAM_LIST_token, SipParser_BOUNDARY_PARAM_token,
  SipParser_VERSION_PARAM_token, SipParser_BASE_PARAM_token,
  SipParser_CONTENT_TYPE_GENERIC_PARAM_DATA_token,
  SipParser_CONTENT_TYPE_GENERIC_PARAM_DATA_VAL_token = 199,
  SipParser_GENERIC_PARAM_LIST_token, SipParser_GENERIC_PARAM_DATA_token,
  SipParser_GENERIC_PARAM_DATA_VAL_token, SipParser_SIP_VERSION_token,
  SipParser_STATUS_CODE_token = 205, SipParser_REASON_PHRASE_token,
  SipParser_EXTENSION_CODE_token = 208, SipParser_REASONE_TEXT_UTF8_token,
  SipParser_ALLOW_VAL_token = 213, SipParser_VIA_LIST_token,
  SipParser_SINGLE_VIA_token, SipParser_SENT_PROTOCOL_token,
  SipParser_SENT_BY_token, SipParser_VIA_PARAM_LIST_token,
  SipParser_VIA_PARAM_token, SipParser_PROTOCOL_NAME_token,
  SipParser_PROTOCOL_VERSION_token, SipParser_TRANSPORT_token,
  SipParser_TRANSPORT_TYPE_token = 224, SipParser_PORT_token = 229,
  SipParser_HOST_NAME_token, SipParser_VIA_HIDDEN_token,
  SipParser_VIA_ALIAS_token, SipParser_VIA_TTL_token,
  SipParser_VIA_MADDR_token, SipParser_VIA_RECEIVED_token,
  SipParser_VIA_BRANCH_token, SipParser_RESPONSE_PORT_token,
  SipParser_VIA_COMPRESSION_token, SipParser_OTHER_PARAM_token,
  SipParser_OTHER_PARAM_EXPR_token, SipParser_IPV_6_REFERENCE_token,
  SipParser_TTL_PARAM_token = 244, SipParser_TTL_token = 246,
  SipParser_RPORT_NAME_token, SipParser_RPORT_VAL_token,
  SipParser_MADDR_PARAM_token = 250,
  SipParser_HOST_NAME_FOR_RECEIVED_token = 253,
  SipParser_COMPRESSION_TYPE_token = 256,
  SipParser_IPV_6_ADDRESS_token = 258, SipParser_TEST_COMMENT_token,
  SipParser_COMMENT_TEXT_token, SipParser_END_COMMENT_token = 262,
  SipParser_REQUEST_URI_token = 265, SipParser_ADDR_SPEC_token,
  SipParser_ABSOLUTE_URI_token, SipParser_ABS_URI_SCHEME_token,
  SipParser_URI_IDENTIFIER_token, SipParser_ABS_URI_SCHEME_CHAR_token,
  SipParser_URI_SCHEME_OR_DISPLAY_NAME_token = 272,
  SipParser_DISPLAY_NAME_token, SipParser_URI_IDENTIFIER_CHAR_token,
  SipParser_URI_IDENTIFIER_WITHOUT_SEPARATORS_token = 276,
  SipParser_TEL_URI_token = 279, SipParser_TEL_PREFIX_token,
  SipParser_TEL_token, SipParser_GLOBAL_OR_LOCAL_NUMBER_token = 283,
  SipParser_OPTIONAL_TEL_URI_PARAMETERS_token,
  SipParser_GLOBAL_NUMBER_token, SipParser_LOCAL_NUMBER_token,
  SipParser_GLOBAL_NUMBER_DIGITS_token, SipParser_PHONEDIGIT_token = 289,
  SipParser_VISUAL_SEPERATOR_token = 293,
  SipParser_LOCAL_NUMBER_DIGITS_token = 295,
  SipParser_HEX_OR_PHONECHAR_token, SipParser_PHONEDIGIT_HEX_token,
  SipParser_PHONECHAR_token = 301, SipParser_TEL_URI_PARAMETERS_token,
  SipParser_TEL_URI_PARAMETER_token,
  SipParser_TEL_URI_EXTENSION_PARAM_token,
  SipParser_TEL_URI_POST_DIAL_PARAM_token,
  SipParser_TEL_URI_ISDN_SUB_ADDR_PARAM_URL_token,
  SipParser_TEL_URI_CONTEXT_PARAM_token,
  SipParser_TEL_URI_ENUMDI_PARAM_token,
  SipParser_URL_GENERIC_PARAM_DATA_token,
  SipParser_TEL_URI_EXTENTION_VALUE_token = 311,
  SipParser_TEL_URI_POST_DIAL_VALUE_token = 314,
  SipParser_TEL_URI_POST_DIAL_CHAR_token, SipParser_DTMFDIGIT_token = 317,
  SipParser_PAUSE_CHAR_token,
  SipParser_TEL_URI_ISDN_SUB_ADDR_VALUE_token = 320, SipParser_URIC_token,
  SipParser_RESERVED_FOR_URIC_token = 323, SipParser_UNRESERVED_token,
  SipParser_PCT_ENCODED_token, SipParser_TEL_URI_CONTEXT_VALUE_token = 328,
  SipParser_DESCRIPTOR_token, SipParser_DOMAINNAME_token,
  SipParser_LABEL_token, SipParser_NEXT_LABEL_token,
  SipParser_ALPHA_token = 336, SipParser_ALPHANUM_OR_HYPHEN_token,
  SipParser_ALPHANUM_token = 341, SipParser_SIP_URL_token = 343,
  SipParser_PRES_URI_token, SipParser_IM_URI_token,
  SipParser_SIP_PREFIX_token, SipParser_URL_token,
  SipParser_PRES_PREFIX_token = 350, SipParser_RV_ABS_PREFIX_token,
  SipParser_IM_PREFIX_token = 354, SipParser_URL_USERINFO_token = 356,
  SipParser_OPTIONAL_URL_PARAMETERS_token, SipParser_OPTIONAL_HEADERS_token,
  SipParser_URL_HOST_token, SipParser_GENERAL_URL_WITHOUT_PARAM_token,
  SipParser_SIP_URL_WITHOUT_PARAMS_token,
  SipParser_TEL_URL_WITHOUT_PARAMS_token,
  SipParser_PRES_URL_WITHOUT_PARAMS_token,
  SipParser_IM_URL_WITHOUT_PARAMS_token,
  SipParser_ABS_URI_WITHOUT_PARAMS_token,
  SipParser_URL_USERINFO_PREFIX_token, SipParser_URL_USERINFO_VAL_token,
  SipParser_HOSTPORT_token, SipParser_USER_INFO_TOKEN_token = 370,
  SipParser_PASSWORD_token, SipParser_TOKEN_USER_INFO_token,
  SipParser_TOKEN_USER_INFO_CHAR_token,
  SipParser_PASSWORD_STRING_token = 375,
  SipParser_PASSWORD_STRING_CHAR_token,
  SipParser_URL_HOST_PREFIX_token = 380, SipParser_TOKEN_HOST_token = 382,
  SipParser_TOKEN_HOST_CHAR_token, SipParser_IPV_4_ADDRESS_token = 387,
  SipParser_IPV_4_PART_token, SipParser_HEXPART_token,
  SipParser_HEXSEQ_token, SipParser_HEX4_token = 393,
  SipParser_URL_PARAMETERS_token = 397, SipParser_URL_PARAMETER_token,
  SipParser_TRANSPORT_PARAM_token, SipParser_USER_PARAM_token,
  SipParser_TTL_PARAM_URL_token, SipParser_MADDR_PARAM_URL_token,
  SipParser_METHOD_PARAM_URL_token, SipParser_LR_PARAM_URL_token,
  SipParser_COMPRESSION_PARAM_URL_token,
  SipParser_TOKENIZED_BY_PARAM_URL_token, SipParser_ORIG_PARAM_URL_token,
  SipParser_USER_PARAM_TYPE_token = 410, SipParser_OTHER_USER_token,
  SipParser_URL_OTHER_PARAM_VAL_token = 423, SipParser_PVALUE_token,
  SipParser_PARAMCHAR_token, SipParser_HEADERS_LIST_token = 429,
  SipParser_HEADER_token, SipParser_HNAME_token = 432,
  SipParser_HVALUE_token, SipParser_REQUEST_LINE_VALUE_token = 440,
  SipParser_REQUEST_METHOD_token, SipParser_PARTY_VALUE_token = 455,
  SipParser_NAME_ADDR_token, SipParser_PARTY_PARAMS_LIST_token,
  SipParser_MORE_THAN_ONE_TOKEN_token = 459,
  SipParser_TAG_PARAM_token = 463, SipParser_PARTY_PARAM_DATA_token = 465,
  SipParser_CONTACT_HEADER_VALUES_token,
  SipParser_CONTACT_HEADER_OTHER_token,
  SipParser_CONTACT_HEADER_VALUE_LIST_token,
  SipParser_CONTACT_HEADER_VALUE_token = 470,
  SipParser_CONTACT_PARAMS_token, SipParser_CONTACT_PARAM_token,
  SipParser_Q_token, SipParser_ACTION_token,
  SipParser_CONTACT_EXPIRES_token, SipParser_Q_VALUE_token = 477,
  SipParser_Q_VALUE_OPTIONAL_token = 479,
  SipParser_ACTION_VALUE_token = 483, SipParser_EXPIRES_VALUE_token = 486,
  SipParser_EXPIRES_VALUE_FOR_CONTACT_token = 488,
  SipParser_DELTA_SECONDS_token, SipParser_SIP_DATE_token,
  SipParser_SIP_DATE_HEADER_VAL_token, SipParser_DATE_token,
  SipParser_TIME_token, SipParser_DAY_token, SipParser_DD_token = 502,
  SipParser_MM_token, SipParser_YY_token, SipParser_HOUR_token = 517,
  SipParser_MINUTE_token, SipParser_SECOND_token, SipParser_TIME_ZONE_token,
  SipParser_AUTHENTICATION_VAL_token = 524, SipParser_CHALLENGE_VALUE_token,
  SipParser_DIGEST_CHALLENGE_LIST_token, SipParser_AUTH_PARAM_LIST_token,
  SipParser_DIGEST_CHALLENGE_token, SipParser_REALM_token,
  SipParser_DOMAIN_token, SipParser_NONCE_token, SipParser_OPAQUE_token,
  SipParser_INTEGRITY_KEY_token, SipParser_CIPHER_KEY_token,
  SipParser_AUTH_PARAM_token, SipParser_STALE_token,
  SipParser_ALGORITHM_token, SipParser_QOP_OPTIONS_token,
  SipParser_PARAM_IN_NEXT_CHALLENGE_token,
  SipParser_REALM_VALUE_token = 541, SipParser_DOMAIN_EQUAL_token = 543,
  SipParser_DOMAIN_WHITE_token, SipParser_DOMAIN_VAL_token = 546,
  SipParser_URI_LIST_token, SipParser_STRING_DOMAIN_PATCH_token,
  SipParser_DOMAIN_SPACE_token, SipParser_NONCE_VALUE_token = 552,
  SipParser_OPAQUE_VALUE_token = 554,
  SipParser_ALGORITHM_DECLARATION_token = 557,
  SipParser_ALGORITHM_AND_AKAV_token, SipParser_AKAV_PREFIX_token = 561,
  SipParser_AKAV_token, SipParser_AKA_VERSION_VAL_token,
  SipParser_ALGORITHM_VAL_token, SipParser_ALGORITHM_PREFIX_token,
  SipParser_QOP_VALUE_LIST_token = 573, SipParser_QOP_VALUE_token,
  SipParser_QUOTED_HEX_VALUE_token = 578,
  SipParser_AUTH_PARAM_DATA_token = 580, SipParser_AUTHORIZATION_VAL_token,
  SipParser_CREDENTIALS_VALUE_token, SipParser_DIGEST_RESPONSE_LIST_token,
  SipParser_DIGEST_RESPONSE_token, SipParser_USERNAME_token,
  SipParser_RESPONSE_token, SipParser_CNONCE_token,
  SipParser_NONCE_COUNT_token, SipParser_AUTS_PARAM_token,
  SipParser_INTEGRITY_PROTECTED_token, SipParser_MESSAGE_QOP_token,
  SipParser_DIGEST_URI_token, SipParser_USERNAME_VALUE_token = 594,
  SipParser_DIGEST_URI_VALUE_token = 596,
  SipParser_CNONCE_VALUE_token = 599, SipParser_NC_VALUE_token = 601,
  SipParser_LHEX_token, SipParser_REQUEST_DIGEST_token = 604,
  SipParser_THIRTY_TWO_LHEX_token,
  SipParser_SIXTY_FOUR_BASE_ENCODING_token = 609,
  SipParser_SIXTY_FOUR_BASE_ENCODING_CHAR_token,
  SipParser_ONE_OR_TWO_EQUAL_SIGNS_token = 612,
  SipParser_ROUTE_LIST_token = 619, SipParser_SINGLE_ROUTE_token,
  SipParser_RETRY_AFTER_VAL_token, SipParser_RETRY_AFTER_COMMENT_token,
  SipParser_RETRY_PARAM_token = 624, SipParser_DURATION_PARAM_token,
  SipParser_RSEQ_VAL_token = 627, SipParser_RESPONSE_NUM_token,
  SipParser_RACK_VAL_token, SipParser_OPTION_TAG_token,
  SipParser_OPTION_TAG_LIST_token, SipParser_SUPPORTED_OPTION_TAG_token,
  SipParser_REFER_TO_HEADER_VALUE_token,
  SipParser_REFERRED_BY_HEADER_VALUE_token = 635,
  SipParser_REFERRER_URI_token, SipParser_REFERRED_BY_PARAMS_token,
  SipParser_REFERRED_BY_PARAM_token = 639,
  SipParser_REFERRED_BY_PARAM_CID_token,
  SipParser_CID_PARAM_STRING_token = 642, SipParser_DOT_ATOM_token,
  SipParser_ATOM_CHAR_token, SipParser_DOT_ATOM_CHAR_token,
  SipParser_EVENT_HEADER_VAL_token = 648, SipParser_EVENT_TYPE_token,
  SipParser_EVENT_PARAMS_token, SipParser_EVENT_PACKAGE_token,
  SipParser_EVENT_TEMPLATE_token, SipParser_EVENT_PARAM_token,
  SipParser_EVENT_ID_token, SipParser_ALLOW_EVENTS_HEADER_VAL_token = 656,
  SipParser_SUBSCRIPTION_STATE_HEADER_VAL_token,
  SipParser_SUBSTATE_VALUE_token, SipParser_SUBSTATE_PARAMS_token,
  SipParser_SUBSTATE_PARAM_token = 663,
  SipParser_SUBSTATE_PARAM_REASON_token,
  SipParser_SUBSTATE_PARAM_RETRY_AFTER_token,
  SipParser_SUBSTATE_PARAM_EXPIRES_token,
  SipParser_SUBSTATE_PARAM_REASON_VAL_token = 669,
  SipParser_DISPOSITION_VAL_token = 676, SipParser_DISPOSITION_TYPE_token,
  SipParser_DISPOSITION_PARAMS_token, SipParser_HANDLING_PARAM_token = 684,
  SipParser_SESSION_EXPIRES_VALUE_token = 688,
  SipParser_REFRESHER_TYPE_token, SipParser_REFRESHER_TYPE_NAME_token,
  SipParser_REFRESHER_TYPE_VALUE_token, SipParser_MIN_SE_VALUE_token = 695,
  SipParser_REPLACES_HEADER_VALUE_token, SipParser_REPLACES_PARAMS_token,
  SipParser_REPLACES_PARAM_token, SipParser_TO_TAG_PARAM_token,
  SipParser_FROM_TAG_PARAM_token, SipParser_EARLY_FLAG_PARAM_token,
  SipParser_AUTHENTICATION_INFO_HEADER_LIST_token = 705,
  SipParser_AUTHENTICATION_INFO_HEADER_PARAM_token,
  SipParser_NEXT_NONCE_token, SipParser_RESPONSE_AUTH_token,
  SipParser_MSG_QOP_token, SipParser_RESPONSE_AUTH_VAL_token = 712,
  SipParser_P_URI_HEADERS_LIST_token = 716,
  SipParser_P_URI_HEADER_VALUE_token, SipParser_P_URI_HEADER_PARAMS_token,
  SipParser_P_VISITED_NETWORK_ID_HEADERS_LIST_token,
  SipParser_P_VISITED_NETWORK_ID_HEADER_VALUE_token,
  SipParser_VNETWORK_SPEC_token,
  SipParser_P_VISITED_NETWORK_ID_PARAMS_token,
  SipParser_P_ACCESS_NETWORK_INFO_HEADER_VALUE_token,
  SipParser_ACCESS_TYPE_token, SipParser_P_ACCESS_NETWORK_INFO_PARAMS_token,
  SipParser_ACCESS_token, SipParser_ACCESS_PARAM_token = 745,
  SipParser_CGI_3GPP_token, SipParser_UTRAN_CELL_ID_3GPP_token,
  SipParser_DSL_LOCATION_token, SipParser_CI_3GPP2_token,
  SipParser_P_CHARGING_FUNCTION_ADDRESSES_HEADER_VALUE_token = 754,
  SipParser_CHARGE_ADDR_PARAM_token, SipParser_CHARGE_ADDR_PARAMS_token,
  SipParser_CHARGE_ADDR_PARAM_CCF_token,
  SipParser_CHARGE_ADDR_PARAM_ECF_token,
  SipParser_GENERAL_VALUE_token = 760,
  SipParser_P_CHARGING_VECTOR_HEADER_VALUE_token = 762,
  SipParser_ICID_VALUE_token, SipParser_CHARGE_PARAMS_token,
  SipParser_CHARGE_PARAM_token = 766,
  SipParser_CHARGE_PARAM_ICID_GEN_ADDR_token,
  SipParser_CHARGE_PARAM_ORIG_IOI_token,
  SipParser_CHARGE_PARAM_TERM_IOI_token,
  SipParser_CHARGE_PARAM_GPRS_CHARGING_INFO_token,
  SipParser_CHARGE_PARAM_XDSL_CHARGING_INFO_token,
  SipParser_PDP_INFO_HIERARCHY_token,
  SipParser_DSL_BEARER_INFO_HIERARCHY_token,
  SipParser_I_WLAN_CHARGING_INFO_token, SipParser_GGSN_token = 778,
  SipParser_AUTH_TOKEN_token, SipParser_BRAS_token,
  SipParser_PDP_INFO_token = 785, SipParser_DSL_BEARER_INFO_token = 787,
  SipParser_SINGLE_PDP_INFO_token, SipParser_SINGLE_DSL_BEARER_INFO_token,
  SipParser_PDP_ITEM_token, SipParser_PDP_SIG_token,
  SipParser_PDP_CID_token, SipParser_FLOW_ID_token,
  SipParser_YES_OR_NO_token = 796, SipParser_DSL_BEARER_ITEM_token = 798,
  SipParser_DSL_BEARER_SIG_token, SipParser_DSL_BEARER_CID_token,
  SipParser_LPAREN_token = 805, SipParser_FLOW_ID_VALUE_token,
  SipParser_FLOW_ID_SYNTAX1_token, SipParser_FLOW_ID_SYNTAX2_token,
  SipParser_DIGITS_token = 812, SipParser_WHITE_IN_WORD_token,
  SipParser_P_MEDIA_AUTHORIZATION_VAL_token = 817,
  SipParser_P_MEDIA_AUTHORIZATION_TOKEN_token,
  SipParser_SECURITY_HEADERS_LIST_token = 820,
  SipParser_SECURITY_HEADER_VALUE_token, SipParser_MECHANISM_NAME_token,
  SipParser_MECHANISM_PARAMS_token, SipParser_MECHANISM_token,
  SipParser_MECHANISM_PARAM_token = 828, SipParser_DIGEST_ALGORITHM_token,
  SipParser_DIGEST_QOP_token, SipParser_DIGEST_VERIFY_token,
  SipParser_SECURITY_ALGORITHM_token, SipParser_SECURITY_PROTOCOL_token,
  SipParser_SECURITY_MODE_token, SipParser_SECURITY_ENCRYPT_ALGORITHM_token,
  SipParser_SECURITY_SPIC_token, SipParser_SECURITY_SPIS_token,
  SipParser_SECURITY_PORTC_token, SipParser_SECURITY_PORTS_token,
  SipParser_DIGEST_ALGORITHM_DECLARATION_token,
  SipParser_DIGEST_VERIFY_VALUE_token = 844,
  SipParser_SECURITY_ALGORITHM_VAL_token = 846,
  SipParser_SECURITY_PROTOCOL_VAL_token = 850,
  SipParser_SECURITY_MODE_VAL_token = 854,
  SipParser_SECURITY_ENCRYPT_ALGORITHM_VAL_token = 858,
  SipParser_TEN_DIGIT_token = 863,
  SipParser_P_DCS_TRACE_PARTY_ID_HEADER_VALUE_token = 867,
  SipParser_P_DCS_OSPS_HEADER_VALUE_token, SipParser_OSPS_TAG_token,
  SipParser_P_DCS_BILLING_INFO_HEADER_VALUE_token = 873,
  SipParser_BILLING_CORRELATION_ID_token, SipParser_FEID_token,
  SipParser_FEID_HOST_token, SipParser_BILLING_INFO_PARAMS_token,
  SipParser_BILLING_CORRELATION_ID_VALUE_token,
  SipParser_FORTY_EIGHT_HEXDIG_token, SipParser_FEID_VALUE_token,
  SipParser_SIXTEEN_HEXDIG_token, SipParser_BILLING_INFO_PARAM_token,
  SipParser_P_DCS_LAES_HEADER_VALUE_token = 889, SipParser_LAES_SIG_token,
  SipParser_LAES_PARAMS_token, SipParser_LAES_PARAM_token,
  SipParser_LAES_CONTENT_token = 894,
  SipParser_P_DCS_REDIRECT_HEADER_VALUE_token = 896,
  SipParser_CALLED_ID_URI_token, SipParser_REDIRECT_PARAMS_token,
  SipParser_REDIRECT_PARAM_token
} SipParser_token_type;

typedef struct SipParser_pcb_struct{
  SipParser_token_type token_number, reduction_token, error_frame_token;
  int input_code;
  int input_value;
  int line, column;
  int ssx, sn, error_frame_ssx;
  int drt, dssx, dsn;
  int ss[49];
  SipParser_vs_type vs[49];
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












		ParserPUriHeader				puriHeader; 
		ParserPVisitedNetworkIDHeader	pvisitedNetworkIDHeader; 
		ParserPAccessNetworkInfoHeader	paccessNetworkInfoHeader; 
		ParserPChargingFunctionAddressesHeader	pchargingFunctionAddressesHeader; 
		ParserPChargingVectorHeader		pchargingVectorHeader; 
		ParserPMediaAuthorizationHeader	pmediaAuthorizationHeader; 
		ParserSecurityHeader			securityHeader; 


		ParserPDCSTracePartyIDHeader	pdcsTracePartyIDHeader; 
		ParserPDCSOSPSHeader			pdcsOSPSHeader; 
		ParserPDCSBillingInfoHeader		pdcsBillingInfoHeader; 
		ParserPDCSLAESHeader			pdcsLAESHeader; 
		ParserPDCSRedirectHeader		pdcsRedirectHeader; 


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

#elif defined(RV_SIP_AUTH_ON) && defined(RV_SIP_TEL_URI_SUPPORT) && defined(RV_SIP_IMS_HEADER_SUPPORT) && !defined(RV_SIP_IMS_DCS_HEADER_SUPPORT) && !defined(RV_SIP_EXTENDED_HEADER_SUPPORT) && !defined(RV_SIP_JSR32_SUPPORT) && !defined(RV_SIP_LIGHT) && !defined(RV_SIP_PRIMITIVES) && !defined(RV_SIP_SUBS_ON)
#ifndef PARSERENGINEIMS_H_1165245772
#define PARSERENGINEIMS_H_1165245772

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
  char ag_vt_63[sizeof(ParserDisposition)];
  char ag_vt_64[sizeof(ParserDispositionParams)];
  char ag_vt_65[sizeof(ParserDispositionParam)];
  char ag_vt_66[sizeof(ParserSessionExpiresHeader)];
  char ag_vt_67[sizeof(RvSipSessionExpiresRefresherType)];
  char ag_vt_68[sizeof(ParserMinSEHeader)];
  char ag_vt_69[sizeof(ParserReplacesEarlyFlagParamType)];
  char ag_vt_70[sizeof(ParserAccess)];
  char ag_vt_71[sizeof(ParserMechanism)];
  char ag_vt_72[sizeof(ParserSecurityAlgorithmType)];
  char ag_vt_73[sizeof(ParserSecurityProtocolType)];
  char ag_vt_74[sizeof(ParserSecurityModeType)];
  char ag_vt_75[sizeof(ParserSecurityEncryptAlgorithmType)];
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
  SipParser_TEL_URI_HEADER_token, SipParser_P_URI_HEADER_token = 59,
  SipParser_P_VISITED_NETWORK_ID_HEADER_token = 61,
  SipParser_P_ACCESS_NETWORK_INFO_HEADER_token = 63,
  SipParser_P_CHARGING_FUNCTION_ADDRESSES_HEADER_token = 65,
  SipParser_P_CHARGING_VECTOR_HEADER_token = 67,
  SipParser_P_MEDIA_AUTHORIZATION_HEADER_token = 69,
  SipParser_SECURITY_HEADER_token = 71, SipParser_OTHER_HEADER_token,
  SipParser_WHITE_token = 74, SipParser_SP_token = 76,
  SipParser_COMMA_token = 78, SipParser_COMMA_BETWEEN_HEADERS_token = 80,
  SipParser_EQUAL_token = 83, SipParser_AT_token = 85,
  SipParser_COLON_token = 87, SipParser_SEMI_COLON_token,
  SipParser_MARK_SIGN_token = 90, SipParser_SLASH_token = 92,
  SipParser_LEFT_PARENTHESIS_token = 94,
  SipParser_LEFT_ANGLE_BRACKET_token = 96,
  SipParser_LEFT_QUOTATION_token = 98, SipParser_HYPHEN_token = 100,
  SipParser_TOKEN_token = 102, SipParser_TOKEN_CHAR_token,
  SipParser_CALL_ID_WORD_token = 105, SipParser_CALL_ID_WORD_CHAR_token,
  SipParser_TOKEN_FOR_CONTENT_TYPE_token = 108,
  SipParser_TOKEN_CHAR_FOR_CONTENT_TYPE_token,
  SipParser_FIELD_VALUE_token = 111, SipParser_QUOTED_STRING_token = 117,
  SipParser_QUOTED_CHAR_token, SipParser_HEX_VALUE_token = 125,
  SipParser_HEX_token, SipParser_INT_NUMBER_token = 128,
  SipParser_DIGIT_token, SipParser_TOKEN_OR_QUOTED_STRING_token = 131,
  SipParser_METHOD_token, SipParser_CSEQ_VAL_token = 139,
  SipParser_SEQUNCE_NUMBER_token, SipParser_CONTENT_LENGTH_VAL_token,
  SipParser_CALL_ID_token, SipParser_MEDIA_TYPE_token,
  SipParser_MEDIA_TYPE_TYPE_token, SipParser_MEDIA_TYPE_SUB_TYPE_token,
  SipParser_OPTIONAL_CONTENT_TYPE_PARAM_LIST_token,
  SipParser_CONTENT_TYPE_PARAM_LIST_token, SipParser_BOUNDARY_PARAM_token,
  SipParser_VERSION_PARAM_token, SipParser_BASE_PARAM_token,
  SipParser_CONTENT_TYPE_GENERIC_PARAM_DATA_token,
  SipParser_CONTENT_TYPE_GENERIC_PARAM_DATA_VAL_token = 171,
  SipParser_GENERIC_PARAM_LIST_token, SipParser_GENERIC_PARAM_DATA_token,
  SipParser_GENERIC_PARAM_DATA_VAL_token, SipParser_SIP_VERSION_token,
  SipParser_STATUS_CODE_token = 177, SipParser_REASON_PHRASE_token,
  SipParser_EXTENSION_CODE_token = 180, SipParser_REASONE_TEXT_UTF8_token,
  SipParser_ALLOW_VAL_token = 185, SipParser_VIA_LIST_token,
  SipParser_SINGLE_VIA_token, SipParser_SENT_PROTOCOL_token,
  SipParser_SENT_BY_token, SipParser_VIA_PARAM_LIST_token,
  SipParser_VIA_PARAM_token, SipParser_PROTOCOL_NAME_token,
  SipParser_PROTOCOL_VERSION_token, SipParser_TRANSPORT_token,
  SipParser_TRANSPORT_TYPE_token = 196, SipParser_PORT_token = 201,
  SipParser_HOST_NAME_token, SipParser_VIA_HIDDEN_token,
  SipParser_VIA_ALIAS_token, SipParser_VIA_TTL_token,
  SipParser_VIA_MADDR_token, SipParser_VIA_RECEIVED_token,
  SipParser_VIA_BRANCH_token, SipParser_RESPONSE_PORT_token,
  SipParser_VIA_COMPRESSION_token, SipParser_OTHER_PARAM_token,
  SipParser_OTHER_PARAM_EXPR_token, SipParser_IPV_6_REFERENCE_token,
  SipParser_TTL_PARAM_token = 216, SipParser_TTL_token = 218,
  SipParser_RPORT_NAME_token, SipParser_RPORT_VAL_token,
  SipParser_MADDR_PARAM_token = 222,
  SipParser_HOST_NAME_FOR_RECEIVED_token = 225,
  SipParser_COMPRESSION_TYPE_token = 228,
  SipParser_IPV_6_ADDRESS_token = 230, SipParser_TEST_COMMENT_token,
  SipParser_COMMENT_TEXT_token, SipParser_END_COMMENT_token = 234,
  SipParser_REQUEST_URI_token = 237, SipParser_ADDR_SPEC_token,
  SipParser_ABSOLUTE_URI_token, SipParser_ABS_URI_SCHEME_token,
  SipParser_URI_IDENTIFIER_token, SipParser_ABS_URI_SCHEME_CHAR_token,
  SipParser_URI_SCHEME_OR_DISPLAY_NAME_token = 244,
  SipParser_DISPLAY_NAME_token, SipParser_URI_IDENTIFIER_CHAR_token,
  SipParser_URI_IDENTIFIER_WITHOUT_SEPARATORS_token = 248,
  SipParser_TEL_URI_token = 251, SipParser_TEL_PREFIX_token,
  SipParser_TEL_token, SipParser_GLOBAL_OR_LOCAL_NUMBER_token = 255,
  SipParser_OPTIONAL_TEL_URI_PARAMETERS_token,
  SipParser_GLOBAL_NUMBER_token, SipParser_LOCAL_NUMBER_token,
  SipParser_GLOBAL_NUMBER_DIGITS_token, SipParser_PHONEDIGIT_token = 261,
  SipParser_VISUAL_SEPERATOR_token = 265,
  SipParser_LOCAL_NUMBER_DIGITS_token = 267,
  SipParser_HEX_OR_PHONECHAR_token, SipParser_PHONEDIGIT_HEX_token,
  SipParser_PHONECHAR_token = 273, SipParser_TEL_URI_PARAMETERS_token,
  SipParser_TEL_URI_PARAMETER_token,
  SipParser_TEL_URI_EXTENSION_PARAM_token,
  SipParser_TEL_URI_POST_DIAL_PARAM_token,
  SipParser_TEL_URI_ISDN_SUB_ADDR_PARAM_URL_token,
  SipParser_TEL_URI_CONTEXT_PARAM_token,
  SipParser_TEL_URI_ENUMDI_PARAM_token,
  SipParser_URL_GENERIC_PARAM_DATA_token,
  SipParser_TEL_URI_EXTENTION_VALUE_token = 283,
  SipParser_TEL_URI_POST_DIAL_VALUE_token = 286,
  SipParser_TEL_URI_POST_DIAL_CHAR_token, SipParser_DTMFDIGIT_token = 289,
  SipParser_PAUSE_CHAR_token,
  SipParser_TEL_URI_ISDN_SUB_ADDR_VALUE_token = 292, SipParser_URIC_token,
  SipParser_RESERVED_FOR_URIC_token = 295, SipParser_UNRESERVED_token,
  SipParser_PCT_ENCODED_token, SipParser_TEL_URI_CONTEXT_VALUE_token = 300,
  SipParser_DESCRIPTOR_token, SipParser_DOMAINNAME_token,
  SipParser_LABEL_token, SipParser_NEXT_LABEL_token,
  SipParser_ALPHA_token = 308, SipParser_ALPHANUM_OR_HYPHEN_token,
  SipParser_ALPHANUM_token = 314, SipParser_SIP_URL_token = 316,
  SipParser_PRES_URI_token, SipParser_IM_URI_token,
  SipParser_SIP_PREFIX_token, SipParser_URL_token,
  SipParser_PRES_PREFIX_token = 323, SipParser_RV_ABS_PREFIX_token,
  SipParser_IM_PREFIX_token = 327, SipParser_URL_USERINFO_token = 329,
  SipParser_OPTIONAL_URL_PARAMETERS_token, SipParser_OPTIONAL_HEADERS_token,
  SipParser_URL_HOST_token, SipParser_GENERAL_URL_WITHOUT_PARAM_token,
  SipParser_SIP_URL_WITHOUT_PARAMS_token,
  SipParser_TEL_URL_WITHOUT_PARAMS_token,
  SipParser_PRES_URL_WITHOUT_PARAMS_token,
  SipParser_IM_URL_WITHOUT_PARAMS_token,
  SipParser_ABS_URI_WITHOUT_PARAMS_token,
  SipParser_URL_USERINFO_PREFIX_token, SipParser_URL_USERINFO_VAL_token,
  SipParser_HOSTPORT_token, SipParser_USER_INFO_TOKEN_token = 343,
  SipParser_PASSWORD_token, SipParser_TOKEN_USER_INFO_token,
  SipParser_TOKEN_USER_INFO_CHAR_token,
  SipParser_PASSWORD_STRING_token = 348,
  SipParser_PASSWORD_STRING_CHAR_token,
  SipParser_URL_HOST_PREFIX_token = 353, SipParser_TOKEN_HOST_token = 355,
  SipParser_TOKEN_HOST_CHAR_token, SipParser_IPV_4_ADDRESS_token = 360,
  SipParser_IPV_4_PART_token, SipParser_HEXPART_token,
  SipParser_HEXSEQ_token, SipParser_HEX4_token = 366,
  SipParser_URL_PARAMETERS_token = 370, SipParser_URL_PARAMETER_token,
  SipParser_TRANSPORT_PARAM_token, SipParser_USER_PARAM_token,
  SipParser_TTL_PARAM_URL_token, SipParser_MADDR_PARAM_URL_token,
  SipParser_METHOD_PARAM_URL_token, SipParser_LR_PARAM_URL_token,
  SipParser_COMPRESSION_PARAM_URL_token,
  SipParser_TOKENIZED_BY_PARAM_URL_token, SipParser_ORIG_PARAM_URL_token,
  SipParser_USER_PARAM_TYPE_token = 383, SipParser_OTHER_USER_token,
  SipParser_URL_OTHER_PARAM_VAL_token = 396, SipParser_PVALUE_token,
  SipParser_PARAMCHAR_token, SipParser_HEADERS_LIST_token = 402,
  SipParser_HEADER_token, SipParser_HNAME_token = 405,
  SipParser_HVALUE_token, SipParser_REQUEST_LINE_VALUE_token = 413,
  SipParser_REQUEST_METHOD_token, SipParser_PARTY_VALUE_token = 428,
  SipParser_NAME_ADDR_token, SipParser_PARTY_PARAMS_LIST_token,
  SipParser_MORE_THAN_ONE_TOKEN_token = 432,
  SipParser_TAG_PARAM_token = 436, SipParser_PARTY_PARAM_DATA_token = 438,
  SipParser_CONTACT_HEADER_VALUES_token,
  SipParser_CONTACT_HEADER_OTHER_token,
  SipParser_CONTACT_HEADER_VALUE_LIST_token,
  SipParser_CONTACT_HEADER_VALUE_token = 443,
  SipParser_CONTACT_PARAMS_token, SipParser_CONTACT_PARAM_token,
  SipParser_Q_token, SipParser_ACTION_token,
  SipParser_CONTACT_EXPIRES_token, SipParser_Q_VALUE_token = 450,
  SipParser_Q_VALUE_OPTIONAL_token = 452,
  SipParser_ACTION_VALUE_token = 456, SipParser_EXPIRES_VALUE_token = 459,
  SipParser_EXPIRES_VALUE_FOR_CONTACT_token = 461,
  SipParser_DELTA_SECONDS_token, SipParser_SIP_DATE_token,
  SipParser_SIP_DATE_HEADER_VAL_token, SipParser_DATE_token,
  SipParser_TIME_token, SipParser_DAY_token, SipParser_DD_token = 475,
  SipParser_MM_token, SipParser_YY_token, SipParser_HOUR_token = 490,
  SipParser_MINUTE_token, SipParser_SECOND_token, SipParser_TIME_ZONE_token,
  SipParser_AUTHENTICATION_VAL_token = 497, SipParser_CHALLENGE_VALUE_token,
  SipParser_DIGEST_CHALLENGE_LIST_token, SipParser_AUTH_PARAM_LIST_token,
  SipParser_DIGEST_CHALLENGE_token, SipParser_REALM_token,
  SipParser_DOMAIN_token, SipParser_NONCE_token, SipParser_OPAQUE_token,
  SipParser_INTEGRITY_KEY_token, SipParser_CIPHER_KEY_token,
  SipParser_AUTH_PARAM_token, SipParser_STALE_token,
  SipParser_ALGORITHM_token, SipParser_QOP_OPTIONS_token,
  SipParser_PARAM_IN_NEXT_CHALLENGE_token,
  SipParser_REALM_VALUE_token = 514, SipParser_DOMAIN_EQUAL_token = 516,
  SipParser_DOMAIN_WHITE_token, SipParser_DOMAIN_VAL_token = 519,
  SipParser_URI_LIST_token, SipParser_STRING_DOMAIN_PATCH_token,
  SipParser_DOMAIN_SPACE_token, SipParser_NONCE_VALUE_token = 525,
  SipParser_OPAQUE_VALUE_token = 527,
  SipParser_ALGORITHM_DECLARATION_token = 530,
  SipParser_ALGORITHM_AND_AKAV_token, SipParser_AKAV_PREFIX_token = 534,
  SipParser_AKAV_token, SipParser_AKA_VERSION_VAL_token,
  SipParser_ALGORITHM_VAL_token, SipParser_ALGORITHM_PREFIX_token,
  SipParser_QOP_VALUE_LIST_token = 546, SipParser_QOP_VALUE_token,
  SipParser_QUOTED_HEX_VALUE_token = 551,
  SipParser_AUTH_PARAM_DATA_token = 553, SipParser_AUTHORIZATION_VAL_token,
  SipParser_CREDENTIALS_VALUE_token, SipParser_DIGEST_RESPONSE_LIST_token,
  SipParser_DIGEST_RESPONSE_token, SipParser_USERNAME_token,
  SipParser_RESPONSE_token, SipParser_CNONCE_token,
  SipParser_NONCE_COUNT_token, SipParser_AUTS_PARAM_token,
  SipParser_INTEGRITY_PROTECTED_token, SipParser_MESSAGE_QOP_token,
  SipParser_DIGEST_URI_token, SipParser_USERNAME_VALUE_token = 567,
  SipParser_DIGEST_URI_VALUE_token = 569,
  SipParser_CNONCE_VALUE_token = 572, SipParser_NC_VALUE_token = 574,
  SipParser_LHEX_token, SipParser_REQUEST_DIGEST_token = 577,
  SipParser_THIRTY_TWO_LHEX_token,
  SipParser_SIXTY_FOUR_BASE_ENCODING_token = 582,
  SipParser_SIXTY_FOUR_BASE_ENCODING_CHAR_token,
  SipParser_ONE_OR_TWO_EQUAL_SIGNS_token = 585,
  SipParser_ROUTE_LIST_token = 592, SipParser_SINGLE_ROUTE_token,
  SipParser_RETRY_AFTER_VAL_token, SipParser_RETRY_AFTER_COMMENT_token,
  SipParser_RETRY_PARAM_token = 597, SipParser_DURATION_PARAM_token,
  SipParser_RSEQ_VAL_token = 600, SipParser_RESPONSE_NUM_token,
  SipParser_RACK_VAL_token, SipParser_OPTION_TAG_token,
  SipParser_OPTION_TAG_LIST_token, SipParser_SUPPORTED_OPTION_TAG_token,
  SipParser_DISPOSITION_VAL_token, SipParser_DISPOSITION_TYPE_token,
  SipParser_DISPOSITION_PARAMS_token, SipParser_HANDLING_PARAM_token = 614,
  SipParser_SESSION_EXPIRES_VALUE_token = 618,
  SipParser_REFRESHER_TYPE_token, SipParser_REFRESHER_TYPE_NAME_token = 621,
  SipParser_REFRESHER_TYPE_VALUE_token, SipParser_MIN_SE_VALUE_token = 626,
  SipParser_REPLACES_HEADER_VALUE_token, SipParser_REPLACES_PARAMS_token,
  SipParser_REPLACES_PARAM_token, SipParser_TO_TAG_PARAM_token,
  SipParser_FROM_TAG_PARAM_token, SipParser_EARLY_FLAG_PARAM_token,
  SipParser_AUTHENTICATION_INFO_HEADER_LIST_token = 636,
  SipParser_AUTHENTICATION_INFO_HEADER_PARAM_token,
  SipParser_NEXT_NONCE_token, SipParser_RESPONSE_AUTH_token,
  SipParser_MSG_QOP_token, SipParser_RESPONSE_AUTH_VAL_token = 643,
  SipParser_P_URI_HEADERS_LIST_token = 647,
  SipParser_P_URI_HEADER_VALUE_token, SipParser_P_URI_HEADER_PARAMS_token,
  SipParser_P_VISITED_NETWORK_ID_HEADERS_LIST_token,
  SipParser_P_VISITED_NETWORK_ID_HEADER_VALUE_token,
  SipParser_VNETWORK_SPEC_token,
  SipParser_P_VISITED_NETWORK_ID_PARAMS_token,
  SipParser_P_ACCESS_NETWORK_INFO_HEADER_VALUE_token,
  SipParser_ACCESS_TYPE_token, SipParser_P_ACCESS_NETWORK_INFO_PARAMS_token,
  SipParser_ACCESS_token, SipParser_ACCESS_PARAM_token = 676,
  SipParser_CGI_3GPP_token, SipParser_UTRAN_CELL_ID_3GPP_token,
  SipParser_DSL_LOCATION_token, SipParser_CI_3GPP2_token,
  SipParser_P_CHARGING_FUNCTION_ADDRESSES_HEADER_VALUE_token = 685,
  SipParser_CHARGE_ADDR_PARAM_token, SipParser_CHARGE_ADDR_PARAMS_token,
  SipParser_CHARGE_ADDR_PARAM_CCF_token,
  SipParser_CHARGE_ADDR_PARAM_ECF_token,
  SipParser_GENERAL_VALUE_token = 691,
  SipParser_P_CHARGING_VECTOR_HEADER_VALUE_token = 693,
  SipParser_ICID_VALUE_token, SipParser_CHARGE_PARAMS_token,
  SipParser_CHARGE_PARAM_token = 697,
  SipParser_CHARGE_PARAM_ICID_GEN_ADDR_token,
  SipParser_CHARGE_PARAM_ORIG_IOI_token,
  SipParser_CHARGE_PARAM_TERM_IOI_token,
  SipParser_CHARGE_PARAM_GPRS_CHARGING_INFO_token,
  SipParser_CHARGE_PARAM_XDSL_CHARGING_INFO_token,
  SipParser_PDP_INFO_HIERARCHY_token,
  SipParser_DSL_BEARER_INFO_HIERARCHY_token,
  SipParser_I_WLAN_CHARGING_INFO_token, SipParser_GGSN_token = 709,
  SipParser_AUTH_TOKEN_token, SipParser_BRAS_token,
  SipParser_PDP_INFO_token = 716, SipParser_DSL_BEARER_INFO_token = 718,
  SipParser_SINGLE_PDP_INFO_token, SipParser_SINGLE_DSL_BEARER_INFO_token,
  SipParser_PDP_ITEM_token, SipParser_PDP_SIG_token,
  SipParser_PDP_CID_token, SipParser_FLOW_ID_token,
  SipParser_YES_OR_NO_token = 727, SipParser_DSL_BEARER_ITEM_token = 729,
  SipParser_DSL_BEARER_SIG_token, SipParser_DSL_BEARER_CID_token,
  SipParser_LPAREN_token = 736, SipParser_FLOW_ID_VALUE_token,
  SipParser_FLOW_ID_SYNTAX1_token, SipParser_FLOW_ID_SYNTAX2_token,
  SipParser_DIGITS_token = 743, SipParser_WHITE_IN_WORD_token,
  SipParser_P_MEDIA_AUTHORIZATION_VAL_token = 748,
  SipParser_P_MEDIA_AUTHORIZATION_TOKEN_token,
  SipParser_SECURITY_HEADERS_LIST_token = 751,
  SipParser_SECURITY_HEADER_VALUE_token, SipParser_MECHANISM_NAME_token,
  SipParser_MECHANISM_PARAMS_token, SipParser_MECHANISM_token,
  SipParser_MECHANISM_PARAM_token = 759, SipParser_DIGEST_ALGORITHM_token,
  SipParser_DIGEST_QOP_token, SipParser_DIGEST_VERIFY_token,
  SipParser_SECURITY_ALGORITHM_token, SipParser_SECURITY_PROTOCOL_token,
  SipParser_SECURITY_MODE_token, SipParser_SECURITY_ENCRYPT_ALGORITHM_token,
  SipParser_SECURITY_SPIC_token, SipParser_SECURITY_SPIS_token,
  SipParser_SECURITY_PORTC_token, SipParser_SECURITY_PORTS_token,
  SipParser_DIGEST_ALGORITHM_DECLARATION_token,
  SipParser_DIGEST_VERIFY_VALUE_token = 775,
  SipParser_SECURITY_ALGORITHM_VAL_token = 777,
  SipParser_SECURITY_PROTOCOL_VAL_token = 781,
  SipParser_SECURITY_MODE_VAL_token = 785,
  SipParser_SECURITY_ENCRYPT_ALGORITHM_VAL_token = 789,
  SipParser_TEN_DIGIT_token = 794
} SipParser_token_type;

typedef struct SipParser_pcb_struct{
  SipParser_token_type token_number, reduction_token, error_frame_token;
  int input_code;
  int input_value;
  int line, column;
  int ssx, sn, error_frame_ssx;
  int drt, dssx, dsn;
  int ss[46];
  SipParser_vs_type vs[46];
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










        ParserRetryAfter           retryAfter;  
        ParserContentDisposition   contentDisposition; 
        ParserRack                 rack;        
        ParserReplacesParams       replacesParams; 
        ParserReplacesParam        replacesParam;  

        ParserSentByInVia          viaSentBy;       












		ParserPUriHeader				puriHeader; 
		ParserPVisitedNetworkIDHeader	pvisitedNetworkIDHeader; 
		ParserPAccessNetworkInfoHeader	paccessNetworkInfoHeader; 
		ParserPChargingFunctionAddressesHeader	pchargingFunctionAddressesHeader; 
		ParserPChargingVectorHeader		pchargingVectorHeader; 
		ParserPMediaAuthorizationHeader	pmediaAuthorizationHeader; 
		ParserSecurityHeader			securityHeader; 









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
