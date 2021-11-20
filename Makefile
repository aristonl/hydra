QEMU_MEMORY = 512M

.PHONY: BOB Inferno

BUILD_INSTRUCTIONS =
ifeq ($(shell echo `uname`), Darwin)
	BUILD_INSTRUCTIONS = vagrant emulate
else
	BUILD_INSTRUCTIONS = BOB Inferno image iso emulate
endif

all: $(BUILD_INSTRUCTIONS)

vagrant:
	@vagrant ssh -c "cd /vagrant/;make BOB Inferno image iso"

BOB:
	@-make -C "BOB" > /dev/null >&1

Inferno:
	@-cd Inferno > /dev/null >&1;make Inferno > /dev/null >&1

image:
ifeq ($(shell [ -e Build/ISO/Echo.img ] && echo 1 || echo 0), 0)
	@dd if=/dev/zero of=Build/ISO/Echo.img bs=512 count=93750 > /dev/null 2>&1
endif
	@-mformat -i Build/ISO/Echo.img -F ::
	@-mmd -i Build/ISO/Echo.img ::/EFI
	@-mmd -i Build/ISO/Echo.img ::/EFI/BOOT
	@-mcopy -i Build/ISO/Echo.img Build/ISO/EFI/BOOT/bootx64.efi ::/EFI/BOOT
	@-mcopy -i Build/ISO/Echo.img Build/ISO/Echo.tga ::
	@-mcopy -i Build/ISO/Echo.img Build/ISO/error.tga ::
	@-mcopy -i Build/ISO/Echo.img Build/ISO/font.psf ::
	@-mcopy -i Build/ISO/Echo.img Build/ISO/inferno ::
	@-mcopy -i Build/ISO/Echo.img Build/ISO/startup.nsh ::

iso:
	@-xorriso -as mkisofs -R -f -e Echo.img -no-emul-boot -V "Echo" -o Build/Echo.iso Build/ISO/ > /dev/null 2>&1

emulate:
ifeq ($(shell echo `uname`), Darwin)
ifeq ($(shell [ -e /tmp/OVMF_CODE.fd ] && [ -e /tmp/OVMF_VARS.fd ] && echo 1 || echo 0), 0)
	@wget https://github.com/Null-LLC/OVMF/raw/main/OVMF_CODE.fd -O /tmp/OVMF_CODE.fd > /dev/null 2>&1
	@wget https://github.com/Null-LLC/OVMF/raw/main/OVMF_VARS.fd -O /tmp/OVMF_VARS.fd > /dev/null 2>&1
endif
	@qemu-system-x86_64 -machine q35 -drive format=raw,file=Build/Echo.iso -drive if=pflash,format=raw,unit=0,file=/tmp/OVMF_CODE.fd,readonly=on -drive if=pflash,format=raw,unit=1,file=/tmp/OVMF_VARS.fd -m $(QEMU_MEMORY) -net none -cpu qemu64 -serial file:serial.log -soundhw pcspk > /dev/null 2>&1
else
ifeq ($(shell [ -e /tmp/OVMF_VARS.fd ] && echo 1 || echo 0), 0)
ifeq ($(shell [ -e /usr/share/OVMF/OVMF_VARS.fd ] && echo 1 || echo 0), 1)
	@cp /usr/share/OVMF/OVMF_VARS.fd /tmp/OVMF_VARS.fd
	@cp /usr/share/OVMF/OVMF_CODE.fd /tmp/OVMF_CODE.fd
else
	@cp /usr/share/OVMF/x64/OVMF_VARS.fd /tmp/OVMF_VARS.fd
	@cp /usr/share/OVMF/x64/OVMF_CODE.fd /tmp/OVMF_CODE.fd
endif
endif
	@qemu-system-x86_64 -machine q35 -drive format=raw,file=Build/Echo.iso -drive if=pflash,format=raw,unit=0,file=/tmp/OVMF_CODE.fd,readonly=on -drive if=pflash,format=raw,unit=1,file=/tmp/OVMF_VARS.fd -m $(QEMU_MEMORY) -net none -cpu qemu64 -serial file:serial.log -soundhw pcspk > /dev/null 2>&1
endif

clean:
	@rm -r Build/Echo.iso Build/ISO/Echo.img Build/ISO/inferno Build/ISO/EFI/BOOT/bootx64.efi
	@rm -r BOB/Build/
	@rm -r Inferno/Build/