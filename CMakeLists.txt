#CMake minimum version reqirement
cmake_minimum_required(VERSION 3.14)

project(Concurrency VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRCS
    main.cpp
    src/thread_pool.h
    src/thread_safe_queue.h
    src/thread_safe_stack.h
    src/thread_sequence.h
    src/thread_safe_dequeue.h
    src/memory_pool.h)

include_directories(src)
add_executable(Concurrency ${SRCS})

