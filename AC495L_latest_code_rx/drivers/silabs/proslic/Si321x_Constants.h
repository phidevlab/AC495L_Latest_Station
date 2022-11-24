/*
** Copyright © 2007 by Silicon Laboratories
**
** $Id: Si321x_Constants.h,v 1.6 2007/02/16 23:54:56 lajordan Exp $
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

#define INIT_SI3210M_DR92 0x60  //  92 0x60 Initialization DC–DC Converter PWM Period (61.035 ns/LSB)
#define INIT_SI3210M_DR93 0x38  //  92 0x60 Initialization DC–DC Converter PWM Period (61.035 ns/LSB)
#define	INIT_DR92	0x7f	//	92 0x5C  7F Initialization DC–DC Converter PWM Period (61.035 ns/LSB)
#define	INIT_DR93	0x14	//	93 0x5D 0x14 0x19 Initialization DC–DC Converter Min. Off Time (61.035 ns/LSB)

enum {
	USA_DIAL_TONE,
	USA_BUSY_TONE,
	USA_CONGESTION_TONE,
	USA_RINGBACK_TONE,
	USA_REORDER_TONE
};

enum {
	USA_DEFAULT_RING
};

enum {
	USA_DEFAULT_FSK
};

enum {
	ZSYNTH_600,
	ZSYNTH_900
};

enum {
	AUDIOGAIN_MUTE,
	AUDIOGAIN_3_5DB,
	AUDIOGAIN_MINUS3_5DB,
	AUDIOGAIN_0DB
};

enum {
	DEFAULT_PCM
};
