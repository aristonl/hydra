WhereAmI="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

case "$OSTYPE" in
  solaris*) OS=0 ;;
  darwin*)  OS=1 ;; 
  linux*)   OS=2 ;;
  bsd*)     OS=3 ;;
  msys*)    OS=4 ;;
  *)        OS=5 ;;
esac

if [ $OS -ne 1 ]; then
    if [ $OS -eq 0 ]; then
        echo "Solaris is currently not supported!"
    fi
    if [ $OS -eq 2 ]; then
        echo "Linux is currently not supported!"
    fi
    if [ $OS -eq 3 ]; then
        echo "BSD is currently not supported!"
    fi
    if [ $OS -eq 4 ]; then
        echo "Windows is currently not supported!"
    fi
    if [ $OS -eq 5 ]; then
        echo "Your OS is currently not supported!"
    fi
fi
if [ $OS -eq 1 ]; then
    which -s brew
    if [[ $? != 0 ]] ; then
        echo "Installing Homebrew..."
        ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    else
        echo "Updating Homebrew..."
        brew update
    fi
    which -s docker
    if [[ $? != 0 ]] ; then
        echo "Installing Docker..."
        brew install docker
    fi
    if docker images | grep -q 'hbm'; then
        echo "HBM image already exists skipping for now..."
    else
        echo "Pulling HBM from Docker..."
        docker pull firelscar/hbm:latest
    fi
    if docker ps -a | grep -q 'hbm'; then
        echo "HBM container already exists skipping for now..."
    else
        echo "Creating HBM container..."
        echo $WhereAmI
        docker run -d -v $WhereAmI:/home/HydraOS/ --name HBM firelscar/hbm:latest tail -f /dev/null
    fi
    echo "Checking for macOS dependencies..."
    which -s mtools
    if [[ $? != 0 ]] ; then
        echo "Installing mtools"
        brew install mtools
    fi
    which -s qemu-system-x86_64
    if [[ $? != 0 ]] ; then
        echo "Installing qemu"
        brew install qemu
    fi
    echo "Done!"
    echo "Build and run with the 'make' command"
    echo "Make a USB with 'make usb'"
fi