/****************************************************************************
*                                                                            
*	Company:			Audiocodes Ltd.                                                      
*                                                                            
*   Project:			VoPP SoC                                                     
*                                                                          
*   Hardware Module: 	AC494-EVM                                                  
*                                                                           
*   File Name: 			silab_fxs_drv.h                                                  
*                                                                            
*                                                                            
******************************************************************************
*                                                                            
* 	DESCRIPTION:		definitions for the silab_fxs driver		  		                                                             
*                                                                            
******************************************************************************/

#ifndef	__SILAB_FXS_DRV_h
#define	__SILAB_FXS_DRV_h

#include "slicioc.h"



/* Flash Time [msec] */
#define FLASH_COUNTER  	0x64 /*msec*/


typedef enum {

	SILAB_ERROR_E = -1,
	SILAB_OK_E = 0,
	
} SILAB_FXS_RESPONSE;	


typedef enum {
	//write cmd
	SILAB_FXS_INIT_CMD=0,	
	SILAB_FXS_SET_LINE_STATE_CMD,
	SILAB_FXS_SET_TIME_SLOT_CMD,
	//read cmd	
	SILAB_FXS_GET_HOOK_STATE_CMD,
} SILAB_FXS_DRIVER_COMMAND_ENT;


/* next are the command structure wrap le88 api parameters */

typedef struct {
	int								command;	
	unsigned char					*CountryName;
} SILAB_FXS_INIT_STCT;
 
typedef struct {
	int								command;
	int 							channel;
	int								state;
	int								flashCheck;
} SILAB_FXS_HOOK_STATE_STCT;

typedef struct {
	int								command;
	int 							channel;
	int								state;
} SILAB_FXS_LINE_STATE_STCT;

typedef struct {
	int								command;
	int								channel;
	int 							txSlot;
	int								rxSlot;
} SILAB_FXS_TIME_SLOT_STCT;

#endif /***__SILAB_FXS_DRV_h***/
