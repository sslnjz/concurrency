#CMake minimum version reqirement
cmake_minimum_required(VERSION 3.14)

project(Concurrency VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(src)
add_executable(Concurrency main.cpp src/thread_safe_dequeue.cpp src/thread_safe_dequeue.h)

