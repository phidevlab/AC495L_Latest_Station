/*--------------------------------------------------------------------
 *  
 * 	Copyright (c) 2006 Outline Electronics Limited.
 *  
 * 	All rights reserved. Reproduction in whole 
 * 	or in part is prohibited without the written
 * 	permission of the copyright owner.
 *--------------------------------------------------------------------
 *--------------------------------------------------------------------
 * FILE:   regdump.c
 *
 * DESCRIPTION:
 *   dump the registers
 *
 *--------------------------------------------------------------------*/

#include <linux/module.h>
#include <linux/types.h>
#include <linux/fcntl.h>



#include "vp_api.h"
#include "profiles8.h"
#include "regdump.h"



#ifndef acl_min
#define acl_min(x, y)	(((x) < (y)) ? (x) : (y))
#endif


#ifndef acl_max
#define acl_max(x, y)	(((x) < (y)) ? (y) : (x))
#endif

/* the command list for read operations */
const MpiReadCmdType RegCmdList[] = {
	{0x02,  1}, {0x04,  1}, {0x41,  1}, {0x43,  1}, 
	{0x45,  1}, {0x47,  1},	{0x4B,  1}, {0x4D,  2}, 
	{0x4F,  2}, {0x51,  1},	{0x53,  1}, {0x55,  1}, 
	{0x57,  1}, {0x61,  1},	{0x69,  1}, {0x6D,  2}, 
	{0x71,  1}, {0x73,  2}, {0x81,  2}, {0x83,  2}, 
	{0x87, 14}, {0x89, 12}, {0x8B, 14}, {0x97,  2}, 
	{0x99, 10}, {0x9B,  5}, {0xA7,  1}, {0xC3,  4}, 
	{0xC7,  2}, {0xCB,  1}, {0xCD,  2},	{0xD1,  4}, 
	{0xD3, 11}, {0xD5,  8}, {0xDF,  1},	{0xE1,  4}, 
	{0xE3,  1}, {0xE5,  3}, {0xE7,  1},	{0xEB,  1}, 
	{0x0, 0}
};

/*--------------------------------------------------------------------
 * ROUTINE:  DoDumpRegs
 *
 * DESCRIPTION:
 *    function to dump all the register values
 *
 * ARGUMENTS:
 *    void
 *
 * RESULTS:
 *   return void
 *--------------------------------------------------------------------*/
void DoDumpRegs(unsigned char ecVal)
{
	int i, j;
	unsigned char cmdData[256];
	printk("Dumping all registers: (ecVal=%d)\n\n", ecVal);
	for(i = 0; RegCmdList[i].readcmd; i++){
		if (!((RegCmdList[i].readcmd)%2))
			continue;//skip even(=write) registers
		VpMpiCmd(0, ecVal, RegCmdList[i].readcmd, RegCmdList[i].datalen, cmdData);
		printk("Register %X: ", RegCmdList[i].readcmd);
		for(j=0; j < RegCmdList[i].datalen; j++){
			printk(" %02X", cmdData[j]);
		
		}
		printk("\n");
	}
}


/*--------------------------------------------------------------------
 * ROUTINE:  DoDumpSingleReg
 *
 * DESCRIPTION:
 *    function to dump a single register
 *
 * ARGUMENTS:
 *    void
 *
 * RESULTS:
 *   return void
 *--------------------------------------------------------------------*/
void DoDumpSingleReg(unsigned char ecVal, unsigned char cmd_register)
{
	int j;
	unsigned char cmdData[64];
	unsigned char paramLen=FindCmdLen(cmd_register);
	

	if (!(cmd_register%2))
	{
		printk(" ERROR DoDumpSingleReg: register has to be odd !!!\r\n");
		return;
	}
	
	if (0 == paramLen)
	{
			printk(" ERROR DoDumpSingleReg: invalid register info !!!!!\r\n");
			return;
	}
	
	
	//validity
	if (paramLen>sizeof(cmdData))
	{
		printk(" ERROR DoDumpSingleReg: paramLen too big !!!!\r\n");
		return;
	}
	
	printk(" DoDumpSingleReg(ecVal=%d, cmd_reg=0x%X, paramLen=%d): Enter \r\n", ecVal, cmd_register, paramLen);//nirp temp remove
	
	VpMpiCmd(0, ecVal, cmd_register, paramLen, cmdData);
	printk("==============================\n");
	printk("Reading register %X: ", cmd_register);
	for(j=0; j < paramLen; j++){
		printk(" %02X", cmdData[j]);
		printk("\n");
	}
	printk("==============================\n");

}

/*--------------------------------------------------------------------
 * ROUTINE:  DoWriteDumpSingleReg
 *
 * DESCRIPTION:
 *    function to write and then read back and dump a single register
 *
 * ARGUMENTS:
 *    void
 *
 * RESULTS:
 *   return void
 *--------------------------------------------------------------------*/
void DoWriteDumpSingleReg(unsigned char ecVal, unsigned char cmd_register, unsigned char* pcmdData)
{
	int j;
	unsigned char cmdDataRead[16];
	unsigned char paramLen=FindCmdLen(cmd_register+1);//table has read registers info;
	
	if (cmd_register%2)
	{
		printk(" ERROR DoWriteDumpSingleReg: register has to be even !!!\r\n");
		return;
	}
	
	if (paramLen>sizeof(cmdDataRead))
	{
		printk(" ERROR DoWriteDumpSingleReg: paramLen too big !!!!\r\n");
		return;
	}

	if (0 == paramLen)
	{
			printk(" ERROR DoWriteDumpSingleReg: invalid register info !!!!!\r\n");
			return;
	}
	printk("DoWriteDumpSingleReg(ecVal=%d, cmd_reg=0x%X, paramLen=%d): Enter \r\n", ecVal, cmd_register, paramLen);//nirp temp remove	
	
	VpMpiCmd(0, ecVal, cmd_register, paramLen, pcmdData);
	
	//Now, read back the data to make sure that the change was made
	VpMpiCmd(0, ecVal, cmd_register+1, paramLen, cmdDataRead);

	printk("==============================\n");
	printk("Read back register %X: ", cmd_register);
	for(j=0; j < paramLen; j++){
		printk(" %02X", cmdDataRead[j]);
		printk("\n");
	}
	printk("==============================\n");

}



/*--------------------------------------------------------------------
 * ROUTINE:  FindCmdLen
 *
 * DESCRIPTION:
 *    function to find the data length of a given register
 *
 * ARGUMENTS:
 *    uint8 cmd: the register value
 *
 * RESULTS:
 *   if success: return the cmdLen
 *   if no found: return 0
 *--------------------------------------------------------------------*/
int FindCmdLen(uint8 cmd)
{
	int i;
	for(i = 0; RegCmdList[i].readcmd; i++){
		if(RegCmdList[i].readcmd == cmd) {
			return RegCmdList[i].datalen;
		}
	}
	return 0; /* not found */
}

