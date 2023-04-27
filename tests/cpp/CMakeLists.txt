cmake_minimum_required(VERSION 3.2)

if(APPLE)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")
#  set(CMAKE_CXX_COMPILER "/usr/local/bin/g++-8")
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # require at least gcc 8.0
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8.0)
      message(FATAL_ERROR "GCC version must be at least 8.0!")
  endif()
endif()

project(test_filepattern)
# Set source directory
set(SOURCE_DIR "../../src/filepattern/cpp")
#Tell CMake that headers are also in SOURCE_DIR
include_directories(${SOURCE_DIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(test "${SOURCE_DIR}/pattern.cpp"
        "${SOURCE_DIR}/interface/filepattern.cpp" 
        "${SOURCE_DIR}/internal/internal_pattern.cpp" 
        "${SOURCE_DIR}/internal/filepattern.cpp"
        "${SOURCE_DIR}/internal/stringpattern.cpp"
        "${SOURCE_DIR}/internal/vectorpattern.cpp"
        "${SOURCE_DIR}/external/external_pattern.cpp"
        "${SOURCE_DIR}/external/external_filepattern.cpp"
        "${SOURCE_DIR}/external/external_stringpattern.cpp"
        "${SOURCE_DIR}/external/external_vectorpattern.cpp"
        "${SOURCE_DIR}/util/fs_stream.cpp"
        "${SOURCE_DIR}/util/sort.cpp"
        "${SOURCE_DIR}/util/vector_parser.cpp"
        "test_main.cpp"
    )

add_executable(runAllTests ${test})

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_link_libraries(runAllTests PRIVATE stdc++fs)
endif()

target_include_directories (runAllTests PUBLIC ${GTEST_INCLUDE_DIRS})
target_link_directories (runAllTests PUBLIC ${GTEST_LIBRARY_PATH})

target_link_libraries (runAllTests PUBLIC gtest ${runAllTests_LIBRARIES})