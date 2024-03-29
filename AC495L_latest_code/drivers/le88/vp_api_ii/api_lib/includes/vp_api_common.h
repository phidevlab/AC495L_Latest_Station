/** \file vp_api_common.h
 * vp_api_common.h
 *
 * Header file for the VP-API-II c files.
 *
 * This file contains all of the VP-API-II declarations that are Voice
 * Termination Device (VTD) family independent.
 *
 * Copyright (c) 2005, Legerity Inc.
 * All rights reserved
 *
 * This software is the property of Legerity , Inc. Please refer to the
 * Non Disclosure Agreement (NDA) that you have signed for more information
 * on legal obligations in using, modifying or distributing this file.
 */

#ifndef VP_API_COMMON_H
#define VP_API_COMMON_H

#include "vp_api_types.h"
#include "vp_api_dev_term.h"
#include "vp_api_event.h"
#include "vp_api_option.h"
#include "vp_api_test.h"


/******************************************************************************
 *                                DEFINES                                     *
 ******************************************************************************/

/*
 * Profile Table Indexes
 */
#define VP_PTABLE_NULL      (VpProfilePtrType)0
#define VP_PTABLE_INDEX1    (VpProfilePtrType)1
#define VP_PTABLE_INDEX2    (VpProfilePtrType)2
#define VP_PTABLE_INDEX3    (VpProfilePtrType)3
#define VP_PTABLE_INDEX4    (VpProfilePtrType)4
#define VP_PTABLE_INDEX5    (VpProfilePtrType)5
#define VP_PTABLE_INDEX6    (VpProfilePtrType)6
#define VP_PTABLE_INDEX7    (VpProfilePtrType)7
#define VP_PTABLE_INDEX8    (VpProfilePtrType)8
#define VP_PTABLE_INDEX9    (VpProfilePtrType)9
#define VP_PTABLE_INDEX10   (VpProfilePtrType)10
#define VP_PTABLE_INDEX11   (VpProfilePtrType)11
#define VP_PTABLE_INDEX12   (VpProfilePtrType)12
#define VP_PTABLE_INDEX13   (VpProfilePtrType)13
#define VP_PTABLE_INDEX14   (VpProfilePtrType)14
#define VP_PTABLE_INDEX15   (VpProfilePtrType)15
#define VP_PTABLE_MAX_INDEX VP_PTABLE_INDEX15


/******************************************************************************
 *                    ENUMERATIONS AND NEW DATA TYPES                         *
 ******************************************************************************/
/** Standard return value for most API library functions */
typedef enum
{
    VP_STATUS_SUCCESS,              /**< Function excecuted successfully */
    VP_STATUS_FAILURE,              /**< Function excecution failed due to
                                     * unspecified reason */
    VP_STATUS_FUNC_NOT_SUPPORTED,   /**< Function not supported for the device*/
    VP_STATUS_INVALID_ARG,          /**< One or more arguments to the function
                                     * are invalid. Under this condition no
                                     * command is issued to the VTD */
    VP_STATUS_MAILBOX_BUSY,         /**< Function failed because VPP�s or DVP�s
                                     * downstream mail box is busy. Possible for
                                     * those functions which use the mailbox.
                                     * The same API function may not result
                                     * this error if called after some time.
                                     * Please see vp_api_cfg.h for mechanisms
                                     * to configure so that application does
                                     * not have to handle this error message.
                                     */
    VP_STATUS_ERR_VTD_CODE,         /**< Mismatch in device type or termination
                                     * type or VP-API compiled code. Returned
                                     * by VpMakeDeviceObject() and
                                     * VpMakeLineObject() */
    VP_STATUS_OPTION_NOT_SUPPORTED, /**< Specified option not supported for the
                                     * device. Used in VpSetOption() and
                                     * VpGetOption(). */
    VP_STATUS_ERR_VERIFY,           /**< The boot load verification process
                                     * indicates that one or more bytes were
                                     * incorrectly loaded. Applicable for DVP
                                     * and VPP in function VpBootLoad() */
    VP_STATUS_DEVICE_BUSY,          /* Resources to excecute the requested
                                     * function are not available. */
    VP_STATUS_MAILBOX_EMPTY,        /**< Mailbox is empty; Data was expected by
                                     * the calling function. */
    VP_STATUS_ERR_MAILBOX_DATA,     /**< Mailbox data does not match the type of
                                     * data that was expected. */
    VP_STATUS_ERR_HBI,              /**< An error occured due to mistmacth in
                                     * HBI interface definition compared to what
                                     * is being requested. */
    VP_STATUS_ERR_IMAGE,            /**< Error in DVP/VPP image */
    VP_STATUS_IN_CRTCL_SECTN,       /**< API is excecuting critical section of
                                     * the code. Hence it cannot support the
                                     * requested service. */
    VP_STATUS_DEV_NOT_INITIALIZED,  /**< Device is not initalized. The device
                                     * needs to be initialized before calling
                                     * the function that returned this error. */
    VP_STATUS_ERR_PROFILE,          /**< An error experienced in the profile.
                                     * Example: Error in profile type, profile
                                     * length error, or any other error in
                                     * the profile. This error is also
                                     * generated when a profile table index is
                                     * passed that is not initialized.  */
	VP_STATUS_INVALID_VOICE_STREAM,	/* This error is generated when an invalid
	                                 * stream identfier is passed to a function
	                                 * that accepts such an argument. */
    VP_STATUS_CUSTOM_TERM_NOT_CFG,  /* Indicates a custom termination type has
                                     * been defined but not yet configured */                                     
    VP_STATUS_NUM_TYPES,            /**< NOT a ERROR, Just used for coding
                                     * purposes */
    VP_STATUS_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpStatusType;

/* Enumeration for Boot load Function */
typedef enum
{
    VP_BOOT_STATE_FIRST,        /* First block to download */
    VP_BOOT_STATE_CONTINUE,     /* Additional block to download */
    VP_BOOT_STATE_LAST,         /* Last block to download */
    VP_BOOT_STATE_FIRSTLAST,    /* First and only block to download */
    VP_NUM_BOOT_STATES,
    VP_BOOT_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpBootStateType;

/* Enumeration for Boot load Function */
typedef enum
{
    VP_BOOT_MODE_NO_VERIFY,     /* No write verification is performed */
    VP_BOOT_MODE_VERIFY,        /* Verify Load Image Checksum */
    VP_NUM_BOOT_MODES,
    VP_BOOT_MODE_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpBootModeType;

/* Version and checksum information for VPP/DVP */
typedef struct
{
    uint16 vtdRevCode;          /* Silicon Revision for VTD */
    uint8 swProductId;          /* VTD Firmware ID */
    uint8 swVerMajor;           /* Major Revision for VTD Firmware */
    uint8 swVerMinor;           /* Minor Revision for VTD Firmware */
} VpVersionInfoType;

typedef struct
{
    uint32 loadChecksum;        /* Calculated Checksum for Code Image */
    VpVersionInfoType vInfo;    /* Version Information for VTD HW/SW */
} VpChkSumType;

typedef enum
{
    VP_PROFILE_DEVICE,          /* Device profile */
    VP_PROFILE_AC,              /* AC profile */
    VP_PROFILE_DC,              /* DC profile */
    VP_PROFILE_RING,            /* Ring profile */
    VP_PROFILE_RINGCAD,         /* Ringing cadence profile */
    VP_PROFILE_TONE,            /* Tone profile */
    VP_PROFILE_METER,           /* Metering profile */
    VP_PROFILE_CID,             /* Caller ID profile */
    VP_PROFILE_TONECAD,         /* Tone cadence profile */
    VP_PROFILE_FXO_CONFIG,      /* FXO configuration profile */
    VP_PROFILE_CUSTOM_TERM,     /* Custom Termination Type profile */
    VP_PROFILE_TEST_CRITERIA,   /* Test criteria profile */
    VP_PROFILE_TEST_TOPOLOGY,   /* Test topology profile */
    VP_NUM_PROFILE_TYPES,
    VP_PROFILE_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpProfileType;

/**< typedef for VpCalCodec() function */
typedef enum {
    VP_DEV_CAL_NOW,     /* Calibrate immediately */
    VP_DEV_CAL_NBUSY,   /* Calibrate if all lines are "on-hook" */
    VP_NUM_DEV_CAL_TYPES,
    VP_DEV_CAL_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpDeviceCalType;

/* Valid line states for VpSetLineState() */
/* Defined else where */

/**< The following types are for VpSetLineTone() function */
typedef struct {
    VpDigitType toneId;  /* Identifies the DTMF tone to generated */
    VpDirectionType dir; /* Direction in which DTMF tone needs to be
                          * generated */
} VpDtmfToneGenType;

typedef enum {
  VP_DIGIT_GENERATION_DTMF,             /* Generate DTMF digit */
  VP_DIGIT_GENERATION_DIAL_PULSE,       /* Generate pulse digit */
  VP_DIGIT_GENERATION_DIAL_HOOK_FLASH,   /* Generate hook flash */
  VP_DIGIT_GEN_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpDigitGenerationType;

/**< The following types are for VpSetRelayState() function */
typedef enum {
    VP_RELAY_NORMAL,                /* No test access, ringing controlled
                                     * by line state */
    VP_RELAY_RESET,                 /* LCAS all-off state, test-out released */
    VP_RELAY_TESTOUT,               /* LCAS all-off state, test-out active */
    VP_RELAY_TALK,                  /* LCAS talk state, test-out released */
    VP_RELAY_RINGING,               /* LCAS ringing state, test-out released */
    VP_RELAY_TEST,                  /* LCAS test state, test-out released */
    VP_RELAY_BRIDGED_TEST,          /* LCAS test/monitor state,
                                     * test-out released */
    VP_RELAY_SPLIT_TEST,            /* LCAS test/monitor state,
                                     * test-out active */
    VP_RELAY_DISCONNECT,            /* LCAS talk state, test-out active */
    VP_RELAY_RINGING_NOLOAD,        /* LCAS ringing state, test-out active */
    VP_RELAY_RINGING_TEST,          /* LCAS test ringing state,
                                     * test-out active */
    VP_RELAY_RSVD1,
    VP_NUM_RELAY_STATES,
    VP_RELAY_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpRelayControlType;

/**< The following types are for VpSetRelGain() function */
typedef enum {
    VP_GAIN_SUCCESS,    /* Gain setting adjusted successfully */
    VP_GAIN_GR_OOR,     /* GR Gain setting overflowed (reset to default) */
    VP_GAIN_GX_OOR,     /* GX Gain setting overflowed (reset to default) */
    VP_GAIN_BOTH_OOR,    /* Both GR & GX overflowed (and reset to default) */
    VP_GAIN_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpGainResultType;

typedef struct {
    VpGainResultType gResult; /* Success / Failure status return */
    uint16 gxValue;           /* new GX register abs value */
    uint16 grValue;           /* new GR register abs value */
} VpRelGainResultsType;

/**< The following types are for VpToneDetectionControl() function */
typedef struct {
    uint8 utdToneDetection;     /* Specifies the settings for UTD filters */
    bool modemToneDetection;    /* If set, turns on modem tone detection */
    bool faxToneDetection;      /* If set, turns on fax tone detection */
    VpDirectionType direction;  /* Specifies direction. */
} VpToneDetectionType;

/**< The following types are for VpDeviceIoAccess() function */
typedef enum {
    VP_DEVICE_IO_WRITE,         /* Perform device specific IO write access */
    VP_DEVICE_IO_READ,           /* Perform device specific IO read access */
    VP_DEVICE_IO_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpDeviceIoAccessType;

typedef enum {
    VP_DEVICE_IO_IGNORE,        /* Ignore I/O access */
    VP_DEVICE_IO_ACCESS,        /* Perform I/O access */
    VP_DEVUCE_IOA_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpDeviceIoAccessMask;

typedef struct {
    VpDeviceIoAccessType accessType;/* Device I/O access type */
    uint32 accessMask_31_0;         /* I/O access mask (Pins 0 - 31) */
    uint32 accessMask_63_32;        /* I/O access mask (Pins 32 - 63) */
    uint32 deviceIOData_31_0;   /* Output pin data (Pins 0 - 31) */
    uint32 deviceIOData_63_32;  /* Output pin data (Pins 32 - 63) */
} VpDeviceIoAccessDataType;

/**< The following types are for VpGetLineStatus() function */
typedef enum {
    VP_INPUT_HOOK,      /* Hook Status (ignoring pulse & flash) */
    VP_INPUT_RAW_HOOK,  /* Hook Status (include pulse & flash) */
    VP_INPUT_GKEY,      /* Ground-Key/Fault Status */
    VP_INPUT_THERM_FLT, /* Thermal Fault Status */
    VP_INPUT_CLK_FLT,   /* Clock Fault Status */
    VP_INPUT_AC_FLT,    /* AC Fault Status */
    VP_INPUT_DC_FLT,    /* DC Fault Status */
    VP_INPUT_BAT1_FLT,  /* Battery 1 Fault Status */
    VP_INPUT_BAT2_FLT,  /* Battery 2 Fault Status */
    VP_INPUT_BAT3_FLT,  /* Battery 3 Fault Status */

    VP_INPUT_RINGING,   /* Ringing Status */
    VP_INPUT_LIU,       /* Line In Use Status */
    VP_INPUT_FEED_DIS,  /* Feed Disable Status */
    VP_INPUT_FEED_EN,   /* Feed Enable Status */
    VP_INPUT_DISCONNECT,/* Feed Disconnect Status */
    VP_INPUT_CONNECT,   /* Feed Connect Status */
    VP_INPUT_POLREV,    /* Reverse Polarity if TRUE, Normal if FALSE */
    VP_NUM_INPUT_TYPES,
    VP_INPUT_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpInputType;

/**< The following types are for VpGetLoopCond() function (return via
 * VpGetResults())
 */
typedef enum {
    VP_BATTERY_UNDEFINED,   /* Battery being used is not known or feature 
                             * not supported */
    VP_BATTERY_1,           /* Battery 1 */
    VP_BATTERY_2,           /* Battery 2 */
    VP_BATTERY_3,           /* Battery 3 */
    VP_NUM_BATTERY_TYPES,
    VP_BATTERY_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpBatteryType;

typedef enum {
    VP_DF_UNDEFINED,        /* DC feed region not known or feature not 
                             * supported */
    VP_DF_ANTI_SAT_REG,     /* DC feed is in anti saturation region */
    VP_DF_CNST_CUR_REG,     /* DC feed is in constant current region */
    VP_DF_RES_FEED_REG,     /* DC feed is in resistive feed region */
    VP_NUM_DC_FEED_TYPES,
    VP_DC_FEED_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpDcFeedRegionType;

typedef struct {
    int16 rloop;                    /**< The loop resistance */
    int16 ilg;                      /**< The longitudinal loop current */
    int16 imt;                      /**< The metallic loop current */
    int16 vsab;                     /**< The tip/ring voltage */
    int16 vbat1;                    /**< The Battery 1 voltage */
    int16 vbat2;                    /**< The Battery 2 voltage */
    int16 vbat3;                    /**< The Battery 3 voltage */
    int16 mspl;                     /**< Metering signal peak level */
    VpBatteryType selectedBat; 		/**< Battery that is presently used for 
                                     *   the DC feed */
    VpDcFeedRegionType dcFeedReg; 	/**< DC feed region presently selected */
} VpLoopCondResultsType;

/**< The following types are for VpReadPacketStatistics() function (return via
 * VpGetResults())
 */
typedef struct {
    uint32 totalUsReadPkts;     /**< Total US packets read by the host */
    uint32 totalUsLostPkts;     /**< Total US lost (not read by the host)
                                 *   packets */
    uint32 totalDsExpectedPkts; /**< Total expected downstream packets */
    uint32 totalDsLostPkts;     /**< Total downstream lost packets */
} VpPacketStatisticsType;

/**< The following types are used for the function VpControlVoiceStream() */
typedef enum {
    VP_VOICE_STREAM_START,     /* Start the voice stream */
    VP_VOICE_STREAM_STOP,      /* Stop the voice stream */
    VP_VOICE_STREAM_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE/* Portability Req. */
} VpVSConfControlType;

typedef enum {
    VP_VS_CONF_FULL_3WAY_MODE,     /* Full 3 way conferencing */
    VP_VS_CONF_DISTRIBUTED_MODE,   /* Distributed conferencing */
    VP_VS_CONF_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpVSConfModeType;

/**< The following types are used for the function VpSendSignal() */
typedef enum {
    VP_SENDSIG_MSG_WAIT_PULSE, /**< Apply a Message Waiting Pulse to the Line */
    VP_SENDSIG_DTMF_DIGIT,
    VP_SENDSIG_PULSE_DIGIT,
    VP_SENDSIG_HOOK_FLASH,
    VP_SENDSIG_FWD_DISCONNECT,	/**< Set in 1mS increments */
    VP_SENDSIG_POLREV_PULSE,	/**< Set in 1mS increments */
    VP_SENDSIG_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpSendSignalType;

typedef struct {
    int8 voltage;   /**< Voltage in Volts to apply to the line. A negative
                     * value means Tip is more Negative than Ring, a positive
                     * value means Ring is more Negative than Tip.
                     */
    uint16 onTime;  /**< Duration of pulse on-time in mS */
    uint16 offTime; /**< Duration of pulse off-tim in mS. If the off-time is
                     * set to 0, the voltage is applied to the line continuously
                     */
    uint8 cycles;   /**< Number of pulses to send on the line. If set to 0, will
                     * repeat forever
                     */
} VpSendMsgWaitType;

/**< The enum is used by the system service layer pcmCollect() function */
typedef enum {
    VP_PCM_OPERATION_AVERAGE      = 0x0001, /* average PCM samples */
    VP_PCM_OPERATION_RANGE        = 0x0002, /* find pk to pk pcm sample data. */
    VP_PCM_OPERATION_RMS          = 0x0004, /* find rms value of pcm samples */
    VP_PCM_OPERATION_MIN          = 0x0008, /* find min pcm value of
                                             * pcm samples */
    VP_PCM_OPERATION_MAX          = 0x0010, /* find max pcm value of
                                             * pcm samples */
    VP_PCM_OPERATION_APP_SPECIFIC = 0x0020, /* generic operation */    
    VP_PCM_OPERATION_FREQ         = 0x0040, /* Measure frequency */
    VP_PCM_OPERATION_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability
                                                             * Req. */
} VpPcmOperationBitType;

typedef uint16 VpPcmOperationMaskType;

typedef struct {
    int16 average;  /* Average value of all collected PCM samples */
    int16 range;    /* Maximum - minimum PCM value during operation time */
    int16 rms;      /* RMS result of all collected pcm samples */
    int16 min;      /* Maximum pcm value found */
    int16 max;      /* Minimum pcm value found */
    void *pApplicationInfo; /* Any results that the implementation chooses to
                             * pass. This data is not interpreted neither
                             * by the VP-API nor the LT-API. LT-API passes
                             * this pointer back to the application as part
                             * of the test result
                             */
    int32 freq;     /* Measured average frequenny */

    bool error;     /* indication of PCM operation process (1 = failure) */
} VpPcmOperationResultsType;

/******************************************************************************
 *                 DEVICE/LINE CONTEXT (SUPPORT) DEFINITION                   *
 ******************************************************************************/
/* The following function pointers are required to get around problem of
 * what comes first? chicken or the egg? when dealing with declaration of
 * device context, function pointer table and hence the following declarations.
 */
struct VpDevCtxType;    /**< forward declaration */
struct VpLineCtxType;   /**< forward declaration */
struct VpEventType;     /**< forward declaration */

/*
 * Initialization functions
 */
typedef VpStatusType
(*VpBootLoadFuncPtrType) (
    struct VpDevCtxType *pDevCtx,
    VpBootStateType state,
    VpImagePtrType pImageBuffer,
    uint32 bufferSize,
    VpScratchMemType *pScratchMem,
    VpBootModeType validation);

typedef VpStatusType
(*VpInitDeviceFuncPtrType) (
    struct VpDevCtxType *pDevCtx,
    VpProfilePtrType pDevProfile,
    VpProfilePtrType pAcProfile,
    VpProfilePtrType pDcProfile,
    VpProfilePtrType pRingProfile,
    VpProfilePtrType pFxoAcProfile,
    VpProfilePtrType pFxoCfgProfile);

typedef VpStatusType
(*VpInitLineFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpProfilePtrType pAcProfile,
    VpProfilePtrType pDcFeedOrFxoCfgProfile,
    VpProfilePtrType pRingProfile);

typedef VpStatusType
(*VpConfigLineFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpProfilePtrType pAcProfile,
    VpProfilePtrType pDcFeedOrFxoCfgProfile,
    VpProfilePtrType pRingProfile);

typedef VpStatusType
(*VpCalCodecFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpDeviceCalType mode);

typedef VpStatusType
(*VpCalLineFuncPtrType) (
    struct VpLineCtxType *pLineCtx);

typedef VpStatusType
(*VpInitRingFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpProfilePtrType pCadProfile,
    VpProfilePtrType pCidProfile);

typedef VpStatusType
(*VpInitCidFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    uint8 length,
    uint8p pCidData);

typedef VpStatusType
(*VpInitMeterFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpProfilePtrType pMeterProfile);

typedef VpStatusType
(*VpInitCustomTermTypeFuncPtrType) (
    struct VpDevCtxType *pDevCtx,
    struct VpLineCtxType *pLineCtx,
    VpProfilePtrType pCustomTermProfile);
    
typedef VpStatusType
(*VpInitProfileFuncPtrType) (
    struct VpDevCtxType *pDevCtx,
    VpProfileType type,
    VpProfilePtrType pProfileIndex,
    VpProfilePtrType pProfile);

typedef VpStatusType
(*VpSoftResetFuncPtrType) (
    struct VpDevCtxType *pDevCtx);

/*
 * Control functions
 */
typedef VpStatusType
(*VpSetLineStateFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpLineStateType state);

typedef VpStatusType
(*VpSetLineToneFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpProfilePtrType pToneProfile,
    VpProfilePtrType pCadProfile,
    VpDtmfToneGenType *pDtmfControl);

typedef VpStatusType
(*VpSetRelayStateFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpRelayControlType rState);

typedef VpStatusType
(*VpSetRelGainFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    uint16 txLevel,
    uint16 rxLevel,
    uint16 handle);

typedef VpStatusType
(*VpSendSignalFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpSendSignalType signalType,
    void *pSignalData);

typedef VpStatusType
(*VpSendDigitFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpDigitGenerationType digitType,
    uint8 digit);

typedef VpStatusType
(*VpSendCidFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    uint8 length,
    VpProfilePtrType pCidProfile,
    uint8p pCidData);

typedef VpStatusType
(*VpContinueCidFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    uint8 length,
    uint8p pCidData);

typedef VpStatusType
(*VpStartMeterFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    uint16 onTime,
    uint16 offTime,
    uint16 numMeters);

typedef VpStatusType
(*VpSetOptionFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    struct VpDevCtxType *pDevCtx,
    VpOptionIdType option,
    void *pValue);

typedef VpStatusType
(*VpToneDetectionControlFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpToneDetectionType *pToneDetection);

typedef VpStatusType
(*VpDeviceIoAccessFuncPtrType) (
    struct VpDevCtxType *pDevCtx,
    VpDeviceIoAccessDataType *pDeviceIoData);

typedef VpStatusType
(*VpVirtualISRFuncPtrType) (
    struct VpDevCtxType *pDevCtx);

typedef VpStatusType
(*VpApiTickFuncPtrType) (
    struct VpDevCtxType *pDevCtx,
    bool *pEventStatus);

typedef VpStatusType
(*VpLowLevelCmdFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    uint8 *pCmdData,
    uint8 len,
    uint16 handle);

/*
 * Status and query functions
 */
typedef bool
(*VpGetEventFuncPtrType) (
    struct VpDevCtxType *pDevCtx,
    struct VpEventType *pEvent);

typedef VpStatusType
(*VpGetLineStatusFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpInputType input,
    bool *pStatus);

typedef VpStatusType
(*VpGetDeviceStatusFuncPtrType) (
    struct VpDevCtxType *pDevCtx,
    VpInputType input,
    uint32 *pDeviceStatus);

typedef VpStatusType
(*VpGetLoopCondFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    uint16 handle);

typedef VpStatusType
(*VpGetOptionFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    struct VpDevCtxType *pDevCtx,
    VpOptionIdType option,
    uint16 handle);

typedef VpStatusType
(*VpGetLineStateFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpLineStateType *pCurrentState);

typedef VpStatusType
(*VpFlushEventsFuncPtrType) (
    struct VpDevCtxType *pDevCtx);

typedef VpStatusType
(*VpGetResultsFuncPtrType) (
    struct VpEventType *pEvent,
    void *pResults);

typedef VpStatusType
(*VpClearResultsFuncPtrType) (
    struct VpDevCtxType *pDevCtx);

typedef VpStatusType
(*VpDtmfDigitDetectedFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpDigitType digit,
	VpDigitSenseType sense);

/*
 * Packet Interface functions
 */
typedef VpStatusType
(*VpControlVoiceStreamFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    uint8 streamId,
    VpVSConfControlType streamControl,
    VpVSConfModeType confMode);

typedef VpStatusType
(*VpReadUpStreamPacketFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpPktDataPtrType pPacketData,
    uint8 streamId);

typedef VpStatusType
(*VpWriteDownStreamPacketFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpPktDataPtrType pPacketData,
    uint8 streamId);

typedef VpStatusType
(*VpGetTimeStampFuncPtrType) (
    struct VpDevCtxType *pDevCtx,
    uint16 *pTimeStamp);

typedef VpStatusType
(*VpReadPacketStatisticsFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    uint16 handle);

/*
 * Test Functions
 */
typedef VpStatusType
(*VpTestLineFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpTestIdType test,
    const void *pArgs,
    uint16 handle);

typedef VpStatusType
(*VpTestLineIntFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpTestIdType test,
    const void *pArgs,
    uint16 handle,
    bool callback);

typedef void 
(*VpTestLineCallbackType) (
    struct VpLineCtxType *pLineCtx,
    VpPcmOperationResultsType *pResults);
    
typedef VpStatusType
(*VpCodeCheckSumFuncPtrType) (
    struct VpDevCtxType *pDevCtx,
    uint16 handle);

typedef VpStatusType
(*VpSelfTestFuncPtrType) (
    struct VpLineCtxType *pLineCtx);

typedef VpStatusType
(*VpFillTestBufFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    uint16 length,
    VpVectorPtrType pData);

/** ApiFunctionPointer Table to be used by Device Context */
typedef struct {
    /* Initialization Functions */
    VpBootLoadFuncPtrType BootLoad;
    VpInitDeviceFuncPtrType InitDevice;
    VpInitLineFuncPtrType InitLine;
    VpConfigLineFuncPtrType ConfigLine;
    VpCalCodecFuncPtrType CalCodec;
    VpCalLineFuncPtrType CalLine;
    VpInitRingFuncPtrType InitRing;
    VpInitCidFuncPtrType InitCid;
    VpInitMeterFuncPtrType InitMeter;
    VpInitCustomTermTypeFuncPtrType InitCustomTerm;    
    VpInitProfileFuncPtrType InitProfile;
    VpSoftResetFuncPtrType SoftReset;

    /* Control Functions */
    VpSetLineStateFuncPtrType SetLineState;
    VpSetLineToneFuncPtrType SetLineTone;
    VpSetRelayStateFuncPtrType SetRelayState;
    VpSetRelGainFuncPtrType SetRelGain;
    VpSendSignalFuncPtrType SendSignal;
    VpSendDigitFuncPtrType SendDigit;
    VpSendCidFuncPtrType SendCid;
    VpContinueCidFuncPtrType ContinueCid;
    VpStartMeterFuncPtrType StartMeter;
    VpSetOptionFuncPtrType SetOption;
    VpToneDetectionControlFuncPtrType ToneDetectionControl;
    VpDeviceIoAccessFuncPtrType DeviceIoAccess;
    VpVirtualISRFuncPtrType VirtualISR;
    VpApiTickFuncPtrType ApiTick;
    VpLowLevelCmdFuncPtrType LowLevelCmd;

    /* Status and query Functions */
    VpGetEventFuncPtrType GetEvent;
    VpGetLineStatusFuncPtrType GetLineStatus;
    VpGetDeviceStatusFuncPtrType GetDeviceStatus;
    VpGetLoopCondFuncPtrType GetLoopCond;
    VpGetOptionFuncPtrType GetOption;
    VpGetLineStateFuncPtrType GetLineState;
    VpFlushEventsFuncPtrType FlushEvents;
    VpGetResultsFuncPtrType GetResults;
    VpClearResultsFuncPtrType ClearResults;
    VpDtmfDigitDetectedFuncPtrType DtmfDigitDetected;

    /* Packet Functions */
    VpControlVoiceStreamFuncPtrType ControlVoiceStream;
    VpReadUpStreamPacketFuncPtrType ReadUpStreamPacket;
    VpWriteDownStreamPacketFuncPtrType WriteDownStreamPacket;
    VpGetTimeStampFuncPtrType GetTimeStamp;
    VpReadPacketStatisticsFuncPtrType ReadPacketStatistics;

    /* Test Functions */
    VpTestLineFuncPtrType TestLine;
    VpTestLineIntFuncPtrType TestLineInt;
    VpTestLineCallbackType TestLineCallback;    
    VpCodeCheckSumFuncPtrType CodeCheckSum;
    VpSelfTestFuncPtrType SelfTest;
    VpFillTestBufFuncPtrType FillTestBuf;
} ApiFunctions;

/******************************************************************************
 *                       DEVICE/LINE CONTEXT DEFINITION                       *
 ******************************************************************************/
/** Voice Path Line Context type */
typedef struct VpLineCtxType {
    struct VpDevCtxType *pDevCtx;   /**< Pointer back Device Context */

    void *pLineObj;                 /**< Pointer (forward) to Line Object */
} VpLineCtxType;

/** Voice Path Device Context type */
typedef struct VpDevCtxType {
    VpDeviceType deviceType;    /**< What type is the device context (enum) */
    void *pDevObj;              /**< Pointer to device object */

    ApiFunctions funPtrsToApiFuncs; /**< Pointers to API functions */

    /**< Pointers to each line context associated with this device.  Set size
     * to max allowable per device in system
     */
    VpLineCtxType *pLineCtx[VP_MAX_LINES_PER_DEVICE];
} VpDevCtxType;

/******************************************************************************
 *                        VP-API-II Definitions that use context              *
 ******************************************************************************/
/** Event struct: Type reported by VpGetEvent(). */
typedef struct VpEventType {
    VpStatusType status; 	    /**< Function return status */

    uint8 channelId;            /**< Channel that caused the event */

    VpLineCtxType *pLineCtx;    /**< Pointer to the line context (corresponding
                                 *   to the channel that caused the event) */

    VpDeviceIdType deviceId;    /**< device chip select ID corresponding to the
                                 *   device that caused the event */

    VpDevCtxType *pDevCtx;      /**< Pointer to the device context
                                 *   (corresponding to the device that caused
                                 *   the event) */

    VpEventCategoryType eventCategory; /**< Event category.  The event catagory
                                        *   is necessary because there are more
                                        *   events than can be specified by the
                                        *   size of "eventId  */

    uint16 eventId;     /**< The event that occurred.  Requires that the event
                         *   catagory be known to interpret */

    uint16 parmHandle;  /**< Event�s Parameter or Host Handle.  This value is
                         *   specified by the application only, not used by the
                         *   API */

    uint16 eventData;   /**< Data associated with the event. Event Id specific*/

	bool hasResults; 	/**< If TRUE indicates this event has results associated 
                		 *   with it */

    VpLineIdType lineId;    /**< Application provide line Id to ease mapping of
                             * lines to specific line contexts.
                             */
} VpEventType;

/**< The following types are for VpGetDeviceInfo() and VpGetLlineInfo()
 * functions */
typedef struct {
    VpLineCtxType *pLineCtx;    /* Pointer to Line Context */
    VpDeviceIdType deviceId;    /* Device identity */
    VpDevCtxType *pDevCtx;      /* Pointer to device Context */
    VpDeviceType deviceType;    /* Device Type */
    uint8 numLines;             /* Number of lines */
    uint8 revCode;              /* Revision Code Number */
} VpDeviceInfoType;

typedef struct {
    VpDevCtxType *pDevCtx;       /* Pointer to device Context */
    uint8 channelId;             /* Channel identity */
    VpLineCtxType *pLineCtx;     /* Pointer to Line Context */
    VpTermType termType;         /* Termination Type */
    VpLineIdType lineId;         /* Application system wide line identifier */
} VpLineInfoType;

/******************************************************************************
 *                        VP-API-II FUNCTION PROTOTYPES                       *
 ******************************************************************************/
/*
 * System configuration functions
 */
EXTERN VpStatusType
VpMakeDeviceObject(
    VpDeviceType deviceType,
    VpDeviceIdType deviceId,
    VpDevCtxType *pDevCtx,
    void *pDevObj);

EXTERN VpStatusType
VpMakeDeviceCtx(
    VpDeviceType deviceType,
    VpDevCtxType *pDevCtx,
    void *pDevObj);

EXTERN VpStatusType
VpMakeLineObject(
    VpTermType termType,
    uint8 channelId,
    VpLineCtxType *pLineCtx,
    void *pLineObj,
    VpDevCtxType *pDevCtx);

EXTERN VpStatusType
VpMakeLineCtx(
    VpLineCtxType *pLineCtx,
    void *pLineObj,
    VpDevCtxType *pDevCtx);

EXTERN VpStatusType
VpFreeLineCtx(
    VpLineCtxType *pLineCtx);
    
EXTERN VpStatusType
VpGetDeviceInfo(
    VpDeviceInfoType *pDeviceInfo);

EXTERN VpStatusType
VpGetLineInfo(
    VpLineInfoType *pLineInfo);

/*
 * Initialization functions
 */
EXTERN VpStatusType
VpBootLoad(
    VpDevCtxType *pDevCtx,
    VpBootStateType state,
    VpImagePtrType pImageBuffer,
    uint32 bufferSize,
    VpScratchMemType *pScratchMem,
    VpBootModeType validation);

EXTERN VpStatusType
VpInitDevice(
    VpDevCtxType *pDevCtx,
    VpProfilePtrType pDevProfile,
    VpProfilePtrType pAcProfile,
    VpProfilePtrType pDcProfile,
    VpProfilePtrType pRingProfile,
    VpProfilePtrType pFxoAcProfile,
    VpProfilePtrType pFxoCfgProfile);

EXTERN VpStatusType
VpInitLine(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pAcProfile,
    VpProfilePtrType pDcFeedOrFxoCfgProfile,
    VpProfilePtrType pRingProfile);

EXTERN VpStatusType
VpConfigLine(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pAcProfile,
    VpProfilePtrType pDcFeedOrFxoCfgProfile,
    VpProfilePtrType pRingProfile);

EXTERN VpStatusType
VpCalCodec(
    VpLineCtxType *pLineCtx,
    VpDeviceCalType mode);

EXTERN VpStatusType
VpCalLine(
    VpLineCtxType *pLineCtx);

EXTERN VpStatusType
VpInitRing(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pCadProfile,
    VpProfilePtrType pCidProfile);

EXTERN VpStatusType
VpInitCid(
    VpLineCtxType *pLineCtx,
    uint8 length,
    uint8p pCidData);

EXTERN VpStatusType
VpInitMeter(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pMeterProfile);

EXTERN VpStatusType
VpInitCustomTermType (
    VpDevCtxType *pDevCtx,
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pCustomTermProfile);

EXTERN VpStatusType
VpInitProfile(
    VpDevCtxType *pDevCtx,
    VpProfileType type,
    VpProfilePtrType pProfileIndex,
    VpProfilePtrType pProfile);

EXTERN VpStatusType
VpSoftReset(
    VpDevCtxType *pDevCtx);


/*
 * Control functions
 */
EXTERN VpStatusType
VpSetLineState(
    VpLineCtxType *pLineCtx,
    VpLineStateType state);

EXTERN VpStatusType
VpSetLineTone(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pToneProfile,
    VpProfilePtrType pCadProfile,
    VpDtmfToneGenType *pDtmfControl);

EXTERN VpStatusType
VpSetRelayState(
    VpLineCtxType *pLineCtx,
    VpRelayControlType rState);

EXTERN VpStatusType
VpSetRelGain(
    VpLineCtxType *pLineCtx,
    uint16 txLevel,
    uint16 rxLevel,
    uint16 handle);

EXTERN VpStatusType
VpSendSignal(
    VpLineCtxType *pLineCtx,
    VpSendSignalType signalType,
    void *pSignalData);

EXTERN VpStatusType
VpSendDigit(
    VpLineCtxType *pLineCtx,
    VpDigitGenerationType digitType,
    uint8 digit);

EXTERN VpStatusType
VpSendCid(
    VpLineCtxType *pLineCtx,
    uint8 length,
    VpProfilePtrType pCidProfile,
    uint8p pCidData);

EXTERN VpStatusType
VpContinueCid(
    VpLineCtxType *pLineCtx,
    uint8 length,
    uint8p pCidData);

EXTERN VpStatusType
VpStartMeter(
    VpLineCtxType *pLineCtx,
    uint16 onTime,
    uint16 offTime,
    uint16 numMeters);

EXTERN VpStatusType
VpSetOption(
    VpLineCtxType *pLineCtx,
    VpDevCtxType *pDevCtx,
    VpOptionIdType option,
    void *pValue);

EXTERN VpStatusType
VpToneDetectionControl(
    VpLineCtxType *pLineCtx,
    VpToneDetectionType *pToneDetection);

EXTERN VpStatusType
VpDeviceIoAccess(
    VpDevCtxType *pDevCtx,
    VpDeviceIoAccessDataType *pDeviceIoData);

EXTERN VpStatusType
VpVirtualISR(
    VpDevCtxType *pDevCtx);

EXTERN VpStatusType
VpApiTick(
    VpDevCtxType *pDevCtx,
    bool *pEventStatus);

EXTERN VpStatusType
VpLowLevelCmd(
    VpLineCtxType *pLineCtx,
    uint8 *pCmdData,
    uint8 len,
    uint16 handle);

/*
 * Status and query functions
 */
EXTERN bool
VpGetEvent(
    VpDevCtxType *pDevCtx,
    VpEventType *pEvent);

EXTERN VpStatusType
VpGetLineStatus(
    VpLineCtxType *pLineCtx,
    VpInputType input,
    bool *pStatus);

EXTERN VpStatusType
VpGetDeviceStatus(
    VpDevCtxType *pDevCtx,
    VpInputType input,
    uint32 *pDeviceStatus);

EXTERN VpStatusType
VpGetLoopCond(
    VpLineCtxType *pLineCtx,
    uint16 handle);

EXTERN VpStatusType
VpGetOption(
    VpLineCtxType *pLineCtx,
    VpDevCtxType *pDevCtx,
    VpOptionIdType option,
    uint16 handle);

EXTERN VpStatusType
VpGetLineState(
    VpLineCtxType *pLineCtx,
    VpLineStateType *pCurrentState);

EXTERN VpStatusType
VpFlushEvents(
    VpDevCtxType *pDevCtx);

EXTERN VpStatusType
VpGetResults(
    VpEventType *pEvent,
    void *pResults);

EXTERN VpStatusType
VpClearResults(
    VpDevCtxType *pDevCtx);

EXTERN VpStatusType
VpDtmfDigitDetected(
    VpLineCtxType *pLineCtx,
    VpDigitType digit,
    VpDigitSenseType sense);

/*
 * Packet Interface functions
 */
EXTERN VpStatusType
VpControlVoiceStream(
    VpLineCtxType *pLineCtx,
    uint8 streamId,
    VpVSConfControlType streamControl,
    VpVSConfModeType confMode);

EXTERN VpStatusType
VpReadUpStreamPacket(
    VpLineCtxType *pLineCtx,
    VpPktDataPtrType pPacketData,
    uint8 streamId);

EXTERN VpStatusType
VpWriteDownStreamPacket(
    VpLineCtxType *pLineCtx,
    VpPktDataPtrType pPacketData,
    uint8 streamId);

EXTERN VpStatusType
VpGetTimeStamp(
    VpDevCtxType *pDevCtx,
    uint16 *pTimeStamp);

EXTERN VpStatusType
VpReadPacketStatistics(
    VpLineCtxType *pLineCtx,
    uint16 handle);

/*
 * Test Functions
 */
EXTERN VpStatusType
VpTestLine(
    VpLineCtxType *pLineCtx,
    VpTestIdType test,
    const void *pArgs,
    uint16 handle);

EXTERN void
VpTestLineCallback(
    VpLineCtxType *pLineCtx,
    VpPcmOperationResultsType *pResults);
    
EXTERN VpStatusType
VpCodeCheckSum(
    VpDevCtxType *pDevCtx,
    uint16 handle);

EXTERN VpStatusType
VpSelfTest(
    VpLineCtxType *pLineCtx);

EXTERN VpStatusType
VpFillTestBuf(
    VpLineCtxType *pLineCtx,
    uint16 length,
    VpVectorPtrType pData);

EXTERN VpStatusType
VpMapLineId(
    VpLineCtxType *pLineCtx,
	VpLineIdType lineId);

#endif /* VP_API_COMMON_H */




