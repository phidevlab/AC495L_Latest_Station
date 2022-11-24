/** @file
 *  @brief This file contains the definitions of all structures and enumerations for drivers. 
 *  This file contains the following 6 subsections:
 *  @n Constant Definitions
 *  @n Device Memory Access Definitions 
 *  @n Packet Structures  (Bit Field regardless) 
 *  @n Enumerated Type Definitions      
 *  @n DSP Download Structures        
 *  @n Packet Attribute Structures   
 */




/*************************************************************************************/
/* AC49xDef.h - AC49x Device Driver / Strctures, Enums and Constants Definitions	 */
/* Copyright (C) 2000 AudioCodes Ltd.                                                */
/* 1/7/01 - Coding started.                                                          */
/*                                                                                   */
/* Modifications :                                                                   */
/*************************************************************************************/
/*                                                                                   */
/* The following abbreviations are used in this file :                               */
/* ---------------------------------------------------                               */
/* Attr   : Attributes                                                               */
/* Addr   : Address                                                                  */
/* Max    : Maximum                                                                  */
/* Min    : Minimum                                                                  */
/* VAD    : Voice Activity Detector                                                  */
/* IBS    : In Band Signaling                                                        */
/* CAS    : Channel Associated Signaling                                             */
/* SID    : Silence Information Descriptor                                           */
/* BFI    : Bad Frame Interpolation                                                  */
/* RTP    : Real-time Transport Protocol                                             */
/* Tx     : Transmit                                                                 */
/* Rx     : Receive                                                                  */
/*************************************************************************************/

#ifndef AC49XDEF_H
#define AC49XDEF_H

           /* All the rest of this file is compiled only for the following devices : */
#if (  (AC49X_DEVICE_TYPE == AC490_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC491_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC491L_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC491_491L_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC494_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC495_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC495L_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC496_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC496E_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC497_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC494E_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC495E_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC496D_DEVICE)\
    )

/*******************************************/
/*******************************************/
/**         Constant Definitions          **/
/*******************************************/
/*******************************************/
            /* AC49X_NUM_OF_CHANNELS defines the number of available channels per device (core). */
            /* AC49X_NUMBER_OF_MEMORY_MAPED_CHANNELS defines the max number of channels for the DSP memory map. */
#if AC49X_DEVICE_TYPE == AC490_DEVICE
    #define AC49X_NUMBER_OF_CHANNELS	            24
#elif AC49X_DEVICE_TYPE == AC491_DEVICE
	#define AC49X_NUMBER_OF_CHANNELS	            32
#elif AC49X_DEVICE_TYPE == AC491L_DEVICE
	#define AC49X_NUMBER_OF_CHANNELS	            18
#elif AC49X_DEVICE_TYPE == AC491_491L_DEVICE
	#define AC49X_NUMBER_OF_491_CHANNELS	        32
	#define AC49X_NUMBER_OF_491L_CHANNELS	        16
	#define AC49X_NUMBER_OF_CHANNELS				32
#elif AC49X_DEVICE_TYPE == AC494_DEVICE
	#define AC49X_NUMBER_OF_CHANNELS	            8
#elif AC49X_DEVICE_TYPE == AC495_DEVICE
	#define AC49X_NUMBER_OF_CHANNELS	            8
#elif AC49X_DEVICE_TYPE == AC495L_DEVICE
	#define AC49X_NUMBER_OF_CHANNELS	            8
#elif AC49X_DEVICE_TYPE == AC496_DEVICE
	#define AC49X_NUMBER_OF_CHANNELS	            8
#elif AC49X_DEVICE_TYPE == AC497_DEVICE
	#define AC49X_NUMBER_OF_CHANNELS	            8
#elif AC49X_DEVICE_TYPE == AC494E_DEVICE
	#define AC49X_NUMBER_OF_CHANNELS	            8
#elif AC49X_DEVICE_TYPE == AC495E_DEVICE
	#define AC49X_NUMBER_OF_CHANNELS	            8
#elif AC49X_DEVICE_TYPE == AC496D_DEVICE
	#define AC49X_NUMBER_OF_CHANNELS	            8
#endif

#define DTMF_CALLER_ID__MINIMUM_MESSAGE_LENGTH      3

#define MAX_NUMBER_OF_ACTIVE_TIME_SLOTS				256
#define AC49X_NUMBER_OF_MEDIA_CHANNEL_TYPES			4

#define	BUFFER_DESCRIPTOR_FULL_ERROR	0x1011
#define BUFFER_DESCRIPTOR_EMPTY_ERROR	0x1012
#define AC49X_RX_PACKET_NOT_READY		0x1013
#define AC49X_RX_PACKET_READY		    0x0000

#define DEVICE_WAIT_TIMEOUT_ERROR		0x1019
#define RX_SEQUENCE_ERROR				0x101B
#define TX_SEQUENCE_ERROR				0x101C
#define RX_CHECKSUM_ERROR				0x101D
#define BAD_PACKET_HEADER				0x101E
#define CHANNEL_ID_ERROR		        0x101F
#define RX_PACKET_SIZE_ERROR			0x1020
#define RX_PACKET_PROTOCOL_ERROR		0x1021
#define RX_PACKET_OPCODE_ERROR			0x1022
#define RX_PACKET_CATEGORY_ERROR		0x1023
#define RX_BD_INDEX_ERROR				0x1024
#define TX_BD_INDEX_ERROR				0x1025
#define RX_BAD_PACKER_ADDRESS_ERROR		0x1026
#define TX_BAD_PACKER_ADDRESS_ERROR		0x1027
#define RESET_DEVICE_FAILED				0x1028
#define UNSUPPORTED_MEDIA_TRANSFER_PORT	0x1029

#define NEXT_ERROR						0x102A

#define COMMAND_ECHO_ERROR_FLAG 		0x8000
#define COMMAND_ECHO_NOT_VALID_ERROR 	0xFFFF0000
#define COMMAND_ECHO_CHECKSUM_ERROR 	0xFFFE0000

#define MAX_PAYLOAD_SIZE                1600 
#define MAX_PACKET_SIZE                 6100 
#define MAX_DELAY_ITERATIONS			2000 /* (was: MAX_DELAY_TIME/DELAY_FOR_DEVICE_RESPONSE)  The calculated number of iterations for delay loops : */ 

#define AC49X_MAX_RTP_CANONICAL_NAME_STRING_LENGTH 64  /*255*/
#define MAX_HDLC_MESSAGE_LENGTH         300
#define MAX_DUA_MESSAGE_LENGTH          64
#define MAX_SS7_DATA_SIZE				278
#define MAX_CALLER_ID_MESSAGE_LENGTH	256
#define MAX_ERROR_MESSAGE_LENGTH        1500
#define MAX_EVENT_MESSAGE_LENGTH        1500
#define MAX_IBS_STRING_SIGNALS          30  
#define MAX_ACOUSTIC_ECHO_CANCELER_EQUALIZER_COEFFICIENTS 128
#define MAX_DEVICE_LOW_PASS_FILTER_COEFFICIENTS 256

#define MAX_NUMBER_OF_EXTENDED_IBS_DIGITS__FORMAT_0 6
#define MAX_NUMBER_OF_EXTENDED_IBS_DIGITS__FORMAT_1 40
#define MAX_EXTENDED_IBS_STRING_SIGNALS MAX_NUMBER_OF_EXTENDED_IBS_DIGITS__FORMAT_0
  
#define MAX_NUMBER_OF_ABCD_PULSE_DIGITS 32        
#define DUMMY_INITIALIZATION_TIME_STEMP 0xFFFFFFFF

#define MAX_SIGNAL_QUANTITY											16 
#define CALL_PROGRESS_AND_USER_DEFINED_TONES_SIGNAL_QUANTITY		32 
#define MAX_NUMBER_OF_3GPP_RFCI										17
#define MAX_NUMBER_OF_3GPP_IUB_TFIS									8
#define MAX_CAS_STRING_LENGTH										10

#define MAX_KEY_SIZE__RTP_CIPHER                                    16
#define MAX_KEY_SIZE__RTCP_CIPHER                                   16
#define MAX_KEY_SIZE__RTP_INITIALIZATION                            16
#define MAX_KEY_SIZE__RTP_MESSAGE_AUTHENTICATION_CODE               174
#define MAX_KEY_SIZE__RTCP_MESSAGE_AUTHENTICATION_CODE              20

#define MAX_KEY_SIZE__MASTER										16
#define MAX_KEY_SIZE__MASTER_SALT									14
#define MAX_KEY_SIZE__MASTER__FORMAT_0								MAX_KEY_SIZE__MASTER
#define MAX_KEY_SIZE__MASTER__FORMAT_1								32
#define AC49X_RTP_MAX_LEAVING_REASON_MESSAGE_SIZE					64

#define RTCP_APP_NAME_LENGTH                                        4
#define RTCP_APP_DATA_LENGTH                                        64

#define MAX_MASTER_KEY_INDEX_SIZE									4
#define SCALE_SRTP_EXTENDED_SEQUENCE_NUMBER_SIZE					6
#define MAX_NUMBER_OF_CONSTELLATION_POINTS							8
#define MAX_NUMBER_OF_TANDEM_FREE_OPERATION_OPTIONAL_AMR_CODECS		5


#define MAX_NUMBER_OF_RFC_3267_CODECS                               8


#define MAX_NUMBER_OF_4_WAY_CONFERENCES_PER_DEVICE					4
#define MAX_NUMBER_OF_4_WAY_CONFERENCE_PARTICIPANTS_PER_CONFERENCE	4
#define MAX_NUMBER_OF_4_WAY_CONFERENCE_PARTICIPANTS_PER_DEVICE		12

#define MAX_NUMBER_OF_9_WAY_CONFERENCES_PER_DEVICE					2
#define MAX_NUMBER_OF_9_WAY_CONFERENCE_PARTICIPANTS_PER_CONFERENCE	9

#define AC49X_V34_PARAMETER_PAYLOAD_SIZE							16

#if (AC49X_DEVICE_TYPE == AC490_DEVICE)
#define EMIF_TEST__ERROR_INFORMATION_ADDRESS						(0x2F22*2)
#define EMIF_TEST__ERROR_INFORMATION_SIZE							120
#endif /*(AC49X_DEVICE_TYPE == AC490_DEVICE)*/

/***************************************************/
/********************   HCRF   *********************/
/***************************************************/
#define HCRF__CPU_REGISTERS_BUFFER_SIZE			192
#define HCRF__TRACE_BUFFER_SIZE					168
#if   (  (AC49X_DEVICE_TYPE == AC491_DEVICE)\
	  || (AC49X_DEVICE_TYPE == AC491L_DEVICE)\
	  || (AC49X_DEVICE_TYPE == AC491_491L_DEVICE)\
	  )

#define HCRF__LOCAL_MEMORY_SIZE					(0x60000-0xC0+1)
#define HCRF_COMMAND_REGISTER_ADDRESS__CORE_A	0x10000000
#define HCRF_COMMAND_REGISTER_ADDRESS__CORE_B	0x10000004
#define HCRF_COMMAND_REGISTER_ADDRESS__CORE_C	0x10000008
#define HCRF_COMMAND_REGISTER_ADDRESS__CORE_D	0x1000000C
#define HCRF_COMMAND_REGISTER_ADDRESS__CORE_E	0x10000010
#define HCRF_COMMAND_REGISTER_ADDRESS__CORE_F	0x10000014

#define HCRF_STATUS_REGISTER_ADDRESS__CORE_A	0x10000020
#define HCRF_STATUS_REGISTER_ADDRESS__CORE_B	0x10000024
#define HCRF_STATUS_REGISTER_ADDRESS__CORE_C	0x10000028
#define HCRF_STATUS_REGISTER_ADDRESS__CORE_D	0x1000002C
#define HCRF_STATUS_REGISTER_ADDRESS__CORE_E	0x10000030
#define HCRF_STATUS_REGISTER_ADDRESS__CORE_F	0x10000034
#define HCRF__MONITOR_INFO_BUFFER_ADDRESS		0x4FCA4

#if   (AC49X_DEVICE_TYPE == AC491L_DEVICE)
#define HCRF_STATUS_REGISTER_ADDRESS__DEVICE_ID 0x10000048
#define AC491L_DEVICE_ID_REGISTER_VALUE			0x2055
#endif

#elif (AC49X_DEVICE_TYPE == AC490_DEVICE)

#define HCRF__LOCAL_MEMORY_SIZE					(0x50000-0xC0+1)
#define HCRF__MONITOR_INFO_BUFFER_SIZE			192
#define HCRF__INT_ENTRY_SIZE					8					
#define HCRF__INT_ENTRY_ADDRESS					0x10050					
#define HCRF__COMMAND_REGISTER_ADDRESS			(0x2F52*2)   //(0x2F1E*2)					
#define HCRF__STATUS_REGISTER_ADDRESS			(0x2F54*2)					
#define HCRF__MONITOR_INFO_BUFFER_ADDRESS		(0x2F56*2)

#elif   (  (AC49X_DEVICE_TYPE == AC494_DEVICE)\
        || (AC49X_DEVICE_TYPE == AC495_DEVICE)\
		|| (AC49X_DEVICE_TYPE == AC495L_DEVICE)\
        || (AC49X_DEVICE_TYPE == AC496_DEVICE)\
		|| (AC49X_DEVICE_TYPE == AC496E_DEVICE)\
        || (AC49X_DEVICE_TYPE == AC497_DEVICE)\
		|| (AC49X_DEVICE_TYPE == AC494E_DEVICE)\
		|| (AC49X_DEVICE_TYPE == AC495E_DEVICE)\
		|| (AC49X_DEVICE_TYPE == AC496D_DEVICE)\
        )

#define HCRF__LOCAL_MEMORY_SIZE					(0x0401FFFF-0x04000200+1)
#define HCRF__MONITOR_INFO_BUFFER_SIZE			192
#define HCRF__INT_ENTRY_SIZE					8					
#define HCRF__INT_ENTRY_ADDRESS					0x3F10				
#define HCRF__COMMAND_REGISTER_ADDRESS			0x709C
#define HCRF__STATUS_REGISTER_ADDRESS	 		0x70A0
#define HCRF__MONITOR_INFO_BUFFER_ADDRESS		0x70A4

#endif
/***************************************************/
/********************   HCRF   *********************/
/***************************************************/

#ifndef _BASE_TYPES_DEFS
#define _BASE_TYPES_DEFS

typedef unsigned char   U8;
typedef unsigned short  U16;
typedef unsigned long   U32;

#endif /* #ifndef _BASE_TYPES_DEFS */

#ifndef MAX_UX
#define MAX_UX
#define MAX_U8          0xFF
#define MAX_U16         0xFFFF
#define MAX_U24         0xFFFFFF
#define MAX_U32         0xFFFFFFFF
#endif
    /* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_G711Alaw_64      8   */
    /* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_G711Mulaw        0   */
    /* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_G726_16          35  */
    /* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_G726_24          36  */
    /* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_G726_32          2   */
    /* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_G726_40          38  */
    /* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_G727_16          39  */
    /* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_G727_24_16       40  */
    /* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_G727_24          41  */
    /* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_G727_32_16       42  */
    /* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_G727_32_24       43  */
    /* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_G727_32=11       44  */
    /* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_G727_40_16       45  */
    /* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_G727_40_24       46  */
    /* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_G727_40_32       47  */
    /* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_G723Low          4   */
    /* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_G723High         4   */
    /* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_G729             18  */
    /*              =18             NA  */
	/* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_GSM              3   */
	/*              =20             NA  */
	/*              =21             NA  */
	/* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_Transparent      56  */
    /* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_G728             15  */
	/*              =24             NA  */
	/* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_NetCoder_4_8     49  */
	/* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_NetCoder_5_6     50  */
	/* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_NetCoder_6_4     51  */
	/* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_NetCoder_7_2     52  */
	/* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_NetCoder_8       53  */
	/* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_NetCoder_8_8     54  */
	/* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_NetCoder_9_6     55  */
	/* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_EVRC             60  */
    /* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_EVRC_TFO         60  */
    /* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_QCELP_8          61  */
    /* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_QCELP_8_TFO      61  */
	/* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_QCELP_13         62  */
    /* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_QCELP_13_TFO     62  */
    /*              =38            NA  */
    /*              =39            NA  */
	/* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_NoCoder          NA  */
    /*              =41            NA  */
    /*              =42            NA  */
    /*              =43            NA  */
	/*              =44            NA  */
    /* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_G711Alaw_5_5  (WRONG TIME PARAMETER WONT WORK IN IP!!!!)  NA  */
    /* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_G711Mulaw_5_5 (WRONG TIME PARAMETER WONT WORK IN IP!!!!)  NA  */
    /* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_G726_32_5_5   (WRONG TIME PARAMETER WONT WORK IN IP!!!!)  NA  */
	/*              =48            NA  */
    /*              =49            NA  */
    /* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_AMR_4_75         105 */
    /* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_AMR_5_15         106 */
	/* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_AMR_5_9          107 */
    /* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_AMR_6_7          108 */
    /* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_AMR_7_4          109 */
    /* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_AMR_7_95         110 */
	/* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_AMR_10_2         111 */
    /* #define DEFAULT_RTP_PAYLOAD_TYPE_FOR_CODER_AMR_12_2         112 */
    /*              =58            NA  */
    /*              =59            NA  */
	/*              =60            NA  */
    /*              =61            NA  */
    /*              =62            NA  */
    /*              =63            NA  */  

/*****************************************\
*******************************************
**   Bit Fields  &  Macro Definitions    **
*******************************************
\*****************************************/
#if AC49X_ENDIAN_MODE == LITTLE_ENDIAN
    #include "AC49xDrv_LittleEndianBitFields.h"
#else
    #include "AC49xDrv_BigEndianBitFields.h"
#endif /*~~AC49X_ENDIAN_MODE*/ 

            /* The following macros enable merging a field that has been
             * split into a Ls and a Ms byte/word part in a structure, or splitting
             * a value into its Ls and Ms fields. The ## is a token-pasting
             * operation that pastes the Ls/Ms byte/word extension to the field name.
             * The follwoing assumptions are made :
             * - The two parts end with the extensions "_Lsb", "_Msb","_LswLsb", "_LswMsb","_MswLsb", and "Msw_Msb".
             * - All part is 8 bits long */

#ifndef MERGE_AND_SPLIT_MACROS
#define MERGE_AND_SPLIT_MACROS

#define MergeFieldsToShort(SourceBitFieldPrefix)		        (\
																  (unsigned short)(((SourceBitFieldPrefix##_Msb)<<8)&0xFF00)\
																+ (unsigned short)(((SourceBitFieldPrefix##_Lsb)   )&0x00FF)\
																)


#define SplitFieldsFromShort(TargetBitFieldPrefix, SourceShort)	{\
																(TargetBitFieldPrefix##_Msb) = (U8)((SourceShort>>8)&0x00FF);\
																(TargetBitFieldPrefix##_Lsb) = (U8)((SourceShort   )&0x00FF);\
																}

																  
#define MergeFieldsToLong(SourceBitFieldPrefix)		            (\
																  (unsigned long )(((SourceBitFieldPrefix##_MswMsb)<<24)&0xFF000000)\
																+ (unsigned long )(((SourceBitFieldPrefix##_MswLsb)<<16)&0x00FF0000)\
																+ (unsigned long )(((SourceBitFieldPrefix##_LswMsb)<<8 )&0x0000FF00)\
																+ (unsigned long )(((SourceBitFieldPrefix##_LswLsb)    )&0x000000FF)\
																)
#define SplitFieldsFromLong(TargetBitFieldPrefix, SourceLong)	{\
																(TargetBitFieldPrefix##_MswMsb) = (U8)((SourceLong>>24)&0x000000FF);\
																(TargetBitFieldPrefix##_MswLsb) = (U8)((SourceLong>>16)&0x000000FF);\
																(TargetBitFieldPrefix##_LswMsb) = (U8)((SourceLong>>8 )&0x000000FF);\
																(TargetBitFieldPrefix##_LswLsb) = (U8)((SourceLong    )&0x000000FF);\
																}
#endif





/********************************************/
/********************************************/
/**     Device Memory MAP Definitions      **/
/********************************************/
/********************************************/
#if (  (AC49X_DEVICE_TYPE == AC494_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC495_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC495L_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC496_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC496E_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC497_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC494E_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC495E_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC496D_DEVICE)\
    )
typedef struct
    {
    unsigned long PeripheralResetRegister;      /* PRCR     */
    unsigned long SoftwareResetRegister;        /* SWCR     */
    unsigned long PinControlRegister;           /* PINCR    */
    unsigned long ResetStatusRegister;          /* RSR      */
    } Tac498ResetControlRegisters;

typedef struct
    {
    unsigned long PowerDownControlRegister;     /* PWDNCR   */
    unsigned long ClockControlRegister;         /* SCLKPCR  */
    unsigned long Reserved;                     /* PINCR    */
    unsigned long ClockDivRegister;             /* RSR      */
    } Tac498ClockControlRegisters;

typedef struct
    {
    unsigned char  DSPMemory[0x60000];
    unsigned char  Reserved_1[0x1FA0000];
    unsigned char  McBSP0[0x100];
    unsigned char  Reserved_2[0x1FF00];
    unsigned char  McBSP1[0x100];
    unsigned char  Reserved_3[0x1FF00];
    unsigned char  PDMAControl[0x200];
    unsigned char  Reserved_4[0x3FE00];
    Tac498ResetControlRegisters  ResetControlRegisters;
    unsigned char  Reserved_5[0x100-sizeof(Tac498ResetControlRegisters)];
    Tac498ClockControlRegisters ClockControlRegisters;
    unsigned char  Reserved_6[0x100-sizeof(Tac498ClockControlRegisters)];
    unsigned char  Reserved_7[0xE00];
    unsigned char  CodecIFControl[0x100];
    unsigned char  McDMA[0x100];
    unsigned char  VLYNQ0Control[0x100];
    unsigned char  VLYNQ1Control[0x100];
    unsigned char  VDMAControl[0x100];
    unsigned char  Reserved_8[0x1F7EB00];
    unsigned char  VLYNQ0Portal[0x4000000];
    unsigned char  VLYNQ1Portal[0x4000000];
    } Tac498MemoryMap;

#ifndef offsetof
#define offsetof(s,m)   (size_t)&(((s *)0)->m)
#endif

#define AC498_PRCR_DSP_MCDMA_BIT    0x00000200
#define AC498_PRCR_DSP_MIF_BIT      0x00000100
#define AC498_PRCR_DSP_DSP_BIT      0x00000080
#define AC498_PRCR_DSP_PDMA_BIT     0x00000020
#define AC498_PRCR_DSP_MBSP0_BIT    0x00000001

#define AC498_VLYNQ_ADDRESS_SPACE_LOW    offsetof(Tac498MemoryMap, VLYNQ0Portal[0])    /* 0x7FFFFFF left 5 pins VLYNQ  */
#define AC498_VLYNQ_ADDRESS_SPACE_HI     offsetof(Tac498MemoryMap, VLYNQ1Portal[0])    /* 0xBFFFFFF right 3 pins VLYNQ */
#define AC498_HPI_BASE_ADDRESS           0x0000000 /*TBD                    */
#define AC498_VLYNQ_BLOCK_SIZE          (0x4000000/4)

#endif /*(AC49X_DEVICE_TYPE == AC494_DEVICE)...*/


/********************************************/
/********************************************/
/**  Device HPI Memory Access Definitions  **/
/********************************************/
/********************************************/
            /* The following constants relate to the DSP memory map. All values are in bytes. */
            /* Note that in the DSP, memory addresses refer to 16-bit words                   */

#if (AC49X_DEVICE_TYPE == AC490_DEVICE)
    #define _490_PROGRAM_DOWNLOAD_BLOCK_ADDRESS         (2*0x200									)

    #define HOST_TX_BUFFER_SIZE	                        (2*0x300									)
    #define HOST_RX_BUFFER_SIZE	                        (2*0x300									)
    #define HOST_TX_BUFFER_DESCRIPTORS_BUFFER_SIZE	    (2*0x60       								)
    #define HOST_RX_BUFFER_DESCRIPTORS_BUFFER_SIZE	    (2*0x60       								)
    #define NETWORK_TX_BUFFER_SIZE	                    (2*0xB40									)
    #define NETWORK_RX_BUFFER_SIZE	                    (2*0xB40									)
    #define NETWORK_TX_BUFFER_DESCRIPTORS_BUFFER_SIZE	(2*0x106      								)
    #define NETWORK_RX_BUFFER_DESCRIPTORS_BUFFER_SIZE	(2*0x106      								)

    #define	HPI_MEMORY_MAP_ADDRESS_OFFSET	            (2*0x1000									)
    #define AC49XIF_BASE_PAGE				            ((HPI_MEMORY_MAP_ADDRESS_OFFSET/2)>>16		)

    #define HPI_MEMORY__HOST_TX_BUFFER                  (HPI_MEMORY_MAP_ADDRESS_OFFSET)
    #define HPI_MEMORY__HOST_RX_BUFFER                  (HPI_MEMORY__HOST_TX_BUFFER                 + HOST_TX_BUFFER_SIZE)

    #define HPI_MEMORY__HOST_TX_BUFFER_DESCRIPTOR       (HPI_MEMORY__HOST_RX_BUFFER                 + HOST_RX_BUFFER_SIZE)
    #define HPI_MEMORY__HOST_RX_BUFFER_DESCRIPTOR       (HPI_MEMORY__HOST_TX_BUFFER_DESCRIPTOR      + HOST_TX_BUFFER_DESCRIPTORS_BUFFER_SIZE)

    #define HPI_MEMORY__NETWORK_TX_BUFFER               (HPI_MEMORY__HOST_RX_BUFFER_DESCRIPTOR      + HOST_RX_BUFFER_DESCRIPTORS_BUFFER_SIZE)
    #define HPI_MEMORY__NETWORK_RX_BUFFER               (HPI_MEMORY__NETWORK_TX_BUFFER              + NETWORK_TX_BUFFER_SIZE)

    #define HPI_MEMORY__NETWORK_TX_BUFFER_DESCRIPTOR    (HPI_MEMORY__NETWORK_RX_BUFFER              + NETWORK_RX_BUFFER_SIZE)
    #define HPI_MEMORY__NETWORK_RX_BUFFER_DESCRIPTOR    (HPI_MEMORY__NETWORK_TX_BUFFER_DESCRIPTOR   + NETWORK_TX_BUFFER_DESCRIPTORS_BUFFER_SIZE)

    #define HPI_MEMORY__DEVICE_CONTROL_REG              (HPI_MEMORY__NETWORK_RX_BUFFER_DESCRIPTOR   + NETWORK_RX_BUFFER_DESCRIPTORS_BUFFER_SIZE)
    #define HPI_MEMORY__DEVICE_STATUS_REG               (HPI_MEMORY__DEVICE_CONTROL_REG             + 4)
    #define HPI_MEMORY__BOOT_STATUS_REG                 (HPI_MEMORY__DEVICE_STATUS_REG              + 4)

#elif (  (AC49X_DEVICE_TYPE == AC491_DEVICE)\
	  || (AC49X_DEVICE_TYPE == AC491L_DEVICE)\
	  || (AC49X_DEVICE_TYPE == AC491_491L_DEVICE)\
	  )															
    #define	HPI_MEMORY_MAP_ADDRESS_OFFSET	            (2*0x25C80									)
    #define AC49XIF_BASE_PAGE				            ((HPI_MEMORY_MAP_ADDRESS_OFFSET/2)>>16		)
    #define HPI_MEMORY__HOST_TX_BUFFER                  (0       + HPI_MEMORY_MAP_ADDRESS_OFFSET   	)
    #define HPI_MEMORY__HOST_RX_BUFFER                  (1792    + HPI_MEMORY_MAP_ADDRESS_OFFSET	)
    #define HPI_MEMORY__HOST_TX_BUFFER_DESCRIPTOR       (3584    + HPI_MEMORY_MAP_ADDRESS_OFFSET	)
    #define HPI_MEMORY__HOST_RX_BUFFER_DESCRIPTOR       (3744    + HPI_MEMORY_MAP_ADDRESS_OFFSET	)
    #define HPI_MEMORY__NETWORK_TX_BUFFER               (3904    + HPI_MEMORY_MAP_ADDRESS_OFFSET	)
    #define HPI_MEMORY__NETWORK_RX_BUFFER               (10048   + HPI_MEMORY_MAP_ADDRESS_OFFSET	)
    #define HPI_MEMORY__NETWORK_TX_BUFFER_DESCRIPTOR    (16192   + HPI_MEMORY_MAP_ADDRESS_OFFSET	)
    #define HPI_MEMORY__NETWORK_RX_BUFFER_DESCRIPTOR    (16748   + HPI_MEMORY_MAP_ADDRESS_OFFSET	)
    #define HPI_MEMORY__DEVICE_CONTROL_REG              (17304   + HPI_MEMORY_MAP_ADDRESS_OFFSET	)
    #define HPI_MEMORY__DEVICE_STATUS_REG               (17308   + HPI_MEMORY_MAP_ADDRESS_OFFSET	)
    #define HPI_MEMORY__BOOT_STATUS_REG                 (17312   + HPI_MEMORY_MAP_ADDRESS_OFFSET	)
																									
    #define HOST_TX_BUFFER_SIZE	                        (2*0x380									)
    #define HOST_RX_BUFFER_SIZE	                        (2*0x380									)
    #define HOST_TX_BUFFER_DESCRIPTORS_BUFFER_SIZE	    (2*0x50       								)
    #define HOST_RX_BUFFER_DESCRIPTORS_BUFFER_SIZE	    (2*0x50       								)
    #define NETWORK_TX_BUFFER_SIZE	                    (2*0xC00									)
    #define NETWORK_RX_BUFFER_SIZE	                    (2*0xC00									)
    #define NETWORK_TX_BUFFER_DESCRIPTORS_BUFFER_SIZE	(2*0x116      								)
    #define NETWORK_RX_BUFFER_DESCRIPTORS_BUFFER_SIZE	(2*0x116									)
#elif   (  (AC49X_DEVICE_TYPE == AC494_DEVICE)\
        || (AC49X_DEVICE_TYPE == AC495_DEVICE)\
		|| (AC49X_DEVICE_TYPE == AC495L_DEVICE)\
        || (AC49X_DEVICE_TYPE == AC496_DEVICE)\
		|| (AC49X_DEVICE_TYPE == AC496E_DEVICE)\
		|| (AC49X_DEVICE_TYPE == AC496E_DEVICE)\
        || (AC49X_DEVICE_TYPE == AC497_DEVICE)\
		|| (AC49X_DEVICE_TYPE == AC494E_DEVICE)\
		|| (AC49X_DEVICE_TYPE == AC495E_DEVICE)\
		|| (AC49X_DEVICE_TYPE == AC496D_DEVICE)\
        )

	#if (AC49X_DEVICE_TYPE == AC496E_DEVICE)
		#define	HPI_MEMORY_MAP_ADDRESS_OFFSET	            (2*0x10000									)
	#elif ((AC49X_DEVICE_TYPE == AC494E_DEVICE)\
		|| (AC49X_DEVICE_TYPE == AC495E_DEVICE)\
		|| (AC49X_DEVICE_TYPE == AC496D_DEVICE)\
		)
		#define	HPI_MEMORY_MAP_ADDRESS_OFFSET	            (2*0x12F48									)
	#else
		#define	HPI_MEMORY_MAP_ADDRESS_OFFSET	            (2*0x2800									)
	#endif

	#define HPI_DEVICE_MEMORY_MAP_ADDRESS_OFFSET		((Device>0)?(2*0x2800):HPI_MEMORY_MAP_ADDRESS_OFFSET)
	#define AC49XIF_BASE_PAGE							((HPI_DEVICE_MEMORY_MAP_ADDRESS_OFFSET/2)>>16		)
																									
    #define HOST_TX_BUFFER_SIZE	                        (2*0x300									)
    #define HOST_RX_BUFFER_SIZE	                        (2*0x300									)
    #define HOST_TX_BUFFER_DESCRIPTORS_BUFFER_SIZE	    (2*0x44       								)
    #define HOST_RX_BUFFER_DESCRIPTORS_BUFFER_SIZE	    (2*0x44       								)
    #define NETWORK_TX_BUFFER_SIZE	                    (2*0x480									)
    #define NETWORK_RX_BUFFER_SIZE	                    (2*0x480									)
    #define NETWORK_TX_BUFFER_DESCRIPTORS_BUFFER_SIZE	(2*0x60      								)
    #define NETWORK_RX_BUFFER_DESCRIPTORS_BUFFER_SIZE	(2*0x60										)

    #define HPI_MEMORY__HOST_TX_BUFFER                  (0											+ HPI_MEMORY_MAP_ADDRESS_OFFSET				)
    #define HPI_MEMORY__HOST_RX_BUFFER                  (HOST_TX_BUFFER_SIZE						+ HPI_MEMORY__HOST_TX_BUFFER				)          
    #define HPI_MEMORY__HOST_TX_BUFFER_DESCRIPTOR       (HOST_RX_BUFFER_SIZE						+ HPI_MEMORY__HOST_RX_BUFFER				)         
    #define HPI_MEMORY__HOST_RX_BUFFER_DESCRIPTOR       (HOST_TX_BUFFER_DESCRIPTORS_BUFFER_SIZE		+ HPI_MEMORY__HOST_TX_BUFFER_DESCRIPTOR		)   
    #define HPI_MEMORY__NETWORK_TX_BUFFER               (HOST_RX_BUFFER_DESCRIPTORS_BUFFER_SIZE		+ HPI_MEMORY__HOST_RX_BUFFER_DESCRIPTOR		)   
    #define HPI_MEMORY__NETWORK_RX_BUFFER               (NETWORK_TX_BUFFER_SIZE						+ HPI_MEMORY__NETWORK_TX_BUFFER				)           
    #define HPI_MEMORY__NETWORK_TX_BUFFER_DESCRIPTOR    (NETWORK_RX_BUFFER_SIZE						+ HPI_MEMORY__NETWORK_RX_BUFFER				)           
    #define HPI_MEMORY__NETWORK_RX_BUFFER_DESCRIPTOR    (NETWORK_TX_BUFFER_DESCRIPTORS_BUFFER_SIZE	+ HPI_MEMORY__NETWORK_TX_BUFFER_DESCRIPTOR	)
    #define HPI_MEMORY__DEVICE_CONTROL_REG              (NETWORK_RX_BUFFER_DESCRIPTORS_BUFFER_SIZE	+ HPI_MEMORY__NETWORK_RX_BUFFER_DESCRIPTOR	)
    #define HPI_MEMORY__DEVICE_STATUS_REG               (4 + HPI_MEMORY__DEVICE_CONTROL_REG			)          
    #define HPI_MEMORY__BOOT_STATUS_REG                 (4 + HPI_MEMORY__DEVICE_STATUS_REG			) 
	#define HPI_MEMORY__TRACE_COMMAND_REG               (4 + HPI_MEMORY__BOOT_STATUS_REG			) 
	#define HPI_MEMORY__TRACE_STATUS_REG                (4 + HPI_MEMORY__TRACE_COMMAND_REG			) 


    #define PROGRAM_DOWLOAD__EXTERNAL_MEMORY_INTERFACE_FLAG 0x20		/* In Words												*/
#endif  /*AC49X_DEVICE_TYPE*/

#define PROGRAM_DOWLOAD__LAST_BLOCK_FLAG            0x80
#define NUMBER_OF_HOST_TX_BUFFER_DESCRIPTORS	    (HOST_TX_BUFFER_DESCRIPTORS_BUFFER_SIZE   /sizeof(Tac49xBufferDescriptor)	)
#define NUMBER_OF_HOST_RX_BUFFER_DESCRIPTORS	    (HOST_RX_BUFFER_DESCRIPTORS_BUFFER_SIZE   /sizeof(Tac49xBufferDescriptor)	)
#define NUMBER_OF_NETWORK_TX_BUFFER_DESCRIPTORS	    (NETWORK_TX_BUFFER_DESCRIPTORS_BUFFER_SIZE/sizeof(Tac49xBufferDescriptor)	)
#define NUMBER_OF_NETWORK_RX_BUFFER_DESCRIPTORS	    (NETWORK_RX_BUFFER_DESCRIPTORS_BUFFER_SIZE/sizeof(Tac49xBufferDescriptor)	)

            /* The following constant is used for default calculation of the devices physical addresses.*/
            /* If the devices are mapped continuously in memory, use sizeof, otherwise use a constant value. */
            /* The user can override this definition in the configuration */
#ifndef AC49XIF_DEVICE_OFFSET
    #if (  (AC49X_DEVICE_TYPE == AC490_DEVICE)\
        || (AC49X_DEVICE_TYPE == AC494_DEVICE)\
        || (AC49X_DEVICE_TYPE == AC495_DEVICE)\
		|| (AC49X_DEVICE_TYPE == AC495L_DEVICE)\
        || (AC49X_DEVICE_TYPE == AC496_DEVICE)\
		|| (AC49X_DEVICE_TYPE == AC496E_DEVICE)\
        || (AC49X_DEVICE_TYPE == AC497_DEVICE)\
		|| (AC49X_DEVICE_TYPE == AC494E_DEVICE)\
		|| (AC49X_DEVICE_TYPE == AC495E_DEVICE)\
		|| (AC49X_DEVICE_TYPE == AC496D_DEVICE)\
        )
        #define AC49XIF_DEVICE_OFFSET	sizeof(Tac49xDeviceMemory)
    #else
        #define AC49XIF_DEVICE_OFFSET	sizeof(Tac49xHpiPort)
    #endif
#endif /* AC49XIF_DEVICE_OFFSET */

#if (  (AC49X_DEVICE_TYPE == AC491_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC491L_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC491_491L_DEVICE)\
	)

#if AC49X_ENDIAN_MODE == BIG_ENDIAN
    /*HPIC Register*/
    #define     MSB_FIRST         0
    #define     LSB_FIRST         1
    #define     HINT_ACTIVE       0
    #define     HINT_DEACTIVE     4
#else
/*HPIC Register*/
    #define     MSB_FIRST         0
    #define     LSB_FIRST         0x100
    #define     HINT_ACTIVE       0
    #define     HINT_DEACTIVE     0x400
#endif /* AC49X_ENDIAN_MODE */

                    /* AC491xxx HPI structure (for multiplexed mode)					*/
                    /* Notes :															*/
                    /* - AC49xxx HPI Bus width is 16 bits.								*/	
                    /* - Since Host address bits 4 and 5 are used to address the HPI,	*/
                    /*   7 padding words were inserted between each HPI word			*/
                    /* - There is no need to address different words for read and write.*/
typedef volatile struct 
     {
                 /*Control Register*/
     unsigned short HPIC1st;           /*Write first byte */
     unsigned short HPI_fill1[7];	   /* 7 padding words */
     unsigned short HPIC2nd;           /*Write second byte*/
     unsigned short HPI_fill2[7];	   /* 7 padding words */
                 /*Data transfer with Post increment after read or write.*/
     unsigned short HPIDA1st;          /*Write first byte */
     unsigned short HPI_fill3[7];	   /* 7 padding words */
     unsigned short HPIDA2nd;          /*Write second byte*/
     unsigned short HPI_fill4[7];	   /* 7 padding words */
                 /*Address Register*/
     unsigned short HPIA1st;           /*Write first byte */
     unsigned short HPI_fill5[7];	   /* 7 padding words */
     unsigned short HPIA2nd;           /*Write second byte*/
     unsigned short HPI_fill6[7];	   /* 7 padding words */
                 /*Data Latches*/
     unsigned short HPID1st;           /*Write first byte */
     unsigned short HPI_fill7[7];	   /* 7 padding words */
     unsigned short HPID2nd;           /*Write second byte*/
     unsigned short HPI_fill8[7];	   /* 7 padding words */
     } Tac49xHpiPort;

#elif   (  (AC49X_DEVICE_TYPE == AC490_DEVICE)\
        || (AC49X_DEVICE_TYPE == AC494_DEVICE)\
        || (AC49X_DEVICE_TYPE == AC495_DEVICE)\
		|| (AC49X_DEVICE_TYPE == AC495L_DEVICE)\
        || (AC49X_DEVICE_TYPE == AC496_DEVICE)\
		|| (AC49X_DEVICE_TYPE == AC496E_DEVICE)\
        || (AC49X_DEVICE_TYPE == AC497_DEVICE)\
		|| (AC49X_DEVICE_TYPE == AC494E_DEVICE)\
		|| (AC49X_DEVICE_TYPE == AC495E_DEVICE)\
		|| (AC49X_DEVICE_TYPE == AC496D_DEVICE)\
        )
typedef volatile struct 
     {
	 unsigned long Hpi;
     } Tac49xHpiPort;
#endif

typedef volatile struct 
    {
	U8	                        PaddingBuffer[HPI_MEMORY_MAP_ADDRESS_OFFSET];  
                                
	U8	                        HostTxBuffer[HOST_TX_BUFFER_SIZE];
	U8	                        HostRxBuffer[HOST_RX_BUFFER_SIZE];
	Tac49xBufferDescriptor	    HostTxBufferDescriptor[NUMBER_OF_HOST_TX_BUFFER_DESCRIPTORS]; 
	Tac49xBufferDescriptor	    HostRxBufferDescriptor[NUMBER_OF_HOST_TX_BUFFER_DESCRIPTORS]; 
                                
	U8	                        NetworkTxBuffer[NETWORK_TX_BUFFER_SIZE];
	U8	                        NetworkRxBuffer[NETWORK_RX_BUFFER_SIZE];
	Tac49xBufferDescriptor	    NetworkTxBufferDescriptor[NUMBER_OF_NETWORK_TX_BUFFER_DESCRIPTORS]; 
	Tac49xBufferDescriptor	    NetworkRxBufferDescriptor[NUMBER_OF_NETWORK_TX_BUFFER_DESCRIPTORS]; 
	
	Tac49xDeviceControlRegister	DeviceControlRegister;	                               
	Tac49xDeviceStatusRegister	DeviceStatusRegister;	                               
	Tac49xBootStatusRegister	BootStatusRegister;	                               
    } Tac49xDeviceMemory;

typedef struct
    {
	int	Tx;	   
	int Rx;	   
    } Tac49xBufferDescriptorCurrentIndex;

#define Tac49xFifoCellSize(FifoCellSize, Name)          \
    struct Tac49xFifoCell##Name                         \
        {                                               \
        int                     Partial;                \
        U16                     Size;                   \
        U8                      Buffer[FifoCellSize];   \
        }                         

#define Tac49xFifo(FifoSize, FifoCellSize, Name)                    \
    struct Tac49xFifo##Name                                          \
        {                                                            \
        U16                                         Head;            \
        U16                                         Tail;            \
        U16                                         ResidentCounter; \
        U16                                         MaxUsage;        \
        Tac49xFifoCellSize(FifoCellSize, Name)      Cell[FifoSize];  \
        } 

#define Tac49x_DeclareFifo(FifoSize,FifoCellSize,Name)  Tac49xFifo(FifoSize,FifoCellSize,Name)

/*****************************************\
*******************************************
**      Enumerated Type Definitions      **
*******************************************
\*****************************************/

typedef enum
	{
	EMIF_TEST_TYPE__PHYSICAL_MEMORY_SPACE=1,
	EMIF_TEST_TYPE__HIGH_ADDRESS_LINES,
	EMIF_TEST_TYPE__LOW_ADDRESS_LINES
	} Tac49xEmifTestType;

typedef enum
	{
	BYTE_ORDERING_TEST_STATUS__OK,
	BYTE_ORDERING_TEST_STATUS__FAILED_DUE_TO_SWAP,
	BYTE_ORDERING_TEST_STATUS__FAILED_DUE_TO_HPI_ACCESS_PROBLEM,
	BYTE_ORDERING_TEST_STATUS__FAILED_DUE_TO_AC491_GPIO_LOW,
	BYTE_ORDERING_TEST_STATUS__FAILED_DUE_TO_WRONG_DEVICE_ID
	} Tac49xByteOrderingTestStatus;

typedef enum
    {
	TRANSFER_MEDIUM__HOST,
	TRANSFER_MEDIUM__NETWORK
	} Tac49xTransferMedium;

typedef enum
	{
	TRANSFER_PORT__HPI_HOST,
	TRANSFER_PORT__HPI_NETWORK,
	TRANSFER_PORT__UTOPIA
	} Tac49xTransferPort;

typedef enum
    {
    FIFO_ACTION_RESULT__SUCCEEDED,    
    FIFO_ACTION_RESULT__FIFO_IS_FULL_ERROR,    
    FIFO_ACTION_RESULT__FIFO_IS_EMPTY_ERROR,
    FIFO_ACTION_RESULT__BUFFER_DESCRIPTOR_FULL_ERROR  
    } Tac49xFifoActionResult;

typedef enum
	{
	CONTROL__DISABLE,
	CONTROL__ENABLE
	} Tac49xControl;

typedef enum
	{
	PROGRAM_DOWNLOAD_STATUS__BOOT_STATUS_REGISTER_NOT_READY,      /*!< AC49x is busy downloading the block, the host must wait*/
	PROGRAM_DOWNLOAD_STATUS__BOOT_STATUS_REGISTER_SUCCESS,        /*!< Success*/
	PROGRAM_DOWNLOAD_STATUS__BOOT_STATUS_REGISTER_FAIL_CHECKSUM,  /*!< Download failed*/
	PROGRAM_DOWNLOAD_STATUS__EMIF_TEST_FAILED,                    /*!< EMIF Test failed*/
	PROGRAM_DOWNLOAD_STATUS__NULL_BOOT_BLOCK=0x10000              /*!< Download failed*/   /* the offset is for garbage recognition (as the register width is 16 bits) */
	} Tac49xProgramDownloadStatus;

typedef enum
	{
	SETUP_DEVICE_STATUS__SUCCESS,
	SETUP_DEVICE_STATUS__FAILED__DUE_TO_SET_DEVICE_CONTROL_REGISTER,
	SETUP_DEVICE_STATUS__FAILED__DUE_TO_UTOPIA_ROUTING_CONFIGURATION,
	SETUP_DEVICE_STATUS__FAILED__DUE_TO_CALL_PROGRESS_CONFIGURATION,
	SETUP_DEVICE_STATUS__FAILED__DUE_TO_USER_DEFINED_TONES_CONFIGURATION,
	SETUP_DEVICE_STATUS__FAILED__DUE_TO_AGC_CONFIGURATION,
	SETUP_DEVICE_STATUS__FAILED__DUE_TO_EXTENDED_CONFIGURATION,
	SETUP_DEVICE_STATUS__FAILED__DUE_TO_OPEN_CONFIGURATION,
	SETUP_DEVICE_STATUS__FAILED__DUE_TO_ECHO_CANCELER_EQUALIZER_CONFIGURATION,
	SETUP_DEVICE_STATUS__FAILED__DUE_TO_LOW_PASS_FILTER_CONFIGURATION
	} Tac49xSetupDeviceStatus;

typedef enum
    {
    PROTOCOL__PROPRIETARY,
    PROTOCOL__RTP,
	PROTOCOL__RTCP,
	PROTOCOL__FAX,
    PROTOCOL__ATM_AAL2,
    PROTOCOL__ATM_AAL1,
	PROTOCOL__MEDIATED_CHANNEL_RTP,
	PROTOCOL__MEDIATED_CHANNEL_RTCP,
	PROTOCOL__CONFERENCE_1,
	PROTOCOL__CONFERENCE_2,
	PROTOCOL__DEBUG,
	PROTOCOL__PLAYBACK_COMMAND,
	PROTOCOL__PLAYBACK_VOICE,
	PROTOCOL__IP_SEC,
	PROTOCOL__CONFERENCE,
	PROTOCOL__MEDIATED_CHANNEL_ATM_AAL1,
	PROTOCOL__MEDIATED_CHANNEL_ATM_AAL2,
	PROTOCOL__RECORD_COMMAND,
	PROTOCOL__RECORD_TDM_PACKET,
	PROTOCOL__RECORD_NETWORK_PACKET,
    PROTOCOL__3GPP_IUB,
	PROTOCOL__DATA_RELAY,
	PROTOCOL__MEDIATED_CHANNEL_FAX,
	PROTOCOL__ERROR
    } Tac49xProtocol;

#define PROTOCOL__T38 PROTOCOL__FAX

typedef enum
    {
	PACKET_CATEGORY__DEVICE_CONFIGURATION_PACKET,
	PACKET_CATEGORY__CHANNEL_CONFIGURATION_PACKET,
	PACKET_CATEGORY__NETWORK_PACKET,
	PACKET_CATEGORY__COMMAND_OR_STATUS_PACKET
    } Tac49xPacketCategory;

typedef enum
	{
	FORMAT__0,
	FORMAT__1,
	FORMAT__2,
	FORMAT__3,
	FORMAT__4,
	FORMAT__5
	} Tac49xFormat;

typedef enum
    {
    DEVICE_CONFIGURATION_OP_CODE__CALL_PROGRESS,
    DEVICE_CONFIGURATION_OP_CODE__USER_DEFINED_TONES,
    DEVICE_CONFIGURATION_OP_CODE__AGC,
	DEVICE_CONFIGURATION_OP_CODE__UTOPIA_ROUTING,
	DEVICE_CONFIGURATION_OP_CODE__EXTENDED,
    DEVICE_CONFIGURATION_OP_CODE__OPEN=6,
	DEVICE_CONFIGURATION_OP_CODE__ECHO_CANCELER_EQUALIZER=8,
	DEVICE_CONFIGURATION_OP_CODE__LOW_PASS_FILTER
    } Tac49xDeviceConfigurationOpCode;

typedef enum  
    {
    CHANNEL_CONFIGURATION_OP_CODE__UPDATE_CHANNEL,
    CHANNEL_CONFIGURATION_OP_CODE__ADVANCED,
    CHANNEL_CONFIGURATION_OP_CODE__CLOSE_CHANNEL,
    CHANNEL_CONFIGURATION_OP_CODE__ACTIVATE_REGULAR_RTP, 
    CHANNEL_CONFIGURATION_OP_CODE__UPDATE_REGULAR_RTP,
    CHANNEL_CONFIGURATION_OP_CODE__CLOSE_REGULAR_RTP,
    CHANNEL_CONFIGURATION_OP_CODE__REGULAR_CHANNEL_ACTIVATE_ATM,
    CHANNEL_CONFIGURATION_OP_CODE__REGULAR_CHANNEL_UPDATE_ATM,
    CHANNEL_CONFIGURATION_OP_CODE__REGULAR_CHANNEL_CLOSE_ATM,
    CHANNEL_CONFIGURATION_OP_CODE__MEDIATED_CHANNEL=11,
    CHANNEL_CONFIGURATION_OP_CODE__T38,
    CHANNEL_CONFIGURATION_OP_CODE__ACTIVATE_FAX_RELAY,
    CHANNEL_CONFIGURATION_OP_CODE__DEACTIVATE_FAX_RELAY,
    CHANNEL_CONFIGURATION_OP_CODE__OPEN=15,
    CHANNEL_CONFIGURATION_OP_CODE__ACTIVATE_MEDIATED_RTP,
    CHANNEL_CONFIGURATION_OP_CODE__UPDATE_MEDIATED_RTP,  
    CHANNEL_CONFIGURATION_OP_CODE__CLOSE_MEDIATED_RTP,
    CHANNEL_CONFIGURATION_OP_CODE__REGULAR_CHANNEL_3GPP_UP_INITIALIZATION=20,
    CHANNEL_CONFIGURATION_OP_CODE__RFC3267_AMR_INITIALIZATION,
    CHANNEL_CONFIGURATION_OP_CODE__ACTIVATE_3_WAY_CONFERENCE,
    CHANNEL_CONFIGURATION_OP_CODE__REGULAR_CHANNEL_MEDIA_PROTECTION_INITIALIZATION,
    CHANNEL_CONFIGURATION_OP_CODE__CODEC_CONFIGURATION,
    CHANNEL_CONFIGURATION_OP_CODE__MEDIATED_CHANNEL_3GPP_UP_INITIALIZATION,
    CHANNEL_CONFIGURATION_OP_CODE__MEDIATED_CHANNEL_ACTIVATE_ATM,
    CHANNEL_CONFIGURATION_OP_CODE__MEDIATED_CHANNEL_UPDATE_ATM,
    CHANNEL_CONFIGURATION_OP_CODE__MEDIATED_CHANNEL_CLOSE_ATM,
    CHANNEL_CONFIGURATION_OP_CODE__MEDIATED_CHANNEL_MEDIA_PROTECTION_INITIALIZATION,
    CHANNEL_CONFIGURATION_OP_CODE__ACOUSTIC_ECHO_CANCELER,
    CHANNEL_CONFIGURATION_OP_CODE__RFC3558_INITIALIZATION,
    CHANNEL_CONFIGURATION_OP_CODE__G729EV_INITIALIZATION,
    CHANNEL_CONFIGURATION_OP_CODE__ACTIVATE_3GPP_IUB,
    CHANNEL_CONFIGURATION_OP_CODE__UPDATE_3GPP_IUB,
    CHANNEL_CONFIGURATION_OP_CODE__CLOSE_3GPP_IUB,
	CHANNEL_CONFIGURATION_OP_CODE__REGULAR_CHANNEL_RTCP_XR,
	CHANNEL_CONFIGURATION_OP_CODE__MEDIATED_CHANNEL_RTCP_XR,
	CHANNEL_CONFIGURATION_OP_CODE__4_WAY_CONFERENCE_OPEN,
	CHANNEL_CONFIGURATION_OP_CODE__4_WAY_CONFERENCE_UPDATE,
	CHANNEL_CONFIGURATION_OP_CODE__4_WAY_CONFERENCE_CLOSE,
	CHANNEL_CONFIGURATION_OP_CODE__4_WAY_CONFERENCE_ADD_LEG,
	CHANNEL_CONFIGURATION_OP_CODE__4_WAY_CONFERENCE_REMOVE_LEG,
	CHANNEL_CONFIGURATION_OP_CODE__UEM_CLIP_INITIALIZATION,
	CHANNEL_CONFIGURATION_OP_CODE__DATA_RELAY_ACTIVATE,
	CHANNEL_CONFIGURATION_OP_CODE__DATA_RELAY_UPDATE,
	CHANNEL_CONFIGURATION_OP_CODE__DATA_RELAY_DEACTIVATE,
	CHANNEL_CONFIGURATION_OP_CODE__DATA_RELAY_ADVANCED,
	CHANNEL_CONFIGURATION_OP_CODE__VOICE_ENHANCEMENT_UPDATE=49,
	CHANNEL_CONFIGURATION_OP_CODE__G711_WB_INITIALIZATION,
	CHANNEL_CONFIGURATION_OP_CODE__MICROSOFT_RTA_INITIALIZATION,
	CHANNEL_CONFIGURATION_OP_CODE__REGULAR_CHANNEL_MEDIA_PROTECTION_UPDATE,
	CHANNEL_CONFIGURATION_OP_CODE__MEDIATED_CHANNEL_MEDIA_PROTECTION_UPDATE,
	CHANNEL_CONFIGURATION_OP_CODE__NETWORK_ACOUSTIC_ECHO_SUPPRESSOR,
	CHANNEL_CONFIGURATION_OP_CODE__MEDIATED_CHANNEL_T38,
	CHANNEL_CONFIGURATION_OP_CODE__SPEEX_INITIALIZATION,
	CHANNEL_CONFIGURATION_OP_CODE__OPUS_INITIALIZATION,
	CHANNEL_CONFIGURATION_OP_CODE__VOICE_ENHANCEMENT_INITIALIZATION,
	CHANNEL_CONFIGURATION_OP_CODE__9_WAY_CONFERENCE_ACTIVATE,
	CHANNEL_CONFIGURATION_OP_CODE__9_WAY_CONFERENCE_RESOURCE_CONFIGURATION,
	CHANNEL_CONFIGURATION_OP_CODE__9_WAY_CONFERENCE_DEACTIVATE,
	CHANNEL_CONFIGURATION_OP_CODE__SILK_INITIALIZATION,
	CHANNEL_CONFIGURATION_OP_CODE__CALL_PROGRESS_UPDATE,
    } Tac49xChannelConfigurationOpCode;

typedef enum
    {
    NETWORK_PACKET_OP_CODE__REGULAR_VOICE,
    NETWORK_PACKET_OP_CODE__SID_OR_SID_WITH_VOICE,
    NETWORK_PACKET_OP_CODE__BAD_FRAME_INTERPOLATION,
    NETWORK_PACKET_OP_CODE__FAX_BYPASS,
    NETWORK_PACKET_OP_CODE__DATA_BYPASS,
    NETWORK_PACKET_OP_CODE__FAX,
    NETWORK_PACKET_OP_CODE__DATA,
    NETWORK_PACKET_OP_CODE__CAS,
    NETWORK_PACKET_OP_CODE__IBS,
/*  NETWORK_PACKET_OP_CODE__PLAYBACK_VOICE=9,*/
/*  NETWORK_PACKET_OP_CODE__PLAYBACK_COMMAND,*/
    NETWORK_PACKET_OP_CODE__PCM,
    NETWORK_PACKET_OP_CODE__NONE = 16
    } Tac49xNetworkPacketOpCode;
                    
typedef enum        
    {               
    COMMAND_PACKET_OP_CODE__IBS_STRING,
    COMMAND_PACKET_OP_CODE__IBS_STOP,
    COMMAND_PACKET_OP_CODE__CAS_STRING,
    COMMAND_PACKET_OP_CODE__CAS_GENERATE,
    COMMAND_PACKET_OP_CODE__HDLC_FRAMER,
    COMMAND_PACKET_OP_CODE__CALLER_ID,
    COMMAND_PACKET_OP_CODE__CALLER_ID_STOP,
    COMMAND_PACKET_OP_CODE__SS7_MTP1,
    COMMAND_PACKET_OP_CODE__GET_REGULAR_CHANNEL_RTCP_PARAMETERS=8,
    COMMAND_PACKET_OP_CODE__GET_MEDIATED_CHANNEL_RTCP_PARAMETERS,
    COMMAND_PACKET_OP_CODE__SET_REGULAR_CHANNEL_RTCP_PARAMETERS,
    COMMAND_PACKET_OP_CODE__SET_MEDIATED_CHANNEL_RTCP_PARAMETERS,
    COMMAND_PACKET_OP_CODE__GET_REGULAR_CHANNEL_3GPP_STATISTICS,
    COMMAND_PACKET_OP_CODE__SET_REGULAR_CHANNEL_3GPP_USER_PLANE_RATE_CONTROL,
    COMMAND_PACKET_OP_CODE__SET_RFC3267_AMR_RATE_CONTROL,
    COMMAND_PACKET_OP_CODE__EXTENDED_IBS_STRING,
    COMMAND_PACKET_OP_CODE__TRUNK_TESTING_MEASUREMENT,
    COMMAND_PACKET_OP_CODE__BER_TEST,
    COMMAND_PACKET_OP_CODE__UTOPIA_STATUS_REQUEST,
    COMMAND_PACKET_OP_CODE__GET_MEDIATED_CHANNEL_3GPP_STATISTICS,
    COMMAND_PACKET_OP_CODE__SET_MEDIATED_CHANNEL_3GPP_USER_PLANE_RATE_CONTROL,
	COMMAND_PACKET_OP_CODE__TANDEM_FREE_OPERATION,
    COMMAND_PACKET_OP_CODE__RESET_SERIAL_PORT,
	COMMAND_PACKET_OP_CODE__GET_REGULAR_CHANNEL_MEDIA_PROTECTION_INFO_COMMAND,
	COMMAND_PACKET_OP_CODE__GET_MEDIATED_CHANNEL_MEDIA_PROTECTION_INFO_COMMAND,
	COMMAND_PACKET_OP_CODE__SET_RFC3558_RATE_MODE,
	COMMAND_PACKET_OP_CODE__TIME_SLOT_MANIPULATION,
	COMMAND_PACKET_OP_CODE__MODULE_STATUS_REQUEST,
	COMMAND_PACKET_OP_CODE__SEND_REGULAR_RTCP_APP,
	COMMAND_PACKET_OP_CODE__SEND_MEDIATED_RTCP_APP,
    COMMAND_PACKET_OP_CODE__DUA_FRAMER = 30,
    COMMAND_PACKET_OP_CODE__EVENTS_CONTROL,
    COMMAND_PACKET_OP_CODE__NTP_TIME_STAMP,
    COMMAND_PACKET_OP_CODE__VIDEO_AUDIO_SYNC,
    COMMAND_PACKET_OP_CODE__G729EV_RATE_CONTROL,
    COMMAND_PACKET_OP_CODE__GET_3GPP_IUB_STATUS,
	COMMAND_PACKET_OP_CODE__AMD,
	COMMAND_PACKET_OP_CODE__GET_REGULAR_CHANNEL_RTCP_XR_MARKOV_MODEL_STATISTICS,
	COMMAND_PACKET_OP_CODE__GET_MEDIATED_CHANNEL_RTCP_XR_MARKOV_MODEL_STATISTICS,
	COMMAND_PACKET_OP_CODE__GET_REGULAR_CHANNEL_RTCP_XR_VOICE_QUALITY_STATISTICS,
	COMMAND_PACKET_OP_CODE__GET_MEDIATED_CHANNEL_RTCP_XR_VOICE_QUALITY_STATISTICS,
	COMMAND_PACKET_OP_CODE__GET_LAST_CAS_DETECTION,
	COMMAND_PACKET_OP_CODE__GET_4_WAY_CONFERENCE_STATUS,
	COMMAND_PACKET_OP_CODE__GET_4_WAY_CONFERENCE_DETAILS,
	COMMAND_PACKET_OP_CODE__PROTECTION_TEST=48,
	COMMAND_PACKET_OP_CODE__GET_RANDOM_NUMBER,
	COMMAND_PACKET_OP_CODE__SWITCH_TO_MODEM,
	COMMAND_PACKET_OP_CODE__MODEM_RELAY_ENDED,
	COMMAND_PACKET_OP_CODE__MODEM_RELAY_SSE,
	COMMAND_PACKET_OP_CODE__UPDATE_IBS_GENERATION_LEVEL,
	COMMAND_PACKET_OP_CODE__RECORD,
	COMMAND_PACKET_OP_CODE__MUTE_CNG,
    } Tac49xCommandPacketOpCode;
                           
typedef enum               
    {                      
    STATUS_OR_EVENT_PACKET_OP_CODE__SYSTEM_ERROR_EVENT,
    STATUS_OR_EVENT_PACKET_OP_CODE__GENERAL_EVENT,
    STATUS_OR_EVENT_PACKET_OP_CODE__IBS_EVENT,
    STATUS_OR_EVENT_PACKET_OP_CODE__CAS_EVENT,
    STATUS_OR_EVENT_PACKET_OP_CODE__HDLC_FRAMER_STATUS,
    STATUS_OR_EVENT_PACKET_OP_CODE__FAX_RELAY_STATUS,
    STATUS_OR_EVENT_PACKET_OP_CODE__JITTER_BUFFER_STATUS,
    STATUS_OR_EVENT_PACKET_OP_CODE__CALLER_ID,
	STATUS_OR_EVENT_PACKET_OP_CODE__PACKETIZER_STATUS_OR_EVENT,
	STATUS_OR_EVENT_PACKET_OP_CODE__DEVICE_STATUS=10,
	STATUS_OR_EVENT_PACKET_OP_CODE__DEBUG_INFORMATION_STATUS,
	STATUS_OR_EVENT_PACKET_OP_CODE__FAX_OR_DATA_SIGNAL_EVENT,
	STATUS_OR_EVENT_PACKET_OP_CODE__SS7_MTP1,
	STATUS_OR_EVENT_PACKET_OP_CODE__UTOPIA_TEST_ERROR_EVENT,
	STATUS_OR_EVENT_PACKET_OP_CODE__TRUNK_TESTING_MEASUREMENT_EVENT,
	STATUS_OR_EVENT_PACKET_OP_CODE__BER_TEST_REPORT_EVENT,
	STATUS_OR_EVENT_PACKET_OP_CODE__UTOPIA_STATUS,
	STATUS_OR_EVENT_PACKET_OP_CODE__TANDEM_FREE_OPERATION_STATUS_AND_MESSAGES,
	STATUS_OR_EVENT_PACKET_OP_CODE__MODULE_STATUS,
	STATUS_OR_EVENT_PACKET_OP_CODE__DUA_FRAMER,
	STATUS_OR_EVENT_PACKET_OP_CODE__VIDEO_AUDIO_SYNC,
	STATUS_OR_EVENT_PACKET_OP_CODE__4_WAY_CONFERENCE_STATUS,
	STATUS_OR_EVENT_PACKET_OP_CODE__4_WAY_CONFERENCE_DETAILS,
	STATUS_OR_EVENT_PACKET_OP_CODE__PROTECTION_TEST_RESPONSE=28,
	STATUS_OR_EVENT_PACKET_OP_CODE__RANDOM_NUMBER_RESPONSE,
	STATUS_OR_EVENT_PACKET_OP_CODE__MODULE_DEBUG_PACKET,
	STATUS_OR_EVENT_PACKET_OP_CODE__MODEM_RELAY,
	STATUS_OR_EVENT_PACKET_OP_CODE__TDM_RECORD,
	STATUS_OR_EVENT_PACKET_OP_CODE__NETWORK_RECORD,
	STATUS_OR_EVENT_PACKET_OP_CODE__T38_MONITOR,
	STATUS_OR_EVENT_PACKET_OP_CODE__USB_SYNCHRONIZATION_STATUS,
	STATUS_OR_EVENT_PACKET_OP_CODE__ERROR
    } Tac49xStatusOrEventPacketOpCode;    

typedef enum
    {
    SYSTEM_ERROR_EVENT_PACKET_INDEX__OF_PACKET_ERROR,
    SYSTEM_ERROR_EVENT_PACKET_INDEX__TIME_SLOT_MISALIGNMENT_DETECTED_AND_CORRECTED,
    SYSTEM_ERROR_EVENT_PACKET_INDEX__COMMAND_SEQUENCE,
    SYSTEM_ERROR_EVENT_PACKET_INDEX__COMMAND_CHECKSUM,
    SYSTEM_ERROR_EVENT_PACKET_INDEX__RX_TX_HOST_AND_NETWORK_OVERRUN,
	SYSTEM_ERROR_EVENT_PACKET_INDEX__OPEN_DEVICE_PARAMETERS,
	SYSTEM_ERROR_EVENT_PACKET_INDEX__ILLEGAL_COMMAND,
	SYSTEM_ERROR_EVENT_PACKET_INDEX__PLAYBACK_JITTER_BUFFER_UNDERRUN,
	SYSTEM_ERROR_EVENT_PACKET_INDEX__PLAYBACK_JITTER_BUFFER_OVERRUN,
	SYSTEM_ERROR_EVENT_PACKET_INDEX__3WAY_CONFERENCE_CONFIGURATION_ERROR,
	SYSTEM_ERROR_EVENT_PACKET_INDEX__ILLEGAL_UTOPIA_CELL,
	SYSTEM_ERROR_EVENT_PACKET_INDEX__3WAY_CONFERENCE_OVERRUN,
	SYSTEM_ERROR_EVENT_PACKET_INDEX__ILLEGAL_PROTOCOL,
	SYSTEM_ERROR_EVENT_PACKET_INDEX__UDP_CHECKSUM_ERROR,
	SYSTEM_ERROR_EVENT_PACKET_INDEX__WRONG_CHANNEL_ID,
	SYSTEM_ERROR_EVENT_PACKET_INDEX__WRONG_PAYLOAD_SIZE,
	SYSTEM_ERROR_EVENT_PACKET_INDEX__WRONG_PROTOCOL,
	SYSTEM_ERROR_EVENT_PACKET_INDEX__ILLEGAL_4_WAY_CONFERENCE_COMMAND,
	SYSTEM_ERROR_EVENT_PACKET_INDEX__SERIAL_PORT_ERROR,
	SYSTEM_ERROR_EVENT_PACKET_INDEX__TIMESLOT_MISALIGNMENT_DETECTED_AND_CORRECTED_DURING_INITIALIZATION,
	SYSTEM_ERROR_EVENT_PACKET_INDEX__OPEN_DEVICE_FAILED__NUMBER_OF_CHANNELS_DOESNT_MATCH_PROFILE,
	SYSTEM_ERROR_EVENT_PACKET_INDEX__DMA_MISALIGNMENT_DETECTED_AND_CORRECTED,
	SYSTEM_ERROR_EVENT_PACKET_INDEX__ILLEGAL_9_WAY_CONFERENCE_COMMAND
    } Tac49xSystemErrorEventPacketIndex;
                              
typedef enum                  
    {
    GENERAL_EVENT_PACKET_INDEX__CAS_STRING_GENERATION_ENDED,
    GENERAL_EVENT_PACKET_INDEX__IBS_STRING_GENERATION_ENDED,
	GENERAL_EVENT_PACKET_INDEX__BROKEN_CONNECTION,
	GENERAL_EVENT_PACKET_INDEX__RESTORE_CONNECTION,
	GENERAL_EVENT_PACKET_INDEX__PLAYBACK_ENDED,
	GENERAL_EVENT_PACKET_INDEX__IBS_STRING_GENERATION_ABORTED,
	GENERAL_EVENT_PACKET_INDEX__BROKEN_PACKET_2_PACKET_CONNECTION,
	GENERAL_EVENT_PACKET_INDEX__SWITCHED_TO_VOICE,
	GENERAL_EVENT_PACKET_INDEX__SWITCHED_TO_BYPASS,
	GENERAL_EVENT_PACKET_INDEX__SWITCHED_TO_FAX,
	GENERAL_EVENT_PACKET_INDEX__SWITCHED_TO_DATA,
	GENERAL_EVENT_PACKET_INDEX__RESET_SERIAL_PORT_ACKNOWLEDGE,
	GENERAL_EVENT_PACKET_INDEX__TDM_RECORD_STARTED,
	GENERAL_EVENT_PACKET_INDEX__TDM_RECORD_ENDED,
	GENERAL_EVENT_PACKET_INDEX__NETWORK_RECORD_STARTED,
	GENERAL_EVENT_PACKET_INDEX__NETWORK_RECORD_ENDED,
	GENERAL_EVENT_PACKET_INDEX__TDM_INBAND_TEXT_RELAY_STARTED,
	GENERAL_EVENT_PACKET_INDEX__TDM_INBAND_TEXT_RELAY_ENDED,
	GENERAL_EVENT_PACKET_INDEX__NETWORK_INBAND_TEXT_RELAY_STARTED,
	GENERAL_EVENT_PACKET_INDEX__NETWORK_INBAND_TEXT_RELAY_ENDED,
	GENERAL_EVENT_PACKET_INDEX__INBAND_TEXT_RELAY_TERMINATED,
    GENERAL_EVENT_PACKET_INDEX__TDM_SERIAL_PORT_HALTED,
    GENERAL_EVENT_PACKET_INDEX__TDM_SERIAL_PORT_RESUMED,
	GENERAL_EVENT_PACKET_INDEX__ANSWER_DETECTOR_DETECTED_SPEECH,
	GENERAL_EVENT_PACKET_INDEX__ANSWER_DETECTOR_DETECTED_END_OF_SPEECH,
	GENERAL_EVENT_PACKET_INDEX__ANSWER_MACHINE_DETECTOR_STATUS_LIVE_CALL,
	GENERAL_EVENT_PACKET_INDEX__ANSWER_MACHINE_DETECTOR_STATUS_ANSWERING_MACHINE,
	GENERAL_EVENT_PACKET_INDEX__ANSWER_MACHINE_DETECTOR_STATUS_NO_DECISION,
	GENERAL_EVENT_PACKET_INDEX__ANSWER_MACHINE_DETECTOR_STATUS_SILENT_CALL,
	GENERAL_EVENT_PACKET_INDEX__ANSWER_MACHINE_DETECTOR_STATUS_BEEP_DETECTED,
	GENERAL_EVENT_PACKET_INDEX__ANSWER_MACHINE_DETECTOR_STATUS_BEEP_TIMEOUT_EXPIRED,
	GENERAL_EVENT_PACKET_INDEX__H324_START_DETECTION,
	GENERAL_EVENT_PACKET_INDEX__H324_END_DETECTION,
	GENERAL_EVENT_PACKET_INDEX__ENERGY_DETECTION_SIGNAL_ENERGY_LOWER_THAN_THRESHOLD,
	GENERAL_EVENT_PACKET_INDEX__ENERGY_DETECTION_SIGNAL_ENERGY_HIGHER_THAN_THRESHOLD,
	GENERAL_EVENT_PACKET_INDEX__DENIAL_OF_SERVICE_ATTACK_ANALYSIS,
	GENERAL_EVENT_PACKET_INDEX__DSP_RECEIVED_VALID_T38_PACKET
    } Tac49xGeneralEventPacketIndex;

typedef enum             
    {
	MEDIA_GATEWAYE_STATE__AUDIO=1,
	MEDIA_GATEWAYE_STATE__VOICE_BAND_DATA,
	MEDIA_GATEWAYE_STATE__MOIP,
	MEDIA_GATEWAYE_STATE__FOIP,
	MEDIA_GATEWAYE_STATE__TOIP
    } Tac49xMediaGatewayState;

typedef enum             
    {
	INITIATING_FLAG__MEDIA_GATEWAY_STATE_IS_INITIATED_BY_REMOTE_MEDIA_GATEWAY,
	INITIATING_FLAG__MEDIA_GATEWAY_STATE_IS_INITIATED_BY_AC49X
    } Tac49xInitiatingFlag;
             
typedef enum             
    {
	REASON_IDENTIFIER_CODE__NULL,
	REASON_IDENTIFIER_CODE__V8_CM,
	REASON_IDENTIFIER_CODE__V8_JM,
	REASON_IDENTIFIER_CODE__V32_AA_OR_TONE_1800_HZ,
	REASON_IDENTIFIER_CODE__V32_AC,
	REASON_IDENTIFIER_CODE__V22_USB1_OR_TONE_2250_HZ,
	REASON_IDENTIFIER_CODE__V22_SB1,
	REASON_IDENTIFIER_CODE__V22_BIS_S1,
	REASON_IDENTIFIER_CODE__V21_CHANNEL_2 ,
	REASON_IDENTIFIER_CODE__V21_CHANNEL_1,
	REASON_IDENTIFIER_CODE__V23_FORWARD_CHANNEL_OR_TONE_1300_HZ,
	REASON_IDENTIFIER_CODE__V23_LOW_CHANNEL,
	REASON_IDENTIFIER_CODE__TONE_2225_HZ_BELL_103_ANSWER_TONE,
	REASON_IDENTIFIER_CODE__V21_CHANNEL_2_HDLC_FLAGS,
	REASON_IDENTIFIER_CODE__INDETERMINATE_SIGNAL,
	REASON_IDENTIFIER_CODE__SILENCE,
	REASON_IDENTIFIER_CODE__CNG_CALLING_FAX_TONE_1100_HZ,
	REASON_IDENTIFIER_CODE__VOICE,
	REASON_IDENTIFIER_CODE__TIME_OUT,
	REASON_IDENTIFIER_CODE__P_STATE_TRANSITION,
	REASON_IDENTIFIER_CODE__CLEARDOWN_PHYSICAL_LAYER_DISCONNECTION,
	REASON_IDENTIFIER_CODE__ANS_SLASH_CED_MODEM_ANSWER_SLASH_CALLED_FAX_TONE_2100_HZ,
	REASON_IDENTIFIER_CODE__ANS_AM,
	REASON_IDENTIFIER_CODE__REVERSALS_ANS,
	REASON_IDENTIFIER_CODE__REVERSALS_ANS_AM,
	REASON_IDENTIFIER_CODE__V8_BIS_CRE_OR_INITIATING_SEGMENT_1_DUAL_TONE_1375_HZ_AND_2002_HZ = 29,
	REASON_IDENTIFIER_CODE__V8_BIS_CRDR_OR_RESPONDING_SEGMENT_1_DUAL_TONE_TONE_1529_HZ_AND_2225_HZ,
	REASON_IDENTIFIER_CODE__BAUDOT_45_bps,
	REASON_IDENTIFIER_CODE__BAUDOT_50_bps,
	REASON_IDENTIFIER_CODE__EDT,
	REASON_IDENTIFIER_CODE__BELL_103_MODEM,
	REASON_IDENTIFIER_CODE__V21_TEXT_T_50,
	REASON_IDENTIFIER_CODE__V23_TEXT_MINITEL,
	REASON_IDENTIFIER_CODE__V18_TEXT_T_140
    } Tac49xReasonIdentifierCode;

typedef enum             
    {
    JITTER_BUFFER_STATUS_REPORT_MODE__SEND_IF_OVERRUN_OR_UNDERRUN_ONLY,
    JITTER_BUFFER_STATUS_REPORT_MODE__SEND_EVERY_ERROR_REPORT_INTERVAL
    } Tac49xJitterBufferStatusReportMode;

typedef enum 
    {
    MEDIA_CHANNEL_TYPE__REGULAR,
    MEDIA_CHANNEL_TYPE__MEDIATED,
    MEDIA_CHANNEL_TYPE__CONFERENCE_1,
    MEDIA_CHANNEL_TYPE__CONFERENCE_2,
    MEDIA_CHANNEL_TYPE__T38
    }Tac49xMediaChannelType;

typedef enum
	{
	PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_RECEIVE_RTCP_STATUS_OR_EVENT,
	PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_RECEIVE_RTCP_STATUS_OR_EVENT,
	PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_3GPP_USER_PLANE_STATE_EVENT,
	PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_3GPP_USER_PLANE_ERROR_EVENT,
	PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_3GPP_USER_PLANE_STATISTIC_STATUS,
	PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_3GPP_USER_PLANE_SELECT_RATE_EVENT,
	PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_PACKETIZER_ERROR_EVENT,
	PACKETIZER_STATUS_OR_EVENT_INDEX__RFC3267_AMR_SELECT_RATE_EVENT,
	PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_TRANSMIT_RTCP_STATUS_OR_EVENT,
	PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_TRANSMIT_RTCP_STATUS_OR_EVENT,
	PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_3GPP_USER_PLANE_STATE_EVENT,
	PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_3GPP_USER_PLANE_ERROR_EVENT,
	PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_3GPP_USER_PLANE_STATISTIC_STATUS,
	PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_3GPP_USER_PLANE_SELECT_RATE_EVENT,
	PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_PACKETIZER_ERROR_EVENT,
	PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_BROKEN_CONNECTION_EVENT,
	PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_BROKEN_CONNECTION_EVENT,
	PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_CONNECTION_ESTABLISH_EVENT,
	PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_CONNECTION_ESTABLISH_EVENT,
	PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_REMOTE_SSRC_CHANGED_EVENT,
	PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_REMOTE_SSRC_CHANGED_EVENT,
	PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_MEDIA_PROTECTION_INFO_EVENT,
	PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_MEDIA_PROTECTION_INFO_EVENT,
	PACKETIZER_STATUS_OR_EVENT_INDEX__RFC3558_RATE_MODE_EVENT,
	PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_RTCP_APP_EVENT,
	PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_RTCP_APP_EVENT,
	PACKETIZER_STATUS_OR_EVENT_INDEX__G729EV_RATE_EVENT,
	PACKETIZER_STATUS_OR_EVENT_INDEX__HANDOUT_EVENT,
	PACKETIZER_STATUS_OR_EVENT_INDEX__3GPP_IUB_STATUS,
	PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_SENDER_RTCP_XR_VOICE_QUALITY_STATISTICS,
	PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_SENDER_RTCP_XR_VOICE_QUALITY_STATISTICS,
	PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_RECEIVER_RTCP_XR_VOICE_QUALITY_STATISTICS,
	PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_RECEIVER_RTCP_XR_VOICE_QUALITY_STATISTICS,
	PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_RTCP_XR_MARKOV_MODEL_STATISTICS,
	PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_RTCP_XR_MARKOV_MODEL_STATISTICS,
	PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_RTCP_XR_ALERT_STATUS,
	PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_RTCP_XR_ALERT_STATUS,
	PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_RTP_SEQUENCE_NUMBER_RECEIVED,
	PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_RTP_SEQUENCE_NUMBER_RECEIVED,
	PACKETIZER_STATUS_OR_EVENT_INDEX__SWITCH_TO_MODEM,
	PACKETIZER_STATUS_OR_EVENT_INDEX__MODEM_RELAY_SSE,
	PACKETIZER_STATUS_OR_EVENT_INDEX__OPUS_STATUS,
	PACKETIZER_STATUS_OR_EVENT_INDEX__TRANSMIT_RTCP_FEEDBACK,
	PACKETIZER_STATUS_OR_EVENT_INDEX__RECEIVE_RTCP_FEEDBACK,
	} Tac49xPacketizerStatusOrEventIndex;

typedef enum
	{
	RTCP_STATUS_OR_EVENT_REPORT_TYPE__RR_PACKET,
	RTCP_STATUS_OR_EVENT_REPORT_TYPE__SR_PACKET,
	RTCP_STATUS_OR_EVENT_REPORT_TYPE__PACKETIZER_STATUS_PACKET_SEND_DUE_TO_GET_RTCP_COMMAND,
	RTCP_STATUS_OR_EVENT_REPORT_TYPE__PACKETIZER_STATUS_PACKET_SEND_DUE_TO_CLOSE_RTP_COMMAND,
	RTCP_STATUS_OR_EVENT_REPORT_TYPE__PACKETIZER_STATUS_PACKET_SEND_DUE_TO_RX_SSRC_CHANGE,
	RTCP_STATUS_OR_EVENT_REPORT_TYPE__PACKETIZER_STATUS_PACKET_SEND_DUE_TO_TX_SSRC_CHANGE
	} Tac49xPacketizerStatusOrEvent__RtcpStatusOrEventReportType;
/*
typedef enum
	{
	RTCP_STATUS_OR_EVENT_REPORT_TYPE__RR_RECEIVE_PACKET,
	RTCP_STATUS_OR_EVENT_REPORT_TYPE__SR_RECEIVE_PACKET,
	RTCP_STATUS_OR_EVENT_REPORT_TYPE__RR_TRANSMIT_PACKET,
	RTCP_STATUS_OR_EVENT_REPORT_TYPE__SR_TRANSMIT_PACKET,
	RTCP_STATUS_OR_EVENT_REPORT_TYPE__PACKETIZER_STATUS_PACKET_SEND_DUE_TO_GET_RTCP_COMMAND
	} Tac49xPacketizerStatusOrEvent__RtcpStatusOrEventReportType;
*/

typedef enum
    {
    PACKETIZER_ERROR_EVENT_INDEX__PACKETIZER_UNEXPECTED_PROTOCOL					= 1,	
	PACKETIZER_ERROR_EVENT_INDEX__PACKETIZER_INVALID_COMMAND_OPCODE,						
	PACKETIZER_ERROR_EVENT_INDEX__PACKETIZER_RECEIVED_INVALID_PACKET_SIZE,	
	PACKETIZER_ERROR_EVENT_INDEX__PACKETIZER_INVALID_ERROR,
	PACKETIZER_ERROR_EVENT_INDEX__VP_RTP_TRANSMIT_REDUNDENT_PAYLOAD_SIZE_EXCEEDED   = 100,	
	PACKETIZER_ERROR_EVENT_INDEX__VP_RTP_RECEIVE_WRONG_PAYLOAD_TYPE,						
	PACKETIZER_ERROR_EVENT_INDEX__VP_RTP_RECEIVE_WRONG_PAYLOAD_SIZE					= 103,		
	PACKETIZER_ERROR_EVENT_INDEX__VP_RTP_RECEIVE_INTERLEAVING_TIMESTAMP,					
	PACKETIZER_ERROR_EVENT_INDEX__VP_RTP_RECEIVE_INVALID_REDUNDENCY_PACKET			= 106,		
	PACKETIZER_ERROR_EVENT_INDEX__VP_RTP_RECEIVE_INVALID_G723_RATE,							
	PACKETIZER_ERROR_EVENT_INDEX__VP_RTP_RECEIVE_INVALID_GSM_EFR_PACKET				= 110,
	PACKETIZER_ERROR_EVENT_INDEX__VP_RTP_RECEIVE_INVALID_GSM_FR_PACKET,
	PACKETIZER_ERROR_EVENT_INDEX__VP_RTP_TRANSMIT_RFC_3389_SID_PAYLOAD_SIZE_EXCEEDED,
	PACKETIZER_ERROR_EVENT_INDEX__VP_RTP_TRANSMIT_INVALID_NOB_REDUNDANCY_CONFIG,
	PACKETIZER_ERROR_EVENT_INDEX__VP_RTP_TRANSMIT_RECEIVED_ON_SENDER_ONLY_CHANNEL,
	PACKETIZER_ERROR_EVENT_INDEX__VP_RTP_RECEIVE_INVALID_PACKET_FORWARDING_CONFIGURATION,
	PACKETIZER_ERROR_EVENT_INDEX__VP_RTP_RECEIVE_INVALID_DECODER_CONFIGURATION,
	PACKETIZER_ERROR_EVENT_INDEX__VP_RTP_RECEIVE_INVALID_ENCODER_CONFIGURATION,
	PACKETIZER_ERROR_EVENT_INDEX__VP_RTP_RECEIVE_SBC_TRANSRATING_OVERFLOW,
    PACKETIZER_ERROR_EVENT_INDEX__VP_RTP_TRANSMIT_INVALID_RTP_TRANSMISSION_BYPASS_NOB_AND_BYPASS_REDUNDANCY_CONFIGURATION,
	PACKETIZER_ERROR_EVENT_INDEX__VP_RTCP_RECEIVE_INVALID_PACKET					= 120,	
	PACKETIZER_ERROR_EVENT_INDEX__VP_RTCP_RECEIVE_INVALID_SDES_PACKET,						
	PACKETIZER_ERROR_EVENT_INDEX__VP_RTCP_COMMAND_INVALID_CANONICAL_NAME_LENGTH,			
	PACKETIZER_ERROR_EVENT_INDEX__VP_RTCP_COMMAND_INVALID_SET_PARAM_INDEX,					
	PACKETIZER_ERROR_EVENT_INDEX__VP_RTCP_RECEIVED_WITH_WRONG_SSRC,			
	PACKETIZER_ERROR_EVENT_INDEX__VP_RTCP_INVALID_BYE_REASON_LENGTH,
	PACKETIZER_ERROR_EVENT_INDEX__VP_RTCP_TRANSMIT_INVALID_NTP_TIME,
	PACKETIZER_ERROR_EVENT_INDEX__VP_RFC2833_RECEIVE_WRONG_EVENT				    = 130,	
	PACKETIZER_ERROR_EVENT_INDEX__VP_RFC2833_DTMF_TRANSMIT_INVALID_REDUNDENCY_DEPTH,
	PACKETIZER_ERROR_EVENT_INDEX__VP_RFC2833_DTMF_RECEIVE_INVALID_REDUNDENCY_DEPTH,
	PACKETIZER_ERROR_EVENT_INDEX__VP_RFC2833_DTMF_RECEIVE_INVALID_CC_VALUE			= 133,
	PACKETIZER_ERROR_EVENT_INDEX__VP_RFC2833_DTMF_TRANSMIT_INVALID_EVENT,
	PACKETIZER_ERROR_EVENT_INDEX__VP_RTP_FEC_INVALID_PARAMETERS,
	PACKETIZER_ERROR_EVENT_INDEX__V_150_1_SSE_RECEIVED_WRONG_OR_UNSUPPORTED_EVENT   = 140,
	PACKETIZER_ERROR_EVENT_INDEX__RFC3558_COMMAND_INIT_INVALID_LOCAL_RATE_MODE      = 150,	
	PACKETIZER_ERROR_EVENT_INDEX__RFC3558_COMMAND_INIT_INVALID_FAR_END_RATE_MODE_REQUEST,	
	PACKETIZER_ERROR_EVENT_INDEX__RFC3558_COMMAND_INIT_INVALID_MIN_OR_MAX_RATE,	
	PACKETIZER_ERROR_EVENT_INDEX__RFC3558_TRANSMIT_INVALID_PAYLOAD_SIZE,	
	PACKETIZER_ERROR_EVENT_INDEX__RFC3558_RECEIVE_INVALID_TOC,	
	PACKETIZER_ERROR_EVENT_INDEX__RFC3558_RECEIVE_INVALID_PAIR_NNN_AND_LLL,	
	PACKETIZER_ERROR_EVENT_INDEX__RECEIVED_INVALID_RTP_PACKET						= 160,
	PACKETIZER_ERROR_EVENT_INDEX__VA_COMMAND_INVALID_PROTOCOL                       = 200,	
	PACKETIZER_ERROR_EVENT_INDEX__VA_AAL2_RECEIVE_WRONG_CID,								
	PACKETIZER_ERROR_EVENT_INDEX__VA_UNEXPECTED_PROTOCOL,									
	PACKETIZER_ERROR_EVENT_INDEX__3GPP_UP_UNEXPECTED_PROTOCOL						= 300,	
	PACKETIZER_ERROR_EVENT_INDEX__3GPP_UP_UNKNOWN_STATE,									
	PACKETIZER_ERROR_EVENT_INDEX__3GPP_UP_INIT_COMMAND__INVALID_LOCAL_RATE,					
	PACKETIZER_ERROR_EVENT_INDEX__3GPP_UP_RATE_COMMAND__INVALID_LOCAL_RATE,					
	PACKETIZER_ERROR_EVENT_INDEX__3GPP_UP_RATE_COMMAND__SLAVE_RATE_CONTROL_NOT_SUPPORTED_BY_VERSION,
	PACKETIZER_ERROR_EVENT_INDEX__3GPP_UP_INIT_COMMAND__INVALID_REMOTE_RATE,				
	PACKETIZER_ERROR_EVENT_INDEX__3GPP_UP_RATE_COMMAND__INVALID_REMOTE_RATE,				
	PACKETIZER_ERROR_EVENT_INDEX__3GPP_UP_TRANSMIT_WRONG_VOICE_PAYLOAD_SIZE,				
	PACKETIZER_ERROR_EVENT_INDEX__RFC3267_OCTET_ALIGNED_VALUE_OVERRUN				=400,	
	PACKETIZER_ERROR_EVENT_INDEX__RFC3267_RECEIVE_WRONG_CMR_VALUE                   =403,							
	PACKETIZER_ERROR_EVENT_INDEX__RFC3267_RECEIVE_WRONG_FT_VALUE,
	PACKETIZER_ERROR_EVENT_INDEX__RFC3267_ILLEGAL_CODER_MODE,
	PACKETIZER_ERROR_EVENT_INDEX__RFC3267_INIT_REDUNDANCY_CONFLICT_WITH_RFC_2198,
	PACKETIZER_ERROR_EVENT_INDEX__RFC3267_RATE_CONTROL_REDUNDANCY_CONFLICT_WITH_RFC_2198,
	PACKETIZER_ERROR_EVENT_INDEX__RFC3267_TOO_MUCH_FRAMES_PER_PACKET,
	PACKETIZER_ERROR_EVENT_INDEX__G729EV_COMMAND_ILLEGAL_REMOTE_MAX_RATE            =420,
	PACKETIZER_ERROR_EVENT_INDEX__G729EV_COMMAND_ILLEGAL_LOCAL_RATE,
	PACKETIZER_ERROR_EVENT_INDEX__G729EV_RECEIVE_ILLEGAL_MBS,
	PACKETIZER_ERROR_EVENT_INDEX__G729EV_RECEIVE_ILLEGAL_FT_VALUE,
	PACKETIZER_ERROR_EVENT_INDEX__SPEEX_ILLEGAL_RATE								=430,
	PACKETIZER_ERROR_EVENT_INDEX__AAL1_TRANSMIT_NOB_NOT_SUPPORTED					=500,	
	PACKETIZER_ERROR_EVENT_INDEX__AAL2_I_366_2_COMMAND_INVALID_PROFILE_GROUP		=600,	
	PACKETIZER_ERROR_EVENT_INDEX__AAL2_I_366_2_COMMAND_INVALID_PROFILE_ID,					
	PACKETIZER_ERROR_EVENT_INDEX__AAL2_I_366_2_COMMAND_INVALID_PROFILE_ENTRY,				
	PACKETIZER_ERROR_EVENT_INDEX__AAL2_I_366_2_TRANSMIT_WRONG_PAYLOAD_SIZE,					
	PACKETIZER_ERROR_EVENT_INDEX__AAL2_I_366_2_RECEIVE_WRONG_UUI,
	PACKETIZER_ERROR_EVENT_INDEX__MEDIA_PROTECTION_INVALID_MODE						=700,
	PACKETIZER_ERROR_EVENT_INDEX__PACKET_CABLE_PROTECTION_ENCRYPTION_AND_AUTHETICATION_BOTH_DISABLED,
	PACKETIZER_ERROR_EVENT_INDEX__PACKET_CABLE_PROTECTION_WRONG_RTP_ENCRYPTION_KEY_SIZE,
	PACKETIZER_ERROR_EVENT_INDEX__PACKET_CABLE_PROTECTION_WRONG_RTP_AUTHENTICATION_KEY_SIZE,
	PACKETIZER_ERROR_EVENT_INDEX__PACKET_CABLE_PROTECTION_WRONG_RTP_DIGEST_SIZE,
	PACKETIZER_ERROR_EVENT_INDEX__PACKET_CABLE_PROTECTION_INVALID_RTP_AUTHENTICATION_STANDARD,
	PACKETIZER_ERROR_EVENT_INDEX__PACKET_CABLE_PROTECTION_WRONG_RTCP_ENCRYPTION_KEY_SIZE,
	PACKETIZER_ERROR_EVENT_INDEX__PACKET_CABLE_PROTECTION_WRONG_RTCP_AUTHENTICATION_KEY_SIZE,
	PACKETIZER_ERROR_EVENT_INDEX__PACKET_CABLE_PROTECTION_WRONG_RTCP_DIGEST_SIZE,
	PACKETIZER_ERROR_EVENT_INDEX__PACKET_CABLE_PROTECTION_INVALID_RTCP_AUTHENTICATION_STANDARD,
	PACKETIZER_ERROR_EVENT_INDEX__SRTP_PROTECTION_ENCRYPTION_AND_AUTHENTICATION_BOTH_DISABLED=720,
	PACKETIZER_ERROR_EVENT_INDEX__SRTP_PROTECTION_WRONG_RTP_ENCRYPTION_KEY_SIZE,
	PACKETIZER_ERROR_EVENT_INDEX__SRTP_PROTECTION_WRONG_RTP_AUTHENTICATION_KEY_SIZE,
	PACKETIZER_ERROR_EVENT_INDEX__SRTP_PROTECTION_WRONG_RTP_DIGEST_SIZE,
	PACKETIZER_ERROR_EVENT_INDEX__SRTP_PROTECTION_INVALID_RTP_AUTHENTICATION_STANDARD,
	PACKETIZER_ERROR_EVENT_INDEX__SRTP_PROTECTION_WRONG_RTCP_ENCRYPTION_KEY_SIZE,
	PACKETIZER_ERROR_EVENT_INDEX__SRTP_PROTECTION_WRONG_RTCP_AUTHENTICATION_KEY_SIZE,
	PACKETIZER_ERROR_EVENT_INDEX__SRTP_PROTECTION_WRONG_RTCP_DIGEST_SIZE,
	PACKETIZER_ERROR_EVENT_INDEX__SRTP_PROTECTION_INVALID_RTCP_AUTHENTICATION_STANDARD,
	PACKETIZER_ERROR_EVENT_INDEX__SRTP_PROTECTION_WRONG_MASTER_KEY_SIZE,
	PACKETIZER_ERROR_EVENT_INDEX__SRTP_PROTECTION_WRONG_MASTER_SALT_SIZE,
	PACKETIZER_ERROR_EVENT_INDEX__SRTP_PROTECTION_WRONG_SESSION_SALT_SIZE,
	PACKETIZER_ERROR_EVENT_INDEX__SRTP_PROTECTION_WRONG_KEY_DERIVATION_RATE,
	PACKETIZER_ERROR_EVENT_INDEX__SRTP_PROTECTION_WRONG_RTP_ENCRYPTION_STANDARD,
	PACKETIZER_ERROR_EVENT_INDEX__SRTP_PROTECTION_WRONG_RTP_ENCRYPTION_STANDARD_AND_KEY_SIZE_COMBINATION,
	PACKETIZER_ERROR_EVENT_INDEX__SRTP_PROTECTION_WRONG_RTCP_ENCRYPTION_STANDARD,
	PACKETIZER_ERROR_EVENT_INDEX__SRTP_PROTECTION_WRONG_RTCP_ENCRYPTION_STANDARD_AND_KEY_SIZE_COMBINATION,
	PACKETIZER_ERROR_EVENT_INDEX__SRTP_PROTECTION_WRONG_MKI_SIZE_ON_SCALED_SRTP_INIT,
	PACKETIZER_ERROR_EVENT_INDEX__UEM_CLIP_INVALID_TX_MODE=905,
	PACKETIZER_ERROR_EVENT_INDEX__UEM_CLIP_INVALID_TX_TIMESTAMP_RESOLUTION,
	PACKETIZER_ERROR_EVENT_INDEX__UEM_CLIP_INVALID_RX_PACKET_SIZE=910,
	PACKETIZER_ERROR_EVENT_INDEX__UEM_CLIP_INVALID_RX_TIMESTAMP_RESOLUTION,
	PACKETIZER_ERROR_EVENT_INDEX__G711_WB_INVALID_TX_MODE=915,
	PACKETIZER_ERROR_EVENT_INDEX__G711_WB_INVALID_TX_TIMESTAMP_RESOLUTION,
	PACKETIZER_ERROR_EVENT_INDEX__G711_WB_INVALID_RX_PACKET_SIZE=920,
	PACKETIZER_ERROR_EVENT_INDEX__G711_WB_INVALID_RX_TIMESTAMP_RESOLUTION
    } Tac49xPacketizerStatusOrEvent__PacketizerErrorEventIndex;

typedef enum
	{
	_3GPP_USER_PLANE_STATE_EVENT__PROTOCOL_STATE__INIT=1,
	_3GPP_USER_PLANE_STATE_EVENT__PROTOCOL_STATE__MASTER_WAIT_TO_INIT,
	_3GPP_USER_PLANE_STATE_EVENT__PROTOCOL_STATE__RUN
	} Tac49xPacketizerStatusOrEvent__3GppUserPlaneStateEvent__ProtocolStateIndex;

typedef enum
    {
    _3GPP_USER_PLANE_ERROR_EVENT_OR_NACK_INDEX__CRC_ERROR_OF_FRAME_HEADER,
    _3GPP_USER_PLANE_ERROR_EVENT_OR_NACK_INDEX__CRC_ERROR_OF_FRAME_PAYLOAD,
    _3GPP_USER_PLANE_ERROR_EVENT_OR_NACK_INDEX__UNEXPECTED_FRAME_NUMBER,
    _3GPP_USER_PLANE_ERROR_EVENT_OR_NACK_INDEX__FRAME_LOSS,
    _3GPP_USER_PLANE_ERROR_EVENT_OR_NACK_INDEX__PDU_TYPE_UNKNOWN,
    _3GPP_USER_PLANE_ERROR_EVENT_OR_NACK_INDEX__UNKNOWN_PRECEDURE,
    _3GPP_USER_PLANE_ERROR_EVENT_OR_NACK_INDEX__UNKNOWN_RESERVED_VALUE,
    _3GPP_USER_PLANE_ERROR_EVENT_OR_NACK_INDEX__UNKNOWN_FIELD,
    _3GPP_USER_PLANE_ERROR_EVENT_OR_NACK_INDEX__FRAME_TOO_SHORT,
    _3GPP_USER_PLANE_ERROR_EVENT_OR_NACK_INDEX__MISSING_FIELD,
    _3GPP_USER_PLANE_ERROR_EVENT_OR_NACK_INDEX__UNEXPECTED_PDU_TYPE       = 16,
    _3GPP_USER_PLANE_ERROR_EVENT_OR_NACK_INDEX__UNEXPECTED_PRECEDURE      = 18,
    _3GPP_USER_PLANE_ERROR_EVENT_OR_NACK_INDEX__UNEXPECTED_RFCI,
    _3GPP_USER_PLANE_ERROR_EVENT_OR_NACK_INDEX__UNEXPECTED_VALUE,
    _3GPP_USER_PLANE_ERROR_EVENT_OR_NACK_INDEX__INITIALIZATION_FAILURE    = 42,
    _3GPP_USER_PLANE_ERROR_EVENT_OR_NACK_INDEX__TIMER_INITIALIZATION_FAILURE,
    _3GPP_USER_PLANE_ERROR_EVENT_OR_NACK_INDEX__NACK_INITIALIZATION_FAILURE,
    _3GPP_USER_PLANE_ERROR_EVENT_OR_NACK_INDEX__RATE_CONTROL_FAILURE,
    _3GPP_USER_PLANE_ERROR_EVENT_OR_NACK_INDEX__ERROR_EVENT_FAILURE,
    _3GPP_USER_PLANE_ERROR_EVENT_OR_NACK_INDEX__TIME_ALIGNMENT_NOT_SUPPORTED,
    _3GPP_USER_PLANE_ERROR_EVENT_OR_NACK_INDEX__TIME_ALIGNMENT_REQUEST_NOT_SUPPORTED,
    _3GPP_USER_PLANE_ERROR_EVENT_OR_NACK_INDEX__USER_PLANE_MODE_NOT_SUPPORTED
    } Tac49xPacketizerStatusOrEvent__3GppUserPlaneErrorEventOrNackIndex;

typedef enum
	{
    _3GPP_USER_PLANE_ERROR_EVENT_OR_NACK_CAUSE__ERROR,
    _3GPP_USER_PLANE_ERROR_EVENT_OR_NACK_CAUSE__NEGATIVE_ACKNOWLEDGEMENT
    } Tac49xPacketizerStatusOrEvent__3GppUserPlaneErrorEventOrNackCause;
                                
typedef enum
	{
	_3GPP_USER_PLANE_SELECT_RATE_EVENT__SENT_OR_RECIEVED_RATE_CONTROL__RECIEVED,
	_3GPP_USER_PLANE_SELECT_RATE_EVENT__SENT_OR_RECIEVED_RATE_CONTROL__SEND
	} Tac49xPacketizerStatusOrEvent__3GppUserPlaneSelectRateEvent__SentOrReceivedRateControl;

typedef enum 
	{
    TRUNK_TESTING_MEASUREMENT_EVENT_INDEX__ENERGY_MEASUREMENT,
    TRUNK_TESTING_MEASUREMENT_EVENT_INDEX__FSK_SIGNAL_DETECTION,
    TRUNK_TESTING_MEASUREMENT_EVENT_INDEX__UNKNOWN_TONE_MEASUREMENT,
    TRUNK_TESTING_MEASUREMENT_EVENT_INDEX__ERROR=15
    } Tac49xTrunkTestingMeasurementEventIndex;

typedef enum 
	{
    TRUNK_TESTING_MEASUREMENT_ERROR_INDEX__NONE,
    TRUNK_TESTING_MEASUREMENT_ERROR_INDEX__SIGNAL_IS_TOO_SHORT_FOR_THE_MEASUREMENT,
    TRUNK_TESTING_MEASUREMENT_ERROR_INDEX__ILLEGAL_MEASUREMENT_OPERATION_WHEN_THE_TRUNK_TESTING_COMMAND_IS_ILLEGAL,
    TRUNK_TESTING_MEASUREMENT_ERROR_INDEX__UNKNOWN_MEASUREMENT_STATE,    /* can occur if module isn't initialized, or statics are being damaged,*/
    TRUNK_TESTING_MEASUREMENT_ERROR_INDEX__ILLEGAL_FSK_SIGNAL            /* occurs if the module doesn't recognize the 1200 ,2200, 1200 signal in the right sequence and times. ,*/
    } Tac49xTrunkTestingMeasurementErrorIndex;

/*
typedef enum
    {
    DEBUG_INFORMATION_PACKET_NUMBER_OF_TX_PACKET_WITHIN_ERROR_REPORT_INTERVAL,
    DEBUG_INFORMATION_PACKET_DEVICE_REAL_TIME_ERROR,
    DEBUG_INFORMATION_CORE_USAGE,
    DEBUG_INFORMATION_NUMBER_OF_DTMF_ERASURE_DETECTIONS_WITHIN_ERROR_REPORT_INTERVAL,
    } Tac49xDebugInformationPacketIndex;
*/
typedef enum
    {
    UTOPIA_CELL_SIZE__53_BYTES,
    UTOPIA_CELL_SIZE__54_BYTES,
    UTOPIA_CELL_SIZE__55_BYTES,
    UTOPIA_CELL_SIZE__56_BYTES,
    UTOPIA_CELL_SIZE__57_BYTES,
    UTOPIA_CELL_SIZE__58_BYTES,
    UTOPIA_CELL_SIZE__59_BYTES,
    UTOPIA_CELL_SIZE__60_BYTES,
    UTOPIA_CELL_SIZE__61_BYTES,
    UTOPIA_CELL_SIZE__62_BYTES,
    UTOPIA_CELL_SIZE__63_BYTES,
    UTOPIA_CELL_SIZE__64_BYTES
    } Tac49xUtopiaCellSize;

typedef enum
    {
    PCM_SERIAL_PORT_RX_TX_DATA_DELAY__0_Bit,
    PCM_SERIAL_PORT_RX_TX_DATA_DELAY__1_Bit,
    PCM_SERIAL_PORT_RX_TX_DATA_DELAY__2_Bit
    } Tac49xPcmSerialPortRxTxDataDelay;

typedef enum
	{
	SERIAL_PORT_SELECTION__VOICE_0_CAS_1,
	SERIAL_PORT_SELECTION__VOICE_1_CAS_0,
	SERIAL_PORT_SELECTION__VOICE_0_CAS_0,
	SERIAL_PORT_SELECTION__VOICE_1_CAS_1
	} Tac49xSerialPortSelection;

typedef enum
	{
	SERIAL_PORT_SELECTION_FORMAT1__PORT_0,
	SERIAL_PORT_SELECTION_FORMAT1__PORT_1
	} Tac49xSerialPortSelection_Format1;



typedef enum
    {
    DTMF_GENERATION_TWIST_DIRECTION__HIGH_FREQUENCY_LEVEL_IS_HIGHER_THAN_LOW_FREQUENCY_LEVEL,
    DTMF_GENERATION_TWIST_DIRECTION__HIGH_FREQUENCY_LEVEL_IS_LOWER_THEN_LOW_FREQUENCY_LEVEL
    } Tac49xDtmfGenerationTwistDirection;

typedef enum
    {
    DTMF_GENERATION_TWIST__DISABLED,
    DTMF_GENERATION_TWIST__1_dB,
    DTMF_GENERATION_TWIST__2_dB,
    DTMF_GENERATION_TWIST__3_dB,
    DTMF_GENERATION_TWIST__4_dB,
    DTMF_GENERATION_TWIST__5_dB,
    DTMF_GENERATION_TWIST__6_dB,
    DTMF_GENERATION_TWIST__7_dB,
    DTMF_GENERATION_TWIST__8_dB,
    DTMF_GENERATION_TWIST__9_dB,
    DTMF_GENERATION_TWIST__10_dB
    } Tac49xDtmfGenerationTwist;

typedef enum
	{
	DTMF_GAP__25_msec,
	DTMF_GAP__35_msec,
	DTMF_GAP__15_msec,
	DTMF_GAP__45_msec
	} Tac49xDtmfGap;

typedef enum 
{
    AGC_MIN_GAIN__0_DB,
    AGC_MIN_GAIN__minus1_DB,
    AGC_MIN_GAIN__minus2_DB,
    AGC_MIN_GAIN__minus3_DB,
    AGC_MIN_GAIN__minus4_DB,
    AGC_MIN_GAIN__minus5_DB,
    AGC_MIN_GAIN__minus6_DB,
    AGC_MIN_GAIN__minus7_DB,
    AGC_MIN_GAIN__minus8_DB,
    AGC_MIN_GAIN__minus9_DB,
    AGC_MIN_GAIN__minus10_DB,
    AGC_MIN_GAIN__minus11_DB,
    AGC_MIN_GAIN__minus12_DB,
    AGC_MIN_GAIN__minus13_DB,
    AGC_MIN_GAIN__minus14_DB,
    AGC_MIN_GAIN__minus15_DB,
    AGC_MIN_GAIN__minus16_DB,
    AGC_MIN_GAIN__minus17_DB,
    AGC_MIN_GAIN__minus18_DB,
    AGC_MIN_GAIN__minus19_DB,
    AGC_MIN_GAIN__minus20_DB,
    AGC_MIN_GAIN__minus21_DB,
    AGC_MIN_GAIN__minus22_DB,
    AGC_MIN_GAIN__minus23_DB,
    AGC_MIN_GAIN__minus24_DB,
    AGC_MIN_GAIN__minus25_DB,
    AGC_MIN_GAIN__minus26_DB,
    AGC_MIN_GAIN__minus27_DB,
    AGC_MIN_GAIN__minus28_DB,
    AGC_MIN_GAIN__minus29_DB,
    AGC_MIN_GAIN__minus30_DB,
    AGC_MIN_GAIN__minus31_DB
} Tac49xAgcMinGain;
typedef enum 
{
    AGC_MAX_GAIN__0_DB,
    AGC_MAX_GAIN__1_DB,
    AGC_MAX_GAIN__2_DB,
    AGC_MAX_GAIN__3_DB,
    AGC_MAX_GAIN__4_DB,
    AGC_MAX_GAIN__5_DB,
    AGC_MAX_GAIN__6_DB,
    AGC_MAX_GAIN__7_DB,
    AGC_MAX_GAIN__8_DB,
    AGC_MAX_GAIN__9_DB,
    AGC_MAX_GAIN__10_DB,
    AGC_MAX_GAIN__11_DB,
    AGC_MAX_GAIN__12_DB,
    AGC_MAX_GAIN__13_DB,
    AGC_MAX_GAIN__14_DB,
    AGC_MAX_GAIN__15_DB,
    AGC_MAX_GAIN__16_DB,
    AGC_MAX_GAIN__17_DB,
    AGC_MAX_GAIN__18_DB
} Tac49xAgcMaxGain;

typedef enum 
{
    AGC_GAIN_SLOPE__0_25_DB_SEC,  /*00*/
    AGC_GAIN_SLOPE__0_50_DB_SEC,  /*01*/
    AGC_GAIN_SLOPE__0_75_DB_SEC,  /*02*/
    AGC_GAIN_SLOPE__1_00_DB_SEC,  /*03*/
    AGC_GAIN_SLOPE__1_25_DB_SEC,  /*04*/
    AGC_GAIN_SLOPE__1_50_DB_SEC,  /*05*/
    AGC_GAIN_SLOPE__1_75_DB_SEC,  /*06*/
    AGC_GAIN_SLOPE__2_00_DB_SEC,  /*07*/
    AGC_GAIN_SLOPE__2_50_DB_SEC,  /*08*/
    AGC_GAIN_SLOPE__3_00_DB_SEC,  /*09*/
    AGC_GAIN_SLOPE__3_50_DB_SEC,  /*10*/
    AGC_GAIN_SLOPE__4_00_DB_SEC,  /*11*/
    AGC_GAIN_SLOPE__4_50_DB_SEC,  /*12*/
    AGC_GAIN_SLOPE__5_00_DB_SEC,  /*13*/
    AGC_GAIN_SLOPE__5_50_DB_SEC,  /*14*/
    AGC_GAIN_SLOPE__6_00_DB_SEC,  /*15*/
    AGC_GAIN_SLOPE__7_00_DB_SEC,  /*16*/
    AGC_GAIN_SLOPE__8_00_DB_SEC,  /*17*/
    AGC_GAIN_SLOPE__9_00_DB_SEC,  /*18*/
    AGC_GAIN_SLOPE__10_00_DB_SEC, /*19*/
    AGC_GAIN_SLOPE__11_00_DB_SEC, /*20*/
    AGC_GAIN_SLOPE__12_00_DB_SEC, /*21*/
    AGC_GAIN_SLOPE__13_00_DB_SEC, /*22*/
    AGC_GAIN_SLOPE__14_00_DB_SEC, /*23*/
    AGC_GAIN_SLOPE__15_00_DB_SEC, /*24*/
    AGC_GAIN_SLOPE__20_00_DB_SEC, /*25*/
    AGC_GAIN_SLOPE__25_00_DB_SEC, /*26*/
    AGC_GAIN_SLOPE__30_00_DB_SEC, /*27*/
    AGC_GAIN_SLOPE__35_00_DB_SEC, /*28*/
    AGC_GAIN_SLOPE__40_00_DB_SEC, /*29*/
    AGC_GAIN_SLOPE__50_00_DB_SEC, /*30*/
    AGC_GAIN_SLOPE__70_00_DB_SEC  /*31*/
} Tac49xAgcGainSlope;

typedef enum
	{
	AGC_LOCATION__AT_ENCODER_INPUT,
	AGC_LOCATION__AT_DECODER_OUTPUT
	} Tac49xAgcLocation;

typedef enum
	{
    AGC_TARGET_ENERGY__0_DBM,
    AGC_TARGET_ENERGY__minus1_DBM,
    AGC_TARGET_ENERGY__minus2_DBM,
    AGC_TARGET_ENERGY__minus3_DBM,
    AGC_TARGET_ENERGY__minus4_DBM,
    AGC_TARGET_ENERGY__minus5_DBM,
    AGC_TARGET_ENERGY__minus6_DBM,
    AGC_TARGET_ENERGY__minus7_DBM,
    AGC_TARGET_ENERGY__minus8_DBM,
    AGC_TARGET_ENERGY__minus9_DBM,
    AGC_TARGET_ENERGY__minus10_DBM,
    AGC_TARGET_ENERGY__minus11_DBM,
    AGC_TARGET_ENERGY__minus12_DBM,
    AGC_TARGET_ENERGY__minus13_DBM,
    AGC_TARGET_ENERGY__minus14_DBM,
    AGC_TARGET_ENERGY__minus15_DBM,
    AGC_TARGET_ENERGY__minus16_DBM,
    AGC_TARGET_ENERGY__minus17_DBM,
    AGC_TARGET_ENERGY__minus18_DBM,
    AGC_TARGET_ENERGY__minus19_DBM,
    AGC_TARGET_ENERGY__minus20_DBM,
    AGC_TARGET_ENERGY__minus21_DBM,
    AGC_TARGET_ENERGY__minus22_DBM,
    AGC_TARGET_ENERGY__minus23_DBM,
    AGC_TARGET_ENERGY__minus24_DBM,
    AGC_TARGET_ENERGY__minus25_DBM,
    AGC_TARGET_ENERGY__minus26_DBM,
    AGC_TARGET_ENERGY__minus27_DBM,
    AGC_TARGET_ENERGY__minus28_DBM,
    AGC_TARGET_ENERGY__minus29_DBM,
    AGC_TARGET_ENERGY__minus30_DBM,
    AGC_TARGET_ENERGY__minus31_DBM
	} Tac49xAgcTargetEnergy;

     /**
      * @n Refer to <A HREF="Tac49xBusSpeed.html">Bus speed support</A> for the supported bus speeds 
      * in the different products.
      */

typedef enum
    {
    BUS_SPEED__2MHZ,    /*!< 2.048 MHz (32 slots)   */
    BUS_SPEED__4MHZ=2,  /*!< 4.096 MHz (64 slots)   */
    BUS_SPEED__8MHZ,    /*!< 8.192 MHz (128 slots)  */
    BUS_SPEED__16MHZ,   /*!< 16.384 MHz (256 slots) */
    BUS_SPEED__32MHZ    /*!< 32.768 MHz (512 slots) */
    } Tac49xBusSpeed;

typedef enum
	{
	CODEC_ENABLE__DISABLE,
	CODEC_ENABLE__FOR_CHANNEL_1,
	CODEC_ENABLE__FOR_CHANNEL_1_AND_2
	} Tac49xCodecEnable;

typedef enum
	{
	PCM_CLK_AND_FRAME_SYNC_SOURCE__EXTERNAL,
	PCM_CLK_AND_FRAME_SYNC_SOURCE__INTERNAL
	}Tac49xPcmClkAndFrameSyncSource;

typedef enum 
    {
    CHANNEL_TYPE__VOICE_FAX,  /*!< RTP/ATM/T.38 Mode*/
    CHANNEL_TYPE__HDLC,       /*!< Channel is opened as HDLC framer*/
    CHANNEL_TYPE__SS7,        /*!< Reserved for internal use*/
    CHANNEL_TYPE__PCM_TO_PCM, /*!< Reserved for internal use*/
    CHANNEL_TYPE__DUA,        /*!< Reserved for internal use*/
    CHANNEL_TYPE__SUMMATION,  /*!< Summation of the type IP = IP + TDM*/
	CHANNEL_TYPE__ADPCM_TO_PCM /*!< ADPCM to PCM over TDM */
    }Tac49xChannelType;

typedef enum 
    {
	CODER__CHANNEL_DISABLED	, /*!< DTMF and CAS signaling only are transferred in RTP packets to the network.*/
	CODER__TRANSPARENT		, 
	CODER__NO_CODER			,  /*!< Reserved for internal use*/
	CODER__G711ALAW  		, 
    CODER__G711MULAW		, 
    CODER__G726_16			, 
    CODER__G726_24			, 
    CODER__G726_32			, 
    CODER__G726_40			, 
    CODER__G727_16			, 
    CODER__G727_24_16		, /*10*/
    CODER__G727_24			, 
    CODER__G727_32_16		, 
    CODER__G727_32_24		, 
    CODER__G727_32			, 
    CODER__G727_40_16		, 
    CODER__G727_40_24		, 
    CODER__G727_40_32		, 
	CODER__G728_16	        ,
	CODER__LINEAR_16_BIT_PCM,
	CODER__NETCODER_6_4		, /*20*/
	CODER__NETCODER_7_2		, 
	CODER__NETCODER_8		, 
	CODER__NETCODER_8_8		, 
	CODER__NETCODER_9_6		, 
    CODER__G723LOW			, 
    CODER__G723HIGH			, 
    CODER__G729			    , 
    CODER__GSM_FULL_RATE_13_2, 
    CODER__MICROSOFT_GSM_13_2,
    CODER__EVRC_8_8K_0_8_K   , /*30 Enhanced Variable Rate Coder (IS-127)*/
    CODER__QCELP_8           ,
    CODER__QCELP_13          ,
/*	CODER__AMR_4_75K		=36,*/
/*	CODER__AMR_5_15K		,	*/
/*	CODER__AMR_5_9K			,	*/
/*	CODER__AMR_6_7K			,	*/
/*	CODER__AMR_7_4K			,	*/
/*	CODER__AMR_7_95K		,	*/
/*	CODER__AMR_10_2K		,	*/
/*	CODER__AMR_12_2K        ,	*/
	CODER__LINEAR_8_BIT_PCM = 47  ,  
	CODER__G729E_11_8K      ,
    CODER__GSM_EFR          ,/*49*/
	CODER__ILBC_15_2K = 51	,
	CODER__ILBC_13_33K,
	CODER__LINEAR_16_BIT_PCM_SAMPLE_RATE_16KHZ,
	CODER__G722_48K,
	CODER__G722_56K,
	CODER__G722_64K,
	CODER__EVRC_B = 66,
    CODER__G729_EV,
	CODER__EG711_A_LAW,
	CODER__EG711_MU_LAW=72,
	CODER__G711_WB_ALOW=76,
	CODER__G711_WB_MULOW,
	CODER__EVRC_WB,
	CODER__UEM_CLIP,
	CODER__G711_1_ALAW,
	CODER__G711_1_MULAW,
	CODER__MICROSOFT_RTA_NB,
	CODER__MICROSOFT_RTA_WB,
	CODER__TETRA,
	CODER__SILK_NB,
	CODER__SILK_WB,
	CODER__SPEEX_NB,
	CODER__SPEEX_WB,
	CODER__OPUS_NB,
	CODER__OPUS_WB,
	CODER__MAX_CODER = CODER__OPUS_WB
    } Tac49xCoder;

typedef enum 
    {
	RTCP_XR_EVENT_CODER__CHANNEL_DISABLED	, /*!< DTMF and CAS signaling only are transferred in RTP packets to the network.*/
	RTCP_XR_EVENT_CODER__TRANSPARENT		, 
	RTCP_XR_EVENT_CODER__NO_CODER			,  /*!< Reserved for internal use*/
	RTCP_XR_EVENT_CODER__G711ALAW  		, 
    RTCP_XR_EVENT_CODER__G711MULAW		, 
    RTCP_XR_EVENT_CODER__G726_16			, 
    RTCP_XR_EVENT_CODER__G726_24			, 
    RTCP_XR_EVENT_CODER__G726_32			, 
    RTCP_XR_EVENT_CODER__G726_40			, 
    RTCP_XR_EVENT_CODER__G727_16			, 
    RTCP_XR_EVENT_CODER__G727_24_16		, /*10*/
    RTCP_XR_EVENT_CODER__G727_24			, 
    RTCP_XR_EVENT_CODER__G727_32_16		, 
    RTCP_XR_EVENT_CODER__G727_32_24		, 
    RTCP_XR_EVENT_CODER__G727_32			, 
    RTCP_XR_EVENT_CODER__G727_40_16		, 
    RTCP_XR_EVENT_CODER__G727_40_24		, 
    RTCP_XR_EVENT_CODER__G727_40_32		, 
	RTCP_XR_EVENT_CODER__G728_16	        ,
	RTCP_XR_EVENT_CODER__LINEAR_16_BIT_PCM,
	RTCP_XR_EVENT_CODER__NETCODER_6_4		, /*20*/
	RTCP_XR_EVENT_CODER__NETCODER_7_2		, 
	RTCP_XR_EVENT_CODER__NETCODER_8		, 
	RTCP_XR_EVENT_CODER__NETCODER_8_8		, 
	RTCP_XR_EVENT_CODER__NETCODER_9_6		, 
    RTCP_XR_EVENT_CODER__G723LOW			, 
    RTCP_XR_EVENT_CODER__G723HIGH			, 
    RTCP_XR_EVENT_CODER__G729			    , 
    RTCP_XR_EVENT_CODER__GSM_FULL_RATE_13_2, 
    RTCP_XR_EVENT_CODER__MICROSOFT_GSM_13_2,
    RTCP_XR_EVENT_CODER__EVRC_8_8K_0_8_K   , /*30 Enhanced Variable Rate Coder (IS-127)*/
    RTCP_XR_EVENT_CODER__QCELP_8           ,
    RTCP_XR_EVENT_CODER__QCELP_13          ,
	RTCP_XR_EVENT_CODER__AMR_4_75K		=36,
	RTCP_XR_EVENT_CODER__AMR_5_15K		,	
	RTCP_XR_EVENT_CODER__AMR_5_9K			,	
	RTCP_XR_EVENT_CODER__AMR_6_7K			,	
	RTCP_XR_EVENT_CODER__AMR_7_4K			,	
	RTCP_XR_EVENT_CODER__AMR_7_95K		,	
	RTCP_XR_EVENT_CODER__AMR_10_2K		,	
	RTCP_XR_EVENT_CODER__AMR_12_2K        ,	
	RTCP_XR_EVENT_CODER__LINEAR_8_BIT_PCM = 47  ,  
	RTCP_XR_EVENT_CODER__G729E_11_8K      ,
    RTCP_XR_EVENT_CODER__GSM_EFR          ,/*49*/
	RTCP_XR_EVENT_CODER__ILBC_15_2K = 51	,
	RTCP_XR_EVENT_CODER__ILBC_13_33K,
	RTCP_XR_EVENT_CODER__LINEAR_16_BIT_PCM_SAMPLE_RATE_16KHZ,
	RTCP_XR_EVENT_CODER__G722_48K,
	RTCP_XR_EVENT_CODER__G722_56K,
	RTCP_XR_EVENT_CODER__G722_64K,
	RTCP_XR_EVENT_CODER__AMR_WB_6_60,
	RTCP_XR_EVENT_CODER__AMR_WB_8_85,
	RTCP_XR_EVENT_CODER__AMR_WB_12_65,
	RTCP_XR_EVENT_CODER__AMR_WB_14_25,
	RTCP_XR_EVENT_CODER__AMR_WB_15_85,
	RTCP_XR_EVENT_CODER__AMR_WB_18_25,
	RTCP_XR_EVENT_CODER__AMR_WB_19_85,
	RTCP_XR_EVENT_CODER__AMR_WB_23_05,
	RTCP_XR_EVENT_CODER__AMR_WB_23_85,
	RTCP_XR_EVENT_CODER__EVRC_B = 66,
    RTCP_XR_EVENT_CODER__G729_EV,
	RTCP_XR_EVENT_CODER__EG711_A_LAW,
	RTCP_XR_EVENT_CODER__EG711_MU_LAW=72,
	RTCP_XR_EVENT_CODER__G711_WB_ALOW=76,
	RTCP_XR_EVENT_CODER__G711_WB_MULOW,
	RTCP_XR_EVENT_CODER__EVRC_WB,
	RTCP_XR_EVENT_CODER__UEM_CLIP,
	RTCP_XR_EVENT_CODER__G711_1_ALAW,
	RTCP_XR_EVENT_CODER__G711_1_MULAW,
	RTCP_XR_EVENT_CODER__MICROSOFT_RTA_NB,
	RTCP_XR_EVENT_CODER__MICROSOFT_RTA_WB,
	RTCP_XR_EVENT_CODER__TETRA,
	RTCP_XR_EVENT_CODER__SILK_NB,
	RTCP_XR_EVENT_CODER__SILK_WB,
	RTCP_XR_EVENT_CODER__SPEEX_NB,
	RTCP_XR_EVENT_CODER__SPEEX_WB,
	RTCP_XR_EVENT_CODER__OPUS_NB,
	RTCP_XR_EVENT_CODER__OPUS_WB
    } Tac49xRtcpXrEventCoder;

#define CODER__LINEAR_PCM CODER__LINEAR_8_BIT_PCM

typedef enum 
    {
	G711_CODER_TYPE__A_LAW,
	G711_CODER_TYPE__MU_LAW,
	G711_CODER_TYPE__TRANSPARENT
    } Tac49xG711CoderType;

typedef enum 
    {
	LAW_SELECT__A_LAW,
	LAW_SELECT__MU_LAW,
    LAW_SELECT__LINEAR_16Bits,
	LAW_SELECT__ADPCM
    } Tac49xLawSelect;

typedef enum 
    {
	CAS_MODE__NO_SIGNALING,  /*!< Signaling Disable*/
	CAS_MODE__A,             /*!< 1-bit CAS*/
	CAS_MODE__AB,            /*!< 2-bit CAS*/
	CAS_MODE__ABCD           /*!< 4-bit CAS*/
    } Tac49xCasMode;

typedef enum 
    {
    CAS_DEBOUNCE__5MS, 
    CAS_DEBOUNCE__10MS,
    CAS_DEBOUNCE__15MS,
    CAS_DEBOUNCE__20MS,
    CAS_DEBOUNCE__25MS,
    CAS_DEBOUNCE__30MS,
    CAS_DEBOUNCE__35MS,
    CAS_DEBOUNCE__40MS,
    CAS_DEBOUNCE__45MS,
    CAS_DEBOUNCE__50MS
    } Tac49xCasDebounce;

typedef enum 
    {
    ECHO_CANCELER_NON_LINEAR_PROCESSOR_MODE__DISABLE,
    ECHO_CANCELER_NON_LINEAR_PROCESSOR_MODE__LOW_SENSITIVITY,
    ECHO_CANCELER_NON_LINEAR_PROCESSOR_MODE__MEDIUM_SENSITIVITY,
    ECHO_CANCELER_NON_LINEAR_PROCESSOR_MODE__HIGH_SENSITIVITY
    } Tac49xEchoCancelerNonLinearProcessor;

typedef enum 
    {
    SILENCE_COMPRESSION_MODE__DISABLE,
    SILENCE_COMPRESSION_MODE__ENABLE_NOISE_ADAPTATION_ENABLE,
    SILENCE_COMPRESSION_MODE__ENABLE_NOISE_ADAPTATION_DISABLE
    } Tac49xSilenceCompressionMode;

typedef enum 
    {
    ECHO_CANCELER_MODE__DISABLE,
    ECHO_CANCELER_MODE__ENABLE,
    ECHO_CANCELER_MODE__FREESE,
    ECHO_CANCELER_MODE__CLEAR
    } Tac49xEchoCancelerMode;

typedef enum 
    {
    PCM_SOURCE__FROM_SERIAL_PORT,
    PCM_SOURCE__FROM_NETWORK
    } Tac49xPcmSource;

typedef enum 
    {
    VOICE_GAIN__MUTE,
    VOICE_GAIN__minus31_DB,
    VOICE_GAIN__minus30_DB,
    VOICE_GAIN__minus29_DB,
    VOICE_GAIN__minus28_DB,
    VOICE_GAIN__minus27_DB,
    VOICE_GAIN__minus26_DB,
    VOICE_GAIN__minus25_DB,
    VOICE_GAIN__minus24_DB,
    VOICE_GAIN__minus23_DB,
    VOICE_GAIN__minus22_DB,
    VOICE_GAIN__minus21_DB,
    VOICE_GAIN__minus20_DB,
    VOICE_GAIN__minus19_DB,
    VOICE_GAIN__minus18_DB,
    VOICE_GAIN__minus17_DB,
    VOICE_GAIN__minus16_DB,
    VOICE_GAIN__minus15_DB,
    VOICE_GAIN__minus14_DB,
    VOICE_GAIN__minus13_DB,
    VOICE_GAIN__minus12_DB,
    VOICE_GAIN__minus11_DB,
    VOICE_GAIN__minus10_DB,
    VOICE_GAIN__minus9_DB,
    VOICE_GAIN__minus8_DB,
    VOICE_GAIN__minus7_DB,
    VOICE_GAIN__minus6_DB,
    VOICE_GAIN__minus5_DB,
    VOICE_GAIN__minus4_DB,
    VOICE_GAIN__minus3_DB,
    VOICE_GAIN__minus2_DB,
    VOICE_GAIN__minus1_DB,
    VOICE_GAIN__0_DB,
    VOICE_GAIN__plus1_DB,
    VOICE_GAIN__plus2_DB,
    VOICE_GAIN__plus3_DB,
    VOICE_GAIN__plus4_DB,
    VOICE_GAIN__plus5_DB,
    VOICE_GAIN__plus6_DB,
    VOICE_GAIN__plus7_DB,
    VOICE_GAIN__plus8_DB,
    VOICE_GAIN__plus9_DB,
    VOICE_GAIN__plus10_DB,
    VOICE_GAIN__plus11_DB,
    VOICE_GAIN__plus12_DB,
    VOICE_GAIN__plus13_DB,
    VOICE_GAIN__plus14_DB,
    VOICE_GAIN__plus15_DB,
    VOICE_GAIN__plus16_DB,
    VOICE_GAIN__plus17_DB,
    VOICE_GAIN__plus18_DB,
    VOICE_GAIN__plus19_DB,
    VOICE_GAIN__plus20_DB,
    VOICE_GAIN__plus21_DB,
    VOICE_GAIN__plus22_DB,
    VOICE_GAIN__plus23_DB,
    VOICE_GAIN__plus24_DB,
    VOICE_GAIN__plus25_DB,
    VOICE_GAIN__plus26_DB,
    VOICE_GAIN__plus27_DB,
    VOICE_GAIN__plus28_DB,
    VOICE_GAIN__plus29_DB,
    VOICE_GAIN__plus30_DB,
    VOICE_GAIN__plus31_DB
    } Tac49xVoiceGain;

typedef enum 
    {
    DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR__0_NONE,
    DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR__1,
    DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR__2,
    DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR__3,
    DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR__4,
    DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR__5,
    DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR__6,
    DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR__7,
    DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR__8,
    DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR__9,
    DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR__10,
    DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR__11,
    DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR__12_MAX,
    DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR__STATIC_DELAY_FOR_BYPASS_MODE,
	DYNAMIC_JITTER_BUFFER_OPTIMIZATION_FACTOR__14
    } Tac49xDynamicJitterBufferOptimizationFactor;

typedef enum 
    {
    IBS_TRANSFER_MODE__RELAY_DISABLE_VOICE_MUTE,
    IBS_TRANSFER_MODE__RELAY_ENABLE_VOICE_DISABLE,
    IBS_TRANSFER_MODE__TRANSPARENT_THROUGH_VOICE,
    IBS_TRANSFER_MODE__RELAY_ENABLE_VOICE_MUTE
    } Tac49xIbsTransferMode;
/*
typedef enum 
    {
    LBS_LEVEL__minus31_DBM,
    LBS_LEVEL__minus30_DBM,
    LBS_LEVEL__minus29_DBM,
    LBS_LEVEL__minus28_DBM,
    LBS_LEVEL__minus27_DBM,
    LBS_LEVEL__minus26_DBM,
    LBS_LEVEL__minus25_DBM,
    LBS_LEVEL__minus24_DBM,
    LBS_LEVEL__minus23_DBM,
    LBS_LEVEL__minus22_DBM,
    LBS_LEVEL__minus21_DBM,
    LBS_LEVEL__minus20_DBM,
    LBS_LEVEL__minus19_DBM,
    LBS_LEVEL__minus18_DBM,
    LBS_LEVEL__minus17_DBM,
    LBS_LEVEL__minus16_DBM,
    LBS_LEVEL__minus15_DBM,
    LBS_LEVEL__minus14_DBM,
    LBS_LEVEL__minus13_DBM,
    LBS_LEVEL__minus12_DBM,
    LBS_LEVEL__minus11_DBM,
    LBS_LEVEL__minus10_DBM,
    LBS_LEVEL__minus9_DBM,
    LBS_LEVEL__minus8_DBM,
    LBS_LEVEL__minus7_DBM,
    LBS_LEVEL__minus6_DBM,
    LBS_LEVEL__minus5_DBM,
    LBS_LEVEL__minus4_DBM,
    LBS_LEVEL__minus3_DBM,
    LBS_LEVEL__minus2_DBM,
    LBS_LEVEL__minus1_DBM,
    LBS_LEVEL__0_DBM,
    } Tac49xIbsLevel;
*/
typedef enum 
    {
    MAX_RATE__2_4_KBPS,
    MAX_RATE__4_8_KBPS,
    MAX_RATE__7_2_KBPS,
    MAX_RATE__9_6_KBPS,
    MAX_RATE__12_KBPS,
    MAX_RATE__14_4_KBPS
    } Tac49xMaxRate;

typedef enum 
    {
	T38_MAX_RATE__TAKEN_FROM_OPEN_OR_UPDATE_CHANNEL_COMMAND,
    T38_MAX_RATE__2_4_KBPS,
    T38_MAX_RATE__4_8_KBPS,
    T38_MAX_RATE__7_2_KBPS,
    T38_MAX_RATE__9_6_KBPS,
    T38_MAX_RATE__12_KBPS,
    T38_MAX_RATE__14_4_KBPS,
	T38_MAX_RATE__16_8_KBPS,
	T38_MAX_RATE__19_2_KBPS,
	T38_MAX_RATE__21_6_KBPS,
	T38_MAX_RATE__24_0_KBPS,
	T38_MAX_RATE__26_4_KBPS,
	T38_MAX_RATE__28_8_KBPS,
	T38_MAX_RATE__31_2_KBPS,
	T38_MAX_RATE__33_6_KBPS
    } Tac49xT38MaxRate;

typedef enum 
    {
    FAX_BIT_RATE__300_BPS,
    FAX_BIT_RATE__2400_BPS,
    FAX_BIT_RATE__4800_BPS,
    FAX_BIT_RATE__7200_BPS,
    FAX_BIT_RATE__9600_BPS,
    FAX_BIT_RATE__12000_BPS,
    FAX_BIT_RATE__14400_BPS,
	FAX_BIT_RATE__16800_BPS,
	FAX_BIT_RATE__19200_BPS,
	FAX_BIT_RATE__21600_BPS,
	FAX_BIT_RATE__24000_BPS,
	FAX_BIT_RATE__26400_BPS,
	FAX_BIT_RATE__28800_BPS,
	FAX_BIT_RATE__31200_BPS,
	FAX_BIT_RATE__33600_BPS
    } Tac49xFaxBitRate;

typedef enum 
    {
    TEST_MODE__NONE,                   /*!< No Test*/
    TEST_MODE__1KHZ_TO_ENCODER,        /*!< 1 kHz Test Tone Injection to the encoder input*/
    TEST_MODE__ALL_CHANNELS_LOOP_BACK  /*!< PCM loop. (This mode affects all the channels in the device)*/
    } Tac49xTestMode;

typedef enum 
    {
    ECHO_CANCELER_LENGTH__8_MSEC,
    ECHO_CANCELER_LENGTH__16_MSEC,
    ECHO_CANCELER_LENGTH__24_MSEC,
    ECHO_CANCELER_LENGTH__32_MSEC,
    ECHO_CANCELER_LENGTH__40_MSEC,
    ECHO_CANCELER_LENGTH__48_MSEC,
    ECHO_CANCELER_LENGTH__56_MSEC,
    ECHO_CANCELER_LENGTH__64_MSEC,
    ECHO_CANCELER_LENGTH__72_MSEC,
    ECHO_CANCELER_LENGTH__80_MSEC,
    ECHO_CANCELER_LENGTH__88_MSEC,
    ECHO_CANCELER_LENGTH__96_MSEC,
    ECHO_CANCELER_LENGTH__104_MSEC,
    ECHO_CANCELER_LENGTH__112_MSEC,
    ECHO_CANCELER_LENGTH__120_MSEC,
    ECHO_CANCELER_LENGTH__128_MSEC
    } Tac49xEchoCancelerLength;

typedef enum /*Not used for ver 105 and up*/
    {
    ECHO_CANCELER_THRESHOLD__minus33_DBM,
    ECHO_CANCELER_THRESHOLD__minus35_DBM,
    ECHO_CANCELER_THRESHOLD__minus37_DBM,
    ECHO_CANCELER_THRESHOLD__minus39_DBM,
    ECHO_CANCELER_THRESHOLD__minus41_DBM,
    ECHO_CANCELER_THRESHOLD__minus43_DBM,
    ECHO_CANCELER_THRESHOLD__minus45_DBM,
    ECHO_CANCELER_THRESHOLD__minus47_DBM,
    ECHO_CANCELER_THRESHOLD__minus49_DBM,
    ECHO_CANCELER_THRESHOLD__minus51_DBM,
    ECHO_CANCELER_THRESHOLD__minus53_DBM,
    ECHO_CANCELER_THRESHOLD__minus55_DBM,
    ECHO_CANCELER_THRESHOLD__minus57_DBM,
    ECHO_CANCELER_THRESHOLD__minus59_DBM,
    ECHO_CANCELER_THRESHOLD__minus61_DBM,
    ECHO_CANCELER_THRESHOLD__minus63_DBM
    } Tac49xEchoCancelerThreshold;

typedef enum 
    {
    ECHO_CANCELER_NLP_THRESHOLD__0_DBM,
    ECHO_CANCELER_NLP_THRESHOLD__minus30_DBM=30,
    ECHO_CANCELER_NLP_THRESHOLD__minus31_DBM,
    ECHO_CANCELER_NLP_THRESHOLD__minus32_DBM,
    ECHO_CANCELER_NLP_THRESHOLD__minus33_DBM,
    ECHO_CANCELER_NLP_THRESHOLD__minus34_DBM,
    ECHO_CANCELER_NLP_THRESHOLD__minus35_DBM,
    ECHO_CANCELER_NLP_THRESHOLD__minus36_DBM,
    ECHO_CANCELER_NLP_THRESHOLD__minus37_DBM,
    ECHO_CANCELER_NLP_THRESHOLD__minus38_DBM,
    ECHO_CANCELER_NLP_THRESHOLD__minus39_DBM,
    ECHO_CANCELER_NLP_THRESHOLD__minus40_DBM,
    ECHO_CANCELER_NLP_THRESHOLD__minus41_DBM,
    ECHO_CANCELER_NLP_THRESHOLD__minus42_DBM,
    ECHO_CANCELER_NLP_THRESHOLD__minus43_DBM,
    ECHO_CANCELER_NLP_THRESHOLD__minus44_DBM,
    ECHO_CANCELER_NLP_THRESHOLD__minus45_DBM,
    ECHO_CANCELER_NLP_THRESHOLD__minus46_DBM,
    ECHO_CANCELER_NLP_THRESHOLD__minus47_DBM,
    ECHO_CANCELER_NLP_THRESHOLD__minus48_DBM,
    ECHO_CANCELER_NLP_THRESHOLD__minus49_DBM,
    ECHO_CANCELER_NLP_THRESHOLD__minus50_DBM,
    ECHO_CANCELER_NLP_THRESHOLD__minus51_DBM,
    ECHO_CANCELER_NLP_THRESHOLD__minus52_DBM,
    ECHO_CANCELER_NLP_THRESHOLD__minus53_DBM,
    ECHO_CANCELER_NLP_THRESHOLD__minus54_DBM,
    ECHO_CANCELER_NLP_THRESHOLD__minus55_DBM,
    ECHO_CANCELER_NLP_THRESHOLD__minus56_DBM,
    ECHO_CANCELER_NLP_THRESHOLD__minus57_DBM,
    ECHO_CANCELER_NLP_THRESHOLD__minus58_DBM,
    ECHO_CANCELER_NLP_THRESHOLD__minus59_DBM,
    ECHO_CANCELER_NLP_THRESHOLD__minus60_DBM,
    ECHO_CANCELER_NLP_THRESHOLD__minus61_DBM,
    ECHO_CANCELER_NLP_THRESHOLD__minus62_DBM,
    ECHO_CANCELER_NLP_THRESHOLD__minus63_DBM
    } Tac49xEchoCancelerNlpThreshold;

typedef enum
	{
	HYBRID_LOSS__0_DB,
	HYBRID_LOSS__3_DB,
	HYBRID_LOSS__6_DB,
	HYBRID_LOSS__9_DB
	} Tac49xHybridLoss;

typedef enum 
    {
    ACTIVE_ECHO_CANCELLATION_MAX_LENGTH__16_MSEC,
    ACTIVE_ECHO_CANCELLATION_MAX_LENGTH__20_MSEC,
    ACTIVE_ECHO_CANCELLATION_MAX_LENGTH__24_MSEC
    }Tac49xActiveEchoCancellationMaxLength;

typedef enum 
    {
    CAS_MODE_DURING_IDLE__IS_CAS_VALUE_DURING_IDLE,
    CAS_MODE_DURING_IDLE__IS_NOT_CHANGE_AFTER_CLOSE_CHANNEL
    } Tac49xCasModeDuringIdle;

typedef enum 
    {
    CONFERENCE_MODE__DISABLE,
    CONFERENCE_MODE__ENABLE_AS_TDM_CHANNEL,
    CONFERENCE_MODE__ENABLE_AS_IP_CHANNEL
    } Tac49xConferenceMode;

typedef enum 
    {
    PCM_ADPCM_FRAME_DURATION__5_MSEC	    = 40,
    PCM_ADPCM_FRAME_DURATION__5_5_MSEC      = 44,
    PCM_ADPCM_FRAME_DURATION__6_MSEC	    = 48,
    PCM_ADPCM_FRAME_DURATION__6_5_MSEC      = 52,
    PCM_ADPCM_FRAME_DURATION__7_MSEC	    = 56,
    PCM_ADPCM_FRAME_DURATION__7_5_MSEC      = 60,
    PCM_ADPCM_FRAME_DURATION__8_MSEC	    = 64,
    PCM_ADPCM_FRAME_DURATION__8_5_MSEC      = 68,
    PCM_ADPCM_FRAME_DURATION__9_MSEC	    = 72,
    PCM_ADPCM_FRAME_DURATION__9_5_MSEC      = 76,
    PCM_ADPCM_FRAME_DURATION__10_MSEC	    = 80,
    PCM_ADPCM_FRAME_DURATION__10_5_MSEC     = 84,
    PCM_ADPCM_FRAME_DURATION__11_MSEC	    = 88,
    PCM_ADPCM_FRAME_DURATION__11_5_MSEC     = 92,
    PCM_ADPCM_FRAME_DURATION__12_MSEC	    = 96,
    PCM_ADPCM_FRAME_DURATION__12_5_MSEC     = 100,
    PCM_ADPCM_FRAME_DURATION__13_MSEC	    = 104,
    PCM_ADPCM_FRAME_DURATION__13_5_MSEC     = 108,
    PCM_ADPCM_FRAME_DURATION__14_MSEC	    = 112,
    PCM_ADPCM_FRAME_DURATION__14_5_MSEC     = 116,
    PCM_ADPCM_FRAME_DURATION__15_MSEC	    = 120,
    PCM_ADPCM_FRAME_DURATION__15_5_MSEC     = 124,
    PCM_ADPCM_FRAME_DURATION__16_MSEC	    = 128,
    PCM_ADPCM_FRAME_DURATION__16_5_MSEC     = 132,
    PCM_ADPCM_FRAME_DURATION__17_MSEC	    = 136,
    PCM_ADPCM_FRAME_DURATION__17_5_MSEC     = 140,
    PCM_ADPCM_FRAME_DURATION__18_MSEC	    = 144,
    PCM_ADPCM_FRAME_DURATION__18_5_MSEC     = 148,
    PCM_ADPCM_FRAME_DURATION__19_MSEC	    = 152,
    PCM_ADPCM_FRAME_DURATION__19_5_MSEC     = 156,
    PCM_ADPCM_FRAME_DURATION__20_MSEC	    = 160,
	PCM_ADPCM_FRAME_DURATION__20_5_MSEC	    = 164,
	PCM_ADPCM_FRAME_DURATION__21_MSEC	    = 168,
	PCM_ADPCM_FRAME_DURATION__21_5_MSEC	    = 172,
	PCM_ADPCM_FRAME_DURATION__22_MSEC	    = 176,
	PCM_ADPCM_FRAME_DURATION__22_5_MSEC	    = 180,
	PCM_ADPCM_FRAME_DURATION__23_MSEC	    = 184,
	PCM_ADPCM_FRAME_DURATION__23_5_MSEC	    = 188,
	PCM_ADPCM_FRAME_DURATION__24_MSEC	    = 192,
	PCM_ADPCM_FRAME_DURATION__24_5_MSEC	    = 196,
	PCM_ADPCM_FRAME_DURATION__25_MSEC	    = 200,
	PCM_ADPCM_FRAME_DURATION__25_5_MSEC	    = 204,
	PCM_ADPCM_FRAME_DURATION__26_MSEC	    = 208,
	PCM_ADPCM_FRAME_DURATION__26_5_MSEC	    = 212,
	PCM_ADPCM_FRAME_DURATION__27_MSEC	    = 216,
	PCM_ADPCM_FRAME_DURATION__27_5_MSEC	    = 220,
	PCM_ADPCM_FRAME_DURATION__28_MSEC	    = 224,
	PCM_ADPCM_FRAME_DURATION__28_5_MSEC	    = 228,
	PCM_ADPCM_FRAME_DURATION__29_MSEC	    = 232,
	PCM_ADPCM_FRAME_DURATION__29_5_MSEC	    = 236,
	PCM_ADPCM_FRAME_DURATION__30_MSEC	    = 240
    } Tac49xPcmAdpcmFrameDuration ;

typedef enum 
    {
    FAX_AND_CALLER_ID_LEVEL__minus_21_DBM,
    FAX_AND_CALLER_ID_LEVEL__minus_20_DBM,
    FAX_AND_CALLER_ID_LEVEL__minus_19_DBM,
    FAX_AND_CALLER_ID_LEVEL__minus_18_DBM,
    FAX_AND_CALLER_ID_LEVEL__minus_17_DBM,
    FAX_AND_CALLER_ID_LEVEL__minus_16_DBM,
    FAX_AND_CALLER_ID_LEVEL__minus_15_DBM,
    FAX_AND_CALLER_ID_LEVEL__minus_14_DBM,
    FAX_AND_CALLER_ID_LEVEL__minus_13_DBM,
    FAX_AND_CALLER_ID_LEVEL__minus_12_DBM,
    FAX_AND_CALLER_ID_LEVEL__minus_11_DBM,
    FAX_AND_CALLER_ID_LEVEL__minus_10_DBM,
    FAX_AND_CALLER_ID_LEVEL__minus_9_DBM,
    FAX_AND_CALLER_ID_LEVEL__minus_8_DBM,
    FAX_AND_CALLER_ID_LEVEL__minus_7_DBM,
    FAX_AND_CALLER_ID_LEVEL__minus_6_DBM
    } Tac49xFaxAndCallerIdLevel;

typedef enum 
    {
	TTY_TRANSFER_METHOD__DISABLE,
	TTY_TRANSFER_METHOD__RELAY,
	TTY_TRANSFER_METHOD__BYPASS,
	TTY_TRANSFER_METHOD__IN_BAND_RELAY
    } Tac49xTtyTransferMethod;

typedef enum 
    {
	FAX_MODEM_TRANSFER_METHOD__DISABLE,
	FAX_MODEM_TRANSFER_METHOD__RELAY,
	FAX_MODEM_TRANSFER_METHOD__BYPASS
    } Tac49xFaxModemTransferMethod;

typedef enum 
    {
    CALLER_ID_STANDARD__TELCORDIA_BELLCORE,
    CALLER_ID_STANDARD__ETSI,
    CALLER_ID_STANDARD__NTT,
    CALLER_ID_STANDARD__DTMF_CLIP_ETSI=8,
    CALLER_ID_STANDARD__DTMF_CLIP_DANISH,
    CALLER_ID_STANDARD__DTMF_CLIP_INDIAN,
    CALLER_ID_STANDARD__DTMF_CLIP_BRAZILIAN,
    CALLER_ID_STANDARD__DTMF_CLIP_NTT_DID 
    } Tac49xCallerIdStandard;

typedef enum
    {
	CALLER_ID_PARAMETER_TYPE__DTMF_END_CHARACTER = -1,
	CALLER_ID_PARAMETER_TYPE__NUMBER = 2,
	CALLER_ID_PARAMETER_TYPE__NUMBER_ABSENCE = 4,
    CALLER_ID_PARAMETER_TYPE__VMWI_STATUS = 11,
    CALLER_ID_PARAMETER_TYPE__CALLING_USER = 22,
	CALLER_ID_PARAMETER_TYPE__REDIRECTING_NUMBER = 26
    } Tac49xCallerIdParameterType;

typedef enum
    {
    DTMF_DIGIT__0,
    DTMF_DIGIT__1,
    DTMF_DIGIT__2,
    DTMF_DIGIT__3,
    DTMF_DIGIT__4,
    DTMF_DIGIT__5,
    DTMF_DIGIT__6,
    DTMF_DIGIT__7,
    DTMF_DIGIT__8,
    DTMF_DIGIT__9,
    DTMF_DIGIT__ASTERISK,
    DTMF_DIGIT__POUND,
    DTMF_DIGIT__A,
    DTMF_DIGIT__B,
    DTMF_DIGIT__C,
    DTMF_DIGIT__D
    } Tac49xDtmfDigit;

typedef enum
    {
    ADVANCED_SIGNALS_DIGIT__FLASH,
    ADVANCED_SIGNALS_DIGIT__ONHOOK,
    ADVANCED_SIGNALS_DIGIT__OFFHOOK,
    ADVANCED_SIGNALS_DIGIT__RING
    } Tac49xAdvancedSignalsDigit;

typedef enum 
    {
    HDLC_SAMPLE_RATE__64_KBPS,
    HDLC_SAMPLE_RATE__4_KBPS,
    HDLC_SAMPLE_RATE__56_KBPS
    } Tac49xHdlcSampleRate;

typedef enum
    {
    HDLC_MININUM_END_FLAGS__1,
    HDLC_MININUM_END_FLAGS__2,
    HDLC_MININUM_END_FLAGS__3,
    HDLC_MININUM_END_FLAGS__4
    } Tac49xHdlcMinimumEndFlags;

typedef enum
    {
    HDLC_MININUM_MESSAGE_LENGTH__1_BYTE,
    HDLC_MININUM_MESSAGE_LENGTH__2_BYTE,
    HDLC_MININUM_MESSAGE_LENGTH__3_BYTE,
    HDLC_MININUM_MESSAGE_LENGTH__4_BYTE,
    HDLC_MININUM_MESSAGE_LENGTH__5_BYTE,
    HDLC_MININUM_MESSAGE_LENGTH__6_BYTE,
    HDLC_MININUM_MESSAGE_LENGTH__7_BYTE,
    HDLC_MININUM_MESSAGE_LENGTH__8_BYTE
    } Tac49xHdlcMinimumMessageLength;

typedef enum
    {
    HDLC_INTER_FRAME_FILL_PATTERN__7E,
    HDLC_INTER_FRAME_FILL_PATTERN__FF
    } Tac49xHdlcInterFrameFillPattern;

typedef enum 
    {
    AMR_ACTIVE_MODE__INACTIVE,
    AMR_ACTIVE_MODE__3GPP_UP,
    AMR_ACTIVE_MODE__RFC_3267
    }Tac49xAmrActiveMode;

typedef enum 
    {
    MEDIA_PROTECTION_MODE__DISABLE,
    MEDIA_PROTECTION_MODE__PACKET_CABLE, 
    MEDIA_PROTECTION_MODE__SRTP 
    } Tac49xMediaProtectionMode;

#define ARRAY_OF__MEDIA_PROTECTION_DIRECTION   2
typedef enum
	{
    MEDIA_PROTECTION_DIRECTION__INBOUND_RX,
    MEDIA_PROTECTION_DIRECTION__OUTBOUND_TX
    } Tac49xMediaProtectionDirection;

typedef enum
	{
    MEDIA_AUTHENTICATION_MODE__SHA1,
    MEDIA_AUTHENTICATION_MODE__MD5,
    MEDIA_AUTHENTICATION_MODE__MMH2,
    MEDIA_AUTHENTICATION_MODE__MMH4
	} Tac49xMediaAuthenticationMode;

typedef enum
	{
    MEDIA_ENCRYPTION_MODE__AES_CM,
	MEDIA_ENCRYPTION_MODE__ARIA
	} Tac49xMediaEncryptionMode;

typedef enum
	{
    IBS_REDUNDANCY_LEVEL__REPETITIONS_NONE,
	IBS_REDUNDANCY_LEVEL__REPETITIONS_1,
	IBS_REDUNDANCY_LEVEL__REPETITIONS_2,
	IBS_REDUNDANCY_LEVEL__REPETITIONS_3
	} Tac49xIbsRedundancyLevel;
                   

typedef enum       
	{              
	ATM_PROTOCOL__AAL2_I_366_2,
	ATM_PROTOCOL__3GPP_USER_PLANE,
	ATM_PROTOCOL__AAL1_I_363_1
	} Tac49xAtmProtocol;

typedef enum 
    {
    PROFILE_GROUP__ITU,
    PROFILE_GROUP__LES,
    PROFILE_GROUP__PROPRIETARY
    } Tac49xProfileGroup;

typedef enum 
    {
    ITU_PROFILE_ID__PCM_64,
    ITU_PROFILE_ID__PCM_64_AND_SILENCE
    } Tac49xItuProfileId;

typedef enum 
    {
    PROPRIETARY_PROFILE_ID__1,
    PROPRIETARY_PROFILE_ID__2
    } Tac49xProprietaryProfileId;

typedef enum 
    {
	TBD
    } Tac49xLesProfileId;

typedef enum 
    {
    ITU_PROFILE_ID_PCM_64_ENTRY__PCM_G711_64_GENERIC
    } Tac49xItuProfileId_Pcm64_Entries;

typedef enum 
    {
    ITU_PROFILE_ID_PCM_64_AND_SILENCE_ENTRY__PCM_G711_64_GENERIC,
    ITU_PROFILE_ID_PCM_64_AND_SILENCE_ENTRY__SID_GENERIC
    } Tac49xItuProfileId_Pcm64AndSilence_Entries;

typedef enum 
    {
    PROPRIETARY_PROFILE_ID_1_ENTRY__PCM_G711_64_GENERIC,
    PROPRIETARY_PROFILE_ID_1_ENTRY__ADPCM_G726_32,
    PROPRIETARY_PROFILE_ID_1_ENTRY__ADPCM_G726_24,
    PROPRIETARY_PROFILE_ID_1_ENTRY__ADPCM_G726_16,
    PROPRIETARY_PROFILE_ID_1_ENTRY__SID_GENERIC
    } Tac49xProprietaryProfileId_1_Entries;

typedef enum 
    {
    PROPRIETARY_PROFILE_ID_2_ENTRY__PCM_G711_64_GENERIC,
    PROPRIETARY_PROFILE_ID_2_ENTRY__ADPCM_G726_32,
    PROPRIETARY_PROFILE_ID_2_ENTRY__SID_GENERIC
    } Tac49xProprietaryProfileId_2_Entries;

typedef enum 
    {
    CAS_RELAY_MODE__TRANSPARENT,
    CAS_RELAY_MODE__TRANSLATED
    } Tac49xCasRelayMode;

typedef enum 
    {
    IBS_STRING_GENERATOR_REDIRECTION__INTO_DECODER_OUTPUT,
    IBS_STRING_GENERATOR_REDIRECTION__INTO_NETWORK,
    IBS_STRING_GENERATOR_REDIRECTION__INTO_ENCODER_INPUT,
    IBS_STRING_GENERATOR_REDIRECTION__INTO_DECODER_OUTPUT_AND_TO_NETWORK_CONCURRENTLY,
    IBS_STRING_GENERATOR_REDIRECTION__INTO_DECODER_OUTPUT_AND_TO_ENCODER_INPUT_CONCURRENTLY
    } Tac49xIbsStringGeneratorRedirection;
/*
typedef enum 
    {
	IBS_SYSTEM_CODE__NONE,
	IBS_SYSTEM_CODE__DTMF,
	IBS_SYSTEM_CODE__USER_DEFINED_TONES,
	IBS_SYSTEM_CODE__MF_R1_OR_SS5,
	IBS_SYSTEM_CODE__MF_R2_FORWARD,
	IBS_SYSTEM_CODE__MF_R2_BACKWARD,
	IBS_SYSTEM_CODE__LINE,
	IBS_SYSTEM_CODE__CALL_PROGRESS,
	IBS_SYSTEM_CODE__SILENCE
    } Tac49xIbsSystemCode;
*/
typedef enum 
    {
	IBS_SYSTEM_CODE__NONE,
	IBS_SYSTEM_CODE__DTMF,
	IBS_SYSTEM_CODE__SILENCE,
	IBS_SYSTEM_CODE__MF_R1_OR_SS5,
	IBS_SYSTEM_CODE__MF_R2_FORWARD,
	IBS_SYSTEM_CODE__MF_R2_BACKWARD,
	IBS_SYSTEM_CODE__CALL_PROGRESS_0_TO_15,
	IBS_SYSTEM_CODE__CALL_PROGRESS_16_TO_31,
	IBS_SYSTEM_CODE__USER_DEFINED_TONES_0_TO_15,
	IBS_SYSTEM_CODE__USER_DEFINED_TONES_16_TO_31,
	IBS_SYSTEM_CODE__ON_THE_FLY_TONES,  /*!< Generates a signal combined of User-Defined Frequency 1, 2 & 3. @n Refer to <A HREF="IBS_SYSTEM_CODE__ON_THE_FLY_TONES - On-the-Fly Digits.html">on-the-fly digits</A>.*/
	IBS_SYSTEM_CODE__FD_TEST_MODE_A,
	IBS_SYSTEM_CODE__FD_TEST_MODE_B,
	IBS_SYSTEM_CODE__ADVANCED_SIGNALS,
	IBS_SYSTEM_CODE__FAX_MODEM_SIGNALS  /*!< RFC 2833 fax/modem signals detection. @n Refer to <A HREF="FaxDataDigits.html">Fax/Modem Signal Digits</A>.*/
    } Tac49xIbsSystemCode;


typedef enum
    {
    ON_THE_FLY_DIGIT__FREQUENCY_A,
    ON_THE_FLY_DIGIT__FREQUENCY_B,
    ON_THE_FLY_DIGIT__FREQUENCY_C,
    ON_THE_FLY_DIGIT__COMBINATION_OF_A_B_FREQUENCIES,
    ON_THE_FLY_DIGIT__COMBINATION_OF_A_C_FREQUENCIES,
    ON_THE_FLY_DIGIT__COMBINATION_OF_B_C_FREQUENCIES,
    ON_THE_FLY_DIGIT__COMBINATION_OF_A_B_C_FREQUENCIES,
    ON_THE_FLY_DIGIT__FREQUENCY_A_MODULATED_BY_FREQUENCY_B,
	ON_THE_FLY_DIGIT__COMBINATION_OF_A_B_C_D_FREQUENCIES
    } Tac49xOnTheFlyDigit;

typedef enum 
    {
    PAYLOAD_TYPE_SOURCE__DEFINED_INTERNALLY_ACCORDING_THE_ENCODER,
    PAYLOAD_TYPE_SOURCE__DEFINED_EXTERNALLY
    } Tac49xPayloadTypeSource;

typedef enum
    {
    IBS_DETECTION_REDIRECTION__ON_INPUT_STREAM_ENCODER_INPUT,
    IBS_DETECTION_REDIRECTION__ON_OUTPUT_STREAM_DECODER_OUTPUT
    } Tac49xIbsDetectionRedirection;

typedef enum
	{
	RTCP_PARAMETER_INDEX__TRANSMIT_PACKET_COUNT,
	RTCP_PARAMETER_INDEX__TRANSMIT_OCTETS_COUNT,
	RTCP_PARAMETER_INDEX__RECEIVE_PACKET_COUNT,
	RTCP_PARAMETER_INDEX__RECEIVE_OCTETS_COUNT,
	RTCP_PARAMETER_INDEX__JITTER,
	RTCP_PARAMETER_INDEX__PACKET_LOSS,
	RTCP_PARAMETER_INDEX__ROUND_TRIP_DELAY
	} Tac49xRtcpParameterIndex;

typedef enum
	{
	CHANNEL_STATE__IDLE,
	CHANNEL_STATE__ACTIVE_NONE,
	CHANNEL_STATE__ACTIVE_ATM,
	CHANNEL_STATE__ACTIVE_RTP,
	CHANNEL_STATE__ACTIVE_3GPP_IUB
	} Tac49xChannelState;

typedef enum
	{
    PLAYBACK_HOST_OPCODE__TDM_START,
    PLAYBACK_HOST_OPCODE__NETWORK_START,
    PLAYBACK_HOST_OPCODE__PLAY_SILENCE,
    PLAYBACK_HOST_OPCODE__END
	} Tac49xPlaybackHostOpcode;

typedef enum
	{
    PLAYBACK_DSP_OPCODE__REQUEST
	} Tac49xPlaybackDspOpcode;

typedef enum
	{
    T38_UDP_ERROR_CORRECTION_METHOD__REDUNDANCY,
	T38_UDP_ERROR_CORRECTION_METHOD__FOREWORD_ERROR_CORRECTION
	} Tac49xT38UdpErrorCorrectionMethod;

typedef enum
	{
    T38_VERSION__0,
	T38_VERSION__1,
	T38_VERSION__2002_ASN1_SYNTAX_TPKT_ENABLED,
	T38_VERSION__V34_V33_SUPPORT_2002_ASN1_SYNTAX_TPKT_ENABLED
	} Tac49xT38Version;

typedef enum
	{
	FAX_RELAY_PROTOCOL__TAKEN_FROM_OPEN_OR_UPDATE_CHANNEL_COMMAND,
	FAX_RELAY_PROTOCOL__T38_TCP,
	FAX_RELAY_PROTOCOL__T38_UDPTL_LOCAL_TCF=3,
	FAX_RELAY_PROTOCOL__T38_UDPTL_TRANSFERRED_TCF
	} Tac49xFaxRelayProtocol;

typedef enum
	{
    IMAGE_DATA_REDUNDANCY_LEVEL__NONE,
    IMAGE_DATA_REDUNDANCY_LEVEL__1,
    IMAGE_DATA_REDUNDANCY_LEVEL__2,                         
    IMAGE_DATA_REDUNDANCY_LEVEL__3                         
	} Tac49xImageDataRedundancyLevel;

typedef enum
	{
    T30_CONTROL_DATA_REDUNDANCY_LEVEL__NONE,
    T30_CONTROL_DATA_REDUNDANCY_LEVEL__1,
    T30_CONTROL_DATA_REDUNDANCY_LEVEL__2,                         
    T30_CONTROL_DATA_REDUNDANCY_LEVEL__3,                         
    T30_CONTROL_DATA_REDUNDANCY_LEVEL__4,                         
    T30_CONTROL_DATA_REDUNDANCY_LEVEL__5,                         
    T30_CONTROL_DATA_REDUNDANCY_LEVEL__6,                         
    T30_CONTROL_DATA_REDUNDANCY_LEVEL__7
	} Tac49xT30ControlDataRedundancyLevel;

typedef enum
	{
	FAX_OR_DATA_SIGNAL_EVENT__SILENCE_OR_UNKNOWN,                       /*Undefined (Unknown signal or silence)							*/
	FAX_OR_DATA_SIGNAL_EVENT__FAX_CNG,									/*CNG-FAX (Calling fax tone, 1100 Hz)							*/
	FAX_OR_DATA_SIGNAL_EVENT__ANS_TONE_2100_FAX_CED_OR_MODEM,			/*FAX-CED or Modem-ANS (Answer tone, 2100 Hz)					*/
	FAX_OR_DATA_SIGNAL_EVENT__ANS_TONE_WITH_REVERSALS,					/*/ANS (Answer tone with reversals)								*/
	FAX_OR_DATA_SIGNAL_EVENT__ANS_TONE_AM,								/*ANSam (AM Answer tone)										*/
	FAX_OR_DATA_SIGNAL_EVENT__ANS_TONE_AM_REVERSALS,					/*/ANSam (AM Answer tone with reversals)						*/
	FAX_OR_DATA_SIGNAL_EVENT__FAX_V21_PREAMBLE_FLAGS,                   /*FAX-V.21 Preamble flags										*/
	FAX_OR_DATA_SIGNAL_EVENT__FAX_V8_JM_V34,                            /*FAX-V.8 JM (T.30 Fax)											*/
	FAX_OR_DATA_SIGNAL_EVENT__VXX_V8_JM_VXX_DATA,                       /*V.XX-V.8 JM (Data or other non-Fax / non-Textphone call)		*/
	FAX_OR_DATA_SIGNAL_EVENT__V32_AA,                                   /*V.32 AA (Calling modem tone, 1800 Hz)							*/
	FAX_OR_DATA_SIGNAL_EVENT__V22_USB1,                                 /*V.22 USB1 (V.22(bis) Unscrambled binary ones)					*/
	FAX_OR_DATA_SIGNAL_EVENT__V8_BIS_INITIATING_DUAL_TONE,              /*V.8bis Initiating dual tone (1375 Hz and 2002 Hz)				*/
	FAX_OR_DATA_SIGNAL_EVENT__V8_BIS_RESPONDING_DUAL_TONE,              /*V.8bis Responding dual tone (1529 Hz and 2225 Hz)				*/
    FAX_OR_DATA_SIGNAL_EVENT__VXX_DATA_SESSION,                         /*V.XX Data session												*/
    FAX_OR_DATA_SIGNAL_EVENT__V21_CHANNEL_2,							/*V.21 Channel 2 (Mark tone or FSK modulation at 1650/1850 Hz)	*/
    FAX_OR_DATA_SIGNAL_EVENT__V23_FORWARD_CHANNEL,						/*V.23 Forward channel (Mark tone, 1300 Hz)						*/
    FAX_OR_DATA_SIGNAL_EVENT__STU_MODEM,								/*STU Modem														*/
    FAX_OR_DATA_SIGNAL_EVENT__V25_CALLING_TONE,							/*V.25 Calling Tone (CT)										*/
    FAX_OR_DATA_SIGNAL_EVENT__V21_CHANNEL_1=18,                         /*V.21 Channel 1 (Mark tone or FSK modulation at 980/1180 Hz)	*/
    FAX_OR_DATA_SIGNAL_EVENT__BELL_103_ANSWER_TONE,                     /*Bell 103 Answer tone, 2225 Hz									*/
    FAX_OR_DATA_SIGNAL_EVENT__TTY,										/*TIA-825A FSK modulation at 1400/1800 Hz (TTY)					*/
    FAX_OR_DATA_SIGNAL_EVENT__FAX_DCN,									/*FAX-DCN (G.3 fax disconnect signal)							*/
    FAX_OR_DATA_SIGNAL_EVENT__FAX_V8_CI,								/*FAX-V.8 CI (T.30 Fax)											*/
    FAX_OR_DATA_SIGNAL_EVENT__FAX_V8_CM,								/*FAX-V.8 CM (T.30 Fax)											*/
    FAX_OR_DATA_SIGNAL_EVENT__VXX_V8_CI,								/*V.XX-V.8 CI (Data or other non-Fax / non-Textphone call)		*/
    FAX_OR_DATA_SIGNAL_EVENT__VXX_V8_CM,								/*V.XX-V.8 CM (Data or other non-Fax / non-Textphone call)		*/
    FAX_OR_DATA_SIGNAL_EVENT__TEXT_V8_CI,								/*TEXT-V.8 CI (V.18 Textphone)									*/
    FAX_OR_DATA_SIGNAL_EVENT__TEXT_V8_CM,								/*TEXT-V.8 CM (V.18 Textphone)									*/
    FAX_OR_DATA_SIGNAL_EVENT__TEXT_V8_JM,								/*TEXT-V.8 JM (V.18 Textphone)									*/
	FAX_OR_DATA_SIGNAL_EVENT__V_92_QC1A,
	FAX_OR_DATA_SIGNAL_EVENT__V_92_QCA1A,
	FAX_OR_DATA_SIGNAL_EVENT__V_92_QC1D,
	FAX_OR_DATA_SIGNAL_EVENT__V_92_QCA1D
	} Tac49xFaxOrDataSignalEvent;                       
                         
typedef enum                                            
	{                                                   
	FAX_RELAY_T30_STATE__INITIALIZATION,                
	FAX_RELAY_T30_STATE__CNG,
	FAX_RELAY_T30_STATE__CED,
	FAX_RELAY_T30_STATE__V21,
	FAX_RELAY_T30_STATE__NSF,
	FAX_RELAY_T30_STATE__NSC,
	FAX_RELAY_T30_STATE__CSI,
	FAX_RELAY_T30_STATE__CIG,
	FAX_RELAY_T30_STATE__DIS,
	FAX_RELAY_T30_STATE__DTC,
	FAX_RELAY_T30_STATE__NSS,
	FAX_RELAY_T30_STATE__TSI,
	FAX_RELAY_T30_STATE__DCS,
	FAX_RELAY_T30_STATE__CTC,
	FAX_RELAY_T30_STATE__CRP,
	FAX_RELAY_T30_STATE__DCN,
	FAX_RELAY_T30_STATE__PRE_MESSAGE_RESPONSE,
	FAX_RELAY_T30_STATE__POST_MESSAGE_RESPONSE,
	FAX_RELAY_T30_STATE__POST_MESSAGE_COMMAND,
	FAX_RELAY_T30_STATE__VXX,
	FAX_RELAY_T30_STATE__TCF,
	FAX_RELAY_T30_STATE__IMAGE,
	FAX_RELAY_T30_STATE__FAX_IMAGE_IDLE,
	FAX_RELAY_T30_STATE__V8_CM,
	FAX_RELAY_T30_STATE__V8_JM,
	FAX_RELAY_T30_STATE__V8_CI,
	FAX_RELAY_T30_STATE__V34_INFO,
	FAX_RELAY_T30_STATE__V34_TRN,
	FAX_RELAY_T30_STATE__V34_CONTROL_CHANNEL,
	FAX_RELAY_T30_STATE__FNV,
	FAX_RELAY_T30_STATE__TNR,
	FAX_RELAY_T30_STATE__TR
	} Tac49xFaxRelayT30State;

typedef enum
	{
	FAX_RELAY_BAD_PACKET_TYPE__DECODED,
	FAX_RELAY_BAD_PACKET_TYPE__ASN1
	} Tac49xFaxRelayBadPacketType;

typedef enum
	{
	FAX_PROTOCOL__T38_TCP,
    FAX_PROTOCOL__FRF11,
	FAX_PROTOCOL__T38_UDP_DATA_RATE_MANAGEMENT_1,
	FAX_PROTOCOL__T38_UDP_DATA_RATE_MANAGEMENT_2
	} Tac49xFaxProtocol;

typedef enum
	{
	CED_TRANSFER_MODE__BY_FAX_RELAY,
	CED_TRANSFER_MODE__IN_VOICE_OR_PCM_BYPASS_MODE_TRIGGER_FAX_BY_V21_PREAMBLE,
	CED_TRANSFER_MODE__IN_VOICE_MODE_OR_PCM_BYPASS,
	CED_TRANSFER_MODE__BY_RFC_4733_AND_RTP
	} Tac49xCedTransferMode;

/* For backward compatability*/
#define CED_TRANSFER_MODE__BY_FAX_RELAY_OR_PCM_BYPASS                CED_TRANSFER_MODE__BY_FAX_RELAY
#define	CED_TRANSFER_MODE__IN_VOICE_MODE_TRIGGER_FAX_BY_V21_PREAMBLE CED_TRANSFER_MODE__IN_VOICE_OR_PCM_BYPASS_MODE_TRIGGER_FAX_BY_V21_PREAMBLE

typedef enum
	{
	_3_WAY_CONFERENCE_MODE__DISABLE,
	_3_WAY_CONFERENCE_MODE__1,
	_3_WAY_CONFERENCE_MODE__2,
	_3_WAY_CONFERENCE_MODE__3,
	_3_WAY_CONFERENCE_MODE__4,
	_3_WAY_CONFERENCE_MODE__5
	} Tac49x3WayConferenceMode;

typedef enum
	{
	_3GPP_RATE__4_75_KBPS,
	_3GPP_RATE__5_15_KBPS,
	_3GPP_RATE__5_9_KBPS,
	_3GPP_RATE__6_7_KBPS,
	_3GPP_RATE__7_4_KBPS,
	_3GPP_RATE__7_95_KBPS,
	_3GPP_RATE__10_2_KBPS,
	_3GPP_RATE__12_2_KBPS,
	_3GPP_RATE__NO_DATA=15,
	_3GPP_RATE__G711_64_KBPS
	} Tac49x3GppRate;

typedef enum
	{
	_3GPP_RATE__6_60_KBPS,
	_3GPP_RATE__8_85_KBPS,
	_3GPP_RATE__12_65_KBPS,
	_3GPP_RATE__14_25_KBPS,
	_3GPP_RATE__15_85_KBPS,
	_3GPP_RATE__18_25_KBPS,
	_3GPP_RATE__19_85_KBPS,
	_3GPP_RATE__23_05_KBPS,
	_3GPP_RATE__23_85_KBPS
	} Tac49x3GppWbRate;

typedef enum
	{
	_3GPP_RFCI__AMR_4_75_KBPS,
	_3GPP_RFCI__AMR_5_15_KBPS,
	_3GPP_RFCI__AMR_5_9_KBPS,
	_3GPP_RFCI__AMR_6_7_KBPS,
	_3GPP_RFCI__AMR_7_4_KBPS,
	_3GPP_RFCI__AMR_7_95_KBPS,
	_3GPP_RFCI__AMR_10_2_KBPS,
	_3GPP_RFCI__AMR_12_2_KBPS,
	_3GPP_RFCI__AMR_SID,
	_3GPP_RFCI__GSM_EFR_SID,
	_3GPP_RFCI__TDMA_EFR_SID,
	_3GPP_RFCI__PDC_EFR_SID,
	_3GPP_RFCI__NO_DATA=15,
	_3GPP_RFCI__G711_64_KBPS
	} Tac49x3GppRfci;

typedef enum
	{
	_3GPP_WB_RFCI__AMR_WB_6_60_KBPS,
	_3GPP_WB_RFCI__AMR_WB_8_85_KBPS,
	_3GPP_WB_RFCI__AMR_WB_12_65_KBPS,
	_3GPP_WB_RFCI__AMR_WB_14_25_KBPS,
	_3GPP_WB_RFCI__AMR_WB_15_85_KBPS,
	_3GPP_WB_RFCI__AMR_WB_18_25_KBPS,
	_3GPP_WB_RFCI__AMR_WB_19_85_KBPS,
	_3GPP_WB_RFCI__AMR_WB_23_05_KBPS,
	_3GPP_WB_RFCI__AMR_WB_23_85_KBPS,
	_3GPP_WB_RFCI__AMR_WB_SID,
	_3GPP_WB_RFCI__AMR_WB_SPEECH_LOST=14,
	_3GPP_WB_RFCI__NO_DATA,
	_3GPP_WB_RFCI__G711_64_KBPS
	} Tac49x3GppWbRfci;

typedef enum
	{
	_3GPP_MODE__SUPPORT,
	_3GPP_MODE__TRANSPARENT
	} Tac49x3GppMode;

typedef enum
	{
	_3GPP_SUPPORT_MODE_TYPE__SLAVE,
	_3GPP_SUPPORT_MODE_TYPE__MASTER,
	_3GPP_SUPPORT_MODE_TYPE__PROPRIETARY
	} Tac49x3GppSupportModeType;

typedef enum
	{
	_3GPP_PROTOCOL_DATA_UNIT_TYPE__TYPE_0,
	_3GPP_PROTOCOL_DATA_UNIT_TYPE__TYPE_1
	} Tac49x3GppProtocolDataUnitType;

typedef enum
	{
	_3GPP_USER_PLANE_VERSION__RELEASE_99,
	_3GPP_USER_PLANE_VERSION__RELEASE_4,
	_3GPP_USER_PLANE_VERSION__RELEASE_5
	} Tac49x3GppUserPlaneVersion;

typedef enum
	{
	_3GPP_USER_PLANE_PROTOCOL_STATE__INIT=1,
	_3GPP_USER_PLANE_PROTOCOL_STATE__MASTER_WAIT_TO_INIT_ACKNOWLEDGE,
	_3GPP_USER_PLANE_PROTOCOL_STATE__RUN
	} Tac49x3GppUserPlaneProtocolState;




typedef enum
	{
	CHANGE_MODE_NEIGHBOR__CHANGED_TO_ANY_RATE,      /*!< Coder rate can be changed to any rate.*/
	CHANGE_MODE_NEIGHBOR__CHANGED_ONLY_TO_NEIGHBOR  /*!< Coder rate can be changed only to the neighbor rate (one level at a time, above or below the current level)*/
	} Tac49xChangeModeNeighbor;

typedef enum
	{
	RFC3267_AMR_RATE__4_75_KBPS,
	RFC3267_AMR_RATE__5_15_KBPS,
	RFC3267_AMR_RATE__5_9_KBPS,
	RFC3267_AMR_RATE__6_7_KBPS,
	RFC3267_AMR_RATE__7_4_KBPS,
	RFC3267_AMR_RATE__7_95_KBPS,
	RFC3267_AMR_RATE__10_2_KBPS,
	RFC3267_AMR_RATE__12_2_KBPS,
	RFC3267_AMR_RATE__NO_MODE_REQUEST=15
	} Tac49xRfc3267AmrRate;
#define RFC3267_AMR_RATE__NO_VOICE  RFC3267_AMR_RATE__NO_MODE_REQUEST

typedef enum
	{
	RFC3267_AMR_WB_RATE__6_6_KBPS,
	RFC3267_AMR_WB_RATE__8_85_KBPS,
	RFC3267_AMR_WB_RATE__12_65_KBPS,
	RFC3267_AMR_WB_RATE__14_25_KBPS,
	RFC3267_AMR_WB_RATE__15_85_KBPS,
	RFC3267_AMR_WB_RATE__18_25_KBPS,
	RFC3267_AMR_WB_RATE__19_85_KBPS,
	RFC3267_AMR_WB_RATE__23_05_KBPS,
	RFC3267_AMR_WB_RATE__23_85_KBPS,
	RFC3267_AMR_WB_RATE__NO_MODE_REQUEST=15
	} Tac49xRfc3267AmrWbRate;
#define RFC3267_AMR_WB_RATE__NO_VOICE  RFC3267_AMR_WB_RATE__NO_MODE_REQUEST

typedef enum
	{
	RFC3267_CODER__AMR,
	RFC3267_CODER__AMR_WB
	} Tac49xRfc3267Coder;

typedef enum
	{
    CALL_PROGRESS_ADD_VOICE_IN_CADENCE_SILENCE,
    CALL_PROGRESS_ADD_VOICE_TO_DIGIT,
    } Tac49xCallProgressAddVoiceMode;

typedef enum
	{
    CALL_PROGRESS_SIGNAL_TYPE__CONTINUOUS,
    CALL_PROGRESS_SIGNAL_TYPE__CADENCE,
    CALL_PROGRESS_SIGNAL_TYPE__BURST,
    CALL_PROGRESS_SIGNAL_TYPE__SPECIAL_INFORMATION_TONE
    } Tac49xCallProgressSignalType;

typedef enum
	{
	RTP_ADPCM_PAYLOAD_FORMAT__LITTLE_ENDIAN,
	RTP_ADPCM_PAYLOAD_FORMAT__BIG_ENDIAN
	} Tac49xRtpAdpcmPayloadFormat;

typedef enum
	{
	VBD_EVENTS_RELAY_TYPE__NONE,
	VBD_EVENTS_RELAY_TYPE__RFC2833,
	VBD_EVENTS_RELAY_TYPE__V_150_1_SSE
	}Tac49xVbdEventsRelayType;

typedef enum 
	{
    TRUNK_TESTING_MEASUREMENT_OPERATION__ENERGY,
    TRUNK_TESTING_MEASUREMENT_OPERATION__FSK_SIGNAL_DETECTION,
    TRUNK_TESTING_MEASUREMENT_OPERATION__UNKNOWN_TONE,
    TRUNK_TESTING_MEASUREMENT_OPERATION__STOP_PREVIOUS
    } Tac49xTrunkTestingMeasurementOperation;

typedef enum 
	{
    TRUNK_TESTING_MEASUREMENT_UNITS__POWER_LOSS_DB,
    TRUNK_TESTING_MEASUREMENT_UNITS__DBM,
    TRUNK_TESTING_MEASUREMENT_UNITS__DBRN
    } Tac49xTrunkTestingMeasurementUnits;

typedef enum 
	{
    TRUNK_TESTING_MEASUREMENT_DIRECTION__DONE_ON_PCM_INPUT,
    TRUNK_TESTING_MEASUREMENT_DIRECTION__DONE_ON_DETECTOR_OUTPUT
    } Tac49xTrunkTestingMeasurementDirection;

typedef enum 
	{
    TRUNK_TESTING_MEASUREMENT_REFERENCE_ENERGY__0dBm,
    TRUNK_TESTING_MEASUREMENT_REFERENCE_ENERGY__1dBm,
    TRUNK_TESTING_MEASUREMENT_REFERENCE_ENERGY__2dBm,
    TRUNK_TESTING_MEASUREMENT_REFERENCE_ENERGY__3dBm,
    TRUNK_TESTING_MEASUREMENT_REFERENCE_ENERGY__4dBm,
    TRUNK_TESTING_MEASUREMENT_REFERENCE_ENERGY__5dBm,
    TRUNK_TESTING_MEASUREMENT_REFERENCE_ENERGY__6dBm,
    TRUNK_TESTING_MEASUREMENT_REFERENCE_ENERGY__7dBm,
    TRUNK_TESTING_MEASUREMENT_REFERENCE_ENERGY__8dBm,
    TRUNK_TESTING_MEASUREMENT_REFERENCE_ENERGY__9dBm,
    TRUNK_TESTING_MEASUREMENT_REFERENCE_ENERGY__10dBm,
    TRUNK_TESTING_MEASUREMENT_REFERENCE_ENERGY__11dBm,
    TRUNK_TESTING_MEASUREMENT_REFERENCE_ENERGY__12dBm,
    TRUNK_TESTING_MEASUREMENT_REFERENCE_ENERGY__13dBm,
    TRUNK_TESTING_MEASUREMENT_REFERENCE_ENERGY__14dBm,
    TRUNK_TESTING_MEASUREMENT_REFERENCE_ENERGY__15dBm,
    TRUNK_TESTING_MEASUREMENT_REFERENCE_ENERGY__minus16dBm,
    TRUNK_TESTING_MEASUREMENT_REFERENCE_ENERGY__minus15dBm,
    TRUNK_TESTING_MEASUREMENT_REFERENCE_ENERGY__minus14dBm,
    TRUNK_TESTING_MEASUREMENT_REFERENCE_ENERGY__minus13dBm,
    TRUNK_TESTING_MEASUREMENT_REFERENCE_ENERGY__minus12dBm,
    TRUNK_TESTING_MEASUREMENT_REFERENCE_ENERGY__minus11dBm,
    TRUNK_TESTING_MEASUREMENT_REFERENCE_ENERGY__minus10dBm,
    TRUNK_TESTING_MEASUREMENT_REFERENCE_ENERGY__minus9dBm,
    TRUNK_TESTING_MEASUREMENT_REFERENCE_ENERGY__minus8dBm,
    TRUNK_TESTING_MEASUREMENT_REFERENCE_ENERGY__minus7dBm,
    TRUNK_TESTING_MEASUREMENT_REFERENCE_ENERGY__minus6dBm,
    TRUNK_TESTING_MEASUREMENT_REFERENCE_ENERGY__minus5dBm,
    TRUNK_TESTING_MEASUREMENT_REFERENCE_ENERGY__minus4dBm,
    TRUNK_TESTING_MEASUREMENT_REFERENCE_ENERGY__minus3dBm,
    TRUNK_TESTING_MEASUREMENT_REFERENCE_ENERGY__minus2dBm,
    TRUNK_TESTING_MEASUREMENT_REFERENCE_ENERGY__minus1dBm
    } Tac49xTrunkTestingMeasurementReferenceEnergy;

typedef enum 
	{
    TRUNK_TESTING_MEASUREMENT_TRIGGER_TYPE__IMMEDIATE,
    TRUNK_TESTING_MEASUREMENT_TRIGGER_TYPE__TRIGGER_TONE_DETECTION,
    TRUNK_TESTING_MEASUREMENT_TRIGGER_TYPE__ENERGY_THRESHOLD
    } Tac49xTrunkTestingMeasurementTriggerType;

typedef enum 
	{
    BER_TEST_RATE__64KBPS,                         /*!< 64 kbps*/
    BER_TEST_RATE__56KBPS_LSB_IGNORED_USED_FOR_T1  /*!< 56 kbps. The LSB of each sample is set to zero and ignored at detection. This rate is used for T1 links.*/
    } Tac49xBerTestRate;

typedef enum 
	{
    BER_TEST_ACTION__START_TEST,                    /*!< Start test*/
    BER_TEST_ACTION__STOP_TEST,                     /*!< Stop test*/
    BER_TEST_ACTION__CLEAR_STATISTIC_COUNTERS,      /*!< Choosing this option clears all test statistical counters and leaves the test running.*/
    BER_TEST_ACTION__SHOW_REPORT,                   /*!< In response to this command the channel sends a BER test report.*/
    BER_TEST_ACTION__INSERT_ERRORS_IN_CURRENT_TEST  /*!< Inserts errors in the Pseudo Random Pattern. A new command clears the previous one if it is unfinished.*/
    } Tac49xBerTestAction;

typedef enum 
	{
    BER_TEST_INJECTED_PATTERN__PSEUDO_RANDOM_SIZE_2047,  /*!< Pseudo Random Pattern (X-11 + X-9) size 2047.*/
    BER_TEST_INJECTED_PATTERN__PSEUDO_RANDOM_SIZE_511,   /*!< Pseudo Random Pattern (X-9 + X-5) size 511.*/
    BER_TEST_INJECTED_PATTERN__FIXED_USER_DEFINED        /*!< Fixed User-defined Pattern.*/
    } Tac49xBerTestInjectedPattern;

typedef enum 
	{
    BER_TEST_DIRECTION__ON_TDM_SIDE,
    BER_TEST_DIRECTION__ON_NETWORK_SIDE
    } Tac49xBerTestDirection;

typedef enum 
	{
    BER_TEST_ERROR_BITS_INSERTION_INTERVAL__1_SECOND,
    BER_TEST_ERROR_BITS_INSERTION_INTERVAL__10_SECONDS,
    BER_TEST_ERROR_BITS_INSERTION_INTERVAL__100_SECONDS,
    BER_TEST_ERROR_BITS_INSERTION_INTERVAL__1000_SECONDS,
    BER_TEST_ERROR_BITS_INSERTION_INTERVAL__10000_SECONDS
    } Tac49xBerTestErrorBitsInsertionInterval;
typedef enum 
	{
    BER_TEST_ERROR_BITS_INSERTION_TIME_MODE__ONCE,
    BER_TEST_ERROR_BITS_INSERTION_TIME_MODE__PERIODICALLY
    } Tac49xBerTestErrorBitsInsertionTimeMode;

typedef enum 
	{
    BER_TEST_ERROR_BITS_INSERTION_PATTERN_MODE__BURST,
    BER_TEST_ERROR_BITS_INSERTION_PATTERN_MODE__PSEUDO_RANDOM
    } Tac49xBerTestErrorBitsInsertionPatternMode;

typedef enum 
	{
    BER_TEST_REPORT_EVENT_INDEX__SYNCHRONIZATION_FOUND,
    BER_TEST_REPORT_EVENT_INDEX__SYNCHRONIZATION_TIMEOUT,
    BER_TEST_REPORT_EVENT_INDEX__SYNCHRONIZATION_LOST,
    BER_TEST_REPORT_EVENT_INDEX__TEST_ENDED,
    BER_TEST_REPORT_EVENT_INDEX__REPORT_DUE_TO_GET_COMMAND
    } Tac49xBerTestReportEventIndex;

typedef enum 
	{
    BER_TEST_REPORT_SYNCHRONIZATION_STATE__LOST,
    BER_TEST_REPORT_SYNCHRONIZATION_STATE__FOUND
    } Tac49xBerTestReportSynchronizationState;

typedef enum 
	{
    HDLC_FRAMER_COMMAND_MESSAGE_TYPE__HDLC_FRAMER_FREE_BUFFER_SIZE_REQUEST,
    HDLC_FRAMER_COMMAND_MESSAGE_TYPE__START_HDLC_FRAMER_OPERATION,
    HDLC_FRAMER_COMMAND_MESSAGE_TYPE__STOP_HDLC_FRAMER_OPERATION,
    HDLC_FRAMER_COMMAND_MESSAGE_TYPE__CLEAR_HDLC_FRAMER_BUFFER,
    HDLC_FRAMER_COMMAND_MESSAGE_TYPE__SEND_HDLC_FRAMER_MESSAGE
    } Tac49xHdlcFramerCommandMessageType;

typedef enum 
	{
    HDLC_FRAMER_STATUS_MESSAGE_TYPE__HDLC_FRAME_RECEIVED,
    HDLC_FRAMER_STATUS_MESSAGE_TYPE__HDLC_CRC_ERROR_DETECTED,
    HDLC_FRAMER_STATUS_MESSAGE_TYPE__HDLC_SHORT_FRAME_ERROR,
    HDLC_FRAMER_STATUS_MESSAGE_TYPE__HDLC_FRAME_EXCEED_LIMIT_ERROR,
    HDLC_FRAMER_STATUS_MESSAGE_TYPE__HDLC_BAD_LINK,
    HDLC_FRAMER_STATUS_MESSAGE_TYPE__HDLC_IDLE_PATTERN_STARTED,
    HDLC_FRAMER_STATUS_MESSAGE_TYPE__HDLC_PREAMBLE_STARTED,
    HDLC_FRAMER_STATUS_MESSAGE_TYPE__HDLC_BUFFER_OVERRUN=8,
    HDLC_FRAMER_STATUS_MESSAGE_TYPE__HDLC_FRAMER_FREE_BUFFER_SIZE, /*HDLC_FRAMER_STATUS_MESSAGE_TYPE__HDLC_BUFFER_STATUS_RESPONSE,*/
    HDLC_FRAMER_STATUS_MESSAGE_TYPE__HDLC_BUFFER_CLEAR_ACKNOWLEDGE,
    HDLC_FRAMER_STATUS_MESSAGE_TYPE__HDLC_RUN_ACKNOWLEDGE,
    HDLC_FRAMER_STATUS_MESSAGE_TYPE__HDLC_START_ACKNOWLEDGE,
    HDLC_FRAMER_STATUS_MESSAGE_TYPE__HDLC_STOP_ACKNOWLEDGE
    } Tac49xHdlcFramerStatusMessageType;


typedef enum 
	{
    BIAS_PIN_OUTPUT_VOLTAGE__1v35,
    BIAS_PIN_OUTPUT_VOLTAGE__2v35
    } Tac49xBiasPinOutputVoltage;

typedef enum 
	{
    SPEAKER_GAIN__0dB,
    SPEAKER_GAIN__1dB,  
    SPEAKER_GAIN__2dB,
    SPEAKER_GAIN__3dB
    } Tac49xSpeakerGain;

typedef enum 
	{
    SIDETONE_GAIN__minusd9dB,
    SIDETONE_GAIN__minusd12dB,
    SIDETONE_GAIN__minusd15dB,
    SIDETONE_GAIN__minusd18dB,
    SIDETONE_GAIN__minusd21dB,
    SIDETONE_GAIN__minusd24dB,
    SIDETONE_GAIN__minusd27dB,
    SIDETONE_GAIN__MUTE   
    } Tac49xSidetoneGain;

typedef enum 
	{
    CODEC_DAC_OUTPUT_PGA_GAIN__0dB,
    CODEC_DAC_OUTPUT_PGA_GAIN__minus1_5dB,
    CODEC_DAC_OUTPUT_PGA_GAIN__minus3dB,
    CODEC_DAC_OUTPUT_PGA_GAIN__minus4_5dB,
    CODEC_DAC_OUTPUT_PGA_GAIN__minus6dB,
    CODEC_DAC_OUTPUT_PGA_GAIN__minus7_5dB,
    CODEC_DAC_OUTPUT_PGA_GAIN__minus9dB,
    CODEC_DAC_OUTPUT_PGA_GAIN__minus10_5dB,
    CODEC_DAC_OUTPUT_PGA_GAIN__minus12dB,
    CODEC_DAC_OUTPUT_PGA_GAIN__minus13_5dB,
    CODEC_DAC_OUTPUT_PGA_GAIN__minus15dB,
    CODEC_DAC_OUTPUT_PGA_GAIN__minus16_5dB,
    CODEC_DAC_OUTPUT_PGA_GAIN__minus18dB,
    CODEC_DAC_OUTPUT_PGA_GAIN__minus19_5dB,
    CODEC_DAC_OUTPUT_PGA_GAIN__minus21dB,
    CODEC_DAC_OUTPUT_PGA_GAIN__minus22_5dB,
    CODEC_DAC_OUTPUT_PGA_GAIN__minus24dB,
    CODEC_DAC_OUTPUT_PGA_GAIN__minus25_5dB,
    CODEC_DAC_OUTPUT_PGA_GAIN__minus27dB,
    CODEC_DAC_OUTPUT_PGA_GAIN__minus28_5dB,
    CODEC_DAC_OUTPUT_PGA_GAIN__minus30dB,
    CODEC_DAC_OUTPUT_PGA_GAIN__minus31_5dB,
    CODEC_DAC_OUTPUT_PGA_GAIN__minus33dB,
    CODEC_DAC_OUTPUT_PGA_GAIN__minus34_5dB,
    CODEC_DAC_OUTPUT_PGA_GAIN__minus36dB,
    CODEC_DAC_OUTPUT_PGA_GAIN__minus37_5dB,
    CODEC_DAC_OUTPUT_PGA_GAIN__minus39dB,
    CODEC_DAC_OUTPUT_PGA_GAIN__minus40_5dB,
    CODEC_DAC_OUTPUT_PGA_GAIN__minus42dB,
    CODEC_DAC_OUTPUT_PGA_GAIN__minus48dB,
    CODEC_DAC_OUTPUT_PGA_GAIN__minus54dB,
    CODEC_DAC_OUTPUT_PGA_GAIN__MUTE
    } Tac49xCodecDacOutputPgaGain;

typedef enum 
	{
    CODEC_ADC_INPUT_PGA_GAIN__0dB,
    CODEC_ADC_INPUT_PGA_GAIN__plus1_5dB,
    CODEC_ADC_INPUT_PGA_GAIN__plus3dB,
    CODEC_ADC_INPUT_PGA_GAIN__plus4_5dB,
    CODEC_ADC_INPUT_PGA_GAIN__plus6dB,
    CODEC_ADC_INPUT_PGA_GAIN__plus7_5dB,
    CODEC_ADC_INPUT_PGA_GAIN__plus9dB,
    CODEC_ADC_INPUT_PGA_GAIN__plus10_5dB,
    CODEC_ADC_INPUT_PGA_GAIN__plus12dB,
    CODEC_ADC_INPUT_PGA_GAIN__plus13_5dB,
    CODEC_ADC_INPUT_PGA_GAIN__plus15dB,
    CODEC_ADC_INPUT_PGA_GAIN__plus16_5dB,
    CODEC_ADC_INPUT_PGA_GAIN__plus18dB,
    CODEC_ADC_INPUT_PGA_GAIN__plus19_5dB,
    CODEC_ADC_INPUT_PGA_GAIN__plus21dB,
    CODEC_ADC_INPUT_PGA_GAIN__plus22_5dB,
    CODEC_ADC_INPUT_PGA_GAIN__plus24dB,
    CODEC_ADC_INPUT_PGA_GAIN__plus25_5dB,
    CODEC_ADC_INPUT_PGA_GAIN__plus27dB,
    CODEC_ADC_INPUT_PGA_GAIN__plus28_5dB,
    CODEC_ADC_INPUT_PGA_GAIN__plus30dB,
    CODEC_ADC_INPUT_PGA_GAIN__plus31_5dB,
    CODEC_ADC_INPUT_PGA_GAIN__plus33dB,
    CODEC_ADC_INPUT_PGA_GAIN__plus34_5dB,
    CODEC_ADC_INPUT_PGA_GAIN__plus36dB,
    CODEC_ADC_INPUT_PGA_GAIN__plus37_5dB,
    CODEC_ADC_INPUT_PGA_GAIN__plus39dB,
    CODEC_ADC_INPUT_PGA_GAIN__plus40_5dB,
    CODEC_ADC_INPUT_PGA_GAIN__plus42dB,
    CODEC_ADC_INPUT_PGA_GAIN__plus48dB,
    CODEC_ADC_INPUT_PGA_GAIN__plus54dB,
    CODEC_ADC_INPUT_PGA_GAIN__MUTE
    } Tac49xCodecAdcInputPgaGain;


			/*******/
			/* TFO */
			/*******/
typedef enum 
	{
	TANDEM_FREE_OPERATION_CODEC_TYPE__GSM_FR		,
	TANDEM_FREE_OPERATION_CODEC_TYPE__GSM_HR		,
	TANDEM_FREE_OPERATION_CODEC_TYPE__GSM_EFR		,
	TANDEM_FREE_OPERATION_CODEC_TYPE__FR_AMR		,
	TANDEM_FREE_OPERATION_CODEC_TYPE__HR_AMR		,
	TANDEM_FREE_OPERATION_CODEC_TYPE__UMTS_AMR		,
	TANDEM_FREE_OPERATION_CODEC_TYPE__UMTS_AMR2		,
	TANDEM_FREE_OPERATION_CODEC_TYPE__TDMA_EFR		,
	TANDEM_FREE_OPERATION_CODEC_TYPE__PDC_EFR		,
	TANDEM_FREE_OPERATION_CODEC_TYPE__FR_AMR_WB		,
	TANDEM_FREE_OPERATION_CODEC_TYPE__UMTS_AMR_WB
	} Tac49xTandemFreeOperationCodecType;

typedef enum 
	{
	TANDEM_FREE_OPERATION_COMMAND_FUNCTION__START,
	TANDEM_FREE_OPERATION_COMMAND_FUNCTION__UPDATE_CODEC_LIST,   
	TANDEM_FREE_OPERATION_COMMAND_FUNCTION__MODIFY_ACTIVE_CODEC, 
	TANDEM_FREE_OPERATION_COMMAND_FUNCTION__STOP
	} Tac49xTandemFreeOperationCommandFunction;

typedef enum 
	{
	TANDEM_FREE_OPERATION_STATUS_FUNCTION__REMOTE_SIDE_CODEC_LIST,
	TANDEM_FREE_OPERATION_STATUS_FUNCTION__OPTIONAL_CODEC,
	TANDEM_FREE_OPERATION_STATUS_FUNCTION__VAD_MISMATCH,
	TANDEM_FREE_OPERATION_STATUS_FUNCTION__MONITORING_REMOTE_SIDE_TFO_MESSAGES,
	TANDEM_FREE_OPERATION_STATUS_FUNCTION__NEGOTIATING_WITH_THE_REMOTE_SIDE,
	TANDEM_FREE_OPERATION_STATUS_FUNCTION__CODEC_MISMATCH,
	TANDEM_FREE_OPERATION_STATUS_FUNCTION__ON,
	TANDEM_FREE_OPERATION_STATUS_FUNCTION__OFF,
	/*TANDEM_FREE_OPERATION_STATUS_FUNCTION__SETUP_ERROR*/
	TANDEM_FREE_OPERATION_STATUS_FUNCTION__END
	} Tac49xTandemFreeOperationStatusFunction;

typedef enum 
	{
	TANDEM_FREE_OPERATION_SYSTEM__GSM,
	TANDEM_FREE_OPERATION_SYSTEM__UMTS=4
	} Tac49xTandemFreeOperationSystem;

typedef enum
	{
	SS7_MTP1_MESSAGE_TYPE__SDL_CC_NORMAL_REQUEST												= 66,
	SS7_MTP1_MESSAGE_TYPE__SDL_SIGNAL_UNIT_REQUEST												,
	SS7_MTP1_MESSAGE_TYPE__SDL_SET_ALIGNMENT_ERROR_RATE_MONITOR_PARAMETERS_TO_NORMAL_REQUEST	= 70,
	SS7_MTP1_MESSAGE_TYPE__SDL_SET_ALIGNMENT_ERROR_RATE_MONITOR_PARAMETERS_TO_EMERGENCY_REQUEST	,
	SS7_MTP1_MESSAGE_TYPE__SDL_START_ALIGNMENT_ERROR_RATE_MONITOR_REQUEST						,
	SS7_MTP1_MESSAGE_TYPE__SDL_STOP_ALIGNMENT_ERROR_RATE_MONITOR_REQUEST						,
	SS7_MTP1_MESSAGE_TYPE__SDL_START_SIGNAL_UNIT_ERROR_RATE_MONITOR_REQUEST						,
	SS7_MTP1_MESSAGE_TYPE__SDL_STOP_SIGNAL_UNIT_ERROR_RATE_MONITOR_REQUEST						,
	SS7_MTP1_MESSAGE_TYPE__SDL_TX_START_REQUEST													,
	SS7_MTP1_MESSAGE_TYPE__SDL_TX_STOP_REQUEST													,
	SS7_MTP1_MESSAGE_TYPE__SDL_RX_START_REQUEST													,
	SS7_MTP1_MESSAGE_TYPE__SDL_RX_STOP_REQUEST													,
	SS7_MTP1_MESSAGE_TYPE__AC49X_SET_CONFIGURATION_REQUEST										= 87,
	SS7_MTP1_MESSAGE_TYPE__AC49X_REQUEST_STOPPING_TRAFFIC_ENTITY_REQUEST						,
	SS7_MTP1_MESSAGE_TYPE__AC49X_RESET_CONFIGURATION_REQUEST									,
	SS7_MTP1_MESSAGE_TYPE__AC49X_INFORMATION_REQUEST											,
	SS7_MTP1_MESSAGE_TYPE__SDL_CC_BUSY_REQUEST													= 98,
	SS7_MTP1_MESSAGE_TYPE__SDL_FORCE_RX_REQUEST													= 107,
	SS7_MTP1_MESSAGE_TYPE__AC49X_DEBUG_INFO														= 153
	} Tac49xSs7Mtp1CommandMessageType;

typedef enum
	{
	SS7_MTP1_MESSAGE_TYPE__SDL_SU_IN						= 99,
	SS7_MTP1_MESSAGE_TYPE__SDL_LINK_FAILURE_IN				= 102,
	SS7_MTP1_MESSAGE_TYPE__SDL_ABORT_PROVING_IN				,
	SS7_MTP1_MESSAGE_TYPE__SDL_TX_REQUEST_IN				,
	SS7_MTP1_MESSAGE_TYPE__SDL_RESET_IN						= 114,
	SS7_MTP1_MESSAGE_TYPE__AC49_SET_CONFIG_CONFIRMATION		= 119,
	SS7_MTP1_MESSAGE_TYPE__AC49_STOP_CONFIRMATION			,
	SS7_MTP1_MESSAGE_TYPE__AC49_RESET_CONFIG_CONFIRMATION	,
	SS7_MTP1_MESSAGE_TYPE__AC49_INFO_CONFIRMATION
	} Tac49xSs7Mtp1StatusMessageType;

typedef enum
	{
	SS7_LINK_RATE__64_KBPS,
	SS7_LINK_RATE__56_KBPS
	} Tac49xSs7LinkRate;

typedef enum
	{
	PCM_LIMITER__DISABLE,
	PCM_LIMITER__MODE_1,	/*!< (-4.91 )dBm - (+2.12)dBm */
	PCM_LIMITER__MODE_2,	/*!< (-12.13)dBm - (-5.26)dBm */
	PCM_LIMITER__MODE_3,	/*!< (-9.15 )dBm - (-2.13)dBm */
	PCM_LIMITER__MODE_4,	/*!< (-6.82 )dBm - (+0.06)dBm */
	PCM_LIMITER__MODE_5		/*!< (-11.02)dBm - (-4.24)dBm */
	} Tac49xPcmLimiter;

typedef enum
	{
	DTMF_SNR__12DB,
	DTMF_SNR__15DB,
	DTMF_SNR__18DB,
	DTMF_SNR__21DB
	} Tac49xDtmfSnr;

typedef enum
	{
	BROKEN_CONNECTION_ACTIVATION_MODE__ACTIVATE_AFTER_FIRST_RTP_PCKAET_ARRIVAL,
	BROKEN_CONNECTION_ACTIVATION_MODE__ACTIVATE_IMMEDIATLY_AFTER_ACTIVATE_RTP_COMMAND
	} Tac49xBrokenConnectionActivationMode;

typedef enum
	{
	DTMF_ERASURE_MODE__DISABLE,
	DTMF_ERASURE_MODE__ERASE_1_COMPONENT,
	DTMF_ERASURE_MODE__ERASE_2_COMPONENTS,
	DTMF_ERASURE_MODE__TEST_MODE
	} Tac49xDtmfErasureMode;

typedef enum
    {
    DTMF_ERASURE_SENSITIVITY__LOW,
    DTMF_ERASURE_SENSITIVITY__MEDIUM_LOW,
    DTMF_ERASURE_SENSITIVITY__MEDIUM_HI,
    DTMF_ERASURE_SENSITIVITY__HIGH
    } Tac49xDtmfErasureSensitivity;

typedef enum
	{
	CAS_GENERATION_DIRECTION__TO_TDM_CAS_PORT,
	CAS_GENERATION_DIRECTION__TO_NET_RFC_2833_PACKET
	} Tac49xCasGenerationDirection;

typedef enum
	{
	CAS_DETECTION_DIRECTION__ON_TDM_CAS_PORT,
	CAS_DETECTION_DIRECTION__ON_NET_RFC_2833_PACKET
	} Tac49xCasDetectionDirection;

typedef enum
	{
	IBS_DETECTION_DIRECTION__ON_TDM,
	IBS_DETECTION_DIRECTION__ON_NET_PACKET,
	IBS_DETECTION_DIRECTION__ON_DECODER_OUTPUT
	} Tac49xIbsDetectionDirection;

typedef enum
	{
	IP_SEC_PORT_SELECTION__HOST,
	IP_SEC_PORT_SELECTION__NETWORK
	} Tac49xIpSecPortSelection;

typedef enum
	{
	PCM_SID_SIZE__1_BYTE,
	PCM_SID_SIZE__5_BYTE=4,
	PCM_SID_SIZE__7_BYTE=6,
	PCM_SID_SIZE__9_BYTE=8,
	PCM_SID_SIZE__11_BYTE=10
	}Tac49xPcmSidSize;

typedef enum
	{
	PCM_CNG_THRESHOLD__minus20dBm_Default,
	PCM_CNG_THRESHOLD__minus5dBm=5,
	PCM_CNG_THRESHOLD__minus6dBm,
	PCM_CNG_THRESHOLD__minus7dBm,
	PCM_CNG_THRESHOLD__minus8dBm,
	PCM_CNG_THRESHOLD__minus9dBm,
	PCM_CNG_THRESHOLD__minus10dBm,
	PCM_CNG_THRESHOLD__minus11dBm,
	PCM_CNG_THRESHOLD__minus12dBm,
	PCM_CNG_THRESHOLD__minus13dBm,
	PCM_CNG_THRESHOLD__minus14dBm,
	PCM_CNG_THRESHOLD__minus15dBm,
	PCM_CNG_THRESHOLD__minus16dBm,
	PCM_CNG_THRESHOLD__minus17dBm,
	PCM_CNG_THRESHOLD__minus18dBm,
	PCM_CNG_THRESHOLD__minus19dBm,
	PCM_CNG_THRESHOLD__minus20dBm,
	PCM_CNG_THRESHOLD__minus21dBm,
	PCM_CNG_THRESHOLD__minus22dBm,
	PCM_CNG_THRESHOLD__minus23dBm,
	PCM_CNG_THRESHOLD__minus24dBm,
	PCM_CNG_THRESHOLD__minus25dBm,
	PCM_CNG_THRESHOLD__minus26dBm,
	PCM_CNG_THRESHOLD__minus27dBm,
	PCM_CNG_THRESHOLD__minus28dBm,
	PCM_CNG_THRESHOLD__minus29dBm,
	PCM_CNG_THRESHOLD__minus30dBm,
	PCM_CNG_THRESHOLD__minus31dBm,
	PCM_CNG_THRESHOLD__minus32dBm,
	PCM_CNG_THRESHOLD__minus33dBm,
	PCM_CNG_THRESHOLD__minus34dBm,
	PCM_CNG_THRESHOLD__minus35dBm,
	PCM_CNG_THRESHOLD__minus36dBm,
	PCM_CNG_THRESHOLD__minus37dBm,
	PCM_CNG_THRESHOLD__minus38dBm,
	PCM_CNG_THRESHOLD__minus39dBm,
	PCM_CNG_THRESHOLD__minus40dBm,
	PCM_CNG_THRESHOLD__minus41dBm,
	PCM_CNG_THRESHOLD__minus42dBm,
	PCM_CNG_THRESHOLD__minus43dBm,
	PCM_CNG_THRESHOLD__minus44dBm,
	PCM_CNG_THRESHOLD__minus45dBm,
	PCM_CNG_THRESHOLD__minus46dBm,
	PCM_CNG_THRESHOLD__minus47dBm,
	PCM_CNG_THRESHOLD__minus48dBm,
	PCM_CNG_THRESHOLD__minus49dBm,
	PCM_CNG_THRESHOLD__minus50dBm,
	PCM_CNG_THRESHOLD__minus51dBm,
	PCM_CNG_THRESHOLD__minus52dBm,
	PCM_CNG_THRESHOLD__minus53dBm,
	PCM_CNG_THRESHOLD__minus54dBm,
	PCM_CNG_THRESHOLD__minus55dBm,
	PCM_CNG_THRESHOLD__minus56dBm,
	PCM_CNG_THRESHOLD__minus57dBm,
	PCM_CNG_THRESHOLD__minus58dBm,
	PCM_CNG_THRESHOLD__minus59dBm,
	PCM_CNG_THRESHOLD__minus60dBm,
	PCM_CNG_THRESHOLD__minus61dBm,
	PCM_CNG_THRESHOLD__minus62dBm,
	PCM_CNG_THRESHOLD__minus63dBm
	} Tac49xPcmCngThreshold;

typedef enum
	{
	GENERIC_CNG_OVERRIDE_NONE,
	GENERIC_CNG_OVERRIDE_NO_PARCORS,
	GENERIC_CNG_OVERRIDE_ALL,
	GENERIC_CNG_OVERRIDE_RESERVED,
	} Tac49xGenericCngOverrideMode;

typedef enum
	{
	UTOPIA_TEST_ERROR_TYPE__ILLEGAL_CELL,
	UTOPIA_TEST_ERROR_TYPE__BAD_PACKET_ILLEGAL_CORE_ID,
	UTOPIA_TEST_ERROR_TYPE__BAD_PACKET_INCORRECT_SEQUENCE_NUMBER,
	UTOPIA_TEST_ERROR_TYPE__BAD_PACKET_INCORRECT_PAYLOAD
	} Tac49xUtopiaTestErrorType;

typedef enum
	{
	INPUT_GAIN_LOCATION__BEFORE_ECHO_CANCELER,
	INPUT_GAIN_LOCATION__AFTER_ECHO_CANCELER_AND_BOFORE_ENCODER
	} Tac49xInputGainLocation;

typedef enum
	{
	IBS_GENERATION_LOCATION__ON_3WAY_CONFERENCE_OUTPUT ,
	IBS_GENERATION_LOCATION__ON_3WAY_CONFERENCE_INPUT
	} Tac49xDecoderIBSGenerationLocation;

typedef enum
	{
	G722_OVERLOAD_POINT__3_DBMO,
	G722_OVERLOAD_POINT__9_DBMO
	} Tac49xG722OverloadPoint;

typedef enum
	{
	ACOUSTIC_ECHO_CANCELER_SUPPORTED_MODE__SPEAKER_PHONE_ONLY,
	ACOUSTIC_ECHO_CANCELER_SUPPORTED_MODE__HANDSET_ONLY,
	ACOUSTIC_ECHO_CANCELER_SUPPORTED_MODE__HEADSET_ONLY,
	ACOUSTIC_ECHO_CANCELER_SUPPORTED_MODE__HANDSET_PLUS_SPEAKER,
	ACOUSTIC_ECHO_CANCELER_SUPPORTED_MODE__HEADSET_PLUS_SPEAKER
	} Tac49xAcousticEchoCancelerSupportedMode;

typedef enum
	{
	ACOUSTIC_ECHO_CANCELER_ECHO_RETURN_LOSS__0_dB,
	ACOUSTIC_ECHO_CANCELER_ECHO_RETURN_LOSS__3_dB,
	ACOUSTIC_ECHO_CANCELER_ECHO_RETURN_LOSS__6_dB,
	ACOUSTIC_ECHO_CANCELER_ECHO_RETURN_LOSS__9_dB,
	ACOUSTIC_ECHO_CANCELER_ECHO_RETURN_LOSS__12_dB,
	ACOUSTIC_ECHO_CANCELER_ECHO_RETURN_LOSS__15_dB
	} Tac49xAcousticEchoCancelerEchoReturnLoss;

typedef enum
	{
	ACOUSTIC_ECHO_CANCELER_MAX_ACTIVE_FILTER_LENGTH__16_msec,
	ACOUSTIC_ECHO_CANCELER_MAX_ACTIVE_FILTER_LENGTH__32_msec,
	ACOUSTIC_ECHO_CANCELER_MAX_ACTIVE_FILTER_LENGTH__64_msec,
	ACOUSTIC_ECHO_CANCELER_MAX_ACTIVE_FILTER_LENGTH__128_msec
	} Tac49xAcousticEchoCancelerMaxActiveFilterLength;

typedef enum
	{
	ACOUSTIC_ECHO_CANCELER_TAIL_LENGTH__64_msec,
	ACOUSTIC_ECHO_CANCELER_TAIL_LENGTH__128_msec,
	ACOUSTIC_ECHO_CANCELER_TAIL_LENGTH__256_msec,
	ACOUSTIC_ECHO_CANCELER_TAIL_LENGTH__512_msec
	} Tac49xAcousticEchoCancelerTailLength;

typedef enum
	{
	ACOUSTIC_ECHO_CANCELER_COEFFICIENT_THRESHOLD__minus_30_dBm=30,
	ACOUSTIC_ECHO_CANCELER_COEFFICIENT_THRESHOLD__minus_31_dBm,
	ACOUSTIC_ECHO_CANCELER_COEFFICIENT_THRESHOLD__minus_32_dBm,
	ACOUSTIC_ECHO_CANCELER_COEFFICIENT_THRESHOLD__minus_33_dBm,
	ACOUSTIC_ECHO_CANCELER_COEFFICIENT_THRESHOLD__minus_34_dBm,
	ACOUSTIC_ECHO_CANCELER_COEFFICIENT_THRESHOLD__minus_35_dBm,
	ACOUSTIC_ECHO_CANCELER_COEFFICIENT_THRESHOLD__minus_36_dBm,
	ACOUSTIC_ECHO_CANCELER_COEFFICIENT_THRESHOLD__minus_37_dBm,
	ACOUSTIC_ECHO_CANCELER_COEFFICIENT_THRESHOLD__minus_38_dBm,
	ACOUSTIC_ECHO_CANCELER_COEFFICIENT_THRESHOLD__minus_39_dBm,
	ACOUSTIC_ECHO_CANCELER_COEFFICIENT_THRESHOLD__minus_40_dBm,
	ACOUSTIC_ECHO_CANCELER_COEFFICIENT_THRESHOLD__minus_41_dBm,
	ACOUSTIC_ECHO_CANCELER_COEFFICIENT_THRESHOLD__minus_42_dBm,
	ACOUSTIC_ECHO_CANCELER_COEFFICIENT_THRESHOLD__minus_43_dBm,
	ACOUSTIC_ECHO_CANCELER_COEFFICIENT_THRESHOLD__minus_44_dBm,
	ACOUSTIC_ECHO_CANCELER_COEFFICIENT_THRESHOLD__minus_45_dBm,
	ACOUSTIC_ECHO_CANCELER_COEFFICIENT_THRESHOLD__minus_46_dBm,
	ACOUSTIC_ECHO_CANCELER_COEFFICIENT_THRESHOLD__minus_47_dBm,
	ACOUSTIC_ECHO_CANCELER_COEFFICIENT_THRESHOLD__minus_48_dBm,
	ACOUSTIC_ECHO_CANCELER_COEFFICIENT_THRESHOLD__minus_49_dBm,
	ACOUSTIC_ECHO_CANCELER_COEFFICIENT_THRESHOLD__minus_50_dBm,
	ACOUSTIC_ECHO_CANCELER_COEFFICIENT_THRESHOLD__minus_51_dBm,
	ACOUSTIC_ECHO_CANCELER_COEFFICIENT_THRESHOLD__minus_52_dBm,
	ACOUSTIC_ECHO_CANCELER_COEFFICIENT_THRESHOLD__minus_53_dBm,
	ACOUSTIC_ECHO_CANCELER_COEFFICIENT_THRESHOLD__minus_54_dBm,
	ACOUSTIC_ECHO_CANCELER_COEFFICIENT_THRESHOLD__minus_55_dBm,
	ACOUSTIC_ECHO_CANCELER_COEFFICIENT_THRESHOLD__minus_56_dBm,
	ACOUSTIC_ECHO_CANCELER_COEFFICIENT_THRESHOLD__minus_57_dBm,
	ACOUSTIC_ECHO_CANCELER_COEFFICIENT_THRESHOLD__minus_58_dBm,
	ACOUSTIC_ECHO_CANCELER_COEFFICIENT_THRESHOLD__minus_59_dBm,
	ACOUSTIC_ECHO_CANCELER_COEFFICIENT_THRESHOLD__minus_60_dBm,
	ACOUSTIC_ECHO_CANCELER_COEFFICIENT_THRESHOLD__minus_61_dBm,
	ACOUSTIC_ECHO_CANCELER_COEFFICIENT_THRESHOLD__minus_62_dBm,
	ACOUSTIC_ECHO_CANCELER_COEFFICIENT_THRESHOLD__minus_63_dBm
	} Tac49xAcousticEchoCancelerCoefficientsThreshold;

typedef enum
	{
	ACOUSTIC_ECHO_CANCELER_NLP_ACTIVATION_THRESHOLD__AUTOMATIC,
	ACOUSTIC_ECHO_CANCELER_NLP_ACTIVATION_THRESHOLD__minus_30_dBm=30,
	ACOUSTIC_ECHO_CANCELER_NLP_ACTIVATION_THRESHOLD__minus_31_dBm,
	ACOUSTIC_ECHO_CANCELER_NLP_ACTIVATION_THRESHOLD__minus_32_dBm,
	ACOUSTIC_ECHO_CANCELER_NLP_ACTIVATION_THRESHOLD__minus_33_dBm,
	ACOUSTIC_ECHO_CANCELER_NLP_ACTIVATION_THRESHOLD__minus_34_dBm,
	ACOUSTIC_ECHO_CANCELER_NLP_ACTIVATION_THRESHOLD__minus_35_dBm,
	ACOUSTIC_ECHO_CANCELER_NLP_ACTIVATION_THRESHOLD__minus_36_dBm,
	ACOUSTIC_ECHO_CANCELER_NLP_ACTIVATION_THRESHOLD__minus_37_dBm,
	ACOUSTIC_ECHO_CANCELER_NLP_ACTIVATION_THRESHOLD__minus_38_dBm,
	ACOUSTIC_ECHO_CANCELER_NLP_ACTIVATION_THRESHOLD__minus_39_dBm,
	ACOUSTIC_ECHO_CANCELER_NLP_ACTIVATION_THRESHOLD__minus_40_dBm,
	ACOUSTIC_ECHO_CANCELER_NLP_ACTIVATION_THRESHOLD__minus_41_dBm,
	ACOUSTIC_ECHO_CANCELER_NLP_ACTIVATION_THRESHOLD__minus_42_dBm,
	ACOUSTIC_ECHO_CANCELER_NLP_ACTIVATION_THRESHOLD__minus_43_dBm,
	ACOUSTIC_ECHO_CANCELER_NLP_ACTIVATION_THRESHOLD__minus_44_dBm,
	ACOUSTIC_ECHO_CANCELER_NLP_ACTIVATION_THRESHOLD__minus_45_dBm,
	ACOUSTIC_ECHO_CANCELER_NLP_ACTIVATION_THRESHOLD__minus_46_dBm,
	ACOUSTIC_ECHO_CANCELER_NLP_ACTIVATION_THRESHOLD__minus_47_dBm,
	ACOUSTIC_ECHO_CANCELER_NLP_ACTIVATION_THRESHOLD__minus_48_dBm,
	ACOUSTIC_ECHO_CANCELER_NLP_ACTIVATION_THRESHOLD__minus_49_dBm,
	ACOUSTIC_ECHO_CANCELER_NLP_ACTIVATION_THRESHOLD__minus_50_dBm,
	ACOUSTIC_ECHO_CANCELER_NLP_ACTIVATION_THRESHOLD__minus_51_dBm,
	ACOUSTIC_ECHO_CANCELER_NLP_ACTIVATION_THRESHOLD__minus_52_dBm,
	ACOUSTIC_ECHO_CANCELER_NLP_ACTIVATION_THRESHOLD__minus_53_dBm,
	ACOUSTIC_ECHO_CANCELER_NLP_ACTIVATION_THRESHOLD__minus_54_dBm,
	ACOUSTIC_ECHO_CANCELER_NLP_ACTIVATION_THRESHOLD__minus_55_dBm,
	ACOUSTIC_ECHO_CANCELER_NLP_ACTIVATION_THRESHOLD__minus_56_dBm,
	ACOUSTIC_ECHO_CANCELER_NLP_ACTIVATION_THRESHOLD__minus_57_dBm,
	ACOUSTIC_ECHO_CANCELER_NLP_ACTIVATION_THRESHOLD__minus_58_dBm,
	ACOUSTIC_ECHO_CANCELER_NLP_ACTIVATION_THRESHOLD__minus_59_dBm,
	ACOUSTIC_ECHO_CANCELER_NLP_ACTIVATION_THRESHOLD__minus_60_dBm,
	ACOUSTIC_ECHO_CANCELER_NLP_ACTIVATION_THRESHOLD__minus_61_dBm,
	ACOUSTIC_ECHO_CANCELER_NLP_ACTIVATION_THRESHOLD__minus_62_dBm,
	ACOUSTIC_ECHO_CANCELER_NLP_ACTIVATION_THRESHOLD__minus_63_dBm
	} Tac49xAcousticEchoCancelerNlpActivationThreshold;

typedef enum
	{
	ACOUSTIC_ECHO_CANCELER_SATURATION_THRESHOLD__2047,
	ACOUSTIC_ECHO_CANCELER_SATURATION_THRESHOLD__4095,
	ACOUSTIC_ECHO_CANCELER_SATURATION_THRESHOLD__6143,
	ACOUSTIC_ECHO_CANCELER_SATURATION_THRESHOLD__8191,

	ACOUSTIC_ECHO_CANCELER_SATURATION_THRESHOLD__10239,
	ACOUSTIC_ECHO_CANCELER_SATURATION_THRESHOLD__12287,
	ACOUSTIC_ECHO_CANCELER_SATURATION_THRESHOLD__14335,
	ACOUSTIC_ECHO_CANCELER_SATURATION_THRESHOLD__16383,

	ACOUSTIC_ECHO_CANCELER_SATURATION_THRESHOLD__18431,
	ACOUSTIC_ECHO_CANCELER_SATURATION_THRESHOLD__20479,
	ACOUSTIC_ECHO_CANCELER_SATURATION_THRESHOLD__22527,
	ACOUSTIC_ECHO_CANCELER_SATURATION_THRESHOLD__24575,

	ACOUSTIC_ECHO_CANCELER_SATURATION_THRESHOLD__26623,
	ACOUSTIC_ECHO_CANCELER_SATURATION_THRESHOLD__28671,
	ACOUSTIC_ECHO_CANCELER_SATURATION_THRESHOLD__30719,
	ACOUSTIC_ECHO_CANCELER_SATURATION_THRESHOLD__32767
	} Tac49xAcousticEchoCancelerSaturationThreshold;

typedef enum
	{
	ACOUSTIC_ECHO_CANCELER_HALF_DUPLEX_MODE__DISABLE,
	ACOUSTIC_ECHO_CANCELER_HALF_DUPLEX_MODE__ON_START_ONLY,
	ACOUSTIC_ECHO_CANCELER_HALF_DUPLEX_MODE__ALWAYS
	} Tac49xAcousticEchoCancelerHalfDuplexMode;

typedef enum
	{
	ACOUSTIC_ECHO_CANCELER_HALF_DUPLEX_ATTENUATION__5_dB_DEFAULT,
	ACOUSTIC_ECHO_CANCELER_HALF_DUPLEX_ATTENUATION__1_dB,
	ACOUSTIC_ECHO_CANCELER_HALF_DUPLEX_ATTENUATION__2_dB,
	ACOUSTIC_ECHO_CANCELER_HALF_DUPLEX_ATTENUATION__3_dB,
	ACOUSTIC_ECHO_CANCELER_HALF_DUPLEX_ATTENUATION__4_dB,
	ACOUSTIC_ECHO_CANCELER_HALF_DUPLEX_ATTENUATION__5_dB,
	} Tac49xAcousticEchoCancelerHalfDuplexAttenuation;

typedef enum
	{
	ACOUSTIC_ECHO_CANCELER_HANDSET_NLP_DISABLED,
	ACOUSTIC_ECHO_CANCELER_HANDSET_NLP_SENSITIVITY_1,
	ACOUSTIC_ECHO_CANCELER_HANDSET_NLP_SENSITIVITY_2,
	ACOUSTIC_ECHO_CANCELER_HANDSET_NLP_SENSITIVITY_3,
	ACOUSTIC_ECHO_CANCELER_HANDSET_NLP_SENSITIVITY_4,
	ACOUSTIC_ECHO_CANCELER_HANDSET_NLP_SENSITIVITY_5,
	} Tac49xAcousticEchoCancelerHandsetNlpSensitivity;

typedef enum
	{
	ACOUSTIC_ECHO_CANCELER_HPF_AS_DEFINED_IN_ADVANCED_CHANNEL_COMMAND,
	ACOUSTIC_ECHO_CANCELER_HPF_DC_REMOVER,
	ACOUSTIC_ECHO_CANCELER_HPF_HP_FILTER,
	ACOUSTIC_ECHO_CANCELER_HPF_DISABLED,
	} Tac49xAcousticEchoCancellerHighPassFilterMode;

typedef enum
    {
    EMIF_TEST_STATUS__PASS,
    EMIF_TEST_STATUS__FAILED_DUE_TO_PROGRAM_DOWNLOAD,
    EMIF_TEST_STATUS__FAILED_DUE_TO_TEST_RESULT
    } Tac49xEmifTestStatus;

typedef enum
	{
	PACKET_RECORDING_ERROR_TYPE__NONE,
	PACKET_RECORDING_ERROR_TYPE__PACKET_SIZE_EXCEEDED,
	PACKET_RECORDING_ERROR_TYPE__FLUSH_METHOD_NOT_APPLIED
	} Tac49xPacketRecordingErrorType;

typedef enum
	{
	PACKET_DIRECTION__TX,
	PACKET_DIRECTION__RX
	} Tac49xPacketDirection;

/* Ethereal  To Be Included*/
/*
typedef enum
	{
	VBD_EVENTS_RELAY_TYPE__NONE,
	VBD_EVENTS_RELAY_TYPE__RFC2833,
	VBD_EVENTS_RELAY_TYPE__V_150_1_SSE
	}Tac49xVbdEventsRelayType;
*/

typedef enum
	{
	RFC_3558_PACKET_FORMAT__HEADER_FREE,
	RFC_3558_PACKET_FORMAT__BUNDLED,
	RFC_3558_PACKET_FORMAT__COMPACT_BUNDLED
	} Tac49xRfc3558PacketFormat;

typedef enum
	{
	RFC_3558_EVRC_RATE_MODE__0_PERCENTAGE_OF_FULL_RATE_FRAMES__CHANGE_TO_HALF_RATE,
	RFC_3558_EVRC_RATE_MODE__25_PERCENTAGE_OF_FULL_RATE_FRAMES__CHANGE_TO_HALF_RATE,
	RFC_3558_EVRC_RATE_MODE__50_PERCENTAGE_OF_FULL_RATE_FRAMES__CHANGE_TO_HALF_RATE,
	RFC_3558_EVRC_RATE_MODE__75_PERCENTAGE_OF_FULL_RATE_FRAMES__CHANGE_TO_HALF_RATE,
	RFC_3558_EVRC_RATE_MODE__100_PERCENTAGE_OF_FULL_RATE_FRAMES__CHANGE_TO_HALF_RATE
	} Tac49xRfc3558EvrcRateMode;

typedef enum
	{
	RFC_3558_CODER_RATE__0_125=1,
	RFC_3558_CODER_RATE__0_25,
	RFC_3558_CODER_RATE__0_5,
	RFC_3558_CODER_RATE__1
	} Tac49xRfc3558CoderRate;

typedef enum
	{
	RFC_3558_EVRC_B_RATE_MODE__TARGET_BIT_RATE_9300,
	RFC_3558_EVRC_B_RATE_MODE__TARGET_BIT_RATE_8500,
	RFC_3558_EVRC_B_RATE_MODE__TARGET_BIT_RATE_7500,
	RFC_3558_EVRC_B_RATE_MODE__TARGET_BIT_RATE_7000,
	RFC_3558_EVRC_B_RATE_MODE__TARGET_BIT_RATE_6600,
	RFC_3558_EVRC_B_RATE_MODE__TARGET_BIT_RATE_6200,
	RFC_3558_EVRC_B_RATE_MODE__TARGET_BIT_RATE_5800,
	RFC_3558_EVRC_B_RATE_MODE__MAX_RATE_0_5_OPERATION_POINT_0
	} Tac49xRfc3558EvrcBRateMode;
	
typedef enum
	{
	RFC_3558_OPERTING_POINT__0,
	RFC_3558_OPERTING_POINT__1,
	RFC_3558_OPERTING_POINT__2
	} Tac49xRfc3558OperationPoint;

typedef enum
	{
	RFC3558_RATE_MODE_EVENT_REASON__NEW_RATE_MODE_REQUEST_RECEIVED_FROM_REMOTE_SIDE,
	RFC3558_RATE_MODE_EVENT_REASON__NEW_RATE_MODE_REQUEST_SENDED_TO_REMOTE_SIDE
	} Tac49xRfc3558RateModeEventReason;

typedef enum
	{
	AIC_CLK_FREQUENCY__8192_KHZ,
	AIC_CLK_FREQUENCY__12288_KHZ,
	AIC_CLK_FREQUENCY__16384_KHZ,
	AIC_CLK_FREQUENCY__24576_KHZ,
	}Tac49xAicClkFrequency;

typedef enum 
    {
    FAX_AND_DATA_GAIN__DEFAULT_0_DB,
    FAX_AND_DATA_GAIN__minus31_DB,
    FAX_AND_DATA_GAIN__minus30_DB,
    FAX_AND_DATA_GAIN__minus29_DB,
    FAX_AND_DATA_GAIN__minus28_DB,
    FAX_AND_DATA_GAIN__minus27_DB,
    FAX_AND_DATA_GAIN__minus26_DB,
    FAX_AND_DATA_GAIN__minus25_DB,
    FAX_AND_DATA_GAIN__minus24_DB,
    FAX_AND_DATA_GAIN__minus23_DB,
    FAX_AND_DATA_GAIN__minus22_DB,
    FAX_AND_DATA_GAIN__minus21_DB,
    FAX_AND_DATA_GAIN__minus20_DB,
    FAX_AND_DATA_GAIN__minus19_DB,
    FAX_AND_DATA_GAIN__minus18_DB,
    FAX_AND_DATA_GAIN__minus17_DB,
    FAX_AND_DATA_GAIN__minus16_DB,
    FAX_AND_DATA_GAIN__minus15_DB,
    FAX_AND_DATA_GAIN__minus14_DB,
    FAX_AND_DATA_GAIN__minus13_DB,
    FAX_AND_DATA_GAIN__minus12_DB,
    FAX_AND_DATA_GAIN__minus11_DB,
    FAX_AND_DATA_GAIN__minus10_DB,
    FAX_AND_DATA_GAIN__minus9_DB,
    FAX_AND_DATA_GAIN__minus8_DB,
    FAX_AND_DATA_GAIN__minus7_DB,
    FAX_AND_DATA_GAIN__minus6_DB,
    FAX_AND_DATA_GAIN__minus5_DB,
    FAX_AND_DATA_GAIN__minus4_DB,
    FAX_AND_DATA_GAIN__minus3_DB,
    FAX_AND_DATA_GAIN__minus2_DB,
    FAX_AND_DATA_GAIN__minus1_DB,
    FAX_AND_DATA_GAIN__0_DB,
    FAX_AND_DATA_GAIN__plus1_DB,
    FAX_AND_DATA_GAIN__plus2_DB,
    FAX_AND_DATA_GAIN__plus3_DB,
    FAX_AND_DATA_GAIN__plus4_DB,
    FAX_AND_DATA_GAIN__plus5_DB,
    FAX_AND_DATA_GAIN__plus6_DB,
    FAX_AND_DATA_GAIN__plus7_DB,
    FAX_AND_DATA_GAIN__plus8_DB,
    FAX_AND_DATA_GAIN__plus9_DB,
    FAX_AND_DATA_GAIN__plus10_DB,
    FAX_AND_DATA_GAIN__plus11_DB,
    FAX_AND_DATA_GAIN__plus12_DB,
    FAX_AND_DATA_GAIN__plus13_DB,
    FAX_AND_DATA_GAIN__plus14_DB,
    FAX_AND_DATA_GAIN__plus15_DB,
    FAX_AND_DATA_GAIN__plus16_DB,
    FAX_AND_DATA_GAIN__plus17_DB,
    FAX_AND_DATA_GAIN__plus18_DB,
    FAX_AND_DATA_GAIN__plus19_DB,
    FAX_AND_DATA_GAIN__plus20_DB,
    FAX_AND_DATA_GAIN__plus21_DB,
    FAX_AND_DATA_GAIN__plus22_DB,
    FAX_AND_DATA_GAIN__plus23_DB,
    FAX_AND_DATA_GAIN__plus24_DB,
    FAX_AND_DATA_GAIN__plus25_DB,
    FAX_AND_DATA_GAIN__plus26_DB,
    FAX_AND_DATA_GAIN__plus27_DB,
    FAX_AND_DATA_GAIN__plus28_DB,
    FAX_AND_DATA_GAIN__plus29_DB,
    FAX_AND_DATA_GAIN__plus30_DB,
    FAX_AND_DATA_GAIN__plus31_DB
    } Tac49xFaxAndDataGain;

typedef enum
	{
	T38_PACKETIZATION_PERIOD__40_msec,
	T38_PACKETIZATION_PERIOD__5_msec,
	T38_PACKETIZATION_PERIOD__10_msec,
	T38_PACKETIZATION_PERIOD__15_msec,
	T38_PACKETIZATION_PERIOD__20_msec,
	T38_PACKETIZATION_PERIOD__25_msec,
	T38_PACKETIZATION_PERIOD__30_msec,
	T38_PACKETIZATION_PERIOD__35_msec
	} Tac49xT38PacketizationPeriod;

typedef enum
	{
	MEDIA_PROTECTION_INFO_TYPE__AUDIT,
	MEDIA_PROTECTION_INFO_TYPE__STATE
	} Tac49xMediaProtectionInfoType;

typedef enum
	{
	MODULE_TYPE__ECHO_CANCELER
	} Tac49xModuleType;

typedef enum
	{
	DTMF_STEP_RATIO__DEFAULT_DISBALED,
	DTMF_STEP_RATIO__DISBALED,
	DTMF_STEP_RATIO__1_dB,
	DTMF_STEP_RATIO__2_dB,
	DTMF_STEP_RATIO__3_dB,
	DTMF_STEP_RATIO__4_dB,
	DTMF_STEP_RATIO__5_dB,
	DTMF_STEP_RATIO__6_dB,
	DTMF_STEP_RATIO__7_dB,
	DTMF_STEP_RATIO__8_dB,
	DTMF_STEP_RATIO__9_dB,
	DTMF_STEP_RATIO__10_dB
	} Tac49xDtmfStepRatio;

typedef enum
	{
	DTMF_STEP_DISTANCE__DEFAULT_0_msec,
	DTMF_STEP_DISTANCE__0_msec,
	DTMF_STEP_DISTANCE__10_msec,
	DTMF_STEP_DISTANCE__20_msec,
	DTMF_STEP_DISTANCE__30_msec,
	DTMF_STEP_DISTANCE__40_msec,
	DTMF_STEP_DISTANCE__50_msec,
	DTMF_STEP_DISTANCE__60_msec,
	DTMF_STEP_DISTANCE__70_msec,
	DTMF_STEP_DISTANCE__80_msec,
	DTMF_STEP_DISTANCE__90_msec,
	DTMF_STEP_DISTANCE__100_msec,
	DTMF_STEP_DISTANCE__110_msec,
	DTMF_STEP_DISTANCE__120_msec
	} Tac49xDtmfStepDistance;

typedef enum
	{
	TDM_PORT_TYPE__VOICE,
	TDM_PORT_TYPE__CAS,
	TDM_PORT_TYPE__VOICE_CONNECTED_SLOT
	}Tac49xTdmPortType;

typedef enum
	{
	TIME_SLOT_MANIPULATION_COMMAND_TYPE__TDM_SIDE_LOOP,
	TIME_SLOT_MANIPULATION_COMMAND_TYPE__NETWORK_SIDE_LOOP,
	TIME_SLOT_MANIPULATION_COMMAND_TYPE__SWAP_CHANNELS,
	TIME_SLOT_MANIPULATION_COMMAND_TYPE__RESTORE_CONNECTION,
	TIME_SLOT_MANIPULATION_COMMAND_TYPE__SWAP_CHANNELS_SPECIFIC_PORT_TYPE
	} Tac49xTimeSlotManipulationCommandType;

typedef enum
	{
	IPP_CHIP_RESOURCE_494,
	IPP_CHIP_RESOURCE_495L
	} Tac49xIPPChipResourceTypes;

typedef enum
	{
	CAS_BITS_LOCATION__LOW_NIBLE,
	CAS_BITS_LOCATION__HI_NIBLE
	} Tac49xCasBitsLocation;

typedef enum
	{
	SAMPLING_RATE__8_KHz,
	SAMPLING_RATE__16_KHz,
	SAMPLING_RATE__24_KHz
	} Tac49xSamplingRate;

typedef enum
	{
	BYPASS_REDUNDANCY_TYPE__NONE,
	BYPASS_REDUNDANCY_TYPE__RFC_2198
	} Tac49xBypassRedundancyType;

typedef enum
	{
	T38_NO_OPERATION_PERIOD__HANDLER_DISABLE,
	T38_NO_OPERATION_PERIOD__1_sec,
	T38_NO_OPERATION_PERIOD__2_sec,
	T38_NO_OPERATION_PERIOD__3_sec,
	T38_NO_OPERATION_PERIOD__4_sec,
	T38_NO_OPERATION_PERIOD__5_sec,
	T38_NO_OPERATION_PERIOD__6_sec,
	T38_NO_OPERATION_PERIOD__7_sec,
	T38_NO_OPERATION_PERIOD__8_sec,
	T38_NO_OPERATION_PERIOD__9_sec,
	T38_NO_OPERATION_PERIOD__10_sec,
	T38_NO_OPERATION_PERIOD__11_sec,
	T38_NO_OPERATION_PERIOD__12_sec,
	T38_NO_OPERATION_PERIOD__13_sec,
	T38_NO_OPERATION_PERIOD__14_sec,
	T38_NO_OPERATION_PERIOD__15_sec,
	T38_NO_OPERATION_PERIOD__16_sec,
	T38_NO_OPERATION_PERIOD__17_sec,
	T38_NO_OPERATION_PERIOD__18_sec,
	T38_NO_OPERATION_PERIOD__19_sec,
	T38_NO_OPERATION_PERIOD__20_sec,
	T38_NO_OPERATION_PERIOD__21_sec,
	T38_NO_OPERATION_PERIOD__22_sec,
	T38_NO_OPERATION_PERIOD__23_sec,
	T38_NO_OPERATION_PERIOD__24_sec,
	T38_NO_OPERATION_PERIOD__25_sec,
	T38_NO_OPERATION_PERIOD__26_sec,
	T38_NO_OPERATION_PERIOD__27_sec,
	T38_NO_OPERATION_PERIOD__28_sec,
	T38_NO_OPERATION_PERIOD__29_sec,
	T38_NO_OPERATION_PERIOD__30_sec,
	T38_NO_OPERATION_PERIOD__31_sec
	} Tac49xT38NoOperationPeriod;

typedef enum
	{
	RECORD_COMMAND_TYPE__TDM_START,
	RECORD_COMMAND_TYPE__TDM_STOP,
	RECORD_COMMAND_TYPE__NETWORK_START,
	RECORD_COMMAND_TYPE__NETWORK_STOP
	} Tac49xRecordCommandType;

typedef enum
    {
    DUA_FRAMER_COMMAND_MESSAGE_TYPE__DISCARD,
    DUA_FRAMER_COMMAND_MESSAGE_TYPE__START_OPERATION,
    DUA_FRAMER_COMMAND_MESSAGE_TYPE__STOP_OPERATION,
    DUA_FRAMER_COMMAND_MESSAGE_TYPE__CLEAR_BUFFER,
    DUA_FRAMER_COMMAND_MESSAGE_TYPE__SEND_MESSAGE_ONCE,
    DUA_FRAMER_COMMAND_MESSAGE_TYPE__SEND_REPEATED_MESSAGE_UNTIL_DISCARD,
    DUA_FRAMER_COMMAND_MESSAGE_TYPE__LOOPBACK_MESSAGE_FOR_DEBUGGING
    } Tac49xDuaFramerCommandMessageType;

typedef enum
    {
    DUA_FRAMER_MODE__DPNSS,
    DUA_FRAMER_MODE__DASS
    } Tac49xDuaFramerMode;

typedef enum
    {
    DUA_FRAMER_STATUS_MESSAGE_TYPE__FRAME_RECEIVED,
    DUA_FRAMER_STATUS_MESSAGE_TYPE__CRC_ERROR_DETECTED,
    DUA_FRAMER_STATUS_MESSAGE_TYPE__SHORT_FRAME_ERROR,
    DUA_FRAMER_STATUS_MESSAGE_TYPE__FRAME_EXCEED_LIMIT_ERROR,
    DUA_FRAMER_STATUS_MESSAGE_TYPE__BAD_LINK,
    DUA_FRAMER_STATUS_MESSAGE_TYPE__BUFFER_OVERRUN=8,
    DUA_FRAMER_STATUS_MESSAGE_TYPE__DISCARD_ACKNOWLEDGE,
    DUA_FRAMER_STATUS_MESSAGE_TYPE__CLEAR_BUFFER_ACKNOWLEDGE,
    DUA_FRAMER_STATUS_MESSAGE_TYPE__RUN_ACKNOWLDGE,
    DUA_FRAMER_STATUS_MESSAGE_TYPE__START_ACKNOWLDGE,
    DUA_FRAMER_STATUS_MESSAGE_TYPE__STOP__ACKNOWLDGE
    } Tac49xDuaFramerStatusMessageType;

typedef enum
    {
    EVENTS_PORT_TYPE__HOST,
    EVENTS_PORT_TYPE__NET
    } Tac49xEventsPortType;

typedef enum
    {
    RFC3267_REDUNDANCY_LEVEL__NONE,
    RFC3267_REDUNDANCY_LEVEL__1,
    RFC3267_REDUNDANCY_LEVEL__2,
    RFC3267_REDUNDANCY_LEVEL__3,
	RFC3267_REDUNDANCY_LEVEL__4,
	RFC3267_REDUNDANCY_LEVEL__5
    } Tac49xRfc3267RedundancyLevel;

typedef enum
    {
    DEBUG_PACKET_TYPE__SYSTEM=0,
	DEBUG_PACKET_TYPE__ECHO_CANCELER=3,
	DEBUG_PACKET_TYPE__DATA_RELAY,
	DEBUG_PACKET_TYPE__RECORD,
	DEBUG_PACKET_TYPE__PLAYBACK,
	DEBUG_PACKET_TYPE__NOISE_REDUCTION=8,
	DEBUG_PACKET_TYPE__NET_AES,
	DEBUG_PACKET_TYPE__GEN_CNG
    } Tac49xDebugPacketModuleType;

typedef enum
    {
    ECHO_CANCELER_DEBUG_PACKET_PAYLOAD_TYPE__STATICS,
    ECHO_CANCELER_DEBUG_PACKET_PAYLOAD_TYPE__RX_MODULE__REFERENCE_SIGNAL,
    ECHO_CANCELER_DEBUG_PACKET_PAYLOAD_TYPE__RX_MODULE__INPUT_SIGNAL,
    ECHO_CANCELER_DEBUG_PACKET_PAYLOAD_TYPE__RX_MODULE__OUTPUT_SIGNAL,
    ECHO_CANCELER_DEBUG_PACKET_PAYLOAD_TYPE__TX_MODULE__INPUT_SIGNAL,
    ECHO_CANCELER_DEBUG_PACKET_PAYLOAD_TYPE__TX_MODULE__OUTPUT_SIGNAL,
    ECHO_CANCELER_DEBUG_PACKET_PAYLOAD_TYPE__RX_MODULE__CONFIGURATION,
    ECHO_CANCELER_DEBUG_PACKET_PAYLOAD_TYPE__TX_MODULE__CONFIGURATION,
    ECHO_CANCELER_DEBUG_PACKET_PAYLOAD_TYPE__AEC_PARAMETER_STATICS,
    ECHO_CANCELER_DEBUG_PACKET_PAYLOAD_TYPE__AEC_EXTERNAL_MEMORY_STATICS,
	ECHO_CANCELER_DEBUG_PACKET_PAYLOAD_TYPE__ACTIVATION_FLAGS
    } Tac49xEchoCancelerDebugPacketPayloadType;

typedef enum
    {
    RECORD_DEBUG_PACKET_PACKET_PAYLOAD_TYPE__START,
    RECORD_DEBUG_PACKET_PACKET_PAYLOAD_TYPE__DATA,
    RECORD_DEBUG_PACKET_PACKET_PAYLOAD_TYPE__STOP
    } Tac49xRecordDebugPacketPayloadType;

typedef enum
    {
    PLAYBACK_DEBUG_PACKET_PAYLOAD_TYPE__START,
    PLAYBACK_DEBUG_PACKET_PAYLOAD_TYPE__PACKET,
    PLAYBACK_DEBUG_PACKET_PAYLOAD_TYPE__SILENCE,
    PLAYBACK_DEBUG_PACKET_PAYLOAD_TYPE__STOP,
    PLAYBACK_DEBUG_PACKET_PAYLOAD_TYPE__REQUEST,
    PLAYBACK_DEBUG_PACKET_PAYLOAD_TYPE__FRAME,
    PLAYBACK_DEBUG_PACKET_PAYLOAD_TYPE__END
    } Tac49xPlaybackDebugPacketPayloadType;

typedef enum
    {
    ECHO_CANCELER_DEBUG_PACKET_PAYLOAD_FORMAT__NONE_SIGNAL,
    ECHO_CANCELER_DEBUG_PACKET_PAYLOAD_FORMAT__LINEAR_SIGNAL,
    ECHO_CANCELER_DEBUG_PACKET_PAYLOAD_FORMAT__LAW_FORMAT_SIGNAL
    } Tac49xEchoCancelerDebugPacketPayloadFormat;

typedef enum
    {
    TDM_INTERFACE_CONFIGURATION__8KHz,
    TDM_INTERFACE_CONFIGURATION__16KHz__FrameSync8KHz,
	TDM_INTERFACE_CONFIGURATION__16KHz__Mode2
    } Tac49xTdmInterfaceConfiguration;

typedef enum
    {
    DTMF_FREQUENCY_DEVIATION__Default_1_5_Percentage,
    DTMF_FREQUENCY_DEVIATION__1_5_Percentage,
    DTMF_FREQUENCY_DEVIATION__1_6_Percentage,
    DTMF_FREQUENCY_DEVIATION__1_7_Percentage,
    DTMF_FREQUENCY_DEVIATION__1_8_Percentage,
    DTMF_FREQUENCY_DEVIATION__1_9_Percentage,
    DTMF_FREQUENCY_DEVIATION__2_Percentage,
    DTMF_FREQUENCY_DEVIATION__2_1_Percentage
    } Tac49xDtmfFrequencyDeviation;

typedef enum
    {
    G729EV_AUTOMATIC_CONTROL_MODE__LOCAL_RATE_CHANGED_BY_MBS,
    G729EV_AUTOMATIC_CONTROL_MODE__LOCAL_RATE_CHANGED_TO_MINIMUM_FROM_LOCAL_RATE_AND_MBS
    } Tac49xG729EvAutomaticLocalRateControlMode;


typedef enum
    {
	FAX_RELAY_STATE__NONE,
	FAX_RELAY_STATE__SWITCHED_TO_FAX,
	FAX_RELAY_STATE__DURING_FAX
	} Tac49xFaxRelayState;

typedef enum
    {
    G729EV_RATE_8_kbps,
    G729EV_RATE_12_kbps,
    G729EV_RATE_14_kbps,
    G729EV_RATE_16_kbps,
    G729EV_RATE_18_kbps,
    G729EV_RATE_20_kbps,
    G729EV_RATE_22_kbps,
    G729EV_RATE_24_kbps,
    G729EV_RATE_26_kbps,
    G729EV_RATE_28_kbps,
    G729EV_RATE_30_kbps,
    G729EV_RATE_32_kbps,
    G729EV_RATE_NO_DATA = 15,
    G729EV_RATE_NO_MBS = 15
    } Tac49xG729EvRate;

typedef enum
    {
    MFC_R1_DETECTION_STANDARD__ITU,
    MFC_R1_DETECTION_STANDARD__R__1_5
    }Tac49xMfcR1DetectionStandard;

typedef enum 
    {
     DTMF_CALLER_ID_STATUS__DECODER_VALID_END, 
	 DTMF_CALLER_ID_STATUS__INVALID_MESSAGE_LENGTH, 
	 DTMF_CALLER_ID_STATUS__DTMF_STRING_OVERFLOWS_OUTPUT_BUFFER, 
	 DTMF_CALLER_ID_STATUS__INVALID_PARAMETER_TYPE, 
	 DTMF_CALLER_ID_STATUS__INVALID_PARAMETER_DATA, 
	 DTMF_CALLER_ID_STATUS__DECODER_ERRONEOUS_STANDARD, 
	 DTMF_CALLER_ID_STATUS__MISSING_START_CHARACTER, 
	 DTMF_CALLER_ID_STATUS__MISSING_END_CHARACTER, 
	 DTMF_CALLER_ID_STATUS__INVALID_PARAMTER_LENGTH,
	 DTMF_CALLER_ID_STATUS__INVALID_SERVICE_TYPE,
 	 DTMF_CALLER_ID_STATUS__DTMF_ERRONEOUS_CHARACTER,
	 DTMF_CALLER_ID_STATUS__DECODER_RETURN_VALUE_NUM
    } Tac49xDtmfCallerIdStatus;


typedef enum
    {
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__0__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__0_25__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__0_50__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__0_75__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__1__PERCENTAGE,
    
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__1_25__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__1_50__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__1_75__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__2__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__2_25__PERCENTAGE,

    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__2_50__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__2_75__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__3__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__3_25__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__3_50__PERCENTAGE,

    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__3_75__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__4__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__4_25__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__4_50__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__4_75__PERCENTAGE,

    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__5__PERCENTAGE, /*20*/
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__5_5__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__6__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__6_5__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__7__PERCENTAGE,

    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__7_5__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__8__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__8_5__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__9__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__9_5__PERCENTAGE,

    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__10__PERCENTAGE,/*30*/
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__10_5__PERCENTAGE, 
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__11__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__11_5__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__12__PERCENTAGE,

    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__12_5__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__13__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__13_5__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__14__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__14_5__PERCENTAGE,

    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__15__PERCENTAGE,/*40*/
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__16__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__17__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__18__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__19__PERCENTAGE,

    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__20__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__21__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__22__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__23__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__24__PERCENTAGE,

    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__25__PERCENTAGE,/*50*/
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__26__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__28__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__30__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__32__PERCENTAGE,

    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__34__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__36__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__38__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__40__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__42__PERCENTAGE,

    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__44__PERCENTAGE,/*60*/
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__46__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__48__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_THRESHOLD__50__PERCENTAGE
    }Tac49xRfc3267FrameLossRatioThreshold;

typedef enum
    {
    RFC3267_FRAME_LOSS_RATIO_HYSTERESIS__0__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_HYSTERESIS__0_25__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_HYSTERESIS__0_50__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_HYSTERESIS__0_75__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_HYSTERESIS__1__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_HYSTERESIS__1_50__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_HYSTERESIS__2__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_HYSTERESIS__2_50__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_HYSTERESIS__3__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_HYSTERESIS__4__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_HYSTERESIS__5__PERCENTAGE, 
    RFC3267_FRAME_LOSS_RATIO_HYSTERESIS__6__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_HYSTERESIS__7__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_HYSTERESIS__8__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_HYSTERESIS__10__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_HYSTERESIS__13__PERCENTAGE,
    RFC3267_FRAME_LOSS_RATIO_HYSTERESIS__17__PERCENTAGE
    }Tac49xRfc3267FrameLossRatioHysteresis;

typedef enum
    {
    RFC3267_AUTOMATIC_RATE_CONTROL_MODE__LOCAL_RATE_CHANGED_ACCORDING_THE_CMR_OF_REMOTE_SIDE,
    RFC3267_AUTOMATIC_RATE_CONTROL_MODE__LOCAL_RATE_CHANGED_ONLY_IF_HIGHER_THAN_CMR
    }Tac49xRfc3267AutomaticRateControlMode;

typedef enum
    {
    HANDOUT_REPORT__RETURN_FROM_HANDOUT,
    HANDOUT_REPORT__HANDOUT
    }Tac49xHandoutReport;

typedef enum
    {
    _3GPP_IUB_STATUS_REASON__RESPONSE_TO_GET_COMMAND,
    _3GPP_IUB_STATUS_REASON__HEADER_CRC_ERROR,
    _3GPP_IUB_STATUS_REASON__PAYLOAD_CRC_ERROR,
    _3GPP_IUB_STATUS_REASON__INVALID_TFI
    }Tac49x3GppIubStatusReason;

typedef enum
    {
    EXTENDED_IBS_STRING_FORMAT__0,
    EXTENDED_IBS_STRING_FORMAT__1
    } Tac49xExtendedIbsStringFormat;

typedef enum 
	{
	PACKET_TO_PACKET_MODE__SYNCHRONOUS,
	PACKET_TO_PACKET_MODE__ASYNCHRONOUS
	} Tac49xPacketToPacketMode;

typedef enum 
	{
	PACKET_TO_PACKET_SEQUENCE_NUMBER_MODE__SEQUENTIAL,
	PACKET_TO_PACKET_SEQUENCE_NUMBER_MODE__NON_SEQUENTIAL
	} Tac49xPacketToPacketSequenceNumberMode;

typedef enum
	{
	PACKET_TO_PACKET_SSRC_MODE__CONFIGURABLE,
	PACKET_TO_PACKET_SSRC_MODE__UPDATED
	} Tac49xPacketToPacketSsrcMode; 

typedef enum
	{
	RTCP_APP_PACKET_FORMAT__COMPOUND,
	RTCP_APP_PACKET_FORMAT__NOT_COMPOUND
	} Tac49xRtcpAppPacketFormat;

typedef enum 
	{
	BLOCK_NONE_PAYLOAD_TYPE_13__REGARD_PACKET_AS_SID,
	BLOCK_NONE_PAYLOAD_TYPE_13__IGNORE_PACKET
	} Tac49xBlockNonePayloadType13;

typedef enum 
	{
	ACTIVE_CHANNEL_PATTERN__PCM_VALUE_IS_MUTE,
	ACTIVE_CHANNEL_PATTERN__PCM_VALUE_IS_PCM_VALUE_DURING_IDLE
	} Tac49xActiveChannelPattern;

typedef enum 
	{
	ANSWER_DETECT_DIRECTION__ENCODER_INPUT,
	ANSWER_DETECT_DIRECTION__DECODER_OUTPUT
	} Tac49xAnswerDetectDirection;

typedef enum 
	{
	ANSWER_DETECT_MODE__AD,
	ANSWER_DETECT_MODE__VAD
	} Tac49xAnswerDetectMode;

typedef enum 
	{
	AMD_OPERATION__START_DETECTION,
	AMD_OPERATION__START_DETECTION_CALL_PICKUP_EVENT_OCCURED,
	AMD_OPERATION__STOP_DETECTION
	} Tac49xAmdOperation;

typedef enum 
	{
	LATCHING_MODE_DYNAMIC_STRICT,	
	LATCHING_MODE_STRICT
	} Tac49xLatchingMode;

typedef enum 
	{
	JITTER_ESTIMATION_RFC_3550,	
	JITTER_ESTIMATION_MAX_PACKET_JITTER
	} Tac49xJitterEstimationType;

typedef enum 
	{
	RTCP_XR_MODE__DISABLED,
	RTCP_XR_MODE__ENABLED
	} Tac49xRtcpXrMode;

typedef enum 
	{
	RTCP_XR_EVENTS_MODE__ENABLE_VOICE_QUALITY_STATISTICS_BUT_NOT_RTCP_XR_PACKET,
	RTCP_XR_EVENTS_MODE__ENABLE_VOICE_QUALITY_STATISTICS_ON_TX,
	RTCP_XR_EVENTS_MODE__ENABLE_VOICE_QUALITY_STATISTICS_ON_TX_AND_RX
	} Tac49xRtcpXrEventsMode;

typedef enum 
	{
	RTCP_XR_ALERT_MODE__NO_CHANGE_IN_ALERT_MODE,
	RTCP_XR_ALERT_MODE__ENABLE_ALERT,
	RTCP_XR_ALERT_MODE__DISABLE_ALERT
	} Tac49xRtcpXrAlertMode;

typedef enum 
	{
	RTCP_XR_SNR_MODE__REPORT_SNR_LEVELS_BUT_DONT_USE_SNR_FOR_VOICE_QUALITY_CALCULATION,
	RTCP_XR_SNR_MODE__REPORT_SNR_LEVELS_AND_USE_THEM_FOR_VOICE_QUALITY_CALCULATION,
	RTCP_XR_SNR_MODE__IGNORE_SNR_LEVELS_AND_DONT_USE_THEM_FOR_VOICE_QUALITY_CALCULATION
	} Tac49xRtcpXrSnrMode;

typedef enum 
	{
	RTCP_XR_ALERT_STATUS__NO_CHANGE_IN_ALERT_MODE,
	RTCP_XR_ALERT_STATUS__ALERT_STARTED,
	RTCP_XR_ALERT_STATUS__ALERT_STOPPED
	} Tac49xAlertStatus;

typedef enum 
	{
	ECHO_CANCELLER_HIGH_PASS_FILTER__DISABLED,
	ECHO_CANCELLER_HIGH_PASS_FILTER__DC_REMOVER,
	ECHO_CANCELLER_HIGH_PASS_FILTER__HIGH_PASS_FILTER
	} Tac49xEchoCancellerHighPassFilter;

typedef enum 
	{
	_4_WAY_CONFERENCE_LEG_TYPE__IP,
	_4_WAY_CONFERENCE_LEG_TYPE__TDM
	} Tac49x4WayConferenceLegType;

typedef enum 
	{
	_4_WAY_CONFERENCE_STATE__NOT_ACTIVE,
	_4_WAY_CONFERENCE_STATE__ACTIVE	
	} Tac49x4WayConferenceState;

typedef enum 
	{
	_9_WAY_CONFERENCE_LEG_TYPE__TDM,
	_9_WAY_CONFERENCE_LEG_TYPE__IP
	} Tac49x9WayConferenceLegType;

typedef enum 
	{
	_9_WAY_CONFERENCE_SUBOPCODE__ADD_USER,
	_9_WAY_CONFERENCE_SUBOPCODE__UPDATE_USER,
	_9_WAY_CONFERENCE_SUBOPCODE__REMOVE_USER,
	} Tac49x9WayConferenceSubopcode;

typedef enum 
	{
	RTP_TIMESTAMP_RESOLUTION__8KHz,
	RTP_TIMESTAMP_RESOLUTION__16KHz
	} Tac49xRtpTimeStampResolution;

typedef enum 
	{
	UEM_CLIP_TRANSMIT_MODE__MODE_0,
	UEM_CLIP_TRANSMIT_MODE__MODE_1,
	UEM_CLIP_TRANSMIT_MODE__MODE_3=3,
	UEM_CLIP_TRANSMIT_MODE__MODE_4
	} Tac49xUemClipTransmitMode;

typedef enum
	{
	PROTECTION_TEST_ACTION__AES_ENCRYPTION,
	PROTECTION_TEST_ACTION__AES_DECRYPTION,
	PROTECTION_TEST_ACTION__HMAC_AUTHENTICATION,
	PROTECTION_TEST_ACTION__SHA1_HASH,
	PROTECTION_TEST_ACTION__NO_ACTION_DUE_TO_ERROR
	} Tac49xProtectionTestAction;

typedef enum
	{
	PROTECTION_TEST_ERROR__WRONG_KEY_SIZE,
	PROTECTION_TEST_ERROR__WRONG_DATA_SIZE
	} Tac49xProtectionTestError;

typedef enum
	{
	MODULE_DEBUG_PACKETS_TRANSFER_METHOD__TO_DEBUG_PROTOCOL_ON_NETWORK_PORT,
	MODULE_DEBUG_PACKETS_TRANSFER_METHOD__TO_PROPRIETARY_PROTOCOL_ON_HOST_PORT
	} Tac49xModuleDebugPacketsTransferMethod;

typedef enum
	{
	SPRT_PACKETS_PROTOCOL__RTP,
	SPRT_PACKETS_PROTOCOL__DATA_RELAY
	} Tac49xSprtPacketsProtocol;

typedef enum
	{
	MODEM_TRANSMISSION_POWER__3_DBM,
	MODEM_TRANSMISSION_POWER__0_DBM,
	MODEM_TRANSMISSION_POWER__minus3_DBM,
	MODEM_TRANSMISSION_POWER__minus6_DBM,
	MODEM_TRANSMISSION_POWER__minus9_DBM,
	MODEM_TRANSMISSION_POWER__minus12_DBM,
	MODEM_TRANSMISSION_POWER__minus15_DBM,
	MODEM_TRANSMISSION_POWER__minus18_DBM,
	MODEM_TRANSMISSION_POWER__minus21_DBM,
	MODEM_TRANSMISSION_POWER__minus24_DBM,
	MODEM_TRANSMISSION_POWER__minus27_DBM,
	MODEM_TRANSMISSION_POWER__minus30_DBM,
	MODEM_TRANSMISSION_POWER__minus33_DBM,
	MODEM_TRANSMISSION_POWER__minus36_DBM,
	MODEM_TRANSMISSION_POWER__minus39_DBM,
	MODEM_TRANSMISSION_POWER__minus42_DBM
	} Tac49xModemTransmissionPower;

typedef enum
	{
	DATA_RELAY_TRANSMISSION_PCM_LEVEL__MINUS_9_5,
	DATA_RELAY_TRANSMISSION_PCM_LEVEL__MINUS_12,
	DATA_RELAY_TRANSMISSION_PCM_LEVEL__MINUS_15,
	DATA_RELAY_TRANSMISSION_PCM_LEVEL__MINUS_1
	} Tac49xDataRelayTransmissionPcmLevel;

typedef enum 
    {
    DATA_RELAY_BIT_RATE__4800_BPS,
    DATA_RELAY_BIT_RATE__7200_BPS,
    DATA_RELAY_BIT_RATE__9600_BPS,
    DATA_RELAY_BIT_RATE__12000_BPS,
    DATA_RELAY_BIT_RATE__14400_BPS,
	DATA_RELAY_BIT_RATE__16800_BPS,
	DATA_RELAY_BIT_RATE__19200_BPS,
	DATA_RELAY_BIT_RATE__21600_BPS,
	DATA_RELAY_BIT_RATE__24000_BPS,
	DATA_RELAY_BIT_RATE__26400_BPS,
	DATA_RELAY_BIT_RATE__28800_BPS,
	DATA_RELAY_BIT_RATE__31200_BPS,
	DATA_RELAY_BIT_RATE__33600_BPS
    } Tac49xDataRelayBitRate;

typedef enum 
	{
	DATA_RELAY_CLEAR_DOWN_REASON__UNKNOWN,
	DATA_RELAY_CLEAR_DOWN_REASON__PHYSICAL_LAYER_RELEASE,
	DATA_RELAY_CLEAR_DOWN_REASON__ABORT=4,
	DATA_RELAY_CLEAR_DOWN_REASON__ON_HOOK,
	DATA_RELAY_CLEAR_DOWN_REASON__ADMINISTRATIVE=7
	} Tac49xDataRelayClearDownReason;

typedef enum
	{
	SWITCH_TO_MODEM_INITIATING_FLAG__REMOTE_SIDE_EVENT,
	SWITCH_TO_MODEM_INITIATING_FLAG__LOCAL_SIDE_EVENT
	} Tac49xSwitchToModemInitiatingFlag;

typedef enum
	{
	SWITCH_TO_MODEM_REASON_IDENTIFIER_CODE__NULL,
	SWITCH_TO_MODEM_REASON_IDENTIFIER_CODE__ANS=21,
	SWITCH_TO_MODEM_REASON_IDENTIFIER_CODE__ANSam,
	SWITCH_TO_MODEM_REASON_IDENTIFIER_CODE__SLASH_ANS,
	SWITCH_TO_MODEM_REASON_IDENTIFIER_CODE__SLASH_ANSam
	} Tac49xSwitchToModemReasonIdentifierCode;

typedef enum
	{
	MODEM_ENDED_REASON_IDENTIFIER_CODE__LOCAL_SIDE_CARRIER_LOST,
	MODEM_ENDED_REASON_IDENTIFIER_CODE__REMOTE_IP_SIDE_ENDED_MOIP
	} Tac49xModemEndedReasonIdentifierCode;

typedef enum
	{
	MODEM_EVENT_INDEX__MODEM_RELAY_ENDED,
	MODEM_EVENT_INDEX__DEBUG_MODEM_STATUS,
	MODEM_EVENT_INDEX__DATA_PUMP_STATUS,
	MODEM_EVENT_INDEX__GENERAL_PURPOSE_PACKET,
	MODEM_EVENT_INDEX__MOIP_SSE
	} Tac49xModemEventIndex;

typedef enum
	{
	MODEM_RELAY_PACKET_DIRECTION__FROM_NET_VOICE_TO_MOIP,
	MODEM_RELAY_PACKET_DIRECTION__TO_NET_MOIP_TO_VOICE,
	MODEM_RELAY_PACKET_DIRECTION__TO_NET_VOICE_TO_MOIP
	} Tac49xModemRelayPacketDirection;

typedef enum
	{
	MODULATION_TYPE__V32,
	MODULATION_TYPE__V34,
	MODULATION_TYPE__COUNT
	} Tac49xModulationType;

typedef enum
	{
	MODEM_CONNECTION_FAIL_REASON__MODEM_CHANNEL_IS_CONNECTED,
	MODEM_CONNECTION_FAIL_REASON__MODEM_CHANNEL_HAS_LOCALLY_DISCONNECTED,
	MODEM_CONNECTION_FAIL_REASON__REMOTE_MODEM_HAS_DISCONNECTED,
	MODEM_CONNECTION_FAIL_REASON__NO_ANSWER_FROM_REMOTE_MODEM,
	MODEM_CONNECTION_FAIL_REASON__DATA_PUMP_PROTOCOLS_OF_LOCAL_AND_REMOTE_ENDS_INCOMPATIBLE,
	MODEM_CONNECTION_FAIL_REASON__MODEM_FAILED_DURING_TRAINING_PHASE=6,
	MODEM_CONNECTION_FAIL_REASON__MODEM_FAILED_DURING_RETRAIN,
	MODEM_CONNECTION_FAIL_REASON__CARRIER_WAS_LOST,
	MODEM_CONNECTION_FAIL_REASON__MODEM_INACTIVITY_TIMEOUT_IS_UP,
	MODEM_CONNECTION_FAIL_REASON__COUNT
	} Tac49xModemConnectionFailReason;

typedef enum 
	{
	ENDIANNESS__LITTLE_ENDIAN,
	ENDIANNESS__BIG_ENDIAN
	} Tac49xEndianness;

typedef enum 
	{
	PLAYBACK_END_NORMAL,
	PLAYBACK_END_TERMINATE
	} Tac49xPlaybackEndMode;

typedef enum 
	{
	G711_1_TRANSMIT_MODE__R1=1,
	G711_1_TRANSMIT_MODE__R2A,
	G711_1_TRANSMIT_MODE__R2B,
	G711_1_TRANSMIT_MODE__R3
	} Tac49xG7111TransmitMode;

typedef enum
	{
	MODEM_RELAY_STATUS_OR_EVENT_INDEX__ENDED,
	MODEM_RELAY_STATUS_OR_EVENT_INDEX__MOIP_SYS_LOG,
	MODEM_RELAY_STATUS_OR_EVENT_INDEX__DATA_PUMP_STATUS,
	MODEM_RELAY_STATUS_OR_EVENT_INDEX__GENERAL_PURPOSE,
	MODEM_RELAY_STATUS_OR_EVENT_INDEX__MOIP_SSE 
	} Tac49xModemRelayStatusOrEventIndex;

typedef enum
	{
	VARIABLE_RATE_SUPPORT__FIXED_BIT_RATE,
	VARIABLE_RATE_SUPPORT__VARIABLE_BIT_RATE
	} Tac49xVariableRateSupport;

typedef enum
	{
	CONSTANT_RATE_SUPPORT__VARIABLE_BIT_RATE,
	CONSTANT_RATE_SUPPORT__FIXED_BIT_RATE
	} Tac49xConstantRateSupport;

typedef enum
	{
	MAX_SAMPLING_FREQUENCY__8KHZ,
	MAX_SAMPLING_FREQUENCY__12KHZ,
	MAX_SAMPLING_FREQUENCY__16KHZ,
	MAX_SAMPLING_FREQUENCY__RESERVED,
	} Tac49xMaxSamplingFrequency;

typedef enum
	{
	TIMESTAMP_RESOLUTION__8KHZ,
	TIMESTAMP_RESOLUTION__12KHZ,
	TIMESTAMP_RESOLUTION__16KHZ,
	TIMESTAMP_RESOLUTION__24KHZ,
	} Tac49xTimestampResolution;

typedef enum
	{
	JITTER_BUFFER_MODE__NORMAL,
	JITTER_BUFFER_MODE__HIGH_QUALITY,
	JITTER_BUFFER_MODE__LOW_DELAY
	} Tac49xJitterBufferMode;

typedef enum
	{
	FORWARD_CORRECTION_MODE__DISABLED,
	FORWARD_CORRECTION_MODE__ENABLED
	} Tac49xForwardErrorCorrectionMode;

typedef enum
	{
	MODEM_RELAY_HIGH_AVAILABILITY_OPTIONS__DISABLE,
	MODEM_RELAY_HIGH_AVAILABILITY_OPTIONS__SEND_MODEM_END_EVENT,
	MODEM_RELAY_HIGH_AVAILABILITY_OPTIONS__START_IN_MODEM_RELAY_STATE
	} Tac49xModemRelayHighAvailabilityOptions;

typedef enum 
    {
	DATA_RELAY_DEBUG_SERVICE__MOIP_BUFFERS_LOST_DATA=15,
    DATA_RELAY_DEBUG_SERVICE__MODEM_CONNECT=101,
	DATA_RELAY_DEBUG_SERVICE__MODEM_RETRAIN,
	DATA_RELAY_DEBUG_SERVICE__MODEM_RATE_RE,
	DATA_RELAY_DEBUG_SERVICE__MODEM_PDSNR=105,
	DATA_RELAY_DEBUG_SERVICE__MODEM_EC_FILTER,
	DATA_RELAY_DEBUG_SERVICE__MODEM_EQUALIZER_FILTER,
	DATA_RELAY_DEBUG_SERVICE__MODEM_EC_CLEAN_SAMPLES=132,
	DATA_RELAY_DEBUG_SERVICE__MODEM_RX_SYMBOLS,
	DATA_RELAY_DEBUG_SERVICE__MODEM_PARAMETERS=135,
	DATA_RELAY_DEBUG_SERVICE__MODEM_PHASE2,
	DATA_RELAY_DEBUG_SERVICE__MOIP_PHASE1=138,
	DATA_RELAY_DEBUG_SERVICE__MODEM_PHASE1,
	DATA_RELAY_DEBUG_SERVICE__MODEM_PHASE3=141,
	DATA_RELAY_DEBUG_SERVICE__MODEM_EQUALIZER_ERROR=149,
	DATA_RELAY_DEBUG_SERVICE__MODEM_DOWN_BITS,
	DATA_RELAY_DEBUG_SERVICE__MODEM_UP_BITS,
	DATA_RELAY_DEBUG_SERVICE__OUTGOING_DATA_TO_NETWORK=167,
	DATA_RELAY_DEBUG_SERVICE__INCOMING_DATA_FROM_NETWORK,
	DATA_RELAY_DEBUG_SERVICE__SAMPLES_IN=200,
	DATA_RELAY_DEBUG_SERVICE__SAMPLES_OUT,
	DATA_RELAY_DEBUG_SERVICE__MOIP_STATE=501,
	DATA_RELAY_DEBUG_SERVICE__SSE_STATE=600,
	DATA_RELAY_DEBUG_SERVICE__COUNT
	} Tac49xDataRelayDebugService;

typedef enum
	{
	DATA_RELAY_DEBUG_OPCODE__GENERAL_PURPOSE,
	DATA_RELAY_DEBUG_OPCODE__BINARY_DEBUG,
	DATA_RELAY_DEBUG_OPCODE__TEXT_DEBUG
	} Tac49xDataRelayDebugOpcode;

typedef enum
	{
	AUTOMATIC_RETRAIN_RENEGOTIATION_CONTROL__RETRAIN_AND_RENEGOTIATION_DISABLED,
	AUTOMATIC_RETRAIN_RENEGOTIATION_CONTROL__RETRAIN_ENABLED,
	AUTOMATIC_RETRAIN_RENEGOTIATION_CONTROL__RENEGOTIATION_ENABLED
	} Tac49xAutomaticRetrainRenegotiationControl;

typedef enum
	{
	CHANNEL_TDM_MODE__WB_LINEAR,
	CHANNEL_TDM_MODE__NB_A_LAW,
	CHANNEL_TDM_MODE__NB_MU_LAW
	} Tac49xChannelTdmMode;

typedef enum
    {
    BEEP_DETECTION_MODE__DISABLE,
	BEEP_DETECTION_MODE__ENABLE_ONLY_IF_ANSWERING_MACHINE_IS_DETECTED,
	BEEP_DETECTION_MODE__ENABLE_ALWAYS
	} Tac49xBeepDetectionMode;

typedef enum
    {
    BEEP_DETECTION_SENSITIVITY__LOW,
    BEEP_DETECTION_SENSITIVITY__LOW_MEDIUM,
    BEEP_DETECTION_SENSITIVITY__MEDIUM_HI,
    BEEP_DETECTION_SENSITIVITY__HI
    } Tac49xBeepDetectionSensitivity;

typedef enum
    {
	H324M_PACKET_MARKER__SEGMENT,
	H324M_PACKET_MARKER__END_SEGMENT
	} Tac49xH324mPacketMarker;

typedef enum
    {
	PROTECTION_AUDIT_EVENT_TRANSFER_MODE__EVENT_SENT_IN_RESPONSE_TO_REQUEST_COMMAND,
	PROTECTION_AUDIT_EVENT_TRANSFER_MODE__EVENT_SENT_IN_RESPONSE_TO_REQUEST_COMMAND_AND_ON_ERROR_REPORT_INTERVALS,
	} Tac49xProtectionAuditEventTransferMode;

typedef enum
    {
	MEDIA_PROTECTION_INFO_EVENT_TRIGGER__SENT_IN_RESPONSE_TO_COMMAND_GET_MEDIA_PROTECTION_INFO,
	MEDIA_PROTECTION_INFO_EVENT_TRIGGER__SENT_AUTOMATICALLY_BY_CHANNEL_DUE_TO_TIMER_EXPIRE_AND_ERROR_DETECTION,
	MEDIA_PROTECTION_INFO_EVENT_TRIGGER__SENT_IN_RESPONSE_TO_CLOSE_RTP_COMMAND
	} Tac49xMediaProtectionInfoStateEventTrigger;

typedef enum
    {
	RECORD_TDM_LOCATION__BEFORE_ECHO_CANCELER,
	RECORD_TDM_LOCATION__AFTER_ECHO_CANCELER,
	RECORD_TDM_LOCATION__AFTER_ENCODER
	} Tac49xRecordTdmLocation;

typedef enum
    {
	RECORD_NETWORK_LOCATION__TDM_OUTPUT,
	RECORD_NETWORK_LOCATION__DECODER_OUTPUT,
	RECORD_NETWORK_LOCATION__BEFORE_DECODER
	} Tac49xRecordNetworkLocation;

typedef enum
    {
	ENERGY_DETECTION_DIRECTION__ENCODER_INPUT,
	ENERGY_DETECTION_DIRECTION__DECODER_OUTPUT
	} Tac49xEnergyDetectionDirection;

typedef enum
    {
	ENERGY_DETECTION_THRESHOLD__minus46_DBM,
	ENERGY_DETECTION_THRESHOLD__minus38_DBM,
	ENERGY_DETECTION_THRESHOLD__minus32_DBM,
	ENERGY_DETECTION_THRESHOLD__minus24_DBM,
	ENERGY_DETECTION_THRESHOLD__minus18_DBM,
	ENERGY_DETECTION_THRESHOLD__minus12_DBM,
	ENERGY_DETECTION_THRESHOLD__minus6_DBM,
	ENERGY_DETECTION_THRESHOLD__0_DBM
	} Tac49xEnergyDetectionThreshold;

typedef enum
    {
	DEVICE_STATUS_INTERVAL_RESOLUTION__100_msec,
	DEVICE_STATUS_INTERVAL_RESOLUTION__1_msec
	} Tac49xDeviceStatusIntervalResolution; 

typedef enum
    {
	RTCP_XR_VOICE_QUALITY_EVENT_TYPE__RESPONSE_TO_QUERY,
	RTCP_XR_VOICE_QUALITY_EVENT_TYPE__PERIODIC_VOICE_QUALITY_REPORT,
	RTCP_XR_VOICE_QUALITY_EVENT_TYPE__CLOSE_CHANNEL_REPORT,
	RTCP_XR_VOICE_QUALITY_EVENT_TYPE__REMOTE_SIDE_REPORT,
	RTCP_XR_VOICE_QUALITY_EVENT_TYPE__REPORT_BEFORE_RESYNC,
	} Tac49xRtcpXrVoiceQualityEventType;

typedef enum
    {
	ECHO_CANCELER_CODEC_INTERFACE__SPEAKER,
	ECHO_CANCELER_CODEC_INTERFACE__HANDSET,
	ECHO_CANCELER_CODEC_INTERFACE__HEADSET,
	ECHO_CANCELER_CODEC_INTERFACE__COUNT
	} Tac49xEchoCancelerCodecInterface;


typedef enum
    {
	RERL_MODE__REPORT_LOCAL_RERL_LEVEL_BUT_DONT_USE_REMOTE_RERL_FOR_VOICE_QUALITY_CALCULATION,
	RERL_MODE__REPORT_LOCAL_RERL_LEVEL_AND_USE_REMOTE_RERL_FOR_VOICE_QUALITY_CALCULATION,
	RERL_MODE__IGNORE_RERL_LEVELS_AND_DONT_USE_REMOTE_RERL_FOR_VOICE_QUALITY_CALCULATION,
	} Tac49xRerlMode;

typedef enum
    {
	MOS_TYPE__REPORT_MOS_FROM_BEGINNING_OF_SESSION,
	MOS_TYPE__REPORT_MOS_FROM_LAST_INTERVAL,
	} Tac49xMosType;

typedef enum
	{
	V34_FAX_BAUD_RATE_DISABLER__ALL_RATES_ARE_ALLOWED_FOR_RECIEVE_AND_TRANSMIT_OPERATIONS,
	V34_FAX_BAUD_RATE_DISABLER__3429_RATE_IS_UNSUPPORTED,
	V34_FAX_BAUD_RATE_DISABLER__3429_RATE_IS_UNSUPPORTED_AND_3200_RATE_IS_DISALLOWED_FOR_TRANSMISSION,
	V34_FAX_BAUD_RATE_DISABLER__3429_RATE_IS_UNSUPPORTED_AND_3200_AND_3000_RATES_ARE_DISALLOWED_FOR_TRANSMISSION,
	} Tac49xV34FaxBaudRateDisabler;

typedef enum
	{
	ECHO_CANCELER_OPERATION_SELECT_MODE__LINE_ECHO_CANCELER,
	ECHO_CANCELER_OPERATION_SELECT_MODE__ACOUSTIC_ECHO_CANCELER,
	ECHO_CANCELER_OPERATION_SELECT_MODE__ACOUSTIC_ECHO_SUPPRESSOR
	} Tac49xEchoCancelerOperationSelectMode;

typedef enum
	{
	ECHO_CANCELER_ATTENUATED_REFERENCE__DISABLED,
	ECHO_CANCELER_ATTENUATED_REFERENCE__minus10_DB,
	ECHO_CANCELER_ATTENUATED_REFERENCE__minus16_DB,
	ECHO_CANCELER_ATTENUATED_REFERENCE__minus22_DB
	} Tac49xEchoCancelerAttenuatedReference;

typedef enum
	{
	DENIAL_OF_SERVICE_WINDOW_SIZE__100_msec,
	DENIAL_OF_SERVICE_WINDOW_SIZE__300_msec,
	DENIAL_OF_SERVICE_WINDOW_SIZE__600_msec,
	DENIAL_OF_SERVICE_WINDOW_SIZE__1000_msec,
	DENIAL_OF_SERVICE_WINDOW_SIZE__1500_msec,
	DENIAL_OF_SERVICE_WINDOW_SIZE__2000_msec,
	DENIAL_OF_SERVICE_WINDOW_SIZE__2500_msec,
	DENIAL_OF_SERVICE_WINDOW_SIZE__3000_msec
	} Tac49xDenialOfServiceWindowSize;

typedef enum
	{
	DENIAL_OF_SERVICE_BANDWIDTH__0_KBPS,
	DENIAL_OF_SERVICE_BANDWIDTH__100_KBPS,
	DENIAL_OF_SERVICE_BANDWIDTH__200_KBPS,
	DENIAL_OF_SERVICE_BANDWIDTH__300_KBPS,
	DENIAL_OF_SERVICE_BANDWIDTH__400_KBPS,
	DENIAL_OF_SERVICE_BANDWIDTH__500_KBPS,
	DENIAL_OF_SERVICE_BANDWIDTH__600_KBPS,
	DENIAL_OF_SERVICE_BANDWIDTH__700_KBPS,	
	DENIAL_OF_SERVICE_BANDWIDTH__800_KBPS,
	DENIAL_OF_SERVICE_BANDWIDTH__900_KBPS,
	DENIAL_OF_SERVICE_BANDWIDTH__1000_KBPS,
	DENIAL_OF_SERVICE_BANDWIDTH__1100_KBPS,
	DENIAL_OF_SERVICE_BANDWIDTH__1200_KBPS,
	DENIAL_OF_SERVICE_BANDWIDTH__1300_KBPS,
	DENIAL_OF_SERVICE_BANDWIDTH__1400_KBPS,
	DENIAL_OF_SERVICE_BANDWIDTH__1500_KBPS
	} Tac49xDenialOfServiceBandWidth;

typedef enum
	{
	DENIAL_OF_SERVICE_ATTACK_PROPERTY__RTP,
	DENIAL_OF_SERVICE_ATTACK_PROPERTY__RTCP,
	DENIAL_OF_SERVICE_ATTACK_PROPERTY__T38
	} Tac49xDenialOfServiceAttackPort;

typedef enum
	{
	T38_ENCAPSULATION_INTO_RTP__DISABLE,
	T38_ENCAPSULATION_INTO_RTP__ENABLE_T38_IFP_ENCAPSULATION_INTO_RTP_PER_T38_RECORDING,
	T38_ENCAPSULATION_INTO_RTP__ENABLE_T38_UDPTL_OVER_RTP
	} Tac49xT38EncapsulationIntoRtp;

typedef enum
    {
	ARTIFICIAL_BANDWIDTH_EXTENSION_DIRECTION__DECODER_OUTPUT,
	ARTIFICIAL_BANDWIDTH_EXTENSION_DIRECTION__ENCODER_INPUT
	} Tac49xArtificialBandwidthExtensionDirection;

typedef	enum
	{
	ECHO_CANCELLER_RESET_CONTROL_ON_EVERY_NETWORK_UPDATE,
	ECHO_CANCELLER_RESET_CONTROL_ONLY_ON_NETWORK_MODIFICATION,
	} Tac49xEchoCancellerResetControl;

typedef enum
	{
	RFC4733_TIMESTAMP_INCREMENT__ACCORDING_TO_NB_FRAME_SIZE,
	RFC4733_TIMESTAMP_INCREMENT__ACCORDING_TO_WB_FRAME_SIZE
	} Tac49xRfc4733TimestampIncrement;

typedef enum
	{
	SPEEX_ENCODER_NB_MODE__2_15_KBPS = 1,
	SPEEX_ENCODER_NB_MODE__5_95_KBPS,
	SPEEX_ENCODER_NB_MODE__8_00_KBPS,
	SPEEX_ENCODER_NB_MODE__11_00_KBPS,
	SPEEX_ENCODER_NB_MODE__15_00_KBPS,
	SPEEX_ENCODER_NB_MODE__18_20_KBPS,
	SPEEX_ENCODER_NB_MODE__24_60_KBPS,
	SPEEX_ENCODER_NB_MODE__3_95_KBPS
	} Tac49xSpeexEncoderNbMode;

typedef enum
	{
	OPUS_PACKET_DURATION__2_5_MS,
	OPUS_PACKET_DURATION__5_MS,
	OPUS_PACKET_DURATION__10_MS,
	OPUS_PACKET_DURATION__20_MS,
	OPUS_PACKET_DURATION__40_MS,
	OPUS_PACKET_DURATION__60_MS,
	OPUS_PACKET_DURATION__80_MS,
	OPUS_PACKET_DURATION__120_MS,
	OPUS_PACKET_DURATION__RESERVED
	} Tac49xOpusPacketDuration;

typedef enum
	{
	SILK_PACKET_DURATION__20_MS,
	SILK_PACKET_DURATION__40_MS,
	SILK_PACKET_DURATION__60_MS,
	SILK_PACKET_DURATION__80_MS,
	SILK_PACKET_DURATION__100_MS,
	SILK_PACKET_DURATION__120_MS,
	SILK_PACKET_DURATION__RESERVED
	} Tac49xSilkPacketDuration;

typedef enum
	{
	OPUS_PACKET_INPUT_MODE__MONO,
	OPUS_PACKET_INPUT_MODE__STEREO
	} Tac49xOpusPacketInputMode;

typedef enum
	{
	OPUS_PACKET_INPUT_BANDWIDTH__RESERVED,
	OPUS_PACKET_INPUT_BANDWIDTH__NARROWBAND,
	OPUS_PACKET_INPUT_BANDWIDTH__MEDIUMBAND,
	OPUS_PACKET_INPUT_BANDWIDTH__WIDEBAND,
	OPUS_PACKET_INPUT_BANDWIDTH__SUPERWIDEBAND,
	OPUS_PACKET_INPUT_BANDWIDTH__FULLBAND,
	} Tac49xOpusPacketInputBandwidth;

typedef enum
	{
	OPUS_PACKET_INPUT_OPERATION_MODE__SILK_ONLY,
	OPUS_PACKET_INPUT_OPERATION_MODE__HYBRID,
	OPUS_PACKET_INPUT_OPERATION_MODE__CELT_ONLY,
	} Tac49xOpusPacketInputOperationMode;

typedef enum
	{
	SPEEX_ENCODER_WB_MODE__3_95_KBPS,
	SPEEX_ENCODER_WB_MODE__5_75_KBPS,
	SPEEX_ENCODER_WB_MODE__7_75_KBPS,
	SPEEX_ENCODER_WB_MODE__9_80_KBPS,
	SPEEX_ENCODER_WB_MODE__12_80_KBPS,
	SPEEX_ENCODER_WB_MODE__16_80_KBPS,
	SPEEX_ENCODER_WB_MODE__20_60_KBPS,
	SPEEX_ENCODER_WB_MODE__23_80_KBPS,
	SPEEX_ENCODER_WB_MODE__27_80_KBPS,
	SPEEX_ENCODER_WB_MODE__34_20_KBPS,
	SPEEX_ENCODER_WB_MODE__42_20_KBPS
	} Tac49xSpeexEncoderWbMode;

typedef enum 
	{
	NOISE_REDUCTION_THRESHOLD_DESIGN__DESIGNED_FOR_STATIONARY_NOISE_SIGNALS,
	NOISE_REDUCTION_THRESHOLD_DESIGN__DESIGNED_FOR_LESS_STATIONARY_NOISE_SIGNALS,
	NOISE_REDUCTION_THRESHOLD_DESIGN__DESIGNED_FOR_LOWEST_ACCEPTABLE_STATIONARY_NOISE_SIGNALS
	} Tac49xNoiseReductionThresholdDesign;

typedef enum 
	{
	NOISE_REDUCTION_MINIMUM_GAIN__minus6_DB,
	NOISE_REDUCTION_MINIMUM_GAIN__minus7_DB,
	NOISE_REDUCTION_MINIMUM_GAIN__minus8_DB,
	NOISE_REDUCTION_MINIMUM_GAIN__minus9_DB,
	NOISE_REDUCTION_MINIMUM_GAIN__minus10_DB,
	NOISE_REDUCTION_MINIMUM_GAIN__minus11_DB,
	NOISE_REDUCTION_MINIMUM_GAIN__minus12_DB
	} Tac49xNoiseReductionMinimumGain;

typedef enum 
	{
	JITTER_BUFFER_VAD_DECISION__SILENCE,
	JITTER_BUFFER_VAD_DECISION__VOICE
	} Tac49xJitterBufferVadDecision;

typedef enum 
	{
	T38_MONITOR_CALL_RESULT__NO_RESULT,
	T38_MONITOR_CALL_RESULT__ERROR_FREE,
	T38_MONITOR_CALL_RESULT__WARNING,
	T38_MONITOR_CALL_RESULT__ERROR,
	T38_MONITOR_CALL_RESULT__NO_T38_SIGNALS,
	T38_MONITOR_CALL_RESULT__AUDIO
	} Tac49xT38MonitorCallResult;

typedef enum 
	{
	T38_MONITOR_FOM__NOT_STARTED,
	T38_MONITOR_FOM__MAX_SPEED_AND_ERROR_FREE,
	T38_MONITOR_FOM__MAX_SPEED_WITH_ERRORS,
	T38_MONITOR_FOM__MAX_SPEED_AND_SEVERELY_ERRORED,
	T38_MONITOR_FOM__LOW_SPEED_AND_ERROR_FREE,
	T38_MONITOR_FOM__LOW_SPEED_WITH_ERRORS,
	T38_MONITOR_FOM__LOW_SPEED_AND_SEVERELY_ERRORED,
	T38_MONITOR_FOM__INCOMPLETE
	} Tac49xT38MonitorFom;


typedef enum 
	{
	T38_MONITOR_BIT_RATE__UNDEFINED_OR_T30_CONTROL_SIGNALS,
	T38_MONITOR_BIT_RATE__DATA_SIGNALING_RATE_IN_2400_BPS_UNITS
	} Tac49xT38MonitorBitRate;

typedef enum 
	{
	T38_MONITOR_MERIT__ABSENT,
	T38_MONITOR_MERIT__T38_ERROR_FREE,
	T38_MONITOR_MERIT__T38_WARNING,
	T38_MONITOR_MERIT__T38_ERROR,
	T38_MONITOR_MERIT__T38_PROBLEMS
	} Tac49xT38MonitorMerit;


typedef enum 
	{
	T38_MONITOR_EMITTER__LOCAL_GW,
	T38_MONITOR_EMITTER__REMOTE_GW
	} Tac49xT38MonitorEmitter;


typedef enum 
	{
	VOICE_ENHANCEMENT_OVERUSE_DEFAULT,
	VOICE_ENHANCEMENT_OVERUSE_MOST,
	VOICE_ENHANCEMENT_OVERUSE_HIGH,
	VOICE_ENHANCEMENT_OVERUSE_MEDIUM,
	VOICE_ENHANCEMENT_OVERUSE_LOW,
	VOICE_ENHANCEMENT_OVERUSE_LEAST
	} Tac49xVoiceEnhancementOveruseFactor;

typedef enum
	{
	VOICE_ENHANCEMENT_RTCP_TMMBR,
	VOICE_ENHANCEMENT_RTCP_EXTENSION,
	} Tac49xVoiceEnhancementFeedbackMode;

/******************************************************/
/***** Transmit/Receive Packet inputs structures ******/
/******************************************************/

    /**
     * @defgroup Tac49xTxPacketParams Tac49xTxPacketParams
     * @ingroup PACKET_HANDLING_STRUCTURES
     * @brief
     * This structure is used as an input to the ::Ac49xTransmitPacket function.
     * @{
     */

    /**
     * <BR>
     * This structure is used as an input to the function ::Ac49xTransmitPacket.
     */

typedef struct
	{
	int						Device;							/*!< [in]*/
	int						Channel;						/*!< [in]*/
	Tac49xProtocol			Protocol;						/*!< [in] ::Tac49xProtocol.*/
	Tac49xTransferMedium	TransferMedium;					/*!< [in] ::Tac49xTransferMedium.*/
	char					*pOutPacket;					/*!< [in] Pointer to the transmitted packet (located 4 bytes before the beginning of the packet). Note that this function writes to these 4 bytes the packet header before the packet is sent.*/
	int						PacketSize;						/*!< [in] Size of the packet in bytes.*/
	Tac49xControl			UdpChecksumIncluded;			/*!< [in] 0 = UDP checksum isn't added. 1 = 2 bytes of the UDP checksum are added to the packet before the RTP/ATM header.*/
	Tac49xControl			FavoriteStream;					/*!< [in] Favorite Stream. When the channel receives media packets from multiple sources. This bit notifies the favorite stream (if exists). @n 0 = This packet does not belong to the favorite stream. @n 1= This packet belongs to the favorite stream.*/
	int						BufferDescriptorCurrentIndex;   /*!< [out] The function ::Ac49xTransmitPacket sets this field to the current index of the Buffer Descriptor. */
	} Tac49xTxPacketParams;

    /** @} */

    /**
     * @defgroup Tac49xRxPacketParams Tac49xRxPacketParams
     * @ingroup PACKET_HANDLING_STRUCTURES
     * @brief
     * This structure is used as an input to the ::Ac49xReceivePacket function.
     * @{
     */

    /**
     * <BR>
     * This structure is used as an input to the ::Ac49xReceivePacket function.
     */

typedef struct
	{
	int						Device;							/*!< [in]*/
	Tac49xTransferMedium	TransferMedium;					/*!< [in]  ::Tac49xTransferMedium.*/
	int						Channel;						/*!< [out]*/
	Tac49xProtocol			Protocol;						/*!< [out] ::Tac49xProtocol.*/
	int						BufferDescriptorCurrentIndex;	/*!< [out] Index of the current packet in the buffer descriptors.*/
	char					*pInPacket;						/*!< [in]  Pointer to the location to which the packet is written (the first 4 bytes contain the HPI header).*/
	int						PacketSize;						/*!< [out] Packet size in bytes (include HPI header).*/
	int						PayloadSize;					/*!< [out] Payload size in bytes (exclude HPI header).*/
	U32						PacketAddress;					/*!< [out] The address of the received packet in the internal AC49x HPI (used for debugging).*/
	} Tac49xRxPacketParams;

    /** @} */

/***************************************************/
/********************   HCRF   *********************/
/***************************************************/
typedef enum
	{
	HCRF_COMMAND_REGISTER_INDEX__CORE_HALT=1,
	HCRF_COMMAND_REGISTER_INDEX__CORE_RESUME,
	HCRF_COMMAND_REGISTER_INDEX__CORE_RELOAD,
	HCRF_COMMAND_REGISTER_INDEX__COMPARE_LOCAL_MEMORY_TO_REFERENCE,
	HCRF_COMMAND_REGISTER_INDEX__GET_CPU_REGISTERS,
	HCRF_COMMAND_REGISTER_INDEX__GET_TRACE_BUFFER,
	HCRF_COMMAND_REGISTER_INDEX__SET_CPU_REGISTERS,
	HCRF_COMMAND_REGISTER_INDEX__CORE_CRASH
	} Tac49xHcrfCommandRegisterIndex;

typedef enum
	{
	HCRF_STATUS_REGISTER_INDEX__HALT_ACKNOWLEDGE=1,
	HCRF_STATUS_REGISTER_INDEX__RESUME_ACKNOWLEDGE,
	HCRF_STATUS_REGISTER_INDEX__RELOAD_COMPLETED,
	HCRF_STATUS_REGISTER_INDEX__RELOAD_FAILED,
	HCRF_STATUS_REGISTER_INDEX__LOCAL_MEMORY_TO_REFERENCE_COMPARISON_FIT,
	HCRF_STATUS_REGISTER_INDEX__LOCAL_MEMORY_TO_REFERENCE_COMPARISON_DIFFER,
	HCRF_STATUS_REGISTER_INDEX__CPU_REGISTERS_READY,
	HCRF_STATUS_REGISTER_INDEX__TRACE_BUFFER_READY
	} Tac49xHcrfStatusRegisterIndex;

typedef enum
	{
	HCRF_CONTEXT__COMMAND_REGISTER,
	HCRF_CONTEXT__STATUS_REGISTER
	} Tac49x_HcrfContext;

typedef struct 
    {
	U16 ReferenceCoreId;   
	Tac49xHcrfCommandRegisterIndex CommandIndex;
    } Tac49xHcrfCommandRegisterAttr; 

typedef struct 
    {
	U16							  WatchdogTimeoutFlag;   
	Tac49xHcrfStatusRegisterIndex StatusIndex;   
    } Tac49xHcrfStatusRegisterAttr; 

/************STATUS*********************************/
/***************************************************/
/**   Packet Structures  (Bit Field regardless)   **/
/***************************************************/
/***************************************************/
typedef volatile struct 
    {
	Tac49xHpiPacketHeader				HpiHeader;
	Tac49xProprietaryPacketHeader		ProprietaryHeader;
    Tac49xSignalCommonAttributes        CommonAttributes; 
	Tac49xCallProgressSignal            Signal[CALL_PROGRESS_AND_USER_DEFINED_TONES_SIGNAL_QUANTITY];
    Tac49xPaddingForFutureNeed _;
    } Tac49xCallProgressDeviceConfigurationPacket;

typedef volatile struct 
    {
	Tac49xHpiPacketHeader				HpiHeader;
	Tac49xProprietaryPacketHeader		ProprietaryHeader;
    Tac49xSignalCommonAttributes        CommonAttributes; 
	Tac49xUserDefinedToneSignal         Signal[CALL_PROGRESS_AND_USER_DEFINED_TONES_SIGNAL_QUANTITY];
    Tac49xPaddingForFutureNeed _;
    } Tac49xUserDefinedTonesDeviceConfigurationPacket;


typedef volatile struct
    {
    union
        {
/*
        Tac49xRegularVoicePayload           RegularVoice;
        Tac49xSidOrSidWithVoicePayload      SidOrSidWithVoice;
        Tac49xBadFrameInterpolationPayload  BadFrameInterpolation;
        Tac49xFaxBypassPayload              FaxBypass;
        Tac49xDataBypassPayload             DataBypass;
        Tac49xFaxPayload                    Fax;
        Tac49xDataPayload                   Data;
        Tac49xCasPayload                    Cas;
        Tac49xIbsPayload                    Ibs;
        Tac49xPcmPayload                    Pcm;
        Tac49xPlaybackPayload               Playback;               
*/
        U8 Payload[MAX_PAYLOAD_SIZE];
        } u;
    } Tac49xNetworkOrMediaPayload;


typedef volatile struct
    {
    union
        {
		Tac49xGetMediaProtectiontInfoPayload			GetMediaProtectiontInfo;
		Tac49xSs7Mtp1CommandPayload						Ss7Mtp1;
		Tac49xUtopiaStatusRequestPayload				UtopiaStatusRequest;
		Tac49xBerTestCommandPayload						BerTest;
		Tac49xTrunkTestingMeasurementCommandPayload		TrunkTestingMeasurement;
		Tac49xExtendedIbsStringCommandPayload			ExtendedIbsString;
        Tac49xIbsStringCommandPayload					IbsString;
        Tac49xCasStringCommandPayload					CasString;   
        Tac49xCasGenerateCommandPayload					Cas;
        Tac49xHdlcFramerMessageCommandPayload			HdlcFramerMessage;
/*      Tac49xPlaybackCommandPayload					Playback;*/
        Tac49xCallerIdPayload							CallerId; 
		Tac49xSetRtcpParametersPayload					SetRtcpParameters;
		Tac49xSet3GppUserPlaneRateControlPayload		Set3GppUserPlaneRateControl;
		Tac49xSetRfc3267AmrRateControlPayload			SetRfc3267AmrRateControl;
		Tac49xTandemFreeOperationPayload				TandemFreeOperation;
		Tac49xSetRfc3558RateModePayload					SetRfc3558RateMode;
		Tac49xTimeSlotManipulationCommandPayload		TimeSlotManipulation;
		Tac49xModuleStatusRequestPayload				ModuleStatusRequest;
		Tac49xRtcpAppCommandPayload						RtcpApp;
        Tac49xDuaFramerCommandPayload					DuaFramer;
        Tac49xEventsControlCommandPayload				EventsControl;
        Tac49xNtpTimeStampCommandPayload				NtpTimeStamp;
        Tac49xVideoAudioSyncMsgPayload					VideoAudioSync;
        Tac49xG729EvRateControlCommandPayload			G729EvRateControl;
		Tac49xAmdCommandPayload							Amd;
		Tac49xActivateFaxRelayPayload					ActivateFaxRelay;
		Tac49xGet4WayConferenceDetailsCommandPayload	Get4WayConferenceDetails;
		Tac49xProtectionTestCommandPayload				ProtectionTest;
		Tac49xSwitchToModemCommandPayload				SwitchToModem;
		Tac49xModemRelayEndedCommandPayload				ModemRelayEnded;
		Tac49xModemRelaySseCommandPayload				ModemRelaySse;
		Tac49xUpdateIbsGenerationLevelCommandPayload	UpdateIbsGenerationLevel;
		Tac49xProprietaryRecordCommandPayload			Record;
		Tac49xMuteCngCommandPayload						MuteCng;
        U8 Payload[MAX_PAYLOAD_SIZE];
        } u;
    } Tac49xCommandPayload;

    /**
     * @defgroup Tac49xStatusOrEventPayload Tac49xStatusOrEventPayload
     * @ingroup PACKET_STRUCTURES
     * @brief
     * This structure is a union of all available status and event packets transmitted by the AC49x VoPP.
     * @{
     */

    /**
     * <BR>
     * This structure is a union of all available status and event packets transmitted by the AC49x VoPP.
     */

typedef volatile struct
    {
    union
        {
		Tac49xSs7Mtp1StatusPayload						Ss7Mtp1tStatus;
		Tac49xUtopiaStatusPayload						UtopiaStatus;
		Tac49xBerTestReportEventPayload					BerTestReportEvent;
		Tac49xTrunkTestingMeasurementEventPayload		TrunkTestingMeasurementEvent;
        Tac49xSystemErrorEventPayload					SystemErrorEvent;
        Tac49xGeneralEventPayload						GeneralEvent;
        Tac49xIbsEventPayload							IbsEvent;
        Tac49xCasEventPayload							CasEvent;
        Tac49xHdlcFramerMessageStatusPayload			HdlcFramerMessageStatus;
        Tac49xFaxRelayStatusPayload						FaxRelayStatus;
        Tac49xJitterBufferStatusPayload					JitterBufferStatus;
        Tac49xCallerIdPayload							CallerId;     
		Tac49xPacketizerStatusOrEventPayload			Packetizer;
		Tac49xPacketizerStatusOrEvent_Format1_Payload	Packetizer_Format1;
		Tac49xDeviceStatusPayload						DeviceStatus;
		Tac49xDebugInformationStatusPayload				DebugInformationStatus; /*!< Reserved for internal use.*/
		Tac49xFaxOrDataSignalStatusPayload				FaxOrDataStatus;
		Tac49xTandemFreeOperationPayload				TandemFreeOperation;
		Tac49xUtopiaTestErrorEventPayload				UtopiaTestErrorEvent;
		Tac49xModuleStatusPayload						ModuleStatus;
        Tac49xDuaFramerStatusPayload					DuaFramerStatus;
        Tac49xVideoAudioSyncMsgPayload					VideoAudioSync;
		Tac49x4WayConferenceStatusPayload				_4WayConferenceStatus;
		Tac49x4WayConferenceDetailsEventPayload			_4WayConferenceDetails;
		Tac49xProtectionTestResponseEventPayload		ProtectionTestResponse;
		Tac49xRandomNumberResponseEventPayload			RandomNumberResponse;
		Tac49xModuleDebugEventPayload					ModuleDebug;
		Tac49xModemRelayEventPayload					ModemRelay;
		Tac49xProprietaryRecordEventPayload				Record;
		Tac49xT38MonitorEventPayload					T38Monitor;
#if (  (AC49X_DEVICE_TYPE == AC494_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC495_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC495L_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC496_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC496E_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC497_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC494E_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC495E_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC496D_DEVICE)\
    )
		Tac49xUsbSynchronizationStatus					UsbSynchronizationStatus;
#endif
		U8 Payload[MAX_PAYLOAD_SIZE];
        } u;
    } Tac49xStatusOrEventPayload;

    /** @} */


typedef volatile struct 
    {
	Tac49xNetworkOrMediaPayload	NetworkOrMedia;
    } Tac49xRtpOrAtmPayload;


    /**
     * @defgroup Tac49xProprietaryPayload Tac49xProprietaryPayload
     * @ingroup PACKET_STRUCTURES
     * @brief
     * This structure defines the proprietary header of status and event packets transmitted by the AC49x VoPP.
     * @{
     */

    /**
     * <BR>
     * This structure defines the proprietary header of status and event packets transmitted by the AC49x VoPP.
     * This structure is also @b internally used for sending configuration commands to the AC49x VoPP.
     */

typedef volatile struct 
    {
    Tac49xProprietaryPacketHeader	ProprietaryHeader;
    union
        {
		Tac49xCommandPayload		Command;
		Tac49xStatusOrEventPayload  StatusOrEvent;
		} u;
    } Tac49xProprietaryPayload;

    /** @} */

    /**
     * @defgroup Tac49xPacket Tac49xPacket
     * @ingroup PACKET_STRUCTURES
     * @brief
     * This structure defines an AC49x VoPP packet.
     * @{
     */

    /**
     * <BR>
     * This structure defines an AC49x VoPP packet. It should be used by the host when the host receives
     * a packet. It is also @b internally used to transmit packets to the AC49x.
     * It contains HpiHeader and a union of RtpOrAtmPayload and ProprietaryPayload.
     */

typedef volatile struct 
    {
    Tac49xHpiPacketHeader		    HpiHeader;            /*!< Defines if the packet is an HPI header packet.*/
    union
        {
        Tac49xRtpOrAtmPayload		 RtpOrAtmPayload;     /*!< Defines if the packet contains an RTP or ATM payload.*/
        Tac49xProprietaryPayload     ProprietaryPayload;  /*!< Defines if the packet contains a proprietary payload.*/
		Tac49xPlaybackPayload		 PlaybackPayload;
		Tac49xRecordPayload			 RecordPayload;       /*!< Defines if the packet contains a record payload.*/
        Tac49xDebugPayload           DebugPayload;        /*!< Defines if the packet contains a debug payload.*/  
        U8 Payload[MAX_PAYLOAD_SIZE];
        } u;
    } Tac49xPacket;

    /** @} */

/*******************************************************************************/
/* 
typedef volatile struct    
    {
    Tac49xPacketHeader ErrorHeader;
    U16 ErrorIndex;
    union
        {
        Tac49xCallProgressDeviceConfigurationPacket       CallProgress     ;
        Tac49xUserDefinedTonesDeviceConfigurationPacket   UserDefinedTones ;
        Tac49xAgcDeviceConfigurationPacket                Agc              ;
        Tac49xOpenDeviceConfigurationPacket               Open             ;
        } Echo;
    } TFailureEcho;

typedef volatile struct
    {
    union
        {
        Tac49xPacketHeader CheckingHeader;
        union
            {
            Tac49xCallProgressDeviceConfigurationPacket       CallProgress     ;
            Tac49xUserDefinedTonesDeviceConfigurationPacket   UserDefinedTones ;
            Tac49xAgcDeviceConfigurationPacket                Agc              ;
            Tac49xOpenDeviceConfigurationPacket               Open             ;
            } Ok;
        TFailureEcho Failure;
        } u;
    } Tac49xInitStateConfigurationEchoPacket;
*/

/******************************************/
/******************************************/
/**  Configuration Attribute Structures  **/
/******************************************/
/******************************************/

            /* Device Register Handling */

    /**
     * @defgroup Tac49xDeviceControlRegisterAttr Tac49xDeviceControlRegisterAttr
     * @ingroup DEVICE_CFG_STRUCTURES
     * @brief
     * Contains the reset bit as well as information on how to transfer commands to the AC49x.
     * @{
     */

    /**
     * <BR>
     * This structure contains the reset bit as well as information on how to transfer commands 
     * to the AC49x. The Device Control Register fields are read-only when the reset bit is set to 1. After 
     * writing to this Register, the Host must wait and poll until the AC49x resets the reset bit back to 0.
     */

typedef struct  
    {
	U8 NetworkCellSize          ;      /*!< The cell size of each packet in the network port (available only in devices that transfer network packets through the Host port). Default value: 56.*/
	U8 HostCellSize             ;      /*!< The cell size of each packet in the Host port. Default value: 44.*/
	Tac49xControl Reset         ;      /*!< Software Reset: @li Device control register is ignored. @li The VoPP performs software reset. All parameters are restored to their defaults. After the device reads this register, it resets the bit back to 0.*/
	Tac49xControl HostDebug 	;      /*!< Host Port Debug Mode: @li Disabled (default). @li Enabled (debug footer is added to all packets that are transferred through the Host port).*/
	Tac49xControl NetworkDebug  ;      /*!< Network Port Debug Mode (Available only in the devices that transfer network packets through the Host port). @li Disabled (default). @li Enabled (debug footer is added to all packets that are transferred through the Host port).*/
    } Tac49xDeviceControlRegisterAttr;

    /** @} */

            /************************************************************************/
            /*   I N I T   S T A T E  configurtion Functions Attributes structures  */
            /************************************************************************/
#if (  (AC49X_DEVICE_TYPE == AC491_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC491L_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC491_491L_DEVICE)\
    )
#ifdef UTOPIA_ROUTING

typedef struct
	{
	int	ToDevice;
	int	ToChannel;
	Tac49xMediaChannelType ToMediaChannelType;
	} Tac49xSingleMediaChannelTypeAttr;
typedef struct
	{
	Tac49xSingleMediaChannelTypeAttr MediaChannelType[AC49X_NUMBER_OF_MEDIA_CHANNEL_TYPES];
	} Tac49xSingleUtopiaRoutingAttr;

    /**
     * @defgroup Tac49xUtopiaRoutingConfigurationAttr Tac49xUtopiaRoutingConfigurationAttr
     * @ingroup DEVICE_CFG_STRUCTURES
     * @brief
     * Defines a packet connection between two channels on the same device when the UTOPIA master performs 
     * loopback of all packets.
     * @{
     */

    /**
     * <BR>
     * This structure is used as an input to the function 
     * ::Ac49xUtopiaRoutingConfiguration. It is used to define a packet connection between two channels on the same 
     * device when the UTOPIA master performs loopback of all packets. If the configuration is not sent, then 
     * each UTOPIA packet header will contain the transmitting channel's core ID and channel ID. If it is, then the 
     * UTOPIA packet header will contain the configured channel ID, core ID and protocol.
     * It is composed of an array of channels, each channel combines 4 sets of configurations: 
     * @li Regular channel (RTP, RTCP, ATM, etc.) @li Mediated channel (Mediated RTP, Mediated RTCP, Mediated ATM, etc.)
     * @li Conf1 (3-way conference protocol) @li Conf2 (3-way conference protocol)
     *
     * Each one of the above sets contains: @li ToDevice @li ToChannel @li ToMediaChannelType (protocol)
     *
     * These parameters replace the device channel and protocol of the original packet. When the Utipia Master 
     * initiates loopback the packet arrives to the configured fields (device, channel).
     */

typedef struct
	{
	Tac49xSingleUtopiaRoutingAttr Route[AC49X_NUMBER_OF_CHANNELS]; 
	} Tac49xUtopiaRoutingConfigurationAttr;

    /** @} */

#endif /* UTOPIA_ROUTING    */          
#endif /* AC49X_DEVICE_TYPE */

    /**
     * This structure configures specific parameters for a single signal.
     * Users must define the Call Progress Tones command fields as follows: 
     * @li The tone pair signal is the combination of the two single frequency signals. If only one single tone is used, the frequency B for this tone must be set to zero.
     * @li The minimum separation between any adjacent frequencies is 40 Hz.
     * @li The same is true for the separation between any frequency, and any frequency of any other active signaling system.
     */

typedef struct  
    {       
	Tac49xCallProgressSignalType    Type       	  								;  /*!< Call Progress Signal Type.*/
	int								ToneAFrequency                              ;  /*!< Frequency, expressed in Hz, in range 300-1980 Hz, in steps of 1 Hz. Unused frequencies must be set to zero.*/
	int								ToneB_OrAmpModulationFrequency              ;  /*!< Frequency, expressed in Hz, within the range of 300-1980 Hz, in steps of 1 Hz. Unused frequencies must be set to zero. If AM Factor <> 0, then this field represents the AM frequency in the range of 1-128 Hz.*/
	int								TwistThreshold      	                    ;  /*!< Maximum acceptable ratio between energies of the components of the tone pair signal. Recommended value is 10 dB, but the actual value is obtained from the Tone Pair source, such as PBX, and line attenuation. Units are in dB.*/
	int								ThirdToneOfTripleBatchDurationTypeFrequency ;  /*!< Frequency, expressed in Hz, within the range of 300-1980 Hz, in steps of 1 Hz. Unused frequencies must be set to zero. Frequency C is only relevant when 'Type' is set to SIT (3).*/
	int								HighEnergyThreshold                         ;  /*!< The single frequency component of the Tone Pairs with energy above this threshold is rejected. Recommended value is 0 dBm, but the actual value is obtained from the Tone Pair source, such as PBX, and line attenuation. The units are in (-dBm)*/
	int								LowEnergyThreshold                          ;  /*!< The single frequency component of the Tone Pairs with energy below this threshold is rejected. Recommended value is -35 dBm, but the actual value is obtained from the Tone Pair source, such as PBX, and line attenuation. Units are in (-dBm)*/
	int								SignalToNoiseRatioThreshold                 ;  /*!< Signals with a Signal To Noise Ratio below this value will not be detected. Units are in dB. Range = 10 dB to 60 dB. Recommended value is 15 dB. The actual value is obtained from the Tone Pair source, such as PBX, and line attenuation.*/
	int								FrequencyDeviationThreshold                 ;  /*!< The deviation allowed in each frequency. Units are in Hertz. Range = 1 to 30 Hz. Recommended value is 10 Hz.*/
	int								ToneALevel     	                            ;  /*!< Output level of the low frequency tone, in call progress generation. Output levels, ranging from -63dBm to 0 dBm. Units are in (-dBm)*/
	int								ToneBLevel                                  ;  /*!< Output level of the high frequency tone, in Call Progress generation. Output levels, ranging from -63 dBm to 0 dBm. Units are in (-dBm)*/
	int								AmFactor                                    ;  /*!< Amplitude Modulation Factor k. @li 0 = Amplitude Modulation disabled. @li 1-50 = The Amplitude Modulation factor in steps of 0.02. @li 51-63 = Reserved.*/
	int								DetectionTimeOrCadenceFirstOnOrBurstDuration;  /*!< The content of this field is according to the signal type: @li Continuous (1) = detection time. @li Cadence (2) = Ton state. Burst @li (3) = duration.*/
	int								CadenceFirstOffDuration                     ;  /*!< When the signal is cadence, this value represents the off duration. Units are in 10 msec. When it is not used, set it to zero.*/
	int								CadenceSecondOnDuration                     ;  /*!< When the signal is cadence, this value represents the on duration. Units are in 10 msec. When it is not used, set it to zero.*/
	int								CadenceSecondOffDuration                    ;  /*!< When the signal is cadence, this value represents the off duration. Units are in 10 msec. When it is not used, set it to zero.*/
	int								CadenceThirdOnDuration                      ;  /*!< When the signal is cadence, this value represents the on duration. Units are in 10 msec. When it is not used, set it to zero.*/
	int								CadenceThirdOffDuration                     ;  /*!< When the signal is cadence, this value represents the off duration. Units are in 10 msec. When it is not used, set it to zero.*/
	int								CadenceFourthOnDuration                     ;  /*!< When the signal is cadence, this value represents the on duration. Units are in 10 msec. When it is not used, set it to zero.*/
	int								CadenceFourthOffDuration                    ;  /*!< When the signal is cadence, this value represents the off duration. Units are in 10 msec. When it is not used, set it to zero.*/
	Tac49xCallProgressAddVoiceMode	AddVoiceMode								;  /*!< This parameter configures how voice is added to the call progress signal. <ul> <li> 0 = Voice is added to signal during silence periods only. <li> 1 = Voice is added to the intier signal (silence and tone periods).</ul> @n @n The parameters <ul> <li> CadenceVoiceAddedWhileFirstOff <li> CadenceVoiceAddedWhileSecondOff <li> CadenceVoiceAddedWhileThirdOff <li> CadenceVoiceAddedWhileFourthOff </ul> configure weather voice is added to the call progress signal. @n Refer to ::Tac49xCallProgressAddVoiceMode. */
	Tac49xControl					CadenceVoiceAddedWhileFirstOff              ;  /*!< If the signal is Cadence, this value configures whether voice is added to the signal. It is used in the call waiting tone. <ul> <li> 0 = During Off time, mute is generated. <li> 1 = During Off time, voice is generated. </ul> The parameter @b AddVoiceMode configures how the voice is added to the call progress signal. (silences periods only or entier signal) */
    Tac49xControl					CadenceVoiceAddedWhileSecondOff             ;  /*!< If the signal is Cadence, this value configures whether voice is added to the signal. It is used in the call waiting tone. <ul> <li> 0 = During Off time, mute is generated. <li> 1 = During Off time, voice is generated. </ul> */
    Tac49xControl					CadenceVoiceAddedWhileThirdOff              ;  /*!< If the signal is Cadence, this value configures whether voice is added to the signal. It is used in the call waiting tone. <ul> <li> 0 = During Off time, mute is generated. <li> 1 = During Off time, voice is generated. </ul> */
    Tac49xControl					CadenceVoiceAddedWhileFourthOff             ;  /*!< If the signal is Cadence, this value configures whether voice is added to the signal. It is used in the call waiting tone. <ul> <li> 0 = During Off time, mute is generated. <li> 1 = During Off time, voice is generated. </ul> */
    } Tac49xCallProgressSignalAttr;

    /**
     * This structure configures specific parameters of a single user defined tone signal.
     */

typedef struct  
    {       
	int                         ToneAFrequency                              ;  /*!< Frequency, expressed in Hz, within the range of 300-3800 Hz in steps of 1 Hz. Unused frequencies must be set to zero.*/
	int                         ToneBFrequency                              ;  /*!< Frequency, expressed in Hz, within the range of 300-3800 Hz in steps of 1 Hz. Unused frequencies must be set to zero.*/
	int                         TwistThreshold      	                    ;  /*!< Maximum acceptable ratio between energies of the components of the tone pair signal. Recommended value is 10 dB, but the actual value is obtained from the Tone Pair source, such as PBX, and line attenuation. Units are in dB.*/
	int                         HighEnergyThreshold                         ;  /*!< The single frequency component of the Tone Pairs with energy above this threshold is rejected. Recommended value is 0 dBm, but the actual value is obtained from the Tone Pair source, such as PBX, and line attenuation. The units are in (-dBm)*/
	int                         LowEnergyThreshold                          ;  /*!< The single frequency component of the Tone Pairs with energy below this threshold is rejected. Recommended value is -35 dBm, but the actual value is obtained from the Tone Pair source, such as PBX, and line attenuation. Units are in (-dBm)*/
	int                         SignalToNoiseRatioThreshold                 ;  /*!< Signals with a Signal To Noise Ratio below this value will not be detected. Units are in dB. Range is 10 dB to 60 dB. Recommended value is 15 dB. The actual value is obtained from the Tone Pair source, such as PBX, and line attenuation.*/
	int                         FrequencyDeviationThreshold                 ;  /*!< The deviation allowed in each frequency. Units are in Hertz. The range is 1 to 30 Hz. Recommended value is 10 Hz.*/
	int                         ToneALevel    	                            ;  /*!< Output level of the low frequency tone, in Call Progress generation. Output levels, ranging from -63 dBm to 0 dBm. Units are in (-dBm).*/
	int                         ToneBLevel                                  ;  /*!< Output level of the low frequency tone, in Call Progress generation. Output levels, ranging from -63 dBm to 0 dBm. Units are in (-dBm).*/
    } Tac49xUserDefinedTonesSignalAttr;

    /**
     * @defgroup Tac49xCallProgressDeviceConfigurationAttr Tac49xCallProgressDeviceConfigurationAttr
     * @ingroup DEVICE_CFG_STRUCTURES
     * @brief
     * Contains up to 32 dual-frequency user defined Call Progress Tones.
     * @{
     */

    /**
     * <BR>
     * This structure contains up to 32 dual-frequency user defined Call Progress Tones.
     */

typedef struct  
    {       
	int                                 NumberOfSignals;       /*!< The number of signals, 0 to 32*/
	int                                 TotalEnergyThreshold;  /*!< Only signals with energy above this threshold are processed for detection. Recommended value is -44 dBm but the actual value is obtained from the Tone Pair source, such as PBX, and line attenuation. Units are -dBm. The range is -50 dBm to -20 dBm*/
	Tac49xCallProgressSignalAttr        Signal[CALL_PROGRESS_AND_USER_DEFINED_TONES_SIGNAL_QUANTITY]; /*!< The signal parameters (refer to Tac49xCallProgressSignalAttr).*/
    } Tac49xCallProgressDeviceConfigurationAttr;

    /** @} */

    /**
     * @defgroup Tac49xUserDefinedTonesDeviceConfigurationAttr Tac49xUserDefinedTonesDeviceConfigurationAttr
     * @ingroup DEVICE_CFG_STRUCTURES
     * @brief
     * Contains up to 32 dual-frequency user defined tones.
     * @{
     */

    /**
     * <BR>
     * This structure contains up to 32 dual-frequency user defined tones.
     */

typedef struct 
    {    
	int                                 NumberOfSignals;      /*!< Number Of Signals. The maximum is 32*/
	int                                 TotalEnergyThreshold; /*!< Total Energy Threshold*/
	Tac49xUserDefinedTonesSignalAttr    Signal[CALL_PROGRESS_AND_USER_DEFINED_TONES_SIGNAL_QUANTITY];
    } Tac49xUserDefinedTonesDeviceConfigurationAttr;

    /** @} */

    /**
     * @defgroup Tac49xAgcDeviceConfigurationAttr Tac49xAgcDeviceConfigurationAttr
     * @ingroup DEVICE_CFG_STRUCTURES
     * @brief
     * This structure is used to configure the AGC (Automatic Gain Control) module.
     * @{
     */

    /**
     * <BR>
     * This structure is used to configure the AGC (Automatic Gain Control) module.
     */

typedef struct
    {
    Tac49xAgcMinGain        MinGain;                  /*!< The index of the minimal gain. Default = -20 dB.*/
    Tac49xAgcMaxGain        MaxGain;                  /*!< The index of the maximal gain. Default = 15 dB.*/
    Tac49xAgcGainSlope		FastAdaptationGainSlope;  /*!< The rate of changes in AGC gain during Fast Adaptation Mode.  Default = 10 dB/sec.*/
    int                     FastAdaptationDuration ;  /*!< The duration of the Fast Adaptation Mode in msec. Default = 1500.*/
    int                     SilenceDuration;          /*!< The silence time duration that determines not active conversation, in seconds. Default = 120.*/
    int                     DetectionDuration;        /*!< The time duration that the AGC waits at the beginning of the conversation, in msec. before starting operation. Default = 15.*/
    int                     SignalingOverhang;        /*!< The time duration that the AGC is disabled, after the end of non-speech events (Call Progress, DTMFs, etc.) in msec. Default = 500.*/
    } Tac49xAgcDeviceConfigurationAttr;

    /** @} */

    /**
     * @defgroup Tac49xExtendedDeviceConfigurationAttr Tac49xExtendedDeviceConfigurationAttr
     * @ingroup DEVICE_CFG_STRUCTURES
     * @brief
     * Contains configuration parameters common to all channels on the device.
     * @{
     */

    /**
     * <BR>
     * This structure contains configuration parameters common to all channels on the device.
     */

typedef struct
    {
	Tac49xControl                   DebugUemClipEnable;                 /*!< Reserved for internal use. Must be set to 0. */
	Tac49xControl                   DmaMisalignmentDetectionEnable;     /*!< This parameter enables sensitive detection of TDM misalignment. The parameter is available on the AC490 VoPP only. @n If is sensitive detection of misalignment occurs, the VoPP reports a System Error with error index 21. Refer to ::Tac49xSystemErrorEventPayload. @n Default = DEFAULT_DISBALED */
	Tac49xControl                   TextV8Jm;                           /*!< This parameter enables the TEXT-V.8 JM event in the fax and data status (Tac49xFaxOrDataSignalStatusPayload). @n Default = CONTROL__ENABLE.*/
	Tac49xControl                   TextV8Cm;                           /*!< This parameter enables the TEXT-V.8 CM event in the fax and data status (Tac49xFaxOrDataSignalStatusPayload). @n Default = CONTROL__ENABLE.*/
	Tac49xControl                   TextV8Ci;                           /*!< This parameter enables the TEXT-V.8 CI event in the fax and data status (Tac49xFaxOrDataSignalStatusPayload). @n Default = CONTROL__ENABLE.*/
	Tac49xControl                   VxxV8Cm;                            /*!< This parameter enables the V.XX-V.8 CM event in the fax and data status (Tac49xFaxOrDataSignalStatusPayload). @n Default = CONTROL__ENABLE.*/
	Tac49xControl                   VxxV8Ci;                            /*!< This parameter enables the V.XX-V.8 CI event in the fax and data status (Tac49xFaxOrDataSignalStatusPayload). @n Default = CONTROL__ENABLE.*/
                                    
	Tac49xControl                   FaxV8Cm;                            /*!< This parameter enables the FAX-V.8 CM event in the fax and data status (Tac49xFaxOrDataSignalStatusPayload). @n Default = CONTROL__ENABLE.*/
	Tac49xControl                   FaxV8Ci;                            /*!< This parameter enables the FAX-V.8 CI event in the fax and data status (Tac49xFaxOrDataSignalStatusPayload). @n Default = CONTROL__ENABLE.*/
    Tac49xControl			        FaxDcn;                             /*!< This parameter enables the fax disconnect event in the fax and data status (Tac49xFaxOrDataSignalStatusPayload). @n Default = CONTROL__ENABLE.*/
    Tac49xControl			        Tty;                                /*!< This parameter enables the TTY event in the fax and data status (Tac49xFaxOrDataSignalStatusPayload). @n Default = CONTROL__ENABLE.*/
    Tac49xControl			        Bell103AnswerTone;                  /*!< This parameter enables the Bell 103 answer tone event in the fax and data status (Tac49xFaxOrDataSignalStatusPayload). @n Default = CONTROL__DISABLE.*/
    Tac49xControl			        V21Channel1;                        /*!< This parameter enables the V.21 channel 1 event in the fax and data status (Tac49xFaxOrDataSignalStatusPayload). @n Default = CONTROL__ENABLE.*/
	Tac49xControl                   V25CallingTone;                     /*!< This parameter enables the V.25 Calling Tone event in the fax and data status (Tac49xFaxOrDataSignalStatusPayload). @n Default = CONTROL__ENABLE.*/
	Tac49xControl                   StuModem;                           /*!< This parameter enables the STU Modem event in the fax and data status (Tac49xFaxOrDataSignalStatusPayload). @n Default = CONTROL__ENABLE.*/
							        
	Tac49xControl			        V23ForwardChannel;					/*!< This parameter enables the V.23 forward channel mark tone event in the fax and data status (Tac49xFaxOrDataSignalStatusPayload). @n Default = CONTROL__ENABLE.*/
    Tac49xControl			        V21Channel2Data;                    /*!< This parameter enables the V.21 channel 2 event in the fax and data status (Tac49xFaxOrDataSignalStatusPayload). @n Default = CONTROL__ENABLE.*/
    Tac49xControl			        DataSessionVxx;                     /*!< This parameter enables the data session event in the fax and data status (Tac49xFaxOrDataSignalStatusPayload). @n Default = CONTROL__ENABLE.*/
	Tac49xControl			        RespondingDualToneV8Bis;            /*!< This parameter enables the responding dual tone V.8bis event in the fax and data status (Tac49xFaxOrDataSignalStatusPayload). @n Default = CONTROL__ENABLE.*/
	Tac49xControl			        InitiatingDualToneV8Bis;            /*!< This parameter enables the initiating dual tone V.8bis event in the fax and data status (Tac49xFaxOrDataSignalStatusPayload). @n Default = CONTROL__ENABLE.*/
	Tac49xControl			        V22Usb1;                            /*!< This parameter enables the V.22 USB 1 event in the fax and data status (Tac49xFaxOrDataSignalStatusPayload). @n Default = CONTROL__ENABLE.*/
	Tac49xControl			        V32Aa;                              /*!< This parameter enables the V.32 AA event in the fax and data status (Tac49xFaxOrDataSignalStatusPayload). @n Default = CONTROL__ENABLE.*/
	Tac49xControl			        VxxV8JmVxxData;                     /*!< This parameter enables the V8 Jm Vxx data event in the fax and data status (Tac49xFaxOrDataSignalStatusPayload). @n Default = CONTROL__ENABLE.*/
							        
	Tac49xControl			        FaxV8JmV34;                         /*!< This parameter enables the fax V8 Jm V.34 event in the fax and data status (Tac49xFaxOrDataSignalStatusPayload). @n Default = CONTROL__ENABLE.*/
	Tac49xControl			        FaxV21PreambleFlags;                /*!< This parameter enables the fax V.21 preamble flags event in the fax and data status (Tac49xFaxOrDataSignalStatusPayload). @n Default = CONTROL__ENABLE.*/
	Tac49xControl			        AnswerToneAmReversals;				/*!< This parameter enables the answer tone AM and phase reversals event in the fax and data status (Tac49xFaxOrDataSignalStatusPayload). @n Default = CONTROL__ENABLE.*/
	Tac49xControl			        AnswerToneAm;						/*!< This parameter enables the answer tone AM event in the fax and data status (Tac49xFaxOrDataSignalStatusPayload). @n Default = CONTROL__ENABLE.*/
	Tac49xControl			        AnswerWithReversalsAns;				/*!< This parameter enables the answer tone with reversals event in the fax and data status (Tac49xFaxOrDataSignalStatusPayload). @n Default = CONTROL__ENABLE.*/
	Tac49xControl			        AnswerTone2100FaxCedOrModemAns;		/*!< This parameter enables the 2100 answer tone event in the fax and data status (Tac49xFaxOrDataSignalStatusPayload). @n Default = CONTROL__ENABLE.*/
	Tac49xControl			        CallingFaxToneCng;                  /*!< This parameter enables the CNG calling fax tone event in the fax and data status (Tac49xFaxOrDataSignalStatusPayload). @n Default = CONTROL__ENABLE.*/
	Tac49xControl			        SilenceOrUnkown;                    /*!< This parameter enables the Undefined (Unknown signal or silence) event in the fax and data status (Tac49xFaxOrDataSignalStatusPayload). Default = CONTROL__ENABLE.*/
    Tac49xMfcR1DetectionStandard    MfcR1DetectionStandard;             /*!< MFC R1 detection standard. @li 0 = ITU (default)  @li 1 = R1.5 */
    Tac49xDtmfErasureSensitivity    DtmfErasureSensitivity;             /*!< Reserved for internal use. Must be set to 0 (DTMF_ERASURE_SENSITIVITY__LOW)*/
    Tac49xDtmfFrequencyDeviation    DtmfFrequencyDeviation;             /*!< Reserved for internal use. Must be set to 0 (Default_1_5_Percentage) */ 
	Tac49xDtmfStepRatio		        DtmfStepRatio;                      /*!< This parameter is configured in order to avoid DTMF false detection. @n Default = DEFAULT_DISBALED.*/
	Tac49xDtmfStepDistance	        DtmfStepDistance;                   /*!< This parameter is configured in order to avoid DTMF false detection. @n Default = DEFAULT_0_msec.*/
	Tac49xControl					DtmfDetectionABCDDisable; 			/*!< This parameter configures whether detection of ABCD DTMF digits is supported. @li 0 = Detection of ABCD DTMF digits is enabled (default)  @li 1 =  detection of ABCD DTMF digits is disabled.*/
#if (  (AC49X_DEVICE_TYPE == AC494_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC495_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC495L_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC496_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC496E_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC497_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC494E_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC495E_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC496D_DEVICE)\
    )
	Tac49xIPPChipResourceTypes		IPPChipResourceType;				/*!< This parameter  should be set to IPP_CHIP_RESOURCE_495L when running on the @b AC495L @b SoC for all other cases set it to IPP_CHIP_RESOURCE_494. @n refer to ::Tac49xIPPChipResourceTypes.  */
#endif
	} Tac49xExtendedDeviceConfigurationAttr;

    /** @} */

    /**
     * Configures the PCM slots of each channel.
     */

typedef struct
    {
    int Slot;                /*!< The channel's slot on the PCM highway. If the Slot is bigger than the maximum available slots, the channel isn't connected to the serial port. Default = Channel*/
    int InterconnectedSlot;  /*!< The channel's secondary voice slot (used on the function ::Ac49xTimeSlotManipulationCommand).*/
    } Tac49xSlotAttr;


    /**
     * Configures the PCM slots of each channel.
     */

typedef struct
    {
	int PcmSlot;                /*!< The channel's voice slot on the PCM highway. If the Slot is greater than the maximum available slots, the channel isn't connected to the serial port. Default = Channel*/
	int CasSlot;                /*!< The channel's CAS slot on the PCM highway.*/
	int InterconnectedPcmSlot;  /*!< The channel's secondary voice slot (used on the function ::Ac49xTimeSlotManipulationCommand).*/
    } Tac49xSlot_Format1Attr;

    /**
     * This structure contains parameters common to all devices on the same multi core VoPP (AC491).
     * Note that these parameters must be identical for all devices on the same multi core VoPP (AC491).
     */

typedef struct
	{
#if (  (AC49X_DEVICE_TYPE == AC494_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC495_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC495L_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC496_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC496E_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC497_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC494E_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC495E_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC496D_DEVICE)\
    )
    Tac49xCodecEnable                               CodecEnable;                 /*!< Defines the analog interface of channels 0 and 1. Channels 3 and 4 remain connected through the serial port regardless of the configuration. @li DISABLE = Voice interface of channels 0 and 1 through serial port. @li FOR_CHANNEL_1 = Voice interface of channel 0 only is through codec. @li CHANNEL_1_AND_2 = Voice interface of channels 0 and 1 is through codec.*/
	Tac49xAicClkFrequency                           AicClkFrequency;             /*!< External AC494 clock frequency. Default = 12288_KHZ.*/
#endif                                              
	Tac49xControl                                   CpuClkOut;                   /*!< Defines the state of the CPU clockout pin. Default = CONTROL__DISABLE.*/
    Tac49xPcmClkAndFrameSyncSource                  PcmClkAndFrameSyncSource;    /*!< PCM Clock & Frame Sync Source. Default = EXTERNAL. @n Available only on the AC494 device.*/
    Tac49xBusSpeed                                  BusSpeed;                    /*!< Serial port PCM highway clock frequency. @li BUS_SPEED__2MHZ = default for AC490. @li BUS_SPEED__8MHZ = default for AC491.*/
    Tac49xControl                                   WatchdogEnable;              /*!< Enables the watch-dog mechanism.*/
    Tac49xLawSelect                                 LawSelect;                   /*!< PCM serial port A-law / Mu-law / Linear / ADPCM configuration.*/
    int                                             UtopiaClockDetection;        /*!< UTOPIA Clock Detection. Default = 0. @li 0= UTOPIA Clock detection disabled. @li 1-255 = UTOPIA Clock Detection Enabled (Clock = UCD * CPU CLOCK/6)*/
    Tac49xUtopiaCellSize                            UtopiaCellSize;              /*!< Utopia Cell Size. Only UTOPIA_CELL_SIZE__56_BYTES is available.*/
    Tac49xPcmSerialPortRxTxDataDelay                PcmSerialPortRxTxDataDelay;  /*!< PCM Serial Port Receive/Transmit Data Delay, in 1-bit resolution. Default = PCM_SERIAL_PORT_RX_TX_DATA_DELAY__1_Bit*/
    Tac49xControl                                   Utopia16BitMode;             /*!< Configures the UTOPIA mode: @li CONTROL__DISABLE = 8-bit. @li CONTROL__ENABLE = 16-bit (default).*/
    int                                             UtopiaLogicalSlaveId;        /*!< 6-bit UTOPIA Logical Slave Address. The 6-bit field is placed in the header of every UTOPIA cell transmitted by the device. Default = 0.*/
    int                                             UtopiaPhysicalSlaveId;       /*!< 5-bit UTOPIA Physical Slave Address. The 5 bits are the PHY address used to identify the port in a MPHY setup. Default = 0.*/
    Tac49xIpSecPortSelection                        IpSecPortSelection;          /*!< Reserved for internal use. Must be set to zero.*/
                                                    
    U16                                             PcmFrameSyncWidth;           /*!< This field is available when PcmClkAndFrameSyncSource is set to INTERNAL (frame sync is output). @n It configures the frame sync width in PCM clock cycles. Width = (PcmFrameSyncWidth + 1). This field is valid in the AC494 only. @n The default is 0.*/
    U16                                             PcmClkDivideRatio;           /*!< This field is available when PcmClkAndFrameSyncSource is set to INTERNAL (PCM clock is output). @n It configures the PCM clock frequency. PCM clock = CPU clock / ((PcmClkDivideRatio + 1)*2). This field is valid in the AC494 only. @n The default is 29.*/
    Tac49xCasBitsLocation                           CasBitsLocation;             /*!< CAS Bits Location (AC491 only). @li 0 = ABCD located on bits 3-0 (low nibble) (default). @li 1 = ABCD located on bits 7-4 (high nibble).*/
    Tac49xEventsPortType                            EventsPortType;              /*!< Reserved for internal use. Must be set to EVENTS_PORT_TYPE__HOST */   
    Tac49xTdmInterfaceConfiguration                 TdmInterfaceConfiguration;   /*!< TDM Interface configuration (available on the AC494 only). @li 0 = 8 KHz sampling frequency (default). @li 1 = 16 KHz sampling frequency - Legerity codec compliance. The framesync frequency is 8 KHz. Each channel has 2 slots per frame. The first slot is as configured. The second slot is placed at Slot + ('Frame Length'/2). (Supported on the AC494 only.) @li 2 = 16 kHz sampling frequency. The framesync frequency is 8 kHz. Each channel has 32-bits or 16-bits slot per frame. Each big slot contains two 8-bit or 16-bit samples. The parameter ::Tac49xDeviceCommonAttr.LawSelect configures the sample size. (Supported on the AC491 only.) */   
	int												Profile;                     /*!< This field defines the features that are going to be supported. Refer to the <A HREF="Profiles.html">profiles table</A> for the feature list. The number of channels supported in each template is different for each profile.  */
    } Tac49xDeviceCommonAttr; 


    /**
     * @defgroup Tac49xOpenDeviceConfigurationAttr Tac49xOpenDeviceConfigurationAttr
     * @ingroup DEVICE_CFG_STRUCTURES
     * @brief
     * Contains configuration parameters common to all channels on the device.
     * @{
     */

    /**
     * <BR>
     * This structure contains configuration parameters common to all channels on the device.
     */

typedef struct
    {
    int										NumberOfChannels;              /*!< Number of channels in the Core. Default for AC491 = 16. Default for AC490 = 12.*/
	Tac49xControl							ConnectedSlotEnable;           /*!< The connected slots are available on the AC490 & AC491 VoPPs. To each channel, 2 Timeslots (TS) can be assigned: a TS which is physically connected to the channel, and another TS that is associated with this channel, but that is not physically connected to it. This feature enables timeslot manipulation with Connected slots.*/
	Tac49xCasMode							CasMode;					   /*!< Configures the CAS detection mode. Default = CAS_MODE__NO_SIGNALING*/
	Tac49xSerialPortSelection				SerialPortSelection;           /*!< The DSP receives two streams. This field informs the DSP as to which stream is related to CAS and which to Voice. @li VOICE_0_CAS_1 = Voice is placed in stream 0. CAS is placed in stream 1. @li VOICE_1_CAS_0 = Voice is placed in stream 1. CAS is placed in stream 0. @li VOICE_0_CAS_0 = Both voice and CAS are placed in stream 0 (the CAS data is located on the connected slots). @li VOICE_1_CAS_1 = Both voice and CAS are placed in stream 1 (the CAS data is located on the connected slots).*/
    Tac49xJitterBufferStatusReportMode		JitterBufferStatusReportMode;  /*!< Jitter Buffer Status Report. @li JITTER_BUFFER_STATUS_REPORT_MODE__SEND_IF_OVERRUN_OR_UNDERRUN_ONLY = Default; JBSR is sent only when overrun or underrun occurs. (The packet is sent to the Host "Error Report Interval" following occurrence of the event). @li JITTER_BUFFER_STATUS_REPORT_MODE__SEND_EVERY_ERROR_REPORT_INTERVAL = Jitter Buffer Status is sent at every "Error Report Interval". The minimal Time is 1s.*/
    Tac49xControl							CasReversalsDetection;         /*!< CAS Reversals Detection Enable. Default = CONTROL__ENABLE. @li CONTROL__DISABLE = Disable. @li CONTROL__ENABLE = Enable.*/
    Tac49xControl							DeviceStatusPacketEnable;      /*!< Device Status Packet Enable. Default = CONTROL__ENABLE. @li CONTROL__DISABLE = Disable. @li CONTROL__ENABLE = Enable.*/
	Tac49xControl							DebugInfoPacketEnable;         /*!< Reserved for internal use; set to CONTROL__DISABLE.*/
	int										CasValueDuringIdle;		       /*!< The CAS ABCD, transmitted during Idle State. Default = 0.*/
	int										PcmValueDuringIdle;		       /*!< The PCM value transmitted during Idle State. Default = 0xd5.*/
    Tac49xCasDebounce						CasDebounce;                   /*!< CAS Debounce value in 5 msec increments. Default = 30 msec. CAS change is detected only if the new value is stable for a time duration longer than CasDebounce.*/
	Tac49xDtmfGenerationTwistDirection		DtmfGenerationTwistDirection;  /*!< This field determines the generation twist direction.*/
	Tac49xDtmfGenerationTwist				DtmfGenerationTwist;           /*!< This field determines the generation twist level.*/
    int										ErrorReportInterval;           /*!< The duration in which the device reports Jitter Buffer Status Packet and Receive Overrun Errors. The resolution is 100 msec. If the value 0 is set, then Status & Error packets are sent once the overrun/underrun has occurred. Default = 10 (equal to 1 sec).*/
	int										DeviceStatusInterval;          /*!< Device Status packet is sent every "Device Status Interval"; (100 msec resolution); Range 1-80 (100 msec - 8 sec). Default = 10 (equal to 1 sec).*/
	Tac49xControl							UtopiaTestEnable;              /*!< If this test is enabled, the AC49x transmits predefined UTOPIA cells. It expect the UTOPIA master to perform a loopback. The received packets are tested aginst the transmitted packets. During the test, channels cannot be opened until a device reset.*/
	Tac49xModuleDebugPacketsTransferMethod	ModuleDebugPacketsTransferMethod;  /*!< Reserved for internal use. Must be set to 0. */
	Tac49xDtmfSnr							DtmfSnr;                       /*!< DTMF Detection Signal-to-Noise Ratio (SNR). If the SNR is below the threshold, DTMFs are not detected. Default = 12 dB*/
	Tac49xDtmfGap							DtmfGap;                       /*!< DTMF gap. If a DTMF digit is cut in the middle, the DTMF 'off' event is not sent as long as the gap does not exceed 'DTMF gap duration'. Default = 25 msec. @n Refer to ::Tac49xDtmfGap.*/
	Tac49xDeviceStatusIntervalResolution	DeviceStatusIntervalResolution;/*!< This field configures the resolution of the field @b DeviceStatusInterval. Refer to ::Tac49xDeviceStatusIntervalResolution. @n Default = 100_msec (0). */
	Tac49xDeviceCommonAttr					Common;                        /*!< Parameters common to all devices on the same multi core VoPP (AC491)*/
    Tac49xSlotAttr							Channel[MAX_NUMBER_OF_ACTIVE_TIME_SLOTS];  /*!< Configures the PCM slots of each channel.*/
    } Tac49xOpenDeviceConfigurationAttr;


    /** @} */


typedef Tac49xOpenDeviceConfigurationAttr Tac49xOpenDeviceConfiguration_Format0Attr;

    /**
     * @defgroup Tac49xOpenDeviceConfiguration_Format1Attr Tac49xOpenDeviceConfiguration_Format1Attr
     * @ingroup DEVICE_CFG_STRUCTURES
     * @brief
     * Contains configuration parameters common to all channels on the device.
     * @{
     */

    /**
     * <BR>
     * This structure contains configuration parameters common to all channels on the device. It is used as an input to the
     * function ::Ac49xOpenDeviceConfiguration_Format1.
     */


typedef struct
	{
	int										NumberOfChannels;					/*!< Number of channels in the Core. Default for AC491 = 16. Default for AC490 = 12.*/
	Tac49xCasMode							CasMode;							/*!< Configures the CAS detection mode. Default = CAS_MODE__NO_SIGNALING*/
    Tac49xJitterBufferStatusReportMode		JitterBufferStatusReportMode;		/*!< Jitter Buffer Status Report. @li JITTER_BUFFER_STATUS_REPORT_MODE__SEND_IF_OVERRUN_OR_UNDERRUN_ONLY = Default; JBSR is sent only when overrun or underrun occurs. (The packet is sent to the Host "Error Report Interval" following occurrence of the event). @li JITTER_BUFFER_STATUS_REPORT_MODE__SEND_EVERY_ERROR_REPORT_INTERVAL = Jitter Buffer Status is sent at every "Error Report Interval". The minimal Time is 1s.*/
    Tac49xControl							CasReversalsDetection;				/*!< CAS Reversals Detection Enable. Default = CONTROL__ENABLE. @li CONTROL__DISABLE = Disable. @li CONTROL__ENABLE = Enable.*/
    Tac49xControl							DeviceStatusPacketEnable;			/*!< Device Status Packet Enable. Default = CONTROL__ENABLE. @li CONTROL__DISABLE = Disable. @li CONTROL__ENABLE = Enable.*/
	Tac49xControl							DebugInfoPacketEnable;				/*!< Reserved for internal use; set to CONTROL__DISABLE.*/
	int										CasValueDuringIdle;					/*!< The CAS ABCD, transmitted during Idle State. Default = 0.*/
	int										PcmValueDuringIdle;					/*!< The PCM value transmitted during Idle State. Default = 0xd5.*/
    Tac49xCasDebounce						CasDebounce;						/*!< CAS Debounce value in 5 msec increments. Default = 30 msec. CAS change is detected only if the new value is stable for a time duration longer than CasDebounce.*/
	Tac49xDtmfGenerationTwistDirection		DtmfGenerationTwistDirection;		/*!< This field determines the generation twist direction.*/
	Tac49xDtmfGenerationTwist				DtmfGenerationTwist;				/*!< This field determines the generation twist level.*/
    int										ErrorReportInterval;				/*!< The duration in which the device reports Jitter Buffer Status Packet and Receive Overrun Errors. The resolution is 100 msec. If the value 0 is set, then Status & Error packets are sent once the overrun/underrun has occurred. Default = 10 (equal to 1 sec).*/
	int										DeviceStatusInterval;				/*!< Device Status packet is sent every "Device Status Interval"; (100 msec resolution); Range 1-80 (100 msec - 8 sec). Default = 10 (equal to 1 sec).*/
	Tac49xControl							UtopiaTestEnable;					/*!< If this test is enabled, the AC49x transmits predefined UTOPIA cells. It expect the UTOPIA master to perform a loopback. The received packets are tested aginst the transmitted packets. During the test, channels cannot be opened until a device reset.*/
	Tac49xModuleDebugPacketsTransferMethod	ModuleDebugPacketsTransferMethod;   /*!< Reserved for internal use. Must be set to 0. */
	Tac49xDtmfSnr							DtmfSnr;							/*!< DTMF Detection Signal-to-Noise Ratio (SNR). If the SNR is below the threshold, DTMFs are not detected. Default = 12 dB*/
	Tac49xDtmfGap							DtmfGap;							/*!< DTMF gap. If a DTMF digit is cut in the middle, the DTMF 'off' event is not sent as long as the gap does not exceed 'DTMF gap duration'. Default = 25 msec. @n Refer to ::Tac49xDtmfGap.*/
	Tac49xDeviceStatusIntervalResolution	DeviceStatusIntervalResolution;     /*!< This field configures the resolution of the field @b DeviceStatusInterval. Refer to ::Tac49xDeviceStatusIntervalResolution. @n Default = 100_msec (0). */
	Tac49xDeviceCommonAttr					Common;								/*!< Parameters common to all devices on the same multi core VoPP (AC491)*/
											
	Tac49xControl							ConnectedSlots;                     /*!< The connected slots are available on the AC490 & AC491 VoPPs. To each channel, 2 Timeslots (TS) can be assigned: a TS which is physically connected to the channel, and another TS that is associated with this channel, but that is not physically connected to it. This feature enables timeslot manipulation with Connected slots.*/
	Tac49xSerialPortSelection_Format1		ConnectedSlotsSerialPortSelection;  /*!< Connected slots Serial Port selection (supported on AC491 VoPP only). It determines on which serial port the connected slots are located.*/
	Tac49xSerialPortSelection_Format1		CasSerialPortSelection;             /*!< CAS slots Serial Port selection (supported on AC491 VoPP only). It determines on which serial port the CAS slots are located.*/
	Tac49xSerialPortSelection_Format1		VoiceSerialPortSelection;           /*!< Voice slots Serial Port selection. It determines on which serial port the Voice slots are located.*/
    Tac49xSlot_Format1Attr					Channel[MAX_NUMBER_OF_ACTIVE_TIME_SLOTS];  /*!< Configures the PCM slots of each channel.*/
	} Tac49xOpenDeviceConfiguration_Format1Attr;

    /** @} */

#if (AC49X_DEVICE_TYPE == AC490_DEVICE)
typedef volatile struct
    {
    U32 ErrorAddress;
    U16 TestType;
    U16 ExpectedValue;
    U16 ReceivedValue;
    }Tac49xSingleEmifTestErrorInformationAttr;

    /**
     * @defgroup Tac49xEmifTestErrorInformationAttr Tac49xEmifTestErrorInformationAttr
     * @ingroup TESTS_STRUCTURES
     * @brief
     * Contains the AC490 VoPP external memory test status report.
     * @{
     */

    /**
     * <BR>
     * This structure contains the AC490 VoPP external memory test status report.
     */

typedef volatile struct
	{
	Tac49xSingleEmifTestErrorInformationAttr SingleEmifTestErrorInformation[EMIF_TEST__ERROR_INFORMATION_SIZE/sizeof(Tac49xSingleEmifTestErrorInformationPayload)];
	} Tac49xEmifTestErrorInformationAttr;

    /** @} */

#endif /*(AC49X_DEVICE_TYPE == AC490_DEVICE)*/


            /*********************************************************************/
            /*  R U N   S T A T E configurtion Functions Attributes structures   */
            /*********************************************************************/


    /**
     * @defgroup Tac49xOpenOrUpdateChannelConfigurationAttr Tac49xOpenOrUpdateChannelConfigurationAttr
     * @ingroup CHANNEL_CFG_STRUCTURES
     * @brief
     * Defines configuration parameters of a single channel.
     * @{
     */

    /**
     * <BR>
     * This structure defines configuration parameters of a single channel. These parameters are independent of the 
     * network type (ATM/RTP). Refer to <A HREF="Tac49xOpenOrUpdateChannelConfigurationAttr - Compliance.html">
     * Tac49xOpenOrUpdateChannelConfigurationAttr compliance</A> to classify which parameters can be updated 
     * on-the-fly.
     */

typedef struct
    {
    Tac49xPcmSource                 			PcmSource                   ;         /*!< @li PCM_SOURCE__FROM_SERIAL_PORT = Regular mode (default) @li PCM_SOURCE__FROM_NETWORK = Packet to packet mode.*/
	Tac49xPacketToPacketMode					PacketToPacketMode			;         /*!< Packet To Packet Mode (Relevant only if PcmSource = PCM_SOURCE__FROM_NETWORK) @li 0 = Synchronous Packet to Packet @li 1 = Asynchronous Packet to Packet.*/
	Tac49xChannelTdmMode						TdmMode						;         /*!< Reserved for internal use. Must be set to 0. */
    Tac49xChannelType               			ChannelType                 ;         /*!< The channel type after it is opened. Default = CHANNEL_TYPE__VOICE_FAX.*/
    Tac49xDtmfErasureMode           			DtmfErasureMode             ;         /*!< This feature should be enabled in DTMF Relay. It distorts the DTMF digit that is transferred through the voice interface prior to DTMF detection. Default = DTMF_ERASURE_MODE__ERASE_1_COMPONENT.*/
    Tac49xIbsTransferMode           			MfTransferMode              ;         /*!< Configures the detected MF-R1, MF-R2B, MF-R2F Transfer Modes. Only IBS_TRANSFER_MODE__RELAY_DISABLE_VOICE_MUTE and IBS_TRANSFER_MODE__TRANSPARENT_THROUGH_VOICE (default) are supported.*/
    Tac49xIbsTransferMode           			DtmfTransferMode            ;         /*!< Configures the detected DTMF transfer mode. Default = IBS_TRANSFER_MODE__TRANSPARENT_THROUGH_VOICE.*/
    Tac49xIbsTransferMode           			CallProgressTransferMode    ;         /*!< Configures the detected Call Progress transfer mode. Only IBS_TRANSFER_MODE__RELAY_DISABLE_VOICE_MUTE and IBS_TRANSFER_MODE__TRANSPARENT_THROUGH_VOICE (default) are supported.*/
    Tac49xControl                   			DetectPcmCas                ;         /*!< Configures the CAS Detection per channel. @li CONTROL__DISABLE = CAS detection disabled. @li CONTROL__ENABLE = CAS detection enabled (default).*/
    Tac49xControl                   			DetectNetCasPackets         ;         /*!< Enables the detection of network CAS packets (RFC 2833). Default = CONTROL__DISABLE.*/
    Tac49xControl                   			DetectNetIbsPackets         ;         /*!< Enables the detection of network IBS packets (RFC 2833). Default = CONTROL__DISABLE.*/
    Tac49xControl                   			IgnoreNetIbsPackets         ;         /*!< Ignore IBS relay packets (RFC 2833). @li CONTROL__DISABLE = IBS relay packets are not ignored (default). @li CONTROL__ENABLE = IBS relay packets are ignored and mute is generated instead.*/
    Tac49xControl                   			IgnoreNetCasPackets         ;         /*!< Ignore CAS relay packets (RFC 2833). @li CONTROL__DISABLE = CAS relay packets are not ignored (default). @li CONTROL__ENABLE = CAS relay packets are ignored.*/
/*  Tac49xIbsTransferMode           			LineSignalingTransferMode   ;*/
    Tac49xIbsDetectionRedirection   			IbsDetectionRedirection     ;         /*!< Configures the IBS detection direction. Default = IBS_DETECTION_REDIRECTION__ON_INPUT_STREAM_ENCODER_INPUT. @n @b Note: If direction is set to ON_OUTPUT_STREAM_DECODER_OUTPUT, then fax detection is disabled.*/
    int                             			IbsLevel_minus_dbm          ;         /*!< DTMF and MF gain (in steps of -dBm) during Relay or IBS string generation. Default = -9 dBm.*/
    Tac49xControl                   			UdpChecksumEnable           ;         /*!< Enables UDP Checksum. If this field is set to CONTROL__ENABLE, then UDP check sum is added to every transmitted network packet.*/
    Tac49xEchoCancelerNonLinearProcessor		EchoCancelerNonLinearProcessor;  /*!< Echo canceler non-linear processor mode. @n Default = ECHO_CANCELER_NON_LINEAR_PROCESSOR_MODE__MEDIUM_SENSITIVITY. @n Refer to ::Tac49xEchoCancelerNonLinearProcessor.*/
/*  Tac49xSilenceCompressionMode    			SilenceCompressionMode      ;*/
/*  int                             			NumberOfVoiceBlocks_minus_1 ;*/
    Tac49xPcmLimiter                			PcmLimiter                  ;         /*!< PCM Limitter mode. Default = PCM_LIMITER__DISABLE*/
    Tac49xHybridLoss                			HybridLoss                  ;         /*!< Echo Canceler Hybrid loss. Default = HYBRID_LOSS__6_DB.*/
    Tac49xVoiceGain                 			VoiceOutputGain             ;         /*!< Voice decoder output gain. Default = VOICE_GAIN__0_DB.*/
/*  Tac49xCoder                     			Decoder                     ;*/
/*  Tac49xMaxRate                   			DataMaxRate                 ;*/
    Tac49xEchoCancelerMode          			EchoCancelerMode            ;         /*!< Echo canceler operation mode. Default = ECHO_CANCELER_MODE__ENABLE.*/
    Tac49xVoiceGain                 			InputGain                   ;         /*!< Input gain. Default = VOICE_GAIN__0_DB.*/
    Tac49xControl                   			CasRelay                    ;         /*!< @li CONTROL__DISABLE = CAS Relay Disable (default). @li CONTROL__ENABLE = CAS Relay Enable.*/
    Tac49xTestMode                  			TestMode                    ;         /*!< Configures the test mode. Default = TEST_MODE__NONE.*/
    Tac49xDynamicJitterBufferOptimizationFactor DynamicJitterBufferOptimizationFactor;  /*!< Dynamic Jitter Buffer Optimization Factor. Default = 10. @li 0 = Static Jitter Buffer. @li 1-12 = This an empirical, user-defined factor, where: 1 is optimal for delay and 12 is optimal for voice quality. @li 13 = Optimal for fax and data transfer. @li 14-15 = Reserved.*/
    Tac49xControl                   			TestFaxModem                ;         /*!< Set to CONTROL__DISABLE (reserved for internal use).*/
    Tac49xControl                   			DetectUserDefinedTones      ;         /*!< Enables Used Defined Tones (UDT) detection. Default = CONTROL__DISABLE.*/
    Tac49xControl                   			DetectCallProgress          ;         /*!< Enables Call Progress Tones detection. Default = CONTROL__DISABLE.*/
    Tac49xControl                   			DetectDTMFRegister          ;         /*!< Enables DTMF detection. Default = CONTROL__ENABLE.*/
    Tac49xControl                   			Reserve_FX2                 ;
    Tac49xControl                   			Reserve_FX5                 ;
    Tac49xControl                   			Reserve_FX4                 ;
    Tac49xControl                   			Reserve_FX15                ;
    Tac49xControl                   			Reserve_FX1                 ;
    Tac49xControl                   			DetectMFR2BackwardRegister  ;         /*!< Enables MF R2 Backward detection .Default = CONTROL__DISABLE.*/
    Tac49xControl                   			DetectMFR2ForwardRegister   ;         /*!< Enables MF R2 Forward detection .Default = CONTROL__DISABLE.*/
    Tac49xControl                   			DetectMFSS5Register         ;         /*!< Enables MF SS-5 Register detection .Default = CONTROL__DISABLE.*/
    Tac49xControl                   			DetectMFR1Register          ;         /*!< Enables MF R1 detection .Default = CONTROL__DISABLE.*/
    Tac49xControl                   			CallerIdOnHookService       ;         /*!< Enables the Call ID "on hook" service (Type 1) detection. Default = CONTROL__DISABLE.*/
    Tac49xControl                   			CallerIdOffHookService      ;         /*!< Enables the Caller ID "off hook" service (Type 2, CID on Call Waiting) detection. Default = CONTROL__DISABLE.*/
    Tac49xControl                   			LongDtmfDetectionEnable     ;         /*!< Long DTMF digit detection event. This event is transmitted if the detected DTMF duration has passed 2 seconds. Default = CONTROL__DISABLE.*/
    Tac49xMaxRate                   			FaxMaxRate                  ;         /*!< Fax Relay Max Rate. Default = MAX_RATE__14_4_KBPS. @n @b Note: This parameter may be  overridden by the function ::Ac49xT38ChannelConfiguration. */
    Tac49xEchoCancelerLength        			EchoCancelerLength          ;         /*!< Echo Canceler length at an 8 msec resolution. Default = ECHO_CANCELER_LENGTH__64_MSEC. @n Refer to ::Tac49xEchoCancelerLength. @n Note that only 32, 64 and 128 msec lengths are supported.*/
    Tac49xControl                   			TriStateDuringIdle          ;         /*!< TriState. Channel PCM Out Mode during Idle State. @li CONTROL__DISABLE = Not Tri State (default). @li CONTROL__ENABLE = Tri State.*/
    Tac49xCasModeDuringIdle         			CasModeDuringIdle           ;         /*!< Idle CAS Mode. The CAS mode when the channel is closed. @li CONTROL__DISABLE = CAS Idle Value (default). @li CONTROL__ENABLE = CAS Value as was before the channel was closed.*/
    int                             			CasValueDuringIdle          ;         /*!< Generated CAS value during Idle State. Default = 0.*/
    int                             			PcmValueDuringIdle          ;         /*!< Generated PCM Value during Idle State. Default = 0xd5.*/
	Tac49xActiveChannelPattern					ActiveChannelPattern		;         /*!< Active channel Pattern. This bit defines the PCM value while the channel is in active state, when the voice decoder, fax, CID, playback or IBS generator are not active. @li 0 = PCM Value is mute  (default) @li 1 = PCM value is equal to 'PCM Vlue during Idle'*/
/*  Tac49xCoder                     			BypassCoder                 ;*/
/*  int                             			BypassNumberOfBlocks_minus_1;*/
    Tac49xFaxAndCallerIdLevel       			FaxAndCallerIdLevel         ;         /*!< Fax and Caller ID output level. Default = FAX_AND_CALLER_ID_LEVEL__minus_12_DBM.*/
	Tac49xControl								FaxSignalsDetectorOnSbc		;		  /*!< Reserved for internal use. Must be set to 0. */
    Tac49xTtyTransferMethod         			TtyTransferMethod           ;         /*!< TIA-825A (TTY) Transfer. Default = TTY_TRANSFER_METHOD__DISABLE. Note that in-band TTY relay is supported with the EVRC Vocoder or by means of CTM adaptation. */
    Tac49xFaxModemTransferMethod    			V34FaxTransferMethod        ;         /*!< V.34 ("Super" G3) Fax Transfer. Default = FAX_MODEM_TRANSFER_METHOD__DISABLE.*/
    Tac49xFaxModemTransferMethod    			V34TransferMethod           ;         /*!< V34 Transfer. Only FAX_MODEM_TRANSFER_METHOD__BYPASS and FAX_MODEM_TRANSFER_METHOD__DISABLE (default) are supported.*/
    Tac49xFaxModemTransferMethod    			V32TransferMethod           ;         /*!< V32 Transfer. Only FAX_MODEM_TRANSFER_METHOD__BYPASS and FAX_MODEM_TRANSFER_METHOD__DISABLE (default) are supported.*/
    Tac49xFaxModemTransferMethod    			V23TransferMethod           ;         /*!< V23 Transfer. Only FAX_MODEM_TRANSFER_METHOD__BYPASS and FAX_MODEM_TRANSFER_METHOD__DISABLE (default) are supported.*/
    Tac49xFaxModemTransferMethod    			V22TransferMethod           ;         /*!< V22 Transfer. Only FAX_MODEM_TRANSFER_METHOD__BYPASS and FAX_MODEM_TRANSFER_METHOD__DISABLE (default) are supported.*/
    Tac49xFaxModemTransferMethod    			V21TransferMethod           ;         /*!< V21 Transfer. Only FAX_MODEM_TRANSFER_METHOD__BYPASS and FAX_MODEM_TRANSFER_METHOD__DISABLE (default) are supported.*/
    Tac49xFaxModemTransferMethod    			Bell103TransferMethod       ;         /*!< Bell 103 Transfer. Only FAX_MODEM_TRANSFER_METHOD__BYPASS and FAX_MODEM_TRANSFER_METHOD__DISABLE (default) are supported.*/
    Tac49xFaxModemTransferMethod    			FaxTransferMethod           ;         /*!< Fax transfer. Default = FAX_MODEM_TRANSFER_METHOD__DISABLE.*/
    Tac49xFaxProtocol               			FaxProtocol                 ;         /*!< Fax protocol. Default = FAX_PROTOCOL__T38_UDP_DATA_RATE_MANAGEMENT_2. @n @b Note: This parameter may be overridden by the function ::Ac49xT38ChannelConfiguration.*/
    Tac49xCedTransferMode           			CedTransferMode             ;         /*!< Fax signal's CED transfer mode. @li 0 = Transfers answer tone using FAX transfer mode. CED or ANSam are relayed using T.38 protocol (V34Fax=1 and FAX=1) or VBD transferred (FAX=2). @li 1 = Reserved. @li 2 = Transfers answer tone using VXX transfer mode. Answer tone is transferred in voice mode (modem transport = voice) or VBD (modem transport = bypass). Fax Relay is triggered by V.21 preamble or by a combination of contra-propagating fax signals (e.g., CNG and CED). @li 3 = Transfers the answer tone using RFC 4733 event packets in parallel with RTP audio/VBD stream. Fax Relay is triggered by V.21 preamble or by a combination of contra-propagating fax signals. */
    Tac49xCallerIdStandard          			CallerIdStandard            ;         /*!< Caller ID standard. Default = CALLER_ID_STANDARD__TELCORDIA_BELLCORE.*/
    Tac49xControl                   			FaxErrorCorrectionMode      ;         /*!< Enable the T.30 fax relay error correction mode Default = CONTROL__ENABLE.*/
    Tac49xControl                   			CngRelayEnable              ;         /*!< CNG Relay/VBD Mode on the detection side. On the generation side, CNG relay is always enabled. @li CONTROL__DISABLE = Disable  - CNG is transferred through voice (default). @li CONTROL__ENABLE = CNG signal is relayed using T.38 protocol (for FAX=1) or VBD transferred (for FAX=2).*/
/*  Tac49xPcmAdpcmFrameDuration     			EncoderPcmAdpcmFrameDuration;*/
	Tac49xControl								AnswerDetectEnable;                   /*!< Answer Detect. @li 0 = Disable (default) @li 1 = Enable */
	Tac49xAnswerDetectDirection					AnswerDetectDirection;                /*!< Answer Detect Direction. @li 0 = Answer Detector located on encoder input (default) @li 1 = Answer Detector located on decoder output */
	Tac49xAnswerDetectMode						AnswerDetectMode;                     /*!< Answer Detect Mode. @li 0 = Answer Detector - not sensitive to non-voice signals such as tones @li 1 = VAD - sensitive to non-voice signals (default) */
	int											AnswerDetectSensitivity;              /*!< Answer Detect Sensitivity. Range = 0 - 7, where 0 is the most sensitive and 7 is the least sensitive. Default = 3. */
    int                             			VoiceJitterBufferMinDelay   ;         /*!< Minimum delay of Dynamic Jitter Buffer, or fixed delay in Static Jitter Buffer, in msec resolution. @n Range: 0 - 280 msec. @n Default = 35 msec. */
    int                             			VoiceJitterBufferMaxDelay   ;         /*!< Maximum delay of Dynamic Jitter Buffer in msec resolution. @n Range: 0 - 1000 msec. @n Default = 150 msec. @n @b Note: The actual maximum size limit varies from product to product.*/
    Tac49xControl                   			VideoSynchronizationEnable  ;         /*!< Reserved for internal use. Must be set to 0. */  
/*  Tac49xPcmAdpcmFrameDuration     			DecoderPcmAdpcmFrameDuration;*/
    Tac49xAgcGainSlope              			AgcSlowAdaptationGainSlope  ;         /*!< The rate of change in AGC gain during Slow Adaptation Mode. @n Default = AGC_GAIN_SLOPE__1_00_DB_SEC.*/
    Tac49xControl                   			AgcEnable                   ;         /*!< Enables Automatic Gain Control. Default = CONTROL__DISABLE.*/
    Tac49xAgcLocation               			AgcLocation                 ;         /*!< @li ENCODER_INPUT = AGC is placed at encoder input (default). @li DECODER_OUTPUT = AGC is placed at decoder output.*/
    Tac49xAgcTargetEnergy           			AgcTargetEnergy             ;         /*!< The desired output energy in -dBm. @n Default = -19 dBm.*/
    Tac49xControl                   			FaxAndDataStatusAndEventsDisable;     /*!< @li CONTROL__ENABLE = Disable Fax and Data Status and Events. @li CONTROL__DISABLE = Enable Fax and Data Status and Events.*/
    Tac49xPcmSidSize                			PcmSidSize                  ;         /*!< G.711/G.726/G.727/iLBC SID size. The SID payload size in bytes. According to ITU G.711 appendix II. Default = 1_BYTE.*/
    Tac49xControl                   			EchoCancelerDebugPackets    ;         /*!< Echo Canceler Debug Packet. This bit enables transmission of packets with debug protocol through the network port. These packets carry debug information which can be used to evaluate the Echo Canceler's performance. It is recommended that the application will support recording of the debug packets so that echo issues in the field can be easily and quickly resolved. */  
	Tac49xControl								H324mDetectionEnable		;         /*!< Reserved for internal use. Must be set to 0. */
	Tac49xControl								PlaybackDebugPacketsEnable  ;         /*!< Reserved for internal use. Must be set to 0. */
	Tac49xControl								RecordingDebugPacketsEnable ;         /*!< Reserved for internal use. Must be set to 0. */
	Tac49xProtectionAuditEventTransferMode		ProtectionAuditEventTransferMode;     /*!< This field configures the transfer mode of ::Tac49xSrtpProtectionAuditEvent. Refer to ::Tac49xProtectionAuditEventTransferMode. @n Default = EVENT_SENT_IN_RESPONSE_TO_REQUEST_COMMAND (0). */
	Tac49xProtectionAuditEventTransferMode		ProtectionStateEventTransferMode;     /*!< This field configures the transfer mode of ::Tac49xSrtpProtectionStateEvent. Refer to ::Tac49xProtectionAuditEventTransferMode. @n Default = EVENT_SENT_IN_RESPONSE_TO_REQUEST_COMMAND (0). */
	Tac49xEnergyDetectionThreshold  			EnergyDetectionThreshold;             /*!< Reserved for internal use. Must be set to 0. */
	Tac49xEnergyDetectionDirection  			EnergyDetectionDirection;             /*!< Reserved for internal use. Must be set to 0. */
	Tac49xControl								EnergyDetectionEnable;                /*!< Reserved for internal use. Must be set to 0. */
	Tac49xControl								DenialOfServiceEventsEnable;          /*!< Reserved for internal use. Must be set to 0. */
	Tac49xDenialOfServiceWindowSize				DenialOfServiceWindowSize;            /*!< Reserved for internal use. Must be set to 0. */
	Tac49xDenialOfServiceBandWidth				DenialOfServiceT38MaxBandWidth;       /*!< Reserved for internal use. Must be set to 0. */
	Tac49xDenialOfServiceBandWidth 				DenialOfServiceRtpMaxBandWidth;       /*!< Reserved for internal use. Must be set to 0. */
	Tac49xDenialOfServiceBandWidth 				DenialOfServiceRtcpMaxBandWidth;      /*!< Reserved for internal use. Must be set to 0. */
	Tac49xArtificialBandwidthExtensionDirection ArtificialBandwidthExtensionDirection;/*!< Reserved for internal use. Must be set to 0. */
	Tac49xControl								ArtificialBandwidthExtensionEnable;	  /*!< Reserved for internal use. Must be set to 0. */
	Tac49xEchoCancellerResetControl				EchoCancellerResetControl;			  /*!< The Echo Canceler's reset control <ul> <li> 0 = The Echo Canceler is reset following these commands: <ul> <li> Open Channel <li> Activate RTP <li> Deactivate RTP <li> Update RTP (if the Encoder field is updated) </ul> <li> 1 = The Echo Canceler is reset following these commands: <ul> <li> Open Channel <li> Activate RTP (only if Encoder or Frame Size are different to what they were in the previous RTP activation) <li> Update RTP (if the Encoder field is updated) </ul> </ul> @n @n Refer to ::Tac49xEchoCancellerResetControl @n Default = 0 */
	Tac49xControl 								NoiseReductionPostGainEnable;		  /*!< This field enables the @b noise @b reduction post gain @li 0 = Disable (default) @li 1 = Enable */
	Tac49xControl 								NoiseReductionSuppressionEnable;	  /*!< Reserved for internal use. Must be set to 1.  */
	Tac49xControl 								NoiseReductionEnable;				  /*!< This field enables the @b noise @b reduction feature. The @b noise @b reduction module is designed to efficiently suppress stationary background noise (e.g. office noise) in addition to enhancing the speech by improving the SNR (Signal to Noise Ratio) and emphasizing the important features of the desired speech signal. On IP phones, in most cases, an additive noise will be captured due to environmental settings, such as office noise, air conditioning. <ul> <li> 0 = Disable (default) <li> 1 = Enable </ul> @n @n @b Note: The noise reduction is supported on the AC494 SoC. It is enabled only when the microphone analog interface is enabled.*/
	Tac49xControl								CompressorEnable;					  /*!< This field enables gain compressor towards the tdm output @n Default = CONTROL__DISABLE (0) */		
    } Tac49xOpenOrUpdateChannelConfigurationAttr;

    /** @} */

    /**
     * @defgroup Tac49xAdvancedChannelConfigurationAttr Tac49xAdvancedChannelConfigurationAttr
     * @ingroup CHANNEL_CFG_STRUCTURES
     * @brief
     * Defines advanced channel configuration parameters.
     * @{
     */

    /**
     * <BR>
     * This structure defines advanced channel configuration parameters.
     */

typedef struct
    {
    Tac49xEchoCancelerThreshold             EchoCancelerThreshold;              /*!< Not in use.*/
    Tac49xActiveEchoCancellationMaxLength   ActiveEchoCancellationMaxLength;    /*!< Not in use.*/
    int                                     EncoderIbsOverhang_10msec;          /*!< Duration after the DTMF OFF packet is generated and before generating voice packet. Field is relevant only if Tac49xOpenOrUpdateChannelConfigurationAttr::DtmfTransferMode or Tac49xOpenOrUpdateChannelConfigurationAttr::MfTransferMode is not IBS_TRANSFER_MODE__TRANSPARENT_THROUGH_VOICE. Resolution: 10 msec (0-2550 msec). @n Default = 10 (i.e., 100 msec).*/
    int                                     DecoderIbsOverhang_10msec;          /*!< Silence duration generated after the IBS (relay) OFF packet is received, before the decoder returns to voice mode. Resolution 10 msec (0-2550 msec). @n Default = 100 (1 sec).*/
    Tac49xControl                           MuteEncoderDuringDtmfGeneration;    /*!< Enables encoder mute during DTMF generation. Default = CONTROL__DISABLE.*/
    int                                     SignalingJitter;                    /*!< Delay before generating the first IBS (relay) or CAS packet (first packet is first IBS or CAS after Open Channel, or if the duration from the previous packet exceeds 8.192 seconds). @n Resolution = 1 msec (0-160 msec). Default = 35 msec.*/
    int                                     FaxModemJitter;                     /*!< Fax relay jitter. Resolution in msec. Range 0 - 511 msec. Default = 0 (Dynamic jitter).*/
    int                                     BypassJitter;                       /*!< Fax or data bypass jitter buffer delay. Resolution in msec; Range 0 -160 msec. Default = 35 msec.*/
    Tac49xEchoCancellerHighPassFilter       EchoCancelerDcRemover;              /*!< Echo Canceler DC remover/high pass filter. @li 0 = Disable (default) @li 1 = DC remover enabled @li 2 = 360 Hz High Pass Filter.*/
	Tac49xEchoCancellerHighPassFilter       TdmOutDCRemover;				    /*!< TDM output DC remover/High Pass Filter. @li 0 = Disable (default) @li 1 = DC remover enabled @li 2 = 360 Hz High Pass Filter.*/
    Tac49xHdlcInterFrameFillPattern         HdlcInterFrameFillPattern;          /*!< Inter frame fill pattern. @li 7E = 0x7E (default). @li FF = 0xFF.*/
    Tac49xHdlcMinimumMessageLength          HdlcMinimumMessageLength;           /*!< Minimum message length. Default = HDLC_MININUM_MESSAGE_LENGTH__1_BYTE.*/
    Tac49xHdlcMinimumEndFlags               HdlcMinimumEndFlags;                /*!< Minimum number of frame end flags. Default = HDLC_MININUM_END_FLAGS__1.*/
    Tac49xHdlcSampleRate                    HdlcSampleRate;                     /*!< HDLC framer sampling rate. Default = HDLC_SAMPLE_RATE__64_KBPS.*/
    int                                     PlaybackWaterMark_msec;             /*!< Defines the threshold of the playback buffer in msec. Once the channel reaches this threshold, it sends a "playback request packet" to the network, requesting more voice packets. Default = 80 msec.*/
    Tac49xControl                           PlaybackRequestEnable;              /*!< @li CONTROL__DISABLE = Playback Request packets will not be sent by the channel. The Host is responsible for the timing of the playback packets. @li CONTROL__ENABLE = Playback Request packets will be sent by the channel. The VoPP channel is responsible for the timing of the playback packets.*/
    Tac49xSs7LinkRate                       Ss7LinkRate;                        /*!< Reserved for internal use; set to 0.*/
    Tac49xTransferMedium                    HdlcTransferMedium;                 /*!< HDLC transfer mode. @li TRANSFER_MEDIUM__HOST = The HDLC framer receives and transmits packet through the host port (default). @li TRANSFER_MEDIUM__NETWORK = The HDLC framer receives and transmits packet through the network port.*/
    Tac49xTransferMedium                    Ss7TransferMedium;                  /*!< Reserved for internal use; set to 0.*/
    Tac49xControl                           EchoCancelerDuringDataBypass;       /*!< Echo Canceler during Data Bypass. @n 0 = Disable (default). @n 1 = Enable.*/
    Tac49xControl                           EchoCancelerDuringFaxBypassDisable; /*!< Echo Canceler during Fax Bypass. @n 0 = Enable (default). @n 1 = Disable.*/
    Tac49xControl                           EchoCancelerNlpDuringBypass;        /*!< Echo Canceler NLP during Fax / Data Bypass. @n 0 = Disable (default). @n 1 = Enable.*/
    Tac49xEchoCancelerOperationSelectMode	AcousticEchoCancelerEnable;         /*!< This field enables the acoustic echo canceler. It is available on the AC494/AC494E SoC only. Refer to ::Tac49xEchoCancelerOperationSelectMode. Note that only modes 0 (LINE_ECHO_CANCELER) and 1 (ACOUSTIC_ECHO_CANCELER) are supported. Mode 2 (ACOUSTIC_ECHO_SUPPRESSOR) is reserved. @n Default = LINE_ECHO_CANCELER. */
    Tac49xControl                           EchoCancelerCng;                    /*!< Echo Canceler Comfort Noise Generation. @n 0 = Disable. @n 1 = Enable (default).*/
    Tac49xControl                           EchoCancelerToneDetector;           /*!< Echo Canceler Tone Detector (for correct operation, it is required that this parameter be set to 1). @n 0 = Disable. @n 1 = Enable (default).*/
    Tac49xControl                           EchoCancelerAggressiveNlp;          /*!< Echo Canceler Aggressive NLP. @li CONTROL__DISABLE = Aggressive NLP disabled . @li CONTROL__ENABLE = Aggressive NLP enabled (default).*/
	Tac49xControl							EchoCancellerAutomaticToneResetMode;/*!< Echo Canceller Automatic Reset during tone detection. @li 0 = Disable automatic reset @li 1 = Enable automatic reset (default)*/ 
	Tac49xControl							EchoCancellerAutomaticToneResetOnMuteMode; /*!< Echo Canceller Automatic Reset when Mute is detected at the TDM input. @li 0 = Disabled (default) @li 1 = Enabled */
	Tac49xControl                           EchoCancelerNoisyEnvironmentMode;   /*!< Echo Canceller Noisy environment Mode. Default = CONTROL__DISABLE.*/
	Tac49xControl                           EchoCancelerReconvergenceSupport;   /*!< Echo Canceller Reconvergence Support. @li 0 = Disable Echo Canceller Reconvergence @li 1 = Enable Echo Canceller Reconvergence  (default)*/
    Tac49xEchoCancelerNlpThreshold          EchoCancelerNlpThreshold;           /*!< Echo Canceler NLP will be active only if the input signal energy is below this threshold. Refer to ::Tac49xEchoCancelerNlpThreshold (default = 0 dBm).*/
    Tac49xControl                           EchoCancelerAutomaticHandler;       /*!< Echo Canceler Automatic Handler. This bit controls the Echo Canceler when Bypass and Fax Relay are disabled. If a fax/modem signal is detected, the Echo Canceler automatically changes its mode. The EC reverts back to Voice Mode only after the channel is closed. Note that if the host disables the Echo Canceler, the Echo Canceler remains closed regardless of the detected fax signals. @n The default is CONTROL__DISABLE.*/
    Tac49xPcmCngThreshold                   PcmCngThreshold;                    /*!< G.711/G.726/G727/iLBC Comfort Noise Generation Threshold. If the decoder receives an SID packet from the network with a noise level higher than the threshold, it clips it to the threshold. (The same threshold applies to both regular and mediated PCM channels). @n Default = minus20dBm.*/
    Tac49xInputGainLocation                 InputGainLocation;                  /*!< Note: If the acoustic echo canceler is enabled, the InputGainLocation must be set to AFTER_ECHO_CANCELER_AND_BOFORE_ENCODER. @li BEFORE_ECHO_CANCELER = Input gain is located before the echo canceler (default). @li AFTER_ECHO_CANCELER_AND_BOFORE_ENCODER = Input gain is located after the echo canceler.*/
    Tac49xDecoderIBSGenerationLocation      DecoderIBSGenerationLocation;       /*!< This parameter configures the location of the IBS generation towards the TDM output with relation to 3-way conferencing.  <ul> <li> 0 = IBS generation towards 3-way conference output (default).  <li> 1 = IBS generation towords 3-way conference input </ul> . @n @n This command configures any IBS generation (Call progress, DTMF, MF,..) and any generation command (IBS string, Extended IBS String, RFC4733 packet) */
	Tac49xFaxAndDataGain                    DataBypassOutputGain;               /*!< The decoder's output gain during data bypass. Default = 0_DB.*/
    Tac49xFaxAndDataGain                    FaxBypassOutputGain;                /*!< The decoder's output gain during fax bypass. Default = 0_DB.*/
    Tac49xDuaFramerMode                     DuaFrameMode;                       /*!< Reserved for internal use. Must be set to 0. */
    U8                                      DuaReceiveTimeout_x10msec;          /*!< Reserved for internal use. Must be set to 0. */
	Tac49xControl							AnswerDetectNoneSpeechEventsEnable; /*!< Answer Detect none-speech events. @li 0 = None-speech events (IBS detections are ignored) @li 1= None-speech events force the Answer Detector to make none-speech decisions (default). */
	int										AnswerDetectSilenceTime_x100ms;     /*!< Answer Detect Silence Time. The silence duration time required for determining end of speech. The resolution is 100 ms. The range is 100 ms to 204 sec. Default is 10 (i.e., 1 sec).*/
	int										AnswerDetectPreWorkDelay_x100ms;    /*!< Answer Detect Pre Work Delay. The initial delay after activating Answer Detector before the Answer Detector module starts to work. This delay may increase the module's immunity to noises and non-speech signals at the beginning of a conversation. The resolution is 100 ms. The range is 0 - 204 sec. Default is 0. */
	Tac49xControl							AnswerDetectLowEnergyDetectionEnable; /*!< Answer Detect low energy threshold. <ul> <li> 0 = Answer detect is not sensitive to low energy signals. <li> 1 = Answer detect is sensitive to low energy signals. </ul> @n Default = 0. */
	int										EnergyDetectionPMajorityThreshold;  /*!< Reserved for internal use. Must be set to 0. */
	int										EnergyDetectionNMajorityFrames;     /*!< Reserved for internal use. Must be set to 0. */
	int										EnergyDetectionMAverageDcBuffers;   /*!< Reserved for internal use. Must be set to 0. */
	int										EnergyDetectionKContinuesVoice;     /*!< Reserved for internal use. Must be set to 0. */
	int										EnergyDetectionClosingDelay;        /*!< Reserved for internal use. Must be set to 0. */
	Tac49xV34FaxBaudRateDisabler			V34FaxBaudRateDisabler;             /*!< This field disables some of the V.34 baud rates during V.34 fax relay. Refer to ::Tac49xV34FaxBaudRateDisabler. @n Default = ALL_RATES_ARE_ALLOWED_FOR_RECIEVE_AND_TRANSMIT_OPERATIONS (0). */
	Tac49xControl							V34FaxPrecoderDisabler;             /*!< This field disables the precoder during V.34 fax relay. <ul> <li> 0 = V.34 fax receiver provides complex-valued precoding coefficients for non-linear equalization on the V.34 fax transmitter. V.34 precoding improves performance of the V.34 fax receiver in conditions of amplitude distortions. (default) <li> 1 = V.34 fax receiver does not provide complex-valued precoding coefficients and, as a result, disables the V.34 precoder on the fax transmitter. This option helps to avoid demodulation failures caused by incorrect precoding by some V.34 fax transmitters. </ul> */
	Tac49xControl							NetworkAcousticEchoSuppressorAttenuationDisable;  /*!< Reserved for internal use. Must be set to 0. */
	Tac49xEchoCancelerAttenuatedReference	EchoCancelerAttenuatedReference;                  /*!< This field enables adding an attenuated echo canceller reference signal to the echo canceller input signal. Refer to ::Tac49xEchoCancelerAttenuatedReference. @n Default = DISABLED (0). */
	Tac49xControl							ExtendedNlpOperationToSuppressAdditiveEchoEnable; /*!< This field enables the echo canceller extended NLP mode. This mode should be enabled in order to suppress additive echo. @n Default = DISABLED (0). */
	Tac49xControl							NoiseReductionDebugPacketsEnable;	 /*!< This field enables the @b noise @b reduction debug packets transmission. <ul> <li> 0 = Disable (default) <li> 1 = Enable </ul>   */
	Tac49xNoiseReductionThresholdDesign		NoiseReductionThresholdDesign;		 /*!< The @b noise @b reduction threshold type. Refer to ::Tac49xNoiseReductionThresholdDesign. Default = DESIGNED_FOR_LOWEST_ACCEPTABLE_STATIONARY_NOISE_SIGNALS (2). */
	int										NoiseReductionPostGain;				 /*!< The post noise suppression gain in Q4. @n For example the value - 16[Q4] = 16/(2^4)=1.  When the value 16 is set, the post gain does not change the output level. @n The recommended range: 10h-20h (Post Gain = 1-2) */
	Tac49xNoiseReductionMinimumGain			NoiseReductionMinimumGain;			 /*!< The maximal noise attenuation during noise-only periods. Refer to ::Tac49xNoiseReductionMinimumGain. @n Default = minus8_DB (2). */

	Tac49xControl							NoiseReductionHandsFreeEnable;		 /*!< Supports noise reduction on the microphone codec interface (hands-free mode) @n Default = CONTROL__ENABLED (1) */
	Tac49xControl							NoiseReductionHandsetEnable;		 /*!< Supports noise reduction on the handset codec interface @n Default = CONTROL__DISABLE (0)   */
	Tac49xControl							NoiseReductionHeadsetEnable;		 /*!< Supports noise reduction on the headset codec interface @n Default = CONTROL__DISABLE (0)   */

	Tac49xControl							GenericCngEnable;					 /*!< This field enables generation of comfort noise towards the analog interface, according to the modes defined in the ::Tac49xAdvancedChannelConfigurationAttr.GenericCngOverrideMode field.  <ul> <li> 0 = @b Disabled. Comfort noise is generated only when the remote channel transmits SID packets. The noise characteristic is according to the SID packet content (default). <li> 1 = @b Enabled. Comfort noise is generated not only when the remote channel transmits SID packets. In this case, the noise characteristic can be calculated locally. </ul>   */		
	Tac49xControl							GenericCngDebugEnable;				 /*!< Reserved for internal use. Must be set to 0. */		
	Tac49xGenericCngOverrideMode			GenericCngOverrideMode;				 /*!< This field configures the local comfort noise generation mode.  The field is valid only if the ::Tac49xAdvancedChannelConfigurationAttr.GenericCngEnable field is set to @b 1. <ul> <li> 0 = @b No @b overriding. Comfort noise is generated locally if the remote channel does not transmit any packets (including SID) during periods of silence (default). <li> 1 = Comfort noise is generated locally: <ul> <li> If the remote channel does not transmit any packets (including SID) during periods of silence. <li> If the remote channel transmits SID packets without PARCOR coefficients </ul>  <li> 2 = Comfort noise is generated locally: <ul> <li> If the remote channel does not transmit any packets (including SID) during periods of silence. <li> If the remote channel transmits any SID packets, the SID payload content is ignored and the noise characteristic is determined locally. </ul> </ul>  */
    Tac49xG722OverloadPoint					G722OverloadPoint;					 /*!< This parameter configures the G722 Overload refernce point.  <ul> <li> 0 = +3dBmO (default).  <li> 1 = +9 dBmO) */

	int										CompressorRatio;					 /*!< This field enables configures the gain compression Ratio @n Default = 0 */		
	int										CompressorThreshold;				 /*!< This field enables configures the gain compression Threshold @n Default = 0 */		
} Tac49xAdvancedChannelConfigurationAttr;

    /** @} */

    /**
     * @defgroup Tac49xActivateOrUpdateRtpChannelConfigurationAttr Tac49xActivateOrUpdateRtpChannelConfigurationAttr
     * @ingroup CHANNEL_CFG_STRUCTURES
     * @brief
     * Defines the RTP/RTCP protocol configuration parameters.
     * @{
     */

    /**
     * <BR>
     * This structure defines the RTP/RTCP protocol configuration parameters.
     */

typedef struct
    {
    Tac49xAmrActiveMode						AmrActiveMode;                             /*!< This parameter defines the AMR/AMR-WB activation mode. The channel supports 3 different operational modes: @li AMR_ACTIVE_MODE__INACTIVE = Regular RTP packets (default). @li AMR_ACTIVE_MODE__3GPP_UP = If this is selected, the channel waits for a 3GPP configuration command. @li AMR_ACTIVE_MODE__RFC_3267 = If this is selected, the channel waits for an RFC 3267 configuration command.*/
    Tac49xControl							DisableRtcpIntervalRandomization;          /*!< @li CONTROL__DISABLE = @b Enable RTCP Interval Randomization (default) @li CONTROL__ENABLE = @b Disable RTCP Interval Randomization*/
    int										RtpRedundancy;                             /*!< The RTP redundancy level. <ul> <li> 0 = RTP redundancy disabled. <li> 1 = RTP redundancy level 1. <li> Other = Reserved. </ul> @n Default = 0.  */
    Tac49xMediaProtectionMode				MediaProtectionMode;                       /*!< Media Protection Mode. If media protection is enabled, the RTP channel is not activated until protection configuration packets are sent. Default = MEDIA_PROTECTION_MODE__DISABLE.*/
    Tac49xIbsRedundancyLevel				IbsRedundancyLevel;                        /*!< Number of redundancy IBS packet generation. Range 0-3. Default = IBS_REDUNDANCY_LEVEL__REPETITIONS_NONE.*/
    Tac49xPayloadTypeSource					TxPayloadTypeSource;                       /*!< Transmit Payload Type Source. @li PAYLOAD_TYPE_SOURCE__DEFINED_INTERNALLY_ACCORDING_THE_ENCODER = Payload Type defined by the AC49x channel, according to the Encoder (default). @li PAYLOAD_TYPE_SOURCE__DEFINED_EXTERNALLY = Payload Type is according to Tac49xActivateOrUpdateRtpChannelConfigurationAttr::TxPayloadType field.*/
    Tac49xPayloadTypeSource					RxPayloadTypeSource;                       /*!< Received Payload Type Source. @li PAYLOAD_TYPE_SOURCE__DEFINED_INTERNALLY_ACCORDING_THE_ENCODER = Payload Type defined by the AC49x channel, according to the Decoder (default). @li PAYLOAD_TYPE_SOURCE__DEFINED_EXTERNALLY = Payload Type is according to Tac49xActivateOrUpdateRtpChannelConfigurationAttr::RxPayloadType field.*/
    int										TxPayloadType;                             /*!< Transmit Payload Type. Default = 8.*/
    int										RxPayloadType;                             /*!< Receive Payload Type. Default = 8.*/
    int										FaxBypassPayloadType;                      /*!< Fax Bypass Payload Type. Default = 100.*/
    int										DataBypassPayloadType;                     /*!< Data Bypass Payload Type. Default = 101.*/
    U16										TxSequenceNumber;                          /*!< Initial transmit sequence number. Users must initialize this field to a 16-bit random number.*/
    U32										RtpSynchronizingSource;                    /*!< Synchronization Source Identifier. Users must initialize this field to a 32-bit random number.*/
    U32										TxTimeStamp;                               /*!< Initial transmit time stamp. Users must initialize this field to a 32-bit random number.*/
    U16										RtcpMeanTxInterval;                        /*!< Defines the mean interval between two transmitted RTCP packets (in msec). If 0, no RTCP packets are transmitted at all. Default = 5000.*/
    Tac49xCoder								Encoder;                                   /*!< Encoder Type. Default = CODER__G711ALAW. If the channel is a mediated channel, then Encoder & Decoder can only be configured to G.711.*/
    Tac49xCoder								Decoder;                                   /*!< Decoder Type. If the channel is a mediated channel, then Encoder & Decoder can only be configured to G.711. Default = CODER__G711ALAW.*/
    Tac49xControl							PcmAdPcmComfortNoiseRtpPayloadTypeEnable;  /*!< PCM/ADPCM Comfort Noise Payload Type Enable. (If enabled, then the decoder can support both modes) @li CONTROL__DISABLE = Payload Type of SID is similar to Voice payload type @li CONTROL__ENABLE = PCM/ADPCM SID has a different RTP payload Type (default)*/
    U8										PcmAdPcmComfortNoiseRtpPayloadType;        /*!< The RTP payload Type of the PCM/ADPCM comfort noise, when Tac49xActivateOrUpdateRtpChannelConfigurationAttr::PcmAdPcmComfortNoiseRtpPayloadTypeEnable = CONTROL__ENABLE. Default = 13.*/
    Tac49xRtpAdpcmPayloadFormat				AdpcmPayloadFormat;                        /*!< ADPCM/Linear 16-bit Payload Format. Default = RTP_ADPCM_PAYLOAD_FORMAT__LITTLE_ENDIAN.*/
    Tac49xVbdEventsRelayType				VbdEventsRelayType;                        /*!< Defines the relay mode of bypass events. Default = NONE.*/
#if (AC49X_DRIVRES_VERSION>=AC49X_DRIVER_VERSION_580)
    int										TxVbdEventsRelayPayloadType;               /*!< Payload Type of transmitted VBD events packets, used to transfer bypass events (SSE events). This parameter is valid only if parameter VbdEventsRelayType isn't NONE. This Payload Type must be different to Signaling RFC 2833 Payload Type.*/
	int										RxVbdEventsRelayPayloadType;               /*!< Payload Type of received VBD events packets, used to transfer bypass events (SSE events). This parameter is valid only if parameter VbdEventsRelayType isn't NONE. This Payload Type must be different to Signaling RFC 2833 Payload Type.*/
#else
	int										VbdEventsRelayPayloadType;                 /*!< Payload Type of VBD events packets, used to transfer bypass events. This parameter is valid only if parameter VbdEventsRelayType isn't NONE. This Payload Type must be different to Signaling RFC 2833 Payload Type.*/
#endif
    int										NumberOfVoiceBlocks_minus_1;               /*!< Number of Payload packed in a single packet. On the encoder side. @n (0 = one block; x = x+1 blocks). Default = 0.*/
    int										NumberOfBypassBlocks_minus_1;              /*!< Number of block of bypass coder. Packed in a packet. @n (0 = 1 block; x = x + 1 blocks). Default = 0.*/
    Tac49xPcmAdpcmFrameDuration				DecoderPcmAdpcmFrameDuration;              /*!< If the G.711, G.726, G.727, G.722, Linear 16-bit PCM, EG.711, or G.711 WB  decoder is selected, then this field sets the frame size of the decoder in 0.125 msec (samples) resolution. Range: 40 - 240 (5 - 30 msec). Default = PCM_ADPCM_FRAME_DURATION__20_MSEC.*/
    Tac49xPcmAdpcmFrameDuration				EncoderPcmAdpcmFrameDuration;              /*!< If the G.711, G.726, G.727, G.722, Linear 16-bit PCM, EG.711, or G.711 WB encoder is selected, then this field sets the frame size of the encoder in 0.125 msec (samples) resolution. @n Range: 40 - 240 (5 - 30 msec). Default = PCM_ADPCM_FRAME_DURATION__20_MSEC.*/
    Tac49xSilenceCompressionMode			SilenceCompressionMode;                    /*!< Encoder Silence Compression Mode. @n Default = SILENCE_COMPRESSION_MODE__DISABLE.*/
    Tac49xBrokenConnectionActivationMode	BrokenConnectionActivationMode;            /*!< Broken Connection Event Activation Mode. Default = ACTIVATE_AFTER_FIRST_RTP_PCKAET_ARRIVAL.*/
    U16										BrokenConnectionEventTimeout_x20msec;      /*!< Timeout that defines when a Broken Connection Event occurs. @n Time resolution is 20 msec. For example: 500 -> 10 sec. Default = 500.*/
    Tac49xCoder								BypassCoder;                               /*!< Bypass Coder Type. Default = CODER__G711ALAW.*/
    int										RtpCanonicalNameStringLength;              /*!< RTP canonical name string length. Must be equal to or less than 64 bytes.*/
    U8										TxSignalingRfc2833PayloadType;             /*!< Signaling RFC 2833 transmit Payload Type. Default = 96.*/
    U8										RxSignalingRfc2833PayloadType;             /*!< Signaling RFC 2833 receive Payload Type. Default = 96.*/
    U8										Rfc2198PayloadType;                        /*!< RFC 2198 Payload Type. Default = 104.*/
    U8                                      RxT38PayloadType;                          /*!< The received T.38 Payload Type when T.38 is configured to be transferred over RTP. Default = 106.*/
    U8                                      TxT38PayloadType;                          /*!< The transmitted T.38 Payload Type when T.38 is configured to be transferred over RTP. Default = 106.*/
    Tac49xControl                           NoOperationEnable;                         /*!< @li CONTROL__DISABLE = @b NO-OP disabled (default) @li CONTROL__ENABLE = @b NO-OP enabled.*/
    U8                                      NoOperationPayloadType;                    /*!< NO-OP Payload Type. Valid only if NO-OP is enabled. Default = 120.*/
    U16                                     NoOperationTimeout_x20msec;                /*!< The time during which no packets are transmitted, before a NO-OP packet is sent. Time resolution is 20 msec. The default is 500 (10 seconds). Valid only if NO-OP is enabled.*/
    Tac49xBypassRedundancyType              BypassRedundancyType;                      /*!< Automatic Bypass Redundancy. @li 0 = Bypass Redundancy corresponds to the configuration of ::Tac49xActivateOrUpdateRtpChannelConfigurationAttr.RtpRedundancy (default) @li 1 = When the channel switches to bypass, the packets are sent with redundancy (RFC 2198).*/
    Tac49xControl                           TxFilterRtp;                               /*!< @li CONTROL__DISABLE = No filter applied (default) @li CONTROL__ENABLE = RTP packets (which aren�t RFC 2833) aren�t transmitted to the network */
    Tac49xControl                           TxFilterRfc2833;                           /*!< @li CONTROL__DISABLE = No filter applied (default) @li CONTROL__ENABLE = RFC 2833 aren�t transmitted to the network. */ 
    Tac49xControl                           TxFilterRtcp;                              /*!< @li CONTROL__DISABLE = No filter applied (default) @li CONTROL__ENABLE = RTCP packets aren�t transmitted to the network. */ 
    Tac49xControl                           RxFilterRtp;                               /*!< @li CONTROL__DISABLE = No filter applied (default) @li CONTROL__ENABLE = Received RTP packets (which aren�t RFC 2833) are ignored. */ 
    Tac49xControl                           RxFilterRfc2833;                           /*!< @li CONTROL__DISABLE = No filter applied (default) @li CONTROL__ENABLE = Received RFC 2833 packets are ignored. */ 
    Tac49xControl                           RxFilterRtcp;                              /*!< @li CONTROL__DISABLE = No filter applied (default) @li CONTROL__ENABLE = Received RTCP packets are ignored. */ 
	Tac49xPacketToPacketSequenceNumberMode  PacketToPacketSequenceNumberMode;          /*!< Packet To Packet Sequence Number Mode (Relevant only in Asynchronous Packet to Packet). @li 0 = @b Sequential: Sequence number is always continuous (even when received SSRC in the associated channel is changed). Initial value is taken from 'Transmit Sequence Number' field (default) @li 1 = @b Non-Sequential: Sequence number is re-initialized each time SSRC is changed according to the new sequence number at the beginning of the talk burst */
	Tac49xPacketToPacketSsrcMode			PacketToPacketSsrcMode;                    /*!< Packet To Packet SSRC Mode (Relevant only in Asynchronous Packet to Packet). @li 0 = @b Configurable: taken from the field ::Tac49xActivateOrUpdateRtpChannelConfigurationAttr.RtpSynchronizingSource (default) @li 1 = @b Updated: The SSRC is copied from the received associated channel */
	int										PacketToPacketNoTranscodingPayloadType;    /*!< Packet To Packet 'No Transcoding' Payload Type (Relevant only in Asynchronous Packet to Packet). When a packet is received with this Payload Type, the VoPP channel doesn't perform transcoding on the payload. If there is packet loss, it is reflected in the outgoing RTP header. */
	Tac49xBlockNonePayloadType13			BlockNonePayloadType13;                    /*!< Block Non-Payload Type13 comfort noise packets. This bit defines how to handle a received packet with a single byte payload size that has the same RTP payload type as the voice decoder payload type, when the decoder is G.711, G.726, G.727 or iLBC. @li 0 = Handle the packet as a SID packet (default) @li 1 = Ignore the packet and generate an error message */
	Tac49xControl							ReportErrorOnReceivedStreamEnable;         /*!< This field enables or disables the error report when the channel is in 'Sender Only' mode (RxFilterRtp = CONTROL__ENABLE) and the VoPP channel receives RTP packets. @li 0 = Don't report error (default) @li 1 = Report error */
	int										IbsRelayTransmissionTimeout;               /*!< IBS Relay Transmission Timeout. @li 0 = No timeout for transmission of IBS Relay packets (default) @li Other = A timeout for transmission of IBS Relay packets to the network. Upon reaching this timeout, 'End-of-signal' is sent and the VoPP channel stops sending the RFC 2833 packets. Empty RTP voice packets are then sent until the end of the IBS signal. The resolution is 100 msec. */
	Tac49xRtcpXrEventsMode					RtcpXrEventsMode;                          /*!< RTCP-XR VQ statistics event mode. @li 0 = Disable ::Tac49xRtcpXrVoiceQualityStatisticsEvent event (default) @li 1 = Enable the ::Tac49xRtcpXrVoiceQualityStatisticsEvent event at the end of the call only. The event is not transmitted when an RTCP-XR packet is received @li 2 = Enable the ::Tac49xRtcpXrVoiceQualityStatisticsEvent event on each received or transmitted RTCP-XR packet and at the end of the call*/
	Tac49xRtcpXrMode						RtcpXrMode;                                /*!< RTCP-XR packet transmission. @li 0 = Disabled (default) @li 1 = Enabled */
	int										RtcpXrInterval;	                           /*!< Reserved for internal use. Must be set to 0. */
	Tac49xControl							RtpSequenceNumberDetectionEnable;          /*!< RTP Sequence Number Detection. @li 0 = Disabled (default) @li 1 = Enabled (detects an RTP packet with Sequence Number defined in ::Tac49xActivateOrUpdateRtpChannelConfigurationAttr.SequenceNumberForDetection) */
	int										SequenceNumberForDetection;                /*!< If the VoPP channel receives an RTP packet with this Sequence Number, it reports a ::Tac49xRtpSequenceNumberReceivedEvent event */
	Tac49xControl							AnswerToneOverRfc2833Enable;               /*!< This bit enables transfer of Fax/modem Answer tone over RFC 2833. This bit is applicable only if Fax/modem bypass is enabled. Once the Answer tone is detected, the channel transmits three RFC 2833 events to the network. When the receiving channel receives the Answer tone events, it sends 'IBS Detection Event' and ignores these packets (i.e., Answer tone is not played to the TDM side). @li 0 = Disabled (default) @li 1 = Enabled*/
	Tac49xModemRelayHighAvailabilityOptions	ModemRelayHighAvailabilityOptions;         /*!< Reserved for internal use. Must be set to 0. */
	int										SseRedundancyDepth;                        /*!< Reserved for internal use. Must be set to 3. */
	Tac49xControl							EventIdEnable;                             /*!< Reserved for internal use. Must be set to 0. */
	int										EventsChannelId;                           /*!< Reserved for internal use. Must be set to 0. */
	Tac49xControl							RtcpRxAuthenticationValidationEnable;      /*!< Reserved for internal use. Must be set to 0.*/
	Tac49xControl							RtpRxAuthenticationValidationEnable;       /*!< Reserved for internal use. Must be set to 0.*/
	Tac49xControl							SrtpTunnelingEnable;                       /*!< Reserved for internal use. Must be set to 0.*/
	Tac49xControl							RtcpForwardingEnable;                      /*!< Reserved for internal use. Must be set to 0.*/
	Tac49xControl							AsyncPacketToPacketHeaderModifyDisable;    /*!< Reserved for internal use. Must be set to 0.*/
	Tac49xControl							AsyncP2pConfiguredPayloadTypeForwardingEnable;        /*!< Reserved for internal use. Must be set to 0.*/
	Tac49xControl							ConfigureVbdEventsRelayPayloadTypeAsUnknown;          /*!< This field configures whether the ::Tac49xActivateOrUpdateRtpChannelConfigurationAttr.RxVbdEventsRelayPayloadType configuration field is valid. If the Host application does not know the value of the payload type by negotiation, it should be set to 1. <ul> <li> 0 = Payload Type is known. <li> 1 = Payload Type is not known. </ul> @n Default = 0. */
	Tac49xControl							ConfigureAsyncP2pTransparentPayloadTypeAsUnknown;     /*!< Reserved for internal use. Must be set to 0.*/
	Tac49xControl							AsyncP2pUnconfiguredPayloadTypeForwardingEnable;	  /*!< Reserved for internal use. Must be set to 0.*/
	Tac49xControl							ConfigureReceivedPayloadTypeAsUnknown;                /*!< This field configures whether the ::Tac49xActivateOrUpdateRtpChannelConfigurationAttr.RxPayloadType configuration field (the decoder payload type) is valid. If the Host application does not know the value of the payload type by negotiation, it should be set to 1. <ul> <li> 0 = Payload Type is known. <li> 1 = Payload Type is not known. </ul> @n Default = 0. */
	Tac49xControl							ConfigureComfortNoisePayloadTypeAsUnknown;            /*!< This field configures whether the ::Tac49xActivateOrUpdateRtpChannelConfigurationAttr.PcmAdPcmComfortNoiseRtpPayloadType configuration field is valid. If the Host application does not know the value of the payload type by negotiation, it should be set to 1. <ul> <li> 0 = Payload Type is known. <li> 1 = Payload Type is not known. </ul> @n Default = 0. */
	Tac49xControl							ConfigureRfc2833PayloadTypeAsUnknown;                 /*!< This field configures whether the ::Tac49xActivateOrUpdateRtpChannelConfigurationAttr.RxSignalingRfc2833PayloadType configuration field is valid. If the Host application does not know the value of the payload type by negotiation, it should be set to 1. <ul> <li> 0 = Payload Type is known. <li> 1 = Payload Type is not known. </ul> @n Default = 0. */
	Tac49xControl							ConfigureRfc2198PayloadTypeAsUnknown;                 /*!< This field configures whether the ::Tac49xActivateOrUpdateRtpChannelConfigurationAttr.Rfc2198PayloadType configuration field is valid. If the Host application does not know the value of the payload type by negotiation, it should be set to 1. <ul> <li> 0 = Payload Type is known. <li> 1 = Payload Type is not known. </ul> @n Default = 0. */
	Tac49xControl							ConfigureNoOpPayloadTypeAsUnknown;                    /*!< This field configures whether the ::Tac49xActivateOrUpdateRtpChannelConfigurationAttr.NoOperationPayloadType configuration field is valid. If the Host application does not know the value of the payload type by negotiation, it should be set to 1. <ul> <li> 0 = Payload Type is known. <li> 1 = Payload Type is not known. </ul> @n Default = 0. */
	Tac49xControl							ConfigureFaxBypassPayloadTypeAsUnknown;               /*!< This field configures whether the ::Tac49xActivateOrUpdateRtpChannelConfigurationAttr.FaxBypassPayloadType configuration field is valid. If the Host application does not know the value of the payload type by negotiation, it should be set to 1. <ul> <li> 0 = Payload Type is known. <li> 1 = Payload Type is not known. </ul> @n Default = 0. */
	Tac49xControl							ConfigureDataBypassPayloadTypeAsUnknown;              /*!< This field configures whether the ::Tac49xActivateOrUpdateRtpChannelConfigurationAttr.DataBypassPayloadType configuration field is valid. If the Host application does not know the value of the payload type by negotiation, it should be set to 1. <ul> <li> 0 = Payload Type is known. <li> 1 = Payload Type is not known. </ul> @n Default = 0. */
	Tac49xControl							ConfigureT38PayloadTypeAsUnknown;                     /*!< This field configures whether the ::Tac49xActivateOrUpdateRtpChannelConfigurationAttr.RxT38PayloadType configuration field is valid. If the Host application does not know the value of the payload type by negotiation, it should be set to 1. <ul> <li> 0 = Payload Type is known. <li> 1 = Payload Type is not known. </ul> @n Default = 0. */
	Tac49xControl							ConfigureFecPayloadTypeAsUnknown;                     /*!<  Reserved for internal use. Must be set to 0.*/
	int										DenialOfServiceLatchingWindowSize_x20msec;            /*!< Reserved for internal use. Must be set to 0.*/
	Tac49xControl							PacketForwardingXrTerminationEnable;                  /*!< Reserved for internal use. Must be set to 0.*/
	Tac49xControl							ModifyNobOfIncomingP2pForwardingChannelEnable;        /*!< Reserved for internal use. Must be set to 0.*/
	Tac49xControl							ModifyRfc2198RedundancyOfIncomingP2pForwardingChannelEnable;   /*!< Reserved for internal use. Must be set to 0.*/
	Tac49xRfc4733TimestampIncrement			Rfc4733TimestampIncrement;									   /*!< The RTP timestamp resolution in RFC 4733 relay packets. This field is only relevant only for wideband coders. Refer to ::Tac49xRfc4733TimestampIncrement. Default = ACCORDING_TO_NB_FRAME_SIZE (0) */
	Tac49xControl							ForwardInvalidatedPacketsEnable;							   /*!< Reserved for internal use. Must be set to 0. */
	int										RtpInterleavingLevel;										   /*!< This field is relevant when the RTP redundancy is enabled (::Tac49xActivateOrUpdateRtpChannelConfigurationAttr.RtpRedundancy > 0). It defines the packet interleaving inside the RFC 2198 structure. Interleaving Level 0 indicates regular redundancy. <ul> <li> 0 = Disabled <li> 1-4 = Interleaving level. </ul> @n Default = 0. */
	int										Rfc4733Interval;											   /*!< The interval between RFC 4733 packet transmissions. <ul> <li> 0 = The interval is set to the encoder's basic frame size. <li> 1 - 15 = The interval is in 10 msec resolution. The range is 10 msec - 150 msec. Each value is rounded up to a multiple of the encoder's basic frame size. </ul> @n Default = 0.*/ 
	int										RtcpEventsMeanTxInterval;									   /*!< This field defines the mean interval between two transmitted RTCP events (::Tac49xRtcpTransmitStatus) to the host (in msec). If it is set to 0, RTCP events to the host are generated at the same time as RTCP packets are transmitted to the network. @n Default = 0. */
    U8                                      RtpCanonicalName[AC49X_MAX_RTP_CANONICAL_NAME_STRING_LENGTH];  /*!< RTP canonical name. Should be unique among all participants within the RTP session*/
	Tac49xControl							VoiceFec;											  /*!< Reserved for internal use. Must be set to 0.*/			   
	Tac49xControl							BypassFec;											  /*!< Reserved for internal use. Must be set to 0.*/			   
	int										FecN;												  /*!< Reserved for internal use. Must be set to 0.*/		   
	int										FecK;												  /*!< Reserved for internal use. Must be set to 0.*/		   
	int										FecPayloadType;										  /*!< Reserved for internal use. Must be set to 0.*/		   
	Tac49xControl							HdrMan;												  /*!< Reserved for internal use. Must be set to 0.*/
	Tac49xControl							PtMan;												  /*!< Reserved for internal use. Must be set to 0.*/
	Tac49xControl							SidMan;												  /*!< Reserved for internal use. Must be set to 0.*/
	int										LatchingCounter1;									  /*!< Reserved for internal use. Must be set to 0.*/
	int										LatchingCounter2;									  /*!< Reserved for internal use. Must be set to 0.*/
	Tac49xLatchingMode						LatchingMode;										  /*!< Reserved for internal use. Must be set to 0.*/
	int										RtpPadding;		                		              /*!< This field defines the number of bytes by which the RTP payload size will be aligned. The RTP payload is padded with zeros at the end of the packet. <ul> <li> 0 - RTP padding is disabled (default) <li> 1 - 4 bytes  <li> 2 - 8 bytes <li> 3 - 12 bytes <li> 4 - 16 bytes <li> 5 - 20 bytes <li> 6 - 24 bytes <li> 7 - 28 bytes <li> 8 = 32 bytes </ul> */
	Tac49xControl							PacketForwardingJBEnable;		                      /*!< Reserved for internal use. Must be set to 0.*/
	Tac49xJitterEstimationType				RtcpJitterEstimationType;                             /*!< Reserved for internal use. Must be set to 0.*/	
	int										RtpTransmitTimeout;									  /*!< Reserved for internal use. Must be set to 0.*/	
	} Tac49xActivateOrUpdateRtpChannelConfigurationAttr;

    /** @} */

    /**
     * @defgroup Tac49xCloseRtpChannelConfigurationAttr Tac49xCloseRtpChannelConfigurationAttr
     * @ingroup CHANNEL_CFG_STRUCTURES
     * @brief
     * This structure is used as an input to the ::Ac49xCloseRegularRtpChannelConfiguration and
     * ::Ac49xCloseMediatedRtpChannelConfiguration functions. It defines the sequence in which an RTP channel is closed.
     * @{
     */

    /**
     * <BR>
     * This structure is used as an input to the ::Ac49xCloseRegularRtpChannelConfiguration and
     * ::Ac49xCloseMediatedRtpChannelConfiguration functions. It defines the sequence in which an RTP channel is closed.
     */

typedef struct
    {
    Tac49xControl		SendRtcpByePacket;                                                /*!< Send RTCP Bye packet. @n 0 = Do not send bye packet. @n 1 = Send RTCP Bye packet to the remote side.*/
    Tac49xControl		SendReason;                                                       /*!< Send reason. @li CONTROL__DISABLE = Do not send Reason with Bye packet (default). @li CONTROL__ENABLE = Send Reason with Bye packet.*/
	Tac49xControl		SendSrtpStateEventEnable;                                         /*!< Send SRTP State event. <ul> <li> 0 = Do not send SRTP state event. <li> 1= Send SRTP state event. </ul> @n Default = 0. */
	U8					ReasonMessageLength;                                              /*!< The reason length, in bytes. This parameter is relevant only if SendReason is set to CONTROL__ENABLE. The maximum Reason length is 64.*/
	U8					LeavingReasonMessage[AC49X_RTP_MAX_LEAVING_REASON_MESSAGE_SIZE];  /*!< The reason for leaving, as defined in RFC 3550.*/
    } Tac49xCloseRtpChannelConfigurationAttr;

    /** @} */

    /**
     * @defgroup Tac49xActivateOrUpdateAtmChannelConfigurationAttr Tac49xActivateOrUpdateAtmChannelConfigurationAttr
     * @ingroup CHANNEL_CFG_STRUCTURES
     * @brief
     * Defines the ATM protocol configuration parameters.
     * @{
     */

    /**
     * <BR>
     * This structure defines the ATM protocol configuration parameters.
     */

typedef struct
    {
    Tac49xAtmProtocol						Protocol;                              /*!< ATM protocol.@li ATM_PROTOCOL__AAL2_I_366_2 = All fields are enabled (default). @li ATM_PROTOCOL__3GPP_USER_PLANE = The channel accepts the 3GPP UP configuration command. Only AAL2TxCID, AAL2RxCID and SC fields are enabled. @li ATM_PROTOCOL__AAL1_I_363_1 = In this mode, the channel works in ATM AAL1. The only field that is enabled is G711 CT.*/
    Tac49xG711CoderType						G711CoderType;                         /*!< G.711 Coder Type. Default = A-Law.*/
	Tac49xBrokenConnectionActivationMode	BrokenConnectionActivationMode;        /*!< Broken Connection Event Activation Mode. Default = ACTIVATE_AFTER_FIRST_RTP_PCKAET_ARRIVAL;*/
	U16										BrokenConnectionEventTimeout_x20msec;  /*!< Timeout that defines when a Broken Connection Event occurs. Time resolution is 20 msec. For example: 500 -> 10 sec. Default = 500.*/
    Tac49xProfileGroup						ProfileGroup;                          /*!< Configures the profile group. Default = 0 (ITU).*/
    int										ProfileEntry;                          /*!< Configures the entry line # in the specific profile to select a specific coder. @n Default = 0. @n Refer to <A HREF="ITU Profile ID Descriptions.html">Profile Entry Descriptions</A>*/
    Tac49xSilenceCompressionMode			SilenceCompressionMode;                /*!< Silence Compression mode -  used only when SID is supported by profile or RFCI. Default = ENABLE_NOISE_ADAPTATION_ENABLE.*/
    int										ProfileId;                             /*!< User selection of the set of voice coders according to the specific profile group. @n Default = 0. @n Refer to <A HREF="ITU Profile ID Descriptions.html">Profile ID Descriptions</A>*/
    int										AtmAdaptionLayer2TxChannelId;          /*!< ATM Adaptation Layer 2 Transmit Channel ID. @n 8 bits. Range: 0 - 255.*/
    int										AtmAdaptionLayer2RxChannelId;          /*!< ATM Adaptation Layer 2 Receive Channel ID. @n 8 bits. Range: 0 - 255.*/
    } Tac49xActivateOrUpdateAtmChannelConfigurationAttr;

    /** @} */

    /**
     * @defgroup Tac49xT38ConfigurationAttr Tac49xT38ConfigurationAttr
     * @ingroup CHANNEL_CFG_STRUCTURES
     * @brief
     * Defines fields for T.38 configuration.
     * @{
     */

    /**
     * <BR>
     * This structure defines fields for T.38 configuration using the ::Ac49xT38ChannelConfiguration function.
     */

typedef struct
	{

    Tac49xT38EncapsulationIntoRtp		RtpEncapsulation;               /*!< T.38 Encapsulation to RTP. Refer to ::Tac49xT38EncapsulationIntoRtp. @n Default = 0.*/
    Tac49xT38PacketizationPeriod		PacketizationPeriod;            /*!< T.38 Packetization period. Default = 40_msec.*/
	Tac49xControl						JbigImageTranscoding;           /*!< JBIG image transcoding. @li CONTROL__DISABLE = Disable (default). @li CONTROL__ENABLE = Enable (not supported).*/
	Tac49xControl						T4MmrTranscoding;               /*!< T.4 MMR Transcoding. @li CONTROL__DISABLE = Disable (default). @li CONTROL__ENABLE = Enable (not supported).*/
	Tac49xControl						T4FillBitRemoval;               /*!< T.4 Fill Bit Removal. @li CONTROL__DISABLE = Disable (default). @li CONTROL__ENABLE = Enable.*/
	int									MaxDatagramSize_In4BytesStep;   /*!< Maximum Size of Datagram in 32 bit words that the far end T.38 gateway can receive. @li 0 = Unlimited (default). @li 1...255 = Far-End gateway limit.*/
	Tac49xT38Version					T38Version;                     /*!< @li 0 = 1998 ASN.1 syntax (default). @li 1 = 1998 ASN.1 syntax, TPKT support. @li 2 = 2002 ASN.1 syntax, TPKT enabled. @li 3 = V.34, V.33 support, 2002 ASN.1 syntax and TPKT enabled. @li 4-15 = Reserved.*/
	Tac49xT38UdpErrorCorrectionMethod	UdpErrorCorrectionMethod;       /*!< Error recovery choice for output UDPTL packets. @li 0 = Redundant secondary IFP packets (default). @li 1 = FEC (Forward Error Correction).*/
	Tac49xImageDataRedundancyLevel		ImageDataRedundancyLevel;       /*!< Redundancy for output Image Data (2400�14400 bps). @li 0 = No Redundancy (Default). @li 1-3 = Redundancy level 1-3.*/
	Tac49xT30ControlDataRedundancyLevel	T30ControlDataRedundancyLevel;  /*!< Redundancy for output T.30 Control Data (300 bps). @n (Default = 0). @li 0 = No Redundancy. @li 1-7 = Redundancy levels 1-7.*/
    Tac49xT38NoOperationPeriod          NoOperationPeriod;              /*!< No Operation Period, in seconds. @li 0 = NOP Handler is disabled (default). @li 1-31 = Period (in seconds) of retransmission of a previously sent T.38 UDPTL packet, when the fax signal in the TDM input of the AC49x is absent or doesn't carry T.30 data.*/
	Tac49xT38MaxRate					MaximumDataRate;                /*!< Maximum data rate of fax image signals. Refer to ::Tac49xT38MaxRate. @n Default = TAKEN_FROM_OPEN_OR_UPDATE_CHANNEL_COMMAND (0). */
	Tac49xFaxRelayProtocol				FaxRelayProtocol;               /*!< Fax Relay Protocol. Refer to ::Tac49xFaxRelayProtocol. @n Default = TAKEN_FROM_OPEN_OR_UPDATE_CHANNEL_COMMAND (0). */
	unsigned int						MaxFaxBuffer;					/*!< Reserved for internal use. Must be set to 0.*/
	} Tac49xT38ConfigurationAttr;

    /** @} */

    /**
     * @defgroup Tac49xActivateOrDeactivate3WayConferenceConfigurationAttr Tac49xActivateOrDeactivate3WayConferenceConfigurationAttr
     * @ingroup CHANNEL_CFG_STRUCTURES
     * @brief
     * Defines the three-way conference parameters that activate or deactivate the 3-way conferencing feature.
     * @{
     */

    /**
     * <BR>
     * This structure defines the three-way conference parameters that activate or deactivate the 3-way 
     * conferencing feature.
     */

typedef struct
	{
	int							ConferenceChannelMate;              /*!< If two of the conference channels are on the same core, this parameter describes the channel ID of the channel with which the conference is performed. Default = 0.*/
	int							ConferenceChannelMate2;				/*!< If the ::Tac49xActivateOrDeactivate3WayConferenceConfigurationAttr.Mode field is set to 5, this field describes the channel ID of the second channel to which the conference is connected. This channel is the TDM channel of the conference.*/
	Tac49x3WayConferenceMode	Mode;                               /*!< 3-Way Conference Mode. @n Default = _3_WAY_CONFERENCE_MODE__DISABLE.*/
	Tac49xControl				CrossGainMatrixEnable;              /*!< This field enables the cross gain matrix. This matrix determines which participant can hear who. <ul> <li> 0 = Cross gain matrix is disabled <li> 1 = Cross gain matrix is enabled. </ul> @n Default = 0. */
	unsigned char				Conference1TransmitPayloadType;     /*!< RTP header payload type. Available in modes 2, 4 only. Default = 8.*/
	unsigned char				Conference2TransmitPayloadType;     /*!< RTP header payload type. Available in mode 4 only. Default = 8.*/
	unsigned short				Conference1TransmitSequenceNumber;  /*!< RTP header sequence number. Available in modes 2, 4 only. Default = 0.*/
	unsigned short				Conference2TransmitSequenceNumber;  /*!< RTP header sequence number. Available in mode 4 only. Default = 0.*/
	unsigned int				Conference1RtpSsrc;                 /*!< 32-bit random number representing the SSRC field in the transmitted RTP packet. Available in modes 2, 4 only. Default = 1.*/
	unsigned int				Conference2RtpSsrc;                 /*!< 32-bit random number representing the SSRC field in the transmitted RTP packet. Available in mode 4 only. Default = 2.*/
	Tac49xControl				CrossGainMatrix11;                  /*!< Cross Gain Matrix element. <ul> <li> 0 = member 1 does not hear member 1 (itself) <li> 1 = member 1 hears  member 1 (itself) </ul> @n refer to <A HREF="ConfMemberToMode.html">Members According to Mode</A> table to associate the member to a channel. @n Default = 0. */
	Tac49xControl				CrossGainMatrix12;                  /*!< Cross Gain Matrix element. <ul> <li> 0 = member 1 does not hear member 2  <li> 1 = member 1 hears  member 2  </ul> @n refer to <A HREF="ConfMemberToMode.html">Members According to Mode</A> table to associate the member to a channel. @n Default = 1. */
	Tac49xControl				CrossGainMatrix13;                  /*!< Cross Gain Matrix element. <ul> <li> 0 = member 1 does not hear member 3  <li> 1 = member 1 hears  member 3  </ul> @n refer to <A HREF="ConfMemberToMode.html">Members According to Mode</A> table to associate the member to a channel. @n Default = 1. */
	Tac49xControl				CrossGainMatrix14;					/*!< Cross Gain Matrix element. <ul> <li> 0 = member 1 does not hear member 4  <li> 1 = member 1 hears  member 4  </ul> @n refer to <A HREF="ConfMemberToMode.html">Members According to Mode</A> table to associate the member to a channel. @n Default = 1. */
	Tac49xControl				CrossGainMatrix21;                  /*!< Cross Gain Matrix element. <ul> <li> 0 = member 2 does not hear member 1  <li> 1 = member 2 hears  member 1  </ul> @n refer to <A HREF="ConfMemberToMode.html">Members According to Mode</A> table to associate the member to a channel. @n Default = 1. */
	Tac49xControl				CrossGainMatrix22;                  /*!< Cross Gain Matrix element. <ul> <li> 0 = member 2 does not hear member 2 (itself) <li> 1 = member 2 hears  member 2 (itself) </ul> @n refer to <A HREF="ConfMemberToMode.html">Members According to Mode</A> table to associate the member to a channel. @n Default = 0. */
	Tac49xControl				CrossGainMatrix23;                  /*!< Cross Gain Matrix element. <ul> <li> 0 = member 2 does not hear member 3  <li> 1 = member 2 hears  member 3  </ul> @n refer to <A HREF="ConfMemberToMode.html">Members According to Mode</A> table to associate the member to a channel. @n Default = 1. */
	Tac49xControl				CrossGainMatrix24;					/*!< Cross Gain Matrix element. <ul> <li> 0 = member 2 does not hear member 4  <li> 1 = member 2 hears  member 4  </ul> @n refer to <A HREF="ConfMemberToMode.html">Members According to Mode</A> table to associate the member to a channel. @n Default = 1. */
	Tac49xControl				CrossGainMatrix31;                  /*!< Cross Gain Matrix element. <ul> <li> 0 = member 3 does not hear member 1  <li> 1 = member 3 hears  member 1  </ul> @n refer to <A HREF="ConfMemberToMode.html">Members According to Mode</A> table to associate the member to a channel. @n Default = 1. */
	Tac49xControl				CrossGainMatrix32;                  /*!< Cross Gain Matrix element. <ul> <li> 0 = member 3 does not hear member 2  <li> 1 = member 3 hears  member 2  </ul> @n refer to <A HREF="ConfMemberToMode.html">Members According to Mode</A> table to associate the member to a channel. @n Default = 1. */
	Tac49xControl				CrossGainMatrix33;                  /*!< Cross Gain Matrix element. <ul> <li> 0 = member 3 does not hear member 3 (itself) <li> 1 = member 3 hears  member 3 (itself) </ul> @n refer to <A HREF="ConfMemberToMode.html">Members According to Mode</A> table to associate the member to a channel. @n Default = 0. */
	Tac49xControl				CrossGainMatrix34;					/*!< Cross Gain Matrix element. <ul> <li> 0 = member 3 does not hear member 4  <li> 1 = member 3 hears  member 4  </ul> @n refer to <A HREF="ConfMemberToMode.html">Members According to Mode</A> table to associate the member to a channel. @n Default = 1. */
	Tac49xControl				CrossGainMatrix41;					/*!< Cross Gain Matrix element. <ul> <li> 0 = member 4 does not hear member 1  <li> 1 = member 4 hears  member 1  </ul> @n refer to <A HREF="ConfMemberToMode.html">Members According to Mode</A> table to associate the member to a channel. @n Default = 1. */                
	Tac49xControl				CrossGainMatrix42;					/*!< Cross Gain Matrix element. <ul> <li> 0 = member 4 does not hear member 2  <li> 1 = member 4 hears  member 2  </ul> @n refer to <A HREF="ConfMemberToMode.html">Members According to Mode</A> table to associate the member to a channel. @n Default = 1. */                
	Tac49xControl				CrossGainMatrix43;					/*!< Cross Gain Matrix element. <ul> <li> 0 = member 4 does not hear member 3  <li> 1 = member 4 hears  member 3  </ul> @n refer to <A HREF="ConfMemberToMode.html">Members According to Mode</A> table to associate the member to a channel. @n Default = 1. */
	Tac49xControl				CrossGainMatrix44;					/*!< Cross Gain Matrix element. <ul> <li> 0 = member 4 does not hear member 4 (itself) <li> 1 = member 4 hears  member 4 (itself) </ul> @n refer to <A HREF="ConfMemberToMode.html">Members According to Mode</A> table to associate the member to a channel. @n Default = 0. */                
	} Tac49xActivateOrDeactivate3WayConferenceConfigurationAttr;

    /** @} */

    /**
     * For each AMR rate (defined in ::Tac49x3GppRate or ::Tac49x3GppWbRate), the Valid field (ENABLED or DISABLED) must be defined. If the Valid field is 
     * defined as ENABLED, an RFCI must be defined. The range of the RFCIs is 0 - 63. For details on the 
     * RFCIs, refer to the 3GPP TS25.415 standard.
     */

typedef struct
	{
	Tac49xControl Valid;        /*!< @li CONTROL__DISABLE = Determines that the rate specified in the field 'FieldofRate', is disabled (default). @li CONTROL__ENABLE = If Valid = ENABLED, provide an RFCI (specified in the field 'FieldofRate').*/
	U8			  FieldOfRate;  /*!< With this field, provide an RFCI for a rate whose 'Valid' field is configured ENABLED.*/
	} Tac49xRfciStruct;

    /**
     * @defgroup Tac49x3GppUserPlaneInitializationConfigurationAttr Tac49x3GppUserPlaneInitializationConfigurationAttr
     * @ingroup CHANNEL_CFG_STRUCTURES
     * @brief
     * Defines the 3GPP UP protocol configuration parameters.
     * @{
     */

    /**
     * <BR>
     * This structure defines the 3GPP UP protocol configuration parameters.
     */

typedef struct
	{
	Tac49x3GppMode					Mode;                                          /*!< ST - Support Mode or Transparent Mode. @li _3GPP_MODE__SUPPORT = Support Mode (default). @li _3GPP_MODE__TRANSPARENT = Transparent Mode. The fields that are enabled in this mode: Local rate. In this mode the voice packets are sent with no PDU header.*/
	Tac49x3GppSupportModeType		SupportModeType;                               /*!< Support Mode Type (the field 'Mode' must be configured as _3GPP_MODE_SUPPORT). @li _3GPP_SUPPORT_MODE_TYPE__SLAVE = Slave Support Mode (default). @n Fields enabled in this mode are: DeliveryOfErroneousSDUs, G711CoderType and UserPlaneVersion. In slave mode, the channel waits for the initialization packet from the master side. @li _3GPP_SUPPORT_MODE_TYPE__MASTER = Master Support Mode. All fields are enabled. The master sends the initialization packet to the slave side, and waits for an Ack. @li _3GPP_SUPPORT_MODE_TYPE__PROPRIETARY = Proprietary Support Mode, used to play announcements/Call Progress Tones during mediation. All fields are enabled. In this mode, the protocol starts to send voice packets immediately, according to the initialization fields.*/
	Tac49xControl					DeliveryOfErroneousSDUs;                       /*!< Delivery of erroneous SDUs. Default = CONTROL__ENABLE.*/
	Tac49x3GppProtocolDataUnitType	ProtocolDataUnitType;                          /*!< Using 3Gpp Protocol Data Unit Type 0/1. @n Protocol Data Unit Type 0 (default).*/
	Tac49xControl					ErrorTransmissionDisable;                      /*!< Disable PDU Type 14 error transmission to the remote side. Default = CONTROL__DISABLE (error enabled).*/
	Tac49xRfc3267Coder				Coder;                                         /*!< This field configures the coder (AMR or AMR-WB). Refer to ::Tac49xRfc3267Coder.  */

	Tac49xControl					SupporetedUserPlaneProtocolVersion_Release99;  /*!< Enables support for protocol version release 99. Default = CONTROL__ENABLE.*/
	Tac49xControl					SupporetedUserPlaneProtocolVersion_Release4;   /*!< Enables support for protocol version release 4. Default = CONTROL__ENABLE.*/
	Tac49xControl					SupporetedUserPlaneProtocolVersion_Release5;   /*!< Enables support for protocol version release 5. Default = CONTROL__DISABLE.*/
	U8								RateControlAcknowledgeTimeout;	               /*!< This timeout (unsigned byte) is used while waiting for an acknowledgement after a rate control packet is sent. Resolution of the timer is 20 msec. For example: 25 > 25*20 = 500 msec. Default = 25.*/
	U8								InitializationAcknowledgeTimeout;              /*!< This timeout (unsigned byte) is used while waiting for an acknowledgement after an Init packet is sent. Resolution of the timer is 20 msec. For example: 25 > 25*20 = 500 msec. Default = 25.*/
	int								LocalRate;                                     /*!< The AMR or AMR-WB local rate (the local rate of the encoder). Refer to ::Tac49x3GppRate for the AMR rates and ::Tac49x3GppWbRate for the AMR-WB rates. @n Default = _3GPP_RATE__12_2_KBPS for AMR or _3GPP_RATE__23_85_KBPS for AMR-WB. */
	int								RemoteRate;                                    /*!< The AMR or AMR-WB remote rate. The remote rate is the rate that the encoder on the other side should use. This field is enabled only in _3GPP_SUPPORT_MODE_TYPE_MASTER mode. The master sends the remote rate RFCI first in the initialization packet, so that the slave starts to send voice packets in this rate, after initialization. Refer to ::Tac49x3GppRate for the AMR rates and ::Tac49x3GppWbRate for the AMR-WB rates.  @n Default = _3GPP_RATE__12_2_KBPS for AMR or _3GPP_RATE__23_85_KBPS for AMR-WB. */
	Tac49xG711CoderType				G711CoderType;                                 /*!< G.711 coder type. Default = G711_CODER_TYPE__A_LAW.*/
	Tac49xRfciStruct				Rfci[MAX_NUMBER_OF_3GPP_RFCI];                 /*!< RFCI array. Each entry in the array corresponds to the AMR rate defined by the Tac49x3GppRate field.*/
	} Tac49x3GppUserPlaneInitializationConfigurationAttr;

    /** @} */

    /**
     * This structure defines the TFI field of each 3GPP Iub AMR rate.
     */

typedef struct
	{
	U8			  Tfi0; /*!< TFI 0 per 3GPP Iub AMR rate. */
	U8			  Tfi1; /*!< TFI 1 per 3GPP Iub AMR rate. */
	U8			  Tfi2; /*!< TFI 2 per 3GPP Iub AMR rate. */
	Tac49xControl Valid; /*!< This bit defines whether the 3GPP Iub AMR rate is supported. */
	} Tac49xTfiStruct;

    /**
     * @defgroup Tac49xActivateOrUpdate3GppIubChannelConfigurationAttr Tac49xActivateOrUpdate3GppIubChannelConfigurationAttr
     * @ingroup CHANNEL_CFG_STRUCTURES
     * @brief
     * Defines the 3GPP Iub protocol configuration parameters.
     * @{
     */

    /**
     * <BR>
     * This structure defines the 3GPP Iub protocol configuration parameters.
     */
typedef struct
	{
    Tac49xControl                           Crc;                                       /*!< Payload CRC. @li 0 = Payload CRC is not calculated @li 1 = Payload CRC is calculated (default) */
	Tac49xControl				            TransmitNoDataPacketsDuringSilence;        /*!< NO Data sending mode. @li 0 = NO Data packets are not sent @li 1 = NO Data packets are sent during silence (default) */
	Tac49xControl				            SilenceCompression;                        /*!< Silence Compression @li 0 = Disable (default) @li 1 = Enable */
    Tac49xBrokenConnectionActivationMode	BrokenConnectionActivationMode;            /*!< Broken Connection Event Activation Mode. Default = ACTIVATE_AFTER_FIRST_RTP_PCKAET_ARRIVAL.*/
    U16										BrokenConnectionEventTimeout_x20msec;      /*!< Timeout that defines when a Broken Connection Event occurs. @n Time resolution is 20 msec. For example: 500 -> 10 sec. Default = 500.*/
    Tac49x3GppRate                          LocalRate;                                 /*!< The AMR encoder local rate. */  
	Tac49xTfiStruct				Tfi[MAX_NUMBER_OF_3GPP_IUB_TFIS];                      /*!< TFI array. Each entry in the array corresponds to the AMR rate defined by the Tac49xTfiStruct structure. */
	} Tac49xActivateOrUpdate3GppIubChannelConfigurationAttr;

   /** @} */

typedef struct
    {
	Tac49xRfc3267RedundancyLevel	        RedundancyLevel;            /*!< AMR redundancy level <ul> <li> 0 = No Redundancy (default) <li> 1-5 = Redundancy level 1-5 <li> 6-7 = Reserved. </ul> */
	U32				                        Rate;                       /*!< AMR Rate policy */
	Tac49xRfc3267FrameLossRatioThreshold	FrameLossRatioThreshold;    /*!< FLR (Frame Loss Ratio) threshold */   
	Tac49xRfc3267FrameLossRatioHysteresis	FrameLossRatioHysteresis;   /*!< FLR (Frame Loss Ratio) threshold hysteresis */
    }Tac49xRfc3267CodecModeAttr;

    /**
     * @defgroup Tac49xRfc3267AmrInitializationConfigurationAttr Tac49xRfc3267AmrInitializationConfigurationAttr
     * @ingroup CHANNEL_CFG_STRUCTURES
     * @brief
     * Defines the RFC 3267 protocol configuration parameters.
     * @{
     */

    /**
     * <BR>
     * This structure defines the RFC 3267 protocol configuration parameters.
     */
typedef struct
	{
	Tac49xControl			                    OctetAlignedEnable;  /*!< @li CONTROL__DISABLE = Bandwidth efficient operation. @li CONTROL__ENABLE = Octet aligned operation (default).*/
	Tac49xChangeModeNeighbor                    ChangeModeNeighbor;  /*!< Determines the Change Neighbor Mode. Default = CHANGE_MODE_NEIGHBOR__CHANGED_TO_ANY_RATE.*/
	Tac49xControl			                    CrcEnable;           /*!< Cyclic Redundancy Check enabled. @li CONTROL__DISABLE = Not used (default). @li CONTROL__ENABLE = CRC added.*/
	U8						                    ModeChangePeriod;    /*!< Default = 0. Specifies a number of frame blocks, i.e., the interval at which codec mode changes are allowed. If set to 0, mode changes are allowed at any time.*/
	U8						                    LocalRate;           /*!< AMR or WB-AMR local rate - the encoder local rate.  Refer to the table <A HREF="AMR and Wide-Band AMR Rates.html">AMR and AMR-WB rates </A>. Default = 7 (AMR 12.2 kbps, AMR-WB 23.05 kbps).*/
	Tac49xControl			                    RemoteRateEnable;    /*!< Enables Remote Rate control. Default = CONTROL__DISABLE.*/
	U8						                    RemoteRate;          /*!< AMR or AMR-WB Remote Rate. This parameter is used to set the rate of the remote side at initialization. This field is enabled according to RemoteRateEnable field. Refer to the table <A HREF="AMR and Wide-Band AMR Rates.html">AMR and AMR-WB rates </A>. Default = 7 (AMR 12.2 kbps, AMR-WB 23.05 kbps).*/
    Tac49xRfc3267Coder                          Coder;               /*!< @li 0 = AMR. @li 1 = AMR-WB */ 
    Tac49xRfc3267RedundancyLevel                RedundancyLevel;                /*!< AMR/AMR-WB redundancy level, according to RFC 3267. @li 0 = No Redundancy (default) @li 1-5 = Redundancy level 1-5 @li 6-7 = Reserved */
   	Tac49xRfc3267AutomaticRateControlMode		AutomaticRateControlMode;       /*!< @li 0 = The local rate automatically changes to the remote side's CMR value (default). @li 1 = Local rate automatically changes to the remote side's CMR value only if it is higher than the remote CMR value. */
	Tac49xControl				                TransmitNoDataPackets;          /*!< @li 0 = 'No Data' packets aren't transmitted (default) @li 1 = 'No Data' packets are transmitted */
	U8				                            NumberOfCodecsPolicies;         /*!< Number Of Codec Modes. @li 0 = No Codec modes are defined. Redundancy and CMR fields are set according to RemoteRate and RedundancyLevel (default) @li 1-9 = The AC49xx uses 3GPP TS 44.318 to: <BR> @li Determine the CMR for the remote rate according to policies (refer to ::aCodecPolicies)  <BR> @li Change the local rate and redundancy according to the CMR obtained from the remote side and according to the policies (refer to ::aCodecPolicies). <BR> RFC 3267 Redundancy field is ignored. Each policy comprises 4 elements: (1) AMR rate (2) FLR threshold (3) Threshold Hysteresis (4) Redundancy level The last threshold level is used to report a handout event, indicating that voice quality is poor. */
	U16				                            DelayHandoutHysteresis_x1msec;  /*!< Hysteresis of Delay Threshold for a handout event (in msec). Default = 100. */
	U16				                            DelayHandoutThreshold_x1msec;   /*!< One-way delay value (msec) that causes the AC49xx to report a handout event. If 0, it is ignored. */
    Tac49xRfc3267CodecModeAttr aCodecPolicies[MAX_NUMBER_OF_RFC_3267_CODECS];   /*!< Each policy comprises 4 elements: (1) AMR rate (2) FLR threshold (3) Threshold Hysteresis (4) Redundancy level. The last threshold level is used to report a handout event, indicating that voice quality is poor. */  
	} Tac49xRfc3267AmrInitializationConfigurationAttr;

    /** @} */

#define ARRAY_OF__RTP_RTCP   2

    /**
     * @defgroup Tac49xPacketCableProtectionConfigurationAttr Tac49xPacketCableProtectionConfigurationAttr
     * @ingroup CHANNEL_CFG_STRUCTURES
     * @brief
     * Defines the packet cable protection configuration parameters.
     * @{
     */

    /**
     * <BR>
     * This structure defines the packet cable protection configuration parameters.
     */

typedef struct
    {
    Tac49xMediaProtectionDirection  Direction                                                                       ;  /*!< @li MEDIA_PROTECTION_DIRECTION__INBOUND_RX = This packet configures the Net to AC49x protection. @li MEDIA_PROTECTION_DIRECTION__OUTBOUND_TX = This packet configures the AC49x to Net protection.*/
    Tac49xControl                   RtpAuthentication                                                               ;  /*!< Enables the RTP  authentication.*/
    Tac49xControl                   RtpEncryption                                                                   ;  /*!< Enables the RTP  encryption.*/
    Tac49xControl                   RtcpAuthentication                                                              ;  /*!< Enables the RTCP authentication.*/
    Tac49xControl                   RtcpEncryption                                                                  ;  /*!< Enables the RTCP encryption.*/
    Tac49xControl                   AntiReplayEnable                                                                ;  /*!< Anti Replay Enable. @li CONTROL__DISABLE = The anti replay procedure in RTP (using the received time stamp) and RTCP (using the received RTCP sequence number) is disabled. @li CONTROL__ENABLE = The anti replay procedure in RTP and RTCP is enabled (default).*/
    U8                              NumberOfRtpTimeStampWrapAround                                                  ;  /*!< Number of times the RTP time stamp wraps around. It can be different to 0 if the connection begins in the middle of a session ( i.e., HA). Default = 0.*/
    Tac49xMediaAuthenticationMode   RtpAuthenticationMode                                                           ;  /*!< Configures the RTP authentication mode. Only MEDIA_AUTHENTICATION_MODE__MMH2 and MEDIA_AUTHENTICATION_MODE__MMH4 are supported.*/
    Tac49xMediaAuthenticationMode   RtcpAuthenticationMode                                                          ;  /*!< Configures the RTCP authentication mode. Only MEDIA_AUTHENTICATION_MODE__SHA1 is supported.*/
    U8                              RtpCipherKeySize                                                                ;  /*!< RTP cipher key size in bytes. Must be set to 16.*/
    U8                              RtcpCipherKeySize                                                               ;  /*!< RTCP cipher key size in bytes. Must be set to 16.*/
    U8                              RtpMessageAuthenticationCodeKeySize                                             ;  /*!< RTP Message Authentication Code key size, in bytes. Must be equal to the largest possible packet (RTP header size + payload size) in the session. Range: 1-174 bytes.*/
    U8                              RtcpMessageAuthenticationCodeKeySize                                            ;  /*!< RTCP Message Authentication Code key size, in bytes. Must be set to 20 bytes.*/
    U8                              RtpDigestSize                                                                   ;  /*!< Size, in bytes, of the Digest. The RTP MAC result, which is added to each packet, must be: @li 2 bytes for MMH2 @li 4 bytes for MMH4*/
    U8                              RtcpDigestSize                                                                  ;  /*!< Size, in bytes, of the Digest. The RTCP MAC result, which is added to each packet. Must be set to 20 or 12 bytes.*/

    U8                              RtpCipherKey                    [MAX_KEY_SIZE__RTP_CIPHER]                      ;  /*!< The RTP cipher key.*/
    U32                             RtpTimeStamp                                                                    ;  /*!< 32-bit received RTP Time Stamp. This field is valid only when Direction = MEDIA_PROTECTION_DIRECTION__INBOUND_RX.*/
    U8                              RtcpCipherKey                   [MAX_KEY_SIZE__RTCP_CIPHER]                     ;  /*!< The RTCP cipher key.*/
    U8                              RtpInitializationKey            [MAX_KEY_SIZE__RTP_INITIALIZATION]              ;  /*!< The RTP initialization key.*/
    U8                              RtpMessageAuthenticationCodeKey [MAX_KEY_SIZE__RTP_MESSAGE_AUTHENTICATION_CODE] ;  /*!< The RTP authentication key.*/
    U8                              RtcpMessageAuthenticationCodeKey[MAX_KEY_SIZE__RTCP_MESSAGE_AUTHENTICATION_CODE];  /*!< The RTCP authentication key.*/
    U32                             RtcpSequenceNumber                                                              ;  /*!< RTCP Sequence Number. It can be different to 0 if the connection begins in the middle of a session ( i.e., HA). Default = 0.*/
    } Tac49xPacketCableProtectionConfigurationAttr;

    /** @} */

    /**
     * @defgroup Tac49xSrtpProtectionConfigurationAttr Tac49xSrtpProtectionConfigurationAttr
     * @ingroup CHANNEL_CFG_STRUCTURES
     * @brief
     * Defines the SRTP configuration parameters.
     * @{
     */

    /**
     * <BR>
     * This structure defines the SRTP configuration parameters. It enables configuration of only 128 bytes master key size.
     */
typedef struct
	{
    Tac49xMediaProtectionDirection	Direction;                              /*!< When calling the function ::Ac49xRegularChannelSrtpProtectionConfiguration with this parameter set to MEDIA_PROTECTION_DIRECTION__INBOUND_RX, this call configures the Net to AC49x protection. @n When calling the function ::Ac49xRegularChannelSrtpProtectionConfiguration with this parameter set to MEDIA_PROTECTION_DIRECTION__OUTBOUND_TX, this call configures the AC49x to Net protection.*/

	Tac49xControl					RtpEncryption;                          /*!< Enables the RTP encryption. Default = CONTROL__ENABLE.*/
	Tac49xControl					RtpAuthentication;                      /*!< Enables the RTP authentication. Default = CONTROL__ENABLE.*/
	Tac49xControl					RtcpEncryption;                         /*!< Enables the RTCP encryption. Default = CONTROL__ENABLE.*/
	Tac49xControl					RtcpAuthentication;                     /*!< Enables the RTCP authentication. Default = CONTROL__ENABLE.*/
	Tac49xControl					AntiReplayEnable;                       /*!< Enables the anti replay mechanism. Default = CONTROL__ENABLE.*/
	Tac49xControl					RocUpdate;			                    /*!< This field configures whether the ::Tac49xSrtpProtectionConfigurationAttr.RtpInitRoc field should be updated following the ::Ac49xUpdateRegularChannelSrtpProtectionConfiguration command. It is valid only for the ::Ac49xUpdateRegularChannelSrtpProtectionConfiguration command. Default = CONTROL__DISABLE.*/
	U8								KeyDerivationRatePower;                 /*!< Key Derivation Rate Power. Denotes a power of 2, meaning that the actual Key Derivation Rate is 2^(KeyDerivationRatePower). 0 = key derivation occurs only in the beginning of session. If not set to 0, it must be set to a value greater than or equal to16 - meaning that the Minimum Key Derivation Rate during the session is 2^16. Default = 0.*/

	U8								RtpMessageAuthenticationCodeKeySize;    /*!< The RTP message authentication code key size, in bytes. Default = 20.*/
	U8								RtpCipherKeySize;                       /*!< The RTP Encryption Key size in bytes (currently, the 16-byte key size is supported). Default = 16.*/
	Tac49xMediaAuthenticationMode	RtpAuthenticationMode;                  /*!< The RTP authentication mode. Default = SHA1.*/
	Tac49xMediaEncryptionMode		RtpEncryptionMode;                      /*!< The RTP Encryption mode. Default = AES_CM.*/
	U8								RtpDigestSize;                          /*!< The RTP digest size in bytes. The digest is the MAC result, which is added to each packet. Default = 10.*/

	U8								RtcpMessageAuthenticationCodeKeySize;   /*!< The RTCP message authentication code key size, in bytes. Default = 20.*/
	U8								RtcpCipherKeySize;                      /*!< The RTCP Encryption Key size in bytes (currently, the 16-byte key size is supported). Default = 16.*/
	Tac49xMediaAuthenticationMode	RtcpAuthenticationMode;                 /*!< The RTCP authentication mode. Default = SHA1.*/
	Tac49xMediaEncryptionMode		RtcpEncryptionMode;                     /*!< The RTCP Encryption mode. Default = AES_CM.*/
	U8								RtcpDigestSize;                         /*!< The RTCP digest size in bytes. The digest is the MAC result, which is added to each packet. Default = 10.*/

	U8								MasterKeySize;                          /*!< The Master Key Size, in bytes. Default = 16.*/
	U8								MasterSaltSize;                         /*!< The Master Salt Size, in bytes. Default = 14.*/
	U8								SessionSaltSize;                        /*!< The Session Salt Size, in bytes. Default = 14.*/

	U16								RtpInitSequenceNumber;                  /*!< This parameter configures the RTP sequence number. It is relevant only in the inbound direction (the received RTP sequence number). The transmitted RTP sequence number is configured using the function ::Ac49xActivateRegularRtpChannelConfiguration. If set to 0, the channel is updated by the received RTP packets. In High Availability (HA) (if the connection begins in the session), it is recommended to set it to its actual value rather than 0. Default = 0.*/
	U32								RtpInitRoc;                             /*!< In the outbound  direction, this parameter configures the  transmitted ROC. In the inbound  direction, this parameter  configures the received ROC. It should be set to 0 unless the connection begins in the middle of a session ( i.e., HA). Default = 0.*/
	U32								RtcpInitIndex;                          /*!< In the outbound  direction, this parameter configures the  transmitted RTCP index. In the inbound  direction, this parameter  configures the received RTCP index. It should be set to 0 unless the connection begins in the middle of a session ( i.e., HA). Default = 0.*/
	Tac49xControl					SsrcValid;                              /*!< This parmeter configures whether the field @b Ssrc is valid. <ul> <li> 0 =  SSRC is not valid. <li> 1 = SSRC is valid. </ul> @n Default = 0. */
	U32								Ssrc;                                   /*!< The remote RTP SSRC. This parameter is valid only if the parameter @b SsrcValid = 1. In addition, it is valid only in receive direction (@b Direction = 0 ). */
	Tac49xControl					ResetCountersOnMediaProtectionAuditEvents;  /*!< This parameter configures the counters on ::Tac49xSrtpProtectionAuditEvent status packet. <ul> <li> 0 = Counters on media protection audit events are accumulative. <li> 1 = Counters on media protection audit events are reset at every event transmission. </ul> @n Default = 0. */
	Tac49xControl					ScaleSrtpModeEnable;					 /*!< Reserved for internal use. Must be set to 0. */
	
    U8								MasterKey[MAX_KEY_SIZE__MASTER];        /*!< The Master Key Data.*/
    U8								MasterSalt[MAX_KEY_SIZE__MASTER_SALT];  /*!< The Master Salt Data.*/

	U8								MasterKeyIndexSize;                        /*!< Master Key Index (MKI) field size. @li 0 = MKI disabled (default) @li Other = MKI size 1 - 4 (in bytes) */
	U8								MasterKeyIndex[MAX_MASTER_KEY_INDEX_SIZE]; /*!< 32-bit Master Key Index. When the MKI size is less than 4, MKI is taken from the MS bytes (starting from Byte 0).*/
	U8								ScaleSrtpExtendedSequenceNumber[SCALE_SRTP_EXTENDED_SEQUENCE_NUMBER_SIZE]; /*!< Reserved for internal use. Must be set to 0. */
    } Tac49xSrtpProtectionConfigurationAttr;
    /** @} */

#define Tac49xSrtpProtectionConfigurationFormat0_Attr Tac49xSrtpProtectionConfigurationAttr

    /**
     * @defgroup Tac49xSrtpProtectionConfigurationFormat1_Attr Tac49xSrtpProtectionConfigurationFormat1_Attr
     * @ingroup CHANNEL_CFG_STRUCTURES
     * @brief
     * Defines the SRTP configuration parameters.
     * @{
     */

    /**
     * <BR>
     * This structure defines the SRTP configuration parameters. It enables configuration of up to 256 bytes master key size.
     */
typedef struct
	{
    Tac49xMediaProtectionDirection	Direction;                              /*!< When calling the function ::Ac49xRegularChannelSrtpProtectionConfiguration with this parameter set to MEDIA_PROTECTION_DIRECTION__INBOUND_RX, this call configures the Net to AC49x protection. @n When calling the function ::Ac49xRegularChannelSrtpProtectionConfiguration with this parameter set to MEDIA_PROTECTION_DIRECTION__OUTBOUND_TX, this call configures the AC49x to Net protection.*/

	Tac49xControl					RtpEncryption;                          /*!< Enables the RTP encryption. Default = CONTROL__ENABLE.*/
	Tac49xControl					RtpAuthentication;                      /*!< Enables the RTP authentication. Default = CONTROL__ENABLE.*/
	Tac49xControl					RtcpEncryption;                         /*!< Enables the RTCP encryption. Default = CONTROL__ENABLE.*/
	Tac49xControl					RtcpAuthentication;                     /*!< Enables the RTCP authentication. Default = CONTROL__ENABLE.*/
	Tac49xControl					AntiReplayEnable;                       /*!< Enables the anti replay mechanism. Default = CONTROL__ENABLE.*/

	Tac49xControl					RocUpdate;			                    /*!< This field configures whether the ::Tac49xSrtpProtectionConfigurationFormat1_Attr.RtpInitRoc field should be updated following the ::Ac49xUpdateRegularChannelSrtpProtectionConfiguration_Format1 command. It is valid only for the ::Ac49xUpdateRegularChannelSrtpProtectionConfiguration_Format1 command. Default = CONTROL__DISABLE.*/
	U8								KeyDerivationRatePower;                 /*!< Key Derivation Rate Power. Denotes a power of 2, meaning that the actual Key Derivation Rate is 2^(KeyDerivationRatePower). 0 = key derivation occurs only in the beginning of session. If not set to 0, it must be set to a value greater than or equal to16 - meaning that the Minimum Key Derivation Rate during the session is 2^16. Default = 0.*/

	U8								RtpMessageAuthenticationCodeKeySize;    /*!< The RTP message authentication code key size, in bytes. Default = 20.*/
	U8								RtpCipherKeySize;                       /*!< The RTP Encryption Key size in bytes (currently, the 16-byte key size is supported). Default = 16.*/
	Tac49xMediaAuthenticationMode	RtpAuthenticationMode;                  /*!< The RTP authentication mode. Default = SHA1.*/
	Tac49xMediaEncryptionMode		RtpEncryptionMode;                      /*!< The RTP Encryption mode. Default = AES_CM.*/
	U8								RtpDigestSize;                          /*!< The RTP digest size in bytes. The digest is the MAC result, which is added to each packet. Default = 10.*/

	U8								RtcpMessageAuthenticationCodeKeySize;   /*!< The RTCP message authentication code key size, in bytes. Default = 20.*/
	U8								RtcpCipherKeySize;                      /*!< The RTCP Encryption Key size in bytes (currently, the 16-byte key size is supported). Default = 16.*/
	Tac49xMediaAuthenticationMode	RtcpAuthenticationMode;                 /*!< The RTCP authentication mode. Default = SHA1.*/
	Tac49xMediaEncryptionMode		RtcpEncryptionMode;                     /*!< The RTCP Encryption mode. Default = AES_CM.*/
	U8								RtcpDigestSize;                         /*!< The RTCP digest size in bytes. The digest is the MAC result, which is added to each packet. Default = 10.*/

	U8								MasterKeySize;                          /*!< The Master Key Size, in bytes. Default = 16.*/
	U8								MasterSaltSize;                         /*!< The Master Salt Size, in bytes. Default = 14.*/
	U8								SessionSaltSize;                        /*!< The Session Salt Size, in bytes. Default = 14.*/

	U16								RtpInitSequenceNumber;                  /*!< This parameter configures the RTP sequence number. It is relevant only in the inbound direction (the received RTP sequence number). The transmitted RTP sequence number is configured using the function ::Ac49xActivateRegularRtpChannelConfiguration. If set to 0, the channel is updated by the received RTP packets. In High Availability (HA) (if the connection begins in the session), it is recommended to set it to its actual value rather than 0. Default = 0.*/
	U32								RtpInitRoc;                             /*!< In the outbound  direction, this parameter configures the  transmitted ROC. In the inbound  direction, this parameter  configures the received ROC. It should be set to 0 unless the connection begins in the middle of a session ( i.e., HA). Default = 0.*/
	U32								RtcpInitIndex;                          /*!< In the outbound  direction, this parameter configures the  transmitted RTCP index. In the inbound  direction, this parameter  configures the received RTCP index. It should be set to 0 unless the connection begins in the middle of a session ( i.e., HA). Default = 0.*/
	Tac49xControl					SsrcValid;                              /*!< This parmeter configures whether the field @b Ssrc is valid. <ul> <li> 0 =  SSRC is not valid. <li> 1 = SSRC is valid. </ul> @n Default = 0. */
	U32								Ssrc;                                   /*!< The remote RTP SSRC. This parameter is valid only if the parameter @b SsrcValid = 1. In addition, it is valid only in receive direction (@b Direction = 0 ). */
	Tac49xControl					ResetCountersOnMediaProtectionAuditEvents;  /*!< This parameter configures the counters on ::Tac49xSrtpProtectionAuditEvent status packet. <ul> <li> 0 = Counters on media protection audit events are accumulative. <li> 1 = Counters on media protection audit events are reset at every event transmission. </ul> @n Default = 0. */
	Tac49xControl					ScaleSrtpModeEnable;					 /*!< Reserved for internal use. Must be set to 0. */
	
    U8								MasterKey[MAX_KEY_SIZE__MASTER__FORMAT_1]; /*!< The Master Key Data.*/
    U8								MasterSalt[MAX_KEY_SIZE__MASTER_SALT];  /*!< The Master Salt Data.*/

	U8								MasterKeyIndexSize;                        /*!< Master Key Index (MKI) field size. @li 0 = MKI disabled (default) @li Other = MKI size 1 - 4 (in bytes) */
	U8								MasterKeyIndex[MAX_MASTER_KEY_INDEX_SIZE]; /*!< 32-bit Master Key Index. When the MKI size is less than 4, MKI is taken from the MS bytes (starting from Byte 0).*/
	U8								ScaleSrtpExtendedSequenceNumber[SCALE_SRTP_EXTENDED_SEQUENCE_NUMBER_SIZE];	/*!< Reserved for internal use. Must be set to 0. */
    } Tac49xSrtpProtectionConfigurationFormat1_Attr;
    /** @} */

    /**
     * @defgroup Tac49xCodecConfigurationAttr Tac49xCodecConfigurationAttr
     * @ingroup CHANNEL_CFG_STRUCTURES
     * @brief
     * Defines the on-chip codec configuration parameters. It is available only in the AC494 SOC.
     * @{
     */

    /**
     * <BR>
     * This structure defines the on-chip codec configuration parameters. It is available only in the AC494 SOC.
     */

#if (  (AC49X_DEVICE_TYPE == AC494_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC495_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC495L_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC496_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC496E_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC497_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC494E_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC495E_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC496D_DEVICE)\
    )
typedef struct
    {
    Tac49xControl	                AdcDitheringCircuit                     ;  /*!< Enables the Analog-to-Digital Converter (ADC) dithering circuit. Default = CONTROL__ENABLE.*/
    Tac49xBiasPinOutputVoltage	    BiasPinOutputVoltage                    ;  /*!< Configures the bias pin output voltage. Default = BIAS_PIN_OUTPUT_VOLTAGE__2v35 (2.35 V).*/
    Tac49xSpeakerGain	            SpeakerGain                             ;  /*!< Configures the Speaker Gain. Default = SPEAKER_GAIN__0dB.*/
    Tac49xControl	                BandPassFilter                          ;  /*!< Enables the 8 kHz band pass filter [300 Hz -3.3 kHz] with the sampling rate at 8 kHz. Default = CONTROL__DISABLE.*/
    Tac49xSamplingRate              SamplingRate                            ;  /*!< Sampling Rate configuration. The Sampling Rate configuration must be the same for both Codec channels. A 16 KHz sampling rate is supported only if the TDM serial port is disabled. @li 0 = 8 KHz (default) @li 1 = 16 KHz */
    Tac49xControl	                MuteHandset		                        ;  /*!< Enables the mute handset output. Default = CONTROL__DISABLE.*/
    Tac49xControl	                MuteHeadset		                        ;  /*!< Enables the mute headset output. Default = CONTROL__DISABLE.*/
    Tac49xControl	                MuteLineOutput		                    ;  /*!< Enables the mute line output. Default = CONTROL__DISABLE.*/
    Tac49xControl	                MuteSpeaker		                        ;  /*!< Enables the mute  8-ohm speaker output. Default = CONTROL__DISABLE.*/
    Tac49xControl	                HeadsetInputToOutput	                ;  /*!< @li CONTROL__DISABLE = The Headset input is not connected to the headset output (default). @li CONTROL__ENABLE = The Headset input is connected to the headset output.*/
    Tac49xControl	                HandsetInputToOutput	                ;  /*!< @li CONTROL__DISABLE = The handset input is not connected to the headset output (default). @li CONTROL__ENABLE = The handset input is connected to the headset output.*/
    Tac49xControl	                CallerIdInputSelect	                    ;  /*!< Enables the connection of the Caller ID input to the ADC channel. Default = CONTROL__DISABLE.*/
    Tac49xControl	                LineInputSelect		                    ;  /*!< Enables the connection of the line driver input to the ADC channel. Default = CONTROL__DISABLE.*/
    Tac49xControl	                MicrophoneInputSelect                   ;  /*!< Enables the connection of the microphone input to the ADC channel.  Default = CONTROL__DISABLE.*/
    Tac49xControl	                HandsetInputSelect	                    ;  /*!< Enables the connection of the handset input to the ADC channel.  Default = CONTROL__DISABLE.*/
    Tac49xControl	                HeadsetInputSelect	                    ;  /*!< Enables the connection of the Headset input to the ADC channel.  Default = CONTROL__DISABLE.*/
    Tac49xSidetoneGain	            AnalogSidetoneGain	                    ;  /*!< Configures the analog sidetone gain. Default = SIDETONE_GAIN__minusd12dB.*/
    Tac49xSidetoneGain	            DigitalSidetoneGain	                    ;  /*!< Configures the digital sidetone gain. Default = SIDETONE_GAIN__MUTE.*/
    Tac49xControl	                AnalogSidetoneOutputSelectForHeadset	;  /*!< Enables the connection of the analog sidetone to headset output. Default = CONTROL__DISABLE.*/
    Tac49xControl	                AnalogSidetoneOutputSelectForHandset	;  /*!< Enables the connection of the analog sidetone to handset output. Default = CONTROL__DISABLE.*/
    Tac49xControl	                SpeakerSelect		                    ;  /*!< Enables the connection of the DAC output to the 8-ohm speaker driver. Default = CONTROL__DISABLE.*/
    Tac49xControl	                LineOutputSelect		                ;  /*!< Enables the connection of the DAC output to the 600-ohm line driver. Default = CONTROL__DISABLE.*/
    Tac49xControl	                HeadsetOutputSelect		                ;  /*!< Enables the connection of the DAC output to the 150-ohm handset driver. Default = CONTROL__DISABLE.*/
    Tac49xControl	                HandsetOutputSelect		                ;  /*!< Enables the connection of the DAC output to the 150-ohm handset driver. Default = CONTROL__DISABLE.*/
    Tac49xCodecDacOutputPgaGain	    DacOutputPgaGain		                ;  /*!< Refer to ::Tac49xCodecDacOutputPgaGain. Configures the DAC (Digital-to-Analog Converter) input PGA (Programmable Gain Amplifier) gain. Default = CODEC_INPUT_PGA_GAIN__minus12dB.*/
    Tac49xCodecAdcInputPgaGain      AdcInputPgaGain		                    ;  /*!< Refer to ::Tac49xCodecAdcInputPgaGain. Configures the ADC (Analog-to-Digital Converter) input PGA (Programmable Gain Amplifier) gain. Default = CODEC_INPUT_PGA_GAIN__0dB.*/
    Tac49xControl                   AnalogLoopback		                    ;  /*!< Enables loopback on the analog interface. Default = CONTROL__DISABLE.*/
    Tac49xControl                   DigitalLoopback		                    ;  /*!< Enables loopback on the digital interface. Default = CONTROL__DISABLE.*/
	Tac49xControl					DualOutputModeEnable					;  /*!< Dual output mode. In this mode the analog outputs of both codec channels are connected to the same AC494 channel. <ul> <li> 0 = Disable. <li> 1 = Enable. </ul> @n Default = 0. @n @n <TABLE> <TR> <TD> <b> Codec Channel </b> </TD> <TD> @b Input </TD> <TD> @b Output </TD> <TD> <b> Digital Output Gain </b> </TD> </TR> <TR ><TD> 0 </TD> <TD> Handset</TD> <TD> Handset </TD> <TD> ::Tac49xOpenOrUpdateChannelConfigurationAttr.VoiceOutputGain </TD> </TR> <TR> <TD> 1 </TD> <TD> Disconnected </TD> <TD> Speaker </TD> <TD> ::Tac49xCodecConfigurationAttr.DualOutputModeGain </TD> </TR></TABLE> @n @n @b Note: There is no limitation on the codec interface; each channel can support any codec interface. @n Refer to the <A HREF="Codec Dual Output Mode.html"> dual output mode block diagram</A>. */
	Tac49xVoiceGain					DualOutputModeGain						;  /*!< The digital output gain. Refer to ::Tac49xVoiceGain. This parameter is valid only if the field ::Tac49xCodecConfigurationAttr.DualOutputModeEnable. is set to 1. This field defines the digital gain of the speaker output. */
    Tac49xControl                   UsbSyncronizationModeEnable             ;  /*!< This parameter enables the external USB headset interface. It is available on the AC494/AC494E SoC. If enabled, the VoPP channel disables the analog CODEC interface. The voice interaface is replaced with RTP packets containing linear 16-bit 16kHz voice. */
    Tac49xControl                   UsbSyncronizationDebugEnable            ;  /*!< Reserved for internal use. Must be set to 0.*/
    } Tac49xCodecConfigurationAttr;
#endif/*((AC49X_DEVICE_TYPE == AC494_DEVICE) || (AC49X_DEVICE_TYPE == AC495_DEVICE))*/

    /** @} */



#if (  (AC49X_DEVICE_TYPE == AC494_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC495_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC495L_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC496_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC496E_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC497_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC494E_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC495E_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC496D_DEVICE)\
    )

#ifndef NDOC
typedef struct
	{
	Tac49xAcousticEchoCancelerSupportedMode					SupportedMode;             /*!< Echo Canceler supported modes. Default = SPEAKER_PHONE_ONLY.*/
	Tac49xControl											SaturationAttenuator;      /*!< Reserved for internal use, must be set to CONTROL__ENABLE.*/
	Tac49xControl											ResidualOverwriteByInput;  /*!< Reserved for internal use, must be set to CONTROL__ENABLE.*/
	Tac49xControl											SoftNlp;                   /*!< Reserved for internal use, must be set to CONTROL__ENABLE.*/
	Tac49xControl											Cng;                       /*!< Reserved for internal use, must be set to CONTROL__ENABLE.*/
	Tac49xControl											ReinitAfterToneSignal;     /*!< Reserved for internal use, must be set to CONTROL__ENABLE.*/
	Tac49xAcousticEchoCancelerHalfDuplexMode				HalfDuplexMode;            /*!< Half Duplex mode. Supported on 8 KHz sampling frequency only. For 16 kHz sampling frequency, this bit  must be set to ON_START_ONLY.*/
	Tac49xControl											AntiHowlingProcessor;      /*!< Reserved for internal use, must be set to CONTROL__ENABLE.*/
	Tac49xAcousticEchoCancelerEchoReturnLoss				EchoReturnLoss;            /*!< Reserved for internal use, must be set to 0_dB.*/
	Tac49xAcousticEchoCancelerMaxActiveFilterLength			MaxActiveFilterLength;     /*!< Reserved for internal use, must be set to 32_msec.*/
	Tac49xAcousticEchoCancelerTailLength					TailLength;                /*!< Tail Length. This is the maximal expected length of the Acoustic Echo Canceler cancelation. The Default is ACOUSTIC_ECHO_CANCELER_TAIL_LENGTH__128_msec. */
	Tac49xAcousticEchoCancelerCoefficientsThreshold			CoefficientsThreshold;     /*!< Reserved for internal use, must be set to 49_dBm.*/
	Tac49xAcousticEchoCancelerNlpActivationThreshold		NlpActivationThreshold;    /*!< Reserved for internal use, must be set to AUTOMATIC.*/
	Tac49xAcousticEchoCancelerSaturationThreshold			SaturationThreshold;       /*!< Saturation threshold 2047 + (Saturaton Threshold) * 2048 ( a signal below this threshold is detected as saturated). The default is 13.*/
    Tac49xControl                                           HighPassFilter;            /*!< High Pass Filter on Microphone Output for Wideband (16k only) Acoustic Echo Canceler. The bit must be set if a hardware HPF filter is not implemented. @li 0 = Disabled @li 1 = Enabled (default) */
    Tac49xControl	                                        Decimation;                /*!< Available on Wideband AEC. Reserved for internal use. Must be set to 1. @li 0 = Disabled @li 1 = Enabled (default) */
	Tac49xControl	                                        IntensiveMode;             /*!< Intensive Mode available on Wideband AEC (16k) only. Reserved for internal use. Must be set to 1. @li 0 = Disabled @li 1 = Enabled (default) */
	int														CommandIndex;    	   	   /*!< Reserved for internal use, must be set to 0. */
	Tac49xControl											PreAdaptationMode;         /*!< This field enables the echo canceller filter adaptation with delay 0 before the delay is estimated. @li 0 = Disabled @li 1 = Enabled (default)*/
	int														HalfDuplexAttenuation;     /*!< This field configures the attenuation, in dB, during half-duplex operation. <ul> <li> 0 = 5dB attenation during double-talk. 10dB attenation during single-talk (default) <li> 1 = 1dB attenation during double-talk. 2dB attenation during single-talk <li> 2 = 2dB attenation during double-talk. 4dB attenation during single-talk <li> 3 = 3dB attenation during double-talk. 6dB attenation during single-talk <li> 4 = 4dB attenation during double-talk. 8dB attenation during single-talk <li> 5 = 5dB attenation during double-talk. 10dB attenation during single-talk  </ul>  */
	int														HandsetNLPSensetivity;     /*!< This field configures NLP sensitivity for the handset interface. <ul> <li> 0 - NLP for the handset interface is disabled. (default)  <li> 1 - 5  - NLP for the handset interface is enabled. 1 - lowest sensitivity.  5 - highest sensitivity.  </ul> */
	} Tac49xAcousticEchoCancelerConfigurationAttr;


#define Tac49xAcousticEchoCancelerConfigurationFormat0_Attr Tac49xAcousticEchoCancelerConfigurationAttr

#endif //NDOC

     /**
     * @defgroup Tac49xAcousticEchoCancelerConfigurationFormat1_Attr Tac49xAcousticEchoCancelerConfigurationFormat1_Attr
     * @ingroup CHANNEL_CFG_STRUCTURES
     * @brief
     * Defines the acoustic echo canceler configuration parameters. It is only available on the AC494 SOC.
     * @{
     */

    /**
     * <BR>
     * This structure defines the echo canceler configuration parameters. It is only available on the AC494 SOC.
     */
typedef struct
	{
	Tac49xAcousticEchoCancellerHighPassFilterMode			HeadsetHPFilterMode;	   /*!< The Headset high pass filter mode. Refer to ::Tac49xAcousticEchoCancellerHighPassFilterMode. Default = ACOUSTIC_ECHO_CANCELER_HPF_DC_REMOVER(1).*/
	Tac49xAcousticEchoCancellerHighPassFilterMode			HandsetHPFilterMode;       /*!< The Handset high pass filter mode. Refer to ::Tac49xAcousticEchoCancellerHighPassFilterMode. Default = ACOUSTIC_ECHO_CANCELER_HPF_DC_REMOVER(1).*/
	Tac49xAcousticEchoCancellerHighPassFilterMode			HandsfreeHPFilterMode;     /*!< The microphone codec interface (hands-free mode) high pass filter mode. Refer to ::Tac49xAcousticEchoCancellerHighPassFilterMode. Default = ACOUSTIC_ECHO_CANCELER_HPF_HP_FILTER (2).*/
	int										NLPMaximumDelay;	   /*!< This parameter defines the known acoustic echo delay boundary. The NLP functions optimally if the echo delay is below this threshold. <ul> <li> 0 = The NLP operation is not limited by the echo delay (default) <li> 1-12 tThe maximal acoustic echo delay in 4ms resolution. The range is 4 to 48ms. </ul> */
	Tac49xControl							ExtendedNlp;		   /*!< This parameter configures the Extended NLP. When enabled, this field applies an excessive NLP attenuation. Should be used only for cases with high non-linearity priority in the echo path. <ul> <li> 0 = Disabled (Default) <li> 1 = Enabled . </ul>*/	
	Tac49xControl							AutomaticNlp;		   /*!< This parameter configures the Extended NLP Automatic Mode. When enabled, this field applies a automatically controlled excessive NLP attenuation. <ul> <li> 0 = Extended NLP is always enabled (Default) <li> 1 = Extended NLP is controlled automatically  . </ul>*/	
	int										EnhancedNlpMode;	   /*!< This parameter configures working point for the extended NLP <ul> <li> 0 = Extended NLP mode is confugred by the advanced channel configuration (Default) <li> 1-7 = Extended NLP is mode is overridden. </ul>*/	
	Tac49xControl							PreConvergenceAttenuation;					/*!< This parameter defines the pre convergence attenuation. <ul> <li> 0 = Attenuation is disabled(Default) <li> 1 = Attenuation is enabled. </ul>*/	
	} Tac49xAcousticEchoCancelerConfigurationFormat1_Attr;
    /** @} */

    /**
     * @defgroup Tac49xEchoCancelerEqualizerConfigurationAttr Tac49xEchoCancelerEqualizerConfigurationAttr
     * @ingroup DEVICE_CFG_STRUCTURES
     *
     * @brief
     * The input structure to the function ::Ac49xEchoCancelerEqualizerConfiguration. It is available only in the AC494 SOC.
     * @{
     */

    /**
     * <BR>
     * The @b Tac49xEchoCancelerEqualizerConfigurationAttr is the input structure to the 
     * function ::Ac49xEchoCancelerEqualizerConfiguration. It contains the acoustic echo 
     * canceller equalizer coefficients.
     * @n It is available only in the AC494 SOC.
     */
typedef struct
	{
	Tac49xEchoCancelerCodecInterface						CodecInterface;                  /*!< This field informs which codec interface is being configured. Refer to ::Tac49xEchoCancelerCodecInterface. */
	int														NarrowBandInputEqualizerLength;  /*!< The ::Tac49xEchoCancelerEqualizerConfigurationAttr.NarrowBandInputEqualizerCoefficients length <ul> <li> 0 = Equalizer disabled. <li> 1-128 = The filter length. </ul> @n Default = 0. */
	U16														NarrowBandInputEqualizerCoefficients[MAX_ACOUSTIC_ECHO_CANCELER_EQUALIZER_COEFFICIENTS];  /*!< The narrow band input equalizer coefficients. */
	int														NarrowBandOutputEqualizerLength; /*!< The ::Tac49xEchoCancelerEqualizerConfigurationAttr.NarrowBandOutputEqualizerCoefficients length <ul> <li> 0 = Equalizer disabled. <li> 1-128 = The filter length. </ul> @n Default = 0. */
	U16														NarrowBandOutputEqualizerCoefficients[MAX_ACOUSTIC_ECHO_CANCELER_EQUALIZER_COEFFICIENTS]; /*!< The narrow band output equalizer coefficients. */
	int														WideBandInputEqualizerLength;    /*!< The ::Tac49xEchoCancelerEqualizerConfigurationAttr.WideBandInputEqualizerCoefficients length <ul> <li> 0 = Equalizer disabled. <li> 1-128 = The filter length. </ul> @n Default = 0. */
	U16														WideBandInputEqualizerCoefficients[MAX_ACOUSTIC_ECHO_CANCELER_EQUALIZER_COEFFICIENTS];    /*!< The wide band input equalizer coefficients. */
	int														WideBandOutputEqualizerLength;   /*!< The ::Tac49xEchoCancelerEqualizerConfigurationAttr.WideBandOutputEqualizerCoefficients length <ul> <li> 0 = Equalizer disabled. <li> 1-128 = The filter length. </ul> @n Default = 0. */
	U16														WideBandOutputEqualizerCoefficients[MAX_ACOUSTIC_ECHO_CANCELER_EQUALIZER_COEFFICIENTS];   /*!< The wide band output equalizer coefficients.*/
	} Tac49xEchoCancelerEqualizerConfigurationAttr;
    /** @} */

    /**
     * @defgroup Tac49xDeviceLowPassFilterConfigurationAttr Tac49xDeviceLowPassFilterConfigurationAttr
     * @ingroup DEVICE_CFG_STRUCTURES
     *
     * @brief
     * The input structure to the function ::Ac49xDeviceLowPassFilterConfiguration. It is available only in the AC494 SOC.
     * @{
     */

    /**
     * <BR>
     * The @b Tac49xDeviceLowPassFilterConfigurationAttr is the input structure to the 
     * function ::Ac49xDeviceLowPassFilterConfiguration. It contains the low pass filter coefficients.
     * @n It is available only in the AC494 SOC.
     */
typedef struct
	{
	int														InputFilterLength;  /*!< The ::Tac49xDeviceLowPassFilterConfigurationAttr.InputFilterCoefficients length <ul> <li> 0 = Filter disabled. <li> 1-256 = The filter length. </ul> @n Default = 0. */
	int														InputFilterCoefficients[MAX_DEVICE_LOW_PASS_FILTER_COEFFICIENTS];  /*!< The input filter coefficients. */
	int														OutputFilterLength; /*!< The ::Tac49xDeviceLowPassFilterConfigurationAttr.OutputFilterCoefficients length <ul> <li> 0 = Filter disabled. <li> 1-256 = The filter length. </ul> @n Default = 0. */
	int														OutputFilterCoefficients[MAX_DEVICE_LOW_PASS_FILTER_COEFFICIENTS]; /*!< The output filter coefficients. */
	} Tac49xDeviceLowPassFilterConfigurationAttr;
    /** @} */
#endif/*((AC49X_DEVICE_TYPE == AC494_DEVICE) || (AC49X_DEVICE_TYPE == AC495_DEVICE))*/


    /**
     * @defgroup Tac49xRfc3558InitializationConfigurationAttr Tac49xRfc3558InitializationConfigurationAttr
     * @ingroup CHANNEL_CFG_STRUCTURES
     * @brief
     * Defines the RFC 3558 configuration parameters.
     * @{
     */

    /**
     * <BR>
     * This structure defines the EVRC transfer protocol parameters.
     */

typedef struct
    {
    Tac49xRfc3558PacketFormat       PacketFormat;                               /*!< The format of the packet. @li 0 = Header Free Packet Format @li 1 = Bundled Packet Format (default) @li 2 = Compact-Bundle Format. In this mode, Local Max Rate and Local Min Rate must be equal.*/
    U8                              LocalRateMode;                              /*!< The rate mode of the local encoder according to RFC 3558. @li 0 = All rate 1 frames remain in rate 1 (default). @li 1 = 3/4 of the rate 1 frames remain as rate 1 and 1/4 are changed to rate 1/2. @li 2 = 1/2 of the rate 1 frames remain as rate 1 and 1/2 are changed to rate 1/2. @li 3 = 1/4 of the rate 1 frames remain as rate 1 and 3/4 are changed to rate 1/2. @li 4 = All rate 1 frames are changed to rate 1/2.*/
    U8                              RemoteRateMode;                             /*!< The rate mode of the remote encoder according to RFC 3558. @li 0 = All rate 1 frames remain in rate 1 (default). @li 1 = 3/4 of the rate 1 frames remain as rate 1 and 1/4 are changed to rate 1/2. @li 2 = 1/2 of the rate 1 frames remain as rate 1 and 1/2 are changed to rate 1/2. @li 3 = 1/4 of the rate 1 frames remain as rate 1 and 3/4 are changed to rate 1/2. @li 4 = All rate 1 frames are changed to rate 1/2.*/
    U8                              LocalMaxRate;                               /*!< The maximum allowed local rate. @li 0 = Reserved. @li 1 = Rate 1/8. @li 2 = Reserved. @li 3 = Rate 1/2. @li 4 = Rate 1 (default).*/
    U8                              LocalMinRate;                               /*!< The minimum allowed local rate. @li 0 = Reserved. @li 1 = Rate 1/8 (default). @li 2 = Reserved. @li 3 = Rate 1/2. @li 4 = Rate 1.*/
    Tac49xControl                   LocalRateModeControlledByRemoteSideEnable;  /*!< Enables Local Rate Mode Conrol from the remote side (by the MMM parameter on the received packets). Default = CONTROL__ENABLE*/
    Tac49xControl                   SilenceCompressionExtensionModeEnable;      /*!< Reserved for internal use. Must be set to CONTROL__DISABLE.*/
    U16                             DtxMin;                                     /*!< Reserved for internal use. Must be set to 0.*/
    U16                             DtxMax;                                     /*!< Reserved for internal use. Must be set to 0.*/
	Tac49xControl					NoiseSuppressionFilterEnable;               /*!< Reserved for internal use. Must be set to 0.*/
	U8								Hangover;                                   /*!< Reserved for internal use. Must be set to 0.*/
    } Tac49xRfc3558InitializationConfigurationAttr;                             

    /** @} */

    /**
     * @defgroup Tac49xG729EvInitializationConfigurationAttr Tac49xG729EvInitializationConfigurationAttr
     * @ingroup CHANNEL_CFG_STRUCTURES
     * @brief
     * Defines the G.729EV Initialization command parameters.
     * @{
     */

    /**
     * <BR>
     * This structure defines the G.729EV Initialization command parameters.
     */
typedef struct
    {
	Tac49xG729EvRate                            LocalBitRate;                   /*!< Local Bit Rate. @li 0 = 8 kbps (default) @li 1 = 12 kbps @li 2 = 14 kbps @li 3 = 16 kbps @li 4 = 18 kbps @li 5 = 20 kbps @li 6 = 22 kbps @li 7 = 24 kbps @li 8 = 26 kbps @li 9 = 28 kbps @li 10 = 30 kbps @li 11 = 32 kbps @li 12-14 = Reserved @li 15 = NO_DATA @li 16-31 = Reserved */
	Tac49xG729EvAutomaticLocalRateControlMode   AutomaticLocalRateControlMode;  /*!< Automatic Rate Control Mode. @li 0 = The local rate is automatically changed according to the MBS of the remote side (default). @li 1 = Local rate is changed automatically only if it is higher than the MBS. */
	Tac49xG729EvRate                            MaxRemoteBitRate;               /*!< Maximum Bit Rate of the remote side. @li 0 = 8 kbps (default) @li 1 = 12 kbps @li 2 = 14 kbps @li 3 = 16 kbps @li 4 = 18 kbps @li 5 = 20 kbps @li 6 = 22 kbps @li 7 = 24 kbps @li 8 = 26 kbps @li 9 = 28 kbps @li 10 = 30 kbps @li 11 = 32 kbps @li 12-14 = Reserved @li 15 = NO_MBS @li 16-31 = Reserved */
    Tac49xG729EvRate                            MaxSessionBitRate;              /*!< The Maximum Rate allowed for the session (it is symmetric - remote and local). ::Tac49xG729EvInitializationConfigurationAttr.MaxRemoteBitRate and ::Tac49xG729EvInitializationConfigurationAttr.LocalBitRate must be equal to or less than this value (default = G729EV_RATE_32_kbps).*/
    } Tac49xG729EvInitializationConfigurationAttr;
    /** @} */

    /**
     * @defgroup Tac49xRtcpXrConfigurationAttr Tac49xRtcpXrConfigurationAttr
     * @ingroup CHANNEL_CFG_STRUCTURES
     * @brief
     * Defines the function ::Ac49xRegularChannelRtcpXrConfiguration parameters.
     * @{
     */

    /**
     * <BR>
     * This structure defines the function ::Ac49xRegularChannelRtcpXrConfiguration parameters.
     */
typedef struct
    {
	Tac49xRtcpXrAlertMode	BurstAlertSetting;                                  /*!< Burst Alert modes. @li 0 = No change in Alert mode (default) @li 1 = Enable Alert. @li 2 = Disable Alert. */
	Tac49xRtcpXrAlertMode	DelayAlertSetting;                                  /*!< Delay Alert modes. @li 0 = No change in Alert mode (default) @li 1 = Enable Alert. @li 2 = Disable Alert. */
	Tac49xRtcpXrAlertMode	LowVoiceQualityAlertSetting;                        /*!< Low Voice Quality Alert modes. @li 0 = No change in Alert mode (default) @li 1 = Enable Alert. @li 2 = Disable Alert. */
	int						MaximumBurstLossRateThreshold;                      /*!< Maximum Burst Loss Rate Threshold. When the loss density is above this threshold and the Burst Duration is more than 350 ms, the Burst Alert is sent. The range is 0 - 255. Default = 205. */
	int						MaximumBurstDurationThreshold;                      /*!< Maximum Burst Duration Threshold. When the burst  duration is above this threshold and when the Loss Rate is above 180, the Burst Alert is sent. Default = 200 ms. */
	int						MaximumOneWayDelayThreshold;                        /*!< Maximum One Way Delay Threshold. Maximal round trip delay divided by two. Above this threshold, the Delay Alert is sent. Default = 400 ms. */
	int						LowVoiceQualityRValueThreshold;                     /*!< Low Voice Quality. Lowest MOS Listening Quality value before sending the Low VQ Alert RVAL Threshold. Default = 65. */
	int						MinimumGapLength;                                   /*!< Minimum gap length to determine between gap or burst of packets. */
	Tac49xRtcpXrSnrMode		SnrMode;                                            /*!< This field configures the <i> Signal Level </i> and <i> Noise Level </i> fields report mode. Refer to ::Tac49xRtcpXrSnrMode. @n Default = 0. */
	Tac49xRerlMode			RerlMode;                                           /*!< This field configures the <i> RERL </i> field report mode. Refer to ::Tac49xRerlMode. @n Default = 0. */
	Tac49xMosType			MosType;                                            /*!< Reserved for internal use. Must be set to REPORT_MOS_FROM_BEGINNING_OF_SESSION (0).*/
	} Tac49xRtcpXrConfigurationAttr;
    /** @} */

    /**
     * @defgroup Tac49xUemClipInitializationAttr Tac49xUemClipInitializationAttr
     * @ingroup CHANNEL_CFG_STRUCTURES
     * @brief
     * Defines the function ::Ac49xUemClipInitializationConfiguration parameters.
     * @{
     */

    /**
     * <BR>
     * This structure defines the function ::Ac49xUemClipInitializationConfiguration parameters.
     */
typedef struct
    {
	Tac49xRtpTimeStampResolution	RtpTimeStampResolution;                     /*!< RTP Timestamp resolution (applies to both directions Rx and Tx). @li 0 = 8 KHz @li 1 = 16 KHz (default) */
	Tac49xUemClipTransmitMode		TransmitMode;                               /*!< UEMCLIP Transmit Mode. On the receive direction, the VoPP channel can receive any of the UEMClip modes. Refer to ::Tac49xUemClipTransmitMode. The default is UEM_CLIP_TRANSMIT_MODE__MODE_4*/
	} Tac49xUemClipInitializationAttr;
    /** @} */


    /**
     * @defgroup Tac49xG7111InitializationAttr Tac49xG7111InitializationAttr
     * @ingroup CHANNEL_CFG_STRUCTURES
     * @brief
     * Defines the function ::Ac49xG7111InitializationConfiguration parameters.
     * @{
     */

    /**
     * <BR>
     * This structure defines the function ::Ac49xG7111InitializationConfiguration parameters.
     */
typedef  struct                     
    {
	Tac49xRtpTimeStampResolution	RtpTimestampResolution;                    /*!< RTP Timestamp resolution (applies to both directions Rx and Tx). @li 0 = 8 KHz @li 1 = 16 KHz (default) */                                                                                   
	Tac49xG7111TransmitMode			TransmitMode;                              /*!< G.711.1 Transmit Mode. Refer to ::Tac49xG7111TransmitMode. The default is G711_1_TRANSMIT_MODE__R3. */
	} Tac49xG7111InitializationAttr;
    /** @} */


    /**
     * @defgroup Tac49xOpusInitializationConfigurationAttr Tac49xOpusInitializationConfigurationAttr
     * @ingroup CHANNEL_CFG_STRUCTURES
     * @brief
     * Defines the ::Ac49xOpusInitializationConfiguration function's parameters.
     * @{
     */

    /**
     * <BR>
     * This structure defines the ::Ac49xOpusInitializationConfiguration function's parameters.
     */
typedef struct
	{
	Tac49xControl                           ForwardErrorCorrection; /*!< This field enables forward error correction @li 0 = Disabled  @li 1 = Enabled (default) */
	Tac49xControl                           DecoderStatusEnable;    /*!< This field enables the decoder status packet. Refer to ::Tac49xOpusDecoderStatusEvent. @li 0 = Disabled (default)  @li 1 = Enabled */
	Tac49xConstantRateSupport               ConstantBitRateSupport; /*!< Reserved for internal use. Must be set to VARIABLE_BIT_RATE (0). */
	Tac49xMaxSamplingFrequency              MaxSamplingFrequency;   /*!< This field configures the encoder max internal sampling frequency.@n Refer to ::Tac49xMaxSamplingFrequency. @n default = 16KHZ. */
	int                                     PacketDuration;         /*!< This field configures the encoder packet duration: <ul> <li> 0 = 2.5ms (not supported) <li> 1 = 5ms (not supported) <li> 2 = 10ms <li> 3 = 20ms (default) <li> 4 = 40ms <li> 5 = 60ms <li> 6 = 80ms (2 * 40ms)  <li> 7 = 120ms (2 * 60ms) </ul> */
	U32                                     MaxAvgBitRate;          /*!< This field configures the maximum average encoder bitrate. The range is 6000 bps to 510,000 bps. @n default = 19,000 bps */
	int                                     MinEncPacketLossRatio;  /*!< This field configures the minimal packet loss percentage reported to the Opus encoder. The Opus encoder enables FEC only if the remote channel reports packet loss via RTCP. This field enables FEC regardless of the remote channel packet loss. @n default = 0. */
	} Tac49xOpusInitializationConfigurationAttr;
    /** @} */


#ifndef NDOC
 
typedef struct
	{
	Tac49xControl                           ForwardErrorCorrection; /*!< This field enables forward error correction @li 0 = Disabled  @li 1 = Enabled (default) */
	Tac49xMaxSamplingFrequency              MaxSamplingFrequency;   /*!< This field configures the encoder max internal sampling frequency.@n Refer to ::Tac49xMaxSamplingFrequency. @n default = 16KHZ. */
	Tac49xTimestampResolution               TimestampResolution;    /*!< This field configures the Timestamp resolution in the RTP packet.@n Refer to ::Tac49xTimestampResolution. @n default = 16KHZ. */
	int                                     PacketDuration;         /*!< This field configures the encoder packet duration: <ul> <li> 0 = 2.5ms (not supported) <li> 1 = 5ms (not supported) <li> 2 = 10ms <li> 3 = 20ms (default) <li> 4 = 40ms <li> 5 = 60ms <li> 6 = 80ms (2 * 40ms)  <li> 7 = 120ms (2 * 60ms) </ul> */
	int										MinEncPacketLossRatio;	/*!< This field configures the minimum packet loss ratio reported to encoder @n default = 0. */
	U32                                     MaxAvgBitRate;          /*!< This field configures the maximum average encoder bitrate. The range is 6000 bps to 510,000 bps. @n default = 19,000 bps */
	} Tac49xSilkInitializationConfigurationAttr;
 
typedef struct
	{
	Tac49xControl                           RtcpFeedbackEnable;				/*!< This field enables RTCP feedback transmossion @li 0 = Disabled  @li 1 = Enabled (default) */
	Tac49xControl                           EncoderBitrateControlEnable;    /*!< This field enables Encoder Bitrate control @li 0 = Disabled  @li 1 = Enabled (default)*/
	Tac49xControl                           SilenceCompressionControlEnable;/*!< This field enables Silence compression control. @li 0 = Disabled   @li 1 = Enabled (default)*/
	int										MaxFecLevel;					/*!< This field defines Maximum FEC level @n default = 1 */
	int										MinPacketFramesize;				/*!< This field defines Minimum packetization time in ms @n default = 20ms*/
	int										MaxPacketFramesize;				/*!< This field defines Maximum packetization time in ms @n default = 120ms*/
	U32										InitialBitrate;					/*!< This field defines Initial coder bit rate in bps @n default = 38,000 bps*/	
	U32										MinBitrate;						/*!< This field defines Minimum coder bit rate in bps @n default = 6,000 bps*/	
	U32										MaxBitrate;						/*!< This field defines Maximum coder bit rate in bps @n default = 50,000 bps*/	
	U32										InitialRemoteBandwidth;			/*!< This field defines Initial bandwidth requested by the RTCP feedback in bps @n default = 54,000 bps*/	
	U32										MinRemoteBandwidth;				/*!< This field defines Minimum bandwidth requested by the RTCP feedback in bps @n default = 8,000 bps*/	
	U32										MaxRemoteBandwidth;				/*!< This field defines Maximum bandwidth requested by the RTCP feedback in bps @n default = 66,000 bps*/
	int										OveruseFactor;					/*!< This field defines Overuse decetion calibration Factor @n default = 0 = Medium sinsitivity*/
	int										HeaderSize;						/*!< This field defines Header size in bytes @n default = 28 = IP + UDP Header*/
	int										MaxTMMBRTransmissionRate;		/*!< This field defines Max TMMBR transmission rate @n default = 50*/
	int										RTCPFeedbackMode;				/*!< This field defines RTCP Feedback Mode @n default = 0*/
	} Tac49xVoiceEnhancementConfigurationAttr;


typedef struct                     
    {
	Tac49xVariableRateSupport			VariableRateSupport;                  /*!< Variable Rate support. @li 0 = Fixed bit rate  @li 1 = Variable bit rate (default) */
	Tac49xJitterBufferMode				JitterBufferMode;                     /*!< Jitter Buffer Mode. Refer to ::Tac49xJitterBufferMode. The default is JITTER_BUFFER_MODE__NORMAL. */
	Tac49xForwardErrorCorrectionMode	ForwardErrorCorrectionMode;           /*!< Forward Error Correction Mode. @li 0 = Disabled  @li 1 = Enabled (internal) - Default */
	int									Bandwidth;                            /*!< The network bandwidth. This parameter limits the RTA encoder bitrate. <ul> <li> @b 0 = @b Automatic - The bandwidth is set according to the RTCP packet received from the remote side. <li> @b Other = @b RTA @b Bandwidth <ul> <li> For NB RTA the rage is 8800 - 29000. <li> for WB RTA the range is 18000 - 29000. </ul> </ul> @n Default = 29000. @n Note: Setting a value outside the range will be treated as if the minimal bandwidth was set. */
	} Tac49xMicrosoftRtaInitializationAttr;                                    

typedef struct
	{
	int			EchoDecisionFactor;			
	int			MaximalEchoLevel_minus_dbm;				
	int			MaximalErlLevel_minus_dbm;				
	int			MaxNoiseLevel_minus_dbm;					
	int			EchoGapTime;						
	int			SpeechGapTime;					
	int			SilenceGapTime;
	int			InitialRefDelay;				
	int			MinRefDelay;					
	int			MaxRefDelay;	
	int			MinGain;
	Tac49xControl		Cng;
	} Tac49xNetworkAcousticEchoSuppressorConfigurationAttr;


typedef struct
	{
	Tac49xControl					Cng;
	Tac49xVariableRateSupport		VariableBitRateSupport;
	int								EncoderMode;
	} Tac49xSpeexInitializationConfigurationAttr;

typedef struct
	{
	int							ConferenceID;
	} Tac49xActivateOrDeactivate9WayConferenceConfigurationAttr;

typedef struct
	{
	int							UserId;
	int							LegType;
	int							ConferenceID;
	int							Subopcode;
	Tac49xControl				HearingMatrixEnable;
	Tac49xControl				UserHearsUser0;
	Tac49xControl				UserHearsUser1;
	Tac49xControl				UserHearsUser2;
	Tac49xControl				UserHearsUser3;
	Tac49xControl				UserHearsUser4;
	Tac49xControl				UserHearsUser5;
	Tac49xControl				UserHearsUser6;
	Tac49xControl				UserHearsUser7;
	Tac49xControl				UserHearsUser8;
	} Tac49x9WayConferenceResourceConfigurationAttr;



#endif /* NDOC */


            /**********************************************************************/
            /*   R U N   S T A T E   Read and Write Packet Attribute Structures   */
            /**********************************************************************/

    /**
     * This structure contains the parameters of a single signal to be played.
     */

typedef struct
    {
    Tac49xControl		Pause;       /*!< @li CONTROL__DISABLE = Disable pause/silence. @li CONTROL__ENABLE = Generate pause/silence.*/
    int					Digit;       /*!< Digit to be played. Refer to <A HREF="DIGIT Field According to Signaling Type.html">Digit Field According to Signaling Type table</A> and to <A HREF="Advanced Signals.html"> Advanced Signals Digits</A>.*/
    Tac49xIbsSystemCode SystemCode;  /*!< The digit signaling system (DTMF, MF, ...). @n Note: 0 (None) = Reserved.*/
    } Tac49xIbsStringSignalAttr;

    /**
     * @defgroup Tac49xIbsStringAttr Tac49xIbsStringAttr
     * @ingroup CHANNEL_COMMAND_STR
     * @brief
     * This structure is used as an input to the ::Ac49xSendIbsString function.
     * @{
     */

    /**
     * <BR>
     * This structure is used as an input to the ::Ac49xSendIbsString function. It contains digits and signals
     * to be played.
     */

typedef struct
    {
	Tac49xIbsStringGeneratorRedirection GeneratorRedirection;	         /*!< Tone Generation Redirection.*/
	int									NumberOfDigits;                  /*!< The number of digits in the IBS string. Range: 1 - 20 digits.*/
	long								OnDuration;		                 /*!< On duration (identical to all digits) in msec.*/
	long								OffDuration;		             /*!< Off duration (identical to all digits) in msec.*/
	long								PauseDuration;		             /*!< Pause duration in msec. @n Applies if the field 'Pause' in structure Tac49xIbsStringSignalAttr is ENABLED.*/
    Tac49xIbsStringSignalAttr			Signal[MAX_IBS_STRING_SIGNALS];  /*!< Signal definition.*/
    } Tac49xIbsStringAttr;

    /** @} */

    /**
     * This structure contains the parameters of a single signal to be played.
     */

typedef struct
	{
	int					SignalLevel_minus_dbm;  /*!< Signal Level, in -dBm resolution. For User-Defined Tones signals or Call Progress Tones signals, if the value 0 is defined in this field (SignalLevel) the levels defined in the ::Ac49xUserDefinedTonesDeviceConfiguration or ::Ac49xCallProgressDeviceConfiguration functions will be generated.*/
    int					Digit;                  /*!< Digit to be played. Refer to <A HREF="DIGIT Field According to Signaling Type.html">Digit Field According to Signaling Type table</A> and to <A HREF="Advanced Signals.html"> Advanced Signals Digits</A>.*/
	U32					SignalDuration;         /*!< The signal duration, in 1 msec resolution.*/
    Tac49xIbsSystemCode SystemCode;             /*!< The IBS system. @n ON_THE_FLY_TONES = Generates a signal combined of UserDefinedToneA, UserDefinedToneB and UserDefinedToneC. @n Refer to <A HREF="IBS_SYSTEM_CODE__ON_THE_FLY_TONES - On-the-Fly Digits.html">on-the-fly digits</A>.*/
	} Tac49xExtendedIbsStringSignalAttr, Tac49xExtendedIbsStringSignal_Format0_Attr;

    /**
     * @defgroup Tac49xExtendedIbsString_Format0_Attr Tac49xExtendedIbsString_Format0_Attr
     * @ingroup CHANNEL_COMMAND_STR
     * @brief
     * This structure is used as an input to the ::Ac49xSendExtendedIbsString_Format0 function.
     * @{
     */

    /**
     * <BR>
     * This structure is used as an input to the ::Ac49xSendExtendedIbsString_Format0 function. It contains information
     * on the signals to be played.
     */

typedef struct                 
    {                                   
    Tac49xIbsStringGeneratorRedirection GeneratorRedirection;                     /*!< Tone Generation Redirection.*/
    Tac49xControl						Repeat;                                   /*!< Repeat the string. @li CONTROL__DISABLE = String is generated only once. @li CONTROL__ENABLE = String is generated repeatedly.*/
    int									NumberOfDigits;                           /*!< Number of digits in the string. Range: 1-6.*/
    U32									TotalDuration;                            /*!< If the field Repeat is enabled, this field defines the total duration of the repeated string. If Repeat is disabled, the field is ignored. Resolution is 1 msec.*/
    U16									UserDefinedToneA;                         /*!< This field enables users to generate any signal combined of four frequencies (UserDefinedToneA, UserDefinedToneB, UserDefinedToneC and UserDefinedToneD) on the fly. The field is valid only if users set parameter SystemCode to option ON_THE_FLY_TONES. The range is 0 - 3990 Hz, in units of Hertz.*/
    U16									UserDefinedToneB;                         /*!< This field enables users to generate any signal combined of four frequencies (UserDefinedToneA, UserDefinedToneB, UserDefinedToneC and UserDefinedToneD) on the fly. The field is valid only if users set the parameter SystemCode to ON_THE_FLY_TONES. The range is 0 - 3990 Hz, in units of Hertz. When generating a single frequency signal, set this field to 0.*/
    U16									UserDefinedToneC;                         /*!< This field enables users to generate any signal combined of four frequencies (UserDefinedToneA, UserDefinedToneB, UserDefinedToneC and UserDefinedToneD) on the fly. The field is valid only if users set the parameter SystemCode to ON_THE_FLY_TONES. The range is 0 - 3990 Hz, in units of Hertz. When generating a single frequency signal, set this field to 0.*/
	U16									UserDefinedToneD;                         /*!< This field enables users to generate any signal combined of four frequencies (UserDefinedToneA, UserDefinedToneB, UserDefinedToneC and UserDefinedToneD) on the fly. The field is valid only if users set the parameter SystemCode to ON_THE_FLY_TONES. The range is 0 - 3990 Hz, in units of Hertz. When generating a single frequency signal, set this field to 0.*/
	U8									AmFactor;                                 /*!< Amplitude Modulation Factor k of the on-the-fly tones.*/
    Tac49xExtendedIbsStringSignalAttr	Signal[MAX_EXTENDED_IBS_STRING_SIGNALS];  /*!< Signal to be played.*/
    } Tac49xExtendedIbsString_Format0_Attr, Tac49xExtendedIbsStringAttr;          

    /** @} */


    /**
     * This structure contains the parameters of a single signal to be played.
     */
typedef struct 
    {
    Tac49xIbsSystemCode             SystemCode;                     /*!< The IBS system. @n ON_THE_FLY_TONES = Generates a signal combined of UserDefinedToneA, UserDefinedToneB and UserDefinedToneC. @n Refer to <A HREF="IBS_SYSTEM_CODE__ON_THE_FLY_TONES - On-the-Fly Digits.html">on-the-fly digits</A>.*/
    int                             Digit;                          /*!< Digit to be played. Refer to <A HREF="DIGIT Field According to Signaling Type.html">Digit Field According to Signaling Type table</A> and to <A HREF="Advanced Signals.html"> Advanced Signals Digits</A>.*/
	} Tac49xExtendedIbsStringSignal_Format1_SignalAttr;

    /**
     * @defgroup Tac49xExtendedIbsString_Format1_Attr Tac49xExtendedIbsString_Format1_Attr
     * @ingroup CHANNEL_COMMAND_STR
     * @brief
     * This structure is used as an input to the ::Ac49xSendExtendedIbsString_Format1 function.
     * @{
     */

    /**
     * <BR>
     * This structure is used as an input to the ::Ac49xSendExtendedIbsString_Format1 function. It contains information
     * on the signals to be played.
     */
typedef struct 
    {
//  Tac49xExtendedIbsStringFormat           Format;
	Tac49xControl                           Repeat;                 /*!< Repeat the string. @li CONTROL__DISABLE = String is generated only once. @li CONTROL__ENABLE = String is generated repeatedly.*/
	Tac49xIbsStringGeneratorRedirection     GeneratorRedirection;   /*!< Tone Generation Redirection.*/	
	int                                     NumberOfDigits;	        /*!< Number of digits in the string. Range: 1-40.*/
    U32								        TotalDuration;          /*!< If the field Repeat is enabled, this field defines the total duration of the repeated string. If Repeat is disabled, the field is ignored. Resolution is 1 msec.*/
	U16								        UserDefinedToneA;       /*!< This field enables users to generate any signal combined of four frequencies (UserDefinedToneA, UserDefinedToneB, UserDefinedToneC and UserDefinedToneD) on the fly. The field is valid only if users set the parameter SystemCode to ON_THE_FLY_TONES. The range is 0 - 3990 Hz, in units of Hertz.*/                                                             
	U16								        UserDefinedToneB;       /*!< This field enables users to generate any signal combined of four frequencies (UserDefinedToneA, UserDefinedToneB, UserDefinedToneC and UserDefinedToneD) on the fly. The field is valid only if users set the parameter SystemCode to ON_THE_FLY_TONES. The range is 0 - 3990 Hz, in units of Hertz. When generating a single frequency signal, set this field to 0.*/
	U16								        UserDefinedToneC;       /*!< This field enables users to generate any signal combined of four frequencies (UserDefinedToneA, UserDefinedToneB, UserDefinedToneC and UserDefinedToneD) on the fly. The field is valid only if users set the parameter SystemCode to ON_THE_FLY_TONES. The range is 0 - 3990 Hz, in units of Hertz. When generating a single frequency signal, set this field to 0.*/
	U16								        UserDefinedToneD;       /*!< This field enables users to generate any signal combined of four frequencies (UserDefinedToneA, UserDefinedToneB, UserDefinedToneC and UserDefinedToneD) on the fly. The field is valid only if users set the parameter SystemCode to ON_THE_FLY_TONES. The range is 0 - 3990 Hz, in units of Hertz. When generating a single frequency signal, set this field to 0.*/
	U8						                AmFactor;               /*!< Amplitude Modulation Factor k of the on-the-fly tones.*/                                                                                                                                                                                                                                                                                         
    int                                     SignalLevel_minus_dbm;  /*!< Signal Level, in -dBm resolution. For User-Defined Tones signals or Call Progress Tones signals, if the value 0 is defined in this field (SignalLevel) the levels defined in the ::Ac49xUserDefinedTonesDeviceConfiguration or ::Ac49xCallProgressDeviceConfiguration functions will be generated.*/
    U32								        OnDuration;             /*!< On duration (identical to all digits) in msec.*/                                                            
    U32								        OffDuration;            /*!< Off duration (identical to all digits) in msec.*/                                                           
    U32								        PauseDuration;          /*!< Pause duration in msec. @n Applies if the field Tac49xExtendedIbsStringSignal_Format1_SignalAttr.SystemCode equals IBS_SYSTEM_CODE__SILENCE.*/
	Tac49xExtendedIbsStringSignal_Format1_SignalAttr	Signal[MAX_NUMBER_OF_EXTENDED_IBS_DIGITS__FORMAT_1];  /*!< Signal to be played.*/
	} Tac49xExtendedIbsString_Format1_Attr;

    /** @} */

    /**
     * @defgroup Tac49xCasAttr Tac49xCasAttr
     * @ingroup CHANNEL_COMMAND_STR
     * @brief
     * This structure is used as an input to the ::Ac49xSendCas function.
     * @{
     */

    /**
     * <BR>
     * This structure is used as an input to the ::Ac49xSendCas function. It contains information
     * on the CAS to be generated on the CAS signaling highway.
     */

typedef struct
    {
    Tac49xControl			 	 A         ;			 /*!< Signaling A bit.*/
    Tac49xControl				 B         ;			 /*!< Signaling B bit.*/
    Tac49xControl				 C         ;			 /*!< Signaling C bit.*/
    Tac49xControl				 D         ;			 /*!< Signaling D bit.*/
    Tac49xControl				 ReversalsA;			 /*!< Reversals on signaling A bit.*/
    Tac49xControl				 ReversalsB;			 /*!< Reversals on signaling B bit.*/
    Tac49xCasGenerationDirection CasGenerationDirection; /*!< CAS generation direction (TDM/NET). Refer to ::Tac49xCasGenerationDirection.*/
    } Tac49xCasAttr;

    /** @} */

typedef struct
	{
	Tac49xControl ReversalsA;                      /*!< CAS pattern reversals A (A inverts its value every 1.5 msec).*/
	Tac49xControl ReversalsB;                      /*!< CAS pattern reversals B (B inverts its value every 1.5 msec).*/
	Tac49xControl A;                               /*!< CAS pattern A.*/
	Tac49xControl B;                               /*!< CAS pattern B.*/
	Tac49xControl C;                               /*!< CAS pattern C.*/
	Tac49xControl D;                               /*!< CAS pattern D.*/
	U32 Duration;                                  /*!< Pattern generation duration. Resolution is 1 msec.*/
	} Tac49xCasPattern;							   /*!< CAS pattern.*/

    /**
     * @defgroup Tac49xCasStringCommandAttr Tac49xCasStringCommandAttr
     * @ingroup CHANNEL_COMMAND_STR
     * @brief
     * This structure is used as an input to the ::Ac49xSendCasString function.
     * @{
     */

    /**
     * <BR>
     * This structure is used as an input to the ::Ac49xSendCasString function. It contains information
     * on the CAS string to be generated on the CAS signaling highway.
     */

typedef struct
    {
	Tac49xCasGenerationDirection	Direction;                      /*!< The direction of the string generation. Default = TDM_CAS_PORT.*/
	Tac49xControl					Repeat;                         /*!< Controls whether the string is repeated. @li CONTROL__ENABLE = String is repeated. @li CONTROL__DISABLE = String is generated once (default).*/
	U8								NumberOfPatterns;               /*!< Number of patterns in the string. Range: 1-10.*/
	U32								TotalDuration;                  /*!< If parameter 'Repeat' is set to CONTROL__ENABLE, this parameter defines the total duration of the repeated string. If parameter 'Repeat' is set to CONTROL__DISABLE, the parameter is ignored. @n If this parameter set to -1, the string is repeated infinitely. Resolution is 1 msec.*/
	Tac49xCasPattern				Pattern[MAX_CAS_STRING_LENGTH]; /*!< Generated pattern. Refer to ::Tac49xCasPattern.*/
    } Tac49xCasStringCommandAttr;

    /** @} */


#ifndef NDOC
typedef struct
    {
	Tac49xRecordCommandType		Command;
	int							Location;
	} Tac49xProprietaryRecordCommandAttr; 
#endif /* NDOC */


    /**
     * @defgroup Tac49xMuteCngCommandAttr Tac49xMuteCngCommandAttr
     * @ingroup CHANNEL_COMMAND_STR
     * @brief
     * This structure is used as an input to the ::Ac49xMuteCngCommand function.
     * @{
     */

    /**
     * <BR>
     * This structure is used as an input to the ::Ac49xMuteCngCommand function. It contains the noise parameters. 
     */
typedef struct
    {
	int							AverageEnergyLevel;         /*!< Average energy level in -dBm units. @n Default = -63dBm (63). */
	Tac49xControl				MuteCngEnable;              /*!< <ul> <li> 0 = stop noise generation. <li> 1 = start noise generation. </ul> */
	int							ParcorCoefficientValue0;   /*!< Parcor coefficient value. @n Default = -16770. */
	int							ParcorCoefficientValue1;   /*!< Parcor coefficient value. @n Default = -258.   */
	int							ParcorCoefficientValue2;   /*!< Parcor coefficient value. @n Default = 0.      */
	int							ParcorCoefficientValue3;   /*!< Parcor coefficient value. @n Default = 2580.   */
	} Tac49xMuteCngCommandAttr;
    /** @} */


    /**
     * @defgroup Tac49xHdlcFramerMessageCommandAttr Tac49xHdlcFramerMessageCommandAttr
     * @ingroup CHANNEL_COMMAND_STR
     * @brief
     * This structure is used as an input to the ::Ac49xSendHdlcFramerMessage function.
     * @{
     */

    /**
     * <BR>
     * This structure is used as an input to the ::Ac49xSendHdlcFramerMessage function. It contains a massege 
     * that should be transmitted on the TDM link.
     */

typedef struct
    {
    Tac49xHdlcFramerCommandMessageType MessageType;  /*!< Message type.*/
    U16 MessageSize;                                 /*!< Message payload size, in bytes. Range: 1-300. This field is relevant only when Message Type = SEND_HDLC_FRAMER_MESSAGE.*/
    U8 Message[MAX_HDLC_MESSAGE_LENGTH];             /*!< HDLC Data Message. These bytes are relevant only when Message Type = SEND_HDLC_FRAMER_MESSAGE.*/
    } Tac49xHdlcFramerMessageCommandAttr;

    /** @} */

    /**
     * @defgroup Tac49xSetRtcpParametersAttr Tac49xSetRtcpParametersAttr
     * @ingroup CHANNEL_COMMAND_STR
     * @brief
     * This structure is used as an input to the ::Ac49xSetRegularRtcpParameters function.
     * @{
     */

    /**
     * <BR>
     * This structure is used as an input to the ::Ac49xSetRegularRtcpParameters function.
     */

typedef struct
	{
	Tac49xRtcpParameterIndex ParameterIndex;  /*!< Index of the modified parameter. @n Default = RTCP_PARAMETER_INDEX__TRANSMIT_PACKET_COUNT.*/
	U32						 ParameterValue;  /*!< The value of the parameter. If the value size is less then 32 bits, then it is located in the LS bits.*/
	} Tac49xSetRtcpParametersAttr;

    /** @} */

    /**
     * @defgroup Tac49xSet3GppUserPlaneRateControlAttr Tac49xSet3GppUserPlaneRateControlAttr
     * @ingroup CHANNEL_COMMAND_STR
     * @brief
     * This structure is used as an input to the ::Ac49xRegularChannelSet3GppUserPlaneRateControl function.
     * @{
     */

    /**
     * <BR>
     * This structure is used as an input to the ::Ac49xRegularChannelSet3GppUserPlaneRateControl function.
     */

typedef struct
	{
    Tac49xControl	LocalRateEnable;   /*!< AMR local rate.Default = CONTROL__DISABLE.*/
    Tac49xControl	RemoteRateEnable;  /*!< AMR remote rate. Default = CONTROL__DISABLE.*/
    int				LocalRate;         /*!< The AMR or AMR-WB local rate (the local rate of the encoder). Refer to ::Tac49x3GppRate for the AMR rates and ::Tac49x3GppWbRate for the AMR-WB rates. @n Default = _3GPP_RATE__12_2_KBPS for AMR or _3GPP_RATE__23_85_KBPS for AMR-WB. @n This field is enabled only if ::Tac49xSet3GppUserPlaneRateControlAttr.LocalRateEnable = 1.*/
	int				RemoteRate;        /*!< The AMR or AMR-WB remote rate. The remote rate is the rate that the encoder on the other side should use. Refer to ::Tac49x3GppRate for the AMR rates and ::Tac49x3GppWbRate for the AMR-WB rates.  @n Default = _3GPP_RATE__12_2_KBPS for AMR or _3GPP_RATE__23_85_KBPS for AMR-WB.  @n This field is enabled only if ::Tac49xSet3GppUserPlaneRateControlAttr.RemoteRateEnable = 1.*/
	} Tac49xSet3GppUserPlaneRateControlAttr;

    /** @} */

    /**
     * @defgroup Tac49xSetRfc3267AmrRateControlAttr Tac49xSetRfc3267AmrRateControlAttr
     * @ingroup CHANNEL_COMMAND_STR
     * @brief
     * This structure is used as an input to the ::Ac49xSetRfc3267AmrRateControl function.
     * @{
     */

    /**
     * <BR>
     * This structure is used as an input to the ::Ac49xSetRfc3267AmrRateControl function.
     */

typedef struct
    {
    Tac49xControl                   LocalRateEnable;   /*!< Enables AMR/AMR-WB local rate. Default = CONTROL__DISABLE.*/
    Tac49xControl                   RemoteRateEnable;  /*!< Enables AMR/AMR-WB remote rate. @li Default = CONTROL__DISABLE.*/
    U8	                            LocalRate;         /*!< AMR/AMR-WB local rate - the encoder local rate. Refer to the table <A HREF="AMR and Wide-Band AMR Rates.html">AMR and AMR-WB rates </A>. This field is enabled only if LocalRateEnable = CONTROL__ENABLE.*/
    U8	                            RemoteRate;        /*!< AMR/AMR-WB remote rate (same as RFC 3267 CMR) - the encoder remote rate. Refer to the table <A HREF="AMR and Wide-Band AMR Rates.html">AMR and AMR-WB rates </A>. This field is enabled only if RemoteRateEnable = CONTROL__ENABLE.*/
    Tac49xRfc3267RedundancyLevel    RedundancyLevel;   /*!< AMR/AMR-WB redundancy, according to RFC 3267. @li 0 = No redundancy (default) @li 1-5 = Redundancy level 1-5 @li 6-7 = Reserved  */ 
    } Tac49xSetRfc3267AmrRateControlAttr;

    /** @} */

typedef struct 
	{
    Tac49xTrunkTestingMeasurementOperation			Operation				;
    Tac49xControl									CMessageFilter			;       
    Tac49xControl									CNotchFilter			;       
    Tac49xTrunkTestingMeasurementUnits				MeasurementUnits		;
    Tac49xTrunkTestingMeasurementDirection			Direction				;     
    Tac49xIbsSystemCode								TriggerToneIbsSystemCode;
    int												TriggerToneDigit		;       
    Tac49xTrunkTestingMeasurementReferenceEnergy	ReferenceEnergy			;       
    Tac49xTrunkTestingMeasurementTriggerType		TriggerType				;
    int												MeasurementDuration		;
    } Tac49xTrunkTestingMeasurementAttr;       

    /**
     * @defgroup Tac49xBerTestAttr Tac49xBerTestAttr
     * @ingroup CHANNEL_COMMAND_STR
     * @brief
     * This structure is used as an input to the ::Ac49xStartBerTest function.
     * @{
     */

    /**
     * <BR>
     * This structure is used as an input to the ::Ac49xStartBerTest function. It contains configuration parameters
     * of the BER (Bit Error Rate) test.
     */

typedef struct 
	{
    Tac49xBerTestRate							Rate;                           /*!< Test bit rate (56 k/64 k)*/
    Tac49xBerTestAction							Action;                         /*!< The BER test action (start, stop. etc.).*/
    Tac49xBerTestInjectedPattern				InjectedPattern;                /*!< The injected pattern.*/
    Tac49xBerTestDirection						Direction;                      /*!< Test direction.*/
    int											UserDefinedPatternSize;         /*!< If the parameter 'UserDefinedPattern' is set to BER_TEST_INJECTED_PATTERN__FIXED_USER_DEFINED, this field defines the pattern size in bits. @n Range: 3-24.*/
    int											UserDefinedPattern;             /*!< If the field 'InjectedPattern' is set to FIXED_USER_DEFINED, this field defines the pattern. If the pattern size is smaller than 24 bits, the pattern is taken from the LS bits of the field.*/
    int											ErrorBitsInsertionNumber;       /*!< Configures the insertion of bit errors to the transmitted pattern. @li 0 = No Error Insertion. @li 1-999 = Insertion of pseudo-random, single or multiple, bit errors from 1 to 999, at an interval defined in parameter ErrorBitsInsertionInterval. @li 1000-1023 = Reserved.*/
    Tac49xBerTestErrorBitsInsertionInterval		ErrorBitsInsertionInterval;     /*!< Configures the interval of Bit Error Insertion.*/
    Tac49xBerTestErrorBitsInsertionTimeMode		ErrorBitsInsertionTimeMode;     /*!< Configures the Error Time Insertion Mode.*/
    Tac49xBerTestErrorBitsInsertionPatternMode	ErrorBitsInsertionPatternMode;  /*!< Configures the Error Pattern Insertion Mode.*/
    int											TestDuration;                   /*!< Test duration, in seconds.  If this field is set to 0, the test runs infinitely. When the test ends, the channel sends a report and automatically terminates.*/
    } Tac49xBerTestAttr;

    /** @} */

    /**
     * @defgroup Tac49xUtopiaStatusRequestAttr Tac49xUtopiaStatusRequestAttr
     * @ingroup CHANNEL_COMMAND_STR
     * @brief
     * This structure is used as an input to the ::Ac49xUtopiaStatusRequest function.
     * @{
     */

    /**
     * <BR>
     * This structure is used as an input to the ::Ac49xUtopiaStatusRequest function.
     */

typedef struct
    {
    Tac49xControl ClearStatistics;  /*!< @li CONTROL__DISABLE = Disable Clear Statistics. @li CONTROL__ENABLE = Enable Clear Statistics.*/
    Tac49xControl StatusRequest;    /*!< @li CONTROL__DISABLE = Disable Status Request. @li CONTROL__ENABLE = Enable Status Request.*/
    } Tac49xUtopiaStatusRequestAttr;

    /** @} */

    /**
     * @defgroup Tac49xGetMediaProtectiontInfoAttr Tac49xGetMediaProtectiontInfoAttr
     * @ingroup CHANNEL_COMMAND_STR
     * @brief
     * This structure is used as an input to the ::Ac49xGetRegularChannelMediaProtectionInfo function.
     * @{
     */

    /**
     * <BR>
     * This structure is used as an input to the ::Ac49xGetRegularChannelMediaProtectionInfo function.
     */

typedef struct
	{
	Tac49xMediaProtectionInfoType InfoType;  /*!< Information type.*/
	} Tac49xGetMediaProtectiontInfoAttr;

    /** @} */

typedef struct
    {
	Tac49xSs7Mtp1CommandMessageType MessageType;
	U16								DataSize;
    U8 Data[MAX_SS7_DATA_SIZE];
    } Tac49xSs7Mtp1CommandAttr;

    /**
     * @defgroup Tac49xSetRfc3558RateModeAttr Tac49xSetRfc3558RateModeAttr
     * @ingroup CHANNEL_COMMAND_STR
     * @brief
     * This structure is used as an input to the ::Ac49xSetRfc3558RateMode function.
     * @{
     */

    /**
     * <BR>
     * This structure is used as an input to the ::Ac49xSetRfc3558RateMode function.
     */

typedef struct
	{
	U8							LocalRateMode;                               /*!< The rate mode of the local encoder according to RFC 3558. @li 0 = All rate 1 frames remain in rate 1. @li 1 = 3/4 of the rate 1 frames remain as rate 1 and 1/4 are changed to rate 1/2. @li 2 = 1/2 of the rate 1 frames remain as rate 1 and 1/2 are changed to rate 1/2. @li 3 = 1/4 of the rate 1 frames remain as rate 1 and 3/4 are changed to rate 1/2. @li 4 = All rate 1 frames are changed to rate 1/2.*/
	U8							RemoteRateMode;                              /*!< The rate mode of the remote encoder according to RFC 3558. @li 0 = All rate 1 frames remain in rate 1. @li 1 = 3/4 of the rate 1 frames remain as rate 1 and 1/4 are changed to rate 1/2. @li 2 = 1/2 of the rate 1 frames remain as rate 1 and 1/2 are changed to rate 1/2. @li 3 = 1/4 of the rate 1 frames remain as rate 1 and 3/4 are changed to rate 1/2. @li 4 = All rate 1 frames are changed to rate 1/2.*/
	Tac49xControl				LocalRateModeControlledByRemoteSideEnable;   /*!< Enables Local Rate Mode Conrol from the remote side (by the MMM parameter on the received packets).*/
	U8							LocalMaxRate;                                /*!< The maximum allowed local rate. @li 0 = Reserved. @li 1 = Rate 1/8. @li 2 = Reserved. @li 3 = Rate 1/2. @li 4 = Rate 1.*/
	U8							LocalMinRate;                                /*!< The minimum allowed local rate. @li 0 = Reserved. @li 1 = Rate 1/8. @li 2 = Reserved. @li 3 = Rate 1/2. @li 4 = Rate 1.*/
	} Tac49xSetRfc3558RateModeAttr;

    /** @} */

    /**
     * @defgroup Tac49xTimeSlotManipulationCommandAttr Tac49xTimeSlotManipulationCommandAttr
     * @ingroup CHANNEL_COMMAND_STR
     * @brief
     * This structure is used as an input to the ::Ac49xTimeSlotManipulationCommand function.
     * @{
     */

    /**
     * <BR>
     * This structure is used as an input to the ::Ac49xTimeSlotManipulationCommand function.
     */

typedef struct
	{
	Tac49xTimeSlotManipulationCommandType	CommandType;    /*!< Swap or loop type.*/
	Tac49xTdmPortType						RxTdmPortType;  /*!< Rx slot's port. The Rx slot is defined by the combination of the parameters RxTdmPortType and RxChannelId.*/
	U8										RxChannelId;    /*!< The channel ID of the Rx slot. The Rx slot is defined by the combination of the parameters RxTdmPortType and RxChannelId.*/
	Tac49xTdmPortType						TxTdmPortType;  /*!< Tx slot's port. The Tx slot is defined by the combination of the parameters TxTdmPortType and TxChannelId.*/
	U8										TxChannelId;    /*!< The channel ID of the Tx slot. The Tx slot is defined by the combination of the parameters TxTdmPortType and TxChannelId.*/
	} Tac49xTimeSlotManipulationCommandAttr;

    /** @} */

    /**
     * @defgroup Tac49xModuleStatusRequestAttr Tac49xModuleStatusRequestAttr
     * @ingroup CHANNEL_COMMAND_STR
     * @brief
     * This structure is used as an input to the ::Ac49xModuleStatusRequest function.
     * @{
     */

    /**
     * <BR>
     * This structure is used as an input to the ::Ac49xModuleStatusRequest function.
     */

typedef struct
	{
	Tac49xModuleType	ModuleType;  /*!< Module.*/
	} Tac49xModuleStatusRequestAttr;

    /** @} */

    /**
     * @defgroup Tac49xRecordCommandAttr Tac49xRecordCommandAttr
     * @ingroup CHANNEL_COMMAND_STR
     * @brief
     * This structure is used as an input to the ::Ac49xRecordCommand function.
     * @{
     */

    /**
     * <BR>
     * This structure is used as an input to the ::Ac49xRecordCommand function.
     */


typedef struct
    {
    Tac49xRecordCommandType Type;							/*!< @li 0 = TDM Record Start @li 1 = TDM Record Stop @li 2 = Network Record Start @li 3 = Network Record Stop */
    Tac49xCoder             Coder;							/*!< @li 3 = PCM A-Law (G.711) @li 4 = PCM Mu-Law (G.711) Note that when �TDM Record� is activated, the field �Coder� is ignored and the format of the recorded packets (A-Law or Mu-Law) accords with the configuration of the field ::Tac49xDeviceCommonAttr.LawSelect in the ::Ac49xOpenDeviceConfiguration function. */
	Tac49xControl			ForceCoderOutputRecordEnable;   /*!< <ul> <li> 0 = Record location 0. <li> 1= Record location 1. </ul> Refer to <A HREF="RecordLocations.html">record location diagram</A>. */
    } Tac49xRecordCommandAttr;

    /** @} */

    /**
     * @defgroup Tac49xRtcpAppCommandAttr Tac49xRtcpAppCommandAttr
     * @ingroup CHANNEL_COMMAND_STR
     * @brief
     * This structure is used as an input to the ::Ac49xSendMediatedRtcpAppCommand function.
     * @{
     */

    /**
     * <BR>
     * This structure is used as an input to the ::Ac49xSendMediatedRtcpAppCommand function.
     */

typedef struct
    {                                                       
	Tac49xRtcpAppPacketFormat	PacketFormat;                 /*!< Reserved for internal use. This bit must be set to zero (0). */ 
    U8							SubType;                      /*!< Set according to RFC 3550. */
    U8							Name[RTCP_APP_NAME_LENGTH];   /*!< Four ASCII characters. Refer to RFC 3550. */
    U16							DataLength_In4Bytes;          /*!< Length of the Application Dependent Data in 32 bit. */
    U8							Data[RTCP_APP_DATA_LENGTH];   /*!< Must be a multiple of 32 bit. Max size is 64 bytes. Refer to RFC 3550. */
    } Tac49xRtcpAppCommandAttr;

    /** @} */

typedef struct
    {
    Tac49xDuaFramerCommandMessageType MessageType;  /*!< Message type.*/
    U16 MessageSize;                                /*!< Message payload size, in bytes. Range: 1-64. This field is relevant only when Message Type = SEND_HDLC_FRAMER_MESSAGE.*/
    U8 Message[MAX_DUA_MESSAGE_LENGTH];             /*!< DUA Data Message. These bytes are relevant only when Message Type = DUA_COMMAND_MESSAGE_TYPE__START.*/
    } Tac49xDuaFramerCommandAttr;

typedef struct
	{
	Tac49xControl	E31;
	Tac49xControl	E30;
	Tac49xControl	E29;
	Tac49xControl	E28;
	Tac49xControl	E27;
	Tac49xControl	E26;
	Tac49xControl	E25;
	Tac49xControl	E24;
	Tac49xControl	E23;
	Tac49xControl	E22;
	Tac49xControl	E21;
	Tac49xControl	E20;
	Tac49xControl   ModuleStatus;
	Tac49xControl	E18;
	Tac49xControl	UtopiaStatus;
	Tac49xControl	BerReport;
	Tac49xControl   TrunkTestingsEvent;
	Tac49xControl   UtopiaTestError;
	Tac49xControl   E13;
	Tac49xControl   FaxAndDataStatus;
	Tac49xControl   E11;
	Tac49xControl   DeviceStatus;
	Tac49xControl   E9;
	Tac49xControl   PacketizerStatus;
	Tac49xControl   CallerId;
	Tac49xControl   JitterBufferStatus;
	Tac49xControl   FaxRelayStatus;
	Tac49xControl   HdlcFramerStatus;
	Tac49xControl   CasEvent;
	Tac49xControl   IbsEvent;
	Tac49xControl   GeneralEvents;
	Tac49xControl   SystemError;
	} Tac49xEventsControlCommandAttr;


  /**
     * @defgroup Tac49xNtpTimeStampCommandAttr Tac49xNtpTimeStampCommandAttr
     * @ingroup CHANNEL_COMMAND_STR
     * @brief
     * This structure is used as an input to the function ::Ac49xNtpTimeStampCommand.
     * @{
     */

    /**
     * <BR>
     * This structure is used as an input to the function ::Ac49xNtpTimeStampCommand.
     */
typedef struct
    {
    U32 Time_x1Sec;                     /*!<  This field represents the NTP time, in seconds. */
    U32 Time_x1SecFraction;             /*!<  This field represents the NTP time, in fractions of seconds. This field's resolution is 1 / (2^32) seconds. */
    } Tac49xNtpTimeStampCommandAttr;
   /** @} */


typedef struct
    {
    U32 TimeStamp;
    U32 RemoteNtpTime_x1msec;
    U32 LocalPlayoutNtpTime_x1msec;
    U32 MaxLocalPlayoutNtpTime_x1msec;
    } Tac49xVideoAudioSyncMsgAttr;

   /**
     * @defgroup Tac49xG729EvRateControlCommandAttr Tac49xG729EvRateControlCommandAttr
     * @ingroup CHANNEL_COMMAND_STR
     * @brief
     * This structure is used as an input to the function ::Ac49xSetG729EvRateControl.
     * @{
     */

    /**
     * <BR>
     * This structure is used as an input to the function ::Ac49xSetG729EvRateControl.
     */
typedef struct
	{
    Tac49xControl LocalBitRateFieldEnable;      /*!< Local rate field @li 0 = Disable (default) @li 1 = Enable */
    Tac49xControl MaxRemoteBitRateFieldEnable;  /*!< Maximum remote bit rate field @li 0 = Disable (default) @li 1 = Enable*/
	Tac49xG729EvRate LocalBitRate;              /*!< Local bit rate. Default = G729EV_RATE_8_kbps */
	Tac49xG729EvRate MaxRemoteBitRate;          /*!< Maximum bit rate of remote side. Default = G729EV_RATE_8_kbps  */ 
	} Tac49xG729EvRateControlCommandAttr;

    /** @} */

           /****************/
           /*      TFO     */
           /****************/
typedef struct
	{
	union
		{
		Tac49xTandemFreeOperationCommandFunction Command;
		Tac49xTandemFreeOperationStatusFunction	 Status	;
		}u;
	} Tac49xTandemFreeOperationFunction;

typedef struct
	{
	Tac49xControl			CodecListEnable;
	Tac49xControl			GsmFr		;
	Tac49xControl			GsmHr 		;
	Tac49xControl			GsmEfr 		;
	Tac49xControl			FrAmr		;
	Tac49xControl			HrAmr		;
	Tac49xControl			UmtsAmr		;
	Tac49xControl			UmtsAmr2	;
	Tac49xControl			TdmaEfr		;
	Tac49xControl			PdcEfr		;
	Tac49xControl			FrAmrWb		;
	Tac49xControl			UmtsAmrWb	;
	} Tac49xTandemFreeOperationCodecListAttr;

typedef struct
	{
	Tac49xControl _4k75				;	 
	Tac49xControl _5k15			  	;
	Tac49xControl _5k9				;
	Tac49xControl _6k7			  	;
	Tac49xControl _7k4				;
	Tac49xControl _7k95			  	;
	Tac49xControl _10k2				;
	Tac49xControl _12k2				;
	} Tac49xTandemFreeOperationNarrowAmrRatesAttr;

typedef struct
	{
	Tac49xControl _4k75				;	 
	Tac49xControl _5k15			  	;
	Tac49xControl _5k9				;
	Tac49xControl _6k7			  	;
	Tac49xControl _7k4				;
	Tac49xControl _7k95			  	;
	Tac49xControl _10k2				;
	Tac49xControl _12k2				;
	Tac49xControl Wide8				;
	} Tac49xTandemFreeOperationWideAmrRatesAttr;

typedef struct
	{
	union
		{
		Tac49xTandemFreeOperationNarrowAmrRatesAttr Narrow;
		Tac49xTandemFreeOperationWideAmrRatesAttr   Wide;
		}u;
	} Tac49xTandemFreeOperationAmrCodecRatesAttr;

typedef struct
    {
	int								 			MaximumNumberOfAmrRates;
	Tac49xTandemFreeOperationCodecType			Type;
	Tac49xTandemFreeOperationAmrCodecRatesAttr	SupportedAmrRates;
	}Tac49xTandemFreeOperationCodecAttr;

typedef struct
    {
	Tac49xTandemFreeOperationSystem				System								;
	Tac49xTandemFreeOperationFunction			Function							;
	Tac49xControl								ActiveAmrCodecRateOptimization		;
	Tac49xTandemFreeOperationCodecListAttr		CodecList							;			 
	Tac49xTandemFreeOperationCodecAttr			SupportedCodec						;
	Tac49xTandemFreeOperationAmrCodecRatesAttr	ActiveAmrCodecRates					;
	int											NumberOfOptionalAmrCodecs			; 
	Tac49xTandemFreeOperationCodecAttr			OptionalAmrCodec[MAX_NUMBER_OF_TANDEM_FREE_OPERATION_OPTIONAL_AMR_CODECS];
	} Tac49xTandemFreeOperationAttr;

typedef struct
	{
    Tac49xAmdOperation				Operation;
    int								AnswerDetectSensitivity;
	int								DecisionParameter8;
	int								DecisionParameter1;
	int								DecisionParameter2;
	int								MaxPostGreetingSilenceTime;
	int								MaxGreetingTime;
	int								SilentCallTimeout;
	int								PreWorkDelay;
	int								DecisionTimeout;
	int								BeepDetectionTimeout; 
	Tac49xBeepDetectionMode			BeepDetectionMode;
	Tac49xBeepDetectionSensitivity	BeepDetectionSensitivity;
	int								MinVoiceLength_x5ms;
	Tac49xControl					LowEnergyDetectionEnable;
	} Tac49xAmdCommandAttr;


    /**
     * @defgroup Tac49xActivateFaxRelayAttr Tac49xActivateFaxRelayAttr
     * @ingroup CHANNEL_CFG_STRUCTURES
     * @brief
     * Defines fields for ::Ac49xActivateFaxRelayCommand.
     * @{
     */

    /**
     * <BR>
     * This structure defines fields for the ::Ac49xActivateFaxRelayCommand function.
     */


typedef struct
	{
	Tac49xControl		SwitchToFax;        /*!< Switch to Fax. @li 0 = Disable (default). The AC49x channel switches to fax relay only after valid fax relay signals/packets are detected. @li 1 = The AC49x channel switches to fax relay immediately after the function ::Ac49xActivateFaxRelayCommand is invoked. 'No Signal' fax relay packets are transmitted to the network until valid fax relay signals are detected.  */
	Tac49xControl		TxFilterT38;        /*!< Reserved for internal use. Must be set to 0.  */
	} Tac49xActivateFaxRelayAttr;

    /** @} */


    /**
     * @defgroup Tac49xOpenOrUpdate4WayConferenceConfigurationAttr Tac49xOpenOrUpdate4WayConferenceConfigurationAttr
     * @ingroup CHANNEL_CFG_STRUCTURES
     * @brief
     * Defines fields for ::Ac49xOpen4WayConferenceConfiguration.
     * @{
     */

    /**
     * <BR>
     * This structure defines fields for the ::Ac49xOpen4WayConferenceConfiguration function.
     */
typedef struct
	{
	int					ConferenceId;           /*!< The ID of the activated/updated conference.*/
	Tac49xControl		Member3HearsMember3;    /*!< Cross-gain matrix. @li 0 = Member 3 does not hear member 3 (default) @li 1 = Member 3  hears member 3 */
	Tac49xControl		Member3HearsMember2;    /*!< Cross-gain matrix. @li 0 = Member 3 does not hear member 2 @li 1 = Member 3  hears member 2 (default) */
	Tac49xControl		Member3HearsMember1;    /*!< Cross-gain matrix. @li 0 = Member 3 does not hear member 1 @li 1 = Member 3  hears member 1 (default) */
	Tac49xControl		Member3HearsMember0;    /*!< Cross-gain matrix. @li 0 = Member 3 does not hear member 0 @li 1 = Member 3  hears member 0 (default) */
	Tac49xControl		Member2HearsMember3;    /*!< Cross-gain matrix. @li 0 = Member 2 does not hear member 3 @li 1 = Member 2  hears member 3 (default) */
	Tac49xControl		Member2HearsMember2;    /*!< Cross-gain matrix. @li 0 = Member 2 does not hear member 2 (default) @li 1 = Member 2  hears member 2 */
	Tac49xControl		Member2HearsMember1;    /*!< Cross-gain matrix. @li 0 = Member 2 does not hear member 1 @li 1 = Member 2  hears member 1 (default) */
	Tac49xControl		Member2HearsMember0;    /*!< Cross-gain matrix. @li 0 = Member 2 does not hear member 0 @li 1 = Member 2  hears member 0 (default) */
	Tac49xControl		Member1HearsMember3;    /*!< Cross-gain matrix. @li 0 = Member 1 does not hear member 3 @li 1 = Member 1  hears member 3 (default) */
	Tac49xControl		Member1HearsMember2;    /*!< Cross-gain matrix. @li 0 = Member 1 does not hear member 2 @li 1 = Member 1  hears member 2 (default) */
	Tac49xControl		Member1HearsMember1;    /*!< Cross-gain matrix. @li 0 = Member 1 does not hear member 1 (default) @li 1 = Member 1  hears member 1 */
	Tac49xControl		Member1HearsMember0;    /*!< Cross-gain matrix. @li 0 = Member 1 does not hear member 0 @li 1 = Member 1  hears member 0 (default) */
	Tac49xControl		Member0HearsMember3;    /*!< Cross-gain matrix. @li 0 = Member 0 does not hear member 3 @li 1 = Member 0  hears member 3 (default) */
	Tac49xControl		Member0HearsMember2;    /*!< Cross-gain matrix. @li 0 = Member 0 does not hear member 2 @li 1 = Member 0  hears member 2 (default) */
	Tac49xControl		Member0HearsMember1;    /*!< Cross-gain matrix. @li 0 = Member 0 does not hear member 1 @li 1 = Member 0  hears member 1 (default) */
	Tac49xControl		Member0HearsMember0;    /*!< Cross-gain matrix. @li 0 = Member 0 does not hear member 0 (default) @li 1 = Member 0  hears member 0 */
	} Tac49xOpenOrUpdate4WayConferenceConfigurationAttr;
    /** @} */


    /**
     * @defgroup Tac49xClose4WayConferenceConfigurationAttr Tac49xClose4WayConferenceConfigurationAttr
     * @ingroup CHANNEL_CFG_STRUCTURES
     * @brief
     * Defines fields for ::Ac49xClose4WayConferenceConfiguration.
     * @{
     */

    /**
     * <BR>
     * This structure defines fields for the ::Ac49xClose4WayConferenceConfiguration function.
     */
typedef struct
	{
	int					ConferenceId;             /*!< The ID of the closed conference.*/
	} Tac49xClose4WayConferenceConfigurationAttr;
    /** @} */

    /**
     * @defgroup Tac49xAdd4WayConferenceLegConfigurationAttr Tac49xAdd4WayConferenceLegConfigurationAttr
     * @ingroup CHANNEL_CFG_STRUCTURES
     * @brief
     * Defines fields for ::Ac49xAdd4WayConferenceLegConfiguration.
     * @{
     */

    /**
     * <BR>
     * This structure defines fields for the ::Ac49xAdd4WayConferenceLegConfiguration function.
     */
typedef struct
	{
	Tac49x4WayConferenceLegType		LegType;         /*!< The Leg type. @li 0 = IP type @li 1 = TDM type */
	int								MemberId;        /*!< The order number of the participant in the conference.  */
	int								ConferenceId;    /*!< The ID of the conference to which the Leg is added. */
	} Tac49xAdd4WayConferenceLegConfigurationAttr;
    /** @} */

    /**
     * @defgroup Tac49xRemove4WayConferenceLegConfigurationAttr Tac49xRemove4WayConferenceLegConfigurationAttr
     * @ingroup CHANNEL_CFG_STRUCTURES
     * @brief
     * Defines fields for ::Ac49xRemove4WayConferenceLegConfiguration.
     * @{
     */

    /**
     * <BR>
     * This structure defines fields for the ::Ac49xRemove4WayConferenceLegConfiguration function.
     */
typedef struct
	{
	Tac49x4WayConferenceLegType		LegType;            /*!< The leg type of the participant in the conference that should be removed. @li 0 = IP type @li 1 = TDM type */         
	int								MemberId;           /*!< The order number of the participant in the conference.  */
	int								ConferenceId;       /*!< The ID of the conference from which the Leg is removed. */    
	} Tac49xRemove4WayConferenceLegConfigurationAttr;
    /** @} */

    /**
     * @defgroup Tac49xGet4WayConferenceDetailsCommandAttr Tac49xGet4WayConferenceDetailsCommandAttr
     * @ingroup CHANNEL_CFG_STRUCTURES
     * @brief
     * Defines fields for ::Ac49xGet4WayConferenceDetailsCommand.
     * @{
     */

    /**
     * <BR>
     * This structure defines fields for the ::Ac49xGet4WayConferenceDetailsCommand function.
     */
typedef struct
	{
	int					ConferenceId;               /*!< The ID of the conference to be queried. */
	} Tac49xGet4WayConferenceDetailsCommandAttr;
    /** @} */

typedef struct
	{
	int							KeySize;
	Tac49xProtectionTestAction	Action;
	int							DataSize;
	U8  Payload[MAX_PAYLOAD_SIZE - sizeof(Tac49xHpiPacketHeader) - sizeof(Tac49xProprietaryPacketHeader) - 4];
	} Tac49xProtectionTestCommandAttr;

    /**
     * @defgroup Tac49xUpdateIbsGenerationLevelCommandAttr Tac49xUpdateIbsGenerationLevelCommandAttr
     * @ingroup CHANNEL_COMMAND_STR
     * @brief
     * This structure is used as an input to the ::Ac49xUpdateIbsGenerationLevelCommand function.
     * @{
     */

    /**
     * <BR>
     * This structure is used as an input to the ::Ac49xUpdateIbsGenerationLevelCommand function. It contains the signal level.
     */
typedef struct
	{
	int			SignalLevel_minus_dbm;               /*!< The signal level in -dBm. */
	} Tac49xUpdateIbsGenerationLevelCommandAttr;   
    /** @} */

typedef struct
    {
	int 								MaxPayloadSizeForSprtTransportChannel2;
	int 								MaxPayloadSizeForSprtTransportChannel0;
	int 								MaxPayloadSizeForSprtTransportChannel3;
	int 								MaxWindowSizeForSprtTransportChannel2;
	int									RxPayloadTypeOfSprtPackets;
	int									TxPayloadTypeOfSprtPackets;
	Tac49xSprtPacketsProtocol 			SprtPacketsTransmitProtocol;
	Tac49xSprtPacketsProtocol 			SprtPacketsReceiveProtocol;
	} Tac49xActivateOrUpdateDataRelayConfigurationAttr;

typedef struct
    {
	Tac49xDataRelayClearDownReason		ClearDownReason;
	} Tac49xDeactivateDataRelayConfigurationAttr;

typedef struct
    {
	Tac49xModemTransmissionPower				ModemTransmissionPower;
	Tac49xControl 								V32ModulationTypeEnable;
	Tac49xControl 								V34ModulationTypeEnable;
	Tac49xControl 								ModemDiagnosticsEnable;
	Tac49xControl								V8Support;
	Tac49xDataRelayTransmissionPcmLevel			PcmLevelOfTransmissionOfPhase1;
	Tac49xAutomaticRetrainRenegotiationControl	AutomaticRetrainRenegotiationControl;
	Tac49xDataRelayBitRate 						MaxTxBitRate;
	Tac49xDataRelayBitRate 						MinTxBitRate;
	Tac49xDataRelayBitRate 						MaxRxBitRate;
	Tac49xDataRelayBitRate 						MinRxBitRate;
	Tac49xControl								V34_3429sps_SymbolRateEnable;
	Tac49xControl								V34_3200sps_SymbolRateEnable;
	Tac49xControl								V34_3000sps_SymbolRateEnable;
	Tac49xControl								V34_2800sps_SymbolRateEnable;
	Tac49xControl								V34_2743sps_SymbolRateEnable;
	Tac49xControl								V34_2400sps_SymbolRateEnable;
	Tac49xControl								DebugServiceModemParametersEnable;
	Tac49xControl								DebugServiceModemPhase2Enable;
	Tac49xControl								DebugServiceMoipPhase1Enable;
	Tac49xControl								DebugServiceModemPhase1Enable;
	Tac49xControl								DebugServiceModemPhase3Enable;
	Tac49xControl								DebugServiceModemEqualizerErrorEnable;
	Tac49xControl								DebugServiceModemDownBitsEnable;
	Tac49xControl								DebugServiceModemUpBitsEnable;
	Tac49xControl								DebugServiceModemConnectEnable;
	Tac49xControl								DebugServiceModemRetrainEnable;
	Tac49xControl								DebugServiceModemRateReEnable;
	Tac49xControl								DebugServiceModemPdsnrEnable;
	Tac49xControl								DebugServiceModemEcFilterEnable;
	Tac49xControl								DebugServiceModemEqualizerFilterEnable;
	Tac49xControl								DebugServiceModemEcCleanSamplesEnable;
	Tac49xControl								DebugServiceModemRxSymbolsEnable;
	Tac49xControl								DebugServiceOutgoingDataToNetwork;
	Tac49xControl								DebugServiceIncomingDataFromNetwork;
	Tac49xControl								DebugServiceSamplesIn;
	Tac49xControl								DebugServiceSamplesOut;
	Tac49xControl								DebugServiceMoipState;
	Tac49xControl								DebugServiceSseState;
	Tac49xControl								DebugServiceMoipBuffersLostData;
	} Tac49xAdvancedDataRelayConfigurationAttr;

typedef struct                     
    {	
    Tac49xSwitchToModemInitiatingFlag		InitiatingFlag;
	Tac49xSwitchToModemReasonIdentifierCode	ReasonIdentifierCode;
	int 									RtpTxSequenceNumber;
	int 									RtpTxTimeStamp;
	int 									RtpTxSsrc;
	} Tac49xSwitchToModemCommandAttr;

typedef struct                     
    {	
	Tac49xModemEndedReasonIdentifierCode	ReasonIdentifierCode;
	int 									RtpTxSequenceNumber;
	int 									RtpTxTimeStamp;
	int 									RtpTxSsrc;
	} Tac49xModemRelayEndedCommandAttr;

typedef struct                     
    {

	Tac49xModemRelayPacketDirection		Direction;
	int									SsePacketSize;
	U8									SsePacketPayload[MAX_PAYLOAD_SIZE];
	} Tac49xModemRelaySseCommandAttr;

           /************************/
           /*      CALLER_ID       */
           /************************/
typedef enum
    {
    CALLER_ID_SERVICE_TYPE__1_ON_HOOK,
    CALLER_ID_SERVICE_TYPE__2_OFF_HOOK
    } Tac49xCallerIdServiceType;  
 
typedef enum
    {
    ETSI_ON_HOOK_METHOD__NO_PRECEDING_DUAL_TONE_ALERT_SIGNAL,
    ETSI_ON_HOOK_METHOD__WITH_PRECEDING_DUAL_TONE_ALERT_SIGNAL
    } Tac49xEtsiOnhookMethod;

typedef enum
    {
    ETSI_ON_HOOK_MARK_SIGNAL_TYPE__180_MARK_BITS_TRANSMITTED_AFTER_SEIZURE_SIGNAL,
	ETSI_ON_HOOK_MARK_SIGNAL_TYPE__80_MARK_BITS_TRANSMITTED_AFTER_SEIZURE_SIGNAL
	} Tac49xEtsiOnhookMarkSignalType;

typedef enum
    {
    CALLER_ID_EVENT__NONE_EVENT,
    CALLER_ID_EVENT__RECEIVE_PROCESS_COMPLETED_CHECKSUM_CRC_IS_VALID,
    CALLER_ID_EVENT__RECEIVE_PROCESS_COMPLETED_CHECKSUM_CRC_IS_ABSENT,
    CALLER_ID_EVENT__RECEIVE_PROCESS_COMPLETED_CHECKSUM_CRC_IS_INVALID,
    CALLER_ID_EVENT__RECEIVE_PROCESS_ABORTED_DUE_TO_NO_SIGNAL_TIMEOUT,
    CALLER_ID_EVENT__RECEIVE_PROCESS_ABORTED_DUE_TO_DISCONNECTION,
    CALLER_ID_EVENT__RECEIVE_PROCESS_ABORTED_DUE_TO_NO_SIGNAL_END_TIME_OUT,
    CALLER_ID_EVENT__GENERATION_COMPLETED_SUCCESSFULLY					 = 17,
    CALLER_ID_EVENT__GENERATION_ABORTED_DUE_TO_STOP_COMMAND,
    CALLER_ID_EVENT__GENERATION_COMPLETED_OR_ABORTED_DUE_TO_ACKNOWLEDGE_TIMEOUT
    } Tac49xCallerIdEvent;

    /**
     * @defgroup Tac49xCallerIdAttr Tac49xCallerIdAttr
     * @ingroup CHANNEL_COMMAND_STR
     * @brief
     * This structure is used as an input to the ::Ac49xSendCallerIdMessage function.
     * @{
     */

    /**
     * <BR>
     * This structure is used as an input to the ::Ac49xSendCallerIdMessage function. It contains Caller ID 
     * message to be played.
     */

typedef struct
	{ 
    Tac49xCallerIdStandard          Standard;					/*!< Caller ID standard.*/
    Tac49xControl                   ChecksumByteAbsent;			/*!< Must be set to CONTROL__DISABLE.*/
    Tac49xCallerIdServiceType       ServiceType;				/*!< Protocol of the packet. @li On-hook service (Type 1). @li Off-hook service (Call Waiting) (Type 2).*/
    Tac49xEtsiOnhookMethod          EtsiOnhookMethod;			/*!< Method. Applies only to ETSI "on-hook" service. @li ETSI on-hook service transmitted without preceding Dual Tone Alerting Signal (DT-AS). @li ETSI on-hook service transmitted with preceding Dual Tone Alerting Signal (DT-AS).*/
	Tac49xEtsiOnhookMarkSignalType  EtsiOnhookMarkSignalType;   /*!< The mark signal type for ETSI "On Hook" service. Refer to ::Tac49xEtsiOnhookMarkSignalType. @n Default = 180_MARK_BITS_TRANSMITTED_AFTER_SEIZURE_SIGNAL (0). */
    Tac49xControl                   Error;						/*!< Must be set to CONTROL__DISABLE.*/
    Tac49xCallerIdEvent             Event;						/*!< Must be set to CALLER_ID_EVENT__NONE_EVENT.*/
    } Tac49xCallerIdAttr;

    /** @} */

typedef volatile struct
	{		
    U8 Value  ;
    /*U8 Type   ;*/
    } Tac49xCallerIdSegment;


typedef volatile struct
    {
    Tac49xCallerIdSegment Type  ;
    Tac49xCallerIdSegment Length;
    } Tac49xCallerIdMessageTitle, Tac49xCallerIdPayloadTitle;

typedef volatile struct
    {
    Tac49xCallerIdMessageTitle Title;
    Tac49xCallerIdSegment Data[MAX_CALLER_ID_MESSAGE_LENGTH];
    } Tac49xCallerIdAsciMessage;


            /*********************************************/
            /*  T H E   C O N V E N I E N T   G R O U P  */
            /*********************************************/

    /**
     * @defgroup Tac49xDriverVersionInfo Tac49xDriverVersionInfo
     * @ingroup CONVENIENT_STRUCTURES
     * @brief
     * This structure contains the identifying information of the AC49x driver.
     * @{
     */

    /**
     * <BR>
     * This structure contains the identifying information of the AC49x driver.
     */

typedef struct
    {             /* just a pilot structure */
    char MandatoryVersionPrefix[32];   
    char SubVersion[32]         ; 
    char Date[11]               ;   /*  mm.dd.yyyy  */
    char sComments[300]         ;
    } Tac49xDriverVersionInfo;

    /** @} */

    /**
     * @defgroup Tac49xDspDownloadInfo Tac49xDspDownloadInfo
     * @ingroup CONVENIENT_STRUCTURES
     * @brief
     * This structure contains the identifying information of the AC49x download image.
     * @{
     */

    /**
     * <BR>
     * This structure contains the identifying information of the AC49x download image.
     */

typedef struct
    {
	char Version[6];
    char Revision[3];
    char Patch[3];
    char PatchNumber[3];
    char Reserved[7];
    char Month[3];
    char Day[3];
    char Year[5];
    char Name[20];
    } Tac49xDspDownloadInfo;

    /** @} */

    /**
     * @defgroup Tac49xSetupDeviceAttr Tac49xSetupDeviceAttr
     * @ingroup CONVENIENT_STRUCTURES
     * @brief
     * This structure is used as an input to the ::Ac49xSetupDevice function.
     * @{
     */

    /**
     * <BR>
     * This structure is used as an input to the ::Ac49xSetupDevice function. It contains all of the 
     * parameters that are common to all of the channels on the device.
     */

typedef struct
    {
    Tac49xDeviceControlRegisterAttr                 DeviceControlRegister;
    Tac49xCallProgressDeviceConfigurationAttr       CallProgress;
    Tac49xUserDefinedTonesDeviceConfigurationAttr   UserDefinedTones;
    Tac49xAgcDeviceConfigurationAttr                Agc;
    Tac49xExtendedDeviceConfigurationAttr           Extended;
	Tac49xFormat									OpenDeviceFormat;  /*!< This bit determines which function is called: ::Ac49xOpenDeviceConfiguration_Format0 or ::Ac49xOpenDeviceConfiguration_Format1. */
    Tac49xOpenDeviceConfiguration_Format0Attr       Open;		 /* Format 0 */
	Tac49xOpenDeviceConfiguration_Format1Attr       OpenFormat1; /* Format 1 */
#if (  (AC49X_DEVICE_TYPE == AC491_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC491L_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC491_491L_DEVICE)\
    )          
#ifdef UTOPIA_ROUTING                             
	Tac49xUtopiaRoutingConfigurationAttr		    UtopiaRouting;
#endif /* UTOPIA_ROUTING	*/
#endif /* AC49X_DEVICE_TYPE */

#if (  (AC49X_DEVICE_TYPE == AC494_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC495_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC495L_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC496_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC496E_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC497_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC494E_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC495E_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC496D_DEVICE)\
    )
	Tac49xEchoCancelerEqualizerConfigurationAttr	EchoCancelerEqualizer[ECHO_CANCELER_CODEC_INTERFACE__COUNT];
	Tac49xDeviceLowPassFilterConfigurationAttr		DeviceLowPassFilter;
#endif /*((AC49X_DEVICE_TYPE == AC494_DEVICE) || (AC49X_DEVICE_TYPE == AC495_DEVICE))*/
	
    } Tac49xSetupDeviceAttr;

    /** @} */

    /**
     * @defgroup Tac49xSetupChannelAttr Tac49xSetupChannelAttr
     * @ingroup CONVENIENT_STRUCTURES
     * @brief
     * This structure contains all the parameters of a single channel.
     * @{
     */

    /**
     * <BR>
     * This structure contains all the parameters of a single channel.
     */

typedef struct
    {
    Tac49xOpenOrUpdateChannelConfigurationAttr                  OpenOrUpdateChannel;
    Tac49xAdvancedChannelConfigurationAttr                      Advanced;
    Tac49xActivateOrUpdateAtmChannelConfigurationAttr           ActivateOrUpdateAtm[2];
    Tac49xOpenOrUpdateChannelConfigurationAttr                  MediatedChannelConfiguration;
    Tac49xActivateOrUpdateRtpChannelConfigurationAttr           ActivateOrUpdateRtp[2];
#if (AC49X_DRIVRES_VERSION>=AC49X_DRIVER_VERSION_105)
    Tac49xCloseRtpChannelConfigurationAttr						CloseRtp[2];
#endif
	Tac49xT38ConfigurationAttr							        T38Configuration;
	Tac49xT38ConfigurationAttr							        MediatedT38Configuration;
	Tac49xActivateOrDeactivate3WayConferenceConfigurationAttr   ActivateOrDeactivate3WayConferenceConfiguration;
	Tac49x3GppUserPlaneInitializationConfigurationAttr	        _3GppUserPlaneInitializationConfiguration[2];
	Tac49xRfc3267AmrInitializationConfigurationAttr				Rfc3267AmrInitializationConfiguration;
    Tac49xPacketCableProtectionConfigurationAttr                PacketCableProtectionConfiguration[2][ARRAY_OF__MEDIA_PROTECTION_DIRECTION];
	Tac49xFormat											    SrtpProtectionFormat;
    Tac49xSrtpProtectionConfigurationAttr						SrtpProtectionConfiguration[2][ARRAY_OF__MEDIA_PROTECTION_DIRECTION];
	Tac49xSrtpProtectionConfigurationFormat1_Attr				SrtpProtectionConfiguration_Format1[2][ARRAY_OF__MEDIA_PROTECTION_DIRECTION];
#if (  (AC49X_DEVICE_TYPE == AC494_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC495_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC495L_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC496_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC496E_DEVICE)\
    || (AC49X_DEVICE_TYPE == AC497_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC494E_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC495E_DEVICE)\
	|| (AC49X_DEVICE_TYPE == AC496D_DEVICE)\
    )
    Tac49xCodecConfigurationAttr                                CodecConfiguration;
	Tac49xFormat											    AcousticEchoCancelerConfigurationFormat;
	Tac49xAcousticEchoCancelerConfigurationAttr					AcousticEchoCancelerConfiguration;
	Tac49xAcousticEchoCancelerConfigurationFormat1_Attr			AcousticEchoCancelerConfiguration_Format1;
#endif/*((AC49X_DEVICE_TYPE == AC494_DEVICE) || (AC49X_DEVICE_TYPE == AC495_DEVICE))*/
	Tac49xRfc3558InitializationConfigurationAttr				Rfc3558InitializationConfiguration;
    Tac49xG729EvInitializationConfigurationAttr                 G729EvInitializationConfiguration;
    Tac49xActivateOrUpdate3GppIubChannelConfigurationAttr       _3GppIubChannelConfiguration;
	Tac49xRtcpXrConfigurationAttr								RtcpXrConfiguration[2];
	Tac49xActivateFaxRelayAttr									ActivateFaxRelay;
	Tac49xUemClipInitializationAttr								UemClipInitialization;
	Tac49xG7111InitializationAttr								G7111Initialization;
	Tac49xMicrosoftRtaInitializationAttr						MicrosoftRtaInitialization;
	Tac49xActivateOrUpdateDataRelayConfigurationAttr			ActivateOrUpdateDataRelay;
	Tac49xAdvancedDataRelayConfigurationAttr					AdvancedDataRelay;
	Tac49xDeactivateDataRelayConfigurationAttr					DeactivateDataRelay;
	Tac49xNetworkAcousticEchoSuppressorConfigurationAttr		NetworkAcousticEchoSuppressor;	
	Tac49xSpeexInitializationConfigurationAttr					SpeexInitialization;
	Tac49xOpusInitializationConfigurationAttr					OpusInitialization;
	Tac49xSilkInitializationConfigurationAttr					SilkInitialization;
	Tac49xVoiceEnhancementConfigurationAttr						VoiceEnhancementConfiguration;
	Tac49xActivateOrDeactivate9WayConferenceConfigurationAttr   ActivateOrDeactivate9WayConferenceConfiguration;
	Tac49x9WayConferenceResourceConfigurationAttr				_9WayConferenceResourceConfiguration;
} Tac49xSetupChannelAttr;

typedef struct
    {
	Tac49xOpenOrUpdate4WayConferenceConfigurationAttr			OpenOrUpdateConference;
	Tac49xClose4WayConferenceConfigurationAttr					CloseConference;		
	} Tac49xSetup4WayConferenceAttr;

typedef struct
    {
	Tac49xAdd4WayConferenceLegConfigurationAttr					AddLeg;
	Tac49xRemove4WayConferenceLegConfigurationAttr				RemoveLeg;
	} Tac49xSetup4WayConferenceLegAttr;

    /** @} */

    /**
     * @defgroup TFlushPacketFunction TFlushPacketFunction
     * @ingroup PACKET_RECORDING
     * @brief
     * Type of packet recording callback function.
     * @{
     */

    /**
     * <BR>
     * The @b TFlushPacketFunction function is a type of packet recording callback function that the user must supply
     * to the ::Ac49xInitializePacketRecording funcion. This function is invoked by the ::Ac49xTransmitPacket and
     * ::Ac49xReceivePacket functions.
     *
     * @param Device           [in] 
     * @param pPacket          [in] Pointer to the packet.
     * @param PacketSize       [in] The size of the packet in bytes.
     * @param PacketDirection  [in] Refer to ::Tac49xPacketDirection.
     *    
     * @par Return values:
     * @e None.
     * 
     * @req
     * @b Header: Declared in AC49xDrv_UserDefinedFunctions.h.
     */

typedef void (*TFlushPacketFunction)(int Device, char *pPacket, U32 PacketSize, Tac49xPacketDirection PacketDirection);

    /** @} */

typedef struct
	{
	void							(*Initialize)(void);
	void							(*Finalize)(void);
	Tac49xPacketRecordingErrorType	(*Add)(int Device, char* pPacket, unsigned int PacketSize, Tac49xPacketDirection PacketDirection);
	TFlushPacketFunction			FlushPacket;
	} Tac49xPacketRecording;



#endif /* AC49X_DEVICE_TYPE ==...Compatible device types "All the rest of this file is compiled only for the following devices:" */
#endif /*ifndef AC49XDEF_H*/
