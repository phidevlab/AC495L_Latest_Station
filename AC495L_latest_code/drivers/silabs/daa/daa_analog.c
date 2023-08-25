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
*   FILE:           daa_if.c
*   
*   DESCRIPTION:    correlates between TPApp and DAA driver.
*
*   NOTES:          None.
*
*----------------------------------------------------------*/
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/poll.h>
#include <linux/ioctl.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/proc_fs.h>
#include <linux/devfs_fs_kernel.h>
#include <linux/param.h>
#include <linux/fs.h>

#include "../../ac49x_dsp/AC49xDrv_Definitions.h"
#include "daa_if.h"
#include "daa_driver.h"
#include "../silabs_def.h"

#define FXS_3215_DRV_DEBUG
#ifdef FXS_3215_DRV_DEBUG
#define DBG_PRINT(a1,a2...)	printk("[%s:%d] " a1, __FUNCTION__, __LINE__, ##a2)
#else
#define DBG_PRINT(a1,a2...)
#endif

#define ULONG unsigned long
/***********************************/
/*  Private variables definitions  */
/***********************************/
volatile int prev_polarity_state [NUM_DAA_LINES];			/* last stable line voltage state, uses for wink and polarity reversal detection*/
volatile int current_polarity_state [ NUM_DAA_LINES ];		/* new line voltage state after interrupt, uses for wink and polarity reversal detection */
volatile int line_polarity_state [NUM_DAA_LINES];			/* line voltage state after off hook, uses to indicate normal or reversed state */ 
polarityType  PolarityState[ NUM_DAA_LINES ];
CurrentDisconnectType CurrentDisconnectState[NUM_DAA_LINES]; 
IndianRingState IndianCalleridRingState[NUM_DAA_LINES];
int PolarityTimersId[NUM_DAA_LINES];
//kos_timer_t HookFlashTimerId[NUM_DEVICES];//NIRP 070607
int CurrentDissconnectTimersId[NUM_DAA_LINES];
int NTTTimersId[NUM_DAA_LINES],NTTCallerIDState[NUM_DAA_LINES];
int IndianCallerIdTimerId[NUM_DAA_LINES];
int LineDissconnectTimersId[NUM_DAA_LINES];
int MessageWaitingTimersId[NUM_DAA_LINES];
int MeteringTimersId[NUM_DAA_LINES];
int NTTPolarityState[NUM_DAA_LINES];
int GroundStartStatus[NUM_DAA_LINES];
int SetVoltagePolarityTimerId[NUM_DAA_LINES];
int SetGrounfStartINTTimerId[NUM_DAA_LINES];
int InServicePortStatus[NUM_DAA_LINES];
ChannelState CurrentAnalogState[NUM_DAA_LINES];
int AnalogPortType[NUM_DAA_LINES];/* NIRP temp get from Yaniv T.B.D .*/

static struct TAnicParams AnicParams;
static struct timer_list HookFlashTimerId[NUM_DAA_LINES];

/* static variables definitions */
/*------------------------------*/
//NIRP temp T.B.D - need to update CID ......
static Tac49xCallerIdStandard CallerIDStandardConfigFXO=CALLER_ID_STANDARD__TELCORDIA_BELLCORE;/* NIRP fxos integration 070206 */


unsigned short DAAHandler (int CID)
{
	
	WORD16 interrupts = 0;
	unsigned char temp, temp2, temp3, temp4, temp_not; /*temp register to contain Register 1*/
	
	
	temp  = daa_byte_read(4,CID);		/* INT source reg*/
	if(temp != 0)
	{
		temp_not = ~temp;
		daa_byte_write(4, temp_not,CID);
	}
	temp3 = daa_byte_read(44,CID);		/* INT source reg*/
	if((temp3 & 0x08) == 0x08)
		daa_byte_write(44,(temp3 & 0xf7),CID);	/* clear INT register */
	if(temp !=0)
	{
		temp2 = daa_byte_read(29,CID);		/* line voltage */
		temp4 = daa_byte_read(3,CID);		/* Mask reg*/
		if (BitInReg(7,temp))				/*check for bit 7 (ring)*/
		{
			switch(daa_byte_read(5,CID) & 0x04)
			{
			case 0x04: interrupts |= DAA_RING_START_INT;
				break;
			case 0: interrupts |= DAA_RING_STOP_INT;
				break;
			}		
			daa_byte_write(6, 0x00 , CID);										/*put power in normal operation*/
		}
        
		if ((BitInReg(0,temp)) && (BitInReg(0,temp4)))					/*check for bit 0 (POL) and not mask*/
		{
					interrupts |= DAA_POLARITY_INT;
		}
		if (BitInReg(1,temp))				/*check for bit 1 (TIP Ground)*/
		{
			interrupts |= DAA_TIP_GROUND_DETECT_INT;	
		}

	}

	if (BitInReg(3,temp3))					/*check for bit 3 (current disconnect)*/
	{
		if((daa_byte_read(5,CID)) & 0x01)	/* case of offhook*/
			interrupts |= DAA_CURRENT_DISCONNECT_INT;
		else
			interrupts |= DAA_MESSAGE_WAITING_INT;
	}
	
	return(interrupts);
}

void InitDaaArrays(int CID)
{
	unsigned char write_reg=		E_DAA_RING;
	prev_polarity_state[CID] =		POSITIVE;						/* defaults to the polarity state */
	current_polarity_state[CID] =	POSITIVE;
	line_polarity_state[CID] =		POSITIVE;
#if M1000	
	if(BoardParams.DuSLICBoardParams.GroundKeyDetection)			/* ground start is enabled */ 
	{
		write_reg |= E_DAA_TIP_GROUND;
		daa_byte_write(32, 0x01 , CID);								/* ground start control */
	}
#endif
	if(CallerIDStandardConfigFXO == CALLER_ID_STANDARD__NTT)	/* NTT caller ID supported */
        write_reg |= E_DAA_POLARITY_REVERSAL;
	daa_byte_write(3, write_reg , CID);								/* enabel INT (ring/polarity/ground start) */
	PolarityState[CID] =			NORMAL_STATE;					/* CID default state	*/ 
	CurrentDisconnectState[CID] =	IDLE_DISCONNECT_STATE;
	NTTTimersId[CID] =				MAX_TIMERS;
	NTTCallerIDState[CID] =			NTT_CID_NOT_ACTIVATED;
	NTTPolarityState[CID] =			NORMAL;
	IndianCalleridRingState[CID] =	NO_RING;
	if ((daa_byte_read(29, CID) & 0x80) == 0x80)					/* negative voltage */
	{
		prev_polarity_state[CID] =	NEGATIVE;						/* defaults to the polarity state */
		current_polarity_state[CID] = NEGATIVE;
		line_polarity_state[CID] =	NEGATIVE;
	}
	else															/* positive voltage */
	{
		prev_polarity_state[CID] =	POSITIVE;						/* defaults to the polarity state */
		current_polarity_state[CID] = POSITIVE;
		line_polarity_state[CID] =	POSITIVE;
	}
	daa_byte_write(43, 0x50, CID);									/* set threshold for positive voltage */
	daa_byte_write(44, 0x07, CID);									/* enable Voltage INT, above threshold */
	CurrentAnalogState[CID].ring =	RING_OFF;
	GroundStartStatus[CID] = GroundStartIdle;
	LineDissconnectTimersId[CID] =	MAX_TIMERS;
	MessageWaitingTimersId[CID] =	MAX_TIMERS;
	MeteringTimersId[CID] =			MAX_TIMERS;
	PolarityTimersId[CID] =			MAX_TIMERS;
	CurrentDissconnectTimersId[CID] = MAX_TIMERS;
	IndianCallerIdTimerId[CID] =	MAX_TIMERS;
	SetVoltagePolarityTimerId[CID] = MAX_TIMERS;
	SetGrounfStartINTTimerId[CID] = MAX_TIMERS;
	InServicePortStatus[CID] = ANALOG_PORT_IN_SERVICE;

       init_timer(&HookFlashTimerId[CID]);
       HookFlashTimerId[CID].expires = jiffies + (HZ/100);    
}

void InitDAATimer(void)
{
    int CID;
    for(CID=0; CID<NUM_DAA_LINES; CID++)
   {
        init_timer(&HookFlashTimerId[CID]);
       HookFlashTimerId[CID].expires = jiffies + (HZ/100);    
   }
}
void WriteDAAPortCoefficients (int CID)
{
#if 1 //NIRP temp remove
return;
#else
    unsigned char WriteReg, InternationalControl1, DCTerminationControl, ACTerminationControl, DAAControl5;
	
	if (AnalogIFDaaGetAnalogPortType(CID) == DAA)
	{
		switch(BoardParams.Ext.CountryCoefficients)
		{
		case Japan:
		case Jordan:
		case Malaysia:
		case Oman:
		case Pakistan:
		case Philippines:
		case Thailand:
			InternationalControl1 = 0x00;
			DCTerminationControl = 0x50;
			ACTerminationControl = 0x00;
			DAAControl5 = 0x00;
			break;
		case New_Zealand:
			InternationalControl1 = 0x00;
			DCTerminationControl = 0xc0;
			ACTerminationControl = 0x04;
			DAAControl5 = 0x00;
			break;
		case TBR21:
			InternationalControl1 = 0x00;
			DCTerminationControl = 0xc2;
			ACTerminationControl = 0x02;
			DAAControl5 = 0x00;
			break;
		case Austria:
		case Bahrain:
		case Belgium:
		case Croatia:
		case Cyprus:
		case Czech_Republic:
		case Denmark:
		case Egypt:
		case Finland:
		case France:
		case Germany:
		case Greece:
		case Hungary:
		case Iceland:
		case Ireland:
		case Israel:
		case Italy:
		case Latvia:
		case Lebanon:
		case Luxembourg:
		case Malta:
		case Morocco:
		case Netherlands:
		case Nigeria:
		case Norway:
		case Poland:
		case Portugal:
		case Romania:
		case Slovakia:
		case Slovenia:
		case Spain:
		case Sweden:
		case Switzerland:
			InternationalControl1 =0x00 ;
			DCTerminationControl = 0xc2;
			ACTerminationControl = 0x02;
			DAAControl5 = 0x08;
			break;
		case China:
			InternationalControl1 = 0x00;
			DCTerminationControl = 0xC0;
			ACTerminationControl = 0x0a;
			DAAControl5 = 0x00;
			break;
		case Bulgaria:
			InternationalControl1 = 0x00;
			DCTerminationControl = 0xc2;
			ACTerminationControl = 0x03;
			DAAControl5 = 0x08;
			break;
		case United_Kingdom:
			InternationalControl1 = 0x00;
			DCTerminationControl = 0xc2;
			ACTerminationControl = 0x05;
			DAAControl5 = 0x08;
			break;
		case Australia:
			InternationalControl1 = 0x40;
			DCTerminationControl = 0x50;
			ACTerminationControl = 0x03;
			DAAControl5 = 0x00;
			break;
		case South_Africa:
			InternationalControl1 = 0x02;
			DCTerminationControl = 0xc0;
			ACTerminationControl = 0x03;
			DAAControl5 = 0x00;
			break;
		case South_Korea:
			InternationalControl1 = 0x02;
			DCTerminationControl = 0xc0;
			ACTerminationControl = 0x00;
			DAAControl5 = 0x00;
			break;
		case Argentina:
		case Brazil:
		case Canada:
		case Chile:
		case Colombia:
		case Ecuador:
		case El_Salvador:
		case Guam:
		case Hong_Kong:
		case India:
		case Indonesia:
		case Kazakhstan:
		case Kuwait:
		case Macao:
		case Mexico:
		case Peru:
		case Russia:
		case Saudi_Arabia:
		case Singapore:
		case Taiwan:
		case UAE:
		case UnitedStates:
		case Yemen:
		default:
			InternationalControl1 = 0x00;
			DCTerminationControl = 0xc0;
			ACTerminationControl = 0x00;
			DAAControl5 = 0x00;
			break;
		}
		daa_byte_write(16, daa_byte_read(16,CID) | InternationalControl1 , CID);			
		daa_byte_write(26, daa_byte_read(26,CID) | DCTerminationControl , CID);
		daa_byte_write(30, daa_byte_read(30,CID) | ACTerminationControl , CID);			
		daa_byte_write(31, daa_byte_read(31,CID) | DAAControl5 , CID);
	}
#endif
}
void write_daa_coefficients (void)
{
	int CID;

	for (CID=0; CID<NUM_DAA_LINES; CID++)		
		if (AnalogIFDaaGetAnalogPortType(CID) == DAA)
			WriteDAAPortCoefficients(CID); 
}

/*-------------------------------------------------------------------
* ROUTINE: AnalogIFInitDAA
*
* DESCRIPTION:
*  Initialize DAA
*
* ARGUMENTS:
*  struct AnicCHParam *ChParam - a pointer to a structure that contains
*     all the user selected DAA parameters
*
* RESULTS: void
*
*-------------------------------------------------------------------*/
int AnalogIFInitDAA (void)   
{
    int ErrNum, CID;
    
	 AnicParams.PCMLawSelect			= 1;/*NIRP PCM Configuration register Alaw*/
        for (CID=0;CID<NUM_DAA_LINES;CID++)
	{
              CurrentAnalogState[CID].hook = ON_HOOK;  //default to onhook
		CurrentAnalogState[CID].line_connected = ANALOG_LINE_CONNECTED; //default to line connected
		CurrentAnalogState[CID].msg_wait = NO_MSG_WAIT;	//default to no MWI
		CurrentAnalogState[CID].polarity = NORMAL;
	}
        
      if ( (ErrNum = DAAInit (&AnicParams,0)) != 0) 
              DBG_PRINT("Init DAA failed (DAAdriver error number %d) ", ErrNum);

      return (ErrNum==0)?1:0;
	
}

void SetVoltagePolarity(ULONG CID, ULONG d1,ULONG d2, ULONG d3)
{
	unsigned char TipRingVoltage1;

	SetVoltagePolarityTimerId[CID] = MAX_TIMERS;
	TipRingVoltage1 = daa_byte_read(29,CID);
	if ((TipRingVoltage1 & 0x80) == 0x80)										/* check the voltage polarity state */
	{
		if(CurrentAnalogState[CID].hook == OFF_HOOK)
		{
			prev_polarity_state[CID] = NEGATIVE ;								/* negative */
			current_polarity_state[CID] = NEGATIVE;
			line_polarity_state[CID] = NEGATIVE;
		
			daa_byte_write(4, 0x00,CID);										/* clear INT register */
			daa_byte_write(43, 0x05, CID);										/*set threshold for current disconnect*/
			daa_byte_write(44, ((daa_byte_read(44,CID) | 0x02) & 0xfa), CID);	/*enable current INT, below threshold*/
			daa_byte_write(3, E_DAA_MET | E_DAA_REC_OVERLOAD | E_DAA_POLARITY_REVERSAL , CID);	/*enabel metering and polarity reversal INT*/
			daa_byte_write(17, (daa_byte_read(17,CID) | 0x20), CID);			/* disable auto calibration */

		}
		else
		{
			daa_byte_write(43, 0x50, CID);										/*set threshold for positive voltage*/
			if(CurrentAnalogState[CID].msg_wait == MSG_WAIT)
				daa_byte_write(44,((daa_byte_read(44,CID)| 0x06) & 0xfe),CID);	/*enable Voltage INT, above threshold*/
			else
				daa_byte_write(44, (daa_byte_read(44,CID) | 0x07), CID);		/*enable Voltage INT, above threshold*/

			prev_polarity_state[CID] = NEGATIVE ;									/* negative */
			current_polarity_state[CID] = NEGATIVE;
		}
	}
	else
	{
		if(CurrentAnalogState[CID].hook == OFF_HOOK)
		{
			prev_polarity_state[CID]= POSITIVE;									/* positive */
			current_polarity_state[CID] = POSITIVE;	
			line_polarity_state[CID] = POSITIVE;
		
			daa_byte_write(4, 0x00,CID);										/* clear INT register */
			daa_byte_write(43, 0x05, CID);										/*set threshold for current disconnect*/
			daa_byte_write(44, ((daa_byte_read(44,CID) | 0x02) & 0xfa), CID);	/*enable current INT, below threshold*/
			daa_byte_write(3, E_DAA_MET | E_DAA_REC_OVERLOAD | E_DAA_POLARITY_REVERSAL , CID);	/*enabel metering and polarity reversal INT*/	
			daa_byte_write(17, (daa_byte_read(17,CID) | 0x20), CID);			/* disable auto calibration */

		}
		else
		{
			daa_byte_write(43, 0x50, CID);										/*set threshold for positive voltage*/
			if(CurrentAnalogState[CID].msg_wait == MSG_WAIT)
				daa_byte_write(44,((daa_byte_read(44,CID)| 0x06) & 0xfe),CID);	/*enable Voltage INT, above threshold*/
			else
				daa_byte_write(44, (daa_byte_read(44,CID) | 0x07), CID);		/*enable Voltage INT, above threshold*/

			prev_polarity_state[CID]= POSITIVE;									/* positive */
			current_polarity_state[CID] = POSITIVE;	
		}
	}
}

/*-----------------------------------------------------------------------------------------
* Function Name:
*  Routine:	AnalogIFDaaSetHook
* 	
* Description:
*  	This function changes the hook state of a channel connected to an analog DAA port.
* 
* Arguments:
*	int CID									- Channel number
*	TAnalogIFHookState AnalogIFHookState	- New hook state to apply (either
*											  ON_HOOK or OFF_HOOK).
* Return Value:
*	0 if success.
*	Otherwise if failure.
*
* Remarks:
*   none
*----------------------------------------------------------------------------------------*/

int AnalogIFDaaSetHook(int CID, int AnalogHookState)
{
	unsigned char write_reg;

    DBG_PRINT("AnalogIFDaaSetHook(CID=%d, AnalogHookState=%d) Enter.....\n", CID, AnalogHookState);

    if(CurrentAnalogState[CID].hook != AnalogHookState)
	{
		AnalogDaaSetHook(CID, AnalogHookState);
		CurrentAnalogState[CID].hook = AnalogHookState;
    	/* reset echo canceller */
		if(AnalogHookState == OFF_HOOK)
		{
            DBG_PRINT("AnalogHookState == OFF_HOOK - Should we rest the EC ?????? T.B.D \n");
            daa_byte_write(44, 0x00 ,CID);								/* disable current INT */
			daa_byte_write(3, E_DAA_MET | E_DAA_REC_OVERLOAD, CID);		/*enabel metering, polarity reversal will be anabled after line stable*/
			daa_byte_write(4, 0x00,CID);								/* clear INT register */
		}
        else if(AnalogHookState == ON_HOOK)										/* onhook state */
	    {
		    daa_byte_write(4, 0x00,CID);										/* clear INT register */
		    write_reg=	E_DAA_RING;
		    daa_byte_write(3, write_reg , CID);								/* enabel INT (ring/polarity/ground start) */
		    daa_byte_write(44, (daa_byte_read(44,CID) & 0xfd), CID);			/* disable Voltage INT*/
		    daa_byte_write(17, (daa_byte_read(17,CID) & 0xdf), CID);			/* enable auto calibration */
		    CurrentAnalogState[CID].msg_wait = NO_MSG_WAIT;
	    }
	}
	else
	{
		if(AnalogHookState == OFF_HOOK)
			DBG_PRINT("Trying to generate off hook while already in off hook state, Cid %d", CID);
		else
			if(AnalogHookState == ON_HOOK)
				DBG_PRINT("Trying to generate on hook while already in on hook state, Cid %d", CID);
	}
    return 0;
}


void AnalogIFDaaSetFlashEnd(unsigned long data)
{	

int CID = (int)data;

#if M1000
	if(BoardParams.DuSLICBoardParams.GroundKeyDetection == 1)			/* ground start is enabled */ 
		GroundStartStatus[CID] = GroundStartFlashHook;
#endif

	AnalogIFDaaSetHook(CID, OFF_HOOK);  
	
#if M1000
	if(BoardParams.DuSLICBoardParams.GroundKeyDetection == 1)			/* ground start is enabled */ 
		GroundStartStatus[CID] = GroundStartIdle;
#endif
 
}


int AnalogIFDaaSetFlash(int CID)
{


    if(CurrentAnalogState[CID].hook == OFF_HOOK)
    {
		AnalogIFDaaSetHook(CID, ANALOG_ON_HOOK);
		HookFlashTimerId[CID].expires  = jiffies+(HZ/2 );
              HookFlashTimerId[CID].data  = CID;  
		HookFlashTimerId[CID].function = AnalogIFDaaSetFlashEnd;
		add_timer(&HookFlashTimerId[CID]);
		
    }
    else
	    DBG_PRINT("Trying to generate flash hook while in on hook state, Cid %d .\n", CID);
    return 0;
}

/*------------------------------------------------------------------------------------------
*Function Name: 
*	AnalogIFQueryDaaStatus
*
*Description:
*	This function will cause the board to generate an acEV_ANALOG_IF_STATUS 
*	event containing the Anic loop status, taken from inside parameters that were
*	calculated during the last ANIC evaluation.
*
*Arguments:
*	int CID			- channel.
*
*Return Value:
*	0 if success.
*	Otherwise if failure.
*
*Remarks:
*   This function can be used to test whether a port is functioning. If an 
*   acEV_ANALOG_IF_STATUS has not been received from the board within a short period 
*   (1 second) since this function was invoked, the User can assume the port is not 
*   functioning correctly.
*
*----------------------------------------------------------------------------------------*/
int AnalogIFQueryDaaStatus (int CID)
{
	acTEvAnalogIfFxoStatusInfo AnalogAnicStatusFromDriver;    
		
	GetAnalogDAAStatusEventInfo (CID, &AnalogAnicStatusFromDriver);
	AnalogAnicStatusFromDriver.Connected = CurrentAnalogState[CID].line_connected;
 
    DBG_PRINT("NTT CallerID failed, no CallerID data was detected");

#if 0 //NIRP T.B.D

	EventType = acEV_ANALOG_IF_FXO_STATUS;

	AnalogAnicStatusFromDriver.BoardTemp = 0;

	SendStatusToPc(CID, EventType, &AnalogAnicStatusFromDriver, sizeof(acTEvAnalogIfFxoStatusInfo));
#endif
    return 0;
}



/************************************************************************************
Function Name: 
	AnalogDaaIFLoopCommand

Description:
	This function will activate the internal loop back of the ANIC.

Arguments:
	[i] int CID
	[i] enum acTAnalogIFLoopCommand  AnalogIFLoopCommand)


Return Value:
	0 if success.
	Otherwise if failure.

Remarks:
   none.

*************************************************************************************/

int AnalogDaaIFLoopCommand(int CID, int AnalogLoopCommandVal)
{
	DaaLoopCommand(CID, AnalogLoopCommandVal);
//	SendStatusToPc(CID,acEV_ANALOG_IF_LOOP,NULL,0); //send start up prompt to PC
    return 0;
}

int AnalogIFDaaRegisterCommand(int CID, unsigned char address, unsigned char value)
{
	AnalogDaaRegisterCommand(CID, address, value);
	//SendStatusToPc(CID,acEV_ANALOG_IF_REGISTER,NULL,0); //send start up prompt to PC
    return 0;
}

unsigned char AnalogIFDaaReadRegisterCommand(int CID, unsigned char address)
{   
	//int EventType;
	acTEvAnalogIfReadRegisterInfo			AnalogReadRegisterStatus;


	AnalogReadRegisterStatus.RegisterValue  =  AnalogDaaReadRegisterCommand(CID, address);	

	//SendStatusToPc(CID, EventType, &AnalogReadRegisterStatus, sizeof(AnalogReadRegisterStatus));
    return 0;
}
void DaaNTTCallerIDDetection(ULONG CID, ULONG d1, ULONG d2, ULONG d3)
{
	//int EventType;
	//acTEvAnalogNTTCallerIdFailedInfo	NTTCallerIDFailedInfo;
	//unsigned char TipRingVoltage;

	if(NTTCallerIDState[CID] == NTT_CID_VALIDATE_POLARITY_REVERSAL)					/* validate polarity reversal */
	{
		if (CurrentAnalogState[CID].ring == RING_ON)				/* polarity returned back to normal no CID*/
		{
			NTTCallerIDState[CID] = NTT_CID_NOT_ACTIVATED;
            
            if(NTTTimersId[CID] != MAX_TIMERS)
			{
				//DeleteTimer(NTTTimersId[CID]);	
				NTTTimersId[CID] = MAX_TIMERS;	
			}
		}
		else																		/* polarity was validated */
		{
			NTTCallerIDState[CID] = NTT_CID_WAIT_FOR_CAR;
			NTTPolarityState[CID] = REVERSE;
			//ChangeTimerPeriodMilli(NTTTimersId[CID],6000);//NIRP T.B.D
		}
		
	}

	else if(NTTCallerIDState[CID] == NTT_CID_WAIT_FOR_CAR)					/* no CAR was detected in 6 sec */
	{
		NTTCallerIDState[CID] = NTT_CID_NOT_ACTIVATED;
		if(NTTTimersId[CID] != MAX_TIMERS)
		{
			//DeleteTimer(NTTTimersId[CID]);
			NTTTimersId[CID] = MAX_TIMERS;
		}
	}

	else if(NTTCallerIDState[CID] == NTT_CID_GENERATE_DC_LOOP)
	{
		AnalogIFDaaSetHook(CID, OFF_HOOK);
		NTTCallerIDState[CID] = NTT_CID_WAIT_FOR_CID;
		//ChangeTimerPeriodMilli(NTTTimersId[CID],3200);//NIRP T.B.D
	}

	else if(NTTCallerIDState[CID] == NTT_CID_WAIT_FOR_CID)			/* no caller id data was detected */
	{
		AnalogIFDaaSetHook(CID, ON_HOOK);

		if(NTTTimersId[CID] != MAX_TIMERS)
		{
			//DeleteTimer(NTTTimersId[CID]);
			NTTTimersId[CID] = MAX_TIMERS;
		}
		NTTCallerIDState[CID] = NTT_CID_NOT_ACTIVATED;

		DBG_PRINT("NTT CallerID failed, no CallerID data was detected");
#if 0 //NIRP T.B.D		
        EventType = acEV_ANALOG_NTT_CALLER_ID_FAILED;
		NTTCallerIDFailedInfo.HookState = acON_HOOK;
		NTTCallerIDFailedInfo.FailCause = acNoCallerIDDataDetected;

		HandleAppEvent( CID, EventType, &NTTCallerIDFailedInfo, sizeof(acTEvAnalogNTTCallerIdFailedInfo),AnalogModule);
#endif
	}

	else if(NTTCallerIDState[CID] == NTT_CID_DATA_DETECTED)
	{
		NTTCallerIDState[CID] = NTT_CID_GENERATE_DC_LOOP_DISCONNECT;
		//ChangeTimerPeriodMilli(NTTTimersId[CID],500);//NIRP T.B.D
	}
	else if(NTTCallerIDState[CID] == NTT_CID_GENERATE_DC_LOOP_DISCONNECT)
	{
		AnalogIFDaaSetHook(CID, ON_HOOK);
		if(NTTTimersId[CID] != MAX_TIMERS)
		{
			//DeleteTimer(NTTTimersId[CID]);
			NTTTimersId[CID] = MAX_TIMERS;
		}
		NTTCallerIDState[CID] = NTT_CID_NOT_ACTIVATED;
	}

}

void DaaWinkStateExpire(ULONG CID, ULONG d1, ULONG d2, ULONG d3)
{
	acTEvAnalogIfReversalPolarityDetectedInfo		AnalogReversalPolarityStatus;
	//int EventType;
	//unsigned char TipRingVoltage1;
	PolarityTimersId[CID] = MAX_TIMERS;
	if(PolarityState[CID] == WINK_STATE)
	{
		PolarityState[CID] = NORMAL_STATE;
		if (current_polarity_state [CID] != prev_polarity_state [CID] )
		{
            prev_polarity_state[CID]=current_polarity_state[CID];			
			AnalogReversalPolarityStatus.LinePolarity = current_polarity_state[CID];
			if (line_polarity_state[CID] == current_polarity_state[CID])
			{
				AnalogReversalPolarityStatus.LinePolarityState = NORMAL;
				CurrentAnalogState[CID].polarity = NORMAL;
			}
			else
			{
				AnalogReversalPolarityStatus.LinePolarityState = REVERSE;
				CurrentAnalogState[CID].polarity = REVERSE;
			}
			
			CurrentDisconnectState[CID] = IDLE_DISCONNECT_STATE;							/* return the current state machine to idle */
		    DBG_PRINT("Polarity reversal detected \n");
#if 0 //NIRP T.B.D
			EventType = acEV_ANALOG_IF_REVERSAL_POLARITY_DETECTED;
			HandleAppEvent( CID, EventType, &AnalogReversalPolarityStatus, sizeof(AnalogReversalPolarityStatus),AnalogModule);	
#endif
			
		}
	}

}

/*--------------------------------------------------------------------
 * TASK:   ANICPeriodicCheckTask
 *
 * DESCRIPTION:
 *   Checks the raw cell queue and errors on the ATM phy.
 *
 *
 * RESULTS:
 *   none
 *--------------------------------------------------------------------*/
void DAAPeriodicPollingCheck(void)
{
	int CID,DaaB;
       //int AnalogDaaRevision;//,temp_counter = 0;
	//int EventType, error_code;
       int first_time,check_line=OFF;
	WORD16 interrupts;
	//TimerUserParams TimerConfig;
	unsigned char TipRingVoltage;
       //unsigned char CurrentReading,CurrentVoltageINTCTRL;
	//unsigned char temp,MaskReg;
#if 0 //NIRP T.B.D
	acTEvAnalogIfRingStartInfo	RingStartInfo;
	acTEvAnalogIfRingEndInfo RingEndInfo;
	acTEvAnalogIfOutOfServiceInfo OutOfServiceInfo;
#endif	
	for ( CID=0; CID < NUM_DAA_LINES; CID++)
		if (AnalogIFDaaGetAnalogPortType(CID) == DAA)
		{
			InitDaaArrays(CID);
		}

	//GaugeSetValue(PM_analogActiveLines,0, 0);								/* set PM_analogActiveLines initial value to "0" */

    while(1)
    {
        first_time = 1;
		for (CID=0; CID < NUM_DAA_LINES; CID++)
			if (AnalogIFDaaGetAnalogPortType(CID) == DAA)
			{
#if M1000			/* dummy read in oredr to sync the SPI to DAA */
//				if(first_time)
//				{
//					daa_byte_read(11,CID);	/* dummy read of version ID*/
//					first_time = 0;
//				}
#endif
				if (GetDAAInterruptsStatus(CID) == TRUE)
				{
					for (DaaB = 0; DaaB < 2 ; DaaB++)
					{
						if(AnalogIFDaaGetAnalogPortType(CID + DaaB) == DAA)
						{
						interrupts = DAAHandler (CID + DaaB);

						if (CurrentAnalogState[CID + DaaB].hook == ON_HOOK)
						{
							if (interrupts & DAA_RING_START_INT) 
							{
							    DBG_PRINT("Got DAA_RING_START_INT ");
                                //LIGHT_PORTS_LEDS(CID+ DaaB,GREEN);#if 0 //NIRP T.B.D
								CurrentAnalogState[CID+ DaaB].ring = RING_ON;
								if(NTTCallerIDState[CID + DaaB] == NTT_CID_WAIT_FOR_CAR)
								{
									NTTCallerIDState[CID + DaaB] = NTT_CID_GENERATE_DC_LOOP;
									//ChangeTimerPeriodMilli(NTTTimersId[CID + DaaB],200);//NIRP T.B.D
								}
								else 
								{		
#if 0 //NIRP T.B.D									
                                    EventType = acEV_ANALOG_IF_RING_START;
									RingStartInfo.EventTrigger = EVENT_TRIGGER_RING_DETECTION;
									HandleAppEvent(CID + DaaB, EventType ,&RingStartInfo ,sizeof(acTEvAnalogIfRingStartInfo),AnalogModule);
#endif
									if(MessageWaitingTimersId[CID + DaaB]!=MAX_TIMERS)
									{
										//DeleteTimer(MessageWaitingTimersId[CID + DaaB]);
										MessageWaitingTimersId[CID + DaaB] = MAX_TIMERS;
									}
									if(CallerIDStandardConfigFXO == CALLER_ID_STANDARD__NTT)
									{									
									if(NTTCallerIDState[CID + DaaB] == NTT_CID_VALIDATE_POLARITY_REVERSAL)
									{
										if(NTTTimersId[CID + DaaB] != MAX_TIMERS)
										{
											//DeleteTimer(NTTTimersId[CID + DaaB]);	
											NTTTimersId[CID + DaaB] = MAX_TIMERS;			
										}
										}
										else
											NTTCallerIDState[CID + DaaB] = NTT_CID_NOT_ACTIVATED;
									}
									if(CallerIDStandardConfigFXO == CALLER_ID_STANDARD__DTMF_CLIP_INDIAN)
									{
										IndianCalleridRingState[CID + DaaB] = RING_START;
#if 0 //NIRP T.B.D										
										TimerConfig.Type = TIMER_THROWAWAY;
										TimerConfig.PeriodMilli = 50;
										TimerConfig.ExpireFunction = DaaIndianCallerIdRingDetection;
										TimerConfig.FunctionParams[0] = CID + DaaB;
										
										AllocateAndRunTimer(TimerConfig,&IndianCallerIdTimerId[CID + DaaB]);
#endif
									}
								}
							}

							else if (interrupts & DAA_RING_STOP_INT) 
							{
							    DBG_PRINT("Got DAA_RING_STOP_INT ");
                                CurrentAnalogState[CID + DaaB].ring = RING_OFF;
								//MWIDetectionExpaire(CID + DaaB, 0, 0, 0);
								//LIGHT_PORTS_LEDS(CID+ DaaB,OFF);	
								if(NTTCallerIDState[CID + DaaB] == NTT_CID_WAIT_FOR_CAR) 
								{
									NTTCallerIDState[CID + DaaB] = NTT_CID_GENERATE_DC_LOOP;
									//ChangeTimerPeriodMilli(NTTTimersId[CID + DaaB],200);//NIRP T.B.D
								}
								else 
								{
#if 0 //NIRP T.B.D
									EventType = acEV_ANALOG_IF_RING_END;
									RingEndInfo.EventTrigger = EVENT_TRIGGER_RING_DETECTION;
									HandleAppEvent(CID + DaaB, EventType ,&RingEndInfo ,sizeof(acTEvAnalogIfRingEndInfo),AnalogModule);
#endif
									if(CallerIDStandardConfigFXO == CALLER_ID_STANDARD__NTT)
									{
										if(NTTCallerIDState[CID + DaaB] == NTT_CID_VALIDATE_POLARITY_REVERSAL)
										{
											if(NTTTimersId[CID + DaaB] != MAX_TIMERS)
											{
												//DeleteTimer(NTTTimersId[CID + DaaB]);	
												NTTTimersId[CID + DaaB] = MAX_TIMERS;
											}
										}
										if(NTTCallerIDState[CID + DaaB] != NTT_CID_NOT_ACTIVATED)
											NTTCallerIDState[CID + DaaB] = NTT_CID_NOT_ACTIVATED;
									}
									else if((CallerIDStandardConfigFXO == CALLER_ID_STANDARD__DTMF_CLIP_INDIAN) && (IndianCalleridRingState[CID + DaaB] == RING_START))
									{
										IndianCalleridRingState[CID + DaaB] = NO_RING;
										if(IndianCallerIdTimerId[CID + DaaB] != MAX_TIMERS)
										{
											//DeleteTimer(IndianCallerIdTimerId[CID + DaaB]);
											IndianCallerIdTimerId[CID + DaaB] = MAX_TIMERS;
										}
									}
									else if((CallerIDStandardConfigFXO == CALLER_ID_STANDARD__DTMF_CLIP_INDIAN) && (IndianCalleridRingState[CID + DaaB] == INDIAN_RING_DETECTION))
									{
										if(IndianCallerIdTimerId[CID + DaaB] != MAX_TIMERS)
										{
											//DeleteTimer(IndianCallerIdTimerId[CID + DaaB]);
											IndianCallerIdTimerId[CID + DaaB] = MAX_TIMERS;
										}
										//vpIndianCIDRingPulseDetected(CID + DaaB);
										IndianCalleridRingState[CID + DaaB] = NO_RING;
									} 
								}
							}

							else if (interrupts & DAA_POLARITY_INT)						/* Polarity Reversal in case of NTT Caller ID */						
								{
							    DBG_PRINT("Got DAA_POLARITY_INT ");
								
//								if (interrupts & DAA_MESSAGE_WAITING_INT) /*case of fulse MWI INT*/
//									interrupts &= ~(DAA_MESSAGE_WAITING_INT);
										if ((CallerIDStandardConfigFXO == CALLER_ID_STANDARD__NTT) && (NTTCallerIDState[CID + DaaB] == NTT_CID_NOT_ACTIVATED) && (CurrentAnalogState[CID + DaaB].ring == RING_OFF))
										{
											NTTCallerIDState[CID + DaaB] = NTT_CID_VALIDATE_POLARITY_REVERSAL;
#if 0 //NIRP T.B.D
											AllocateTimer(&NTTTimersId[CID + DaaB]);
				  
											TimerConfig.Type = TIMER_PERIODIC;
									        TimerConfig.PeriodMilli = 350;								/* DC Loop detection period is 6 sec */
											TimerConfig.ExpireFunction = DaaNTTCallerIDDetection;
											TimerConfig.FunctionParams[0] = CID + DaaB;
											ConfigTimer(NTTTimersId[CID + DaaB],TimerConfig);
											RunTimer(NTTTimersId[CID + DaaB]);
#endif
										}
										else 															/* in case of NTT new polarity will be updated in the NTT_CID_VALIDATE_POLARITY_REVERSAL */
								{
											if(CallerIDStandardConfigFXO != CALLER_ID_STANDARD__NTT)
									{									
										TipRingVoltage = daa_byte_read(29,CID + DaaB);
									if((TipRingVoltage & 0x80) == 0x80) 
										current_polarity_state[CID + DaaB] = NEGATIVE;
									else
										current_polarity_state[CID + DaaB] = POSITIVE;
									
												prev_polarity_state[CID + DaaB] = current_polarity_state[CID + DaaB];
									}
									}
								}

								if ((interrupts & DAA_MESSAGE_WAITING_INT) && (CurrentAnalogState[CID + DaaB].ring == RING_OFF) && (MessageWaitingTimersId[CID + DaaB]==MAX_TIMERS))						/* MWI */
								{
							        DBG_PRINT("Got DAA_MESSAGE_WAITING_INT ");

#if 0 //NIRP T.B.D
									TimerConfig.Type = TIMER_THROWAWAY;
									TimerConfig.PeriodMilli = 350;
									TimerConfig.ExpireFunction = MWIDetectionExpaire;
									TimerConfig.FunctionParams[0] = CID + DaaB;
					  
									AllocateAndRunTimer(TimerConfig,&MessageWaitingTimersId[CID + DaaB]);
#endif
								}
#if M1000								
							if (interrupts & DAA_TIP_GROUND_DETECT_INT)
							{
							MaskReg = (daa_byte_read(3,CID + DaaB) & 0x02);
								if((BoardParams.DuSLICBoardParams.GroundKeyDetection) && (MaskReg == 0x02))
								{
									temp = daa_byte_read(32,CID + DaaB);
									if((GroundStartStatus[CID + DaaB] == GroundStartIdle) && ((temp & 0x04) == 0x00))
										{
									LIGHT_PORTS_LEDS(CID+ DaaB,GREEN);	
											GroundStartStatus[CID + DaaB] = GroundStartCOSeizure;
											EventType = acEV_ANALOG_IF_RING_START;
											RingStartInfo.EventTrigger = EVENT_TRIGGER_GROUND_START_DETECTION;
											
											HandleAppEvent(CID + DaaB, EventType ,&RingStartInfo ,sizeof(acTEvAnalogIfRingStartInfo),AnalogModule);									
										}
								else
								{
										if((GroundStartStatus[CID + DaaB] == GroundStartCOSeizure) && ((temp & 0x04) == 0x04))			/* check if CO disconnected TIP from ground */
									{
											LIGHT_PORTS_LEDS(CID+ DaaB,OFF);
											GroundStartStatus[CID + DaaB] = GroundStartIdle;
											EventType = acEV_ANALOG_IF_RING_END;
											RingEndInfo.EventTrigger = EVENT_TRIGGER_GROUND_START_DETECTION;
											HandleAppEvent(CID + DaaB, EventType ,&RingEndInfo ,sizeof(acTEvAnalogIfRingEndInfo),AnalogModule);								
									}
								}
								}
							}
#endif
						}

						if (CurrentAnalogState[CID + DaaB].hook == OFF_HOOK)
						{

							if ((interrupts & DAA_METERING_SIGNAL_ON_INT) && (MeteringTimersId[CID + DaaB]==MAX_TIMERS))
							{
#if 0 //NIRP T.B.D
								TimerConfig.Type = TIMER_ONE_TIME;
								TimerConfig.PeriodMilli = 50;
								TimerConfig.ExpireFunction = MeteringDetectionExpaire;
								TimerConfig.FunctionParams[0] = CID + DaaB;
								TimerConfig.FunctionParams[1] = NOISE_STATE;
								AllocateAndRunTimer(TimerConfig,&MeteringTimersId[CID + DaaB]);
#endif								
							}
							
	/* for now there is no need in the end of the metering tone, for future use */ 
	//						if (interrupts & DAA_METERING_SIGNAL_OFF_INT) 
	//						{
	//							AddDaaHookEvent(CID, EV_ANALOG_IF_END_METERING_TONE);	
	//						}					
#if 0 //NIRP T.B.D
							if (interrupts & DAA_POLARITY_INT)
							{
								TipRingVoltage = daa_byte_read(29,CID + DaaB);
//								if ((((TipRingVoltage & 0x80) == 0x80) && (TipRingVoltage < 0xfd)) ||  (((TipRingVoltage & 0x80) == 0x00) && (TipRingVoltage > 0x03)))
								{
									if (NTTCallerIDState[CID + DaaB] == NTT_CID_NOT_ACTIVATED)
									{
										if ((TipRingVoltage & 0x80) == 0x80) /* negative */
											current_polarity_state[CID + DaaB] = NEGATIVE;
										else
											current_polarity_state[CID + DaaB] = POSITIVE;
								
										/* Normal state - after first ReversalPolarity INT	*/
										if (PolarityState[CID + DaaB] == NORMAL_STATE)
										{
											if (current_polarity_state[CID + DaaB] != prev_polarity_state[CID + DaaB])
												PolarityState[CID + DaaB] = NOISE_STATE;
										}					
 										else if ((PolarityState[CID + DaaB] == NOISE_EXPAIRE_STATE) && (current_polarity_state[CID + DaaB] == prev_polarity_state[CID + DaaB]))	/* case we got another INT with in 50 msec from the first - means noise)*/
										{
											PolarityState[CID + DaaB] = NORMAL_STATE;
										}
										else if (PolarityState[CID + DaaB] == WINK_STATE)	/* case of an INT during the Timer run time */
										{
											if (current_polarity_state [CID + DaaB] == prev_polarity_state [CID + DaaB] )
											{
												if(PolarityTimersId[CID + DaaB] != MAX_TIMERS)
												{
													DeleteTimer(PolarityTimersId[CID + DaaB]);
													PolarityTimersId[CID + DaaB] = MAX_TIMERS;
												}
												EventType = acEV_ANALOG_IF_WINK;
												HandleAppEvent( CID + DaaB, EventType, NULL, 0,AnalogModule);	
												PolarityState[CID + DaaB] = NORMAL_STATE;
											}
										}
									}
								
								}
							}
							/* if there is a polarity reversal interrupt then no current disconnect is possible */
							if (interrupts & DAA_CURRENT_DISCONNECT_INT)									/* check if far end disconnect is enabled by CD*/
							{
								CurrentReading = daa_byte_read(28,CID + DaaB);

								if ((CurrentReading < 0x05)&& (CurrentDisconnectState[CID + DaaB] == IDLE_DISCONNECT_STATE))				/* current below threshold */ 				
								{
									if(CurrentDissconnectTimersId[CID + DaaB] == MAX_TIMERS)
									{
#if 0 //NIRP T.B.D									
									/* do another current read after 50msec to validate the current disconnect */
									TimerConfig.Type = TIMER_THROWAWAY;
									TimerConfig.PeriodMilli = 50;
									TimerConfig.ExpireFunction = DaaCurrentDissconnectDetection;
									TimerConfig.FunctionParams[0] = CID + DaaB;
					  
									AllocateAndRunTimer(TimerConfig,&CurrentDissconnectTimersId[CID + DaaB]);
#endif
									}
								}				
								else if(CurrentDisconnectState[CID + DaaB] == POSSIBLE_DISCONNECT)					/* case of another interrupt in the first 900msec */
								{
									CurrentDisconnectState[CID + DaaB] = IDLE_DISCONNECT_STATE;
									if(CurrentDissconnectTimersId[CID+ DaaB] != MAX_TIMERS)
									{							
										DeleteTimer(CurrentDissconnectTimersId[CID + DaaB]);
										CurrentDissconnectTimersId[CID+ DaaB] = MAX_TIMERS;
									}
									DBG_PRINT("Current disconnect duration is too long");
									if(daa_byte_read(44,CID + DaaB) & 0x01)											/* change INT polarity */
										daa_byte_write(44,(daa_byte_read(44,CID + DaaB) & 0xfe),CID + DaaB);
									else
										daa_byte_write(44,(daa_byte_read(44,CID + DaaB)| 0x01),CID + DaaB);
								}
								else if(CurrentDisconnectState[CID + DaaB] == DISCONNECT_DETECTION)				/* current returned within 750-1100 msec */ 
								{
									if(CurrentDissconnectTimersId[CID+ DaaB] != MAX_TIMERS)
									{							
										DeleteTimer(CurrentDissconnectTimersId[CID + DaaB]);
										CurrentDissconnectTimersId[CID+ DaaB] = MAX_TIMERS;
									}
									CurrentDisconnectState[CID + DaaB] = IDLE_DISCONNECT_STATE;
									if(CurrentReading > 0x03)												/* current above 5.5mA*/
									{
										EventType = acEV_ANALOG_IF_CURRENT_DISCONNECT_DETECTED;
										HandleAppEvent( CID + DaaB, EventType, NULL, 0,AnalogModule);	
									}
									if(daa_byte_read(44,CID + DaaB) & 0x01)											/* change INT polarity */
										daa_byte_write(44,(daa_byte_read(44,CID + DaaB) & 0xfe),CID + DaaB);
									else
										daa_byte_write(44,(daa_byte_read(44,CID + DaaB)| 0x01),CID + DaaB);
								}
								else if(CurrentDisconnectState[CID + DaaB] == LINE_DISCONNECTED)					/* line was reconnected */ 
								{
									if(CurrentReading > 0x03)												/* current above 5.5mA */
									{
										CurrentDisconnectState[CID + DaaB] = IDLE_DISCONNECT_STATE;
										CurrentAnalogState[CID+ DaaB].line_connected = ANALOG_LINE_CONNECTED;
										EventType = acEV_ANALOG_IF_LINE_CONNECTED;
										HandleAppEvent( CID + DaaB, EventType, NULL, 0,AnalogModule);
									}
									if(daa_byte_read(44,CID + DaaB) & 0x01)											/* change INT polarity */
										daa_byte_write(44,(daa_byte_read(44,CID + DaaB) & 0xfe),CID + DaaB);
									else
										daa_byte_write(44,(daa_byte_read(44,CID + DaaB)| 0x01),CID + DaaB);
								}
								
							}
#endif
#if M1000
							if ((interrupts & DAA_TIP_GROUND_DETECT_INT) && (BoardParams.DuSLICBoardParams.GroundKeyDetection))
							{
								if(GroundStartStatus[CID + DaaB] == GroundStartRingGrounded)			/* DAA init the offhook, waiting to CO reply */
								{									
									GroundStartStatus[CID + DaaB] = GroundStartTIPGrounded;
									AnalogIFDaaSetHook(CID + DaaB, OFF_HOOK); 
								}
							}
#endif

						} /* end off_hook events */
						}	/* end if(AnalogPortType[CID + DaaB] == DAA) */					
					}			
				} /* end if interrupt */
				
#if 0 //NIRP T.B.D   - test procudure temporarily removed                
                for (DaaB = 0; DaaB < 2 ; DaaB++)
				{
					if(AnalogIFDaaGetAnalogPortType(CID + DaaB) == DAA)
					{
						if(check_line == ON)				  /* test will run every 100 msec*/ 		  
						{
						  AnalogDaaRevision = AnalogIFGetDaaChipRevision(CID + DaaB);
						  if ((AnalogDaaRevision > 0x01) && (AnalogDaaRevision < 0xff))
						  {
							  if(InServicePortStatus[CID + DaaB]==ANALOG_PORT_OUT_OF_SERVICE) /* back to service*/
							  {
								  InServicePortStatus[CID + DaaB]=ANALOG_PORT_IN_SERVICE;
								  
								  OutOfServiceInfo.TriggerCause = ANALOG_TRIGGER_CAUSE_SPI;
								  HandleAppEvent( CID + DaaB, acEV_ANALOG_IF_CONNECTED, &OutOfServiceInfo , sizeof(acTEvAnalogIfOutOfServiceInfo),AnalogModule);					  
								  
								  eMIB_ClearAnalogPortsMoAlarm(CID + DaaB,eTRAP_acAnalogPortSPIOutOfService_TRAP_TYPE_D); 					  
							  }
						  }
						  else
						  {
							  if(InServicePortStatus[CID + DaaB]==ANALOG_PORT_IN_SERVICE) /* out of service*/
							  {
								  InServicePortStatus[CID + DaaB]=ANALOG_PORT_OUT_OF_SERVICE;
								  OutOfServiceInfo.TriggerCause = ANALOG_TRIGGER_CAUSE_SPI;
								  HandleAppEvent( CID + DaaB, acEV_ANALOG_IF_OUT_OF_SERVICE, &OutOfServiceInfo , sizeof(acTEvAnalogIfOutOfServiceInfo),AnalogModule);		
								  
								  eMIB_RaiseAnalogportsMoAlarm(CID + DaaB,eTRAP_acAnalogPortSPIOutOfService_TRAP_TYPE_D,major);
							  }
						  }
					
						}
						if (CurrentAnalogState[CID + DaaB].hook == OFF_HOOK)
						{
							if (PolarityState[CID + DaaB] == NOISE_STATE)
								PolarityState[CID + DaaB] = NOISE_EXPAIRE_STATE;
							else if (PolarityState[CID + DaaB] == NOISE_EXPAIRE_STATE) /* check if noise was over */
							{
								if (current_polarity_state[CID + DaaB] == prev_polarity_state[CID + DaaB])
									PolarityState[CID + DaaB] = NORMAL_STATE;
								else
								{
									TipRingVoltage = daa_byte_read(29,CID + DaaB);
									if ((TipRingVoltage & 0x80) == 0x80) /* negative */
										current_polarity_state[CID + DaaB] = NEGATIVE;
									else
										current_polarity_state[CID + DaaB] = POSITIVE;
									/* double check that the voltage polarity haven't been change since the INT (50msec ago) */
									if (current_polarity_state[CID + DaaB] == prev_polarity_state[CID + DaaB])
										PolarityState[CID + DaaB] = NORMAL_STATE;
									else
									{
									if ((((TipRingVoltage & 0x80) == 0x80) && (TipRingVoltage < 0xfd)) ||  (((TipRingVoltage & 0x80) == 0x00) && (TipRingVoltage > 0x03)))
									{
										PolarityState[CID + DaaB] = WINK_STATE;	
#if 0 //NIRP T.B.D										
										TimerConfig.Type = TIMER_THROWAWAY;
										TimerConfig.PeriodMilli = BoardParams.Ext.WinkTime + 50;
										TimerConfig.ExpireFunction = DaaWinkStateExpire;
										TimerConfig.FunctionParams[0] = CID + DaaB;
										
										AllocateAndRunTimer(TimerConfig,&PolarityTimersId[CID + DaaB]);
#endif
									}
									else
										PolarityState[CID + DaaB] = NORMAL_STATE;
									}
									
								}
							}
							
							
						}
						if (CurrentAnalogState[CID + DaaB].hook == ON_HOOK) 
						{
							if((CurrentAnalogState[CID + DaaB].ring == RING_OFF) && (check_line == ON))
								AnalogIFDaaQueryOnHookPortStatus(CID + DaaB);
						}
					}
				}
#endif
			} /* end if DAA */
		/* check the line connect state every 100msec*/
		if(check_line == ON)
			check_line=OFF;
		else
			check_line=ON;
    }
}

int AnalogDAAIFSetPCMMode(int Cid, unsigned char mode)
{
 	switch (mode)
	{
	case 3:
			daa_byte_write(33, ((daa_byte_read(33,Cid) | 0x08) & ~(0x10)) , Cid);	//PCM Configuration register  Ulaw
			break;
	case 1:	
			daa_byte_write(33, (daa_byte_read(33,Cid) & ~(0x18)) , Cid);	//PCM Configuration register  Alaw
			break;
	default: break;
	}
	
  return(0);	
}

int AnalogIFGetDaaChipRevision(int port)
{
	return (GetDaaChipRevision(port));
}
/*------------------------------------------------------------------------------------------
*Function Name: 
*	AnalogIFDAAStatusInfo
*
*Description:
*	This function will return the DAA line status  
*	containing the DAA loop status, taken from inside parameters that were
*	calculated during the last DAA evaluation.
*
*Arguments:
*	int CID			- channel.
*	struct acTEvAnalogIfFxoStatusInfo *AnalogDAAStatusInfoPtr
*Return Value:
*	void
*Remarks:
*   This function can be used to test whether a port is functioning. 
*
*----------------------------------------------------------------------------------------*/
void AnalogIFDAAStatusInfo (int CID, acTEvAnalogIfFxoStatusInfo *AnalogDAAStatusInfoPtr)
{
	GetAnalogDAAStatusEventInfo (CID, AnalogDAAStatusInfoPtr);
	AnalogDAAStatusInfoPtr->Connected = CurrentAnalogState[CID].line_connected;
 
	AnalogDAAStatusInfoPtr->BoardTemp = 0;
}
/*------------------------------------------------------------------------------------------
*Function Name: 
*	AnalogIFGetDaaLineParameters
*
*Description:
*	This function will return the DAA line parameters  
*
*Arguments:
*	int CID			- channel.
*	acAnalogIfDaaLinParametersInfo *AnalogDAALinParametersInfo
*Return Value:
*	void
*Remarks:
*
*----------------------------------------------------------------------------------------*/
void AnalogIFGetDaaLineParameters(int CID, acAnalogIfDaaLineParametersInfo *AnalogDAALineParametersInfo)
{
	
	if(AnalogIFDaaGetAnalogPortType(CID) == DAA)
	{	
		//AnalogDAALineParametersInfo->LineCurrent 		= (float)(daa_byte_read(28,CID) * 1.1);
		AnalogDAALineParametersInfo->LineVoltage 		= daa_byte_read(29,CID);
		AnalogDAALineParametersInfo->HookState 			= CurrentAnalogState[CID].hook;
		AnalogDAALineParametersInfo->RingState 			= CurrentAnalogState[CID].ring;
		AnalogDAALineParametersInfo->ConnectivityState 	= CurrentAnalogState[CID].line_connected;
		AnalogDAALineParametersInfo->PolarityState 		= (CurrentAnalogState[CID].hook)? CurrentAnalogState[CID].polarity:2;
		AnalogDAALineParametersInfo->LinePolarity 		= (daa_byte_read(29,CID) & 0x80) >> 7;
		AnalogDAALineParametersInfo->MWIState 			= CurrentAnalogState[CID].msg_wait;	
	}
	else
		DBG_PRINT("GetDaaLineParameters can be activated only on DAA port");
}
#if M1000
void PortBackToServiceDAA(unsigned long CID, unsigned long LastPortToInit,
						  unsigned long InitErr, unsigned long unused4)
{
	int ErrNum=0,TimerID;
	TimerUserParams Timer;

	if (CID < LastPortToInit)
	{
		
		WatchDogLongKickStart(25);
		if ((AnalogIFDaaGetAnalogPortType(CID) == DAA) || (AnalogIFDaaGetAnalogPortType(CID) == 9))
		{
			CurrentAnalogState[CID].hook = ON_HOOK;  //default to onhook
			CurrentAnalogState[CID].line_connected = ANALOG_LINE_CONNECTED;	//default to line connected
			CurrentAnalogState[CID].msg_wait = NO_MSG_WAIT;
			if ( (ErrNum = DAAInitPort (&AnicParams,CID,0)) != 0) 
			{
				DBG_PRINT("Init DAA port failed (DAAdriver error number %d), CID%d ", ErrNum, CID);
				InitErr++;
			}
			else
			{
				WriteDAAPortCoefficients(CID);
				InitDaaArrays(CID);
				HandleAppEvent(CID, acEV_ANALOG_IF_CONNECTED, NULL , 0,AnalogModule);
			}
		}
		CID++;
		if(CID < LastPortToInit)
		{
			Timer.Type = TIMER_THROWAWAY;
			Timer.FunctionParams[0]= CID;
			Timer.FunctionParams[1]= LastPortToInit;
			Timer.FunctionParams[2]= InitErr;
			Timer.ExpireFunction = PortBackToServiceDAA;
			Timer.PeriodMilli = 100;
			AllocateAndRunTimer(Timer,&TimerID);
		}
		else
		{
			if(!InitErr)
				ModuleBackToServiceAnalogEnded(ANALOG_PORT_TO_MODULE(CID-1), 1);
			else
				ModuleBackToServiceAnalogEnded(ANALOG_PORT_TO_MODULE(CID-1), 0);
		}
		WatchDogLongKickEnd();
	}
	else
	{
		if(!InitErr)
			ModuleBackToServiceAnalogEnded(ANALOG_PORT_TO_MODULE(CID), 1);
		else
			ModuleBackToServiceAnalogEnded(ANALOG_PORT_TO_MODULE(CID), 0);
	}
}

void ModuleOutOfServiceDAA(int Module)
{
	int i, CID;
	for(i=0;i<MODULE_NUMBER_OF_PORTS(Module);i++)
	{
		CID = MODULE_PORT_TO_ANALOG_PORT(Module,i);
		if (AnalogIFDaaGetAnalogPortType(CID) == DAA)
		{
			AnalogIFDaaGetAnalogPortType(CID) = 9;
			HandleAppEvent(CID, acEV_ANALOG_IF_OUT_OF_SERVICE, NULL, 0,AnalogModule);
			ClearDaaTimers(CID);
		}
	}
}
void ModuleBackToServiceDAA(int Module)
{
    int CID,LastPortToInit;
	
	InitAnalogModule(Module);

	CID = MODULE_PORT_TO_ANALOG_PORT(Module,0);
	LastPortToInit = CID + MODULE_NUMBER_OF_PORTS(Module);
	PortBackToServiceDAA(CID, LastPortToInit,0,0);
}




#endif


unsigned char  AnalogIFRingStatus(int CID)
{
    unsigned char data=0;
    unsigned char ringState = 0;
    
    if(AnalogPortType[CID]!=DAA) return ringState;
    data = (daa_byte_read(5, CID) & 0x04);
    switch(data)
    {
       case 0x04: 
    		ringState=1;
    		break;
    	case 0: 
    		ringState=0;
    		break;
    }	
    return ringState;
}

int AnalogIFDaaSeizeLine(int CID)
{
     CurrentAnalogState[CID].hook = OFF_HOOK;
    daa_byte_write(5, (daa_byte_read(5,CID) | 0x01), CID);	//turn on DAA control 1 bit 0
    return 0;
}

int AnalogIFDaaReleaseLine(int CID)
{
      CurrentAnalogState[CID].hook = ON_HOOK;
    daa_byte_write(5, daa_byte_read(5,CID) &~(0x01) , CID);	//turn off DAA control 1 bit 0
    return 0;
}
