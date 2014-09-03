
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

# for compatibility between glm 0.9.4 and 0.9.5
add_definitions("-DGLM_FORCE_RADIANS")

#
# Sources
#

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}")

set(api_includes
    ${include_path}/ExampleWindowEventHandler.h
    ${include_path}/Context.h
    ${include_path}/ContextFormat.h
    ${include_path}/events.h
    ${include_path}/keys.h
    ${include_path}/MainLoop.h
    ${include_path}/Window.h
    ${include_path}/WindowEventHandler.h
)

set(sources
    ${source_path}/ExampleWindowEventHandler.cpp
    ${source_path}/Context.cpp
    ${source_path}/ContextFormat.cpp
    ${source_path}/events.cpp
    ${source_path}/MainLoop.cpp
    ${source_path}/Window.cpp
    ${source_path}/WindowEventDispatcher.h
    ${source_path}/WindowEventDispatcher.cpp
    ${source_path}/WindowEventHandler.cpp
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
