#ifndef _LCD_MESSAGES_H
#define _LCD_MESSAGES_H



/*#define SEVENTEEN     17
#define MAX_BUFFER_SIZE    256
*/





char registering_msg[SEVENTEEN]             = {"REGISTERING"};
char global_call_msg[SEVENTEEN]             = {"GLOBAL CALL "};
char intercom_msg[SEVENTEEN]                = {"INTERCOM "};
char negative_response_msg[SEVENTEEN]       = {"TRY AGAIN"};
char global_alm_msg[SEVENTEEN]              = {"GLOBAL ALM"};
char page_reset_msg[SEVENTEEN]              = {"Resetting "};
char route_add_msg[MAX_BUFFER_SIZE]         = {"route add -net %s netmask 255.255.255.255 eth0"};
char route_del_msg[MAX_BUFFER_SIZE]         = {"route del -net %s netmask 255.255.255.255 eth0"};

char configuring_msg[SEVENTEEN]             = {"CONFIGURING      "};
char P2P_msg[SEVENTEEN]                     = {"P2P:%s           "};
char CONF_msg[SEVENTEEN]                    = {"CONF:%s          "};
char CONFERENCE_msg[SEVENTEEN]              = {"CONF             "};
char CONFERENCE_full[SEVENTEEN]             = {"CONF FULL        "};
char GRP_CL_msg[SEVENTEEN]                  = {"GP%d FR%d        "};
char IDLE_msg[SEVENTEEN]                    = {"STN_NO:%d  IDLE  "};
char Channel_msg[SEVENTEEN]                 = {"STN_NO:%d CH:%d  "};
char callterminated_msg[SEVENTEEN]          = {"CALL TERMINATED  "};
//char global_call_msg[SEVENTEEN]             = {"GLB FR %d        "};
//char global_alm_msg[SEVENTEEN]              = {"GB AL %d         "};
char global_terminate[SEVENTEEN]            = {"GLB TERMINATED   "};
char group_msg[SEVENTEEN]                   = {"GP%d FR%d        "};
char zone_terminate[SEVENTEEN]              = {"ZONE TERMINATED  "};
char zone_call_msg[SEVENTEEN]               = {"ZN FR %d         "};
char zone_alm_msg[SEVENTEEN]                = {"ZN AL:%d         "};
//char intercom_msg[SEVENTEEN]                = {"Channel:%d       "};
char pttkey_msg[SEVENTEEN]                  = {"PTT KEY PRESSED  "};
//char negative_response_msg[SEVENTEEN]       = {"TRY AGAIN        "};
char icom_response_msg[SEVENTEEN]           = {"INVALID CHANNEL  "};

char reset_msg[SEVENTEEN]                   = {"Resetting        "};
char STATION_busy[SEVENTEEN]                = {"STATION BUSY       "};
//char ZONE_NUMBR[SEVENTEEN]                  = {"ZONE NO:%d         "};
//char route_add_msg[MAX_BUFFER_SIZE]         = {"route add -net %s netmask 255.255.255.255 eth0"};
//char route_del_msg[MAX_BUFFER_SIZE]         = {"route del -net %s netmask 255.255.255.255 eth0"};















#endif
