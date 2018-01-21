#
# Makefile for compiling an Arduino INO for a Digispark device
#

DIGISTUMP_DIR = ~/git/arduino/DigistumpArduino/digistump-avr

VARIANT_DIR = $(DIGISTUMP_DIR)/variants/digispark
CORE_DIR = $(DIGISTUMP_DIR)/cores/tiny
LIB_DIR = $(DIGISTUMP_DIR)/libraries/DigisparkCDC

DEVICE = attiny85
CLOCK = 16000000

DEFINES = -DF_CPU=$(CLOCK)L -DARDUINO=10805 -DARDUINO_AVR_DIGISPARK \
	  -DARDUINO_ARCH_AVR

INCLUDES = -I $(VARIANT_DIR) -I $(CORE_DIR) -I $(LIB_DIR)

CFLAGS = -Wall -Os -w -fno-exceptions -ffunction-sections -fdata-sections \
	 -MMD -mmcu=$(DEVICE) $(DEFINES) $(INCLUDES)
CXXFLAGS = $(CFLAGS)
SFLAGS = -x assembler-with-cpp -mmcu=$(DEVICE) $(DEFINES) $(INCLUDES)
LDFLAGS = -Os -Wl,--gc-sections -mmcu=$(DEVICE)

CC = avr-gcc
CXX = avr-g++

INO = $(wildcard *.ino)

VARIANT_SRCS = \
	$(VARIANT_DIR)/pins_arduino.c

CORE_SRCS := \
	$(CORE_DIR)/main.cpp \
	$(CORE_DIR)/wiring.c \
	$(CORE_DIR)/Print.cpp

LIB_SRCS := \
	$(LIB_DIR)/DigiCDC.cpp \
	$(LIB_DIR)/usbdrv.c \
	$(LIB_DIR)/osccal.c \
	$(LIB_DIR)/usbdrvasm.S

# -----------------------------------------------------------------------------

INO_OBJ = $(INO:.ino=.o)
INO_ELF = $(INO:.ino=.elf)
INO_EEP = $(INO:.ino=.eep)
INO_HEX = $(INO:.ino=.hex)

VARIANT_OBJS := $(VARIANT_SRCS:.c=.o)
VARIANT_OBJS := $(notdir $(VARIANT_OBJS))

CORE_OBJS := $(CORE_SRCS:.c=.o)
CORE_OBJS := $(CORE_OBJS:.cpp=.o)
CORE_OBJS := $(notdir $(CORE_OBJS))

LIB_OBJS := $(LIB_SRCS:.c=.o)
LIB_OBJS := $(LIB_OBJS:.cpp=.o)
LIB_OBJS := $(LIB_OBJS:.S=.o)
LIB_OBJS := $(notdir $(LIB_OBJS))

ALL_OBJS = $(INO_OBJ) $(VARIANT_OBJS) $(CORE_OBJS) $(LIB_OBJS)

all: $(INO_EEP) $(INO_HEX)

%.cpp: %.ino
	cp $< $@

%.o: $(VARIANT_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: $(CORE_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: $(CORE_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o: $(LIB_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o: $(LIB_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: $(LIB_DIR)/%.S
	$(CC) $(SFLAGS) -c -o $@ $<

%.elf: $(ALL_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(ALL_OBJS)

%.eep: %.elf
	avr-objcopy -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load \
	    --no-change-warnings --change-section-lma .eeprom= $< $@

%.hex: %.elf
	avr-objcopy -O ihex -R .eeprom $< $@
	../fw-size $<

flash: $(INO_HEX)
	../micronucleus --run $<

clean:
	rm -f *.cpp *.d *.o *.elf *.eep *.hex

.PHONY: all flash clean

# Prevent make from removing intermediary files
.SECONDARY:
