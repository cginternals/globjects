
set(target osg-example)

find_package(OpenSceneGraph COMPONENTS osgViewer osgDB  osgGA osgUtil QUIET)

if (NOT OPENSCENEGRAPH_FOUND)
    message(STATUS "Example ${target} (disabled - OSG not found)")
    return()
endif()

message(STATUS "Example ${target}")

# External libraries

# Includes

include_directories(
    ${GLOBJECTS_EXAMPLE_DEPENDENCY_INCLUDES}
    ${OPENSCENEGRAPH_INCLUDE_DIRS}
)

include_directories(
    BEFORE
    ${GLOBJECTS_EXAMPLE_INCLUDES}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Libraries

set(libs
    ${GLOBJECTS_EXAMPLES_LIBRARIES}
    ${OPENSCENEGRAPH_LIBRARIES}
)

# Sources

set(headers
    impl.h
)
set(sources
    main.cpp
    impl.cpp
)

# Build executable

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

# Deployment

# Do NOT install this example 

#install(TARGETS ${target} COMPONENT examples
#    RUNTIME DESTINATION ${INSTALL_EXAMPLES}
#   LIBRARY DESTINATION ${INSTALL_SHARED}
#   ARCHIVE DESTINATION ${INSTALL_LIB}
#)
