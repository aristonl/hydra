cmake_minimum_required(VERSION 3.16)

project(Hydra C CXX ASM_NASM)

include_directories(.)
include_directories(Libraries/LibC)
include_directories(Libraries/LibM)
include_directories(Libraries/LibTar)
include_directories(Libraries/LibUtils)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/Libraries)

add_subdirectory(Inferno)
add_subdirectory(Libraries)

add_custom_target(Build
	COMMAND ninja
	COMMAND ninja install-inferno image iso
	USES_TERMINAL
)

add_custom_target(install-inferno
	COMMAND mv Inferno/Inferno Build/ISO/kernel
	USES_TERMINAL
)

if(NOT EXISTS Build/ISO/Hydra.img)
	set(imagecmd dd if=/dev/zero of=Build/ISO/Hydra.img bs=512 count=93750)
endif()

add_custom_target(image
	COMMAND ${imagecmd}
	COMMAND mformat -i Build/ISO/Hydra.img -F ::
	COMMAND mmd -i Build/ISO/Hydra.img ::/EFI
	COMMAND mmd -i Build/ISO/Hydra.img ::/EFI/BOOT
	COMMAND mcopy -i Build/ISO/Hydra.img Build/ISO/EFI/BOOT/bootx64.efi ::/EFI/BOOT
	COMMAND mcopy -i Build/ISO/Hydra.img Build/ISO/error.tga ::
	COMMAND mcopy -i Build/ISO/Hydra.img Build/ISO/kernel ::
	COMMAND mcopy -i Build/ISO/Hydra.img Build/ISO/startup.nsh ::
	USES_TERMINAL
)

add_custom_target(iso
	COMMAND xorriso -as mkisofs -R -f -e Hydra.img -no-emul-boot -V "Hydra" -o Build/Hydra.iso Build/ISO/
	USES_TERMINAL
)

if(NOT EXISTS /tmp/OVMF_CODE.fd)
	set(preqemu wget https://github.com/Null-LLC/OVMF/raw/main/OVMF_CODE.fd -O /tmp/OVMF_CODE.fd && wget https://github.com/Null-LLC/OVMF/raw/main/OVMF_VARS.fd -O /tmp/OVMF_VARS.fd)
endif()

add_custom_target(emulate
	COMMAND ${preqemu}
	COMMAND qemu-system-x86_64 -machine q35 -drive format=raw,file=Build/Hydra.iso -drive if=pflash,format=raw,unit=0,file=/tmp/OVMF_CODE.fd,readonly=on -drive if=pflash,format=raw,unit=1,file=/tmp/OVMF_VARS.fd -m 256M -net none -cpu qemu64 -serial mon:stdio
	USES_TERMINAL
)