/*--------------------------------------------------------------------
 *  
 * 	Copyright (c) 2006 Outline Electronics Limited.
 *  
 * 	All rights reserved. Reproduction in whole 
 * 	or in part is prohibited without the written
 * 	permission of the copyright owner.
 *--------------------------------------------------------------------
 *--------------------------------------------------------------------
 * FILE:   regdump.h
 *
 * DESCRIPTION:
 *   header file for regdump.c
 *
 *--------------------------------------------------------------------*/

#ifndef __REGDUMP_H__
#define __REGDUMP_H__

typedef struct {
	unsigned char readcmd;
	unsigned char datalen;
} MpiReadCmdType;

void DoDumpRegs(unsigned char ecVal);
void DoDumpSingleReg(unsigned char ecVal, unsigned char cmd_register);
void DoWriteDumpSingleReg(unsigned char ecVal, unsigned char cmd_register, unsigned char* pcmdData);

int FindCmdLen(unsigned char cmd);

#endif /* __REGDUMP_H__ */

