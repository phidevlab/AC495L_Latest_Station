/****************************************************************************
*                                                                            
*	Company:			Audiocodes Ltd.                                                      
*                                                                            
*   Project:			VoPP SoC                                                     
*                                                                          
*   Hardware Module: 	AC4xx                                                  
*                                                                           
*   File Name: 			katri_fxs_drv.h                                                  
*                                                                            
*                                                                            
******************************************************************************
*                                                                            
* 	DESCRIPTION:		definitions for the katri fxs driver		  		                                                             
*                                                                            
******************************************************************************/

#define AC49X_FXSFXO_DRV_DIR_NAME_STR		"fxsfxo"
#define AC49X_FXSFXO_DRV_DEVICE_NAME_STR	"0"
#define AC49X_FXSFXO_DRV_FD_NAME_STR		"/dev/fxsfxo/0"


/* this is the list of the commands to the fxsfxo driver */
typedef enum {

	FXSFXO_GET_INFO_CMD_E,
	FXSFXO_LINE_1_CMD_E,
	FXSFXO_LINE_2_CMD_E,
	FXSFXO_LINE_3_CMD_E,
	FXSFXO_LINE_4_CMD_E,

} FXSFXO_DRIVER_COMMAND_ENT;

typedef enum {

	FXSFXO_OFF_CMD_E = 0,
	FXSFXO_ON_CMD_E = 1,

} FXSFXO_COMMAND_ENT;


#define LINE_1_GPIO_BIT	0x20000 /* bit 17 */
#define LINE_2_GPIO_BIT	0x400 	/* bit 10 */
#define LINE_3_GPIO_BIT	0x10000 /* bit 16 */
#define LINE_4_GPIO_BIT	0x100 	/* bit 8 */

#define LINE_1_STATUS_MASK	0x01   	/* 0 = off_hook , 1 = on_hook */
#define LINE_2_STATUS_MASK	0x02
#define LINE_3_STATUS_MASK	0x04
#define LINE_4_STATUS_MASK	0x08

#define LINE_1_CMD_CPIO_BIT	0x2000	/* SYS_GPIO~DATA_OUT_1 bit 13, gpio 45 */
#define LINE_2_CMD_CPIO_BIT	0x1000	/* SYS_GPIO~DATA_OUT_1 bit 12, gpio 44 */
#define LINE_3_CMD_CPIO_BIT	0x8000	/* SYS_GPIO~DATA_OUT_1 bit 15, gpio 47 */
#define LINE_4_CMD_CPIO_BIT	0x4000	/* SYS_GPIO~DATA_OUT_1 bit 14, gpio 46 */

#define LINE_1_CMD_MASK	0x01   	/* 0 = ring_on, 1 = ring_off */
#define LINE_2_CMD_MASK	0x02
#define LINE_3_CMD_MASK	0x04
#define LINE_4_CMD_MASK	0x08



typedef enum {

	AC494_FXSFXO_ERROR_E = 	-1,
	AC494_FXSFXO_OK_E	=	0,

} AC494_FXSFXO_STATUS_ENT;


#define SIZE_OF_FLAG	4 /* sizeof(unsigned int) */
 

typedef struct {

	int		lineStatus;

} FXSFXO_INFO_STCT;


typedef struct {

	unsigned int 					command;
	FXSFXO_INFO_STCT		 		fxsfxoInfo;

} AC494_FXSFXO_GET_STATUS_STCT;


typedef enum {

	AC494_FXS_MODULE_E,
	AC494_FXO_MODULE_E,
	AC494_EMPTY_MODULE_E

} AC494_FXSFXO_MODULE_TYPE_ENT;

#define AC494_FXSFXO_NUM_OF_MODULES	4



typedef struct {

	int		onTimer;
	int		offTimer;
	int		onUpTimer;
	int		onDownTimer;

} AC494_FXSFXO_TOGGLE_MODE_STCT;


#define FXSFXO_POLLING_TIME	50

#define	TOGGLE_ON_T		700000
#define	TOGGLE_OFF_T	2000000

#define TOGGLE_ON_UP_T  	50000	
#define TOGGLE_ON_DOWN_T    25000



typedef struct {

	int	ringState;

} AC494_FXFSFXO_STCT;




	
	
