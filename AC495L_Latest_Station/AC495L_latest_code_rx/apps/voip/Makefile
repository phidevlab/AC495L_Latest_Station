
LEGAL_PLATFORMS := p b i s grocx
ifneq ($(PLATFORM_TYPE), $(findstring $(PLATFORM_TYPE),$(LEGAL_PLATFORMS)))
$(error The PLATFORM_TYPE "$(PLATFORM_TYPE)" is not supported. Legal values are: $(LEGAL_PLATFORMS))
endif

ifndef PLATFORM_TYPE
echo Warning: PLATFORM_TYPE unspecified !!!
$(warning ****Warning: PLATFORM_TYPE unspecified !!!!)
endif

LEGAL_APPS := 320_ipp ac488_cvt ac483_cvt ac494_tulip_fxss ac494_sdp_ipp ac494_sdp_ata ac494_smb ceva  ac494_tulip2_40 ac494_tulip2_21 ac494_orchid ac494_orchid_ata
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

LEGAL_VOIP_PROT := sip mgcp
ifneq ($(VOIP_PROT), $(findstring $(VOIP_PROT),$(LEGAL_VOIP_PROT)))
$(error The VOIP_PROT "$(VOIP_PROT)" is not supported. Legal values are: $(LEGAL_VOIP_PROT))
endif


LEGAL_STUN_SUPPORT_OPTION:= on off
ifneq ($(STUN_SUPPORT), $(findstring $(STUN_SUPPORT),$(LEGAL_STUN_SUPPORT_OPTION)))
$(error The STUN_SUPPORT "$(STUN_SUPPORT)" is not supported. Legal values are: $(LEGAL_STUN_SUPPORT_OPTION))
endif


TOPDIR := $(shell pwd)/../

ifeq ($(PLATFORM_TYPE), p)
PLATFORM_FOLDER := p_platform
endif

ifeq ($(PLATFORM_TYPE), b)

ifeq ($(APP_TYPE), ceva)
PLATFORM_FOLDER := b_ceva_platform
else
PLATFORM_FOLDER := b_platform
endif

endif

ifeq ($(PLATFORM_TYPE), i)
PLATFORM_FOLDER := i_platform
endif
ifeq ($(PLATFORM_TYPE), grocx)
PLATFORM_FOLDER := grocx_platform
endif

ifeq ($(PLATFORM_TYPE), s)

ifeq ($(APP_TYPE),320_ipp)
PLATFORM_FOLDER := 3xx_platform
else
PLATFORM_FOLDER := s_platform
endif

endif

ifeq ($(VOIP_PROT), mgcp)
RADVISION_FOLDER := radvision/mgcp
else
RADVISION_FOLDER := radvision/sip
endif



RV_ARCHIVE_DIR := $(TOPDIR)platform/$(PLATFORM_FOLDER)/$(RADVISION_FOLDER)/archive_files/
#RV_ARCHIVE_DIR := $(TOPDIR)platform/$(PLATFORM_FOLDER)/$(RADVISION_FOLDER)/archive_files_with_logs/
MD5_ARCHIVE_DIR := $(TOPDIR)platform/$(PLATFORM_FOLDER)/appl_archive_files/md5/
CFG_INFRASTRUCTURE_ARCHIVE_DIR := $(TOPDIR)platform/$(PLATFORM_FOLDER)/appl_archive_files/cfg_infrastructure/
VALIDATION_ARCHIVE_DIR := $(TOPDIR)platform/$(PLATFORM_FOLDER)/appl_archive_files/validation/

ifeq ($(APP_TYPE),320_ipp)
MESSAGE_QUEUE_ARCHIVE_DIR=$(TOPDIR)/../3xxHD/MSGQ/
endif

ifeq ($(APP_TYPE), ac494_orchid)
MICROWINDOWS_ARCHIVE_DIR=$(TOPDIR)ac494_utils/microwindows-0.91/src/lib/
endif

VPATH:= $(TOPDIR) $(TOPDIR)voip/source $(TOPDIR)voip/source/common \
		$(TOPDIR)core/acMessageQueue $(TOPDIR)core/acTimerMechanism 

ifeq ($(VOIP_PROT), mgcp)
VPATH +=	$(TOPDIR)voip/source/mgcp
else
VPATH +=	$(TOPDIR)voip/source/sip
VPATH +=	$(TOPDIR)voip/source/sip/olstun
endif


ifeq ($(PLATFORM_TYPE), s)
VPATH += $(TOPDIR)voip/source/ac494_source
endif

SRC_VOIP:= 	iphone_localNetRtcpRx voip_media_handler iphone_fxs \
			iphone_localNetRtcpTerm iphone_localNetRtcpTx iphone_localNetRx iphone_localNetTerm \
			iphone_localNetTx iphone_paging acGatewayAPI voip_task_specific_handlers
SRC_VOIP+=	voip_main acl_log acl_dsp_telephony_events ata_stubs acl_line_connection call_mgt_stubs acl_dsp_telephony_commands \
			voip_rtp_utils voip_utils acl_voip_conf led acl_ui_commands enums voip_status_handler license acl_telephony_adv_sip_features  config_manager server_coms ui radvision_events keypad_interface


ifeq ($(PLATFORM_TYPE), s)

ifeq ($(APP_TYPE), 320_ipp)
SRC_VOIP += acl_keypad_adaptation_layer acIPPhoneAPI IPPMessageHandler
else 

ifeq ($(APP_TYPE), ac494_sdp_ipp)
SRC_VOIP += iphone_hwLcd iphone_hwRtc iphone_hwKeypad acl_keypad_adaptation_layer acIPPhoneAPI acl_LCD_adaptation_layer nanox_hwLcd
else 
ifeq ($(APP_TYPE), ac494_orchid)
SRC_VOIP += iphone_hwLcd iphone_hwRtc iphone_hwKeypad acl_keypad_adaptation_layer acIPPhoneAPI acl_LCD_adaptation_layer nanox_hwLcd
else
ifeq ($(APP_TYPE), ac494_orchid_ata)
SRC_VOIP += iphone_fxo iphone_fxsWinbond
else
SRC_VOIP += iphone_fxo iphone_fxsWinbond
endif
endif
endif

endif 

endif 




ifeq ($(VOIP_PROT), mgcp)
SRC_MGCP :=  acl_rv_mgcp_api acl_mgcp_call_mngr
else
SRC_SIP :=	acl_rv_sip_api acl_sip_call_mngr acl_call_sip_init

#supply .o file no need this 
ifeq ($(STUN_SUPPORT), on)
SRC_SIP +=      olStunClient olStunMsg olStunData
endif
endif
 

SRC_AC_MESSAGE_QUEUE := acBufferPool acBufferList quLinkedQueue linuxHal acl_MsgQ

SRC_AC_TIMER_MECHANISM := acTimers

SRCS := $(SRC_VOIP) $(SRC_AC_MESSAGE_QUEUE) $(SRC_AC_TIMER_MECHANISM) 

ifeq ($(VOIP_PROT), mgcp)
SRCS += $(SRC_MGCP)
else
SRCS +=	$(SRC_SIP)
endif

APP_NAME := voip_task

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
ifeq ($(PLATFORM_TYPE), grocx)
CROSS_COMPILE=arm-linux-
endif
ifeq ($(PLATFORM_TYPE), s)

ifeq ($(APP_TYPE),320_ipp)
LINUX_DIR := $(TOPDIR)../3xxHD/AC494_build_2_2_0/buildroot/build_mipsel_nofpu/linux-2.6.10_dev-malta-mips2_fp_len/
CROSS_COMPILE := /usr/local/MIPSEL_UCLIBC/bin/mipsel-linux-uclibc-
else
LINUX_DIR := $(TOPDIR)../buildroot/build_mipsel_nofpu/linux-2.6.10_dev-malta-mips2_fp_len/
CROSS_COMPILE := /opt/codefidence/mipsel-linux-uclibc/bin/
endif
STRIP=$(CROSS_COMPILE)strip


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

ifeq ($(PLATFORM_TYPE), p)
LDFLAGS += -L$(TOPDIR)platform/$(PLATFORM_FOLDER)/appl_archive_files/libraries/ -lPASONU 
endif
ifneq ($(PLATFORM_TYPE), s)
LDFLAGS += -lrt
endif


ifeq ($(PLATFORM_TYPE), i)
LDFLAGS += -Wl -elf2flt 
endif

WARNING = -Wall -W -Wno-unused-parameter -Wno-comment
#WARNING = -W -Wno-unused-parameter 

ifneq ($(PLATFORM_TYPE), s)
ifneq ($(APP_TYPE), ceva)
INCLUDEFLAGS =  -I$(TOPDIR)../kernel/drivers/char/ac48_dsp/ \
			    -I$(TOPDIR)../kernel/drivers/char/ac48_dsp/voice_packetizer/ \
			    -I$(TOPDIR)../kernel/drivers/char/ac48_dsp/voice_packetizer/CommonResources/Include/ \
			    -I$(TOPDIR)../kernel/drivers/char/ac48_dsp/voice_packetizer/CommonResources/AC48xDrivers/Include/ \
			    -I$(TOPDIR)../kernel/drivers/char/ac48_dsp/voice_packetizer/VoicePacketizer/Include/ \
			    -I$(TOPDIR)../kernel/drivers/char/ac48_dsp/voice_packetizer/VoicePacketizer/RTCP_XR/include/ \
			    -I$(TOPDIR)../kernel/drivers/char/le88/ -I$(TOPDIR)../kernel/drivers/char/le88/vp_api_ii/driver/ 
endif
endif

ifeq ($(APP_TYPE), ceva)
INCLUDEFLAGS =  -I$(TOPDIR)../kernel_soft_dsp/ac49x_ceva_dsp/ -I$(TOPDIR)../kernel_soft_dsp/le88/
endif

INCLUDEFLAGS += -I$(TOPDIR)/ 
INCLUDEFLAGS += -I$(TOPDIR)core/acCfg/
INCLUDEFLAGS += -I$(TOPDIR)voip/include/ 
INCLUDEFLAGS += -I$(TOPDIR)voip/include/common/ 
INCLUDEFLAGS += -I$(TOPDIR)core/acMessageQueue/ 
INCLUDEFLAGS += -I$(TOPDIR)core/acTimerMechanism/ 

ifeq ($(VOIP_PROT), mgcp)
INCLUDEFLAGS += -I$(TOPDIR)voip/include/mgcp/ 
else
INCLUDEFLAGS += -I$(TOPDIR)voip/include/sip/ 
INCLUDEFLAGS += -I$(TOPDIR)voip/include/sip/olstun/
endif


ifeq ($(PLATFORM_TYPE), s)

DRV_DIR := $(LINUX_DIR)drivers/char/

				 
INCLUDEFLAGS += -I$(TOPDIR)voip/include/ac494_include \
				-I$(LINUX_DIR)include/ \
				-I$(KERNELDIR)/arch/mips/include/asm/mach-avalanche/generic \
				-I$(DRV_DIR)include/ \
				-I$(DRV_DIR)ac49x_dsp/ \
				-I$(DRV_DIR)avalanche_lcd/ \
				-I$(DRV_DIR)le88/ \
			    -I$(DRV_DIR)le88/vp_api_ii/driver/ \
				-I$(DRV_DIR)w682388/

INCLUDEFLAGS +=	-I$(LINUX_DIR)/include/asm-mips/mach-avalanche/generic/
#INCLUDEFLAGS += -I$(LINUX_DIR)include/asm/avalanche/titan/ 

ifeq ($(APP_TYPE), ac494_orchid)
INCLUDEFLAGS += -I$(STAGING_DIR)/usr/include/nanox/
endif

ifeq ($(APP_TYPE),320_ipp)
INCLUDEFLAGS += -I$(TOPDIR)../3xxHD/ -I$(TOPDIR)../3xxHD/MSGQ
endif

endif

RADVISION_INCLUDES := $(TOPDIR)platform/$(PLATFORM_FOLDER)/$(RADVISION_FOLDER)/header_files/

### Radvision includes ###
INCLUDEFLAGS += -I$(RADVISION_INCLUDES)

ALL_ARCHIVE_LIST += $(foreach dir,$(VALIDATION_ARCHIVE_DIR),$(wildcard $(dir)/*.a))


ifeq ($(VOIP_PROT), mgcp)
ALL_ARCHIVE_LIST += $(RV_ARCHIVE_DIR)/libmgcp.a
ALL_ARCHIVE_LIST += $(RV_ARCHIVE_DIR)/libsdp.a 
ALL_ARCHIVE_LIST += $(RV_ARCHIVE_DIR)/libcore.a
else
#ALL_ARCHIVE_LIST += $(foreach dir,$(RV_ARCHIVE_DIR),$(wildcard $(dir)/*.a)) 
#ALL_ARCHIVE_LIST += $(RV_ARCHIVE_DIR)/libepp.a 
#ALL_ARCHIVE_LIST += $(RV_ARCHIVE_DIR)/libIppMediaControl.a 
ALL_ARCHIVE_LIST += $(RV_ARCHIVE_DIR)/libMtf.a 
ALL_ARCHIVE_LIST += $(RV_ARCHIVE_DIR)/librvsip.a
ALL_ARCHIVE_LIST += $(RV_ARCHIVE_DIR)/librvsdputil.a 
ALL_ARCHIVE_LIST += $(RV_ARCHIVE_DIR)/librvsdp.a 
ALL_ARCHIVE_LIST += $(RV_ARCHIVE_DIR)/librvads.a 
ALL_ARCHIVE_LIST += $(RV_ARCHIVE_DIR)/librvcommon.a 
endif

ALL_ARCHIVE_LIST += $(foreach dir,$(MD5_ARCHIVE_DIR),$(wildcard $(dir)/*.a)) 
ALL_ARCHIVE_LIST += $(foreach dir,$(CFG_INFRASTRUCTURE_ARCHIVE_DIR),$(wildcard $(dir)/*.a)) 

ifeq ($(APP_TYPE),320_ipp)
ALL_ARCHIVE_LIST += $(foreach dir,$(MESSAGE_QUEUE_ARCHIVE_DIR),$(wildcard $(dir)/*.a)) 
endif

ifeq ($(APP_TYPE), ac494_orchid)
#ALL_ARCHIVE_LIST += $(foreach dir,$(MICROWINDOWS_ARCHIVE_DIR),$(wildcard $(dir)/*.a)) 
endif

ifeq ($(PLATFORM_TYPE), p)
CDEFS := -mcpu=arm920t -pipe -mstructure-size-boundary=8
endif
ifeq ($(PLATFORM_TYPE), i)
CDEFS := -mstructure-size-boundary=8
endif
ifeq ($(PLATFORM_TYPE), grocx)
CDEFS := -mstructure-size-boundary=8
endif


OSFLAG = -DLINUX -D_GNU_SOURCE -D_REENTRANT $(CDEFS)
CFLAGS = $(OSFLAG) $(WARNING) $(INCLUDEFLAGS) -D_THREAD_SAFE
ifeq ($(VOIP_PROT), mgcp)
CFLAGS += -DACL_MGCP_APPLICATION
endif
ifneq ($(VOIP_PROT), mgcp)
CFLAGS += -DACL_SIP_APPLICATION
ifeq ($(STUN_SUPPORT), on)
CFLAGS += -DSTUN -DRV_MTF_STUN -DOLSTUN_DEBUG
endif 
endif
CFLAGS_REL = -Os
CFLAGS_DBG = -g2 -DDEBUG -O
#CFLAGS_DBG = -g -DDEBUG


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
endif

endif

ifeq ($(PLATFORM_TYPE), i)
CFLAGS += -DI_PLATFORM -DAC48_CPE_VOIP_TOOLKIT
endif
ifeq ($(PLATFORM_TYPE), grocx)
CFLAGS += -DGROCX_PLATFORM -DAC48_CPE_VOIP_TOOLKIT
endif
ifeq ($(PLATFORM_TYPE), s)
CFLAGS += -DS_PLATFORM
endif


ifeq ($(PLATFORM_TYPE), p)
TARGET_CPU_ENDIAN_LINE := RV_ARCH_LITTLE_ENDIAN
endif

ifeq ($(PLATFORM_TYPE), b)

ifeq ($(APP_TYPE), ceva)
TARGET_CPU_ENDIAN_LINE := RV_ARCH_BIG_ENDIAN
else
TARGET_CPU_ENDIAN_LINE := RV_ARCH_BIG_ENDIAN
endif

endif

ifeq ($(PLATFORM_TYPE), i)
TARGET_CPU_ENDIAN_LINE := RV_ARCH_BIG_ENDIAN
endif
ifeq ($(PLATFORM_TYPE), grocx)
TARGET_CPU_ENDIAN_LINE := RV_ARCH_LITTLE_ENDIAN
endif
ifeq ($(PLATFORM_TYPE), s)
TARGET_CPU_ENDIAN_LINE := RV_ARCH_LITTLE_ENDIAN
endif

ifeq ($(SLIC_DEVICE), legerity)
CFLAGS += -DFXS_LEGERITY
endif
ifeq ($(SLIC_DEVICE), silabs)
CFLAGS += -DFXS_SILABS -DFXO_SILABS
endif



TARGET_CPU_BITS_LINE := RV_ARCH_BITS_32
COMPILER_TOOL_LINE := RV_TOOL_TYPE_GNU
TARGET_OS_LINE := RV_OS_TYPE_LINUX
TARGET_OS_VERSION_LINE := RV_OS_LINUX_MVISTA_3_1

# Set CFLAGS according to the variables defined in default.mak
CFLAGS += -DRV_TOOL_TYPE=$(COMPILER_TOOL_LINE)
CFLAGS += -DRV_OS_TYPE=$(TARGET_OS_LINE)
CFLAGS += -DRV_OS_VERSION=$(TARGET_OS_VERSION_LINE)
CFLAGS += -DRV_ARCH_ENDIAN=$(TARGET_CPU_ENDIAN_LINE)
CFLAGS += -DRV_ARCH_BITS=$(TARGET_CPU_BITS_LINE)
CFLAGS += -DRV_RELEASE
#CFLAGS += -DRV_NOLOG
ifeq ($(VOIP_PROT), mgcp)
CFLAGS += -D_MONTAVISTA
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
endif
ifeq ($(APP_TYPE), 320_ipp)
CFLAGS += -DAC494_SDP_IPPHONE
CFLAGS += -DCONFIG_SDP_IPPHONE
CFLAGS += -DCONFIG_GTEK
endif
ifeq ($(APP_TYPE), ac494_sdp_ata)
CFLAGS += -DAC494_SDP_ATA
CFLAGS += -DCONFIG_SDP_ATA
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
endif

ifeq ($(APP_TYPE), ac494_orchid_ata)
CFLAGS += -DAC49_ORCHID_ATA_VOIP_TOOLKIT
CFLAGS += -DCONFIG_ORCHID_ATA
endif

endif

ifeq ($(APP_TYPE),320_ipp)
CFLAGS += -DCONFIG_RG_IPPHONE
CFLAGS += -DCONFIG_GTEK
else 
ifeq ($(APP_TYPE), ac494_sdp_ipp)
CFLAGS += -DCONFIG_RG_IPPHONE
else
ifeq ($(APP_TYPE), ac494_orchid)
CFLAGS += -DCONFIG_RG_IPPHONE
else
CFLAGS += -DCONFIG_RG_ATA
endif
endif

endif 


ifeq ($(VOIP_PROT), mgcp)
CFLAGS += -DCONFIG_RG_VOIP_RV_MGCP
else
CFLAGS += -DCONFIG_RG_VOIP_RV_SIP
endif

#Mark Chen: define flags externally
CFLAGS += $(CUST_CFLAGS)
INCLUDEFLAGS += $(CUST_INCLUDEFLAGS)
LDFLAGS += $(CUST_LDFLAGS)

REL_OBJS:= $(SRCS:%=$(OBJDIR_REL)/%.o)
REL_DEPS:= $(SRCS:%=$(DEPDIR_REL)/%.d)

#AlexChen add for olstun
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
	@$(CC) $(REL_OBJS) $(ALL_ARCHIVE_LIST) $(LDFLAGS) -o $@
ifneq ($(PLATFORM_TYPE), i)
	@$(STRIP) --strip-unneeded  -R=.comment -R=.note $@
endif


$(TARGETDIR_DBG)/$(APP_NAME): $(DBG_OBJS) Makefile
	@echo linking  $@
	@[ -d $(TARGETDIR_DBG) ] || mkdir $(TARGETDIR_DBG)
	@$(CC) $(DBG_OBJS) $(ALL_ARCHIVE_LIST) $(LDFLAGS) -o $@

	
clean:
	@echo clean
	@rm -rf Object $(TARGETDIR_REL) $(TARGETDIR_DBG)


$(OBJDIR_REL)/%.o: %.c Makefile
	@[ -d $(DEPDIR_REL) ] || mkdir -p $(DEPDIR_REL)
	@[ -d $(OBJDIR_REL) ] || mkdir -p $(OBJDIR_REL)
ifeq ($(STUN_SUPPORT), on)
	@cp source/sip/olstun/Release/*.o $(OBJDIR_REL)
endif
	@echo compiling $<
ifneq ($(PLATFORM_TYPE), s)
	@$(CC) -MMD -MF $(DEPDIR_REL)/$(*F).d -MT $@ $(CFLAGS) $(CFLAGS_REL) -c $< -o $@
	@[ -s $(DEPDIR_REL)/$(*F).d ] || rm -f $(DEPDIR_REL)/$(*F).d
endif
ifeq ($(PLATFORM_TYPE), s)
#	@echo $(CC) $(CFLAGS) $(CFLAGS_REL) -c $< -o $@
	@$(CC) $(CFLAGS) $(CFLAGS_REL) -c $< -o $@
	@[ -s $(DEPDIR_REL)/$(*F).d ] || rm -f $(DEPDIR_REL)/$(*F).d
endif


$(OBJDIR_DBG)/%.o: %.c Makefile
	@[ -d $(DEPDIR_DBG) ] || mkdir -p $(DEPDIR_DBG)
	@[ -d $(OBJDIR_DBG) ] || mkdir -p $(OBJDIR_DBG)
ifeq ($(STUN_SUPPORT), on)
	@cp source/sip/olstun/Debug/*.o $(OBJDIR_DBG)
	@cp source/sip/olstun/Debug/*.d $(DEPDIR_DBG)
endif
	@echo compiling $<
	@$(CC) -MMD -MF $(DEPDIR_DBG)/$(*F).d -MT $@ $(CFLAGS) $(CFLAGS_DBG) -c $< -o $@
	@[ -s $(DEPDIR_DBG)/$(*F).d ] || rm -f $(DEPDIR_DBG)/$(*F).d


-include $(REL_DEPS) $(DBG_DEPS)


