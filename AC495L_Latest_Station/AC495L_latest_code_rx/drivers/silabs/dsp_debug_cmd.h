#ifndef DSP_DEBUG_CMD_H
#define DSP_DEBUG_CMD_H

#if __cplusplus
extern "C" {
#endif

//define here
//#define GPIO_PIN  1
//#define GPIO_OUTPUT_PIN 0
//#define GPIO_INPUT_PIN 1

//nirp dsp cli

typedef enum {
    DSP_DEBUG_CMD_LE88_REG_DUMP,

    DSP_DEBUG_CMD_MPI_READ,

    DSP_DEBUG_CMD_MPI_WRITE,

    DSP_DEBUG_CMD_SETUP_DEVICE,

    DSP_DEBUG_CMD_OPEN_CHANNEL,

    DSP_DEBUG_CMD_CLOSE_CHANNEL,

    DSP_DEBUG_CMD_SET_CODECS,

    DSP_DEBUG_CMD_TONE,

    DSP_DEBUG_CMD_BIT,

    DSP_DEBUG_CMD_PACKET_RECORDING,

DSP_DEBUG_CMD_LAST,

} enum_dsp_debug_cmd_t;

typedef struct 
{
	enum_dsp_debug_cmd_t dsp_debug_cmd;
    int	cmd_len;
    unsigned char param[50];
} voip_dsp_debug_cmd_t;

#if __cplusplus
}
#endif

#endif //DSP_DEBUG_CMD_H
