/* these commands decide which register is written to/read from*/
#define GPIO_EXPANDER_CMD_INPUT0		0x00
#define GPIO_EXPANDER_CMD_INPUT1		0x01
#define GPIO_EXPANDER_CMD_OUTPUT0		0x02
#define GPIO_EXPANDER_CMD_OUTPUT1		0x03
#define GPIO_EXPANDER_CMD_POLARITY_INVERSION_0	0x04
#define GPIO_EXPANDER_CMD_POLARITY_INVERSION_1	0x05
#define GPIO_EXPANDER_CMD_CONFIG_0		0x06
#define GPIO_EXPANDER_CMD_CONFIG_1		0x07
/*i2c address*/
#define GPIO_EXPANDER_ADDRESS_L		0x20
#define GPIO_EXPANDER_ADDRESS_U		0x21

#define GPIO_EXPANDER_DEFAULT_CONFIG_0 0x00 /*default config. sets pins 0-7
					      as outputs*/
#define GPIO_EXPANDER_DEFAULT_CONFIG_1 0x18 /* default config. sets pins
					       10-12,15-17 as outputs,
					       13,14 as inputs*/
#define GPIO_EXPANDER_DEFAULT_OUTPUT_0 0xF9 /* output default 1, input default 0
					      */
#define GPIO_EXPANDER_DEFAULT_OUTPUT_1 0xFF

/*first reg pins*/
#define EXP2_GPIO1_NUM 0
#define USB1_COP_EN_NUM 1
#define USB2_COP_EN_NUM 2
#define EXP1_GPIO1_NUM 3
#define LED_SPEAKER_NUM 4
#define LED_HEADSET_NUM 5
#define LED_VOICEMAIL_NUM 6
#define LED_MUTE_NUM 7
/*second reg pins*/
#define EXP3_GPIO1_NUM 10
#define BT_RST_N_NUM 11
#define EXP2_EXT_NUM 13
#define EXP3_EXT_NUM 14
#define EXP1_RST_NUM 15
#define EXP2_RST_NUM 16
#define EXP3_RST_NUM 17

