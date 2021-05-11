RAM = 1G
GNUEFI = bootloader
OVMFDIR = Build/OVMF
CC = gcc
ASMC = nasm
LD = ld

CFLAGS = -ffreestanding -fshort-wchar -mno-red-zone
ASMFLAGS = 
LDFLAGS = -T $(LDS) -static -Bsymbolic -nostdlib

SRCDIR := Kernel
OBJDIR := Build/Binaries
BUILDDIR = Build
BOOTEFI := $(BUILDDIR)/bootloader.efi
LDS = $(SRCDIR)/kernel.ld

all:
	@-printf "Building Kernel...\r"
	@-d=$$(date +%s);\
        make build;\
	time=$$(($$(date +%s)-d));\
	if [ $$time -eq 1 ]; then\
		echo "Kernel took $$((time)) second to compile!";\
	else\
		echo "Kernel took $$((time)) seconds to compile!";\
	fi
	@-printf "Making ISO...     \r"
	@-make image
	@-printf "Starting Qemu...  \r"
	@-make run

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC = $(call rwildcard,$(SRCDIR),*.cpp)  
ASMSRC = $(call rwildcard,$(SRCDIR),*.asm)  
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
OBJS += $(patsubst $(SRCDIR)/%.asm, $(OBJDIR)/%_asm.o, $(ASMSRC))
DIRS = $(wildcard $(SRCDIR)/*)

.PHONY: build

build:
	@-if [ `head -n 1 hbm` = "OS=macOS" ]; then\
		docker exec HBM /bin/bash -c "cd /home/HydraOS/;make kernel";\
	elif [ `head -n 1 hbm` = "OS=Debian" ]; then\
		make kernel;\
	else\
		echo "Please run ./setup-environment.sh";\
	fi

kernel: $(OBJS) link

$(OBJDIR)/drivers/interrupts/interrupts.o: $(SRCDIR)/drivers/interrupts/interrupts.cpp
	@-mkdir -p $(@D)
	@-$(CC) -mno-red-zone -mgeneral-regs-only -ffreestanding -c $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@-mkdir -p $(@D)
	@-$(CC) $(CFLAGS) -c $^ -o $@
	
$(OBJDIR)/%_asm.o: $(SRCDIR)/%.asm
	@-mkdir -p $(@D)
	@-$(ASMC) $(ASMFLAGS) $^ -f elf64 -o $@
	 
link:
	@-$(LD) $(LDFLAGS) -o $(BUILDDIR)/kernel.elf $(OBJS)

image:
	@-dd if=/dev/zero of=$(BUILDDIR)/Hydra.iso bs=512 count=93750 > /dev/null 2>&1
	@-mformat -i $(BUILDDIR)/Hydra.iso -f 1440 ::
	@-mmd -i $(BUILDDIR)/Hydra.iso ::/EFI
	@-mmd -i $(BUILDDIR)/Hydra.iso ::/EFI/BOOT
	@-mcopy -i $(BUILDDIR)/Hydra.iso $(BOOTEFI) ::/EFI/BOOT
	@-mcopy -i $(BUILDDIR)/Hydra.iso $(BUILDDIR)/startup.nsh ::
	@-mcopy -i $(BUILDDIR)/Hydra.iso $(BUILDDIR)/kernel.elf ::
	@-mcopy -i $(BUILDDIR)/Hydra.iso $(BUILDDIR)/font.psf ::

run:
	@-if [ `head -n 1 hbm` = "OS=macOS" ]; then\
		qemu-system-x86_64 -drive file=$(BUILDDIR)/Hydra.iso,format=raw -m $(RAM) -cpu qemu64 -drive if=pflash,format=raw,unit=0,file="$(OVMFDIR)/OVMF_CODE-pure-efi.fd",readonly=on -drive if=pflash,format=raw,unit=1,file="$(OVMFDIR)/OVMF_VARS-pure-efi.fd" -net none -boot order=d;\
	elif [ `head -n 1 hbm` = "OS=Debian" ]; then\
		if [ -d "/mnt/c/" ]; then\
			cmd.exe /C qemu-system-x86_64 -drive file=$(BUILDDIR)/Hydra.iso,format=raw -m $(RAM) -cpu qemu64 -drive if=pflash,format=raw,unit=0,file=$(OVMFDIR)/OVMF_CODE-pure-efi.fd,readonly=on -drive if=pflash,format=raw,unit=1,file=$(OVMFDIR)/OVMF_VARS-pure-efi.fd -net none -boot order=d;\
		else\
			qemu-system-x86_64 -drive file=$(BUILDDIR)/Hydra.iso,format=raw -m $(RAM) -cpu qemu64 -drive if=pflash,format=raw,unit=0,file="$(OVMFDIR)/OVMF_CODE-pure-efi.fd",readonly=on -drive if=pflash,format=raw,unit=1,file="$(OVMFDIR)/OVMF_VARS-pure-efi.fd" -net none -boot order=d;\
		fi;\
	else\
		echo "Please run ./setup-environment.sh";\
	fi

clean:
	@-rm -r Build/Binaries/*