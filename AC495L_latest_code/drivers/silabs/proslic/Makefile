DRV_DIR := $(M)
EXTRA_CFLAGS:=$(CUST_KCFLAGS)
EXTRA_CFLAGS += -I$(DRV_DIR)/include/
EXTRA_CFLAGS += -DCONFIG_NUM_SLIC_DEVICES=1

obj-m += silabs_fxs_3215_drv_mod.o
silabs_fxs_3215_drv_mod-objs := fxs_3215_drv.o silabs_fxs_wrapper.o ProSLIC.o Si321x_Intf.o

