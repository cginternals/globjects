
set(target ${EXAMPLE_COMMON})
message(STATUS "Helper ${target}")

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

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}")

set(api_includes
    ${include_path}/AbstractCoordinateProvider.h
    ${include_path}/AxisAlignedBoundingBox.h
    ${include_path}/Camera.h
    ${include_path}/ExampleWindowEventHandler.h
    ${include_path}/Icosahedron.h
    ${include_path}/navigationmath.h
    ${include_path}/ScreenAlignedQuad.h
    ${include_path}/StringSourceDecorator.h
    ${include_path}/StringTemplate.h
    ${include_path}/Timer.h
    ${include_path}/VertexDrawable.h
    ${include_path}/WorldInHandNavigation.h
)

set(sources
    ${source_path}/AbstractCoordinateProvider.cpp
    ${source_path}/AxisAlignedBoundingBox.cpp
    ${source_path}/Camera.cpp
    ${source_path}/ExampleWindowEventHandler.cpp
    ${source_path}/Icosahedron.cpp
    ${source_path}/navigationmath.cpp
    ${source_path}/ScreenAlignedQuad.cpp
    ${source_path}/StringSourceDecorator.cpp
    ${source_path}/StringTemplate.cpp
    ${source_path}/Timer.cpp
    ${source_path}/VertexDrawable.cpp
    ${source_path}/WorldInHandNavigation.cpp
)

# Group source files
set(header_group "Header Files (API)")
set(source_group "Source Files")
source_group_by_path(${include_path} "\\\\.h$|\\\\.hpp$" 
    ${header_group} ${api_includes})
source_group_by_path(${source_path} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${source_group} ${sources})

#
# Build library
#

add_library(${target} STATIC ${api_includes} ${sources})
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
    DEBUG_POSTFIX               "d${DEBUG_POSTFIX}"
    INCLUDE_PATH	            ${include_path})
