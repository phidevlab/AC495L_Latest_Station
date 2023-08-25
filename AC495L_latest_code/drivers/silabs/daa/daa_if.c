#include <linux/config.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/ioport.h>
#include <linux/fcntl.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/proc_fs.h>
#include <asm/io.h>
#include <asm/uaccess.h>
//#include <asm/system.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/devfs_fs_kernel.h>
#include <linux/spinlock.h> 
//#include <linux/wait.h>
//Linux 2.6 only
#include <linux/cdev.h>
#include <pal.h>

//#include <asm/avalanche/generic/pal_vlynq.h>
//#include <asm/avalanche/generic/pal_vlynqIoctl.h>


#include "AnalogDriver.h"
#include "daa_if.h"
#include "daa_driver.h"
#include "../../ac49x_mpi/mpi_drv.h"
#include "../dsp_debug_cmd.h"
#include "../silabs_def.h"

/*------------------------------*/
/* static variables definitions */
/*------------------------------*/

extern int AnalogPortType[NUM_DAA_LINES];
int AnalogIFDaaGetAnalogPortType(int CID)
{
    return AnalogPortType[CID];
}


/************************************************************************************
Function Name: 
	GetDAAInterruptsStatus

Description:
	Find out if a port has produced an interrupt

Arguments:
	int port			- daa port.

Return Value:
	0 if success.
	Otherwise if failure.

Remarks:
   
*************************************************************************************/
int GetDAAInterruptsStatus(int CID)
{
	int int_mask=1;
#if MEDIAPACK_118
	PDA	*GIoMem = (PDA *)(GetIMMR() & IO_MAP_MASK);
	unsigned TempRegVal;
	TempRegVal = ((GIoMem->pcmcia_pipr &0xff000000)>>24);
#endif

#if M1000
	/* each device has it's own INT, since each 2 devices INT are handled together both INT lines should be checked */  
	switch(ANALOG_PORT_TO_MODULE_PORT(CID))
	{
	case 0: 
		int_mask = (MODULE_REGISTER(ANALOG_PORT_TO_MODULE(CID) ,MODULE_INT_GA_REGISTER) & CODEC0_INT);	/* channel 0 */
		if (int_mask != 0)		/* no INT in first channel */
		{
			int_mask = (MODULE_REGISTER(ANALOG_PORT_TO_MODULE(CID) ,MODULE_INT_GA_REGISTER) & CODEC1_INT);	/* channel 1 */
		}
		break;
	case 2: 
		int_mask = (MODULE_REGISTER(ANALOG_PORT_TO_MODULE(CID) ,MODULE_INT_GA_REGISTER) & CODEC2_INT);	/* channel 2 */
		if (int_mask != 0)		/* no INT in first channel */
		{
			int_mask = (MODULE_REGISTER(ANALOG_PORT_TO_MODULE(CID) ,MODULE_INT_GA_REGISTER) & CODEC3_INT);	/* channel 3 */
		}
		break;
	}

#elif MEDIAPACK_118
	/* in case of MP114 MIX CID 2 & 3 are connected to INT 4 &5 */
  switch(CID)
  {
  case 0:	
  case 1: 
		if (GIoMem->siu_sipend & ANALOG_PORT_0_1_INT)
				int_mask = 0;										/* channel 1 and 2 */
		break;
  case 2:	
  case 3: 
		if(((TempRegVal & 0xf0)>>4) == FXS_PORT_2_FXO_PORT_2)
		{
			if (GIoMem->siu_sipend & ANALOG_PORT_4_5_INT)
				int_mask = 0;										/* channel 3 and 4 */	
		}
		else
		{
		if (GIoMem->siu_sipend & ANALOG_PORT_2_3_INT)
				int_mask = 0;										/* channel 3 and 4 */
		}
		break;
  case 4:	
  case 5:
		if(((TempRegVal & 0xf0)>>4) == FXS_PORT_2_FXO_PORT_2);
		else
		{
		if (GIoMem->siu_sipend & ANALOG_PORT_4_5_INT)
				int_mask = 0;										/* channel 3 and 4 */
		}
		break;
  case 6:	
  case 7: 
		if (GIoMem->siu_sipend & ANALOG_PORT_6_7_INT)
				int_mask = 0;										/* channel 7 and 8 */
		break;
  }

#endif

  /* interrupt is pend if mask == 0 */
  return ((int_mask != 0)? FALSE:TRUE);
}

/*==============================================================//
// DAAReset - perform reset of the DAA 							//
//									                            //
//                                                              //
//--------------------------------------------------------------//*/
void DAAReset(void)
{
	int ResetDone=0;
	int data;
	int i, rest_count;
	
	volatile PIN_SEL_REG_ARRAY_T *pin_sel_array = (PIN_SEL_REG_ARRAY_T*) AVALANCHE_PIN_SEL_BASE;//by anson

	for(i=0; i<NUM_DAA_LINES; i++)
		AnalogPortType[i] = IN_VALID;
                 
	for(rest_count=0; (rest_count<10)&&(!ResetDone); rest_count++)
	{
#if defined(CONFIG_TULIP) || defined(CONFIG_SMB)
		pin_sel_array->reg[14-1] |=  0x3000;	//by anson
		printk("reg13=0X%X\n", pin_sel_array->reg[13-1]);
#elif defined(CONFIG_ORCHID) || defined(CONFIG_ORCHID_ATA)
		printk("reg9=0X%X\n", pin_sel_array->reg[9-1]);
		printk("reg14=0X%X\n", pin_sel_array->reg[14-1]);
		printk("reg15=0X%X\n", pin_sel_array->reg[15-1]);
#endif
	
		PAL_sysGpioCtrl(SILABS_RESET, GPIO_PIN, GPIO_OUTPUT_PIN);
#if defined(CONFIG_TULIP) || defined(CONFIG_SMB) || defined(CONFIG_ORCHID) || defined(CONFIG_ORCHID_ATA)
		PAL_sysGpioCtrl(SILABS_MODE, GPIO_PIN, GPIO_OUTPUT_PIN);//by anson
#endif

		for(i=0; i<NUM_DAA_LINES; i++)
		{
			PAL_sysGpioCtrl(CHIP_CS(i), GPIO_PIN, GPIO_OUTPUT_PIN);
			PAL_sysGpioOutBit(CHIP_CS(i), DAA_UNSELECTED);//make sure no CS to FXO (via V5 logic)
		}
              
		PAL_sysGpioOutBit(SILABS_RESET, 0);//assert the reset
#if defined(CONFIG_TULIP) || defined(CONFIG_SMB) || defined(CONFIG_ORCHID) || defined(CONFIG_ORCHID_ATA)
		PAL_sysGpioOutBit(SILABS_MODE, SILABS_DAA_SELECTED);//ENABLE DAA CONTROL by anson
#endif
		udelay(10);/*reset delay - should be at least 10 PCLK cycles( setting it to 20 cycles which are 10us) */
		PAL_sysGpioOutBit(SILABS_RESET ,1);//release the reset
        //dummy SPI read to verify succusfull reset
		//PAL_sysGpioOutBit(44, 0);//when MPI CS is asserted - it will be sent to FXO
        
		for(i=0; i<NUM_DAA_LINES; i++)
		{
			udelay(5000);
			data =  daa_byte_read(1,i);
			if(0 == data)
			{
				AnalogPortType[i] = DAA;
				printk("silabs card %d is detected\n", i);
				ResetDone=1;	
			}			
		}      
	}
        
	printk("DAAReset: it took %d attempts to reset the device \n", rest_count);
	return;
}

 int IsValidDaaPort(int CID)
 {
    if((CID>=0)&&(CID<NUM_DAA_LINES)&&(AnalogPortType[CID]==DAA)) 
           return 1;
    else
          return 0;
 }

/*==============================================================//
// DAAInit - Initiates the DAA Chip.							//
//									                            //
//                                                              //
//--------------------------------------------------------------//*/
int DAAInit(struct TAnicParams *AnicParam,int IsEnableMediaSecurity) /*for ANIC TEST add  (struct TAnicParams *AnicParam)*/
{
	int CID;
	DAAReset();
         for (CID=0; CID<NUM_DAA_LINES; CID++)		
        {
            DAAInitPort(AnicParam,CID,IsEnableMediaSecurity);
        }

        InitDAATimer();
  return (0);
}

/*==============================================================//
// DAAInitPort - Initiates the DAA single port Chip.			//
//									                            //
//                                                              //
//--------------------------------------------------------------//*/
int DAAInitPort(struct TAnicParams *AnicParam,int CID,int IsEnableMediaSecurity) 
{
	unsigned char WriteReg;

	printk("DAAInitPort: CID=%d, AnalogPortType[CID]=%d,   \n", CID, AnalogPortType[CID]);
    if (AnalogPortType[CID] == DAA)
	{

		udelay(75);
		daa_byte_write(1, 0x80 , CID);								/*put power in normal operation */
		udelay(100);												/* short delay after reset      */		
		daa_byte_write(6, 0x00 , CID);								/*put power in normal operation */

		WriteReg = 0x00;
		WriteReg |= 0x21;						/* enable PCM */
		switch (AnicParam->PCMLawSelect)
		{
		case 3:
			WriteReg |= 0x08;					/*PCM Configuration register Ulaw*/
			WriteReg &= ~(0x10);
			break;
		case 1:	
			WriteReg &= ~(0x18);				/*PCM Configuration register Alaw*/
			break;
		default: 
			WriteReg &= ~(0x18);				/*PCM Configuration register Alaw*/
			break;
		}
		daa_byte_write(33, WriteReg , CID);		/* PCM/SPI mode select */							
		
		daa_byte_write(26, 0x00 , CID);			/*dc termination					*/
		daa_byte_write(30, 0x00 , CID);			/* AC termination control amir */
		
		daa_byte_write(7, 0x00 , CID);			/*sample rate 8Khz*/
		daa_byte_write(15, 0x00 , CID);			/*receive and transmit signals are not muted*/
		
		/* INT amir */
		daa_byte_write(2, 0x87 , CID);			/* enable INT pin, Tx and Rx path */
		daa_byte_write(5, 0x08 , CID);			/*put channel in onhook and allow CallerID*/
		
		daa_byte_write(3, E_DAA_RING | E_DAA_POLARITY_REVERSAL , CID);		/*enabel Ring and polarity reversal INT*/
		
		/* ring 15-66Hz, min cadence 300 on, 300 off*/
		daa_byte_write(16, 0x00 , CID);			/*international control 1 - put power in normal operation*/
		daa_byte_write(17, 0x04 , CID);			/*international control 2 - enable billing tone detection*/
		daa_byte_write(18, 0x02 , CID);			/*international control 3 NIRP 071115 RFWE=1 enable ring detector full wave*/
		daa_byte_write(22, 0x0F , CID);			/*ring vaidation control 1*/
		daa_byte_write(23, 0x13 , CID);			/*ring vaidation control 2*/
		daa_byte_write(24, 0x92 , CID);			/*ring vaidation control 3*/
		
		
		
		/* gain settings amir */
		daa_byte_write(20, 0x00 , CID);			/*Call progress receive attenuation	*/
		daa_byte_write(21, 0x00 , CID);			/*Call progress transmit attenuation*/
		daa_byte_write(25, 0x00 , CID);			/*resistor calibration				*/
		daa_byte_write(38, 0x16/*NIRP orig 0x00*/, CID);			/*transmit gain control 2			*/
		daa_byte_write(39, 0x00 , CID);			/*recive gain control 2				*/
		daa_byte_write(40, 0x00 , CID);			/*transmit gain control 3			*/
		daa_byte_write(41, 0x00 , CID);			/*recive gain control 3				*/
		daa_byte_write(45, 0x00 , CID);			/*programmable hybrid register 1	*/
		daa_byte_write(46, 0x00 , CID);			/*programmable hybrid register 2	*/
		daa_byte_write(47, 0x00 , CID);			/*programmable hybrid register 3	*/
		daa_byte_write(48, 0x00 , CID);			/*programmable hybrid register 4	*/
		daa_byte_write(49, 0x00 , CID);			/*programmable hybrid register 5	*/
		daa_byte_write(50, 0x00 , CID);			/*programmable hybrid register 6	*/
		daa_byte_write(51, 0x00 , CID);			/*programmable hybrid register 7	*/
		daa_byte_write(52, 0x00 , CID);			/*programmable hybrid register 8	*/
		   
		/* analog settings */
		daa_byte_write(31, 0xa2 , CID);			/*DAA control 5	VI 63262 DC offset issue solved with SILABA instructions	*/
		daa_byte_write(32, 0x03 , CID);			/*ground start control				*/


#if defined(CONFIG_TULIP2_21) || defined(CONFIG_TULIP2_40)
        //NIRP 071202 timeslot configuration
        if (0 == NUM_DAA_LINES)
        {//DAA is configured to make sure there is no PCM noise. Timeslot is beyond the relevant scale
		    daa_byte_write(34, 0x29,      CID);		/*PCM transmit start count -low byte*/
		    daa_byte_write(35, 0x00 ,   CID);		/*PCM transmit start count -high byte*/
		    daa_byte_write(36, 0x29,      CID);		/*PCM receive start count -low byte	*/
		    daa_byte_write(37, 0x00 ,   CID);		/*PCM receive start count -high byte*/
        }

        //NIRP 071202 timeslot configuration
        if ((1 == NUM_DAA_LINES) && (1 == NUM_SLIC_LINES))
        {   //NIRP 071202 - see ac494.c for time slot allocation - FXO is connected to DSP channel 2 (3-d timeslot)
		    daa_byte_write(34, 0x09,    CID);		/*PCM transmit start count -low byte*/
		    daa_byte_write(35, 0x00 ,   CID);		/*PCM transmit start count -high byte*/
		    daa_byte_write(36, 0x09,    CID);		/*PCM receive start count -low byte	*/
		    daa_byte_write(37, 0x00 ,   CID);		/*PCM receive start count -high byte*/
        }
        //NIRP 080506 timeslot configuration for MP-203
        if ((1 == NUM_DAA_LINES) && (2 == NUM_SLIC_LINES))
        {   //NIRP 071202 - see ac494.c for time slot allocation - FXO is connected to DSP channel 2 (3-d timeslot)
		    daa_byte_write(34, 0x41,    CID);		/*PCM transmit start count -low byte*/
		    daa_byte_write(35, 0x00 ,   CID);		/*PCM transmit start count -high byte*/
		    daa_byte_write(36, 0x41,    CID);		/*PCM receive start count -low byte	*/
		    daa_byte_write(37, 0x00 ,   CID);		/*PCM receive start count -high byte*/
        }
#else
		daa_byte_write(34, 9+16*CID,      CID);		/*PCM transmit start count -low byte*/
		daa_byte_write(35, 0x00 ,   CID);		/*PCM transmit start count -high byte*/
		daa_byte_write(36, 9+16*CID,      CID);		/*PCM receive start count -low byte	*/
		daa_byte_write(37, 0x00 ,   CID);		/*PCM receive start count -high byte*/
#endif

#if M1000
		daa_byte_write(34, ANALOG_PORT_TO_MODULE_PORT(CID)*8, CID);		/*PCM transmit start count -low byte*/
		daa_byte_write(35, 0x00 , CID);									/*PCM transmit start count -high byte*/
		daa_byte_write(36, ANALOG_PORT_TO_MODULE_PORT(CID)*8, CID);		/*PCM receive start count -low byte	*/
		daa_byte_write(37, 0x00 , CID);									/*PCM receive start count -high byte*/
#endif
		daa_byte_write(43, 0x50 , CID);			/*line current/voltage INT threshold*/
		daa_byte_write(44, 0x07 , CID);			/*line current/voltage INT control: current,int below thr	*/
		daa_byte_write(59, 0x00 , CID);			/*spark quenching control			*/		
		
		if(IsEnableMediaSecurity == 1)	/* in case of SRTP each DSP handle 3 channels, all analog ports without dsp r blocked */
		{
#if MEDIAPACK_118
			AnalogPortNum = GetAnalogPortNumber();
			if(((CID == 3) && (AnalogPortNum == 4)) || (((CID == 6) ||(CID == 7)) && (AnalogPortNum == 8)))
			{
				AnalogPortType[CID] = 9;
				BSP_ErrorHandler(NoticeMsg,0,__FILE__,__LINE__,-1,"Analog port %d is not active while Media security is activated",CID);
			}
#elif M1000
			if((ANALOG_PORT_EXIST(CID)) && (MODULE_NUMBER_OF_PORTS(ANALOG_PORT_TO_MODULE(CID)) == 4) && (ANALOG_PORT_TO_MODULE_PORT(CID) == 3))
			{
				AnalogPortType[CID] = 9;
				LIGHT_PORTS_LEDS(CID, RED);
				BSP_ErrorHandler(NoticeMsg,0,__FILE__,__LINE__,-1,"Analog port %d is not active while Media security is activated",CID);
			}
#endif
		}

	}
	
	return (0);
}


/*--------------------------------------------------------------------
 * ROUTINE:   SetDAACS
 *
 * DESCRIPTION:
 *   Set the Daa chip select.
 *	 
 *--------------------------------------------------------------------*/
void SetDAACS(unsigned char state,int CID)
{

	int cs = 0;

	cs = CHIP_CS(CID);
    if(cs)
    {
#if defined(CONFIG_TULIP) || defined(CONFIG_SMB) || defined(CONFIG_ORCHID) || defined(CONFIG_ORCHID_ATA)
		PAL_sysGpioOutBit(SILABS_MODE,  SILABS_DAA_SELECTED);		
#endif
		if(state==CS_UP)
			PAL_sysGpioOutBit(cs, DAA_SELECTED);
		else
			PAL_sysGpioOutBit(cs, DAA_UNSELECTED);
	}

	return;
}



/************************************************************************************
Function Name:
	AnalogDaaSetHook
	
Description:
	This function changes the hook state of a channel connected to an analog DAA port.

Arguments:
	int CID									- Channel number
	TAnalogIFHookState AnalogIFHookState	- New hook state to apply (either
											  ON_HOOK or OFF_HOOK).
Return Value:
	0 if success.
	Otherwise if failure.

Remarks:
   none
*************************************************************************************/

int AnalogDaaSetHook(int CID, int AnalogHookStateVal)
{
	if ( AnalogHookStateVal == ANALOG_ON_HOOK ) 
	{
#if (MEDIAPACK_118 || M1000)
		LIGHT_PORTS_LEDS(CID,OFF);	
#endif	
		daa_byte_write(5,0x08,CID);												/*onhook*/
		return(0);
	}
	else
	if ( AnalogHookStateVal == ANALOG_OFF_HOOK )
	{
#if (MEDIAPACK_118 || M1000)
		LIGHT_PORTS_LEDS(CID,GREEN);	
#endif
		daa_byte_write(15,daa_byte_read(15,CID) | (0x80),CID);					/*cut of transmit path*/
		daa_byte_write(5,0x01,CID);	/*off-hook conversation*/
		daa_byte_write(15,daa_byte_read(15,CID) & ~(0x80),CID);					/*give back transmit path*/

		return(0);
	}
	else
		return(1);
}


/************************************************************************************
Function Name: 
	GetAnalogDAAStatusEventInfo

Description:
	Get the Status

Arguments:
	int CID			- channel.

Return Value:
	0 if success.
	Otherwise if failure.

Remarks:
   
*************************************************************************************/
int GetAnalogDAAStatusEventInfo (int CID, struct acTEvAnalogIfFxoStatusInfo *AnalogAnicStatusPtr)
{
	struct acTEvAnalogIfFxoStatusInfo *AnalogAnicStatusVal;
	AnalogAnicStatusVal = (struct acTEvAnalogIfFxoStatusInfo*) AnalogAnicStatusPtr;

	AnalogAnicStatusVal->AnicInternalState = ((int)daa_byte_read(5,CID) & 0x01);	/*Daa state (on/off hook)*/
	AnalogAnicStatusVal->HwSwVersion = (int)daa_byte_read(11,CID);					/*H/W versions*/
	AnalogAnicStatusVal->Connected = 0;
	AnalogAnicStatusVal->DualUse = 0;
	AnalogAnicStatusVal->OutOfService = 0;
	AnalogAnicStatusVal->SeizedLine = 0;
	AnalogAnicStatusVal->RemoteDisconnect = 0;
	AnalogAnicStatusVal->PolarityStatus = (daa_byte_read(29,CID) & 0x80) >> 7;

	return(0);
}

/************************************************************************************
Function Name: 
	DaaLoopCommand

Description:DaaLoopCommand
	This function will activate the internal loop back of the DAA.

Arguments:
	[i] int CID
	[i] enum TAnalogLoopCommand  AnalogLoopCommand)


Return Value:
	0 if success.
	Otherwise if failure.

Remarks:
   none.

*************************************************************************************/

int DaaLoopCommand(int CID, int AnalogLoopCommandVal)
{
	
	switch(AnalogLoopCommandVal)
	{
/* yaniv
	case Stop_Loop: / *End tests* /
		anic_byte_write(0,10,anic_byte_read(0,10,CID) & ~(0x24), CID);
		anic_byte_write(96,14,anic_byte_read(96,14,CID) & ~(0x04), CID);
		return(0);

	case PcmTest: / *PCM loop* /
		anic_byte_write(0,10,anic_byte_read(0,10,CID) | 0x20, CID);
		return(0);

	case AnicDTest: / *ANICD test* /
		anic_byte_write(96,14,anic_byte_read(96,14,CID) | 0x04, CID); 
		return(0);

	case AnicATest: / *ANICA test* /
		anic_byte_write(0,10,anic_byte_read(0,10,CID) | 0x04, CID);
		return(0);

	default:
		return(1);
*/
	}
	return 0;

}


/************************************************************************************
Function Name: 
	AnalogDaaRegisterCommand

Description:
	This function will change DAA registers

Arguments:
	[i] int CID
	[i] enum TAnalogLoopCommand  AnalogLoopCommand)


Return Value:
	0 if success.
	Otherwise if failure.

Remarks:
   none.

*************************************************************************************/

int AnalogDaaRegisterCommand(int CID,unsigned char address, unsigned char value) 
{
	daa_byte_write(address, value, CID);
	return(0);
}

unsigned char AnalogDaaReadRegisterCommand(int CID,unsigned char address) 
{
  int i;
  unsigned char reg;

	if (address == 0xff)
	  {
		for (i=1; i<60; i++)
		  {
			reg = daa_byte_read(i,CID);
			printk("I 0, O %d : %x\n",i,reg);
			mdelay(50);
		  }		
	  }
		
	return(daa_byte_read(address,CID));
}



/****************************************************************************
   Description:
         Returns Chip Revision of DAA
                                                                                                
  Arguments:
           
  Return:
         int  -  Content of register 11
                                     
  Remarks:
        
 ***************************************************************************/

int GetDaaChipRevision(int CID)
{
	return(daa_byte_read(11,CID));
}

int BitInReg(int Bit_Number, unsigned char Register)

{

            return(Register & (0x01 << Bit_Number));

}




 
