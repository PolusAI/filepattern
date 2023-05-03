cmake_minimum_required(VERSION 3.2)

if(APPLE)

	# try to find gcc for github workflows
	find_program(GCC_EXECUTABLE gcc)
	if(GCC_EXECUTABLE)
	set(CMAKE_C_COMPILER "${GCC_EXECUTABLE}")
		message(STATUS "Using GCC: ${GCC_EXECUTABLE}")
	else()
		message(STATUS "GCC not found, falling back to Clang")
	endif()

	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # require at least gcc 8.0
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8.0)
      message(FATAL_ERROR "GCC version must be at least 8.0!")
  endif()
endif()

project(filepattern)
# Set source directory
set(SOURCE_DIR "src/filepattern/cpp")
#Tell CMake that headers are also in SOURCE_DIR
include_directories(${SOURCE_DIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(lib/pybind11)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

pybind11_add_module(backend MODULE ${SOURCES} "${SOURCE_DIR}/bindings.cpp" 
                                              "${SOURCE_DIR}/pattern.cpp"
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
                                            )

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_link_libraries(backend PRIVATE stdc++fs)
endif()

if(BUILD_LIB)
	target_compile_definitions(backend PRIVATE WITH_PYTHON_H)
endif()

option(RUN_GTEST "Downloads google unit test API and runs google test scripts to test Nyxus" OFF)
set(RUN_GTEST ON)

if (RUN_GTEST)
	# Download and unpack googletest at configure time
	configure_file(tests/cpp/CMakeLists.txt.gtest googletest-download/CMakeLists.txt)
	execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
			WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)
	execute_process(COMMAND ${CMAKE_COMMAND} --build .
			WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)

	# Prevent GoogleTest from overriding our compiler/linker options
	# when building with Visual Studio
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

	# Add googletest directly to our build. This adds
	# the following targets: gtest, gtest_main, gmock
	# and gmock_main
	add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
			${CMAKE_BINARY_DIR}/googletest-build)

	# The gtest/gmock targets carry header search path
	# dependencies automatically when using CMake 2.8.11 or
	# later. Otherwise we have to add them here ourselves.
	if (CMAKE_VERSION VERSION_LESS 2.8.11)
		include_directories("${gtest_SOURCE_DIR}/include")
	endif ()
	#	enable_testing()
	add_subdirectory(tests/cpp)
	message(STATUS "GTEST downloaded and imported")
endif (RUN_GTEST)