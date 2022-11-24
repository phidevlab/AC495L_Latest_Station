	/********************************************************************
 * AudioCodes improved application phone adaptation layer           *
 *                                                                  *
 * File Name:	acl_lcd_adaptation_layer.c                          *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 20/11/08 - Coding started.                                       *
 *																	*
 ********************************************************************/




#include <stdio.h>

#include "acl_call_mngr.h"
#include "acl_LCD_adaptation_layer.h"
#ifndef NANOX
#include "iphone_hwLcd.h"
#else
#include "nanox_hwLcd.h"
#endif
#include "string.h"

#define MAX_DISPLAY_STRING 100


static int first_time_boot = 1;


static int LCDDisplay(char *s1, char *s2)
{
	char string[MAX_DISPLAY_STRING];
	
	if ( (!s1) || (!s2)  )
	{
		printf ("[%s,%d] , ERROR - parameters can not be NULL \n ",__FUNCTION__,__LINE__ ); 
		return -1;
	}
	
	strcpy(string,s1);
	strcat(string," ");
	strcat(string,s2);
#ifndef NANOX
	lcdClear();
	lcdString(string,strlen(string) );
#else
    printf("%s\r\n", string);
#endif
	return 0;
}

int LCDDisplayIdle(void *pInfo)
{
    CallInfoData *pMyCallInfoData = (CallInfoData *)pInfo;
	return LCDDisplay("Idle", pMyCallInfoData->myLineId);
}

int LCDDisplayInomingCall(void *pInfo)
{   
    CallInfoData *pMyCallInfoData = (CallInfoData *)pInfo;
	return LCDDisplay("Incoming call", pMyCallInfoData->otherPartyId);
}

int LCDDisplayDialing(void *pInfo)
{   
    CallInfoData *pMyCallInfoData = (CallInfoData *)pInfo;
	return LCDDisplay("Dialing", pMyCallInfoData->otherPartyId);
}

int LCDDisplayCallInProgress(void *pInfo)
{   
    CallInfoData *pMyCallInfoData = (CallInfoData *)pInfo;
	return LCDDisplay("call in progress", pMyCallInfoData->otherPartyId);
}

int LCDDisplayHold(void * pInfo)
{
	CallInfoData *pMyCallInfoData = (CallInfoData *)pInfo;
	return LCDDisplay("Hold", pMyCallInfoData->otherPartyId);
}

int LCDDisplayCallEnd(void * pInfo)
{
	CallInfoData *pMyCallInfoData = (CallInfoData *)pInfo;
	return LCDDisplay("Call End", pMyCallInfoData->otherPartyId);
}

int LCDDisplayBusy(void * pInfo)
{
	CallInfoData *pMyCallInfoData = (CallInfoData *)pInfo;
	return LCDDisplay("Busy", pMyCallInfoData->otherPartyId);
}

int LCDDisplayConference(void * pInfo)
{
	CallInfoData *pMyCallInfoData = (CallInfoData *)pInfo;
	return LCDDisplay("Conf", pMyCallInfoData->otherPartyId);
}

int LCDDisplayOtherPartyDisconnected(void *pInfo)
{   
    CallInfoData *pMyCallInfoData = (CallInfoData *)pInfo;
	return LCDDisplay("Disconnected",pMyCallInfoData->otherPartyId);
}

int LCDDisplayRingingParty(void *pInfo)
{    
    CallInfoData *pMyCallInfoData = (CallInfoData *)pInfo;
	return LCDDisplay("Other party ringing",pMyCallInfoData->otherPartyId);
}

int LCDDisplayWrongNumber(void *pInfo)
{    
    CallInfoData *pMyCallInfoData = (CallInfoData *)pInfo;
	return LCDDisplay("Wrong number", pMyCallInfoData->otherPartyId);
}

int LCDDisplayVolume(int volume,int max_volume)
{

	int i;

	char volStr[MAX_DISPLAY_STRING];
	if (volume<0)
	{
		printf("[%s,%d] ERROR : Volume must be positive volume=%d \n",__FUNCTION__,__LINE__,volume);
		return -1;
	}
	if (volume>max_volume)
	{
		printf("[%s,%d]ERROR: Volume must be <= to max_volume volume=%d , max_volume=%d \n ",__FUNCTION__,__LINE__,volume,max_volume);
		return -1;
	}
	

#ifdef NANOX	
    if(volume<10)
	    sprintf(volStr,"Volume: 0%d/%d", volume, max_volume);
	else
	    sprintf(volStr,"Volume: %d/%d", volume, max_volume);
    LCDDisplayVolumeStr(volStr);
#else
	sprintf(volStr,"Volume: %d",volume);
	LCDDisplay(volStr,"");
	lcdMoveCursor((int)1, (int)0);
	
	for (i=0;i<volume;i++)
		volStr[i]='*';
	for (i=i;i<max_volume;i++)
		volStr[i]='_';
	volStr[max_volume]=0;

	lcdString((char *)&volStr,max_volume);
#endif
	return 0;
}

int LCDDisplayVolumeStr(char *pInfo)
{    
#ifdef NANOX
    if(pInfo)
    {   
        if(DATA_SIZE < strlen(pInfo))
            return 0;
        message_t message2Send;
        memset((void*)&message2Send, (char)0, sizeof(message_t));

        message2Send.id = VOIP_DISPLAY_VOLUME;
        strncpy(message2Send.data, pInfo, strlen(pInfo));
        
        nanox_messageQ_send(&message2Send);
    }            
#endif
    return 0;
}

int LCDDisplayDatum(int display_state, void *pInfo)
{
    CallInfoData *pMyCallInfoData = (CallInfoData *)pInfo;
    
#ifdef NANOX
    message_t message2Send;
    memset((void*)&message2Send, (char)0, sizeof(message_t));

    if( (0 == display_state) && (first_time_boot) )
    {
        first_time_boot = 0;
        return 0;
    }

    message2Send.id = display_state;
    memcpy(message2Send.data, pMyCallInfoData, sizeof(CallInfoData));


    nanox_messageQ_send(&message2Send);
#if 0
    printf("Idle\r\n[%s][%s][%d][%d][%d][%s][%s]\r\n",
                pMyCallInfoData->myLineId, 
                pMyCallInfoData->myLineDescription,
                pMyCallInfoData->numOfActiveCalls,
                pMyCallInfoData->callIndex,
                pMyCallInfoData->isHDcall,
                pMyCallInfoData->otherPartyId,
                pMyCallInfoData->otherPartyDescription);           
#endif
                
#endif
    return 0;
}

