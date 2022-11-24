/*******************************************************************************   
 * FILE PURPOSE:    DAA IO Control Driver 
 *******************************************************************************   
 * FILE NAME:       daaioc.c                                                   
 *                                                                                 
 * DESCRIPTION:     Header code for Linux DAA Driver (AudioCodes NIRP)
 *                                                                                 
 * REVISION HISTORY:  
 *   
 * Date           Description                               Author
 *-----------------------------------------------------------------------------
 ******************************************************************************/   
#ifndef DAAIOC_H
#define DAAIOC_H


#define DAA_CMD_CODE 0xE1

typedef enum {
	mtChangeStateToOffHook = 0,
	mtChangeStateToOnHook,
	mtReadCIDInfo
} MODE_TYPE_DAA;

#define DAAIOC_W_MODE    				_IOW(DAA_CMD_CODE, 0x91, MODE_TYPE_DAA)
#define DAAIOC_DEBUG_CMD				_IOW(DAA_CMD_CODE, 0x92, voip_dsp_debug_cmd_t*)
#define DAAIOC_INIT_DAA				    _IOW(DAA_CMD_CODE, 0x93, MODE_TYPE_DAA)
#define DAAIOC_EXECUTE_HOOK_FLASH	    _IOW(DAA_CMD_CODE, 0x94, MODE_TYPE_DAA)


#define DAAIOC_R_MODE    				_IOR(DAA_CMD_CODE, 0x81, MODE_TYPE_DAA*)
#define DAAIOC_R_FXO_DET_RING         	_IOR(DAA_CMD_CODE, 0x82, int*)
#define DAAIOC_R_FXO_READ_DEFAULT_VAL   _IOR(DAA_CMD_CODE, 0x83, int*)

#endif
