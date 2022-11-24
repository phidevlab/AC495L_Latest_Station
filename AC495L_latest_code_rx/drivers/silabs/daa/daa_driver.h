#ifndef DAADRIVER_H
#define DAADRIVER_H

#include "AnalogDriver.h"

#if __cplusplus
extern "C" {
#endif

#define DAA_RING_START_INT			1 << 0
#define DAA_RING_STOP_INT			1 << 1
#define DAA_METERING_SIGNAL_ON_INT	1 << 2
#define DAA_METERING_SIGNAL_OFF_INT	1 << 3
#define DAA_POLARITY_INT			1 << 4
#define DAA_CURRENT_DISCONNECT_INT	1 << 5			
#define DAA_MESSAGE_WAITING_INT		1 << 6
#define DAA_TIP_GROUND_DETECT_INT	1 << 7
#define DAA_INT2					1 << 8
#define DAA_INT3					1 << 9
#define DAA_INT4					1 << 10
#define DAA_INT5					1 << 11

/*Maskregister for interrupts*/
#define E_DAA_RING			0x80
#define E_DAA_REC_OVERLOAD	0x40
#define E_DAA_FRAME_DETECT	0x20
#define E_DAA_MET			0x10
#define E_DAA_DROP_OUT		0x08
#define E_DAA_LOOP_CURRENT_OVERLAOD	0x04
#define E_DAA_TIP_GROUND	0x02
#define E_DAA_POLARITY_REVERSAL	0x01


 
// Functions Prototypes
int AnalogIFDaaGetAnalogPortType(int CID);
void DAAReset(void);//NIRP 070514
int DAAInit(struct TAnicParams *,int); 
int DAAInitPort(struct TAnicParams *,int,int); 
int IsValidDaaPort(int CID);
void SetDAACS(unsigned char ,int );
int AnalogDaaSetHook(int , int );
int AnalogGenerateFlash(int , int );
int AnalogQueryAnicStatus (int );
int GetAnalogDAAStatusEventInfo (int , acTEvAnalogIfFxoStatusInfo*);
int DaaLoopCommand (int , int );
int AnalogDaaRegisterCommand(int, unsigned char, unsigned char);
unsigned char AnalogDaaReadRegisterCommand(int, unsigned char);
WORD16 DAAHandler (int );
void InitDAATimer(void);
int BitInReg(int Bit_Number, unsigned char );
unsigned char daa_byte_read(unsigned char , int );
void daa_byte_write(unsigned char  , unsigned char,int );
void AnicSendCoeffients(struct TAnicCoeffients *, int);
int GetDAAInterruptsStatus(int );
int GetDaaChipRevision(int CID);
void write_daa_cram_ini(void);

#if __cplusplus
}
#endif

#endif  //DAADRIVER_H

