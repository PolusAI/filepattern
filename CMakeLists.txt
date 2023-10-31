cmake_minimum_required(VERSION 3.20)
project(filepattern)

# this is a workaround for GitHub Action for wheelbuiling
if(DEFINED ENV{FILEPATTERN_DEP_DIR})
	set(CMAKE_PREFIX_PATH $ENV{FILEPATTERN_DEP_DIR})
	link_directories($ENV{FILEPATTERN_DEP_DIR}/lib)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(RUN_GTEST "Downloads google unit test API and runs google test scripts to test Filepattern" OFF)


if(BUILD_PYTHON_LIB)
	find_package(pybind11 CONFIG REQUIRED)
	pybind11_add_module(backend
						src/filepattern/cpp/bindings.cpp
	)
	find_package(filepattern QUIET)
	if (NOT filepattern_FOUND)
		message(STATUS "libfilepattern not found. It will be build from source.")
		set(filepattern_SHARED_LIB ON)
		add_subdirectory(src/filepattern/cpp ${CMAKE_BINARY_DIR}/third-party)
		add_dependencies(backend filepattern)
		target_include_directories(backend PRIVATE ${CMAKE_BINARY_DIR}/third-party/include)
	else()
		target_compile_definitions(backend PRIVATE WITH_LIBFILEPATTERN)
	endif()
	target_link_libraries(backend PRIVATE filepattern::filepattern)	
	if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		target_link_libraries(backend PRIVATE stdc++fs)
	  endif()

endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

## Running Tests
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
	add_subdirectory(tests/cpp/)
	message(STATUS "GTEST downloaded and imported")
endif (RUN_GTEST)
