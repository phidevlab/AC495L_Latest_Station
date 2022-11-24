
LEGAL_PLATFORMS := b s p grocx
ifneq ($(PLATFORM_TYPE), $(findstring $(PLATFORM_TYPE),$(LEGAL_PLATFORMS)))
$(error The PLATFORM_TYPE "$(PLATFORM_TYPE)" is not supported. Legal values are: $(LEGAL_PLATFORMS))
endif

ifndef PLATFORM_TYPE
echo Warning: PLATFORM_TYPE unspecified !!!
$(warning ****Warning: PLATFORM_TYPE unspecified !!!!)
endif

LEGAL_APPS := ac488_cvt ac483_cvt ac494_tulip_fxss ac494_sdp_ipp ac494_sdp_ata ac494_smb ceva ac494_tulip2_40 ac494_tulip2_21 ac494_orchid ac494_orchid_ata
ifneq ($(APP_TYPE), $(findstring $(APP_TYPE),$(LEGAL_APPS)))
$(error The APP_TYPE "$(APP_TYPE)" is not supported. Legal values are: $(LEGAL_APPS))
endif

ifndef APP_TYPE
echo Warning: APP_TYPE unspecified !!!
$(warning ****Warning: APP_TYPE unspecified !!!!)
endif

ifndef SLIC_DEVICE
SLIC_DEVICE := legerity
endif

LEGAL_SLIC := legerity silabs
ifneq ($(SLIC_DEVICE), $(findstring $(SLIC_DEVICE),$(LEGAL_SLIC)))
$(error The SLIC_DEVICE "$(SLIC_DEVICE)" is not supported. Legal values are: $(LEGAL_SLIC))
endif


TOPDIR := $(shell pwd)/../

VPATH:= $(TOPDIR)/gateway_api_sample_application/source $(TOPDIR)/voip/source/common/ $(TOPDIR)/gateway_api_sample_application/network_handling/source

SRC_GATEWAY_API := acGatewayAPI	

ifeq ($(PLATFORM_TYPE), s)

ifeq ($(APP_TYPE), ac494_sdp_ipp)
SRC_GATEWAY_API += acIPPhoneAPI
endif

ifeq ($(APP_TYPE), ac494_orchid)
SRC_GATEWAY_API += acIPPhoneAPI
endif

endif 
SRC_NETWORK_HANDLING := 	gw_api_localNetRtcpRx gw_api_localNetRtcpTerm gw_api_localNetRtcpTx \
						gw_api_localNetRx gw_api_localNetTerm gw_api_localNetTx

SRCS := gw_api_sample_appl media_events_handling $(SRC_GATEWAY_API) $(SRC_NETWORK_HANDLING)


APP_NAME := gw_api_sample_appl


ifeq ($(PLATFORM_TYPE), p)
CROSS_COMPILE=arm-linux-uclibc-
endif

ifeq ($(PLATFORM_TYPE), b)

ifeq ($(APP_TYPE), ceva)
CROSS_COMPILE=mips-wrs-linux-gnu-mips_softfp-glibc_small-
else
CROSS_COMPILE=mips-linux-
endif

endif

ifeq ($(PLATFORM_TYPE), i)
CROSS_COMPILE=armeb-linux-
endif
ifeq ($(PLATFORM_TYPE), s)

LINUX_DIR := $(TOPDIR)../buildroot/build_mipsel_nofpu/linux-2.6.10_dev-malta-mips2_fp_len/
CROSS_COMPILE := /opt/codefidence/mipsel-linux-uclibc/bin/
STRIP=$(CROSS_COMPILE)strip

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

WARNING = -Wall -W -Wno-unused-parameter
#WARNING = -W -Wno-unused-parameter

INCLUDEFLAGS = -I$(TOPDIR)/gateway_api_sample_application/include -I$(TOPDIR)/voip/include/common/ -I$(TOPDIR)/gateway_api_sample_application/network_handling/include/ 

ifneq ($(PLATFORM_TYPE), s)
ifneq ($(APP_TYPE), ceva)
INCLUDEFLAGS +=  -I$(TOPDIR)../kernel/drivers/char/ac48_dsp/ \
                            -I$(TOPDIR)../kernel/drivers/char/ac48_dsp/voice_packetizer/ \
                            -I$(TOPDIR)../kernel/drivers/char/ac48_dsp/voice_packetizer/CommonResources/Include/ \
                            -I$(TOPDIR)../kernel/drivers/char/ac48_dsp/voice_packetizer/CommonResources/AC48xDrivers/Include/ \
                            -I$(TOPDIR)../kernel/drivers/char/ac48_dsp/voice_packetizer/VoicePacketizer/Include/ \
                            -I$(TOPDIR)../kernel/drivers/char/ac48_dsp/voice_packetizer/VoicePacketizer/RTCP_XR/include/ \
                            -I$(TOPDIR)../kernel/drivers/char/le88/ -I$(TOPDIR)../kernel/drivers/char/le88/vp_api_ii/driver/
endif
endif

ifeq ($(PLATFORM_TYPE), s)

DRV_DIR := $(LINUX_DIR)drivers/char/

INCLUDEFLAGS += -I$(TOPDIR)voip/include/ac494_include \
                                -I$(LINUX_DIR)include/ \
				-I$(DRV_DIR)include/ \
                                -I$(DRV_DIR)ac49x_dsp/ \
                                -I$(DRV_DIR)le88/ \
                                -I$(DRV_DIR)le88/vp_api_ii/driver/ \
                                -I$(DRV_DIR)w682388/ \
				-I$(TOPDIR)voip/include/common/

INCLUDEFLAGS += -I$(LINUX_DIR)/include/asm-mips/mach-avalanche/generic/
#INCLUDEFLAGS += -I$(LINUX_DIR)include/asm/avalanche/titan/ 

endif

ifeq ($(APP_TYPE), ceva)
INCLUDEFLAGS +=  -I$(TOPDIR)../../kernel_soft_dsp/ac49x_ceva_dsp/ -I$(TOPDIR)../../kernel_soft_dsp/le88/
endif

ifeq ($(PLATFORM_TYPE), p)
CDEFS := -mcpu=arm920t -pipe -mstructure-size-boundary=8
endif
ifeq ($(PLATFORM_TYPE), i)
CDEFS := -mstructure-size-boundary=8
endif

OSFLAG = -DLINUX -D_GNU_SOURCE -D_REENTRANT $(CDEFS)
CFLAGS = $(OSFLAG) $(WARNING) $(INCLUDEFLAGS)
CFLAGS_REL = -Os
CFLAGS_DBG = -g2 -DDEBUG -O

ifeq ($(APP_TYPE), ac488_cvt)
CFLAGS += -DAC488_CPE_VOIP_TOOLKIT
endif
ifeq ($(APP_TYPE), ac483_cvt)
CFLAGS += -DAC483_CPE_VOIP_TOOLKIT
endif


ifeq ($(PLATFORM_TYPE), p)
CFLAGS += -DP_PLATFORM -DAC48_CPE_VOIP_TOOLKIT
endif

ifeq ($(PLATFORM_TYPE), b)

ifeq ($(APP_TYPE), ceva)
CFLAGS += -DB_CEVA_PLATFORM
else
CFLAGS += -DB_PLATFORM -DAC48_CPE_VOIP_TOOLKIT
CFLAGS += -fomit-frame-pointer
endif

endif

ifeq ($(PLATFORM_TYPE), i)
CFLAGS += -DI_PLATFORM -DAC48_CPE_VOIP_TOOLKIT
endif
ifeq ($(PLATFORM_TYPE), s)
CFLAGS += -DS_PLATFORM
endif


ifeq ($(PLATFORM_TYPE), s)

# convert APP_TYPE to uppercase
ifeq ($(APP_TYPE), ac494_tulip_fxss)
CFLAGS += -DAC494_TULIP_FXSS
CFLAGS += -DCONFIG_TULIP
endif
ifeq ($(APP_TYPE), ac494_sdp_ipp)
CFLAGS += -DAC494_SDP_IPPHONE
CFLAGS += -DCONFIG_SDP_IPPHONE
CFLAGS += -DGW_APPL_SDP
endif
ifeq ($(APP_TYPE), ac494_sdp_ata)
CFLAGS += -DAC494_SDP_ATA
CFLAGS += -DCONFIG_SDP_ATA
CFLAGS += -DGW_APPL_SDP
endif
ifeq ($(APP_TYPE), ac494_smb)
CFLAGS += -DAC49_SMB_VOIP_TOOLKIT
CFLAGS += -DCONFIG_SMB
endif
ifeq ($(APP_TYPE), ac494_tulip2_40)
CFLAGS += -DAC494_TULIP2_40
CFLAGS += -DCONFIG_TULIP2_40
endif
ifeq ($(APP_TYPE), ac494_tulip2_21)
CFLAGS += -DAC494_TULIP2_21
CFLAGS += -DCONFIG_TULIP2_21
endif

ifeq ($(APP_TYPE), ac494_orchid)
CFLAGS += -DAC49_ORCHID_VOIP_TOOLKIT
CFLAGS += -DCONFIG_ORCHID
CFLAGS += -DGW_APPL_SDP
endif

ifeq ($(APP_TYPE), ac494_orchid_ata)
CFLAGS += -DAC49_ORCHID_ATA_VOIP_TOOLKIT
CFLAGS += -DCONFIG_ORCHID_ATA
CFLAGS += -DGW_APPL_SDP
endif

endif

ifeq ($(SLIC_DEVICE), legerity)
CFLAGS += -DFXS_LEGERITY
endif
ifeq ($(SLIC_DEVICE), silabs)
CFLAGS += -DFXS_SILABS -DFXO_SILABS
endif

CFLAGS += -DPOLLING_METHOD__BLOCKING
CFLAGS += $(CUST_CFLAGS)

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
	@$(CC) $(REL_OBJS) $(CFG_INFRASTRUCTURE_ARCHIVE) $(LDFLAGS) -o $@
ifneq ($(PLATFORM_TYPE), i)
	@$(STRIP) --strip-unneeded  -R=.comment -R=.note $@
endif


$(TARGETDIR_DBG)/$(APP_NAME): $(DBG_OBJS) Makefile
	@echo linking  $@
	@[ -d $(TARGETDIR_DBG) ] || mkdir $(TARGETDIR_DBG)
	@$(CC) $(DBG_OBJS) $(CFG_INFRASTRUCTURE_ARCHIVE) $(LDFLAGS) -o $@

	
clean:
	@echo clean
	@rm -rf Object $(TARGETDIR_REL) $(TARGETDIR_DBG)

$(OBJDIR_REL)/%.o: %.c Makefile
	@[ -d $(DEPDIR_REL) ] || mkdir -p $(DEPDIR_REL)
	@[ -d $(OBJDIR_REL) ] || mkdir -p $(OBJDIR_REL)
	@echo compiling $<
ifneq ($(PLATFORM_TYPE), s)
	@echo $(CC) -MMD -MF $(DEPDIR_REL)/$(*F).d -MT $@ $(CFLAGS) $(CFLAGS_REL) -c $< -o $@
	@$(CC) -MMD -MF $(DEPDIR_REL)/$(*F).d -MT $@ $(CFLAGS) $(CFLAGS_REL) -c $< -o $@
endif
ifeq ($(PLATFORM_TYPE), s)
	@echo $(CC) $(CFLAGS) $(CFLAGS_REL) -c $< -o $@
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


