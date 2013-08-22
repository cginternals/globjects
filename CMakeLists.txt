cmake_minimum_required(VERSION 2.8.9 FATAL_ERROR)


# PROJECT DESCRIPTION

set(META_PROJECT_NAME        "glow")
set(META_VERSION_MAJOR       "0")
set(META_VERSION_MINOR       "1")
set(META_VERSION_PATCH       "0")
set(META_VERSION             "${META_VERSION_MAJOR}.${META_VERSION_MINOR}.${META_VERSION_PATCH}")
set(META_AUTHOR_ORGANIZATION "glow group")
set(META_AUTHOR_DOMAIN       "https://github.com/hpicgs/glow/")


# OPTIONS

option(OPTION_LIMIT_CONFIGS  "Generate limited configs (Release; Debug)" ON)
option(OPTION_LOCAL_INSTALL  "Install to a local directory instead of the system" OFF)
option(OPTION_BUILD_STATIC   "Build static libraries" OFF)
option(OPTION_BUILD_EXAMPLES "Build examples" ON)
option(OPTION_ERRORS_AS_EXCEPTION "Throw exceptions" OFF)

# CMAKE CONFIGURATION

set(GLOW_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Include cmake modules from ./cmake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Set configuration types
if(OPTION_LIMIT_CONFIGS)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Limited Configs" FORCE)
endif()

# Project
project(${META_PROJECT_NAME} C CXX)

# Generate folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Include custom cmake functions
include(cmake/Custom.cmake)


# PLATFORM AND ARCHITECTURE

# Architecture (32/64 bit)
set(X64 OFF)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(X64 ON)
endif()

# Check for linux
if(UNIX AND NOT APPLE)
    set(LINUX 1)
endif()

# Setup platform specifics (compile flags, etc., ...)
if(MSVC)
    message(STATUS "Configuring for platform Windows/MSVC.")
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PlatformWindowsMSVC.cmake)
elseif(WIN32 AND CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "Configuring for platform Windows/GCC.")
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PlatformWindowsGCC.cmake)
elseif(LINUX AND CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "Configuring for platform Linux/GCC.")
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PlatformLinuxGCC.cmake)
elseif(APPLE)
    message(STATUS "Configuring for platform MacOS.")
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PlatformMacOS.cmake)
else()
    # Unsupported system/compiler
    message(WARNING "Unsupported platform/compiler combination")
endif()


# Install locations

set(project ${META_PROJECT_NAME})
if(WIN32)
    set(INSTALL_BIN           ".")                      # C:\Programme\<project>
    set(INSTALL_SHARED        ".")                      # C:\Programme\<project>
    set(INSTALL_LIB           "lib")                    # C:\Programme\<project>\lib
    set(INSTALL_INCLUDE       "include")                # C:\Programme\<project>\include
    set(INSTALL_DOC           "doc")                    # C:\Programme\<project>\doc
    set(INSTALL_APPLICATIONS  "")                       # Not available under Windows
    set(INSTALL_ICONS         "")                       # Not available under Windows
    set(INSTALL_INIT          "")                       # Not available under Windows
    set(INSTALL_DATA          ".")                      # C:\Programme\<project>
    set(INSTALL_EXAMPLES      ".")                      # C:\Programme\<project>
elseif(LINUX)
    set(INSTALL_BIN           "bin")                    # /usr/[local]/bin
    set(INSTALL_SHARED        "lib")                    # /usr/[local]/lib
    set(INSTALL_LIB           "lib")                    # /usr/[local]/lib
    set(INSTALL_INCLUDE       "include")                # /usr/[local]/include
    set(INSTALL_DOC           "share/doc/${project}")   # /usr/[local]/share/doc/<project>
    set(INSTALL_APPLICATIONS  "share/applications")     # /usr/[local]/share/applications
    set(INSTALL_ICONS         "share/pixmaps")          # /usr/[local]/share/pixmaps
    set(INSTALL_INIT          "/etc/init")              # /etc/init (upstart init scripts)
    set(INSTALL_DATA          "share/${project}")       # /usr/[local]/share/<project>
    set(INSTALL_EXAMPLES      "share/${project}")       # /usr/[local]/share/<project>

    # Adjust target paths for Local installs
    if(OPTION_LOCAL_INSTALL)
        # Put binaries in root directory and keep data directory name
        set(INSTALL_BIN                       ".")           # /<INSTALL_PREFIX>
        set(INSTALL_DATA                      ".")           # /<INSTALL_PREFIX>

        # We have to change the RPATH of binaries to achieve a usable local install.
        # [TODO] For binaries, "$ORIGIN/lib" is right, so that libraries are found in ./lib.
        #        However, I have not yet tested what happens when libraries use other libraries.
        #        In that case, they might need the rpath $ORIGIN instead ...
        set(CMAKE_SKIP_BUILD_RPATH            FALSE)         # Use automatic rpath for build
        set(CMAKE_BUILD_WITH_INSTALL_RPATH    FALSE)         # Use specific rpath for INSTALL
        set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)         # NO automatic rpath for INSTALL
        set(CMAKE_INSTALL_RPATH               "$ORIGIN/lib") # Libraries are relative to binary
    endif()
endif()



# GET REPOSITORY VERSION

# Add a revision file containing the git-head tag for cpack and install
include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_SHA1)

# Generate a shorter, googlelike variation for rev
string(SUBSTRING ${GIT_SHA1} 0 12 GIT_REV)
file(WRITE ${CMAKE_BINARY_DIR}/revision ${GIT_REV})

# [TODO] Really?? Do we want this deployed?
#install(FILES ${CMAKE_BINARY_DIR}/revision DESTINATION .)

# [TODO] Deal with GIT-NOTFOUND


# GLOWMETA_VERSION LIBRARY


# Target
set(target glow)
message(STATUS "lib ${target}")

# Find externals
find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)
find_package(GLEW REQUIRED)

# Include directories
include_directories(
    ${CMAKE_BINARY_DIR} # for Meta.h
    ${OPENGL_INCLUDE_DIR}
    ${GLM_INCLUDE_PATH}
    ${GLEW_INCLUDE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs)

# Libraries
set(libs
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARY}
)

# Definitions
add_definitions("-DGLOW_EXPORTS")

if (OPTION_ERRORS_AS_EXCEPTION)
	add_definitions("-DGLOW_GL_ERROR_RAISE_EXCEPTION")
else()
    add_definitions("-D_HAS_EXCEPTIONS=0")
endif()

# Sources
set(header_path "${CMAKE_CURRENT_SOURCE_DIR}/include/glow")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/source")

# Generate meta-header
configure_file(${source_path}/meta.h.in meta.h)

set(api_includes
    ${header_path}/glow.h

    ${header_path}/AbstractLogHandler.h
    ${header_path}/AbstractUniform.h
    ${header_path}/AbstractArray.h
    ${header_path}/Array.h
    ${header_path}/Array.hpp
    ${header_path}/AutoTimer.h
    ${header_path}/Buffer.h
    ${header_path}/Changeable.h
    ${header_path}/ChangeListener.h
    ${header_path}/ConsoleLogger.h
    ${header_path}/Context.h
    ${header_path}/ContextFormat.h
    ${header_path}/Error.h
    ${header_path}/FrameBufferObject.h
    ${header_path}/formatString.h
    ${header_path}/formatString.hpp
    ${header_path}/logging.h
    ${header_path}/logging.hpp
    ${header_path}/LogMessage.h
    ${header_path}/KeyEvent.h
    ${header_path}/Object.h
    ${header_path}/Program.h
    ${header_path}/Program.hpp
    ${header_path}/query.h
    ${header_path}/RawFile.h
    ${header_path}/RawFile.hpp
    ${header_path}/ref_ptr.hpp
    ${header_path}/Referenced.h
    ${header_path}/RenderBufferObject.h
    ${header_path}/Screen.h
    ${header_path}/Shader.h
    ${header_path}/ShaderCode.h
    ${header_path}/ShaderFile.h
    ${header_path}/ShaderSource.h
    ${header_path}/Texture.h
    ${header_path}/Timer.h
    ${header_path}/TransformFeedback.h
    ${header_path}/Uniform.h
    ${header_path}/Uniform.hpp
    ${header_path}/Version.h
    ${header_path}/VertexArrayObject.h
    ${header_path}/VertexAttributeBinding.h
    ${header_path}/Window.h
    ${header_path}/WindowEventHandler.h
)

set(sources
    ${source_path}/AbstractUniform.cpp
    ${source_path}/AbstractNativeContext.cpp
    ${source_path}/AbstractNativeContext.h
    ${source_path}/AbstractNativeWindow.cpp
    ${source_path}/AbstractNativeWindow.h
    ${source_path}/AutoTimer.cpp
    ${source_path}/Buffer.cpp
    ${source_path}/Changeable.cpp
    ${source_path}/ChronoTimer.cpp
    ${source_path}/ChronoTimer.h
    ${source_path}/ConsoleLogger.cpp
    ${source_path}/Context.cpp
    ${source_path}/ContextFormat.cpp
    ${source_path}/Error.cpp
    ${source_path}/FrameBufferObject.cpp
    ${source_path}/formatString.cpp
    ${source_path}/GLxContext.cpp
    ${source_path}/GLxContext.h
    ${source_path}/KeyEvent.cpp
    ${source_path}/logging.cpp
    ${source_path}/LogMessage.cpp
    ${source_path}/Object.cpp
    ${source_path}/Program.cpp
    ${source_path}/query.cpp
    ${source_path}/RawFile.cpp
    ${source_path}/Referenced.cpp
    ${source_path}/RenderBufferObject.cpp
    ${source_path}/Screen.cpp
    ${source_path}/Shader.cpp
    ${source_path}/ShaderCode.cpp
    ${source_path}/ShaderFile.cpp
    ${source_path}/Texture.cpp
    ${source_path}/Timer.cpp
    ${source_path}/TransformFeedback.cpp
    ${source_path}/Uniform.cpp
    ${source_path}/Version.cpp
    ${source_path}/VertexArrayObject.cpp
    ${source_path}/VertexAttributeBinding.cpp
    ${source_path}/Window.cpp
    ${source_path}/WGLContext.cpp
    ${source_path}/WGLContext.h
    ${source_path}/WindowsWindow.cpp
    ${source_path}/WindowsWindow.h
    ${source_path}/WindowEventHandler.cpp
    ${source_path}/X11Window.cpp
    ${source_path}/X11Window.h
)

# Group source files
set(glow_header_group "Header Files (API)")
set(glow_source_group "Source Files")
source_group_by_path(${header_path} "\\\\.h$|\\\\.hpp$" 
    ${glow_header_group} ${api_includes})
source_group_by_path(${source_path} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${glow_source_group} ${sources})

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
    INCLUDE_PATH	            "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Deployment
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_BIN}
    LIBRARY DESTINATION ${INSTALL_SHARED}
    ARCHIVE DESTINATION ${INSTALL_LIB}
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION ${INSTALL_INCLUDE})


# EXAMPLES

set(GLOW_EXAMPLES_FOLDER "Examples")

unset(sources)
unset(api_includes)

if(OPTION_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()


# DEPLOYMENT

# Data files
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/data
    DESTINATION ${INSTALL_DATA}
)

# Packaging
add_subdirectory(packages)
