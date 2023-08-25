/** \file hbi_common.c
 * hbi_common.c
 *
 *  This file contains the platform-independent Host Bus Interface layer.
 *
 * Copyright (c) 2005, Legerity Inc.
 * All rights reserved
 *
 * This software is the property of Legerity , Inc. Please refer to the
 * Non Disclosure Agreement (NDA) that you have signed for more information
 * on legal obligations in using, modifying or distributing this file.
 */
#include "vp_api.h"

#if defined (VP_CC_DVP_SERIES) || defined (VP_CC_VPP_SERIES) /* Compile Further
                                                           * only if required */

#include "hbi_common.h"

/* VpHbiDvpVppReset()
 *
 *  This function simply causes the DVP/VPP to reset and is called by
 *  VpBootLoad() before loading the DVP/VPP device's RAM.
 *
 * Params
 *  'deviceId' - identifies the chip select for the device
 *
 * Returns
 *  none
 *
 * Notes
 *  This function is platform dependent, which means that it is defined by the
 *  customer, and should be located in ..import\<PLATFORM_DIR>\hbi_hal_port.c.
 */
bool VpHbiDvpVppReset(VpDeviceIdType deviceId) {
    uint16 regVal = 0x0005;
    /* Reset the DVP. */
    if (VpHbiWr(deviceId, HW_Reg_HWRES, &regVal) == FALSE)
        return FALSE;
    /* Configure the HBI. */
    if (VpHalHbiCmd(deviceId, HBI_CMD_CONFIGURE(HBI_PINCONFIG)) == FALSE)
        return FALSE;

    return TRUE;
} /* VpHbiDvpVppReset() */


/* Sets page 255 base register using HBI commands, and puts value into
 * 'base255reg'. */
bool VpHbiSetBase255(VpDeviceIdType deviceId, uint32 amba_addr,
                        uint16p base255reg) {

    /* Page 255 acts as a sliding window to the AMBA bus.  You slide the window
       by setting the Page 255 Base Register (VP_REGID_BASE255).  Read the
       existing value so we won't disturb the reserved bits. */

    if (!VpHbiRd(deviceId, HW_Reg_BASE255, base255reg))
        return FALSE;

    /* Set the Page 255 Base Register to the page containing amba_addr.
       The upper 10 bits of the AMBA address go into the non-reserved bits
       [14:5] of the Page 255 Base Register.  Don't disturb the reserved
       bits. */
#define RSVD_bits       0x801F /* selects the RSVD bits in the BASE255 reg */
#define upper_10_bits   0x1FF80/* selects the upper 10 bits of an AMBA address*/
    *base255reg &= RSVD_bits;
    *base255reg |= (amba_addr & upper_10_bits) >> 2;
    if (!VpHbiWr(deviceId, HW_Reg_BASE255, base255reg))
        return FALSE;

    return TRUE;
} /* VpHbiSetBase255 */


/* VpHbiAmbaRdWr()
 *
 *  This function reads/writes 'numwords' words of DVP/VPP memory starting at the
 *  specified AMBA address.
 *
 * Params
 *  'readWrite' - FALSE for read, TRUE for write
 *  'deviceId' - deviceId of the DVP/VPP device
 *  'amba_addr' - the AMBA address to start reading/writing at
 *  'numwords' - the number of data words to read/write, minus 1
 *  'data' - a pointer to the data to be read/written; use data = (uint16p)0
 *      to write zeroes for all data words
 *
 * Returns
 *  TRUE on success, FALSE on failure
 *
 * Notes
 */
bool
VpHbiAmbaRdWr(
    bool readWrite,
    VpDeviceIdType deviceId,
    uint32 amba_addr,
    uint32 numwords,
    uint16p data)
{
    uint16 base255reg;
    uint8 offset = amba_addr & (VP_HBI_PAGE_SIZE - 1);
    uint16 chunkSize = VP_HBI_PAGE_SIZE - offset;

    numwords++;

    if (chunkSize > numwords) {
        chunkSize = numwords;
    }

    if (
        /* Set Page 255 base register to specified AMBA address. */
        (VpHbiSetBase255(deviceId, amba_addr, &base255reg) == FALSE) ||

        /* Select page 255 for paged writing. */
        (VpHalHbiCmd(deviceId, HBI_CMD_SELECT_PAGE(255)) == FALSE)
    ) {
        return FALSE;
    }

    /* Page 255 can only address VP_HBI_PAGE_SIZE = 128 words at a time.  
     * After processing each 128 words, the Page 255 Base Register has 
     * to be incremented. */

    do {
        /* Read/write the data at Page 255, offset 0. */
        if (readWrite == TRUE) {
            uint16 cmd = HBI_CMD_PAGED_WRITE(offset, chunkSize - 1);
            if (!VpHalHbiWrite(deviceId, cmd, chunkSize - 1, data)) {
                return FALSE;
            }
        } else {
            uint16 cmd = HBI_CMD_PAGED_READ(offset, chunkSize - 1);
            if (!VpHalHbiRead(deviceId, cmd, chunkSize - 1, data)) {
                return FALSE;
            }
        }


        /* Increase the AMBA address in the Page 255 Base Register by 
         * 128 words.  Since bit 8 of the AMBA address is bit 5 in the 
         * BASE255 register, this is done by adding 0x0020 to the BASE255
         * register. */

        base255reg += 0x0020;
        if (!VpHbiWr(deviceId, HW_Reg_BASE255, &base255reg))
            return FALSE;

        /* Increment the data pointer by the amount of data read/written. */

        if (data != (uint16p)0) {
            data = &data[chunkSize];
        }
        numwords -= chunkSize;

        /* Set amount of data to be transferred in next block. */

        if (numwords < VP_HBI_PAGE_SIZE) {
            chunkSize = numwords;
        } else {
            chunkSize = VP_HBI_PAGE_SIZE;
        }

        /* In all but the first block, offset = 0. */
        offset = 0;

	} while (numwords > 0);

    return TRUE;
} /* VpHbiAmbaRdWr() */


/* The following functions take less codespace when inlined.  Not all compilers
 * support inlining, though. */


/* VpHbiRd()
 *
 *  This is the primary HBI "read" function used by the VP API and is
 *  designed to read one instance of an object of the specified type from the
 *  DVP/VPP. HbiHalRd() is useful for reading VP objects such as "profiles"
 *  where the length of the object is fixed and there is more than one instance
 *  of that object. This function hides some of the details VpHalHbiRead(),
 *  making it safer and easier to use.
 *
 * Params
 *  'deviceId' - identifies the chip select for the device
 *  'page', 'offset', 'words' - these are filled in by the register info macros
 *      found above in this file.
 *  'words' - number of words to read, minus 1 (if zero, use VpHalHbiCmd)
 *  'pDest' - poiunter to buffer in which to place the data
 *
 * Returns
 *  This function returns TRUE on success, or FALSE on failure.
 *
 * Notes
 *  The reentrancy of this function depends on the reentrancy of
 *  VpHalHbiRead().
 *  If your compiler supports inline functions, this is a good candidate.
 *  On our system, inlining VpHbiWr() and VpHbiRd() reduces codesize.
 */
bool VpHbiRd(VpDeviceIdType deviceId, uint8 page, uint8 offset, uint8 words,
                uint16p pDest) {
    if (page == VP_DP) {
        return VpHalHbiRead(deviceId, HBI_CMD_DIRECT_READ(offset, words), words,
            pDest);
    }
    return VpHalHbiCmd(deviceId, HBI_CMD_SELECT_PAGE(page)) &&
        VpHalHbiRead(deviceId, HBI_CMD_PAGED_READ(offset, words), words, pDest);
} /* VpHbiRd() */


/* VpHbiWr()
 *
 *  This is the primary HBI "write" function used by the VP API and is
 *  designed to write one instance of an object of the specified type to the
 *  DVP/VPP. Like VpHbiRd(), VpHbiWr() is most useful for copying VP objects
 *  with fixed lengths and multiple instances. This function provides a slightly
 *  higher level of abstraction than VpHalHbiWrite().
 *
 * Params
 *  'deviceId' - identifies the chip select for the device
 *  'page', 'offset', 'words' - these are filled in by the register info macros
 *      found above in this file.
 *  'words' - number of words to write, minus 1 (if zero, use VpHalHbiCmd)
 *  'pSrc' points to the source buffer from which the data is copied to the
 *      HBI; use pSrc = (uint8p)0 to pass zeroes for all data words.
 *
 * Returns
 *  This function returns TRUE on success, or FALSE on failure.
 *
 * Notes
 *  The reentrancy of this function depends on the reentrancy of
 *  VpHalHbiWrite().
 *  If your compiler supports inline functions, this is a good candidate.
 *  On our system, inlining VpHbiWr() and VpHbiRd() reduces codesize.
 */
bool VpHbiWr(VpDeviceIdType deviceId, uint8 page, uint8 offset, uint8 words, 
                uint16p pSrc) {
    if (page == VP_DP) {
        return VpHalHbiWrite(deviceId, HBI_CMD_DIRECT_WRITE(offset, words),
            words, pSrc);
    }
    return VpHalHbiCmd(deviceId, HBI_CMD_SELECT_PAGE(page)) &&
        VpHalHbiWrite(deviceId, HBI_CMD_PAGED_WRITE(offset, words),
            words, pSrc);
} /* VpHbiWr() */

#ifdef VP_CC_VPP_SERIES
/* VpHbiRd8()
 *  This function is similar to VpHbiRd except for it is used to read a stream
 * of bytes. This function is useful when reading packets for VPP devices.
 *
 * Params
 *  'deviceId' - identifies the chip select for the device
 *  'page', Page from where the access needs to be performed.
 *  'words' - number of words to read, minus 1 (if zero, use VpHalHbiCmd)
 *  'pDest' - poiunter to buffer in which to place the data
 *
 * Returns
 *  This function returns TRUE on success, or FALSE on failure.
 */
bool VpHbiRd8(VpDeviceIdType deviceId, uint8 page, uint8 offset, uint8 words, 
                uint8p pDest) {
    if (page == VP_DP) {
        return VpHalHbiRead8(deviceId, HBI_CMD_DIRECT_READ(offset, words), 
                                words, pDest);
    }
    return VpHalHbiCmd(deviceId, HBI_CMD_SELECT_PAGE(page)) &&
           VpHalHbiRead8(deviceId, HBI_CMD_PAGED_READ(offset, words), 
                            words, pDest);
} /* VpHbiRd8() */


/* VpHbiWr8()
 *  This function is similar to VpHbiWr except for it is used to write a stream
 * of bytes. This function is useful when writing packets for VPP devices.
 *
 * Params
 *  'deviceId' - identifies the chip select for the device
 *  'page', Page from where the access needs to be performed.
 *  'words' - number of words to write, minus 1 (if zero, use VpHalHbiCmd)
 *  'pSrc' points to the source buffer from which the data is copied to the
 *      HBI; use pSrc = (uint8p)0 to pass zeroes for all data words.
 *
 * Returns
 *  This function returns TRUE on success, or FALSE on failure.
 */
bool VpHbiWr8(VpDeviceIdType deviceId, uint8 page, uint8 offset, uint8 words, 
                uint8p pSrc) {
    if (page == VP_DP) {
        return VpHalHbiWrite8(deviceId, HBI_CMD_DIRECT_WRITE(offset, words), 
                                words, pSrc);
    }
    return VpHalHbiCmd(deviceId, HBI_CMD_SELECT_PAGE(page)) &&
	       VpHalHbiWrite8(deviceId, HBI_CMD_PAGED_WRITE(offset, words), 
	                        words, pSrc);
} /* VpHbiWr8() */
#endif /* VP_CC_VPP_SERIES */




#endif /* (VP_CC_DVP_SERIES) || (VP_CC_VPP_SERIES) */




