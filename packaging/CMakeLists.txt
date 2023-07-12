include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

if (NOT DEFINED filepattern_INSTALL_CMAKEDIR)
   set(filepattern_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/filepattern"
       CACHE STRING "Path to filepattern CMake files")
endif ()

install(TARGETS filepattern EXPORT filepattern_Targets
        RUNTIME COMPONENT filepattern_Runtime
        LIBRARY COMPONENT filepattern_Runtime
        NAMELINK_COMPONENT filepattern_Development
        ARCHIVE COMPONENT filepattern_Development
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
        )

install(DIRECTORY "${filepattern_SOURCE_DIR}/src/filepattern/cpp/include/" "${filepattern_BINARY_DIR}/include/"
        TYPE INCLUDE
        COMPONENT filepattern_Development)

if (BUILD_SHARED_LIBS)
    set(type shared)
else ()
    set(type static)
endif ()

install(EXPORT filepattern_Targets
        DESTINATION "${filepattern_INSTALL_CMAKEDIR}"
        NAMESPACE filepattern::
        FILE filepattern-${type}-targets.cmake
        COMPONENT filepattern_Development)
        
write_basic_package_version_file(
    filepatternConfigVersion.cmake
    VERSION 2.0.0
    COMPATIBILITY SameMajorVersion)

install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/filepatternConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/filepatternConfigVersion.cmake"
    DESTINATION "${filepattern_INSTALL_CMAKEDIR}"
    COMPONENT filepattern_Development)