/****************************************************************************
*
*	Company:            Audiocodes Ltd.
*
*   Project:            VoPP SoC
*
*   Hardware Module:    AC494-EVM
*
*   File Name:          silab_daa_drv.h
*
*
******************************************************************************
*
*   DESCRIPTION:        definitions for the silab_daa driver                                                                 
*
******************************************************************************/

#ifndef __SILAB_DAA_DRV_h
#define __SILAB_DAA_DRV_h

#include "daaioc.h"



/* Flash Time [msec] */
#define FLASH_COUNTER   0x64 /*msec*/


typedef enum {

    SILAB_DAA_ERROR_E = -1,
    SILAB_DAA_OK_E = 0,

} SILAB_DAA_RESPONSE;


typedef enum {
    //write cmd
    SILAB_DAA_INIT_CMD=0,
  	SILAB_DAA_EXECUTE_HOOK_FLASH_CMD,
    SILAB_DAA_CHANGE_HOOK_STATE_CMD,
    //read cmd
    SILAB_DAA_DET_RING_CMD,
} SILAB_DAA_DRIVER_COMMAND_ENT;


/* next are the command structure wrap le88 api parameters */

typedef struct {
    int                             command;
} SILAB_DAA_INIT_STCT;

typedef struct 
{
	int command;
}SILAB_DAA_FLASH_STCT;

typedef struct {
    int                             command;
    int                             channel;
    int                             ringState;
} SILAB_DAA_DET_RING_STCT;

typedef struct {
    int                             command;
    int                             channel;
} SILAB_DAA_EXECUTE_HOOK_FLASH_STCT;

typedef struct {
    int                             command;
    int                             channel;
	int								hookState;
} SILAB_DAA_CHANGE_HOOK_STATE_STCT;

#endif /***__SILAB_DAA_DRV_h***/

