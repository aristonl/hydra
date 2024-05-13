set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

SET(CMAKE_SYSTEM_NAME Generic)

SET(CMAKE_SYSTEM_PROCESSOR x86_64)
SET(CMAKE_CROSSCOMPILING 1)

cmake_minimum_required(VERSION 3.5.1)

project(bob CXX)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_COMPILER "x86_64-w64-mingw32-g++")

set(SOURCE_FILES main.cpp)

# x86_64-w64-mingw32-g++ -Werror -m64 -maccumulate-outgoing-args -ffreestanding -c $^ -o $@
set(CMAKE_CXX_FLAGS "-Werror -m64 -maccumulate-outgoing-args -ffreestanding")

# x86_64-w64-mingw32-g++ $(Objects) -Werror -m64 -nostdlib -shared -Wl,-dll -Wl,--subsystem,10 -e boot -o bootx64.efi
set(CMAKE_EXE_LINKER_FLAGS "-Werror -m64 -nostdlib -shared -Wl,-dll -Wl,--subsystem,10 -e boot")

set(CMAKE_OSX_ARCHITECTURES x86_64)

set(CMAKE_OSX_SYSROOT "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk")

add_executable(bootx64.efi ${SOURCE_FILES})

add_custom_target(srcboot
	COMMAND cp bootx64.efi ../Build/ISO/EFI/BOOT/bootx64.efi
	DEPENDS bootx64.efi
)
