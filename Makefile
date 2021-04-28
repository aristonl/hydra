RAM = 512M
GNUEFI = bootloader
OVMFDIR = etc
LDS = src/kernel.ld
CC = gcc
ASMC = nasm
LD = ld

CFLAGS = -ffreestanding -fshort-wchar
ASMFLAGS = 
LDFLAGS = -T $(LDS) -static -Bsymbolic -nostdlib

SRCDIR := src
OBJDIR := bin
BUILDDIR = Hydra
BOOTEFI := $(GNUEFI)/x86_64/bootloader/main.efi

all:
	@-if [ $(OS) = "macOS" ]; then\
        docker exec HBM /bin/bash -c "cd /home/HydraOS/bootloader/;make bootloader";\
        docker exec HBM /bin/bash -c "cd /home/HydraOS/;make kernel";\
		make buildimg;make run;\
	elif [ $(OS) = "Ubuntu" ] || [ $(OS) = "Debian" ]; then\
		cd bootloader;make bootloader;cd ../;make kernel; make buildimg; make run;\
    fi

clean:
	@-rm -r bin Hydra
	@-mkdir bin Hydra

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC = $(call rwildcard,$(SRCDIR),*.cpp)  
ASMSRC = $(call rwildcard,$(SRCDIR),*.asm)  
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
OBJS += $(patsubst $(SRCDIR)/%.asm, $(OBJDIR)/%_asm.o, $(ASMSRC))
DIRS = $(wildcard $(SRCDIR)/*)

kernel: $(OBJS) link

$(OBJDIR)/interrupts/interrupts.o: $(SRCDIR)/interrupts/interrupts.cpp
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

buildimg:
	@-dd if=/dev/zero of=$(BUILDDIR)/Hydra.img bs=512 count=93750
	@-mformat -i $(BUILDDIR)/Hydra.img -f 1440 ::
	@-mmd -i $(BUILDDIR)/Hydra.img ::/EFI
	@-mmd -i $(BUILDDIR)/Hydra.img ::/EFI/BOOT
	@-mcopy -i $(BUILDDIR)/Hydra.img $(BOOTEFI) ::/EFI/BOOT
	@-mcopy -i $(BUILDDIR)/Hydra.img startup.nsh ::
	@-mcopy -i $(BUILDDIR)/Hydra.img $(BUILDDIR)/kernel.elf ::
	@-mcopy -i $(BUILDDIR)/Hydra.img etc/font.psf ::

run:
	@-qemu-system-x86_64 -drive file=$(BUILDDIR)/Hydra.img,format=raw -m $(RAM) -cpu qemu64 -drive if=pflash,format=raw,unit=0,file="$(OVMFDIR)/OVMF_CODE-pure-efi.fd",readonly=on -drive if=pflash,format=raw,unit=1,file="$(OVMFDIR)/OVMF_VARS-pure-efi.fd" -net none
