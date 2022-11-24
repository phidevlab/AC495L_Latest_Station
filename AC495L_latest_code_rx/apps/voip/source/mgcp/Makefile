
LEGAL_PLATFORMS := p b i s grocx
ifneq ($(PLATFORM_TYPE), $(findstring $(PLATFORM_TYPE),$(LEGAL_PLATFORMS)))
$(error The PLATFORM_TYPE "$(PLATFORM_TYPE)" is not supported. Legal values are: $(LEGAL_PLATFORMS))
endif

ifndef PLATFORM_TYPE
echo Warning: PLATFORM_TYPE unspecified !!!
$(warning ****Warning: PLATFORM_TYPE unspecified !!!!)
endif

LEGAL_APPS := ac488_cvt ac483_cvt ac494_tulip_fxss ac494_sdp_ipp ac494_sdp_ata ac494_smb ceva ac494_tulip2_40 ac494_tulip2_21
ifneq ($(APP_TYPE), $(findstring $(APP_TYPE),$(LEGAL_APPS)))
$(error The APP_TYPE "$(APP_TYPE)" is not supported. Legal values are: $(LEGAL_APPS))
endif

ifndef APP_TYPE
echo Warning: APP_TYPE unspecified !!!
$(warning ****Warning: APP_TYPE unspecified !!!!)
endif

TOPDIR := $(shell pwd)/../../../

VPATH:= $(TOPDIR)core/acValidation/

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
CROSS_COMPILE := armeb-linux-
endif
ifeq ($(PLATFORM_TYPE), grocx)
CROSS_COMPILE=arm-linux-
endif
ifeq ($(PLATFORM_TYPE), s)
LINUX_DIR := $(TOPDIR)../buildroot/build_mipsel_nofpu/linux-2.6.10_dev-malta-mips2_fp_len/
CROSS_COMPILE=/opt/codefidence/mipsel-linux-uclibc/bin/
endif

CC	= $(CROSS_COMPILE)gcc
AR	= $(CROSS_COMPILE)ar

INCLUDEFLAGS= -I.

INCLUDEFLAGS += -I$(TOPDIR)voip/include/common/ 
INCLUDEFLAGS += -I$(TOPDIR)core/acTimerMechanism/ 
INCLUDEFLAGS += -I$(TOPDIR)core/acCfg/

INCLUDEFLAGS += -I$(TOPDIR)voip/include/mgcp/ 



ifeq ($(PLATFORM_TYPE), s)

DRV_DIR := $(LINUX_DIR)drivers/char/
INCLUDEFLAGS += -I$(DRV_DIR)ac49x_dsp/

else

ifeq ($(APP_TYPE), ceva)

INCLUDEFLAGS += -I$(TOPDIR)../kernel_soft_dsp/ac49x_ceva_dsp/ -I$(TOPDIR)../kernel_soft_dsp/ac49x_ceva_dsp/le88/

else

INCLUDEFLAGS +=  -I$(TOPDIR)../kernel/drivers/char/ac48_dsp/ \
			    -I$(TOPDIR)../kernel/drivers/char/ac48_dsp/voice_packetizer/ \
			    -I$(TOPDIR)../kernel/drivers/char/ac48_dsp/voice_packetizer/CommonResources/Include/ \
			    -I$(TOPDIR)../kernel/drivers/char/ac48_dsp/voice_packetizer/CommonResources/AC48xDrivers/Include/ \
			    -I$(TOPDIR)../kernel/drivers/char/ac48_dsp/voice_packetizer/VoicePacketizer/Include/ \
			    -I$(TOPDIR)../kernel/drivers/char/ac48_dsp/voice_packetizer/VoicePacketizer/RTCP_XR/include/ \
			    -I$(TOPDIR)../kernel/drivers/char/le88/ -I$(TOPDIR)../kernel/drivers/char/le88/vp_api_ii/driver/ 
endif
endif



ifeq ($(PLATFORM_TYPE), p)
PLATFORM_FOLDER := p_platform
INCLUDEFLAGS += -I$(TOPDIR)../kernel/platform/p_platform/include/
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
PLATFORM_FOLDER := s_platform
endif

RADVISION_FOLDER := radvision/mgcp

RADVISION_INCLUDES := $(TOPDIR)platform/$(PLATFORM_FOLDER)/$(RADVISION_FOLDER)/header_files/

### Radvision includes ###
INCLUDEFLAGS += -I$(RADVISION_INCLUDES)

WARNING = -Wall -W -Wno-unused-parameter -Wno-comment

OSFLAG = -DLINUX -D_GNU_SOURCE -D_REENTRANT
CFLAGS = $(OSFLAG) $(WARNING) $(INCLUDEFLAGS) -D_THREAD_SAFE
CFLAGS += -DACL_SIP_APPLICATION
CFLAGS_REL = -Os
CFLAGS_DBG = -g2 -DDEBUG -O

ifeq ($(PLATFORM_TYPE), p)
CFLAGS += -DP_PLATFORM -DAC48_CPE_VOIP_TOOLKIT
CFLAGS += -mcpu=arm920t -pipe -mstructure-size-boundary=8
endif

ifeq ($(PLATFORM_TYPE), b)

ifeq ($(APP_TYPE), ceva)
CFLAGS += -DB_CEVA_PLATFORM -DWORDS_BIGENDIAN
else
CFLAGS += -DB_PLATFORM -DAC48_CPE_VOIP_TOOLKIT -DWORDS_BIGENDIAN
CFLAGS += -fomit-frame-pointer
endif

endif

ifeq ($(PLATFORM_TYPE), i)
CFLAGS += -DI_PLATFORM -DAC48_CPE_VOIP_TOOLKIT -DWORDS_BIGENDIAN -mstructure-size-boundary=8
endif
ifeq ($(PLATFORM_TYPE), grocx)
CFLAGS += -DGROCX_PLATFORM -DAC48_CPE_VOIP_TOOLKIT -mstructure-size-boundary=8
endif
ifeq ($(PLATFORM_TYPE), s)
CFLAGS += -DS_PLATFORM 
endif

CFLAGS += -D_MONTAVISTA

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

endif


ifeq ($(APP_TYPE), ac488_cvt)
CFLAGS += -DAC488_CPE_VOIP_TOOLKIT
endif
ifeq ($(APP_TYPE), ac483_cvt)
CFLAGS += -DAC483_CPE_VOIP_TOOLKIT
endif

CFLAGS += -DCONFIG_RG_VOIP_RV_MGCP

OBJS = acl_validation.o
LIBA = validation.a


#.PHONY: all clean

all:  $(OBJS)

	$(AR) rs $(LIBA) $(OBJS)

	mv $(LIBA) $(TOPDIR)/platform/$(PLATFORM_FOLDER)/appl_archive_files/validation

clean:
	rm -f $(LIBA) $(OBJS)

	rm -f $(TOPDIR)/platform/$(PLATFORM_FOLDER)/appl_archive_files/validation/$(LIBA)

acl_authentication.o:
	$(CC) -c $(VPATH)acl_validation.c $(CFLAGS)
