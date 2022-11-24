# File: drivers/char/ac49x_dsp/Makefile
#
# Makefile for the Linux ac49x dsp driver.
#

EXTRA_CFLAGS:=$(CUST_KCFLAGS)
EXTRA_CFLAGS+= -I$(TOPDIR)/include/asm/avalanche/generic -DEXPORT_SYMTAB

#EXTRA_CFLAGS += -DWINBOND_SLIC_EN
EXTRA_CFLAGS += -DLE88_SLIC_EN

# O_TARGET := ac49x_mpi.o
obj-m += ac49x_mpi.o
# list-multi      := ac49x_mpi.o



ac49x_mpi-objs := mpi_drv.o

# include $(TOPDIR)/Rules.make

ac49x_mpi.o:  $(ac49x_mpi-objs)
	$(LD) -s -r -o $@ $(ac49x_mpi-objs)

clean:
	rm -f core *.o *.a *.s
