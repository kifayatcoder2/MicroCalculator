# ArduinoMicroCalculator - Makefile
#
# Usage:
#   make TARGET=uno                    # build build/uno/microcalc.hex
#   make TARGET=uno PORT=/dev/ttyACM0 flash
#   make TARGET=nano PORT=/dev/ttyUSB0 flash
#   make TARGET=mega2560 PORT=/dev/ttyACM0 flash
#   make TARGET=atmega1280 PORT=/dev/ttyUSB0 flash
#   make allboards                     # just compile all four, no flashing
#   make clean
#
# PORT is your board's serial port:
#   Linux:   /dev/ttyACM0 or /dev/ttyUSB0  (see `ls /dev/tty*` or dmesg)
#   macOS:   /dev/cu.usbmodemXXXX or /dev/cu.usbserial-XXXX
#   Windows: COM3, COM4, ... (if using WSL/MSYS avrdude, or run from cmd)

TARGET  ?= uno
VALID_TARGETS := uno nano mega2560 atmega1280

ifeq ($(filter $(TARGET),$(VALID_TARGETS)),)
$(error Unknown TARGET '$(TARGET)'. Valid targets: $(VALID_TARGETS))
endif

PORT ?= /dev/ttyACM0
F_CPU = 16000000UL

CC       = avr-gcc
OBJCOPY  = avr-objcopy
AVRSIZE  = avr-size
AVRDUDE  = avrdude

BUILD_DIR = build/$(TARGET)
SRC_DIR   = src
SRCS      = calc.c uart.c main.c
OBJS      = $(addprefix $(BUILD_DIR)/,$(SRCS:.c=.o))
ELF       = $(BUILD_DIR)/microcalc.elf
HEX       = $(BUILD_DIR)/microcalc.hex

# --- Per-target configuration -----------------------------------------
#
# USART0's registers (UBRR0H/L, UCSR0A/B/C, UDR0) are identical on all
# three chips below, so uart.c never changes. What DOES change is:
#   - the AVR part name passed to -mmcu / avrdude -p
#   - the avrdude bootloader "programmer" protocol and upload baud rate,
#     which come straight from Arduino's own boards.txt:
#       uno       -> stk500v1 ("arduino"), 115200
#       nano      -> stk500v1 ("arduino"), 57600  (classic ATmega328 Nano
#                    bootloader; some newer Nanos ship optiboot at
#                    115200 instead -- if 57600 fails to sync, try
#                    `make TARGET=nano AVRDUDE_BAUD=115200 flash`)
#       mega2560  -> stk500v2 ("wiring"), 115200
#       atmega1280-> stk500v1 ("arduino"), 57600 (original Mega bootloader)

ifeq ($(TARGET),uno)
  MCU                 = atmega328p
  AVRDUDE_PROGRAMMER  = arduino
  AVRDUDE_BAUD        = 115200
  BOARD_NAME          = Arduino Uno (ATmega328P)
endif
ifeq ($(TARGET),nano)
  MCU                 = atmega328p
  AVRDUDE_PROGRAMMER  = arduino
  AVRDUDE_BAUD        = 57600
  BOARD_NAME          = Arduino Nano (ATmega328P)
endif
ifeq ($(TARGET),mega2560)
  MCU                 = atmega2560
  AVRDUDE_PROGRAMMER  = wiring
  AVRDUDE_BAUD        = 115200
  BOARD_NAME          = Arduino Mega 2560 (ATmega2560)
endif
ifeq ($(TARGET),atmega1280)
  MCU                 = atmega1280
  AVRDUDE_PROGRAMMER  = arduino
  AVRDUDE_BAUD        = 57600
  BOARD_NAME          = Arduino Mega (ATmega1280)
endif

CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -DBOARD_NAME='"$(BOARD_NAME)"' \
         -Wall -Wextra -Os -std=c11 \
         -ffunction-sections -fdata-sections \
         -I$(SRC_DIR)

LDFLAGS = -mmcu=$(MCU) -Wl,--gc-sections

.PHONY: all flash monitor size clean allboards
all: $(HEX)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(ELF): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex -R .eeprom $(ELF) $(HEX)

size: $(ELF)
	$(AVRSIZE) --mcu=$(MCU) -C $(ELF)

flash: $(HEX)
	$(AVRDUDE) -c $(AVRDUDE_PROGRAMMER) -p $(MCU) -P $(PORT) \
	    -b $(AVRDUDE_BAUD) -D -U flash:w:$(HEX):i

monitor:
	@echo "Opening $(PORT) at 115200 baud (the firmware's runtime Serial speed)."
	@echo "Ctrl-] to quit (Ctrl-A K if using 'screen' instead)."
	python3 -m serial.tools.miniterm $(PORT) 115200

allboards:
	@for t in $(VALID_TARGETS); do \
	    echo "==== building $$t ===="; \
	    $(MAKE) TARGET=$$t all || exit 1; \
	done

clean:
	rm -rf build
