/*
 *  Copyright (C) 2004 Texas Instruments, Inc.  All rights reserved.
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 */

/*******************************************************************************
 * FILE PURPOSE:    Graphical LCD Driver and Window Manager Stack Source
 *******************************************************************************
 * FILE NAME:       lcd_regs.h
 *
 * DESCRIPTION:     Titan register defines file
 *
 * REVISION HISTORY:
 * 09-04-2004      Nakshatra     Initial creation
 *
 * (C) Copyright 2004, Texas Instruments, Inc
 ******************************************************************************/

#ifndef LCD_REGS_H
#define LCD_REGS_H
#define  MMU_BASE                            	(0xA0000000)

#define  SYS_RESET_BASE                      	(MMU_BASE+0x08611600)
#define  LCD_RESET                              (1<<24)
#define pSYS_RESET_PERIPH_CTRL_REG           	((volatile UINT32 *)(SYS_RESET_BASE+0x0000))
#define  SYS_RESET_PERIPH_CTRL_REG           	(*pSYS_RESET_PERIPH_CTRL_REG)              
#define pSYS_RESET_PIN_SEL_REG(sel)          	((volatile UINT32 *)(SYS_RESET_BASE+0x000C+((sel-1)*0x04)))  
#define  SYS_RESET_PIN_SEL_REG(sel)          	(*pSYS_RESET_PIN_SEL_REG(sel))

#define  SYS_GPIO_BASE                       	(MMU_BASE+0x08610900)
#define pSYS_GPIO_ENBL_0_REG                 	((volatile UINT32 *)(SYS_GPIO_BASE+0x0018))
#define  SYS_GPIO_ENBL_0_REG                 	(*pSYS_GPIO_ENBL_0_REG)                   
#define pSYS_GPIO_ENBL_1_REG                 	((volatile UINT32 *)(SYS_GPIO_BASE+0x001C))
#define  SYS_GPIO_ENBL_1_REG                 	(*pSYS_GPIO_ENBL_1_REG)                   
#define pSYS_GPIO_DIR_0_REG                  	((volatile UINT32 *)(SYS_GPIO_BASE+0x0010))
#define  SYS_GPIO_DIR_0_REG                  	(*pSYS_GPIO_DIR_0_REG)                    
#define pSYS_GPIO_DIR_1_REG                  	((volatile UINT32 *)(SYS_GPIO_BASE+0x0014))
#define  SYS_GPIO_DIR_1_REG                  	(*pSYS_GPIO_DIR_1_REG)                    
#define pSYS_GPIO_DATA_IN_0_REG             	((volatile UINT32 *)(SYS_GPIO_BASE+0x0000))
#define  SYS_GPIO_DATA_IN_0_REG             	(*pSYS_GPIO_DATA_IN_0_REG)               
#define pSYS_GPIO_DATA_IN_1_REG             	((volatile UINT32 *)(SYS_GPIO_BASE+0x0004))
#define  SYS_GPIO_DATA_IN_1_REG             	(*pSYS_GPIO_DATA_IN_1_REG)               
#define pSYS_GPIO_DATA_OUT_0_REG             	((volatile UINT32 *)(SYS_GPIO_BASE+0x0008))
#define  SYS_GPIO_DATA_OUT_0_REG             	(*pSYS_GPIO_DATA_OUT_0_REG)               
#define pSYS_GPIO_DATA_OUT_1_REG             	((volatile UINT32 *)(SYS_GPIO_BASE+0x000C))
#define  SYS_GPIO_DATA_OUT_1_REG             	(*pSYS_GPIO_DATA_OUT_1_REG)               

/* Clock Controller Block */
#define  SYS_CLK_BASE                        	(MMU_BASE+0x08610A00)
#define pSYS_CLK_DSBL_CTRL_REG	             	((volatile UINT32 *)(SYS_CLK_BASE+0x0000))
#define  SYS_CLK_DSBL_CTRL_REG       	      	(*pSYS_CLK_DSBL_CTRL_REG)
#define pSYS_CLK_LCD_CLK_CTRL_REG            	((volatile UINT32 *)(SYS_CLK_BASE+0x0080))
#define  SYS_CLK_LCD_CLK_CTRL_REG    	      	(*pSYS_CLK_LCD_CLK_CTRL_REG)             
	
/* LCD Block */
#define  LCD_BASE                            	(MMU_BASE+0x08612300)                   
#define pLCD_BLK_REV_REG                     	((volatile UINT32 *)(LCD_BASE+0x0000))
#define  LCD_BLK_REV_REG                     	(*pLCD_BLK_REV_REG)                  
#define pLCD_CTRL_REG                        	((volatile UINT32 *)(LCD_BASE+0x0004))
#define  LCD_CTRL_REG                        	(*pLCD_CTRL_REG)                     
#define pLCD_STAT_REG                        	((volatile UINT32 *)(LCD_BASE+0x0008))
#define  LCD_STAT_REG                        	(*pLCD_STAT_REG)                     
#define pLCD_LIDD_CTRL_REG                   	((volatile UINT32 *)(LCD_BASE+0x000C))
#define  LCD_LIDD_CTRL_REG                   	(*pLCD_LIDD_CTRL_REG)                
#define pLCD_LIDD_CS0_CONFIG_REG             	((volatile UINT32 *)(LCD_BASE+0x0010))
#define  LCD_LIDD_CS0_CONFIG_REG             	(*pLCD_LIDD_CS0_CONFIG_REG)          
#define pLCD_LIDD_CS0_ADDR_REG               	((volatile UINT32 *)(LCD_BASE+0x0014))
#define  LCD_LIDD_CS0_ADDR_REG               	(*pLCD_LIDD_CS0_ADDR_REG)            
#define pLCD_LIDD_CS0_DATA_REG               	((volatile UINT32 *)(LCD_BASE+0x0018))
#define  LCD_LIDD_CS0_DATA_REG               	(*pLCD_LIDD_CS0_DATA_REG)            
#define pLCD_LIDD_CS1_CONFIG_REG             	((volatile UINT32 *)(LCD_BASE+0x001C))
#define  LCD_LIDD_CS1_CONFIG_REG             	(*pLCD_LIDD_CS1_CONFIG_REG)          
#define pLCD_LIDD_CS1_ADDR_REG               	((volatile UINT32 *)(LCD_BASE+0x0020))
#define  LCD_LIDD_CS1_ADDR_REG               	(*pLCD_LIDD_CS1_ADDR_REG)            
#define pLCD_LIDD_CS1_DATA_REG               	((volatile UINT32 *)(LCD_BASE+0x0024))
#define  LCD_LIDD_CS1_DATA_REG               	(*pLCD_LIDD_CS1_DATA_REG)            
#define pLCD_RASTER_CTRL_REG                 	((volatile UINT32 *)(LCD_BASE+0x0028))
#define  LCD_RASTER_CTRL_REG                 	(*pLCD_RASTER_CTRL_REG)              
#define pLCD_RASTER_TIMING_0_REG             	((volatile UINT32 *)(LCD_BASE+0x002C))
#define  LCD_RASTER_TIMING_0_REG             	(*pLCD_RASTER_TIMING_0_REG)          
#define pLCD_RASTER_TIMING_1_REG             	((volatile UINT32 *)(LCD_BASE+0x0030))
#define  LCD_RASTER_TIMING_1_REG             	(*pLCD_RASTER_TIMING_1_REG)          
#define pLCD_RASTER_TIMING_2_REG             	((volatile UINT32 *)(LCD_BASE+0x0034))
#define  LCD_RASTER_TIMING_2_REG             	(*pLCD_RASTER_TIMING_2_REG)          
#define pLCD_RASTER_SUBPANEL_DISP_REG        	((volatile UINT32 *)(LCD_BASE+0x0038))
#define  LCD_RASTER_SUBPANEL_DISP_REG        	(*pLCD_RASTER_SUBPANEL_DISP_REG)     
#define pLCD_DMA_CTRL_REG                    	((volatile UINT32 *)(LCD_BASE+0x0040))
#define  LCD_DMA_CTRL_REG                    	(*pLCD_DMA_CTRL_REG)                 
#define pLCD_DMA_FRM_BUF_BASE_ADDR_0_REG     	((volatile UINT32 *)(LCD_BASE+0x0044))
#define  LCD_DMA_FRM_BUF_BASE_ADDR_0_REG     	(*pLCD_DMA_FRM_BUF_BASE_ADDR_0_REG)  
#define pLCD_DMA_FRM_BUF_CEILING_ADDR_0_REG  	((volatile UINT32 *)(LCD_BASE+0x0048))
#define  LCD_DMA_FRM_BUF_CEILING_ADDR_0_REG  	(*pLCD_DMA_FRM_BUF_CEILING_ADDR_0_REG)
#define pLCD_DMA_FRM_BUF_BASE_ADDR_1_REG     	((volatile UINT32 *)(LCD_BASE+0x004C))
#define  LCD_DMA_FRM_BUF_BASE_ADDR_1_REG     	(*pLCD_DMA_FRM_BUF_BASE_ADDR_1_REG)  
#define pLCD_DMA_FRM_BUF_CEILING_ADDR_1_REG  	((volatile UINT32 *)(LCD_BASE+0x0050))
#define  LCD_DMA_FRM_BUF_CEILING_ADDR_1_REG  	(*pLCD_DMA_FRM_BUF_CEILING_ADDR_1_REG)
		
#endif /* LCD_REGS_H */
