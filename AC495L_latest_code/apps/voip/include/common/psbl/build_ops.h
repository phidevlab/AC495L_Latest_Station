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
 * FILE PURPOSE:    Environment variables header file.
 *******************************************************************************
 * FILE NAME:       build_ops.h
 *
 * DESCRIPTION:     Build operations header file.
 *
 * (C) Copyright 2004, Texas Instruments, Inc
 ******************************************************************************/

#ifndef _BLDOPS_H_
#define	_BLDOPS_H_

#define BLDOPS_TI_BINARY             0x00000001
#define BLDOPS_ELF                   0x00000002
#define BLDOPS_GZIP                  0x00000004
#define BLDOPS_7ZIP                  0x00000008
#define BLDOPS_LZMA                  0x00000010
#define BLDOPS_FFS                   0x00000020
#define BLDOPS_TFTP                  0x00000040
#define BLDOPS_FTP                   0x00000080
#define BLDOPS_DHCP                  0x00000100
#define BLDOPS_PCAPP                 0x00000200
#define BLDOPS_DUAL_IMAGE            0x00000400
#define BLDOPS_TFTP_FAILOVER         0x00000800
#define BLDOPS_NAND_SUPPORT          0x00001000

#endif
