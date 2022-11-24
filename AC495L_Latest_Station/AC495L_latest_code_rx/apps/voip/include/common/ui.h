#ifndef _UI_MODULE_H
#define _UI_MODULE_H





#define MAX_LCD_LINES 4
//extern void update_led1_state(struct ui_led_msg *data);
//extern void update_lcd_state(struct ui_lcd_msg *data);

//MODULE_INIT(UI);
#define     HANDSET_RESET_KEY    47
#define TWENTY_FIVE	25
#define TWENTY_ONE	21

#define FOUR 4

#define LCD_ADDRESS 0x27
#define IO_ADDRESS 0x24

#define IOCTL_CLEAR_DISPLAY                 	'0'
#define IOCTL_PRINT                    		'1'
#define IOCTL_LCD_INITIALIZATION        	'2'
#define IOCTL_IO_INITIALIZATION		    	'3'
#define IOCTL_SET_BIT_IO_EXPANDER	    	'4'
#define IOCTL_GET_BIT_IO_EXPANDER	    	'5'


 struct ui_led_msg
 {

   short int parameter;
   short int led_no;
   short int cmd;
   char text[21]; //because from voipapp this is common so incase of led this field contains zero
   short int flash_cntr[4];
 }led_msg;




#define FIVE 5
#define SEVEN 7


/************Station States**************/
#define UI_INIT_STATE                   '1'//1
#define UI_VCCS_REGISTERING_STATE       '2'//2
#define UI_SIP_REGISTERING_STATE        '3'
#define UI_CONFIGURING_STATE            '4'
#define UI_IDLE_STATE                   '5'
#define UI_RINGBACK_STATE               '7'
#define UI_P2P_STATE                    '8'
#define UI_PG_INITIATOR_STATE           '9'
#define UI_INTERCOM_STATE               'b'
#define UI_GROUP_STATE                  'c'
#define UI_ICOM_HOP_STATE               'd'
#define UI_DSP_RADV_INIT_STATE          'f'
#define UI_CONFERENCE_STATE             'i'
#define UI_GROUP_CONTRIBUTOR_STATE      'p'




#define INTERDIGIT_TIMEOUT 30

//int update_lcd_state(void *data);
//int update_lcd_msg(char *data,int line_no,int clear_lcd,int position);

#define LCD_CLR_FIRST_LINE                  1
#define LCD_CLR_SECOND_LINE                 2
#define LCD_CLR_BOTH_LINE                   3
#define LCD_CLR_11_POSITION                 4
#define NO_LCD_CLR                          0

#define IOCTL_CLEAR_DISPLAY			        '0'
#define IOCTL_PRINT                    		'1'
#define IOCTL_LCD_INITIALIZATION        	'2'
#define IOCTL_IO_INITIALIZATION		    	'3'
#define IOCTL_SET_BIT_IO_EXPANDER	    	'4'
#define IOCTL_GET_BIT_IO_EXPANDER	    	'5'

#define PG_MUTE_ON 1
extern int gp_pvt,gp_pg,led_timer_flag,lcd_timer_flag,g_iserver_socket_fd;
extern void create_timer_fd();
//struct sockaddr_un ui_address;
void keypad_sig_handler(void);
void startKeypad(void);
socklen_t ui_adress_length;
//void update_lcd_msg(struct ioctl_mesg msg);
extern int  g_iconnection_fd;
extern int  g_iInterdigit_timer;
extern void clear_validation_processing();

unsigned int          g_uiPvt_call_time_lcd;
unsigned int          g_uipg_call_time_lcd_init;
unsigned int          g_uipg_call_time_lcd;

struct ioctl_io_mesg{				// a structure to be passed to ioctl argument
	char kbuf[52];
 	unsigned int i2c_device_address;
	short int cmd;
    unsigned int pin_no;
    unsigned int pin_status;
}io_msg;

struct ioctl_lcd_mesg{				// a structure to be passed to ioctl argument
	char kbuf[50];
	unsigned int lineNumber;
	unsigned int start_position;
	unsigned int i2c_device_address;
	short int cmd;
}msg;

fd_set readset,testset;
#endif
