/****************************************************************************
*                                                                            
*	Company:			Audiocodes Ltd.                                                      
*                                                                            
*   Project:			VoIP SoC                                                     
*                                                                          
*   Hardware Module: 	AC4xx                                                  
*                                                                           
*   File Name: 			nanox_hwLcd.h                                                 
*                                                                            
*	Creation Date:    	                                                     
*    	                                                                        
*   Last Update By:		                                                     
*                                                                            
******************************************************************************
*                                                                            
* 	DESCRIPTION:		
******************************************************************************/


#ifndef	__NANOX_HW_LCD_h
#define	__NANOX_HW_LCD_h

#if defined(GLCD_CPT480272)
#include "nanox_hwLcd_CPT480272.h"
#elif defined(LCD_ST7567)
#include "nanox_hwLcd_st7567.h"	
#else
#error "No LCD type defined"	
#endif
#endif 

