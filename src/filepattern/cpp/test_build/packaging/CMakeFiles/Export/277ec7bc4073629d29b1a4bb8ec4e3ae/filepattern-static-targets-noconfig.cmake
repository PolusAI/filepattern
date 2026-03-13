#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "filepattern::filepattern" for configuration ""
set_property(TARGET filepattern::filepattern APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(filepattern::filepattern PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libfilepattern.a"
  )

list(APPEND _cmake_import_check_targets filepattern::filepattern )
list(APPEND _cmake_import_check_files_for_filepattern::filepattern "${_IMPORT_PREFIX}/lib/libfilepattern.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
