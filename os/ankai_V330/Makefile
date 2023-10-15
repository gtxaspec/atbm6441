###############################################################################
# 	* Project name atbm6031 SDIO WiFi
#  * @author charlie
#  * @date	2019-12-03
###############################################################################

PROJ = libdrv_wifi_atbm6031
TARGET = $(PROJ).a

TOPDIR 	?= $(shell pwd)/../../../../V33X_SDK
PROJ_TOP_DIR ?= $(TOPDIR)
LWIP_VER := 2.1.0

###############################################################################
#  * Configuration
###############################################################################

include $(TOPDIR)/config.mk

LIBDIR = $(TOPDIR)/os/driver

KERNELDIR = $(KERNEL_DIR)
bsp_dir = $(KERNELDIR)/bsp/anyka
driver_dir = $(shell pwd)/../../../../V33X_SDK

MK_DIR = $(shell pwd)/../..
###############################################################################
#  * include .h
###############################################################################

INCLUDE    =	-I. \
		-I$(MK_DIR) \
		-I$(MK_DIR)/hal_apollo \
		-I$(TOPDIR)/include_inner\
        -I$(driver_dir)/include \
		-I$(bsp_dir)/include\
		-I$(bsp_dir) \
		-I$(KERNELDIR)/include \
		-I$(KERNELDIR)/components/finsh \
		-I$(KERNELDIR)/components/drivers/wlan \
		-I$(KERNELDIR)/components/net/netdev/include \
		-I$(KERNELDIR)/components/net/lwip-$(LWIP_VER)/src/arch/include \
		-I$(KERNELDIR)/components/net/lwip-$(LWIP_VER)/src/include \
		-I$(KERNELDIR)/components/net/lwip-$(LWIP_VER)/src/include/lwip \
		-I$(KERNELDIR)/components/net/lwip-$(LWIP_VER)/src \
		-I$(KERNELDIR)/components/net/lwip-$(LWIP_VER)/src/arch


INCLUDE +=$(KERNEL_INCLUDE)
BUILDPATH  ?= build

###############################################################################
#  * include .c
###############################################################################
SRCDIR := $(MK_DIR)/hal_apollo
OSDIR  := $(MK_DIR)/os/ankai_V330
DEFINE += -DRT_USING_NEWLIB
DEFINE += -DHAVE_CCONFIG_H
DEFINE += -D_POSIX_REALTIME_SIGNALS
DEFINE += -DAK_RTOS

CSRCS  := \
		$(OSDIR)/atbm_os_atomic.c \
		$(OSDIR)/atbm_os_mem.c \
		$(OSDIR)/atbm_os_misc.c \
		$(OSDIR)/atbm_os_mutex.c \
		$(OSDIR)/atbm_os_proto.c \
		$(OSDIR)/atbm_os_queue.c \
		$(OSDIR)/atbm_os_sdio.c \
		$(OSDIR)/atbm_os_skbuf.c \
		$(OSDIR)/atbm_os_thread.c \
		$(OSDIR)/atbm_os_timer.c \
		$(OSDIR)/atbm_os_api.c \
		$(OSDIR)/ankai_cli.c

CSRCS  += \
		$(SRCDIR)/fwio.c \
		$(SRCDIR)/wsm.c \
		$(SRCDIR)/main.c \
		$(SRCDIR)/iface.c \
		$(SRCDIR)/txrx.c  \
		$(SRCDIR)/agg-rx.c \
		$(SRCDIR)/rx.c	\
		$(SRCDIR)/sta_info.c \
		$(SRCDIR)/util.c \
		$(SRCDIR)/apollo_sdio.c \
		$(SRCDIR)/atbm_platform.c \
		$(SRCDIR)/hwio_sdio.c \
		$(SRCDIR)/bh_sdio.c \
		$(SRCDIR)/dcxo_dpll.c \
		$(SRCDIR)/atbm_ioctl.c

COBJS += $(patsubst %.c, $(BUILDPATH)/%.o, $(CSRCS))

DEFINE += -DATBM_WIFI_PLATFORM=20

DEFINE += -DCC_HAVE_ASM_GOTO -DCONFIG_IF1NAME=\"wlan%d\" -DCONFIG_IF2NAME=\"p2p%d\" 
DEFINE += -DATBM_WIFI_QUEUE_LOCK_BUG -DATBM_CHANGE_AP_TIMESTAMP_TO_BOOTTIME -DATBM_USE_SAVED_FW 
DEFINE += -DATBM_APOLLO_SUSPEND_RESUME_FILTER_ENABLE -DCONFIG_ATBM_IOCTRL -DOPER_CLOCK_USE_SEM -DEXIT_MODULE_RESET_USB=0 
DEFINE += -DATBM_VIF_LIST_USE_RCU_LOCK -DATBM_SUPPORT_SMARTCONFIG -DWIFI_FW_DOWNLOAD -DCONFIG_ATBM_APOLLO_DEBUG -DCONFIG_TX_NO_CONFIRM 
DEFINE += -DCONFIG_ATBM_APOLLO_TESTMODE -DCONFIG_ATBMWIFI_WDT -DCONFIG_TXPOWER_DCXO_VALUE=\""/atbm_txpwer_dcxo_cfg.txt"\" 
DEFINE += -DPROJ_TYPE=HERA -DDPLL_CLOCK=DPLL_CLOCK_24M -DCONFIG_ATBM_APOLLO_SUPPORT_SGI -DSDIO_BUS -DPS_SETUP 
DEFINE += -DCONFIG_MAC80211_ATBM_RC_DEFAULT_MINSTREL -DPROBE_RESP_EXTRA_IE -DCONFIG_MAC80211_ATBM_ROAMING_CHANGES

###############################################################################
#  * build rule
###############################################################################

.PHONY: all prepare target clean install 

all: prepare target install

prepare:
ifneq ($(BUILDPATH),)
	@for i in $(SRCDIR); \
	do mkdir -p $(BUILDPATH)/$$i; \
	done
endif

target: $(BUILDPATH)/$(TARGET)
$(BUILDPATH)/$(TARGET): $(COBJS) 
	@echo ---------------------[build out]----------------------------------	
	$(AR) -rsv $@  $(COBJS) 
	
install: 
	@if [ ! -e $(BUILDPATH)/$(TARGET) ];then\
		echo "\033[1;31m"$(TARGET) is not exist"\033[m";\
		/bin/false;	\
	fi
	@echo "\033[1;32m"install $(BUILDPATH)/$(TARGET) to $(LIBDIR)"\033[m"
	cp $(BUILDPATH)/$(TARGET) $(LIBDIR)

clean: 
	rm -rf $(BUILDPATH)
	rm $(LIBDIR)/$(TARGET)

###############################################################################
#  * Rules
###############################################################################

include $(shell pwd)/rules.mk



