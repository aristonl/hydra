all: 
	@x86_64-w64-mingw32-gcc -Wall -Werror -m64 -mabi=ms -ffreestanding -c Source/main.cpp -o Build/main.o
	@x86_64-w64-mingw32-gcc Build/main.o -Wall -Werror -m64 -mabi=ms -nostdlib -shared -Wl,-dll -Wl,--subsystem,10 -e boot -o ../Build/ISO/EFI/BOOT/bootx64.efi