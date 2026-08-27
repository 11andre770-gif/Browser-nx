TARGET := NXBrowser
BUILD := build
SOURCES := source
INCLUDES := include

ARCH := -march=armv8-a+crc+crypto -mtune=cortex-a57 -mtp=soft -fPIE
CFLAGS := -g -Wall -O2 -ffunction-sections $(ARCH) -I$(INCLUDES)
CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions
LDFLAGS := -specs=$(DEVKITPRO)/libnx/switch.specs -g

CFILES := $(foreach dir,$(SOURCES),$(wildcard $(dir)/*.c))
CPPFILES := $(foreach dir,$(SOURCES),$(wildcard $(dir)/*.cpp))
OBJS := $(CFILES:.c=.o) $(CPPFILES:.cpp=.o)

all: $(TARGET).nro

$(TARGET).elf: $(OBJS)
	@echo " LINK  $@"
	$(CXX) $(LDFLAGS) $(OBJS) -lnx -o $@

%.o: %.c
	@echo " CC    $<"
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	@echo " CXX   $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET).nro: $(TARGET).elf
	@echo " NRO   $@"
	elf2nro $< $@ --icon=icon.jpg --nacp=app.nacp

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).nro

.PHONY: all clean
