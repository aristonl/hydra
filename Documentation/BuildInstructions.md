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
ninja Emulate
```
A QEMU window will open and will virtualize Hydra. You can also find the in the `$HYDRA_REPO_DIRECTORY/Build` directory .iso files if needed.
## More Information
If you want to build just an ISO file for use on a virtual machine software (i.e. VirtualBox), you can run:
```bash
ninja Inferno image iso
```
