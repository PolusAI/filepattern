cmake_minimum_required(VERSION 3.12)

set (CMAKE_CXX_STANDARD 17)

message(STATUS "Build tests")
#==== Compiler Options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(TEST_SRC 
        ../../src/filepattern/cpp/pattern.cpp
        ../../src/filepattern/cpp/interface/filepattern.cpp
        ../../src/filepattern/cpp/internal/internal_pattern.cpp
        ../../src/filepattern/cpp/internal/filepattern.cpp
        ../../src/filepattern/cpp/internal/stringpattern.cpp
        ../../src/filepattern/cpp/internal/vectorpattern.cpp
        ../../src/filepattern/cpp/external/external_pattern.cpp
        ../../src/filepattern/cpp/external/external_filepattern.cpp
        ../../src/filepattern/cpp/external/external_stringpattern.cpp
        ../../src/filepattern/cpp/external/external_vectorpattern.cpp
        ../../src/filepattern/cpp/util/fs_stream.cpp
        ../../src/filepattern/cpp/util/sort.cpp
        ../../src/filepattern/cpp/util/vector_parser.cpp
        test_main.cpp
)

add_executable(runAllTests ${TEST_SRC})


if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  list(APPEND runAllTests_LIBRARIES "stdc++fs" )
endif()

target_include_directories (runAllTests PUBLIC ${GTEST_INCLUDE_DIRS})
target_link_directories (runAllTests PUBLIC ${GTEST_LIBRARY_PATH})

target_link_libraries (runAllTests PUBLIC gtest ${runAllTests_LIBRARIES})