cmake_minimum_required(VERSION 3.20)
project(filepattern)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (filepattern_VERSION_MAJOR 2)
set (filepattern_VERSION_MINOR 0)
set (filepattern_VERSION_BUILD 0)
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
if (NOT BUILD_PYTHON_LIB) # Not taking the setup.py route, just building libs
	if (NOT MSVC)
		if (NOT DEFINED CMAKE_CXX_VISIBILITY_PRESET AND
			NOT DEFINED CMAKE_VISIBILITY_INLINES_HIDDEN)
		set(CMAKE_CXX_VISIBILITY_PRESET hidden)
		set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)
		endif ()
	endif()


	if(DEFINED filepattern_SHARED_LIB)
		set(BUILD_SHARED_LIBS ${filepattern_SHARED_LIB})
	endif()
	add_library(filepattern ${SOURCE})
	add_library(filepattern::filepattern ALIAS filepattern)
	set_target_properties(filepattern PROPERTIES
						VERSION ${filepattern_VERSION}
						SOVERSION ${filepattern_VERSION_MAJOR})
	target_include_directories(
		filepattern PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>")
	include(GenerateExportHeader)
	generate_export_header(filepattern EXPORT_FILE_NAME include/filepattern/filepattern_export.h)
	target_compile_definitions(
		filepattern PUBLIC "$<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:FILEPATTERN_STATIC_DEFINE>")
	target_compile_definitions(filepattern PUBLIC FP_CPP_LIB_EXPORT)
	target_include_directories(
		filepattern PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>")
	target_link_libraries(filepattern PRIVATE stdc++fs)

	string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)
	option(filepattern_INCLUDE_PACKAGING "Include packaging rules for FilePattern" "${is_top_level}")
	if (filepattern_INCLUDE_PACKAGING)
		add_subdirectory(packaging)
	endif ()
endif()



if(BUILD_PYTHON_LIB)
	add_subdirectory(lib/pybind11)
	pybind11_add_module(backend
						${SOURCE}
						src/filepattern/cpp/bindings.cpp
	)
	target_compile_definitions(backend PRIVATE WITH_PYTHON_H)
	if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		target_link_libraries(backend PRIVATE stdc++fs)
	  endif()
	  
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)



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
