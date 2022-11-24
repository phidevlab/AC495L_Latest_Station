/** \file hbi_common.h
 * hbi_common.h
 *
 * This file declares the DVP Host Bus Interface layer register/mail box
 * mapping.
 *
 * Copyright (c) 2005, Legerity Inc.
 * All rights reserved
 *
 * This software is the property of Legerity , Inc. Please refer to the
 * Non Disclosure Agreement (NDA) that you have signed for more information
 * on legal obligations in using, modifying or distributing this file.
 */


#ifndef _HBI_COMMON_H
#define _HBI_COMMON_H

#include "vp_hal.h"

/******************************************************************************
 *                        		DEFINES					                      *
 ******************************************************************************/
 #define VP_HBI_PAGE_SIZE            0x80
/* HBI Direct Page (Hardware registers common to DVP/VPP).
 *
 * page offset linear  contents                         (len * cnt = tot = hex)
 * =-=-=-=-=-=-=-=-=-=-=-=-=  PAGE BOUNDARY  =-=-=-=-=-=-=-=-=-=-=-=-=-=
 *  DP    00   DVP HW REG: INTIND               (  1 *   1 =   1 =   1)
 * ---------------------------------------------------------------------
 *  DP    01   DVP HW REG: INTPARAM             (  1 *   1 =   1 =   1)
 * ---------------------------------------------------------------------
 *  DP    02   DVP HW REG: MBOXFLAG             (  1 *   1 =   1 =   1)
 * ---------------------------------------------------------------------
 *  DP    03   DVP HW REG: CRC255               (  1 *   1 =   1 =   1)
 * ---------------------------------------------------------------------
 *  DP    04   DVP HW REG: BASE255              (  1 *   1 =   1 =   1)
 * ---------------------------------------------------------------------
 *  DP    05   DVP HW REG: MBOFFSET             (  1 *   1 =   1 =   1)
 * ---------------------------------------------------------------------
 *  DP    06   DVP HW REG: HWRES                (  1 *   1 =   1 =   1)
 * ---------------------------------------------------------------------
 *  DP    07   DVP HW REG: PCLKSEL              (  1 *   1 =   1 =   1)
 * ---------------------------------------------------------------------
 *  DP    08   DVP HW REG: PCMCLKSLOT           (  1 *   1 =   1 =   1)
 * ---------------------------------------------------------------------
 *  DP    09   DVP HW REG: SYSINTSTAT           (  1 *   1 =   1 =   1)
 * ---------------------------------------------------------------------
 *  DP    0A   DVP HW REG: SYSINTMASK           (  1 *   1 =   1 =   1)
 * ---------------------------------------------------------------------
 */
#define VP_DP 254    /* magic page number indicating direct page */
                        /*      page,   offset, words - 1*/
#define HW_Reg_INTIND           VP_DP,  0x00,   0
#define HW_Reg_INTPARAM         VP_DP,  0x01,   0
#define HW_Reg_INTIND_AND_INTPARAM         VP_DP,  0x00,   1
#define HW_Reg_MBOXFLAG         VP_DP,  0x02,   0
#define HW_Reg_CRC255           VP_DP,  0x03,   0
#define HW_Reg_BASE255          VP_DP,  0x04,   0
#define HW_Reg_MBOFFSET         VP_DP,  0x05,   0
#define HW_Reg_HWRES            VP_DP,  0x06,   0
#define HW_Reg_PCLKSEL          VP_DP,  0x07,   0
#define HW_Reg_PCMCLKSLOT       VP_DP,  0x08,   0
#define HW_Reg_SYSINTSTAT       VP_DP,  0x09,   0
#define HW_Reg_SYSINTMASK       VP_DP,  0x0A,   0

/* HBI commands: */
#define HBI_CMD_PAGED_READ(offset, length)   (0x0000 + ((uint16)offset << 8) + \
                                                length)
#define HBI_CMD_PAGED_WRITE(offset, length)  (0x0080 + ((uint16)offset << 8) + \
                                                length)
#define HBI_CMD_DIRECT_READ(offset, length)  (0x8000 + ((uint16)offset << 8) + \
                                                length)
#define HBI_CMD_DIRECT_WRITE(offset, length) (0x8080 + ((uint16)offset << 8) + \
                                                length)
#define HBI_CMD_START_MBOX_RD(length)        (0xF800 + length)
#define HBI_CMD_START_MBOX_WR(length)        (0xF900 + length)
#define HBI_CMD_CONT_MBOX_RD(length)         (0xFA00 + length)
#define HBI_CMD_CONT_MBOX_WR(length)         (0xFB00 + length)
#define HBI_CMD_CONFIGURE(options)           (0xFD00 + options)
#define HBI_CMD_SELECT_PAGE(page)            (0xFE00 + page)
#define HBI_CMD_NOP                          0xFFFF

/******************************************************************************
 *                        FUNCTION PROTOTYPES			                      *
 ******************************************************************************/
EXTERN bool VpHbiRd8(VpDeviceIdType deviceId, uint8 page, uint8 offset,
                        uint8 words, uint8p pDest);
EXTERN bool VpHbiWr8(VpDeviceIdType deviceId, uint8 page, uint8 offset,
                        uint8 words, uint8p pSrc);
EXTERN bool VpHbiRd(VpDeviceIdType deviceId, uint8 page, uint8 offset,
                        uint8 words, uint16p pDest);
EXTERN bool VpHbiWr(VpDeviceIdType deviceId, uint8 page, uint8 offset,
                        uint8 words, uint16p pSrc);
EXTERN bool VpHbiDvpVppReset(VpDeviceIdType deviceId);
EXTERN bool VpHbiSetBase255(VpDeviceIdType deviceId, uint32 amba_addr,
                        uint16p base255reg);
EXTERN bool VpHbiAmbaRdWr(bool readWrite, VpDeviceIdType deviceId,
                        uint32 amba_addr, uint32 numwords, uint16p data);

#define VpHbiAmbaWrite(deviceId, amba_addr, numwords, pData) \
    VpHbiAmbaRdWr(TRUE, deviceId, amba_addr, numwords, pData)
#define VpHbiAmbaRead(deviceId, amba_addr, numwords, pData) \
    VpHbiAmbaRdWr(FALSE, deviceId, amba_addr, numwords, pData)

#endif /* _HBI_COMMON_H */




