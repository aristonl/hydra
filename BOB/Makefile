# Recommended that you use WSL 2 for the time being...
# Installs: build-essential mingw-w64
# Requirement: Please install QEMU 2020 on your Host system (Windows) https://qemu.weilnetz.de/w64/ (PLEASE ENSURE YOU DOWNLOAD A VERSION FROM 2020)
all: build emulate

emulate:
	@printf "Emulating...               \r"
	@qemu-system-x86_64.exe -drive format=raw,unit=0,file=Build/Drive.hdd -bios Misc/BIOS64.bin -m 256M

build:
	@mkdir -p Build/Source/
	@printf "Compiling...             \r"
	@x86_64-w64-mingw32-gcc -Wall -Werror -m64 -mabi=ms -ffreestanding -c Source/main.c -o Build/Source/main.o
	@x86_64-w64-mingw32-gcc Build/Source/main.o -Wall -Werror -m64 -mabi=ms -nostdlib -shared -Wl,-dll -Wl,--subsystem,10 -e efi_main -o Build/BOOTX64.EFI
	@printf "Mounting...              \r"
	@sudo mkdir -p /mnt/Echo
	@loc=`sudo losetup Build/Drive.hdd -fP --show`;sudo mount --verbose -tvfat $$loc\p1 /mnt/Echo
	@printf "Moving...               \r"
	@-sudo mv Build/BOOTX64.EFI /mnt/Echo/EFI/Boot/BOOTX64.EFI 2>/dev/null
	@printf "Unmounting...          \r"
	@sudo umount /mnt/Echo
	@sudo losetup -D

.PHONY: build