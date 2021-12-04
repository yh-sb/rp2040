# Raspberry RP2040 submodule of [mcu-framework](https://github.com/yhsb2k/mcu-framework)

[![build](https://github.com/yhsb2k/rp2040/workflows/build/badge.svg)](https://github.com/yhsb2k/rp2040/actions?workflow=build)
[![license](https://img.shields.io/github/license/yhsb2k/rp2040?color=blue)](https://github.com/yhsb2k/rp2040/blob/master/LICENSE)

## How to build
```
git clone --recursive https://github.com/yhsb2k/rp2040.git
cd rp2040
make
```
**Other targets:**
```
make flash - Upload firmware to the target
make erase - Erase all memory on the target
make reset - Reset the target
make debug - Upload firmware to the target and start the debug session
```

## Requirements
* [GNU Arm Embedded Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
* [CMake](https://cmake.org/download)
* [Make](http://gnuwin32.sourceforge.net/packages/make.htm)
