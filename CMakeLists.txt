cmake_minimum_required(VERSION 3.16)

project(Hydra C CXX ASM_NASM)

include_directories(.)
include_directories(lib/libc/include)
include_directories(lib/libm)
include_directories(lib/libtar)
include_directories(lib/libutils)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/lib)

add_subdirectory(Kernel)
add_subdirectory(lib)
