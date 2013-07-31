
# GLOW_FOUND
# GLOW_INCLUDE_PATH
# GLOW_LIBRARY

IF(NOT GLOW_FOUND)
	IF(TARGET glow AND HAS_TARGET_PROPERTY(GLOW_INCLUDE_PATH glow))
		GET_TARGET_PROPERTY(GLOW_INCLUDE_PATH glow INCLUDE_PATH)
		SET(GLOW_LIBRARY glow)
	ELSE()
		IF(X64)

		    FIND_PATH(GLOW_INCLUDE_PATH glow/ref_ptr.hpp
			${GLOW_HOME}/include
			$ENV{GLOW_DIR}/include
			$ENV{GLOW_HOME}/include
			$ENV{PROGRAMFILES}/GLOW/include
			/usr/include
			/usr/local/include
			/sw/include
			/opt/local/include
			DOC "The directory where glow/ref_ptr.hpp resides")

		    FIND_LIBRARY(GLOW_LIBRARY
			NAMES GLOW glow
			PATHS
			${GLOW_HOME}/lib
			$ENV{GLOW_DIR}/lib
			$ENV{GLOW_HOME}/lib
			/usr/lib64
			/usr/local/lib64
			/sw/lib64
			/opt/local/lib64
			DOC "The GLOW library")

		ELSE()

		    FIND_PATH( GLOW_INCLUDE_PATH glow/ref_ptr.hpp
			${GLOW_HOME}/include
			$ENV{GLOW_DIR}/include
			$ENV{GLOW_HOME}/include
			$ENV{PROGRAMFILES}/glow/include
			/usr/include
			/usr/local/include
			/sw/include
			/opt/local/include
			DOC "The directory where glow/ref_ptr.hpp resides")

		    FIND_LIBRARY(GLOW_LIBRARY
			NAMES GLOW glow
			PATHS
			${GLOW_HOME}/lib
			$ENV{GLOW_DIR}/lib
			$ENV{GLOW_HOME}/lib		
			/usr/lib
			/usr/local/lib
			/sw/lib
			/opt/local/lib
			DOC "The GLOW library")

		ENDIF()
	ENDIF()

	IF(GLOW_INCLUDE_PATH AND GLOW_LIBRARY)
	    SET(GLOW_FOUND 1 CACHE STRING "Set to 1 if GLOW is found, 0 otherwise")
	ELSE()
	    SET(GLOW_FOUND 0 CACHE STRING "Set to 1 if GLOW is found, 0 otherwise")
	    MESSAGE(WARNING "Note: an envvar like GLOW_HOME assists this script to locate glow.")
	ENDIF()

	MARK_AS_ADVANCED( GLOW_FOUND )
ENDIF()
