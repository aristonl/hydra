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
set(CMAKE_CXX_FLAGS "-ffreestanding -fshort-wchar -g -mabi=sysv -no-pie -mno-red-zone -mgeneral-regs-only -O2 -fno-stack-protector -fno-exceptions")
add_link_options(-static -Bsymbolic -fno-stack-protector -nostdlib -e main)
include_directories(Include)
add_executable(Inferno ${CPP_SRCS} ${ASM_SRCS})

set_target_properties(Inferno PROPERTIES LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld)