# Building Hydra
This document will explain how to build Hydra and prerequisites needed.

## Prerequisites
Make sure you have all the prerequisites needed to build.
### Ubuntu
`sudo apt install mtools xorriso qemu qemu-system mingw-w64 ovmf build-essential nasm bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo`
### Building
First run `make setup`. It will throw a `build.config: no such file or directory` error but you can ignore this. After it finishes running, you can run `make`.
