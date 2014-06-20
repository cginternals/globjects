find_package(OpenSceneGraph COMPONENTS osgViewer osgDB  osgGA osgUtil QUIET)

if(OPENSCENEGRAPH_FOUND)

set(target osgexample)
message(STATUS "Example ${target}")

#
# External libraries
#

#
# Includes
#

include_directories(
    ${GLOW_EXAMPLE_DEPENDENCY_INCLUDES}
)

include_directories(
    BEFORE
    ${GLOW_EXAMPLE_INCLUDES}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OPENSCENEGRAPH_INCLUDE_DIRS}
)

#
# Libraries
#

set(libs
    ${GLOW_EXAMPLES_LIBRARIES}
    ${OPENSCENEGRAPH_LIBRARIES}
)

#
# Sources
#

set(headers
    impl.h
)
set(sources
    main.cpp
    impl.cpp
)

#
# Build executable
#

add_executable(${target} ${headers} ${sources})

target_link_libraries(${target} ${libs})

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