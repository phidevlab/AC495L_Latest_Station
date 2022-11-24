/** @file
 *  @brief This file includes definitions and compilation switches. Users should configure these definitions 
 *  according to their operation system and processor.
 */


/************************************************************************************/
/* AC49xDrv_Cfg.h - AC49x Device Driver / User Configuration Selection Header File	*/
/*																					*/
/* Copyright (C) 2000 AudioCodes Ltd.												*/
/*																					*/
/* Modifications :																	*/
/*																					*/
/* 1/7/01 - Coding started.															*/
/*																					*/
/************************************************************************************/
#ifndef AC49XCFG_H
#define AC49XCFG_H 

/*******************************************/
/*******************************************/
/**    Current Configuration Selection    **/
/*******************************************/
/*******************************************/

/* The follwoing are saved user configurations: */
/* The user can modify/add configurations.      */

/*-----------------------------------------------------------------------------------------*/
#define AC49X_CONFIG_NONE					0
#define AC49X_CONFIG_AC490_AEB				1  
#define AC49X_CONFIG_AC491xxx_AEB			2   
#define AC49X_CONFIG_AC494xxx_AEB			3  
#define AC49X_CONFIG_AC495xxx_AEB			4
#define AC49X_CONFIG_AC496xxx_AEB			5
#define AC49X_CONFIG_AC497xxx_AEB			6
#define AC49X_CONFIG_AC496Exxx_AEB			7
#define AC49X_CONFIG_AC491Lxxx_AEB			8 
#define AC49X_CONFIG_AC494Exxx_AEB			9 
#define AC49X_CONFIG_AC495Exxx_AEB			10 
#define AC49X_CONFIG_AC496Dxxx_AEB			11
#define AC49X_CONFIG_AC491_491L_AEB			12
#define AC49X_CONFIG_AC495Lxxx_AEB			13


#if defined(AC490)
	#define AC49X_CURRENT_CONFIG AC49X_CONFIG_AC490_AEB
#elif defined(AC491xxx)
	#define AC49X_CURRENT_CONFIG AC49X_CONFIG_AC491xxx_AEB
#elif defined(AC494xxx)
	#define AC49X_CURRENT_CONFIG AC49X_CONFIG_AC494xxx_AEB
#elif defined(AC495xxx)
	#define AC49X_CURRENT_CONFIG AC49X_CONFIG_AC495xxx_AEB
#elif defined(AC496xxx)
	#define AC49X_CURRENT_CONFIG AC49X_CONFIG_AC496xxx_AEB
#elif defined(AC497xxx)
	#define AC49X_CURRENT_CONFIG AC49X_CONFIG_AC497xxx_AEB
#elif defined(AC496Exxx)
	#define AC49X_CURRENT_CONFIG AC49X_CONFIG_AC496Exxx_AEB
#elif defined(AC491Lxxx)
	#define AC49X_CURRENT_CONFIG AC49X_CONFIG_AC491Lxxx_AEB
#elif defined(AC494Exxx)
	#define AC49X_CURRENT_CONFIG AC49X_CONFIG_AC494Exxx_AEB
#elif defined(AC495Exxx)
	#define AC49X_CURRENT_CONFIG AC49X_CONFIG_AC495Exxx_AEB
#elif defined(AC496Dxxx)
	#define AC49X_CURRENT_CONFIG AC49X_CONFIG_AC496Dxxx_AEB
#elif defined(AC491_491L)
	#define AC49X_CURRENT_CONFIG AC49X_CONFIG_AC491_491L_AEB
#elif defined(AC495Lxxx)
	#define AC49X_CURRENT_CONFIG AC49X_CONFIG_AC495Lxxx_AEB
#endif

#ifndef AC49X_CURRENT_CONFIG
	#define AC49X_CURRENT_CONFIG	AC49X_CONFIG_NONE
#endif /* AC49X_CURRENT_CONFIG */

#if	(AC49X_CURRENT_CONFIG == AC49X_CONFIG_NONE)
	#error The selected configuration is not implemented, please set "AC49X_CURRENT_CONFIG" to one of the supported options.
#endif

#define AC49X_OS__NONE						0
#define AC49X_OS__WINDOWS					1
#define AC49X_OS__VXWORKS					2
#define AC49X_OS__LINUX_MONTAVISTA			3


/*-----------------------------------------------------------------------------------------*/

/*******************************************/
/*******************************************/
/**     Configuration Implementation      **/
/*******************************************/
/*******************************************/

/* This section implements the various saved configurations by setting relevant parameters. */
/* ---------------------------------------------------------------------------------------- */

/* AC49X_DEVICE_TYPE selects the AudioCodes Packet Processor :
 *
 * AC49X_ENDIAN_MODE selects the host's CPU byte-order type :
 *     LITTLE_ENDIAN		- (LSB first) should be used for Intel CPUs,
 *     BIG_ENDIAN			- (MSB first) should be used for Motoral/RISC CPUs
 *
 * DSP_TO_HOST_ADDRESS_SHIFT defines the difference between the DSP and the
 *     Host addressing modes. Since the DSP addresses 16-bit words, and the
 *     Host usually addresses 8-bit words, the value should be 1 to indicate that
 *     a single shift operation is required for address translations.
 *     For hosts that address 16-bit words, the value should be set to 0.
 *
 * AC49X_NUMBER_OF_DEVICES selects the number of DSP cores on the user's boards.
 *
 *
 * AC48XIF_BASE_ADDRESS defines the base address in the host's memory of the array of DSPs
 *
 *
 * AC48XIF_DEVICES_ADDRESS_OFFSET defines the offset in bytes between two consecutive DSPs.
 *     If the user does not define this constant, it is assumed that the DSPs are continously
 *     mapped in the host's memory.
 *
 *
 *     The options are listed in the table obove.
 *     This constant selects the appropriate hardware access macro in AC48xHW.h.
 *
 */
	 

#if	AC49X_CURRENT_CONFIG == AC49X_CONFIG_AC490_AEB
	#define AC49X_DEVICE_TYPE		        AC490_DEVICE			/* The DSP device type */
	#define AC49X_ENDIAN_MODE		        LITTLE_ENDIAN			/* The Host endian mode */
	#define DSP_TO_HOST_ADDRESS_SHIFT       1						/* Addressed word size shift (DSP->Host) */
	#define NUMBER_OF_DEVICES_PER_DSP	    1 						/* Number of devices (DSP cores) available */
    #define AC49X_NUMBER_OF_DEVICES		    (NUMBER_OF_DEVICES_PER_DSP*1)
	#define AC49XIF_BASE_ADDRESS	        0x00000000				/* The base address of the HPI multiplexed mode */
	#define AC49XIF_HCNTL0_OFFSET	        0x00800000				/* The HCNTL0 address */
	#define AC49XIF_DEVICE_OFFSET           0x00000000				/* Since the devices (cores) are not continously mapped, */
	#define AC49X_DRIVRES_VERSION			AC49X_DRIVER_VERSION_700
	#define FIFO_USAGE_ENABLE__HOST				1
	#define FIFO_USAGE_ENABLE__NETWORK			1

	#define Ac49xGetNumberOfCore(Device) (Device%NUMBER_OF_DEVICES_PER_DSP)
	#define Ac49xGetNumberOfDsp(Device)  (int)(Device/NUMBER_OF_DEVICES_PER_DSP)

#elif	AC49X_CURRENT_CONFIG == AC49X_CONFIG_AC491xxx_AEB
	#define AC49X_DEVICE_TYPE		        AC491_DEVICE			/* The DSP device type */
	#define AC49X_ENDIAN_MODE		        LITTLE_ENDIAN			/* The Host endian mode */
	#define DSP_TO_HOST_ADDRESS_SHIFT       1						/* Addressed word size shift (DSP->Host) */
	#define NUMBER_OF_DEVICES_PER_DSP	    6 						/* Number of devices (DSP cores) available */
    #define AC49X_NUMBER_OF_DEVICES		    (NUMBER_OF_DEVICES_PER_DSP*1)
	#define AC49XIF_BASE_ADDRESS	        0x00000000				/* The base address of the HPI multiplexed mode */
	#define AC49XIF_DEVICE_OFFSET           0x00800000				/* The HCNTL0 address */
	#define AC49XIF_ENTRY_ADDRESS           0x00000100				/* Since the devices (cores) are not continously mapped */
	#define UTOPIA_ROUTING 
	#define AC49X_DRIVRES_VERSION			AC49X_DRIVER_VERSION_700

	#define FIFO_USAGE_ENABLE__HOST				1
	#define FIFO_USAGE_ENABLE__NETWORK			1

	#define Ac49xGetNumberOfCore(Device) (Device%NUMBER_OF_DEVICES_PER_DSP)
	#define Ac49xGetNumberOfDsp(Device)  (int)(Device/NUMBER_OF_DEVICES_PER_DSP)

#elif	AC49X_CURRENT_CONFIG == AC49X_CONFIG_AC491Lxxx_AEB
	#define AC49X_DEVICE_TYPE		        AC491L_DEVICE			/* The DSP device type */
	#define AC49X_ENDIAN_MODE		        LITTLE_ENDIAN			/* The Host endian mode */
	#define DSP_TO_HOST_ADDRESS_SHIFT       1						/* Addressed word size shift (DSP->Host) */
	#define NUMBER_OF_DEVICES_PER_DSP	    2 						/* Number of devices (DSP cores) available */
    #define AC49X_NUMBER_OF_DEVICES		    (NUMBER_OF_DEVICES_PER_DSP*1)
	#define AC49XIF_BASE_ADDRESS	        0x00000000				/* The base address of the HPI multiplexed mode */
	#define AC49XIF_DEVICE_OFFSET           0x00800000				/* The HCNTL0 address */
	#define AC49XIF_ENTRY_ADDRESS           0x00000100				/* Since the devices (cores) are not continously mapped */
//	#define UTOPIA_ROUTING 
	#define AC49X_DRIVRES_VERSION			AC49X_DRIVER_VERSION_700

	#define FIFO_USAGE_ENABLE__HOST				1
	#define FIFO_USAGE_ENABLE__NETWORK			1

	#define Ac49xGetNumberOfCore(Device) (Device%NUMBER_OF_DEVICES_PER_DSP)
	#define Ac49xGetNumberOfDsp(Device)  (int)(Device/NUMBER_OF_DEVICES_PER_DSP)

#elif	AC49X_CURRENT_CONFIG == AC49X_CONFIG_AC491_491L_AEB
	#define AC49X_DEVICE_TYPE		        AC491_491L_DEVICE		/* The DSP device type */
	#define AC49X_ENDIAN_MODE		        LITTLE_ENDIAN			/* The Host endian mode */
	#define DSP_TO_HOST_ADDRESS_SHIFT       1						/* Addressed word size shift (DSP->Host) */
	#define NUMBER_OF_491_DEVICES_PER_DSP	6 						/* Number of devices (DSP cores) available */
	#define NUMBER_OF_491L_DEVICES_PER_DSP	2 						/* Number of devices (DSP cores) available */
	//#define NUMBER_OF_DEVICES_PER_DSP		(NUMBER_OF_491_DEVICES_PER_DSP+NUMBER_OF_491L_DEVICES_PER_DSP)
    #define AC49X_NUMBER_OF_491_DEVICES		(NUMBER_OF_491_DEVICES_PER_DSP*3)
	#define AC49X_NUMBER_OF_491L_DEVICES	(NUMBER_OF_491L_DEVICES_PER_DSP*4)
	#define AC49X_NUMBER_OF_DEVICES			(AC49X_NUMBER_OF_491_DEVICES+AC49X_NUMBER_OF_491L_DEVICES)
	#define AC49XIF_BASE_ADDRESS	        0x00000000				/* The base address of the HPI multiplexed mode */
	#define AC49XIF_DEVICE_OFFSET           0x00800000				/* The HCNTL0 address */
	#define AC49XIF_ENTRY_ADDRESS           0x00000100				/* Since the devices (cores) are not continously mapped */
	#define UTOPIA_ROUTING 
	#define AC49X_DRIVRES_VERSION			AC49X_DRIVER_VERSION_700

	#define FIFO_USAGE_ENABLE__HOST				1
	#define FIFO_USAGE_ENABLE__NETWORK			1

	#define AC49X_491_491L_DEVICES_ORDER	AC49X_491_DEVICES_FIRST

	
	#define Ac49xIs491Device(Device) ((AC49X_491_491L_DEVICES_ORDER==AC49X_491_DEVICES_FIRST  && Device<AC49X_NUMBER_OF_491_DEVICES) || \
									  (AC49X_491_491L_DEVICES_ORDER==AC49X_491L_DEVICES_FIRST && Device>=AC49X_NUMBER_OF_491L_DEVICES))


	#define Ac49xGetNumberOfCore(Device) ac49xCoreOfDeviceMap[Device]
	#define Ac49xGetNumberOfDsp(Device)  ac49xDspOfDeviceMap[Device]

#elif	AC49X_CURRENT_CONFIG == AC49X_CONFIG_AC494xxx_AEB
	#define AC49X_DEVICE_TYPE		        AC494_DEVICE			/* The DSP device type */

	#define AC49X_OS_CONFIG					AC49X_OS__WINDOWS  
	#ifndef AC49X_OS_CONFIG
		#error The selected Operationg System is not implemented, please set "AC49X_OS_CONFIG" to one of the supported options.
	#endif

	#if		AC49X_OS_CONFIG == AC49X_OS__VXWORKS
		#define AC49X_ENDIAN_MODE		    BIG_ENDIAN			/* The Host endian mode */
	#elif	AC49X_OS_CONFIG == AC49X_OS__LINUX_MONTAVISTA
		#define AC49X_ENDIAN_MODE		    LITTLE_ENDIAN			/* The Host endian mode */
	#elif	AC49X_OS_CONFIG == AC49X_OS__WINDOWS
		#define AC49X_ENDIAN_MODE		    LITTLE_ENDIAN			/* The Host endian mode */
	#endif	/* endif AC49X_OS_CONFIG types */

	#define DSP_TO_HOST_ADDRESS_SHIFT       1						/* Addressed word size shift (DSP->Host) */
	#define NUMBER_OF_DEVICES_PER_DSP	    1 						/* Number of devices (DSP cores) available */
    #define AC49X_NUMBER_OF_DEVICES		    (NUMBER_OF_DEVICES_PER_DSP*4)


	#define DSP_PHYSICAL_ADDRESS_OFFSET     0x04000000				/* translate address From Program download address to physical address*/
	#define DSP_VIRTUAL_ADDRESS_OFFSET      0xA0000000				/* translate from physical address to virtual address.*/

	#define EMIF_VIRTUAL_ADDRESS_OFFSET		0xA0000000				/* translate from physical address to virtual address.*/
	#define AC49X_SDRAM_SIZE				0x4000000				/* must be a multipile of 4M */
	#define CASH_SEG_B_ALIGNED				0x400000				/* 4M */
	#define	DSP_EXTERNAL_MEMORY_OFFSET		0x400000				/* 4M */
	#define DSP_PROGRAM_SIZE				0x80000
	#define EMIF_PHYSICAL_BASE_ADDRESS		0x14000000
	#define EMIF_MAX_ADRESS					(EMIF_PHYSICAL_BASE_ADDRESS + AC49X_SDRAM_SIZE)
	#define EMIF_4M_DSP_ALIGEND				(EMIF_MAX_ADRESS - CASH_SEG_B_ALIGNED + EMIF_VIRTUAL_ADDRESS_OFFSET)
	#define CASH_SEG_B						(((EMIF_MAX_ADRESS - CASH_SEG_B_ALIGNED) >> 22 ) & 0x3FF)
	#define	EMIF_PHYSICAL_ADDRESS_OFFSET	(EMIF_MAX_ADRESS - CASH_SEG_B_ALIGNED - DSP_EXTERNAL_MEMORY_OFFSET)  

	#define PCM_CLK_AND_FRAME_SYNC_SOURCE_EXTERNAL 1
	#define AC49X_DRIVRES_VERSION			AC49X_DRIVER_VERSION_700


	#define FIFO_USAGE_ENABLE__HOST				0
	#define FIFO_USAGE_ENABLE__NETWORK			0

	#define Ac49xGetNumberOfCore(Device) (Device%NUMBER_OF_DEVICES_PER_DSP)
	#define Ac49xGetNumberOfDsp(Device)  (int)(Device/NUMBER_OF_DEVICES_PER_DSP)

#elif	AC49X_CURRENT_CONFIG == AC49X_CONFIG_AC495xxx_AEB
	#define AC49X_DEVICE_TYPE		        AC495_DEVICE			/* The DSP device type */

	#define AC49X_OS_CONFIG					AC49X_OS__WINDOWS  
	#ifndef AC49X_OS_CONFIG
		#error The selected Operationg System is not implemented, please set "AC49X_OS_CONFIG" to one of the supported options.
	#endif

	#if		AC49X_OS_CONFIG == AC49X_OS__VXWORKS
		#define AC49X_ENDIAN_MODE		    BIG_ENDIAN				/* The Host endian mode */
	#elif	AC49X_OS_CONFIG == AC49X_OS__LINUX_MONTAVISTA
		#define AC49X_ENDIAN_MODE		    LITTLE_ENDIAN			/* The Host endian mode */
	#elif	AC49X_OS_CONFIG == AC49X_OS__WINDOWS
		#define AC49X_ENDIAN_MODE		    LITTLE_ENDIAN			/* The Host endian mode */
	#endif	/* endif AC49X_EVM_OS_CONFIG types */

	#define DSP_TO_HOST_ADDRESS_SHIFT       1						/* Addressed word size shift (DSP->Host) */
	#define NUMBER_OF_DEVICES_PER_DSP	    1 						/* Number of devices (DSP cores) available */
    #define AC49X_NUMBER_OF_DEVICES		    (NUMBER_OF_DEVICES_PER_DSP*4)

	#define DSP_PHYSICAL_ADDRESS_OFFSET     0x04000000				/* translate address From Program download address to physical address*/
	#define DSP_VIRTUAL_ADDRESS_OFFSET      0xA0000000				/* translate from physical address to virtual address.*/

	#define EMIF_VIRTUAL_ADDRESS_OFFSET		0xA0000000				/* translate from physical address to virtual address.*/
	#define AC49X_SDRAM_SIZE				0x4000000				/* must be a multipile of 4M */
	#define CASH_SEG_B_ALIGNED				0x400000				/* 4M */
	#define	DSP_EXTERNAL_MEMORY_OFFSET		0x400000				/* 4M */
	#define DSP_PROGRAM_SIZE				0x80000
	#define EMIF_PHYSICAL_BASE_ADDRESS		0x14000000
	#define EMIF_MAX_ADRESS					(EMIF_PHYSICAL_BASE_ADDRESS + AC49X_SDRAM_SIZE)
	#define EMIF_4M_DSP_ALIGEND				(EMIF_MAX_ADRESS - CASH_SEG_B_ALIGNED + EMIF_VIRTUAL_ADDRESS_OFFSET)
	#define CASH_SEG_B						(((EMIF_MAX_ADRESS - CASH_SEG_B_ALIGNED) >> 22 ) & 0x3FF)
	#define	EMIF_PHYSICAL_ADDRESS_OFFSET	(EMIF_MAX_ADRESS - CASH_SEG_B_ALIGNED - DSP_EXTERNAL_MEMORY_OFFSET)  

	#define PCM_CLK_AND_FRAME_SYNC_SOURCE_EXTERNAL 1
	#define AC49X_DRIVRES_VERSION			AC49X_DRIVER_VERSION_700
	#define FIFO_USAGE_ENABLE__HOST				0
	#define FIFO_USAGE_ENABLE__NETWORK			0

	#define Ac49xGetNumberOfCore(Device) (Device%NUMBER_OF_DEVICES_PER_DSP)
	#define Ac49xGetNumberOfDsp(Device)  (int)(Device/NUMBER_OF_DEVICES_PER_DSP)

#elif	AC49X_CURRENT_CONFIG == AC49X_CONFIG_AC495Lxxx_AEB
	#define AC49X_DEVICE_TYPE		        AC495L_DEVICE			/* The DSP device type */

	#define AC49X_OS_CONFIG					AC49X_OS__WINDOWS  
	#ifndef AC49X_OS_CONFIG
		#error The selected Operationg System is not implemented, please set "AC49X_OS_CONFIG" to one of the supported options.
	#endif

	#if		AC49X_OS_CONFIG == AC49X_OS__VXWORKS
		#define AC49X_ENDIAN_MODE		    BIG_ENDIAN			/* The Host endian mode */
	#elif	AC49X_OS_CONFIG == AC49X_OS__LINUX_MONTAVISTA
		#define AC49X_ENDIAN_MODE		    LITTLE_ENDIAN			/* The Host endian mode */
	#elif	AC49X_OS_CONFIG == AC49X_OS__WINDOWS
		#define AC49X_ENDIAN_MODE		    LITTLE_ENDIAN			/* The Host endian mode */
	#endif	/* endif AC49X_OS_CONFIG types */

	#define DSP_TO_HOST_ADDRESS_SHIFT       1						/* Addressed word size shift (DSP->Host) */
	#define NUMBER_OF_DEVICES_PER_DSP	    1 						/* Number of devices (DSP cores) available */
    #define AC49X_NUMBER_OF_DEVICES		    (NUMBER_OF_DEVICES_PER_DSP*4)


	#define DSP_PHYSICAL_ADDRESS_OFFSET     0x04000000				/* translate address From Program download address to physical address*/
	#define DSP_VIRTUAL_ADDRESS_OFFSET      0xA0000000				/* translate from physical address to virtual address.*/

	#define EMIF_VIRTUAL_ADDRESS_OFFSET		0xA0000000				/* translate from physical address to virtual address.*/
	#define AC49X_SDRAM_SIZE				0x4000000				/* must be a multipile of 4M */
	#define CASH_SEG_B_ALIGNED				0x400000				/* 4M */
	#define	DSP_EXTERNAL_MEMORY_OFFSET		0x400000				/* 4M */
	#define DSP_PROGRAM_SIZE				0x80000
	#define EMIF_PHYSICAL_BASE_ADDRESS		0x14000000
	#define EMIF_MAX_ADRESS					(EMIF_PHYSICAL_BASE_ADDRESS + AC49X_SDRAM_SIZE)
	#define EMIF_4M_DSP_ALIGEND				(EMIF_MAX_ADRESS - CASH_SEG_B_ALIGNED + EMIF_VIRTUAL_ADDRESS_OFFSET)
	#define CASH_SEG_B						(((EMIF_MAX_ADRESS - CASH_SEG_B_ALIGNED) >> 22 ) & 0x3FF)
	#define	EMIF_PHYSICAL_ADDRESS_OFFSET	(EMIF_MAX_ADRESS - CASH_SEG_B_ALIGNED - DSP_EXTERNAL_MEMORY_OFFSET)  

	#define PCM_CLK_AND_FRAME_SYNC_SOURCE_EXTERNAL 1
	#define AC49X_DRIVRES_VERSION			AC49X_DRIVER_VERSION_700


	#define FIFO_USAGE_ENABLE__HOST				0
	#define FIFO_USAGE_ENABLE__NETWORK			0

	#define Ac49xGetNumberOfCore(Device) (Device%NUMBER_OF_DEVICES_PER_DSP)
	#define Ac49xGetNumberOfDsp(Device)  (int)(Device/NUMBER_OF_DEVICES_PER_DSP)

#elif	AC49X_CURRENT_CONFIG == AC49X_CONFIG_AC496xxx_AEB
	#define AC49X_DEVICE_TYPE		        AC496_DEVICE			/* The DSP device type */

	#define AC49X_OS_CONFIG					AC49X_OS__WINDOWS  
	#ifndef AC49X_OS_CONFIG
		#error The selected Operationg System is not implemented, please set "AC49X_OS_CONFIG" to one of the supported options.
	#endif

	#if		AC49X_OS_CONFIG == AC49X_OS__VXWORKS
		#define AC49X_ENDIAN_MODE		    BIG_ENDIAN				/* The Host endian mode */
	#elif	AC49X_OS_CONFIG == AC49X_OS__LINUX_MONTAVISTA
		#define AC49X_ENDIAN_MODE		    LITTLE_ENDIAN			/* The Host endian mode */
	#elif	AC49X_OS_CONFIG == AC49X_OS__WINDOWS
		#define AC49X_ENDIAN_MODE		    LITTLE_ENDIAN			/* The Host endian mode */
	#endif	/* endif AC49X_EVM_OS_CONFIG types */

	#define DSP_TO_HOST_ADDRESS_SHIFT       1						/* Addressed word size shift (DSP->Host) */
	#define NUMBER_OF_DEVICES_PER_DSP	    1 						/* Number of devices (DSP cores) available */
    #define AC49X_NUMBER_OF_DEVICES		    (NUMBER_OF_DEVICES_PER_DSP*4)

	#define DSP_PHYSICAL_ADDRESS_OFFSET     0x04000000				/* translate address From Program download address to physical address*/
	#define DSP_VIRTUAL_ADDRESS_OFFSET      0xA0000000				/* translate from physical address to virtual address.*/

	#define EMIF_VIRTUAL_ADDRESS_OFFSET		0xA0000000				/* translate from physical address to virtual address.*/
	#define AC49X_SDRAM_SIZE				0x4000000				/* must be a multipile of 4M */
	#define CASH_SEG_B_ALIGNED				0x400000				/* 4M */
	#define	DSP_EXTERNAL_MEMORY_OFFSET		0x400000				/* 4M */
	#define DSP_PROGRAM_SIZE				0x80000
	#define EMIF_PHYSICAL_BASE_ADDRESS		0x14000000
	#define EMIF_MAX_ADRESS					(EMIF_PHYSICAL_BASE_ADDRESS + AC49X_SDRAM_SIZE)
	#define EMIF_4M_DSP_ALIGEND				(EMIF_MAX_ADRESS - CASH_SEG_B_ALIGNED + EMIF_VIRTUAL_ADDRESS_OFFSET)
	#define CASH_SEG_B						(((EMIF_MAX_ADRESS - CASH_SEG_B_ALIGNED) >> 22 ) & 0x3FF)
	#define	EMIF_PHYSICAL_ADDRESS_OFFSET	(EMIF_MAX_ADRESS - CASH_SEG_B_ALIGNED - DSP_EXTERNAL_MEMORY_OFFSET)  

	#define PCM_CLK_AND_FRAME_SYNC_SOURCE_EXTERNAL 1
	#define AC49X_DRIVRES_VERSION			AC49X_DRIVER_VERSION_700
	#define FIFO_USAGE_ENABLE__HOST				0
	#define FIFO_USAGE_ENABLE__NETWORK			0

	#define Ac49xGetNumberOfCore(Device) (Device%NUMBER_OF_DEVICES_PER_DSP)
	#define Ac49xGetNumberOfDsp(Device)  (int)(Device/NUMBER_OF_DEVICES_PER_DSP)

#elif	AC49X_CURRENT_CONFIG == AC49X_CONFIG_AC496Exxx_AEB
	#define AC49X_DEVICE_TYPE		        AC496E_DEVICE			/* The DSP device type */

	#define AC49X_OS_CONFIG					AC49X_OS__WINDOWS  
	#ifndef AC49X_OS_CONFIG
		#error The selected Operationg System is not implemented, please set "AC49X_OS_CONFIG" to one of the supported options.
	#endif

	#if		AC49X_OS_CONFIG == AC49X_OS__VXWORKS
		#define AC49X_ENDIAN_MODE		    BIG_ENDIAN				/* The Host endian mode */
	#elif	AC49X_OS_CONFIG == AC49X_OS__LINUX_MONTAVISTA
		#define AC49X_ENDIAN_MODE		    LITTLE_ENDIAN			/* The Host endian mode */
	#elif	AC49X_OS_CONFIG == AC49X_OS__WINDOWS
		#define AC49X_ENDIAN_MODE		    LITTLE_ENDIAN			/* The Host endian mode */
	#endif	/* endif AC49X_EVM_OS_CONFIG types */

	#define DSP_TO_HOST_ADDRESS_SHIFT       1						/* Addressed word size shift (DSP->Host) */
	#define NUMBER_OF_DEVICES_PER_DSP	    1 						/* Number of devices (DSP cores) available */
    #define AC49X_NUMBER_OF_DEVICES		    (NUMBER_OF_DEVICES_PER_DSP*4)

	#define DSP_PHYSICAL_ADDRESS_OFFSET     0x04008000				/* translate address From Program download address to physical address*/
	#define DSP_VIRTUAL_ADDRESS_OFFSET      0xA0000000				/* translate from physical address to virtual address.*/

	#define EMIF_VIRTUAL_ADDRESS_OFFSET		0xA0000000				/* translate from physical address to virtual address.*/
	#define AC49X_SDRAM_SIZE				0x4000000				/* must be a multipile of 4M */
	#define CASH_SEG_B_ALIGNED				0x400000				/* 4M */
	#define	DSP_EXTERNAL_MEMORY_OFFSET		0x400000				/* 4M */
	#define DSP_PROGRAM_SIZE				0x80000
#define EMIF_PHYSICAL_BASE_ADDRESS		0x14000000 /* CHECK LATER */
	#define EMIF_MAX_ADRESS					(EMIF_PHYSICAL_BASE_ADDRESS + AC49X_SDRAM_SIZE)
	#define EMIF_4M_DSP_ALIGEND				(EMIF_MAX_ADRESS - CASH_SEG_B_ALIGNED + EMIF_VIRTUAL_ADDRESS_OFFSET)
	#define CASH_SEG_B						(((EMIF_MAX_ADRESS - CASH_SEG_B_ALIGNED) >> 22 ) & 0x3FF)
	#define	EMIF_PHYSICAL_ADDRESS_OFFSET	(EMIF_MAX_ADRESS - CASH_SEG_B_ALIGNED - DSP_EXTERNAL_MEMORY_OFFSET)  

	#define PCM_CLK_AND_FRAME_SYNC_SOURCE_EXTERNAL 1
	#define AC49X_DRIVRES_VERSION			AC49X_DRIVER_VERSION_700
	#define FIFO_USAGE_ENABLE__HOST				0
	#define FIFO_USAGE_ENABLE__NETWORK			0

	#define Ac49xGetNumberOfCore(Device) (Device%NUMBER_OF_DEVICES_PER_DSP)
	#define Ac49xGetNumberOfDsp(Device)  (int)(Device/NUMBER_OF_DEVICES_PER_DSP)

#elif	AC49X_CURRENT_CONFIG == AC49X_CONFIG_AC497xxx_AEB
	#define AC49X_DEVICE_TYPE		        AC497_DEVICE			/* The DSP device type */

	#define AC49X_OS_CONFIG					AC49X_OS__WINDOWS  
	#ifndef AC49X_OS_CONFIG
		#error The selected Operationg System is not implemented, please set "AC49X_OS_CONFIG" to one of the supported options.
	#endif

	#if		AC49X_OS_CONFIG == AC49X_OS__VXWORKS
		#define AC49X_ENDIAN_MODE		    BIG_ENDIAN				/* The Host endian mode */
	#elif	AC49X_OS_CONFIG == AC49X_OS__LINUX_MONTAVISTA
		#define AC49X_ENDIAN_MODE		    LITTLE_ENDIAN			/* The Host endian mode */
	#elif	AC49X_OS_CONFIG == AC49X_OS__WINDOWS
		#define AC49X_ENDIAN_MODE		    LITTLE_ENDIAN			/* The Host endian mode */
	#endif	/* endif AC49X_EVM_OS_CONFIG types */

	#define DSP_TO_HOST_ADDRESS_SHIFT       1						/* Addressed word size shift (DSP->Host) */
	#define NUMBER_OF_DEVICES_PER_DSP	    1 						/* Number of devices (DSP cores) available */
    #define AC49X_NUMBER_OF_DEVICES		    (NUMBER_OF_DEVICES_PER_DSP*4)

	#define DSP_PHYSICAL_ADDRESS_OFFSET     0x04000000				/* translate address From Program download address to physical address*/
	#define DSP_VIRTUAL_ADDRESS_OFFSET      0xA0000000				/* translate from physical address to virtual address.*/

	#define EMIF_VIRTUAL_ADDRESS_OFFSET		0xA0000000				/* translate from physical address to virtual address.*/
	#define AC49X_SDRAM_SIZE				0x4000000				/* must be a multipile of 4M */
	#define CASH_SEG_B_ALIGNED				0x400000				/* 4M */
	#define	DSP_EXTERNAL_MEMORY_OFFSET		0x400000				/* 4M */
	#define DSP_PROGRAM_SIZE				0x80000
	#define EMIF_PHYSICAL_BASE_ADDRESS		0x14000000
	#define EMIF_MAX_ADRESS					(EMIF_PHYSICAL_BASE_ADDRESS + AC49X_SDRAM_SIZE)
	#define EMIF_4M_DSP_ALIGEND				(EMIF_MAX_ADRESS - CASH_SEG_B_ALIGNED + EMIF_VIRTUAL_ADDRESS_OFFSET)
	#define CASH_SEG_B						(((EMIF_MAX_ADRESS - CASH_SEG_B_ALIGNED) >> 22 ) & 0x3FF)
	#define	EMIF_PHYSICAL_ADDRESS_OFFSET	(EMIF_MAX_ADRESS - CASH_SEG_B_ALIGNED - DSP_EXTERNAL_MEMORY_OFFSET)  

	#define PCM_CLK_AND_FRAME_SYNC_SOURCE_EXTERNAL 1
	#define AC49X_DRIVRES_VERSION			AC49X_DRIVER_VERSION_700

	#define FIFO_USAGE_ENABLE__HOST				0
	#define FIFO_USAGE_ENABLE__NETWORK			0

	#define Ac49xGetNumberOfCore(Device) (Device%NUMBER_OF_DEVICES_PER_DSP)
	#define Ac49xGetNumberOfDsp(Device)  (int)(Device/NUMBER_OF_DEVICES_PER_DSP)

#elif (   (AC49X_CURRENT_CONFIG == AC49X_CONFIG_AC494Exxx_AEB)\
	   || (AC49X_CURRENT_CONFIG == AC49X_CONFIG_AC495Exxx_AEB)\
	   || (AC49X_CURRENT_CONFIG == AC49X_CONFIG_AC496Dxxx_AEB)\
	  )


	#if (AC49X_CURRENT_CONFIG == AC49X_CONFIG_AC494Exxx_AEB)
		#define AC49X_DEVICE_TYPE		        AC494E_DEVICE			/* The DSP device type */
	#elif (AC49X_CURRENT_CONFIG == AC49X_CONFIG_AC495Exxx_AEB)
		#define AC49X_DEVICE_TYPE		        AC495E_DEVICE			/* The DSP device type */
	#elif (AC49X_CURRENT_CONFIG == AC49X_CONFIG_AC496Dxxx_AEB)
		#define AC49X_DEVICE_TYPE		        AC496D_DEVICE			/* The DSP device type */
	#endif 


	#define AC49X_OS_CONFIG					AC49X_OS__WINDOWS 
	#ifndef AC49X_OS_CONFIG
		#error The selected Operationg System is not implemented, please set "AC49X_OS_CONFIG" to one of the supported options.
	#endif

	#if		AC49X_OS_CONFIG == AC49X_OS__VXWORKS
		#define AC49X_ENDIAN_MODE		    BIG_ENDIAN				/* The Host endian mode */
	#elif	AC49X_OS_CONFIG == AC49X_OS__LINUX_MONTAVISTA
		#define AC49X_ENDIAN_MODE		    LITTLE_ENDIAN			/* The Host endian mode */
	#elif	AC49X_OS_CONFIG == AC49X_OS__WINDOWS
		#define AC49X_ENDIAN_MODE		    LITTLE_ENDIAN			/* The Host endian mode */
	#endif	/* endif AC49X_EVM_OS_CONFIG types */

	#define DSP_TO_HOST_ADDRESS_SHIFT       1						/* Addressed word size shift (DSP->Host) */
	#define NUMBER_OF_DEVICES_PER_DSP	    1 						/* Number of devices (DSP cores) available */
    #define AC49X_NUMBER_OF_DEVICES		    (NUMBER_OF_DEVICES_PER_DSP*4)

	#define DSP_PHYSICAL_ADDRESS_OFFSET     0x04000000				/* translate address From Program download address to physical address*/
	#define DSP_VIRTUAL_ADDRESS_OFFSET      0xA0000000				/* translate from physical address to virtual address.*/

	#define EMIF_VIRTUAL_ADDRESS_OFFSET		0xA0000000				/* translate from physical address to virtual address.*/
	#define AC49X_SDRAM_SIZE				0x4000000				/* must be a multipile of 4M */
	#define CASH_SEG_B_ALIGNED				0x400000				/* 4M */
	#define	DSP_EXTERNAL_MEMORY_OFFSET		0x400000				/* 4M */
	#define DSP_PROGRAM_SIZE				0x80000
	#define EMIF_PHYSICAL_BASE_ADDRESS		0x14000000
	#define EMIF_MAX_ADRESS					(EMIF_PHYSICAL_BASE_ADDRESS + AC49X_SDRAM_SIZE)
	#define EMIF_4M_DSP_ALIGEND				(EMIF_MAX_ADRESS - CASH_SEG_B_ALIGNED + EMIF_VIRTUAL_ADDRESS_OFFSET)
	#define CASH_SEG_B						(((EMIF_MAX_ADRESS - CASH_SEG_B_ALIGNED) >> 22 ) & 0x3FF)
	#define	EMIF_PHYSICAL_ADDRESS_OFFSET	(EMIF_MAX_ADRESS - CASH_SEG_B_ALIGNED - DSP_EXTERNAL_MEMORY_OFFSET)  

	#define SET_PDCR_MASK					0x05221A20
	#define CLEAR_PDCR_MASK					(~(SET_PDCR_MASK))
	#define SET_PRCR_MASK					0x20151C20
	#define CLEAR_PRCR_MASK					(~(SET_PRCR_MASK))
	#define SET_MASTER_CTRL_REG_MASK		0x00000300
	#define CLEAR_MASTER_CTRL_REG_MASK		(~(0x000FFCB1))
	#define PINSEL9_MASK					0x0000FF00

	#define PCM_CLK_AND_FRAME_SYNC_SOURCE_EXTERNAL 1
	#define AC49X_DRIVRES_VERSION			AC49X_DRIVER_VERSION_700

	#define FIFO_USAGE_ENABLE__HOST				0
	#define FIFO_USAGE_ENABLE__NETWORK			0

	#define Ac49xGetNumberOfCore(Device) (Device%NUMBER_OF_DEVICES_PER_DSP)
	#define Ac49xGetNumberOfDsp(Device)  (int)(Device/NUMBER_OF_DEVICES_PER_DSP)

#else
	#define NUMBER_OF_DEVICES_PER_DSP	1						/* Number of devices (DSP cores) available */

#endif /*~~AC49X_CURRENT_CONFIG*/

/*Redefine AC49X_ENDIAN_MODE*/
#if defined(CONFIG_AC49X_LITTLE_ENDIAN)
	#undef AC49X_ENDIAN_MODE
	#define AC49X_ENDIAN_MODE			LITTLE_ENDIAN	
#elif defined(CONFIG_AC49X_BIG_ENDIAN)
	#undef AC49X_ENDIAN_MODE
	#define AC49X_ENDIAN_MODE			BIG_ENDIAN
#endif

/*Redefine AC49X_OS_CONFIG*/
#undef AC49X_OS_CONFIG
#define AC49X_OS_CONFIG AC49X_OS__LINUX_MONTAVISTA

/*Redefine AC49X_NUMBER_OF_DEVICES*/
#if defined(CONFIG_NUMBER_OF_DSP)
#undef AC49X_NUMBER_OF_DEVICES
#define AC49X_NUMBER_OF_DEVICES (NUMBER_OF_DEVICES_PER_DSP*CONFIG_NUMBER_OF_DSP)
#endif

/*Redefine PCM_CLK_AND_FRAME_SYNC_SOURCE_EXTERNAL*/
#undef PCM_CLK_AND_FRAME_SYNC_SOURCE_EXTERNAL
#define PCM_CLK_AND_FRAME_SYNC_SOURCE_EXTERNAL 0

/*Redefine AC49X_SDRAM_SIZE*/
#if defined(CONFIG_AC49X_SDRAM_SIZE)
	#undef AC49X_SDRAM_SIZE
	#define AC49X_SDRAM_SIZE CONFIG_AC49X_SDRAM_SIZE
#else
    extern unsigned long prom_memsize;/*Defined and caculated in /arch/mips/audc/generic/Memory.c for sdk3.10*/
    #undef AC49X_SDRAM_SIZE
    #define AC49X_SDRAM_SIZE prom_memsize
#endif

/*******************************************/
/*******************************************/
/**  Miscellaneous User Configurations    **/
/*******************************************/
/*******************************************/


/* The max number of Ac49xUserDef_DelayForDeviceResponse iteration allowed */
/* WaitForDeviceResponse is a user-defined function that performs a */
/* constant delay (e.g. by calling sleep(1)). */
/* A value of 10000 is equal to 10 sec if Ac49xUserDef_DelayForDeviceResponse waits for 1msec */

/* The user-implemented Ac49xUserDef_DelayForDeviceResponse delay time in micro sec : */
#define DELAY_FOR_DEVICE_RESPONSE	200

/* The default maximum delay time (before declaring an error) in msec: */
/* Note that the actual time will be longer because of the device polling */
/* time which is added to the user delay time at each iteration. */
#define MAX_DELAY_TIME			    5000

/* Set CHECK_COMMAND_ECHO to enable checking of the echoed host-to-dsp */
/* command. The check is done by comparing the echo to the original command. */
/* Set to 0 to diable the check. */
#define CHECK_COMMAND_ECHO		    1

/* Debugging control - set to 1 to  enable Tx/Rx debug option (depend on 'ac49xDebugMode'). */
/* Debugging control - set to 0 to disable Tx/Rx debug option (save processing time).       */
#define ALLOW_DEBUG_MODE					0

#define PACKET_RECORDING_ENABLE				1

#define CRASH_TEST_ENABLE					0

#define FIFO_CELL_SIZE__HOST				44 /*This value must be Greater or equal to HostCellSize at Tac49xDeviceControlRegisterAttr     */
#define FIFO_CELL_SIZE__NETWORK				56 /*This value must be Greater or equal to NetworkCellSize at Tac49xDeviceControlRegisterAttr  */

#define FIFO_SIZE__HOST                     ((U16)(1<<10)) /* have to be power of 2 (range: 0 to 15)*/  
#define FIFO_SIZE__NETWORK                  ((U16)(1<<10)) /* have to be power of 2 (range: 0 to 15)*/  

#endif /* ifndef AC49XCFG_H */
