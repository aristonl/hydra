rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

AssemblySource = $(call rwildcard,Source,*.s)  
Objects = $(patsubst Source/%.s, Build/Source/%.so, $(AssemblySource))
Directories = $(wildcard Source/*)

Build/Source/%.so: Source/%.s
	@echo Compiling /$(subst Source/,,$^)
	@cd Source;nasm $(subst Source/,,$^) -o ../$@

clean:
	@rm -r Build/Source
	@mkdir Build/Source

all:
	@cd Source;nasm boot.s -o ../Build/Source/boot.so
	@cd Source;nasm extension.s -f elf64 -o ../Build/Source/extension.elf
	@cd Build/Source;ld -o ../bootloader.bin -Ttext 0x7e00 extension.elf
	@cat Build/Source/boot.so Build/Source/extension.so > Build/Bootloader.bin

emulate:
	@qemu-system-x86_64 -drive file=Build/Bootloader.bin,format=raw -m 256M -cpu qemu64