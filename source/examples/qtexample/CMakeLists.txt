find_package(Qt5Core 5.1 )
find_package(Qt5Gui 5.1 )
find_package(Qt5Widgets 5.1 )

if(Qt5Core_DIR AND Qt5Gui_DIR AND Qt5Widgets_DIR)

set(target qtexample)
message(STATUS "Example ${target}")

set(CMAKE_AUTOMOC ON)

#
# External libraries
#

#
# Includes
#

include_directories(
    ${GLOBJECTS_EXAMPLE_DEPENDENCY_INCLUDES}
)

include_directories(
    BEFORE
    ${GLOBJECTS_EXAMPLE_INCLUDES}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

#
# Libraries
#

set(libs
    ${GLOBJECTS_EXAMPLES_LIBRARIES}
)

#
# Sources
#

set(headers
	QtOpenGLWindow.h
)
set(sources
    main.cpp
    QtOpenGLWindow.cpp
)

#
# Build executable
#

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

#
# Deployment
#

install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_EXAMPLES}
#   LIBRARY DESTINATION ${INSTALL_SHARED}
#   ARCHIVE DESTINATION ${INSTALL_LIB}
)

endif()
