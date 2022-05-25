
# This config script tries to locate the project either in its source tree
# or from an install location.
# 
# Please adjust the list of submodules to search for.

# find dependencies for globjects
# - glbindings must searched for before MODULE_NAMES declaration, otherwise it
#   glbinding-config.cmake overwrites the variable MODULE_NAMES
include(CMakeFindDependencyMacro)

if (NOT TARGET glm::glm)
    find_dependency(glm)
endif ()
if (NOT TARGET glbinding::glbinding)
    find_dependency(glbinding)
endif ()
find_package(Eigen3 3.3 QUIET)


# List of modules
set(MODULE_NAMES
    globjects
)


# Macro to search for a specific module
macro(find_module FILENAME)
    if(EXISTS "${FILENAME}")
        set(MODULE_FOUND TRUE)
        include("${FILENAME}")
    endif()
endmacro()

# Macro to search for all modules
macro(find_modules PREFIX)
    foreach(module_name ${MODULE_NAMES})
        if(TARGET ${module_name})
            set(MODULE_FOUND TRUE)
        else()
            find_module("${CMAKE_CURRENT_LIST_DIR}/${PREFIX}/${module_name}/${module_name}-export.cmake")
        endif()
    endforeach(module_name)
endmacro()

# Try install location
set(MODULE_FOUND FALSE)
find_modules("cmake")

if(MODULE_FOUND)
    return()
endif()

# Try common build locations
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    find_modules("build-debug/cmake")
    find_modules("build/cmake")
else()
    find_modules("build/cmake")
    find_modules("build-debug/cmake")
endif()


# Signal success/failure to CMake
set(globjects_FOUND ${MODULE_FOUND})
