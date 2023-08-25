#include <ssp_drv.h>
#include <ssp_hal.h>


// ********* Linux driver Constants ******************************************************************

#define MINOR_NUM_START		0   // minor number starts from 0
#define MINOR_NUM_COUNT		1   // the number of minor numbers required

#define MAX_BUF_LENGTH  	50  // maximum length of a buffer to copy from user space to kernel space
#define MAX_BUF_LEN	  	52
#define MIN_BUF_LENGTH		1
#define MIN(x, y) (((x) < (y)) ? (x) : (y))


// ********* IOCTL COMMAND ARGUMENTS ******************************************************************

	// Identifiers for ioctl reqursts
				           

#define IOCTL_IO_INITIALIZATION		'3'
#define IOCTL_SET_BIT_IO_EXPANDER	'4'
#define IOCTL_GET_BIT_IO_EXPANDER	'5'

// ******** LCD Constants ************************************************************************

#define RS_COMMAND_MODE		0   // command mode to select Insruction register with RS signal
#define RS_DATA_MODE		1   // data mode to select Data register with RS signal

#define ENABLE_SET  		1
#define ENABLE_CLEAR		0

#define WRITE_SET               0
#define READ_SET		1

#define PIN_CLEAR		0
#define PIN_SET			1

#define LCD_FIRST_LINE		1
#define LCD_SECOND_LINE		2

#define NUM_CHARS_PER_LINE      16  // the number of characters per line
struct ioctl_lcd_data{				// a structure to be passed to ioctl argument
	char kbuf_lcd[MAX_BUF_LENGTH];
	unsigned int lineNumber ;
	unsigned int start_position ;	 
	char i2c_device_address;
	unsigned int command;	 
};

struct ioctl_io_data{				// a structure to be passed to ioctl argument
	char kbuf_io[MAX_BUF_LEN];	 
	char i2c_device_address;
	unsigned int command;
	unsigned int pin_no;
	unsigned int pin_status;
};
static void lcd_initialize( unsigned char * cmd ,unsigned char addr);
static void lcd_setup();
static void lcd_send_cmd(unsigned char cmd, unsigned char addr);
static void lcd_send_char(unsigned char data,unsigned char addr);
 
static void lcd_print(char * msg, unsigned int lineNumber, unsigned int start_position,unsigned char addr);
static void send_data_to_ioexpander(unsigned char *cmd ,unsigned int pin,unsigned int set_reset,unsigned char addr);
static void ioexpander_initialization(unsigned char *cmd,unsigned char addr);
static void lcd_setPosition(unsigned int line, unsigned int start_position, unsigned char addr);
static void lcd_clear_display(unsigned char addr);
static void set(unsigned char data,unsigned char addr);
static void clear(unsigned char data,unsigned char addr);

char res[1];


#define SET			1
#define CLEAR                 	0
#define IO_WRITE               	0
#define IO_READ                 1
#define LCD                 	2
#define IO_ADDRESS      	0x24
#define LCD_ADDRESS    		0x27             
#define CLASS_NAME  	"klcd_io"
#define DEVICE_NAME 	"klcd_io"
#define LCD_FIRST_LINE_ADDRESS  0x80
#define LCD_SECOND_LINE_ADDRESS  0xC0

#define DATA_EN_ACTIVATE              0x0D
#define DATA_EN_DEACTIVATE            0x09

#define CMD_EN_ACTIVATE		      0x0C
#define CMD_EN_DEACTIVATE   	      0X08 

#define MASK_HIGHER_BIT               0xF0
#define   ZERO            0
#define   ONE             1
#define   TWO             2
#define   THREE           3
#define   FOUR            4
#define   FIVE            5
#define   SIX             6
#define   SEVEN           7
#define   EIGHT           8
#define   NINE            9
#define   TEN             10
#define FIFTYONE	51
 

static dev_t 		dev_number;	// dynamically allocated device major number
struct cdev  		klcd_io_cdev;	// cdev structure
static struct class *  	klcd_io_class;	// class structure

