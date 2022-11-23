# Setup NASM
set(CMAKE_ASM_NASM_SOURCE_FILE_EXTENSIONS ${CMAKE_ASM_NASM_SOURCE_FILE_EXTENSIONS} s S)
enable_language(ASM_NASM)
set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)
set(CMAKE_ASM_NASM_COMPILE_OBJECT "nasm <SOURCE> -f elf64 -o <OBJECT>")

# Find source files
file(GLOB_RECURSE CPP_SRCS Source/*.cpp)
file(GLOB_RECURSE ASM_SRCS Source/*.s)
set_source_files_properties(${ASM_SRCS} PROPERTIES LANGUAGE ASM_NASM)

# Compile
set(CMAKE_CXX_FLAGS "-ffreestanding -fshort-wchar -g -mabi=sysv -no-pie -mno-red-zone -fpermissive -mgeneral-regs-only -O2 -fno-stack-protector -fno-exceptions")
add_link_options(-static -Bsymbolic -T${CMAKE_CURRENT_SOURCE_DIR}/linker.ld -fno-stack-protector -nostdlib -e main)
include_directories(Include)
add_executable(Inferno ${CPP_SRCS} ${ASM_SRCS})

set_target_properties(Inferno PROPERTIES LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld)

add_custom_command(TARGET Inferno POST_BUILD
	COMMAND mv ${CMAKE_CURRENT_SOURCE_DIR}/Inferno ${CMAKE_CURRENT_SOURCE_DIR}/../Build/ISO/kernel
	USES_TERMINAL
)

if(NOT EXISTS Build/ISO/Hydra.img)
	set(imagecmd dd if=/dev/zero of=${CMAKE_CURRENT_SOURCE_DIR}/../Build/ISO/Hydra.img bs=512 count=93750)
endif()

add_custom_command(TARGET Inferno POST_BUILD
	COMMAND ${imagecmd}
	COMMAND mformat -i ${CMAKE_CURRENT_SOURCE_DIR}/../Build/ISO/Hydra.img -F ::
	COMMAND mmd -i ${CMAKE_CURRENT_SOURCE_DIR}/../Build/ISO/Hydra.img ::/EFI
	COMMAND mmd -i ${CMAKE_CURRENT_SOURCE_DIR}/../Build/ISO/Hydra.img ::/EFI/BOOT
	COMMAND mcopy -i ${CMAKE_CURRENT_SOURCE_DIR}/../Build/ISO/Hydra.img ${CMAKE_CURRENT_SOURCE_DIR}/../Build/ISO/EFI/BOOT/bootx64.efi ::/EFI/BOOT
	COMMAND mcopy -i ${CMAKE_CURRENT_SOURCE_DIR}/../Build/ISO/Hydra.img ${CMAKE_CURRENT_SOURCE_DIR}/../Build/ISO/error.tga ::
	COMMAND mcopy -i ${CMAKE_CURRENT_SOURCE_DIR}/../Build/ISO/Hydra.img ${CMAKE_CURRENT_SOURCE_DIR}/../Build/ISO/kernel ::
	COMMAND mcopy -i ${CMAKE_CURRENT_SOURCE_DIR}/../Build/ISO/Hydra.img ${CMAKE_CURRENT_SOURCE_DIR}/../Build/ISO/startup.nsh ::
	USES_TERMINAL
)

add_custom_command(TARGET Inferno POST_BUILD
	COMMAND xorriso -as mkisofs -R -f -e Hydra.img -no-emul-boot -V "Hydra" -o ${CMAKE_CURRENT_SOURCE_DIR}/../Build/Hydra.iso ${CMAKE_CURRENT_SOURCE_DIR}/../Build/ISO/
	USES_TERMINAL
)

if(NOT EXISTS /tmp/OVMF_CODE.fd)
	set(preqemu wget https://github.com/Null-LLC/OVMF/raw/main/OVMF_CODE.fd -O /tmp/OVMF_CODE.fd && wget https://github.com/Null-LLC/OVMF/raw/main/OVMF_VARS.fd -O /tmp/OVMF_VARS.fd)
endif()

# I'ma be honest, probably a better way to do this is to run it thru a shell 
# script (like Serenity) so we don't have to make custom targets for emulate
# and no display emulate and instead we could run something like:
#
# 		./run no-display
#
# or something like that. Would make life much easier...
# 
# 	- aristonl
#	P.S. wouldn't it be easier just to move the emulate targets to the root
#		 CMakeLists.txt?
add_custom_target(emulate
	COMMAND ${preqemu}
	COMMAND qemu-system-x86_64 -gdb serial -machine q35 -drive format=raw,file=${CMAKE_CURRENT_SOURCE_DIR}/../Build/Hydra.iso -drive if=pflash,format=raw,unit=0,file=/tmp/OVMF_CODE.fd,readonly=on -drive if=pflash,format=raw,unit=1,file=/tmp/OVMF_VARS.fd -m 256M -net none -cpu qemu64 -serial mon:stdio
	USES_TERMINAL
)

add_custom_target(emulate-no-display
	COMMAND ${preqemu}
	COMMAND qemu-system-x86_64 -machine q35 -drive format=raw,file=${CMAKE_CURRENT_SOURCE_DIR}/../Build/Hydra.iso -drive if=pflash,format=raw,unit=0,file=/tmp/OVMF_CODE.fd,readonly=on -drive if=pflash,format=raw,unit=1,file=/tmp/OVMF_VARS.fd -m 256M -net none -cpu qemu64 -display none -serial mon:stdio
	USES_TERMINAL
)

add_dependencies(emulate Inferno)
