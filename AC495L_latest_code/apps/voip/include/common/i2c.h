#ifndef I2C_H_INCLUDED
#define I2C_H_INCLUDED

#endif // I2C2_H_INCLUDED

/**********************************************************************

filename: i2c.h


 * Copyright (c) by Phi Audiocom Systems Pvt. Ltd.                    *
 * This software is copyrighted by and is the sole property of        *
 * Phi Audiocom Systems Pvt. Ltd.                                     *
 *
 * AUTHOR: Ambarish K.
 *
 * Release : 31 JAN 2023



***********************************************************************/

#include "keypad_interface.h"

#define LCD_ADDRESS           0x20
#define IO_ADDRESS            0x46


#define IO_INPUT_ADDR         0X24
#define IO_OUTPUT_ADDR        0X21


#define I2C_ROW_ADDRESS       0x23	
#define I2C_COLUMN_ADDRESS    0x22

/*#define LCD_ADDRESS 0x20
#define IO_ADDRESS 0x46


#define IO_INPUT_ADDR 0X25
#define IO_OUTPUT_ADDR 0X23


#define I2C_ROW_ADDRESS 0x38	
#define I2C_COLUMN_ADDRESS 0x20*/

#define IO_DIAG_ADDRESS		  0X25

#define IO_WRITE              0
#define IO_READ               1

#define IO_PIN_ON             0
#define IO_PIN_OFF            1

#define MAX_ROW               8 // keyboards_rows
#define MAX_COL               8 //  keyboard_columns

#define ROW                   1	 // for scn_key_code[] array
#define COLUMN                0 // for scn_key_code[] array

#define ST_KEY_IDLE           0
#define ST_KEY_PRSS_CNFRM     1
#define ST_KEY_PRESS_CHK_FOR_RLS 2
#define ST_KEY_RLS_CNFRM      3

#define SCAN_COLUMN_1        0xFE
#define SCAN_COLUMN_2        0xFD
#define SCAN_COLUMN_3        0xFB
#define SCAN_COLUMN_4        0xF7
#define SCAN_COLUMN_5        0xEF
#define SCAN_COLUMN_6        0xDF
#define SCAN_COLUMN_7        0xBF
#define SCAN_COLUMN_8        0x7F

#define COLUMN_DEFAULT_CASE  0xFF

#define PRESS_DEBOUNCE_TIMECOUNT   5
#define RELEASE_DEBOUNCE_TIMECOUNT 5

extern int pin_status;
//#define ICOM_LED_ON "0xFE"
//#define CALL_THROUGH_LED_ON "0xFD"
//#define LAN_LED_ON "0xFB"

/*unsigned char ICOM_LED_ON[] = {0xFE};
unsigned char CALL_THROUGH_LED_ON[] = {0xFD};
unsigned char LAN_LED_ON[] = {0xFB};

unsigned char ICOM_LED_OFF[] = {0x01};
unsigned char CALL_THROUGH_LED_OFF[] = {0x02};
unsigned char LAN_LED_OFF[] = {0x04};*/

static keypad_keys digitToKeyArr[EIGHT][EIGHT] =
	{
		{D1_KEY, D2_KEY, D3_KEY, D4_KEY, D5_KEY, D6_KEY, VOL_DN_KEY, VOL_UP_KEY},
		{BLANK, RESET_KEY, BLANK, SRE_MUTE_KEY, DIGIT_0, PTT_KEY, BLANK, BLANK},
		{PRIO_KEY, BLANK, I_COM_KEY, DIGIT_7, DIGIT_8, DIGIT_9, D26_KEY, D25_KEY},
		{AIM_KEY, ENTER_KEY, EPBX_KEY, DIGIT_4, DIGIT_5, DIGIT_6, D19_KEY, D18_KEY},
		{BLANK, ZONE_KEY, CONF_KEY, DIGIT_1, DIGIT_2, DIGIT_3, D12_KEY, D11_KEY},
		{BLANK, D30_KEY, D29_KEY, BLANK, BLANK, BLANK, D28_KEY, BLANK},
		{D17_KEY, D16_KEY, D15_KEY, D21_KEY, D22_KEY, D23_KEY, D14_KEY, D24_KEY},
		{D13_KEY, D20_KEY, D27_KEY, D7_KEY, D8_KEY, D9_KEY, BLANK, D10_KEY}};

static char* keysInString[EIGHT][EIGHT] =
	{
		{"D1_KEY", "D2_KEY", "D3_KEY", "D4_KEY", "D5_KEY", "D6_KEY", "VOL_DN_KEY", "VOL_UP_KEY"},
		{"BLANK", "RESET_KEY", "BLANK", "SRE_MUTE_KEY", "DIGIT_0", "PTT_KEY", "BLANK", "BLANK"},
		{"PRIO_KEY", "BLANK", "I_COM_KEY", "DIGIT_7", "DIGIT_8", "DIGIT_9", "D26_KEY", "D25_KEY"},
		{"AIM_KEY", "ENTER_KEY", "EPBX_KEY", "DIGIT_4", "DIGIT_5", "DIGIT_6", "D19_KEY", "D18_KEY"},
		{"BLANK", "ZONE_KEY", "CONF_KEY", "DIGIT_1", "DIGIT_2", "DIGIT_3", "D12_KEY", "D11_KEY"},
		{"BLANK", "D30_KEY", "D29_KEY", "BLANK", "BLANK", "BLANK", "D28_KEY", "BLANK"},
		{"D17_KEY", "D16_KEY", "D15_KEY", "D21_KEY", "D22_KEY", "D23_KEY", "D14_KEY", "D24_KEY"},
		{"D13_KEY", "D20_KEY", "D27_KEY", "D7_KEY", "D8_KEY", "D9_KEY", "BLANK", "D10_KEY"}};

void scan_keyboard(int g_uiFsu_state);

void open_lcd_io();

void io_initialization();

void lcd_initialization();

char read_column(void);

char extract_column_number(unsigned char column_status);

void check_current_row(unsigned char row_no);


void clear_keyboard_status(void);

void initialize_rows();

void keyboard_scan_in_isr(int g_uiFsu_state);

void write_to_io(unsigned char pin_number, int pin_operation);

void write_to_diag_io(unsigned char pin_number, int pin_operation);

void gpio_lan_led();

int lcd_io_Fd;

char c_Fsu_state;

int read_from_io( unsigned int pin_number);

// void update_lcd_msg(struct ioctl_lcd_mesg msg);
// extern void print_i2c();

extern unsigned char keyboard_state, current_row, current_column, key_pressed_event, key_pressed_flag, key_released_event, press_and_hold_event, keyboard_detected_key;
extern unsigned int debounce_cntr;

void checkI2C();
