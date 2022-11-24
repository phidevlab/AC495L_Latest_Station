#ifndef ANALOGDRIVER
#define ANALOGDRIVER

#define MAX_USABLE_CHANNELS_PER_ANALOG_BOARD 24

#define ANALOG_ON_HOOK 0
#define ANALOG_OFF_HOOK 1

#ifndef ON
#       define ON       (1)
#endif

#ifndef OFF
#       define OFF      (0)
#endif


#define WORD8 unsigned char
#define WORD16 unsigned short
#define WORD32 unsigned long

typedef struct
{
  unsigned char hook;  
  unsigned char polarity;
  unsigned char msg_wait;
  unsigned char	ring;
  unsigned char caller_id;
  unsigned char line_connected;
} ChannelState;

enum {FXO = 0,FXS = 1, E_M = 2, FXO_FXS = 3, DAA = 4, IN_VALID = 9};
enum	{NO_MSG_WAIT = 0, MSG_WAIT = 1};
enum	{CALLER_ID_OFF = 0, CALLER_ID_ON = 1};
enum	{NO_LOGICAL_STATE_CHANGE = 0, LOGICAL_STATE_CHANGE = 1};
struct acTAnalogFXOStatus
{
	int OutOfService;				/*=1  if  Out Of Service  - 			VTR=V0 */
	int BoardTemp;					/*Board Temperature */
	int Connected;					/*=1  if Line is connected - 			VTR>VH */
	int DualUse;					/*=1  if Dual Use  - 				VL< VTR<VH */
	int PolarityStatus;				/*=1  if positive polarity, 1 if negative */
	int SeizedLine;					/*=1  if there is current in IRG */
	int RemoteDisconnect;			/*=1  if there is no current in IR */
	int HwSwVersion;				/*  Hardware & Software version of ANIC */
	int AnicInternalState;			/*  Current ANIC state (0-7)		*/	
};

struct acTAnalogReadRegisterStatus
{
	int  RegisterValue;							
};

struct acTAnalogGroundKeyDetectionStatus
{
	int  GNKP;							
};

struct acTAnalogReversalPolarityStatus
{
	int Polarity;
};

struct acTAnalogFXSStatus
{
	int		RevNum;					//Revision number
	int		ChipID1;				//Chip Identification 1
	int		ChipID2;				//Chip Identification 2
	int		ChipID3;				//Chip Identification 3
	int		Temp;					//Duslic internal Temperature measurements
	int		Hook;					//Hook state 
	int		BoardTemp;				//FXS board temperature
	int		CheckSumMSB;			//CheckSumMSB
	int		CheckSumLSB;			//CheckSumLSB
	int		MessageWaitingIndication;//MWI status
	int		Ring;					//Ring Status
	int		ReversalPolarity;		//Reversal Polarity Status
	int		FXSLineCurrentReading;
	int		FXSLineVoltageReading;
	int		FXSAnalogVoltageReading;
	int		FXSRingVoltageReading;
	int		PCMSyncFailNumber;
	int		CodecValidation;		// Bit X = 0 --> Codec exists, Bit X = 1 --> Codec error
	int		BoardAnalogVoltages;	// Bit 3 = 5V | Bit 2 = 56V | Bit 1 = -24V | Bit = -48V
									// 0 - Valid Voltage,  1 - Wrong Voltage							// 0 - Valid Voltage,  1 - Wrong Voltage
};

// ANIC structures
struct TAnicCoeffients
{
	unsigned char	group; //group of coeffients
	long			address;
	unsigned char	data;
};

struct TAnalogIFPolarityStatus
{
	int Positive;			//=1 if positive
	int Negative;			//=1 if negative
};

//enums for AnicIF.c

enum TAnalogLoopCommand
{
	 Stop_Loop=0,				//stop test
	 PcmTest=1,					//if PCM loop test was requested
	 AnicDTest=2,				//if Digital loop test was requested
	 AnicATest=3				//if Analog loop test was requested
};
struct TAnicParams
{
	unsigned char RingDetectMode;	/* (default: 0) by THR (=0) or by amp+freq (=1) */
	unsigned char RingPersistence;/* (default: 10) Ring Persistence */
	unsigned char RingDeglitch ;	/* (default: 1000) Ring deglitch time */
	unsigned char RingOffTime; 	/* (default: 0x0f)	*/
	unsigned char MeasPersistence;/* (default: 0x00ff) Measurements persistence */
	unsigned char MeteringOnTime;	/* (default :0) Metering on tone time */
	unsigned char MeteringOffTime;/* (default :0) Metering off tone time */
	unsigned char THRMeasPersistence;/* (default: 0x00ff) Measurements persistence */
	unsigned char WinkTime;		/* (default: 0) wink time */
	unsigned char FlashTime;		/* (default: 0) flash time*/
	unsigned char DTMFDialOnTime;	/* (default: 0) DTMF Dial on time */
	unsigned char DTMFDialOffTime;/* (default: 0) DTMF Dial off time */
	unsigned char FarEndDisconnectType;	/* (default: 0) by CPT/PplarityReversal/current disconnect*/
	unsigned char PCMLawSelect;
};
enum EGroundStartStates
{
	 GroundStartIdle=0,				//Idle
	 GroundStartRingGrounded=1,		//Ring connected to ground
	 GroundStartTIPGrounded=2,		//TIP connected to ground 
	 GroundStartCOSeizure=3,
	 GroundStartFlashHook=4
};

#define RING_OFF 0
#define RING_ON  1

#endif
