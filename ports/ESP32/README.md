# MicroCalculator

This is ESP32 port for ESP32-P4,ESP32-S3,ESP32(original)

## Why this one has no per-chip driver code at all

The three chips don't share a console peripheral the way the AVR
boards all shared USART0:

| Chip      | CPU                          | Default console                          |
|-----------|-------------------------------|-------------------------------------------|
| ESP32     | Dual-core Xtensa LX6          | UART0, via an external USB-UART bridge chip on the dev board |
| ESP32-S3  | Dual-core Xtensa LX7          | Built-in native USB Serial/JTAG controller |
| ESP32-P4  | Dual-core RISC-V              | Built-in native USB Serial/JTAG controller |

The P4 isn't even the same instruction set architecture as the other
two. Hand-writing register-level UART/USB drivers for three
peripherals across two architectures would be a lot of fragile code
for no real benefit — this is exactly the problem ESP-IDF's
console/VFS layer already solves. `idf.py set-target <chip>` picks the
right peripheral and wires it to `stdin`/`stdout`, so `main.c` never
mentions UART or USB and is identical for all three targets. This is
the standard, idiomatic way to write portable ESP-IDF firmware —
analogous to how avr-libc's runtime handled AVR startup, or how the
CMSDK UART driver was shared across the QEMU boards.

## Prerequisites

Install ESP-IDF (v5.x recommended; v5.2+ is needed for ESP32-P4
support) by following Espressif's setup guide:
<https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/index.html>
Then, in every new shell:
```
. $HOME/esp/esp-idf/export.sh     # or wherever you installed it
```
## Build & flash
```
idf.py set-target esp32          # or esp32s3 / esp32p4
idf.py build
idf.py -p PORT flash monitor
```
Replace `PORT` with your board's serial port (e.g. `/dev/ttyUSB0`,
`/dev/ttyACM0`, or `COM3`). `idf.py set-target` regenerates `sdkconfig`
for the chosen chip, so switching targets is just re-running that
command with a different value — no source changes needed.

To leave the monitor: `Ctrl-]`.
