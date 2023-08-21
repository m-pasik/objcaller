cmake_minimum_required(VERSION 3.10)
project(objcaller VERSION 0.0.1 DESCRIPTION "CLI for calling functions from binary files.")
CMAKE_POLICY(SET CMP0072 NEW)

set(CMAKE_C_FLAGS_RELEASE "-Wall -O3 -g -ggdb")

set(CMAKE_C_FLAGS_DEBUG "-Wall -O0 -fsanitize=address -g -ggdb")

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/release)
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/debug)
endif()

file(GLOB_RECURSE SOURCES src/main.c)

file(GLOB_RECURSE LIB_SOURCES src/test.c)

add_library(test SHARED ${LIB_SOURCES})
include_directories(test ${CMAKE_CURRENT_SOURCE_DIR}/include)
set_target_properties(test PROPERTIES VERSION ${PROJECT_VERSION} PUBLIC_HEADER include/test.h)

include_directories(callobj ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(callobj ${SOURCES})


