
LEGAL_PLATFORMS := p b i s
ifneq ($(PLATFORM_TYPE), $(findstring $(PLATFORM_TYPE),$(LEGAL_PLATFORMS)))
$(error The PLATFORM_TYPE "$(PLATFORM_TYPE)" is not supported. Legal values are: $(LEGAL_PLATFORMS))
endif

ifndef PLATFORM_TYPE
echo Warning: PLATFORM_TYPE unspecified !!!
$(warning ****Warning: PLATFORM_TYPE unspecified !!!!)
endif


TOPDIR := $(shell pwd)/../


VPATH:= $(TOPDIR) $(TOPDIR)../source $(TOPDIR)../source/acMessageQueue \



SRC_AC_MESSAGE_QUEUE := acBufferPool acBufferList quLinkedQueue linuxHal 
SRC_AC_TIMER_MECHANISM := acTimers Timer_Test

SRCS := $(SRC_AC_MESSAGE_QUEUE) $(SRC_AC_TIMER_MECHANISM) acl_log


#echo $(warning $(SRC_AC_MESSAGE_QUEUE))


APP_NAME := Timer_Test

ifeq ($(PLATFORM_TYPE), p)
CROSS_COMPILE=arm-linux-uclibc-
endif
ifeq ($(PLATFORM_TYPE), b)
CROSS_COMPILE=mips-linux-
endif
ifeq ($(PLATFORM_TYPE), i)
CROSS_COMPILE=armeb-linux-
endif
ifeq ($(PLATFORM_TYPE), s)

LEGAL_KERNEL := 4 6
ifneq ($(KERNEL_TYPE), $(findstring $(KERNEL_TYPE),$(LEGAL_KERNEL)))
$(error The KERNEL_TYPE "$(KERNEL_TYPE)" is not supported. Legal values are: $(LEGAL_KERNEL))
endif

ifndef KERNEL_TYPE
echo Warning: KERNEL_TYPE unspecified !!!
$(warning ****Warning: KERNEL_TYPE unspecified !!!!)
endif

ifeq ($(KERNEL_TYPE), 4)
LINUX_DIR := $(TOPDIR)../psp_linux/kernel/linux-2.4.17_mvl21/
CROSS_COMPILE := /vobs/psp_linux/tools/mipsel-linux-uclibc/bin/mipsel-uclibc-
#for MV proffesional edition
STRIP=/opt/hardhat/devkit/mips/fp_le/bin/mips_fp_le-strip
#for MV Preview kit
#STRIP=/opt/montavista/previewkit/mips/fp_le/bin/mips_fp_le-strip
endif

ifeq ($(KERNEL_TYPE), 6)
LINUX_DIR := $(TOPDIR)../buildroot/build_mipsel_nofpu/linux-2.6.10_dev-malta-mips2_fp_len/
CROSS_COMPILE := /opt/codefidence/mipsel-linux-uclibc/bin/
STRIP=$(CROSS_COMPILE)strip
endif

endif


AS	= $(CROSS_COMPILE)as
LD	= $(CROSS_COMPILE)ld
CC	= $(CROSS_COMPILE)gcc
CPP	= $(CC) -E
AR	= $(CROSS_COMPILE)ar
NM	= $(CROSS_COMPILE)nm
ifneq ($(PLATFORM_TYPE), s)
STRIP = $(CROSS_COMPILE)strip
endif
SSTRIP	= $(CROSS_COMPILE)sstrip
OBJCOPY	= $(CROSS_COMPILE)objcopy
OBJDUMP	= $(CROSS_COMPILE)objdump

OBJDIR_REL:=Object/Release
DEPDIR_REL:=Object/Release
OBJDIR_DBG:=Object/Debug
DEPDIR_DBG:=Object/Debug

LDFLAGS = -lpthread
ifneq ($(PLATFORM_TYPE), s)
LDFLAGS += -lrt
endif

ifeq ($(PLATFORM_TYPE), i)
LDFLAGS += -Wl -elf2flt 
endif

#WARNING = -Wall -W -Wno-unused-parameter
#WARNING = -W -Wno-unused-parameter


INCLUDEFLAGS = -I$(TOPDIR)/ 
INCLUDEFLAGS += -I$(TOPDIR)../include/ 
INCLUDEFLAGS += -I$(TOPDIR)../source/acMessageQueue/ 
INCLUDEFLAGS += -I$(TOPDIR)../source/acTimerMechanism/ 

ifeq ($(PLATFORM_TYPE), s)


DRV_DIR := $(LINUX_DIR)drivers/char/
				 
INCLUDEFLAGS += -I$(TOPDIR)iphone/include/ac494_include \
				-I$(LINUX_DIR)include/ \
				-I$(DRV_DIR)ac49x_dsp/ \
				-I$(DRV_DIR)le88/ \
			    -I$(DRV_DIR)le88/vp_api_ii/driver/ \
				-I$(DRV_DIR)w682388/

ifeq ($(KERNEL_TYPE), 4)
INCLUDEFLAGS +=	-I$(LINUX_DIR)/include/asm/avalanche/generic/
#INCLUDEFLAGS += -I$(LINUX_DIR)include/asm/avalanche/titan/ 
endif
ifeq ($(KERNEL_TYPE), 6)
INCLUDEFLAGS +=	-I$(LINUX_DIR)/include/asm-mips/mach-avalanche/generic/
#INCLUDEFLAGS += -I$(LINUX_DIR)include/asm/avalanche/titan/ 
endif

endif


ifeq ($(PLATFORM_TYPE), p)
CDEFS := -mcpu=arm920t -pipe -mstructure-size-boundary=8
endif
ifeq ($(PLATFORM_TYPE), i)
CDEFS := -mstructure-size-boundary=8
endif


OSFLAG = -DLINUX -D_GNU_SOURCE -D_REENTRANT $(CDEFS)
CFLAGS = $(OSFLAG) $(WARNING) $(INCLUDEFLAGS) -D_THREAD_SAFE
ifneq ($(VOIP_PROT), mgcp)
CFLAGS += -DACL_SIP_APPLICATION
endif
ifneq ($(PLATFORM_TYPE), s)
CFLAGS += -DAC48_CPE_VOIP_TOOLKIT
endif
CFLAGS_REL = -Os
CFLAGS_DBG = -g2 -DDEBUG -O




ifeq ($(PLATFORM_TYPE), p)
CFLAGS += -DP_PLATFORM
endif
ifeq ($(PLATFORM_TYPE), b)
CFLAGS += -DB_PLATFORM
endif
ifeq ($(PLATFORM_TYPE), i)
CFLAGS += -DI_PLATFORM
endif
ifeq ($(PLATFORM_TYPE), s)
CFLAGS += -DS_PLATFORM
ifeq ($(KERNEL_TYPE), 4)
CFLAGS += -DKERNEL_TYPE2_4
endif
ifeq ($(KERNEL_TYPE), 6)
CFLAGS += -DKERNEL_TYPE2_6
endif
endif


REL_OBJS:= $(SRCS:%=$(OBJDIR_REL)/%.o)
REL_DEPS:= $(SRCS:%=$(DEPDIR_REL)/%.d)
DBG_OBJS:= $(SRCS:%=$(OBJDIR_DBG)/%.o)
DBG_DEPS:= $(SRCS:%=$(DEPDIR_DBG)/%.d)
TARGETDIR_REL = Release
TARGETDIR_DBG = Debug

TARGETS:= $(TARGETDIR_REL)/$(APP_NAME) $(TARGETDIR_DBG)/$(APP_NAME)

.PHONY: all clean release debug


all: $(TARGETS) 

debug: $(TARGETDIR_DBG)/$(APP_NAME)

release: $(TARGETDIR_REL)/$(APP_NAME)


$(TARGETDIR_REL)/$(APP_NAME): $(REL_OBJS) Makefile
	@echo linking  $@
	@[ -d $(TARGETDIR_REL) ] || mkdir $(TARGETDIR_REL)
	$(CC) $(REL_OBJS) $(LDFLAGS) -o $@
ifneq ($(PLATFORM_TYPE), i)
	@$(STRIP) --strip-unneeded  -R=.comment -R=.note $@
endif


$(TARGETDIR_DBG)/$(APP_NAME): $(DBG_OBJS) Makefile
	@echo linking  $@
	@[ -d $(TARGETDIR_DBG) ] || mkdir $(TARGETDIR_DBG)
	$(CC) $(DBG_OBJS) $(LDFLAGS) -o $@

	
clean:
	@echo clean
	@rm -rf Object $(TARGETDIR_REL) $(TARGETDIR_DBG)

$(OBJDIR_REL)/%.o: %.c Makefile
	@[ -d $(DEPDIR_REL) ] || mkdir -p $(DEPDIR_REL)
	@[ -d $(OBJDIR_REL) ] || mkdir -p $(OBJDIR_REL)
	@echo compiling $<
ifneq ($(PLATFORM_TYPE), s)
	@$(CC) -MMD -MF $(DEPDIR_REL)/$(*F).d -MT $@ $(CFLAGS) $(CFLAGS_REL) -c $< -o $@
	@[ -s $(DEPDIR_REL)/$(*F).d ] || rm -f $(DEPDIR_REL)/$(*F).d
endif
ifeq ($(PLATFORM_TYPE), s)
	@echo $(CC) $(CFLAGS) $(CFLAGS_REL) -c $< -o $@
	@$(CC) $(CFLAGS) $(CFLAGS_REL) -c $< -o $@
	@[ -s $(DEPDIR_REL)/$(*F).d ] || rm -f $(DEPDIR_REL)/$(*F).d
endif


$(OBJDIR_DBG)/%.o: %.c Makefile
	@[ -d $(DEPDIR_DBG) ] || mkdir -p $(DEPDIR_DBG)
	@[ -d $(OBJDIR_DBG) ] || mkdir -p $(OBJDIR_DBG)
	@echo compiling $<
	@$(CC) -MMD -MF $(DEPDIR_DBG)/$(*F).d -MT $@ $(CFLAGS) $(CFLAGS_DBG) -c $< -o $@
	@[ -s $(DEPDIR_DBG)/$(*F).d ] || rm -f $(DEPDIR_DBG)/$(*F).d


-include $(REL_DEPS) $(DBG_DEPS)



