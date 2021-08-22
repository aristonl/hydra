### TEMP VARIABLES
## WILL BE REMOVED IN NEXT COMMIT BY FIRELSCAR
## DONT USE SETUP UNTIL NEXT COMMIT, IT'S NOT FINISHED!
QEMU = normal
QEMU_MEMORY = 512M

DOCKER = false
DOCKER_CONTAINER =
DOCKER_ECHO_ROOT =

all: build image iso emulate

setup:
	@mkdir -p Build/Source/
	@mkdir -p Build/ISO/EFI/BOOT
ifeq ($(shell echo $$WSL_DISTRO_NAME),)

else
	@echo "We've detected that this is running on WSL\n"
	@printf "Do you want to use QEMU on windows? [Y/"
	@printf "n] "
	@read -r qemu_type;if [ "$$qemu_type" = "Y" ] || [ "$$qemu_type" = "y" ] || [ "$$qemu_type" = "" ]; then \
		if [ -f build.config ]; then \
			sed -i '$(shell echo `grep -n "^QEMU=" build.config | cut -d : -f 1`)s/.*/QEMU=windows/' build.config; \
		else \
			touch build.config; \
			echo "QEMU=windows" >> build.config; \
			echo "QEMU_MEMORY=512M" >> build.config; \
		fi \
	else \
		if [ -f build.config ]; then \
			sed -i '$(shell echo `grep -n "^QEMU=" build.config | cut -d : -f 1`)s/.*/QEMU=normal/' build.config; \
		else \
			touch build.config; \
			echo "QEMU=normal" >> build.config; \
			echo "QEMU_MEMORY=512M" >> build.config; \
		fi \
	fi
endif

build:
	@x86_64-w64-mingw32-gcc -Wall -Werror -m64 -mabi=ms -ffreestanding -c Source/main.cpp -o Build/Source/main.o
	@x86_64-w64-mingw32-gcc Build/Source/main.o -Wall -Werror -m64 -mabi=ms -nostdlib -shared -Wl,-dll -Wl,--subsystem,10 -e boot -o Build/ISO/EFI/BOOT/bootx64.efi

image:
ifeq ($(shell [ -e Build/ISO/Echo.img ] && echo 1 || echo 0), 0)
	@dd if=/dev/zero of=Build/ISO/Echo.img bs=512 count=93750 > /dev/null 2>&1
endif
ifeq ($(DOCKER), true)
	@docker exec -it $(DOCKER_CONTAINER) sh -c "cd $(DOCKER_ECHO_ROOT);mformat -i Build/ISO/Echo.img -f 1440 ::"
else
	@-mformat -i Build/ISO/Echo.img -f 1440 ::
endif
	@-mmd -i Build/ISO/Echo.img ::/EFI
	@-mmd -i Build/ISO/Echo.img ::/EFI/BOOT
	@-mcopy -i Build/ISO/Echo.img Build/ISO/EFI/BOOT/bootx64.efi ::/EFI/BOOT
	@-mcopy -i Build/ISO/Echo.img Build/ISO/startup.nsh ::

iso:
	@-xorriso -as mkisofs -R -f -e Echo.img -no-emul-boot -V "Echo" -o Build/Echo.iso Build/ISO/ > /dev/null 2>&1

emulate:
ifeq ($(QEMU), windows)
ifeq ($(shell [ -e /tmp/OVMF_VARS.fd ] && echo 1 || echo 0), 0)
	@cp /usr/share/OVMF/OVMF_VARS.fd /tmp/OVMF_VARS.fd
endif
	@qemu-system-x86_64.exe -drive format=raw,file=Build/Echo.iso -drive if=pflash,format=raw,unit=0,file=\\\\wsl\$$\\\$(shell echo $$WSL_DISTRO_NAME)\\usr\\share\\OVMF\\OVMF_CODE.fd,readonly=on -drive if=pflash,format=raw,unit=1,file=\\\\wsl\$$\\\$(shell echo $$WSL_DISTRO_NAME)\\tmp\\OVMF_VARS.fd -m 256M
endif
ifeq ($(QEMU), normal)
	@qemu-system-x86_64 -drive format=raw,file=Build/Echo.iso -drive if=pflash,format=raw,unit=0,file=/usr/share/OVMF/OVMF_CODE.fd,readonly=on -drive if=pflash,format=raw,unit=1,file=/tmp/OVMF_VARS.fd -m 256M
endif

.PHONY: build