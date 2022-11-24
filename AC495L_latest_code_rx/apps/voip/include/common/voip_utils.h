/********************************************************************
 * AudioCodes improved application utils                            *
 *                                                                  *
 * File Name:	voip_utils.h                                        *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 26/02/08 - Coding started.                                       *
 *																	*
 ********************************************************************/
#ifndef _VOIP_UTILS_H_
#define _VOIP_UTILS_H_

#include "enums.h"
#include "voip_gen_defs.h"

/* conversion macros*/
typedef struct appl_code2code_s		           AC_CODE2CODE_MAP;
typedef struct code2str_s			           AC_CODE2STR_MAP;

#define AC_CODE_TO_VALUE(codeList, code)		appl_code2code(codeList, code)
#define AC_CODE_TO_STR(codeList, code)		    code2str(codeList, code)
#define AC_STR_TO_CODE(codeList, str)		    str2code(codeList, str)
/* Macro for converting an RTP payload type to the codec name */
#define AC_RTP_PT_TO_NAME(payload_type)			code2str_ex(rtp_payload_type_t_str, payload_type)
#define AC_RTP_NAME_TO_PT(payload_name)			str2code_ex(rtp_payload_type_t_str, payload_name)

#define AC_RG_COMPONENT_REGISTER(comp)			/*comp_register(comp)*/
#define AC_RG_COMPONENT_UNREGISTER(comp)		/*comp_unregister(comp)*/


#define nfree(buff) 		free(buff) 

#define MZERO(x) memset(&(x), 0, sizeof(x)) 


#if defined(AC488_CPE_VOIP_TOOLKIT)		/* 48802 has only 2 DSP channels */
#define getDspChannelByLineAndVoiceCh(line, VoiceCh) 	VoiceCh ?  (-1) : ( line ^ VoiceCh )
#else
#define getDspChannelByLineAndVoiceCh(line, VoiceCh) ( ((line)*2) - ((line)%2) + ((VoiceCh)*2) )
#endif


/* Prototypes */
int AC_TIMER_INIT();
int AC_TIMER_DESTROY();
void AC_TIMER_SET(long duration, etimer_func_t function, void *data);
int AC_TIMER_EXISTS(etimer_func_t function, void *data);
void AC_TIMER_DEL(etimer_func_t function, void *data);
void AC_IPC_TIMER_SET(long duration, etimer_func_t function, void *data);
int AC_IPC_TIMER_EXISTS(etimer_func_t function, void *data);
void AC_IPC_TIMER_DEL(etimer_func_t function, void *data);

void *AC_ZALLOC_L(size_t size);
void *AC_ZALLOC_E(size_t size);
char *AC_STRNCPY(char *dst, const char *src, size_t len);
int appl_code2code(appl_code2code_t *list, int code);
char *code2str_ex(code2str_ex_t *list, int code);
int str2code_ex(code2str_ex_t *list, char *str);
char *code2str(code2str_t *list, int code);
int str2code(code2str_t *list, char *str);
char *itoa(int num);
char *strdup_e(char *s);

#ifdef CONFIG_RG_VOIP_RV_MGCP
char **str_tolower(char **s);
#endif /* CONFIG_RG_VOIP_RV_MGCP */

/* Structure Definitions */

typedef struct
{
	int line;
	int voiceChannel;
} lineAndVoiceChannel;

extern lineAndVoiceChannel 		LineAndVoiceChannel2DspChannelConversionTable[];

extern int getDspChannelByLineAndVoiceChannel(int line, int VoiceChannel);

#endif /* _VOIP_UTILS_H_ */
