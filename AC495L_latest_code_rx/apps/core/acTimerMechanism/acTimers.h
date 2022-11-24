/*     TIMER acTimers.h */
/*===============================================================*/

#ifndef	__AC_TIMERS
#define	__AC_TIMERS

#if defined(B_CEVA_PLATFORM)
#define TIMER_SUN_PATH  "/audiocodes/unix_domain_sockets_entries/"
#else
#define TIMER_SUN_PATH  "/tmp/"
#endif

#define TIMER_MAGIC   0x12345678

#define TIMER_2_TASK_LEVEL(Timer) 1


typedef void (*TypeExpire)(void *data);

//TIMER_ONE_TIME and TIMER_PERIODIC_ROUNDED not supported
//TIMER_PERIODIC_ROUNDED not supported because the LIst don't support re-insert with out new allocation
typedef enum{TIMER_THROWAWAY,TIMER_ONE_TIME,TIMER_PERIODIC,TIMER_PERIODIC_ROUNDED} TimerType; 


enum TimerErrorCodes{
					TIMER_OK = 0,
                     			TIMER_ERROR_NO_FREE = 1,
					 /*					 TIMER_ERROR_PERIOD_TOO_BIG = 2,*/
					 TIMER_ERROR_START_FREE_TIMER = 3,
					 TIMER_ERROR_STOP_FREE_TIMER = 4,
					 TIMER_ERROR_STOP_NOT_ACTIVE = 5,
					 TIMER_ERROR_DELETE_FREE_TIMER = 6,
					 TIMER_ERROR_CONFIG_FREE_TIMER = 7,
					 TIMER_ILLEGAL_TID = 8,
					 TIMER_ILLEGAL_PARAM = 9,
					 TIMER_NOT_INIT_YET = 10,
					 TIMER_ERROR_QUERY_NOT_ACTIVE = 11,
					 TIMER_ERROR_CONFIG_RUN_TIMER = 12,
                			TIMER_ERROR_GENERAL_ERR = 13
};


typedef struct{
  TimerType         	Type;
  long     		   	Period; /*[msec]*/  /* or RoundedPeriod */
  TypeExpire        	ExpireFunction;
  void			*data;
}TimerUserParams;


typedef struct {
  unsigned long     	TimerMagic;
  unsigned long     	ExpireTime;
  TimerUserParams   UserParams;
} TimerRecord;


#endif /*__AC_TIMERS*/
