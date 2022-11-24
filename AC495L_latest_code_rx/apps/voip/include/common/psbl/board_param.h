#ifndef __BOARD_PARAM_H__
#define __BOARD_PARAM_H__

typedef enum
{
    BD_PARAM_SDRAM_ID,
    BD_PARAM_BLF_LCD_ID,
    BD_PARAM_EXTERN_SW_ID,
    BD_PARAM_SW_1000M_ID,
    BD_PARAM_MAIN_LCD_ID,
    BD_PARAM_MAX_ID
}bd_param_t;


/*Use 3 bits to express different sdram size*/
#define BD_PARAM_SDRAM_MASK        (0x7 << 0)
#define BD_PARAM_SDRAM_16M         (0x0 << 0)
#define BD_PARAM_SDRAM_32M         (0x1 << 0)
#define BD_PARAM_SDRAM_64M         (0x2 << 0)
#define BD_PARAM_SDRAM_128M        (0x3 << 0)
#define BD_PARAM_SDRAM_256M        (0x4 << 0)
#define BD_PARAM_SDRAM_512M        (0x5 << 0)
#define BD_PARAM_SDRAM_1024M       (0x6 << 0)
#define BD_PARAM_SDRAM_2048M       (0x7 << 0)

/*Use 2 bits to express if blf lcd exists*/
#define BD_PARAM_BLF_LCD_MASK      (0x3 << 3)
#define BD_PARAM_NO_BLF_LCD        (0x0 << 3)
#define BD_PARAM_BLF_LCD           (0x1 << 3)


/*Use 2 bits to express different extern switch*/
#define BD_PARAM_EXTERN_SW_MASK      (0x3 << 5)
#define BD_PARAM_NO_EXTERN_SW        (0x0 << 5)
#define BD_PARAM_EXTERN_SW_RTL8363SB (0x1 << 5)

/*Use 1 bits to express if switch supports 1000M link speed*/
#define BD_PARAM_SW_1000M_MASK      (0x1 << 7)
#define BD_PARAM_SW_1000M_NO        (0x0 << 7)
#define BD_PARAM_SW_1000M_YES       (0x1 << 7)

/*Use 3 bits to express main LCD type*/
#define BD_PARAM_MAIN_LCD_MASK      (0x7 << 8)
#define BD_PARAM_MAIN_LCD_AUTODET   (0x0 << 8)
#define BD_PARAM_MAIN_NORMAL_LCD    (0x1 << 8)
#define BD_PARAM_MAIN_PLUS_LCD      (0x2 << 8)

unsigned int get_bd_param_env(void);
void set_bd_param_env(unsigned int val);

unsigned int get_bd_param(bd_param_t index);
void set_bd_param(bd_param_t index, unsigned int val);

#endif
