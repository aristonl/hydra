GUEST-BOOTLOADER = /home/HydraOS/gnu-efi/
GUEST-KERNEL = /home/HydraOS/kernel/
HOST-KERNEL = /Users/fire/Documents/GitHub/Hydra/kernel/

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
P3:
	@echo Creating OS ISO
	@-cd $(HOST-KERNEL);make buildiso