/******************************************************************** 
 * acGatewayDriversDefs - AudioCodes Analog VoIP Gateway DSP and SLIC API          * 
 *                                                                        *
 * Structure, Enum and Constant Type Definitions                          * 	
 *                                                                        *
 * File Name:	acIPPDriversDefs.h                                    *
 *                                                                        *
 * Copyright (C) 2008 AudioCodes Ltd.                                     *
 *                                                                        *
 * Modifications :                                                        *
 *                                                                        *
 * 12/11/08 - Coding started.                                             *
 *                                                                        *
 ********************************************************************/
#ifndef ACIPPDRIVERSDEFS
#define ACIPPDRIVERSDEFS

typedef enum{

	ACIPP_MEM_MODIFY_CMD_E = 100,
	ACIPP_MEM_DISPLAY_CMD_E,
	ACIPP_MEM_CHECKSUM_CMD_E, 
	ACIPP_SET_DSP_DEVICE_CONTROL_REGISTER_CMD_E,
	ACIPP_SET_INPUT_GAIN_CMD_E,
	ACIPP_SET_OUTPUT_GAIN_CMD_E,
	ACIPP_SET_SPK_STATUS_CMD_E,
	ACIPP_RING_START_CMD_E,
	ACIPP_RING_STOP_CMD_E
}acIPPEDriversCommands;


typedef enum {
	AC494_CODEC_NONE,
	AC494_CODEC_HANDSET,
	AC494_CODEC_HEADSET,
	AC494_CODEC_MIC,
	AC494_CODEC_SPEAKER,
	AC494_CODEC_LINE

} acIPPCodecType;

/********************************************************************* 
 * Structure Definitions     												  *
 *********************************************************************/

typedef struct {

	unsigned int 					Command;
	int 							Channel;
	void				 			*data;
} acIPPCommand;


#endif
	

