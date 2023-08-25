
LEGAL_PLATFORMS := p b i s grocx
ifneq ($(PLATFORM_TYPE), $(findstring $(PLATFORM_TYPE),$(LEGAL_PLATFORMS)))
$(error The PLATFORM_TYPE "$(PLATFORM_TYPE)" is not supported. Legal values are: $(LEGAL_PLATFORMS))
endif

ifndef PLATFORM_TYPE
echo Warning: PLATFORM_TYPE unspecified !!!
$(warning ****Warning: PLATFORM_TYPE unspecified !!!!)
endif


LEGAL_APPS := ac488_cvt ac483_cvt ac494_tulip_fxss ac494_sdp_ipp ac494_sdp_ata ac494_smb ac494_tulip2_40 ac494_tulip2_21 ac494_orchid ac494_orchid_ata
ifneq ($(APP_TYPE), $(findstring $(APP_TYPE),$(LEGAL_APPS)))
$(error The APP_TYPE "$(APP_TYPE)" is not supported. Legal values are: $(LEGAL_APPS))
endif

ifndef APP_TYPE
echo Warning: APP_TYPE unspecified !!!
$(warning ****Warning: APP_TYPE unspecified !!!!)
endif


TOPDIR := $(shell pwd)/../


VPATH:= $(TOPDIR)/iphone/source $(TOPDIR)/iphone/source/acMessageQueue
ifeq ($(PLATFORM_TYPE), s)
VPATH += $(TOPDIR)/iphone/source/librt $(TOPDIR)/iphone/source/ac494_source
endif

SRC_IPHONE := 	acl_api_demo acl_log iphone_api iphone_fxs ac49x_iphone_dsp \
				iphone_fxsLegerity iphone_hw iphone_hwNet acl_dsp_handler \
				iphone_hwNetRx iphone_hwNetTx iphone_lineTerm iphone_localNetRtcpRx \
				iphone_localNetRtcpTerm iphone_localNetRtcpTx iphone_localNetRx iphone_localNetTerm \
				iphone_localNetTx iphone_manager iphone_netExternalAccess iphone_phyExternalAccess \
				iphone_phyTerm iphone_remoteNetTerm iphone_terminal_cmds iphone_voiceChannelTerm

#ifneq ($(PLATFORM_TYPE), s)
#SRC_IPHONE += ac48x_iphone_dsp
#endif

ifeq ($(PLATFORM_TYPE), s)
SRC_IPHONE += iphone_hwLcd iphone_hwRtc iphone_hwKeypad iphone_fxsKatri iphone_fxsWinbond
endif


SRC_AC_MESSAGE_QUEUE := acBufferPool acBufferList quLinkedQueue linuxHal acl_MsgQ

SRCS := $(SRC_IPHONE) $(SRC_AC_MESSAGE_QUEUE)

APP_NAME := acl_api_demo


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
#STRIP=/opt/hardhat/devkit/mips/fp_le/bin/mips_fp_le-strip
#for MV Preview kit
STRIP=/opt/montavista/previewkit/mips/fp_le/bin/mips_fp_le-strip
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
STRIP	= $(CROSS_COMPILE)strip
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


ifneq ($(PLATFORM_TYPE), s)
INCLUDEFLAGS = -I$(TOPDIR)cfg_infrastructure/ -I$(TOPDIR)../kernel/drivers/char/ac48_dsp/ \
			    -I$(TOPDIR)../kernel/drivers/char/ac48_dsp/voice_packetizer/ \
			    -I$(TOPDIR)../kernel/drivers/char/ac48_dsp/voice_packetizer/CommonResources/Include/ \
			    -I$(TOPDIR)../kernel/drivers/char/ac48_dsp/voice_packetizer/CommonResources/AC48xDrivers/Include/ \
			    -I$(TOPDIR)../kernel/drivers/char/ac48_dsp/voice_packetizer/VoicePacketizer/Include/ \
			    -I$(TOPDIR)../kernel/drivers/char/ac48_dsp/voice_packetizer/VoicePacketizer/RTCP_XR/include/ \
			    -I$(TOPDIR)../kernel/drivers/char/le88/ -I$(TOPDIR)../kernel/drivers/char/le88/vp_api_ii/driver/ 
endif


INCLUDEFLAGS += -I$(TOPDIR)/ 
INCLUDEFLAGS += -I$(TOPDIR)iphone/include/ 
INCLUDEFLAGS += -I$(TOPDIR)iphone/source/acMessageQueue/ 

ifeq ($(PLATFORM_TYPE), s)

ifeq ($(KERNEL_TYPE), 4)
INCLUDEFLAGS +=	-I$(LINUX_DIR)/include/asm/avalanche/generic/
#INCLUDEFLAGS += -I$(LINUX_DIR)include/asm/avalanche/titan/ 
endif
ifeq ($(KERNEL_TYPE), 6)
INCLUDEFLAGS +=	-I$(LINUX_DIR)/include/asm-mips/mach-avalanche/generic/
#INCLUDEFLAGS += -I$(LINUX_DIR)include/asm/avalanche/titan/ 
endif

INCLUDEFLAGS +=	-I$(LINUX_DIR)include/ \
				-I$(LINUX_DIR)drivers/char/ac49x_dsp/ \
				-I$(LINUX_DIR)drivers/char/le88/ -I$(LINUX_DIR)drivers/char/le88/vp_api_ii/driver/ \
				-I$(LINUX_DIR)drivers/char/w682388/

INCLUDEFLAGS += -I$(TOPDIR)iphone/include/ac494_include -I$(TOPDIR)iphone/source/librt/
endif


ifeq ($(PLATFORM_TYPE), p)
CDEFS := -mcpu=arm920t -pipe -mstructure-size-boundary=8
endif
ifeq ($(PLATFORM_TYPE), i)
CDEFS := -mstructure-size-boundary=8
endif

OSFLAG = -DLINUX -D_GNU_SOURCE -D_REENTRANT $(CDEFS)
CFLAGS = $(OSFLAG) $(WARNING) $(INCLUDEFLAGS) -D_THREAD_SAFE

CFLAGS_REL = -Os
CFLAGS_DBG = -g2 -DDEBUG -O


ifeq ($(APP_TYPE), ac488_cvt)
CFLAGS += -DAC488_CPE_VOIP_TOOLKIT
endif
ifeq ($(APP_TYPE), ac483_cvt)
CFLAGS += -DAC483_CPE_VOIP_TOOLKIT
endif


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

# convert APP_TYPE to uppercase
ifeq ($(APP_TYPE), ac494_tulip_fxss)
CFLAGS += -DAC494_TULIP_FXSS
CFLAGS += -DCONFIG_TULIP
APP_NAME := acl_api_demo_tulip
endif
ifeq ($(APP_TYPE), ac494_sdp_ipp)
CFLAGS += -DAC494_SDP_IPPHONE
CFLAGS += -DCONFIG_SDP_IPPHONE
APP_NAME := acl_api_demo_sdp_IPP
endif
ifeq ($(APP_TYPE), ac494_orchid)
CFLAGS += -DAC49_ORCHID_VOIP_TOOLKIT
CFLAGS += -DCONFIG_ORCHID
APP_NAME := acl_api_demo_orchid
endif
ifeq ($(APP_TYPE), ac494_orchid_ata)
CFLAGS += -DAC49_ORCHID_ATA_VOIP_TOOLKIT
CFLAGS += -DCONFIG_ORCHID
APP_NAME := acl_api_demo_orchid_ata
endif
ifeq ($(APP_TYPE), ac494_sdp_ata)
CFLAGS += -DAC494_SDP_ATA
CFLAGS += -DCONFIG_SDP_ATA
APP_NAME := acl_api_demo_sdp_ATA
endif
ifeq ($(APP_TYPE), ac494_smb)
CFLAGS += -DAC49_SMB_VOIP_TOOLKIT
CFLAGS += -DCONFIG_SMB
APP_NAME := acl_api_demo_SMB
endif
ifeq ($(APP_TYPE), ac494_tulip2_40)
CFLAGS += -DAC494_TULIP2_40
CFLAGS += -DCONFIG_TULIP2_40
APP_NAME := acl_api_demo_tulip2_40
endif
ifeq ($(APP_TYPE), ac494_tulip2_21)
CFLAGS += -DAC494_TULIP2_21
CFLAGS += -DCONFIG_TULIP2_21
APP_NAME := acl_api_demo_tulip2_21
endif

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
	@$(CC) $(REL_OBJS) $(LDFLAGS) -o $@
ifneq ($(PLATFORM_TYPE), i)
	@$(STRIP) --strip-unneeded  -R=.comment -R=.note $@
endif


$(TARGETDIR_DBG)/$(APP_NAME): $(DBG_OBJS) Makefile
	@echo linking  $@
	@[ -d $(TARGETDIR_DBG) ] || mkdir $(TARGETDIR_DBG)
	@$(CC) $(DBG_OBJS) $(LDFLAGS) -o $@

clean:
	@echo clean
	@rm -rf Object $(TARGETDIR_REL) $(TARGETDIR_DBG)

$(OBJDIR_REL)/%.o: %.c Makefile
	@[ -d $(DEPDIR_REL) ] || mkdir -p $(DEPDIR_REL)
	@[ -d $(OBJDIR_REL) ] || mkdir -p $(OBJDIR_REL)
	@echo compiling $<
ifneq ($(PLATFORM_TYPE), s)
#	@echo $(CC) -MMD -MF $(DEPDIR_REL)/$(*F).d -MT $@ $(CFLAGS) $(CFLAGS_REL) -c $< -o $@
	@$(CC) -MMD -MF $(DEPDIR_REL)/$(*F).d -MT $@ $(CFLAGS) $(CFLAGS_REL) -c $< -o $@
endif
ifeq ($(PLATFORM_TYPE), s)
#	@echo $(CC) $(CFLAGS) $(CFLAGS_REL) -c $< -o $@
	@$(CC) $(CFLAGS) $(CFLAGS_REL) -c $< -o $@
endif
	@[ -s $(DEPDIR_REL)/$(*F).d ] || rm -f $(DEPDIR_REL)/$(*F).d


$(OBJDIR_DBG)/%.o: %.c Makefile
	@[ -d $(DEPDIR_DBG) ] || mkdir -p $(DEPDIR_DBG)
	@[ -d $(OBJDIR_DBG) ] || mkdir -p $(OBJDIR_DBG)
	@echo compiling $<
	@$(CC) -MMD -MF $(DEPDIR_DBG)/$(*F).d -MT $@ $(CFLAGS) $(CFLAGS_DBG) -c $< -o $@
	@[ -s $(DEPDIR_DBG)/$(*F).d ] || rm -f $(DEPDIR_DBG)/$(*F).d


-include $(REL_DEPS) $(DBG_DEPS)
