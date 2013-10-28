
# GLOW_FOUND
# GLOW_INCLUDE_DIR

# GLOW_LIBRARY
# GLOW_UTILS_LIBRARY
# GLOW_WINDOW_LIBRARY

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

if (CMAKE_BUILD_TYPE STREQUAL "Release")
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
	
	if (NOT GLOW_LIBRARY)
		message(STATUS "glow release library requested but not found, search for debug version.")
		find_library(GLOW_LIBRARY
			NAMES glowd
			PATHS ${LIB_PATHS}
			DOC "The glow debug library")
	endif()
	
	if (NOT GLOW_UTILS_LIBRARY)
		message(STATUS "glowutils release library requested but not found, search for debug version.")
		find_library(GLOW_UTILS_LIBRARY
			NAMES glowutilsd
			PATHS ${LIB_PATHS}
			DOC "The glowutils debug library")
	endif()
	
	if (NOT GLOW_WINDOW_LIBRARY)
		message(STATUS "glowwindow release library requested but not found, search for debug version.")
		find_library(GLOW_WINDOW_LIBRARY
			NAMES glowwindowd
			PATHS ${LIB_PATHS}
			DOC "The glowwindow debug library")
	endif()
else()
	find_library(GLOW_LIBRARY
		NAMES glowd
		PATHS ${LIB_PATHS}
		DOC "The glow debug library")
	
	find_library(GLOW_UTILS_LIBRARY
		NAMES glowutilsd
		PATHS ${LIB_PATHS}
		DOC "The glowutils debug library")

	find_library(GLOW_WINDOW_LIBRARY
		NAMES glowwindowd
		PATHS ${LIB_PATHS}
		DOC "The glowwindow debug library")
	
	if (GLOW_LIBRARY)
		message(STATUS "Using debug version of glow")
	else()
		message(STATUS "glow debug library requested but not found, search for release version.")
		find_library(GLOW_LIBRARY
			NAMES glow
			PATHS ${LIB_PATHS}
			DOC "The glow library")
	endif()
	
	if (GLOW_UTILS_LIBRARY)
		message(STATUS "Using debug version of glowutils")
	else()
		message(STATUS "glowutils debug library requested but not found, search for release version.")
		find_library(GLOW_UTILS_LIBRARY
			NAMES glowutils
			PATHS ${LIB_PATHS}
			DOC "The glowutils library")
	endif()
	
	if (GLOW_WINDOW_LIBRARY)
		message(STATUS "Using debug version of glowwindow")
	else()
		message(STATUS "glowwindow debug library requested but not found, search for release version.")
		find_library(GLOW_WINDOW_LIBRARY
			NAMES glowwindow
			PATHS ${LIB_PATHS}
			DOC "The glowwindow library")
	endif()
endif()

if(GLOW_INCLUDE_DIR AND GLOW_LIBRARY)
    set(GLOW_FOUND 1 CACHE STRING "Set to 1 if glow is found, 0 otherwise")
else()
    set(GLOW_FOUND 0 CACHE STRING "Set to 1 if glow is found, 0 otherwise")
    message(WARNING "Note: an envvar like GLOW_DIR assists this script to locate glow.")
endif()

mark_as_advanced(GLOW_FOUND)
