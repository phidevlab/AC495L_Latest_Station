/**********************************************************************
 *                                                                    *
 * File Name:	i2c.c                                                 *
 *
 *                                                                    *
 *
 * Copyright (c) by Phi Audiocom Systems Pvt. Ltd.                    *
 * This software is copyrighted by and is the sole property of        *
 * Phi Audiocom Systems Pvt. Ltd.                                     *
 *
 * Release : 31 JAN 2023                                              *
 *                                                                    *
 **********************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <pty.h>
#include <utmp.h>
#include <pthread.h>
#include <linux/input.h>
#include <time.h>
#include <poll.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/timerfd.h>
#include <stdint.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <signal.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <signal.h>

#include "i2c.h"
#include "keypad.h"
#include "ui.h"

#include "keypad_interface.h"
#include "voip_main.h"
#include "server_coms.h"
#include "lcd_task.h"

char lcd[] = {0x20};
char io[] = {0x21};
unsigned char arr[1] = {0x00};

char SCAN_ROW_1[] = {0xFE};
char SCAN_ROW_2[] = {0xFD};
char SCAN_ROW_3[] = {0xFB};
char SCAN_ROW_4[] = {0xF7};
char SCAN_ROW_5[] = {0xEF};
char SCAN_ROW_6[] = {0xDF};
char SCAN_ROW_7[] = {0xBF};
char SCAN_ROW_8[] = {0x7F};

int lcd_io_Fd = -1;

unsigned char keyboard_state, current_row, current_column, key_pressed_event, key_pressed_flag, key_released_event, press_and_hold_event, keyboard_detected_key;
unsigned int debounce_cntr;

/*******************************************************************************************
 * Function name: open_lcd_io                                                          *
 * Input arg    : void                                                                     *
 * Output arg   : NONE                                                                     *
 * Description  : This function open lcd driver    						*
 *                                                                                         *
 ********************************************************************************************/
void open_lcd_io()
{
    printf("***************OPENING LCD DRIVER ****************\n");

    if ((lcd_io_Fd = open("/dev/klcd_io", O_RDWR)) == -1)
    {
        printf("[%s , %d ] can't open lcd device\r\n", __FUNCTION__, __LINE__);
        return;
    }
    // lcd_initialization();
}

/*******************************************************************************************
 * Function name: io_init                                                          *
 * Input arg    : void                                                                     *
 * Output arg   : NONE                                                                     *
 * Description  : This function initialize io expander 						 *
 *                                                                                         *
 ********************************************************************************************/
void io_initialization()
{
    int return_value;
    /* int input_no;
     char row_init_data[]={0x00};
     char col_init_data[]={0xFF};


     io_msg.i2c_device_address=I2C_ROW_ADDRESS;
     strcpy(io_msg.kbuf,row_init_data);
     ioctl(lcd_io_Fd,IO_WRITE, &io_msg);

     //printf("PRINT ROW %d \n",io_msg.i2c_device_address);

     sleep(1);

     io_msg.i2c_device_address=I2C_COLUMN_ADDRESS;
     strcpy(io_msg.kbuf,col_init_data);
     ioctl(lcd_io_Fd,IO_READ,  &io_msg);
     //printf("PRINT COLUMN %d \n",io_msg.i2c_device_address);
     sleep(1);*/

    /* printf("INSIDE IO INIT");
     char data[]={0xAA,0x55};

     char addr_1=I2C_ROW_ADDRESS;
     io_msg.cmd=IOCTL_IO_INITIALIZATION;
     io_msg.i2c_device_address=addr_1;
     strcpy(io_msg.kbuf,data);
     update_io_msg(io_msg);


     char addr_2=I2C_COLUMN_ADDRESS;
     io_msg.cmd=IOCTL_IO_INITIALIZATION;
     io_msg.i2c_device_address=addr_2;
     strcpy(io_msg.kbuf,data);
     update_io_msg(io_msg);*/

    char init_rows_low[] = {0xFF};

    io_msg.i2c_device_address = I2C_ROW_ADDRESS;
    strcpy(io_msg.kbuf, init_rows_low);
    return_value = ioctl(lcd_io_Fd, IO_WRITE, &io_msg);
    // printf("Rows %d \n", return_value);

    char init_col_high[] = {0xFF};

    io_msg.i2c_device_address = I2C_COLUMN_ADDRESS;
    strcpy(io_msg.kbuf, init_col_high);
    return_value = ioctl(lcd_io_Fd, IO_WRITE, &io_msg);
    // printf("Columns %d \n", return_value);
}

void update_io_msg(struct ioctl_io_mesg io_msg)
{
    switch (io_msg.cmd)
    {
    case IOCTL_IO_INITIALIZATION:
        ioctl(lcd_io_Fd, IO_WRITE, &io_msg);
        break;

    case IOCTL_SET_BIT_IO_EXPANDER:
        ioctl(lcd_io_Fd, IO_WRITE, &io_msg);
        break;

    case IOCTL_GET_BIT_IO_EXPANDER:
        ioctl(lcd_io_Fd, IO_READ, arr);
        break;

    default:
        printf("Invalid state in update_io_msg\n");
        break;
    }
}

/*******************************************************************************************
 * Function name: lcd_init                                                          *
 * Input arg    : void                                                                     *
 * Output arg   : NONE                                                                     *
 * Description  : This function initialize lcd     						*
 *                                                                                         *
 ********************************************************************************************/

void lcd_initialization()
{
    printf("In lcd_initialization\n");
    char buf[] = {0x30, 0x30, 0x30, 0x20, 0x28, 0x08, 0x01, 0x06, 0x0C};
    char addr = LCD_ADDRESS;
    printf("inside lcd_init %x\n ", addr);
    msg.i2c_device_address = addr;
    msg.cmd = IOCTL_LCD_INITIALIZATION;
    strcpy(msg.kbuf, buf);
    // memcpy(msg.kbuf,buf,9);
    msg.lineNumber = ONE;
    msg.start_position = ZERO;

    //  update_lcd_msg(msg);
}

/*******************************************************************************************
 * Function name: read_column                                                              *
 * Input arg    : void                                                                     *
 * Output arg   : char                                                                     *
 * Description  : This function reads the column status / gpio pin status         *
 *                                                                                         *
 ********************************************************************************************/

char read_column(void)
{
    unsigned char port2_pins_status, columns_keyboard_status;

    // reading columns p2.7-p2.0
    // columns_keyboard_status = ioctl(lcd_io_Fd,IO_READ,(I2C_COLUMN_ADDRESS));// masking bits from 8th bit(7-0 column status)

    // io_msg.i2c_device_address = I2C_COLUMN_ADDRESS;

    ioctl(lcd_io_Fd, IO_READ, &io_msg);
    columns_keyboard_status = io_msg.kbuf[0];
    // printf("**** in read_column  0x%08x \n", columns_keyboard_status);
    //   printf("*************** keyboard buffer  %x \n",io_msg.kbuf);
    return (columns_keyboard_status);
}

char extract_column_number(unsigned char column_status)
{
    // printf("Column status in extract col %x \n",column_status);
    unsigned char column_no = 0;
    column_status = 0XFF;
    switch (column_status) // key_pressed_flag = SET, if one column is detected
    {
    case SCAN_COLUMN_1:
        column_no = 1;
        key_pressed_flag = SET;

        break; // column 1
    case SCAN_COLUMN_2:
        column_no = 2;
        key_pressed_flag = SET;

        break; // column 2
    case SCAN_COLUMN_3:
        column_no = 3;
        key_pressed_flag = SET;

        break; // column 3
    case SCAN_COLUMN_4:
        column_no = 4;
        key_pressed_flag = SET;

        break; // column 4
    case SCAN_COLUMN_5:
        column_no = 5;
        key_pressed_flag = SET;

        break; // column 5
    case SCAN_COLUMN_6:
        column_no = 6;
        key_pressed_flag = SET;

        break; // column 6
    case SCAN_COLUMN_7:
        column_no = 7;
        key_pressed_flag = SET;

        break; // column 7
    case SCAN_COLUMN_8:
        column_no = 8;
        key_pressed_flag = SET;

        break; // column 8

    default:
      
        column_no = COLUMN_DEFAULT_CASE; // if multi keys are pressed - 0xFF
    }

    return (column_no);
}

void check_current_row(unsigned char row_no)
{

    switch (row_no) // only specific current_row is made low
    {
    case 1:
        io_msg.i2c_device_address = I2C_ROW_ADDRESS;
        strcpy(io_msg.kbuf, SCAN_ROW_1);
        ioctl(lcd_io_Fd, IO_WRITE, &io_msg);

        break;
    case 2:
        io_msg.i2c_device_address = I2C_ROW_ADDRESS;
        strcpy(io_msg.kbuf, SCAN_ROW_2);
        ioctl(lcd_io_Fd, IO_WRITE, &io_msg);

        break;
    case 3:
        io_msg.i2c_device_address = I2C_ROW_ADDRESS;
        strcpy(io_msg.kbuf, SCAN_ROW_3);
        ioctl(lcd_io_Fd, IO_WRITE, &io_msg);

        break;
    case 4:
        io_msg.i2c_device_address = I2C_ROW_ADDRESS;
        strcpy(io_msg.kbuf, SCAN_ROW_4);
        ioctl(lcd_io_Fd, IO_WRITE, &io_msg);

        break;
    case 5:
        io_msg.i2c_device_address = I2C_ROW_ADDRESS;
        strcpy(io_msg.kbuf, SCAN_ROW_5);
        ioctl(lcd_io_Fd, IO_WRITE, &io_msg);

        break;
    case 6:
        io_msg.i2c_device_address = I2C_ROW_ADDRESS;
        strcpy(io_msg.kbuf, SCAN_ROW_6);
        ioctl(lcd_io_Fd, IO_WRITE, &io_msg);

        break;
    case 7:
        io_msg.i2c_device_address = I2C_ROW_ADDRESS;
        strcpy(io_msg.kbuf, SCAN_ROW_7);
        ioctl(lcd_io_Fd, IO_WRITE, &io_msg);

        break;
    case 8:
        io_msg.i2c_device_address = I2C_ROW_ADDRESS;
        strcpy(io_msg.kbuf, SCAN_ROW_8);
        ioctl(lcd_io_Fd, IO_WRITE, &io_msg);

        break;
    default:
        printf("****** INSIDE DEFAULT OF ST_KEY_IDLE ******");
        break;
    }
}

void scan_keyboard(void)
{
    unsigned char recent_column_keyboard_state = 0, column_number = 0, one_key_detection_flag = 0, check_row_no = 0, check_current_column_status = 0;
    char scan_row_number = 0;
    unsigned int check_row = 0;
    // printf("++Scanning **** \n");
    switch (keyboard_state)
    {
    case ST_KEY_IDLE:

        for (check_row = 1; check_row < MAX_ROW; check_row++)
        {
            // getTime();
            // usleep(1000);
            switch (check_row)
            {

            case 1:
                io_msg.i2c_device_address = I2C_ROW_ADDRESS;
                // printf("I2C SCAN ROW %x \n",SCAN_ROW_1);
                strcpy(io_msg.kbuf, SCAN_ROW_1);
                ioctl(lcd_io_Fd, IO_WRITE, &io_msg);
                check_row_no = 1;
                break;
            case 2:
                io_msg.i2c_device_address = I2C_ROW_ADDRESS;
                strcpy(io_msg.kbuf, SCAN_ROW_2);
                ioctl(lcd_io_Fd, IO_WRITE, &io_msg);
                check_row_no = 2;
                break;
            case 3:
                io_msg.i2c_device_address = I2C_ROW_ADDRESS;
                strcpy(io_msg.kbuf, SCAN_ROW_3);
                ioctl(lcd_io_Fd, IO_WRITE, &io_msg);
                check_row_no = 3;
                break;
            case 4:
                io_msg.i2c_device_address = I2C_ROW_ADDRESS;
                strcpy(io_msg.kbuf, SCAN_ROW_4);
                ioctl(lcd_io_Fd, IO_WRITE, &io_msg);
                check_row_no = 4;
                break;
            case 5:
                io_msg.i2c_device_address = I2C_ROW_ADDRESS;
                strcpy(io_msg.kbuf, SCAN_ROW_5);
                ioctl(lcd_io_Fd, IO_WRITE, &io_msg);
                check_row_no = 5;
                break;
            case 6:
                io_msg.i2c_device_address = I2C_ROW_ADDRESS;
                strcpy(io_msg.kbuf, SCAN_ROW_6);
                ioctl(lcd_io_Fd, IO_WRITE, &io_msg);
                check_row_no = 6;
                break;
            case 7:
                io_msg.i2c_device_address = I2C_ROW_ADDRESS;
                strcpy(io_msg.kbuf, SCAN_ROW_7);
                ioctl(lcd_io_Fd, IO_WRITE, &io_msg);
                check_row_no = 7;
                break;
            case 8:
                io_msg.i2c_device_address = I2C_ROW_ADDRESS;
                strcpy(io_msg.kbuf, SCAN_ROW_8);
                ioctl(lcd_io_Fd, IO_WRITE, &io_msg);
                check_row_no = 8;
                break;
            default:
                printf("****** INSIDE DEFAULT OF ST_KEY_IDLE ******\n");
                break;
            }

            recent_column_keyboard_state = read_column(); // column status after making single row low
            column_number = extract_column_number(recent_column_keyboard_state);

            if (key_pressed_flag == SET && current_column == CLEAR) // On first key detection, current_column=CLEAR
            {
                
                current_column = column_number; // for first key detect/ single key detect
                current_row = check_row_no;     // respective row_no

                key_pressed_flag = CLEAR;
                one_key_detection_flag = SET; // for first key detection this flag is SET
            }
            else if (key_pressed_flag == SET && current_column != CLEAR) // condition true, in case of (multiple key pressed) on second key detection
            {
            
                current_column = CLEAR; // CLEAR
                current_row = CLEAR;

                keyboard_state = ST_KEY_IDLE; // on second key detection this scanning is breaked(case of multiple key detection)

                key_pressed_flag = CLEAR;
                one_key_detection_flag = CLEAR; // on second key detection this flag is CLEAR

                break;
            }
        }

        if (one_key_detection_flag == SET) // check for one key detection, then confirm key_press
        {
           
            debounce_cntr = PRESS_DEBOUNCE_TIMECOUNT; // PRESS debonce time set
            keyboard_state = ST_KEY_PRSS_CNFRM;
            one_key_detection_flag = CLEAR;
        }

        initialize_rows(); // initialise ROWS back to LOGIC HIGH

        break;

    case ST_KEY_PRSS_CNFRM:

       
        check_current_row(current_row); // only specific row is made low, then columns are checked

        recent_column_keyboard_state = read_column(); // colno contains in 2nd check
        check_current_column_status = recent_column_keyboard_state & (1 << (current_column - 1));

        if (check_current_column_status == CLEAR) // current_column is checked, status is clear if current_column pin status is low
        {

            keyboard_detected_key = digitToKeyArr[current_row - 1][current_column - 1]; // final key detected from keyboard
                                                                                        //   printf("+++++++++++++++ key pressed ------ : %c n",keyboard_detected_key);

            key_pressed_event = SET; // flag for key press event

            keyboard_state = ST_KEY_PRESS_CHK_FOR_RLS;
        }
        else
        {
            current_column = CLEAR; // CLEAR
            current_row = CLEAR;

            keyboard_state = ST_KEY_IDLE;
        }

        initialize_rows(); // initialise ROWS back to LOGIC HIGH

        break;

    case ST_KEY_PRESS_CHK_FOR_RLS:

        check_current_row(current_row); // only specific row is made low,then columns are checked

        recent_column_keyboard_state = read_column(); // colno contains in 2nd check
        check_current_column_status = recent_column_keyboard_state & (1 << (current_column - 1));

        if (check_current_column_status == CLEAR) // CLEAR means key is pressed
        {
            keyboard_state = ST_KEY_PRESS_CHK_FOR_RLS;
        }
        else if (check_current_column_status) // key is released
        {
            debounce_cntr = RELEASE_DEBOUNCE_TIMECOUNT; // PRESS debonce time set
            keyboard_state = ST_KEY_RLS_CNFRM;
        }

        initialize_rows(); // initialise ROWS back to LOGIC HIGH

        break;

    case ST_KEY_RLS_CNFRM:

        check_current_row(current_row); // only specific row is made low, then columns are checked

        recent_column_keyboard_state = read_column(); // colno contains in 2nd check
        check_current_column_status = recent_column_keyboard_state & (1 << (current_column - 1));

        if (check_current_column_status == CLEAR) // CLEAR means key is pressed
        {
            keyboard_state = ST_KEY_PRESS_CHK_FOR_RLS;
        }
        else // key is released
        {
            current_column = CLEAR; //
            current_row = CLEAR;

            key_released_event = SET; // flag for key release event

            keyboard_state = ST_KEY_IDLE;
        }

        initialize_rows(); // initialise ROWS back to LOGIC HIGH

        break;
    }
}

/**CLEARS_ALL_VARIABLE_FLAGS_USED_IN_KEYBOARD_SCAN_ROUTINE**/
void clear_keyboard_status(void)
{
    current_column = CLEAR; //
    current_row = CLEAR;

    keyboard_state = ST_KEY_IDLE;

    keyboard_detected_key = CLEAR;
    key_pressed_event = CLEAR;
    key_released_event = CLEAR;
    // initialize_rows();    // initialise ROWS back to LOGIC HIGH
}

void initialize_rows()
{

    char init_rows_high[] = {0xFF};

    io_msg.i2c_device_address = I2C_ROW_ADDRESS;
    strcpy(io_msg.kbuf, init_rows_high);
    ioctl(lcd_io_Fd, IO_WRITE, &io_msg);
}

void checkI2C()
{
    int i;

    open_lcd_io();

    // unsigned arr[2]={I2C_ROW_ADDRESS};

    //  io_msg.i2c_device_address=I2C_ROW_ADDRESS;
    //  strcpy(io_msg.kbuf,init_rows_high);

    // for(i=0;i<=10;i++)
    // {
    /*ioctl(lcd_io_Fd,IO_READ, arr[0]);
     sleep(1);
     printf("read arr[0] %x \n", arr[0]);
     sleep(1);*/

    char init_rows_high[] = {0x00};

    io_msg.i2c_device_address = I2C_ROW_ADDRESS;
    strcpy(io_msg.kbuf, init_rows_high);
    ioctl(lcd_io_Fd, IO_WRITE, &io_msg);
    printf("WRITE ROW DONE \n");

    char init_col_low[] = {0xFE};

    io_msg.i2c_device_address = I2C_COLUMN_ADDRESS;
    strcpy(io_msg.kbuf, init_col_low);
    ioctl(lcd_io_Fd, IO_WRITE, &io_msg);
    printf("WRITE COL DONE \n");

    // msleep(12);
    //}

    /* io_msg.i2c_device_address=I2C_ROW_ADDRESS;
                 strcpy(io_msg.kbuf,row_data);
                 ioctl(lcd_io_Fd,IO_WRITE, &io_msg);
                 check_row_no=1; */
}
