/** \file boot_common.c
 * boot_common.c
 *
 * This file contains all of the VP-API-II declarations that are Voice 
 * Termination Device (VTD) family independent.	The implementation in this file 
 * is applicable to DVP and VPP classes of devices.
 *
 * Copyright (c) 2005, Legerity Inc.
 * All rights reserved
 *
 * This software is the property of Legerity , Inc. Please refer to the
 * Non Disclosure Agreement (NDA) that you have signed for more information
 * on legal obligations in using, modifying or distributing this file.
 */

/* INCLUDES */
#include "vp_api.h"

/* Compile Further only if required */
#if defined (VP_CC_DVP_SERIES) || defined (VP_CC_VPP_SERIES) 

#include "boot_common.h"

#ifndef VP_NO_COMPRESS
#include "zlib.h"
#ifdef VP_COMPRESS_TEST
#ifndef debug_printf
/* Define the following as necessary to redirect the debug output */
#define debug_printf printk
#endif /* debug_printf */
bool veCompressTestEnable = FALSE;
# endif /* VP_COMPRESS_TEST */
#else /* VP_NO_COMPRESS */
# undef VP_COMPRESS_TEST
#endif /* VP_NO_COMPRESS */



/**
 * BootLoadImage()
 *  Loads an executable image into the VTD (DVP/VPP).
 *
 * Refer to the VoicePath API User's Guide for details about this function.
 */
VpStatusType
BootLoadImage(
	VpDevCtxType *pDevCtx, 
    VpBootStateType state,
    VpImagePtrType pImageBuffer,
    uint32 bufferSize,
    VpScratchMemType *pScratchMem,
    VpBootModeType validation
    )
{
	VpDeviceIdType deviceId;
	void *pDevObj = pDevCtx->pDevObj;
#ifndef VP_NO_COMPRESS
    VpZlibStreamType *stream = &(pScratchMem->stream); /* data structure for 
                                                        * inflate() I/O */
    uint8p inflated = pScratchMem->inflated; /* buffer for holding 
                                              * inflated data */
    int retval = Z_OK;
#endif /* !VP_NO_COMPRESS */
    int newblocks;
    VpImagePtrType inflatedHPtr;    /* a _huge pointer to inflated */
    uint16p pRecCnt;                /* number of records remaining */
    bool firstblock;                /* whether we're processing the first
                                       block in the image */
    uint16 data;
    int cFailCount;

	switch(pDevCtx->deviceType) {
#if defined (VP_CC_DVP_SERIES)
	    case VP_DEV_DVP_SERIES:
		    deviceId = ((VpDvpDeviceObjectType *)pDevObj)->deviceId;
            pRecCnt = &((VpDvpDeviceObjectType *)pDevObj)->recCnt;
			break;
#endif

#if defined (VP_CC_VPP_SERIES)
	    case VP_DEV_VPP_SERIES:
		    deviceId = ((VpVppDeviceObjectType *)pDevObj)->deviceId;
            pRecCnt = &((VpVppDeviceObjectType *)pDevObj)->recCnt;
			break;
#endif
		default:
            return VP_STATUS_INVALID_ARG;
	}

    switch (state) {
        case VP_BOOT_STATE_FIRSTLAST:
        case VP_BOOT_STATE_FIRST:
            firstblock = TRUE;
            break;
        case VP_BOOT_STATE_CONTINUE:
        case VP_BOOT_STATE_LAST:
            firstblock = FALSE;
            break;
        default:
            return VP_STATUS_INVALID_ARG;
    }

    /* User passes scratchMemPtr != VP_NULL for compressed images. */
    if (pScratchMem != VP_NULL) {

#ifdef VP_NO_COMPRESS
        /* If compression isn't compiled in, return with error. */
        return VP_STATUS_INVALID_ARG;
#else /* !VP_NO_COMPRESS */
        inflatedHPtr = inflated;

        if ((state == VP_BOOT_STATE_FIRSTLAST) || 
            (state == VP_BOOT_STATE_FIRST)) {

            /* Initialize data structures for VpZlibInflate().  Ignore retval
               (it's hard-coded to return Z_OK). */
            retval = VpZlibInflateInit2(stream, -1 * VP_COMPRESS_WINDOW_BITS,
                    &pScratchMem->state, pScratchMem->window);

            /* stream->next_in points to the input (deflated) buffer for 
             *      inflate().
             * stream->avail_in indicates the number of bytes available there.
             * stream->next_out points to the output (inflated) buffer, which is
             *     filled in by inflate() and must be dumped to VpHbiBootWr().
             * stream->avail_out indicates the amount of space remaining 
             *   there. */

            stream->next_out = &inflated[0];
            stream->avail_out = VP_COMPRESS_OUTBUF_PAGES * 128;

#ifdef VP_COMPRESS_TEST
            if (veCompressTestEnable) {
                stream->adler = adler32(0L, Z_NULL, 0);
            }
#endif /* VP_COMPRESS_TEST */

        }
        stream->avail_in = bufferSize;
        stream->next_in = pImageBuffer;
#endif /* !VP_NO_COMPRESS */

    } else {
        /* Uncompressed image.  Make sure length is a multiple of 128 bytes. */
        if ((bufferSize % 128) > 0) {
            /* debug_printf("Image ends with partial page."); */
            return VP_STATUS_ERR_IMAGE;
        }
    }

    /* As each chunk of pImageBuffer is processed, the length of the chunk will
       be subtracted from bufferSize.  Keep going until bufferSize = 0.  */
#ifdef VP_NO_COMPRESS
    while (bufferSize > 0){
#else /* !VP_NO_COMPRESS */
    while ((bufferSize > 0) && (retval != Z_STREAM_END)){
        /* If pImageBuffer is compressed... */
        if (pScratchMem != VP_NULL) {

            /* Inflate from pImageBuffer to the inflated[] buffer. */
            /* debug_printf("Calling inflate() with stream->avail_in = %lu, 
                stream->avail_out = %lu\n", (unsigned long)stream->avail_in, 
                (unsigned long)stream->avail_out); */
            retval = VpZlibInflate(stream, Z_SYNC_FLUSH);

            /* Handle inflate() error conditions. */
            if ((retval != Z_OK) && (retval != Z_STREAM_END)) {

#ifdef VP_COMPRESS_TEST
                if (veCompressTestEnable) {
                    debug_printf("inflate() returned %d\n", retval);
                }
#endif /* VP_COMPRESS_TEST */

                VpHbiDvpVppReset(deviceId); /* halt the DVP */
                return VP_STATUS_ERR_IMAGE;
            }

            if (stream->avail_out % 128) {
                /* We ran out of input, but didn't produce an integer number
                   of pages of output (i.e. a multiple of 128 bytes). */
                if (
                    (state == VP_BOOT_STATE_LAST) ||
                    (state == VP_BOOT_STATE_FIRSTLAST)
                ) {

#ifdef VP_COMPRESS_TEST
                    if (veCompressTestEnable) {
                        debug_printf("inflate() = %d; stream->avail_in = %lu;"
                                     " stream->avail_out = %lu\n", retval, 
                                     (unsigned long)stream->avail_in, 
                                     (unsigned long)stream->avail_out);
                    }
#endif /* VP_COMPRESS_TEST */

                    VpHbiDvpVppReset(deviceId); /* halt the DVP */
                    return VP_STATUS_ERR_IMAGE;
                } else {
                    /* Expecting another call to VpBootload() to continue. */
                    return VP_STATUS_SUCCESS;
                }
            }           

            /* Prepare to process the newly inflated block(s). */
            newblocks = ((VP_COMPRESS_OUTBUF_PAGES * 128) - 
                            stream->avail_out) / 128;
            inflatedHPtr = inflated;

            /* Reset output pointer to the beginning of the output buffer. */
            stream->next_out = &inflated[0];
            stream->avail_out = VP_COMPRESS_OUTBUF_PAGES * 128;

            /* Reduce bufferSize to match the amount of input data remaining. */
            bufferSize = stream->avail_in;
  
        } else
#endif /* !VP_NO_COMPRESS */
        {
            /* Prepare to process the blocks directly from pImageBuffer. */
            inflatedHPtr = pImageBuffer;
            newblocks = bufferSize / 128;
            bufferSize = 0;
        }

        /* Process the new block(s). */
        while (newblocks--) {

            /* First block in the DVP image contains special data. */
            if (firstblock) {

#ifdef VP_COMPRESS_TEST
            if (!veCompressTestEnable) {
#endif /* VP_COMPRESS_TEST */

                /* Sanity check for magic numbers at the beginning of the 
                 * boot stream. */
                if ((inflatedHPtr[0] != 0xFE) || (inflatedHPtr[1] != 0xFF)) {
                    /* debug_printf("Magic numbers = %hX %hX\n", 
                     * (uint16)inflatedHPtr[0], (uint16)inflatedHPtr[1]); */
                    return VP_STATUS_ERR_IMAGE;
                }

                /* Extract the record count from the boot stream. */
                *pRecCnt = ((uint16)inflatedHPtr[6] << 8) + inflatedHPtr[7];
                /* debug_printf("initial recCnt = %lu\n", 
                 * (unsigned long) recCnt); */

                /* Initialize the HBI and reset the DVP. */
                if (
                    (VpHalHbiInit(deviceId) == FALSE) ||
                    (VpHbiDvpVppReset(deviceId) == FALSE) /* initiate boot 
                                                           * sequence */
                ) {
                    return VP_STATUS_ERR_HBI;
                }

                /* Check to make sure clock fault is not present; Provide
                 * enough time such that internal PLL settles.
                 * Removal of CFAIL bit also means clock autodetection
                 * is complete.
                 */
                #define SYSINTSTAT_CFAIL_BIT_MASK 0x1000
                #define CFAIL_LOOP_COUNT 2000 /* Assuming 100nS per access,
                   * and to wait a maximum of 200micro seconds */
                cFailCount = 0;
                do {
                    VpHbiRd(deviceId, HW_Reg_SYSINTSTAT, &data);
                    cFailCount++;
                } while (((data & SYSINTSTAT_CFAIL_BIT_MASK) ==
                                                 SYSINTSTAT_CFAIL_BIT_MASK) &&
                         (cFailCount < CFAIL_LOOP_COUNT));

                /* DevNotes: It is probably OK to continue; We will probably
                 * get a Page 255 CRC error */

                #ifdef DEBUG_BOOTLOAD
                term_printf("Sys Int register = 0x%.4x\n", data);
                #endif /* DEBUG_BOOTLOAD */

#ifdef VP_CLEAR_CODE_MEM
                if (
                    /* Clear CODE PMRAM Pages 0, 1, 2, 3 */
                    /* Leave CM 0 untouched, so the DVP won't start running. */
                    (VpHbiAmbaWrite(deviceId, 0x10000, 0x0BFFF, (uint16p)0) == 
                        FALSE) ||
                
                    /* Clear CODE PMRAM Page 4 */
                    (VpHbiAmbaWrite(deviceId, 0x08000, 0x02FFF, (uint16p)0) == 
                        FALSE) ||

                    /* Clear CODE PMRAM Fixed Page, but skip over CM 0 */
                    (VpHbiAmbaWrite(deviceId, 0x1C002, 0x023FD, (uint16p)0) == 
                        FALSE)
                ) {
                    return VP_STATUS_ERR_HBI;
                }
#endif /* VP_CLEAR_CODE_MEM */

#ifdef VP_COMPRESS_TEST
            }
#endif /* VP_COMPRESS_TEST */

                firstblock = FALSE;
            }

#ifdef VP_COMPRESS_TEST
        if (veCompressTestEnable) {
            stream->adler = adler32(stream->adler, inflatedHPtr, 128);
        } else {
#endif /* VP_COMPRESS_TEST */

            /* Transmit the block over the HBI bus. */
            if (!VpHalHbiBootWr(deviceId, 63,inflatedHPtr)) {
                VpHbiDvpVppReset(deviceId); /* halt the DVP */
                return VP_STATUS_ERR_HBI;
            }

#ifdef VP_COMPRESS_TEST
        }
#endif /* VP_COMPRESS_TEST */

            (*pRecCnt)--;
            inflatedHPtr = &inflatedHPtr[128];
        } /* while (newblocks) */
    } /* until we run out of input data */

#ifdef VP_COMPRESS_TEST
if (veCompressTestEnable) {
    if ((state == VP_BOOT_STATE_FIRSTLAST) || (state == VP_BOOT_STATE_LAST)) {
        term_printf("(adler32 = %8.8lXh) ", stream->adler);
    }
} else {
#endif /* VP_COMPRESS_TEST */

    /* If we've sent all the blocks, check the hardware checksum and 
     * record count. */
    if ((state == VP_BOOT_STATE_FIRSTLAST) || (state == VP_BOOT_STATE_LAST)) {
        if (validation == VP_BOOT_MODE_VERIFY) {
            /* recCnt should be 0 after decrementing once for each block sent.*/
            if (*pRecCnt != 0) {
                /* debug_printf("recCnt = %lu\n", (unsigned long) recCnt); */
                VpHbiDvpVppReset(deviceId); /* halt the DVP */
                return VP_STATUS_ERR_IMAGE;
            }

            /* Recycle 'recCnt' to read DVP page 255 CRC register. */
            VpHbiRd(deviceId, HW_Reg_CRC255, pRecCnt);

            /* Test for page 255 CRC equal to expected value. */
            if (*pRecCnt != 0xAA55) {
                /* debug_printf("CRC255 = %hX\n", recCnt); */
                VpHbiDvpVppReset(deviceId); /* halt the DVP */
                #ifdef DEBUG_BOOTLOAD
                term_printf("Error: Page 255 CRC = 0x%.4x\n", *pRecCnt);
                #endif /* DEBUG_BOOTLOAD */
                return VP_STATUS_ERR_VERIFY;
            }
        }
    }

#ifdef VP_COMPRESS_TEST
}
#endif /* VP_COMPRESS_TEST */

    return VP_STATUS_SUCCESS;
} /* BootLoadImage() */

#endif /* (VP_CC_DVP_SERIES) || (VP_CC_VPP_SERIES) */




