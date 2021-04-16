GUEST-BOOTLOADER = /home/HydraOS/gnu-efi/
GUEST-KERNEL = /home/HydraOS/kernel/
HOST-KERNEL = /Users/fire/Documents/GitHub/Hydra/kernel/

all:
	@-sh compile.sh
P1:
	@echo Creating bootloader...
	@-cd $(GUEST-BOOTLOADER);make bootloader
	@echo Creating Kernel binary...
	@-cd $(GUEST-KERNEL);make kernel
	@echo Done!
P2:
	@echo Creating OS image
	@-cd $(HOST-KERNEL);make buildimg
	@echo Running OS
	@-cd $(HOST-KERNEL);make run
usb:
	@-cp /Users/fire/Documents/GitHub/Hydra/kernel/bin/kernel.elf /Volumes/ESD-USB/kernel.elf
	@-cp /Users/fire/Documents/GitHub/Hydra/kernel/bin/font.psf /Volumes/ESD-USB/font.psf
	@-cp /Users/fire/Documents/GitHub/Hydra/gnu-efi/x86_64/bootloader/main.efi /Volumes/ESD-USB/efi/boot/bootx64.efi