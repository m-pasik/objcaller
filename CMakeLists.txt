cmake_minimum_required(VERSION 3.10)
project(objcaller VERSION 0.0.1 DESCRIPTION "CLI for calling functions from binary files.")
CMAKE_POLICY(SET CMP0072 NEW)

set(CMAKE_C_FLAGS_RELEASE "-Wall -O3")

set(CMAKE_C_FLAGS_DEBUG "-Wall -O0 -fsanitize=address -g -ggdb")

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/release)
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/debug)
endif()

file(GLOB_RECURSE SOURCES src/*.c)

include_directories(callobj ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(callobj ${SOURCES})

enable_testing()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testbin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testbin)

add_library(testing SHARED test/test.c)
include_directories(testing ${CMAKE_CURRENT_SOURCE_DIR}/test)
set_target_properties(testing PROPERTIES VERSION ${PROJECT_VERSION} PUBLIC_HEADER test/test.h)

include_directories(tester ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/test)
add_executable(tester test/main.c src/call.c src/loader.c test/test.c)

add_test(
    NAME test1
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testbin
    COMMAND tester test1
)
set_tests_properties(
    test1
    PROPERTIES
        PASS_REGULAR_EXPRESSION "^1 2 3 4 5 6\n"
        DESCRIPTION "Test passing integer arguments to integer registers."
)

add_test(
    NAME test2
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testbin
    COMMAND tester test2
)
set_tests_properties(
    test2
    PROPERTIES
        PASS_REGULAR_EXPRESSION "^1 2 3 4 5 6\n"
        DESCRIPTION "Test passing long long arguments to integer registers."
)

add_test(
    NAME test3
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testbin
    COMMAND tester test3
)
set_tests_properties(
    test3
    PROPERTIES
        PASS_REGULAR_EXPRESSION "^1 2 3\n"
        DESCRIPTION "Test passing __int128_t arguments to integer registers."
)

add_test(
    NAME test4
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testbin
    COMMAND tester test4
)
set_tests_properties(
    test4
    PROPERTIES
        PASS_REGULAR_EXPRESSION "^1 2 3 4 5 6 7 8 9 10\n"
        DESCRIPTION "Test passing integer arguments to stack."
)

add_test(
    NAME test5
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testbin
    COMMAND tester test5
)
set_tests_properties(
    test5
    PROPERTIES
        PASS_REGULAR_EXPRESSION "^1 2 3 4 5\n"
        DESCRIPTION "Test passing long long and __int128_t arguments to stack."
)

add_test(
    NAME test6
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testbin
    COMMAND tester test6
)
set_tests_properties(
    test6
    PROPERTIES
        PASS_REGULAR_EXPRESSION "^0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8\n"
        DESCRIPTION "Test passing double arguments to float registers."
)

add_test(
    NAME test7
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testbin
    COMMAND tester test7
)
set_tests_properties(
    test7
    PROPERTIES
        PASS_REGULAR_EXPRESSION "^0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0\n"
        DESCRIPTION "Test passing double arguments to stack."
)

add_test(
    NAME test8
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testbin
    COMMAND tester test8
)
set_tests_properties(
    test8
    PROPERTIES
        PASS_REGULAR_EXPRESSION "^0.1 0.2 0.3\n"
        DESCRIPTION "Test passing long double arguments to stack."
)

add_test(
    NAME test9
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testbin
    COMMAND tester test9
)
set_tests_properties(
    test9
    PROPERTIES
        PASS_REGULAR_EXPRESSION "^0.1 1 0.2 2 0.3 3 0.4 4 0.5 5 0.6 6 0.7 7 0.8 8 0.9 9 1.0\n"
        DESCRIPTION "Test passing both integer and double arguments to stack."
)

add_custom_target(tests)
add_dependencies(tests testing tester)
