ifdef CONFIG_MIPS_ARIES
EXTRA_CFLAGS+="-I$(srctree)/arch/mips/include/asm/mach-avalanche/aries/"
else
EXTRA_CFLAGS+="-I$(srctree)/arch/mips/include/asm/mach-avalanche/titan/"
endif

obj-m += char_lcd_io.o
ac49x_demo-objs := char_lcd_io.o 


