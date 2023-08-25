#ifndef _IPHONE_PAGING_PUB_H_
#define _IPHONE_PAGING_PUB_H_

#define NUM_OF_PAGING 1
#define PAGING_IP_LENGTH 16
#define MAX_PAGING_STREAM 10

/* this is for outside config define, define it as your wish.
   save the config yourself and define the function:
   paging_get_config()
   to return it for thread use.
*/
typedef enum
{
    E_PAGING_OUTGOING_LINE,
    E_PAGING_OUTGOING_START,
    E_PAGING_INCOMING_START,
    E_PAGING_INCOMING_TIMEOUT,
    E_PAGING_INCOMING_IGNORE,
    E_PAGING_INCOMING_SWITCH,
    E_PAGING_END
}PAGING_EVENT;


extern void acl_init_paging();
extern void paging_send_packet(char* buff, unsigned int len,int channel);
extern int paging_is_paging_now();

#endif
