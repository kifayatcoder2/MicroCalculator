# MicroCalculator
This is the MPS2 Port for MicroCalculator Project and this port is designed for ARM MPS2 series which is used on QEMU
# Supported boards :
1. ARM MPS2 with AN385 FPGA image for Cortex M3
2. ARM MPS2 with AN386 FPGA image for Cortex M4
3. ARM MPS2 with AN500 FPGA image for Cortex M7
4. ARM MPS2 with AN505 FPGA image for Cortex M33
# How to Compile and Start in QEMU :
1. Install Dependents for compiling
On Debian/Ubuntu:
```
sudo apt install gcc-arm-none-eabi qemu-system-arm
```
On macOS (Homebrew):
```
brew install --cask gcc-arm-embedded
brew install qemu
```
# 2. Select your board to use :

1. ARM MPS2 with AN385 FPGA image for Cortex M3
```
make TARGET=an385
```

2. ARM MPS2 with AN386 FPGA image for Cortex M4
```
make TARGET=an386
```

3. ARM MPS2 with AN500 FPGA image for Cortex M7
```
make TARGET=an500
```

4. ARM MPS2 with AN505 FPGA image for Cortex M33
```
make TARGET=an505
```
5. All Boards:
```
make allboards
```
# 3. Compile it:
Pick your boards available in Makefile
then run example I want for ARM MPS2 with AN385 FPGA image for Cortex M3 use this command:
```
make TARGET=an385
```
after compiling completed you will find elf file in build/board type you selected/microcalc.elf and you can start it in QEMU by:

```
qemu-system-arm -M mps2-selected-board \
    -kernel build/board type you selected/microcalc.elf -nographic -semihosting-config enable=off
```
