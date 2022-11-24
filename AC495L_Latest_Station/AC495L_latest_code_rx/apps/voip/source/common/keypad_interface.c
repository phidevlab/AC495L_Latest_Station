/**********************************************************************
 *                                                                    *
 * File Name:	Keypad_interface.c                                    *
 *
 * Author: S.K. SHINDE                                                *
 *
 * Copyright (c) by Phi Audiocom Systems Pvt. Ltd.                    *
 * This software is copyrighted by and is the sole property of        *
 * Phi Audiocom Systems Pvt. Ltd.                                     *
 *
 * Release : 4 MAR 2018                                               *
 *                                                                    *
 **********************************************************************/



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pty.h>
#include <utmp.h>
#include <pthread.h>
#include <linux/input.h>
#include <time.h>
#include <poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/stat.h>
#include "keypad.h"


#include "ui.h"
#include "voip_main.h"
#include "keypad_interface.h"

//#include "lcd_messages.h"
//#include "ui_gpio_interface.h"



char c_Fsu_state;
char g_arcLCDMsg[TWENTY];
char g_IcomLcdMsg[TWENTY];
char g_ConfLcdMsg[TWENTY];

int  g_iKeyEventFD;
int  g_valid_key_sequence = CLEAR;
int  g_iKeyboard_state=IDLE_PROCESS;
int  g_ucDialed_Sequence_No = CLEAR;//,c_Fsu_state;
int  g_iarcDialed_Key_Array[ELEVEN];
int  g_iInterdigit_timer = CLEAR;





/*************************************************************************************************
 * Function name: clear_validation_processing                                                    *
 * Input arg    : void                                                                           *
 * Output arg   : NONE                                                                           *
 * Description  : This function clear the keys received in the buffer and set ui state to idle.  *
 *                                                                                               *
**************************************************************************************************/
void clear_validation_processing()
{
    g_iKeyboard_state      = IDLE_PROCESS;
    g_valid_key_sequence   = CLEAR;
    g_ucDialed_Sequence_No = 0;
    g_iInterdigit_timer = CLEAR;

//    send_data_to_lcd(2,"                   ");
    memset(g_iarcDialed_Key_Array, 0, sizeof(g_iarcDialed_Key_Array));
    //memset(g_arcLCDMsg, 0, sizeof(g_arcLCDMsg));
}

/*************************************************************************************************
 * Function name: clear_validation_processing                                                    *
 * Input arg    : pressed key                                                                    *
 * Output arg   : NONE                                                                           *
 * Description  : This function validate the keys in particular state                            *
 *                                                                                               *
**************************************************************************************************/

void validate_key(keypad_keys cInkey)
{
    /*Validation must take place considering-
    1. Current State of the station
    2. Current State of the keyboard
    3. Depending on the current mode of the keyboard, an input key will be processed
    4. Mode of the station (Initiator/Follower)  */

    printf("Keypad Mode: - %c  %d\n", c_Fsu_state,cInkey);
    //c_Fsu_state='5';
     switch(c_Fsu_state)
    {
        case UI_IDLE_STATE:

            switch(g_iKeyboard_state)
            {
                case IDLE_PROCESS:
                    if(cInkey >= P2P_DIGIT0_LOWER_VALUE && cInkey <= P2P_DIGIT0_UPPER_VALUE)
                    {
                         g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = P2P;
                         g_ucDialed_Sequence_No++;
                         g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = cInkey;

                        //Update display accordingly
//                        send_data_to_lcd(2, "                   ");
                        printf("UI Idle state pressed key=%d\n",g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]);
                        memcpy(g_arcLCDMsg,"P2P to:         ", SIXTEEN);

                        sprintf(&g_arcLCDMsg[SEVEN],"%d",g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]);
                        //update_lcd_msg(g_arcLCDMsg,LINE2,LCD_CLR_SECOND_LINE,0);
                           send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,"                ",ZERO,LCD_ADDRESS,IOCTL_PRINT);
                           send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_arcLCDMsg,ZERO,LCD_ADDRESS,IOCTL_PRINT);

                        g_ucDialed_Sequence_No++;
                        g_iInterdigit_timer=INTERDIGIT_TIMEOUT;
                        g_iKeyboard_state = P2P_KEY_DIALLING_MODE;
                    }
                    else
                    {
                switch(cInkey)
                {

                    case ZONE_KEY:
                        printf("ZONE KEY PRESSED \n");
                        g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = ZONE;
                        g_ucDialed_Sequence_No++;
                        g_iKeyboard_state = ZONE_KEY_DIALLING_MODE;
                    break;

                    case CONF_KEY: printf("CONF KEY PRESSED \n"); break;

                    case AIM_KEY:   printf("AIM_KEY KEY PRESSED \n"); break;
                    case ENTER_KEY: printf("ENTER_KEY KEY PRESSED \n"); break;
                    case EPBX_KEY:  printf("EPBX_KEY KEY PRESSED \n"); break;
                    case PRIO_KEY:  printf("PRIO_KEY KEY PRESSED \n"); break;

                    case VOL_UP_KEY: printf("VOL_UP_KEY KEY PRESSED \n");
                        volume_up_handle(PG_CHANNEL);
                    break;

                    case VOL_DN_KEY: printf("VOL_DN_KEY KEY PRESSED \n");
                        volume_down_handle(PG_CHANNEL);
                    break;

                    case I_COM_KEY:

                        printf("I_COM_KEY KEY PRESSED \n");
                        g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = INTERCOM_RX;
                        g_ucDialed_Sequence_No++;
                        g_iInterdigit_timer=INTERDIGIT_TIMEOUT;
                        g_iKeyboard_state = ICOM_KEY_DIALLING_MODE;

                        bzero(&g_IcomLcdMsg,sizeof(g_IcomLcdMsg));
                        memcpy(g_IcomLcdMsg,"CH: ", THREE);
                        //update_lcd_msg(g_IcomLcdMsg,LINE2,LCD_CLR_SECOND_LINE,0);
                        //send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_IcomLcdMsg,LCD_CLR_SECOND_LINE,0);


                    break;

                    case RESET_KEY:
                        printf("RESET KEY PRESSED \n");
                        clear_validation_processing();
                        g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = RESET;
                        g_ucDialed_Sequence_No++;
                        g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = cInkey;
                        g_valid_key_sequence = SET;
                    break;

                    case SRE_MUTE_KEY: printf("SRE_MUTE KEY PRESSED \n");
                    break;

                    case PTT_KEY:
                        printf("PTT_KEY KEY PRESSED \n");
                        g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = '0';
                        g_ucDialed_Sequence_No++;
                        g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = cInkey;
                        g_valid_key_sequence = SET;

                    break;

                    case D1_KEY:
                    case D2_KEY:
                    case D3_KEY:
                    case D4_KEY:
                    case D5_KEY:
                    case D6_KEY:
                    case D7_KEY:
                    case D8_KEY:
                    case D9_KEY:
                    case D10_KEY:
                    case D11_KEY:
                    case D12_KEY:
                    case D13_KEY:
                    case D14_KEY:
                    case D15_KEY:
                    case D16_KEY:
                    case D17_KEY:
                    case D18_KEY:
                    case D19_KEY:
                    case D20_KEY:
                    case D21_KEY:
                    case D22_KEY:
                    case D23_KEY:
                    case D24_KEY:
                    case D25_KEY:
                    case D26_KEY:
                    case D27_KEY:
                    case D28_KEY:
                    case D29_KEY:
                    case D30_KEY:

                        printf("D%d key pressed \n",cInkey-NINE);

                        g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = dir_info[cInkey-DIR_KEY_OFFSET].call_type[0];

                    switch(g_iarcDialed_Key_Array[g_ucDialed_Sequence_No])
                    {
                     case 'P':   //P2P CALL
                              g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]=P2P;
                              g_ucDialed_Sequence_No++;
                              g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]=
                              dir_info[cInkey-DIR_KEY_OFFSET].g_cCaller[0]-'0';
                              g_ucDialed_Sequence_No++;
                              g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]=
                              dir_info[cInkey-DIR_KEY_OFFSET].g_cCaller[1]-'0';
                              g_ucDialed_Sequence_No++;
                              g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]=
                              dir_info[cInkey-DIR_KEY_OFFSET].g_cCaller[2]-'0';
                              printf("P2p call to D1 %s\n",g_iarcDialed_Key_Array);
                              g_valid_key_sequence = SET;
                     break;

                     case 'G':  //GROUP CALL
                              g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]=GROUP;
                              g_ucDialed_Sequence_No++;
                              g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]=
                              dir_info[cInkey-DIR_KEY_OFFSET].number[0]-'0';
                              g_ucDialed_Sequence_No++;
                              g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]=
                              dir_info[cInkey-DIR_KEY_OFFSET].number[1]-'0';
                              g_valid_key_sequence = SET;
                    break;

                     case 'Z': //ZONE CALL
                              g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]=ZONE;
                              g_ucDialed_Sequence_No++;
                              g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]=
                              dir_info[cInkey-DIR_KEY_OFFSET].number[0]-'0';
                              g_ucDialed_Sequence_No++;
                              g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]=
                              dir_info[cInkey-DIR_KEY_OFFSET].number[1]-'0';
                              g_valid_key_sequence = SET;
                     break;

                     case 'A': //GLOABAL ALARM
                              g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]=GLOBAL_ALARM;
                              g_ucDialed_Sequence_No++;
                              g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]=
                              dir_info[cInkey-DIR_KEY_OFFSET].number[0]-'0';
                              g_ucDialed_Sequence_No++;
                              g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]=
                              dir_info[cInkey-DIR_KEY_OFFSET].number[1]-'0';
                              g_valid_key_sequence = SET;
                     break;

                     case 'L': //GLOBAL CALL
                             g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]=GLOBAL;
                             g_valid_key_sequence = SET;

                     break;

                     case 'I': //INTERCOM CALL
                             g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]=INTERCOM_RX;
                             g_ucDialed_Sequence_No++;
                             g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]=
                             dir_info[cInkey-DIR_KEY_OFFSET].number[0]-'0';
                             g_ucDialed_Sequence_No++;
                             g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]=
                             dir_info[cInkey-DIR_KEY_OFFSET].number[1]-'0';
                             g_valid_key_sequence = SET;

                     break;

                     case 'E': //ZONE ALARM CALL
                             // printf("Zone alarm call  %s \n",dir_info[cInkey-DIR_KEY_OFFSET].number);

                              g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]=ZONE_ALARM;
                                printf("Zone alarm call %d %d \n",g_ucDialed_Sequence_No,g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]);
                              g_ucDialed_Sequence_No++;

                              g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]=
                              dir_info[cInkey-DIR_KEY_OFFSET].number[0]-'0';
                               printf("Zone alarm call %d %d \n",g_ucDialed_Sequence_No,g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]);
                              g_ucDialed_Sequence_No++;

                              g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]=
                              dir_info[cInkey-DIR_KEY_OFFSET].number[1]-'0';
                               printf("Zone alarm call %d %d \n",g_ucDialed_Sequence_No,g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]);
                               g_ucDialed_Sequence_No++;

                              g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]=
                              dir_info[cInkey-DIR_KEY_OFFSET].number[2]-'0';
                               printf("Zone alarm call %d %d \n",g_ucDialed_Sequence_No,g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]);
                               g_ucDialed_Sequence_No++;

                              g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]=
                              dir_info[cInkey-DIR_KEY_OFFSET].number[3]-'0';
                               printf("Zone alarm call %d %d \n",g_ucDialed_Sequence_No,g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]);
                              g_valid_key_sequence = SET;
                     break;

                   }
        break;


                    default:

                            clear_validation_processing();
                            printf("IN DEFAULT  keyboard_state is=%d....fsu_state is=%c...\n",g_iKeyboard_state,c_Fsu_state);
                    break;


                        }
                    }
                    break;

               case P2P_KEY_DIALLING_MODE:
                    switch(cInkey)
                    {

					   case RESET_KEY:

                            send_msg_ui_lcd(INIT_STATE,LCD,LINE2,CONTINUE_DISPLAY,"RESET_KEY       ",ZERO,
                            LCD_ADDRESS,IOCTL_PRINT);

                            clear_validation_processing();
                            if(lcd_fcs_default_icom == 0)
                            {
                                sprintf(g_cLine3_buf,"STN_NO:%d  IDLE",stn_config.logical_id);
                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,
                                LCD_ADDRESS,IOCTL_PRINT);
                            }
                            else
                            {
                                sprintf(g_cLine3_buf,"STN_NO:%d CH:%d ",stn_config.logical_id,lcd_fcs_default_icom);
                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,
                                LCD_ADDRESS,IOCTL_PRINT);
                            }

					   break;

                       default:
                            switch(g_ucDialed_Sequence_No)
                            {

                                case DIGIT1:
                                    if(cInkey >= DIGIT_0 && cInkey <= DIGIT_9)
                                    {printf("DIGIT1\n");
                                        g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = cInkey;
                                        sprintf(&g_arcLCDMsg[EIGHT],"%d",g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]);
                                        //update_lcd_msg(g_arcLCDMsg,LINE2,LCD_CLR_SECOND_LINE,0);
                                       // send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_arcLCDMsg,NO_LCD_CLR,0);
                                        send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_arcLCDMsg,ZERO,
                                        LCD_ADDRESS,IOCTL_PRINT);
                                        printf("Pressed key=%d\n",g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]);
                                        g_iInterdigit_timer=INTERDIGIT_TIMEOUT;
                                        g_ucDialed_Sequence_No++;
                                    }
                                    else
                                    {
                                        printf("Invalid Key!\n");
//                                        send_data_to_lcd(2, "Invalid Key Pressed");
                                        //update_lcd_msg("INVALID_KEY",LINE2,LCD_CLR_SECOND_LINE,0);
                                        send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,"  INVALID_KEY   ",ZERO,
                                        LCD_ADDRESS,IOCTL_PRINT);

                                        clear_validation_processing();

                                        if(lcd_fcs_default_icom == 0)
                                        {
                                            sprintf(g_cLine3_buf,"STN_NO:%d  IDLE",stn_config.logical_id);
                                            send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,
                                            LCD_ADDRESS,IOCTL_PRINT);
                                        }
                                        else
                                        {
                                            sprintf(g_cLine3_buf,"STN_NO:%d CH:%d ",stn_config.logical_id,lcd_fcs_default_icom);
                                            send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,
                                            LCD_ADDRESS,IOCTL_PRINT);
                                        }
                                    }
                                    break;

                                case DIGIT2:
                                    if(cInkey >= DIGIT_0 && cInkey <= DIGIT_9)
                                    {printf("DIGIT2\n");
                                        g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = cInkey;
                                        sprintf(&g_arcLCDMsg[NINE],"%d",g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]);
                                        // update_lcd_msg(g_arcLCDMsg,LINE2,LCD_CLR_SECOND_LINE,0);
                                        send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_arcLCDMsg,ZERO,
                                        LCD_ADDRESS,IOCTL_PRINT);
                                        printf("PRessed key=%d\n",g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]);
                                        g_ucDialed_Sequence_No++;
                                        if(g_ucDialed_Sequence_No == NO_OF_PTP_CALL_DIGITS)
                                        {
                                            printf("P2P calling......\n");
                                            g_iInterdigit_timer=CLEAR;
                                            g_valid_key_sequence = SET;
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        printf("Invalid Key!\n");
//                                        send_data_to_lcd(2, "Invalid Key Pressed");
                                        //update_lcd_msg("INVALID_KEY",LINE2,LCD_CLR_SECOND_LINE,0);
                                        send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,"  INVALID_KEY   ",ZERO,
                                        LCD_ADDRESS,IOCTL_PRINT);

                                        clear_validation_processing();

                                        if(lcd_fcs_default_icom == 0)
                                        {
                                            sprintf(g_cLine3_buf,"STN_NO:%d  IDLE",stn_config.logical_id);
                                            send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,
                                            LCD_ADDRESS,IOCTL_PRINT);
                                        }
                                        else
                                        {
                                            sprintf(g_cLine3_buf,"STN_NO:%d CH:%d ",stn_config.logical_id,lcd_fcs_default_icom);
                                            send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,
                                            LCD_ADDRESS,IOCTL_PRINT);
                                        }
                                    }
                                    break;
                            }
                            break;
                    }
                    break;

                case EPBX_KEY_DIALLING_MODE:
                    break;

                case ZONE_KEY_DIALLING_MODE:
                     switch(cInkey)
                    {
                        case RESET_KEY:
					   		clear_validation_processing();
                        break;

                        default:
                            switch(g_ucDialed_Sequence_No)
                            {
                               case DIGIT0:
                                    if(cInkey > DIGIT_0 && cInkey <= DIGIT_4)
                                    {
                                        g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = cInkey;
                                        strcat(g_arcLCDMsg, &g_iarcDialed_Key_Array[2]);
//                                        send_data_to_lcd(2, g_arcLCDMsg);
                                        printf("Pressed key=%d\n",g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]);
                                        g_iInterdigit_timer=INTERDIGIT_TIMEOUT;
                                        g_ucDialed_Sequence_No++;
                                    }
                                    else
                                    {
                                        printf("Invalid Key!\n");
//                                      send_data_to_lcd(2, "Invalid Key Pressed");
                                        clear_validation_processing();
                                    }
                               break;

                               case DIGIT1:
                                    switch(g_iarcDialed_Key_Array[DIGIT0])
                                    {
                                        case DIGIT_1:
                                        case DIGIT_2:
                                        case DIGIT_3:
                                            if(cInkey > DIGIT_0 && cInkey <= DIGIT_9)
                                            {
                                                g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = cInkey;
                                                strcat(g_arcLCDMsg, &g_iarcDialed_Key_Array[2]);
//                                              send_data_to_lcd(2, g_arcLCDMsg);
                                                printf("Pressed key=%d\n",g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]);
                                                g_iInterdigit_timer=INTERDIGIT_TIMEOUT;
                                                g_ucDialed_Sequence_No++;
                                                 if(g_ucDialed_Sequence_No == 3)
                                                 {
                                                    printf("ZONE calling......\n");
                                                    g_iInterdigit_timer=CLEAR;
                                                    g_valid_key_sequence = SET;
                                                    break;
                                                 }
                                            }
                                            else
                                            {
                                                printf("Invalid Key!\n");
//                                              send_data_to_lcd(2, "Invalid Key Pressed");
                                                clear_validation_processing();
                                            }

                                        break;

                                        case DIGIT_4:
                                            if(cInkey > DIGIT_0 && cInkey <= DIGIT_2)
                                            {
                                                g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = cInkey;
                                                strcat(g_arcLCDMsg, &g_iarcDialed_Key_Array[2]);
//                                              send_data_to_lcd(2, g_arcLCDMsg);
                                                printf("Pressed key=%d\n",g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]);
                                                g_iInterdigit_timer=INTERDIGIT_TIMEOUT;
                                                g_ucDialed_Sequence_No++;

                                                if(g_ucDialed_Sequence_No == 3)
                                                 {
                                                    printf("ZONE calling......\n");
                                                    g_iInterdigit_timer=CLEAR;
                                                    g_valid_key_sequence = SET;
                                                    break;
                                                 }
                                            }
                                            else
                                            {
                                                printf("Invalid Key!\n");
//                                              send_data_to_lcd(2, "Invalid Key Pressed");
                                                clear_validation_processing();
                                            }
                                        break;
                                    }
                               break;

                            }
                        break;
                    }

                    break;

                case FSU_MENU_MODE:
                    break;

                case ICOM_KEY_DIALLING_MODE:
                    switch(cInkey)
                    {
                      case RESET_KEY:
					   		//update_lcd_msg("RESET_KEY        ",LINE2,LCD_CLR_SECOND_LINE,0);
					   		send_msg_ui_lcd(INIT_STATE,LCD,LINE2,CONTINUE_DISPLAY,"  RESET_KEY     ",ZERO,
                            LCD_ADDRESS,IOCTL_PRINT);
                            clear_validation_processing();
                            if(lcd_fcs_default_icom == 0)
                            {
                                sprintf(g_cLine3_buf,"STN_NO:%d  IDLE",stn_config.logical_id);
                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,
                                LCD_ADDRESS,IOCTL_PRINT);
                            }
                            else
                            {
                                sprintf(g_cLine3_buf,"STN_NO:%d CH:%d ",stn_config.logical_id,lcd_fcs_default_icom);
                                send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,
                                LCD_ADDRESS,IOCTL_PRINT);
                            }
					   break;

                       default:
                            switch(g_ucDialed_Sequence_No)
                            {
                                case DIGIT0:
                                    if(cInkey >= DIGIT_1 && cInkey <= DIGIT_4)
                                    {
                                       //bzero(&g_IcomLcdMsg,sizeof(g_IcomLcdMsg));
                                       g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = cInkey;
                                       strcat(g_IcomLcdMsg, &g_iarcDialed_Key_Array[1]);
                                       g_iInterdigit_timer=INTERDIGIT_TIMEOUT;
                                       printf("ICOM KEY %d\n  g_arcLCDMsg[1] %c\n ,g_arcLCDMsg[1] %d\n",g_iarcDialed_Key_Array[g_ucDialed_Sequence_No],g_arcLCDMsg[0],g_arcLCDMsg[0]);
                                      //memcpy(g_IcomLcdMsg,"ICOM: ", FIVE);
                                       sprintf(&g_IcomLcdMsg[THREE],"%d",g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]);
                                       //update_lcd_msg(g_IcomLcdMsg,LINE2,LCD_CLR_SECOND_LINE,0);
                                      // send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_IcomLcdMsg,LCD_CLR_SECOND_LINE,0);
                                       g_ucDialed_Sequence_No++;
                                       break;
                                    }
                                    else
                                    {
                                        printf("DIGIT 0 Invalid Key!\n");

                                        //update_lcd_msg("INVALID_KEY",LINE2,LCD_CLR_SECOND_LINE,0);
                                        //send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,"INVALID_KEY        ",LCD_CLR_SECOND_LINE,0);
                                        clear_validation_processing();
                                       // sprintf(g_cLine2_buf,"STN_NO:%d  IDLE  ",stn_config.logical_id);
                                       // send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,LCD_CLR_SECOND_LINE,0);
                                        if(lcd_fcs_default_icom == 0)
                                        {
                                            sprintf(g_cLine3_buf,"STN_NO:%d  IDLE",stn_config.logical_id);
                                            send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,
                                            LCD_ADDRESS,IOCTL_PRINT);
                                        }
                                        else
                                        {
                                            sprintf(g_cLine3_buf,"STN_NO:%d CH:%d ",stn_config.logical_id,lcd_fcs_default_icom);
                                            send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,
                                            LCD_ADDRESS,IOCTL_PRINT);
                                        }

                                    }
                                    break;

                                case DIGIT1:
                                    switch(g_iarcDialed_Key_Array[DIGIT0])
                                    {
                                        case ONE:
                                            if(cInkey >= DIGIT_1 && cInkey <= DIGIT_9)
                                            {
                                                g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = cInkey;
                                                strcat(g_IcomLcdMsg, &g_iarcDialed_Key_Array[2]);

                                                sprintf(&g_IcomLcdMsg[FOUR],"%d",g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]);
                                                //update_lcd_msg(g_IcomLcdMsg,LINE2,LCD_CLR_SECOND_LINE,0);
                                                //send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_IcomLcdMsg,LCD_CLR_SECOND_LINE,0);

                                                g_ucDialed_Sequence_No++;
                                                if(g_ucDialed_Sequence_No == NO_OF_GROUP_CALL_DIGITS)
                                                {
                                                    printf("ICOM HOP Calling..........\n ");
                                                    g_iInterdigit_timer=CLEAR;
                                                    g_valid_key_sequence = SET;
                                                    break;
                                                }

                                            }
                                            else
                                            {
                                                printf("DIGIT 1 1 Invalid Key!\n");
                                                update_lcd_msg("INVALID_KEY",LINE2,LCD_CLR_SECOND_LINE,0);
                                               // send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,"INVALID_KEY        ",LCD_CLR_SECOND_LINE,0);
                                                clear_validation_processing();
                                                if(lcd_fcs_default_icom == 0)
                                                {
                                                    sprintf(g_cLine3_buf,"STN_NO:%d  IDLE",stn_config.logical_id);
                                                    send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,
                                                    LCD_ADDRESS,IOCTL_PRINT);
                                                }
                                                else
                                                {
                                                    sprintf(g_cLine3_buf,"STN_NO:%d CH:%d ",stn_config.logical_id,lcd_fcs_default_icom);
                                                    send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,
                                                    LCD_ADDRESS,IOCTL_PRINT);
                                                }
                                            }
                                            break;
                                        case TWO:
                                            if(cInkey >= DIGIT_1 && cInkey <= DIGIT_9)
                                            {
                                                g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = cInkey;
                                                strcat(g_IcomLcdMsg, &g_iarcDialed_Key_Array[2]);

                                                sprintf(&g_IcomLcdMsg[FOUR],"%d",g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]);
                                                //update_lcd_msg(g_IcomLcdMsg,LINE2,LCD_CLR_SECOND_LINE,0);
                                                 //send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_IcomLcdMsg,LCD_CLR_SECOND_LINE,0);

                                                g_ucDialed_Sequence_No++;
                                                if(g_ucDialed_Sequence_No == NO_OF_GROUP_CALL_DIGITS)
                                                {
                                                    printf("ICOM HOP Calling..........\n ");
                                                    g_iInterdigit_timer=CLEAR;
                                                    g_valid_key_sequence = SET;
                                                    break;
                                                }

                                            }
                                            else
                                            {
                                                printf("DIGIT 1 2 Invalid Key!\n");
                                                 update_lcd_msg("INVALID_KEY",LINE2,LCD_CLR_SECOND_LINE,0);
                                                //send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,"INVALID_KEY        ",LCD_CLR_SECOND_LINE,0);
                                                clear_validation_processing();
                                                if(lcd_fcs_default_icom == 0)
                                                {
                                                    sprintf(g_cLine3_buf,"STN_NO:%d  IDLE",stn_config.logical_id);
                                                    send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,
                                                    LCD_ADDRESS,IOCTL_PRINT);
                                                }
                                                else
                                                {
                                                    sprintf(g_cLine3_buf,"STN_NO:%d CH:%d ",stn_config.logical_id,lcd_fcs_default_icom);
                                                    send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,
                                                    LCD_ADDRESS,IOCTL_PRINT);
                                                }

                                            }
                                            break;

                                        case THREE:
                                            if(cInkey >= DIGIT_1 && cInkey <= DIGIT_9)
                                            {
                                                g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = cInkey;
                                                strcat(g_IcomLcdMsg, &g_iarcDialed_Key_Array[2]);
                                                sprintf(&g_IcomLcdMsg[FOUR],"%d",g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]);
                                                //update_lcd_msg(g_IcomLcdMsg,LINE2,LCD_CLR_SECOND_LINE,0);
                                                //send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_IcomLcdMsg,LCD_CLR_SECOND_LINE,0);

                                                g_ucDialed_Sequence_No++;
                                                if(g_ucDialed_Sequence_No == NO_OF_GROUP_CALL_DIGITS)
                                                {
                                                    printf("ICOM HOP Calling..........\n ");
                                                    g_iInterdigit_timer=CLEAR;
                                                    g_valid_key_sequence = SET;
                                                     break;
                                                }

                                            }
                                            else
                                            {
                                                printf("DIGIT 1 3 Invalid Key!\n");
                                                update_lcd_msg("INVALID_KEY",LINE2,LCD_CLR_SECOND_LINE,0);
                                                //send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,"INVALID_KEY        ",LCD_CLR_SECOND_LINE,0);
                                                clear_validation_processing();
                                                if(lcd_fcs_default_icom == 0)
                                                {
                                                    sprintf(g_cLine3_buf,"STN_NO:%d  IDLE",stn_config.logical_id);
                                                    send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,
                                                    LCD_ADDRESS,IOCTL_PRINT);
                                                }
                                                else
                                                {
                                                    sprintf(g_cLine3_buf,"STN_NO:%d CH:%d ",stn_config.logical_id,lcd_fcs_default_icom);
                                                    send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,
                                                    LCD_ADDRESS,IOCTL_PRINT);
                                                }

                                            }
                                            break;

                                        case FOUR:
                                            if(cInkey >= DIGIT_1 && cInkey <= DIGIT_9)
                                            {
                                                g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = cInkey;
                                                strcat(g_IcomLcdMsg, &g_iarcDialed_Key_Array[2]);
                                                sprintf(&g_IcomLcdMsg[FOUR],"%d",g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]);
                                                //update_lcd_msg(g_IcomLcdMsg,LINE2,LCD_CLR_SECOND_LINE,0);
                                                //send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_IcomLcdMsg,LCD_CLR_SECOND_LINE,0);

                                                g_ucDialed_Sequence_No++;
                                                if(g_ucDialed_Sequence_No == NO_OF_GROUP_CALL_DIGITS)
                                                {
                                                    printf("ICOM HOP Calling..........\n ");
                                                    g_iInterdigit_timer=CLEAR;
                                                    g_valid_key_sequence = SET;
                                                    break;
                                                }

                                            }
                                            else
                                            {
                                                    printf("DIGIT 1 4 Invalid Key!\n");
                                                    update_lcd_msg("INVALID_KEY",LINE2,LCD_CLR_SECOND_LINE,0);
                                                   // send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,"INVALID_KEY        ",LCD_CLR_SECOND_LINE,0);
                                                    clear_validation_processing();
                                                    if(lcd_fcs_default_icom == 0)
                                                    {
                                                        sprintf(g_cLine3_buf,"STN_NO:%d  IDLE",stn_config.logical_id);
                                                        send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,
                                                        LCD_ADDRESS,IOCTL_PRINT);
                                                    }
                                                    else
                                                    {
                                                        sprintf(g_cLine3_buf,"STN_NO:%d CH:%d ",stn_config.logical_id,lcd_fcs_default_icom);
                                                        send_msg_ui_lcd(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine3_buf,ZERO,
                                                        LCD_ADDRESS,IOCTL_PRINT);
                                                    }
                                            }
                                            break;
                                    }
                                    break;
                            }
                            break;
                    }
                    break;



                default:
                printf("Invalid Key!\n");
///                send_data_to_lcd(2, "Invalid Key Pressed");
                clear_validation_processing();
                break;
            }
            break;

        case  UI_P2P_STATE:

                    switch(cInkey)
                    {

                         case VOL_UP_KEY: printf("VOL_UP_KEY KEY PRESSED \n");
                            // volume_up_handle(PVT_CHANNEL);

                         break;
                         case VOL_DN_KEY: printf("VOL_DN_KEY KEY PRESSED \n");
                            //  volume_down_handle(PVT_CHANNEL);
                         break;
                         case RESET_KEY:
                              g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = RESET;
                              g_ucDialed_Sequence_No++;
                              g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = cInkey;
                              g_valid_key_sequence = SET;
                              printf("RESET KEY PRESSED \n");

                         break;

                         case CONF_KEY:

                                g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = CONFERENCE;
                                g_ucDialed_Sequence_No++;
                               // memcpy(g_arcLCDMsg, "CONF Calling to ", FIFTEEN);
                                printf("CONF KEY PRESSED in P2P STATE\n");
                             //   strcat(g_arcLCDMsg, &g_iarcDialed_Key_Array[1]);
//                              send_data_to_lcd(2, g_arcLCDMsg);

                                bzero(&g_ConfLcdMsg,sizeof(g_ConfLcdMsg));
                                memcpy(g_ConfLcdMsg,"CONF: ",FIVE);
                               // update_lcd_msg(g_ConfLcdMsg,LINE2,LCD_CLR_FIRST_10_POSITION,0);
                                //send_msg_ui(P2P_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_ConfLcdMsg,LCD_CLR_FIRST_10_POSITION,0);

                                g_iInterdigit_timer=INTERDIGIT_TIMEOUT;
                                g_iKeyboard_state = CONF_KEY_DIALLING_MODE;

                        break;

                        default:

                            printf("inside default of UI_P2P_STATE /////Entered key is invalid.... %d\n",cInkey);
                            break;


                    }

                    ///FOR CONFERENCE CALL ONLY
                    switch(g_iKeyboard_state)
                    {
                        case CONF_KEY_DIALLING_MODE:

                         switch(g_ucDialed_Sequence_No)
                         {
                                case 1:

                                    if(cInkey >= P2P_DIGIT0_LOWER_VALUE && cInkey <= P2P_DIGIT0_UPPER_VALUE)
                                    {
                                        g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = cInkey;
                                        printf("UI Idle state pressed key=%d\n",g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]);
                                        strcat(g_arcLCDMsg, &g_iarcDialed_Key_Array[1]);

                                        sprintf(&g_ConfLcdMsg[FIVE],"%d",g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]);
                                        //update_lcd_msg(g_ConfLcdMsg,LINE2,LCD_CLR_FIRST_10_POSITION,0);
                                        //send_msg_ui(P2P_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_ConfLcdMsg,LCD_CLR_FIRST_10_POSITION,0);

                                        g_ucDialed_Sequence_No++;
                                        g_iInterdigit_timer=INTERDIGIT_TIMEOUT;
                                    }
                                break;


                                case 2:
                                    if(cInkey >= DIGIT_0 && cInkey <= DIGIT_9)
                                    {
                                        g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = cInkey;
                                        strcat(g_arcLCDMsg, &g_iarcDialed_Key_Array[2]);
//                                        send_data_to_lcd(2, g_arcLCDMsg);
                                        printf("DIGIT !Pressed key=%d\n",g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]);

                                        sprintf(&g_ConfLcdMsg[SIX],"%d",g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]);
                                        //update_lcd_msg(g_ConfLcdMsg,LINE2,LCD_CLR_FIRST_10_POSITION,0);
                                        //send_msg_ui(P2P_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_ConfLcdMsg,LCD_CLR_FIRST_10_POSITION,0);

                                        g_iInterdigit_timer=INTERDIGIT_TIMEOUT;
                                        g_ucDialed_Sequence_No++;
                                    }
                                    else
                                    {
                                        printf("Invalid Key!\n");
//                                        send_data_to_lcd(2, "Invalid Key Pressed");
                                        //update_lcd_msg("INVALID_KEY",LINE2,LCD_CLR_FIRST_10_POSITION,0);
                                        //send_msg_ui(P2P_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_ConfLcdMsg,LCD_CLR_FIRST_10_POSITION,0);
                                        clear_validation_processing();
                                       // sprintf(g_cLine2_buf,"STN_NO:%d  IDLE  ",stn_config.logical_id);
                                       // send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,LCD_CLR_SECOND_LINE);
                                    }
                                    break;

                                case 3:
                                    if(cInkey >= DIGIT_0 && cInkey <= DIGIT_9)
                                    {
                                        g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = cInkey;
                                        strcat(g_arcLCDMsg, &g_iarcDialed_Key_Array[3]);
//                                        send_data_to_lcd(2, g_arcLCDMsg);
                                        printf("DIGIT2 Preessed key=%d\n",g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]);

                                        sprintf(&g_ConfLcdMsg[SEVEN],"%d",g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]);
                                         //update_lcd_msg(g_ConfLcdMsg,LINE2,LCD_CLR_FIRST_10_POSITION,0);
                                         //send_msg_ui(P2P_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_ConfLcdMsg,LCD_CLR_FIRST_10_POSITION,0);


                                        g_ucDialed_Sequence_No++;
                                        if(g_ucDialed_Sequence_No ==4)
                                        {
                                            printf("P2P calling......\n");
                                            g_iInterdigit_timer=CLEAR;
                                            g_valid_key_sequence = SET;
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        printf("Invalid Key!\n");
//                                        send_data_to_lcd(2, "Invalid Key Pressed");

                                       // update_lcd_msg("INVALID_KEY",LINE2,LCD_CLR_FIRST_10_POSITION,0);
                                       //send_msg_ui(P2P_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_ConfLcdMsg,LCD_CLR_FIRST_10_POSITION,0);
                                        clear_validation_processing();
                                        //sprintf(g_cLine2_buf,"STN_NO:%d  IDLE  ",stn_config.logical_id);
                                        //send_msg_ui(IDLE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_cLine2_buf,LCD_CLR_SECOND_LINE);
                                    }
                                    break;
                            }

                        break;
                    }

            break;

        case  UI_RINGBACK_STATE:

                    switch(cInkey)
                    {

                    }

            break;

         case  UI_INTERCOM_STATE:

               switch(cInkey)
               {
                 case RESET_KEY:
                      g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = RESET;
                     g_ucDialed_Sequence_No++;
                     g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = cInkey;
                     g_valid_key_sequence = SET;
                     printf("RESET KEY PRESSED \n");
                break;
               }

            break;


         case  UI_CONFERENCE_STATE:

               switch(cInkey)
               {
                 case RESET_KEY:
                     g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = RESET;
                     g_ucDialed_Sequence_No++;
                     g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = cInkey;
                     g_valid_key_sequence = SET;
                     printf("RESET KEY PRESSED \n");
                 break;

                 case CONF_KEY:
                     g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = CONFERENCE;
                     g_ucDialed_Sequence_No++;
                    // memcpy(g_arcLCDMsg, "CONF Calling to ", FIFTEEN);
                     printf("CONF KEY PRESSED in UI_CONFERENCE_STATE**********\n");

                     bzero(&g_ConfLcdMsg,sizeof(g_ConfLcdMsg));
                     memcpy(g_ConfLcdMsg,"CONF: ", FIVE);
                     //update_lcd_msg(g_ConfLcdMsg,LINE2,LCD_CLR_FIRST_10_POSITION,0);
                     //send_msg_ui(CONFERENCE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_ConfLcdMsg,LCD_CLR_FIRST_10_POSITION,0);


                //   strcat(g_arcLCDMsg, &g_iarcDialed_Key_Array[1]);
//                   send_data_to_lcd(2, g_arcLCDMsg);
                     g_iInterdigit_timer=INTERDIGIT_TIMEOUT;
                     g_iKeyboard_state = CONF_KEY_DIALLING_MODE;

                 break;
               }


                   ///FOR CONFERENCE CALL ONLY
                    switch(g_iKeyboard_state)
                    {
                        case CONF_KEY_DIALLING_MODE:

                         switch(g_ucDialed_Sequence_No)
                         {
                                case 1:

                                    if(cInkey >= P2P_DIGIT0_LOWER_VALUE && cInkey <= P2P_DIGIT0_UPPER_VALUE)
                                    {
                                        g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = cInkey;
                                        printf("UI Idle state pressed key=%d\n",g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]);
                                        strcat(g_arcLCDMsg, &g_iarcDialed_Key_Array[1]);

                                        sprintf(&g_ConfLcdMsg[FIVE],"%d",g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]);
                                       // update_lcd_msg(g_ConfLcdMsg,LINE2,LCD_CLR_FIRST_10_POSITION,0);
                                       //send_msg_ui(CONFERENCE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_ConfLcdMsg,LCD_CLR_FIRST_10_POSITION,0);

                                        g_ucDialed_Sequence_No++;
                                        g_iInterdigit_timer=INTERDIGIT_TIMEOUT;
                                    }

                                break;


                                case 2:
                                    if(cInkey >= DIGIT_0 && cInkey <= DIGIT_9)
                                    {
                                        g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = cInkey;
                                        strcat(g_arcLCDMsg, &g_iarcDialed_Key_Array[2]);
//                                        send_data_to_lcd(2, g_arcLCDMsg);
                                        printf("DIGIT !Pressed key=%d\n",g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]);

                                        sprintf(&g_ConfLcdMsg[SIX],"%d",g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]);
                                       // update_lcd_msg(g_ConfLcdMsg,LINE2,LCD_CLR_FIRST_10_POSITION,0);
                                       //send_msg_ui(CONFERENCE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_ConfLcdMsg,LCD_CLR_FIRST_10_POSITION,0);

                                        g_iInterdigit_timer=INTERDIGIT_TIMEOUT;
                                        g_ucDialed_Sequence_No++;
                                    }
                                    else
                                    {
                                        printf("Invalid Key!\n");
//                                        send_data_to_lcd(2, "Invalid Key Pressed");
                                        //update_lcd_msg("INVALID_KEY",LINE2,LCD_CLR_FIRST_10_POSITION,0);
                                        //update_lcd_msg("DAIL_AGAIN ",LINE2,LCD_CLR_FIRST_10_POSITION,0);
                                         //send_msg_ui(CONFERENCE_STATE,LCD,LINE2,CONTINUE_DISPLAY,"INVALID_KEY",LCD_CLR_FIRST_10_POSITION,0);
                                         //send_msg_ui(CONFERENCE_STATE,LCD,LINE2,CONTINUE_DISPLAY,"DAIL_AGAIN",LCD_CLR_FIRST_10_POSITION,0);
                                        sleep(0.5);
                                        //update_lcd_msg("CONF",LINE2,LCD_CLR_FIRST_10_POSITION,0);
                                        //send_msg_ui(CONFERENCE_STATE,LCD,LINE2,CONTINUE_DISPLAY,"CONF",LCD_CLR_FIRST_10_POSITION,0);
                                        clear_validation_processing();
                                    }
                                    break;

                                case 3:
                                    if(cInkey >= DIGIT_0 && cInkey <= DIGIT_9)
                                    {
                                        g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = cInkey;
                                        strcat(g_arcLCDMsg, &g_iarcDialed_Key_Array[3]);
//                                        send_data_to_lcd(2, g_arcLCDMsg);
                                        printf("DIGIT2 Preessed key=%d\n",g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]);

                                        sprintf(&g_ConfLcdMsg[SEVEN],"%d",g_iarcDialed_Key_Array[g_ucDialed_Sequence_No]);
                                        //update_lcd_msg(g_ConfLcdMsg,LINE2,LCD_CLR_FIRST_10_POSITION,0);
                                        //send_msg_ui(CONFERENCE_STATE,LCD,LINE2,CONTINUE_DISPLAY,g_ConfLcdMsg,LCD_CLR_FIRST_10_POSITION,0);

                                        g_ucDialed_Sequence_No++;
                                        if(g_ucDialed_Sequence_No ==4)
                                        {
                                            printf("P2P calling......\n");
                                            g_iInterdigit_timer=CLEAR;
                                            g_valid_key_sequence = SET;
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        printf("Invalid Key!\n");
//                                        send_data_to_lcd(2, "Invalid Key Pressed");
                                         //update_lcd_msg("INVALID_KEY",LINE2,LCD_CLR_FIRST_10_POSITION,0);
                                         //update_lcd_msg("DAIL_AGAIN ",LINE2,LCD_CLR_FIRST_10_POSITION,0);
                                         //send_msg_ui(CONFERENCE_STATE,LCD,LINE2,CONTINUE_DISPLAY,"INVALID_KEY",LCD_CLR_FIRST_10_POSITION,0);
                                         //send_msg_ui(CONFERENCE_STATE,LCD,LINE2,CONTINUE_DISPLAY,"DAIL_AGAIN",LCD_CLR_FIRST_10_POSITION,0);
                                         sleep(0.5);
                                         //update_lcd_msg("CONF",LINE2,LCD_CLR_FIRST_10_POSITION,0);
                                         //send_msg_ui(CONFERENCE_STATE,LCD,LINE2,CONTINUE_DISPLAY,"CONF",LCD_CLR_FIRST_10_POSITION,0);

                                        clear_validation_processing();
                                    }
                                    break;
                            }

                        break;

                    default:break;
                    }

            break;

         case  UI_GROUP_STATE:

               switch(cInkey)
               {
                 case RESET_KEY:
                     g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = RESET;
                     g_ucDialed_Sequence_No++;
                     g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = cInkey;
                     g_valid_key_sequence = SET;
                     printf("RESET KEY PRESSED \n");
                break;

                case PTT_KEY:
                     printf("PTT_KEY KEY PRESSED \n");
                     g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = '0';
                     g_ucDialed_Sequence_No++;
                     g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = cInkey;
                     g_valid_key_sequence = SET;

                break;
               }

            break;


        case  UI_GROUP_CONTRIBUTOR_STATE:

            switch(cInkey)
            {
              case RESET_KEY:
                     g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = RESET;
                     g_ucDialed_Sequence_No++;
                     g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = cInkey;
                     g_valid_key_sequence = SET;
                     printf("RESET KEY PRESSED \n");
              break;
            }

            break;

        case  UI_ICOM_HOP_STATE:

               switch(cInkey)
               {
                 case RESET_KEY:
                     g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = RESET;
                     g_ucDialed_Sequence_No++;
                     g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = cInkey;
                     g_valid_key_sequence = SET;
                     printf("RESET KEY PRESSED \n");
                break;

                case PTT_KEY:
                     printf("PTT_KEY KEY PRESSED \n");
                     g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = '0';
                     g_ucDialed_Sequence_No++;
                     g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = cInkey;
                     g_valid_key_sequence = SET;

                break;
               }

            break;

         case  UI_PG_INITIATOR_STATE:

               switch(cInkey)
               {

                 case RESET_KEY:
                      printf("RESET KEY PRESSED \n");
                       g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = RESET;
                      g_ucDialed_Sequence_No++;
                      //clear_validation_processing();
                      g_iarcDialed_Key_Array[g_ucDialed_Sequence_No] = cInkey;
                      g_valid_key_sequence = SET;
                 break;
               }
            break;

        default:
            printf("Invalid Key!%d\n",c_Fsu_state);
//            send_data_to_lcd(2, "Invalid Key Pressed");
            clear_validation_processing();
            break;
     }
}

/*************************************************************************************************
 * Function name: keypad_report                                                                  *
 * Input arg    : pressed key,int                                                                *
 * Output arg   : NONE                                                                           *
 * Description  : This function send the valid key sequence via socket to voip_task process      *
 *                                                                                               *
**************************************************************************************************/

void keypad_report(keypad_key,pressed)
{
	int i=0,n;
	char buf[FIVE],call_type[FOUR];

    printf("pressed %d\n",pressed);

		if(pressed == EVENT_KEY_UP)
		{

            if(keypad_key != 52)
            {

                validate_key(keypad_key);

                if(g_valid_key_sequence == SET)
                {
                    printf("%d\n",g_iarcDialed_Key_Array[0]);
                    if(g_iarcDialed_Key_Array[0] > 0)
                    snprintf(call_type,FOUR,"%c",(char)g_iarcDialed_Key_Array[0]);//-'0');
                //   printf("call types is %c\n",call_type);
                    sprintf(buf,"%s%d%d%d%d",call_type,g_iarcDialed_Key_Array[1],g_iarcDialed_Key_Array[2],
                    g_iarcDialed_Key_Array[3],g_iarcDialed_Key_Array[4],g_iarcDialed_Key_Array[5]);
                    printf("sent data on socket is %s \n",buf);


                    n=send(g_iconnection_fd,buf,sizeof(buf),0);
                    if(n>0)
                    {
                        printf("successfully sent data \n");
                    }
                    else
                    {
                        perror("");
                        printf("failed to send data \n");
                    }
                        clear_validation_processing();

                }
            //keypad_key = 0;


            }
            else
            {
                printf("invalid Key Sequence - Dial Again from start \n");
                clear_validation_processing();
            }


        }

}










