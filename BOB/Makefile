# Recommended that you use WSL 2 for the time being...
# Installs: build-essential mtools qemu mingw-w64
all: build emulate

emulate:
	@printf "Emulating...               \r"
	@qemu-system-x86_64.exe -drive format=raw,unit=0,file=Build/Echo.img -drive if=pflash,format=raw,unit=0,file=Misc/ovmfc.fd,readonly=on -drive if=pflash,format=raw,unit=1,file=Misc/ovmfv.fd -m 256M  > /dev/null 2>&1

build:
	@mkdir -p Build/Source/
	@printf "Compiling...             \r"
	@x86_64-w64-mingw32-g++ -Wall -Werror -m64 -mabi=ms -ffreestanding -c Source/main.cpp -o Build/Source/main.o
	@x86_64-w64-mingw32-g++ Build/Source/main.o -Wall -Werror -m64 -mabi=ms -nostdlib -shared -Wl,-dll -Wl,--subsystem,10 -e boot -o Build/bootx64.efi
	@printf "Creating Image...             \r"
ifeq ($(shell [ -e Build/Echo.img ] && echo 1 || echo 0 ), 0)
	@dd if=/dev/zero of=Build/Echo.img bs=512 count=93750 > /dev/null 2>&1
endif
	@printf "Formatting Image...             \r"
	@-mformat -i Build/Echo.img -f 1440 ::
	@printf "Copying Files...             \r"
	@-mmd -i Build/Echo.img ::/EFI
	@-mmd -i Build/Echo.img ::/EFI/BOOT
	@-mcopy -i Build/Echo.img Build/bootx64.efi ::/EFI/BOOT
	@-mcopy -i Build/Echo.img Misc/startup.nsh ::

.PHONY: build