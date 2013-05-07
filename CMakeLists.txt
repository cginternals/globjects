
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

set(GLOW_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

include_directories(
    ${CMAKE_BINARY_DIR} # for Meta.h
    ${OPENGL_INCLUDE_DIR}
    ${GLM_INCLUDE_PATH}
    ${GLEW_INCLUDE_PATH}
    ${CMAKE_SOURCE_DIR}/libs)

set(GLOW_HEADER_GROUP "Header Files (API)")
set(GLOW_SOURCE_GROUP "Source Files")


# setup platform specifics (compile flags, etc., ...)

if(MSVC)
    message(STATUS "Configuring for platform Windows/MSVC.")
    include(${CMAKE_SOURCE_DIR}/cmake/PlatformWindowsMSVC.cmake)
elseif(WIN32 AND CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "Configuring for platform Windows/GCC.")
    include(${CMAKE_SOURCE_DIR}/cmake/PlatformWindowsGCC.cmake)
elseif(LINUX AND CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "Configuring for platform Linux/GCC.")
    include(${CMAKE_SOURCE_DIR}/cmake/PlatformLinuxGCC.cmake)
elseif(APPLE)
    message(STATUS "Configuring for platform MacOS.")
    include(${CMAKE_SOURCE_DIR}/cmake/PlatformMacOS.cmake)
else()
    # TODO configure for apple        
    message(WARNING "Unsupported platform/compiler combination")
endif()


include(cmake/Custom.cmake)

add_subdirectory("source")

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
