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


#ifndef	__NANOX_HW_LCD_ST7567_h
#define	__NANOX_HW_LCD_ST7567_h
#include "nano-X.h"
#include "acl_LCD_adaptation_layer.h"


#define COLORS_SET_1

#ifdef COLORS_SET_1

#define CLOCK_WIN_BG_COLOR 			GR_RGB(20,40,50)
#define CLOCK_WIN_TEXT_COLOR 		GR_RGB(200,200,200)

#define VOLUME_WIN_BG_COLOR 		GR_RGB(20,40,50)
#define VOLUME_WIN_TEXT_COLOR 		GR_RGB(200,200,200)

#define BUTTON_WIN_BG_COLOR 		GR_RGB(20,40,50)
#define BUTTON_ON_COLOR 			GR_RGB(200,210,255)
#define BUTTON_OFF_COLOR 			GR_RGB(70,90,110)
#define BUTTON_TEXT_ON_COLOR 		GR_RGB(0,0,0)
#define BUTTON_TEXT_OFF_COLOR 		GR_RGB(180,180,180)
#define BUTTON_SHADOW_ON_COLOR 		GR_RGB(10,10,20)
#define BUTTON_SHADOW_OFF_COLOR 	GR_RGB(50,70,90)

#define SYSTEM_WIN_BG_COLOR 		GR_RGB(0,0,0)

#define SYSTEM_STATUS_TEXT_COLOR 	GR_RGB(255,255,255)
#define SYSTEM_STATUS_BG_COLOR 		GR_RGB(0,0,0)

#define SYSTEM_LINE_TEXT_COLOR 		GR_RGB(255,255,255)

#endif



#ifdef COLORS_SET_2

#define CLOCK_WIN_BG_COLOR 			GR_RGB(50,40,20)
#define CLOCK_WIN_TEXT_COLOR 		GR_RGB(200,200,200)

#define VOLUME_WIN_BG_COLOR 		GR_RGB(50,40,20)
#define VOLUME_WIN_TEXT_COLOR 		GR_RGB(200,200,200)

#define BUTTON_WIN_BG_COLOR 		GR_RGB(50,40,20)
#define BUTTON_ON_COLOR 			GR_RGB(255,210,200)
#define BUTTON_OFF_COLOR 			GR_RGB(110,90,70)
#define BUTTON_TEXT_ON_COLOR 		GR_RGB(0,0,0)
#define BUTTON_TEXT_OFF_COLOR 		GR_RGB(180,180,180)
#define BUTTON_SHADOW_ON_COLOR 		GR_RGB(20,10,10)
#define BUTTON_SHADOW_OFF_COLOR 	GR_RGB(90,70,50)

#define SYSTEM_WIN_BG_COLOR 		GR_RGB(40,20,0)

#define SYSTEM_STATUS_TEXT_COLOR 	GR_RGB(255,255,255)
#define SYSTEM_STATUS_BG_COLOR 		GR_RGB(170,50,50)

#define SYSTEM_LINE_TEXT_COLOR 		GR_RGB(255,255,255)

#endif


#ifdef COLORS_SET_3

#define CLOCK_WIN_BG_COLOR 			GR_RGB(20,50,40)
#define CLOCK_WIN_TEXT_COLOR 		GR_RGB(0,200,200)

#define VOLUME_WIN_BG_COLOR 			GR_RGB(20,50,40)
#define VOLUME_WIN_TEXT_COLOR 		GR_RGB(0,200,200)

#define BUTTON_WIN_BG_COLOR 		GR_RGB(20,50,40)
#define BUTTON_ON_COLOR 			GR_RGB(200,255,210)
#define BUTTON_OFF_COLOR 			GR_RGB(70,110,90)
#define BUTTON_TEXT_ON_COLOR 		GR_RGB(0,0,0)
#define BUTTON_TEXT_OFF_COLOR 		GR_RGB(0,180,180)
#define BUTTON_SHADOW_ON_COLOR 		GR_RGB(10,20,10)
#define BUTTON_SHADOW_OFF_COLOR 	GR_RGB(50,90,70)

#define SYSTEM_WIN_BG_COLOR 		GR_RGB(0,40,20)

#define SYSTEM_STATUS_TEXT_COLOR 	GR_RGB(0,255,255)
#define SYSTEM_STATUS_BG_COLOR 		GR_RGB(50,170,50)

#define SYSTEM_LINE_TEXT_COLOR 		GR_RGB(0,255,255)

#endif



#ifdef COLORS_SET_4

#define CLOCK_WIN_BG_COLOR 			GR_RGB(50,50,50)
#define CLOCK_WIN_TEXT_COLOR 		GR_RGB(200,200,200)

#define VOLUME_WIN_BG_COLOR 			GR_RGB(50,50,50)
#define VOLUME_WIN_TEXT_COLOR 		GR_RGB(200,200,200)

#define BUTTON_WIN_BG_COLOR 		GR_RGB(50,50,50)
#define BUTTON_ON_COLOR 			GR_RGB(210,210,210)
#define BUTTON_OFF_COLOR 			GR_RGB(90,90,90)
#define BUTTON_TEXT_ON_COLOR 		GR_RGB(0,0,0)
#define BUTTON_TEXT_OFF_COLOR 		GR_RGB(180,180,180)
#define BUTTON_SHADOW_ON_COLOR 		GR_RGB(20,20,20)
#define BUTTON_SHADOW_OFF_COLOR 	GR_RGB(50,50,50)

#define SYSTEM_WIN_BG_COLOR 		GR_RGB(30,30,30)

#define SYSTEM_STATUS_TEXT_COLOR 	GR_RGB(255,255,255)
#define SYSTEM_STATUS_BG_COLOR 		GR_RGB(100,100,100)

#define SYSTEM_LINE_TEXT_COLOR 		GR_RGB(255,255,255)

#endif





/**********************************************************************************************************************************************/
/* Screens definitions */
/**********************************************************************************************************************************************/

#define LCD_WIDTH	132
#define LCD_HIGHT	64


/* CLOCK WINDOW */
/****************/
#define CLOCK_WIN_WIDTH			132
#define CLOCK_WIN_HIGHT			18
#define CLOCK_WIN_X				0
#define CLOCK_WIN_Y				0
//#define CLOCK_WIN_FONT			"10x20.pcf"
#define CLOCK_WIN_FONT			"6x13.pcf"



extern GR_FONT_ID 		clock_font;
extern GR_WINDOW_ID 	clock_wid;
extern GR_GC_ID 		clock_gc;


/* VOLUME WINDOW */
/****************/
#define VOLUME_WIN_WIDTH			132
#define VOLUME_WIN_HIGHT			18
#define VOLUME_WIN_X				0
#define VOLUME_WIN_Y				0
#define VOLUME_WIN_FONT			"6x13.pcf"



extern GR_FONT_ID 		volume_font;
extern GR_WINDOW_ID 	volume_wid;
extern GR_GC_ID 		volume_gc;


/* BUTTONS WINDOW */
/******************/
#define BUTTON_WIN_WIDTH			132
#define BUTTON_WIN_HIGHT			18
#define BUTTON_WIN_X				0
#define BUTTON_WIN_Y				LCD_HIGHT-BUTTON_WIN_HIGHT
#define BUTTON_WIN_FONT				"6x13.pcf"


extern GR_FONT_ID 		buttons_font;
extern GR_WINDOW_ID 	buttons_wid;
extern GR_GC_ID 		buttons_gc;


/* SYSTEM WINDOW */
/*****************/
#define SYSTEM_WIN_WIDTH		132
#define SYSTEM_WIN_HIGHT		LCD_HIGHT-CLOCK_WIN_HIGHT-BUTTON_WIN_HIGHT
#define SYSTEM_WIN_X			0
#define SYSTEM_WIN_Y			0 + CLOCK_WIN_HIGHT


extern GR_WINDOW_ID 	system_wid;
extern GR_GC_ID 		system_gc;


/* SYSTEM STATUS */
/*****************/
#define	SYSTEM_STATUS_WIDTH					112
#define	SYSTEM_STATUS_HIGHT					15
#define	SYSTEM_STATUS_X						0
#define	SYSTEM_STATUS_Y						0
#define SYSTEM_STATUS_FONT					"6x13.pcf"


extern GR_FONT_ID 		system_status_font;


/* SYSTEM LINES	 */
/*****************/
#define SYSTEM_LINE_WIDTH					70
#define SYSTEM_LINE_HIGHT					15
#define SYSTEM_LINE_X_OFFSET				10
#define SYSTEM_LINE_Y_OFFSET				10
#define SYSTEM_LINE_FONT					"6x13.pcf"
#define	SYSTEM_LINE_IMAGE_BIG_SIZE			140
#define	SYSTEM_LINE_IMAGE_SMALL_SIZE		60


extern GR_FONT_ID 		system_lines_font;

/* SYSTEM LINE 1 */
/*****************/
#define	SYSTEM_LINE1_NUMBER_WIDTH			SYSTEM_LINE_WIDTH
#define	SYSTEM_LINE1_NUMBER_HIGHT			SYSTEM_LINE_HIGHT
#define	SYSTEM_LINE1_NUMBER_X				SYSTEM_LINE_X_OFFSET
#define	SYSTEM_LINE1_NUMBER_Y				SYSTEM_STATUS_Y+SYSTEM_STATUS_HIGHT

#define	SYSTEM_LINE1_IMAGE_BIG_WIDTH		SYSTEM_LINE_IMAGE_BIG_SIZE
#define	SYSTEM_LINE1_IMAGE_BIG_HIGHT		SYSTEM_LINE_IMAGE_BIG_SIZE
#define	SYSTEM_LINE1_IMAGE_SMALL_WIDTH		SYSTEM_LINE_IMAGE_SMALL_SIZE
#define	SYSTEM_LINE1_IMAGE_SMALL_HIGHT		SYSTEM_LINE_IMAGE_SMALL_SIZE
#define	SYSTEM_LINE1_IMAGE_X				SYSTEM_LINE1_NUMBER_X+SYSTEM_LINE1_NUMBER_WIDTH
#define	SYSTEM_LINE1_IMAGE_Y				SYSTEM_LINE1_NUMBER_Y

#define	SYSTEM_LINE1_CALLERID_WIDTH			SYSTEM_LINE_WIDTH
#define	SYSTEM_LINE1_CALLERID_HIGHT			SYSTEM_LINE_HIGHT
#define	SYSTEM_LINE1_CALLERID_X				SYSTEM_LINE_X_OFFSET
#define	SYSTEM_LINE1_CALLERID_Y				SYSTEM_LINE1_NUMBER_Y+SYSTEM_LINE1_NUMBER_HIGHT+SYSTEM_LINE_Y_OFFSET

/* SYSTEM LINE 2 */
/*****************/
#define	SYSTEM_LINE2_NUMBER_WIDTH			SYSTEM_LINE_WIDTH
#define	SYSTEM_LINE2_NUMBER_HIGHT			SYSTEM_LINE_HIGHT
#define	SYSTEM_LINE2_NUMBER_X				SYSTEM_LINE_X_OFFSET
#define	SYSTEM_LINE2_NUMBER_Y				SYSTEM_LINE1_CALLERID_Y+SYSTEM_LINE1_CALLERID_HIGHT+SYSTEM_LINE_Y_OFFSET+3

#define	SYSTEM_LINE2_IMAGE_BIG_WIDTH		SYSTEM_LINE_IMAGE_BIG_SIZE
#define	SYSTEM_LINE2_IMAGE_BIG_HIGHT		SYSTEM_LINE_IMAGE_BIG_SIZE
#define	SYSTEM_LINE2_IMAGE_SMALL_WIDTH		SYSTEM_LINE_IMAGE_SMALL_SIZE
#define	SYSTEM_LINE2_IMAGE_SMALL_HIGHT		SYSTEM_LINE_IMAGE_SMALL_SIZE
#define	SYSTEM_LINE2_IMAGE_X				SYSTEM_LINE2_NUMBER_X+SYSTEM_LINE2_NUMBER_WIDTH+10
#define	SYSTEM_LINE2_IMAGE_Y				SYSTEM_LINE2_NUMBER_Y

#define	SYSTEM_LINE2_CALLERID_WIDTH			SYSTEM_LINE_WIDTH
#define	SYSTEM_LINE2_CALLERID_HIGHT			SYSTEM_LINE_HIGHT
#define	SYSTEM_LINE2_CALLERID_X				SYSTEM_LINE_X_OFFSET
#define	SYSTEM_LINE2_CALLERID_Y				SYSTEM_LINE2_NUMBER_Y+SYSTEM_LINE2_NUMBER_HIGHT+SYSTEM_LINE_Y_OFFSET


typedef enum {
	STATUS_LINE1,
	STATUS_LINE2,
	STATUS_LINE3,
	STATUS_LINE4
} LCD_LINE_ID_E;


typedef enum {
	LCD_BLINK_OFF,
	LCD_BLINK_ON
} LCD_BLINK_MODE_E;

/**********************************************************************************************************************************************/
/* Buttons definitions */
/**********************************************************************************************************************************************/

typedef enum {
	LCD_BUTTON_LINE1,
	LCD_BUTTON_LINE2,
	LCD_BUTTON_LINE3,
	LCD_BUTTON_LINE4,
	LCD_BUTTON_HOLD,
	LCD_BUTTON_CONF,
	LCD_BUTTON_SPK,
	LCD_BUTTON_MAX
} LCD_BUTTON_E;

typedef enum {
	LCD_BUTTON_STATUS_OFF,
	LCD_BUTTON_STATUS_ON
} LCD_BUTTON_STATUS_E;

struct lcd_button {
	char 					*name;
	LCD_BUTTON_E			id;
	LCD_BUTTON_STATUS_E		status;
	LCD_BLINK_MODE_E		blinkMode;			/* for blink mode */
};

//#define NUMBER_OF_BUTTONS (sizeof buttonsList / sizeof(struct lcd_button))
#define NUMBER_OF_BUTTONS (5)

#define BUTTON_WIDTH_OFFSET 		10
#define BUTTON_HIGHT_OFFSET 		5
#define BUTTON_WIDTH 				(BUTTON_WIN_WIDTH / NUMBER_OF_BUTTONS - (BUTTON_WIDTH_OFFSET * 2))
#define BUTTON_HIGHT 				(BUTTON_WIN_HIGHT - (BUTTON_HIGHT_OFFSET * 2))
#define BUTTON_SHADOW_OFFSET		2



/**********************************************************************************************************************************************/

/**********************************************************************************************************************************************/
/* Status definitions */
/**********************************************************************************************************************************************/

typedef enum {
	STATUS_INCOMING_CALL,
	STATUS_DIALING,
	STATUS_CALL_IN_PROGRESS,
	STATUS_DISCONNECTED,
	STATUS_PARTY_REJECT,
	STATUS_OTHER_LINE_RING,
	STATUS_CONFERENCE,
	STATUS_ENTER_NUMBER,
	STATUS_HOLD,
	STATUS_MUTE,
	STATUS_WRONG_NUMBER,
	STATUS_IDLE
} LCD_STATUS_E;

extern char *statusMsgArr[];

/**********************************************************************************************************************************************/


/**********************************************************************************************************************************************/
/* PhoneBook demo */
/**********************************************************************************************************************************************/


struct phoneBookEntry {
	char 					*number;
	char					*callerId;
	char					*imageId;
};

/**********************************************************************************************************************************************/

/**********************************************************************************************************************************************/
/* icons definition */
/**********************************************************************************************************************************************/

#define 	incomingCallIconFileName 	"incoming.bmp"
#define 	logoImageFileName 			"logo.bmp"


extern GR_IMAGE_ID incomingCallIcon;
extern GR_IMAGE_ID logoImage;

typedef enum {
	LCD_IMAGE_SIZE_BIG,
	LCD_IMAGE_SIZE_SMALL
} LCD_IMAGE_SIZE_E;

typedef enum {
	LCD_IMAGE_STATUS_ON,
	LCD_IMAGE_STATUS_OFF
} LCD_IMAGE_STATUS_E;

extern GR_IMAGE_ID line1_image;
extern GR_IMAGE_ID line2_image;


struct imageStatus {
	GR_IMAGE_ID				id;
	LCD_IMAGE_SIZE_E		size;
	LCD_IMAGE_STATUS_E		status;
	LCD_BLINK_MODE_E		blinkMode;
};

/**********************************************************************************************************************************************/
/* Line statue definition */
/**********************************************************************************************************************************************/

#define 	DESC_LINE_1 	"Line 1"
#define 	DESC_LINE_2 	"Line 2"
#if 0
typedef enum {
	DISPLAY_STATUS_UNKNOW = -1,
	DISPLAY_STATUS_IDLE=0,
	DISPLAY_STATUS_DIALING=1,
	DISPLAY_STATUS_RINGING_PARTY=2,
	DISPLAY_STATUS_CALL_IN_PROGRESS=3,
	DISPLAY_STATUS_PARTY_HUNG_UP=4,
	DISPLAY_STATUS_WRONG_NUMBER=5,
	DISPLAY_STATUS_INCOMING_CALL=6,
	DISPLAY_STATUS_HOLD=7,
	DISPLAY_STATUS_BUSY=8,
	DISPLAY_STATUS_CALL_END=9,
	DISPLAY_STATUS_TRANSFER=10,
	DISPLAY_STATUS_CONFERENCE=11,
	DISPLAY_STATUS_MUTE=12,
	DISPLAY_STATUS_LINE=13,
	DISPLAY_STATUS_OFFHOOK=14,
	DISPLAY_STATUS_ONHOOK=15,
	DISPLAY_STATUS_SPK_ON=16,
	DISPLAY_STATUS_SPK_OFF=17,
	DISPLAY_STATUS_VOLUME=18,
} DISPLAY_STATUS_E;
#endif

typedef enum {
	LINE_STATUS_IDLE=0,
	LINE_STATUS_CONNECTED=1,
	LINE_STATUS_CONNECTTING=2,
} LINE_STATUS_E;

struct LineInfo {
	voip_display_state_t        display_state;
	LINE_STATUS_E           connState;
	int                     isApplied;          // void flash to INPROGRESS after CALLEND
	char                    myNumber[MAX_STRING_INFO_SIZE];
	char                    myDesc[MAX_STRING_INFO_SIZE];
};

struct LinesInfo {
    int                     activeLine;
    int                     callIndex;
    int                     isSPK;
    int                     isHold;
    int                     isConf;
    int                     numOfActiveCalls;
	struct LineInfo         lineInfo[2];
};


/**********************************************************************************************************************************************/

void updateDialingNumber(int digit);
void clearDialingNumber(void);
int initScreen(void);

void* updateScreenEvents(void* arg);
void create_nanox_threads(void);


int  nanox_idle(CallInfoData *pMyCallInfoData);
int  nanox_incoming_call(CallInfoData *pMyCallInfoData);
int  nanox_dialing(CallInfoData *pMyCallInfoData);
int  nanox_call_in_progress(CallInfoData *pMyCallInfoData);
int  nanox_other_party_disconnected(CallInfoData *pMyCallInfoData);
int  nanox_callend(CallInfoData *pMyCallInfoData);
int  nanox_ringing_party(CallInfoData *pMyCallInfoData);
int  nanox_wrong_number(CallInfoData *pMyCallInfoData);
int  nanox_display_volume(char * strVolume);
int  nanox_hold(CallInfoData *pMyCallInfoData);
int  nanox_speaker(int isOn);
int  nanox_conf(CallInfoData *pMyCallInfoData);
int  nanox_mute(CallInfoData *pMyCallInfoData);
int  nanox_transfer(CallInfoData *pMyCallInfoData);


/**********************************************************************************************************************************************/

#endif /* __NANOX_HW_LCD_h*/


#ifndef IPPMESSAGEQUEUE_H
#define IPPMESSAGEQUEUE_H
#define DATA_SIZE 256
#define NUM_MSGQ 1000

typedef struct{
long 	type;/*we don`t care about this value!we always initialize it to 1 (must be positive)*/
int 		id;
char 	data[DATA_SIZE];
}message_t;

int nanox_messageQ_destroy(int MsgQKey);
int nanox_messageQ_send(message_t *msg);
int nanox_messageQ_init(int MsgQKey);
int nanox_messageQ_receive(message_t *msg);
int nanox_messageQ_getMsgID();

#endif /*IPPMESSAGEQUEUE_H*/

