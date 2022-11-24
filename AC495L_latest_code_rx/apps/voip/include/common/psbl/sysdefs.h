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
 * FILE NAME:       sysdefs.h
 *
 * DESCRIPTION:     System definitions
 *
 * (C) Copyright 2003, Texas Instruments, Inc
 ******************************************************************************/

/************************************************************************
 *
 *  sysdefs.h
 *
 *  System definitions
 *
 * ######################################################################
 *
 * Copyright (C) 1999 MIPS Technologies, Inc. All rights reserved.
 *
 * Unpublished rights reserved under the Copyright Laws of the United
 * States of America.
 *
 * This document contains information that is confidential and proprietary
 * to MIPS Technologies, Inc. and may be disclosed only to those employees
 * of MIPS Technologies, Inc. with a need to know, or as otherwise
 * permitted in writing by MIPS Technologies, Inc. or a
 * contractually-authorized third party. Any copying, modifying, use or
 * disclosure of this information (in whole or in part) which is not
 * expressly permitted in writing by MIPS Technologies, Inc. or a
 * contractually-authorized third party is strictly prohibited. At a
 * minimum, this information is protected under trade secret and unfair
 * competition laws and the expression of the information contained herein
 * is protected under federal copyright laws. Violations thereof may
 * result in criminal penalties and fines.
 *
 * MIPS Technologies, Inc. or any contractually-authorized third party
 * reserves the right to change the information contained in this document
 * to improve function, design or otherwise. MIPS Technologies, Inc. does
 * not assume any liability arising out of the application or use of this
 * information. Any license under patent rights or any other intellectual
 * property rights owned by MIPS Technologies or third parties shall be
 * conveyed by MIPS Technologies, Inc. or any contractually-authorized
 * third party in a separate license agreement fully executed by the
 * parties.
 *
 * The information contained in this document constitutes "Commercial
 * Computer Software" or "Commercial Computer Software Documentation," as
 * described in FAR 12.212 for civilian agencies, and DFARS 227.7202 for
 * military agencies. This information may only be disclosed to the U.S.
 * Government, or to U.S. Government users, with prior written consent from
 * MIPS Technologies, Inc. or a contractually-authorized third party. Such
 * disclosure to the U.S. Government, or to U.S. Government users, shall
 * be subject to license terms and conditions at least as restrictive and
 * protective of the confidentiality of this information as the terms and
 * conditions used by MIPS Technologies, Inc. in its license agreements
 * covering this information.
 *
 ************************************************************************/


#ifndef SYSDEFS_H
#define SYSDEFS_H


/************************************************************************
 *  Include files
 ************************************************************************/

/************************************************************************
 *  Definitions
*************************************************************************/

#ifdef _ASSEMBLER_

/******** ASSEMBLER SPECIFIC DEFINITIONS ********/

#define SWAPEND32( src, tmp0, tmp1 )\
		and	tmp0, src, 0xff;\
		srl	src,  8;\
		sll	tmp0, 8;\
		and	tmp1, src, 0xff;\
		or	tmp0, tmp1;\
		srl	src,  8;\
		sll	tmp0, 8;\
		and	tmp1, src, 0xff;\
		or	tmp0, tmp1;\
		srl	src,  8;\
		sll	tmp0, 8;\
		or	src,  tmp0


#define KSEG0A(reg)\
	  	and reg, ~KSEG_MSK;\
	  	or  reg, KSEG0BASE;

#define KSEG1A(reg)\
	  	and reg, ~KSEG_MSK;\
	  	or  reg, KSEG1BASE;

#define KUSEGA(reg)\
		and reg, ~KSEG_MSK

#define LEAF(name)\
  		.text;\
  		.globl	name;\
  		.ent	name;\
name:


#define SLEAF(name)\
  		.text;\
  		.ent	name;\
name:


#define END(name)\
  		.size name,.-name;\
  		.end	name


#define EXTERN(name)

#else

/******** C specific definitions ********/

#define SWAPEND32(d)    ((BYTE(d,0) << 24) |\
		         (BYTE(d,1) << 16) |\
		         (BYTE(d,2) << 8)  |\
		         (BYTE(d,3) << 0))

#include <psbl/stddef.h>
/*#  ifndef _SIZE_T_                                  */
/*#    define _SIZE_T_        __typeof(sizeof(int))   */
/*     typedef _SIZE_T_ size_t;                       */
/*#  endif                                            */
#endif /* #ifdef _ASSEMBLER_ */



/******** DEFINITIONS FOR BOTH ASSEMBLER AND C ********/


#ifndef FALSE
#define FALSE		          0
#endif

#ifndef TRUE
#define TRUE			  (!FALSE)
#endif

#ifndef NULL
#define NULL		          ((void *)0)
#endif

#define MIN(x,y)		      ((x) < (y) ? (x) : (y))
#define MAX(x,y)      		  ((x) > (y) ? (x) : (y))
 
#define INCWRAP( ptr, size )      (ptr) = ((ptr)+1) % (size)
#define DECWRAP( ptr, size )      (ptr) = (((ptr) == 0) ? ((size) - 1) : ((ptr) - 1))

#define MAXUINT(w)	(\
		((w) == sizeof(UINT8))  ? 0xFFU :\
		((w) == sizeof(UINT16)) ? 0xFFFFU :\
		((w) == sizeof(UINT32)) ? 0xFFFFFFFFU : 0\
		        )

#define MAXINT(w)	(\
		((w) == sizeof(INT8))  ? 0x7F :\
		((w) == sizeof(INT16)) ? 0x7FFF :\
		((w) == sizeof(INT32)) ? 0x7FFFFFFF : 0\
		        )

#define MSK(n)			  ((1 << (n)) - 1)

#define KSEG_MSK		  0xE0000000
#define KSEG0BASE		  0x80000000
#define KSEG1BASE		  0xA0000000

#ifdef _ASSEMBLER_
#define KSEG1(addr)               (((addr) & ~KSEG_MSK) | KSEG1BASE)
#define KSEG0(addr)               (((addr) & ~KSEG_MSK) | KSEG0BASE)
#define KUSEG(addr)               ( (addr) & ~KSEG_MSK)
#else
#define KSEG1(addr)               (((UINT32)(addr) & ~KSEG_MSK) | KSEG1BASE)
#define KSEG0(addr)               (((UINT32)(addr) & ~KSEG_MSK) | KSEG0BASE)
#define KUSEG(addr)               ( (UINT32)(addr) & ~KSEG_MSK)
#endif

#define UNCACHED(addr)		  KSEG1(addr)
#define CACHED(addr)		  KSEG0(addr)
#define PHYS(addr)		      KUSEG(addr)
#define BUS(addr)             ((unsigned int)(addr) & ~0xE0000000)

#define REG8( addr )		  (*(volatile UINT8 *) (addr))
#define REG16( addr )		  (*(volatile UINT16 *)(addr))
#define REG32( addr )		  (*(volatile UINT32 *)(addr))

#define KSEG_MASK                   0xE0000000
#ifndef KSEG1_BASE
#define KSEG1_BASE                  0xA0000000
#endif

#ifndef PHYS_ADDR
#define PHYS_ADDR(addr)             ((addr) & ~KSEG_MASK)
#endif

#ifndef PHYS_TO_K1
#define PHYS_TO_K1(addr)            (PHYS_ADDR(addr)|KSEG1_BASE)
#endif


#define REG32_ADDR(addr)                (volatile UINT32 *)(PHYS_TO_K1(addr))
#define REG32_DATA(addr)                (*(volatile UINT32 *)(PHYS_TO_K1(addr)))
#define REG32_WRITE(addr, data)     REG32_DATA(addr) = data;
#define REG32_READ(addr, data)      data = (UINT32) REG32_DATA(addr);

#if 0
#define REG64( addr )		  (*(volatile UINT64 *)(addr))
#endif

/* Register field mapping */
#define REGFIELD(reg, rfld)	  (((reg) & rfld##_MSK) >> rfld##_SHF)

/* absolute register address, access 					*/
#define	REGA(addr)	(*((volatile UINT32 *) (addr)))

/* register read field							*/
#define	REGARD(addr,fld)	((REGA(addr) & addr##_##fld##_##MSK) 	\
			 >> addr##_##fld##_##SHF)

/* register write numeric field value					*/
#define	REGAWRI(addr,fld,intval) ((REGA(addr) & ~(addr##_##fld##_##MSK))\
				 | ((intval) << addr##_##fld##_##SHF))

/* register write enumerated field value				*/
#define	REGAWRE(addr,fld,enumval) ((REGA(addr) & ~(addr##_##fld##_##MSK))\
			| ((addr##_##fld##_##enumval) << addr##_##fld##_##SHF))


/* Examples:
 * 
 *	exccode = REGARD(CPU_CAUSE,EXC);
 *
 *	REGA(SDR_CONTROL) = REGAWRI(OSG_CONTROL,TMO,17)
 *			 | REGAWRE(OSG_CONTROL,DTYPE,PC1);
 */


/* relative register address, access: base address + offsett		*/
#define	REG(base,offs)	(*((volatile UINT32 *)((UINT32)(base) + offs##_##OFS) ))

/* register read field							*/
#define	REGRD(base,offs,fld) ((REG(base,offs) & offs##_##fld##_##MSK) 	\
			 >> offs##_##fld##_##SHF)

/* register write numeric field value					*/
#define	REGWRI(base,offs,fld,intval)((REG(base,offs)& ~(offs##_##fld##_##MSK))\
				 | ((intval) << offs##_##fld##_##SHF))

/* register write enumerated field value				*/
#define	REGWRE(base,offs,fld,enumval)((REG(base,offs) & ~(offs##_##fld##_##MSK))\
				| ((offs##_##fld##_##enumval) << offs##_##fld##_##SHF))


/* physical register address, access: base address + offsett		*/
#define	REGP(base,phys)	(*((volatile UINT32 *)((UINT32)(base) + (phys)) ))

/* physical register read field							*/
#define	REGPRD(base,phys,fld) ((REGP(base,phys) & phys##_##fld##_##MSK) 	\
			 >> phys##_##fld##_##SHF)

/* physical register write numeric field value					*/
#define	REGPWRI(base,phys,fld,intval)((REGP(base,phys)& ~(phys##_##fld##_##MSK))\
				 | ((intval) << phys##_##fld##_##SHF))

/* physical register write enumerated field value				*/
#define	REGPWRE(base,phys,fld,enumval)((REGP(base,phys) & ~(phys##_##fld##_##MSK))\
				| ((phys##_##fld##_##enumval) << phys##_##fld##_##SHF))


/* Endian related macros */

#define SWAP_BYTEADDR32( addr )	  ( ((addr) & ~0x3) | (3 - ((addr) & 0x3)) )
#define BYTE(d,n)		  (((d) >> ((n) << 3)) & 0xFF)

#ifdef EL
#define REGW32LE(addr, data)      REG32(addr) = (data)
#define REGR32LE(addr, data)      (data)      = REG32(addr)
#else
#define REGW32LE(addr, data)      REG32(addr) = SWAPEND32(data)
#define REGR32LE(addr, data)      (data)      = REG32(addr), (data) = SWAPEND32(data)
#endif

#define REGW8LE(addr, data)       REG8(addr) = (data)
#define REGR8LE(addr, data)       (data)     = REG32(addr)


/* Set of 'LE'-macros, convert by BE: */
#ifdef EL
#define CPU_TO_LE32( value ) (value)
#define LE32_TO_CPU( value ) (value)

#define CPU_TO_LE16( value ) (value)
#define LE16_TO_CPU( value ) (value)
#else
#define CPU_TO_LE32( value ) ( (                ((UINT32)value)  << 24) |   \
                               ((0x0000FF00UL & ((UINT32)value)) <<  8) |   \
                               ((0x00FF0000UL & ((UINT32)value)) >>  8) |   \
                               (                ((UINT32)value)  >> 24)   )
#define LE32_TO_CPU( value ) CPU_TO_LE32( value )

#define CPU_TO_LE16( value ) ( ((UINT16)(((UINT16)value)  << 8)) |   \
                               ((UINT16)(((UINT16)value)  >> 8))   )
#define LE16_TO_CPU( value ) CPU_TO_LE16( value )
#endif

/* Set of 'BE'-macros, convert by LE: */
#ifdef EB
#define CPU_TO_BE32( value ) (value)
#define BE32_TO_CPU( value ) (value)

#define CPU_TO_BE16( value ) (value)
#define BE16_TO_CPU( value ) (value)
#else
#define CPU_TO_BE32( value ) ( (                ((UINT32)value)  << 24) |   \
                               ((0x0000FF00UL & ((UINT32)value)) <<  8) |   \
                               ((0x00FF0000UL & ((UINT32)value)) >>  8) |   \
                               (                ((UINT32)value)  >> 24)   )
#define BE32_TO_CPU( value ) CPU_TO_BE32( value )

#define CPU_TO_BE16( value ) ( ((UINT16)(((UINT16)value)  << 8)) |   \
                               ((UINT16)(((UINT16)value)  >> 8))   )
#define BE16_TO_CPU( value ) CPU_TO_BE16( value )
#endif


/* Standard ports */
#define PORT_TTY0	0
#define PORT_TTY1	1
#define PORT_NET	2
#define DEFAULT_PORT	PORT_TTY0
#define DEBUG_PORT	PORT_TTY0
#define GDB_PORT	PORT_TTY1

/* Control characters */
#define CTRL_A          ('A'-0x40)
#define CTRL_B          ('B'-0x40)
#define CTRL_C          ('C'-0x40)
#define CTRL_D          ('D'-0x40)
#define CTRL_E          ('E'-0x40)
#define CTRL_F          ('F'-0x40)
#define CTRL_H          ('H'-0x40)
#define CTRL_K          ('K'-0x40)
#define CTRL_N          ('N'-0x40)
#define CTRL_P          ('P'-0x40)
#define CTRL_U          ('U'-0x40)
#define DEL             0x7F
#define TAB             0x09
#define CR              0x0D
#define LF              0x0A
#define ESC             0x1B
#define SP              0x20
#define CSI             0x9B
#define EOF_SREC        -2
#define UART_ERROR      -1


/************************************************************************
 *  Public variables
 ************************************************************************/

/************************************************************************
 *  Public functions
 ************************************************************************/

#endif /* #ifndef SYSDEFS_H */



