
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
CROSS_COMPILE := armeb-linux-
endif
ifeq ($(PLATFORM_TYPE), grocx)
CROSS_COMPILE :=arm-linux-
endif
ifeq ($(PLATFORM_TYPE), s)

ifeq ($(APP_TYPE),320_ipp)
CROSS_COMPILE := /usr/local/MIPSEL_UCLIBC/bin/mipsel-linux-uclibc-
else
CROSS_COMPILE=/opt/codefidence/mipsel-linux-uclibc/bin/
endif 

endif


CC	= $(CROSS_COMPILE)gcc
AR	= $(CROSS_COMPILE)ar


INCLUDEFLAGS= -I.

CFLAGS =  $(INCLUDEFLAGS)
ifeq ($(PLATFORM_TYPE), p)
CFLAGS += -DP_PLATFORM  -DAC48_CPE_VOIP_TOOLKIT
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
CFLAGS += -DI_PLATFORM -DAC48_CPE_VOIP_TOOLKIT -DWORDS_BIGENDIAN
endif
ifeq ($(PLATFORM_TYPE), grocx)
CFLAGS += -DGROCX_PLATFORM -DAC48_CPE_VOIP_TOOLKIT
endif
ifeq ($(PLATFORM_TYPE), s)
CFLAGS += -DS_PLATFORM 
endif


OBJS = md5.o obscure.o
LIBA = md5.a


#.PHONY: all clean

all:  $(OBJS)
	$(AR) rs $(LIBA) $(OBJS)
	cp $(LIBA) $(TOPDIR)/platform/s_platform/appl_archive_files/md5/
clean:
	rm -f $(LIBA) $(OBJS)


md5.o:
	$(CC) -c md5.c $(CFLAGS)
	
obscure.o:
	$(CC) -c obscure.c $(CFLAGS)



 
