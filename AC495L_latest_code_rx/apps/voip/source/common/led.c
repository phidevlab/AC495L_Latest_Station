/********************************************************************
 * AudioCodes improved application leds                             *
 *                                                                  *
 * File Name:	led.c                                               *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 02/03/08 - Coding started.                                       *
 *																	*
 ********************************************************************/

//#include <misc_funcs.h>
//#include <unistd.h>
//#include <syscalls.h>
//#include <kos/kos_chardev.h>
//#include "../../../vendor/audiocodes/ac494/led/led_ioctl.h"
#include "led.h"

#define MAX_FXO_NUM	1
#define MAX_FXS_NUM	4
/*----------------------- enums and struct ------------------------- */

enum _ProductType
{
	e_INVALID_PRODUCT_TYPE = -1,
	e_1FXS,
	e_2FXS,
	e_1FXS1FXO,
	e_2FXS1FXO,
	e_4FXS,
	e_4FXS1FXO,
	e_MP202,
	e_MAX_PRODUCT_TYPE
};

enum LedsColors
{
	e_GREEN = 0,
	e_RED,
#if defined (CONFIG_AC494_LE88)
	e_COLORS_NUM_IN_LED = 1
#else
	e_COLORS_NUM_IN_LED = 2
#endif
};


/*inclued all the information needed for led configuration*/
typedef struct _led_handle_t
{
	int LedNum;
	int gpio0;
	int gpio1;
}led_handle_t;

typedef struct _code_to_str_t
{
	int code;
	char *str;
}code_to_str_t;

/*----------------------- globals and statics ----------------------- */

/* table for all of the possible leds in all of the product type*/
static const led_handle_t s_LedHandleTable[e_MAX_PRODUCT_TYPE][e_MAX_LED_NUM] = {
			/*phone0*/	 /*phone1*/	  /*phone2*/  /*phone3*/	/*fxo*/		/*status*/
/* fxs */	{{0, 15, 16},{-1,-1, -1},{-1, -1, -1},{-1, -1, -1},{-1, -1, -1},{2, 7, 8}},
/* 2fxs */	{{0, 15, 16},{1, 17, 18},{-1, -1, -1},{-1, -1, -1},{-1, -1, -1},{2, 7, 8}},
/* fxso */	{{0, 15, 16},{-1,-1, -1},{-1, -1, -1},{-1, -1, -1},{1 , 17, 18},{2, 7, 8}},
/* 2fxso*/	{{0, 9, 10},{1, 11, 12},{-1, -1, -1},{-1, -1, -1},{2 , 17, 18},{3, 7, 8}},
/* 4fxs */	{{0, 9 , 10},{1, 11, 12},{2,  13, 14},{ 3, 15, 16},{-1,-1, -1 },{4, 7, 8}},
/* 4fxso*/	{{0, 9 , 10},{1, 11, 12},{2,  13, 14},{ 3, 15, 16},{4 , 17, 18},{5, 7, 8}},
/* mp202*/	{{0, 6 , -1},{1, 7, -1},{-1, -1, -1},{ -1, -1, -1},{-1, -1, -1},{-1,-1,-1}},

};

static const int s_ProductTypeTable[MAX_FXO_NUM+1][MAX_FXS_NUM+1] = {
			/*0fxs  1fxs	  2fxs		3fxs  4fxs*/	
	/*0fxo*/{-1,  e_1FXS,	  e_2FXS,	  -1,	e_4FXS},
	/*1fxo*/{-1,  e_1FXS1FXO, e_2FXS1FXO, -1,  e_4FXS1FXO}
};
	
static const code_to_str_t s_LedCode2Str[] = {
	{e_LED_PHONE0,	"Phone0"},
	{e_LED_PHONE1,	"Phone1"},
	{e_LED_PHONE2,	"Phone2"},
	{e_LED_PHONE3,	"Phone3"},
	{e_LED_FXO,		"Line"},
	{e_LED_STATUS,	"Status"},
	{e_INVALID_LED, "Invalid led number"},

	
};

static const code_to_str_t s_StateCode2Str[] = {
	{e_LED_STATE_OFF,					"Off"},
	{e_GREEN_LED_STATE_ON,				"Green On"},
	{e_GREEN_LED_STATE_SLOW_FLASH,		"Green slow flash"},
	{e_GREEN_LED_STATE_FAST_FLASH,		"Green fast flash"},
	{e_GREEN_LED_STATE_ONESHOT_ON,		"Green one shot"},
	{e_GREEN_LED_STATE_VERY_SLOW_FLASH, "Green very slow flash"},
	{e_RED_LED_STATE_ON,				"Red on"},
	{e_RED_LED_STATE_SLOW_FLASH,		"Red slow flash"},
	{e_RED_LED_STATE_FAST_FLASH,		"Red fast flash"},
	{e_RED_LED_STATE_ONESHOT_ON,		"Red one shot"},
	{e_RED_LED_STATE_VERY_SLOW_FLASH,	"Red very slow flash"},
	{e_INVALID_STATE,					"Invalid state number"},

};

static const code_to_str_t s_ProductCode2Str[] = {
	{e_1FXS,				"1FXS"},
	{e_2FXS,				"2FXS"},
	{e_1FXS1FXO,			"1FXS1FXO"},
	{e_2FXS1FXO,			"2FXS1FXO"},
	{e_4FXS,				"4FXS"},
	{e_4FXS1FXO,			"4FXS1FXO"},
	{e_MP202,				"MP202"},
	{e_INVALID_PRODUCT_TYPE,"Invalid" }
};
	
int set_phone_led_to_state(int LedIdx, int state);
void debug_print_product_type(void);


/*----------------------- api functions ------------------------------ */


//ER06 - Allow reading the current LED state
int get_phone_led_state(int LineIdx)
{
      return 0;
}

int set_phone_led_to_state(int LedIdx, int state)
{
	return 0;
}
int set_all_led_to_state(int state)
{
	return 0;
}

int led_init(void)
{
	return 0;
}

void led_uninit(void)
{

}

void led_clear(void)
{

}

int led_set(int led_number)
{
	return 0;
}

int led_reset(int led_number)
{
	return 0;
}

