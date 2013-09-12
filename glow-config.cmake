
# GLOW_FOUND
# GLOW_INCLUDE_DIR
# GLOW_LIBRARY

# GLOW_UTILS_LIBRARY
# GLOW_WINDOW_LIBRARY

find_path(GLOW_INCLUDE_DIR glow/ref_ptr.hpp
	$ENV{GLOW_DIR}/include
	${GLOW_DIR}/include
	$ENV{PROGRAMFILES}/GLOW/include
	/usr/include
	/usr/local/include
	/sw/include
	/opt/local/include
	DOC "The directory where glow/ref_ptr.hpp resides")

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

find_library(GLOW_LIBRARY
	NAMES glow
	PATHS ${LIB_PATHS}
	DOC "The glow library")

find_library(GLOW_UTILS_LIBRARY
	NAMES glowutils
	PATHS ${LIB_PATHS}
	DOC "The glowutils library")

find_library(GLOW_WINDOW_LIBRARY
	NAMES glowwindow
	PATHS ${LIB_PATHS}
	DOC "The glowwindow library")

IF(GLOW_INCLUDE_DIR AND GLOW_LIBRARY)
    SET(GLOW_FOUND 1 CACHE STRING "Set to 1 if glow is found, 0 otherwise")
ELSE()
    SET(GLOW_FOUND 0 CACHE STRING "Set to 1 if glow is found, 0 otherwise")
    MESSAGE(WARNING "Note: an envvar like GLOW_DIR assists this script to locate glow.")
ENDIF()

MARK_AS_ADVANCED(GLOW_FOUND)
