
# GLOBJECTS_FOUND

# GLOBJECTS_LIBRARIES
# GLOBJECTS_INCLUDES

# GLOBJECTS_BASE_LIBRARY
# GLOBJECTS_BASE_LIBRARY_RELEASE
# GLOBJECTS_BASE_LIBRARY_DEBUG
# GLOBJECTS_BASE_INCLUDE_DIR

# GLOBJECTS_CORE_LIBRARY
# GLOBJECTS_CORE_LIBRARY_RELEASE
# GLOBJECTS_CORE_LIBRARY_DEBUG
# GLOBJECTS_CORE_INCLUDE_DIR

include(FindPackageHandleStandardArgs)

if(CMAKE_CURRENT_LIST_FILE)
    get_filename_component(GLOBJECTS_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
endif()

file(TO_CMAKE_PATH "$ENV{PROGRAMFILES}" ENVPROGRAMFILES)
file(TO_CMAKE_PATH "$ENV{GLOBJECTS_DIR}" ENVGLOBJECTS_DIR)

set(GLOBJECTS_INCLUDES "")
set(GLOBJECTS_LIBRARIES "")

set(LIB_PATHS   
    ${GLOBJECTS_DIR}/build
    ${GLOBJECTS_DIR}/build/Release
    ${GLOBJECTS_DIR}/build/Debug
    ${GLOBJECTS_DIR}/build-release
    ${GLOBJECTS_DIR}/build-debug
    ${ENVGLOBJECTS_DIR}/lib
    ${GLOBJECTS_DIR}/lib
    ${ENVPROGRAMFILES}/globjects/lib
    /usr/lib
    /usr/local/lib
    /sw/lib
    /opt/local/lib
    /usr/lib64
    /usr/local/lib64
    /sw/lib64
    /opt/local/lib64
)

macro (LIST_CONTAINS var value)
    set (${var} FALSE)
    string(TOUPPER ${value} VALUE_UPPER)
    foreach (value2 ${ARGN})
        string(TOUPPER ${value2} VALUE2_UPPER)
        if (${VALUE_UPPER} STREQUAL ${VALUE2_UPPER})
            set (${var} TRUE)
        endif ()
    endforeach ()
endmacro ()

if (globjects_FIND_COMPONENTS)
    LIST_CONTAINS(INCLUDE_GLOBJECTS_BASE base ${globjects_FIND_COMPONENTS})
    LIST_CONTAINS(INCLUDE_GLOBJECTS_CORE core ${globjects_FIND_COMPONENTS})
else ()
    set (INCLUDE_GLOBJECTS_BASE FALSE)
    set (INCLUDE_GLOBJECTS_CORE FALSE)
endif ()

if (NOT ${INCLUDE_GLOBJECTS_BASE} AND NOT ${INCLUDE_GLOBJECTS_CORE})
    message (WARNING "No components for globjects selected")
    
    return()
endif ()

macro (find LIB_NAME HEADER)
    set(HINT_PATHS ${ARGN})

    if (${LIB_NAME} STREQUAL "globjects")
        set(LIB_NAME_UPPER GLOBJECTS_CORE)
        set(LIBNAME globjects)
    else()
        string(TOUPPER GLOBJECTS_${LIB_NAME} LIB_NAME_UPPER)
        set(LIBNAME "globjects-${LIB_NAME}")
    endif()
    
    find_path(${LIB_NAME_UPPER}_INCLUDE_DIR ${HEADER}
        ${ENVGLOBJECTS_DIR}/include
        ${ENVGLOBJECTS_DIR}/source/${LIBNAME}/include
        ${GLOBJECTS_DIR}/include
        ${GLOBJECTS_DIR}/source/${LIBNAME}/include
        ${ENVPROGRAMFILES}/globjects/include
        /usr/include
        /usr/local/include
        /sw/include
        /opt/local/include
        DOC "The directory where ${header} resides")

    find_library(${LIB_NAME_UPPER}_LIBRARY_RELEASE
        NAMES ${LIBNAME}
        PATHS ${HINT_PATHS}
        DOC "The ${LIB_NAME} library")
    find_library(${LIB_NAME_UPPER}_LIBRARY_DEBUG
        NAMES ${LIBNAME}d
        PATHS ${HINT_PATHS}
        DOC "The ${LIB_NAME} debug library")
    
    if(${LIB_NAME_UPPER}_LIBRARY_RELEASE AND ${LIB_NAME_UPPER}_LIBRARY_DEBUG)
        set(${LIB_NAME_UPPER}_LIBRARY "optimized" ${${LIB_NAME_UPPER}_LIBRARY_RELEASE} "debug" ${${LIB_NAME_UPPER}_LIBRARY_DEBUG})
    elseif(${LIB_NAME_UPPER}_LIBRARY_RELEASE)
        set(${LIB_NAME_UPPER}_LIBRARY ${${LIB_NAME_UPPER}_LIBRARY_RELEASE})
    elseif(${LIB_NAME_UPPER}_LIBRARY_DEBUG)
        set(${LIB_NAME_UPPER}_LIBRARY ${${LIB_NAME_UPPER}_LIBRARY_DEBUG})
    endif()

    # DEBUG
    # message("${LIB_NAME_UPPER}_INCLUDE_DIR     = ${${LIB_NAME_UPPER}_INCLUDE_DIR}")
    # message("${LIB_NAME_UPPER}_LIBRARY_RELEASE = ${${LIB_NAME_UPPER}_LIBRARY_RELEASE}")
    # message("${LIB_NAME_UPPER}_LIBRARY_DEBUG   = ${${LIB_NAME_UPPER}_LIBRARY_DEBUG}")
    # message("${LIB_NAME_UPPER}_LIBRARY         = ${${LIB_NAME_UPPER}_LIBRARY}")

    list(APPEND GLOBJECTS_INCLUDES ${${LIB_NAME_UPPER}_INCLUDE_DIR})
    list(APPEND GLOBJECTS_LIBRARIES ${${LIB_NAME_UPPER}_LIBRARY})
endmacro()

if (${INCLUDE_GLOBJECTS_CORE})
    set (INCLUDE_GLOBJECTS_BASE TRUE)
endif ()

if (${INCLUDE_GLOBJECTS_BASE})
    find(base      globjects-base/globjects-base_api.h     ${LIB_PATHS})
endif ()
if (${INCLUDE_GLOBJECTS_CORE})
    find(globjects globjects/globjects_api.h               ${LIB_PATHS})
endif ()

# add dependencies
if (${INCLUDE_GLOBJECTS_CORE})
    if(NOT GLOBJECTS_BASE_LIBRARY)
        message(WARNING "Required globjects-base not found")
    endif()
    
    find_package(glbinding REQUIRED)
    list(APPEND GLOBJECTS_INCLUDES ${GLBINDING_INCLUDES})
    list(APPEND GLOBJECTS_LIBRARIES ${GLBINDING_LIBRARIES})
endif ()

# DEBUG
# message("GLOBJECTS_INCLUDES  = ${GLOBJECTS_INCLUDES}")
# message("GLOBJECTS_LIBRARIES = ${GLOBJECTS_LIBRARIES}")

find_package_handle_standard_args(GLOBJECTS DEFAULT_MSG GLOBJECTS_LIBRARIES GLOBJECTS_INCLUDES)
mark_as_advanced(GLOBJECTS_FOUND)
