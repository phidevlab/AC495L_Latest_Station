/*
 * pcfdemo - demonstrate PCF font loading for Nano-X
 */

#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include "acl_call_mngr.h"
#if defined(NANOX)

#include <keypad.h>
#define MWINCLUDECOLORS
#include "nano-X.h"
#include "nanox_hwLcd.h"

#include "acl_LCD_adaptation_layer.h"

//#define NANOX_MESSAGE_QUEUE_DEBUG
#ifdef NANOX_MESSAGE_QUEUE_DEBUG
#define DBG_PRINT(arg1,arg2...) printf ("[%s,%s,%d] "arg1,__FILE__,__FUNCTION__,__LINE__,##arg2)
#else
#define DBG_PRINT(arg1,arg2...)
#endif

/**********************************************************************************************************************************************/
GR_FONT_ID 		clock_font = 0;
GR_WINDOW_ID 	clock_wid;
GR_GC_ID 		clock_gc;

GR_FONT_ID 		volume_font = 0;
GR_WINDOW_ID 	volume_wid;
GR_GC_ID 		volume_gc;


GR_FONT_ID 		buttons_font = 0;
GR_WINDOW_ID 	buttons_wid;
GR_GC_ID 		buttons_gc;

GR_WINDOW_ID 	system_wid;
GR_GC_ID 		system_gc;

GR_FONT_ID 		system_status_font = 0;

GR_FONT_ID 		system_lines_font = 0;

GR_IMAGE_ID incomingCallIcon;
GR_IMAGE_ID logoImage;

GR_IMAGE_ID line1_image;
GR_IMAGE_ID line2_image;

struct lcd_button buttonsList[] = {
	{"L1", LCD_BUTTON_LINE1, LCD_BUTTON_STATUS_OFF, LCD_BLINK_OFF},
	{"L2", LCD_BUTTON_LINE2, LCD_BUTTON_STATUS_OFF, LCD_BLINK_OFF},
	{"Hold", LCD_BUTTON_HOLD, LCD_BUTTON_STATUS_OFF, LCD_BLINK_OFF},
	{"Conf", LCD_BUTTON_CONF, LCD_BUTTON_STATUS_OFF, LCD_BLINK_OFF},
	{"SPK", LCD_BUTTON_SPK, LCD_BUTTON_STATUS_OFF, LCD_BLINK_OFF}
};

char *statusMsgArr[]={"Incomming Call","Dialing...","Call in progress","Disconnected","Party Reject","Waiting Call...","Conference","Enter Number:","Hold...","Mute...","Wrong Number"," Welcome IPP Demo"};


struct phoneBookEntry phoneBookList[] = {
	{"039764526", "Itzik Feiglevitch", "itzik.bmp"},
	{"4403", "Ami Lahav", "ami.bmp"},
};

struct imageStatus imagesList[] = {
	{-1, LCD_IMAGE_SIZE_SMALL, LCD_IMAGE_STATUS_ON, LCD_BLINK_OFF},
	{-1, LCD_IMAGE_SIZE_SMALL, LCD_IMAGE_STATUS_ON, LCD_BLINK_OFF}
};

// this global variable maintains the line info
struct LinesInfo gLinesInfo;
char gcVolume[16]={'V','o','l','u','m','e',':',' ','1','0','/','2','1','0'};
pthread_t updateScreenEventsTid;

/**********************************************************************************************************************************************/

static char dialing_number[64]=" ";
static int	dialNumberMode=0;

static void buildClockWindow(void);
static void buildVolumeWindow(void);
static void updateClock(void);
static void buildButtonsWindow(void);
static void updateButtons(void);
static void updateButton(LCD_BUTTON_E buttonId, LCD_BUTTON_STATUS_E buttonStatus, LCD_BLINK_MODE_E blinkMode);
static void updateButtonIndex(int index, LCD_BUTTON_STATUS_E buttonStatus, LCD_BLINK_MODE_E blinkMode);
static void updateButtonsBlink(void);
static void buildSystemWindow(void);
static void updateStatus(LCD_STATUS_E statusId);
static void updateLines(void);
static void updateLine(LCD_LINE_ID_E lineId, char *number, char *callerId, GR_IMAGE_ID imageId, LCD_IMAGE_SIZE_E imageSize, LCD_IMAGE_STATUS_E status,LCD_BLINK_MODE_E blinkMode);
static void updateLineText(LCD_LINE_ID_E lineId, char *number, char *callerId);
static void updateLineImage(LCD_LINE_ID_E lineId, GR_IMAGE_ID imageId, LCD_IMAGE_SIZE_E imageSize, LCD_IMAGE_STATUS_E status, LCD_BLINK_MODE_E blinkMode);
static void updateImageBlink(void);
static void showImage(LCD_LINE_ID_E lineId);
static void clearImage(LCD_LINE_ID_E lineId);
static void systemBGimage(GR_IMAGE_ID imageId);
static void systemClearScreen(void);

/**********************************************************************************************************************************************/
static void systemBGimage(GR_IMAGE_ID imageId)
{
		GrDrawImageToFit(system_wid, system_gc, 0+60, 0+90, SYSTEM_WIN_WIDTH-130, SYSTEM_WIN_HIGHT-140, imageId);
}

static void systemClearScreen(void)
{
		GrSetGCForeground(system_gc, SYSTEM_WIN_BG_COLOR);
		GrFillRect(system_wid, system_gc, 0, 0, SYSTEM_WIN_WIDTH, SYSTEM_WIN_HIGHT);
}

void updateDialingNumber(int digit)
{
	char newNumber[64];

	sprintf(newNumber,"%s%d",dialing_number,digit);
	strcpy(dialing_number,newNumber);
	updateLineText(STATUS_LINE1, dialing_number, " ");

}

void clearDialingNumber(void)
{
 	strcpy(dialing_number," ");
	updateLineText(STATUS_LINE1, dialing_number, " ");
	updateLineText(STATUS_LINE2, " " , " ");
}

static void buildClockWindow(void)
{
	clock_wid = GrNewWindowEx(GR_WM_PROPS_APPWINDOW, "clock", GR_ROOT_WINDOW_ID, CLOCK_WIN_X,CLOCK_WIN_Y,CLOCK_WIN_WIDTH,CLOCK_WIN_HIGHT, CLOCK_WIN_BG_COLOR);
	GrSelectEvents(clock_wid, GR_EVENT_MASK_EXPOSURE | GR_EVENT_MASK_CLOSE_REQ | GR_EVENT_MASK_KEY_UP | GR_EVENT_MASK_KEY_DOWN);
	GrMapWindow(clock_wid);
	clock_gc = GrNewGC();

	clock_font = GrCreateFont(CLOCK_WIN_FONT, 0, 0);
	if (!clock_font) {
		printf("Unable to load clock_font\r\n");
	}

	GrSetGCFont(clock_gc, clock_font);
	GrSetGCForeground(clock_gc, CLOCK_WIN_TEXT_COLOR);
	GrSetGCBackground(clock_gc, CLOCK_WIN_BG_COLOR);
}

static void buildVolumeWindow(void)
{
    return;
	volume_wid = GrNewWindowEx(GR_WM_PROPS_APPWINDOW, "volume", GR_ROOT_WINDOW_ID, VOLUME_WIN_X, VOLUME_WIN_Y, VOLUME_WIN_WIDTH, VOLUME_WIN_HIGHT, VOLUME_WIN_BG_COLOR);
	GrSelectEvents(volume_wid, GR_EVENT_MASK_EXPOSURE | GR_EVENT_MASK_CLOSE_REQ | GR_EVENT_MASK_KEY_UP | GR_EVENT_MASK_KEY_DOWN);
	GrMapWindow(volume_wid);
	volume_gc = GrNewGC();

	volume_font = GrCreateFont(CLOCK_WIN_FONT, 0, 0);
	if (!volume_font) {
		printf("Unable to load volume_font\r\n");
	}

	GrSetGCFont(volume_gc, volume_font);
	GrSetGCForeground(volume_gc, VOLUME_WIN_TEXT_COLOR);
	GrSetGCBackground(volume_gc, VOLUME_WIN_BG_COLOR);
}

static void updateClock(void)
{
	char		buf[64];
	time_t		rawtime;

	time(&rawtime);
	sprintf(buf, "%s %s", ctime(&rawtime), gcVolume);
	GrText(clock_wid, clock_gc, 0, 5, buf, strlen(buf)-1, GR_TFTOP | GR_TFASCII);
}

static void buildButtonsWindow(void)
{
	buttons_wid = GrNewWindowEx(GR_WM_PROPS_APPWINDOW, "buttons", GR_ROOT_WINDOW_ID, BUTTON_WIN_X,BUTTON_WIN_Y,BUTTON_WIN_WIDTH,BUTTON_WIN_HIGHT, BUTTON_WIN_BG_COLOR);
	GrSelectEvents(buttons_wid, GR_EVENT_MASK_EXPOSURE | GR_EVENT_MASK_CLOSE_REQ | GR_EVENT_MASK_KEY_UP | GR_EVENT_MASK_KEY_DOWN);
	GrMapWindow(buttons_wid);
	buttons_gc = GrNewGC();

	buttons_font = GrCreateFont(BUTTON_WIN_FONT, 0, 0);
	if (!buttons_font) {
		printf("Unable to load buttons_font\r\n");
	}

	GrSetGCFont(buttons_gc, buttons_font);

}

static void buildSystemWindow(void)
{
	system_wid = GrNewWindowEx(GR_WM_PROPS_APPWINDOW, "system", GR_ROOT_WINDOW_ID, SYSTEM_WIN_X,SYSTEM_WIN_Y,SYSTEM_WIN_WIDTH,SYSTEM_WIN_HIGHT, SYSTEM_WIN_BG_COLOR);
	GrSelectEvents(system_wid, GR_EVENT_MASK_EXPOSURE|GR_EVENT_MASK_CLOSE_REQ | GR_EVENT_MASK_KEY_UP | GR_EVENT_MASK_KEY_DOWN);
	GrMapWindow(system_wid);
	system_gc = GrNewGC();

	system_status_font = GrCreateFont(SYSTEM_STATUS_FONT, 0, 0);
	if (!system_status_font) {
		printf("Unable to load system_status_font\r\n");
	}

	system_lines_font = GrCreateFont(SYSTEM_LINE_FONT, 0, 0);
	if (!system_lines_font) {
		printf("Unable to load system_lines_font\r\n");
	}
}

static void updateStatus(LCD_STATUS_E statusId)
{
	//printf("statusid=%d\n", statusId);
	GrSetGCFont(system_gc, system_status_font);

	GrSetGCForeground(system_gc, SYSTEM_STATUS_BG_COLOR);
	GrFillRect(system_wid, system_gc, SYSTEM_STATUS_X,SYSTEM_STATUS_Y, SYSTEM_STATUS_WIDTH,SYSTEM_STATUS_HIGHT);
	GrSetGCBackground(system_gc, SYSTEM_STATUS_BG_COLOR);
	GrSetGCForeground(system_gc, SYSTEM_STATUS_TEXT_COLOR);
	GrText(system_wid, system_gc, SYSTEM_STATUS_X,SYSTEM_STATUS_Y, statusMsgArr[statusId], strlen(statusMsgArr[statusId]), GR_TFTOP | GR_TFASCII);

}

static void updateButtonsBlink(void)
{
	int index;

	for(index=0; index < NUMBER_OF_BUTTONS; index++) {
		if(buttonsList[index].blinkMode == LCD_BLINK_ON) {
			if(buttonsList[index].status == LCD_BUTTON_STATUS_ON)
				updateButtonIndex(index, LCD_BUTTON_STATUS_OFF, LCD_BLINK_ON);
			else
				updateButtonIndex(index, LCD_BUTTON_STATUS_ON, LCD_BLINK_ON);
		}
	}
}

static void updateButton(LCD_BUTTON_E buttonId, LCD_BUTTON_STATUS_E buttonStatus, LCD_BLINK_MODE_E blinkMode)
{

	int index;
	int wrongId;

	for(index=0, wrongId=1 ; index < NUMBER_OF_BUTTONS; index++) {
		if(buttonsList[index].id == buttonId) {
			wrongId = 0;
			break;
		}
	}
	if(wrongId)
		return;

	updateButtonIndex(index, buttonStatus, blinkMode);

}

static void updateButtons(void)
{
    int iLine = 0;
    int CurLine = gLinesInfo.callIndex;
    int AnotherLine = (0==CurLine)? 1: 0;

    voip_display_state_t line_state = gLinesInfo.lineInfo[CurLine].display_state;
    //DBG_PRINT("\r\nLine[%d] + state[%d]\r\n", CurLine, line_state)

    switch(line_state)
    {
    case VOIP_DISPLAY_IDLE:
	case VOIP_DISPLAY_ONHOOK:
        gLinesInfo.lineInfo[CurLine].connState = LINE_STATUS_IDLE;
        gLinesInfo.lineInfo[CurLine].isApplied = 0;
        gLinesInfo.isConf = 0;
        gLinesInfo.isHold = 0;
        break;
    case VOIP_DISPLAY_DIALING:
    case VOIP_DISPLAY_OFFHOOK:
    case VOIP_DISPLAY_WRONG_NUMBER:
    case VOIP_DISPLAY_RINGING_PARTY:
    case VOIP_DISPLAY_BUSY:
        gLinesInfo.lineInfo[CurLine].connState = LINE_STATUS_CONNECTTING;
        gLinesInfo.lineInfo[CurLine].isApplied = 1;
        gLinesInfo.isHold = 0;
        break;
    case VOIP_DISPLAY_INCOMING_CALL:
        gLinesInfo.lineInfo[CurLine].connState = LINE_STATUS_CONNECTTING;
        gLinesInfo.lineInfo[CurLine].isApplied = 1;
        gLinesInfo.isConf = 0;
        gLinesInfo.isHold = 0;
        break;
    case VOIP_DISPLAY_CALL_IN_PROGRESS:
        if(gLinesInfo.lineInfo[CurLine].isApplied)
        {
            gLinesInfo.lineInfo[CurLine].connState = LINE_STATUS_CONNECTED;
            gLinesInfo.activeLine = CurLine;
        }
        gLinesInfo.isHold = 0;
        break;
    case VOIP_DISPLAY_PARTY_HUNG_UP:
        gLinesInfo.lineInfo[CurLine].connState = LINE_STATUS_CONNECTTING;
        gLinesInfo.lineInfo[CurLine].isApplied = 1;
        gLinesInfo.activeLine = AnotherLine;
        gLinesInfo.isConf = 0;
        gLinesInfo.isHold = 0;
        break;
    case VOIP_DISPLAY_CALL_END:
        gLinesInfo.lineInfo[CurLine].connState = LINE_STATUS_IDLE;
        gLinesInfo.lineInfo[CurLine].isApplied = 0;
        gLinesInfo.activeLine = AnotherLine;
        gLinesInfo.isConf = 0;
        gLinesInfo.isHold = 0;
        break;
    case VOIP_DISPLAY_HOLD:
        gLinesInfo.lineInfo[CurLine].isApplied = 1;
        gLinesInfo.activeLine = AnotherLine;
        gLinesInfo.isHold = 1;
        break;
    case VOIP_DISPLAY_TRANSFER:
        break;
    case VOIP_DISPLAY_CONFERENCE:
        gLinesInfo.lineInfo[CurLine].isApplied = 1;
        gLinesInfo.activeLine = CurLine;
        gLinesInfo.isConf = 1;
        gLinesInfo.isHold = 0;
        break;
    case VOIP_DISPLAY_MUTE:
	case VOIP_DISPLAY_LINE:
	case VOIP_DISPLAY_SPK_ON:
	case VOIP_DISPLAY_SPK_OFF:
	case VOIP_DISPLAY_VOLUME:
	    break;
    default:
        printf("\r\nUnknow Line Statue [%d]\r\n", line_state);
        break;
    }

    for(; iLine < 2; ++iLine)
    {
        if(LINE_STATUS_CONNECTED == gLinesInfo.lineInfo[iLine].connState)
        {   // another line is connected
            if(iLine == gLinesInfo.activeLine)
                updateButton((LCD_BUTTON_E)iLine, LCD_BUTTON_STATUS_ON, LCD_BLINK_OFF);
            else
                updateButton((LCD_BUTTON_E)iLine, LCD_BUTTON_STATUS_ON, LCD_BLINK_ON);
        }
        else if(LINE_STATUS_CONNECTTING == gLinesInfo.lineInfo[iLine].connState)
        {   // another line is connectting
            updateButton((LCD_BUTTON_E)iLine, LCD_BUTTON_STATUS_ON, LCD_BLINK_ON);
        }
        else
        {
            updateButton((LCD_BUTTON_E)iLine, LCD_BUTTON_STATUS_OFF, LCD_BLINK_OFF);
        }
    }

    updateButton(LCD_BUTTON_HOLD, gLinesInfo.isHold, LCD_BLINK_OFF);
    updateButton(LCD_BUTTON_CONF, gLinesInfo.isConf, LCD_BLINK_OFF);
    updateButton(LCD_BUTTON_SPK, gLinesInfo.isSPK, LCD_BLINK_OFF);

}

static void updateButtonIndex(int index, LCD_BUTTON_STATUS_E buttonStatus, LCD_BLINK_MODE_E blinkMode)
{
	int x, y;
	int nameLen;
	int name_x;
	int name_y;


	buttonsList[index].status = buttonStatus;
	buttonsList[index].blinkMode = blinkMode;

	x = (index * (BUTTON_WIDTH + 2 * BUTTON_WIDTH_OFFSET)) + BUTTON_WIDTH_OFFSET;
	y = BUTTON_HIGHT_OFFSET;
	nameLen = strlen(buttonsList[index].name);
	name_x = x + (BUTTON_WIDTH / 2) - ((nameLen * 9)/2);

	name_y = y + 1;

	if(buttonStatus == LCD_BUTTON_STATUS_ON) {
		GrSetGCForeground(buttons_gc, BUTTON_SHADOW_ON_COLOR);
    	GrFillRect(buttons_wid, buttons_gc, x+BUTTON_SHADOW_OFFSET,y+BUTTON_SHADOW_OFFSET, BUTTON_WIDTH,BUTTON_HIGHT);
		GrSetGCForeground(buttons_gc, BUTTON_ON_COLOR);
		GrFillRect(buttons_wid, buttons_gc, x,y, BUTTON_WIDTH,BUTTON_HIGHT);
		GrSetGCForeground(buttons_gc, BUTTON_TEXT_ON_COLOR);
		GrSetGCBackground(buttons_gc, BUTTON_ON_COLOR);
		GrText(buttons_wid, buttons_gc, name_x, name_y, buttonsList[index].name, nameLen, GR_TFTOP | GR_TFASCII);
	}
	else {
		GrSetGCForeground(buttons_gc, BUTTON_SHADOW_OFF_COLOR);
		GrFillRect(buttons_wid, buttons_gc, x+BUTTON_SHADOW_OFFSET,y+BUTTON_SHADOW_OFFSET, BUTTON_WIDTH,BUTTON_HIGHT);
		GrSetGCForeground(buttons_gc, BUTTON_OFF_COLOR);
		GrFillRect(buttons_wid, buttons_gc, x,y, BUTTON_WIDTH,BUTTON_HIGHT);
		GrSetGCForeground(buttons_gc, BUTTON_TEXT_OFF_COLOR);
		GrSetGCBackground(buttons_gc, BUTTON_OFF_COLOR);
		GrText(buttons_wid, buttons_gc, name_x, name_y, buttonsList[index].name, nameLen, GR_TFTOP | GR_TFASCII);

	}

}

static void updateLine(LCD_LINE_ID_E lineId, char *number, char *callerId, GR_IMAGE_ID imageId, LCD_IMAGE_SIZE_E imageSize, LCD_IMAGE_STATUS_E status, LCD_BLINK_MODE_E blinkMode)
{
	updateLineText(lineId, number, callerId);
#if defined(GLCD_CPT480272)
	updateLineImage(lineId, imageId, imageSize, status, blinkMode);
	showImage(lineId);
#endif
}

static void updateLines(void)
{
    GR_IMAGE_ID lineImages[2]={line1_image, line2_image};
    int iLine = 0;
    //printf("\r\nLine1[%d]+Line2[%d]\r\n", gLinesInfo.lineInfo[0].connState, gLinesInfo.lineInfo[1].connState);

    //if(gLinesInfo.lineInfo[0].connState || gLinesInfo.lineInfo[1].connState)
    {
        for(; iLine < 2; ++iLine)
        {
            if(LINE_STATUS_CONNECTTING == gLinesInfo.lineInfo[iLine].connState)
            {
                if(gLinesInfo.lineInfo[(iLine+1)%2].connState)
          	        updateLine((LCD_LINE_ID_E)iLine, gLinesInfo.lineInfo[iLine].myNumber, gLinesInfo.lineInfo[iLine].myDesc,
          	            incomingCallIcon, LCD_IMAGE_SIZE_SMALL, LCD_IMAGE_STATUS_ON, LCD_BLINK_ON);
          	    else
          	        updateLine(STATUS_LINE1, gLinesInfo.lineInfo[iLine].myNumber, gLinesInfo.lineInfo[iLine].myDesc,
          	            incomingCallIcon, LCD_IMAGE_SIZE_BIG, LCD_IMAGE_STATUS_ON, LCD_BLINK_ON);
            }
            else if(LINE_STATUS_CONNECTED == gLinesInfo.lineInfo[iLine].connState)
            {
                if(gLinesInfo.lineInfo[(iLine+1)%2].connState)
          	        updateLine((LCD_LINE_ID_E)iLine, gLinesInfo.lineInfo[iLine].myNumber, gLinesInfo.lineInfo[iLine].myDesc,
          	            lineImages[iLine], LCD_IMAGE_SIZE_SMALL, LCD_IMAGE_STATUS_ON, LCD_BLINK_OFF);
          	    else
          	        updateLine(STATUS_LINE1, gLinesInfo.lineInfo[iLine].myNumber, gLinesInfo.lineInfo[iLine].myDesc,
          	            lineImages[iLine], LCD_IMAGE_SIZE_BIG, LCD_IMAGE_STATUS_ON, LCD_BLINK_OFF);
            }
            else
            {
                if(gLinesInfo.lineInfo[(iLine+1)%2].connState)
          	        updateLine(STATUS_LINE2, " ", " ", -1, 0, 0, 0);
          	    else
          	        updateLine(STATUS_LINE1, " ", " ", -1, 0, 0, 0);

            }
        }
    }
}

static void updateLineImage(LCD_LINE_ID_E lineId, GR_IMAGE_ID imageId, LCD_IMAGE_SIZE_E imageSize, LCD_IMAGE_STATUS_E status, LCD_BLINK_MODE_E blinkMode)
{
	imagesList[lineId].id = imageId;
	imagesList[lineId].size = imageSize;
	imagesList[lineId].status = status;
	imagesList[lineId].blinkMode = blinkMode;
}

static void updateImageBlink(void)
{
	int lineId;

	for(lineId = STATUS_LINE1 ; lineId <= STATUS_LINE2 ; lineId++) {

		if(imagesList[lineId].blinkMode == LCD_BLINK_ON)
		{
			if(imagesList[lineId].status == LCD_IMAGE_STATUS_ON)
				imagesList[lineId].status = LCD_IMAGE_STATUS_OFF;
			else
				imagesList[lineId].status = LCD_IMAGE_STATUS_ON;

			showImage(lineId);
		}

	}

}

static void showImage(LCD_LINE_ID_E lineId)
{
	int x,y;
	int imageSizeWidth,imageSizeHight;

	if(imagesList[lineId].id == -1)
		return;

	if(lineId == STATUS_LINE1) {
		x=SYSTEM_LINE1_IMAGE_X;
		y=SYSTEM_LINE1_IMAGE_Y;
	}
	else if(lineId == STATUS_LINE2) {
		x=SYSTEM_LINE2_IMAGE_X;
		y=SYSTEM_LINE2_IMAGE_Y;
	}

	if(imagesList[lineId].size == LCD_IMAGE_SIZE_SMALL) {
		imageSizeWidth = SYSTEM_LINE1_IMAGE_SMALL_WIDTH;
		imageSizeHight = SYSTEM_LINE1_IMAGE_SMALL_HIGHT;
	}
	else {
		imageSizeWidth = SYSTEM_LINE1_IMAGE_BIG_WIDTH;
		imageSizeHight = SYSTEM_LINE1_IMAGE_BIG_HIGHT;
	}

	if(imagesList[lineId].status == LCD_IMAGE_STATUS_ON)
		GrDrawImageToFit(system_wid, system_gc, x, y, imageSizeWidth, imageSizeHight, imagesList[lineId].id);
	else {
		GrSetGCForeground(system_gc, SYSTEM_WIN_BG_COLOR);
		GrFillRect(system_wid, system_gc, x, y, imageSizeWidth, imageSizeHight);
	}


}

static void clearImage(LCD_LINE_ID_E lineId)
{
	int x,y;
	int imageSizeWidth,imageSizeHight;

	if(imagesList[lineId].id == -1)
		return;

	if(lineId == STATUS_LINE1) {
		x=SYSTEM_LINE1_IMAGE_X;
		y=SYSTEM_LINE1_IMAGE_Y;
	}
	else if(lineId == STATUS_LINE2) {
		x=SYSTEM_LINE2_IMAGE_X;
		y=SYSTEM_LINE2_IMAGE_Y;
	}

	if(imagesList[lineId].size == LCD_IMAGE_SIZE_SMALL) {
		imageSizeWidth = SYSTEM_LINE1_IMAGE_SMALL_WIDTH;
		imageSizeHight = SYSTEM_LINE1_IMAGE_SMALL_HIGHT;
	}
	else {
		imageSizeWidth = SYSTEM_LINE1_IMAGE_BIG_WIDTH;
		imageSizeHight = SYSTEM_LINE1_IMAGE_BIG_HIGHT;
	}

	GrSetGCForeground(system_gc, SYSTEM_WIN_BG_COLOR);
	GrFillRect(system_wid, system_gc, x, y, imageSizeWidth, imageSizeHight);
}

static void updateLineText(LCD_LINE_ID_E lineId, char *number, char *callerId)
{
	int x_number;
	int y_number;
	int x_callerid;
	int y_callerid;
	int x_line;
	int y_line;

	GrSetGCFont(system_gc, system_lines_font);

	if(lineId == STATUS_LINE1) {
		x_number = SYSTEM_LINE1_NUMBER_X;
		y_number = SYSTEM_LINE1_NUMBER_Y;
		x_callerid = SYSTEM_LINE1_CALLERID_X;
		y_callerid = SYSTEM_LINE1_CALLERID_Y;
	}
	else if(lineId == STATUS_LINE2) {
		x_number = SYSTEM_LINE2_NUMBER_X;
		y_number = SYSTEM_LINE2_NUMBER_Y;
		x_callerid = SYSTEM_LINE2_CALLERID_X;
		y_callerid = SYSTEM_LINE2_CALLERID_Y;
	}

	GrSetGCBackground(system_gc, SYSTEM_WIN_BG_COLOR);
	GrSetGCForeground(system_gc, SYSTEM_WIN_BG_COLOR);
	GrFillRect(system_wid, system_gc, x_number, y_number, SYSTEM_LINE_WIDTH, SYSTEM_LINE_HIGHT+4);
	GrFillRect(system_wid, system_gc, x_callerid, y_callerid, SYSTEM_LINE_WIDTH, SYSTEM_LINE_HIGHT+4);

	GrSetGCBackground(system_gc, SYSTEM_WIN_BG_COLOR);
	GrSetGCForeground(system_gc, SYSTEM_LINE_TEXT_COLOR);
	GrText(system_wid, system_gc, x_number, y_number, number, strlen(number), GR_TFTOP | GR_TFASCII);
	GrText(system_wid, system_gc, x_callerid, y_callerid, callerId, strlen(callerId), GR_TFTOP | GR_TFASCII);
#if defined(GLCD_CPT480272)
	if(lineId == STATUS_LINE1) {
		x_line = x_callerid;
		y_line = y_callerid + SYSTEM_LINE_HIGHT+6;
		GrFillRect(system_wid, system_gc, x_line, y_line, SYSTEM_LINE_WIDTH, 1);
	}
#endif
}

int initScreen(void)
{
	if (GrOpen() == -1)
		return (-1);

	buildClockWindow();
	buildSystemWindow();
	buildVolumeWindow();
	buildButtonsWindow();

	if (!(incomingCallIcon = GrLoadImageFromFile(incomingCallIconFileName, 0)))
		fprintf(stderr, "Can't load image file incomingCallIcon\r\n");
	if (!(logoImage = GrLoadImageFromFile(logoImageFileName, 0)))
		fprintf(stderr, "Can't load image file logoImage\r\n");
	if (!(line1_image = GrLoadImageFromFile("itzik.bmp", 0)))
		fprintf(stderr, "Can't load image file line1_image\r\n");
	if (!(line2_image = GrLoadImageFromFile("ami.bmp", 0)))
		fprintf(stderr, "Can't load image file line2_image\r\n");

	nanox_idle(NULL);

	return 0;
}

void * updateScreenEvents(void* arg)
{
	GR_EVENT	event;
	voip_display_state_t    voip_event=VOIP_DISPLAY_INVALID;
    message_t   new_msg;
    int         iRet=0;
    CallInfoData *pMyCallInfoData=NULL;

    memset((void*)&new_msg, (char)0, sizeof(message_t));

    initScreen();
    printf("\r\nInitialized the LCD Screen\r\n");

    // This thread is the client, only receive
    if (nanox_messageQ_init(NUM_MSGQ) != 0)
  	    DBG_PRINT ("creating nanox message queue");

	while (1)
	{
		GrGetNextEventTimeout(&event, 500L);

		switch (event.type)
		{
			case GR_EVENT_TYPE_CLOSE_REQ:
			    printf("GR_EVENT_TYPE = GR_EVENT_TYPE_CLOSE_REQ[%d]\r\n", event.type);
				GrClose();
				return NULL;;
			case GR_EVENT_TYPE_TIMEOUT:
			    // 1. update the main GUI
				updateClock();
				updateButtonsBlink();
				updateImageBlink();

                // 2. update the VoIP event
                if( (iRet=nanox_messageQ_receive(&new_msg)) < 0)
                {
                    memset((void*)&new_msg, (char)0, sizeof(message_t));
                    new_msg.id = VOIP_DISPLAY_INVALID;    //VOIP_DISPLAY_INVALID
                    DBG_PRINT ("receive message queue[0]");
                }
                else
                {
                    pMyCallInfoData = (CallInfoData *)new_msg.data;
                #if 0
                    printf("\r\n[%d][%d]=[%s][%s][%d][%d][%d][%s][%s]\r\n",
                        new_msg.id,
                        pMyCallInfoData->display_state,
                        pMyCallInfoData->myLineId,
                        pMyCallInfoData->myLineDescription,
                        pMyCallInfoData->numOfActiveCalls,
                        pMyCallInfoData->callIndex,
                        pMyCallInfoData->isHDcall,
                        pMyCallInfoData->otherPartyId,
                        pMyCallInfoData->otherPartyDescription);
                #endif
                    DBG_PRINT ("receive message");
                }

                voip_event = (voip_display_state_t)new_msg.id;

				//if(voip_event!=-1)
					//printf("voip_event=%d\n", voip_event);
				switch(voip_event)
				{
				    case VOIP_DISPLAY_IDLE: // 0  VOIP_DISPLAY_IDLE
				        nanox_idle(pMyCallInfoData);
				        break;
				    case VOIP_DISPLAY_DIALING: // 1 VOIP_DISPLAY_DIALING
				        nanox_dialing(pMyCallInfoData);
				        break;
				    case VOIP_DISPLAY_RINGING_PARTY: // 2  VOIP_DISPLAY_RINGING_PARTY
				        nanox_ringing_party(pMyCallInfoData);
				        break;
				    case VOIP_DISPLAY_CALL_IN_PROGRESS: // 3  VOIP_DISPLAY_CALL_IN_PROGRESS
				        nanox_call_in_progress(pMyCallInfoData);
				        break;
				    case VOIP_DISPLAY_PARTY_HUNG_UP: // 4  VOIP_DISPLAY_PARTY_HUNG_UP
				        nanox_other_party_disconnected(pMyCallInfoData);
				        break;
				    case VOIP_DISPLAY_WRONG_NUMBER: // 5  VOIP_DISPLAY_WRONG_NUMBER
				        nanox_wrong_number(pMyCallInfoData);
				        break;
				    case VOIP_DISPLAY_INCOMING_CALL: // 6  VOIP_DISPLAY_INCOMING_CALL
				        nanox_incoming_call(pMyCallInfoData);
				        break;
				    case VOIP_DISPLAY_HOLD: // 7  VOIP_DISPLAY_HOLD
				        nanox_hold(pMyCallInfoData);
				        break;
				    case VOIP_DISPLAY_BUSY: // 8  VOIP_DISPLAY_BUSY
				        ;
				        break;
				    case VOIP_DISPLAY_CALL_END: // 9  VOIP_DISPLAY_CALL_END    // ON HOOK
				        nanox_callend(pMyCallInfoData);
				        break;
				    case VOIP_DISPLAY_TRANSFER: // 10 VOIP_DISPLAY_TRANSFER
				        nanox_transfer(pMyCallInfoData);
				        break;
				    case VOIP_DISPLAY_CONFERENCE: // 11 VOIP_DISPLAY_CONFERENCE
				        nanox_conf(pMyCallInfoData);
				        break;
				    case VOIP_DISPLAY_MUTE: // 12 VOIP_DISPLAY_MUTE
                    case VOIP_DISPLAY_LINE: // 13 VOIP_DISPLAY_LINE
                        break;
                    case VOIP_DISPLAY_OFFHOOK: // 14 VOIP_DISPLAY_OFFHOOK
                        nanox_dialing(pMyCallInfoData);
                        break;
                    case VOIP_DISPLAY_ONHOOK:// 15 VOIP_DISPLAY_ONHOOK
	                    break;

				    case VOIP_DISPLAY_SPK_ON:    // 16 SPK OM
				        nanox_speaker(1);
				        break;
				    case VOIP_DISPLAY_SPK_OFF:    // 17 SPK
				        nanox_speaker(0);
				        break;
				    case VOIP_DISPLAY_VOLUME:    // VOL UP
				        nanox_display_volume(new_msg.data);
				        break;
				    case 504:    // MUTE
				        nanox_mute(pMyCallInfoData);
				        break;
				    case VOIP_DISPLAY_INVALID:// VOIP_DISPLAY_INVALID
				        break;
				    default:
				        printf("VoIP Event = %d\r\n", voip_event);
				        break;
				}
				break;
			case GR_EVENT_TYPE_EXPOSURE:
			    break;
			default:
				printf("GR_EVENT_TYPE = %d\r\n", event.type);
				break;
		}

	}

    return NULL;
}


void create_nanox_threads(void)
{
	pthread_t			tid;

	struct sched_param	sched;
	pthread_attr_t		attr;

	memset(&sched, 0, sizeof(sched));

	/* create the receiver thread */
	if( (pthread_attr_init(&attr) != 0) )
	{
		//acl_log(ACL_LOG_ERROR,"pthread_attr_init");
		exit(1);
	}
	if( (pthread_attr_setschedpolicy(&attr, SCHED_RR) != 0) )
	{
		//acl_log(ACL_LOG_ERROR,"pthread_attr_setschedpolicy");
		exit(1);
	}
  	sched.sched_priority = 76;/*68;	*/
 	if( (pthread_attr_setschedparam(&attr, &sched) != 0) )
	{
		//acl_log(ACL_LOG_ERROR,"pthread_attr_setschedparam");
		exit(1);
	}
	/* create the receiver thread */
 	if( (pthread_create( &tid, &attr, updateScreenEvents, 0 ) != 0) )
	{
		//acl_log(ACL_LOG_ERROR,"pthread_create - updateScreenEvents");
		printf("\r\nERROR: pthread_create - updateScreenEvents\r\n");
		exit(1);
	}

	updateScreenEventsTid = tid;

	return;

} /* end of start() */

/**********************************************************************************************************************************************/
/********************************Event*********************************************************************************/
/**********************************************************************************************************************************************/

int  nanox_idle(CallInfoData *pMyCallInfoData)
{
	systemClearScreen();
	clearDialingNumber();
	dialNumberMode=0;
	clearImage(STATUS_LINE1);
	clearImage(STATUS_LINE2);
	updateButton(LCD_BUTTON_LINE1, LCD_BUTTON_STATUS_OFF, LCD_BLINK_OFF);
	updateButton(LCD_BUTTON_LINE2, LCD_BUTTON_STATUS_OFF, LCD_BLINK_OFF);
	updateButton(LCD_BUTTON_HOLD, LCD_BUTTON_STATUS_OFF, LCD_BLINK_OFF);
	updateButton(LCD_BUTTON_CONF, LCD_BUTTON_STATUS_OFF, LCD_BLINK_OFF);
	updateButton(LCD_BUTTON_SPK, LCD_BUTTON_STATUS_OFF, LCD_BLINK_OFF);

	systemClearScreen();

	updateStatus(STATUS_IDLE);
	systemBGimage(logoImage);
    return 0;
}


int  nanox_incoming_call(CallInfoData *pMyCallInfoData)
{
    int iLine = 0;
    if(pMyCallInfoData)
    {
        iLine = (pMyCallInfoData->callIndex > 1) ? (0) : (pMyCallInfoData->callIndex );
        gLinesInfo.callIndex = iLine;
        gLinesInfo.numOfActiveCalls = pMyCallInfoData->numOfActiveCalls;
        gLinesInfo.lineInfo[iLine].display_state = (voip_display_state_t)pMyCallInfoData->display_state;
        memcpy(gLinesInfo.lineInfo[iLine].myNumber, pMyCallInfoData->otherPartyId, MAX_STRING_INFO_SIZE);
        memcpy(gLinesInfo.lineInfo[iLine].myDesc, pMyCallInfoData->otherPartyDescription, MAX_STRING_INFO_SIZE);
    }

	systemClearScreen();
	clearDialingNumber();
	clearImage(STATUS_LINE1);
	clearImage(STATUS_LINE2);

	updateStatus(STATUS_INCOMING_CALL);

	updateButtons();
	updateLines();

	dialNumberMode=0;
    return 0;
}


int  nanox_dialing(CallInfoData *pMyCallInfoData)
{
    int iLine = 0;
    if(pMyCallInfoData)
    {
        iLine = (pMyCallInfoData->callIndex > 1) ? (0) : (pMyCallInfoData->callIndex );
        gLinesInfo.callIndex = iLine;
        gLinesInfo.numOfActiveCalls = pMyCallInfoData->numOfActiveCalls;
        gLinesInfo.lineInfo[iLine].display_state = (voip_display_state_t)pMyCallInfoData->display_state;
        memcpy(gLinesInfo.lineInfo[iLine].myNumber, pMyCallInfoData->otherPartyId, MAX_STRING_INFO_SIZE);
        memcpy(gLinesInfo.lineInfo[iLine].myDesc, pMyCallInfoData->otherPartyDescription, MAX_STRING_INFO_SIZE);
    }

	systemClearScreen();
	clearDialingNumber();
	clearImage(STATUS_LINE1);
	clearImage(STATUS_LINE2);

	updateStatus(STATUS_ENTER_NUMBER);

	updateButtons();
	updateLines();


	dialNumberMode=1;

    return 0;
}


int  nanox_call_in_progress(CallInfoData *pMyCallInfoData)
{
    int iLine = 0;
    if(pMyCallInfoData)
    {
        iLine = (pMyCallInfoData->callIndex > 1) ? (0) : (pMyCallInfoData->callIndex );
        gLinesInfo.callIndex = iLine;
        gLinesInfo.numOfActiveCalls = pMyCallInfoData->numOfActiveCalls;
        gLinesInfo.lineInfo[iLine].display_state = (voip_display_state_t)pMyCallInfoData->display_state;
        memcpy(gLinesInfo.lineInfo[iLine].myNumber, pMyCallInfoData->otherPartyId, MAX_STRING_INFO_SIZE);
        memcpy(gLinesInfo.lineInfo[iLine].myDesc, pMyCallInfoData->otherPartyDescription, MAX_STRING_INFO_SIZE);
    }

	systemClearScreen();
	clearDialingNumber();
	clearImage(STATUS_LINE1);
	clearImage(STATUS_LINE2);

	updateStatus(STATUS_CALL_IN_PROGRESS);

	updateButtons();
	updateLines();

	dialNumberMode=0;

    return 0;
}


int  nanox_other_party_disconnected(CallInfoData *pMyCallInfoData)
{
    int iLine = 0;
    if(pMyCallInfoData)
    {
        iLine = (pMyCallInfoData->callIndex > 1) ? (0) : (pMyCallInfoData->callIndex );
        gLinesInfo.callIndex = iLine;
        gLinesInfo.numOfActiveCalls = pMyCallInfoData->numOfActiveCalls;
        gLinesInfo.lineInfo[iLine].display_state = (voip_display_state_t)pMyCallInfoData->display_state;
        memcpy(gLinesInfo.lineInfo[iLine].myNumber, pMyCallInfoData->otherPartyId, MAX_STRING_INFO_SIZE);
        memcpy(gLinesInfo.lineInfo[iLine].myDesc, pMyCallInfoData->otherPartyDescription, MAX_STRING_INFO_SIZE);
    }

	systemClearScreen();
	clearDialingNumber();
	clearImage(STATUS_LINE1);
	clearImage(STATUS_LINE2);
    if(LINE_STATUS_CONNECTED == gLinesInfo.lineInfo[iLine].connState)
        updateStatus(STATUS_DISCONNECTED);
    else if(LINE_STATUS_CONNECTTING == gLinesInfo.lineInfo[iLine].connState)
        updateStatus(STATUS_PARTY_REJECT);

	updateButtons();
	updateLines();

	dialNumberMode=0;
    return 0;
}


int  nanox_callend(CallInfoData *pMyCallInfoData)
{
    int iLine = 0;
    if(pMyCallInfoData)
    {
        iLine = (pMyCallInfoData->callIndex > 1) ? (0) : (pMyCallInfoData->callIndex );
        gLinesInfo.callIndex = iLine;
        gLinesInfo.numOfActiveCalls = pMyCallInfoData->numOfActiveCalls;
        gLinesInfo.lineInfo[iLine].display_state = (voip_display_state_t)pMyCallInfoData->display_state;
        memset(gLinesInfo.lineInfo[iLine].myNumber, (char)0, MAX_STRING_INFO_SIZE);
        memset(gLinesInfo.lineInfo[iLine].myDesc, (char)0, MAX_STRING_INFO_SIZE);
    }

	systemClearScreen();
	clearDialingNumber();
	clearImage(STATUS_LINE1);
	clearImage(STATUS_LINE2);

	updateStatus(STATUS_IDLE);

	updateButtons();
	updateLines();

	dialNumberMode=0;
    return 0;
}


int  nanox_ringing_party(CallInfoData *pMyCallInfoData)
{
    int iLine = 0;
    if(pMyCallInfoData)
    {
        iLine = (pMyCallInfoData->callIndex > 1) ? (0) : (pMyCallInfoData->callIndex );
        gLinesInfo.callIndex = iLine;
        gLinesInfo.numOfActiveCalls = pMyCallInfoData->numOfActiveCalls;
        gLinesInfo.lineInfo[iLine].display_state = (voip_display_state_t)pMyCallInfoData->display_state;
        memcpy(gLinesInfo.lineInfo[iLine].myNumber, pMyCallInfoData->otherPartyId, MAX_STRING_INFO_SIZE);
        memcpy(gLinesInfo.lineInfo[iLine].myDesc, pMyCallInfoData->otherPartyDescription, MAX_STRING_INFO_SIZE);
    }

	systemClearScreen();
	clearImage(STATUS_LINE1);
	clearImage(STATUS_LINE2);

	updateStatus(STATUS_DIALING);

	updateButtons();
	updateLines();

    return 0;
}


int  nanox_wrong_number(CallInfoData *pMyCallInfoData)
{
    int iLine = 0;
    if(pMyCallInfoData)
    {
        iLine = (pMyCallInfoData->callIndex > 1) ? (0) : (pMyCallInfoData->callIndex );
        gLinesInfo.callIndex = iLine;
        gLinesInfo.numOfActiveCalls = pMyCallInfoData->numOfActiveCalls;
        gLinesInfo.lineInfo[iLine].display_state = (voip_display_state_t)pMyCallInfoData->display_state;
        memcpy(gLinesInfo.lineInfo[iLine].myNumber, pMyCallInfoData->otherPartyId, MAX_STRING_INFO_SIZE);
        memcpy(gLinesInfo.lineInfo[iLine].myDesc, pMyCallInfoData->otherPartyDescription, MAX_STRING_INFO_SIZE);
    }

	systemClearScreen();
	clearImage(STATUS_LINE1);
	clearImage(STATUS_LINE2);

	updateStatus(STATUS_WRONG_NUMBER);

	updateButtons();
	updateLines();

    return 0;
}

int  nanox_display_volume(char * strVolume)
{
    /**
	systemClearScreen();
	updateStatus(STATUS_DIALING);
	clearImage(STATUS_LINE1);
	clearImage(STATUS_LINE2);

	updateButton(LCD_BUTTON_LINE1, LCD_BUTTON_STATUS_ON, LCD_BLINK_OFF);
	updateButton(LCD_BUTTON_LINE2, LCD_BUTTON_STATUS_OFF, LCD_BLINK_OFF);
	updateButton(LCD_BUTTON_HOLD, LCD_BUTTON_STATUS_OFF, LCD_BLINK_OFF);
	updateButton(LCD_BUTTON_CONF, LCD_BUTTON_STATUS_OFF, LCD_BLINK_OFF);
	updateButton(LCD_BUTTON_SPK, LCD_BUTTON_STATUS_OFF, LCD_BLINK_OFF);

	updateLine(STATUS_LINE1, " ", " ", incomingCallIcon, LCD_IMAGE_SIZE_SMALL, LCD_IMAGE_STATUS_ON, LCD_BLINK_OFF);
	updateLine(STATUS_LINE2, " ", " ", -1, 0, 0, 0);
	/**/
	strcpy(gcVolume, strVolume);
	//printf("\r\nvolume:[%s]\r\n", gcVolume);
    return 0;
}

int  nanox_hold(CallInfoData *pMyCallInfoData)
{
    int iLine = 0;
    if(pMyCallInfoData)
    {
        iLine = (pMyCallInfoData->callIndex > 1) ? (0) : (pMyCallInfoData->callIndex );
        gLinesInfo.callIndex = iLine;
        gLinesInfo.isConf = 0;
        gLinesInfo.isHold = 1;
        gLinesInfo.numOfActiveCalls = pMyCallInfoData->numOfActiveCalls;
        gLinesInfo.lineInfo[iLine].display_state = (voip_display_state_t)pMyCallInfoData->display_state;
        memcpy(gLinesInfo.lineInfo[iLine].myNumber, pMyCallInfoData->otherPartyId, MAX_STRING_INFO_SIZE);
        memcpy(gLinesInfo.lineInfo[iLine].myDesc, pMyCallInfoData->otherPartyDescription, MAX_STRING_INFO_SIZE);
    }

	systemClearScreen();
	clearDialingNumber();
	clearImage(STATUS_LINE1);
	clearImage(STATUS_LINE2);

	updateStatus(STATUS_HOLD);

	updateButtons();
	updateLines();

    return 0;
}

int  nanox_speaker(int isOn)
{
    if(isOn)
        gLinesInfo.isSPK = 1;
    else
        gLinesInfo.isSPK = 0;

	updateButtons();

    return 0;
}

int  nanox_conf(CallInfoData *pMyCallInfoData)
{
    int iLine = 0;
    if(pMyCallInfoData)
    {
        iLine = (pMyCallInfoData->callIndex > 1) ? (0) : (pMyCallInfoData->callIndex );
        gLinesInfo.callIndex = iLine;
        gLinesInfo.numOfActiveCalls = pMyCallInfoData->numOfActiveCalls;
        gLinesInfo.isConf = 1;
        gLinesInfo.isHold = 0;
        gLinesInfo.lineInfo[iLine].display_state = (voip_display_state_t)pMyCallInfoData->display_state;
        memcpy(gLinesInfo.lineInfo[0].myNumber, pMyCallInfoData->myLineId, MAX_STRING_INFO_SIZE);
        memcpy(gLinesInfo.lineInfo[0].myDesc, pMyCallInfoData->myLineDescription, MAX_STRING_INFO_SIZE);
        memcpy(gLinesInfo.lineInfo[1].myNumber, pMyCallInfoData->otherPartyId, MAX_STRING_INFO_SIZE);
        memcpy(gLinesInfo.lineInfo[1].myDesc, pMyCallInfoData->otherPartyDescription, MAX_STRING_INFO_SIZE);
    }

	systemClearScreen();
	clearDialingNumber();
	clearImage(STATUS_LINE1);
	clearImage(STATUS_LINE2);

	updateStatus(STATUS_CONFERENCE);

	updateButtons();
	updateLines();

    return 0;
}

int  nanox_mute(CallInfoData *pMyCallInfoData)
{
    int iLine = 0;
    if(pMyCallInfoData)
    {
        iLine = (pMyCallInfoData->callIndex > 1) ? (0) : (pMyCallInfoData->callIndex );
        gLinesInfo.callIndex = iLine;
        gLinesInfo.numOfActiveCalls = pMyCallInfoData->numOfActiveCalls;
        gLinesInfo.lineInfo[iLine].display_state = (voip_display_state_t)pMyCallInfoData->display_state;
        memcpy(gLinesInfo.lineInfo[iLine].myNumber, pMyCallInfoData->otherPartyId, MAX_STRING_INFO_SIZE);
        memcpy(gLinesInfo.lineInfo[iLine].myDesc, pMyCallInfoData->otherPartyDescription, MAX_STRING_INFO_SIZE);
    }

	systemClearScreen();
	clearDialingNumber();
	clearImage(STATUS_LINE1);
	clearImage(STATUS_LINE2);

	updateStatus(STATUS_MUTE);

	updateButtons();
	updateLines();

    return 0;
}

int  nanox_transfer(CallInfoData *pMyCallInfoData)
{
    int iLine = 0;
    if(pMyCallInfoData)
    {
        iLine = (pMyCallInfoData->callIndex > 1) ? (0) : (pMyCallInfoData->callIndex );
        gLinesInfo.callIndex = iLine;
        gLinesInfo.numOfActiveCalls = pMyCallInfoData->numOfActiveCalls;
        gLinesInfo.lineInfo[iLine].display_state = (voip_display_state_t)pMyCallInfoData->display_state;
        memcpy(gLinesInfo.lineInfo[iLine].myNumber, pMyCallInfoData->otherPartyId, MAX_STRING_INFO_SIZE);
        memcpy(gLinesInfo.lineInfo[iLine].myDesc, pMyCallInfoData->otherPartyDescription, MAX_STRING_INFO_SIZE);
    }

	systemClearScreen();
	clearDialingNumber();
	clearImage(STATUS_LINE1);
	clearImage(STATUS_LINE2);

	updateStatus(STATUS_IDLE);

	updateButtons();
	updateLines();

    return 0;
}


/////////////////////////////////////////////////////////////////////////
////////////     message queue          /////////////////////////////////
/////////////////////////////////////////////////////////////////////////

static int nanoxMsgQID=-1;

static void error_print(const char* format , ...)
{
    static int error_counter=0;
    static struct timeval  last_time;
    struct timeval  current_time ;
    gettimeofday(&current_time , NULL);
    if (current_time.tv_sec > last_time.tv_sec  + 10 )
    {
        error_counter = 0;
    }
    else
    {
        error_counter++;
    }

    gettimeofday(&last_time , NULL);
    if (error_counter <= 10)
    {
        printf (format);
    }
}

int nanox_messageQ_destroy(int MsgQKey)
{
    int ret;
    ret = msgctl(MsgQKey, IPC_RMID, NULL);
    if (ret<0)
    {
        error_print("[%s,%d] Error: can`t destroy message queue\n ",__FUNCTION__,__LINE__);
        return -1;
    }
    return 0;
}


int nanox_messageQ_init(int MsgQKey)
{
    if((nanoxMsgQID = msgget(MsgQKey,IPC_CREAT )) < 0)
    {
        error_print("[%s,%d] Error: can`t create message queue reciver\n ",__FUNCTION__,__LINE__);
        return -1;
    }
    else
    {
        DBG_PRINT("message Q send had been created with fd %d",nanoxMsgQID);
        return 0;
    }
}


int nanox_messageQ_receive(message_t* msg)
{
    return msgrcv(nanoxMsgQID,(void*)msg,sizeof(message_t),0,IPC_NOWAIT ) ;
}

int nanox_messageQ_send(message_t *msg)
{
    DBG_PRINT("sending message %d \n  ",msg->id);
    msg->type=1;/*must be positive in order to prevent send failur , we don`t care about this value!*/
    if ( (msgsnd(nanoxMsgQID, (void*)msg , sizeof(message_t), 0)) <0)
    {
        error_print("[%s,%d] Error: in message send \n ",__FUNCTION__,__LINE__);
        return -1;
    }
    else
        return 0;
}

int nanox_messageQ_getMsgID()
{
    return nanoxMsgQID;
}
#endif

