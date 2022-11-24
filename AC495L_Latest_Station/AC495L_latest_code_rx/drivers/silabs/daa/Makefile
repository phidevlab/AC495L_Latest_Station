#File : drivers/char/silabs/daa/Makefile
#Makefile for Linux silab SLIC

DRV_DIR := $(M)
EXTRA_CFLAGS:=$(CUST_KCFLAGS)
EXTRA_CFLAGS += -I$(DRV_DIR)/include/


obj-m += silabs_daa_3050_drv_mod.o
silabs_daa_3050_drv_mod-objs := daa_3050_drv.o daa_analog.o daa_if.o

