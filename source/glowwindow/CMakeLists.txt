
# Target
set(target glowwindow)
message(STATUS "lib ${target}")

# Libraries
set(libs
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARY}
    ${EXTRA_LIBS}
    glow
)

# Definitions
add_definitions("-DGLOWWINDOW_EXPORTS")

if (OPTION_ERRORS_AS_EXCEPTION)
	add_definitions("-DGLOW_GL_ERROR_RAISE_EXCEPTION")
else()
    add_definitions("-D_HAS_EXCEPTIONS=0")
endif()

# Sources
set(header_path "${GLOW_SOURCE_DIR}/include/glowwindow")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}")

set(api_includes
    ${header_path}/Context.h
    ${header_path}/ContextFormat.h
    ${header_path}/glowwindow.h
    ${header_path}/KeyEvent.h
    ${header_path}/Window.h
    ${header_path}/WindowEventHandler.h
)

set(sources
    ${source_path}/AbstractNativeContext.cpp
    ${source_path}/AbstractNativeContext.h
    ${source_path}/AbstractNativeWindow.cpp
    ${source_path}/AbstractNativeWindow.h
    ${source_path}/Context.cpp
    ${source_path}/ContextFormat.cpp
    ${source_path}/GLxContext.cpp
    ${source_path}/GLxContext.h
    ${source_path}/KeyEvent.cpp
    ${source_path}/Window.cpp
    ${source_path}/WindowEventHandler.cpp
    ${source_path}/WindowsWindow.cpp
    ${source_path}/WindowsWindow.h
    ${source_path}/X11Window.cpp
    ${source_path}/X11Window.h
    ${source_path}/WGLContext.cpp
    ${source_path}/WGLContext.h
)

# Group source files
set(glowwindow_header_group "Header Files (API)")
set(glowwindow_source_group "Source Files")
source_group_by_path(${header_path} "\\\\.h$|\\\\.hpp$" 
    ${glowwindow_header_group} ${api_includes})
source_group_by_path(${source_path} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${glowwindow_source_group} ${sources})

# Build library
if(OPTION_BUILD_STATIC)
    add_library(${target} ${api_includes} ${sources})
else()
    add_library(${target} SHARED ${api_includes} ${sources})
endif()
target_link_libraries(${target} ${libs})
set_target_properties(${target}
    PROPERTIES
    LINKER_LANGUAGE CXX
    COMPILE_DEFINITIONS_DEBUG   "${DEFAULT_COMPILE_DEFS_DEBUG}"
    COMPILE_DEFINITIONS_RELEASE "${DEFAULT_COMPILE_DEFS_RELEASE}"
    COMPILE_FLAGS               "${DEFAULT_COMPILE_FLAGS}"
    LINK_FLAGS_DEBUG            "${DEFAULT_LINKER_FLAGS_DEBUG}"
    LINK_FLAGS_RELEASE          "${DEFAULT_LINKER_FLAGS_RELEASE}"
    DEBUG_POSTFIX               "d${DEBUG_POSTFIX}"
    INCLUDE_PATH	            "${GLOW_SOURCE_DIR}/include")

# Deployment
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_BIN}
    LIBRARY DESTINATION ${INSTALL_SHARED}
    ARCHIVE DESTINATION ${INSTALL_LIB}
)
