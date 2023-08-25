/*  
 *  Copyright(c) 2018 Phi Audiocom Systems Pvt Ltd. 	All Rights Reserved
 */

/* description: A user level test program to check the ioctl functionality ofthe implemented 
Linux device driver, which controls a 16x2 character LCD (with HD44780 LCD controller) with 4 bit mode.
This LCD is interfaced with AC494E using GPIO pins.
(Tested on Linux 3.8.10)

   name:	Abhay J Gurav (abhaygurav@phiaudiocom.net)
   date:	2018-July
   platform:	AC494E
*/

#ifndef DRIVER_H_
#define DRIVER_H_

#include <linux/ioctl.h>

#define MAX_BUF_LENGTH  	50  /* maximum length of a buffer to copy from user space to kernel space
				       (MUST NOT CHANGE THIS)
				    */
struct ioctl_mesg{
	char kbuf[MAX_BUF_LENGTH];	// a string to be printed on the LCD
	unsigned int lineNumber;	// line number (should be either 1 or 2)
	unsigned int nthCharacter;	// nth Character of a line (0 refers to the beginning of the line)
};

// ******************* IOCTL COMMAND ARGUMENTS ********************************************

#define KLCD_MAGIC_NUMBER   		0xBC  // a "magic" number to uniquely identify the device 

  // Identifiers for ioctl reqursts
#define IOCTL_PRINT_ON_FIRSTLINE  	'1'
#define IOCTL_PRINT_ON_SECONDLINE 	'2'	/* (Note) ioctl will not be called if this is unsigned int 2, which
						          is a reserved number. Thus it is fixed to '2'
						 */
#define IOCTL_PRINT_WITH_POSITION 	'3'
#define IOCTL_CURSOR_ON			'4'
#define IOCTL_CURSOR_OFF		'5'

#define WRITE_TEST_MODE1		'W'    // check error handling
#define WRITE_TEST_MODE2		'X'
#define WRITE_TEST_MODE3		'Y'

// ******************** IOCTL MACROS ********************************************************

#define KLCD_IOCTL_CLEAR_DISPLAY    	_IOW( KLCD_MAGIC_NUMBER, IOCTL_CLEAR_DISPLAY , struct ioctl_mesg)
#define KLCD_IOCTL_PRINT_ON_FIRSTLINE   _IOW( KLCD_MAGIC_NUMBER, IOCTL_PRINT_ON_FIRSTLINE,  struct ioctl_mesg)
#define KLCD_IOCTL_PRINT_ON_SECONDLINE  _IOW( KLCD_MAGIC_NUMBER, IOCTL_PRINT_ON_SECONDLINE, struct ioctl_mesg)
#define KLCD_IOCTL_PRINT_WITH_POSITION  _IOW( KLCD_MAGIC_NUMBER, IOCTL_PRINT_WITH_POSITION, struct ioctl_mesg)

#define KLCD_IOCTL_CURSOR_ON  		_IOW( KLCD_MAGIC_NUMBER, IOCTL_CURSOR_ON, struct ioctl_mesg)
#define KLCD_IOCTL_CURSOR_OFF  		_IOW( KLCD_MAGIC_NUMBER, IOCTL_CURSOR_OFF, struct ioctl_mesg)

#endif
