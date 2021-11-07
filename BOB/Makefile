rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

CPPSourceCode = $(call rwildcard,Source,*.cpp)
Objects = $(patsubst Source/%.cpp, Build/%.o, $(CPPSourceCode))
Directories = $(wildcard Source/*)

Build/%.o: Source/%.cpp
	@-mkdir -p $(@D)
	@-x86_64-w64-mingw32-gcc -Werror -m64 -maccumulate-outgoing-args -ffreestanding -c $^ -o $@

all: $(Objects)
	@x86_64-w64-mingw32-gcc Build/main.o -Werror -m64 -nostdlib -shared -Wl,-dll -Wl,--subsystem,10 -e boot -o ../Build/ISO/EFI/BOOT/bootx64.efi