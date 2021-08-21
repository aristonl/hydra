# Recommended that you use WSL 2 for the time being...
# Installs: build-essential mtools qemu mingw-w64
all: build emulate

emulate:
	@printf "Emulating...               \r"
ifeq ($(shell [ -e /tmp/OVMF_VARS.fd ] && echo 1 || echo 0), 0)
	@cp /usr/share/OVMF/OVMF_VARS.fd /tmp/OVMF_VARS.fd
endif
	@qemu-system-x86_64.exe -drive format=raw,file=Build/Echo.iso -drive if=pflash,format=raw,unit=0,file=\\\\wsl\$$\\\Ubuntu-20.04\\usr\\share\\OVMF\\OVMF_CODE.fd,readonly=on -drive if=pflash,format=raw,unit=1,file=\\\\wsl\$$\\\Ubuntu-20.04\\tmp\\OVMF_VARS.fd -m 256M

build:
	@mkdir -p Build/Source/
	@mkdir -p Build/ISO/EFI/BOOT
	@printf "Compiling...             \r"
	@x86_64-w64-mingw32-gcc -Wall -Werror -m64 -mabi=ms -ffreestanding -c Source/main.cpp -o Build/Source/main.o
	@x86_64-w64-mingw32-gcc Build/Source/main.o -Wall -Werror -m64 -mabi=ms -nostdlib -shared -Wl,-dll -Wl,--subsystem,10 -e boot -o Build/ISO/EFI/BOOT/bootx64.efi
	@printf "Creating Image...             \r"
ifeq ($(shell [ -e Build/ISO/Echo.img ] && echo 1 || echo 0), 0)
	@dd if=/dev/zero of=Build/ISO/Echo.img bs=512 count=93750 > /dev/null 2>&1
endif
	@printf "Formatting Image...             \r"
	@-mformat -i Build/ISO/Echo.img -f 1440 ::
#@docker exec -it EBM sh -c "cd /home/Echo/BOB/;mformat -i Build/ISO/Echo.img -f 1440 ::"
	@printf "Copying Files...             \r"
	@-mmd -i Build/ISO/Echo.img ::/EFI
	@-mmd -i Build/ISO/Echo.img ::/EFI/BOOT
	@-mcopy -i Build/ISO/Echo.img Build/ISO/EFI/BOOT/bootx64.efi ::/EFI/BOOT
	@-mcopy -i Build/ISO/Echo.img Build/ISO/startup.nsh ::
	@printf "Creating ISO...             \r"
	@-xorriso -as mkisofs -R -f -e Echo.img -no-emul-boot -V "Echo" -o Build/Echo.iso Build/ISO/ > /dev/null 2>&1

.PHONY: build