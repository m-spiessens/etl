cmake_minimum_required(VERSION 3.22)

project(etl)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(etl INTERFACE)

target_include_directories(etl
INTERFACE
    ./
)

if(${CMAKE_SYSTEM_NAME} STREQUAL Linux)

    include(CTest)

    add_executable(test_memorychain)
    add_test(NAME test_memorychain COMMAND test_memorychain)

    target_sources(test_memorychain
    PRIVATE
        test_memorychain.cpp
    )

    target_link_libraries(test_memorychain
    PRIVATE
        etl
    )

    target_compile_options(test_memorychain
    PRIVATE
        -pedantic
        -Wall
        -Wextra
    )

    add_executable(test_queue)
    add_test(NAME test_queue COMMAND test_queue)

    target_include_directories(test_queue
    PRIVATE
        ./
    )

    target_sources(test_queue
    PRIVATE
        test_queue.cpp
    )

    target_link_libraries(test_queue
    PRIVATE
        etl
    )

    target_compile_options(test_queue
    PRIVATE
        -pedantic
        -Wall
        -Wextra
    )

    add_executable(test_pool)
    add_test(NAME test_pool COMMAND test_pool)

    target_include_directories(test_pool
    PRIVATE
        ./
    )

    target_sources(test_pool
    PRIVATE
        test_pool.cpp
    )

    target_link_libraries(test_pool
    PRIVATE
        etl
    )

    target_compile_options(test_pool
    PRIVATE
        -pedantic
        -Wall
        -Wextra
    )

endif()
