#CMake minimum version reqirement
cmake_minimum_required(VERSION 3.14)

project(Concurrency)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)
aux_source_directory(. DIR_SRCS)

add_executable(Concurrency ${DIR_SRCS})

