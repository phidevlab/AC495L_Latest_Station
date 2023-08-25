/****************************************************************************
*
*   Company:			Audiocodes Ltd.
*
*   Project:			VoPP SoC
*
*   Hardware Module: 	AC494-EVM
*
*   File Name: 			dsp_drv.c
*
*
******************************************************************************
*
* 	DESCRIPTION:		This file wrap the AC49X driver to a char device
*
******************************************************************************/



#include <linux/config.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/ioport.h>
#include <linux/fcntl.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/proc_fs.h>
#include <asm/io.h>
#include <asm/uaccess.h>
//#include <asm/system.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/version.h> /* For LINUX_VERSION_CODE and KERNEL_VERSION */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,5,0)
#include <linux/devfs_fs_kernel.h>
#endif
#include <linux/spinlock.h>
#include <linux/vmalloc.h>

//#include <linux/wait.h>
//Linux 2.6 only
#include <linux/cdev.h>


#include <pformCfg.h>
#include <avalanche_intc.h>

#include "AC49xDrv_Drv.h"
#include "dsp_drv.h"

#include <linux/in.h>
#include <net/sock.h>
#include <linux/inet.h>

/********************************************************************************************************************/
/********************************************************************************************************************/

#include <pal.h>

#include <pal_vlynq.h>
#include <pal_vlynqIoctl.h>


#if defined(CONFIG_SDP_IPPHONE) || defined(CONFIG_ORCHID)
#include "acIPPDriversDefs.h"
#endif


MODULE_DESCRIPTION("AC49x dsp kernel module");
MODULE_AUTHOR("AudioCodes");
MODULE_LICENSE("GPL");

#define AVALANCHE_LOW_VLYNQ_RESET_BIT             15
#define AVALANCHE_HIGH_VLYNQ_RESET_BIT            16
#define VLYNQ0_RESET_GPIO_NUM                       0x7

static IRQ_CONFIG_T irq_config1[] = { { 0, 0},
                                      { 1, 1},
                                      { 2, 2},
                                      { 3, 3},
                                      { 4, 4},
                                      { 5, 5},
                                      { 6, 6},
                                      { 7, 7},
                                      {-1, 0}
                                    };
static IRQ_CONFIG_T irq_config2[] = { { 0, 8},
                                      { 1, 9},
                                      { 2, 10},
                                      { 3, 11},
                                      { 4, 12},
                                      { 5, 13},
                                      { 6, 14},
                                      { 7, 15},
                                      {-1, 0 }
                                    };
static IRQ_CONFIG_T irq_config3[] = { { 0, 16},
                                      { 1, 17},
                                      { 2, 18},
                                      { 3, 19},
                                      { 4, 20},
                                      { 5, 21},
                                      { 6, 22},
                                      { 7, 23},
                                      {-1, 0 }
                                    };
static IRQ_CONFIG_T irq_config4[] = { { 0, 24},
                                      { 1, 25},
                                      { 2, 26},
                                      { 3, 27},
                                      { 4, 28},
                                      { 5, 29},
                                      { 6, 30},
                                      { 7, 31},
                                      {-1, 0 }
                                    };


static REGION_CONFIG_T region_config_AC498_1st_LowRxMap_ForHpi[]
                                                         = {
                                                                { 0, AC498_HPI_BASE_ADDRESS,                                AC498_VLYNQ_BLOCK_SIZE,     Remote},
                                                                { 1, AC498_VLYNQ_ADDRESS_SPACE_HI,                          AC498_VLYNQ_BLOCK_SIZE,     Remote},
                                                                { 2, AC498_VLYNQ_ADDRESS_SPACE_HI+AC498_VLYNQ_BLOCK_SIZE,   AC498_VLYNQ_BLOCK_SIZE,     Remote},
                                                                {-1, 0, 0, 0}
                                                           };
static REGION_CONFIG_T region_config_AC498_2nd_LowRxMap_ForHpi[]
                                                         = {
                                                                { 0, AC498_HPI_BASE_ADDRESS,                                AC498_VLYNQ_BLOCK_SIZE,     Remote},
                                                                { 1, AC498_VLYNQ_ADDRESS_SPACE_HI,                          AC498_VLYNQ_BLOCK_SIZE,     Remote},

                                                                {-1, 0, 0, 0}
                                                           };
static REGION_CONFIG_T region_config_AC498_3rd_LowRxMap_ForHpi[]
                                                         = {
                                                                { 0, AC498_HPI_BASE_ADDRESS,                                AC498_VLYNQ_BLOCK_SIZE,     Remote},
                                                                {-1, 0, 0, 0}
                                                           };


/*********************************************************************************/
static REGION_CONFIG_T region_config_AC498_1st_LowRxMap_ForRegisters[]
                                                         = {
                                                                { 0, offsetof(Tac498MemoryMap, McBSP0[0]),                  AC498_VLYNQ_BLOCK_SIZE, Remote},
                                                                { 1, AC498_VLYNQ_ADDRESS_SPACE_HI,                          AC498_VLYNQ_BLOCK_SIZE, Remote},
                                                                { 2, AC498_VLYNQ_ADDRESS_SPACE_HI+AC498_VLYNQ_BLOCK_SIZE,   AC498_VLYNQ_BLOCK_SIZE, Remote},
                                                                {-1, 0, 0, 0}
                                                           };
static REGION_CONFIG_T region_config_AC498_2nd_LowRxMap_ForRegisters[]
                                                         = {
                                                                { 0, offsetof(Tac498MemoryMap, McBSP0[0]),                  AC498_VLYNQ_BLOCK_SIZE, Remote},
                                                                { 1, AC498_VLYNQ_ADDRESS_SPACE_HI,                          AC498_VLYNQ_BLOCK_SIZE, Remote},
                                                                {-1, 0, 0, 0}
                                                           };
static REGION_CONFIG_T region_config_AC498_3rd_LowRxMap_ForRegisters[]
                                                         = {
                                                                { 0, offsetof(Tac498MemoryMap, McBSP0[0]),                  AC498_VLYNQ_BLOCK_SIZE, Remote},
                                                                {-1, 0, 0, 0}
                                                           };



// To maintain the reference of vlynq configuration across wlan driver load and
// unload. Not an ideal solution but still cleanup for this release.
static PAL_VLYNQ_CONFIG_T  vlynq_config;
static PAL_VLYNQ_DEV_HND   *p_vlynq_dev[3] = {NULL,NULL,NULL};
static PAL_VLYNQ_DEV_HND 	*p_root_vlynq = NULL;
static PAL_VLYNQ_HND       *p_vlynq[4]     = {NULL,NULL,NULL,NULL};


U32  Ac498BaseAddress[AC49X_NUMBER_OF_DEVICES-1] = {AC498_BASE_ADRESS,
                                                    		AC498_BASE_ADRESS+AC498_VLYNQ_BLOCK_SIZE,
                                                    		AC498_BASE_ADRESS+2*AC498_VLYNQ_BLOCK_SIZE
                                                   			};

/****** Global definitions in order not to allocate it on the stack ******/
static char			recordingRemoteIpAddress[ADDRESS_STR_MAX_LEN];
static int				recordingRemotePort;
static Tac49xControl	packetRecordingStatus;
static Tac49xControl	networkRecordingStatus;
static Tac49xControl	tdmRecordingStatus;


/* char device write() related global definitions */
static Tac49xTxPacketParams 									TxPacketParams;
static Tac49xIbsStringAttr  									IbsStringAttr;
static Tac49xExtendedIbsStringAttr  							extendedIbsStringAttr;
//static Tac49xExtendedIbsString_Format1_Attr 					extendedIbsStringAttr_Format1;
static Tac49xActivateOrDeactivate3WayConferenceConfigurationAttr 	_3WayConferenceAttr;
static Tac49xCloseRtpChannelConfigurationAttr					closeRtpChannelConfigurationAttr;
static Tac49xT38ConfigurationAttr								T38ConfigurationAttr;
static Tac49xActivateFaxRelayAttr							ActivateFaxRelayAttr;
static char 					getEventBuffer[AC494_DSP_RECEIVER_PACKET_SIZE];
static char 					getMediaBuffer[AC494_DSP_RECEIVER_PACKET_SIZE];

/* char device read() related global definitions */
static Tac49xRxPacketParams 		RxEventPacketParams;
static Tac49xRxPacketParams 		RxMediaPacketParams;


static ac494_dev_t				dspChannelsAttributes[NUM_OF_DSP_CHANNELS];

int isActivateRTP[AC49X_NUMBER_OF_DEVICES][AC49X_NUMBER_OF_CHANNELS];

char dev_name_AC498_0[] = "AC498_0";
char dev_name_AC498_1[] = "AC498_1";
char dev_name_AC498_2[] = "AC498_2";

static unsigned int uilocal_ipPR=0x0A10020A;//10.16.2.10
static u16 portPR=50000;

static unsigned int phi_ssrc;

code2code_t ptimes[] =
{
	{10, PCM_ADPCM_FRAME_DURATION__10_MSEC},
	{20, PCM_ADPCM_FRAME_DURATION__20_MSEC},
/* add before this record*/
	{-1, -1},
};
code2code_t ptime_to_encoder_frame_duration_711_726[] =
{
	{10, PCM_ADPCM_FRAME_DURATION__10_MSEC},
	{20, PCM_ADPCM_FRAME_DURATION__20_MSEC},
	{30, PCM_ADPCM_FRAME_DURATION__10_MSEC},
/* add before this record*/
	{-1, -1},
};

code2code_t ptime_to_decoder_frame_duration_711_726[] =
{
	{10, PCM_ADPCM_FRAME_DURATION__10_MSEC},
	{20, PCM_ADPCM_FRAME_DURATION__10_MSEC},
	{30, PCM_ADPCM_FRAME_DURATION__10_MSEC},
/* add before this record*/
	{-1, -1},
};
code2code_t ptime_to_voice_blocks_711_726[] =
{
	{10, 0},
	{20, 0},
	{30, 2},
/* add before this record*/
	{-1, -1},
};

code2code_t ptime_to_voice_blocks_729[] =
{
	{10, 0},
	{20, 1},
	{30, 2},
/* add before this record*/
	{-1, -1},
};

code2code_t ptime_to_voice_blocks_723[] =
{
	{10, 0},
	{20, 0},
	{30, 0},
	{60, 1},
/* add before this record*/
	{-1, -1},
};

code2code_t ptime_to_voice_blocks_722[] =
{
	{10, 0},
	{20, 1},
	{30, 2},
/* add before this record*/
	{-1, -1},
};

code2code_t EchoCancelerNonLinearProcessor[] =
{
	{ACG_ECHO_CANCELER_NON_LINEAR_PROCESSOR_MODE__DISABLE, ECHO_CANCELER_NON_LINEAR_PROCESSOR_MODE__DISABLE},
	{ACG_ECHO_CANCELER_NON_LINEAR_PROCESSOR_MODE__ENABLE, ECHO_CANCELER_NON_LINEAR_PROCESSOR_MODE__MEDIUM_SENSITIVITY},
/* add before this record*/
	{-1, -1},
};

static int kcode2code(code2code_t *list, int code)
{
    for (; list->code != -1 && list->code != code; list++);
    	return list->val;
}
/*end of supporting configurable packetization time*/

/********************************************************************************************************************/
/********************************************************************************************************************/



extern void Ac49xSetSegB(int segB);

int dspDev_setDefaults(int Device);

int setDeviceParams(int Device, int NumberOfCallProgressTones,
		acgTCallProgressTone *pCallProgressTones, Tac49xSetupDeviceAttr *pSetupDeviceAttr);
int startEC(Tac49xSetupChannelAttr *pSetupChannelAttr);
int CodecConfig(int Device, int Channel, acgTChannelParameters *pChannelParameters);
int AdvanceChannelConfig(int Device, int Channel);
int OpenUpdateChannelConfig(int Device, int Channel, acgTChannelParameters *pChannelParameters);
int ActivateUpdateRegularRtpChannelConfig(Tac49xActivateOrUpdateRtpChannelConfigurationAttr *rtpInfo, acgTChannelParameters *pChannelParameters);

#if defined(CONFIG_SDP_IPPHONE) || defined(CONFIG_ORCHID)
void setCodecAEC(Tac49xSetupChannelAttr *pSetupChannelAttr,  acIPPCodecType codecType);
int setCodecParams(int Device, int Channel, acIPPCodecType codecType);
#endif
void set3wcParams(Tac49xActivateOrDeactivate3WayConferenceConfigurationAttr *p3WayConferenceAttr,  int Channel, int Mode);
int sendCallerIdMessage(int Device, int Channel, acgTCallerId *pCallerId);
int sendMWIMessage(int Device, int Channel, acgTMWI *pMWI);
void setToneParams(acgTTone *pTone, Tac49xIbsStringAttr *pIbsStringAttr);
void setToneSequenceParams(acgTToneSequence *pToneSequence, Tac49xIbsStringAttr *pIbsStringAttr);
void setToneExtendedParams(acgTToneExtended *pToneExtended, Tac49xExtendedIbsStringAttr *pIbsExtendedStringAttr);
void setRingerParams(int duration, Tac49xExtendedIbsStringAttr  *pExtendedIbsStringAttr);

static void InitPacketRecordingInfo(char *pRemoteRecordingIpAddress, int remoteRecordingPort);
static void SetPacketRecordingStatus(Tac49xControl status);
static void SetNetworkRecordingStatus(Tac49xControl status);
static void SetTDMRecordingStatus(Tac49xControl status);
static int IsRecordingEnable(Tac49xProtocol protocol);
static short rand( void );

int Translate49xDspEventsToAcgEvents(acgTEventInfo *pDspEvent, Tac49xRxPacketParams *pRxPacketParams, Tac49xPacket *pBufEvetStatu);

void ACL_sysGpioFuncSet(int pinSelReg,   int shift,     int func);


static Tac49xPacket		Packet;
static char			MediaTempBuff[MAX_MEDIA_PACKET_SIZE];
static char			*ManipulateTimeslotsTempBuff = NULL;

/*extern void ac494dspStart(void);*/


dspChannel AbsCh2DspConversionTable[] =
{
/*
Absolute channel  Device    Channel
----------------  --------  --------
*/
/*     0      */    {0,			0},
/*     1      */	{0,			1},
/*     2      */	{0,         2},
/*     3      */	{0,			3},
/*     4      */	{1,         0},
/*     5      */	{1,			1},
/*     6      */	{1,         2},
/*     7      */	{1,			3},
/*     8      */	{2,         0},
/*     9      */	{2,			1},
/*     10     */	{2,         2},
/*     11     */	{2,			3},
/*     12     */	{3,         0},
/*     13     */	{3,		 	1},
/*     14     */	{3,         2},
/*     15     */	{3,			3},
};

#define getAbsChannelByDspChannel(device,channel) ( ((channel)<0) ? (-1) : ((device)*AC49X_NUMBER_OF_CHANNELS+(channel)) )
#define getDspDeviceByAbsChannel(AbsCh) (AbsCh/AC49X_NUMBER_OF_CHANNELS)
#define getDspChByAbsChannel(AbsCh) (AbsCh%AC49X_NUMBER_OF_CHANNELS)



#if/*def*/ 0/*CONFIG_DEVFS_FS*/
static devfs_handle_t devfs_dir_handle = NULL;
#endif
static DSP_DEV  *dsp_dev = NULL;
static struct cdev dsp_cdev;



static unsigned int baseAddr_dspDev;
static int			size_dspDev;


struct socket   *pDspPRsock;


/* Function Implementation */

void hexDump(char *pStart, int bytesToPrint)
{
	int i;
	char* temp = pStart;
	printk("\n");
	for(i=0; i<bytesToPrint; i++)
	{
		printk("%02X", *(temp));
		temp++;
		if(i%4==0)
			printk(" ");
//		if(i%40==0)
//			printk("\r\n ");
	}
}

/*returns 0 for success and -1 for failure == given channel is not in range*/
int getDspChannelByAbsChannel(int AbsCh, dspChannel *dspCh)
{
	if ((0 > AbsCh) || (MAX_CID_NUM < AbsCh))
		return -1;

    dspCh->device = getDspDeviceByAbsChannel(AbsCh);
	dspCh->channel = getDspChByAbsChannel(AbsCh);

	return 0;
}

void FlushPacketToNet(int Device, char *_pPacket, U32 PacketSize, Tac49xPacketDirection PacketDirection)
{

	static Tac49xAnalysisPacket              AnalysisPacket;
	int        BytesSent;
	static U32 TimeStamp = 0;
	static U16 Sequence  = 0;
	static Tac49xPacket *Packet;
	static U16 iRTPPacketsCounter=80;

	Packet = (Tac49xPacket*)_pPacket;

	if(!IsRecordingEnable(Packet->HpiHeader.Protocol))	/* don't send recorded packets if disable */
		return;
#if 0
	if (Packet->HpiHeader.Protocol == PROTOCOL__RTP)
	{
		if (iRTPPacketsCounter--)
			return;
		else
			iRTPPacketsCounter=80;
	}
#endif
	memset((void*)&AnalysisPacket.Header, 0, sizeof(Tac49xAnalysisPacketHeader));
	AnalysisPacket.Header.Device     = Device;
	AnalysisPacket.Header.Direction  = PacketDirection;

        AnalysisPacket.Header.Ver_Msb        = AC49X_DRIVER_VERSION_540;
	AnalysisPacket.Header.Ver_Lsb        = 6;

	AnalysisPacket.Header.SubVersion = 4;
	SplitFieldsFromShort(AnalysisPacket.Header.SequenceNumber,     Sequence);
	SplitFieldsFromLong( AnalysisPacket.Header.TimeStamp,      TimeStamp);

	memcpy((void*)AnalysisPacket.Payload, (void*)_pPacket, PacketSize);

	BytesSent = dspDev_PRSendoSocket((char *)&AnalysisPacket, PacketSize+sizeof(Tac49xAnalysisPacketHeader));
	//BytesSent = write(ac494_dev->flushPacketSocket, (char *)&AnalysisPacket, PacketSize+sizeof(Tac49xAnalysisPacketHeader));

	TimeStamp+=1;  /* TBD */
	Sequence+=1;
}

/****************************************************************************
*
*	Name:		dspDev_read
*----------------------------------------------------------------------------
*	Abstract:	read routine from the device
*----------------------------------------------------------------------------
*	Input:		file 	- file decriptor
*				buf		- buf to fill
*				count	- size of buf
*				ppos	- flags
*----------------------------------------------------------------------------
*  	Output:		buf	- read data
*----------------------------------------------------------------------------
*	Returns: 	success = AC494_DSP_OK_E, error = AC494_DSP_ERROR_E,
*				in case of GET_MEDIA_CMD_E check Ac49xReceivePacket() return value
******************************************************************************/
static ssize_t dspDev_read( struct file * file, char * buf, size_t count, loff_t *ppos )
{

	int 							ret = AC494_DSP_ERROR_E;
	unsigned int 					flag;

	acgTCommand					*pAcgCmd=NULL;
	acgTGetMedia 					*pGetMedia=NULL;
	acgTGetMedia 					GetMedia;
	acgEPollingMethod 				PollingMethod;
	acgTEventInfo 					DspEvent;

	acgTGetEventInfo				*pGetEventInfo = NULL;

	Tac49xPacket					bufEvetStatus;

	AC494_DSP_SetupChannel_STCT		*pSetupChannel=NULL;
	AC494_DSP_SetupDevice_STCT		*pSetupDevice=NULL;
	AC494_DSP_Receive_STCT			*pReceive=NULL;
	AC494_DSP_Receive_STCT			Receive;

    /* CR note - large allocation on stack... */
	char 							buffer[AC494_DSP_RECEIVER_PACKET_SIZE];
	char 							*user_pInPacket=NULL;
	int								absCh = 0;
	int								Device = 0;
	int								Channel = 0;
	dspChannel                      dspCh;
	int gpio;
	flag = *(unsigned int *)buf;	/* get the command flag and switch to the command */

	switch(flag) {

		/* get media command - read the media packets from the dsp */

		case ACG_GET_MEDIA_CMD_E:

			/* cast the pointer */
			pAcgCmd = (acgTCommand *)buf;

			pGetMedia = (acgTGetMedia *)pAcgCmd->data;


			user_pInPacket = pGetMedia->Buff; 	/* save the user buffer pointer */
 			PollingMethod = pGetMedia->PollingMethod;  /* get PollingMethod */



			/*
				the purpose of this loop is to read from all the devices before "going to sleep"
				if you checked all devices and still don't have anything to read then sleep.
			*/
receive_packet:
			RxMediaPacketParams.pInPacket = getMediaBuffer;    /* point to the local buffer */
			RxMediaPacketParams.Device = 0;
			RxMediaPacketParams.TransferMedium = TRANSFER_MEDIUM__NETWORK;

	            	do {
		                ret = Ac49xReceivePacket(&RxMediaPacketParams);	/* get the dsp buffer */
		                if (0 == ret) /* success*/
		                    break;

		                if ( ret != BUFFER_DESCRIPTOR_EMPTY_ERROR)
		                	printk("DSP device %d Ac49xReceivePacket error 0x%x.\n", RxMediaPacketParams.Device, ret);

		                RxMediaPacketParams.Device = ((RxMediaPacketParams.Device+1) % AC49X_NUMBER_OF_DEVICES);

	            	} while(RxMediaPacketParams.Device!=0);

			if(0 == ret)
			{
				absCh = getAbsChannelByDspChannel(RxMediaPacketParams.Device, RxMediaPacketParams.Channel);
				RxMediaPacketParams.Channel = absCh;	/* CR Note - Channel changed from DSP-channel to abs channel */

				/* copy the received packet to user */
				if(copy_to_user(user_pInPacket, getMediaBuffer + HPI_HEADER_LEN, RxMediaPacketParams.PayloadSize))
				{
				    ret = -EFAULT;
				}
			}
			else if(ret == BUFFER_DESCRIPTOR_EMPTY_ERROR)
	            	{

				if(PollingMethod==ACG_POLLING_METHOD__BLOCKING) {
					dsp_dev->get_media_cond = 0;
		               	wait_event_interruptible(dsp_dev->get_media_wait, dsp_dev->get_media_cond != 0);

					if(dsp_dev->get_media_cond != 0) {
						goto receive_packet;
					}
					else{
				                ret = ACG_NO_PACKET;
						break;
					}
				}
				else{
                                        ret = ACG_NO_PACKET;
					break;
				}
	            	}

			GetMedia.Channel 		= RxMediaPacketParams.Channel;
			GetMedia.Buff 			= user_pInPacket;
			GetMedia.Len 			= RxMediaPacketParams.PayloadSize;
			GetMedia.MediaProtocol 	= RxMediaPacketParams.Protocol;


			/* copy the receive structure back to the user space */
			if(copy_to_user(pAcgCmd->data , &GetMedia, sizeof(acgTGetMedia)))
			{
				ret = -EFAULT;
			}

			/*ret = AC494_DSP_OK_E;*/
			break;


		/* get event command - read the events from the dsp */

		case ACG_DSP_GET_EVENT_CMD:

			/* cast the pointer */
			pAcgCmd = (acgTCommand *)buf;

			pGetEventInfo = (acgTGetEventInfo *)pAcgCmd->data;

			PollingMethod = pGetEventInfo->PollingMethod;

			memset(&DspEvent, 0, sizeof(DspEvent));
			memset(&RxEventPacketParams, 0, sizeof(Tac49xRxPacketParams));


receive_event_packet:

			DspEvent.Channel = -1;
			DspEvent.EventType = ACG_EVENT_TYPE__NO_EVENT;

			RxEventPacketParams.pInPacket = getEventBuffer;         /* point to the local buffer */

			/*
				the purpose of this loop is to read from all the devices before "going to sleep"
				if you checked all devices and still don't have anything to read then sleep.
			*/

			RxEventPacketParams.TransferMedium = TRANSFER_MEDIUM__HOST;

			RxEventPacketParams.pInPacket = (char *)&bufEvetStatus;

	            	do {

		                ret = Ac49xReceivePacket(&RxEventPacketParams);	/* get the dsp buffer */

		                if (0 == ret) /* success*/
		                    break;

		                if ( ret != BUFFER_DESCRIPTOR_EMPTY_ERROR)
                    			printk("DSP device %d Ac49xReceivePacket error 0x%x.\n", RxEventPacketParams.Device, ret);

				RxEventPacketParams.Device = ((RxEventPacketParams.Device+1) % AC49X_NUMBER_OF_DEVICES);

	            	} while(RxEventPacketParams.Device != 0);

	           	if(0 == ret)
            	{
					ret = Translate49xDspEventsToAcgEvents(&DspEvent, &RxEventPacketParams, &bufEvetStatus);
					if (-1 == ret)
						break;

            	}
				else if(ret == BUFFER_DESCRIPTOR_EMPTY_ERROR)
            	{
					if(PollingMethod==ACG_POLLING_METHOD__BLOCKING)	{
		                dsp_dev->get_event_cond = 0;
		                wait_event_interruptible(dsp_dev->get_event_wait, dsp_dev->get_event_cond != 0);


						if(dsp_dev->get_event_cond != 0)
						{
							goto receive_event_packet;
						}
						else
						{
							ret = ACG_NO_PACKET;
							break;
						}
					}
					else
					{
						ret = ACG_NO_PACKET;
						break;
					}
            	}

			/* copy the event information to the user space */
			if(copy_to_user(pGetEventInfo->Buff, &DspEvent, sizeof(acgTEventInfo)))
			{
				ret = -EFAULT;
			}

			break;

		/* get channel command - returns the channel status */

		case GET_CHANNEL_CMD_E:

			pSetupChannel = (AC494_DSP_SetupChannel_STCT *)buf;   /* cast the pointer */
			absCh = pSetupChannel->Channel;                     /* copy the channel data */

			/*convert the absolute channel to the appropriate dsp device and channel*/
			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;
			Device = dspCh.device;
			Channel = dspCh.channel;

			if(copy_to_user(&(pSetupChannel->SetupChannelAttr),(char *)&(dsp_dev->dspDev[Device].SetupChannelAttr[Channel]),sizeof(Tac49xSetupChannelAttr))) {
				ret = -EFAULT;
			}
			else   {
				ret = AC494_DSP_OK_E;
			}
			break;

		case GET_DEVICE_CMD_E:

			pSetupDevice = (AC494_DSP_SetupDevice_STCT *)buf;   	/* cast the pointer */
			Device = pSetupDevice->Device;

			if(copy_to_user(&(pSetupDevice->SetupDeviceAttr),(char *)&(dsp_dev->dspDev[Device].SetupDeviceAttr),sizeof(Tac49xSetupDeviceAttr))) {
				ret = -EFAULT;
			}
			else {
				ret = AC494_DSP_OK_E;
			}
			break;


		/* get receive command - read the buffers from the dsp */

		case RECEIVE_CMD_E:
		case RECEIVE_EVENT_CMD_E:

			pReceive = (AC494_DSP_Receive_STCT *)buf;    		/* cast the pointer */
			/* CR Note - copy_from_user is not needed - can use pReceive->RxPacketParams directly */
			if(copy_from_user(	&(Receive.RxPacketParams),      /* copy the receive structure to kernel */
								&(pReceive->RxPacketParams),
								sizeof(Tac49xRxPacketParams))) {
				ret = -EFAULT;
				break;
			}
			user_pInPacket = Receive.RxPacketParams.pInPacket; /* save the user buffer pointer */
			Receive.RxPacketParams.pInPacket = buffer;         /* point to the local buffer */

			/*
				the pupose of this loop is to read from all 4 devices before "going to sleep"
				if you checked all devices and still dont ahv anything to read then sleep.
			*/

            do {

		if(dsp_dev->dspDev[Receive.RxPacketParams.Device].downloadStatus)
			ret = Ac49xReceivePacket(&(Receive.RxPacketParams));	/* get the dsp buffer */
		else
			ret = BUFFER_DESCRIPTOR_EMPTY_ERROR;

                if (0 == ret) /* success*/
                    break;

                if ( ret != BUFFER_DESCRIPTOR_EMPTY_ERROR)
                    printk("DSP device %d Ac49xReceivePacket error 0x%x.\n", Receive.RxPacketParams.Device, ret);

                Receive.RxPacketParams.Device = ((Receive.RxPacketParams.Device+1) % AC49X_NUMBER_OF_DEVICES);

            } while(Receive.RxPacketParams.Device!=0);

            if(0 == ret)
            {
                absCh = getAbsChannelByDspChannel(Receive.RxPacketParams.Device, Receive.RxPacketParams.Channel);
                Receive.RxPacketParams.Channel = absCh;	/* CR Note - Channel changed from DSP-channel to abs channel */

                /* copy the received packet to user */
                if(copy_to_user(user_pInPacket,buffer,Receive.RxPacketParams.PacketSize))
                {
                    ret = -EFAULT;
                }
            }
			else if(ret == BUFFER_DESCRIPTOR_EMPTY_ERROR)
            {
                if (flag == RECEIVE_CMD_E)
                {
                    dsp_dev->get_media_cond = 0;
                    wait_event_interruptible(dsp_dev->get_media_wait, dsp_dev->get_media_cond != 0);
                }
                if (flag == RECEIVE_EVENT_CMD_E)
                {
                    dsp_dev->get_event_cond  = 0;
                    wait_event_interruptible(dsp_dev->get_event_wait, dsp_dev->get_event_cond  != 0);
                }

                Receive.RxPacketParams.Channel = -1;
                ret = AC494_DSP_NO_PACKETS;
            }

			/* point back to the user buffer */
			Receive.RxPacketParams.pInPacket = user_pInPacket;
            /* copy the receive structure back to the user space */
			if(copy_to_user(&(pReceive->RxPacketParams),&(Receive.RxPacketParams),sizeof(Tac49xRxPacketParams)))
			{
				ret = -EFAULT;
			}
			/*ret = AC494_DSP_OK_E;*/
			break;


			case ACG_STOP_FAX_RELAY_CMD_E:

             pAcgCmd = (acgTCommand *)buf;
             gpio = pAcgCmd->Channel;
            // printk("IN GPIO read \n");
             switch(gpio)
             {
             case 0:  ret=PAL_sysGpioInBit(0); break;
             case 1:  ret=PAL_sysGpioInBit(1); break;
             case 29: ret=PAL_sysGpioInBit(29); break;
             case 31: ret=PAL_sysGpioInBit(31); break;
             case 32: ret=PAL_sysGpioInBit(32); break;
             case 34: ret=PAL_sysGpioInBit(34); break;
            }
    break;


   		/* get receive event command - read the buffers from the dsp */
		default:

			ret = AC494_DSP_ERROR_E;
			break;

	}

	return ret;

} /* end of dspDev_read() */
/******************************************************************************/


void ClearDownloadStatus(void)
{
	int Device;
	for(Device = 0 ; Device < AC49X_NUMBER_OF_DEVICES ; Device++) {
					dsp_dev->dspDev[Device].downloadStatus = 0;
					dsp_dev->dspDev[Device].initStatus = 0;
	}

}
/****************************************************************************
*
*	Name:		dspDev_write
*----------------------------------------------------------------------------
*	Abstract:	write routine to the device
*----------------------------------------------------------------------------
*	Input:		file 	- file decriptor
*				buf		- buf to fill
*				count	- size of buf
*				ppos	- flags
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	case of error = AC494_DSP_ERROR_E
******************************************************************************/
static ssize_t dspDev_write(struct file *file, const char *buf, size_t count, loff_t *ppos)
{
	char 			*temp_buf;
	int 			order;
    unsigned char 		*p494temp_buf=NULL;
	unsigned char 		*p498temp_buf=NULL;
	int 			ret= AC494_DSP_ERROR_E;
	unsigned int 	flag;
	int				absCh = 0;
	int				Device = 0;
	int				NumberOf498DspDevices = 0;
	int                         deviceCnt = 0;
	int                         deviceNum = 0;
	int				Channel = 0;
	dspChannel      dspCh;

#if defined(CONFIG_SMB) || defined(CONFIG_ORCHID_ATA)
    /* for 16 channels version the order should be 0,3,2,1*/
    int                         devOrder[] = {0, 3, 2, 1};
#elif defined 	CONFIG_TULIP2_40
    int 			devOrder[] = {0, 1};
#else
    int                         devOrder[] = {0};
#endif

	/* the next structures use to wrap the AC49X routines */
	acgTCommand						*pAcgCmd=NULL;

	acgECoder     playbackCoder;

	static	acgTInitializationParameters 	InitializationParams;
	acgTInitializationParameters 			*pInitializationParams=NULL;

	acgTChannelParameters				*pChannelParams=NULL;
	acgTChannelParameters				ChannelParams;

	acgTTone 						*pTone=NULL;
	acgTTone 						Tone;

	acgTToneSequence 					*pToneSequence=NULL;
	acgTToneSequence 					ToneSequence;

	acgTToneExtended 					*pToneExtended=NULL;
	acgTToneExtended 					ToneExtended;

	acgTMWI 							*pMWI=NULL;
	acgTMWI 							MWI;

	acgTCallerId 						*pCallerId=NULL;
	acgTCallerId 						CallerId;

	acgT3WayConference 				*p3WayConference=NULL;
	acgT3WayConference 				_3WayConference;

	acgTDebugInformation 				*pDebugInformation;
	acgTDebugInformation 				DebugInformation;

	acgTDecodeMedia 					*pDecodeMedia=NULL;

	static acgTPlaybackData             PlaybackVoice;
    acgTPlaybackData                    *pPlaybackVoice;
    int iSilence;

	static acgTTimeSlotManipulationAttr TimeSlotManipulation;
    acgTTimeSlotManipulationAttr        *pTimeSlotManipulation;

	int 								NumberOfCallProgressTones;	/* Number of defined call progress tones */

//	ACL_DSP_GainConfg_STCT								gainConfig;
//	ACL_DSP_Ringer_STCT									ringer;

#if defined(CONFIG_SDP_IPPHONE) || defined(CONFIG_ORCHID)
	acIPPCodecType 					IPPCodecType;
	acgEVoiceGain					IPPOutputGain;
	acgEVoiceGain					IPPInputGain;
#endif

	AC494_DSP_SetupDevice_STCT								*pSetupDevice;
	AC494_DSP_SetupChannel_STCT								*pSetupChannel;
	AC494_DSP_CodecConfg_STCT								*pCodecConfg;
	AC494_DSP_AdvanceChannelConfg_STCT						*pAdvanceChannelConfg;
	AC494_DSP_OpenUpdateChannelConfg_STCT 					*pOpenChannelConfg;
	AC494_DSP_ActivateUpdateRegularRtpChannelConfg_STCT 	*pActivateRegularRtp;
	AC494_DSP_SendIbsString_STCT							sendIbsString;
	AC494_DSP_SendExtendedIbsString_STCT					sendExtendedIbsString;
	AC494_DSP_Transmit_STCT									TransmitPacket;
	AC494_DSP_Transmit_STCT									*pTransmitPacket;
	AC494_DSP_ActivateOrDeactivate3WayConf_STCT				conf3way;
	AC494_DSP_CloseChannel_STCT								closeChannel;
#if defined(CONFIG_SDP_IPPHONE) || defined(CONFIG_ORCHID)
	AC494_DSP_CODEC_DIR_STCT								codecDir;
#endif
	AC494_DSP_AcousticEchoCancelerConfiguration_STCT		*pAchousticEchoCancelerConfg;
	AC494_DSP_ProgramDownload_STCT							*pProgramDownload;
	AC494_DSP_SetDefaults_STCT								*pSetDefaults;

	AC494_MEM_MODIFY_STCT			mm;
	AC494_MEM_DISPLAY_STCT			md;
	AC494_MEM_CHECKSUM_STCT			mc;

	/*A.L. 061031 T.38*/
	AC494_DSP_T38ChannelConfiguration_STCT					t38ChannelConfig;
	AC494_DSP_ActivateUpdateFaxRelayChannelConfg_STCT		t38ChannelActivate;
	AC494_DSP_CloseRegularRtpChannelConfg_STCT				closeRegularRtp;
	AC494_DSP_TimeSlotsManipulation_STCT	                *pManipulateTimeSlots;
    AC494_DSP_CallerId_STCT                                 sendCallerId;
	Tac49xRecordCommandAttr									RecordCommandst;
	AC494_DSP_PacketRecordingStartStop_STCT					PacketRecordingStartStop;

	int temp;
    int error;
	unsigned int RegVal;
	int i;
	static int cnt;

	flag = *(unsigned int *)buf; 	/* get the command flag and switch to the command */

	switch(flag) {
		/* download command - download a program to the dsp */
		case ACG_DOWNLOAD_CMD_E:

			/* cast the pointer */
			pAcgCmd = (acgTCommand *)buf;

			pInitializationParams = (acgTInitializationParameters *)pAcgCmd->data;

			if(copy_from_user(	&InitializationParams,      /* copy the receive structure to kernel */
								pInitializationParams,
								sizeof(acgTInitializationParameters)))
			{
				ret = -EFAULT;
				break;
			}


			NumberOf498DspDevices = InitializationParams.SecondaryDSPDeviceInformation.NumberOfDevices;

			printk("NumberOf498DspDevices=%d\n", NumberOf498DspDevices);

			for (deviceCnt = 0; deviceCnt < 1 + NumberOf498DspDevices; deviceCnt++)
			{
				if(dsp_dev->dspDev[deviceCnt].downloadStatus)
					return AC494_DSP_OK_E; /* don't download dsp becasue already been downloaded */
			}

			if(InitializationParams.PrimaryDSPDeviceInformation.ProgramSize != 0)
			{
				p494temp_buf = vmalloc(InitializationParams.PrimaryDSPDeviceInformation.ProgramSize);
				if (p494temp_buf == NULL){
			            	printk("vmalloc failed\r\n");
					return -ENOMEM;
				}
			}
			else
			{
			        printk("Primary device program size is %d\r\n", InitializationParams.PrimaryDSPDeviceInformation.ProgramSize);
				return AC494_DSP_ERROR_E;
			}

			if(InitializationParams.SecondaryDSPDeviceInformation.ProgramSize != 0)
			{
				p498temp_buf = vmalloc(InitializationParams.SecondaryDSPDeviceInformation.ProgramSize);
				if (p498temp_buf == NULL){
			            	printk("vmalloc failed\r\n");
					return -ENOMEM;
				}
			}

			/* copy the program */
			if(copy_from_user(p494temp_buf, InitializationParams.PrimaryDSPDeviceInformation.pProgram, InitializationParams.PrimaryDSPDeviceInformation.ProgramSize)){
				ret = -EFAULT;
				break;
			}

			printk("DSP device download : 494 firmware file size is %d\n", pInitializationParams->PrimaryDSPDeviceInformation.ProgramSize);

			if(InitializationParams.SecondaryDSPDeviceInformation.pProgram != NULL)
			{
				if(copy_from_user(p498temp_buf, InitializationParams.SecondaryDSPDeviceInformation.pProgram, InitializationParams.SecondaryDSPDeviceInformation.ProgramSize)){
					ret = -EFAULT;
					break;
				}
				printk("DSP device download : 498 firmware file size is %d\n", InitializationParams.SecondaryDSPDeviceInformation.ProgramSize);
				printk("\nInitializing vlynq\n");
				vlynq_dev_init_vdsp();

			}

			for (deviceCnt = 0; deviceCnt < 1 + NumberOf498DspDevices; deviceCnt++)
			{
				deviceNum = devOrder[deviceCnt];

				if(deviceNum != 0) 		/* 498 device */
				{
					printk("DSP device %d, download a %d channels DSP firmware..", deviceNum, p498temp_buf[26] - 48);
	 				ret = Ac49xBoot(deviceNum, p498temp_buf, 0, 0);
	   	        	dsp_dev->dspDev[deviceNum].numOfChannels = p498temp_buf[26] - 48;

	   	        	if (ret == PROGRAM_DOWNLOAD_STATUS__BOOT_STATUS_REGISTER_SUCCESS)
	   	        	{
				        printk("passed\r\n");
				    }
			        else
			        {
				        printk("failed, ret = %d\r\n", ret);
				    }

	        	}
	        	else 		/* 494 device */
				{
					printk("DSP device %d, download a %d channels DSP firmware..", deviceNum, p494temp_buf[26] - 48);
		            /*A.L 061218 - Init function should be called before the download process !!!*/
		            Ac49xInitDriver(CONTROL__DISABLE);
			        ret = Ac49xUserDef_Boot(deviceNum, p494temp_buf, 0, 0);	/* boot the program to the dsp */
				    /*ac494dspStart();*/
				    dsp_dev->dspDev[deviceNum].numOfChannels = p494temp_buf[26] - 48;
				    if (ret == PROGRAM_DOWNLOAD_STATUS__BOOT_STATUS_REGISTER_SUCCESS)
	   	        	{
				        printk("passed\r\n");
				    }
			        else
			        {
				        printk("failed, ret = %d\r\n", ret);
				    }
                }

			}

			if(p494temp_buf != NULL)
				vfree(p494temp_buf);

			if(p498temp_buf != NULL)
				vfree(p498temp_buf);

			Ac49xResetBufferDescriptors();

			if (ret != PROGRAM_DOWNLOAD_STATUS__BOOT_STATUS_REGISTER_SUCCESS)
                return ret;

			/*************************************/
			/*** Set default params and setup devices ***/
			/*************************************/

        	ret = sock_create(PF_INET, SOCK_DGRAM, 0, &pDspPRsock);
        	if (ret<0)
        	{
            	printk("Error during creation of socket; terminating\n");
            	return ret;
        	}

#if ( PACKET_RECORDING_ENABLE == 1 )

                       	printk("DSP packet recording init..");
			InitPacketRecordingInfo(InitializationParams.RemoteRecordingIpAddress, InitializationParams.RemoteRecordingPort);

			Ac49xInitializePacketRecording(FlushPacketToNet);
                       	printk("passed\r\n");

			mdelay(300);
#endif //( PACKET_RECORDING_ENABLE == 1 )

			NumberOfCallProgressTones = InitializationParams.NumberOfCallProgressTones;

			for (deviceCnt = 0; deviceCnt < 1 + NumberOf498DspDevices; deviceCnt++)
			{
				deviceNum = devOrder[deviceCnt];

				ret = dspDev_setDefaults(deviceNum);
				if (-1 == ret)
					break;

				/* cast the pointer */

				printk("Setup device %d..", deviceNum);

				ret = setDeviceParams(deviceNum, NumberOfCallProgressTones,
									InitializationParams.CallProgressTones,
									&(dsp_dev->dspDev[deviceNum].SetupDeviceAttr));
				if (-1 == ret)
					break;

		        ret = Ac49xSetupDevice(deviceNum, &(dsp_dev->dspDev[deviceNum].SetupDeviceAttr));
                if(ret != SETUP_DEVICE_STATUS__SUCCESS)
                    printk("failed, ret = %d\r\n", ret);

				for(Channel=0; Channel < dsp_dev->dspDev[deviceNum].numOfChannels; Channel++)
				{
					ret = AdvanceChannelConfig(deviceNum, Channel);
					if (-1 == ret)
						break;
				}

				if(ret == AC494_DSP_OK_E)
				{
					dsp_dev->dspDev[deviceNum].initStatus = 1; 	/* update init flag to true */
					printk("passed\r\n");
				}

				if(deviceNum == 0) {
				    	/* setup the device */
		                if((*(volatile unsigned int *)(EMIF_4M_DSP_ALIGEND + 0x380000)) == 0)  {  /* (EMIF_4M_DSP_ALIGEND + 0x380000) = DSP start address at SDRAM */
		            	    /* check that the firmware is ok (silicon version) */
		            	    printk("<1>Incorrect silicon version\r\n");
						    ret = -EFAULT;
					    }
	            }

#if defined(CONFIG_SDP_IPPHONE) || defined(CONFIG_ORCHID)
				if(dsp_dev->dspDev[deviceNum].SetupDeviceAttr.Open.NumberOfChannels != 4)
				{
					ret = startEC(&(dsp_dev->dspDev[deviceNum].SetupChannelAttr[0]));
				}
				else
				{
					printk("can't start AEC with a 4 channels application\r\n");
					ret = -EFAULT;
					break;
				}
#endif /*CONFIG_SDP_IPPHONE */

				dsp_dev->dspDev[deviceNum].downloadStatus = 1; 	/* update the download flag to true */

			}

			break;



		/* reset buffers descriptor command */

		case ACG_RESET_BUF_DES_E:

			/* reset the buffer descriptors */
			Ac49xResetBufferDescriptors();

			ret = AC494_DSP_OK_E;

			break;



		/* decode media command  - write buffer to the dsp buffer */

		case ACG_DECODE_MEDIA_CMD_E:

			/* cast the pointer */
			pAcgCmd = (acgTCommand *)buf;

			pDecodeMedia = (acgTDecodeMedia *)pAcgCmd->data;


			absCh = pAcgCmd->Channel;
			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
			{
				break;
			}


			TxPacketParams.Channel = dspCh.channel;
			TxPacketParams.Device = dspCh.device;
			TxPacketParams.PacketSize = pDecodeMedia->Len + HPI_HEADER_LEN;  /* save the first 4 bytes for the header */
			TxPacketParams.Protocol = pDecodeMedia->MediaProtocol;

			TxPacketParams.FavoriteStream = CONTROL__DISABLE;
			TxPacketParams.TransferMedium = TRANSFER_MEDIUM__NETWORK;
			TxPacketParams.UdpChecksumIncluded = CONTROL__DISABLE;

			/* copy the user buffer to kernel */
 			/* save the first 4 bytes for the header */
			if(copy_from_user(MediaTempBuff + HPI_HEADER_LEN, pDecodeMedia->Buff, pDecodeMedia->Len)) {
				ret = -EFAULT;
				break;
            		}

			/* points to the kernel buffer */
			TxPacketParams.pOutPacket = MediaTempBuff;

			ret = Ac49xTransmitPacket(&TxPacketParams);
			break;



       		/* re-initialize echo canceller command */

		case ACG_REINITIALIZE_ECHO_CANCELER_CMD_E:

			/* cast the pointer */
			pAcgCmd = (acgTCommand *)buf;

			absCh = pAcgCmd->Channel;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;

			Device = dspCh.device;
			Channel = dspCh.channel;

			/* check for init flag */
			if(!(dsp_dev->dspDev[Device].initStatus)) {
				ret = AC494_DSP_NO_INIT_E;
				break;
			}

			dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.EchoCancelerMode = ECHO_CANCELER_MODE__CLEAR;  /* EC will init internally and re-learn signals */

			ret = Ac49xUpdateChannelConfiguration(Device, Channel, &(dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel));

			break;



        /* open channel command */

		case ACG_OPEN_CHANNEL_CMD_E:

			/* cast the pointer */
			pAcgCmd = (acgTCommand *)buf;

			pChannelParams = (acgTChannelParameters *)pAcgCmd->data;

			absCh = pAcgCmd->Channel;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;

			Device = dspCh.device;
			Channel = dspCh.channel;

			/* check for init flag */
			if(!(dsp_dev->dspDev[Device].initStatus)) {
				ret = AC494_DSP_NO_INIT_E;
				break;
			}

			if(copy_from_user(	&ChannelParams,      /* copy the receive structure to kernel */
								pChannelParams,
								sizeof(acgTChannelParameters)))
			{
				ret = -EFAULT;
				break;
			}

#if defined(CONFIG_SDP_IPPHONE) || defined(CONFIG_ORCHID)

			/* setup channel configuration */
            		ret = CodecConfig(Device, Channel, &ChannelParams);
			if (-1 == ret)
			{
				break;
			}
#endif /* CONFIG_SDP_IPPHONE */
			ret = OpenUpdateChannelConfig(Device, Channel, &ChannelParams);
			if (-1 == ret)
				break;

			/* Configure the regular rtp channel without activating it */
			ret = ActivateUpdateRegularRtpChannelConfig(&dsp_dev->dspDev[Device].SetupChannelAttr[Channel].ActivateOrUpdateRtp[MEDIA_CHANNEL_TYPE__REGULAR], &ChannelParams);
			if (-1 == ret)
				break;

			/* Open the DSP channel */
			ret = Ac49xOpenChannelConfiguration(Device, Channel, &(dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel));

			break;


        /* update channel command */

		case ACG_UPDATE_CHANNEL_CMD_E:

			/* cast the pointer */
			pAcgCmd = (acgTCommand *)buf;

			pChannelParams = (acgTChannelParameters *)pAcgCmd->data;

			absCh = pAcgCmd->Channel;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;

			Device = dspCh.device;
			Channel = dspCh.channel;

			/* check for init flag */
			if(!(dsp_dev->dspDev[Device].initStatus)) {
				ret = AC494_DSP_NO_INIT_E;
				break;
			}

			if(copy_from_user(	&ChannelParams,      /* copy the receive structure to kernel */
								pChannelParams,
								sizeof(acgTChannelParameters)))
			{
				ret = -EFAULT;
				break;
			}

#if defined(CONFIG_SDP_IPPHONE) || defined(CONFIG_ORCHID)

			/* setup channel configuration */
            		ret = CodecConfig(Device, Channel, pChannelParams);
			if (-1 == ret)
				break;


#endif /* CONFIG_SDP_IPPHONE */

			ret = OpenUpdateChannelConfig(Device, Channel, &ChannelParams);
			if (-1 == ret)
				break;

			/* Configure the regular rtp channel without activating it */
			ret = ActivateUpdateRegularRtpChannelConfig(&dsp_dev->dspDev[Device].SetupChannelAttr[Channel].ActivateOrUpdateRtp[MEDIA_CHANNEL_TYPE__REGULAR], &ChannelParams);
			if (-1 == ret)
				break;

			/* Update the DSP channel */
			ret = Ac49xUpdateChannelConfiguration(Device, Channel, &(dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel));

			if(RTP_IS_ACTIVE == isActivateRTP[Device][Channel])
			{
				/* Update the RTP channel */
				ret = Ac49xUpdateRegularRtpChannelConfiguration(Device, Channel, &(dsp_dev->dspDev[Device].SetupChannelAttr[Channel].ActivateOrUpdateRtp[MEDIA_CHANNEL_TYPE__REGULAR]));
			}

			break;


		/* play tone command */

		case ACG_PLAY_TONE_CMD_E:

			memset(&IbsStringAttr, 0, sizeof(IbsStringAttr));

			/* cast the pointer */
			pAcgCmd = (acgTCommand *)buf;

			pTone = (acgTTone *)pAcgCmd->data;

			absCh = pAcgCmd->Channel;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;

			Device = dspCh.device;
			Channel = dspCh.channel;

			/* check for init flag */
			if(!(dsp_dev->dspDev[Device].initStatus)) {
				ret = AC494_DSP_NO_INIT_E;
				break;
			}

			if(copy_from_user(	&Tone,      /* copy the receive structure to kernel */
								pTone,
								sizeof(acgTTone)))
			{
				ret = -EFAULT;
				break;
			}

			setToneParams(&Tone, &IbsStringAttr);

			/* send ibs string */
			ret = Ac49xSendIbsString(Device, Channel, &IbsStringAttr);

			break;


		/* play tone sequence command */

		case ACG_PLAY_TONE_SEQUENCE_CMD_E:

			memset(&IbsStringAttr, 0, sizeof(IbsStringAttr));

			/* cast the pointer */
			pAcgCmd = (acgTCommand *)buf;

			pToneSequence = (acgTToneSequence *)pAcgCmd->data;

			absCh = pAcgCmd->Channel;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;

			Device = dspCh.device;
			Channel = dspCh.channel;

			/* check for init flag */
			if(!(dsp_dev->dspDev[Device].initStatus)) {
				ret = AC494_DSP_NO_INIT_E;
				break;
			}

			if(copy_from_user(	&ToneSequence,      /* copy the receive structure to kernel */
								pToneSequence,
								sizeof(acgTToneSequence)))
			{
				ret = -EFAULT;
				break;
			}

			setToneSequenceParams(&ToneSequence, &IbsStringAttr);

			/* send ibs string */
			ret = Ac49xSendIbsString(Device, Channel, &IbsStringAttr);

			break;


		case ACG_PLAY_TONE_EXTENDED_CMD_E:

			memset(&extendedIbsStringAttr, 0, sizeof(extendedIbsStringAttr));

			/* cast the pointer */
			pAcgCmd = (acgTCommand *)buf;

			pToneExtended = (acgTToneExtended *)pAcgCmd->data;

			absCh = pAcgCmd->Channel;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;

			Device = dspCh.device;
			Channel = dspCh.channel;

			/* check for init flag */
			if(!(dsp_dev->dspDev[Device].initStatus)) {
				ret = AC494_DSP_NO_INIT_E;
				break;
			}

			if(copy_from_user(	&ToneExtended,      /* copy the receive structure to kernel */
								pToneExtended,
								sizeof(acgTToneExtended)))
			{
				ret = -EFAULT;
				break;
			}

			setToneExtendedParams(&ToneExtended, &extendedIbsStringAttr);

			/* send extended ibs string */
			ret = Ac49xSendExtendedIbsString(Device, Channel, &extendedIbsStringAttr);

			break;
		/* play tone command */

		case ACG_STOP_TONE_CMD_E:

			/* cast the pointer */
			pAcgCmd = (acgTCommand *)buf;

			absCh = pAcgCmd->Channel;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;

			Device = dspCh.device;
			Channel = dspCh.channel;

			/* check for init flag */
			if(!(dsp_dev->dspDev[Device].initStatus)) {
				ret = AC494_DSP_NO_INIT_E;
				break;
			}

			ret = Ac49xSendIbsStop(Device, Channel);

			break;

#if defined(CONFIG_SDP_IPPHONE) || defined(CONFIG_ORCHID)

		/* set input gain command */

		case ACIPP_SET_INPUT_GAIN_CMD_E:


			/* cast the pointer */
			pAcgCmd = (acgTCommand *)buf;


		       IPPInputGain= *( (acgEVoiceGain*)pAcgCmd->data);
			absCh = pAcgCmd->Channel;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;

			Device = dspCh.device;
			Channel = dspCh.channel;

			/* check for init flag */
			if(!(dsp_dev->dspDev[Device].initStatus)) {
				printk("[%s,%d] DSP is not initialized \n",__FUNCTION__,__LINE__);
				ret = AC494_DSP_NO_INIT_E;
				break;
			}

			dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.InputGain = IPPInputGain;

			ret = Ac49xUpdateChannelConfiguration(Device, Channel, &(dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel));

			break;


		/* set output gain command */
		case ACIPP_SET_OUTPUT_GAIN_CMD_E:


			/* cast the pointer */
			pAcgCmd = (acgTCommand *)buf;


		       IPPOutputGain= *( (acgEVoiceGain*)pAcgCmd->data);
			absCh = pAcgCmd->Channel;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;

			Device = dspCh.device;
			Channel = dspCh.channel;

			/* check for init flag */
			if(!(dsp_dev->dspDev[Device].initStatus)) {
				printk("[%s,%d] DSP is not initialized \n",__FUNCTION__,__LINE__);
				ret = AC494_DSP_NO_INIT_E;
				break;
			}

			dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.VoiceOutputGain = IPPOutputGain;

			ret = Ac49xUpdateChannelConfiguration(Device, Channel, &(dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel));

			break;

		/* set speaker status command */

		case ACIPP_SET_SPK_STATUS_CMD_E:


			/* cast the pointer */
			pAcgCmd = (acgTCommand *)buf;

			IPPCodecType = *((acIPPCodecType *)pAcgCmd->data);
			absCh = pAcgCmd->Channel;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;

			Device = dspCh.device;
			Channel = dspCh.channel;

			/* check for init flag */
			if(!(dsp_dev->dspDev[Device].initStatus)) {
				ret = AC494_DSP_NO_INIT_E;
				break;
			}

			setCodecAEC(&(dsp_dev->dspDev[Device].SetupChannelAttr[Channel]),  IPPCodecType);
			ret = Ac49xCodecConfiguration(Device, Channel, &(dsp_dev->dspDev[Device].SetupChannelAttr[Channel].CodecConfiguration));
			if (-1 == ret)
				return ret;


#if 0
			ret = Ac49xUpdateChannelConfiguration(Device, Channel, &(dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel));
			if (-1 == ret)
				return ret;


			if(RTP_IS_ACTIVE == isActivateRTP[Device][Channel])
			{
				/* Update the RTP channel */
				ret = Ac49xUpdateRegularRtpChannelConfiguration(Device, Channel, &(dsp_dev->dspDev[Device].SetupChannelAttr[Channel].ActivateOrUpdateRtp[MEDIA_CHANNEL_TYPE__REGULAR]));

				printk("Ac49xUpdateRegularRtpChannelConfiguration()  ret = %d \n ",ret);
			}
#endif


			break;


		/* ring command */

		case ACIPP_RING_START_CMD_E:

			memset(&extendedIbsStringAttr, 0, sizeof(extendedIbsStringAttr));

			/* cast the pointer */
			pAcgCmd = (acgTCommand *)buf;

			absCh =  pAcgCmd->Channel;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;

			Device = dspCh.device;
			Channel = dspCh.channel;

			/* check for init flag */
			if(!(dsp_dev->dspDev[Device].initStatus)) {
				ret = AC494_DSP_NO_INIT_E;
				break;
			}

			setRingerParams(-1, &extendedIbsStringAttr); /*set duration to infinit*/

			/* send ibs string */
			ret = Ac49xSendExtendedIbsString(Device, Channel, &extendedIbsStringAttr);

		break;

		case ACIPP_RING_STOP_CMD_E:

			/* cast the pointer */
			pAcgCmd = (acgTCommand *)buf;

			absCh =  pAcgCmd->Channel;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;

			Device = dspCh.device;
			Channel = dspCh.channel;

			/* check for init flag */
			if(!(dsp_dev->dspDev[Device].initStatus)) {
				ret = AC494_DSP_NO_INIT_E;
				break;
			}


			/* stop ibs  */
			ret = Ac49xSendIbsStop(Device, Channel);

		break;






		case ACIPP_MEM_MODIFY_CMD_E:

            /* copy the user structure to kernel */
			if(copy_from_user(&mm, buf, sizeof(AC494_MEM_MODIFY_STCT))) {
				ret = -EFAULT;
                		break;
			}

			while((mm.count--)>0) {
            			*((unsigned int *)mm.addr) = mm.data;
				mm.addr+=4;
			}

			ret = AC494_DSP_OK_E;

			break;

		case ACIPP_MEM_DISPLAY_CMD_E:

            /* copy the user structure to kernel */
			if(copy_from_user(&md, buf, sizeof(AC494_MEM_DISPLAY_STCT))) {
				ret = -EFAULT;
                		break;
			}

			printk("<1>\r\n");
			while((md.count--)>0) {
				printk("<1> %x : %x \r\n",md.addr ,*(unsigned int *)md.addr);
				md.addr+=4;
			}

			ret = AC494_DSP_OK_E;

			break;

		case ACIPP_MEM_CHECKSUM_CMD_E:

            /* copy the user structure to kernel */
			if(copy_from_user(&mc, buf, sizeof(AC494_MEM_CHECKSUM_STCT))) {
				ret = -EFAULT;
               			break;
			}

			mc.checksum = 0;
			while((mc.count--)>0) {
				mc.checksum += *(unsigned int *)md.addr;
			}
			printk("<1>checksum = %x\r\n",mc.checksum);

			ret = AC494_DSP_OK_E;

			break;


		case ACIPP_SET_DSP_DEVICE_CONTROL_REGISTER_CMD_E:

			/* reset the buffer descriptors */
			Ac49xResetBufferDescriptors();

			ret = Ac49xDeviceSetControlRegister(Device, &(dsp_dev->dspDev[Device].SetupDeviceAttr.DeviceControlRegister));

			break;









#endif /*CONFIG_SDP_IPPHONE*/

		/* start 3 way conference command */
		case ACG_SET_3WAYCONF_CMD_E:

			memset(&_3WayConferenceAttr, 0, sizeof(_3WayConferenceAttr));

			/* cast the pointer */
			pAcgCmd = (acgTCommand *)buf;

			p3WayConference = (acgT3WayConference *)pAcgCmd->data;

			absCh = p3WayConference->ConferenceChannelMate;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;
			Device = dspCh.device;
			Channel = dspCh.channel;

			/*convert the conference channel from absolute values to dsp values*/
			ret = getDspChannelByAbsChannel(pAcgCmd->Channel, &dspCh);
			if (-1 == ret)
				return ret;

			if(copy_from_user(	&_3WayConference,      /* copy the receive structure to kernel */
								p3WayConference,
								sizeof(acgT3WayConference)))
			{
                printk("SET 3 way conf failed\n");///anuja
				ret = -EFAULT;
				break;
			}

			if (_3WayConference.Mode==ACG__3_WAY_CONFERENCE_MODE__1)
				set3wcParams(&_3WayConferenceAttr, dspCh.channel, ACG__3_WAY_CONFERENCE_MODE__1); /*Mode is incorrect*/
			if (_3WayConference.Mode==ACG__3_WAY_CONFERENCE_MODE__DISABLE)
				set3wcParams(&_3WayConferenceAttr, dspCh.channel, ACG__3_WAY_CONFERENCE_MODE__DISABLE); /*Mode is incorrect*/

			/* check for init flag */
			if(!(dsp_dev->dspDev[Device].initStatus)) {
				ret = AC494_DSP_NO_INIT_E;
				break;
			}

			/* send ibs string */
			ret = Ac49xActivateOrDeactivate3WayConferenceConfiguration(Device, Channel, &(_3WayConferenceAttr));

			break;


		case ACG_CLOSE_CHANNEL_CMD_E:


			/* cast the pointer */
			pAcgCmd = (acgTCommand *)buf;

			absCh = pAcgCmd->Channel;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;

			Device = dspCh.device;
			Channel = dspCh.channel;

			/* check for init flag */
			if(!(dsp_dev->dspDev[Device].initStatus)) {
				ret = AC494_DSP_NO_INIT_E;
				break;
			}

			ret = Ac49xCloseChannelConfiguration(Device, Channel);

			break;




		/* start fax VBD command */
	    case ACG_START_FAX_VBD_CMD_E:
			pAcgCmd = (acgTCommand *)buf;
			phi_ssrc = pAcgCmd->Channel;
			ret = AC494_DSP_OK_E;

			break;



		/* stop fax VBD command */
        case ACG_STOP_FAX_VBD_CMD_E:
			 pAcgCmd = (acgTCommand *)buf;
			ret = AC494_DSP_OK_E;

			break;



		/* start fax relay command */
        case ACG_START_FAX_RELAY_CMD_E:

	pAcgCmd = (acgTCommand *)buf;

            ret = AC494_DSP_OK_E;
			break;


		/* stop fax relay command */
        case ACG_STOP_FAX_RELAY_CMD_E:

			memset(&closeRtpChannelConfigurationAttr, 0, sizeof(closeRtpChannelConfigurationAttr));

			/* cast the pointer */
			pAcgCmd = (acgTCommand *)buf;

			absCh = pAcgCmd->Channel;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;
			Device = dspCh.device;
			Channel = dspCh.channel;

			/* check for init flag */
			if(!(dsp_dev->dspDev[Device].initStatus)) {
				ret = AC494_DSP_NO_INIT_E;
				break;
			}

			/* deactivate T.38 */
			ret = Ac49xDeactivateFaxRelayCommand(Device, Channel);
			if (-1 == ret)
				return ret;

			ret = Ac49xActivateRegularRtpChannelConfiguration(Device, Channel, &(dsp_dev->dspDev[Device].SetupChannelAttr[Channel].ActivateOrUpdateRtp[0]));

			 ret = AC494_DSP_OK_E;
			break;



        case ACG_ACTIVATE_RTP_CMD_E:

			/* cast the pointer */
			pAcgCmd = (acgTCommand *)buf;

			absCh = pAcgCmd->Channel;
			cnt++;
			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;
			Device = dspCh.device;
			Channel = dspCh.channel;

			/* check for init flag */
			if(!(dsp_dev->dspDev[Device].initStatus)) {
				ret = AC494_DSP_NO_INIT_E;
				break;
			}
            printk("ATIVATED RTP %d times \n",cnt);///anuja
			dsp_dev->dspDev[Device].SetupChannelAttr[Channel].ActivateOrUpdateRtp[MEDIA_CHANNEL_TYPE__REGULAR].RtpSynchronizingSource = phi_ssrc;//(rand()<<16)+rand();
             // the initial value of the sequence number and timestamp should be random
			dsp_dev->dspDev[Device].SetupChannelAttr[Channel].ActivateOrUpdateRtp[MEDIA_CHANNEL_TYPE__REGULAR].TxSequenceNumber = (rand()<<16)+rand();
			dsp_dev->dspDev[Device].SetupChannelAttr[Channel].ActivateOrUpdateRtp[MEDIA_CHANNEL_TYPE__REGULAR].TxTimeStamp = (rand()<<16)+rand();

			ret = Ac49xActivateRegularRtpChannelConfiguration(Device, Channel, &(dsp_dev->dspDev[Device].SetupChannelAttr[Channel].ActivateOrUpdateRtp[MEDIA_CHANNEL_TYPE__REGULAR]));
			if (-1 == ret)
				return ret;

			isActivateRTP[Device][Channel] = RTP_IS_ACTIVE;

            break;



        case ACG_DEACTIVATE_RTP_CMD_E:

			memset(&closeRtpChannelConfigurationAttr, 0, sizeof(closeRtpChannelConfigurationAttr));

			/* cast the pointer */
			pAcgCmd = (acgTCommand *)buf;

			absCh = pAcgCmd->Channel;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;
			Device = dspCh.device;
			Channel = dspCh.channel;

			/* check for init flag */
			if(!(dsp_dev->dspDev[Device].initStatus)) {
				ret = AC494_DSP_NO_INIT_E;
				break;
			}

            		ret = Ac49xCloseRegularRtpChannelConfiguration(Device, Channel, &closeRtpChannelConfigurationAttr);
    			if (-1 == ret)
				return ret;

			isActivateRTP[Device][Channel] = RTP_IS_DEACTIVE;

            		break;

	    case ACG_MANIPULATE_TIMESLOTS_CMD_E:

			/* cast the pointer */
			pManipulateTimeSlots = (AC494_DSP_TimeSlotsManipulation_STCT *)buf;

			/* allocate kernel buffer */
			ManipulateTimeslotsTempBuff = kmalloc(sizeof(AC494_DSP_TimeSlotsManipulation_STCT),GFP_KERNEL);


			/* copy the user buffer to kernel */
			if(copy_from_user(ManipulateTimeslotsTempBuff,	pManipulateTimeSlots,sizeof(AC494_DSP_TimeSlotsManipulation_STCT))) {
				ret = -EFAULT;
			 	break;
           		 }

            		absCh = pManipulateTimeSlots->Channel;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;
			Device = dspCh.device;

			/* check for init flag */
			if(!(dsp_dev->dspDev[Device].initStatus)) {
				ret = AC494_DSP_NO_INIT_E;
				break;
			}

			ret = Ac49xTimeSlotManipulationCommand(Device, &(pManipulateTimeSlots->Attr));

            		/* free the kernel buffer */
			kfree(ManipulateTimeslotsTempBuff);

			break;

    	/* send caller id */
    	case ACG_SEND_CALLER_ID_CMD:

    		/* cast the pointer */
    		pAcgCmd = (acgTCommand *)buf;

    		pCallerId = (acgTCallerId *)pAcgCmd->data;

            	absCh = pAcgCmd->Channel;
    		ret = getDspChannelByAbsChannel(absCh, &dspCh);
    		if (-1 == ret)
    			return ret;

    		Device = dspCh.device;
    		Channel = dspCh.channel;

    		if(copy_from_user(	&CallerId,      /* copy the receive structure to kernel */
    								pCallerId,
    								sizeof(acgTCallerId)))
    		{
    			ret = -EFAULT;
    			break;
    		}

     		ret = sendCallerIdMessage(Device, Channel, &CallerId);

    		break;



    	/* stop caller id */
    	case ACG_STOP_CALLER_ID_CMD:

    		/* cast the pointer */
    		pAcgCmd = (acgTCommand *)buf;

            	absCh = pAcgCmd->Channel;
    		ret = getDspChannelByAbsChannel(absCh, &dspCh);
    		if (-1 == ret)
    			return ret;

    		Device = dspCh.device;
    		Channel = dspCh.channel;

    		ret = Ac49xSendCallerIdStop(Device, Channel);

    		break;



    	/* send message waiting indication command */
    	case ACG_PLAY_MWI_CMD_E:

    		/* cast the pointer */
    		pAcgCmd = (acgTCommand *)buf;

    		pMWI = (acgTMWI *)pAcgCmd->data;

            	absCh = pAcgCmd->Channel;
    		ret = getDspChannelByAbsChannel(absCh, &dspCh);
    		if (-1 == ret)
    			return ret;

    		Device = dspCh.device;
    		Channel = dspCh.channel;

    		if(copy_from_user(	&MWI,      /* copy the receive structure to kernel */
    								pMWI,
    								sizeof(acgTMWI)))
    		{
    			ret = -EFAULT;
    			break;
    		}

     		ret = sendMWIMessage(Device, Channel, &MWI);

    		break;


    	/* send message waiting indication command */
    	case ACG_STOP_MWI_CMD_E:

    		/* cast the pointer */
    		pAcgCmd = (acgTCommand *)buf;

    		pMWI = (acgTMWI *)pAcgCmd->data;

            	absCh = pAcgCmd->Channel;
    		ret = getDspChannelByAbsChannel(absCh, &dspCh);
    		if (-1 == ret)
    			return ret;

    		Device = dspCh.device;
    		Channel = dspCh.channel;

    		if(copy_from_user(	&MWI,      /* copy the receive structure to kernel */
    								pMWI,
    								sizeof(acgTMWI)))
    		{
    			ret = -EFAULT;
    			break;
    		}

     		ret = sendMWIMessage(Device, Channel, &MWI);

    		break;


    	/* debug information command */
    	case ACG_DEBUG_INFORMATION_ACTIVATION:

    		/* cast the pointer */
    		pAcgCmd = (acgTCommand *)buf;

    		pDebugInformation = (acgTDebugInformation *)pAcgCmd->data;

            	absCh = pDebugInformation->SpecificRecordingInformation.Channel;


    		ret = getDspChannelByAbsChannel(absCh, &dspCh);
    		if (-1 == ret)
    			return ret;

    		Device = dspCh.device;
    		Channel = dspCh.channel;

    		if(copy_from_user(	&DebugInformation,      /* copy the receive structure to kernel */
    								pDebugInformation,
    								sizeof(acgTDebugInformation)))
    		{
    			ret = -EFAULT;
    			break;
    		}


    		if(ACG_CONTROL__ENABLE == DebugInformation.PacketRecording)
    			SetPacketRecordingStatus(CONTROL__ENABLE);
    		else
    		SetPacketRecordingStatus(CONTROL__DISABLE);

    		/* check for init flag - if not initialized yet don't activate debug information */
    		if(!(dsp_dev->dspDev[Device].initStatus)) {
    			ret = AC494_DSP_OK_E;
    			break;
    		}

    		RecordCommandst.Coder = CODER__G711ALAW;

    		if(ACG_CONTROL__ENABLE == DebugInformation.SpecificRecordingInformation.NetworkRecording)
    		{
    			SetNetworkRecordingStatus(CONTROL__ENABLE);
    			RecordCommandst.Type	= RECORD_COMMAND_TYPE__NETWORK_START;
    		}
    		else
    		{
    			SetNetworkRecordingStatus(CONTROL__DISABLE);
    			RecordCommandst.Type	= RECORD_COMMAND_TYPE__NETWORK_STOP;
    		}

    		ret = Ac49xRecordCommand(Device, Channel, &RecordCommandst);
    		if (-1 == ret)
    			return ret;

    		if(ACG_CONTROL__ENABLE == DebugInformation.SpecificRecordingInformation.TDMRecording)
    		{
    			SetTDMRecordingStatus(CONTROL__ENABLE);
    			RecordCommandst.Type	= RECORD_COMMAND_TYPE__TDM_START;
    		}
    		else
    		{
    			SetTDMRecordingStatus(CONTROL__DISABLE);
    			RecordCommandst.Type	= RECORD_COMMAND_TYPE__TDM_STOP;
    		}

    		ret = Ac49xRecordCommand(Device, Channel, &RecordCommandst);
    		if (-1 == ret)
    			return ret;

    		break;



		case ACG_PLAYBACK_TDM_START_CMD:
			pAcgCmd = (acgTCommand *)buf;
    	    absCh = pAcgCmd->Channel;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;

			Device = dspCh.device;
			Channel = dspCh.channel;


			pPlaybackVoice = (acgTPlaybackData *)pAcgCmd->data;
			if(copy_from_user(	&PlaybackVoice,      /* copy the receive structure to kernel */
								pPlaybackVoice,
								sizeof(acgECoder)+sizeof(int)))
			{
				ret = -EFAULT;
				break;
			}
			playbackCoder = PlaybackVoice.Coder;
	        printk("\r\nplaybackCoder = [%d]\r\n", playbackCoder);

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
			if(CODER__LINEAR_16_BIT_PCM_SAMPLE_RATE_16KHZ == playbackCoder)
			{
				dsp_dev->dspDev[Device].SetupChannelAttr[Channel].CodecConfiguration.SamplingRate = SAMPLING_RATE__16_KHz;
			}
			else
			{
				dsp_dev->dspDev[Device].SetupChannelAttr[Channel].CodecConfiguration.SamplingRate = SAMPLING_RATE__8_KHz;
			}

			Ac49xCodecConfiguration(Device, Channel, &(dsp_dev->dspDev[Device].SetupChannelAttr[Channel].CodecConfiguration));

			ret  = Ac49xPlaybackTdmStart(Device, Channel, playbackCoder, AC49X_ENDIAN_MODE);
#endif
			break;


	    case ACG_PLAYBACK_NETWORK_START_CMD:
	        pAcgCmd = (acgTCommand *)buf;
	        absCh = pAcgCmd->Channel;

	        ret = getDspChannelByAbsChannel(absCh, &dspCh);
	        if (-1 == ret)
	            return ret;

	        Device = dspCh.device;
	        Channel = dspCh.channel;
			pPlaybackVoice = (acgTPlaybackData *)pAcgCmd->data;
			if(copy_from_user(	&PlaybackVoice,      /* copy the receive structure to kernel */
								pPlaybackVoice,
								sizeof(acgECoder)+sizeof(int)))
			{
				ret = -EFAULT;
				break;
			}
			playbackCoder = PlaybackVoice.Coder;
	        printk("\r\nplaybackCoder = [%d]\r\n", playbackCoder);
	        if((3 != playbackCoder) && (4 != playbackCoder))
	        {
	            printk("\r\nError: No Support [%d], only CODER__G711ALAW and CODER__G711MULAW\r\n", playbackCoder);
	            break;
	        }
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

			dsp_dev->dspDev[Device].SetupChannelAttr[Channel].CodecConfiguration.SamplingRate = SAMPLING_RATE__8_KHz;

			Ac49xCodecConfiguration(Device, Channel, &(dsp_dev->dspDev[Device].SetupChannelAttr[Channel].CodecConfiguration));

			ret  = Ac49xPlaybackNetworkStart(Device, Channel, playbackCoder, AC49X_ENDIAN_MODE);
#endif
			break;


 		case ACG_PLAYBACK_PLAY_VOICE_CMD:
			/* cast the pointer */
			pAcgCmd = (acgTCommand *)buf;
			pPlaybackVoice = (acgTPlaybackData *)pAcgCmd->data;

			if(copy_from_user(	&PlaybackVoice,      /* copy the receive structure to kernel */
								pPlaybackVoice,
								sizeof(acgTPlaybackData)))
			{
				ret = -EFAULT;
				break;
			}

	        absCh = pAcgCmd->Channel;
			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;

			Device = dspCh.device;
			Channel = dspCh.channel;

	        Ac49xPlaybackPlayVoice(Device, Channel, PlaybackVoice.buf , PlaybackVoice.size);

			break;


	    case ACG_PLAYBACK_PLAY_SILENCE_CMD:
	        /* cast the pointer */
	        pAcgCmd = (acgTCommand *)buf;
	        absCh = pAcgCmd->Channel;
	        ret = getDspChannelByAbsChannel(absCh, &dspCh);
	        if (-1 == ret)
	            return ret;
	        iSilence = *(int *)pAcgCmd->data;

	        Device = dspCh.device;
	        Channel = dspCh.channel;
	        ret  = Ac49xPlaybackPlaySilence(Device, Channel, iSilence);

	        break;


		case ACG_PLAYBACK_END_CMD:
			/* cast the pointer */
			pAcgCmd = (acgTCommand *)buf;
		   	absCh = pAcgCmd->Channel;
			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;

			Device = dspCh.device;
			Channel = dspCh.channel;
			ret  = Ac49xPlaybackEnd(Device, Channel, 1);

	   		break;


	 	case ACG_TIMESLOT_MANIPULATION_CMD:
			/* cast the pointer */
			pAcgCmd = (acgTCommand *)buf;
			pTimeSlotManipulation = (acgTTimeSlotManipulationAttr *)pAcgCmd->data;

			if(copy_from_user(	&TimeSlotManipulation,      /* copy the receive structure to kernel */
								pTimeSlotManipulation,
								sizeof(acgTTimeSlotManipulationAttr)))
			{
				ret = -EFAULT;
				break;
			}

	        absCh = pAcgCmd->Channel;
			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;

			Device = dspCh.device;
			Channel = dspCh.channel;

	        return Ac49xTimeSlotManipulationCommand(Device, &TimeSlotManipulation);

			break;

       	case START_PACKET_RECORDING:
            if(copy_from_user(&PacketRecordingStartStop, buf, sizeof(AC494_DSP_PacketRecordingStartStop_STCT))) {
              ret = -EFAULT;
              printk("copy_from_user err\r\n");
              break;
            }
            uilocal_ipPR=PacketRecordingStartStop.IPAddr;
            portPR = PacketRecordingStartStop.Port;
            ret = AC494_DSP_OK_E;
            break;

		/* download command - download a program to the dsp */
		case DOWNLOAD_CMD_E:
			/* allocate a page for the program */
			ret = 1;
			//break;//Alex we don't have firmware now, break here.
			order = (19 - PAGE_SHIFT > 0) ? 19 - PAGE_SHIFT : 0;
			temp_buf = (char *)__get_free_pages(GFP_KERNEL,order);


			/* cast the pointer */
			pProgramDownload = (AC494_DSP_ProgramDownload_STCT *)buf;
			Device = pProgramDownload->Device;

			printk("DSP device download : file size is %d..\n",pProgramDownload->fileSize);
			/* copy the program */
			if(copy_from_user(temp_buf,pProgramDownload->program, pProgramDownload->fileSize/*count - SIZE_OF_FLAG*/))
				ret = -EFAULT;
			else  {
	            printk("DSP device %d, download a %d channels DSP firmware......", Device, temp_buf[26] - 48);
                if(Device != 0) {
	                ret = Ac49xBoot(Device, temp_buf, 0, 0);
#if 1 /*540 == 4*/
           	        dsp_dev->dspDev[Device].numOfChannels = temp_buf[26] - 48;
#else /*new firmware ; in the firmware it says that it is a 4 channel but it is really a 2 channel*/
					dsp_dev->dspDev[Device].numOfChannels = 2;
#endif

                }
                else
				{
                    ClearDownloadStatus();
					error = sock_create(PF_INET, SOCK_DGRAM, 0, &pDspPRsock);
		        	if (error<0)
		        	{
		               	printk("Error during creation of socket; terminating\n");
		               	return -1;
		        	}
                    /*A.L 061218 - Init function should be called before the download process !!!*/
                    Ac49xInitDriver(CONTROL__DISABLE);
#if ( PACKET_RECORDING_ENABLE == 1 )
                   	printk("DSP packet recording init..\n");
					InitPacketRecordingInfo("10.16.2.10", 50000);
                  	Ac49xInitializePacketRecording(FlushPacketToNet);
                  	SetPacketRecordingStatus(CONTROL__ENABLE);
                   	mdelay(500);
#endif //( PACKET_RECORDING_ENABLE == 1 )

	        	    ret = Ac49xUserDef_Boot(Device, temp_buf, 0, 0);	/* boot the program to the dsp */
		            /*ac494dspStart();*/
		            dsp_dev->dspDev[Device].numOfChannels = temp_buf[26] - 48;
                }
            }

			dsp_dev->dspDev[Device].downloadStatus = 1; 	/* update the download flag to true */
            if (ret == PROGRAM_DOWNLOAD_STATUS__BOOT_STATUS_REGISTER_SUCCESS)
				printk("*****passed*****\r\n");
			else
				printk("failed, ret = %d\r\n", ret);


            free_pages((unsigned long)temp_buf,order); 	/* free the page */

			break;


		/* reset buffers descriptor command */
		case RESET_BUF_DES_E:

			/* check for init flag */
#if 0
			if(!(dsp_dev->initStatus)) {
				ret = AC494_DSP_NO_INIT_E;
				break;
			}
#endif

			/* reset the buffer descriptors */
			Ac49xResetBufferDescriptors();

			ret = AC494_DSP_OK_E;

			break;

		/* transmit command  - write buffer to the dsp buffer */

		case TRANSMIT_CMD_E:

#if 0
			/* check for init flag */
			if(!(dsp_dev->initStatus)) {
				ret = AC494_DSP_NO_INIT_E;
				break;
			}
#endif

			/* cast the pointer */
			pTransmitPacket = (AC494_DSP_Transmit_STCT *)buf;

#if 0
			printk("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n");
			for(i=0x7B7828; i<(0x7B7838); i+=4)
			{
				RegVal = Ac49xUserDef_EMIF_Read32(i);

				printk("~~~~~ Ac49xTransmitPacket, before kmalloc, address: %x, Value = %X\r\n", i, RegVal);
				//0x7b0000 (address in bytes) the value should be  EDE8BF3C  get 3CBFE8ED

			}
			printk("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n");
#endif


			/* allocate kernel buffer */
			if( !(temp_buf = kmalloc(pTransmitPacket->TxPacketParams.PacketSize,GFP_KERNEL)) )
			{
				ret = -EFAULT;
				break;

			}

#if 0
			printk("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n");
			for(i=0x7B7828; i<(0x7B7838); i+=4)
			{
				RegVal = Ac49xUserDef_EMIF_Read32(i);

				printk("~~~~~ Ac49xTransmitPacket, after kmalloc, address: %x, Value = %X\r\n", i, RegVal);
				//0x7b0000 (address in bytes) the value should be  EDE8BF3C  get 3CBFE8ED

			}
			printk("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n");
#endif

			absCh = pTransmitPacket->TxPacketParams.Channel;
			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
			{
				/* free the kernel buffer */
				kfree(temp_buf);
				break;
			}
			pTransmitPacket->TxPacketParams.Device = dspCh.device;
			pTransmitPacket->TxPacketParams.Channel = dspCh.channel;

			/* copy the user tx structure to kernel */
			if(copy_from_user(	&(TransmitPacket.TxPacketParams),      /* copy the receive structure to kernel */
								&(pTransmitPacket->TxPacketParams),
								sizeof(Tac49xTxPacketParams))) {
				ret = -EFAULT;
				/* free the kernel buffer */
				kfree(temp_buf);
				break;
			}

			/* copy the user buffer to kernel */
			if(copy_from_user(temp_buf,pTransmitPacket->TxPacketParams.pOutPacket,pTransmitPacket->TxPacketParams.PacketSize)) {
				ret = -EFAULT;
				/* free the kernel buffer */
				kfree(temp_buf);
			 	break;
            }

			/* points to the kernel buffer */
			TransmitPacket.TxPacketParams.pOutPacket = temp_buf;


			ret = Ac49xTransmitPacket(&(TransmitPacket.TxPacketParams));

#if 0
			printk("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n");
			for(i=0x7B7828; i<(0x7B7838); i+=4)
			{
				RegVal = Ac49xUserDef_EMIF_Read32(i);

				printk("~~~~~ Ac49xTransmitPacket, after, address: %x, Value = %X\r\n", i, RegVal);
				//0x7b0000 (address in bytes) the value should be  EDE8BF3C  get 3CBFE8ED
			}
			printk("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n");
#endif

            /* free the kernel buffer */
			kfree(temp_buf);

			break;

		/* setup device command */

		case SETUP_DEVICE_CMD_E:

            error = sock_create(PF_INET, SOCK_DGRAM, 0, &pDspPRsock);
            if (error<0)
            {
                   printk("Error during creation of socket; terminating\n");
                   return -1;
            }
			/* cast the pointer */
			pSetupDevice = (AC494_DSP_SetupDevice_STCT *)buf;
            Device = pSetupDevice->Device;

            if(Device == 0) {
                #if ( PACKET_RECORDING_ENABLE == 1 )
                       printk("DSP packet recording init..");
                       Ac49xInitializePacketRecording(FlushPacketToNet);
                       mdelay(500);
                       printk("passed\r\n");
                #endif //( PACKET_RECORDING_ENABLE == 1 )
            }

			/* copy the user structure to kernel */
			if(copy_from_user(	&(dsp_dev->dspDev[Device].SetupDeviceAttr),
								&(pSetupDevice->SetupDeviceAttr),
								sizeof(Tac49xSetupDeviceAttr))) {
				ret = -EFAULT;
				break;
			}
            printk("DSP device %d, SetupDevice...", Device);
            ret = Ac49xSetupDevice(Device, &(dsp_dev->dspDev[Device].SetupDeviceAttr));
			if(ret == SETUP_DEVICE_STATUS__SUCCESS)
			{
				dsp_dev->dspDev[Device].initStatus = 1; 	/* update init flag to true */
				mdelay(500);
	            printk("passed\r\n");
			}
			else
				printk("failed, ret = %d\r\n", ret);

			if(Device == 0) {
			    /* setup the device */
 			    Device = pSetupDevice->Device;

                if((*(volatile unsigned int *)(EMIF_4M_DSP_ALIGEND + 0x380000)) == 0)  {  /* (EMIF_4M_DSP_ALIGEND + 0x380000) = DSP start address at SDRAM */
            	    /* check that the firmware is ok (silicon version) */
            	    printk("<1>Incorrect silicon version\r\n");
				    ret = -EFAULT;
			    }
            }

            break;





		case SET_DEVICE_CMD_E:

			/* cast the pointer */
			pSetupDevice = (AC494_DSP_SetupDevice_STCT *)buf;
			Device = pSetupDevice->Device;

			/* copy the user structure to kernel */
			if(copy_from_user(	&(dsp_dev->dspDev[Device].SetupDeviceAttr),
								&(pSetupDevice->SetupDeviceAttr),
								sizeof(Tac49xSetupDeviceAttr))) {
				ret = -EFAULT;
				break;
			}

			break;


		case SET_CHANNEL_CMD_E:

            /* copy the user structure to kernel */
			pSetupChannel = (AC494_DSP_SetupChannel_STCT *)buf;
			absCh = pSetupChannel->Channel;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;
			Device = dspCh.device;
			Channel = dspCh.channel;

			/* copy the user structure to kernel */
			if((temp = copy_from_user(	&(dsp_dev->dspDev[Device].SetupChannelAttr[Channel]),
								&(pSetupChannel->SetupChannelAttr),
								sizeof(Tac49xSetupChannelAttr)))) {
				ret = -EFAULT;
				break;
			}
			else {
				ret = AC494_DSP_OK_E;
			}
			break;


		case DEFAULTS_DSP_CMD_E:

			pSetDefaults = (AC494_DSP_SetDefaults_STCT *)buf;
			Device = pSetDefaults->Device;

			/* check for download flag */
			if(!(dsp_dev->dspDev[Device].downloadStatus)) {
				ret = AC494_DSP_NO_DOWNLOAD_E;
				break;
			}

			/* set to run mode and defaults values */

			ret = dspDev_setDefaults(Device);

			break;


		/* codec configuration command */

		case CODEC_CNFG_CMD_E:


			/* copy the user structure to kernel */
			pCodecConfg = (AC494_DSP_CodecConfg_STCT *)buf;
			absCh = pCodecConfg->Channel;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;
			Device = dspCh.device;
			Channel = dspCh.channel;

			/* check for init flag */
			if(!(dsp_dev->dspDev[Device].initStatus)) {
				ret = AC494_DSP_NO_INIT_E;
				break;
			}


			if(copy_from_user(	&(dsp_dev->dspDev[Device].SetupChannelAttr[Channel].CodecConfiguration),
								&(pCodecConfg->Attr),
								sizeof(Tac49xCodecConfigurationAttr))) {
				ret = -EFAULT;
				break;

			}

			ret = Ac49xCodecConfiguration(Device, Channel, &(dsp_dev->dspDev[Device].SetupChannelAttr[Channel].CodecConfiguration));

			break;

		/* advance channel configuration command */

		case ADVANCE_CHANNEL_CNFG_CMD_E:

            /* copy the user structure to kernel */
			pAdvanceChannelConfg = (AC494_DSP_AdvanceChannelConfg_STCT *)buf;
			absCh = pAdvanceChannelConfg->Channel;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;
			Device = dspCh.device;
			Channel = dspCh.channel;

			/* check for init flag */
			if(!(dsp_dev->dspDev[Device].initStatus)) {
				ret = AC494_DSP_NO_INIT_E;
				break;
			}

			if(copy_from_user(	&(dsp_dev->dspDev[Device].SetupChannelAttr[Channel].Advanced),
								&(pAdvanceChannelConfg->Attr),
								sizeof(Tac49xAdvancedChannelConfigurationAttr))) {
				ret = -EFAULT;
                break;
			}

			/* set advance channel configuration */

			ret = Ac49xAdvancedChannelConfiguration(Device, Channel, &(dsp_dev->dspDev[Device].SetupChannelAttr[Channel].Advanced));

			break;


		case ECHO_CANCELER_CNFG_CMD_E:

            /* copy the user structure to kernel */
			pAchousticEchoCancelerConfg = (AC494_DSP_AcousticEchoCancelerConfiguration_STCT *)buf;
			Device = pAchousticEchoCancelerConfg->Device;
			absCh = pAchousticEchoCancelerConfg->Channel;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;
			Device = dspCh.device;
			Channel = dspCh.channel;

			/* check for init flag */
			if(!(dsp_dev->dspDev[Device].initStatus)) {
				ret = AC494_DSP_NO_INIT_E;
				break;
			}

			if(copy_from_user(	&(dsp_dev->dspDev[Device].SetupChannelAttr[Channel].AcousticEchoCancelerConfiguration),
								&(pAchousticEchoCancelerConfg->Attr),
								sizeof(Tac49xAcousticEchoCancelerConfigurationAttr))) {
				ret = -EFAULT;
                break;
			}

			/* set advance channel configuration */

			ret = Ac49xAcousticEchoCancelerConfiguration(Device, Channel, &(dsp_dev->dspDev[Device].SetupChannelAttr[Channel].AcousticEchoCancelerConfiguration));

			break;




        /* open channel command */

		case OPEN_CHANNEL_CMD_E:

            /* copy the user structure to kernel */
			pOpenChannelConfg = (AC494_DSP_OpenUpdateChannelConfg_STCT *)buf;
			absCh = pOpenChannelConfg->Channel;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;
			Device = dspCh.device;
			Channel = dspCh.channel;

			/* check for init flag */
			if(!(dsp_dev->dspDev[Device].initStatus)) {
				ret = AC494_DSP_NO_INIT_E;
				break;
			}

			if(copy_from_user(	&(dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel),
								&(pOpenChannelConfg->Attr),
								sizeof(Tac49xOpenOrUpdateChannelConfigurationAttr))) {
				ret = -EFAULT;
				break;
			}

			/* set open channel configuration */

			ret = Ac49xOpenChannelConfiguration(Device, Channel, &(dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel));

			RecordCommandst.Type	= RECORD_COMMAND_TYPE__NETWORK_START;
			RecordCommandst.Coder 	= CODER__G711ALAW;
			PacketRecordingStartStop.Device = Device;
			PacketRecordingStartStop.Channel = Channel;
			mdelay(100);
			/*Ac49xRecordCommand(PacketRecordingStartStop.Device, PacketRecordingStartStop.Channel, &RecordCommandst);*/
			break;

        /* update channel command */

		case UPDATE_CHANNEL_CMD_E:

			/* copy the user structure to kernel */
			pOpenChannelConfg = (AC494_DSP_OpenUpdateChannelConfg_STCT *)buf;
			absCh = pOpenChannelConfg->Channel;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;
			Device = dspCh.device;
			Channel = dspCh.channel;

			/* check for init flag */
			if(!(dsp_dev->dspDev[Device].initStatus)) {
				ret = AC494_DSP_NO_INIT_E;
				break;
			}

			if(copy_from_user(	&(dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel),
								&(pOpenChannelConfg->Attr),
								sizeof(Tac49xOpenOrUpdateChannelConfigurationAttr))) {
				ret = -EFAULT;
            	break;
			}

			/* set update channel configuration */

			ret = Ac49xUpdateChannelConfiguration(Device, Channel, &(dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel));

			break;

		/* active regular channel command */

		case ACTIVE_REGULAR_CHANNEL_CMD_E:


            /* copy the user structure to kernel */
			pActivateRegularRtp = (AC494_DSP_ActivateUpdateRegularRtpChannelConfg_STCT *)buf;
			absCh = pActivateRegularRtp->Channel;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;
			Device = dspCh.device;
			Channel = dspCh.channel;

			/* check for init flag */
			if(!(dsp_dev->dspDev[Device].initStatus)) {
				ret = AC494_DSP_NO_INIT_E;
				break;
			}

			if(copy_from_user(	&(dsp_dev->dspDev[Device].SetupChannelAttr[Channel].ActivateOrUpdateRtp),
								&(pActivateRegularRtp->Attr),
								sizeof(Tac49xActivateOrUpdateRtpChannelConfigurationAttr))) {
				ret = -EFAULT;
				break;
			}

			/* set active regular channel configuration */

			ret = Ac49xActivateRegularRtpChannelConfiguration(Device, Channel, &(dsp_dev->dspDev[Device].SetupChannelAttr[Channel].ActivateOrUpdateRtp[0]));

			break;

		/* update regular channel command */

		case UPDATE_REGULAR_CHANNEL_CMD_E:


			/* copy the user structure to kernel */
			pActivateRegularRtp = (AC494_DSP_ActivateUpdateRegularRtpChannelConfg_STCT *)buf;
			absCh = pActivateRegularRtp->Channel;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;
			Device = dspCh.device;
			Channel = dspCh.channel;

			/* check for init flag */
			if(!(dsp_dev->dspDev[Device].initStatus)) {
				ret = AC494_DSP_NO_INIT_E;
				break;
			}

			if(copy_from_user(	&(dsp_dev->dspDev[Device].SetupChannelAttr[Channel].ActivateOrUpdateRtp),
								&(pActivateRegularRtp->Attr),
								sizeof(Tac49xActivateOrUpdateRtpChannelConfigurationAttr))) {
				ret = -EFAULT;
				break;
			}

			/* set update regular channel configuration */

			ret = Ac49xUpdateRegularRtpChannelConfiguration(Device, Channel, &(dsp_dev->dspDev[Device].SetupChannelAttr[Channel].ActivateOrUpdateRtp[0]));

			break;

		/* send ibs string command */

		case SEND_IBS_STRING_CMD_E:


            /* copy the user structure to kernel */
			if(copy_from_user(&sendIbsString, buf, sizeof(AC494_DSP_SendIbsString_STCT))) {
				ret = -EFAULT;
                break;
			}

			absCh = sendIbsString.Channel;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;
			Device = dspCh.device;
			Channel = dspCh.channel;

			/* check for init flag */
			if(!(dsp_dev->dspDev[Device].initStatus)) {
				ret = AC494_DSP_NO_INIT_E;
				break;
			}

			/* send ibs string */
			ret = Ac49xSendIbsString(Device, Channel, &(sendIbsString.Attr));

			break;

		/* send ibs string command */

		case SEND_IBS_STOP_CMD_E:


            /* copy the user structure to kernel */
			if(copy_from_user(&sendExtendedIbsString, buf, sizeof(AC494_DSP_SendExtendedIbsString_STCT))) {
				ret = -EFAULT;
                break;
			}
			absCh  = sendExtendedIbsString.Channel;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;
			Device = dspCh.device;
			Channel = dspCh.channel;

			/* check for init flag */
			if(!(dsp_dev->dspDev[Device].initStatus)) {
				ret = AC494_DSP_NO_INIT_E;
				break;
			}


			ret = Ac49xSendIbsStop(Device, Channel);

			break;

		/* send ibs string command */

		case SEND_EXTENDED_IBS_STRING_CMD_E:


            /* copy the user structure to kernel */
			if(copy_from_user(&sendExtendedIbsString, buf, sizeof(AC494_DSP_SendExtendedIbsString_STCT))) {
				ret = -EFAULT;
                break;
			}

			absCh = sendExtendedIbsString.Channel;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;
			Device = dspCh.device;
			Channel = dspCh.channel;

			/* check for init flag */
			if(!(dsp_dev->dspDev[Device].initStatus)) {
				ret = AC494_DSP_NO_INIT_E;
				break;
			}

			/* send ibs string */

			ret = Ac49xSendExtendedIbsString(Device, Channel, &(sendExtendedIbsString.Attr));

			break;
#if defined(CONFIG_SDP_IPPHONE) || defined(CONFIG_ORCHID)
		case SET_CODEC_CMD_E:


            /* copy the user structure to kernel */
			if(copy_from_user(&codecDir, buf, sizeof(AC494_DSP_CODEC_DIR_STCT))) {
				ret = -EFAULT;
                break;
			}

			absCh = codecDir.Channel;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;
			Device = dspCh.device;
			Channel = dspCh.channel;

			/* check for init flag */
			if(!(dsp_dev->dspDev[Device].initStatus)) {
				ret = AC494_DSP_NO_INIT_E;
				break;
			}

			ret = AC494_DSP_OK_E;

			switch(codecDir.direction) {

				case AC494_CODEC_INPUT:

					switch(codecDir.user) {

						case AC494_CODEC_HANDSET:
							dsp_dev->dspDev[Device].SetupChannelAttr[Channel].CodecConfiguration.HandsetInputSelect = CONTROL__ENABLE;
        					break;
						case AC494_CODEC_HEADSET:
							dsp_dev->dspDev[Device].SetupChannelAttr[Channel].CodecConfiguration.HeadsetInputSelect = CONTROL__ENABLE;
        					break;
						case AC494_CODEC_MIC:
							dsp_dev->dspDev[Device].SetupChannelAttr[Channel].CodecConfiguration.MicrophoneInputSelect = CONTROL__ENABLE;
        					break;
						case AC494_CODEC_LINE:
							dsp_dev->dspDev[Device].SetupChannelAttr[Channel].CodecConfiguration.LineInputSelect = CONTROL__ENABLE;
        					break;
						default:
							ret = AC494_DSP_ERROR_E;
							break;
					}
					break;

				case AC494_CODEC_OUTPUT:

					switch(codecDir.user) {

						case AC494_CODEC_HANDSET:
							dsp_dev->dspDev[Device].SetupChannelAttr[Channel].CodecConfiguration.HandsetOutputSelect = CONTROL__ENABLE;
        					break;
						case AC494_CODEC_HEADSET:
							dsp_dev->dspDev[Device].SetupChannelAttr[Channel].CodecConfiguration.HeadsetOutputSelect = CONTROL__ENABLE;
        					break;
						case AC494_CODEC_SPEAKER:
							dsp_dev->dspDev[Device].SetupChannelAttr[Channel].CodecConfiguration.SpeakerSelect = CONTROL__ENABLE;
        					break;
						case AC494_CODEC_LINE:
							dsp_dev->dspDev[Device].SetupChannelAttr[Channel].CodecConfiguration.LineOutputSelect = CONTROL__ENABLE;
        					break;
						default:
							ret = AC494_DSP_ERROR_E;
							break;
					}
					break;

				case AC494_CODEC_LOOP_ON:

					switch(codecDir.user) {

						case AC494_CODEC_HANDSET:
							dsp_dev->dspDev[Device].SetupChannelAttr[Channel].CodecConfiguration.HandsetInputToOutput = CONTROL__ENABLE;
        					break;
						case AC494_CODEC_HEADSET:
							dsp_dev->dspDev[Device].SetupChannelAttr[Channel].CodecConfiguration.HeadsetInputToOutput = CONTROL__ENABLE;
        					break;
						default:
							ret = AC494_DSP_ERROR_E;
							break;
					}
					break;

				case AC494_CODEC_LOOP_OFF:

					switch(codecDir.user) {

						case AC494_CODEC_HANDSET:
							dsp_dev->dspDev[Device].SetupChannelAttr[Channel].CodecConfiguration.HandsetInputToOutput = CONTROL__DISABLE;
        					break;
						case AC494_CODEC_HEADSET:
							dsp_dev->dspDev[Device].SetupChannelAttr[Channel].CodecConfiguration.HeadsetInputToOutput = CONTROL__DISABLE;
        					break;
						default:
							ret = AC494_DSP_ERROR_E;
							break;
					}
					break;

				default:
					ret = AC494_DSP_ERROR_E;
					break;

			} /* end switch */


			break;
#endif

		case SET_3WAYCONF_CMD_E:


            /* copy the user structure to kernel */
			if(copy_from_user(&conf3way, buf, sizeof(AC494_DSP_ActivateOrDeactivate3WayConf_STCT))) {
				ret = -EFAULT;
                break;
			}

			absCh = conf3way.Channel;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;
			Device = dspCh.device;
			Channel = dspCh.channel;

			/*convert the conference channel from absolute values to dsp values*/
			ret = getDspChannelByAbsChannel(conf3way.Attr.ConferenceChannelMate, &dspCh);
			if (-1 == ret)
				return ret;
			conf3way.Attr.ConferenceChannelMate = dspCh.channel;

			/* check for init flag */
			if(!(dsp_dev->dspDev[Device].initStatus)) {
				ret = AC494_DSP_NO_INIT_E;
				break;
			}

			/* send ibs string */
			ret = Ac49xActivateOrDeactivate3WayConferenceConfiguration(Device, Channel, &(conf3way.Attr));

			break;


		case CLOSE_CHANNEL_CMD_E:


            /* copy the user structure to kernel */
			if(copy_from_user(&closeChannel, buf, sizeof(AC494_DSP_CloseChannel_STCT))) {
				ret = -EFAULT;
                break;
			}

			absCh = closeChannel.Channel;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;
			Device = dspCh.device;
			Channel = dspCh.channel;

			/* check for init flag */
			if(!(dsp_dev->dspDev[Device].initStatus)) {
				ret = AC494_DSP_NO_INIT_E;
				break;
			}

			ret = Ac49xCloseChannelConfiguration(Device, Channel);

			break;


		case MEM_MODIFY_CMD_E:

            /* copy the user structure to kernel */
			if(copy_from_user(&mm, buf, sizeof(AC494_MEM_MODIFY_STCT))) {
				ret = -EFAULT;
                break;
			}

			while((mm.count--)>0) {
            	*((unsigned int *)mm.addr) = mm.data;
				mm.addr+=4;
			}

			ret = AC494_DSP_OK_E;

			break;

		case MEM_DISPLAY_CMD_E:

            /* copy the user structure to kernel */
			if(copy_from_user(&md, buf, sizeof(AC494_MEM_DISPLAY_STCT))) {
				ret = -EFAULT;
                break;
			}

			printk("<1>\r\n");
			while((md.count--)>0) {
				printk("<1> %x : %x \r\n",md.addr ,*(unsigned int *)md.addr);
				md.addr+=4;
			}

			ret = AC494_DSP_OK_E;

			break;

		case MEM_CHECKSUM_CMD_E:

            /* copy the user structure to kernel */
			if(copy_from_user(&mc, buf, sizeof(AC494_MEM_CHECKSUM_STCT))) {
				ret = -EFAULT;
                break;
			}

			mc.checksum = 0;
			while((mc.count--)>0) {
				mc.checksum += *(unsigned int *)md.addr;
			}
			printk("<1>checksum = %x\r\n",mc.checksum);

			ret = AC494_DSP_OK_E;

			break;

		case SET_DSP_DEVICE_CONTROL_REGISTER_CMD_E:
			{
				/* cast the pointer */
				pSetupDevice = (AC494_DSP_SetupDevice_STCT *)buf;
				Device = pSetupDevice->Device;

				/* copy the user structure to kernel */
				if(copy_from_user(	&(dsp_dev->dspDev[Device].SetupDeviceAttr),
									&(pSetupDevice->SetupDeviceAttr),
									sizeof(Tac49xSetupDeviceAttr)))
				{
					ret = -EFAULT;
					break;
				}

				if (Device==0) //Reset Buffer descriptors only for Device 0
				{
					Ac49xResetBufferDescriptors();
					printk("DSP device %d called Ac49xResetBufferDescriptors.\n", Device);
				}
				ret = Ac49xDeviceSetControlRegister(Device, &(dsp_dev->dspDev[Device].SetupDeviceAttr.DeviceControlRegister));
			}
			break;

		/*A.L. 061031 T.38*/
		case T38_CHANNEL_CNFG_CMD_E:
			/*  copy the user structure to kernel */
			if (copy_from_user(&t38ChannelConfig, buf, sizeof(AC494_DSP_T38ChannelConfiguration_STCT)))
             {
			    ret = -EFAULT;
                break;
             }

			absCh = t38ChannelConfig.Channel;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;
			Device = dspCh.device;
			Channel = dspCh.channel;

			/* check for init flag */
			if(!(dsp_dev->dspDev[Device].initStatus)) {
				ret = AC494_DSP_NO_INIT_E;
				break;
			}
            /* activate fax relay */
			ret = Ac49xT38ChannelConfiguration(Device, Channel, &t38ChannelConfig.Attr);

            break;
		/*A.L. 061031 T.38*/
        case ACTIVE_FAX_RELAY_CHANNEL_CMD_E:
			/*printk("Debug dsp_drv :: ACTIVE_FAX_RELAY_CHANNEL_CMD_E\r\n");*/

			/*  copy the user structure to kernel */
			if (copy_from_user(&t38ChannelActivate, buf, sizeof(AC494_DSP_ActivateUpdateFaxRelayChannelConfg_STCT)))
             {
			    ret = -EFAULT;
                break;
             }

			absCh = t38ChannelActivate.Channel;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;
			Device = dspCh.device;
			Channel = dspCh.channel;

			/* check for init flag */
			if(!(dsp_dev->dspDev[Device].initStatus)) {
				ret = AC494_DSP_NO_INIT_E;
				break;
			}

			/*printk("Debug dsp_drv :: Device %d,Channel %d\r\n",Device,Channel);*/

			/* set t38 channel activate */

			ret = Ac49xActivateFaxRelayCommand(Device, Channel, &t38ChannelActivate.Attr);
			break;
		/*A.L. 061031 T.38*/
        case CLOSE_REGULAR_CHANNEL_CMD_E:
			/*printk("Debug dsp_drv :: CLOSE_REGULAR_CHANNEL_CMD_E\r\n");*/

            /* copy the user structure to kernel */
			if(copy_from_user(&closeRegularRtp, buf, sizeof(AC494_DSP_CloseRegularRtpChannelConfg_STCT))) {
				ret = -EFAULT;
                break;
			}

			absCh = closeRegularRtp.Channel;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;
			Device = dspCh.device;
			Channel = dspCh.channel;

			/* check for init flag */
			if(!(dsp_dev->dspDev[Device].initStatus)) {
				ret = AC494_DSP_NO_INIT_E;
				break;
			}

            ret = Ac49xCloseRegularRtpChannelConfiguration(Device, Channel, &closeRegularRtp.Attr);

            break;

	    case MANIPULATE_TIMESLOTS_CMD_E:

			/* cast the pointer */
			pManipulateTimeSlots = (AC494_DSP_TimeSlotsManipulation_STCT *)buf;

			/* allocate kernel buffer */
			temp_buf = kmalloc(sizeof(AC494_DSP_TimeSlotsManipulation_STCT),GFP_KERNEL);


			/* copy the user buffer to kernel */
			if(copy_from_user(temp_buf,	pManipulateTimeSlots,sizeof(AC494_DSP_TimeSlotsManipulation_STCT))) {
				ret = -EFAULT;
			 	break;
            }

            absCh = pManipulateTimeSlots->Channel;

			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;
			Device = dspCh.device;

			/* check for init flag */
			if(!(dsp_dev->dspDev[Device].initStatus)) {
				ret = AC494_DSP_NO_INIT_E;
				break;
			}

			ret = Ac49xTimeSlotManipulationCommand(Device, &(pManipulateTimeSlots->Attr));

            /* free the kernel buffer */
			kfree(temp_buf);

			break;

	    case VLYNQ_VDSP_INIT_CMD_E:

            /* copy the user structure to kernel */
			vlynq_dev_init_vdsp();
			ret = AC494_DSP_OK_E;

			break;

    	/* send caller id */
    	case SEND_CALLER_ID:

            /* copy the user structure to kernel */
			if(copy_from_user(&sendCallerId, buf, sizeof(AC494_DSP_CallerId_STCT))) {
				ret = -EFAULT;
                break;
			}

            absCh = sendCallerId.Channel;
			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;
			Device = dspCh.device;

            ret = Ac49xSendCallerIdMessage(Device, sendCallerId.Channel, &sendCallerId.CallerIdMessage,	&sendCallerId.CallerIdMessage);

			break;


    	/* stop caller id */
    	case STOP_CALLER_ID:

            /* copy the user structure to kernel */
			if(copy_from_user(&sendCallerId, buf, sizeof(AC494_DSP_CallerId_STCT))) {
				ret = -EFAULT;
                break;
			}

            absCh = sendCallerId.Channel;
			ret = getDspChannelByAbsChannel(absCh, &dspCh);
			if (-1 == ret)
				return ret;
			Device = dspCh.device;

			ret = Ac49xSendCallerIdStop(Device, sendCallerId.Channel);

			break;


    	default:
    		ret = AC494_DSP_ERROR_E;
    		break;

	} 	/* end switch */

	return ret;

} /* end of dspDev_write() */
/******************************************************************************/



/****************************************************************************
*
*	Name:		dspDev_setDefaults
*----------------------------------------------------------------------------
*	Abstract:	set the dsp device and channels to defaults values
*----------------------------------------------------------------------------
*	Input:		none
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	case of success = AC494_DSP_OK_E
******************************************************************************/

int dspDev_setDefaults(int Device)
{
	int channel;

	/* get device defaults */
	Ac49xSetDefaultSetupDeviceAttr(&(dsp_dev->dspDev[Device].SetupDeviceAttr));

	dsp_dev->dspDev[Device].SetupDeviceAttr.Open.NumberOfChannels = dsp_dev->dspDev[Device].numOfChannels;

	/* set the channels over the device */
	for(channel=0; channel< (dsp_dev->dspDev[Device].numOfChannels) ; channel++)
	{
		dsp_dev->dspDev[Device].SetupDeviceAttr.Open.Channel[channel].Slot = channel;
		dsp_dev->dspDev[Device].SetupDeviceAttr.Open.Channel[channel].InterconnectedSlot = channel+4;
	}

	/* set channels defaults */
	for(channel=0; channel< (dsp_dev->dspDev[Device].numOfChannels) ; channel++)
		Ac49xSetDefaultSetupChannelAttr(&(dsp_dev->dspDev[Device].SetupChannelAttr[channel]));

	for(channel=0; channel< (dsp_dev->dspDev[Device].numOfChannels) ; channel++) {
		dsp_dev->dspDev[Device].SetupChannelAttr[channel].OpenOrUpdateChannel.DetectCallProgress				= CONTROL__ENABLE;
		dsp_dev->dspDev[Device].SetupChannelAttr[channel].OpenOrUpdateChannel.DetectDTMFRegister				= CONTROL__ENABLE;
	}

#if 0
	/* by default connect device 0 channel 0 to the handset */
	dsp_dev->dspDev[Device/*0*/].SetupChannelAttr[Device/*0*/].CodecConfiguration.HandsetInputToOutput				= CONTROL__ENABLE;
	dsp_dev->dspDev[Device/*0*/].SetupChannelAttr[Device/*0*/].CodecConfiguration.HandsetInputSelect				= CONTROL__ENABLE;
	dsp_dev->dspDev[Device/*0*/].SetupChannelAttr[Device/*0*/].CodecConfiguration.HandsetOutputSelect				= CONTROL__ENABLE;
#endif
	Ac49xResetDeviceBufferDescriptors(Device); //0);


	return AC494_DSP_OK_E;

} /* end of dspDev_setDefaults() */
/******************************************************************************/

/*********************************************************************************************/

static long dspDev_ioctl(struct file *fp, unsigned int cmd, unsigned long arg)
{
	return 0;
}


/*********************************************************************************************/

static int dspDev_open( 	struct inode 	*inode,
							struct file 	*file )
{
	return 0;
}

/*********************************************************************************************/

static int dspDev_release( 	struct inode 	*inode,
							struct file 	*file )
{
	int Device;
	int Channel;

	for(Device = 0 ; Device < AC49X_NUMBER_OF_DEVICES ; Device++)
	{
		for(Channel = 0 ; Channel < AC49X_NUMBER_OF_CHANNELS ; Channel++)
		{
			dspChannelsAttributes[Channel + AC49X_NUMBER_OF_CHANNELS*Device].last_digit_pressed = -1;

			isActivateRTP[Device][Channel] = RTP_IS_DEACTIVE;
		}
	}


	return 0;
}
/*********************************************************************************************/

int dspDev_fasync(	int 		fd,
					struct file *file,
					int 		mode)
{
	return 0;
}

/*********************************************************************************************/
/* Interface changed in v. 2.6.18 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,18)
static int dspDev_flush(struct file *file, fl_owner_t id)
#else
static int dspDev_flush(struct file *file)
#endif
{
	return 0;
}

/*********************************************************************************************/


/* set the device operations to the functions pointers */
struct file_operations dspDev_fops = {
	owner:  THIS_MODULE,
	read:   dspDev_read,
	write:	dspDev_write,
	unlocked_ioctl:	dspDev_ioctl,
	fasync: dspDev_fasync,
	flush:  dspDev_flush,
	open:	dspDev_open,
	release:  dspDev_release
};

static void timer_func(unsigned long data)
{

    dsp_dev->timer.expires += (HZ/100);
	add_timer(&dsp_dev->timer);

	dsp_dev->get_media_cond = 1;
	wake_up_interruptible(&dsp_dev->get_media_wait);
	dsp_dev->get_event_cond = 1;
	wake_up_interruptible(&dsp_dev->get_event_wait);
}

/****************************************************************************
*
*	Name:		dspDev_init
*----------------------------------------------------------------------------
*	Abstract:		initialize the device (call with insmod)
*----------------------------------------------------------------------------
*	Input:		none
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	case of success = 0, else -1
******************************************************************************/
int __init	dspDev_init(void)
{

	char devname[10];
	int  	Device, Channel;
//linux 2.6
	int	retval;
	dev_t   dev_id;

	/* allocate space to the device structure */
	dsp_dev = kmalloc(sizeof(DSP_DEV), GFP_KERNEL);

    	memset(dsp_dev, 0, sizeof(DSP_DEV));

	baseAddr_dspDev = (unsigned int)dsp_dev;
	size_dspDev		= (int)sizeof(DSP_DEV);

    	dsp_dev->dspDev[0].dspCashSegment = CASH_SEG_B;
	(*(volatile int* )(0xA5080014)) = dsp_dev->dspDev[0].dspCashSegment;
	dsp_dev->dspDev[0].dspCashSegment = (dsp_dev->dspDev[0].dspCashSegment << 22) & 0xFFFC0000;

	printk("<1>dsp device 0 memory at block %x\r\n",dsp_dev->dspDev[0].dspCashSegment);

#if/*def*/ 0/*CONFIG_DEVFS_FS*/
	devfs_dir_handle = devfs_mk_dir(NULL, "dsp", NULL);
	sprintf(devname, "0");
	dsp_dev->handle=devfs_register(devfs_dir_handle, devname, DEVFS_FL_AUTO_DEVNUM, 0, 0,
		S_IFCHR | S_IRUGO | S_IWUGO, &dspDev_fops,NULL);
#else
	dsp_dev->major=DSP_MAJOR;
	 if (dsp_dev->major) {
		 dev_id = MKDEV(dsp_dev->major, 0);
		 retval = register_chrdev_region(dev_id, DSP_COUNT, devname);
	 } else {
		 retval = alloc_chrdev_region(&dev_id, 0, DSP_COUNT, devname);
		 dsp_dev->major = MAJOR(dev_id);
	 }

	if (retval) {
		printk(KERN_ERR "%s: can't get dsp major! %d\n", devname, dsp_dev->major);
		return -1;
	}

    	printk(KERN_DEBUG "dsp: base=%#x major=%d\n", baseAddr_dspDev, dsp_dev->major);

    	cdev_init(&dsp_cdev, &dspDev_fops);
    	cdev_add(&dsp_cdev, dev_id, DSP_COUNT);

#endif

	SetPacketRecordingStatus(CONTROL__DISABLE);

	for(Device = 0 ; Device < AC49X_NUMBER_OF_DEVICES ; Device++)
	{
		for(Channel = 0 ; Channel < AC49X_NUMBER_OF_CHANNELS ; Channel++)
		{
			dspChannelsAttributes[Channel + AC49X_NUMBER_OF_CHANNELS*Device].last_digit_pressed = -1;

			isActivateRTP[Device][Channel] = RTP_IS_DEACTIVE;
		}

		dsp_dev->dspDev[Device].downloadStatus = 0;
		dsp_dev->dspDev[Device].initStatus = 0;
	}

/*Max outline*/
//A.L 070808 - This define interfere IP Phone (cause the LCD not to work well). Need to check it
#if !defined CONFIG_SDP_IPPHONE && !defined CONFIG_SDP_IPPHONE
#if defined CONFIG_SMB || defined CONFIG_TULIP2_40 || defined CONFIG_TULIP2_21
//#if (MPI_FOR_SMB == CONFIG_SMB)
			{
				int i;
            /* copy the user structure to kernel */
	    		/*reset ac498*/
	    		*(volatile unsigned int*)(KSEG1ADDR(0x0861162C)) |= 0x0000C000;
	    		mb();
			    /*IO direction (reset ac498)*/
	    		*(volatile unsigned int*)(KSEG1ADDR(0x08610910)) &= 0xF7FFFFFF;
	    		mb();
				/*GPIO ENABLE (reset ac498)*/
	    		*(volatile unsigned int*)(KSEG1ADDR(0x08610918)) |= 0x08000000;
	    		mb();
			    /*Reset ac498 (0)*/
	 		   *(volatile unsigned int*)(KSEG1ADDR(0x08610908)) &= 0xF7FFFFFF;
	    		mb();
	    		for ( i = 0; i < 62400000 ; i++);
	   			/*Reset ac498 (1)*/
	    		*(volatile unsigned int*)(KSEG1ADDR(0x08610908)) |= 0x08000000;
	    		mb();
				for ( i = 0; i < 100000/2 ; i++);			//0.5 second delay before accept command
			}
#else

#if defined(CONFIG_ORCHID) || defined(CONFIG_ORCHID_ATA)

#if defined (CONFIG_ORCHID_MAIN)
#warning "Orchid MAIN: Set the PINSEL"
   /* ACL_sysGpioFuncSet(10, 22, 2); /*GPIO27*/
  /*  PAL_sysGpioCtrl(27, GPIO_PIN, GPIO_OUTPUT_PIN);
    PAL_sysGpioOutBit(27, 0);
    vlynq_delay_wait(20000);
    PAL_sysGpioOutBit(27, 1);*/
#else
#warning "Orchid ET: Set the PINSEL"
    /*ACL_sysGpioFuncSet(10, 10, 2); /*GPIO21*/
    /*PAL_sysGpioCtrl(21, GPIO_PIN, GPIO_OUTPUT_PIN);
    PAL_sysGpioOutBit(21, 0);
    vlynq_delay_wait(20000);
    PAL_sysGpioOutBit(21, 1);*/
#endif

#endif

#endif
#endif
   	init_waitqueue_head(&dsp_dev->get_media_wait);
   	init_waitqueue_head(&dsp_dev->get_event_wait);

    init_timer(&dsp_dev->timer);
//	dsp_dev->timer.data = (unsigned long) dsp_dev;
	dsp_dev->timer.function = timer_func;

    spin_lock_init(&dsp_dev->lock);

    dsp_dev->timer.expires = (HZ/100) + jiffies;

	add_timer(&dsp_dev->timer);

	return (0);

} /* end of dspDev_init() */
/******************************************************************************/







void dspDev_exit(void)
{

	dev_t dev_id = MKDEV(dsp_dev->major, 0);

    del_timer(&dsp_dev->timer);
#if/*def*/ 0/*CONFIG_DEVFS_FS*/
        devfs_unregister(devfs_dir_handle);
#else
	cdev_del(&dsp_cdev);
	unregister_chrdev_region(dev_id, DSP_COUNT);
#endif
	kfree(dsp_dev);
}

/********************************************************************************************************************/
/********************************************************************************************************************/


int dspDev_PRSendoSocket(char* buff, int len)
{

    struct msghdr msg;
    mm_segment_t oldfs;
    struct iovec iov;
    struct sockaddr_in sin;
    struct sockaddr_in *addr;
    //in_addr_t addrPR;

    addr=&sin;

    //addrPR=inet_addr(local_ipPR);
    //addrPR.s_addr=0xAC106401;//172.16.100.1

    /* Build structure with destination address */
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = in_aton( recordingRemoteIpAddress );//addrPR.s_addr;
    sin.sin_port = htons( (unsigned short)recordingRemotePort );

    msg.msg_name = addr;
    if (addr)
      msg.msg_namelen = sizeof(struct sockaddr_in);
    else
      msg.msg_namelen = 0;

    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = NULL;
    msg.msg_controllen = 0;
    msg.msg_flags = MSG_NOSIGNAL;
    msg.msg_iov->iov_len = len;
    msg.msg_iov->iov_base = buff;

    oldfs = get_fs();
    set_fs(KERNEL_DS);
    len = sock_sendmsg(pDspPRsock, &msg, len);
    set_fs(oldfs);

    return len;
}

void ACL_sysGpioFuncSet(int pinSelReg,   int shift,     int func)
{
	unsigned int temp;
	unsigned int addr;
#if defined(CONFIG_ORCHID)||defined(CONFIG_ORCHID_ATA)
	addr = ARIES_IOMUX_PIN_SEL_1 + (4 * (pinSelReg-1));
#else
	addr = TITAN_IOMUX_PIN_SEL_1 + (4 * (pinSelReg-1));
#endif
	temp = *(volatile unsigned int *)addr;
	temp &= ~(0x3 << shift);
	temp |= (func << shift);
	*(volatile unsigned int *)addr = temp;
}


void  vlynq_dev_init_vdsp(void)
{
    volatile unsigned int *reset_base = (unsigned int *) AVALANCHE_RESET_CONTROL_BASE;

    printk("starting vlynq_vdsp configuration...");

	*reset_base &= ~((1 << AVALANCHE_LOW_VLYNQ_RESET_BIT) | (1 << AVALANCHE_HIGH_VLYNQ_RESET_BIT));

    vlynq_delay_wait(20000);

    /* Bring vlynq out of reset if not already done */

	/* by itzik - going to high */
    *reset_base |= /* (1 << AVALANCHE_HIGH_VLYNQ_RESET_BIT); */ (1 << AVALANCHE_LOW_VLYNQ_RESET_BIT);  /* | (1 << AVALANCHE_HIGH_VLYNQ_RESET_BIT); */
    vlynq_delay_wait(20000); /* Allowing sufficient time to VLYNQ to settle down.*/

#if defined(CONFIG_ORCHID) || defined(CONFIG_ORCHID_ATA)
#if defined (CONFIG_ORCHID_MAIN)
#warning "Orchid MAIN: Set the PINSEL"
  /*  ACL_sysGpioFuncSet(10, 22, 2); /*GPIO27*/
     /*PAL_sysGpioCtrl(27, GPIO_PIN, GPIO_OUTPUT_PIN);
    PAL_sysGpioOutBit(27, 0);
    vlynq_delay_wait(20000);
    PAL_sysGpioOutBit(27, 1);*/
#else
#warning "Orchid: Set the PINSEL"
     /*ACL_sysGpioFuncSet(10, 10, 2); /*GPIO21*/
     /*PAL_sysGpioCtrl(21, GPIO_PIN, GPIO_OUTPUT_PIN);
    PAL_sysGpioOutBit(21, 0);
    vlynq_delay_wait(20000);
    PAL_sysGpioOutBit(21, 1);*/
#endif
#else

	/* by itzik - going to high */
    PAL_sysGpioCtrl(VLYNQ0_RESET_GPIO_NUM,1,0);
    vlynq_delay_wait(20000);

	/* by itzik - going to high */
    PAL_sysGpioOutBit(VLYNQ0_RESET_GPIO_NUM, 1);
    vlynq_delay_wait(50000);
	/* by itzik - going to high */
    PAL_sysGpioOutBit(VLYNQ0_RESET_GPIO_NUM, 0);
    vlynq_delay_wait(50000);
#endif

    PAL_sysVlynqInit();
	AcVlynq_Init_SoC_VlynqHi__to__AC498_VlynqLow();

#if defined(CONFIG_SMB) || defined(CONFIG_ORCHID_ATA)
	AcVlynq_Init_AC498_VlynqHi__to__AC498_VlynqLow(0);

	AcVlynq_Init_AC498_VlynqHi__to__AC498_VlynqLow(1);
#endif
	AcVlynq_WaitForVlynqLinkStatus();


	AcVlynq_MapToAc498LocalMemory();
   	AcVlynq_MapToAc498Registers();

	AcVlynq_WaitForVlynqLinkStatus();

	printk("passed\r\n");

#if 0
   /* vlynq_delay_wait(20000);
	PAL_sysGpioCtrl(27, GPIO_PIN, GPIO_OUTPUT_PIN);
    PAL_sysGpioOutBit(27, 0);//logic low
    vlynq_delay_wait(20000);
    PAL_sysGpioOutBit(27, 1);//logic high
    vlynq_delay_wait(20000);*/
#endif



}







void AcVlynq_Init_SoC_VlynqHi__to__AC498_VlynqLow(void)
{

    REGION_CONFIG_T *p_region = &region_config_AC498_1st_LowRxMap_ForRegisters[0];
	IRQ_CONFIG_T	*p_irq = &irq_config1[0];
    char dev_name[] = "AC49Soc";
    int instance = 0;


    /* printk("Setting up the SoC VLYNQ-HI configuration.\n")*/;

    vlynq_config.on_soc             = TRUE;


/* by itzik - going to high */
    /* vlynq_config.base_addr          = AVALANCHE_HIGH_VLYNQ_CONTROL_BASE; */


#if (AC49X_VLYNQ_PORT == AC49X_VLYNQ_PORT__3_HI)
    vlynq_config.base_addr          = AVALANCHE_HIGH_VLYNQ_CONTROL_BASE;
#else
    vlynq_config.base_addr          = AVALANCHE_LOW_VLYNQ_CONTROL_BASE;
#endif


    vlynq_config.local_clock_div    = 0x5;
    vlynq_config.local_clock_dir    = pal_vlynq_clk_out;
    vlynq_config.local_intr_local   = 0x1;
    vlynq_config.local_intr_vector  = 31;
    vlynq_config.local_intr_enable  = 1;
    vlynq_config.local_int2cfg      = 1;
    vlynq_config.local_intr_pointer = 0x08612400;
    vlynq_config.local_endianness   = pal_vlynq_little_en;

	/* by itzik - going to high */
#if 0
    vlynq_config.local_tx_addr      = PHYS_ADDR(AVALANCHE_HIGH_VLYNQ_MEM_MAP_BASE);
	printk("vlynq_config.local_tx_addr = %x\r\n",vlynq_config.local_tx_addr);
#endif


#if (AC49X_VLYNQ_PORT == AC49X_VLYNQ_PORT__3_HI)
    vlynq_config.local_tx_addr      = 0x40000000; /*PHYS_ADDR(AVALANCHE_HIGH_VLYNQ_MEM_MAP_BASE);*/
#else
    vlynq_config.local_tx_addr      = PHYSADDR(AVALANCHE_LOW_VLYNQ_MEM_MAP_BASE);  /*PHYS_ADDR(AVALANCHE_LOW_VLYNQ_MEM_MAP_BASE);*/
#endif



    vlynq_config.peer_clock_div     = 0x2;
    vlynq_config.peer_clock_dir     = pal_vlynq_clk_in;
    vlynq_config.peer_intr_local    = 0x0;
    vlynq_config.peer_intr_vector   = 30;
    vlynq_config.peer_intr_enable   = 1;
    vlynq_config.peer_int2cfg       = 0;
    vlynq_config.peer_endianness    = pal_vlynq_little_en;

	/* by itzik - going to high */
	/* vlynq_config.peer_tx_addr       = AC498_VLYNQ_ADDRESS_SPACE_HI;  */
    vlynq_config.peer_tx_addr       = AC498_VLYNQ_ADDRESS_SPACE_LOW;

    p_vlynq[0] = PAL_vlynqInit(&vlynq_config);
 /*   Bring494VlynqOutOfReset();*/
    if(!p_vlynq[0])
        {
        printk("SoC Failed to initialize the vlynq @ 0x%08x.\n",vlynq_config.base_addr);
        printk("error msg = %s.\n",vlynq_config.error_msg);
        goto av_vlynq_init_fail;
        }
    p_vlynq_dev[0] = PAL_vlynqDevCreate(p_vlynq[0], dev_name, instance, 0, 1);
    if(!p_vlynq_dev[0])
        {
        printk("Failed to create the %s%d reference for vlynq.\n",dev_name, instance);
        goto av_vlynq_dev_fail;
        }
    if(PAL_vlynqAddDevice(p_vlynq[0], p_vlynq_dev[0], 1))
        {
        printk("Failed to add %s%d reference into vlynq.\n",dev_name, instance);
        goto av_vlynq_add_device_fail;
        }
    while(p_region->id > -1)
        {
        if(PAL_vlynqMapRegion(p_vlynq[0], p_region->remote, p_region->id, p_region->offset, p_region->size, p_vlynq_dev[0]))
            {
            p_region->id = -1;
            printk("Failed to map regions for %s%d in vlynq.\n", dev_name, instance);
            goto av_vlynq_map_region_fail;
            }
        p_region++;
        }
    /*
    while(p_irq->hw_intr_line > -1)
        {
        if(PAL_vlynqMapIrq(p_vlynq[0], p_irq->hw_intr_line, p_irq->irq, p_vlynq_dev[0]))
            {
            p_irq->hw_intr_line = -1;
            printf("Failed to map interrupts for %s%d in vlynq.\n", dev_name, instance);
            goto av_vlynq_map_irq_fail;
            }
        p_irq++;
        }
    if(AC49SoC_vlynq_irq_cfg())
        {
        printf("Failed to set the irq on peer vlynq.\n");
        goto av_vlynq_set_irq_fail;
        }
        */
    /* printk("*******************************************   Success in setting the VLYNQ **************************.\n")*/;
    p_root_vlynq = p_vlynq[0];
    {
        unsigned int base_addr;
        if(!PAL_vlynqGetDevBase(p_vlynq[0], 0x0, &base_addr, p_vlynq_dev[0]))
        {
        /*printk("The base address is %x.\n", 0xa0000000 | base_addr)*/;
        }
    }
    return;


/*************************************/

av_vlynq_set_irq_fail:
av_vlynq_map_irq_fail:

    p_irq = &irq_config1[0];
    while(p_irq->hw_intr_line > -1)
        PAL_vlynqUnMapIrq(p_vlynq[0], p_irq->irq, p_vlynq_dev[0]);


/*************************************/
av_vlynq_map_region_fail:
    p_region = &region_config_AC498_1st_LowRxMap_ForHpi[0];
    while(p_region->id > -1)
        PAL_vlynqUnMapRegion(p_vlynq[0], p_region->remote, p_region->id, p_vlynq_dev[0]);

/*************************************/
av_vlynq_add_device_fail:
    #if 0
        pal_vlynq_dev_destroy(p_vlynq_dev);
    #endif

/*************************************/
av_vlynq_dev_fail:
    PAL_vlynqCleanUp(p_vlynq_dev);

/*************************************/
av_vlynq_init_fail:
	return;

}





void AcVlynq_MapToAc498Registers(void)
{
    REGION_CONFIG_T *p_region_ForHpi;
    REGION_CONFIG_T *p_region_ForRegisters;
    int Ac498Device;

    for(Ac498Device=0; Ac498Device<=AC49X_NUMBER_OF_DEVICES-2; Ac498Device++)
        {
        switch(Ac498Device)
            {
            case 0:
                p_region_ForHpi         = &region_config_AC498_1st_LowRxMap_ForHpi[0];
                p_region_ForRegisters   = &region_config_AC498_1st_LowRxMap_ForRegisters[0];
                break;
            case 1:
                p_region_ForHpi         = &region_config_AC498_2nd_LowRxMap_ForHpi[0];
                p_region_ForRegisters   = &region_config_AC498_2nd_LowRxMap_ForRegisters[0];
                break;
            case 2:
                p_region_ForHpi         = &region_config_AC498_3rd_LowRxMap_ForHpi[0];
                p_region_ForRegisters   = &region_config_AC498_3rd_LowRxMap_ForRegisters[0];
                break;
            default:
                return;
            }
        while(p_region_ForRegisters->id > -1)
            {
            PAL_vlynqUnMapRegion(p_vlynq[Ac498Device], p_region_ForHpi->remote, p_region_ForHpi->id, p_vlynq_dev[Ac498Device]);
            if(PAL_vlynqMapRegion(p_vlynq[Ac498Device], p_region_ForRegisters->remote, p_region_ForRegisters->id, p_region_ForRegisters->offset, p_region_ForRegisters->size, p_vlynq_dev[Ac498Device]))
                {
                p_region_ForRegisters->id = -1;
                printk("Failed to remap regions for device%d.\n", Ac498Device);
                }
            p_region_ForHpi++;
	        p_region_ForRegisters++;
            }
        }
}


void AcVlynq_MapToAc498LocalMemory(void)
{
    REGION_CONFIG_T *p_region_ForHpi;
    REGION_CONFIG_T *p_region_ForRegisters;
    int Ac498Device;

    for(Ac498Device=AC49X_NUMBER_OF_DEVICES-2; Ac498Device>=0; Ac498Device--)
        {
        switch(Ac498Device)
            {
            case 0:
                p_region_ForHpi         = &region_config_AC498_1st_LowRxMap_ForHpi[0];
                p_region_ForRegisters   = &region_config_AC498_1st_LowRxMap_ForRegisters[0];
                break;
            case 1:
                p_region_ForHpi         = &region_config_AC498_2nd_LowRxMap_ForHpi[0];
                p_region_ForRegisters   = &region_config_AC498_2nd_LowRxMap_ForRegisters[0];
                break;
            case 2:
                p_region_ForHpi         = &region_config_AC498_3rd_LowRxMap_ForHpi[0];
                p_region_ForRegisters   = &region_config_AC498_3rd_LowRxMap_ForRegisters[0];
                break;
            default:
                return;
            }
        while(p_region_ForHpi->id > -1)
            {
            PAL_vlynqUnMapRegion(p_vlynq[Ac498Device], p_region_ForRegisters->remote, p_region_ForRegisters->id, p_vlynq_dev[Ac498Device]);
            if(PAL_vlynqMapRegion(p_vlynq[Ac498Device], p_region_ForHpi->remote, p_region_ForHpi->id, p_region_ForHpi->offset, p_region_ForHpi->size, p_vlynq_dev[Ac498Device]))
                {
                p_region_ForHpi->id = -1;
                printk("Failed to remap regions for device%d.\n", Ac498Device);
                }
            p_region_ForHpi++;
	    p_region_ForRegisters++;
            }
        }
}

U32 AcVlynq_GetAc498BaseAddress(int Device)
{
    return Ac498BaseAddress[Device];
}


U32  AcVlynq_WaitForVlynqLinkStatus(void)
{
    int i;
//    volatile unsigned long *pVlynqStatusReg;
    volatile unsigned long *pResetDisableReg = (volatile unsigned long*)AVALANCHE_RESET_CONTROL_BASE;

    if((*pResetDisableReg&0x8000)==0)
        return -1;

    vlynq_delay_wait(200000);
    for(i=0; i<AC49X_NUMBER_OF_DEVICES-1; i++)
        {
        if(!PAL_vlynqGetLinkStatus(p_vlynq[i]))
            return -1;
        }
    return 0;
}


int AcVlynq_494UpadateVlynqClk() /*rais the clk to 50 MHz by changing the local DIV value*/
{
    return PAL_vlynqConfigClock(p_vlynq[0], pal_vlynq_clk_out, pal_vlynq_clk_in, 2, 1);
}





void AcVlynq_Init_AC498_VlynqHi__to__AC498_VlynqLow(int Ac498Device)
{

    REGION_CONFIG_T *p_region;
    IRQ_CONFIG_T    *p_irq      = &irq_config2[0];
    int             instance    = Ac498Device+1;
	char 			*dev_name;


   /* printk("Setting up the SoC ac498 configuration. %d\n",Ac498Device)*/;

	switch(Ac498Device) {

		case 0:
    		dev_name = dev_name_AC498_0;
			break;
		case 1:
    		dev_name = dev_name_AC498_1;
			break;
		case 2:
    		dev_name = dev_name_AC498_2;
			break;
        default:
            return;
	}

    switch(Ac498Device)
        {
        case 0:
            p_region = &region_config_AC498_2nd_LowRxMap_ForRegisters[0];
            break;
        case 1:
            p_region = &region_config_AC498_3rd_LowRxMap_ForRegisters[0];
            break;
        default:
            return;
        }


    vlynq_config.on_soc             = FALSE;
    vlynq_config.base_addr          = Ac498BaseAddress[Ac498Device]+offsetof(Tac498MemoryMap, VLYNQ1Control[0])-offsetof(Tac498MemoryMap, McBSP0[0]);


    vlynq_config.local_clock_div    = 0x1;
    vlynq_config.local_clock_dir    = pal_vlynq_clk_out;
    vlynq_config.local_intr_local   = 0x1;
    vlynq_config.local_intr_vector  = 29;
    vlynq_config.local_intr_enable  = 1;
    vlynq_config.local_int2cfg      = 0;
    vlynq_config.local_intr_pointer = 0;
    vlynq_config.local_endianness   = pal_vlynq_little_en;
    vlynq_config.local_tx_addr      = AC498_VLYNQ_ADDRESS_SPACE_HI;

    vlynq_config.peer_clock_div     = 0x1;
    vlynq_config.peer_clock_dir     = pal_vlynq_clk_in;
    vlynq_config.peer_intr_local    = 0x0;
    vlynq_config.peer_intr_vector   = 28;
    vlynq_config.peer_intr_enable   = 1;
    vlynq_config.peer_int2cfg       = 0;
    vlynq_config.peer_endianness    = pal_vlynq_little_en;
    vlynq_config.peer_tx_addr       = AC498_VLYNQ_ADDRESS_SPACE_LOW;

    p_vlynq[Ac498Device+1] = PAL_vlynqInit(&vlynq_config);

    if(!p_vlynq[Ac498Device+1])
        {
        printk("498 Failed to initialize the vlynq @ 0x%08x.\n", vlynq_config.base_addr);
        printk("Error Message: %s.\n", vlynq_config.error_msg);
        goto av_vlynq_init_fail;
        }
    if(!PAL_vlynqChainAppend(p_vlynq[Ac498Device+1], p_vlynq[Ac498Device]))
        /* printk("Appended the chain.\n")*/;
    else
        {
        printk("Failed to add to the chain.\n");
        return;
        }

    p_vlynq_dev[Ac498Device+1] = PAL_vlynqDevCreate(p_vlynq[Ac498Device+1], dev_name, instance, 0, 1);

    if(!p_vlynq_dev[Ac498Device+1])
        {
        printk("Failed to create the %s%d reference for vlynq.\n", dev_name, instance);
        goto av_vlynq_dev_fail;
        }
    printk("%s%d, success to create the reference for vlynq.\n", dev_name, instance);

    /* printk("The vlynq %x and dev %x.\n", p_vlynq[Ac498Device+1], p_vlynq_dev[Ac498Device+1])*/;
    if(PAL_vlynqAddDevice(p_vlynq[Ac498Device+1], p_vlynq_dev[Ac498Device+1], 1))
        {
        printk("Failed to add %s%d reference into vlynq.\n", dev_name, instance);
        goto av_vlynq_add_device_fail;
        }
    while(p_region->id > -1)
        {
        if(PAL_vlynqMapRegion(p_vlynq[Ac498Device+1], p_region->remote, p_region->id, p_region->offset, p_region->size, p_vlynq_dev[Ac498Device+1]))
            {
            p_region->id = -1;
            printk("Failed to map regions for %s%d in vlynq.\n", dev_name, instance);
            goto av_vlynq_map_region_fail;
            }
        p_region++;
        }
    /* printk("Success in setting the VLYNQ.\n")*/;
    {
        unsigned int base_addr;
        if(!PAL_vlynqGetDevBase(p_vlynq[Ac498Device+1], 0x0, &base_addr, p_vlynq_dev[Ac498Device+1]))
            /* printk("The base address is %x.\n", 0xa0000000 | base_addr)*/;
        else
            printk("Failed to get the base address.\n");
    }
    return;

/*************************************/
av_vlynq_set_irq_fail:
av_vlynq_map_irq_fail:
    p_irq = &irq_config2[0];
    while(p_irq->hw_intr_line > -1)
        PAL_vlynqUnMapIrq(p_vlynq[Ac498Device+1], p_irq->irq, p_vlynq_dev[Ac498Device+1]);

 /*************************************/
av_vlynq_map_region_fail:
    switch(Ac498Device)
        {
        case 0:
            p_region = &region_config_AC498_2nd_LowRxMap_ForHpi[0];
            break;
        case 1:
            p_region = &region_config_AC498_3rd_LowRxMap_ForHpi[0];
            break;
        default:
            return;
        }
    while(p_region->id > -1)
        PAL_vlynqUnMapRegion(p_vlynq[Ac498Device+1], p_region->remote, p_region->id, p_vlynq_dev[Ac498Device+1]);

 /*************************************/
av_vlynq_add_device_fail:
    #if 0
        pal_vlynq_dev_destroy(p_vlynq_dev[Ac498Device+1]);
    #endif

 /*************************************/
av_vlynq_dev_fail:
    PAL_vlynqCleanUp(p_vlynq_dev[Ac498Device+1]);

 /*************************************/
av_vlynq_init_fail:
	return;
}

//#endif

int setDeviceParams(int Device, int NumberOfCallProgressTones,
		acgTCallProgressTone *pCallProgressTones, Tac49xSetupDeviceAttr *pSetupDeviceAttr)
{
	int ret = AC494_DSP_OK_E;

	int toneIndex = 0;

    	pSetupDeviceAttr->UserDefinedTones.NumberOfSignals = NUM_USER_TONES;
    	pSetupDeviceAttr->UserDefinedTones.TotalEnergyThreshold = 44;

	/* The user didn't define call progress tones - default values should be used */
	if(NumberOfCallProgressTones != 0)
	{
		pSetupDeviceAttr->CallProgress.NumberOfSignals = NumberOfCallProgressTones;
		pSetupDeviceAttr->CallProgress.TotalEnergyThreshold = 44;


		for(toneIndex=0; toneIndex<NumberOfCallProgressTones; toneIndex++)
		{
		    	pSetupDeviceAttr->CallProgress.Signal[toneIndex].Type = pCallProgressTones[toneIndex].Type;
			pSetupDeviceAttr->CallProgress.Signal[toneIndex].ToneAFrequency = pCallProgressTones[toneIndex].ToneAFrequency;
			pSetupDeviceAttr->CallProgress.Signal[toneIndex].ToneB_OrAmpModulationFrequency = pCallProgressTones[toneIndex].ToneB_OrAmpModulationFrequency;
			pSetupDeviceAttr->CallProgress.Signal[toneIndex].ToneALevel = pCallProgressTones[toneIndex].ToneALevel;
			pSetupDeviceAttr->CallProgress.Signal[toneIndex].ToneBLevel = pCallProgressTones[toneIndex].ToneBLevel;
			pSetupDeviceAttr->CallProgress.Signal[toneIndex].DetectionTimeOrCadenceFirstOnOrBurstDuration = pCallProgressTones[toneIndex].DetectionTimeOrCadenceFirstOnOrBurstDuration;
			pSetupDeviceAttr->CallProgress.Signal[toneIndex].CadenceFirstOffDuration = pCallProgressTones[toneIndex].CadenceFirstOffDuration;
			pSetupDeviceAttr->CallProgress.Signal[toneIndex].CadenceSecondOnDuration = pCallProgressTones[toneIndex].CadenceSecondOnDuration;
			pSetupDeviceAttr->CallProgress.Signal[toneIndex].CadenceSecondOffDuration = pCallProgressTones[toneIndex].CadenceSecondOffDuration;
			pSetupDeviceAttr->CallProgress.Signal[toneIndex].CadenceThirdOnDuration = pCallProgressTones[toneIndex].CadenceThirdOnDuration;
			pSetupDeviceAttr->CallProgress.Signal[toneIndex].CadenceThirdOffDuration = pCallProgressTones[toneIndex].CadenceThirdOffDuration;
			pSetupDeviceAttr->CallProgress.Signal[toneIndex].CadenceFourthOnDuration = pCallProgressTones[toneIndex].CadenceFourthOnDuration;
			pSetupDeviceAttr->CallProgress.Signal[toneIndex].CadenceFourthOffDuration = pCallProgressTones[toneIndex].CadenceFourthOffDuration;
			pSetupDeviceAttr->CallProgress.Signal[toneIndex].CadenceVoiceAddedWhileFirstOff = pCallProgressTones[toneIndex].CadenceVoiceAddedWhileFirstOff;
			pSetupDeviceAttr->CallProgress.Signal[toneIndex].CadenceVoiceAddedWhileSecondOff = pCallProgressTones[toneIndex].CadenceVoiceAddedWhileSecondOff;
			pSetupDeviceAttr->CallProgress.Signal[toneIndex].CadenceVoiceAddedWhileThirdOff = pCallProgressTones[toneIndex].CadenceVoiceAddedWhileThirdOff;
			pSetupDeviceAttr->CallProgress.Signal[toneIndex].CadenceVoiceAddedWhileFourthOff = pCallProgressTones[toneIndex].CadenceVoiceAddedWhileFourthOff;

			pSetupDeviceAttr->CallProgress.Signal[toneIndex].HighEnergyThreshold = 35;	    /* -dbm */
			pSetupDeviceAttr->CallProgress.Signal[toneIndex].LowEnergyThreshold = 0;	    /* dbm  */

			pSetupDeviceAttr->CallProgress.Signal[toneIndex].TwistThreshold = 10;
			pSetupDeviceAttr->CallProgress.Signal[toneIndex].ThirdToneOfTripleBatchDurationTypeFrequency = 0;
			pSetupDeviceAttr->CallProgress.Signal[toneIndex].SignalToNoiseRatioThreshold = 15;
			pSetupDeviceAttr->CallProgress.Signal[toneIndex].FrequencyDeviationThreshold = 10;
			pSetupDeviceAttr->CallProgress.Signal[toneIndex].AmFactor = 0;

		}
	}

	pSetupDeviceAttr->Open.Common.BusSpeed		= BUS_SPEED__2MHZ;
	pSetupDeviceAttr->Open.CasDebounce				= CAS_DEBOUNCE__10MS;
	pSetupDeviceAttr->Open.CasMode				= CAS_MODE__ABCD;
	pSetupDeviceAttr->Open.CasReversalsDetection		= CONTROL__DISABLE;
	pSetupDeviceAttr->Open.DeviceStatusPacketEnable	= CONTROL__ENABLE;
	pSetupDeviceAttr->Open.DebugInfoPacketEnable		= CONTROL__ENABLE;
	pSetupDeviceAttr->Open.CasValueDuringIdle		= 0x00;
    	pSetupDeviceAttr->Open.Common.UtopiaLogicalSlaveId  = 1;/*A.L 061218*/
    	pSetupDeviceAttr->Open.Common.UtopiaPhysicalSlaveId = 1;/*A.L 061218*/
	pSetupDeviceAttr->Open.Common.AicClkFrequency 	= AIC_CLK_FREQUENCY__16384_KHZ; /*AIC_CLK_FREQUENCY__8192_KHZ;*/ /*AIC clock, comes from the ALTERA to the DSP in the EVB*/


#if defined(CONFIG_SLIC_LE88_MODULE)||defined(CONFIG_SLIC_WINBOND_MODULE)|| \
	defined(CONFIG_SLIC_SILABS_FXS_MODULE)||defined(CONFIG_SLIC_SILABS_DAA)

       	pSetupDeviceAttr->Open.Common.CodecEnable			= CODEC_ENABLE__DISABLE;
    	pSetupDeviceAttr->Open.Common.AicClkFrequency     		= AIC_CLK_FREQUENCY__16384_KHZ;
    	pSetupDeviceAttr->Open.Common.PcmClkAndFrameSyncSource 	= PCM_CLK_AND_FRAME_SYNC_SOURCE__INTERNAL;
    	pSetupDeviceAttr->Open.Common.PcmClkDivideRatio		= 23; /*A.L 061218 was 19;*/
    	pSetupDeviceAttr->Open.Common.PcmFrameSyncWidth	= 0;

#elif defined(CONFIG_SDP_IPPHONE) || defined(CONFIG_ORCHID)

	pSetupDeviceAttr->Open.Common.CodecEnable		= CODEC_ENABLE__FOR_CHANNEL_1_AND_2;
	pSetupDeviceAttr->Open.Common.AicClkFrequency	= AIC_CLK_FREQUENCY__16384_KHZ; /*AIC clock, comes from the ALTERA to the DSP in the EVB*/

#else
	printk("Illigal configuration flag\n");
	return AC494_DSP_ERROR_E;
#endif


#if 0
    	if(PcmType == ACG_DSP_PCM_FXS_KATRI_E)  {
        	pSetupDeviceAttr->Open.Common.CodecEnable 				= CODEC_ENABLE__DISABLE;
    	}
#endif


	switch(Device) {
		case 0:
			pSetupDeviceAttr->Open.Channel[0].Slot               = 0;
			pSetupDeviceAttr->Open.Channel[0].InterconnectedSlot = 4;

			pSetupDeviceAttr->Open.Channel[1].Slot               = 1;
			pSetupDeviceAttr->Open.Channel[1].InterconnectedSlot = 5;

			//pSetupDeviceAttr->Open.Channel[2].Slot               = 8;
			pSetupDeviceAttr->Open.Channel[2].Slot               = 32;
			//pSetupDeviceAttr->Open.Channel[2].InterconnectedSlot = 12;
			pSetupDeviceAttr->Open.Channel[2].InterconnectedSlot = 32;

			pSetupDeviceAttr->Open.Channel[3].Slot               = 9;
			pSetupDeviceAttr->Open.Channel[3].InterconnectedSlot = 13;
			/*ACL A.L this was a bug. not needed here. It is being set by default to External, and set to Internal if needed above depends on the fxsAppFlag*/
			/*pSetupDeviceAttr->Open.Common.PcmClkAndFrameSyncSource = PCM_CLK_AND_FRAME_SYNC_SOURCE__INTERNAL;*/

			break;

		case 1:

			pSetupDeviceAttr->Open.Channel[0].Slot               = 2;
			pSetupDeviceAttr->Open.Channel[0].InterconnectedSlot = 6;

			pSetupDeviceAttr->Open.Channel[1].Slot               = 3;
			pSetupDeviceAttr->Open.Channel[1].InterconnectedSlot = 7;

			pSetupDeviceAttr->Open.Channel[2].Slot               = 10;
			pSetupDeviceAttr->Open.Channel[2].InterconnectedSlot = 14;

			pSetupDeviceAttr->Open.Channel[3].Slot               = 11;
			pSetupDeviceAttr->Open.Channel[3].InterconnectedSlot = 15;
			pSetupDeviceAttr->Open.Common.PcmClkAndFrameSyncSource = PCM_CLK_AND_FRAME_SYNC_SOURCE__EXTERNAL;

			break;
		case 2:

			pSetupDeviceAttr->Open.Channel[0].Slot               = 4;
			pSetupDeviceAttr->Open.Channel[0].InterconnectedSlot = 8;

			pSetupDeviceAttr->Open.Channel[1].Slot               = 5;
			pSetupDeviceAttr->Open.Channel[1].InterconnectedSlot = 9;

			pSetupDeviceAttr->Open.Channel[2].Slot               = 12;
			pSetupDeviceAttr->Open.Channel[2].InterconnectedSlot = 16;

			pSetupDeviceAttr->Open.Channel[3].Slot               = 13;
			pSetupDeviceAttr->Open.Channel[3].InterconnectedSlot = 17;

			pSetupDeviceAttr->Open.Common.PcmClkAndFrameSyncSource = PCM_CLK_AND_FRAME_SYNC_SOURCE__EXTERNAL;

			break;

		case 3:

			pSetupDeviceAttr->Open.Channel[0].Slot               = 6;
			pSetupDeviceAttr->Open.Channel[0].InterconnectedSlot = 10;

			pSetupDeviceAttr->Open.Channel[1].Slot               = 7;
			pSetupDeviceAttr->Open.Channel[1].InterconnectedSlot = 11;

			pSetupDeviceAttr->Open.Channel[2].Slot               = 14;
			pSetupDeviceAttr->Open.Channel[2].InterconnectedSlot = 18;

			pSetupDeviceAttr->Open.Channel[3].Slot               = 15;
			pSetupDeviceAttr->Open.Channel[3].InterconnectedSlot = 19;

			pSetupDeviceAttr->Open.Common.PcmClkAndFrameSyncSource = PCM_CLK_AND_FRAME_SYNC_SOURCE__EXTERNAL;

			break;
		default:
			printk("Illigal Device %d\n", Device);
			break;

	} /* end switch */

	return ret;

}

int CodecConfig(int Device, int Channel, acgTChannelParameters *pChannelParameters)
{
	switch(pChannelParameters->RTPStreamParameters.Coder)
        {
		case CODER__G722_48K:
		case CODER__G722_56K:
		case CODER__G722_64K:
			dsp_dev->dspDev[Device].SetupChannelAttr[Channel].CodecConfiguration.SamplingRate = SAMPLING_RATE__16_KHz;
			break;
        	default:
			dsp_dev->dspDev[Device].SetupChannelAttr[Channel].CodecConfiguration.SamplingRate = SAMPLING_RATE__8_KHz;
			break;
	}
	return(Ac49xCodecConfiguration(Device, Channel, &(dsp_dev->dspDev[Device].SetupChannelAttr[Channel].CodecConfiguration)));

}

int AdvanceChannelConfig(int Device, int Channel)
{
	return (Ac49xAdvancedChannelConfiguration(Device, Channel, &(dsp_dev->dspDev[Device].SetupChannelAttr[Channel].Advanced)));
}

int OpenUpdateChannelConfig(int Device, int Channel, acgTChannelParameters *pChannelParameters)
{
	dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.FaxAndDataStatusAndEventsDisable = CONTROL__DISABLE;

	dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.DtmfTransferMode = pChannelParameters->SignalingParameters.DTMFTransportMethod;
	dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.EchoCancelerMode = pChannelParameters->EchoCancellerParameters.EchoCanceler;
	dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.EchoCancelerNonLinearProcessor =  kcode2code(EchoCancelerNonLinearProcessor, pChannelParameters->EchoCancellerParameters.EchoCancelerNonLinearProcessor);

	dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.TestMode = pChannelParameters->MiscellaneousParameters.TestMode;

	dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.VoiceOutputGain = pChannelParameters->GainParameters.VoiceOutputGain;
	dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.InputGain = pChannelParameters->GainParameters.InputGain;
	dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.AgcEnable = pChannelParameters->GainParameters.AgcEnable;
	dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.AgcLocation = pChannelParameters->GainParameters.AgcLocation;
	dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.AgcTargetEnergy = pChannelParameters->GainParameters.AgcTargetEnergy* (-1); /* match to DSP values according to Tac49xAgcTargetEnergy */

	dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.VoiceJitterBufferMinDelay = pChannelParameters->JitterBufferParameters.VoiceJitterBufferMinDelay;
	dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.VoiceJitterBufferMaxDelay = pChannelParameters->JitterBufferParameters.VoiceJitterBufferMaxDelay;
	dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.DynamicJitterBufferOptimizationFactor = pChannelParameters->JitterBufferParameters.DynamicJitterBufferOptimizationFactor;
	dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.FaxErrorCorrectionMode = pChannelParameters->FaxModemParameters.FaxErrorCorrection;
	dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.CngRelayEnable = pChannelParameters->FaxModemParameters.CngRelay;
	dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.FaxMaxRate = pChannelParameters->FaxModemParameters.FaxMaxRate;
	dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.CallerIdStandard = pChannelParameters->CallerIDParameters.CallerIdStandard;


	if((pChannelParameters->FaxModemParameters.FaxTransferMethod != ACG_FAX_MODEM_TRANSPORT__RELAY) &&
		(pChannelParameters->FaxModemParameters.FaxTransferMethod != pChannelParameters->FaxModemParameters.ModemTransferMethod))
	{
		printk("Illigal Fax/Modem Configuration, Fax/Modem transfer method must be the equal when fax is not configured as transport Relay\n");
		return (-1);
	}
	else	if(pChannelParameters->FaxModemParameters.FaxTransferMethod == ACG_FAX_MODEM_TRANSPORT__VBD)
		{
			dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.FaxTransferMethod = FAX_MODEM_TRANSFER_METHOD__BYPASS;

			dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.V21TransferMethod = FAX_MODEM_TRANSFER_METHOD__BYPASS;
			dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.V22TransferMethod = FAX_MODEM_TRANSFER_METHOD__BYPASS;
			dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.V23TransferMethod = FAX_MODEM_TRANSFER_METHOD__BYPASS;
			dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.V32TransferMethod = FAX_MODEM_TRANSFER_METHOD__BYPASS;
			dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.V34TransferMethod = FAX_MODEM_TRANSFER_METHOD__BYPASS;
		}
		else if(pChannelParameters->FaxModemParameters.ModemTransferMethod == ACG_FAX_MODEM_TRANSPORT__VBD)
		{
			dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.FaxTransferMethod = pChannelParameters->FaxModemParameters.FaxTransferMethod;

			dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.V21TransferMethod = FAX_MODEM_TRANSFER_METHOD__BYPASS;
			dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.V22TransferMethod = FAX_MODEM_TRANSFER_METHOD__BYPASS;
			dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.V23TransferMethod = FAX_MODEM_TRANSFER_METHOD__BYPASS;
			dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.V32TransferMethod = FAX_MODEM_TRANSFER_METHOD__BYPASS;
			dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.V34TransferMethod = FAX_MODEM_TRANSFER_METHOD__BYPASS;
		}
		else
		{
			dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.FaxTransferMethod = pChannelParameters->FaxModemParameters.FaxTransferMethod;

			dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.V21TransferMethod = pChannelParameters->FaxModemParameters.ModemTransferMethod;
			dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.V22TransferMethod = pChannelParameters->FaxModemParameters.ModemTransferMethod;
			dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.V23TransferMethod = pChannelParameters->FaxModemParameters.ModemTransferMethod;
			dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.V32TransferMethod = pChannelParameters->FaxModemParameters.ModemTransferMethod;
			dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.V34TransferMethod = pChannelParameters->FaxModemParameters.ModemTransferMethod;
		}

	return 0;
}

int ActivateUpdateRegularRtpChannelConfig(Tac49xActivateOrUpdateRtpChannelConfigurationAttr *rtpInfo, acgTChannelParameters *pChannelParameters)
{
	int 	pTime = pChannelParameters->RTPStreamParameters.PTime;
	int 	coder = pChannelParameters->RTPStreamParameters.Coder;


	rtpInfo->Decoder = coder;
	rtpInfo->Encoder = coder;
	rtpInfo->RxPayloadType = pChannelParameters->RTPStreamParameters.RxVoicePayloadType;
	rtpInfo->TxPayloadType = pChannelParameters->RTPStreamParameters.TxVoicePayloadType;

	rtpInfo->FaxBypassPayloadType = pChannelParameters->RTPStreamParameters.FaxBypassPayloadType;
	rtpInfo->DataBypassPayloadType = pChannelParameters->RTPStreamParameters.ModemByassPayloadType;
	rtpInfo->BypassCoder = pChannelParameters->FaxModemParameters.BypassCoder;
	rtpInfo->TxSignalingRfc2833PayloadType = pChannelParameters->RTPStreamParameters.TxDTMFRelayPayloadType;
	rtpInfo->RxSignalingRfc2833PayloadType = pChannelParameters->RTPStreamParameters.RxDTMFRelayPayloadType;
	rtpInfo->SilenceCompressionMode = pChannelParameters->RTPStreamParameters.SilenceCompressionMode;


    	/*set defaults*/
	rtpInfo->EncoderPcmAdpcmFrameDuration = kcode2code(ptimes, pTime);
	rtpInfo->DecoderPcmAdpcmFrameDuration = kcode2code(ptimes, pTime);

    	rtpInfo->RxPayloadTypeSource = PAYLOAD_TYPE_SOURCE__DEFINED_INTERNALLY_ACCORDING_THE_ENCODER;
    	rtpInfo->TxPayloadTypeSource = PAYLOAD_TYPE_SOURCE__DEFINED_INTERNALLY_ACCORDING_THE_ENCODER;


	if(pChannelParameters->FaxModemParameters.FaxTransferMethod == ACG_FAX_MODEM_TRANSPORT__VBD)
	{
		if(pChannelParameters->RTPStreamParameters.FaxBypassPayloadType != pChannelParameters->RTPStreamParameters.ModemByassPayloadType)
		{
      			printk("FaxBypassPayloadType(%d) and ModemByassPayloadType(%d) are different, setting both to FaxBypassPayloadType: %d\r\n",
					rtpInfo->FaxBypassPayloadType, rtpInfo->DataBypassPayloadType,
					rtpInfo->FaxBypassPayloadType);

			rtpInfo->DataBypassPayloadType = pChannelParameters->RTPStreamParameters.FaxBypassPayloadType;
		}
	}


	/*Note: at this stage we assume that encoder == decoder...*/
	switch(coder)
	{
		case CODER__G726_16:
		        rtpInfo->RxPayloadTypeSource = PAYLOAD_TYPE_SOURCE__DEFINED_EXTERNALLY;
		        rtpInfo->TxPayloadTypeSource = PAYLOAD_TYPE_SOURCE__DEFINED_EXTERNALLY;
			rtpInfo->NumberOfVoiceBlocks_minus_1 = kcode2code(ptime_to_voice_blocks_711_726, pTime);
			rtpInfo->EncoderPcmAdpcmFrameDuration = kcode2code(ptime_to_encoder_frame_duration_711_726, pTime);
			rtpInfo->DecoderPcmAdpcmFrameDuration = kcode2code(ptime_to_decoder_frame_duration_711_726, pTime);
			break;
	        /*no break, do also the case bellow*/
		case CODER__G711ALAW:
		case CODER__G711MULAW:
		case CODER__G726_24:
		case CODER__G726_32:
		case CODER__G726_40:
		        rtpInfo->RxPayloadTypeSource = PAYLOAD_TYPE_SOURCE__DEFINED_INTERNALLY_ACCORDING_THE_ENCODER;
		        rtpInfo->TxPayloadTypeSource = PAYLOAD_TYPE_SOURCE__DEFINED_INTERNALLY_ACCORDING_THE_ENCODER;
			rtpInfo->NumberOfVoiceBlocks_minus_1 = kcode2code(ptime_to_voice_blocks_711_726, pTime);
			rtpInfo->EncoderPcmAdpcmFrameDuration = kcode2code(ptime_to_encoder_frame_duration_711_726, pTime);
			rtpInfo->DecoderPcmAdpcmFrameDuration = kcode2code(ptime_to_decoder_frame_duration_711_726, pTime);
			break;
		case CODER__G723LOW:
		case CODER__G723HIGH:
		        rtpInfo->RxPayloadTypeSource = PAYLOAD_TYPE_SOURCE__DEFINED_INTERNALLY_ACCORDING_THE_ENCODER;
		        rtpInfo->TxPayloadTypeSource = PAYLOAD_TYPE_SOURCE__DEFINED_INTERNALLY_ACCORDING_THE_ENCODER;
			rtpInfo->NumberOfVoiceBlocks_minus_1 = kcode2code(ptime_to_voice_blocks_723, pTime);
			break;
		case CODER__G729:
		        rtpInfo->RxPayloadTypeSource = PAYLOAD_TYPE_SOURCE__DEFINED_INTERNALLY_ACCORDING_THE_ENCODER;
		        rtpInfo->TxPayloadTypeSource = PAYLOAD_TYPE_SOURCE__DEFINED_INTERNALLY_ACCORDING_THE_ENCODER;
			rtpInfo->NumberOfVoiceBlocks_minus_1 = kcode2code(ptime_to_voice_blocks_729, pTime);
			break;

		case CODER__G722_48K:
		case CODER__G722_56K:
		case CODER__G722_64K:
	              	rtpInfo->RxPayloadTypeSource = PAYLOAD_TYPE_SOURCE__DEFINED_EXTERNALLY;
	              	rtpInfo->TxPayloadTypeSource = PAYLOAD_TYPE_SOURCE__DEFINED_EXTERNALLY;
	              	rtpInfo->NumberOfVoiceBlocks_minus_1 = kcode2code(ptime_to_voice_blocks_722, pTime);
	              	break;

		default:
			printk("ActivateUpdateRegularRtpChannelConfig():: unsupported coder type %d\r\n", coder);
	}

	return 0;

}


void set3wcParams(Tac49xActivateOrDeactivate3WayConferenceConfigurationAttr *p3WayConferenceAttr,  int Channel, int Mode)
{
	p3WayConferenceAttr->ConferenceChannelMate = Channel;
	p3WayConferenceAttr->Mode = Mode;

	p3WayConferenceAttr->Conference1TransmitPayloadType = 8;
	p3WayConferenceAttr->Conference1TransmitSequenceNumber = 0;
	p3WayConferenceAttr->Conference1RtpSsrc = 1;
	p3WayConferenceAttr->Conference2TransmitPayloadType = 8;
	p3WayConferenceAttr->Conference2TransmitSequenceNumber = 0;
	p3WayConferenceAttr->Conference2RtpSsrc = 2;
}

#if defined(CONFIG_SDP_IPPHONE) || defined(CONFIG_ORCHID)
int setCodecParams(int Device, int Channel, acIPPCodecType codecType)
{
	int ret = AC494_DSP_OK_E;

	switch(codecType)
	{
		case AC494_CODEC_HANDSET:

			dsp_dev->dspDev[Device].SetupChannelAttr[Channel].CodecConfiguration.HandsetInputSelect = CONTROL__ENABLE;
			dsp_dev->dspDev[Device].SetupChannelAttr[Channel].CodecConfiguration.HandsetOutputSelect = CONTROL__ENABLE;
			break;

		case AC494_CODEC_HEADSET:

			dsp_dev->dspDev[Device].SetupChannelAttr[Channel].CodecConfiguration.HeadsetInputSelect = CONTROL__ENABLE;
			dsp_dev->dspDev[Device].SetupChannelAttr[Channel].CodecConfiguration.HeadsetOutputSelect = CONTROL__ENABLE;

			break;

		case AC494_CODEC_MIC:

			dsp_dev->dspDev[Device].SetupChannelAttr[Channel].CodecConfiguration.MicrophoneInputSelect = CONTROL__ENABLE;
			dsp_dev->dspDev[Device].SetupChannelAttr[Channel].CodecConfiguration.SpeakerSelect = CONTROL__ENABLE;

			break;

		case AC494_CODEC_LINE:

			dsp_dev->dspDev[Device].SetupChannelAttr[Channel].CodecConfiguration.LineInputSelect = CONTROL__ENABLE;
			dsp_dev->dspDev[Device].SetupChannelAttr[Channel].CodecConfiguration.LineOutputSelect = CONTROL__ENABLE;

			break;

		default:
			printk("error value codecType");
			ret = AC494_DSP_ERROR_E;
			break ;


	}

	return ret;

}

#endif

void setToneParams(acgTTone *pTone, Tac49xIbsStringAttr *pIbsStringAttr)
{
	pIbsStringAttr->GeneratorRedirection = pTone->Direction;
	pIbsStringAttr->NumberOfDigits = 1;
	pIbsStringAttr->OnDuration = pTone->OnDuration;
	pIbsStringAttr->OffDuration = pTone->OffDuration;

	pIbsStringAttr->Signal[0].Digit = pTone->ToneElement.ToneIndex;

	if(pTone->ToneElement.Type == ACG_TONE_TYPE__CALL_PROGRESS_TONE)
	{
		if((pTone->ToneElement.ToneIndex >= 0) && (pTone->ToneElement.ToneIndex <= 15))
			pIbsStringAttr->Signal[0].SystemCode = IBS_SYSTEM_CODE__CALL_PROGRESS_0_TO_15;
		else if((pTone->ToneElement.ToneIndex > 15) && (pTone->ToneElement.ToneIndex <= 31))
			pIbsStringAttr->Signal[0].SystemCode = IBS_SYSTEM_CODE__CALL_PROGRESS_16_TO_31;
		else
			printk("\nToneIndex is out of limit\n");

	}
	else if(pTone->ToneElement.Type == ACG_TONE_TYPE__DTMF)
	{
		pIbsStringAttr->Signal[0].SystemCode = IBS_SYSTEM_CODE__DTMF;
	}
	else
		printk("\nTone Type is out of limit\n");

}

void setToneSequenceParams(acgTToneSequence *pToneSequence, Tac49xIbsStringAttr *pIbsStringAttr)
{
	int curSignal = 0;

	pIbsStringAttr->GeneratorRedirection = pToneSequence->Direction;
	pIbsStringAttr->NumberOfDigits = pToneSequence->NumOfTones;
	pIbsStringAttr->OnDuration = pToneSequence->OnDuration;
	pIbsStringAttr->OffDuration = pToneSequence->OffDuration;

	for(curSignal=0; curSignal<pToneSequence->NumOfTones; curSignal++)
	{
		pIbsStringAttr->Signal[curSignal].Digit = pToneSequence->Tones[curSignal].ToneIndex;

		if(pToneSequence->Tones[curSignal].Type == ACG_TONE_TYPE__CALL_PROGRESS_TONE)
		{
			if((pToneSequence->Tones[curSignal].ToneIndex >= 0) && (pToneSequence->Tones[curSignal].ToneIndex <= 15))
				pIbsStringAttr->Signal[curSignal].SystemCode = IBS_SYSTEM_CODE__CALL_PROGRESS_0_TO_15;
			else if((pToneSequence->Tones[curSignal].ToneIndex > 15) && (pToneSequence->Tones[curSignal].ToneIndex <= 31))
				pIbsStringAttr->Signal[curSignal].SystemCode = IBS_SYSTEM_CODE__CALL_PROGRESS_16_TO_31;
			else
				printk("\nToneIndex is out of limit\n");

		}
		else if(pToneSequence->Tones[curSignal].Type == ACG_TONE_TYPE__DTMF)
		{
			pIbsStringAttr->Signal[curSignal].SystemCode = IBS_SYSTEM_CODE__DTMF;
		}
		else
			printk("\nTone Type is out of limit\n");

	}

}

void setToneExtendedParams(acgTToneExtended *pToneExtended, Tac49xExtendedIbsStringAttr *pIbsExtendedStringAttr)
{
	int curSignal = 0;

	pIbsExtendedStringAttr->GeneratorRedirection = pToneExtended->GeneratorRedirection;
	pIbsExtendedStringAttr->Repeat = pToneExtended->Repeat;
	pIbsExtendedStringAttr->NumberOfDigits = pToneExtended->NumberOfDigits;
	pIbsExtendedStringAttr->TotalDuration = pToneExtended->TotalDuration;
	pIbsExtendedStringAttr->UserDefinedToneA = pToneExtended->UserDefinedToneA;
	pIbsExtendedStringAttr->UserDefinedToneB = pToneExtended->UserDefinedToneB;
	pIbsExtendedStringAttr->UserDefinedToneC = pToneExtended->UserDefinedToneC;
	//pIbsExtendedStringAttr->UserDefinedToneD = pToneExtended->UserDefinedToneD;
	pIbsExtendedStringAttr->AmFactor = pToneExtended->AmFactor;

	for(curSignal=0; curSignal<pToneExtended->NumberOfDigits; curSignal++)
	{
		pIbsExtendedStringAttr->Signal[curSignal].SignalLevel_minus_dbm = pToneExtended->Tones[curSignal].SignalLevel_minus_dbm;
		pIbsExtendedStringAttr->Signal[curSignal].Digit = pToneExtended->Tones[curSignal].Digit;
		pIbsExtendedStringAttr->Signal[curSignal].SignalDuration = pToneExtended->Tones[curSignal].SignalDuration;
		pIbsExtendedStringAttr->Signal[curSignal].SystemCode = pToneExtended->Tones[curSignal].SystemCode;
	}

}


void setRingerParams(int duration, Tac49xExtendedIbsStringAttr  *pExtendedIbsStringAttr)
{
	pExtendedIbsStringAttr->GeneratorRedirection = IBS_STRING_GENERATOR_REDIRECTION__INTO_DECODER_OUTPUT;
	pExtendedIbsStringAttr->Repeat = CONTROL__ENABLE;
	pExtendedIbsStringAttr->NumberOfDigits = 4;
	pExtendedIbsStringAttr->TotalDuration = duration;
	pExtendedIbsStringAttr->UserDefinedToneA = 0;
	pExtendedIbsStringAttr->UserDefinedToneB = 0;

	pExtendedIbsStringAttr->Signal[0].SignalLevel_minus_dbm = 0;
	pExtendedIbsStringAttr->Signal[0].Digit = 1;
	pExtendedIbsStringAttr->Signal[0].SignalDuration = 100;
	pExtendedIbsStringAttr->Signal[0].SystemCode = IBS_SYSTEM_CODE__DTMF;

	pExtendedIbsStringAttr->Signal[1].SignalLevel_minus_dbm = 0;
	pExtendedIbsStringAttr->Signal[1].Digit = 2;
	pExtendedIbsStringAttr->Signal[1].SignalDuration = 100;
	pExtendedIbsStringAttr->Signal[1].SystemCode = IBS_SYSTEM_CODE__DTMF;

	pExtendedIbsStringAttr->Signal[2].SignalLevel_minus_dbm = 0;
	pExtendedIbsStringAttr->Signal[2].Digit = 3;
	pExtendedIbsStringAttr->Signal[2].SignalDuration = 100;
	pExtendedIbsStringAttr->Signal[2].SystemCode = IBS_SYSTEM_CODE__DTMF;

	pExtendedIbsStringAttr->Signal[3].SignalLevel_minus_dbm = 0;
	pExtendedIbsStringAttr->Signal[3].Digit = 9;
	pExtendedIbsStringAttr->Signal[3].SignalDuration = 1200;
	pExtendedIbsStringAttr->Signal[3].SystemCode = IBS_SYSTEM_CODE__CALL_PROGRESS_0_TO_15;

}

int sendCallerIdMessage(int Device, int Channel, acgTCallerId *pCallerId)
{
	int 						AdditionalSize, ParameterLength, i;
    	int 						PayloadIndex = sizeof(Tac49xCallerIdPayloadTitle);
	static Tac49xCallerIdAttr    	CallerIdMessage;
	static Tac49xPacket               	Packet;
    	Tac49xCallerIdPayloadTitle  	*pPayloadTitle;
    	Tac49xCallerIdPayloadFooter 	*pFooter;
	Tac49xCallerIdTimeAndDay 	*pTimeAndDay;
	Tac49xCallerIdAsciMessage 	*pCallingNumber;
	Tac49xCallerIdAsciMessage 	*pCallingName;

	memset((void *)&Packet, 0, sizeof(Tac49xPacket));

	CallerIdMessage.Standard = dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.CallerIdStandard;
        CallerIdMessage.ChecksumByteAbsent = CONTROL__DISABLE;
        CallerIdMessage.Error = CONTROL__DISABLE;
        CallerIdMessage.Event = CALLER_ID_EVENT__NONE_EVENT;


	pTimeAndDay = (Tac49xCallerIdTimeAndDay *)((Tac49xCallerIdPayloadTitle*)&Packet.u.ProprietaryPayload.u.Command.u.CallerId.Message[PayloadIndex]);

	AdditionalSize = sizeof(Tac49xCallerIdTimeAndDay);
	pTimeAndDay->Title.Type.Value =CALLER_ID_MULTIPLE_DATA_MESSAGE_PARAMETER__DATE_AND_TIME;
	pTimeAndDay->Title.Length.Value = sizeof(Tac49xCallerIdTimeAndDay) -sizeof(Tac49xCallerIdMessageTitle);
	memcpy((void *)pTimeAndDay->Month, pCallerId->Time, pTimeAndDay->Title.Length.Value);
	PayloadIndex += AdditionalSize;

	pCallingNumber = (Tac49xCallerIdAsciMessage *)((Tac49xCallerIdPayloadTitle*)&Packet.u.ProprietaryPayload.u.Command.u.CallerId.Message[PayloadIndex]);


	ParameterLength = strlen(pCallerId->Number);
	AdditionalSize  = (sizeof(pCallingNumber->Title) + ParameterLength);
	pCallingNumber->Title.Type.Value =CALLER_ID_MULTIPLE_DATA_MESSAGE_PARAMETER__CALLING_LINE_NUMBER;
	pCallingNumber->Title.Length.Value = ParameterLength;
	for(i=0; i < ParameterLength; i++)
		pCallingNumber->Data[i].Value = pCallerId->Number[i];
	PayloadIndex += AdditionalSize;

	/* ACL NB 070513 If there is no name, we would not send an empty string */
	if ( strcmp(pCallerId->Name, "") )
	{
		pCallingName =(Tac49xCallerIdAsciMessage*)&Packet.u.ProprietaryPayload.u.Command.u.CallerId.Message[PayloadIndex];

		ParameterLength = strlen(pCallerId->Name);
		AdditionalSize  = (sizeof(pCallingName->Title) + ParameterLength);
		pCallingName->Title.Type.Value =CALLER_ID_MULTIPLE_DATA_MESSAGE_PARAMETER__CALLING_NAME;
		pCallingName->Title.Length.Value = ParameterLength;
		for(i=0; i < ParameterLength; i++)
			pCallingName->Data[i].Value = pCallerId->Name[i];
		PayloadIndex += AdditionalSize;
	}

	pPayloadTitle =(Tac49xCallerIdPayloadTitle*)&Packet.u.ProprietaryPayload.u.Command.u.CallerId.Message[0];

	pPayloadTitle->Type.Value = 0x80;

	pPayloadTitle->Length.Value = (PayloadIndex -sizeof(Tac49xCallerIdPayloadTitle));

	pFooter = (Tac49xCallerIdPayloadFooter*)(pPayloadTitle+PayloadIndex);
	pFooter->Checksum.Value    = 0;
	pFooter->End.Value     = 0;

	CallerIdMessage.ServiceType  = pCallerId->CallerIdServiceType;

        return(Ac49xSendCallerIdMessage(Device, Channel, &CallerIdMessage, (U8 *)Packet.u.ProprietaryPayload.u.Command.u.CallerId.Message));

}

int sendMWIMessage(int Device, int Channel, acgTMWI *pMWI)
{
	int 						PayloadIndex = 0;
	static Tac49xCallerIdAttr    	CallerIdMessage;
	static Tac49xPacket               	Packet;
	Tac49xCallerIdPayloadTitle  	*pPayloadTitle;
	Tac49xCallerIdAsciMessage 	*pMsgWaitingInd;
	Tac49xCallerIdPayloadFooter 	*pFooter;

	memset((void *)&Packet, 0, sizeof(Tac49xPacket));
	memset((void *)&CallerIdMessage, 0, sizeof(Tac49xCallerIdAttr));

	CallerIdMessage.Standard = dsp_dev->dspDev[Device].SetupChannelAttr[Channel].OpenOrUpdateChannel.CallerIdStandard;
	CallerIdMessage.ChecksumByteAbsent = CONTROL__DISABLE;
	CallerIdMessage.Error = CONTROL__DISABLE;
	CallerIdMessage.Event  = CALLER_ID_EVENT__NONE_EVENT;
	CallerIdMessage.ServiceType = pMWI->CallerIdServiceType;

	pPayloadTitle = (Tac49xCallerIdPayloadTitle*)&Packet.u.ProprietaryPayload.u.Command.u.CallerId.Message[0];

	//printk("ac494_send_vmwi_msg PayloadIndex=%d\n", PayloadIndex );

	pPayloadTitle->Type.Value = 0x82;
	pPayloadTitle->Length.Value = 0x03;

	pMsgWaitingInd= (Tac49xCallerIdAsciMessage*)&Packet.u.ProprietaryPayload.u.Command.u.CallerId.Message[PayloadIndex];
	pMsgWaitingInd->Data[0].Value = 0x0b;
	PayloadIndex++;
		pMsgWaitingInd->Data[1].Value = 0x01;
	PayloadIndex++;

	if ( pMWI->Activate )
		   pMsgWaitingInd->Data[2].Value =0xff;
	else
		   pMsgWaitingInd->Data[2].Value =0x0;
	PayloadIndex++;

	pFooter = (Tac49xCallerIdPayloadFooter*)(pPayloadTitle+PayloadIndex);
	pFooter->Checksum.Value = 0;
	pFooter->End.Value = 0;

	return(Ac49xSendCallerIdMessage(Device, Channel, &CallerIdMessage, (U8 *)Packet.u.ProprietaryPayload.u.Command.u.CallerId.Message));

}


#if defined(CONFIG_SDP_IPPHONE) || defined(CONFIG_ORCHID)
void setCodecAEC(Tac49xSetupChannelAttr *pSetupChannelAttr,  acIPPCodecType codecType)
{

	switch(codecType)
	{
		case AC494_CODEC_SPEAKER:
               	pSetupChannelAttr->CodecConfiguration.AdcDitheringCircuit             = CONTROL__ENABLE;
		       	pSetupChannelAttr->CodecConfiguration.BiasPinOutputVoltage            = BIAS_PIN_OUTPUT_VOLTAGE__2v35 ;
		       	pSetupChannelAttr->CodecConfiguration.SpeakerGain                     = SPEAKER_GAIN__3dB ;
			    pSetupChannelAttr->CodecConfiguration.BandPassFilter                  = CONTROL__ENABLE;
		       	pSetupChannelAttr->CodecConfiguration.MuteHandset                     = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.MuteHeadset                     = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.MuteLineOutput                  = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.MuteSpeaker                     = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.HeadsetInputToOutput            = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.HandsetInputToOutput            = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.CallerIdInputSelect             = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.LineInputSelect                 = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.MicrophoneInputSelect           = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.HandsetInputSelect              = CONTROL__DISABLE;
		       	pSetupChannelAttr->CodecConfiguration.HeadsetInputSelect              = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.AnalogSidetoneGain              = SIDETONE_GAIN__minusd12dB;
		        pSetupChannelAttr->CodecConfiguration.DigitalSidetoneGain             = SIDETONE_GAIN__MUTE;
		        pSetupChannelAttr->CodecConfiguration.AnalogSidetoneOutputSelectForHeadset  = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.AnalogSidetoneOutputSelectForHandset  = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.SpeakerSelect                   = CONTROL__ENABLE;
		        pSetupChannelAttr->CodecConfiguration.LineOutputSelect                = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.HeadsetOutputSelect             = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.HandsetOutputSelect             = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.DacOutputPgaGain                = CODEC_DAC_OUTPUT_PGA_GAIN__0dB  ;
		        pSetupChannelAttr->CodecConfiguration.AdcInputPgaGain                 = CODEC_ADC_INPUT_PGA_GAIN__plus37_5dB   ;
		        pSetupChannelAttr->CodecConfiguration.AnalogLoopback                  = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.DigitalLoopback                 = CONTROL__DISABLE;

			pSetupChannelAttr->OpenOrUpdateChannel.VoiceOutputGain                = VOICE_GAIN__minus9_DB;


		break;
		case AC494_CODEC_MIC:

		       	pSetupChannelAttr->CodecConfiguration.AdcDitheringCircuit             = CONTROL__ENABLE;
		       	pSetupChannelAttr->CodecConfiguration.BiasPinOutputVoltage            = BIAS_PIN_OUTPUT_VOLTAGE__2v35 ;
		       	pSetupChannelAttr->CodecConfiguration.SpeakerGain                     = SPEAKER_GAIN__0dB ;
			pSetupChannelAttr->CodecConfiguration.BandPassFilter                  = CONTROL__ENABLE;
		       	pSetupChannelAttr->CodecConfiguration.MuteHandset                     = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.MuteHeadset                     = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.MuteLineOutput                  = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.MuteSpeaker                     = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.HeadsetInputToOutput            = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.HandsetInputToOutput            = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.CallerIdInputSelect             = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.LineInputSelect                 = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.MicrophoneInputSelect           = CONTROL__ENABLE;
		        pSetupChannelAttr->CodecConfiguration.HandsetInputSelect              = CONTROL__DISABLE;
		       	pSetupChannelAttr->CodecConfiguration.HeadsetInputSelect              = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.AnalogSidetoneGain              = SIDETONE_GAIN__minusd12dB;
		        pSetupChannelAttr->CodecConfiguration.DigitalSidetoneGain             = SIDETONE_GAIN__MUTE;
		        pSetupChannelAttr->CodecConfiguration.AnalogSidetoneOutputSelectForHeadset  = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.AnalogSidetoneOutputSelectForHandset  = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.SpeakerSelect                   = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.LineOutputSelect                = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.HeadsetOutputSelect             = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.HandsetOutputSelect             = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.DacOutputPgaGain                = CODEC_DAC_OUTPUT_PGA_GAIN__0dB  ;
			pSetupChannelAttr->CodecConfiguration.AdcInputPgaGain                 = CODEC_ADC_INPUT_PGA_GAIN__plus37_5dB ;
		        pSetupChannelAttr->CodecConfiguration.AnalogLoopback                  = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.DigitalLoopback                 = CONTROL__DISABLE;

			pSetupChannelAttr->OpenOrUpdateChannel.VoiceOutputGain                = VOICE_GAIN__minus9_DB;

	       		break;

		case AC494_CODEC_HEADSET:

			pSetupChannelAttr->CodecConfiguration.AdcDitheringCircuit             = CONTROL__ENABLE;
		        pSetupChannelAttr->CodecConfiguration.BiasPinOutputVoltage            = BIAS_PIN_OUTPUT_VOLTAGE__2v35 ;
		        pSetupChannelAttr->CodecConfiguration.SpeakerGain                     = SPEAKER_GAIN__0dB;
		        pSetupChannelAttr->CodecConfiguration.BandPassFilter                  = CONTROL__ENABLE;
		        pSetupChannelAttr->CodecConfiguration.MuteHandset                     = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.MuteHeadset                     = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.MuteLineOutput                  = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.MuteSpeaker                     = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.HeadsetInputToOutput            = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.HandsetInputToOutput            = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.CallerIdInputSelect             = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.LineInputSelect                 = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.MicrophoneInputSelect           = CONTROL__ENABLE;
			pSetupChannelAttr->CodecConfiguration.HandsetInputSelect              = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.HeadsetInputSelect              = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.AnalogSidetoneGain              = SIDETONE_GAIN__minusd12dB;
		        pSetupChannelAttr->CodecConfiguration.DigitalSidetoneGain             = SIDETONE_GAIN__MUTE;
		        pSetupChannelAttr->CodecConfiguration.AnalogSidetoneOutputSelectForHeadset  = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.AnalogSidetoneOutputSelectForHandset  = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.SpeakerSelect                   = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.LineOutputSelect                = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.HeadsetOutputSelect             = CONTROL__ENABLE;
			pSetupChannelAttr->CodecConfiguration.HandsetOutputSelect             = CONTROL__DISABLE;
			pSetupChannelAttr->CodecConfiguration.DacOutputPgaGain                = CODEC_DAC_OUTPUT_PGA_GAIN__minus24dB ;
			pSetupChannelAttr->CodecConfiguration.AdcInputPgaGain                 = CODEC_ADC_INPUT_PGA_GAIN__plus12dB;
		        pSetupChannelAttr->CodecConfiguration.AnalogLoopback                  = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.DigitalLoopback                 = CONTROL__DISABLE;

			pSetupChannelAttr->OpenOrUpdateChannel.VoiceOutputGain                = VOICE_GAIN__0_DB;
	       		break;

		case AC494_CODEC_HANDSET:
	pSetupChannelAttr->CodecConfiguration.AdcDitheringCircuit             = CONTROL__ENABLE;
		        pSetupChannelAttr->CodecConfiguration.BiasPinOutputVoltage            = BIAS_PIN_OUTPUT_VOLTAGE__2v35 ;
		        pSetupChannelAttr->CodecConfiguration.SpeakerGain                     = SPEAKER_GAIN__0dB;
		        pSetupChannelAttr->CodecConfiguration.BandPassFilter                  = CONTROL__ENABLE;
		        pSetupChannelAttr->CodecConfiguration.MuteHandset                     = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.MuteHeadset                     = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.MuteLineOutput                  = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.MuteSpeaker                     = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.HeadsetInputToOutput            = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.HandsetInputToOutput            = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.CallerIdInputSelect             = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.LineInputSelect                 = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.MicrophoneInputSelect           = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.HandsetInputSelect              = CONTROL__ENABLE;
		        pSetupChannelAttr->CodecConfiguration.HeadsetInputSelect              = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.AnalogSidetoneGain              = SIDETONE_GAIN__minusd12dB;
		        pSetupChannelAttr->CodecConfiguration.DigitalSidetoneGain             = SIDETONE_GAIN__MUTE;
		        pSetupChannelAttr->CodecConfiguration.AnalogSidetoneOutputSelectForHeadset  = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.AnalogSidetoneOutputSelectForHandset  = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.SpeakerSelect                   = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.LineOutputSelect                = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.HeadsetOutputSelect             = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.HandsetOutputSelect             = CONTROL__ENABLE; //CONTROL__ENABLE
		        pSetupChannelAttr->CodecConfiguration.DacOutputPgaGain                = CODEC_DAC_OUTPUT_PGA_GAIN__minus24dB ;
		        //pSetupChannelAttr->CodecConfiguration.AdcInputPgaGain                 = CODEC_ADC_INPUT_PGA_GAIN__plus12dB ;
		        pSetupChannelAttr->CodecConfiguration.AdcInputPgaGain                 = CODEC_ADC_INPUT_PGA_GAIN__plus12dB ;
		        pSetupChannelAttr->CodecConfiguration.AnalogLoopback                  = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.DigitalLoopback                 = CONTROL__DISABLE;

			pSetupChannelAttr->OpenOrUpdateChannel.VoiceOutputGain                = VOICE_GAIN__0_DB;

	        	break;


		default:

		   	pSetupChannelAttr->CodecConfiguration.AdcDitheringCircuit             = CONTROL__ENABLE;
		        pSetupChannelAttr->CodecConfiguration.BiasPinOutputVoltage            = BIAS_PIN_OUTPUT_VOLTAGE__2v35 ;
		        pSetupChannelAttr->CodecConfiguration.SpeakerGain                     = SPEAKER_GAIN__0dB;
		        pSetupChannelAttr->CodecConfiguration.BandPassFilter                  = CONTROL__ENABLE;
		        pSetupChannelAttr->CodecConfiguration.MuteHandset                     = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.MuteHeadset                     = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.MuteLineOutput                  = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.MuteSpeaker                     = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.HeadsetInputToOutput            = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.HandsetInputToOutput            = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.CallerIdInputSelect             = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.LineInputSelect                 = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.MicrophoneInputSelect           = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.HandsetInputSelect              = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.HeadsetInputSelect              = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.AnalogSidetoneGain              = SIDETONE_GAIN__minusd12dB;
		        pSetupChannelAttr->CodecConfiguration.DigitalSidetoneGain             = SIDETONE_GAIN__MUTE;
		        pSetupChannelAttr->CodecConfiguration.AnalogSidetoneOutputSelectForHeadset  = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.AnalogSidetoneOutputSelectForHandset  = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.SpeakerSelect                   = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.LineOutputSelect                = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.HeadsetOutputSelect             = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.HandsetOutputSelect             = CONTROL__DISABLE;
			pSetupChannelAttr->CodecConfiguration.DacOutputPgaGain                = CODEC_DAC_OUTPUT_PGA_GAIN__minus24dB ;
			pSetupChannelAttr->CodecConfiguration.AdcInputPgaGain                 = CODEC_ADC_INPUT_PGA_GAIN__plus12dB ;
		        pSetupChannelAttr->CodecConfiguration.AnalogLoopback                  = CONTROL__DISABLE;
		        pSetupChannelAttr->CodecConfiguration.DigitalLoopback                 = CONTROL__DISABLE;

			pSetupChannelAttr->OpenOrUpdateChannel.VoiceOutputGain                = VOICE_GAIN__0_DB;

	        	break;

	} /* end switch */
}
#endif

int startEC(Tac49xSetupChannelAttr *pSetupChannelAttr)
{
	int ret = AC494_DSP_OK_E;

	pSetupChannelAttr->Advanced.AcousticEchoCancelerEnable =CONTROL__ENABLE;
	pSetupChannelAttr->Advanced.InputGainLocation = INPUT_GAIN_LOCATION__AFTER_ECHO_CANCELER_AND_BOFORE_ENCODER;

	ret = Ac49xAcousticEchoCancelerConfiguration(0, 0, &(pSetupChannelAttr->AcousticEchoCancelerConfiguration));
	if (-1 == ret)
		return ret;

	ret = Ac49xAdvancedChannelConfiguration(0, 0, &(pSetupChannelAttr->Advanced));
	if (-1 == ret)
		return ret;

#if defined(CONFIG_SDP_IPPHONE) || defined(CONFIG_ORCHID)
	setCodecAEC(pSetupChannelAttr,  AC494_CODEC_HANDSET);
#endif

	return ret;
}


int Translate49xDspEventsToAcgEvents(acgTEventInfo *pDspEvent, Tac49xRxPacketParams *pRxPacketParams, Tac49xPacket *pBufEvetStatus)
{
	int 			absCh;
	int			opcode;
	int 			ret = AC494_DSP_OK_E;

	Tac49xCallerIdPayload 					*callerid = NULL;
   	Tac49xIbsEventPayload 					*ibsevent = NULL;
  	Tac49xPacketizerStatusOrEventPayload 		*packetizer = NULL;
   	Tac49xSystemErrorEventPayload 			*error_p = NULL;
   	Tac49xJitterBufferStatusPayload 			*jitter = NULL;
   	Tac49xGeneralEventPayload		    		*generalEvent = NULL;
   	Tac49xFaxRelayStatusPayload				*faxRelayEventStatus = NULL;
   	Tac49xFaxOrDataSignalStatusPayload		*faxOrDataSignalStatus = NULL;

        absCh = getAbsChannelByDspChannel(pRxPacketParams->Device, pRxPacketParams->Channel);
        pDspEvent->Channel = absCh;	/* CR Note - Channel changed from DSP-channel to abs channel */

	if ( (pRxPacketParams->Protocol == PROTOCOL__PROPRIETARY) &&
		(pRxPacketParams->PacketSize != 0) &&
		(pRxPacketParams->PayloadSize != 0) &&
		(pBufEvetStatus->HpiHeader.Protocol == PROTOCOL__PROPRIETARY) &&
		(pBufEvetStatus->u.ProprietaryPayload.ProprietaryHeader.PacketCategory ==
		PACKET_CATEGORY__COMMAND_OR_STATUS_PACKET) )
	{

		opcode = pBufEvetStatus->u.ProprietaryPayload.ProprietaryHeader.Opcode;

		switch (opcode)
		{
			case STATUS_OR_EVENT_PACKET_OP_CODE__SYSTEM_ERROR_EVENT:
				error_p = &(pBufEvetStatus->u.ProprietaryPayload.u.StatusOrEvent.u.SystemErrorEvent);
				switch (error_p->ErrorMessageIndex)
				{
					case SYSTEM_ERROR_EVENT_PACKET_INDEX__OF_PACKET_ERROR:
						DSP_DRV_LogDebug("Dsp error event :: SYSTEM_ERROR_EVENT_PACKET_INDEX__OF_PACKET_ERROR\r\n");
						break;
					case SYSTEM_ERROR_EVENT_PACKET_INDEX__TIME_SLOT_MISALIGNMENT_DETECTED_AND_CORRECTED:
						DSP_DRV_LogDebug("Dsp error event :: SYSTEM_ERROR_EVENT_PACKET_INDEX__TIME_SLOT_MISALIGNMENT_DETECTED_AND_CORRECTED\r\n");
						break;
					case SYSTEM_ERROR_EVENT_PACKET_INDEX__COMMAND_SEQUENCE:
						DSP_DRV_LogDebug("Dsp error event :: SYSTEM_ERROR_EVENT_PACKET_INDEX__COMMAND_SEQUENCE\r\n");
						break;
					case SYSTEM_ERROR_EVENT_PACKET_INDEX__COMMAND_CHECKSUM:
						DSP_DRV_LogDebug("Dsp error event :: SYSTEM_ERROR_EVENT_PACKET_INDEX__COMMAND_CHECKSUM\r\n");
						break;
					case SYSTEM_ERROR_EVENT_PACKET_INDEX__RX_TX_HOST_AND_NETWORK_OVERRUN:
						DSP_DRV_LogDebug("Dsp error event :: SYSTEM_ERROR_EVENT_PACKET_INDEX__RX_TX_HOST_AND_NETWORK_OVERRUN\r\n");
						break;
					case SYSTEM_ERROR_EVENT_PACKET_INDEX__OPEN_DEVICE_PARAMETERS:
						DSP_DRV_LogDebug("Dsp error event :: SYSTEM_ERROR_EVENT_PACKET_INDEX__OPEN_DEVICE_PARAMETERS\r\n");
						break;
					case SYSTEM_ERROR_EVENT_PACKET_INDEX__ILLEGAL_COMMAND:
						DSP_DRV_LogDebug("Dsp error event :: SYSTEM_ERROR_EVENT_PACKET_INDEX__ILLEGAL_COMMAND\r\n");
						break;
					case SYSTEM_ERROR_EVENT_PACKET_INDEX__PLAYBACK_JITTER_BUFFER_UNDERRUN:
						DSP_DRV_LogDebug("Dsp error event :: SYSTEM_ERROR_EVENT_PACKET_INDEX__PLAYBACK_JITTER_BUFFER_UNDERRUN\r\n");
						break;
					case SYSTEM_ERROR_EVENT_PACKET_INDEX__PLAYBACK_JITTER_BUFFER_OVERRUN:
						DSP_DRV_LogDebug("Dsp error event :: SYSTEM_ERROR_EVENT_PACKET_INDEX__PLAYBACK_JITTER_BUFFER_OVERRUN\r\n");
						break;
					case SYSTEM_ERROR_EVENT_PACKET_INDEX__3WAY_CONFERENCE_CONFIGURATION_ERROR:
						DSP_DRV_LogDebug("Dsp error event :: SYSTEM_ERROR_EVENT_PACKET_INDEX__3WAY_CONFERENCE_CONFIGURATION_ERROR\r\n");
						break;
					case SYSTEM_ERROR_EVENT_PACKET_INDEX__ILLEGAL_UTOPIA_CELL:
						DSP_DRV_LogDebug("Dsp error event :: SYSTEM_ERROR_EVENT_PACKET_INDEX__ILLEGAL_UTOPIA_CELL\r\n");
						break;
					case SYSTEM_ERROR_EVENT_PACKET_INDEX__3WAY_CONFERENCE_OVERRUN:
						DSP_DRV_LogDebug("Dsp error event :: SYSTEM_ERROR_EVENT_PACKET_INDEX__3WAY_CONFERENCE_OVERRUN\r\n");
						break;
					case SYSTEM_ERROR_EVENT_PACKET_INDEX__ILLEGAL_PROTOCOL:
						DSP_DRV_LogDebug("Dsp error event :: SYSTEM_ERROR_EVENT_PACKET_INDEX__ILLEGAL_PROTOCOL\r\n");
						break;
					case SYSTEM_ERROR_EVENT_PACKET_INDEX__UDP_CHECKSUM_ERROR:
						DSP_DRV_LogDebug("Dsp error event :: SYSTEM_ERROR_EVENT_PACKET_INDEX__UDP_CHECKSUM_ERROR\r\n");
						break;
					case SYSTEM_ERROR_EVENT_PACKET_INDEX__WRONG_CHANNEL_ID:
						DSP_DRV_LogDebug("Dsp error event :: SYSTEM_ERROR_EVENT_PACKET_INDEX__WRONG_CHANNEL_ID\r\n");
						break;
					case SYSTEM_ERROR_EVENT_PACKET_INDEX__WRONG_PAYLOAD_SIZE:
						DSP_DRV_LogDebug("Dsp error event :: SYSTEM_ERROR_EVENT_PACKET_INDEX__WRONG_PAYLOAD_SIZE\r\n");
						break;
					case SYSTEM_ERROR_EVENT_PACKET_INDEX__WRONG_PROTOCOL:
						DSP_DRV_LogDebug("Dsp error event :: SYSTEM_ERROR_EVENT_PACKET_INDEX__WRONG_PROTOCOL\r\n");
						break;
					case SYSTEM_ERROR_EVENT_PACKET_INDEX__SERIAL_PORT_ERROR:
						DSP_DRV_LogDebug("Dsp error event :: SYSTEM_ERROR_EVENT_PACKET_INDEX__SERIAL_PORT_ERROR\r\n");
						break;
					case SYSTEM_ERROR_EVENT_PACKET_INDEX__TIMESLOT_MISALIGNMENT_DETECTED_AND_CORRECTED_DURING_INITIALIZATION:
						DSP_DRV_LogDebug("Dsp error event :: SYSTEM_ERROR_EVENT_PACKET_INDEX__TIME_SLOT_MISALIGNMENT_DETECTED_AND_CORRECTED_DURING_INIT\r\n");
						break;
					default:
						ret = AC494_DSP_ERROR_E;
						printk("Dsp event error :: unknown %d\r\n", error_p->ErrorMessageIndex);
						break;
				}

				break;

			case STATUS_OR_EVENT_PACKET_OP_CODE__GENERAL_EVENT:
				generalEvent = &(pBufEvetStatus->u.ProprietaryPayload.u.StatusOrEvent.u.GeneralEvent);

				switch(generalEvent->EventIndex)
		       		{
					case GENERAL_EVENT_PACKET_INDEX__SWITCHED_TO_FAX:
						DSP_DRV_LogDebug("Dsp general event :: GENERAL_EVENT_PACKET_INDEX__SWITCHED_TO_FAX\r\n");

						pDspEvent->EventType = ACG_EVENT_TYPE__FAX_START;
						pDspEvent->EventInfo.FaxModem.InitiatingFlag = generalEvent->u.StateSignalingEventPayload.InitiatingFlag;

						break;

					case GENERAL_EVENT_PACKET_INDEX__SWITCHED_TO_BYPASS:
						DSP_DRV_LogDebug("Dsp general event :: GENERAL_EVENT_PACKET_INDEX__SWITCHED_TO_BYPASS\r\n");

						pDspEvent->EventType = ACG_EVENT_TYPE__FAX_OR_MODEM_START;
						pDspEvent->EventInfo.FaxModem.InitiatingFlag = generalEvent->u.StateSignalingEventPayload.InitiatingFlag;

						break;

					case GENERAL_EVENT_PACKET_INDEX__SWITCHED_TO_VOICE:
						DSP_DRV_LogDebug("Dsp general event :: GENERAL_EVENT_PACKET_INDEX__SWITCHED_TO_VOICE\r\n");

						pDspEvent->EventType = ACG_EVENT_TYPE__FAX_OR_MODEM_END;
						pDspEvent->EventInfo.FaxModem.InitiatingFlag = generalEvent->u.StateSignalingEventPayload.InitiatingFlag;

						break;

					case GENERAL_EVENT_PACKET_INDEX__CAS_STRING_GENERATION_ENDED:
						DSP_DRV_LogDebug("Dsp general event :: GENERAL_EVENT_PACKET_INDEX__CAS_STRING_GENERATION_ENDED\r\n");
						break;
					case GENERAL_EVENT_PACKET_INDEX__IBS_STRING_GENERATION_ENDED:
						DSP_DRV_LogDebug("Dsp general event :: GENERAL_EVENT_PACKET_INDEX__IBS_STRING_GENERATION_ENDED\r\n");
						break;
					case GENERAL_EVENT_PACKET_INDEX__BROKEN_CONNECTION:
						DSP_DRV_LogDebug("Dsp general event :: GENERAL_EVENT_PACKET_INDEX__BROKEN_CONNECTION\r\n");
						break;
					case GENERAL_EVENT_PACKET_INDEX__RESTORE_CONNECTION:
						DSP_DRV_LogDebug("Dsp general event :: GENERAL_EVENT_PACKET_INDEX__RESTORE_CONNECTION\r\n");
						break;
					case GENERAL_EVENT_PACKET_INDEX__PLAYBACK_ENDED:
						DSP_DRV_LogDebug("Dsp general event :: GENERAL_EVENT_PACKET_INDEX__PLAYBACK_ENDED\r\n");
						break;
					case GENERAL_EVENT_PACKET_INDEX__IBS_STRING_GENERATION_ABORTED:
						DSP_DRV_LogDebug("Dsp general event :: GENERAL_EVENT_PACKET_INDEX__IBS_STRING_GENERATION_ABORTED\r\n");
						break;
					case GENERAL_EVENT_PACKET_INDEX__BROKEN_PACKET_2_PACKET_CONNECTION:
						DSP_DRV_LogDebug("Dsp general event :: GENERAL_EVENT_PACKET_INDEX__BROKEN_PACKET_2_PACKET_CONNECTION\r\n");
						break;
					case GENERAL_EVENT_PACKET_INDEX__SWITCHED_TO_DATA:
						DSP_DRV_LogDebug("Dsp general event :: GENERAL_EVENT_PACKET_INDEX__SWITCHED_TO_DATA\r\n");
						break;
					case GENERAL_EVENT_PACKET_INDEX__RESET_SERIAL_PORT_ACKNOWLEDGE:
						DSP_DRV_LogDebug("Dsp general event :: GENERAL_EVENT_PACKET_INDEX__RESET_SERIAL_PORT_ACKNOWLEDGE\r\n");
						break;
					case GENERAL_EVENT_PACKET_INDEX__TDM_RECORD_STARTED:
						DSP_DRV_LogDebug("Dsp general event :: GENERAL_EVENT_PACKET_INDEX__TDM_RECORD_STARTED\r\n");
						break;
					case GENERAL_EVENT_PACKET_INDEX__TDM_RECORD_ENDED:
						DSP_DRV_LogDebug("Dsp general event :: GENERAL_EVENT_PACKET_INDEX__TDM_RECORD_ENDED\r\n");
						break;
					case GENERAL_EVENT_PACKET_INDEX__NETWORK_RECORD_STARTED:
						DSP_DRV_LogDebug("Dsp general event :: GENERAL_EVENT_PACKET_INDEX__NETWORK_RECORD_STARTED\r\n");
						break;
					case GENERAL_EVENT_PACKET_INDEX__NETWORK_RECORD_ENDED:
						DSP_DRV_LogDebug("Dsp general event :: GENERAL_EVENT_PACKET_INDEX__NETWORK_RECORD_ENDED\r\n");
						break;
					case GENERAL_EVENT_PACKET_INDEX__TDM_INBAND_TEXT_RELAY_STARTED:
						DSP_DRV_LogDebug("Dsp general event :: GENERAL_EVENT_PACKET_INDEX__TDM_INBAND_TEXT_RELAY_STARTED\r\n");
						break;
					case GENERAL_EVENT_PACKET_INDEX__TDM_INBAND_TEXT_RELAY_ENDED:
						DSP_DRV_LogDebug("Dsp general event :: GENERAL_EVENT_PACKET_INDEX__TDM_INBAND_TEXT_RELAY_ENDED\r\n");
						break;
					case GENERAL_EVENT_PACKET_INDEX__NETWORK_INBAND_TEXT_RELAY_STARTED:
						DSP_DRV_LogDebug("Dsp general event :: GENERAL_EVENT_PACKET_INDEX__NETWORK_INBAND_TEXT_RELAY_STARTED\r\n");
						break;
					case GENERAL_EVENT_PACKET_INDEX__NETWORK_INBAND_TEXT_RELAY_ENDED:
						DSP_DRV_LogDebug("Dsp general event :: GENERAL_EVENT_PACKET_INDEX__NETWORK_INBAND_TEXT_RELAY_ENDED\r\n");
						break;
					case GENERAL_EVENT_PACKET_INDEX__INBAND_TEXT_RELAY_TERMINATED:
						DSP_DRV_LogDebug("Dsp general event :: GENERAL_EVENT_PACKET_INDEX__INBAND_TEXT_RELAY_TERMINATED\r\n");
						break;
					case GENERAL_EVENT_PACKET_INDEX__TDM_SERIAL_PORT_HALTED:
						DSP_DRV_LogDebug("Dsp general event :: GENERAL_EVENT_PACKET_INDEX__TDM_SERIAL_PORT_HALTED\r\n");
						break;
					case GENERAL_EVENT_PACKET_INDEX__TDM_SERIAL_PORT_RESUMED:
						DSP_DRV_LogDebug("Dsp general event :: GENERAL_EVENT_PACKET_INDEX__TDM_SERIAL_PORT_RESUMED\r\n");
						break;
					case GENERAL_EVENT_PACKET_INDEX__ANSWER_DETECTOR_DETECTED_SPEECH:
						DSP_DRV_LogDebug("Dsp general event :: GENERAL_EVENT_PACKET_INDEX__ANSWER_DETECTOR_DETECTED_SPEECH\r\n");
						break;
					case GENERAL_EVENT_PACKET_INDEX__ANSWER_DETECTOR_DETECTED_END_OF_SPEECH:
						DSP_DRV_LogDebug("Dsp general event :: GENERAL_EVENT_PACKET_INDEX__ANSWER_DETECTOR_DETECTED_END_OF_SPEECH\r\n");
						break;
					case GENERAL_EVENT_PACKET_INDEX__ANSWER_MACHINE_DETECTOR_STATUS_LIVE_CALL:
						DSP_DRV_LogDebug("Dsp general event :: GENERAL_EVENT_PACKET_INDEX__ANSWER_MACHINE_DETECTOR_STATUS_LIVE_CALL\r\n");
						break;
					case GENERAL_EVENT_PACKET_INDEX__ANSWER_MACHINE_DETECTOR_STATUS_ANSWERING_MACHINE:
						DSP_DRV_LogDebug("Dsp general event :: GENERAL_EVENT_PACKET_INDEX__ANSWER_MACHINE_DETECTOR_STATUS_ANSWERING_MACHINE\r\n");
						break;
					case GENERAL_EVENT_PACKET_INDEX__ANSWER_MACHINE_DETECTOR_STATUS_NO_DECISION:
						DSP_DRV_LogDebug("Dsp general event :: GENERAL_EVENT_PACKET_INDEX__ANSWER_MACHINE_DETECTOR_STATUS_NO_DECISION\r\n");
						break;
					case GENERAL_EVENT_PACKET_INDEX__ANSWER_MACHINE_DETECTOR_STATUS_SILENT_CALL:
						DSP_DRV_LogDebug("Dsp general event :: GENERAL_EVENT_PACKET_INDEX__ANSWER_MACHINE_DETECTOR_STATUS_SILENT_CALL\r\n");
						break;
					case GENERAL_EVENT_PACKET_INDEX__ANSWER_MACHINE_DETECTOR_STATUS_BEEP_DETECTED:
						DSP_DRV_LogDebug("Dsp general event :: GENERAL_EVENT_PACKET_INDEX__ANSWER_MACHINE_DETECTOR_STATUS_BEEP_DETECTED\r\n");
						break;
					case GENERAL_EVENT_PACKET_INDEX__ANSWER_MACHINE_DETECTOR_STATUS_BEEP_TIMEOUT_EXPIRED:
						DSP_DRV_LogDebug("Dsp general event :: GENERAL_EVENT_PACKET_INDEX__ANSWER_MACHINE_DETECTOR_STATUS_BEEP_TIMEOUT_EXPIRED\r\n");
						break;
					case GENERAL_EVENT_PACKET_INDEX__H324_START_DETECTION:
						DSP_DRV_LogDebug("Dsp general event :: GENERAL_EVENT_PACKET_INDEX__H324_START_DETECTION\r\n");
						break;
					case GENERAL_EVENT_PACKET_INDEX__H324_END_DETECTION:
						DSP_DRV_LogDebug("Dsp general event :: GENERAL_EVENT_PACKET_INDEX__H324_END_DETECTION\r\n");
						break;
					default:
						ret = AC494_DSP_ERROR_E;
						printk("Dsp general event error :: unknown %d\r\n", generalEvent->EventIndex);
						break;

					break;
				}

			case STATUS_OR_EVENT_PACKET_OP_CODE__IBS_EVENT:

				ibsevent = &(pBufEvetStatus->u.ProprietaryPayload.u.StatusOrEvent.u.IbsEvent);

				DSP_DRV_LogDebug("ibs time=%d , dir=%d, dig=%d, sys=%d, lev=%d\n",
					(int)MergeFieldsToLong(ibsevent->TimeStamp),
					ibsevent->Direction, ibsevent->Digit, ibsevent->System,
					ibsevent->Level);

				if(absCh<0)
				{
					printk("Dsp event opcode :: STATUS_OR_EVENT_PACKET_OP_CODE__IBS_EVENT, absCh is negative (%d)\r\n", absCh);
					break;
				}

			        if (IBS_SYSTEM_CODE__DTMF == ibsevent->System)
			        {
			            	dspChannelsAttributes[absCh].last_digit_pressed=ibsevent->Digit;		/*NIRP 061228 save the info for the ibs event which is received when the digit is unpressed */
					pDspEvent->EventType = ACG_EVENT_TYPE__DTMF_START;
					pDspEvent->EventInfo.Tone.Type = ACG_TONE_TYPE__DTMF;
					pDspEvent->EventInfo.Tone.ToneIndex = ibsevent->Digit;
					pDspEvent->EventInfo.Tone.Direction = ibsevent->Direction;

			        }
			        if ((IBS_SYSTEM_CODE__NONE == ibsevent->System) &&
						(ibsevent->Digit == dspChannelsAttributes[absCh].last_digit_pressed))	/*Check if the digit was unpressed */
			        {
			            	dspChannelsAttributes[absCh].last_digit_pressed=-1;
					pDspEvent->EventType = ACG_EVENT_TYPE__DTMF_END;
					pDspEvent->EventInfo.Tone.Type = ACG_TONE_TYPE__DTMF;
					pDspEvent->EventInfo.Tone.ToneIndex = ibsevent->Digit;
					pDspEvent->EventInfo.Tone.Direction = ibsevent->Direction;
			        }

				ibsevent = NULL;

				break;

			case STATUS_OR_EVENT_PACKET_OP_CODE__CAS_EVENT:
					DSP_DRV_LogDebug("Dsp event opcode :: STATUS_OR_EVENT_PACKET_OP_CODE__CAS_EVENT\r\n");
				break;
			case STATUS_OR_EVENT_PACKET_OP_CODE__HDLC_FRAMER_STATUS:
					DSP_DRV_LogDebug("Dsp event opcode :: STATUS_OR_EVENT_PACKET_OP_CODE__HDLC_FRAMER_STATUS\r\n");
				break;

			case STATUS_OR_EVENT_PACKET_OP_CODE__FAX_RELAY_STATUS:
				DSP_DRV_LogDebug("Dsp event opcode :: STATUS_OR_EVENT_PACKET_OP_CODE__FAX_RELAY_STATUS\r\n");

				faxRelayEventStatus = &(pBufEvetStatus->u.ProprietaryPayload.u.StatusOrEvent.u.FaxRelayStatus);

				switch(faxRelayEventStatus->T30RxState)
				{
					case FAX_RELAY_T30_STATE__INITIALIZATION:
						DSP_DRV_LogDebug("Dsp fax state :: FAX_RELAY_T30_STATE__INITIALIZATION\r\n");
						break;
					case FAX_RELAY_T30_STATE__CNG:
						DSP_DRV_LogDebug("Dsp fax state :: FAX_RELAY_T30_STATE__CNG\r\n");
						break;
					case FAX_RELAY_T30_STATE__CED:
						DSP_DRV_LogDebug("Dsp fax state :: FAX_RELAY_T30_STATE__CED\r\n");
						break;
					case FAX_RELAY_T30_STATE__V21:
						DSP_DRV_LogDebug("Dsp fax state :: FAX_RELAY_T30_STATE__V21\r\n");
						break;
					case FAX_RELAY_T30_STATE__NSF:
						DSP_DRV_LogDebug("Dsp fax state :: FAX_RELAY_T30_STATE__NSF\r\n");
						break;
					case FAX_RELAY_T30_STATE__NSC:
						DSP_DRV_LogDebug("Dsp fax state :: FAX_RELAY_T30_STATE__NSC\r\n");
						break;
					case FAX_RELAY_T30_STATE__CSI:
						DSP_DRV_LogDebug("Dsp fax state :: FAX_RELAY_T30_STATE__CSI\r\n");
						break;
					case FAX_RELAY_T30_STATE__CIG:
						DSP_DRV_LogDebug("Dsp fax state :: FAX_RELAY_T30_STATE__CIG\r\n");
						break;
					case FAX_RELAY_T30_STATE__DIS:
						DSP_DRV_LogDebug("Dsp fax state :: FAX_RELAY_T30_STATE__DIS\r\n");
						break;
					case FAX_RELAY_T30_STATE__DTC:
						DSP_DRV_LogDebug("Dsp fax state :: FAX_RELAY_T30_STATE__DTC\r\n");
						break;
					case FAX_RELAY_T30_STATE__NSS:
						DSP_DRV_LogDebug("Dsp fax state :: FAX_RELAY_T30_STATE__NSS\r\n");
						break;
					case FAX_RELAY_T30_STATE__TSI:
						DSP_DRV_LogDebug("Dsp fax state :: FAX_RELAY_T30_STATE__TSI\r\n");
						break;
					case FAX_RELAY_T30_STATE__DCS:
						DSP_DRV_LogDebug("Dsp fax state :: FAX_RELAY_T30_STATE__DCS\r\n");
						break;
					case FAX_RELAY_T30_STATE__CTC:
						DSP_DRV_LogDebug("Dsp fax state :: FAX_RELAY_T30_STATE__CTC\r\n");
						break;
					case FAX_RELAY_T30_STATE__CRP:
						DSP_DRV_LogDebug("Dsp fax state :: FAX_RELAY_T30_STATE__CRP\r\n");
						break;
					case FAX_RELAY_T30_STATE__DCN:
						DSP_DRV_LogDebug("Dsp fax state :: FAX_RELAY_T30_STATE__DCN\r\n");
						break;
					case FAX_RELAY_T30_STATE__PRE_MESSAGE_RESPONSE:
						DSP_DRV_LogDebug("Dsp fax state :: FAX_RELAY_T30_STATE__PRE_MESSAGE_RESPONSE\r\n");
						break;
					case FAX_RELAY_T30_STATE__POST_MESSAGE_RESPONSE:
						DSP_DRV_LogDebug("Dsp fax state :: FAX_RELAY_T30_STATE__POST_MESSAGE_RESPONSE\r\n");
						break;
					case FAX_RELAY_T30_STATE__POST_MESSAGE_COMMAND:
						DSP_DRV_LogDebug("Dsp fax state :: FAX_RELAY_T30_STATE__POST_MESSAGE_COMMAND\r\n");
						break;
					case FAX_RELAY_T30_STATE__VXX:
						DSP_DRV_LogDebug("Dsp fax state :: FAX_RELAY_T30_STATE__VXX\r\n");
						break;
					case FAX_RELAY_T30_STATE__TCF:
						DSP_DRV_LogDebug("Dsp fax state :: FAX_RELAY_T30_STATE__TCF\r\n");
						break;
					case FAX_RELAY_T30_STATE__IMAGE:
						DSP_DRV_LogDebug("Dsp fax state :: FAX_RELAY_T30_STATE__IMAGE\r\n");
						break;
					case FAX_RELAY_T30_STATE__V8_CI:
						DSP_DRV_LogDebug("Dsp fax state :: FAX_RELAY_T30_STATE__V8_CI\r\n");
						break;
					case FAX_RELAY_T30_STATE__V8_CM:
						DSP_DRV_LogDebug("Dsp fax state :: FAX_RELAY_T30_STATE__V8_CM\r\n");
						break;
					case FAX_RELAY_T30_STATE__V8_JM:
						DSP_DRV_LogDebug("Dsp fax state :: FAX_RELAY_T30_STATE__V8_JM\r\n");
						break;
					default:
						printk("Dsp fax state :: unknown\r\n");
						break;
				}

				break;

			case STATUS_OR_EVENT_PACKET_OP_CODE__JITTER_BUFFER_STATUS:
				DSP_DRV_LogDebug("Dsp event opcode :: STATUS_OR_EVENT_PACKET_OP_CODE__JITTER_BUFFER_STATUS\r\n");
				jitter = &(pBufEvetStatus->u.ProprietaryPayload.u.StatusOrEvent.u.JitterBufferStatus);

				DSP_DRV_LogDebug("jitter buffer event time=%d\r\n", (int)MergeFieldsToLong(jitter->TimeStamp));

			break;

			case STATUS_OR_EVENT_PACKET_OP_CODE__CALLER_ID:
				DSP_DRV_LogDebug("Dsp event opcode :: STATUS_OR_EVENT_PACKET_OP_CODE__CALLER_ID\r\n");

				callerid = &(pBufEvetStatus->u.ProprietaryPayload.u.StatusOrEvent.u.CallerId);

				if(callerid->Event == CALLER_ID_EVENT__GENERATION_COMPLETED_SUCCESSFULLY)
				{
					pDspEvent->EventType = ACG_EVENT_TYPE__CALLER_ID_END;
				}

				DSP_DRV_LogDebug("caller id event standard=%d, event=%d\n", callerid->Standard, callerid->Event);

				callerid = NULL;

				break;

			case STATUS_OR_EVENT_PACKET_OP_CODE__PACKETIZER_STATUS_OR_EVENT:
				DSP_DRV_LogDebug("Dsp event opcode :: STATUS_OR_EVENT_PACKET_OP_CODE__PACKETIZER_STATUS_OR_EVENT\r\n");

				packetizer = &(pBufEvetStatus->u.ProprietaryPayload.u.StatusOrEvent.u.Packetizer);

				switch (packetizer->StatusOrEventIndex)
				{
					case PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_RECEIVE_RTCP_STATUS_OR_EVENT:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_RECEIVE_RTCP_STATUS_OR_EVENT\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_RECEIVE_RTCP_STATUS_OR_EVENT:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_RECEIVE_RTCP_STATUS_OR_EVENT\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_3GPP_USER_PLANE_STATE_EVENT:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_3GPP_USER_PLANE_STATE_EVENT\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_3GPP_USER_PLANE_ERROR_EVENT:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_3GPP_USER_PLANE_ERROR_EVENT\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_3GPP_USER_PLANE_STATISTIC_STATUS:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_3GPP_USER_PLANE_STATISTIC_STATUS\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_3GPP_USER_PLANE_SELECT_RATE_EVENT:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_3GPP_USER_PLANE_SELECT_RATE_EVENT\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_PACKETIZER_ERROR_EVENT:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_PACKETIZER_ERROR_EVENT\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__RFC3267_AMR_SELECT_RATE_EVENT:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__RFC3267_AMR_SELECT_RATE_EVENT\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_TRANSMIT_RTCP_STATUS_OR_EVENT:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_TRANSMIT_RTCP_STATUS_OR_EVENT\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_TRANSMIT_RTCP_STATUS_OR_EVENT:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_TRANSMIT_RTCP_STATUS_OR_EVENT\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_3GPP_USER_PLANE_STATE_EVENT:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_3GPP_USER_PLANE_STATE_EVENT\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_3GPP_USER_PLANE_ERROR_EVENT:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_3GPP_USER_PLANE_ERROR_EVENT\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_3GPP_USER_PLANE_STATISTIC_STATUS:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_3GPP_USER_PLANE_STATISTIC_STATUS\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_3GPP_USER_PLANE_SELECT_RATE_EVENT:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_3GPP_USER_PLANE_SELECT_RATE_EVENT\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_PACKETIZER_ERROR_EVENT:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_PACKETIZER_ERROR_EVENT\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_BROKEN_CONNECTION_EVENT:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_BROKEN_CONNECTION_EVENT\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_BROKEN_CONNECTION_EVENT:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_BROKEN_CONNECTION_EVENT\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_CONNECTION_ESTABLISH_EVENT:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_CONNECTION_ESTABLISH_EVENT\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_CONNECTION_ESTABLISH_EVENT:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_CONNECTION_ESTABLISH_EVENT\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_REMOTE_SSRC_CHANGED_EVENT:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_REMOTE_SSRC_CHANGED_EVENT\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_REMOTE_SSRC_CHANGED_EVENT:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_REMOTE_SSRC_CHANGED_EVENT\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_MEDIA_PROTECTION_INFO_EVENT:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_MEDIA_PROTECTION_INFO_EVENT\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_MEDIA_PROTECTION_INFO_EVENT:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_MEDIA_PROTECTION_INFO_EVENT\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__RFC3558_RATE_MODE_EVENT:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__RFC3558_RATE_MODE_EVENT\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_RTCP_APP_EVENT:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_RTCP_APP_EVENT\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_RTCP_APP_EVENT:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_RTCP_APP_EVENT\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__G729EV_RATE_EVENT:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__G729EV_RATE_EVENT\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__HANDOUT_EVENT:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__HANDOUT_EVENT\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__3GPP_IUB_STATUS:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__3GPP_IUB_STATUS\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_SENDER_RTCP_XR_VOICE_QUALITY_STATISTICS:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_SENDER_RTCP_XR_VOICE_QUALITY_STATISTICS\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_SENDER_RTCP_XR_VOICE_QUALITY_STATISTICS:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_SENDER_RTCP_XR_VOICE_QUALITY_STATISTICS\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_RECEIVER_RTCP_XR_VOICE_QUALITY_STATISTICS:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_RECEIVER_RTCP_XR_VOICE_QUALITY_STATISTICS\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_RECEIVER_RTCP_XR_VOICE_QUALITY_STATISTICS:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_RECEIVER_RTCP_XR_VOICE_QUALITY_STATISTICS\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_RTCP_XR_MARKOV_MODEL_STATISTICS:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_RTCP_XR_MARKOV_MODEL_STATISTICS\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_RTCP_XR_MARKOV_MODEL_STATISTICS:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_RTCP_XR_MARKOV_MODEL_STATISTICS\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_RTCP_XR_ALERT_STATUS:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_RTCP_XR_ALERT_STATUS\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_RTCP_XR_ALERT_STATUS:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_RTCP_XR_ALERT_STATUS\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_RTP_SEQUENCE_NUMBER_RECEIVED:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__REGULAR_CHANNEL_RTP_SEQUENCE_NUMBER_RECEIVED\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_RTP_SEQUENCE_NUMBER_RECEIVED:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__MEDIATED_CHANNEL_RTP_SEQUENCE_NUMBER_RECEIVED\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__SWITCH_TO_MODEM:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__SWITCH_TO_MODEM\r\n");
						break;
					case PACKETIZER_STATUS_OR_EVENT_INDEX__MODEM_RELAY_SSE:
						DSP_DRV_LogDebug("Dsp packetizer event :: PACKETIZER_STATUS_OR_EVENT_INDEX__MODEM_RELAY_SSE\r\n");
						break;
					default:
						ret = AC494_DSP_ERROR_E;
						printk("Dsp packetizer event :: unknown %d\r\n", packetizer->StatusOrEventIndex);
						break;
				}

				break;

			case STATUS_OR_EVENT_PACKET_OP_CODE__DEVICE_STATUS:
				DSP_DRV_LogDebug("Dsp event opcode :: STATUS_OR_EVENT_PACKET_OP_CODE__DEVICE_STATUS\r\n");
				break;
			case STATUS_OR_EVENT_PACKET_OP_CODE__DEBUG_INFORMATION_STATUS:
				DSP_DRV_LogDebug("Dsp event opcode :: STATUS_OR_EVENT_PACKET_OP_CODE__DEBUG_INFORMATION_STATUS\r\n");
				break;
			case STATUS_OR_EVENT_PACKET_OP_CODE__FAX_OR_DATA_SIGNAL_EVENT:
				DSP_DRV_LogDebug("Dsp event opcode :: STATUS_OR_EVENT_PACKET_OP_CODE__FAX_OR_DATA_SIGNAL_EVENT\r\n");
				faxOrDataSignalStatus = &(pBufEvetStatus->u.ProprietaryPayload.u.StatusOrEvent.u.FaxOrDataStatus);
				if(faxOrDataSignalStatus->FaxBypassFlag)
					DSP_DRV_LogDebug("Dsp fax,data signal :: fax bypass is active\r\n");
				else
					DSP_DRV_LogDebug("Dsp fax,data signal :: Voice ,data bypass or fax relay\r\n");

				switch(faxOrDataSignalStatus->SignalDetectedOnDecoderOutput)
				{
					case FAX_OR_DATA_SIGNAL_EVENT__SILENCE_OR_UNKNOWN:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on decode output=>FAX_OR_DATA_SIGNAL_EVENT__SILENCE_OR_UNKNOWN\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__FAX_CNG:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on decode output=>FAX_OR_DATA_SIGNAL_EVENT__FAX_CNG\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__ANS_TONE_2100_FAX_CED_OR_MODEM:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on decode output=>FAX_OR_DATA_SIGNAL_EVENT__ANS_TONE_2100_FAX_CED_OR_MODEM\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__ANS_TONE_WITH_REVERSALS:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on decode output=>FAX_OR_DATA_SIGNAL_EVENT__ANS_TONE_WITH_REVERSALS\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__ANS_TONE_AM:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on decode output=>FAX_OR_DATA_SIGNAL_EVENT__ANS_TONE_AM\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__ANS_TONE_AM_REVERSALS:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on decode output=>FAX_OR_DATA_SIGNAL_EVENT__ANS_TONE_AM_REVERSALS\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__FAX_V21_PREAMBLE_FLAGS:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on decode output=>FAX_OR_DATA_SIGNAL_EVENT__FAX_V21_PREAMBLE_FLAGS\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__FAX_V8_JM_V34:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on decode output=>FAX_OR_DATA_SIGNAL_EVENT__FAX_V8_JM_V34\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__VXX_V8_JM_VXX_DATA:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on decode output=>FAX_OR_DATA_SIGNAL_EVENT__VXX_V8_JM_VXX_DATA\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__V32_AA:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on decode output=>FAX_OR_DATA_SIGNAL_EVENT__V32_AA\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__V22_USB1:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on decode output=>FAX_OR_DATA_SIGNAL_EVENT__V22_USB1\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__V8_BIS_INITIATING_DUAL_TONE:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on decode output=>FAX_OR_DATA_SIGNAL_EVENT__V8_BIS_INITIATING_DUAL_TONE\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__V8_BIS_RESPONDING_DUAL_TONE:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on decode output=>FAX_OR_DATA_SIGNAL_EVENT__V8_BIS_RESPONDING_DUAL_TONE\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__VXX_DATA_SESSION:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on decode output=>FAX_OR_DATA_SIGNAL_EVENT__VXX_DATA_SESSION\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__V21_CHANNEL_2:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on decode output=>FAX_OR_DATA_SIGNAL_EVENT__V21_CHANNEL_2\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__V23_FORWARD_CHANNEL:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on decode output=>FAX_OR_DATA_SIGNAL_EVENT__V23_FORWARD_CHANNEL\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__STU_MODEM:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on decode output=>FAX_OR_DATA_SIGNAL_EVENT__STU_MODEM\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__V25_CALLING_TONE:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on decode output=>FAX_OR_DATA_SIGNAL_EVENT__V25_CALLING_TONE\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__V21_CHANNEL_1:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on decode output=>FAX_OR_DATA_SIGNAL_EVENT__V21_CHANNEL_1\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__BELL_103_ANSWER_TONE:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on decode output=>FAX_OR_DATA_SIGNAL_EVENT__BELL_103_ANSWER_TONE\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__TTY:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on decode output=>FAX_OR_DATA_SIGNAL_EVENT__TTY\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__FAX_DCN:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on decode output=>FAX_OR_DATA_SIGNAL_EVENT__FAX_DCN\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__FAX_V8_CI:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on decode output=>FAX_OR_DATA_SIGNAL_EVENT__FAX_V8_CI\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__FAX_V8_CM:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on decode output=>FAX_OR_DATA_SIGNAL_EVENT__FAX_V8_CM\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__VXX_V8_CI:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on decode output=>FAX_OR_DATA_SIGNAL_EVENT__VXX_V8_CI\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__VXX_V8_CM:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on decode output=>FAX_OR_DATA_SIGNAL_EVENT__VXX_V8_CM\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__TEXT_V8_CI:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on decode output=>FAX_OR_DATA_SIGNAL_EVENT__TEXT_V8_CI\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__TEXT_V8_CM:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on decode output=>FAX_OR_DATA_SIGNAL_EVENT__TEXT_V8_CM\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__TEXT_V8_JM:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on decode output=>FAX_OR_DATA_SIGNAL_EVENT__TEXT_V8_JM\r\n");
						break;
					default:
						printk("Dsp fax,data signal :: signal detected on decode output=>unknown\r\n");
						break;
				}

				switch(faxOrDataSignalStatus->SignalDetectedOnEncoderInput)
				{
					case FAX_OR_DATA_SIGNAL_EVENT__SILENCE_OR_UNKNOWN:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on encode input=>FAX_OR_DATA_SIGNAL_EVENT__SILENCE_OR_UNKNOWN\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__FAX_CNG:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on encode input=>FAX_OR_DATA_SIGNAL_EVENT__FAX_CNG\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__ANS_TONE_2100_FAX_CED_OR_MODEM:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on encode input=>FAX_OR_DATA_SIGNAL_EVENT__ANS_TONE_2100_FAX_CED_OR_MODEM\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__ANS_TONE_WITH_REVERSALS:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on encode input=>FAX_OR_DATA_SIGNAL_EVENT__ANS_TONE_WITH_REVERSALS\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__ANS_TONE_AM:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on encode input=>FAX_OR_DATA_SIGNAL_EVENT__ANS_TONE_AM\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__ANS_TONE_AM_REVERSALS:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on encode input=>FAX_OR_DATA_SIGNAL_EVENT__ANS_TONE_AM_REVERSALS\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__FAX_V21_PREAMBLE_FLAGS:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on encode input=>FAX_OR_DATA_SIGNAL_EVENT__FAX_V21_PREAMBLE_FLAGS\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__FAX_V8_JM_V34:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on encode input=>FAX_OR_DATA_SIGNAL_EVENT__FAX_V8_JM_V34\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__VXX_V8_JM_VXX_DATA:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on encode input=>FAX_OR_DATA_SIGNAL_EVENT__VXX_V8_JM_VXX_DATA\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__V32_AA:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on encode input=>FAX_OR_DATA_SIGNAL_EVENT__V32_AA\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__V22_USB1:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on encode input=>FAX_OR_DATA_SIGNAL_EVENT__V22_USB1\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__V8_BIS_INITIATING_DUAL_TONE:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on encode input=>FAX_OR_DATA_SIGNAL_EVENT__V8_BIS_INITIATING_DUAL_TONE\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__V8_BIS_RESPONDING_DUAL_TONE:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on encode input=>FAX_OR_DATA_SIGNAL_EVENT__V8_BIS_RESPONDING_DUAL_TONE\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__VXX_DATA_SESSION:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on encode input=>FAX_OR_DATA_SIGNAL_EVENT__VXX_DATA_SESSION\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__V21_CHANNEL_2:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on encode input=>FAX_OR_DATA_SIGNAL_EVENT__V21_CHANNEL_2\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__V23_FORWARD_CHANNEL:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on encode input=>FAX_OR_DATA_SIGNAL_EVENT__V23_FORWARD_CHANNEL\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__STU_MODEM:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on encode input=>FAX_OR_DATA_SIGNAL_EVENT__STU_MODEM\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__V25_CALLING_TONE:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on encode input=>FAX_OR_DATA_SIGNAL_EVENT__V25_CALLING_TONE\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__V21_CHANNEL_1:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on encode input=>FAX_OR_DATA_SIGNAL_EVENT__V21_CHANNEL_1\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__BELL_103_ANSWER_TONE:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on encode input=>FAX_OR_DATA_SIGNAL_EVENT__BELL_103_ANSWER_TONE\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__TTY:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on encode input=>FAX_OR_DATA_SIGNAL_EVENT__TTY\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__FAX_DCN:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on encode input=>FAX_OR_DATA_SIGNAL_EVENT__FAX_DCN\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__FAX_V8_CI:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on encode input=>FAX_OR_DATA_SIGNAL_EVENT__FAX_V8_CI\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__FAX_V8_CM:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on encode input=>FAX_OR_DATA_SIGNAL_EVENT__FAX_V8_CM\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__VXX_V8_CI:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on encode input=>FAX_OR_DATA_SIGNAL_EVENT__VXX_V8_CI\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__VXX_V8_CM:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on encode input=>FAX_OR_DATA_SIGNAL_EVENT__VXX_V8_CM\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__TEXT_V8_CI:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on encode input=>FAX_OR_DATA_SIGNAL_EVENT__TEXT_V8_CI\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__TEXT_V8_CM:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on encode input=>FAX_OR_DATA_SIGNAL_EVENT__TEXT_V8_CM\r\n");
						break;
					case FAX_OR_DATA_SIGNAL_EVENT__TEXT_V8_JM:
						DSP_DRV_LogDebug("Dsp fax,data signal :: signal detected on encode input=>FAX_OR_DATA_SIGNAL_EVENT__TEXT_V8_JM\r\n");
						break;
					default:
						printk("Dsp fax,data signal :: signal detected on encode input=>unknown\r\n");
						break;
				}

		                break;

			case STATUS_OR_EVENT_PACKET_OP_CODE__SS7_MTP1:
				DSP_DRV_LogDebug("Dsp event opcode :: STATUS_OR_EVENT_PACKET_OP_CODE__SS7_MTP1\r\n");
				break;
			case STATUS_OR_EVENT_PACKET_OP_CODE__UTOPIA_TEST_ERROR_EVENT:
				DSP_DRV_LogDebug("Dsp event opcode :: STATUS_OR_EVENT_PACKET_OP_CODE__UTOPIA_TEST_ERROR_EVENT\r\n");
				break;
			case STATUS_OR_EVENT_PACKET_OP_CODE__TRUNK_TESTING_MEASUREMENT_EVENT:
				DSP_DRV_LogDebug("Dsp event opcode :: STATUS_OR_EVENT_PACKET_OP_CODE__TRUNK_TESTING_MEASUREMENT_EVENT\r\n");
				break;
			case STATUS_OR_EVENT_PACKET_OP_CODE__BER_TEST_REPORT_EVENT:
				DSP_DRV_LogDebug("Dsp event opcode :: STATUS_OR_EVENT_PACKET_OP_CODE__BER_TEST_REPORT_EVENT\r\n");
				break;
			case STATUS_OR_EVENT_PACKET_OP_CODE__UTOPIA_STATUS:
				DSP_DRV_LogDebug("Dsp event opcode :: STATUS_OR_EVENT_PACKET_OP_CODE__UTOPIA_STATUS\r\n");
				break;
			case STATUS_OR_EVENT_PACKET_OP_CODE__TANDEM_FREE_OPERATION_STATUS_AND_MESSAGES:
				DSP_DRV_LogDebug("Dsp event opcode :: STATUS_OR_EVENT_PACKET_OP_CODE__TANDEM_FREE_OPERATION_STATUS_AND_MESSAGES\r\n");
				break;
			case STATUS_OR_EVENT_PACKET_OP_CODE__MODULE_STATUS:
				DSP_DRV_LogDebug("Dsp event opcode :: STATUS_OR_EVENT_PACKET_OP_CODE__MODULE_STATUS\r\n");
				break;
			case STATUS_OR_EVENT_PACKET_OP_CODE__DUA_FRAMER:
				DSP_DRV_LogDebug("Dsp event opcode :: STATUS_OR_EVENT_PACKET_OP_CODE__DUA_FRAMER\r\n");
				break;
			case STATUS_OR_EVENT_PACKET_OP_CODE__VIDEO_AUDIO_SYNC:
				DSP_DRV_LogDebug("Dsp event opcode :: STATUS_OR_EVENT_PACKET_OP_CODE__VIDEO_AUDIO_SYNC\r\n");
				break;
			case STATUS_OR_EVENT_PACKET_OP_CODE__4_WAY_CONFERENCE_STATUS:
				DSP_DRV_LogDebug("Dsp event opcode :: STATUS_OR_EVENT_PACKET_OP_CODE__4_WAY_CONFERENCE_STATUS\r\n");
				break;
			case STATUS_OR_EVENT_PACKET_OP_CODE__4_WAY_CONFERENCE_DETAILS:
				DSP_DRV_LogDebug("Dsp event opcode :: STATUS_OR_EVENT_PACKET_OP_CODE__4_WAY_CONFERENCE_DETAILS\r\n");
				break;
			case STATUS_OR_EVENT_PACKET_OP_CODE__PROTECTION_TEST_RESPONSE:
				DSP_DRV_LogDebug("Dsp event opcode :: STATUS_OR_EVENT_PACKET_OP_CODE__PROTECTION_TEST_RESPONSE\r\n");
				break;
			case STATUS_OR_EVENT_PACKET_OP_CODE__RANDOM_NUMBER_RESPONSE:
				DSP_DRV_LogDebug("Dsp event opcode :: STATUS_OR_EVENT_PACKET_OP_CODE__RANDOM_NUMBER_RESPONSE\r\n");
				break;
			case STATUS_OR_EVENT_PACKET_OP_CODE__MODULE_DEBUG_PACKET:
				DSP_DRV_LogDebug("Dsp event opcode :: STATUS_OR_EVENT_PACKET_OP_CODE__MODULE_DEBUG_PACKET\r\n");
				break;
			case STATUS_OR_EVENT_PACKET_OP_CODE__MODEM_RELAY:
				DSP_DRV_LogDebug("Dsp event opcode :: STATUS_OR_EVENT_PACKET_OP_CODE__MODEM_RELAY\r\n");
				break;
			case STATUS_OR_EVENT_PACKET_OP_CODE__ERROR:
				DSP_DRV_LogDebug("Dsp event opcode :: STATUS_OR_EVENT_PACKET_OP_CODE__ERROR\r\n");

		                break;
			default:
				DSP_DRV_LogDebug("Dsp event opcode :: unknown\r\n");
				DSP_DRV_LogDebug("status packet: medium=%d, protocol=%d\n",
								pRxPacketParams->TransferMedium,
								pRxPacketParams->Protocol);
				DSP_DRV_LogDebug("bufEvetStatus->HpiHeader.Protocol = %d, event.Channel=%d\n",
						pBufEvetStatus->HpiHeader.Protocol, pBufEvetStatus->HpiHeader.Channel);

				DSP_DRV_LogDebug("proprietary category = %d, opcode=%d\n",
					pBufEvetStatus->u.ProprietaryPayload.ProprietaryHeader.PacketCategory,
					opcode);
		               break;
		}

	}

	return ret;

}


/****************************************************************************
*
*	Name:	InitPacketRecordingInfo
*----------------------------------------------------------------------------
*	Abstract:	Initialize packet recording information
*----------------------------------------------------------------------------
*	Input:	char 		*pRemoteRecordingIpAddress
*			int 		remoteRecordingPort
*----------------------------------------------------------------------------
*  	Output:		none
*----------------------------------------------------------------------------
*	Returns: 	case of success = 0, else -1
******************************************************************************/
static void InitPacketRecordingInfo(char *pRemoteRecordingIpAddress, int remoteRecordingPort)
{
	if(strlen(pRemoteRecordingIpAddress) < ADDRESS_STR_MAX_LEN)
	{
		strcpy(recordingRemoteIpAddress, pRemoteRecordingIpAddress);
 		recordingRemotePort = remoteRecordingPort;
	}
}

static void SetPacketRecordingStatus(Tac49xControl status)
{
	packetRecordingStatus = status;
}
static void SetNetworkRecordingStatus(Tac49xControl status)
{
	networkRecordingStatus = status;
}
static void SetTDMRecordingStatus(Tac49xControl status)
{
	tdmRecordingStatus = status;
}


static int IsRecordingEnable(Tac49xProtocol protocol)
{
	Tac49xControl	status = CONTROL__DISABLE;

	switch(protocol)
{
		case PROTOCOL__PROPRIETARY:
		case PROTOCOL__PLAYBACK_COMMAND:
		case PROTOCOL__PLAYBACK_VOICE:
		case PROTOCOL__RTP:
		case PROTOCOL__RTCP:
				status = packetRecordingStatus;
				break;

		case PROTOCOL__RECORD_TDM_PACKET:
				status = tdmRecordingStatus;
				break;

		case PROTOCOL__RECORD_NETWORK_PACKET:
				status = networkRecordingStatus;
				break;

		default:
				break;
	}

	return (status);

}

static short rand( void )
{
	short data;

	get_random_bytes( &data, 2 );   /* (-32767) - (32767) */
	if(data < 0) /* data must be positive */
		data = -data;		/* 0 -32767 */

	return data;
}


module_init(dspDev_init);
module_exit(dspDev_exit);

