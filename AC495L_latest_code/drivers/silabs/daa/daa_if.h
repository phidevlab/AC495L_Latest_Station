/*-----------------------------------------------------------
*
*       Copyright (C) 2000 AudioCodes Ltd.
* 
*       All rights reserved. Reproduction in whole
*       or in part is prohibited without the written
*       permission of the copyright owner.
*
*------------------------------------------------------------
*------------------------------------------------------------
*
*   FILE:           daa_if.h
*   
*   DESCRIPTION:    correlates between TPApp and DAA driver.
*
*   NOTES:          None.
*
*----------------------------------------------------------*/

#ifndef DAAIF_H
#define DAAIF_H

/*#if __cplusplus
extern "C" {
#endif*/


#define POSITIVE 1
#define NEGATIVE 0

#define ON_HOOK     0
#define OFF_HOOK    1

#define FALSE       0
#define TRUE        1

//typedef unsigned long ULONG;

typedef struct acAnalogIfDaaLineParametersInfo
{
    float LineCurrent;          /* [mA]                             */
    signed char LineVoltage;    /* [V]                              */
    char HookState;             /* 0-Onhook, 1-Offhook              */
    char RingState;             /* 0-Off, 1-On                      */
    char ConnectivityState;     /* 0-Disconnected, 1-Connected      */
    char PolarityState;         /* 0-< =Normal, 1-Reversed, 2-N\A   */
    char LinePolarity;          /* 0-Positive, 1-Negative           */
    char MWIState;              /* 0-Off, 1-On                      */
    char Dummy;
    int  Dummy1;
} acAnalogIfDaaLineParametersInfo;

typedef enum TNTTCIDState
{
    NTT_CID_NOT_ACTIVATED,
    NTT_CID_GENERATE_DC_LOOP,
    NTT_CID_WAIT_FOR_CID,
    NTT_CID_GENERATE_DC_LOOP_DISCONNECT,
    NTT_CID_VALIDATE_POLARITY_REVERSAL,
    NTT_CID_WAIT_FOR_CAR,
    NTT_CID_DATA_DETECTED,
}TNTTCIDState ;


#define MAX_TIMERS 5

enum AnalogLineState
	{
	ANALOG_LINE_DISCONNECTED,
	ANALOG_LINE_CONNECTED
	};

typedef enum {NORMAL_STATE,NOISE_STATE,NOISE_EXPAIRE_STATE,WINK_STATE,POLARITY_REVERSAL_STATE} polarityType;
typedef enum 
{
	IDLE_DISCONNECT_STATE = 0,
	POSSIBLE_DISCONNECT = 1,
	DISCONNECT_DETECTION = 2,
	LINE_DISCONNECTED = 3
} CurrentDisconnectType;

typedef enum {NO_RING, RING_START, INDIAN_RING_DETECTION} IndianRingState;
	
enum LinePolartityState
{
	NORMAL,
	REVERSE
};
/*
struct AnalogOpenChannelCmd 
{
	int	CID;
	struct acTOpenChannelCmd Cmd;
};	
*/
enum CountryCoeff
{
	Argentina = 0,
	Australia = 1,
	Austria = 2, 
	Bahrain = 3, 
	Belgium = 4, 
	Brazil = 5, 
	Bulgaria = 6, 
	Canada = 7, 
	Chile = 8, 
	China = 9, 
	Colombia = 10, 
	Croatia = 11, 
	Cyprus = 12, 
	Czech_Republic = 13, 
	Denmark = 14, 
	Ecuador = 15, 
	Egypt = 16, 
	El_Salvador = 17, 
	Finland = 18, 
	France = 19, 
	Germany = 20,
	Greece = 21, 
	Guam = 22, 
	Hong_Kong = 23, 
	Hungary = 24, 
	Iceland = 25, 
	India = 26, 
	Indonesia = 27, 
	Ireland = 28, 
	Israel = 29, 
	Italy = 30, 
	Japan = 31, 
	Jordan = 32, 
	Kazakhstan = 33, 
	Kuwait = 34, 
	Latvia = 35, 
	Lebanon = 36, 
	Luxembourg = 37, 
	Macao = 38, 
	Malaysia = 39, 
	Malta = 40, 
	Mexico = 41, 
	Morocco = 42, 
	Netherlands = 43, 
	New_Zealand = 44, 
	Nigeria = 45, 
	Norway = 46, 
	Oman = 47, 
	Pakistan = 48, 
	Peru = 49, 
	Philippines = 50, 
	Poland = 51, 
	Portugal = 52, 
	Romania = 53, 
	Russia = 54, 
	Saudi_Arabia = 55, 
	Singapore = 56, 
	Slovakia = 57, 
	Slovenia = 58, 
	South_Africa = 59, 
	South_Korea = 60, 
	Spain = 61, 
	Sweden = 62, 
	Switzerland = 63, 
	Syria = 64, 
	Taiwan = 65, 
	TBR21 = 66, 
	Thailand = 67, 
	UAE = 68, 
	United_Kingdom = 69, 
	UnitedStates = 70,
	Yemen = 71
};

enum AnalogPortServiceStatus
	{
	ANALOG_PORT_OUT_OF_SERVICE,
	ANALOG_PORT_IN_SERVICE
	};


/* NIRP temp get from Yaniv......*/
//============================================
typedef struct acTEvAnalogIfFxoStatusInfo {
	int AnicInternalState;
	int HwSwVersion;
	int Connected;
	int DualUse;
	int OutOfService;
	int SeizedLine;
	int RemoteDisconnect;
	int PolarityStatus;
	int BoardTemp ;

} acTEvAnalogIfFxoStatusInfo ;

typedef struct acTEvAnalogIfReversalPolarityDetectedInfo {
	int LinePolarity;
    int LinePolarityState;
} acTEvAnalogIfReversalPolarityDetectedInfo;

typedef struct acTToneDetectionRep {
	int DetectionDirection;
    int ToneType;
} acTToneDetectionRep;



typedef struct acTEvAnalogIfReadRegisterInfo{
	int RegisterValue;
} acTEvAnalogIfReadRegisterInfo ;





/* NIRP temp get from Yaniv......END*/
//============================================


/* functions prototypes */
void AddDaaHookEvent(int , int );
int AnalogIFInitDAA (void);
int AnalogIFDaaSetHook(int , int);
int AnalogIFDaaSetFlash(int CID);
int AnalogIFQueryDaaStatus (int );
int AnalogDaaIFLoopCommand(int , int );
int AnalogIFDaaRegisterCommand(int, unsigned char, unsigned char);
unsigned char AnalogIFDaaReadRegisterCommand(int, unsigned char);
void DAAPeriodicPollingCheck(void);
int AnalogDAAIFSetPCMMode(int , unsigned char );
int AnalogIFGetDaaChipRevision(int port);
void FilterAnalogDAAEvents(int Cid,int EventID,void *EventInfo);
void AnalogIFDaaQueryOnHookPortStatus (int);
void AnalogIFDAAStatusInfo (int CID, acTEvAnalogIfFxoStatusInfo *AnalogDAAStatusInfoPtr);
void AnalogIFGetDaaLineParameters (int CID, acAnalogIfDaaLineParametersInfo *AnalogDAALineParametersInfo);
#if M1000
void ModuleOutOfServiceDAA(int);
void ModuleBackToServiceDAA(int);
#endif
unsigned char  AnalogIFRingStatus(int CID);
int AnalogIFDaaSeizeLine(int CID);
int AnalogIFDaaReleaseLine(int CID);


int PAL_sysGpioOutBit(unsigned int gpio_pin, int value);
int PAL_sysGpioInBit(unsigned int gpio_pin);

void DoDAADumpRegs(unsigned char ch);


/*#if __cplusplus
}
#endif*/

#endif /* #ifndef DAAIF_H */
