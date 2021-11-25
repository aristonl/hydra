rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

CSourceCode = $(call rwildcard,Source,*.c)
CPPSourceCode = $(call rwildcard,Source,*.cpp)
ASMSourceCode = $(call rwildcard,Source,*.s)
Objects = $(patsubst Source/%.c, Build/%.o, $(CSourceCode))
Objects += $(patsubst Source/%.cpp, Build/%.o, $(CPPSourceCode))
Objects += $(patsubst Source/%.s, Build/%.so, $(ASMSourceCode))
Directories = $(wildcard Source/*)

Build/Drivers/Interrupts/Interrupts.o: Source/Drivers/Interrupts/Interrupts.cpp
	@-mkdir -p $(@D)
	@-gcc -ffreestanding -fshort-wchar -mabi=sysv -no-pie -mno-red-zone -mgeneral-regs-only -fno-exceptions -c $^ -o $@

Build/%.o: Source/%.cpp
	@-mkdir -p $(@D)
	@-g++ -ffreestanding -fshort-wchar -fstack-protector -mno-red-zone -mabi=sysv -no-pie -fno-exceptions -c $^ -o $@

Build/%.so: Source/%.s
	@-mkdir -p $(@D)
	@-nasm $^ -f elf64 -o $@

Inferno: $(Objects)
	@gcc -Tlinker.ld -static -Bsymbolic -nostdlib -o ../Build/ISO/inferno $(Objects)
