# Building Hydra
This document will explain how to build Hydra and prerequisites needed.

## Prerequisites
Make sure you have all the prerequisites needed to build.
### Ubuntu / Debian
```bash
sudo apt install mtools xorriso qemu qemu-system ovmf build-essential nasm bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo
```
### Arch Linux / Manjaro
```bash
sudo pacman -S --needed base-devel mtools libisoburn qemu qemu-arch-extra edk2-ovmf nasm gmp libmpc mpfr
```
### macOS
```bash
brew install vagrant
brew install --cask virtualbox
vagrant up
```
If you get an error while building that says Connection to 127.0.0.1 closed. You can ignore it.
## Building
In order to build Echo, you will need to first configure your build by running the following command:
```bash
make setup
```
It may ask you to enter text so input as needed (ex: Docker Container Name for macOS).
Following this, you can `git pull` as needed. You won't usually need to reconfigure your build after it.
<br>
Once your ready to run, you can build and run Echo with:
```bash
make
```
A QEMU window will open and will virtualize Echo. You can also find the in the `$ECHO_REPO_DIRECTORY/Build` directory .iso files if needed.
## More Information
If you want to build just an ISO file for use on a virtual machine software (i.e. VirtualBox), you can run:
```bash
make BOB Inferno image iso
```
Note for Linux users: If you allocate <2G of memory, the bootloader cannot load into the kernel. Please allocate >2G of memory to QEMU for it to work properly.