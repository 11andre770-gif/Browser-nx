#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment")
endif

TOPDIR ?= $(CURDIR)
include $(DEVKITPRO)/libnx/switch_rules

TARGET := NXBrowser
BUILD := build
SOURCES := source
DATA := data
INCLUDES := include

NO_ICON := 1
APP_TITLE := NX Browser
APP_AUTHOR := 11andre770-gif
APP_VERSION := 0.1.0

ARCH := -march=armv8-a+crc+crypto -mtune=cortex-a57 -mtp=soft -fPIE
CFLAGS := -g -Wall -O2 -ffunction-sections $(ARCH) $(DEFINES)
CFLAGS += $(INCLUDE) -D__SWITCH__
CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions
ASFLAGS := -g $(ARCH)
LIBS := -lnx

ifneq ($(BUILD),$(notdir $(CURDIR)))

export OUTPUT := $(CURDIR)/$(TARGET)
export TOPDIR := $(CURDIR)
export VPATH := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) $(foreach dir,$(DATA),$(CURDIR)/$(dir))
export DEPSDIR := $(CURDIR)/$(BUILD)

CFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
BINFILES := $(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

export OFILES_BIN := $(addsuffix .o,$(BINFILES))
export OFILES_SRC := $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
export OFILES := $(OFILES_BIN) $(OFILES_SRC)
export INCLUDE := $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) -I$(DEVKITPRO)/libnx/include -I. -iquote $(CURDIR)/include/switch/
export LIBPATHS := $(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: all clean
all: $(BUILD)

$(BUILD):
	@mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

clean:
	rm -rf $(BUILD) $(TARGET).nro $(TARGET).elf $(TARGET).nacp

else

DEPENDS := $(OFILES:.o=.d)
-include $(DEPENDS)

.PHONY: all
all: $(OUTPUT).nro

$(OUTPUT).nro: $(OUTPUT).elf
$(OUTPUT).elf: $(OFILES)

endif
