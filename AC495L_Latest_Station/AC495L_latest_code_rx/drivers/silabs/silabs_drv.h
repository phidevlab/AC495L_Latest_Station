/****************************************************************************
*                                                                            
*	Company:			Audiocodes Ltd.                                                      
*                                                                            
*   Project:			VoPP SoC                                                     
*                                                                          
*   Hardware Module: 	AC494-EVM                                                  
*                                                                           
*   File Name: 			silabs_drv.h                                                  
*                                                                            
*                                                                            
******************************************************************************
*                                                                            
* 	DESCRIPTION:		definitions for the slic and daa driver		  		                                                             
*                                                                            
******************************************************************************/

#ifndef SILABS_DRV_H
#define SILABS_DRV_H

#include "acGatewayDefs.h"			/* AudioCodes Gateway Definitions */

#define SLIC_MAJOR 235
#define SLIC_NAME "FxsDrv"

#define DAA_MAJOR 236
#define DAA_NAME "DaaDrv"

typedef enum {

	SILAB_ERROR_E = -1,
	SILAB_OK_E = 0,
	
} SILAB_FXS_RESPONSE;	

#define UNUSED_ARG(_arg) if (_arg) {}
#endif
