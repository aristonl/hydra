# Building Hydra
This document will explain how to build Hydra and prerequisites needed.

## Prerequisites
Make sure you have all the prerequisites needed to build.
### Ubuntu / Debian
```bash
sudo apt install mtools xorriso qemu qemu-system mingw-w64 ovmf build-essential nasm bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo cmake ninja-build
```
### Arch Linux / Manjaro
```bash
sudo pacman -S --needed base-devel mtools libisoburn qemu qemu-arch-extra mingw-w64-gcc mingw-w64-binutils edk2-ovmf nasm gmp libmpc mpfr cmake ninja
```
## Building
Following this, you can `git pull` as needed. You won't usually need to reconfigure your build after it.
<br>
Once your ready to run, you can build and run Hydra with:
```bash
cmake -GNinja .
ninja
```
This will build the ISO and save it into the `$HYDRA_ROOT/Build` folder.

## Emulating
You can use the ISO in other virtualization software such as VirtualBox or
VMWare or use QEMU to emulate it. You can either write up the command
yourself or use the default QEMU command that comes with CMakeLists.txt
which can be ran with `ninja Emulate`.
