rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

AssemblySource = $(call rwildcard,Source,*.s)  
Objects = $(patsubst Source/%.s, Build/Source/%.so, $(AssemblySource))
Directories = $(wildcard Source/*)

Build/Source/%.so: Source/%.s
	@echo Compiling /$(subst Source/,,$^)
	@nasm $^ -o $@

all: $(Objects)

emulate:
	@qemu-system-x86_64 Build/Source/boot.so