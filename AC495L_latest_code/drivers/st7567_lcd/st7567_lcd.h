/*******************************************************************************
 * FILE NAME:       405hd_lcd.h
 *
 * DESCRIPTION:     405hd_lcd functions
 *
 * (C) Copyright 2010, AudioCodes, Inc
 ******************************************************************************/
#ifndef __ST7567_LCD_H__
#define __ST7567_LCD_H__
#include <ssp_drv.h>
#include <ssp_hal.h>
//#include "autoconf.h" /*$(BUILD_DIR)/include/generated*/

#define OUTPUT	0
#define INPUT	1

#define HIGH		1
#define LOW		0

#define LCD_A0		28
#define LCD_RST		31

#define LCDFB_LCD_CONTRAST 0x1
#define LCDFB_LCD_ENABLE   0x2
#define LCDFB_LCD_DISABLE  0x3

int st7567_contral(SSP_HAL_SPI_INFO_T * fd_spi, unsigned char cmd);

int st7567_status(SSP_HAL_SPI_INFO_T * fd_spi, unsigned char *status, unsigned int len);

int st7567_write_data(SSP_HAL_SPI_INFO_T * fd_spi, unsigned char *data, int len);

#endif

