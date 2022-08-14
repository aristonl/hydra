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
