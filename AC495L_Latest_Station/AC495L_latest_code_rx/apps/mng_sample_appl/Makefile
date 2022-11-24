
LEGAL_PLATFORMS := p b i s grocx
ifneq ($(PLATFORM_TYPE), $(findstring $(PLATFORM_TYPE),$(LEGAL_PLATFORMS)))
$(error The PLATFORM_TYPE "$(PLATFORM_TYPE)" is not supported. Legal values are: $(LEGAL_PLATFORMS))
endif

ifndef PLATFORM_TYPE
echo Warning: PLATFORM_TYPE unspecified !!!
$(warning ****Warning: PLATFORM_TYPE unspecified !!!!)
endif

LEGAL_APPS := ac488_cvt ac483_cvt ac494_tulip_fxss ac494_sdp_ipp ac494_sdp_ata ac494_smb ceva 320_ipp ac494_tulip2_40 ac494_tulip2_21 ac494_orchid ac494_orchid_ata
ifneq ($(APP_TYPE), $(findstring $(APP_TYPE),$(LEGAL_APPS)))
$(error The APP_TYPE "$(APP_TYPE)" is not supported. Legal values are: $(LEGAL_APPS))
endif

ifndef APP_TYPE
echo Warning: APP_TYPE unspecified !!!
$(warning ****Warning: APP_TYPE unspecified !!!!)
endif


TOPDIR := $(shell pwd)/../

ifeq ($(PLATFORM_TYPE), p)
CFG_INFRASTRUCTURE_ARCHIVE_DIR := $(TOPDIR)platform/p_platform/appl_archive_files/cfg_infrastructure/
endif

ifeq ($(PLATFORM_TYPE), b)

ifeq ($(APP_TYPE), ceva)
CFG_INFRASTRUCTURE_ARCHIVE_DIR := $(TOPDIR)platform/b_ceva_platform/appl_archive_files/cfg_infrastructure/
else
CFG_INFRASTRUCTURE_ARCHIVE_DIR := $(TOPDIR)platform/b_platform/appl_archive_files/cfg_infrastructure/
endif

endif

ifeq ($(PLATFORM_TYPE), i)
CFG_INFRASTRUCTURE_ARCHIVE_DIR := $(TOPDIR)platform/i_platform/appl_archive_files/cfg_infrastructure/
endif
ifeq ($(PLATFORM_TYPE), grocx)
CFG_INFRASTRUCTURE_ARCHIVE_DIR := $(TOPDIR)platform/grocx_platform/appl_archive_files/cfg_infrastructure/
endif
ifeq ($(PLATFORM_TYPE), s)
CFG_INFRASTRUCTURE_ARCHIVE_DIR := $(TOPDIR)platform/s_platform/appl_archive_files/cfg_infrastructure/
endif


VPATH:= $(TOPDIR)

SRC_MNG_SAMPLE_APPL := 	mng_sample_appl

SRCS := $(SRC_MNG_SAMPLE_APPL) 


APP_NAME := mng_sample_appl


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
STRIP	= $(CROSS_COMPILE)strip
endif
SSTRIP	= $(CROSS_COMPILE)sstrip
OBJCOPY	= $(CROSS_COMPILE)objcopy
OBJDUMP	= $(CROSS_COMPILE)objdump

OBJDIR_REL:=Object/Release
DEPDIR_REL:=Object/Release
OBJDIR_DBG:=Object/Debug
DEPDIR_DBG:=Object/Debug

#LDFLAGS = -lpthread -lrt
##LDFLAGS = -lpthread 
ifeq ($(PLATFORM_TYPE), i)
LDFLAGS += -Wl -elf2flt 
endif

WARNING = -Wall -W -Wno-unused-parameter
#WARNING = -W -Wno-unused-parameter


INCLUDEFLAGS= -I$(TOPDIR) -I$(TOPDIR)core/acCfg -I$(TOPDIR)voip/include  -I$(LINUX_DIR)drivers/char/ac49x_dsp/

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
ifeq ($(PLATFORM_TYPE), grocx)
CFLAGS += -DGROCX_PLATFORM
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




CFG_INFRASTRUCTURE_ARCHIVE = $(CFG_INFRASTRUCTURE_ARCHIVE_DIR)/cfg_infrastructure.a


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


