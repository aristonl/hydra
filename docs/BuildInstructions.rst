Building Inferno
================
This document will explain how to build Inferno and prerequisites needed.

Prerequisites
-------------
    Make sure you have all the prerequisites needed to build.

    - Ubuntu / Debian:
    ::
        $ sudo apt install mtools xorriso qemu qemu-system ovmf build-essential nasm bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo cmake ninja-build

    - Arch Linux / Manjaro:
    ::
        $ sudo pacman -S --needed base-devel mtools libisoburn qemu qemu-arch-extra edk2-ovmf nasm gmp libmpc mpfr cmake ninja

    - macOS:
    ::
        $ brew install x86_64-elf-gcc ninja cmake qemu mtools xorriso nasm

*NOTE*: Make sure you export x86_64-elf-gcc/x86_64-elf-g++ as your CC and CXX so CMake doesn't configure using clang.

Downloading
-----------
 - You can download the latest release tarball and unpack it:
   ::
        $ xz -cd inferno-0.1.x.tar.xz | tar xvf -
   
   Replace the "X" with the version number of the release.

 - You can also upgrade between releases by patching. To install
   by patching, download all the patch files for each version
   since your current version, then you can enter the top-level 
   directory of the Hydra/Inferno source and execute:
   ::
        $ patch -p1
   
 - You can also clone the repository although we'd highly
   recommend not using the ``main`` branch but any of the release
   branches.

Compiling
---------
Once you are configured, you can build Inferno with:
::
   $ cmake -GNinja .
   $ ninja

This will build the ISO and save it into the ``$SRC_ROOT/Build`` folder.

## Emulating
You can use the ISO in other virtualization software such as VirtualBox or
VMWare or use QEMU to emulate it. You can either write up the command
yourself or use the default QEMU command that comes with CMakeLists.txt
which can be ran with ``ninja emulate`` or ``ninja emulate-no-display`.
