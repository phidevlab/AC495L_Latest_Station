#ifndef __W682388CMD_H__
#define __W682388CMD_H__

#define W682388_DEV 		"/dev/w682388/0"

#define W682388_OK_E			0

#define CH_0	0
#define CH_1	1

#define NUMBER_OF_FXS	2

#ifndef OK
#define OK		0
#endif	//OK

#ifndef ERROR
#define ERROR	-1
#endif	//ERROR


#define FLASH_COUNTER_MILISECONDS  	2000

/********************************************************************************
* Command
********************************************************************************/
typedef enum {
	//write cmd
	W682388_INIT_CMD=0,
	W682388_INIT_LINE_CMD,
	W682388_SET_CODEC_CMD,
	W682388_SET_TIME_SLOT_CMD,
	W682388_SET_LINE_STATE_CMD,
	W682388_SET_REG_CMD,
    W682388_SET_COUNTRY_CMD,
	//read cmd
	W682388_GET_HOOK_STATE_CMD,
    W682388_GET_REG_CMD,
	
} W682388_DRIVER_COMMAND_ENT;

/********************************************************************************
* Write Structure
********************************************************************************/
typedef struct {
	unsigned int 					command;
} W682388_INIT_STCT;

typedef struct {
	unsigned int 					command;
	int 							channel;
} W682388_INIT_LINE_STCT;

typedef struct {
	unsigned int 					command;
	int 							channel;
	int								codec;
} W682388_CODEC_STCT;

typedef struct {
	unsigned int 					command;
	int 							channel;
	int								txSlot;
	int								rxSlot;
} W682388_TIME_SLOT_STCT;

typedef struct {
	unsigned int 					command;
	int 							channel;
	int								state;
} W682388_LINE_STATE_STCT;

typedef struct {
	unsigned int 	command;
    int             address;
    int             data;
} W682388_SET_REG_STCT;

typedef struct {
	unsigned int 	command;
    int             country;
} W682388_SET_COUNTRY_STCT;

/********************************************************************************
* Read Structure
********************************************************************************/
typedef struct {
	unsigned int 					command;
	int 							channel;
	int								state;
} W682388_HOOK_STATE_STCT;

typedef struct {
	unsigned int 	command;
    int             address;
    int             data;
} W682388_GET_REG_STCT;

#endif //__W682388CMD_H__
