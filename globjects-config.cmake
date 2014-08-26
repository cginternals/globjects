
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

# GLOBJECTS_UTILS_LIBRARY
# GLOBJECTS_UTILS_LIBRARY_RELEASE
# GLOBJECTS_UTILS_LIBRARY_DEBUG
# GLOBJECTS_UTILS_INCLUDE_DIR

# GLOBJECTS_WINDOW_LIBRARY
# GLOBJECTS_WINDOW_LIBRARY_RELEASE
# GLOBJECTS_WINDOW_LIBRARY_DEBUG
# GLOBJECTS_WINDOW_INCLUDE_DIR

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

    list(APPEND GLOBJECTS_INCLUDES ${${LIB_NAME_UPPER}_INCLUDE_DIR})
    list(APPEND GLOBJECTS_LIBRARIES ${${LIB_NAME_UPPER}_LIBRARY})

    # DEBUG
    # message("${LIB_NAME_UPPER}_INCLUDE_DIR     = ${${LIB_NAME_UPPER}_INCLUDE_DIR}")
    # message("${LIB_NAME_UPPER}_LIBRARY_RELEASE = ${${LIB_NAME_UPPER}_LIBRARY_RELEASE}")
    # message("${LIB_NAME_UPPER}_LIBRARY_DEBUG   = ${${LIB_NAME_UPPER}_LIBRARY_DEBUG}")
    # message("${LIB_NAME_UPPER}_LIBRARY         = ${${LIB_NAME_UPPER}_LIBRARY}")

endmacro()

find(base      globjects-base/globjects-base_api.h     ${LIB_PATHS})
find(globjects globjects/globjects_api.h               ${LIB_PATHS})
find(utils     globjects-utils/globjects-utils_api.h   ${LIB_PATHS})
find(window    globjects-window/globjects-window_api.h ${LIB_PATHS})

if(GLOBJECTS_BASE_LIBRARY)
  # no dependencies
endif()

if(GLOBJECTS_CORE_LIBRARY)
  # add dependencies
  if(NOT GLOBJECTS_BASE_LIBRARY)
    message(WARNING "globjects-base not found")
  endif()
  find_package(glbinding REQUIRED)
  list(APPEND GLOBJECTS_INCLUDES ${GLBINDING_INCLUDES})
  list(APPEND GLOBJECTS_LIBRARIES ${GLBINDING_LIBRARIES})
endif()

if(GLOBJECTS_UTILS_LIBRARY)
  # add dependencies
  if(NOT GLOBJECTS_CORE_LIBRARY)
    message(WARNING "globjects not found")
  endif()
endif()

if(GLOBJECTS_WINDOW_LIBRARY)
  # add dependencies
  if(NOT GLOBJECTS_BASE_LIBRARY)
    message(WARNING "globjects-base not found")
  endif()
  
  find_package(GLFW REQUIRED)
  list(APPEND GLOBJECTS_INCLUDES ${GLFW_INCLUDE_DIR})
  list(APPEND GLOBJECTS_LIBRARIES ${GLFW_LIBRARY})
endif()

# DEBUG
# message("GLOBJECTS_INCLUDES  = ${GLOBJECTS_INCLUDES}")
# message("GLOBJECTS_LIBRARIES = ${GLOBJECTS_LIBRARIES}")

find_package_handle_standard_args(GLOBJECTS DEFAULT_MSG GLOBJECTS_LIBRARIES GLOBJECTS_INCLUDES)
mark_as_advanced(GLOBJECTS_FOUND)
