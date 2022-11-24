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
 * FILE PURPOSE:    MIPS processor definitions.
 *******************************************************************************
 * FILE NAME:       mips.h 
 *
 * DESCRIPTION:     MIPS processor definitions.
 *
 * (C) Copyright 2003, Texas Instruments, Inc
 ******************************************************************************/

#ifndef _MIPS_H_
#define _MIPS_H_


/************************************************************************
 *  Include files
 ************************************************************************/

#include "sysdefs.h"

/************************************************************************
 *  Definitions
*************************************************************************/

/* SDE compiler does not allow ".bss" */
#ifdef _SDE_
#define BSS	.section bss
#else
#define BSS	.bss
#endif  

/* CPU register file */
#define zero		$0
#define	AT		$1
#define	v0		$2
#define	v1		$3
#define	a0		$4
#define	a1		$5
#define	a2		$6
#define	a3		$7
#define	t0		$8
#define	t1		$9
#define	t2		$10
#define	t3		$11
#define	t4		$12
#define	t5		$13
#define	t6		$14
#define	t7		$15
#define ta0		$12
#define ta1		$13
#define ta2		$14
#define ta3		$15
#define	s0		$16
#define	s1		$17
#define	s2		$18
#define	s3		$19
#define	s4		$20
#define	s5		$21
#define	s6		$22
#define	s7		$23
#define	t8		$24
#define	t9		$25
#define	k0		$26
#define	k1		$27
#define	gp		$28
#define	sp		$29
#define	s8		$30
#define	fp		$30
#define	ra		$31


/* System Control Coprocessor (CP0) registers */

#define C0_INDEX	$0		/* TLB Index */
#define C0_RANDOM	$1		/* TLB Random */
#define C0_ENTRYLO0	$2		/* TLB EntryLo0 */
#define C0_ENTRYLO1	$3		/* TLB EntryLo1 */
#define C0_CONTEXT	$4		/* Context */
#define C0_PAGEMASK	$5		/* TLB PageMask */
#define C0_WIRED	$6		/* TLB Wired */
#define C0_BADVADDR	$8		/* Bad Virtual Address */
#define C0_COUNT 	$9		/* Count */
#define C0_ENTRYHI	$10		/* TLB EntryHi */
#define C0_COMPARE	$11		/* Compare */
#define C0_STATUS	$12		/* Processor Status */
#define C0_CAUSE	$13		/* Exception Cause */
#define C0_EPC		$14		/* Exception PC */
#define C0_PRID		$15		/* Processor Revision Indentifier */
#define C0_CONFIG	$16		/* Config */
#define C0_CONFIG1	$16		/* Config1 */
#define C0_LLADDR	$17		/* LLAddr */
#define C0_WATCHLO	$18		/* WatchpointLo */
#define C0_WATCHHI	$19		/* WatchpointHi */
#define C0_DEBUG        $23		/* EJTAG Debug Register */
#define C0_DEPC         $24		/* Program counter at last EJTAG debug exception */
#define C0_PERFCNT      $25		/* Performance counter interface */
#define C0_ERRCTL	$26		/* ECC */
#define C0_CACHEERR	$27		/* CacheErr */
#define C0_TAGLO	$28		/* TagLo */
#define C0_DATALO	$28		/* DataLo */
#define C0_TAGHI	$29		/* TagHi */
#define C0_DATAHI	$29		/* DataHi */
#define C0_ERROREPC	$30		/* ErrorEPC */
#define C0_DESAVE	$31		/* EJTAG dbg exc. save register */

/* System Control Coprocessor (CP0) registers select fields */

#define C0_INDEX_SEL	0		/* TLB Index */
#define C0_RANDOM_SEL	0		/* TLB Random */
#define C0_TLBLO0_SEL	0		/* TLB EntryLo0 */
#define C0_TLBLO1_SEL	0		/* TLB EntryLo1 */
#define C0_CONTEXT_SEL	0		/* Context */
#define C0_PAGEMASK_SEL	0		/* TLB PageMask */
#define C0_WIRED_SEL	0		/* TLB Wired */
#define C0_BADVADDR_SEL	0		/* Bad Virtual Address */
#define C0_COUNT_SEL	0		/* Count */
#define C0_ENTRYHI_SEL	0		/* TLB EntryHi */
#define C0_COMPARE_SEL	0		/* Compare */
#define C0_STATUS_SEL	0		/* Processor Status */
#define C0_CAUSE_SEL	0		/* Exception Cause */
#define C0_EPC_SEL	0		/* Exception PC */
#define C0_PRID_SEL	0		/* Processor Revision Indentifier */
#define C0_CONFIG_SEL	0		/* Config */
#define C0_CONFIG1_SEL	1		/* Config1 */
#define C0_LLADDR_SEL	0		/* LLAddr */
#define C0_WATCHLO_SEL	0		/* WatchpointLo */
#define C0_WATCHHI_SEL	0		/* WatchpointHi */
#define C0_DEBUG_SEL    0		/* EJTAG Debug Register */
#define C0_DEPC_SEL     0		/* Program counter at last EJTAG debug exception */
#define C0_PERFCNT_SEL  0		/* Performance counter interface */
#define C0_ERRCTL_SEL	0		/* ECC */
#define C0_CACHEERR_SEL	0		/* CacheErr */
#define C0_TAGLO_SEL	0		/* TagLo */
#define C0_DATALO_SEL	1		/* DataLo */
#define C0_TAGHI_SEL	0		/* TagHi */
#define C0_DATAHI_SEL	1		/* DataHi */
#define C0_ERROREPC_SEL	0		/* ErrorEPC */
#define C0_DESAVE_SEL	0		/* EJTAG dbg exc. save register */


/* C0_CONFIG register encoding */

#define C0_CONFIG_M_SHF			31
#define C0_CONFIG_M_WIDTH		1
#define C0_CONFIG_M_MSK     		(MSK(1) << C0_CONFIG_M_SHF)
#define C0_CONFIG_M_BIT			C0_CONFIG_M_MSK

#define C0_CONFIG_WC_SHF		19
#define C0_CONFIG_WC_WIDTH		1
#define C0_CONFIG_WC_MSK    		(MSK(1) << C0_CONFIG_WC_SHF)
#define C0_CONFIG_WC_BIT		C0_CONFIG_WC_MSK

#define C0_CONFIG_BE_SHF		15
#define C0_CONFIG_BE_WIDTH		1
#define C0_CONFIG_BE_MSK    		(MSK(1) << C0_CONFIG_BE_SHF)
#define C0_CONFIG_BE_BIT		C0_CONFIG_BE_MSK

#define C0_CONFIG_AT_SHF		13
#define C0_CONFIG_AT_MSK		(MSK(2) << C0_CONFIG_AT_SHF)
#define C0_CONFIG_AT_MIPS32		0
#define C0_CONFIG_AT_MIPS64_32ADDR	1
#define C0_CONFIG_AT_MIPS64		2

#define C0_CONFIG_AR_SHF		10
#define C0_CONFIG_AR_MSK		(MSK(3) << C0_CONFIG_AR_SHF)
#define C0_CONFIG_AR_REV1		0

#define C0_CONFIG_MT_SHF		7
#define C0_CONFIG_MT_MSK		(MSK(3) << C0_CONFIG_MT_SHF)
#define C0_CONFIG_MT_NONE		0
#define C0_CONFIG_MT_TLB		1
#define C0_CONFIG_MT_BAT		2
#define C0_CONFIG_MT_NON_STD		3

#define C0_CONFIG_K0_SHF		0
#define C0_CONFIG_K0_MSK		(MSK(3) << C0_CONFIG_K0_SHF)
#define C0_CONFIG_K0_WTHRU_NOALLOC	0
#define C0_CONFIG_K0_WTHRU_ALLOC	1
#define C0_CONFIG_K0_UNCACHED		2
#define C0_CONFIG_K0_NONCOHERENT	3
#define C0_CONFIG_K0_COHERENTXCL	4
#define C0_CONFIG_K0_COHERENTXCLW	5
#define C0_CONFIG_K0_COHERENTUPD	6
#define C0_CONFIG_K0_UNCACHED_ACCEL	7


/* C0_CONFIG1 register encoding */

#define C0_CONFIG1_MMUSIZE_SHF		25
#define C0_CONFIG1_MMUSIZE_MSK		(MSK(6) << C0_CONFIG1_MMUSIZE_SHF)

#define C0_CONFIG1_IS_SHF		22
#define C0_CONFIG1_IS_MSK		(MSK(3) << C0_CONFIG1_IS_SHF)

#define C0_CONFIG1_IL_SHF		19
#define C0_CONFIG1_IL_MSK		(MSK(3) << C0_CONFIG1_IL_SHF)

#define C0_CONFIG1_IA_SHF		16
#define C0_CONFIG1_IA_MSK		(MSK(3) << C0_CONFIG1_IA_SHF)

#define C0_CONFIG1_DS_SHF		13
#define C0_CONFIG1_DS_MSK		(MSK(3) << C0_CONFIG1_DS_SHF)

#define C0_CONFIG1_DL_SHF		10
#define C0_CONFIG1_DL_MSK		(MSK(3) << C0_CONFIG1_DL_SHF)

#define C0_CONFIG1_DA_SHF		7
#define C0_CONFIG1_DA_MSK		(MSK(3) << C0_CONFIG1_DA_SHF)

#define C0_CONFIG1_WR_SHF		3
#define C0_CONFIG1_WR_MSK		(MSK(1) << C0_CONFIG1_WR_SHF)
#define C0_CONFIG1_WR_BIT		C0_CONFIG1_WR_MSK

#define C0_CONFIG1_CA_SHF		2
#define C0_CONFIG1_CA_MSK		(MSK(1) << C0_CONFIG1_CA_SHF)
#define C0_CONFIG1_CA_BIT		C0_CONFIG1_CA_MSK

#define C0_CONFIG1_FP_SHF		0
#define C0_CONFIG1_FP_MSK		(MSK(1) << C0_CONFIG1_FP_SHF)
#define C0_CONFIG1_FP_BIT		C0_CONFIG1_FP_MSK

/* C0_STATUS register encoding */

#define C0_STATUS_CU3_SHF		31
#define C0_STATUS_CU3_MSK		(MSK(1) << C0_STATUS_CU3_SHF)
#define C0_STATUS_CU3_BIT		C0_STATUS_CU3_MSK

#define C0_STATUS_CU2_SHF		30
#define C0_STATUS_CU2_MSK		(MSK(1) << C0_STATUS_CU2_SHF)
#define C0_STATUS_CU2_BIT		C0_STATUS_CU2_MSK

#define C0_STATUS_CU1_SHF		29
#define C0_STATUS_CU1_MSK		(MSK(1) << C0_STATUS_CU1_SHF)
#define C0_STATUS_CU1_BIT		C0_STATUS_CU1_MSK

#define C0_STATUS_CU0_SHF		28
#define C0_STATUS_CU0_MSK		(MSK(1) << C0_STATUS_CU0_SHF)
#define C0_STATUS_CU0_BIT		C0_STATUS_CU0_MSK

#define C0_STATUS_RP_SHF		27
#define C0_STATUS_RP_MSK		(MSK(1) << C0_STATUS_RP_SHF)
#define C0_STATUS_RP_BIT		C0_STATUS_RP_MSK

#define C0_STATUS_RE_SHF		25
#define C0_STATUS_RE_MSK		(MSK(1) << C0_STATUS_RE_SHF)
#define C0_STATUS_RE_BIT		C0_STATUS_RE_MSK

#define C0_STATUS_BEV_SHF		22
#define C0_STATUS_BEV_MSK		(MSK(1) << C0_STATUS_BEV_SHF)
#define C0_STATUS_BEV_BIT		C0_STATUS_BEV_MSK

#define C0_STATUS_TS_SHF		21
#define C0_STATUS_TS_MSK		(MSK(1) << C0_STATUS_TS_SHF)
#define C0_STATUS_TS_BIT		C0_STATUS_TS_MSK

#define C0_STATUS_SR_SHF		20
#define C0_STATUS_SR_MSK		(MSK(1) << C0_STATUS_SR_SHF)
#define C0_STATUS_SR_BIT		C0_STATUS_SR_MSK

#define C0_STATUS_NMI_SHF		19
#define C0_STATUS_NMI_MSK		(MSK(1) << C0_STATUS_NMI_SHF)
#define C0_STATUS_NMI_BIT		C0_STATUS_NMI_MSK

#define C0_STATUS_IM_SHF		8
#define C0_STATUS_IM_MSK		(MSK(8) << C0_STATUS_IM_SHF)

#define C0_STATUS_UM_SHF		4
#define C0_STATUS_UM_MSK		(MSK(1) << C0_STATUS_UM_SHF)
#define C0_STATUS_UM_BIT		C0_STATUS_UM_MSK

#define C0_STATUS_ERL_SHF		2
#define C0_STATUS_ERL_MSK		(MSK(1) << C0_STATUS_ERL_SHF)
#define C0_STATUS_ERL_BIT		C0_STATUS_ERL_MSK

#define C0_STATUS_EXL_SHF		1
#define C0_STATUS_EXL_MSK		(MSK(1) << C0_STATUS_EXL_SHF)
#define C0_STATUS_EXL_BIT		C0_STATUS_EXL_MSK

#define C0_STATUS_IE_SHF		0
#define C0_STATUS_IE_MSK		(MSK(1) << C0_STATUS_IE_SHF)
#define C0_STATUS_IE_BIT		C0_STATUS_IE_MSK


/* C0_PRID register encoding */

#define C0_PRID_OPT_SHF			24
#define C0_PRID_OPT_MSK			(MSK(8) << C0_PRID_OPT_SHF)

#define C0_PRID_COMP_SHF		16
#define C0_PRID_COMP_MSK		(MSK(8) << C0_PRID_COMP_SHF)
#define C0_PRID_COMP_MIPS		0x1

#define C0_PRID_PRID_SHF		8
#define C0_PRID_PRID_MSK		(MSK(8) << C0_PRID_PRID_SHF)
/* JADE */
#define C0_PRID_PRID_4Kc		0x80
#define C0_PRID_PRID_4Km		0x83
#define C0_PRID_PRID_4Kp		0x83
/* OPAL */
#define C0_PRID_PRID_5Kc		0x81

#define C0_PRID_PRID_R4000  		0x04
#define C0_PRID_PRID_RM52XX  		0x28        /* QED RM52XX */


#define C0_PRID_REV_SHF			0
#define C0_PRID_REV_MSK			(MSK(8) << C0_PRID_REV_SHF)

/* Memory controller */
#define MEMCTRL_MAJREV_SHF              8
#define MEMCTRL_MAJREV_MSK              (MSK(8) << MEMCTRL_MAJREV_SHF)

#define MEMCTRL_MINREV_SHF              8
#define MEMCTRL_MINREV_MSK              (MSK(8) << MEMCTRL_MINREV_SHF)


/* C0_ENTRYHI register encoding */

#define C0_ENTRYHI_VPN2_SHF		13
#define C0_ENTRYHI_VPN2_MSK		(MSK(19) << C0_ENTRYHI_VPN2_SHF)

#define C0_ENTRYHI_ASID_SHF		0
#define C0_ENTRYHI_ASID_MSK		(MSK(8) << C0_ENTRYHI_ASID_SHF)


/* C0_CAUSE register encoding */

#define C0_CAUSE_BD_SHF			31
#define C0_CAUSE_BD_MSK			(MSK(1) << C0_CAUSE_BD_SHF)
#define C0_CAUSE_BD_BIT			C0_CAUSE_BD_MSK

#define C0_CAUSE_CE_SHF			28
#define C0_CAUSE_CE_MSK			(MSK(2) << C0_CAUSE_CE_SHF)

#define C0_CAUSE_IV_SHF			23
#define C0_CAUSE_IV_MSK			(MSK(1) << C0_CAUSE_IV_SHF)
#define C0_CAUSE_IV_BIT			C0_CAUSE_IV_MSK

#define C0_CAUSE_WP_SHF			22
#define C0_CAUSE_WP_MSK			(MSK(1) << C0_CAUSE_WP_SHF)
#define C0_CAUSE_WP_BIT			C0_CAUSE_WP_MSK


#define C0_CAUSE_IP_SHF			8
#define C0_CAUSE_IP_MSK			(MSK(8) << C0_CAUSE_IP_SHF)

#define C0_CAUSE_CODE_SHF		2
#define C0_CAUSE_CODE_MSK		(MSK(5) << C0_CAUSE_CODE_SHF)
#define C0_CAUSE_CODE_INT		0
#define C0_CAUSE_CODE_MOD		1
#define C0_CAUSE_CODE_TLBL		2
#define C0_CAUSE_CODE_TLBS		3
#define C0_CAUSE_CODE_ADEL		4
#define C0_CAUSE_CODE_ADES		5
#define C0_CAUSE_CODE_IBE		6
#define C0_CAUSE_CODE_DBE		7
#define C0_CAUSE_CODE_SYS		8
#define C0_CAUSE_CODE_BP		9
#define C0_CAUSE_CODE_RI		10
#define C0_CAUSE_CODE_CPU		11
#define C0_CAUSE_CODE_OV		12
#define C0_CAUSE_CODE_TR		13
#define C0_CAUSE_CODE_FPE		15
#define C0_CAUSE_CODE_WATCH		23
#define C0_CAUSE_CODE_MCHECK		24


/* cache operations */

#define CACHE_OP( code, type )			( ((code) << 2) | (type) )

#define ICACHE_INDEX_INVALIDATE			CACHE_OP(0x0, 0)
#define ICACHE_INDEX_LOAD_TAG			CACHE_OP(0x1, 0)
#define ICACHE_INDEX_STORE_TAG			CACHE_OP(0x2, 0)
#define DCACHE_INDEX_WRITEBACK_INVALIDATE	CACHE_OP(0x0, 1)
#define DCACHE_INDEX_LOAD_TAG			CACHE_OP(0x1, 1)
#define DCACHE_INDEX_STORE_TAG			CACHE_OP(0x2, 1)

#define ICACHE_ADDR_HIT_INVALIDATE		CACHE_OP(0x4, 0)
#define ICACHE_ADDR_FILL			CACHE_OP(0x5, 0)
#define ICACHE_ADDR_FETCH_LOCK			CACHE_OP(0x7, 0)
#define DCACHE_ADDR_HIT_INVALIDATE		CACHE_OP(0x4, 1)
#define DCACHE_ADDR_HIT_WRITEBACK_INVALIDATE	CACHE_OP(0x5, 1)
#define DCACHE_ADDR_HIT_WRITEBACK		CACHE_OP(0x6, 1)
#define DCACHE_ADDR_FETCH_LOCK			CACHE_OP(0x7, 1)


/* instructions for 4Kc core */
/*#define eret                                  0x42000018*/
#define DCACHEADDRHITWRITEBACKA0              0xbc990000
#define DCACHEADDRHITWRITEBACKINVALIDATEA0    0xbc950000      
#define ICACHEINDEXINVALIDATEA0               0xbc800000
#define ICACHEADDRHITINVALIDATEA0             0xbc980000
#define DCACHEADDRHITWRITEBACKT1              0xbd390000
#define ICACHEADDRHITINVALIDATET1             0xbd300000
#define ICACHEINDEXSTORETAGA2                 0xbcc80000
#define DCACHEINDEXSTORETAGA2                 0xbcc90000
#define DCACHEINDEXWRITEBACKINVALIDATEA0      0xbc810000

#define NOPS	nop; nop; nop


#define MFLO(dst)\
		mflo dst;\
 	  	NOPS

#define MTC0(src, dst)\
	        mtc0 src,dst;\
		NOPS

#define MFC0(dst, src)\
	  	mfc0 dst,src;\
		NOPS

#define MFC0_SEL_OPCODE(dst, src, sel)\
	  	.word (0x40000000 | ((dst)<<16) | ((src)<<11) | (sel));\
		NOPS

#define MTC0_SEL_OPCODE(dst, src, sel)\
	  	.word (0x40800000 | ((dst)<<16) | ((src)<<11) | (sel));\
		NOPS


/* Instruction opcode fields */
#define OPC_SPECIAL   0x0
#define OPC_REGIM     0x1
#define OPC_J         0x2
#define OPC_JAL	      0x3
#define OPC_BEQ	      0x4
#define OPC_BNE	      0x5
#define OPC_BLEZ      0x6
#define OPC_BGTZ      0x7
#define OPC_BEQL      0x14
#define OPC_BNEL      0x15
#define OPC_BLEZL     0x16
#define OPC_BGTZL     0x17
#define OPC_LUI       15
#define OPC_ORI       13

/* Instruction function fields */
#define FUNC_JR	      0x8
#define FUNC_JALR     0x9

/* Instruction rt fields */
#define RT_BLTZ	      0x0
#define RT_BGEZ	      0x1
#define RT_BLTZL      0x2
#define RT_BGEZL      0x3
#define RT_BLTZAL     0x10
#define RT_BGEZAL     0x11
#define RT_BLTZALL    0x12
#define RT_BGEZALL    0x13

/* Access macros for instruction fields */
#define MIPS_OPCODE( instr)	((instr) >> 26)
#define MIPS_FUNCTION(instr)	((instr) & MSK(6))
#define MIPS_RT(instr)		(((instr) >> 16) & MSK(5))
#define MIPS_RS(instr)		(((instr) >> 21) & MSK(5))
#define MIPS_OFFSET(instr)	((instr) & 0xFFFF)
#define MIPS_TARGET(instr)	((instr) & MSK(26))

/* Instructions */
#define OPCODE_BREAK	  	0x0005000d
#define OPCODE_NOP		0
#define OPCODE_JUMP(addr)	( (OPC_J << 26) | (((addr) >> 2) & 0x3FFFFFF) )
#define OPCODE_LUI(addr,rd)     ( (OPC_LUI << 26) | (rd << 16) | (((addr) >> 16) & 0xffff) )
#define OPCODE_ORI(addr,rd)     ( (OPC_ORI << 26) | (rd << 21) | (rd << 16) | ((addr) & MSK(16)) )
#define OPCODE_JR(rs)           ( (OPC_SPECIAL) | (rs << 21) | (FUNC_JR) )


/* Config1 cache field decoding */
#define CACHE_CALC_SPW(s)	( 64 << (s) )
#define CACHE_CALC_LS(l)	( (l) ? 2 << (l) : 0 )
#define CACHE_CALC_BPW(l,s)	( CACHE_CALC_LS(l) * CACHE_CALC_SPW(s) )
#define CACHE_CALC_ASSOC(a)	( (a) + 1 )

/* Useful TLB defines */
#define TLB_PAGESIZE_4KB	0x000
#define TLB_PAGESIZE_16KB	0x003
#define TLB_PAGESIZE_64KB	0x00F
#define TLB_PAGESIZE_256KB	0x03F
#define TLB_PAGESIZE_1MB	0x0FF
#define TLB_PAGESIZE_4MB	0x3FF
#define TLB_PAGESIZE_16MB	0xFFF

#define TLB_FLAGS_GLOBAL	0x0001
#define TLB_FLAGS_VALID		0x0002
#define TLB_FLAGS_WRITEABLE	0x0004
#define TLB_FLAGS_CACHEABLE	0x0008

#define NTLBID	16

#endif /* #ifndef _MIPS_H_ */









