#CMake minimum version reqirement
cmake_minimum_required(VERSION 3.14)

project(Concurrency VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(src)
add_executable(Concurrency main.cpp)

