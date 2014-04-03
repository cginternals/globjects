
# Target
set(target glowwindow)
message(STATUS "Lib ${target}")

#
# External libraries
#

find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLFW REQUIRED)

#
# Includes
#

include_directories(
    ${OPENGL_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
)

include_directories(
    BEFORE
    ${CMAKE_SOURCE_DIR}/source/glow/include
    ${CMAKE_SOURCE_DIR}/source/glowutils/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

#
# Libraries
#

set(libs
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARY}
    ${GLFW_LIBRARY}
    glow
    glowutils
)

#
# Compiler definitions
#

if (OPTION_BUILD_STATIC)
    add_definitions("-DGLOWWINDOW_STATIC")
else()
    add_definitions("-DGLOWWINDOW_EXPORTS")
endif()

if (OPTION_ERRORS_AS_EXCEPTION)
	add_definitions("-DGLOW_GL_ERROR_RAISE_EXCEPTION")
else()
    add_definitions("-D_HAS_EXCEPTIONS=0")
endif()

# for compatibility between glm 0.9.4 and 0.9.5
add_definitions("-DGLM_FORCE_RADIANS")

#
# Sources
#

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/source")

set(api_includes
    ${include_path}/Context.h
    ${include_path}/ContextFormat.h
    ${include_path}/glowwindow.h
    ${include_path}/events.h
    ${include_path}/MainLoop.h
    ${include_path}/Window.h
    ${include_path}/WindowEventHandler.h
)

set(sources
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

if(OPTION_BUILD_STATIC)
    add_library(${target} ${api_includes} ${sources})
else()
    add_library(${target} SHARED ${api_includes} ${sources})
endif()
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

#
# Deployment
#

# Library
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_BIN}
    LIBRARY DESTINATION ${INSTALL_SHARED}
    ARCHIVE DESTINATION ${INSTALL_LIB}
)

# Header files
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${target} DESTINATION ${INSTALL_INCLUDE})
