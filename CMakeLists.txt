#CMake minimum version reqirement
cmake_minimum_required(VERSION 3.14)

project(Concurrency)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)
add_executable(Concurrency main.cpp)

