rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

AssemblySource = $(call rwildcard,Source,*.s)  
Objects = $(patsubst Source/%.s, Build/Source/%.so, $(AssemblySource))
Directories = $(wildcard Source/*)

Build/Source/%.so: Source/%.s
	@printf "Compiling /$(subst Source/,,$^)...                    \r"
	@cd Source;nasm $(subst Source/,,$^) -o ../$@

clean:
	@rm -r Build/Source
	@mkdir Build/Source

all: $(Objects)
	@cat Build/Source/boot.so Build/Source/extension.so > Build/Bootloader.bin

emulate:
	@qemu-system-x86_64 -drive file=Build/Bootloader.bin,format=raw