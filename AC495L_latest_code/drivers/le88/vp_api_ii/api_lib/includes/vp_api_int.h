/** \file vp_api_int.h
 * vp_api_int.h
 *
 * Header file for the API-II c files. This file contains the declarations
 * and defintions required to implement the VTD specific VP-API. This file
 * is used by VP-API internally. This file should not be included by the
 * application.
 *
 * Copyright (c) 2005, Legerity Inc.
 * All rights reserved
 *
 * This software is the property of Legerity , Inc. Please refer to the
 * Non Disclosure Agreement (NDA) that you have signed for more information
 * on legal obligations in using, modifying or distributing this file.
 */

#ifndef VP_API_INT_H
#define VP_API_INT_H

#include "vp_api_cfg.h"

#include "vp_api_types.h"
#include "vp_api.h"

#include "vp_debug.h"

/******************************************************************************
 *                        		VP-API Defines                    			  *
 ******************************************************************************/
#define VP_ALL_LINES            0xFF  /**< Internal constant used to identify
                                       * all lines
                                       */

/**< Profile types as defined by Profile Wizard */
typedef enum {
    VP_PRFWZ_PROFILE_AC,
    VP_PRFWZ_PROFILE_DC,
    VP_PRFWZ_PROFILE_TONE,
    VP_PRFWZ_PROFILE_TONECAD,
    VP_PRFWZ_PROFILE_RING,
    VP_PRFWZ_PROFILE_CID_TYPE1,
    VP_PRFWZ_PROFILE_CID_TYPE2,
    VP_PRFWZ_PROFILE_METER,
    VP_PRFWZ_PROFILE_RINGCAD,
	VP_PRFWZ_PROFILE_TEST_CRITERIA,
	VP_PRFWZ_PROFILE_TEST_TOPOLOGY,
    VP_PRFWZ_PROFILE_FXS_CTRL,
    VP_PRFWZ_PROFILE_METERING_GEN,          /**< API Internal Profile Type */
    VP_PRFWZ_PROFILE_HOOK_FLASH_DIG_GEN,    /**< API Internal Profile Type */
    VP_PRFWZ_PROFILE_DIAL_PULSE_DIG_GEN,    /**< API Internal Profile Type */
    VP_PRFWZ_PROFILE_DTMF_DIG_GEN,          /**< API Internal Profile Type */
    VP_PRFWZ_PROFILE_MSG_WAIT_PULSE_INT,    /**< API Internal Profile Type */
    VP_PRFWZ_PROFILE_LINE_CAL_INT,          /**< API Internal Profile Type */    
	VP_PRFWZ_PROFILE_FWD_DISC_INT,          /**< API Internal Profile Type */
	VP_PRFWZ_PROFILE_POLREV_PULSE_INT,      /**< API Internal Profile Type */
    VP_PRFWZ_PROFILE_FXO_CONFIG = 0xFE,
    VP_PRFWZ_PROFILE_DEVICE = 0xFF,
    VP_PRFWZ_PROFILE_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE/* Portability Req.*/
} VpProfileWizProfileType;

typedef enum {
    VP_PRFWZ_CUSTOM_ST_STANDBY = 0x00,
    VP_PRFWZ_CUSTOM_ST_ACTIVE = 0x02,
    VP_PRFWZ_CUSTOM_ST_ACTIVE_PR = 0x03,
    VP_PRFWZ_CUSTOM_ST_TIP_OPEN = 0x04,
    VP_PRFWZ_CUSTOM_ST_OHT = 0x06,
    VP_PRFWZ_CUSTOM_ST_OHT_PR = 0x07,
    VP_PRFWZ_CUSTOM_ST_DISCONNECT = 0x08,
    VP_PRFWZ_CUSTOM_ST_RINGING = 0x09,
    VP_PRFWZ_CUSTOM_ST_DET_MAP = 0xFF,
    VP_PRFWZ_CUSTOM_ST_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE/* Portability Req.*/
} VpProfileWizCustomStateType;

/** These locations are common to all profiles used in the API */
typedef enum
{
    VP_PROFILE_TYPE_MSB = 0,
    VP_PROFILE_TYPE_LSB = 1,
    VP_PROFILE_INDEX = 2,
	VP_PROFILE_LENGTH = 3,
	VP_PROFILE_VERSION = 4,
	VP_PROFILE_MPI_LEN = 5,
    VP_PROFILE_HDR_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpProfileHeaderFieldType;

/* Define the Ringing Type (balanced/unbalanced) offset from end of MPI data */
#define VP_PROFILE_RING_TYPE_OFFSET     1

typedef enum
{
    /*
     * These parameters are the absolute locations of the specified value in
     * the CID profile
     */
    VP_CID_PROFILE_FSK_PARAM_LEN = 6,
    VP_CID_PROFILE_FSK_PARAM_CMD = 7,
    VP_CID_PRFT_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpCidProfileFieldType;

typedef enum
{
    /*
     * These parameters are offset from the end of the FSK data to the value
     * specified
     */
    VP_CID_PROFILE_CHECKSUM_OFFSET_MSB = 1,
    VP_CID_PROFILE_CHECKSUM_OFFSET_LSB = 2,
    VP_CID_PROFILE_LENGTH_OF_ELEMENTS_MSB = 3,
    VP_CID_PROFILE_LENGTH_OF_ELEMENTS_LSB = 4,
    VP_CID_PROFILE_START_OF_ELEMENTS_MSB = 5,
    VP_CID_PROFILE_START_OF_ELEMENTS_LSB = 6,
    VP_CID_PRFOFF_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpCidProfileOffsetFieldType;

/** These locations are for Tone and Ringing Cadence (Sequencer) profiles */
typedef enum
{
    VP_PROFILE_TYPE_SEQUENCER_COUNT_MSB = 6,
    VP_PROFILE_TYPE_SEQUENCER_COUNT_LSB = 7,
    VP_PROFILE_TYPE_SEQUENCER_START = 8,
    VP_PROFILE_LOC_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpProfileLocationParamsSequencer;

/******************************************************************************
 *                        API Specific common FUNCTION PROTOTYPES             *
 ******************************************************************************/
VpProfileType
ConvertPrfWizPrfType2ApiType(
	const VpProfileWizProfileType type);

EXTERN int
GetProfileIndex (
    const VpProfilePtrType pProfile);

EXTERN bool
VpVerifyProfileType(
    VpProfileType type,
    VpProfilePtrType pProfile);

EXTERN bool
VpIsDigit(
    VpDigitType digit);

#if defined (VP_CC_DVP_SERIES)
/******************************************************************************
 *                        DVP Specific FUNCTION PROTOTYPES                    *
 ******************************************************************************/
VpStatusType
VpMakeDvpDeviceObject(
	VpDevCtxType *pDevCtx,
	VpDvpDeviceObjectType *pDevObj);

VpStatusType
VpMakeDvpDeviceCtx(
    VpDevCtxType *pDevCtx,
    VpDvpDeviceObjectType *pDevObj);

VpStatusType
VpMakeDvpLineObject(
	VpTermType termType,
	uint8 channelId,
	VpLineCtxType *pLineCtx,
    VpDvpLineObjectType *pLineObj,
	VpDevCtxType *pDevCtx);
#endif /* VP_CC_DVP_SERIES */

#if defined (VP_CC_VPP_SERIES)
/******************************************************************************
 *                        VPP Specific FUNCTION PROTOTYPES                    *
 ******************************************************************************/
VpStatusType
VpMakeVppDeviceObject(
	VpDevCtxType *pDevCtx,
	VpVppDeviceObjectType *pDevObj);

VpStatusType
VpMakeVppDeviceCtx(
    VpDevCtxType *pDevCtx,
    VpVppDeviceObjectType *pDevObj);

VpStatusType
VpMakeVppLineObject(
	VpTermType termType,
	uint8 channelId,
	VpLineCtxType *pLineCtx,
    VpVppLineObjectType *pLineObj,
	VpDevCtxType *pDevCtx);
#endif /* VP_CC_VPP_SERIES */


/* Include the necessary files depending on the requirement of the project */
#if defined (VP_CC_790_SERIES)

#ifndef VP790_EC_CH1
#define VP790_EC_CH1    0x01
#define VP790_EC_CH2    0x02
#define VP790_EC_CH3    0x04
#define VP790_EC_CH4    0x08
#endif

VpStatusType
VpMakeVp790DeviceObject(
	VpDevCtxType *pDevCtx,
	Vp790DeviceObjectType *pDevObj);

VpStatusType
VpMakeVp790DeviceCtx(
    VpDevCtxType *pDevCtx,
    Vp790DeviceObjectType *pDevObj);

VpStatusType
VpMake790LineObject(
	VpTermType termType,
	uint8 channelId,
	VpLineCtxType *pLineCtx,
	Vp790LineObjectType *pLineObj,
	VpDevCtxType *pDevCtx);

VpStatusType
Vp790CommandInstruction(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pSeqData);

void 
Vp790MuteChannel(
    VpLineCtxType *pLineCtx,
    bool mode);

bool
Vp790FSKGeneratorReady(
    VpLineCtxType *pLineCtx);

bool
Vp790CliGetEncodedByte(
    VpLineCtxType *pLineCtx,
    uint8 *pByte);

VpStatusType
Vp790CtrlSetCliTone(
    VpLineCtxType *pLineCtx,
    bool mode);

void 
Vp790CtrlSetFSKGen(
    VpLineCtxType *pLineCtx,
    VpCidGeneratorControlType mode, 
    uint8 data);    
#endif

#if defined (VP_CC_880_SERIES)

#ifndef VP880_EC_CH1
#define VP880_EC_CH1                0x01
#define VP880_EC_CH2                0x02
#endif

#ifndef VP880_WIDEBAND_MODE
#define VP880_WIDEBAND_MODE         0x20
#endif

VpStatusType
VpMakeVp880DeviceObject(
	VpDevCtxType *pDevCtx,
	Vp880DeviceObjectType *pDevObj);

VpStatusType
VpMakeVp880DeviceCtx(
    VpDevCtxType *pDevCtx,
    Vp880DeviceObjectType *pDevObj);

VpStatusType
VpMakeVp880LineObject(
	VpTermType termType,
	uint8 channelId,
	VpLineCtxType *pLineCtx,
	Vp880LineObjectType *pLineObj,
	VpDevCtxType *pDevCtx);

VpStatusType
Vp880CommandInstruction(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pSeqData);

void
Vp880MuteChannel(
    VpLineCtxType *pLineCtx,
    bool mode);

bool
Vp880FSKGeneratorReady(
    VpLineCtxType *pLineCtx);

bool
Vp880CliGetEncodedByte(
    VpLineCtxType *pLineCtx,
    uint8 *pByte);

VpStatusType
Vp880CtrlSetCliTone(
    VpLineCtxType *pLineCtx,
    bool mode);

void
Vp880CtrlSetFSKGen(
    VpLineCtxType *pLineCtx,
    VpCidGeneratorControlType mode,
    uint8 data);

VpStatusType
Vp880SetDTMFGenerators(
    VpLineCtxType *pLineCtx,
    VpCidGeneratorControlType mode,
    VpDigitType digit);

#endif

#if defined (VP_CC_580_SERIES)
VpStatusType
VpMakeVp580DeviceObject(
	VpDevCtxType *pDevCtx,
	Vp580DeviceObjectType *pDevObj);

VpStatusType
VpMakeVp580DeviceCtx(
    VpDevCtxType *pDevCtx,
    Vp580DeviceObjectType *pDevObj);

VpStatusType
VpMakeVp580LineObject(
	VpTermType termType,
	uint8 channelId,
	VpLineCtxType *pLineCtx,
	Vp580LineObjectType *pLineObj,
	VpDevCtxType *pDevCtx);

VpStatusType
Vp580CommandInstruction(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pSeqData);

#endif

/* Functions used internal to the API -- device dependant */
#if defined (VP_CC_790_SERIES) || defined (VP_CC_880_SERIES) \
 || defined (VP_CC_580_SERIES)

/* Converts ms to units of API_TICKRATE */
#define MS_TO_TICKRATE(MS, API_TICKRATE)  \
    ((uint16)(((((uint32)MS*256*2)/API_TICKRATE)+1)/2))
#define TICKS_TO_MS(TICKS, API_TICKRATE)  \
    (uint16) (((uint32)API_TICKRATE * (uint32)TICKS) / 256)

#define NO_DATA     0x00
#define NOOP_CMD    0x06  /**< Standard No operation comnmand for all devices */

#define MAX_CFAIL_TEST      10
#define CFAIL_TEST_INTERVAL 10

/**< CallerID Profile Data structure definitions */
#define VP_FSK_MARK_SIGNAL     0xFF
#define VP_FSK_CHAN_SEIZURE    0x55

typedef enum
{
    VP_CLI_NULL = 0,
    VP_CLI_POLREV,              /**< No Parameters */
    VP_CLI_MUTEON,              /**< No Parameters */
    VP_CLI_MUTEOFF,             /**< No Parameters */
    VP_CLI_ALERTTONE,           /**< Duration/Tone definition parameters */
    VP_CLI_ALERTTONE2,          /**< Internal Alert Tone indicator */
    VP_CLI_SILENCE,             /**< Duration Parameter */
    VP_CLI_SILENCE_MASKHOOK,    /**< Duration/Masked-hook length parameters */
    VP_CLI_DETECT,              /**< Tone and Timeout parameters */
    VP_CLI_CHANSEIZURE,         /**< Duration Parameter */
    VP_CLI_MARKSIGNAL,          /**< Duration Parameter */
    VP_CLI_MESSAGE,             /**< No Parameters */
    VP_CLI_USER_DEFINED,        /**< Not supported in API-II */
    VP_CLI_EOT = 0x0D,          /**< No Parameters */
    VP_CLI_DTMF_MESSAGE = 0x0E, /**< Message data to be sent in DTMF format */
    VP_CLI_ELM_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpApiElementsType;

void
VpImplementNonMaskEvents(
    VpOptionEventMaskType *pLineEventsMask,
	VpOptionEventMaskType *pDevEventsMask);

VpStatusType
VpImplementDefaultSettings(
    VpDevCtxType *pDevCtx,
    VpLineCtxType *pLineCtx);

/* VP API Function Prototypes - Not associated with a function pointer */
uint8
ConvertApiState2PrfWizState(
    const VpLineStateType state);

void
InitCadenceVars(
    VpLineCtxType *pLineCtx);

void
InitTimerVars(
    VpLineCtxType *pLineCtx);

bool
VpServiceSeq (
    VpDevCtxType *pDevCtx);

VpStatusType
VpCidSeq(
    VpLineCtxType *pLineCtx);

void
VpCliStopCli(
    VpLineCtxType *pLineCtx);

bool
VpUpdateDP(
	uint16 tickRate,
    VpOptionPulseType *pPulseSpecs,
	VpDialPulseDetectType *pDpStruct,
	VpOptionEventMaskType *pLineEvents);

void
VpInitDP(
	VpDialPulseDetectType *pDpStruct);

VpStatusType
VpCSLACGetLineStatus(
    VpLineCtxType *pLineCtx,
    VpInputType input,
    bool *pStatus);

VpStatusType
VpCSLACInitMeter(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pMeterProfile);

VpStatusType
VpCSLACStartMeter(
    VpLineCtxType *pLineCtx,
    uint16 onTime,
    uint16 offTime,
    uint16 numMeters);

VpStatusType
VpCSLACClearResults(
    VpDevCtxType *pDevCtx);

VpStatusType
VpCSLACDtmfDigitDetected(
    VpLineCtxType *pLineCtx,
    VpDigitType digit,
    VpDigitSenseType sense);

void
VpConvertFixed2Csd(
    uint16 fixed,
    uint8 *csdBuf);

uint16
VpConvertCsd2Fixed(
    uint8 *csdBuf);

#endif

#if defined (VP_CC_790_SERIES)
VpStatusType
Vp790SetLineStateInt(
    VpLineCtxType *pLineCtx,
    VpLineStateType state);

VpStatusType
Vp790SetLineTone(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pToneProfile,
    VpProfilePtrType pCadProfile,
    VpDtmfToneGenType *pDtmfControl);
#endif

#if defined (VP_CC_880_SERIES)
VpStatusType
Vp880SetLineStateInt(
    VpLineCtxType *pLineCtx,
    VpLineStateType state);

VpStatusType
Vp880SetLineTone(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pToneProfile,
    VpProfilePtrType pCadProfile,
    VpDtmfToneGenType *pDtmfControl);
    
#endif

#if defined (VP_CC_580_SERIES)
VpStatusType
Vp580SetLineStateInt(
    VpLineCtxType *pLineCtx,
    VpLineStateType state);

VpStatusType
Vp580SetLineTone(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pToneProfile,
    VpProfilePtrType pCadProfile,
    VpDtmfToneGenType *pDtmfControl);
    
#endif

#endif /* VP_API_INT_H */




