OS = "macOS"

all:
	@-if [ $(OS) = "macOS" ]; then\
        docker exec HBM /bin/bash -c "cd /home/HydraOS/gnu-efi/;make bootloader";\
        docker exec HBM /bin/bash -c "cd /home/HydraOS/kernel/;make kernel";\
		cd kernel;make buildimg;make run;\
	elif [ $(OS) = "Ubuntu" ] || [ $(OS) = "Debian" ]; then\
		cd gnu-efi;make bootloader;cd ../kernel;make kernel; make buildimg; make run;\
    fi
