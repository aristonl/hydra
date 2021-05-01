# Hydra Build Instructions
## Prerequisites
### Linux
You will need to install G++, GCC, make, NASM, mtools, and QEMU to install Hydra.

#### Debian/Ubuntu
`sudo apt install build-essential nasm mtools qemu-system` 

#### Arch Linux
`setup-environment.sh` has no current support for Arch at the moment. 
```bash
sudo pacman -Sy base-devel nasm mtools qemu
```

### macOS
Before getting the packages listed above, you must have Xcode Tools to install `brew` which is needed to install the packages.
Once you have Xcode Tools, you can install Homebrew.
```bash
brew install docker mtools qemu
```

### Windows
Hydra can be built and ran under WSL Version 2 *only*.

WSL Version 2 requires Windows version 2004 or higher, with OS Build 19041 or greater. Here is a guide on how to [get WSL2](https://docs.microsoft.com/en-us/windows/wsl/install-win10).
Once installed, you will need to make sure the distribution you want to use (and the new default) is using Version 2:
- `wsl -l -v` will list distros and versions,<br/>
- `wsl --set-version <distro> <version>` is used to convert a distro to another version, and<br/>
- `wsl --set-default-version 2` will set the default version for all new distros (if desired.)<br/>

The installation then proceeds as usual.

WSL2 does not natively support graphical applications.
You can either install QEMU natively on windows and allow WSL to talk to it, or you can install an X Server for windows.

#### Setting up an X server with WSL:

- Install [Vcxsrv](https://sourceforge.net/projects/vcxsrv/) on Windows.
- When you start up Vcxsrv, make sure to set the Display number to 0, and to Disable access control.
- Before actually doing **make**, you need to set the DISPLAY environmental variable as such:

```bash
export DISPLAY=$(cat /etc/resolv.conf | grep nameserver | awk '{print $2}'):0
```
This is due to a bug in WSL2. For more information, microsoft/WSL#4106.
- Connect to the window server from WSL.

Now you can finally, **make**.

## Build
To build Hydra, make sure you run `./setup-environment.sh` as it will create all the necessary files and config's needed to build. After, you can finally run `make`.
