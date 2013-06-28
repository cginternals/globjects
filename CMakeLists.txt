cmake_minimum_required(VERSION 2.8.9 FATAL_ERROR)


# PROJECT CONFIG

set(META_PROJECT_NAME "glow")

set(META_VERSION_MAJOR "0")
set(META_VERSION_MINOR "0")

set(META_AUTHOR_ORGANIZATION "glow Group")
set(META_AUTHOR_DOMAIN "https://github.com/cgcostume/glow/")



option(OPTION_LIMIT_CONFIGS "Generate limited configs (Release; Debug)" ON)

option(OPTION_LOCAL_INSTALL "Install to local directory instead of system" OFF)

option(OPTION_BUILD_STATIC "Build static libraries" OFF)
option(OPTION_BUILD_EXAMPLES "Build examples" OFF)



if(OPTION_LIMIT_CONFIGS)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Limited Configs" FORCE)
endif()

set(PROJECT_NAME ${META_PROJECT_NAME})  
project(${PROJECT_NAME} C CXX)

# check if x64 and provide this information

set(X64 OFF)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(X64 ON)
endif()

# check for linux

if(UNIX AND NOT APPLE)
    set(LINUX 1)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_MODULE_PATH 
    ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


# RETRIEVE REPOSITORY VERSION AND GENERATE META HEADER

# add a revision file containing the git-head tag for cpack and install

include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_SHA1)

# generate a shorter, googlelike variation for rev

string(SUBSTRING ${GIT_SHA1} 0 12 GIT_REV)
file(WRITE ${CMAKE_BINARY_DIR}/revision ${GIT_REV})
install(FILES ${CMAKE_BINARY_DIR}/revision DESTINATION .)


# THIRD PARTIES

find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)
find_package(GLEW REQUIRED)


# SOURCES AND INCLUDES

set(GLOW_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(
    ${CMAKE_BINARY_DIR} # for Meta.h
    ${OPENGL_INCLUDE_DIR}
    ${GLM_INCLUDE_PATH}
    ${GLEW_INCLUDE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs)

set(GLOW_HEADER_GROUP "Header Files (API)")
set(GLOW_SOURCE_GROUP "Source Files")


# setup platform specifics (compile flags, etc., ...)

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
    # TODO configure for apple        
    message(WARNING "Unsupported platform/compiler combination")
endif()


include(cmake/Custom.cmake)


set(target glow)
message(STATUS "lib ${target}")

set(HEADER_PATH "${GLOW_INCLUDE_DIR}/${target}")
set(SOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/source")

configure_file(${SOURCE_PATH}/meta.h.in meta.h)

set(HEADERS
    ${HEADER_PATH}/declspec.h

    ${HEADER_PATH}/internal/FileReader.h
    ${HEADER_PATH}/internal/UniformSetter.hpp
    
    ${HEADER_PATH}/info.h
    
    ${HEADER_PATH}/Array.hpp
    ${HEADER_PATH}/ArrayData.h
    ${HEADER_PATH}/Buffer.h
    ${HEADER_PATH}/Changeable.h
    ${HEADER_PATH}/ChangeListener.h
    ${HEADER_PATH}/Error.h
    ${HEADER_PATH}/FrameBufferObject.h
    ${HEADER_PATH}/Log.h
    ${HEADER_PATH}/Object.h
    ${HEADER_PATH}/Program.h
    ${HEADER_PATH}/ref_ptr.hpp
    ${HEADER_PATH}/Referenced.h
    ${HEADER_PATH}/RenderBufferObject.h
    ${HEADER_PATH}/Shader.h
    ${HEADER_PATH}/ShaderFile.h
    ${HEADER_PATH}/ShaderSource.h
    ${HEADER_PATH}/Texture.h
    ${HEADER_PATH}/Uniform.h
    ${HEADER_PATH}/VertexArrayObject.h
    ${HEADER_PATH}/VertexAttributeBinding.h
    ${HEADER_PATH}/VertexBuffer.h
)

set(SOURCES

    ${SOURCE_PATH}/internal/FileReader.cpp
    ${SOURCE_PATH}/internal/UniformSetter.cpp
    ${SOURCE_PATH}/info.cpp
    
    ${SOURCE_PATH}/Buffer.cpp
    ${SOURCE_PATH}/Changeable.cpp
    ${SOURCE_PATH}/Error.cpp
    ${SOURCE_PATH}/FrameBufferObject.cpp
    ${SOURCE_PATH}/Log.cpp
    ${SOURCE_PATH}/Object.cpp
    ${SOURCE_PATH}/Program.cpp
    ${SOURCE_PATH}/Referenced.cpp
    ${SOURCE_PATH}/RenderBufferObject.cpp
    ${SOURCE_PATH}/Shader.cpp
    ${SOURCE_PATH}/ShaderFile.cpp
    ${SOURCE_PATH}/ShaderSource.cpp
    ${SOURCE_PATH}/Texture.cpp
    ${SOURCE_PATH}/Uniform.cpp
    ${SOURCE_PATH}/VertexArrayObject.cpp
    ${SOURCE_PATH}/VertexAttributeBinding.cpp
    ${SOURCE_PATH}/VertexBuffer.cpp
)

# group source files

source_group_by_path(${HEADER_PATH} "\\\\.h$|\\\\.hpp$" 
    ${GLOW_HEADER_GROUP} ${HEADERS})
source_group_by_path(${SOURCE_PATH} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${GLOW_SOURCE_GROUP} ${SOURCES})

# setup target

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR} ${GLOW_INCLUDE_DIR})

if(OPTION_BUILD_STATIC)
    add_library(${target} ${HEADERS} ${SOURCES})
else()
    add_library(${target} SHARED ${HEADERS} ${SOURCES})
endif()

target_link_libraries(${target}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARY}
)

set_target_properties(${target}
    PROPERTIES
    LINKER_LANGUAGE CXX
    COMPILE_DEFINITIONS_DEBUG   "${DEFAULT_COMPILE_DEFS_DEBUG}"
    COMPILE_DEFINITIONS_RELEASE "${DEFAULT_COMPILE_DEFS_RELEASE}"
    COMPILE_FLAGS               "${DEFAULT_COMPILE_FLAGS}"
    LINK_FLAGS_DEBUG            "${DEFAULT_LINKER_FLAGS_DEBUG}"
    LINK_FLAGS_RELEASE          "${DEFAULT_LINKER_FLAGS_RELEASE}"
    DEBUG_POSTFIX "d${DEBUG_POSTFIX}")

add_definitions("-DGLOW_EXPORTS")



if(OPTION_BUILD_EXAMPLES)
    add_subdirectory("examples")
endif()


# CPACK DEPLOY

set(CPACK_GENERATOR "ZIP")
set(CPACK_SOURCE_GENERATOR "ZIP")

set(CPACK_PACKAGE_VERSION_MAJOR ${META_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${META_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${GIT_REV})

set(CPACK_INSTALL_DIRECTORIES ${CMAKE_BINARY_DIR}/install)
set(CPACK_IGNORE_FILES "/.pdb/;/.ilk/;/.svn/;/.hg/;/.git/;.swp$;.#;/#")
set(CPACK_OUTPUT_FILE_PREFIX ${CMAKE_BINARY_DIR}/install)

include(CPack)
