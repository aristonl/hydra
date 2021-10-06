rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

CSourceCode = $(call rwildcard,Source,*.c)
CPPSourceCode = $(call rwildcard,Source,*.cpp)
ASMSourceCode = $(call rwildcard,Source,*.s)
Objects = $(patsubst Source/%.c, Build/%.o, $(CSourceCode))
Objects += $(patsubst Source/%.cpp, Build/%.o, $(CPPSourceCode))
Objects += $(patsubst Source/%.s, Build/%.so, $(ASMSourceCode))
Directories = $(wildcard Source/*)

Build/%.o: Source/%.c
	@-mkdir -p $(@D)
	@-gcc -ffreestanding -fshort-wchar -mabi=ms -no-pie -c $^ -o $@

Build/%.o: Source/%.cpp
	@-mkdir -p $(@D)
	@-g++ -ffreestanding -fshort-wchar -mabi=ms -no-pie -c $^ -o $@

Build/%.so: Source/%.s
	@-mkdir -p $(@D)
	@-nasm $^ -f elf64 -o $@

all: $(Objects)
	@ld -T linker.ld -L../std/ -L../Libraries/ -static -Bsymbolic -nostdlib -o ../Build/ISO/inferno $(Objects)