all:
	@mkdir -p Build/
	@x86_64-w64-mingw32-gcc -Werror -m64 -maccumulate-outgoing-args -ffreestanding -c Source/main.cpp -o Build/main.o
	@x86_64-w64-mingw32-gcc Build/main.o -Werror -m64 -nostdlib -shared -Wl,-dll -Wl,--subsystem,10 -e boot -o ../Build/ISO/EFI/BOOT/bootx64.efi