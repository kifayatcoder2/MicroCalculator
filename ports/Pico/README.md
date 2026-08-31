# MicroCalculator

this is Pico port for Raspberry Pi Pico board
## Design notes

- Like the ESP32 port, this uses the vendor SDK's `stdio` layer
  (`pico/stdlib.h`) instead of touching UART/USB registers directly.
  `CMakeLists.txt` enables **both** backends at once
  (`pico_enable_stdio_usb` and `pico_enable_stdio_uart`), so the REPL
  works whether you connect over the Pico's micro-USB port (as a USB
  CDC serial device) or wire up an external UART adapter to GPIO0
  (TX) / GPIO1 (RX) — no source change needed either way.
- `calc.c`/`calc.h` are the identical files used in the other three
  MicroCalculator projects.
- This project intentionally skips copying `pico_sdk_import.cmake`
  (the small helper script every `pico-examples` project uses) into
  the repo. Instead it includes `pico_sdk_init.cmake` from
  `PICO_SDK_PATH` directly — a pattern documented in the Pico SDK's
  own README as an alternative to the copied-file approach. Same
  result, one less file to keep in sync with your SDK version.

## Prerequisites

- The Pico SDK (`pico-sdk`), plus the `arm-none-eabi-gcc` toolchain
  and CMake. Follow Raspberry Pi's official "Getting Started" guide:
  <https://www.raspberrypi.com/documentation/microcontrollers/c_sdk.html>
- Set `PICO_SDK_PATH` to point at your `pico-sdk` checkout:
  ```
  export PICO_SDK_PATH=/path/to/pico-sdk
  ```
# Building:
```
mkdir build && cd build
cmake ..
make -j4
```
This produces `build/microcalculator.uf2` (among other output files).

# Flashing:
1. Hold the **BOOTSEL** button on the Pico while plugging it into USB
   (or while pressing RESET if wired to one).
2. It appears as a USB mass-storage drive named `RPI-RP2`.
3. Copy `microcalculator.uf2` onto that drive. The Pico reboots
   automatically and runs the firmware.

## Connect a serial terminal

- **Over USB**: the Pico enumerates as a USB CDC serial port
  (`/dev/ttyACM0` on Linux, `/dev/cu.usbmodem*` on macOS, a `COM*` port
  on Windows). Baud rate doesn't matter for USB CDC.
- **Over UART0**: wire an external USB-UART adapter's RX to the
  Pico's GPIO0 (pin 1, TX) and TX to GPIO1 (pin 2, RX), plus a shared
  ground. Default baud rate is 115200.

Either way:
```
minicom -D /dev/ttyACM0        # or: screen /dev/ttyACM0
```
