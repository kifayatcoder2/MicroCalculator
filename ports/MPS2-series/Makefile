# MicroCalculator - Makefile
#
# Usage:
#   make TARGET=an385      # build for QEMU mps2-an385  (Cortex-M3)
#   make TARGET=an386      # build for QEMU mps2-an386  (Cortex-M4)
#   make TARGET=an500      # build for QEMU mps2-an500  (Cortex-M7)
#   make TARGET=an505      # build for QEMU mps2-an505  (Cortex-M33, TrustZone)
#   make TARGET=an385 run  # build then launch under QEMU
#   make allboards          # build all four targets, one after another
#   make clean

TARGET  ?= an385
VALID_TARGETS := an385 an386 an500 an505

ifeq ($(filter $(TARGET),$(VALID_TARGETS)),)
$(error Unknown TARGET '$(TARGET)'. Valid targets: $(VALID_TARGETS))
endif

CROSS   ?= arm-none-eabi-
CC       = $(CROSS)gcc
OBJDUMP  = $(CROSS)objdump
SIZE     = $(CROSS)size

BUILD_DIR = build/$(TARGET)
SRC_DIR   = src
SRCS      = startup.c uart.c calc.c main.c
OBJS      = $(addprefix $(BUILD_DIR)/,$(SRCS:.c=.o))
ELF       = $(BUILD_DIR)/microcalc.elf
MAP       = $(BUILD_DIR)/microcalc.map

# --- Per-target configuration -----------------------------------------
#
# UART0 base addresses and SYSCLK are taken directly from QEMU's board
# models (hw/arm/mps2.c and hw/arm/mps2-tz.c):
#   an385/an386/an500 : CMSDK UART0 @ 0x40004000, SYSCLK = 25 MHz
#   an505             : Secure UART0 alias @ 0x50200000 (NS alias is
#                        0x40200000; AN505 boots Secure with
#                        init-svtor = 0x10000000), APB clock = 20 MHz
#
# BAUDDIV = clock / desired_baud, computed here for 115200 baud.

ifeq ($(TARGET),an385)
  MCPU          = cortex-m3
  QEMU_MACHINE  = mps2-an385
  LDSCRIPT      = linker/an385_an386_an500.ld
  UART0_BASE    = 0x40004000
  BAUDDIV       = 217
  BOARD_NAME    = AN385 (Cortex-M3)
endif
ifeq ($(TARGET),an386)
  MCPU          = cortex-m4
  QEMU_MACHINE  = mps2-an386
  LDSCRIPT      = linker/an385_an386_an500.ld
  UART0_BASE    = 0x40004000
  BAUDDIV       = 217
  BOARD_NAME    = AN386 (Cortex-M4)
endif
ifeq ($(TARGET),an500)
  MCPU          = cortex-m7
  QEMU_MACHINE  = mps2-an500
  LDSCRIPT      = linker/an385_an386_an500.ld
  UART0_BASE    = 0x40004000
  BAUDDIV       = 217
  BOARD_NAME    = AN500 (Cortex-M7)
endif
ifeq ($(TARGET),an505)
  MCPU          = cortex-m33
  QEMU_MACHINE  = mps2-an505
  LDSCRIPT      = linker/an505.ld
  UART0_BASE    = 0x50200000
  BAUDDIV       = 173
  BOARD_NAME    = AN505 (Cortex-M33, Secure)
endif

DEFS = -DUART0_BASE=$(UART0_BASE)UL \
       -DUART_BAUDDIV_VALUE=$(BAUDDIV) \
       -DBOARD_NAME='"$(BOARD_NAME)"'

CFLAGS  = -mcpu=$(MCPU) -mthumb -mfloat-abi=soft \
          -Wall -Wextra -O2 -g -std=c11 \
          -ffreestanding -fno-builtin -fno-common \
          -ffunction-sections -fdata-sections \
          -I$(SRC_DIR) $(DEFS)

LDFLAGS = -mcpu=$(MCPU) -mthumb -mfloat-abi=soft \
          -nostdlib -nostartfiles -T $(LDSCRIPT) \
          -Wl,--gc-sections -Wl,-Map=$(MAP)

.PHONY: all run clean size disasm allboards
all: $(ELF)

allboards:
	@for t in $(VALID_TARGETS); do \
	    echo "==== building $$t ===="; \
	    $(MAKE) TARGET=$$t all || exit 1; \
	done

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(ELF): $(OBJS) $(LDSCRIPT)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

size: $(ELF)
	$(SIZE) $(ELF)

disasm: $(ELF)
	$(OBJDUMP) -d $(ELF) | less

run: $(ELF)
	qemu-system-arm -M $(QEMU_MACHINE) -cpu $(MCPU) -kernel $(ELF) \
	    -nographic -semihosting-config enable=off

clean:
	rm -rf build
