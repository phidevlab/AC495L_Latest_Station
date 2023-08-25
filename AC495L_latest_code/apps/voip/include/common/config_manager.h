
#ifndef _CONFIG_MANAGER_H
#define _CONFIG_MANAGER_H


#include "voip_main.h"

#define CONFIG_FILE_1 		"/audc/voip/stn_cfg.txt"
#define CONFIG_FILE_2 		"/audc/voip/stn_cfg_bc.txt"


#define FL_RD			    "r"
#define FL_WR			    "w+"
#define FILESYSTEM_RW_CMD 	"mount -o remount,rw /"
#define FILESYSTEM_RO_CMD 	"mount -o remount,ro /"


#define PJS_PATH		    "/home/root/vccs/bin/pjsua-arm-arago-linux-gnueabi"

#define FCS_ID_STRING		"FCS ID - "
#define NW_TYPE_STRING		"NETWORK TYPE - "
#define NW_MAC_STRING		"MAC - "
#define NW_IP_STRING		"IP - "
#define NW_SN_STRING		"gc_subnetmask - "
#define NW_GW_STRING		"GATEWAY - "

#define SET_INTERFACE_ETH0_DOWN		"ifconfig eth0 down"
#define SET_INTERFACE_MAC			"ifconfig eth0 hw ether"
#define SET_INTERFACE_ETH0_UP		"ifconfig eth0 up"
#define SET_GATEWAY 				"route add default gw"



#define FILESYSTEM_RW_CMD		"mount -o remount,rw /"
#define FILESYSTEM_RO_CMD		"mount -o remount,ro /"


#define CHAR_PER_LINE	64
#define END_OF_LINE	'\n'


extern int update_fcs_config_file_backup(void);
extern int copy_fcs_config_file(void);
extern int get_station_config (void);
extern void init_config_manager();
extern int set_current_time_date(char *buf);

extern unsigned short gui_ip_address[FOUR];
extern unsigned short gui_mac_id[SIX];

extern char gc_network_type[TEN];
extern char gc_station_ip[TWENTY];
extern char gc_subnetmask[TWENTY];
extern char gc_gateway[TWENTY];



#endif
