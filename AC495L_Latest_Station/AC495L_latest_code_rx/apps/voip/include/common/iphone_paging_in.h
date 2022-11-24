#ifndef _IPHONE_PAGING_IN_H_
#define _IPHONE_PAGING_IN_H_

#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "acIPPhoneAPI.h"
#include "acl_dsp_telephony_commands.h"
#include "acl_call_mngr.h"
//#include "ac_struct.h"
#include "voip_utils.h"
//#include "iphone_localNetTerm.h"
//#include "Pbx_struct.h"
#include "acl_log.h"
#include "voip_media_handler.h"
#include "iphone_paging_pub.h"

#define PAGING_BUFF_LEN 1024

#define PAGING_END_TIMEOUT	1500  // the time we find stream lost.
#define PAGING_START_TIMEOUT	50  // every time we wait and check all call is disconnect or not when try to paging outgoing.
#define PAGING_PTIME        20

#define UNUSED(a) a=a

typedef struct
{
      unsigned char V:2;
	  unsigned char P:1;
	  unsigned char X:1;
	  unsigned char CC:4;
	  unsigned char PT:7;
	  unsigned char M:1;
	  unsigned short SN;
	  unsigned int TimeStamp;
	  unsigned int SSRC;
} RTPHeader;

typedef struct
{
   unsigned int bits;
   unsigned int ssrc;
} RTCPHeader;

typedef enum{
	PAGE_IDLE,
	PRIVATE_IDLE,
	PRIVATE_INCOMING_WAITING,
	PAGE_INCOMING_ONGOING,
	PRIVATE_INCOMING_ONGOING,
	PRIVATE_OUTGOING_ONGOING,
	PAGE_OUTGOING_ONGOING,
	P_STATE_MAX
}PAGINGSTATE;

typedef struct
{
    char    enable;
    char    address[PAGING_IP_LENGTH];
    int     port;
}PAGING_GROUP;

typedef struct
{
    char    enable;
    int     payload;
    PAGING_GROUP group[NUM_OF_PAGING];
}PAGING_CONFIG;

typedef struct
{
    unsigned int     SSRC;
    unsigned int     group;
    unsigned int     count;
    unsigned int     timer_record_count;
    char             ignore;
}PAGING_STREAM;

typedef struct
{
    fd_set           pagingRtpReceivingFds;
    int              pagingRtpMaxFd;
    int              pagingFdDB[110]; //RTP/RTCP
    int              pagingFdDBRealSize;
    char	         pagingBuff[PAGING_BUFF_LEN];
    pthread_t        pagingTid;

    unsigned int     currentSSRC;
    unsigned int     currentGID;
    PAGINGSTATE      state;
    PAGING_STREAM    stream[MAX_PAGING_STREAM];
}PAGING_WORKING;
PAGING_WORKING   pWork_m;



/* function internal */
extern void* paging_get_config();
extern void select_codec(int ch,int codec);
extern void paging_received_packet();
extern void paging_state_handle(PAGINGSTATE state);
extern void paging_outgoing_wait_timer(void *data);
extern void paging_stream_monitor_timer(void *data);
//extern void paging_event_received(PAGING_EVENT event,void* data);

/* function external */

extern void dsp_voice_stop(int n, int ch);
extern int acl_is_in_active_call();
extern void acl_disconnect_all_calls();

#endif
