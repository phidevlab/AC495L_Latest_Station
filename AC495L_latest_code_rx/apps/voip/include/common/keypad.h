/*******************************************************************************
 * FILE PURPOSE:    Keypad Module Header file
 *******************************************************************************
 * FILE NAME:       keypad.h
 *
 * DESCRIPTION:     Header file for Linux Keypad Driver
 *
 * REVISION HISTORY:
 *
 * Date           Description                               Author
 *-----------------------------------------------------------------------------
 * 27 Aug 2003    Initial Creation                          Sharath Kumar
 *
 * 16 Dec 2003    Updates for 5.7                           Sharath Kumar
 *
 * (C) Copyright 2003, Texas Instruments, Inc
 ******************************************************************************/
#ifndef _TI_KEYPAD_H_
#define _TI_KEYPAD_H_


/*Mark*/
typedef struct key_event {
    int row;  /* Stores the row  number of the key   */
    int column;  /* Stores the column number of the key */
    int key_press_time;  /* This is a flag to indicate whether the.
                          * event was key-press or key-release event.
                          * For key-press event this will contain 0.
                          * For key-release event this will contain the
                          * duration(in milisec) for which the last key was pressed.
                          */
}KEY_EVENT;


/* Defines for IOCTLs */
#define TI_KEY_DEBOUNCE_VALUE    1

/* KEYPAD config parameters */
#ifdef TNETV1050SDB
#define COLUMN_MAP             0x003F
#define ROW_MAP                0x0F00
#endif

#ifdef TNETV1050RDB
#define COLUMN_MAP             0x00FF
#define ROW_MAP                0x0F00
#endif

#ifdef TNETV1051SDB
#define COLUMN_MAP             0x00FF
#define ROW_MAP                0xFF00
#endif

#define DEBOUNCE_TIME             (20) /* in milsecs */
#define BUFFER_SIZE               (256)


#endif /* _TI_KEYPAD_H_ */
