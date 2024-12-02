cmake_minimum_required(VERSION 3.20)
project(libfilepattern)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(DEFINED ENV{FILEPATTERN_DEP_DIR})
	set(CMAKE_PREFIX_PATH $ENV{FILEPATTERN_DEP_DIR})
	link_directories($ENV{FILEPATTERN_DEP_DIR}/lib)
endif()

file(READ version.h VER_FILE )
string(REGEX MATCH "#define PROJECT_VER \"([0-9]+)\.([0-9]+)\.([0-9+])\"" _ "${VER_FILE}")
set (filepattern_VERSION_MAJOR ${CMAKE_MATCH_1})
set (filepattern_VERSION_MINOR ${CMAKE_MATCH_2})
set (filepattern_VERSION_PATCH ${CMAKE_MATCH_3})
set(filepattern_VERSION "${filepattern_VERSION_MAJOR}.${filepattern_VERSION_MINOR}.${filepattern_VERSION_PATCH}")
message(STATUS "Building libfilepattern ${filepattern_VERSION}" )

if(JAVA_BINDING)
	add_compile_definitions(JAVA_BINDING)
endif()

#==== Source files
set(SOURCE  pattern.cpp
			interface/filepattern.cpp
			internal/internal_pattern.cpp
			internal/arraypattern.cpp
			internal/filepattern.cpp
			internal/stringpattern.cpp
			internal/vectorpattern.cpp
			external/external_pattern.cpp
			external/external_filepattern.cpp
			external/external_stringpattern.cpp
			external/external_vectorpattern.cpp
			util/fs_stream.cpp
			util/sort.cpp
			util/vector_parser.cpp
)

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
generate_export_header(filepattern EXPORT_FILE_NAME include/filepattern_export.h)
target_compile_definitions(
	filepattern PUBLIC "$<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:FILEPATTERN_STATIC_DEFINE>")
target_compile_definitions(filepattern PUBLIC FP_CPP_LIB_EXPORT)
target_include_directories(
	filepattern PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>")
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_link_libraries(filepattern PRIVATE stdc++fs)
endif()

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)
option(filepattern_INCLUDE_PACKAGING "Include packaging rules for FilePattern" "${is_top_level}")
if (filepattern_INCLUDE_PACKAGING)
	add_subdirectory(packaging)
endif ()


set(CMAKE_POSITION_INDEPENDENT_CODE ON)

