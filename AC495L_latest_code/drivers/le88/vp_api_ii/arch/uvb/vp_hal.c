/** \file vp_hal.c
 * vp_hal.c
 *
 * This file contains the platform dependent code for the Hardware Abstraction
 * Layer (HAL). This implementation is for embedded Linux. This HAL 
 * implementation assumes a Linux kernel mode driver and also an application 
 * level interface to those kernel drivers.
 *
 * Copyright (c) 2004, Legerity Inc.
 * All rights reserved
 *
 * This software is the property of Legerity , Inc. Please refer to the
 * Non Disclosure Agreement (NDA) that you have signed for more information
 * on legal obligations in using, modifying or distributing this file.
 */

//Nirp - kernel mode changes
#include <linux/init.h>
#include <linux/module.h>
#include <linux/poll.h>


#ifdef USE_OLD_MPICMD
#include "asm/board/uvb_conn.h"
#include "uvb_xs.h"
#endif /* USE_OLD_MPICMD */
#include "vp_api_types.h"
#include "hbi_hal.h"
#include "hbi_hal.h"
#include "sys_service.h"



#define UINT8 uint8

/* 
 * The API header is needed to define the Device Types used by the API to know
 * how to implement VpMpiReset
 */
#include "vp_api_dev_term.h"

/*****************************************************************************
 * HAL functions for VCP and VPP. Not necessary for CSLAC devices.
 ****************************************************************************/
/* 
 * Define HBI_HAL_DEBUG to include parameter checking code and several
 * printk()s showing HBI activity.
 */
#undef HBI_HAL_DEBUG

#ifdef HBI_HAL_DEBUG
#include <stdio.h>
#include <string.h>

/* For run-time control of HBI tracing: */
static bool VpHbiTraceEnable = FALSE;
#endif /* HBI_HAL_DEBUG */


/* Changed the HBI deviceId data to match the MPI format S.H.
#define GET_CONNECTOR_ID(deviceId) (((deviceId) & 0x04)?1:0 )
#define GET_CHIP_SELECT(deviceId) ((deviceId) & 0x03)

#define GPI_WrData8(connector, cs, data, size) \
        GPI_Data(connector, cs, 0, GPI_WR_CMD, data, size)

*/
#define GET_CONNECTOR_ID(deviceId) (((deviceId) & 0x10)?0:1 )
#define GET_CHIP_SELECT(deviceId) ((deviceId) & 0x0C)
#define GET_TRANSFER_SIZE(deviceId) ((deviceId) & 0x01)
#define WRITE_COMMAND 0
#define READ_COMMAND 1


#define WORD_CHAR_CNVR_BUF_SIZE 512

#ifdef USE_HBI

static uint8 tempBuf[512]; /* All zero buffer */

/**
 * VpHalHbiInit(): Configures the HBI bus and glue logic (if any)
 *
 * This function performs any tasks necessary to prepare the system for
 * communicating through the HBI, including writing the HBI configuration
 * register.  The HBI read and write functions should work after HbiHalInit()
 * is successfully executed. HbiHalInit() should be well-behaved even if called
 * more than once between system resets. HbiHalInit() is called from
 * VpBootLoad() since VpBootLoad() is normally the first VoicePath function
 * that the host application will call.
 *
 * This function is called by VpBootLoad() before sending the DVP firmware
 * image through the HBI.
 *
 * Params:
 *  uint8 deviceId: Device Id (chip select ID)
 *
 * Returns:
 *  This function returns FALSE if some error occurred during HBI initialization
 *  or TRUE otherwise.
 */
bool VpHalHbiInit(
    VpDeviceIdType deviceId)
{
    /* 
     * Note that Setting up the basic device should be handled by the
     * some board bring up function. That function should setup the
     * CPLD based on the line module that is plugged in. Those functions
     * would configure the CPLD so that basic communication to the part
     * can happen between the HAL and the line module.
     */

    /* Write the HBI configuration register. */
    return VpHalHbiCmd(deviceId, HBI_CMD_CONFIGURE_INT + HBI_PINCONFIG);
} /* VpHalHbiInit() */

/**
 * VpHalHbiCmd(): Sends a command word over the HBI, with no data words.
 *
 *  Accepts a uint16 HBI command which is little-endian or big-endian, 
 * depending on the host architecture.  Command words on the HBI bus are always 
 * big-endian. This function is responsible for byte-swapping if required. This 
 * implementation for Linux does not need any byte swapping.
 *
 * Params:
 * uint8 deviceId: Device Id (chip select ID)
 * uint16 cmd: the command word to send
 *
 * Returns:
 *   TRUE on success, FALSE on failure
 */
bool VpHalHbiCmd(
    VpDeviceIdType deviceId, 
    uint16 cmd) 
{

#ifdef HBI_HAL_DEBUG
    printk(" CMD <== %04hX\n", cmd);
#endif /* HBI_HAL_DEBUG */

    if(!GPI_Command(GET_CONNECTOR_ID(deviceId), GET_CHIP_SELECT(deviceId), 
            GET_TRANSFER_SIZE(deviceId), WRITE_COMMAND, cmd, NULL, 0)) {
        return TRUE;    /* success */
    } else {
        return FALSE;    /* Failure */
    }
} /* VpHalHbiCmdWr() */

/**
 * VpHalHbiWrite(): Sends a command word and up to 256 data words over the HBI.
 *
 *  Accepts a uint16 HBI command which is little-endian or big-endian, depending
 * on the host architecture.  Command words on the HBI bus are always big-
 * endian.  This function is responsible for byte-swapping the command word, if
 * required. Note that this Linux implementation does not need byte swapping.
 *
 *  Accepts an array of uint16 data words.  No byte-swapping is necessary on
 * data words in this function.  Instead, the HBI bus can be configured in
 * VpHalHbiInit() to match the endianness of the host platform.
 *
 * Params:
 *   uint8 deviceId: Device Id (chip select ID)
 *   uint16 cmd: the command word to send
 *   uint8 numwords: the number of data words to send, minus 1
 *   uint16p data: the data itself; use data = (uint16p)0 to send
 *      zeroes for all data words
 *
 * Returns:
 *   TRUE on success, FALSE on failure
 */
bool VpHalHbiWrite(
    VpDeviceIdType deviceId, 
    uint16 cmd, 
    uint8 numwords, 
    uint16p data) 
{

    int numBytes = (numwords + 1)* 2;
    /* 
     * Since the Linux for PowerPC archiecture defines uint8 as one byte and 
     * uint16 as 2 bytes and since PowerPC architecture is a big-endian 
     * architecture. There is no need to convert from uint16 data to uint8 data 
     * array before calling the GPI library(and kernel). Such a conversion 
     * happens automatically by converting the pointer type.
     */
    uint8 *srcPtr = (uint8 *)data;

#ifdef HBI_HAL_DEBUG
    printk(" CMD <== %04hX\n", cmd);
#endif /* HBI_HAL_DEBUG */

    if(data == (uint16p)0) {
        /*
         * DevNotes: Not efficient; Kernal mode driver can be modified to handle 
         * this
         */
        srcPtr = tempBuf;
    }

    if(!GPI_Command(GET_CONNECTOR_ID(deviceId), GET_CHIP_SELECT(deviceId), 
            GET_TRANSFER_SIZE(deviceId), WRITE_COMMAND, cmd, srcPtr, numBytes)) {
        return TRUE;    /* success */
    } else {
        return FALSE;    /* Failure */
    }

} /* VpHalHbiWrite() */

/**
 * VpHalHbiRead(): Sends a command, and receives up to 256 data words over the
 * HBI.
 *
 *  Accepts a uint16 HBI command which is little-endian or big-endian, depending
 * on the host architecture.  Command words on the HBI bus are always big-
 * endian.  This function is responsible for byte-swapping the command word, if
 * required.
 *
 * Retrieves an array of uint16 data words.  No byte-swapping is necessary on
 * data words in this function.  Instead, the HBI bus can be configured in
 * VpHalHbiInit() to match the endianness of the host platform.
 *
 * Params:
 *   uint8 deviceId: Device Id (chip select ID)
 *   uint8 numwords: the number of words to receive, minus 1
 *   uint16p data: where to put them
 *
 * Returns:
 *   TRUE on success, FALSE on failure
 */
bool VpHalHbiRead(
    VpDeviceIdType deviceId, 
    uint16 cmd, 
    uint8 numwords, 
    uint16p data) 
{
    int numBytes = (numwords + 1)* 2;

#ifdef HBI_HAL_DEBUG
    printk(" CMD <== %04hX\n", cmd);
#endif /* HBI_HAL_DEBUG */

    /* 
     * Since the Linux for PowerPC archiecture defines uint8 as one byte and 
     * uint16 as 2 bytes and since PowerPC architecture is a big-endian 
     * architecture. There is no need to convert from uint16 data to uint8 data 
     * array before calling the GPI library(and kernel). Such a conversion 
     * happens automatically by converting the pointer type.
     */

    if(!GPI_Command(GET_CONNECTOR_ID(deviceId), GET_CHIP_SELECT(deviceId), 
            GET_TRANSFER_SIZE(deviceId), READ_COMMAND, cmd, 
            (unsigned char *)data, numBytes)) {
        return TRUE;    /* success */
    } else {
        return FALSE;    /* Failure */
    }
} /* VpHalHbiRead() */

/**
 * VpHalHbiWrite8(): Sends a command word and up to 256 data words over the HBI.
 *
 *  Accepts a uint16 HBI command which is little-endian or big-endian, depending
 * on the host architecture.  Command words on the HBI bus are always big-
 * endian.  This function is responsible for byte-swapping the command word, if
 * required. Note that this Linux implementation does not need byte swapping.
 *
 *  Accepts an array of uint8 data words.  No byte-swapping is necessary on
 * data words in this function.  Instead, the HBI bus can be configured in
 * VpHalHbiInit() to match the endianness of the host platform.
 *
 * Params:
 *   uint8 deviceId: Device Id (chip select ID)
 *   uint16 cmd: the command word to send
 *   uint8 numwords: the number of data words to send, minus 1
 *   uint8p data: the data itself; use data = (uint8p)0 to send
 *   zeroes for all data words
 *
 * Returns:
 *   TRUE on success, FALSE on failure
 */
bool VpHalHbiWrite8(
    VpDeviceIdType deviceId, 
    uint16 cmd, 
    uint8 numwords, 
    uint8p data) 
{

    int numBytes = (numwords + 1)* 2;
    uint8 *srcPtr = data;

#ifdef HBI_HAL_DEBUG
    printk(" CMD <== %04hX\n", cmd);
#endif /* HBI_HAL_DEBUG */

    if(data == (uint8p)0) {
        /* 
         * DevNotes: Not efficient; Kernal mode driver can be modified to handle 
         * this
         */
        srcPtr = tempBuf; 
    }

    if(!GPI_Command(GET_CONNECTOR_ID(deviceId), GET_CHIP_SELECT(deviceId), 
            GET_TRANSFER_SIZE(deviceId), WRITE_COMMAND, cmd, srcPtr, numBytes)) {
        return TRUE;    /* success */
    } else {
        return FALSE;    /* Failure */
    }
} /* VpHalHbiWrite8() */

/**
 * VpHalHbiRead8(): Sends a command, and receives up to 256 data words over the
 * HBI.
 *
 *  Accepts a uint16 HBI command which is little-endian or big-endian, depending
 * on the host architecture.  Command words on the HBI bus are always big-
 * endian.  This function is responsible for byte-swapping the command word, if
 * required.
 *
 *  Retrieves an array of uint8 data bytes.  No byte-swapping is necessary on
 * data words in this function.  Instead, the HBI bus can be configured in
 * VpHalHbiInit() to match the endianness of the host platform.
 *
 * Params:
 *   uint8 deviceId: Device Id (chip select ID)
 *   uint8 numwords: the number of words to receive, minus 1
 *   uint8p data: where to put them
 *
 * Returns:
 *   TRUE on success, FALSE on failure
 */
bool VpHalHbiRead8(
    VpDeviceIdType deviceId, 
    uint16 cmd, 
    uint8 numwords, 
    uint8p data) 
{

    int numBytes = (numwords + 1)* 2;

#ifdef HBI_HAL_DEBUG
    printk(" CMD <== %04hX\n", cmd);
#endif /* HBI_HAL_DEBUG */

    if(!GPI_Command(GET_CONNECTOR_ID(deviceId), GET_CHIP_SELECT(deviceId), 
            GET_TRANSFER_SIZE(deviceId), READ_COMMAND, cmd, data, numBytes)) {
        return TRUE;    /* success */
    } else {
        return FALSE;    /* Failure */
    }
} /* VpHalHbiRead8() */

/**
 * VpHalHbiBootWr():
 *
 *  This is used by the VpBootLoad() function to send the boot stream to the
 * DVP.  This function is separate from VpHalHbiWrite(), for the following
 * reasons:
 *
 *  1. This function does not accept a command word; only data words.
 *  2. This function accepts uint8 data, instead of uint16 data.  Be careful
 *     not to assume that this data is word-aligned in memory.
 *  3. The HBI must be configured for big-endian data words while the boot
 *     stream is being transmitted, regardless of the endianness of the host
 *     platform.  This is because the boot image is an opaque stream of HBI
 *     command words and data words.  Therefore, commands and data cannot be
 *     distinguished for separate treatment by this function.  Since HBI
 *     command words are always big-endian, data words have to be big-endian,
 *     too.  The boot stream is stored big-endian in memory, even on little-
 *     endian hosts.
 *        If VpHalHbiInit() configures the HBI for little-endian data words,
 *     then this function must temporarily change the configuration by calling
 *     VpHalHbiCmd(HBI_CMD_CONFIGURE(...)), and change it back before
 *     returning.  In such a case, this function will need to swap each pair
 *     of bytes in the boot stream before sending.
 *        Another possibility (as in the case of our test platform) is a
 *     little-endian host architecture, with the HBI bus configured for big-
 *     endian data words.  In this case, byte-swapping has to be done in
 *     VpHalHbiWrite() or in the glue logic between the host and the DVP.
 *     In these setups, VpHalHbiBootWr() does not need to reconfigure the
 *     HBI.
 *  4. This function takes a VpImagePtrType pointer to char data, which is a
 *     platform-dependent type defined in vp_hal.h.  In our system, a "_huge"
 *     pointer is needed for this function, since the boot stream is larger
 *     than 64 kB.
 *
 * Params
 *   uint8 deviceId: Device Id (chip select ID)
 *  'length' specifies the number of 16-bit words to write to the DVP.
 *  'pBuf' points into the boot stream.
 *
 * Returns
 *  HbiHalBootWr() returns TRUE on success, FALSE on failure.
 *
 * Notes
 *  THIS FUNCTION IS NOT REENTRANT!
 */
bool VpHalHbiBootWr(
    VpDeviceIdType deviceId, 
    uint8 numwords, 
    VpImagePtrType data) 
{

    int numBytes = (numwords + 1)*2;

    if(!GPI_Data(GET_CONNECTOR_ID(deviceId), GET_CHIP_SELECT(deviceId), 
            GET_TRANSFER_SIZE(deviceId), GPI_WR_CMD, data, numBytes)) {
        return TRUE;    /* success */
    } else {
        return FALSE;    /* Failure */
    }
} /* VpHalHbiBootWr() */

#endif /* USE_HBI */

/*****************************************************************************
 * HAL functions for CSLAC devices. Not necessary for VCP and VPP
 ****************************************************************************/

#ifndef USE_OLD_MPICMD
#define TELEDEV_NAME 		"/dev/ac494_mpi"
#define TELE_RESET			1000

static int mpifd = -1; /* file descripter for MPI driver */

/*--------------------------------------------------------------------
 * ROUTINE:   VpMpiDeviceOpen
 *
 * DESCRIPTION:
 *   function to open the MPI driver, will open the device
 *   and assign the file descripter to 'mpifd'.
 *   
 *
 * ARGUMENTS:
 *   void
 *
 * RESULTS:
 *   return void
 *--------------------------------------------------------------------*/
int VpMpiDeviceOpen(void)
{
#if 1//Nirp - no need to open a device - the MPI driver is called directly from the slic
	return 0;
#else
	if(mpifd > 0 ){
		/* mpifd already open */
		return 0;
	}
	mpifd = open(TELEDEV_NAME,O_RDWR);
	if(mpifd < 0) {
		perror("Open " TELEDEV_NAME);
		return -1;
	}
	return 0;
#endif
}

/*--------------------------------------------------------------------
 * ROUTINE:   VpMpiDeviceClose
 *
 * DESCRIPTION:
 *   function to close the MPI driver
 *   
 *
 * ARGUMENTS:
 *   void
 *
 * RESULTS:
 *   return void
 *--------------------------------------------------------------------*/
int VpMpiDeviceClose(void)
{
#if 1//Nirp - no need to open a device - the MPI driver is called directly from the slic
	return 0;
#else
	int ret;
	if( mpifd < 0 ) {
		return -1;
	}
	ret = close(mpifd);
	if(ret == 0){
		mpifd = -1;
	}
	return -1;
#endif
}

#if 0//Nirp - this function can be called directly since Slic driver and MPI driver are in kernel
/*--------------------------------------------------------------------
 * ROUTINE:   VpMpiCmd
 *
 * DESCRIPTION:
 *   function to access the MPI driver, issue the command to MPI.
 *   
 *
 * ARGUMENTS:
 *   VpDeviceIdType deviceId - device ID
 *   uint8 ecVal             - EC register value
 *   uint8 cmd,          	 - Command number
 *   uint8 cmdLen,       	 - Number of bytes used by command (cmd)
 *   uint8 *dataPtr			 - Pointer to the data location
 *
 * RESULTS:
 *   return void
 *--------------------------------------------------------------------*/
void
VpMpiCmd(
    VpDeviceIdType deviceId,  /* Chip select, connector and 3 or 4 wire 
                               * interface for command */
    uint8 ecVal,        	  /* Value to write to the EC register */
    uint8 cmd,          	  /* Command number */
    uint8 cmdLen,       	  /* Number of bytes used by command (cmd) */
    uint8 *dataPtr)     	  /* Pointer to the data location */
{
#define EC_REG_Rd	0x4B
#define EC_REG_Wr	0x4A
#define EC_REG_LEN	0x01
	unsigned char *param;
    
	if( mpifd < 0 && VpMpiDeviceOpen() < 0){
		printf("Error open MPI device\n");
		return;
	}

	param = (unsigned char*)malloc( (cmdLen + 2) * sizeof(unsigned char) );
	if( !param ){
		perror("malloc param");
		return;
	}

	/* Following code set the target EC Channel, EC1 = 0x01, EC2= 0x02 */
	if(cmd != EC_REG_Rd)
	{
		param[0]=EC_REG_Wr;
		param[1]=EC_REG_LEN;
		param[2] = ecVal;
		
		write(mpifd, param, 3);
	}
	
	param[0] = cmd;
	param[1] = cmdLen;
	
	
	/* All odd addresses are for read operations */
	if (cmd & 1) {
		/* check if read instruction */
		read(mpifd, param, (cmdLen + 2));
		
		if (dataPtr != NULL) {
			memcpy(dataPtr, &param[2], cmdLen);
		}
	}
	else {
		/* All even addresses are for write operations */
		if (dataPtr != NULL) {
			memcpy(&param[2],dataPtr,cmdLen);
		}
		write(mpifd, param, cmdLen + 2);
	}

	if(param) free(param);
	return;
}
#endif

#if 0
/* Nirp/Jungo this code was part of the vendor code  - ioctl not supported */
/* Nirp/Jungo this function is now part of the mpi driver.... */

/*--------------------------------------------------------------------
 * ROUTINE:   VpMpiReset
 *
 * DESCRIPTION:
 *   function to reset the MPI bus
 *   
 *
 * ARGUMENTS:
 *   VpDeviceIdType deviceId - device ID
 *   VpDeviceType deviceType - device type
 *
 * RESULTS:
 *   return void
 *--------------------------------------------------------------------*/
void
VpMpiReset(
    VpDeviceIdType deviceId,
    VpDeviceType deviceType)
{
	if( mpifd < 0 && VpMpiDeviceOpen() < 0){
		printk("Error open MPI device\n");
		return;
	}
	ioctl(mpifd, TELE_RESET);
	return;
}
#endif

#else /* USE_OLD_MPICMD */
/**
 * VpMpiCmd()
 *  This function executes a Device MPI command through the MPI port. It 
 * executes both read and write commands. The read or write operation is 
 * determined by the "cmd" argument (odd = read, even = write). The caller must 
 * ensure that the data array is large enough to hold the data being collected.
 * Because this command used hardware resources, this procedure is not
 * re-entrant.
 *
 * Preconditions:
 *  The device must be initialized.
 *
 * Postconditions:
 *   The data pointed to by dataPtr, using the command "cmd", with length
 * "cmdLen" has been sent to the MPI bus via the chip select associated with
 * deviceId.
 */
void
VpMpiCmd(
    VpDeviceIdType deviceId,    /**< Chip select, connector and 3 or 4 wire 
                                 * interface for command
                                 */
    uint8 ecVal,        /**< Value to write to the EC register */
    uint8 cmd,          /**< Command number */
    uint8 cmdLen,       /**< Number of bytes used by command (cmd) */
    uint8 *dataPtr)     /**< Pointer to the data location */
{
#define EC_REG_Rd    0x4B

    int cpldConfig, threeOrFourWire, bitOverride = 0xFF;
    uint8 rtnDataBufIdx = 7, x = 0, i, buffer[255];
    /* The deviceId is actually the cpld configuration between byte transfers */
    cpldConfig = deviceId;

    /* The two most sig bits of the cpld register need to be set to a 1 
     * (always). */
    cpldConfig |= 0xC0;

    /* check to see if the user has declared that the part has a 3 or 4 wire 
     * interface. */
    threeOrFourWire = cpldConfig & 0x20;

    /* to do the writes the 0x20 bit in the cpld register has to be removed
     * this bit should only be enabled for 3 wire commands that are reading back
     * data. see for loop below */
    cpldConfig &= 0xDF;

    /* Start building the data array */
    buffer[x++] = cpldConfig;

    /* If a EC read is being preformed don't set the EC register */
    if (EC_REG_Rd == cmd) {
        rtnDataBufIdx = 3;
    } else {
        buffer[x++] = 0x4A;
        buffer[x++] = cpldConfig;
        buffer[x++] = ecVal;
        buffer[x++] = cpldConfig;
    }

    /* Determine if cmd is a read command, if so change the interface bit
       which currently reflects the 3 or 4 wire status of the part*/
    if (cmd & 0x01) {
        bitOverride = 0x00;
        cpldConfig |= threeOrFourWire;
    }

    /* Add command to buffer */
    buffer[x++] = cmd;

    /* If the command is a read fill the buffer with cpld register commands and
     * 0's otherwise fill it with the cpld register commands and the bytes 
     * provided by the user */
    for (i = 0; i < cmdLen; i++) {
        buffer[x++] = cpldConfig;
        buffer[x++] = dataPtr[i] & bitOverride;       
    }

    VpSysEnterCritical(deviceId, VP_MPI_CRITICAL_SEC);

    SPI_MPI_RAW_Xfer( buffer, x );

    VpSysExitCritical(deviceId, VP_MPI_CRITICAL_SEC);
    
    /* Placing result data back into users data pointer */
    if (cmd & 0x01) {
        for (i = 0; rtnDataBufIdx <= x; rtnDataBufIdx+=2) {
            dataPtr[i++] = buffer[rtnDataBufIdx];
        }
    }

    return;

} /* End VpMpiCmd */

/**
 * VpMpiReset()
 *  This function executes a 16-bit MPI command with the CS low to cause a 
 * reset of the device.
 *
 * Preconditions:
 *  None.
 *
 * Postconditions:
 *  CS for the device specified by deviceId on the MPI bus is high (after being
 * held low for 16 clock cycles).
 */
void
VpMpiReset(
    VpDeviceIdType deviceId,
    VpDeviceType deviceType)
{
    uint8 ecValDummy = 0x00;
    uint8 cmdDummy = 0x00;
    uint8 cmdLenDummy = 0x00;
    uint8 dummyBuffer[1];
    
    /* Vp790 Type reset sets the Chip Select Low for 16 data clocks cycles */
    if (deviceType == VP_DEV_790_SERIES) {
        /* Hal layer implemention = xxxxxx11 corresponds to a 16 bit CS */
        VpMpiCmd((deviceId | 0x03), ecValDummy, cmdDummy, cmdLenDummy, 
            dummyBuffer);
    }
}
#endif /* USE_OLD_MPICMD */

