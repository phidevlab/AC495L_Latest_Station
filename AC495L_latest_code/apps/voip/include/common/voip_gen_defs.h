/********************************************************************
 * AudioCodes improved application  general definitions             *
 *                                                                  *
 * File Name:	voip_gen_defs.h                                     *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 26/02/08 - Coding started.                                       *
 *																	*
 ********************************************************************/

#ifndef _VOIP_GEN_DEFS_H_
#define _VOIP_GEN_DEFS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>
#include <semaphore.h>

#include "enums.h"
#include "acGatewayDefs.h"
#include "acTimers.h"

#include "cip_def.h"
#include "cip_api.h"
#include "cfg_api.h"


#if defined(CONFIG_RG_ATA)
#define vcCONFIG_RG_ATA 				1
#define vcCONFIG_RG_IPPHONE			0
#elif defined(CONFIG_RG_IPPHONE)
#define vcCONFIG_RG_ATA 				0
#define vcCONFIG_RG_IPPHONE			1
#endif


#define vcCONFIG_RG_FIREWALL		0


#if defined(AC49_SMB_VOIP_TOOLKIT) || defined(AC494_TULIP2_40) || defined(AC49_ORCHID_ATA_VOIP_TOOLKIT)
#define MAX_NUM_OF_LINES           		8
#elif defined(AC494_TULIP2_21)
#define MAX_NUM_OF_LINES				4
#elif defined(CONFIG_RG_IPPHONE)
#define MAX_NUM_OF_LINES          		1
#else
#define MAX_NUM_OF_LINES           		1
#endif

#define VOIP_ABSOLUTE_MAX_NUMBER_OF_LINES	8 	/* Compatible for SMB VoIP Toolklit */

#define CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES	MAX_NUM_OF_LINES	/* need to be configurable */

#define NUMBER_OF_CHANNELS_PER_LINE 	    3//2
#define NUMBER_OF_DSP_CHANNELS		CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES*NUMBER_OF_CHANNELS_PER_LINE

#define FIRMWARE_PATH					""

#if defined(AC49_ORCHID_VOIP_TOOLKIT)
//#define DSP_FIRMWARE_NAME       		FIRMWARE_PATH"494E002ce4.720.25"
#define DSP_FIRMWARE_NAME				FIRMWARE_PATH"/494002ce4.720.20"
#elif defined(AC49_ORCHID_ATA_VOIP_TOOLKIT)
#define DSP_FIRMWARE_NAME       		FIRMWARE_PATH"494E004ce2.680.07"
#else
#ifdef CONFIG_RG_ATA
#define DSP_FIRMWARE_NAME				FIRMWARE_PATH"494004ce3.680.07"
#endif
#ifdef CONFIG_RG_IPPHONE
//#define DSP_FIRMWARE_NAME				FIRMWARE_PATH"494002ce4.720.20"
#define DSP_FIRMWARE_NAME				FIRMWARE_PATH"494003ce3.720.87"
#endif
#endif

#if defined(AC49_SMB_VOIP_TOOLKIT) || defined(AC494_TULIP2_40) || defined(AC49_ORCHID_ATA_VOIP_TOOLKIT)
#define SECOND_DSP_FIRMWARE_NAME       FIRMWARE_PATH"498004ce1.680.07"
#endif

/*#define RV_CFLAG_TLS*/

/***********************/
/***  general definitions  ***/
/***********************/
#define AC_MAX_DOMAIN_NAME_LEN        	255
#define AC_MAX_HOST_NAME_LEN            		63
#define AC_DOTTED_IP_LEN                    		16 	/* length of: xxx.xxx.xxx.xxx\0 */
#define AC_MAX_USERNAME_LEN                 	64
#define AC_MAX_PASSWORD_LEN                	64

#define ADDRESS_STR_MAX_LEN			128

#define LONG_NUM_LEN 					16

#define MAX_SIP_LOG_MSG_SIZE 			1000
#define ACL_MSG_SIZE						1500

#define ON_HOOK							0
#define OFF_HOOK						1

#define 	ACL_SUCCESS	0
#define 	ACL_FAILURE		-1

#define FALSE 			0
#define TRUE  			1

/***************************/
/***  general type definitions  ***/
/***************************/
typedef unsigned int 	size_t;

typedef unsigned int	 u32;
typedef unsigned short	 u16;
typedef unsigned char	 u8;

typedef void (*etimer_func_t)(void *data);



/*********************************/
/***  general enumerators definitions  ***/
/*********************************/

typedef enum {
	DSP_DEBUG_CMD_LE88_REG_DUMP,
	DSP_DEBUG_CMD_MPI_READ,
	DSP_DEBUG_CMD_MPI_WRITE,
	DSP_DEBUG_CMD_SETUP_DEVICE,
	DSP_DEBUG_CMD_OPEN_CHANNEL,
	DSP_DEBUG_CMD_CLOSE_CHANNEL,
	DSP_DEBUG_CMD_SET_CODECS,
	DSP_DEBUG_CMD_TONE,
	DSP_DEBUG_CMD_BIT,
	DSP_DEBUG_CMD_PACKET_RECORDING,
	DSP_DEBUG_CMD_LAST,
} enum_dsp_debug_cmd_t;


//NIRP 070610 VI 54097
typedef enum {
    DSP_ADAPTATION_LAYER_GENERAL_COMMAND_SET_FXS_POLARITY_REVERSAL,
    DSP_ADAPTATION_LAYER_GENERAL_COMMAND_LAST,
} enum_dsp_adaptation_layer_general_command_t;

typedef enum {
    DSP_PHONE_INPUT_EVENT,
    SLIC_PHONE_INPUT_EVENT,
    DAA_PHONE_INPUT_EVENT,
    KEYPAD_PHONE_INPUT_EVENT,
    MISC_PHONE_INPUT_EVENT,
} enum_phone_input_events_t;

typedef enum
{
	MISC_PHONE_EVENT_TYPE_STUN,
} enum_misc_phone_event_type;

// NIRP 070527 DAA commands
typedef enum {
    DSP_DAA_COMMANDS_CHANGE_HOOK_STATE_TO_OFF_HOOK,
    DSP_DAA_COMMANDS_CHANGE_HOOK_STATE_TO_ON_HOOK,
    DSP_DAA_COMMANDS_FLASH_HOOK,
    DSP_DAA_COMMANDS_LAST,
} enum_dsp_daa_commands_t;

/*******************************/
/***  general structures definitions  ***/
/*******************************/

typedef struct {
    enum_phone_input_events_t                             phone_input_event;
    void                                                                   *info;
} voip_event_info_t;

typedef struct
{
   enum_misc_phone_event_type EventType;
   union
   {
      struct
      {
         int Sfd;
      }Stun;
   }EventInfo;
}voip_misc_phone_event_info;

typedef struct {
    enum_dsp_adaptation_layer_general_command_t command;
    int                                         TurnPolRevOn;
    int                                         line;//phy line
} dsp_adaptation_layer_general_command_args_t;

typedef struct {

	enum_dsp_debug_cmd_t 	dsp_debug_cmd;
	int						cmd_len;
	unsigned char				param[50];

} voip_dsp_debug_cmd_t;

// NIRP 070527
typedef struct {
    enum_dsp_daa_commands_t command_enum;
    int                     line;
    int                     channel;
} dsp_send_daa_commands_args_t;

typedef struct appl_code2code_s
{
   	int code;
   	int val;
} appl_code2code_t;

typedef struct code2str_s
{
	int code;
	char *str;
	char **lang; /* the GUI representation */
	int disabled;
} code2str_t;

typedef struct
{
    int code;
    char **str_list;
} code2str_ex_t;

typedef struct
{
	etimer_func_t 	func;
	void  		*data;
	int	 		id;
	int			dummy;
} timerParams2Id_t;

typedef struct
{
	PORT_STATUS	portStatus;

} line_state_t;

#endif /* _VOIP_GEN_DEFS_H_ */

