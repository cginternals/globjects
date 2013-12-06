
# GLOW_FOUND
# GLOW_INCLUDE_DIR

# GLOW_LIBRARY
# GLOW_LIBRARY_DEBUG
# GLOW_LIBRARIES
# GLOW_UTILS_LIBRARY
# GLOW_UTILS_LIBRARY_DEBUG
# GLOW_UTILS_LIBRARIES
# GLOW_WINDOW_LIBRARY
# GLOW_WINDOW_LIBRARY_DEBUG
# GLOW_WINDOW_LIBRARIES

if(CMAKE_CURRENT_LIST_FILE)
	get_filename_component(GLOW_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
endif()

file(TO_CMAKE_PATH "$ENV{PROGRAMFILES}" ENVPROGRAMFILES)
file(TO_CMAKE_PATH "$ENV{GLOW_DIR}" ENVGLOW_DIR)

find_path(GLOW_INCLUDE_DIR glow/glow.h
	${ENVGLOW_DIR}/include
	${GLOW_DIR}/include
	${ENVPROGRAMFILES}/glow/include
	/usr/include
	/usr/local/include
	/sw/include
	/opt/local/include
	DOC "The directory where glow/glow.h resides")

set(LIB_PATHS 	
	${GLOW_DIR}/build
	${GLOW_DIR}/build/Release
	${GLOW_DIR}/build/Debug
	${GLOW_DIR}/build-release
	${GLOW_DIR}/build-debug
	${ENVGLOW_DIR}/lib
	${GLOW_DIR}/lib
	${ENVPROGRAMFILES}/glow/lib
	/usr/lib
	/usr/local/lib
	/sw/lib
	/opt/local/lib
	/usr/lib64
	/usr/local/lib64
	/sw/lib64
	/opt/local/lib64
)

macro (find LIB_NAME)
	set(HINT_PATHS ${ARGN})

	if (${LIB_NAME} STREQUAL "glow")
		set(LIB_NAME_UPPER GLOW)
		set(LIBNAME glow)
	else()
		string(TOUPPER GLOW_${LIB_NAME} LIB_NAME_UPPER)
		set(LIBNAME glow${LIB_NAME})
	endif()

	find_library(${LIB_NAME_UPPER}_LIBRARY
		NAMES ${LIBNAME}
		PATHS ${HINT_PATHS}
		DOC "The ${LIB_NAME} library")
	find_library(${LIB_NAME_UPPER}_LIBRARY_DEBUG
		NAMES ${LIBNAME}d
		PATHS ${HINT_PATHS}
		DOC "The ${LIB_NAME} debug library")
	
	if(${LIB_NAME_UPPER}_LIBRARY AND ${LIB_NAME_UPPER}_LIBRARY_DEBUG)
		set(${LIB_NAME_UPPER}_LIBRARIES "optimized" ${${LIB_NAME_UPPER}_LIBRARY} "debug" ${${LIB_NAME_UPPER}_LIBRARY_DEBUG})
	elseif(${LIB_NAME_UPPER}_LIBRARY)
		set(${LIB_NAME_UPPER}_LIBRARIES ${${LIB_NAME_UPPER}_LIBRARY})
	elseif(${LIB_NAME_UPPER}_LIBRARY_DEBUG)
		set(${LIB_NAME_UPPER}_LIBRARIES ${${LIB_NAME_UPPER}_LIBRARY_DEBUG})
	endif()
endmacro()

find(glow ${LIB_PATHS})
find(utils ${LIB_PATHS})
#find(window ${LIB_PATHS})

find_package_handle_standard_args(GLOW DEFAULT_MSG GLOW_LIBRARIES GLOW_INCLUDE_DIR)
mark_as_advanced(GLOW_FOUND)
