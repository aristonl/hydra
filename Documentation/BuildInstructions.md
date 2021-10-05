# Building Hydra
This document will explain how to build Hydra and prerequisites needed.

## Ubuntu
### Prerequisites
`sudo apt install mtools xorriso qemu qemu-system mingw-w64 ovmf build-essential`
### Building
First run `make setup`. It will throw a `build.config: no such file or directory` error but you can ignore this. After it finishes running, you can run `make`. It will open a QEMU window running BOB (Better Opensource Bootloader).