cmake_minimum_required(VERSION 3.22)

project(etl)

include(CTest)

add_executable(test_memorychain)

target_include_directories(test_memorychain
PRIVATE
    ./
)

target_sources(test_memorychain
PRIVATE
    test_memorychain.cpp
)

target_compile_options(test_memorychain
PRIVATE
    -std=c++20
    -pedantic
)

add_test(NAME test_memorychain COMMAND test_memorychain)

add_executable(test_queue)

target_include_directories(test_queue
PRIVATE
    ./
)

target_sources(test_queue
PRIVATE
    test_queue.cpp
)

target_compile_options(test_queue
PRIVATE
    -std=c++20
    -pedantic
)

add_test(NAME test_queue COMMAND test_queue)

add_executable(test_pool)

target_include_directories(test_pool
PRIVATE
    ./
)

target_sources(test_pool
PRIVATE
    test_pool.cpp
)

target_compile_options(test_pool
PRIVATE
    -std=c++20
    -pedantic
)

add_test(NAME test_pool COMMAND test_pool)
