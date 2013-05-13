
# FREEGLUT_FOUND
# FREEGLUT_INCLUDE_PATH
# FREEGLUT_LIBRARY

# this favors freeglut over glut, but looks for glut as well ;)
# so default GLUT_ vars will be set if GLUT is found instead of freeglut

FIND_PATH(FREEGLUT_INCLUDE_PATH GL/freeglut.h
    $ENV{GLUT_DIR}/include
    $ENV{GLUT_HOME}/include
    $ENV{FREEGLUT_DIR}/include
    $ENV{FREEGLUT_HOME}/include
    $ENV{PROGRAMFILES}/FREEGLUT/include
    /usr/include
    /usr/local/include
    /sw/include
    /opt/local/include
    DOC "The directory where GL/freeglut.h resides")
	
IF(X64)
       
    FIND_LIBRARY(FREEGLUT_LIBRARY
	NAMES FREEGLUT freeglut GLUT glut
	PATHS
	$ENV{GLUT_DIR}/lib/x64
	$ENV{GLUT_HOME}/lib/x64
	$ENV{FREEGLUT_DIR}/lib/x64
	$ENV{FREEGLUT_HOME}/lib/x64
	/usr/lib64
	/usr/local/lib64
	/sw/lib64
	/opt/local/lib64
	DOC "The FREEGLUT library")

ELSE()

    FIND_LIBRARY(FREEGLUT_LIBRARY
	NAMES FREEGLUT freeglut GLUT glut
	PATHS
	$ENV{GLUT_DIR}/lib
	$ENV{GLUT_HOME}/lib
	$ENV{FREEGLUT_DIR}/lib
	$ENV{FREEGLUT_HOME}/lib
	/usr/lib
	/usr/local/lib
	/sw/lib
	/opt/local/lib
	DOC "The FREEGLUT library")

ENDIF()

IF(FREEGLUT_INCLUDE_PATH AND FREEGLUT_LIBRARY)
  	SET(FREEGLUT_FOUND 1 CACHE STRING "Set to 1 if FREEGLUT was found, 0 otherwise.")
ELSE()
	SET(FREEGLUT_FOUND 0 CACHE STRING "Set to 1 if FREEGLUT was found, 0 otherwise.")
  	MESSAGE(WARNING "Note: envvars like GLUT_HOME or FREEGLUT_HOME assist this script to locate glut or freeglut.")
ENDIF()

IF(NOT FREEGLUT_FOUND)
	FIND_PACKAGE(GLUT REQUIRED)
ENDIF()


MARK_AS_ADVANCED(FREEGLUT_FOUND)
