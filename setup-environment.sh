#!/bin/bash
WhereAmI="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

case "$OSTYPE" in
  darwin*)  OS=0 ;; 
  linux*)   OS=1 ;;
  *)        OS=2 ;;
esac

if [ $OS -eq 2 ]; then
    echo "Please use Windows, macOS, Arch, or Ubuntu!"
    return
fi
if [ $OS -eq 0 ]; then
    which -s brew
    if [[ $? != 0 ]] ; then
        echo "Installing Homebrew..."
        ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)" > /dev/null 2>&1
    else
        echo "Updating Homebrew..."
        brew update > /dev/null 2>&1
    fi
    which -s docker
    if [[ $? != 0 ]] ; then
        echo "Installing Docker..."
        brew install docker > /dev/null 2>&1
    fi
    if docker images | grep -q 'hbm'; then
        echo "HBM image already exists skipping for now..."
    else
        echo "Pulling HBM from Docker..."
        docker pull firelscar/hbm:latest > /dev/null 2>&1
    fi
    if docker ps -a | grep -q 'hbm'; then
        echo "HBM container already exists skipping for now..."
    else
        echo "Creating HBM container..."
        docker run -d -v $WhereAmI:/home/HydraOS/ --name HBM firelscar/hbm:latest tail -f /dev/null > /dev/null 2>&1
    fi
    echo "Checking for macOS dependencies..."
    which -s mtools
    if [[ $? != 0 ]] ; then
        echo "Installing mtools..."
        brew install mtools > /dev/null 2>&1
    fi
    which -s qemu-system-x86_64
    if [[ $? != 0 ]] ; then
        echo "Installing qemu..."
        brew install qemu > /dev/null 2>&1
    fi
    echo "Done!"
    echo "Build and run with the 'make' command"
    echo "Make a USB with 'make usb'"
    return
fi
if [ "$(uname -s)" = "Linux" ]; then
	KERNEL=$(uname -r)
	if [ -f /etc/debian_version ]; then	
		if [ "$(awk -F= '/DISTRIB_ID/ {print $2}' /etc/lsb-release)" = "Ubuntu" ]; then
			DIST="Ubuntu"
		else
			DIST="Debian"
		fi
	elif [ -f /etc/arch-release ]; then
		DIST="Arch"
	fi
  echo ${DIST}
  return
fi