/********************************************************************
 * AudioCodes improved application                   	            *
 *																	*
 * File Name:	acl_ui_commands.h									*
 *																	*
 * Copyright (C) 2008 AudioCodes Ltd.								*
 *																	*
 * Modifications :													*
 *																	*
 * 26/02/08 - Coding started.										*
 *																	*
 ********************************************************************/

#ifndef _PHONE_OUTPUT_H_
#define _PHONE_OUTPUT_H_

#include "acl_line_connection.h"

/* Definitions of LCD lines */
#define LCD_TITLE_LINE 0
#define LCD_CODEC_LINE 1
#define LCD_SELF_ID_LINE 2
#define LCD_STATE_LINE 2
#define LCD_PARTY_ID_LINE 3
#define LCD_ERROR_LINE 3

int phone_output_init(void);

void phone_output_uninit(void);

void phone_output_show_volume(int setting);

void phone_output_clear(void);

void phone_output_print(int c, int r, char *str);

void phone_output_print_centered(int r, char *str);

void phone_output_ind(char *indid,  int state_on);

#endif
