/****************************************************************************************
 * File			:	mpi_drv.c
 *
 * Description	:	Telephony interface driver
 * Version		:	1.2
 * 
 * (c) Copyright 2005, Outline Electronics Limited, All rights reserved.
 *
 * 
 * Change Log:
 *
 * Date          By         Description
 * ===========   ========   =============================================================
 * 2005.08.22    Edison     First created
 *							TELE_CLK = 160KHz
 * 2006.01.04	 Edison     Removed the VP880_MAX_PARAM limitation for R&W access
 * 2006.01.24	 Edison		Support setting the TELE_DCLK through TELE_DCLK param 
 *							in mpi_drv.h
 *							working frequency: 0.625KHz to 40960 KHz (@DSP_CLK=81920KHz)
 *							default :2.048MHz
 ****************************************************************************************/
#include <linux/config.h>

#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/ioport.h>
#include <linux/fcntl.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/proc_fs.h>
#include <asm/io.h>
#include <asm/uaccess.h>
//#include <asm/system.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/devfs_fs_kernel.h>
#include <linux/spinlock.h> 

#include <pal.h>
#include "mpi_drv.h"

//#define DBG_ON
#ifdef DBG_ON
#define dbg(a1,a2...) printk("[%s:%d]" a1 ,__FUNCTION__,__LINE__,##a2 )
#else
#define dbg(a1,a2...) do {} while (0)
#endif

typedef struct tele_dev_stc {               
	int			device_id;
	char			teleif_name[80];
//	devfs_handle_t 		handle; 
	struct proc_dir_entry	*procfs_entry;
	struct semaphore 		sem; /* mutex lock */  
} TELE_DEV; 

// #define  MPI_DRV_DEBUG//Nirp
#define  MPI_DRV_EXPORT_VPMPICMD//Nirp

#ifdef MPI_DRV_DEBUG
#define DBG_PRINT(a1,a2...)	printk("[%s:%d] " a1, __FUNCTION__, __LINE__, ##a2)
#else
#define DBG_PRINT(a1,a2...)
#endif

/***********************************
 * Global Variables
 **********************************/
//static devfs_handle_t devfs_dir_handle = NULL;
static TELE_DEV *tele_dev = NULL;
static unsigned int baseAddr_teleDev;
static int size_teleDev;
static unsigned long usdelay = 0;		//2006.01.24

/*******************************************************************************
 * Function:	tele_delay				
 * Description:	return the us time delay required when writing consective 
 *				TELEPHONY registers
 * Argument:	void
 * Return:		us delay
 * Remark:		provide the necessary delay for consective registers access
 * 				(2006.01.24)
 ******************************************************************************/
unsigned long tele_delay(void)
{
	return ((unsigned long)1000/TELE_DCLK)+(((1000.0/TELE_DCLK>1000/TELE_DCLK))?1:0);
}


/*******************************************************************************
 * Function:	tele_reset
 * Description:	Reset and enable for the telephony interface after specified
 * 				time
 * Argument:	bvalue 	0 - immediately enable the interface (2006.01.24)
 *						Other values - the us delay
 * Return:		void
 * Remark:		Assume DSP initialized
 ******************************************************************************/
void tele_reset(unsigned short bvalue)
{
	volatile unsigned int regValue;	
	regValue = *(volatile unsigned int*)( KSEG1ADDR(DSP_RESET_DSBL_CTRL) );
	mb();
	
	if(bvalue)
	{
	regValue &= (volatile unsigned int)~(0x00000004);
	*(volatile unsigned int*)(KSEG1ADDR(DSP_RESET_DSBL_CTRL)) = regValue;
	mb();
		udelay(bvalue);	//delay for bvalue us
	}
	regValue |= (volatile unsigned int)(0x00000004);
	*(volatile unsigned int*)(KSEG1ADDR(DSP_RESET_DSBL_CTRL)) = regValue;
	printk("regvalue %x\n", regValue);
	mb();
	regValue = *(volatile unsigned int *)(KSEG1ADDR(DSP_RESET_DSBL_CTRL));
	mb();
	printk("regvalue from DSPRESETDSBLCTRL %x\n", regValue);
}

/*******************************************************************************
 * Function:	mpi_write
 * Description: Command/data to be sent to the Telephony MPI bus

 * Argument:	bvalue 	- Command/data to be sent (1 bits to 16 bits)
 * Return:		void
 * Remark:		(2006.01.24) 	- use usdelay to be the udelay() parameter
 *								- WST bit checked at the end of the function
 *				(2005.08.22) 	- use fix mdelay time
 *								- WST bit checked in the front part
 ******************************************************************************/
void mpi_write(unsigned short bvalue)
{
	volatile unsigned int regValue;
	
	//check if the WST bit set in SPDR
#if ((MPI_VER_MAJOR==1) && (MPI_VER_MINOR<2))
	// following should be checked after the writing WCMD to TELE_SPCR1 ? (2006.01.24)
	do
	{
		regValue = *(volatile unsigned int*)( KSEG1ADDR(TELE_SPDR) );
		mb();
		regValue &= (1<<WST_OFFSET);
	}while(regValue);
#endif	
	
	*(volatile unsigned int*)( KSEG1ADDR(TELE_SPDR) ) = (volatile unsigned int)(bvalue & SP_WDATA_MSK);
	mb();
	

	//Consecutive writes to MPI registers should be at least 1 TELE_CLK apart.
#if ((MPI_VER_MAJOR==1) && (MPI_VER_MINOR<2))
	mdelay(1);
#else
	udelay(usdelay);
#endif
	
	// intialize transmittion
	regValue = (*(volatile unsigned int*) KSEG1ADDR(TELE_SPCR1) );
	mb();
#ifdef WINBOND_SLIC_EN
	regValue &= 0xff48ffff;		//Set SPI mode to 8 bits
	regValue |=  0x00480000;  
#endif
#ifdef LE88_SLIC_EN
	regValue |=  (1<<WCMD_OFFSET);
#endif
	*(volatile unsigned int*)( KSEG1ADDR(TELE_SPCR1) )=(volatile unsigned int)regValue;
	mb();
	
#if ((MPI_VER_MAJOR==1) && (MPI_VER_MINOR<2))
	// too long
	mdelay(10);
#else
	// following should be checked after the writing WCMD to TELE_SPCR1 ? (2006.01.24)
	do
	{
		regValue = *(volatile unsigned int*)( KSEG1ADDR(TELE_SPDR) );
		mb();
		regValue &= (1<<WST_OFFSET);
	}while(regValue);
	
	// Wait for 1 TELE_CLK or longer
	udelay((usdelay>LONG_DELAY)?usdelay:LONG_DELAY);
#endif
}

#ifdef WINBOND_SLIC_EN
void mpi_write16(unsigned short bvalue)
{
	volatile unsigned int regValue;
	
	//check if the MPI busy
	do
	{
		regValue = *(volatile unsigned int*)( KSEG1ADDR(TELE_SPDR) );
		mb();
		regValue &= 0x80000000;
	}while(regValue);
	
	*(volatile unsigned int*)( KSEG1ADDR(TELE_SPDR) ) = (volatile unsigned int)(bvalue&0xffff);
	mb();
	
	//Consecutive writes to MPI registers should be at least 1 TELE_CLK apart.
	mdelay(1);
	
	// intialize transmittion
	regValue = (*(volatile unsigned int*) KSEG1ADDR(TELE_SPCR1) );
	mb();
	regValue &= 0xff40ffff;	  // Clear SPI Data Length
	regValue |=  0x00500000;  //Set SPI mode to 16 bits for ProX Command Address cycle
	*(volatile unsigned int*)( KSEG1ADDR(TELE_SPCR1) )=(volatile unsigned int)regValue;
	mb();
	
	//Consecutive writes to MPI registers should be at least 1 TELE_CLK apart.
	mdelay(1);
}
#endif


/*******************************************************************************
 * Function:	mpi_read
 * Description: data to be read from by the Telephony MPI bus

 * Argument:	void
 * Return:		the bit values read from the TELE_DI
 * Remark:		(2006.01.24) 	- use usdelay to be the udelay() parameter
 *				(2005.08.22) 	- use fix mdelay time
 ******************************************************************************/
unsigned short mpi_read(void)
{
	volatile unsigned int regValue;

	regValue = *(volatile unsigned int*)( KSEG1ADDR(TELE_SPBR) );
	mb();
	// Check if new data had already exists in SPBR
	if(regValue & (1<<RST_OFFSET))
		return (regValue & SP_RDATA_MSK);
	//initialize receive
	regValue = (*(volatile unsigned int*) KSEG1ADDR(TELE_SPCR1) );
	mb();
#ifdef WINBOND_SLIC_EN
	regValue &= 0xff00ffff;		//Set SPI mode to 8 bits	
	regValue |=  0x00280000;
#endif
#ifdef LE88_SLIC_EN
	regValue |=  (1<<RCMD_OFFSET);
#endif
	*(volatile unsigned int*)( KSEG1ADDR(TELE_SPCR1) ) = (volatile unsigned int)regValue;
	mb();
	
	//Consecutive writes to MPI registers should be at least 1 TELE_CLK apart.
#if ((MPI_VER_MAJOR==1) && (MPI_VER_MINOR<2))
	mdelay(1);
#else
	udelay(usdelay);
#endif	
	
	do
	{	
		regValue = *(volatile unsigned int*)( KSEG1ADDR(TELE_SPBR) );
		mb();
	}while ((regValue & (1<<RST_OFFSET)) == 0);
	
	return (unsigned short)(regValue & SP_RDATA_MSK);
}
#ifdef MPI_DRV_EXPORT_VPMPICMD//Nirp - this function can be called directly from Slic driver
static ssize_t tele_write(const char *buf, size_t count);
static ssize_t tele_read(const char *buf, size_t count);

void ACL_sysGpioFuncSet(int pinSelReg, int shift, int func)
{  
	unsigned int temp;    
	unsigned int addr;    
#if defined(CONFIG_MIPS_ARIES)
	addr = ARIES_IOMUX_PIN_SEL_1 + (4 * (pinSelReg-1));
#elif defined(CONFIG_MIPS_TITAN)
	addr = TITAN_IOMUX_PIN_SEL_1 + (4 * (pinSelReg-1));
#else
	#error "Unsupported hardware"
#endif
	temp = *(volatile unsigned int *)addr;   
	temp &= ~(0x3 << shift);   
	temp |= (func << shift);    
	*(volatile unsigned int *)addr = temp;
}

// following functions for Legerity device.
void VpMpiCmd(unsigned char deviceId, unsigned char ecVal, unsigned char cmd, unsigned char paramLen, unsigned char *paramPtr)
{
	unsigned char param[ACL_MAX_COMMAND_SIZE];

    unsigned char tmpParam[3] = {OP_COND_REG_Rd,OP_COND_REG_LEN,0};
	int i;
#ifdef MPI_DRV_DEBUG
	int i;
#endif

	

	#if 0
		ACL_sysGpioFuncSet(10, 10, 2);		/*GPIO21*/
		mb();		
		PAL_sysGpioCtrl(21, GPIO_PIN, GPIO_OUTPUT_PIN);      

		
		while(1)
		{
			int i;
			for(i=0; i<10000; i++) mb();
			PAL_sysGpioOutBit(21, 1);
			for(i=0; i<10000; i++) mb();	
			PAL_sysGpioOutBit(21, 0);
				
		}
	#endif	

	
	if(!paramPtr)	return;
	
	if( paramLen +2 >ACL_MAX_COMMAND_SIZE){
		printk(">>>>>> ERROR VpMpiCmd: command length (%d) too big for internal buffer !!!!!", paramLen);
		return;
	}

#if defined(CONFIG_ORCHID) || defined(CONFIG_ORCHID_ATA)
#warning "It is a MPI_FOR_ORCHID: chose device"

    switch (deviceId){
        case 0:
                //GPIO43 data out
                //PAL_sysGpioOutBit(17, 0);//GPIO17
                PAL_sysGpioOutBit(43, 0);//GPIO43
                mb();
                break;
        case 1:
                //GPIO46 data out
                PAL_sysGpioOutBit(46, 0);
                mb();
                break;
        case 2:
                //GPIO45 data out
                PAL_sysGpioOutBit(45, 0);
                mb();
                break;
        case 3:
                //GPIO48 data out
                PAL_sysGpioOutBit(48, 0);
                mb();
                break;
		}


    //udelay(50); //delay for bvalue us
    for(i=0; i<1000; i++)mb();

#elif defined (CONFIG_SMB)
#warning "It is a MPI_FOR_SMB: chose device"

	switch (deviceId){
        case 0:
                //GPIO44 data out
                *(volatile unsigned int*)(KSEG1ADDR(0x0861090C)) &= 0xFEFFF;
                mb();
                break;
        case 1:
                //GPIO45 data out
                *(volatile unsigned int*)(KSEG1ADDR(0x0861090C)) &= 0xFDFFF;
                mb();
                break;
        case 2:
                //GPIO46 data out
                *(volatile unsigned int*)(KSEG1ADDR(0x0861090C)) &= 0xFBFFF;
                mb();
                break;
        case 3:
                //GPIO47 data out
                *(volatile unsigned int*)(KSEG1ADDR(0x0861090C)) &= 0xF7FFF;
                mb();
                break;
		}
#if 0
	down( &tele_dev->sem );

    if (ecVal == 1){
        mpi_write(0x4A);
        mpi_write(0x01);
        }
    else if(ecVal == 2){
        mpi_write(0x4A);
        mpi_write(0x02);
        }

    up ( &tele_dev->sem);
#endif
#endif
	/* Following code set the target EC Channel, EC1 = 0x01, EC2= 0x02 */
	if(cmd != EC_REG_Rd)
	{
		param[0]=EC_REG_Wr;
		param[1]=EC_REG_LEN;
		param[2] = ecVal;
		tele_write(param, 3);
	}
#ifdef MPI_DRV_DEBUG
	if (!(cmd%2))//write registern are even
	{
		printk(">>> MPI(EC Val=%d): Cmd = 0x%04X Value = ",ecVal, cmd);
		for(i=0; i<paramLen; i++)
			printk("0x%02X ", paramPtr?paramPtr[i]:0);
		printk(".\n");

	}
#endif
	
	param[0] = cmd;
	param[1] = paramLen;
	
	/* All odd addresses are for read operations */
	if (cmd & 1) {
		/* check if read instruction */
		tele_read(param, (paramLen + 2));
		
		if (paramPtr != NULL) {
			memcpy(paramPtr, &param[2], paramLen);
		}
	}
	else {
		/* All even addresses are for write operations */
		if (paramPtr != NULL) {
			memcpy(&param[2],paramPtr,paramLen);
		}
		tele_write(param, paramLen + 2);

        /*A.L 061005 - workaround for VI 44497: There is a deviation on the DC-BAIS  that cause Echo- Canceler problem.
        In order to solve the problem -> when setting 0x56, check if the HPF was disable in 0x70. If disabled, then enabled it*/

#ifdef MPI_DRV_DEBUG
        tele_read(tmpParam, 3);
    	printk(">>> MPI(EC Val=%d): Cmd = 0x%04X before write = %x \r\n",ecVal, cmd, tmpParam[2]);
#endif
        
        if (cmd == SYS_STATE_REG_Wr) {
            tele_read(tmpParam, 3);//read reg 0x71
#ifdef MPI_DRV_DEBUG
    	    printk(">>> MPI(EC Val=%d): Cmd = 0x%04X set 0x56, read 0x71\r\n",ecVal, tmpParam[0]);
#endif
            if (tmpParam[2] & HIGH_PASS_DIS) { //check if HPF disabled
                tmpParam[0] = OP_COND_REG_Wr;
                tmpParam[2] &= ~(HIGH_PASS_DIS);//enable the HPF
#ifdef MPI_DRV_DEBUG
                printk(">>> MPI(EC Val=%d): Cmd = 0x%04X reg 0x71 going to write = %x... \r\n",ecVal, tmpParam[0], tmpParam[2]);
#endif
        		tele_write(tmpParam, 3);                
            }
        }

#ifdef MPI_DRV_DEBUG
        tmpParam[0] = OP_COND_REG_Rd;
        tele_read(tmpParam, 3);
    	printk(">>> MPI(EC Val=%d): Cmd = 0x%04X after write = %x \r\n",ecVal, cmd, tmpParam[2]);
#endif
	}
	
#if defined (CONFIG_SMB) 
#warning "SMB: Set all #CS of GPIO to High"
	//GPIO data out
    *(volatile unsigned int*)(KSEG1ADDR(0x0861090C)) |= 0xF000;
    mb();
#elif defined(CONFIG_ORCHID) || defined(CONFIG_ORCHID_ATA) 
#warning "Orchid: Set all #CS of GPIO to High"
	//GPIOs data out 1
    //*(volatile unsigned int*)(KSEG1ADDR(0x0861090C)) |= 0x16800;    
	PAL_sysGpioOutBit(43, 1);//PAL_sysGpioOutBit(17, 1);
	PAL_sysGpioOutBit(46, 1);
	PAL_sysGpioOutBit(45, 1);
	PAL_sysGpioOutBit(48, 1);
	for(i=0; i<1000; i++)mb();// the fly line needs the delaying
    mb();
#endif

	return;

}
void
VpMpiReset(unsigned char  deviceId, int deviceType)
{
	tele_reset(50);
}

#ifdef WINBOND_SLIC_EN
////////////////////////////////////////////////////
// The following functions are for Winbond devices.
//
void VpPxMpiRead(unsigned char deviceId, unsigned char address, unsigned char paramLen, unsigned char *paramPtr)
{
	unsigned char i;
	unsigned short CmdAddress=0x8000;

	if(!paramPtr)	return;

		CmdAddress |= (unsigned short)address;

		// Send Command and Address
		down( &tele_dev->sem );

		mpi_write16(CmdAddress);	

		for (i=0; i< paramLen ; i++)
			*(unsigned char*)paramPtr++ = (unsigned char)mpi_read();
		up( &tele_dev->sem );


}

void VpPxMpiWrite(unsigned char deviceId, unsigned char address, unsigned char paramLen, unsigned char *paramPtr)
{
	unsigned char i;
	unsigned short CmdAddress = 0x00FF;

	if(!paramPtr)	return;

		CmdAddress &= (unsigned short)address;

		// Write Data
		down( &tele_dev->sem );
		mpi_write16(CmdAddress);

		for (i=0; i< paramLen ; i++)
			mpi_write(*(unsigned char*)paramPtr++ );
		up( &tele_dev->sem );

}

////////////////end Winbond SPI cmds
#endif



#endif//Nirp MPI interface is now part of the MPI driver

/*********************************************************************************************/
static ssize_t 
#ifdef MPI_DRV_EXPORT_VPMPICMD
tele_write(const char *buf, size_t count)
{
	unsigned char i;
	unsigned char cmd = *( (unsigned char *) ( buf ) );
	unsigned char len = *( (unsigned char *) ( buf + 1 ) );
	unsigned char *param = (unsigned char *) ( buf + 2 );
#else
tele_write(struct file *file, const char *buf, size_t count, loff_t *ppos)
{
	unsigned char i;
	TELE_DEV *tele = (TELE_DEV *)file->private_data;	
	unsigned char cmd = *( (unsigned char *) ( buf ) );
	unsigned char len = *( (unsigned char *) ( buf + 1 ) );
	unsigned char *param = (unsigned char *) ( buf + 2 );
#endif
	down( &tele_dev->sem );

	
	dbg( "CMD: %x\n", cmd );
	mpi_write( cmd );
	
	for (i=0; i< len ; i++)
		mpi_write( param[i] );
		
	up(&tele_dev->sem);
	return count;
}

/*********************************************************************************************/
static ssize_t 
#ifdef MPI_DRV_EXPORT_VPMPICMD
tele_read(const char *buf, size_t count)
{
	unsigned char i;
	unsigned char cmd = *( (unsigned char *) ( buf ) );
	unsigned char len = *( (unsigned char *) ( buf + 1 ) );
	unsigned char *param = (unsigned char *) ( buf + 2 );
#else
tele_read(struct file *file, const char *buf, size_t count, loff_t *ppos)
{
	unsigned char i;
	TELE_DEV *tele = (TELE_DEV *)file->private_data;	
	unsigned char cmd = *( (unsigned char *) ( buf ) );
	unsigned char len = *( (unsigned char *) ( buf + 1 ) );
	unsigned char *param = (unsigned char *) ( buf + 2 );
#endif
	
	down(&tele_dev->sem);
	
	dbg( "CMD: %x\n", cmd );
	mpi_write(cmd);
	
	for ( i=0; i < len ; i++ )
		*param++ = (unsigned char)mpi_read();
	up(&tele_dev->sem);
	
	return count;
}

/*********************************************************************************************/
static long 
tele_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{    
	TELE_DEV *tele = (TELE_DEV *)file->private_data;
	unsigned short value = *(unsigned short*)arg;
	
	down(&tele->sem);
	
	switch (cmd){
		case TELE_RESET:
			tele_reset(value);
			break;
		case MPI_RESET:
			mpi_write(0x04);
			break;
	}
	
	up(&tele->sem);
	return 0;
}

/*********************************************************************************************/
static int 
tele_open(struct inode *inode, struct file *file )
{
	down(&tele_dev->sem);
	file->private_data = tele_dev;
	up(&tele_dev->sem);
	
	dbg("Device opened!\n");
	return 0;
}

/*********************************************************************************************/
static int 
tele_release(struct inode *inode, struct file *file )
{
	return 0;
}
/*********************************************************************************************/

static int 
tele_fasync(int fd, struct file *file, int mode)
{
	return 0;
}

/*********************************************************************************************/

static int 
tele_flush(struct file *file)
{
	return 0;
}


struct file_operations tele_fops = {
	owner:  THIS_MODULE,
	read:   tele_read,
	write:	tele_write,
	unlocked_ioctl:	tele_ioctl,
	fasync: tele_fasync,
	flush:  tele_flush,
	open:	tele_open,
	release:  tele_release
};


int __init teleDev_init(void)
{
	#define SPCR1_REG_VAL		(0<<DPIN_MODE_OFFSET)|(1<<DCE_OFFSET)|(8<<DATALEN_OFFSET)|((unsigned short)(( DSP_CLK/2 )/TELE_DCLK -1 )<<DCLK_DIV_OFFSET )
	char devname[10];
	volatile unsigned int regValue;
	
	printk("AC49xMPI: Device init start.\n");
	tele_dev = (TELE_DEV *)kmalloc(sizeof(TELE_DEV),GFP_KERNEL);
	
	if(tele_dev == NULL)
		goto init_err;
	memset(tele_dev,0,sizeof(tele_dev));
	
	baseAddr_teleDev = (unsigned int)tele_dev;
	size_teleDev = (int)sizeof(TELE_DEV);

//	devfs_dir_handle = devfs_mk_dir(NULL,"mpi",NULL);
	sprintf(devname,"0");
//	tele_dev->handle = devfs_register( devfs_dir_handle , devname, DEVFS_FL_AUTO_DEVNUM, 0, 0,
//		S_IFCHR | S_IRUGO | S_IWUGO, &tele_fops,NULL);
	//tele_dev -> lock = SPIN_LOCK_UNLOCKED;
	sema_init(&tele_dev->sem,1);

#if !defined(CONFIG_ORCHID) &&  !defined(CONFIG_ORCHID_ATA) 

	// following Multiplex the IO pins to Telephony Interface dedicated IO pins	
	// mux TELE_DO, TELE_DI, TELE_DCLK
	regValue = (*(volatile unsigned int* )( KSEG1ADDR(IOMUX_PIN_SEL_13) ) );
	mb();
	regValue &= ~0x000ff000;
	regValue |=  0x55015000;		
	*(volatile unsigned int*)(KSEG1ADDR(IOMUX_PIN_SEL_13)) = regValue;
	mb();
	
	// mux TELE_nRESET, TELE_INT, TELE_nCS
	regValue = (*(volatile unsigned int* )( KSEG1ADDR(IOMUX_PIN_SEL_14) ) );
	mb();
	regValue &= ~0x0000f3cc;
	regValue |=  0x00001140;		
	*(volatile unsigned int*)(KSEG1ADDR(IOMUX_PIN_SEL_14)) = regValue;
	mb();
	
	regValue = *(volatile unsigned int *)(KSEG1ADDR(0x08611600));
	mb();

	if(regValue & 0x100000)
		printk("DSP is active.\n");
	else
	{
		printk("Error! DSP is not active.\n"
		    "Please activate it before loading the MPI driver.\n");
		goto init_err;
	}
#endif

	// disable the pull low reset to the Telephony interface
	tele_reset(50);
	// initialize the DCLK out, frequency and number of bits per transmittion
	regValue=(*(volatile unsigned int*) KSEG1ADDR(TELE_SPCR1) );
	mb();
	
#if ((MPI_VER_MAJOR==1) && (MPI_VER_MINOR<2))
	regValue = 0x010800ff;			//160KHz
#else	
	regValue = SPCR1_REG_VAL;
#endif

#if defined (CONFIG_SLIC_SILABS_FXS_MODULE) || defined (CONFIG_SLIC_SILABS_FXS_MODULE)
	regValue = 0x010800ff;//nirp ~~~ 192Khz	    
	printk(KERN_NOTICE "DSP-TELE: SPCR1 reg value = 0x%X .\n", regValue);
#endif
	*(volatile unsigned int*)(KSEG1ADDR(TELE_SPCR1)) = regValue;
	mb();
	
	printk(KERN_NOTICE "DSP-TELE: Telephony Interface driver installed.\n");
	
#ifdef DBG_ON
	mpi_write(0x73);
	printk("product code:0x%x\n",mpi_read() & BYTE_MSK);
	printk("revision code:0x%x\n",mpi_read() & BYTE_MSK);
#endif

#if ((MPI_VER_MAJOR==1) && (MPI_VER_MINOR<2))
#else
#if 1//Nirp 060410 when removing the debug prints, the Slic is not configured properly - need fine tune
	usdelay=LONG_DELAY;
#else
	usdelay=tele_delay();
#endif
	dbg("TELE_SPCR1 REG: %x\n", regValue);
	DBG_PRINT("usdelay       : %ld\n", usdelay);
#endif

#if defined(CONFIG_ORCHID) || defined(CONFIG_ORCHID_ATA) 
#warning "It is the initilization of Orchid GPIO"

	{
	    int i;
	    printk("teleDev_init for Orchid\n");
		mb();
		ACL_sysGpioFuncSet(14, 20, 2);  /*GPIO31: RFU(R144); SIP-16P's pin10*/
		mb();
		ACL_sysGpioFuncSet(9, 6, 2);    /*GPIO04: SCC_RESET_N; SIP-16P's pin16*/
		mb();
		ACL_sysGpioFuncSet(15, 4, 1);   /*GPIO42: General LED*/

		ACL_sysGpioFuncSet(10, 0, 2);   /*GPIO17: Fly wire for MPI_#CS_0 temporary*/
		ACL_sysGpioFuncSet(15, 24, 2);  /*GPIO43: MPI_#CS_0*/
		ACL_sysGpioFuncSet(15, 10, 1);	/*GPIO46: MPI_#CS_1*/
		ACL_sysGpioFuncSet(15, 6, 1);	/*GPIO45: MPI_#CS_2*/
		ACL_sysGpioFuncSet(15, 12, 1);	/*GPIO48: MPI_#CS_3*/

		mb();
		mb();
		mb();
		PAL_sysGpioCtrl(31, GPIO_PIN, GPIO_OUTPUT_PIN);
		PAL_sysGpioCtrl(4, GPIO_PIN, GPIO_OUTPUT_PIN); 
		PAL_sysGpioCtrl(42, GPIO_PIN, GPIO_OUTPUT_PIN);
		PAL_sysGpioOutBit(42, 0);// light GENERAL LED
		for(i=0; i<1000; i++)mb();
		PAL_sysGpioOutBit(4, 1);
		
		PAL_sysGpioCtrl(17, GPIO_PIN, GPIO_OUTPUT_PIN); 
		PAL_sysGpioCtrl(43, GPIO_PIN, GPIO_OUTPUT_PIN);
		PAL_sysGpioCtrl(46, GPIO_PIN, GPIO_OUTPUT_PIN);
		PAL_sysGpioCtrl(45, GPIO_PIN, GPIO_OUTPUT_PIN);
		PAL_sysGpioCtrl(48, GPIO_PIN, GPIO_OUTPUT_PIN);
		mb();
		mb();
		mb();
	 }
#endif
	return (0);

init_err:
	if(tele_dev!=NULL)
		kfree(tele_dev);
	printk("MPI init error\n");
	return -ENOMEM;
}


void teleDev_exit(void)
{
//	devfs_unregister(devfs_dir_handle);
	kfree(tele_dev);
}


/*******************************************************************************
 * Function:	spi8xx_byte_write
 * Description: Command/data to be sent to the Telephony MPI bus - will be used by the
 *              Silabs drivers
 ******************************************************************************/
int spi8xx_byte_write(unsigned char data)
{
    //printk("spi8xx_byte_write: data=0x%X \n", data);
    mpi_write((unsigned short)data);
    return 0;
}
/*******************************************************************************
 * Function:	spi8xx_byte_write_read
 * Description: Command/data to be sent to the Telephony MPI bus - will be used by the
 *              Silabs drivers
 ******************************************************************************/
unsigned char spi8xx_byte_write_read(unsigned char value, int *ret_code)
{
    unsigned short ret_value_mpi_read;
    if (NULL != ret_code)
        *ret_code=0;
    
    //printk("spi8xx_byte_write_read: value=0x%X \n", value);
    spi8xx_byte_write(value);
    ret_value_mpi_read=mpi_read();
    //printk("spi8xx_byte_write_read: ret_value_mpi_read=0x%X \n", ret_value_mpi_read);
    return (unsigned char)ret_value_mpi_read;
}


module_init(teleDev_init);
module_exit(teleDev_exit);

MODULE_LICENSE("Proprietary");
MODULE_AUTHOR("Outline Electronics Limitd.");
MODULE_DESCRIPTION("MPI driver for AC49X device");

EXPORT_SYMBOL(VpMpiCmd);//Nirp - this function can be called directly from Slic driver
EXPORT_SYMBOL(spi8xx_byte_write);
EXPORT_SYMBOL(spi8xx_byte_write_read);
#ifdef WINBOND_SLIC_EN
EXPORT_SYMBOL(VpPxMpiRead);
EXPORT_SYMBOL(VpPxMpiWrite);
#endif
