/*******************************************************************************
**+--------------------------------------------------------------------------+**
**|                            ****                                          |**
**|                            ****                                          |**
**|                            ******o***                                    |**
**|                      ********_///_****                                   |**
**|                      ***** /_//_/ ****                                   |**
**|                       ** ** (__/ ****                                    |**
**|                           *********                                      |**
**|                            ****                                          |**
**|                            ***                                           |**
**|                                                                          |**
**|         Copyright (c) 1998-2003 Texas Instruments Incorporated           |**
**|                        ALL RIGHTS RESERVED                               |**
**|                                                                          |**
**| Permission is hereby granted to licensees of Texas Instruments           |**
**| Incorporated (TI) products to use this computer program for the sole     |**
**| purpose of implementing a licensee product based on TI products.         |**
**| No other rights to reproduce, use, or disseminate this computer          |**
**| program, whether in part or in whole, are granted.                       |**
**|                                                                          |**
**| TI makes no representation or warranties with respect to the             |**
**| performance of this computer program, and specifically disclaims         |**
**| any responsibility for any damages, special or consequential,            |**
**| connected with the use of this program.                                  |**
**|                                                                          |**
**+--------------------------------------------------------------------------+**
*******************************************************************************/

/*******************************************************************************
 * FILE PURPOSE:    System definitions
 *******************************************************************************
 * FILE NAME:       system.h
 *
 * DESCRIPTION:     PSBL system header file, defining system specific params
 *                  like int/long
 *
 * (C) Copyright 2003, Texas Instruments, Inc
 ******************************************************************************/

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

typedef unsigned int   u4;
typedef unsigned short u2;
typedef unsigned char  u1;
typedef signed int     s4;
typedef signed char    s1;
typedef signed short   s2;

/*
 * Register Interface routines:
 * All register interfaces should use these.
 *
 * Macros supported for:
 * - mask   : Mask creation with bit fields,
 * - reg_r  :  register read, with bit manipulation,
 * - reg_w  :  register write, invalidation old contents,
 * - reg_rmw:  register read-modify-write, updating only a defined
 *            location of the register.
 *
 * {hi, lo} set defines the bit locations in a register.
 */

#define MASK(hi, lo) (((~(~0 << (hi))) & (~0 << (lo))) | (1 << (hi)))

#define REG32_R(addr, hi, lo) \
        			 ((*(volatile u4 *)(addr) & MASK((hi), (lo))) >> (lo))

#define REG32_W(addr, val) *(volatile u4 *)(addr) =  val

#define REG32_RMW(addr, hi, lo, val)\
        *(volatile u4 *)(addr) =    \
        ((*(volatile u4 *)(addr) & ~MASK((hi), (lo))) | ((val) << (lo)))

#endif /* _SYSTEM_H_ */
