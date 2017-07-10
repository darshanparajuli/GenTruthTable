cmake_minimum_required(VERSION 3.7)

set(SOURCES ../src/includes/truthtable.h ../src/truthtable.cpp ../src/tokenizer.cpp ../src/includes/tokenizer.h ../src/includes/symbols.h)
set(GTEST_LIBRARY /usr/lib/libgtest.so /usr/lib/libgtest_main.so)

add_executable(GenTruthTableTest main.cpp ${SOURCES})
target_link_libraries(GenTruthTableTest ${OPTION_PARSER_LIBRARY} ${GTEST_LIBRARY})
