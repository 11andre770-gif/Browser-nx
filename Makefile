# Use the maintained devkitPro/libnx build rules.
# This keeps NACP/NRO generation aligned with current Switch tooling.

TARGET := NXBrowser
BUILD := build
SOURCES := source
INCLUDES := include

APP_TITLE := NX Browser
APP_AUTHOR := 11andre770-gif
APP_VERSION := 0.1.0
NO_ICON := 1

ARCH := -march=armv8-a+crc+crypto -mtune=cortex-a57 -mtp=soft -fPIE
CFLAGS := -g -Wall -O2 -ffunction-sections $(ARCH)
CFLAGS += -I$(CURDIR)/$(INCLUDES) -D__SWITCH__
CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions
ASFLAGS := -g $(ARCH)
LDFLAGS := -specs=$(DEVKITPRO)/libnx/switch.specs -g $(ARCH)
LIBS := -lnx

TOPDIR ?= $(CURDIR)

include $(DEVKITPRO)/libnx/switch_rules
