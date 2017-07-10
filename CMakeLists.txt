cmake_minimum_required(VERSION 3.7)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

project(GenTruthTable)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g -pedantic -O0 -Wno-gnu")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O2 -DNDEBUG")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

set(OPTION_PARSER_LIBRARY /usr/local/lib/libOptionParser.a)

add_subdirectory(src)

if (${CMAKE_BUILD_TYPE} MATCHES Debug)
    add_subdirectory(unit_test)
endif ()
