set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

SET(CMAKE_SYSTEM_NAME Generic)

SET(CMAKE_SYSTEM_PROCESSOR x86_64)
SET(CMAKE_CROSSCOMPILING 1)

set(CMAKE_C_COMPILER "${CMAKE_CURRENT_SOURCE_DIR}/Toolchain/Local/x86_64/bin/x86_64-hydra-gcc")
set(CMAKE_CXX_COMPILER "${CMAKE_CURRENT_SOURCE_DIR}/Toolchain/Local/x86_64/bin/x86_64-hydra-g++")

cmake_minimum_required(VERSION 3.16)

project(Hydra C CXX ASM_NASM)

include_directories(.)
include_directories(Libraries/LibC/include)
include_directories(Libraries/LibM)
include_directories(Libraries/LibTar)
include_directories(Libraries/LibUtils)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/Libraries)

add_subdirectory(Boot)
add_subdirectory(Kernel)
add_subdirectory(Libraries)
