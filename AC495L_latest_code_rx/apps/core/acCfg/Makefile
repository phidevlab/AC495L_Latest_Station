
LEGAL_APPS := p b i s grocx
ifneq ($(PLATFORM_TYPE), $(findstring $(PLATFORM_TYPE),$(LEGAL_APPS)))
$(error The PLATFORM_TYPE "$(PLATFORM_TYPE)" is not supported. Legal values are: $(LEGAL_APPS))
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


TOPDIR := $(shell pwd)/../../


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
CROSS_COMPILE := /usr/local/MIPSEL_UCLIBC/bin/mipsel-linux-uclibc-
else
CROSS_COMPILE := /opt/codefidence/mipsel-linux-uclibc/bin/
endif
STRIP=$(CROSS_COMPILE)strip

endif


CC	= $(CROSS_COMPILE)gcc
AR	= $(CROSS_COMPILE)ar

WARNING = -Wall -W -Wno-unused-parameter -Wno-comment

INCLUDEFLAGS= -I.

CFLAGS =  $(INCLUDEFLAGS) $(WARNING)

ifeq ($(APP_TYPE), ac488_cvt)
CFLAGS += -DAC488_CPE_VOIP_TOOLKIT
endif
ifeq ($(APP_TYPE), ac483_cvt)
CFLAGS += -DAC483_CPE_VOIP_TOOLKIT
endif


ifeq ($(PLATFORM_TYPE), p)
CFLAGS += -DP_PLATFORM -DAC48_CPE_VOIP_TOOLKIT
CFLAGS += -mcpu=arm920t -pipe -mstructure-size-boundary=8
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
CFLAGS += -DI_PLATFORM -DAC48_CPE_VOIP_TOOLKIT -mstructure-size-boundary=8
endif
ifeq ($(PLATFORM_TYPE), grocx)
CFLAGS += -DGROCX_PLATFORM -DAC48_CPE_VOIP_TOOLKIT -mstructure-size-boundary=8
endif
ifeq ($(PLATFORM_TYPE), s)
CFLAGS += -DS_PLATFORM
endif


OBJS = cfg_api.o cip_api.o cfg_handlers.o
LIBA = cfg_infrastructure.a


#.PHONY: all clean

all:  $(OBJS)
	@$(AR) rs $(LIBA) $(OBJS)
	
ifeq ($(PLATFORM_TYPE), p)
	cp $(LIBA) $(TOPDIR)/platform/p_platform/appl_archive_files/cfg_infrastructure/
endif

ifeq ($(PLATFORM_TYPE), b)

ifeq ($(APP_TYPE), ceva)
	cp $(LIBA) $(TOPDIR)/platform/b_ceva_platform/appl_archive_files/cfg_infrastructure/
else
	cp $(LIBA) $(TOPDIR)/platform/b_platform/appl_archive_files/cfg_infrastructure/
endif

endif

ifeq ($(PLATFORM_TYPE), i)
	cp $(LIBA) $(TOPDIR)/platform/i_platform/appl_archive_files/cfg_infrastructure/
endif
ifeq ($(PLATFORM_TYPE), grocx)
	cp $(LIBA) $(TOPDIR)/platform/grocx_platform/appl_archive_files/cfg_infrastructure/
endif
ifeq ($(PLATFORM_TYPE), s)

ifeq ($(APP_TYPE),320_ipp)
	cp $(LIBA) $(TOPDIR)/platform/3xx_platform/appl_archive_files/cfg_infrastructure/
else
	mkdir -p $(TOPDIR)/platform/s_platform/appl_archive_files/cfg_infrastructure/
	cp $(LIBA) $(TOPDIR)/platform/s_platform/appl_archive_files/cfg_infrastructure/
endif


	cp $(LIBA) $(TOPDIR)/platform/s_platform/appl_archive_files/cfg_infrastructure/
endif


clean:
	rm -f $(LIBA) $(OBJS)
	
ifeq ($(PLATFORM_TYPE), p)
	rm -f $(TOPDIR)/platform/p_platform/appl_archive_files/cfg_infrastructure/$(LIBA)
endif

ifeq ($(PLATFORM_TYPE), b)

ifeq ($(APP_TYPE), ceva)
	rm -f $(TOPDIR)/platform/b_ceva_platform/appl_archive_files/cfg_infrastructure/$(LIBA)
else
	rm -f $(TOPDIR)/platform/b_platform/appl_archive_files/cfg_infrastructure/$(LIBA)
endif

endif

ifeq ($(PLATFORM_TYPE), i)
	rm -f $(TOPDIR)/platform/i_platform/appl_archive_files/cfg_infrastructure/$(LIBA)
endif
ifeq ($(PLATFORM_TYPE), grocx)
	rm -f $(TOPDIR)/platform/grocx_platform/appl_archive_files/cfg_infrastructure/$(LIBA)
endif
ifeq ($(PLATFORM_TYPE), s)

ifeq ($(APP_TYPE),320_ipp)
	rm -f $(TOPDIR)/platform/3xx_platform/appl_archive_files/cfg_infrastructure/$(LIBA)
else
	rm -f $(TOPDIR)/platform/s_platform/appl_archive_files/cfg_infrastructure/$(LIBA)
endif

endif


