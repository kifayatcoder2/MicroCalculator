cmake_minimum_required(VERSION 3.13)

# Locate the Pico SDK via PICO_SDK_PATH (env var, or -DPICO_SDK_PATH=...
# on the cmake command line), then include its init script directly.
# This is the SDK's own documented alternative to copying
# pico_sdk_import.cmake into every project -- see "Or by cloning the
# SDK locally, but without copying pico_sdk_import.cmake" in the
# pico-sdk README.
if (DEFINED ENV{PICO_SDK_PATH} AND (NOT PICO_SDK_PATH))
    set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})
endif ()

if (NOT PICO_SDK_PATH)
    message(FATAL_ERROR
        "PICO_SDK_PATH is not set. Point it at your pico-sdk checkout, e.g.\n"
        "  export PICO_SDK_PATH=/path/to/pico-sdk\n"
        "or pass -DPICO_SDK_PATH=/path/to/pico-sdk to cmake.")
endif ()

include(${PICO_SDK_PATH}/pico_sdk_init.cmake)

project(microcalculator C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Default board is the original Raspberry Pi Pico (RP2040); override
# with -DPICO_BOARD=pico2 etc. if you want to try this on other
# boards in the pico-sdk board list.
pico_sdk_init()

add_executable(microcalculator
    main.c
    calc.c
)

target_link_libraries(microcalculator pico_stdlib)

# Make the REPL available over BOTH the micro-USB port (as a USB CDC
# serial device) and the UART0 pins (GPIO0/GPIO1), so it works whether
# you're plugged in over USB or wired up to an external UART adapter.
pico_enable_stdio_usb(microcalculator 1)
pico_enable_stdio_uart(microcalculator 1)

# Also produce a .uf2 file (in addition to .elf) for drag-and-drop
# flashing via the Pico's BOOTSEL mass-storage mode.
pico_add_extra_outputs(microcalculator)
