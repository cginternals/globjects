
set(target qt-example)

# Qt5

# good resource: http://www.kdab.com/using-cmake-with-qt-5/

# http://qt-project.org/forums/viewthread/30006/
if(MSVC)
    cmake_policy(SET CMP0020 NEW)
endif()

# External libraries

find_package(Qt5Core    5.1 QUIET)
find_package(Qt5Gui     5.1 QUIET)
find_package(Qt5Widgets 5.1 QUIET)

if (NOT Qt5Core_FOUND)
    message("Example ${target} skipped: Qt5 not found")
    return()
endif()

message(STATUS "Example ${target}")


set(CMAKE_AUTOMOC ON)
set(AUTOMOC_MOC_OPTIONS PROPERTIES FOLDER CMakeAutomocTargets)

# Probably works in the next cmake release -> http://www.cmake.org/Bug/view.php?id=13788
# What we do not want is automocs beside the project -> http://www.cmake.org/Bug/view.php?id=13688
set_property(GLOBAL PROPERTY AUTOMOC_FOLDER CMakeAutomocTargets)


# Includes

include_directories(
    ${GLOBJECTS_EXAMPLE_DEPENDENCY_INCLUDES}
)

include_directories(
    BEFORE
    ${GLOBJECTS_EXAMPLE_INCLUDES}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Libraries

set(libs
    ${GLOBJECTS_EXAMPLES_LIBRARIES}
)

# Sources

set(headers
    WindowQt.h
)
set(sources
    main.cpp
    WindowQt.cpp
)

# Build executable

add_executable(${target} ${headers} ${sources})

target_link_libraries(${target} ${libs})
qt5_use_modules(${target} Core Gui Widgets)

set_target_properties(${target}
    PROPERTIES
    LINKER_LANGUAGE              CXX
    FOLDER                      "${IDE_FOLDER}"
    COMPILE_DEFINITIONS_DEBUG   "${DEFAULT_COMPILE_DEFS_DEBUG}"
    COMPILE_DEFINITIONS_RELEASE "${DEFAULT_COMPILE_DEFS_RELEASE}"
    COMPILE_FLAGS               "${DEFAULT_COMPILE_FLAGS}"
    LINK_FLAGS_DEBUG            "${DEFAULT_LINKER_FLAGS_DEBUG}"
    LINK_FLAGS_RELEASE          "${DEFAULT_LINKER_FLAGS_RELEASE}"
    DEBUG_POSTFIX               "d${DEBUG_POSTFIX}")

# Deployment

# Do NOT install this example 

#install(TARGETS ${target} COMPONENT examples
#    RUNTIME DESTINATION ${INSTALL_EXAMPLES}
#   LIBRARY DESTINATION ${INSTALL_SHARED}
#   ARCHIVE DESTINATION ${INSTALL_LIB}
#)

# deploy qt binaries (glraw debug target should not be deployed)
#if(WIN32)
#    install_qt(examples ${INSTALL_EXAMPLES} Core Gui Widgets OpenGL)
#    install_qt_platforms(examples ${INSTALL_EXAMPLES} qwindows)
#endif()
