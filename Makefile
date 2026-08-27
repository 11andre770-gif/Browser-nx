#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>/devkitpro")
endif

TOPDIR ?= $(CURDIR)

# libnx's maintained Switch build rules must be included before project variables.
include $(DEVKITPRO)/libnx/switch_rules

#---------------------------------------------------------------------------------
# Project
#---------------------------------------------------------------------------------
TARGET := NXBrowser
BUILD := build
SOURCES := source
DATA := data
INCLUDES := include

NO_ICON := 1
APP_TITLE := NX Browser
APP_AUTHOR := 11andre770-gif
APP_VERSION := 0.1.0

#---------------------------------------------------------------------------------
# Compiler options
#---------------------------------------------------------------------------------
ARCH := -march=armv8-a+crc+crypto -mtune=cortex-a57 -mtp=soft -fPIE

CFLAGS := -g -Wall -O2 -ffunction-sections \
          $(ARCH) $(DEFINES)
CFLAGS += $(INCLUDE) -D__SWITCH__

CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions
ASFLAGS := -g $(ARCH)
LDFLAGS := -specs=$(DEVKITPRO)/libnx/switch.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)
LIBS := -lnx

#---------------------------------------------------------------------------------
