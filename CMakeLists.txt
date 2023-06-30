cmake_minimum_required(VERSION 3.2)
project(PolusFilepattern)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (filepattern_VERSION_MAJOR 1)
set (filepattern_VERSION_MINOR 0)
set (filepattern_VERSION_BUILD 3)
set(filepattern_VERSION "${filepattern_VERSION_MAJOR}.${filepattern_VERSION_MINOR}.${filepattern_VERSION_BUILD}")

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

# set fp headers
set(fp_HEADERS
	src/filepattern/cpp/interface/filepattern.h
)
#==== Source files
set(SOURCE  src/filepattern/cpp/pattern.cpp
			src/filepattern/cpp/interface/filepattern.cpp
			src/filepattern/cpp/internal/internal_pattern.cpp 
			src/filepattern/cpp/internal/filepattern.cpp
			src/filepattern/cpp/internal/stringpattern.cpp
			src/filepattern/cpp/internal/vectorpattern.cpp
			src/filepattern/cpp/external/external_pattern.cpp
			src/filepattern/cpp/external/external_filepattern.cpp
			src/filepattern/cpp/external/external_stringpattern.cpp
			src/filepattern/cpp/external/external_vectorpattern.cpp
			src/filepattern/cpp/util/fs_stream.cpp
			src/filepattern/cpp/util/sort.cpp
			src/filepattern/cpp/util/vector_parser.cpp
)

if(BUILD_CPP_LIB)
	add_library(filepattern SHARED ${SOURCE})
endif()



if(BUILD_PYTHON_LIB)
	add_subdirectory(lib/pybind11)
	pybind11_add_module(backend
						${SOURCE}
						src/filepattern/cpp/bindings.cpp
	)
	target_compile_definitions(backend PRIVATE WITH_PYTHON_H)
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)


add_executable(	test_fp
				${SOURCE}
				tests/cpp/main.cpp
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if(BUILD_PYTHON_LIB)
  	target_link_libraries(backend PRIVATE stdc++fs)
  endif()
  if(BUILD_CPP_LIB)
	target_link_libraries(filepattern PRIVATE stdc++fs)
endif()
  target_link_libraries(test_fp PRIVATE stdc++fs)
endif()


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