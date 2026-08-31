# MicroCalculator
this is Arduino port for Uno,NANO,mega(2560),mega(1280) boards


| Target       | Chip        | avrdude programmer | Upload baud |
|--------------|-------------|---------------------|-------------|
| `uno`        | ATmega328P  | `arduino` (stk500v1)| 115200      |
| `nano`       | ATmega328P  | `arduino` (stk500v1)| 57600 *     |
| `mega2560`   | ATmega2560  | `wiring` (stk500v2) | 115200      |
| `atmega1280` | ATmega1280  | `arduino` (stk500v1)| 57600       |

\* Classic Nanos ship with an older bootloader at 57600 baud; some
newer Nanos ship the Uno-style optiboot at 115200 instead. If
`57600` fails to sync, try `make TARGET=nano AVRDUDE_BAUD=115200 flash`.

## Prerequisites

- `avr-gcc` / `avr-libc` / `avr-objcopy` / `avr-size` (the AVR GCC toolchain)
- `avrdude`

On Debian/Ubuntu:
```
sudo apt install gcc-avr avr-libc avrdude
```
On macOS (Homebrew):
```
brew install avr-gcc avrdude
```

## Build & flash

Find your board's serial port first (`ls /dev/tty*` on Linux/macOS
before and after plugging in the board, or Device Manager on Windows),
then:

```
make TARGET=uno        PORT=/dev/ttyACM0 flash
make TARGET=nano       PORT=/dev/ttyUSB0 flash
make TARGET=mega2560   PORT=/dev/ttyACM0 flash
make TARGET=atmega1280 PORT=/dev/ttyUSB0 flash
```

Then open a serial terminal at **115200 baud** — the Arduino IDE's
Serial Monitor works fine, or:

```
make TARGET=uno PORT=/dev/ttyACM0 monitor
```

To build without flashing: `make TARGET=uno`. To compile all four
without touching hardware: `make allboards`.

After you compiled it it will automatically will be flashed to board
