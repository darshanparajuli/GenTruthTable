cmake_minimum_required(VERSION 3.7)

set(SOURCE_FILES main.cpp includes/truthtable.h truthtable.cpp tokenizer.cpp includes/tokenizer.h includes/symbols.h)
add_executable(GenTruthTable ${SOURCE_FILES})
target_link_libraries(GenTruthTable /usr/local/lib/libOptionParser.a)

if (${CMAKE_BUILD_TYPE} MATCHES Release)
    install(TARGETS GenTruthTable DESTINATION bin)
endif ()
