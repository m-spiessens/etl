cmake_minimum_required(VERSION 3.22)

project(MemoryChain)

include(CTest)

add_executable(memorychain)

target_include_directories(memorychain
PRIVATE
    ./
)

target_sources(memorychain
PRIVATE
    main.cpp
)

target_compile_options(memorychain
PRIVATE
    -std=c++20
    -pedantic
)

add_test(NAME memorychain COMMAND memorychain)
