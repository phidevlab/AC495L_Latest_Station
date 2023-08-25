#ifndef __MPI_DRV_H__
#define __MPI_DRV_H__

//#define DEBUG_MODE
#define MPI_VER_MAJOR					01
#define MPI_VER_MINOR					02



#if defined CONFIG_SMB
#warning "It is a MPI_FOR_SMB"
#define MPI_FOR_SMB				CONFIG_SMB
#endif

#if defined CONFIG_ORCHID || defined(CONFIG_ORCHID_ATA) 
#warning "It is a MPI_FOR_ORCHID"
#define MPI_FOR_ORCHID	CONFIG_ORCHID
#endif

//following parameter are in khz
#define DSP_CLK							81920
#define TELE_DCLK						2048			//minimum 1 to maximum 8192
//long delay in us
#define LONG_DELAY						10	/*EA 040607 orig value was 100*/
                                            /*EA 040607 changed from 100 to 10 to get SMB to work orig value was 30*/
#if 0
#define LONG_DELAY						100	/*Nirp 060410 orig value was 3*/
											/*Nirp 060411 changed from 30 to 100 to get CID to work orig value was 30*/
#endif
#define DATALEN							8
#define BYTE_MSK						0xff

// following are the registers in physical address
#define IOMUX_PIN_SEL_13	0x0861163c			
#define IOMUX_PIN_SEL_14 	0x08611640	

#define DSP_RESET_DSBL_CTRL				0x05080000
#if defined(CONFIG_MIPS_TITAN)
#warning "It is a SPI_FOR_SMB"
#define TELE_SPCR1		0x05081010		// SPI control register
#define TELE_SPDR		0x05081018		// SPI Write buffer
#define TELE_SPBR		0x0508101C		// SPI Read buffer
#define TELE_RCR		0x05081020		// Ring in control
#else
#warning "It is a SPI_FOR_ORCHID"
#define TELE_SPCR1		0x050002C8 		// SPI control register
#define TELE_SPDR		0x050002CC 		// SPI Write buffer
#define TELE_SPBR		0x050002D0 		// SPI Read buffer
#define TELE_RCR		0x050002D4 		// Ring in control
#if 0
#define TELE_SPCR1		0x050003C8 		// SPI control register
#define TELE_SPDR		0x050003CC 		// SPI Write buffer
#define TELE_SPBR		0x050003D0 		// SPI Read buffer
#define TELE_RCR		0x050003D4 		// Ring in control
#endif

#endif

//for DSP_RESET_DSBL_CTRL register

#define DSP_RESET_TELE_IF_DSBL_MSK		0x00000004
#define DSP_RESET_MCBSP_EXT_DSBL_MSK	0x00000002	
#define DSP_RESET_MCBSP_INT_DSBL_MSK	0x00000001

#define TELE_RESET		1000	
#define MPI_RESET		1001

#define TELE_MINOR 		0x8B

//For SPCR1 register 0x0508:1010
#define DPIN_MODE_OFFSET				31
#define INTEN_OFFSET					25
#define DCE_OFFSET						24
#define WCMD_OFFSET						22
#define RCMD_OFFSET						21
#define DATALEN_OFFSET					16
#define DCLK_DIV_OFFSET					0

#define DCLKLEN_MSK						0x1f	
#define DCLK_DIV_MSK	0xffff

//For SPDR register 0x0508:1018
#define WST_OFFSET						31
#define SP_WDATA_OFFSET					0

#define SP_WDATA_MSK					0xffff

//For SPBR register 0x0508:101c
#define RST_OFFSET						31
#define SP_RDATA_OFFSET					0

#define SP_RDATA_MSK					0xffff

#define VP880_MAX_PARAM					14

#define EC_REG_Rd	0x4B//Nirp
#define EC_REG_Wr	0x4A//Nirp
#define EC_REG_LEN	0x01//Nirp

#define OP_COND_REG_Rd  	0x71//A.L
#define OP_COND_REG_Wr  	0x70//A.L
#define OP_COND_REG_LEN	    0x01//A.L

#define SYS_STATE_REG_Wr	0x56//A.L

#define HIGH_PASS_DIS       0x20//A.L


#define ACL_MAX_COMMAND_SIZE 200//Nirp

/*typedef unsigned char VpDeviceIdType;
typedef unsigned char uint8;*/
int spi8xx_byte_write(unsigned char data);/* NIRP fxos integration 070206 */
unsigned char spi8xx_byte_write_read(unsigned char value, int *ret_code);/* NIRP fxos integration 070206 */
// function prototype
// following functions for Legerity device.
void VpMpiCmd(unsigned char, unsigned char, unsigned char, unsigned char, unsigned char *);
#ifdef WINBOND_SLIC_EN
// following functions are for the Winbond device.
void VpPxMpiRead(unsigned char deviceId, unsigned char address, unsigned char paramLen, unsigned char *paramPtr);
void VpPxMpiWrite(unsigned char deviceId, unsigned char address, unsigned char paramLen, unsigned char *paramPtr);
#endif

#endif //__MPI_DRV_H__
