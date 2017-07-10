cmake_minimum_required(VERSION 3.7)

set(SOURCES main.cpp includes/truthtable.h truthtable.cpp tokenizer.cpp includes/tokenizer.h includes/symbols.h)
add_executable(GenTruthTable ${SOURCES})
target_link_libraries(GenTruthTable ${OPTION_PARSER_LIBRARY})

if (${CMAKE_BUILD_TYPE} MATCHES Release)
    install(TARGETS GenTruthTable DESTINATION bin)
endif ()
