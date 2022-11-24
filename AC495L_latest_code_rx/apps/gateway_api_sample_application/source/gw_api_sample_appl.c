
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "general_defs.h"
#include "acGatewayAPI.h"
#ifdef GW_APPL_SDP
#include "acIPPhoneAPI.h"
#endif
#include "gw_api_localNetRx.h"

#define GW_API_SAMPLE_APPL_DEBUG_PRINT

#ifdef GW_API_SAMPLE_APPL_DEBUG_PRINT
#define DPRINTK(a1,a2...)	printf("\n" a1, ##a2)
#else
#define DPRINTK(a1,a2...)
#endif



/*int LoadFileToBuf(char* buf,size_t bufsize,char* fileName);*/
int start_paging_Cmd(int argc, char* argv[]);
int acgGetDefaultInitDeviceParameters_cmd(int argc, char* argv[]);
int acgInitializeDSP_cmd(int argc, char* argv[]);
int acgModifyCPT_cmd(int argc, char* argv[]);
int acgReInitializeDSP_cmd(int argc, char* argv[]);
int acgCloseDSP_cmd(int argc, char* argv[]);
int acgGetDefaultChannelParameters_cmd(int argc, char* argv[]);
int acgOpenChannel_cmd(int argc, char* argv[]);
int acgUpdateChannel_cmd(int argc, char* argv[]);
int acgCloseChannel_cmd(int argc, char* argv[]);
int acgActivateRTP_cmd(int argc, char* argv[]);
int acgDeactivateRTP_cmd(int argc, char* argv[]);
int acgPlayTone_cmd(int argc, char* argv[]);
int acgPlayToneSequence_cmd(int argc, char* argv[]);
int acgPlayToneSpecial_cmd(int argc, char* argv[]);
int acgPlayAmTone_cmd(int argc, char* argv[]);
int acgStopTone_cmd(int argc, char* argv[]);
int acgPlayMWI_cmd(int argc, char* argv[]);
int acgStopMWI_cmd(int argc, char* argv[]);
int acgPlayCallerID_cmd(int argc, char* argv[]);
int acgStopCallerID_cmd(int argc, char* argv[]);
int acgStart3WayConference_cmd(int argc, char* argv[]);
int acgStop3WayConference_cmd(int argc, char* argv[]);
int acgReinitializeEchoCanceller_cmd(int argc, char* argv[]);
int acgDebugInformationActivation_cmd(int argc, char* argv[]);
int acgInitializeFXS_cmd(int argc, char* argv[]);
int acgCloseFXS_cmd(int argc, char* argv[]);
int acgStartRing_cmd(int argc, char* argv[]);
int acgStopRing_cmd(int argc, char* argv[]);
int acgSeizeLine_cmd(int argc, char* argv[]);
int acgReleaseLine_cmd(int argc, char* argv[]);
int acgGenerateFlash_cmd(int argc, char* argv[]);
int SetChannelParam_cmd(int argc, char* argv[]);
sub_cmd_t GetChannelCmdType(int argc, char* argv[]);
int CreateConnection_cmd(int argc, char* argv[]);
#ifdef GW_APPL_SDP
int SpeakerStatus_cmd(int argc, char* argv[]);
#endif
int RunScript_cmd(int argc, char* argv[]);
int acgStartFax_cmd(int argc, char* argv[]);
int acgStopFax_cmd(int argc, char* argv[]);
int acgSetRegularRTCPParameters_cmd(int argc, char* argv[]);
int acgGetRegularRTCPParameters_cmd(int argc, char* argv[]);
int acgSetNtpTimeStamp_cmd(int argc, char* argv[]);
int acgGetVersionInfo_cmd(int argc, char* argv[]);
int acgPlayback_cmd(int argc, char* argv[]);
int acgPlaybackEnd_cmd(int argc, char* argv[]);
int RecordingConfig_cmd(int argc, char* argv[]);
int exit_cmd(int argc, char* argv[]);
int help_cmd(int argc, char* argv[]);

int ParseCmdLine(char *cmd_lin, int *ret_argc, char *ret_argv[]);
int ExecuteCommand(int argc, char *argv[]);
int RunCLI(void);

/* External functions */
void initMediaEvents(void);
void closeMediaEvents(void);

#if POLLING_METHOD__BLOCKING
int launchFxsThread(void);
void launchDSPThreads(void);
#else
int launchGlobalThread(void);
#endif

int get_hook_state(int line);

int aclVoipRtpUtils_createSockets(void);
int gw_api_createRtpRxThread(void);
int gw_api_createRtcpRxThread(void);

void aclVoipRtpUtils_resetNetworkingDBs(void);
void aclVoipRtpUtils_freeNetworkingDbs(void);
void aclVoipRtpUtils_ChannelAddrSet(char* addr, unsigned short port, int channel);
//void aclVoipRtpUtils_ChannelConnect(int channel);

void setConnectionMode(int channel, conection_mode_t mode);
void setLocalConnectionChannelMapping(int firstChannel, int secondChannel);


CmdList Commands[]={
	{"acgGetDefaultInitDeviceParameters", "Gets the default values to the DSP initialization structure.", "Usage: acgGetDefaultInitDeviceParameters", acgGetDefaultInitDeviceParameters_cmd},
	{"acgInitializeDSP", "Initializes the DSP.", "Usage: acgInitializeDSP", acgInitializeDSP_cmd},
	{"acgModifyCPT", "Modify call progress tone parameters(for taking effect, acgInitializeDSP or acgReInitializeDSP should be called after this command).", "Usage: acgModifyCPT <Index> <Type(0-CONTINUOUS,1-CADENCE,2-BURST,3-SPECIAL)> <ToneA[Hz]> <ToneB[Hz]> <ToneA[dbm]> <ToneB[dbm]> <FirstOn[10ms]> <FirstOff[10ms]> <SecondOn[10ms]> <SecondOff[10ms]> <ThirdOn[10ms]> <ThirdOff[10ms]> <FourthOn[10ms]> <FourthOff[10ms]>", acgModifyCPT_cmd},
	{"acgReInitializeDSP", "ReInitializes the DSP(for taking effect, acgInitializeDSP or acgReInitializeDSP should be called after this command).", "Usage: acgReInitializeDSP", acgReInitializeDSP_cmd},
	{"acgCloseDSP", "Closes the DSP.", "Usage: acgCloseDSP", acgCloseDSP_cmd},
	{"acgGetDefaultChannelParameters", "Gets the default channel configuration parameters.", "Usage: acgGetDefaultChannelParameters", acgGetDefaultChannelParameters_cmd},
	{"acgOpenChannel", "Opens a DSP channel with the provided configuration parameters.", "Usage: acgOpenChannel <DSP channel (0 to number of DSP channels minus 1)>", acgOpenChannel_cmd},
	{"acgUpdateChannel", "Updates a DSP channel with the provided configuration parameters.", "Usage: acgUpdateChannel <DSP channel (0 to number of DSP channels minus 1)>", acgUpdateChannel_cmd},
	{"acgCloseChannel", "Closes a DSP channel and sets it to idle state.", "Usage: acgCloseChannel <DSP channel (0 to number of DSP channels minus 1)>", acgCloseChannel_cmd},
	{"acgActivateRTP", "Activates the RTP stream of a channel that was previously opened by a call to acgOpenChannel.", "Usage: acgActivateRTP <DSP channel (0 to number of DSP channels minus 1)>", acgActivateRTP_cmd},
	{"acgDeactivateRTP", "Deactivates the RTP stream of a channel that was previously activated by a call to acgActivateRTP.", "Usage: acgDeactivateRTP <DSP channel (0 to number of DSP channels minus 1)>", acgDeactivateRTP_cmd},
	{"acgPlayTone", "Plays a single DTMF or Call Progress tone towards the local TDM (telephone) side or remote IP network side.", "Usage: acgPlayTone <DSP channel (0 to number of DSP channels minus 1)>  <Direction: (0-local, 1-network, 2-encoder, 3-local&network, 4-local&encoder)>  <OnDuration: [msec]>  <OffDuration: [msec]>  <ToneType: (0-DTMF, 1-Call progress tone)>  <Tone index or DTMF digit>", acgPlayTone_cmd},
	{"acgPlayToneSequence", "Plays a sequence of DTMF or Call Progress tones towards the local TDM (telephone) side or remote IP network side.", "Usage: acgPlayToneSequence <DSP channel (0 to number of DSP channels minus 1)>  <Direction: (0-local, 1-network, 2-encoder, 3-local&network, 4-local&encoder)>  <OnDuration: [msec]>  <OffDuration: [msec]>  <NumOfTones: (0-15)> <ToneType: (0-DTMF, 1-Call progress tone)>  <Tone index or DTMF digit>", acgPlayToneSequence_cmd},
	{"acgPlayToneSpecial", "Plays a tone with three combined frequencies towards the local TDM (telephone) side or remote IP network side.", "Usage: acgPlayToneSpecial <DSP channel (0 to number of DSP channels minus 1)>  <Direction: (0-local, 1-network, 2-encoder, 3-local&network, 4-local&encoder)>  <NumberOfTones(1-6)> <FrequencyA[Hz]> <FrequencyB[Hz]> <FrequencyC[Hz]> <FrequencyD[Hz]> <SignalLevel_n [-dbm]> <Digit_n [0-8]> <SignalDuration_n [ms]> ...", acgPlayToneSpecial_cmd},
	{"acgPlayAmTone", "Plays AM tone with two combined frequencies towards the local TDM (telephone) side or remote IP network side.", "Usage: acgPlayAmTone <DSP channel (0 to number of DSP channels minus 1)>  <Direction: (0-local, 1-network, 2-encoder, 3-local&network, 4-local&encoder)> <Duration: [msec]> <FrequencyA[Hz]> <FrequencyB[Hz]> <AmFactor> ", acgPlayAmTone_cmd},
	{"acgStopTone", "Stops a single tone or a sequence of tones.", "Usage: acgStopTone <DSP channel (0 to number of DSP channels minus 1)>", acgStopTone_cmd},
	{"acgPlayMWI", "Plays a message waiting indication.", "Usage: acgPlayMWI <DSP channel (0 to number of DSP channels minus 1)>", acgPlayMWI_cmd},
	{"acgStopMWI", "Stops the message waiting indication.", "Usage: acgStopMWI <DSP channel (0 to number of DSP channels minus 1)>", acgStopMWI_cmd},
	{"acgPlayg_cCallerID", "Generates a g_cCaller ID signal towards the local TDM.", "Usage: acgPlayg_cCallerID <DSP channel (0 to number of DSP channels minus 1)> <Number: String without spaces> <Name: String without spaces> <Name Status: ((0-Present, 1-Private, 2-Unavailable)> <Number Status: (0-Present, 1-Private, 2-Unavailable)> <Date&Time[MMDDhhmm]> <Call Type: (1, 2 ,3, 4, 5, 6, 7, 10, 11, 50, 81)>", acgPlayCallerID_cmd},
	{"acgStopg_cCallerID", "Stops g_cCaller ID generation.", "Usage: acgStopg_cCallerID <DSP channel (0 to number of DSP channels minus 1)", acgStopCallerID_cmd},
	{"acgStart3WayConference", "Starts a 3-way conference.", "Usage: acgStart3WayConference <DSP channel (0 to number of DSP channels minus 1)>", acgStart3WayConference_cmd},
	{"acgStop3WayConference", "Stops a 3-way conference.", "Usage: acgStop3WayConference <DSP channel (0 to number of DSP channels minus 1)>", acgStop3WayConference_cmd},
	{"acgReinitializeEchoCanceller", "Reinitializes the echo canceller.", "Usage: acgReinitializeEchoCanceller <DSP channel (0 to number of DSP channels minus 1)>", acgReinitializeEchoCanceller_cmd},
	{"acgDebugInformationActivation", "Debug information activation.", "Usage: acgDebugInformationActivation <DSP channel (0 to number of DSP channels minus 1)> <Packet Recording: (0-disbale, 1-enable)> <Network Recording: (0-disbale, 1-enable)> <TDM Recording: (0-disbale, 1-enable)>", acgDebugInformationActivation_cmd},
	{"acgInitializeFXS", "Initializes the SLIC device.", "Usage: acgInitializeFXS", acgInitializeFXS_cmd},
	{"acgCloseFXS", "Closes the SLIC device.", "Usage: acgCloseFXS", acgCloseFXS_cmd},
	{"acgStartRing", "Starts ringing on an FXS port.", "Usage: acgStartRing <LineId (0 to number of lines minus 1)>", acgStartRing_cmd},
	{"acgStopRing", "Stops ringing on an FXS port.", "Usage: acgStopRing <LineId (0 to number of lines minus 1)>", acgStopRing_cmd},
	{"acgGenerateFlash", "Generate flash on an FOS port.", "Usage: acgGenerateFlash <LineId (0 to number of lines minus 1)>", acgGenerateFlash_cmd},
	{"acgSeizeLine", "Seize line on an FOS port.", "Usage: acgSeizeLine <LineId (0 to number of lines minus 1)>", acgSeizeLine_cmd},
	{"acgReleaseLine", "Release line on an FOS port.", "Usage: acgReleaseLine <LineId (0 to number of lines minus 1)>", acgReleaseLine_cmd},
#ifdef GW_APPL_SDP
	{"SpeakerStatus", "toggel between the handset and the speaker on channel 0 .", "Usage: spkStatus <(0-handset, 1-speaker)>", SpeakerStatus_cmd},
#endif
	{"SetChannelParam", "Sets the DSP channel parameters information.", "Usage: SetChannelParam <subgroup parameter> <parameter> <value>\n\t--you can get the value's meaning by inputting the command without value\n\t<subgroup parameter>\n\t\t<parameter>\n\n\t[g_cCallerID]\n\t\tCIDOnHook | CIDOffHook | CIDStandard\n\t[FaxModem]\n\t\tFaxMethod | FaxErrorCorrectionMode | EnableCNG | FaxMaxRate | CEDTransferMode | V34TransferMethod | V32TransferMethod | V23TransferMethod | V22TransferMethod | V21TransferMethod | Bell103TransferMethod | V34FaxTransferMethod | BypassCoder | BypassJitter | DataBypassOutputGain | FaxBypassOutputGain\n\t[RTPStream]\n\t\tCoder | PTime | SilenceCompressionMode | FaxBypassPT | ModemBypassPT | IBSRedundancy | RTPRedundancy | RFC2833TxPT | RFC2833RxPT | RFC2198PT | RTCPMeanTxInterval | CName | TxRtcpFilter | RxRtcpFilter\n\t[Signaling]\n\t\tDTMFTransferMode | CPTTransferMode | DetectCallProgress | IBSRedirection | DetectDTMFRegister | IBSLevel | DetectLongDTMF\n\t[JitterBuffer]\n\t\tVoiceJBMin | VoiceJBMax\n\t[EchoCanceller]\n\t\tECNLP | ECMode | EchoCancellerLength\n\t[Miscellaneous]\n\t\tTestMode\n\t[Gain]\n\t\tInputGain | VoiceOutputGain\n\t[help]-Display all help", SetChannelParam_cmd},
	{"CreateConnection", "Creates an RTP connection.", "Usage: CreateConnection <DSP channel (0 to number of DSP channels minus 1)> <DSP channel (0 to number of DSP channels minus 1)>\nCreateConnection <DSP channel (0 to number of DSP channels minus 1)> <Remote IP address:remote port>", CreateConnection_cmd},
	{"RunScript", "Runs a script.",    "Usage: RunScript <script file name>", RunScript_cmd},
	{"acgStartFax", "Start T38.", "Usage: acgStartFax <Channel 0>", acgStartFax_cmd},
	{"acgStopFax", "Stop T38.", "Usage: acgStopFax <Channel 0>", acgStopFax_cmd},
	{"acgSetRTCPParameters", "Set RTCP Parameters.", "Usage: acgSetRTCPParameters <DSP channel (0 to number of DSP channels minus 1)> <Index(0-transmit packet count,1-transmit octets count,2-receive packet count,3-receive octets count,4-jitter,5-packet loss,6-round trip delay)> <value> ", acgSetRegularRTCPParameters_cmd},
	{"acgGetRTCPParameters", "Get RTCP Parameters.", "Usage: acgGetRTCPParameters <DSP channel (0 to number of DSP channels minus 1)>", acgGetRegularRTCPParameters_cmd},
	{"acgSetNtpTimeStamp", "Set ntp time stamp.", "Usage: acgSetNtpTimeStamp <DSP channel (0 to number of DSP channels minus 1)> <NTP time in seconds: [sec]> <NTP time in fractions of seconds: [sec]> ", acgSetNtpTimeStamp_cmd},
	{"acgGetVersionInfo", "Get version information.", "Usage: acgGetVersionInfo", acgGetVersionInfo_cmd},
	{"acgPlayback", "Play back ring.", "Usage: acgPlayback <DSP channel (0 to number of DSP channels minus 1)> <pbType (0-TDM, 1-NETWORK)> <Coder (G711-A=3, G711-mu=4, G726-16=5, G726-24=6, G726-32=7, G726-40=8, G729=27, G723-5.3=25, G723-6.3=26)> <FileName (string)>", acgPlayback_cmd},
	{"acgPlaybackEnd", "Play back ring stop.", "Usage: acgPlaybackEnd <DSP channel (0 to number of DSP channels minus 1)>", acgPlaybackEnd_cmd},
	{"RecordingConfig", "Config recording IP address and port number.", "Usage: RecordingConfig <IP address> <Port Number>", RecordingConfig_cmd},
	{"exit", "Exits this sample application.",  "Usage: exit", exit_cmd},
	{"help", "Getting help on available commands.", "Usage: help [<Command>]", help_cmd},
	{"start_paging","multicast paging ","Usage:start_paging_Cmd",start_paging_Cmd}
};
GroupCmdList ChannelCommands[]={
    /*Signaling cmds*/
    {E_DTMFTransferMode,        "Signaling", "DTMFTransferMode", "<Value according to the acgESignalingTransportMethod enum. (0-mute, 1-relay, 2-transparent, 3-relay enable voice mute, 4-Mixed)>"},
	{E_CPTTransferMode,        "Signaling", "CPTTransferMode", "<Value (2-Transparent, 4-Mixed)>"},


    {E_DetectCallProgress,      "Signaling", "DetectCallProgress", "<Value (0-disbale, 1-enable)>"},
    {E_IBSRedirection,          "Signaling", "IBSRedirection", "<Value according to the acgEIbsDetectionDirection enum>"},
    {E_DetectDTMFRegister,      "Signaling", "DetectDTMFRegister", "<Value (0-disbale, 1-enable)>"},
    {E_IBSLevel,                "Signaling", "IBSLevel", "<Value (0 - 31 dBm). Default = -9 dBm>"},
    {E_DetectLongDTMF,          "Signaling", "DetectLongDTMF", "<Value (0-disbale, 1-enable)>"},
    /*Gain cmds*/
    {E_InputGain,               "Gain", "InputGain", "<Value according to the acgEVoiceGain enum. (0-mute, in turn: -31dB to +31dB. default 32=0dB)>"},
    {E_VoiceOutputGain,         "Gain", "VoiceOutputGain","<Value according to the acgEVoiceGain enum. (0-mute, in turn: -31dB to +31dB. default 32=0dB)>"},
   	/*g_cCallerID cmds*/
    {E_CIDOnHook,               "g_cCallerID", "CIDOnHook", "<Value (0-disbale, 1-enable)>"},
    {E_CIDOffHook,              "g_cCallerID", "CIDOffHook", "<Value (0-disbale, 1-enable)>"},
    {E_CIDStandard,             "CallerID", "CIDStandard", "<Value according to the acgEg_cCallerIdStandard enum"},
    /*JitterBuffer cmds*/
    {E_VoiceJBMin,              "JitterBuffer", "VoiceJBMin", "<Value (Range: 0 - 280 msec. Default = 35 msec)>"},
    {E_VoiceJBMax,              "JitterBuffer", "VoiceJBMax", "<Value (Range: 0 - 300 msec. Default = 150 msec)>"},
    {E_JBOptimizationFactor,    "JitterBuffer", "JBOptimizationFactor", "<Value (Range: 0 - 12. 13 = static delay for bypass mode)>"},
    /*EchoCanceller cmds*/
    {E_ECNLP,                   "EchoCanceller", "ECNLP", "<Value (0-disbale, 1-enable)>"},
    {E_ECMode,                  "EchoCanceller", "ECMode", "<Value (0-disbale, 1-enable)>"},
    {E_EchoCancellerLength,     "EchoCanceller", "EchoCancellerLength", "<Value (0-8ms, 1-16ms, 2-24ms, 3-32ms)>"},
    /*FaxModem cmds*/
    {E_FaxMethod,               "FaxModem", "FaxMethod", "<Value (0-disbale, 1-relay, 2-bypass)>"},
    {E_FaxErrorCorrectionMode,  "FaxModem", "FaxErrorCorrectionMode", "<Value (0-disbale, 1-enable)>"},
    {E_EnableCNG,               "FaxModem", "EnableCNG", "<Value (0-disbale, 1-enable)>"},
    {E_FaxMaxRate,              "FaxModem", "FaxMaxRate", "<Value (0-2.4, 1-4.8, 2-7.2, 3-9.6, 4-12, 5-14.4) kbps>"},
    {E_CEDTransferMode,         "FaxModem", "CEDTransferMode", "<Value (0-ByFaxRelay, 1-InVoiceOrPMCBypassModeTriggerFaxByV21Preamble)"},
    {E_V34TransferMethod,       "FaxModem", "V34TransferMethod", "<Value (0-disable, 2-bypass)"},
    {E_V32TransferMethod,       "FaxModem", "V32TransferMethod", "<Value (0-disable, 2-bypass)"},
    {E_V23TransferMethod,       "FaxModem", "V23TransferMethod", "<Value (0-disable, 2-bypass)"},
    {E_V22TransferMethod,       "FaxModem", "V22TransferMethod", "<Value (0-disable, 2-bypass)"},
    {E_V21TransferMethod,       "FaxModem", "V21TransferMethod", "<Value (0-disable, 2-bypass)"},
    {E_Bell103TransferMethod,   "FaxModem", "Bell103TransferMethod", "<Value (0-disable, 2-bypass)"},
    {E_V34FaxTransferMethod,    "FaxModem", "V34FaxTransferMethod", "<Value (0-disable, 1-relay, 2-bypass, 3-VBD)"},
    {E_BypassCoder,             "FaxModem", "BypassCoder", "<Value according to the 'acgECoder' coder enum"},
    {E_BypassJitter,            "FaxModem", "BypassJitter", "<Value (Range 0 -160 msec. Default = 150 msec)>"},
    {E_DataBypassOutputGain,    "FaxModem", "DataBypassOutputGain", "<Value according to the 'Tac49xFaxAndDataGain' coder enum"},
    {E_FaxBypassOutputGain,     "FaxModem", "FaxBypassOutputGain", "<Value according to the 'Tac49xFaxAndDataGain' coder enum"},
    /*RTPStream cmds*/
    {E_Coder,                   "RTPStream", "Coder", "<Value (G711-A=3, G711-mu=4, G726-16=5, G726-24=6, G726-32=7, G726-40=8, G729=27, G723-5.3=25, G723-6.3=26, G722-64=56)>"},
    {E_PTime,                   "RTPStream", "PTime", "<Proper value according to the coder>"},
    {E_SilenceCompressionMode,  "RTPStream", "SilenceCompressionMode", "<Value (0-disbale, 1-enable)>"},
    {E_FaxBypassPT,             "RTPStream", "FaxBypassPT", "<Proper value according to the FaxBypassPayloadType, default is 102>"},
    {E_ModemBypassPT,           "RTPStream", "ModemBypassPT", "<Proper value according to the ModemByassPayloadType, default is 103>"},
    {E_IBSRedundancy,           "RTPStream", "IBSRedundancy", "<Value according to the 'acgEIbsRedundancyLevel' coder enum>"},
    {E_RTPRedundancy,           "RTPStream", "RTPRedundancy", "<Value (0-disbale, 1-enable)>"},
    {E_RFC2833TxPT,             "RTPStream", "RFC2833TxPT", "Proper value according to the coder"},
    {E_RFC2833RxPT,             "RTPStream", "RFC2833RxPT", "Proper value according to the coder"},
    {E_RFC2198PT,               "RTPStream", "RFC2198PT", "Proper value according to the coder"},
    {E_RTCPMeanTxInterval,      "RTPStream", "RTCPMeanTxInterval", "Proper value according to the coder"},
    {E_CName,                   "RTPStream", "CName", "String:RTP canonical name"},
    {E_TxRtcpFilter,            "RTPStream", "TxRtcpFilter", "<Value (0-disbale, 1-enable)>"},
    {E_RxRtcpFilter,            "RTPStream", "RxRtcpFilter", "<Value (0-disbale, 1-enable)>"},
    /*Miscellaneous*/
    {E_TestMode,                "Miscellaneous", "TestMode", "<Value according to the acgETestMode enum>"},
    {E_ERROR,                   "","",""}
};


static acgTInitializationParameters 	InitializationParams;
static acgTChannelParameters	ChannelParameters;

static channel_t				ChannelInformation[ACG_NUMBER_OF_DSP_CHANNEL];
static ApplStatus				applicationStatusParams;

static char 					scriptBuf[CONFIGURATION_BUFFER_MAX_LEN] = "";


appl_code2code_t coder2payload[] =
{
	{ACG_CODER__G711MULAW, 	0},
	{ACG_CODER__G726_32, 		2},
	{ACG_CODER__G723HIGH,  	4},
	{ACG_CODER__G711ALAW, 	8},
	{ACG_CODER__G722_48K, 		9},
	{ACG_CODER__G729, 		18},
/* add before this record*/
	{-1, -1},
};
static char scDirection2Name[ACG_DIRECTION__MAXIMUM][16]={"local", "network", "encoder", "local&network", "local&encoder"};
/************************************/
/**********  Internal Functions **********/
/************************************/

int appl_code2code(appl_code2code_t *list, int code)
{
	for (; list->code != -1 && list->code != code; list++);

	return list->val;
}


static char* voip_conf_util_get_next_line(char** p)
{
	char *result = NULL;

	result = strtok( *p, "\n" );

	if( result == NULL )
	{
		return NULL;
	}
	else
	{
		*p +=(strlen(result) + strlen("\n"));

		if((result[strlen(result)-1] == '\n') || (result[strlen(result)-1] == '\r'))
			result[strlen(result)-1] = 0;

		if((result[strlen(result)] == '\n') || (result[strlen(result)] == '\r'))
			result[strlen(result)] = 0;

	}
	return result;

}

int voip_conf_util_load_cfg_file(char* pScriptBuf)
{
	int 		argc;
	char 		*argv[MAX_ARGS+1];

	char   	*ptr = NULL;
	char   	*retString = NULL;
	char		*pline = NULL;

	int 		ret = 0;

	if(pScriptBuf !=NULL)
	{
	    ptr = pScriptBuf;
	}
	else
	{
	    printf("illegal configuration file - NULL\r\n");
	    return (-1);
	}

	pline = AC_ZALLOC_L(MAX_CMD_LINE_LEN);

	while((retString = voip_conf_util_get_next_line(&ptr))!=NULL)
	{
		strcpy(pline, retString);

		/* Skip lines starting with ';' and '#' */
		if(pline[0]==';' || pline[0]=='#')
			continue;
#if 0
		if( pline[ strlen(pline) - 1 ] == '\n' )
			pline[ strlen(pline) - 1 ] = 0; /* take the newline character off */
#endif
		if((pline[0] == '\n')||(pline[0] == '\r')||
							(pline[0] == '\t')||(pline[0] == '\0'))
		{
			continue;
		}

		printf("\nVoIP >> %s", pline);

		ParseCmdLine(pline, &argc, argv);

		ret = ExecuteCommand(argc, argv);
		if(ret<0)
		{
			printf("Command '%s' returned with error code %d.\n",argv[0],ret);
		}

		usleep(700000);

	}

	if(pline)
		nfree(pline);

	return 0;
}


/***************************************************************************************
 * cfg_util_load_file
 * -------------------------------------------------------------------------------------
 * General:   Opens a file and copies the content of the file to a buffer.
 * Return Value: True is succeeded, False if failed
 * -------------------------------------------------------------------------------------
 * Arguments:
 * Input: 	 	string - the buffer
 *		 	integer - buffer length
 *
 * Output:   	string - file name
 **************************************************************************************/
int cfg_util_load_file(char* buf, size_t bufsize, char* fileName)
{
	FILE* file;

	char *pLine;
	unsigned int cnt=0;

	if ((file = fopen(fileName, "r")) == NULL) {
		printf("ERROR: '%s' File Not Found\n",fileName);
		return (-1);
	}

	strcpy(buf,"");

	pLine = AC_ZALLOC_L(CONFIGURATION_BUFFER_MAX_LEN);
	if(!pLine)
	{
		fclose(file);
		return(-1);
	}
	memset(pLine , 0, sizeof(CONFIGURATION_BUFFER_MAX_LEN));

	while (fgets(pLine, CONFIGURATION_BUFFER_MAX_LEN-1, file) != NULL)
	{
		/* Skip lines starting with ';' */
		if(pLine[0]==';')
			continue;
		if(cnt+strlen(pLine)<bufsize)
		{
			strcat(buf, pLine);
	        	cnt+=strlen(pLine);
		}
		else
		{
			printf("\n*** Buffer size is not large enough %d - It must be increased ***\n", CONFIGURATION_BUFFER_MAX_LEN);

			if(pLine)
				free(pLine);

			fclose(file);
			return (-1);
		}
	}

	if(pLine)
		free(pLine);

	fclose(file);

	return 0;

}


void *AC_ZALLOC_L(size_t size)
{
	void *memory = NULL;
	/* uClibc crashes on malloc(0) */
	if (!size)
		return memory;

	memory = malloc(size);

	if (memory == NULL)
	{
		printf("zalloc failed\n");
		return memory;
	}

	memset(memory, 0, size);

	return memory;
}


#if defined(B_CEVA_PLATFORM)

#define CHUNK_SIZE 256

#define CONDITIONALLY_WRITE_CHUNK() 																\
	{                                                                                           \
		if (data_chunk_cnt == CHUNK_SIZE) 														\
		{                                    													\
			pInitializationParams->PrimaryDSPDeviceInformation.NumberOfDevices = 1;				\
			pInitializationParams->PrimaryDSPDeviceInformation.pProgram = data_chunk;			\
			pInitializationParams->PrimaryDSPDeviceInformation.ProgramSize = CHUNK_SIZE;		\
			if(acgInitializeDSP(pInitializationParams) != ACG_SUCCESS )							\
			{                                                                                   \
				printf("error - acgInitializeDSP()\r\n");										\
				return;                                   										\
			}                                              										\
			data_chunk_cnt = 0;                             									\
			memset(data_chunk, 0x0, CHUNK_SIZE);               									\
		}																						\
	}

void initDsp(acgTInitializationParameters 	*pInitializationParams)
{
	FILE *pBinaryFd;
	char file_name[100] = {0};
	char data_chunk[CHUNK_SIZE] = {0};
	int data_chunk_cnt = 0;
        int curr_char, prev_char;
        unsigned char count;

	strcpy(file_name,"/gw_api/binary/ceva.fwb");

	if(!(pBinaryFd= fopen(file_name,"r")))
	{
		printf("error open file %s", file_name);
		exit(-1);
	}

	prev_char = EOF;
	while ((curr_char = fgetc(pBinaryFd)) != EOF)
	{
		data_chunk[data_chunk_cnt++] = curr_char;
		CONDITIONALLY_WRITE_CHUNK();
        if (curr_char == prev_char)
		{
			count = fgetc(pBinaryFd);
			while (count > 0)
			{
				data_chunk[data_chunk_cnt++] = curr_char;
				count--;
				CONDITIONALLY_WRITE_CHUNK();
			}
			prev_char = EOF;
		}
        else
            prev_char = curr_char;

		CONDITIONALLY_WRITE_CHUNK();
  	} /*end while*/


    	if(fclose(pBinaryFd) == -1)
	{
		printf("error closing file %s", file_name);
		return;
	}

	/*end of firmware downloading*/
	pInitializationParams->PrimaryDSPDeviceInformation.NumberOfDevices = 1;
	pInitializationParams->PrimaryDSPDeviceInformation.pProgram = NULL;
	pInitializationParams->PrimaryDSPDeviceInformation.ProgramSize = 0;

	if(acgInitializeDSP(pInitializationParams) != ACG_SUCCESS )
	{
		printf("error - acgInitializeDSP()\r\n");
		return;
	}

	DPRINTK("Initializing the DSP...Done\n");

}
#endif /* B_CEVA_PLATFORM */

#if defined(S_PLATFORM)
static int ReadFirmware(char *pfirmwareName, char **ppfirmwareBuff, int *pfirmwareSize)
{
       FILE	                        	*firmwareFd;
	struct stat	                 fileStat;
    if((firmwareFd = fopen(pfirmwareName,"rb")) == NULL)
	{
		perror("can't open firmware file\r\n");
		return -1;
	}

	if(stat(pfirmwareName, &fileStat) == -1)
	{
	      fclose(firmwareFd);
		perror("can't get file status\r\n");
		return -1;
	}

	if((*ppfirmwareBuff = (char *)malloc(fileStat.st_size)) == NULL)
	{
	       fclose(firmwareFd);
		perror("can't allocate file size \r\n");
		return -1;
	}

	if(fread(*ppfirmwareBuff ,1, fileStat.st_size, firmwareFd) != (unsigned int)fileStat.st_size)
	{
	        fclose(firmwareFd);
               free(*ppfirmwareBuff);
		perror("error while read file\r\n");
		return -1;
	}

        *pfirmwareSize =  fileStat.st_size;
	fclose(firmwareFd);
       return 0;
}
#endif /*S_PLATFORM */


int isValidDSPChannel(int dspChannel)
{
	if( (dspChannel>=0) && (dspChannel<ACG_NUMBER_OF_DSP_CHANNEL) )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int isValidFXSLine(int line)
{
	if( (line>=0) && (line<NUMBER_OF_FXS_LINES) )
		return 1;
	else
		return 0;
}

int isConnectionwithRemoteIP(char *str, char *pRemoteAddress, int *pRemotePort)
{
	char temp[MAX_CMD_LINE_LEN] = "";
	int delimiter = ':';
	char *pToCharacter;

	strcpy(temp, str);

	pToCharacter = strchr(temp, delimiter) ;

	if(pToCharacter != NULL )
	{
		*pToCharacter = 0;	/* Replace the delimiter with NULL to device into two strings */

		strcpy(pRemoteAddress, temp);
		*pRemotePort = atoi(pToCharacter+1);

		return 1;
	}
	else
	{
		return 0;
	}
}



int RunCLI(void)
{
	char cmd_lin[MAX_CMD_LINE_LEN];
	int argc;
	char *argv[MAX_ARGS+1];
	int ret=0;

	memset(cmd_lin, 0, sizeof(cmd_lin));

	while(ret!=EXIT_CODE)
	{
		printf("VoIP >> ");

		fgets( cmd_lin, MAX_CMD_LINE_LEN, stdin );

		if( cmd_lin[ strlen(cmd_lin) - 1 ] == '\n' )
			cmd_lin[ strlen(cmd_lin) - 1 ] = 0; /* take the newline character off the filename */

		if((cmd_lin[0] == '\n')||(cmd_lin[0] == '\r')||
							(cmd_lin[0] == '\t')||(cmd_lin[0] == '\0'))
		{
			continue;
		}

		/* memset argv first */
		memset(argv, 0, sizeof(argv));
		ParseCmdLine(cmd_lin, &argc, argv);
		ret = ExecuteCommand(argc, argv);
		if(ret<0)
		{
			printf("Command '%s' returned with error code %d.\n",argv[0],ret);
		}
	}

	return 0;

}

static int PreProcessCmdLine(char *cmd_lin)
{
    char seps[]="'";
    char *pStart=NULL, *pEnd=NULL;
	int iX=0;
    //printf("\r\n cmd_lin = [%s]\r\n", cmd_lin);

	while(!pStart && !pEnd)
	{
		pStart=strstr(cmd_lin,seps);
		if(!pStart)
			break;
		pEnd=strstr(pStart+1,seps);
		if(!pEnd)
			break;

		for(iX=(pStart-cmd_lin); iX<(pEnd-cmd_lin); iX++)
		{
			if( 0x20==cmd_lin[iX] )
				cmd_lin[iX] = 0x10;    /* Convet SPACE to 0x10 */
		}
		*pEnd = *pStart = 0x20;	/* replace the ' as SPACE */
		pEnd = pStart = NULL;
	}
	//printf("\r\n cmd_lin = [%s]\r\n", cmd_lin);
	return 0;
}

static void PostProcessCmdLine(int argc, char *ret_argv[])
{
	char **argv=ret_argv;
    int iX=0, iY=0;

    for(iX=0; iX < argc; iX++)
    {   /* Process all argv, if it includes */
        //printf("\t argv[%d]=%s\n", iX, argv[iX]);
        for(iY=0; iY < (int)strlen(argv[iX]); iY++)
        {
            if( 0x10==argv[iX][iY] )
                argv[iX][iY] = 0x20;    /* Convet it to SPACE */
        }
        //printf("\t argv[%d]=%s\n", iX, argv[iX]);
    }
}
int ParseCmdLine(char *cmd_lin, int *ret_argc, char *ret_argv[])
{
	char seps[]=" \t";
	char **argv=ret_argv;
	char *tok;
	int argc=0;

    PreProcessCmdLine(cmd_lin);

	/* Parse the command line */

	*ret_argc=0;
	tok=strtok(cmd_lin,seps);
	if(!tok)
		return -1;

	while(tok)
	{
		argv[argc++]=tok;
		tok=strtok(NULL,seps);
	}

    PostProcessCmdLine(argc, ret_argv);

	*ret_argc=argc;
	return 0;
}

int ExecuteCommand(int argc, char *argv[])
{
	int i;

	for(i=0; strlen(Commands[i].name)>0 ; i++)
	{
		if(!strcmp(Commands[i].name,argv[0]))
			return Commands[i].func(argc,argv);
	}

	printf("Unknown command - %s\n",argv[0]);
	return -1;

}


/*******************************************************
********************************************************

				Gateway API Based Commands

********************************************************
********************************************************/
int acgGetDefaultInitDeviceParameters_cmd(int argc, char* argv[])
{
	int i=0;

	switch(argc)
	{
		case 1:
				break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

	memset(&InitializationParams, 0, sizeof(InitializationParams));

	/* Gets the defaults values to the DSP initialization structure */
	if(acgGetDefaultInitDeviceParameters(&InitializationParams)!= ACG_SUCCESS )
	{
		printf("error - acgGetDefaultInitDeviceParameters()\r\n");
		return (-1);
	}

	DPRINTK("Setting default device parameters\n");

	/* Setting a fake remote IP Address and port for packet recording mechanism */
	strcpy(InitializationParams.RemoteRecordingIpAddress, " ");
	InitializationParams.RemoteRecordingPort = 50002;

#if defined(AC49_SMB_VOIP_TOOLKIT) || defined(AC494_TULIP2_40) || defined(AC49_ORCHID_ATA_VOIP_TOOLKIT)
	/*
        DSP:        0   0   0   0   1   1   1   1   2   2   2   2   3   3   3   3
        Channel:    0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15
        fxs line:   0   1   x   x   2   3   x   x   4   5   x   x   6   7   x   x
	*/

	for(i=0; i<(ACG_NUMBER_OF_DSP_CHANNEL); i++)
	{
		/*ChannelInformation[i].conferenceMate = i +(ACG_NUMBER_OF_DSP_CHANNEL/2);*/
		ChannelInformation[i].conferenceMate = i +(AC49X_NUMBER_OF_CHANNELS/2);
		/* FXS line 0/1/....  is mapped to DSP channel 0/1/....  respectively */
		ChannelInformation[i].fxs_line = i/2;
		i++;
		ChannelInformation[i].conferenceMate = i +(AC49X_NUMBER_OF_CHANNELS/2);
        /* FXS line 0/1/....  is mapped to DSP channel 0/1/....  respectively */
        ChannelInformation[i].fxs_line = i/2+1;
		i+=2; /* 4 channels in 1 dsp, 2 & 3 are 0 & 1's matechannel not mapped with FXS line.  respectively*/
		/* Channels 4 & 5 are mapped with FXS line 2 & 3 */
	}
#else
	for(i=0; i<(ACG_NUMBER_OF_DSP_CHANNEL/2); i++)
	{
		ChannelInformation[i].conferenceMate = i +(ACG_NUMBER_OF_DSP_CHANNEL/2);
		/* FXS line 0/1/....  is mapped to DSP channel 0/1/....  respectively */
		ChannelInformation[i].fxs_line = i;
	}
#endif
	applicationStatusParams.is_default_init_device_parameters = 1;

	return 0;

}

int acgInitializeDSP_cmd(int argc, char* argv[])
{
	if(!applicationStatusParams.is_default_init_device_parameters)
	{
		printf("\nacgGetDefaultInitDeviceParameters was not called!!!\r\n");
		return (-1);
	}

	switch(argc)
	{
		case 1:
				break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

/* add by alex for supporting Tulip and SMB */
#if defined(S_PLATFORM)
        char                                                    *pFirmwareBuff = NULL;
       int                                              FileSize;
#if defined(AC49_SMB_VOIP_TOOLKIT) || defined(AC494_TULIP2_40) || defined(AC49_ORCHID_ATA_VOIP_TOOLKIT)
       char                                                     *pSecondFirmwareBuff = NULL;
       int                                              SecondFileSize;
#endif
#endif

#if defined(S_PLATFORM)
        if(ReadFirmware(DSP_FIRMWARE_NAME, &pFirmwareBuff, &FileSize)==-1)
        {
            printf("error while read firmware[%s]\r\n", DSP_FIRMWARE_NAME);
                return -1;
        }
        InitializationParams.PrimaryDSPDeviceInformation.pProgram = pFirmwareBuff;
        InitializationParams.PrimaryDSPDeviceInformation.ProgramSize = FileSize;
        InitializationParams.PrimaryDSPDeviceInformation.NumberOfDevices = 1;
#if defined(AC49_SMB_VOIP_TOOLKIT) || defined(AC49_ORCHID_ATA_VOIP_TOOLKIT)
       if(ReadFirmware(SECOND_DSP_FIRMWARE_NAME, &pSecondFirmwareBuff, &SecondFileSize)==-1)
        {
            printf("error while read second firmware[%s]\r\n", SECOND_DSP_FIRMWARE_NAME);
                return -1;
        }

        InitializationParams.SecondaryDSPDeviceInformation.pProgram = pSecondFirmwareBuff;
        InitializationParams.SecondaryDSPDeviceInformation.ProgramSize = SecondFileSize;
        InitializationParams.SecondaryDSPDeviceInformation.NumberOfDevices = 3;
#endif
#if defined(AC494_TULIP2_40)
       if(ReadFirmware(SECOND_DSP_FIRMWARE_NAME, &pSecondFirmwareBuff, &SecondFileSize)==-1)
        {
            perror("error while read second firmware\r\n");
                return -1;
        }

        InitializationParams.SecondaryDSPDeviceInformation.pProgram = pSecondFirmwareBuff;
        InitializationParams.SecondaryDSPDeviceInformation.ProgramSize = SecondFileSize;
        InitializationParams.SecondaryDSPDeviceInformation.NumberOfDevices = 1;
#endif
#else

        /* AC48x DSP Template */
        InitializationParams.DspVersionTemplateNumber = 0;

#endif /* defined(S_PLATFORM) */

/* alex */

#if defined(B_CEVA_PLATFORM)

	initDsp(&InitializationParams);

#else
	/* Initialize the DSP */
    if(acgInitializeDSP(&InitializationParams) != ACG_SUCCESS )
    {
        printf("error - acgInitializeDSP()\r\n");
        return -1;
    }
#endif /* defined(S_PLATFORM) */

#if defined(S_PLATFORM)
	free(InitializationParams.PrimaryDSPDeviceInformation.pProgram);
#if defined(AC49_SMB_VOIP_TOOLKIT) || defined(AC494_TULIP2_40) || defined(AC49_ORCHID_ATA_VOIP_TOOLKIT)
	free(InitializationParams.SecondaryDSPDeviceInformation.pProgram);
#endif
#endif /* defined(S_PLATFORM) */

#if POLLING_METHOD__BLOCKING
	launchDSPThreads();
#endif
	applicationStatusParams.is_dsp_device_initialized = 1;

	return 0;

}

int acgModifyCPT_cmd(int argc, char* argv[])
{
	int CPTIndex, i;
	acgTCallProgressTone *pCallProgressTone;
	acgECallProgressSignalType type;

	if(!applicationStatusParams.is_default_init_device_parameters)
	{
		printf("\nacgGetDefaultInitDeviceParameters was not called!!!\r\n");
		return (-1);
	}

	if(argc<2)
	{	/*strcpy(argv[1], argv[0]);*/
		help_cmd(2, argv);
		return (-1);
	}

	i = 1;
	CPTIndex = atoi(argv[i++]);

	/*For make sure CPTIndex is less and equal NumberOfCallProgressTones*/
	if(CPTIndex>InitializationParams.NumberOfCallProgressTones)
	{
		CPTIndex = InitializationParams.NumberOfCallProgressTones;
		printf("\nReached the end of index, call progress tone will appended as the last one, call progress tone index is changed to %d\n", CPTIndex);
	}

	if(CPTIndex > MAX_TONES_TO_PLAY-1)
	{
		printf("\ncall progess tone index exceed MAX_TONES_TO_PLAY\n");
		return (-1);
	}

	pCallProgressTone = &InitializationParams.CallProgressTones[CPTIndex];

	memset(pCallProgressTone, 0, sizeof(acgTCallProgressTone));

	if(i<argc)pCallProgressTone->Type = atoi(argv[i++]);  /*Call Progress Signal Type.*/
	if(i<argc)pCallProgressTone->ToneAFrequency = atoi(argv[i++]); /* Frequency, expressed in Hz, in range 300-1980 Hz, in steps of 1 Hz. Unused frequencies must be set to zero.*/
	if(i<argc)pCallProgressTone->ToneB_OrAmpModulationFrequency = atoi(argv[i++]);  /* Frequency, expressed in Hz, within the range of 300-1980 Hz, in steps of 1 Hz. Unused frequencies must be set to zero. If AM Factor <> 0, then this field represents the AM frequency in the range of 1-128 Hz.*/
	if(i<argc)pCallProgressTone->ToneALevel = atoi(argv[i++]);  /* Output level of the low frequency tone, in call progress generation. Output levels, ranging from -63dBm to 0 dBm. Units are in (-dBm)*/
	if(i<argc)pCallProgressTone->ToneBLevel = atoi(argv[i++]);  /* Output level of the high frequency tone, in Call Progress generation. Output levels, ranging from -63 dBm to 0 dBm. Units are in (-dBm)*/
	if(i<argc)pCallProgressTone->DetectionTimeOrCadenceFirstOnOrBurstDuration = atoi(argv[i++]);  /*!< The content of this field is according to the signal type: @li Continuous (1) = detection time. @li Cadence (2) = Ton state. Burst @li (3) = duration.*/
	if(i<argc)pCallProgressTone->CadenceFirstOffDuration = atoi(argv[i++]);  /* When the signal is cadence, this value represents the off duration. Units are in 10 msec. When it is not used, set it to zero.*/
	if(i<argc)pCallProgressTone->CadenceSecondOnDuration = atoi(argv[i++]);  /* When the signal is cadence, this value represents the on duration. Units are in 10 msec. When it is not used, set it to zero.*/
	if(i<argc)pCallProgressTone->CadenceSecondOffDuration = atoi(argv[i++]);  /* When the signal is cadence, this value represents the off duration. Units are in 10 msec. When it is not used, set it to zero.*/
	if(i<argc)pCallProgressTone->CadenceThirdOnDuration = atoi(argv[i++]);  /* When the signal is cadence, this value represents the on duration. Units are in 10 msec. When it is not used, set it to zero.*/
	if(i<argc)pCallProgressTone->CadenceThirdOffDuration = atoi(argv[i++]);  /* When the signal is cadence, this value represents the off duration. Units are in 10 msec. When it is not used, set it to zero.*/
	if(i<argc)pCallProgressTone->CadenceFourthOnDuration = atoi(argv[i++]);  /* When the signal is cadence, this value represents the on duration. Units are in 10 msec. When it is not used, set it to zero.*/
	if(i<argc)pCallProgressTone->CadenceFourthOffDuration = atoi(argv[i++]);  /* When the signal is cadence, this value represents the off duration. Units are in 10 msec. When it is not used, set it to zero.*/

	if(CPTIndex==InitializationParams.NumberOfCallProgressTones)InitializationParams.NumberOfCallProgressTones++;

	type = pCallProgressTone->Type;
	printf("\nToneIndex: %d Type: %s ToneAFrequency: %d ToneBFrequency: %d ToneALevel: %d ToneBLevel: %d "
		"FirstOnDuration: %d FirstOffDuration: %d "
		"SecondOnDuration: %d SecondOffDuration: %d "
		"ThirdOnDuration: %d ThirdOffDuration: %d "
		"FourthOnDuration: %d FourthOffDuration %d\n",
		CPTIndex,
		(type==0)?"CONTINUOUS":((type==1)?"CADENCE":((type==2)?"BURST":((type==3)?"SPECIAL":"UNKNOW"))),
		pCallProgressTone->ToneAFrequency,
		pCallProgressTone->ToneB_OrAmpModulationFrequency,
		pCallProgressTone->ToneALevel,
		pCallProgressTone->ToneBLevel,
		pCallProgressTone->DetectionTimeOrCadenceFirstOnOrBurstDuration,
		pCallProgressTone->CadenceFirstOffDuration,
		pCallProgressTone->CadenceSecondOnDuration,
		pCallProgressTone->CadenceSecondOffDuration,
		pCallProgressTone->CadenceThirdOnDuration,
		pCallProgressTone->CadenceThirdOffDuration,
		pCallProgressTone->CadenceFourthOnDuration,
		pCallProgressTone->CadenceFourthOffDuration);

	return (0);
}

int acgReInitializeDSP_cmd(int argc, char* argv[])
{
	if(!applicationStatusParams.is_dsp_device_initialized)
	{
		printf("\nacgInitializeDSP was not called!!!\r\n");
		return (-1);
	}

	switch(argc)
	{
		case 1:
				break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

	acgReInitializeDSP(&InitializationParams);

	return (0);
}

int acgCloseDSP_cmd(int argc, char* argv[])
{
/**************************************************************/
/**************************************************************/
	printf("This command is not supported in the RTP demo!!!\n");
	return (-1);
/**************************************************************/
/**************************************************************/
	switch(argc)
	{
		case 1:
				break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

	/* Close the DSP */
	if(acgCloseDSP() != ACG_SUCCESS )
	{
 		printf("error - acgCloseDSP()\r\n");
		return (-1);
	}

	DPRINTK("Closing the DSP\n");

	return 0;

}

int acgGetDefaultChannelParameters_cmd(int argc, char* argv[])
{
	if(!applicationStatusParams.is_dsp_device_initialized)
	{
		printf("\nacgInitializeDSP was not called!!!\r\n");
		return (-1);
	}

	switch(argc)
	{
		case 1:
				break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

	setLocalConnectionChannelMapping(-1, -1);
	/* Setting default parameters to all channels */
	if(acgGetDefaultChannelParameters(&ChannelParameters) != ACG_SUCCESS )
	{
		printf("error - acgGetDefaultChannelParameters()\r\n");
		return (-1);
	}

	DPRINTK("Getting default channel parameters\n");

	applicationStatusParams.is_default_channel_parameters = 1;

	return 0;

}

int acgOpenChannel_cmd(int argc, char* argv[])
{
	int dspChannel = -1;

	if(!applicationStatusParams.is_default_channel_parameters)
	{
		printf("\nacgGetDefaultChannelParameters was not called!!!\r\n");
		return (-1);
	}

	switch(argc)
	{
		case 2:
				dspChannel = atoi(argv[1]);
				break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

	if(isValidDSPChannel(dspChannel))
	{
		/* Open a DSP channel */
		if(acgOpenChannel(dspChannel, &ChannelParameters) != ACG_SUCCESS )
		{
			printf("error - acgOpenChannel()\r\n");
			return (-1);
		}
	}
	else
	{
		printf("Invalid DSP channel %d\r\n", dspChannel);
		return (-1);
	}

	DPRINTK("Opening channel %d\n", dspChannel);

	ChannelInformation[dspChannel].is_channel_open = 1;

	return 0;

}

int acgUpdateChannel_cmd(int argc, char* argv[])
{
	int dspChannel = -1;

	switch(argc)
	{
		case 2:
				dspChannel = atoi(argv[1]);
				break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

	if(!ChannelInformation[dspChannel].is_channel_open)
	{
		printf("\nacgOpenChannel was not called for channel %d!!!\r\n", dspChannel);
		return (-1);
	}

	if(isValidDSPChannel(dspChannel))
	{
		/* Update a DSP channel */
		if(acgUpdateChannel(dspChannel, &ChannelParameters) != ACG_SUCCESS )
		{
			printf("error - acgUpdateChannel()\r\n");
			return (-1);
		}
	}
	else
	{
		printf("Invalid DSP channel %d\r\n", dspChannel);
		return (-1);
	}

	DPRINTK("Updating channel %d\n", dspChannel);

	return 0;

}

int acgCloseChannel_cmd(int argc, char* argv[])
{
	int dspChannel = -1;

	switch(argc)
	{
		case 2:
				dspChannel = atoi(argv[1]);
				break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

	if(isValidDSPChannel(dspChannel))
	{
		if(!ChannelInformation[dspChannel].is_channel_open)
		{
			printf("\nacgOpenChannel was not called for channel %d - Closing it is meaningless!!!\r\n", dspChannel);
			return (-1);
		}

		/* Close a DSP channel */
		if(acgCloseChannel(dspChannel) != ACG_SUCCESS )
		{
			printf("error - acgCloseChannel()\r\n");
			return (-1);
		}

		ChannelInformation[dspChannel].is_channel_open = 0;
	}
	else
	{
		printf("Invalid DSP channel %d\r\n", dspChannel);
		return (-1);
	}

	DPRINTK("Closing channel %d\n", dspChannel);

	return 0;

}

int acgActivateRTP_cmd(int argc, char* argv[])
{
	int dspChannel = -1;

	switch(argc)
	{
		case 2:
				dspChannel = atoi(argv[1]);
				break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

	if(isValidDSPChannel(dspChannel))
	{
		if(!ChannelInformation[dspChannel].is_channel_open)
		{
			printf("\nacgOpenChannel was not called for channel %d - Can't activate RTP on a close channel!!!\r\n", dspChannel);
			return (-1);
		}

		/* Activate the DSP channel RTP stream */
		if(acgActivateRTP(dspChannel) != ACG_SUCCESS )
		{
			printf("error - acgActivateRTP()\r\n");
			return (-1);
		}

		ChannelInformation[dspChannel].is_rtp_activated = 1;
	}
	else
	{
		printf("Invalid DSP channel %d\r\n", dspChannel);
		return (-1);
	}

	DPRINTK("Activating RTP on channel %d\n", dspChannel);

	return 0;

}

int acgDeactivateRTP_cmd(int argc, char* argv[])
{
	int dspChannel = -1;

	switch(argc)
	{
		case 2:
			dspChannel = atoi(argv[1]);
			break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

	if(isValidDSPChannel(dspChannel))
	{
		if(!ChannelInformation[dspChannel].is_rtp_activated)
		{
			printf("\nacgActivateRTP was not called for channel %d - Deactivating it is meaningless!!!\r\n", dspChannel);
			return (-1);
		}

		/* Deactivates (closes) the DSP channel RTP stream */
		if(acgDeactivateRTP(dspChannel) != ACG_SUCCESS )
		{
			printf("error - acgDeactivateRTP()\r\n");
			return (-1);
		}

		ChannelInformation[dspChannel].is_rtp_activated = 0;
	}
	else
	{
		printf("Invalid DSP channel %d\r\n", dspChannel);
		return (-1);
	}

	DPRINTK("Deactivating RTP on channel %d\n", dspChannel);

	return 0;

}

int acgPlayTone_cmd(int argc, char* argv[])
{
	acgTTone 	Tone;

	int 			dspChannel = -1;

	memset(&Tone, 0, sizeof(Tone));

	switch(argc)
	{
		case 7:
				dspChannel = atoi(argv[1]);
				break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

	if(isValidDSPChannel(dspChannel))
	{
		if(!ChannelInformation[dspChannel].is_channel_open)
		{
			printf("\nacgOpenChannel was not called for channel %d - Can't play tone on a closed channel!!!\r\n", dspChannel);
			return (-1);
		}

		Tone.Direction = (acgEDirection)atoi(argv[2]);
		Tone.OnDuration = atoi(argv[3]);
		Tone.OffDuration = atoi(argv[4]);
		Tone.ToneElement.Type = (acgEToneType)atoi(argv[5]);
		Tone.ToneElement.ToneIndex = atoi(argv[6]);

		/* Play a tone towards the local TDM side or network side */
		if(acgPlayTone(dspChannel, &Tone) != ACG_SUCCESS )
		{
			printf("error - acgDeactivateRTP()\r\n");
			return (-1);
		}

		DPRINTK("Playing tone on channel %d Direction: %s OnDuration: %d OffDuration: %d Type: %s ToneIndex: %d\n",
					dspChannel, scDirection2Name[Tone.Direction], Tone.OnDuration, Tone.OffDuration,
					(Tone.ToneElement.Type ? "Call Progress Tone" : "DTMF"), Tone.ToneElement.ToneIndex);

	}
	else
	{
		printf("Invalid DSP channel %d\r\n", dspChannel);
		return (-1);
	}

	return 0;

}

int acgPlayToneSequence_cmd(int argc, char* argv[])
{
	acgTToneSequence 	Tones;

	int 			dspChannel = -1;
	int 			i;

	memset(&Tones, 0, sizeof(Tones));

	switch(argc)
	{
		case 8:
				dspChannel = atoi(argv[1]);
				break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

	if(isValidDSPChannel(dspChannel))
	{
		if(!ChannelInformation[dspChannel].is_channel_open)
		{
			printf("\nacgOpenChannel was not called for channel %d - Can't play tone on a closed channel!!!\r\n", dspChannel);
			return (-1);
		}

		Tones.Direction = (acgEDirection)atoi(argv[2]);
		Tones.OnDuration = atoi(argv[3]);
		Tones.OffDuration = atoi(argv[4]);
		Tones.NumOfTones = atoi(argv[5]);
		for(i=0; i<Tones.NumOfTones; i++){
			Tones.Tones[i].Type = (acgEToneType)atoi(argv[6]);
			Tones.Tones[i].ToneIndex = atoi(argv[7]);
		}

		/* Play a tone towards the local TDM side or network side */
		if(acgPlayToneSequence(dspChannel, &Tones) != ACG_SUCCESS )
		{
			printf("error - acgDeactivateRTP()\r\n");
			return (-1);
		}

		DPRINTK("Playing tones on channel %d Direction: %s OnDuration: %d OffDuration: %d NumOfTones: %d Type: %s ToneIndex: %d\n",
					dspChannel, scDirection2Name[Tones.Direction], Tones.OnDuration, Tones.OffDuration,
					Tones.NumOfTones,
					(Tones.Tones[0].Type ? "Call Progress Tone" : "DTMF"), Tones.Tones[0].ToneIndex);

	}
	else
	{
		printf("Invalid DSP channel %d\r\n", dspChannel);
		return (-1);
	}

	return 0;
}


int acgPlayToneSpecial_cmd(int argc, char* argv[])
{
	acgTToneExtended 	Tones;

	int 			dspChannel = -1;
	int             iNumberOfTones = 1;
	int 			i;

    if(argc > 3)
    {
        iNumberOfTones = atoi(argv[3]);
        if((argc != (iNumberOfTones*3+8))
            || (iNumberOfTones<1)
            || (iNumberOfTones>MAX_TONE_EXTENDED_TO_PLAY))
        {
            help_cmd(2, argv);
    		return (-1);
    	}
    }
    else
    {
        help_cmd(2, argv);
		return (-1);
	}

	memset(&Tones, 0, sizeof(Tones));

    dspChannel = atoi(argv[1]);

	if(isValidDSPChannel(dspChannel))
	{
		if(!ChannelInformation[dspChannel].is_channel_open)
		{
			printf("\nacgOpenChannel was not called for channel %d - Can't play tone on a closed channel!!!\r\n", dspChannel);
			return (-1);
		}

		Tones.GeneratorRedirection = (acgEDirection)atoi(argv[2]);  /*!< Tone Generation Redirection.*/
		Tones.Repeat = ACG_CONTROL__DISABLE;                        /*!< Repeat the string. @li CONTROL__DISABLE = String is generated only once. @li CONTROL__ENABLE = String is generated repeatedly.*/
		/*Tones.TotalDuration = 0;*/                               /*!< If the field Repeat is enabled, this field defines the total duration of the repeated string. If Repeat is disabled, the field is ignored. Resolution is 1 msec.*/
	    Tones.NumberOfDigits = iNumberOfTones;                 /*!< Number of digits in the string. Range: 1-6.*/
		Tones.UserDefinedToneA = atoi(argv[4]);                     /*!< This field enables users to generate any signal combined of three frequencies (UserDefinedToneA, UserDefinedToneB and UserDefinedToneC) on the fly. The field is valid only if users set parameter SystemCode to option ON_THE_FLY_TONES. Range is 0 - 3990 Hz, in units of Hertz.*/
		Tones.UserDefinedToneB = atoi(argv[5]);                     /*!< This field enables users to generate any signal combined of three frequencies (UserDefinedToneA, UserDefinedToneB and UserDefinedToneC) on the fly. The field is valid only if users set the parameter SystemCode to ON_THE_FLY_TONES. Range is 0 - 3990 Hz, in units of Hertz. When generating a single frequency signal, set this field to 0.*/
		Tones.UserDefinedToneC = atoi(argv[6]);                     /*!< This field enables users to generate any signal combined of three frequencies (UserDefinedToneA, UserDefinedToneB and UserDefinedToneC) on the fly. The field is valid only if users set the parameter SystemCode to ON_THE_FLY_TONES. Range is 0 - 3990 Hz, in units of Hertz. When generating a single frequency signal, set this field to 0.*/
		Tones.UserDefinedToneD = atoi(argv[7]);                     /*!< This field enables users to generate any signal combined of three frequencies (UserDefinedToneA, UserDefinedToneB and UserDefinedToneC) on the fly. The field is valid only if users set the parameter SystemCode to ON_THE_FLY_TONES. Range is 0 - 3990 Hz, in units of Hertz. When generating a single frequency signal, set this field to 0.*/
		Tones.AmFactor = 0;

		for(i=0; i<Tones.NumberOfDigits; i++)
		{
			Tones.Tones[i].SignalLevel_minus_dbm = atoi(argv[7+i*3+1]);  /*!< Signal Level, in -dBm resolution. For User-Defined Tones signals or Call Progress Tones signals, if the value 0 is defined in this field (SignalLevel) the levels defined in the ::Ac49xUserDefinedTonesDeviceConfiguration or ::Ac49xCallProgressDeviceConfiguration functions will be generated.*/
			Tones.Tones[i].Digit = atoi(argv[7+i*3+2]);                  /*!< Digit to be played. Refer to <A HREF="DIGIT Field According to Signaling Type.html">Digit Field According to Signaling Type table</A> and to <A HREF="Advanced Signals.html"> Advanced Signals Digits</A>.*/
			Tones.Tones[i].SignalDuration = atoi(argv[7+i*3+3]);
			Tones.Tones[i].SystemCode = ACG_IBS_SYSTEM_CODE__ON_THE_FLY_TONES;
		}
		/* Play a tone towards the local TDM side or network side */
		if(acgPlayToneExtended(dspChannel, &Tones) != ACG_SUCCESS )
		{
			printf("error - acgDeactivateRTP()\r\n");
			return (-1);
		}

		DPRINTK("\nPlaying tones on channel %d Direction: %s NumberOfTones: %d FrequencyA: %dHz FrequencyB: %dHz FrequencyC: %dHz FrequencyD: %dHz\n",
					dspChannel, scDirection2Name[Tones.GeneratorRedirection],
					Tones.NumberOfDigits,
					Tones.UserDefinedToneA,  Tones.UserDefinedToneB,  Tones.UserDefinedToneC,  Tones.UserDefinedToneD);

	}
	else
	{
		printf("Invalid DSP channel %d\r\n", dspChannel);
		return (-1);
	}

	return 0;
}

int acgPlayAmTone_cmd(int argc, char* argv[])
{
	acgTToneExtended 	Tones;

	int 			dspChannel = -1;

	memset(&Tones, 0, sizeof(Tones));

	switch(argc)
	{
		case 7:
				dspChannel = atoi(argv[1]);
				break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

	if(isValidDSPChannel(dspChannel))
	{
		if(!ChannelInformation[dspChannel].is_channel_open)
		{
			printf("\nacgOpenChannel was not called for channel %d - Can't play tone on a closed channel!!!\r\n", dspChannel);
			return (-1);
		}

		Tones.GeneratorRedirection = (acgEDirection)atoi(argv[2]); /*!< Tone Generation Redirection.*/
		Tones.Repeat = ACG_CONTROL__DISABLE;                     /*!< Repeat the string. @li CONTROL__DISABLE = String is generated only once. @li CONTROL__ENABLE = String is generated repeatedly.*/
		Tones.UserDefinedToneA = atoi(argv[4]);                         /*!< This field enables users to generate any signal combined of three frequencies (UserDefinedToneA, UserDefinedToneB and UserDefinedToneC) on the fly. The field is valid only if users set parameter SystemCode to option ON_THE_FLY_TONES. Range is 0 - 3990 Hz, in units of Hertz.*/
		Tones.UserDefinedToneB = atoi(argv[5]);                         /*!< This field enables users to generate any signal combined of three frequencies (UserDefinedToneA, UserDefinedToneB and UserDefinedToneC) on the fly. The field is valid only if users set the parameter SystemCode to ON_THE_FLY_TONES. Range is 0 - 3990 Hz, in units of Hertz. When generating a single frequency signal, set this field to 0.*/
		Tones.AmFactor = atoi(argv[6]);

		Tones.Tones[0].SignalLevel_minus_dbm = 0;  /*!< Signal Level, in -dBm resolution. For User-Defined Tones signals or Call Progress Tones signals, if the value 0 is defined in this field (SignalLevel) the levels defined in the ::Ac49xUserDefinedTonesDeviceConfiguration or ::Ac49xCallProgressDeviceConfiguration functions will be generated.*/
		Tones.Tones[0].Digit = 7;                  /*!< Digit to be played. Refer to <A HREF="DIGIT Field According to Signaling Type.html">Digit Field According to Signaling Type table</A> and to <A HREF="Advanced Signals.html"> Advanced Signals Digits</A>.*/
		Tones.Tones[0].SignalDuration = atoi(argv[3]);
		Tones.Tones[0].SystemCode = ACG_IBS_SYSTEM_CODE__ON_THE_FLY_TONES;
		Tones.NumberOfDigits = 1;
		/* Play a tone towards the local TDM side or network side */
		if(acgPlayToneExtended(dspChannel, &Tones) != ACG_SUCCESS )
		{
			printf("error - acgDeactivateRTP()\r\n");
			return (-1);
		}

		DPRINTK("\nPlaying Am tones on channel %d Direction: %s Duration: %d FrequencyA: %dHz FrequencyB: %dHz AmFactor %d\n",
					dspChannel, scDirection2Name[Tones.GeneratorRedirection],
					Tones.Tones[0].SignalDuration,
					Tones.UserDefinedToneA,  Tones.UserDefinedToneB,Tones.AmFactor);

	}
	else
	{
		printf("Invalid DSP channel %d\r\n", dspChannel);
		return (-1);
	}

	return 0;
}


int acgStopTone_cmd(int argc, char* argv[])
{
	int dspChannel = -1;

	switch(argc)
	{
		case 2:
				dspChannel = atoi(argv[1]);
				break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

	if(isValidDSPChannel(dspChannel))
	{
		if(!ChannelInformation[dspChannel].is_channel_open)
		{
			printf("\nacgOpenChannel was not called for channel %d - Can't stop tone on a close channel!!!\r\n", dspChannel);
			return (-1);
		}

		/* Stop a tone if already been generated towards the local TDM side or network side */
		if(acgStopTone(dspChannel) != ACG_SUCCESS )
		{
			printf("error - acgStopTone()\r\n");
			return (-1);
		}
	}
	else
	{
		printf("Invalid DSP channel %d\r\n", dspChannel);
		return (-1);
	}

	DPRINTK("Stopping tone on channel %d\n", dspChannel);

	return 0;

}

int acgPlayMWI_cmd(int argc, char* argv[])
{
	acgECallerIdServiceType CallerIdServiceType;

	int dspChannel = -1;

	switch(argc)
	{
		case 2:
				dspChannel = atoi(argv[1]);
				break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

	if(isValidDSPChannel(dspChannel))
	{
		if(!ChannelInformation[dspChannel].is_channel_open)
		{
			printf("\nacgOpenChannel was not called for channel %d - Can't play MWI on a close channel!!!\r\n", dspChannel);
			return (-1);
		}

		CallerIdServiceType = get_hook_state(ChannelInformation[dspChannel].fxs_line);

		DPRINTK("Playing MWI on channel %d while in %s\n",
					dspChannel, (CallerIdServiceType ? "off hook state (type 2)" : "on hook state (type 1)"));

		/* Play MWI */
		if(acgPlayMWI(dspChannel, CallerIdServiceType) != ACG_SUCCESS )
		{
			printf("error - acgPlayMWI()\r\n");
			return (-1);
		}
	}
	else
	{
		printf("Invalid DSP channel %d\r\n", dspChannel);
		return (-1);
	}

	DPRINTK("Playing MWI on channel %d\n", dspChannel);

	return 0;

}

int acgStopMWI_cmd(int argc, char* argv[])
{
	acgECallerIdServiceType CallerIdServiceType;

	int dspChannel = -1;

	switch(argc)
	{
		case 2:
				dspChannel = atoi(argv[1]);
				break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

if(isValidDSPChannel(dspChannel))
	{
		if(!ChannelInformation[dspChannel].is_channel_open)
		{
			printf("\nacgOpenChannel was not called for channel %d - Can't stop MWI on a close channel!!!\r\n", dspChannel);
			return (-1);
		}

		CallerIdServiceType =  get_hook_state(ChannelInformation[dspChannel].fxs_line);

		/* Stop MWI */
		if(acgStopMWI(dspChannel, CallerIdServiceType) != ACG_SUCCESS )
		{
			printf("error - acgStopMWI()\r\n");
			return (-1);
		}
	}
	else
	{
		printf("Invalid DSP channel %d\r\n", dspChannel);
		return (-1);
	}

	DPRINTK("Stop playing MWI on channel %d\n", dspChannel);

	return 0;

}

static int acgIsDTMFOfCallerID(void)
{
	if ( (ACG_CALLER_ID_STANDARD__DTMF_CLIP_ETSI == ChannelParameters.CallerIDParameters.CallerIdStandard)   ||
	     (ACG_CALLER_ID_STANDARD__DTMF_CLIP_DANISH == ChannelParameters.CallerIDParameters.CallerIdStandard) ||
	     (ACG_CALLER_ID_STANDARD__DTMF_CLIP_INDIAN == ChannelParameters.CallerIDParameters.CallerIdStandard) ||
	     (ACG_CALLER_ID_STANDARD__DTMF_CLIP_BRAZILIAN == ChannelParameters.CallerIDParameters.CallerIdStandard) )
	    	return 1;
	else
	    	return 0;
}

int acgPlayCallerID_cmd(int argc, char* argv[])
{
	acgECallerIdServiceType 		CallerIdServiceType;
	acgTCallerId 				    CallerId;
	acgEPhoneCallerNameStatus		nameStatus=0;
	acgEPhoneCallerNumberStatus 	numberStatus=0;
	U8								callType = 0x01;

	int dspChannel = -1;

	memset(&CallerId, 0, sizeof(CallerId));

	switch(argc)
	{
		case 8:
				dspChannel = atoi(argv[1]);
				nameStatus = (acgEPhoneCallerNameStatus)atoi(argv[4]);
				numberStatus = (acgEPhoneCallerNumberStatus)atoi(argv[5]);
				break;

        case 3:
                dspChannel = atoi(argv[1]);
                if(!acgIsDTMFOfCallerID())
                {
                    printf("\nerror:The current g_cCallerIdStandard is not DTMF mode on channel %d\r\n", dspChannel);
			        return (-1);
                }
                break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

	if(isValidDSPChannel(dspChannel))
	{
	    if(!ChannelInformation[dspChannel].is_channel_open)
		{
			printf("\nacgOpenChannel was not called for channel %d - Can't play g_cCaller id on a close channel!!!\r\n", dspChannel);
			return (-1);
		}

		if(3 != argc)
		{
    		if((nameStatus > ACG_PHONE_CALLER_NAME_UNAVAILABLE) || (numberStatus > ACG_PHONE_CALLER_NAME_UNAVAILABLE))
    		{
    			printf("\nThe arrange of status is 0-2!!!\r\n");
    			return (-1);
    		}
    	}

		CallerIdServiceType = (acgECallerIdServiceType)get_hook_state(ChannelInformation[dspChannel].fxs_line);

		CallerId.CallerIdServiceType = CallerIdServiceType;
		strcpy(CallerId.Number, argv[2]);
		if(3 != argc)
		{
    		strcpy(CallerId.Name, argv[3]);
    		strcpy(CallerId.Time, argv[6]);

			callType = (U8)atoi(argv[7]);
    		switch(callType)
    		{
    			case 1:
    					CallerId.CallType = 0x01;
    					break;
    	        case 2:
    					CallerId.CallType = 0x02;
    					break;
    			case 3:
    					CallerId.CallType = 0x03;
    					break;
    	        case 4:
    					CallerId.CallType = 0x04;
    					break;
    			case 5:
    					CallerId.CallType = 0x05;
    					break;
    	        case 6:
    					CallerId.CallType = 0x06;
    					break;
    			case 7:
    					CallerId.CallType = 0x07;
    					break;
    	        case 10:
    					CallerId.CallType = 0x10;
    					break;
    	        case 11:
    					CallerId.CallType = 0x11;
    					break;
    			case 50:
    					CallerId.CallType = 0x50;
    					break;
    			case 81:
    					CallerId.CallType = 0x81;
    					break;
    			default:
    					help_cmd(2, argv);
    					printf ("Legal values for Call Type: 1, 2 ,3, 4, 5, 6, 7, 10, 11, 50, 81\n");
    					return (-1);
    					break;
    		}
    		CallerId.NameStatus = nameStatus;
    		CallerId.NumberStatus = numberStatus;
        }

		/* Generate g_cCaller Id towards the local TDM */
		if(acgPlayCallerID(dspChannel, &CallerId) != ACG_SUCCESS )
		{
			printf("error - acgPlayg_cCallerID()\r\n");
			return (-1);
		}

		DPRINTK("Playing g_cCaller id on channel %d Name: %s Number: %s Service Type: %s Name Status: %s Number Status: %s DateTime: %s\n",
					dspChannel, CallerId.Name, CallerId.Number,
					(CallerIdServiceType ? "off hook state (type 2)" : "on hook state (type 1)"),
					ACG_PHONE_CALLER_NAME_PRESENT==nameStatus ? "Name Present": (ACG_PHONE_CALLER_NAME_PRIVATE==nameStatus ? "Name Private" : "Name Suppressed"),
					ACG_PHONE_CALLER_NUMBER_PRESENT==numberStatus ? "Number Present": (ACG_PHONE_CALLER_NUMBER_PRIVATE==numberStatus ? "Number Private" : "Number Suppressed"),
					CallerId.Time);

	}
	else
	{
		printf("Invalid DSP channel %d\r\n", dspChannel);
		return (-1);
	}

	return 0;

}

int acgStopCallerID_cmd(int argc, char* argv[])
{
	int dspChannel = -1;

#if defined(B_CEVA_PLATFORM)
/**************************************************************/
/**************************************************************/
	printf("This command is not supported under the current DSP software!!!\n");
	return (-1);
/**************************************************************/
/**************************************************************/
#endif

	switch(argc)
	{
		case 2:
				dspChannel = atoi(argv[1]);
				break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

	if(isValidDSPChannel(dspChannel))
	{
		if(!ChannelInformation[dspChannel].is_channel_open)
		{
			printf("\nacgOpenChannel was not called for channel %d - Can't stop g_cCaller id on a close channel!!!\r\n", dspChannel);
			return (-1);
		}

		/* Stop g_cCaller Id generation */
		if(acgStopCallerID(dspChannel) != ACG_SUCCESS )
		{
			printf("error - acgStopg_cCallerID()\r\n");
			return (-1);
		}
	}
	else
	{
		printf("Invalid DSP channel %d\r\n", dspChannel);
		return (-1);
	}

	DPRINTK("Stop g_cCaller id on channel %d\n", dspChannel);

	return 0;
}

int acgStart3WayConference_cmd(int argc, char* argv[])
{

	int dspChannel = -1;

	switch(argc)
	{
		case 2:
				dspChannel = atoi(argv[1]);
				break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

	if(isValidDSPChannel(dspChannel))
	{
		if(!ChannelInformation[dspChannel].is_channel_open)
		{
			printf("\nacgOpenChannel was not called for channel %d - 3 way conferencing will not work on a channel which was not RTP activated!!!\r\n", dspChannel);
			//return (-1);
		}
		if(!ChannelInformation[ChannelInformation[dspChannel].conferenceMate].is_channel_open)
		{
			printf("\nacgOpenChannel was not called for channel %d - 3 way conferencing will not work on a channel which was not RTP activated!!!\r\n", ChannelInformation[dspChannel].conferenceMate);
			//return (-1);
		}

		/* Start 3 way conference */
		//if(acgStart3WayConference(dspChannel, ChannelInformation[dspChannel].conferenceMate) != ACG_SUCCESS )
		if(acgStart3WayConference(0, 2) != ACG_SUCCESS )
		{
			printf("error - acgStart3WayConference()\r\n");
			return (-1);
		}
	}
	else
	{
		printf("Invalid DSP channel %d\r\n", dspChannel);
		return (-1);
	}

	DPRINTK("Three way conferencing was established between DSP channels %d and %d\n", dspChannel, ChannelInformation[dspChannel].conferenceMate);

	return 0;

}

int acgStop3WayConference_cmd(int argc, char* argv[])
{

	int dspChannel = -1;

	switch(argc)
	{
		case 2:
				dspChannel = atoi(argv[1]);
				break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

	if(isValidDSPChannel(dspChannel))
	{
		/* Stop 3 way conference */
		if(acgStop3WayConference(dspChannel, ChannelInformation[dspChannel].conferenceMate) != ACG_SUCCESS )
		{
			printf("error - acgStop3WayConference()\r\n");
			return (-1);
		}
	}
	else
	{
		printf("Invalid DSP channel %d\r\n", dspChannel);
		return (-1);
	}

	DPRINTK("Stop three way conferencing between DSP channels %d and %d\n", dspChannel, ChannelInformation[dspChannel].conferenceMate);

	return 0;

}

int acgReinitializeEchoCanceller_cmd(int argc, char* argv[])
{

	int dspChannel = -1;


	switch(argc)
	{
		case 2:
				dspChannel = atoi(argv[1]);
				break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

	if(isValidDSPChannel(dspChannel))
	{
		if(!ChannelInformation[dspChannel].is_channel_open)
		{
			printf("\nacgOpenChannel was not called for channel %d - Can't reinitialize echo canceller on a close channel!!!\r\n", dspChannel);
			return (-1);
		}

		/* Reinitialize echo canceller */
		if(acgReinitializeEchoCanceller(dspChannel) != ACG_SUCCESS )
		{
			printf("error - acgReinitializeEchoCanceller()\r\n");
			return (-1);
		}
	}
	else
	{
		printf("Invalid DSP channel %d\r\n", dspChannel);
		return (-1);
	}

	DPRINTK("Reinitializing echo canceller on channel %d\n", dspChannel);

	return 0;

}


int acgDebugInformationActivation_cmd(int argc, char* argv[])
{
	acgTDebugInformation DebugInformation;

	int dspChannel = -1;

	switch(argc)
	{
		case 5:
				dspChannel = atoi(argv[1]);
				break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

	memset(&DebugInformation, 0, sizeof(DebugInformation));

	if(isValidDSPChannel(dspChannel))
	{
		DebugInformation.PacketRecording = (acgEControl)atoi(argv[2]);

		DebugInformation.SpecificRecordingInformation.Channel = (int)atoi(argv[1]);
		DebugInformation.SpecificRecordingInformation.NetworkRecording = (acgEControl)atoi(argv[3]);
		DebugInformation.SpecificRecordingInformation.TDMRecording = (acgEControl)atoi(argv[4]);

		/* Debug information activation */
		if(acgDebugInformationActivation(&DebugInformation) != ACG_SUCCESS)
		{
			printf("error - acgDebugInformationActivation()\r\n");
			return (-1);
		}
	}
	else
	{
		printf("Invalid DSP channel %d\r\n", dspChannel);
		return (-1);
	}

	DPRINTK("Debug Information Activation\n");

	return 0;
}


int acgInitializeFXS_cmd(int argc, char* argv[])
{
	acgTFXSInitializationParameters	FXSInitializationParameters;

	if(!applicationStatusParams.is_dsp_device_initialized)
	{
		printf("\nacgInitializeDSP was not called!!!\r\n");
		return (-1);
	}

	switch(argc)
	{
		case 1:
				break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

#if defined(S_PLATFORM)
	system("insmod /tmp/ac49x_mpi.ko");
#if defined(AC494_TULIP2_21) || defined(AC494_TULIP2_40) || defined(FXS_SILABS) || defined(FXO_SILABS)
	system("insmod /tmp/silabs_daa_3050_drv_mod.ko");
	system("insmod /tmp/silabs_fxs_3215_drv_mod.ko");
#else
	system("insmod /tmp/le88.ko");
#endif
#endif
	/* Initialize the SLIC device */

#if defined(AC494_TULIP2_21) || defined(FXO_SILABS)
	acgInitializeDAA(); /* DAA must init before fxs. And if there is daa device. you need to initialize it even though you don't need it now. */
#endif
	FXSInitializationParameters.coefficients = NULL;
	if(acgInitializeFXS(&FXSInitializationParameters) != ACG_SUCCESS )
	{
		printf("error - acgInitializeFXS()\r\n");
		return (-1);
	}

#if POLLING_METHOD__BLOCKING

#if defined(AC494_TULIP2_21) || defined(FXO_SILABS)
	if(launchFxoThread() != ACG_SUCCESS )
	{
		printf("error - launchFxo()\r\n");
		return (-1);
	}
#endif
	if(launchFxsThread() != ACG_SUCCESS )
	{
		printf("error - launchFxs()\r\n");
		return (-1);
	}

#else

	if(launchGlobalThread() != ACG_SUCCESS )
	{
		printf("error - launchFxs()\r\n");
		return (-1);
	}

#endif

	DPRINTK("Initializing the FXS...Done\n");

	applicationStatusParams.is_slic_device_initialized = 1;

	return 0;

}

int acgCloseFXS_cmd(int argc, char* argv[])
{

/**************************************************************/
/**************************************************************/
	printf("This command is not supported in the RTP demo!!!\n");
	return (-1);
/**************************************************************/
/**************************************************************/

	if(!applicationStatusParams.is_slic_device_initialized)
	{
		printf("\nacgInitializeFXS was not called!!!\r\n");
		return (-1);
	}

	switch(argc)
	{
		case 1:
				break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

	/* Close the SLIC device */
	if(acgCloseFXS() != ACG_SUCCESS )
	{
		printf("error - acgCloseFXS()\r\n");
		return (-1);
	}

	applicationStatusParams.is_slic_device_initialized = 0;

	return 0;

}

int acgStartRing_cmd(int argc, char* argv[])
{

	int line = -1;

	switch(argc)
	{
		case 2:
				line = atoi(argv[1]);
				break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

	if(isValidFXSLine(line))
	{
		if(!applicationStatusParams.is_slic_device_initialized)
		{
			printf("\nacgInitializeFXS was not called!!!\r\n");
			return (-1);
		}

		/* Start ring on a FXS port */
		if(acgStartRing(line) != ACG_SUCCESS )
		{
			printf("error - acgStartRing()\r\n");
			return (-1);
		}
	}
	else
	{
		printf("Invalid FXS line %d\r\n", line);
		return (-1);
	}

	DPRINTK("Start ringing on line %d\n", line);

	return 0;

}

int acgStopRing_cmd(int argc, char* argv[])
{

	int line = -1;

	switch(argc)
	{
		case 2:
				line = atoi(argv[1]);
				break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

	if(isValidFXSLine(line))
	{
		if(!applicationStatusParams.is_slic_device_initialized)
		{
			printf("\nacgInitializeFXS was not called!!!\r\n");
			return (-1);
		}

		/* Stop ring on a FXS port */
		if(acgStopRing(line) != ACG_SUCCESS )
		{
			printf("error - acgStopRing()\r\n");
			return (-1);
		}
	}
	else
	{
		printf("Invalid FXS line %d\r\n", line);
		return (-1);
	}

	DPRINTK("Stop ringing on line %d\n", line);

	return 0;

}

int acgSeizeLine_cmd(int argc, char* argv[])
{

	int line = -1;

	switch(argc)
	{
		case 2:
				line = atoi(argv[1]);
				break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

	if(isValidFXSLine(line))
	{
		if(!applicationStatusParams.is_slic_device_initialized)
		{
			printf("\nacgInitializeFXS was not called!!!\r\n");
			return (-1);
		}

		/* Seize a line on a FXS port */
		if(acgSeizeLine(line) != ACG_SUCCESS )
		{
			printf("error - acgSeizeLine()\r\n");
			return (-1);
		}
	}
	else
	{
		printf("Invalid FXO line %d\r\n", line);
		return (-1);
	}

	DPRINTK("Seize line on line %d\n", line);

	return 0;

}

int acgReleaseLine_cmd(int argc, char* argv[])
{

	int line = -1;

	switch(argc)
	{
		case 2:
				line = atoi(argv[1]);
				break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

	if(isValidFXSLine(line))
	{
		if(!applicationStatusParams.is_slic_device_initialized)
		{
			printf("\nacgInitializeFXS was not called!!!\r\n");
			return (-1);
		}

		/* Release line on a FXS port */
		if(acgReleaseLine(line) != ACG_SUCCESS )
		{
			printf("error - acgReleaseLine()\r\n");
			return (-1);
		}
	}
	else
	{
		printf("Invalid FXO line %d\r\n", line);
		return (-1);
	}

	DPRINTK("Release line on line %d\n", line);

	return 0;

}

int acgGenerateFlash_cmd(int argc, char* argv[])
{

	int line = -1;

	switch(argc)
	{
		case 2:
				line = atoi(argv[1]);
				break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

	if(isValidFXSLine(line))
	{
		if(!applicationStatusParams.is_slic_device_initialized)
		{
			printf("\nacgInitializeFXS was not called!!!\r\n");
			return (-1);
		}

		/* Generate a FLASH on a FXO port */
		if(acgGenerateFlash(line) != ACG_SUCCESS )
		{
			printf("error - acgGenerateFlash()\r\n");
			return (-1);
		}
	}
	else
	{
		printf("Invalid FXO line %d\r\n", line);
		return (-1);
	}

	DPRINTK("Generate FLASH on line %d\n", line);

	return 0;

}

int exit_cmd(int argc, char* argv[])
{
	printf("Exiting Gateway API Sample Application\n");

	closeMediaEvents();

	return EXIT_CODE;
}

int help_cmd(int argc, char* argv[])
{
	int 		i;
	int		isCommandAvailable=0;

	switch(argc)
	{

		case 1:
				printf("\nThe following commands are available:\n\n");
				break;
		default:
				break;
	}

	for(i=0; strlen(Commands[i].name)>0 ; i++)
		if( (argc==1) || !strcmp(argv[0],Commands[i].name) )
		{
			printf("%s - %s\n%s\n\n",Commands[i].name, Commands[i].description, Commands[i].syntax);
			isCommandAvailable = 1;
		}
	if( !isCommandAvailable)
		printf("\nThe requested command is not available.\n");

	return 0;

}


/*******************************************************
********************************************************

				Sample Application Commands

********************************************************
********************************************************/

int SetChannelParam_cmd(int argc, char* argv[])
{
	int 		channelParamValue = -1;
	sub_cmd_t   retCmdType = E_ERROR;

    retCmdType = GetChannelCmdType(argc, argv);
    if((E_BypassJitter != retCmdType)
        && (E_DataBypassOutputGain != retCmdType)
        && (E_FaxBypassOutputGain != retCmdType)
    )
    {
    	if(!applicationStatusParams.is_default_channel_parameters)
    	{
    		printf("\nacgGetDefaultChannelParameters was not called!!!\r\n");
    		return (-1);
    	}
    }

    if((0 < retCmdType) && (retCmdType < E_ERROR))
        channelParamValue = atoi(argv[3]);

    switch(retCmdType)
    {
    /*    Signaling cmds*/
    case E_DTMFTransferMode:
        if(channelParamValue>=0 && channelParamValue<=4)
        {
    	    ChannelParameters.SignalingParameters.DTMFTransportMethod = channelParamValue;
    	    DPRINTK("Setting DTMFTransportMethod to %d\n", ChannelParameters.SignalingParameters.DTMFTransportMethod);
        }
        else
    	    DPRINTK("\n%s::Illegal input - the range set for DTMFTransportMethod is 0-4\n", __FUNCTION__);
        break;
	case E_CPTTransferMode:
        if(channelParamValue==2 || channelParamValue==4)
        {
    	    ChannelParameters.SignalingParameters.CPTTransportMethod = channelParamValue;
    	    DPRINTK("Setting CPTTransportMethod to %d\n", ChannelParameters.SignalingParameters.CPTTransportMethod);
        }
        else
    	    DPRINTK("\n%s::Illegal input - the range set for CPTTransportMethod is Transparent (2) or Mixed (4)\n", __FUNCTION__);
        break;
    case E_DetectCallProgress:
    	ChannelParameters.SignalingParameters.DetectCallProgress = channelParamValue;
    	DPRINTK("Setting DetectCallProgress to %d\n", ChannelParameters.SignalingParameters.DetectCallProgress);
        if( ChannelParameters.SignalingParameters.DetectCallProgress == ACG_CONTROL__ENABLE )
            DPRINTK("Detection from network is committed only for G.711\n");
    	break;
    case E_DetectDTMFRegister:
    	ChannelParameters.SignalingParameters.DetectDTMFRegister = channelParamValue;
    	DPRINTK("Setting DetectDTMFRegister to %d\n", ChannelParameters.SignalingParameters.DetectDTMFRegister);
        if( ChannelParameters.SignalingParameters.DetectDTMFRegister == ACG_CONTROL__ENABLE )
            DPRINTK("Detection from network is committed only for G.711\n");
        break;
    case E_IBSRedirection:
        ChannelParameters.SignalingParameters.DetectionRedirection = channelParamValue;
    	DPRINTK("Setting IbsDetectionRedirection to %d\n", ChannelParameters.SignalingParameters.DetectionRedirection);
    	break;
    case E_IBSLevel:
        ChannelParameters.SignalingParameters.IbsLevel_minus_dbm = channelParamValue;
    	DPRINTK("Setting IbsLevel_minus_dbm to %d\n", ChannelParameters.SignalingParameters.IbsLevel_minus_dbm);
    	break;
    case E_DetectLongDTMF:
        ChannelParameters.SignalingParameters.LongDtmfDetectionEnable = channelParamValue;
    	DPRINTK("Setting LongDtmfDetectionEnable to %d\n", ChannelParameters.SignalingParameters.LongDtmfDetectionEnable);
    	break;
    /*    Gain cmds*/
    case E_InputGain:
        ChannelParameters.GainParameters.InputGain = channelParamValue;
    	DPRINTK("Setting InputGain to %d\n", ChannelParameters.GainParameters.InputGain);
        break;
    case E_VoiceOutputGain:
        ChannelParameters.GainParameters.VoiceOutputGain = channelParamValue;
    	DPRINTK("Setting VoiceOutputGain to %d\n", ChannelParameters.GainParameters.VoiceOutputGain);
        break;
    /*    g_cCallerID cmds*/
    case E_CIDOnHook:
        ChannelParameters.CallerIDParameters.CallerIdOnHookDetection = channelParamValue;
    	DPRINTK("Setting g_cCallerIdOnHookDetection to %d\n", ChannelParameters.CallerIDParameters.CallerIdOnHookDetection);
        break;
    case E_CIDOffHook:
        ChannelParameters.CallerIDParameters.CallerIdOffHookDetection = channelParamValue;
    	DPRINTK("Setting g_cCallerIdOffHookDetection to %d\n", ChannelParameters.CallerIDParameters.CallerIdOffHookDetection);
        break;
    case E_CIDStandard:
        ChannelParameters.CallerIDParameters.CallerIdStandard = channelParamValue;
    	DPRINTK("Setting g_cCallerIdStandard to %d\n", ChannelParameters.CallerIDParameters.CallerIdStandard);
        break;
    /*    JitterBuffer cmds*/
    case E_VoiceJBMin:
        if(channelParamValue <= ChannelParameters.JitterBufferParameters.VoiceJitterBufferMaxDelay)
        {
            ChannelParameters.JitterBufferParameters.VoiceJitterBufferMinDelay = channelParamValue;
    	    DPRINTK("Setting VoiceJitterBufferMinDelay to %d\n", ChannelParameters.JitterBufferParameters.VoiceJitterBufferMinDelay);
    	}
    	else
    	{
    	    printf("\nThe value should be not greater than current VoiceJitterBufferMaxDelay[%d]\n", ChannelParameters.JitterBufferParameters.VoiceJitterBufferMaxDelay);
    	}
        break;
    case E_VoiceJBMax:
        if(channelParamValue >= ChannelParameters.JitterBufferParameters.VoiceJitterBufferMinDelay)
        {
            ChannelParameters.JitterBufferParameters.VoiceJitterBufferMaxDelay = channelParamValue;
    	    DPRINTK("Setting VoiceJitterBufferMaxDelay to %d\n", ChannelParameters.JitterBufferParameters.VoiceJitterBufferMaxDelay);
    	}
    	else
    	{
    	    printf("\nThe value should be not less than current VoiceJitterBufferMinDelay[%d]\n", ChannelParameters.JitterBufferParameters.VoiceJitterBufferMinDelay);
    	}
        break;
    case E_JBOptimizationFactor:
        ChannelParameters.JitterBufferParameters.DynamicJitterBufferOptimizationFactor = channelParamValue;
    	DPRINTK("Setting DynamicJitterBufferOptimizationFactor to %d\n", ChannelParameters.JitterBufferParameters.DynamicJitterBufferOptimizationFactor);
        break;
    /*EchoCanceller cmds*/
    case E_ECNLP:
        ChannelParameters.EchoCancellerParameters.EchoCancelerNonLinearProcessor = channelParamValue;
    	DPRINTK("Setting EchoCancelerNonLinearProcessor to %d\n", ChannelParameters.EchoCancellerParameters.EchoCancelerNonLinearProcessor);
        break;
    case E_ECMode:
        ChannelParameters.EchoCancellerParameters.EchoCanceler = channelParamValue;
    	DPRINTK("Setting EchoCanceler to %d\n", ChannelParameters.EchoCancellerParameters.EchoCanceler);
        break;
    case E_EchoCancellerLength:
        if(channelParamValue>=0 && channelParamValue<=3)
        {
    	    ChannelParameters.EchoCancellerParameters.EchoCancelerLength = channelParamValue;
    		DPRINTK("Setting EchoCancellerLength to %d\n", ChannelParameters.EchoCancellerParameters.EchoCancelerLength);
        }
        else
    	    DPRINTK("\n%s::Illegal input - the range set for EchoCancellerLength is 0-3(8msec - 32msec)\n", __FUNCTION__);
    	break;
    /*    FaxModem cmds*/
    case E_FaxMethod:
        /*
        if( (ACG_FAX_MODEM_TRANSPORT__RELAY != channelParamValue) &&
            (ChannelParameters.FaxModemParameters.ModemTransferMethod != channelParamValue))
        {
            DPRINTK("Illigal Fax/Modem Configuration, Fax/Modem transfer method must be the equal when fax is not configured as transport Relay");
		    DPRINTK("Current FaxTransferMethod[%d],ModemTransferMethod[%d]\n", ChannelParameters.FaxModemParameters.FaxTransferMethod, ChannelParameters.FaxModemParameters.ModemTransferMethod);
        }
        else
        {
            ChannelParameters.FaxModemParameters.FaxTransferMethod = channelParamValue;
    	    DPRINTK("Setting FaxTransferMethod to %d\n", ChannelParameters.FaxModemParameters.FaxTransferMethod);
    	}*/
        if( channelParamValue>=0 && channelParamValue<=3)
        {
            ChannelParameters.FaxModemParameters.FaxTransferMethod = channelParamValue;
    	    DPRINTK("Setting FaxTransferMethod to %d\n", ChannelParameters.FaxModemParameters.FaxTransferMethod);
    	}
        else
    	    DPRINTK("\n%s::Illegal input - the range set for FaxTransferMethod is 0-3\n", __FUNCTION__);
        break;
    case E_FaxErrorCorrectionMode:
        ChannelParameters.FaxModemParameters.FaxErrorCorrection = channelParamValue;
    	DPRINTK("Setting FaxErrorCorrection to %d\n", ChannelParameters.FaxModemParameters.FaxErrorCorrection);
        break;
    case E_EnableCNG:
        ChannelParameters.FaxModemParameters.CngRelay = channelParamValue;
    	DPRINTK("Setting CngRelay to %d\n", ChannelParameters.FaxModemParameters.CngRelay);
        break;
    case E_FaxMaxRate:
        ChannelParameters.FaxModemParameters.FaxMaxRate = channelParamValue;
    	DPRINTK("Setting FaxMaxRate to %d\n", ChannelParameters.FaxModemParameters.FaxMaxRate);
        break;
    case E_CEDTransferMode:
        if(0 == channelParamValue || 1 == channelParamValue)
        {
            ChannelParameters.FaxModemParameters.CEDTransferMode = channelParamValue;
    	    DPRINTK("Setting CEDTransferMode to %d\n", ChannelParameters.FaxModemParameters.CEDTransferMode);
    	}
        else
    	    DPRINTK("\n%s::Illegal input - the range set for CEDTransferMode is 0(by fax relay) or 1(bypass mode)\n", __FUNCTION__);
        break;
    case E_V34TransferMethod:
        if(0 == channelParamValue || 2 == channelParamValue)
        {
            ChannelParameters.FaxModemParameters.V34TransferMethod = channelParamValue;
    	    DPRINTK("Setting V34TransferMethod to %d\n", ChannelParameters.FaxModemParameters.V34TransferMethod);
    	}
        else
    	    DPRINTK("\n%s::Illegal input - the range set for V32TransferMethod is 0(Disable) or 2(Bypass)\n", __FUNCTION__);
    	break;
    case E_V32TransferMethod:
        if(0 == channelParamValue || 2 == channelParamValue)
        {
            ChannelParameters.FaxModemParameters.V32TransferMethod = channelParamValue;
    	    DPRINTK("Setting V32TransferMethod to %d\n", ChannelParameters.FaxModemParameters.V32TransferMethod);
    	}
        else
    	    DPRINTK("\n%s::Illegal input - the range set for V32TransferMethod is 0(Disable) or 2(Bypass)\n", __FUNCTION__);
    	break;
    case E_V23TransferMethod:
        if(0 == channelParamValue || 2 == channelParamValue)
        {
            ChannelParameters.FaxModemParameters.V23TransferMethod = channelParamValue;
    	    DPRINTK("Setting V23TransferMethod to %d\n", ChannelParameters.FaxModemParameters.V23TransferMethod);
    	}
        else
    	    DPRINTK("\n%s::Illegal input - the range set for V23TransferMethod is 0(Disable) or 2(Bypass)\n", __FUNCTION__);
    	break;
    case E_V22TransferMethod:
        if(0 == channelParamValue || 2 == channelParamValue)
        {
            ChannelParameters.FaxModemParameters.V22TransferMethod = channelParamValue;
    	    DPRINTK("Setting V22TransferMethod to %d\n", ChannelParameters.FaxModemParameters.V22TransferMethod);
    	}
        else
    	    DPRINTK("\n%s::Illegal input - the range set for V21TransferMethod is 0(Disable) or 2(Bypass)\n", __FUNCTION__);
    	break;
    case E_V21TransferMethod:
        if(0 == channelParamValue || 2 == channelParamValue)
        {
            ChannelParameters.FaxModemParameters.V21TransferMethod = channelParamValue;
    	    DPRINTK("Setting V21TransferMethod to %d\n", ChannelParameters.FaxModemParameters.V21TransferMethod);
    	}
        else
    	    DPRINTK("\n%s::Illegal input - the range set for V21TransferMethod is 0(Disable) or 2(Bypass)\n", __FUNCTION__);
    	break;
    case E_Bell103TransferMethod:
        if(0 == channelParamValue || 2 == channelParamValue)
        {
            ChannelParameters.FaxModemParameters.Bell103TransferMethod = channelParamValue;
    	    DPRINTK("Setting Bell103TransferMethod to %d\n", ChannelParameters.FaxModemParameters.Bell103TransferMethod);
    	}
        else
    	    DPRINTK("\n%s::Illegal input - the range set for Bell103TransferMethod is 0(Disable) or 2(Bypass)\n", __FUNCTION__);
    	break;
    case E_V34FaxTransferMethod:
        if(0 <= channelParamValue && 3 >= channelParamValue)
        {
            ChannelParameters.FaxModemParameters.V34FaxTransferMethod = channelParamValue;
    	    DPRINTK("Setting V34FaxTransferMethod to %d\n", ChannelParameters.FaxModemParameters.V34FaxTransferMethod);
    	}
        else
    	    DPRINTK("\n%s::Illegal input - the range set for V34FaxTransferMethod is 0 - 3\n", __FUNCTION__);
    	break;
    case E_BypassCoder:
    	DPRINTK("The old BypassCoder is %d", ChannelParameters.FaxModemParameters.BypassCoder);
        ChannelParameters.FaxModemParameters.BypassCoder = channelParamValue;
    	DPRINTK("Setting BypassCoder to %d\n", ChannelParameters.FaxModemParameters.BypassCoder);
        break;
    case E_BypassJitter:
        if(0 <= channelParamValue && 160 >= channelParamValue)
        {
        	DPRINTK("The old BypassJitter is %d", InitializationParams.AdvancedChannelParameters.BypassJitter);
            InitializationParams.AdvancedChannelParameters.BypassJitter = channelParamValue;
        	DPRINTK("Setting BypassJitter to %d\n", InitializationParams.AdvancedChannelParameters.BypassJitter);
    	}
    	else
    	    DPRINTK("\n%s::Illegal input - the range set for BypassJitter is 0 - 160\n", __FUNCTION__);
        break;
    case E_DataBypassOutputGain:
    	DPRINTK("The old DataBypassOutputGain is %d", InitializationParams.AdvancedChannelParameters.DataBypassOutputGain);
        InitializationParams.AdvancedChannelParameters.DataBypassOutputGain = channelParamValue;
    	DPRINTK("Setting DataBypassOutputGain to %d\n", InitializationParams.AdvancedChannelParameters.DataBypassOutputGain);
        break;
    case E_FaxBypassOutputGain:
    	DPRINTK("The old FaxBypassOutputGain is %d", InitializationParams.AdvancedChannelParameters.FaxBypassOutputGain);
        InitializationParams.AdvancedChannelParameters.FaxBypassOutputGain = channelParamValue;
    	DPRINTK("Setting FaxBypassOutputGain to %d\n", InitializationParams.AdvancedChannelParameters.FaxBypassOutputGain);
        break;
    /*    RTPStream cmds*/
    case E_Coder:
        if((3 != channelParamValue) && (4 != channelParamValue) && (5 != channelParamValue)
            && (6 != channelParamValue) && (7 != channelParamValue) && (8 != channelParamValue)
            && (25 != channelParamValue) && (26 != channelParamValue) && (27 != channelParamValue) && (56 != channelParamValue)
           )
        {
            DPRINTK("\n%s::Illegal input - the range set for Coder is (3,4,5,6,7,8,25,26,27,56)\n", __FUNCTION__);
        }
        else
        {
            ChannelParameters.RTPStreamParameters.Coder = channelParamValue;
        	DPRINTK("Setting Coder to %d\n", ChannelParameters.RTPStreamParameters.Coder);
       		ChannelParameters.RTPStreamParameters.TxVoicePayloadType = appl_code2code(coder2payload, ChannelParameters.RTPStreamParameters.Coder);
        	ChannelParameters.RTPStreamParameters.RxVoicePayloadType = appl_code2code(coder2payload, ChannelParameters.RTPStreamParameters.Coder);
        }
        break;
    case E_PTime:
        ChannelParameters.RTPStreamParameters.PTime = channelParamValue;
    	DPRINTK("Setting PTime to %d\n", ChannelParameters.RTPStreamParameters.PTime);
        break;
    case E_SilenceCompressionMode:
        if(channelParamValue>0)
    	    channelParamValue = 1;
    	ChannelParameters.RTPStreamParameters.SilenceCompressionMode = channelParamValue;
    	DPRINTK("Setting SilenceCompressionMode to %d\n", ChannelParameters.RTPStreamParameters.SilenceCompressionMode);
        break;
    case E_FaxBypassPT:
        ChannelParameters.RTPStreamParameters.FaxBypassPayloadType = channelParamValue;
    	DPRINTK("Setting FaxBypassPayloadType to %d\n", ChannelParameters.RTPStreamParameters.FaxBypassPayloadType);
        break;
    case E_ModemBypassPT:
        ChannelParameters.RTPStreamParameters.ModemByassPayloadType = channelParamValue;
    	DPRINTK("Setting ModemByassPayloadType to %d\n", ChannelParameters.RTPStreamParameters.ModemByassPayloadType);
        break;
    case E_IBSRedundancy:
        if(0 <= channelParamValue && 3 >= channelParamValue)
        {
            ChannelParameters.RTPStreamParameters.IbsRedundancyLevel = channelParamValue;
    	    DPRINTK("Setting IbsRedundancyLevel to %d\n", ChannelParameters.RTPStreamParameters.IbsRedundancyLevel);
        }
        else
    	    DPRINTK("\n%s::Illegal input - the range set for IbsRedundancyLevel is 0 - 3\n", __FUNCTION__);
    	break;
    case E_RTPRedundancy:
        ChannelParameters.RTPStreamParameters.RtpRedundancy = channelParamValue;
    	DPRINTK("Setting RtpRedundancy to %d\n", ChannelParameters.RTPStreamParameters.RtpRedundancy);
        break;
    case E_RFC2833TxPT:
        /*ChannelParameters.RTPStreamParameters.TxSignalingRfc2833PayloadType = channelParamValue;*/
    	/*DPRINTK("Setting TxSignalingRfc2833PayloadType to %d\n", ChannelParameters.RTPStreamParameters.TxSignalingRfc2833PayloadType);*/
        ChannelParameters.RTPStreamParameters.TxDTMFRelayPayloadType = channelParamValue;
    	DPRINTK("Setting TxSignalingRfc2833PayloadType to %d\n", ChannelParameters.RTPStreamParameters.TxDTMFRelayPayloadType);
        break;
    case E_RFC2833RxPT:
        /*ChannelParameters.RTPStreamParameters.RxSignalingRfc2833PayloadType = channelParamValue;*/
    	/*DPRINTK("Setting RxSignalingRfc2833PayloadType to %d\n", ChannelParameters.RTPStreamParameters.RxSignalingRfc2833PayloadType);*/
        ChannelParameters.RTPStreamParameters.RxDTMFRelayPayloadType = channelParamValue;
    	DPRINTK("Setting RxSignalingRfc2833PayloadType to %d\n", ChannelParameters.RTPStreamParameters.RxDTMFRelayPayloadType);
        break;
    case E_RFC2198PT:
        ChannelParameters.RTPStreamParameters.Rfc2198PayloadType = channelParamValue;
    	DPRINTK("Setting Rfc2198PayloadType to %d\n", ChannelParameters.RTPStreamParameters.Rfc2198PayloadType);
        break;
    case E_RTCPMeanTxInterval:
        ChannelParameters.RTPStreamParameters.RtcpMeanTxInterval = channelParamValue;
    	DPRINTK("Setting RtcpMeanTxInterval to %d\n", ChannelParameters.RTPStreamParameters.RtcpMeanTxInterval);
        break;
    case E_CName:
        {
            int iLoop = 0;
            int iLen = strlen(argv[3]);
    	    DPRINTK("Setting CName to [");
            for(iLoop=0; (iLoop<=iLen)&&(iLoop<=ACG_MAX_RTP_CANONICAL_NAME_STRING_LENGTH); iLoop++)
            {
                ChannelParameters.RTPStreamParameters.CName[iLoop] = argv[3][iLoop];
    	        printf("%c", ChannelParameters.RTPStreamParameters.CName[iLoop]);
    	    }
    	  	printf("]");
    	    ChannelParameters.RTPStreamParameters.CNameLength = iLoop-1;
    	    DPRINTK("Setting CNameLength to [%d]", ChannelParameters.RTPStreamParameters.CNameLength);
    	    DPRINTK("\n");
    	}
        break;
    case E_TxRtcpFilter:
        ChannelParameters.RTPStreamParameters.TxRtcpFilter = channelParamValue;
    	DPRINTK("Setting TxRtcpFilter to %d\n", ChannelParameters.RTPStreamParameters.TxRtcpFilter);
        break;
    case E_RxRtcpFilter:
        ChannelParameters.RTPStreamParameters.RxRtcpFilter = channelParamValue;
    	DPRINTK("Setting RxRtcpFilter to %d\n", ChannelParameters.RTPStreamParameters.RxRtcpFilter);
        break;
    /*    Miscellaneous*/
    case E_TestMode:
        ChannelParameters.MiscellaneousParameters.TestMode = channelParamValue;
    	DPRINTK("\nSetting TestMode to %d\n", ChannelParameters.MiscellaneousParameters.TestMode);
        break;
    case 0:
        break;
    default:
        printf("\n%s::Illegal input\r\n", __FUNCTION__);
		return (-1);
    }

	return 0;
}

sub_cmd_t GetChannelCmdType(int argc, char* argv[])
{
	char 		subgroupParam[MAX_ARG_LEN] = "";
	char 		channelParam[MAX_ARG_LEN] = "";
    sub_cmd_t   retCmdType = E_ERROR;
    int index = 0;
    int iListSize = sizeof(ChannelCommands)/sizeof(GroupCmdList);

	switch(argc)
	{
	    case 2: /*print some group help*/
	        strcpy(subgroupParam, argv[1]);
	        if(!strcmp(subgroupParam, "help"))
	        {
	            for(index=0; index < iListSize-1; index++)
				{
				    printf("\nUsage: SetChannelParam %s %s %s\n", ChannelCommands[index].subgroup, ChannelCommands[index].param, ChannelCommands[index].usage);
                    retCmdType = 0;
                }
            }
            else
            {
                for(index=0; index < iListSize-1; index++)
				{
				    if( !strcmp(subgroupParam, ChannelCommands[index].subgroup) )
				    {
				        printf("\nUsage: SetChannelParam %s %s %s\n", ChannelCommands[index].subgroup, ChannelCommands[index].param, ChannelCommands[index].usage);
                        retCmdType = 0;
                    }
                }
            }
	        break;
	    case 3:
		case 4:
            strcpy(subgroupParam, argv[1]);
            strcpy(channelParam, argv[2]);
            for(index=0; index < iListSize-1; index++)
            {
			    if( (!strcmp(subgroupParam, ChannelCommands[index].subgroup))
				      && (!strcmp(channelParam, ChannelCommands[index].param)) )
		        {
				    retCmdType = ChannelCommands[index].cmd_type;
			        break;
			    }
            }

            if(4 == argc)
			    break;
            else if(index < iListSize-1)
            {
                printf("\nUsage: SetChannelParam %s %s %s\n", ChannelCommands[index].subgroup, ChannelCommands[index].param, ChannelCommands[index].usage);
                retCmdType = 0;
                break;
            }
		default:
	        /*strcpy(argv[1], argv[0]);*/
		    help_cmd(2, argv);
		    break;
	}

	/*printf("\nindex[%d], retCmdType[%d]\n", index, retCmdType);*/
    return retCmdType;
}

int CreateConnection_cmd(int argc, char* argv[])
{
	conection_mode_t 	mode;
	char 				remoteAddress[ACL_ADDRESS_STR_MAX_LEN];
	int 				remotePort, ret = -1;

	switch(argc)
	{
		case 3:
				break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

	ret = isConnectionwithRemoteIP(argv[2], remoteAddress, &remotePort);

	if(ret)
	{
		if(!isValidDSPChannel(atoi(argv[1])))
		{
			printf("Invalid DSP channel %d\r\n", atoi(argv[1]));
			return (-1);
		}

		mode = CONN_MODE_REMOTE;
		setConnectionMode(atoi(argv[1]), mode);

		aclVoipRtpUtils_ChannelAddrSet(remoteAddress, remotePort, atoi(argv[1]));

		aclVoipRtpUtils_ChannelConnect(atoi(argv[1]));

		DPRINTK("Create connection between DSP channel %d to remote IP Address: %s and port %d\n", atoi(argv[1]), remoteAddress, remotePort);


	}
	else   /* Connection between two local DSP channels */
	{
		if(!isValidDSPChannel(atoi(argv[1])) && !isValidDSPChannel(atoi(argv[2])))
		{
			printf("Invalid DSP channels %d %d\r\n", atoi(argv[1]), atoi(argv[2]));
			return (-1);
		}

		mode = CONN_MODE_LOCAL;
		setConnectionMode(atoi(argv[1]), mode);

		setLocalConnectionChannelMapping(atoi(argv[1]), atoi(argv[2]));

		DPRINTK("Set local connection between DSP channel %d to %d\n", atoi(argv[1]), atoi(argv[2]));
	}

	return 0;

}

#ifdef GW_APPL_SDP
int SpeakerStatus_cmd(int argc, char* argv[])
{
    int spkStat;

    #if !defined(AC494_SDP_IPPHONE) && !defined(AC49_ORCHID_VOIP_TOOLKIT)
    /**************************************************************/
    /**************************************************************/
      printf("This command is only supported under SDP!!!\n");
      return (-1);
    /**************************************************************/
    /**************************************************************/
    #endif

    switch(argc)
    {
        case 2:
                spkStat = atoi(argv[1]);
                break;

        default:
                /*strcpy(argv[1], argv[0]);*/
                help_cmd(2, argv);
                return (-1);
                break;
    }

    if(spkStat < 0 || spkStat > 1)
    {
        printf("spkStat must be between 0 to 1\r\n");
        return -1;
    }

    if(spkStat == 1)
		acIPPSetAudioDevice(AC494_CODEC_SPEAKER);
    else
        acIPPSetAudioDevice(AC494_CODEC_HANDSET);

    if(acgUpdateChannel(0, &ChannelParameters) != ACG_SUCCESS )
    {
        printf("error - acgUpdateChannel()\r\n");
        return (-1);
    }
    return 0;
}
#endif
int RunScript_cmd(int argc, char* argv[])
{
	char		*pConfigFileName;
	pConfigFileName = AC_ZALLOC_L(CONFIGURATION_FILE_NAME_SIZE);

	switch(argc)
	{
		case 2:
				strcpy(pConfigFileName,argv[1]);
				break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

	if(cfg_util_load_file(scriptBuf, sizeof(scriptBuf), pConfigFileName) != 0)
	{
		printf("ERROR: Failed to read %s File\n", pConfigFileName);
		return (-1);
	}

	if(pConfigFileName)
		nfree(pConfigFileName);

	if(voip_conf_util_load_cfg_file(scriptBuf) == -1)
	{
		printf("error - voip_conf_util_load_cfg_file()\r\n");
		return (-1);
	}

	return 0;

}

int acgStartFax_cmd(int argc, char* argv[])
{
	int Channel = -1;

	switch(argc)
	{
		case 2:
				Channel = atoi(argv[1]);
				break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

    if(acgUpdateChannel(Channel, &ChannelParameters) != ACG_SUCCESS )
    {
        printf("error - acgUpdateChannel()\r\n");
        return (-1);
    }

	setT38(Channel);
	acgStartFax(Channel, ACG_FAX_MODEM_TRANSPORT__RELAY);
	return 0;

}

int acgStopFax_cmd(int argc, char* argv[])
{
	int Channel = -1;

	switch(argc)
	{
		case 2:
				Channel = atoi(argv[1]);
				break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

    /*acgStopFax(Channel, ACG_FAX_MODEM_TRANSPORT__RELAY);*/
    unsetT38(Channel);

	return 0;

}

int acgSetRegularRTCPParameters_cmd(int argc, char* argv[])
{
    int dspChannel = -1;
    acgTRegularRTCPParamters RegularRTCPParameters;

    switch(argc)
    {
        case 4:
                dspChannel = atoi(argv[1]);
                break;

        default:
                /*strcpy(argv[1], argv[0]);*/
                help_cmd(2, argv);
                return (-1);
                break;
    }

    if(isValidDSPChannel(dspChannel))
    {
        RegularRTCPParameters.RTCPParamIndex = (acgERTCPRegularParameter)atoi(argv[2]);
        RegularRTCPParameters.RTCPParamValue = (int)atoi(argv[3]);

        /* set regular rtcp parameters */
        if(acgSetRegularRTCPParameters(dspChannel,&RegularRTCPParameters ) != ACG_SUCCESS )
        {
            printf("error - acgSetRegularRTCPParameters()\r\n");
            return (-1);
        }
    }
    else
    {
        printf("Invalid DSP channel %d\r\n", dspChannel);
        return (-1);
    }

    DPRINTK("set regular rtcp parameters on channel %d\n", dspChannel);

    return 0;

}

int acgGetRegularRTCPParameters_cmd(int argc, char* argv[])
{
    int dspChannel = -1;

    switch(argc)
    {
        case 2:
                dspChannel = atoi(argv[1]);
                break;

        default:
                /*strcpy(argv[1], argv[0]);*/
                help_cmd(2, argv);
                return (-1);
                break;
    }

    if(isValidDSPChannel(dspChannel))
    {
        /* set regular rtcp parameters */
        if(acgGetRegularRTCPParameters(dspChannel ) != ACG_SUCCESS )
        {
            printf("error - acgSetRegularRTCPParameters()\r\n");
            return (-1);
        }
    }
    else
    {
        printf("Invalid DSP channel %d\r\n", dspChannel);
        return (-1);
    }

    DPRINTK("get regular rtcp parameters on channel %d\n", dspChannel);

    return 0;

}

int acgSetNtpTimeStamp_cmd(int argc, char* argv[])
{
    int dspChannel = -1;
    acgTNtpTimeStamp NtpTimeStamp;

    switch(argc)
    {
        case 4:
                dspChannel = atoi(argv[1]);
                break;

        default:
                /*strcpy(argv[1], argv[0]);*/
                help_cmd(2, argv);
                return (-1);
                break;
    }

    if(isValidDSPChannel(dspChannel))
    {
        NtpTimeStamp.TimeX1Sec= (int)atoi(argv[2]);
        NtpTimeStamp.TimeX1SecFraction = (int)atoi(argv[3]);

        if(acgSetNtpTimeStamp(dspChannel,&NtpTimeStamp ) != ACG_SUCCESS )
        {
            printf("error - acgSetNtpTimeStamp()\r\n");
            return (-1);
        }
    }
    else
    {
        printf("Invalid DSP channel %d\r\n", dspChannel);
        return (-1);
    }

    DPRINTK("set ntp time stamp on channel %d time in seconds %d time in fractions of seconds %d\n", dspChannel,NtpTimeStamp.TimeX1Sec,NtpTimeStamp.TimeX1SecFraction);

    return 0;
 }

int acgGetVersionInfo_cmd(int argc, char* argv[])
{

	char FirmwareVersion[256] = {0};
	acgTGetVersionInfo VersionInfo;

	VersionInfo.FirmwareVersion = FirmwareVersion;
	switch(argc)
	{
		case 1:
				break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

	if(acgGetVersionInfo(&VersionInfo) == ACG_SUCCESS)
	{
		printf("Dsp firmware info:%s\n", VersionInfo.FirmwareVersion);
	}
	return 0;
}

int acgPlayback_cmd(int argc, char* argv[])
{
    int dspChannel = -1;
    int pbType = 0;
	acgECoder	Coder;
	char strFileName[128]={0};

	switch(argc)
	{
		case 5:
                dspChannel = atoi(argv[1]);
                pbType = (int)atoi(argv[2]);
                Coder = (int)atoi(argv[3]);
                strcpy(strFileName, argv[4]);
                if((ACG_CODER__G711ALAW == Coder) || (ACG_CODER__G711MULAW == Coder)
                    || (ACG_CODER__G726_16 == Coder) || (ACG_CODER__G726_24 == Coder)
                    || (ACG_CODER__G726_32 == Coder) || (ACG_CODER__G726_40 == Coder)
                    || (ACG_CODER__G723LOW == Coder) || (ACG_CODER__G723HIGH == Coder)
					|| (ACG_CODER__G729 == Coder) || (ACG_CODER__LINEAR_16_BIT_PCM_SAMPLE_RATE_16KHZ == Coder)
                    )
                	break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

    if(isValidDSPChannel(dspChannel))
    {
        if(acgPlaybackStart(dspChannel, pbType, Coder, strFileName) != ACG_SUCCESS )
        {
            printf("error - acgPlaybackStart()\r\n");
            return (-1);
        }
    }
    else
    {
        printf("Invalid DSP channel %d\r\n", dspChannel);
        return (-1);
    }

    /*DPRINTK("play back (%s) on channel %d Coder = %d pWaveFile = %s\n", (0==pbType)?"TDM":"Network", dspChannel, Coder, strFileName);*/

	return 0;
}

int acgPlaybackEnd_cmd(int argc, char* argv[])
{
    int dspChannel = -1;
    int ret = 0;

	switch(argc)
	{
		case 2:
                dspChannel = atoi(argv[1]);
				break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

    ret = acgPlaybackEnd(dspChannel);

    printf("play back end on channel %d:[%d]\r\n", dspChannel, ret);

	return 0;
}

int RecordingConfig_cmd(int argc, char* argv[])
{
	int port;
	int ip[4];

	if(!applicationStatusParams.is_default_init_device_parameters)
	{
		printf("\nacgGetDefaultInitDeviceParameters was not called!!!\r\n");
		return (-1);
	}

	switch(argc)
	{
		case 3:
				break;

		default:
				/*strcpy(argv[1], argv[0]);*/
				help_cmd(2, argv);
				return (-1);
				break;
	}

	/* Setting a fake remote IP Address and port for packet recording mechanism */
	if(4==sscanf(argv[1], "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]))
	{
		sprintf(InitializationParams.RemoteRecordingIpAddress, "%d.%d.%d.%d",
								(0xff&ip[0]), (0xff&ip[1]), (0xff&ip[2]), (0xff&ip[3]));
	}

	if(1==sscanf(argv[2], "%d", &port))
	{
		InitializationParams.RemoteRecordingPort = port;
	}

	printf("Recording ip address is %s, port is %d\n",
								InitializationParams.RemoteRecordingIpAddress,
								InitializationParams.RemoteRecordingPort);
	return 0;
}

int start_paging_Cmd(int argc, char* argv[])
{

    // acl_init_paging();
}


int main(int argc, char* argv[])
{
	int i;

	memset(&applicationStatusParams, 0, sizeof(applicationStatusParams));
	memset(&ChannelInformation, 0, sizeof(ChannelInformation));

	for(i=0; i<(ACG_NUMBER_OF_DSP_CHANNEL); i++)
	{
		ChannelInformation[i].fxs_line = -1;
	}
#if defined(B_CEVA_PLATFORM)
	system("insmod /gw_api/dsp/init.ko");
	system("/gw_api/scripts/ceva_dsp_load.sh");
	system("/gw_api/scripts/le88drv_load.sh");
#endif

#if defined(S_PLATFORM)
	system("insmod /tmp/init.ko");
	system("insmod /tmp/ac49x_dsp.ko");
#endif

	initMediaEvents();

	aclVoipRtpUtils_resetNetworkingDBs();

    if (aclVoipRtpUtils_createSockets() == -1)
    {
		printf("error - aclVoipRtpUtils_createSockets()\r\n");
      		/*problem - print some error and probably return*/
    }

	/* create the rtp/rtcp rx thread */
	gw_api_createRtpRxThread();
	gw_api_createRtcpRxThread();

	if(argc==1)
	{
		RunCLI();
	}
/*	else
		ExecuteCommand(argc-1, argv+1);
*/
	return 0;
}




/******************************************************************************/
