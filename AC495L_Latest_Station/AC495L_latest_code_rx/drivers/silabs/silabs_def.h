#ifndef _SILABS_HW_H
#define _SILABS_HW_H

#define CS_DOWN 1
#define CS_UP   0

#if defined(CONFIG_ORCHID) || defined(CONFIG_ORCHID_ATA)
#define SILABS_RESET 4//SCC_RESET_N; SIP-16P's pin16
#define SILABS_MODE 31//? RFU(R144); SIP-16P's pin10 
#else
#define SILABS_RESET 51
#define SILABS_MODE 43
#endif
#define SILABS_DAA_SELECTED 0
#define SILABS_FXS_SELECTED 1

#if defined(CONFIG_SMB) || defined(CONFIG_ORCHID) || defined(CONFIG_ORCHID_ATA)
#define DAA_SELECTED   0
#define DAA_UNSELECTED  1
#define SLIC_SELECTED    0
#define SLIC_UNSELECTED   1
#elif defined(CONFIG_TULIP) || defined(CONFIG_TULIP2_40) || defined(CONFIG_TULIP2_21)
#define DAA_SELECTED   0
#define DAA_UNSELECTED  1
#define SLIC_SELECTED    1
#define SLIC_UNSELECTED   0
#endif

#if defined(CONFIG_ORCHID) || defined(CONFIG_ORCHID_ATA)
#define SILABS_CS0 43
#define SILABS_CS1 46
#define SILABS_CS2 45
#define SILABS_CS3 48
#else
#define SILABS_CS0 44
#define SILABS_CS1 45
#define SILABS_CS2 46
#define SILABS_CS3 47
#endif

#if defined(CONFIG_TULIP2_21) || defined(CONFIG_TULIP2_40)
#define CONFIG_AC494_SILABS_FXOS_DAISY_CHAIN_MODE	1
#endif
static inline int CHIP_CS(int chip_index)
{
    int cs = -1;
    switch(chip_index)
    {
    case 0: cs=SILABS_CS0; break;
    case 1: cs=SILABS_CS1; break;
    case 2: cs=SILABS_CS2; break;
    case 3: cs=SILABS_CS3; break;
    }
    return cs;
}

#define NUM_SLIC_LINES CONFIG_NUM_SLIC_DEVICES
#ifndef CONFIG_NUM_DAA_DEVICES
#define NUM_DAA_LINES 0
#else
#define NUM_DAA_LINES CONFIG_NUM_DAA_DEVICES
#endif

//#define NUM_SLIC_LINES 1
//#define NUM_DAA_LINES 1

#define SILABS_FXS_POLLING_INTERVAL 10000
#define SILABS_FXS_FLASH_COUNTER  	700 /*msec*/

#if defined(CONFIG_SMB) || defined (CONFIG_TULIP) || defined(CONFIG_ORCHID) || defined(CONFIG_ORCHID_ATA)
#define CH_TO_SLIC(ch)     (ch/2)
#define CH_TO_DAA(ch)      (ch/2)

#define SLIC_TO_CH(slic_index)   (2*slic_index)
#define DAA_TO_CH(daa_index)   (2*daa_index+1)
#endif

#if defined (CONFIG_TULIP2_40)
#define CH_TO_SLIC(ch)	(ch) //((ch%2)+(2*((ch)/4)))//((ch)/2 + (ch)%2)
#define SLIC_TO_CH(slic_index)	(slic_index)//((slic_index)*2 - (slic_index)%2)
#define CH_TO_DAA(ch)	(ch)
#define DAA_TO_CH(daa_index)	(daa_index)
#endif

#if defined (CONFIG_TULIP2_21)
#define CH_TO_SLIC(ch) (ch)
#define SLIC_TO_CH(slic_index) (slic_index)
#define CH_TO_DAA(ch)	(0)
#define DAA_TO_CH(daa_index)	(2*daa_index+2)
#endif

#define TICKS_PER_SECOND	0x64
#endif
