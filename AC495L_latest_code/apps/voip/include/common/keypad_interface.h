#ifndef _KEYPAD_INTERFACE_H
#define _KEYPAD_INTERFACE_H

#include "voip_main.h"


typedef enum
{
	/*KEYPAD_KEY_DIGIT_0,
	KEYPAD_KEY_DIGIT_1,
	KEYPAD_KEY_DIGIT_2,
	KEYPAD_KEY_DIGIT_3,
	KEYPAD_KEY_DIGIT_4,
	KEYPAD_KEY_DIGIT_5,
	KEYPAD_KEY_DIGIT_6,
	KEYPAD_KEY_DIGIT_7,
	KEYPAD_KEY_DIGIT_8,
	KEYPAD_KEY_DIGIT_9,
	KEYPAD_KEY_ASTERIKS,
	KEYPAD_KEY_POUND,
	KEYPAD_KEY_TRANSFER,
	KEYPAD_KEY_CONFERENCE,
	KEYPAD_KEY_HOLD,
	KEYPAD_KEY_LINE_1,
	KEYPAD_KEY_LINE_2,
	KEYPAD_KEY_VOLUME_UP,
	KEYPAD_KEY_VOLUME_DOWN,
	KEYPAD_KEY_MUTE,
	KEYPAD_KEY_SPEAKER,
	KEYPAD_KEY_HEADSET,
	KEYPAD_KEY_OFF_HOOK,
	KEYPAD_KEY_ON_HOOK,
	KEYPAD_KEY_PAGING_1,
	KEYPAD_KEY_PAGING_2,
	KEYPAD_KEY_PAGING_SKIP,
	KEYPAD_KEY_PAGING_SWITCH,
	KEYPAD_KEY_INVALID,*/

	DIGIT_0,
	DIGIT_1,
	DIGIT_2,
	DIGIT_3,
	DIGIT_4,
	DIGIT_5,
	DIGIT_6,
	DIGIT_7,
	DIGIT_8,
	DIGIT_9,
	D1_KEY,
	D2_KEY,
	D3_KEY,
	D4_KEY,
	D5_KEY,
	D6_KEY,
	D7_KEY,
	D8_KEY,
	D9_KEY,
	D10_KEY,
	D11_KEY,
	D12_KEY,
	D13_KEY,
	D14_KEY,
	D15_KEY,
	D16_KEY,
	D17_KEY,
	D18_KEY,
	D19_KEY,
	D20_KEY,
	D21_KEY,
	D22_KEY,
	D23_KEY,
	D24_KEY,
	D25_KEY,
	D26_KEY,
	D27_KEY,
	D28_KEY,
	D29_KEY,
	D30_KEY,
	KEYPAD_KEY_ASTERIKS,
	PAGE_KEY,
	GROUP_KEY,
	CHANNEL_KEY,
	GLOBAL_KEY,
	VOL_DN_KEY,
	VOL_UP_KEY,
	RESET_KEY,
	SRE_MUTE_KEY,
	PTT_KEY,
	PRIO_KEY,
	I_COM_KEY,
	AIM_KEY,
	ENTER_KEY,
	EPBX_KEY,
	ZONE_KEY,
	CONF_KEY,
	BLANK,
	NUM_OF_KEYPAD_KEYS

}keypad_keys;




struct sigaction action;
#define MAX_EVENTS_TO_READ       256
#define KEY_GROUP_NUMERIC        'N'
#define KEY_GROUP_DIRECT         'D'
#define KEY_GROUP_FUNCTION       'F'
#define KEY_GROUP_SPECIAL        'H'

#define EVENT_FUNCTION_1_KEY    'W'
#define EVENT_FUNCTION_2_KEY    'X'
#define EVENT_FUNCTION_3_KEY    'Y'
#define EVENT_FUNCTION_4_KEY    'Z'

#define EVENT_CHANNEL_PTP_KEY   'C'

/* key events need to send p2p state machine */
#define EVENT_KEY_UP            0 //'u'
#define EVENT_KEY_DOWN          1//'d'
#define EVENT_KEY_AUTO_REPEAT   2//'a'

#define EVENT_PAGE_REQ_KEY      'S'
#define EVENT_PTT_KEY           'P'
//#define EVENT_HOOK_ON         'R'

#define EVENT_GROUP_KEY         'G'
#define EVENT_CHANNEL_HOP_KEY   'A'
#define EVENT_ZONE_KEY          'B'
#define EVENT_GLOBAL_KEY        'E'
#define EVENT_EPABX_KEY         'I'
#define EVENT_CLEAR_KEY         'J'
#define EVENT_ZONE_ALARM_KEY    'K'
#define EVENT_GLOBAL_ALARM_KEY  'L'
#define EVENT_ALARM_KEY         'M'
#define EVENT_ZONE_CALL_KEY     'O'
#define EVENT_GLOBAL_CALL_KEY   'Q'
#define EVENT_ALARM_CALL_KEY    'T'
#define EVENT_CONFERENCE_KEY    'U'
#define EVENT_TALK_KEY          'V'
#define EVENT_EPABX_CALL_KEY    'c'

#define SYSTEM_SIZE 2

#define MAX_DIALLING_LIMIT           11
#define CLEAR                        0
#define SET                          1


/****************KEY_COLLECTION_VALIDATION_PROCESS*************/
#define IDLE_PROCESS                       1
#define P2P_KEY_DIALLING_MODE              2
#define CONF_KEY_DIALLING_MODE             3
#define GROUP_KEY_DIALLING_MODE            4
#define EPBX_KEY_DIALLING_MODE             5
#define ZONE_KEY_DIALLING_MODE             6
#define GLOBAL_KEY_DIALLING_MODE           7
#define FSU_MENU_MODE                      8
#define ICOM_KEY_DIALLING_MODE             9
#define ALM_KEY_SEQ_MODE	    		  10
#define IDLE_CONF_PROCESS				  11
#define REBOOT_MODE                       12

#define SELECT_EPBX_DIALLING_MODE          1
#define INTERNAL_DIALLING_MODE             2
#define EXTERNAL_DIALLING_MODE             3


/***********KEYBOARD_SWITCHCASE_STATES********/
 #define ST_KEY_IDLE 				 0
 #define ST_KEY_PRSS_CNFRM           1
 #define ST_KEY_PRESS_CHK_FOR_RLS    2
 #define ST_KEY_RLS_CNFRM            3


/***********KEY LIMITS***************/

#define         NO_OF_PTP_CALL_DIGITS               4
#define         P2P_DIGIT0_LOWER_VALUE             1 // 1
#define         P2P_DIGIT0_UPPER_VALUE             9 // 3
#define         MAX_STN_ID_NUMBER                  999 // MAX STN_ID in the SYTEM

#define         NO_OF_GROUP_CALL_DIGITS              3

#define         ZONE_DIGIT0_LOWER_VALUE              1
#define         ZONE_DIGIT0_HIGHER_VALUE             8
#define         NO_OF_ZONE_CALL_DIGITS                1



#define         DIGIT0                             1
#define         DIGIT1                             2
#define         DIGIT2                             3
#define 		DIGIT3							   4
#define 		DIGIT4							   5
#define 		DIGIT5							   6
#define 		DIGIT6							   7
#define 		DIGIT7							   8
#define 		DIGIT8							   9
#define 		DIGIT9							   10


int  keycode_to_directkey_no(int iKeycode);

char get_key_group_type(int iPressed_KeyCode);
char keycode_to_ascii(int iKeycode);
void clear_validation_processing();
void validate_key(keypad_keys cInkey);



void Keyboard_Handler(void);

extern int volume_up_handle(int ch);
extern int volume_down_handle(int ch);
extern int set_volume(int ch,int level);


extern int         g_iKeyEventFD;
extern int         g_valid_key_sequence;
extern int         g_iKeyboard_state;
extern int         g_ucDialed_Sequence_No;//,c_Fsu_state;
extern int 		   g_iarcDialed_Key_Array[ELEVEN];

extern char        c_Fsu_state;
extern char        g_arcLCDMsg[TWENTY];
extern char        voipapp_msg[FIFTEEN];
extern int 		   sre_mute_flag;

#endif // _KEYPAD_INTERFACE_H
