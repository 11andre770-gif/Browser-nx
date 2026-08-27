#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>/devkitpro")
endif

TOPDIR ?= $(CURDIR)
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
# Code generation
#---------------------------------------------------------------------------------
ARCH := -march=armv8-a+crc+crypto -fPIE

CFLAGS := -g -Wall -O2 -ffunction-sections \
           $(ARCH) $(DEFINES)
CFLAGS += $(INCLUDE) -D__SWITCH__

CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions
ASFLAGS := -g $(ARCH)
LDFLAGS := -specs=$(DEVKITPRO)/libnx/switch.specs -g -Wl,-Map,$(notdir $*.map)
LIBS := -lnx

#---------------------------------------------------------------------------------
# Build rules
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
export OUTPUT := $(CURDIR)/$(TARGET)
export TOPDIR := $(CURDIR)
export VPATH := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) $(foreach dir,$(DATA),$(CURDIR)/$(dir))
export DEPSDIR := $(CURDIR)/$(BUILD)

CFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
BINFILES := $(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

export OFILES_SRC := $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
export OFILES_BIN := $(addsuffix .o,$(BINFILES))
export OFILES := $(OFILES_BIN) $(OFILES_SRC)

export INCLUDE := $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) $(foreach dir,$(LIBDIRS),-I$(dir)/include) -I. -iquote $(CURDIR)/include

.PHONY: all clean

all: $(OUTPUT).nro

else

DEPENDS := $(OFILES:.o=.d)

$(OUTPUT).nro: $(OUTPUT).elf
	@$(ELF2NRO) $< $@ $(NROFLAGS)

$(OUTPUT).elf: $(OFILES)
	@echo " LINK  $@"
	@$(CXX) $(LDFLAGS) $(OFILES) $(LIBS) -o $@

$(OFILES_SRC): $(HFILES)
-include $(DEPENDS)

endif
