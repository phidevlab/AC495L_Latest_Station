/** @file
 *  @brief This file contains constant defined values for configuration options. Users should not edit this 
 *  file. Following are the 5 lists contained in this file:
 *  @n AC48/9x Device Types Definitions
 *  @n AC49x HPI Types Definitions
 *  @n Device Memory Access Definitions
 *  @n Endian (Byte-Order) Mode Definitions
 *  @n Packet Header Type Related Definition
 */


/***********************************************************************************/
/* Ac49xCfg.h - AC49x Device Driver / Constant Configuration Options Definitions   */
/*                                                                                 */
/* Copyright (C) 2000 AudioCodes Ltd.                                              */
/*                                                                                 */
/* Modifications :                                                                 */
/*                                                                                 */
/* 1/7/01 - Coding started.                                                        */ 
/*                                                                                 */
/* *****************                                                               */ 
/* IMPORTANT NOTES :                                                               */
/* *****************                                                               */
/*                                                                                 */
/* This file contains constant values for configuration options.                   */
/* The user SHOULD NOT edit this file.                                             */
/*                                                                                 */
/***********************************************************************************/

#ifndef AC49XOPT_H
#define AC49XOPT_H 

/*******************************************/
/*******************************************/
/**         AC49x Drivers Versions		  **/
/*******************************************/
/*******************************************/
#define AC49X_DRIVER_VERSION_105 105
#define AC49X_DRIVER_VERSION_106 106
#define AC49X_DRIVER_VERSION_107 107
#define AC49X_DRIVER_VERSION_108 108
#define AC49X_DRIVER_VERSION_520 520
#define AC49X_DRIVER_VERSION_540 540
#define AC49X_DRIVER_VERSION_560 560
#define AC49X_DRIVER_VERSION_580 580
#define AC49X_DRIVER_VERSION_600 600
#define AC49X_DRIVER_VERSION_620 620
#define AC49X_DRIVER_VERSION_640 640
#define AC49X_DRIVER_VERSION_660 660
#define AC49X_DRIVER_VERSION_680 680
#define AC49X_DRIVER_VERSION_700 700
#define AC49X_DRIVER_VERSION_720 720


/*******************************************/
/*******************************************/
/**  AC48/9x Device Types Definitions     **/
/*******************************************/
/*******************************************/
#define	AC490_DEVICE		1
#define	AC491_DEVICE		2
#define	AC494_DEVICE		3
#define AC495_DEVICE		4
#define AC496_DEVICE		5
#define AC497_DEVICE		6
#define AC496E_DEVICE		7
#define AC491L_DEVICE		8
#define AC494E_DEVICE		9
#define AC495E_DEVICE		10
#define AC496D_DEVICE		11
#define	AC491_491L_DEVICE	12
#define AC495L_DEVICE		13

/*******************************************/
/*******************************************/
/** Endian (Byte-Order) Mode Definitions  **/
/*******************************************/
/*******************************************/

#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN	0	/* (LSB first) should be used for Intel CPUs, */
#endif

#ifndef BIG_ENDIAN
#define BIG_ENDIAN		1	/* (MSB first) should be used for Motoral/RISC CPUs */
#endif

//#if (AC49X_CURRENT_CONFIG == AC49X_CONFIG_AC491_491L_AEB)
	#define AC49X_491_DEVICES_FIRST		0
	#define AC49X_491L_DEVICES_FIRST	1
//#endif

#endif /* ifndef AC49XOPT_H */
