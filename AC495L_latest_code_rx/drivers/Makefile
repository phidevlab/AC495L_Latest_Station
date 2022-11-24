#
# Makefile for the kernel audiocodes character device drivers.
#
obj-y += ac49x_dsp/


ifeq ($(BOARD), $(findstring $(BOARD),tulip smb orchid_main orchid_et))
obj-y += ac49x_mpi/
obj-y += le88/
obj-y += silabs/proslic/
obj-y += silabs/daa/
endif

ifeq ($(BOARD), $(findstring $(BOARD),m495l 405hd))
obj-y += st7567_lcd/
endif

obj-y += ac49x_lcd_io/
obj-y +=  gpio_drv/
obj-y +=  avalanche_lcd/
