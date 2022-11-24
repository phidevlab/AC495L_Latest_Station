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
**|         Copyright (c) 1998-2005 Texas Instruments Incorporated           |**
**|                        ALL RIGHTS RESERVED                               |**
**|                                                                          |**
**+--------------------------------------------------------------------------+**
*******************************************************************************/

/*******************************************************************************
 * FILE PURPOSE:    Flash operations header
 *******************************************************************************
 * FILE NAME:       flashop.h
 *
 * DESCRIPTION:     Flash operations header
 *
 ******************************************************************************/

#ifndef _FLASHOP_H_
#define _FLASHOP_H_

#ifdef _STANDALONE
#include <psbl/stddef.h>
#include <psbl/hw.h>
#else
#include "stddef.h"
#include "hw.h"
#endif

typedef int (*procref1)(bit32u base);
typedef int (*procref2)(bit32u base,int size);
typedef int (*procref3)(bit32u base);
typedef int (*procref4)(bit32u adr, bit8 cVal);
typedef int (*procref5)(void);
typedef int (*procref6)(bit32u base,int size,int verbose);
typedef unsigned int (*procref7)(bit32u base);

typedef struct _Adam2Flash
          {
          procref1 FWBGetBlockSize; /* called to get block size */
          procref6 FWBErase;        /* called to erase blocks */
          procref3 FWBOpen;         /* called to open block writes */
          procref4 FWBWriteByte;    /* called to write block byte */
          procref5 FWBClose;        /* called to close block writes */
          procref2 FWBUnLock;       /* called to close block writes */
          procref2 FWBLock;         /* called to close block writes */
          procref7 FWBGetNextBlock;  /* called to get base of the next blk */
          }Adam2Flash;



int FWBGetBlockSize(unsigned int base);           /*Get Flash Block Size            */
int FWBErase(unsigned int base,int size,int verbose); /*Erase flash                     */
int FWBOpen(unsigned int base);                   /*Prepare for flash writes        */
int FWBWriteByte(unsigned int adr, char cVal);    /*Write byte to flash             */
int FWBClose(void);                         /*Write any pending data to flash */
int FWBUnLock(unsigned int adr,int size);       /*Unlock Flash block(s)           */
int FWBLock(unsigned int from,int size);        /*Lock Flash block(s)             */
int GetFlashOps(Adam2Flash * ops);
int FWBGet_flash_type(void); /* Valid for dual flash boards */
unsigned long FWBGet_flash_size();
void acl_flash_init();
void acl_print_flash_info();
int FWBEraseAll(void);
#ifdef BE
void FWBReset(bit32u adr);
#endif
unsigned int FWBGetNextBlock(unsigned int base);  /* Get base addr of next block    */

/*NAND Flash operation functions*/
int FWBGetBlockSize_nand(unsigned int base);           /*Get Flash Block Size            */
int FWBErase_nand(unsigned int base,int size,int verbose); /*Erase flash                     */
int FWBOpen_nand(unsigned int base);                   /*Prepare for flash writes        */
int FWBWriteByte_nand(unsigned int adr, char cVal);    /*Write byte to flash             */
int FWBClose_nand(void);                         /*Write any pending data to flash */
int FWBUnLock_nand(unsigned int adr,int size);       /*Unlock Flash block(s)           */
int FWBLock_nand(unsigned int from,int size);        /*Lock Flash block(s)             */

int FWBGet_flash_type_nand(void); /* Valid for dual flash boards */
void FWBReset_nand(bit32u adr);
unsigned int FWBGetNextBlock_nand(unsigned int base);  /* Get base addr of next block    */


#ifdef _STANDALONE
#ifdef DUAL_FFS
extern unsigned int CS3_SIZE;
#else
#define CS3_SIZE _CS3_SIZE
#endif
#endif

#endif /* _FLASHOP_H_ */

