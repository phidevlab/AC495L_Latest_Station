/********************************************************************
 * AudioCodes improved application utils                            *
 *                                                                  *
 * File Name:	voip_utils.c                                        *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 26/02/08 - Coding started.                                       *
 *																	*
 ********************************************************************/
#ifdef CONFIG_RG_VOIP_RV_MGCP
#include <ctype.h>
#endif

#include <sched.h> 

#include "voip_utils.h"
#include "voip_rtp_utils.h"
#include "acTimersAPI.h"


/*#define AC_VOIP_UTILS_DEBUG*/

#ifdef AC_VOIP_UTILS_DEBUG
//#define DBG_PRINT(a1,a2...)	IppLogMessage(RV_FALSE, "[%s:%4d] " a1,__FUNCTION__, __LINE__, ##a2)
#define DBG_PRINT(a1,a2...)		printf( "[%s:%4d] " a1,__FUNCTION__, __LINE__, ##a2)
#else
#define DBG_PRINT(a1,a2...)
#endif

/* ER08 - Added macro for error messages (not regular debug messages) that shouls go into the RV logs */
#define DBG_PRINT_ERR(a1,a2...)	IppLogMessage(RV_TRUE, "[%s:%4d] " a1,__FUNCTION__, __LINE__, ##a2)


static timerParams2Id_t 		TimerParams2Id[ACL_MAX_NUMBER_OF_TIMERS];
static sem_t				TimerSem;

lineAndVoiceChannel 		LineAndVoiceChannel2DspChannelConversionTable[] = 
{

/* 
DSP channel  		Line  	Voice channel
-------------------------------------
*/

#if defined(AC488_CPE_VOIP_TOOLKIT)

/*     0      */    	{0,			0},
/*     1      */		{1,			0},					
/*     2      */		{-1,        -1},
/*     3      */		{-1,		-1},
#elif defined(CONFIG_RG_IPPHONE)
/*     0      */    	{0,			0},
/*     1      */		{0,			1},		

#else

/*     0      */    	{0,			0},
/*     1      */		{1,			0},					
/*     2      */		{0,         1},
/*     3      */		{1,			1},

/*     4      */		{2,         		0},
/*     5      */		{3,			0},
/*     6      */		{2,         		1},
/*     7      */		{3,			1},

/*     8      */		{4,         		0},
/*     9      */		{5,			0},
/*     10     */		{4,        		1},
/*     11     */		{5,			1},

/*     12     */		{6,        		0},
/*     13     */		{7,		 	0},
/*     14     */		{6,        	 	1},
/*     15     */		{7,			1},

#endif
};

/** Prototypes **/
static int FindTimerIdEntry(etimer_func_t function, void *data);
static void DeleteTimerParamsEntry(int id);
static void General_Callback_Function(void *data);


int AC_TIMER_INIT()
{
	int i;

   	DBG_PRINT(">>\r\n");

	if(AcTimerInit() <0 )
	{
		printf("AcTimerInit failed");
		return (-1);
	}

	if(sem_init(&TimerSem, 0, 1) < 0)
	{
		printf("sem_init failed::TimerSem\r\n");
		return (-1);
	}

	for(i=0; i<ACL_MAX_NUMBER_OF_TIMERS; i++)
	{
		TimerParams2Id[i].func = NULL;
		TimerParams2Id[i].data = NULL;
		TimerParams2Id[i].id = -1;
	}

	DBG_PRINT("<<\r\n");

	return 0;
}

int AC_TIMER_DESTROY()
{
	int i;
	
   	DBG_PRINT(">>\r\n");
		
	if(AcTimerDestroy() != TIMER_OK)
        	printf("\nAcTimerDestroy failed\r\n");

	if(sem_destroy(&TimerSem) < 0)
	{
		perror("sem_destroy::TimerSem\r\n");
	}

	for(i=0; i<ACL_MAX_NUMBER_OF_TIMERS; i++)
	{
		TimerParams2Id[i].func = NULL;
		TimerParams2Id[i].data = NULL;
		TimerParams2Id[i].id = -1;
	}

	DBG_PRINT("<<\r\n");

	return 0;
}

void AC_TIMER_SET(long duration, etimer_func_t function, void *data)
{
	TimerUserParams 	userParams;
	int				timerId;
	int				BufferIndex;

   	DBG_PRINT(">>\r\n");

    	DBG_PRINT("\nduration = %d   function = 0x%08x   data = 0x%08x\n\r", duration, function, data);
	
	/* In case timer was set with duration 0 invoke the callback function immediately and return */
	if(duration == 0)
	{
//		duration = 70;
//		if(sched_yield() < 0)
//			printf("\nsched_yield failed\r\n");
			
		(*function)(data);

		return;
	}

   	sem_wait(&TimerSem); 

	for(BufferIndex=0; BufferIndex<ACL_MAX_NUMBER_OF_TIMERS; BufferIndex++)
	{
		if(TimerParams2Id[BufferIndex].id == (-1)){
   			DBG_PRINT("TimerParams2Id[%d].id = %d\r\n", BufferIndex, TimerParams2Id[BufferIndex].id);
			break;
		}
	}

	if(BufferIndex >= ACL_MAX_NUMBER_OF_TIMERS)
	{
		printf("\nToo many timers were defined %d\r\n", BufferIndex);
		sem_post(&TimerSem); 
		return;		
	}

	TimerParams2Id[BufferIndex].data = data;
	TimerParams2Id[BufferIndex].func = function;

	userParams.ExpireFunction = General_Callback_Function;
	userParams.data = (void *)&(TimerParams2Id[BufferIndex]);
	userParams.Period = duration;
	userParams.Type = TIMER_THROWAWAY;

	if(AcArmTimer(&userParams, &timerId) != TIMER_OK)
	{
        	printf("\nAcArmTimer failed\r\n");
		sem_post(&TimerSem); 
		return;
	}

	TimerParams2Id[BufferIndex].id = timerId;

	sem_post(&TimerSem); 

	DBG_PRINT("<<\r\n");

}

int AC_TIMER_EXISTS(etimer_func_t function, void *data)
{
	int retValue = -1;

   	DBG_PRINT(">>\r\n");

	retValue = FindTimerIdEntry(function, data);

   	DBG_PRINT("retValue = %d\r\n", retValue);

	if(retValue == -1)
		return 0;
	else
		return 1;

}

void AC_TIMER_DEL(etimer_func_t function, void *data)	
{
	int timerIdEntry;

   	DBG_PRINT(">>\r\n");

	timerIdEntry = FindTimerIdEntry(function, data);
		
	if(timerIdEntry != -1)
	{
		if(AcDeleteTimer(TimerParams2Id[timerIdEntry].id) != TIMER_OK)
        		printf("\nAcDeleteTimer failed   id = %d\r\n", TimerParams2Id[timerIdEntry].id);

		DeleteTimerParamsEntry(timerIdEntry);
	}

	DBG_PRINT("<<\r\n");

}

void AC_IPC_TIMER_SET(long duration, etimer_func_t function, void *data)
{
	AC_TIMER_SET(duration, function, data);
}

int AC_IPC_TIMER_EXISTS(etimer_func_t function, void *data)
{
	return(AC_TIMER_EXISTS(function, data));
}

void AC_IPC_TIMER_DEL(etimer_func_t function, void *data)
{
	AC_TIMER_DEL(function, data);
}

static int FindTimerIdEntry(etimer_func_t function, void *data)
{
	int i, retValue = -1;
	
   	DBG_PRINT(">>\r\n");

	sem_wait(&TimerSem); 

	for(i=0; i<ACL_MAX_NUMBER_OF_TIMERS; i++)
	{
		if((TimerParams2Id[i].func == function) && (TimerParams2Id[i].data == data))
		{
    			DBG_PRINT("i = %d\n\r", i);
			retValue = i;
			break;
		}
	}

	sem_post(&TimerSem); 

	DBG_PRINT("<<\r\n");

	return (retValue);
	
}

static void DeleteTimerParamsEntry(int id)
{	
	TimerParams2Id[id].func = NULL;
	TimerParams2Id[id].data = NULL;
	TimerParams2Id[id].id = -1;
}

static void General_Callback_Function(void *data)
{
	int timerIdEntry;
	
	timerParams2Id_t 	*pTimerParams2Id;
	timerParams2Id_t	Tp;

   	DBG_PRINT(">>\r\n");

	pTimerParams2Id = (timerParams2Id_t *)data;
	memset(&Tp, 0, sizeof(Tp));
	memcpy(&Tp, pTimerParams2Id, sizeof(Tp));

	timerIdEntry = FindTimerIdEntry(pTimerParams2Id->func, pTimerParams2Id->data);
		
	if(timerIdEntry != -1)
	{
		DeleteTimerParamsEntry(timerIdEntry);
	}
	else
	{
		printf("timerIdEntry %d\n", timerIdEntry);
	}
	/* invoke the callback function */
	(*Tp.func)(Tp.data);


	DBG_PRINT("<<\r\n");

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

void *AC_ZALLOC_E(size_t size)
{
	return(AC_ZALLOC_L(size));
}

char *AC_STRNCPY(char *dst, const char *src, size_t len)
{
	strncpy(dst, src, len);
	dst[len] = 0;
	return dst;
}

 
int appl_code2code(appl_code2code_t *list, int code)
{
	for (; list->code != -1 && list->code != code; list++);

	return list->val;
}

char *code2str_ex(code2str_ex_t *list, int code)
{
	int new_code = -1;

	for (; list->code != -1 && list->code != code; list++)
	{
		new_code = payload_type_enum_to_id(list->code);
		if(new_code != -1)
		{
			if(new_code == code)	/* For dynamic payload type */
				break;
		}
	}

	return list->str_list ? *(list->str_list) : NULL;
}

int str2code_ex(code2str_ex_t *list, char *str)
{
	char **str_ix;
	int 	new_code = -1;

	for (; list->code != -1; list++)
	{
		for (str_ix = list->str_list; *str_ix; str_ix++)
		{
			if (!strcmp(*str_ix, str))
			{
				new_code = payload_type_enum_to_id(list->code);
				if(new_code != -1)
					return new_code;  /* For dynamic payload type */
				else
					return list->code;
			}
		}
	}

	return -1;
}

char *code2str(code2str_t *list, int code)
{
	for (; list->code != -1 && list->code != code; list++);

	return list->lang ? *list->lang : list->str;
}

int str2code(code2str_t *list, char *str)
{
    for (; list->code != -1 && list->str && strcmp(list->str, str); list++);

    return list->code;
}

char *itoa(int num)
{
	static char ret[16];

	*ret = 0;
	snprintf(ret, sizeof(ret), "%d", num);
	return ret;
}

int getDspChannelByLineAndVoiceChannel(int line, int VoiceChannel)
{
	if ((0 > line) || (CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES < line))
		return -1;
	if ((0 > VoiceChannel) || (1 < VoiceChannel))
		return -1;

#ifdef CONFIG_RG_IPPHONE
	return VoiceChannel;
#else
	return getDspChannelByLineAndVoiceCh(line, VoiceChannel);
#endif
}


char *strdup_e(char *s)
{
    return strdup(s);
}

#ifdef CONFIG_RG_VOIP_RV_MGCP

static char **str_alloc(char **s)
{
    nfree(*s);
    *s = strdup("");
    return s;
}

char **str_tolower(char **s)
{
    char *p;
    
    if (!*s)
	return str_alloc(s);
    for (p = *s; *p; p++)
        *p = tolower(*p);
    return s;
}

#endif /* CONFIG_RG_VOIP_RV_MGCP */

