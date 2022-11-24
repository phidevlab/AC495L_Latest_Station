/****************************************************************************
*
*	Company:			Audiocodes Ltd.
*
*   Project:			VoPP SoC
*
*   Hardware Module: 	AC494-EVM
*
*   File Name: 			dsp_drv.h
*
*
******************************************************************************
*
* 	DESCRIPTION:		definitions for the ac49xdsp driver
*
******************************************************************************/

#ifndef	__DSP_DRV_h
#define	__DSP_DRV_h

#include "AC49xDrv_Options.h"		/* Constant Configuration Options Definitions  */
#include "AC49xDrv_Config.h"		        /* User User Defined Configuration Selection Header File*/
#include "AC49xDrv_Definitions.h"		/* Strcutures, Enums and Constants Definitions */
#include "AC49xDrv_Api.h"
/*
#include "AC49xDrv_UserDefinedFunctions.h"
*/

#include "acGatewayDefs.h"    /* AudioCodes Gateway Definitions */
#if defined(CONFIG_SDP_IPPHONE) || defined(CONFIG_ORCHID)
#include "acIPPDriversDefs.h"
#endif


/*#define  DSP_DRV_DEBUG*/

#ifdef DSP_DRV_DEBUG
#define DSP_DRV_LogDebug(format,args...) printk(format,##args)
#else
#define DSP_DRV_LogDebug(format,args...)
#endif


#define ADDRESS_STR_MAX_LEN		128

#define HPI_HEADER_LEN 				4

#define MAX_MEDIA_PACKET_SIZE		1600

#define NUM_USER_TONES	6
#define NUM_OF_CHANNELS 4

#define MAX_NUM_OF_CHANNELS_PER_DSP 4 //AC49X_NUMBER_OF_CHANNELS
#define AC494_DSP_RECEIVER_PACKET_SIZE	1500
#define DSP_PROGRAM_SIZE	0x80000
#define DSP_PROGRAM_ORDER_SIZE	19 /* 0x80000 = 2^19 */
#define NUM_OF_DSP_CHANNELS      3 // 2 //		AC49X_NUMBER_OF_DEVICES*AC49X_NUMBER_OF_CHANNELS


#undef _ATYPE_
#define _ATYPE_		(__SIZE_TYPE__)

#undef _ATYPE32_
#define _ATYPE32_	(int)

#undef _ACAST32_
#define _ACAST32_	_ATYPE_ _ATYPE32_	/* widen if necessary */


#define PHYSADDR(a)  ((_ACAST32_ (a)) & 0x1fffffff)

#define CALLER_ID_MULTIPLE_DATA_MESSAGE_PARAMETER__DATE_AND_TIME 	1
#define CALLER_ID_MULTIPLE_DATA_MESSAGE_PARAMETER__CALLING_LINE_NUMBER 	2
#define CALLER_ID_MULTIPLE_DATA_MESSAGE_PARAMETER__CALLING_NAME 	7

#define SERVERPORT 50000

//kernel 2.6
#define DSP_MAJOR 241
#define DSP_COUNT 32


/* VLYNQ definition */
#define AC49X_VLYNQ_PORT__3_HI 1
#define AC49X_VLYNQ_PORT__5_LOW 0

#define AC49X_VLYNQ_PORT AC49X_VLYNQ_PORT__5_LOW


#define VLYNQ_TNETW1X30_MEM_OFFSET 	0xC0000000  /* Physical address of ACX111 memory */
#define VLYNQ_TNETW1X30_MEM_SIZE   		0x00040000  /* Total size of the ACX111 memory   */
#define VLYNQ_TNETW1X30_REG_OFFSET 	0xF0000000  /* PHYS_ADDR of ACX111 control registers   */
#define VLYNQ_TNETW1X30_REG_SIZE   		0x00022000  /* Size of ACX111 registers area, MAC+PHY  */
#define TNETW1X30_VL1_REMOTE_SIZE  		0x01000000
#define TNETW1X30_TX_MAP           			0x00000000   /* Window start address into APEX */
/* WLAN 1350 definition */
#define VLYNQ_TNETW1X50_MEM_OFFSET 	0x00000000  /* Physical address of TNETW1150 memory */
#define VLYNQ_TNETW1X50_MEM_SIZE   		0x00080000  /* Total size of the TNETW1150 memory   */
#define VLYNQ_TNETW1X50_REG_OFFSET 	0x00300000  /* PHYS_ADDR of TNETW1150 control registers   */
#define VLYNQ_TNETW1X50_REG_SIZE   		0x00100000  /* Size of TNETW1150 registers area, MAC+PHY  */
#define TNETW1X50_VL1_REMOTE_SIZE  		0x01000000
#define TNETW1X50_TX_MAP           			0x60000000   /* Window start address into APEX */

#define AVALANCHE_VL_MEM_SIZE  	   		0x04000000	/* 64 MBytes */

#define VLYNQ_TNETW1X50_VL0_PORTAL_OFFSET	0x60000000
#define VLYNQ_TNETW1X50_VL1_PORTAL_SIZE	     	(VLYNQ_VDSP_MEM_SIZE + VLYNQ_VDSP_RESOURCE_SIZE)
#define VLYNQ_TNETW1X50_VL1_PORTAL_OFFSET	0x40000000
#define VLYNQ_TNETW1X50_VL0_PORTAL_SIZE		AVALANCHE_VL_MEM_SIZE


#define DEFAULT_VLYNQ_POLARITY 		0
#define VLYNQ_POLARITY_LOW     		1
#define VLYNQ_POLARITY_HIGH    		2

#define AC498_BASE_ADRESS 			AVALANCHE_LOW_VLYNQ_MEM_MAP_BASE /*0xAC000000*/


/* VLYNQ prototypes */
void  vlynq_dev_init_vdsp(void);
void AcVlynq_Init_SoC_VlynqHi__to__AC498_VlynqLow(void);
void AcVlynq_Init_AC498_VlynqHi__to__AC498_VlynqLow(int Ac498Device);
U32  AcVlynq_WaitForVlynqLinkStatus(void);
void vlynq_delay_wait(unsigned int count);
void AcVlynq_MapToAc498LocalMemory(void);
void AcVlynq_MapToAc498Registers(void);


/* Packet recording prototypes */
extern void 	Ac49xInitializePacketRecording(TFlushPacketFunction FlushPacket);
int 			dspDev_PRSendoSocket(char* buff, int len);


/******************************/
/***** Enumerators Definitions *****/
/******************************/
/* this is the list of the commands to the dsp driver */
typedef enum {

	DOWNLOAD_CMD_E=1024,
	RESET_BUF_DES_E,
	TRANSMIT_CMD_E,
	RECEIVE_CMD_E,
    RECEIVE_EVENT_CMD_E,
	SETUP_DEVICE_CMD_E,
	CODEC_CNFG_CMD_E,
	ADVANCE_CHANNEL_CNFG_CMD_E,
	OPEN_CHANNEL_CMD_E,
	UPDATE_CHANNEL_CMD_E,
	ACTIVE_REGULAR_CHANNEL_CMD_E,
	UPDATE_REGULAR_CHANNEL_CMD_E,
	INIT_DSP_CMD_E,
	DEFAULTS_DSP_CMD_E,
	SEND_IBS_STRING_CMD_E,
	SEND_IBS_STOP_CMD_E,
	SEND_EXTENDED_IBS_STRING_CMD_E,
	GET_CHANNEL_CMD_E,
	GET_DEVICE_CMD_E,
	SET_CHANNEL_CMD_E,
	SET_DEVICE_CMD_E,
	SET_3WAYCONF_CMD_E,
	SET_CODEC_CMD_E,
	CLOSE_CHANNEL_CMD_E,
	MEM_MODIFY_CMD_E,
	MEM_DISPLAY_CMD_E,
	MEM_CHECKSUM_CMD_E,
	ECHO_CANCELER_CNFG_CMD_E,
	SET_DSP_DEVICE_CONTROL_REGISTER_CMD_E,
	START_PACKET_RECORDING,//AmiL
	STOP_PACKET_RECORDING,//AmiL
        /*A.L. 061031 T.38*/
	T38_CHANNEL_CNFG_CMD_E,
	ACTIVE_FAX_RELAY_CHANNEL_CMD_E,
	CLOSE_REGULAR_CHANNEL_CMD_E,
    MANIPULATE_TIMESLOTS_CMD_E,
	VLYNQ_VDSP_INIT_CMD_E,
   	SEND_CALLER_ID,
	STOP_CALLER_ID,
} DSP_DRIVER_COMMAND_ENT;

typedef enum {

	RTP_IS_DEACTIVE=0,
	RTP_IS_ACTIVE=1

} ACL_RTP_ACTIVATION;


typedef enum {
	local,
	Remote
} TVlynqLocation;


/* use this list to get the status of each routine */
typedef enum {

	AC494_DSP_ERROR_E = 	-1,
	AC494_DSP_OK_E	=	0,
	AC494_DSP_NO_MODULE_E,
	AC494_DSP_NO_DOWNLOAD_E,
	AC494_DSP_NO_INIT_E,
	AC494_DSP_NO_PACKETS
} AC494_DSP_STATUS_ENT;

/*
typedef enum {

	AC494_CODEC_HANDSET,
	AC494_CODEC_HEADSET,
	AC494_CODEC_MIC,
	AC494_CODEC_SPEAKER,
	AC494_CODEC_LINE

} AC494_CODEC_USER_ENT;
*/
typedef enum {

	AC494_CODEC_INPUT,
	AC494_CODEC_OUTPUT,
	AC494_CODEC_LOOP_ON,
	AC494_CODEC_LOOP_OFF

} AC494_CODEC_DIRECTION_ENT;


/******************************/
/****** Structures Definitions ******/
/******************************/
#define SIZE_OF_FLAG	4 /* sizeof(unsigned int) */

#ifdef __KERNEL__

typedef struct {

	int last_digit_pressed;

} ac494_dev_t;

typedef struct dspAttr_stc {

	Tac49xSetupChannelAttr  SetupChannelAttr[MAX_NUM_OF_CHANNELS_PER_DSP];  /* the channels status */
	Tac49xSetupDeviceAttr	SetupDeviceAttr; 		/* the device status */
	int					    downloadStatus;  		/* download flag, 0=no download */
	int					    initStatus;                       /* init flag, 0=no init */
	int 					dspCashSegment;   	/* derived from the dinamic allocation for the dsp memory */
	int					    numOfChannels;

} DSP_ATTR_STCT;


/* define the driver structure holding the dsp device and channels status */
typedef struct dsp_dev_stc {

//kernel 2.6
#if/*def*/ 0/*CONFIG_DEVFS_FS*/
        devfs_handle_t  handle;
#else
        int major;
#endif
    DSP_ATTR_STCT			dspDev[AC49X_NUMBER_OF_DEVICES];
  	int						get_media_cond;
  	int						get_event_cond;
  	wait_queue_head_t		get_media_wait;
  	wait_queue_head_t		get_event_wait;
    struct timer_list 		timer;
  	spinlock_t 				lock;

} DSP_DEV;

typedef struct
{
	int 	device;
	int 	channel;
} dspChannel;


typedef struct {
    Tac49xCallerIdMessageTitle 	Title;
    Tac49xCallerIdSegment 		Month[2];
    Tac49xCallerIdSegment 		Day[2];
    Tac49xCallerIdSegment 		Hour[2];
    Tac49xCallerIdSegment 		Minute[2];
} Tac49xCallerIdTimeAndDay;

typedef struct {
    Tac49xCallerIdSegment 		Checksum;
    Tac49xCallerIdSegment 		End;
} Tac49xCallerIdPayloadFooter;


typedef struct
{
    int code;
    int val;
} code2code_t;


typedef struct
{
    int           id;
    unsigned int  offset;
    unsigned int  size;
    unsigned char remote;

} REGION_CONFIG_T;

typedef struct
{
    char hw_intr_line;
    int  irq;

} IRQ_CONFIG_T;

#endif /*  __KERNEL__  */

typedef struct {

	unsigned int 					command;
	int 							Channel;
	int							InputGain;
	Tac49xVoiceGain				VoiceOutputGain;

} ACL_DSP_GainConfg_STCT;

typedef struct {

	unsigned int 					command;
	int 							Channel;
	int 							duration;

} ACL_DSP_Ringer_STCT;

typedef struct {

	unsigned int 					command;
	int 							Device;

} AC494_DSP_SetDefaults_STCT;


typedef struct {

	unsigned int 					command;
	int 							Device;
	int 							fileSize;
	char				 			*program;

} AC494_DSP_ProgramDownload_STCT;



typedef struct {

	unsigned int	command;
	char			*params;

} DSP_CHAR_WRITE_COMMAND_STC;

typedef struct {

	unsigned int 					command;
	int 							Device;
	Tac49xSetupDeviceAttr 			SetupDeviceAttr;

} AC494_DSP_SetupDevice_STCT;

typedef struct {

	unsigned int 					command;
	int								Device;
	int 							Channel;
	Tac49xSetupChannelAttr 			SetupChannelAttr;

} AC494_DSP_SetupChannel_STCT;

typedef struct {

	unsigned int 					command;
	int 							Device;
	int 							Channel;
	Tac49xCodecConfigurationAttr	Attr;

} AC494_DSP_CodecConfg_STCT;

typedef struct {

	unsigned int 							command;
	int 									Device;
	int 									Channel;
	Tac49xAdvancedChannelConfigurationAttr	Attr;

} AC494_DSP_AdvanceChannelConfg_STCT;

typedef struct {

	unsigned int 							command;
	int 									Device;
	int 									Channel;
	Tac49xOpenOrUpdateChannelConfigurationAttr Attr;

} AC494_DSP_OpenUpdateChannelConfg_STCT;

typedef struct {

	unsigned int 							command;
	int 									Device;
	int 									Channel;
	Tac49xActivateOrUpdateRtpChannelConfigurationAttr Attr;

} AC494_DSP_ActivateUpdateRegularRtpChannelConfg_STCT;

typedef struct {

	unsigned int 							command;
	int 									Device;
	int 									Channel;
	Tac49xIbsStringAttr 					Attr;

} AC494_DSP_SendIbsString_STCT;

typedef struct {

	unsigned int 							command;
	int 									Device;
	int 									Channel;

} AC494_DSP_SendIbsStop_STCT;

typedef struct {

	unsigned int 							command;
	int 									Device;
	int 									Channel;
	Tac49xExtendedIbsStringAttr				Attr;

} AC494_DSP_SendExtendedIbsString_STCT;

typedef struct {

	unsigned int 					command;
	Tac49xTxPacketParams 			TxPacketParams;

} AC494_DSP_Transmit_STCT;

typedef struct {

	unsigned int 					command;
	Tac49xRxPacketParams 			RxPacketParams;

} AC494_DSP_Receive_STCT;


typedef struct {

	unsigned int 												command;
	int 														Device;
	int 														Channel;
	Tac49xActivateOrDeactivate3WayConferenceConfigurationAttr 	Attr;

} AC494_DSP_ActivateOrDeactivate3WayConf_STCT;

#if defined(CONFIG_SDP_IPPHONE) || defined(CONFIG_ORCHID)
typedef struct {

	unsigned int 				command;
	int							Device;
	int							Channel;
	acIPPCodecType		        user;
	AC494_CODEC_DIRECTION_ENT	direction;

} AC494_DSP_CODEC_DIR_STCT;
#endif

typedef struct {

	unsigned int 							command;
	int 									Device;
	int 									Channel;

} AC494_DSP_CloseChannel_STCT;


typedef struct {

	unsigned int					command;
	unsigned int 					addr;
	unsigned int 					data;
	int								count;

} AC494_MEM_MODIFY_STCT;

typedef struct {

	unsigned int 							command;
	unsigned int							addr;
	int										count;

} AC494_MEM_DISPLAY_STCT;

typedef struct {

	unsigned int 				command;
	unsigned int 				addr1;
	int							count;
	int							checksum;

} AC494_MEM_CHECKSUM_STCT;


typedef struct {

	unsigned int 									command;
	int 											Device;
	int 											Channel;
	Tac49xAcousticEchoCancelerConfigurationAttr		Attr;

} AC494_DSP_AcousticEchoCancelerConfiguration_STCT;

/*A.L. 061031 T.38*/
typedef struct {
	unsigned int						command;
	int							Device;
	int							Channel;
	Tac49xT38ConfigurationAttr				Attr;
} AC494_DSP_T38ChannelConfiguration_STCT;

typedef struct {
	unsigned int						command;
	int							Device;
	int							Channel;
	Tac49xActivateFaxRelayAttr				Attr;
} AC494_DSP_ActivateUpdateFaxRelayChannelConfg_STCT;

typedef struct {
	unsigned int 						command;
	int 							Device;
	int 							Channel;
	Tac49xCloseRtpChannelConfigurationAttr			Attr;
} AC494_DSP_CloseRegularRtpChannelConfg_STCT;

typedef struct {

	unsigned int 									command;
	int 											Device;
	int 											Channel;
	Tac49xTimeSlotManipulationCommandAttr 			Attr;

} AC494_DSP_TimeSlotsManipulation_STCT;

typedef struct {

	unsigned int 							command;
    int 									Device;
	int 									Channel;
	Tac49xCallerIdAttr      				CallerIdMessage;

} AC494_DSP_CallerId_STCT;

typedef struct {

	unsigned int 							command;
	int 									Device;
	int 									Channel;
	unsigned int 							IPAddr;
       unsigned short                                          Port;
} AC494_DSP_PacketRecordingStartStop_STCT;

typedef struct {

	unsigned int 									command;

} AC494_DSP_GENERAL_CMD_STCT;

#endif /*__DSP_DRV_h*/

