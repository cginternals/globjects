
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

find_path(GLOW_INCLUDE_DIR glow/glow.h
	$ENV{GLOW_DIR}/include
	${GLOW_DIR}/include
	$ENV{PROGRAMFILES}/GLOW/include
	/usr/include
	/usr/local/include
	/sw/include
	/opt/local/include
	DOC "The directory where glow/glow.h resides")

set(LIB_PATHS 	
	$ENV{GLOW_DIR}/lib
	${GLOW_DIR}/lib
	/usr/lib
	/usr/local/lib
	/sw/lib
	/opt/local/lib
	/usr/lib64
	/usr/local/lib64
	/sw/lib64
	/opt/local/lib64
)

macro (find LIB_NAME HINT_PATHS)

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

	if(${${LIB_NAME_UPPER}_LIBRARY_DEBUG})
		set(${LIB_NAME_UPPER}_LIBRARIES optimized ${${LIB_NAME_UPPER}_LIBRARY} debug ${${LIB_NAME_UPPER}_LIBRARY_DEBUG})
	else()
		set(${LIB_NAME_UPPER}_LIBRARIES ${${LIB_NAME_UPPER}_LIBRARY})
	endif()

endmacro()

find(glow ${LIB_PATHS})
find(utils ${LIB_PATHS})

if(GLOW_INCLUDE_DIR AND GLOW_LIBRARY)
    set(GLOW_FOUND 1 CACHE STRING "Set to 1 if glow is found, 0 otherwise")
else()
    set(GLOW_FOUND 0 CACHE STRING "Set to 1 if glow is found, 0 otherwise")
    message(WARNING "Note: an envvar like GLOW_DIR assists this script to locate glow.")
endif()

mark_as_advanced(GLOW_FOUND)
