/****************************************************************************
*                                                                            
*	Company:			Audiocodes Ltd.                                                      
*                                                                            
*   Project:			VoPP SoC                                                     
*                                                                          
*   Hardware Module: 	AC494-EVM                                                  
*                                                                           
*   File Name: 			dsp_drv.h                                                  
*                                                                            
*                                                                            
******************************************************************************
*                                                                            
* 	DESCRIPTION:		definitions for the ac49xdsp driver		  		                                                             
*                                                                            
******************************************************************************/

#ifndef	__LE88_DRV_h
#define	__LE88_DRV_h

#include "slicioc.h"
		
#include "acGatewayDefs.h"			/* AudioCodes Gateway Definitions */


#define MAX_LINES	  8
#define MAX_DEVICES	  4
#define NUM_DEVICES	  CONFIG_NUM_SLIC_DEVICES
#define NUM_LINES     2
#define FXS_LINE_0    0
#define FXS_LINE_1    1
#define FXS_LINE_2    2
#define FXS_LINE_3    3

/* Flash Time [msec] */
#define FLASH_COUNTER  	700 /*msec*/

#define SLIC_MAJOR 235

#define DSP_RESET_DSBL_CTRL		0x05080000



typedef enum {

	LE88_ERROR_E = -1,
	LE88_OK_E = 0,
	
} LE88_RESPONSE;	

typedef struct {

	char 		fxsOnHook;
	char 		isHookStateChange;	/* Indicating whether the hook state has changed */
	char 		onHookCount;
	char 		hookStateArray;

	char 		fxsFlash;
	char 		flashCheck;

	char 		polarityReversal;
	char 		fxoRingStart;
	char 		fxoRingEnd;

} telephonyEventBuffer_t;


typedef struct {
	int								command;
	int								channel;
	int 								txSlot;
	int								rxSlot;
} LE88_TIME_SLOT_STCT;


#endif /***__LE88_DRV_h***/
