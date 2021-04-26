all:
	@if [ $(OS) = "macOS" ]; then\
        docker exec HBM /bin/bash -c "cd /home/HydraOS/gnu-efi/;make bootloader";\
        docker exec HBM /bin/bash -c "cd /home/HydraOS/kernel/;make kernel";\
		cd kernel;make buildimg;make run;\
	elif [ $(OS) = "Ubuntu" ] || [ $(OS) = "Debian" ]; then\
		cd gnu-efi;make bootloader;cd ../kernel;make kernel; make buildimg; make run;\
    fi

usb:
	@-cp /Users/fire/Documents/GitHub/Hydra/kernel/bin/kernel.elf /Volumes/ESD-USB/kernel.elf
	@-cp /Users/fire/Documents/GitHub/Hydra/kernel/bin/font.psf /Volumes/ESD-USB/font.psf
	@-cp /Users/fire/Documents/GitHub/Hydra/gnu-efi/x86_64/bootloader/main.efi /Volumes/ESD-USB/efi/boot/bootx64.efi
