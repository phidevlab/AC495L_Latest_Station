# File: drivers/char/le88/Makefile
#
# Makefile for the Le88 Compatible SLIC
#
LE88_DIR := $(M)/le88/
EXTRA_CFLAGS += -I$(LE88_DIR) \
				-I$(LE88_DIR)vp_api_ii/api_lib/includes/ \
				-I$(LE88_DIR)vp_api_ii/api_lib/vp880_api/ \
				-I$(LE88_DIR)vp_api_ii/arch/uvb/ \
			   	-I$(LE88_DIR)vp_api_ii/driver/ \
				-I$(M)/include/
				
				
EXTRA_CFLAGS += -DS_PLATFORM
EXTRA_CFLAGS += -DCONFIG_NUM_SLIC_DEVICES=1

obj-m += le88.o

COMMON_DIR := vp_api_ii/api_lib/common/
VP880_API_DIR := vp_api_ii/api_lib/vp880_api/
ARCH_DIR := vp_api_ii/arch/uvb/
DRIVER_DIR := vp_api_ii/driver/

COMMON_SOURCE = $(COMMON_DIR)boot_common.o $(COMMON_DIR)hbi_common.o \
				$(COMMON_DIR)vp_api.o $(COMMON_DIR)vp_api_common.o 
VP880_API_SOURCE = $(VP880_API_DIR)apicnt.o $(VP880_API_DIR)apiInit.o \
				   $(VP880_API_DIR)apiquery.o 
ARCH_SOURCE = $(ARCH_DIR)sys_service.o $(ARCH_DIR)vp_hal.o 
DRIVER_SOURCE = $(DRIVER_DIR)profiles8.o $(DRIVER_DIR)regdump.o 

le88-objs := le88_drv.o $(COMMON_SOURCE) $(VP880_API_SOURCE) $(ARCH_SOURCE) $(DRIVER_SOURCE) 

clean:
	rm -f core *.o *.a *.s
