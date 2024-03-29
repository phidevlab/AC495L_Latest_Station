/*
** Copyright ?2007 by Silicon Laboratories
**
** $Id: si321x_constants.c,v 1.3 2007/02/16 23:54:56 lajordan Exp $
**
**
** Author(s): 
** laj
**
** Distributed by: 
** Silicon Laboratories, Inc
**
** This file contains proprietary information.	 
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
**
*/
#include "proslic.h"




//initial values are defined using Si3210 indirect addresses
//the address is adjusted if a Si3215 is detected in the function in the 
//SPI driver (...proslic_api/example_drivers/win/proslic_ppt_mb/proslic_spiGci) 
//called posssibleAddressCorrect()
#define	INIT_IR0		0x55C2	// DTMF_ROW_0_PEAK	
#define	INIT_IR1		0x51E6  //	DTMF_ROW_1_PEAK,	
#define	INIT_IR2		0x4B85	//	DTMF_ROW2_PEAK,		
#define	INIT_IR3		0x4937	//	DTMF_ROW3_PEAK,	
#define	INIT_IR4		0x3333	//	DTMF_COL1_PEAK,	
#define	INIT_IR5		0x0202	//	DTMF_FWD_TWIST,
#define	INIT_IR6		0x0202	//	DTMF_RVS_TWIST,	
#define	INIT_IR7		0x0198	//	DTMF_ROW_RATIO,	
#define	INIT_IR8		0x0198	//	DTMF_COL_RATIO,		
#define	INIT_IR9		0x0611	//	DTMF_ROW_2ND_ARM,
#define	INIT_IR10		0x0202	//	DTMF_COL_2ND_ARM,	
#define	INIT_IR11		0x00E5	//	DTMF_PWR_MIN_,	
#define	INIT_IR12		0x0A1C	//	DTMF_OT_LIM_TRES,		
#define	INIT_IR13		0x7b30	//	OSC1_COEF,	
#define	INIT_IR14		0x0063	//	OSC1X,	
#define	INIT_IR15		0x0000	//	OSC1Y,	
#define	INIT_IR16		0x7870	//	OSC2_COEF,	
#define	INIT_IR17		0x007d	//	OSC2X,	
#define	INIT_IR18		0x0000	//	OSC2Y,	
#define	INIT_IR19		0x0000	//	RING_V_OFF,
#define	INIT_IR20		0x07E6C	//	RING_OSC,      ACL NB 080604 - VI64425,	
#define	INIT_IR21		0x0241	//	RING_X,	      ACL NB 080604 - VI64425,
#define	INIT_IR22		0x0000	//	RING_Y,	
#define	INIT_IR23		0x2000	//	PULSE_ENVEL,	
#define	INIT_IR24		0x2000	//	PULSE_X,
#define	INIT_IR25		0x0000	//	PULSE_Y,	
#define	INIT_IR26		0x4000	//	RECV_DIGITAL_GAIN,	
#define	INIT_IR27		0x4000	//	XMIT_DIGITAL_GAIN,	
#define	INIT_IR28		0x1000	//	LOOP_CLOSE_TRES,	
#define	INIT_IR29		0x3600	//	RING_TRIP_TRES,	
#define	INIT_IR30		0x1000	//	COMMON_MIN_TRES,	
#define	INIT_IR31		0x080	//	COMMON_MAX_TRES,	
	
#define	INIT_IR35		0x8000	//	LOOP_CLSRE_FlTER,
#define	INIT_IR36		0x0320	//	RING_TRIP_FILTER,	
	
#define	INIT_IR40		0x200  	//	CM_BIAS_RINGING,	
#define	INIT_IR41		0xC00	//	DCDC_MIN_V,	
#define	INIT_IR42		0x0	    
#define	INIT_IR43		0xE00	//	"LOOP_CLOSE_TRES Low
#define	INIT_IR99		0x00DA	// FSK 0 FREQ PARAM
#define	INIT_IR100		0x6B60	// FSK 0 AMPL PARAM
#define	INIT_IR101		0x0074	// FSK 1 FREQ PARAM
#define	INIT_IR102		0x79C0	// FSK 1 AMPl PARAM
#define	INIT_IR103		0x1120	// FSK 0to1 SCALER
#define	INIT_IR104		0x3BE0	// FSK 1to0 SCALER
#define	INIT_IR97		0x0000	// TRASMIT_FILTER
#define INIT_SI3210M_DR92 0x60  //  92 0x60 Initialization DC�DC Converter PWM Period (61.035 ns/LSB)
#define INIT_SI3210M_DR93 0x38  //  92 0x60 Initialization DC�DC Converter PWM Period (61.035 ns/LSB)
#define	INIT_DR92	0x7f	//	92 0x5C  7F Initialization DC�DC Converter PWM Period (61.035 ns/LSB)
#define	INIT_DR93	0x14	//	93 0x5D 0x14 0x19 Initialization DC�DC Converter Min. Off Time (61.035 ns/LSB)
#define	INIT_IR32		0x7c0  	//	PWR_ALARM_Q1Q2,		
#define	INIT_IR33		0x2600	//	PWR_ALARM_Q3Q4,
#define	INIT_IR34		0x1B80	//	PWR_ALARM_Q5Q6,	
#define	INIT_IR37		0x08c	//	TERM_LP_POLE_Q1Q2,	
#define	INIT_IR38		0x0100	//	TERM_LP_POLE_Q3Q4,	
#define	INIT_IR39		0x010	//	TERM_LP_POLE_Q5Q6,


static const ProslicRegInit regInitTable [] = {
	{1, INIT_DR1},
	{34, INIT_DR34},
	{48, INIT_DR48},
	{49, INIT_DR49},
	{50, INIT_DR50},
	{51, INIT_DR51},
	{65, INIT_DR65},		//	External Bipolar Transistor Setting. Changed default value (AN35)  
	{71, INIT_DR71},
	{74, INIT_DR74}, //ACL NB 080604 VI64425
	//{63, INIT_DR63},
	//{67, INIT_DR67},
	//{69, INIT_DR69},
	//{70, INIT_DR70},
	{0xFF,0xFF} //end flag
};

static const ProslicRAMInit ramInitTable [] = { //see si321x_registers.h for definitions
	{0, INIT_IR0},	
	{1,INIT_IR1},	
	{2,INIT_IR2},	
	{3,INIT_IR3},	
	{4,INIT_IR4},	
	{5,INIT_IR5},	
	{6,INIT_IR6},	
	{7,INIT_IR7},	
	{8,INIT_IR8},	
	{9,INIT_IR9},	
	{10,INIT_IR10},	
	{11,INIT_IR11},
	{12,INIT_IR12},	
	{13,INIT_IR13},	
	{14,INIT_IR14},	
	{15,INIT_IR15},
	{16,INIT_IR16},	
	{17,INIT_IR17},	
	{18,INIT_IR18},	
	{19,INIT_IR19},	
	{20,INIT_IR20},	
	{21,INIT_IR21},	
	{22,INIT_IR22},	
	{23,INIT_IR23},
	{24,INIT_IR24},	
	{25,INIT_IR25},	
	{26,INIT_IR26},	
	{27,INIT_IR27},	
	{28,INIT_IR28},	
	{29,INIT_IR29},
	{30,INIT_IR30},	
	{31,INIT_IR31},	
	{32,INIT_IR32},
	{33,INIT_IR33},	
	{34,INIT_IR34},
	{35,0x8000},	
	{36,0x8000},	
	{37,0x8000},	
	{38,0x8000},	
	{39,0x8000},	
	{40,INIT_IR40},
	{41,INIT_IR41},	
	{43,INIT_IR43},
	{99,INIT_IR99},	
	{100,INIT_IR100},	
	{101,INIT_IR101},	
	{102,INIT_IR102},	
	{103,INIT_IR103},	
	{104,INIT_IR104},	
	{0xFFFF, 0xFFFF} //end flag
};

static const ProslicRAMInit ramInitTable2 [] = {	
	{35,INIT_IR35},	//	0x8000	LOOP_CLSRE_FlTER
	{36,INIT_IR36},	//	0x0320	RING_TRIP_FILTER
	{37,INIT_IR37},	//	0x08c	TERM_LP_POLE_Q1Q2
	{38,INIT_IR38},	//	0x0100	TERM_LP_POLE_Q3Q4
	{39,INIT_IR39},	//	0x0010	TERM_LP_POLE_Q5Q6
	{0xFFFF, 0xFFFF} //end flag
};

/*
** Tone Generator configuration
*/
#define OSC_TOPHONE 0x2
#define OSC_350HZ_MINUS18DBM_8KHZ	0x7b30,0x63,0
#define OSC_350HZ_MINUS18DBM_16KHZ	0x7ed0,0x31,0
#define OSC_NOTIME				0,0
#define OSC_0_5SEC				0xa0,0xf
#define OSC_0_3SEC				0x60,0x9
#define OSC_0_2SEC				0x40,0x6
#define OSC_4SEC				0x0,0x7d
#define	OSC_2SEC				0x80,0x3e
#define OSC_440HZ_MINUS18DBM_8KHZ	0x7870,0x7d,0 
#define OSC_440HZ_MINUS18DBM_16KHZ	0x7e20,0x3d,0 
#define OSC_480HZ_MINUS18DBM_8KHZ	0x7700,0x89,0
#define OSC_480HZ_MINUS18DBM_16KHZ	0x7dc0,0x43,0
#define OSC_620HZ_MINUS18DBM_8KHZ	0x7120,0xb2,0
#define OSC_620HZ_MINUS18DBM_16KHZ	0x7c40,0x57,0

//Si3210 Tone generator (8kHz)
Si321x_Tone_Cfg Si321x_Tone_Presets [] = {
{{OSC_350HZ_MINUS18DBM_8KHZ,OSC_NOTIME,OSC_NOTIME},{OSC_440HZ_MINUS18DBM_8KHZ,OSC_NOTIME,OSC_NOTIME},OSC_TOPHONE,OSC_TOPHONE},
{{OSC_480HZ_MINUS18DBM_8KHZ,OSC_0_5SEC,OSC_0_5SEC},{OSC_620HZ_MINUS18DBM_8KHZ,OSC_0_5SEC,OSC_0_5SEC},OSC_TOPHONE,OSC_TOPHONE},
{{OSC_480HZ_MINUS18DBM_8KHZ,OSC_0_2SEC,OSC_0_3SEC},{OSC_620HZ_MINUS18DBM_8KHZ,OSC_0_2SEC,OSC_0_3SEC},OSC_TOPHONE,OSC_TOPHONE},
{{OSC_480HZ_MINUS18DBM_8KHZ,OSC_2SEC,OSC_4SEC},{OSC_440HZ_MINUS18DBM_8KHZ,OSC_2SEC,OSC_4SEC},OSC_TOPHONE,OSC_TOPHONE},
{{OSC_480HZ_MINUS18DBM_8KHZ,OSC_0_3SEC,OSC_0_2SEC},{OSC_620HZ_MINUS18DBM_8KHZ,OSC_0_3SEC,OSC_0_2SEC},OSC_TOPHONE,OSC_TOPHONE}
};

Si321x_Tone_Cfg Si3215_Tone_Presets [] = {
{{OSC_350HZ_MINUS18DBM_16KHZ,OSC_NOTIME,OSC_NOTIME},{OSC_440HZ_MINUS18DBM_16KHZ,OSC_NOTIME,OSC_NOTIME},OSC_TOPHONE,OSC_TOPHONE},
{{OSC_480HZ_MINUS18DBM_16KHZ,OSC_0_5SEC,OSC_0_5SEC},{OSC_620HZ_MINUS18DBM_16KHZ,OSC_0_5SEC,OSC_0_5SEC},OSC_TOPHONE,OSC_TOPHONE},
{{OSC_480HZ_MINUS18DBM_16KHZ,OSC_0_2SEC,OSC_0_3SEC},{OSC_620HZ_MINUS18DBM_16KHZ,OSC_0_2SEC,OSC_0_3SEC},OSC_TOPHONE,OSC_TOPHONE},
{{OSC_480HZ_MINUS18DBM_16KHZ,OSC_2SEC,OSC_4SEC},{OSC_440HZ_MINUS18DBM_16KHZ,OSC_2SEC,OSC_4SEC},OSC_TOPHONE,OSC_TOPHONE},
{{OSC_480HZ_MINUS18DBM_16KHZ,OSC_0_3SEC,OSC_0_2SEC},{OSC_620HZ_MINUS18DBM_16KHZ,OSC_0_3SEC,OSC_0_2SEC},OSC_TOPHONE,OSC_TOPHONE}
};

Si321x_Ring_Cfg Si321x_Ring_Presets [] = {
	{0xfb,0,0,0x7f00,0x80,0x3e,0x00,0x7d,0x1e}
};

Si321x_FSK_Cfg Si321x_FSK_Presets [] = {
	{0x1110,0x3c00,0x6b60,0x79c0,0x1b4,0xe9}
};

Si321x_FSK_Cfg Si3215_FSK_Presets [] = {
	{0x1110,0x3c00,0x6b60,0x79c0,0x1b4,0xe9}
};

#define ZSYNTH_600				0
#define ZSYNTH_900				1
#define ZSYNTH_600_1UF			2
#define ZSYNTH_900_2_16UF		3
#define ZSYNTH_CTR21			4
#define ZSYNTH_220_820_120NF	5
#define ZSYNTH_220_820_115NF	6
#define ZSYNTH_200_680_100NF	7
#define ZSYNTH_ENABLED			8

#define ZSYNTH_NO_LINE_CAP_COMP 0
#define ZSYNTH_LINE_CAP_4_7NF   0x10
#define ZSYNTH_LINE_CAP_10NF	0x20


//NIRP 070628 adding country name to the structure
Si321x_Zsynth_Cfg Si321x_Zsynth_Presets [] = {
	{ZSYNTH_LINE_CAP_10NF,ZSYNTH_600|ZSYNTH_ENABLED, "USA"},
	{ZSYNTH_LINE_CAP_10NF,ZSYNTH_600|ZSYNTH_ENABLED, "China"},
	{ZSYNTH_LINE_CAP_10NF,ZSYNTH_600|ZSYNTH_ENABLED, "Netherlands"},
	{ZSYNTH_LINE_CAP_10NF,ZSYNTH_900|ZSYNTH_ENABLED, "Brazil"},
	{ZSYNTH_LINE_CAP_10NF,ZSYNTH_CTR21|ZSYNTH_ENABLED, "Israel"},
	{ZSYNTH_LINE_CAP_10NF,ZSYNTH_CTR21|ZSYNTH_ENABLED, "Italy"},
	{ZSYNTH_LINE_CAP_10NF,ZSYNTH_CTR21|ZSYNTH_ENABLED, "UK"},
	{ZSYNTH_LINE_CAP_10NF,ZSYNTH_220_820_120NF|ZSYNTH_ENABLED, "France"},
	{ZSYNTH_LINE_CAP_10NF,ZSYNTH_220_820_115NF|ZSYNTH_ENABLED, "Germany"},
	{-1,-1, NULL}
};

#define AUDIOGAIN_0DB			0
#define AUDIOGAIN_3_5DB			2
#define AUDIOGAIN_MINUS3_5DB	1
#define AUDIOGAIN_MUTE			3

Si321x_audioGain_Cfg Si321x_AudioGain_Presets [] = {
	{AUDIOGAIN_0DB},
	{AUDIOGAIN_MINUS3_5DB},
	{AUDIOGAIN_3_5DB},
	{AUDIOGAIN_MUTE}
};

Si321x_DCfeed_Cfg Si321x_Dcfeed_Presets [] = {
	{0,0,0}
};

Si321x_PulseMeter_Cfg Si321x_PulseMeter_Presets [] = {
	{0,0,0,0,0}
};

#define ALAW			0
#define ULAW			1
#define LINEAR_8BIT		2
#define LINEAR_16BIT	3

Si321x_PCM_Cfg Si321x_Pcm_Presets [] = {
	{0,0,ALAW<<3,0,0}/*NIRP 070415 default is now ALAWnot ULAW*/
};
	
