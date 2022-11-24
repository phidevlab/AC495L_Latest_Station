/********************************************************************
 * AudioCodes improved application lcd                              *
 *                                                                  *
 * File Name:	voip_lcd.h                                          *
 *                                                                  *
 * Copyright (C) 2007 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 30/12/07 - Coding started.                                       *
 *																	*
 ********************************************************************/

#ifndef _VOIP_LCD_H_
#define _VOIP_LCD_H_

#include "acl_common.h"

#define LCD_LINE_LENGTH 20
#define LCD_NUMBER_OF_LINES 4

/* Open the LCD for output
 * Returns: 0 on success, -1 on failure */
int lcd_open(void);

/* Close LCD output stream */
void lcd_close(void);

/* Fills the write buffer with spaces */
void lcd_clear(void);

/* Write the given formatted text to the write buffer, starting from
 * the given coordinate. 
 * Note: The boundaries are 0-(LCD_LINE_LENGTH-1), 0-(LCD_NUMBER_OF_LINES-1)
 * Note: if message is too long to fit on screen, considering the write
 *   location, it will be truncated.
 * Returns: 0 on success, -1 if write fails for any reason */
int lcd_printf(int x, int y, char *fmt, ...);

/* Write the given text on the specified line, centered.
 * Note: The boundaries for line number are 0-(LCD_NUMBER_OF_LINES-1)
 * Note: if message is too long to fit on the line screen, it will be
 * truncated. */
void lcd_print_centered(int line, char *s);

/* This function is used only in ATA */
typedef struct {
    char number[MAX_CALLER_ID_SIZE];
    char name[MAX_CALLER_ID_SIZE];
    char addr[AC_MAX_HOST_NAME_LEN + 1];
    int distRingType;
} lcd_caller_id_data_t;
extern lcd_caller_id_data_t *caller_id;

void lcd_set_caller_id(int n, char *number, char *name, char *address, int distRingType); /* ACL NB 070624 support distRing*/

#endif /* _VOIP_LCD_H_ */

