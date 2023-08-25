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
**|         Copyright (c) 1998-2004 Texas Instruments Incorporated           |**
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
 * FILE PURPOSE:    7-ZIP configuration file
 *******************************************************************************
 * FILE NAME:       7zip_conf.h
 *
 * DESCRIPTION:     7-ZIP configuration file
 *
 * (C) Copyright 2004, Texas Instruments, Inc
 ******************************************************************************/

#ifndef ZIP7_CONF_H
#define ZIP7_CONF_H


/* Configuration of 7-ZIP uncompressed image start address.
   The user must update this value according to the link address of the image.
   
   The 7-ZIP decompressor will write the image to the following address: */
#define ZIP7_START_ADDR    0x94020000

#endif /* OMOD_LZMA_H */
