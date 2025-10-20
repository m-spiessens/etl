cmake_minimum_required(VERSION 3.22)

project(etl)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(etl STATIC)

target_include_directories(etl
PUBLIC
    ./
)

target_sources(etl
PRIVATE
    source/string.cpp
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

    add_executable(test_map)
    add_test(NAME test_map COMMAND test_map)

    target_include_directories(test_map
    PRIVATE
        ./
    )

    target_sources(test_map
    PRIVATE
        test_map.cpp
    )

    target_link_libraries(test_map
    PRIVATE
        etl
    )

    target_compile_options(test_map
    PRIVATE
        -pedantic
        -Wall
        -Wextra
    )

    add_executable(test_map_string)
    add_test(NAME test_map_string COMMAND test_map_string)

    target_include_directories(test_map_string
    PRIVATE
        ./
    )

    target_sources(test_map_string
    PRIVATE
        test_map_string.cpp
    )

    target_link_libraries(test_map_string
    PRIVATE
        etl
    )

    target_compile_options(test_map_string
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

    add_executable(test_string)
    add_test(NAME test_string COMMAND test_string)

    target_include_directories(test_string
    PRIVATE
        ./
    )

    target_sources(test_string
    PRIVATE
        test_string.cpp
    )

    target_link_libraries(test_string
    PRIVATE
        etl
    )

    target_compile_options(test_string
    PRIVATE
        -pedantic
        -Wall
        -Wextra
    )

    add_executable(test_buffer)
    add_test(NAME test_buffer COMMAND test_buffer)

    target_include_directories(test_buffer
    PRIVATE
        ./
    )

    target_sources(test_buffer
    PRIVATE
        test_buffer.cpp
    )

    target_link_libraries(test_buffer
    PRIVATE
        etl
    )

    target_compile_options(test_buffer
    PRIVATE
        -pedantic
        -Wall
        -Wextra
    )

    add_executable(test_list)
    add_test(NAME test_list COMMAND test_list)

    target_include_directories(test_list
    PRIVATE
        ./
    )

    target_sources(test_list
    PRIVATE
        test_list.cpp
    )

    target_link_libraries(test_list
    PRIVATE
        etl
    )

    target_compile_options(test_list
    PRIVATE
        -pedantic
        -Wall
        -Wextra
    )

endif()
