#!/bin/bash
WhereAmI="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

case "$OSTYPE" in
  darwin*)  OS=0 ;;
  linux*)   OS=1 ;;
  *)        OS=2 ;;
esac

if [ $OS -eq 2 ]; then
    echo "Please use Windows, macOS, Arch, or Ubuntu!"
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
    echo -e "OS = macOS\n$(cat Makefile)" > Makefile
    echo "Done!"
    echo "Build and run with the 'make' command"
    exit
fi
if [ "$(uname -s)" = "Linux" ]; then
	KERNEL=$(uname -r)
	if [ -f /etc/debian_version ]; then
		if [ "$(awk -F= '/DISTRIB_ID/ {print $2}' /etc/lsb-release > /dev/null 2>&1)" = "Ubuntu" ]; then
			DIST="Ubuntu"
		else
			DIST="Debian"
		fi
	elif [ -f /etc/arch-release ]; then
		DIST="Arch"
        echo "Arch Linux is currently not available for building Hydra."
	fi
fi
if [ $DIST == "Ubuntu" ] || [ $DIST == "Debian" ]; then
	echo "Checking for $DIST dependencies..."
	which -a sudo > /dev/null 2>&1
    if [[ $? != 0 ]] ; then
        echo "Installing sudo..."
        apt update
        apt install sudo -y > /dev/null 2>&1
    fi
	sudo apt update > /dev/null 2>&1
	which -a make > /dev/null 2>&1
    if [[ $? != 0 ]] ; then
        echo "Installing make..."
        sudo apt install make -y > /dev/null 2>&1
    fi
	which -a gcc > /dev/null 2>&1
    if [[ $? != 0 ]] ; then
            echo "Installing build-essential..."
            sudo apt install build-essential -y > /dev/null 2>&1
    fi
	which -a nasm > /dev/null 2>&1
    if [[ $? != 0 ]] ; then
            echo "Installing nasm..."
            sudo apt install nasm -y > /dev/null 2>&1
    fi
	which -a mtools > /dev/null 2>&1
    if [[ $? != 0 ]] ; then
            echo "Installing mtools..."
            sudo apt install mtools -y > /dev/null 2>&1
    fi
	which -a qemu-system-x86_64 > /dev/null 2>&1
    if [[ $? != 0 ]] ; then
            echo "Installing qemu-system..."
            sudo apt install qemu-system -y > /dev/null 2>&1
    fi
	echo -e "OS = $DIST\n$(cat Makefile)" > Makefile
	echo "Done!"
    echo "Build and run with the 'make' command"
    exit
fi
