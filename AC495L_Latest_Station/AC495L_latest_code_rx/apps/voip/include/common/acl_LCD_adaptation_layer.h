/**************************************************************************
 * AudioCodes improved application - Keypad adaptation layer *
 *                                                                        *
 * Structure, Enum and Constant Type Definitions                          * 	
 *                                                                        *
 * File Name:	acl_LCD_adaptation_layer.h                                  *
 *                                                                        *
 * Copyright (C) 2007 AudioCodes Ltd.                                     *
 *                                                                        *
 * Modifications :                                                        *
 *                                                                        *
 * 20/11/08 - Coding started.                                          *
 *                                                                        *
 **************************************************************************/

#ifndef ACL_LCD_ADAPTATION_LAYER_H
#define ACL_LCD_ADAPTATION_LAYER_H

#define MAX_STRING_INFO_SIZE 32
typedef struct
{
	char myLineId[MAX_STRING_INFO_SIZE];
	char myLineDescription[MAX_STRING_INFO_SIZE];
	char otherPartyId[MAX_STRING_INFO_SIZE];
	char otherPartyDescription[MAX_STRING_INFO_SIZE];
	int numOfActiveCalls;
	int callIndex;
	int isHDcall;
	int display_state;
}CallInfoData;

int LCDDisplayDatum(int display_state, void *pInfo);
int LCDDisplayVolumeStr(char *pInfo);

int LCDDisplayIdle(void *pInfo);

int LCDDisplayInomingCall(void *pInfo);

int LCDDisplayDialing(void *pInfo);

int LCDDisplayCallInProgress(void *pInfo);

int LCDDisplayHold(void *pInfo);

int LCDDisplayCallEnd(void *pInfo);

int LCDDisplayBusy(void *pInfo);

int LCDDisplayOtherPartyDisconnected(void *pInfo);

int LCDDisplayRingingParty(void *pInfo);

int LCDDisplayWrongNumber(void *pInfo);

int LCDDisplayConference(void *pInfo);
int LCDDisplayVolume(int volume,int max_volume);

#endif /*ACL_LCD_ADAPTATION_LAYER_H*/

