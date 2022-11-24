
/*******************************************************************************   
 * FILE PURPOSE:    SLIC IO Control Driver 
 *******************************************************************************   
 * FILE NAME:       slicioc.c                                                   
 *                                                                                 
 * DESCRIPTION:     Header code for Linux slic Driver                             
 *                                                                                 
 * REVISION HISTORY:  
 *   
 * Date           Description                               Author
 *-----------------------------------------------------------------------------
 
 * 2005/7/4 23:54	First draft								Paul Wong
 * (C) Copyright 2005, Outline Electronics Limited                                      
 ******************************************************************************/   
#ifndef SLICIOC_H
#define SLICIOC_H

#define SLIC_CMD_CODE 0xEE

typedef enum {
	mtOPENCIRCUIT = 0,
	mtRINGING,
	mtACTIVE,
	mtONHOOKTX,
	mtTIPOPEN,
	mtSTANDBY,
	mtACTIVEPOLARITYREVERSAL,
	mtOHTPOLARITYREVERSAL
} MODE_TYPE;

typedef enum {
	suUNSet= 0,
	suSet
} SET_UNSET;

#define SLICIOC_W_MODE    				_IOW(SLIC_CMD_CODE, 0x91, MODE_TYPE)
#define SLICIOC_W_RELAY  				_IOW(SLIC_CMD_CODE, 0x92, SET_UNSET)
#define SLICIOC_W_CID  					_IOW(SLIC_CMD_CODE, 0x93, SET_UNSET)
#define SLICIOC_W_HOOK_ON_LINE  		_IOW(SLIC_CMD_CODE, 0x94, SET_UNSET)
#define SLICIOC_W_RING_SET		  		_IOW(SLIC_CMD_CODE, 0x95, SET_UNSET)
#define SLICIOC_W_RING_COUNT_CONFIG 	_IOW(SLIC_CMD_CODE, 0x96, int)
#define SLICIOC_DEBUG_CMD				_IOW(SLIC_CMD_CODE, 0x97, voip_dsp_debug_cmd_t*)//nirp dsp cli


#define SLICIOC_R_MODE    				_IOR(SLIC_CMD_CODE, 0x81, MODE_TYPE*)
#define SLICIOC_R_RELAY  				_IOR(SLIC_CMD_CODE, 0x82, SET_UNSET*)
#define SLICIOC_R_CID  					_IOR(SLIC_CMD_CODE, 0x83, SET_UNSET*)
#define SLICIOC_R_HOOK_DET_ON_PHONE  	_IOR(SLIC_CMD_CODE, 0xA1, SET_UNSET*)
#define SLICIOC_R_HOOK_DET_ON_LINE  	_IOR(SLIC_CMD_CODE, 0xA2, SET_UNSET*)
#define SLICIOC_R_RING_DET  			_IOR(SLIC_CMD_CODE, 0xA3, SET_UNSET*)
#define SLICIOC_R_LCUR_DET  			_IOR(SLIC_CMD_CODE, 0xA4, SET_UNSET*)
#define SLICIOC_R_RING_COUNT		 	_IOR(SLIC_CMD_CODE, 0xA5, int*)

#endif
